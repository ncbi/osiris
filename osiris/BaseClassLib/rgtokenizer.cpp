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
*  FileName: rgtokenizer.cpp
*  Author:   Robert Goor
*
*/
//
// class RGStringTokenizer which extracts tokens from an RGString
//

#include "rgtokenizer.h"

#include <cstdlib>


size_t RGStringArray::DefaultSize = 50;
size_t RGStringArray::DefaultIncrement = 50;
RGString RGStringArray::NullString;


RGStringTokenizer :: RGStringTokenizer (RGString& str) : Str (str) {

	StringData = (Str.Data)->GetData ();
	CurrentPtr = StringData;
	it = new RGDListIterator (DelimiterList);
	itRemove = new RGDListIterator (RemoveList);
}


RGStringTokenizer :: ~RGStringTokenizer () {

	delete it;
}


void RGStringTokenizer :: ClearDelimiters () {

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
	
	size_t Length = delim->Length ();
	char* q = ptr;
	const char* p = delim->GetData ();
	
	for (size_t i=0; i<Length; i++) {

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
	size_t Length;

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

	StringData = (Str.Data)->GetData ();
	CurrentPtr = StringData;
}


void RGStringTokenizer :: ResetString (RGString& NewStr) {

	Str = NewStr;
	StringData = (Str.Data)->GetData ();
	CurrentPtr = StringData;
}


void RGStringTokenizer :: Split (RGStringArray& Tokens, RGStringArray& Delimiters) {

	// assumes that delimiter definitions have already been set

	ResetTokenizer ();
	Tokens.ResetArray ();
	Delimiters.ResetArray ();
	RGString NewToken;
	RGString NextDelimiter;

	while (NextToken (NewToken, NextDelimiter)) {

		Tokens.Append (NewToken);
		Delimiters.Append (NextDelimiter);
	}
}


void RGStringTokenizer :: Join (RGString& Answer, const RGStringArray& s1, const RGStringArray& s2) {

	Answer = "";
	size_t i;
	size_t Len = s1.Length ();

	if (s2.Length() > Len)
		Len = s2.Length ();

	for (i=0; i<Len; i++) {

		if (s1 [i] != NULL)
			Answer+= s1[i];

		if (s2 [i] != NULL)
			Answer+= s2[i]; 
	}

	ResetTokenizer ();
}


Boolean RGStringTokenizer :: CharacterIsInRemoveList (char c) {

	RGString* NextRemove;
	itRemove->Reset ();
	char a;

	while (NextRemove = (RGString*)(*itRemove)()) {

		a = NextRemove->GetCharacter (0);

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


void RGStringArray :: ResizeUp (size_t NewSize) {

	size_t Size = ComputeTargetSize (NewSize);
	
	if (Size > ArraySize)
		BuildNewArray (NewSize);
}


void RGStringArray :: ResetLength (size_t NewLength) {

	size_t i;
	
	if (NewLength < CurrentLength) {

		for (i=NewLength; i<CurrentLength; i++) {

			delete StringArray[i];
			StringArray[i] = NULL;
		}
	}

	else if (NewLength > CurrentLength) {

		Resize (NewLength);
	}

	CurrentLength = NewLength;
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


void RGStringArray :: ResetArray () {

	for (size_t i=0; i<CurrentLength; i++) {

		delete StringArray [i];
		StringArray [i] = NULL;
	}

	CurrentLength = 0;
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
		return rgNAN;

	return (int)strtol (StringArray[index]->GetData(), (char**)NULL, 10);  //atoi (StringArray[index]->GetData());
}



double RGStringArray :: GetDoubleValue (size_t index) const {

	if ((index >= CurrentLength) || (StringArray [index] == NULL))
		return dNAN;

	return strtod (StringArray[index]->GetData(), (char**)NULL);  //atof (StringArray[index]->GetData());
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


RGStringSearch :: RGStringSearch (const RGString& target, RGString& search) : Target (target), SearchString (search), CurrentOffset (0) {

	TLength = Target.Length ();
	TLength1 = TLength - 1;
	TargetData = (char*) Target.GetData ();
	TargetEnd = TargetData + TLength1;
	StringData = (char*)SearchString.GetData ();
  size_t i;
	for (i=0; i<256; i++)
		Skip [i] = TLength;

	unsigned char Index;
	unsigned char Last = *((unsigned char*)TargetEnd);
	SkipLast = TLength;

	for (i=0; i<TLength1; i++) {

		Index = *((unsigned char*)(TargetData + i));
		Skip [Index] = TLength1 - i;
	}

	SkipLast = Skip [Last];
	Skip [Last] = 0;
}



RGStringSearch :: ~RGStringSearch () {

}


void RGStringSearch :: ResetSearch () {

	StringData = (char*)SearchString.GetData ();
	CurrentOffset = 0;
}



Boolean RGStringSearch :: FindNext (size_t StartOffset, size_t& FoundBeginOffset, size_t& FoundEndOffset) {

	if (StringData == NULL)
		return FALSE;

	if (TLength == 0)
		return FALSE;

	size_t SLength = SearchString.Length ();
	size_t CurrentOffset = StartOffset;
	size_t Jump;
	char* CurrentTestSite; 
	size_t CurrentTestOffset;

	while (CurrentOffset + TLength1 < SLength) {

		CurrentTestOffset = CurrentOffset + TLength1;
		CurrentTestSite = StringData + CurrentTestOffset;
		Jump = Skip [*((unsigned char*)CurrentTestSite)];

		if (Jump == 0) {

			if (TestForMatch (CurrentOffset)) {

				FoundBeginOffset = CurrentOffset;
				FoundEndOffset = CurrentOffset + TLength1;
				return TRUE;
			}

			Jump = SkipLast;
		}

		CurrentOffset += Jump;
	}

	return FALSE;
}



Boolean RGStringSearch :: FindAndReplaceNext (size_t StartOffset, size_t& FoundEndOffset, const RGString& replace) {

	size_t FoundBeginOffset;
	
	if (!FindNext (StartOffset, FoundBeginOffset, FoundEndOffset))
		return FALSE;

	if (TLength == 0)
		return FALSE;

	SearchString.ReplaceSubstring (FoundBeginOffset, FoundEndOffset, replace);
	ResetPointer ();
	return TRUE;
}



Boolean RGStringSearch :: FindAndReplaceAll (const RGString& replace) {

	size_t StartOffset = 0;
	size_t FoundEndOffset = SearchString.Length () - 1;
	Boolean ReturnValue = FALSE;

	while (FindAndReplaceNext (StartOffset, FoundEndOffset, replace)) {

		StartOffset = FoundEndOffset;
		ReturnValue = TRUE;
	}

	return ReturnValue;
}


Boolean RGStringSearch :: FindNext (size_t& FoundBeginOffset, size_t& FoundEndOffset) {

	if (StringData == NULL)
		return FALSE;

	if (TLength == 0)
		return FALSE;

	size_t SLength = SearchString.Length ();
	size_t Jump;
	char* CurrentTestSite; 
	size_t CurrentTestOffset;

	while (CurrentOffset + TLength1 < SLength) {

		CurrentTestOffset = CurrentOffset + TLength1;
		CurrentTestSite = StringData + CurrentTestOffset;
		Jump = Skip [*((unsigned char*)CurrentTestSite)];

		if (Jump == 0) {

			if (TestForMatch (CurrentOffset)) {

				FoundBeginOffset = CurrentOffset;
				FoundEndOffset = CurrentOffset + TLength1;
				CurrentOffset = FoundEndOffset + 1;
				return TRUE;
			}

			Jump = SkipLast;
		}

		CurrentOffset += Jump;
	}

	return FALSE;
}


Boolean RGStringSearch :: FindAndReplaceNext (size_t& FoundEndOffset, const RGString& replace) {

	size_t FoundBeginOffset;
	
	if (!FindNext (FoundBeginOffset, FoundEndOffset))
		return FALSE;

	if (TLength == 0)
		return FALSE;

	SearchString.ReplaceSubstring (FoundBeginOffset, FoundEndOffset, replace);
	CurrentOffset = FoundEndOffset;
	ResetPointer ();
	return TRUE;
}


Boolean RGStringSearch :: TestForMatch (size_t offset) {

	char* CurrentS = StringData + offset;
	char* CurrentT = TargetData;

	while (CurrentT <= TargetEnd) {

		if (*CurrentS != *CurrentT)
			return FALSE;

		CurrentS++;
		CurrentT++;
	}

	return TRUE;
}


Boolean RGStringSearch :: TestForMatch (size_t offset, RGString& NewSearchString) {

	char* CurrentS = (char*) (NewSearchString.GetData () + offset);
	char* CurrentT = TargetData;

	while (CurrentT <= TargetEnd) {

		if (*CurrentS != *CurrentT)
			return FALSE;

		CurrentS++;
		CurrentT++;
	}

	return TRUE;
}


void RGStringSearch :: ResetPreview () {

	PreviewOffset = CurrentOffset;
}


void RGStringSearch :: ResetPreviewToBeginning () {

	PreviewOffset = 0;
}


Boolean RGStringSearch :: PreviewNext (size_t& FoundBeginOffset, size_t& FoundEndOffset, RGString& NewSearchString) {

	if (TLength == 0)
		return FALSE;

	size_t SLength = NewSearchString.Length ();
	char* PreviewSource = (char*)NewSearchString.GetData ();
	size_t Jump;
	char* CurrentTestSite; 
	size_t CurrentTestOffset;

	while (PreviewOffset + TLength1 < SLength) {

		CurrentTestOffset = PreviewOffset + TLength1;
		CurrentTestSite = PreviewSource + CurrentTestOffset;
		Jump = Skip [*((unsigned char*)CurrentTestSite)];

		if (Jump == 0) {

			if (TestForMatch (PreviewOffset, NewSearchString)) {

				FoundBeginOffset = PreviewOffset;
				FoundEndOffset = PreviewOffset + TLength1;
				PreviewOffset = FoundEndOffset + 1;
				return TRUE;
			}

			Jump = SkipLast;
		}

		PreviewOffset += Jump;
	}

	return FALSE;
}


Boolean RGStringSearch :: PreviewNext (size_t StartOffset, size_t& FoundBeginOffset, size_t& FoundEndOffset, RGString& NewSearchString) {

	if (TLength == 0)
		return FALSE;

	size_t SLength = NewSearchString.Length ();
	char* PreviewSource = (char*)NewSearchString.GetData ();
	size_t LocalOffset = StartOffset;
	size_t Jump;
	char* CurrentTestSite; 
	size_t CurrentTestOffset;

	while (LocalOffset + TLength1 < SLength) {

		CurrentTestOffset = LocalOffset + TLength1;
		CurrentTestSite = PreviewSource + CurrentTestOffset;
		Jump = Skip [*((unsigned char*)CurrentTestSite)];

		if (Jump == 0) {

			if (TestForMatch (LocalOffset, NewSearchString)) {

				FoundBeginOffset = LocalOffset;
				FoundEndOffset = LocalOffset + TLength1;
				return TRUE;
			}

			Jump = SkipLast;
		}

		LocalOffset += Jump;
	}

	return FALSE;
}


void RGStringSearch :: ResetPointer () {

	StringData = (char*)SearchString.GetData ();
}


RGBracketStringSearch :: RGBracketStringSearch (const RGString& left, const RGString& right, RGString& search) : 
SearchLeft (left, search), SearchRight (right, search), SearchString (search), CurrentOffset (0) {

}



RGBracketStringSearch :: ~RGBracketStringSearch () {

}



void RGBracketStringSearch :: ResetSearch () {

	SearchLeft.ResetSearch ();
	SearchRight.ResetSearch ();
	CurrentOffset = 0;
}


Boolean RGBracketStringSearch :: FindNextBracketedString (size_t StartOffset, size_t& FoundEndOffset, 
														  RGString& includedString) {

	size_t FoundBegin;
	size_t FoundLeftEnd;
	size_t FoundRightBegin;
	Boolean Found = SearchLeft.FindNext (StartOffset, FoundBegin, FoundLeftEnd);

	if (!Found)
		return FALSE;

    FoundLeftEnd++;
	Found = SearchRight.FindNext (FoundLeftEnd, FoundRightBegin, FoundEndOffset);

	if (!Found)
		return FALSE;

	if (FoundRightBegin == FoundLeftEnd)
		includedString = "";

	else
		includedString = SearchString.ExtractSubstring (FoundLeftEnd, FoundRightBegin - 1);

	return TRUE;
}



Boolean RGBracketStringSearch :: FindNextNonemptyBracketedString (size_t StartOffset, size_t& FoundEndOffset, 
																  RGString& includedString) {

	size_t FoundBegin;
	size_t FoundLeftEnd;
	size_t FoundRightBegin;
	Boolean Found;
	size_t Start = StartOffset;

	while (TRUE) {
		
		Found = SearchLeft.FindNext (Start, FoundBegin, FoundLeftEnd);

		if (!Found)
			return FALSE;

		FoundLeftEnd++;
		Found = SearchRight.FindNext (FoundLeftEnd, FoundRightBegin, FoundEndOffset);

		if (!Found)
			return FALSE;

		if (FoundRightBegin != FoundLeftEnd) {

			includedString = SearchString.ExtractSubstring (FoundLeftEnd, FoundRightBegin - 1);
			return TRUE;
		}

		Start = FoundEndOffset + 1;
	}

	return FALSE;
}


Boolean RGBracketStringSearch :: FindNextBracketedString (size_t& FoundEndOffset, RGString& includedString) {

	size_t FoundBegin;
	size_t FoundLeftEnd;
	size_t FoundRightBegin;
	Boolean Found = SearchLeft.FindNext (CurrentOffset, FoundBegin, FoundLeftEnd);

	if (!Found)
		return FALSE;

    FoundLeftEnd++;
	Found = SearchRight.FindNext (FoundLeftEnd, FoundRightBegin, FoundEndOffset);

	if (!Found)
		return FALSE;

	if (FoundRightBegin == FoundLeftEnd)
		includedString = "";

	else
		includedString = SearchString.ExtractSubstring (FoundLeftEnd, FoundRightBegin - 1);

	CurrentOffset = FoundEndOffset + 1;
	return TRUE;
}


Boolean RGBracketStringSearch :: FindNextNonemptyBracketedString (size_t& FoundEndOffset, RGString& includedString) {


	size_t FoundBegin;
	size_t FoundLeftEnd;
	size_t FoundRightBegin;
	Boolean Found;
	size_t Start = CurrentOffset;

	while (TRUE) {
		
		Found = SearchLeft.FindNext (Start, FoundBegin, FoundLeftEnd);

		if (!Found)
			return FALSE;

		FoundLeftEnd++;
		Found = SearchRight.FindNext (FoundLeftEnd, FoundRightBegin, FoundEndOffset);

		if (!Found)
			return FALSE;

		CurrentOffset = FoundEndOffset + 1;
		
		if (FoundRightBegin != FoundLeftEnd) {

			includedString = SearchString.ExtractSubstring (FoundLeftEnd, FoundRightBegin - 1);
			return TRUE;
		}

		Start = CurrentOffset;
	}

	return FALSE;
}


void RGBracketStringSearch :: ResetPreview () {

	PreviewOffset = CurrentOffset;
	SearchLeft.ResetPreview ();
	SearchRight.ResetPreview ();
}


void RGBracketStringSearch :: ResetPreviewToBeginning () {

	PreviewOffset = 0;
	SearchLeft.ResetPreviewToBeginning ();
	SearchRight.ResetPreviewToBeginning ();
}


Boolean RGBracketStringSearch :: PreviewNextBracketedString (size_t& FoundEndOffset, RGString& includedString, RGString& NewSearchString) {

	size_t FoundBegin;
	size_t FoundLeftEnd;
	size_t FoundRightBegin;
	Boolean Found = SearchLeft.PreviewNext (PreviewOffset, FoundBegin, FoundLeftEnd, NewSearchString);

	if (!Found)
		return FALSE;

    FoundLeftEnd++;
	Found = SearchRight.PreviewNext (FoundLeftEnd, FoundRightBegin, FoundEndOffset, NewSearchString);

	if (!Found)
		return FALSE;

	if (FoundRightBegin == FoundLeftEnd)
		includedString = "";

	else
		includedString = NewSearchString.ExtractSubstring (FoundLeftEnd, FoundRightBegin - 1);

	FoundEndOffset++;
	PreviewOffset = FoundEndOffset;
	return TRUE;
}


RGXMLTagSearch :: RGXMLTagSearch (const RGString& tag, RGString& search) {

	RGString left = "<" + tag + ">";
	RGString right = "</" + tag + ">";
	mDoesTheWork = new RGBracketStringSearch (left, right, search);
}



RGXMLTagSearch :: ~RGXMLTagSearch () {

	delete mDoesTheWork;
}


void RGXMLTagSearch :: ResetSearch () {

	mDoesTheWork->ResetSearch ();
}


Boolean RGXMLTagSearch :: FindNextTag (size_t StartOffset, size_t& FoundEndOffset, RGString& includedString) {

	return mDoesTheWork->FindNextBracketedString (StartOffset, FoundEndOffset, includedString);
}


Boolean RGXMLTagSearch :: FindNextNonemptyTag (size_t StartOffset, size_t& FoundEndOffset, RGString& includedString) {

	return mDoesTheWork->FindNextNonemptyBracketedString (StartOffset, FoundEndOffset, includedString);
}


Boolean RGXMLTagSearch :: FindNextTag (size_t& FoundEndOffset, RGString& includedString) {

	return mDoesTheWork->FindNextBracketedString (FoundEndOffset, includedString);
}


Boolean RGXMLTagSearch :: FindNextNonemptyTag (size_t& FoundEndOffset, RGString& includedString) {

	return mDoesTheWork->FindNextNonemptyBracketedString (FoundEndOffset, includedString);
}



RGCaseIndependentStringSearch :: RGCaseIndependentStringSearch (const RGString& target) : Target (target), CurrentOffset (0) {

	Target.ToUpper ();
	RGString lower (Target);
	lower.ToLower ();

	TLength = Target.Length ();
	TLength1 = TLength - 1;
	TargetData = (char*) Target.GetData ();
	char* LTargetData = (char*) lower.GetData ();
	TargetEnd = TargetData + TLength1;
  size_t i;
	for (i=0; i<256; i++)
		Skip [i] = TLength;

	unsigned char Index;
	unsigned char Last = *((unsigned char*)TargetEnd);
	unsigned char last = *((unsigned char*)(LTargetData + TLength1));
	SkipLast = TLength;

	for (i=0; i<TLength1; i++) {

		Index = *((unsigned char*)(TargetData + i));
		Skip [Index] = TLength1 - i;
		Index = *((unsigned char*)(LTargetData + i));
		Skip [Index] = TLength1 - i;
	}

	SkipLast = Skip [Last];
	Skip [Last] = 0;
	Skip [last] = 0;
}



RGCaseIndependentStringSearch :: ~RGCaseIndependentStringSearch () {

}

void RGCaseIndependentStringSearch :: ResetSearch () {

	CurrentOffset = 0;
}

Boolean RGCaseIndependentStringSearch :: FindNext (size_t StartOffset, size_t& FoundBeginOffset, size_t& FoundEndOffset, const RGString& searchString) const {

	size_t SLength = searchString.Length ();
	
	if (SLength == 0)
		return FALSE;

	if (TLength == 0)
		return FALSE;

	char* StringData = (char*)searchString.GetData ();
	
	size_t CurrentPosition = StartOffset;
	size_t Jump;
	char* CurrentTestSite; 
	size_t CurrentTestOffset;

	while (CurrentPosition + TLength1 < SLength) {

		CurrentTestOffset = CurrentPosition + TLength1;
		CurrentTestSite = StringData + CurrentTestOffset;
		Jump = Skip [*((unsigned char*)CurrentTestSite)];

		if (Jump == 0) {

			if (TestForMatchCaseIndependent (CurrentPosition, searchString)) {

				FoundBeginOffset = CurrentPosition;
				FoundEndOffset = CurrentPosition + TLength1;
				return TRUE;
			}

			Jump = SkipLast;
		}

		CurrentPosition += Jump;
	}

	return FALSE;
}


Boolean RGCaseIndependentStringSearch :: FindNext (size_t& FoundBeginOffset, size_t& FoundEndOffset, const RGString& searchString) {

	size_t SLength = searchString.Length ();
	
	if (SLength == 0)
		return FALSE;

	if (TLength == 0)
		return FALSE;

	size_t Jump;
	char* CurrentTestSite; 
	size_t CurrentTestOffset;
	char* StringData = (char*) searchString.GetData ();

	while (CurrentOffset + TLength1 < SLength) {

		CurrentTestOffset = CurrentOffset + TLength1;
		CurrentTestSite = StringData + CurrentTestOffset;
		Jump = Skip [*((unsigned char*)CurrentTestSite)];

		if (Jump == 0) {

			if (TestForMatchCaseIndependent (CurrentOffset, searchString)) {

				FoundBeginOffset = CurrentOffset;
				FoundEndOffset = CurrentOffset + TLength1;
				CurrentOffset = FoundEndOffset + 1;
				return TRUE;
			}

			Jump = SkipLast;
		}

		CurrentOffset += Jump;
	}

	return FALSE;
}


Boolean RGCaseIndependentStringSearch :: TestForMatchCaseIndependent (size_t offset, const RGString& searchString) const {

	char* CurrentS = (char*) (searchString.GetData () + offset);
	char* CurrentT = TargetData;
	char c;

	while (CurrentT <= TargetEnd) {

		c = toupper (*CurrentS);
		
		if (c != *CurrentT)
			return FALSE;

		CurrentS++;
		CurrentT++;
	}

	return TRUE;
}
