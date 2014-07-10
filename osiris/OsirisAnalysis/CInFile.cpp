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

*  FileName: CInFile.cpp
*  Author:   Douglas Hoffman
*
*/
#include <string.h>
#include <ctype.h>
#include "CInFile.h"
#include "nwx/vectorptr.h"

bool CInFileAllele::Setup(wxString &sIn, CInFile *)
{
  wxString s;
  CInFile::_extractErrorStr(sIn,&s,&nError);
  dAllele = atof(s.utf8_str());
  bOffLadder =
    (s.Find(wxChar('*')) >= 0) ||
    (s.Find("OL") >= 0);
  return (dAllele > 0.1);
}

CInFileAlleleList::~CInFileAlleleList() {;}

void CInFileAlleleList::Setup(wxString &s, CInFile *pIn)
{
  CInFileAllele ifa;
  vector<wxString> vs;
  size_t nCount = CInFile::_split(s.utf8_str(),&vs,',');
  size_t i;
  Clear();
  m_sDisplay.Alloc(s.Len() + 4);
  //if(nCount)
  {
    reserve(nCount);
    for(i = 0; i < nCount; i++)
    {
      wxString &sa(vs.at(i));
      if( (!sa.IsEmpty()) && (sa.CmpNoCase("OK")) )
      {
        ifa.Setup(sa,pIn);
        if(ifa.nError)
        {
          m_bError = true;
        }
        push_back(ifa);
        if(ifa.dAllele > 0.0)
        {
          if(m_sDisplay.Len())
          {
            m_sDisplay.Append(", ");
          }
          m_sDisplay.Append(wxString::Format("%g",ifa.dAllele));
        }
        else if(m_sDisplay.IsEmpty())
        {
          m_sDisplay = "?";
        }
      }
    }
  }
}

CInFileSample::~CInFileSample()
{
  Clear();
}

void CInFileSample::Clear()
{
  m_sName.clear();
  m_sPosControl.clear();
  m_sILS.clear();
  m_sChannel.clear();
  m_sFullName.clear();
  m_sError.clear();

  m_nILSerror = 0;
  m_nSampleError = 0;
  m_nChannelError = 0;
  vectorptr<CInFileAlleleList>::cleanup(&m_vpAlleles);
}
void CInFileSample::Load(vector<wxString> &sLine,CInFile *pInFile)
{
  wxString s;
  size_t nSize = sLine.size();
  size_t i;
  Clear();
  CInFile::_extractErrorStr(sLine.at(0),&m_sName,&m_nSampleError);
  CInFile::_extractErrorStr(sLine.at(1),&m_sILS,&m_nILSerror);
  if(pInFile->PositiveControlColumn())
  {
    nSize--;
    m_sPosControl = sLine.at(nSize);
  }
  else
  {
    m_sPosControl.Clear();
  }
  if(!m_sILS.CmpNoCase("OK"))
  {
    m_sILS.Empty();
  }
  if(nSize > 2)
  {
    CInFile::_extractErrorStr(sLine.at(2),&m_sChannel,&m_nChannelError);
  }
  m_sFullName = m_sName;
  int n = m_sName.Find(wxChar('/'),true);
  if(n != wxNOT_FOUND)
  {
    m_sName.Remove(0,n+1);
  }
  n = m_sName.Find(wxChar('\\'),true);
  if(n != wxNOT_FOUND)
  {
    m_sName.Remove(0,n+1);
  }
  if(nSize > 3)
  {
    m_vpAlleles.reserve(nSize - 3);
    for(i = 3; i < nSize; i++)
    {
      m_vpAlleles.push_back(new CInFileAlleleList(sLine.at(i), pInFile));
    }
  }
}

void CInFileSample::UpdateErrorMap(map<int,size_t> *pmap, size_t ndx)
{
  CInFileAlleleList *pl;
  int nError;
#define INSERT(n) \
  if(n > 0) { pmap->insert(map<int,size_t>::value_type(n,ndx)); }

  INSERT(m_nILSerror);
  INSERT(m_nSampleError);
  INSERT(m_nChannelError);

  for(size_t i = 0; i < m_vpAlleles.size(); ++i)
  {
    pl = m_vpAlleles.at(i);
    if(pl->HasError())
    {
      for(size_t j = 0; j < pl->size(); j++)
      {
        nError = pl->at(j).nError;
        INSERT(nError);
      }
    }
  }
#undef INSERT
}

CInFile::~CInFile() { Clear();}

void CInFile::Clear()
{
  for(vector<CInFileSample *>::iterator itr1 = m_apSamples.begin();
    itr1 != m_apSamples.end();
    ++itr1)
  {
    delete (*itr1);
  }
  for(map<int,wxString *>::iterator itr2 = m_apErrors.begin();
    itr2 != m_apErrors.end();
    ++itr2)
  {
    delete itr2->second;
  }
  m_asColumns.clear();
  m_apSamples.clear();
  m_apErrors.clear();

}

