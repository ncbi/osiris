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

*  FileName: CKitColors.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_KIT_COLORS_H__
#define __C_KIT_COLORS_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "wxIDS.h"
#include "ConfigDir.h"

class CChannelColors : public nwxXmlPersist
{
public:
  CChannelColors() 
  {
    RegisterAll(true); 
  }
  virtual ~CChannelColors() 
  {
  }
  unsigned int m_nr;
  wxString m_sDyeName;
  wxColour m_ColorRaw;
  wxColour m_ColorAnalyzed;
  wxColour m_ColorLadder;
protected:
  virtual void RegisterAll(bool bInConstructor = false);
};

class CSingleKitColors : public nwxXmlPersist
{
public:
  CSingleKitColors() : m_IOChannelColors(true)
  {
    RegisterAll(true);
  }
  virtual ~CSingleKitColors() 
  {
    m_IOChannelColors.Cleanup();
  }
  const CChannelColors *GetColorChannel(unsigned int nChannel) const;
  size_t ChannelCount() const
  {
    return m_vChannelColors.size();
  }
  const wxString &GetKitName() const
  {
    return m_sKitName;
  }
protected:
  virtual void RegisterAll(bool bInConstructor = false);
private:
  wxString m_sKitName;
  vector<CChannelColors *> m_vChannelColors;
  mutable map<unsigned int, CChannelColors *> m_mapChannelColors;
  TnwxXmlIOPersistVector<CChannelColors> m_IOChannelColors;
};

class CKitColors : public nwxXmlPersist
{
public:
  CKitColors() : m_IOkitColors(true)
  {
    RegisterAll(true);
    Load();
  }
  virtual ~CKitColors()
  {
    m_IOkitColors.Cleanup();
  }
  virtual void Init()
  {
    m_mapKitColors.clear();
    nwxXmlPersist::Init();
  }
  virtual void Init(void *)
  {
    Init();
  }
  bool Load();

  const wxColour &GetColor(
    const wxString &sKitName, DATA_TYPE n, unsigned int nChannel);

  const CSingleKitColors *GetKitColors(
    const wxString &sKitName) const;

  const CChannelColors *GetColorChannel(
    const wxString  &sKitName, unsigned int nChannel) const
  {
    const CSingleKitColors *pKit = GetKitColors(sKitName);
    const CChannelColors *pRtn = 
      (pKit == NULL) ? NULL : pKit->GetColorChannel(nChannel);
    return pRtn;
  }

protected:
  virtual void RegisterAll(bool bInConstructor = false);
private:
  static const wxColour g_BLACK;
  vector<CSingleKitColors *> m_vpKitColors;
  mutable map<wxString,CSingleKitColors *> m_mapKitColors;
  TnwxXmlIOPersistVector<CSingleKitColors> m_IOkitColors;
};



#endif
