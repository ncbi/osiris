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
*  FileName: CILSselection.h
*  Author:   Douglas Hoffman
*
*  container for two Combo Boxes, one for ILS family
*  and one for ILS.  The one for ILS family is shown
*  only when a kit is selected that utilized this
*  feature.  This class provides a wxSizer object
*  for the parent window to show the combo boxes 
*  and labels.
*
*/
#ifndef __C_ILS_SELECTION_H__
#define __C_ILS_SELECTION_H__

#include <wx/string.h>
#include <wx/event.h>

class wxChoice;
class wxWindow;
class wxSizer;
class wxBoxSizer;
class wxStaticText;
class CILSfamily;

class CILSselection
{
public:
  CILSselection(wxWindow *pParent);
  virtual ~CILSselection();
  wxSizer *GetSizer()
  {
    return m_pSizer;
  }
  void FamilyChange();
  void SetKit(const wxString &sKitName);
  void SetLS(const wxString &sLS); // this sets family name
  wxString GetFamily();
  wxString GetLS();
private:
  wxString m_sFamily;  // last family set
  wxString m_sKit;
  wxBoxSizer *m_pSizer;
  wxChoice *m_pComboFamily;
  wxChoice *m_pComboLS;
  wxStaticText *m_pLabelFamily;
  wxStaticText *m_pLabelLS;
  wxWindow *m_pParent;

  bool _ShowingFamily();
  bool _ShowingSingleILS();
  static const wxString &_FindDisplayName
    (const CILSfamily *pFamily, const wxString &sLSname);
  static const wxString &_FindLSname
    (const CILSfamily *pFamily, const wxString &sDisplayName);
};

#endif
