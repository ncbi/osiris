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
*  FileName: Platform.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef __WXMAC__
#define MANUALLY_PLACE_FRAMES
#define COLOR_MENU_ITEMS 0
#define ID_SPLITTER_STYLE (wxSP_3DSASH | wxSP_LIVE_UPDATE)
#define REUSE_MENUS 0
// pixels to subtract from screen height for 'full screen'
#define MENU_BAR_ALLOWANCE 32
#define HAS_STATUS_BAR 0
#define CHECK_FRAME_ON_TIMER 0

//  if REUSE_MENUS == 0 then there is a problem if a single 
//  menu is used as a popup
//  and in a menu bar, either as a pulldown or submenu.
//  the problem has occurred on the mac but not windows.
//  this will be deprecated because it isn't worth the extra
//  code to support both, so it is set to 0
#else
#define COLOR_MENU_ITEMS 1
#define ID_SPLITTER_STYLE (wxSP_3DSASH | wxSP_NO_XP_THEME)
#define REUSE_MENUS 0
#define MENU_BAR_ALLOWANCE 0
#define HAS_STATUS_BAR 1
#define CHECK_FRAME_ON_TIMER 0
#endif

#ifdef __WXMSW__
#define HAS_CUSTOM_COLORS 1
#else
#define HAS_CUSTOM_COLORS 0
#endif



// char strings for X and checkmark in wingdings font

#ifdef __WXMSW__
#define CHAR_ATTN "\xfb"
#define CHAR_OK "\xfc"
#define USE_WINGDINGS 1
#else
#ifdef __WXMAC__
#define CHAR_ATTN wxChar(10008)
#define CHAR_OK wxChar(10004)
#define USE_WINGDINGS 0
#else
// X11
#define CHAR_ATTN "X"
#define CHAR_OK "OK"
#define USE_WINGDINGS 0
#endif
#endif

// key code for ^C (copy to clipboard)

#ifdef __WXMAC__
#define KEY_CODE_COPY 99
#else
#define KEY_CODE_COPY 3
#endif


// problem with layout for wxComboBox, wxListBox, etc.
#ifdef __WXMAC__NO_LONGER_NEEDED__
#define BOX_COMBO(pCombo,pSizer,nFlags) \
{\
  wxBoxSizer *pSizerCombo = new wxBoxSizer(wxVERTICAL); \
  pSizerCombo->Add(pCombo,0,wxTOP,4); \
  pSizer->Add(pSizerCombo,0,nFlags,ID_BORDER);\
}

#else

#define BOX_COMBO(pCombo,pSizer,nFlags) \
{ pSizer->Add(pCombo,0,nFlags,ID_BORDER);}

#endif


// file name delimiter

#ifdef __WXMSW__
#define FIX_FILE_NAME(s) s.Replace(wxS("/"),wxS("\\"),true);
#else
#define FIX_FILE_NAME(s)
#endif

#ifdef __WXDEBUG__
#define LOG_DEBUG 1
#endif

#if defined(__WXMAC__)

class wxFrame;
class wxEvtHandler;
typedef wxEvtHandler mainFrameSuper;
typedef wxFrame CMDIFrameSuper;
#define CMDIFramePARENT NULL
#define mainFrameIsWindow 0
#define DRAG_DROP_FILES 0
#define INCLUDE_FRAME <wx/frame.h>

#define __NO_MDI__

#elif defined(__WXMSW__)

class wxMDIParentFrame;
class wxMDIChildFrame;
typedef wxMDIChildFrame CMDIFrameSuper;
typedef wxMDIParentFrame mainFrameSuper;
#define CMDIFramePARENT parent
#define mainFrameIsWindow 1
#define DRAG_DROP_FILES 1
#define INCLUDE_FRAME <wx/mdi.h>

#else

//  maybe GTK someday
class wxFrame;
typedef wxFrame mainFrameSuper;
typedef wxFrame CMDIFrameSuper;
#define CMDIFramePARENT NULL
#define mainFrameIsWindow 1
#define DRAG_DROP_FILES 0
#define INCLUDE_FRAME <wx/frame.h>

#define __NO_MDI__

#endif



#endif
