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
*  FileName: LadderData.h
*  Author:   Robert Goor
*
*/
//
// class LadderSets reads in Marker Set, Locus and Allele information and organizes it to be able to retrieve relevant info.  Each
// individual LadderData object returns information for a given Marker Set
//

#ifndef _LADDERDATA_H_
#define _LADDERDATA_H_

#include "rgdefs.h"
#include "rgpersist.h"
#include "rgtree.h"
#include "rgstring.h"
#include "DataSignal.h"
#include "rghashtable.h"
#include "rgdlist.h"

class CoordinateTransform;
struct SupplementaryData;


const int _MARKERID_ = 1030;
const int _LOCUSID_	= 1031;
const int _ALLELEDATA_ = 1032;
const int _LADDERDATA_ = 1033;

PERSISTENT_PREDECLARATION (MarkerID)
PERSISTENT_PREDECLARATION (LocusID)
PERSISTENT_PREDECLARATION (AlleleData)
PERSISTENT_PREDECLARATION (LadderData)

class MarkerID : public RGPersistent {

PERSISTENT_DECLARATION (MarkerID)

public:
	MarkerID ();
	MarkerID (const RGString& setName, int setID);
	~MarkerID ();

	int GetSetID () const { return SetID; }
	RGString GetSetName () const { return SetName; }

	static void SetSearchByName () { SearchByName = TRUE; }
	static void SetSearchByID () { SearchByName = FALSE; }

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

protected:
	RGString SetName;
	int SetID;

	static Boolean SearchByName;
};


class LocusID : public RGPersistent {

PERSISTENT_DECLARATION (LocusID)

public:
	LocusID ();
	LocusID (const RGString& locusName, int locusID);
	LocusID (const LocusID& lid);
	~LocusID ();

	LocusID& operator=(const LocusID& lid);

	int GetLocusID () const { return LocusId; }
	RGString GetLocusName () const { return LocusName; }

	void SetLocusID (int locusID) { LocusId = locusID; }
	void SetLocusName (const RGString& locusName) { LocusName = locusName; }

	static void SetSearchByName () { SearchByName = TRUE; }
	static void SetSearchByID () { SearchByName = FALSE; }

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

protected:
	RGString LocusName;
	int LocusId;

	static Boolean SearchByName;
};


class AlleleData : public RGPersistent {

PERSISTENT_DECLARATION (AlleleData)

public:
	AlleleData ();
	AlleleData (int locusID, const RGString& alleleName, int bp, int curve, int channel);
	AlleleData (const LocusID* id, const RGString& alleleName, int bp, int curve, int channel);
	AlleleData (const AlleleData& ad);
	~AlleleData ();

	AlleleData& operator=(const AlleleData& ad);

	int GetLocusID () const { return mLocusID; }
	RGString GetAlleleName () const { return AlleleName; }
	int GetBP () const { return BP; }
	int GetCurveNumber () const { return Curve; }
	int GetChannel () const { return Channel; }
	const DataSignal* GetCurvePointer () const { return CorrespondingPeak; }

	void SetLocusID (int locusID) { mLocusID = locusID; }
	void SetLocusID (const LocusID* id) { mLocusID = id->GetLocusID (); }
	void SetAlleleName (const RGString& name) { AlleleName = name; }

	void SetChannel (int channel) { Channel = channel; }
	void SetCurve (int curve) { Curve = curve; }

	void SetCurvePointer (DataSignal* ds) { CorrespondingPeak = ds; }

	static void SetSearchByName () { SearchByName = TRUE; }
	static void SetSearchByChannelCurve () { SearchByName = FALSE; }

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

protected:
	int mLocusID;
	RGString AlleleName;
	int BP;
	int Curve;
	int Channel;
	DataSignal* CorrespondingPeak;

	static Boolean SearchByName;
};


class LadderData : public RGPersistent {

PERSISTENT_DECLARATION (LadderData)

public:
	LadderData ();
	LadderData (RGString& dataString);
	LadderData (const LadderData& ld);
	~LadderData ();

	Boolean IsValid () const { return Valid; }
	Boolean RepresentsMarkerSet (const RGString& markerSet) const;
	RGString GetMarkerSet () const { return MarkerSet; }

	int InsertChannelPeaks (int channel, RGDList& curveList, const SupplementaryData& extraData);
	int BuildILSSplineBPToTime (const SupplementaryData& extraData);
	void SetMarkerSet (const RGString& newSet) { MarkerSet = newSet; }

	const DataSignal* GetCurve (const RGString& locus, const RGString& allele, int& channel);
	double GetTimeForCorrespondingBP (double bp, int channel);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

protected:
	RGString MarkerSet;
	RGHashTable LocusList;
	RGHashTable AlleleListByName;
	RGHashTable AlleleListByChannelCurve;
	Boolean Valid;
	CoordinateTransform** splineBPToTime;
};


class LadderSets {

public:
	LadderSets (const RGString& relativePath);
	~LadderSets ();

	Boolean IsValid () const { return Valid; }

	LadderData* GetLadderDataForMarkerSet (const RGString& markerSet);

protected:
	RGHashTable MarkerSets;
	Boolean Valid;
};


#endif  /*  _LADDERDATA_H_  */

