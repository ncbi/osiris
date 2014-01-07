/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*  FileName: rgschema.cpp
*  Author:   Robert Goor
*
*/
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



/* A lexical scanner generated by flex */

/* Scanner skeleton version:
 * $Header: /src/NCBI/vault.ncbi/internal/osiris/BaseClassLib/rgschema.cpp,v 1.16 2011/05/11 20:12:20 hoffman Exp $
 */

#define FLEX_SCANNER
#define YY_FLEX_MAJOR_VERSION 2
#define YY_FLEX_MINOR_VERSION 5

#include <stdio.h>
#include <errno.h>

/* cfront 1.2 defines "c_plusplus" instead of "__cplusplus" */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif


#ifdef __cplusplus

#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif

/* Use prototypes in function declarations. */
#define YY_USE_PROTOS

/* The "const" storage-class-modifier is valid. */
#define YY_USE_CONST

#else	/* ! __cplusplus */

#if __STDC__

#define YY_USE_PROTOS
#define YY_USE_CONST

#endif	/* __STDC__ */
#endif	/* ! __cplusplus */

#ifdef __TURBOC__
 #pragma warn -rch
 #pragma warn -use
#include <io.h>
#include <stdlib.h>
#define YY_USE_CONST
#define YY_USE_PROTOS
#endif

#ifdef YY_USE_CONST
#define yyconst const
#else
#define yyconst
#endif


#ifdef YY_USE_PROTOS
#define YY_PROTO(proto) proto
#else
#define YY_PROTO(proto) ()
#endif


/* Returned upon end-of-file. */
#define YY_NULL 0

/* Promotes a possibly negative, possibly signed char to an unsigned
 * integer for use as an array index.  If the signed char is negative,
 * we want to instead treat it as an 8-bit unsigned char, hence the
 * double cast.
 */
#define YY_SC_TO_UI(c) ((unsigned int) (unsigned char) c)

/* Enter a start condition.  This macro really ought to take a parameter,
 * but we do it the disgusting crufty way forced on us by the ()-less
 * definition of BEGIN.
 */
#define BEGIN yy_start = 1 + 2 *

/* Translate the current start state into a value that can be later handed
 * to BEGIN to return to the state.  The YYSTATE alias is for lex
 * compatibility.
 */
#define YY_START ((yy_start - 1) / 2)
#define YYSTATE YY_START

/* Action number for EOF rule of a given start state. */
#define YY_STATE_EOF(state) (YY_END_OF_BUFFER + state + 1)

/* Special action meaning "start processing a new file". */
#define YY_NEW_FILE yyrestart( yyin )

#define YY_END_OF_BUFFER_CHAR 0

/* Size of default input buffer. */
#define YY_BUF_SIZE 16384

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern int yyleng;
extern FILE *yyin, *yyout;

#define EOB_ACT_CONTINUE_SCAN 0
#define EOB_ACT_END_OF_FILE 1
#define EOB_ACT_LAST_MATCH 2

/* The funky do-while in the following #define is used to turn the definition
 * int a single C statement (which needs a semi-colon terminator).  This
 * avoids problems with code like:
 *
 * 	if ( condition_holds )
 *		yyless( 5 );
 *	else
 *		do_something_else();
 *
 * Prior to using the do-while the compiler would get upset at the
 * "else" because it interpreted the "if" statement as being all
 * done when it reached the ';' after the yyless() call.
 */

/* Return all but the first 'n' matched characters back to the input stream. */

#define yyless(n) \
	do \
		{ \
		/* Undo effects of setting up yytext. */ \
		*yy_cp = yy_hold_char; \
		YY_RESTORE_YY_MORE_OFFSET \
		yy_c_buf_p = yy_cp = yy_bp + n - YY_MORE_ADJ; \
		YY_DO_BEFORE_ACTION; /* set up yytext again */ \
		} \
	while ( 0 )

#define unput(c) yyunput( c, yytext_ptr )

/* The following is because we cannot portably get our hands on size_t
 * (without autoconf's help, which isn't available because we want
 * flex-generated scanners to compile on their own).
 */
typedef unsigned int yy_size_t;


struct yy_buffer_state
	{
	FILE *yy_input_file;

	char *yy_ch_buf;		/* input buffer */
	char *yy_buf_pos;		/* current position in input buffer */

	/* Size of input buffer in bytes, not including room for EOB
	 * characters.
	 */
	yy_size_t yy_buf_size;

	/* Number of characters read into yy_ch_buf, not including EOB
	 * characters.
	 */
	int yy_n_chars;

	/* Whether we "own" the buffer - i.e., we know we created it,
	 * and can realloc() it to grow it, and should free() it to
	 * delete it.
	 */
	int yy_is_our_buffer;

	/* Whether this is an "interactive" input source; if so, and
	 * if we're using stdio for input, then we want to use getc()
	 * instead of fread(), to make sure we stop fetching input after
	 * each newline.
	 */
	int yy_is_interactive;

	/* Whether we're considered to be at the beginning of a line.
	 * If so, '^' rules will be active on the next match, otherwise
	 * not.
	 */
	int yy_at_bol;

	/* Whether to try to fill the input buffer when we reach the
	 * end of it.
	 */
	int yy_fill_buffer;

	int yy_buffer_status;
#define YY_BUFFER_NEW 0
#define YY_BUFFER_NORMAL 1
	/* When an EOF's been seen but there's still some text to process
	 * then we mark the buffer as YY_EOF_PENDING, to indicate that we
	 * shouldn't try reading from the input source any more.  We might
	 * still have a bunch of tokens to match, though, because of
	 * possible backing-up.
	 *
	 * When we actually see the EOF, we change the status to "new"
	 * (via yyrestart()), so that the user can continue scanning by
	 * just pointing yyin at a new input file.
	 */
#define YY_BUFFER_EOF_PENDING 2
	};

static YY_BUFFER_STATE yy_current_buffer = 0;

/* We provide macros for accessing buffer states in case in the
 * future we want to put the buffer states in a more general
 * "scanner state".
 */
#define YY_CURRENT_BUFFER yy_current_buffer


/* yy_hold_char holds the character lost when yytext is formed. */
static char yy_hold_char;

static int yy_n_chars;		/* number of characters read into yy_ch_buf */


int yyleng;

/* Points to current character in buffer. */
static char *yy_c_buf_p = (char *) 0;
static int yy_init = 1;		/* whether we need to initialize */
static int yy_start = 0;	/* start state number */

/* Flag which is used to allow yywrap()'s to do buffer switches
 * instead of setting up a fresh yyin.  A bit of a hack ...
 */
static int yy_did_buffer_switch_on_eof;

void yyrestart YY_PROTO(( FILE *input_file ));

void yy_switch_to_buffer YY_PROTO(( YY_BUFFER_STATE new_buffer ));
void yy_load_buffer_state YY_PROTO(( void ));
YY_BUFFER_STATE yy_create_buffer YY_PROTO(( FILE *file, int size ));
void yy_delete_buffer YY_PROTO(( YY_BUFFER_STATE b ));
void yy_init_buffer YY_PROTO(( YY_BUFFER_STATE b, FILE *file ));
void yy_flush_buffer YY_PROTO(( YY_BUFFER_STATE b ));
#define YY_FLUSH_BUFFER yy_flush_buffer( yy_current_buffer )

YY_BUFFER_STATE yy_scan_buffer YY_PROTO(( char *base, yy_size_t size ));
YY_BUFFER_STATE yy_scan_string YY_PROTO(( yyconst char *yy_str ));
YY_BUFFER_STATE yy_scan_bytes YY_PROTO(( yyconst char *bytes, int len ));

static void *yy_flex_alloc YY_PROTO(( yy_size_t ));
static void *yy_flex_realloc YY_PROTO(( void *, yy_size_t ));
static void yy_flex_free YY_PROTO(( void * ));

#define yy_new_buffer yy_create_buffer

#define yy_set_interactive(is_interactive) \
	{ \
	if ( ! yy_current_buffer ) \
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_is_interactive = is_interactive; \
	}

#define yy_set_bol(at_bol) \
	{ \
	if ( ! yy_current_buffer ) \
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_at_bol = at_bol; \
	}

#define YY_AT_BOL() (yy_current_buffer->yy_at_bol)


#define YY_USES_REJECT

#define yywrap() 1
#define YY_SKIP_YYWRAP
typedef unsigned char YY_CHAR;
FILE *yyin = (FILE *) 0, *yyout = (FILE *) 0;
typedef int yy_state_type;
#define YY_FLEX_LEX_COMPAT
extern int yylineno;
int yylineno = 1;
extern char yytext[];


static yy_state_type yy_get_previous_state YY_PROTO(( void ));
static yy_state_type yy_try_NUL_trans YY_PROTO(( yy_state_type current_state ));
static int yy_get_next_buffer YY_PROTO(( void ));
static void yy_fatal_error YY_PROTO(( yyconst char msg[] ));

/* Done after the current pattern has been matched and before the
 * corresponding action - sets up yytext.
 */
#define YY_DO_BEFORE_ACTION \
	yytext_ptr = yy_bp; \
	yyleng = (int) (yy_cp - yy_bp); \
	yy_hold_char = *yy_cp; \
	*yy_cp = '\0'; \
	if ( yyleng + yy_more_offset >= YYLMAX ) \
		YY_FATAL_ERROR( "token too large, exceeds YYLMAX" ); \
	yy_flex_strncpy( &yytext[yy_more_offset], yytext_ptr, yyleng + 1 ); \
	yyleng += yy_more_offset; \
	yy_prev_more_offset = yy_more_offset; \
	yy_more_offset = 0; \
	yy_c_buf_p = yy_cp;

