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
*  FileName: CPlotData.cpp
*  Author:   Douglas Hoffman
*  Purpose: read and parse OSIRIS plot (.plt) files.
*
*  the following code in 
*  void CPlotChannel::RegisterAll(bool bInConstructor)
*  should eventually be deprecated. 
* BEGIN CODE
*   // the following two lines are for v2.2 beta 2 compatibility
*   // and should eventually be deprecated
*   RegisterUint(_T("BaselineStart"),&m_nBaselineStartObsolete);
*   RegisterIntVector(_T("BaselinePoints"),&m_vnBaselinePointsObsolete);
* END CODE
*
*  At the same time class variables w/ names ending with "Obsolete" and 
*    functions w/ names containing "FixBaseline"
*    should be removed along with their references
*
*
*
*/
#include "mainApp.h"
#include <wx/dir.h>
#include "CPlotData.h"
#include "COARfile.h"
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

const wxString g_TagRawPoints();
const wxString g_TagAnalyzedPoints();
const wxString g_TagLadderPoints(_T("ladderPoints"));
const wxString g_TagNr(_T("nr"));

void CArtifact::RegisterAll(bool bInConstructor)
{
  RegisterInt(_T("level"),&m_nLevel);
  RegisterDouble(_T("mean"),&m_dMean);
  RegisterInt(_T("height"),&m_nHeight);
  RegisterDouble(_T("meanbps"),&m_dMeanBPS);
  RegisterDouble(_T("fit"),&m_dFit);
  RegisterWxString(_T("label"),&m_sLabel);
  RegisterWxString(_T("equivAllele"),&m_sEquivAllele);

  if(bInConstructor)
  {
    Init();
  }
}


void CSamplePeak::RegisterAll(bool bInConstructor)
{
  RegisterDouble(_T("mean"),&m_dMean);
  RegisterInt(_T("height"),&m_nHeight);
  RegisterDouble(_T("meanbps"),&m_dMeanBps);
  RegisterDouble(_T("BPS"),&m_dBPS);
  RegisterWxString(_T("allele"),&m_sAllele);
  RegisterDoubleM1(_T("fit"),&m_dFit);
  if(bInConstructor)
  {
    Init();
  }
}

void CSamplePeak::FixAmel(int nStart, int nEnd)
{
  int nMean = GetMean();
  if( (nMean >= nStart) &&
      (nMean <= nEnd) )
  {
    wxString s(m_sAllele);
    s.Trim(true);
    s.Trim(false);
    if(s.Len() == 1)
    {
      m_sAllele.Replace(_T("1"),_T("X"));
      m_sAllele.Replace(_T("2"),_T("Y"));
    }
  }
}
#if BASELINE_START
/*
void MINMAX_RFU::Setup(const vector<int> &vn, int nStart_)
{
  if((vn.size() > 0) && !IsSetup())
  {
    size_t nSize = vn.size();
    size_t nStart =
      ((nStart_ < 0) || (nStart_ > (int)nSize))
      ? 0 : (size_t)nStart_;
    size_t i;
    int n;
    (nStart >= nSize) && (nStart = 0);
    m_nMinAll = vn.at(0);
    m_nMaxAll = m_nMinAll;
    m_nMinFromStart = vn.at(nStart);
    m_nMaxFromStart =  m_nMinFromStart;
    for(i = 1; i <= nStart; i++)
    {
      n = vn.at(i);
      (n < m_nMinAll) && (m_nMinAll = n);
      (n > m_nMaxAll) && (m_nMaxAll = n);
    }
    for(i = nStart + 1; i < nSize; i++)
    {
      n = vn.at(i);
      (n < m_nMinAll) && (m_nMinAll = n);
      (n < m_nMinFromStart) && (m_nMinFromStart = n);
      (n > m_nMaxAll) && (m_nMaxAll = n);
      (n > m_nMaxFromStart) && (m_nMaxFromStart = n);
    }
  }
}
*/
#endif
void CPlotChannel::FixBaseline()
{
  if(m_nBaselineStartObsolete > 0)
  {
    m_nBaselineStart = m_nBaselineStartObsolete;
    m_nBaselineStartObsolete = 0;
  }
  if(m_vnBaselinePointsObsolete.size())
  {
    m_vnBaselinePoints = m_vnBaselinePointsObsolete;
    m_vnBaselinePointsObsolete.clear();
  }
}
void CPlotChannel::FixAmel(int nStart, int nEnd)
{
  vector<CSamplePeak *>::iterator itr;
  vector<CSamplePeak *> *pv[] = {&m_vSamplePeak,&m_vLadderPeak};
  vector<CSamplePeak *> *pp;
  size_t COUNT = sizeof(pv) / sizeof(pv[0]);
  for(size_t i = 0; i < COUNT; i++)
  {
    pp = pv[i];
    for(itr = pp->begin();
      itr != pp->end();
      ++itr)
    {
      (*itr)->FixAmel(nStart,nEnd);
    }
  }
}

