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
*  FileName: nwxXmlCMF.h
*  Author:   Douglas Hoffman
*
*   Class for reading/writing CODIS CMF v3.2 files
*/
#ifndef __NWX_XML_CMF_H__
#define __NWX_XML_CMF_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"

//*********************************************** nwxXmlCMFAllele

class nwxXmlCMFAllele : public nwxXmlPersist
{
public:
  nwxXmlCMFAllele() : m_bALLELEREQUIRED(false)
  {
    RegisterAll(true);
  }
  nwxXmlCMFAllele(const wxString &sValue, bool bRequired = false)
  {
    RegisterAll(true);
    SetAllele(sValue);
    SetRequired(bRequired);
  }
  nwxXmlCMFAllele(const nwxXmlCMFAllele &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  nwxXmlCMFAllele &operator = (const nwxXmlCMFAllele &x)
  {
    m_sALLELEVALUE = x.m_sALLELEVALUE;
    m_bALLELEREQUIRED = x.m_bALLELEREQUIRED;
    return (*this);
  }
  nwxXmlCMFAllele &operator = (const wxString &x)
  {
    SetAllele(x);
    m_bALLELEREQUIRED = false;
    return *this;
  }

  virtual void RegisterAll(bool b = false);
  virtual bool Skip(void *p);
  operator const wxString &() const
  {
    return m_sALLELEVALUE;
  }
  const wxString &GetAlleleValue() const
  {
    return m_sALLELEVALUE;
  }
  bool IsRequired() const
  {
    return m_bALLELEREQUIRED;
  }

  void SetAllele(const wxString &x)
  {
    m_sALLELEVALUE = x;
    m_sALLELEVALUE.Truncate(LEN_ALLELE);
  }
  void SetRequired(bool b)
  {
    m_bALLELEREQUIRED = b;
  }

  static const size_t LEN_ALLELE;
private:
  bool m_bALLELEREQUIRED; // attribute
  wxString m_sALLELEVALUE;
};

//*********************************************** nwxXmlCMFLocus

class nwxXmlCMFLocus : public nwxXmlPersist
{
public:
  virtual ~nwxXmlCMFLocus();
  nwxXmlCMFLocus() : m_dtREADINGDATETIME((time_t)0)
  {
    RegisterAll(true);
  }
  nwxXmlCMFLocus(const nwxXmlCMFLocus &x)
  {
    (*this) = x;
    RegisterAll(true);
  }
  nwxXmlCMFLocus &operator = (const nwxXmlCMFLocus &x);
  virtual void RegisterAll(bool b = false);
  virtual bool Skip(void *p);
  const wxString &GetBatchID() const
  {
    return m_sBATCHID;
  }
  const wxString &GetKit() const
  {
    return m_sKIT;
  }
  const wxString &GetLocusName() const
  {
    return m_sLOCUSNAME;
  }
  const wxString &GetReadingBy() const
  {
    return m_sREADINGBY;
  }
  const wxDateTime &GetReadingDateTime() const
  {
    return m_dtREADINGDATETIME;
  }
  size_t GetAlleleCount() const
  {
    return m_vpALLELE.size();
  }
  const nwxXmlCMFAllele &GetAllele(size_t n) const
  {
    return *m_vpALLELE.at(n);
  }

  // modifications

  bool SetKit(const wxString &s);
  bool SetLocusName(const wxString &s);

  void SetBatchID(const wxString &s)
  {
    m_sBATCHID = s;
    m_sBATCHID.Truncate(LEN_BATCH_ID);
  }
  void SetReadingBy(const wxString &s) 
  {
    m_sREADINGBY = s;
    m_sREADINGBY.Truncate(LEN_READING_BY);
  }
  void SetReadingDateTime(const wxDateTime &dt) { m_dtREADINGDATETIME = dt;}
  void AddAllele(nwxXmlCMFAllele *pAllele)
  { m_vpALLELE.push_back(pAllele); }
  void AddAllele(const nwxXmlCMFAllele &Allele)
  {
    nwxXmlCMFAllele *pAllele(new nwxXmlCMFAllele(Allele));
    AddAllele(pAllele);
  }
  void AddAllele(const wxString &sName,bool bRequired = false)
  {
    m_vpALLELE.push_back(new nwxXmlCMFAllele(sName,bRequired));
  }
  vector<nwxXmlCMFAllele *> *GetAlleles() // this is sloppy
  { return &m_vpALLELE; }

