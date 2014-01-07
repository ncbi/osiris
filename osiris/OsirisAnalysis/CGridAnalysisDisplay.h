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
*  FileName: CGridAnalysisDisplay.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __CGRID_ANALYSIS_DISPLAY_H__
#define __CGRID_ANALYSIS_DISPLAY_H__


class CGridAnalysisDisplay
{
public:
  static const int SORT_DISPLAYED_NAME;
  static const int SORT_FILE_NAME;
  static const int SORT_SAMPLE_NAME;
  static const int SORT_SEVERITY;
  static const int SORT_RUN_TIME;


  static int GetSortType(bool bForDisplay = true);
  static void SetSortTypeFromEvent(int nID);
  static void SetDisplayType(int n);
  static void SetDisplayTypeFromEvent(int nID);
  static int GetDisplayType();
  static bool GetControlsOnTop();
  static void SetControlsOnTop(bool b);


private:
  CGridAnalysisDisplay() {} // prevent instantiation
};



#endif