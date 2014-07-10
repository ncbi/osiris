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
*  FileName: CXMLmessageBook.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include "CXMLmessageBook.h"
#include "ConfigDir.h"
#include "nwx/vectorptr.h"
//***********************************************  CXMLmessageBookDataInfo

const wxString CXMLmessageBookSM::g_sMessageTypeCount("count");
const wxString CXMLmessageBookSM::g_sMessageTypePct("percent");
const wxString CXMLmessageBookSM::g_sMessageTypePreset("preset");

class CXMLmessageBookDI : public nwxXmlPersist
{
public:
  CXMLmessageBookDI(
    wxString *psThresholdDescriptor, int *pnThreshold)
    : m_psThresholdDescriptor(psThresholdDescriptor),
      m_pnThreshold(pnThreshold)
  {
    RegisterAll(true);
  }
  virtual ~CXMLmessageBookDI();
protected:
  void RegisterAll(bool = false);
private:
  wxString *m_psThresholdDescriptor;
  int *m_pnThreshold;
};
CXMLmessageBookDI::~CXMLmessageBookDI() {}

void CXMLmessageBookDI::RegisterAll(bool)
{
  RegisterIntPositive("Threshold",m_pnThreshold);
  RegisterWxString("ThresholdDescriptor",m_psThresholdDescriptor);
}

//***********************************************  CXMLmessageBookSM

CXMLmessageBookSM::CXMLmessageBookSM()
{
  m_pDI = new CXMLmessageBookDI(&m_sThresholdDescriptor, &m_nThreshold );
  RegisterAll(true);
}
CXMLmessageBookSM::~CXMLmessageBookSM() 
{
  delete m_pDI;
}

void CXMLmessageBookSM::RegisterAll(bool)
{
  RegisterWxString("MessageType",&m_sMessageType);
  RegisterWxString("Name",&m_sMessageName);
  Register("CountingDataInfo",m_pDI);
  Register("PercentDataInfo",m_pDI);
  Register("PresetDataInfo",m_pDI);

}

//***********************************************  CXMLmessageBook

CXMLmessageBook::CXMLmessageBook() : m_vpSMall("SmartMessage")
{
  m_bOK = false;
  RegisterAll(true);
};

CXMLmessageBook::~CXMLmessageBook()
{
  Cleanup();
}
void CXMLmessageBook::RegisterAll(bool)
{
  Register("MessageDeclarations",&m_vpSMall);
}
void CXMLmessageBook::Cleanup()
{
  m_vpSM.clear();
  m_vpSMall.Clear();
}
bool CXMLmessageBook::LoadFile(const wxString &sFileName)
{
  Cleanup();
  m_bOK = nwxXmlPersist::LoadFile(sFileName);
  if(m_bOK)
  {
    BuildSMList();
  }
  return m_bOK;
}
void CXMLmessageBook::BuildSMList()
{
  vector<CXMLmessageBookSM *>::const_iterator itrm;
  m_vpSM.clear();
  m_vpSM.reserve(24);
  for(itrm = m_vpSMall->begin(); itrm != m_vpSMall->end(); ++itrm)
  {
    if(!(*itrm)->GetDescriptor().IsEmpty())
    {
      m_vpSM.push_back(*itrm);
    }
  }
}
bool CXMLmessageBook::CheckReload()
{
  wxString sLastFile = GetLastFileName();
  if(sLastFile.IsEmpty())
  {
    m_bOK = false;
  }
  else if(CheckFileModification(false))
  {
     m_bOK = LoadFile(sLastFile);
  }
  if(!m_bOK)
  {
    Cleanup();
  }
  return m_bOK;
}

