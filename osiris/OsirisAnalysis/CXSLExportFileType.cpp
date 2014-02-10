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
*  FileName: CXSLExportFileType.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "ConfigDir.h"
#include "CXSLExportFileType.h"
#include "nwx/nwxFileUtil.h"
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/arrstr.h>
#include <wx/string.h>

#define COPY(z)  z = x.z
#define NOTEQ(z) (z != x.z)

//*********************************************** CXSLParam


const wxChar * const 
  CXSLParam::g_TYPES[CXSLParam::COUNT] =
  CXSLParamTypeStrings;


void CXSLParam::GetChoiceArray(wxArrayString *psChoices)
{
  psChoices->Empty();
  psChoices->Alloc(CXSLParam::COUNT);
  for(int i = 0; i < CXSLParam::COUNT; i++)
  {
    psChoices->Add(g_TYPES[i],1);
  }
}

CXSLParam &CXSLParam::operator = (const CXSLParam &x)
{
  COPY(m_vsChoiceList);

  COPY(m_sName);
  COPY(m_sDescription);
  COPY(m_sType);
  COPY(m_sCheckedValue);
  COPY(m_sUncheckedValue);
  COPY(m_dMin);
  COPY(m_dMax);
  COPY(m_bHasMin);
  COPY(m_bHasMax);

  COPY(m_sInFileDefaultDirectory);
  COPY(m_sInFileRequired);
  COPY(m_sInFileAllowOverride);
  COPY(m_vsInFileTypeList);

  return *this;
}
void CXSLParam::Update(const CXSLParam &x)
{
  if(m_sDescription.IsEmpty() && (m_sType == g_TYPES[0]))
  {
    (*this) = x;
  }
}

bool CXSLParam::operator == (const CXSLParam &x) const
{
  bool bRtn = false;
  if(NOTEQ(m_sName)) {}
  else if(NOTEQ(m_sDescription)) {}
  else if(NOTEQ(m_sType)) {}
  else if(IsNumericType() && (
      NOTEQ(m_bHasMin) || 
      NOTEQ(m_bHasMax) ||
      (m_bHasMin && NOTEQ(m_dMin)) ||
      (m_bHasMax && NOTEQ(m_dMax))
    ))
  {}
  else if(IsCheckboxType() &&
    ( NOTEQ(m_sCheckedValue) || NOTEQ(m_sUncheckedValue) )
    )
  {}
  else if(IsInputFileType() && (
     NOTEQ(m_sInFileDefaultDirectory) ||
     NOTEQ(GetInFileAllowOverride()) ||
     NOTEQ(GetInFileRequired()) ||
     NOTEQ(m_vsInFileTypeList)
     ))
  {}
  else if(IsChoiceType() &&
      NOTEQ(m_vsChoiceList))
  {}
  else
  {
    bRtn = true;
  }
  return bRtn;
}

CXSLParam::CXSLParamType CXSLParam::GetTypeEnum(
  const wxString &sType)
{
  int i;
  int nSIZE = (int)CXSLParam::COUNT;
  CXSLParamType nRtn = CXSLParam::COUNT;
  for(i = 0; i < nSIZE; i++)
  {
    if(sType == g_TYPES[i])
    {
      nRtn = (CXSLParamType)i;
      i = nSIZE;
    }
  }
  return nRtn;
}

bool CXSLParam::SetType(CXSLParam::CXSLParamType n)
{
  bool bRtn = (n < CXSLParam::COUNT) && ((int)n >= 0);
  if(bRtn)
  {
    m_sType = g_TYPES[n];
    bRtn = true;
  }
  return bRtn;
}

bool CXSLParam::SetType(const wxString &sType)
{
  CXSLParamType nType = GetTypeEnum(sType);
  bool bRtn = (nType != CXSLParam::COUNT);
  if(bRtn)
  {
    m_sType = sType;
  }
  return bRtn;
}

