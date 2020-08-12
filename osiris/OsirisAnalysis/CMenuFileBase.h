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
*  FileName: CMenuFileBase.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_MENU_FILE_BASE_H__
#define __C_MENU_FILE_BASE_H__
#include <wx/menu.h>
#include "wxIDS.h"

class CMenuFileBase : public wxMenu
{
public:
  CMenuFileBase(bool bClose = true);
  virtual ~CMenuFileBase();
  size_t GetInsertPoint()
  {
    return m_nInsertPoint;
  }
  int InsertPrintItems(int nPos)
  {
    int nRtn = 4; // number of insertions
    InsertSeparator(nPos);
    Insert(nPos, wxID_PRINT);
#ifdef __WXMAC__
    Insert(nPos, IDpageMargins, wxT("Page Margins..."));
    nRtn++;
#endif
    Insert(nPos, IDpageSetup, wxT("Page Set&up..."));
    InsertSeparator(nPos);
    return nRtn;
  }
  static const wxString ARCHIVE_CREATE;
  static const wxString ARCHIVE_EXTRACT;
protected:  
  size_t m_nInsertPoint;

};


#endif
