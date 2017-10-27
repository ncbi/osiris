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

*  FileName: CDialogParameters.cpp
*  Author:   Douglas Hoffman
*
*/

#include "Platform.h"
#include <wx/filename.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/hyperlink.h>
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/utils.h>
#include "wxIDS.h"
#include "CDialogParameters.h"
#include "CDialogShowLabSettings.h"
#include "mainApp.h"
#include "CLabSettings.h"
#include "CGridRFUParam.h"
#include "nwx/nwxFileUtil.h"

#if 0
#define LINK_PREFIX wxS("file:///")
#ifdef __WXMSW__
#define LINK_PREFIX_LEN 8
#else
#define LINK_PREFIX_LEN 7
#endif

wxString CDialogParameters::CreateFileURL(const wxString &sFile,bool bForce)
{
  wxString sLink;
  wxString s(sFile);

  if(bForce || wxDirExists(s) || wxFileExists(s))
  {
    sLink.Alloc(sFile.Len() + LINK_PREFIX_LEN + 4);
    sLink = LINK_PREFIX;
#ifndef __WXMSW__
    while(s.StartsWith(wxS("/")))
    {
      s = s.Mid(1);
    }
#endif
    sLink.Append(s);
  }
  return sLink;
}
#endif

wxWindow *CDialogParameters::CreateLabSettingsLink(const wxString &sLabel)
{
  wxHyperlinkCtrl *pLink =
    new wxHyperlinkCtrl(this,IDlab,sLabel,wxEmptyString,
      wxDefaultPosition, wxDefaultSize,
      wxHL_ALIGN_LEFT | wxNO_BORDER);
  return pLink;
}
wxWindow *CDialogParameters::CreateHyperlink(int nID, const wxString &sDir, bool bForce)
{
  wxString sDirLink;
  wxWindow *pRtn = NULL;
  if(bForce || wxDirExists(sDir))
  {
    sDirLink = sDir;
  }
  if(sDirLink.IsEmpty())
  {
    pRtn = new wxStaticText(this,nID,sDir);
  }
  else
  {
    wxHyperlinkCtrl *pLink = new wxHyperlinkCtrl(this,nID,sDir,sDirLink,
      wxDefaultPosition, wxDefaultSize,
      wxHL_ALIGN_LEFT  | wxNO_BORDER | wxHL_CONTEXTMENU );
    pRtn = pLink;
  }
  return pRtn;
}

