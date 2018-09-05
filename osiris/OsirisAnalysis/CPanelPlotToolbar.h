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
*  FileName: CPanelPlotToolbar.h
*  Author:   Douglas Hoffman
*
*  Provide a toolbar for each plot using the CPanelPlot class
*/
#ifndef __C_PANEL_PLOT_TOOLBAR_H__
#define __C_PANEL_PLOT_TOOLBAR_H__

// set to 0 for using wxCustomButton for Channel toggle buttons, set to 1 for native
#ifndef __USING_NATIVE_TOGGLE
#define __USING_NATIVE_TOGGLE 0
#endif

#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include <wx/combobox.h>
#include <wx/bitmap.h>
#include <nwx/nwxToggleButton.h>
#include <wx/button.h>
#include <wx/scrolwin.h>
#include "nwx/nwxShiftSizer.h"
#include "nwx/nwxButtonMenu.h"

#if !__USING_NATIVE_TOGGLE
#include "wx/things/toggle.h"
typedef wxCustomButton CHANNEL_BUTTON;
#else
typedef nwxToggleButton CHANNEL_BUTTON;
#endif

#include "nwx/nsstd.h"
#include "CPlotController.h"
#include "wxIDS.h"


#define PLOT_TOOLBAR_SHIFT_ALL "\nHold down the shift key to set all plots."

class CKitColors;
class CPlotData;
class CPanelHistoryMenu;
class CMenuHistory;
class CMenuLabels;

class CComboArtifact : public wxComboBox, IArtifactDisplayController
{
public:
  CComboArtifact(wxWindow *parent, wxWindowID id);
  virtual void SetIntValue(int n);
  virtual int GetIntValue();
  virtual void SetDefault();
};


/////////////////////////////////////////////////////////////////////


