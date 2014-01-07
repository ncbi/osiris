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
*  FileName: COARfile.cpp
*  Author:   Douglas Hoffman
*
*/
#define __C_OAR_FILE_CPP__
#include "mainApp.h"
#include <wx/filename.h>
#include <wx/dir.h>
#include "COARfile.h"
#include "CDirList.h"
#include "CComboLabels.h"
#include "CXMLmessageBook.h"
#include <wx/log.h>
#include <wx/datetime.h>
#include "nwx/stdb.h"
#include <memory>
#include "nwx/stde.h"
#include "nwx/nwxString.h"
#include "nwx/nwxXmlCMF.h"
#include "nwx/nwxFileUtil.h"
#include "CLabSettings.h"
#include "OsirisVersion.h"
#include "COmittedAlerts.h"
#include "OsirisFileTypes.h"

const wxString COARfile::g_sRootOAR(_T("OsirisAnalysisReport"));
const wxString COARfile::g_sLocusName(_T("LocusName"));
const wxString COARfile::g_sMessageNumber(_T("MessageNumber"));
TnwxXmlIOPersistVectorSimple<int,nwxXmlIOint> COARfile::g_iovn;
nwxXmlIOPersistVectorWxString COARfile::g_ioLocusName;
    // handles XML IO for vector<wxString>, mainly LocusName

//********************************************** COARsample2CMFSpecimen

nwxXmlCMFSpecimen *COARsample2CMFSpecimen::Find(COARsample *p)
{
  iterator itr = find(p);
  nwxXmlCMFSpecimen *pRtn = (itr == end()) ? NULL : itr->second;
  return pRtn;
}
bool COARsample2CMFSpecimen::Insert(COARsample *p, nwxXmlCMFSpecimen *ps)
{
  bool bRtn = false;
  if(find(p) == end())
  {
    bRtn = true;
    insert(value_type(p,ps));
  }
  return bRtn;
}

//********************************************** COARfile
wxString COARfile::FindAnalysisFile(
  const wxString &sDir, const wxString &sSampleName)
{
  wxDir dir(sDir);
  wxString sRtn;
  if(dir.IsOpened())
  {
    wxString sFileName;
    wxString sFileLower;
    const wxString sFileSpec(_T("*.o?r"));
    bool b;
    const int flags = wxDIR_FILES | wxDIR_HIDDEN;
    multimap<wxDateTime,wxString> mapDateName;
    multimap<wxDateTime,wxString>::reverse_iterator itr;
    for(b = dir.GetFirst(&sFileName,sFileSpec,flags);
      b; b = dir.GetNext(&sFileName))
    {
      wxFileName fn(sDir,sFileName);
      if(!IS_ANALYSIS_FILE(sFileName))
      {}
      else if(fn.FileExists())
      {
        mapDateName.insert(
          multimap<wxDateTime,wxString>::value_type(
          fn.GetModificationTime(),fn.GetFullPath()) );
      }
#ifdef _DEBUG
      else
      {
        wxString sErr(_T("File: "));
        sErr.Append(sFileName);
        sErr.Append(_T(" is not found."));
        wxASSERT_MSG(0,sErr);
      }
#endif
    }
    if(mapDateName.empty()) {}
    else if(sSampleName.IsEmpty())
    {
      sRtn = mapDateName.rbegin()->second;
    }
    else
    {
      for(itr = mapDateName.rbegin();
          itr != mapDateName.rend();
          ++itr)
      {
        COARfile oar;
        if( oar.LoadFile(itr->second) &&
            (oar.GetSampleByName(sSampleName) != NULL) )
        {
          sRtn = itr->second;
          break;
        }
      }
    }
  }
  return sRtn;
}
const wxString &COARfile::RootNode(void) const
{
  return g_sRootOAR;
}
void COARfile::RegisterAll(bool)
{
  RegisterWxString(_T("Version"),&m_sVersion);
  RegisterWxStringNotEmpty(_T("FullVersion"),&m_sFullVersion);
  RegisterWxStringNotEmpty(_T("MsgBookBuildTime"),&m_sMsgBookBuildTime);
  Register(_T("Heading"),&m_heading);
  Register(_T("Table"),&m_vpTable);
  Register(_T("DirectoryAlerts"),&m_DirAlerts);
  Register(_T("DirectoryNotes"),&m_notes);
  Register(_T("OldDirectoryNotes"),&m_pvOldNotes);
  Register(_T("DirectoryReviews"),&m_DirReviews);
  Register(_T("DirectoryAcceptance"),&m_DirAcceptance);
  Register(_T("Messages"),&m_messages);
  Register(_T("ExportSpecifications"),&m_mapExportSpecifications);
}

void COARfile::__BuildLocusMap() const
{
  // build map
  wxString sLocus;
  wxString sLocusUpper;
  const COARchannel *pChannel;
  size_t nSize = m_heading.GetChannelCount();
  size_t nSizeChannel;
  for(size_t i = 0; i < nSize; ++i)
  {
    pChannel = m_heading.GetChannel(i);
    nSizeChannel = pChannel->GetLocusCount();
    for(size_t j = 0; j < nSizeChannel; ++j)
    {
      sLocus = pChannel->GetLocusName(j);
      sLocusUpper = sLocus;
      sLocusUpper.MakeUpper();
      if(m_mapLocusChannel.find(sLocusUpper) == m_mapLocusChannel.end())
      {
        m_mapLocusChannel.insert(
          map<wxString,const COARchannel *>::value_type(sLocusUpper,pChannel) );
        m_vsLocus.push_back(sLocus);
        m_vnChannelNr.push_back(pChannel->GetChannelNr());
      }
      else
      {
        wxString sMsg;
        sMsg.Printf(_T("Locus, %s, appears in OAR file, %s, more than once"),
          sLocus.c_str(), m_sFileName.c_str());
        mainApp::LogMessage(sMsg);
      }
    }
  }
}

