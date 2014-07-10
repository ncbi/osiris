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
*  FileName: COmittedAlerts.cpp
*  Author:   Douglas Hoffman
*
*/

#include "COmittedAlerts.h"
#include "nwx/nwxString.h"

const wxString COmittedAlerts::LABEL_DIRECTORY("Directory");
const wxString COmittedAlerts::LABEL_SAMPLE("Sample");
const wxString COmittedAlerts::LABEL_CHANNEL("Channel");
const wxString COmittedAlerts::LABEL_ILS("ILS");
const wxString COmittedAlerts::LABEL_SAMPLE_LOCUS("Sample Locus");
const wxString COmittedAlerts::LABEL_DIRECTORY_LOCUS("Directory Locus");



void COmittedAlerts::Add(CAlertViewStatus::TYPE nType, int nCount)
{
  if(nCount > 0)
  {
    int nTotal = nCount;
    MAP_TYPES::iterator itr = m_mapOmitted.find(nType);
    if(itr != m_mapOmitted.end())
    {
      nTotal += itr->second;
      m_mapOmitted.erase(itr);
    }
    m_mapOmitted.insert(
      MAP_TYPES::value_type(
        nType,nTotal));
  }
}

const wxString *COmittedAlerts::TypeToString(
  CAlertViewStatus::TYPE nType)
{
  const wxString *psRtn(NULL);

  switch(nType)
  {
  case CAlertViewStatus::DIRECTORY:
    psRtn = &LABEL_DIRECTORY;
    break;
  case CAlertViewStatus::SAMPLE:
    psRtn = &LABEL_SAMPLE;
    break;
  case CAlertViewStatus::CHANNEL:
    psRtn = &LABEL_CHANNEL;
    break;
  case CAlertViewStatus::ILS:
    psRtn = &LABEL_ILS;
    break;
  case CAlertViewStatus::SAMPLE_LOCUS:
    psRtn = &LABEL_SAMPLE_LOCUS;
    break;
  case CAlertViewStatus::DIRECTORY_LOCUS:
    psRtn = &LABEL_DIRECTORY_LOCUS;
    break;
  }
  return psRtn;
}

wxString COmittedAlerts::Format()
{
  wxString sRtn;
  size_t nSize = m_mapOmitted.size();
  if(nSize > 0)
  {
    CAlertViewStatus::TYPE anTypes[] = 
    {
      CAlertViewStatus::DIRECTORY,
      CAlertViewStatus::DIRECTORY_LOCUS,
      CAlertViewStatus::SAMPLE,
      CAlertViewStatus::CHANNEL,
      CAlertViewStatus::ILS,
      CAlertViewStatus::SAMPLE_LOCUS
    };
    MAP_TYPES::iterator itr;
    size_t LEN = sizeof(anTypes) / sizeof(anTypes[0]);
    int nCount = 0;
    const wxString *psType;
    sRtn.Alloc(nSize << 4);
    sRtn = "Omitted alerts: ";
    for(size_t i = 0; i < LEN; i++)
    {
      itr = m_mapOmitted.find(anTypes[i]);
      if(itr != m_mapOmitted.end())
      {
        nCount = itr->second;
        psType = TypeToString(itr->first);
        sRtn.Append("\n  ");
        sRtn.Append(*psType);
        sRtn.Append(" (");
        sRtn.Append(nwxString::FormatNumber(itr->second));
        sRtn.Append(")");
      }
    }
  }
  return sRtn;
}
