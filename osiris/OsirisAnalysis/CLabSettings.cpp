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
*  OSIRIS is a desktop tool working on your computer with your own data.
*  Your sample profile data is processed on your computer and is not sent
*  over the internet.
*
*  For quality monitoring, OSIRIS sends some information about usage
*  statistics  back to NCBI.  This information is limited to use of the
*  tool, without any sample, profile or batch data that would reveal the
*  context of your analysis.  For more details and instructions on opting
*  out, see the Privacy Information section of the OSIRIS User's Guide.
*
* ===========================================================================
*

*  FileName: CLabSettings.cpp
*  Author:   Douglas Hoffman
*
*/
#define __C_LAB_SETTINGS_CPP__
#include "CLabSettings.h"
#include <wx/arrstr.h>
// #include "CLabSettingsFileName.h"
#include "ConfigDir.h"
#include "CDirList.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxString.h"
#include "nwx/vectorSort.h"
#include "nwx/nwxXmlCmfList.h"
#include "OsirisVersion.h"

const wxString CLabSynonym::g_SYNONYM("Synonym");

const wxString CLabSettings::g_LABROOT("LabSettings");

#define __OTHER__ "Other"

nwxXmlIOint CLabReview::g_ioAccept(1);
nwxXmlIOint CLabReview::g_ioReview(2);
const wxChar * const CLabReview::g_LABELS[CLabReview::__REVIEW_COUNT] = 
{
  wxS("Default"),
  wxS("Locus"),
  wxS("Channel"),
  wxS("ILS"),
  wxS("Sample"),
  wxS("Directory")
};
nwxXmlIOintPositive CLabReview::g_io(false);


#ifdef __WXDEBUG__
int CLabNameStrings::g_nCreate = 0;
int CLabNameStrings::g_nDestroy = 0;
int CLabSettings::g_nCreate = 0;
int CLabSettings::g_nDestroy = 0;
#endif
const char * const CLabNameStrings::DEFAULT_SPECIMEN_CATEGORY(__OTHER__);
const char * const CLabNameStrings::TYPE_LADDER("Ladder");
const char * const CLabNameStrings::TYPE_POS_CONTROL("Positive Control");
const char * const CLabNameStrings::TYPE_NEG_CONTROL("Negative Control");
const char * const CLabNameStrings::TYPE_POSSIBLE_MIXTURE("Possible mixture");
const char * const CLabNameStrings::TYPE_SINGLE_SOURCE("Single source");

wxString CLabSpecimenCategory::g_sLONGEST_TYPE;


CLabSettingsInfo::~CLabSettingsInfo() {}

void CLabSettingsInfo::RegisterAll(bool)
{
  RegisterWxString("VolumeName",&m_sVolumeName);
  RegisterWxStringNotEmpty("Version",&m_sVersion);
  RegisterWxStringNotEmpty("DataFileType",&m_sDataFileType);
  RegisterWxStringNotEmpty("Protocol",&m_sProtocol);
  RegisterWxStringNotEmpty("Lot",&m_sLot);
  RegisterWxStringNotEmpty("Notes",&m_sNotes);
  RegisterWxStringNotEmpty("UserId",&m_sUserId);
  RegisterWxDateTime("LastUpdate",&m_dtLastUpdate);
}

bool CLabSettingsInfo::operator ==(const CLabSettingsInfo &x) const
{
  bool bRtn = false;
  LABEQLOG(m_sVolumeName,"CLabSettingsInfo.m_sVolumeName")
  else LABEQLOG(m_sVersion,"CLabSettingsInfo.m_sVersion.m_sVersion")
  else if (!StringEQwithDefault(m_sDataFileType,x.m_sDataFileType,CDirList::GetDefaultExt()))
  {
    LAB_LOG("CLabSettingsInfo.m_sVersion.m_sDataFileType");
  }
  else LABEQLOG(m_sProtocol,"CLabSettingsInfo.m_sProtocol")
  else LABEQLOG(m_sLot,"CLabSettingsInfo.m_sLot")
  else LABEQLOG(m_sNotes,"CLabSettingsInfo.m_sNotes")
  else
  {
    bRtn = true;
  }
  // SKIPPING
     ////// LABEQ(m_sUserId);
     ////// LABEQ(m_dtLastUpdate);
  return bRtn;
}
double CLabLocus::AlleleNumber(const wxString &s)
{
  wxString sAllele(s);
  nwxString::Trim(&sAllele);
  double d = 0.0;
  if(!sAllele.IsEmpty())
  {
    d = atof(sAllele.utf8_str());
    if(d > 0.99)
    {
      d = nwxRound::Round(d * 10);
      d *= 0.1;
    }
    else
    {
      d = 0.0;
    }
  }
  return d;
}
bool CLabLocus::ValidateAllele(wxString *ps, bool bAmel)
{
  wxString sAllele(*ps);
  nwxString::Trim(&sAllele);
  if(sAllele.IsEmpty()) {}
  else if(!bAmel)
  {
    double dAllele = AlleleNumber(sAllele);
    if(dAllele > 0.0)
    {
      sAllele = nwxString::FormatNumber(dAllele);
    }
    else
    {
      sAllele.Empty();
    }
  }
  else if(sAllele.Len() != 1)
  {
    sAllele.Empty();
  }
  else
  {
    sAllele.MakeUpper();
    wxChar c = sAllele.GetChar(0);
    if(c != 'X' && c != 'Y')
    {
      sAllele.Empty();
    }
  }
  *ps = sAllele;
  return !sAllele.IsEmpty();
}
size_t CLabLocus::BuildList(
  const wxString &_sAlleles, vector<wxString> *pvs, bool bAmel)
{
  // build a list of loci from a delimited list, usually
  // entered by a user
  wxString sAlleles(_sAlleles);
  wxString s;
  size_t nLen = sAlleles.Len();
  size_t i;
  wxChar c;
  pvs->clear();

  if(bAmel)
  {
    multiset<wxChar> sXY;
    const wxChar X('X');
    const wxChar Y('Y');
    sAlleles.MakeUpper();
    for(i = 0; i < nLen; i++)
    {
      c = sAlleles.GetChar(i);
      if((c == X) || (c == Y))
      {
        s = c;
        sXY.insert(c);
      }
    }
    size_t nSize = sXY.size();
    if(nSize)
    {
      // start with the first
      c = *sXY.begin();
      s = c;
      pvs->push_back(s);
      if(nSize > 1 && c == X)
      {
        // allow XX or XY
        // get last item, if more than 2, then skip others
        c = *sXY.rbegin();
        s = c;
        pvs->push_back(s);
      }
    }
  }
  else
  {
    const wxChar dot('.');
    const wxChar space(' ');
    for(i = 0; i < nLen; i++)
    {
      c = sAlleles.GetChar(i);
      if(!( (c == dot) || nwxString::IsNumeric(c) ))
      {
        sAlleles.SetChar(i,space);
      }
    }
    nwxString::Trim(&sAlleles);
    while( sAlleles.Replace("  "," ",true) > 0 )
    {}
    if(!sAlleles.IsEmpty())
    {
      multiset<double> sd;
      vector<wxString> vs;
      multiset<double>::iterator itrd;
      vector<wxString>::iterator itr;
      double d;
      nwxString::Split(sAlleles.utf8_str(),&vs," ");
      for(itr = vs.begin(); itr != vs.end(); ++itr)
      {
        d = AlleleNumber(*itr);
        if(d > 0)
        {
          sd.insert(d);
        }
      }
      pvs->reserve(sd.size());
      for(itrd = sd.begin(); itrd != sd.end(); ++itrd)
      {
        s = nwxString::FormatNumber(*itrd);
        pvs->push_back(s);
      }
    }
  }
  i = pvs->size();
  return i;
}

