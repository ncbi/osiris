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
*  FileName: wxXslSheet.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __WX_XSL_SHEET_H__
#define __WX_XSL_SHEET_H__

#include "nwx/nwxXmlPersist.h"
#include "wxXsl/wxXslObject.h"
#include "nwx/nsstd.h"
#include <wx/stream.h>
#include <wx/string.h>
#include <wx/datetime.h>

#include <map>
#include <vector>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>

class wxXml2Document;
class wxXslSheet;

class wxXslParams : public nwxXmlPersist
{
public:
  wxXslParams() : 
    nwxXmlPersist(true), // readonly
    m_param(this),
    m_ppParms(NULL),
    m_nPtrAlloc(0)
  {
    RegisterAll(true);
  }
  wxXslParams(const wxXslParams &x) :
    nwxXmlPersist(true), // readonly
    m_param(this),
    m_ppParms(NULL),
    m_nPtrAlloc(0)
  {
    RegisterAll(true);
    (*this) = x;
  }

  virtual ~wxXslParams();
  wxXslParams &operator = (const wxXslParams &x)
  {
    Clear();
    m_mapParms = x.m_mapParms;
    return *this;
  }

  bool LoadFromStyleSheet(wxXslSheet *pSheet);
  bool SetParm(const wxString &sName, const wxString &sValue);
  bool ClearParm(const wxString &sName);
  const wxChar *GetParm(const wxString &sName) const;
  void Clear();
  const map<wxString,wxString> &GetParmsMap() const
  {
    return m_mapParms;
  }
  const char **GetParms();
  virtual void Init();
protected:
  virtual void RegisterAll(bool = false);
private:
  class wxXslSingleParam : public nwxXmlPersist
  {
    // class used to load nodes from XML
    // and update m_mapParms
  public:
    wxXslSingleParam(wxXslParams *parent) :
        m_pParent(parent)
    {
      RegisterAll(true);
    }
    virtual bool LoadFromNode(wxXmlNode *, void *pObj);
    virtual bool LoadFromNode(wxXmlNode *);
  protected:
    virtual void RegisterAll(bool = false);
  private:
    void _UpdateParent()
    {
      m_pParent->SetParm(m_sName,m_sValue);
    }
    wxXslParams *m_pParent;
    wxString m_sName;
    wxString m_sValue;
  };

  map<wxString, wxString> m_mapParms;
  wxXslSingleParam m_param;
  const char **m_ppParms;
  size_t m_nPtrAlloc;
  bool m_bParmsModified;
};

class wxXslSheet : public wxXslObject
{
public:
  wxXslSheet();
  wxXslSheet(const wxChar *psURL);
  wxXslSheet(wxInputStream &str);

  virtual ~wxXslSheet();
  bool Load();
  bool Load(const wxChar *psURL)
  {
    m_sURL = psURL;
    return Load();
  }
  bool Load(wxInputStream &str);
  bool CheckReload();
  bool IsOK() const
  {
    return (m_pSheet != NULL);
  }
  bool IsOK(bool bCheckReload)
  {
    if(bCheckReload)
    {
      CheckReload();
    }
    return IsOK();
  }
  wxXml2Document *TransformToDOM(wxXml2Document *);
  bool TransformToFile(const wxString &sFileName,wxXml2Document *);
  void ClearParms()
  {
    m_parmsUser.Clear();
  }
  void ClearSheetParms()
  {
    m_parmsSheet.Clear();
    m_bParmSheetOK = false;
  }
  bool ClearParm(const wxString &sName)
  {
    return m_parmsUser.ClearParm(sName);
  }
  bool SetParm(const wxString &sName, const wxString &sValue)
  {
    bool b = m_parmsUser.SetParm(sName,sValue);
    return b;
  }
  const wxXslParams &GetSheetParms() const
  {
    return m_parmsSheet;
  }
  const wxXslParams &GetParms() const
  {
    return m_parmsUser;
  }
  void SetParms(const wxXslParams &x)
  {
    m_parmsUser = x;
  }
  /*
  void ClearAll()
  {
    _CleanupSheet();
    ClearParms();
    m_sURL.Empty();
    m_bFileURL = false;
    m_dtFile.Set(time_t(0));
  }
  */
private:
  xmlDocPtr _Transform(wxXml2Document *pXml2);
  void _LoadSheetParms(); // STOP HERE
  void _CleanupSheet();
  wxXslParams m_parmsSheet;
  wxXslParams m_parmsUser;
  wxDateTime m_dtFile;
  wxString m_sURL;
  xsltStylesheetPtr m_pSheet;
  bool m_bParmSheetOK;
  bool m_bFileURL;
};

#endif

