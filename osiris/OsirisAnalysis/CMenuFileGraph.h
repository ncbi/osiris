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
*  FileName: CMenuFileGraph.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_MENU_FILE_GRAPH_H__
#define __C_MENU_FILE_GRAPH_H__

#include "CMenuFileBase.h"
#include "wxIDS.h"

class CMenuFileGraph : public CMenuFileBase
{
public:
  CMenuFileGraph() : CMenuFileBase()
  {
    // insert in reverse order
    Insert(m_nInsertPoint,
           IDExportGraphic,EXPORT_GRAPH,EXPORT_GRAPH_HELP);
    InsertPrintItems(m_nInsertPoint, true);
  }
  virtual ~CMenuFileGraph();
  static const wxString EXPORT_GRAPH;
  static const wxString EXPORT_GRAPH_HELP;
};

#endif
