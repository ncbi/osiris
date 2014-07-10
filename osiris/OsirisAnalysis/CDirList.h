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

*  FileName: CDirList.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIR_LIST_H__
#define __C_DIR_LIST_H__

#include <wx/string.h>
#include <wx/dir.h>
#include <wx/colour.h>
#include <wx/stopwatch.h>
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/stdb.h"
#include <map>
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "CParmOsiris.h"

enum CDirListStatus
{
  DIRLIST_OK = 0,
  DIRLIST_UNSPECIFIED,
  DIRLIST_EMPTY,
  DIRLIST_INPUT_NOT_EXIST,
  DIRLIST_OUTPUT_NOT_EXIST,
  DIRLIST_READ_ERROR
};

enum CDirEntryStatus
{
  DIRENTRY_PENDING = 0,
  DIRENTRY_DONE,
  DIRENTRY_RUNNING,
  DIRENTRY_ERROR,
  DIRENTRY_EXPORT_ERROR,
  DIRENTRY_CANCELED
};


class CDirEntry : public nwxXmlPersist
{
public:
  static bool CanViewStatus(int n)
  {
    bool bRtn = 
      (n == DIRENTRY_DONE) ||
      (n == DIRENTRY_ERROR) ||
      (n == DIRENTRY_EXPORT_ERROR);
    return bRtn;
  }
  static bool FinishedStatus(int n)
  {
    bool bRtn = 
      CanViewStatus(n) ||
      (n == DIRENTRY_CANCELED);
    return bRtn;
  }
  CDirEntry()
  {
    _Init();
    RegisterAll(true);
  }
  CDirEntry(const CDirEntry &x)
  {
    (*this) = x;
  }
  CDirEntry *Clone() const
  {
    return new CDirEntry(*this);
  }
  CDirEntry(const wxString &sInput, const wxString &sOutput, const wxString &sTimeStamp) //, long ndx = -1)
  {
    // STOP HERE - setup time stamp
    _Init();
    SetInputDir(sInput);
    m_sTimeStamp = sTimeStamp;
    m_sOutputDir = sOutput;
    m_sRunOutput.Alloc(16384);
    m_nNdx = -1; //ndx;
    RegisterAll(true);
  }

  bool InputDirExists() const
  {
    return m_bInputExists;
  }
  long GetIndex() const
  {
    return m_nNdx;
  }
  void SetIndex(long ndx)
  {
    m_nNdx = ndx;
  }
/*
  const wxString &GetOAR() const
  {
    return m_sOAR;
  }
  void SetOAR(const wxString &s)
  {
    m_sOAR = s;
  }
*/
  bool IsOutputModified() const
  {
    return m_bOutputModified;
  }
  const wxString &GetRunOutput()
  {
    m_bOutputModified = false;
    return m_sRunOutput;
  }
  void SetRunOutput(const wxString &s = wxEmptyString)
  {
    m_bOutputModified = true;
    m_sRunOutput = s;
  }
  void AppendRunOutput(const wxString &s)
  {
    m_bOutputModified = true;
    m_sRunOutput.Append(s);
  }

  const wxString &GetInputDir() const
  {
    return m_sInputDir;
  }
  const wxString &GetOutputDir() const
  {
    return m_sOutputDir;
  }
  const wxDateTime &GetStartTime() const
  {
    return m_dtStart;
  }
  const wxDateTime &GetStopTime() const
  {
    return m_dtStop;
  }
  void ClearStartStopTime()
  {
    time_t t(0);
    m_dtStart.Set(t);
    m_dtStop.Set(t);
  }
  void SetStartTime()
  {
    m_dtStart.SetToCurrent();
  }
  void SetStopTime()
  {
    m_dtStop.SetToCurrent();
  }
  wxString FormatStartTime() const;
  wxString FormatStopTime() const;

  void SetOutputDir(const wxString &s)
  {
    m_sOutputDir = s;
    m_parmOsiris.SetOutputDirectory(s);
    m_sOutputFile.Empty();
  }
  CDirEntryStatus GetStatus() const
  {
    return m_nStatus;
  }
  const wxString &GetStatusString() const
  {
    return *m_psStatus;
  }
  const wxColour &GetStatusColour() const
  {
    return *m_pStatusColour;
  }

