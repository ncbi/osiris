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

using namespace std;


//struct dirent {
    
//	char d_name[NAME_MAX + 1];   /*!< file name (null-terminated) */
//};

#ifdef WIN32


static HANDLE findfile_directory(char const *name, LPWIN32_FIND_DATA data) {

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

    return FindFirstFileA(search_spec, data);
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
		(dwAttr = GetFileAttributes(fullName.GetData ())) == INVALID_FILE_ATTRIBUTES) {

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

			direct->hFind = findfile_directory (fullName.GetData (), &direct->find_data);

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
        (dwAttr = GetFileAttributes(fullName)) == INVALID_FILE_ATTRIBUTES) {

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

			direct->hFind = findfile_directory (fullName, &direct->find_data);

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

        if (!FindNextFileA(direct->hFind, &direct->find_data)) {

            // Exhausted all matches, so close and null the
            // handle.
            
            FindClose(direct->hFind);
            direct->hFind = INVALID_HANDLE_VALUE;
        }

        Name = (direct->dirent).d_name;
    }

	return TRUE;
}


Boolean RGDirectory :: FileIsDirectory (const RGString& Name) {

	unsigned long Attributes = GetFileAttributes (Name.GetData ());

	if (Attributes & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;

	return FALSE;
}


void RGDirectory :: RewindDirectory () {

	// Close the search handle, if not already done. 

    if (direct->hFind != INVALID_HANDLE_VALUE) {

        FindClose(direct->hFind);
    }

    direct->hFind = findfile_directory(direct->directory, &direct->find_data);

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
	
	if (FileOrDirectoryExists (fullName)) {

		cout << "File or directory " << fullName << " already exists" << endl;
		return FALSE;
	}

	return CreateDirectory (fullName, NULL);
}


Boolean RGDirectory :: MakeDirectory (const char* fullName) {

	if (!fullName || !(*fullName))
		return FALSE;

    // Must not already exist 

	if (FileOrDirectoryExists (fullName))
		return FALSE;

	return CreateDirectory (fullName, NULL);
}


Boolean RGDirectory :: MoveDirectory (const RGString& oldName, const RGString& suffix, int MaxLevels) {

	int i = 0;
	RGString newName = oldName + suffix;

	while (i < MaxLevels) {

		if (!FileOrDirectoryExists (newName)) {

//			FinalName = newName;
			cout << "Attempting to move " << oldName.GetData () << " to " << newName.GetData () << endl;
			return MoveFile (oldName.GetData (), newName.GetData ());
		}

		i++;
		newName += suffix;
	}

	return FALSE;
}


Boolean RGDirectory :: MoveDirectory (const RGString& oldName, const RGString& newName) {

	return MoveFile (oldName, newName);
}


Boolean RGDirectory :: FileOrDirectoryExists (const RGString& fullPathName) {

	DWORD dwAttr = GetFileAttributes (fullPathName);

	if (dwAttr == INVALID_FILE_ATTRIBUTES)
		return FALSE;

/*	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile (fullPathName, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose (hFind);
	cout << "File exists:  " << fullPathName << endl;
	return TRUE;*/
	return TRUE;
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
