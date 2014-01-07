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
*  FileName: CDialogCellHistory.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_CELL_HISTORY_H__
#define __C_DIALOG_CELL_HISTORY_H__

#include <wx/dialog.h>
#include "COARfile.h"
#include "nwx/stdb.h"
#include <set>
#include <map>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

class CSplitterCellHistory;

class CDialogCellHistory : public wxDialog
{
public:
  CDialogCellHistory(
    const COARlocus &Locus,
    const COARmessages &Msg,
    const set<wxDateTime> &setDateTime,
    const wxString &sSampleName,
    wxWindow *parent, 
    wxWindowID id = wxID_ANY,
    const wxSize &sz = wxDefaultSize);
  CDialogCellHistory(
    const COARsample &Sample,
    int nChannel,
    const COARlocus &Locus,
    const COARmessages &Msg,
    const set<wxDateTime> &setDateTime,
    wxWindow *parent, 
    wxWindowID id = wxID_ANY,
    const wxSize &sz = wxDefaultSize);
  CDialogCellHistory(
    int nSelect,
    const COARfile &File,
    const COARsample &Sample,
    const map<int,wxString> &mapChannelNames,
    wxWindow *parent,
    wxWindowID id = wxID_ANY,
    const wxSize &sz = wxDefaultSize);
private:
  void _Setup(CSplitterCellHistory *pSplitter);
};


#endif
