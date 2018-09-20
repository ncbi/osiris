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
*   RegisterUint("BaselineStart",&m_nBaselineStartObsolete);
*   RegisterIntVector("BaselinePoints",&m_vnBaselinePointsObsolete);
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
#include <math.h>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxLog.h"
#include "OsirisMath/coordtrans.h"

const wxString g_TagRawPoints();
const wxString g_TagAnalyzedPoints();
const wxString g_TagLadderPoints("ladderPoints");
const wxString g_TagNr("nr");

void CArtifact::RegisterAll(bool bInConstructor)
{
  RegisterInt("level",&m_nLevel);
  RegisterDouble("mean",&m_dMean);
  RegisterInt("height",&m_nHeight);
  RegisterDouble("PullupHeightCorrection",&m_dPullupHeightCorrection);
  RegisterDouble("meanbps",&m_dMeanBPS);
  RegisterDouble("PeakArea",&m_dPeakArea);
  RegisterDouble("Width",&m_dWidth);
  RegisterDouble("fit",&m_dFit);
  RegisterWxString("label",&m_sLabel);
  RegisterWxString("equivAllele",&m_sEquivAllele);

  if(bInConstructor)
  {
    Init();
  }
}


void CSamplePeak::RegisterAll(bool bInConstructor)
{
  RegisterDouble("mean",&m_dMean);
  RegisterInt("height",&m_nHeight);
  RegisterDouble("PullupHeightCorrection",&m_dPullupHeightCorrection);
  RegisterDouble("meanbps",&m_dMeanBps);
  RegisterDouble("PeakArea",&m_dPeakArea);
  RegisterDouble("Width",&m_dWidth);
  RegisterDouble("BPS",&m_dBPS);
  RegisterWxString("allele",&m_sAllele);
  RegisterDoubleM1("fit",&m_dFit);
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
      m_sAllele.Replace("1","X");
      m_sAllele.Replace("2","Y");
    }
  }
}
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
  RegisterInt("nr",&m_nr);
  RegisterDouble("minRFU",&m_dMinRfu);
  RegisterIntVector("rawPoints",&m_vnRawPoints);
  RegisterIntVector("analyzedPoints",&m_vnAnalyzedPoints);
  RegisterIntVector("ladderPoints",&m_vnLadderPoints);
  RegisterUint("baselineStart",&m_nBaselineStart);
  RegisterIntVector("baselinePoints",&m_vnBaselinePoints);
  // the following two lines are for v2.2 beta 1 compatibility
  // and should eventually be deprecated
  RegisterUint("BaselineStart",&m_nBaselineStartObsolete);
  RegisterIntVector("BaselinePoints",&m_vnBaselinePointsObsolete);
  Register("samplePeak",&m_IOpeak,(void *)&m_vSamplePeak);
  Register("artifact",&m_IOartifact,(void *)&m_vArtifact);
  Register("ladderPeak",&m_IOpeak,(void *)&m_vLadderPeak);
  if(bInConstructor)
  {
    Init();
  }
}

