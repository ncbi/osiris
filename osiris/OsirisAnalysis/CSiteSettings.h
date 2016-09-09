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
*  FileName: CSiteSettings.h
*  Author:   Douglas Hoffman
*  Purpoase: Store settings for this installation of OSIRIS
*    as opposed to CParmOsiris which is USER settings
*
*/
#ifndef __C_SITE_SETTINGS_H__
#define __C_SITE_SETTINGS_H__

#include "nwx/nwxSettingsFile.h"
#include "nwx/nwxGlobalObject.h"

class CSiteSettings : public nwxSettingsFile
{
  // constructor/destructor are private
  // and called from public static functions
  // GetGlobal() and CleanupGlobal()
  // defined in nwxGlobalObject.h
private:
  CSiteSettings();
  virtual ~CSiteSettings()  {}
public:
  virtual const wxString &RootNode(void) const
  {
    return m_sROOT_NODE;
  }
private:
  const wxString m_sROOT_NODE;
  nwxDECLARE_GLOBAL_OBJECT_XML(CSiteSettings)
};


#endif
