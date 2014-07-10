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

*  FileName: CDirList.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CDirList.h"
#include "OsirisFileTypes.h"
#include <wx/filename.h>
#include <wx/file.h>
#include "nwx/vectorptr.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxString.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxKillDir.h"
#include <time.h>


const wxString CDirEntry::g_sSTATUS_PENDING("pending");
const wxString CDirEntry::g_sSTATUS_DONE("completed");
const wxString CDirEntry::g_sSTATUS_RUNNING("running");
const wxString CDirEntry::g_sSTATUS_ERROR("failed");
const wxString CDirEntry::g_sSTATUS_EXPORT_ERROR("no export file");
const wxString CDirEntry::g_sSTATUS_CANCELED("canceled");

const wxColour CDirEntry::g_colPENDING(128,128,128);
const wxColour CDirEntry::g_colDONE(0,0,0);
const wxColour CDirEntry::g_colERROR(224,0,0);

const wxChar * const CDirList::DEFAULT_EXT(wxS("fsa"));

wxString CDirEntry::FormatStartTime() const
{
  return nwxString::FormatDateTime(m_dtStart,wxEmptyString);
}
wxString CDirEntry::FormatStopTime() const
{
  return nwxString::FormatDateTime(m_dtStop,wxEmptyString);
}

void CDirEntry::UpdateStatus()
{
  switch(m_nStatus)
  {
    case DIRENTRY_DONE:
      m_psStatus = &g_sSTATUS_DONE;
      m_pStatusColour = &g_colDONE;
      break;
    case DIRENTRY_RUNNING:
      m_psStatus = &g_sSTATUS_RUNNING;
      m_pStatusColour = &g_colPENDING;
      break;
    case DIRENTRY_ERROR:
      m_psStatus = &g_sSTATUS_ERROR;
      m_pStatusColour = &g_colERROR;
      break;
    case DIRENTRY_EXPORT_ERROR:
      m_psStatus = &g_sSTATUS_EXPORT_ERROR;
      m_pStatusColour = &g_colDONE;
      break;
    case DIRENTRY_CANCELED:
      m_psStatus = &g_sSTATUS_CANCELED;
      m_pStatusColour = &g_colERROR;
      break;
    case DIRENTRY_PENDING:
    default:
      m_psStatus = &g_sSTATUS_PENDING;
      m_pStatusColour = &g_colPENDING;
      m_sRunOutput.Empty();
      break;
  }
}

void CDirEntry::CleanupFiles()
{
  wxFileName fn(m_sOutputFile);
  wxString sDir(fn.GetPath());
  size_t nLenDir = sDir.Len();
  size_t nLenParent = m_sOutputDir.Len();
  if( (nLenDir >= nLenParent) && 
      (nLenParent > 4) &&
      nwxString::FileNameStringEqual(
          sDir.Mid(0,nLenParent),m_sOutputDir)
    )
  {    
    nwxKillDir::KillAfterStatic(sDir,false,m_dtStart);
  }
#ifdef __WXDEBUG__
  else
  {
    wxASSERT_MSG(0,"Problem matching directory names");
  }
#endif
}

const wxString &CDirEntry::GetOutputFile()
{
  if(m_sOutputFile.IsEmpty())
  {
    wxString sFullPath;
    wxString sNameBase;
    sNameBase.Alloc(256);
    sFullPath.Alloc(256);
    sFullPath = GetInputDir();
    nwxFileUtil::NoEndWithSeparator(&sFullPath);
    wxFileName fn(sFullPath);
    sNameBase = fn.GetFullName();
    sFullPath = m_sOutputDir;
    nwxFileUtil::EndWithSeparator(&sFullPath);
    sFullPath.Append(sNameBase);
    nwxFileUtil::EndWithSeparator(&sFullPath);
    if(m_parmOsiris.GetTimeStampSubDir())
    {
      if(m_sTimeStamp.IsEmpty())
      {
        wxASSERT_MSG(false,
          "Empty time stamp in CDirEntry::GetOutputFile()");
      }
      else
      {
        sFullPath.Append(m_sTimeStamp);
        nwxFileUtil::EndWithSeparator(&sFullPath);
      }
    }
    sFullPath.Append(sNameBase);
    sFullPath.Append(".oar");
    m_sOutputFile = sFullPath;
  }
  return m_sOutputFile;
}

