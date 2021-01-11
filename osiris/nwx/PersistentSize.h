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
*  FileName: PersistentSize.h
*  Author:   Douglas Hoffman
*
*  Macros for saving and restoring the size of a window for future use
*
*  Usage: Add appropriate DECLARE_ macro in class definition
*         Add appropriate IMPLEMENT_ macro in class .cpp file
*         Add appropriate EVT_ macro in EVENT_TABLE in .cpp file
*         Use appropriate GET_PERSISTENT_ macros when creating windows, 
*                            usually passed to parent class constructor
*/
#ifndef __PERSISTENT_SIZE_H__
#define __PERSISTENT_SIZE_H__

#include <wx/event.h>
#include <wx/string.h>
#include "nwx/nwxXmlWindowSizes.h"

#define DECLARE_PERSISTENT_SIZE \
public: \
  void OnPersistResize(wxSizeEvent &e);

#define DECLARE_PERSISTENT_SIZE_OPTIONAL \
private: \
  bool m_bPERSIST_SIZE; \
  DECLARE_PERSISTENT_SIZE \
  void SetUsePersistSize(bool b = true) { m_bPERSIST_SIZE = b; } \
  bool GetUsePersistSize() { return m_bPERSIST_SIZE; }


#define DECLARE_PERSISTENT_POSITION \
public: \
void OnPersistMove(wxMoveEvent &);

#define DECLARE_PERSISTENT_POSITION_OPTIONAL \
private: \
  bool m_bPERSIST_MOVE; \
  DECLARE_PERSISTENT_POSITION \
  void SetUsePersistPosition(bool b = true) { m_bPERSIST_MOVE = b; } \
  bool GetUsePersistPosition() { return m_bPERSIST_MOVE; }

#define DECLARE_PERSISTENT_SIZE_POSITION \
  DECLARE_PERSISTENT_SIZE \
  DECLARE_PERSISTENT_POSITION


#define DECLARE_PERSISTENT_SIZE_POSITION_OPTIONAL \
  DECLARE_PERSISTENT_SIZE_OPTIONAL \
  DECLARE_PERSISTENT_POSITION_OPTIONAL



#define INIT_PERSISTENT_SIZE(b) SetUsePersistSize(b);
#define INIT_PERSISTENT_POSITION(b) SetUsePersistPosition(b);
#define INIT_PERSISTENT_SIZE_POSITION(b) \
  INIT_PERSISTENT_POSITION(b) \
  INIT_PERSISTENT_SIZE(b)

#define FUNC_ON_RESIZE_NAME(className,check, sName) \
void className::OnPersistResize(wxSizeEvent &e) \
{ \
  if((e.GetEventObject() == this) && !IsMaximized() && !IsIconized()) \
  { nwxXmlWindowSizes::SaveWindowSizeGlobal( \
      sName, GetSize(), check ); \
  } \
  e.Skip(); \
}



#define FUNC_ON_RESIZE(className,check) \
  FUNC_ON_RESIZE_NAME( className,check,wxString( #className) )


#define IMPLEMENT_PERSISTENT_SIZE(className) \
  FUNC_ON_RESIZE(className, true)

#define IMPLEMENT_PERSISTENT_SIZE_NAME(className,sName) \
  FUNC_ON_RESIZE_NAME(className, true, sName)

#define IMPLEMENT_PERSISTENT_SIZE_OPTIONAL(className) \
  FUNC_ON_RESIZE(className, GetUsePersistSize() )

#define FUNC_ON_POSITION_NAME(className, check, sName) \
void className::OnPersistMove(wxMoveEvent &e) \
{ \
  if(e.GetEventObject() == this && !IsMaximized() && !IsIconized()) \
  { nwxXmlWindowSizes::SaveWindowPosGlobal( \
      sName, GetPosition(), check); \
  } \
  e.Skip(); \
}

#define FUNC_ON_POSITION(className,check) \
  FUNC_ON_POSITION_NAME(className, check, wxString( #className ))

#define IMPLEMENT_PERSISTENT_POSITION(className) \
   FUNC_ON_POSITION(className,true)

#define IMPLEMENT_PERSISTENT_POSITION_NAME(className,sName) \
   FUNC_ON_POSITION_NAME(className,true,sName)

#define IMPLEMENT_PERSISTENT_POSITION_OPTIONAL(className) \
   FUNC_ON_POSITION(className,GetUsePersistPosition())

#define IMPLEMENT_PERSISTENT_SIZE_POSITION(className) \
  IMPLEMENT_PERSISTENT_SIZE(className) \
  IMPLEMENT_PERSISTENT_POSITION(className)

#define IMPLEMENT_PERSISTENT_SIZE_POSITION_NAME(className,sName) \
  IMPLEMENT_PERSISTENT_SIZE_NAME(className,sName) \
  IMPLEMENT_PERSISTENT_POSITION_NAME(className,sName)


#define IMPLEMENT_PERSISTENT_SIZE_POSITION_OPTIONAL(className) \
  IMPLEMENT_PERSISTENT_SIZE_OPTIONAL(className) \
  IMPLEMENT_PERSISTENT_POSITION_OPTIONAL(className)

// call 'PersistentSize' event functions from user defined event functions
#define CALL_PERSIST_RESIZE(e) OnPersistResize(e)
#define CALL_PERSIST_MOVE(e) OnPersistMove(e)



#define GET_PERSISTENT_SIZE(className) nwxXmlWindowSizes::GetWindowSizeGlobal( wxString( #className ) )
#define GET_PERSISTENT_SIZE_DEFAULT(className, szDefault) nwxXmlWindowSizes::GetWindowSizeGlobal( wxString( #className ), szDefault)
#define GET_PERSISTENT_POSITION(className) nwxXmlWindowSizes::GetWindowPosGlobal( wxString( #className ) )
#define GET_PERSISTENT_POSITION_DEFAULT(className, ptDefault) nwxXmlWindowSizes::GetWindowPosGlobal( wxString( #className ), true, ptDefault )

#define GET_PERSISTENT_POSITION_SIZE(className) \
  GET_PERSISTENT_POSITION(className), \
  GET_PERSISTENT_SIZE(className)
//  previous is in the same order as a wx constructor


#define GET_PERSISTENT_SIZE_OPTIONAL(className, Implemented) \
  nwxXmlWindowSizes::GetWindowSizeGlobal( wxString( #className ), Implemented )
#define GET_PERSISTENT_POSITION_OPTIONAL(className, Implemented) \
  nwxXmlWindowSizes::GetWindowPosGlobal( wxString( #className ), Implemented )


#define EVT_PERSISTENT_SIZE(className) \
EVT_SIZE(className::OnPersistResize)

#define EVT_PERSISTENT_POSITION(className) \
EVT_MOVE(className::OnPersistMove)

#define EVT_PERSISTENT_SIZE_POSITION(className) \
  EVT_PERSISTENT_POSITION(className) \
  EVT_PERSISTENT_SIZE(className)


#endif
