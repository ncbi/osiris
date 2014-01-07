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
*  FileName: LadderData.cpp
*  Author:   Robert Goor
*
*/
//
// class LadderSets reads in Marker Set, Locus and Allele information and organizes it to be able to retrieve relevant info.  Each
// individual LadderData object returns information for a given Marker Set
//


#include "LadderData.h"
#include "coordtrans.h"
#include "rgtokenizer.h"
#include "rgfile.h"

/*
#include <iostream>
#include <stdlib.h>

using namespace std;  */


Boolean MarkerID::SearchByName = TRUE;
Boolean LocusID::SearchByName = TRUE;
Boolean AlleleData::SearchByName = TRUE;


PERSISTENT_DEFINITION (MarkerID, _MARKERID_, "MarkerID")
PERSISTENT_DEFINITION (LocusID, _LOCUSID_, "LocusID")
PERSISTENT_DEFINITION (AlleleData, _ALLELEDATA_, "AlleleData")
PERSISTENT_DEFINITION (LadderData, _LADDERDATA_, "LadderData")


MarkerID :: MarkerID () : RGPersistent (), SetID (-1) {

}


MarkerID :: MarkerID (const RGString& setName, int setID) : RGPersistent (),
SetName (setName), SetID (setID) {

}


MarkerID :: ~MarkerID () {

}


int MarkerID :: CompareTo (const RGPersistent* p) const {

	MarkerID* q = (MarkerID*)p;

	if (MarkerID::SearchByName) {

		return SetName.CompareTo (&(q->SetName));
	}

	if (SetID < q->SetID)
		return -1;

	if (SetID == q->SetID)
		return 0;

	return 1;
}


unsigned MarkerID :: HashNumber (unsigned long Base) const {

	if (MarkerID::SearchByName)
		return SetName.HashNumber (Base);

	return SetID%Base;
}


Boolean MarkerID :: IsEqualTo (const RGPersistent* p) const {

	MarkerID* q = (MarkerID*)p;

	if (MarkerID::SearchByName) {

		return SetName.IsEqualTo (&(q->SetName));
	}

	return SetID == q->SetID;
}


LocusID :: LocusID () : RGPersistent (), LocusId (-1) {

}


LocusID :: LocusID (const RGString& locusName, int locusId) : RGPersistent (),
LocusName (locusName), LocusId (locusId) {

}


LocusID :: LocusID (const LocusID& lid) : RGPersistent (), LocusName (lid.LocusName),
LocusId (lid.LocusId) {

}


LocusID :: ~LocusID () {

}


LocusID& LocusID :: operator=(const LocusID& lid) {

	if (this == &lid)
		return *this;

	LocusName = lid.LocusName;
	LocusId = lid.LocusId;
	return *this;
}



int LocusID :: CompareTo (const RGPersistent* p) const {

	LocusID* q = (LocusID*)p;
	int tempID;

	if (LocusID::SearchByName) {

		return LocusName.CompareTo (&(q->LocusName));
	}

	tempID = LocusId - q->LocusId;

	if (tempID < 0)
		return -1;

	if (tempID == 0)
		return 0;

	return 1;
}


unsigned LocusID :: HashNumber (unsigned long Base) const {

	if (LocusID::SearchByName) {

		return LocusName.HashNumber (Base);
	}

	return LocusId%Base;
}


Boolean LocusID :: IsEqualTo (const RGPersistent* p) const {

	LocusID* q = (LocusID*)p;

	if (LocusID::SearchByName) {

		return LocusName.IsEqualTo (&(q->LocusName));
	}

	return LocusId == q->LocusId;
}


AlleleData :: AlleleData () : RGPersistent (), mLocusID (-1), BP (0), Curve (-1), Channel (-1), 
CorrespondingPeak (NULL) {

}


AlleleData :: AlleleData (int locusID, const RGString& alleleName, int bp, int curve, int channel) : RGPersistent (),
mLocusID (locusID), AlleleName (alleleName), BP (bp), Curve (curve), Channel (channel), CorrespondingPeak (NULL) {

}


