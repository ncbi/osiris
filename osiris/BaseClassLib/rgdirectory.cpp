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
*  FileName: rgdirectory.cpp
*  Author:   Robert Goor
*
*/
//
// class RGDirectory, which encapsulates directory management
//

#include "rgdirdefs.h"
#include "rgdirectory.h"

#include "stdio.h"
#include <iostream>


#ifdef _WINDOWS

#include <stringapiset.h>

static bool FIND_DATA_FROM_WIDE(LPWIN32_FIND_DATAA pDest, LPWIN32_FIND_DATAW pSrc)
{

// convert from LPWIN32_FIND_DATAW to LPWIN32_FIND_DATAA
//  if these structures change, this code will need to be updated
//  they are currently found in minwinbase.h at the time 
//  this comment was created is 
//  C:\Program Files (x86)\Windows Kits\10\Include\10.0.17134.0\um\minwinbase.h
//  see also:
// https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-win32_find_dataw
// https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-win32_find_dataa

//  the following fields do not apply to windows
//  DWORD dwFileType;
//  DWORD dwCreatorType;
//  WORD  wFinderFlags;

#define COPY_ITEM(x) pDest->x = pSrc->x
  COPY_ITEM(dwFileAttributes);
  COPY_ITEM(ftCreationTime);
  COPY_ITEM(ftLastAccessTime);
  COPY_ITEM(ftLastWriteTime);
  COPY_ITEM(nFileSizeHigh);
  COPY_ITEM(nFileSizeLow);
  COPY_ITEM(dwReserved0);
  COPY_ITEM(dwReserved1);
#undef COPY_ITEM

  int n;
  const int cbMultiByte = sizeof(pDest->cFileName) / sizeof(pDest->cFileName[0]);
  n = WideCharToMultiByte(
    CP_UTF8,                    // UINT CodePage
    WC_ERR_INVALID_CHARS,       // DWORD dwFlags
    pSrc->cFileName,            // lpWideCharStr
    -1,                         // int ccWideChar
    pDest->cFileName,           // lpMultiByteStr
    cbMultiByte,                // int cbMultiByte
    NULL, NULL);                // lpDefaultChar, lpUsedDefaultChar

  if (!n)
  {
    cout << "Conversion of file name to UTF-8 failed" << endl;
  }
  else
  {

    // https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte

    const int cbMultiByteA = sizeof(pDest->cAlternateFileName) / sizeof(pDest->cAlternateFileName[0]);
    n = WideCharToMultiByte(
      CP_UTF8,                    // UINT CodePage
      WC_ERR_INVALID_CHARS,       // DWORD dwFlags
      pSrc->cAlternateFileName,   // lpWideCharStr
      -1,                         // int ccWideChar
      pDest->cAlternateFileName,  // lpMultiByteStr
      cbMultiByteA,               // int cbMultiByte
      NULL, NULL);                // lpDefaultChar, lpUsedDefaultChar
    if (!n)
    {
      cout << "Conversion of alternate file name to UTF-8 failed" << endl;
    }
  }
  return !!n;
}

using namespace std;

static bool findNextFile(DIR *pDir)
{
  bool bRtn = true;
  if (!FindNextFileW(pDir->hFind, &pDir->find_dataw)) {

    // Exhausted all matches, so close and null the
    // handle.

    bRtn = false;
  }
  else if (!FIND_DATA_FROM_WIDE(&pDir->find_data, &pDir->find_dataw))
  {
    bRtn = false;
    pDir->find_data.cAlternateFileName[0] = 0;
    pDir->find_data.cFileName[0] = 0;
  }
  if (!bRtn)
  {
    FindClose(pDir->hFind);
    pDir->hFind = INVALID_HANDLE_VALUE;
  }
  return bRtn;
}

static HANDLE findfile_directory(char const *name, DIR *pDir) {

    char    search_spec[_MAX_PATH +1];
    // Simply add the *.*, ensuring the path separator is
    // included.
    //
    lstrcpyA(search_spec, name);
    if(search_spec[lstrlenA(search_spec) - 1] != '\\')
    {
        lstrcatA(search_spec, "\\*.*");
    }
    else
    {
        lstrcatA(search_spec, "*.*");
    }
    RGString wsearch(search_spec);
    HANDLE rtn = FindFirstFileW(wsearch.GetWData(), &pDir->find_dataw);
    if (rtn == INVALID_HANDLE_VALUE)
    {}
    else if(!FIND_DATA_FROM_WIDE(&pDir->find_data, &pDir->find_dataw))
    {
      FindClose(rtn);
      rtn = INVALID_HANDLE_VALUE;
    }
    return rtn;
}