#define YY_NUM_RULES 65
#define YY_END_OF_BUFFER 66
static yyconst short int yy_acclist[245] =
    {   0,
       66,   64,   65,    2,   64,   65,    1,    2,   65,   16,
       64,   65,   64,   65,   33,   63,   64,   65,   64,   65,
       32,   33,   64,   65,   64,   65,   17,   64,   65,    3,
       64,   65,   63,   64,   65,   63,   64,   65,   63,   64,
       65,   63,   64,   65,   63,   64,   65,   63,   64,   65,
       63,   64,   65,   63,   64,   65,   63,   64,   65,   63,
       64,   65,   63,   64,   65,   63,   64,   65,   63,   64,
       65,    2,   33,   33,   63,   63,   33,   63,   63,   15,
       32,   33,   63,   63,   63,   63,   63,   63,   63,   63,
       63,   63,   63,   63,   63,   63,   33,   63,   33,   63,

       63,   63,   63,   63,   63,   63,   63,   23,   63,   63,
       63,   63,   63,   63,   63,   63,   33,    5,   63,   25,
       63,   63,   63,   63,   18,   63,   63,   63,   63,   63,
       63,   19,   63,   63,   63,   63,   63,   63,   63,   63,
       63,   24,   63,   63,   63,   63,   63,   27,   63,   38,
       63,   63,   63,   63,   63,   63,   63,   63,   63,   63,
        4,   35,   63,   63,   63,   63,   63,   29,   63,   63,
       63,   63,    4,    4,   26,   63,   30,   63,   63,   63,
       28,   63,   63,   63,   63,   36,   39,   21,   63,   20,
       63,   63,   63,   22,   63,   40,   57,   63,   63,   13,

       42,    6,   63,   63,   41,    7,   37,   34,   63,   63,
       14,   48,   58,   59,   11,   63,   63,   12,   46,   62,
       43,   63,   63,   49,   44,   56,   50,   60,   63,   47,
       55,   53,   54,   52,   63,   45,   51,   31,   63,   61,
        9,   10,    8,    8
    } ;

static yyconst short int yy_accept[492] =
    {   0,
        1,    1,    1,    2,    4,    7,   10,   13,   15,   19,
       21,   25,   27,   30,   33,   36,   39,   42,   45,   48,
       51,   54,   57,   60,   63,   66,   69,   72,   73,   74,
       75,   76,   77,   79,   80,   81,   83,   83,   83,   83,
       83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
       93,   94,   95,   96,   97,   98,   98,   99,   99,  101,
      101,  101,  101,  101,  101,  102,  103,  104,  105,  106,
      107,  108,  110,  111,  112,  113,  114,  115,  116,  117,
      118,  118,  118,  119,  119,  120,  122,  123,  124,  125,
      127,  128,  129,  130,  131,  132,  134,  135,  136,  137,

      137,  137,  137,  137,  137,  137,  137,  137,  137,  137,
      137,  137,  137,  137,  137,  138,  139,  140,  141,  142,
      144,  145,  146,  147,  148,  150,  152,  152,  152,  152,
      152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
      152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
      152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
      162,  162,  162,  162,  162,  162,  162,  162,  162,  162,
      162,  162,  162,  162,  162,  162,  162,  163,  163,  163,
      163,  163,  163,  163,  163,  163,  163,  163,  164,  165,
      166,  167,  168,  170,  171,  172,  173,  174,  175,  175,

      175,  175,  175,  175,  175,  175,  175,  175,  175,  175,
      175,  175,  175,  175,  175,  175,  175,  175,  175,  175,
      175,  175,  175,  175,  175,  175,  175,  175,  175,  175,
      177,  179,  180,  181,  183,  184,  185,  186,  186,  186,
      186,  187,  187,  187,  187,  187,  187,  187,  187,  187,
      187,  187,  188,  188,  188,  188,  188,  188,  188,  188,
      188,  188,  188,  188,  188,  188,  188,  188,  188,  188,
      190,  192,  193,  194,  196,  196,  196,  196,  196,  196,
      196,  196,  196,  196,  196,  196,  196,  196,  196,  196,
      197,  198,  198,  198,  198,  198,  198,  198,  198,  198,

      198,  198,  198,  198,  198,  199,  200,  200,  200,  200,
      200,  200,  200,  200,  200,  200,  200,  201,  201,  201,
      201,  202,  202,  202,  202,  202,  202,  202,  202,  202,
      203,  203,  203,  203,  203,  203,  204,  205,  205,  205,
      205,  206,  206,  207,  207,  207,  207,  207,  207,  207,
      207,  207,  207,  207,  207,  207,  207,  207,  207,  208,
      209,  209,  209,  209,  210,  211,  211,  211,  212,  212,
      212,  212,  212,  212,  212,  212,  212,  213,  213,  213,
      213,  214,  214,  214,  215,  215,  216,  216,  216,  216,
      217,  218,  218,  218,  218,  218,  219,  219,  219,  219,

      219,  219,  219,  219,  219,  219,  219,  219,  220,  220,
      221,  223,  224,  224,  224,  225,  225,  225,  225,  225,
      226,  227,  227,  227,  227,  227,  227,  228,  229,  230,
      230,  230,  230,  231,  231,  231,  231,  231,  231,  231,
      232,  233,  234,  235,  236,  236,  237,  238,  238,  238,
      238,  238,  238,  240,  240,  240,  240,  240,  241,  241,
      241,  241,  242,  243,  243,  243,  243,  243,  243,  243,
      243,  243,  243,  243,  243,  243,  243,  243,  243,  243,
      243,  243,  243,  243,  243,  243,  243,  244,  244,  245,
      245

    } ;

static yyconst int yy_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    2,    4,    5,    1,    1,    1,    1,    1,    1,
        1,    1,    6,    1,    7,    8,    9,   10,   10,   10,
       10,   10,   10,   10,   10,   10,   10,   11,    1,   12,
       13,   14,   15,    1,   16,   16,   17,   18,   19,   16,
       20,   16,   21,   16,   16,   22,   16,   16,   23,   16,
       16,   16,   24,   25,   16,   16,   16,   16,   16,   16,
        1,    1,    1,    1,   26,    1,   27,   28,   29,   30,

       31,   32,   33,   34,   35,   16,   36,   37,   38,   39,
       40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
       50,   16,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,

        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1
    } ;

static yyconst int yy_meta[51] =
    {   0,
        1,    1,    2,    1,    1,    1,    1,    3,    3,    3,
        3,    1,    1,    1,    1,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3
    } ;

static yyconst short int yy_base[497] =
    {   0,
        0,    0, 1078, 1079,   49,   51, 1079,   47,   49, 1063,
       53,   58, 1079, 1079,   55,   60,   67,   73,   91,   94,
       80,   99,   86,  118,  124,  127,  130,  140,  136,   64,
      138,    0,  141,  167, 1079,   77, 1069, 1026, 1059, 1029,
      170,  173,  176,  179,  197,  200,  209,  220,  223,  232,
      244,  253,  256,  262,  181,  151,  159, 1062, 1061, 1063,
     1025, 1053, 1053, 1055,  265,  285,  288,  292,  296,  317,
      320,  323,  326,  329,  347,  356,  359,  366,  369, 1055,
     1057, 1052, 1079,  388,  379,  387,  418,  392,  428,  432,
      437,  440,  450,  460,  463,  469,  481,  490,  494, 1055,

     1054,  481, 1021, 1019,  155,   76, 1019, 1026, 1025,   87,
     1024,  166, 1014, 1019,  504,  523,  526,  530,  534,  537,
      556,  560,  563,  569,  581,  590,  117, 1012,  166, 1020,
     1019,  216, 1010, 1010, 1016, 1000, 1000, 1013, 1016, 1013,
      991, 1001,  990,  999,  993, 1002,  133,  997,  989,  998,
      593,  600,  603,  614,  621,  624,  632,  627,  652,  306,
      994, 1000,  985,  979,  984,  993,  984,  987,  984,  982,
      984,  983,  989,  982,  989,  980,  973,  982,  203,  265,
      969,  982,  981,  965,  969,  982,  963,  655,  660,  664,
      678,  683,  686,  692,  713,  716,  404,  266,  966,  968,

      974,  102,  959,  972,  956,  960,  955,  962,  967,  966,
      957,  965,  949,  947,  961,  946,  941,  950,  957,  938,
      947,  954,  941,  945,  953,  950,  943,  942,  947,  719,
      724,  727,  746,  751,  754,  758,  765,  940,  945,  936,
     1079,  943,  930,  934,  940,  933,  942,  937,  928,  919,
      915, 1079,  917,  921,  918,  915,  919,  918,  907,  915,
      914,  903,  906,  913,  894,  897,  903,  914,  907,  776,
      783,  787,  790,  795,  899,  887,  877,  887,  876,  883,
      869,  875,  855,  845,  842,  855,  845,  839,  840, 1079,
     1079,  819,  818,  817,  812,  810,  811,  814,  821,  793,

      803,  806,  791,  779,  801,  808,  759,  762,  754,  768,
      752,  756,  737,  729,  717,  231, 1079,  706,  706,  705,
     1079,  694,  685,  672,  670,  667,  640,  639,  669,  666,
      636,  646,  626,  634,  630,  819,  813,  275,  642,  611,
     1079,  603, 1079,  615,  595,  599,  588,  571,  585,  558,
      578,  550,  544,  547,  532,  531,  534,  527, 1079,  547,
      517,  522,  525,  827,  831,  513,  501, 1079,  511,  507,
      524,  488,  487,  484,  481,  479, 1079,  483,  476,  474,
     1079,  471,  467, 1079,  456, 1079,  462,  440,  453,  849,
      852,  443,  440,  461,  427, 1079,  433,  426,  390,  403,

      391,  383,  363,  362,  361,  357,  361, 1079,  350, 1079,
      855,  859,  341,  350, 1079,  335,  358,  339,  328, 1079,
     1079,  318,  319,  316,  314,  313, 1079, 1079,  863,  293,
      309,  307, 1079,  359,  381,  450,  485,  281,  272, 1079,
     1079, 1079, 1079,  881,  273, 1079, 1079,  453,  581,  603,
      259,  246,  887,  238,  271,  615,  264, 1079,  263,  210,
      708, 1079, 1079,  206,  746,  233,  776,  212,  881,  198,
      703,  188,  696,  181,  874,  171,  900,  171,  902,  117,
      913,  124,  915,  112,  921,   59,  926,   78, 1079, 1079,
       76,   53,  954,  957,  960,  963

    } ;

