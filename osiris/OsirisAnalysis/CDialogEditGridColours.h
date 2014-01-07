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
*  FileName: CDialogEditGridColours.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_EDIT_GRID_COLOURS_H__
#define __C_DIALOG_EDIT_GRID_COLOURS_H__

#include <wx/grid.h>
#include <wx/dialog.h>
class CParmOsiris;
class mainFrame;
class wxToggleButton;
class wxStaticText;

#include <wx/bmpbuttn.h>
typedef wxBitmapButton ColorButton;

class CGridColorGrid;
class CGridAttrGrid;

class CDialogEditGridColours : public wxDialog
{
public:
  CDialogEditGridColours(mainFrame *parent);
  virtual ~CDialogEditGridColours();
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
  bool IsModified()
  {
    return m_bModified;
  }
private:
  wxSize _ComputeGridSize(wxGrid *);
  static void _SetButtonColour(ColorButton *p, const wxColour &c);
  ColorButton *_CreateButton(wxWindow *parent,wxWindowID id);
  void OnButtonBG(wxCommandEvent &e);
  void OnButtonFG(wxCommandEvent &e);
  void OnButtonDefault(wxCommandEvent &e);
  void OnButtonReverse(wxCommandEvent &e);
  void OnAttrButton(wxCommandEvent &e);
  void OnGrid(wxGridEvent &e);
  void OnGridSelect(wxGridRangeSelectEvent &e);
  void _UpdateButtons(int nRowColor,int nRowAttr);
  bool _ShowColourDialog(wxColour *pColour);
  void _SetupGridColoursAttr(CParmOsiris *pParm);
  CGridColorGrid *m_pGridColor;
  CGridAttrGrid *m_pGridAttr;
  ColorButton *m_pButtonFG;
  ColorButton *m_pButtonBG;
  wxStaticText *m_pTextFG;
  wxStaticText *m_pTextBG;
  wxButton *m_pButtonReverse;
  wxToggleButton *m_pToggleBold;
  wxToggleButton *m_pToggleItalic;
  wxToggleButton *m_pToggleReverse;

  void _UpdateAttributesEdited(); 
    // when font attributes have changed for edited cells
    // this function will update the font for
    //  ROW_CELL_ALERT_EDITED and
    //  ROW_CELL_EDITED
  void _UpdateColorsEdited();
    // when colors for edited or alert cells has changed
    // this function will update colors for
    // ROW_EDITED or ROW_NEEDS_ATTENTION

  int m_nRowColor;
  int m_nRowAttr;
  int m_nInSelect;
  int m_nAttrNeedsAttn;
  int m_nAttrEdited;
  bool m_bModified;
  DECLARE_EVENT_TABLE();
};

#endif