  bool IsAmel()
  {
    wxString s(m_sLOCUSNAME);
    s.MakeLower();
    bool b = s.StartsWith(_T("amel"));
    return b;
  }
  static const size_t LEN_BATCH_ID;
  static const size_t LEN_READING_BY;
private:
  
  //  all changes MUST be reflected in 
  //  nwxXmlCMFLocus &operator = (const nwxXmlCMFLocus &x);
  wxString m_sBATCHID; // attribute
  wxString m_sKIT; // attribute
  wxString m_sLOCUSNAME;
  wxString m_sREADINGBY;
  wxDateTime m_dtREADINGDATETIME;
  vector<nwxXmlCMFAllele *> m_vpALLELE;

  TnwxXmlIOPersistVector<nwxXmlCMFAllele> m_ioALLELE;
};

//*********************************************** nwxXmlCMFSpecimen

class nwxXmlCMFSpecimen : public nwxXmlPersist
{
public:
  nwxXmlCMFSpecimen() : m_bPARTIAL(false), m_bExclude(false)
  {
    RegisterAll(true);
  }
  nwxXmlCMFSpecimen(const nwxXmlCMFSpecimen &x)
  {
    RegisterAll(true);
    *this = x;
  }
  virtual ~nwxXmlCMFSpecimen();
  nwxXmlCMFSpecimen &operator = (const nwxXmlCMFSpecimen &x);
  virtual void RegisterAll(bool b = false);
  virtual bool Skip(void *p);
  bool Skip()
  {
    return Skip((void *)this);
  }
  const wxString &GetSourceID() const { return m_sSOURCEID; }
  const wxString &GetCaseID() const { return m_sCASEID; }
  const bool GetPartial() const { return m_bPARTIAL; }
  const wxString &GetSpecimenID() const { return m_sSPECIMENID; }
  const wxString &GetSpecimenCategory() const { return m_sSPECIMENCATEGORY; }
  const wxString &GetSpecimenComment() const { return m_sSPECIMENCOMMENT; }
  size_t GetLocusCount() const { return m_vpLOCUS.size(); }
  const nwxXmlCMFLocus &GetLocus(size_t n) const { return *m_vpLOCUS.at(n); }

  // modifications

  static const size_t LEN_SOURCE_ID;
  static const size_t LEN_CASE_ID;
  static const size_t LEN_SPECIMEN_ID;
  static const size_t LEN_SPECIMEN_CATEGORY;
  static const size_t LEN_SPECIMEN_COMMENT;

