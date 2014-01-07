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
*  FileName: rgintarray.h
*  Author:   Robert Goor
*
*/
//
//  class RGIntArray is a persistent class that represents an array of integers
//

#ifndef _RGINTARRAY_H_
#define _RGINTARRAY_H_

#include "rgdefs.h"
#include "rgpersist.h"
#include "rgfile.h"
#include "rgvstream.h"

const int _RGINTARRAY_ = 10;

class Artifice {
 public:
	Artifice ();
	~Artifice ();
};


PERSISTENT_PREDECLARATION (RGIntArray)

class RGIntArray : public RGPersistent {

	PERSISTENT_DECLARATION (RGIntArray)

	friend class Artifice;

public:
	RGIntArray ();
	RGIntArray (size_t n);
	RGIntArray (const RGIntArray&);
	RGIntArray (size_t n, int* i);
	~RGIntArray ();

	RGIntArray& operator=(const RGIntArray&);
	int& operator[](size_t n) { return array[n]; }  // no bounds checking
	size_t Length () const { return length; }

	void SetAndExpand (size_t n, int i);  // expands if necessary
	int& GetAndExpand (size_t n);   // expands if necessary
	void ExpandToSize (size_t N);

	int GraduatedCompareTo (const RGIntArray& a, const int* compareList) const;
	Boolean GraduatedIsEqualTo (const RGIntArray& a, const int* compareList) const;

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	static size_t CompLength ();
	static int& Get (size_t n);
	static void ExpandTo (size_t N);

protected:
	int* array;
	size_t length;

	static int* CompareList;
	static size_t CompareLength;
	static Artifice Art;
};


#endif  /*  _RGINTARRAY_H_  */