void CXSLParam::RegisterAll(bool)
{
  m_vsInFileTypeList.SetSkipIfEmpty(true);
  m_vsChoiceList.SetSkipIfEmpty(true);
  m_ioMin.SetNoSkip(&m_bHasMin);
  m_ioMax.SetNoSkip(&m_bHasMax);
  m_ioCheck.SetType(&m_sType);
  m_ioInFile.SetType(&m_sType);
  RegisterWxString(_T("name"),&m_sName);
  RegisterWxString(_T("description"),&m_sDescription);
  RegisterWxString(_T("type"),&m_sType);
  Register(_T("choicelist"),&m_vsChoiceList);
  RegisterBoolSkipFalse(_T("has-min"),&m_bHasMin);
  Register(_T("min"),&m_ioMin,(void *)&m_dMin);
  RegisterBoolSkipFalse(_T("has-max"),&m_bHasMax);
  Register(_T("max"),&m_ioMax,(void *)&m_dMax);
  Register(_T("checked-value"),&m_ioCheck,(void *) &m_sCheckedValue);
  Register(_T("unchecked-value"),&m_ioCheck,(void *) &m_sUncheckedValue);
  Register(_T("inputFileDefaultDir"),&m_ioInFile,&m_sInFileDefaultDirectory);
  Register(_T("inputFileRequired"),&m_ioInFile,&m_sInFileRequired);
  Register(_T("inputFileAllowOverride"),&m_ioInFile,&m_sInFileAllowOverride);
  Register(_T("inputFileExt"),&m_vsInFileTypeList);
}

bool CXSLParam::BuildChoiceList(
  wxArrayString *pas, const wxString &sFind) const
{
  const vector<wxString> &vs(GetChoiceList());
  pas->Empty();
  pas->Alloc(vs.size());
  vector<wxString>::const_iterator itr;
  bool bRtn = false;
  for(itr = vs.begin(); itr != vs.end(); ++itr)
  {
    if( (!bRtn) && (sFind == *itr) )
    {
      bRtn = true;
    }
    pas->Add(*itr);
  }
  return bRtn;
}

wxString CXSLParam::GetInFileTypeList() const
{
  wxString sRtn;
  const vector<wxString> &vs = m_vsInFileTypeList.GetVector();
  size_t nLen = vs.size();
  size_t i;
  for(i = 0; i < nLen; i++)
  {
    if(i)
    {
      sRtn.Append(CXSLExportFileType::LIST_SEPARATOR);
    }
    sRtn.Append(vs.at(i));
  }
  return sRtn;
}
void CXSLParam::SetInFileTypeList(const wxString &_s, wxString *psError)
{
  set<wxString> ss;
  wxString s(_s);
  nwxString::Trim(&s);
  size_t nLen = s.Len();
  int nProblem = 0;
  if(nLen)
  {
    CXSLExportFileType::ParseFileExtList(s,&ss,&nProblem);
    if(nProblem && (psError != NULL))
    {
      psError->Append(CXSLExportFileType::FILE_TYPE_ERROR);
    }

  }
  vector<wxString> &vs = m_vsInFileTypeList.GetVector();
  vs.clear();
  vs.reserve(ss.size());
  for(set<wxString>::iterator itr = ss.begin();
    itr != ss.end();
    ++itr)
  {
    vs.push_back(*itr);
  }
}

bool CXSLParam::IsFileExtOK(const wxString &_s) const
{
  bool bRtn = GetInFileAllowOverride();
  if(!bRtn)
  {
    wxString s = CXSLExportFileType::ParseFileExt(_s);
    const vector<wxString> &vs = m_vsInFileTypeList.GetVector();
    bRtn = !vs.size(); // if empty, anything goes
    for(vector<wxString>::const_iterator itr = vs.begin();
      (itr != vs.end()) && (!bRtn);
      ++itr)
    {
      if((*itr) == s)
      {
        bRtn = true;
      }
    }
  }
  return bRtn;
}
bool CXSLParam::IsSaveLastLocation() const
{
  return IsInputFileType() &&
    (m_sInFileDefaultDirectory == CXSLExportFileType::USE_LAST_LOCATION);
}

//*********************************************** CXSLAutoCreate