static yyconst short int yy_def[497] =
    {   0,
      490,    1,  490,  490,  490,  490,  490,  490,  491,  490,
      490,  490,  490,  490,  491,  491,  491,  491,  491,  491,
      491,  491,  491,  491,  491,  491,  491,  490,  490,  490,
      491,  492,  492,  491,  490,  490,  490,  490,  493,  490,
      491,  491,  491,  491,  491,  491,  491,  491,  491,  491,
      491,  491,  491,  491,  490,  490,  492,  490,  492,  490,
      490,  493,  490,  490,  491,  491,  491,  491,  491,  491,
      491,  491,  491,  491,  491,  491,  491,  491,  491,  490,
      494,  490,  490,  490,  491,  491,  491,  491,  491,  491,
      491,  491,  491,  491,  491,  491,  491,  491,  491,  494,

      494,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  491,  491,  491,  491,  491,  491,
      491,  491,  491,  491,  491,  491,  494,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      491,  491,  491,  491,  491,  491,  491,  491,  491,  494,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  491,  491,  491,
      491,  491,  491,  491,  491,  491,  494,  490,  490,  490,

      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  491,
      491,  491,  491,  491,  491,  491,  491,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  491,
      491,  491,  491,  491,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  495,  490,

      490,  490,  490,  490,  491,  491,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  495,  495,
      490,  490,  490,  490,  490,  491,  491,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  491,  491,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  491,
      491,  490,  490,  490,  490,  490,  490,  490,  490,  490,

      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      491,  491,  490,  490,  490,  490,  490,  496,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  491,  490,
      490,  490,  490,  496,  496,  496,  496,  490,  490,  490,
      490,  490,  490,  491,  490,  490,  490,  496,  490,  496,
      490,  490,  491,  490,  490,  496,  490,  490,  490,  490,
      496,  490,  490,  490,  496,  490,  496,  490,  496,  490,
      496,  490,  496,  490,  496,  490,  496,  490,  496,  490,
      496,  490,  496,  490,  496,  490,  496,  490,  490,    0,
      490,  490,  490,  490,  490,  490

    } ;

static yyconst short int yy_nxt[1130] =
    {   0,
        4,    5,    6,    4,    7,    4,    8,    9,   10,   11,
        4,   12,   13,   14,    4,   15,   15,   15,   15,   15,
       15,   15,   15,   15,   15,    4,   16,   17,   18,   15,
       15,   15,   15,   15,   15,   15,   15,   19,   20,   15,
       21,   15,   22,   23,   24,   25,   26,   15,   27,   15,
       28,   28,   28,   28,   29,   32,   30,   32,   33,   32,
       29,   37,   36,   32,   32,   32,   38,   34,   32,   32,
       32,   29,   39,   30,   32,   32,   32,   32,   31,   34,
       32,   32,   32,   32,   29,   32,   36,   41,   32,   32,
       32,  489,   32,   42,   32,   32,   32,  488,   32,   32,

       32,   32,   32,   32,   32,   32,   40,   32,   32,   32,
      138,   32,   43,  143,   49,  241,   32,   44,  139,   32,
       46,  144,   47,  127,   32,   45,   32,   32,   32,   48,
      160,   50,   32,   32,   32,   32,   32,   32,   32,   32,
       32,   28,   28,   32,  242,   55,   32,   32,   32,   32,
       33,  486,   32,   53,   56,   32,   58,   58,  484,   57,
       80,  482,   52,   32,   58,   58,   56,   51,   59,  183,
       54,   57,   58,   58,  184,   32,   59,   32,   32,   32,
       32,   32,   32,   32,   32,   32,   32,   32,   32,   32,
       55,  135,   32,  136,  146,   32,  147,  480,   32,   56,

      148,   32,  162,  137,   32,   32,   32,   32,   32,   32,
       32,   56,   67,   65,  163,  478,   66,   32,   32,   32,
      476,  217,   32,  218,  219,   32,  474,   68,   32,   32,
       32,   32,   32,   32,   32,   69,  472,   70,  470,   71,
       32,   32,   32,  468,  166,   32,  167,  347,   32,  466,
      168,   72,   32,   32,   32,  348,   74,   32,  464,   75,
       73,   32,   32,   32,   32,   32,   32,  198,  198,   32,
       32,   32,   32,   32,   32,   32,  463,  462,   32,  460,
       77,   32,  459,  220,   76,  221,  222,   32,   79,  458,
       32,  366,   78,   32,   32,   32,   32,   32,   32,  367,

       32,   32,   32,  457,   32,   32,   32,  197,  198,   85,
       32,  454,  101,   32,   88,   86,  452,   32,   89,  451,
      447,   32,  446,  445,   87,   32,   32,   32,   32,   32,
       32,   32,   32,   32,   32,   32,   32,   32,   32,   32,
      435,  436,   32,  443,  442,   32,  441,   90,   32,  440,
      437,   32,  439,   92,   32,   32,   32,   32,  438,   94,
      448,  449,   93,   91,   32,   32,   32,   32,   32,   32,
      437,  433,   32,  432,   32,   32,   32,   32,   32,   32,
      431,   32,  435,  436,   32,  430,   96,   32,   32,   32,
       95,   32,  437,  428,   32,   32,   32,   32,   97,  427,

       32,   32,   32,  426,   32,  197,  198,  425,  424,  423,
      101,   98,   32,   99,  103,  422,  104,   32,  105,  106,
      117,  115,  107,  108,  109,  110,   32,   32,   32,  421,
      111,  112,  113,  420,  419,  114,   32,   32,   32,  418,
       32,   32,   32,   32,  116,   32,   32,   32,   32,   32,
       32,  435,  436,   32,  448,  449,  118,   32,   32,   32,
       32,  437,   32,  417,  437,   32,  416,  119,   32,   32,
       32,   32,   32,   32,  415,   32,  121,   32,   32,   32,
      414,  413,  120,  410,  409,   32,  448,  449,   32,   32,
       32,   32,  408,  450,   32,  407,  437,  122,   32,   32,

       32,  406,   32,   32,   32,  405,   32,  123,  404,  128,
      403,  129,   32,   32,   32,   32,  130,  402,  401,   32,
      125,  400,  399,  131,  132,  398,  124,  126,  397,   32,
      151,   32,   32,   32,   32,   32,   32,  396,   32,   32,
       32,  395,   32,   32,   32,   32,   32,   32,   32,  394,
      393,   32,  392,  389,  153,   32,  388,  387,  154,   32,
      386,  385,   32,  152,   32,   32,   32,  384,   32,   32,
       32,   32,   32,   32,  383,  382,  155,   32,   32,   32,
      381,   32,  449,  449,  156,   32,  157,  380,   32,   32,
       32,   32,  455,  379,   32,  378,  377,  158,   32,   32,

       32,   32,   32,   32,  448,  449,   32,  159,   32,   32,
       32,   32,   32,   32,  437,   32,  448,  449,   32,  376,
      375,  188,   32,   32,   32,   32,  437,  374,   32,   32,
       32,   32,   32,   32,   32,   32,   32,   32,  373,   32,
       32,   32,   32,  189,  372,  371,   32,  370,  190,   32,
      369,  456,   32,  194,  193,  368,  363,   32,  461,  191,
       32,   32,   32,   32,   32,   32,  362,  192,   32,   32,
       32,  195,   32,   32,   32,  361,  360,   32,  359,  330,
       32,  196,  330,  358,  357,   32,   32,   32,   32,   32,
      231,   32,   32,   32,   32,   32,   32,  448,  449,  230,

       32,   32,   32,   32,  448,  449,  232,  437,   32,  448,
      449,   32,  356,  234,  437,  355,  354,   32,  465,  437,
      233,   32,   32,   32,   32,   32,   32,   32,   32,   32,
      353,  235,   32,   32,   32,   32,   32,   32,   32,  352,
      475,   32,  473,  351,   32,  350,  237,  448,  449,   32,
      349,  346,   32,  345,   32,   32,   32,  437,  236,   32,
       32,   32,   32,   32,   32,  344,   32,   32,   32,  343,
      270,   32,  467,   32,   32,   32,   32,  448,  449,   32,
      342,  341,  272,   32,   32,   32,   32,  437,  340,  271,
       32,   32,   32,   32,  274,   32,   32,   32,   32,   32,

       32,   32,  273,   32,   32,   32,  339,  338,   32,   32,
       32,   32,   32,  305,  469,   32,   32,   32,   32,  335,
       32,   32,   32,   32,  306,  334,   32,   32,   32,   32,
      333,  332,  331,   32,  330,   32,   32,   32,   32,   32,
       32,   32,  328,  327,   32,  336,  326,  337,  325,  324,
      391,  365,   32,  364,  323,  322,   32,   32,   32,   32,
       32,   32,   32,   32,   32,   32,  390,   32,   32,   32,
      321,   32,   32,   32,   32,  448,  449,   32,  320,  319,
       32,  318,  448,  449,   32,  437,  317,  411,   32,   32,
       32,   32,  437,  316,  412,   32,   32,   32,  429,  315,

      477,  448,  449,  448,  449,  314,   32,  313,  444,  312,
      311,  437,   32,  437,  448,  449,  448,  449,  310,  471,
      309,  453,  448,  449,  437,  308,  437,  448,  449,  307,
      304,  303,  437,  302,  487,  301,  481,  437,  300,  299,
      298,  297,  296,  295,  479,  294,  293,  292,  291,  290,
      289,  288,  483,  485,   62,   62,   62,  100,  287,  100,
      329,  286,  329,  434,  434,  434,  285,  284,  283,  282,
      281,  280,  279,  278,  277,  276,  275,  269,  268,  267,
      266,  265,  264,  263,  262,  261,  260,  259,  258,  257,
      256,  255,  254,  253,  252,  251,  250,  249,  248,  247,

      246,  245,  244,  243,  240,  239,  238,  229,  228,  227,
      226,  225,  224,  223,  216,  215,  214,  213,  212,  211,
      210,  209,  208,  207,  206,  205,  204,  203,  202,  201,
      200,  199,  187,  186,  185,  182,  181,  180,  179,  178,
      177,  176,  175,  174,  173,  172,  171,  170,  169,  165,
      164,  161,  150,  149,  145,  142,  141,  140,  134,  133,
      127,  101,  102,  101,   80,   84,   83,   63,   82,   81,
       59,   80,   64,   63,   61,   60,   35,  490,    3,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,

      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490
    } ;

