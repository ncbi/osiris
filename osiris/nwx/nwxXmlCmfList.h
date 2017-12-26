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
*  FileName: nwxXmlCmfList.h
*  Author:   Douglas Hoffman
*  Date:     2017-12-22
*
*   Class for reading CMF enumerations xml file
*   This file is created by running cmf-lists.xsl (in ../OsirisXML)
*   on the CMF xml schema (../OsirisXML/CMF-2017.xsd at this time 2017-12-22)
*/

#ifndef __NWX_XML_CMF_LIST_H__
#define __NWX_XML_CMF_LIST_H__

#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/nwxNameValuePair.h"

class nwxXmlCmfList : public nwxXmlPersist
{
public:
  nwxXmlCmfList();
  nwxXmlCmfList(const wxString &sFilePath);
  virtual void RegisterAll(bool b = false);
  const std::vector<wxString> &GetKits() const
  {
    return m_vsKitType.GetVector();
  }
  const std::vector<wxString> &GetSpecimenCategories() const
  {
    return m_vsSpecimenCategoryType.GetVector();
  }
  const std::vector<wxString> &GetLoci() const
  {
    return m_vsLocusNameType.GetVector();
  }
  const std::vector<nwxNameValuePair *> &GetLocusAlias() const
  {
    return *(m_vLocusAlias.Get());
  }
  virtual const wxString &RootNode(void) const
  {
    return g_sROOT_NODE;
  }

private:
  void _Cleanup()
  {
    m_vsKitType.Init();
    m_vsSpecimenCategoryType.Init();
    m_vsLocusNameType.Init();
    m_vLocusAlias.Init();
  }
  void _LoadFromMemory();
  nwxXmlPersistVectorWxString m_vsKitType;
  nwxXmlPersistVectorWxString m_vsSpecimenCategoryType;
  nwxXmlPersistVectorWxString m_vsLocusNameType;
  TnwxXmlPersistVector<nwxNameValuePair> m_vLocusAlias;

  static const wxString g_sROOT_NODE;
  static const wxString g_sITEM;
};

#endif
