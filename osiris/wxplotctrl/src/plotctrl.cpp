/////////////////////////////////////////////////////////////////////////////
// Name:        plotctrl.cpp
// Purpose:     wxPlotCtrl
// Author:      John Labenski, Robert Roebling
// Modified by:
// Created:     8/27/2002
// Copyright:   (c) John Labenski, Robert Roebling
// Licence:     wxWindows license
//      5/19/2010 - Doug Hoffman (DJH) - modified calls in
//         wxPlotCtrl::OnChar(e) to send an event 
//             when changing the axes from a keystroke
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "plotctrl.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/panel.h"
    #include "wx/scrolbar.h"
    #include "wx/event.h"
    #include "wx/timer.h"
    #include "wx/dcmemory.h"
    #include "wx/msgdlg.h"
    #include "wx/geometry.h"
    #include "wx/sizer.h"
    #include "wx/dcscreen.h"
    #include "wx/textctrl.h"
#endif // WX_PRECOMP

#include "wx/splitter.h"
#include "wx/math.h"
#include "wx/image.h"

#include "wx/plotctrl/plotctrl.h"
#include "wx/plotctrl/plotdraw.h"

#include <math.h>
#include <float.h>
#include <limits.h>

// MSVC hogs global namespace with these min/max macros - remove them
#ifdef max
    #undef max
#endif
#ifdef min
    #undef min
#endif
#ifdef GetYValue   // Visual Studio 7 defines this
    #undef GetYValue
#endif

//-----------------------------------------------------------------------------
// Consts
//-----------------------------------------------------------------------------

#define MAX_PLOT_ZOOMS 5
#define TIC_STEPS 3

std::numeric_limits<wxDouble> wxDouble_limits;
const wxDouble wxPlot_MIN_DBL   = wxDouble_limits.min()*10;
const wxDouble wxPlot_MAX_DBL   = wxDouble_limits.max()/10;
const wxDouble wxPlot_MAX_RANGE = wxDouble_limits.max()/5;

// Draw borders around the axes, title, and labels for sizing testing
//#define DRAW_BORDERS

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY( wxArrayPoint2DDouble );
WX_DEFINE_OBJARRAY( wxArrayRect2DDouble );
WX_DEFINE_OBJARRAY( wxArrayPlotCurve );

#include "../art/ledgrey.xpm"
#include "../art/ledgreen.xpm"

#include "../art/hand.xpm"
#include "../art/grab.xpm"

static wxCursor s_handCursor;
static wxCursor s_grabCursor;

// same as wxPlotRect2DDouble::Contains, but doesn't convert to wxPoint2DDouble
inline bool wxPlotRect2DDoubleContains(double x, double y, const wxRect2DDouble &rect)
{
    return ((x>=rect.m_x) && (y>=rect.m_y) && (x<=rect.GetRight()) && (y<=rect.GetBottom()));
}

//----------------------------------------------------------------------------
// Event types
//----------------------------------------------------------------------------

// wxPlotCtrlEvent
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_ADD_CURVE)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_DELETING_CURVE)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_DELETED_CURVE)

DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_CURVE_SEL_CHANGING)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_CURVE_SEL_CHANGED)

DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_MOUSE_MOTION)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_CLICKED)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_DOUBLECLICKED)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_POINT_CLICKED)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_POINT_DOUBLECLICKED)

DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_AREA_SEL_CREATING)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_AREA_SEL_CHANGING)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_AREA_SEL_CREATED)

DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_VIEW_CHANGING)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_VIEW_CHANGED)

DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_CURSOR_CHANGING)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_CURSOR_CHANGED)

DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_ERROR)

DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_BEGIN_TITLE_EDIT)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_END_TITLE_EDIT)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_BEGIN_X_LABEL_EDIT)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_END_X_LABEL_EDIT)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_BEGIN_Y_LABEL_EDIT)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_END_Y_LABEL_EDIT)

DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_MOUSE_FUNC_CHANGING)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_MOUSE_FUNC_CHANGED)

// wxPlotCtrlSelEvent
//DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_RANGE_SEL_CREATING)
//DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_RANGE_SEL_CREATED)
//DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_RANGE_SEL_CHANGING)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_RANGE_SEL_CHANGED)

/*
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_VALUE_SEL_CREATING)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_VALUE_SEL_CREATED)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_VALUE_SEL_CHANGING)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_VALUE_SEL_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_AREA_SEL_CHANGED)

DEFINE_EVENT_TYPE(wxEVT_PLOTCTRL_AREA_CREATE)
*/

// The code below translates the event.GetEventType to a string name for debugging
#define aDEFINE_LOCAL_EVENT_TYPE(t) if (eventType == t) return wxString(wxT(#t));

wxString wxPlotCtrl_GetEventName(wxEventType eventType)
{
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_ADD_CURVE)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_DELETING_CURVE)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_DELETED_CURVE)

    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_CURVE_SEL_CHANGING)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_CURVE_SEL_CHANGED)

    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_MOUSE_MOTION)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_CLICKED)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_DOUBLECLICKED)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_POINT_CLICKED)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_POINT_DOUBLECLICKED)

    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_AREA_SEL_CREATING)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_AREA_SEL_CHANGING)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_AREA_SEL_CREATED)

    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_VIEW_CHANGING)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_VIEW_CHANGED)

    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_CURSOR_CHANGING)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_CURSOR_CHANGED)

    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_ERROR)

    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_BEGIN_TITLE_EDIT)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_END_TITLE_EDIT)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_BEGIN_X_LABEL_EDIT)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_END_X_LABEL_EDIT)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_BEGIN_Y_LABEL_EDIT)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_END_Y_LABEL_EDIT)

    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_MOUSE_FUNC_CHANGING)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_MOUSE_FUNC_CHANGED)

    // wxPlotCtrlSelEvent
    //DEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_RANGE_SEL_CREATING)
    //DEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_RANGE_SEL_CREATED)
    //DEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_RANGE_SEL_CHANGING)
    aDEFINE_LOCAL_EVENT_TYPE(wxEVT_PLOTCTRL_RANGE_SEL_CHANGED)

    return wxT("Unknown Event Type");
}

//-----------------------------------------------------------------------------
// wxPlotCtrlEvent
//-----------------------------------------------------------------------------
IMPLEMENT_ABSTRACT_CLASS(wxPlotCtrlEvent, wxNotifyEvent)

wxPlotCtrlEvent::wxPlotCtrlEvent(wxEventType commandType, wxWindowID id, wxPlotCtrl *window)
                :wxNotifyEvent(commandType, id), m_curve(NULL), m_curve_index(-1),
                 m_curve_dataindex(-1), m_mouse_func(wxPLOTCTRL_MOUSE_NOTHING),
                 m_x(0.0), m_y(0.0),
                 m_width(0.0), m_height(0.0)
{
    SetEventObject( (wxObject*)window );
}

//-----------------------------------------------------------------------------
// wxPlotCtrlSelEvent
//-----------------------------------------------------------------------------
IMPLEMENT_ABSTRACT_CLASS(wxPlotCtrlSelEvent, wxPlotCtrlEvent)

wxPlotCtrlSelEvent::wxPlotCtrlSelEvent(wxEventType commandType, wxWindowID id,
                                           wxPlotCtrl *window)
                     :wxPlotCtrlEvent(commandType, id, window), m_selecting(false)
{
}

//-----------------------------------------------------------------------------
// wxPlotCtrlArea
//-----------------------------------------------------------------------------
IMPLEMENT_CLASS(wxPlotCtrlArea, wxWindow)

BEGIN_EVENT_TABLE(wxPlotCtrlArea, wxWindow)
    EVT_ERASE_BACKGROUND( wxPlotCtrlArea::OnEraseBackground )
    EVT_PAINT           ( wxPlotCtrlArea::OnPaint )
    EVT_MOUSE_EVENTS    ( wxPlotCtrlArea::OnMouse )
    EVT_CHAR            ( wxPlotCtrlArea::OnChar )
    EVT_KEY_DOWN        ( wxPlotCtrlArea::OnKeyDown )
    EVT_KEY_UP          ( wxPlotCtrlArea::OnKeyUp )
END_EVENT_TABLE()

bool wxPlotCtrlArea::Create( wxWindow *parent, wxWindowID win_id )
{
    if (!wxWindow::Create(parent, win_id, wxDefaultPosition, wxSize(100,100),
                  wxNO_BORDER|wxWANTS_CHARS|wxCLIP_CHILDREN, wxT("wxPlotCtrlArea")))
        return false;

    SetSizeHints(4, 4); // Don't allow window to get smaller than this!

    m_owner = wxDynamicCast(parent, wxPlotCtrl);
    return true;
}

void wxPlotCtrlArea::Init()
{
    m_owner = NULL;
}

void wxPlotCtrlArea::OnChar( wxKeyEvent &event )
{
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlArea"));
    m_owner->ProcessAreaEVT_CHAR(event);
}
void wxPlotCtrlArea::OnKeyDown( wxKeyEvent &event )
{
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlArea"));
    m_owner->ProcessAreaEVT_KEY_DOWN(event);
}
void wxPlotCtrlArea::OnKeyUp( wxKeyEvent &event )
{
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlArea"));
    m_owner->ProcessAreaEVT_KEY_UP(event);
}
void wxPlotCtrlArea::OnMouse( wxMouseEvent &event )
{
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlArea"));
    m_owner->ProcessAreaEVT_MOUSE_EVENTS(event);
}

void wxPlotCtrlArea::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc( this );
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlArea"));

    if ((m_owner->GetRedrawType() & wxPLOTCTRL_REDRAW_BLOCKER) != 0)
        return;

    int redraw_type = m_owner->GetRedrawType();

/*
    wxRegionIterator upd( GetUpdateRegion() );
    while (upd)
    {
        //printf("Region %d %d %d %d \n", upd.GetX(), upd.GetY(), upd.GetWidth(), upd.GetHeight() ); fflush(stdout);
        Paint( &dc, upd.GetRect() );
        upd++;
    }
*/

    if (redraw_type & wxPLOTCTRL_REDRAW_PLOT)
    {
        CreateBitmap( m_owner->GetPlotAreaRect() );
        m_owner->SetRedrawType(redraw_type & ~wxPLOTCTRL_REDRAW_PLOT);
    }

    if (m_bitmap.Ok())
        dc.DrawBitmap(m_bitmap, 0, 0, false);

    if (m_owner->GetCrossHairCursor() && m_owner->GetPlotAreaRect().Contains(m_mousePt))
        m_owner->DrawCrosshairCursor( &dc, m_mousePt );

    m_owner->DrawMouseMarker(&dc, m_owner->GetAreaMouseMarker(), m_mouseRect);
}

void wxPlotCtrlArea::CreateBitmap( const wxRect &rect )
{
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlArea"));

    wxRect refreshRect(rect);
    wxRect clientRect(m_owner->GetPlotAreaRect());
    refreshRect.Intersect(clientRect);

    if ((refreshRect.width == 0) || (refreshRect.height == 0)) return;

    // if the bitmap need to be recreated then refresh everything
    if (!m_bitmap.Ok() || (clientRect.width  != m_bitmap.GetWidth()) ||
                          (clientRect.height != m_bitmap.GetHeight()) )
    {
        m_bitmap.Create(clientRect.width, clientRect.height);
        refreshRect = clientRect;
    }

    wxMemoryDC mdc;
    mdc.SelectObject( m_bitmap );
    m_owner->DrawAreaWindow( &mdc, refreshRect );
    mdc.SelectObject( wxNullBitmap );
}

//-----------------------------------------------------------------------------
// wxPlotCtrlAxis
//-----------------------------------------------------------------------------
IMPLEMENT_CLASS(wxPlotCtrlAxis, wxWindow)

BEGIN_EVENT_TABLE(wxPlotCtrlAxis, wxWindow)
    EVT_ERASE_BACKGROUND( wxPlotCtrlAxis::OnEraseBackground )
    EVT_PAINT           ( wxPlotCtrlAxis::OnPaint )
    EVT_MOUSE_EVENTS    ( wxPlotCtrlAxis::OnMouse )
    EVT_CHAR            ( wxPlotCtrlAxis::OnChar )
END_EVENT_TABLE()

bool wxPlotCtrlAxis::Create( wxWindow *parent, wxWindowID win_id, wxPlotCtrlAxis_Type style )
{
    if (!wxWindow::Create(parent, win_id, wxDefaultPosition, wxDefaultSize,
                   wxNO_BORDER|wxWANTS_CHARS|wxCLIP_CHILDREN, wxT("wxPlotCtrlAxis")))
        return false;

    SetSizeHints(4, 4); // Don't allow window to get smaller than this!

    m_style = style;
    m_owner = wxDynamicCast(parent, wxPlotCtrl);

    if (style == wxPLOTCTRL_Y_AXIS)
        SetCursor( wxCursor(wxCURSOR_SIZENS) );
    else
        SetCursor( wxCursor(wxCURSOR_SIZEWE) );

    return true;
}

void wxPlotCtrlAxis::Init()
{
    m_style = wxPLOTCTRL_X_AXIS;
    m_owner = NULL;
}

void wxPlotCtrlAxis::OnChar( wxKeyEvent &event )
{
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlAxis"));
    m_owner->ProcessAxisEVT_CHAR(event);
}
void wxPlotCtrlAxis::OnMouse( wxMouseEvent &event )
{
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlAxis"));
    m_owner->ProcessAxisEVT_MOUSE_EVENTS(event);
}

void wxPlotCtrlAxis::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc( this );
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlAxis"));

    int redraw_type = m_owner->GetRedrawType();
    if ((redraw_type & wxPLOTCTRL_REDRAW_BLOCKER) != 0)
        return;

    if (IsXAxis() && ((redraw_type & wxPLOTCTRL_REDRAW_XAXIS) != 0))
    {
        m_owner->SetRedrawType(redraw_type & ~wxPLOTCTRL_REDRAW_XAXIS);
        CreateBitmap();
    }
    else if (!IsXAxis() && ((redraw_type & wxPLOTCTRL_REDRAW_YAXIS) != 0))
    {
        m_owner->SetRedrawType(redraw_type & ~wxPLOTCTRL_REDRAW_YAXIS);
        CreateBitmap();
    }

    if (m_bitmap.Ok())
        dc.DrawBitmap(m_bitmap, 0, 0, false);
}

void wxPlotCtrlAxis::CreateBitmap()
{
    wxCHECK_RET(m_owner, wxT("Invalid parent of wxPlotCtrlAxis"));

    m_owner->UpdateWindowSize();
    wxSize clientSize = GetClientSize();
    if ((clientSize.x < 2) || (clientSize.y < 2)) return;

    if (!m_bitmap.Ok() || (clientSize.x != m_bitmap.GetWidth()) ||
                          (clientSize.y != m_bitmap.GetHeight()) )
    {
        m_bitmap.Create(clientSize.x, clientSize.y);
    }

    wxMemoryDC mdc;
    mdc.SelectObject( m_bitmap );
    if (IsXAxis())
        m_owner->DrawXAxis( &mdc, true );
    else
        m_owner->DrawYAxis( &mdc, true );

    mdc.SelectObject( wxNullBitmap );
}

//-----------------------------------------------------------------------------
// wxPlotCtrl
//-----------------------------------------------------------------------------
IMPLEMENT_ABSTRACT_CLASS(wxPlotCtrl, wxWindow )

BEGIN_EVENT_TABLE(wxPlotCtrl, wxWindow )
    //EVT_ERASE_BACKGROUND ( wxPlotCtrl::OnEraseBackground ) // clear for MSW
    EVT_SIZE             ( wxPlotCtrl::OnSize )
    EVT_PAINT            ( wxPlotCtrl::OnPaint )
    EVT_CHAR             ( wxPlotCtrl::OnChar )
    EVT_SCROLL           ( wxPlotCtrl::OnScroll )
    EVT_IDLE             ( wxPlotCtrl::OnIdle )
    EVT_MOUSE_EVENTS     ( wxPlotCtrl::OnMouse )
    EVT_TIMER            ( wxID_ANY, wxPlotCtrl::OnTimer )

    EVT_TEXT_ENTER       ( wxID_ANY, wxPlotCtrl::OnTextEnter)
END_EVENT_TABLE()

void wxPlotCtrl::Init()
{
    m_activeCurve  = NULL;
    m_active_index = -1;

    m_cursorMarker.CreateEllipseMarker(wxPoint2DDouble(0,0),
                                       wxSize(2, 2),
                                       wxGenericPen(wxGenericColour(0, 255, 0)));
    m_cursor_curve  = -1;
    m_cursor_index  = -1;

    m_selection_type = wxPLOTCTRL_SELECT_MULTIPLE;

    m_show_key    = true;

    m_show_title = m_show_xlabel = m_show_ylabel = false;
    m_title  = wxT("Title");
    m_xLabel = wxT("X-Axis");
    m_yLabel = wxT("Y-Axis");

    m_titleFont    = *wxSWISS_FONT;
    m_titleColour  = *wxBLACK;
    m_borderColour = *wxBLACK;

    m_scroll_on_thumb_release = false;
    m_crosshair_cursor        = false;
    m_draw_symbols            = true;
    m_draw_lines              = true;
    m_draw_spline             = false;
    m_draw_grid               = true;
    m_fit_on_new_curve        = true;
    m_show_xAxis              = true;
    m_show_yAxis              = true;

    m_zoom = wxPoint2DDouble( 1.0, 1.0 );
    m_history_views_index = -1;

    m_fix_aspectratio = false;
    m_aspectratio     = 1.0;

    m_defaultPlotRect   = wxRect2DDouble( -10.0, -10.0, 20.0, 20.0 );
    m_viewRect          = m_defaultPlotRect;
    m_curveBoundingRect = m_defaultPlotRect;
    m_areaClientRect    = wxRect(0, 0, 10, 10);

    m_xAxisTickFormat = m_yAxisTickFormat = wxT("%lf");
    m_xAxisTick_step  = m_yAxisTick_step  = 1.0;
    m_xAxisTick_count = m_yAxisTick_count = 4;
    m_correct_ticks   = true;

    m_areaDrawer      = NULL;
    m_xAxisDrawer     = NULL;
    m_yAxisDrawer     = NULL;
    m_keyDrawer       = NULL;
    m_curveDrawer     = NULL;
    m_dataCurveDrawer = NULL;
    m_markerDrawer    = NULL;

    m_xAxis           = NULL;
    m_yAxis           = NULL;
    m_area            = NULL;
    m_xAxisScrollbar  = NULL;
    m_yAxisScrollbar  = NULL;
    m_textCtrl        = NULL;

    m_activeBitmap   = wxBitmap(ledgreen_xpm);
    m_inactiveBitmap = wxBitmap(ledgrey_xpm);
    m_focusedWin     = NULL;
    m_greedy_focus   = false;

    m_redraw_type    = wxPLOTCTRL_REDRAW_BLOCKER;
    m_batch_count    = 0;

    m_axisFontSize.x    = 6;
    m_axisFontSize.y    = 12;
    ResetYAxisTextWidth();
    m_area_border_width = 1;
    m_border            = 4;
    m_min_exponential   = 1000;
    m_pen_print_width   = 0.4;

    m_timer      = NULL;
    m_winCapture = NULL;

    m_area_mouse_marker   = wxPLOTCTRL_MARKER_RECT;
    m_area_mouse_func     = wxPLOTCTRL_MOUSE_ZOOM;
    m_area_mouse_cursorid = wxCURSOR_CROSS;

    m_mouse_cursorid = wxCURSOR_ARROW;
    m_nYAxisDigits = 0;
}

