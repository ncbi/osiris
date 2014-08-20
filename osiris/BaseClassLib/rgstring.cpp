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
*  FileName: rgstring.cpp
*  Author:   Robert Goor
*
*/
//
// class RGString, inherits from RGPersistent and manages strings
//

#include <math.h>
#include <stdlib.h>

#include "rgstring.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgsimplestring.h"
#include "RGTextOutput.h"


int RGString::DoubleResolution = 15;



PERSISTENT_DEFINITION(RGString, _RGSTRING_, "xs:string")


RGString :: RGString () : RGPersistent (), StringLength (0) {

	Data = new RGStringData;
}


RGString :: RGString (const char* str) : RGPersistent () {

	Data = new RGStringData (str);
	StringLength = strlen (str);
}

RGString ::RGString (const char * str, size_t size) : RGPersistent() 
{
  Data = new RGStringData(str);
  Truncate(size);
}



RGString :: RGString (const RGString& str) : RGPersistent (str), StringLength (str.StringLength) {

	Data = (str.Data)->MakeCopy ();
}


RGString :: RGString (const RGSimpleString& str) : RGPersistent (), StringLength (str.StringLength) {

	Data = (str.Data)->MakeCopy ();
}



RGString :: RGString (size_t size) : RGPersistent (), StringLength (0) {

	Data = new RGStringData (size);
}



RGString :: ~RGString () {

	delete Data;
}


void RGString :: ResizeLength (size_t size) {

	RGStringData* NewData = new RGStringData (*Data, size);
	delete Data;
	Data = NewData;
}


/*char RGString :: operator[](size_t i) const {

	char* StringArray = Data->GetData ();
	
	if ((i >= 0) && (i < StringLength))
		return StringArray [i];

	return '\0';
}


char RGString :: operator()(size_t i) const {

	char* StringArray = Data->GetData ();
	return StringArray [i];
}

*/

void RGString :: SetCharacter (char c, size_t i) { // with bounds checking

	RGStringData* NewData;
	
	if (Data->GetReferenceCount () > 1) {

		NewData = new RGStringData (*Data);
		delete Data;
		Data = NewData;
	}
	
	char* StringArray = Data->GetData ();
	
	if ((i >= 0) && (i < StringLength)) {

		StringArray [i] = c;

		if (c == '\0')
			StringLength = i;
	}
}



char RGString :: GetCharacter (size_t i) const { // without bounds checking?

	char* StringArray = Data->GetData ();

	if ((i >= 0) && (i < StringLength))
		return StringArray [i];

	return '\0';
}


char RGString :: GetFirstCharacter () const {

	if (StringLength == 0)
		return '\0';
	
	char* StringArray = Data->GetData ();
	return StringArray [0];
}


char RGString :: GetLastCharacter () const {

	if (StringLength == 0)
		return '\0';
	
	char* StringArray = Data->GetData ();
	return StringArray [StringLength - 1];
}


RGString RGString :: ExtractLastCharacters (size_t nChars) const {

	if (nChars >= StringLength)
		return *this;
	
	size_t last = StringLength - 1;
	size_t first = last - nChars + 1;

	RGString ReturnValue (last - first + 2);
	const char* str = GetData () + first;
	char* strOut = ReturnValue.Data->GetData ();

	for (size_t i=first; i<=last; i++) {

		*strOut = *str;
		strOut++;
		str++;
	}

	*strOut = '\0';
	ReturnValue.StringLength = last - first + 1;

	return ReturnValue;
}



RGString RGString :: ExtractAndRemoveLastCharacters (size_t nChars) {

	if (nChars == 0) {

		RGString Oops;
		return Oops;
	}
	
	if (Data->GetReferenceCount () > 1)	{

		RGStringData* NewData = new RGStringData (*Data);
		delete Data;
		Data = NewData;
	}
	
	if (nChars >= StringLength) {

		RGString All (*this);
		char* OldData = Data->GetData ();
		OldData [0] = '\0';
		StringLength = 0;
		return All;
	}

	size_t last = StringLength - 1;
	size_t first = last - nChars + 1;
	
	RGString ReturnValue (last - first + 2);
	const char* str = GetData () + first;
	char* strOut = ReturnValue.Data->GetData ();

	for (size_t i=first; i<=last; i++) {

		*strOut = *str;
		strOut++;
		str++;
	}

	*strOut = '\0';
	size_t StrLen = last - first + 1;
	size_t NewStringLength = StringLength - StrLen;
	ReturnValue.StringLength = StrLen;

	//
	// move in place in increasing order
	//

	char* EPtr = (char*)GetData () + first;
	char* ePtr = (char*)GetData() + last + 1;
	char* sourcePtr = (char*)GetData () + NewStringLength;
	char* endPtr = (char*)GetData () + StringLength;

	while (ePtr < endPtr) {

		*EPtr = *ePtr;
		EPtr++;
		ePtr++;
	}

	while (sourcePtr <= endPtr) {

		*sourcePtr = '\0';
		sourcePtr++;
	}

	StringLength = NewStringLength;
	return ReturnValue;
}



