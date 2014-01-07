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
*  FileName: CDialogCMF.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_DIALOG_CMF__
#define __C_DIALOG_CMF__

#include <wx/dialog.h>

class COARfile;
class COARsampleSort;
class wxCheckBox;
class nwxXmlCMF;
class CGridCMF;
class wxTextCtrl;
class wxComboBox;
class wxFileDialog;

class CDialogCMF : public wxDialog
{
public:
  CDialogCMF(
    wxWindow *parent, 
    wxWindowID id,
    COARfile *pFile,
    COARsampleSort *pSort);
  virtual ~CDialogCMF();
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
  virtual bool Show(bool show = true);
  bool IsIncomplete()
  {
    // return true if user clicked on "Later" to close window
    return m_bIncomplete;
  }
private:
  bool _SaveFile(const wxString &sFileName);
  nwxXmlCMF *m_pCMF;
  COARfile *m_pFile;

  CGridCMF *m_pGrid;
  wxTextCtrl *m_pTextSourceLab;
  wxTextCtrl *m_pTextDestLab;
  wxTextCtrl *m_pTextUserID;
  wxTextCtrl *m_pTextBatchID;

  wxTextCtrl *m_pTextFileName;
  wxCheckBox *m_pCheckViewLocation;
  wxComboBox *m_pComboType;
  wxFileDialog *m_pDialogFile;

  bool m_bIncomplete;
  bool m_bTransferred;
  void _Cleanup();
  void _CleanupCMF();
  void _ComputeSize();
  wxString _GetDefaultFileName();
public:
  void OnButtonLater(wxCommandEvent &e);
  void OnButtonBrowse(wxCommandEvent &e);
  DECLARE_EVENT_TABLE();
};

#endif
