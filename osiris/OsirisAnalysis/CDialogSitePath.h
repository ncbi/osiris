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
*
*  FileName: CDialogSitePath.h
*  Author:   Douglas Hoffman
*  Date:     2018/02/07
*
*  dialog window for the user to select a
*  group for a shared site path (site settings:
*     Volumes, exports, etc)
*
*/
#ifndef __CDIALOG_SITE_PATH_H__
#define __CDIALOG_SITE_PATH_H__



#include <wx/dialog.h>

class wxStaticText;
class wxChoice;
class wxRadioButton;

class CDialogSitePath: public wxDialog
{
public:
  CDialogSitePath(wxWindow *parent);
  virtual ~CDialogSitePath() {;}
  const wxString &GetGroup()
  {
    return m_sGroup;
  }
  bool ProcessSitePath(bool bPromptError = true);
  void OnRadioButton(wxCommandEvent &);
private:
  void _promptError();
  size_t _setupGroupList(wxArrayString *pasKeep);
#ifdef __WXMAC__
  static bool _filterGroup(const wxString &sGroupName);
#endif
  wxRadioButton *m_pRadioUserOnly;
  wxRadioButton *m_pRadioGroup;
  wxStaticText *m_pTextGroup;
  wxChoice *m_pChoiceGroup;
  wxString m_sGroup;
  bool m_bNeedAdmin;
  DECLARE_ABSTRACT_CLASS(CDialogSitePath)
  DECLARE_EVENT_TABLE()
};


#endif