void CDirEntry::RegisterAll(bool)
{
  wxASSERT_MSG(sizeof(int) == sizeof(CDirEntryStatus),
    "Code Error in CDirEntry::RegisterAll()"
      "sizeof enum differs from sizeof int");
  RegisterWxString("InputDir",&m_sInputDir);
  RegisterWxString("OutputDir",&m_sOutputDir);
  RegisterWxString("OutputFile",&m_sOutputFile);
  RegisterWxDateTimeXML("StartTime",&m_dtStart);
  RegisterWxDateTimeXML("StopTime",&m_dtStop);
  Register("parms",&m_parmOsiris,(void *)&m_parmOsiris);
  RegisterWxString("RunOutput",&m_sRunOutput);
  RegisterInt("Status",(int *)&m_nStatus);
  RegisterLong("index",&m_nNdx);
}

bool CDirEntry::LoadFromNode(wxXmlNode *p, void *pObj)
{
  bool bRtn = nwxXmlPersist::LoadFromNode(p,pObj);
  _AfterLoadFromNode(bRtn);
  return bRtn;
}
bool CDirEntry::LoadFromNode(wxXmlNode *p)
{
  bool bRtn = nwxXmlPersist::LoadFromNode(p);
  _AfterLoadFromNode(bRtn);
  return bRtn;
}
void CDirEntry::_AfterLoadFromNode(bool bRtn)
{
  if(bRtn)
  {
    UpdateStatus();
    _CheckInputDir();
    if(m_parmOsiris.GetInputDirectory().IsEmpty())
    {
      m_parmOsiris.SetInputDirectory(m_sInputDir);
    }
    if(m_parmOsiris.GetOutputDirectory().IsEmpty())
    {
      m_parmOsiris.SetOutputDirectory(m_sOutputDir);
    }
  }
  else
  {
    SetStatus(DIRENTRY_ERROR);
  }
  m_bOutputModified = true;
}
void CDirEntry::_CheckInputDir()
{
  wxString sDir = GetInputDir();
  nwxFileUtil::EndWithSeparator(&sDir);
  m_bInputExists = wxFileName::DirExists(sDir);
}

/////////////////////////////////////////////////////////// CDirList
CDirList::FILE_MAP CDirList::g_mapFileSpec;

void CDirList::RemoveAt(size_t ndx)
{
  m_vpDir.removeAt(ndx);
}
/*
void CDirList::RemoveNonPending()
{
  CDirEntry *pe;
  size_t n = GetCount();
  while(n > 0)
  {
    --n;
    pe = At(n);
    if(pe->GetStatus() != DIRENTRY_PENDING)
    {
      RemoveAt(n);
    }
  }
}
*/
void CDirList::CopyNonEntry(const CDirList &x)
{
  // copy all data except entries
  // this code is common for "operator =" and CopyPending
  Cleanup();
  m_parmOsiris = x.m_parmOsiris;
  m_sDirInput = x.m_sDirInput;
  m_sDirInputBase = x.m_sDirInputBase;
  m_sDirOutput = x.m_sDirOutput;
  m_sLastDir = x.m_sLastDir;
  m_sOpenError = x.m_sOpenError;
  m_sFileName = x.m_sFileName;
  m_sDirSave = x.m_sDirSave;
  m_sDirLoad = x.m_sDirLoad;
  m_nStatus = x.m_nStatus;
}

/*
void CDirList::CopyPending(const CDirList &x)
{
  // this is crap because the source should NOT be set to pending
  CopyNonEntry(x);
  size_t n = x.m_vpDir.Size();
  vector<CDirEntry *> *pv = m_vpDir.Get();
  CDirEntry *pe;
  pv->reserve(n);
  for(size_t i = 0; i < n; i++)
  {
    if(pe->GetStatus() == DIRENTRY_PENDING)
    {
      pe = x.m_vpDir.at(i)->Clone();
      pv->push_back(pe);
    }
  }
}
*/

