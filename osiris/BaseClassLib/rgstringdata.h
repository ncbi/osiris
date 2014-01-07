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
*  FileName: rgstringdata.h
*  Author:   Robert Goor
*
*/
//
//  class StringData, which is the basis for the string classes.  It manages reference counting, etc.
//

#ifndef _STRINGDATA_H_
#define _STRINGDATA_H_

#include <cctype>

const int RGDEFAULT_SIZE = 17 * sizeof (unsigned long);   // should be multiple of sizeof(unsigned long)
const int RGDEFAULT_INCREMENT = 6 * sizeof (unsigned long);   // should be multiple of sizeof(unsigned long)


// A string helper class that implements reference counting for copy on write

class RGStringData {

public:
	RGStringData ();
	RGStringData (size_t size);
	RGStringData (const char* str);
	RGStringData (const char* str, size_t size);
	RGStringData (const RGStringData& str);
	RGStringData (const RGStringData& str, size_t size);
	~RGStringData ();

	RGStringData* MakeCopy ();
	void CopyData (const char*);

	operator char*() const { return Data; }  // gets data
	char* GetData() const { return Data; }

	int IncrementReferenceCount () { ReferenceCount++; return ReferenceCount;}
	int DecrementReferenceCount () { ReferenceCount--; return ReferenceCount;}
	int GetReferenceCount () const { return ReferenceCount; }

	size_t GetDataLength () const { return DataLength; }
	size_t WithoutCopyIncreaseSizeTo (size_t size);
	size_t IncreaseSizeTo (size_t size);
	size_t IncreaseSizeBy (size_t size);

	RGStringData* Append (const char*);
	void ResetData ();  // not safe to use unless know for sure reference count is 1

	void AppendCharacter (char, size_t);

	void* operator new (size_t);
	void operator delete (void*);

protected:
	char* Data;
	int ReferenceCount;
	size_t DataLength;

	size_t ComputeNewLength (size_t size);
};



#endif  /*  _STRINGDATA_H_  */