void CPlotChannel::_setupPeakTimeBPS()
{
  size_t nPEAKS = (m_pdPeakTime == NULL) ? GetPeakCount() : 0;
  if(nPEAKS)
  {
    vector<CSamplePeak *>::iterator itr;
    size_t n = (nPEAKS << 1) * sizeof(double); // size of two arrays
    m_pdPeakTime = (double *)malloc(n);
    m_pdPeakBPS = &m_pdPeakTime[nPEAKS]; // second half of array
    double *pTime = m_pdPeakTime;
    double *pBPS = m_pdPeakBPS;
    for(itr = m_vSamplePeak.begin();
      itr != m_vSamplePeak.end();
      ++itr)
    {
      *pTime = (*itr)->GetTime();
      *pBPS = (*itr)->GetMeanBPS();
      ++pTime;
      ++pBPS;
    }
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
const int CPlotLocus::MARGIN = 10;

void CPlotLocus::RegisterAll(bool bInConstructor)
{
  RegisterUint("channel",&m_nChannel);
  RegisterInt("start",&m_nStart);
  RegisterInt("end",&m_nEnd);
  RegisterInt("startExtended",&m_nStartExtended);
  RegisterInt("endExtended",&m_nEndExtended);
  RegisterWxString("name",&m_sName);
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
  CSplineTransform::FreeBPAsAFunctionOfTime(m_pdILS_BPs);
  m_pdILS_BPs = NULL;
  m_bCannotSetBPS = false;
}

void CPlotData::__setupILSBps()
{
  // should be called ONLY by _setupILSBps()
  CPlotChannel *pChannel = FindChannel(GetILSChannel());
  double *pdTime = (pChannel == NULL) ? NULL : pChannel->GetPeakTime();
  if(pdTime == NULL)
  {
    m_bCannotSetBPS = true;
  }
  else
  {
    double *pdBPS = pChannel->GetPeakBps();
    m_pdILS_BPs = CSplineTransform::GetBPAsAFunctionOfTime(
      pdTime,
      pdBPS,
      (int) pChannel->GetPeakCount(),
      (int)GetPointCount());
    pChannel->CleanupPeakTimeILS();
  }
}

void CPlotData::RegisterAll(bool bInConstructor)
{
  RegisterWxString("Version",&m_sVersion);
  RegisterWxString("FullVersion",&m_sFullVersion);
  RegisterWxString("filename",&m_sFilename);
  RegisterWxString("associatedLadder",&m_sLadder);
  RegisterUint("start",&m_nStart);
  RegisterUint("begin",&m_nBegin);
  RegisterUint("interval",&m_nInterval);
  RegisterUint("end",&m_nEnd);
  RegisterWxString("kit",&m_sKit);
  RegisterUint("ilsChannel",&m_nILS);
  Register("channel",&m_IOchannel,(void *)&m_vChannels);
  Register("locus",&m_IOlocus,(void *)&m_vLocus);
  Register("parameters",&m_parm,(void *)&m_parm);
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
      if(sName.StartsWith("AMEL"))
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
double CPlotData::_Interpolate(
  double dX,
  const double *pdXlist,
  const double *pdYlist,
  size_t nCount)
{
  double dRtn = 0.0;
  if(nCount == 1)
  {
    dRtn = pdYlist[0];
  }
  else if(nCount > 1)
  {
    double d;
    int ndxLo = 0;
    int ndxHi = nCount;
    int nMid;
    bool bDone = false;
    ndxHi--;
    while(ndxHi >= ndxLo)
    {
      nMid = (ndxLo + ndxHi) >> 1;
      d = dX - pdXlist[nMid];
      if(d < 0.0)
      {
        ndxHi = nMid - 1;
      }
      else if(d > 0.0)
      {
        ndxLo = nMid + 1;
      }
      else
      {
        ndxLo = nMid;
        ndxHi = ndxLo - 1;
        dRtn = pdYlist[nMid];
        bDone = true;
      }
    }
    if(!bDone)
    {
      if(ndxLo >= (int)nCount)
      {
        // x > max(x), extrapolate using last 2 points
        ndxLo = nCount - 1;
        ndxHi = ndxLo - 1;
      }
      else if(ndxHi < 0)
      {
        // x < min(x), extrapolate using first 2 points
        ndxHi = 0;
        ndxLo = 1;
      }
      else if ((ndxHi + 1) != ndxLo)
      {
        wxString sMsg = wxString::Format(
          "CPlotData::_Interpolate() problems: %d points, x = %g, ndxLo = %d, ndxHi = %d, range (%g, %g) - (%g, %g)",
          (int) nCount, dX, ndxLo, ndxHi,
          pdXlist[0], pdYlist[0],
          pdXlist[nCount - 1], pdYlist[nCount - 1]
          );
        nwxLog::LogMessage(sMsg);
#ifdef TMP_DEBUG
        wxASSERT_MSG(0,sMsg);
#endif
      }
      // at this point ndxLo > ndxHi
      double dx0 = pdXlist[ndxHi];
      double dx1 = pdXlist[ndxLo];
      double dy0 = pdYlist[ndxHi];
      double dy1 = pdYlist[ndxLo];
      double deltaX = (dx1 - dx0);
      const double MIN_X = 1.0E-5;
      if( deltaX < MIN_X && deltaX > -MIN_X)
      {
        wxString sMsg = wxString::Format(
          "CPlotData::_Interpolate() delta x = %g",deltaX);
        nwxLog::LogMessage(sMsg);
#ifdef TMP_DEBUG
        wxASSERT_MSG(0,sMsg);
#endif
        dRtn = dy0;
      }
      else
      {
        double deltaXpt = (dX - dx0);
        if(deltaXpt < MIN_X && deltaXpt > -MIN_X)
        {
          dRtn = dy0; // no need to interpolate
        }
        else
        {
          double dSlope = (dy1 - dy0) / deltaX;
          dRtn = dy0 + deltaXpt * dSlope;
        }
      }
    }
  }
  return dRtn;
}



double CPlotData::ILSBpsToTime(double dBPS)
{
  // bsearch BPS in list, find corresponding times
  // and interpolate
  double *pdTime = GetTimePoints();
  double *pdBPS = GetILSBpsPoints();
  size_t nPointCount = GetPointCount();
  return _Interpolate(dBPS, pdBPS, pdTime, nPointCount);
}

double CPlotData::TimeToILSBps(double dTime)
{
  // interpolate base pairs from time
  double *pdTime = GetTimePoints();
  double *pdBPS = GetILSBpsPoints();
  size_t nPointCount = GetPointCount();
  return _Interpolate(dTime, pdTime, pdBPS, nPointCount);
}
CPlotChannel *CPlotData::FindChannel(unsigned int n)
{
  CPlotChannel *pRtn(NULL);
  size_t nSize = m_vChannels.size();
  map<unsigned int,CPlotChannel *>::iterator itr;
  if(m_mapChannels.empty() && (n <= nSize))
  {
    pRtn = m_vChannels.at(n-1);
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

