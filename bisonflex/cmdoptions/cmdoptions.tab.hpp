/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/**
 ** \file cmdoptions.tab.hpp
 ** Define the CO::parser class.
 */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef YY_YY_CMDOPTIONS_TAB_HPP_INCLUDED
# define YY_YY_CMDOPTIONS_TAB_HPP_INCLUDED

/* "%code requires" blocks.  */
/* Line 33 of lalr1.cc  */
#line 34 "../../parser/cmdoptions.y"

    namespace CO {
        class BisonScanner;
        class FlexScanner;
        class Parser;
    }
    #include "bootimage.h"
    #include "help.h"
    #include "location.hh"


/* Line 33 of lalr1.cc  */
#line 59 "cmdoptions.tab.hpp"


#include <string>
#include <iostream>
#include "stack.hh"


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Line 33 of lalr1.cc  */
#line 24 "../../parser/cmdoptions.y"
namespace CO {
/* Line 33 of lalr1.cc  */
#line 76 "cmdoptions.tab.hpp"

  /// A Bison parser.
  class BisonParser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {
/* Line 33 of lalr1.cc  */
#line 64 "../../parser/cmdoptions.y"

    uint64_t number;
    char* cstring;


/* Line 33 of lalr1.cc  */
#line 94 "cmdoptions.tab.hpp"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef BIF::location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
     _IMAGE = 258,
     _FILL = 259,
     _O_TOK = 260,
     I = 261,
     _H = 262,
     _DEBUG_TOK = 263,
     _LEGACY = 264,
     _NONBOOTING = 265,
     _PACKAGENAME = 266,
     _BIF_HELP = 267,
     _LOG = 268,
     ERROR = 269,
     WARNING = 270,
     INFO = 271,
     DEBUG = 272,
     TRACE = 273,
     _SPLIT = 274,
     _PROCESS_BITSTREAM = 275,
     MCS = 276,
     BIN = 277,
     _OUT_TYPE = 278,
     _DUMP = 279,
     DUMP_PLM = 280,
     DUMP_PMC_CDO = 281,
     DUMP_BOOT_FILES = 282,
     _DUMP_DIR = 283,
     DUMP_SLAVE_PDIS = 284,
     DUMP_PUF_PDI = 285,
     DUMP_AC_KEYS = 286,
     _ARCH = 287,
     ZYNQ = 288,
     ZYNQMP = 289,
     VERSAL = 290,
     _R = 291,
     FPGA = 292,
     VERSALNET = 293,
     TELLURIDE = 294,
     VERSAL_2VE_2VM = 295,
     LASSEN = 296,
     LASSEN_DL9 = 297,
     SPARTANUP = 298,
     _DUAL_QSPI_MODE = 299,
     _DUAL_OSPI_MODE = 300,
     PARALLEL = 301,
     STACKED = 302,
     _W = 303,
     ON = 304,
     OFF = 305,
     _NOAUTHBLOCKS = 306,
     _ZYNQMPES1 = 307,
     _OVERLAYCDO = 308,
     _EFUSEPPKBITS = 309,
     _GENERATE_HASHES = 310,
     _PADIMAGEHEADER = 311,
     _SPKSIGNATURE = 312,
     _GENERATE_KEYS = 313,
     PEM = 314,
     RSA = 315,
     ECDSAP521 = 316,
     AUTH = 317,
     GREY = 318,
     METAL = 319,
     LMS = 320,
     _EFUSEPUFBITS = 321,
     _SECUREDEBUG = 322,
     ECDSA = 323,
     _AUTHJTAG = 324,
     _ENCRYPT = 325,
     BBRAM = 326,
     EFUSE = 327,
     _P_TOK = 328,
     _INTERFACE = 329,
     SMAPx8 = 330,
     SMAPx16 = 331,
     SMAPx32 = 332,
     SPI = 333,
     BPIx8 = 334,
     BPIx16 = 335,
     _READ = 336,
     READ_BH = 337,
     READ_IHT = 338,
     READ_IH = 339,
     READ_PHT = 340,
     READ_AC = 341,
     _VERIFY = 342,
     _VERIFYKDF = 343,
     _VERIFYENC = 344,
     _AUTH_OPTIMIZATION = 345,
     _ZYNQMPENCRDUMP = 346,
     HEXVALUE = 347,
     IDENTIFIER = 348,
     FILENAME = 349,
     QFILENAME = 350,
     HEXSTRING = 351,
     EQUALS = 352,
     HMAC = 353,
     STARTCBC = 354,
     KEY0 = 355,
     COMMA = 356,
     HBIFHELP = 357,
     HARCH = 358,
     HIMAGE = 359,
     HFILL = 360,
     HO = 361,
     HP = 362,
     HW = 363,
     HEFUSEPPKBITS = 364,
     HGENHASHES = 365,
     HLEGACY = 366,
     HPADHDR = 367,
     H_SPKSIGN = 368,
     HAUTHOPT = 369,
     HPACKAGE = 370,
     HENCRYPT = 371,
     HGENKEYS = 372,
     HDQSPI = 373,
     HLOG = 374,
     HZYNQMPES1 = 375,
     HPROCESSBIT = 376,
     HNONBOOTING = 377,
     HENCRDUMP = 378,
     HPOSTPROCESS = 379,
     HVERIFY = 380,
     HSECUREDEBUG = 381,
     HREAD = 382,
     HVERIFYKDF = 383,
     HVERIFYENC = 384,
     HDUMP = 385,
     HDUMPDIR = 386,
     HOVLCDO = 387,
     HOUTTYPE = 388,
     H_BIF_INIT = 389,
     H_BIF_UDFBH = 390,
     H_BIF_AES = 391,
     H_BIF_PPK = 392,
     H_BIF_PSK = 393,
     H_BIF_SPK = 394,
     H_BIF_SSK = 395,
     H_BIF_SPKSIGN = 396,
     H_BIF_HIVEC = 397,
     H_BIF_HDRSIGN = 398,
     H_BIF_BOOTIMAGE = 399,
     H_BIF_BL = 400,
     H_BIF_PID = 401,
     H_BIF_ENCR = 402,
     H_BIF_AUTH = 403,
     H_BIF_CHKSM = 404,
     H_BIF_ELYHNDOFF = 405,
     H_BIF_BHSIGN = 406,
     H_BIF_TCMBOOT = 407,
     H_BIF_OPTIONALDATA = 408,
     H_BIF_POWNER = 409,
     H_BIF_PRESIGN = 410,
     H_BIF_UDF = 411,
     H_BIF_XIP = 412,
     H_BIF_ALIGN = 413,
     H_BIF_OFFSET = 414,
     H_BIF_RES = 415,
     H_BIF_LOAD = 416,
     H_BIF_TZ = 417,
     H_BIF_STARTUP = 418,
     H_BIF_KEYSRC = 419,
     H_BIF_FSBLCFG = 420,
     H_BIF_BOOTDEV = 421,
     H_BIF_DESTCPU = 422,
     H_BIF_DESTDEV = 423,
     H_BIF_EL = 424,
     H_SPLIT = 425,
     H_BIF_AUTHPARAM = 426,
     H_BIF_BHKEY = 427,
     H_BIF_PFW = 428,
     H_BIF_BLOCKS = 429,
     H_BIF_METAL = 430,
     H_BIF_BHIV = 431,
     H_BIF_BOOTVEC = 432,
     H_BIF_PUFDATA = 433,
     H_BIF_PTYPE = 434,
     H_BIF_IMAGECFG = 435,
     H_BIF_PMCCONFIG = 436,
     H_BIF_AARCH32 = 437,
     H_BIF_BIGENDIAN = 438,
     H_BIF_BOOTCONFIG = 439,
     H_BIF_COPY = 440,
     H_BIF_CORE = 441,
     H_BIF_DELAY_HANDOFF = 442,
     H_BIF_DELAY_LOAD = 443,
     H_BIF_FILE = 444,
     H_BIF_ID = 445,
     H_BIF_IMAGE = 446,
     H_BIF_METAHDR = 447,
     H_BIF_NAME = 448,
     H_BIF_PARTITION = 449,
     H_BIF_SLR = 450,
     H_BIF_TYPE = 451,
     H_BIF_KEYSRCENCR = 452,
     H_BIF_PARENTID = 453,
     H_DPACM_ENABLE = 454,
     H_BIF_USERKEYS = 455,
     HVN_BIF_PCR = 456,
     HVN_BIF_PCR_MINDEX = 457,
     HV_BIF_IMAGESTORE = 458
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    BisonParser (CO::FlexScanner& scanner_yyarg, Options& options_yyarg);
    virtual ~BisonParser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef BIF::stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef BIF::stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef BIF::stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    /// Internal symbol numbers.
    typedef unsigned char token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const short int yypact_[];
    static const signed char yypact_ninf_;

    /// For a state, default reduction number.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned char yydefact_[];

    static const signed char yypgoto_[];
    static const short int yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const unsigned short int yytable_[];
    static const signed char yytable_ninf_;

    static const short int yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned char yystos_[];

    /// For a rule, its LHS.
    static const unsigned char yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[]; 

#if YYDEBUG
    /// For a symbol, its name in clear.
    static const char* const yytname_[];

    /// A type to store symbol numbers and -1.
    typedef short int rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned short int yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    ///                     If null, do not display the symbol, just free it.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

    /* User arguments.  */
    CO::FlexScanner& scanner;
    Options& options;
  };
/* Line 33 of lalr1.cc  */
#line 24 "../../parser/cmdoptions.y"
} // CO
/* Line 33 of lalr1.cc  */
#line 483 "cmdoptions.tab.hpp"



#endif /* !YY_YY_CMDOPTIONS_TAB_HPP_INCLUDED  */
