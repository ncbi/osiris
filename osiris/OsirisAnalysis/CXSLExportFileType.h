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
*  FileName: CXSLExportFileType.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_XSL_EXPORT_FILE_TYPE_H__
#define __C_XSL_EXPORT_FILE_TYPE_H__


#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/nwxString.h"
#include "wxXsl/wxXslSheet.h"
#include <wx/dir.h>

class wxArrayString;


//*********************************************** CXSLParam

class CXSLParam : public nwxXmlPersist
{

public:

typedef enum 
{
  TEXT = 0,
  INTEGER,
  DECIMAL,
  CHOICE,
  CHOICE_TEXT,
  CHECKBOX,
  INPUT_FILE,
  IGNORE_PARAM,
  COUNT
} CXSLParamType;

// the following list is used for 
// display when selecting a data type for a parameter
// and the offset for each must correspond to the enum
// above

#define CXSL_PARAM_IGNORE wxS("ignore")

#define CXSLParamTypeStrings \
{ \
  wxS("text"), \
  wxS("integer"), \
  wxS("decimal"), \
  wxS("choice"), \
  wxS("choice/text"), \
  wxS("checkbox"), \
  wxS("input file name"), \
  CXSL_PARAM_IGNORE \
}


  CXSLParam(
    const wxString &sName = wxEmptyString) :
    m_sName(sName),
    m_sDescription(wxEmptyString),
    m_sType(g_TYPES[0]),
    m_vsChoiceList("value"),
    m_dMin(0.0),
    m_dMax(0.0),
    m_bHasMin(0.0),
    m_bHasMax(0.0)
  {
    RegisterAll(true);
  }
  CXSLParam(const CXSLParam &x) :
    m_vsChoiceList("value")
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CXSLParam() {}

  CXSLParam &operator = (const CXSLParam &x);
  void Update(const CXSLParam &x);
  bool operator == (const CXSLParam &x) const;
  bool operator != (const CXSLParam &x) const
  {
    return !((*this) == x);
  }
  static void GetChoiceArray(wxArrayString *psChoices);
  //  using name for the search key in CXSLExportFileType::m_mapParam
  //  the following two methods are required for TnwxXmlPersistMap

  const wxString &GetKey() const
  {
    return m_sName;
  }
  void SetKey(const wxString &s)
  {
    // should be called ONLY by CXSLExportFileType::m_mapParam
    m_sName = s;
  }

  // set/get methods

  // <name> tag

  const wxString &GetName() const
  {
    return m_sName;
  }
  // SetName() and SetDefaultValue() are intentionally 
  // omitted because it is obtained
  // from an xsl file and cannot be changed here

  // <description>

  void SetDescription(const wxString &s)
  {
    m_sDescription = s;
  }

  const wxString &GetDescription() const
  {
    return m_sDescription;
  }
  const wxString &GetDescriptionOrName() const
  {
    return m_sDescription.IsEmpty() ? m_sName : m_sDescription;
  }

  //  <type> 

  bool IsChoiceType() const
  {
    return 
      (m_sType == g_TYPES[CHOICE]) ||
      (m_sType == g_TYPES[CHOICE_TEXT]);
  }
  bool IsNumericType() const
  {
    return
      (m_sType == g_TYPES[INTEGER]) ||
      (m_sType == g_TYPES[DECIMAL]);
  }
  static bool IsCheckboxType(const wxString &sType)
  {
    return sType == g_TYPES[CHECKBOX];
  }
  bool IsCheckboxType() const
  {
    return IsCheckboxType(m_sType);
  }
  static bool IsInputFileType(const wxString &sType)
  {
    return sType == g_TYPES[INPUT_FILE];
  }
  bool IsInputFileType() const
  {
    return IsInputFileType(m_sType);
  }
  static bool IsIgnoreType(const wxString &sType)
  {
    return (sType == CXSL_PARAM_IGNORE);
  }
  bool IsIgnoreType() const
  {
    return IsIgnoreType(m_sType);
  }