AlleleData :: AlleleData (const LocusID* id, const RGString& alleleName, int bp, int curve, int channel) : RGPersistent (),
mLocusID (id->GetLocusID ()), AlleleName (alleleName), BP (bp), Curve (curve), Channel (channel), CorrespondingPeak (NULL) {

}


AlleleData :: AlleleData (const AlleleData& ad) : RGPersistent (),
mLocusID (ad.mLocusID), AlleleName (ad.AlleleName), BP (ad.BP), Curve (ad.Curve), Channel (ad.Channel), CorrespondingPeak (NULL) {

}


AlleleData :: ~AlleleData () {

}


AlleleData& AlleleData :: operator=(const AlleleData& ad) {

	if (this == &ad)
		return *this;

	mLocusID = ad.mLocusID;
	AlleleName = ad.AlleleName;
	BP = ad.BP;
	Curve = ad.Curve;
	Channel = ad.Channel;
	CorrespondingPeak = NULL;
	return *this;
}


int AlleleData :: CompareTo (const RGPersistent* p) const {

	AlleleData* q = (AlleleData*)p;
	int temp;
	int tempID;

	if (AlleleData::SearchByName) {

		tempID = mLocusID - q->mLocusID;

		if (tempID < 0)
			return -1;

		if (tempID == 0) {

			temp = AlleleName.CompareTo (&(q->AlleleName));

			if (temp < 0)
				return -1;

			if (temp == 0)
				return 0;

			return 1;
		}

		return 1;
	}

	temp = Channel - q->Channel;
	tempID = Curve - q->Curve;

	if (temp < 0)
		return -1;

	if (temp == 0) {

		if (tempID < 0)
			return -1;

		if (tempID == 0)
			return 0;

		return 1;
	}

	return 1;
}


unsigned AlleleData :: HashNumber (unsigned long Base) const {

	if (AlleleData::SearchByName) {

		return (mLocusID * AlleleName.HashNumber (Base))%Base;
	}

	return (Channel * Curve)%Base;
}


Boolean AlleleData :: IsEqualTo (const RGPersistent* p) const {

	AlleleData* q = (AlleleData*)p;

	if (AlleleData::SearchByName)
		return ((mLocusID == q->mLocusID) && (AlleleName.IsEqualTo (&(q->AlleleName))));

	return ((Channel == q->Channel) && (Curve == q->Curve));
}


LadderData :: LadderData () : RGPersistent (), Valid (FALSE) {

	splineBPToTime = new CoordinateTransform* [5];

	for (int i=0; i<=4; i++)
		splineBPToTime [i] = NULL;
}