RGDirectory :: RGDirectory (const RGString& fullName) : direct (NULL) {

    DWORD dwAttr;

#ifdef FILENAME_MAX

		if (fullName.Length () > FILENAME_MAX) {

			cout << "Full path name too long:  name = " << fullName.GetData () << endl;
			return;
		}

#endif

    // Must be a valid name 
	if( (fullName.Length () == 0) ||
		(dwAttr = GetFileAttributesW(fullName.GetWData ())) == INVALID_FILE_ATTRIBUTES) {

        errno = ENOENT;
    }

    // Must be a directory 

    else if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {

        errno = ENOTDIR;
    }

    else {

        direct = new DIR;   //(DIR*)malloc(sizeof(DIR));

        if (direct == NULL) {

            errno = ENOMEM;
        }

        else {
          direct->hFind = findfile_directory (fullName.GetData (), direct);

            if (direct->hFind == INVALID_HANDLE_VALUE) {

                //free(directory);
                delete direct;
                direct = NULL;
            }

            else {

                // Save the directory, in case of rewind.
                lstrcpyA(direct->directory, fullName.GetData ());
                lstrcpyA(direct->dirent.d_name, direct->find_data.cFileName);
            }
        }
    }
}



RGDirectory :: RGDirectory (const char* fullName) : direct (NULL) {

	DWORD dwAttr;

#ifdef FILENAME_MAX

		if (strlen (fullName) > FILENAME_MAX) {

			cout << "Full path name too long:  name = " << fullName << endl;
			return;
		}

#endif

    // Must be a valid name 
	if( !fullName || !(*fullName) ||
        (dwAttr = GetFileAttributesW(RGString(fullName).GetWData())) == INVALID_FILE_ATTRIBUTES) {

        errno = ENOENT;
    }

    // Must be a directory 

    else if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {

        errno = ENOTDIR;
    }

    else {

        direct = new DIR;    //(DIR*)malloc(sizeof(DIR));

        if (direct == NULL) {

            errno = ENOMEM;
        }

        else {

			direct->hFind = findfile_directory (fullName, direct);

            if (direct->hFind == INVALID_HANDLE_VALUE) {

                //free(directory);
				delete direct;
                direct = NULL;
            }

            else {

                // Save the directory, in case of rewind.
                lstrcpyA(direct->directory, fullName);
                lstrcpyA(direct->dirent.d_name, direct->find_data.cFileName);
            }
        }
    }
}



RGDirectory :: ~RGDirectory () {

    if (direct == NULL) {

        errno = EBADF;
    }

    else {

        // Close the search handle, if not already done. 
        if(direct->hFind != INVALID_HANDLE_VALUE) {

            FindClose (direct->hFind);
        }

        //free (directory);
		delete direct;
		direct = NULL;
    }
}


Boolean RGDirectory :: IsValid () const {

	return (direct != NULL);
}


Boolean RGDirectory :: ReadNextDirectory (RGString& Name) {

	// The last find exhausted the matches, so return NULL. 

    if (direct->hFind == INVALID_HANDLE_VALUE) {

        errno = EBADF;
		return FALSE;
    }

    else {

        // Copy the result of the last successful match to
       // dirent.
        
        lstrcpyA (direct->dirent.d_name, direct->find_data.cFileName);

        // Attempt the next match.
        findNextFile(direct);
        Name = (direct->dirent).d_name;
    }

	return TRUE;
}