const wxChar * const wxPlotCtrl::YAxisExtentString = wxT("555555555555"); // MUST BE LENGTH OF MAX_Y_DEFAULT_DIGITS
int wxPlotCtrl::g_nYAxisDefaultDigits = 9;
const int wxPlotCtrl::MIN_Y_DEFAULT_DIGITS = 1;
const int wxPlotCtrl::MAX_Y_DEFAULT_DIGITS = 12;
const wxChar * wxPlotCtrl::_GetYAxisExtentStr() const
{
  int nOffset = MAX_Y_DEFAULT_DIGITS - GetYAxisDigits();
  return YAxisExtentString + nOffset;
}


bool wxPlotCtrl::Create( wxWindow *parent, wxWindowID win_id,
                         const wxPoint &pos, const wxSize &size,
                         wxPlotCtrlAxis_Type WXUNUSED(flag),
                         const wxString &name )
{
    m_redraw_type = wxPLOTCTRL_REDRAW_BLOCKER; // no paints until finished

    if (!wxWindow::Create(parent, win_id, pos,
                          wxSize(size.x > 20 ? size.x : 20, size.y > 20 ? size.y : 20),
                          wxWANTS_CHARS|wxCLIP_CHILDREN, name))
        return false;

    SetSizeHints( 20, 20 ); // Don't allow window to get smaller than this!

    if (!s_handCursor.Ok())
    {
        wxImage image(wxBitmap(hand_xpm).ConvertToImage());
        image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, image.GetWidth()/2);
        image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, image.GetHeight()/2);
        s_handCursor = wxCursor(image);
    }
    if (!s_grabCursor.Ok())
    {
        wxImage image(wxBitmap(grab_xpm).ConvertToImage());
        image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, image.GetWidth()/2);
        image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, image.GetHeight()/2);
        s_grabCursor = wxCursor(image);
    }

    m_areaDrawer      = new wxPlotDrawerArea(this);
    m_xAxisDrawer     = new wxPlotDrawerXAxis(this);
    m_yAxisDrawer     = new wxPlotDrawerYAxis(this);
    m_keyDrawer       = new wxPlotDrawerKey(this);
    m_curveDrawer     = new wxPlotDrawerCurve(this);
    m_dataCurveDrawer = new wxPlotDrawerDataCurve(this);
    m_markerDrawer    = new wxPlotDrawerMarker(this);

    wxFont axisFont(GetFont());
    ResetYAxisTextWidth();
    GetTextExtent(wxT("5"), &m_axisFontSize.x, &m_axisFontSize.y, NULL, NULL, &axisFont);
    if ((m_axisFontSize.x < 2) || (m_axisFontSize.y < 2)) // don't want to divide by 0
    {
        m_axisFontSize.x = 6;
        m_axisFontSize.y = 12;
        wxFAIL_MSG(wxS("Can't determine the font size for the axis! I'll guess.\n")
                       wxS("The display might be corrupted, however you may continue."));
    }

    m_xAxisDrawer->SetTickFont(axisFont);
    m_yAxisDrawer->SetTickFont(axisFont);
//    m_xAxisDrawer->SetLabelFont(*wxSWISS_FONT); // needs to be rotated
//    m_yAxisDrawer->SetLabelFont(*wxSWISS_FONT); //   swiss works

    m_xAxis = new wxPlotCtrlAxis( this, ID_PLOTCTRL_X_AXIS, wxPLOTCTRL_X_AXIS );
    m_yAxis = new wxPlotCtrlAxis( this, ID_PLOTCTRL_Y_AXIS, wxPLOTCTRL_Y_AXIS );
    m_area  = new wxPlotCtrlArea( this, ID_PLOTCTRL_AREA );
    m_xAxisScrollbar = new wxScrollBar(this, ID_PLOTCTRL_X_SCROLLBAR,
                                       wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);
    m_yAxisScrollbar = new wxScrollBar(this, ID_PLOTCTRL_Y_SCROLLBAR,
                                       wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL);

    m_area->SetCursor( wxCURSOR_CROSS );
    m_area->SetBackgroundColour(*wxWHITE);
    m_xAxis->SetBackgroundColour(*wxWHITE);
    m_yAxis->SetBackgroundColour(*wxWHITE);
    wxWindow::SetBackgroundColour(*wxWHITE);

    m_area->SetForegroundColour(*wxLIGHT_GREY);

    // update the sizes of the title and axis labels
    SetPlotTitle(GetPlotTitle());
    SetXAxisLabel(GetXAxisLabel());
    SetYAxisLabel(GetYAxisLabel());

    m_redraw_type = 0;             // redraw when all done
    Redraw(wxPLOTCTRL_REDRAW_WHOLEPLOT);

    return true;
}

wxPlotCtrl::~wxPlotCtrl()
{
    delete m_areaDrawer;
    delete m_xAxisDrawer;
    delete m_yAxisDrawer;
    delete m_keyDrawer;
    delete m_curveDrawer;
    delete m_dataCurveDrawer;
    delete m_markerDrawer;
}

void wxPlotCtrl::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc(this);

    DrawActiveBitmap(&dc);
    DrawPlotCtrl(&dc);
}

void wxPlotCtrl::DrawActiveBitmap( wxDC* dc )
{
    wxCHECK_RET(dc, wxT("invalid dc"));

    if (m_xAxisScrollbar && m_yAxisScrollbar &&
        m_xAxisScrollbar->IsShown() && m_yAxisScrollbar->IsShown())
    {
        wxSize size = GetClientSize();
        int left = m_xAxisScrollbar->GetRect().GetRight();
        int top  = m_yAxisScrollbar->GetRect().GetBottom();
        wxRect rect(left, top, size.x - left, size.y - top);
        // clear background
        dc->SetBrush(wxBrush(GetBackgroundColour(), wxSOLID));
        dc->SetPen(*wxTRANSPARENT_PEN);
        dc->DrawRectangle(rect);
        // center the bitmap
        wxPoint pt(rect.x + (rect.width - 15)/2, rect.y + (rect.width - 15)/2);
        dc->DrawBitmap(m_focusedWin ? m_activeBitmap : m_inactiveBitmap,
                       pt.x, pt.y, true);
    }
}
void wxPlotCtrl::DrawPlotCtrl( wxDC *dc )
{
    wxCHECK_RET(dc, wxT("invalid dc"));

    if (m_show_title && !m_title.IsEmpty())
    {
        dc->SetFont(GetPlotTitleFont());
        dc->SetTextForeground(GetPlotTitleColour());
        dc->DrawText(m_title, m_titleRect.x, m_titleRect.y);
    }

    bool draw_xlabel = (m_show_xlabel && !m_xLabel.IsEmpty());
    bool draw_ylabel = (m_show_ylabel && !m_yLabel.IsEmpty());

    if (draw_xlabel || draw_ylabel)
    {
#if 0
        wxRect r(0, 0, 1, 1);
        dc->GetClippingBox(r);
#endif
        dc->SetFont(GetAxisLabelFont());
        dc->SetTextForeground(GetAxisLabelColour());
        dc->DestroyClippingRegion();
        if (draw_xlabel)
            dc->DrawText(m_xLabel, m_xLabelRect.x, m_xLabelRect.y);
        if (draw_ylabel)
            dc->DrawRotatedText(m_yLabel, m_yLabelRect.x, m_yLabelRect.y + m_yLabelRect.height, 90);
#if 0
        if (r != wxRect(0, 0, 1, 1))
        {
          dc->SetClippingRegion(r);
        }
#endif
    }

#ifdef DRAW_BORDERS
    // Test code for sizing to show the extent of the axes
    dc->SetBrush( *wxTRANSPARENT_BRUSH );
    dc->SetPen( wxPen(GetBorderColour(), 1, wxSOLID) );
    dc->DrawRectangle(m_titleRect);
    dc->DrawRectangle(m_xLabelRect);
    dc->DrawRectangle(m_yLabelRect);
#endif // DRAW_BORDERS
}

void wxPlotCtrl::SetPlotWinMouseCursor(wxStockCursor cursorid)
    // DJH 9/3/15 changed parameter from int to wxStockCursor
{
    if (cursorid == m_mouse_cursorid) return;
    m_mouse_cursorid = cursorid;
    SetCursor(wxCursor(cursorid));
}

void wxPlotCtrl::OnMouse( wxMouseEvent &event )
{
    if (event.ButtonDown() && IsTextCtrlShown())
    {
        HideTextCtrl(true, true);
        return;
    }

    wxSize size(GetClientSize());
    wxPoint mousePt(event.GetPosition());

    if ((m_show_title  && m_titleRect.Contains(mousePt)) ||
        (m_show_xlabel && m_xLabelRect.Contains(mousePt)) ||
        (m_show_ylabel && m_yLabelRect.Contains(mousePt)))
    {
        SetPlotWinMouseCursor(wxCURSOR_IBEAM);
    }
    else
        SetPlotWinMouseCursor(wxCURSOR_ARROW);

    if (event.ButtonDClick(1) && !IsTextCtrlShown())
    {
        if (m_show_title && m_titleRect.Contains(mousePt))
            ShowTextCtrl(wxPLOTCTRL_EDIT_TITLE, true);
        else if (m_show_xlabel && m_xLabelRect.Contains(mousePt))
            ShowTextCtrl(wxPLOTCTRL_EDIT_XAXIS, true);
        else if (m_show_ylabel && m_yLabelRect.Contains(mousePt))
            ShowTextCtrl(wxPLOTCTRL_EDIT_YAXIS, true);
    }
}

void wxPlotCtrl::ShowTextCtrl(wxPlotCtrlTextCtrl_Type type, bool send_event)
{
    switch (type)
    {
        case wxPLOTCTRL_EDIT_TITLE :
        {
            if (m_textCtrl)
            {
                if (m_textCtrl->GetId() != wxEVT_PLOTCTRL_END_TITLE_EDIT)
                    HideTextCtrl(true, true);
                else
                    return; // already shown
            }

            if (send_event)
            {
                wxPlotCtrlEvent pevent(wxEVT_PLOTCTRL_BEGIN_TITLE_EDIT, GetId(), this);
                pevent.SetString(m_title);
                if (!DoSendEvent(pevent)) return;
            }

            m_textCtrl = new wxTextCtrl(this, wxEVT_PLOTCTRL_END_TITLE_EDIT, GetPlotTitle(),
                                        wxPoint(m_areaRect.x, 0),
                                        wxSize(m_areaRect.width, m_titleRect.height+2*m_border),
                                        wxTE_PROCESS_ENTER);

            m_textCtrl->SetFont(GetPlotTitleFont());
            m_textCtrl->SetForegroundColour(GetPlotTitleColour());
            m_textCtrl->SetBackgroundColour(GetBackgroundColour());
            break;
        }
        case wxPLOTCTRL_EDIT_XAXIS :
        {
            if (m_textCtrl)
            {
                if (m_textCtrl->GetId() != wxEVT_PLOTCTRL_END_X_LABEL_EDIT)
                    HideTextCtrl(true, true);
                else
                    return; // already shown
            }

            if (send_event)
            {
                wxPlotCtrlEvent pevent(wxEVT_PLOTCTRL_BEGIN_X_LABEL_EDIT, GetId(), this);
                pevent.SetString(m_xLabel);
                if (!DoSendEvent(pevent)) return;
            }

            m_textCtrl = new wxTextCtrl(this, wxEVT_PLOTCTRL_END_X_LABEL_EDIT, GetXAxisLabel(),
                                        wxPoint(m_areaRect.x, m_xAxisRect.GetBottom()),
                                        wxSize(m_areaRect.width, m_xLabelRect.height+2*m_border),
                                        wxTE_PROCESS_ENTER);

            m_textCtrl->SetFont(GetAxisLabelFont());
            m_textCtrl->SetForegroundColour(GetAxisLabelColour());
            m_textCtrl->SetBackgroundColour(GetBackgroundColour());
            break;
        }
        case wxPLOTCTRL_EDIT_YAXIS :
        {
            if (m_textCtrl)
            {
                if (m_textCtrl->GetId() != wxEVT_PLOTCTRL_END_Y_LABEL_EDIT)
                    HideTextCtrl(true, true);
                else
                    return; // already shown
            }

            if (send_event)
            {
                wxPlotCtrlEvent pevent(wxEVT_PLOTCTRL_BEGIN_Y_LABEL_EDIT, GetId(), this);
                pevent.SetString(m_yLabel);
                if (!DoSendEvent(pevent)) return;
            }

            m_textCtrl = new wxTextCtrl(this, wxEVT_PLOTCTRL_END_Y_LABEL_EDIT, GetYAxisLabel(),
                                        wxPoint(0, m_areaRect.y+m_areaRect.height/2),
                                        wxSize(m_clientRect.width - m_axisFontSize.y/2, m_yLabelRect.width+2*m_border),
                                        wxTE_PROCESS_ENTER);

            m_textCtrl->SetFont(GetAxisLabelFont());
            m_textCtrl->SetForegroundColour(GetAxisLabelColour());
            m_textCtrl->SetBackgroundColour(GetBackgroundColour());
            break;
        }
    }
}

void wxPlotCtrl::HideTextCtrl(bool save_value, bool send_event)
{
    wxCHECK_RET(m_textCtrl, wxT("HideTextCtrl, but textctrl is not shown"));

    long event_type = m_textCtrl->GetId();
    wxString value  = m_textCtrl->GetValue();

    m_textCtrl->Destroy();
    m_textCtrl = NULL;

    if (!save_value)
        return;

    bool changed = false;

    if (event_type == wxEVT_PLOTCTRL_END_TITLE_EDIT)
        changed = (value != GetPlotTitle());
    else if (event_type == wxEVT_PLOTCTRL_END_X_LABEL_EDIT)
        changed = (value != GetXAxisLabel());
    else if (event_type == wxEVT_PLOTCTRL_END_Y_LABEL_EDIT)
        changed = (value != GetYAxisLabel());

    if (!changed)
        return;

    if (send_event)
    {
        wxPlotCtrlEvent event(event_type, GetId(), this);
        event.SetString(value);
        if (!DoSendEvent(event)) return;
    }

    if (event_type == wxEVT_PLOTCTRL_END_TITLE_EDIT)
        SetPlotTitle(value);
    else if (event_type == wxEVT_PLOTCTRL_END_X_LABEL_EDIT)
        SetXAxisLabel(value);
    else if (event_type == wxEVT_PLOTCTRL_END_Y_LABEL_EDIT)
        SetYAxisLabel(value);
}

bool wxPlotCtrl::IsTextCtrlShown() const
{
    return m_textCtrl && m_textCtrl->IsShown();
}

void wxPlotCtrl::OnTextEnter( wxCommandEvent &event )
{
    // we send a fake event so that we can destroy the textctrl the second time
    if (event.GetId() == 1)
        HideTextCtrl(true, true);
    else
    {
        wxCommandEvent newevt(wxEVT_COMMAND_TEXT_ENTER, 1);
        GetEventHandler()->AddPendingEvent( newevt );
    }
}

void wxPlotCtrl::OnIdle( wxIdleEvent &event )
{
    CheckFocus();
    event.Skip();
}

bool wxPlotCtrl::CheckFocus()
{
    wxWindow *win = FindFocus();

    if (win == m_focusedWin)
        return true;

    if ((win==m_area)||(win==m_xAxis)||(win==m_yAxis)||(win==this))
    {
        if (!m_focusedWin)
        {
            m_focusedWin = win;
            wxClientDC dc(this);
            wxSize size = GetClientSize();
            dc.DrawBitmap(m_activeBitmap, size.GetWidth()-15, size.GetHeight()-15,true);
        }
    }
    else if (m_focusedWin)
    {
        m_focusedWin = NULL;
        wxClientDC dc(this);
        wxSize size = GetClientSize();
        dc.DrawBitmap(m_inactiveBitmap, size.GetWidth()-15, size.GetHeight()-15,true);
    }
    return m_focusedWin != NULL;
}

void wxPlotCtrl::EndBatch(bool force_refresh)
{
    if ( m_batch_count > 0 )
    {
        m_batch_count--;
        if ( (m_batch_count <= 0) && force_refresh )
        {
            Redraw(wxPLOTCTRL_REDRAW_WHOLEPLOT);
            AdjustScrollBars();
        }
    }
}

bool wxPlotCtrl::SetBackgroundColour( const wxColour &colour )
{
    wxCHECK_MSG(colour.Ok(), false, wxT("invalid colour"));
    m_area->SetBackgroundColour(colour);
    m_xAxis->SetBackgroundColour(colour);
    m_yAxis->SetBackgroundColour(colour);
    wxWindow::SetBackgroundColour(colour);

    Redraw(wxPLOTCTRL_REDRAW_EVERYTHING);
    return true;
}
void wxPlotCtrl::SetGridColour( const wxColour &colour )
{
    wxCHECK_RET(colour.Ok(), wxT("invalid colour"));
    m_area->SetForegroundColour(colour);
    Redraw(wxPLOTCTRL_REDRAW_PLOT);
}
void wxPlotCtrl::SetBorderColour( const wxColour &colour )
{
    wxCHECK_RET(colour.Ok(), wxT("invalid colour"));
    m_borderColour = colour;
    Redraw(wxPLOTCTRL_REDRAW_PLOT);
}
void wxPlotCtrl::SetCursorColour( const wxColour &colour )
{
    wxCHECK_RET(colour.Ok(), wxT("invalid colour"));
    m_cursorMarker.GetPen().SetColour(colour);
    wxClientDC dc(m_area);
    DrawCurveCursor( &dc );
}
wxColour wxPlotCtrl::GetCursorColour() const
{
    return m_cursorMarker.GetPen().GetColour();
}
int wxPlotCtrl::GetCursorSize() const
{
    return m_cursorMarker.GetSize().x;
}
void wxPlotCtrl::SetCursorSize(int size)
{
    m_cursorMarker.SetSize(wxSize(size, size));
}

wxFont wxPlotCtrl::GetAxisFont() const
{
    return m_xAxisDrawer->m_tickFont; // FIXME
}
wxColour wxPlotCtrl::GetAxisColour() const
{
    return m_xAxisDrawer->m_tickColour.GetColour(); // FIXME
}


void wxPlotCtrl::SetAxisFont( const wxFont &font )
{
    wxCHECK_RET(font.Ok(), wxT("invalid font"));

    if (m_xAxisDrawer) m_xAxisDrawer->SetTickFont(font);
    if (m_yAxisDrawer) m_yAxisDrawer->SetTickFont(font);

    int x=6, y=12, decent=0, leading=0;

    GetTextExtent(wxT("5"), &x, &y, &decent, &leading, &font);
    m_axisFontSize.x = x+leading;
    m_axisFontSize.y = y+decent;

    ResetYAxisTextWidth();

    //m_axisFontSize.x = m_xAxis->GetCharWidth();
    //m_axisFontSize.y = m_xAxis->GetCharHeight();
    if ((m_axisFontSize.x < 2) || (m_axisFontSize.y < 2)) // don't want to divide by 0
    {
        static bool first_try = false;

        m_axisFontSize.x = 6;
        m_axisFontSize.y = 12;
        wxMessageBox(wxT("Can't determine the font size for the axis.\n")
                     wxT("Reverting to a default font."),
                     wxT("Font error"), wxICON_ERROR, this);

        if (!first_try)
        {
            first_try = true;
            SetAxisFont(*wxNORMAL_FONT);
        }
        else
            first_try = false;
    }

    DoSize();
    Redraw(wxPLOTCTRL_REDRAW_XAXIS|wxPLOTCTRL_REDRAW_YAXIS);
}
void wxPlotCtrl::SetAxisColour( const wxColour &colour )
{
    wxCHECK_RET(colour.Ok(), wxT("invalid colour"));
    if (m_xAxisDrawer) m_xAxisDrawer->SetTickColour(colour);
    if (m_yAxisDrawer) m_yAxisDrawer->SetTickColour(colour);
    Redraw(wxPLOTCTRL_REDRAW_XAXIS|wxPLOTCTRL_REDRAW_YAXIS);
}

