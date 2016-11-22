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
*  FileName: rgparray.h
*  Author:   Robert Goor
*
*/
//
// class RGPArray, which maintains arrays of arbitrary persistent objects -- see RGStringArray, which is similar,
// but for strings
//

#ifndef _RGPARRAY_H_
#define _RGPARRAY_H_

#include "rgdefs.h"
#include "rgpersist.h"
#include "rgstring.h"

const int _RGPARRAY_ = 6;

class RGFile;
class RGVInStream;
class RGVOutStream;

enum RGPARRAYDELETEOPTION {RGDELETEALL, RGDELETEARRAYONLY};

PERSISTENT_PREDECLARATION (RGPArray)


class RGPArray : public RGPersistent {

	PERSISTENT_DECLARATION(RGPArray)

public:
	RGPArray ();    // default size
	RGPArray (size_t size);   // specify size
	~RGPArray ();

	void Resize (size_t NewSize);
	void Append (RGPersistent* NewItem);
	void ResetArray ();

	RGPersistent* GetElementAt(size_t) const;  // performs limit checking and expands array if needed 
	void ReplaceElementAt (size_t index, RGPersistent* NewItem);  // performs limit checking and expands array if needed
	RGPersistent* RemoveElementAt (size_t);

	size_t Length () const { return CurrentLength; }

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	static void ResetDefaultSize (size_t NewSize);
	static void ResetDefaultIncrement (size_t Increment);
	static void SetDeleteAll () { DeleteOption = RGDELETEALL; }
	static void SetDeleteArrayOnly () { DeleteOption = RGDELETEARRAYONLY; }

protected:
	static size_t DefaultSize;
	static size_t DefaultIncrement;
	static RGPARRAYDELETEOPTION DeleteOption;

	size_t ArraySize;
	size_t CurrentLength;

	RGPersistent** pArray;

	size_t ComputeNewSize (size_t Target);
	size_t ComputeTargetSize (size_t Target);
	void NullNewItems (size_t First, size_t Last);
	void BuildNewArray (size_t size);
};


#endif  /*  _RGPARRAY_H_  */
