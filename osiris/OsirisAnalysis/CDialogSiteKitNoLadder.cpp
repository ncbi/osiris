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
*  FileName: CDialogSiteKitNoLadder.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CSiteKitNoLadder.h"
#include "CDialogSiteKitNoLadder.h"
#include "CButtons.h"
#include "CGridEdit.h"
#include "mainApp.h"
#include <wx/stattext.h>
#include <wx/radiobut.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/arrstr.h>
#include "nwx/nwxGrid.h"
#include "nwx/vectorptr.h"


class _CGridChannelInfo : public _CGridEdit
{
public:
  _CGridChannelInfo(wxWindow *parent);
  virtual ~_CGridChannelInfo();
  bool GetData(std::map<int, CSiteKitChannelDetails *> *pData);
  bool SetData(std::map<int, CSiteKitChannelDetails *> *pData);
  void SetEmpty(int nChannels = 2);
  const wxString &GetLastError()
  {
    return m_sLastError;
  }
  void GoToErrorCell()
  {
    if (m_nErrorRow >= 0 && m_nErrorCol > 0)
    {
      GoToCell(m_nErrorRow, m_nErrorCol);
    }
  }
private:
  enum
  {
    COL_CHANNEL = 0,
    COL_FSA_CHANNEL,
    COL_DYE_NAME,
    COL_COLOR,
    COL_COUNT
  };
  void _cleanup()
  {
    vectorptr<CSiteKitChannelDetails>::cleanup(&m_vpChannelTemp);
  }
  wxString m_sLastError;
  std::vector<CSiteKitChannelDetails *> m_vpChannelTemp; 
  void _SetChannelCount(int n);
  int m_nChannels;
  int m_nErrorRow;
  int m_nErrorCol;
};

_CGridChannelInfo::_CGridChannelInfo(wxWindow *parent) :
  _CGridEdit(parent, wxID_ANY),
  m_nChannels(2),
  m_nErrorRow(-1),
  m_nErrorCol(-1)
{
  const wxChar * const psLabels[] =
  {
    wxS("Channel"),
    wxS("FSA/HID"),
    wxS("Dye Name"),
    wxS("Color")
  };
  const size_t LABELS = sizeof(psLabels) / sizeof(psLabels[0]);
  size_t i;
  SetRowColCount(CHANNEL_MAX, COL_COUNT);
  for (i = 0; i < LABELS; ++i)
  {
    SetColLabelValue(i, psLabels[i]);
  }
  AutoSizeColumns(true);
}
_CGridChannelInfo::~_CGridChannelInfo()
{
  _cleanup();
}

void _CGridChannelInfo::_SetChannelCount(int n)
{
  const wxColour colorDisabled(GetDisabledColour());
  const wxColor colorDefault(GetDefaultCellBackgroundColour());
  int i;
  if (n < 2) { n = 2; }
  else if (n > CHANNEL_MAX) { n = CHANNEL_MAX; }
  for (i = n; i < CHANNEL_MAX; ++i)
  {
    // disable unused rows
    SetRowReadOnly(i, true);
    SetRowBackgroundColour(i, colorDisabled);
    ClearRowValues(i);

  }
  int nRow1 = 1;  // 1 based row number
  for (i = 0; i < n; ++i)
  {
    SetRowReadOnly(i, false);
    SetRowBackgroundColour(i, colorDefault);
    SetCellBackgroundColour(i, COL_CHANNEL, colorDisabled);
    SetReadOnly(i, COL_CHANNEL, true);
    SetCellValue(i, COL_CHANNEL, nwxString::FormatNumber(nRow1));
    nRow1++;
  }
  GoToCell(0, 1);
  SetCellValue(n - 1, COL_CHANNEL, wxS("ILS"));
  m_nChannels = n;
}
void _CGridChannelInfo::SetEmpty(int nChannels)
{
  wxString s;
  int i;
  int nRow1 = 1;  // 1 based row number
  // set FSA/HID channel number column to defaults
  _SetChannelCount(nChannels);
  for (i = 0; i < m_nChannels; ++i)
  {
    SetCellValue(i, COL_FSA_CHANNEL, nwxString::FormatNumber(nRow1));
    SetCellValue(i, COL_DYE_NAME, wxEmptyString);
    SetCellValue(i, COL_COLOR, wxEmptyString);
    nRow1++;
  }
}