  void SetStatus(CDirEntryStatus n)
  {
    if(n != m_nStatus)
    {
      m_nStatus = n;
      UpdateStatus();
    }
  }
/*
  bool IsStatusOK() const
  {
    return (m_nStatus == DIRENTRY_DONE) && !m_sOAR.IsEmpty();
  }
*/
  bool IsStatusIncomplete() const
  {
    return (m_nStatus == DIRENTRY_PENDING) ||
      (m_nStatus == DIRENTRY_RUNNING);
  }
  void Cancel()
  {
    if(IsStatusIncomplete())
    {
      SetStatus(DIRENTRY_CANCELED);
    }
  }
  const wxString &GetTimeStamp() const
  {
    return m_sTimeStamp;
  }
  const wxString &GetOutputFile();

//  const wxString &GetOutputFile(const wxString &sOutputDir);
//  const wxString &GetOutputFile()
//  {
//    return GetOutputFile(m_sOutputDir);
//  }
  void SetParmOsiris(const CParmOsiris &x)
  {
    m_parmOsiris = x;
  }
  const CParmOsiris *GetParmOsiris() const
  {
    return &m_parmOsiris;
  }
  virtual void RegisterAll(bool = false);
  virtual bool LoadFromNode(wxXmlNode *, void *pObj);
  virtual bool LoadFromNode(wxXmlNode *);
  void CleanupFiles();
private:
  void SetInputDir(const wxString &s)
  {
    m_sInputDir = s;
    m_parmOsiris.SetInputDirectory(s);
    m_sOutputFile.Empty();
    _CheckInputDir();
  }
  void _AfterLoadFromNode(bool bRtn);
  void _CheckInputDir();
  void UpdateStatus();
  void _Init()
  {
    ClearStartStopTime();
    m_nNdx = -1;
    m_bOutputModified = false;
    m_bInputExists = false;
    m_nStatus = DIRENTRY_PENDING;
    UpdateStatus();
    m_sRunOutput.Alloc(2048);
  }

  CParmOsiris m_parmOsiris; 
    // used for saving in obt file. if a re-analysis
    // takes place with 'some' directories and different
    // parameters then each run can have different parameters
    // so this is why they are saved with each directory

  wxString m_sInputDir;  // also in m_parmOsiris, but more up to date
  wxString m_sOutputDir;
  wxString m_sOutputFile;
  wxString m_sTimeStamp; // used for setting up output file, 
          // but not registered in RegisterAll():
//  wxString m_sOAR;
  wxString m_sRunOutput;
  wxDateTime m_dtStart;
  wxDateTime m_dtStop;
  const wxString *m_psStatus;
  const wxColour *m_pStatusColour;
  CDirEntryStatus m_nStatus;
  long m_nNdx;
  bool m_bOutputModified;
  bool m_bInputExists;

  static const wxString g_sSTATUS_PENDING;
  static const wxString g_sSTATUS_DONE;
  static const wxString g_sSTATUS_RUNNING;
  static const wxString g_sSTATUS_ERROR;
  static const wxString g_sSTATUS_EXPORT_ERROR;
  static const wxString g_sSTATUS_CANCELED;

  static const wxColour g_colPENDING;
  static const wxColour g_colDONE;
  static const wxColour g_colERROR;

};

class CDirList : public wxDirTraverser, public nwxXmlPersist
{
public:

  CDirList() : m_vpDir("dirEntry")
  {
    _Init();
    RegisterAll(true);
  }
  CDirList(const CDirList &x) : m_vpDir("dirEntry")
  {
    _Init();
    RegisterAll(true);
    (*this) = x;
    ClearTimeStamp();
  }
  CDirList *Clone() const
  {
    return new CDirList(*this);
  }

//  void CopyPending(const CDirList &x);
  void CopyNonEntry(const CDirList &x);

  virtual ~CDirList()
  {
    Cleanup();
  }
  CDirList &operator = (const CDirList &);
  CDirEntry *AddEntry(const wxString &sInputDir, const wxString &sOutputDir);
  virtual wxDirTraverseResult OnDir(const wxString& dirname);
  virtual wxDirTraverseResult OnFile(const wxString& filename);
  virtual wxDirTraverseResult OnOpenError(const wxString& openerrorname);

