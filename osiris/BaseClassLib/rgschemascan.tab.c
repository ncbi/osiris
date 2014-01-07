/* A Bison parser, made by GNU Bison 1.875b.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yaccmod.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SEQUENCE = 258,
     ENDSEQUENCE = 259,
     BEGINELEMENT = 260,
     END = 261,
     QUOTE = 262,
     STRINGNAME = 263,
     EQUALS = 264,
     NAME = 265,
     TYPE = 266,
     MINOCCURS = 267,
     MAXOCCURS = 268,
     UNBOUNDED = 269,
     REFERENCE = 270,
     ABSTRACT = 271,
     SUBSTITUTIONGROUP = 272,
     COMPLEXSEQUENCE = 273,
     BEGINKEY = 274,
     ENDKEY = 275,
     SELECTOR = 276,
     XPATH = 277,
     FIELD = 278,
     KEYREFERENCE = 279,
     ENDKEYREFERENCE = 280,
     INCLUDE = 281,
     SCHEMALOCATION = 282,
     ENDELEMENT = 283,
     REFERTOKEY = 284,
     ENDCOMPLEXTYPE = 285,
     COMPLEXTYPE = 286,
     RIGHTBRACKET = 287,
     RGEOF = 288,
     BASE = 289,
     EXTENSION = 290,
     ENDEXTENSION = 291,
     SIMPLETYPE = 292,
     ENDSIMPLETYPE = 293,
     RESTRICTION = 294,
     ENDRESTRICTION = 295,
     VALUE = 296,
     MININCLUSIVE = 297,
     MAXINCLUSIVE = 298,
     MINEXCLUSIVE = 299,
     MAXEXCLUSIVE = 300,
     ENUMERATION = 301,
     LENGTH = 302,
     MAXLENGTH = 303,
     MINLENGTH = 304,
     TOTALDIGITS = 305,
     FRACTIONDIGITS = 306,
     WHITESPACE = 307,
     WHITESPACEDATA = 308
   };
#endif
#define SEQUENCE 258
#define ENDSEQUENCE 259
#define BEGINELEMENT 260
#define END 261
#define QUOTE 262
#define STRINGNAME 263
#define EQUALS 264
#define NAME 265
#define TYPE 266
#define MINOCCURS 267
#define MAXOCCURS 268
#define UNBOUNDED 269
#define REFERENCE 270
#define ABSTRACT 271
#define SUBSTITUTIONGROUP 272
#define COMPLEXSEQUENCE 273
#define BEGINKEY 274
#define ENDKEY 275
#define SELECTOR 276
#define XPATH 277
#define FIELD 278
#define KEYREFERENCE 279
#define ENDKEYREFERENCE 280
#define INCLUDE 281
#define SCHEMALOCATION 282
#define ENDELEMENT 283
#define REFERTOKEY 284
#define ENDCOMPLEXTYPE 285
#define COMPLEXTYPE 286
#define RIGHTBRACKET 287
#define RGEOF 288
#define BASE 289
#define EXTENSION 290
#define ENDEXTENSION 291
#define SIMPLETYPE 292
#define ENDSIMPLETYPE 293
#define RESTRICTION 294
#define ENDRESTRICTION 295
#define VALUE 296
#define MININCLUSIVE 297
#define MAXINCLUSIVE 298
#define MINEXCLUSIVE 299
#define MAXEXCLUSIVE 300
#define ENUMERATION 301
#define LENGTH 302
#define MAXLENGTH 303
#define MINLENGTH 304
#define TOTALDIGITS 305
#define FRACTIONDIGITS 306
#define WHITESPACE 307
#define WHITESPACEDATA 308




/* Copy the first part of user declarations.  */
#line 2 "rgschemascan.y"




#include <stdio.h>
#include <string.h>
//#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "rgdefs.h"
#include "rgstring.h"
#include "rgdlist.h"
#include "rgxmlschema.h"
#include "rgxmlrestrict.h"

using namespace std;

#define yyparse RGXMLSchemaSummary::YYPARSE

extern FILE *yyin, *yyout;

#define nl '\n'  

#ifndef _WINDOWS
#ifndef CYGWIN_NT
static void fcloseall()
{
  if(yyin != (FILE *)0)
  {
    fclose(yyin);
    yyin = (FILE *)0;
  }
  if(yyout != (FILE *)0)
  {
    fclose(yyout);
    yyout = (FILE *)0;
  }
}
#endif
#endif


int yylex();
int yyerror(const char *s);

RGDList NameStack;
RGDList NumberStack;
RGDList ElementNameStack;
int ElementDepth;
int ListDepth;
int SequenceDepth;
RGXmlDataEnumeration* EnumerationType;
RGDList RestrictionStack;
RGXmlDataRestriction* Restriction;

int i;
int N;
unsigned long DataSizeLimit;
RGPUnsigned ScalarDataSizeLimit;


char buffer[80]; // used for constant type strings

RGString ErrorMsg;
int status;
RGString* NewString;
RGString* NewElementName;
RGString* ParentElementName;
RGString* ReferenceElementName;
RGXMLAttributeList* CurrentAttributes;
RGXMLAttributeList* LastAttribute;
RGXMLAttributeList* NextAttribute;
RGString IncludePath;
RGXMLType* NewType;
RGXMLType* ListElement;
Boolean Success;
RGString ErrorString;

