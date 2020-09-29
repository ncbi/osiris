#pragma once
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

#ifndef _MODPAIRS_H_
#define _MODPAIRS_H_

#include "RGString.h"
#include <list>


class ModPair {

public:
	ModPair () {}
	ModPair (int low, int high) : mLow (low), mHigh (high) {}
	~ModPair () {}
	bool ReadModPair (const RGString& inString);
	void SetModRegion (bool* modArray, int size);
	void PrintPair () {
		cout << "Low = " << mLow << " and High = " << mHigh << "\n";
	}

protected:
	int mLow;
	int mHigh;
};


class ChannelModPairs {

public:
	ChannelModPairs (int channelNumber) : mChannel (channelNumber) {}
	~ChannelModPairs ();

	bool isEmpty () const { mChannelPairs.empty (); }
	void AddModPair (const ModPair* mp);
	void SetModRegions (bool* modArray, int size);
	bool ReadModPair (const RGString& inString);
	void PrintList ();

protected:
	list<ModPair*> mChannelPairs;
	int mChannel;
};


class SampleModList {

public:
	SampleModList (const RGString& name, int nChannels);
	SampleModList (int nChannels);
	~SampleModList ();

	void AddModPair (int channel, const ModPair* mp);
	void BeingUsedForSample (const RGString& name);
	ChannelModPairs* GetModPairsForChannel (int channelNumber);
	RGString GetName () const { return mSampleName; }

	//Need read sample list function
	RGString ReadSampleModList (const RGString& sampleString);
	void PrintList ();

protected:
	RGString mSampleName;
	ChannelModPairs** mChannelPairs;
	bool mWasUsed;
	RGString mUsedBySampleName;
	int mChannels;
};


class OverallModList {

public:
	OverallModList (int nChannels);
	~OverallModList ();

	void AddSampleModList (const RGString& name);
	SampleModList* GetSampleModList (const RGString& name);
	//Need read overall list function
	RGString ReadNextSampleData (const RGString& sampleData, RGString& sampleName);
	void ReadOverallModList (const RGString& sampleData);
	void PrintList ();

protected:
	list<SampleModList*> mSampleList;
	int mChannels;
};


#endif