void CInFile::_chomp(char *ps)
{
  char *psEnd = ps + strlen(ps);
  for(psEnd--; psEnd >= ps; psEnd--)
  {
    if(strchr("\r\n ",*psEnd))
    {
      (*psEnd) = 0;
    }
    else
    {
      psEnd = ps;
    }
  }
}

int CInFile::_extractErrorStr(wxString &s, wxString *psStr, int *pnErr)
{
  wxChar cAmp('&');
  const char *ps;
  const char *psDigit;
  bool bDone = false;
  if(s.GetChar(0) == cAmp)
  {
    ps = s.utf8_str();
    ps++;
    psDigit = ps;
    while(isdigit(*psDigit)) { psDigit++;}
    if( ((*psDigit) == '&') && (psDigit > ps) )
    {
      psDigit++;
      (*psStr) = psDigit;
      (*pnErr) = atoi(ps);
      bDone = true;
    }
  }
  if(!bDone)
  {
    (*pnErr) = 0;
    (*psStr) = s;
  }
  return *pnErr;
}
bool CInFile::_isFsa(wxString &s)
{
  size_t n(s.Len());
  bool bRtn = (n > 4) &&
      !s.Mid(n - 4).CmpNoCase(".fsa");
  return bRtn;
}
#if 0
/*
bool CInFile::_isLadder(vector<wxString> &vs)
{
  wxString s(vs.at(0));
  bool bRtn = false;
  s.MakeLower();
  if(s.Find("ladder") >= 0)
  {
    bRtn = true;
  }
  else
  {
    bRtn = (vs.size() > 2);
    for(size_t i = 2; bRtn && (i < vs.size()); i++)
    {
      if(vs.at(i).CmpNoCase("OK"))
      {
        bRtn = false;
      }
    }
  }
  return bRtn;
}
*/
#endif
/*
size_t CInFile::_split(const char *ps, vector<wxString> *pvs, char csep)
{
  size_t nRtn = 1;
  pvs->clear();
  const char *psSubString = ps;
  const char *psSep;
  for(psSep = strchr(psSubString,csep);
    psSep != NULL;
    psSep = strchr(psSubString,csep) )
  {
    if(psSep != psSubString)
    {
      pvs->push_back(wxString(psSubString,psSep - psSubString));
    }
    else
    {
      pvs->push_back(wxEmptyString);
    }
    psSep++;
    psSubString = psSep;
    nRtn++;
  }
  pvs->push_back(wxString(psSubString));
  return nRtn;
}
*/
wxString *CInFile::GetError(int n, bool bAdd)
{
  map<int, wxString*>::iterator itr = m_apErrors.find(n);
  wxString *pRtn = NULL;
  if(itr != m_apErrors.end())
  {
    pRtn = itr->second;
  }
  else if(bAdd)
  {

    pRtn = new wxString;
    pRtn->Alloc(1024);
    m_apErrors.insert(map<int, wxString*>::value_type(n,pRtn));
  }
  return pRtn;
}

void CInFile::AddToError(int nError, vector<wxString> &vs)
{
  wxString *ps = GetError(nError,true);
  wxString sError;
  wxString spaces("    ");
  wxChar nl('\n');
  size_t iStart = (vs.empty() || vs.at(0).IsEmpty()) ? 1 : 0;
  size_t i;
  for(i = iStart; i < vs.size(); ++i)
  {
    if(i > iStart)
    {
      sError += spaces;
    }
    sError += vs.at(i);
  }
  sError += nl;
  ps->Append(sError);

  CInFileSample *psample = GetSampleFromError(nError);
  if(psample != NULL)
  {
    psample->AppendFullErrorReport(sError);
  }
}