  void SetLocusReadingBy(const wxString &x);
  void SetSourceID(const wxString &x) 
  {
    m_sSOURCEID = x;
    m_sSOURCEID.Truncate(LEN_SOURCE_ID);
  }
  void SetCaseID(const wxString &x) 
  {
    m_sCASEID = x; 
    m_sCASEID.Truncate(LEN_CASE_ID);
  }
  void SetExclude(bool b) { m_bExclude = b; }
  void SetPartial(bool b) { m_bPARTIAL = b; }
  void SetSpecimenID(const wxString &x)
  {
    m_sSPECIMENID = x; 
    m_sSPECIMENID.Truncate(LEN_SPECIMEN_ID);
  }
  void SetSpecimenCategory(const wxString &x)
  {
    m_sSPECIMENCATEGORY = x; 
    m_sSPECIMENCATEGORY.Truncate(LEN_SPECIMEN_CATEGORY);
  }
  void SetSpecimenComment(const wxString &x) 
  {
    m_sSPECIMENCOMMENT = x; 
    m_sSPECIMENCOMMENT.Truncate(LEN_SPECIMEN_COMMENT);
  }
  void AddLocus(nwxXmlCMFLocus *pLocus) { m_vpLOCUS.push_back(pLocus); }
  void AddLocus(const nwxXmlCMFLocus &Locus)
  {
    nwxXmlCMFLocus *pLocus(new nwxXmlCMFLocus(Locus));
    AddLocus(pLocus);
  }
  nwxXmlCMFLocus *AddNewLocus(const wxString &sName)
  {
    nwxXmlCMFLocus *pRtn = new nwxXmlCMFLocus;
    if(pRtn->SetLocusName(sName))
    {
      AddLocus(pRtn);
    }
    else
    {
      delete pRtn;
      pRtn = NULL;
    }
    return pRtn;
  }
  vector<nwxXmlCMFLocus *> *GetLoci()  // this is sloppy
  {
    return &m_vpLOCUS;
  }
private:
  // all modifications below MUST be reflected 
  // in nwxXmlCMFSpecimen &operator = (const nwxXmlCMFSpecimen &x))
  wxString m_sSOURCEID;  // attribute
  wxString m_sCASEID;    // attribute
  wxString m_sSPECIMENID;
  wxString m_sSPECIMENCATEGORY;
  wxString m_sSPECIMENCOMMENT;
  bool m_bPARTIAL;   // attribute
  bool m_bExclude;
  vector<nwxXmlCMFLocus *> m_vpLOCUS;
  TnwxXmlIOPersistVector<nwxXmlCMFLocus> m_ioLOCUS;
};

//*********************************************** nwxXmlCMF

class nwxXmlCMF : public nwxXmlPersist
{
public:
#ifdef _DEBUG
  static void UnitTest();
  static void UnitTest1(const nwxXmlCMF &x);
#endif
  nwxXmlCMF() 
  {
    RegisterAll(true);
    Init();
  }
  nwxXmlCMF(const nwxXmlCMF &x)
  {
    RegisterAll(true);
    *this = x;
  }
  virtual ~nwxXmlCMF();
  virtual void RegisterAll(bool b = false);
  bool AllExcluded();
  virtual bool SaveFile(const wxString &sFileName);
  nwxXmlCMF &operator = (const nwxXmlCMF &);
  virtual const wxString &RootNode(void) const;

  virtual bool IsOK(vector<wxString> *pMsgs = NULL);

  double GetHeaderVersion() const { return m_dHEADERVERSION;}
  const wxString &GetMessageType() const { return m_sMESSAGETYPE;}
  const wxString &GetDestination() const { return m_sDESTINATIONORI;}
  const wxString &GetSourceLab() const { return m_sSOURCELAB;}
  const wxString &GetSubmitByUserID() const { return m_sSUBMITBYUSERID;}
  const wxDateTime &GetSubmitDateTime() const { return m_dtSUBMITDATETIME;}
  const wxString &GetBatchId() const { return m_sBATCHID; }
  const wxString &GetKit() const { return m_sKIT; }

  // modifications
private:
  // these parameters are private because only one value is supported
  void SetHeaderVersion(double x = nwxXmlCMF::DEFAULT_HEADER_VERSION)
  { m_dHEADERVERSION = x; }
  void SetMessageType(const wxString &x = nwxXmlCMF::MESSAGE_TYPE)
  { m_sMESSAGETYPE = x; }
public:

  static const size_t LEN_DEST_LAB;
  static const size_t LEN_SOURCE_LAB;
  static const size_t LEN_SUBMIT_BY;
  static const size_t LEN_BATCH_ID;

  void SetLocusReadingBy(const wxString &x);

  void SetDestination(const wxString &x)
  {
    m_sDESTINATIONORI = x;
    m_sDESTINATIONORI.Truncate(LEN_DEST_LAB);
  }
  void SetSourceLab(const wxString &x)
  {
    m_sSOURCELAB = x;
    m_sSOURCELAB.Truncate(LEN_SOURCE_LAB);
  }
  void SetSubmitByUserID(const wxString &x, bool bAllLoci = false)
  {
    m_sSUBMITBYUSERID = x;
    m_sSUBMITBYUSERID.Truncate(LEN_SUBMIT_BY);
    if(bAllLoci)
    {
      SetLocusReadingBy(x);
    }
  }
  void SetSubmitDateTimeCurrent()
  {
    m_dtSUBMITDATETIME.SetToCurrent();
  }
  void SetSubmitDateTime(const wxDateTime &x)
  { m_dtSUBMITDATETIME = x; }
  void SetBatchId(const wxString &x)
  {
    m_sBATCHID = x;
    m_sBATCHID.Truncate(LEN_BATCH_ID);
  }
  bool SetKit(const wxString &x)
  {
    m_sKIT = x;
    return SetupKitName(&m_sKIT);
  }