const COARchannel *COARfile::GetChannelFromLocus(const wxString &sLocusName) const
{
  wxString sLocus(sLocusName);
  _BuildLocusMap();
  sLocus.MakeUpper();
  map<wxString, const COARchannel *>::const_iterator itrch = m_mapLocusChannel.find(sLocus);
  const COARchannel *pRtn = (itrch != m_mapLocusChannel.end()) ? itrch->second : NULL;
  return pRtn;
}

COARsample *COARfile::GetSampleByName(const wxString &sName)
{
  COARsample *pRtn(NULL);
  vector<COARsample *> *vpSample(m_vpTable.Get());
  vector<COARsample *>::iterator itr;
  for(itr = vpSample->begin();
    itr != vpSample->end();
    ++itr)
  {
    if(nwxString::FileNameStringEqual(sName,(*itr)->GetName()))
    {
      pRtn = *itr;
      break;
    }
  }
  return pRtn;
}
void COARfile::FormatText(wxString *ps, int nIndent)
{
  wxString s(*ps);
  wxChar *pSpaces;
  wxChar *pBlank;
  wxChar spaces[] = "                     ";
  int MAX = sizeof(spaces) / sizeof(spaces[0]) - 1;

  if(nIndent >= MAX) {nIndent = MAX - 1;}
  pSpaces = &spaces[MAX - nIndent - 1];
  *pSpaces = wxChar('\n');
  pBlank = pSpaces;
  pBlank++; // spaces, no line feed

  // remove leading and trailing spaces
  // fix all end of lines and make them \n proceeded
  // by spaces for indentation (pSpaces)

  nwxString::Trim(&s);
  while(s.Replace(_T("\r\n"),_T("\n"),true)) {;}
  s.Replace(_T("\r"),_T("\n"),true);
  while(s.Replace(_T("\n "),_T("\n"),true)) {;}
  while(s.Replace(_T("\n\t"),_T("\n"),true)) {;}
  s.Replace(_T("\n"),pSpaces,true);
  nwxString::Trim(&s);
  if(s.IsEmpty())
  {
    ps->Empty();
  }
  else
  {
    *ps = pBlank;
    ps->Append(s);
  }
}

wxString COARfile::FormatMessages(const vector<int> &vn, int nIndent, const wxDateTime *pTime) const
{

  const COARmessage *pmsg;
  wxString sRtn;
  wxString s;

  // should be an odd number of spaces, because a char will
  // be replaced by a line feed (\n)

  bool bFirst = true;

  for(vector<int>::const_iterator itr = vn.begin();
    itr != vn.end();
    ++itr)
  {
    pmsg = GetMessageByNumber(*itr,pTime);
    if((pmsg != NULL) && !pmsg->GetHidden() && pmsg->DisplayOsiris())
    {
      s = pmsg->GetText();
      FormatText(&s,nIndent);
      if(!s.IsEmpty())
      {
        if(!bFirst)
        {
          sRtn.Append(_T("\n"));
        }
        sRtn.Append(s);
        bFirst = false;
      }
    }
  }
  return sRtn;
}

int COARfile::CountMessages(const vector<int> &vn,const wxDateTime *pTime) const
{

  const COARmessage *pmsg;
  int nRtn = 0;
  for(vector<int>::const_iterator itr = vn.begin();
    itr != vn.end();
    ++itr)
  {
    pmsg = GetMessageByNumber(*itr,pTime);
    if((pmsg != NULL) && 
        !pmsg->GetHidden() && 
        !pmsg->GetText().IsEmpty() &&
        pmsg->DisplayOsiris()
        )
    {
      nRtn++;
    }
  }
  return nRtn;
}

const wxString COARfile::g_sComma(_T(", "));

void COARfile::AppendCellString(wxString *pCell, const wxString &_s, int n)
{
  wxString s(_s);
  int i;
  nwxString::Trim(&s);
  if(!s.IsEmpty())
  {
    if(n < 1) { n = 1;}
    for(i = 0; i < n; i++)
    {
      if(!pCell->IsEmpty())
      {
        pCell->Append(g_sComma);
      }
      pCell->Append(s);
    }
  }
}
void COARfile::_ClearMessageBook()
{
  if(m_pMsgBook != NULL)
  {
    delete m_pMsgBook;
    m_pMsgBook = NULL;
  }
  m_bCheckedMsgBook = false;
}

CXMLmessageBook *COARfile::GetMessageBook()
{
  if(!m_bCheckedMsgBook)
  {
    m_bCheckedMsgBook = true;
    wxString sFileName = FindMessageBookFile();
    if(!sFileName.IsEmpty())
    {
      auto_ptr<CXMLmessageBook> pB(new CXMLmessageBook());
      if(pB->LoadFile(sFileName) && pB->IsOK())
      {
        m_pMsgBook = pB.release();
      }
    }
  }
  return m_pMsgBook;
}
wxString COARfile::FindFileByName(const wxString &sName)
{
  vector<wxString> vsPath;
  wxString sPath;
  size_t PATHCOUNT = 3;

  vsPath.reserve(PATHCOUNT);
  sPath = GetFileName();
  vsPath.push_back(sPath);
  sPath = GetHeadingFileName();
  vsPath.push_back(sPath);
  sPath = GetHeadingNewFileName();
  vsPath.push_back(sPath);
  for(size_t i = 0; i < PATHCOUNT; i++)
  {
    sPath = vsPath.at(i);
    if(!sPath.IsEmpty())
    {
      wxFileName fn(sPath);
      sPath = fn.GetPath();
      nwxFileUtil::EndWithSeparator(&sPath);
      sPath.Append(sName);
      if(wxFileName::FileExists(sPath))
      {
        i = PATHCOUNT; // loop exit
      }
      else
      {
        sPath.Empty();
      }
    }
  }
  return sPath;
}