CXSLAutoCreate &CXSLAutoCreate::operator =(const CXSLAutoCreate &x)
{
  COPY(m_bActive);
  COPY(m_bAppendDateToFileName);
  COPY(m_sLocation);
  COPY(m_sFileName);
  COPY(m_sFileExt);
  return *this;
}

void CXSLAutoCreate::Update(const CXSLAutoCreate &x)
{
  // copy if empty
  if( (x.m_bActive) && x.IsEmpty())
  {
    (*this) = x;
  }
}

bool CXSLAutoCreate::operator ==(const CXSLAutoCreate &x) const
{
  bool bRtn = false;
  if(NOTEQ(m_bActive)) {}
  else if(!m_bActive)
  {
    // if both are inactive, then they are equal
    bRtn = true; 
  }
  else if(NOTEQ(m_bAppendDateToFileName)) {}
  else if(NOTEQ(m_sLocation)) {}
  else if(NOTEQ(m_sFileName)) {}
  else if(NOTEQ(m_sFileExt)) {}
  else
  {
    bRtn = true;
  }
  return bRtn;
}

void CXSLAutoCreate::RegisterAll(bool)
{
  RegisterBool(_T("active"),&m_bActive);
  RegisterWxString(_T("location"),&m_sLocation);
  RegisterWxString(_T("file-name"),&m_sFileName);
  RegisterBool(_T("file-name-date"),&m_bAppendDateToFileName);
  RegisterWxString(_T("file-extension"),&m_sFileExt);
}

bool CXSLAutoCreate::IsValidFileName(const wxString &s, bool bExt)
{
  const wxChar *pss = s.c_str();
  const wxChar *ps;
  const wxChar DOT('.');
  bool bRtn = !s.IsEmpty();
  for(ps = pss; bRtn && (*ps); ps++)
  {
    if(nwxString::IsAlphaNumeric(*ps)) {}
    else if(CXSLExportFileType::OK_EXT_CHARS.Find(*ps) != wxNOT_FOUND) {}
    else if((!bExt) && ((*ps) == DOT)) {}
    else
    {
      bRtn = false;
    }
  }
  return bRtn;
}

//*********************************************** CXSLExportFileType

const wxString CXSLExportFileType::g_sROOTNODE(_T("export"));
const wxChar * const CXSLExportFileType::FILE_TYPE_ERROR(
  _T("One or more file types are invalid.\n"
      "  Use letters, numbers, hyphens, and\n"
      "  underscores only.\n"));
const wxChar * const CXSLExportFileType::LIST_SEPARATOR = _T(",");


wxString CXSLExportFileType::CheckOutputFile(const wxString &sFileName)
{
  wxFileName fn(sFileName);
  wxString sRtn;
  const wxChar *ps = NULL;
  if(!fn.FileExists())
  {
    ps = _T(", was not created.");
  }
  else if(fn.GetSize() != 0)
  {}
  else if(::wxRemoveFile(sFileName))
  {
    ps = _T(", had no data and was deleted.");
  }
  else
  {
    ps = _T(", has no data but could not be deleted.");
  }
  if(ps != NULL)
  {
    sRtn = _T("Exported file, ");
    sRtn.Append(fn.GetFullName());
    sRtn.Append(ps);
  }
  return sRtn;
}

CXSLExportFileType &CXSLExportFileType::operator = (
  const CXSLExportFileType &x)
{
  COPY(m_sName);
  COPY(m_ssFileExt);
  COPY(m_bOverrideExt);
  COPY(m_sDefaultLocation);
  COPY(m_sXSLFile);
  COPY(m_sExePath);
  COPY(m_mapParam);
  COPY(m_auto);
  _SetupSheet();
  return *this;
}

