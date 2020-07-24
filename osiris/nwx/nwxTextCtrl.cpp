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
*  FileName: nwxTextCtrl.cpp
*  Author:   Douglas Hoffman
*
*/
#include <wx/valnum.h>
#include "nwx/nwxTextCtrl.h"
#include "nwx/nwxString.h"

IMPLEMENT_ABSTRACT_CLASS(nwxTextCtrl,wxTextCtrl)
IMPLEMENT_ABSTRACT_CLASS(nwxTextCtrlInteger, wxTextCtrl)
BEGIN_EVENT_TABLE(nwxTextCtrl,wxTextCtrl)
EVT_SET_FOCUS(nwxTextCtrl::OnSetFocus)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(nwxTextCtrlInteger, wxTextCtrl)
EVT_SET_FOCUS(nwxTextCtrlInteger::OnSetFocus)
END_EVENT_TABLE()


void nwxTextCtrl::OnSetFocus(wxFocusEvent &e)
{
  long nFrom;
  long nTo;
  GetSelection(&nFrom,&nTo);
  // force beginning of text to be visible
  SetInsertionPoint(0);
  e.Skip();
}

class nwxIntegerValidator : public wxIntegerValidator<int>
{
public:
  nwxIntegerValidator(int nMin, int nMax):
    wxIntegerValidator<int>(NULL, 0)
  {
    SetRange(nMin, nMax);
  }
  virtual ~nwxIntegerValidator() {}
};

int nwxTextCtrlInteger::g_nWidth10 = -1;
wxWindow *nwxTextCtrlInteger::g_pParent = NULL;

wxSize nwxTextCtrlInteger::_ComputeSize(int nDigits, wxWindow *parent)
{
  if (g_nWidth10 < 0 || parent != g_pParent)
  {
    g_pParent = parent;
    wxSize sz = g_pParent->GetTextExtent(wxT("9999999999"));
    g_nWidth10 = sz.GetWidth();
  }
  int nW = (nDigits > 0) ? (((nDigits + 2) * g_nWidth10) + 9) / 10 : -1;
  return wxSize(nW, -1);
}

nwxTextCtrlInteger::nwxTextCtrlInteger(
  wxWindow *parent,
  wxWindowID id,
  int nMin,
  int nMax,
  int nValue,
  int nDigits,
  long style) :
  wxTextCtrl(parent, id,
    wxEmptyString, wxDefaultPosition, _ComputeSize(nDigits, parent),
    style, nwxIntegerValidator(nMin, nMax)),
  m_nMin(nMin),
  m_nMax(nMax)
{
  SetIntValue(nValue);
  SetDigits(nDigits);
}
void nwxTextCtrlInteger::SetIntValue(int nValue)
{
  wxString s;
  if (nValue >= m_nMin && nValue <= m_nMax)
  {
    s = nwxString::FormatNumber(nValue);
  }
  SetValue(s);
}
void nwxTextCtrlInteger::SetDigits(int n)
{
  if (n > 0)
  {
    SetSize(_ComputeSize(n, GetParent()));
  }
}

int nwxTextCtrlInteger::GetIntValue(int nEmptyValue)
{
  int nRtn = nEmptyValue;
  wxString s = GetValue();
  if (!s.IsEmpty())
  {
    nRtn = atoi(s.utf8_str());
  }
  return nRtn;
}
void nwxTextCtrlInteger::OnSetFocus(wxFocusEvent &e)
{
  SelectAll();
  e.Skip();
}