char RGString :: RemoveLastCharacter () {

	if (StringLength == 0)
		return '\0';

	if (Data->GetReferenceCount () > 1)	{

		RGStringData* NewData = new RGStringData (*Data);
		delete Data;
		Data = NewData;
	}

	char* OldData = Data->GetData ();
	size_t L = StringLength - 1;
	char ReturnValue = OldData [L];
	OldData [L] = '\0';
	StringLength--;
	return ReturnValue;
}


RGString& RGString :: operator=(const char* str) {

	if (Data->GetReferenceCount () == 1)
		Data->CopyData (str);
	
	else {

		delete Data;
		Data = new RGStringData (str);
	}

	StringLength = strlen (str);
	return *this;
}



RGString& RGString :: operator=(const RGString& str) {

	if (Data == str.Data)
		return *this;
	
	delete Data;
	Data = (str.Data)->MakeCopy ();
	StringLength = str.StringLength;
	return *this;
}


RGString& RGString :: operator=(const RGSimpleString& str) {

	if (Data == str.Data)
		return *this;
	
	delete Data;
	Data = (str.Data)->MakeCopy ();
	StringLength = str.StringLength;
	return *this;
}


RGString& RGString :: operator+=(const char* str) {  // Append

	return Append (str);
}



RGString& RGString :: operator+=(const RGString& str) {  // Append

	return Append (str);
}


RGString& RGString :: operator<<(const char* s) {

	return Append (s);
}


RGString& RGString :: operator<<(const RGString& s) {

	return Append (s);
}


RGString& RGString :: operator<<(int i) {

	char temp [35];
	*this += _ltoa (i, temp, 10);
	return *this;
}


RGString& RGString :: operator<<(double d) {

	char buffer [40];
	*this += gcvt (d, RGString::DoubleResolution, buffer);
	return *this;
}


RGString& RGString :: operator<<(unsigned long i) {

	char temp [35];
	*this += _ultoa (i, temp, 10);
	return *this;
}


RGString& RGString :: operator<<(char c) {

	return Append (c);
}


RGString& RGString :: operator<<(long l) {

	char temp [35];
	*this += _ltoa (l, temp, 10);
	return *this;
}


RGString& RGString :: operator<<(short s) {

	char temp [35];
	*this += _ltoa ((long)s, temp, 10);
	return *this;
}


RGString& RGString :: operator<<(unsigned char c) {

	char temp [35];
	*this += _ultoa ((unsigned long)c, temp, 10);
	return *this;
}


RGString& RGString :: operator<<(unsigned int i) {

	char temp [35];
	*this += _ultoa ((unsigned long)i, temp, 10);
	return *this;
}


RGString& RGString :: operator<<(unsigned short s) {

	char temp [35];
	*this += _ultoa ((unsigned long)s, temp, 10);
	return *this;
}


RGString& RGString :: Append (const char* str) {

	RGString Temp (str);

	size_t N = StringLength + strlen (str) + 1;
	RGStringData* NewData;
	
	if (Data->GetReferenceCount () == 1)		
		Data->IncreaseSizeTo (N);
	
	else {

		NewData = new RGStringData (*Data, N);
		delete Data;
		Data = NewData;
	}

	Data->Append (Temp.GetData ());
	StringLength = N - 1;
	return *this;
}



RGString& RGString :: Append (const RGString& str) {

	RGString Temp (str);

	size_t N = StringLength + str.StringLength + 1;
	RGStringData* NewData;
	
	if (Data->GetReferenceCount () == 1)		
		Data->IncreaseSizeTo (N);
	
	else {

		NewData = new RGStringData (*Data, N);
		delete Data;
		Data = NewData;
	}

	Data->Append (Temp.GetData());
	StringLength = N - 1;
	return *this;
}