LadderData :: LadderData (RGString& dataString) : RGPersistent (),
Valid (TRUE) {

	RGString Allele;
	RGString Locus;
	RGString AlleleName;
	RGString LocusName;
	RGString LocusChannel;
	RGString BP;
	RGString CurveNo;
	RGBracketStringSearch SetNameSearch ("<Name>", "</Name>", dataString);
	RGBracketStringSearch LocusSearch ("<Locus>", "</Locus>", dataString);
	RGBracketStringSearch LocusNameSearch ("<Name>", "</Name>", Locus);
	RGBracketStringSearch LocusChannelSearch ("<Channel>", "</Channel>", Locus);
	RGBracketStringSearch AlleleSearch ("<Allele>", "</Allele>", Locus);
	RGBracketStringSearch AlleleNameSearch ("<Name>", "</Name>", Allele);
	RGBracketStringSearch AlleleBPSearch ("<BP>", "</BP>", Allele);
	RGBracketStringSearch AlleleCurveNoSearch ("<CurveNo>", "</CurveNo>", Allele);
	size_t startLocusPos = 0;
	size_t endLocusPos;
	size_t endPos;
	int locusID = 0;
	LocusID* newLocus;
	size_t startAllelePos;
	size_t endAllelePos;
	int channel;
	int bp;
	int curve;
	AlleleData* newAllele;
	LocusID::SetSearchByName ();
//	char c;

	splineBPToTime = new CoordinateTransform* [5];

	for (int i=0; i<=4; i++)
		splineBPToTime [i] = NULL;

	if (!SetNameSearch.FindNextBracketedString (0, endPos, MarkerSet))
		Valid = FALSE;

//	cout << "Found marker set named:  " << MarkerSet << endl;

	while (LocusSearch.FindNextBracketedString (startLocusPos, endLocusPos, Locus)) {

		LocusNameSearch.ResetSearch ();
		LocusChannelSearch.ResetSearch ();
		AlleleSearch.ResetSearch ();

		if (!LocusNameSearch.FindNextBracketedString (0, endPos, LocusName))
			Valid = FALSE;

		locusID++;
//		cout << "    Found locus named:  " << LocusName << " and id = " << locusID << endl;
		newLocus = new LocusID (LocusName, locusID);
		LocusList.Append (newLocus);

		if (!LocusChannelSearch.FindNextBracketedString (0, endPos, LocusChannel))
			Valid = FALSE;

		channel = LocusChannel.ConvertToInteger ();
		startAllelePos = 0;
		curve = 0;

		while (AlleleSearch.FindNextBracketedString (startAllelePos, endAllelePos, Allele)) {

			startAllelePos = endAllelePos;
			AlleleNameSearch.ResetSearch ();
			AlleleBPSearch.ResetSearch ();
			AlleleCurveNoSearch.ResetSearch ();
			curve++;

			if (!AlleleNameSearch.FindNextBracketedString (0, endPos, AlleleName))
				Valid = FALSE;

			if (!AlleleBPSearch.FindNextBracketedString (endPos, endPos, BP))
				Valid = FALSE;

			//if (!AlleleCurveNoSearch.FindNextBracketedString (0, endPos, CurveNo))
			//	Valid = FALSE;

			bp = BP.ConvertToInteger ();
//			curve = CurveNo.ConvertToInteger ();
//			cout << "          Found allele named:  " << AlleleName << " with locusID = " << newLocus->GetLocusID () << ", channel = " << channel;
//			cout << ", BP = " << bp << " and curve number = " << curve << endl;
			newAllele = new AlleleData (newLocus, AlleleName, bp, curve, channel);
			AlleleData::SetSearchByName ();
			AlleleListByName.Append (newAllele);
			AlleleData::SetSearchByChannelCurve ();
			AlleleListByChannelCurve.Append (newAllele);
		}

		startLocusPos = endLocusPos;
	}

//	cout << "Input a character..." << endl;
//	cin >> c;
}


LadderData :: LadderData (const LadderData& ld) : RGPersistent (), MarkerSet (ld.MarkerSet),
LocusList (ld.LocusList), AlleleListByName (ld.AlleleListByName), AlleleListByChannelCurve ((ld.AlleleListByChannelCurve).GetNumberOfTables()),
Valid (ld.Valid) {

	splineBPToTime = new CoordinateTransform* [5];

	for (int i=0; i<=4; i++)
		splineBPToTime [i] = NULL;

	AlleleData::SetSearchByChannelCurve ();
	RGHashTableIterator it (AlleleListByName);
	AlleleData* ad;

	while (ad = (AlleleData*) it()) {

		AlleleListByChannelCurve.Insert (ad);
	}
}



LadderData :: ~LadderData () {

	LocusList.ClearAndDelete ();
	AlleleListByChannelCurve.Clear ();
	AlleleListByName.ClearAndDelete ();
	
	for (int i=0; i<=4; i++)
		delete splineBPToTime [i];

	delete[] splineBPToTime;
}



Boolean LadderData :: RepresentsMarkerSet (const RGString& markerSet) const {

	return MarkerSet.IsEqualTo (&(markerSet));
}