bool CInFile::Load(const char *psFileName)
{
  vector<wxString> vsLine;
  vector<wxString>::iterator itrLine;
  map<wxString,wxString> mapParms;
  typedef map<wxString,wxString>::value_type VALUEPARM;
  map<wxString,wxString>::iterator itrParm;
  const wxString sSampleName("Sample Name");
  const wxString sControl("+Cntrl");
  wxString s1;
  wxString s2;
  FILE *fin = fopen(psFileName,"r");
  CInFileSample *pSample;
  size_t i;
  int nRFU;
  int nCurrentError = 0;
  bool bRtn = (fin != NULL);
  bool bRecDone = false;
  bool bEndSamples = false;
  bool bInParameters = false;
  m_bPosCtrlColumn = false;

//  char *pamp;
//  char *p;
//  char *p1;

  if(bRtn)
  {
    Clear();
    const int LINE_LEN = 1024;
    size_t nCount;
    char sLine[LINE_LEN];

    vsLine.reserve(24);
    memset(sLine,0,sizeof(sLine));
    while(bRtn && (fgets(sLine,LINE_LEN,fin) != NULL) )
    {
      bRecDone = false;
      _chomp(sLine);
      nCount = _split(sLine,&vsLine);
      if(nCount > 3 && !bEndSamples)
      {
        if(!vsLine.at(0).CmpNoCase(sSampleName))
        {
          size_t nSize = vsLine.size();
          if(nSize > 0)
          {
            const wxString &sLast(vsLine.at(nSize - 1));
            if(!sLast.CmpNoCase(sControl))
            {
              m_bPosCtrlColumn = true;
            }
          }
          m_asColumns.reserve(nSize);
          for(i = 0; i < nSize; ++i)
          {
            m_asColumns.push_back(vsLine.at(i));
          }
          bRecDone = true;
        }
        else if(nCount == m_asColumns.size())
        {
//          if(!_isLadder(vsLine))
          {
            pSample = new CInFileSample(vsLine,this);
            m_apSamples.push_back(pSample);
          }
          bRecDone = true;
        }
      }
      if(bRecDone) {;} // this was a table line
      else if(bInParameters)
      {
        if(sLine[0])
        {
          char *psSep = strchr(sLine,':');
          if(psSep != NULL)
          {
            *psSep = 0;
            psSep++;
            s1 = sLine;
            s2 = psSep;
            s1.Trim(true);
            s2.Trim(true);
            s1.Trim(false);
            s2.Trim(false);
            s1.MakeLower();
            mapParms.insert(VALUEPARM(s1,s2));
          }
        }
        else
        {
          bInParameters = false;

          // set up osiris parameters

          vector<wxString> vsRfu;
          wxString UNKNOWN("unknown");
#define SETUPPARM(xx) \
          s1 = xx; \
          s1.Trim(true); \
          s1.MakeLower(); \
          itrParm = mapParms.find(s1); \
          if(itrParm != mapParms.end()) { s2 = itrParm->second; } \
          else {s2 = UNKNOWN; }

          SETUPPARM(CParmOsiris::LABEL_INPUT)
          m_parms.SetInputDirectory(s2);
          SETUPPARM(CParmOsiris::LABEL_OUTPUT)
          m_parms.SetOutputDirectory(s2);
          SETUPPARM(CParmOsiris::LABEL_KIT)
          m_parms.SetKitName(s2);
          SETUPPARM(CParmOsiris::LABEL_LS)
          m_parms.SetLsName(s2);
          UNKNOWN.Empty();
          SETUPPARM(CParmOsiris::LABEL_RFU);

#undef SETUPPARM

          vsRfu.reserve(3);
          _split(s2.utf8_str(),&vsRfu,',');
          while(vsRfu.size() < 3)
          {
            vsRfu.push_back("0");
          }
#define GETRFU(xx) \
          s1 = vsRfu.at(xx); \
          s1.Trim(true); \
          s1.Trim(false); \
          nRFU = atoi(s1.utf8_str());

          GETRFU(0);
          m_parms.SetMinRFU_Sample(nRFU);
          GETRFU(1);
          m_parms.SetMinRFU_ILS(nRFU);
          GETRFU(2);
          m_parms.SetMinRFU_Ladder(nRFU);
#undef GETRFU

        }
      }
      else if(!strcmp(sLine,"Parameters:"))
      {
        bInParameters = true;

      }
      else if( (nCount == 2 || nCount == 3) && _isFsa(vsLine.at(0)) )
      {
         // this is a negative control
        pSample = new CInFileSample(vsLine,this);
        m_apSamples.push_back(pSample);
      }
      else if(
        m_apSamples.size() &&
        (vsLine.size() == 1) &&
        vsLine.at(0).IsEmpty() )
      {
        bEndSamples = true;
        BuildErrorToSample();
      }
      else
      {
        int n;
        if(vsLine.at(0).IsEmpty() &&
          (nCurrentError > 0) &&
          (vsLine.size() > 1))
        {
          AddToError(nCurrentError,vsLine);
        }
        else if( _extractErrorStr(vsLine.at(0),&s1,&n) )
        {
          bool bDone = (n == nCurrentError);
          vsLine[0] = s1;
          nCurrentError = n;
          AddToError(nCurrentError,vsLine);
          if(bDone)
          {
            nCurrentError = 0;
          }
        }
      }
    }
    fclose(fin);
  }
  if( bRtn && (!GetColumnCount() || !GetRowCount()) )
  {
    bRtn = false;
  }
  if(bRtn)
  {
    m_sFileName = psFileName;
  }
  return bRtn;
}

CInFileSample *CInFile::GetSampleFromError(int nError)
{
  map<int,size_t>::iterator itr = m_mapErrorToSample.find(nError);
  CInFileSample *pRtn(NULL);
  if(itr != m_mapErrorToSample.end())
  {
    size_t n = itr->second;
    pRtn = m_apSamples.at(n);
  }
  return pRtn;
}

void CInFile::BuildErrorToSample()
{
  CInFileSample *ps;
  size_t ndx = 0;
  m_mapErrorToSample.clear();
  for(ndx = 0; ndx < m_apSamples.size(); ++ndx)
  {
    ps = m_apSamples.at(ndx);
    ps->UpdateErrorMap(&m_mapErrorToSample,ndx);
  }
}
