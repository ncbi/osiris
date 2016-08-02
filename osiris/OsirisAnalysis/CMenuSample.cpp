#include "CMenuSample.h"

#include <wx/button.h>
#include <wx/utils.h>
#include <wx/sizer.h>
#include "CMDIFrame.h"
#include "wxIDS.h"

CMenuSample::CMenuSample() : wxMenu()
{
  Append(IDmenuDisplayGraph,
    "Display &Graph",
    "Display channel plots.  Hold the shift key down for a single plot"
    );
  Append(IDmenuTable,"Show &Table\tCtrl+T","Display the analysis table");
  Append(IDmenuShowHideToolbar,CMDIFrame::HIDE_TOOLBARS);
  Append(IDmenuHistory,"&History...\tCtrl+H");
  Append(IDSampleApplyAll,"&Apply All\tCtrl+P");
  Append(IDmenuDisableSample,"&Disable Sample\tAlt+X");
};

IMPLEMENT_ABSTRACT_CLASS(CToolbarSample,wxPanel)

CToolbarSample::CToolbarSample(CMDIFrame *parent,bool bUserReadOnly) :
  wxPanel(parent,wxID_ANY),
  m_pParent(parent),
  m_pTextUser(NULL)
{
  const int FLAG =
    wxTOP | wxBOTTOM | wxLEFT | wxALIGN_CENTRE_VERTICAL;
  wxButton *pButtonGraph = 
    new wxButton(this,IDmenuDisplayGraph,wxT("Graph"),
      wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  wxButton *pButtonTable = 
    new wxButton(this,IDmenuTable,wxT("Table"),
      wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  wxButton *pButtonHistory = 
    new wxButton(this,IDmenuHistory,wxT("History"),
      wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  wxButton *pButtonApply = 
    new wxButton(this,IDSampleApplyAll,wxT("Apply All"),
      wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  wxButton *pButtonDisable = 
    new wxButton(this,IDmenuDisableSample,wxT("Disable"),
      wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  m_pTextUser = new wxTextCtrl(this,wxID_ANY,wxGetUserId());
  m_pTextUser->SetEditable(!bUserReadOnly);
  wxBoxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
  pSizer->Add(pButtonGraph,0,FLAG,ID_BORDER);
  pSizer->Add(pButtonTable,0,FLAG,ID_BORDER);
  pSizer->Add(pButtonHistory,0,FLAG,ID_BORDER);
  pSizer->Add(pButtonApply,0,FLAG,ID_BORDER);
  pSizer->Add(pButtonDisable,0,FLAG,ID_BORDER);
  pSizer->Add(m_pTextUser,1,FLAG | wxRIGHT, ID_BORDER);
  SetSizer(pSizer);
  Layout();
}
wxString CToolbarSample::GetUserID()
{
  return m_pTextUser->GetValue();
}
void CToolbarSample::OnButton(wxCommandEvent &e)
{
  m_pParent->MenuEvent(e);
}
BEGIN_EVENT_TABLE(CToolbarSample,wxPanel)
EVT_BUTTON(wxID_ANY,CToolbarSample::OnButton)
END_EVENT_TABLE()
