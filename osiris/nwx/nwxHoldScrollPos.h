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
*  FileName: nwxHoldScrollPos.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_HOLD_SCROLL_POS_H__
#define __NWX_HOLD_SCROLL_POS_H__

#include <wx/scrolwin.h>

class nwxHoldScrollPos
{
public:
  nwxHoldScrollPos(wxScrolledWindow *pWin, bool bTop = true) :
      m_pWin(pWin)
  {
    if(m_pWin != NULL)
    {
      m_pWin->GetViewStart(&m_nx,&m_ny);
      if(bTop)
      {
        m_pWin->Scroll(0,0);
      }
    }
  }
  virtual ~nwxHoldScrollPos()
  {
    if(m_pWin != NULL)
    {
      m_pWin->Scroll(m_nx,m_ny);
    }
  }
private:
  wxScrolledWindow *m_pWin;
  int m_nx;
  int m_ny;
};

#endif