  CDirEntry *At(size_t n)
  {
    return m_vpDir->at(n);
  }
  size_t GetCount()
  {
    return m_vpDir->size();
  }
  CDirListStatus Status() const
  {
    return m_nStatus;
  }
  bool IsOK()
  {
    return !!GetCount() && !m_nStatus;
  }
  const wxString &OpenErrorName()
  {
    return m_sOpenError;
  }
  const wxString &GetFileName() const
  {
    return m_sFileName;
  }
  void SetParmOsiris(const CParmOsiris &x)
  {
    m_parmOsiris = x;
  }
  void SetParmOsirisAndUpdate(const CParmOsiris &x)
  {
    SetParmOsiris(x);
    m_sDirInput = x.GetInputDirectory();
    m_sDirOutput = x.GetOutputDirectory();
    if(x.GetTimeStampSubDir())
    {
      CreateFileName(true);
    }
    else
    {
      ClearTimeStamp();
    }
  }
  bool SetParmOsirisAndTraverse(const CParmOsiris &x, int nFileType)
  {
    SetParmOsiris(x);
    bool bRtn = Traverse(nFileType);
    return bRtn;
  }
  bool SetParmOsirisAndTraverse(const CParmOsiris &x, const wxString &sFileType)
  {
    int n = ExtToType(sFileType);
    return SetParmOsirisAndTraverse(x,n);
  }
  const CParmOsiris &GetParmOsiris()
  {
    return m_parmOsiris;
  }
  const wxString &GetDirLoad()
  {
    return m_sDirLoad;
  }
  const wxString &GetDirInput()
  {
    return m_sDirInput;
  }
  const wxString &GetDirOutput()
  {
    return m_sDirOutput;
  }
  void RemoveAt(size_t ndx);
  // void RemoveNonPending();
  wxString GetBaseFileName();
  bool SaveFile(); // automatically generate file name
  static bool AdjustFileName(
    wxString *ps, const wxString &sOld, const wxString &sNew);
  bool AdjustFileName(wxString *ps)
  {
    // adjust file name based on changes of location of the .obr file since it was saved
    // return true if *ps was changed
    return AdjustFileName(ps,m_sDirSave,m_sDirLoad);
  }

  void UpdateOutputDir();
  virtual bool LoadFile(const wxString &sFileName);
  virtual bool SaveFile(const wxString &sFileName);
  virtual void RegisterAll(bool = false);
  static const wxChar *GetDefaultExt()
  {
    return DEFAULT_EXT;
  }
  static int MaxDayOfMonth(int nYear, int nMonth);
  static bool IsTimeStamp(const wxString &s);
  static wxString GetValidExtension(const wxString &_s)
  {
    wxString s(_s);
    wxString sRtn(wxEmptyString);
    s.MakeLower();
    INITMAP();
    for(FILE_MAP::iterator itr = g_mapFileSpec.begin();
      itr != g_mapFileSpec.end();
      ++itr)
    {
      if(s.EndsWith(itr->second.Mid(1).MakeLower()))
      {
        sRtn = itr->second.Mid(2);
        break;
      }
    }
    return sRtn;
  }
  static void GetTypes(vector<wxString> *ps)
  {
    INITMAP();
    ps->clear();
    ps->reserve(2);
    for(FILE_MAP::iterator itr = g_mapFileSpec.begin();
      itr != g_mapFileSpec.end();
      ++itr)
    {
      ps->push_back(itr->second.Mid(2));
    }
  }
  enum
  {
    FILE_FSA = 1,
    FILE_HID = 2,
    FILE_ANY = FILE_FSA | FILE_HID // bitwise 'or' of all the above
  };

  typedef map<int,const wxString> FILE_MAP;
private:
  static FILE_MAP g_mapFileSpec;
  static void INITMAP()
  {
    if(g_mapFileSpec.empty())
    {
      g_mapFileSpec.insert(FILE_MAP::value_type(FILE_FSA,"*.fsa")); 
      g_mapFileSpec.insert(FILE_MAP::value_type(FILE_HID,"*.hid"));
    }
  }
  static int ExtToType(const wxString &_sExt);

  bool Traverse(
    const wxString &sDirInput,
    const wxString &sDirOutput,
    int nFileType = CDirList::FILE_ANY);

  bool Traverse(int nFileType = CDirList::FILE_ANY)
  {
    return Traverse(
      m_parmOsiris.GetInputDirectory(),
      m_parmOsiris.GetOutputDirectory(),
      nFileType);
  }

  bool CreateFileName(bool bForce = false);
  void ClearTimeStamp()
  {
    m_sTimeStamp.Empty();
  }
  const wxString &GetTimeStamp(int nAdd = 0);
  const wxString &GetSavePath(bool bCreate = false);
  bool Find(const wxString &s)
  {
    bool b = m_setDirs.find(s) == m_setDirs.end();
    return !b;
  }
  void _Init()
  {
    m_nStatus = DIRLIST_UNSPECIFIED;
    m_vpDir->reserve(400);
  }
  void Cleanup();

//   DATA
  TnwxXmlPersistVector<CDirEntry> m_vpDir;
  CParmOsiris m_parmOsiris;
  wxString m_sDirInput;
  wxString m_sDirInputBase;
  wxString m_sDirOutput;
  wxString m_sLastDir;
  wxString m_sOpenError;
  wxString m_sFileName;
  wxString m_sDirSave;
  wxString m_sDirLoad;
  wxString m_sTimeStamp;
  set<wxString> m_setDirs;
  CDirListStatus m_nStatus;
  int m_nFileType;
	static const wxChar * const DEFAULT_EXT;
};


#endif
