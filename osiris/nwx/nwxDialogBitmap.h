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
*  FileName: nwxDialogBitmap.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_DIALOG_BITMAP_H__
#define __NWX_DIALOG_BITMAP_H__

#include <wx/dialog.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/scrolwin.h>
#include <wx/dc.h>
#include <wx/button.h>
#include <wx/textctrl.h>

class nwxScrolledBitmap : public wxScrolledWindow
{
public:
  enum
  {
    SZ_ACTUAL,
    SZ_FIT_TO_WINDOW,
    SZ_PCT
  };

  nwxScrolledBitmap(
      wxBitmap *pBitmap,
      wxWindow *parent,
      wxWindowID id,
      wxPoint pos = wxDefaultPosition,
      wxSize sz = wxDefaultSize,
      long style = wxHSCROLL | wxVSCROLL) :
    wxScrolledWindow(parent,id,pos,sz,style),
    m_image(pBitmap->ConvertToImage()),
    m_dScaleResize(1.0),
    m_dScale(1.0),
    m_pBitmap(pBitmap),
    m_nSizeType(SZ_ACTUAL)
  {
    m_imgResize = m_image;
    //SetBackgroundColour(*wxWHITE);
  }

  virtual ~nwxScrolledBitmap();
  virtual void OnDraw(wxDC &dc);

  void SetActualSize()
  {
    _SetSizeType(SZ_ACTUAL);
  }
  void SetFitToWindow()
  {
    _SetSizeType(SZ_FIT_TO_WINDOW);
  }
  void SetSize(double d)
  {
    if(d < 10.0) { d = 10.0;}
    if(d > 400.0) { d = 400.0;}
    _SetSizeType(SZ_PCT,d);
  }
  void ChangePct(double d)
  {
    SetSize(GetPct() + d);
  }
  double GetPct()
  {
    return m_dScale * 100.0;
  }
private:
  void _DrawBitmap(wxDC &dc, wxBitmap &Bitmap);
  void _SetSizeType(int n, double d = 100.0);
  const wxImage m_image;
  wxImage m_imgResize;
  double m_dScaleResize;
  double m_dScale;
  wxBitmap *m_pBitmap;
  int m_nSizeType;
  static const int BORDER_WIDTH;
  static const int BORDER_WIDTH2;
};

class nwxDialogBitmap : public wxDialog
{
public:
  nwxDialogBitmap(
    wxBitmap *pBitmap,
    wxWindow *parent,
    wxWindowID id,
    const wxString &sTitle = _T("View Image"),
    const wxPoint &pos = wxDefaultPosition,
    const wxSize &sz = wxDefaultSize,
    long style = wxDEFAULT_DIALOG_STYLE);
  virtual ~nwxDialogBitmap();
  void SetActualSize()
  {
    m_pScrollWin->SetActualSize();
  }
  void SetFitToWindow()
  {
    m_pScrollWin->SetFitToWindow();
  }
  void SetSize(double d)
  {
    m_pScrollWin->SetSize(d);
  }
  void ChangePct(double d)
  {
    m_pScrollWin->ChangePct(d);
  }
  double GetPct()
  {
    return m_pScrollWin->GetPct();
  }


private:
  static void _SetupTextWidth(wxTextCtrl *p);
  wxButton *m_pButtonMinus;
  wxButton *m_pButtonPlus;
  wxButton *m_pButtonActual;
  wxButton *m_pButtonFit;
  wxTextCtrl *m_pTextPct;
  nwxScrolledBitmap *m_pScrollWin;
public:
  void OnButton(wxCommandEvent &);
  void OnSetPct(wxCommandEvent &);
  void OnSize(wxSizeEvent &);
  DECLARE_EVENT_TABLE();
};

#endif
