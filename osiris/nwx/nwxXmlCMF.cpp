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
*  FileName: nwxXmlCMF.cpp
*  Author:   Douglas Hoffman
*
*   Class for reading/writing CODIS CMF v3.2 files
*/

#include "nwxXmlCMF.h"

#define CP(elem) elem = x.elem
#define CPV(Type,elem) vectorptr<Type>::copy(&elem,x.elem)

// field length

const size_t nwxXmlCMFAllele::LEN_ALLELE(10);
const size_t nwxXmlCMFLocus::LEN_BATCH_ID(32);
const size_t nwxXmlCMFLocus::LEN_READING_BY(20);
const size_t nwxXmlCMFSpecimen::LEN_SOURCE_ID(10);
const size_t nwxXmlCMFSpecimen::LEN_CASE_ID(32);
const size_t nwxXmlCMFSpecimen::LEN_SPECIMEN_ID(24);
const size_t nwxXmlCMFSpecimen::LEN_SPECIMEN_CATEGORY(21);
const size_t nwxXmlCMFSpecimen::LEN_SPECIMEN_COMMENT(255);
const size_t nwxXmlCMF::LEN_DEST_LAB(10);
const size_t nwxXmlCMF::LEN_SOURCE_LAB(10);
const size_t nwxXmlCMF::LEN_SUBMIT_BY(20);
const size_t nwxXmlCMF::LEN_BATCH_ID(32);


//***********************************************  nwxXmlCMFAllele


void nwxXmlCMFAllele::RegisterAll(bool)
{
  RegisterWxStringNotEmpty(wxS("ALLELEVALUE"),&m_sALLELEVALUE);
  RegisterBoolSkipFalseAttribute(wxS("ALLELEREQUIRED"),&m_bALLELEREQUIRED);
}
bool nwxXmlCMFAllele::Skip(void *p)
{
  bool bRtn = false;
  if(p == (void *) this)
  {
    bRtn = m_sALLELEVALUE.IsEmpty() && !m_bALLELEREQUIRED;
  }
#ifdef _DEBUG
  else
  {
    wxASSERT_MSG(0,"nwxXmlCMFAllele::Skip(void *p) -- p != this");
  }
#else
  else
  {
    bRtn = ((nwxXmlCMFAllele *)p)->Skip(p);
  }
#endif
  return bRtn;
}

//***********************************************  nwxXmlCMFLocus

nwxXmlCMFLocus::~nwxXmlCMFLocus()
{
  vectorptr<nwxXmlCMFAllele>::cleanup(&m_vpALLELE);
}

nwxXmlCMFLocus &nwxXmlCMFLocus::operator =(const nwxXmlCMFLocus &x)
{
  CP(m_sBATCHID);
  CP(m_sKIT);
  CP(m_sLOCUSNAME);
  CP(m_sREADINGBY);
  CP(m_dtREADINGDATETIME);
  CPV(nwxXmlCMFAllele,m_vpALLELE);
  return *this;
}
bool nwxXmlCMFLocus::Skip(void *p)
{
  bool bRtn = true;
  if(p != (void *) this)
  {
    bRtn = ((nwxXmlCMFLocus *)p)->Skip(p);
  }
  else if(m_sLOCUSNAME.IsEmpty() || m_sREADINGBY.IsEmpty())
  {
    bRtn = true;
  }
  else
  {
    size_t n = m_vpALLELE.size();
    for(size_t i = 0; i < n; ++i)
    {
      nwxXmlCMFAllele *pa(m_vpALLELE.at(i));
      if(!pa->Skip((void *)pa))
      {
        // we have at least one allele
        bRtn = false;
        i = n; // loop exit
      }
    }
  }
  return bRtn;
}



