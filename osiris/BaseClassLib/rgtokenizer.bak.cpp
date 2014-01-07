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
*  FileName: rgtokenizer.bak.cpp
*  Author:   Robert Goor
*
*/
//
// class RGStringTokenizer which extracts tokens from an RGString
//

#include "rgtokenizer.h"

#include <stdlib.h>
#include <math.h>


size_t RGStringArray::DefaultSize = 50;
size_t RGStringArray::DefaultIncrement = 50;
RGString RGStringArray::NullString;


RGStringTokenizer :: RGStringTokenizer (RGString* str) : Str (str) {

	StringData = (Str->Data)->GetData ();
	CurrentPtr = StringData;
	it = new RGDListIterator (DelimiterList);
	itRemove = new RGDListIterator (RemoveList);
}


RGStringTokenizer :: ~RGStringTokenizer () {

	delete it;
}


void RGStringTokenizer :: ClearTokens () {

	DelimiterList.ClearAndDelete ();
}


void RGStringTokenizer :: AddDelimiter (const RGString& str) {

	RGString* NewToken = new RGString (str);
	DelimiterList.Append (NewToken);
}


void RGStringTokenizer :: AddDelimiter (const char* str) {

	RGString* NewToken = new RGString (str);
	DelimiterList.Append (NewToken);
}


void RGStringTokenizer :: AddDelimiter (char c) {

	char str[2];
	str [0] = c;
	str [1] = '\0';
	RGString* NewToken = new RGString (str);
	DelimiterList.Append (NewToken);
}


void RGStringTokenizer :: AddDelimiters (RGDList& strlist) {

	DelimiterList = strlist;
}


void RGStringTokenizer :: ClearRemoveItems () {

	RemoveList.ClearAndDelete ();
}



void RGStringTokenizer :: AddRemoveItem (char c) {

	char str[2];
	str [0] = c;
	str [1] = '\0';
	RGString* NewToken = new RGString (str);
	RemoveList.Append (NewToken);
}



Boolean RGStringTokenizer :: CompareForDelimiterAtPointer (const RGString* delim, char* ptr) {

	//  starts at ptr in StringData and compares against the string delim
	
	int Length = delim->Length ();
	char* q = ptr;
	const char* p = delim->GetData ();
	
	for (int i=0; i<Length; i++) {

		if ((*q == '\0') || (*p != *q))
			return FALSE;

		p++;
		q++;
	}

	return TRUE;
}



RGString* RGStringTokenizer :: LookForAnyDelimiterAtPointer (char* ptr) {

	RGString* NextDelim;
	it->Reset ();

	while (NextDelim = (RGString*)(*it)()) {

		if (CompareForDelimiterAtPointer (NextDelim, ptr)) {

			return NextDelim;
		}
	}

	return NULL;
}



Boolean RGStringTokenizer :: FirstTokenIsNull (RGString*& delim) {

	it->Reset ();
	RGString* Next;

	while (Next = (RGString*)(*it)()) {

		if (CompareForDelimiterAtPointer (Next, StringData)) {
			
			delim = Next;
			return TRUE;
		}
	}

	return FALSE;
}



RGString* RGStringTokenizer :: SkipLeadingDelimiter () {

	RGString* delim;
	int Length;

	if (FirstTokenIsNull (delim)) {

		Length = delim->Length ();
		CurrentPtr = StringData + Length;
		return delim;
	}

	return NULL;
}



Boolean RGStringTokenizer :: NextToken (RGString& Token, RGString& Delimiter) {

	RGString* delim;
	char CurrentCharacter = *CurrentPtr;
	Token = "";
	Delimiter = "";
	Boolean IsNewToken = TRUE;

	while (CurrentCharacter != '\0') {

		delim = LookForAnyDelimiterAtPointer (CurrentPtr);

		if (delim) {

			CurrentPtr += delim->Length ();
			Delimiter = *delim;
			return IsNewToken;
		}

		if (!CharacterIsInRemoveList (CurrentCharacter))
			Token.Append (CurrentCharacter);

		CurrentPtr++;
		CurrentCharacter = *CurrentPtr;
	}

	if (Token.Length () == 0)
		IsNewToken = FALSE;

	return IsNewToken;
}



