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
*  FileName: rgfile.h
*  Author:   Robert Goor
*
*/
#ifndef _RGFILE_H_
#define _RGFILE_H_

//
//	class RGFile - class for disk I/O operations
//

#include <cstdio>
#include "rgdefs.h"

using namespace std;


class RGFile {
  
public:
  RGFile (const char* name, const char* mode = NULL);  // Assumes a binary file; under Windows, 
		// may have to use mode = "wb+"
  virtual ~RGFile ();
       
  const char* Access() const;
  const char* GetName ()   const { return FileName; }
  FILE* GetFile () const { return FilePtr; }
  Boolean isValid () const { return FilePtr != NULL; }
  Boolean Exists();
  static Boolean Exists (const char* name);
  Boolean isEmpty ();
  long GetSizeOfFile () const;

	virtual Boolean Close();
  Boolean Read (char& c);
  Boolean Read (short& i);
  Boolean Read (int& i);
  Boolean Read (long& i);
  Boolean Read (unsigned char& c);
  Boolean Read (unsigned short&);
  Boolean Read (unsigned int&);
  Boolean Read (unsigned long&);
  Boolean Read (float&);
  Boolean Read (double&);
  Boolean Read (char*, size_t N);
  Boolean Read (short*, size_t N);
  Boolean Read (int*, size_t N);
  Boolean Read (long*, size_t N);
  Boolean Read (unsigned char* c, size_t N);
  Boolean Read (unsigned short* i, size_t N);
  Boolean Read (unsigned int* i, size_t N);
  Boolean Read (unsigned long* i, size_t N);
  Boolean Read (float*, size_t N);
  Boolean Read (double*, size_t N);
  Boolean Read (char* string);   // Reads to EOF or '\0'  
  Boolean ReadLine (char* string);   // Reads to EOF or '\0' or '\n'
  
  Boolean Write (char c);
  Boolean Write (short s);
  Boolean Write (int i);
  Boolean Write (long l);
  Boolean Write (unsigned char c);
  Boolean Write (unsigned short s);
  Boolean Write (unsigned int i);
  Boolean Write (unsigned long l);
  Boolean Write (float f);
  Boolean Write (double d);
  Boolean Write (const char* string);
  Boolean Write (const short*, size_t N);
  Boolean Write (const int*, size_t N);
  Boolean Write (const long*, size_t N);
  Boolean Write (const unsigned char* c, size_t N);
  Boolean Write (const unsigned short* i, size_t N);
  Boolean Write (const unsigned int* i, size_t N);
  Boolean Write (const unsigned long* i, size_t N);
  Boolean Write (const float*, size_t N);
  Boolean Write (const double*, size_t N);
  Boolean Write (const char* str, size_t N);

  unsigned long  CurrentOffset (); // Returns current file offset
  void ClearError ();  // reset to no error
  Boolean Eof();           // Returns TRUE if file at EOF
  Boolean Erase ();
  Boolean Error ();     // Returns TRUE if Current Error flag != 0
  Boolean Flush ();     // Writes all pending output
  Boolean SeekTo (unsigned long offset); // offset from beginning of file
  Boolean SeekToBegin () { return SeekTo (0); }
  Boolean SeekToEnd ();

  Boolean eof() {return Eof();}
  Boolean good () {return (isValid () && !Error() && !Eof());}

protected:
  char* FileName;
  FILE* FilePtr;
  char* Mode;
};

inline RGFile& operator<<(RGFile& f, char x) 
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, short x) 
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, int x)
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, long x) 
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, unsigned char x) 
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, unsigned short x) 
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, unsigned int x) 
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, unsigned long x) 
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, float x) 
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, double x) 
{ f.Write (x); return f; }

inline RGFile& operator<<(RGFile& f, const char* x) 
{ f.Write (x); return f; }

inline RGFile& operator>>(RGFile& f, char& x) 
{ f.Read (x); return f; }

inline RGFile& operator>>(RGFile& f, short& x) 
{ f.Read (x); return f; }

inline RGFile& operator>>(RGFile& f, int& x) 
{ f.Read (x); return f; }

inline RGFile& operator>>(RGFile& f, long& x) 
{ f.Read (x); return f; }

inline RGFile& operator>>(RGFile& f, unsigned char& x) 
{ f.Read (x); return f; }

inline RGFile& operator>>(RGFile& f, unsigned short& x) 
{ f.Read (x); return f; }

inline RGFile& operator>>(RGFile& f, unsigned int& x) 
{ f.Read (x); return f; }

inline RGFile& operator>>(RGFile& f, unsigned long& x) 
{ f.Read (x); return f; }

inline RGFile& operator>>(RGFile& f, float& x) 
{ f.Read (x); return f; }

inline RGFile& operator>>(RGFile& f, double& x)
{ f.Read (x); return f; }

RGFile& operator>>(RGFile& s, char*& ip);
RGFile& operator>>(RGFile& s, short*& ip);
RGFile& operator>>(RGFile& s, int*& ip);
RGFile& operator>>(RGFile& s, long*& ip);
RGFile& operator>>(RGFile& s, unsigned char*& ip);
RGFile& operator>>(RGFile& s, unsigned int*& ip);
RGFile& operator>>(RGFile& s, unsigned long*& ip);
RGFile& operator>>(RGFile& s, unsigned short*& ip);
RGFile& operator>>(RGFile& s, float*& ip);
RGFile& operator>>(RGFile& s, double*& ip);


#endif  /* _RGFILE_H_ */
