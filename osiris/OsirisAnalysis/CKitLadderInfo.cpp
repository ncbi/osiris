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
*  FileName: CKitLadderInfo.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CKitLadderInfo.h"

void CKitLadderInfo::_Cleanup()
{
  setptr<CKitLadderLocus, CKitLadderLocusLess>::cleanup(&m_setLocus);
}

CKitLadderInfo::~CKitLadderInfo()
{
  _Cleanup();
}

CKitLadderInfo::CKitLadderInfo()
  : m_setLS(_T("LSName"))
{
  RegisterAll(true);
}

CKitLadderInfo::CKitLadderInfo(
  const CKitLadderInfo &x) : m_setLS(_T("LSName"))
{
  RegisterAll(true);
  (*this) = x;
}

CKitLadderInfo &CKitLadderInfo::operator =(const CKitLadderInfo &x)
{
  Init();
  m_sName = x.m_sName;
  m_setLocus = x.m_setLocus;
  m_setLS = x.m_setLS;
  return *this;
}

void CKitLadderInfo::Init()
{
  _Cleanup();
  m_sName.Empty();
  m_setLS->clear();
}
void CKitLadderInfo::Init(void *p)
{
  if(p == (void *)this)
  {
    Init();
  }
}

void CKitLadderInfo::RegisterAll(bool bInit)
{
  if(bInit)
  {
    Init();
  }
  Register(_T("Kits"),this);
  Register(_T("Set"),this);
  RegisterWxString(_T("Name"),&m_sName);
  Register(_T("LS"),&m_setLS);
  Register(_T("Locus"),&m_setIO,(void *)&m_setLocus);
}