void CXSLExportFileType::Update(const CXSLExportFileType &x)
{
  // similar to operator = except only copy empty properties
  if(m_sName.IsEmpty())
  {
      COPY(m_sName);
  }
  if(m_ssFileExt.empty())
  {
    COPY(m_ssFileExt);
  }
  COPY(m_bOverrideExt);
  if(m_sDefaultLocation.IsEmpty())
  {
    COPY(m_sDefaultLocation);
  }
  CXSLParam *pParam;
  m_auto.Update(x.m_auto);
  for(CXSLmapParam::const_iterator itr = x.m_mapParam.begin();
    itr != x.m_mapParam.end();
    ++itr)
  {
    pParam = m_mapParam.Find(itr->first);
    if(pParam == NULL)
    {
      m_mapParam.Insert(itr->second);
    }
    else
    {
      pParam->Update(*(itr->second));
    }
  }
}


bool CXSLExportFileType::operator == (
  const CXSLExportFileType &x) const
{
  bool bRtn = false;
  if(NOTEQ(m_sName)) {}
  else if(NOTEQ(m_ssFileExt)) {}
  else if((!m_ssFileExt.empty()) && NOTEQ(m_bOverrideExt)) {}
  else if(IsDefaultLocationAnalysisFile() 
    != x.IsDefaultLocationAnalysisFile()) {}
  else if(NOTEQ(m_sDefaultLocation)) {}
  else if(NOTEQ(m_sXSLFile)) {}
  else if(NOTEQ(m_sExePath)) {}
  else if(NOTEQ(m_mapParam)) {}
  else if(NOTEQ(m_auto)) {}
  else
  {
    bRtn = true;
  }
  return bRtn; 
}

const wxString CXSLExportFileType::GetXSLfile(bool bCheck) const
{
  wxString sRtn(m_sXSLFile);
  if(bCheck && m_sExePath.Len())
  {
    wxFileName fn(sRtn);
    if(!fn.FileExists())
    {
      wxString sExePath = mainApp::GetConfig()->GetExePath();
      if(!nwxString::FileNameStringEqual(sExePath,m_sExePath))
      {
        fn.MakeRelativeTo(m_sExePath);
        fn.MakeAbsolute(sExePath);
        if(fn.FileExists())
        {
          sRtn = fn.GetFullPath();
        }
      }
    }
  }
  return sRtn;
}

bool CXSLExportFileType::SetXSLFile(
  const wxString &s, bool bRestoreIfInvalid)
{
  bool bRtn = true;
  wxString sSave = m_sXSLFile;
  wxString sSavePath = m_sExePath;
  if(s != m_sXSLFile)
  {
    m_sXSLFile = s;
    m_sExePath = mainApp::GetConfig()->GetExePath();
  }
  else
  {
    bRestoreIfInvalid = false;
  }
  _SetupSheet();
  bRtn = m_Sheet.IsOK();
  if(bRtn)
  {}
  else if(bRestoreIfInvalid)
  {
    m_sXSLFile = sSave;
    m_sExePath = sSavePath;
    _SetupSheet();
  }
  return bRtn;
}

bool CXSLExportFileType::UpdateExePath()
{
  bool bRtn = false;
  if(m_Sheet.IsOK() && m_sExePath.IsEmpty())
  {
    m_sExePath = mainApp::GetConfig()->GetExePath();
    bRtn = true;
  }
  return bRtn;
}

