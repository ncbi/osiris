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
*  FileName: nwxGlobalObject.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_GLOBAL_OBJECT_H__
#define __NWX_GLOBAL_OBJECT_H__
// stdio.h - defines NULL
#include <stdio.h>
template <class T> class nwxGlobalObject
{
public:
  nwxGlobalObject<T>() {}
  virtual ~nwxGlobalObject<T>()
  {
    T::CleanupGlobal();
  }
};

#define nwxDECLARE_GLOBAL_OBJECT_base(T) \
  static void CleanupGlobal()       \
  {                                 \
    if(g_pGlobal != NULL)           \
    { delete g_pGlobal;             \
      g_pGlobal = NULL;             \
    }                               \
  }                                 \
private:                            \
  static T *g_pGlobal;              \
  static nwxGlobalObject<T> XX_MANAGER;


#define nwxDECLARE_GLOBAL_OBJECT(T) \
public:                             \
  static T *GetGlobal()             \
  {                                 \
    if(g_pGlobal == NULL)           \
    { g_pGlobal = new T;            \
    }                               \
    return g_pGlobal;               \
  }                                 \
nwxDECLARE_GLOBAL_OBJECT_base(T)

#define nwxDECLARE_GLOBAL_OBJECT_XML(T)   \
public:                                   \
  static T *GetGlobal()                   \
  {                                       \
    if(g_pGlobal == NULL)                 \
    {                                     \
      g_pGlobal = new T;                  \
    }                                     \
    else                                  \
    {                                     \
      g_pGlobal->CheckFileModification(); \
    }                                     \
    return g_pGlobal;                     \
  }                                       \
nwxDECLARE_GLOBAL_OBJECT_base(T)


#define nwxIMPLEMENT_GLOBAL_OBJECT(T) \
T *T::g_pGlobal(NULL);                \
nwxGlobalObject<T> T::XX_MANAGER;

#define nwxGET_GLOBAL(T) T::GetGlobal()

#endif