  const wxString &GetType() const
  {
    return m_sType;
  }
  static CXSLParamType GetTypeEnum(const wxString &sType);
  CXSLParamType GetTypeEnum() const
  {
    return GetTypeEnum(m_sType);
  }
  bool SetType(CXSLParamType n); 
  // return true if n is within range

  bool SetType(const wxString &sType); 
  // return true if sType is in the list, CXSLParamTypeStrings, above


  // <choicelist>

  const vector<wxString> &GetChoiceList() const
  {
    return m_vsChoiceList.GetVector();
  }
  void SetChoiceList(const vector<wxString> &vs)
  {
    vector<wxString> &x = m_vsChoiceList.GetVector();
    x = vs;
  }
  bool BuildChoiceList(
    wxArrayString *pas,
    const wxString &sFind = wxEmptyString) const;

  // <min> <max> <has-min> <has-max>

  bool GetHasMin() const
  {
    return m_bHasMin;
  }
  bool GetHasMax() const
  {
    return m_bHasMax;
  }
  double GetMin() const
  {
    return m_dMin;
  }
  double GetMax() const
  {
    return m_dMax;
  }
  

  void SetNoMin()
  {
    m_bHasMin = false;
    m_dMin = 0.0;
  }
  void SetNoMax()
  {
    m_bHasMax = false;
    m_dMax = 0.0;
  }
  void SetMin(double d)
  {
    m_dMin = d;
    m_bHasMin = true;
  }
  void SetMax(double d)
  {
    m_dMax = d;
    m_bHasMax = true;
  }

  bool CheckRange(double d) const
  {
    bool bRtn = false;
    if(!IsNumericType())
    {}
    else if(m_bHasMin && (d < m_dMin))
    {}
    else if(m_bHasMax && (d > m_dMax))
    {}
    else
    {
      bRtn = true;
    }
    return bRtn;
  }

  // <checked-value> <unchecked-value>

  void SetCheckedValue(const wxString &s)
  {
    m_sCheckedValue = s;
  }
  void SetUncheckedValue(const wxString &s)
  {
    m_sUncheckedValue = s;
  }
  const wxString &GetCheckedValue() const
  {
    return m_sCheckedValue;
  }
  const wxString &GetUncheckedValue() const
  {
    return m_sUncheckedValue;
  }
  const wxString &GetCheckboxValue(bool bChecked) const
  {
    const wxString &s =
      bChecked
      ? GetCheckedValue()
      : GetUncheckedValue();
    return s;
  }

  // input file type

  bool IsSaveLastLocation() const;

  bool IsFileExtOK(const wxString &s) const;

  void SetInFileDefaultDir(const wxString &s)
  {
    m_sInFileDefaultDirectory = s;
  }
  const wxString &GetInFileDefaultDir() const
  {
    return m_sInFileDefaultDirectory;
  }
  void SetInFileRequired(bool b)
  {
    m_sInFileRequired = b ? "1" : "";
  }
  void SetInFileAllowOverride(bool b)
  {
    m_sInFileAllowOverride = b ? "1" : "";
  }
  static bool GetBoolValue(const wxString &s)
  {
    bool bRtn = true;
    if(s.IsEmpty())
    {
      bRtn = false;
    }
    else if(!s.CmpNoCase("false"))
    {
      bRtn = false;
    }
    else if(s == "0")
    {
      bRtn = false;
    }
    return bRtn;
  }
  bool GetInFileRequired() const
  {
    return GetBoolValue(m_sInFileRequired);
  }
  bool GetInFileAllowOverride() const
  {
    return GetBoolValue(m_sInFileAllowOverride);
  }
  void SetInFileTypeList(const wxString &s, wxString *psError = NULL);
  wxString GetInFileTypeList() const;
  const vector<wxString> &GetInFileTypes() const
  {
    return m_vsInFileTypeList.GetVector();
  }

