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
*  FileName: CPanelLabSettingsUtils.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __CPANEL_LAB_SETTINGS_UTILS_H__
#define __CPANEL_LAB_SETTINGS_UTILS_H__
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/choicebk.h>
#include "wxIDS.h"
#include "CLabSettings.h"

//********************************************************************
//
//    INameValidator
//
class INameValidator
{
public:
  virtual ~INameValidator(){;}
  virtual bool IsNameUsed(const wxString &s) = 0;
};
//********************************************************************
//
//    ISetReadOnly
//
class ISetReadOnly
{
public:
  virtual void SetReadOnly(bool b = true) = 0;
protected:
  ISetReadOnly() {;}
  virtual ~ISetReadOnly() {;}
};
//********************************************************************
//
//    nwxPanel
//
class nwxPanel : public wxPanel, public ISetReadOnly
{
public:
  nwxPanel(wxWindow *parent, wxWindowID id = wxID_ANY) :
      wxPanel(parent,id) {;}
  DECLARE_CLASS(nwxPanel)
};




//********************************************************************
//
//    nwxChoiceButtons
//
class nwxChoiceButtons
{
private:
  nwxChoiceButtons() {} // private - prevent instantiation
  static wxButton *_CreateButton(
    wxWindow *parent,
    wxWindowID nID,
    const wxString &sItemName,
    const wxChar *psLabel, 
    const wxChar *psPrefix,
    bool bReadOnly,
    wxSizer *pSizer
    );
public:
  static const wxChar * const LABEL_ADD;
  static const wxChar * const LABEL_REMOVE;
  static const wxChar * const LABEL_RENAME;

  static wxButton *CreateAddButton(
    wxWindow *parent,
    const wxString &sItemName,
    bool bReadOnly = false,
    wxSizer *pSizer = NULL)
  {
    return _CreateButton(
			parent,(wxWindowID)IDadd,
      sItemName,LABEL_ADD,wxS("Add a new "),
      bReadOnly,pSizer);
  }
  static wxButton *CreateRemoveButton(
    wxWindow *parent,
    const wxString &sItemName,
    bool bReadOnly = false,
    wxSizer *pSizer = NULL)
  {
    return _CreateButton(
      parent,wxWindowID(IDremove),
      sItemName,LABEL_REMOVE,wxS("Remove selected "),
      bReadOnly,pSizer);
  }
  static wxButton *CreateRenameButton(
    wxWindow *parent, 
    const wxString &sItemName, 
    bool bReadOnly = false,
    wxSizer *pSizer = NULL)
  {
    return _CreateButton(
			parent,wxWindowID(IDrename),
      sItemName,LABEL_RENAME,wxS("Rename selected "),
      bReadOnly,pSizer);
  }
};


//********************************************************************
//
//    nwxChoicebook
//
class nwxChoicebook : public wxChoicebook, public ISetReadOnly, public INameValidator
{
public:
  nwxChoicebook(wxWindow *parent) : wxChoicebook(
    parent,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    wxCHB_TOP)
  {
    m_pLabelCaption = NULL;
    m_pButtonAdd = NULL;
    m_pButtonRemove = NULL;
    m_pButtonRename = NULL;
    m_bReadOnly = false;
  }
  void SetCaption(const wxString &sCaption);
  void SetupButtons(const wxString &sItemName = wxEmptyString, bool bRename = false);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true);
  bool IsReadOnly()
  {
    return m_bReadOnly;
  }
  bool InsertPageSorted(nwxPanel *page,const wxString &text, bool select = false, int imageId = -1)
  {
    size_t n = FindInsertPosition(text);
    page->SetReadOnly(m_bReadOnly);
    bool bRtn = InsertPage(n,page,text,select,imageId);
    return bRtn;
  }
  virtual bool IsNameUsed(const wxString &sName);
  bool RenamePage(const wxString &sNewName, int nSelection = -1);



private:
  wxButton *CreateAddButton(
    const wxString &sItemName,
    wxSizer *pSizer)
  {
    return nwxChoiceButtons::CreateAddButton(this,sItemName,m_bReadOnly,pSizer);
  }
  wxButton *CreateRemoveButton(
    const wxString &sItemName,
    wxSizer *pSizer)
  {
    return nwxChoiceButtons::CreateRemoveButton(this,sItemName,m_bReadOnly,pSizer);
  }
  wxButton *CreateRenameButton(
    const wxString &sItemName,
    wxSizer *pSizer)
  {
    return nwxChoiceButtons::CreateRenameButton(this,sItemName,m_bReadOnly,pSizer);
  }
  size_t FindInsertPosition(const wxString &s);
  wxButton *m_pButtonAdd;
  wxButton *m_pButtonRemove;
  wxButton *m_pButtonRename;
  wxStaticText *m_pLabelCaption;
  bool m_bReadOnly;
};


#endif
