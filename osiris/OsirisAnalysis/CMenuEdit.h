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
*  FileName: CMenuEdit.h
*    edit popup menu for class CFrameAnalysis
*    with static utility functions for the "Table"
*    pulldown menu on the menu bar
*
*    Also contains CMenuReview and CMenuAccept
*
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_MENU_EDIT_H__
#define __C_MENU_EDIT_H__

#include <wx/menu.h>
#include <wx/string.h>



//**************************************************** CMenuReviewAccept
class CMenuReviewAccept : public wxMenu
{
public:
  typedef enum
  {
    // array subscripts
    ITEM_LOCUS = 0,
    ITEM_SAMPLE,
    ITEM_ILS,
    ITEM_CHANNELS,
    ITEM_DIRECTORY,
    ITEM_COUNT
  } ITEM_TYPE;

  CMenuReviewAccept(bool bAccept);
  static void SetLocusName(wxMenuItem *pItem, const wxString &sLocus, const wxString &sAccel = wxEmptyString);
  void SetLocusName(const wxString &sLocus)
  {
    SetLocusName(m_pItemLocus,sLocus,m_sAccel);
  }
  void SetEditNotices()
  {
    SetLocusName(m_pItemLocus,wxEmptyString);
  }
  void EnableType(CMenuReviewAccept::ITEM_TYPE n, bool b = true)
  {
    Enable(m_pnIDS[n],b);
  }
  void EnableLocus(bool b = true)
  {
    EnableType(ITEM_LOCUS,b);
  }
  void EnableSample(bool b = true)
  {
    EnableType(ITEM_SAMPLE,b);
  }
  void EnableILS(bool b = true)
  {
    EnableType(ITEM_ILS,b);
  }
  void EnableChannels(bool b = true)
  {
    EnableType(ITEM_CHANNELS,b);
  }
  void EnableDirectory(bool b = true)
  {
    EnableType(ITEM_DIRECTORY,b);
  }
  bool AllDisabled();
  bool NotAllDisabled()
  {
    return !AllDisabled();
  }
  void CopyState(CMenuReviewAccept *pTo);
  static int ContainMenu(wxMenu *p, wxMenu *pFind);
private:
  wxString m_sAccel;
  wxMenuItem *m_pItemLocus;
  const int * const m_pnIDS;
  static const int g_anIDReview[ITEM_COUNT];
  static const int g_anIDAccept[ITEM_COUNT];
  static const wxChar *g_asLabels[ITEM_COUNT];
};

class CMenuReview : public CMenuReviewAccept
{
public:
  CMenuReview() : CMenuReviewAccept(false) {}
};
class CMenuAccept : public CMenuReviewAccept
{
public:
  CMenuAccept() : CMenuReviewAccept(true) {}
};
//**************************************************** IMenuEdit

class CBaseMenuEdit : public wxMenu
{
public:
  CBaseMenuEdit()  {}

  virtual void EnableAccept(bool bLocus, bool bSample, bool bILS, bool bChannels, bool bDir);
  virtual void EnableReview(bool bLocus, bool bSample, bool bILS, bool bChannels, bool bDir);
  virtual void SetSampleEnabled(bool bEnable,bool bAllowDisable = true);
  virtual void SetLocus(const wxString &s, bool bEnabled = true);
  virtual void EnableEditLocus(bool bEnable);
  virtual void EnableEditDirectory(bool bEnable);

  void CopyStateEdit(CBaseMenuEdit *pTo);
  static void EnableByType(
    wxMenuItem *pItem, 
    CMenuReviewAccept *pMenu, 
    bool bLocus, 
    bool bSample, 
    bool bILS, 
    bool bChannels,
    bool bDir);

    // copy state of a single menu item
  static void CopyState(wxMenuItem *pmTo, wxMenuItem *pmFrom);
    // find a menu item in each menu and copy state
  static void CopyState(wxMenu *pmTo, wxMenu *pmFrom, int nID);

protected:
  CMenuReview *GetMenuReview()
  {
    return m_pMenuReview;
  }
  CMenuAccept *GetMenuAccept()
  {
    return m_pMenuAccept;
  }
  wxMenuItem *GetMenuItemReview()
  {
    return m_pMenuItemReview;
  }
  wxMenuItem *GetMenuItemAccept()
  {
    return m_pMenuItemAccept;
  }
  virtual ~CBaseMenuEdit() {}
  wxMenuItem *m_pMenuItemAccept;
  wxMenuItem *m_pMenuItemReview;
  CMenuReview *m_pMenuReview;
  CMenuAccept *m_pMenuAccept;

};

//**************************************************** CMenuEdit

class CMenuEdit : public CBaseMenuEdit
{
public:
  static const wxString LABEL_ACCEPT_ALERTS;
  static const wxString LABEL_REVIEW_EDIT;
  static const wxString LABEL_EDIT_DIRECTORY;

  CMenuEdit();
  virtual ~CMenuEdit();
};

#endif