bool _CGridChannelInfo::SetData(std::map<int, CSiteKitChannelDetails *> *pData)
{
  size_t nSize = pData->size();
  bool bRtn = (nSize >= 2) && (nSize <= CHANNEL_MAX) &&
    CSiteKitNoLadder::IsChannelListComplete(pData);
  if (bRtn)
  {
    std::map<int, CSiteKitChannelDetails *>::iterator itr;
    int i = 0;
    for (itr = pData->begin(); itr != pData->end(); ++itr)
    {
      const CSiteKitChannelDetails *pChan = itr->second;
      SetCellValue(i, COL_FSA_CHANNEL, nwxString::FormatNumber(pChan->GetChannelFromFile()));
      SetCellValue(i, COL_DYE_NAME, pChan->GetDyeName());
      SetCellValue(i, COL_COLOR, pChan->GetColor());
      ++i;
    }
  }
  else
  {
    SetEmpty((int)nSize);
  }
  return bRtn;
}

bool _CGridChannelInfo::GetData(std::map<int, CSiteKitChannelDetails *> *pData)
{
  wxString sColor;
  wxString sDyeName;
  wxString sFSAchannel;
  std::set<int> setChannel; // checks for duplicate FSA/HID channel number
  int nRow;
  int nFSAchannel;
  int nChannel = 0;
  _cleanup();
  m_sLastError.Empty();
  m_nErrorCol = -1;
  m_nErrorRow = -1;
  for (nRow = 0; (nRow < m_nChannels) && (m_nErrorRow < 0); ++nRow)
  {
    sDyeName = GetCellValue(nRow, COL_DYE_NAME);
    sColor = GetCellValue(nRow, 3);
    sFSAchannel = GetCellValue(nRow, 1);
    if (sDyeName.IsEmpty())
    {
      m_sLastError = wxS("Dye name not specified");
      m_nErrorCol = COL_DYE_NAME;
      m_nErrorRow = nRow;
    }
    else if (sColor.IsEmpty())
    {
      m_sLastError = wxS("Color is not specified");
      m_nErrorCol = COL_COLOR;
      m_nErrorRow = nRow;
    }
#define SET_FSA_ERROR_LOC \
  m_nErrorCol = COL_FSA_CHANNEL; \
  m_nErrorRow = nRow;
#define FSA_CHANNEL_PREFIX "FSA/HID channel number"

#define SET_FSA_CHANNEL_ERROR_UN(s) \
  m_sLastError = FSA_CHANNEL_PREFIX; \
  m_sLastError.Append(" "); \
  m_sLastError.Append(s); \
  SET_FSA_ERROR_LOC

#define SET_FSA_CHANNEL_ERROR(suffix) \
  m_sLastError = FSA_CHANNEL_PREFIX ", "; \
  m_sLastError.Append(nwxString::FormatNumber(nFSAchannel)) ; \
  m_sLastError.Append(", "); \
  m_sLastError.Append(suffix); \
  SET_FSA_ERROR_LOC

    else if (sFSAchannel.IsEmpty())
    {
      SET_FSA_CHANNEL_ERROR_UN("is not specified");
    }
    else if(!nwxString::IsInteger(sFSAchannel,false))
    {
      SET_FSA_CHANNEL_ERROR_UN("is not an interger");
    }
    else if( (nFSAchannel = atoi(sFSAchannel.utf8_str())) < 1 )
    {
      //  no maximum range at this time
      // SET_FSA_CHANNEL_ERROR("is out of range");
      SET_FSA_CHANNEL_ERROR("must be at least 1");
    }
    else if(setChannel.find(nFSAchannel) != setChannel.end())
    {
      SET_FSA_CHANNEL_ERROR("is specified more than once");

    }
#undef SET_FSA_CHANNEL_ERROR
#undef FSA_CHANNEL_PREFIX
    else
    {
      nChannel++;
      setChannel.insert(nFSAchannel); // used for finding duplicates
      m_vpChannelTemp.push_back(
        new CSiteKitChannelDetails(nChannel, nFSAchannel, sDyeName, sColor));
    }
  }
  bool bRtn = false;
  if (m_nErrorRow < 0)
  {
    bRtn = true;
    mapptr<int, CSiteKitChannelDetails>::cleanup(pData);
    while (m_vpChannelTemp.size() > 0)
    {
      CSiteKitChannelDetails *pTemp = *(m_vpChannelTemp.rbegin());
      pData->insert(
        std::map<int, CSiteKitChannelDetails *>::value_type(
          pTemp->GetKey(), pTemp));
      m_vpChannelTemp.pop_back();
    }
  }
  else
  {
    //  clean up m_vpChannelTemp
    _cleanup();
  }
  return bRtn;
}