void CPlotChannel::RegisterAll(bool bInConstructor)
{
  RegisterInt(_T("nr"),&m_nr);
  RegisterDouble(_T("minRFU"),&m_dMinRfu);
  RegisterIntVector(_T("rawPoints"),&m_vnRawPoints);
  RegisterIntVector(_T("analyzedPoints"),&m_vnAnalyzedPoints);
  RegisterIntVector(_T("ladderPoints"),&m_vnLadderPoints);
  RegisterUint(_T("baselineStart"),&m_nBaselineStart);
  RegisterIntVector(_T("baselinePoints"),&m_vnBaselinePoints);
  // the following two lines are for v2.2 beta 1 compatibility
  // and should eventually be deprecated
  RegisterUint(_T("BaselineStart"),&m_nBaselineStartObsolete);
  RegisterIntVector(_T("BaselinePoints"),&m_vnBaselinePointsObsolete);
  Register(_T("samplePeak"),&m_IOpeak,(void *)&m_vSamplePeak);
  Register(_T("artifact"),&m_IOartifact,(void *)&m_vArtifact);
  Register(_T("ladderPeak"),&m_IOpeak,(void *)&m_vLadderPeak);
  if(bInConstructor)
  {
    Init();
  }
}

size_t CPlotChannel::GetPointCount()
{
  if(!m_nPointCount)
  {
    size_t n;
    n = m_vnRawPoints.size();
    if(n > m_nPointCount) { m_nPointCount = n;}
    n = m_vnAnalyzedPoints.size();
    if(n > m_nPointCount) { m_nPointCount = n;}
    n = m_vnLadderPoints.size();
    if(n > m_nPointCount) { m_nPointCount = n;}
    n = m_vnBaselinePoints.size();
    if(n > m_nPointCount) { m_nPointCount = n;}
  }
  return m_nPointCount;
}

void CPlotChannel::BuildList(vector<int> &vn, double **p, size_t nPointCount)
{
  size_t i = nPointCount * sizeof(double);
  double *pd = (double *)malloc(i);
  memset(pd,0,i);
  *p = pd;
  size_t nSize = vn.size();
  if(nSize > nPointCount)
  {
    nSize = nPointCount;
  }
  for(i = 0; i < nSize; i++)
  {
    (*pd) = double(vn.at(i));
    pd++;
  }
}
#if BASELINE_START
double *CPlotChannel::GetBaselineX()
{
  size_t nSize = m_vnBaselinePoints.size();
  if(nSize && (m_pdBaselineX == NULL))
  {
    size_t i;
    unsigned int n = m_nBaselineStart;
    i = nSize * sizeof(double);
    m_pdBaselineX = (double *) malloc(i);
    memset(m_pdBaselineX,0,i);
    double *pd = m_pdBaselineX;
    for(i = 0; i < nSize; i++)
    {
      (*pd) = double(n);
      n++;
      pd++;
    }
  }
  return m_pdBaselineX;
}
#endif
const int CPlotLocus::MARGIN = 10;

void CPlotLocus::RegisterAll(bool bInConstructor)
{
  RegisterUint(_T("channel"),&m_nChannel);
  RegisterInt(_T("start"),&m_nStart);
  RegisterInt(_T("end"),&m_nEnd);
  RegisterInt(_T("startExtended"),&m_nStartExtended);
  RegisterInt(_T("endExtended"),&m_nEndExtended);
  RegisterWxString(_T("name"),&m_sName);
  if(bInConstructor)
  {
    Init();
  }
}

CPlotData::~CPlotData()
{
  _Cleanup();
}

