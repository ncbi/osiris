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
*  FileName: COARdirectoryAlerts.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_DIRECTORY_ALERTS_H__
#define __COAR_DIRECTORY_ALERTS_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"

class COARmessages;

class COARbaseLocus : public nwxXmlPersist
{
public:
  COARbaseLocus() : m_vnMessages(_T("MessageNumber"))
  {
    RegisterAll(true);
  }
  COARbaseLocus(const COARbaseLocus &x) : 
    m_vnMessages(_T("MessageNumber"))
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARbaseLocus();
  COARbaseLocus &operator = (const COARbaseLocus &x)
  {
    m_sLocusName = x.m_sLocusName;
    vector<int> *pvn = m_vnMessages.GetVectorPtr();
    const vector<int> &vnX = x.m_vnMessages.GetVector();
    *pvn = vnX;
    return *this;
  }
  const wxString &GetLocusName() const
  {
    return m_sLocusName;
  }
  size_t GetMessageCount() const
  {
    return m_vnMessages.GetVector().size();
  }
  int GetMessageNumber(size_t ndx) const
  {
    return m_vnMessages.GetVector().at(ndx);
  }
  const vector<int> *GetMessageNumbers() const
  {
    return &(m_vnMessages.GetVector());
  }
  void AppendMessagesToList(vector<int> *pvn) const;

protected:
  virtual void RegisterAll(bool = false);
private:
  wxString m_sLocusName;
  TnwxXmlPersistVectorSimple<int,nwxXmlIOint> m_vnMessages;
};

class COARdirectoryAlerts : public nwxXmlPersist
{
public:
  COARdirectoryAlerts() : m_ioBaseLocus(true)
  {
    RegisterAll(true);
  }
  COARdirectoryAlerts(const COARdirectoryAlerts &x)
    : m_ioBaseLocus(true)
  {
    RegisterAll(true);
    (*this) = x;
  }
  COARdirectoryAlerts &operator = (const COARdirectoryAlerts &x)
  {
    m_vnMessages = x.m_vnMessages;
    vectorptr<COARbaseLocus>::copy(&m_vpBaseLocus, x.m_vpBaseLocus);
    return *this;
  }
  virtual ~COARdirectoryAlerts();
  const COARbaseLocus *GetBaseLocusAlertsByLocus(
    const wxString &sLocusName) const
  {
    const COARbaseLocus *pRtn = NULL;
    if(!m_vpBaseLocus.empty())
    {
      pRtn = _GetBaseLocusAlertsByLocus(sLocusName);
    }
    return pRtn;
  }
  const vector<COARbaseLocus *> *GetBaseLociAlerts() const
  {
    return &m_vpBaseLocus;
  }
  const vector<int> *GetMessagesNumbers() const
  {
    return &m_vnMessages;
  }
  size_t GetMessageCount() const
  {
    return m_vnMessages.size();
  }
  int GetMessageNumber(size_t ndx) const
  {
    return m_vnMessages.at(ndx);
  }
  bool FindDisplayed(const COARmessages &AllMessages) const;
  bool NeedAttention(
    const COARmessages &AllMessages,
    const CLabSettings &lab, 
    int nReviewCount, int nAcceptCount) const;
  bool IsModified(
    const COARmessages &AllMessages, 
    const wxDateTime *pTime = NULL) const;
  size_t BuildMessageList(
    COARmessages *pMsgs,
    vector<wxString> *pvsLocus,
    const COARmessages &AllMessages) const;
  virtual bool Skip(void *pObj);
  void AppendMessagesToList(vector<int> *pvn) const;
  bool HasAlerts() const;
protected:
  void _ClearLocusMap()
  {
    m_mapLocus.clear();
  }
  virtual void RegisterAll(bool = false);
private:
  const COARbaseLocus *_GetBaseLocusAlertsByLocus(
    const wxString &sLocusName) const;
  vector<COARbaseLocus *> m_vpBaseLocus;
  vector<int> m_vnMessages;
  mutable map<wxString, const COARbaseLocus *> m_mapLocus;
  TnwxXmlIOPersistVector<COARbaseLocus> m_ioBaseLocus;
};

#endif
