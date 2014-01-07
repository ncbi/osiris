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
*  FileName: COARnotes.cpp
*  Author:   Douglas Hoffman
*
*/

#include "COARnotes.h"
#include "COARreview.h"
#include "nwx/nwxString.h"
const wxString COARnotes::g_sNode(_T("Notes"));
const wxString COARnotes::g_EmptyString;

void COARnotes::RegisterAll(bool)
{
  RegisterWxString(_T("Text"),&m_sText);
  RegisterWxDateTime(_T("Update"),&m_Update);
}

bool COARnotes::Update(
  const COARnotes &x,
  vector<COARnotes *> *pvpOldNotes,
  const wxDateTime &t)
{
  bool bRtn = (x != *this);
  if(bRtn)
  {
    if(!Skip(NULL))
    {
      COARnotes *pNew = new COARnotes(*this);
      pvpOldNotes->push_back(pNew);
    }
    (*this) = x;
    m_Update = t;
  }
  return bRtn;
}

void COARnotes::Append(
  const wxString &sText,
  const wxString &sUser,
  vector<COARnotes *> *pvpOldNotes)
{
  wxString sNewText =
    AppendNotes(m_sText,sText,sUser);
  if(!Skip(NULL))
  {
    COARnotes *pNew = new COARnotes(*this);
    pvpOldNotes->push_back(pNew);
  }
  m_sText = sNewText;
  m_Update.SetToCurrent();
}

wxString COARnotes::AppendNotes(
  const wxString &sNotes,
  const wxString &_sAppend,
  const wxString &_sUser)
{
  wxString sRtn;
  wxString sAppend(_sAppend);
  wxString sUser(_sUser);
  wxString sTime;
  wxDateTime dt;
  nwxString::Trim(&sAppend);
  nwxString::Trim(&sUser);
  if(sAppend.IsEmpty())
  {
    sAppend = _T("<no new notes added>");
  }
  sTime.Alloc(sUser.Len() + 64);
  sTime = _T("Edited ");
  dt.SetToCurrent();
  sTime.Append(dt.Format(COARreview::TIME_FORMAT));
  if(!sUser.IsEmpty())
  {
    sTime.Append(_T(" by "));
    sTime.Append(sUser);
  }
  sTime.Append(_T("\n"));
  sRtn.Alloc(sNotes.Len() + sAppend.Len() + sTime.Len() + 4);
  if(!sNotes.IsEmpty())
  {
    sRtn = sNotes;
    sRtn.Append(_T("\n\n"));
  }
  sRtn.Append(sTime);
  sRtn.Append(sAppend);
  return sRtn;
}



bool COARnotes::_AreNotesEdited(
  const COARnotes *pNotesCurrent,
  const vector<COARnotes *> *pvNotesOld,
  const wxDateTime *pTime)
{
  const COARnotes *pNotes;
  bool bRtn = false;
  if(pTime == NULL)
  {
    // we have some notes
    bRtn = (pvNotesOld->size() > 0) || 
      pNotesCurrent->NotEmpty();
  }
  else if( (pNotesCurrent->GetTime() < *pTime) && 
    pNotesCurrent->NotEmpty() )
  {
    // current notes are older than *pTime
    // and not empty
    bRtn = true;
  }
  else
  {
    // look for any notes that are older than *pTime and not empty
    for(vector<COARnotes *>::const_iterator itr = pvNotesOld->begin();
      itr != pvNotesOld->end();
      ++itr)
    {
      pNotes = *itr;
      if( (pNotes->GetTime() < *pTime) && 
          pNotes->NotEmpty() )
      {
        bRtn = true;
        break;
      }
    }
  }
  return bRtn;
}


const COARnotes *COARnotes::_FindNotes(
  const COARnotes *pNotesCurrent, 
  const vector<COARnotes *> *pvNotesOld, 
  const wxDateTime *pTime)
{
  const COARnotes *pRtn(NULL);
  if((pTime != NULL) && (pNotesCurrent->GetTime() > *pTime))
  {
    for(vector<COARnotes *>::const_iterator itr = pvNotesOld->begin();
      itr != pvNotesOld->end();
      ++itr)
    {
      const wxDateTime &dt((*itr)->GetTime());
      if(dt > *pTime) {;} // skip this one
      else if( (pRtn == NULL) || (dt > pRtn->GetTime()) )
      {
        pRtn = *itr;
      }
    }
  }
  else
  {
    pRtn = pNotesCurrent;
  }
  return pRtn;
};