void CLabLocus::RegisterAll(bool)
{
  ClearRegistration();
  RegisterWxString("Name",&m_sName);
  Register("Allele", &m_ioAllele, (void *) &m_vsAlleles);
}

void CLabLocus::SortAlleles()
{
  vectorSort<wxString,CAlleleLess>::Sort(&m_vsAlleles);
}

bool CLabLocus::LoadFromNode(wxXmlNode *pNode)
{
  bool bRtn = nwxXmlPersist::LoadFromNode(pNode);
  ChangeDataToDisplay();
  return bRtn;
}
bool CLabLocus::LoadFromNode(wxXmlNode *pNode, void *pObj)
{
  bool bRtn = nwxXmlPersist::LoadFromNode(pNode,pObj);
  ChangeDataToDisplay();
  return bRtn;
}
nwxXmlNodeList *CLabLocus::CreateNodeList(const wxString &sNodeName)
{
  vector<wxString> vs(m_vsAlleles);
  ChangeDisplayToData();
  nwxXmlNodeList *pRtn = nwxXmlPersist::CreateNodeList(sNodeName);
  m_vsAlleles = vs;
  return pRtn;
}
nwxXmlNodeList *CLabLocus::CreateNodeList(const wxString &sNodeName, void *pObj)
{
  vector<wxString> vs(m_vsAlleles);
  ChangeDisplayToData();
  nwxXmlNodeList *pRtn = nwxXmlPersist::CreateNodeList(sNodeName,pObj);
  m_vsAlleles = vs;
  return pRtn;
}
void CLabLocus::ChangeDataToDisplay()
{
  if( IsAmel() )
  {
    for(size_t i =0; i < m_vsAlleles.size(); ++i)
    {
      wxString &sAllele(m_vsAlleles.at(i));
      sAllele.Trim(true);
      sAllele.Trim(false);
      if(sAllele == "1")
      {
        sAllele = "X";
      }
      else if(sAllele == "2")
      {
        sAllele = "Y";
      }
    }
  }
}

void CLabLocus::ChangeDisplayToData()
{
  if(IsAmel())
  {
    for(size_t i = 0; i < m_vsAlleles.size(); ++i)
    {
      wxString &sAllele(m_vsAlleles.at(i));
      sAllele.Trim(true);
      sAllele.Trim(false);
      sAllele.MakeUpper();
      if(sAllele == "X")
      {
        sAllele = "1";
      }
      else if(sAllele == "Y")
      {
        sAllele = "2";
      }
    }
  }
}

void CLabLocusCollection::ClearIncompleteData()
{
  vector<CLabLocus *> *p(Get());
  vector<CLabLocus *> vpKeep;
  vector<CLabLocus *> vpKill;
  vpKeep.reserve(p->size());
  vpKill.reserve(p->size());
  for(vector<CLabLocus *>::iterator itr = p->begin();
    itr != p->end();
    ++itr)
  {
    if((*itr)->GetAlleleCount())
    {
      vpKeep.push_back(*itr);
    }
    else
    {
      vpKill.push_back(*itr);
    }
  }
  if(vpKeep.size() < p->size())
  {
    (*p) = vpKeep;
    vectorptr<CLabLocus>::cleanup(&vpKill);
  }
}


void CLabPositiveControl::RegisterAll(bool)
{
  ClearRegistration();
  RegisterWxString("Name",&m_sSynonym);
  RegisterWxString("FileNameString",&m_sFileNameString);
  Register("Loci",&m_Loci);
}

void CLabPositiveControls::ClearIncompleteData()
{
  vector<CLabPositiveControl *> *p = Get();
  vector<CLabPositiveControl *> vKeep;
  vector<CLabPositiveControl *> vKill;
  vKeep.reserve(p->size());
  vKill.reserve(p->size());
  for(vector<CLabPositiveControl *>::iterator itr = p->begin();
    itr != p->end();
    ++itr)
  {
    (*itr)->ClearIncompleteData();
    if((*itr)->Size())
    {
      vKeep.push_back(*itr);
    }
    else
    {
      vKill.push_back(*itr);
    }
  }
  if(vKeep.size() < p->size())
  {
    (*p) = vKeep;
    vectorptr<CLabPositiveControl>::cleanup(&vKill);
  }
}

bool CLabPositiveControls::Rename(const wxString &_sOld, const wxString &_sNew)
{
  //
  //  return false if _sOld != _sNew and no positive control
  //   matching _sOld is found
  //
  //  return true otherwise
  //
  //  note: 'true' is returned if _sOld == _sNew and
  //  _sNew is not empty even if no match is found 
  //  because under this condition, no
  //  search is performed
  //
  //  If _sNew is empty, false is returned
  //
  wxString sOld(_sOld);
  wxString sNew(_sNew);
  wxString s;
  bool bRtn = false;
  nwxString::Trim(&sOld);
  nwxString::Trim(&sNew);
  if(!sNew.IsEmpty())
  {
    bRtn = (sOld == sNew);
    vector<CLabPositiveControl *> *pv = this->Get();
    for(vector<CLabPositiveControl *>::iterator itr = pv->begin();
      (!bRtn) && (itr != pv->end());
      ++itr)
    {
      s = (*itr)->GetName();
      nwxString::Trim(&s);
      if(s == sOld)
      {
        (*itr)->SetName(sNew);
        bRtn = true;
      }
    }
  }
  return bRtn;
}

const wxChar *CLabNegativeCtrlNameSet::g_psNodeName(wxS("NegativeControl"));

void CLabMarkerSetSpecifications::ClearIncompleteData()
{
  vector<CLabMarkerSetCollection *> *p(Get());
  vector<CLabMarkerSetCollection *> vKeep;
  vector<CLabMarkerSetCollection *> vKill;
  vKeep.reserve(p->size());
  vKill.reserve(p->size());

  for(vector<CLabMarkerSetCollection *>::iterator itr = p->begin();
    itr != p->end();
    ++itr)
  {
    (*itr)->ClearIncompleteData();
    if((*itr)->HasData())
    {
      vKeep.push_back(*itr);
    }
    else
    {
      vKill.push_back(*itr);
    }
  }
  if(vKeep.size() < p->size())
  {
    *p = vKeep;
    vectorptr<CLabMarkerSetCollection>::cleanup(&vKill);
  }
}

bool CLabMarkerSetCollection::HasData()
{
  bool bRtn = false;
  if(!m_sMarkerSetName.IsEmpty())
  {
    bRtn = true;
  }
  else if(!m_sLadderFileName.IsEmpty())
  {
    bRtn = true;
  }
  else if(m_lociOffLadder.Get()->size())
  {
    bRtn = true;
  }
  else if(m_lociSampleTrialleles.Get()->size())
  {
    bRtn = true;
  }
  else if(m_lociPosCtrlTrialleles.Get()->size())
  {
    bRtn = true;
  }
  else if(m_PositiveControls.Size())
  {
    bRtn = true;
  }
  return bRtn;
}