static yyconst short int yy_chk[1130] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        5,    5,    6,    6,    8,  492,    8,    9,    9,    9,
       11,   12,   11,   15,   15,   15,   12,    9,   16,   16,
       16,   30,   12,   30,    9,   17,   17,   17,  491,    9,
       15,   18,   18,   18,   36,   16,   36,   16,   21,   21,
       21,  488,   17,   17,   23,   23,   23,  486,   18,   19,

       19,   19,   20,   20,   20,   21,   12,   22,   22,   22,
      106,   23,   18,  110,   23,  202,   19,   19,  106,   20,
       20,  110,   21,  127,   22,   19,   24,   24,   24,   22,
      127,   23,   25,   25,   25,   26,   26,   26,   27,   27,
       27,   28,   28,   24,  202,   29,   31,   31,   31,   25,
       33,  484,   26,   26,   29,   27,   56,   56,  482,   33,
       56,  480,   25,   31,   57,   57,   29,   24,   57,  147,
       27,   33,   34,   34,  147,   34,   34,   34,   41,   41,
       41,   42,   42,   42,   43,   43,   43,   44,   44,   44,
       55,  105,   34,  105,  112,   41,  112,  478,   42,   55,

      112,   43,  129,  105,   44,   45,   45,   45,   46,   46,
       46,   55,   43,   41,  129,  476,   42,   47,   47,   47,
      474,  179,   45,  179,  179,   46,  472,   44,   48,   48,
       48,   49,   49,   49,   47,   45,  470,   46,  468,   47,
       50,   50,   50,  466,  132,   48,  132,  316,   49,  464,
      132,   48,   51,   51,   51,  316,   49,   50,  460,   50,
       48,   52,   52,   52,   53,   53,   53,  198,  198,   51,
       54,   54,   54,   65,   65,   65,  459,  457,   52,  455,
       52,   53,  454,  180,   51,  180,  180,   54,   54,  452,
       65,  338,   53,   66,   66,   66,   67,   67,   67,  338,

       68,   68,   68,  451,   69,   69,   69,  160,  160,   65,
       66,  445,  160,   67,   68,   66,  439,   68,   69,  438,
      432,   69,  431,  430,   67,   70,   70,   70,   71,   71,
       71,   72,   72,   72,   73,   73,   73,   74,   74,   74,
      418,  418,   70,  426,  425,   71,  424,   70,   72,  423,
      418,   73,  422,   72,   74,   75,   75,   75,  419,   74,
      434,  434,   73,   71,   76,   76,   76,   77,   77,   77,
      434,  417,   75,  416,   78,   78,   78,   79,   79,   79,
      414,   76,  435,  435,   77,  413,   76,   85,   85,   85,
       75,   78,  435,  409,   79,   86,   86,   86,   77,  407,

       88,   88,   88,  406,   85,  197,  197,  405,  404,  403,
      197,   78,   86,   79,   84,  402,   84,   88,   84,   84,
       88,   85,   84,   84,   84,   84,   87,   87,   87,  401,
       84,   84,   84,  400,  399,   84,   89,   89,   89,  398,
       90,   90,   90,   87,   87,   91,   91,   91,   92,   92,
       92,  436,  436,   89,  448,  448,   89,   90,   93,   93,
       93,  436,   91,  397,  448,   92,  395,   91,   94,   94,
       94,   95,   95,   95,  394,   93,   93,   96,   96,   96,
      393,  392,   92,  389,  388,   94,  437,  437,   95,   97,
       97,   97,  387,  437,   96,  385,  437,   94,   98,   98,

       98,  383,   99,   99,   99,  382,   97,   95,  380,  102,
      379,  102,  115,  115,  115,   98,  102,  378,  376,   99,
       98,  375,  374,  102,  102,  373,   97,   99,  372,  115,
      115,  116,  116,  116,  117,  117,  117,  371,  118,  118,
      118,  370,  119,  119,  119,  120,  120,  120,  116,  369,
      367,  117,  366,  363,  117,  118,  362,  361,  118,  119,
      360,  358,  120,  116,  121,  121,  121,  357,  122,  122,
      122,  123,  123,  123,  356,  355,  119,  124,  124,  124,
      354,  121,  449,  449,  121,  122,  122,  353,  123,  125,
      125,  125,  449,  352,  124,  351,  350,  123,  126,  126,

      126,  151,  151,  151,  450,  450,  125,  124,  152,  152,
      152,  153,  153,  153,  450,  126,  456,  456,  151,  349,
      348,  151,  154,  154,  154,  152,  456,  347,  153,  155,
      155,  155,  156,  156,  156,  158,  158,  158,  346,  154,
      157,  157,  157,  152,  345,  344,  155,  342,  153,  156,
      340,  450,  158,  157,  156,  339,  335,  157,  456,  154,
      159,  159,  159,  188,  188,  188,  334,  155,  189,  189,
      189,  158,  190,  190,  190,  333,  332,  159,  331,  330,
      188,  159,  329,  328,  327,  189,  191,  191,  191,  190,
      189,  192,  192,  192,  193,  193,  193,  473,  473,  188,

      194,  194,  194,  191,  471,  471,  190,  473,  192,  461,
      461,  193,  326,  192,  471,  325,  324,  194,  461,  461,
      191,  195,  195,  195,  196,  196,  196,  230,  230,  230,
      323,  194,  231,  231,  231,  232,  232,  232,  195,  322,
      473,  196,  471,  320,  230,  319,  196,  465,  465,  231,
      318,  315,  232,  314,  233,  233,  233,  465,  195,  234,
      234,  234,  235,  235,  235,  313,  236,  236,  236,  312,
      232,  233,  465,  237,  237,  237,  234,  467,  467,  235,
      311,  310,  235,  236,  270,  270,  270,  467,  309,  233,
      237,  271,  271,  271,  237,  272,  272,  272,  273,  273,

      273,  270,  236,  274,  274,  274,  308,  307,  271,  305,
      305,  305,  272,  272,  467,  273,  306,  306,  306,  304,
      274,  337,  337,  337,  273,  303,  305,  336,  336,  336,
      302,  301,  300,  306,  299,  364,  364,  364,  337,  365,
      365,  365,  298,  297,  336,  305,  296,  306,  295,  294,
      365,  337,  364,  336,  293,  292,  365,  390,  390,  390,
      391,  391,  391,  411,  411,  411,  364,  412,  412,  412,
      289,  429,  429,  429,  390,  475,  475,  391,  288,  287,
      411,  286,  469,  469,  412,  475,  285,  390,  429,  444,
      444,  444,  469,  284,  391,  453,  453,  453,  412,  283,

      475,  477,  477,  479,  479,  282,  444,  281,  429,  280,
      279,  477,  453,  479,  481,  481,  483,  483,  278,  469,
      277,  444,  485,  485,  481,  276,  483,  487,  487,  275,
      269,  268,  485,  267,  485,  266,  479,  487,  265,  264,
      263,  262,  261,  260,  477,  259,  258,  257,  256,  255,
      254,  253,  481,  483,  493,  493,  493,  494,  251,  494,
      495,  250,  495,  496,  496,  496,  249,  248,  247,  246,
      245,  244,  243,  242,  240,  239,  238,  229,  228,  227,
      226,  225,  224,  223,  222,  221,  220,  219,  218,  217,
      216,  215,  214,  213,  212,  211,  210,  209,  208,  207,

      206,  205,  204,  203,  201,  200,  199,  187,  186,  185,
      184,  183,  182,  181,  178,  177,  176,  175,  174,  173,
      172,  171,  170,  169,  168,  167,  166,  165,  164,  163,
      162,  161,  150,  149,  148,  146,  145,  144,  143,  142,
      141,  140,  139,  138,  137,  136,  135,  134,  133,  131,
      130,  128,  114,  113,  111,  109,  108,  107,  104,  103,
      101,  100,   82,   81,   80,   64,   63,   62,   61,   60,
       59,   58,   40,   39,   38,   37,   10,    3,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,

      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490,  490,
      490,  490,  490,  490,  490,  490,  490,  490,  490
    } ;

static yy_state_type yy_state_buf[YY_BUF_SIZE + 2], *yy_state_ptr;
static char *yy_full_match;
static int yy_lp;
#define REJECT \
{ \
*yy_cp = yy_hold_char; /* undo effects of setting up yytext */ \
yy_cp = yy_full_match; /* restore poss. backed-over text */ \
++yy_lp; \
goto find_rule; \
}
static int yy_more_offset = 0;
static int yy_prev_more_offset = 0;
#define yymore() (yy_more_offset = yy_flex_strlen( yytext ))
#define YY_NEED_STRLEN
#define YY_MORE_ADJ 0
#define YY_RESTORE_YY_MORE_OFFSET \
	{ \
	yy_more_offset = yy_prev_more_offset; \
	yyleng -= yy_more_offset; \
	}
#ifndef YYLMAX
#define YYLMAX 8192
#endif

char yytext[YYLMAX];
char *yytext_ptr;
#line 1 "rgschemascan.l"
#define INITIAL 0
#define YY_NEVER_INTERACTIVE 1
#line 4 "rgschemascan.l"


#include "rgstring.h"
#include "rgschemascan.tab.h"
#include "rgxmlschema.h"

extern int lineno;        /* current line number  */

RGString* s;



/*  */
#line 853 "lex.yy.c"

/* Macros after this point can all be overridden by user definitions in
 * section 1.
 */

#ifndef YY_SKIP_YYWRAP
#ifdef __cplusplus
extern "C" int yywrap YY_PROTO(( void ));
#else
extern int yywrap YY_PROTO(( void ));
#endif
#endif