void nwxXmlCMFLocus::RegisterAll(bool)
{
  RegisterWxStringNotEmptyAttribute(wxS("BATCHID"),&m_sBATCHID);
  RegisterWxStringNotEmptyAttribute(wxS("KIT"),&m_sKIT);
  RegisterWxString(wxS("LOCUSNAME"),&m_sLOCUSNAME);
  RegisterWxString(wxS("READINGBY"),&m_sREADINGBY);
  RegisterWxDateTimeXML(wxS("READINGDATETIME"),&m_dtREADINGDATETIME);
  Register(wxS("ALLELE"),&m_ioALLELE,(void *)&m_vpALLELE);
}
bool nwxXmlCMFLocus::SetKit(const wxString &s)
{
  wxString sSave = m_sKIT;
  m_sKIT = s;
  bool bRtn = nwxXmlCMF::SetupKitName(&m_sKIT);
  if(!bRtn)
  {
    m_sKIT = sSave;
  }
  return bRtn;
}
bool nwxXmlCMFLocus::SetLocusName(const wxString &s)
{
  wxString sSave = m_sLOCUSNAME;
  m_sLOCUSNAME = s;
  bool bRtn = nwxXmlCMF::SetupLocusName(&m_sLOCUSNAME);
  if(!bRtn)
  {
    m_sLOCUSNAME = sSave;
  }
  return bRtn;
}


//***********************************************  nwxXmlCMFSpecimen

nwxXmlCMFSpecimen::~nwxXmlCMFSpecimen()
{
  vectorptr<nwxXmlCMFLocus>::cleanup(&m_vpLOCUS);
}

void nwxXmlCMFSpecimen::SetLocusReadingBy(const wxString &x)
{
  vector<nwxXmlCMFLocus *>::iterator itr;
  for(itr = m_vpLOCUS.begin();
    itr != m_vpLOCUS.end();
    ++itr)
  {
    (*itr)->SetReadingBy(x);
  }
}

nwxXmlCMFSpecimen &nwxXmlCMFSpecimen::operator = (const nwxXmlCMFSpecimen &x)
{
  CP(m_sSOURCEID);
  CP(m_sCASEID);
  CP(m_sSPECIMENID);
  CP(m_sSPECIMENCATEGORY);
  CP(m_sSPECIMENCOMMENT);
  CP(m_bPARTIAL);

  CP(m_bExclude);

  CPV(nwxXmlCMFLocus,m_vpLOCUS);
  return *this;
}

void nwxXmlCMFSpecimen::RegisterAll(bool)
{
  RegisterWxStringNotEmptyAttribute(wxS("SOURCEID"),&m_sSOURCEID);
  RegisterWxStringNotEmptyAttribute(wxS("CASEID"),&m_sCASEID);
  RegisterBoolSkipFalseAttribute(wxS("PARTIAL"),&m_bPARTIAL);

  RegisterWxString(wxS("SPECIMENID"),&m_sSPECIMENID);
  RegisterWxString(wxS("SPECIMENCATEGORY"),&m_sSPECIMENCATEGORY);
  RegisterWxStringNotEmpty(wxS("SPECIMENCOMMENT"),&m_sSPECIMENCOMMENT);
  Register(wxS("LOCUS"),&m_ioLOCUS,(void *)&m_vpLOCUS);
}

bool nwxXmlCMFSpecimen::Skip(void *p)
{
  bool bRtn = true;
  if(p != (void *) this)
  {
    bRtn = ((nwxXmlCMFSpecimen *)p)->Skip(p);
  }
  else if (
    m_bExclude ||
    m_sSPECIMENCATEGORY.IsEmpty() || 
    m_sSPECIMENID.IsEmpty()
    )
  {
    bRtn = true;
  }
  else
  {
    size_t n = m_vpLOCUS.size();
    for(size_t i = 0; i < n; i++)
    {
      nwxXmlCMFLocus *pl(m_vpLOCUS.at(i));
      if(!pl->Skip((void *)pl))
      {
        bRtn = false;
        i = n; // loop exit
      }
    }
  }
  return bRtn;
}

//***********************************************  nwxXmlCMF

const wxString nwxXmlCMF::g_sCODISImportFile(wxS("CODISImportFile"));
const wxString nwxXmlCMF::g_sXMLNSvalue(wxS("urn:CODISImportFile-schema"));
const wxString nwxXmlCMF::g_sSchemaLocationValue(wxS(
"urn:CODISImportFile-schema\nhttp://www.ncbi.nlm.nih.gov/projects/SNP/osiris/CODIS-32.Appendix-B.xsd"
));