  static int TypeFromString(const wxString &s)
  {
    int nRtn = -1;
    for(int i = 0; i < COUNT; i++)
    {
      if(s == g_TYPES[i])
      {
        nRtn = i;
        i = COUNT;
      }
    }
    return nRtn;
  }



protected:
  virtual void RegisterAll(bool = false);
private:
  class IOminMax : public nwxXmlIOdouble
  {
  public:
    IOminMax(double d = 0) : nwxXmlIOdouble(d), m_pbNoSkip(NULL)
    {}
    virtual bool Skip(void *)
    {
      return (m_pbNoSkip != NULL) && !(*m_pbNoSkip);
    }
    void SetNoSkip(bool *pb)
    {
      m_pbNoSkip = pb;
    }
  private:
    bool *m_pbNoSkip;
  };
  class IOcheckBoxValue : public nwxXmlIOwxString
  {
  public:
    IOcheckBoxValue() : 
      nwxXmlIOwxString(true), m_psType(NULL)
    {}
    virtual bool Skip(void *p)
    {
      const wxString *ps = (const wxString *)p;
      bool bRtn = false;
      if(ps->IsEmpty())
      {
        bRtn = true;
      }
      else if(m_psType == NULL)
      {
        // bRtn = false; // already set
      }
      else
      {
        bRtn = !CXSLParam::IsCheckboxType(*m_psType);
      }
      return bRtn;
    }
    void SetType(wxString *psType)
    {
      m_psType = psType;
    }
  private:
    wxString *m_psType;
  };
  class IOinputFileType : public nwxXmlIOwxString
  {
  public:
    IOinputFileType() : nwxXmlIOwxString(true), m_psType(NULL)
    {};

    virtual bool Skip(void *p)
    {
      const wxString *ps = (const wxString *)p;
      bool bRtn = false;
      if(ps->IsEmpty())
      {
        bRtn = true;
      }
      else if(m_psType == NULL)
      {
        // bRtn = false; // already set
      }
      else
      {
        bRtn = !CXSLParam::IsInputFileType(*m_psType);
      }
      return bRtn;
    }
    void SetType(wxString *psType)
    {
      m_psType = psType;
    }
  private:
    wxString *m_psType;
  };
  IOminMax m_ioMin;
  IOminMax m_ioMax;
  IOcheckBoxValue m_ioCheck;
  IOinputFileType m_ioInFile;
  wxString m_sName;
  wxString m_sDescription;
  wxString m_sType;
  wxString m_sCheckedValue;
  wxString m_sUncheckedValue;
  // input file type
  wxString m_sInFileAllowOverride;
  wxString m_sInFileRequired;
  wxString m_sInFileDefaultDirectory;
  nwxXmlPersistVectorWxString m_vsInFileTypeList;

  nwxXmlPersistVectorWxString m_vsChoiceList;

  double m_dMin;
  double m_dMax;
  bool m_bHasMin;
  bool m_bHasMax;
  static const wxChar * const g_TYPES[COUNT];
};

typedef TnwxXmlPersistMap<wxString,CXSLParam,nwxStringLessNoCaseSort> CXSLmapParam;

//*********************************************** CXSLAutoCreate

class CXSLAutoCreate : public nwxXmlPersist
{
public:
  CXSLAutoCreate() : 
      m_bActive(false), m_bAppendDateToFileName(false)
  {
    RegisterAll(true);
  }
  CXSLAutoCreate(const CXSLAutoCreate &x) : 
    m_bActive(false), m_bAppendDateToFileName(false)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CXSLAutoCreate() {}
  CXSLAutoCreate &operator = (const CXSLAutoCreate &x);
  void Update(const CXSLAutoCreate &x);
  bool IsEmpty() const
  {
    return (!m_bActive) && 
      (!m_bAppendDateToFileName) &&
      m_sLocation.IsEmpty() &&
      m_sFileName.IsEmpty() &&
      m_sFileExt.IsEmpty();
  }
  bool operator == (const CXSLAutoCreate &x) const;
  bool operator != (const CXSLAutoCreate &x) const
  {
    return !((*this) == x);
  }
  static bool IsValidFileName(const wxString &s, bool bExt = false);
  static bool IsValidFileExt(const wxString &s)
  {
    return IsValidFileName(s,true);
  }
  virtual bool Skip()
  {
    bool bRtn = IsEmpty();
    return bRtn;
  }
  virtual bool Skip(void *p)
  {
    return ((CXSLAutoCreate *)p)->Skip();
  }