wxString COARfile::FindMessageBookFile()
{
  wxString sRtn;
  wxString s = GetHeadingFileName();
  if(!s.IsEmpty())
  {
    wxFileName fn(s);
    s = fn.GetName();
    if(!s.IsEmpty())
    {
      s.Append(_T(".msgBook.xml"));
      sRtn = FindFileByName(s);
    }
  }
  return sRtn;
}
wxString COARfile::FindPlotFile(COARsample *pSample)
{
  wxString sRtn;
  if(pSample != NULL)
  {
    wxString sName = pSample->GetName();
    wxString sExt = CDirList::GetValidExtension(sName);
    size_t nLen = sExt.Len();
    if(nLen)
    {
      size_t n = sName.Len();
      sName = sName.Truncate(n - nLen - 1);
    }
    sName.Append(_T(".plt"));
    sRtn = FindFileByName(sName);
  }
  return sRtn;
}

#define ADD_STRING(s) vs.push_back(s); nLen += s.Len();
#define ADD_STRINGE(s) \
  if(!s.IsEmpty()) { ADD_STRING(s); vs.push_back(wxEmptyString); }
#define FORMAT_CH_NOTICE _T("Channel %d " COAR_NOTICE_DISPLAY ":")
#define _ADD_NOTES(s,n,TITLE) \
if(!s.IsEmpty()) \
{\
  wxString st(_T(TITLE)); \
  FormatText(&st,n); \
  ADD_STRING(st); \
  FormatText(&s,n + 2); \
  ADD_STRINGE(s); \
}

#define APPEND_STRING(s1,s2)             \
if(s2.Len())                             \
{                                        \
  if(s1.Len()) { s1.Append(_T("\n")); }  \
  s1.Append(s2);                         \
}

#define ADD_NOTES(s,n) _ADD_NOTES(s,n,"Notes:")

#define ADD_CHANNEL_NOTES(s,n) _ADD_NOTES(s,n,"Channel notes:")

#define ADD_REVIEW_CHANNEL_NOTES(sRev,s,n) \
FormatText(&sRev,n); \
_ADD_NOTES(sRev,n,"Channels:") \
_ADD_NOTES(s,n,"Channel notes:")

#define ADD_REVIEW_NOTES(sRev,sNotes,n) \
FormatText(&sRev,n); \
ADD_STRINGE(sRev); \
ADD_NOTES(sNotes,n)

