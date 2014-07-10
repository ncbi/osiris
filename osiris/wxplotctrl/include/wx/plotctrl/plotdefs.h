/////////////////////////////////////////////////////////////////////////////
// Name:        plotdefs.h
// Purpose:     Definitions for wxPlotLib
// Author:      John Labenski
// Modified by:
// Created:     1/08/2005
// RCS-ID:      $Id: plotdefs.h,v 1.3 2009/06/08 20:53:44 hoffman Exp $
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_PLOTDEF_H__
#define __WX_PLOTDEF_H__

#include "wx/defs.h"
#include "wx/geometry.h"

// ----------------------------------------------------------------------------
// DLLIMPEXP macros
// ----------------------------------------------------------------------------

// These are our DLL macros (see the contrib libs like wxPlot)
#ifdef WXMAKINGDLL_PLOTCTRL
    #define WXDLLIMPEXP_PLOTCTRL WXEXPORT
    #define WXDLLIMPEXP_DATA_PLOTCTRL(type) WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_PLOTCTRL WXIMPORT
    #define WXDLLIMPEXP_DATA_PLOTCTRL(type) WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_PLOTCTRL
    #define WXDLLIMPEXP_DATA_PLOTCTRL(type) type
#endif

// ----------------------------------------------------------------------------
// Convenience macros
// ----------------------------------------------------------------------------

// Check if value is >= min_val and <= max_val
#define wxPCHECK_MINMAX_RET(val, min_val, max_val, msg) \
    wxCHECK_RET((int(val)>=int(min_val))&&(int(val)<=int(max_val)), msg)

#define wxPCHECK_MINMAX_MSG(val, min_val, max_val, ret, msg) \
    wxCHECK_MSG((int(val)>=int(min_val))&&(int(val)<=int(max_val)), ret, msg)

#define RINT(x) (int((x) >= 0 ? ((x) + 0.5) : ((x) - 0.5)))

#define LONG_TO_WXCOLOUR(c) wxColour((unsigned char)((c>>16)&0xFF), (unsigned char)((c>>8 )&0xFF), (unsigned char)((c)&0xFF))
#define WXCOLOUR_TO_LONG(c) ((c.Red()<<16)|(c.Green()<<8)|(c.Blue()))

inline void PRINT_WXRECT(const wxString& str, const wxRect& r)                 { wxPrintf(wxT("%s xy(%d %d) wh(%d %d) rb(%d %d)\n"), str.utf8_str(), r.x,   r.y,   r.width,   r.height,   r.GetRight(), r.GetBottom()); }
inline void PRINT_WXRECT2DDOUBLE(const wxString& str, const wxRect2DDouble& r) { wxPrintf(wxT("%s xy(%g %g) wh(%g %g) rb(%g %g)\n"), str.utf8_str(), r.m_x, r.m_y, r.m_width, r.m_height, r.GetRight(), r.GetBottom()); }

// ----------------------------------------------------------------------------
// Speed up drawing routines in wxDC
// ----------------------------------------------------------------------------

// Skip the wxWidgets drawing routines since they calc an unnecessary bounding rect
// You may turn this off by defining wxPLOTCTRL_FAST_GRAPHICS=0 to the compilier
#ifndef wxPLOTCTRL_FAST_GRAPHICS
    #define wxPLOTCTRL_FAST_GRAPHICS 1
#endif // wxPLOTCTRL_FAST_GRAPHICS

// ----------------------------------------------------------------------------
// wxWidgets backwards compatibility macros
// ----------------------------------------------------------------------------

#include "wx/dynarray.h"
#ifndef WX_DECLARE_OBJARRAY_WITH_DECL // for wx2.4 backwards compatibility
    #define WX_DECLARE_OBJARRAY_WITH_DECL(T, name, expmode) WX_DECLARE_USER_EXPORTED_OBJARRAY(T, name, WXDLLIMPEXP_PLOTCTRL)
#endif

#if !wxCHECK_VERSION(2,5,0)
    bool WXRECT2DDOUBLE_EQUAL(const wxRect2DDouble& r1, const wxRect2DDouble& r2)
    {
        return (r1.m_x == r1.m_x) && (r1.m_y == r1.m_y) &&
               (r1.m_width == r1.m_width) && (r1.m_height == r1.m_height);
    }
#else
    #define WXRECT2DDOUBLE_EQUAL(r1, r2) ((r1) == (r2))
#endif // wxCHECK_VERSION(2,5,0)

#if !wxCHECK_VERSION(2,6,0)
    #define wxIMAGE_OPTION_CUR_HOTSPOT_X wxCUR_HOTSPOT_X
    #define wxIMAGE_OPTION_CUR_HOTSPOT_Y wxCUR_HOTSPOT_Y
#endif //wxCHECK_VERSION(2,7,0)


#endif  // __WX_PLOTDEF_H__
