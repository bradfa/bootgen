/******************************************************************************
* Copyright 2015-2022 Xilinx, Inc.
* Copyright 2022-2023 Advanced Micro Devices, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
******************************************************************************/

/*
-------------------------------------------------------------------------------
***********************************************   H E A D E R   F I L E S   ***
-------------------------------------------------------------------------------
*/
#include "readimage-zynqmp.h"
#include "authkeys.h"
#include "Keccak-compact.h"
#include "encryption-zynqmp.h"
#include "encryptutils.h"
#include "systemutils.h"
#define BITSTREAM_AUTH_CHUNK_SIZE  0x800000 //8MB = 8*1024*1024



/*******************************************************************************/
static void RearrangeEndianess(uint8_t *array, uint32_t size)
{
    uint32_t lastIndex = size - 1;
    char tempInt = 0;

    // If array is NULL, return
    if (!array)
    {
        return;
    }

    for (uint32_t loop = 0; loop <= (lastIndex / 2); loop++)
    {
        tempInt = array[loop];
        array[loop] = array[lastIndex - loop];
        array[lastIndex - loop] = tempInt;
    }
}

/*******************************************************************************/
void ZynqMpReadImage::VerifyAuthentication(bool verifyImageOption)
{
    ReadHeaderTableDetails();
    ReadBinaryFile();

    if (iHT->headerAuthCertificateWordOffset != 0)
    {
        VerifyHeaderTableSignature();
    }
    else
    {
        LOG_ERROR("Bootimage %s is not authenticated. Authentication verification cannot be done on this image.", binFilename.c_str());
    }

    VerifyPartitionSignature();
    if (authenticationVerified)
    {
        LOG_MSG("Authentication is verified on bootimage %s", binFilename.c_str());
    }
    else
    {
        LOG_ERROR("Authentication verification failed on bootimage %s", binFilename.c_str());
    }
    
}

/*******************************************************************************/
bool ZynqMpReadImage::VerifySignature(bool nist, uint8_t * data, size_t dataLength, ACKey4096* acKey, uint8_t * signature)
{
    /* Find SHA-384 hash from data */
    uint8_t hashLength = SHA3_LENGTH_BYTES;
    uint8_t* shaHash = new uint8_t[hashLength];
    if (nist)
    {
        crypto_hash_NIST_SHA3(shaHash, data, dataLength);
    }
    else
    {
        crypto_hash(shaHash, data, dataLength);
    }

    LOG_TRACE("Hash from data");
    LOG_DUMP_BYTES(shaHash, hashLength);

    rsa = RSA_new();

    BIGNUM* n = BN_new();
    n->d = (BN_ULONG*)acKey->N;
    n->dmax = RSA_4096_KEY_LENGTH / sizeof(BN_ULONG);
    n->top = RSA_4096_KEY_LENGTH / sizeof(BN_ULONG);
    n->flags = 0;
    n->neg = 0;
    
    BIGNUM* e = BN_new();
    e->d = (BN_ULONG*)acKey->E;
    e->dmax = 1;
    e->top = 1;
    e->flags = 0;
    e->neg = 0;
    
#if OPENSSL_VERSION_NUMBER > 0x10100000L
    BIGNUM *d = NULL;
    RSA_set0_key(rsa, n, e, d);
    RearrangeEndianess((uint8_t*)RSA_get0_e(rsa)->d, sizeof(uint32_t));
    RearrangeEndianess((uint8_t*)RSA_get0_n(rsa)->d, RSA_4096_KEY_LENGTH);
#else
    rsa->n = n;
    rsa->e = e;
    RearrangeEndianess((uint8_t*)rsa->e->d, sizeof(uint32_t));
    RearrangeEndianess((uint8_t*)rsa->n->d, RSA_4096_KEY_LENGTH);
#endif

    /* Find SHA-384 hash from signature */    
    uint8_t* opensslHashPadded = new uint8_t[RSA_4096_KEY_LENGTH]; //chnage key length to signature length
    
    if (RSA_public_encrypt(RSA_4096_KEY_LENGTH, signature, (unsigned char*)opensslHashPadded, rsa, RSA_NO_PADDING) < 0)
    {
        LOG_ERROR("RSA_public_encrypt error");
    }
    /* comment */
#if OPENSSL_VERSION_NUMBER > 0x10100000L
    RearrangeEndianess((uint8_t*)RSA_get0_n(rsa)->d, RSA_4096_KEY_LENGTH);
    RearrangeEndianess((uint8_t*)RSA_get0_e(rsa)->d, sizeof(uint32_t));
#else
    RearrangeEndianess((uint8_t*)rsa->n->d, RSA_4096_KEY_LENGTH);
    RearrangeEndianess((uint8_t*)rsa->e->d, sizeof(uint32_t));
#endif

    uint8_t* opensslHash = new uint8_t[hashLength];
    memcpy(opensslHash,opensslHashPadded + RSA_4096_KEY_LENGTH - hashLength, hashLength);
    LOG_TRACE("Hash from signature");
    LOG_DUMP_BYTES(opensslHash, hashLength);
    
    /* compare openssl Hash with calculated shaHash */
    if (memcmp(shaHash, opensslHash, hashLength) == 0)
    {
        delete[] shaHash;
        delete[] opensslHash;
        delete[] opensslHashPadded;
        return true;
    }
    else
    {
        delete[] shaHash;
        delete[] opensslHash;
        delete[] opensslHashPadded;
        return false;
    }    
}