CDialogParameters::CDialogParameters(
  wxWindow *parent, 
  const CParmOsiris *pOsiris, 
  const wxString *psFileName,
  const wxString *pLadderInfo,
  const wxString *psOrigin,
  CLabSettings *pLabSettings,
  const CXMLmessageBook *pMsgBook) :
    wxDialog(parent,wxID_ANY,"Analysis Parameters",
         wxDefaultPosition, wxDefaultSize,
         wxDEFAULT_DIALOG_STYLE),
    m_pLabSettings(pLabSettings),
    m_pMsgBook(pMsgBook)
{
  if(psFileName != NULL)
  {
    m_sFileName = *psFileName;
    FIX_FILE_NAME(m_sFileName);
  }
  wxString UNKNOWN("unknown");
  wxString sDir;
  wxString sDirLink;
  wxString s;
  wxString sVolume;
  wxString sKit;
  if(pLabSettings != NULL)
  {
    sVolume = pLabSettings->GetVolumeName();
    sKit = pLabSettings->GetKitName();
  }
  else
  {
    sKit = pOsiris->GetKitName();
  }


  int FLAG = wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT;
  int FLAG_TOP = wxALIGN_TOP | wxALIGN_LEFT;
  int nLadder = (pLadderInfo != NULL) && !pLadderInfo->IsEmpty();
  int nVolume = !sVolume.IsEmpty();
  int nOrigin = (psOrigin != NULL) && !psOrigin->IsEmpty();
  
  wxWindow *pLink;
  wxFlexGridSizer *pSizerTop = new wxFlexGridSizer(7 + nOrigin + nVolume + nLadder,2,ID_BORDER,ID_BORDER);
  pSizerTop->SetFlexibleDirection(wxBOTH);

#define BOLD_LABEL(pWin,sText) \
  pWin = new wxStaticText(this,wxID_ANY,sText); \
  mainApp::SetBoldFont(pWin);
  wxStaticText *pLabel;

  // Origin

  if(nOrigin)
  {
    BOLD_LABEL(pLabel,CParmOsiris::LABEL_ORIGIN);
    pSizerTop->Add(pLabel,0, FLAG);
    pSizerTop->Add(
       new wxStaticText(this,wxID_ANY,*psOrigin),
       0, FLAG);
  }
  // Input directory and font

  pLink = CreateHyperlink(IDhyperlinkInput,pOsiris->GetInputDirectory());
  BOLD_LABEL(pLabel,CParmOsiris::LABEL_INPUT);
  pSizerTop->Add(pLabel,0, FLAG);
  pSizerTop->Add(pLink, 0, FLAG);

  // Output directory

  BOLD_LABEL(pLabel,CParmOsiris::LABEL_OUTPUT);
  pLink = CreateHyperlink(
    IDhyperlinkOutput,pOsiris->GetOutputDirectory(),true);
  pSizerTop->Add(pLabel,0,FLAG);
  pSizerTop->Add(pLink, 0,FLAG);

  // Kit Name

  BOLD_LABEL(pLabel,CParmOsiris::LABEL_KIT);
  pSizerTop->Add(pLabel,0,FLAG);
  pSizerTop->Add(
    new wxStaticText(this,wxID_ANY,pOsiris->GetKitName()),
    0,FLAG);

  if(nVolume)
  {
    // volume name
    if(pMsgBook != NULL)
    {
      // show kit name with link
      pLink = CreateLabSettingsLink(sVolume);
    }
    else
    {
      pLink = new wxStaticText(this,wxID_ANY,sVolume);
    }
    BOLD_LABEL(pLabel,CParmOsiris::LABEL_VOLUME);
    pSizerTop->Add(pLabel,0, FLAG);
    pSizerTop->Add(pLink, 0, FLAG);
  }

  // Lane Standard Name

  BOLD_LABEL(pLabel,CParmOsiris::LABEL_LS);
  pSizerTop->Add(pLabel,0,FLAG);
  pSizerTop->Add(
    new wxStaticText(this,wxID_ANY,pOsiris->GetLsName()),
    0, FLAG);

  // Minimum RFU, first build sizer for all values
#if 1
  CGridRFUParam *pGridRFU = new CGridRFUParam(this);
  wxBoxSizer *pSizerRFU = new wxBoxSizer(wxHORIZONTAL);
  pSizerRFU->Add(pGridRFU,1,wxALIGN_LEFT | wxTOP | wxBOTTOM | wxRIGHT, ID_BORDER);
  pSizerRFU->AddSpacer(100);
  pGridRFU->SetupParms(pOsiris,sKit);
  pGridRFU->TransferDataToWindow();
  BOLD_LABEL(pLabel,CParmOsiris::LABEL_RFU);
  pSizerTop->Add(pLabel,0,FLAG_TOP);
  pSizerTop->Add(pSizerRFU,0,FLAG);
#else
  wxString sRFU;
  int nRFU;

#define SET_RFU(xx,nn) \
if(nn > 0) \
{ \
  if(!sRFU.IsEmpty()) { sRFU.Append(", "); } \
  sRFU.Append(xx); \
  nRFU = nn; \
  sRFU.Append(wxString::Format("%d",nRFU)); \
}

#define SET_GRID_LINE(sLabel) \
  BOLD_LABEL(pLabel,sLabel); \
  pGridRFU->Add(pLabel,0,FLAG); \
  pGridRFU->Add(new wxStaticText(this,wxID_ANY,sRFU),0,FLAG); \
  sRFU.Empty();


  wxFlexGridSizer *pGridRFU(new wxFlexGridSizer(3,2,ID_BORDER,ID_BORDER));
  pGridRFU->SetFlexibleDirection(wxBOTH);

  SET_RFU(CParmOsiris::LABEL_ANALYSIS,pOsiris->GetMinRFU_Sample());
  SET_RFU(
    CParmOsiris::LABEL_DETECTION,pOsiris->GetSampleDetectionThreshold());
  SET_RFU(CParmOsiris::LABEL_INTERLOCUS, pOsiris->GetMinRFU_Interlocus());
  SET_GRID_LINE(CParmOsiris::LABEL_RFU_SAMPLE);

  SET_RFU(CParmOsiris::LABEL_ANALYSIS, pOsiris->GetMinRFU_Ladder());
  SET_RFU(CParmOsiris::LABEL_INTERLOCUS, pOsiris->GetMinLadderInterlocusRFU());
  SET_GRID_LINE(CParmOsiris::LABEL_RFU_LADDER);

  nRFU = pOsiris->GetMinRFU_ILS();
  if(nRFU > 0)
  {
    sRFU = wxString::Format("%d",nRFU);
    SET_GRID_LINE(CParmOsiris::LABEL_RFU_ILS);
  }

#undef SET_RFU
#undef SET_GRID_LINE

  // finish RFU

  BOLD_LABEL(pLabel,CParmOsiris::LABEL_RFU);
  pSizerTop->Add(pLabel,0,FLAG_TOP);
  pSizerTop->Add(pGridRFU,0,FLAG);

#endif

  BOLD_LABEL(pLabel,CParmOsiris::LABEL_DATA);
  const wxString &sValue = 
    pOsiris->GetDataAnalyzed()
    ? CParmOsiris::LABEL_DATA_ANALYZED
    : CParmOsiris::LABEL_DATA_RAW;
  pSizerTop->Add(pLabel,0,FLAG);
  pSizerTop->Add(
      new wxStaticText(this,wxID_ANY,sValue),
      0,FLAG);

  // ladder file

  if(nLadder)
  {
    wxFileName fn(*pLadderInfo);
    BOLD_LABEL(pLabel,"Ladder File ");
    pSizerTop->Add(pLabel,0,FLAG);
    pSizerTop->Add(
      new wxStaticText(this,wxID_ANY,fn.GetFullName()),
      0,FLAG);
  }

#undef BOLD_LABEL

  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  wxButton *pButton = new wxButton(this,wxID_OK,"OK");
  pButton->SetDefault();
  pSizer->Add(pSizerTop,1,wxALL | wxEXPAND, ID_BORDER << 1);
  pSizer->Add(pButton,  0,wxBOTTOM | wxALIGN_CENTER, 
              ID_BORDER);
  pSizer->Fit(this);
  SetSizer(pSizer);
  CentreOnParent();
}