#ifndef YY_NO_UNPUT
static void yyunput YY_PROTO(( int c, char *buf_ptr ));
#endif

#ifndef yytext_ptr
static void yy_flex_strncpy YY_PROTO(( char *, yyconst char *, int ));
#endif

#ifdef YY_NEED_STRLEN
static int yy_flex_strlen YY_PROTO(( yyconst char * ));
#endif

#ifndef YY_NO_INPUT
#ifdef __cplusplus
static int yyinput YY_PROTO(( void ));
#else
static int input YY_PROTO(( void ));
#endif
#endif

#if YY_STACK_USED
static int yy_start_stack_ptr = 0;
static int yy_start_stack_depth = 0;
static int *yy_start_stack = 0;
#ifndef YY_NO_PUSH_STATE
static void yy_push_state YY_PROTO(( int new_state ));
#endif
#ifndef YY_NO_POP_STATE
static void yy_pop_state YY_PROTO(( void ));
#endif
#ifndef YY_NO_TOP_STATE
static int yy_top_state YY_PROTO(( void ));
#endif

#else
#define YY_NO_PUSH_STATE 1
#define YY_NO_POP_STATE 1
#define YY_NO_TOP_STATE 1
#endif

#ifdef YY_MALLOC_DECL
YY_MALLOC_DECL
#else
#if __STDC__
#ifndef __cplusplus
#include <stdlib.h>
#endif
#else
/* Just try to get by without declaring the routines.  This will fail
 * miserably on non-ANSI systems for which sizeof(size_t) != sizeof(int)
 * or sizeof(void*) != sizeof(int).
 */
#endif
#endif

/* Amount of stuff to slurp up with each read. */
#ifndef YY_READ_BUF_SIZE
#define YY_READ_BUF_SIZE 8192
#endif

/* Copy whatever the last rule matched to the standard output. */

#ifndef ECHO
/* This used to be an fputs(), but since the string might contain NUL's,
 * we now use fwrite().
 */
#define ECHO (void) fwrite( yytext, yyleng, 1, yyout )
#endif

/* Gets input and stuffs it into "buf".  number of characters read, or YY_NULL,
 * is returned in "result".
 */
#ifndef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( yy_current_buffer->yy_is_interactive ) \
		{ \
		int c = '*', n; \
		for ( n = 0; n < max_size && \
			     (c = getc( yyin )) != EOF && c != '\n'; ++n ) \
			buf[n] = (char) c; \
		if ( c == '\n' ) \
			buf[n++] = (char) c; \
		if ( c == EOF && ferror( yyin ) ) \
			YY_FATAL_ERROR( "input in flex scanner failed" ); \
		result = n; \
		} \
	else \
		{ \
		errno=0; \
		while ( (result = fread(buf, 1, max_size, yyin))==0 && ferror(yyin)) \
			{ \
			if( errno != EINTR) \
				{ \
				YY_FATAL_ERROR( "input in flex scanner failed" ); \
				break; \
				} \
			errno=0; \
			clearerr(yyin); \
			} \
		}
#endif

/* No semi-colon after return; correct usage is to write "yyterminate();" -
 * we don't want an extra ';' after the "return" because that will cause
 * some compilers to complain about unreachable statements.
 */
#ifndef yyterminate
#define yyterminate() return YY_NULL
#endif

/* Number of entries by which start-condition stack grows. */
#ifndef YY_START_STACK_INCR
#define YY_START_STACK_INCR 25
#endif

/* Report a fatal error. */
#ifndef YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) yy_fatal_error( msg )
#endif

/* Default declaration of generated scanner - a define so the user can
 * easily add parameters.
 */
#ifndef YY_DECL
#define YY_DECL int yylex YY_PROTO(( void ))
#endif

/* Code executed at the beginning of each rule, after yytext and yyleng
 * have been set up.
 */
#ifndef YY_USER_ACTION
#define YY_USER_ACTION
#endif

/* Code executed at the end of each rule. */
#ifndef YY_BREAK
#define YY_BREAK break;
#endif

#define YY_RULE_SETUP \
	YY_USER_ACTION

