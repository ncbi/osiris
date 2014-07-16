/////////////////////////////////////////////////////////////////////////////
// File:        wx.rc
// Purpose:     wxWindows resource definitions. ALWAYS include
//              this in your application resource file.
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id: OsirisAnalysis.inc,v 1.2 2009/04/27 14:13:17 hoffman Exp $
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__MWERKS__)
#include <winresrc.h>
#else
#include <windows.h>
#endif

#if defined(_WIN32_WCE)
    #include "wx/msw/wince/wince.rc"
#endif

#include "wx/msw/rcdefs.h"

//////////////////////////////////////////////////////////////////////////////
//
// This is the MDI Window menu
//

wxWindowMenu MENU DISCARDABLE
BEGIN
    POPUP "&Window"
    BEGIN
        MENUITEM "&Cascade",                    4002
        MENUITEM "Tile &Horizontally",          4001
        MENUITEM "Tile &Vertically",            4005
	MENUITEM "", -1
        MENUITEM "&Arrange Icons",              4003
        MENUITEM "&Next",                       4004
    END
END

//////////////////////////////////////////////////////////////////////////////
//
// Standard wxWindows Cursors
//

WXCURSOR_HAND           CURSOR  DISCARDABLE     "wx/msw/hand.cur"
WXCURSOR_BULLSEYE       CURSOR  DISCARDABLE     "wx/msw/bullseye.cur"
WXCURSOR_PENCIL         CURSOR  DISCARDABLE     "wx/msw/pencil.cur"
WXCURSOR_MAGNIFIER      CURSOR  DISCARDABLE     "wx/msw/magnif1.cur"
WXCURSOR_ROLLER         CURSOR  DISCARDABLE     "wx/msw/roller.cur"
WXCURSOR_PBRUSH         CURSOR  DISCARDABLE     "wx/msw/pbrush.cur"
WXCURSOR_PLEFT          CURSOR  DISCARDABLE     "wx/msw/pntleft.cur"
WXCURSOR_PRIGHT         CURSOR  DISCARDABLE     "wx/msw/pntright.cur"
WXCURSOR_BLANK          CURSOR  DISCARDABLE     "wx/msw/blank.cur"
//WXCURSOR_RIGHT_ARROW    CURSOR  DISCARDABLE     "wx/msw/rightarr.cur"
WXCURSOR_CROSS          CURSOR  DISCARDABLE     "wx/msw/cross.cur"


//////////////////////////////////////////////////////////////////////////////
//
// Default Icons
//

// First wx icon in alphabetical order, so it will be used by Explorer if the
// application doesn't have any icons of its own


// Standard small icons

//////////////////////////////////////////////////////////////////////////////
//
// Bitmaps
//

// For wxContextHelpButton
csquery                 BITMAP "wx/msw/csquery.bmp"

// For obtaining the RGB values of standard colours
wxBITMAP_STD_COLOURS    BITMAP "wx/msw/colours.bmp"

//////////////////////////////////////////////////////////////////////////////
//
// Manifest file for Windows XP
//

#if !defined(wxUSE_NO_MANIFEST) || (wxUSE_NO_MANIFEST == 0)
#if !defined(WX_MSC_FULL_VER) || WX_MSC_FULL_VER < 140040130

// see "about isolated applications" topic in MSDN
#ifdef ISOLATION_AWARE_ENABLED
#define wxMANIFEST_ID 2
#else
#define wxMANIFEST_ID 1
#endif

#if defined(WX_CPU_AMD64)
wxMANIFEST_ID 24 "wx/msw/amd64.manifest"
#elif defined(WX_CPU_IA64)
wxMANIFEST_ID 24 "wx/msw/ia64.manifest"
#elif defined(WX_CPU_X86)
wxMANIFEST_ID 24 "wx/msw/wx.manifest"
#endif

#endif // !defined(WX_MSC_FULL_VER) || WX_MSC_FULL_VER < 140040130
#endif // !defined(wxUSE_NO_MANIFEST) || (wxUSE_NO_MANIFEST == 0)
