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
*  FileName: CXMLmessageBook.h
*  Author:   Douglas Hoffman
*     this is to be used to READ the message bool xml file
*     and extract only 'wanted' information which at this time
*     is
*  /ConditionalTextUnits/ConditionalTestUnit/MessageList/SmartMessage/
*         CountingDataInfo or PercentDataInfo
*
*/
#ifndef __CXML_MESSAGE_BOOK_H__
#define __CXML_MESSAGE_BOOK_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/nwxString.h"
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"

class CXMLmessageBookDI;
  // <CountingDataInfo> <PercentDataInfo>

class CXMLmessageBookSM : public nwxXmlPersist
{
  // <SmartMessage>
public:
  CXMLmessageBookSM();
  virtual ~CXMLmessageBookSM();
  const wxString &GetDescriptor() const
  {
    return m_sThresholdDescriptor;
  }
  const bool EmptyDescriptor() const
  {
    return m_sThresholdDescriptor.IsEmpty();
  }
  const wxString &GetName() const
  {
    return m_sMessageName;
  }
  bool IsPercent() const
  {
    return m_sMessageType == g_sMessageTypePct;
  }
  bool IsCount() const
  {
    return m_sMessageType == g_sMessageTypeCount;
  }
  bool IsPreset() const
  {
    return m_sMessageType == g_sMessageTypePreset;
  }
  int GetThreshold() const
  {
    return m_nThreshold;
  }
protected:
  virtual void RegisterAll(bool = false);
private:
  CXMLmessageBookDI *m_pDI;
  wxString m_sThresholdDescriptor;
  wxString m_sMessageName;
  wxString m_sMessageType;
  static const wxString g_sMessageTypePct;
  static const wxString g_sMessageTypeCount;
  static const wxString g_sMessageTypePreset;
  int m_nThreshold;
};



class CXMLmessageBook : public nwxXmlPersist
{
public:
  CXMLmessageBook();
  virtual ~CXMLmessageBook();
  const CXMLmessageBookSM *GetMessage(size_t ndx) const
  {
    return m_vpSM.at(ndx);
  }
  size_t GetMessageCount() const
  {
    return m_vpSM.size();
  }
  virtual bool LoadFile(const wxString &sFileName);
  bool CheckReload();
  bool IsOK() const
  {
    return m_bOK;
  }
  //const CXMLmessageBookSM *FindByName(const wxString &s);
protected:
  virtual void RegisterAll(bool = false);
private:
  void Cleanup();
  void BuildSMList();
  TnwxXmlPersistVector<CXMLmessageBookSM> m_vpSMall;
  vector<const CXMLmessageBookSM *> m_vpSM;
  bool m_bOK;
};


#endif
