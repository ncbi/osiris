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
*  FileName: rgfile.cpp
*  Author:   Robert Goor
*
*/
//
//	class RGFile - class for disk I/O operations
//

#include "rgfile.h"
#ifdef _WINDOWS
#include <io.h>
#else
#include <sys/stat.h>
#endif
#include <cstdlib>
#include <cstring>
//#include <types.h>
//#include <stat.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unix.h>
#include <stdio.h>
#include <iostream>

using namespace std;


#define READ(c)	fread ((char*)&c, sizeof(c), 1, FilePtr)
#define READN(c,n) fread ((char*)c, sizeof(*c), n, FilePtr)
#define WRITE(c) fwrite ((char*)&c, sizeof(c), 1, FilePtr)
#define WRITEN(c,n)	fwrite ((const char*) c, sizeof(*c), n, FilePtr)

//static const char* UpdateMode = "rb+";  // for Windows
//static const char*    NewMode = "wb+";  // for Windows

static const char* UpdateMode = "r+";
static const char*    NewMode = "w+";


RGFile :: RGFile(const char* name, const char* mode) : 
	FileName (NULL),
	FilePtr (NULL),
	Mode (NULL) {

#ifdef FILENAME_MAX

		if (strlen (name) > FILENAME_MAX) {

			cout << "Full path file name too long:  name = " << name << endl;
			return;
		}

#endif

  if (mode)
    FilePtr = fopen (name, mode);
  
  else {

    mode = UpdateMode;   // Default mode
    FilePtr = fopen (name, mode);	// Open existing file for update

	if (FilePtr == NULL) {			// File does not exist...create it

		mode = NewMode;
		FilePtr = fopen (name, mode);	// Open new file
    }
  }

  FileName = new char [strlen (name) + 1];
  strcpy (FileName, name);
  Mode = new char [strlen (mode) + 1];
  strcpy (Mode, mode);
}

RGFile :: ~RGFile()
{
	Close();
  delete[] FileName;
  delete[] Mode;
//  delete FilePtr;
}

const char* RGFile :: Access() const {
  
	return Mode;
}

Boolean RGFile :: Close()
{
	bool bRtn = false;
	if(FilePtr != NULL)
	{
		bRtn = !!fclose(FilePtr);
		FilePtr = NULL;
	}
	return bRtn;
}
Boolean RGFile :: Exists() {

	if (FilePtr) return TRUE;
		return FALSE;
}

Boolean RGFile :: Exists (const char* name) {

  FILE* fp = fopen (name, UpdateMode);

  if (fp) {
    
	  fflush(fp);
	  fclose(fp);
	  return TRUE;
  }

  return FALSE;
}


Boolean RGFile :: isEmpty () {

	int ch;
	
	if ((ch = getc (FilePtr)) == EOF)
		return TRUE;

	ungetc (ch, FilePtr);
	return FALSE;
}


long RGFile :: GetSizeOfFile () const {
#ifdef _WINDOWS
	int fd = _fileno (FilePtr);
	return _filelength (fd);
#else
  int fd = fileno(FilePtr);
  struct stat buf;
  long nRtn = 0;
  if(!fstat(fd,&buf))
  {
    nRtn = (long) buf.st_size;
  }
  return nRtn;
#endif
}


Boolean RGFile :: Read (unsigned short& x) { return READ (x) == 1; }
Boolean RGFile :: Read (char& x) { return READ (x) == 1; }
Boolean RGFile :: Read (int& x) { return READ (x) == 1; }
Boolean RGFile :: Read (long& x) { return READ (x) == 1; }
Boolean RGFile :: Read (unsigned char& x) { return READ (x) == 1; }
Boolean RGFile :: Read (unsigned int& x) { return READ (x) == 1; }
Boolean RGFile :: Read (unsigned long& x) { return READ (x) == 1; }
Boolean RGFile :: Read (short& x) { return READ (x) == 1; }
Boolean RGFile :: Read (double& x) { return READ (x) == 1; }
Boolean RGFile :: Read (float& x) { return READ (x) == 1; }
Boolean RGFile :: Read (char* p, size_t N)  { return READN (p, N) == N; }
Boolean RGFile :: Read (short* p, size_t N)  { return READN (p, N) == N; }
Boolean RGFile :: Read (int* p, size_t N)  { return READN (p, N) == N; }
Boolean RGFile :: Read (long* p, size_t N)  { return READN (p, N) == N; }
Boolean RGFile :: Read (double* p, size_t N)  { return READN (p, N) == N; }
Boolean RGFile :: Read (float* p, size_t N)  { return READN (p, N) == N; }

