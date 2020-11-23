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

*  FileName: CProcessAnalysis.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wx/filename.h>
#include "CProcessAnalysis.h"
#include "CDirList.h"
#include "CParmOsiris.h"
#include "CVolumes.h"
#include "CKitList.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxWCharBuffer.h"

CProcessAnalysis::CProcessAnalysis(
  CDirEntry *pDirEntry, 
  const CVolume *pVolume,
  wxEvtHandler *parent, int nID) :
    nwxProcess(parent,nID),
    m_pDirEntry(pDirEntry),
    m_bInError(false)
{
  pDirEntry->AddBlankErrorIfNeeded();

#define END_LINE sStdin.Append(";\n")

#define APPEND_LINE(name,value) \
  sStdin.Append(name);          \
  sStdin.Append(wxS("="));      \
  sStdin.Append(value);         \
  END_LINE

#define FORMAT_INT(n) \
  s.Printf("%d",n)

#define APPEND_INT(name,n) \
  FORMAT_INT(n);           \
  APPEND_LINE(name,s)


  wxString sStdin;
  wxString s;
  const CParmOsiris *pParm = pDirEntry->GetParmOsiris();
  const ConfigDir *pDir = mainApp::GetConfig();
  CPersistKitList *pKitList = mainApp::GetKitList();


  sStdin.Alloc(4096);
  APPEND_LINE("InputDirectory",pParm->GetInputDirectory());      //  1
  APPEND_LINE("LadderDirectory",pDir->GetExeConfigPath());       //  2
  APPEND_LINE("ReportDirectory",pParm->GetOutputDirectory());    //  3
  APPEND_LINE("MarkerSetName",pParm->GetKitName());              //  4
  APPEND_LINE("LaneStandardName",pParm->GetLsName());            //  5
  APPEND_LINE("CriticalOutputLevel","15");                   //  6
  APPEND_INT("MinSampleRFU",pParm->GetMinRFU_Sample());          //  7
  APPEND_INT("MinLaneStandardRFU",pParm->GetMinRFU_ILS());       //  8
  APPEND_INT("MinLadderRFU",pParm->GetMinRFU_Ladder());          //  9
  APPEND_INT("MinInterlocusRFU",pParm->GetMinRFU_Interlocus());  // 10
  if (pKitList->IsLadderFree(pParm->GetKitName()))
  {
    // IF this is no longer needed in the future, 
    // the function
    //     bool COARfile::IsLadderFree()
    // will need to be rewritten
    APPEND_LINE("LadderFree", "true");
  }

  // channel data

  typedef struct 
  {
    const vector<int> *pan;
    const wxChar *ps;
  } CHANNEL_OVERRIDE;
  CHANNEL_OVERRIDE chanInfo[] =
  {
    { &pParm->GetChannelRFU(), wxS("AnalysisThresholdOverride") },
    { &pParm->GetChannelDetection(), wxS("DetectionThresholdOverride") }
  };
  const size_t nOVR = sizeof(chanInfo) / sizeof(chanInfo[0]);
  wxString sName;
  for (size_t ndx = 0; ndx < nOVR; ndx++)
  {
    const vector<int> *pan = chanInfo[ndx].pan;
    const wxChar *ps = chanInfo[ndx].ps;
    vector<int>::const_iterator itr;
    int nChannel;
    for(itr = pan->begin(), nChannel = 1;
        itr != pan->end();
        ++itr, ++nChannel)
    {
      if((*itr) > 0)
      {
        FORMAT_INT(nChannel);
        sName = ps;
        sName.Append(wxS(":"));
        sName.Append(s);
        APPEND_INT(sName,*itr);
      }
    }
  }

  // end channel data

  if(pParm->GetTimeStampSubDir())
  {
    const wxString &s1 = pDirEntry->GetTimeStamp();
    if(!s1.IsEmpty())
    {
      APPEND_LINE("OutputSubdirectory",s1);
    }
    else
    {
      wxASSERT_MSG(false,"Time stamp is empty");
    }
  }

  // the following may eventually be moved to CParmOsiris and
  //  CLabSettings::GetMinRFU() if it is added to the user interface

  int nMinLadderInterlocus = pParm->GetMinLadderInterlocusRFU();
//    pVolume->GetLabSettings()->GetThresholds()->GetRFUladder()->GetMinRFUinterlocus();
  int nMinDetection = pParm->GetSampleDetectionThreshold();
//    pVolume->GetLabSettings()->GetThresholds()->GetRFUsample()->GetMinDetection();
  if(nMinLadderInterlocus >= 0)
  {
    APPEND_INT("MinLadderInterlocusRFU",nMinLadderInterlocus);   // 11
  }
  if(nMinDetection >= 0)
  {
    APPEND_INT("SampleDetectionThreshold",nMinDetection);
  }
  s = pParm->GetDataAnalyzed() ? "A" : "R";
  APPEND_LINE("RawDataString",s);  // 11 or 12
  APPEND_LINE("LabSettings",pVolume->GetLabSettingsFileName());
  APPEND_LINE("StandardSettings",pVolume->GetStdSettingsFileName());
  APPEND_LINE("MessageBook",pDir->GetMessageBookFileName());
  const wxString &sOverride(pParm->GetAnalysisOverride());
  if(!sOverride.IsEmpty())
  {
    APPEND_LINE("override",sOverride);
  }

  END_LINE;

#undef APPEND_INT
#undef APPEND_LINE
#undef END_LINE

  //  finished building stdin for analysis program

  wxASSERT_MSG(sizeof(char) == 1,
      "sizeof(char) != 1, "
        "therefore this code needs to be changed");
  
  s = pDir->GetExePath();
  nwxFileUtil::EndWithSeparator(&s);
#ifdef __WXMSW__
  s += "TestAnalysisDirectoryLC.exe";
#else
  s += "TestAnalysisDirectoryLC";
#endif
  nwxWCharBuffer exe(s);
  wchar_t *argv[] = { exe.Get(), NULL  };
  m_dProgress = 0.0;
  Run(argv);
  mainApp::LogMessage(sStdin);
  WriteToProcess(sStdin);
}