bool CXSLExportFileType::AutoTransform(const wxString &sInputFileName)
{
  wxString sAutoError;
  bool bRtn = false;
  if(!HasAutoTransform())
  {
    sAutoError = _T("Automatic export is not specified");
  }
  else if(!XSLfileOK())
  {
    sAutoError = _T("Cannot export ");
    sAutoError.Append(m_sName);
    sAutoError.Append(_T(
      ".  Stylesheet is either invalid or not found"));
  }
  else
  {
    wxFileName fn(sInputFileName);
    wxString sLocation;
    wxXml2Document doc;
    if(!fn.IsFileReadable()) 
    {
       // we are done
      sAutoError = _T("Cannot read input file, ");
      sAutoError.Append(sInputFileName);
    }
    else if(!doc.Load(sInputFileName))
    {
      sAutoError = _T("Cannot export file.  Input file, ");
      sAutoError.Append(sInputFileName);
      sAutoError.Append(_T(", is not valid"));
    }
    else if(m_auto.IsAnalysisLocation())
    {
      sLocation = fn.GetPath();
    }
    else
    {
      sLocation = m_auto.GetLocation();
    }
    if(sLocation.IsEmpty()) 
    {
      sAutoError = _T("Location for automatic export is not specified.");
    }
    else if(!wxFileName::IsDirWritable(sLocation))
    {
      sAutoError = _T("Cannot export file to location, ");
      sAutoError.Append(sLocation);
      sAutoError.Append(_T(", permission denied"));
    }
    else
    {
      // figure out file extension
      const wxString &sExtTmp(m_auto.GetFileExt());
      wxString sExt;
      if(!sExtTmp.IsEmpty())
      {
        sExt = _T(".");
        sExt.Append(sExtTmp);
      }

      nwxFileUtil::EndWithSeparator(&sLocation);
      if(m_auto.IsAnalysisFileName())
      {
        sLocation.Append(fn.GetName());
      }
      else
      {
        sLocation.Append(m_auto.GetFileName());
      }
      if(m_auto.GetAppendDateToFileName())
      {
        wxDateTime dt;
        dt.SetToCurrent();
        sLocation.Append(dt.Format(_T("-%Y%m%d-%H%M%S")));
      }
      bool bDone = false;
      wxString sOutFileName;
      int i = 0;
      while(!bDone)
      {
        sOutFileName = sLocation;
        if(i)
        {
          sOutFileName.Append(wxString::Format(_T("_%d"),i));
        }
        sOutFileName.Append(sExt);
        bDone = !wxFileName::FileExists(sOutFileName);
        i++;
      }
      bRtn = TransformToFile(&doc,sOutFileName);
      if(!bRtn)
      {
        sAutoError = 
          _T("An error occurred when attempting to export ");
        sAutoError.Append(m_sName);
      }
      else
      {
        sAutoError = CheckOutputFile(sOutFileName);
        bRtn = sAutoError.IsEmpty();
      }
    }
  }
  if(!sAutoError.IsEmpty())
  {
    wxXml2Object::SendError(sAutoError);
  }
  return bRtn;
}

wxString CXSLExportFileType::ParseFileExt(const wxString &s)
{
  wxString sRtn = s;
  size_t nLen;
  bool bRtn = false;
  while(sRtn.StartsWith(_T(".")))
  {
    sRtn = sRtn.Mid(1);
  }
  sRtn.MakeLower();
  nLen = sRtn.Len();
  if(nLen > 0 && nLen < 12)
  {
    const wxChar *pss = sRtn.c_str();
    const wxChar *ps;
    bRtn = true;
    for(ps = pss; *ps && bRtn; ps++)
    {
      if(nwxString::IsAlphaNumeric(*ps)) {}
      else if (OK_EXT_CHARS.Find(*ps) != wxNOT_FOUND) {}
      else
      {
        bRtn = false;
      }
    }
  }
  if(!bRtn)
  {
    sRtn.Empty();
  }
  return sRtn;
}

size_t CXSLExportFileType::ParseFileExtList(
  const wxString &_s,set<wxString> *pss, int *pnProblem)
{
  // KILL This
  wxString s(_s);
  wxString sTmp;
  set<wxString> ss;
  const wxChar *psKill(_T(",.:;\\/|"));
  const wxChar *p;
  wxChar schar[2] = {0,0};
  if(pnProblem != NULL)
  {
    *pnProblem = 0;
  }
  for(p = psKill; *p; p++)
  {
    schar[0] = *p;
    s.Replace(schar,_T(" "),true);
  }
  nwxString::Trim(&s);
  while(s.Replace(_T("  "),_T(" "))) {}
  nwxString::Split(s.c_str(),&ss,_T(" "));
  pss->clear();
  for(set<wxString>::iterator itr = ss.begin();
    itr != ss.end();
    ++itr)
  {
    sTmp = ParseFileExt(*itr);
    if(!sTmp.IsEmpty())
    {
      pss->insert(sTmp);
    }
    else if(pnProblem != NULL)
    {
      (*pnProblem)++;
    }
  }
  return pss->size();
}
const wxChar * const CXSLExportFileType::ERROR_MESSAGE = _T("File export was unsuccessful.");
const wxString CXSLExportFileType::OK_EXT_CHARS(_T("-_~!@#$%^+="));
const wxString CXSLExportFileType::USE_DEFAULT(_T("*D"));
const wxString CXSLExportFileType::USE_ANALYSIS_FILE(_T("*A"));
const wxString CXSLExportFileType::USE_ANALYSIS_PARENT(_T("*P"));
const wxString CXSLExportFileType::USE_LAST_LOCATION(_T("*L"));
const wxString CXSLExportFileType::USE_OUTPUT_LOCATION(_T("*O"));