void CPlotData::_Cleanup()
{
  FREEPTR(m_pdX);
  m_mapChannels.clear();
  m_nPointCount = 0;
  m_IOchannel.Cleanup();
  m_IOlocus.Cleanup();
}
void CPlotData::RegisterAll(bool bInConstructor)
{
  RegisterWxString(_T("filename"),&m_sFilename);
  RegisterWxString(_T("associatedLadder"),&m_sLadder);
  RegisterUint(_T("start"),&m_nStart);
  RegisterUint(_T("begin"),&m_nBegin);
  RegisterUint(_T("interval"),&m_nInterval);
  RegisterUint(_T("end"),&m_nEnd);
  RegisterWxString(_T("kit"),&m_sKit);
  RegisterUint(_T("ilsChannel"),&m_nILS);
  Register(_T("channel"),&m_IOchannel,(void *)&m_vChannels);
  Register(_T("locus"),&m_IOlocus,(void *)&m_vLocus);
  Register(_T("parameters"),&m_parm,(void *)&m_parm);
  if(bInConstructor)
  {
    Init();
  }
}
void CPlotData::_FixBaseline()
{
  map<unsigned int,CPlotChannel *> ::iterator itr;
  for(itr = m_mapChannels.begin();
    itr != m_mapChannels.end();
    ++itr)
  {
    itr->second->FixBaseline();
  }
}
bool CPlotData::LoadFromNode(wxXmlNode *pNode)
{
  bool bRtn = nwxXmlPersist::LoadFromNode(pNode);
  if(bRtn)
  {
    _FixBaseline();
    // find amel and change 1,2 to X,Y
    wxString sName;
    CPlotLocus *pLocus = NULL;
    CPlotChannel *pChan = NULL;
    for(vector<CPlotLocus *>::iterator itr = m_vLocus.begin();
      (pLocus == NULL) && (itr != m_vLocus.end());
      ++itr)
    {
      sName = (*itr)->GetName();
      sName.Trim(false);
      sName.MakeUpper();
      if(sName.StartsWith(_T("AMEL")))
      {
        pLocus = *itr;
        pChan = FindChannel(pLocus->GetChannel());
        pChan->FixAmel(pLocus->GetStartPlus(),pLocus->GetEndPlus());
      }
    }
  }
  return bRtn;
}

size_t CPlotData::GetPointCount()
{
  if( (!m_nPointCount) && (m_nInterval > 0) &&
    (m_nStart < m_nEnd) )
  {
    size_t n;
    for(vector<CPlotChannel *>::iterator itr = m_vChannels.begin();
      itr != m_vChannels.end();
      ++itr)
    {
      n = (*itr)->GetPointCount();
      if(n > m_nPointCount ) { m_nPointCount = n;}
    }
  }
  return m_nPointCount;
}

bool CPlotData::HasBaseline()
{
  bool bRtn = false;
  for(vector<CPlotChannel *>::iterator itr = m_vChannels.begin();
    (!bRtn) && (itr != m_vChannels.end());
    ++itr)
  {
    bRtn = (*itr)->HasBaseline();
  }
  return bRtn;
}
#if BASELINE_START
size_t CPlotData::GetBaselinePointCount(unsigned int nChannel)
{
  CPlotChannel *pChannel = FindChannel(nChannel);
  size_t nRtn =
    (pChannel == NULL)
    ? 0
    : pChannel->GetBaselinePointCount();
  return nRtn;
}
double *CPlotData::GetBaselineTimePoints(unsigned int nChannel)
{
  CPlotChannel *pChannel = FindChannel(nChannel);
  double *pdRtn =
    (pChannel == NULL)
    ? NULL
    : pChannel->GetBaselineX();
  return pdRtn;
}
unsigned int CPlotData::GetBaselineStart(unsigned int nChannel)
{
  CPlotChannel *pChannel = FindChannel(nChannel);
  unsigned int nRtn =
    (pChannel == NULL)
    ? 0
    : pChannel->GetBaselineStart();
  return nRtn;
}
#endif

double *CPlotData::GetTimePoints()
{
  size_t nPoints = GetPointCount();
  if((nPoints > 0) && (m_pdX == NULL))
  {
    double *p;
    unsigned int i;
    unsigned int nValue = m_nStart;
    size_t nSize = nPoints * sizeof(double);
    m_pdX = (double *)malloc(nSize);
    memset(m_pdX,0,nSize);
    p = m_pdX;
    for(i = 0; i < nPoints; i++)
    {
      (*p) = double(nValue);
      nValue += m_nInterval;
      p++;
    }
  }
  return m_pdX;
}

CPlotChannel *CPlotData::FindChannel(unsigned int n)
{
  CPlotChannel *pRtn(NULL);
  size_t nSize = m_vChannels.size();
  map<unsigned int,CPlotChannel *>::iterator itr;
  if(m_mapChannels.empty() && (n < nSize))
  {
    pRtn = m_vChannels.at(n);
    if(pRtn->m_nr != (int)n)
    {
      CPlotChannel *p;
      // not ordered, build map
      pRtn = NULL;
      for(size_t i = 0; i < nSize; i++)
      {
        p = m_vChannels.at(i);
        itr = m_mapChannels.find(p->m_nr);
        if(itr != m_mapChannels.end())
        {
          m_mapChannels.erase(itr);
        }
        m_mapChannels.insert(
          map<unsigned int,CPlotChannel *>::value_type(
            p->m_nr,p));
      }
    }
  }
  if(pRtn == NULL)
  {
    itr = m_mapChannels.find(n);
    if(itr != m_mapChannels.end())
    {
      pRtn = itr->second;
    }
  }
  return pRtn;
}