wxFont wxPlotCtrl::GetAxisLabelFont() const
{
    return m_xAxisDrawer->m_labelFont; // FIXME
}
wxColour wxPlotCtrl::GetAxisLabelColour() const
{
    return m_xAxisDrawer->m_labelColour.GetColour(); // FIXME
}
void wxPlotCtrl::SetAxisLabelFont( const wxFont &font )
{
    wxCHECK_RET(font.Ok(), wxT("invalid font"));
    if (m_xAxisDrawer) m_xAxisDrawer->SetLabelFont(font);
    if (m_yAxisDrawer) m_yAxisDrawer->SetLabelFont(font);
    SetXAxisLabel(GetXAxisLabel());      // FIXME - lazy hack
    SetYAxisLabel(GetYAxisLabel());
}
void wxPlotCtrl::SetAxisLabelColour( const wxColour &colour )
{
    wxCHECK_RET(colour.Ok(), wxT("invalid colour"));
    if (m_xAxisDrawer) m_xAxisDrawer->SetLabelColour(colour);
    if (m_yAxisDrawer) m_yAxisDrawer->SetLabelColour(colour);
    SetXAxisLabel(GetXAxisLabel());      // FIXME - lazy hack
    SetYAxisLabel(GetYAxisLabel());
}
void wxPlotCtrl::SetPlotTitleFont(const wxFont &font)
{
    wxCHECK_RET(font.Ok(), wxT("invalid font"));
    m_titleFont = font;
    SetPlotTitle(GetPlotTitle());
}
void wxPlotCtrl::SetPlotTitleColour(const wxColour &colour)
{
    wxCHECK_RET(colour.Ok(), wxT("invalid colour"));
    m_titleColour = colour;
    SetPlotTitle(GetPlotTitle());
}

wxFont wxPlotCtrl::GetKeyFont() const
{
    return m_keyDrawer->m_font; // FIXME
}
wxColour wxPlotCtrl::GetKeyColour() const
{
    return m_keyDrawer->m_fontColour.GetColour(); // FIXME
}
void wxPlotCtrl::SetKeyFont( const wxFont &font )
{
    wxCHECK_RET(font.Ok(), wxT("invalid font"));
    m_keyDrawer->SetFont(font);
    Redraw(wxPLOTCTRL_REDRAW_PLOT);
}
void wxPlotCtrl::SetKeyColour( const wxColour & colour )
{
    wxCHECK_RET(colour.Ok(), wxT("invalid colour"));
    m_keyDrawer->SetFontColour(colour);
    Redraw(wxPLOTCTRL_REDRAW_PLOT);
}

// ------------------------------------------------------------------------
// Title, axis labels, and key
// ------------------------------------------------------------------------

void wxPlotCtrl::SetXAxisLabel(const wxString &label)
{
    if (label.IsEmpty())
        m_xLabel = wxT("X - Axis");
    else
        m_xLabel = label;

    wxFont font = GetAxisLabelFont();
    GetTextExtent(m_xLabel, &m_xLabelRect.width, &m_xLabelRect.height, NULL, NULL, &font);

    m_xLabel = label;
    Refresh();
    DoSize();
}

void wxPlotCtrl::SetYAxisLabel(const wxString &label)
{
    if (label.IsEmpty())
        m_yLabel = wxT("Y - Axis");
    else
        m_yLabel = label;

    wxFont font = GetAxisLabelFont();
    GetTextExtent(m_yLabel, &m_yLabelRect.height, &m_yLabelRect.width, NULL, NULL, &font);

    m_yLabel = label;

    Refresh();
    DoSize();
}

void wxPlotCtrl::SetPlotTitle(const wxString &title)
{
    if (title.IsEmpty())
        m_title = wxT("Title");
    else
        m_title = title;

    wxFont font = GetPlotTitleFont();
    GetTextExtent(m_title, &m_titleRect.width, &m_titleRect.height, NULL, NULL, &font);

    m_title = title;

    Refresh();
    DoSize();
}

wxPoint wxPlotCtrl::GetKeyPosition() const
{
    return m_keyDrawer->m_keyPosition;
}
bool wxPlotCtrl::GetKeyInside() const
{
    return m_keyDrawer->m_key_inside;
}
void wxPlotCtrl::SetKeyPosition(const wxPoint &pos, bool stay_inside)
{
    m_keyDrawer->m_keyPosition = pos;
    m_keyDrawer->m_key_inside = stay_inside;
    Redraw(wxPLOTCTRL_REDRAW_PLOT);
}

void wxPlotCtrl::CreateKeyString()
{
    m_keyString.Clear();
    int n, count = m_curves.GetCount();
    for (n = 0; n < count; n++)
    {
        wxString key;
        if (GetDataCurve(n))
            key = GetDataCurve(n)->GetFilename();
        else if (GetFunctionCurve(n))
            key = GetFunctionCurve(n)->GetFunctionString();
        else
            key.Printf(wxT("Curve %d"), n);

        m_keyString += (key + wxT("\n"));
    }
}

// ------------------------------------------------------------------------
// Curve Accessors
// ------------------------------------------------------------------------

bool wxPlotCtrl::AddCurve( wxPlotCurve *curve, bool select, bool send_event )
{
    if (!curve || !curve->Ok())
    {
        if (curve) delete curve;
        wxCHECK_MSG( false, false, wxT("Invalid curve") );
    }

    m_curves.Add( curve );
    m_curveSelections.Add(new wxRangeDoubleSelection());
    m_dataSelections.Add(new wxRangeIntSelection());

    CalcBoundingPlotRect();
    CreateKeyString();

    if (send_event)
    {
        wxPlotCtrlEvent event(wxEVT_PLOTCTRL_ADD_CURVE, GetId(), this);
        event.SetCurve(curve, m_curves.GetCount()-1);
        (void)DoSendEvent( event );
    }

    m_batch_count++;
    if (select) SetActiveCurve(curve, send_event);
    m_batch_count--;

    if (m_fit_on_new_curve)
        SetZoom( -1, -1, 0, 0, send_event );
    else
        Redraw(wxPLOTCTRL_REDRAW_PLOT);

    return true;
}

bool wxPlotCtrl::AddCurve( const wxPlotCurve &curve, bool select, bool send_event )
{
    wxCHECK_MSG(curve.Ok(), false, wxT("invalid wxPlotCurve"));

    if (wxDynamicCast(&curve, wxPlotData))
        return AddCurve(new wxPlotData(*wxDynamicCast(&curve, wxPlotData)), select, send_event);
    if (wxDynamicCast(&curve, wxPlotFunction))
        return AddCurve(new wxPlotFunction(*wxDynamicCast(&curve, wxPlotFunction)), select, send_event);

    wxFAIL_MSG(wxT("Unable to ref curve type added to plot"));
    return false;
}

bool wxPlotCtrl::DeleteCurve( wxPlotCurve* curve, bool send_event )
{
    wxCHECK_MSG(curve, false, wxT("invalid plotcurve"));

    int index = m_curves.Index( *curve );
    wxCHECK_MSG( index != wxNOT_FOUND, false, wxT("Unknown PlotCurve") );

    return DeleteCurve(index, send_event);
}

bool wxPlotCtrl::DeleteCurve( int n, bool send_event )
{
    wxCHECK_MSG((n>=-1)&&(n<int(m_curves.GetCount())), false, wxT("Invalid curve index"));

    if (send_event)
    {
        wxPlotCtrlEvent event( wxEVT_PLOTCTRL_DELETING_CURVE, GetId(), this );
        event.SetCurveIndex(n);
        if (!DoSendEvent(event)) return false;
    }

    BeginBatch(); // don't redraw yet

    if (n < 0)
    {
        InvalidateCursor(send_event);
        ClearSelectedRanges(-1, send_event);
        m_dataSelections.Clear();
        m_curveSelections.Clear();
        m_curves.Clear();
    }
    else
    {
        if (m_cursor_curve == n)
            InvalidateCursor(send_event);
        else if (m_cursor_curve > n)
            m_cursor_curve--;

        ClearSelectedRanges(n, send_event);
        m_dataSelections.RemoveAt(n);
        m_curveSelections.RemoveAt(n);
        m_curves.RemoveAt(n);
    }

    int old_active_index = m_active_index;
    m_active_index = -1;
    m_activeCurve = NULL;

    if (old_active_index >= int(m_curves.GetCount()))
    {
        // force this invalid, can't override this, the curve is "gone"
        SetActiveIndex( m_curves.GetCount() - 1, send_event );
    }
    else if (old_active_index >= 0)
    {
        SetActiveIndex( old_active_index, send_event );
    }

    EndBatch(false); // still don't redraw

    CalcBoundingPlotRect();
    CreateKeyString();
    Redraw(wxPLOTCTRL_REDRAW_PLOT);

    if (send_event)
    {
        wxPlotCtrlEvent event1( wxEVT_PLOTCTRL_DELETED_CURVE, GetId(), this );
        event1.SetCurveIndex(n);
        (void)DoSendEvent( event1 );
    }

    return true;
}

wxPlotCurve* wxPlotCtrl::GetCurve( int n ) const
{
    wxCHECK_MSG((n >= 0) && (n < GetCurveCount()), NULL, wxT("Invalid index"));
    return &(m_curves.Item(n));
}

void wxPlotCtrl::SetActiveCurve( wxPlotCurve* current, bool send_event )
{
    wxCHECK_RET(current, wxT("Invalid curve"));

    int index = m_curves.Index( *current );
    wxCHECK_RET( index != wxNOT_FOUND, wxT("Unknown PlotCurve") );

    SetActiveIndex( index, send_event );
}

void wxPlotCtrl::SetActiveIndex( int curve_index, bool send_event )
{
    wxCHECK_RET((curve_index < GetCurveCount()), wxT("Invalid index"));

    if (send_event)
    {
        wxPlotCtrlEvent event( wxEVT_PLOTCTRL_CURVE_SEL_CHANGING, GetId(), this);
        event.SetCurve(m_activeCurve, m_active_index);
        if (!DoSendEvent(event)) return;
    }

    if ((curve_index >= 0) && m_curves.Item(curve_index).Ok())
    {
        m_active_index = curve_index;
        m_activeCurve = &(m_curves.Item(curve_index));
    }
    else
    {
        m_active_index = -1;
        m_activeCurve = NULL;
    }

    if (send_event)
    {
        wxPlotCtrlEvent event( wxEVT_PLOTCTRL_CURVE_SEL_CHANGED, GetId(), this);
        event.SetCurve(m_activeCurve, m_active_index);
        (void)DoSendEvent( event );
    }

    Redraw(wxPLOTCTRL_REDRAW_PLOT);
}

wxArrayInt wxPlotCtrl::GetPlotDataIndexes() const
{
    wxArrayInt array;
    size_t n, count = m_curves.GetCount();
    for (n=0; n<count; n++)
    {
        if (wxDynamicCast(&m_curves.Item(n), wxPlotData))
            array.Add(n);
    }
    return array;
}
wxArrayInt wxPlotCtrl::GetPlotFunctionIndexes() const
{
    wxArrayInt array;
    size_t n, count = m_curves.GetCount();
    for (n=0; n<count; n++)
    {
        if (wxDynamicCast(&m_curves.Item(n), wxPlotFunction))
            array.Add(n);
    }
    return array;
}

//-------------------------------------------------------------------------
// Markers
//-------------------------------------------------------------------------

int wxPlotCtrl::AddMarker( const wxPlotMarker& marker )
{
    m_plotMarkers.Add(marker);
    return m_plotMarkers.GetCount() - 1;
}

void wxPlotCtrl::RemoveMarker(int marker)
{
    wxCHECK_RET((marker >= 0) && (marker < (int)m_plotMarkers.GetCount()), wxT("Invalid marker number"));
    m_plotMarkers.RemoveAt(marker);
}

void wxPlotCtrl::ClearMarkers()
{
    m_plotMarkers.Clear();
}

wxPlotMarker wxPlotCtrl::GetMarker(int marker) const
{
    wxCHECK_MSG((marker >= 0) && (marker < (int)m_plotMarkers.GetCount()), wxPlotMarker(),
                wxT("Invalid marker number"));
    return m_plotMarkers[marker];
}

//-------------------------------------------------------------------------
// Cursor position
//-------------------------------------------------------------------------

void wxPlotCtrl::InvalidateCursor(bool send_event)
{
    bool changed = m_cursor_curve >= 0;
    m_cursor_curve = -1;
    m_cursor_index = -1;
    m_cursorMarker.SetPlotPosition(wxPoint2DDouble(0, 0));

    if (send_event && changed)
    {
        wxPlotCtrlEvent plotEvent(wxEVT_PLOTCTRL_CURSOR_CHANGED, GetId(), this );
        (void)DoSendEvent( plotEvent );
    }
}
bool wxPlotCtrl::IsCursorValid()
{
    if (m_cursor_curve < 0) return false;

    // sanity check
    if (m_cursor_curve >= int(m_curves.GetCount()))
    {
        wxFAIL_MSG(wxT("Invalid cursor index"));
        InvalidateCursor(true);
        return false;
    }

    wxPlotData *plotData = GetDataCurve(m_cursor_curve);
    if (plotData)
    {
        // sanity check
        if (m_cursor_index < 0)
        {
            wxFAIL_MSG(wxT("Invalid cursor data index"));
            InvalidateCursor(true);
            return false;
        }
        // if the curve shrinks or is bad
        if (!plotData->Ok() || (m_cursor_index >= (int)plotData->GetCount()))
        {
            InvalidateCursor(true);
            return false;
        }

        m_cursorMarker.SetPlotPosition(plotData->GetPoint(m_cursor_index));
    }
    else
    {
        wxDouble x = m_cursorMarker.GetPlotRect().m_x;
        m_cursorMarker.GetPlotRect().m_y = GetCurve(m_cursor_curve)->GetY(x);
    }

    return true;
}

wxPoint2DDouble wxPlotCtrl::GetCursorPoint()
{
    wxCHECK_MSG(IsCursorValid(), wxPoint2DDouble(0, 0), wxT("invalid cursor"));
    return m_cursorMarker.GetPlotPosition();
}

bool wxPlotCtrl::SetCursorDataIndex(int curve_index, int cursor_index, bool send_event)
{
    wxCHECK_MSG(CurveIndexOk(curve_index) && GetDataCurve(curve_index),
                false, wxT("invalid curve index"));

    wxPlotData *plotData = GetDataCurve(curve_index);

    wxCHECK_MSG((cursor_index>=0) && plotData->Ok() && (cursor_index < (int)plotData->GetCount()),
                 false, wxT("invalid index"));

    // do nothing if already set
    if ((m_cursor_curve == curve_index) && (m_cursor_index == cursor_index))
        return false;

    wxPoint2DDouble cursorPt(plotData->GetPoint(cursor_index));

    if (send_event)
    {
        wxPlotCtrlEvent cursor_event(wxEVT_PLOTCTRL_CURSOR_CHANGING, GetId(), this );
        cursor_event.SetPosition( cursorPt.m_x, cursorPt.m_y );
        cursor_event.SetCurve( plotData, curve_index );
        cursor_event.SetCurveDataIndex(cursor_index);
        if (!DoSendEvent( cursor_event )) return false;
    }

    int old_cursor_curve = m_cursor_curve;
    int old_cursor_index = m_cursor_index;
    m_cursorMarker.SetPlotPosition(cursorPt);
    m_cursor_curve = curve_index;
    m_cursor_index = cursor_index;

    if (send_event)
    {
        wxPlotCtrlEvent cursor_event(wxEVT_PLOTCTRL_CURSOR_CHANGED, GetId(), this );
        cursor_event.SetPosition( cursorPt.m_x, cursorPt.m_y );
        cursor_event.SetCurve( plotData, curve_index );
        cursor_event.SetCurveDataIndex(cursor_index);
        (void)DoSendEvent( cursor_event );
    }

    if ((m_active_index == old_cursor_curve) && (m_active_index == m_cursor_curve))
    {
        RedrawDataCurve(curve_index, old_cursor_index, old_cursor_index);
        RedrawDataCurve(curve_index, m_cursor_index, m_cursor_index);
    }
    else
        Redraw(wxPLOTCTRL_REDRAW_PLOT);

    return true;
}
bool wxPlotCtrl::SetCursorXPoint(int curve_index, double x, bool send_event)
{
    wxCHECK_MSG(CurveIndexOk(curve_index), false, wxT("invalid curve index"));

    if (GetDataCurve(curve_index))
        return SetCursorDataIndex(curve_index, GetDataCurve(curve_index)->GetIndexFromX(x), send_event);

    // do nothing if already set
    if ((m_cursor_curve == curve_index) && (m_cursorMarker.GetPlotRect().m_x == x))
        return false;

    wxPlotCurve *plotCurve = GetCurve(curve_index);
    wxPoint2DDouble cursorPt(x, plotCurve->GetY(x));

    if (send_event)
    {
        wxPlotCtrlEvent cursor_event(wxEVT_PLOTCTRL_CURSOR_CHANGING, GetId(), this );
        cursor_event.SetPosition(cursorPt.m_x, cursorPt.m_y);
        cursor_event.SetCurve( plotCurve, curve_index );
        if (!DoSendEvent( cursor_event )) return false;
    }

    m_cursorMarker.SetPlotPosition(cursorPt);
    m_cursor_curve = curve_index;
    m_cursor_index = -1;

    if (send_event)
    {
        wxPlotCtrlEvent cursor_event(wxEVT_PLOTCTRL_CURSOR_CHANGED, GetId(), this );
        cursor_event.SetPosition(cursorPt.m_x, cursorPt.m_y);
        cursor_event.SetCurve( plotCurve, curve_index );
        (void)DoSendEvent( cursor_event );
    }

    Redraw(wxPLOTCTRL_REDRAW_PLOT);
    return true;
}

void wxPlotCtrl::MakeCursorVisible(bool center, bool send_event)
{
    wxCHECK_RET(IsCursorValid(), wxT("invalid plot cursor"));

    if (center)
    {
        wxPoint2DDouble origin = m_viewRect.GetLeftTop() -
                                 m_viewRect.GetCentre() +
                                 GetCursorPoint();

        SetOrigin(origin.m_x, origin.m_y, send_event);
        return;
    }

    wxPoint2DDouble origin = GetCursorPoint();

    if (m_viewRect.Contains(origin))
        return;

    double dx = 4/m_zoom.m_x;
    double dy = 4/m_zoom.m_y;

    if (origin.m_x < m_viewRect.m_x)
        origin.m_x -= dx;
    else if (origin.m_x > m_viewRect.GetRight())
        origin.m_x = m_viewRect.m_x + (origin.m_x - m_viewRect.GetRight()) + dx;
    else
        origin.m_x = m_viewRect.m_x;

    if (origin.m_y < m_viewRect.m_y)
        origin.m_y -= dy;
    else if (origin.m_y > m_viewRect.GetBottom())
        origin.m_y = m_viewRect.m_y + (origin.m_y - m_viewRect.GetBottom()) + dy;
    else
        origin.m_y = m_viewRect.m_y;

    SetOrigin(origin.m_x, origin.m_y, send_event);
}

