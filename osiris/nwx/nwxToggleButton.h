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
*  FileName: nwxToggleButton
*  Author:   Douglas Hoffman
*
*  When wxToggleButton (windows) is set and disabled, it looks enabled
*   unset it if disabled but return the 'set' value when calling GetValue()
*
*/
#ifndef __NWX_TOGGLE_BUTTON_H__
#define __NWX_TOGGLE_BUTTON_H__

#include <wx/tglbtn.h>

class nwxToggleButton : public wxToggleButton
{
public:
  nwxToggleButton() : wxToggleButton(), m_bValue(false)
  {}
 	nwxToggleButton (
    wxWindow *parent, 
    wxWindowID id, 
    const wxString &label, 
    const wxPoint &pos=wxDefaultPosition, 
    const wxSize &size=wxDefaultSize, 
    long style=0, 
    const wxValidator &val=wxDefaultValidator, 
    const wxString &name=wxCheckBoxNameStr):
   	  wxToggleButton (parent, id, label, pos, size, style, val, name),
      m_bValue(false)
  {}
  virtual ~nwxToggleButton() {}
  virtual bool GetValue() const
  {
    if(IsEnabled())
    {
      m_bValue = wxToggleButton::GetValue();
    }
    return m_bValue;
  }
  virtual void SetValue(bool state)
  {
    m_bValue = state;
    if(IsEnabled())
    {
      wxToggleButton::SetValue(state);
    }
  }
  virtual bool Enable(bool enable = true)
  {
    bool bRtn = IsEnabled();
    if(enable != bRtn)
    {
      if(!enable)
      {
        // currently enabled, going to disable
        GetValue(); // sets m_bValue
        wxToggleButton::SetValue(false);
      }
      wxToggleButton::Enable(enable);
    }
    return bRtn;
  }
  bool Disable()
  {
    return Enable(false);
  }
private:
  mutable bool m_bValue;
};

#endif