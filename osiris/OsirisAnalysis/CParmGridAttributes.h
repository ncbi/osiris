#ifndef __C__PARM_GRID_ATTRIBUTES_H__
#define __C__PARM_GRID_ATTRIBUTES_H__
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
*  FileName: CParmGridAttributes.h
*  Author:   Douglas Hoffman
*
*/

#include "nwx/nwxXmlPersist.h"

typedef enum 
{
  GRID_NORMAL = 0,
  GRID_ALERT_SAMPLE = 1,
  GRID_ALERT_CELL = 3,   // 1 | 2
  GRID_EDITED_SAMPLE = 4,
  GRID_EDITED_CELL = 12, // 4 | 8
  GRID_EDITED_ALERT_SAMPLE = 5, // GRID_ALERT_SAMPLE | GRID_EDITED_SAMPLE
  GRID_EDITED_ALERT_CELL = 15,  // all the above
  GRID_DISABLED = 16,

  GRID_AND_FLAG = 31, //  all the above or'ed

  GRID_ACCEPTED = 32,
  GRID_REVIEWED = 64

} GRID_FLAG;

typedef enum
{
  GRID_ITALIC = 1,
  GRID_BOLD = 2,
  GRID_REVERSE = 4,
  GRID_ATTR_ALL = 7
} GRID_ATTRIBUTE;

inline GRID_FLAG operator |=(GRID_FLAG &gf1, GRID_FLAG gf2)
{
  gf1 = (GRID_FLAG)(gf1 | gf2);
  return gf1;
}

class CParmGridAttributes : public nwxXmlPersist
{
public:
  CParmGridAttributes()
  {
    SetDefaultColours();
    RegisterAll(true);
  };
  CParmGridAttributes(const CParmGridAttributes &x)
  {
    _CheckSetup();
    RegisterAll(true);
    (*this) = x;
  }
  bool operator == (const CParmGridAttributes &x) const;
  bool operator != (const CParmGridAttributes &x) const
  {
    bool nEq = ((*this) == x);
    return !nEq;
  }
  CParmGridAttributes & operator = (const CParmGridAttributes &x);

  virtual bool Skip();
  virtual bool Skip(void *pOBJ);



  bool SetDefaultColours();
  bool SetDefaults()
  {
    return SetDefaultColours();
  }
  bool SetBackground(int nGRID_FLAG, const wxColour &c);
  bool SetForeground(int nGRID_FLAG, const wxColour &c);
  bool SetNormalForeground(const wxColour &c)
  {
    return SetForeground(GRID_NORMAL,c);
  }
  bool SetAlertForeground(const wxColour &c)
  {
    return SetForeground(GRID_ALERT_CELL,c);
  }
  bool SetDisabledForeground(const wxColour &c)
  {
    return SetForeground(GRID_DISABLED,c);
  }
  bool SetStatusForeground(const wxColour cFG, bool bNeedsAttn)
  {
    wxColour *pc(
      bNeedsAttn ? &m_cFG_STATUS_NEEDS_ATTN : &m_cFG_STATUS_OK);
    bool bRtn = _SetColour(pc,cFG);
    return bRtn;
  }
  bool SetStatusBackground(const wxColour cBG,bool bNeedsAttn)
  {
    wxColour *pc(
      bNeedsAttn ? &m_cBG_STATUS_NEEDS_ATTN : &m_cBG_STATUS_OK);
    bool bRtn = _SetColour(pc,cBG);
    return bRtn;
  }
  static bool SetAttr(int *pn, int n)
  {
    n &= (int) GRID_ATTR_ALL;
    bool bRtn = (n != *pn);
    *pn = n;
    return bRtn;
  }
  bool SetAttrNeedsAttn(int n)
  {
    return SetAttr(&m_nATTR_NEED_ATTN,n);
  }
  bool SetAttrEdited(int n)
  {
    return SetAttr(&m_nATTR_EDITED,n);
  }
  const wxColour &GetBackground(int nGRID_FLAG) const;
  const wxColour &GetForeground(int nGRID_FLAG) const;
  const wxColour &GetStatusForeground(bool bNeedsAttn) const
  {
    const wxColour &c(
      bNeedsAttn ? m_cFG_STATUS_NEEDS_ATTN : m_cFG_STATUS_OK);
    return c;
  }
  const wxColour &GetStatusBackground(bool bNeedsAttn) const
  {
    const wxColour &c(
      bNeedsAttn ? m_cBG_STATUS_NEEDS_ATTN : m_cBG_STATUS_OK);
    return c;
  }
  int GetAttrNeedsAttn() const
  {
    return m_nATTR_NEED_ATTN;
  }
  int GetAttrEdited() const
  {
    return m_nATTR_EDITED;
  }
  
  bool IsItalic(int n) const
  {
    return _HasAttribute(n,GRID_ITALIC);
  }
  bool IsBold(int nGRID_FLAG) const
  {
    return _HasAttribute(nGRID_FLAG,GRID_BOLD);
  }
  bool IsReverse(int nGRID_FLAG) const
  {
    return _HasAttribute(nGRID_FLAG,GRID_REVERSE);
  }

protected:
  virtual void RegisterAll(bool bInConstructor = false);
  bool _HasAttribute(int nGRID_FLAG, GRID_ATTRIBUTE Attr) const;

private:
  // default colours, static