//-------------------------------------------------------------------------
// Selected points, data curves use
//-------------------------------------------------------------------------
bool wxPlotCtrl::HasSelection(int curve_index) const
{
    if (curve_index == -1)
    {
        int n, count = m_curveSelections.GetCount();
        for ( n = 0; n < count; n++ )
        {
            if ((m_curveSelections[n].GetCount() > 0) ||
                (m_dataSelections[n].GetCount() > 0))
                return true;
        }
        return false;
    }

    wxCHECK_MSG(CurveIndexOk(curve_index), false, wxT("invalid curve index"));
    return (m_curveSelections[curve_index].GetCount() > 0) ||
           (m_dataSelections[curve_index].GetCount() > 0);
}

wxRangeDoubleSelection *wxPlotCtrl::GetCurveSelection(int curve_index) const
{
    wxCHECK_MSG(CurveIndexOk(curve_index), NULL, wxT("invalid curve index"));
    return &m_curveSelections[curve_index];
}

wxRangeIntSelection *wxPlotCtrl::GetDataCurveSelection(int curve_index) const
{
    wxCHECK_MSG(CurveIndexOk(curve_index), NULL, wxT("invalid curve index"));
    return &m_dataSelections[curve_index];
}

bool wxPlotCtrl::UpdateSelectionState(int curve_index, bool send_event)
{
    wxCHECK_MSG(CurveIndexOk(curve_index), false, wxT("invalid curve index"));
    switch (m_selection_type)
    {
        case wxPLOTCTRL_SELECT_NONE   : break; // should have been handled
        case wxPLOTCTRL_SELECT_SINGLE :
        {
            if (HasSelection())
                return ClearSelectedRanges(-1, send_event);

            break;
        }
        case wxPLOTCTRL_SELECT_SINGLE_CURVE :
        {
            int n, count = m_curves.GetCount();
            bool done = false;
            for ( n = 0; n < count; n++ )
            {
                if ((n != curve_index) && HasSelection(n))
                    done |= ClearSelectedRanges(n, send_event);
            }
            return done;
        }
        case wxPLOTCTRL_SELECT_SINGLE_PER_CURVE :
        {
            if (HasSelection(curve_index))
                return ClearSelectedRanges(curve_index, send_event);

            break;
        }
        case wxPLOTCTRL_SELECT_MULTIPLE : break; // anything goes
        default : break;
    }

    return false;
}

bool wxPlotCtrl::DoSelectRectangle(int curve_index, const wxRect2DDouble &rect,
                                     bool select, bool send_event)
{
    wxCHECK_MSG((curve_index >= -1) && (curve_index<int(m_curves.GetCount())),
                false, wxT("invalid plotcurve index"));
    wxCHECK_MSG((rect.m_width > 0) || (rect.m_height > 0), false, wxT("invalid selection range"));

    if (m_selection_type == wxPLOTCTRL_SELECT_NONE)
        return false;

    if (!IsFinite(rect.m_x, wxT("Selection x is NaN")) ||
        !IsFinite(rect.m_y, wxT("Selection y is NaN")) ||
        !IsFinite(rect.m_width, wxT("Selection width is NaN")) ||
        !IsFinite(rect.m_height, wxT("Selection height is NaN")) )
        return false;

    bool done = false;

    // Run this code for all the curves if curve == -1 then exit
    if (curve_index == -1)
    {
        size_t n, curve_count = m_curves.GetCount();

        for (n = 0; n < curve_count; n++)
            done |= DoSelectRectangle(n, rect, select, send_event);

        return done;
    }

    // check the selection type and clear previous selections if necessary
    if (select)
        UpdateSelectionState(curve_index, send_event);

    bool is_x_range = rect.m_height <= 0;
    bool is_y_range = rect.m_width <= 0;
    wxRangeDouble xRange(rect.m_x, rect.GetRight());
    wxRangeDouble yRange(rect.m_y, rect.GetBottom());

    wxPlotData *plotData = GetDataCurve(curve_index);
    if (plotData)
    {
        wxCHECK_MSG(plotData->Ok(), false, wxT("Invalid data curve"));
        wxRect2DDouble r(plotData->GetBoundingRect());

        if ((xRange.m_max < r.GetLeft()) || (xRange.m_min > r.GetRight()))
            return false;

        if (is_x_range && plotData->GetIsXOrdered())
        {
            int min_  = plotData->GetIndexFromX(xRange.m_min);
            int max_  = plotData->GetIndexFromX(xRange.m_max);
            int count = plotData->GetCount();

            if ( (plotData->GetXValue(min_) > xRange.m_min) && (min_ > 0) &&
                (plotData->GetXValue(min_-1) > xRange.m_min) )
                min_--;
            if ( (plotData->GetXValue(min_) < xRange.m_min) && (min_ < count -1) )
                min_++;

            if ( (plotData->GetXValue(max_) > xRange.m_max) && (max_ > 0) )
                max_--;
            if ( (plotData->GetXValue(max_) < xRange.m_max) && (max_ < count-1) &&
                (plotData->GetXValue(max_+1) < xRange.m_max) )
                max_++;

            wxRangeInt sel(min_, max_); // always check if max < min! - not a bug

            if (!sel.IsEmpty())
            {
                if (select)
                    m_curveSelections[curve_index].SelectRange(wxRangeDouble(rect.m_x, rect.GetRight()));
                else
                    m_curveSelections[curve_index].DeselectRange(wxRangeDouble(rect.m_x, rect.GetRight()));

                return DoSelectDataRange(curve_index, sel, select, send_event);
            }
            else
                return false;
        }
        else // not ordered or not just an x selection
        {
            int i, count = plotData->GetCount();
            int first_sel = -1;
            double *x_data = plotData->GetXData();
            double *y_data = plotData->GetYData();

            int min_ = plotData->GetCount()-1, max_ = 0;

            wxRangeIntSelection ranges;

            for (i=0; i<count; i++)
            {
                if ((is_x_range && xRange.Contains(*x_data)) ||
                    (is_y_range && yRange.Contains(*y_data)) ||
                    (!is_x_range && !is_y_range && wxPlotRect2DDoubleContains(*x_data, *y_data, rect)))
                {
                    if (select)
                    {
                        if (m_dataSelections[curve_index].SelectRange(wxRangeInt(i,i)))
                        {
                            ranges.SelectRange(wxRangeInt(i,i));
                            done = true;
                        }
                    }
                    else
                    {
                        if (m_dataSelections[curve_index].DeselectRange(wxRangeInt(i,i)))
                        {
                            ranges.SelectRange(wxRangeInt(i,i));
                            done = true;
                        }
                    }

                    min_ = wxMin(min_, i);
                    max_ = wxMin(max_, i);

                    if (done && (first_sel == -1))
                        first_sel = i;
                }

                x_data++;
                y_data++;
            }

            if (done && (min_ <= max_))
                RedrawDataCurve(curve_index, min_, max_);

            if (done)
            {
                if (select)
                    m_curveSelections[curve_index].SelectRange(wxRangeDouble(rect.m_x, rect.GetRight()));
                else
                    m_curveSelections[curve_index].DeselectRange(wxRangeDouble(rect.m_x, rect.GetRight()));
            }

            if (send_event && done)
            {
                wxPlotCtrlSelEvent event( wxEVT_PLOTCTRL_RANGE_SEL_CHANGED, GetId(), this);
                event.SetCurve(GetCurve(curve_index), curve_index);
                event.SetDataSelectionRange( wxRangeInt(first_sel, first_sel), select );
                event.SetDataSelections(ranges);
                (void)DoSendEvent( event );
            }

            return done;
        }
    }
    else
    {
        if (select)
            done = m_curveSelections[curve_index].SelectRange(wxRangeDouble(rect.m_x, rect.GetRight()));
        else
            done = m_curveSelections[curve_index].DeselectRange(wxRangeDouble(rect.m_x, rect.GetRight()));

        if (send_event && done)
        {
            wxPlotCtrlSelEvent event( wxEVT_PLOTCTRL_RANGE_SEL_CHANGED, GetId(), this);
            event.SetCurve(GetCurve(curve_index), curve_index);
            event.SetCurveSelectionRange( xRange, select );
            (void)DoSendEvent( event );
        }

        if (done) RedrawCurve(curve_index, xRange.m_min, xRange.m_max);

        return done;
    }
}

bool wxPlotCtrl::DoSelectDataRange(int curve_index, const wxRangeInt &range,
                                     bool select, bool send_event)
{
    wxCHECK_MSG(CurveIndexOk(curve_index), false, wxT("invalid plotcurve index"));
    wxCHECK_MSG(!range.IsEmpty(), false, wxT("invalid selection range"));

    if (m_selection_type == wxPLOTCTRL_SELECT_NONE)
        return false;

    wxPlotData *plotData = GetDataCurve(curve_index);
    wxCHECK_MSG(plotData && (range.m_min >= 0) && (range.m_max < (int)plotData->GetCount()), false, wxT("invalid index"));

    // check the selection type and clear previous selections if necessary
    if (select)
        UpdateSelectionState(curve_index, send_event);

    bool done = false;

    if (select)
        done = m_dataSelections[curve_index].SelectRange(range);
    else
        done = m_dataSelections[curve_index].DeselectRange(range);

    if (send_event && done)
    {
        wxPlotCtrlSelEvent event( wxEVT_PLOTCTRL_RANGE_SEL_CHANGED, GetId(), this);
        event.SetCurve(GetCurve(curve_index), curve_index);
        event.SetDataSelectionRange( range, select );
        event.GetDataSelections().SelectRange(range);
        (void)DoSendEvent( event );
    }

    if (done) RedrawDataCurve(curve_index, range.m_min, range.m_max);

    return done;
}

int wxPlotCtrl::GetSelectedRangeCount(int curve_index) const
{
    wxCHECK_MSG(CurveIndexOk(curve_index), 0, wxT("invalid plotcurve index"));

    if (GetDataCurve(curve_index))
        return m_dataSelections[curve_index].GetCount();
    else
        return m_curveSelections[curve_index].GetCount();
}

bool wxPlotCtrl::ClearSelectedRanges(int curve_index, bool send_event)
{
    wxCHECK_MSG((curve_index >= -1) && (curve_index<int(m_curves.GetCount())),
                false, wxT("invalid plotcurve index"));

    bool done = false;

    if (curve_index == -1)
    {
        for (size_t n=0; n<m_curves.GetCount(); n++)
            done |= ClearSelectedRanges(n, send_event);

        return done;
    }
    else
    {
        if (IsDataCurve(curve_index))
        {
            done = m_dataSelections[curve_index].GetCount() > 0;
            m_dataSelections[curve_index].Clear();
            m_curveSelections[curve_index].Clear();
            if (done)
                RedrawDataCurve(curve_index, 0, GetDataCurve(curve_index)->GetCount()-1);
        }
        else
        {
            done = m_curveSelections[curve_index].GetCount() > 0;
            m_curveSelections[curve_index].Clear();
            m_dataSelections[curve_index].Clear();
            if (done)
                RedrawCurve(curve_index, m_viewRect.m_x, m_viewRect.GetRight());
        }
    }

    if (send_event && done)
    {
        wxPlotCtrlSelEvent event( wxEVT_PLOTCTRL_RANGE_SEL_CHANGED, GetId(), this);
        event.SetCurve(GetCurve(curve_index), curve_index);

        if (IsDataCurve(curve_index))
            event.SetDataSelectionRange(wxRangeInt(0, GetDataCurve(curve_index)->GetCount()-1), false);
        else
            event.SetCurveSelectionRange(wxEmptyRangeDouble, false);

        (void)DoSendEvent( event );
    }
    return done;
}

// ------------------------------------------------------------------------
// Get/Set origin, size, and Zoom in/out of view, set scaling, size...
// ------------------------------------------------------------------------
/*

// FIXME - can't shift the bitmap due to off by one errors in ClipLineToRect

void wxPlotCtrl::ShiftOrigin( int dx, int dy, bool send_event )
{
    if ((dx == 0) && (dy == 0)) return;

    if (send_event)
    {
        wxPlotCtrlEvent event( wxEVT_PLOTCTRL_VIEW_CHANGING, GetId(), this);
        event.SetCurve(m_activeCurve, m_active_index);
        if (DoSendEvent(event)) return;
    }

    {
        wxBitmap tempBitmap(m_areaClientRect.width, m_areaClientRect.height);
        wxMemoryDC mdc;
        mdc.SelectObject(tempBitmap);
        mdc.DrawBitmap( m_area->m_bitmap, dx, dy, false );
        mdc.SelectObject(wxNullBitmap);
        m_area->m_bitmap = tempBitmap;
    }
    wxRect rx, ry;

    m_viewRect.m_x -= dx / m_zoom.m_x;
    m_viewRect.m_y += dy / m_zoom.m_y;

    if (dx != 0)
    {
        rx = wxRect((dx>0 ? -5 : m_areaClientRect.width+dx-5), 0, labs(dx)+10, m_areaClientRect.height);
        RedrawXAxis(false);
    }
    if (dy != 0)
    {
        ry = wxRect(0, (dy>0 ? -5 : m_areaClientRect.height+dy-5), m_areaClientRect.width, labs(dy)+10);
        RedrawYAxis(false);
    }

    printf("Shift %d %d rx %d %d %d %d, ry %d %d %d %d\n", dx, dy, rx.x, rx.y, rx.width, rx.height, ry.x, ry.y, ry.width, ry.height); fflush(stdout);

    if (rx.width > 0) m_area->CreateBitmap( rx );
        //m_area->Refresh(false, &rx);
    if (ry.height > 0) m_area->CreateBitmap( ry );
        //m_area->Refresh(false, &ry);

    {
        wxClientDC cdc(m_area);
        cdc.DrawBitmap(m_area->m_bitmap, 0, 0);
    }

    AdjustScrollBars();

    if (send_event)
    {
        wxPlotCtrlEvent event( wxEVT_PLOTCTRL_VIEW_CHANGED, GetId(), this);
        event.SetCurve(m_activeCurve, m_active_index);
        (void)DoSendEvent( event );
    }
}
*/

bool wxPlotCtrl::MakeCurveVisible(int curve_index, bool send_event)
{
    if (curve_index < 0)
        return SetZoom( -1, -1, 0, 0, send_event );

    wxCHECK_MSG(curve_index < GetCurveCount(), false, wxT("Invalid curve index"));
    wxPlotCurve *curve = GetCurve(curve_index);
    wxCHECK_MSG(curve && curve->Ok(), false, wxT("Invalid curve"));
    return SetViewRect(curve->GetBoundingRect(), send_event);
}

bool wxPlotCtrl::SetViewRect(const wxRect2DDouble &view, bool send_event)
{
    double zoom_x = m_areaClientRect.width/view.m_width;
    double zoom_y = m_areaClientRect.height/view.m_height;
    return SetZoom(zoom_x, zoom_y, view.m_x, view.m_y, send_event);
}

bool wxPlotCtrl::SetZoom( const wxPoint2DDouble &zoom, bool around_center, bool send_event )
{
    if (around_center && (zoom.m_x > 0) && (zoom.m_y > 0))
    {
        double origin_x = (m_viewRect.GetLeft() + m_viewRect.m_width/2.0);
        origin_x -= (m_viewRect.m_width/2.0)*m_zoom.m_x/zoom.m_x;
        double origin_y = (m_viewRect.GetTop() + m_viewRect.m_height/2.0);
        origin_y -= (m_viewRect.m_height/2.0)*m_zoom.m_y/zoom.m_y;
        return SetZoom( zoom.m_x, zoom.m_y, origin_x, origin_y, send_event );
    }
    else
        return SetZoom( zoom.m_x, zoom.m_y, m_viewRect.GetLeft(), m_viewRect.GetTop(), send_event );
}

bool wxPlotCtrl::SetZoom( const wxRect &window, bool send_event )
{
    if ((window.GetHeight()<1) || (window.GetWidth()<1)) return false;

    double origin_x = GetPlotCoordFromClientX(window.GetX());
    double origin_y = GetPlotCoordFromClientY(window.GetY()+window.GetHeight());
    double zoom_x = m_zoom.m_x * double(m_areaClientRect.width) /(window.GetWidth());
    double zoom_y = m_zoom.m_y * double(m_areaClientRect.height)/(window.GetHeight());

    bool ok = SetZoom( zoom_x, zoom_y, origin_x, origin_y, send_event );
    if (ok) AddHistoryView();
    return ok;
}

bool wxPlotCtrl::SetZoom( double zoom_x, double zoom_y,
                          double origin_x, double origin_y, bool send_event )
{
    // fit to window if zoom <= 0
    if (zoom_x <= 0)
    {
        zoom_x = double(m_areaClientRect.width)/(m_curveBoundingRect.m_width);
        origin_x = m_curveBoundingRect.m_x;
    }
    if (zoom_y <= 0)
    {
        zoom_y = double(m_areaClientRect.height)/(m_curveBoundingRect.m_height);
        origin_y = m_curveBoundingRect.m_y;
    }

    if (m_fix_aspectratio)
        FixAspectRatio( &zoom_x, &zoom_y, &origin_x, &origin_y );

    double view_width  = m_areaClientRect.width/zoom_x;
    double view_height = m_areaClientRect.height/zoom_y;

    if (!IsFinite(zoom_x, wxT("X zoom is NaN"))) return false;
    if (!IsFinite(zoom_y, wxT("Y zoom is NaN"))) return false;
    if (!IsFinite(origin_x, wxT("X origin is not finite"))) return false;
    if (!IsFinite(origin_y, wxT("Y origin is not finite"))) return false;
    if (!IsFinite(view_width, wxT("Plot width is NaN"))) return false;
    if (!IsFinite(view_height, wxT("Plot height is NaN"))) return false;

    bool x_changed = false, y_changed = false;

    if ((m_viewRect.m_x != origin_x) || (m_zoom.m_x != zoom_x))
        x_changed = true;
    if ((m_viewRect.m_y != origin_y) || (m_zoom.m_y != zoom_y))
        y_changed = true;

    bool bOK = true;
    if (x_changed || y_changed)
    {
        if (send_event)
        {
            wxPlotCtrlEvent event( wxEVT_PLOTCTRL_VIEW_CHANGING, GetId(), this);
            event.SetCurve(m_activeCurve, m_active_index);
            event.SetPosition(origin_x, origin_y);
            event.SetSize(view_width, view_height);
            bOK = DoSendEvent(event);
        }

        if(bOK)
        {
          m_zoom.m_x = zoom_x;
          m_zoom.m_y = zoom_y;

          m_viewRect.m_x = origin_x;
          m_viewRect.m_y = origin_y;
          m_viewRect.m_width  = view_width;
          m_viewRect.m_height = view_height;
        }
    }

    // redraw even if unchanged since we expect that it should be different
    Redraw(wxPLOTCTRL_REDRAW_PLOT | (x_changed?wxPLOTCTRL_REDRAW_XAXIS:0) |
                                    (y_changed?wxPLOTCTRL_REDRAW_YAXIS:0));

    if (!m_batch_count)
        AdjustScrollBars();

    if (send_event && (x_changed || y_changed))
    {
        wxPlotCtrlEvent event( wxEVT_PLOTCTRL_VIEW_CHANGED, GetId(), this);
        event.SetCurve(m_activeCurve, m_active_index);
        event.SetPosition(origin_x, origin_y);
        (void)DoSendEvent( event );
    }

    return bOK;
}

