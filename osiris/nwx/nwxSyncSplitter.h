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
*  FileName: nwxSyncSplitter.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_SYNC_SPLITTER_H__
#define __NWX_SYNC_SPLITTER_H__

#include <wx/splitter.h>
#include "nwx/TnwxSync.h"

class nwxSyncSplitter : public TnwxSync<wxSplitterWindow,int>
{
public:
  nwxSyncSplitter() 
  {
    m_bChanging = false;
  }
  virtual void SyncThis(wxSplitterWindow *p,int n)
  {
    if(p->GetSashPosition() != n)
    {
      p->SetSashPosition(n,true);
    }
  }
  virtual void Sync(wxSplitterWindow *p)
  {
    //
    //  if m_bChanging == false or p->IsShown() == false
    //  then the wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED
    //  event is the result of the window being resized
    //  and therefore the sash positions should not be
    //  sync'ed because it causes unwanted side effects
    //
    if(m_bChanging && p->IsShown() && Find(p))
    {
      int n = p->GetSashPosition();
      m_bChanging = false;
      SyncTo(n);
    }
  }
  void SetIsChanging(wxSplitterWindow *p)
  {
    //
    // this is called from the 
    // wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING event
    // handler -- when the user is dragging
    // the sash and NOT when the window is being
    // resized
    //
    if(Find(p))
    {
      m_bChanging = true;
    }
  }
private: 
  bool m_bChanging;
};

#endif
