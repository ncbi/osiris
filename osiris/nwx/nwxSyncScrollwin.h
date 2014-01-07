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
*  FileName: nwxSyncScrollwin.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_SYNC_SCROLL_WIN_H__
#define __NWX_SYNC_SCROLL_WIN_H__

#include <wx/scrolwin.h>
#include "nwx/TnwxSync.h"
#include "nwx/CIncrementer.h"

class nwxSyncScrollwin : public TnwxSync<wxScrolledWindow,wxPoint>
{
public:
  nwxSyncScrollwin() : m_n(0) {;}
  virtual void SyncThis(wxScrolledWindow *p, wxPoint pt)
  {
    p->Scroll(pt.x, pt.y);
  }
  virtual void Sync(wxScrolledWindow *p)
  {
    if((!m_n) && p->IsShown() && Find(p))
    {
      CIncrementer xxx(m_n);
      wxPoint pt;
      p->GetViewStart(&pt.x,&pt.y);
      SyncTo(pt);
    }
  }
private:
  int m_n; // CIncrementer(m_n) prevents re-entry into Sync
};

#endif