void wxPlotCtrl::SetFixAspectRatio(bool fix, double ratio)
{
    wxCHECK_RET(ratio > 0, wxT("Invalid aspect ratio"));
    m_fix_aspectratio = fix;
    m_aspectratio = ratio;
}

void wxPlotCtrl::FixAspectRatio( double *zoom_x, double *zoom_y, double *origin_x, double *origin_y )
{
    wxCHECK_RET(zoom_x && zoom_y && origin_x && origin_y, wxT("Invalid parameters"));

    //get the width and height of the view in plot coordinates
    double viewWidth = m_areaClientRect.width / (*zoom_x);
    double viewHeight = m_areaClientRect.height / (*zoom_y);

    //get the centre of the visible area in plot coordinates
    double xCentre = (*origin_x) + viewWidth / 2;
    double yCentre = (*origin_y) + viewHeight / 2;

    //if zoom in one direction is more than in the other, reduce both to the lower value
    if( (*zoom_x) * m_aspectratio > (*zoom_y) )
    {
        (*zoom_x) = (*zoom_y) * m_aspectratio;
        (*zoom_y) = (*zoom_y);
    }
    else
    {
        (*zoom_x) = (*zoom_x);
        (*zoom_y) = (*zoom_x) / m_aspectratio;
    }

    //update the plot coordinate view width and height based on the new zooms
    viewWidth = m_areaClientRect.width / (*zoom_x);
    viewHeight = m_areaClientRect.height / (*zoom_y);

    //create the new bottom-left corner of the view in plot coordinates
    *origin_x = xCentre - (viewWidth / 2);
    *origin_y = yCentre - (viewHeight / 2);
}

void wxPlotCtrl::SetDefaultBoundingRect( const wxRect2DDouble &rect, bool send_event )
{
    wxCHECK_RET(wxFinite(rect.m_x)&&wxFinite(rect.m_y)&&wxFinite(rect.GetRight())&&wxFinite(rect.GetBottom()), wxT("bounding rect is NaN"));
    wxCHECK_RET((rect.m_width > 0) && (rect.m_height > 0), wxT("Plot Size < 0"));
    m_defaultPlotRect = rect;
    CalcBoundingPlotRect();
    SetZoom( m_areaClientRect.width/rect.m_width,
             m_areaClientRect.height/rect.m_height,
             rect.m_x, rect.m_y, send_event);
}

void wxPlotCtrl::AddHistoryView()
{
    if (!(wxFinite(m_viewRect.GetLeft())&&wxFinite(m_viewRect.GetRight())&&wxFinite(m_viewRect.GetTop())&&wxFinite(m_viewRect.GetBottom()))) return;

    if ((m_history_views_index >= 0)
        && (m_history_views_index < int(m_historyViews.GetCount()))
        && WXRECT2DDOUBLE_EQUAL(m_viewRect, m_historyViews[m_history_views_index]))
            return;

    if (int(m_historyViews.GetCount()) >= MAX_PLOT_ZOOMS)
    {
        if (m_history_views_index < int(m_historyViews.GetCount())-1)
        {
            m_historyViews[m_history_views_index] = m_viewRect;
        }
        else
        {
            m_historyViews.RemoveAt(0);
            m_historyViews.Add(m_viewRect);
        }
    }
    else
    {
        m_historyViews.Add(m_viewRect);
        m_history_views_index++;
    }
}

void wxPlotCtrl::NextHistoryView(bool foward, bool send_event)
{
    int count = m_historyViews.GetCount();

    // try to set it to the "current" history view
    if ((m_history_views_index > -1) && (m_history_views_index < count))
    {
        if (!WXRECT2DDOUBLE_EQUAL(m_viewRect, m_historyViews[m_history_views_index]))
            SetViewRect(m_historyViews[m_history_views_index], send_event);
    }

    if (foward)
    {
        if ((count > 0) && (m_history_views_index < count - 1))
        {
            m_history_views_index++;
            SetViewRect(m_historyViews[m_history_views_index], send_event);
        }
    }
    else
    {
        if (m_history_views_index > 0)
        {
            m_history_views_index--;
            SetViewRect(m_historyViews[m_history_views_index], send_event);
        }
        else
            SetZoom(-1, -1, 0, 0, send_event);
    }
}

void wxPlotCtrl::SetAreaMouseFunction( wxPlotCtrlMouse_Type func, bool send_event )
{
    if (func == m_area_mouse_func) return;

    if (send_event)
    {
        wxPlotCtrlEvent event1(wxEVT_PLOTCTRL_MOUSE_FUNC_CHANGING, GetId(), this);
        event1.SetMouseFunction(func);
        if (!DoSendEvent(event1))
            return;
    }

    m_area_mouse_func = func;

    switch (func)
    {
        case wxPLOTCTRL_MOUSE_ZOOM :
        {
            SetAreaMouseCursor(wxCURSOR_MAGNIFIER); //wxCURSOR_CROSS);
            break;
        }
        case wxPLOTCTRL_MOUSE_SELECT   :
        case wxPLOTCTRL_MOUSE_DESELECT :
        {
            SetAreaMouseCursor(wxCURSOR_ARROW);
            break;
        }
        case wxPLOTCTRL_MOUSE_PAN :
        {
            SetAreaMouseCursor(wxCURSOR_HAND);
            SetAreaMouseMarker(wxPLOTCTRL_MARKER_NONE);
            break;
        }
        case wxPLOTCTRL_MOUSE_NOTHING :
        default :
        {
            SetAreaMouseCursor(wxCURSOR_CROSS);
            SetAreaMouseMarker(wxPLOTCTRL_MARKER_NONE);
            break;
        }
    }

    if (send_event)
    {
        wxPlotCtrlEvent event2(wxEVT_PLOTCTRL_MOUSE_FUNC_CHANGED, GetId(), this);
        event2.SetMouseFunction(func);
        (void)DoSendEvent(event2);
    }
}

void wxPlotCtrl::SetAreaMouseMarker( wxPlotCtrlMarker_Type type )
{
    if (type == m_area_mouse_marker)
        return;

    wxClientDC dc(m_area);
    DrawMouseMarker( &dc, m_area_mouse_marker, m_area->m_mouseRect );
    m_area_mouse_marker = type;
    DrawMouseMarker( &dc, m_area_mouse_marker, m_area->m_mouseRect );
}

void wxPlotCtrl::SetAreaMouseCursor(wxStockCursor cursorid)
    // DJH 9/3/15 changed parameter from int to wxStockCursor
{
    if (cursorid == m_area_mouse_cursorid)
        return;

    m_area_mouse_cursorid = cursorid;

    if (cursorid == wxCURSOR_HAND)
        m_area->SetCursor(s_handCursor);
    else if (cursorid == CURSOR_GRAB)
        m_area->SetCursor(s_grabCursor);
    else
        m_area->SetCursor(wxCursor(cursorid));
}

void wxPlotCtrl::OnSize( wxSizeEvent& )
{
    DoSize();
}

bool wxPlotCtrl::RenderScrollbars()
{
  return true;
}

int wxPlotCtrl::_GetYAxisTextWidth()
{
  if ( (!m_y_axis_text_width) && (m_yAxisDrawer != NULL) )
  {
    int x, y, d, leading;
    const wxFont &font(m_yAxisDrawer->GetTickFont());
    GetTextExtent(_GetYAxisExtentStr(), &x, &y, &d, &leading, &font);
    m_y_axis_text_width = x + leading;
  }
  return m_y_axis_text_width;
}

void wxPlotCtrl::DoSize(const wxRect &boundingRect, bool set_window_sizes)
{
    if (!m_area) return; // we're not created yet

    m_redraw_type = wxPLOTCTRL_REDRAW_BLOCKER;  // block OnPaints until done

    wxSize size;

    if(boundingRect == wxRect(0, 0, 0, 0))
    {
        UpdateWindowSize();
        size = GetClientSize();
    }
    else
    {
        size.x = boundingRect.width;
        size.y = boundingRect.height;
    }

    // wait until we have a normal size
    if ((size.x < 2) || (size.y < 2)) return;

    int sb_width = RenderScrollbars() ? m_yAxisScrollbar->GetSize().GetWidth() : 0;

    m_clientRect = wxRect(0, 0, size.x-sb_width, size.y-sb_width);

    // title and label positions, add padding here
    wxRect titleRect  = m_show_title  ? wxRect(m_titleRect).Inflate(m_border)  : wxRect(0,0,1,1);
    wxRect xLabelRect = m_show_xlabel ? wxRect(m_xLabelRect).Inflate(m_border) : wxRect(0,0,1,1);
    wxRect yLabelRect = m_show_ylabel ? wxRect(m_yLabelRect).Inflate(m_border) : wxRect(0,0,1,1);

    // this is the border around the area, it lets you see about 1 digit extra on axis
    int area_border = m_axisFontSize.y/2;

    // use the area_border between top of y-axis and area as bottom border of title
    if (m_show_title) titleRect.height -= m_border;

    int yaxis_width  = GetShowYAxis() ? _GetYAxisTextWidth() : 1;
    int xaxis_height = GetShowXAxis() ? m_axisFontSize.y    : area_border;

    int area_width  = m_clientRect.width  - yLabelRect.GetRight() - yaxis_width - 2*area_border;
    int area_height = m_clientRect.height - titleRect.GetBottom() - xaxis_height - xLabelRect.height - area_border;

    m_yAxisRect = wxRect(yLabelRect.GetRight(),
                         titleRect.GetBottom(),
                         yaxis_width,
                         area_height + 2*area_border);

    m_xAxisRect = wxRect(m_yAxisRect.GetRight(),
                         m_yAxisRect.GetBottom() - area_border + 1,
                         area_width + 2*area_border,
                         xaxis_height);

    m_areaRect = wxRect(m_yAxisRect.GetRight() + area_border,
                        m_yAxisRect.GetTop() + area_border,
                        area_width,
                        area_height);

    // scrollbar to right and bottom
    if (set_window_sizes)
    {
        if (sb_width)
        {
          m_yAxisScrollbar->SetSize(m_clientRect.width, 0, sb_width, m_clientRect.height);
          m_xAxisScrollbar->SetSize(0, m_clientRect.height, m_clientRect.width, sb_width);
        }
        m_yAxis->Show(GetShowYAxis());
        m_xAxis->Show(GetShowXAxis());
        if (GetShowYAxis()) m_yAxis->SetSize(m_yAxisRect);
        if (GetShowXAxis()) m_xAxis->SetSize(m_xAxisRect);

        m_area->SetSize(m_areaRect);
        UpdateWindowSize();
    }
    else
        m_areaClientRect = wxRect(wxPoint(0,0), m_areaRect.GetSize());

    m_titleRect.x = m_areaRect.x + ( m_areaRect.width - m_titleRect.GetWidth() ) / 2;
    //m_titleRect.x = m_clientRect.width/2-m_titleRect.GetWidth()/2; center on whole plot
    m_titleRect.y = m_border;

    m_xLabelRect.x = m_areaRect.x + m_areaRect.width/2 - m_xLabelRect.width/2;
    m_xLabelRect.y = m_xAxisRect.GetBottom() + m_border;

    m_yLabelRect.x = m_border;
    m_yLabelRect.y = m_areaRect.y + m_areaRect.height/2 - m_yLabelRect.height/2;

    double zoom_x = m_areaClientRect.width/m_viewRect.m_width;
    double zoom_y = m_areaClientRect.height/m_viewRect.m_height;
    if (!IsFinite(zoom_x, wxT("Plot zoom is NaN"))) return;
    if (!IsFinite(zoom_y, wxT("Plot zoom is NaN"))) return;

    if (m_fix_aspectratio)
    {
        FixAspectRatio( &zoom_x, &zoom_y, &m_viewRect.m_x, &m_viewRect.m_y );

        m_viewRect.m_width = m_areaClientRect.width/zoom_x;
        m_viewRect.m_height = m_areaClientRect.height/zoom_y;
    }

    m_zoom.m_x = zoom_x;
    m_zoom.m_y = zoom_y;

    wxPlotCtrlEvent event( wxEVT_PLOTCTRL_VIEW_CHANGED, GetId(), this);
    event.SetCurve(m_activeCurve, m_active_index);
    (void)DoSendEvent( event );

    m_redraw_type = 0;
    Redraw(wxPLOTCTRL_REDRAW_WHOLEPLOT);
}

void wxPlotCtrl::CalcBoundingPlotRect()
{
    int i, count = m_curves.GetCount();

    if (count > 0)
    {
        bool valid_rect = false;

        wxRect2DDouble rect = m_curves[0].GetBoundingRect();

        if ( IsFinite(rect.m_x, wxT("left curve boundary is NaN")) &&
             IsFinite(rect.m_y, wxT("bottom curve boundary is NaN")) &&
             IsFinite(rect.GetRight(), wxT("right curve boundary is NaN")) &&
             IsFinite(rect.GetBottom(), wxT("top curve boundary is NaN")) &&
             (rect.m_width >= 0) && (rect.m_height >= 0) )
        {
            valid_rect = true;
        }
        else
            rect = wxNullPlotBounds;

        for (i=1; i<count; i++)
        {
            wxRect2DDouble curveRect = m_curves[i].GetBoundingRect();

            if ((curveRect.m_width) <= 0 || (curveRect.m_height <= 0))
                continue;

            wxRect2DDouble newRect;
            if (!valid_rect)
                newRect = curveRect;
            else
                newRect = rect.CreateUnion(curveRect);

            if ( IsFinite(newRect.m_x, wxT("left curve boundary is NaN")) &&
                 IsFinite(newRect.m_y, wxT("bottom curve boundary is NaN")) &&
                 IsFinite(newRect.GetRight(), wxT("right curve boundary is NaN")) &&
                 IsFinite(newRect.GetBottom(), wxT("top curve boundary is NaN")) &&
                 (newRect.m_width >= 0) && (newRect.m_height >= 0))
            {
                if (!valid_rect) valid_rect = true;
                rect = newRect;
            }
        }

        // maybe just a single point, center it using default size
        bool zeroWidth = false, zeroHeight = false;

        if (rect.m_width == 0.0)
        {
            zeroWidth = true;
            rect.m_x = m_defaultPlotRect.m_x;
            rect.m_width = m_defaultPlotRect.m_width;
        }
        if (rect.m_height == 0.0)
        {
            zeroHeight = true;
            rect.m_y = m_defaultPlotRect.m_y;
            rect.m_height = m_defaultPlotRect.m_height;
        }

        m_curveBoundingRect = rect;

        // add some padding so the edge points can be seen
        double w = (!zeroWidth)  ? rect.m_width/50.0  : 0.0;
        double h = (!zeroHeight) ? rect.m_height/50.0 : 0.0;
        m_curveBoundingRect.Inset(-w, -h, -w, -h);
    }
    else
        m_curveBoundingRect = m_defaultPlotRect;

   AdjustScrollBars();
}

void wxPlotCtrl::Redraw(int type)
{
    if (m_batch_count) return;

    if (type & wxPLOTCTRL_REDRAW_XAXIS)
    {
        m_redraw_type |= wxPLOTCTRL_REDRAW_XAXIS;
        AutoCalcXAxisTicks();
        if (m_correct_ticks == true)
            CorrectXAxisTicks();
        CalcXAxisTickPositions();
    }
    if (type & wxPLOTCTRL_REDRAW_YAXIS)
    {
        m_redraw_type |= wxPLOTCTRL_REDRAW_YAXIS;
        AutoCalcYAxisTicks();
        if (m_correct_ticks == true)
            CorrectYAxisTicks();
        CalcYAxisTickPositions();
    }

    if (type & wxPLOTCTRL_REDRAW_PLOT)
    {
        m_redraw_type |= wxPLOTCTRL_REDRAW_PLOT;
        m_area->Refresh(false);
    }

    if (type & wxPLOTCTRL_REDRAW_XAXIS)
        m_xAxis->Refresh(false);
    if (type & wxPLOTCTRL_REDRAW_YAXIS)
        m_yAxis->Refresh(false);

    if (type & wxPLOTCTRL_REDRAW_WINDOW)
        Refresh();
}

void wxPlotCtrl::DrawAreaWindow( wxDC *dc, const wxRect &rect )
{
    wxCHECK_RET(dc, wxT("invalid dc"));

    // GTK doesn't like invalid parameters
    wxRect refreshRect = rect;
    wxRect clientRect(GetPlotAreaRect());
    refreshRect.Intersect(clientRect);

    if ((refreshRect.width == 0) || (refreshRect.height == 0)) return;

    dc->SetClippingRegion(refreshRect);

    dc->SetBrush( wxBrush(GetBackgroundColour(), wxSOLID) );
    dc->SetPen( wxPen(GetBorderColour(), m_area_border_width, wxSOLID) );
    dc->DrawRectangle(clientRect);

    DrawTickMarks( dc, refreshRect );
    DrawMarkers( dc, refreshRect );

    dc->DestroyClippingRegion();

    int i;
    wxPlotCurve *curve;
    wxPlotCurve *activeCurve = GetActiveCurve();
    for(i=0; i<GetCurveCount(); i++)
    {
        curve = GetCurve(i);

        if (curve != activeCurve)
        {
            if (wxDynamicCast(curve, wxPlotData))
                DrawDataCurve( dc, wxDynamicCast(curve, wxPlotData), i, refreshRect );
            else
                DrawCurve( dc, curve, i, refreshRect );
        }
    }
    // active curve is drawn on top
    if (activeCurve)
    {
        if (wxDynamicCast(activeCurve, wxPlotData))
            DrawDataCurve( dc, wxDynamicCast(activeCurve, wxPlotData), GetActiveIndex(), refreshRect );
        else
            DrawCurve( dc, activeCurve, GetActiveIndex(), refreshRect );
    }

    DrawCurveCursor( dc );
    DrawKey( dc );

    // refresh border
    dc->SetBrush( *wxTRANSPARENT_BRUSH );
    dc->SetPen( wxPen(GetBorderColour(), m_area_border_width, wxSOLID) );
    dc->DrawRectangle(clientRect);

    dc->SetPen( wxNullPen );
    dc->SetBrush( wxNullBrush );
}

