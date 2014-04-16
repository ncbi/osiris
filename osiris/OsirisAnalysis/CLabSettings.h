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
*  FileName: CLabSettings.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_LAB_SETTINGS_H__
#define __C_LAB_SETTINGS_H__

#include "mainApp.h"
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include <wx/string.h>
#include <wx/utils.h>
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/nsstd.h"
#include "nwx/CIncrementer.h"
#include "nwx/nwxString.h"
#include "nwx/nwxRound.h"

#define LABCP(z) z = x.z
#define LABEQ(z) (z == x.z)
#define LABEQD(z) (nwxRound::Round(z * 10000) == nwxRound::Round(x.z * 10000))


#ifdef __WXDEBUG__
#define LAB_LOG_INEQ 1
#endif

#ifdef LAB_LOG_INEQ
#define LAB_LOG(xx) (mainApp::LogMessage(_T(xx)))
#else
#define LAB_LOG(x)
#endif

#define LABEQLOG(z,xx) if (!LABEQ(z)) { LAB_LOG(xx); }
#define LABEQDLOG(z,xx) if (!LABEQD(z)) { LAB_LOG(xx); }


class wxArrayString;


//*******************************************************************
//
//  CLabSettingsInfo
//

class CLabSettingsInfo : public nwxXmlPersist
{
public:
  CLabSettingsInfo()
  {
    RegisterAll(true);
  }
  CLabSettingsInfo(const CLabSettingsInfo &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CLabSettingsInfo();

  const wxString &GetVolumeName() const
  {
    return m_sVolumeName;
  }
  const wxString &GetUserID() const
  {
    return m_sUserId;
  }
  const wxString &GetVersion() const
  {
    return m_sVersion;
  }
  const wxString &GetProtocol() const
  {
    return m_sProtocol;
  }
  const wxString &GetLot() const
  {
    return m_sLot;
  }
  const wxString &GetNotes() const
  {
    return m_sNotes;
  }
  const wxString &GetDataFileType() const
  {
    return m_sDataFileType;
  }
  const wxDateTime &GetLastUpdate() const
  {
    return m_dtLastUpdate;
  }
  bool SetDataFileType(const wxString &s)
  {
    return _SET(&m_sDataFileType,s);
  }
  bool SetVolumeName(const wxString &s)
  {
    return _SET(&m_sVolumeName,s);
  }
  bool SetVersion(const wxString &s)
  {
    return _SET(&m_sVersion,s);
  }
  bool SetProtocol(const wxString &s)
  {
    return _SET(&m_sProtocol,s);
  }
  bool SetLot(const wxString &s)
  {
    return _SET(&m_sLot,s);
  }
  bool SetNotes(const wxString &s)
  {
    return _SET(&m_sNotes,s);
  }
  void UpdateTime()
  {
    m_dtLastUpdate.SetToCurrent();
  }
  bool SetUserID(const wxString &_s = wxEmptyString)
  {
    wxString sUser(_s);
    if(sUser.IsEmpty())
    {
      sUser = ::wxGetUserId(); 
      if(sUser.IsEmpty())
      {
        sUser = _T("Unknown user");
      }
    }
    return _SET(&m_sUserId,sUser);
  }

  CLabSettingsInfo &operator = (const CLabSettingsInfo &x)
  {
    LABCP(m_sVolumeName);
    LABCP(m_sVersion);
    LABCP(m_sDataFileType);
    LABCP(m_sProtocol);
    LABCP(m_sLot);
    LABCP(m_sNotes);
    LABCP(m_sUserId);
    LABCP(m_dtLastUpdate);
    return *this;
  }
  bool operator == (const CLabSettingsInfo &x) const;
protected:
  virtual void RegisterAll(bool = false);
private:
  wxString m_sVolumeName;
  wxString m_sVersion;
  wxString m_sProtocol;
  wxString m_sLot;
  wxString m_sNotes;
  wxString m_sUserId;
  wxString m_sDataFileType;
  wxDateTime m_dtLastUpdate;

  static bool StringEQwithDefault(const wxString &s1, const wxString &s2, const wxString &sDefault)
  {
    const wxString &ss1(s1.IsEmpty() ? sDefault : s1);
    const wxString &ss2(s2.IsEmpty() ? sDefault : s2);
    return !ss1.Cmp(ss2);
  }

  bool _SET(bool *pb, bool b)
  {
    bool bRtn = false;
    if(b != *pb)
    {
      *pb = b;
      bRtn = true;
    }
    return bRtn;
  }
  bool _SET(wxString *ps, const wxString &psValue)
  {
    bool bRtn = false;
    if(psValue != (*ps))
    {
      *ps = psValue;
      bRtn = true;
    }
    return bRtn;
  }
};

class CAlleleLess
{
public:
  CAlleleLess() {}
  bool cmp(const wxString &a1, const wxString &a2)
  {
    bool bRtn = false;
    if(
      (!a1.IsEmpty()) && (!a2.IsEmpty()) &&
      (a1.GetChar(0) < 'A') && (a2.GetChar(0) < 'A')
      )
    {
      // numeric
      double d1 = atof(a1.c_str());
      double d2 = atof(a2.c_str());
      bRtn = (d1 < d2);
    }
    else
    {
      bRtn = (a1.Cmp(a2) < 0);
    }
    return bRtn;
  }
  bool operator()(const wxString &a1, const wxString &a2)
  {
    return cmp(a1,a2);
  }
  bool operator()(const wxString *pa1, const wxString *pa2)
  {
    return cmp(*pa1,*pa2);
  }

};

//*******************************************************************
//
//  CLabLocus
//

class CLabLocus : public nwxXmlPersist
{
public:
  CLabLocus() : m_ioAllele()
  {
    RegisterAll(true);
  }
  CLabLocus(const CLabLocus &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CLabLocus() {;}
  CLabLocus &operator =(const CLabLocus &x)
  {
    LABCP(m_sName);
    LABCP(m_vsAlleles);
    return *this;
  }
  bool operator ==(const CLabLocus &x) const
  {
    bool bRtn = false;
    LABEQLOG(m_sName,"CLabLocus.m_sName")
    else LABEQLOG(m_vsAlleles,"CLabLocus.m_vsAlleles")
    else
    {
      bRtn = true;
    }
    return bRtn;
  }
  size_t GetAlleleCount()
  {
    return m_vsAlleles.size();
  }
  const wxString &GetAllele(int ndx = 0) const
  {
    return m_vsAlleles.at(ndx);
  }
  void ClearAlleles()
  {
    m_vsAlleles.clear();
  }
  void AddAllele(const wxString &sAllele)
  {
    m_vsAlleles.push_back(sAllele);
  }
  vector<wxString> *GetAlleles()
  {
    return &m_vsAlleles;
  }
  void SetAlleles(const vector<wxString> &vs)
  {
    m_vsAlleles = vs;
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  const void SetName(const wxString &x)
  {
    m_sName = x;
  }
  static size_t BuildList(
    const wxString &s, vector<wxString> *pvs, bool bAmel = false);
  static bool ValidateAllele(wxString *ps, bool bAmel);
  static double AlleleNumber(const wxString &s);
  wxString FormatAlleleList() const
  {
    return FormatAlleleList(m_vsAlleles);
  }
  static wxString FormatAlleleList(const vector<wxString> &vs)
  {
    wxString sRtn;
    nwxString::Join(vs,&sRtn,_T(", "));
    return sRtn;
  }
  void SortAlleles();
  virtual bool LoadFromNode(wxXmlNode *pNode);
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj);
  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName);
  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName, void *pObj);

  static bool IsAmel(const wxString &_sName)
  {
    wxString sName(_sName);
    sName.MakeUpper();
    sName.Trim(true);
    sName.Trim(false);
    return sName.StartsWith(_T("AMEL"));
  }
  bool IsAmel()
  {
    return IsAmel(m_sName);
  }
protected:
  void ChangeDataToDisplay();
  void ChangeDisplayToData();