CDirList &CDirList::operator =(const CDirList &x)
{

  CopyNonEntry(x);

  // now the difficult part, copy the array

  size_t n = x.m_vpDir.Size();
  vector<CDirEntry *> *pv = m_vpDir.Get();
  CDirEntry *pe;
  pv->reserve(n);

  for(size_t i = 0; i < n; i++)
  {
    pe = x.m_vpDir.at(i)->Clone();
    pv->push_back(pe);
  }
  return *this;
}

bool CDirList::Traverse(
  const wxString &sDirInput, 
  const wxString &sDirOutput,
  int nFileType)
{
  INITMAP();
  Cleanup();
  if(!nFileType)
  {
    nFileType = FILE_ANY;
  }
  m_nFileType = nFileType;
  if(!wxDir::Exists(sDirInput)) //wxFileName::Mkdir(sDirInput,0755,wxPATH_MKDIR_FULL))
  {
    m_nStatus = DIRLIST_INPUT_NOT_EXIST;
  }
  else if(!wxFileName::Mkdir(sDirOutput,0755,wxPATH_MKDIR_FULL))
  {
    m_nStatus = DIRLIST_OUTPUT_NOT_EXIST;
  }
  else
  {
    wxFileName fnIn(sDirInput);
    wxFileName fnOut(sDirOutput);
    m_sDirInput = fnIn.GetFullPath();
    m_sDirOutput = fnOut.GetFullPath();
    m_sDirSave.Empty();
    m_sDirLoad.Empty();

    // for each input file, remove base input and last dir
    // append to output dir
    nwxFileUtil::EndWithSeparator(&m_sDirOutput);
    nwxFileUtil::NoEndWithSeparator(&m_sDirInput);
    m_sDirInputBase = fnIn.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    nwxFileUtil::EndWithSeparator(&m_sDirInputBase);
    if(m_parmOsiris.GetTimeStampSubDir())
    {
      CreateFileName(true);
    }
    else
    {
      ClearTimeStamp();
    }
    wxDir dirIn(sDirInput);
    OnDir(sDirInput);
    dirIn.Traverse(*this,wxEmptyString,wxDIR_DIRS);
    m_nStatus = m_vpDir->empty() ? DIRLIST_EMPTY : DIRLIST_OK;
    m_setDirs.clear();
  }
  return IsOK();
};
void CDirList::Cleanup()
{
  m_setDirs.clear();
  m_sDirInput.Empty();
  m_sDirOutput.Empty();
  m_sLastDir.Empty();
  m_sOpenError.Empty();
  m_sDirSave.Empty();
  m_sDirLoad.Empty();
  m_nStatus = DIRLIST_UNSPECIFIED;
  m_vpDir.Clear();
}

wxDirTraverseResult CDirList::OnDir(const wxString &s)
{
  wxString sFileName;
  map<int,const wxString>::iterator itr;
  for(itr = g_mapFileSpec.begin();
    itr != g_mapFileSpec.end();
    ++itr)
  {
    if(itr->first & m_nFileType)
    {
      sFileName = wxDir::FindFirst(s,itr->second,wxDIR_FILES);
      if(!sFileName.IsEmpty())
      {
        OnFile(sFileName);
        break;
      }
    }
  }
  return wxDIR_CONTINUE;
}
wxDirTraverseResult CDirList::OnFile(const wxString& filename)
{
  wxFileName fn(filename);
  wxString sInputPath = fn.GetPath(wxPATH_GET_VOLUME); // chop off file name to get input path
  if((sInputPath != m_sLastDir)
    && fn.IsFileReadable() 
    && !Find(sInputPath)
    )
  {    
    wxString sOut(m_sDirOutput);
    m_sLastDir = sInputPath;
    wxFileName fnPath(sInputPath);
    wxString sPath = fnPath.GetPath(wxPATH_GET_VOLUME); // chop off last directory for output path
    if(sPath.StartsWith(m_sDirInputBase))
    {
      sOut.Append(sPath.Mid(m_sDirInputBase.Len()));
    }
    AddEntry(sInputPath,sOut);
    m_setDirs.insert(sInputPath);
  }
  return wxDIR_CONTINUE;
}
CDirEntry *CDirList::AddEntry(const wxString &sInputDir, const wxString &sOutputDir)
{
  CDirEntry *pEntry(new CDirEntry(sInputDir,sOutputDir,m_sTimeStamp));
  pEntry->SetParmOsiris(m_parmOsiris);
  pEntry->SetIndex((long)m_vpDir->size());
  m_vpDir->push_back(pEntry);
  return pEntry;
}
wxDirTraverseResult CDirList::OnOpenError(const wxString& openerrorname)
{
  m_nStatus = DIRLIST_READ_ERROR;
  m_sOpenError = openerrorname;
  return wxDIR_STOP;
}

