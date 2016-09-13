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

*  FileName: wxIDS.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __WXIDS_H__
#define __WXIDS_H__

#ifdef __WXMAC__
#define ID_BORDER 6
#define __WINDOW_LIST__
#else
#define ID_BORDER 4
#endif

//
//  IDmenuItems for displaying menu Alleles, BPS, etc.
//    this works as an offset to a 'base' number
//

enum
{
  IDmenuDisplayNone = 0,
  IDmenuDisplayAlleles,
  IDmenuDisplayBPS,
  IDmenuDisplayRFU,
  IDmenuDisplayTime,
  IDmenuDisplayPeakArea,
  IDmenuDisplay_MAX
};

enum
{
  IDmenuDisplayNameFile = 0,
  IDmenuDisplayNameSample,
  IDmenuDisplayName_MAX
};

//
//  wxMenuItem IDs for Plot Menus
//  This works as an offset from IDmenuPlotBase_0 - IDmenuPlotBase_7

enum
{
  IDmenuPlotBase = 0,
  IDmenuPlotDataAnalyzed,
  IDmenuPlotDataRaw,
  IDmenuPlotDataLadder,
  IDmenuPlotDataBaseline,
  IDmenuPlotSync,
  IDmenuPlotILS,
  IDmenuPlotRFU,
  IDmenuPlotLadderLabels,
  IDmenuPlotLabels_BEGIN,

  // display menu items for CFramePlot menus
  // The actual numbers are IDmenuDisplayBEGIN + offset where
  //  the offset is from the IDmenuDisplay... enumerations above
  //  This determines whether to display Alleles, BPS, RFU, etc.

  IDmenuPlotLabels_END = IDmenuPlotLabels_BEGIN + IDmenuDisplay_MAX,
  IDmenuPlotArtifactsNone,
  IDmenuPlotArtifactsAll,
  IDmenuPlotArtifactsCritical,
  IDmenuPlotResetAxes,
  IDmenuPlotResetAxesFull,
  IDmenuPlotMultiple,
  IDmenuPlotAppend,
  IDmenuPlotRemove,
  IDmenuPlotRemoveOthers,

  // allow for 24 Loci

  IDmenuPlotLocus,
  IDmenuPlotLocusMax = IDmenuPlotLocus + 23,

  // allow for 8 channels

  IDmenuPlotChannel1,
  IDmenuPlotChannel2,
  IDmenuPlotChannel3,
  IDmenuPlotChannel4,
  IDmenuPlotChannel5,
  IDmenuPlotChannel6,
  IDmenuPlotChannel7,
  IDmenuPlotChannel8,

  IDmenuPlot_MAX
};

#define IDmenuPlotChannelMax IDmenuPlotChannel8

#define ID_GET_CHANNEL_ID_FROM_NR(n) (IDmenuPlotChannel1 + (n) - 1)
//#define ID_GET_CHANNEL_NR_FROM_ID(nID) ((nID) - IDmenuChannel1 + 1)

#define ID_IS_WITHIN(n,lo,hi) \
  ( ((n) >= (lo)) && ((n) <= (hi)) )

#define ID_PLOT_IS_LOCUS(n) \
  ID_IS_WITHIN(n,IDmenuPlotLocus,IDmenuPlotLocusMax)
#define ID_PLOT_IS_LABELS(n) \
  ID_IS_WITHIN(n,IDmenuPlotLabels_BEGIN,IDmenuPlotLabels_END)
#define IDmenuPlotChannelMAX  IDmenuPlotChannel8
#define ID_PLOT_IS_CHANNEL(n) \
  ID_IS_WITHIN(n,IDmenuPlotChannel1,IDmenuPlotChannelMax)
//   window and menu IDs used for wxWidgets in OSIRIS
#define ID_PLOT_IS_PLOT(n) \
  ID_IS_WITHIN(n,IDmenuPlotBase, IDmenuPlot_MAX)

#define ID_MAX_USER_EXPORT 20

#define ID_IS_USER_EXPORT(n) \
  ID_IS_WITHIN(n,IDExportUser,IDExportUserSubMenu - 1)