void wxPlotCtrl::DrawMouseMarker( wxDC *dc, int type, const wxRect &rect )
{
    wxCHECK_RET(dc, wxT("invalid window"));

    if ((rect.width == 0) || (rect.height == 0))
        return;

    wxRasterOperationMode logical_fn = dc->GetLogicalFunction();
#ifdef __WXMAC__
    dc->SetLogicalFunction( wxXOR );
    dc->SetBrush( *wxTRANSPARENT_BRUSH );
    dc->SetPen(*wxThePenList->FindOrCreatePen(*wxWHITE, 1, wxPENSTYLE_SHORT_DASH));
#else
    dc->SetLogicalFunction( wxINVERT );
    dc->SetBrush( *wxTRANSPARENT_BRUSH );
    dc->SetPen(*wxThePenList->FindOrCreatePen(*wxBLACK, 1, wxDOT));
#endif
    switch (type)
    {
        case wxPLOTCTRL_MARKER_NONE : break;

        case wxPLOTCTRL_MARKER_RECT :
        {
            // rects are drawn to width and height - 1, doesn't line up w/ cursor, who cares?
            dc->DrawRectangle( rect.x, rect.y, rect.width, rect.height );
            break;
        }
        case wxPLOTCTRL_MARKER_VERT :
        {
            if (rect.width != 0)
            {
                int height = GetClientSize().y;
                dc->DrawLine(rect.x, 1, rect.x, height-2);
                dc->DrawLine(rect.GetRight()+1, 1, rect.GetRight()+1, height-2);
            }
            break;
        }
        case wxPLOTCTRL_MARKER_HORIZ :
        {
            if (rect.height != 0)
            {
                int width = GetClientSize().x;
                dc->DrawLine(1, rect.y, width-2, rect.y);
                dc->DrawLine(1, rect.GetBottom()+1, width-2, rect.GetBottom()+1);
            }
            break;
        }
        default : break;
    }

    dc->SetBrush( wxNullBrush );
    dc->SetPen( wxNullPen );
    dc->SetLogicalFunction( logical_fn );
}

void wxPlotCtrl::DrawCrosshairCursor( wxDC *dc, const wxPoint &pos )
{
    wxCHECK_RET(dc, wxT("invalid window"));

    dc->SetPen(*wxBLACK_PEN);
    wxRasterOperationMode logical_fn = dc->GetLogicalFunction();
    dc->SetLogicalFunction( wxINVERT );

    dc->CrossHair(pos.x, pos.y);

    dc->SetPen( wxNullPen );
    dc->SetLogicalFunction( logical_fn );
}

void wxPlotCtrl::DrawDataCurve( wxDC *dc, wxPlotData *curve, int curve_index, const wxRect &rect )
{
    wxCHECK_RET(dc && m_dataCurveDrawer && curve && curve->Ok(), wxT("invalid curve"));

    m_dataCurveDrawer->SetDCRect(rect);
    m_dataCurveDrawer->SetPlotViewRect(m_viewRect);
    m_dataCurveDrawer->Draw(dc, curve, curve_index);
}

void wxPlotCtrl::DrawCurve( wxDC *dc, wxPlotCurve *curve, int curve_index, const wxRect &rect )
{
    wxCHECK_RET(dc && m_curveDrawer && curve && curve->Ok(), wxT("invalid curve"));

    m_curveDrawer->SetDCRect(rect);
    m_curveDrawer->SetPlotViewRect(m_viewRect);
    m_curveDrawer->Draw(dc, curve, curve_index);
}

void wxPlotCtrl::RedrawDataCurve(int index, int min_index, int max_index)
{
    if (m_batch_count) return;

    wxCHECK_RET((index>=0)&&(index<(int)m_curves.GetCount()), wxT("invalid curve index"));

    wxPlotData *plotData = GetDataCurve(index);
    wxCHECK_RET(plotData, wxT("not a data curve"));

    int count = plotData->GetCount();
    wxCHECK_RET((min_index<=max_index)&&(min_index>=0)&&(max_index>=0)&&(min_index<count)&&(max_index<count), wxT("invalid data index"));

    wxRect rect(m_areaClientRect);
    int cursor_size = GetCursorSize();

    if (plotData->GetIsXOrdered())
    {
        double x = plotData->GetXValue(wxMax(min_index-1, 0));

        if (x > m_viewRect.GetRight())
            return;
        else if (x < m_viewRect.m_x)
            rect.x = 0;
        else
            rect.x = GetClientCoordFromPlotX(x) - cursor_size/2 - 1;

        x = plotData->GetXValue(wxMin(max_index+1, (int)plotData->GetCount()-1));

        if (x < m_viewRect.m_x)
            return;
        else if (x > m_viewRect.GetRight())
            rect.SetRight(m_areaClientRect.width);
        else
            rect.SetRight(GetClientCoordFromPlotX(x) + cursor_size/2 + 1);

        rect.Intersect(m_areaClientRect);
    }

    wxMemoryDC mdc;
    mdc.SelectObject( m_area->m_bitmap );
    DrawDataCurve( &mdc, plotData, index, rect );
    DrawCurveCursor(&mdc);
    wxClientDC dc(m_area);
    dc.Blit(rect.x, rect.y, rect.width, rect.height, &mdc, rect.x, rect.y);
    mdc.SelectObject( wxNullBitmap );
}

void wxPlotCtrl::RedrawCurve(int index, double min_x, double max_x)
{
    if (m_batch_count) return;

    wxCHECK_RET((min_x<=max_x)&&(index>=0)&&(index<(int)m_curves.GetCount()), wxT("invalid curve index"));
    wxCHECK_RET(!GetDataCurve(index), wxT("invalid curve"));
    wxRect rect(m_areaClientRect);

    if (min_x > m_viewRect.GetRight())
        return;
    if (min_x < m_viewRect.m_x)
        min_x = m_viewRect.m_x;

    rect.x = GetClientCoordFromPlotX(min_x);

    if (max_x < m_viewRect.m_x)
        return;
    if (max_x > m_viewRect.GetRight())
        max_x = m_viewRect.GetRight();

    rect.width = GetClientCoordFromPlotX(max_x) - rect.x;

    if (rect.width < 1) return;

    wxMemoryDC mdc;
    mdc.SelectObject( m_area->m_bitmap );
    DrawCurve(&mdc, GetCurve(index), index, rect);
    DrawCurveCursor(&mdc);
    wxClientDC dc(m_area);
    dc.Blit(rect.x, rect.y, rect.width, rect.height, &mdc, rect.x, rect.y);
    mdc.SelectObject( wxNullBitmap );
}

void wxPlotCtrl::DrawKey( wxDC *dc )
{
    wxCHECK_RET(dc && m_keyDrawer, wxT("invalid window"));
    if (!GetShowKey() || m_keyString.IsEmpty())
        return;

    wxRect dcRect(wxPoint(0, 0), GetPlotAreaRect().GetSize());
    m_keyDrawer->SetDCRect(dcRect);
    m_keyDrawer->SetPlotViewRect(m_viewRect);
    m_keyDrawer->Draw(dc, m_keyString);
}

void wxPlotCtrl::DrawCurveCursor( wxDC *dc )
{
    wxCHECK_RET(dc, wxT("invalid window"));
    if (!IsCursorValid()) return;

    m_markerDrawer->SetPlotViewRect(m_viewRect);
    m_markerDrawer->SetDCRect(wxRect(wxPoint(0,0), m_area->GetClientSize()));
    m_markerDrawer->Draw(dc, m_cursorMarker);
}

void wxPlotCtrl::DrawTickMarks( wxDC *dc, const wxRect& rect )
{
    wxRect clientRect(GetPlotAreaRect());
    // DJH 2/19/09 change width to m_area_border_width
    dc->SetPen( wxPen(GetGridColour(), m_area_border_width, wxSOLID) ); 

    int xtick_length = GetDrawGrid() ? clientRect.height : 10;
    int ytick_length = GetDrawGrid() ? clientRect.width  : 10;

    int tick_pos, i;
    // X-axis ticks
    int tick_count = m_xAxisTicks.GetCount();
    for (i=0; i<tick_count; i++)
    {
        tick_pos = m_xAxisTicks[i];
        if (tick_pos < rect.x)
            continue;
        else if (tick_pos > rect.GetRight())
            break;

        dc->DrawLine(tick_pos, clientRect.height, tick_pos, clientRect.height - xtick_length);
    }

    // Y-axis ticks
    tick_count = m_yAxisTicks.GetCount();
    for (i=0; i<tick_count; i++)
    {
        tick_pos = m_yAxisTicks[i];
        if (tick_pos < rect.y)
            break;
        else if (tick_pos > rect.GetBottom())
            continue;

        dc->DrawLine(0, tick_pos, ytick_length, tick_pos);
    }
}

void wxPlotCtrl::DrawMarkers( wxDC *dc, const wxRect& rect )
{
    wxCHECK_RET(m_markerDrawer, wxT("Invalid marker drawer"));
    m_markerDrawer->SetPlotViewRect(m_viewRect);
    m_markerDrawer->SetDCRect(rect);
    m_markerDrawer->Draw(dc, m_plotMarkers);
}

void wxPlotCtrl::DrawXAxis( wxDC *dc, bool refresh )
{
    wxCHECK_RET(m_xAxisDrawer, wxT("Invalid x axis drawer"));

    m_xAxisDrawer->SetTickPositions( m_xAxisTicks );
    m_xAxisDrawer->SetTickLabels( m_xAxisTickLabels );
    m_xAxisDrawer->SetPlotViewRect(m_viewRect);
    wxSize clientSize = m_xAxisRect.GetSize();
    m_xAxisDrawer->SetDCRect(wxRect(wxPoint(0,0),clientSize));
    m_xAxisDrawer->Draw(dc, refresh);
}

void wxPlotCtrl::DrawYAxis( wxDC *dc, bool refresh )
{
    wxCHECK_RET(m_yAxisDrawer, wxT("Invalid y axis drawer"));

    m_yAxisDrawer->SetTickPositions( m_yAxisTicks );
    m_yAxisDrawer->SetTickLabels( m_yAxisTickLabels );
    m_yAxisDrawer->SetPlotViewRect(m_viewRect);
    wxSize clientSize = m_yAxisRect.GetSize();
    m_yAxisDrawer->SetDCRect(wxRect(wxPoint(0,0),clientSize));
    m_yAxisDrawer->Draw(dc, refresh);
}

wxRect ScaleRect(const wxRect& rect, double x_scale, double y_scale)
{
    return wxRect( int(rect.x*x_scale+0.5),     int(rect.y*y_scale+0.5),
                   int(rect.width*x_scale+0.5), int(rect.height*y_scale+0.5) );
}

void wxPlotCtrl::DrawWholePlot( wxDC *dc, const wxRect &boundingRect, double dpi, bool bAutoCalcTicks )
{
    wxCHECK_RET(dc, wxT("invalid dc"));
    wxCHECK_RET(dpi > 0, wxT("Invalid dpi for plot drawing"));

    bool bPrinting = (dpi >= 150);
    //set font scale so 1pt = 1pixel at 72dpi
    double fontScale = (double)dpi / (bPrinting ? 96.0 : 72.0);
    //one pixel wide line equals (m_pen_print_width) millimeters wide
    double penScale = (double)m_pen_print_width * dpi / 25.4;

    //save old values
    wxFont oldAxisFont      = GetAxisFont();
    wxFont oldAxisLabelFont = GetAxisLabelFont();
    wxFont oldPlotTitleFont = GetPlotTitleFont();
    wxFont oldKeyFont       = GetKeyFont();

    // DJH 2/19/09 -- more old values

    double oldCurveDrawerScale = 0.0;
    double oldDataCurveDrawerScale = 0.0;
    double oldMarkerDrawerScale = 0.0;

    if(bPrinting)
    {
      oldCurveDrawerScale = m_curveDrawer->GetPenScale();
      oldDataCurveDrawerScale = m_dataCurveDrawer->GetPenScale();
      oldMarkerDrawerScale = m_markerDrawer->GetPenScale();
      m_curveDrawer->SetPenScale(fontScale);
      m_dataCurveDrawer->SetPenScale(fontScale);
      m_markerDrawer->SetPenScale(fontScale);
    }

    // END DJH 2/19/09 -- more old values

    int old_area_border_width = m_area_border_width;
    int old_border = m_border;
    int old_cursor_size = m_cursorMarker.GetSize().x;
    wxPoint2DDouble old_zoom = m_zoom;
    wxRect2DDouble  old_view = m_viewRect;
    wxRect old_areaClientRect = m_areaClientRect;

    //resize border and border pen
    m_area_border_width = RINT(m_area_border_width * penScale);
    m_border = RINT(m_border * penScale);

    //resize the curve cursor
    m_cursorMarker.SetSize(wxSize(int(old_cursor_size * penScale), int(old_cursor_size * penScale)));

    //resize the fonts
    wxFont axisFont = GetAxisFont();
    axisFont.SetPointSize( wxMax(2, RINT(axisFont.GetPointSize() * fontScale)) );
    SetAxisFont( axisFont );

    wxFont axisLabelFont = GetAxisLabelFont();
    axisLabelFont.SetPointSize( wxMax(2, RINT(axisLabelFont.GetPointSize() * fontScale)) );
    SetAxisLabelFont( axisLabelFont );

    wxFont plotTitleFont = GetPlotTitleFont();
    plotTitleFont.SetPointSize( wxMax(2, RINT(plotTitleFont.GetPointSize() * fontScale)) );
    SetPlotTitleFont( plotTitleFont );

    wxFont keyFont = GetKeyFont();
    keyFont.SetPointSize( wxMax(2, RINT(keyFont.GetPointSize() * fontScale)) );
    SetKeyFont( keyFont );

    //reload the original zoom and view rect in case it was changed by any of the font changes
    m_zoom     = old_zoom;
    m_viewRect = old_view;

    //resize all window component rects to the bounding rect
    DoSize( boundingRect, false );
    //AutoCalcTicks();  // don't reset ticks since it might not be WYSIWYG

    //
    //  DJH - 2/24/2009  added parameter to determine whether AutoCalcTicks()
    //    should be called because the exported plots looked bad
    //
    if(bAutoCalcTicks) { AutoCalcTicks(); }

    //reload the original zoom and view rect in case it was changed by any of the font changes
    m_zoom = wxPoint2DDouble(old_zoom.m_x * double(m_areaClientRect.width)/old_areaClientRect.width,
                             old_zoom.m_y * double(m_areaClientRect.height)/old_areaClientRect.height);

//    wxPrintf(wxT("DPI %g, font %g pen%g\n"), dpi, fontScale, penScale);
//    DJH 09/03/2015 - wxPrintf is deprecated
    puts(
      wxString::Format(wxT("DPI %g, font %g pen%g\n"), dpi, fontScale, penScale).ToUTF8()
      );
    PRINT_WXRECT(wxT("Whole plot"), boundingRect);
    PRINT_WXRECT(wxT("Area plot"), m_areaRect);
    PRINT_WXRECT(wxT("Xaxis plot"), m_xAxisRect);
    PRINT_WXRECT(wxT("Yaxis plot"), m_yAxisRect);

    //draw all components to the provided dc
    dc->SetDeviceOrigin(long(boundingRect.x+m_xAxisRect.GetLeft()),
                        long(boundingRect.y+m_xAxisRect.GetTop()));
    CalcXAxisTickPositions();
    DrawXAxis(dc, false);

    dc->SetDeviceOrigin(long(boundingRect.x+m_yAxisRect.GetLeft()),
                        long(boundingRect.y+m_yAxisRect.GetTop()));
    CalcYAxisTickPositions();
    DrawYAxis(dc, false);

    dc->SetDeviceOrigin(long(boundingRect.x+m_areaRect.GetLeft()),
                        long(boundingRect.y+m_areaRect.GetTop()));
    DrawAreaWindow(dc, m_areaClientRect);

    dc->SetDeviceOrigin(boundingRect.x, boundingRect.y);
    DrawPlotCtrl(dc);

    dc->SetBrush(*wxTRANSPARENT_BRUSH);
    dc->SetPen(*wxRED_PEN);
    dc->SetDeviceOrigin(boundingRect.x, boundingRect.y);

#if 0
    //  DJH 2/19/2009 - removed red rectangles, 
    //  what was the original author thinking

    dc->DrawRectangle(m_xAxisRect);
    dc->DrawRectangle(m_yAxisRect);
    dc->DrawRectangle(m_areaRect);
#endif


    //restore old values
    m_area_border_width = old_area_border_width;
    m_border = old_border;
    m_cursorMarker.SetSize(wxSize(old_cursor_size, old_cursor_size));


    SetAxisFont( oldAxisFont );
    SetAxisLabelFont( oldAxisLabelFont );
    SetPlotTitleFont( oldPlotTitleFont );
    SetKeyFont( oldKeyFont );
    m_zoom     = old_zoom;
    m_viewRect = old_view;

    // DJH 2/19/09 restore more old values

    if(bPrinting)
    {
      m_curveDrawer->SetPenScale(oldCurveDrawerScale);
      m_dataCurveDrawer->SetPenScale(oldDataCurveDrawerScale);
      m_markerDrawer->SetPenScale(oldMarkerDrawerScale);
    }

    // END DJH 2/19/09 restore more old values

    //update to window instead of printer
    UpdateWindowSize();
    Redraw(wxPLOTCTRL_REDRAW_WHOLEPLOT); // recalc ticks for this window
}

// ----------------------------------------------------------------------------
// Axis tick calculations
// ----------------------------------------------------------------------------

void wxPlotCtrl::DoAutoCalcTicks(bool x_axis)
{
    double start = 0.0, end = 1.0;
    int i, n, window = 100;

    double   *tick_step = NULL;
    int      *tick_count = NULL;
    wxString *tickFormat = NULL;

    if (x_axis)
    {
        tick_step  = &m_xAxisTick_step;
        tick_count = &m_xAxisTick_count;
        tickFormat = &m_xAxisTickFormat;

        window = GetPlotAreaRect().width;
        m_xAxisTicks.Clear(); // kill it in case something goes wrong
        start = m_viewRect.GetLeft();
        end   = m_viewRect.GetRight();
        *tick_count = window/(m_axisFontSize.x*10);
    }
    else
    {
        tick_step  = &m_yAxisTick_step;
        tick_count = &m_yAxisTick_count;
        tickFormat = &m_yAxisTickFormat;

        window = GetPlotAreaRect().height;
        m_yAxisTicks.Clear();
        start = m_viewRect.GetTop();
        end   = m_viewRect.GetBottom();
        double tick_count_scale = window/(m_axisFontSize.y*2.0) > 2.0 ? 2.0 : 1.5;
        *tick_count = int(window/(m_axisFontSize.y*tick_count_scale) + 0.5);
    }

    if (window < 5) return; // FIXME

    if (!IsFinite(start, wxT("axis range is not finite")) ||
        !IsFinite(end, wxT("axis range is not finite")) )
    {
        *tick_count = 0;
        return;
    }

    double range = end - start;
    double max = fabs(start) > fabs(end) ? fabs(start) : fabs(end);
    double min = fabs(start) < fabs(end) ? fabs(start) : fabs(end);
    bool exponential = (min >= m_min_exponential) || (max < 1.0/m_min_exponential) ? true : false;
    int places = exponential ? 1 : int(floor(fabs(log10(max))));

    if (!IsFinite(range, wxT("axis range is not finite")) ||
        !IsFinite(min,   wxT("axis range is not finite")) ||
        !IsFinite(max,   wxT("axis range is not finite")) )
    {
        *tick_count = 0;
        return;
    }

    *tick_step = 1.0;
    int int_log_range = int( log10(range) );
    if      (int_log_range > 0) { for (i=0; i <  int_log_range; i++) (*tick_step) *= 10; }
    else if (int_log_range < 0) { for (i=0; i < -int_log_range; i++) (*tick_step) /= 10; }

    double stepsizes[TIC_STEPS] = { .1, .2, .5 };
    double step10 = (*tick_step) / 10.0;
    int sigFigs = 0;
    int digits = 0;

    for (n=0; n<4; n++)
    {
        for (i=0; i<TIC_STEPS; i++)
        {
            *tick_step = step10 * stepsizes[i];

            if (exponential)
                sigFigs = labs(int(log10(max)) - int(log10(*tick_step)));
            else
                sigFigs = (*tick_step) >= 1.0 ? 0 : int(ceil(-log10(*tick_step)));

            if (x_axis)
            {
                digits = 1 + places + (sigFigs > 0 ? 1+sigFigs : 0) + (exponential ? 4 : 0);
                *tick_count = int(double(window)/double((digits+3)*m_axisFontSize.x) + 0.5);
            }

            if ((range/(*tick_step)) <= (*tick_count)) break;
        }
        if ((range/(*tick_step)) <= (*tick_count)) break;
        step10 *= 10.0;
    }

    //if (!x_axis) wxPrintf(wxT("Ticks %d %lf, %d\n"), n, *tick_step, *tick_count);

    if (sigFigs > 9) sigFigs = 9; // FIXME

    if (exponential) tickFormat->Printf( wxT("%%.%dle"), sigFigs );
    else             tickFormat->Printf( wxT("%%.%dlf"), sigFigs );

    *tick_count = int(ceil(range/(*tick_step))) + 1;

//  note : first_tick = ceil(start / tick_step) * tick_step;
}

