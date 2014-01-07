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
*  FileName: rgintarray.cpp
*  Author:   Robert Goor
*
*/
//
//  class RGIntArray is a persistent class that represents an array of integers
//


#include "rgintarray.h"

PERSISTENT_DEFINITION (RGIntArray, _RGINTARRAY_, "IntegerArray")

int* RGIntArray::CompareList = new int [3];
size_t RGIntArray::CompareLength = 3;
Artifice RGIntArray::Art;

Artifice::Artifice() {

	RGIntArray::CompareList [0] = 0;
	RGIntArray::CompareList [1] = -1;
	RGIntArray::CompareList [2] = -1;
}


Artifice::~Artifice() {

	delete[] RGIntArray::CompareList;
}


RGIntArray :: RGIntArray () : RGPersistent (), length (1) {

	array = new int [1];
	array [0] = 0;
}


RGIntArray :: RGIntArray (size_t n) : RGPersistent (), length (n) {

	array = new int [n];

	for (size_t i=0; i<n; i++)
		array [i] = 0;
}


RGIntArray :: RGIntArray (const RGIntArray& a) : RGPersistent (a), length (a.length) {

	array = new int [length];

	for (size_t i=0; i<length; i++)
		array [i] = a.array [i];
}


RGIntArray :: RGIntArray (size_t n, int* iarray) : RGPersistent (), length (n) {

	array = new int [n];

	for (size_t i=0; i<n; i++)
		array [i] = iarray [i];
}


RGIntArray :: ~RGIntArray () {

	delete[] array;
}



RGIntArray& RGIntArray :: operator=(const RGIntArray& a) {

	if (this == &a)
		return *this;

	if (length != a.length) {
	
		delete[] array;
		length = a.length;
		array = new int [length];
	}

	for (size_t i=0; i<length; i++)
		array [i] = a.array [i];

	return *this;
}



void RGIntArray :: SetAndExpand (size_t n, int i) {

	ExpandToSize (n + 1);
	array [n] = i;
}


int& RGIntArray :: GetAndExpand (size_t n) {

	ExpandToSize (n + 1);
	return array [n];
}


void RGIntArray :: ExpandToSize (size_t N) {

	int* NewArray;

	if (length < N) {
    size_t i;
		NewArray = new int [N];

		for (i=0; i<length; i++)
			NewArray [i] = array [i];

		for (i=length; i<N; i++)
			NewArray [i] = 0;

		delete[] array;
		array = NewArray;
		length = N;
	}
}


int RGIntArray :: GraduatedCompareTo (const RGIntArray& a, const int* compareList) const {

	int i = 0;
	int j = compareList [0];
	int a1;
	int a2;

	while (j >= 0) {

		a1 = array [j];
		a2 = a.array [j];

		if (a1 < a2)
			return -1;

		if (a1 > a2)
			return 1;

		i++;
		j = compareList [i];
	}

	return 0;
}


Boolean RGIntArray :: GraduatedIsEqualTo (const RGIntArray& a, const int* compareList) const {

	int i = 0;
	int j = compareList [0];

	while (j >= 0) {

		if (array [j] != a.array [j])
			return FALSE;

		i++;
		j = compareList [i];
	}

	return TRUE;
}


size_t RGIntArray :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += sizeof (size_t) + length * sizeof (int);
	return size;
}


int RGIntArray :: CompareTo (const RGPersistent* p) const {

	RGIntArray* q = (RGIntArray*) p;
	int i = 0;
	int j = RGIntArray::CompareList[0];
	int a1, a2;

	while (j >= 0) {

		a1 = array [j];
		a2 = q->array [j];

		if (a1 < a2)
			return -1;

		if (a1 > a2)
			return 1;

		i++;
		j = RGIntArray::CompareList [i];
	}

	return 0;
}


unsigned RGIntArray :: HashNumber (unsigned long Base) const {

	return 0;
}


Boolean RGIntArray :: IsEqualTo (const RGPersistent* p) const {

	RGIntArray* q = (RGIntArray*) p;
	int i = 0;
	int j = RGIntArray::CompareList [0];

	while (j >= 0) {

		if (array [j] != q->array [j])
			return FALSE;

		i++;
		j = RGIntArray::CompareList [i];
	}

	return TRUE;
}


void RGIntArray :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	int temp;
//	f.Read (length);
	f.Read (temp);
	length = temp;
	f.Read (array, length);
}


void RGIntArray :: RestoreAll (RGVInStream& vin) {

	RGPersistent::RestoreAll (vin);
	int temp;
	vin >> temp;
//	vin >> length;
	length = temp;
	vin.Get (array, length);
}


void RGIntArray :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	f.Write ((unsigned int)length);
	f.Write (array, length);
}


void RGIntArray :: SaveAll (RGVOutStream& vout) const {

	RGPersistent::SaveAll (vout);
	vout << (unsigned int)length;
	vout.put (array, length);
}



size_t RGIntArray :: CompLength () {

	return CompareLength;
}


int& RGIntArray :: Get (size_t n) {

	ExpandTo (n + 1);
	return CompareList [n];
}


void RGIntArray :: ExpandTo (size_t N) {

	int* NewArray;

	if (CompareLength < N) {

		NewArray = new int [N];
    size_t i;
		for (i=0; i<CompareLength; i++)
			NewArray [i] = CompareList [i];

		for (i=CompareLength; i<N; i++)
			NewArray [i] = -1;

		delete[] CompareList;
		CompareList = NewArray;
		CompareLength = N;
	}
}