CDialogParameters::~CDialogParameters() {}

void CDialogParameters::OnHyperlink(wxHyperlinkEvent &e)
{
#define ERROR_MSG \
        "This file or folder does not exist.\n" \
        "A possible cause is that this file was\n" \
        "created on a different computer."

#define ERROR_PROMPT ERROR_MSG \
        "\n\nWould you like to view the folder containing\n" \
        "the file currently being viewed?"

  if(e.GetId() == IDlab)
  {
    CDialogShowLabSettings dlg(
      this,wxID_ANY,m_pLabSettings, m_pMsgBook);
    dlg.ShowModal();
  }
  else
  {
    wxString sErr;
    wxString sFileName = e.GetURL();
    bool bError = false;
    FIX_FILE_NAME(sFileName);
    const wxChar * const CANNOT_OPEN(wxS("Cannot open folder for "));
#ifdef __WXDEBUG__
  size_t nLenURL = sFileName.Len();
  const wchar_t *pURL = sFileName.wc_str();
  mainApp::LogMessageV(wxS("%d %ls"),(int)nLenURL,pURL);
#endif
    if( wxDirExists(sFileName) )
    {
    }
    else if(e.GetId() == IDhyperlinkInput)
    {
      sErr = ERROR_MSG;
      bError = true;
    }
    else
    {
      wxMessageDialog dlg(this,ERROR_PROMPT,"Error", wxYES_NO | wxYES_DEFAULT | wxICON_ERROR);
      int n = dlg.ShowModal();
      if(n == wxID_YES)
      {
        wxFileName fnFile(m_sFileName);
        sFileName = fnFile.GetPath();
        if(!wxDirExists(sFileName))
        {
          bError = true;
          sErr = CANNOT_OPEN;
          sErr.Append(m_sFileName);
        }
      }
      else
      {
        bError = true;
      }
    }
    if(!bError)
    {
      if(sFileName.IsEmpty()) {}
      else if(!nwxFileUtil::ShowFileFolder(sFileName))
      {
        sErr = CANNOT_OPEN;
        sErr.Append(sFileName);
        bError = true;
      }
      if(!bError)
      {
        EndModal(wxID_OK);
      }
    }
    if(!sErr.IsEmpty())
    {
      mainApp::LogMessage(sErr);
      mainApp::ShowError(sErr,this);
    }
  }
  //e.Skip();
}

BEGIN_EVENT_TABLE(CDialogParameters,wxDialog)
EVT_HYPERLINK(wxID_ANY, CDialogParameters::OnHyperlink)
END_EVENT_TABLE()
