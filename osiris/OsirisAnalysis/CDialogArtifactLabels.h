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
*  FileName: CDialogArtifactLabels.ch
*  Author:   Douglas Hoffman
*
*  Dialog window for modifying label for artifact on plots
* 
*/
#ifndef __C_DIALOG_ARTIFACT_LABELS_H__
#define __C_DIALOG_ARTIFACT_LABELS_H__

#include <wx/dialog.h>
class CArtifactLabelsUser;
class wxListBox;
class wxTextCtrl;
class wxCheckBox;
class wxStaticText;

class CDialogArtifactLabels : public wxDialog
{
public:
  DECLARE_ABSTRACT_CLASS(CDialogArtifactLabels)

  CDialogArtifactLabels(wxWindow *parent);
  virtual ~CDialogArtifactLabels();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void OnMoveUp(wxCommandEvent &e);
  void OnMoveDown(wxCommandEvent &e);
  void OnCheck(wxCommandEvent &e);
  void OnListSelect(wxCommandEvent &e);
  void SetReadOnly(bool bReadOnly = true);
  bool IsModified()
  {
    return m_bModified;
  }
private:
  void _Cleanup();
  bool _CanSaveFile();
  void _swap(int nSelected, int nAdjacent);
  void _UpdateSelectionData();
  void _SetupButtons();
  wxStaticText *m_pLabelArtifactLabel;
  wxCheckBox *m_pCheckDefault;
  wxTextCtrl *m_pTextDisplay;
  wxListBox *m_pList;
  wxButton *m_pButtonUP,*m_pButtonDN;
  CArtifactLabelsUser *m_pA;
  int m_nSelected;
  bool m_bReadOnly;
  bool m_bModified;
  DECLARE_EVENT_TABLE()
};


#endif
