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
#include "nwx/nwxFileUtil.h"
#ifdef __WXMSW__
#define strdup _strdup
#endif

CProcessAnalysis::CProcessAnalysis(
  CDirEntry *pDirEntry, 
  const CVolume *pVolume,
  wxEvtHandler *parent, int nID) :
    CProcess(parent,nID),
    m_psExe(NULL),
    m_pDirEntry(pDirEntry)
{
#define END_LINE sStdin.Append(";\n")

#define APPEND_LINE(name,value) \
  sStdin.Append(name "="); \
  sStdin.Append(value); \
  END_LINE

#define APPEND_INT(name,n) \
  s.Printf("%d",n); \
  APPEND_LINE(name,s);


  wxString sStdin;
  wxString s;
  const CParmOsiris *pParm = pDirEntry->GetParmOsiris();
  const ConfigDir *pDir = mainApp::GetConfig();
  
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
  if(pParm->GetTimeStampSubDir())
  {
    const wxString &s = pDirEntry->GetTimeStamp();
    if(!s.IsEmpty())
    {
      APPEND_LINE("OutputSubdirectory",s);
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
  m_psExe = strdup(s.utf8_str());

  char *argv[] = { m_psExe, NULL  };
  m_dProgress = 0.0;
  Run(argv);
  wxOutputStream *pOut = GetOutputStream();
  wxASSERT_MSG(pOut != NULL,"Cannot get output stream for process");
  mainApp::LogMessage(sStdin);
  if(pOut != NULL)
  {
    const char *pChar = sStdin.ToUTF8();
    size_t nLen = sStdin.Len();
    size_t n;
    while(nLen > 0)
    {
      n = pOut->Write((void *)pChar,nLen).LastWrite();
      if(n)
      {
        pChar += n;
        nLen -= n;
      }
      else
      {
        nLen = 0;
        Cancel();
      }
    }
  }
}

CProcessAnalysis::~CProcessAnalysis() 
{
  if(m_psExe != NULL)
  {
    free((void *)m_psExe);
    m_psExe = NULL;
  }
}

void CProcessAnalysis::ProcessLine(
  const char *p,size_t nLen, bool)
{
  static const char PROGRESS[] = "Progress = ";
  static const size_t nLEN = sizeof(PROGRESS) / sizeof(PROGRESS[0]) - 1;
  if(!strncmp(PROGRESS,p,nLEN))
  {
    double dProgress = atof(p + nLEN);
    if(dProgress > 0.0)
    {
      m_dProgress = dProgress;
    }
  }
  else
  {
    wxString s(p,nLen);
    m_pDirEntry->AppendRunOutput(s);
  }
}

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