enum
{
  _junk = wxID_HIGHEST,
  IDprocess,
  IDanalyze,
  IDopenPlot,
  IDopenBatch,
  IDExportGraphic,
  IDMaxLadderLabels,  // maximum number of peak labels on ladder graph. 12/07/10
  IDpreview,
  IDlab,
  IDartifactLabels,
  IDexport,
  IDeditColours,
  IDlog,
  IDgrid,
  IDgridLocus,
  IDeditNotes,
  IDeditUser,
  IDtimer,
  IDvolumeName,
  IDbrowseInput,
  IDbrowseOutput,
  IDhyperlinkInput,
  IDhyperlinkOutput,
  IDhyperlinkStatus,
  IDhyperlinkStatus_END = IDhyperlinkStatus + 50,
  IDbuttonSelectAll,
  IDbuttonClear,
  IDbuttonCancel,
  IDbuttonCancelAll,
  IDbuttonReAnalyze,
  IDbuttonView,
  IDbuttonFinishLater, // CMF file
  IDbuttonEditBGcolor, // CDialogEditGridColours
  IDbuttonEditFGcolor,
  IDbuttonReverse,
  IDbuttonResetDefault,
  IDtextSearchMRU,
  IDbuttonClearSearchMRU,
  IDbuttonCleanMRU,

  IDlistProcesses,

  IDlistMRU,

  IDhelpAbout,
  IDhelpContactUs,
  IDcheckForUpdates,
  IDhelp,

  // graphics window
  //  removed many button IDs and replaced with #define below
  IDgraphRebuild,

  IDgraphZoomOut,
  IDgraphSyncAxes,
  IDgraphLabelsCombo,
  IDgraphNameCombo,
  IDgraphArtifactCombo,
  IDgraphMultiple,
  IDgraphAppend,
  IDgraphRemove,
  IDgraphTimer,

  // buttons for lab settings, export, etc.

  IDadd,
  IDremove,
  IDrename,
  IDedit,
  IDback,
  IDnext,
  IDlock,
  IDvolume,


  // Export PNG file

  IDpngUnits,
  IDpngUsage,


  IDmenu_START,  // used for EVT_MENU_RANGE
  // begin menu and toolbar IDs for MDI child windows

  //*****************************************************
  //  menus in CFrameAnalysis

  IDmenuEditCell,
  IDmenuEditDirectory,
  IDmenuAcceptLocus,
  IDmenuAcceptSample,
  IDmenuAcceptILS,
  IDmenuAcceptChannels,
  IDmenuAcceptDirectory,
  IDmenuReviewLocus,
  IDmenuReviewSample,
  IDmenuReviewILS,
  IDmenuReviewChannels,
  IDmenuReviewDirectory,
  IDmenuDisableSample,
  IDmenuDisableMultiple,
  IDmenuDisplayGraph,
  IDmenuDisplaySample, // Temporary sample window trigger STOP HERE

  IDmenuTogglePreview,
  IDmenuReAnalyze,
  IDmenuHistoryView,

  // sorting

  IDmenuSortDisplayedName,
  IDmenuSortFileName,
  IDmenuSortSampleName,
  IDmenuSortSeverity,
  IDmenuSortRunTime,

  // check item

  IDmenuSortControlsAtTop,

  // display name

  // determines whether to display file names or sample names
  // uses enumeration above that ends with IDmenuDisplayName_MAX
  IDmenuDisplayNameBASE,
  IDmenuDisplayNameBASEMAX = IDmenuDisplayNameBASE + IDmenuDisplayName_MAX,

  // display menu items for CFrameAnalysis menus
  // The actual numbers are IDmenuDisplayBEGIN + offset where
  //  the offset is from the IDmenuDisplay... enumerations above
  //  This determines whether to display Alleles, BPS, RFU, etc.
  IDmenuDisplayBEGIN,
  IDmenuDisplayEND = IDmenuDisplayBEGIN + IDmenuDisplay_MAX,

  IDExportCMF,
  // user defined export options
  IDExportUser,
  IDExportUserSubMenu = IDExportUser + ID_MAX_USER_EXPORT,

  // common menu items

  IDmenuParameters,
  IDmenuShowHideToolbar,
  IDmenuShowHidePlotScrollbars,  // CFramePlot
  IDmenuShowHideWindowScrollbar, // CFramePlot
  IDframePlot,
  IDmenuTable,
  // file history menu items

  IDmenuHistoryTop, // used for history submenu,
                    // but needs an ID in CFramePlot
  IDmenuHistoryCurrent,
  IDmenuHistoryOriginal,
  IDmenuHistoryMore,
  IDmenuHistory,
  IDmenuHistoryMAX = IDmenuHistory + 20,