CDialogSiteKitNoLadder::CDialogSiteKitNoLadder(
  wxWindow *parent) :
    wxDialog(parent, wxID_ANY,
      wxS("OSIRIS Ladder Free Marker Sets"),
      wxDefaultPosition, wxDefaultSize,
      mainApp::DIALOG_STYLE)
{
  const wxChar *psLabels[] =
  {
    wxS("Marker set:"),
    wxS("ILS group:"),
    wxS("ILS selection:"),
    wxS("Channels"),
    wxS("Channel details:")
  };
  enum NDX_LABEL
  {
    nKIT_NAME = 0,
    nILS_GROUP,
    nILS,
    nCHANNELS,
    nCHANNEL_DETAILS
  };
//  CSiteKitNoLadderCollection *pSiteKits = CSiteKitNoLadderCollection::GetGlobal();
//  pSiteKits->Load();
  const wxChar * const psItem = wxS("marker set");
  wxBoxSizer *pSizerTop = new wxBoxSizer(wxHORIZONTAL);
  wxGridSizer *pSizerMisc = new wxGridSizer(3, 2, ID_BORDER, ID_BORDER);
  wxBoxSizer *pSizerButtons = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  int TOP_STYLE = wxLEFT | wxTOP | wxALIGN_CENTER_VERTICAL;
  int BUTTON_STYLE = TOP_STYLE | wxBOTTOM;
  int MISC_STYLE = wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT;
  m_pChoiceKit = new wxChoice(this, IDvolume);
  pSizerTop->Add(new wxStaticText(this, wxID_ANY, psLabels[nKIT_NAME]), 0, TOP_STYLE, ID_BORDER);
  pSizerTop->Add(m_pChoiceKit, 0, TOP_STYLE, ID_BORDER);
  pSizerTop->Add(CButtons::New(this, psItem), 0, TOP_STYLE, ID_BORDER);
  pSizerTop->Add(CButtons::Remove(this, psItem), 0, TOP_STYLE, ID_BORDER);
  pSizerTop->Add(CButtons::Rename(this, psItem), 0, TOP_STYLE | wxRIGHT, ID_BORDER);

  m_pChoiceILSgroup = new wxChoice(this, ID_ILS_FAMILY);
  m_pChoiceILS = new wxChoice(this, wxID_ANY);
  pSizerMisc->Add(new wxStaticText(this, wxID_ANY, psLabels[nILS_GROUP]), 0, MISC_STYLE);
  pSizerMisc->Add(m_pChoiceILSgroup, 0, MISC_STYLE);
  pSizerMisc->Add(new wxStaticText(this, wxID_ANY, psLabels[nILS]), 0, MISC_STYLE);
  pSizerMisc->Add(m_pChoiceILS, 0, MISC_STYLE);
  pSizerMisc->Add(new wxStaticText(this, wxID_ANY, psLabels[nCHANNELS]), 0, MISC_STYLE);

  wxArrayString asChannels;
  for (int i = 1; i <= CHANNEL_MAX; ++i)
  { asChannels.Add(nwxString::FormatNumber(i)); }

  m_pChoiceChannelCount = new wxChoice(
    this, wxID_ANY, wxDefaultPosition, wxDefaultSize, asChannels);
  pSizerMisc->Add(m_pChoiceChannelCount, 0, MISC_STYLE);

  m_pGridChannels = new _CGridChannelInfo(this);
  wxBoxSizer *pSizerGrid = new wxBoxSizer(wxVERTICAL);
  pSizerGrid->Add(new wxStaticText(
    this, wxID_ANY, psLabels[nCHANNEL_DETAILS]), 
    0, wxALIGN_LEFT | wxLEFT, ID_BORDER);
  pSizerGrid->Add(m_pGridChannels, 0, 
    wxLEFT | wxALIGN_LEFT, ID_BORDER + ID_BORDER + ID_BORDER);
  
  pSizerButtons->Add(CButtons::Lock(this), 0, BUTTON_STYLE, ID_BORDER);
  pSizerButtons->AddStretchSpacer(1);
  pSizerButtons->Add(CButtons::OK(this), 0, BUTTON_STYLE, ID_BORDER);
  pSizerButtons->Add(CButtons::Cancel(this), 0, BUTTON_STYLE, ID_BORDER);
  pSizerButtons->Add(CButtons::Apply(this), 0, BUTTON_STYLE | wxRIGHT, ID_BORDER);

  pSizerAll->Add(pSizerTop, 0, 0, 0);
  pSizerAll->Add(pSizerMisc, 0, 0, 0);
  pSizerAll->Add(pSizerGrid, 1, 0, 0);
  pSizerAll->Add(pSizerButtons, 0, 0, 0);
  SetSizer(pSizerAll);
  Layout();
};