CProcessAnalysis::~CProcessAnalysis() 
{}

static const char PING[] = "<Ping>";
static const char PING_END[] = "</Ping>";
static const char ERROR_TAG[] = "<*****>";
static const char ERROR_TAG_END[] = "</*****>";

#define STR_ARR_LEN(X) ((sizeof(X)/sizeof(X[0])) - 1)

void CProcessAnalysis::_CheckRunningError()
{
  int n = m_sRunningError.Find(ERROR_TAG_END);
  if (n >= 0)
  {
    wxString s = m_sRunningError.Mid(0, n);
    m_pDirEntry->AddError(s);
    mainApp::Ping2(PING_EVENT, "AnalysisError", "msg", s);
    s += m_sRunningError.Mid(n + STR_ARR_LEN(ERROR_TAG_END));
    m_pDirEntry->AppendRunOutput(s);
    m_bInError = false;
    m_sRunningError.Empty();
  }
}

void CProcessAnalysis::ProcessLine(
  const char *p,size_t nLen, bool)
{
#define STARTS_WITH(X) (!strncmp(p, X, STR_ARR_LEN(X)))

  static const char PROGRESS[] = "Progress = ";
  
  static const int MAX_BUFFER_SIZE = 512;
  char BUFFER[MAX_BUFFER_SIZE];
  bool bWithinBuffer = (strlen(p) < MAX_BUFFER_SIZE);
  if (m_bInError)
  {
    m_sRunningError.Append(p);
    _CheckRunningError();
  }
  else if(STARTS_WITH(PROGRESS))
  {
    double dProgress = atof(p + STR_ARR_LEN(PROGRESS));
    if(dProgress > 0.0)
    {
      m_dProgress = dProgress;
    }
  }
  else if( bWithinBuffer &&
           STARTS_WITH(PING) &&
           (strstr(p, PING_END) != NULL)
         )
  {
    strcpy(BUFFER, p + STR_ARR_LEN(PING));
    *strstr(BUFFER, PING_END) = 0;
    m_pDirEntry->AddPing(atoi(BUFFER));
    mainApp::Ping2(PING_EVENT, "AnalysisAlert", "Alert", BUFFER);
  }
  else if (STARTS_WITH(ERROR_TAG))
  {
    m_sRunningError = wxString::FromUTF8(p + STR_ARR_LEN(ERROR_TAG));
    m_bInError = true;
    _CheckRunningError();
  }
  else
  {
    m_pDirEntry->AppendRunOutput(wxString::FromUTF8(p,nLen));
  }
#undef STARTS_WITH
}
#undef STR_ARR_LEN

bool CProcessAnalysis::IsOutputModified()
{
  return m_pDirEntry->IsOutputModified();
}
wxString CProcessAnalysis::GetOutput()
{
  wxString sRtn = m_pDirEntry->GetRunOutput();
  sRtn.Append("\n");
  return sRtn;
}

