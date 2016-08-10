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
*  FileName: nwxXmlWindowSizes.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_WINDOW_SIZES_H__
#define __NWX_WINDOW_SIZES_H__

#include <wx/string.h>
#include <wx/window.h>
#include <wx/splitter.h>
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/nwxTimerReceiver.h"

class nwxWindowSize : public nwxXmlPersist
{
public:
  nwxWindowSize(const wxString &sName) :
      m_sName(sName),
      m_size(wxDefaultSize),
      m_pos(wxDefaultPosition)
  {
    RegisterAll(true);
  }
  nwxWindowSize(
    const wxString &sName, 
    const wxSize &sz, 
    const wxPoint &pos = wxDefaultPosition) :
      m_sName(sName),
      m_size(sz),
      m_pos(pos)
  {
    RegisterAll(true);
  }
  nwxWindowSize() :
      m_size(wxDefaultSize),
      m_pos(wxDefaultPosition)
  {
    RegisterAll(true);
  }
  nwxWindowSize(const nwxWindowSize &x)
  {
    RegisterAll(true);
    CopyFrom(x);
  }
  void CopyFrom(const nwxWindowSize &x)
  {
    m_sName = x.GetName();
    m_size = x.GetSize();
    m_pos = x.GetPos();
  }
  nwxWindowSize & operator = (const nwxWindowSize &x)
  {
    CopyFrom(x);
    return *this;
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  const wxSize &GetSize() const
  {
    return m_size;
  }
  const wxPoint &GetPos() const
  {
    return m_pos;
  }
  bool SetPos(const wxPoint &x)
  {
    bool bRtn = (x != m_pos);
    if(bRtn) { m_pos = x; }
    return bRtn;
  }
  bool SetSize(const wxSize &x)
  {
    bool bRtn = (x != m_size);
    if(bRtn) { m_size = x; }
    return bRtn;
  }
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(wxT("name"),&m_sName);
    RegisterIntPositive(wxT("w"),&m_size.x);
    RegisterIntPositive(wxT("h"),&m_size.y);
    RegisterIntPositive(wxT("x"),&m_pos.x);
    RegisterIntPositive(wxT("y"),&m_pos.y);
  }
private:
  wxString m_sName;
  wxSize m_size;
  wxPoint m_pos;
};

class nwxSplitterPos : public nwxXmlPersist
{
public:
  nwxSplitterPos()
  {
    _Clear();
    RegisterAll(true);
  }
  nwxSplitterPos(const wxString &s)
  {
    _Clear();
    m_sName = s;
    RegisterAll(true);
  }
  nwxSplitterPos(const wxString &s, int n)
  {
    _Clear();
    m_sName = s;
    m_nPos = n;
    RegisterAll(true);
  }
  nwxSplitterPos(const nwxSplitterPos &x)
  {
    CopyFrom(x);
    RegisterAll(true);
  }
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(wxT("name"),&m_sName);
    RegisterInt("x",&m_nPos);
  }
  void CopyFrom(const nwxSplitterPos &x)
  {
    m_nPos = x.GetPosition();
    m_sName = x.GetName();
  }
  nwxSplitterPos &operator = (const nwxSplitterPos &x)
  {
    CopyFrom(x);
    return *this;
  }
  int GetPosition() const
  {
    return m_nPos;
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  bool SetPosition( int x)
  {
    bool bRtn = x != m_nPos;
    if(bRtn) { m_nPos = x; }
    return bRtn;
  }
  virtual void Init()
  {
    nwxXmlPersist::Init();
    _Clear();
  }
  virtual void Init(void *p)
  {
    if(p == (void *)this)
    {
      nwxXmlPersist::Init(p);
      _Clear();
    }
  }
private:
  void _Clear()
  {
    m_nPos = -1;
    m_sName.Clear();
  }
  wxString m_sName;
  int m_nPos;
};


class nwxXmlWindowSizesLess
{
public:
  bool operator()(const nwxSplitterPos &x1, const nwxSplitterPos &x2) const
  {
    const wxString &s1(x1.GetName());
    const wxString &s2(x2.GetName());
    int n  = s1.CmpNoCase(s2);
    if(!n) { n = s1.Cmp(s2); }
    return n < 0;
  }
  bool operator()(nwxSplitterPos *p1, nwxSplitterPos *p2) const
  {
    return (*this)(*p1,*p2);
  }
  bool operator()(const nwxSplitterPos *p1, const nwxSplitterPos *p2) const
  {
    return (*this)(*p1,*p2);
  }
  bool operator()(const nwxWindowSize &x1, const nwxWindowSize &x2) const
  {
    const wxString &s1(x1.GetName());
    const wxString &s2(x2.GetName());
    int n  = s1.CmpNoCase(s2);
    if(!n) { n = s1.Cmp(s2); }
    return n < 0;
  }
  bool operator()(nwxWindowSize *p1, nwxWindowSize *p2) const
  {
    return (*this)(*p1,*p2);
  }
  bool operator()(const nwxWindowSize *p1, const nwxWindowSize *p2) const
  {
    return (*this)(*p1,*p2);
  }
  static nwxXmlIOintPositive m_IOint;
};