void CDirList::RegisterAll(bool)
{
  wxASSERT_MSG(sizeof(int) == sizeof(CDirListStatus),
    "Code Error in CDirList::RegisterAll()"
      "sizeof enum differs from sizeof int");

  RegisterWxString("dirInput",&m_sDirInput);
  RegisterWxString("dirOutput",&m_sDirOutput);
  RegisterWxString("dirOpenError",&m_sOpenError);
  RegisterInt("status",(int *) &m_nStatus);
  Register("parms",&m_parmOsiris,(void *)&m_parmOsiris);
  Register("dirList",&m_vpDir);
}

bool CDirList::LoadFile(const wxString &sFileName)
{
  Cleanup();
  bool bRtn = nwxXmlPersist::LoadFile(sFileName);
  if(bRtn)
  {
    m_sFileName = sFileName;
    wxFileName fn(m_sFileName);
    m_sDirLoad = fn.GetPath();
    nwxFileUtil::EndWithSeparator(&m_sDirLoad);
    m_parmOsiris.SetOutputDirectory(m_sDirLoad);
    GetSavePath();
  }
  else
  {
    Cleanup();
  }
  return bRtn;
}
const wxString &CDirList::GetSavePath(bool bCreate)
{
  // return the full directory path of where this file was 
  // or will be saved based on m_sDirInput and m_sDirOutput
  if(m_sDirSave.IsEmpty())
  {
    wxString sFnIn;
    m_sDirSave = m_sDirOutput;

#define CHECK_DIR_NAME 0
#if CHECK_DIR_NAME
    bool bCreateSubDir = true;
#endif
    {
      wxString sDirInput(m_sDirInput);
      nwxFileUtil::NoEndWithSeparator(&sDirInput);
      wxFileName fnIn(sDirInput);
      sFnIn = fnIn.GetFullName();

#if CHECK_DIR_NAME
    // check if input and output directory names are identical
    // if not, then a subdirectory will be created in the output directory

      nwxFileUtil::NoEndWithSeparator(&m_sDirSave);
      wxFileName fnOut(m_sDirSave);
      wxString sFnOut = fnOut.GetFullName();
      bCreateSubDir = !nwxString::FileNameStringEqual(sFnIn,sFnOut);
#endif
    }
    nwxFileUtil::EndWithSeparator(&m_sDirSave);
#if CHECK_DIR_NAME
    if(bCreateSubDir)
#endif
    {
      m_sDirSave.Append(sFnIn);
      nwxFileUtil::EndWithSeparator(&m_sDirSave);
    }
  }
  if(!bCreate) {} // done
  else if(wxDirExists(m_sDirSave)) {} // done
  else if(!wxFileName::Mkdir(m_sDirSave,0755,wxPATH_MKDIR_FULL))
  {
    // could not create
    m_sDirSave.Empty();
  }
  return m_sDirSave;
}

int CDirList::MaxDayOfMonth(int nYear, int nMonth)
{
  static const int DOM[] =
  {
    0,
    31, 28, 31, 30, // jan, feb, mar, apr
    31, 30, 31, 31, // may, jun, jul, aug
    30, 31, 30, 31  // sep, oct, nov, dev
  };
  int nMaxDay = -1;
  if(nMonth > 0 && nMonth < 13)
  {
    int nPlus = ((nMonth == 2) && !(nYear & 3)) ? 1 : 0; 
        // feb of leap year
    nMaxDay = DOM[nMonth] + nPlus;
  }
  return nMaxDay;
}

