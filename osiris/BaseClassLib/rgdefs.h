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
*  FileName: rgdefs.h
*  Author:   Robert Goor
*
*/
//
// Definitions of constants, etc
//

#ifndef _RGDEFS_H_
#define _RGDEFS_H_

#include <limits.h>
#include <float.h>
#ifndef _WINDOWS
#include <sys/types.h>
#endif

#define Boolean int
#ifndef TRUE
#define TRUE true
#endif
#ifndef FALSE
#define FALSE false
#endif

#define dNAN -1.0e99
#define rgNAN 999999999

#ifdef _I32_MAX

#define INTMAX _I32_MAX
#define INTMIN _I32_MIN

#define UINTMAX _UI32_MAX
#define UINTMIN 0

// THIS SHOULD BE UINTMAX, UINTMIN
#define ULONGMAX _UI32_MAX
#define ULONGMIN 0

#else

#ifdef INT_MIN

#define INTMAX INT_MAX
#define INTMIN INT_MIN

#else

#define INTMAX 0x7fffffff
#define INTMIN 0x80000000
#endif

#define UINTMAX 0xffffffff
#define UINTMIN 0

#define ULONGMAX 0xffffffff
#define ULONGMIN 0

#endif

#ifdef DBL_MAX

#define DOUBLEMAX DBL_MAX
#define DOUBLEMIN -DBL_MAX

#else

#define DOUBLEMAX 1.7E+308 
#define DOUBLEMIN -1.7E+308

#endif

#ifdef _I32
typedef _I32 INT32;
#else
typedef int INT32;
#endif

#ifdef _U32
typedef _U32 UINT32;
#else
typedef unsigned int UINT32;
#endif

#ifdef _I16
typedef _I16 INT16;
#else
typedef short INT16;
#endif

#ifdef _U16
typedef _U16 UINT16;
#else
typedef unsigned short UINT16;
#endif

typedef unsigned char UINT8;

#ifdef _WIN32
typedef __int64 INT64;
#else
typedef int64_t INT64;
#endif

#ifdef _WIN32
#define snprintf _snprintf
#endif

#ifdef _WIN32
#define gcvt _gcvt
#define lcvt _lcvt
#define ecvt _ecvt
#define fcvt _fcvt
#define fcloseall _fcloseall
#define strdup _strdup
#endif

#ifndef _WIN32

extern char *_ltoa(long value, char *buffer, int radix);
extern char *_ultoa(unsigned long value, char *buffer, int radix);
extern char *itoa(int value, char *buffer, int radix);
#endif


#endif  /*  _RGDEFS_H_  */