wxString CPlotData::GetLocusName(int nChannel, int nTime)
{
  wxString sRtn;
  CPlotLocus *pl = NULL;
  size_t nSize = m_vLocus.size();
  for(size_t i = 0; i < nSize; i++)
  {
    pl = m_vLocus.at(i);
    if(pl->MatchChannelTime(nChannel,nTime))
    {
      sRtn = pl->GetName();
    }
  }
  return sRtn;
}

const int CPlotData::TYPE_ANALYZED = 1;
const int CPlotData::TYPE_RAW = 2;
const int CPlotData::TYPE_LADDER = 4;
const int CPlotData::TYPE_BASELINE = 8;
const int CPlotData::TYPE_ALL = 15; // the above or'ed

CPlotLocus *CPlotData::FindLocus(const wxString &sLocusName)
{
  CPlotLocus *pRtn = NULL;
  for(vector<CPlotLocus *>::iterator itr = m_vLocus.begin();
    (pRtn == NULL) && (itr != m_vLocus.end());
    ++itr)
  {
    if(! (*itr)->GetName().CmpNoCase(sLocusName) )
    {
      pRtn = *itr;
    }
  }
  return pRtn;
}

bool CPlotData::GetLocusRange(
  const wxString &sLocusName,
  int *px1, int *px2, int *py1, int *py2,
  int nType)
{
  CPlotLocus *ppl(NULL);
  CPlotChannel *pCh(NULL);
  unsigned int nChannel;
  bool bRtn = false;
  ppl = FindLocus(sLocusName);
  if(ppl != NULL)
  {
      nChannel = ppl->GetChannel();
      pCh = FindChannel(nChannel);
      bRtn = (
        ( pCh != NULL ) &&
        ( !!(nType & TYPE_ALL) )
             );
  }
  if(bRtn)
  {
    // this could be in the for loop but is easier to read here
    vector<int> *ppvn[4];
    vector<int> *pvn;
    int y;
    int ndxMax = 0;
    int x1;
    int x2;
    int nSize;
#if BASELINE_START
    int ndxBaseline = -1;
#endif
    if(nType & TYPE_ANALYZED)
    {
      ppvn[ndxMax++] = &pCh->m_vnAnalyzedPoints;
    }
    if(nType & TYPE_RAW)
    {
      ppvn[ndxMax++] = &pCh->m_vnRawPoints;
    }
    if(nType & TYPE_LADDER)
    {
      ppvn[ndxMax++] = &pCh->m_vnLadderPoints;
    }
    if(nType & TYPE_BASELINE)
    {
#if BASELINE_START
      ndxBaseline = ndxMax;
#endif
      ppvn[ndxMax++] = &pCh->m_vnBaselinePoints;
    }
    *px2 = ppl->GetEndExtended();
    *px1 = ppl->GetStartExtended();
    *py1 = 0; // if all numbers are > 0, use 0 for min
    *py2 = 1; // make it > *py1
    for(int j = 0; j < ndxMax; ++j)
    {
      pvn = ppvn[j];
      x1 = *px1;
      x2 = *px2;
#if BASELINE_START
      unsigned int nBaselineStart = pCh->GetBaselineStart();
      if(j == ndxBaseline && nBaselineStart != 0)
      {
        x1 -= nBaselineStart;
        x2 -= nBaselineStart;
      }
#endif
      nSize = (int) pvn->size();
      if(x2 >= nSize)
      {
        x2 = nSize;
        x2--;
      }
      if(x1 < 0)
      {
        x1 = 0;
      }
      for(int n = x1; n <= x2; ++n)
      {
        y = pvn->at((size_t)n);
        if(y < *py1)
        {
          *py1 = y;
        }
        if(y > *py2)
        {
          *py2 = y;
        }
      }
    }
  }
  return bRtn;
}


wxString CPlotData::FindAnalysisFile() const
{
  wxString sRtn;
  const wxString &sFileName(GetLastFileName());
  const wxString &sSampleName = GetFilename();
  const wxString &sOriginalDir = m_parm.GetOutputDirectory();
  if(!sFileName.IsEmpty())
  {
    wxFileName fn(sFileName);
    wxString sDir = fn.GetPath();
    wxDir dir(sDir);
    sRtn = COARfile::FindAnalysisFile(sDir,sSampleName);
    if(sRtn.IsEmpty() &&
      !nwxString::FileNameStringEqual(sDir,sOriginalDir))
    {
      sRtn = COARfile::FindAnalysisFile(sOriginalDir,sSampleName);
    }
  }
  return sRtn;
}