void RGStringTokenizer :: ResetTokenizer () {

	CurrentPtr = StringData;
}


void RGStringTokenizer :: ResetString (RGString* NewStr) {

	Str = NewStr;
	StringData = (Str->Data)->GetData ();
	CurrentPtr = StringData;
}


RGStringArray* RGStringTokenizer :: Split (RGStringArray*& Delimiters) {

	// assumes that delimiter definitions have already been set

	ResetTokenizer ();
	RGStringArray* NewTokenArray = new RGStringArray;
	Delimiters = new RGStringArray;
	RGString NewToken;
	RGString NextDelimiter;

	while (NextToken (NewToken, NextDelimiter)) {

		NewTokenArray->Append (NewToken);
		Delimiters->Append (NextDelimiter);
	}

	return NewTokenArray;
}


RGString* RGStringTokenizer :: Join (const RGStringArray& s1, const RGStringArray& s2) {

	(*Str) = "";
	size_t i;
	size_t Len = s1.Length ();

	if (s2.Length() > Len)
		Len = s2.Length ();

	for (i=0; i<Len; i++) {

		if (s1 [i] != NULL)
			(*Str)+= s1[i];

		if (s2 [i] != NULL)
			(*Str)+= s2[i]; 
	}

	return Str;
}


Boolean RGStringTokenizer :: CharacterIsInRemoveList (char c) {

	RGString* NextRemove;
	itRemove->Reset ();
	char a;

	while (NextRemove = (RGString*)(*itRemove)()) {

		a = (*NextRemove)[0];

		if (a == c)
			return TRUE;
	}

	return FALSE;
}


RGStringArray :: RGStringArray () : ArraySize (RGStringArray::DefaultSize), 
CurrentLength (0) {
	// use default size

	StringArray = new RGString* [ArraySize];
	NullNewItems (0, ArraySize);
}



RGStringArray :: RGStringArray (size_t size) : ArraySize (RGStringArray::DefaultSize), 
CurrentLength (0) {
	// specify size

	ArraySize = ComputeNewSize (size);
	StringArray = new RGString* [ArraySize];
	NullNewItems (0, ArraySize);
}



RGStringArray :: ~RGStringArray () {

	for (size_t i=0; i<ArraySize; i++)
		delete StringArray [i];

	delete[] StringArray;
}

RGString RGStringArray :: operator[](size_t i) const {
	
	// performs limit checking but does not expand array if needed

	if ((i >= CurrentLength) || (StringArray[i] == NULL))
		return RGStringArray::NullString;

	return *(StringArray [i]);
}



RGString& RGStringArray :: operator[](size_t i) {
	
	// performs limit checking and expands array if needed; deletes replaced item

	if (i >= CurrentLength) {

		if (i >= ArraySize - 1)
			BuildNewArray (i + 1);

		CurrentLength = i + 1;
		StringArray[i] = new RGString;
	}

	else if (StringArray[i] == NULL)
		StringArray[i] = new RGString;

	return *(StringArray[i]);
}


void RGStringArray :: Resize (size_t NewSize) {

	size_t i;
	size_t Size = ComputeTargetSize (NewSize);
	RGString** NewArray;
	
	if (Size < ArraySize) {

		if (NewSize < CurrentLength) {

			for (i=NewSize; i<CurrentLength; i++) {

				delete StringArray[i];
				StringArray[i] = NULL;
			}

			CurrentLength = NewSize;
		}

		NewArray = new RGString* [Size];

		for (i=0; i<Size; i++)
			NewArray[i] = StringArray[i];

		delete[] StringArray;
		StringArray = NewArray;
		ArraySize = Size;
	}

	else
		BuildNewArray (NewSize);
}


