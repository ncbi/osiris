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
*  FileName: nwxShiftSizer.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_SHIFT_SIZER_H__
#define __NWX_SHIFT_SIZER_H__

#include <wx/sizer.h>
#include <wx/timer.h>
#include <wx/event.h>
#include "nwx/nwxTimerReceiver.h"

class wxScrolledWindow;
class wxStaticBitmap;

class InwxShiftReceiver
{
public:
  virtual ~InwxShiftReceiver() {;}
  virtual void ShiftLeft(bool bShiftKey) = 0;
  virtual void ShiftRight(bool bShiftKey) = 0;
  virtual bool CanShiftLeft() = 0;
  virtual bool CanShiftRight() = 0;
};

class nwxShiftSizer : public wxBoxSizer, nwxTimerReceiver
{
public:
  nwxShiftSizer(
    wxScrolledWindow *pPeerWindow,
    InwxShiftReceiver *pReceiver,
    int nBorder = 0
    ,int nWaitInterval = 250
    //,bool bUseExternalTimer = false // EXT TIMER
    );

  virtual ~nwxShiftSizer();
  void SetInterval(int n);
  void BeginShift(wxStaticBitmap *p);
  void EndShift();
  virtual void OnTimer(wxTimerEvent &e);
  void UpdateSize();
  bool Show(bool bShow = true);
  void Hide()
  {
    Show(false);
  }
  /*
   // EXT TIMER
  bool UseExternalTimer()
  {
    return m_bUseExternalTimer;
  }
  */
private:
  void _SetupLayout(bool bShow = true);
  bool _ShowButtons(bool bShow);
  bool _UpdateButtons();
  bool _DoShift();
  InwxShiftReceiver *m_pReceiver;
  wxStaticBitmap *m_pButtonLeft;
  wxStaticBitmap *m_pButtonRight;
  //wxTimer *m_pTimer; // EXT TIMER
  wxScrolledWindow *m_pPeerWindow;

  wxStaticBitmap *m_pCurrent;

  int m_nTotalInterval;
    // updated with each timer event while button is pressed

  int m_nWaitInterval;
  int m_nBorder;
  //bool m_bUseExternalTimer;  // EXT TIMER
  bool m_bIgnoreTimer;

};

#endif