const double nwxXmlCMF::DEFAULT_HEADER_VERSION = 3.2;
const wxString nwxXmlCMF::MESSAGE_TYPE(wxS("Import"));
nwxXmlIOdoubleFmt nwxXmlCMF::g_ioVersion(1);

nwxXmlCMF::~nwxXmlCMF()
{
  ClearAllSpecimen();
}

bool nwxXmlCMF::IsOK(vector<wxString> *pMsgs)
{
  bool bRtn = true;
  bool bSpecimen = false;
  vector<wxString> vs;
  if(pMsgs == NULL)
  {
    pMsgs = &vs;
  }
  if(m_sDESTINATIONORI.IsEmpty())
  {
    bRtn = false;
    pMsgs->push_back(wxS("Destination lab is not specified"));
  }
  if(m_sSOURCELAB.IsEmpty())
  {
    bRtn = false;
    pMsgs->push_back(wxS("Source lab is not specified"));
  }
  if(m_sSUBMITBYUSERID.IsEmpty())
  {
    bRtn = false;
    pMsgs->push_back(wxS("Submitter is not specified"));
  }
  size_t n = m_vpSPECIMEN.size();
  nwxXmlCMFSpecimen *p;
  for(size_t i = 0; i < n; i++)
  {
    p = m_vpSPECIMEN.at(i);
    if(!p->Skip((void *)p))
    {
      i = n; // loop exit
      bSpecimen = true;
    }
  }
  if(!bSpecimen)
  {
    bRtn = false;
    pMsgs->push_back(wxS("There are no specimens selected or specimen information is incomplete"));
  }
  return bRtn;
}

void nwxXmlCMF::RegisterAll(bool)
{
  RegisterWxStringNotEmptyAttribute(g_sXMLNS,&m_sXMLNS);
  RegisterWxStringNotEmptyAttribute(g_sXMLNSxsi,&m_sXMLNSxsi);
  RegisterWxStringNotEmptyAttribute(g_sXMLNSsql,&m_sXMLNSsql);
  RegisterWxStringNotEmptyAttribute(g_sXSIschemaLocation,&m_sXSISchemaLocationValue);

  Register(wxS("HEADERVERSION"),&g_ioVersion, (void *)&m_dHEADERVERSION);
  RegisterWxString(wxS("MESSAGETYPE"),&m_sMESSAGETYPE);
  RegisterWxString(wxS("DESTINATIONORI"),&m_sDESTINATIONORI);
  RegisterWxString(wxS("SOURCELAB"),&m_sSOURCELAB);
  RegisterWxString(wxS("SUBMITBYUSERID"),&m_sSUBMITBYUSERID);
  RegisterWxDateTimeXML(wxS("SUBMITDATETIME"),&m_dtSUBMITDATETIME);
  RegisterWxStringNotEmpty(wxS("BATCHID"),&m_sBATCHID);
  RegisterWxStringNotEmpty(wxS("KIT"),&m_sKIT);
  Register(wxS("SPECIMEN"),&m_ioSPECIMEN,(void *) &m_vpSPECIMEN);
}
bool nwxXmlCMF::AllExcluded()
{
  bool bEmpty = true;
  vector<nwxXmlCMFSpecimen *>::iterator itr;
  for(itr = m_vpSPECIMEN.begin();
      bEmpty && (itr != m_vpSPECIMEN.end());
      ++itr)
  {
    if(!(*itr)->Skip())
    {
      bEmpty = false;
    }
  }
  return bEmpty;
}

bool nwxXmlCMF::SaveFile(const wxString &sFileName)
{
  bool bRtn = false;
  if(!AllExcluded())
  {
    bRtn = nwxXmlPersist::SaveFile(sFileName);
  }
  return bRtn;
}