  void ClearAllSpecimen()
  {
    vectorptr<nwxXmlCMFSpecimen>::cleanup(&m_vpSPECIMEN);
  }
  void AddSpecimen(nwxXmlCMFSpecimen *p)
  { m_vpSPECIMEN.push_back(p); }
  void AddSpecimen(const nwxXmlCMFSpecimen &s)
  {
    nwxXmlCMFSpecimen *p(new nwxXmlCMFSpecimen(s));
    AddSpecimen(p);
  }
  nwxXmlCMFSpecimen *AddNewSpecimen(const wxString &sName)
  {
    nwxXmlCMFSpecimen *pRtn = new nwxXmlCMFSpecimen();
    pRtn->SetSpecimenID(sName);
    AddSpecimen(pRtn);
    return pRtn;
  }

  size_t GetSpecimenCount() const { return m_vpSPECIMEN.size();}
  const nwxXmlCMFSpecimen &GetSpecimen(size_t n) const
  {
    return *m_vpSPECIMEN.at(n);
  }
  nwxXmlCMFSpecimen *GetSpecimenPtr(size_t n)
  {
    return m_vpSPECIMEN.at(n);
  }
  vector<nwxXmlCMFSpecimen *> *GetSpecimen()  // sloppy
  { return &m_vpSPECIMEN; }

  virtual void Init()
  {
    nwxXmlPersist::Init();
    _InitAttributes();
    SetHeaderVersion();
    SetMessageType();
    SetSubmitDateTimeCurrent();
  }
  virtual void Init(void *p)
  {
    ((nwxXmlCMF *)p)->Init();
  }
  static const wxString MESSAGE_TYPE;
  static const double DEFAULT_HEADER_VERSION;

  static bool SetupKitName(wxString *ps);
  static bool SetupLocusName(wxString *ps);
protected:
  static const wxString g_sCODISImportFile;
  static const wxString g_sSchemaLocationValue;
  static const wxString g_sXMLNSvalue;
  static nwxXmlIOdoubleFmt g_ioVersion;
private:
  void _InitAttributes();
  // changes to the following must be reflected in  
  //  nwxXmlCMF &operator = (const nwxXmlCMF &);

  // attributes

  wxString m_sXMLNSxsi;
  wxString m_sXMLNSsql;
  wxString m_sXMLNS;
  wxString m_sXSISchemaLocationValue;

  // XML elements

  double m_dHEADERVERSION;
  wxString m_sMESSAGETYPE;
  wxString m_sDESTINATIONORI;
  wxString m_sSOURCELAB;
  wxString m_sSUBMITBYUSERID;
  wxDateTime m_dtSUBMITDATETIME;
  wxString m_sBATCHID;
  wxString m_sKIT;
  vector<nwxXmlCMFSpecimen *> m_vpSPECIMEN;

  TnwxXmlIOPersistVector<nwxXmlCMFSpecimen> m_ioSPECIMEN;

  // static data and static functions
  // for setting up kit and locus names compliant 
  // with the CMF 3.2 schema
  
  // g_mapKit, g_mapLocus
  //  Used to find the proper name from a name that may not
  //  have the expected upper/lower case letters
  static map<wxString,wxString> g_mapKit;
  static map<wxString,wxString> g_mapLocus;
  static void _SetupMaps();
  static void _SetupMap(const char **list, size_t SIZE, map<wxString,wxString> *pmss);
  static void _SetupSearch(wxString *ps);
  static bool _Find(wxString *ps,const map<wxString,wxString> &mapss);
};

#endif
