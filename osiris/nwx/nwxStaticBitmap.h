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
*  FileName: nwxStaticBitmap.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_STATIC_BITMAP__
#define __NWX_STATIC_BITMAP__

#include <wx/window.h>
#include <wx/statbmp.h>
#include <wx/bitmap.h>

class nwxStaticBitmap : public wxStaticBitmap
{
public:
  nwxStaticBitmap() : wxStaticBitmap() 
  {
    m_bLoaded = false;
  }
  nwxStaticBitmap(
    wxWindow *parent, 
    int id,
    const wxString &sFileName,
    const wxPoint &pos = wxDefaultPosition,
    const wxSize &size = wxDefaultSize,
    long style = 0)
  {
    m_bLoaded = false;
    Create(parent,id,sFileName,pos,size,style);
  }
  nwxStaticBitmap(
    wxWindow *parent, 
    int id,
    void *pBuffer,
    size_t nSize,
    wxBitmapType nType = wxBITMAP_TYPE_ANY,
    const wxPoint &pos = wxDefaultPosition,
    const wxSize &size = wxDefaultSize,
    long style = 0)
  {
    m_bLoaded = false;
    Create(parent,id,pBuffer,nSize,nType,pos,size,style);
  }

  virtual ~nwxStaticBitmap() {;}
  bool Create(
    wxWindow* parent, 
    int id, 
    const wxString &sFileName,
    const wxPoint &pos = wxDefaultPosition,
    const wxSize &size = wxDefaultSize,
    long style = 0);
  bool Create(
    wxWindow* parent, 
    int id, 
    void *pBuffer,
    size_t nSize,
    wxBitmapType nType = wxBITMAP_TYPE_ANY,
    const wxPoint &pos = wxDefaultPosition,
    const wxSize &size = wxDefaultSize,
    long style = 0);

  bool LoadImage(const wxString &sFileName,wxBitmapType nType = wxBITMAP_TYPE_ANY);
  bool LoadImage(void *pBuffer, size_t nSize,wxBitmapType nType = wxBITMAP_TYPE_ANY);
  static bool LoadImage(
    void *pBuffer, size_t nSize, wxBitmap *pBitmap,
    wxBitmapType nType = wxBITMAP_TYPE_ANY);
  bool IsImageLoaded()
  {
    return m_bLoaded;
  }
  static void AddPngHandler();
private:
  static bool LoadImage(const wxString &sFileName, wxBitmap *pBitmap, wxBitmapType nType = wxBITMAP_TYPE_ANY)
  {
    bool bRtn = pBitmap->LoadFile(sFileName,nType);
    return bRtn;
  }
  bool m_bLoaded;
  static bool m_bPngLoaded;
};


#endif
