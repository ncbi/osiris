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
*  OSIRIS is a desktop tool working on your computer with your own data.
*  Your sample profile data is processed on your computer and is not sent
*  over the internet.
*
*  For quality monitoring, OSIRIS sends some information about usage
*  statistics  back to NCBI.  This information is limited to use of the
*  tool, without any sample, profile or batch data that would reveal the
*  context of your analysis.  For more details and instructions on opting
*  out, see the Privacy Information section of the OSIRIS User's Guide.
*
* ===========================================================================
*
*
*  FileName: CPositiveControls.h
*  Author:   Douglas Hoffman
*
*  classes for reading and writing standard positive control files.
*
*/
#ifndef __C_POSITIVE_CONTROLS_H__
#define __C_POSITIVE_CONTROLS_H__
#include "nwx/nwxGlobalObject.h"
#include <wx/string.h>

class _CPositiveControls;

class CPositiveControlsAll
{
public:
  static const wxString GetFileName(const wxString &sControlName);
  nwxDECLARE_GLOBAL_OBJECT(CPositiveControlsAll)
private:
  CPositiveControlsAll();
  virtual ~CPositiveControlsAll();
  void _Cleanup();
  void _CheckSiteFile();
  wxString m_sSiteFileName;
  _CPositiveControls *m_pPosOsiris;
  _CPositiveControls *m_pPosSite;
  bool m_bErrorLogged;
};



#endif