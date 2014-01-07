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
*  FileName: CDialogExportPlot.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_EXPORT_PLOT_H__
#define __C_DIALOG_EXPORT_PLOT_H__

#include <wx/dialog.h>
#include "nwx/nwxNumberValidator.h"
class wxCheckBox;
class wxTextCtrl;
class wxRadioBox;
class wxComboBox;
class wxFileDialog;
class CFramePlot;
class wxBitmap;

class CDialogExportPlot : public wxDialog
{
public:
  enum UNIT
  {
    UNIT_PIXELS = 0,
    UNIT_IN,
    UNIT_CM,
  };
  enum USAGE
  {
    USAGE_SCREEN = 0,
    USAGE_PRINT
  };
  static const int nDPI_SCREEN;
  static const int nDPI_PRINT;
  static const double dSCREEN_TO_PRINT;
  static const double dPRINT_TO_SCREEN;

  CDialogExportPlot(
    CFramePlot *parent,
    wxWindowID id,
    const wxString &sPlotFileName);
  virtual ~CDialogExportPlot();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual bool Show(bool show = true);
  //static void SetupTextCtrlWidth(wxTextCtrl *p, int nMin = 50);
private:
  void _TransferWidthHeightToWindow(double dWidth, double dHeight);
  static double _GetDValue(wxTextCtrl *p);
  double _GetWidth()
  {
    return _GetDValue(m_pTextWidth);
  }
  double _GetHeight()
  {
    return _GetDValue(m_pTextHeight);
  }
  int _GetDPI()
  {
    int n = (m_nUsage == USAGE_SCREEN) ? nDPI_SCREEN : nDPI_PRINT;
    return n;
  }
  void _BuildBitmap();
  void _CleanupBitmap();

  nwxNumberValidator m_valid;
  wxString m_sPlotFile;
  wxString m_sDefaultFile;
  int m_nUnits;
  int m_nUsage;

  wxComboBox *m_pUnits;
  wxRadioBox *m_pUsage;
  wxTextCtrl *m_pTextTitle;
  wxTextCtrl *m_pTextWidth;
  wxTextCtrl *m_pTextHeight;
  wxTextCtrl *m_pTextFileName;
  wxCheckBox *m_pCheckViewLocation;
  wxFileDialog *m_pDialogFile;
  wxBitmap *m_pBitmap;
  CFramePlot *m_pParent;

  void _UpdateValidator();
  void _SetRange();
  void _SetTypeInt(bool bInteger);
  void _SetType()
  {
    _SetTypeInt(m_nUnits == UNIT_PIXELS);
  }
  void _ValidateUU()
  {
    switch(m_nUnits)
    {
    case UNIT_PIXELS:
    case UNIT_IN:
    case UNIT_CM:
      break;
    default:
      m_nUnits = UNIT_PIXELS;
    }
    switch(m_nUsage)
    {
    case USAGE_SCREEN:
    case USAGE_PRINT:
      break;
    default:
      m_nUsage = USAGE_SCREEN;
    }
  }
public:
  void OnUnits(wxCommandEvent &e);
  void OnUsage(wxCommandEvent &e);
  void OnButtonBrowse(wxCommandEvent &e);
  void OnButtonPreview(wxCommandEvent &e);
  void OnChangeText(wxCommandEvent &e);
  DECLARE_EVENT_TABLE()
};

#endif
