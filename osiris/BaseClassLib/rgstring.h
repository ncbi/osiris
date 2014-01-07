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
*  FileName: rgstring.h
*  Author:   Robert Goor
*
*/
//
// class RGString, inherits from RGPersistent and manages strings
//

#ifndef _RGSTRING_H_
#define _RGSTRING_H_

#include "rgpersist.h"
#include "rgdefs.h"
#include "rgstringdata.h"

#include <cstring>
#include <cctype>
#include <iostream>
#include <string>


const int _RGSTRING_ = 3;

class RGStringTokenizer;
class RGStringSearch;
class RGCaseIndependentStringSearch;
class RGSimpleString;

enum RGCASE {RGMatchCase, RGIgnoreCase};


using namespace std;


PERSISTENT_PREDECLARATION (RGString)


class RGString : public RGPersistent {

friend class RGStringTokenizer;
friend class RGStringSearch;
friend class RGCaseIndependentStringSearch;
friend class RGSimpleString;

PERSISTENT_DECLARATION (RGString)

public:

	RGString ();
	RGString (const char*);
	RGString (const char*,size_t size);
	RGString (const RGString&);
	RGString (const RGSimpleString&);
	RGString (size_t size);
	virtual ~RGString ();

	void ResizeLength (size_t size);
  void IncreaseSizeTo(size_t size)
  {
    Data->IncreaseSizeTo(size);
  }

	operator const char*() const { return Data->GetData(); }  // make inline
	const char* GetData () const { return Data->GetData(); }

	RGString& operator=(const char*);
	RGString& operator=(const RGString&);
	RGString& operator=(const RGSimpleString&);
	RGString& operator+=(const char*);  // Append
	RGString& operator+=(const RGString& s);  // Append

	RGString& operator<<(const char*);
	RGString& operator<<(const RGString& s);
	RGString& operator<<(int i);
	RGString& operator<<(double d);
	RGString& operator<<(unsigned long i);
	RGString& operator<<(char c);
	RGString& operator<<(long l);
	RGString& operator<<(short s);
	RGString& operator<<(unsigned char c);
	RGString& operator<<(unsigned int i);
	RGString& operator<<(unsigned short s);

	void SetCharacter (char, size_t);  // with bounds checking
  void Truncate(size_t n)
  {
    if (n < StringLength)
	    SetCharacter(0,n);
  }
	char GetCharacter (size_t) const;  // with bounds checking
	char GetFirstCharacter () const;
	char GetLastCharacter () const;
	RGString ExtractLastCharacters (size_t nChars) const;
	RGString ExtractAndRemoveLastCharacters (size_t nChars);
	char RemoveLastCharacter ();

//	char& operator[](size_t); // with bounds checking
//	char& operator()(size_t); // without bounds checking?
//	char operator[](size_t) const;
//	char operator()(size_t) const;

	RGString& Append (const char*);
	RGString& Append (const RGString&);
	RGString& Append (char c);

	int CompareTo (const char*, RGCASE Case = RGMatchCase) const;
	int CompareTo (const RGString&, RGCASE Case = RGMatchCase) const;

	Boolean IsEmpty () const { return StringLength == 0; }
	size_t Length () const { return StringLength; }
	size_t StringLengthOnDisk () const;

	RGString& Prepend(const char*);
	RGString& Prepend(const RGString&);

	void ToUpper ();
	void ToLower ();
	void Convert (unsigned long i, unsigned long base);
	void Convert (int i, unsigned long base);
	void Reverse ();

	int ConvertToInteger () const;
	unsigned long ConvertToUnsignedLong () const;
	double ConvertToDouble () const;

	Boolean FindSubstring (const RGString& str, size_t& position) const;
	Boolean FindNextSubstring (size_t start, const RGString& str, size_t& position) const;
	Boolean FindSubstringCaseIndependent (const RGString& str, size_t& position) const;
	Boolean FindNextSubstringCaseIndependent (size_t start, const RGString& str, size_t& position) const;
	Boolean FindLastSubstring (const RGString& str, size_t& startPosition, size_t& endPosition) const;
	Boolean FindLastSubstringCaseIndependent (const RGString& str, size_t& startPosition, size_t& endPosition);
	RGString ExtractSubstring (size_t first, size_t last) const;
	RGString ExtractAndRemoveSubstring (size_t first, size_t last);

	Boolean FindAndReplaceNextSubstring (const RGString& target, const RGString& replace, size_t& position);
	Boolean FindAndReplaceAllSubstrings (const RGString& target, const RGString& replace);
	Boolean ReplaceSubstring (size_t startPosn, size_t& endPosn, const RGString& replace);

	istream& ReadFile(istream&);   // Read to EOF or null character.
	istream& ReadLine(istream&);   // Read to EOF or newline.
	istream& ReadString(istream&);   // Read to EOF or null character.
	istream& ReadToDelimiter (istream&, char delimiter = '\n'); // Read to EOF or delimiter.
	istream& ReadToken(istream&);  // Read separated by white space.
	void ReadStandardInputLine ();

	RGFile& ReadTextFile (RGFile&);  // Read to EOF or null character
	RGFile& ReadTextLine (RGFile&);  // Read to EOF or newline
	RGFile& ReadTextString (RGFile&);  // Read to EOF or null character
	RGFile& ReadToEndOfFile (RGFile& f);
	RGFile& ReadTextToDelimiter (RGFile&, char delimiter = '\n');  // Read to EOF or delimiter
	RGFile& ReadTextToken (RGFile&);  // Read separated by white space.

	void WriteTextLine (RGFile&);  // writes a new line character after end

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	static void SetDoubleResolution (int r) { DoubleResolution = r; }
	static int GetDoubleResolution () { return DoubleResolution; }

protected:

	RGStringData* Data;
	size_t StringLength;

	void ResetData ();
	void ConvertReverse (unsigned long i, unsigned long base);

	friend RGString operator+(const RGString&, const RGString&);
	friend RGString operator+(const RGString&,  const char*);
	friend RGString operator+(const char*, const RGString&);
	friend bool operator>(const RGString&, const RGString &);
	friend bool operator<(const RGString&, const RGString &);
	friend bool operator>(const RGString&, const char *);
	friend bool operator<(const RGString&, const char *);
	friend Boolean operator==(const RGString&, const char*);
//	friend Boolean operator==(const char*, const RGString&);
	friend Boolean operator!=(const RGString&, const char*);

	static int DoubleResolution;

};


#endif  /*  _RGSTRING_H_  */