void COARfile::_FormatCommonAlerts(
  COmittedAlerts *pOmitted,
  const COARsample *pSample,
  vector<wxString> &vs,
  size_t &nLen,
  const CAlertViewStatus &viewStatus,
  const wxDateTime *pTime,
  bool bReverse) const
{

  vector<int> vn;
  wxString s;
  wxString sTitle;
  wxString sNotes;
  wxString sReview;
  wxString sNeed;
  int i;
  int n;
  int nOmit;
  const int COUNT = 3;
  int nReview = 0;
  int nAccept = 0;
  bool bCanEditArtifacts = CanEditArtifacts();
  bool bHasDirMessages = HasDisplayedDirectoryMessages();
  bool bShowSomeDir =
    viewStatus.GetDirectory() || viewStatus.GetDirectoryLocus();
    
  for(i = 0; i < COUNT; i++)
  {
    s.Empty();
    sReview.Empty();
    sNotes.Empty();
    vn.clear();
    n = bReverse ? (COUNT - i - 1) : i;
    switch (n)
    {
    case 0:

      if(bCanEditArtifacts)
      {
        const COARnotes *pNotes = GetNotesDir(pTime);
        GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_DIR);
        sReview = FormatReviewAcceptance(pTime);
        sNotes = COARnotes::GetText(pNotes);
      }
      if(
        bHasDirMessages ||
        (!sNotes.IsEmpty()) || 
        (!sReview.IsEmpty())
        )
      {
        wxString sDirTitle(_T("Directory " COAR_NOTICE_DISPLAY ":"));
        const vector<COARbaseLocus *> *pbl = GetBaseLociAlerts();
        vector<COARbaseLocus *>::const_iterator itr;
        bool bTitleAdded = false;
        GetDirectoryMessages(&vn);
        if(vn.size())
        {
          if(viewStatus.GetDirectory())
          {
            s = FormatMessages(vn,2,pTime);
          }
          else
          {
            nOmit = CountMessages(vn,pTime);
            pOmitted->Add(
              CAlertViewStatus::DIRECTORY,nOmit);
          }
          vn.clear();
        }
        if(!s.IsEmpty())
        {
          ADD_STRING(sDirTitle);
          ADD_STRINGE(s);
          bTitleAdded = true;
        }
        for(itr = pbl->begin(); itr != pbl->end(); ++itr)
        {
          vn.clear();
          (*itr)->AppendMessagesToList(&vn);
          if(!vn.size())
          {}
          else if(!viewStatus.GetDirectoryLocus())
          {
            nOmit = CountMessages(vn,pTime);
            pOmitted->Add(CAlertViewStatus::DIRECTORY_LOCUS,nOmit);
          }
          else
          {
            s = FormatMessages(vn,4,pTime);
            if(!s.IsEmpty())
            {
              if(!bTitleAdded)
              {
                ADD_STRING(sDirTitle);
                bTitleAdded = true;
              }
              sTitle = (*itr)->GetLocusName();
              sTitle.Append(_T(" directory " COAR_NOTICE_DISPLAY ":"));
              FormatText(&sTitle,2);
              ADD_STRING(sTitle);
              ADD_STRINGE(s);
            }
          }
        }
        if(bShowSomeDir)
        {
          if(!bTitleAdded)
          {
            ADD_STRING(sDirTitle);
            bTitleAdded = true;
          }
          if(bCanEditArtifacts && bHasDirMessages)
          {
            sNeed = CheckDirStatus(nReview,nAccept,pTime);
            APPEND_STRING(sReview,sNeed);
          }
          ADD_REVIEW_NOTES(sReview,sNotes,2);
        }
      }
      break;
    case 1:
      pSample->AppendSampleAlerts(&vn);
      if(!viewStatus.GetSample())
      {
        nOmit = CountMessages(vn,pTime);
        pOmitted->Add(CAlertViewStatus::SAMPLE,nOmit);
      }
      else
      {
        if(vn.size())
        {
          s = FormatMessages(vn,2,pTime);
        }

        sNotes = pSample->GetNotesSampleStr(pTime);
        if(bCanEditArtifacts)
        {
          GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_SAMPLE);
          sReview = pSample->FormatSampleReviewAcceptance(pTime);
          sNeed = CheckSampleStatus(pSample,nReview,nAccept,pTime);
          APPEND_STRING(sReview,sNeed);
        }
        if(!s.IsEmpty() || !sNotes.IsEmpty() || !sReview.IsEmpty() )
        {
          sTitle = pSample->GetName();
          sTitle.Append(_T(" " COAR_NOTICE_DISPLAY ":"));
          ADD_STRING(sTitle);
          ADD_STRINGE(s);
          ADD_REVIEW_NOTES(sReview,sNotes,2);
        }
      }
      break;
    case 2:
      pSample->AppendILSAlerts(&vn);
      if(!viewStatus.GetILS())
      {
        nOmit = CountMessages(vn,pTime);
        pOmitted->Add(CAlertViewStatus::ILS,nOmit);
      }
      else
      {
        sNotes = pSample->GetNotesILSStr(pTime);
        nwxString::Trim(&sNotes);
        if(vn.size())
        {
          s = FormatMessages(vn,2,pTime);
          vn.clear();
        }
        if(bCanEditArtifacts)
        {
          GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_ILS);
          sReview = pSample->FormatILSReviewAcceptance(pTime);
          sNeed = CheckILSStatus(pSample,nReview,nAccept,pTime);
          APPEND_STRING(sReview,sNeed);
        }
        if(!s.IsEmpty() || !sNotes.IsEmpty() || !sReview.IsEmpty())
        {
          sTitle = _T("ILS channel " COAR_NOTICE_DISPLAY ":");
          ADD_STRING(sTitle);
          ADD_STRINGE(s);
          ADD_REVIEW_NOTES(sReview,sNotes,2);
        }
      }
      break;
    }
  }
}

wxString COARfile::FormatIlsAlerts(
  const COARsample *pSample, 
  const CAlertViewStatus &viewStatus,
  const wxDateTime *pTime) const
{
  COmittedAlerts mapOmitted;
  wxString sRtn;
  vector<int> vn;
  vector<wxString> vs;
  size_t nLen = 0;
  _FormatCommonAlerts(&mapOmitted,pSample,vs,nLen,viewStatus,pTime);
  _JoinStrings(sRtn,vs,&mapOmitted,nLen);
  return sRtn;
}

wxString COARfile::FormatChannelAlerts(
  const COARsample *pSample, 
  const CAlertViewStatus &viewStatus,
  const wxDateTime *pTime) const
{
  COmittedAlerts mapOmitted;
  wxString sRtn;
  wxString s;
  wxString sReview;
  wxString sTitle;
  vector<int> vn;
  vector<wxString> vs;

  const COARchannel *pChannel;
  size_t nChannelCount = m_heading.GetChannelCount();
  size_t i;
  size_t nLen = 0;
  const vector<int> *pvn;
  const COARchannelAlert *pcha;
  bool bOmit = !viewStatus.GetChannel();

  _FormatCommonAlerts(&mapOmitted, pSample,vs,nLen,viewStatus,pTime);

  for(i = 0; i < nChannelCount; i++)
  {
    pChannel = m_heading.GetChannel(i);
    pcha = pSample->GetChannelAlertsByChannel(pChannel->GetChannelNr());
    if(pcha != NULL)
    {
      pvn = pcha->GetMessageNumbers();
      if((pvn == NULL) || (!pvn->size()))
      {}
      else if(bOmit)
      {
        int nOmit = CountMessages(*pvn,pTime);
        mapOmitted.Add(CAlertViewStatus::CHANNEL,nOmit);
      }
      else
      {
        s = FormatMessages(*pvn,2,pTime);
        if(!s.IsEmpty())
        {
          sTitle.Printf(FORMAT_CH_NOTICE,pChannel->GetChannelNr());
          ADD_STRING(sTitle);
          ADD_STRINGE(s);
        }
      }
    }
  }

  if(!bOmit)
  {
    s = pSample->GetNotesChannelStr(pTime);
    if(CanEditArtifacts())
    {
      wxString sNeed;
      int nReview;
      int nAccept;
      GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_CHANNEL);
      sReview = pSample->FormatChannelReviewAcceptance(pTime);
      sNeed = CheckChannelStatus(pSample,nReview,nAccept,pTime);
      APPEND_STRING(sReview,sNeed);
    }
    ADD_REVIEW_CHANNEL_NOTES(sReview,s,2);
  }
  _JoinStrings(sRtn,vs,&mapOmitted,nLen);
  return sRtn;
}