  virtual void RegisterAll(bool = false);
private:
  nwxXmlIOPersistVectorWxString m_ioAllele;
  vector<wxString> m_vsAlleles;
  wxString m_sName;
};
class CLabLocusCollection : public TnwxXmlPersistVector<CLabLocus>
{
public:
  CLabLocusCollection() : TnwxXmlPersistVector<CLabLocus>(_T("Locus")) {;}
  virtual ~CLabLocusCollection()
  {
    ClearIncompleteData();
    Clear();
  }
  bool operator == (const CLabLocusCollection &x) const
  {
    bool bRtn = IsEqual(x);
    return bRtn;
  }
  void ClearIncompleteData();
};

//************************************************************
//
//  CLabPositiveControl
//
class CLabPositiveControl : public nwxXmlPersist
{
public:
  CLabPositiveControl()
  {
    RegisterAll(true);
  }
  CLabPositiveControl(const CLabPositiveControl &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CLabPositiveControl()
  {
    ClearIncompleteData();
  }

  CLabPositiveControl &operator = (const CLabPositiveControl &x)
  {
    LABCP(m_sSynonym);
    LABCP(m_sFileNameString);
    LABCP(m_Loci);
    return *this;
  }
  bool operator == (const CLabPositiveControl &x) const
  {
    bool bRtn = false;
    LABEQLOG(m_sSynonym,"CLabPositiveControl.m_sSynonym")
    else LABEQLOG(m_sFileNameString,"CLabPositiveControl.m_sFileNameString")
    else LABEQLOG(m_Loci,"CLabPositiveControl.m_Loci")
    else
    {
      bRtn = true;
    }
    return bRtn;
  }
  size_t Size() const
  {
    return m_Loci.Size();
  }
  const CLabLocus &GetLocus(int ndx = 0) const
  {
    CLabLocus *pRtn = m_Loci.at(ndx);
    return *pRtn;
  }
  vector<CLabLocus *> *operator ->()
  {
    return m_Loci.Get();
  }
  vector<CLabLocus *> *Get()
  {
    return m_Loci.Get();
  }
  const wxString &GetSynonym() const
  {
    return m_sSynonym;
  }
  const wxString &GetFileNameString() const
  {
    return m_sFileNameString;
  }
  const wxString &GetName() const
  {
    return GetSynonym();
  }
  void SetSynonym(const wxString &x)
  {
    m_sSynonym = x;
  }
  void SetFileNameString(const wxString &x)
  {
    m_sFileNameString = x;
  }
  void SetName(const wxString &x)
  {
    SetSynonym(x);
  }
  CLabLocusCollection *GetLoci()
  {
    return &m_Loci;
  }
  void ClearIncompleteData()
  {
    m_Loci.ClearIncompleteData();
  }
  
protected:
  virtual void RegisterAll(bool = false);

private:
  CLabLocusCollection m_Loci;
  wxString m_sSynonym;
  wxString m_sFileNameString;
};

class CLabPositiveControls : public TnwxXmlPersistVector<CLabPositiveControl>
{
public:
  CLabPositiveControls() : TnwxXmlPersistVector<CLabPositiveControl>(_T("PositiveControl")) {;}
  virtual ~CLabPositiveControls()
  {
    ClearIncompleteData();
    Clear();
  }
  bool operator == (const CLabPositiveControls &x) const
  {
    bool bRtn = IsEqual(x);
    return bRtn;
  }
  void SetPositiveControls(vector<CLabPositiveControl *> *pvp)
  {
    // use pointers from pvp and clear pvp
    Clear();
    vector<CLabPositiveControl *> *pThis = Get();
    // copy pointers
    (*pThis) = (*pvp);
    // clear incoming array to prevent reuse
    pvp->clear();
  }
  bool Rename(const wxString &sOld, const wxString &sNew);
  void ClearIncompleteData();
};

//************************************************************
//
//  CLabNegativeCtrlName
//
class CLabNegativeCtrlName : public nwxXmlPersist
{
public:
  CLabNegativeCtrlName()
  {
    RegisterAll(true);
  }
  CLabNegativeCtrlName(const CLabNegativeCtrlName &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  CLabNegativeCtrlName &operator = (const CLabNegativeCtrlName &x)
  {
    LABCP(m_sName);
    LABCP(m_sFileNameString);
    return *this;
  }
  bool operator == (const CLabNegativeCtrlName &x) const
  {
    bool bRtn = false;
    LABEQLOG(m_sName,"CLabNegativeCtrlName.m_sName")
    else LABEQLOG(m_sFileNameString,"CLabNegativeCtrlName.m_sFileNameString")
    else
    {
      bRtn = true;
    }
    return bRtn;
  }
  bool Less(const CLabNegativeCtrlName &x) const
  {
    int n = nwxString::CompareSort(GetName(),x.GetName());
    if(!n)
    {
      n = nwxString::Compare(
        GetFileNameString(),x.GetFileNameString());
    }
    return n < 0;
  }
  bool operator < (const CLabNegativeCtrlName &x) const
  {
    bool bRtn = this->Less(x);
    return bRtn;
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  const wxString &GetFileNameString() const
  {
    return m_sFileNameString;
  }
  void SetName(const wxString &sName)
  {
    m_sName = sName;
    nwxString::Trim(&m_sName);
  }
  void SetFileNameString(const wxString &s)
  {
    m_sFileNameString = s;
    nwxString::Trim(&m_sFileNameString);
  }
  bool IsOK()
  {
    bool bRtn = !m_sName.IsEmpty();
    return bRtn;
  }
  virtual bool Skip(void *p)
  {
    CLabNegativeCtrlName *pobj = (CLabNegativeCtrlName *)p;
    bool bRtn = !pobj->IsOK();
    return bRtn;
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(_T("Name"),&m_sName);
    RegisterWxString(_T("FileNameString"),&m_sFileNameString);
  }
private:
  wxString m_sName;
  wxString m_sFileNameString;
};

class LessNegativeCtrlName
{
public:
  LessNegativeCtrlName() {}
  bool operator()(const CLabNegativeCtrlName &x1, const CLabNegativeCtrlName &x2) const
  {
    return x1.Less(x2);
  }
  bool operator()(const CLabNegativeCtrlName *px1, const CLabNegativeCtrlName *px2) const
  {
    return px1->Less(*px2);
  }
};

class CLabNegativeCtrlNameSet :
  public TnwxXmlPersistSet<CLabNegativeCtrlName,LessNegativeCtrlName>
{
public:
  static const wxChar *g_psNodeName;
  CLabNegativeCtrlNameSet() :
    TnwxXmlPersistSet<CLabNegativeCtrlName,LessNegativeCtrlName>(
      g_psNodeName,false)
  {
  }
  CLabNegativeCtrlNameSet(const CLabNegativeCtrlNameSet &x) :
    TnwxXmlPersistSet<CLabNegativeCtrlName,LessNegativeCtrlName>(
      g_psNodeName,false)
  {
    *this = x;
  }
  CLabNegativeCtrlNameSet &operator = (
    const CLabNegativeCtrlNameSet &x)
  {
    setptr<CLabNegativeCtrlName,LessNegativeCtrlName>::copy(
      Get(),*x.Get());
    return *this;
  }
  bool operator == (
    const CLabNegativeCtrlNameSet &x) const
  {
    bool bRtn = IsEqual(x);
    return bRtn;
  }
};

//************************************************************ level 3
//
//  CLabMarkerSetCollection
//
class CLabMarkerSetCollection : public nwxXmlPersist
{
public:
  CLabMarkerSetCollection()
  {
    RegisterAll(true);
  }
  CLabMarkerSetCollection(const CLabMarkerSetCollection &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CLabMarkerSetCollection()
  {
    ClearIncompleteData();
  }
  CLabMarkerSetCollection &operator = (const CLabMarkerSetCollection &x)
  {
    LABCP(m_sMarkerSetName);
    LABCP(m_sILSName);
    LABCP(m_bAllowUserOverrideILS);
    LABCP(m_bAnalyzed);
    LABCP(m_lociOffLadder);
    LABCP(m_lociSampleTrialleles);
    LABCP(m_lociPosCtrlTrialleles);
    LABCP(m_PositiveControls);
    LABCP(m_NegativeControls);
    return *this;
  }
  bool operator == (const CLabMarkerSetCollection &x) const
  {
    bool bRtn = false;
    LABEQLOG(m_sMarkerSetName,"CLabMarkerSetCollection.m_sMarkerSetName")
    else LABEQLOG(m_sILSName,"CLabMarkerSetCollection.m_sILSName")
    else LABEQLOG(m_bAllowUserOverrideILS,"CLabMarkerSetCollection.m_bAllowUserOverrideILS")
    else LABEQLOG(m_bAnalyzed,"CLabMarkerSetCollection.m_bAnalyzed")
    else LABEQLOG(m_lociOffLadder,"CLabMarkerSetCollection.m_lociOffLadder")
    else LABEQLOG(m_lociSampleTrialleles,"CLabMarkerSetCollection.m_lociSampleTrialleles")
    else LABEQLOG(m_lociPosCtrlTrialleles,"CLabMarkerSetCollection.m_lociPosCtrlTrialleles")
    else LABEQLOG(m_PositiveControls,"CLabMarkerSetCollection.m_PositiveControls")
    else LABEQLOG(m_NegativeControls,"CLabMarkerSetCollection.m_NegativeControls")
    else
    {
      bRtn = true;
    }
    return bRtn;
  }

  const wxString &GetMarkerSetName() const
  {
    return m_sMarkerSetName;
  }
  const wxString &GetILSName() const
  {
    return m_sILSName;
  }
  bool GetAllowUserOverrideILS() const
  {
    return m_bAllowUserOverrideILS;
  }
  bool GetAnalyzed() const
  {
    return m_bAnalyzed;
  }
  void SetMarkerSetName(const wxString &s)
  {
    m_sMarkerSetName = s;
  }
  void SetILSName(const wxString &s)
  {
    m_sILSName = s;
  }
  void SetAllowUserOverrideILS(bool b)
  {
    m_bAllowUserOverrideILS = b;
  }
  void SetAnalyzed(bool b)
  {
    m_bAnalyzed = b;
  }
  CLabPositiveControls *GetPositiveControls()
  {
    return &m_PositiveControls;
  }
  CLabLocusCollection *GetOffLadderLoci()
  {
    return &m_lociOffLadder;
  }
  CLabLocusCollection *GetSampleTriallleles()
  {
    return &m_lociSampleTrialleles;
  }
  CLabLocusCollection *GetPosCtrlTrialleles()
  {
    return &m_lociPosCtrlTrialleles;
  }
  void ClearIncompleteData()
  {
    m_lociOffLadder.ClearIncompleteData();
    m_lociSampleTrialleles.ClearIncompleteData();
    m_lociPosCtrlTrialleles.ClearIncompleteData();
    m_PositiveControls.ClearIncompleteData();
  }
  bool HasData();

protected:
  virtual void RegisterAll(bool = false);

private:
  wxString m_sMarkerSetName;
  wxString m_sILSName;
  CLabLocusCollection m_lociOffLadder;
  CLabLocusCollection m_lociSampleTrialleles;
  CLabLocusCollection m_lociPosCtrlTrialleles;
  CLabPositiveControls m_PositiveControls;
  CLabNegativeCtrlNameSet m_NegativeControls;
  bool m_bAllowUserOverrideILS;
  bool m_bAnalyzed;
};

class CLabMarkerSetSpecifications : public TnwxXmlPersistVector<CLabMarkerSetCollection>
{
public:
  CLabMarkerSetSpecifications() : TnwxXmlPersistVector<CLabMarkerSetCollection>(_T("LabMarkerSetCollection")) {;}
  virtual ~CLabMarkerSetSpecifications()
  {
    ClearIncompleteData();
    Clear();
  }
  bool operator == (const CLabMarkerSetSpecifications &x) const
  {
    bool bRtn = IsEqual(x);
    return bRtn;
  }
  void ClearIncompleteData();
};

//*******************************************************************
//
//  CLabSynonym
//
class CLabSynonym : public nwxXmlPersistVectorWxString
{
public:
  CLabSynonym() : nwxXmlPersistVectorWxString(g_SYNONYM)
  {;}
  virtual ~CLabSynonym() {;}
private:
  static const wxString g_SYNONYM;
};
//*******************************************************************
//
//  CLabSpecimenCategory
//

class CLabSpecimenCategory : public nwxXmlPersist
{
public:
  CLabSpecimenCategory()
  {
    RegisterAll(true);
  }
  CLabSpecimenCategory(const CLabSpecimenCategory &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  CLabSpecimenCategory(const wxString &sName)
  {
    RegisterAll(true);
    SetName(sName);
  }
  CLabSpecimenCategory(size_t ndx)
  {
    RegisterAll(true);
    SetName(ndx);
  }
  virtual ~CLabSpecimenCategory() {;}
  CLabSpecimenCategory &operator = (const CLabSpecimenCategory &x)
  {
    LABCP(m_sCategoryName);
    LABCP(m_vsSynonym);
    return *this;
  }
  bool operator == (const CLabSpecimenCategory &x) const
  {
    bool bRtn = false;
    LABEQLOG(m_sCategoryName,"CLabSpecimenCategory.m_sCategoryName")
    else LABEQLOG(m_vsSynonym,"CLabSpecimenCategory.m_vsSynonym")
    else
    {
      bRtn = true;
    }
    return bRtn;
  }
  bool operator != (const CLabSpecimenCategory &x) const
  {
    bool bRtn = !((*this) == x);
    return bRtn;
  }

  void SetName(const wxString &s)
  {
    m_sCategoryName = s;
  }
  void SetName(size_t ndx);
  void SetSynonyms(const wxString &s);
  void SetSynonyms(const vector<wxString> *ps);
  int FindSynonym(const wxString &sSampleName) const;
  const wxString &GetName() const
  {
    return m_sCategoryName;
  }
  const vector<wxString> &GetSynonyms() const
  {
    return m_vsSynonym;
  }
  virtual bool Skip(void *p);

  static const char *LongestType();
  static void AppendTypeArray(wxArrayString *pas, bool bClear = false);
  static size_t TypeCount();
  static bool IndexInRange(size_t ndx);
  static const char * const TYPES[];
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(_T("Type"),&m_sCategoryName);
    Register(_T("Synonym"),&m_ioSynonym,(void *)&m_vsSynonym);
  }
private:
  wxString m_sCategoryName;
  vector<wxString> m_vsSynonym;
  nwxXmlIOPersistVectorWxString m_ioSynonym;
  static const char *LONGEST_TYPE;
};

class CLabSpecimenLess
{
public:
  CLabSpecimenLess() {;}
  bool operator()(
    const CLabSpecimenCategory &x1,
    const CLabSpecimenCategory &x2) const
  {
    nwxStringLessNoCase xless;
    return xless(x1.GetName(),x2.GetName());
  }
  bool operator()(
    const CLabSpecimenCategory *px1,
    const CLabSpecimenCategory *px2) const
  {
    return (*this)(*px1,*px2);
  }
};

class  CLabSetSpecimenCategory:
  public set<CLabSpecimenCategory *,CLabSpecimenLess>
{
public:
  CLabSetSpecimenCategory() {;}
  CLabSetSpecimenCategory(const CLabSetSpecimenCategory &x)
  {
    (*this) = x;
  }
//  virtual ~CLabSetSpecimenCategory();
  bool operator == (const CLabSetSpecimenCategory &x) const;
  void SetSynonyms(
    const wxString &sName, const wxString &sList);
  void SetSynonyms(
    const wxString &sName, const vector<wxString> &vsList);
  void SetSynonyms(
    size_t ndx, const wxString &sList)
  {
    if(CLabSpecimenCategory::IndexInRange(ndx))
    {
      SetSynonyms(
        _T(CLabSpecimenCategory::TYPES[ndx]),
        sList);
    }
  }
  void SetSynonyms(
    size_t ndx, const vector<wxString> &vsList)
  {
    if(CLabSpecimenCategory::IndexInRange(ndx))
    {
      SetSynonyms(
        _T(CLabSpecimenCategory::TYPES[ndx]),
        vsList);
    }
  }
  const CLabSpecimenCategory *Find(const wxString &sName)
  {
    CLabSpecimenCategory s(sName);
    const_iterator itr = find(&s);
    const CLabSpecimenCategory *pRtn(NULL);
    if(itr != end())
    {
      pRtn = *itr;
    }
    return pRtn;
  }
  const vector<wxString> *GetSynonyms(const wxString &sName)
  {
    const CLabSpecimenCategory *pC(Find(sName));
    const vector<wxString> *pRtn(NULL);
    if(pC != NULL)
    {
      pRtn = &(pC->GetSynonyms());
    }
    return pRtn;
  }
  const vector<wxString> *GetSynonyms(size_t ndx)
  {
    return GetSynonyms(CLabSpecimenCategory::TYPES[ndx]);
  }
  CLabSetSpecimenCategory &operator = (const CLabSetSpecimenCategory &x);
  wxString GetCategory(const wxString &sSample) const;
  void Cleanup()
  {
    setptr<CLabSpecimenCategory,CLabSpecimenLess>::cleanup(this);
  }

private:
  bool _TestEquivalent(const CLabSetSpecimenCategory &x) const;
  CLabSpecimenCategory *_Find(const wxString &sName, bool bCreate = false)
  {
    CLabSpecimenCategory *pRtn = (CLabSpecimenCategory *)Find(sName);
    if((pRtn == NULL) && bCreate)
    {
      pRtn = new CLabSpecimenCategory(sName);
      insert(pRtn);
    }
    return pRtn;
  }
};

typedef TnwxXmlIOPersistSet<CLabSpecimenCategory,CLabSpecimenLess>
  CLabIOsetSpecimenCategory;



//************************************************************ 
//
//  CLabLocusThreshold
//
class CLabLocusThreshold : public nwxXmlPersist
{
public:
  CLabLocusThreshold()
  {
    RegisterAll(true);
  }
  CLabLocusThreshold(const wxString &sLocusName) :
    m_sName(sLocusName)
  {
    RegisterAll(true);
  }
  CLabLocusThreshold(const CLabLocusThreshold &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  CLabLocusThreshold &operator = (const CLabLocusThreshold &x)
  {
    LABCP(m_sName);
    LABCP(m_dPullupFractionFilter);
    LABCP(m_dFractionMaxPeak);
    LABCP(m_dStutter);
    LABCP(m_dPlusStutter);
    LABCP(m_dAdenylation);
    LABCP(m_dHeterozygousImbalanceLimit);
    LABCP(m_dMinBoundForHomozygote);
    return *this;
  }
  virtual ~CLabLocusThreshold() {}
  bool operator == (const CLabLocusThreshold &x) const;

  const wxString &GetLocusName() const
  {
    return m_sName;
  }
  double GetFractionMaxPeak() const
  {
    return m_dFractionMaxPeak;
  }
  double GetPullupFractionFilter() const
  {
    return m_dPullupFractionFilter;
  }
  double GetStutter() const
  {
    return m_dStutter;
  }
  double GetPlusStutter() const
  {
    return m_dPlusStutter;
  }
  double GetAdenylation() const
  {
    return m_dAdenylation;
  }
  double GetHeterozygousImbalanceLimit() const
  {
    return m_dHeterozygousImbalanceLimit;
  }
  double GetMinBoundForHomozygote() const
  {
    return m_dMinBoundForHomozygote;
  }

  void SetLocusName(const wxString &s)
  {
    m_sName = s;
  }
  void SetFractionMaxPeak(double d)
  {
    m_dFractionMaxPeak = d;
  }
  void SetPullupFractionFilter(double d)
  {
    m_dPullupFractionFilter = d;
  }
  void SetStutter(double d)
  {
    m_dStutter = d;
  }
  void SetPlusStutter(double d)
  {
    m_dPlusStutter = d;
  }
  void SetAdenylation(double d)
  {
    m_dAdenylation = d;
  }
  void SetHeterozygousImbalanceLimit(double d)
  {
    m_dHeterozygousImbalanceLimit = d;
  }
  void SetMinBoundForHomozygote(double d)
  {
    m_dMinBoundForHomozygote = d;
  }

  virtual void Init();
  virtual void Init(void *p);
  virtual bool Skip(void *p);
  bool Skip() const;

protected:
  virtual void RegisterAll(bool b = false);
private:
  wxString m_sName;
  double m_dFractionMaxPeak;
  double m_dPullupFractionFilter;
  double m_dStutter;
  double m_dPlusStutter;
  double m_dAdenylation;
  double m_dHeterozygousImbalanceLimit;
  double m_dMinBoundForHomozygote;

private:
  class IOdoubleNoMinus1 : public nwxXmlIOdouble
  {
  public:
    IOdoubleNoMinus1()
    {
      SetDefault(CLabLocusThreshold::DEFAULT);
    }
    virtual bool Skip(void *pObj)
    {
      double &x(CAST(pObj));
      bool bRtn = (x == m_DEFAULT);
      return bRtn;
    }
  };
  static const double DEFAULT;
  static IOdoubleNoMinus1 g_io;
};
class CLabLocusThresholdLess
{
public:
  CLabLocusThresholdLess() {}
  virtual ~CLabLocusThresholdLess() {}
  bool operator()(const CLabLocusThreshold &a1, const CLabLocusThreshold &a2) const
  {
    const wxString &s1(a1.GetLocusName());
    const wxString &s2(a2.GetLocusName());
    int n = s1.CmpNoCase(s2);
    if(!n)
    {
      // it's case sensitive, but sorting is not case sensitive
      n = s1.Cmp(s2);
    }
    return (n < 0);
  }
  bool operator()(const CLabLocusThreshold *pa1, const CLabLocusThreshold *pa2) const
  {
    return (*this)(*pa1,*pa2);
  }
};

class SetLabLocusThresholdIO : public TnwxXmlIOPersistSet<CLabLocusThreshold,CLabLocusThresholdLess>
{
public:
  SetLabLocusThresholdIO() : TnwxXmlIOPersistSet<CLabLocusThreshold,CLabLocusThresholdLess>(false)
  {};
};
class SetLabLocusThreshold : public set<CLabLocusThreshold *,CLabLocusThresholdLess>
{
public:
  SetLabLocusThreshold &operator = (const SetLabLocusThreshold &x)
  {
    setptr<CLabLocusThreshold,CLabLocusThresholdLess>::copy(this,x);
    return *this;
  }
  void Set(const CLabLocusThreshold &x)
  {
    iterator itr = find((CLabLocusThreshold *)&x);
    bool bSkip = x.Skip();
    if(itr == end())
    {
      if(!bSkip)
      {
        CLabLocusThreshold *pNew = new CLabLocusThreshold(x);
        insert(pNew);
      }
    }
    else if(!bSkip)
    {
      CLabLocusThreshold *p = *itr;
      (*p) = x;
    }
    else
    {
      erase(itr);
    }
  }
  CLabLocusThreshold *Find(const wxString &sLocusName, bool bCreate = true)
  {
    CLabLocusThreshold x(sLocusName);
    iterator itr = find(&x);
    CLabLocusThreshold *pRtn = NULL;
    if(itr != end())
    {
      pRtn = *itr;
    }
    else if(bCreate && !sLocusName.IsEmpty())
    {
      pRtn = new CLabLocusThreshold(sLocusName);
      insert(pRtn);
    }
    return pRtn;
  }
  const CLabLocusThreshold *Find(const wxString &sLocusName) const
  {
    CLabLocusThreshold x(sLocusName);
    const_iterator itr = find(&x);
    CLabLocusThreshold *pRtn = (itr == end()) ? NULL : *itr;
    return pRtn;
  }
  void MakeVector(vector<CLabLocusThreshold *> &v) const
  {
    const_iterator itr;
    for(itr = begin(); itr != end(); ++itr)
    {
      if(!(*itr)->Skip())
      {
        v.push_back(*itr);
      }
    }
  }
  bool operator ==(const SetLabLocusThreshold &x) const
  {
    const_iterator itr;
    vector<CLabLocusThreshold *> v1;
    vector<CLabLocusThreshold *> v2;
    bool bRtn = false;
    v1.reserve(this->size());
    v2.reserve(x.size());
    MakeVector(v1);
    x.MakeVector(v2);
    bRtn = vectorptr<CLabLocusThreshold>::IsEqual(v1,v2);
    return bRtn;
  }
  void Cleanup()
  {
    setptr<CLabLocusThreshold,CLabLocusThresholdLess>::cleanup(this);
  }
};

//************************************************************ level 3
//
//  CLabRFU
//
class CLabRFU : public nwxXmlPersist
{
public:
  typedef enum
  {
    TYPE_SAMPLE = 0,
    TYPE_LADDER,
    TYPE_ILS
  } RFU_TYPE;

  CLabRFU(CLabRFU::RFU_TYPE nType)
    : m_nType(nType)
  {
    RegisterAll(true);
  }
  /*
  CLabRFU() : m_nType(TYPE_SAMPLE)
  {
    RegisterAll(true);
  }
  */
  CLabRFU(const CLabRFU &x)
  {
    m_nType = x.m_nType;
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CLabRFU()
  {
    m_apLocusThreshold.Cleanup();
  }
  CLabRFU &operator =(const CLabRFU &x)
  {
    bool bDiff = (m_nType != x.m_nType);
    if(bDiff)
    {
      LABCP(m_nType);
      RegisterAll();
    }
    LABCP(m_apLocusThreshold);
    LABCP(m_dPullupFractionFilter);
    LABCP(m_dFractionMaxPeak);
    LABCP(m_dStutter);
    LABCP(m_dPlusStutter);
    LABCP(m_dAdenylation);
    LABCP(m_nMin);
    LABCP(m_nMinInterlocus);
    LABCP(m_nMax);
    LABCP(m_nDetection);
    return *this;
  }
  bool operator == (const CLabRFU &x) const;
  void SetPullupFractionFilter(double d)
  {
    m_dPullupFractionFilter = d;
  }
  void SetFractionMaxPeak(double d)
  {
    m_dFractionMaxPeak = d;
  }
  void SetStutterThreshold(double d)
  {
    m_dStutter = d;
  }
  void SetPlusStutterThreshold(double d)
  {
    m_dPlusStutter = d;
  }
  void SetAdenylationThreshold(double d)
  {
    m_dAdenylation = d;
  }
  void SetMinRFU(int n)
  {
    m_nMin = n;
  }
  void SetMinRFUinterlocus(int n)
  {
    m_nMinInterlocus = n;
  }
  void SetMaxRFU(int n)
  {
    m_nMax = n;
  }
  void SetMinDetection(int n)
  {
    m_nDetection = n;
  }
  double GetPullupFractionFilter() const
  {
    return m_dPullupFractionFilter;
  }
  double GetFractionMaxPeak() const
  {
    return m_dFractionMaxPeak;
  }
  double GetStutterThreshold()  const
  {
    return m_dStutter;
  }
  double GetPlusStutterThreshold()  const
  {
    return m_dPlusStutter;
  }
  double GetAdenylationThreshold()  const
  {
    return m_dAdenylation;
  }
  int GetMinRFU()  const
  {
    return m_nMin;
  }
  int GetMinDetection() const
  {
    return m_nDetection;
  }
  int GetMinRFUinterlocus()  const
  {
    return m_nMinInterlocus;
  }
  int GetMaxRFU()  const
  {
    return m_nMax;
  }
  CLabLocusThreshold *GetLabLocusThreshold(
    const wxString &sLocus, bool bCreate = true)
  {
    CLabLocusThreshold *pRtn = m_apLocusThreshold.Find(sLocus,bCreate);
    return pRtn;
  }
  const CLabLocusThreshold *GetLabLocusThreshold(
    const wxString &sLocus) const
  {
    const CLabLocusThreshold *pRtn = m_apLocusThreshold.Find(sLocus);
    return pRtn;
  }
  void SetLocusThreshold(const CLabLocusThreshold &x)
  {
    m_apLocusThreshold.Set(x);
  }


  virtual void Init(void *p);
  virtual void Init();
protected:
  virtual void RegisterAll(bool b = false);
private:
  SetLabLocusThreshold m_apLocusThreshold;
  SetLabLocusThresholdIO m_ioLocusThreshold;
  double m_dPullupFractionFilter;
  double m_dFractionMaxPeak;
  double m_dStutter;
  double m_dPlusStutter;
  double m_dAdenylation;
  int m_nMin;
  int m_nMinInterlocus;
  int m_nMax;
  int m_nDetection; // min Detection RFU for sample only
  RFU_TYPE m_nType;
};




//************************************************************
//
//  CLabThresholdMessage
//

class CLabThresholdMessage : public nwxXmlPersist
{
public:
  CLabThresholdMessage() : m_nValue(0)
  {
    RegisterAll(true);
  }
  CLabThresholdMessage(const wxString &sName, int nValue) :
    m_sName(sName), m_nValue(nValue)
  {
    RegisterAll(false);
  }
  CLabThresholdMessage(const CLabThresholdMessage &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  CLabThresholdMessage *Clone() const
  {
    CLabThresholdMessage *pRtn = new CLabThresholdMessage(*this);
    return pRtn;
  }
  virtual ~CLabThresholdMessage();
  virtual bool Skip(void *p);

  CLabThresholdMessage &operator =(const CLabThresholdMessage &x)
  {
    LABCP(m_sName);
    LABCP(m_nValue);
    return *this;
  }
  bool operator ==(const CLabThresholdMessage &x)
  {
    bool bRtn = false;
    LABEQLOG(m_sName,"CLabThresholdMessage.m_sName")
    else LABEQLOG(m_nValue,"CLabThresholdMessage.m_nValue")
    else
    {
      bRtn = true;
    }
    return bRtn;
  }

  void SetThreshold(int nValue)
  {
    m_nValue = nValue;
  }
  int GetThreshold() const
  {
    return m_nValue;
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
protected:
  void RegisterAll(bool = true);
private:
  wxString m_sName;
  int m_nValue;
};

//************************************************************ 
//
//  CLabThresholdMessageLess
//
class CLabThresholdMessageLess
{
public:
  CLabThresholdMessageLess() {;}
  bool operator()(
    const CLabThresholdMessage &x1,
    const CLabThresholdMessage &x2) const
  {
    nwxStringLessNoCase xless;
    return xless(x1.GetName(),x2.GetName());
  }
  bool operator()(
    const CLabThresholdMessage *px1,
    const CLabThresholdMessage *px2) const
  {
    return (*this)(*px1,*px2);
  }
};

typedef TnwxXmlPersistSet<CLabThresholdMessage,CLabThresholdMessageLess> LAB_MESSAGE_SET;

class CLabMessageSet : public LAB_MESSAGE_SET
{
public:
  CLabMessageSet() : LAB_MESSAGE_SET(_T("MessageThreshold"),false)
  {}
  CLabMessageSet(const CLabMessageSet &x) : LAB_MESSAGE_SET(x)
  {}
  virtual ~CLabMessageSet() {};
  void RemoveMessage(const wxChar *ps)
  {
    CLabThresholdMessage xm(ps,0);
    set<CLabThresholdMessage*,CLabThresholdMessageLess>::iterator
        itr = Get()->find(&xm);
    if(itr != Get()->end())
    {
      delete *itr;
      Get()->erase(itr);
    }
  }
  void RemoveSkipped();

  bool IsEqualNoLab(const CLabMessageSet &x) const;
};



//************************************************************ level 2
//
//  CLabThresholds
//
class CLabThresholds : public nwxXmlPersist
{
public:
  CLabThresholds() : 
    m_rfuLadder(CLabRFU::TYPE_LADDER),
    m_rfuLS(CLabRFU::TYPE_ILS),
    m_rfuSample(CLabRFU::TYPE_SAMPLE)
  {
    RegisterAll(true);
  }
  CLabThresholds(const CLabThresholds &x) :
    m_rfuLadder(CLabRFU::TYPE_LADDER),
    m_rfuLS(CLabRFU::TYPE_ILS),
    m_rfuSample(CLabRFU::TYPE_SAMPLE)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CLabThresholds() 
  {
    m_spMsgs.Clear();
  }
  CLabThresholds &operator = (const CLabThresholds &x)
  {
    LABCP(m_sMinBoundHomozygoteUnit);
    LABCP(m_rfuLadder);
    LABCP(m_rfuLS);
    LABCP(m_rfuSample);
    LABCP(m_dHeterozygousImbalanceLimit);
    LABCP(m_dMinBoundForHomozygote);
    LABCP(m_dMaxResidualForAlleleCall);
    LABCP(m_nMaxNumberOfPullupsPerSample);
    LABCP(m_nMaxNumberOfStutterPeaksPerSample);
    LABCP(m_nMaxNumberOfSpikesPerSample);
    LABCP(m_nMaxNumberOfAdenylationsPerSample);
    LABCP(m_nMaxNumberOfOLAllelesPerSample);
    LABCP(m_nMinBPSForArtifacts);
    LABCP(m_nMaxRFUForIncompleteSample);
    LABCP(m_nMaxExcessiveResidual);
    LABCP(m_bAllowMinRFUoverride);
    LABCP(m_spMsgs);

    return *this;
  }

  bool operator == (const CLabThresholds &x) const;
  //void SetupMessages();
  void SetupCompatibility();
  virtual void Init(void *p);
  virtual void Init();


  int GetValue(const wxString &sMsgName, int nDefault = -1) const;
  void SetValue(const wxString &sMsgName,
    int nValue,
    bool bCreateIfNeeded = true);

  wxChar GetMinBoundHomozygoteUnit() const
  {
    wxChar c(0);
    if(!m_sMinBoundHomozygoteUnit.IsEmpty())
    {
      c = m_sMinBoundHomozygoteUnit.GetChar(0);
    }
    return c;
  }
  void SetMinBoundHomozygoteUnit(const wxString &s)
  {
    m_sMinBoundHomozygoteUnit = s;
  }
  void SetMinBoundHomozygoteUnit(wxChar c)
  {
    m_sMinBoundHomozygoteUnit = c;
  }
  int GetMaxNumberOfPullupsPerSample() const
  {
    return GetValue(g_psmNumberOfPullUpsInSample);
  }
  int GetMaxNumberOfStutterPeaksPerSample() const
  {
    return GetValue(g_psmNumberOfStuttersInSample);
  }
  int GetMaxNumberOfAdenylationsPerSample()  const
  {
    return GetValue(g_psmNumberOfAdenylationsInSample);
  }
  int GetMaxNumberOfOLAllelesPerSample()  const
  {
    return GetValue(g_psmNumberOfOffLadderAllelesInSample);
  }
  int GetMaxExcessiveResidual()  const
  {
    return GetValue(g_psmNumberOfExcessiveResidualsInSample);
  }
  int GetMaxRFUForIncompleteSample() const
  {
    return m_nMaxRFUForIncompleteSample;
  }

  void SetMaxNumberOfPullupsPerSample(int n)
  {
    SetValue(g_psmNumberOfPullUpsInSample,n);
  }
  void SetMaxNumberOfStutterPeaksPerSample(int n)
  {
    SetValue(g_psmNumberOfStuttersInSample,n);
  }
  void SetMaxNumberOfAdenylationsPerSample(int n)
  {
    SetValue(g_psmNumberOfAdenylationsInSample,n);
  }
  void SetMaxNumberOfOLAllelesPerSample(int n)
  {
    SetValue(g_psmNumberOfOffLadderAllelesInSample,n);
  }
  void SetMaxExcessiveResidual(int n)
  {
    SetValue(g_psmNumberOfExcessiveResidualsInSample,n);
  }
  void SetMaxRFUForIncompleteSample(int n)
  {
    m_nMaxRFUForIncompleteSample = n;
  }
  void SetMinBPSForArtifacts(int n)
  {
    m_nMinBPSForArtifacts = n;
  }
  int GetMinBPSForArtifacts() const
  {
    return m_nMinBPSForArtifacts;
  }
  void SetMaxResidualForAlleleCall(double d)
  {
    m_dMaxResidualForAlleleCall = d;
  }
  double GetMaxResidualForAlleleCall() const
  {
    return m_dMaxResidualForAlleleCall;
  }
  bool GetAllowMinRFUoverride() const
  {
    return m_bAllowMinRFUoverride;
  }
  void SetAllowMinRFUoverride(bool b)
  {
    m_bAllowMinRFUoverride = b;
  }
  CLabRFU *GetRFUsample()
  {
    return &m_rfuSample;
  }
  CLabRFU *GetRFUladder()
  {
    return &m_rfuLadder;
  }
  CLabRFU *GetRFUls()
  {
    return &m_rfuLS;
  }
  const CLabRFU *GetRFUsample() const
  {
    return &m_rfuSample;
  }
  const CLabRFU *GetRFUladder() const
  {
    return &m_rfuLadder;
  }
  const CLabRFU *GetRFUls() const
  {
    return &m_rfuLS;
  }
  void SetHeterozygousImbalanceLimit(double d)
  {
    m_dHeterozygousImbalanceLimit = d;
  }
  double GetHeterozygousImbalanceLimit() const
  {
    return m_dHeterozygousImbalanceLimit;
  }
  void SetMinBoundForHomozygote(double d)
  {
    m_dMinBoundForHomozygote = d;
  }
  double GetMinBoundForHomozygote() const
  {
    return m_dMinBoundForHomozygote;
  }
  static const wxChar * const g_psmNumberOfPullUpsInSample;
  static const wxChar * const g_psmNumberOfStuttersInSample;
  static const wxChar * const g_psmNumberOfAdenylationsInSample;
  static const wxChar * const g_psmNumberOfOffLadderAllelesInSample;
  static const wxChar * const g_psmNumberOfExcessiveResidualsInSample;

protected:
  virtual void RegisterAll(bool = false);


  CLabRFU m_rfuLadder;
  CLabRFU m_rfuLS;
  CLabRFU m_rfuSample;
  CLabMessageSet m_spMsgs;

  wxString m_sMinBoundHomozygoteUnit;
  double m_dHeterozygousImbalanceLimit;
  double m_dMinBoundForHomozygote;
  double m_dMaxResidualForAlleleCall;
  int m_nMaxNumberOfSpikesPerSample;// not used
  int m_nMinBPSForArtifacts;
  int m_nMaxRFUForIncompleteSample;
  bool m_bAllowMinRFUoverride;


private:
  // obsolete, replaced with messages and made
  // private with get/set functions

  int m_nMaxNumberOfPullupsPerSample;
  int m_nMaxNumberOfStutterPeaksPerSample;
  int m_nMaxNumberOfAdenylationsPerSample;
  int m_nMaxNumberOfOLAllelesPerSample;
  int m_nMaxExcessiveResidual;

  // end obsolete

  void __SetupNameInt();
  void SetupNameInt()
  {
    if(!m_mapNameInt.size())
    {
      __SetupNameInt();
    }
  }
  mutable map<wxString,int *> m_mapNameInt;
};

//************************************************************ level 2
//
//  CLabNameStrings
//
class CLabNameStrings : public nwxXmlPersist
{
public:
  CLabNameStrings() : m_ioSpecimenCategory(false)
  {
    RegisterAll(true);
#ifdef __WXDEBUG__
    g_nCreate++;
#endif
  }
  virtual ~CLabNameStrings() 
  {
    m_setSpecimenCategory.Cleanup();
#ifdef __WXDEBUG__
    g_nDestroy++;
#endif
  }

  CLabNameStrings &operator = (const CLabNameStrings &x)
  {
    LABCP(m_bUseSampleName);
    LABCP(m_vsSynonymLadder);
    LABCP(m_vsSynonymPosCtrl);
    LABCP(m_vsSynonymNegCtrl);
    LABCP(m_vsSynonymPossibleMixture);
    LABCP(m_vsSynonymSingleSource);

  LABCP(m_sStdCtrlName);
    LABCP(m_setSpecimenCategory);
    return *this;
  }
  bool operator == (const CLabNameStrings &x) const
  {
    bool bRtn = false;
    LABEQLOG(m_vsSynonymLadder,"CLabNameStrings.m_vsSynonymLadder")
    else LABEQLOG(m_vsSynonymPosCtrl,"CLabNameStrings.m_vsSynonymPosCtrl")
    else LABEQLOG(m_vsSynonymNegCtrl,"CLabNameStrings.m_vsSynonymNegCtrl")
    else LABEQLOG(m_vsSynonymPossibleMixture,"CLabNameStrings.m_vsSynonymPossibleMixture")
    else LABEQLOG(m_vsSynonymSingleSource,"CLabNameStrings.m_vsSynonymSingleSource")
    else LABEQLOG(m_sStdCtrlName,"CLabNameStrings.m_sStdCtrlName")
    else LABEQLOG(m_setSpecimenCategory,"CLabNameStrings.m_setSpecimenCategory")
    else LABEQLOG(m_bUseSampleName,"CLabNameStrings.m_bUseSampleName")
    else
    {
      bRtn = true;
    }
    return bRtn;
  }
  bool UseSampleName() const
  {
    return m_bUseSampleName;
  }
  void SetUseSampleName(bool b)
  {
    m_bUseSampleName = b;
  }
  virtual void Init()
  {
    m_setSpecimenCategory.Cleanup();
    nwxXmlPersist::Init();
  }
  virtual void Init(void *p)
  {
    ((CLabNameStrings *)p)->Init();
  }
  wxString GetCategory(const wxString &sSampleName) const;
  static int FindSubstring(
    const wxString &s, const vector<wxString> &vs);
protected:
  virtual void RegisterAll(bool = false);
private:
  CLabIOsetSpecimenCategory m_ioSpecimenCategory;
public:
  CLabSynonym m_vsSynonymLadder;
  CLabSynonym m_vsSynonymPosCtrl;
  CLabSynonym m_vsSynonymNegCtrl;
  CLabSynonym m_vsSynonymPossibleMixture;
  CLabSynonym m_vsSynonymSingleSource;
  wxString m_sStdCtrlName;
  CLabSetSpecimenCategory m_setSpecimenCategory;
  bool m_bUseSampleName;
#ifdef __WXDEBUG__
  static int g_nCreate;
  static int g_nDestroy;
#endif
public:
  static const char * const DEFAULT_SPECIMEN_CATEGORY;
  static const char * const TYPE_LADDER;
  static const char * const TYPE_POS_CONTROL;
  static const char * const TYPE_NEG_CONTROL;
  static const char * const TYPE_POSSIBLE_MIXTURE;
  static const char * const TYPE_SINGLE_SOURCE;
};

//************************************************************ 
//
//  CLabReview
//
class CLabReview : public nwxXmlPersist
{
public:
  typedef enum
  {
    REVIEW_0 = 0,
    REVIEW_DEFAULT = 0,
    REVIEW_LOCUS,
    REVIEW_CHANNEL,
    REVIEW_ILS,
    REVIEW_SAMPLE,
    REVIEW_DIR,
    __REVIEW_COUNT
  } REVIEW_TYPE;

  static bool IsDefault(int n)
  {
    return (n == REVIEW_DEFAULT);
  }
  CLabReview()
  {
    RegisterAll(true);
  }
  CLabReview(const CLabReview &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  CLabReview &operator = (const CLabReview &x)
  {
    memcpy((void *)m_nReviewers,(void *)x.m_nReviewers,sizeof(m_nReviewers));
    memcpy((void *)m_nAcceptance,(void *)x.m_nAcceptance,sizeof(m_nAcceptance));
    m_bAllowUserNameOverride = x.m_bAllowUserNameOverride;
    m_bAllowExportWithAttnReqd = x.m_bAllowExportWithAttnReqd;
    return *this;
  }
  bool operator ==(const CLabReview &x) const
  {
#define LABMEMCMP(z,xx) if(memcmp((void *)x.z,(void *)z,sizeof(z))) { LAB_LOG(xx); }

    bool bRtn = false;
    LABMEMCMP(m_nReviewers,"CLabReview.m_nReviewers")
    else  LABMEMCMP(m_nAcceptance,"CLabReview.m_nAcceptance")
    else LABEQLOG(m_bAllowUserNameOverride,"CLabReview.m_bAllowUserNameOverride")
    else LABEQLOG(m_bAllowExportWithAttnReqd,"CLabReview.m_bAllowExportWithAttnReqd")
    else
    {
      bRtn = true;
    }
    return bRtn;

#undef LABMEMCMP
  }
private:
  static int _GetCount(int nType, bool bCheckDefault, const int *pnArray)
  {
    int nRtn = pnArray[nType];
    if(nRtn < 0 && bCheckDefault)
    {
      if(!IsDefault(nType))
      {
        nRtn = pnArray[CLabReview::REVIEW_DEFAULT];
      }
      if(nRtn < 0)
      {
        nRtn = 0;
      }
    }
    return nRtn;
  }
public:
  bool GetAllowUserNameOverride() const
  {
    return m_bAllowUserNameOverride;
  }
  void SetAllowUserNameOverride(bool bAllow)
  {
    m_bAllowUserNameOverride = bAllow;
  }
  bool GetAllowExportWithAttnReqd() const
  {
    return m_bAllowExportWithAttnReqd;
  }
  void SetAllowExportWithAttnReqd(bool bAllow)
  {
    m_bAllowExportWithAttnReqd = bAllow;
  }
  int GetReviewerCount(
    int nType = CLabReview::REVIEW_DEFAULT,
    bool bCheckDefault = true) const
  {

    int nRtn = _GetCount(nType,bCheckDefault,m_nReviewers);
    return nRtn;
  }
  int GetAcceptanceCount(
    int nType = CLabReview::REVIEW_DEFAULT,
    bool bCheckDefault = true) const
  {
    int nRtn = _GetCount(nType,bCheckDefault,m_nAcceptance);
    return nRtn;
  }
  void SetReviewerCount(int n, int nType = CLabReview::REVIEW_DEFAULT)
  {
    m_nReviewers[nType] = n;
  }
  void SetAcceptanceCount(int n, int nType = CLabReview::REVIEW_DEFAULT)
  {
    m_nAcceptance[nType] = n;
  }

  static const wxChar * const g_LABELS[__REVIEW_COUNT];
protected:
  virtual void RegisterAll(bool = false);
private:
  bool m_bAllowUserNameOverride;
  bool m_bAllowExportWithAttnReqd;
  int m_nReviewers[__REVIEW_COUNT];
  int m_nAcceptance[__REVIEW_COUNT];

  static nwxXmlIOint g_ioReview;
  static nwxXmlIOint g_ioAccept;
  static nwxXmlIOintPositive g_io;
};

//************************************************************ level 1
//
//  CLabSettings
//
class CLabSettings: public nwxXmlPersist
{
public:
  CLabSettings()
  {
#ifdef __WXDEBUG__
    g_nCreate++;
#endif
    RegisterAll(true);
  }
  CLabSettings(const CLabSettings &x)
  {
#ifdef __WXDEBUG__
    g_nCreate++;
#endif
    RegisterAll(true);
    *this = x;
  }
  virtual ~CLabSettings()
  {
#ifdef __WXDEBUG__
    g_nDestroy++;
#endif
    ClearIncompleteData();
  }
  virtual bool SaveFile(const wxString &sFileName);
  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName)
  {
    ClearIncompleteData();
    SyncUseSampleName();
    return nwxXmlPersist::CreateNodeList(sNodeName);
  }
  void SyncUseSampleName()
  {
    // boolean for using name strings with samples or 
    // file names is in  CLabNameStrings m_str;
    // the analysis program looks for it in 
    // CLabThresholds m_thresholds;
    int n = m_str.UseSampleName() ? 1 : 0;
    m_thresholds.SetValue(_T("smUseSampleNamesForControlSampleTestsPreset"),n,true);
  }
  void ClearIncompleteData()
  {
    m_marker.ClearIncompleteData();
  }
  virtual const wxString &RootNode(void) const
  {
    return g_LABROOT;
  }
  CLabSettingsInfo *GetLabSettingsInfo()
  {
    return &m_info;
  }
  const CLabSettingsInfo *GetLabSettingsInfo() const
  {
    return &m_info;
  }
  CLabNameStrings *GetLabStrings()
  {
    return &m_str;
  }
  const CLabNameStrings *GetLabStrings() const
  {
    return &m_str;
  }
  CLabThresholds *GetThresholds()
  {
    return &m_thresholds;
  }
  const CLabThresholds *GetThresholds() const
  {
    return &m_thresholds;
  }
  CLabMarkerSetSpecifications *GetMarkers()
  {
    return &m_marker;
  }
  const CLabMarkerSetSpecifications *GetMarkers() const
  {
    return &m_marker;
  }
  CLabMarkerSetCollection *GetMarkerCollection()
  {
    CLabMarkerSetCollection *pRtn = 
      m_marker.Get()->size()
      ? m_marker.Get()->at(0)
      : NULL;
    return pRtn;
  }
  const CLabMarkerSetCollection *GetMarkerCollection() const
  {
    CLabMarkerSetCollection *pRtn = 
      m_marker.Get()->size()
      ? m_marker.Get()->at(0)
      : NULL;
    return pRtn;
  }
  CLabReview *GetReviewAcceptance()
  {
    return &m_review;
  }
  const CLabReview *GetReviewAcceptance() const
  {
    return &m_review;
  }
  bool operator ==(const CLabSettings &x)  const;
  bool operator !=(const CLabSettings &x)
  {
    return !((*this) == x);
  }
  CLabSettings &operator =(const CLabSettings &x)
  {
    LABCP(m_info);
    LABCP(m_str);
    LABCP(m_thresholds);
    LABCP(m_marker);
    LABCP(m_review);
    return *this;
  }
  void GetMinRFU(
    int *pnSample, int *pnILS, int *pnLadder, 
    int *pnInterlocus, int *pnLadderInterlocus,
    int *pnSampleDetection) const
  {
    *pnSample = m_thresholds.GetRFUsample()->GetMinRFU();
    *pnInterlocus = m_thresholds.GetRFUsample()->GetMinRFUinterlocus();
    *pnSampleDetection = m_thresholds.GetRFUsample()->GetMinDetection();

    *pnLadder = m_thresholds.GetRFUladder()->GetMinRFU();
    *pnLadderInterlocus = m_thresholds.GetRFUladder()->GetMinRFUinterlocus();

    *pnILS    = m_thresholds.GetRFUls()->GetMinRFU();
  }
  bool CanOverrideMinRfu() const
  {
    return m_thresholds.GetAllowMinRFUoverride();
  }
  void SetCanOverrideMinRfu(bool b)
  {
    m_thresholds.SetAllowMinRFUoverride(b);
  }
  bool GetAllowUserOverrideILS() const
  {
    const CLabMarkerSetCollection *pMarker = GetMarkerCollection();
    bool bRtn = false;
    if(pMarker != NULL)
    {
      bRtn = pMarker->GetAllowUserOverrideILS();
    }
    return bRtn;
  }
  void SetAllowUserOverrideILS(bool b)
  {
    CLabMarkerSetCollection *pMarker = GetMarkerCollection();
    if(pMarker != NULL)
    {
      pMarker->SetAllowUserOverrideILS(b);
    }
  }
  bool GetAnalyzedData() const
  {
    const CLabMarkerSetCollection *pMarker = GetMarkerCollection();
    bool bRtn = false;
    if(pMarker != NULL)
    {
      bRtn = pMarker->GetAnalyzed();
    }
    return bRtn;
  }
  void SetAnalyzedData(bool b)
  {
    CLabMarkerSetCollection *pMarker = GetMarkerCollection();
    if(pMarker != NULL)
    {
      pMarker->SetAnalyzed(b);
    }
  }
  const wxString &GetVolumeName() const
  {
    return m_info.GetVolumeName();
  }
  wxString GetKitName() const
  {
    const CLabMarkerSetCollection *pMarker = GetMarkerCollection();
    wxString sRtn;
    if(pMarker != NULL)
    {
      sRtn = pMarker->GetMarkerSetName();
    }
    return sRtn;
  }
  int GetReviewerCount(int nType = CLabReview::REVIEW_DEFAULT) const
  {
    return m_review.GetReviewerCount(nType);
  }
  int GetReviewerCountSample() const
  {
    return m_review.GetReviewerCount(CLabReview::REVIEW_SAMPLE);
  }
  int GetReviewerCountDir() const
  {
    return m_review.GetReviewerCount(CLabReview::REVIEW_DIR);
  }
  int GetReviewerCountILS() const
  {
    return m_review.GetReviewerCount(CLabReview::REVIEW_ILS);
  }
  int GetReviewerCountChannel() const
  {
    return m_review.GetReviewerCount(CLabReview::REVIEW_CHANNEL);
  }
  int GetReviewerCountLocus() const
  {
    return m_review.GetReviewerCount(CLabReview::REVIEW_LOCUS);
  }

  int GetAcceptanceCount(int nType = CLabReview::REVIEW_DEFAULT) const
  {
    return m_review.GetAcceptanceCount(nType);
  }
  int GetAcceptanceCountSample() const
  {
    return m_review.GetAcceptanceCount(CLabReview::REVIEW_SAMPLE);
  }
  int GetAcceptanceCountDir() const
  {
    return m_review.GetAcceptanceCount(CLabReview::REVIEW_DIR);
  }
  int GetAcceptanceCountILS() const
  {
    return m_review.GetAcceptanceCount(CLabReview::REVIEW_ILS);
  }
  int GetAcceptanceCountChannel() const
  {
    return m_review.GetAcceptanceCount(CLabReview::REVIEW_CHANNEL);
  }
  int GetAcceptanceCountLocus() const
  {
    return m_review.GetAcceptanceCount(CLabReview::REVIEW_LOCUS);
  }
  bool GetReviewerAllowUserOverride() const
  {
    return m_review.GetAllowUserNameOverride();
  }
  bool GetAllowExportWithAttnReqd() const
  {
    return m_review.GetAllowExportWithAttnReqd();
  }


  void SetReviewerCount(int n,int nType = CLabReview::REVIEW_DEFAULT)
  {
    m_review.SetReviewerCount(n,nType);
  }
  void SetAcceptanceCount(int n,int nType = CLabReview::REVIEW_DEFAULT)
  {
    m_review.SetAcceptanceCount(n,nType);
  }


//  bool Load(); 
#ifdef __WXDEBUG__
  static void UnitTest();
#endif

protected:
  virtual void RegisterAll(bool = false);

private:
  CLabSettingsInfo m_info;

  // NameStrings
  CLabNameStrings m_str;

  // Thresholds
  CLabThresholds m_thresholds;

  // Marker Set Specs
  CLabMarkerSetSpecifications m_marker;

  // number of reviewers

  CLabReview m_review;

  static const wxString g_LABROOT;


private:
  static nwxXmlIOdoublePositive g_xmlIOdoubleGt0;
  static nwxXmlIOdouble g_xmlIOdouble;
  static nwxXmlIOintPositive g_xmlIOint;
public:
  static InwxXmlIO *GetIOdouble()
  {
    return &g_xmlIOdouble;
  }
  static InwxXmlIO *GetIOint()
  {
    return &g_xmlIOint;

  }
  static InwxXmlIO *GetIOdoubleGt0()
  {
    return &g_xmlIOdoubleGt0;
  }

////   Global info
#ifdef __WXDEBUG__
  static int g_nCreate;
  static int g_nDestroy;
#endif

};
#ifndef __C_LAB_SETTINGS_CPP__
#undef LABEQLOG
#undef LABEQDLOG
#undef LAB_LOG
#undef LAB_LOG_INEQ
#undef LABCP
#undef LABEQ
#undef LABEQD
#endif
#endif