void wxPlotCtrl::CorrectXAxisTicks()
{
    double start = ceil(m_viewRect.GetLeft() / m_xAxisTick_step) * m_xAxisTick_step;
    wxString label;
    label.Printf( m_xAxisTickFormat.wc_str(), start);
    if (label.ToDouble( &start ))
    {
        double x = GetClientCoordFromPlotX( start );
        double zoom_x = (GetClientCoordFromPlotX(start+m_xAxisTick_step)-x)/m_xAxisTick_step;
        double origin_x = start - x/zoom_x;
        BeginBatch();
        if (!SetZoom( zoom_x, m_zoom.m_y, origin_x, m_viewRect.GetTop(), true ))
            m_xAxisTick_count = 0;  // oops

        EndBatch(false); // don't draw just block
    }
}
void wxPlotCtrl::CorrectYAxisTicks()
{
    double start = ceil(m_viewRect.GetTop() / m_yAxisTick_step) * m_yAxisTick_step;
    wxString label;
    label.Printf( m_yAxisTickFormat.wc_str(), start);
    if (label.ToDouble( &start ))
    {
        double y = GetClientCoordFromPlotY( start );
        double zoom_y = (y-GetClientCoordFromPlotY(start+m_yAxisTick_step))/m_yAxisTick_step;
        double origin_y = start - (GetPlotAreaRect().height - y)/zoom_y;
        BeginBatch();
        if (!SetZoom( m_zoom.m_x, zoom_y, m_viewRect.GetLeft(), origin_y, true ))
            m_yAxisTick_count = 0;  // oops

        EndBatch(false);
    }
}

wxString wxPlotCtrl::_FormatTickLabel(const wxString &sFormat, double d)
{
  wxString sRtn = wxString::Format(sFormat, d == 0.0 ? 0.0 : d); // avoid -0 on plot
  // now check for -0
  const wxChar *p = sRtn;
  if(*p == wxT('-'))
  {
    bool bFix = true;
    wxChar c0('0'), c9('9');
    for(++p; bFix && *p; ++p)
    {
      if ((*p) > c0 && (*p) <= c9)
      {
        bFix = false;
      }
    }
    if (bFix)
    {
      sRtn.Remove(0);
    }
  }
  return sRtn;
}

void wxPlotCtrl::CalcXAxisTickPositions()
{
    double current = ceil(m_viewRect.GetLeft() / m_xAxisTick_step) * m_xAxisTick_step;
    m_xAxisTicks.Clear();
    m_xAxisTickLabels.Clear();
    int i, x, windowWidth = GetPlotAreaRect().width;
    for (i=0; i<m_xAxisTick_count; i++)
    {
        if (!IsFinite(current, wxT("axis label is not finite"))) return;

        x = GetClientCoordFromPlotX( current );

        if ((x >= -1) && (x < windowWidth+2))
        {
            m_xAxisTicks.Add(x);
            m_xAxisTickLabels.Add(_FormatTickLabel(m_xAxisTickFormat, current));
        }

        current += m_xAxisTick_step;
    }
}
void wxPlotCtrl::CalcYAxisTickPositions()
{
    double current = ceil(m_viewRect.GetTop() / m_yAxisTick_step) * m_yAxisTick_step;
    m_yAxisTicks.Clear();
    m_yAxisTickLabels.Clear();
    int i, y, windowWidth = GetPlotAreaRect().height;
    for (i=0; i<m_yAxisTick_count; i++)
    {
        if (!IsFinite(current, wxT("axis label is not finite")))
            return;

        y = GetClientCoordFromPlotY( current );

        if ((y >= -1) && (y < windowWidth+2))
        {
            m_yAxisTicks.Add(y);
            m_yAxisTickLabels.Add(_FormatTickLabel(m_yAxisTickFormat, current));
        }

        current += m_yAxisTick_step;
    }
}

// ----------------------------------------------------------------------------
// Event processing
// ----------------------------------------------------------------------------

void wxPlotCtrl::ProcessAreaEVT_MOUSE_EVENTS( wxMouseEvent &event )
{
    wxPoint& m_mousePt   = m_area->m_mousePt;
    wxRect&  m_mouseRect = m_area->m_mouseRect;

    wxPoint lastMousePt = m_mousePt;
    m_mousePt = event.GetPosition();

    if (event.ButtonDown() && IsTextCtrlShown())
    {
        HideTextCtrl(true, true);
        return;
    }

    if (GetGreedyFocus() && (FindFocus() != m_area))
        m_area->SetFocus();

    double plotX = GetPlotCoordFromClientX(m_mousePt.x),
           plotY = GetPlotCoordFromClientY(m_mousePt.y);

    wxClientDC dc(m_area);

    // Mouse motion
    if (lastMousePt != m_area->m_mousePt)
    {
        wxPlotCtrlEvent evt_motion(wxEVT_PLOTCTRL_MOUSE_MOTION, GetId(), this);
        evt_motion.SetPosition( plotX, plotY );
        (void)DoSendEvent( evt_motion );

        // Draw the crosshair cursor
        if (GetCrossHairCursor())
        {
            if (!event.Entering() || m_area->HasCapture())
                DrawCrosshairCursor( &dc, lastMousePt );
            if (!event.Leaving() || m_area->HasCapture())
                DrawCrosshairCursor( &dc, m_mousePt );
        }
    }

    // Wheel scrolling up and down
    if (event.GetWheelRotation() != 0)
    {
        double dir = event.GetWheelRotation() > 0 ? 0.25 : -0.25;
        SetOrigin( m_viewRect.GetLeft(),
                   m_viewRect.GetTop() + dir*m_viewRect.m_height, true);
    }

    int active_index = GetActiveIndex();

    // Initial Left down selection
    if (event.LeftDown() || event.LeftDClick())
    {
        if (FindFocus() != m_area) // fixme MSW focus problems
            m_area->SetFocus();

        if (m_area_mouse_cursorid == wxCURSOR_HAND)
            SetAreaMouseCursor(CURSOR_GRAB);

        // send a click or doubleclick event
        wxPlotCtrlEvent click_event( event.ButtonDClick() ? wxEVT_PLOTCTRL_DOUBLECLICKED : wxEVT_PLOTCTRL_CLICKED,
                                 GetId(), this );
        click_event.SetPosition( plotX, plotY );
        (void)DoSendEvent( click_event );

        if (!event.ButtonDClick())
            m_mouseRect = wxRect(m_mousePt, wxSize(0, 0));

        int data_index = -1;
        int curve_index = -1;

        wxPoint2DDouble dpt(2.0/m_zoom.m_x, 2.0/m_zoom.m_y);
        wxPoint2DDouble curvePt;

        if (FindCurve(wxPoint2DDouble(plotX, plotY), dpt, curve_index, data_index, &curvePt))
        {
            wxPlotCurve *plotCurve = GetCurve(curve_index);
            wxPlotData  *plotData  = wxDynamicCast(plotCurve, wxPlotData);

            if (plotCurve)
            {
                wxPlotCtrlEvent pt_click_event( event.ButtonDClick() ? wxEVT_PLOTCTRL_POINT_DOUBLECLICKED : wxEVT_PLOTCTRL_POINT_CLICKED,
                                            GetId(), this );
                pt_click_event.SetPosition( curvePt.m_x, curvePt.m_y );
                pt_click_event.SetCurve( plotCurve, curve_index );
                pt_click_event.SetCurveDataIndex(data_index);
                (void)DoSendEvent( pt_click_event );

                // send curve selection switched event
                if (curve_index != GetActiveIndex())
                    SetActiveIndex( curve_index, true );

                if (!event.LeftDClick() && (m_area_mouse_func == wxPLOTCTRL_MOUSE_SELECT))
                {
                    if (plotData)
                        SelectDataRange(curve_index, wxRangeInt(data_index,data_index), true);
                    else
                        SelectXRange(curve_index, wxRangeDouble(curvePt.m_x,curvePt.m_x), true);
                }
                else if (!event.LeftDClick() && (m_area_mouse_func == wxPLOTCTRL_MOUSE_DESELECT))
                {
                    if (plotData)
                        DeselectDataRange(curve_index, wxRangeInt(data_index,data_index), true);
                    else
                        DeselectXRange(curve_index, wxRangeDouble(curvePt.m_x,curvePt.m_x), true);
                }
                else
                {
                    if (plotData)
                        SetCursorDataIndex(curve_index, data_index, true);
                    else
                        SetCursorXPoint(curve_index, curvePt.m_x, true);
                }
                return;
            }
        }
    }
    // Finished marking rectangle or scrolling, perhaps
    else if (event.LeftUp())
    {
        SetCaptureWindow(NULL);

        if (m_area_mouse_cursorid == CURSOR_GRAB)
            SetAreaMouseCursor(wxCURSOR_HAND);

        StopMouseTimer();

        if (m_mouseRect == wxRect(0,0,0,0))
            return;

        wxRect rightedRect = m_mouseRect;

        // rightedRect always goes from upper-left to lower-right
        //   don't fix m_mouseRect since redrawing will be off
        if (rightedRect.width < 0)
        {
            rightedRect.x += rightedRect.width;
            rightedRect.width = -rightedRect.width;
        }
        if (rightedRect.height < 0)
        {
            rightedRect.y += rightedRect.height;
            rightedRect.height = -rightedRect.height;
        }

        // Zoom into image
        if (m_area_mouse_func == wxPLOTCTRL_MOUSE_ZOOM)
        {
            if ((m_area_mouse_marker == wxPLOTCTRL_MARKER_RECT) &&
                ((rightedRect.width > 10) && (rightedRect.height > 10)))
                SetZoom( rightedRect, true );
            else if ((m_area_mouse_marker == wxPLOTCTRL_MARKER_VERT) &&
                     (rightedRect.width > 10) )
                SetZoom( wxRect(rightedRect.x, 0, rightedRect.width, m_areaClientRect.height), true );
            else if ((m_area_mouse_marker == wxPLOTCTRL_MARKER_HORIZ) &&
                     (rightedRect.height > 10) )
                SetZoom( wxRect(0, rightedRect.y, m_areaClientRect.width, rightedRect.height ), true );
            else
                DrawMouseMarker( &dc, m_area_mouse_marker, m_mouseRect );
        }
        // Select a range of points
        else if ((m_area_mouse_func == wxPLOTCTRL_MOUSE_SELECT) && (active_index >= 0))
        {
            BeginBatch(); // if you select nothing, you don't get a refresh

            wxRect2DDouble plotRect = GetPlotRectFromClientRect(rightedRect);

            if ((m_area_mouse_marker == wxPLOTCTRL_MARKER_VERT) && (plotRect.m_width > 0))
                SelectXRange(active_index, wxRangeDouble(plotRect.m_x, plotRect.GetRight()), true );
            else if ((m_area_mouse_marker == wxPLOTCTRL_MARKER_HORIZ) && (plotRect.m_height > 0))
                SelectYRange(active_index, wxRangeDouble(plotRect.m_y, plotRect.GetBottom()), true );
            else if ((plotRect.m_width > 0) || (plotRect.m_height > 0))
                SelectRectangle(active_index, plotRect, true );

            m_mouseRect = wxRect(0,0,0,0);
            EndBatch();
        }
        // Deselect a range of points
        else if ((m_area_mouse_func == wxPLOTCTRL_MOUSE_DESELECT) && (active_index >= 0))
        {
            BeginBatch();

            wxRect2DDouble plotRect = GetPlotRectFromClientRect(rightedRect);

            if ((m_area_mouse_marker == wxPLOTCTRL_MARKER_VERT) && (plotRect.m_width > 0))
                DeselectXRange(active_index, wxRangeDouble(plotRect.m_x, plotRect.GetRight()), true );
            else if ((m_area_mouse_marker == wxPLOTCTRL_MARKER_HORIZ) && (plotRect.m_height > 0))
                DeselectYRange(active_index, wxRangeDouble(plotRect.m_y, plotRect.GetBottom()), true );
            else if ((plotRect.m_width > 0) || (plotRect.m_height > 0))
                DeselectRectangle(active_index, plotRect, true );

            m_mouseRect = wxRect(0,0,0,0);
            EndBatch();
        }
        // Nothing to do - erase the rect
        else
        {
            DrawMouseMarker(&dc, m_area_mouse_marker, m_mouseRect);
        }

        m_mouseRect = wxRect(0,0,0,0);
        return;
    }
    // Marking the rectangle or panning around
    else if ( event.LeftIsDown() && event.Dragging() )
    {
        SetCaptureWindow(m_area);

        if (m_area_mouse_cursorid == wxCURSOR_HAND)
            SetAreaMouseCursor(CURSOR_GRAB);

        // Move the origin
        if (m_area_mouse_func == wxPLOTCTRL_MOUSE_PAN)
        {
            if (!m_areaClientRect.Contains(event.GetPosition()))
            {
                StartMouseTimer(ID_AREA_TIMER);
            }

            m_mouseRect = wxRect(0,0,0,0); // no marker

            double dx = m_mousePt.x - lastMousePt.x;
            double dy = m_mousePt.y - lastMousePt.y;
            SetOrigin(m_viewRect.GetLeft() - dx/m_zoom.m_x,
                      m_viewRect.GetTop()  + dy/m_zoom.m_y, true );
            return;
        }
        else
        {
            if (m_mouseRect != wxRect(0,0,0,0))
                DrawMouseMarker(&dc, m_area_mouse_marker, m_mouseRect);
            else
                m_mouseRect = wxRect(m_mousePt, wxSize(1, 1));

            m_mouseRect.width  = m_mousePt.x - m_mouseRect.x;
            m_mouseRect.height = m_mousePt.y - m_mouseRect.y;

            DrawMouseMarker(&dc, m_area_mouse_marker, m_mouseRect);
        }

        return;
    }
    return;
}

void wxPlotCtrl::ProcessAxisEVT_MOUSE_EVENTS( wxMouseEvent &event )
{
    if (event.ButtonDown() && IsTextCtrlShown())
    {
        HideTextCtrl(true, true);
        return;
    }

    wxPoint pos = event.GetPosition();
    wxPlotCtrlAxis *axisWin = (wxPlotCtrlAxis*)event.GetEventObject();
    wxCHECK_RET(axisWin, wxT("Unknown window"));

    wxPoint& m_mousePt = axisWin->m_mousePt;

    if (event.LeftIsDown() && (axisWin != GetCaptureWindow()))
    {
        SetCaptureWindow(axisWin);
        m_mousePt = pos;
        return;
    }
    else if (!event.LeftIsDown())
    {
        SetCaptureWindow(NULL);
        StopMouseTimer();
    }
    else if (event.LeftIsDown())
    {
        wxSize winSize = axisWin->GetSize();

        if (( axisWin->IsXAxis() && ((pos.x < 0) || (pos.x > winSize.x))) ||
            (!axisWin->IsXAxis() && ((pos.y < 0) || (pos.y > winSize.y))) )
        {
            m_mousePt = pos;
            StartMouseTimer(axisWin->IsXAxis() ? ID_XAXIS_TIMER : ID_YAXIS_TIMER);
        }
        else if (IsTimerRunning())
            m_mousePt = pos;
    }

    int wheel = event.GetWheelRotation();

    if (wheel != 0)
    {
        wheel = wheel > 0 ? 1 : wheel < 0 ? -1 : 0;
        double dx = 0, dy = 0;

        if (axisWin->IsXAxis())
            dx = wheel*m_viewRect.m_width/4.0;
        else
            dy = wheel*m_viewRect.m_height/4.0;

        SetOrigin(m_viewRect.GetLeft() + dx,
                  m_viewRect.GetTop()  + dy, true);
    }

    if ((!GetScrollOnThumbRelease() && event.LeftIsDown() && event.Dragging()) ||
        (GetScrollOnThumbRelease() && event.LeftUp()))
    {
        double x = m_viewRect.GetLeft();
        double y = m_viewRect.GetTop();

        if (axisWin->IsXAxis())
            x += (pos.x - m_mousePt.x)/m_zoom.m_x;
        else
            y += (m_mousePt.y-pos.y)/m_zoom.m_y;

        SetOrigin(x, y, true);
    }

    if (!GetScrollOnThumbRelease())
        m_mousePt = pos;
}

void wxPlotCtrl::ProcessAreaEVT_KEY_DOWN( wxKeyEvent &event )
{
    //wxPrintf(wxT("wxPlotCtrl::ProcessAreaEVT_KEY_DOWN %d `%c` S%dC%dA%d\n"), int(event.GetKeyCode()), (wxChar)event.GetKeyCode(), event.ShiftDown(), event.ControlDown(), event.AltDown());
    event.Skip(true);

    int  code  = event.GetKeyCode();
    bool alt   = event.AltDown()     || (code == WXK_ALT);
    bool ctrl  = event.ControlDown() || (code == WXK_CONTROL);
    bool shift = event.ShiftDown()   || (code == WXK_SHIFT);

    if (shift && !alt && !ctrl)
        SetAreaMouseFunction(wxPLOTCTRL_MOUSE_SELECT, true);
    else if (!shift && ctrl && !alt)
        SetAreaMouseFunction(wxPLOTCTRL_MOUSE_DESELECT, true);
    else if (ctrl && shift && alt)
        SetAreaMouseFunction(wxPLOTCTRL_MOUSE_PAN, true);
    else // if (!ctrl || !shift || !alt)
        SetAreaMouseFunction(wxPLOTCTRL_MOUSE_ZOOM, true);
}

void wxPlotCtrl::ProcessAreaEVT_KEY_UP( wxKeyEvent &event )
{
    //wxPrintf(wxT("wxPlotCtrl::ProcessAreaEVT_KEY_UP %d `%c` S%dC%dA%d\n"), int(event.GetKeyCode()), (wxChar)event.GetKeyCode(), event.ShiftDown(), event.ControlDown(), event.AltDown());
    event.Skip(true);

    int  code  = event.GetKeyCode();
    bool alt   = event.AltDown()     && (code != WXK_ALT);
    bool ctrl  = event.ControlDown() && (code != WXK_CONTROL);
    bool shift = event.ShiftDown()   && (code != WXK_SHIFT);

    if (shift && !ctrl && !alt)
        SetAreaMouseFunction(wxPLOTCTRL_MOUSE_SELECT, true);
    else if (!shift && ctrl && !alt)
        SetAreaMouseFunction(wxPLOTCTRL_MOUSE_DESELECT, true);
    else if (shift && ctrl && alt)
        SetAreaMouseFunction(wxPLOTCTRL_MOUSE_PAN, true);
    else // if (!shift && !ctrl && !alt)
        SetAreaMouseFunction(wxPLOTCTRL_MOUSE_ZOOM, true);
}