wxString COARfile::FormatLocusAlerts(
  const COARsample *pSample,
  COARlocus *pLocus,
  const CAlertViewStatus &viewStatus,
  const wxDateTime *pTime) const
{
  COmittedAlerts mapOmitted;
  wxString sRtn;
  vector<wxString> vs;
  size_t nLen = 0;
  bool bOmit = !viewStatus.GetSampleLocus();
  if(pLocus != NULL)
  {
    wxString s;
    wxString sReview;
    wxString sTitle;
    wxString sNotes = pLocus->GetNotes(pTime);;
    vector<int> vn;
    pLocus->AppendAlerts(&vn,pTime);
    if(bOmit)
    {
      int n = CountMessages(vn,pTime);
      mapOmitted.Add(CAlertViewStatus::SAMPLE_LOCUS,n);
    }
    else
    {
      if(CanEditArtifacts())
      {
        int nReview;
        int nAccept;
        GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_LOCUS);
        sReview = pLocus->FormatReviewAcceptance(pTime);
        s = CheckLocusStatus(pLocus,nReview,nAccept,pTime);
        APPEND_STRING(sReview,s);
        s.Clear();
      }
      if(vn.size())
      {
        s = FormatMessages(vn,2,pTime);
      }
      if( !(s.IsEmpty() && sNotes.IsEmpty() && sReview.IsEmpty()) )
      {
        sTitle = pLocus->GetName();
        sTitle.Append(_T(" " COAR_NOTICE_DISPLAY ":"));
        ADD_STRING(sTitle);
        ADD_STRINGE(s);
        ADD_REVIEW_NOTES(sReview,sNotes,2);
        s.Empty();
      }
    }
  }
  _FormatCommonAlerts(&mapOmitted,pSample,vs,nLen,viewStatus, pTime,true);
  _JoinStrings(sRtn,vs,&mapOmitted,nLen);
  return sRtn;
}

bool COARfile::UpdateMessages(const COARmessages &msgs, const wxDateTime &t)
{
  // first check messages
  bool bRtn = false;
  if(m_messages.IsModified(msgs))
  {
    size_t nCount = msgs.GetMessageCount();
    size_t i;
    m_bModified = true;
    const COARmessage *pMsg;
    for(i = 0; i < nCount; i++)
    {
      pMsg = msgs.GetMessage(i);
      if(m_messages.UpdateMessage(*pMsg,t))
      {
        bRtn = true;
        m_bModified = true;
      }
    }
  }
  return bRtn;
}

bool COARfile::UpdateSampleLocus(
  COARsample *pSample, const COARlocus &Locus, const wxDateTime &t)
{
  bool bRtn = false;
  COARlocus *pLocus = pSample->FindLocus(Locus.GetName());
  if( (pLocus != NULL) && ((*pLocus) != Locus) )
  {
    if(pLocus->Update(Locus,t))
    {
      bRtn = true;
      m_bModified = true;
    }
  }
  return bRtn;
}

wxString COARfile::CheckStatus(
  int nRequired,
  int nHave,
  const wxString &sMessage)
{
  wxString sRtn;
  if(nHave < nRequired)
  {
    int nDiff = nRequired - nHave;
    wxString sn;
    sRtn.Alloc(sMessage.Len() + 12);
    sn.Printf(_T(" (%d)"),nDiff);
    sRtn.Append(sMessage);
    sRtn.Append(sn);
  }
  return sRtn;
}

#define NEED_REV " needs review"
#define NEED_ACC " needs acceptance"

#define SET_REVIEW(p,v) if(p != NULL) { *p = v; }

