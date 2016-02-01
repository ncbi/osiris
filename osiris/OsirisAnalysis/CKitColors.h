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
*
*  FileName: CKitColors.h
*  Author:   Douglas Hoffman
*
*  1/26/15 - Using the file kitcolors2.0.xml
*    load the data structure and maintaining compatibility to the older version
*    that used kitcolors.xml.  The classes CKitColors and CSingleKitColors
*    are no longer subclasses of nwxXmlPersist, but use the classes 
*    CKitColors2 and CPersistKitList (CKitList.cpp/h)
*    to load the XML data.
*/
#ifndef __C_KIT_COLORS_H__
#define __C_KIT_COLORS_H__

#include <wx/string.h>
#include <wx/colour.h>
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"

class CPersistKitList; // CKitList.h
class CKitChannel;     // CKitList.h
class CKitColors2;     // CKitColors2.h
class CKitColorDye;    // CKitColors2.h

class CChannelColors
{
public:
  CChannelColors(
      const CKitChannel *pChannel, 
      const CKitColors2 *p);
  CChannelColors(unsigned int nChannel, const wxString &sDyeName, const CKitColors2 *p);
  virtual ~CChannelColors() {}
  const wxString &GetDyeName() const
  {
    return m_sDyeName;
  }
  unsigned int GetChannelNumber() const
  {
    return m_nr;
  }
  const wxColour GetColorAnalyzed() const
  {
    return m_ColorAnalyzed;
  }
  const wxColour GetColorLadder() const
  {
    return m_ColorLadder;
  }
  const wxColour GetColorRaw() const
  {
    return m_ColorRaw;
  }
  const wxColour *GetColorAnalyzedPtr() const
  {
    return &m_ColorAnalyzed;
  }
  const wxColour *GetColorLadderPtr() const
  {
    return &m_ColorLadder;
  }
  const wxColour *GetColorRawPtr() const
  {
    return &m_ColorRaw;
  }
  const wxColour *GetColorPtr(DATA_TYPE n) const;
private:
  void _SetColors(const CKitColorDye *pDye);
  unsigned int m_nr;
  wxString m_sDyeName;
  wxColour m_ColorRaw;
  wxColour m_ColorAnalyzed;
  wxColour m_ColorLadder;
};

class CSingleKitColors
{
  //  need to accommodate multiple ILS dye names/colors

public:
  CSingleKitColors(
    const wxString &sKitName,
    CPersistKitList *pKitList,
    const CKitColors2 *pColors2);
  virtual ~CSingleKitColors();
  const CChannelColors *GetColorChannel(unsigned int nChannel) const;
  const CChannelColors *GetILSChannel(const wxString &sILSFamily);
  size_t ChannelCount() const
  {
    return m_mapChannelColors.size();
  }
  const wxString &GetKitName() const
  {
    return m_sKitName;
  }
private:
  std::map<unsigned int, CChannelColors *> m_mapChannelColors;
  std::map<const wxString,CChannelColors *> m_mapILSColors;
  wxString m_sKitName;
  unsigned int m_nILSchannel;
};

class CKitColors
{
public:
  CKitColors() : m_pKitColors2(NULL)
  {
    _Load();
  }
  virtual ~CKitColors();

  const wxColour &GetColor(
    const wxString &sKitName, DATA_TYPE n, unsigned int nChannel) const;
  const wxColour &GetColorByDye(const wxString &sDyeName, DATA_TYPE n = ANALYZED_DATA) const;

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
  static const wxColour g_BLACK;

private:
  bool _Load();
  std::map<const wxString,CSingleKitColors *> m_mapKitColors;
  CKitColors2 *m_pKitColors2;
};



#endif
