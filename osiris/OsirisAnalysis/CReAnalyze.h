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
*  FileName: CReAnalyze.h
*  Author:   Douglas Hoffman
*    Copy fsa/hid files to a new directory for re-analysis and
*    with disabled files omitted and report any errors
*
*
*/
#ifndef __C_RE_ANALYZE_H__
#define __C_RE_ANALYZE_H__

#include <wx/string.h>
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

class COARfile;
class COARsample;
class CParmOsiris;

class CReAnalyze
{
public:
  CReAnalyze(COARfile *pFile);
  virtual ~CReAnalyze();
  const CParmOsiris &GetParms() const
  {
    return *m_pParms;
  }
  const wxString &GetErrorMessage() const
  {
    return m_sErrors;
  }
  bool IsOK() const
  {
    return m_bOK;
  }
  bool CleanupNewFiles();
private:
  void _Cleanup();
  bool _Mkdir(COARfile *);
  bool _FindSamples(COARfile *);
  bool _CopySamples();
  vector<wxString> m_vsSamples;
  vector<wxString> m_vsNewFiles;
  wxString m_sInputDir;
  wxString m_sNewInputDir;
  wxString m_sErrors;
  CParmOsiris *m_pParms;
  bool m_bOK;
};

#endif