void RGStringArray :: Append (const RGString& NewItem) {

	BuildNewArray (CurrentLength + 1);

	if (StringArray [CurrentLength] == NULL)
		StringArray [CurrentLength] = new RGString (NewItem);

	else
		*(StringArray [CurrentLength]) = NewItem;

	CurrentLength++;
}


void RGStringArray :: Append (const char* NewItem) {

	RGString* NewString = new RGString (NewItem);
	BuildNewArray (CurrentLength + 1);
	StringArray [CurrentLength] = NewString;
	CurrentLength++;
}



void RGStringArray :: Replace (size_t index, const RGString& item) {
	
	// performs limit checking and expands array if needed

	if (index >= CurrentLength) {

		if (index >= ArraySize - 1)
			BuildNewArray (index + 1);

		CurrentLength = index + 1;
	}

	if (StringArray[index] == NULL)
		StringArray [index] = new RGString (item);

	else
		*(StringArray[index]) = item;
}



void RGStringArray :: Replace (size_t index, const char* NewItem) {

	// performs limit checking and expands array if needed

	if (index >= CurrentLength) {

		if (index >= ArraySize - 1)
			BuildNewArray (index + 1);

		CurrentLength = index + 1;
	}

	if (StringArray[index] == NULL)
		StringArray [index] = new RGString (NewItem);

	else
		*(StringArray[index]) = NewItem;
}



int RGStringArray :: GetIntegerValue (size_t index) const {

	if ((index >= CurrentLength) || (StringArray [index] == NULL))
		return NAN;

	return atoi (StringArray[index]->GetData());
}



double RGStringArray :: GetDoubleValue (size_t index) const {

	if ((index >= CurrentLength) || (StringArray [index] == NULL))
		return dNAN;

	return atof (StringArray[index]->GetData());
}



void RGStringArray :: ResetDefaultSize (size_t NewSize) {

	DefaultSize = NewSize;
}


void RGStringArray :: ResetDefaultIncrement (size_t Increment) {

	DefaultIncrement = Increment;
}


size_t RGStringArray :: ComputeNewSize (size_t Target) {

	size_t Difference;
	size_t NumberOfDefaults;
	size_t AddedSpace;
	size_t Incr = RGStringArray::DefaultIncrement;

	if (Target <= ArraySize)
		return ArraySize;

	Difference = Target - ArraySize;
	NumberOfDefaults = Difference / Incr;
	AddedSpace = Incr * NumberOfDefaults;

	if (AddedSpace < Difference)
		AddedSpace += Incr;

	return ArraySize + AddedSpace;
}


size_t RGStringArray :: ComputeTargetSize (size_t Target) {

	size_t Difference;
	size_t NumberOfIncrements;
	size_t Incr = RGStringArray::DefaultIncrement;
	size_t AddedSize;
	
	if (Target <= RGStringArray::DefaultSize)
		return RGStringArray::DefaultSize;

	Difference = Target - RGStringArray::DefaultSize;
	NumberOfIncrements = Difference / Incr;
	AddedSize = NumberOfIncrements * Incr;

	if (AddedSize < Difference)
		AddedSize += Incr;

	return RGStringArray::DefaultSize + AddedSize;
}



void RGStringArray :: NullNewItems (size_t First, size_t Last) {

	for (size_t i=First; i<Last; i++)
		StringArray [i] = NULL;
}



void RGStringArray :: BuildNewArray (size_t size) {

	RGString** NewArray;
	size_t NewSize = ComputeNewSize (size);

	if (NewSize > ArraySize) {

		NewArray = new RGString* [NewSize];

		for (size_t i=0; i<CurrentLength; i++)
			NewArray [i] = StringArray [i];

		delete[] StringArray;
		StringArray = NewArray;
		NullNewItems (CurrentLength, NewSize);
		ArraySize = NewSize;
	}
}