/*******************************************************************************/
void ZynqMpReadImage::VerifyHeaderTableSignature()
{
    Separator();
    LOG_MSG("Verifying Header Authentication Certificate");
    uint64_t offset = 0;
    size_t result;

    FILE *binFile;
    binFile = fopen(binFilename.c_str(), "rb");

    AuthCertificate4096Structure* auth_cert = (AuthCertificate4096Structure*)(aCs.front());

    /* Verifying Header SPK Signature */
    VerifySPKSignature(auth_cert);

    /* Partition Signature should not be included for hash calculation. */
    size_t headersSize = bH->sourceOffset - bH->imageHeaderByteOffset - RSA_4096_KEY_LENGTH;
    if(bH->sourceOffset == 0)
    {
        headersSize = (pHT->partitionWordOffset * 4) - bH->imageHeaderByteOffset - RSA_4096_KEY_LENGTH;
    }
    uint8_t* tempBuffer = new uint8_t[headersSize];
    memset(tempBuffer, 0, headersSize);
    offset = bH->imageHeaderByteOffset;

    if (!(fseek(binFile, offset, SEEK_SET)))
    {
        result = fread(tempBuffer, 1, headersSize, binFile);
        if (result != headersSize)
        {
            LOG_ERROR("Error reading signature");
        }
    }
    else
    {
        LOG_ERROR("Error parsing Headers from BootImage file %s", binFilename.c_str());
    }

    bool signatureVerified = VerifySignature(true, tempBuffer, headersSize, &auth_cert->acSpk, (unsigned char*)(&auth_cert->acPartitionSignature));
    if (signatureVerified)
    {
        LOG_MSG("    Header Signature Verified");
    }
    else
    {
        LOG_MSG("    Header Signature Verification Failed");
        authenticationVerified = false;
        LOG_ERROR("Authentication verification failed on bootimage %s", binFilename.c_str());
    }

    fclose(binFile);
    delete[] tempBuffer;
}

/*******************************************************************************/
void ZynqMpReadImage::VerifySPKSignature(AuthCertificate4096Structure* auth_cert)
{
    bool nist = false;
    size_t size = 0;
    ACKey4096 key;

    if (auth_cert->acHeader & 0x80000)
    {
        nist = true;
    }
    
    // Hash of SPK - AH + SPK ID + SPK FULL + Padding
    size = sizeof(auth_cert->acHeader) + sizeof(auth_cert->spkId) + sizeof(key.N) + sizeof(key.N_extension) + sizeof(key.E) + sizeof(key.Padding);
    uint8_t* tempBuffer = new uint8_t[size];
    
    WriteLittleEndian32(tempBuffer, auth_cert->acHeader);
    WriteLittleEndian32(tempBuffer + sizeof(auth_cert->acHeader), auth_cert->spkId);
    memcpy(tempBuffer + sizeof(auth_cert->acHeader) + sizeof(auth_cert->spkId), auth_cert->acSpk.N, sizeof(key.N));
    memcpy(tempBuffer + sizeof(auth_cert->acHeader) + sizeof(auth_cert->spkId) + sizeof(key.N), auth_cert->acSpk.N_extension, sizeof(key.N_extension));
    memcpy(tempBuffer + sizeof(auth_cert->acHeader) + sizeof(auth_cert->spkId) + sizeof(key.N) + sizeof(key.N_extension), auth_cert->acSpk.E, sizeof(key.E));
    memcpy(tempBuffer + sizeof(auth_cert->acHeader) + sizeof(auth_cert->spkId) + sizeof(key.N) + sizeof(key.N_extension) + sizeof(key.E), auth_cert->acSpk.Padding, sizeof(key.Padding));
     
    bool signatureVerified = VerifySignature(nist, tempBuffer, size, &auth_cert->acPpk, (unsigned char*)(&auth_cert->acSpkSignature));
    if (signatureVerified)
    {
        LOG_MSG("    SPK Signature Verified");
    }
    else
    {
        LOG_MSG("    SPK Signature Verification Failed");
        authenticationVerified = false;
        LOG_ERROR("Authentication verification failed on bootimage %s", binFilename.c_str());
    }
    delete[] tempBuffer;
}


