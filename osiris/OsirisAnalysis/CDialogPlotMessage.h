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
*  FileName: CDialogPlotMessage.h
*  Author:   Douglas Hoffman
*  Purpose:  When showing history or editing from a plot window,
*    an analysis file must be used.  This 
*     
*/

#ifndef __C_DIALOG_PLOT_MESSAGE_H__
#define __C_DIALOG_PLOT_MESSAGE_H__

#include <wx/string.h>
#include <wx/msgdlg.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>

class CDialogPlotMessageFind : public wxMessageDialog
{
public:
  enum
  {
    MSG_TYPE_HISTORY,
    MSG_TYPE_EDIT,
    MSG_TYPE_TABLE,
    MSG_TYPE_PRINT
  };
  static const wxString &GetMessage(int nType)
  {
    const wxString *psRtn(NULL);
    switch (nType)
    {
    case MSG_TYPE_HISTORY:
      psRtn = &MSG_HISTORY;
      break;
    case MSG_TYPE_TABLE:
      psRtn = &MSG_TABLE;
      break;
    case MSG_TYPE_PRINT:
      psRtn = &MSG_PRINT;
      break;
    case MSG_TYPE_EDIT:
    default:
      psRtn = &MSG_EDIT;
    }
    return *psRtn;
  }

  CDialogPlotMessageFind(
    wxWindow *parent, 
    int nType = CDialogPlotMessageFind::MSG_TYPE_HISTORY) :
      wxMessageDialog(parent,
        CDialogPlotMessageFind::GetMessage(nType),
        "Find file",
        wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION)
  {;}
private:
  static const wxString MSG_EDIT;
  static const wxString MSG_HISTORY;
  static const wxString MSG_TABLE;
  static const wxString MSG_PRINT;

};


class CDialogPlotMessage : public wxDialog
{
public:
  CDialogPlotMessage(wxWindow *parent, const wxString &sFileName);
  bool DontShowAgain()
  {
    return m_pCheckBox->GetValue();
  }
private:
  wxCheckBox *m_pCheckBox;

};

#endif