  // <active tag>

  void SetActive(bool b)
  {
    m_bActive = b;
  }
  bool IsActive() const
  {
    return m_bActive;
  }


  // <location> tag

  const wxString &GetLocation() const
  {
    return m_sLocation;
  }
  bool IsAnalysisLocation() const;
  void SetAnalysisLocation();
  void SetDefaultLocation();
  bool SetLocation(const wxString &s)
  {
    bool bRtn = wxDir::Exists(s);
    if(bRtn)
    {
      m_sLocation = s;
    }
    return bRtn;
  }


  // <file-name> tag

  const wxString &GetFileName() const
  {
    return m_sFileName;
  }
  bool IsAnalysisFileName() const;

  void SetUseAnalysisFileName();
  bool SetFileName(const wxString &s)
  {
    bool bRtn = IsValidFileName(s);
    if(bRtn)
    {
      m_sFileName = s;
    }
    return bRtn;
  }

  // <file-name-date> tag

  bool GetAppendDateToFileName() const
  {
    return m_bAppendDateToFileName;
  }
  void SetAppendDateToFileName(bool b)
  {
    m_bAppendDateToFileName = b;
  }

  // <file-extension> tag

  const wxString &GetFileExt() const
  {
    return m_sFileExt;
  }
  bool SetFileExt(const wxString &_s)
  {
    wxString s = _s;
    bool bRtn = false;
    s.MakeLower();
    while(s.StartsWith("."))
    {
      s = s.Mid(1);
    }
    if( s.IsEmpty() || IsValidFileExt(s) )
    {
      m_sFileExt = s;
      bRtn = true;
    }
    return bRtn;
  }



protected:
  virtual void RegisterAll(bool = false);
private:
  wxString m_sLocation;
  wxString m_sFileName;
  wxString m_sFileExt;
  bool m_bActive;
  bool m_bAppendDateToFileName;
};

//*********************************************** CXSLExportFileType

class CXSLExportFileType : public nwxXmlPersist
{
public:
  static const wxChar * const LIST_SEPARATOR;
  static const wxChar * const FILE_TYPE_ERROR;
  CXSLExportFileType() 
    : m_mapParam("param",false), m_bOverrideExt(false)
  {
    RegisterAll(true);
  }
  CXSLExportFileType(const CXSLExportFileType &x) 
    : m_mapParam("param",false), m_bOverrideExt(false)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CXSLExportFileType() {}

  static wxString CheckOutputFile(const wxString &sFileName);
  CXSLExportFileType &operator = (const CXSLExportFileType &x);
  bool operator == (const CXSLExportFileType &x) const;
  bool operator != (const CXSLExportFileType &x) const
  {
    return !((*this) == x);
  }
  void Update(const CXSLExportFileType &x);
  virtual bool Skip(void *p)
  {
    CXSLExportFileType *pObj = (CXSLExportFileType *)p;
//    bool bFail1 = !pObj->XSLfileOK();
    bool bFail2 = pObj->m_sName.IsEmpty();
//    return (bFail1 || bFail2);
    return bFail2;
  }
  virtual bool LoadFromNode(wxXmlNode *p, void *pObj)
  {
    bool bRtn = nwxXmlPersist::LoadFromNode(p,pObj);
    if(bRtn)
    {
      _SetupSheet();
    }
    return bRtn;
  }
  virtual bool LoadFromNode(wxXmlNode *p)
  {
    bool bRtn = nwxXmlPersist::LoadFromNode(p);
    if(bRtn)
    {
      _SetupSheet();
    }
    return bRtn;
  }