/*******************************************************************************/
void ZynqMpReadImage::VerifyPartitionSignature(void)
{
    size_t result;
    uint64_t offset = 0;

    std::list<uint8_t*>::iterator authCertificate = aCs.begin();
    authCertificate++;

    FILE *binFile;
    binFile = fopen(binFilename.c_str(), "rb");

    /* Partition Extraction */
    std::list<std::string>::iterator partitionName = pHTNames.begin();
    for (std::list<ZynqMpPartitionHeaderTableStructure*>::iterator partitionHdr = pHTs.begin(); partitionHdr != pHTs.end(); partitionHdr++, authCertificate++, partitionName++)
    {
        if ((*partitionHdr)->authCertificateOffset != 0)
        {
            Separator();
            LOG_MSG("Verifying Partition '%s' Authentication Certificate", (*partitionName).c_str());
            AuthCertificate4096Structure* auth_cert = (AuthCertificate4096Structure*)(*authCertificate);

            bool checkLoadAddrInBhAndPht = ((*partitionHdr)->destinationExecAddress == bH->fsblExecAddress);
            bool isItBootloader = (checkLoadAddrInBhAndPht && (bH->sourceOffset != 0));
            
            if (isItBootloader)
            {
                uint32_t bHLength = sizeof(ZynqMpBootHeaderStructure) + sizeof(RegisterInitTable);
                if (bH->fsblAttributes & 0xC0)
                {
                    bHLength += PUF_DATA_LENGTH;
                }

                uint8_t* tempBHBuffer = new uint8_t[bHLength];
                size_t result = fread(tempBHBuffer, 1, bHLength, binFile);
                if (result != bHLength)
                {
                    LOG_ERROR("Error reading boot header while verifying ");
                }

                bool signatureVerified = VerifySignature(false, tempBHBuffer, bHLength, &auth_cert->acSpk, (unsigned char*)(&auth_cert->acBhSignature));
                if (signatureVerified)
                {
                    LOG_MSG("    BootHeader Signature Verified");
                }
                else
                {
                    LOG_MSG("    BootHeader Signature Verification Failed");
                    authenticationVerified = false;
                    LOG_ERROR("Authentication verification failed on bootimage %s", binFilename.c_str());
                }
                delete[] tempBHBuffer;
            }

            /* Verifying Partition SPK Signature */
            VerifySPKSignature(auth_cert);

           /* Partition Signature should not be included for hash calculation. */
            uint32_t bufferLength = ((*partitionHdr)->totalPartitionLength * 4) - RSA_4096_KEY_LENGTH;
            bool signatureVerified = false;
            if (((((*partitionHdr)->partitionAttributes) >> PH_DEST_DEVICE_SHIFT_ZYNQMP) & PH_DEST_DEVICE_MASK_ZYNQMP) == 2)
            {
                bufferLength = ((*partitionHdr)->totalPartitionLength * 4);
            }

            uint8_t* tempBuffer = new uint8_t[bufferLength];
            memset(tempBuffer, 0, bufferLength);

            offset = (*partitionHdr)->partitionWordOffset * 4;
            if (!(fseek(binFile, offset, SEEK_SET)))
            {
                result = fread(tempBuffer, 1, bufferLength, binFile);
                if (result != bufferLength)
                {
                    LOG_ERROR("Error reading partition for hash calculation");
                }
            }
            else
            {
                LOG_ERROR("Error parsing Partitions from BootImage file %s",binFilename.c_str());
            }
           
            if (((((*partitionHdr)->partitionAttributes) >> PH_DEST_DEVICE_SHIFT_ZYNQMP) & PH_DEST_DEVICE_MASK_ZYNQMP) == 2)
            {
                uint32_t blockSize = BITSTREAM_AUTH_CHUNK_SIZE; 
                uint32_t lastBlockSize = ((*partitionHdr)->totalPartitionLength * 4) - (sizeof(AuthCertificate4096Structure) * plAcCount) - (BITSTREAM_AUTH_CHUNK_SIZE * (plAcCount - 1));

                for(int i = 0; i<plAcCount ; i++)
                {
                    if (i == plAcCount - 1 && (lastBlockSize < BITSTREAM_AUTH_CHUNK_SIZE))
                    {
                        blockSize = lastBlockSize;
                    }
                    if(i != 0)
                    {
                        authCertificate++;
                    }
                    AuthCertificate4096Structure* cert = (AuthCertificate4096Structure*)(*authCertificate);
                    uint8_t* buffer = new uint8_t[ blockSize + sizeof(AuthCertificate4096Structure) - RSA_4096_KEY_LENGTH];
                    memcpy(buffer, tempBuffer + BITSTREAM_AUTH_CHUNK_SIZE * i, blockSize);
                    memcpy(buffer + blockSize, cert, (sizeof(AuthCertificate4096Structure) - RSA_4096_KEY_LENGTH));
                    signatureVerified = VerifySignature(!isItBootloader, buffer, blockSize + (sizeof(AuthCertificate4096Structure) - RSA_4096_KEY_LENGTH), &auth_cert->acSpk, (unsigned char*)(&cert->acPartitionSignature));             
                    if(!signatureVerified)
                    {
                        LOG_MSG("    Partition Signature Verification Failed");
                        authenticationVerified = false;
                        LOG_ERROR("Authentication verification failed on bootimage %s", binFilename.c_str());
                    }
                    delete[] buffer;
                }
            }
            else
            {
                signatureVerified = VerifySignature(!isItBootloader, tempBuffer, bufferLength, &auth_cert->acSpk, (unsigned char*)(&auth_cert->acPartitionSignature));
            }
            if (signatureVerified)
            {
                LOG_MSG("    Partition Signature Verified");
            }
            else
            {
                LOG_MSG("    Partition Signature Verification Failed");
                authenticationVerified = false;
                LOG_ERROR("Authentication verification failed on bootimage %s", binFilename.c_str());
            }
            delete[] tempBuffer;
        }
        else
        {
            //EXIT
        }
    }
    fclose(binFile);
    Separator();
}

