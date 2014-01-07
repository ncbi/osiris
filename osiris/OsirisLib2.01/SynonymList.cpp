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
*  FileName: SynonymList.cpp
*  Author:   Robert Goor
*
*/
//
//  class SynonymList retains a list of synonyms for a given word or concept and tests if a word equals one of the synonyms 
//  or if a word contains one of the synonyms.  Later, we'll add xml input capability
//

#include "SynonymList.h"
#include "rgtokenizer.h"


SynonymList :: SynonymList () : mIt (mList), mValid (true) {

}


SynonymList :: SynonymList (const RGString& tagName, const RGString& xmlString) : mIt (mList), mValid (true) {

	RGString startTag = "<" + tagName + ">";
	RGString endTag = "</" + tagName + ">";
	RGString str (xmlString);
	RGString txtSearchStr;
	RGBracketStringSearch TagSearch (startTag, endTag, str);
	RGBracketStringSearch TextSearch ("<text>", "</text>", txtSearchStr);
	RGString textString;
	size_t startPos = 0;
	size_t endPos;
	RGString* newString;

	if (!TagSearch.FindNextBracketedString (startPos, endPos, txtSearchStr))
		mValid = false;

	else {

		startPos = 0;
		TextSearch .ResetSearch ();

		while (TextSearch.FindNextBracketedString (startPos, endPos, textString)) {

			startPos = endPos;
			newString = new RGString (textString);
			mList.Append (newString);
		}
	}
}


SynonymList :: ~SynonymList () {

	mList.ClearAndDelete ();
}


void SynonymList :: AddSynonym (const RGString& str) {

	RGString* newString = new RGString (str);
	mList.Append (newString);
}


void SynonymList :: AddSynonym (const char* str) {

	RGString* newString = new RGString (str);
	mList.Append (newString);
}


bool SynonymList :: DoesTargetStringEqualASynonym (const RGString& target) {

	RGString* nextString;
	mIt.Reset ();

	while (nextString = (RGString*) mIt()) {

		if (target.IsEqualTo (nextString))
			return true;
	}

	return false;
}


bool SynonymList :: DoesTargetStringContainASynonym (const RGString& target) {

	RGString* nextString;
	mIt.Reset ();
	size_t pos = 0;

	while (nextString = (RGString*) mIt()) {

		if (target.FindSubstring (*nextString, pos))
			return true;
	}

	return false;
}


bool SynonymList :: DoesTargetStringEqualASynonymCaseIndep (const RGString& target) {

	RGString lcTarget (target);
	lcTarget.ToLower ();
	mIt.Reset ();
	RGString test;
	RGString* nextString;

	while (nextString = (RGString*) mIt ()) {

		test = *nextString;
		test.ToLower ();

		if (lcTarget.IsEqualTo (&test))
			return true;
	}

	return false;
}



bool SynonymList :: DoesTargetStringContainASynonymCaseIndep (const RGString& target) {

	mIt.Reset ();
	RGString* nextString;
	size_t pos = 0;

	while (nextString = (RGString*) mIt ()) {

		if (target.FindSubstringCaseIndependent (*nextString, pos))
			return true;
	}

	return false;
}