  wxXml2Document *TransformToDOM(wxXml2Document *pDoc)
  {
    wxXml2Document *pRtn = m_Sheet.TransformToDOM(pDoc);
    m_Sheet.ClearParms();
    return pRtn;
  }
  wxXml2Document *TransformToDOM(
    wxXml2Document *pDoc, 
    const map<wxString,wxString> *pMapParams)
  {
    _SetupSheetParams(pMapParams);
    return TransformToDOM(pDoc);
  }
  bool HasAutoTransform() const
  {
    return m_auto.IsActive();
  }
  bool AutoTransform(const wxString &sFileName);

  bool TransformToFile(
    wxXml2Document *pDoc,
    const wxString &sFileName)
  {
    bool bRtn = 
      XSLfileOK() &&
        m_Sheet.TransformToFile(sFileName,pDoc);
    m_Sheet.ClearParms();
    return bRtn;
  }
  bool TransformToFile(
    wxXml2Document *pDoc,
    const map<wxString,wxString> *pMapParams,
    const wxString &sFileName,
    const wxString &sInFileName = wxEmptyString)
  {
    _SetupSheetParams(pMapParams,sInFileName,sFileName);
    return TransformToFile(pDoc,sFileName);
  }
  bool TransformToFile(
    const wxString &sInFile,
    const wxString &sOutFile,
    const map<wxString,wxString> *pMapParams);

  virtual const wxString &RootNode(void) const
  {
    return g_sROOTNODE;
  }


  // <name> tag

  const wxString &GetKey() const
  {
    // used for TnwxXmlIOPersistMap<K,T,S>
    return m_sName;
  }
  void SetKey(const wxString &s)
  {
    // used for TnwxXmlIOPersistMap<K,T,S>
    m_sName = s;
  }
  const wxString &GetName() const
  {
    return GetKey();
  }
  void SetName(const wxString &s)
  {
    SetKey(s);
  }

  // <file-extension> tag

  const set<wxString> &GetFileExt() const
  {
    return m_ssFileExt;
  }
  wxString GetFileExtJoined(const wxChar *psSep = wxS(", ")) const
  {
    wxString s;
    nwxString::Join(m_ssFileExt,&s, (const char *)psSep);
    return s;
  }
  static wxString ParseFileExt(const wxString &s);
  static size_t ParseFileExtList(
    const wxString &s,set<wxString> *pss,
    int *pnProblem = NULL);
  bool AddFileExt(const wxString &s)
  {
    wxString sTmp = ParseFileExt(s);
    bool bRtn = (sTmp.Len() > 0);
    if(bRtn)
    {
      m_ssFileExt.insert(sTmp);
    }
    return bRtn;
  }
  size_t SetFileExt(const set<wxString> &vs, bool bCheck = true)
  {
    if(!bCheck)
    {
      m_ssFileExt = vs;
    }
    else
    {
      set<wxString>::const_iterator itr;
      m_ssFileExt.clear();
      for(itr = vs.begin(); itr != vs.end(); ++itr)
      {
        AddFileExt(*itr);
      }
    }
    return m_ssFileExt.size();
  }

  // <extension-override> tag

  void SetAllowExtOverride(bool b)
  {
    m_bOverrideExt = b;
  }
  bool AllowExtOverride() const
  {
    return m_bOverrideExt;
  }


  // <default-location> tag

  const wxString &GetDefaultLocation() const
  {
    return m_sDefaultLocation;
  }
  bool IsDefaultLocationLast() const
  {
    return IsUseLastLocation(m_sDefaultLocation,false);
  }
  bool IsDefaultLocationAnalysisFile() const
  {
    return IsUseAnalysisFileName(m_sDefaultLocation,true);
  }
  bool SetDefaultLocation(const wxString &s)
  {
    bool bRtn = wxDir::Exists(s);
    if(bRtn)
    {
      m_sDefaultLocation = s;
    }
    return bRtn;
  }
  void SetDefaultLocationAnalysisFile()
  {
    m_sDefaultLocation = USE_ANALYSIS_FILE;
  }
  void SetDefaultLocationLast()
  {
    m_sDefaultLocation = USE_LAST_LOCATION;
  }

  // <xsl-file> tag

