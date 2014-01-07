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

*  FileName: nwxRectSet.cpp
*  Author:   Douglas Hoffman
*
*/

#include "nwx/nwxRectSet.h"

bool RectCompareLess::operator()(const wxRect &a, const wxRect &b)
{
  bool bRtn = false;
  if(a.GetX() < b.GetX())
  {
    bRtn = true;
  }
  else if(a.GetX() != b.GetX()) {}
  else if(a.GetWidth() < b.GetWidth())
  {
    bRtn = true;
  }
  else 
  {
    int ay2 = a.GetY();
    int ay1 = ay2 + a.GetHeight();
    int by2 = b.GetY();
    int by1 = by2 + b.GetHeight();

    // compare Y in data coords not window coords
    // which is the opposite direction

    if(ay1 > by1)
    {
      // bottom of a is lower than 
      // bottom of b in coord space
      bRtn = true;
    }
    else if(ay1 != by1) {}
    else if(ay2 > by2)
    {
      // top of a is lower than
      // top of b in coord space
      bRtn = true;
    }
  }
  return bRtn;
}

bool nwxRectSet::ClearOverlap(wxRect *pr, bool bUP, bool bInsert)
{
  bool bRtn = false;
  int ny;
  const int ITERATIONS = 4;
  for(int i = 0; i < ITERATIONS; i++)
  {
    bool bChange = false;
    for(iterator itr = begin();
      itr != end();
      ++itr)
    {
      const wxRect &x(*itr);
      if(x.Intersects(*pr))
      {
        if(bUP)
        {
          ny = x.GetY() - pr->GetWidth();
          // this may be negative
        }
        else
        {
          ny = x.GetY() + x.GetWidth();
        }
        pr->SetY(ny);
        bChange = true;
      }
    }
    if(!bChange) 
    {
      i = ITERATIONS; // loop exit
    }
    else
    {
      bRtn = true;
    }
  }
  if(bInsert)
  {
   insert(*pr);
  }
  return bRtn;
}
