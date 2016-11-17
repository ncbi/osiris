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
*  FileName: rgparray.cpp
*  Author:   Robert Goor
*
*/
//
// class RGPArray, which maintains arrays of arbitrary persistent objects -- see RGStringArray, which is similar,
// but for strings
//

#include "rgparray.h"
#include "rgnull.h"
#include "rgfile.h"
#include "rgvstream.h"

RGPARRAYDELETEOPTION RGPArray :: DeleteOption = RGDELETEALL;

PERSISTENT_DEFINITION (RGPArray, _RGPARRAY_, "PArray")


size_t RGPArray::DefaultSize = 50;
size_t RGPArray::DefaultIncrement = 50;


RGPArray :: RGPArray () : RGPersistent (), ArraySize (RGPArray::DefaultSize), 
CurrentLength (0) {
	// use default size

	pArray = new RGPersistent* [ArraySize];
	NullNewItems (0, ArraySize);
}



RGPArray :: RGPArray (size_t size) : RGPersistent (), ArraySize (RGPArray::DefaultSize), 
CurrentLength (0) {
	// specify size

	ArraySize = ComputeNewSize (size);
	pArray = new RGPersistent* [ArraySize];
	NullNewItems (0, ArraySize);
}



RGPArray :: ~RGPArray () {

	if (RGPArray::DeleteOption == RGDELETEALL) {
	
		for (size_t i=0; i<ArraySize; i++)
			delete pArray [i];
	}

	delete[] pArray;
}


RGPersistent* RGPArray :: GetElementAt(size_t i) const {
	
	if (i >= CurrentLength)
		return NULL;

	return pArray [i];
}


void RGPArray :: ReplaceElementAt (size_t i, RGPersistent* NewItem) {
	
	// performs limit checking and expands array if needed

	if (i >= CurrentLength) {

		if (i >= ArraySize - 1)
			BuildNewArray (i + 1);

		CurrentLength = i + 1;
	}

	delete pArray [i];
	pArray [i] = NewItem;
}


RGPersistent* RGPArray :: RemoveElementAt (size_t i) {

	RGPersistent* temp1;
	RGPersistent* temp2;

	if (i >= CurrentLength)
		return NULL;

	temp1 = pArray [i];
	temp2 = new RGNull;
	pArray [i] = temp2;
	return temp1;
}


void RGPArray :: Resize (size_t NewSize) {

	size_t i;
	size_t Size = ComputeTargetSize (NewSize);
	RGPersistent** NewArray;
	
	if (Size < ArraySize) {

		if (NewSize < CurrentLength) {

			for (i=NewSize; i<CurrentLength; i++) {

				delete pArray[i];
				pArray[i] = NULL;
			}

			CurrentLength = NewSize;
		}

		NewArray = new RGPersistent* [Size];

		for (i=0; i<Size; i++)
			NewArray[i] = pArray[i];

		delete[] pArray;
		pArray = NewArray;
		ArraySize = Size;
	}

	else
		BuildNewArray (NewSize);
}


void RGPArray :: Append (RGPersistent* NewItem) {

	BuildNewArray (CurrentLength + 1);
	delete pArray [CurrentLength];
	pArray [CurrentLength] = NewItem;

	CurrentLength++;
}


void RGPArray :: ResetArray () {

	for (size_t i=0; i<CurrentLength; i++) {

		delete pArray [i];
		pArray [i] = new RGNull;
	}

	CurrentLength = 0;
}



void RGPArray :: ResetDefaultSize (size_t NewSize) {

	DefaultSize = NewSize;
}


void RGPArray :: ResetDefaultIncrement (size_t Increment) {

	DefaultIncrement = Increment;
}


size_t RGPArray :: ComputeNewSize (size_t Target) {

	size_t Difference;
	size_t NumberOfDefaults;
	size_t AddedSpace;
	size_t Incr = RGPArray::DefaultIncrement;

	if (Target <= ArraySize)
		return ArraySize;

	Difference = Target - ArraySize;
	NumberOfDefaults = Difference / Incr;
	AddedSpace = Incr * NumberOfDefaults;

	if (AddedSpace < Difference)
		AddedSpace += Incr;

	return ArraySize + AddedSpace;
}


size_t RGPArray :: ComputeTargetSize (size_t Target) {

	size_t Difference;
	size_t NumberOfIncrements;
	size_t Incr = RGPArray::DefaultIncrement;
	size_t AddedSize;
	
	if (Target <= RGPArray::DefaultSize)
		return RGPArray::DefaultSize;

	Difference = Target - RGPArray::DefaultSize;
	NumberOfIncrements = Difference / Incr;
	AddedSize = NumberOfIncrements * Incr;

	if (AddedSize < Difference)
		AddedSize += Incr;

	return RGPArray::DefaultSize + AddedSize;
}



void RGPArray :: NullNewItems (size_t First, size_t Last) {

	RGPersistent* nil;
	
	for (size_t i=First; i<Last; i++) {

		nil = new RGNull;
		pArray [i] = nil;
	}
}



void RGPArray :: BuildNewArray (size_t size) {

	RGPersistent** NewArray;
	size_t NewSize = ComputeNewSize (size);

	if (NewSize > ArraySize) {

		NewArray = new RGPersistent* [NewSize];

		for (size_t i=0; i<CurrentLength; i++)
			NewArray [i] = pArray [i];

		delete[] pArray;
		pArray = NewArray;
		NullNewItems (CurrentLength, NewSize);
		ArraySize = NewSize;
	}
}



size_t RGPArray :: StoreSize () const {

	size_t size = RGPersistent::StoreSize () + sizeof (size_t);

	for (size_t i=0; i<ArraySize; i++)
		size += pArray[i]->StoreSize ();

	return size;
}



int RGPArray :: CompareTo (const RGPersistent* p) const {

	return 0;
}


unsigned RGPArray :: HashNumber (unsigned long Base) const {

	return 0;
}


Boolean RGPArray :: IsEqualTo (const RGPersistent* p) const {

	return FALSE;
}


void RGPArray :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	int temp;
	f.Read (temp);
//	f.Read (ArraySize);
	ArraySize = temp;
	BuildNewArray (ArraySize);

	for (size_t i=0; i<ArraySize; i++) {

		delete pArray [i];
		f >> pArray [i];
	}
}


void RGPArray :: RestoreAll (RGVInStream& vin) {

	RGPersistent::RestoreAll (vin);
	int temp;
	vin >> temp;
//	vin >> ArraySize;
	ArraySize = temp;
	BuildNewArray (ArraySize);

	for (size_t i=0; i<ArraySize; i++) {

		delete pArray [i];
		vin >> pArray [i];
	}
}


void RGPArray :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	f.Write ((unsigned int)ArraySize);

	for (size_t i=0; i<ArraySize; i++)
		pArray[i]->SaveAll (f);
}


void RGPArray :: SaveAll (RGVOutStream& vout) const {

	RGPersistent::SaveAll (vout);
	vout << (unsigned int)ArraySize;

	for (size_t i=0; i<ArraySize; i++)
		pArray[i]->SaveAll (vout);
}
