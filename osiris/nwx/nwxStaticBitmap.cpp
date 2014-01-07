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

*  FileName: nwxStaticBitmap.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwx/nwxStaticBitmap.h"
#include <wx/image.h>
#include <wx/mstream.h>
bool nwxStaticBitmap::m_bPngLoaded = false;

void nwxStaticBitmap::AddPngHandler()
{
  if(!m_bPngLoaded)
  {
    wxImage::AddHandler(new wxPNGHandler);
    m_bPngLoaded = true;
  }
}

bool nwxStaticBitmap::Create(
    wxWindow *parent, 
    int id,
    const wxString &sFileName,
    const wxPoint &pos,
    const wxSize &size,
    long style)
{
  wxBitmap bitmap;
  bool bRtn = LoadImage(sFileName,&bitmap) &&
    wxStaticBitmap::Create(parent,id,bitmap,pos,size,style);
  if(bRtn)
  {
    m_bLoaded = true;
  }
  return bRtn;
}

bool nwxStaticBitmap::Create(
    wxWindow *parent, 
    int id,
    void *pBuffer,
    size_t nSize,
    wxBitmapType nType,
    const wxPoint &pos,
    const wxSize &size,
    long style)
{
  wxBitmap bitmap;
  bool bRtn = LoadImage(pBuffer,nSize,&bitmap,nType) &&
    wxStaticBitmap::Create(parent,id,bitmap,pos,size,style);
  if(bRtn)
  {
    m_bLoaded = true;
  }
  return bRtn;
}

bool nwxStaticBitmap::LoadImage(
  const wxString &sFileName,wxBitmapType nType)
{
  wxBitmap bitmap;
  AddPngHandler();
  bool bRtn = LoadImage(sFileName,&bitmap,nType);
  if(bRtn)
  {
    SetBitmap(bitmap);
    m_bLoaded = true;
  }
  return bRtn;
}

bool nwxStaticBitmap::LoadImage(
  void *pBuffer, size_t nSize, wxBitmapType nType)
{
  wxBitmap bitmap;
  AddPngHandler();
  bool bRtn = LoadImage(pBuffer,nSize,&bitmap,nType);
  if(bRtn)
  {
    SetBitmap(bitmap);
    m_bLoaded = true;
  }
  return bRtn;
}
bool nwxStaticBitmap::LoadImage(
  void *pBuffer, size_t nSize, wxBitmap *pBitmap, wxBitmapType nType)
{
  AddPngHandler();
  wxMemoryInputStream str((const char *)pBuffer, nSize);
  wxImage img(str,nType);
  bool bRtn = img.IsOk();
  if(bRtn)
  {
    wxBitmap bitmap(img);
    bRtn = bitmap.IsOk();
    if(bRtn)
    {
      *pBitmap = bitmap;
    }
  }
  return bRtn;
}