bool CDirList::IsTimeStamp(const wxString &s)
{
  bool bRtn = true;
  if(s.Len() != 15)
  {
    bRtn = false;
  }
  else if(s.GetChar(8) != '_')
  {
    bRtn = false;
  }
  else if(!nwxString::IsInteger(s.Left(8),false))
  {
    bRtn = false;
  }
  else if(!nwxString::IsInteger(s.Right(6),false))
  {
    bRtn = false;
  }
  else
  {
#define BETWEEN(n,min,max) ((n >= min) && (n <= max))

    int nY = atoi(s.Left(4).utf8_str());
    int nM = atoi(s.Mid(4,2).utf8_str());
    int nD = atoi(s.Mid(6,2).utf8_str());
    int nHH = atoi(s.Mid(9,2).utf8_str());
    int nMM = atoi(s.Mid(11,2).utf8_str());
    int nSS = atoi(s.Mid(13,2).utf8_str());
    // check year to see if newer than this software
    if( !BETWEEN(nY,2011,2099) )
    {
      bRtn = false;
    }
    else if( !BETWEEN(nM,1,12) )
    {
      bRtn = false;
    }
    else if( !BETWEEN(nD,1,MaxDayOfMonth(nY,nM)) )
    {
      bRtn = false;
    }
    else if( !BETWEEN(nHH,0,23) )
    {
      bRtn = false;
    }
    else if(! BETWEEN(nMM,0,59) )
    {
      bRtn = false;
    }
    else if(! BETWEEN(nSS,0,59) )
    {
      bRtn = false;
    }
  }
#undef BETWEEN
  return bRtn;
}

const wxString &CDirList::GetTimeStamp(int nAdd)
{
  // if nAdd != 0, then rebuild time stamp
  // if nAdd > 0, then add to time in seconds
  // if nAdd < 0, this signifies a rebuild to current time
  if(nAdd || m_sTimeStamp.IsEmpty())
  {
      time_t t;
      time(&t);
      if(nAdd > 0)
      {
        t += nAdd;
      }
      struct tm *plt = localtime(&t);
      m_sTimeStamp.Printf("%04d%02d%02d_%02d%02d%02d",
      plt->tm_year + 1900, plt->tm_mon + 1, plt->tm_mday,
      plt->tm_hour, plt->tm_min, plt->tm_sec);
  }
  return m_sTimeStamp;
}
bool CDirList::CreateFileName(bool bForce)
{
  bool bDone = false;
  if((!bForce) && m_sFileName.Len())
  {
    bDone = true;
  }
//  else if(!wxFileName::Mkdir(m_sDirSave,0755,wxPATH_MKDIR_FULL))
//  {
//    bDone = false;
//  }
  else
  {
    wxString sPathBase = GetSavePath(true);
    if(!sPathBase.IsEmpty())
    {
      wxString sName(sPathBase);
      wxString sPath;
      nwxFileUtil::NoEndWithSeparator(&sName);
      wxFileName fn(sName);
      sName = fn.GetFullName();
      sName.Replace(" ","_",true);
      if(sName.IsEmpty())
      {
        wxASSERT_MSG(
          false,"Problem with CDirList::CreateFileName()");
        sName = "run_";
      }
      else
      {
        sName.Append("_");
      }
      wxString sFullName;
      wxString sExt(EXT_BATCH);
      int nAdd = 0;
      sPathBase.Append(sName);
      bool bDoneLoop = false;
      while(!bDoneLoop)
      {
        sPath = sPathBase;
        sPath.Append(GetTimeStamp(nAdd));
        sPath.Append(sExt);
        sFullName = nwxFileUtil::SetupFileName(sPath,sExt);
        if(sFullName.IsEmpty())
        {
          bDoneLoop = true; // end loop
          wxASSERT_MSG(false,
            "sFullName is Empty in CDirList::CreateFileName()");
        }
        else if(wxFileName::FileExists(sFullName))
        {
          nAdd++;
        }
        else
        {
          wxFile xFile(sFullName.wc_str(),wxFile::write_excl);
          if(!xFile.IsOpened())
          {
            wxString s("Cannot create file: ");
            s.Append(sFullName);
            wxASSERT_MSG(false,s);
          }
          else
          {
            m_sFileName = sFullName;
            bDone = true;
          }
          bDoneLoop = true;
        }
      }
    }
    else
    {
      wxASSERT_MSG(
          false,"empty path in CDirList::CreateFileName()");
    }
  }
  return bDone;
}
//                              CDirList::SaveFile()
//
//  generate file name based on output dir and time
//    file name begins with dir name and ends 
//    with ".obr" (osiris batch report)
//
//
bool CDirList::SaveFile()
{
  bool bRtn = CreateFileName();
  if(bRtn)
  {
    bRtn = SaveFile(m_sFileName);
  }
  if(bRtn)
  {
    m_sDirLoad = GetSavePath();
  }
  else
  {
    m_sDirLoad.Empty();
    m_sDirSave.Empty();
  }
  return bRtn;
}


