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
*  FileName: ModPairs.h
*  Author:   Robert Goor
*
*/
//
//     class ModPair and ChannelModPairs and SampleModLists
//


#include "ModPairs.h"
#include "rgtokenizer.h"

bool ModPair::ReadModPair (const RGString& inString) {

	size_t startIndex = 0;
	size_t endIndex;
	RGString resultString;
	RGString XMLString = inString;
	RGXMLTagSearch lowSearch ("Low", XMLString);
	RGXMLTagSearch highSearch ("High", XMLString);

	if (!lowSearch.FindNextTag (startIndex, endIndex, resultString)) {

		return false;
	}

	mLow = resultString.ConvertToInteger ();
	startIndex = endIndex + 1;

	if (!highSearch.FindNextTag (startIndex, endIndex, resultString)) {

		return false;
	}

	mHigh = resultString.ConvertToInteger ();
	return true;
}


void ModPair::SetModRegion (bool* modArray, int size) {

	int low = mLow;
	int high = mHigh;

	if (high >= size)
		high = size - 1;

	if (low < 0)
		low = 0;

	int i;

	for (i=low; i<=high; i++)
		modArray [i] = true;
}


ChannelModPairs::~ChannelModPairs () {

}


void ChannelModPairs::AddModPair (const ModPair* mp) {

	ModPair* newMP = (ModPair*)mp;
	mChannelPairs.push_back (newMP);
}



void ChannelModPairs::SetModRegions (bool* modArray, int size) {

	list<ModPair*>::iterator pairIt;
	ModPair* nextPair;

	for (pairIt=mChannelPairs.begin (); pairIt!=mChannelPairs.end (); pairIt++) {

		nextPair = *pairIt;
		nextPair->SetModRegion (modArray, size);
	}
}



bool ChannelModPairs::ReadModPair (const RGString& inString) {

	ModPair* mp = new ModPair ();
	bool result = mp->ReadModPair (inString);
	return result;
}


