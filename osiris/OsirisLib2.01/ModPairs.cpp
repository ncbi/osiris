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

	ModPair* next;

	while (!mChannelPairs.empty ()) {

		next = mChannelPairs.front ();
		mChannelPairs.pop_front ();
		delete next;
	}
}


void ChannelModPairs::AddModPair (const ModPair* mp) {

	ModPair* newMP = (ModPair*)mp;
	mChannelPairs.push_back (newMP);
}



void ChannelModPairs::SetModRegions (bool* modArray, int size) {

	list<ModPair*>::iterator pairIt;
	ModPair* nextPair;
	int i = 0;

	for (pairIt=mChannelPairs.begin (); pairIt!=mChannelPairs.end (); pairIt++) {

		nextPair = *pairIt;
		nextPair->SetModRegion (modArray, size);
		i++;
	}

	//cout << "Set mods for " << i << " regions\n";
}



bool ChannelModPairs::ReadModPair (const RGString& inString) {

	ModPair* mp = new ModPair ();
	bool result = mp->ReadModPair (inString);
	mChannelPairs.push_back (mp);
	return result;
}


void ChannelModPairs::PrintList () {

	list<ModPair*>::iterator pairIt;
	ModPair* nextPair;
	cout << "Channel number " << mChannel << " pairs:\n";

	for (pairIt=mChannelPairs.begin (); pairIt!=mChannelPairs.end (); pairIt++) {

		nextPair = *pairIt;
		nextPair->PrintPair ();
	}
}


SampleModList::SampleModList (const RGString& name, int nChannels) : mSampleName (name), mWasUsed (false), mChannels (nChannels) {

	mChannelPairs = new ChannelModPairs* [nChannels + 1];
	int i;

	for (i=1; i<=nChannels; i++)
		mChannelPairs [i] = new ChannelModPairs (nChannels);
}


SampleModList::SampleModList (int nChannels) : mWasUsed (false), mChannels (nChannels) {

	mChannelPairs = new ChannelModPairs* [nChannels + 1];
	int i;

	for (i=1; i<=nChannels; i++)
		mChannelPairs [i] = new ChannelModPairs (i);
}



SampleModList::~SampleModList () {

	int i;
	ChannelModPairs* cmp;

	for (i=1; i<=mChannels; i++) {

		cmp = mChannelPairs [i];
		delete cmp;
	}

	delete[] mChannelPairs;
}



void SampleModList::AddModPair (int channel, const ModPair* mp) {

	mChannelPairs [channel]->AddModPair (mp);
}



void SampleModList::BeingUsedForSample (const RGString& name) {

	if (mWasUsed) {

		cout << "<*****>Duplicate mod pairs use:  Mod pairs used for " << mUsedBySampleName << " now being used by " << name << "</*****>\n";
		cout << "<Ping>651</Ping>" << endl;
	}

	mUsedBySampleName = name;
	mWasUsed = true;
}



ChannelModPairs* SampleModList::GetModPairsForChannel (int channelNumber) {

	return mChannelPairs [channelNumber];
}




//Need read sample list function
RGString SampleModList::ReadSampleModList (const RGString& sampleString) {

	RGString XMLString = sampleString;
	RGString ignoreString;
	RGString channelString;
	RGString name;
	RGXMLTagSearch nameSearch ("Name", XMLString);
	RGXMLTagSearch ignoreSearch ("Ignore", XMLString);
	RGXMLTagSearch channelSearch ("Channel", ignoreString);
	size_t startIndex = 0;
	size_t endIndex;
	size_t endIgnore;
	int channel;

	if (!nameSearch.FindNextTag (startIndex, endIndex, name)) {

		cout << "Couldn't find name" << endl;
		return "";
	}

	mSampleName = name;
	startIndex = endIndex + 1;

	while (true) {

		if (!ignoreSearch.FindNextTag (startIndex, endIndex, ignoreString)) {

			break;
		}
		
		startIndex = endIndex + 1;
		channelSearch.ResetSearch ();

		if (!channelSearch.FindNextTag (0, endIgnore, channelString)) {

			//Error output!!!  no channel number
			cout << "Couldn't find channel" << endl;
			return "";
		}

		else {

			channel = channelString.ConvertToInteger ();
			mChannelPairs [channel]->ReadModPair (ignoreString);
		}
	}

	return name;
}


void SampleModList::PrintList () {

	cout << "Sample " << mSampleName << " pairs:\n";
	int i;

	for (i=1; i<=mChannels; i++)
		mChannelPairs [i]->PrintList ();
}



OverallModList::OverallModList (int nChannels) : mChannels (nChannels) {

}



OverallModList::~OverallModList () {

	SampleModList* next;

	while (!mSampleList.empty ()) {

		next = mSampleList.front ();
		mSampleList.pop_front ();
		delete next;
	}
}

void OverallModList::AddSampleModList (const RGString& name) {

	list<SampleModList*>::iterator it;
	SampleModList* next;
	RGString currentName;

	for (it=mSampleList.begin (); it!=mSampleList.end (); it++) {

		next = *it;
		currentName = next->GetName ();

		if (name == currentName)
			return;
	}

	next = new SampleModList (name, mChannels);
}



SampleModList* OverallModList::GetSampleModList (const RGString& name) {

	list<SampleModList*>::iterator it;
	SampleModList* next;
	RGString currentName;
	size_t pos = 0;

	for (it=mSampleList.begin (); it!=mSampleList.end (); it++) {

		next = *it;
		currentName = next->GetName ();

		if (name.FindSubstringCaseIndependent (currentName, pos))
			return next;
	}

	return NULL;
}



//Need read overall list function
RGString OverallModList::ReadNextSampleData (const RGString& sampleData, RGString& sampleName) {

	SampleModList* next = new SampleModList (mChannels);
	sampleName = next->ReadSampleModList (sampleData);
	mSampleList.push_back (next);
	return sampleName;
}



void OverallModList::ReadOverallModList (const RGString& sampleData) {

	RGString XMLString = sampleData;
	RGString sampleString;
	RGString sampleName;
	RGXMLTagSearch sampleSearch ("Sample", XMLString);
	size_t startIndex = 0;
	size_t endIndex;

	//cout << "Searching sample data:\n";
	//cout << XMLString << endl;

	while (sampleSearch.FindNextTag (startIndex, endIndex, sampleString)) {

		//cout << "Sample String:\n";
		//cout << sampleString << endl;
		startIndex = endIndex + 1;
		ReadNextSampleData (sampleString, sampleName);

		if (sampleName == "")
			cout << "Could not find sample name in mods data..." << endl;
	}
}


void OverallModList::PrintList () {

	list<SampleModList*>::iterator it;
	SampleModList* next;
	RGString currentName;

	cout << "Overall modifications list:\n";

	for (it=mSampleList.begin (); it!=mSampleList.end (); it++) {

		next = *it;
		next->PrintList ();
	}
}

