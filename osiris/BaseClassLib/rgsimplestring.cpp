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
*  FileName: rgsimplestring.cpp
*  Author:   Robert Goor
*
*/
//
// class RGSimpleString, inherits from RGPersistent and manages strings
//

//#include <stdlib.h>

#include "rgsimplestring.h"
#include "rgstring.h"
#include "rgfile.h"
#include "rgvstream.h"



RGSimpleString :: RGSimpleString () : StringLength (0) {

	Data = new RGStringData;
}


RGSimpleString :: RGSimpleString (const char* str) {

	Data = new RGStringData (str);
	StringLength = strlen (str);
}



RGSimpleString :: RGSimpleString (const RGSimpleString& str) : StringLength (str.StringLength) {

	Data = (str.Data)->MakeCopy ();
}


RGSimpleString :: RGSimpleString (const RGString& str) : StringLength (str.StringLength) {

	Data = (str.Data)->MakeCopy ();
}



RGSimpleString :: RGSimpleString (size_t size) : StringLength (0) {

	Data = new RGStringData (size);
}



RGSimpleString :: ~RGSimpleString () {

	delete Data;
}


void RGSimpleString :: ResizeLength (size_t size) {

	RGStringData* NewData = new RGStringData (*Data, size);
	delete Data;
	Data = NewData;
}



RGSimpleString& RGSimpleString :: operator=(const char* str) {

	if (Data->GetReferenceCount () == 1)
		Data->CopyData (str);
	
	else {

		delete Data;
		Data = new RGStringData (str);
	}

	StringLength = strlen (str);
	return *this;
}



RGSimpleString& RGSimpleString :: operator=(const RGSimpleString& str) {

	if (Data == str.Data)
		return *this;
	
	delete Data;
	Data = (str.Data)->MakeCopy ();
	StringLength = str.StringLength;
	return *this;
}


RGSimpleString& RGSimpleString :: operator=(const RGString& str) {

	if (Data == str.Data)
		return *this;
	
	delete Data;
	Data = (str.Data)->MakeCopy ();
	StringLength = str.StringLength;
	return *this;
}



int RGSimpleString :: CompareTo (const char* str) const {

	char* MyData = Data->GetData ();
	return strcmp (MyData, str);
}



int RGSimpleString :: CompareTo (const RGSimpleString& str) const {

	char* MyData = Data->GetData ();
	return strcmp (MyData, str.GetData());
}


void RGSimpleString :: WriteTextLine (RGFile& f) {

	f.Write (GetData (), StringLength);
	f.Write ("\n", 1);
}


size_t RGSimpleString :: StringLengthOnDisk () const {

	size_t N = sizeof (unsigned long);
	size_t Length1 = StringLength + 1;
	size_t NumberOfInts = Length1 / N;

	if (N * NumberOfInts < Length1)
		NumberOfInts++;

	return N * NumberOfInts;
}



size_t RGSimpleString :: StoreSize () const {

	size_t N = sizeof (size_t);
	size_t Size = N + StringLengthOnDisk ();
	return Size;
}


unsigned RGSimpleString :: HashNumber (unsigned long Base) const {

	//
	//  This produces a hash number between 0 and Base - 1, inclusive
	//
	
	unsigned long Total = 0;
	size_t i = 0;
	size_t n = Length ();
	unsigned char* MyData = (unsigned char*)Data->GetData ();

	while (i < n) {

		Total = (Base * Total + *MyData)%Base;
		MyData++;
		i++;
	}

	return Total;
}


void RGSimpleString :: RestoreAll (RGFile& f) {

	RGStringData* NewData;
	int temp;
	f.Read (temp);
//	f.Read (StringLength);
	StringLength = temp;

	size_t N = StringLengthOnDisk () / sizeof (unsigned long);

	if (Data->GetReferenceCount () > 1) {

		NewData = new RGStringData (StringLength + 1);
		delete Data;
		Data = NewData;
	}

	else
		Data->IncreaseSizeTo (StringLength + 1);

	char* MyData = Data->GetData ();
	unsigned long* MyLongData = (unsigned long*) MyData;
	f.Read (MyLongData, N);
}



void RGSimpleString :: RestoreAll (RGVInStream& vin) {

	
	RGStringData* NewData;
	int temp;
	vin >> temp;
//	vin >> StringLength;
	StringLength = temp;

	size_t N = StringLengthOnDisk () / sizeof (unsigned long);

	if (Data->GetReferenceCount () > 1) {

		NewData = new RGStringData (StringLength + 1);
		delete Data;
		Data = NewData;
	}

	else
		Data->IncreaseSizeTo (StringLength + 1);

	char* MyData = Data->GetData ();
	unsigned long* MyLongData = (unsigned long*) MyData;
	vin.Get (MyLongData, N);
}



void RGSimpleString :: SaveAll (RGFile& f) const {

	char* MyData = Data->GetData ();
	unsigned long* MyLongData = (unsigned long*) MyData;
	size_t N = StringLengthOnDisk () / sizeof (unsigned long);

	f.Write ((unsigned int)StringLength);
	f.Write (MyLongData, N);
}



void RGSimpleString :: SaveAll (RGVOutStream& vout) const {

	char* MyData = Data->GetData ();
	unsigned long* MyLongData = (unsigned long*) MyData;
	size_t N = StringLengthOnDisk ();
	size_t NumberOfInts = N / sizeof (unsigned long);

	vout << (unsigned int)StringLength;
	vout.put (MyLongData, NumberOfInts);
}


Boolean operator==(const RGSimpleString& s1, const char* s2) {

	return s1.CompareTo (s2) == 0;
}


Boolean operator!=(const RGSimpleString& s1, const char* s2) {

	return s1.CompareTo (s2) != 0;
}