YY_DECL
	{
	register yy_state_type yy_current_state;
	register char *yy_cp, *yy_bp;
	register int yy_act;

#line 25 "rgschemascan.l"


#line 1018 "lex.yy.c"

	if ( yy_init )
		{
		yy_init = 0;

#ifdef YY_USER_INIT
		YY_USER_INIT;
#endif

		if ( ! yy_start )
			yy_start = 1;	/* first start state */

		if ( ! yyin )
			yyin = stdin;

		if ( ! yyout )
			yyout = stdout;

		if ( ! yy_current_buffer )
			yy_current_buffer =
				yy_create_buffer( yyin, YY_BUF_SIZE );

		yy_load_buffer_state();
		}

	while ( 1 )		/* loops until end-of-file is reached */
		{
		yy_cp = yy_c_buf_p;

		/* Support of yytext. */
		*yy_cp = yy_hold_char;

		/* yy_bp points to the position in yy_ch_buf of the start of
		 * the current run.
		 */
		yy_bp = yy_cp;

		yy_current_state = yy_start;
		yy_state_ptr = yy_state_buf;
		*yy_state_ptr++ = yy_current_state;
yy_match:
		do
			{
			register YY_CHAR yy_c = yy_ec[YY_SC_TO_UI(*yy_cp)];
			while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
				{
				yy_current_state = (int) yy_def[yy_current_state];
				if ( yy_current_state >= 491 )
					yy_c = yy_meta[(unsigned int) yy_c];
				}
			yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
			*yy_state_ptr++ = yy_current_state;
			++yy_cp;
			}
		while ( yy_base[yy_current_state] != 1079 );

yy_find_action:
		yy_current_state = *--yy_state_ptr;
		yy_lp = yy_accept[yy_current_state];
find_rule: /* we branch to this label when backing up */
		for ( ; ; ) /* until we find what rule we matched */
			{
			if ( yy_lp && yy_lp < yy_accept[yy_current_state + 1] )
				{
				yy_act = yy_acclist[yy_lp];
					{
					yy_full_match = yy_cp;
					break;
					}
				}
			--yy_cp;
			yy_current_state = *--yy_state_ptr;
			yy_lp = yy_accept[yy_current_state];
			}

		YY_DO_BEFORE_ACTION;

		if ( yy_act != YY_END_OF_BUFFER )
			{
			int yyl;
			for ( yyl = 0; yyl < yyleng; ++yyl )
				if ( yytext[yyl] == '\n' )
					++yylineno;
			}

do_action:	/* This label is used only to access EOF actions. */


		switch ( yy_act )
	{ /* beginning of action switch */
case 1:
YY_RULE_SETUP
#line 27 "rgschemascan.l"
{ ++lineno; }
	YY_BREAK
case 2:
YY_RULE_SETUP
#line 29 "rgschemascan.l"
{/*cout << "spaces..." << endl;*/}
	YY_BREAK
case 3:
YY_RULE_SETUP
#line 30 "rgschemascan.l"
{ return RIGHTBRACKET; }
	YY_BREAK
case 4:
YY_RULE_SETUP
#line 31 "rgschemascan.l"
{ /*cout << "Found comment:  " << yytext << endl;*/ }
	YY_BREAK
case 5:
YY_RULE_SETUP
#line 32 "rgschemascan.l"
{ /*cout << "Found opening line:  " << yytext << endl;*/ }
	YY_BREAK
case 6:
YY_RULE_SETUP
#line 33 "rgschemascan.l"
{ /*cout << "Found schema start; ignoring..." << endl;*/ }
	YY_BREAK
case 7:
YY_RULE_SETUP
#line 34 "rgschemascan.l"
{ /*cout << "Found schema end; ignoring..." << endl;*/}
	YY_BREAK
case 8:
YY_RULE_SETUP
#line 35 "rgschemascan.l"
{ /*cout << "Found annotation:  " << yytext << endl;*/}
	YY_BREAK
case 9:
YY_RULE_SETUP
#line 36 "rgschemascan.l"
{;}
	YY_BREAK
case 10:
YY_RULE_SETUP
#line 37 "rgschemascan.l"
{;}
	YY_BREAK
case 11:
YY_RULE_SETUP
#line 41 "rgschemascan.l"
{ SequenceDepth++; return SEQUENCE; }
	YY_BREAK
case 12:
YY_RULE_SETUP
#line 42 "rgschemascan.l"
{ return ENDSEQUENCE; }
	YY_BREAK
case 13:
YY_RULE_SETUP
#line 43 "rgschemascan.l"
{ ElementDepth++; return BEGINELEMENT; }
	YY_BREAK
case 14:
YY_RULE_SETUP
#line 44 "rgschemascan.l"
{ return ENDELEMENT; }
	YY_BREAK
case 15:
YY_RULE_SETUP
#line 45 "rgschemascan.l"
{ return END; }
	YY_BREAK
case 16:
YY_RULE_SETUP
#line 46 "rgschemascan.l"
{ return QUOTE; }
	YY_BREAK
case 17:
YY_RULE_SETUP
#line 48 "rgschemascan.l"
{ return EQUALS; }
	YY_BREAK
case 18:
YY_RULE_SETUP
#line 49 "rgschemascan.l"
{ return NAME; }
	YY_BREAK
case 19:
YY_RULE_SETUP
#line 50 "rgschemascan.l"
{ return TYPE; }
	YY_BREAK
case 20:
YY_RULE_SETUP
#line 51 "rgschemascan.l"
{ return MINOCCURS; }
	YY_BREAK
case 21:
YY_RULE_SETUP
#line 52 "rgschemascan.l"
{ return MAXOCCURS; }
	YY_BREAK
case 22:
YY_RULE_SETUP
#line 53 "rgschemascan.l"
{ return UNBOUNDED; }
	YY_BREAK
case 23:
YY_RULE_SETUP
#line 54 "rgschemascan.l"
{ return REFERENCE; }
	YY_BREAK
case 24:
YY_RULE_SETUP
#line 55 "rgschemascan.l"
{ return REFERTOKEY; }
	YY_BREAK
case 25:
YY_RULE_SETUP
#line 56 "rgschemascan.l"
{ return BASE; }
	YY_BREAK
case 26:
YY_RULE_SETUP
#line 57 "rgschemascan.l"
{ return ABSTRACT; }
	YY_BREAK
case 27:
YY_RULE_SETUP
#line 58 "rgschemascan.l"
{ /*cout << "Found value token" << endl;*/ return VALUE; }
	YY_BREAK
case 28:
YY_RULE_SETUP
#line 59 "rgschemascan.l"
{ /*cout << "Found preserve token" << endl;*/ return WHITESPACEDATA; }
	YY_BREAK
case 29:
YY_RULE_SETUP
#line 60 "rgschemascan.l"
{ /*cout << "Found replace token" << endl;*/ return WHITESPACEDATA; }
	YY_BREAK
case 30:
YY_RULE_SETUP
#line 61 "rgschemascan.l"
{ /*cout << "Found collapse token" << endl;*/ return WHITESPACEDATA; }
	YY_BREAK
case 31:
YY_RULE_SETUP
#line 62 "rgschemascan.l"
{ return SUBSTITUTIONGROUP; }
	YY_BREAK
case 32:
YY_RULE_SETUP
#line 63 "rgschemascan.l"
{ yylval.ivalue = strtoul (yytext, 0, 10); s = new RGString (yytext); NameStack.Prepend (s);
							return STRINGNAME; }
	YY_BREAK
case 33:
YY_RULE_SETUP
#line 65 "rgschemascan.l"
{ yylval.dvalue = strtod(yytext,0);
							s = new RGString (yytext); NameStack.Prepend(s); return STRINGNAME; }
	YY_BREAK
case 34:
YY_RULE_SETUP
#line 67 "rgschemascan.l"
{ return COMPLEXSEQUENCE; }
	YY_BREAK
case 35:
YY_RULE_SETUP
#line 68 "rgschemascan.l"
{ return BEGINKEY; }
	YY_BREAK
case 36:
YY_RULE_SETUP
#line 69 "rgschemascan.l"
{ return ENDKEY; }
	YY_BREAK
case 37:
YY_RULE_SETUP
#line 70 "rgschemascan.l"
{ return SELECTOR; }
	YY_BREAK
case 38:
YY_RULE_SETUP
#line 71 "rgschemascan.l"
{ return XPATH; }
	YY_BREAK
case 39:
YY_RULE_SETUP
#line 72 "rgschemascan.l"
{ return FIELD; }
	YY_BREAK
case 40:
YY_RULE_SETUP
#line 73 "rgschemascan.l"
{ return KEYREFERENCE; }
	YY_BREAK
case 41:
YY_RULE_SETUP
#line 74 "rgschemascan.l"
{ return ENDKEYREFERENCE; }
	YY_BREAK
case 42:
YY_RULE_SETUP
#line 75 "rgschemascan.l"
{ return INCLUDE; }
	YY_BREAK
case 43:
YY_RULE_SETUP
#line 76 "rgschemascan.l"
{ return SCHEMALOCATION; }
	YY_BREAK
case 44:
YY_RULE_SETUP
#line 77 "rgschemascan.l"
{ ElementDepth++; return COMPLEXTYPE; }
	YY_BREAK
case 45:
YY_RULE_SETUP
#line 78 "rgschemascan.l"
{ return ENDCOMPLEXTYPE; }
	YY_BREAK
case 46:
YY_RULE_SETUP
#line 79 "rgschemascan.l"
{ ElementDepth++; return SIMPLETYPE; }
	YY_BREAK
case 47:
YY_RULE_SETUP
#line 80 "rgschemascan.l"
{ return ENDSIMPLETYPE; }
	YY_BREAK
case 48:
YY_RULE_SETUP
#line 81 "rgschemascan.l"
{ return EXTENSION; }
	YY_BREAK
case 49:
YY_RULE_SETUP
#line 82 "rgschemascan.l"
{ return ENDEXTENSION; }
	YY_BREAK
case 50:
YY_RULE_SETUP
#line 83 "rgschemascan.l"
{ return RESTRICTION; }
	YY_BREAK
case 51:
YY_RULE_SETUP
#line 84 "rgschemascan.l"
{ return ENDRESTRICTION; }
	YY_BREAK
case 52:
YY_RULE_SETUP
#line 85 "rgschemascan.l"
{ return MININCLUSIVE; }
	YY_BREAK
case 53:
YY_RULE_SETUP
#line 86 "rgschemascan.l"
{ return MAXINCLUSIVE; }
	YY_BREAK
case 54:
YY_RULE_SETUP
#line 87 "rgschemascan.l"
{ return MINEXCLUSIVE; }
	YY_BREAK
case 55:
YY_RULE_SETUP
#line 88 "rgschemascan.l"
{ return MAXEXCLUSIVE; }
	YY_BREAK
case 56:
YY_RULE_SETUP
#line 89 "rgschemascan.l"
{ return ENUMERATION; }
	YY_BREAK
case 57:
YY_RULE_SETUP
#line 90 "rgschemascan.l"
{ return LENGTH; }
	YY_BREAK
case 58:
YY_RULE_SETUP
#line 91 "rgschemascan.l"
{ return MAXLENGTH; }
	YY_BREAK
case 59:
YY_RULE_SETUP
#line 92 "rgschemascan.l"
{ return MINLENGTH; }
	YY_BREAK
case 60:
YY_RULE_SETUP
#line 93 "rgschemascan.l"
{ return TOTALDIGITS; }
	YY_BREAK
case 61:
YY_RULE_SETUP
#line 94 "rgschemascan.l"
{ return FRACTIONDIGITS; }
	YY_BREAK
case 62:
YY_RULE_SETUP
#line 95 "rgschemascan.l"
{ /*cout << "Found whitespace token" << endl;*/ return WHITESPACE; }
	YY_BREAK
case YY_STATE_EOF(INITIAL):
#line 96 "rgschemascan.l"
{ return RGEOF; }
	YY_BREAK
case 63:
YY_RULE_SETUP
#line 98 "rgschemascan.l"
{ s = new RGString (yytext); NameStack.Prepend (s); return STRINGNAME; }
	YY_BREAK
case 64:
YY_RULE_SETUP
#line 100 "rgschemascan.l"
{ return yytext[0]; }	/* needed to return all other characters. */
	YY_BREAK
case 65:
YY_RULE_SETUP
#line 104 "rgschemascan.l"
ECHO;
	YY_BREAK
#line 1440 "lex.yy.c"

	case YY_END_OF_BUFFER:
		{
		/* Amount of text matched not including the EOB char. */
		int yy_amount_of_matched_text = (int) (yy_cp - yytext_ptr) - 1;

		/* Undo the effects of YY_DO_BEFORE_ACTION. */
		*yy_cp = yy_hold_char;
		YY_RESTORE_YY_MORE_OFFSET

		if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_NEW )
			{
			/* We're scanning a new file or input source.  It's
			 * possible that this happened because the user
			 * just pointed yyin at a new source and called
			 * yylex().  If so, then we have to assure
			 * consistency between yy_current_buffer and our
			 * globals.  Here is the right place to do so, because
			 * this is the first action (other than possibly a
			 * back-up) that will match for the new input source.
			 */
			yy_n_chars = yy_current_buffer->yy_n_chars;
			yy_current_buffer->yy_input_file = yyin;
			yy_current_buffer->yy_buffer_status = YY_BUFFER_NORMAL;
			}

		/* Note that here we test for yy_c_buf_p "<=" to the position
		 * of the first EOB in the buffer, since yy_c_buf_p will
		 * already have been incremented past the NUL character
		 * (since all states make transitions on EOB to the
		 * end-of-buffer state).  Contrast this with the test
		 * in input().
		 */
		if ( yy_c_buf_p <= &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			{ /* This was really a NUL. */
			yy_state_type yy_next_state;

			yy_c_buf_p = yytext_ptr + yy_amount_of_matched_text;

			yy_current_state = yy_get_previous_state();

			/* Okay, we're now positioned to make the NUL
			 * transition.  We couldn't have
			 * yy_get_previous_state() go ahead and do it
			 * for us because it doesn't know how to deal
			 * with the possibility of jamming (and we don't
			 * want to build jamming into it because then it
			 * will run more slowly).
			 */

			yy_next_state = yy_try_NUL_trans( yy_current_state );

			yy_bp = yytext_ptr + YY_MORE_ADJ;

			if ( yy_next_state )
				{
				/* Consume the NUL. */
				yy_cp = ++yy_c_buf_p;
				yy_current_state = yy_next_state;
				goto yy_match;
				}

			else
				{
				yy_cp = yy_c_buf_p;
				goto yy_find_action;
				}
			}

		else switch ( yy_get_next_buffer() )
			{
			case EOB_ACT_END_OF_FILE:
				{
				yy_did_buffer_switch_on_eof = 0;

				if ( yywrap() )
					{
					/* Note: because we've taken care in
					 * yy_get_next_buffer() to have set up
					 * yytext, we can now set up
					 * yy_c_buf_p so that if some total
					 * hoser (like flex itself) wants to
					 * call the scanner after we return the
					 * YY_NULL, it'll still work - another
					 * YY_NULL will get returned.
					 */
					yy_c_buf_p = yytext_ptr + YY_MORE_ADJ;

					yy_act = YY_STATE_EOF(YY_START);
					goto do_action;
					}

				else
					{
					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
					}
				break;
				}

			case EOB_ACT_CONTINUE_SCAN:
				yy_c_buf_p =
					yytext_ptr + yy_amount_of_matched_text;

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_match;

			case EOB_ACT_LAST_MATCH:
				yy_c_buf_p =
				&yy_current_buffer->yy_ch_buf[yy_n_chars];

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_find_action;
			}
		break;
		}

	default:
		YY_FATAL_ERROR(
			"fatal flex scanner internal error--no action found" );
	} /* end of action switch */
		} /* end of scanning one token */
	} /* end of yylex */