void CLabMarkerSetCollection::RegisterAll(bool)
{
  ClearRegistration();
  RegisterWxString("MarkerSetName",&m_sMarkerSetName);
  RegisterWxStringNotEmpty("LadderFileName",&m_sLadderFileName);
  RegisterWxString("ILS",&m_sILSName);
  RegisterBoolTrue("UserOverrideILS",&m_bAllowUserOverrideILS);
  RegisterBoolSkipFalse("Analyzed",&m_bAnalyzed);
  Register("PositiveControls",&m_PositiveControls);
  Register("NegativeControls",&m_NegativeControls);
  Register("OffLadderAlleles",&m_lociOffLadder);
  Register("SampleAcceptedTrialleles",&m_lociSampleTrialleles);
  Register("PosCtrlAcceptedAlleles",&m_lociPosCtrlTrialleles);
}

//CLabSetSpecimenCategory::~CLabSetSpecimenCategory()
//{
// Cleanup();
//}
bool CLabSetSpecimenCategory::_TestEquivalent(const CLabSetSpecimenCategory &x) const
{
  // test is all items in 'this' have an equivalent item x
  const_iterator itr;
  const_iterator itrx;
  CLabSpecimenCategory *p;
  CLabSpecimenCategory *px;
  bool bRtn = true;
  for(itr = begin(); bRtn && (itr != end()); ++itr)
  {
    p = *itr;
    itrx = x.find(p);
    if(itrx == x.end())
    {
      if(!p->Skip(p))
      {
        bRtn = false;
      }
    }
    else
    {
      px = *itrx;
      if((*px) != (*p))
      {
        bRtn = false;
      }
    }
  }
  return bRtn;
}
bool CLabSetSpecimenCategory::operator ==(const CLabSetSpecimenCategory &x) const
{
  bool bRtn = 
    setptr<CLabSpecimenCategory,CLabSpecimenLess>::IsEqual(
      *this,x);
  if(bRtn) {} // done
  else if(size() <= x.size())
  {
    bRtn = _TestEquivalent(x) && x._TestEquivalent(*this);
  }
  else
  {
    bRtn = x._TestEquivalent(*this) && _TestEquivalent(x);
  }
  return bRtn;
}

bool CLabSpecimenCategory::Skip(void *pObj)
{
  CLabSpecimenCategory *pSpec = ((CLabSpecimenCategory *)pObj);
  bool bRtn = pSpec->m_vsSynonym.empty() || 
    pSpec->m_sCategoryName.IsEmpty();
  return bRtn;
}


void CLabSpecimenCategory::AppendTypeArray(wxArrayString *pas, bool bClear)
{
  const std::vector<wxString> &aSpecimen = GetSpecimenCategories();
  std::vector<wxString>::const_iterator itr;
  size_t n = aSpecimen.size();
  if(bClear)
  {
    pas->Empty();
  }
  pas->Alloc(pas->GetCount() + n);
  for (itr = aSpecimen.begin();
    itr != aSpecimen.end();
    ++itr)
  {
    pas->Add(*itr);
  }
}

size_t CLabSpecimenCategory::TypeCount()
{
  const std::vector<wxString> &aSpecimen = GetSpecimenCategories();
  std::vector<wxString>::const_iterator itr;
  size_t n = aSpecimen.size();
  return n;
}

const char *CLabSpecimenCategory::LongestType()
{
  if(g_sLONGEST_TYPE.IsEmpty())
  {
    const std::vector<wxString> &aSpecimen = GetSpecimenCategories();
    std::vector<wxString>::const_iterator itr;
    std::vector<wxString>::const_iterator itrMax;
    size_t nLen;
    size_t nLenMax = 0;
    for (itr = aSpecimen.begin();
      itr != aSpecimen.end();
      ++itr)
    {
      nLen = (*itr).Length();
      if(nLen > nLenMax)
      {
        nLenMax = nLen;
        itrMax = itr;
      }
    }
    g_sLONGEST_TYPE = *itrMax;
  }
  return g_sLONGEST_TYPE.c_str();
}
const std::vector<wxString> &CLabSpecimenCategory::GetSpecimenCategories()
{
  return mainApp::GetCMFlist()->GetSpecimenCategories();
}
const char *CLabSpecimenCategory::GetType(size_t ndx)
{
  const std::vector<wxString> &aSpecimen = GetSpecimenCategories();
  const char *psRtn = NULL;
  if(ndx < aSpecimen.size())
  {
    psRtn = aSpecimen.at(ndx).c_str();
  }
  return psRtn;
}


bool CLabSpecimenCategory::IndexInRange(size_t ndx)
{
  bool bInRange = (ndx < TypeCount());
#ifdef _DEBUG
  if(!bInRange)
  {
    wxString s;
    s.Printf(
      "CLabSpecimenCategory::IndexInRange(size_t ndx), "
          "ndx (%d) is out of range",
      (int) ndx);
    wxASSERT_MSG(0,s);
  }
#endif
  return bInRange;
}
void CLabSpecimenCategory::SetName(size_t ndx)
{
  const std::vector<wxString> &aSpecimen = GetSpecimenCategories();
  if(ndx < aSpecimen.size())
  {
    m_sCategoryName = aSpecimen.at(ndx);
  }
}

void CLabSpecimenCategory::SetSynonyms(const vector<wxString> *ps)
{
  m_vsSynonym.clear();
  m_vsSynonym = *ps;
#ifdef _DEBUG
  wxASSERT_MSG(m_vsSynonym.size() == ps->size(),
    "CLabSpecimenCategory::SetSynonyms() did not copy vector");
#endif
  nwxString::Trim(&m_vsSynonym);

#if 0
  m_vsSynonym.clear();
  m_vsSynonym.reserve(ps->size());
  for(vector<wxString>::const_iterator itr = ps->begin();
    itr != ps->end();
    ++itr)
  {
    m_vsSynonym.push_back(*itr);
  }
#endif
}
void CLabSpecimenCategory::SetSynonyms(const wxString &sList)
{
  vector<wxString> vs;
  wxString s(sList);
  nwxString::Trim(&s);
  nwxString::FixEOLU(&s,true);
  nwxString::SplitLines(s.utf8_str(),&vs,true,true);
  SetSynonyms(&vs);
}

int CLabSpecimenCategory::FindSynonym(const wxString &sSampleName) const
{
  int nRtn = CLabNameStrings::FindSubstring(sSampleName, m_vsSynonym);
  return nRtn;
}

void CLabSetSpecimenCategory::SetSynonyms(
  const wxString &sName, const vector<wxString> &vsList)
{
  CLabSpecimenCategory *p(_Find(sName,true));
  p->SetSynonyms(&vsList);
}
void CLabSetSpecimenCategory::SetSynonyms(
  const wxString &sName, const wxString &sList)
{
  CLabSpecimenCategory *p(_Find(sName,true));
  p->SetSynonyms(sList);
}

wxString CLabSetSpecimenCategory::GetCategory(
  const wxString &sSample) const
{
  wxString sRtn;
  int n;
  for(const_iterator itr = begin();
    (itr != end()) && sRtn.IsEmpty();
    ++itr)
  {
    n = (*itr)->FindSynonym(sSample);
    if(n != wxNOT_FOUND)
    {
      sRtn = (*itr)->GetName();
    }
  }
  return sRtn;
}

CLabSetSpecimenCategory &CLabSetSpecimenCategory::operator = 
          (const CLabSetSpecimenCategory &x)
{
  setptr<CLabSpecimenCategory,CLabSpecimenLess>::copy(this,x);
  return *this;
}
//************************************************* CLabLocusThreshold

const double CLabLocusThreshold::DEFAULT(-1.0);
CLabLocusThreshold::IOdoubleNoMinus1 CLabLocusThreshold::g_io;

