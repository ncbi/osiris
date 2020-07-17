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
*  FileName: nwxTextCtrl.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_TEXT_CTRL__H__
#define __NWX_TEXT_CTRL__H__

#include <wx/textctrl.h>

class nwxTextCtrl : public wxTextCtrl
{
public:
  // this is a hack to work around a problem where
  // the beginning of the text is sometimes not visible and
  // is scrolled off to the left, even if the window is not too
  // small to contain all of the text
  nwxTextCtrl(
    wxWindow *parent,
    wxWindowID id,
    const wxString &value = wxEmptyString,
    const wxPoint &pos = wxDefaultPosition,
    const wxSize &size = wxDefaultSize,
    long style = 0,
    const wxValidator &validator = wxDefaultValidator,
    const wxString &name = wxTextCtrlNameStr) :
      wxTextCtrl(parent,id,value,pos,size,style,validator,name)
      {
        return;
      }
  virtual ~nwxTextCtrl() {}
private:
  void OnSetFocus(wxFocusEvent &e);

  DECLARE_ABSTRACT_CLASS(nwxTextCtrl);
  DECLARE_EVENT_TABLE()
};

class nwxTextCtrlInteger : public wxTextCtrl
{
public:
  nwxTextCtrlInteger(
    wxWindow *parent,
    wxWindowID id,
    int nMin,
    int nMax,
    int nValue = 0x7fffffff,
    int nDigits = 5,
    long style = 0L);
  virtual ~nwxTextCtrlInteger() {}
  void SetDigits(int n);
  int GetIntValue(int nEmptyValue = 0);
private:
  void OnSetFocus(wxFocusEvent &e);
  static wxSize _ComputeSize(int nDigits, wxWindow *parent);
  static int g_nWidth10;
  static wxWindow *g_pParent;
  DECLARE_ABSTRACT_CLASS(nwxTextCtrlInteger);
  DECLARE_EVENT_TABLE()
};


#endif