void wxPlotCtrl::OnChar(wxKeyEvent &event)
{

  // 5/19/2010 - DJH - modified calls to send an event 
  //             when changing the axes from a keystroke

    //wxPrintf(wxT("wxPlotCtrl::OnChar %d `%c` S%dC%dA%d\n"), int(event.GetKeyCode()), (wxChar)event.GetKeyCode(), event.ShiftDown(), event.ControlDown(), event.AltDown());

    // select the next curve if possible, or cursor point like left mouse
    if (event.GetKeyCode() == WXK_SPACE)
    {
        if (event.ShiftDown() || event.ControlDown())
        {
            if (IsCursorValid())
            {
                if (GetDataCurve(m_cursor_curve))
                    DoSelectDataRange(m_cursor_curve, wxRangeInt(m_cursor_index,m_cursor_index), !event.ControlDown(), true);
                else
                {
                    wxPoint2DDouble pt(m_cursorMarker.GetPlotPosition());
                    DoSelectRectangle(m_cursor_curve, wxRect2DDouble(pt.m_x,0,pt.m_x,0), !event.ControlDown(), true);
                }
            }
        }
        else
        {
            int count = GetCurveCount();
            if ((count < 1) || ((count == 1) && (m_active_index == 0))) return;
            int index = (m_active_index + 1 > count - 1) ? 0 : m_active_index + 1;
            SetActiveIndex( index, true );
        }
        return;
    }

    // These are reserved for the program
    if (event.ControlDown() || event.AltDown())
    {
        event.Skip(true);
        return;
    }

    switch (event.GetKeyCode())
    {
        // cursor keys moves the plot origin around
        case WXK_LEFT  : SetOrigin(m_viewRect.GetLeft() - m_viewRect.m_width/10.0, m_viewRect.GetTop(),true); return;
        case WXK_RIGHT : SetOrigin(m_viewRect.GetLeft() + m_viewRect.m_width/10.0, m_viewRect.GetTop(),true); return;
        case WXK_UP    : SetOrigin(m_viewRect.GetLeft(), m_viewRect.GetTop() + m_viewRect.m_height/10.0,true); return;
        case WXK_DOWN  : SetOrigin(m_viewRect.GetLeft(), m_viewRect.GetTop() - m_viewRect.m_height/10.0,true); return;
        case WXK_PAGEUP    : SetOrigin(m_viewRect.GetLeft(), m_viewRect.GetTop() + m_viewRect.m_height/2.0,true); return;
        case WXK_PAGEDOWN  : SetOrigin(m_viewRect.GetLeft(), m_viewRect.GetTop() - m_viewRect.m_height/2.0,true); return;

        // Center the plot on the cursor point, or 0,0
        case WXK_HOME :
        {
            if (IsCursorValid())
                MakeCursorVisible(true, true);
            else
                SetOrigin(-m_viewRect.m_width/2.0, -m_viewRect.m_height/2.0, true);

            return;
        }
        // try to make the current curve fully visible
        case WXK_END :
        {
            wxPlotData *plotData = GetActiveDataCurve();
            if (plotData)
            {
                wxRect2DDouble bound = plotData->GetBoundingRect();
                bound.Inset(-bound.m_width/80.0, -bound.m_height/80.0);
                SetViewRect(bound, true);
            }
            else if (GetActiveCurve())
            {
                wxPlotCurve *curve = GetActiveCurve();
                double y, min, max;

                y = max = min = curve->GetY(GetPlotCoordFromClientX(0));

                for (int i=1; i<m_areaClientRect.width; i++)
                {
                    y = curve->GetY(GetPlotCoordFromClientX(i));

                    if (wxFinite(y) != 0)
                    {
                        if (y > max) max = y;
                        if (y < min) min = y;
                    }
                }

                if (max == min)
                {
                    min -= 5;
                    max += 5;
                }

                wxRect2DDouble bound(m_viewRect.m_x, min, m_viewRect.m_width, max-min);
                SetViewRect(bound, true);
            }

            return;
        }

        // zoom in and out
        case wxT('a'): SetZoom( wxPoint2DDouble(m_zoom.m_x/1.5, m_zoom.m_y    ), true, true ); return;
        case wxT('d'): SetZoom( wxPoint2DDouble(m_zoom.m_x*1.5, m_zoom.m_y    ), true, true ); return;
        case wxT('w'): SetZoom( wxPoint2DDouble(m_zoom.m_x,     m_zoom.m_y*1.5), true, true ); return;
        case wxT('x'): SetZoom( wxPoint2DDouble(m_zoom.m_x,     m_zoom.m_y/1.5), true, true ); return;

        case wxT('q'): SetZoom( wxPoint2DDouble(m_zoom.m_x/1.5, m_zoom.m_y*1.5), true, true ); return;
        case wxT('e'): SetZoom( wxPoint2DDouble(m_zoom.m_x*1.5, m_zoom.m_y*1.5), true, true ); return;
        case wxT('z'): SetZoom( wxPoint2DDouble(m_zoom.m_x/1.5, m_zoom.m_y/1.5), true, true ); return;
        case wxT('c'): SetZoom( wxPoint2DDouble(m_zoom.m_x*1.5, m_zoom.m_y/1.5), true, true ); return;

        case wxT('='): { wxRect2DDouble r = GetViewRect(); r.Scale(.67); r.SetCentre(GetAreaMousePoint()); SetViewRect(r, true); return; }
        case wxT('-'): { wxRect2DDouble r = GetViewRect(); r.Scale(1.5); r.SetCentre(GetAreaMousePoint()); SetViewRect(r, true); return; }

        case wxT('s'): MakeCurveVisible(GetActiveIndex(), true); break;

        // Select previous/next point in a curve
        case wxT('<'): case wxT(','):
        {
            double x = GetPlotCoordFromClientX(m_areaClientRect.width-1);
            wxPlotData *plotData = GetActiveDataCurve();
            if (plotData)
            {
                if (!IsCursorValid())
                    SetCursorDataIndex(m_active_index, plotData->GetIndexFromX(x, wxPlotData::index_floor), true);
                else if (m_cursor_index > 0)
                    SetCursorDataIndex(m_cursor_curve, m_cursor_index-1, true);
            }
            else if (m_active_index >= 0)
            {
                if (!IsCursorValid())
                    SetCursorXPoint(m_active_index, x, true);
                else
                {
                    x = GetPlotCoordFromClientX((GetClientCoordFromPlotX(m_cursorMarker.GetPlotRect().m_x)-1));
                    SetCursorXPoint(m_cursor_curve, x, true);
                }
            }

            MakeCursorVisible(false, true);

            return;
        }
        case wxT('>'): case wxT('.'):
        {
            double x = GetPlotCoordFromClientX(0);
            wxPlotData *plotData = GetActiveDataCurve();
            if (plotData)
            {
                int count = plotData->GetCount();

                if (!IsCursorValid())
                    SetCursorDataIndex(m_active_index, plotData->GetIndexFromX(x, wxPlotData::index_ceil), true);
                else if (m_cursor_index < count - 1)
                    SetCursorDataIndex(m_cursor_curve, m_cursor_index+1, true);
            }
            else if (m_active_index >= 0)
            {
                if (!IsCursorValid())
                    SetCursorXPoint(m_active_index, x, true);
                else
                {
                    x = GetPlotCoordFromClientX((GetClientCoordFromPlotX(m_cursorMarker.GetPlotRect().m_x)+1));
                    SetCursorXPoint(m_cursor_curve, x, true);
                }
            }

            MakeCursorVisible(false, true);

            return;
        }

        // go to the last or next zoom
        case wxT('[') : NextHistoryView(false, true); return;
        case wxT(']') : NextHistoryView(true,  true); return;

        // delete the selected curve
        case WXK_DELETE:
        {
            if (m_activeCurve) DeleteCurve(m_activeCurve, true);
            return;
        }
        // delete current selection or go to next curve and delete it's selection
        //   finally invalidate cursor
        case WXK_ESCAPE :
        {
            BeginBatch();
            if ((m_active_index >= 0) && (GetSelectedRangeCount(m_active_index) > 0))
            {
                ClearSelectedRanges(m_active_index, true);
            }
            else
            {
                bool has_cleared = false;

                for (int i=0; i<GetCurveCount(); i++)
                {
                    if (GetSelectedRangeCount(i) > 0)
                    {
                        ClearSelectedRanges(i, true);
                        has_cleared = true;
                        break;
                    }
                }

                if (!has_cleared)
                {
                    if (IsCursorValid())
                        InvalidateCursor(true);
                    else if (m_active_index > -1)
                        SetActiveIndex( -1, true );
                }
            }
            EndBatch(); // ESC is also a generic clean up routine too!
            break;
        }

        default: event.Skip(true); break;
    }
}

void wxPlotCtrl::UpdateWindowSize()
{
    m_areaClientRect = wxRect(wxPoint(0,0), m_area->GetClientSize());
    // If something happens to make these true, there's a problem
    if (m_areaClientRect.width  < 10) m_areaClientRect.width  = 10;
    if (m_areaClientRect.height < 10) m_areaClientRect.height = 10;
}

void wxPlotCtrl::AdjustScrollBars()
{
    double range, thumbsize, position;
    double pagesize;

    range = (m_curveBoundingRect.m_width * m_zoom.m_x);
    if (!IsFinite(range, wxT("plot's x range is NaN"))) return;
    if (range > 32000) range = 32000; else if (range < 1) range = 1;

    thumbsize = (range * (m_viewRect.m_width/m_curveBoundingRect.m_width));
    if (!IsFinite(thumbsize, wxT("plot's x range is NaN"))) return;
    if (thumbsize > range) thumbsize = range; else if (thumbsize < 1) thumbsize = 1;

    position = (range * ((m_viewRect.GetLeft() - m_curveBoundingRect.GetLeft())/m_curveBoundingRect.m_width));
    if (!IsFinite(position, wxT("plot's x range is NaN"))) return;
    if (position > range - thumbsize) position = range - thumbsize; else if (position < 0) position = 0;
    pagesize = thumbsize;

    m_xAxisScrollbar->SetScrollbar( int(position), int(thumbsize), int(range), int(pagesize) );

    range = (m_curveBoundingRect.m_height * m_zoom.m_y);
    if (!IsFinite(range, wxT("plot's y range is NaN"))) return;
    if (range > 32000) range = 32000; else if (range < 1) range = 1;

    thumbsize = (range * (m_viewRect.m_height/m_curveBoundingRect.m_height));
    if (!IsFinite(thumbsize, wxT("plot's x range is NaN"))) return;
    if (thumbsize > range) thumbsize = range; else if (thumbsize < 1) thumbsize = 1;

    position = (range - range * ((m_viewRect.GetTop() - m_curveBoundingRect.GetTop())/m_curveBoundingRect.m_height) - thumbsize);
    if (!IsFinite(position, wxT("plot's x range is NaN"))) return;
    if (position > range - thumbsize) position = range - thumbsize; else if (position < 0) position = 0;
    pagesize = thumbsize;

    m_yAxisScrollbar->SetScrollbar( int(position), int(thumbsize), int(range), int(pagesize) );
}

void wxPlotCtrl::OnScroll(wxScrollEvent& event)
{
    if (m_scroll_on_thumb_release && (event.GetEventType() == wxEVT_SCROLL_THUMBTRACK))
        return;

    if (event.GetId() == ID_PLOTCTRL_X_SCROLLBAR)
    {
        double range = m_xAxisScrollbar->GetRange();
        if (range < 1) return;
        double position = m_xAxisScrollbar->GetThumbPosition();
        double origin_x = m_curveBoundingRect.GetLeft() + m_curveBoundingRect.m_width*(position/range);
        if (!IsFinite(origin_x, wxT("plot's x-origin is NaN"))) return;
        m_viewRect.m_x = origin_x;
        Redraw(wxPLOTCTRL_REDRAW_PLOT|wxPLOTCTRL_REDRAW_XAXIS);
    }
    else if (event.GetId() == ID_PLOTCTRL_Y_SCROLLBAR)
    {
        double range = m_yAxisScrollbar->GetRange();
        if (range < 1) return;
        double position = m_yAxisScrollbar->GetThumbPosition();
        double thumbsize = m_yAxisScrollbar->GetThumbSize();
        double origin_y = m_curveBoundingRect.GetTop() + m_curveBoundingRect.m_height*((range-position-thumbsize)/range);
        if (!IsFinite(origin_y, wxT("plot's y-origin is NaN"))) return;
        m_viewRect.m_y = origin_y;
        Redraw(wxPLOTCTRL_REDRAW_PLOT|wxPLOTCTRL_REDRAW_YAXIS);
    }
}

bool wxPlotCtrl::IsFinite(double n, const wxString &msg) const
{
    if (!wxFinite(n))
    {
        if (!msg.IsEmpty())
        {
            wxPlotCtrlEvent event(wxEVT_PLOTCTRL_ERROR, GetId(), (wxPlotCtrl*)this);
            event.SetString(msg);
            (void)DoSendEvent( event );
        }

        return false;
    }

    return true;
}

bool wxPlotCtrl::FindCurve(const wxPoint2DDouble &pt, const wxPoint2DDouble &dpt,
                           int &curve_index, int &data_index, wxPoint2DDouble *curvePt) const
{
    curve_index = data_index = -1;

    if (!IsFinite(pt.m_x,  wxT("point is not finite"))) return false;
    if (!IsFinite(pt.m_y,  wxT("point is not finite"))) return false;
    if (!IsFinite(dpt.m_x, wxT("point is not finite"))) return false;
    if (!IsFinite(dpt.m_y, wxT("point is not finite"))) return false;

    int curve_count = GetCurveCount();
    if (curve_count < 1) return false;

    for (int n=-1; n<curve_count; n++)
    {
        // find the point in the selected curve first
        if (n == -1)
        {
            if (m_active_index >= 0)
                n = m_active_index;
            else
                n = 0;
        }
        else if (n == m_active_index)
            continue;

        wxPlotCurve *plotCurve = GetCurve(n);
        wxPlotData  *plotData = wxDynamicCast(plotCurve, wxPlotData);

        // find the index of the closest point in a wxPlotData curve
        if (plotData)
        {
            // check if curve has BoundingRect
            wxRect2DDouble rect = plotData->GetBoundingRect();
            if ( ((rect.m_width > 0) &&
                 ((pt.m_x+dpt.m_x < rect.GetLeft()) || (pt.m_x-dpt.m_x > rect.GetRight()))) ||
                 ((rect.m_height > 0) &&
                 ((pt.m_y+dpt.m_y < rect.GetTop()) || (pt.m_y-dpt.m_y > rect.GetBottom()))) )
            {
                if ((n == m_active_index) && (n > 0)) n = -1; // start back at 0
                continue;
            }

            int index = plotData->GetIndexFromXY(pt.m_x, pt.m_y, dpt.m_x);

            double x = plotData->GetXValue(index);
            double y = plotData->GetYValue(index);

            if ((fabs(x-pt.m_x) <= dpt.m_x) && (fabs(y-pt.m_y) <= dpt.m_y))
            {
                curve_index = n;
                data_index = index;
                if (curvePt) *curvePt = wxPoint2DDouble(x, y);
                return true;
            }
        }
        else // not a data curve, just find y at this x pos
        {
            wxRect2DDouble rect = plotCurve->GetBoundingRect();
            if ((rect.m_width <= 0) || ((pt.m_x+dpt.m_x >= rect.GetLeft()) && (pt.m_x-dpt.m_x <= rect.GetRight())))
            {
                if ((rect.m_height <= 0) || ((pt.m_y >= rect.GetTop()) && (pt.m_y-dpt.m_y <= rect.GetBottom())))
                {
                    double y = plotCurve->GetY(pt.m_x);
                    if (fabs(y - pt.m_y) <= dpt.m_y)
                    {
                        curve_index = n;
                        if (curvePt) *curvePt = wxPoint2DDouble(pt.m_x, y);
                        return true;
                    }
                }
            }
        }

        // continue searching through curves
        // if on the current then start back at the beginning if not already at 0
        if ((n == m_active_index) && (n > 0)) n = -1;
    }
    return false;
}

bool wxPlotCtrl::DoSendEvent(wxPlotCtrlEvent &event) const
{
/*
    if (event.GetEventType() != wxEVT_PLOTCTRL_MOUSE_MOTION)
    {
        wxLogDebug(wxT("wxPlotCtrlEvent '%s' CurveIndex: %d, DataIndex: %d, Pos: %lf %lf, MouseFn %d"),
            wxPlotCtrl_GetEventName(event.GetEventType).c_str(),
            event.GetCurveIndex(), event.GetCurveDataIndex(),
            event.GetX(), event.GetY(), event.GetMouseFunction());
    }
*/
    return !GetEventHandler()->ProcessEvent(event) || event.IsAllowed();
}

void wxPlotCtrl::StartMouseTimer(wxWindowID win_id)
{
#if wxCHECK_VERSION(2,5,0)
    if (m_timer && (m_timer->GetId() != win_id))
        StopMouseTimer();
#else
    StopMouseTimer();  // always stop it I guess
#endif // wxCHECK_VERSION(2,5,0)

    if (!m_timer)
        m_timer = new wxTimer(this, win_id);

    if (!m_timer->IsRunning())
        m_timer->Start(200, true); // one shot timer
}
void wxPlotCtrl::StopMouseTimer()
{
    if (m_timer)
    {
        if (m_timer->IsRunning())
            m_timer->Stop();

        delete m_timer;
        m_timer = NULL;
    }
}

bool wxPlotCtrl::IsTimerRunning()
{
    return (m_timer && m_timer->IsRunning());
}

void wxPlotCtrl::OnTimer( wxTimerEvent &event )
{
    wxPoint mousePt;

    switch (event.GetId())
    {
        case ID_AREA_TIMER  : mousePt = m_area->m_mousePt;  break;
        case ID_XAXIS_TIMER : mousePt = m_xAxis->m_mousePt; break;
        case ID_YAXIS_TIMER : mousePt = m_yAxis->m_mousePt; break;
        default :
        {
            event.Skip(); // someone else's timer?
            return;
        }
    }

    double dx = (mousePt.x<0) ? -20 : (mousePt.x>GetPlotAreaRect().width ) ?  20 : 0;
    double dy = (mousePt.y<0) ?  20 : (mousePt.y>GetPlotAreaRect().height) ? -20 : 0;
    dx /= m_zoom.m_x;
    dy /= m_zoom.m_y;

    if (((dx == 0) && (dy == 0)) ||
        !SetOrigin(GetViewRect().GetLeft() + dx, GetViewRect().GetTop() + dy, true) )
    {
        StopMouseTimer();
    }
    else
        StartMouseTimer(event.GetId()); // restart timer for another round
}

void wxPlotCtrl::SetCaptureWindow(wxWindow *win)
{
    if (m_winCapture && (m_winCapture != win) && m_winCapture->HasCapture())
        m_winCapture->ReleaseMouse();

    m_winCapture = win;

    if (m_winCapture && (!m_winCapture->HasCapture()))
        m_winCapture->CaptureMouse();
}