bool CLabLocusThreshold::operator ==(const CLabLocusThreshold &x) const
{
  bool bRtn = false;
  LABEQLOG(m_sName,"CLabLocusThreshold.m_sName")  
  else LABEQDLOG(m_dPullupFractionFilter,"CLabLocusThreshold.m_dPullupFractionFilter")
  else LABEQDLOG(m_dFractionMaxPeak,"CLabLocusThreshold.m_dFractionMaxPeak")
  else LABEQDLOG(m_dStutter,"CLabLocusThreshold.m_dStutter")
  else LABEQDLOG(m_dStutterRight,"CLabLocusThreshold.m_dStutterRight")
  else LABEQDLOG(m_dPlusStutter,"CLabLocusThreshold.m_dPlusStutter")
  else LABEQDLOG(m_dPlusStutterRight,"CLabLocusThreshold.m_dPlusStutterRight")
  else LABEQDLOG(m_dAdenylation,"CLabLocusThreshold.m_dAdenylation")
  else LABEQDLOG(m_dHeterozygousImbalanceLimit,"CLabLocusThreshold.m_dHeterozygousImbalanceLimit")
  else LABEQDLOG(m_dMinBoundForHomozygote,"CLabLocusThreshold.m_dMinBoundForHomozygote")
  else
  {
    bRtn = true;
  }
  return bRtn;
};
void CLabLocusThreshold::Init()
{
  wxString s(m_sName);
  nwxXmlPersist::Init();
  m_sName = s;
}
void CLabLocusThreshold::Init(void *p)
{
  CLabLocusThreshold *pp = (CLabLocusThreshold *)p;
  pp->Init();
}
bool CLabLocusThreshold::Skip() const
{
  bool bRtn = true;
  if(!m_sName.IsEmpty())
  {
    const double *pd[] =
    {
      &m_dFractionMaxPeak,
      &m_dPullupFractionFilter,
      &m_dStutter,
      &m_dStutterRight,
      &m_dPlusStutter,
      &m_dPlusStutterRight,
      &m_dAdenylation,
      &m_dHeterozygousImbalanceLimit,
      &m_dMinBoundForHomozygote
    };
    size_t N = sizeof(pd) / sizeof(pd[0]);
    const double **pp = pd;
    for(size_t i = 0; i < N; i++)
    {
      if(!g_io.Skip((void *)(*pp)))
      {
        bRtn = false;
        i = N; //loop exit
      }
      pp++;
    }
  }
  return bRtn;
}
bool CLabLocusThreshold::Skip(void *p)
{
  CLabLocusThreshold *pp = (CLabLocusThreshold *)p;
  return pp->Skip();
}

void CLabLocusThreshold::RegisterAll(bool b)
{
  if(b)
  {
    Init();
  }
  RegisterWxString("LocusName",&m_sName);
  Register("FractionOfMaxPeak",&g_io, (void *)&m_dFractionMaxPeak);
  Register("PullupFractionalFilter",&g_io, (void *)&m_dPullupFractionFilter);
  Register("StutterThreshold",&g_io, (void *)&m_dStutter);
  Register("StutterThresholdRight",CLabSettings::GetIOdoubleGt0(), (void *)&m_dStutterRight);
  Register("PlusStutterThreshold",CLabSettings::GetIOdoubleGt0(), (void *)&m_dPlusStutter);
  Register("PlusStutterThresholdRight",CLabSettings::GetIOdoubleGt0(), (void *)&m_dPlusStutterRight);
  Register("AdenylationThreshold",&g_io, (void *)&m_dAdenylation);
  Register("HeterzygousImbalanceLimit",&g_io, (void *)&m_dHeterozygousImbalanceLimit);
  Register("MinBoundForHomozygote",&g_io, (void *)&m_dMinBoundForHomozygote);
}
//************************************************* CLabSampleChannelThreshold


const int CLabSampleChannelThreshold::g_DEFAULT(-1);

void CLabSampleChannelThreshold::Init(void *p)
{
  if(p == (void *)this)
  {
    Init();
  }
  else
  {
    CLabSampleChannelThreshold *pp = (CLabSampleChannelThreshold *)p;
    pp->Init();
  }
}
void CLabSampleChannelThreshold::Init()
{
  m_nMin = g_DEFAULT;
  m_nDetection = g_DEFAULT;
}
void CLabSampleChannelThreshold::RegisterAll(bool b)
{
  if(b)
  {
    Init();
  }
  ClearRegistration();
  RegisterInt("Channel",&m_nChannel);
  Register(
    "MinimumRFU",
    CLabSettings::GetIOint(), 
    (void *)&m_nMin);
  Register(
    "MinDetectionRFU",
    CLabSettings::GetIOint(), 
    (void *)&m_nDetection);
}

bool CLabSampleChannelThreshold::Skip(void *p)
{
  CLabSampleChannelThreshold *pp = 
    (p == NULL) ? this : (CLabSampleChannelThreshold *)p;
  return pp->IsEmpty();
}
bool CLabSampleChannelThreshold::Skip()
{
  return IsEmpty();
}

//************************************************* CLabNsStutter
const double CLabNsStutter::INVALID_RATIO(-1.0);
const int CLabNsStutter::INVALID_BPS(-10000);


void CLabNsStutter::LocusStutter::Init()
{
  setptr<Threshold,ThresholdLess>::cleanup(&m_setThreshold);
  m_sLocusName.Clear();
}
double CLabNsStutter::LocusStutter::FindRatio(int nBPS) const
{
  Threshold t(nBPS);
  std::set<Threshold *,ThresholdLess>::const_iterator itr =
    m_setThreshold.find(&t);
  double dRtn = 
    (itr == m_setThreshold.end()) 
    ? CLabNsStutter::INVALID_RATIO
    : (*itr)->GetRatio();
  return dRtn;
}
void CLabNsStutter::LocusStutter::SetRatio(int nBPS,double dRatio)
{
  if(CLabNsStutter::BpsOK(nBPS))
  {
    CLabNsStutter::Threshold xt(nBPS);
    std::set<CLabNsStutter::Threshold *,CLabNsStutter::ThresholdLess>::iterator
      itr = m_setThreshold.find(&xt);
    bool bInsert = CLabNsStutter::RatioOK(dRatio);
    if(itr == m_setThreshold.end()) {}
    else if( bInsert && ((*itr)->GetRatio() == dRatio) )
    {
      bInsert = false; // valid and no change
    }
    else
    {
      m_setThreshold.erase(itr); // replace it with different ratio
    }
    if(bInsert)
    {
      m_setThreshold.insert(new CLabNsStutter::Threshold(nBPS,dRatio));
    }
  }
}
CLabNsStutter::LocusStutter &CLabNsStutter::LocusStutter::operator = (const CLabNsStutter::LocusStutter &x)
{
  setptr<Threshold,ThresholdLess>::copy(&m_setThreshold,x.m_setThreshold);
  m_sLocusName = x.m_sLocusName;
  return (*this);
}

void CLabNsStutter::Init()
{
  setptr<LocusStutter,LocusStutterLess>::cleanup(&m_data);
}
int CLabNsStutter::LocusStutter::FindAllBPS(std::vector<int> *pan) const
{
  std::set<Threshold *,ThresholdLess>::const_iterator itr;
  pan->clear();
  pan->reserve(m_setThreshold.size());
  for(itr = m_setThreshold.begin();
    itr != m_setThreshold.end();
    ++itr)
  {
    if(!(*itr)->Skip())
    {
      pan->push_back((*itr)->GetBPS());
    }
  }
  int nRtn = int(pan->size());
  return nRtn;
}
bool CLabNsStutter::LocusStutter::operator==(const CLabNsStutter::LocusStutter &x) const
{
  bool bRtn = (m_sLocusName == x.m_sLocusName) &&
        setptr<Threshold,ThresholdLess>::IsEqual(
          m_setThreshold,x.m_setThreshold);
  return bRtn;
}