const wxChar * const CXSLExportFileType::INFILE = _T("inputFile");
const wxChar * const CXSLExportFileType::OUTFILE = _T("outputFile");


void CXSLExportFileType::RegisterAll(bool)
{
  RegisterWxString(_T("name"),&m_sName);
  Register(_T("file-extension"),&m_IOext,&m_ssFileExt);
  RegisterBool(_T("extension-override"),&m_bOverrideExt);
  RegisterWxString(_T("default-location"),&m_sDefaultLocation);
  RegisterWxString(_T("xsl-file"),&m_sXSLFile);
  RegisterWxStringNotEmpty(_T("exe-path"),&m_sExePath);
  Register(_T("xsl-params"),&m_mapParam);
  Register(_T("auto-create"),&m_auto);
}

void CXSLExportFileType::_SetupSheet()
{
  m_Sheet.Load(GetXSLfile(true));
  if(m_Sheet.IsOK())
  {
    const wxXslParams &parms(m_Sheet.GetSheetParms());
    const map<wxString,wxString> &mapParm = parms.GetParmsMap();
    map<wxString,wxString>::const_iterator itr;
    CXSLmapParam::iterator itrm;
    set<CXSLParam *> setKeep;
    set<CXSLParam *> setKill;

    // do not allow inputFile nor outputFile in the param list
    // because they are reserved

    size_t nOmit = 0;
    for(itr = mapParm.begin(); itr != mapParm.end(); ++itr)
    {
      if( (itr->first == INFILE)  || (itr->first == OUTFILE) )
      {
        nOmit++;
      }
      else if(!m_mapParam.HasKey(itr->first))
      {
        m_mapParam.Insert(new CXSLParam(itr->first));
      }
    }
    if(m_mapParam->size() != (mapParm.size() - nOmit))
    {
      // remove unused parameters
      for(itrm = m_mapParam.begin(); 
          itrm != m_mapParam.end();
          ++itrm)
      {
        if(
          (itrm->first == INFILE) ||
          (itrm->first == OUTFILE) ||
          (mapParm.find(itrm->first) == mapParm.end())
          )
        {
          setKill.insert(itrm->second);
        }
        else
        {
          setKeep.insert(itrm->second);
        }
      }
      if(setKill.size() > 0)
      {
        setptr<CXSLParam>::cleanup(&setKill);
        m_mapParam.Get()->clear();
        for(set<CXSLParam *>::iterator itrs = setKeep.begin();
          itrs != setKeep.end();
          ++itrs)
        {
          m_mapParam.Insert(*itrs);
        }
      }
    }
    wxASSERT_MSG(m_mapParam->size() == (mapParm.size() - nOmit),
      _T("Parameter conflict in CXSLExportFileType::_SetupSheet()"));
  }
  else
  {
//    m_Sheet.ClearAll();
  }
}
void CXSLExportFileType::_SetupSheetParams(
  const map<wxString,wxString> *pMapParam)
{
  m_Sheet.ClearParms();
  if(pMapParam != NULL)
  {
    map<wxString,wxString>::const_iterator itr;
    for(itr = pMapParam->begin();
      itr != pMapParam->end();
      ++itr)
    {
      m_Sheet.SetParm(itr->first,itr->second);
    }
  }
}

void CXSLExportFileType::_SetupSheetParams(
  const map<wxString,wxString> *pMapParam,
  const wxString &sInputFileName,
  const wxString &sOutputFileName)
{
  _SetupSheetParams(pMapParam);
  m_Sheet.SetParm(INFILE,sInputFileName);
  m_Sheet.SetParm(OUTFILE,sOutputFileName);
}