Boolean RGFile :: Read (unsigned char* c, size_t N) { return READN (c, N) == N; }
Boolean RGFile :: Read (unsigned short* i, size_t N) { return READN (i, N) == N; }
Boolean RGFile :: Read (unsigned int* i, size_t N) { return READN (i, N) == N; }
Boolean RGFile :: Read (unsigned long* i, size_t N) { return READN (i, N) == N; }


Boolean RGFile :: Read (char* str) {

  int n;

  while (1) {
    n = fgetc (FilePtr);

    if( n == EOF || n == '\0') 
		break;

    *str++ = (char) n;
  }

  *str = '\0';
  return TRUE;
}


Boolean RGFile :: ReadLine (char* _str) {

	int n;
	Boolean ReturnValue = TRUE;
  char *str = _str;
	while (1) {

		n = fgetc (FilePtr);

		if (n == EOF) 
    {
      if(str == _str)
      {
        // djh 7/17/07 -- 0 bytes read;
			  ReturnValue = FALSE;
      }
			break;
		}
		
		if((n == '\0') || (n == '\n')) 
			break;

		*str++ = (char) n;
	}

	*str = '\0';
	return ReturnValue;
}


Boolean RGFile :: Write (char x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (unsigned char x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (short x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (int x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (long x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (unsigned short x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (unsigned int x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (unsigned long x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (double x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (float x) { return WRITE (x) == 1; }
Boolean RGFile :: Write (const int* p, size_t N) { return WRITEN (p, N) == N; }
Boolean RGFile :: Write (const long* p, size_t N) { return WRITEN (p, N) == N; }
Boolean RGFile :: Write (const char* p, size_t N) { return WRITEN (p, N) == N; }
Boolean RGFile :: Write (const short* p, size_t N) { return WRITEN (p, N) == N; }
Boolean RGFile :: Write (const float* p, size_t N) { return WRITEN (p, N) == N; }
Boolean RGFile :: Write (const double* p, size_t N) { return WRITEN (p, N) == N; }

Boolean RGFile :: Write (const unsigned char* c, size_t N) { return WRITEN (c, N) == N; }
Boolean RGFile :: Write (const unsigned short* i, size_t N) { return WRITEN (i, N) == N; }
Boolean RGFile :: Write (const unsigned int* i, size_t N) { return WRITEN (i, N) == N; }
Boolean RGFile :: Write (const unsigned long* i, size_t N) { return WRITEN (i, N) == N; }

Boolean RGFile :: Write (const char* str) {
  return Write(str, strlen (str));
}


unsigned long RGFile :: CurrentOffset() { return ftell (FilePtr); }
void RGFile :: ClearError() { clearerr (FilePtr); }
Boolean RGFile :: Eof()   { return feof (FilePtr); }

Boolean RGFile :: Erase() {

  fflush(FilePtr);

  if (fclose (FilePtr) != 0)
	  return FALSE;

//  if (unlink (FileName))
//	  return FALSE;

  delete FilePtr;
  FilePtr = fopen (FileName, NewMode);

  if (FilePtr == NULL)
	  return FALSE;

  return TRUE;
}


Boolean RGFile :: Error()  { return ferror (FilePtr); }

Boolean RGFile :: Flush() { return fflush (FilePtr) != EOF; }

Boolean RGFile :: SeekTo (unsigned long offset) { return fseek (FilePtr, offset, 0) >= 0; }
Boolean RGFile :: SeekToEnd () { return fseek (FilePtr, 0, 2) >= 0; }

RGFile& operator>>(RGFile& f, char*& x) {

	x = new char;
	f.Read (*x);
	return f;
}


RGFile& operator>>(RGFile& f, unsigned char*& x) {

	x = new unsigned char;
	f.Read (*x);
	return f;
}

RGFile& operator>>(RGFile& f, double*& x) {

	x = new double;
	f.Read (*x);
	return f;
}

RGFile& operator>>(RGFile& f, float*& x) {

	x = new float;
	f.Read (*x);
	return f;
}

RGFile& operator>>(RGFile& f, int*& x) {

	x = new int;
	f.Read (*x);
	return f;
}

RGFile& operator>>(RGFile& f, long*& x) {

	x = new long;
	f.Read (*x);
	return f;
}

RGFile& operator>>(RGFile& f, short*& x) {

	x = new short;
	f.Read (*x);
	return f;
}

RGFile& operator>>(RGFile& f, unsigned int*& x) {

	x = new unsigned int;
	f.Read (*x);
	return f;
}

RGFile& operator>>(RGFile& f, unsigned long*& x) {

	x = new unsigned long;
	f.Read (*x);
	return f;
}

RGFile& operator>>(RGFile& f, unsigned short*& x) {

	x = new unsigned short;
	f.Read (*x);
	return f;
}
