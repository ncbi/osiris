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
*  FileName: UnitTest.cpp
*  Author:   Douglas Hoffman
*/
#ifdef __WXDEBUG__

#include "mainApp.h"
#include "UnitTest.h"
#include "nwx/nwxString.h"
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxRound.h"
#include "nwx/nwxXmlCMF.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxXmlUnitTest.h"
#include "nwx/nwxKillDir.h"
#include "nwx/nwxLock.h"
#include "CVolumes.h"
#include "CKitList.h"
#include "CLabSettings.h"
#include "ConfigDir.h"
#include "CXSLExportFileType.h"
#include "COsirisVersion.h"
#include <wx/utils.h>

// set to true if skipping unit test
bool UnitTest::g_HasRun(false);
//bool UnitTest::g_HasRun(true);

void UnitTest::_Run()
{
  try
  {
    wxString sos = ::wxGetOsDescription();
    int n1,n2;
    wxGetOsVersion(&n1,&n2);
    nwxString::UnitTest_DateTime();
    nwxXmlIOwxDateTime::UnitTest();
    nwxRound::UnitTest();
    nwxXmlCMF::UnitTest();
    nwxString::UnitTest_Trim();
    nwxString::UnitTest_FindNoCase();
    nwxString::UnitTest_FileExt();
    nwxXmlUnitTest::RunUnitTest();
    nwxKillDir::UnitTest();
    nwxLock::UnitTest();
    COsirisVersion::UnitTest();

    {
      ConfigDir *pDir = mainApp::GetConfig();
      wxString sPath = pDir->GetExeVolumePath();
      nwxFileUtil::EndWithSeparator(&sPath);
      sPath.Append("ID");
      nwxFileUtil::EndWithSeparator(&sPath);
      sPath.Append("ID_access.txt");
    }
    CVolumes::UnitTest();
    CPersistKitList::UnitTest();
    CLabSettings::UnitTest();
    CExportFiles::UnitTest();
  }
  catch(...)
  {
    wxASSERT_MSG(0,"Exception caught in UnitTest::_Run()");
  }
}

#endif