RGString TempName;
RGString TempType;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 94 "rgschemascan.y"
typedef union YYSTYPE {
  
  unsigned int   ivalue;
  double		 dvalue;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 273 "rgschemascan.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 285 "rgschemascan.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  26
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   284

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  41
/* YYNRULES -- Number of rules. */
#define YYNRULES  85
/* YYNRULES -- Number of states. */
#define YYNSTATES  262

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   308

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    34,    36,    39,    47,    55,    57,
      60,    68,    76,    84,    92,   100,   108,   116,   124,   132,
     140,   149,   157,   169,   180,   183,   186,   191,   197,   202,
     208,   213,   216,   221,   229,   232,   237,   238,   241,   243,
     245,   247,   249,   251,   253,   255,   257,   259,   261,   263,
     265,   271,   277,   283,   289,   295,   301,   307,   313,   319,
     325,   331,   337,   340,   343,   347,   349,   352,   358,   364,
     366,   368,   369,   372,   375,   378
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      55,     0,    -1,    56,    -1,    55,    56,    -1,    66,    -1,
      67,    -1,    68,    -1,    65,    -1,    57,    -1,    58,    -1,
      33,    -1,    59,    -1,    62,    -1,    37,    85,    39,    85,
      60,    40,    38,    -1,    61,    -1,    60,    61,    -1,    46,
      41,     9,     7,     8,     7,     6,    -1,    37,    85,    39,
      85,    63,    40,    38,    -1,    64,    -1,    63,    64,    -1,
      42,    41,     9,     7,     8,     7,     6,    -1,    43,    41,
       9,     7,     8,     7,     6,    -1,    44,    41,     9,     7,
       8,     7,     6,    -1,    45,    41,     9,     7,     8,     7,
       6,    -1,    47,    41,     9,     7,     8,     7,     6,    -1,
      48,    41,     9,     7,     8,     7,     6,    -1,    49,    41,
       9,     7,     8,     7,     6,    -1,    50,    41,     9,     7,
       8,     7,     6,    -1,    51,    41,     9,     7,     8,     7,
       6,    -1,    52,    41,     9,     7,    53,     7,     6,    -1,
      31,    85,    18,    74,    94,    66,     4,    30,    -1,    31,
      85,    18,    94,    66,     4,    30,    -1,    31,    85,    35,
      85,    18,    74,    94,    66,     4,    36,    30,    -1,    31,
      85,    35,    85,    18,    94,    66,     4,    36,    30,    -1,
       5,    84,    -1,     5,     6,    -1,     5,    85,    91,    28,
      -1,     5,    85,    68,    91,    28,    -1,     5,    85,    68,
      28,    -1,     5,    85,    69,    91,    28,    -1,     5,    85,
      69,    28,    -1,    26,    84,    -1,    31,    85,    86,    30,
      -1,    31,    85,    35,    85,    86,    36,    30,    -1,    31,
      84,    -1,    31,    32,    86,    30,    -1,    -1,    70,    71,
      -1,    72,    -1,    73,    -1,    74,    -1,    75,    -1,    76,
      -1,    77,    -1,    78,    -1,    79,    -1,    80,    -1,    81,
      -1,    82,    -1,    83,    -1,    10,     9,     7,     8,     7,
      -1,    11,     9,     7,     8,     7,    -1,    12,     9,     7,
       8,     7,    -1,    13,     9,     7,     8,     7,    -1,    13,
       9,     7,    14,     7,    -1,    15,     9,     7,     8,     7,
      -1,    16,     9,     7,     8,     7,    -1,    17,     9,     7,
       8,     7,    -1,    27,     9,     7,     8,     7,    -1,    22,
       9,     7,     8,     7,    -1,    29,     9,     7,     8,     7,
      -1,    34,     9,     7,     8,     7,    -1,    70,     6,    -1,
      70,    32,    -1,     3,    87,     4,    -1,    66,    -1,    87,
      66,    -1,    19,    85,    92,    93,    20,    -1,    24,    85,
      92,    93,    25,    -1,    88,    -1,    89,    -1,    -1,    91,
      90,    -1,    21,    84,    -1,    23,    84,    -1,    76,    32,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   128,   128,   129,   132,   133,   134,   135,   136,   137,
     140,   147,   148,   152,   227,   228,   231,   249,   310,   311,
     314,   337,   360,   383,   406,   425,   444,   463,   482,   501,
     509,   580,   651,   725,   801,   862,   868,   922,   928,   934,
     995,  1053,  1074,  1124,  1190,  1221,  1228,  1229,  1234,  1235,
    1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,
    1248,  1273,  1296,  1304,  1312,  1326,  1351,  1374,  1396,  1418,
    1440,  1462,  1486,  1493,  1500,  1514,  1515,  1521,  1527,  1533,
    1534,  1537,  1538,  1541,  1547,  1553
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEQUENCE", "ENDSEQUENCE", "BEGINELEMENT", 
  "END", "QUOTE", "STRINGNAME", "EQUALS", "NAME", "TYPE", "MINOCCURS", 
  "MAXOCCURS", "UNBOUNDED", "REFERENCE", "ABSTRACT", "SUBSTITUTIONGROUP", 
  "COMPLEXSEQUENCE", "BEGINKEY", "ENDKEY", "SELECTOR", "XPATH", "FIELD", 
  "KEYREFERENCE", "ENDKEYREFERENCE", "INCLUDE", "SCHEMALOCATION", 
  "ENDELEMENT", "REFERTOKEY", "ENDCOMPLEXTYPE", "COMPLEXTYPE", 
  "RIGHTBRACKET", "RGEOF", "BASE", "EXTENSION", "ENDEXTENSION", 
  "SIMPLETYPE", "ENDSIMPLETYPE", "RESTRICTION", "ENDRESTRICTION", "VALUE", 
  "MININCLUSIVE", "MAXINCLUSIVE", "MINEXCLUSIVE", "MAXEXCLUSIVE", 
  "ENUMERATION", "LENGTH", "MAXLENGTH", "MINLENGTH", "TOTALDIGITS", 
  "FRACTIONDIGITS", "WHITESPACE", "WHITESPACEDATA", "$accept", 
  "declarations", "declaration", "endoffile", "simpleTypes", 
  "enumerationType", "enumerations", "enumeration", "restrictedDataType", 
  "restrictions", "restriction", "list", "element", "include", 
  "complexDef", "complexDefNoName", "attributes", "attribute", 
  "nameAttribute", "typeAttribute", "minAttribute", "maxAttribute", 
  "listAttribute", "refAttribute", "absAttribute", "subsAttribute", 
  "locAttribute", "xpathAttribute", "reftoKeyAttribute", "baseAttribute", 
  "attributesEnd", "attributesRightBracket", "sequence", "elements", 
  "key", "keyreference", "keydefinition", "keydefinitions", "selector", 
  "field", "listAttributeBracket", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    54,    55,    55,    56,    56,    56,    56,    56,    56,
      57,    58,    58,    59,    60,    60,    61,    62,    63,    63,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      65,    65,    65,    65,    66,    66,    66,    66,    66,    66,
      66,    67,    68,    68,    68,    69,    70,    70,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    87,    88,    89,    90,
      90,    91,    91,    92,    93,    94
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     7,     1,     2,     7,     7,     1,     2,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       8,     7,    11,    10,     2,     2,     4,     5,     4,     5,
       4,     2,     4,     7,     2,     4,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     2,     2,     3,     1,     2,     5,     5,     1,
       1,     0,     2,     2,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,    46,    46,    46,    10,    46,     0,     2,     8,     9,
      11,    12,     7,     4,     5,     6,    35,     0,    34,    81,
       0,    41,    44,     0,     0,     0,     1,     3,    72,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    73,
       0,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    46,    81,    81,     0,     0,     0,
      46,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
      46,    46,    36,    79,    80,    82,    75,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    37,    39,
       0,     0,    74,    76,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    45,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      15,     0,    19,    60,    61,    62,    63,    64,    65,    66,
      67,    69,    68,    70,    71,    83,    46,     0,     0,     0,
      31,     0,     0,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,    17,    84,    77,    78,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      32,    20,    21,    22,    23,    16,    24,    25,    26,    27,
      28,    29
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     6,     7,     8,     9,    10,   131,   132,    11,   133,
     134,    12,    13,    14,    15,    56,    24,    41,    42,    43,
      44,    45,    90,    47,    48,    49,    50,    51,    52,    53,
      22,    19,    61,    87,    83,    84,    85,    57,   150,   187,
      91
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -80
static const short yypact[] =
{
      10,    24,   -80,   -80,   -80,   -80,    11,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,    79,   -80,    21,
     106,   -80,   -80,    14,   142,   -26,   -80,   -80,   -80,    36,
      52,    55,    57,    58,    60,    61,    62,    63,    64,   -80,
      65,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,    43,    49,    50,    -5,    74,     8,
     -80,    56,   -80,    73,    75,    80,    81,    86,    90,    91,
      92,    93,    95,    97,   102,    15,   -80,     5,   -80,     7,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,    51,    98,    96,
      82,    74,     9,   -80,   158,   107,   112,   116,    32,   117,
     118,   119,   121,   122,   123,   124,   104,   -80,   -80,   -80,
     115,   115,   -80,   -80,   130,    74,   -80,   135,     8,   105,
     101,   103,   108,   109,   120,   125,   126,   127,   129,   131,
     132,   -18,   -80,   147,   -80,   136,   138,   139,   140,   149,
     153,   155,   156,   168,   170,   171,   172,   -80,   102,   -80,
     157,   157,   151,   177,   152,    96,    74,   154,   174,   184,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   148,
     -80,   182,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   201,   197,   193,
     -80,    74,   220,   -80,   218,   219,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   -80,   -80,   -80,   -80,   -80,
     -80,   233,   191,   230,   231,   232,   234,   235,   236,   237,
     238,   239,   240,   188,   213,   241,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   242,   -80,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   267,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
     -80,   -80,   268,   -80,   -80,   -80,   -80,   144,   -80,   -80,
     143,   -80,   -53,   -80,   265,   -80,    -1,   -80,   -80,   -80,
     -50,   -80,    34,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
       2,     3,   -67,   -80,   -80,   -80,   -80,     4,   166,   128,
     -79
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned short yytable[] =
{
      17,    20,    17,    18,    21,    86,    23,   106,    25,    89,
     115,    26,    58,    62,    80,     1,     1,    58,    58,    81,
      31,    88,   169,    82,    80,   119,    80,   118,   124,    81,
      16,    81,    59,   108,   113,   109,     2,     2,   117,   156,
     138,     3,     3,     4,     4,    63,   139,     5,     5,    60,
     107,    46,    54,    17,    46,   112,     1,    75,    46,    77,
      79,    64,   153,    92,    65,    94,    66,    67,   155,    68,
      69,    70,    71,    72,    73,    74,   191,    76,    78,     1,
      95,   119,    96,   110,   111,    28,    93,    97,    98,    29,
      30,    31,    32,    99,    33,    34,    35,   100,   101,   102,
     103,    36,   104,   192,   105,    58,    37,   114,    38,    88,
     148,    39,    28,    40,   116,   135,    29,    30,    31,    32,
     136,    33,    34,    35,   137,   140,   141,   142,    36,   143,
     144,   145,   146,    37,   147,    38,   149,   152,   211,   154,
      40,   157,   158,   173,   159,   174,   175,   176,    20,   160,
     161,   185,    29,    30,    31,    32,   177,    33,    34,    35,
     178,   162,   179,   180,    36,   139,   163,   164,   165,    37,
     166,    38,   167,   168,    39,   181,    40,   182,   183,   184,
     186,   189,   190,   194,   193,    20,   205,   171,   207,   120,
     121,   122,   123,   195,   125,   126,   127,   128,   129,   130,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     206,   208,   209,   210,   212,   213,   214,   225,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   226,   227,
     228,   236,   229,   230,   231,   232,   233,   234,   235,   237,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   238,   250,   261,    27,   170,   172,   151,     0,   188,
       0,     0,     0,     0,    55
};

static const short yycheck[] =
{
       1,     2,     3,     1,     2,    58,     3,    74,     5,    59,
      89,     0,     3,    39,    19,     5,     5,     3,     3,    24,
      12,    13,    40,    28,    19,    92,    19,    18,    46,    24,
       6,    24,    18,    28,    87,    28,    26,    26,    91,   118,
       8,    31,    31,    33,    33,     9,    14,    37,    37,    35,
      35,    17,    31,    54,    20,     4,     5,    54,    24,    55,
      56,     9,   115,    60,     9,    62,     9,     9,   118,     9,
       9,     9,     9,     9,     9,    32,   155,    28,    28,     5,
       7,   148,     7,    80,    81,     6,    30,     7,     7,    10,
      11,    12,    13,     7,    15,    16,    17,     7,     7,     7,
       7,    22,     7,   156,     7,     3,    27,     9,    29,    13,
     107,    32,     6,    34,    32,     8,    10,    11,    12,    13,
       8,    15,    16,    17,     8,     8,     8,     8,    22,     8,
       8,     8,     8,    27,    30,    29,    21,     7,   191,     4,
      34,    36,    41,     7,    41,     7,     7,     7,   149,    41,
      41,   149,    10,    11,    12,    13,     7,    15,    16,    17,
       7,    41,     7,     7,    22,    14,    41,    41,    41,    27,
      41,    29,    41,    41,    32,     7,    34,     7,     7,     7,
      23,     4,    30,     9,    30,   186,    38,    40,   186,    42,
      43,    44,    45,     9,    47,    48,    49,    50,    51,    52,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     9,     9,     9,     9,     9,     9,     9,     9,     9,
      38,    20,    25,    30,     4,     7,     7,    36,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     4,     8,     8,
       8,    53,     8,     8,     8,     8,     8,     8,     8,    36,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       7,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,    30,    30,     6,     6,   131,   133,   111,    -1,   151,
      -1,    -1,    -1,    -1,    19
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     5,    26,    31,    33,    37,    55,    56,    57,    58,
      59,    62,    65,    66,    67,    68,     6,    70,    84,    85,
      70,    84,    84,    85,    70,    85,     0,    56,     6,    10,
      11,    12,    13,    15,    16,    17,    22,    27,    29,    32,
      34,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    31,    68,    69,    91,     3,    18,
      35,    86,    39,     9,     9,     9,     9,     9,     9,     9,
       9,     9,     9,     9,    32,    85,    28,    91,    28,    91,
      19,    24,    28,    88,    89,    90,    66,    87,    13,    74,
      76,    94,    85,    30,    85,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,    86,    35,    28,    28,
      85,    85,     4,    66,     9,    94,    32,    66,    18,    86,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    60,    61,    63,    64,     8,     8,     8,     8,    14,
       8,     8,     8,     8,     8,     8,     8,    30,    85,    21,
      92,    92,     7,    66,     4,    74,    94,    36,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    40,
      61,    40,    64,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,    84,    23,    93,    93,     4,
      30,    94,    66,    30,     9,     9,     9,     9,     9,     9,
       9,     9,     9,     9,     9,    38,    38,    84,    20,    25,
      30,    66,     4,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     4,    36,     8,     8,     8,     8,
       8,     8,     8,     8,     8,     8,    53,    36,    30,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
      30,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
/* #ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else 
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 10:
#line 140 "rgschemascan.y"
    {
					
				//	cout << "Found end of file" << endl;
					return 0;
				;}
    break;

  case 13:
#line 153 "rgschemascan.y"
    {
					
					NewType = new RGXMLSimpleEnumerationType ();
					NewType->SetEnumeration (EnumerationType);
					
					if (EnumerationType == NULL) {
					
						ErrorString = "****error in enumeration type:  no values found";
						yyerror (ErrorString);
						return 0;
					}
					
					//
					//  NextAttribute should contain the restriction base
					//
					
					NextAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					
					if (NextAttribute == NULL) {
					
						ErrorString = "****error in enumeration type info:  no attributes found for base";
						yyerror (ErrorString);
						return 0;
					}
					
					//
					//  LastAttribute should contain the simple name
					//
					
					LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					
					if (LastAttribute == NULL) {
					
						ErrorString = "****error in enumeration type info:  no attributes found for name";
						yyerror (ErrorString);
						return 0;
					}
					
					NewType->ResetName (LastAttribute->GetName ());
					NewType->ResetBase (NextAttribute->GetBase ());
					
					if (RGXMLSchemaSummary::DebugFlag) {
					
						cout << "Found simple type with name:  " << LastAttribute->GetName ();
						cout << " with base:  " << NextAttribute->GetBase () << endl;
					}
					
					delete LastAttribute;
					delete NextAttribute;
					
					N = EnumerationType->NumberOfItems ();
					
					if (RGXMLSchemaSummary::DebugFlag) {
					
						for (i=0; i<N; i++)
							cout << "    Value = " << EnumerationType->GetStringValue (i);
					}
						
					ResultList->Append (NewType);
					
					ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
					if (ElementDepth > 0) {
					
						ErrorString = "****error in building simple enumeration type named " + NewType->GetName ();
						ErrorString += ":  must not have nested elements with simple types";
						yyerror (ErrorString);
						return 0;
					}
					
					EnumerationType = NULL;  // reset to begin again
				;}
    break;

  case 16:
#line 231 "rgschemascan.y"
    {

					if (EnumerationType == NULL)
						EnumerationType = new RGXmlDataEnumeration;
					
					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in retrieving enumeration value:  no string value found";
						yyerror (ErrorString);
						return 0;
					}
					
					EnumerationType->AddValue (NewString);
				;}
    break;

  case 17:
#line 250 "rgschemascan.y"
    {
				
						NewType = new RGXMLRestrictedSimpleType ();
						
						//
						//  NextAttribute should contain the restriction base
						//
						
						NextAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
						
						if (NextAttribute == NULL) {
					
							ErrorString = "****error in simple type info:  no attributes found for base";
							yyerror (ErrorString);
							return 0;
						}
						
						//
						//  LastAttribute should contain the simple name
						//
						
						LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
						
						if (LastAttribute == NULL) {
					
							ErrorString = "****error in simple type info:  no attributes found for name";
							yyerror (ErrorString);
							return 0;
						}
						
						if (RGXMLSchemaSummary::DebugFlag) {
						
							cout << "Found simple type with name:  " << LastAttribute->GetName ();				
							cout << " with base:  " << NextAttribute->GetBase () << endl;
						}
						
						NewType->ResetName (LastAttribute->GetName ());
						NewType->ResetBase (NextAttribute->GetBase ());
						delete NextAttribute;
						delete LastAttribute;
						
						//
						// Then, create a simple type with the found name and copy RestrictionList into it
						//
						
						NewType->AddRestrictionList (RestrictionStack);
						ResultList->Append (NewType);
						
						ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
						if (ElementDepth > 0) {
						
							ErrorString = "****error in building simple restriction type named " + NewType->GetName ();
							ErrorString += ":  must not have nested elements with simple types";
							yyerror (ErrorString);
							return 0;
						}
					;}
    break;

  case 20:
#line 314 "rgschemascan.y"
    {
			
				NewString = (RGString*) NameStack.GetFirst ();
				
				if (NewString == NULL) {
				
					ErrorString = "****error in retrieving minInclusive value:  no string value found";
					yyerror (ErrorString);
					return 0;
				}
				
				Restriction = new RGXmlDataMinInclusive ();
				
				if (!Restriction->SetStringValue (NewString)) {
				
					ErrorString = "****error in setting minInclusive value:  could not set string value";
					yyerror (ErrorString);
					return 0;
				}
				
				RestrictionStack.Append (Restriction);
			;}
    break;

  case 21:
#line 337 "rgschemascan.y"
    {
			
				NewString = (RGString*) NameStack.GetFirst ();
				
				if (NewString == NULL) {
				
					ErrorString = "****error in retrieving maxInclusive value:  no string value found";
					yyerror (ErrorString);
					return 0;
				}
				
				Restriction = new RGXmlDataMaxInclusive ();
				
				if (!Restriction->SetStringValue (NewString)) {
				
					ErrorString = "****error in setting maxInclusive value:  could not set string value";
					yyerror (ErrorString);
					return 0;
				}
				
				RestrictionStack.Append (Restriction);
			;}
    break;

  case 22:
#line 360 "rgschemascan.y"
    {
			
				NewString = (RGString*) NameStack.GetFirst ();
				
				if (NewString == NULL) {
				
					ErrorString = "****error in retrieving minExclusive value:  no string value found";
					yyerror (ErrorString);
					return 0;
				}
				
				Restriction = new RGXmlDataMinExclusive ();
				
				if (!Restriction->SetStringValue (NewString)) {
				
					ErrorString = "****error in setting minExclusive value:  could not set string value";
					yyerror (ErrorString);
					return 0;
				}
				
				RestrictionStack.Append (Restriction);
			;}
    break;

  case 23:
#line 383 "rgschemascan.y"
    {
			
				NewString = (RGString*) NameStack.GetFirst ();
				
				if (NewString == NULL) {
				
					ErrorString = "****error in retrieving maxExclusive value:  no string value found";
					yyerror (ErrorString);
					return 0;
				}
				
				Restriction = new RGXmlDataMaxExclusive ();
				
				if (!Restriction->SetStringValue (NewString)) {
				
					ErrorString = "****error in setting maxExclusive value:  could not set string value";
					yyerror (ErrorString);
					return 0;
				}
				
				RestrictionStack.Append (Restriction);
			;}
    break;

  case 24:
#line 406 "rgschemascan.y"
    {
			
				NewString = (RGString*) NameStack.GetFirst ();
				
				if (NewString == NULL) {
				
					ErrorString = "****error in retrieving length value:  no string value found";
					yyerror (ErrorString);
					return 0;
				}
				
				DataSizeLimit = NewString->ConvertToUnsignedLong ();
				delete NewString;
				Restriction = new RGXmlDataLength ();
				ScalarDataSizeLimit = DataSizeLimit;
				Restriction->SetLimit (ScalarDataSizeLimit);			
				RestrictionStack.Append (Restriction);
			;}
    break;

  case 25:
#line 425 "rgschemascan.y"
    {
			
				NewString = (RGString*) NameStack.GetFirst ();
				
				if (NewString == NULL) {
				
					ErrorString = "****error in retrieving maxLength value:  no string value found";
					yyerror (ErrorString);
					return 0;
				}
				
				DataSizeLimit = NewString->ConvertToUnsignedLong ();
				delete NewString;
				Restriction = new RGXmlDataMaxLength ();
				ScalarDataSizeLimit = DataSizeLimit;
				Restriction->SetLimit (ScalarDataSizeLimit);			
				RestrictionStack.Append (Restriction);
			;}
    break;

  case 26:
#line 444 "rgschemascan.y"
    {
			
				NewString = (RGString*) NameStack.GetFirst ();
				
				if (NewString == NULL) {
				
					ErrorString = "****error in retrieving minLength value:  no string value found";
					yyerror (ErrorString);
					return 0;
				}
				
				DataSizeLimit = NewString->ConvertToUnsignedLong ();
				delete NewString;
				Restriction = new RGXmlDataMinLength ();
				ScalarDataSizeLimit = DataSizeLimit;
				Restriction->SetLimit (ScalarDataSizeLimit);			
				RestrictionStack.Append (Restriction);
			;}
    break;

  case 27:
#line 463 "rgschemascan.y"
    {
			
				NewString = (RGString*) NameStack.GetFirst ();
				
				if (NewString == NULL) {
				
					ErrorString = "****error in retrieving totalDigits value:  no string value found";
					yyerror (ErrorString);
					return 0;
				}
				
				DataSizeLimit = NewString->ConvertToUnsignedLong ();
				delete NewString;
				Restriction = new RGXmlDataTotalDigits ();
				ScalarDataSizeLimit = DataSizeLimit;
				Restriction->SetLimit (ScalarDataSizeLimit);			
				RestrictionStack.Append (Restriction);
			;}
    break;

  case 28:
#line 482 "rgschemascan.y"
    {
			
				NewString = (RGString*) NameStack.GetFirst ();
				
				if (NewString == NULL) {
				
					ErrorString = "****error in retrieving fractionDigits value:  no string value found";
					yyerror (ErrorString);
					return 0;
				}
				
				DataSizeLimit = NewString->ConvertToUnsignedLong ();
				delete NewString;
				Restriction = new RGXmlDataFractionDigits ();
				ScalarDataSizeLimit = DataSizeLimit;
				Restriction->SetLimit (ScalarDataSizeLimit);			
				RestrictionStack.Append (Restriction);
			;}
    break;

  case 29:
#line 501 "rgschemascan.y"
    {
	
					// Just ignore this for now
					//cout << "Found whitespace restriction" << endl;
			
			;}
    break;

  case 30:
#line 510 "rgschemascan.y"
    {

			//	cout << "Found list with min attribute directive" << endl;
				ListDepth--;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex list:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  nesting of unbounded list declarations not allowed";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth < 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  unmatched unbounded list identifiers";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLComplexList ();
								
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  attributes incomplete or missing";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  must not have nested elements with unbounded lists";
					yyerror (ErrorString);
					return 0;
				}
				
				ListElement = (RGXMLType*)ListElementStack.GetFirst ();
				
				if (ListElement == NULL) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  could not find list element declaration";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType->SetListElement (ListElement); // This function makes a copy of 'ListElement'; clean up after
				delete ListElement;
				
				delete NewElementName;
				ResultList->Append (NewType);
			;}
    break;

  case 31:
#line 581 "rgschemascan.y"
    {

			//	cout << "Found list directive" << endl;
				ListDepth--;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex list:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  nesting of unbounded list declarations not allowed";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth < 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  unmatched unbounded list identifiers";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLComplexList ();
								
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  attributes incomplete or missing";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  must not have nested elements with unbounded lists";
					yyerror (ErrorString);
					return 0;
				}
				
				ListElement = (RGXMLType*)ListElementStack.GetFirst ();
				
				if (ListElement == NULL) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  could not find list element declaration";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType->SetListElement (ListElement); // This function makes a copy of 'ListElement'; clean up after
				delete ListElement;
				
				delete NewElementName;
				ResultList->Append (NewType);
			;}
    break;

  case 32:
#line 652 "rgschemascan.y"
    {

			//	cout << "Found list with min attribute directive" << endl;
				ListDepth--;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex list with extension:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  nesting of unbounded list declarations not allowed";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth < 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  unmatched unbounded list identifiers";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLComplexList ();
				NextAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				LastAttribute->SetBaseFromAttributeList (NextAttribute);
				delete NextAttribute;
								
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  must not have nested elements with unbounded lists";
					yyerror (ErrorString);
					return 0;
				}
				
				ListElement = (RGXMLType*)ListElementStack.GetFirst ();
				
				if (ListElement == NULL) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  could not find list element declaration";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType->SetListElement (ListElement);
				delete ListElement;
				
				delete NewElementName;
				ResultList->Append (NewType);
			;}
    break;

  case 33:
#line 726 "rgschemascan.y"
    {

			//	cout << "Found list directive" << endl;
				ListDepth--;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex list with extension:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  nesting of unbounded list declarations not allowed";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth < 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  unmatched unbounded list identifiers";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLComplexList ();
				NextAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				LastAttribute->SetBaseFromAttributeList (NextAttribute);
				delete NextAttribute;
								
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  must not have nested elements with unbounded lists";
					yyerror (ErrorString);
					return 0;
				}
				
				ListElement = (RGXMLType*)ListElementStack.GetFirst ();
				
				if (ListElement == NULL) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  could not find list element declaration";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType->SetListElement (ListElement);
				delete ListElement;
				
				delete NewElementName;
				ResultList->Append (NewType);
			;}
    break;

  case 34:
#line 801 "rgschemascan.y"
    {

			//	cout << "Found element with attributes only" << endl;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building simple element:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLElementType ();
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?
				
				if (ElementDepth > 0) {
				
					ParentElementName = (RGString*) ElementNameStack.First ();  // this should be parent element's name
					
					if (ParentElementName == NULL) {
					
						ErrorString = "****error in building element named " + *NewElementName;
						ErrorString += ":  parent name is missing";
						yyerror (ErrorString);
						return 0;
					}
					
					Success = NewType->SetContainerName (*ParentElementName);
					
					if (!Success) {
					
						ErrorString = "****error in setting parent name:  " + *ParentElementName;
						yyerror (ErrorString);
						return 0;
					}
				}
				
				delete NewElementName;
				
				if (SequenceDepth > 0)				
					SequencedElementsStack.Append (NewType);
				
				else if (ListDepth > 0)
					ListElementStack.Prepend (NewType);
				
				else
					ResultList->Append (NewType);
			;}
    break;

  case 35:
#line 862 "rgschemascan.y"
    {
	
			//	cout << "Found extremely simple element, with no info" << endl;
				ElementDepth--;  // there is nothing to do with this; it contains no information
			;}
    break;

  case 36:
#line 868 "rgschemascan.y"
    {
				
			//	cout << "Found element with attributes and keys and/or key references" << endl;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building element:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				// ignore key data for now
				
				NewType = new RGXMLElementType ();
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?
				
				if (ElementDepth > 0) {
				
					ParentElementName = (RGString*) ElementNameStack.First ();  // this should be previous element's name
					Success = NewType->SetContainerName (*ParentElementName);
					
					if (!Success) {
					
						ErrorString = "****error in setting parent name:  " + *ParentElementName;
						yyerror (ErrorString);
						return 0;
					}
				}
				
				delete NewElementName;
				
				if (SequenceDepth > 0)				
					SequencedElementsStack.Append (NewType);
				
				else if (ListDepth > 0)
					ListElementStack.Prepend (NewType);
				
				else
					ResultList->Append (NewType);
			;}
    break;

  case 37:
#line 922 "rgschemascan.y"
    {

				// How can a complex type with name appear as a sub-element of a named element?  Don't think so...
				cout << "***Found complex element, possibly including attributes, keys, key references and subelements" << endl;
			;}
    break;

  case 38:
#line 928 "rgschemascan.y"
    {
	
				// How can a complex type with name appear as a sub-element of a named element?  Don't think so...
				cout << "****Found complex element, possibly including attributes, keys, and subelements:  NO REFS" << endl;
	;}
    break;

  case 39:
#line 934 "rgschemascan.y"
    {

				// the complexDefNoName is, essentially, a sequence, so we treat it like one; ignore key definitions for now
				
			//	cout << "Found complex element, possibly including attributes, keys, key references and subelements" << endl;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				//
				// There should be no parent for this kind of element
				//
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex element with keys:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				SequenceDepth--;
				NewType = new RGXMLElementType ();
				
				//
				// complexDefNoName has no name or other attributes, so get name from element attributes
				//
				
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  must not have nested elements with sequences";
					yyerror (ErrorString);
					return 0;
				}
				
				Success = NewType->AddSubElementList (SequencedElementsStack);
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  could not transfer element sequence stack";
					yyerror (ErrorString);
					return 0;
				}
				
				delete NewElementName;
				ResultList->Append (NewType);
			;}
    break;

  case 40:
#line 995 "rgschemascan.y"
    {
	
				// the complexDefNoName is, essentially, a sequence, so we treat it like one; ignore key definitions for now
				
			//	cout << "Found complex element, possibly including attributes, keys, and subelements:  NO REFS" << endl;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex element:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				SequenceDepth--;
				NewType = new RGXMLElementType ();
				
				//
				// complexDefNoName has no name or other attributes, so get name from element attributes
				//
				
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  must not have nested elements with sequences";
					yyerror (ErrorString);
					return 0;
				}
				
				Success = NewType->AddSubElementList (SequencedElementsStack);
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  could not transfer element sequence stack";
					yyerror (ErrorString);
					return 0;
				}
				
				delete NewElementName;
				ResultList->Append (NewType);
	;}
    break;

  case 41:
#line 1053 "rgschemascan.y"
    {

			//	cout << "Found include directive" << endl;
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				
				if (LastAttribute == NULL) {
				
					yyerror ("****Warning****No attributes for include statement");
				}
				
				else {
				
					LastAttribute->GetIncludePath (IncludePath);
					delete LastAttribute;
					NewString = new RGString (IncludePath);
					LocalSchemasNeeded.Append (NewString);
				}
			;}
    break;

  case 42:
#line 1074 "rgschemascan.y"
    {

				//	cout << "Found complex type definition, possibly including sequence" << endl;
					NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
					
					if (NewElementName == NULL) {
					
						ErrorString = "****error in building complex type:  name not in name stack";
						yyerror (ErrorString);
						return 0;
					}
					
					SequenceDepth--;
					NewType = new RGXMLComplexType ();
					LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					Success = NewType->SetAttributes (LastAttribute);
					delete LastAttribute;
					
					if (!Success) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  attributes incomplete";
						yyerror (ErrorString);
						return 0;
					}
					
					ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
					
					if (ElementDepth > 0) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  named complex type may not have a parent element";
						yyerror (ErrorString);
						return 0;
					}
					
					Success = NewType->AddSubElementList (SequencedElementsStack);
					
					if (!Success) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  could not transfer element sequence stack";
						yyerror (ErrorString);
						return 0;
					}
					
					delete NewElementName;
					ResultList->Append (NewType);
				;}
    break;

  case 43:
#line 1124 "rgschemascan.y"
    {
	
				//	cout << "Found complex type definition with attributes, extension base and sequence" << endl;
					NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
					
					if (NewElementName == NULL) {
					
						ErrorString = "****error in building complex type:  name not in name stack";
						yyerror (ErrorString);
						return 0;
					}
					
					SequenceDepth--;
					NewType = new RGXMLComplexType ();
					NextAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					
					if ((NextAttribute == NULL) || (LastAttribute == NULL)) {
					
						ErrorString = "****error in building complex type with extension named " + *NewElementName;
						ErrorString += ":  could not get both attribute lists";
						yyerror (ErrorString);
						return 0;
					}
					
					LastAttribute->SetBaseFromAttributeList (NextAttribute);
					delete NextAttribute;
					Success = NewType->SetAttributes (LastAttribute);
					
					LastAttribute->GetNameAndBase (TempName, TempType);
				//	cout << "Found complex type with EXTENSION:  Name = " << TempName << " and Base = " << TempType << endl;
					
					delete LastAttribute;
					
					if (!Success) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  attributes incomplete";
						yyerror (ErrorString);
						return 0;
					}
					
					ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
					
					if (ElementDepth > 0) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  must not have a parent element";
						yyerror (ErrorString);
						return 0;
					}
					
					Success = NewType->AddSubElementList (SequencedElementsStack);
					
					if (!Success) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  could not transfer element sequence stack";
						yyerror (ErrorString);
						return 0;
					}
					
					delete NewElementName;
					ResultList->Append (NewType);
				;}
    break;

  case 44:
#line 1190 "rgschemascan.y"
    {
	
				//	cout << "Found 'simple' complex type:  attributes only" << endl;
					NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
					
					if (NewElementName == NULL) {
					
						ErrorString = "****error in building complex type with attributes only:  name not in name stack";
						yyerror (ErrorString);
						return 0;
					}
					
					NewType = new RGXMLComplexType ();
					LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					Success = NewType->SetAttributes (LastAttribute);
					delete LastAttribute;
					
					if (!Success) {
					
						ErrorString = "****error in building complex element named " + *NewElementName;
						ErrorString += ":  attributes incomplete";
						yyerror (ErrorString);
						return 0;
					}
					
					delete NewElementName;
					ElementDepth--;  // this removes the depth level for this element...any left?
					ResultList->Append (NewType);
	;}
    break;

  case 45:
#line 1221 "rgschemascan.y"
    {
	
				//	cout << "Found complex type definition with sequence but no attributes" << endl;
					ElementDepth--;
				;}
    break;

  case 60:
#line 1248 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming name attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
					
				//	cout << "Found name attribute:  " << *NewString << endl;
					NewElementName = new RGString (*NewString);
					ElementNameStack.Prepend (NewElementName);
					
					if (!CurrentAttributes->AddAttribute (RGNAME, NewString)) {
						
						yyerror("****error in name attribute:  require exactly one value");
						return 0;
					}
				;}
    break;

  case 61:
#line 1273 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming type attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
					
				//	cout << "Found type attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGTYPE, NewString)) {
						
						yyerror("****error in type attribute:  require exactly one value");
						return 0;
					}
				;}
    break;

  case 62:
#line 1296 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
				//	cout << "Found 'min occurs' attribute:  " << *NewString << endl;
					delete NewString;  // ignore it for now
				;}
    break;

  case 63:
#line 1304 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
				//	cout << "Found 'max occurs' attribute:  " << *NewString << endl;
					delete NewString;  // ignore it for now
				;}
    break;

  case 64:
#line 1312 "rgschemascan.y"
    {

					ListDepth++;
					
					if (ListDepth > 1) {
					
						yyerror("****error in list attribute:  no nested unbounded list declarations allowed");
						return 0;
					}
					
				//	cout << "Found 'max occurs' attribute:  unbounded" << endl;
				;}
    break;

  case 65:
#line 1326 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming reference attribute:  no string data found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found ref attribute:  " << *NewString << endl;
					
					NewElementName = new RGString (*NewString);
					ElementNameStack.Prepend (NewElementName);
					
					if (!CurrentAttributes->AddAttribute (RGREFERENCE, NewString)) {
						
						yyerror("****error in reference attribute:  require exactly one value");
						return 0;
					}
				;}
    break;

  case 66:
#line 1351 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming abstract attribute:  no string data found";
						yyerror (ErrorString);
						return 0;
					}
					
					NewString->ToLower ();
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found abstract attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGABSTRACT, NewString)) {
						
						yyerror("****error in abstract attribute:  require exactly one value");
						return 0;
					}
				;}
    break;

  case 67:
#line 1374 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming substitution group attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found substitution group attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGSUBSTITUTIONGROUP, NewString)) {
						
						yyerror("****error in substitution group attribute:  require exactly one value");
						return 0;
					}
				;}
    break;

  case 68:
#line 1396 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming location attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found schema location attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGINCLUDEPATH, NewString)) {
						
						yyerror("****error in schema location attribute:  require exactly one value");
						return 0;
					}
				;}
    break;

  case 69:
#line 1418 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming xpath attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found xpath attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGXPATH, NewString)) {
						
						yyerror("****error in xpath attribute:  require a value");
						return 0;
					}
				;}
    break;

  case 70:
#line 1440 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming key reference attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found key reference attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGKEYREFERENCE, NewString)) {
						
						yyerror("****error in key reference attribute:  require exactly one value");
						return 0;
					}
				;}
    break;

  case 71:
#line 1462 "rgschemascan.y"
    {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming base attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found base attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGBASE, NewString)) {
						
						yyerror("****error in base attribute:  require exactly one value");
						return 0;
					}
					
				//	CurrentAttributes->AddAttribute (RGNAME, NewString);
				;}
    break;

  case 72:
#line 1486 "rgschemascan.y"
    {

	//	cout << "Found attributes plus End directive" << endl;
		SaveCurrentAttributes ();
	;}
    break;

  case 73:
#line 1493 "rgschemascan.y"
    {

	//	cout << "Found attributes plus right bracket" << endl;
		SaveCurrentAttributes ();
	;}
    break;

  case 74:
#line 1500 "rgschemascan.y"
    {

			//	cout << "Found sequence" << endl;
				
				if (SequenceDepth > 1) {
				
					ErrorString = "****error in sequence for element named " + *(RGString*)ElementNameStack.First ();
					ErrorString += ":  sequence within sequence not allowed";
					yyerror (ErrorString);
					return 0;
				}
			;}
    break;

  case 76:
#line 1515 "rgschemascan.y"
    {
				
			//	cout << "Found multiple elements..." << endl;
	;}
    break;

  case 77:
#line 1521 "rgschemascan.y"
    {

			//	cout << "Found key directive" << endl;
			;}
    break;

  case 78:
#line 1527 "rgschemascan.y"
    {

			//		cout << "Found key reference directive" << endl;
				;}
    break;

  case 79:
#line 1533 "rgschemascan.y"
    { cout << "Found a key" << endl; ;}
    break;

  case 80:
#line 1534 "rgschemascan.y"
    { cout << "Found a key reference" << endl; ;}
    break;

  case 83:
#line 1541 "rgschemascan.y"
    {

			//	cout << "Found selector directive" << endl;
			;}
    break;

  case 84:
#line 1547 "rgschemascan.y"
    {

			//	cout << "Found field directive" << endl;
			;}
    break;

  case 85:
#line 1553 "rgschemascan.y"
    {

	//	cout << "Found a listAttribute plus right bracket" << endl;
	;}
    break;


    }

/* Line 998 of yacc.c.  */
#line 2887 "rgschemascan.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1561 "rgschemascan.y"

    
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>


int lineno = 1;
int last_err_line;
int errcount;
int TotalErrors;


#define MAX_ERR_PER_LINE  30

/*
 * yyerr() is called automatically by YYPARSE() when an 
 * error is detected in the input stream.   To avoid an avalanch of
 * errors, yyerror() only prints one message per line,
 * and if it sees too-many errors on a line, aborts the program.
 */
 
int yyerror(const char *s)
{
	char ermsg[80];
	TotalErrors++;
	
	if (lineno == last_err_line) {
	
	    if (errcount++ >= MAX_ERR_PER_LINE) {
	    
			sprintf(ermsg, "%s: Line %d",s,lineno);
			fprintf(stderr,"line %d: too many errors to continue\n",lineno);
			exit(1);  //???
	    }
	    
	    fprintf(stderr,"******line %d: %s\n",lineno,s);
		return 0;
	}
	
	errcount = 0;
	sprintf (ermsg, "%s: Line %d",s,lineno);
    fprintf (stderr,"******line %d: %s\n",lineno,s);
    cout << s << endl;
    last_err_line = lineno;
    return 1;
}



int RGXMLSchemaSummary :: CompileSchema () {
	
	int errcnt = 0;
	int status;
	size_t startPosition;
	size_t endPosition;
	RGString* NextSchemaFile;
	RGXMLSchemaSummary* NewSummary;
	
	NameStack.ClearAndDelete ();
	NumberStack.ClearAndDelete ();
	AttributeStack.ClearAndDelete ();
	XPathStack.ClearAndDelete ();
	SequencedElementsStack.ClearAndDelete ();
	NestedElementNameStack.ClearAndDelete ();
	ElementDepth = 0;
	ListDepth = 0;
	SequenceDepth = 0;
	ListElementStack.ClearAndDelete ();
	ElementNameStack.ClearAndDelete ();
	RestrictionStack.ClearAndDelete ();
	
	EnumerationType = NULL;
	
	LocalSchemasNeeded.ClearAndDelete ();
	SchemaDataResults.ClearAndDelete ();
	IncludedSchemaSummaries.ClearAndDelete ();
	FilesAlreadyDone.ClearAndDelete ();
	SchemaPath.FindAndReplaceAllSubstrings ("\\", "/");
	RGString LogPath (SchemaPath);
	Boolean FoundSlash = LogPath.FindLastSubstring ("/", startPosition, endPosition);
	
	if (FoundSlash)	{
	
		LogPath.ExtractAndRemoveSubstring (endPosition + 1, SchemaPath.Length () - 1);
		LogPath += "Logfile.txt";
	}
		
	else
		LogPath = "Logfile.txt";

	ClearAttributes ();
	
	FilesScanned = &FilesAlreadyDone;
	ResultList = &SchemaDataResults;
	
	RGString* ThisPath = new RGString (SchemaPath);
	FilesScanned->Append (ThisPath);
	
	yyin = freopen (SchemaPath, "rt", stdin);
	yyout = freopen (LogPath, "wt", stdout);

	TotalErrors = 0;
	
	if (YYPARSE()) {
		
		errcnt++;
		cout << "Schema " << SchemaPath << " did not compile." << endl;
		return 1;
	}
		
	NameStack.ClearAndDelete ();
	NumberStack.ClearAndDelete ();
	cout << "Schema " << SchemaPath << " compiled successfully." << endl;
	RGDListIterator it (LocalSchemasNeeded);
	
	while (NextSchemaFile = (RGString*) it ()) {
	
		NewSummary = new RGXMLSchemaSummary (*NextSchemaFile);
		status = NewSummary->CompileIncludedSchema (*FilesScanned, *ResultList);
		IncludedSchemaSummaries.Append (NewSummary);
		
		if (status > 0)
			return status;
	}
	
	fcloseall ();
	return 0;
}


int RGXMLSchemaSummary :: CompileIncludedSchema (RGDList& FilesAlreadyScanned, RGDList& Results) {

	int errcnt = 0;
	int status;
	RGString* NextSchemaFile;
	RGXMLSchemaSummary* NewSummary;
	
	NameStack.ClearAndDelete ();
	NumberStack.ClearAndDelete ();
	AttributeStack.ClearAndDelete ();
	XPathStack.ClearAndDelete ();
	SequencedElementsStack.ClearAndDelete ();
	NestedElementNameStack.ClearAndDelete ();
	ElementDepth = 0;
	ListDepth = 0;
	SequenceDepth = 0;
	ListElementStack.ClearAndDelete ();
	ElementNameStack.ClearAndDelete ();
	RestrictionStack.ClearAndDelete ();
	
	EnumerationType = NULL;
	
	LocalSchemasNeeded.ClearAndDelete ();
	SchemaDataResults.ClearAndDelete ();
	IncludedSchemaSummaries.ClearAndDelete ();

	ClearAttributes ();
	
	FilesScanned = &FilesAlreadyScanned;
	ResultList = &Results;
	
	RGString* ThisPath = new RGString (SchemaPath);
	
	if (!FilesScanned->Contains (ThisPath))
		FilesScanned->Append (ThisPath);
		
	else {
	
		delete ThisPath;
		return 0;
	}
		
	yyin = freopen (SchemaPath, "rt", stdin);

	TotalErrors = 0;
	
	if (YYPARSE()) {
		
		errcnt++;
		cout << "Schema " << SchemaPath << " did not compile." << endl;
		return 1;
	}
		
	NameStack.ClearAndDelete ();
	NumberStack.ClearAndDelete ();
	cout << "Schema " << SchemaPath << " compiled successfully." << endl;
	RGDListIterator it (LocalSchemasNeeded);
	
	while (NextSchemaFile = (RGString*) it ()) {
	
		NewSummary = new RGXMLSchemaSummary (*NextSchemaFile);
		status = NewSummary->CompileIncludedSchema (*FilesScanned, *ResultList);
		IncludedSchemaSummaries.Append (NewSummary);
		
		if (status > 0)
			return status;
	}
	
	fcloseall ();
	return 0;
}