bool COARfile::NeedDirReview()
{
  bool bRtn = false;
  if(IsDirMessageEdited())
  {
    int nAcceptanceCount;
    int nReviewerCount;
    int nCount = m_DirReviews.GetCount();
    GetReviewerCounts(
        &nReviewerCount, &nAcceptanceCount,CLabReview::REVIEW_DIR);
    bRtn = (nCount < nReviewerCount);
  }
  return bRtn;
}
bool COARfile::NeedDirAcceptance()
{
  bool bRtn = false;
  if(HasDisplayedDirectoryMessages() && (!IsDirMessageEdited()))
  {
    int nAcceptanceCount;
    int nReviewerCount;
    int nCount = m_DirAcceptance.GetCount();
    GetReviewerCounts(
        &nReviewerCount, &nAcceptanceCount,CLabReview::REVIEW_DIR);
    bRtn = (nCount < nAcceptanceCount);
  }
  return bRtn;
}
wxString COARfile::CheckDirStatus(
  int nReview,
  int nAccept,
  const wxDateTime *pTime,
  bool *pbReview) const
{
  wxString sRtn;
  int nCount;
  if(IsDirMessageEdited(pTime))
  {
    nCount = m_DirReviews.GetCount(pTime);
    sRtn = CheckStatus(nReview,nCount,_T("Directory " NEED_REV));
    SET_REVIEW(pbReview,true);
  }
  else if(HasDisplayedDirectoryMessages())
  {
    nCount = m_DirAcceptance.GetCount(pTime);
    sRtn = CheckStatus(nAccept,nCount,_T("Directory " NEED_ACC));
    SET_REVIEW(pbReview,false);
  }
  return sRtn;
}
wxString COARfile::CheckChannelStatus(
  const COARsample *pSample,
  int nReview,
  int nAccept,
  const wxDateTime *pTime,
  bool *pbReview) const
{
  wxString sRtn;
  const COARmessages *pMsg(GetMessages());
  if(pSample->IsCellChannelEdited(pTime))
  {
    unsigned int nCount = pSample->GetChannelReviewCount(pTime);
    sRtn = CheckStatus(nReview,nCount,_T("Channel" NEED_REV));
    SET_REVIEW(pbReview,true)
  }
  else if(pSample->HasChannelAlert(pMsg,pTime))
  {
    unsigned int nCount = pSample->GetChannelAcceptanceCount(pTime);
    sRtn = CheckStatus(
      nAccept,nCount,_T("Channel" NEED_ACC));
    SET_REVIEW(pbReview,false)
  }
  return sRtn;
}
wxString COARfile::CheckILSStatus(
  const COARsample *pSample,
  int nReview,
  int nAccept,
  const wxDateTime *pTime,
  bool *pbReview) const
{
  wxString sRtn;
  const COARmessages *pMsg(GetMessages());
  if(pSample->IsCellILSEdited(pTime))
  {
    unsigned int nCount = pSample->GetILSReviewCount(pTime);
    sRtn = CheckStatus(nReview,nCount,_T("ILS" NEED_REV));
    SET_REVIEW(pbReview,true)
  }
  else if(pSample->HasILSAlert(pMsg,pTime))
  {
    unsigned int nCount = pSample->GetILSAcceptanceCount(pTime);
    sRtn = CheckStatus(
      nAccept,nCount,_T("ILS" NEED_ACC));
    SET_REVIEW(pbReview,false)
  }
  return sRtn;
}
wxString COARfile::CheckSampleStatus(
  const COARsample *pSample,
  int nReview,
  int nAccept,
  const wxDateTime *pTime,
  bool *pbReview) const
{
  wxString sRtn;
  const COARmessages *pMsg(GetMessages());
  if(pSample->IsSampleLevelEdited(pMsg,pTime))
  {
    unsigned int nCount = pSample->GetSampleReviewCount(pTime);
    sRtn = CheckStatus(
      nReview,nCount,_T("Sample" NEED_REV));
    SET_REVIEW(pbReview,true)
  }
  else if(pSample->HasSampleAlert(pMsg,pTime))
  {
    unsigned int nCount = pSample->GetSampleAcceptanceCount(pTime);
    sRtn = CheckStatus(
      nAccept,nCount,_T("Sample" NEED_ACC));
    SET_REVIEW(pbReview,false)
  }
  return sRtn;
}
wxString COARfile::CheckLocusStatus(
  const COARlocus *pLocus,
  int nReview,
  int nAccept,
  const wxDateTime *pTime,
  bool *pbReview) const
{
  wxString sRtn;
  if(pLocus != NULL)
  {
    const COARmessages *pMsg(GetMessages());
    if(pLocus->HasBeenEdited(pMsg,pTime))
    {
      wxString s(pLocus->GetName());
      unsigned int nCount = pLocus->GetReviewCount(pTime);
      s.Append(_T(NEED_REV));
      sRtn = CheckStatus(nReview,nCount,s);
      SET_REVIEW(pbReview,true)
    }
    else if(pLocus->HasAlerts(pMsg,pTime))
    {
      wxString s(pLocus->GetName());
      unsigned int nCount = pLocus->GetAcceptanceCount(pTime);
      s.Append(_T(NEED_ACC));
      sRtn = CheckStatus(nAccept,nCount,s);
      SET_REVIEW(pbReview,false)
    }
  }
  return sRtn;
}

#undef SET_REVIEW


wxString COARfile::FormatAlertsBySample(
  const COARsample *pSample, 
  const CAlertViewStatus &viewStatus,
  const wxDateTime *pTime) const
{
  COmittedAlerts mapOmitted;
  vector<int> vn;
  vector<wxString> vs;
  wxString s;
  wxString sReview;
  wxString sNotes;
  wxString sTitle;
  wxString sNeed;
  wxString sRtn;
  size_t nLen = 0;
  size_t i;

  vs.reserve(20);

  _FormatCommonAlerts(&mapOmitted,pSample,vs,nLen,viewStatus,pTime);

  // now loop through loci
  const vector<int> *pvn;
  wxString sLocusName;
  const COARlocus *pLocus;
  size_t nLoci = GetLocusCount();
  int nCurrentChannel = -1;
  int nChannel;
  int nLastChannelMessage = -1;
  int nReview;
  int nAccept;
  int nOmit;
  bool bOmitLocus = !viewStatus.GetSampleLocus();
  bool bOmitChannel = !viewStatus.GetChannel(); 

#define CHANNEL_MESSAGES \
  sTitle.Printf(FORMAT_CH_NOTICE,nChannel); \
  ADD_STRING(sTitle); \
  nLastChannelMessage = nChannel;
  GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_LOCUS);

  for(i = 0; i < nLoci; i++)
  {
    nChannel = GetChannelNr(i);
    sLocusName = GetLocusName(i);
    if(nChannel != nCurrentChannel)
    {
      nCurrentChannel = nChannel;
      const COARchannelAlert *pcha = pSample->GetChannelAlertsByChannel(nChannel);
      if(pcha != NULL)
      {
        pvn = pcha->GetMessageNumbers();
        if((pvn == NULL) || (!pvn->size()))
        {}
        else if(bOmitChannel)
        {
          nOmit = CountMessages(*pvn,pTime);
          mapOmitted.Add(CAlertViewStatus::CHANNEL,nOmit);
        }
        else
        {
          s = FormatMessages(*pvn,2,pTime);
          if(!s.IsEmpty())
          {
            CHANNEL_MESSAGES;
            ADD_STRINGE(s);
          }
        }
      }
    }
    pLocus = pSample->FindLocus(sLocusName);
    if(pLocus != NULL)
    {
      s.Empty();
      pLocus->AppendAlerts(&vn,pTime);
      if(bOmitLocus)
      {
        nOmit = CountMessages(vn,pTime);
        mapOmitted.Add(CAlertViewStatus::SAMPLE_LOCUS,nOmit);
      }
      else
      {
        if(vn.size())
        {
          s = FormatMessages(vn,4,pTime);
        }
        sNotes = pLocus->GetNotes(pTime);
        if(CanEditArtifacts())
        {
          sReview = pLocus->FormatReviewAcceptance(pTime);
          sNeed = CheckLocusStatus(pLocus,nReview,nAccept,pTime);
          APPEND_STRING(sReview,sNeed);
        }
        if( !(s.IsEmpty() && sNotes.IsEmpty() && sReview.IsEmpty())  )
        {
          if(nChannel != nLastChannelMessage)
          {
            CHANNEL_MESSAGES;
            vs.push_back(wxEmptyString);
          }
          sTitle.Printf(_T("  %s " COAR_NOTICE_DISPLAY ":"),sLocusName.c_str());
          ADD_STRING(sTitle);
          ADD_STRINGE(s);
          ADD_REVIEW_NOTES(sReview,sNotes,4);
        }
      }
      vn.clear();
    }
  }
  if(!bOmitChannel)
  {
    sNotes = pSample->GetNotesChannelStr(pTime);
    nwxString::Trim(&sNotes);
    if(CanEditArtifacts())
    {
      GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_CHANNEL);
      sReview = pSample->FormatChannelReviewAcceptance(pTime);
      sNeed = CheckChannelStatus(pSample,nReview,nAccept,pTime);
      APPEND_STRING(sReview,sNeed);
    }
    ADD_REVIEW_CHANNEL_NOTES(sReview,sNotes,2);
  }
  _JoinStrings(sRtn,vs,&mapOmitted,nLen);
  return sRtn;
}