nwxXmlCMF &nwxXmlCMF::operator = (const nwxXmlCMF &x)
{
  CP(m_sXMLNSxsi);
  CP(m_sXMLNSsql);
  CP(m_sXMLNS);
  CP(m_sXSISchemaLocationValue);
  CP(m_dHEADERVERSION);
  CP(m_sMESSAGETYPE);
  CP(m_sDESTINATIONORI);
  CP(m_sSOURCELAB);
  CP(m_sSUBMITBYUSERID);
  CP(m_dtSUBMITDATETIME);
  CP(m_sBATCHID);
  CP(m_sKIT);
  CPV(nwxXmlCMFSpecimen,m_vpSPECIMEN);
  return *this;
}

void nwxXmlCMF::SetLocusReadingBy(const wxString &x)
{
  vector<nwxXmlCMFSpecimen *>::iterator itr;
  for(itr = m_vpSPECIMEN.begin();
    itr != m_vpSPECIMEN.end();
    ++itr)
  {
    (*itr)->SetLocusReadingBy(x);
  }
}

const wxString &nwxXmlCMF::RootNode() const
{
  return g_sCODISImportFile;
}

void nwxXmlCMF::_InitAttributes()
{
//  m_sXMLNSxsi = nwxXmlPersist::g_sXSIvalue;
//  m_sXMLNSsql = nwxXmlPersist::g_sSQLvalue;
  m_sXMLNS = g_sXMLNSvalue;
//  m_sXSISchemaLocationValue = g_sSchemaLocationValue;
}

map<wxString,wxString> nwxXmlCMF::g_mapKit;
map<wxString,wxString> nwxXmlCMF::g_mapLocus;

void nwxXmlCMF::_SetupSearch(wxString *ps)
{
  ps->Replace(wxS(" "),wxS(""),true);
  ps->Replace(wxS("\t"),wxS(""),true);
  ps->Replace(wxS("\r"),wxS(""),true);
  ps->Replace(wxS("\n"),wxS(""),true);
  ps->MakeLower();
}

bool nwxXmlCMF::_Find(wxString *ps, const map<wxString,wxString> &mapss)
{
  wxString s(*ps);
  bool bRtn = true;
  _SetupSearch(&s);
  map<wxString,wxString>::const_iterator itr = mapss.find(s);
  if(itr != mapss.end())
  {
    *ps = itr->second;
  }
  else
  {
    bRtn = false;
  }
  return bRtn;
}

void nwxXmlCMF::_SetupMap(const char **list, size_t SIZE, map<wxString,wxString> *pmss)
{
  size_t i;
  wxString sName;
  wxString sValue;
  pmss->clear();
  for(i = 0; i < SIZE; ++i)
  {
    sName = list[i];
    sValue = sName;
    _SetupSearch(&sName);
    pmss->insert(map<wxString,wxString>::value_type(sName,sValue));
  }
}

void nwxXmlCMF::_SetupMaps()
{
  if(!g_mapKit.size())
  {
    wxString sValue;
    wxString sName;

    static const char *listKits[] = 
    {
      "COfiler",
      "Profiler Plus",
      "Identifiler",
      "PowerPlex 16"
    };
    static const char *listLocus[] = 
    {
      "AMEL",
      "Amelogenin",
      "CSF1PO",
      "D13S317",
      "D16S539",
      "D18S51",
      "D19S433",
      "D21S11",
      "D2S1338",
      "D3S1358",
      "D5S818",
      "D7S820",
      "D8S1179",
      "FGA",
      "Penta D",
      "Penta E",
      "TH01",
      "THO1",
      "TP0X",
      "TPOX",
      "vWA"
    };

    size_t SZkit = sizeof(listKits) / sizeof(listKits[0]);
    size_t SZlocus = sizeof(listLocus) / sizeof(listLocus[0]);
    _SetupMap(listKits,SZkit,&g_mapKit);
    _SetupMap(listLocus,SZlocus,&g_mapLocus);
  }
}
bool nwxXmlCMF::SetupKitName(wxString *ps)
{
  wxString s(*ps);
  if(!g_mapKit.size())
  {
    _SetupMaps();
  }
  bool bRtn = _Find(ps,g_mapKit);
  return bRtn;
}

bool nwxXmlCMF::SetupLocusName(wxString *ps)
{
  if(!g_mapLocus.size())
  {
    _SetupMaps();
  }
  bool bRtn = _Find(ps,g_mapLocus);
  return bRtn;
}