int CXSLExportFileType::GetUserParamCount() const
{
  CXSLmapParam::const_iterator itr;
  const CXSLParam *pParam;
  int nRtn = 0;
  for(itr = m_mapParam.begin();
    itr != m_mapParam.end();
    ++itr)
  {
    pParam = itr->second;
    if(pParam->GetType() != CXSL_PARAM_IGNORE)
    {
      nRtn++;
    }
  }
  return nRtn;
}

bool CXSLExportFileType::TransformToFile(
  const wxString &sInFile, const wxString &sOutFile, 
  const std::map<wxString,wxString> *pMapParams)
{
  wxXml2Document doc(sInFile);
  bool bRtn = false;
  if(doc.IsOk())
  {
    bRtn = TransformToFile(&doc,pMapParams,sOutFile,sInFile);
  }
  return bRtn;
}


//*********************************************** CXSLExportFiles

wxString CExportFiles::g_sFileName;
const wxString CExportFiles::ROOTNODE(_T("exports"));

CExportFiles *CExportFiles::g_pGlobal(NULL);
CExportFiles::CExportFilesGlobal CExportFiles::g_xxx;





void CExportFiles::_SetupFileName()
{
  ConfigDir *pDir = mainApp::GetConfig();
  g_sFileName = pDir->GetSitePath();
  nwxFileUtil::EndWithSeparator(&g_sFileName);
  g_sFileName.Append(_T("exports.xml"));
}

bool CExportFiles::FileExists()
{
  if(g_sFileName.IsEmpty())
  {
    _SetupFileName();
  }
  bool b = wxFileName::FileExists(g_sFileName);
  return b;
}
bool CExportFiles::SaveExportFile()
{
  bool bRtn = true;
  if(!FileExists())
  {
    wxFileName fn(g_sFileName);
    wxString sDir = fn.GetPath();
    if(!nwxFileUtil::MkDir(sDir))
    {
      bRtn = false;
    }
  }
  if(bRtn)
  {
    UpdateExePath();
    bRtn = SaveFile(g_sFileName);
  }
  return bRtn;
}

bool CExportFiles::CreateFileIfNotExists()
{
  // return true if the file exists upon exit
  bool bRtn = FileExists();
  if(!bRtn)
  {
    bRtn = SaveExportFile();
  }
  return bRtn;
}
bool CExportFiles::CheckXSLmodification()
{
  bool bRtn = false;
  for(iterator itr = begin();
    itr != end();
    ++itr)
  {
    CXSLExportFileType *pxsl = itr->second;
    if(pxsl->CheckXSLmodification())
    {
      bRtn = true;
    }
  }
  return bRtn;
}
bool CExportFiles::LockExportFile(int nWait)
{
  bool bRtn = false;
  if(CreateFileIfNotExists())
  {
    bRtn = Lock(nWait);
  }
  return bRtn;
}