int CLabNsStutter::FindAllBPS(std::vector<int> *pan) const
{
  std::vector<int> anTemp;
  std::vector<int>::iterator itrN;
  std::set<LocusStutter *,LocusStutterLess>::const_iterator itrL;
  int nRtn = 0;
  pan->clear();
  pan->reserve(m_data.size() << 2);  // reserve 4 per locus

  for(itrL = m_data.begin(); itrL != m_data.end(); ++itrL)
  {
    (*itrL)->FindAllBPS(&anTemp);
    for(itrN = anTemp.begin(); itrN != anTemp.end(); ++itrN)
    {
      pan->push_back(*itrN);
    }
  }
  if(pan->size())
  {
    vectorSort<int>::Sort(pan);
    nRtn = (int) pan->size();
  }
  return nRtn;
}
CLabNsStutter &CLabNsStutter::operator=(const CLabNsStutter &x)
{
  setptr<LocusStutter,LocusStutterLess>::copy(&m_data,x.m_data);
  return *this;
}
bool CLabNsStutter::operator == (const CLabNsStutter &x) const
{
  bool bRtn = setptr<LocusStutter,LocusStutterLess>::IsEqual(
    m_data,x.m_data);
  return bRtn;
}
double CLabNsStutter::FindRatio(const wxString &sLocus, int nBPS) const
{
  std::set<LocusStutter *,LocusStutterLess>::const_iterator itr =
    _Iterator(sLocus);
  double dRtn = (itr == m_data.end()) 
    ? CLabNsStutter::INVALID_RATIO
    : (*itr)->FindRatio(nBPS);
  return dRtn;
}
std::set<CLabNsStutter::LocusStutter *,CLabNsStutter::LocusStutterLess>::const_iterator 
    CLabNsStutter::_Iterator(const wxString &sLocus) const
{
  CLabNsStutter::LocusStutter ls(sLocus);
  std::set<LocusStutter *,LocusStutterLess>::const_iterator itr =
    m_data.find(&ls);
  return itr;
}
bool CLabNsStutter::LocusUsed(const wxString &sLocus) const
{
  std::set<LocusStutter *,LocusStutterLess>::const_iterator
    itr = _Iterator(sLocus);
  return (itr != m_data.end());
}
void CLabNsStutter::Set(const wxString &sLocus, int nBPS, double dRatio)
{
  LocusStutter x(sLocus);
  LocusStutter *px = NULL;
  std::set<LocusStutter *,LocusStutterLess>::iterator itr =
    m_data.find(&x);
  if(itr == m_data.end())
  {
    px = new LocusStutter(sLocus);
    m_data.insert(px);
  }
  else
  {
    px = *itr;
  }
  px->SetRatio(nBPS,dRatio);
}


//************************************************* CLabRFU

void CLabRFU::Init()
{
  m_apLocusThreshold.Cleanup();
  m_NsStutter.Init();
  nwxXmlPersist::Init();
  m_dPullupFractionFilter = -1.0;
  m_dFractionMaxPeak = -1.0;
  m_dStutter = 0.15;
  m_dPlusStutter = 0.15;
  m_dAdenylation = 1.0/3.0;
  m_nMin = 150;
  m_nMinInterlocus = -1;
  m_nMax = -1;
  m_nDetection = -1;
}

void CLabRFU::Init(void *p)
{
  if(p == (void *)this)
  {
    Init();
  }
  else
  {
    CLabRFU *pp = (CLabRFU *)p;
    pp->Init();
  }
}

void CLabRFU::RegisterAll(bool b)
{
  if(b)
  {
    Init();
  }
  ClearRegistration();
  Register(
    "MinimumRFU",
    CLabSettings::GetIOint(), 
    (void *)&m_nMin);
  if(m_nType == TYPE_SAMPLE || m_nType == TYPE_LADDER)
  {
    Register(
      "MinimumRFUinterlocus",
      CLabSettings::GetIOint(), 
      (void *)&m_nMinInterlocus);
  }
  RegisterInt("MaximumRFU",&m_nMax);
  if(m_nType == TYPE_SAMPLE)
  {
    Register("MinDetectionRFU",
      CLabSettings::GetIOint(), 
      (void *)&m_nDetection);
  }
  Register(
    "FractionOfMaxPeak",
      CLabSettings::GetIOdouble(),
    (void *)&m_dFractionMaxPeak);
  Register(
    "PullupFractionalFilter",
    CLabSettings::GetIOdouble(),
    (void *)&m_dPullupFractionFilter);
  Register(
    "StutterThreshold",
    CLabSettings::GetIOdouble(),
    (void *)&m_dStutter);
  Register(
    "PlusStutterThreshold",
    CLabSettings::GetIOdoubleGt0(),
    (void *)&m_dPlusStutter);
  Register(
    "AdenylationThreshold",
    CLabSettings::GetIOdouble(),
    (void *)&m_dAdenylation);
  Register(
    "LocusThreshold",
    &m_ioLocusThreshold,
    (void *)&m_apLocusThreshold);
  if(m_nType == TYPE_SAMPLE)
  {
    Register(
      "NsStutterThresholds",
      &m_NsStutter);
  }
}

bool CLabRFU::operator ==(const CLabRFU &x) const
{
  bool bRtn = false;
  LABEQLOG(m_apLocusThreshold,"CLabRFU.m_apLocusThreshold")
  else LABEQLOG(m_NsStutter,"CLabRFU.m_NsStutter")
  else LABEQDLOG(m_dPullupFractionFilter,"CLabRFU.m_dPullupFractionFilter")
  else LABEQDLOG(m_dFractionMaxPeak,"CLabRFU.m_dFractionMaxPeak")
  else LABEQDLOG(m_dStutter,"CLabRFU.m_dStutter")
  else LABEQDLOG(m_dPlusStutter,"CLabRFU.m_dPlusStutter")
  else LABEQDLOG(m_dAdenylation,"CLabRFU.m_dAdenylation")
  else LABEQLOG(m_nMin,"CLabRFU.m_nMin")
  else LABEQLOG(m_nMax,"CLabRFU.m_nMax")
  else if(m_nType == TYPE_ILS)
  {
    bRtn = true;
  }
  else LABEQLOG(m_nMinInterlocus,"CLabRFU.m_nMinInterlocus")
  else if(m_nType == TYPE_LADDER)
  {
    bRtn = true;
  }
  else LABEQLOG(m_nDetection,"CLabRFU.m_nDetection")
  else
  {
    bRtn = true;
  }
  return bRtn;
}
//*********************************************  CLabMessageSet

