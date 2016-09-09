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
*  FileName: nwxSettingsFile.h
*  Author:   Douglas Hoffman
*  Purpoase: Store name/value settings to a file
*     Used for OSIRIS site settings, in the future will be
*     used for strings file.
*
*/
#ifndef __NWX_SETTINGS_FILE__
#define __NWX_SETTINGS_FILE__

#include "nwx/nwxXmlPersistCollections.h"

class nwxSettingsFile : public nwxXmlPersist
{
public:
  nwxSettingsFile(const wxString &sTagName) : m_sTagName(sTagName)
  {
    RegisterAll(true);
  }
  nwxSettingsFile() : m_sTagName(wxT("Setting"))
  {
    RegisterAll(true);
  }
  virtual ~nwxSettingsFile()
  {
    m_io.Cleanup();
  }
private:
  class CSetting : public nwxXmlPersist
  {
  public:
    CSetting(const wxString &sKey, const wxString &sValue) :
        m_sKey(sKey),
        m_sValue(sValue)
        {}
    CSetting() {}
    virtual ~CSetting() {}
    const wxString &GetKey() const
    {
      return m_sKey;
    }
    const wxString &GetValue() const
    {
      return m_sValue;
    }
    void SetValue(const wxString &s)
    {
      m_sValue = s;
    }
  protected:
    virtual void RegisterAll(bool = false)
    {
      RegisterWxString(wxT("Name"),&m_sKey);
      RegisterWxString(wxT("Value"),&m_sValue);
    }
  private:
    wxString m_sKey;
    wxString m_sValue;
  };

  // constructor/destructor are private
  // and called from public static functions
  // GetGlobal() and CleanupGlobal()
  // defined in nwxGlobalObject.h
public:
  const wxString &GetValue(const wxString &sKey, const wxString &Default = wxEmptyString) const
  {
    const CSetting *ps = Find(sKey);
    const wxString &rtn = (ps == NULL) ? Default : ps->GetValue();
    return rtn;
  }
  int GetIntValue(const wxString &sKey, int Default = 0) const
  {
    const CSetting *ps = Find(sKey);
    int rtn = (ps == NULL) ? Default : StringTo::Int(ps->GetValue());
    return rtn;
  }
  unsigned int GetUIntValue(const wxString &sKey, unsigned int Default = 0) const
  {
    const CSetting *ps = Find(sKey);
    unsigned int rtn = (ps == NULL) ? Default : StringTo::Uint(ps->GetValue());
    return rtn;
  }
  bool GetBoolValue(const wxString &sKey, bool Default = false) const
  {
    const CSetting *ps = Find(sKey);
    bool rtn = (ps == NULL) ? Default : StringTo::Bool(ps->GetValue());
    return rtn;
  }
  double GetDoubleValue(const wxString &sKey, double Default = 0.0) const
  {
    const CSetting *ps = Find(sKey);
    double rtn = (ps == NULL) ? Default : StringTo::Double(ps->GetValue());
    return rtn;
  }

  void SetValue(const wxString &sKey, const wxString &sValue)
  {
    CSetting *ps = Find(sKey);
    if(ps == NULL)
    {
      m_map.insert(
        std::map<wxString, CSetting *>::value_type(
          sKey, new CSetting(sKey,sValue)
        ));
    }
    else
    {
      ps->SetValue(sValue);
    }
  }
  void SetIntValue(const wxString &sKey, int nValue)
  {
    SetValue(sKey,ToString::Int(nValue));
  }
  void SetUIntValue(const wxString &sKey, unsigned int nValue)
  {
    SetValue(sKey,ToString::Uint(nValue));
  }
  void SetBoolValue(const wxString &sKey, bool bValue)
  {
    SetValue(sKey,ToString::Bool(bValue));
  }
  void SetDoubleValue(const wxString &sKey, double dValue)
  {
    SetValue(sKey,ToString::Double(dValue));
  }
  bool RemoveKey(const wxString &sKey)
  {
    bool bRtn = false;
    std::map<wxString, CSetting *>::iterator itr = m_map.find(sKey);
    if(itr != m_map.end())
    {
      m_map.erase(itr);
      bRtn = true;
    }
    return bRtn;
  }

protected:
  virtual void RegisterAll(bool = false)
  {
    Register(m_sTagName,&m_io,&m_map);
  }
private:
  const CSetting *Find(const wxString &sKey) const
  {
    std::map<wxString, CSetting *>::const_iterator itr = m_map.find(sKey);
    const CSetting *pRtn = (itr == m_map.end()) ? NULL : itr->second;
    return pRtn;
  }
  CSetting *Find(const wxString &sKey)
  {
    std::map<wxString, CSetting *>::iterator itr = m_map.find(sKey);
    CSetting *pRtn = (itr == m_map.end()) ? NULL : itr->second;
    return pRtn;
  }
  const wxString m_sTagName;
  std::map<wxString, CSetting *> m_map;
  TnwxXmlIOPersistMap<wxString, CSetting> m_io;
  
};


#endif