/* yy_get_next_buffer - try to read in a new buffer
 *
 * Returns a code representing an action:
 *	EOB_ACT_LAST_MATCH -
 *	EOB_ACT_CONTINUE_SCAN - continue scanning from current position
 *	EOB_ACT_END_OF_FILE - end of file
 */

static int yy_get_next_buffer()
	{
	register char *dest = yy_current_buffer->yy_ch_buf;
	register char *source = yytext_ptr;
	register int number_to_move, i;
	int ret_val;

	if ( yy_c_buf_p > &yy_current_buffer->yy_ch_buf[yy_n_chars + 1] )
		YY_FATAL_ERROR(
		"fatal flex scanner internal error--end of buffer missed" );

	if ( yy_current_buffer->yy_fill_buffer == 0 )
		{ /* Don't try to fill the buffer, so this is an EOF. */
		if ( yy_c_buf_p - yytext_ptr - YY_MORE_ADJ == 1 )
			{
			/* We matched a single character, the EOB, so
			 * treat this as a final EOF.
			 */
			return EOB_ACT_END_OF_FILE;
			}

		else
			{
			/* We matched some text prior to the EOB, first
			 * process it.
			 */
			return EOB_ACT_LAST_MATCH;
			}
		}

	/* Try to read more data. */

	/* First move last chars to start of buffer. */
	number_to_move = (int) (yy_c_buf_p - yytext_ptr) - 1;

	for ( i = 0; i < number_to_move; ++i )
		*(dest++) = *(source++);

	if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_EOF_PENDING )
		/* don't do the read, it's not guaranteed to return an EOF,
		 * just force an EOF
		 */
		yy_current_buffer->yy_n_chars = yy_n_chars = 0;

	else
		{
		int num_to_read =
			yy_current_buffer->yy_buf_size - number_to_move - 1;

		while ( num_to_read <= 0 )
			{ /* Not enough room in the buffer - grow it. */
#ifdef YY_USES_REJECT
			YY_FATAL_ERROR(
"input buffer overflow, can't enlarge buffer because scanner uses REJECT" );
#else

			/* just a shorter name for the current buffer */
			YY_BUFFER_STATE b = yy_current_buffer;

			int yy_c_buf_p_offset =
				(int) (yy_c_buf_p - b->yy_ch_buf);

			if ( b->yy_is_our_buffer )
				{
				int new_size = b->yy_buf_size * 2;

				if ( new_size <= 0 )
					b->yy_buf_size += b->yy_buf_size / 8;
				else
					b->yy_buf_size *= 2;

				b->yy_ch_buf = (char *)
					/* Include room in for 2 EOB chars. */
					yy_flex_realloc( (void *) b->yy_ch_buf,
							 b->yy_buf_size + 2 );
				}
			else
				/* Can't grow it, we don't own it. */
				b->yy_ch_buf = 0;

			if ( ! b->yy_ch_buf )
				YY_FATAL_ERROR(
				"fatal error - scanner input buffer overflow" );

			yy_c_buf_p = &b->yy_ch_buf[yy_c_buf_p_offset];

			num_to_read = yy_current_buffer->yy_buf_size -
						number_to_move - 1;
#endif
			}

		if ( num_to_read > YY_READ_BUF_SIZE )
			num_to_read = YY_READ_BUF_SIZE;

		/* Read in more data. */
		YY_INPUT( (&yy_current_buffer->yy_ch_buf[number_to_move]),
			yy_n_chars, num_to_read );

		yy_current_buffer->yy_n_chars = yy_n_chars;
		}

	if ( yy_n_chars == 0 )
		{
		if ( number_to_move == YY_MORE_ADJ )
			{
			ret_val = EOB_ACT_END_OF_FILE;
			yyrestart( yyin );
			}

		else
			{
			ret_val = EOB_ACT_LAST_MATCH;
			yy_current_buffer->yy_buffer_status =
				YY_BUFFER_EOF_PENDING;
			}
		}

	else
		ret_val = EOB_ACT_CONTINUE_SCAN;

	yy_n_chars += number_to_move;
	yy_current_buffer->yy_ch_buf[yy_n_chars] = YY_END_OF_BUFFER_CHAR;
	yy_current_buffer->yy_ch_buf[yy_n_chars + 1] = YY_END_OF_BUFFER_CHAR;

	yytext_ptr = &yy_current_buffer->yy_ch_buf[0];

	return ret_val;
	}


/* yy_get_previous_state - get the state just before the EOB char was reached */

static yy_state_type yy_get_previous_state()
	{
	register yy_state_type yy_current_state;
	register char *yy_cp;

	yy_current_state = yy_start;
	yy_state_ptr = yy_state_buf;
	*yy_state_ptr++ = yy_current_state;

	for ( yy_cp = yytext_ptr + YY_MORE_ADJ; yy_cp < yy_c_buf_p; ++yy_cp )
		{
		register YY_CHAR yy_c = (*yy_cp ? yy_ec[YY_SC_TO_UI(*yy_cp)] : 1);
		while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
			{
			yy_current_state = (int) yy_def[yy_current_state];
			if ( yy_current_state >= 491 )
				yy_c = yy_meta[(unsigned int) yy_c];
			}
		yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
		*yy_state_ptr++ = yy_current_state;
		}

	return yy_current_state;
	}


/* yy_try_NUL_trans - try to make a transition on the NUL character
 *
 * synopsis
 *	next_state = yy_try_NUL_trans( current_state );
 */

#ifdef YY_USE_PROTOS
static yy_state_type yy_try_NUL_trans( yy_state_type yy_current_state )
#else
static yy_state_type yy_try_NUL_trans( yy_current_state )
yy_state_type yy_current_state;
#endif
	{
	register int yy_is_jam;

	register YY_CHAR yy_c = 1;
	while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
		{
		yy_current_state = (int) yy_def[yy_current_state];
		if ( yy_current_state >= 491 )
			yy_c = yy_meta[(unsigned int) yy_c];
		}
	yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
	yy_is_jam = (yy_current_state == 490);
	if ( ! yy_is_jam )
		*yy_state_ptr++ = yy_current_state;

	return yy_is_jam ? 0 : yy_current_state;
	}


#ifndef YY_NO_UNPUT
#ifdef YY_USE_PROTOS
static void yyunput( int c, register char *yy_bp )
#else
static void yyunput( c, yy_bp )
int c;
register char *yy_bp;
#endif
	{
	register char *yy_cp = yy_c_buf_p;

	/* undo effects of setting up yytext */
	*yy_cp = yy_hold_char;

	if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
		{ /* need to shift things up to make room */
		/* +2 for EOB chars. */
		register int number_to_move = yy_n_chars + 2;
		register char *dest = &yy_current_buffer->yy_ch_buf[
					yy_current_buffer->yy_buf_size + 2];
		register char *source =
				&yy_current_buffer->yy_ch_buf[number_to_move];

		while ( source > yy_current_buffer->yy_ch_buf )
			*--dest = *--source;

		yy_cp += (int) (dest - source);
		yy_bp += (int) (dest - source);
		yy_current_buffer->yy_n_chars =
			yy_n_chars = yy_current_buffer->yy_buf_size;

		if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
			YY_FATAL_ERROR( "flex scanner push-back overflow" );
		}

	*--yy_cp = (char) c;

	if ( c == '\n' )
		--yylineno;

	yytext_ptr = yy_bp;
	yy_hold_char = *yy_cp;
	yy_c_buf_p = yy_cp;
	}
#endif	/* ifndef YY_NO_UNPUT */


#ifdef __cplusplus
static int yyinput()
#else
static int input()
#endif
	{
	int c;

	*yy_c_buf_p = yy_hold_char;

	if ( *yy_c_buf_p == YY_END_OF_BUFFER_CHAR )
		{
		/* yy_c_buf_p now points to the character we want to return.
		 * If this occurs *before* the EOB characters, then it's a
		 * valid NUL; if not, then we've hit the end of the buffer.
		 */
		if ( yy_c_buf_p < &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			/* This was really a NUL. */
			*yy_c_buf_p = '\0';

		else
			{ /* need more input */
			int offset = yy_c_buf_p - yytext_ptr;
			++yy_c_buf_p;

			switch ( yy_get_next_buffer() )
				{
				case EOB_ACT_LAST_MATCH:
					/* This happens because yy_g_n_b()
					 * sees that we've accumulated a
					 * token and flags that we need to
					 * try matching the token before
					 * proceeding.  But for input(),
					 * there's no matching to consider.
					 * So convert the EOB_ACT_LAST_MATCH
					 * to EOB_ACT_END_OF_FILE.
					 */

					/* Reset buffer status. */
					yyrestart( yyin );

					/* fall through */

				case EOB_ACT_END_OF_FILE:
					{
					if ( yywrap() )
						return EOF;

					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
#ifdef __cplusplus
					return yyinput();
#else
					return input();
#endif
					}

				case EOB_ACT_CONTINUE_SCAN:
					yy_c_buf_p = yytext_ptr + offset;
					break;
				}
			}
		}

	c = *(unsigned char *) yy_c_buf_p;	/* cast for 8-bit char's */
	*yy_c_buf_p = '\0';	/* preserve yytext */
	yy_hold_char = *++yy_c_buf_p;

	if ( c == '\n' )
		++yylineno;

	return c;
	}


#ifdef YY_USE_PROTOS
void yyrestart( FILE *input_file )
#else
void yyrestart( input_file )
FILE *input_file;
#endif
	{
	if ( ! yy_current_buffer )
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE );

	yy_init_buffer( yy_current_buffer, input_file );
	yy_load_buffer_state();
	}


#ifdef YY_USE_PROTOS
void yy_switch_to_buffer( YY_BUFFER_STATE new_buffer )
#else
void yy_switch_to_buffer( new_buffer )
YY_BUFFER_STATE new_buffer;
#endif
	{
	if ( yy_current_buffer == new_buffer )
		return;

	if ( yy_current_buffer )
		{
		/* Flush out information for old buffer. */
		*yy_c_buf_p = yy_hold_char;
		yy_current_buffer->yy_buf_pos = yy_c_buf_p;
		yy_current_buffer->yy_n_chars = yy_n_chars;
		}

	yy_current_buffer = new_buffer;
	yy_load_buffer_state();

	/* We don't actually know whether we did this switch during
	 * EOF (yywrap()) processing, but the only time this flag
	 * is looked at is after yywrap() is called, so it's safe
	 * to go ahead and always set it.
	 */
	yy_did_buffer_switch_on_eof = 1;
	}