/*******************************************************************************/
void ZynqMpReadImage::VerifyEncryption(std::string nkyFile)
{
    ReadHeaderTableDetails();

    ZynqMpEncryptionContext encCtx;
    encCtx.ReadEncryptionKeyFile(nkyFile);

    uint8_t aesKey0[AES_GCM_KEY_SZ];
    memcpy_be(aesKey0, encCtx.GetAesKey(), AES_GCM_KEY_SZ);

    /* IV 0 is stored in the boot header at offset 0xA0 (secureHdrIv).
       bH is populated by fread so secureHdrIv holds raw file bytes; use memcpy. */
    uint8_t baseIv[AES_GCM_IV_SZ];
    memcpy(baseIv, bH->secureHdrIv, AES_GCM_IV_SZ);

    FILE *binFile = fopen(binFilename.c_str(), "rb");
    if (!binFile)
    {
        LOG_ERROR("Cannot read file %s", binFilename.c_str());
    }

    AesGcmEncryptionContext aesGcm;
    bool allVerified = true;
    int partIdx = 0;

    for (std::list<ZynqMpPartitionHeaderTableStructure*>::iterator it = pHTs.begin(); it != pHTs.end(); it++, partIdx++)
    {
        ZynqMpPartitionHeaderTableStructure* pHT = *it;

        if (!((pHT->partitionAttributes >> PH_ENCRYPT_SHIFT_ZYNQMP) & PH_ENCRYPT_MASK_ZYNQMP))
        {
            continue;
        }

        Separator();
        LOG_MSG("Verifying encryption of partition %d", partIdx);

        /* Per-partition effective IV: base IV with last byte incremented by partition index.
           uint8_t addition wraps at 256 with no carry, matching the same logic in
           ZynqMpEncryptionContext::ChunkifyAndEncrypt. */
        uint8_t effectiveIv[AES_GCM_IV_SZ];
        memcpy(effectiveIv, baseIv, AES_GCM_IV_SZ);
        effectiveIv[AES_GCM_IV_SZ - 1] += (uint8_t)partIdx;

        uint8_t currentKey[AES_GCM_KEY_SZ];
        uint8_t currentIv[AES_GCM_IV_SZ];
        memcpy(currentKey, aesKey0, AES_GCM_KEY_SZ);
        memcpy(currentIv, effectiveIv, AES_GCM_IV_SZ);

        uint32_t partOffset = pHT->partitionWordOffset * NUM_BYTES_PER_WORD;
        uint32_t partLength = pHT->encryptedPartitionLength * NUM_BYTES_PER_WORD;

        uint8_t* encData = new uint8_t[partLength];
        fseek(binFile, partOffset, SEEK_SET);
        size_t bytesRead = fread(encData, 1, partLength, binFile);
        if (bytesRead != partLength)
        {
            LOG_ERROR("Error reading partition %d data from %s", partIdx, binFilename.c_str());
        }

        uint32_t offset = 0;
        bool partVerified = true;
        uint32_t nextBlkWords = 0;

        /* Decrypt the secure header (encrypted with Key 0 / effective IV).
           Plaintext = [Key_next(32) | IV_next(12) | next_block_word_size(4)]. */
        uint8_t secHdrPt[SECURE_HDR_SZ];
        int ptLen = SECURE_HDR_SZ;
        int ret = aesGcm.AesGcm256Decrypt(secHdrPt, ptLen,
            currentKey, currentIv, NULL, 0,
            encData + offset, SECURE_HDR_SZ,
            encData + offset + SECURE_HDR_SZ);
        if (ret <= 0)
        {
            LOG_MSG("    Secure header GCM tag verification FAILED (wrong key or corrupted image)");
            partVerified = false;
        }
        else
        {
            LOG_MSG("    Secure header GCM tag verified");
            /* The bootloader partition stores zeros in the key field of the secure header
               as a sentinel meaning block 0 uses the same key as the secure header itself.
               Non-bootloader partitions store the actual next key here. */
            bool keyIsZero = true;
            for (int i = 0; i < AES_GCM_KEY_SZ; i++)
            {
                if (secHdrPt[i] != 0) { keyIsZero = false; break; }
            }
            if (!keyIsZero)
            {
                memcpy(currentKey, secHdrPt, AES_GCM_KEY_SZ);
            }
            memcpy(currentIv, secHdrPt + AES_GCM_KEY_SZ, AES_GCM_IV_SZ);
            nextBlkWords = ReadLittleEndian32(secHdrPt + AES_GCM_KEY_SZ + AES_GCM_IV_SZ);
        }
        offset += SECURE_HDR_SZ + AES_GCM_TAG_SZ;

        /* Walk data blocks. Each block's plaintext tail = [Key_next(32) | IV_next(12) | next_word_size(4)],
           which provides the key/IV for the following block. */
        int blockIdx = 0;
        while (partVerified && nextBlkWords > 0)
        {
            uint32_t currBlkSize = nextBlkWords * NUM_BYTES_PER_WORD;
            uint32_t ctLen = currBlkSize + SECURE_HDR_SZ;

            uint8_t* blockPt = new uint8_t[ctLen];
            ptLen = ctLen;
            ret = aesGcm.AesGcm256Decrypt(blockPt, ptLen,
                currentKey, currentIv, NULL, 0,
                encData + offset, ctLen,
                encData + offset + ctLen);
            if (ret <= 0)
            {
                LOG_MSG("    Block %d GCM tag verification FAILED", blockIdx);
                partVerified = false;
                delete[] blockPt;
                break;
            }
            LOG_MSG("    Block %d GCM tag verified", blockIdx);

            memcpy(currentKey, blockPt + currBlkSize, AES_GCM_KEY_SZ);
            memcpy(currentIv, blockPt + currBlkSize + AES_GCM_KEY_SZ, AES_GCM_IV_SZ);
            nextBlkWords = ReadLittleEndian32(blockPt + currBlkSize + AES_GCM_KEY_SZ + AES_GCM_IV_SZ);

            delete[] blockPt;
            offset += ctLen + AES_GCM_TAG_SZ;
            blockIdx++;
        }

        delete[] encData;

        if (partVerified)
        {
            LOG_MSG("Encryption verified on partition %d", partIdx);
        }
        else
        {
            LOG_MSG("Encryption verification FAILED on partition %d", partIdx);
            allVerified = false;
        }
    }

    fclose(binFile);
    Separator();

    if (allVerified)
    {
        LOG_MSG("Encryption is verified on bootimage %s", binFilename.c_str());
    }
    else
    {
        LOG_ERROR("Encryption verification failed on bootimage %s", binFilename.c_str());
    }
}
