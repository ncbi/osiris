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
*  FileName: CMenuFileBase.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CMenuFileBase.h"
#include "wxIDS.h"

CMenuFileBase::CMenuFileBase(bool bClose)
{
  Append(IDanalyze,"&New Analysis...\tCtrl+N");
  Append(wxID_OPEN);

  m_nInsertPoint =  GetMenuItemCount();
#ifndef __WXMAC__
  Append(IDopenPlot,"O&pen Plot File...\tCtrl+P");
  Append(IDopenBatch,"Open &Batch File...\tCtrl+B");
#endif
  Append(IDlistMRU,"Recent Files...\tCtrl+R");
  if(bClose) Append(wxID_CLOSE ,"&Close\tCtrl+W");
  AppendSeparator();
#ifdef __WXMSW__
  Append(wxID_EXIT, "E&xit\tAlt+F4");
#else    
  Append(wxID_EXIT);
#endif

}

CMenuFileBase::~CMenuFileBase() {}