void COARfile::_JoinStrings(
  wxString &s,
  const vector<wxString> &vs, 
  COmittedAlerts *pOmitted,
  size_t nLen)
{
  wxString sOmitted = pOmitted->Format();
  wxString sn(_T("\n"));
  size_t nSize = vs.size();
  s.Empty();
  s.Alloc(nLen + nSize  + sOmitted.Len() + 32);
  for(size_t i = 0; i < nSize; ++i)
  {
    s.Append(vs.at(i));
    s.Append(sn);
  }
  if(!sOmitted.IsEmpty())
  {
    if(!s.IsEmpty())
    {
      s.Append(sn);
    }
    s.Append(sOmitted);
    s.Append(sn);
  }
}
#if 0
/*
void COARfile::_UpdateFileVersion()
{
  if(GetVersion() < 1.2)
  {
    // set up allele IDs, version 1.1 did not have them
    vector<COARsample *> *pvpTable  = m_vpTable.Get();
    vector<COARsample *>::iterator itr;
    int nID = 0;
    for(itr = pvpTable->begin();
      itr != pvpTable->end();
      ++itr)
    {
      (*itr)->SetupAlleleIDs(&nID);
    }
  }
}
*/
#endif

void COARfile::_PostProcessFile()
{
  // whatever needs to be done after the file
  // is loaded

  wxFileName fn(m_sFileName);
  wxDateTime dt = fn.GetModificationTime();
  m_heading.CorrectFileTime(dt);
  vector<COARsample *> *vpSample = m_vpTable.Get();
  vector<COARsample *>::iterator itr;
  for(itr = vpSample->begin(); itr != vpSample->end(); ++itr)
  {
    (*itr)->PostProcessFile(this);
  }
}
bool COARfile::LoadFile(const wxString &sFileName)
{
  m_dtLastLoad.SetToCurrent();
  _ClearLocusInfo();
  _ClearMessageBook();
  bool bRtn = nwxXmlPersist::LoadFile(sFileName);
  m_bModified = false;
  if(bRtn)
  {
    m_sFileName = sFileName;
    _PostProcessFile();
//    _UpdateFileVersion(); // if this is an old version, then update
    //  nevermind

  }
  else
  {
    m_sFileName.clear();
  }
  return bRtn;
}