bool CExportFiles::LoadExportFile()
{
  bool bRtn = false;
  bool bLocked = IsLocked() || (m_nBatch > 0);
  if(bLocked || (m_nBatch > 0))
  {
    // don't check for reload nor save if locked
    bRtn = true;
  }
  else if(!FileExists())
  {
    // SaveFile(g_sFileName); -- 
    // don't need to save an empty file
    Init();
    bRtn = true;
  }
  else if(GetLastFileName() == g_sFileName)
  {
    bRtn = CheckFileModification(false)
      ? ReloadFile()
      : true;
  }
  else
  {
    bRtn = LoadFile(g_sFileName);
  }
  return bRtn;
}
size_t CExportFiles::GetAutoList(vector<CXSLExportFileType *> *pList)
{
  pList->clear();
  size_t nSize = Get()->size();
  if(nSize)
  {
    pList->reserve(nSize);
    iterator itr;
    CXSLExportFileType *pXSL;
    for(itr = Get()->begin();
      itr != Get()->end();
      ++itr)
    {
      pXSL = itr->second;
      if(pXSL->GetAutoCreate().IsActive())
      {
        pList->push_back(pXSL);
      }
    }
  }
  return pList->size();
}
size_t CExportFiles::GetItems(wxArrayString *pvs)
{
  wxString sLogMessage;
  vector<wxString> vsKillList;
  pvs->Empty();
  iterator itr;
  if(Get()->size())
  {
    pvs->Alloc(Get()->size());
    for(itr = Get()->begin();
      itr != Get()->end();
      ++itr)
    {
      pvs->Add(itr->first);
      if(!itr->second->XSLfileOK())
      {
        sLogMessage = _T("Invalid XSL file for ");
        sLogMessage.Append(itr->first);
        sLogMessage.Append(_T("\n  file: "));
        sLogMessage.Append(itr->second->GetXSLfile(false));
        const wxString &sExe = itr->second->GetExePath();
        if(sExe.Len())
        {
          sLogMessage.Append(_T(";  exepath: "));
          sLogMessage.Append(sExe);
        }
        mainApp::LogMessage(sLogMessage);
      }
    }
  }
  return pvs->GetCount();
}

#ifdef __WXDEBUG__
bool CExportFiles::UnitTest()
{
#if 1
  wxString sError;
  CExportFiles x;
  ConfigDir *pDir = mainApp::GetConfig();
  wxString sFile = pDir->GetSitePath();
  nwxFileUtil::EndWithSeparator(&sFile);
  wxString sFile2 = sFile;
  sFile.Append(_T("exportText.xml"));
  sFile2.Append(_T("exportTextB.xml"));
  CXSLExportFileType *pExport = new CXSLExportFileType;
  pExport->SetName("Allele Spreadsheet");
  pExport->AddFileExt(_T(".tab"));
  pExport->SetDefaultLocationAnalysisFile();
  pExport->SetXSLFile(_T("tab.xsl"));
  x.Insert(pExport);
  if(!pExport->XSLfileOK())
  {
    pExport->SetXSLFile(_T("..\\tab.xsl"));
  }

  if(!pExport->XSLfileOK())
  {
    sError.Append(_T("Problem loading XSL file\n"));
  }
  else
  {
    CExportFiles xx;
    vector<wxString> vsYN;
    vsYN.reserve(2);
    vsYN.push_back(_T("yes"));
    vsYN.push_back(_T("no"));
    CXSLParam *pParam = pExport->FindParam(_T("OL"));
    if(pParam == NULL)
    {
      sError.Append(_T("Cannot find param: OL\n"));
    }
    else
    {
      pParam->SetDescription(_T("Label off ladder alleles"));
      pParam->SetType(CXSLParam::CHOICE);
      pParam->SetChoiceList(vsYN);
    }
    pParam = pExport->FindParam(_T("UseChannelNr"));
    if(pParam == NULL)
    {
      sError.Append(_T("Cannot find param: UseChannelNr"));
    }
    else
    {
      pParam->SetDescription(_T("Show Channel Numbers"));
      pParam->SetType(CXSLParam::INTEGER);
      pParam->SetMin(0);
      pParam->SetMax(1);
    }
    CXSLAutoCreate *pAuto = pExport->GetAutoCreatePtr();
    pAuto->SetActive(true);
    pAuto->SetDefaultLocation();
    pAuto->SetFileName(_T("spreadsheet"));
    pAuto->SetFileExt(_T(".tab"));
    if(!x.SaveFile(sFile))
    {
      sError.Append(_T("Problem saving file\n"));
    }
    else if(!xx.LoadFile(sFile))
    {
      sError.Append(_T("Problem loading file\n"));
    }
    else 
    {
      if(xx != x)
      {
        sError.Append(_T("Comparison failed\n"));
      }
      if(!xx.SaveFile(sFile2))
      {
        sError.Append(_T("Problem saving second file\n"));
      }
    }
  }
  GetGlobal();
  wxASSERT_MSG(sError.IsEmpty(),sError);
  return sError.IsEmpty();
#else
  return true;
#endif
}
#endif