  static const wxColour g_cBG_ALERT_CELL;
  static const wxColour g_cBG_ALERT_SAMPLE;
  static const wxColour g_cBG_EDITED_CELL;
  static const wxColour g_cBG_EDITED_SAMPLE;
  static const wxColour g_cBG_EDITED_ALERT_CELL;
  static const wxColour g_cBG_EDITED_ALERT_SAMPLE;
  static const wxColour g_cBG_DISABLED;
  static const wxColour g_cBG_NORMAL;
  static const wxColour g_cBG_STATUS_OK;
  static const wxColour g_cBG_STATUS_NEEDS_ATTN;

  static const wxColour g_cFG_STATUS_OK;
  static const wxColour g_cFG_STATUS_NEEDS_ATTN;
  static const wxColour g_cFG_NORMAL;
  static const wxColour g_cFG_ALERT;
  static const wxColour g_cFG_DISABLED;

  static const int g_nATTR_NEED_ATTN;
  static const int g_nATTR_EDITED;


  // io for colours, static

  static nwxXmlIOwxColour g_ioBG_ALERT_CELL;
  static nwxXmlIOwxColour g_ioBG_ALERT_SAMPLE;
  static nwxXmlIOwxColour g_ioBG_EDITED_CELL;
  static nwxXmlIOwxColour g_ioBG_EDITED_SAMPLE;
  static nwxXmlIOwxColour g_ioBG_EDITED_ALERT_CELL;
  static nwxXmlIOwxColour g_ioBG_EDITED_ALERT_SAMPLE;
  static nwxXmlIOwxColour g_ioBG_DISABLED;
  static nwxXmlIOwxColour g_ioBG_NORMAL;
  static nwxXmlIOwxColour g_ioBG_STATUS_OK;
  static nwxXmlIOwxColour g_ioBG_STATUS_NEEDS_ATTN;

  static nwxXmlIOwxColour g_ioFG_STATUS_OK;
  static nwxXmlIOwxColour g_ioFG_STATUS_NEEDS_ATTN;
  static nwxXmlIOwxColour g_ioFG_NORMAL;
  static nwxXmlIOwxColour g_ioFG_ALERT;
  static nwxXmlIOwxColour g_ioFG_DISABLED;

  static nwxXmlIOint g_ioATTR_NEED_ATTN;
  static nwxXmlIOint g_ioATTR_EDITED;



  // actual colours

  wxColour m_cBG_ALERT_CELL;
  wxColour m_cBG_ALERT_SAMPLE;
  wxColour m_cBG_EDITED_CELL;
  wxColour m_cBG_EDITED_SAMPLE;
  wxColour m_cBG_EDITED_ALERT_CELL;
  wxColour m_cBG_EDITED_ALERT_SAMPLE;
  wxColour m_cBG_DISABLED;
  wxColour m_cBG_NORMAL;
  wxColour m_cBG_STATUS_OK;
  wxColour m_cBG_STATUS_NEEDS_ATTN;


  wxColour m_cFG_ALERT_CELL;
  wxColour m_cFG_ALERT_SAMPLE;
  wxColour m_cFG_EDITED_CELL;
  wxColour m_cFG_EDITED_SAMPLE;
  wxColour m_cFG_EDITED_ALERT_CELL;
  wxColour m_cFG_EDITED_ALERT_SAMPLE;
  wxColour m_cFG_DISABLED;
  wxColour m_cFG_NORMAL;
  wxColour m_cFG_STATUS_OK;
  wxColour m_cFG_STATUS_NEEDS_ATTN;

  int m_nATTR_NEED_ATTN;
  int m_nATTR_EDITED;


  vector<wxColour *> m_vpColours;
  static vector<nwxXmlIOwxColour *> g_vpioColours;
  static vector<const wxColour *> g_vpDefaultColours;
  static vector<const wxChar *> g_vpsNames;

  wxColour *_GetForegroundPtr(int nGRID_FLAG);
  wxColour *_GetBackgroundPtr(int nGRID_FLAG, wxColour *pDefault = NULL);
  const wxColour *_GetColorPtrC(
    int nGRID_FLAG, const wxColour *pDefault, bool bBG) const;
  const wxColour *_GetForegroundPtrC(int nGRID_FLAG) const
  {
    return _GetColorPtrC(nGRID_FLAG,&m_cFG_NORMAL,false);
  }
  const wxColour *_GetBackgroundPtrC(int nGRID_FLAG, const wxColour *pDefault = NULL) const
  {
    return _GetColorPtrC(nGRID_FLAG,pDefault,true);
  }
  void _Setup();
  void _CheckSetup()
  {
    if(!m_vpColours.size())
    {
      _Setup();
    }
  }
  static bool _SetColour(wxColour *pc, const wxColour &c);


};

#endif

