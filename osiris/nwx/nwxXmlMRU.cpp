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

*  FileName: nwxXmlMRU.cpp
*  Author:   Douglas Hoffman
*
*/

#include "nwx/nwxXmlMRU.h"
#include <wx/file.h>
#include <wx/filefn.h>

#include "nwx/stdb.h"
#include <memory>
#include "nwx/stde.h"

#include "nwx/nsstd.h"
#include "nwx/CIncrementer.h"

//const wxChar * const nwxMRU::MRU_TIME_FORMAT("%x %X");
const wxString nwxXmlMRU::g_sMRU("MRU");

nwxMRU::nwxMRU(const wxString &sPath) : m_pfn(NULL)
{
  wxFileName fn(sPath);
  if(fn.FileExists())
  {
    wxDateTime tCreate;
    m_sPath = fn.GetFullPath();
    fn.GetTimes(&m_timeAccess,&m_timeMod,&tCreate);
  }
  else
  {
    m_sPath = sPath;
    m_timeMod.Set((time_t) 0);
    m_timeAccess.Set((time_t) 0);
  }
  RegisterAll(true);
}
int nwxMRU::CompareModTime()
{
  // return 1 if file has been modified since being added to XML MRU
  //   -1 if file date/time is older than in XML MRU
  //   0 if they are equal
  //   -2 if the file cannot be read or found

  int nRtn = 0;
  wxFileName fn(m_sPath);
  if(!fn.IsFileReadable())
  {
    nRtn = -2;
  }
  else
  {
    time_t tMRUtime = m_timeMod.GetTicks();
    time_t tModTime = fn.GetModificationTime().GetTicks();
    if(tModTime < tMRUtime)
    {
      nRtn = -1;
    }
    else if(tModTime > tMRUtime)
    {
      nRtn = 1;
    }
  }
  return nRtn;
}
bool nwxMRU::UpdateModTime()
{
  wxFileName fn(m_sPath);
  bool bRtn = false;
  if(fn.FileExists())
  {
    wxDateTime timeMod = fn.GetModificationTime();
    if(timeMod != m_timeMod)
    {
      bRtn = true;
      m_timeMod = timeMod;
    }
  }
  else
  {
    m_timeMod.Set((time_t)0);
  }
  return bRtn;
}

int nwxMRU::CompareName(const nwxMRU &x) const
{
  _SetupFn();
  x._SetupFn();
  wxString s1;
  wxString s2;
  if(m_pfn->IsOk())
  {
    s1 = m_pfn->GetFullName();
  }
  if(x.m_pfn->IsOk())
  {
    s2 = x.m_pfn->GetFullName();
  }
  int nRtn = s1.CmpNoCase(s2);
  return nRtn;
}
int nwxMRU::CompareTimeAccess(const nwxMRU &x) const
{
  int nRtn = 0;
  if(m_timeAccess < x.m_timeAccess)
  {
    nRtn = -1;
  }
  else if(m_timeAccess > x.m_timeAccess)
  {
    nRtn = 1;
  }
  return nRtn;
}
int nwxMRU::CompareTimeModified(const nwxMRU &x) const
{
  int nRtn = 0;
  if(m_timeMod < x.m_timeMod)
  {
    nRtn = -1;
  }
  else if(m_timeMod > x.m_timeMod)
  {
    nRtn = 1;
  }
  return nRtn;
}
int nwxMRU::CompareLocation(const nwxMRU &x) const
{
  wxString s1;
  wxString s2;
  _SetupFn();
  x._SetupFn();
  if(m_pfn->IsOk()) { s1 = m_pfn->GetPath();}
  if(x.m_pfn->IsOk()) { s2 = x.m_pfn->GetPath();}
  int nRtn = s1.CmpNoCase(s2);
  return nRtn;
}



const size_t nwxXmlMRU::DEFAULT_MAX_MRU_SIZE(1000);

void nwxXmlMRU::RegisterAll(bool)
{
  Register("Files",&m_setMRU);
}
bool nwxXmlMRU::BeginBatch(int nWait)
{
  bool bRtn = false;
  if(m_nBatch)
  {
    bRtn = UpdateLock();
  }
  else if(Lock(nWait))
  {
    bRtn = true;
    CheckFileModification(true);
  }
  if(bRtn)
  {
    m_nBatch++;
  }
  return bRtn;
}
void nwxXmlMRU::EndBatch()
{
  if(m_nBatch > 0)
  {
    m_nBatch--;
    if(!m_nBatch)
    {
      Save();
      Unlock();
    }
  }
}