  // Plot Menus, base numbers with offsets
  //   in IDmenuPlot enumeration above

  IDmenuPlotBase_0,
  IDmenuPlotBase_1 = IDmenuPlotBase_0 + IDmenuPlot_MAX,
  IDmenuPlotBase_2 = IDmenuPlotBase_1 + IDmenuPlot_MAX,
  IDmenuPlotBase_3 = IDmenuPlotBase_2 + IDmenuPlot_MAX,
  IDmenuPlotBase_4 = IDmenuPlotBase_3 + IDmenuPlot_MAX,
  IDmenuPlotBase_5 = IDmenuPlotBase_4 + IDmenuPlot_MAX,
  IDmenuPlotBase_6 = IDmenuPlotBase_5 + IDmenuPlot_MAX,
  IDmenuPlotBase_7 = IDmenuPlotBase_6 + IDmenuPlot_MAX,

  IDmenuPlotBase_END,

    // sample window
  IDmenuSampleTile,
  IDSampleApplyAll,
  IDSampleApply,
  IDSampleAccept,
  IDSampleApprove,

  IDmenu_END,

  //  END menus in CFrameAnalysis
  //*****************************************************
  //  BEGIN window list menu
#ifdef __WINDOW_LIST__
#ifdef __WXMAC__
  IDmenuWindow_Minimize,
  IDmenuWindow_Zoom,
  IDmenuWindow_AllToFront,
#endif
  IDmenuWindow_Frame,
  IDmenuWindow_Frame_END = IDmenuWindow_Frame + 100, 
                  // hopefully overkill, but only showing 100 in menu
#endif
  // END window list menu


  // file history buttons, panels, etc.

  IDhistoryPanel,
  IDhistoryButton,
  IDhistoryPrevNext,
  IDsplitterWindow,

  // ILS Family in Lab Settings and Analysis dialog

  ID_ILS_FAMILY

};

#define IDmenuPlotBase_MAX IDmenuPlotBase_7
#define ID_GET_PLOT_BASE(n)  \
  (IDmenuPlotBase_0 + (n * IDmenuPlot_MAX))
#define ID_GET_PLOT_NUMBER(ID) \
  ( (ID - IDmenuPlotBase_0) / IDmenuPlot_MAX )
#define ID_IS_MENU_PLOT(nID) \
  ID_IS_WITHIN(nID,IDmenuPlotBase_0,IDmenuPlotBase_END)


enum DATA_TYPE
{
  ANALYZED_DATA = 0,
  RAW_DATA,
  LADDER_DATA,
  BASELINE_DATA,
  COUNT_DATA
};

#define ID_IS_HISTORY(n) \
  ID_IS_WITHIN(n,IDmenuHistoryCurrent,IDmenuHistoryMAX)

#define ID_IS_DISPLAY(n,base) \
  ID_IS_WITHIN((n - base),IDmenuDisplayNone, IDmenuDisplayPeakArea)

#define IDgraphAnalyzed IDgraphRebuild
#define IDgraphRaw IDgraphRebuild
#define IDgraphLadder IDgraphRebuild
#define IDgraphBaseline IDgraphRebuild
#define IDgraphILS IDgraphRebuild
#define IDgraphRFU IDgraphRebuild
#define IDgraphLadderLabels IDgraphRebuild
// channel 1 IDgraphRebuild channels must be in order
// to loop through the numbers if part of enum
#define IDgraphCH1 IDgraphRebuild
#define IDgraphCH2 IDgraphRebuild
#define IDgraphCH3 IDgraphRebuild
#define IDgraphCH4 IDgraphRebuild
#define IDgraphCH5 IDgraphRebuild
#define IDgraphCH6 IDgraphRebuild
#define IDgraphCH7 IDgraphRebuild
#define IDgraphCH8 IDgraphRebuild

#define IDbuttonDetails IDmenuParameters
#define IDgraphTable IDmenuTable
#define IDdelete IDremove


#define IDmenuSortBASE IDmenuSortDisplayedName

#define CHANNEL_MAX 8

#define MIN_CRITICAL_ARTIFACT 15

#define SIZE_EDIT_LOCUS 800,600
#define SIZE_EDIT_ALERTS 640,480
#define SIZE_LOCUS_HISTORY 920,690


#endif
