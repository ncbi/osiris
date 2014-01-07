/////////////////////////////////////////////////////////////////////////////
// Name:        plotprnt.cpp
// Purpose:     wxPlotPrintout
// Author:      John Labenski
// Modified by:
// Created:     8/27/2002
// Copyright:   (c) John Labenski
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "plotprnt.h"
#endif

// For compilers that support precompilation, includes "wx.h".
// #include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/msgdlg.h"
    #include "wx/dcmemory.h"
#endif // WX_PRECOMP

#include "wx/module.h"
#include "wx/printdlg.h"

#include "wx/plotctrl/plotprnt.h"
#include "wx/plotctrl/plotctrl.h"

//-----------------------------------------------------------------------------
// wxPlotPrintout
//-----------------------------------------------------------------------------
IMPLEMENT_ABSTRACT_CLASS(wxPlotPrintout, wxPrintout)

wxPrintData *wxPlotPrintout::s_wxPlotPrintData = NULL;
wxPageSetupData *wxPlotPrintout::s_wxPlotPageSetupData = NULL;
bool wxPlotPrintout::s_wxPlotPrintdata_static = false;
bool wxPlotPrintout::s_wxPlotPagesetupdata_static = false;

wxPrintData *wxPlotPrintout::GetPrintData(bool create_on_demand)
{
    if (create_on_demand && (s_wxPlotPrintData == NULL))
    {
        wxPrintData *printData = new wxPrintData;
        printData->SetPaperId(wxPAPER_LETTER);
        SetPrintData(printData, false);
    }

    return s_wxPlotPrintData;
}
wxPageSetupData *wxPlotPrintout::GetPageSetupData(bool create_on_demand)
{
    if (create_on_demand && (s_wxPlotPageSetupData == NULL))
    {
        wxPageSetupData *pageSetupData = new wxPageSetupData;
        pageSetupData->SetPaperSize(wxPAPER_LETTER);
        pageSetupData->SetMarginTopLeft(wxPoint(20, 20));
        pageSetupData->SetMarginBottomRight(wxPoint(20, 20));
        SetPageSetupData(pageSetupData, false);
    }

    return s_wxPlotPageSetupData;
}

bool wxPlotPrintout::GetPrintDataStatic()     { return s_wxPlotPrintdata_static; }
bool wxPlotPrintout::GetPageSetupDataStatic() { return s_wxPlotPagesetupdata_static; }

void wxPlotPrintout::SetPrintData( wxPrintData *printData, bool is_static )
{
    if (s_wxPlotPrintData && !s_wxPlotPrintdata_static)
        delete s_wxPlotPrintData;

    s_wxPlotPrintData = printData;
    s_wxPlotPrintdata_static = is_static;
}
void wxPlotPrintout::SetPageSetupData( wxPageSetupData *pageSetupData, bool is_static )
{
    if (s_wxPlotPageSetupData && !s_wxPlotPagesetupdata_static)
        delete s_wxPlotPageSetupData;

    s_wxPlotPageSetupData = pageSetupData;
    s_wxPlotPagesetupdata_static = is_static;
}


wxPlotPrintout::wxPlotPrintout( wxPlotCtrl* plotWin, const wxString &title )
               : wxPrintout(title), m_plotWin(plotWin)
{
}

bool wxPlotPrintout::OnBeginDocument(int startPage, int endPage)
{
    if (!wxPrintout::OnBeginDocument(startPage, endPage))
        return false;

    return true;
}

bool wxPlotPrintout::OnPrintPage(int page_n)
{
    wxDC *dc = GetDC();
    wxCHECK_MSG(dc && m_plotWin, false, wxT("Invalid dc or plotctrl"));

    if (page_n != 1) return false;

    // Get the whole size of the page in mm
    wxSize pageMMSize;
    GetPageSizeMM(&pageMMSize.x, &pageMMSize.y);

    // Get the ppi of the screen and printer
    wxSize ppiScr,ppiPrn;
    GetPPIScreen( &ppiScr.x, &ppiScr.y);
    GetPPIPrinter(&ppiPrn.x, &ppiPrn.y);

    float ppi_scale_x = float(ppiPrn.x)/float(ppiScr.x);
    float ppi_scale_y = float(ppiPrn.y)/float(ppiScr.y);

    // Get the size of DC in pixels and the number of pixels in the page
    wxSize dcSize, pagePixSize;
    dc->GetSize(&dcSize.x, &dcSize.y);
    GetPageSizePixels(&pagePixSize.x, &pagePixSize.y);

    float dc_pagepix_scale_x = float(dcSize.x)/float(pagePixSize.x);
    float dc_pagepix_scale_y = float(dcSize.y)/float(pagePixSize.y);

    // the actual ppi using the size of the dc or page in pixels
    //wxSize pixelSize = IsPreview() ? dcSize : pagePixSize;
    //float page_ppi_x = float(pixelSize.x) * (25.4 / float(pageMMSize.x));
    //float page_ppi_y = float(pixelSize.y) * (25.4 / float(pageMMSize.y));

    // If printer pageWidth == current DC width, then this doesn't
    // change. But w might be the preview bitmap width, so scale down.
    float dc_scale_x = ppi_scale_x * dc_pagepix_scale_x;
    float dc_scale_y = ppi_scale_y * dc_pagepix_scale_y;

    // calculate the pixels / mm (25.4 mm = 1 inch)
    float ppmm_x = float(ppiScr.x) / 25.4;
    float ppmm_y = float(ppiScr.y) / 25.4;

    // Adjust the page size for the pixels / mm scaling factor
    //wxSize paperSize = GetPageSetupData(true)->GetPaperSize();
    wxSize page = pageMMSize;
    page.x      = int(page.x * ppmm_x);
    page.y      = int(page.y * ppmm_y);
    wxRect pageRect  = wxRect(0, 0, page.x, page.y);

    // get margins informations and convert to printer pixels
    wxPoint topLeft     = GetPageSetupData(true)->GetMarginTopLeft();
    wxPoint bottomRight = GetPageSetupData(true)->GetMarginBottomRight();

    int top    = int(topLeft.y     * ppmm_y);
    int bottom = int(bottomRight.y * ppmm_y);
    int left   = int(topLeft.x     * ppmm_x);
    int right  = int(bottomRight.x * ppmm_x);
    wxRect printRect = wxRect(left, top, page.x-(left+right), page.y-(top+bottom));
    // printRect is the unscaled rect, suitable if you set the dc scale
    //  which we don't because it doesn't scale the fonts
    //dc->SetUserScale(dc_scale_x, dc_scale_y);

    wxRect rect(int(printRect.x*dc_scale_x),     int(printRect.y*dc_scale_x),
                int(printRect.width*dc_scale_y), int(printRect.height*dc_scale_y));
    double dpi = ppiScr.x;

    //set dpi of the drawwholeplot function
    if (IsPreview())
    {
        //dpi is the (screen dpi) * (percent of screen used by preview)
        dpi = double(ppiScr.x) * dc_pagepix_scale_x; //((double)pagePixSize.x / rect.width); //((double)rect.width / dcSize.x);
    }

    m_plotWin->DrawWholePlot( dc, rect, dpi );
    return true;
}