Boolean RGDirectory :: FileIsDirectory (const RGString& Name) {

	unsigned long Attributes = GetFileAttributesW (Name.GetWData ());

	if (Attributes & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;

	return FALSE;
}


void RGDirectory :: RewindDirectory () {

	// Close the search handle, if not already done. 

    if (direct->hFind != INVALID_HANDLE_VALUE) {

        FindClose(direct->hFind);
    }

    direct->hFind = findfile_directory(direct->directory, direct);

    if (direct->hFind != INVALID_HANDLE_VALUE) {

        lstrcpyA(direct->dirent.d_name, direct->find_data.cFileName);
    }
}


void RGDirectory :: CloseDirectory () {

	if (direct == NULL) {

        errno = EBADF;
    }

    else {

        // Close the search handle, if not already done. 
        if(direct->hFind != INVALID_HANDLE_VALUE) {

            FindClose (direct->hFind);
        }

        //free (directory);
		delete direct;
		direct = NULL;
    }
}


Boolean RGDirectory :: MakeDirectory (const RGString& fullName) {

	if (fullName.Length () == 0)
		return FALSE;

    // Must not already exist 
  const wchar_t *pwName = fullName.GetWData();
	if (FileOrDirectoryExists (pwName)) {

		cout << "File or directory " << fullName << " already exists" << endl;
		return FALSE;
	}

	return CreateDirectoryW(pwName, NULL);
}


Boolean RGDirectory :: MakeDirectory (const char* fullName) {

  return (!fullName || !(*fullName)) ? false : MakeDirectory(RGString(fullName));
}


Boolean RGDirectory :: MoveDirectory (const RGString& oldName, const RGString& suffix, int MaxLevels) {

	int i = 0;
	RGString newName = oldName + suffix;

	while (i < MaxLevels) {

		if (!FileOrDirectoryExists (newName)) {

//			FinalName = newName;
			cout << "Attempting to move " << oldName.GetData () << " to " << newName.GetData () << endl;
			return MoveFileW (oldName.GetWData (), newName.GetWData ());
		}

		i++;
		newName += suffix;
	}

	return FALSE;
}


Boolean RGDirectory :: MoveDirectory (const RGString& oldName, const RGString& newName) {

	return MoveFileW (oldName.GetWData(), newName.GetWData());
}



Boolean RGDirectory::FileOrDirectoryExists(const wchar_t *fullPathName) {

  DWORD dwAttr = GetFileAttributesW(fullPathName);
  return !(dwAttr == INVALID_FILE_ATTRIBUTES);
}

Boolean RGDirectory::FileOrDirectoryExists(const RGString& fullPathName) {
  return FileOrDirectoryExists(fullPathName.GetWData());
}

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

RGDirectory :: RGDirectory (const RGString& fullName) {

	direct = opendir (fullName.GetData ());
}



RGDirectory :: RGDirectory (const char* fullName) {

	direct = opendir (fullName);
}


RGDirectory :: ~RGDirectory () {

	if (direct != NULL)
	{
    closedir (direct);
  }
}

Boolean RGDirectory :: IsValid () const {

	return (direct != NULL);
}


Boolean RGDirectory :: ReadNextDirectory (RGString &rtn) {

	struct dirent* temp = readdir (direct);
  bool bRtn = false;
	if (temp != NULL)
  {
    bRtn = true;
		rtn = temp->d_name;
  }


	return bRtn;
}


Boolean RGDirectory :: FileIsDirectory (const RGString& Name) {

	struct stat statbuf;
	lstat(Name.GetData (), &statbuf);
	
	if (S_ISDIR (statbuf.st_mode) == 0)
		return FALSE;

	return TRUE;
}


void RGDirectory :: RewindDirectory () {

	rewinddir (direct);
}

/*
Boolean RGDirectory :: MoveDirectory (const RGString& oldName, const RGString& suffix, int MaxLevels) {

	int i = 0;
	RGString newName = oldName + suffix;

	while (i < MaxLevels) {

		if (!FileOrDirectoryExists (newName)) {

			return MoveFile (oldName, newName);
		}

		i++;
		newName += suffix;
	}

	return FALSE;
}


Boolean RGDirectory :: MoveDirectory (const RGString& oldName, const RGString& newName) {

	return MoveFile (oldName, newName);
}
*/

Boolean RGDirectory :: FileOrDirectoryExists (const RGString& fullPathName) 
{
  struct stat buf;
  int nStat = stat(fullPathName,&buf);
  bool bRtn = (!nStat) && (buf.st_mode & S_IFDIR) ;
  return bRtn;
}

Boolean RGDirectory :: MakeDirectory (const char *fullName)
{
  int n = mkdir(fullName,0777);
  return (n == 0);
}
Boolean RGDirectory :: MakeDirectory (const RGString& fullName)
{
  return MakeDirectory(fullName.GetData());
}

#endif
