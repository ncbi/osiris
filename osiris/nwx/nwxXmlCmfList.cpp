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
*  FileName: nwxXmlCmfList.cpp
*  Author:   Douglas Hoffman
*  Date:     2017-12-22
*
*   Class for reading CMF enumerations xml file
*   This file is created by running cmf-lists.xsl (in ../OsirisXML)
*   on the CMF xml schema (../OsirisXML/CMF-2017.xsd at this time 2017-12-22)
*/

#include "nwx/nwxXmlCmfList.h"
#include "nwx/nwxXmlCmfListStatic.h"
#include <wx/mstream.h>
#include <wx/filename.h>

const wxString nwxXmlCmfList::g_sROOT_NODE(wxS("CmfList"));
const wxString nwxXmlCmfList::g_sITEM(wxS("Item"));

#define INIT_MEMBERS \
  m_vsKitType(g_sITEM,true), \
  m_vsSpecimenCategoryType(g_sITEM,true), \
  m_vsLocusNameType(g_sITEM,true), \
  m_vLocusAlias(g_sITEM,true)

nwxXmlCmfList::nwxXmlCmfList() : INIT_MEMBERS
{
  RegisterAll();
  _LoadFromMemory();
}

nwxXmlCmfList::nwxXmlCmfList(const wxString &sFilePath) : INIT_MEMBERS
{
  RegisterAll();
  if(!( wxFileName::FileExists(sFilePath) && LoadFile(sFilePath) ))
  {
    _LoadFromMemory();
  }
}

void nwxXmlCmfList::_LoadFromMemory()
{
  wxMemoryInputStream st((void *) nwxXmlCmfListStatic, sizeof(nwxXmlCmfListStatic));
  if(!LoadStream(st))
  {
    _Cleanup();
  }
}
void nwxXmlCmfList::RegisterAll(bool)
{
  Register(wxS("KitType"),&m_vsKitType);
  Register(wxS("SpecimenCategoryType"),&m_vsSpecimenCategoryType);
  Register(wxS("LocusNameType"),&m_vsLocusNameType);
  Register(wxS("LocusAlias"), &m_vLocusAlias);
}
