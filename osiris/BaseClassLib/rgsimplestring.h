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
*  FileName: rgsimplestring.h
*  Author:   Robert Goor
*
*/
//
// class RGSimpleString which is a non-persistent string class for use with XML tag name data, for example
//



#ifndef _RGSIMPLESTRING_H_
#define _RGSIMPLESTRING_H_

#include "rgdefs.h"
#include "rgstringdata.h"

#include <cstring>
#include <cctype>
#include <iostream>
#include <string>

class RGString;
class RGFile;
class RGVInStream;
class RGVOutStream;


using namespace std;


class RGSimpleString {

friend class RGString;

public:

	RGSimpleString ();
	RGSimpleString (const char*);
	RGSimpleString (const RGSimpleString&);
	RGSimpleString (const RGString&);
	RGSimpleString (size_t size);
	~RGSimpleString ();

	void ResizeLength (size_t size);

	const char* GetData () const { return Data->GetData(); }

	RGSimpleString& operator=(const char*);
	RGSimpleString& operator=(const RGString&);
	RGSimpleString& operator=(const RGSimpleString&);

	int CompareTo (const char*) const;
	int CompareTo (const RGSimpleString&) const;

	Boolean IsEmpty () const { return StringLength == 0; }
	size_t Length () const { return StringLength; }

	void WriteTextLine (RGFile&);  // writes a new line character after end

	size_t StoreSize () const;
	unsigned HashNumber (unsigned long Base) const;

	void RestoreAll (RGFile&);
	void RestoreAll (RGVInStream&);
	void SaveAll (RGFile&) const;
	void SaveAll (RGVOutStream&) const;

protected:

	RGStringData* Data;
	size_t StringLength;

	friend Boolean operator==(const RGSimpleString&, const char*);
	friend Boolean operator!=(const RGSimpleString&, const char*);

	size_t StringLengthOnDisk () const;
};


#endif  /*  _RGSIMPLESTRING_H_  */

