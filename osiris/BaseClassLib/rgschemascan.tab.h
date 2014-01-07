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
*  FileName: rgschemascan.tab.h
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




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 94 "rgschemascan.y"
typedef union YYSTYPE {
  
  unsigned int   ivalue;
  double		 dvalue;
} YYSTYPE;
/* Line 1251 of yacc.c.  */
#line 149 "rgschemascan.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