RGString& RGString :: Append (char c) {

	size_t N = StringLength + 2;
	RGStringData* NewData;
	
	if (Data->GetReferenceCount () == 1)		
		Data->IncreaseSizeTo (N);
	
	else {

		NewData = new RGStringData (*Data, N);
		delete Data;
		Data = NewData;
	}

	Data->AppendCharacter (c, StringLength);
	StringLength++;
	return *this;
}


int RGString :: CompareTo (const char* str, RGCASE Case) const {

	char* MyData = Data->GetData ();
	

	if (Case == RGMatchCase)
		return strcmp (MyData, str);

	RGString MyLowerData (MyData);
	RGString CmpLowerData (str);
	MyLowerData.ToLower ();
	CmpLowerData.ToLower ();
	return MyLowerData.CompareTo (&CmpLowerData);
}



int RGString :: CompareTo (const RGString& str, RGCASE Case) const {

	char* MyData = Data->GetData ();

	if (Case == RGMatchCase)
		return strcmp (MyData, str.GetData());

	RGString MyLowerData (MyData);
	RGString CmpLowerData (str);
	MyLowerData.ToLower ();
	CmpLowerData.ToLower ();
	return MyLowerData.CompareTo (&CmpLowerData);
}


RGString& RGString :: Prepend (const char* str) {

	RGString* Temp = new RGString (str);
	Temp->Append (*this);
	*this = *Temp;
	delete Temp;
	return *this;
}



RGString& RGString :: Prepend(const RGString& str) {

	RGString* Temp = new RGString (str);
	Temp->Append (*this);
	*this = *Temp;
	delete Temp;
	return *this;
}


void RGString :: ResetData () {

	RGStringData* NewData;
	
	if (Data->GetReferenceCount () > 1) {

		NewData = new RGStringData;
		delete Data;
		Data = NewData;
	}

	else	
		Data->ResetData ();

	StringLength = 0;
}


void RGString :: ToUpper () {

	RGStringData* NewData;
	
	if (Data->GetReferenceCount () > 1) {

		NewData = new RGStringData (*Data);
		delete Data;
		Data = NewData;
	}
	
	char* MyData = Data->GetData ();
	char* EndData = MyData + StringLength;
	
	for (char* p=MyData; p<EndData; p++)
		*p = toupper (*p);
}



void RGString :: ToLower () {

	RGStringData* NewData;
	
	if (Data->GetReferenceCount () > 1) {

		NewData = new RGStringData (*Data);
		delete Data;
		Data = NewData;
	}
	
	char* MyData = Data->GetData ();
	char* EndData = MyData + StringLength;

	for (char* p=MyData; p<EndData; p++)
		*p = tolower (*p);
}



