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
*  FileName: rgstringdata.cpp
*  Author:   Robert Goor
*
*/
//
//  class StringData, which is the basis for the string classes.  It manages reference counting, etc.
//


#include <cstring>
#include <cctype>
#include <string>
#include <stdlib.h>

#include "rgstringdata.h"


RGStringData :: RGStringData () : ReferenceCount (1), DataLength (RGDEFAULT_SIZE) {

	Data = new char [DataLength];
	Data [0] = '\0';
}



RGStringData :: RGStringData (size_t size) : ReferenceCount (1) {

	DataLength = ComputeNewLength (size);
	Data = new char [DataLength];
	Data [0] = '\0';
}



RGStringData :: RGStringData (const char* str) : ReferenceCount (1) {

	size_t N = strlen (str) + 1;

	DataLength = ComputeNewLength (N);
	Data = new char [DataLength];
	strcpy (Data, str);
}



RGStringData :: RGStringData (const char* str, size_t size) : ReferenceCount (1) {

	size_t N = strlen (str) + 1;

	if (N < size)
		N = size;

	DataLength = ComputeNewLength (N);
	Data = new char [DataLength];
	strcpy (Data, str);
}



RGStringData :: RGStringData (const RGStringData& str) : ReferenceCount (1),
DataLength (str.DataLength) {

	Data = new char [DataLength];
	strcpy (Data, str.Data);
}


RGStringData :: RGStringData (const RGStringData& str, size_t size) : ReferenceCount (1) {

	size_t N = strlen (str.Data) + 1;

	if (N < size)
		N = size;

	DataLength = ComputeNewLength (N);
	Data = new char [DataLength];
	strcpy (Data, str.Data);
}



RGStringData :: ~RGStringData () {

	ReferenceCount--;

	if (ReferenceCount < -1) {

		Data = NULL;
	}

	else if (ReferenceCount <= 0) {

		delete [] Data;
		Data = NULL;
	}
}


void* RGStringData :: operator new (size_t nbytes) {
    
	return malloc (nbytes);
 }
 
 
void RGStringData :: operator delete (void* p)
{
  RGStringData* d = (RGStringData*) p;

  if (d->GetReferenceCount () <= 0) {
		
		free (p);
  }
 }


RGStringData* RGStringData :: MakeCopy () {

	RGStringData* NewData = (RGStringData*) this;
	ReferenceCount++;
	return NewData;
}


void RGStringData :: CopyData (const char* str) {

	if (Data != str) {

		if ((Data < str) && (str < Data + DataLength)) {

			char* DataPtr = Data;
			size_t L = strlen (str);
			char* strPtr = (char*)str;

			for (size_t i=0; i<L; i++) {
				
				*DataPtr = *strPtr;
				DataPtr++;
				strPtr++;
			}

			*DataPtr = '\0';
		}

		else {

			size_t N = strlen (str) + 1;
			WithoutCopyIncreaseSizeTo (N);
			strcpy (Data, str);
		}
	}
}


RGStringData* RGStringData :: Append (const char* str) {

	if ((Data <= str) && (str < Data + DataLength)) {

		size_t L = strlen (str);
		size_t M = strlen (Data);
		char* DataPtr = Data + M;
		char* strPtr = (char*)str;

		for (size_t i=0; i<L; i++) {
			
			*DataPtr = *strPtr;
			DataPtr++;
			strPtr++;
		}

		*DataPtr = '\0';
	}

	else
		strcat (Data, str);

	return this;
}


void RGStringData :: ResetData () {

	Data [0] = '\0';
}


void RGStringData :: AppendCharacter (char c, size_t location) {

	size_t N = location + 1;
	
	if (N >= DataLength)
		IncreaseSizeBy (RGDEFAULT_INCREMENT);

	Data [location] = c;
	Data [N] = '\0';
}



size_t RGStringData :: ComputeNewLength (size_t size) {

	size_t NewSize;
	size_t N;
	
	if (size <= RGDEFAULT_SIZE)
		NewSize = RGDEFAULT_SIZE;
	
	else {

		N = (size - RGDEFAULT_SIZE) / RGDEFAULT_INCREMENT;
		NewSize = N * RGDEFAULT_INCREMENT + RGDEFAULT_SIZE;

		if (NewSize < size)
			NewSize += RGDEFAULT_INCREMENT;
	}

	return NewSize;
}


size_t RGStringData :: IncreaseSizeTo (size_t size) {

	size_t NewSize = ComputeNewLength (size);
	char* NewData;

	if (NewSize > DataLength) {

		NewData = new char [NewSize];
		strcpy (NewData, Data);
		delete[] Data;
		Data = NewData;
		DataLength = NewSize;
	}
		
	return DataLength;
}


size_t RGStringData :: WithoutCopyIncreaseSizeTo (size_t size) {

	size_t NewSize = ComputeNewLength (size);
	char* NewData;

	if (NewSize > DataLength) {

		NewData = new char [NewSize];
		delete[] Data;
		Data = NewData;
		DataLength = NewSize;
	}
		
	return DataLength;
}



size_t RGStringData :: IncreaseSizeBy (size_t size) {

	return IncreaseSizeTo (size + DataLength);
}