CDialogSiteKitNoLadder::~CDialogSiteKitNoLadder()
{}

bool CDialogSiteKitNoLadder::TransferDataFromWindow()
{
  m_KitLadder.SetKitName(m_pChoiceKit->GetStringSelection());
  m_KitLadder.SetILSgroup(m_pChoiceILSgroup->GetStringSelection());
  m_KitLadder.SetILS(m_pChoiceILS->GetStringSelection());
  bool bRtn = m_pGridChannels->GetData(m_KitLadder.GetChannelInfo());
  if (!bRtn)
  {
    m_pGridChannels->GoToErrorCell();
    mainApp::ShowAlert(m_pGridChannels->GetLastError(),this);
  }
  return bRtn;
}

bool CDialogSiteKitNoLadder::TransferDataToWindow()
{
  // assume kit name is set
  m_pChoiceILSgroup->SetStringSelection(m_KitLadder.GetILSgroup());
  bool bRtn = _setupILSlist();
  if (!m_pChoiceILS->SetStringSelection(m_KitLadder.GetILS()))
  {
    bRtn = false;
  }
  int nChannelCount = m_KitLadder.GetChannelCount();
  if (nChannelCount < 2)
  {
    nChannelCount = 2;
    m_pGridChannels->SetEmpty(2);
  }
  else if (!m_pGridChannels->SetData(m_KitLadder.GetChannelInfo()))
  {
    bRtn = false;
  }
  if(!m_pChoiceChannelCount->SetStringSelection(
      nwxString::FormatNumber(nChannelCount)))
  {
    bRtn = false;
  }
  return bRtn;
}