  const wxString GetXSLfile(bool bCheck = true) const;
  const wxString &GetExePath() const
  {
    return m_sExePath;
  }
  bool UpdateExePath();
  bool SetXSLFile(const wxString &s, bool bRestoreIfInvalid = false);
  bool XSLfileOK() const
  {
    return m_Sheet.IsOK();
  }
  bool CheckXSLmodification()
  {
    bool bRtn = m_Sheet.CheckReload();
    return bRtn;
  }

  // <xsl-params> tag

  const CXSLmapParam &GetMapParam() const
  {
    return m_mapParam;
  }
  CXSLParam *FindParam(const wxString &sName)
  {
    CXSLParam *pRtn(NULL);
    CXSLmapParam::iterator itr = m_mapParam->find(sName);
    if(itr != m_mapParam->end())
    {
      pRtn = itr->second;
    }
    return pRtn;
  }
  void SetParam(const CXSLParam &Param)
  {
    m_mapParam.InsertOrReplace(Param);
  }
  const wxChar *GetDefaultParamValue(const wxString &s)
  {
    const wxChar *pRtn = m_Sheet.GetSheetParms().GetParm(s);
    return pRtn;
  }

  const wxChar *GetDefaultParamValue(const CXSLParam &Param)
  {
    return GetDefaultParamValue(Param.GetName());
  }
  const wxChar *GetDefaultParamValue(const CXSLParam *pParam)
  {
    return GetDefaultParamValue(*pParam);
  }
  int GetUserParamCount() const;

  // <auto-create>

  const CXSLAutoCreate &GetAutoCreate() const
  {
    return m_auto;
  }
  CXSLAutoCreate *GetAutoCreatePtr()
  {
    return &m_auto;
  }
private:
  static bool _ISVALUE(
    const wxString &s, const wxString &sCompare, bool bTrueIfEmpty)
  {
    bool bRtn = s.IsEmpty() ? bTrueIfEmpty : (s == sCompare);
    return bRtn;
  }

public:
  static const wxChar * const ERROR_MESSAGE;
  static const wxString OK_EXT_CHARS;
  static const wxString USE_DEFAULT;
  static const wxString USE_ANALYSIS_FILE;
  static const wxString USE_ANALYSIS_PARENT;
  static const wxString USE_LAST_LOCATION;
  static const wxString USE_OUTPUT_LOCATION;

  static bool IsUseDefault(
    const wxString &s, bool bTrueIfEmpty = false)
  {
    return _ISVALUE(s,USE_DEFAULT,bTrueIfEmpty);
  }
  static bool IsUseAnalysisFileName(
    const wxString &s, bool bTrueIfEmpty = false)
  {
    return _ISVALUE(s,USE_ANALYSIS_FILE,bTrueIfEmpty);
  }
  static bool IsUseAnalysisParent(
    const wxString &s, bool bTrueIfEmpty = false)
  {
    return _ISVALUE(s,USE_ANALYSIS_PARENT,bTrueIfEmpty);
  }
  static bool IsUseLastLocation(
    const wxString &s, bool bTrueIfEmpty = false)
  {
    return _ISVALUE(s,USE_LAST_LOCATION,bTrueIfEmpty);
  }
  static bool IsUseOutputLocation(
    const wxString &s, bool bTrueIfEmpty = false)
  {
    return _ISVALUE(s,USE_OUTPUT_LOCATION,bTrueIfEmpty);
  }

protected:
  virtual void RegisterAll(bool = false);
private:
  static const wxChar * const INFILE;
  static const wxChar * const OUTFILE;
  void _SetupSheet();
  void _SetupSheetParams(const map<wxString,wxString> *pMapParam);
  void _SetupSheetParams(const map<wxString,wxString> *pMapParam,
    const wxString &sInputFileName,
    const wxString &sOutputFileName);
  nwxXmlIOPersistSetWxString m_IOext;
  wxString m_sName; // descriptive name
  set<wxString> m_ssFileExt;
  wxString m_sDefaultLocation;
  wxString m_sXSLFile;
  wxString m_sExePath;
  wxString m_sThisPath;
  CXSLmapParam m_mapParam;
  CXSLAutoCreate m_auto;
  bool m_bOverrideExt;