bool wxPlotPrintout::ShowPrintDialog()
{
    wxCHECK_MSG(GetPlotCtrl(), false, wxT("Invalid plot window"));
    wxPrintDialogData printDialogData(*wxPlotPrintout::GetPrintData(true));

    wxPrinter printer(& printDialogData);
    if (!printer.Print(GetPlotCtrl(), this, true))
    {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR)
            wxMessageBox(_("There was a problem printing.\n"
                           "Perhaps your printer is not setup correctly?"),
                         _("Printing"), wxOK, GetPlotCtrl());
    }

    if (wxPrinter::GetLastError() != wxPRINTER_CANCELLED)
        *wxPlotPrintout::GetPrintData(true) = printer.GetPrintDialogData().GetPrintData();

    return wxPrinter::GetLastError() == wxPRINTER_NO_ERROR;
}
bool wxPlotPrintout::ShowPrintPreviewDialog(const wxString& frameTitle)
{
    wxCHECK_MSG(GetPlotCtrl(), false, wxT("Invalid plot window"));
    // Pass two printout objects: for preview, and possible printing.
    wxPrintDialogData printDialogData(*wxPlotPrintout::GetPrintData(true));
    wxPrintPreview *preview = new wxPrintPreview(new wxPlotPrintout(GetPlotCtrl(), GetTitle()),
                                                 new wxPlotPrintout(GetPlotCtrl(), GetTitle()),
                                                 &printDialogData);
    if (!preview->Ok())
    {
        delete preview;
        wxMessageBox(_("There was a problem previewing.\n"
                       "Perhaps your printer is not setup correctly?"),
                     _("Previewing"), wxOK, GetPlotCtrl());
        return false;
    }

    wxRect r(wxGetClientDisplayRect());
    r.width  = wxMin(r.width,  600);
    r.height = wxMin(r.height, 650);
    wxPreviewFrame *frame = new wxPreviewFrame(preview, GetPlotCtrl(),
                                               frameTitle,
                                               wxDefaultPosition, r.GetSize());
    frame->Centre(wxBOTH);
    frame->Initialize();
    frame->Show(true);
    return true;
}
bool wxPlotPrintout::ShowPrintSetupDialog()
{
    wxCHECK_MSG(GetPlotCtrl(), false, wxT("Invalid plot window"));
    wxPrintDialogData printDialogData(*wxPlotPrintout::GetPrintData(true));
    wxPrintDialog printerDialog(GetPlotCtrl(), & printDialogData);
#if !wxCHECK_VERSION(2,7,0)
    printerDialog.GetPrintDialogData().SetSetupDialog(true);
#endif //!wxCHECK_VERSION(2,7,0)

    if (printerDialog.ShowModal() != wxID_CANCEL)
    {
        *wxPlotPrintout::GetPrintData(true) = printerDialog.GetPrintDialogData().GetPrintData();
        return true;
    }

    return false;
}
bool wxPlotPrintout::ShowPrintPageSetupDialog()
{
    wxCHECK_MSG(GetPlotCtrl(), false, wxT("Invalid plot window"));
    *wxPlotPrintout::GetPageSetupData(true) = *wxPlotPrintout::GetPrintData();
    wxPageSetupDialog pageSetupDialog(GetPlotCtrl(), wxPlotPrintout::GetPageSetupData());

    if (pageSetupDialog.ShowModal() != wxID_CANCEL)
    {
        *wxPlotPrintout::GetPrintData(true) = pageSetupDialog.GetPageSetupData().GetPrintData();
        *wxPlotPrintout::GetPageSetupData(true) = pageSetupDialog.GetPageSetupData();
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
// wxPlotCtrlModule - setup anything after init and delete before closing
//-----------------------------------------------------------------------------

class wxPlotCtrlModule : public wxModule
{
DECLARE_DYNAMIC_CLASS(wxPlotCtrlModule)
public:
    wxPlotCtrlModule() : wxModule() {}
    bool OnInit()
    {
        return true;
    }
    void OnExit()
    {
        wxPlotPrintout::SetPrintData(NULL, false);
        wxPlotPrintout::SetPageSetupData(NULL, false);
    }
};

IMPLEMENT_DYNAMIC_CLASS(wxPlotCtrlModule, wxModule)