#ifdef YY_USE_PROTOS
void yy_load_buffer_state( void )
#else
void yy_load_buffer_state()
#endif
	{
	yy_n_chars = yy_current_buffer->yy_n_chars;
	yytext_ptr = yy_c_buf_p = yy_current_buffer->yy_buf_pos;
	yyin = yy_current_buffer->yy_input_file;
	yy_hold_char = *yy_c_buf_p;
	}


#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_create_buffer( FILE *file, int size )
#else
YY_BUFFER_STATE yy_create_buffer( file, size )
FILE *file;
int size;
#endif
	{
	YY_BUFFER_STATE b;

	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
	if ( ! b )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_buf_size = size;

	/* yy_ch_buf has to be 2 characters longer than the size given because
	 * we need to put in 2 end-of-buffer characters.
	 */
	b->yy_ch_buf = (char *) yy_flex_alloc( b->yy_buf_size + 2 );
	if ( ! b->yy_ch_buf )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_is_our_buffer = 1;

	yy_init_buffer( b, file );

	return b;
	}


#ifdef YY_USE_PROTOS
void yy_delete_buffer( YY_BUFFER_STATE b )
#else
void yy_delete_buffer( b )
YY_BUFFER_STATE b;
#endif
	{
	if ( ! b )
		return;

	if ( b == yy_current_buffer )
		yy_current_buffer = (YY_BUFFER_STATE) 0;

	if ( b->yy_is_our_buffer )
		yy_flex_free( (void *) b->yy_ch_buf );

	yy_flex_free( (void *) b );
	}


#ifndef _WIN32
#include <unistd.h>
#else
#ifndef YY_ALWAYS_INTERACTIVE
#ifndef YY_NEVER_INTERACTIVE
extern int isatty YY_PROTO(( int ));
#endif
#endif
#endif

#ifdef YY_USE_PROTOS
void yy_init_buffer( YY_BUFFER_STATE b, FILE *file )
#else
void yy_init_buffer( b, file )
YY_BUFFER_STATE b;
FILE *file;
#endif


	{
	yy_flush_buffer( b );

	b->yy_input_file = file;
	b->yy_fill_buffer = 1;

#if YY_ALWAYS_INTERACTIVE
	b->yy_is_interactive = 1;
#else
#if YY_NEVER_INTERACTIVE
	b->yy_is_interactive = 0;
#else
	b->yy_is_interactive = file ? (isatty( fileno(file) ) > 0) : 0;
#endif
#endif
	}


#ifdef YY_USE_PROTOS
void yy_flush_buffer( YY_BUFFER_STATE b )
#else
void yy_flush_buffer( b )
YY_BUFFER_STATE b;
#endif

	{
	if ( ! b )
		return;

	b->yy_n_chars = 0;

	/* We always need two end-of-buffer characters.  The first causes
	 * a transition to the end-of-buffer state.  The second causes
	 * a jam in that state.
	 */
	b->yy_ch_buf[0] = YY_END_OF_BUFFER_CHAR;
	b->yy_ch_buf[1] = YY_END_OF_BUFFER_CHAR;

	b->yy_buf_pos = &b->yy_ch_buf[0];

	b->yy_at_bol = 1;
	b->yy_buffer_status = YY_BUFFER_NEW;

	if ( b == yy_current_buffer )
		yy_load_buffer_state();
	}


#ifndef YY_NO_SCAN_BUFFER
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_buffer( char *base, yy_size_t size )
#else
YY_BUFFER_STATE yy_scan_buffer( base, size )
char *base;
yy_size_t size;
#endif
	{
	YY_BUFFER_STATE b;

	if ( size < 2 ||
	     base[size-2] != YY_END_OF_BUFFER_CHAR ||
	     base[size-1] != YY_END_OF_BUFFER_CHAR )
		/* They forgot to leave room for the EOB's. */
		return 0;

	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
	if ( ! b )
		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_buffer()" );

	b->yy_buf_size = size - 2;	/* "- 2" to take care of EOB's */
	b->yy_buf_pos = b->yy_ch_buf = base;
	b->yy_is_our_buffer = 0;
	b->yy_input_file = 0;
	b->yy_n_chars = b->yy_buf_size;
	b->yy_is_interactive = 0;
	b->yy_at_bol = 1;
	b->yy_fill_buffer = 0;
	b->yy_buffer_status = YY_BUFFER_NEW;

	yy_switch_to_buffer( b );

	return b;
	}
#endif


#ifndef YY_NO_SCAN_STRING
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_string( yyconst char *yy_str )
#else
YY_BUFFER_STATE yy_scan_string( yy_str )
yyconst char *yy_str;
#endif
	{
	int len;
	for ( len = 0; yy_str[len]; ++len )
		;

	return yy_scan_bytes( yy_str, len );
	}
#endif


#ifndef YY_NO_SCAN_BYTES
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_bytes( yyconst char *bytes, int len )
#else
YY_BUFFER_STATE yy_scan_bytes( bytes, len )
yyconst char *bytes;
int len;
#endif
	{
	YY_BUFFER_STATE b;
	char *buf;
	yy_size_t n;
	int i;

	/* Get memory for full buffer, including space for trailing EOB's. */
	n = len + 2;
	buf = (char *) yy_flex_alloc( n );
	if ( ! buf )
		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_bytes()" );

	for ( i = 0; i < len; ++i )
		buf[i] = bytes[i];

	buf[len] = buf[len+1] = YY_END_OF_BUFFER_CHAR;

	b = yy_scan_buffer( buf, n );
	if ( ! b )
		YY_FATAL_ERROR( "bad buffer in yy_scan_bytes()" );

	/* It's okay to grow etc. this buffer, and we should throw it
	 * away when we're done.
	 */
	b->yy_is_our_buffer = 1;

	return b;
	}
#endif


#ifndef YY_NO_PUSH_STATE
#ifdef YY_USE_PROTOS
static void yy_push_state( int new_state )
#else
static void yy_push_state( new_state )
int new_state;
#endif
	{
	if ( yy_start_stack_ptr >= yy_start_stack_depth )
		{
		yy_size_t new_size;

		yy_start_stack_depth += YY_START_STACK_INCR;
		new_size = yy_start_stack_depth * sizeof( int );

		if ( ! yy_start_stack )
			yy_start_stack = (int *) yy_flex_alloc( new_size );

		else
			yy_start_stack = (int *) yy_flex_realloc(
					(void *) yy_start_stack, new_size );

		if ( ! yy_start_stack )
			YY_FATAL_ERROR(
			"out of memory expanding start-condition stack" );
		}

	yy_start_stack[yy_start_stack_ptr++] = YY_START;

	BEGIN(new_state);
	}
#endif


#ifndef YY_NO_POP_STATE
static void yy_pop_state()
	{
	if ( --yy_start_stack_ptr < 0 )
		YY_FATAL_ERROR( "start-condition stack underflow" );

	BEGIN(yy_start_stack[yy_start_stack_ptr]);
	}
#endif


#ifndef YY_NO_TOP_STATE
static int yy_top_state()
	{
	return yy_start_stack[yy_start_stack_ptr - 1];
	}
#endif

#ifndef YY_EXIT_FAILURE
#define YY_EXIT_FAILURE 2
#endif

#ifdef YY_USE_PROTOS
static void yy_fatal_error( yyconst char msg[] )
#else
static void yy_fatal_error( msg )
char msg[];
#endif
	{
	(void) fprintf( stderr, "%s\n", msg );
	exit( YY_EXIT_FAILURE );
	}



/* Redefine yyless() so it works in section 3 code. */

#undef yyless
#define yyless(n) \
	do \
		{ \
		/* Undo effects of setting up yytext. */ \
		yytext[yyleng] = yy_hold_char; \
		yy_c_buf_p = yytext + n; \
		yy_hold_char = *yy_c_buf_p; \
		*yy_c_buf_p = '\0'; \
		yyleng = n; \
		} \
	while ( 0 )


/* Internal utility routines. */

#ifndef yytext_ptr
#ifdef YY_USE_PROTOS
static void yy_flex_strncpy( char *s1, yyconst char *s2, int n )
#else
static void yy_flex_strncpy( s1, s2, n )
char *s1;
yyconst char *s2;
int n;
#endif
	{
	register int i;
	for ( i = 0; i < n; ++i )
		s1[i] = s2[i];
	}
#endif

#ifdef YY_NEED_STRLEN
#ifdef YY_USE_PROTOS
static int yy_flex_strlen( yyconst char *s )
#else
static int yy_flex_strlen( s )
yyconst char *s;
#endif
	{
	register int n;
	for ( n = 0; s[n]; ++n )
		;

	return n;
	}
#endif


#ifdef YY_USE_PROTOS
static void *yy_flex_alloc( yy_size_t size )
#else
static void *yy_flex_alloc( size )
yy_size_t size;
#endif
	{
	return (void *) malloc( size );
	}

#ifdef YY_USE_PROTOS
static void *yy_flex_realloc( void *ptr, yy_size_t size )
#else
static void *yy_flex_realloc( ptr, size )
void *ptr;
yy_size_t size;
#endif
	{
	/* The cast to (char *) in the following accommodates both
	 * implementations that use char* generic pointers, and those
	 * that use void* generic pointers.  It works with the latter
	 * because both ANSI C and C++ allow castless assignment from
	 * any pointer type to void*, and deal with argument conversions
	 * as though doing an assignment.
	 */
	return (void *) realloc( (char *) ptr, size );
	}

#ifdef YY_USE_PROTOS
static void yy_flex_free( void *ptr )
#else
static void yy_flex_free( ptr )
void *ptr;
#endif
	{
	free( ptr );
	}

#if YY_MAIN
int main()
	{
	yylex();
	return 0;
	}
#endif
#line 104 "rgschemascan.l"