  wxXslSheet m_Sheet;
  static const wxString g_sROOTNODE;
};



//*********************************************** CXSLAutoCreate
//  inline functions that require CXSLExportFileType

inline bool CXSLAutoCreate::IsAnalysisLocation() const
{
  return CXSLExportFileType::IsUseAnalysisFileName(m_sLocation,false);
}
inline void CXSLAutoCreate::SetAnalysisLocation()
{
  m_sLocation = CXSLExportFileType::USE_ANALYSIS_FILE;
}
inline void CXSLAutoCreate::SetDefaultLocation()
{
  m_sLocation = CXSLExportFileType::USE_DEFAULT;
}

inline bool CXSLAutoCreate::IsAnalysisFileName() const
{
  return CXSLExportFileType::IsUseAnalysisFileName(m_sFileName,false);
}
inline void CXSLAutoCreate::SetUseAnalysisFileName()
{
  m_sFileName = CXSLExportFileType::USE_ANALYSIS_FILE;
}

//*********************************************** CXSLExportFiles

class CExportFiles : 
  public TnwxXmlPersistMap<
    wxString,CXSLExportFileType,nwxStringLessNoCaseSort>
{
public:
  CExportFiles() : TnwxXmlPersistMap<
    wxString,CXSLExportFileType,nwxStringLessNoCaseSort>
      ("export",true), m_nBatch(0)
  {
  }
  CExportFiles(const CExportFiles &x) : TnwxXmlPersistMap<
    wxString,CXSLExportFileType,nwxStringLessNoCaseSort>
      (x),m_nBatch(0)
  {
  }
  bool CheckXSLmodification();
  bool LoadExportFile();
  bool SaveExportFile();
  bool LockExportFile(int nWait = 0);
  static const wxString &GetFileName()
  {
    return g_sFileName;
  }
  bool CreateFileIfNotExists();
  size_t GetAutoList(vector<CXSLExportFileType *> *pList);
  size_t GetItems(wxArrayString *pvs);
  size_t UpdateExePath()
  {
    size_t nRtn = 0;
    TnwxXmlPersistMap<wxString,CXSLExportFileType,nwxStringLessNoCaseSort>::iterator itr;
    for(itr = begin(); itr != end(); ++itr)
    {
      if(itr->second->UpdateExePath())
      {
        nRtn++;
      }
    }
    return nRtn;
  }
  static CExportFiles *GetGlobal()
  {
    if(g_pGlobal == NULL)
    {
      g_pGlobal = new CExportFiles();
    }
    CExportFiles *pRtn = g_pGlobal;
    
    // if previously loaded, check for modification

    if(!g_pGlobal->LoadExportFile())
    {
      pRtn = NULL;
    }
    return pRtn;
  }
  static bool IsGlobalLocked()
  {
    bool bRtn = 
      (GetGlobal() == NULL) &&
      (g_pGlobal != NULL) &&
      (g_pGlobal->IsLocked());
    return bRtn;
  }
  static void CleanupGlobal()
  {
    if(g_pGlobal != NULL)
    {
      delete g_pGlobal;
      g_pGlobal = NULL;
    }
  }
  void BeginBatch()
  {
    // prevent updating when > 0
    m_nBatch++;
  }
  void EndBatch()
  {
    // prevent updating
    m_nBatch--;
  }
#ifdef __WXDEBUG__
  static bool UnitTest();
  virtual const wxString &RootNode(void) const
  {
    return ROOTNODE;
  }

#endif
private:
  class CExportFilesGlobal
  {
  public:
    CExportFilesGlobal() {}
    virtual ~CExportFilesGlobal()
    {
      CExportFiles::CleanupGlobal();
    }
  };
  static bool _FileExists();
  static void _SetupFileName();
  int m_nBatch;
  static wxString g_sFileName;
  static const wxString ROOTNODE;
  static CExportFiles *g_pGlobal;
  static CExportFilesGlobal g_xxx;
};



#endif