bool nwxXmlMRU::RemoveFile(const wxString &sPath)
{
  bool bRtn = false;
  nwxMRUBatch bat(this);
  if(bat.IsOK())
  {
    nwxMRU m(sPath);
    set<nwxMRU *, MRULessByPath> *pSet = m_setMRU.Get();
    set<nwxMRU *, MRULessByPath>::iterator itr = pSet->find(&m);
    if(itr != pSet->end())
    {
      delete *itr;
      pSet->erase(itr);
      m_vpMRU.clear();
      m_bMod = true;
      bRtn = true;
    }
  }
  return bRtn;
}
bool nwxXmlMRU::UpdateModTime(const wxString &sPath, bool bRemoveMissing)
{
  bool bRtn = false;
  nwxMRUBatch bat(this);
  if(bat.IsOK())
  {
    nwxMRU m(sPath);
    set<nwxMRU *, MRULessByPath> *pSet = m_setMRU.Get();
    set<nwxMRU *, MRULessByPath>::iterator itr = pSet->find(&m);
    if(itr != pSet->end())
    {
      bRtn = true;
      (*itr)->UpdateModTime();
      if(bRemoveMissing && !(*itr)->GetModTime())
      {
        delete *itr;
        pSet->erase(itr);
        m_vpMRU.clear();
        m_bMod = true;
      }
    }
  }
  return bRtn;
}
void nwxXmlMRU::UpdateModTimeAll(
  InwxProgressCallback *pCB, bool bRemoveMissing)
{
#ifdef _DEBUG
  wxString s;
  wxString sPath;
#endif
  nwxMRUBatch bat(this);
  vector<nwxMRU *> vMRU = *GetVector(true);
  vector<nwxMRU *>::iterator itr;
  int nDone = 0;
  for(itr = vMRU.begin(); itr != vMRU.end(); ++itr)
  {
#ifdef _DEBUG
    sPath = (*itr)->GetFullPath();
    if(!UpdateModTime(sPath,bRemoveMissing))
    {
      s.Append("\n");
      s.Append(sPath);
    }
#else
    UpdateModTime((*itr)->GetFullPath(),bRemoveMissing);
#endif
    if(pCB != NULL)
    {
      ++nDone;
      if(!pCB->UpdateProgress(nDone))
      {
        break;
      }
    }
  }
#ifdef _DEBUG
  wxASSERT_MSG(s.IsEmpty(),s);
#endif
}

nwxMRU *nwxXmlMRU::Find(const wxString &sPath, bool bCreate)
{
  nwxMRU m(sPath);
  nwxMRU *pRtn(NULL);
  set<nwxMRU *, MRULessByPath> *pSet = m_setMRU.Get();
  set<nwxMRU *, MRULessByPath>::iterator itr = pSet->find(&m);
  if(itr != pSet->end())
  {
    pRtn = *itr;
    if(pRtn->GetModTime() != m.GetModTime())
    {
      pRtn->SetModTime(m.GetModTime());
      m_bMod = true;
    }
  }
  else if(bCreate && m.GetAccessTime())
  {
    pRtn = new nwxMRU(m);
    pSet->insert(pRtn);
    m_vpMRU.clear();
    m_bMod = true;
  }
  return pRtn;
}


bool nwxXmlMRU::UpdateFileAccess(const wxString &sPath)
{
  bool bRtn = true;
  nwxMRUBatch bat(this);
  if(bat.IsOK())
  {
    nwxMRU *px(Find(sPath,true));
    if(px != NULL)
    {
      px->SetAccessTimeNow();
      bRtn = true;
    }
    m_bMod = true;
  }
  return bRtn;
}



bool nwxXmlMRU::Save()
{
  bool bRtn = false;
  if( m_bMod && (!m_nBatch) )
  {
    bRtn = nwxXmlPersist::SaveFile(GetLastFileName());
    if(bRtn)
    {
      m_bMod = false;
    }
  }
  return bRtn;
}


bool nwxXmlMRU::SetFileName(const wxString &sFileName)
{
  bool bRtn = false;
  if(sFileName == GetLastFileName())
  {
    bRtn = CheckFileModification(true);
  }
  else if( !m_nBatch && !IsLocked() )
  {
    bRtn = LoadFile(sFileName,true);
    if(!bRtn)
    {
      _Clear(false);
    }
    _RemoveExcess();
  }
  return bRtn;
}


void nwxXmlMRU::_BuildVector()
{
  set<nwxMRU *,MRULessByPath> *pSet = m_setMRU.Get();
  m_vpMRU.clear();
  m_vpMRU.reserve(pSet->size());
  for(set<nwxMRU *,MRULessByPath>::iterator itr = pSet->begin();
    itr != pSet->end();
    ++itr)
  {
    m_vpMRU.push_back(*itr);
  }
}

void nwxXmlMRU::_RemoveExcess()
{
  set<nwxMRU *,MRULessByPath> *pSet = m_setMRU.Get();
  if((m_nMaxSize > 0) && (pSet->size() > m_nMaxSize))
  {
    set<nwxMRU *, MRULessByAccess> setMRU;
    {
      // copy set into a new set sorted by access time
      CopySet<nwxMRU *,MRULessByAccess,MRULessByPath> xx(setMRU,*pSet);
    }

    //  remove the items at the beginning until the size
    //  is reduced to m_nMaxSize

    while(setMRU.size() > m_nMaxSize)
    {
      set<nwxMRU *, MRULessByAccess>::iterator itr = setMRU.begin();
      nwxMRU *pmru(*itr);
      setMRU.erase(itr);
      delete pmru;
    }
    {
      // copy the new set back
      CopySet<nwxMRU *,MRULessByPath,MRULessByAccess> yy(*pSet,setMRU);
    }
    m_vpMRU.clear();
  }
}