int LadderData :: InsertChannelPeaks (int channel, RGDList& curveList, const SupplementaryData& extraData) {

	AlleleData::SetSearchByChannelCurve ();
	RGDListIterator it (curveList);
	AlleleData target;
	target.SetChannel (channel);
	DataSignal* nextSignal;
	int i = 1;
	AlleleData* FoundAllele;
	int status = 0;

	int N = curveList.Entries ();
	double* bp = new double [N];
	double* time = new double [N];

//	char c;
//	cout << "Populating alleles for channel number " << channel << endl;

	while (nextSignal = (DataSignal*)it ()) {

		target.SetCurve (i);
		FoundAllele = (AlleleData*) AlleleListByChannelCurve.Find (&target);

		if (FoundAllele) {

			FoundAllele->SetCurvePointer (nextSignal);
			bp [i - 1] = (double)FoundAllele->GetBP ();
			time [i - 1] = nextSignal->GetMean ();
//			cout << "Found allele for curve " << i << " named " << FoundAllele->GetAlleleName () << endl;
		}

		else
			status = -1;

		i++;
	}

	if (status >= 0) {

		delete splineBPToTime [channel];
		splineBPToTime [channel] = new CSplineTransform (bp, time, N, extraData);
	}

	delete[] bp;
	delete[] time;

//	cout << "status = " << status << endl;
//	cout << "Input a character..." << endl;
//	cin >> c;
	return status;
}


int LadderData :: BuildILSSplineBPToTime (const SupplementaryData& extraData) {

	delete splineBPToTime [4];
	splineBPToTime [4] = new CSplineTransform (extraData.abscissas, extraData.ordinates, extraData.size);
	return 0;
}


const DataSignal* LadderData :: GetCurve (const RGString& locus, const RGString& allele, int& channel) {

	int locusID;
	LocusID::SetSearchByName ();
	LocusID LID;
	LID.SetLocusName (locus);
	LocusID* FoundLocus = (LocusID*) LocusList.Find (&LID);

	if (!FoundLocus)
		return NULL;

	locusID = FoundLocus->GetLocusID ();
	AlleleData::SetSearchByName ();
	AlleleData target;
	target.SetLocusID (locusID);
	target.SetAlleleName (allele);
	AlleleData* FoundAllele = (AlleleData*) AlleleListByName.Find (&target);

	if (!FoundAllele)
		return NULL;

	channel = FoundAllele->GetChannel ();
	return FoundAllele->GetCurvePointer ();
}


double LadderData :: GetTimeForCorrespondingBP (double bp, int channel) {

	if (splineBPToTime [channel] == NULL)
		return -1.0;

//	return splineBPToTime [channel]->Evaluate ((double)bp);
	return splineBPToTime [channel]->EvaluateWithExtrapolation (bp);
}


int LadderData :: CompareTo (const RGPersistent* p) const {

	LadderData* q = (LadderData*) p;

	return MarkerSet.CompareTo (&(q->MarkerSet));
}


unsigned LadderData :: HashNumber (unsigned long Base) const {

	return MarkerSet.HashNumber (Base);
}


Boolean LadderData :: IsEqualTo (const RGPersistent* p) const {

	LadderData* q = (LadderData*) p;

	return MarkerSet.IsEqualTo (&(q->MarkerSet));
}



LadderSets :: LadderSets (const RGString& relativePath) : Valid (TRUE) {

	size_t N = relativePath.Length () - 1;
	char EndChar = relativePath.GetCharacter (N);
	RGString fileName;

	if (EndChar == '/')
		fileName = relativePath + "LadderData.xml";

	else
		fileName = relativePath + "/LadderData.xml";

	RGString dataString;
	RGString SetString;
	RGBracketStringSearch SetSearch ("<Set>", "</Set>", dataString);
	size_t startSet = 0;
	size_t endSet;
	LadderData* nextMarkerSet;

	RGFile file (fileName, "rt");

	if (!file.isValid ())
		Valid = FALSE;

	else {

		dataString.ReadTextFile (file);
		SetSearch.ResetSearch ();

		while (SetSearch.FindNextBracketedString (startSet, endSet, SetString)) {

			nextMarkerSet = new LadderData (SetString);
			MarkerSets.Append (nextMarkerSet);

			if (!nextMarkerSet->IsValid ())
				Valid = FALSE;

			startSet = endSet;
		}
	}
}


LadderSets :: ~LadderSets () {

	MarkerSets.ClearAndDelete ();
}


LadderData* LadderSets :: GetLadderDataForMarkerSet (const RGString& markerSet) {

	LadderData ladder;
	ladder.SetMarkerSet (markerSet);
	return (LadderData*) MarkerSets.Find (&ladder);
}


