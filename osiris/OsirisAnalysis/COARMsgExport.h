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
*  FileName: COARMsgExport.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_MSG_EXPORT_H__
#define __COAR_MSG_EXPORT_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"

class COARmsgTextLoc : public nwxXmlPersist
{
public:
  COARmsgTextLoc()
  {
    RegisterAll(true);
  }
  COARmsgTextLoc(const COARmsgTextLoc &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARmsgTextLoc() {;}
  COARmsgTextLoc &operator = (const COARmsgTextLoc &x)
  {
    m_sText = x.m_sText;
    m_sLocation = x.m_sLocation;
    return *this;
  }
  bool operator == (const COARmsgTextLoc &x) const
  {
    return (m_sText == x.m_sText) &&
      (m_sLocation == x.m_sLocation);
  }
  bool operator != (const COARmsgTextLoc &x) const
  {
    return !((*this) == x);
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("Text",&m_sText);
    RegisterWxString("Location",&m_sLocation);
  }
private:
  wxString m_sText;
  wxString m_sLocation;
};


class COARmsgExportProtocol : public nwxXmlPersist
{
public:
  COARmsgExportProtocol()
  {
    RegisterAll(true);
  }
  COARmsgExportProtocol(const COARmsgExportProtocol &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARmsgExportProtocol()
  {
    vectorptr<COARmsgTextLoc>::cleanup(&m_vpExportProtocolType);
  }
  const wxString &GetGroup() const
  {
    return m_sGroup;
  }
  COARmsgExportProtocol &operator = (const COARmsgExportProtocol &x)
  {
    COARcopy(m_sProtocolName);
    COARcopy(m_sLevel);
    COARcopy(m_sGroup);
    COARcopy(m_nPriority);
    COARcopy(m_sNotification);
    COARcopy(m_bDoNotExport);
    COARcopyVP(COARmsgTextLoc,m_vpExportProtocolType);
    return *this;
  }
  bool operator == (const COARmsgExportProtocol &x) const
  {
    bool bRtn = true;
#define CMPXX(f) if(f != x.f) { bRtn = false; }
    CMPXX(m_sProtocolName)
    else CMPXX(m_sLevel)
    else CMPXX(m_sGroup)
    else CMPXX(m_nPriority)
    else CMPXX(m_bDoNotExport)
    else CMPXX(m_sNotification)
    else if(!vectorptr<COARmsgTextLoc>::IsEqual(
      m_vpExportProtocolType,x.m_vpExportProtocolType))
    {
      bRtn = false;
    }
#undef CMPXX
    return bRtn;
  }
  bool operator != (const COARmsgExportProtocol &x) const
  {
    return !((*this) == x);
  }
private:
  TnwxXmlIOPersistVector<COARmsgTextLoc> m_ioType;
  vector<COARmsgTextLoc *> m_vpExportProtocolType;
  wxString m_sProtocolName;
  wxString m_sLevel;
  wxString m_sGroup;
  wxString m_sNotification;
  int m_nPriority;
  bool m_bDoNotExport;
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("ProtocolName",&m_sProtocolName);
    RegisterWxString("Level",&m_sLevel);
    RegisterIntNonZero("Priority",&m_nPriority);
    Register("TextLoc",&m_ioType,(void *)&m_vpExportProtocolType);
    RegisterWxStringNotEmpty("Group",&m_sGroup);
    RegisterBoolSkipFalse("DoNotExport",&m_bDoNotExport);
    RegisterWxStringNotEmpty("Notification",&m_sNotification);
  }
};

class COARmsgExport : public nwxXmlPersist
{
public:
  COARmsgExport()
  {
    RegisterAll(true);
  }
  COARmsgExport(const COARmsgExport &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARmsgExport()
  {
    vectorptr<COARmsgExportProtocol>::cleanup(&m_vpExportProtocol);
  }
  COARmsgExport operator = (const COARmsgExport &x)
  {
    m_sMsgName = x.m_sMsgName;
    vectorptr<COARmsgExportProtocol>::copy(
      &m_vpExportProtocol,x.m_vpExportProtocol);
    return *this;
  }
  bool operator == (const COARmsgExport &x) const
  {
    bool bRtn = true;
    if(m_sMsgName != x.m_sMsgName)
    {
      bRtn = false;
    }
    else if(!vectorptr<COARmsgExportProtocol>::IsEqual(
      m_vpExportProtocol,x.m_vpExportProtocol))
    {
      bRtn = false;
    }
    return bRtn;
  }
  bool operator != (const COARmsgExport &x)
  {
    return !((*this) == x);
  }
  void SetKey(const wxString &s)
  {
    m_sMsgName = s;
  }
  const wxString &GetKey() const
  {
    return m_sMsgName;
  }
  const vector<COARmsgExportProtocol *> *GetProtocols() const
  {
    return &m_vpExportProtocol;
  }
private:
  wxString m_sMsgName;
  TnwxXmlIOPersistVector<COARmsgExportProtocol> m_ioProtocol;
  vector<COARmsgExportProtocol *> m_vpExportProtocol;
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("MsgName",&m_sMsgName);
    Register(
      "Protocol",
      &m_ioProtocol,
      (void *)&m_vpExportProtocol);
  }
};

class COARmsgExportMap : 
  public TnwxXmlPersistMap<wxString,COARmsgExport>
{
public:
  COARmsgExportMap() : 
    TnwxXmlPersistMap<wxString,COARmsgExport>("MsgExport")
  {}
  virtual ~COARmsgExportMap()
  {}
  COARmsgExportMap &operator = (const COARmsgExportMap &x)
  {
    Copy(x);
    return *this;
  }
  size_t GetGroupsByMsgName(
    const wxString &sMsgName, set<wxString> *psetGroups) const
  {
    psetGroups->clear();
    const COARmsgExport *p = Find(sMsgName);
    if(p != NULL)
    {
      const vector<COARmsgExportProtocol *> *pvProto = 
        p->GetProtocols();
      vector<COARmsgExportProtocol *>::const_iterator itrp;
      for(itrp = pvProto->begin();
        itrp != pvProto->end();
        ++itrp)
      {
        const wxString &sGroup((*itrp)->GetGroup());
        if(!sGroup.IsEmpty())
        {
          psetGroups->insert(sGroup);
        }
      }
    }
    return psetGroups->size();
  }
  bool HasGroupByMsgName(
    const wxString &sMsgName, const wxString &sGroup) const
  {
    set<wxString> ss;
    bool bRtn = false;
    if(GetGroupsByMsgName(sMsgName,&ss))
    {
      bRtn = (ss.find(sGroup) != ss.end());
    }
    return bRtn;
  }
  bool HasGroupByMsgName(
    const wxString &sMsgName, const set<wxString> &ssGroup) const
  {
    set<wxString> ss;
    bool bRtn = false;
    if(GetGroupsByMsgName(sMsgName,&ss))
    {
      set<wxString>::iterator itr;
      for(itr = ss.begin(); 
          (!bRtn) && (itr != ss.end());
          ++itr)
      {
        if(ssGroup.find(*itr) != ssGroup.end())
        {
          bRtn = true;
        }
      }
    }
    return bRtn;
  }
};


#endif
