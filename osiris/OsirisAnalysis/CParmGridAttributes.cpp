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
*  FileName: CParmGridAttributes.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CParmGridAttributes.h"

// default colors




const wxColour CParmGridAttributes::g_cBG_ALERT_CELL(255,255,169);
const wxColour CParmGridAttributes::g_cBG_ALERT_SAMPLE(255,255,224);
const wxColour CParmGridAttributes::g_cBG_EDITED_CELL(208,208,255);
const wxColour CParmGridAttributes::g_cBG_EDITED_SAMPLE(232,232,255);
const wxColour CParmGridAttributes::g_cBG_EDITED_ALERT_CELL(208,208,255);//(255,192,128);
const wxColour CParmGridAttributes::g_cBG_EDITED_ALERT_SAMPLE(255,255,224); //(255,236,224);
const wxColour CParmGridAttributes::g_cBG_DISABLED(240,240,240);
const wxColour CParmGridAttributes::g_cBG_NORMAL(255,255,255);

const wxColour CParmGridAttributes::g_cBG_STATUS_NEEDS_ATTN(192,0,0);
const wxColour CParmGridAttributes::g_cBG_STATUS_OK(255,255,255);
const wxColour CParmGridAttributes::g_cFG_STATUS_NEEDS_ATTN(255,255,255);
const wxColour CParmGridAttributes::g_cFG_STATUS_OK(0,192,0);


const wxColour CParmGridAttributes::g_cFG_NORMAL(0,0,0);
const wxColour CParmGridAttributes::g_cFG_ALERT(192,0,0);
const wxColour CParmGridAttributes::g_cFG_DISABLED(176,176,176);

const int CParmGridAttributes::g_nATTR_NEED_ATTN((int) GRID_BOLD | (int) GRID_REVERSE);
const int CParmGridAttributes::g_nATTR_EDITED((int) GRID_ITALIC);


  // io for colours, static