bool CLabMessageSet::IsEqualNoLab(const CLabMessageSet &x) const
{
  CLabMessageSet xThis(*this);
  CLabMessageSet xx(x);
  const wxChar *LIST[] =
  {
    CLabThresholds::g_psmNumberOfPullUpsInSample,
    CLabThresholds::g_psmNumberOfStuttersInSample,
    CLabThresholds::g_psmNumberOfAdenylationsInSample,
    CLabThresholds::g_psmNumberOfOffLadderAllelesInSample,
    CLabThresholds::g_psmNumberOfExcessiveResidualsInSample,
    wxS("smUseSampleNamesForControlSampleTestsPreset"), // this is a hack - this one is ignored
    NULL
  };
  for (const wxChar **pp = LIST; *pp != NULL; pp++)
  {
    xThis.RemoveMessage(*pp);
    xx.RemoveMessage(*pp);
  }
  xThis.RemoveSkipped();
  xx.RemoveSkipped();
  bool bRtn = xThis.IsEqual(xx);
  return bRtn;
}
void CLabMessageSet::RemoveSkipped()
{
  set<CLabThresholdMessage *,CLabThresholdMessageLess>::iterator itr;
  set<CLabThresholdMessage *,CLabThresholdMessageLess>::iterator itrNext;
  set<CLabThresholdMessage *,CLabThresholdMessageLess> *pSet = Get();
  vector<CLabThresholdMessage *> vp;
  vector<CLabThresholdMessage *>::iterator itrv;
  CLabThresholdMessage *pMsg;
  vp.reserve(pSet->size());
  bool bBegin = false;
  for(itr = itrNext = pSet->begin(); 
    itr != pSet->end();
    itr = itrNext)
  {
    bBegin = false;    
    if((*itr)->Skip(*itr))
    {
      bBegin = (itr == pSet->begin());
      if(!bBegin)
      {
        --itrNext;
      }
      pMsg = *itr;
      pSet->erase(itr);
      delete pMsg; // must delete AFTER erase
      if(pSet->empty())
      {
        break;
      }
    }
    if(bBegin)
    {
      itrNext = pSet->begin();
    }
    else
    {
      ++itrNext;
    }
  }
}


//*********************************************  CLabThresholdMessage

CLabThresholdMessage::~CLabThresholdMessage() {}
bool CLabThresholdMessage::Skip(void *)
{
  // this needs to be changed to compare to the default
//  CLabThresholdMessage *ptm = (p == NULL) ? this : (CLabThresholdMessage *)p;
//  return (ptm->m_nValue == -1);
  return false;
}
void CLabThresholdMessage::RegisterAll(bool)
{
  RegisterWxString("MsgName",&m_sName);
  RegisterInt("MsgThreshold",&m_nValue);
}
const wxChar * const CLabThresholds::g_psmNumberOfPullUpsInSample
  (wxS("smNumberOfPullUpsInSample"));
const wxChar * const CLabThresholds::g_psmNumberOfStuttersInSample
  (wxS("smNumberOfStuttersInSample"));
const wxChar * const CLabThresholds::g_psmNumberOfAdenylationsInSample
  (wxS("smNumberOfAdenylationsInSample"));
const wxChar * const CLabThresholds::g_psmNumberOfOffLadderAllelesInSample
  (wxS("smNumberOfOffLadderAllelesInSample"));
const wxChar * const CLabThresholds::g_psmNumberOfExcessiveResidualsInSample
  (wxS("smNumberOfExcessiveResidualsInSample"));

void CLabThresholds::__SetupNameInt()
{
  pair<wxString, int *> mapItem;
#define INSERT_PAIR(ps,n) \
  mapItem.first = ps; \
  mapItem.second = &n; \
  m_mapNameInt.insert(mapItem);

  INSERT_PAIR(
      g_psmNumberOfPullUpsInSample,
      m_nMaxNumberOfPullupsPerSample
      );
  INSERT_PAIR(
      g_psmNumberOfStuttersInSample,
      m_nMaxNumberOfStutterPeaksPerSample
      );
  INSERT_PAIR(
      g_psmNumberOfAdenylationsInSample,
      m_nMaxNumberOfAdenylationsPerSample
      );
  INSERT_PAIR(
      g_psmNumberOfOffLadderAllelesInSample,
      m_nMaxNumberOfOLAllelesPerSample
      );
  INSERT_PAIR(
      g_psmNumberOfExcessiveResidualsInSample,
      m_nMaxExcessiveResidual
      );
#undef INSERT_PAIR
}

void CLabThresholds::SetValue(
  const wxString &sMsgName,
  int nValue,
  bool bCreateIfNeeded)
{
  CLabThresholdMessage x(sMsgName,nValue);
  set<CLabThresholdMessage *,CLabThresholdMessageLess> *ps = m_spMsgs.Get();
  set<CLabThresholdMessage *,CLabThresholdMessageLess>::iterator itrs =
    ps->find(&x);
  CLabThresholdMessage *pT;
  bool bFound = (itrs != ps->end());
  if(x.Skip(&x))
  {
    if(bFound)
    {
      pT = *itrs;
      ps->erase(itrs);
      delete pT;
    }
  }
  else if(!bFound)
  {
    if(bCreateIfNeeded)
    {
      ps->insert(x.Clone());
    }
  }
  else
  {
    (*itrs)->SetThreshold(x.GetThreshold());
  }
  map<wxString,int *>::iterator itr = m_mapNameInt.find(sMsgName);
  if(itr != m_mapNameInt.end())
  {
    *(itr->second) = nValue;
  }
}
int CLabThresholds::GetValue(const wxString &sMsgName, int nDefault) const
{
  CLabThresholdMessage x(sMsgName,0);
  const set<CLabThresholdMessage *,CLabThresholdMessageLess> *ps = m_spMsgs.Get();
  set<CLabThresholdMessage *,CLabThresholdMessageLess>::const_iterator itrs =
    ps->find(&x);
  int nRtn = nDefault;
  if(itrs == ps->end())
  {
    map<wxString,int *>::const_iterator itr = m_mapNameInt.find(sMsgName);
    if(itr != m_mapNameInt.end())
    {
      // this should never happen
      nRtn = *(itr->second);
    }
  }
  else
  {
    nRtn = (int)(*itrs)->GetThreshold();
  }
  return nRtn;
}
void CLabThresholds::SetupCompatibility()
{
  set<CLabThresholdMessage *,CLabThresholdMessageLess>::iterator itrs;
  set<CLabThresholdMessage *,CLabThresholdMessageLess> *pSet =
    m_spMsgs.Get();
  for(map<wxString,int *>::iterator itr = m_mapNameInt.begin();
    itr != m_mapNameInt.end();
    ++itr)
  {
    CLabThresholdMessage x(itr->first,*(itr->second));
    itrs = pSet->find(&x);
    if(itrs != pSet->end())
    {
      // copy value from message set
      *(itr->second) = (int)(*itrs)->GetThreshold();
    }
    else
    {
      // otherwise invalidate it
      *(itr->second) = -1;
    }
  }
}
/*
void CLabThresholds::SetupMessages()
{
  // use m_mapNameInt
  //  after loading from a file, if old file version, create messages
  //  from the five parameters that were converted to messages
  //  otherwise, copy data from messages to the five parameters'
  //
  wxString sMsg;
  set<CLabThresholdMessage *,CLabThresholdMessageLess> *pSet =
    m_spMsgs.Get();
  set<CLabThresholdMessage *,CLabThresholdMessageLess>::iterator itrs;
  for(map<wxString,int *>::iterator itr = m_mapNameInt.begin();
    itr != m_mapNameInt.end();
    ++itr)
  {
    CLabThresholdMessage x(itr->first,*(itr->second));
    itrs = pSet->find(&x);
    if(itrs != pSet->end())
    {
      // copy value from message set
      *(itr->second) = (int)(*itrs)->GetThreshold();
    }
    else if(*(itr->second) > 0)
    {
      // copy to message set
      pSet->insert(x.Clone());
    }
  }
}
*/
void CLabThresholds::Init()
{
  nwxXmlPersist::Init();
  m_sMinBoundHomozygoteUnit.Empty();
  m_dHeterozygousImbalanceLimit = -1.0;
  m_dMinBoundForHomozygote = -1.0;
  m_dMaxResidualForAlleleCall = -1.0;
  m_nMaxNumberOfPullupsPerSample = -1;
  m_nMaxNumberOfStutterPeaksPerSample = -1;
  m_nMaxNumberOfSpikesPerSample = -1;
  m_nMaxNumberOfAdenylationsPerSample = -1;
  m_nMaxNumberOfOLAllelesPerSample = -1;
  m_nMinBPSForArtifacts = -1;
  m_nMaxRFUForIncompleteSample = -1;
  m_nMaxExcessiveResidual = -1;
  m_spMsgs.Clear();
}
void CLabThresholds::Init(void *p)
{
  if(p == (void *)this)
  {
    Init();
  }
  else
  {
    nwxXmlPersist *pp = (nwxXmlPersist *)p;
    pp->Init();
  }
}