class CPanelPlotToolbar : 
  public wxScrolledWindow, 
  public CPlotController,
  public InwxShiftReceiver
{
public:
  CPanelPlotToolbar(
    wxWindow *parent, 
    CPlotData *pData, 
    CKitColors *pColors, 
    CMenuHistory *pMenu,
    int nMenuNumber,
    bool bDraw = true);
  virtual ~CPanelPlotToolbar();
  void SyncTo(CPanelPlotToolbar *p);

  // data

  virtual bool AnalyzedValue();
  virtual bool RawValue();
  virtual bool LadderValue();
  virtual bool BaselineValue();
  virtual bool BaselineEnabled();
  virtual void ShowAnalyzed(bool b = true);
  virtual void ShowRaw(bool b = true);
  virtual void ShowLadder(bool b = true);
  virtual void ShowBaseline(bool b = true);
  virtual void EnableBaseline(bool b = true);

  // channels 

  virtual bool ChannelValue(unsigned int n);
  virtual void ShowChannel(unsigned int n,bool b = true);
  virtual int ChannelCount();

  //  Sync, ILS, RFU 

  virtual bool SyncValue();
  virtual bool MinRfuValue();
  virtual bool LadderLabels();
  virtual bool ILSValue();
  virtual bool XBPSValue();
  virtual void SetSync(bool b = true);
  virtual void ShowILS(bool b = true);
  virtual void ShowMinRfu(bool b = true);
  virtual void ShowLadderLabels(bool b = true);
  virtual void SetXBPS(bool b = true);


  // labels, artifacts

  virtual size_t GetLabelTypes(vector<unsigned int> *pan);
  virtual void SetLabelTypes(const vector<unsigned int> &an);
  virtual void SetLabelType(LABEL_PLOT_TYPE n, LABEL_PLOT_TYPE nDefault = LABEL_NONE);
  virtual int ArtifactValue();
  virtual void SetArtifactValue(int nLevel);

  // enable/disable Label menu
  virtual void EnableLabelMenu(bool b = true);
  virtual bool IsLabelMenuEnabled();


  //  enable, disable append, delete buttons

  virtual void EnableAppend(bool b = true);
  virtual bool IsAppendEnabled();
  virtual void EnableDelete(bool b = true);
  virtual bool IsDeleteEnabled();

  //  enable, disable peak area label

  virtual void EnablePeakAreaLabel(bool b);
  virtual bool PeakAreaLabelEnabled();

  void SetHistoryMenu(CMenuHistory *pMenu);
  void UpdateHistoryButtons();

  // IShiftReceiver functions
  virtual void ShiftLeft(bool bShiftKey);
  virtual void ShiftRight(bool bShiftKey);
  virtual bool CanShiftLeft();
  virtual bool CanShiftRight();

  // positioning
  void GetPosition(int *x,int *y)
  {
    m_pPanel->GetPosition(x,y);
  }
  void Move(int x,int y)
  {
    m_pPanel->Move(x,y);
  }
  // the following should be a std::map<wxObject *,int>
  bool IsButtonAnalyzed(wxObject *p)
  {
    return (p == (wxObject *) m_pButtonAnalyzed);
  }
  bool IsButtonRaw(wxObject *p)
  {
    return (p == (wxObject *) m_pButtonRaw);
  }
  bool IsButtonLadder(wxObject *p)
  {
    return (p == (wxObject *) m_pButtonLadder);
  }
  bool IsButtonBaseline(wxObject *p)
  {
    return (p == (wxObject *) m_pButtonBaseline);
  }
  bool IsButtonRfu(wxObject *p)
  {
    return (p == (wxObject *) m_pButtonRfu);
  }
  bool IsButtonLadderLabels(wxObject *p)
  {
    return (p == (wxObject *) m_pButtonLadderLabels);
  }
  bool IsButtonILS(wxObject *p)
  {
    return (p == (wxObject *) m_pButtonILS);
  }
  bool IsButtonSync(wxObject *p)
  {
    return (p == (wxObject *) m_pButtonSync);
  }

  unsigned int GetChannelFromButton(wxObject *p)
  {
    int nRtn = 0;
    for(int i = 0; i <= m_nChannelCount; i++)
    {
      if((wxObject *)(m_pButtonChannel[i]) == p)
      {
        nRtn = i;
        i = CHANNEL_MAX; // loop exit
      }
    }
    return nRtn;
  }
  CMenuLabels *GetMenuLabels()
  {
    return m_pMenuLabels;
  }
  void RefreshChannelButtons()
  {
    // when covered by a dialog, IsEnabled() sometimes returns a false
    // negative, this function corrects this when needed
    for(unsigned int n = 0; n <= CHANNEL_MAX; ++n)
    {
      CHANNEL_BUTTON *pButton = GetChannelButton(n);
      if(pButton != NULL)
      {
        pButton->Refresh();
      }
    }
  }

private:
  void _EnableBaseline(bool b)
  {
    m_pButtonBaseline->Enable(b);
  }

  CHANNEL_BUTTON *GetChannelButton(unsigned int n)
  {
    CHANNEL_BUTTON *p = (n <= CHANNEL_MAX)
      ? m_pButtonChannel[n] : NULL;
    return p;
  }
  CHANNEL_BUTTON *m_pButtonChannel[CHANNEL_MAX + 1]; 
       // add one to size to make it a 1 based array
  CComboArtifact *m_pComboArtifact;
  nwxToggleButton *m_pButtonSync;
  nwxToggleButton *m_pButtonAnalyzed;
  nwxToggleButton *m_pButtonRaw;
  nwxToggleButton *m_pButtonLadder;
  nwxToggleButton *m_pButtonBaseline;
  nwxToggleButton *m_pButtonRfu;
  nwxToggleButton *m_pButtonLadderLabels;
  nwxToggleButton *m_pButtonILS;
  wxButton *m_pButtonDelete;
  wxButton *m_pButtonAppend;
  nwxButtonMenu *m_pButtonLabels;
  CPanelHistoryMenu *m_pPanelHistory;
  CMenuLabels *m_pMenuLabels;
  wxPanel *m_pPanel;
  vector<wxWindow *> m_vShiftWindows;
  int m_nChannelCount;
};

#endif