void RGString :: ConvertReverse (unsigned long i, unsigned long base) {

	unsigned long reduced;
	unsigned long digit;
	char Cdigit [10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	
	if (base <= 1)
		return;

	unsigned long temp = i;

	if (temp == 0) {

		*this = "0";
		return;
	}

	*this = "";

	while (temp != 0) {
	
		reduced = temp / base;
		digit = temp - base * reduced;
		Append (Cdigit [digit]);
		temp = reduced;
	}
}


void RGString :: Convert (unsigned long i, unsigned long base) {

	char temp [35];
	*this = _ultoa (i, temp, base);
}



void RGString :: Convert (int i, unsigned long base) {

	char temp [35];
	*this = _ltoa (i, temp, base);
}


void RGString :: Reverse () {

	char temp;
	
	if (StringLength <= 1)
		return;
	
	char* first = Data->GetData ();
	char* last = first + (StringLength - 1);
	
	while (first < last) {
	
		temp = *first;
		*first = *last;
		*last = temp;
		first++;
		last--;
	}
}


int RGString :: ConvertToInteger () const {

	char* str = Data->GetData ();
	return (int)strtol (str, (char**)NULL, 10);  //atoi (str);
}


unsigned long RGString :: ConvertToUnsignedLong () const {

	char* str = Data->GetData ();
	return strtoul (str, NULL, 10);
}


double RGString :: ConvertToDouble () const {

	char* str = Data->GetData ();
	return strtod (str, (char**)NULL);  //atof (str);
}


Boolean RGString :: FindSubstring (const RGString& str, size_t& position) const {

	return FindNextSubstring (0, str, position);
}


Boolean RGString :: FindNextSubstring (size_t start, const RGString& str, size_t& position) const {

	const char* target = str.GetData();
	size_t targetLen = str.Length ();
	const char* source = GetData ();
	size_t sourceIndex = start;
	Boolean Match = FALSE;
	position = 0;
	char c1, c2;

	char* TargetPtr = (char*)target;
	char* TargetEnd = (char*)TargetPtr + targetLen;
	char* MTargetPtr;
	char* MSource = (char*) source + start;
	char* MSourcePtr;

	while (sourceIndex + targetLen <= StringLength) {

		MTargetPtr = TargetPtr;
		MSourcePtr = MSource;

		while (MTargetPtr < TargetEnd) {

			c1 = *(MSourcePtr);
			c2 = *(MTargetPtr);
			
			if (c1 == c2) {

				MTargetPtr++;
				MSourcePtr++;
				Match = TRUE;  // at least, so far
			}

			else {
				
				Match = FALSE;
				break;
			}
		}

		if (Match) {

			position = sourceIndex;
			return TRUE;
		}

		sourceIndex++;
		MSource++;
	}

	return FALSE;
}


Boolean RGString :: FindSubstringCaseIndependent (const RGString& str, size_t& position) const {

	return FindNextSubstringCaseIndependent (0, str, position);
}


Boolean RGString :: FindNextSubstringCaseIndependent (size_t start, const RGString& str, size_t& position) const {

	RGString Str (str);
	Str.ToUpper ();
	const char* target = Str.GetData();
	size_t targetLen = Str.Length ();
	const char* source = GetData ();
	size_t sourceIndex = start;
	Boolean Match = FALSE;
	position = 0;
	char c1, c2;

	char* TargetPtr = (char*)target;
	char* TargetEnd = (char*)TargetPtr + targetLen;
	char* MTargetPtr;
	char* MSource = (char*) source + start;
	char* MSourcePtr;

	while (sourceIndex + targetLen <= StringLength) {

		MTargetPtr = TargetPtr;
		MSourcePtr = MSource;

		while (MTargetPtr < TargetEnd) {

			c1 = toupper(*MSourcePtr);
			c2 = *MTargetPtr;
			
			if (c1 == c2) {

				MTargetPtr++;
				MSourcePtr++;
				Match = TRUE;  // at least, so far
			}

			else {
				
				Match = FALSE;
				break;
			}
		}

		if (Match) {

			position = sourceIndex;
			return TRUE;
		}

		sourceIndex++;
		MSource++;
	}

	return FALSE;
}


Boolean RGString :: FindLastSubstring (const RGString& str, size_t& startPosition, size_t& endPosition) const {

	const char* target = str.GetData();
	size_t targetLen = str.Length ();
	const char* source = GetData ();

	if (targetLen > StringLength)
		return FALSE;

	size_t sourceIndex = StringLength - targetLen;
	Boolean Match = FALSE;
	startPosition = sourceIndex;
	char c1, c2;

	char* TargetPtr = (char*)target;
	char* TargetEnd = (char*)TargetPtr + targetLen;
	char* MTargetPtr;
	char* MSource = (char*) source + sourceIndex;
	char* MSourcePtr;

	while (MSource >= source) {

		MTargetPtr = TargetPtr;
		MSourcePtr = MSource;

		while (MTargetPtr < TargetEnd) {

			c1 = *(MSourcePtr);
			c2 = *(MTargetPtr);
			
			if (c1 == c2) {

				MTargetPtr++;
				MSourcePtr++;
				Match = TRUE;  // at least, so far
			}

			else {
				
				Match = FALSE;
				break;
			}
		}

		if (Match) {

			startPosition = sourceIndex;
			endPosition = sourceIndex + targetLen - 1;
			return TRUE;
		}

		sourceIndex--;
		MSource--;
	}

	return FALSE;
}


Boolean RGString :: FindLastSubstringCaseIndependent (const RGString& str, size_t& startPosition, size_t& endPosition) {

	RGString Str (str);
	Str.ToUpper ();
	const char* target = Str.GetData();
	size_t targetLen = Str.Length ();
	const char* source = GetData ();

	if (targetLen > StringLength)
		return FALSE;

	size_t sourceIndex = StringLength - targetLen;
	Boolean Match = FALSE;
	startPosition = sourceIndex;
	char c1, c2;

	char* TargetPtr = (char*)target;
	char* TargetEnd = (char*)TargetPtr + targetLen;
	char* MTargetPtr;
	char* MSource = (char*) source + sourceIndex;
	char* MSourcePtr;

	while (MSource >= source) {

		MTargetPtr = TargetPtr;
		MSourcePtr = MSource;

		while (MTargetPtr < TargetEnd) {

			c1 = toupper(*MSourcePtr);
			c2 = *(MTargetPtr);
			
			if (c1 == c2) {

				MTargetPtr++;
				MSourcePtr++;
				Match = TRUE;  // at least, so far
			}

			else {
				
				Match = FALSE;
				break;
			}
		}

		if (Match) {

			startPosition = sourceIndex;
			endPosition = sourceIndex + targetLen - 1;
			return TRUE;
		}

		sourceIndex--;
		MSource--;
	}

	return FALSE;
}



RGString RGString :: ExtractSubstring (size_t first, size_t last) const {

	if (last >= StringLength)
		last = StringLength - 1;

	if (last < first) {

		RGString Oops;
		return Oops;
	}

	RGString ReturnValue (last - first + 2);
	const char* str = GetData () + first;
	char* strOut = ReturnValue.Data->GetData ();

	for (size_t i=first; i<=last; i++) {

		*strOut = *str;
		strOut++;
		str++;
	}

	*strOut = '\0';
	ReturnValue.StringLength = last - first + 1;

	return ReturnValue;
}


RGString RGString :: ExtractAndRemoveSubstring (size_t first, size_t last) {

	if (last >= StringLength)
		last = StringLength - 1;

	if (last < first) {

		RGString Oops;
		return Oops;
	}

	if (Data->GetReferenceCount () > 1)	{

		RGStringData* NewData = new RGStringData (*Data);
		delete Data;
		Data = NewData;
	}
	
	RGString ReturnValue (last - first + 2);
	const char* str = GetData () + first;
	char* strOut = ReturnValue.Data->GetData ();

	for (size_t i=first; i<=last; i++) {

		*strOut = *str;
		strOut++;
		str++;
	}

	*strOut = '\0';
	size_t StrLen = last - first + 1;
	size_t NewStringLength = StringLength - StrLen;
	ReturnValue.StringLength = StrLen;

	//
	// move in place in increasing order
	//

	char* EPtr = (char*)GetData () + first;
	char* ePtr = (char*)GetData() + last + 1;
	char* sourcePtr = (char*)GetData () + NewStringLength;
	char* endPtr = (char*)GetData () + StringLength;

	while (ePtr < endPtr) {

		*EPtr = *ePtr;
		EPtr++;
		ePtr++;
	}

	while (sourcePtr <= endPtr) {

		*sourcePtr = '\0';
		sourcePtr++;
	}

	StringLength = NewStringLength;
	return ReturnValue;
}


Boolean RGString :: FindAndReplaceNextSubstring (const RGString& target, const RGString& replace, size_t& position) {

	size_t TLength = target.Length ();
	size_t RLength = replace.Length ();
	size_t FoundPosition;
	char* sourcePtr;
	char* rPtr;
	char* ePtr;
	char* EPtr;

	if (TLength == 0)
		return FALSE;

	Boolean Found = FindNextSubstring (position, target, FoundPosition);

	if (!Found)
		return FALSE;

	size_t N = StringLength + RLength - TLength + 1;
	size_t NewStringLength = N - 1;
	RGStringData* NewData;
	
	if (Data->GetReferenceCount () == 1)		
		Data->IncreaseSizeTo (N);
	
	else {

		NewData = new RGStringData (*Data, N);
		delete Data;
		Data = NewData;
	}
	
	if (TLength > RLength) {

		// move in place in increasing order
		sourcePtr = (char*)GetData() + FoundPosition;
		rPtr = (char*)replace.GetData ();
		ePtr = sourcePtr + TLength;

		while (*rPtr != '\0') {

			*sourcePtr = *rPtr;
			sourcePtr++;
			rPtr++;
		}

		while (*ePtr != '\0') {

			*sourcePtr = *ePtr;
			sourcePtr++;
			ePtr++;
		}

		*sourcePtr = '\0';
	}

	else if (TLength < RLength) {

		// move in place in decreasing order
		EPtr = (char*)GetData () + StringLength - 1;
		rPtr = (char*)replace.GetData ();
		ePtr = (char*)GetData() + FoundPosition + TLength;
		sourcePtr = (char*)GetData () + NewStringLength;
		*sourcePtr = '\0';
		sourcePtr--;

		while (EPtr >= ePtr) {

			*sourcePtr = *EPtr;
			sourcePtr--;
			EPtr--;
		}

		sourcePtr = (char*)GetData() + FoundPosition;

		while (*rPtr != '\0') {

			*sourcePtr = *rPtr;
			sourcePtr++;
			rPtr++;
		}
	}

	else {  // they are equal

		rPtr = (char*)replace.GetData ();
		sourcePtr = (char*)GetData() + FoundPosition;

		while (*rPtr != '\0') {

			*sourcePtr = *rPtr;
			sourcePtr++;
			rPtr++;
		}
	}

	StringLength = NewStringLength;
	position = FoundPosition + RLength;
	return TRUE;
}


Boolean RGString :: FindAndReplaceAllSubstrings (const RGString& target, const RGString& replace) {

	size_t position = 0;
	Boolean ReturnValue = FALSE;

	while (TRUE) {

		if (FindAndReplaceNextSubstring (target, replace, position))
			ReturnValue = TRUE;

		else
			break;
	}

	return ReturnValue;
}


Boolean RGString :: ReplaceSubstring (size_t startPosn, size_t& endPosn, const RGString& replace) {

	size_t TLength = endPosn - startPosn + 1;
	size_t RLength = replace.Length ();
	size_t FoundPosition = startPosn;
	char* sourcePtr;
	char* rPtr;
	char* ePtr;
	char* EPtr;

	size_t N = StringLength + RLength - TLength + 1;
	size_t NewStringLength = N - 1;
	RGStringData* NewData;
	
	if (Data->GetReferenceCount () == 1)		
		Data->IncreaseSizeTo (N);
	
	else {

		NewData = new RGStringData (*Data, N);
		delete Data;
		Data = NewData;
	}
	
	if (TLength > RLength) {

		// move in place in increasing order
		sourcePtr = (char*)GetData() + FoundPosition;
		rPtr = (char*)replace.GetData ();
		ePtr = sourcePtr + TLength;

		while (*rPtr != '\0') {

			*sourcePtr = *rPtr;
			sourcePtr++;
			rPtr++;
		}

		while (*ePtr != '\0') {

			*sourcePtr = *ePtr;
			sourcePtr++;
			ePtr++;
		}

		*sourcePtr = '\0';
	}

	else if (TLength < RLength) {

		// move in place in decreasing order
		EPtr = (char*)GetData () + StringLength - 1;
		rPtr = (char*)replace.GetData ();
		ePtr = (char*)GetData() + FoundPosition + TLength;
		sourcePtr = (char*)GetData () + NewStringLength;
		*sourcePtr = '\0';
		sourcePtr--;

		while (EPtr >= ePtr) {

			*sourcePtr = *EPtr;
			sourcePtr--;
			EPtr--;
		}

		sourcePtr = (char*)GetData() + FoundPosition;

		while (*rPtr != '\0') {

			*sourcePtr = *rPtr;
			sourcePtr++;
			rPtr++;
		}
	}

	else {  // they are equal

		rPtr = (char*)replace.GetData ();
		sourcePtr = (char*)GetData() + FoundPosition;

		while (*rPtr != '\0') {

			*sourcePtr = *rPtr;
			sourcePtr++;
			rPtr++;
		}
	}

	StringLength = NewStringLength;
	endPosn = FoundPosition + RLength;
	return TRUE;
}


istream& RGString :: ReadFile(istream& istr){
	
	// Read to EOF or null character

	return ReadToDelimiter (istr, '\0');
}



istream& RGString :: ReadLine(istream& istr) {
	
	// Read to EOF or newline.

	return ReadToDelimiter (istr, '\n');
}



istream& RGString :: ReadString(istream& istr) {
	
	// Read to EOF or null character.

	return ReadToDelimiter (istr, '\0');
}


istream& RGString :: ReadToDelimiter (istream& istr, char delimiter) {
	
	// Read to EOF or delimiter.

	char c;

	ResetData ();

	while (TRUE) {

		istr >> c;

		if ((c == EOF) || (c == delimiter))
			break;

		Data->AppendCharacter (c, StringLength);
		StringLength++;
	}

	return istr;
}


istream& RGString :: ReadToken(istream& istr) {
	
	// Read separated by white space.

	Boolean FoundToken = FALSE;
	char c;

	ResetData ();

	while (TRUE) {

		istr >> c;

		if ((c == ' ') || (c == '\n') || (c == '\t') || (c == EOF))
			break;
	}

	while (c != EOF) {

		Data->AppendCharacter (c, StringLength);
		StringLength++;

		istr >> c;

		if ((c == ' ') || (c == '\n') || (c == '\t'))
			break;
	}

	return istr;
}


void RGString :: ReadStandardInputLine () {

	ResetData ();
	string s1;

	getline (cin, s1, '\n');

	if (s1.length () == 0)
		getline (cin, s1, '\n');

	*this = (char*) s1.c_str ();
}


RGFile& RGString :: ReadTextFile (RGFile& f) {
	
	// Read to EOF or null character
	long fileSize = f.GetSizeOfFile ();
	IncreaseSizeTo ((size_t)(1.1 * fileSize));

	return ReadTextToDelimiter (f, '\0');
}


RGFile& RGString :: ReadTextLine (RGFile& f) {
	
	// Read to EOF or newline

	return ReadTextToDelimiter (f, '\n');
}


RGFile& RGString :: ReadTextString (RGFile& f) {
	
	// Read to EOF or null character

	return ReadTextToDelimiter (f, '\0');
}


RGFile& RGString :: ReadToEndOfFile (RGFile& f) {

	char c;

	ResetData ();

	while (TRUE) {

		f.Read (c);

		if (f.Eof ())
			break;

		Data->AppendCharacter (c, StringLength);
		StringLength++;
	}

	return f;
}


RGFile& RGString :: ReadTextToDelimiter (RGFile& f, char delimiter) {
	
	// Read to EOF or delimiter

	char c;

	ResetData ();

	while (TRUE) {

		f.Read (c);

		if ((f.Eof ()) || (c == delimiter))
			break;

		Data->AppendCharacter (c, StringLength);
		StringLength++;
	}

	return f;
}


RGFile& RGString :: ReadTextToken (RGFile& f) {
	
	// Read separated by white space.

	Boolean FoundToken = FALSE;
	char c;

	ResetData ();

	while (TRUE) {

		f.Read (c);

		if ((c == ' ') || (c == '\n') || (c == '\t') || (f.Eof ()))
			break;
	}

	while (!f.Eof ()) {

		Data->AppendCharacter (c, StringLength);
		StringLength++;

		f.Read (c);

		if ((c == ' ') || (c == '\n') || (c == '\t'))
			break;
	}

	return f;
}


void RGString :: WriteTextLine (RGFile& f) {

	f.Write (GetData (), StringLength);
	f.Write ("\n", 1);
}


size_t RGString :: StringLengthOnDisk () const {

	size_t N = sizeof (unsigned long);
	size_t Length1 = StringLength + 1;
	size_t NumberOfInts = Length1 / N;

	if (N * NumberOfInts < Length1)
		NumberOfInts++;

	return N * NumberOfInts;
}



size_t RGString :: StoreSize () const {

	size_t N = sizeof (size_t);
	size_t Size = RGPersistent::StoreSize () + N + StringLengthOnDisk ();
	return Size;
}



int RGString :: CompareTo (const RGPersistent* s) const {

	const RGString* str = (const RGString*) s;
	char* MyData = Data->GetData ();
	const char* strData = (str->Data)->GetData ();
	return strcmp (MyData, strData);
}



unsigned RGString :: HashNumber (unsigned long Base) const {

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



Boolean RGString :: IsEqualTo (const RGPersistent* s) const {

	const RGString* str = (const RGString*) s;
	char* MyData = Data->GetData ();
	const char* strData = (str->Data)->GetData ();
	return strcmp (MyData, strData) == 0;
}


void RGString :: RestoreAll (RGFile& f) {

	RGStringData* NewData;
	RGPersistent::RestoreAll (f);
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



void RGString :: RestoreAll (RGVInStream& vin) {

	
	RGStringData* NewData;
	RGPersistent::RestoreAll (vin);
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



void RGString :: SaveAll (RGFile& f) const {

	char* MyData = Data->GetData ();
	unsigned long* MyLongData = (unsigned long*) MyData;
	size_t N = StringLengthOnDisk () / sizeof (unsigned long);

	RGPersistent::SaveAll (f);
	f.Write ((unsigned int)StringLength);
	f.Write (MyLongData, N);
}



void RGString :: SaveAll (RGVOutStream& vout) const {

	char* MyData = Data->GetData ();
	unsigned long* MyLongData = (unsigned long*) MyData;
	size_t N = StringLengthOnDisk ();
	size_t NumberOfInts = N / sizeof (unsigned long);

	RGPersistent::SaveAll (vout);
	vout << (unsigned int)StringLength;
	vout.put (MyLongData, NumberOfInts);
}



RGString operator+(const RGString& s1, const RGString& s2) {

	RGString s3 (s1);
	s3 += s2;
	return s3;
}



RGString operator+(const RGString& s1,  const char* s2) {

	RGString s3 (s1);
	s3 += s2;
	return s3;
}



RGString operator+(const char* s1, const RGString& s2) {

	RGString s3 (s1);
	s3 += s2;
	return s3;
}


Boolean operator==(const RGString& s1, const char* s2) {

	return s1.CompareTo (s2) == 0;
}

bool operator <(const RGString &s1, const RGString &s2)
{
	return s1.CompareTo(s2) < 0;
}
bool operator >(const RGString &s1, const RGString &s2)
{
	return s1.CompareTo(s2) > 0;
}
bool operator <(const RGString &s1, const char *s2)
{
	return s1.CompareTo(s2) < 0;
}
bool operator >(const RGString &s1, const char *s2)
{
	return s1.CompareTo(s2) > 0;
}

Boolean operator!=(const RGString& s1, const char* s2) {

	return s1.CompareTo (s2) != 0;
}


RGTraceString :: RGTraceString () {

#ifdef _TRACEDEBUG
	mTrace.ResizeLength (500000);
#endif

}


RGTraceString :: RGTraceString (size_t size) {

#ifdef _TRACEDEBUG
	mTrace.ResizeLength (size);
#endif

}


RGTraceString :: ~RGTraceString () {

#ifdef _TRACEDEBUG
	mTrace = "\0";
#endif

}

void RGTraceString :: Reset () {

#ifdef _TRACEDEBUG
	mTrace = "\0";
#endif

}


void RGTraceString :: Save (const RGString& fullPath, const RGString& fileName) {

#ifdef _TRACEDEBUG
	RGString fullOutPath = fullPath + "/" + fileName;
	size_t start;
	size_t end;
	int lengthOfExtension;

	if (fullOutPath.FindLastSubstring (".", start, end)) {

		lengthOfExtension = fullOutPath.Length () - start - 1;
		fullOutPath.ExtractAndRemoveLastCharacters (lengthOfExtension);
	}

	fullOutPath += "trace.txt";
	cout << "Writing trace data to " << (char*)fullOutPath.GetData () << endl;
	RGTextOutput outFile (fullOutPath, FALSE);
	outFile << mTrace;
	//delete outFile;
	Reset ();
#endif
}

RGTraceString& RGTraceString :: operator<<(const RGString& str) {

#ifdef _TRACEDEBUG
	mTrace << str;
#endif

	return *this;
}



RGTraceString& RGTraceString :: operator<<(const char* str) {

#ifdef _TRACEDEBUG
	mTrace << str;
#endif

	return *this;
}



RGTraceString& RGTraceString :: operator<<(double d) {

#ifdef _TRACEDEBUG
	mTrace << d;
#endif

	return *this;
}



RGTraceString& RGTraceString :: operator<<(int i) {

#ifdef _TRACEDEBUG
	mTrace << i;
#endif

	return *this;
}



RGTraceString& RGTraceString :: operator<<(unsigned long i) {

#ifdef _TRACEDEBUG
	mTrace << i;
#endif

	return *this;
}



RGTraceString& RGTraceString :: operator<<(char c) {

#ifdef _TRACEDEBUG
	mTrace << c;
#endif

	return *this;
}


RGTraceString& RGTraceString :: operator<<(long l) {

#ifdef _TRACEDEBUG
	mTrace << l;
#endif

	return *this;
}



RGTraceString& RGTraceString :: operator<<(short s) {

#ifdef _TRACEDEBUG
	mTrace << s;
#endif

	return *this;
}



RGTraceString& RGTraceString :: operator<<(unsigned char c) {

#ifdef _TRACEDEBUG
	mTrace << c;
#endif

	return *this;
}



RGTraceString& RGTraceString :: operator<<(unsigned int i) {

#ifdef _TRACEDEBUG
	mTrace << i;
#endif

	return *this;
}



RGTraceString& RGTraceString :: operator<<(unsigned short s) {

#ifdef _TRACEDEBUG
	mTrace << s;
#endif

	return *this;
}

