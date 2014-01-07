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
*  FileName: rgvstream.h
*  Author:   Robert Goor
*
*/
#ifndef _RGVSTREAM_H_
#define _RGVSTREAM_H_

//
//	classes RGVInStream and RGVOutStream - abstract classes for streamed I/O operations
//

#include <iostream>

using std::ios;

class RGvios {

public:

  virtual ~RGvios();

  virtual int eof() = 0;
  virtual int fail() = 0;
  virtual int bad() = 0;
  virtual int good() = 0;
  virtual int rdstate() = 0;
  virtual void clear (int v = 0) = 0;
};


class RGios : public RGvios {

public:

  RGios() : status(0) {;}
  virtual ~RGios();
  
  virtual int eof() {return status & ios::eofbit;}
  virtual int fail() {return status & ios::failbit;}
  virtual int bad() {return status & ios::badbit;}
  virtual int good() {return status == 0;}
  virtual int rdstate() {return status;}
  virtual void clear(int s = 0)  {status = s;}

protected:

  int status;
};


class RGVInStream : public RGvios {

public:

  virtual ~RGVInStream();
  virtual int eof() = 0;
  virtual int fail() = 0;
  virtual int bad() = 0;
  virtual int good() = 0;
  virtual int rdstate () = 0;
  virtual void clear (int v = 0) = 0;

  virtual int Get() = 0;  // return EOF when at eof
  virtual RGVInStream& Get(char&) = 0;   // number
  virtual RGVInStream& Get(unsigned char&) = 0;  // number
  virtual RGVInStream& Get(char*, size_t N) = 0;
  virtual RGVInStream& Get(short*, size_t N) = 0;
  virtual RGVInStream& Get(int*, size_t N) = 0;
  virtual RGVInStream& Get(long*, size_t N) = 0;
  virtual RGVInStream& Get(float*, size_t N) = 0;
  virtual RGVInStream& Get(double*, size_t N) = 0;
  virtual RGVInStream& Get(unsigned char* , size_t N) = 0;
  virtual RGVInStream& Get(unsigned short*, size_t N) = 0;
  virtual RGVInStream& Get(unsigned int*, size_t N) = 0;
  virtual RGVInStream& Get(unsigned long*, size_t N) = 0;
  virtual RGVInStream& GetString(char* s, size_t N) = 0;  // string, so look for null termination
  virtual RGVInStream& operator>>(char&) = 0;   // char
  virtual RGVInStream& operator>>(short&) = 0;
  virtual RGVInStream& operator>>(int& i) = 0;
  virtual RGVInStream& operator>>(long&) = 0;
  virtual RGVInStream& operator>>(float&) = 0;
  virtual RGVInStream& operator>>(double&) = 0;
  virtual RGVInStream& operator>>(unsigned char&) = 0;
  virtual RGVInStream& operator>>(unsigned short&) = 0;
  virtual RGVInStream& operator>>(unsigned int&) = 0;
  virtual RGVInStream& operator>>(unsigned long&) = 0;
};


class RGVOutStream : public RGvios {

public:
  virtual ~RGVOutStream();
  virtual int eof() = 0;
  virtual int fail() = 0;
  virtual int bad() = 0;
  virtual int good() = 0;
  virtual int rdstate() = 0;
  virtual void clear (int s = 0) = 0;

  virtual RGVOutStream& putString(const char*, size_t) = 0;   // string with null termination
  virtual RGVOutStream& operator<<(const char*) = 0;   // char
  virtual RGVOutStream& operator<<(char) = 0;   // char
  virtual RGVOutStream& operator<<(unsigned char) = 0;
  virtual RGVOutStream& operator<<(double) = 0;
  virtual RGVOutStream& operator<<(float) = 0;
  virtual RGVOutStream& operator<<(int) = 0;
  virtual RGVOutStream& operator<<(unsigned int) = 0;
  virtual RGVOutStream& operator<<(long) = 0;
  virtual RGVOutStream& operator<<(unsigned long) = 0;
  virtual RGVOutStream& operator<<(short) = 0;
  virtual RGVOutStream& operator<<(unsigned short) = 0;
  virtual RGVOutStream& flush() = 0;

  virtual RGVOutStream& put(char) = 0;   // number
  virtual RGVOutStream& put(const char* p, size_t N) = 0;   // numbers
  virtual RGVOutStream& put(unsigned char) = 0;
  virtual RGVOutStream& put(const unsigned char*, size_t N) = 0;
  virtual RGVOutStream& put(const short*, size_t N) = 0;
  virtual RGVOutStream& put(const unsigned short*, size_t N) = 0;
  virtual RGVOutStream& put(const int*, size_t N) = 0;
  virtual RGVOutStream& put(const unsigned int*, size_t N) = 0;
  virtual RGVOutStream& put(const long*, size_t N) = 0;
  virtual RGVOutStream& put(const unsigned long*, size_t N) = 0;
  virtual RGVOutStream& put(const float*, size_t N) = 0;
  virtual RGVOutStream& put(const double*, size_t N) = 0;
};

inline RGVOutStream&  operator<<(RGVOutStream& str, RGVOutStream& (*fn)(RGVOutStream&)) {
  return (*fn)(str);   // this allows pointers to functions of output streams?  Do we need it?
}

RGVOutStream& flush(RGVOutStream&);

RGVInStream& operator>>(RGVInStream& s, char*& Input);
RGVInStream& operator>>(RGVInStream& s, short*& Input);
RGVInStream& operator>>(RGVInStream& s, int*& Input);
RGVInStream& operator>>(RGVInStream& s, long*& Input);
RGVInStream& operator>>(RGVInStream& s, float*& Input);
RGVInStream& operator>>(RGVInStream& s, double*& Input);
RGVInStream& operator>>(RGVInStream& s, unsigned char*& Input);
RGVInStream& operator>>(RGVInStream& s, unsigned short*& Input);
RGVInStream& operator>>(RGVInStream& s, unsigned int*& Input);
RGVInStream& operator>>(RGVInStream& s, unsigned long*& Input);

#endif /* _RGVSTREAM_H_ */
