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
*  FileName: rgdirdefs.h
*  Author:   Robert Goor
*
*/
//
//  definitions for directory management
//

#ifndef _RGDIRDEFS_H_
#define _RGDIRDEFS_H_


#ifndef NAME_MAX
 #define NAME_MAX   (260)   /*!< The maximum number of characters (including null terminator) in a directory entry name */
#endif /* !NAME_MAX */

#ifdef _WINDOWS

#include <windows.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <errno.h>

struct dirent {
    
	char d_name[NAME_MAX + 1];   /*!< file name (null-terminated) */
};


struct dirent_dir {

    char            directory[_MAX_DIR + 1];    
    WIN32_FIND_DATAA find_data;                  // The Win32 FindFile data. 
    WIN32_FIND_DATAW find_dataw;                  // The Win32 FindFile data. wide char
    HANDLE          hFind;                      // The Win32 FindFile handle. 
    struct dirent   dirent;                     // The handle's entry.
};

#endif

#endif  /*  _RGDIRDEFS_H_  */