nwxXmlCMF *COARfile::CreateCMF(COARsample2CMFSpecimen *pMap)
{
  wxString sUser = wxGetUserId();
  auto_ptr<nwxXmlCMF> pCMF(new nwxXmlCMF);
  vector<wxString> vsLoci;
  size_t i;
  size_t j;
  size_t n;
  size_t nAlleleCount;
  COARlocus *pLocus;
  nwxXmlCMFSpecimen *pCMFSpecimen(NULL);
  nwxXmlCMFLocus *pCMFLocus(NULL);
  const wxDateTime &dtFile(GetFileTime());
  wxDateTime dtLocus;
  bool bIsAmel(false);
  if(sUser.IsEmpty())
  {
    sUser = _T("unknown");
  }
//  size_t nLen = sUser.Len();


  time_t t;
  wxString sComment(_T("\n\nCMF 3.2 file created by OSIRIS version "));
  sComment.Append(_T(OSIRIS_VERSION));
  sComment.Append(_T("\n"));
  time(&t);
  sComment.Append(nwxString::FormatDateTime(t));
  sComment.Append(_T("\n\n"));
  sComment.Replace(_T("-"),_T("/"),true);
  pCMF->SetComment(sComment);


  pCMF->SetDestination(_T("Lab"));
  pCMF->SetSourceLab(_T("Lab"));
  pCMF->SetSubmitByUserID(sUser);
  pCMF->SetSubmitDateTimeCurrent();
  pCMF->SetKit(GetKitName());

  // set up locus list

  vsLoci.reserve(32);
  COARchannel *pChannel;
  const vector<COARchannel *> *vpChannels = m_heading.GetAllChannels();
  for(vector<COARchannel *>::const_iterator itrch = vpChannels->begin();
    itrch != vpChannels->end();
    ++itrch)
  {
    pChannel = *itrch;
    n = pChannel->GetLocusCount();
    for(i = 0; i < n; i++)
    {
      vsLoci.push_back(pChannel->GetLocusName(i));
    }
  }

  //
  n = GetSampleCount();
  for(i = 0; i < n; i++)
  {
    COARsample *pSample = GetSample(i);
    if(pSample->IsSampleType() && !pSample->IsDisabled())
    {
      pCMFSpecimen = NULL;
      for(vector<wxString>::iterator itrs = vsLoci.begin();
        itrs != vsLoci.end();
        ++itrs)
      {
        pCMFLocus = NULL;
        bIsAmel = false;
        pLocus = pSample->FindLocus(*itrs);
        nAlleleCount =
          (pLocus == NULL) ? 0 : pLocus->AlleleCount();
        dtLocus = dtFile;
        for(j = 0; j < nAlleleCount; j++)
        {
          COARallele *pAllele = pLocus->GetAllele(j);
          if(!pAllele->IsDisabled())
          {
//            const wxString &sName = pAllele->GetName();
            if(pCMFSpecimen == NULL)
            {
              pCMFSpecimen = pCMF->AddNewSpecimen(pSample->GetName());
              if(pMap != NULL)
              {
                pMap->Insert(pSample,pCMFSpecimen);
              }

              // Specimen category will need user input
              // use "Other" for now
              pCMFSpecimen->SetSpecimenCategory(_T("Other"));
            }
            if(pCMFLocus == NULL)
            {
              pCMFLocus = pCMFSpecimen->AddNewLocus(*itrs);
              if(pCMFLocus != NULL)
              {
                pCMFLocus->SetReadingBy(sUser);
                bIsAmel = pCMFLocus->IsAmel();
              }
              else
              {
                wxString sError = "Unknown locus name, ";
                sError.Append(*itrs);
                sError.Append(_T(", found when creating CMF file"));
                wxASSERT_MSG(false,sError);
                mainApp::LogMessage(sError);
                j = nAlleleCount; // end this loop
              }
            }
            if(pCMFLocus != NULL)
            {
              pCMFLocus->AddAllele(pAllele->FormatName(bIsAmel,true));
              const wxDateTime &dt(pAllele->GetUpdateTime());
              if(dt > dtLocus)
              {
                dtLocus = dt;
              }
            }
          }
        }  // end allele loop
        if(pCMFLocus != NULL)
        {
          // we had at least one allele
          pCMFLocus->SetReadingDateTime(dtLocus);
        }
      }
    }
  }
  return pCMF.release();
}

void COARfile::UpdateVersion()
{
  double d = GetVersion();
  if(d > 1.101 && d < 2.0)
  {
    m_sVersion = "2.0";
  }
  SetIsModified();
}

bool COARfile::IsTampered(unsigned int nSeconds) const
{
  bool bRtn = m_heading.IsTampered();
  if(!bRtn)
  {
    time_t t1 = GetXMLmodTime().GetTicks();
    time_t t2 = GetFileModTime().GetTicks();
    time_t nDiff = (t1 > t2) ? (t1 - t2) : (t2 - t1);
    bRtn = (nDiff > (time_t)nSeconds);
    if(bRtn)
    {
      m_heading.SetTampered(true);
    }
  }
  return bRtn;
}

bool COARfile::SamplesDisabled() const
{
  bool bRtn = false;
  vector<COARsample *>::const_iterator itr;
  if(m_pLastSampleDisabled == NULL) {}
  else if(m_pLastSampleDisabled->IsDisabled())
  {
    bRtn = true;
  }
  else
  {
    m_pLastSampleDisabled = NULL;
  }
  for(itr = m_vpTable.Get()->begin();
    (!bRtn) && (itr != m_vpTable.Get()->end());
    ++itr)
  {
    if((*itr)->IsDisabled() && (*itr)->IsSampleType())
    {
      bRtn = true;
      m_pLastSampleDisabled = *itr;
    }
  }
  return bRtn;
}

size_t COARfile::GetDisabledSamples(
  vector<const COARsample *> *pv,
  bool bIncludeNonSamples) const
{
  vector<COARsample *>::const_iterator itr;
  size_t nRtn = 0;
  bool bDisabled;
  bool bSampleType;

  pv->clear();
  pv->reserve(16);
  for(itr = m_vpTable.Get()->begin();
    itr != m_vpTable.Get()->end();
    ++itr)
  {
    bDisabled = (*itr)->IsDisabled();
    bSampleType = (bIncludeNonSamples || bDisabled)
      ? (*itr)->IsSampleType()
      : false;

    if( (bDisabled && bSampleType) ||
        (bIncludeNonSamples && !bSampleType) )
    {
      pv->push_back(*itr);
      if(bSampleType)
      {
        nRtn++;
      }
    }
  }
  return nRtn;
}

wxString COARfile::GetFileExt(bool bDot) const
{
  wxString sRtn;
  if(bDot)
  {
    sRtn = _T(".");
  }
  size_t nLen = sRtn.Len();
  sRtn.Append(
    GetLabSettings().GetLabSettingsInfo()->GetDataFileType());
  if(sRtn.Len() == nLen)
  {
    sRtn.Append(CDirList::GetDefaultExt());
  }
  return sRtn;
}