void CLabThresholds::RegisterAll(bool bFirst)
{
  if(bFirst)
  {
    SetupNameInt();
  }
  ClearRegistration();
  Register("LadderRFUTests",&m_rfuLadder);
  Register("LaneStandardRFUTests",&m_rfuLS);
  Register("SampleRFUTests",&m_rfuSample);
  Register("ChannelThresholds",&m_setChannelThresholds);

  Register(
    "HeterozygousImbalanceLimit",
    CLabSettings::GetIOdouble(),
    (void *)&m_dHeterozygousImbalanceLimit);
  Register(
    "MinBoundForHomozygote",
    CLabSettings::GetIOdouble(),
    (void *)&m_dMinBoundForHomozygote);
  RegisterWxString(
    "MinBoundHomozygoteUnit",&m_sMinBoundHomozygoteUnit);
  RegisterIntPositive(
    "MaxNumberOfPullupsPerSample",&m_nMaxNumberOfPullupsPerSample);
  RegisterIntPositive(
    "MaxNumberOfStutterPeaksPerSample",&m_nMaxNumberOfStutterPeaksPerSample);
  RegisterIntPositive(
    "MaxNumberOfSpikesPerSample",&m_nMaxNumberOfSpikesPerSample);
  RegisterIntPositive(
    "MaxNumberOfAdenylationsPerSample",&m_nMaxNumberOfAdenylationsPerSample);
  RegisterIntPositive(
    "MaxNumberOfOLAllelesPerSample",&m_nMaxNumberOfOLAllelesPerSample);
  Register(
    "MaxResidualForAlleleCall",
    CLabSettings::GetIOdouble(),
    (void *)&m_dMaxResidualForAlleleCall);
  RegisterIntPositive(
    "MaxExcessiveResidual",
    &m_nMaxExcessiveResidual);    

  RegisterIntPositive(
    "MinBPSForArtifacts",&m_nMinBPSForArtifacts);
  RegisterIntPositive(
      "AlleleRFUOverloadThreshold",
      &m_nMaxRFUForIncompleteSample);

  RegisterBoolTrue("AllowMinRFUOverride",&m_bAllowMinRFUoverride);

  Register("SmartMessageThresholds",&m_spMsgs);

}
bool CLabThresholds::operator ==(const CLabThresholds &x) const
{
  bool bRtn = false;
  LABEQLOG(m_sMinBoundHomozygoteUnit,"CLabThresholds.m_sMinBoundHomozygoteUnit")
  else LABEQLOG(m_setChannelThresholds,"CLabThresholds.m_setChannelThresholds")
  else LABEQLOG(m_rfuLadder,"CLabThresholds.m_rfuLadder")
  else LABEQLOG(m_rfuLS,"CLabThresholds.m_rfuLS")
  else LABEQLOG(m_rfuSample,"CLabThresholds.m_rfuSample")
  else LABEQLOG(m_dHeterozygousImbalanceLimit,"CLabThresholds.m_dHeterozygousImbalanceLimit")
  else LABEQLOG(m_dMinBoundForHomozygote,"CLabThresholds.m_dMinBoundForHomozygote")
  else LABEQLOG(m_dMaxResidualForAlleleCall,"CLabThresholds.m_dMaxResidualForAlleleCall")
  else LABEQLOG(GetMaxNumberOfPullupsPerSample(),"CLabThresholds.m_nMaxNumberOfPullupsPerSample")
  else LABEQLOG(GetMaxNumberOfStutterPeaksPerSample(),"CLabThresholds.m_nMaxNumberOfStutterPeaksPerSample")
  else LABEQLOG(m_nMaxNumberOfSpikesPerSample,"CLabThresholds.m_nMaxNumberOfSpikesPerSample")
  else LABEQLOG(GetMaxNumberOfAdenylationsPerSample(),"CLabThresholds.m_nMaxNumberOfAdenylationsPerSample")
  else LABEQLOG(GetMaxNumberOfOLAllelesPerSample(),"CLabThresholds.m_nMaxNumberOfOLAllelesPerSample")
  else LABEQLOG(m_nMinBPSForArtifacts,"CLabThresholds.m_nMinBPSForArtifacts")
  else LABEQLOG(m_nMaxRFUForIncompleteSample,"CLabThresholds.m_nMaxRFUForIncompleteSample")
  else LABEQLOG(GetMaxExcessiveResidual(),"CLabThresholds.m_nMaxExcessiveResidual")
  else LABEQLOG(m_bAllowMinRFUoverride,"CLabThresholds.m_bAllowMinRFUoverride")
  else if(!m_spMsgs.IsEqualNoLab(x.m_spMsgs))
  {
    LAB_LOG("CLabThresholds.m_spMsgs");
  }
  else
  {
    bRtn = true;
  }

  return bRtn;
}

int CLabNameStrings::FindSubstring(
  const wxString &s,
  const vector<wxString> &vs)
{
  int nRtn = wxNOT_FOUND;
  int ndx = 0;
  for(vector<wxString>::const_iterator itr = vs.begin();
    (itr != vs.end()) && (nRtn == wxNOT_FOUND);
    ++itr)
  {
    const wxString &sub(*itr);
    if(nwxString::ContainsNoCase(s,sub))
    {
      nRtn = ndx;
    }
    ndx++;
  }
  return nRtn;
}

wxString CLabNameStrings::GetCategory(
  const wxString &sSampleName) const
{
  wxString sSampleLower(sSampleName);
  sSampleLower.MakeLower();
  wxString sRtn;
  const CLabSynonym *psynList[] =
  {
    &m_vsSynonymLadder,
    &m_vsSynonymPosCtrl,
    &m_vsSynonymNegCtrl,
    &m_vsSynonymPossibleMixture,
    &m_vsSynonymSingleSource
  };
  const char *cat[] =
  {
    TYPE_LADDER,
    TYPE_POS_CONTROL,
    TYPE_NEG_CONTROL,
    TYPE_POSSIBLE_MIXTURE,
    TYPE_SINGLE_SOURCE
  };
  size_t nSize = sizeof(psynList) / sizeof(psynList[0]);
  size_t i;
  for(i = 0; i < nSize; i++)
  {
    if(FindSubstring(sSampleLower,psynList[i]->GetVector()) != wxNOT_FOUND)
    {
      sRtn = cat[i];
      i = nSize; // loop exit
    }
  }
  if(sRtn.IsEmpty())
  {
    sRtn = m_setSpecimenCategory.GetCategory(sSampleLower);
  }
  return sRtn;
}

