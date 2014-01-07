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
*  FileName: CMenuFile.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_MENU_FILE_H__
#define __C_MENU_FILE_H__

#include <wx/menu.h>
#include <wx/datetime.h>
#include <wx/arrstr.h>
#include "nwx/nwxTimerReceiver.h"
#include "wxIDS.h"
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"


class CXSLExportFileType;

class CMenuFile : public wxMenu, public nwxTimerReceiver
{
public:
  static const wxString EXPORT_GRAPH;
  static const wxString EXPORT_GRAPH_HELP;
  static const wxString EXPORT_CMF;
  static const wxString EXPORT_CMF_HELP;

  CMenuFile();
  virtual ~CMenuFile();
  bool CheckUpdate(); // check if user export file formats have changed
  virtual void OnTimer(wxTimerEvent &e);
  CXSLExportFileType *GetFileTypeByID(int nID);
  void ScheduleUpdate()
  {
    m_TimerInterval.SetWaitTime(0);
  }
  void ForceUpdate()
  {
    m_bNeedsUpdate = true;
    ScheduleUpdate();
  }
  bool IsUpdating()
  {
    return (m_nUpdating > 0);
  }
  void EnableSave(bool bSaveAs,bool bSave, bool bExport)
  {
    Enable(wxID_SAVEAS,bSaveAs);
    Enable(wxID_SAVE, bSave);
    EnableTableExports(bExport);
  }
  void EnableExportGraph(bool bEnable)
  {
    Enable(IDExportGraphic,bEnable);
  }
  void EnableTableExports(bool bEnable);
  bool TableExportsEnabled()
  {
    return IsEnabled(IDExportCMF);
  }
private:
  void _Clear();

  static const int UPDATE_INTERVAL; // update interval in ms
  static const int MAX_FILE_MENU_COUNT;
      // maximum number of items to add to the file menu
      // if this number is exceeded, then create submenu

  nwxTimerInterval m_TimerInterval;
  wxArrayString m_vsFileTypes; // offset is ID - IDExportUser
  vector<bool> m_vbValid;
  int m_nInsertPoint;
  int m_nUpdating; // prevent reentry in CheckUpdate and 
                  //  prevent updating from BeginBatch() and EndBatch()
  wxMenuItem *m_pSubMenu; // used if too many types
  bool m_bNeedsUpdate;


};

#endif