void CDialogSiteKitNoLadder::OnChangeKit(wxCommandEvent &e)
{
  wxString sKit = e.GetString();
  wxString sPrevKit = m_KitLadder.GetKitName();
  if (sKit == sPrevKit) 
  {} // do nothing
  else if (!_changeKitOK())
  {
    m_pChoiceKit->SetStringSelection(sPrevKit);
  }
  else
  {

    CSiteKitNoLadderCollection *pKits = CSiteKitNoLadderCollection::GetGlobal();
    const CSiteKitNoLadder *pKit = pKits->FindByName(sKit);
    if (pKit == NULL)
    {
      wxString sError = "Cannot find marker set: ";
      sError.Append(sKit);
      mainApp::ShowError(sError, this);
      m_pChoiceKit->SetStringSelection(sPrevKit);
    }
    else
    {
      CSiteKitNoLadder kitBackup = m_KitLadder;
      m_KitLadder = *pKit;
      if (!TransferDataToWindow())
      {
        wxString sError = "An error occurred when changing marker set to ";
        sError.Append(sKit);
        mainApp::ShowError(sError, this);
        m_KitLadder = kitBackup;
        m_pChoiceKit->SetStringSelection(sPrevKit);
        TransferDataToWindow();
      }
    }
  }
}

void CDialogSiteKitNoLadder::OnRemove(wxCommandEvent &e)
{
  wxString sMsg;
  wxString sKitName = m_KitLadder.GetKitName(); // do not use const wxString&
  if (sKitName.IsEmpty())
  {
    // error, should never happen
    sMsg = "There is not name for this kit.  Internal error.";
  }
  if(!m_KitLadder.IsLocked())
  {
    // error, should never happen because
    // button should be disabled
    sMsg = "Marker set, ";
    sMsg.Append(sKitName);
    sMsg.Append(", is not locked and cannot be removed.");
  }
  else if(!m_KitLadder.UnLock())
  {
    // error
    sMsg = "Cannot unlock marker set: ";
    sMsg.Append(sKitName);
  }
  else
  {
    CSiteKitNoLadderCollection *pKits = CSiteKitNoLadderCollection::GetGlobal();
    if (!pKits->RemoveKit(m_KitLadder))
    {
      sMsg = "An error occurred when attempting to removed files for kit: ";
      sMsg.Append(sKitName);
    }
    m_KitLadder.Init();
    pKits->Load(true);
    _setup();
    if (!pKits->GetSize())
    {
      _initEmpty();
    }
    else
    {
      if (!m_pChoiceKit->SetStringSelection(sKitName))
      {
        m_pChoiceKit->Select(0);
      }
      OnChangeKit(e);
    }
  }
}

bool CDialogSiteKitNoLadder::_currentKitModified()
{
  CMessageSuppressor xxx;
  CSiteKitNoLadder kitBackup = m_KitLadder;
  bool bRtn = TransferDataFromWindow()
    ? (kitBackup != m_KitLadder)
    : true;
  m_KitLadder = kitBackup;
  return bRtn;
}
IMPLEMENT_PERSISTENT_SIZE_POSITION(CDialogSiteKitNoLadder)

BEGIN_EVENT_TABLE(CDialogSiteKitNoLadder, wxDialog)
EVT_PERSISTENT_SIZE_POSITION(CDialogSiteKitNoLadder)
EVT_BUTTON(IDremove, CDialogSiteKitNoLadder::OnRemove)
EVT_BUTTON(IDadd, CDialogSiteKitNoLadder::OnAdd)
EVT_BUTTON(IDrename, CDialogSiteKitNoLadder::OnRename)
EVT_CHOICE(IDvolume, CDialogSiteKitNoLadder::OnChangeKit)
EVT_CHOICE(ID_ILS_FAMILY, CDialogSiteKitNoLadder::OnChangeILS)
EVT_BUTTON(wxID_OK, CDialogSiteKitNoLadder::OnOK)
EVT_BUTTON(IDlock, CDialogSiteKitNoLadder::OnLock)
EVT_BUTTON(wxID_CANCEL, CDialogSiteKitNoLadder::OnCancel)
EVT_BUTTON(wxID_APPLY, CDialogSiteKitNoLadder::OnApply)

END_EVENT_TABLE()