void CLabNameStrings::RegisterAll(bool)
{
  ClearRegistration();
  Register("LadderStrings",&m_vsSynonymLadder);
  Register("StdPositveControlStrings",&m_vsSynonymPosCtrl);
  Register("NegativeControlStrings",&m_vsSynonymNegCtrl);
  Register("SingleSourceStrings",&m_vsSynonymSingleSource);
  Register("PossibleMixtureStrings",&m_vsSynonymPossibleMixture);
  RegisterWxString("StandardControlName",&m_sStdCtrlName);
  Register(
    "SpecimenCategory",
    &m_ioSpecimenCategory,
    (void *)&m_setSpecimenCategory);
  RegisterBool("UseSampleName",&m_bUseSampleName);
}
//********************************************************  CLabReview

void CLabReview::RegisterAll(bool)
{
  wxString s;
  const wxChar *ACCEPTANCE(wxS("Acceptance"));
  const wxChar *REVIEWERS(wxS("Reviews"));
  const wxChar *BASE[] = { ACCEPTANCE, REVIEWERS };
  const wxChar *pBASE;
  InwxXmlIO *pIO[] = { &g_ioAccept, &g_ioReview };
  int *ppn[] = { &m_nAcceptance[0], &m_nReviewers[0] };
  int *pn;
  wxChar N;

  for(int j = 0; j < 2; j++)
  {
    pn = ppn[j];
    pBASE = BASE[j];
    Register(pBASE,pIO[j],(void *) pn);
    for(int i = 1; i < __REVIEW_COUNT; i++)
    {
      N = wxChar('0') + i;
      s = pBASE;
      s.Append(N);
      ++pn;
      Register(s,&g_io,(void *) pn);
    }
  }
  RegisterBoolSkipFalse(
    "AllowUserNameOverride",&m_bAllowUserNameOverride);
  RegisterBoolSkipFalse(
    "AllowExportWithAttnReqd",&m_bAllowExportWithAttnReqd);
}

//********************************************************  CLabSettings

bool CLabSettings::SaveFile(const wxString &sFileName)
{
  CLabSettingsInfo *pInfo = GetLabSettingsInfo();
  CLabSettingsInfo infoBackup(*pInfo);
  pInfo->UpdateTime();
  pInfo->SetUserID();
  pInfo->SetVersion(OSIRIS_VERSION);
  bool bRtn = nwxXmlPersist::SaveFile(sFileName);
  if(!bRtn)
  {
    *pInfo = infoBackup;
  }
  return bRtn;
}

/*
bool CLabSettings::Load()
{
  CLabSettingsFileName fn;
  bool bRtn = fn.IsReadable() && LoadFile(fn);
  if(bRtn)
  {
    // setup messages if loading older file version
    this->m_thresholds.SetupMessages();
  }
  return bRtn;
}
*/
void CLabSettings::RegisterAll(bool)
{
  ClearRegistration();
  Register("Info",&m_info);
  Register("NameStrings",&m_str);
  Register("Thresholds",&m_thresholds);
  Register("LabMarkerSetSpecifications",&m_marker);
  Register("ReviewAcceptanceCounts",&m_review);
}
bool CLabSettings::operator ==(const CLabSettings &x) const

{
  bool bRtn = false;
  LABEQLOG(m_info,"CLabSettings.m_info")
  else LABEQLOG(m_str,"CLabSettings.m_str")
  else LABEQLOG(m_thresholds,"CLabSettings.m_thresholds")
  else LABEQLOG(m_marker,"CLabSettings.m_marker")
  else LABEQLOG(m_review,"CLabSettings.m_review")
  else
  {
    bRtn = true;
  }
  return bRtn;
}

nwxXmlIOdouble CLabSettings::g_xmlIOdouble(-1.0);
nwxXmlIOintPositive CLabSettings::g_xmlIOint;
nwxXmlIOdoublePositive CLabSettings::g_xmlIOdoubleGt0(true);
                                        // true == non zero
void CLabSettings::GetMinRFU(
    int *pnSample, int *pnILS, int *pnLadder, 
    int *pnInterlocus, int *pnLadderInterlocus,
    int *pnSampleDetection,
    vector<int> *m_panChannelRFU,
    vector<int> *m_panChannelDetection) const
{
  *pnSample = m_thresholds.GetRFUsample()->GetMinRFU();
  *pnInterlocus = m_thresholds.GetRFUsample()->GetMinRFUinterlocus();
  *pnSampleDetection = m_thresholds.GetRFUsample()->GetMinDetection();

  *pnLadder = m_thresholds.GetRFUladder()->GetMinRFU();
  *pnLadderInterlocus = m_thresholds.GetRFUladder()->GetMinRFUinterlocus();

  *pnILS    = m_thresholds.GetRFUls()->GetMinRFU();
  const set<CLabSampleChannelThreshold *,CLabSampleChannelThresholdLess> 
    *psetChan = m_thresholds.GetChannelThresholds()->Get();
    
  const CLabSampleChannelThreshold *plab;
  m_panChannelRFU->clear();
  m_panChannelDetection->clear();
  set<CLabSampleChannelThreshold *,CLabSampleChannelThresholdLess>::const_reverse_iterator
      itr = psetChan->rbegin();
  if(itr != psetChan->rend())
  {
    plab = *itr;
    size_t n = (size_t) plab->GetChannel();
    m_panChannelRFU->reserve(n);
    m_panChannelDetection->reserve(n);
    for(; itr != psetChan->rend(); ++itr)
    {
      plab = *itr;
      n = (size_t) plab->GetChannel();
      while(m_panChannelDetection->size() < n)
      {
        m_panChannelDetection->push_back(-1);
        m_panChannelRFU->push_back(-1);
      }
      --n;
      m_panChannelDetection->at(n) = plab->GetDetection();
      m_panChannelRFU->at(n) = plab->GetMin();
    }
  }
}


#ifdef __WXDEBUG__

void CLabSettings::UnitTest()
{
  //  THIS IS NOT A THOROUGH TEST

  ConfigDir *pDir = mainApp::GetConfig();
  wxString sPath = pDir->GetExeVolumePath();
  wxString sNew = pDir->GetExePath();
  wxString sNew2;
  wxString sError;

  nwxFileUtil::EndWithSeparator(&sNew);
  sNew2 = sNew;
  sNew.Append("testlab.xml");
  sNew2.Append("testlab2.xml");
  nwxFileUtil::EndWithSeparator(&sPath);
  sPath.Append("ID");
  nwxFileUtil::EndWithSeparator(&sPath);
  sPath.Append("ID_LabSettings.xml");
  CLabSettings x;
  
  if(!x.LoadFile(sPath))
  {
    sError.Append("\nCLabSettings::UnitTest() cannot load file: ");
    sError.Append(sPath);
  }
  else
  {
    CLabSettings y(x);
    CLabSettings z;
    ::wxRemoveFile(sNew); // remove old copies of temp files
    ::wxRemoveFile(sNew2);
    if(y != x)
    {
      sError.Append("\nCopy of lab settings failed");
    }
    if(!y.SaveFile(sNew))
    {
      sError.Append("\nCannot save test lab settings file: ");
      sError.Append(sNew);
    }
    else if(!z.LoadFile(sNew))
    {
      sError.Append("\nCannot load test lab settings file: ");
      sError.Append(sNew);
    }
    else if(!z.SaveFile(sNew2))
    {
      sError.Append("\nCannot save test lab settings file: ");
      sError.Append(sNew2);
    }
    else 
    {
      x.GetLabSettingsInfo()->SetVersion(OSIRIS_VERSION);
      if(x != z)
      {
        sError.Append("\nSave and restore of lab settings yielded different results");
      }
    }
  }
  wxASSERT_MSG(sError.IsEmpty(),sError);
}


#endif