class nwxXmlWindowSizes : public nwxXmlPersist, public nwxTimerReceiver
{
public:
  static nwxXmlWindowSizes *GetGlobal()
  {
    return g_pXWS;
  }
  static nwxXmlWindowSizes *SetupGlobal(const wxString &sPath)
  {
    if( (g_pXWS == NULL) || (g_pXWS->GetPath() != sPath) )
    {
      CleanupGlobal();
      g_pXWS = new nwxXmlWindowSizes(sPath);
    }
    return g_pXWS;
  }
  static void CleanupGlobal()
  {
    if(g_pXWS != NULL)
    {
      delete g_pXWS;
      g_pXWS = NULL;
    }
  }
  static const wxSize &GetWindowSizeGlobal(const wxString &s, bool bImplemented = true)
  {
    if((!bImplemented) || (g_pXWS == NULL))
    {
      return wxDefaultSize;
    }
    return g_pXWS->GetWindowSize(s);
  }
  static void SaveWindowSizeGlobal(const wxString &s, const wxSize &sz, bool bImplemented = true)
  {
    if(bImplemented && (g_pXWS != NULL))
    {
      g_pXWS->SaveWindowSize(s,sz);
    }
  }
  static void SaveWindowPosGlobal(const wxString &s, const wxPoint &pt, bool bImplemented = true)
  {
    if(bImplemented && (g_pXWS != NULL))
    {
      g_pXWS->SaveWindowPos(s,pt);
    }
  }
  static const wxPoint &GetWindowPosGlobal(const wxString &s, bool bImplemented = true)
  {
    if((!bImplemented) || (g_pXWS == NULL))
    {
      return wxDefaultPosition;
    }
    return g_pXWS->GetWindowPos(s);
  }

  nwxXmlWindowSizes(const wxString &sPath);
  virtual ~nwxXmlWindowSizes()
  {
    if(this == g_pXWS)
    {
      g_pXWS = NULL;
    }
    _CheckSaveFile(true);
    _Clear(false);
    m_setSize.Clear();
    m_setSplitter.Clear();
  }
  const wxString &GetPath() const
  {
    return m_sPath;
  }
  virtual const wxString &RootNode(void) const
  {
    return g_sROOT;
  }
  virtual void RegisterAll(bool = false)
  {
    Register("windows",&m_setSize);
    Register("splitters",&m_setSplitter);
  }
  virtual void OnTimer(wxTimerEvent &e);

  // size and position methods
  void SaveWindowSize(const wxString &sName, const wxSize &sz);
  void SaveWindowSize(wxWindow *pwin,const wxString &sName)
  {
    SaveWindowSize(sName,pwin->GetSize());
  }
  void SaveWindowSize(wxWindow *pwin)
  {
    SaveWindowSize(_GetWindowName(pwin),pwin->GetSize());
  }
  void SaveWindowPos(const wxString &sName, const wxPoint &sz);
  void SaveWindowPos(wxWindow *pwin,const wxString &sName)
  {
    SaveWindowPos(sName,pwin->GetPosition());
  }
  void SaveWindowPos(wxWindow *pwin)
  {
    SaveWindowPos(_GetWindowName(pwin),pwin->GetPosition());
  }

  void SaveWindowSizePos(wxWindow *pwin, const wxString &sName);
  void SaveWindowSizePos(wxWindow *pwin)
  {
    SaveWindowSizePos(pwin,_GetWindowName(pwin));
  }
  const wxSize &GetWindowSize(const wxString &sName);
  const wxSize &GetWindowSize(wxWindow *pwin)
  {
    return GetWindowSize(_GetWindowName(pwin));
  }
  const wxPoint &GetWindowPos(const wxString &sName);
  const wxPoint &GetWindowPos(wxWindow *pwin)
  {
    return GetWindowPos(_GetWindowName(pwin));
  }

  // splitter methods

  void SaveSplitterPos(const wxString &sName, int n);
  void SaveSplitterPos(wxSplitterWindow *pWin, const wxString &sName)
  {
    SaveSplitterPos(sName,pWin->GetSashPosition());
  }
  int GetSplitterPos(const wxString &sName);

private:
  static nwxXmlWindowSizes *g_pXWS;
  static const wxChar *_GetWindowName(wxWindow *pwin)
  {
#ifdef __WXDEBUG__
    wxClassInfo *p = pwin->GetClassInfo();
    const wxChar *pRtn = p->GetClassName();
    return pRtn;
#else
    return pwin->GetClassInfo()->GetClassName();
#endif
  }
  nwxWindowSize *_FindWindowSize(const wxString &sName, bool bCreate = true);
  nwxSplitterPos *_FindSplitterPos(const wxString &sName, bool bCreate = true);
  void _CheckLoadFile(bool bForce = false);
  void _CheckSaveFile(bool bForce = false);
  void _Clear(bool bConstructor = true)
  {
    m_setSize.Clear();
    m_setSplitter.Clear();
    if(bConstructor)
    {
      m_setSize.SetNodeName("window");
      m_setSplitter.SetNodeName("splitter");
    }
  }
  void _ResetInterval()
  {
    m_interval.ResetWaitTime();
    m_bCanCheckFile = false;
    m_bCanSaveFile = false;
  }
  TnwxXmlPersistSet<nwxWindowSize,nwxXmlWindowSizesLess> m_setSize;
  TnwxXmlPersistSet<nwxSplitterPos,nwxXmlWindowSizesLess> m_setSplitter;
  nwxTimerInterval m_interval;
  wxString m_sPath;
  bool m_bLoaded;
  bool m_bLocallyModified;
  bool m_bCanCheckFile; // make sure that _CheckLoadFile does not check the file too often
  bool m_bCanSaveFile;  // make sure that _CheckSaveFile does not check the file too often
  static const wxString g_sROOT;
};

#endif