bool CDirList::SaveFile(const wxString &sFileName)
{
  UpdateOutputDir();
  m_sDirSave.Empty();
  bool bRtn = nwxXmlPersist::SaveFile(sFileName);
  if(bRtn && ((void *)&sFileName != (void *)&m_sFileName))
  {
    m_sFileName = sFileName;
  }
  return bRtn;
}

wxString CDirList::GetBaseFileName()
{
  wxFileName fn(m_sFileName);
  wxString sRtn = fn.GetFullName();
  return sRtn;
}
void CDirList::UpdateOutputDir()
{
  for(vector<CDirEntry *>::iterator itr = m_vpDir->begin();
    itr != m_vpDir->end();
    ++itr)
  {
    CDirEntry *pe(*itr);
    pe->GetOutputFile();
  }
}

bool CDirList::AdjustFileName(wxString *ps, const wxString &sOld, const wxString &sNew)
{
  //
  //  modify the path of a file name *ps from sOld to sNew
  //  iff *ps is within the path of sOld
  //  return true if *ps is modified
  size_t nLen = sOld.Len();
  bool bRtn = false;
  if(nLen && (ps->Len() >= nLen))
  {
    wxFileName fOld(sOld);
    wxFileName fNew(sNew);
    if(!fOld.SameAs(fNew))
    {
      wxFileName fn(ps->Mid(0,nLen));
      if(fn.SameAs(fOld))
      {
        wxString s = sNew;
        s += ps->Mid(nLen);
        (*ps) = s;
        bRtn = true;
      }
    }
  }
  return bRtn;
}
int CDirList::ExtToType(const wxString &_sExt)
{
  int nRtn = _sExt.IsEmpty() ? CDirList::FILE_FSA : 0;
  if(!nRtn)
  {
    wxString sExt("*.");
    sExt.Append(_sExt);
    sExt.MakeLower();
    INITMAP();
    for(FILE_MAP::iterator itr = g_mapFileSpec.begin();
      itr != g_mapFileSpec.end();
      ++itr)
    {
      if(!itr->second.CmpNoCase(sExt))
      {
        nRtn = itr->first;
        break;
      }
    }
  }
  return nRtn;
}


/*
void CDirList::AdjustOutputDirs()
{
  if( 
    (!m_sDirSave.IsEmpty()) && 
    (!m_sDirLoad.IsEmpty()) &&
    (m_sDirSave != m_sDirLoad) )
  {
    //
    // if this obt file has moved, then adjust all 
    // CDirEntry occurrences
    //
    wxString sOutput;
    vector<CDirEntry *> *pvp = m_vpDir.Get();
    CDirEntry *p;
    size_t n = pvp->size();
    for(size_t i = 0; i < n; i++)
    {
      p = pvp->at(i);
      sOutput = p->GetOutputDir();
      if(AdjustFileName(&sOutput))
      {
        p->SetOutputDir(sOutput);
      }
    }
    m_sDirLoad = m_sDirSave;
    m_parmOsiris.SetOutputDirectory(m_sDirSave);
  }
}

*/
