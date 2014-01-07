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
*  FileName: rgvstream.cpp
*  Author:   Robert Goor
*
*/
//
//	classes RGVInStream and RGVOutStream - abstract classes for streamed I/O operations
//


#include "rgvstream.h"

RGios::~RGios() {;}
RGvios::~RGvios() {;}
RGVOutStream::~RGVOutStream() {;}
RGVInStream::~RGVInStream() {;}

RGVOutStream& flush(RGVOutStream& str) { return str.flush();}

RGVInStream& operator>>(RGVInStream& str, char*& ptr) {

	ptr = new char;
	str.Get (*ptr);
	return str;
}

RGVInStream& operator>>(RGVInStream& str, short*& ptr) {

	ptr = new short;
	str >> *ptr;
	return str;
}

RGVInStream& operator>>(RGVInStream& str, int*& ptr) {

	ptr = new int;
	str >> *ptr;
	return str;
}

RGVInStream& operator>>(RGVInStream& str, long*& ptr) {

	ptr = new long;
	str >> *ptr;
	return str;
}

RGVInStream& operator>>(RGVInStream& str, float*& ptr) {

	ptr = new float;
	str >> *ptr;
	return str;
}

RGVInStream& operator>>(RGVInStream& str, double*& ptr) {

	ptr = new double;
	str >> *ptr;
	return str;
}

RGVInStream& operator>>(RGVInStream& str, unsigned char*& ptr) {

	ptr = new unsigned char;
	str.Get (*ptr);
	return str;
}

RGVInStream& operator>>(RGVInStream& str, unsigned short*& ptr) {

	ptr = new unsigned short;
	str >> *ptr;
	return str;
}

RGVInStream& operator>>(RGVInStream& str, unsigned int*& ptr) {

	ptr = new unsigned int;
	str >> *ptr;
	return str;
}

RGVInStream& operator>>(RGVInStream& str, unsigned long*& ptr) {

	ptr = new unsigned long;
	str >> *ptr;
	return str;
}
