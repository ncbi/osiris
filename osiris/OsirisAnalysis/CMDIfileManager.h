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
*  FileName: CMDIfileManager.h
*  Author:   Douglas Hoffman
*  Purpose:  Used for managing CMDIFrame and COARfile objects when 
*    they are used by one or more windows.  When a window is deleted, 
*    its association with a COARfile object is removed and 
*    when the COARfile object is no longer used it is deleted.  
*/
#include <wx/timer.h>
#include <wx/event.h>
#include "nwx/stdb.h"
#include <map>
#include <set>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

class COARfile;
class CMDIFrame;
class CFrameAnalysis;

typedef map<CMDIFrame *, COARfile *> CMDI_WF; // window to file
typedef map<COARfile *, set<CMDIFrame *> > CMDI_FW; // file to windows

class CMDIfileManager
{
public:
  CMDIfileManager() {;}
  virtual ~CMDIfileManager();
  CMDIFrame *FindWindowByName(const wxString &sPath, bool bRaise);
  int RunningFrameCount();
  bool CloseAll();
  int EditedFiles();
  // void OnTimer(wxTimerEvent &e);
  void InsertWindow(CMDIFrame *pWin, COARfile *pFile = NULL);
  void RemoveWindow(CMDIFrame *pWin);
  bool FindWindow(CMDIFrame *pWin);
  void KillOARfile(COARfile *pFile);
  COARfile *FindOARfile(const wxString &sName);
  CFrameAnalysis *FindAnalysisFrame(COARfile *pFile);
  void UpdateHistory(COARfile *pFile);
  void UpdateSamplePlot(COARfile *pFile, const wxString &sSampleName);
  void UpdateOARfile(COARfile *pFile, bool bUpdateAnalysis = true);
  void UpdateLadderLabels();
  void UpdateFileMenu();
  void UpdatePlots(COARfile *pFile)
  {
    UpdateOARfile(pFile,false);
  }
  void DiscardChanges(COARfile *pFile);
  size_t Size()
  {
    return m_mapWindowFile.size();
  }
  void RefreshAllOAR();

private:
  bool _IteratorOK(CMDI_WF::iterator &itr)
  {
    bool b = (itr != m_mapWindowFile.end());
    return b;
  }
  bool _IteratorOK(CMDI_FW::iterator &itr)
  {
    bool b = (itr != m_mapFileWindows.end());
    return b;
  }
  void _RemoveIterator(CMDI_WF::iterator &itr)
  {
    _RemoveCOARfile(itr->second,itr->first);
    m_mapWindowFile.erase(itr);
  }
  // functions for managing m_mapFileWindows
  void _RemoveCOARfile(COARfile *pFile, CMDIFrame *pWin);
  void _InsertCOARfile(COARfile *pFile, CMDIFrame *pWin);

  CMDI_WF m_mapWindowFile;
  CMDI_FW m_mapFileWindows;

};