nwxXmlIOwxColour CParmGridAttributes::g_ioBG_ALERT_CELL(g_cBG_ALERT_CELL,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioBG_ALERT_SAMPLE(g_cBG_ALERT_SAMPLE,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioBG_EDITED_CELL(g_cBG_EDITED_CELL,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioBG_EDITED_SAMPLE(g_cBG_EDITED_SAMPLE,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioBG_EDITED_ALERT_CELL(g_cBG_EDITED_ALERT_CELL,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioBG_EDITED_ALERT_SAMPLE(g_cBG_EDITED_ALERT_SAMPLE,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioBG_DISABLED(g_cBG_DISABLED,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioBG_NORMAL(g_cBG_NORMAL,true);

nwxXmlIOwxColour CParmGridAttributes::g_ioBG_STATUS_OK(g_cBG_STATUS_OK,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioBG_STATUS_NEEDS_ATTN(g_cBG_STATUS_NEEDS_ATTN,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioFG_STATUS_OK(g_cFG_STATUS_OK,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioFG_STATUS_NEEDS_ATTN(g_cFG_STATUS_NEEDS_ATTN,true);

nwxXmlIOwxColour CParmGridAttributes::g_ioFG_NORMAL(g_cFG_NORMAL,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioFG_ALERT(g_cFG_ALERT,true);
nwxXmlIOwxColour CParmGridAttributes::g_ioFG_DISABLED(g_cFG_DISABLED,true);

nwxXmlIOint CParmGridAttributes::g_ioATTR_NEED_ATTN(g_nATTR_NEED_ATTN);
nwxXmlIOint CParmGridAttributes::g_ioATTR_EDITED(g_nATTR_EDITED);

// for default and io on foreground colours, use #defines
// because they are mostly the same

#define g_ioFG_ALERT_CELL g_ioFG_ALERT
#define g_ioFG_ALERT_SAMPLE  g_ioFG_NORMAL
#define g_ioFG_EDITED_CELL g_ioFG_NORMAL
#define g_ioFG_EDITED_SAMPLE g_ioFG_NORMAL
#define g_ioFG_EDITED_ALERT_CELL g_ioFG_ALERT
#define g_ioFG_EDITED_ALERT_SAMPLE g_ioFG_NORMAL

#define g_cFG_ALERT_CELL g_cFG_ALERT
#define g_cFG_ALERT_SAMPLE  g_cFG_NORMAL
#define g_cFG_EDITED_CELL g_cFG_NORMAL
#define g_cFG_EDITED_SAMPLE g_cFG_NORMAL
#define g_cFG_EDITED_ALERT_CELL g_cFG_ALERT
#define g_cFG_EDITED_ALERT_SAMPLE g_cFG_NORMAL

vector<nwxXmlIOwxColour *> CParmGridAttributes::g_vpioColours;
vector<const wxColour *> CParmGridAttributes::g_vpDefaultColours;
vector<const wxChar *> CParmGridAttributes::g_vpsNames;


void CParmGridAttributes::_Setup()
{
  const size_t COUNT = 20;
  if(!m_vpColours.size())
  {
    if(!g_vpioColours.size())
    {
      g_vpioColours.reserve(COUNT);
      g_vpsNames.reserve(COUNT);
      g_vpDefaultColours.reserve(COUNT);

      g_vpDefaultColours.push_back(&g_cBG_ALERT_CELL);
      g_vpDefaultColours.push_back(&g_cBG_ALERT_SAMPLE);
      g_vpDefaultColours.push_back(&g_cBG_EDITED_CELL);
      g_vpDefaultColours.push_back(&g_cBG_EDITED_SAMPLE);
      g_vpDefaultColours.push_back(&g_cBG_EDITED_ALERT_CELL);
      g_vpDefaultColours.push_back(&g_cBG_EDITED_ALERT_SAMPLE);
      g_vpDefaultColours.push_back(&g_cBG_DISABLED);
      g_vpDefaultColours.push_back(&g_cBG_NORMAL);
      g_vpDefaultColours.push_back(&g_cBG_STATUS_OK);
      g_vpDefaultColours.push_back(&g_cBG_STATUS_NEEDS_ATTN);

      g_vpDefaultColours.push_back(&g_cFG_ALERT_CELL);
      g_vpDefaultColours.push_back(&g_cFG_ALERT_SAMPLE);
      g_vpDefaultColours.push_back(&g_cFG_EDITED_CELL);
      g_vpDefaultColours.push_back(&g_cFG_EDITED_SAMPLE);
      g_vpDefaultColours.push_back(&g_cFG_EDITED_ALERT_CELL);
      g_vpDefaultColours.push_back(&g_cFG_EDITED_ALERT_SAMPLE);
      g_vpDefaultColours.push_back(&g_cFG_DISABLED);
      g_vpDefaultColours.push_back(&g_cFG_NORMAL);
      g_vpDefaultColours.push_back(&g_cFG_STATUS_OK);
      g_vpDefaultColours.push_back(&g_cFG_STATUS_NEEDS_ATTN);


      g_vpioColours.push_back(&g_ioBG_ALERT_CELL);
      g_vpioColours.push_back(&g_ioBG_ALERT_SAMPLE);
      g_vpioColours.push_back(&g_ioBG_EDITED_CELL);
      g_vpioColours.push_back(&g_ioBG_EDITED_SAMPLE);
      g_vpioColours.push_back(&g_ioBG_EDITED_ALERT_CELL);
      g_vpioColours.push_back(&g_ioBG_EDITED_ALERT_SAMPLE);
      g_vpioColours.push_back(&g_ioBG_DISABLED);
      g_vpioColours.push_back(&g_ioBG_NORMAL);
      g_vpioColours.push_back(&g_ioBG_STATUS_OK);
      g_vpioColours.push_back(&g_ioBG_STATUS_NEEDS_ATTN);

      g_vpioColours.push_back(&g_ioFG_ALERT_CELL);
      g_vpioColours.push_back(&g_ioFG_ALERT_SAMPLE);
      g_vpioColours.push_back(&g_ioFG_EDITED_CELL);
      g_vpioColours.push_back(&g_ioFG_EDITED_SAMPLE);
      g_vpioColours.push_back(&g_ioFG_EDITED_ALERT_CELL);
      g_vpioColours.push_back(&g_ioFG_EDITED_ALERT_SAMPLE);
      g_vpioColours.push_back(&g_ioFG_DISABLED);
      g_vpioColours.push_back(&g_ioFG_NORMAL);
      g_vpioColours.push_back(&g_ioFG_STATUS_OK);
      g_vpioColours.push_back(&g_ioFG_STATUS_NEEDS_ATTN);


      g_vpsNames.push_back(_T("AlertCellBG"));
      g_vpsNames.push_back(_T("AlertSampleBG"));
      g_vpsNames.push_back(_T("EditedCellBG"));
      g_vpsNames.push_back(_T("EditedSampleBG"));
      g_vpsNames.push_back(_T("EditedAlertCellBG"));
      g_vpsNames.push_back(_T("EditedAlertSampleBG"));
      g_vpsNames.push_back(_T("DisabledBG"));
      g_vpsNames.push_back(_T("ColorBG"));
      g_vpsNames.push_back(_T("StatusOKBG"));
      g_vpsNames.push_back(_T("StatusATTNBG"));

      g_vpsNames.push_back(_T("AlertCellFG"));
      g_vpsNames.push_back(_T("AlertSampleFG"));
      g_vpsNames.push_back(_T("EditedCellFG"));
      g_vpsNames.push_back(_T("EditedSampleFG"));
      g_vpsNames.push_back(_T("EditedAlertCellFG"));
      g_vpsNames.push_back(_T("EditedAlertSampleFG"));
      g_vpsNames.push_back(_T("DisabledFG"));
      g_vpsNames.push_back(_T("ColorFG"));
      g_vpsNames.push_back(_T("StatusOKFG"));
      g_vpsNames.push_back(_T("StatusATTNFG"));

    }
    m_vpColours.reserve(COUNT);
    m_vpColours.push_back(&m_cBG_ALERT_CELL);
    m_vpColours.push_back(&m_cBG_ALERT_SAMPLE);
    m_vpColours.push_back(&m_cBG_EDITED_CELL);
    m_vpColours.push_back(&m_cBG_EDITED_SAMPLE);
    m_vpColours.push_back(&m_cBG_EDITED_ALERT_CELL);
    m_vpColours.push_back(&m_cBG_EDITED_ALERT_SAMPLE);
    m_vpColours.push_back(&m_cBG_DISABLED);
    m_vpColours.push_back(&m_cBG_NORMAL);
    m_vpColours.push_back(&m_cBG_STATUS_OK);
    m_vpColours.push_back(&m_cBG_STATUS_NEEDS_ATTN);

    m_vpColours.push_back(&m_cFG_ALERT_CELL);
    m_vpColours.push_back(&m_cFG_ALERT_SAMPLE);
    m_vpColours.push_back(&m_cFG_EDITED_CELL);
    m_vpColours.push_back(&m_cFG_EDITED_SAMPLE);
    m_vpColours.push_back(&m_cFG_EDITED_ALERT_CELL);
    m_vpColours.push_back(&m_cFG_EDITED_ALERT_SAMPLE);
    m_vpColours.push_back(&m_cFG_DISABLED);
    m_vpColours.push_back(&m_cFG_NORMAL);
    m_vpColours.push_back(&m_cFG_STATUS_OK);
    m_vpColours.push_back(&m_cFG_STATUS_NEEDS_ATTN);

  }
}

bool CParmGridAttributes::operator ==(const CParmGridAttributes &x) const
{
  bool bRtn = true;

  if(m_nATTR_NEED_ATTN != x.m_nATTR_NEED_ATTN) { bRtn = false; }
  else if(m_nATTR_EDITED != x.m_nATTR_EDITED)  { bRtn = false; }
  else
  {
    vector<wxColour *>::const_iterator itr1 = m_vpColours.begin();
    vector<wxColour *>::const_iterator itr2 = x.m_vpColours.begin();
    size_t nSize = m_vpColours.size();

    for(size_t i = 0; i < nSize; i++)
    {
      if( (*(*itr1)) != (*(*itr2)) )
      {
        bRtn = false;
        i = nSize;
      }
      else
      {
        ++itr1;
        ++itr2;
      }
    }
  }
  return bRtn;
}

CParmGridAttributes& CParmGridAttributes::operator=(const CParmGridAttributes &x)
{
  vector<wxColour *>::iterator itr1 = m_vpColours.begin();
  vector<wxColour *>::const_iterator itr2 = x.m_vpColours.begin();
  size_t nSize = m_vpColours.size();

  m_nATTR_NEED_ATTN = x.m_nATTR_NEED_ATTN;
  m_nATTR_EDITED = x.m_nATTR_EDITED;
  
  for(size_t i = nSize; i < nSize; i++)
  {
    (*(*itr1)) = (*(*itr2));
  }
  return *this;
}
void CParmGridAttributes::RegisterAll(bool)
{
  wxString s;
  vector<nwxXmlIOwxColour *>::iterator itrio = g_vpioColours.begin();
  vector<wxColour *>::iterator itr = m_vpColours.begin();
  vector<const wxChar *>::iterator itrName = g_vpsNames.begin();
  size_t nCount = g_vpioColours.size();
  for(size_t i = 0; i < nCount; i++)
  {
    s = *itrName;
    Register(s,*itrio,(void *)(*itr));
    ++itr;
    ++itrName;
    ++itrio;
  }
  Register(_T("AttrNeedsAttn"),&g_ioATTR_NEED_ATTN,&m_nATTR_NEED_ATTN);
  Register(_T("AttrEdited"),&g_ioATTR_EDITED,&m_nATTR_EDITED);
}
bool CParmGridAttributes::Skip()
{
  size_t nCount = g_vpioColours.size();
  nwxXmlIOwxColour *pio;
  wxColour *pc;
  bool bRtn = true;
  vector<nwxXmlIOwxColour *>::iterator itrio = g_vpioColours.begin();
  vector<wxColour *>::iterator itr = m_vpColours.begin();
  if( (m_nATTR_NEED_ATTN != g_nATTR_NEED_ATTN) ||
      (m_nATTR_EDITED != g_nATTR_EDITED) )
  {
    bRtn = false;
    nCount = 0;
  }
  for(size_t i = 0; i < nCount; i++)
  {
    pio = *itrio;
    pc = *itr;
    if(!pio->Skip((void *)pc))
    {
      bRtn = false;
      i = nCount; // loop exit
    }
    else
    {
      ++itr;
      ++itrio;
    }
  }
  return bRtn;
}

bool CParmGridAttributes::Skip(void *pObj)
{
  CParmGridAttributes *p = (CParmGridAttributes *)pObj;
  bool bRtn = p->Skip();
  return bRtn;
}
bool CParmGridAttributes::_SetColour(wxColour *pc, const wxColour &c)
{
  bool bRtn = ((*pc) != c);
  if(bRtn)
  {
    *pc = c;
  }
  return bRtn;
}
bool CParmGridAttributes::SetDefaultColours()
{
  _CheckSetup();
  size_t nCount = g_vpioColours.size();
  vector<wxColour *>::iterator itr = m_vpColours.begin();
  vector<const wxColour *>::iterator itrd = g_vpDefaultColours.begin();
  bool bRtn = false;
  for(size_t i = 0; i < nCount; i++)
  {
    if(_SetColour(*itr,*(*itrd)))
    {
      bRtn = true;
    }
    ++itr;
    ++itrd;
  }
  m_nATTR_NEED_ATTN = g_nATTR_NEED_ATTN;
  m_nATTR_EDITED = g_nATTR_EDITED;
  return bRtn;
}

const wxColour &CParmGridAttributes::GetForeground(int n) const
{
  const wxColour *pc = _GetForegroundPtrC(n);
  return *pc;
}
const wxColour &CParmGridAttributes::GetBackground(int n) const
{
  const wxColour *pc = _GetBackgroundPtrC(n);
  return *pc;
}
bool CParmGridAttributes::SetForeground(
  int nGRID_FLAG, const wxColour &c)
{
  wxColour *pc =_GetForegroundPtr(nGRID_FLAG);
  bool bRtn = _SetColour(pc,c);
  return bRtn;
}
wxColour *CParmGridAttributes::_GetForegroundPtr(
  int nGRID_FLAG)
{
  const wxColour *p = _GetForegroundPtrC(nGRID_FLAG);
  return (wxColour *)p;
}

const wxColour *CParmGridAttributes::_GetColorPtrC(
  int n, const wxColour *pDefault, bool bBG) const
{
  const wxColour *pc = pDefault;

  if(!n)
  {
     // normal
    pc = bBG ? &m_cBG_NORMAL : &m_cFG_NORMAL;
  }

  // check cell status

  else if(n & GRID_DISABLED)
  {
    pc = bBG ? &m_cBG_DISABLED : &m_cFG_DISABLED;
  }
  else if((n & GRID_EDITED_ALERT_CELL)
      == GRID_EDITED_ALERT_CELL)
  {
    pc = bBG ? &m_cBG_EDITED_ALERT_CELL : &m_cFG_EDITED_ALERT_CELL;
  }
  else if((n & GRID_ALERT_CELL) == GRID_ALERT_CELL)
  {
    pc = bBG ? &m_cBG_ALERT_CELL : &m_cFG_ALERT_CELL;
  }
  else if((n & GRID_EDITED_CELL) == GRID_EDITED_CELL)
  {
    pc = bBG ? &m_cBG_EDITED_CELL : &m_cFG_EDITED_CELL;
  }

  //  now check sample status

  else if((n & GRID_EDITED_ALERT_SAMPLE) == GRID_EDITED_ALERT_SAMPLE)
  {
    pc = bBG ? &m_cBG_EDITED_ALERT_SAMPLE : &m_cFG_EDITED_ALERT_SAMPLE;
  }
  else if((n & GRID_ALERT_SAMPLE) == GRID_ALERT_SAMPLE)
  {
    pc = bBG ? &m_cBG_ALERT_SAMPLE : &m_cFG_ALERT_SAMPLE;
  }
  else if((n & GRID_EDITED_SAMPLE) == GRID_EDITED_SAMPLE)
  {
    pc = bBG ? &m_cBG_EDITED_SAMPLE : &m_cFG_EDITED_SAMPLE;
  }
#ifdef _DEBUG
  else
  {
    // all conditions failed
    wxString s;
    s.Printf("CParmGridAttributes::_GetColorPtrC(GRID_FLAG = %d)",(int)n);
    wxASSERT_MSG(0,s);
  }
#endif
  return pc;
}
wxColour *CParmGridAttributes::_GetBackgroundPtr(
  int nGRID_FLAG, wxColour *pDefault)
{
  const wxColour *p = _GetBackgroundPtrC(nGRID_FLAG,pDefault);
  return (wxColour *)p;
}

bool CParmGridAttributes::SetBackground(int nGRID_FLAG,const wxColour &c)
{
  wxColour *pc = _GetBackgroundPtr(nGRID_FLAG,NULL);
  bool bRtn = (pc == NULL) ? false : _SetColour(pc,c);
  return bRtn;
}

bool CParmGridAttributes::_HasAttribute(
  int nGRID_FLAG,GRID_ATTRIBUTE Attr) const
{
  bool bRtn = false;
  int nAttr = (int) Attr;
  if(!!(nGRID_FLAG & GRID_DISABLED))
  {} // disabled, do nothing
  else if( (nGRID_FLAG & GRID_EDITED_CELL) == GRID_EDITED_CELL )
  {
    // edited cell
    if(!!(m_nATTR_EDITED & nAttr))
    {
      // 'edited' attribute match
      bRtn = true;
    }
    else if( (!(nGRID_FLAG & GRID_REVIEWED)) &&
        (!!(m_nATTR_NEED_ATTN & nAttr)) )
    {
      //  edited, not reviewed attention, 
      //  and 'needs attention' attribute match
      bRtn = true;
    }
  }
  else if(
    ((nGRID_FLAG & GRID_ALERT_CELL) == GRID_ALERT_CELL) &&
    (!(nGRID_FLAG & GRID_ACCEPTED)) )
  {
    // has alerts, not edited, not accepted
    bRtn = !!(m_nATTR_NEED_ATTN & nAttr);
  }
  return bRtn;
}
