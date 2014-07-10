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

*  FileName: nwxTypeValidator.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_TYPE_VALIDATOR_H__
#define __NWX_TYPE_VALIDATOR_H__

#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/msgdlg.h>
#include "nwx/nwxBookCtrlFocus.h"
template <class T, class W = wxTextCtrl> class tnwxTypeValidator : public wxTextValidator
{
public:
  tnwxTypeValidator<T,W> (T *p, T tlo, T thi, T tdefault, bool bAllowEmpty = true, bool bIsNumber = false) :
    wxTextValidator(),
    m_sPrefix("Value"),
    m_tLo(tlo),
    m_tHi(thi),
    m_tDefault(tdefault),
    m_pT(p),
    m_bAllowEmpty(bAllowEmpty),
    m_bIsNumber(bIsNumber)
  { ; }

  tnwxTypeValidator<T,W>(const tnwxTypeValidator<T,W> &x) 
    : wxTextValidator(x)
  {
    _Copy(x);
  }

  // PURE VIRTUAL FUNCTIONS

  virtual wxString ToString(T x) = 0;
  virtual T FromString(const wxString &s) = 0;
  virtual bool ValidateString(const wxString &s) = 0;


  virtual bool TransferToWindow()
  {
    bool bRtn = false;
    if( (m_pT != NULL) && InRange(*m_pT) )
    {
      bRtn = true;
      wxString s = ToString(*m_pT);
      W *pw = (W *) GetWindow();
      pw->SetValue(s);
    }
    return bRtn;
  }

  virtual bool TransferFromWindow()
  {
    wxString sError;
    T tValue;
    bool bRtn = SilentValidate(&sError,&tValue);
    if(bRtn && (m_pT != NULL))
    {
      *m_pT = tValue;
    }
    return bRtn;
  }
  virtual bool Validate(wxWindow *parent)
  {
    wxString sError;
    T x;
    bool bRtn = wxTextValidator::Validate(parent) && SilentValidate(&sError,&x);
    if(!bRtn)
    {
      if(sError.Len())
      {
        wxMessageDialog mb(parent,sError,"Error", wxOK | wxICON_ERROR );
        mb.ShowModal();
      }
      nwxBookCtrlFocus::Focus(GetWindow());
      //GetWindow()->SetFocus();
    }
    return bRtn;
  }

  void SetRange(T tLo, T tHi)
  {
    m_tLo = tLo;
    m_tHi = tHi;
    if(m_tLo > m_tHi)
    {
      T x = m_tLo;
      m_tLo = m_tHi;
      m_tHi = x;
    }
  }
  void SetDefaultValue(T x)
  {
    m_tDefault = x;
  }
  T GetDefaultValue()
  {
    return m_tDefault;
  }
  void SetIsNumber(bool b)
  {
    m_bIsNumber = b;
  }
  void GetIsNumber()
  {
    return m_bIsNumber;
  }
  void SetAllowEmpty(bool b)
  {
    m_bAllowEmpty = b;
  }
  bool GetAllowEmpty()
  {
    return m_bAllowEmpty;
  }
  void SetErrorPrefix(const wxString &s)
  {
    m_sPrefix = s;
  }
  const wxString &GetErrorPrefix()
  {
    return m_sPrefix;
  }

  virtual wxString GetErrorEmpty()
  {
    wxString sRtn(m_sPrefix);
    sRtn += " is empty.  A value is required.";
    return sRtn;
  }
  virtual wxString GetErrorInvalidNumber()
  {
    wxTextCtrl *pw = (wxTextCtrl *) GetWindow();
    wxString sErr = pw->GetValue();
    sErr.Append(" is not a valid number.");
    return sErr;
  }
  virtual wxString GetErrorInvalid()
  {
    wxString sRtn;
    if(m_bIsNumber)
    {
      sRtn = GetErrorInvalidNumber();
    }
    else
    {
      sRtn = m_sPrefix;
      sRtn += " is not valid.";
    }
    return sRtn;
  }
  virtual wxString GetErrorRange()
  {
    wxString sRtn(m_sPrefix);
    sRtn += " is out of range.\n";
    sRtn += ToString(m_tLo);
    sRtn += " - ";
    sRtn += ToString(m_tHi);
    return sRtn;
  }

  virtual void GetValue(wxString *ps)
  {
    W *pw = (W *) GetWindow();
    (*ps) = pw->GetValue();
  }

protected:

  bool InRange(T x)
  {
    return !((x > m_tHi) || (x < m_tLo));
  }
  virtual bool SilentValidate(wxString *psError, T *pT)
  {
    wxString s;
    T tValue = m_tDefault;
    bool bRtn = true;

    GetValue(&s);
    s.Trim(true);
    s.Trim(false);
    if(s.IsEmpty())
    {
      if(!m_bAllowEmpty)
      {
        *psError = GetErrorEmpty();
        bRtn = false;
      }
    }
    else if (!ValidateString(s))
    {
      bRtn = false;
      *psError = GetErrorInvalid();
    }
    else 
    {
      tValue = FromString(s);
      if(!InRange(tValue))
      {
        *psError = GetErrorRange();
        bRtn = false;
      }
    }
    if(bRtn && (pT != NULL))
    {
      *pT = tValue;
    }
    return bRtn;
  }
  void _Copy(const tnwxTypeValidator<T,W> &x)
  {
#define CP(z) z = x.z
    CP(m_sPrefix);
    CP(m_tLo);
    CP(m_tHi);
    CP(m_tDefault);
    CP(m_pT);
    CP(m_bAllowEmpty);
#undef CP
  }


  wxString m_sPrefix;
  T m_tLo;
  T m_tHi;
  T m_tDefault;
  T *m_pT;
  bool m_bAllowEmpty;
  bool m_bIsNumber;
};


#endif
