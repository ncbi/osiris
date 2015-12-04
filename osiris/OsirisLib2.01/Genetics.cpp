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
*  FileName: Genetics.cpp
*  Author:   Robert Goor
*
*/
//
//   These are the abstract base classes for the population genetic categories:  class BaseAllele, BaseLocus, PopulationMarkerSet and PopulationCollection,
// class Allele and Locus...
//

#include "Genetics.h"
#include "DataSignal.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgtokenizer.h"
#include "coordtrans.h"
#include "RGTextOutput.h"
#include "OsirisMsg.h"
#include "OutputLevelManager.h"
#include "STRNotices.h"
#include "ControlFit.h"
#include "ListFunctions.h"
#include "RecursiveInnerProduct.h"
#include "ChannelData.h"
#include "IndividualGenotype.h"
#include "SmartMessage.h"
#include "SmartNotice.h"
#include "STRSmartNotices.h"
#include "CoreBioComponent.h"

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

double Locus::MinimumFractionOfAverageWidth = 0.5;
double Locus::MaximumMultipleOfAverageWidth = 2.0; 
double Locus::MinimumFractionOfAveragePeak = 0.2;
double Locus::MaximumMultipleOfAveragePeak = -1.0;

double Locus::MinimumFractionOfAverageSampleWidth = 0.2;
double Locus::MaximumMultipleOfAverageSampleWidth = 3.0; 
double Locus::MinimumFractionOfAverageSamplePeak = 0.2;
double Locus::MaximumMultipleOfAverageSamplePeak = -1.0;

double Locus::HeterozygousImbalanceLimit = -1.0;
double Locus::MinBoundForHomozygote = -1.0;

double Locus::SampleStutterThreshold = 0.15;
double Locus::SamplePlusStutterThreshold = 0.05;
double Locus::SampleAdenylationThreshold = 0.3333;
double Locus::GridStutterThreshold = 0.15;
double Locus::GridAdenylationThreshold = 0.3333;

double Locus::SampleFractionalFilter = -1.0;
double Locus::GridFractionalFilter = -1.0;
double Locus::SamplePullupFractionalFilter = -1.0;
double Locus::GridPullupFractionalFilter = -1.0;
bool Locus::ExpectRFUUnitsForHomozygoteBound = true;
double Locus::MaxResidualForAlleleCalls = -1.0;
double Locus::AlleleOverloadThreshold = -1.0;
bool* Locus::InitialMatrix = NULL;
bool Locus::NoYForAMEL = false;
bool Locus::DisableStutterFilter = false;
bool Locus::DisableAdenylationFilter = false;
bool Locus::CallOnLadderAdenylation = false;


Boolean Locus::TestRatio = TRUE;


PERSISTENT_DEFINITION (Allele, _ALLELE_, "Allele")
PERSISTENT_DEFINITION (Locus, _LOCUS_, "Locus")
PERSISTENT_DEFINITION (LaneStandard, _LANESTANDARD_, "LaneStandard")
PERSISTENT_DEFINITION (PopulationMarkerSet, _POPULATIONMARKERSET_, "PopulationMarkerSet")


IdealControlSetInfo :: IdealControlSetInfo (const double* pts, const double* diffs, const double* leftNorms, const double* rightNorms, const int* htInd, int size, bool accurate) :
mPts (pts), mDiffs (diffs), mLeftNorm2s (leftNorms), mRightNorm2s (rightNorms), mHeightIndices (htInd), mHeightRangeInfo (NULL), 
mSetSize (size), mCapMaxHeightIndex (true), mSignalIntervalIsAccurate (accurate), mDiscrepancy (2) {

}


void SetDifference (RGDList& listA, RGDList& listB, RGDList& AButNotB, RGDList& BButNotA) {

	RGDListIterator itA (listA);
	RGDListIterator itB (listB);
	RGPersistent* p;

	while (p = itA ()) {

		if (!listB.Contains (p))
			AButNotB.Append (p);
	}

	while (p = itB ()) {

		if (!listA.Contains (p))
			BButNotA.Append (p);
	}
}


Allele :: Allele () : RGPersistent (), mLink (NULL), CorrespondingSignal (NULL), Linked (FALSE), Valid (TRUE) {

	mLink = new BaseAllele ();
}


Allele :: Allele (BaseAllele* link, const RGString& xmlString) : RGPersistent (), mLink (link), CorrespondingSignal (NULL),
Linked (TRUE) {

	Valid = mLink->IsValid ();

	if (!Valid)
		Msg += mLink->GetError ();
}


Allele :: Allele (const Allele& allele) : RGPersistent (), CorrespondingSignal (allele.CorrespondingSignal) {

	if (allele.Linked) {

		mLink = allele.mLink;
		Linked = TRUE;
		Valid = allele.Valid;
	}

	else {

		mLink = new BaseAllele ();
		Linked = FALSE;
		Valid = TRUE;
	}
}


Allele :: Allele (const Allele& allele, CoordinateTransform* trans) : RGPersistent () {

	if (allele.Linked) {

		mLink = allele.mLink;
		Linked = TRUE;
		Valid = allele.Valid;

		if (allele.CorrespondingSignal != NULL) {

			CorrespondingSignal = mLink->GetNewPrimarySignature (*allele.CorrespondingSignal, trans);
			CorrespondingSignal->SetAlleleName (allele.GetAlleleName ());
			CorrespondingSignal->SetBioID (allele.CorrespondingSignal->GetBioID ());
			CorrespondingSignal->SetApproximateBioID (allele.CorrespondingSignal->GetApproximateBioID ());
			CorrespondingSignal->SetApproxBioIDPrime (allele.CorrespondingSignal->GetApproxBioIDPrime ());
			CorrespondingSignal->SetCurveFit (allele.CorrespondingSignal->GetCurveFit ());
		}
	}

	else {

		mLink = new BaseAllele ();
		Linked = FALSE;
		Valid = TRUE;
	}
}


Allele :: ~Allele () {

	if (!Linked)
		delete mLink;
}


void Allele :: ClearAndDestroy () {

	delete mLink;
	mLink = NULL;
}


void Allele :: SetCurvePointer (DataSignal* ds) {

	CorrespondingSignal = ds;
	CorrespondingSignal->SetBioID ((double)mLink->GetBioID ());
	CorrespondingSignal->SetAlleleName (mLink->GetAlleleName ());
}


void Allele :: SetBioID (int id) {

	mLink->SetBioID (id);
}


int Allele :: GetBioID () const {

	return mLink->GetBioID ();
}


int Allele :: CompareTo (const RGPersistent* p) const {

	Allele* q = (Allele*) p;
	return mLink->CompareTo (q->mLink);
}


unsigned Allele :: HashNumber (unsigned long Base) const {

	return mLink->HashNumber (Base);
}



Boolean Allele :: IsEqualTo (const RGPersistent* p) const {

	Allele* q = (Allele*) p;
	return mLink->IsEqualTo (q->mLink);
}


void Allele :: Write (RGFile& textOutput, const RGString& indent) {

	textOutput << indent.GetData () << "Allele name = " << mLink->GetAlleleName ().GetData ();
	RGString temp;
	temp.Convert (mLink->GetCurveNumber (), 10);
	textOutput << ", Curve Number = " << temp.GetData ();
	temp.Convert (mLink->GetBioID (), 10);
	textOutput << ", BioId = " << temp.GetData () << "\n";
}



Locus :: Locus () : SmartMessagingObject (), mLink (NULL), Linked (FALSE), mSampleAnalysisMap (NULL), mSampleTimeFromBPMap (NULL), 
MaximumSampleTime (0.0), MinimumSampleTime (0.0), mMinTimeForSample (0.0), mMaxTimeForSample (0.0), mTimeForOneIDUnitLeft (0.0),
mTimeForOneIDUnitRight (0.0), mNumberOfCraters (0), mIsOK (false), mIsAMEL (false), mMinExtendedLocusTime (-1.0), mMaxExtendedLocusTime (-1.0),
mMaxPeak (0.0), mLargestPeak (NULL), mFirstTime (0.0), mLastTime (0.0) {

	mLink = new BaseLocus ();
	AlleleIterator = new RGDListIterator (AlleleList);
	InitializeSmartMessages ();
}


Locus :: Locus (BaseLocus* link, const RGString& xmlString) : SmartMessagingObject (), mLink (link), Linked (TRUE),
mSampleAnalysisMap (NULL), mSampleTimeFromBPMap (NULL), MaximumSampleTime (0.0), MinimumSampleTime (0.0), 
mMinTimeForSample (0.0), mMaxTimeForSample (0.0), mTimeForOneIDUnitLeft (0.0), mTimeForOneIDUnitRight (0.0), 
mNumberOfCraters (0), mIsOK (false), mIsAMEL (false), mMinExtendedLocusTime (-1.0), mMaxExtendedLocusTime (-1.0), mMaxPeak (0.0), mLargestPeak (NULL), 
mFirstTime (0.0), mLastTime (0.0) {

	AlleleIterator = new RGDListIterator (AlleleList);
	Valid = BuildAlleleLists (xmlString);

	if (link->GetLocusName () == "AMEL")
		mIsAMEL = true;

	InitializeSmartMessages ();
}


Locus :: Locus (const Locus& locus) : SmartMessagingObject ((SmartMessagingObject&)locus), mSampleAnalysisMap (NULL), mSampleTimeFromBPMap (NULL), 
MaximumSampleTime (locus.MaximumSampleTime), MinimumSampleTime (locus.MinimumSampleTime),
mMinTimeForSample (0.0), mMaxTimeForSample (0.0), mTimeForOneIDUnitLeft (0.0), mTimeForOneIDUnitRight (0.0), 
mNumberOfCraters (locus.mNumberOfCraters), mIsOK (locus.mIsOK), mIsAMEL (locus.mIsAMEL), mMinExtendedLocusTime (locus.mMinExtendedLocusTime), 
mMaxExtendedLocusTime (locus.mMaxExtendedLocusTime), mMaxPeak (locus.mMaxPeak), mLargestPeak (NULL), mFirstTime (0.0), mLastTime (0.0) {

	AlleleIterator = new RGDListIterator (AlleleList);

	if (locus.Linked) {

		mLink = locus.mLink;
		Linked = TRUE;
		Valid = locus.Valid;
		AlleleList = locus.AlleleList;
		AlleleIterator->Reset ();
		Allele* nextAllele;

		while (nextAllele = (Allele*)(*AlleleIterator)()) {

			BaseAllele::SetSearchByName ();
			AlleleListByName.Insert (nextAllele);
			BaseAllele::SetSearchByCurve ();
			AlleleListByCurve.Insert (nextAllele);
		}
	}

	else {

		mLink = new BaseLocus ();
		Linked = FALSE;
		Valid = TRUE;
	}

	InitializeSmartMessages (locus);
}


Locus :: Locus (const Locus& locus, CoordinateTransform* trans) : SmartMessagingObject ((SmartMessagingObject&)locus), mSampleAnalysisMap (NULL), mSampleTimeFromBPMap (NULL), 
MaximumSampleTime (locus.MaximumSampleTime), MinimumSampleTime (locus.MinimumSampleTime),
mMinTimeForSample (0.0), mMaxTimeForSample (0.0), mTimeForOneIDUnitLeft (0.0), mTimeForOneIDUnitRight (0.0), mNumberOfCraters (locus.mNumberOfCraters), mIsOK (locus.mIsOK), 
mIsAMEL (locus.mIsAMEL), mMaxPeak (locus.mMaxPeak), mLargestPeak (NULL), mFirstTime (0.0), mLastTime (0.0) {

	AlleleIterator = new RGDListIterator (AlleleList);

	if (locus.Linked) {

		mLink = locus.mLink;
		Linked = TRUE;
		Valid = locus.Valid;
		Allele* nextAllele;
		Allele* newAllele;
		RGDList tempAlleles = locus.AlleleList;

		while (nextAllele = (Allele*) tempAlleles.GetFirst ()) {

			newAllele = new Allele (*nextAllele, trans);
			AlleleList.Append (newAllele);
			delete nextAllele;
		}

		AlleleIterator->Reset ();

		while (nextAllele = (Allele*)(*AlleleIterator)()) {

			BaseAllele::SetSearchByName ();
			AlleleListByName.Insert (nextAllele);
			BaseAllele::SetSearchByCurve ();
			AlleleListByCurve.Insert (nextAllele);
			FinalSignalList.Append ((DataSignal*)nextAllele->GetCurvePointer ());
		}

		MaximumSampleTime = trans->EvaluateWithExtrapolation (locus.MaximumSampleTime);
		MinimumSampleTime = trans->EvaluateWithExtrapolation (locus.MinimumSampleTime);
		mMinExtendedLocusTime = trans->EvaluateWithExtrapolation (locus.mMinExtendedLocusTime);
		mMaxExtendedLocusTime = trans->EvaluateWithExtrapolation (locus.mMaxExtendedLocusTime);
	}

	else {

		mLink = new BaseLocus ();
		Linked = FALSE;
		Valid = TRUE;
	}

	InitializeSmartMessages (locus);
}


Locus :: ~Locus () {

	AlleleListByName.Clear ();
	AlleleListByCurve.Clear ();
	AlleleList.ClearAndDelete ();
	NonStandardAlleleList.ClearAndDelete ();
	delete AlleleIterator;

	if (!Linked)
		delete mLink;

	delete mSampleAnalysisMap;
	delete mSampleTimeFromBPMap;
	LocusSignalList.Clear ();
	FinalSignalList.Clear ();
	mSmartList.Clear ();
	NewNoticeList.ClearAndDelete ();

	list<CompoundSignalInfo*>::const_iterator c1Iterator;
	CompoundSignalInfo* nextLink;

	for (c1Iterator = mSignalLinkList.begin (); c1Iterator != mSignalLinkList.end (); c1Iterator++) {

		nextLink = *c1Iterator;
		delete nextLink;
	}

	mSignalLinkList.clear ();
	mGridArtifactList.Clear ();
	mExtendedLeft.Clear ();
	mExtendedRight.Clear ();
}


void Locus :: ClearAndDestroy () {

	RGDListIterator it (AlleleList);
	Allele* nextAllele;

	while (nextAllele = (Allele*) it ())
		nextAllele->ClearAndDestroy ();
	
	delete mLink;
	mLink = NULL;
	AlleleList.ClearAndDelete ();
	AlleleListByName.Clear ();
	AlleleListByCurve.Clear ();
}


void Locus :: SetTableLink (int linkNumber) {

	RGString temp;
	temp.Convert (linkNumber, 10);
	mTableLink = "&" + temp + "&";
}


RGString Locus :: GetLocusName () const {

	return mLink->GetLocusName ();
}


void Locus :: SetLocusName (const RGString& name) {

	mLink->SetLocusName (name);
}


int Locus :: GetLocusChannel () const {

	return mLink->GetChannel ();
}


void Locus :: SetLocusChannel (int channel) {

	mLink->SetChannel (channel);
}


int Locus :: NumberOfAmbiguousAlleles () {

	int n = 0;
	RGDListIterator it1 (mExtendedLeft);
	RGDListIterator it2 (mExtendedRight);
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) it1 ()) {

		if ((nextSignal->IsPossibleInterlocusAllele (-1)) && (nextSignal->IsPossibleInterlocusAllele (1)))
			n++;
	}

	while (nextSignal = (DataSignal*) it2 ()) {

		if ((nextSignal->IsPossibleInterlocusAllele (-1)) && (nextSignal->IsPossibleInterlocusAllele (1)))
			n++;
	}

	return n;
}


const int* Locus :: GetRelativeHeights () const {

	return mLink->GetRelativeHeights ();
}


void Locus :: AppendBaseLocusToList (RGDList& locusList) {

	if (mLink != NULL)
		locusList.Append (mLink);
}


void Locus :: ResetAlleles () {

	AlleleIterator->Reset ();
}


Allele* Locus :: GetNextAllele () {

	return (Allele*)(*AlleleIterator)();
}


Allele* Locus :: FindAllele (const RGString& name) {

	BaseAllele::SetSearchByName ();
	mTarget.SetAlleleName (name);
	return (Allele*)AlleleListByName.Find (&mTarget);
}


Allele* Locus :: FindAllele (int curve) {

	BaseAllele::SetSearchByCurve ();
	mTarget.SetCurve (curve);
	return (Allele*)AlleleListByCurve.Find (&mTarget);
}


Allele* Locus :: FindAlleleByID (int id) {

	RGDListIterator it (AlleleList);
	Allele* nextAllele;

	while (nextAllele = (Allele*) it()) {

		if (id == nextAllele->GetBioID ())
			return nextAllele;
	}

	return NULL;
}


Allele* Locus :: FindNearestAllele (int id) {

	RGDListIterator it (AlleleList);
	Allele* nextAllele;
	Allele* nearestAllele = (Allele*) it ();

	if (nearestAllele == NULL)
		return NULL;

	int nearestDistance = abs (id - nearestAllele->GetBioID ());
	int distance;

	while (nextAllele = (Allele*) it()) {

		distance = abs (id - nextAllele->GetBioID ());

		if (distance <= nearestDistance) {

			nearestDistance = distance;
			nearestAllele = nextAllele;
		}

		else
			return nearestAllele;

	}

	return nearestAllele;
}


bool Locus :: SignalIsIntegralMultipleOfRepeatAboveLadder (DataSignal* nextSignal) {

	// signal is above locus so locus is to left of signal
	
	int repeat = mLink->GetCoreNumber ();
	double id = nextSignal->GetBioID (-1);
	double gridId = mLink->GetMaxGridID ();
	int displacement = (int) floor (id - gridId + 0.5);

	if (displacement%repeat == 0)
		return true;

	return false;
}


bool Locus :: SignalIsIntegralMultipleOfRepeatBelowLadder (DataSignal* nextSignal) {

	// signal is below locus so locus is to right of signal
	
	int repeat = mLink->GetCoreNumber ();
	double id = nextSignal->GetBioID (1);
	double gridId = mLink->GetMinGridID ();
	int displacement = (int) floor (gridId -id + 0.5);

	if (displacement%repeat == 0)
		return true;

	return false;
}


RGString Locus :: ReconstructAlleleName (int id, Boolean& isOffGrid) {

	Allele* correspondingAllele = FindAlleleByID (id);

	if (correspondingAllele != NULL) {

		isOffGrid = FALSE;
		return correspondingAllele->GetAlleleName ();
	}

	isOffGrid = TRUE;

	if (mIsAMEL)
		return "";

	correspondingAllele = FindNearestAllele (id);
	return mLink->ReconstructAlleleName (id, correspondingAllele);
}


int Locus :: InsertAlleleSignals (RGDList& curveList) {

	RGDListIterator it (curveList);
	Allele* allele;
	DataSignal* signal;
	RGDListIterator alleleIt (AlleleList);
	RGString name;

	if (curveList.Entries () != AlleleList.Entries ()) {
		
		ErrorString << "Number of curves does not match number of alleles for locus named " << mLink->GetLocusName () << "\n";
		return -1;
	}

	while (signal = (DataSignal*)it()) {

		allele = (Allele*)alleleIt ();

		if (allele == NULL) {
			
			ErrorString << "Number of curves exceeds number of alleles for locus named " << mLink->GetLocusName () << "\n";
			return -1;
		}

		allele->SetCurvePointer (signal);
		name = allele->GetAlleleName ();
		signal->SetAlleleName (name);
	}

	if (!BuildMappings (curveList))
		return -1;

	return 0;
}


const DataSignal* Locus :: GetCurve (const RGString& alleleName) {

	mTarget.SetAlleleName (alleleName);
	BaseAllele::SetSearchByName ();
	Allele* allele = (Allele*)AlleleListByName.Find (&mTarget);

	if (allele == NULL)
		return NULL;

	return allele->GetCurvePointer ();
}


Boolean Locus :: IsTimeWithinLocusSample (double t) {

	if (mSampleAnalysisMap == NULL) {

		ErrorString << "Attempting to use Sample Analysis Map without initialization for locus named " << mLink->GetLocusName () << "\n";
		return FALSE;
	}

//	if ((t >= MinimumSampleTime) && (t <= MaximumSampleTime))
//		return TRUE;

	double id = floor (GetBPFromTimeForAnalysis (t) + 0.5);

	if (GridLocusContainsID (id))
		return TRUE;

	return FALSE;
}


Boolean Locus :: IsTimeWithinExtendedLocusSample (double t) {

	if (mSampleAnalysisMap == NULL) {

		ErrorString << "Attempting to use Sample Analysis Map without initialization for locus named " << mLink->GetLocusName () << "\n";
		return FALSE;
	}

	double id = floor (GetBPFromTimeForAnalysis (t) + 0.5);

	if (ExtendedLocusContainsID (id))
		return TRUE;

	return FALSE;
}


Boolean Locus :: IsTimeWithinExtendedLocusSample (double t, int& location) {

	if (mSampleAnalysisMap == NULL) {

		cout << "Attempting to use Sample Analysis Map without initialization for locus named " << mLink->GetLocusName () << "\n";
		return FALSE;
	}

	double id = floor (GetBPFromTimeForAnalysis (t) + 0.5);

	if (ExtendedLocusContainsID (id, location))
		return TRUE;

	return FALSE;
}


DataSignal* Locus :: RemoveSignalFromLocusList (DataSignal* target) {

	mExtendedLeft.RemoveReference (target);
	mExtendedRight.RemoveReference (target);

	return (DataSignal*) LocusSignalList.RemoveReference (target);
}


int Locus :: PromoteSignalToAllele (DataSignal* target) {

//	unsigned signalId = target->GetSignalID ();
	Locus* locus = (Locus*) target->GetLocus (0);

	if (locus != NULL)
		return 0;
	
	if (mExtendedLeft.RemoveReference (target) != NULL) {

		target->SetAlleleInformation (1);
		return 0;
	}

	else if (mExtendedRight.RemoveReference (target) != NULL) {

		target->SetAlleleInformation (-1);
		return 0;
	}

	return -1;
}


int Locus :: PromoteNonAmbiguousSignalsToAlleles (RGDList& signalList) {

	//
	//  This is sample stage 3
	//

	RGDListIterator itLeft (mExtendedLeft);
	RGDListIterator itRight (mExtendedRight);
	DataSignal* nextSignal;
//	unsigned signalId;
	bool isNotCore;

	while (nextSignal = (DataSignal*) itLeft ()) {

//		signalId = nextSignal->GetSignalID ();
		isNotCore = (nextSignal->GetLocus (0) == NULL);

		if ((!nextSignal->IsPossibleInterlocusAllele (-1)) && isNotCore) {

			itLeft.RemoveCurrentItem ();
			signalList.RemoveReference (nextSignal);  // this signal is no longer shared
			nextSignal->SetAlleleInformation (1);  // promote allele info to primary position
		}
	}

	while (nextSignal = (DataSignal*) itRight ()) {

//		signalId = nextSignal->GetSignalID ();
		isNotCore = (nextSignal->GetLocus (0) == NULL);

		if ((!nextSignal->IsPossibleInterlocusAllele (1)) && isNotCore) {

			itRight.RemoveCurrentItem ();
			signalList.RemoveReference (nextSignal);  // this signal is no longer shared
			nextSignal->SetAlleleInformation (-1);  // promote allele info to primary position
		}
	}

	return 0;
}


int Locus :: PromoteOwnCoreSignalsToAllelesAndRemoveOthers (RGDList& signalList) {

	DataSignal* nextSignal;
	RGDListIterator it (mSmartList);
	Locus* parentLocus;

	while (nextSignal = (DataSignal*) it ()) {

		parentLocus = (Locus*) nextSignal->GetLocus (0);

		if (parentLocus == NULL)
			continue;

		if (parentLocus != this) {

			mExtendedLeft.RemoveReference (nextSignal);
			mExtendedRight.RemoveReference (nextSignal);
			LocusSignalList.RemoveReference (nextSignal);
//			nextSignal->SetPossibleInterlocusAllele (1, false);
//			nextSignal->SetPossibleInterlocusAllele (-1, false);
		}
	}

	return 0;
}


int Locus :: PromoteSignalToLeftExtendedAllele (DataSignal* target) {

	if (mExtendedLeft.RemoveReference (target) != NULL) {

		target->SetAlleleInformation (1);
		return 0;
	}

	return -1;
}


int Locus :: PromoteSignalToRightExtendedAllele (DataSignal* target) {

	if (mExtendedRight.RemoveReference (target) != NULL) {

		target->SetAlleleInformation (-1);
		return 0;
	}

	return -1;
}


int Locus :: InsertSignalExtendedLeft (DataSignal* newSignal) {

	// We have to make sure that newSignal has been "called" appropriately
	
	mExtendedLeft.InsertWithNoReferenceDuplication (newSignal);
	LocusSignalList.InsertWithNoReferenceDuplication (newSignal);
	mSmartList.InsertWithNoReferenceDuplication (newSignal);
	return 0;
}


int Locus :: InsertSignalExtendedRight (DataSignal* newSignal) {

	// We have to make sure that newSignal has been "called" appropriately

	mExtendedRight.InsertWithNoReferenceDuplication (newSignal);
	LocusSignalList.InsertWithNoReferenceDuplication (newSignal);
	mSmartList.InsertWithNoReferenceDuplication (newSignal);
	return 0;
}


double Locus :: HasHeightRatioWithExclusiveMaxPeak (DataSignal* target, bool& causesHeightImbalance) {

	if (LocusSignalList.Entries () <= 1) {

		causesHeightImbalance = false;
		return 1.0;
	}
	
	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	double maxPeakExclusive = 0.0;
//	double heteroLimit = Locus::GetHeterozygousImbalanceLimit ();
	double heteroLimit = GetLocusSpecificSampleHeterozygousImbalanceThreshold ();
	double targetPeak = target->Peak ();
	double peak;
	int n = 0;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal == target)
			continue;

		if ((nextSignal->IsPossibleInterlocusAllele (-1)) && (nextSignal->IsPossibleInterlocusAllele (1)))
			continue;
		
		peak = nextSignal->Peak ();
		n++;

		if (peak > maxPeakExclusive)
			maxPeakExclusive = peak;
	}

	if (n == 0)
		causesHeightImbalance = false;

	else if ((targetPeak < heteroLimit * maxPeakExclusive) || (maxPeakExclusive < heteroLimit * targetPeak))
		causesHeightImbalance = true;

	else
		causesHeightImbalance = false;

	return maxPeakExclusive / targetPeak;
}


int Locus :: TestForAcceptedTriAllele (Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes) {

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	RGString alleleName;
	IndividualAllele* newAllele;
	RGDList alleleList;
	bool testForAcceptedTriallele;
	RGString lName = GetLocusName ();
	int location;
//	unsigned signalId;

	if (NumberOfAmbiguousAlleles () == 0)
		return 0;
	
	if (LocusSignalList.Entries () == 3) {

		while (nextSignal = (DataSignal*) it ()) {

			location = - TestSignalPositionRelativeToLocus (nextSignal);  // return value is signal position relative to locus...reverse it to get locus position relative to signal
			alleleName = nextSignal->GetAlleleName (location);
			newAllele = new IndividualAllele ();
			newAllele->SetName (alleleName);
			alleleList.Append (newAllele);
		}

		if (isPosCntl)
			testForAcceptedTriallele = pGenotypes->ContainsControlTriallele (GetLocusName (), alleleList);

		else
			testForAcceptedTriallele = pGenotypes->ContainsSampleTriallele (GetLocusName (), alleleList);
		
		alleleList.ClearAndDelete ();

		if (testForAcceptedTriallele) {

			it.Reset ();

			while (nextSignal = (DataSignal*) it ()) {

//				signalId = nextSignal->GetSignalID ();
				location = - TestSignalPositionRelativeToLocus (nextSignal);  // return value is signal position relative to locus...reverse it to get locus position relative to signal
				nextSignal->SetAcceptedTriAllele (location, true);
			}
		}
	}

	return 0;
}


int Locus :: AnalyzeGridLocus (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	PrecomputeAverages (LocusSignalList);

	RGDListIterator CurveIterator (LocusSignalList);
	FinalSignalList.Clear ();
	DataSignal* FollowingSignal;
	DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
	int TestResult;
	int Size = mLink->GetLocusVectorSize ();
	msg.WriteEmptyLine ();
	msg.WriteLocusName (mLink->GetLocusName (), Size);
	RGString eString;
	int status = 0;
	Notice* newNotice;
	Notice* foundNotice;
	RGDList overFlow;
	RGDListIterator overflowIterator (overFlow);
	RGString info;
	int recursiveStatus;

	RGDList toTheLeftList;
	RGDList toTheRightList;
	RGDList tempSignalList;
	RGDListIterator tempIterator (tempSignalList);

	double MaxPeak = 0.0;
	double peak;
	CurveIterator.Reset ();

	bool isAMEL = mIsAMEL;

	/*if (mLink->GetLocusName () == "AMEL")
		isAMEL = true;*/

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		peak = nextSignal->Peak ();

		if (peak > MaxPeak)
			MaxPeak = peak;
	}

	double fractionalThreshold = 0.0;
	double fractionalFilter = Locus::GetGridFractionalFilter ();
	
	if (fractionalFilter > 0.0) {
	
		fractionalThreshold = fractionalFilter * MaxPeak;
	}

/*	while (nextSignal = (DataSignal*) LocusSignalList.GetFirst ()) {

		if (nextSignal->Peak () <= 0.35 * MaxPeak)
			artifactList.InsertWithNoReferenceDuplication (nextSignal);

		else {

			LocusSignalList.Prepend (nextSignal);
			break;
		}
	}

	while (nextSignal = (DataSignal*) LocusSignalList.GetLast ()) {

		if (nextSignal->Peak () <= 0.40 * MaxPeak)
			artifactList.InsertWithNoReferenceDuplication (nextSignal);

		else {

			LocusSignalList.Append (nextSignal);
			break;
		}
	}
*/
	CurveIterator.Reset ();

	while (nextSignal = (DataSignal*)LocusSignalList.GetFirst ()) {

		// Test for out of bounds on various quantities...
		FollowingSignal = (DataSignal*)LocusSignalList.First ();

		if ((fractionalFilter > 0.0) && (nextSignal->Peak () <= fractionalThreshold)) {

//			supplementalList.InsertWithNoReferenceDuplication (nextSignal);
			artifactList.InsertWithNoReferenceDuplication (nextSignal);
			newNotice = new SignalPeakBelowFractionalFilterHeight;
//			nextSignal->ClearNoticeObjects ();
			nextSignal->AddNoticeToList (newNotice);
		}

		else {

			TestResult = TestNeighbors (PreviousSignal, nextSignal, FollowingSignal);

			if (TestResult <= 0)
				artifactList.InsertWithNoReferenceDuplication (nextSignal);

			else
				FinalSignalList.Append (nextSignal);
		}

		PreviousSignal = (DataSignal*) FinalSignalList.Last ();
	}

	RGDListIterator FinalIterator (FinalSignalList);

	// Removing "Flanking peak" test from grid loci.  The test is, at best, unnecessary and, at
	// worst, risky.  STR grids, or ladders, often have acceptable peaks that come close to failing
	// this test and, in practice, it has never been needed to analyze a ladder locus successfully.
	// Now, internal lane standards are another story, because their peaks should not exhibit so much
	// local variation.

/*	if ((Size > 0) && (FinalSignalList.Entries () > Size)) {

		while (nextSignal = (DataSignal*)FinalIterator ()) {

			if (!nextSignal->FlankingPeakProportionExceeds (0.4)) {

				FinalIterator.RemoveCurrentItem ();
				artifactList.InsertWithNoReferenceDuplication (nextSignal);
				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", "Peak out of place in final locus list:  uncategorized artifact");
			}
		}
	}
*/
	// Adding a "Flanking sum peak" test for grid loci.  This tests, more or less, if the to flanking peaks can have contributed either stutter or
	// adenylation to the current peak, at least if there are too many peaks...otherwise, skip it.

/*	if ((Size > 0) && (FinalSignalList.Entries () > Size)) {

		while (nextSignal = (DataSignal*)FinalIterator ()) {

			if (!nextSignal->FlankingPeakSumProportionExceeds (0.2)) {

				FinalIterator.RemoveCurrentItem ();
				artifactList.InsertWithNoReferenceDuplication (nextSignal);
				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", "Peak out of place in final locus list:  uncategorized artifact");
			}
		}
	}

	if ((Size > 0) && (FinalSignalList.Entries () > Size)) {

		FinalIterator.Reset ();
		RGDListIterator FinalIterator2 (FinalSignalList);
		nextSignal = (DataSignal*) FinalIterator ();
		FinalIterator2 ();

		while (FollowingSignal = (DataSignal*)FinalIterator2 ()) {

			if (nextSignal->Peak () <= Locus::GridStutterThreshold * FollowingSignal->Peak ()) {

				FinalIterator.RemoveCurrentItem ();
				newNotice = new StutterFound;
				nextSignal->AddNoticeToList (newNotice);
				nextSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", "Peak is stutter for following characteristic");
				artifactList.InsertWithNoReferenceDuplication (nextSignal);
				nextSignal = (DataSignal*) FinalIterator ();
			}

			else
				break;
		}
	}
*/

	double correlation = 0.0;
	int Displacement = 0;  // = mLink->SelectBestSubsetOfCharacteristics (FinalSignalList, correlation);
	FinalIterator.Reset ();
	DataSignal* prevSignal;

	if ((Size > 0) && (FinalSignalList.Entries () < Size)) {

		int nCurves = FinalSignalList.Entries ();
		eString << nCurves << " available out of " << Size << " required for Locus..." << "\n";
		eString << "LOCUS NAMED " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS\n";
		ExcelText.Write (OutputLevelManager::LocusGridQuality, eString);

		newNotice = new LocusHasTooFewPeaks;
		foundNotice = RemoveNotice (newNotice);
		delete foundNotice;
		AddNoticeToList (newNotice);
		
//		if (print)
//			msg.WriteInsufficientPeaksForControlSet (FinalSignalList.Entries ());  // !!!!!

		status = -1;
	}

	else if ((Size > 0) && (FinalSignalList.Entries () == Size)) {

		// Just compute the correlation and call it a day!

		double* signalArray = new double [Size];
		int iii = 0;
		prevSignal = (DataSignal*) FinalIterator ();

		while (nextSignal = (DataSignal*) FinalIterator ()) {

			signalArray [iii] = nextSignal->GetMean () - prevSignal->GetMean ();
			prevSignal = nextSignal;
			iii++;
		}

		const double* idealNormalizedDifferences = mLink->GetDifferenceVector ();
		correlation = PopulationMarkerSet::VectorDotProductWithDifferences (signalArray, idealNormalizedDifferences, Size-1);
		delete[] signalArray;
	}

	else {

		if (FinalSignalList.Entries () >= Size) {

			overFlow.Clear ();
			const double* differenceArray = mLink->GetUnnormalizedDifferenceVector ();
			double extremePoints [3];
			const double* actualArray = mLink->GetLocusVector ();
			extremePoints [0] = actualArray [0];
			extremePoints [2] = actualArray [Size - 1];
			extremePoints [1] = actualArray [Size - 2];
			const double* leftNorm2s = mLink->GetNormsLeft ();
			const double* rightNorm2s = mLink->GetNormsRight ();
			const int* hts = mLink->GetRelativeHeights ();
			IdealControlSetInfo ctlInfo (actualArray, differenceArray, leftNorm2s, rightNorm2s, hts, Size, true);
			int startLimit = (Size / 2) - 1;
			int startPts;

			startPts = Size;
			int maxDiscrepancy;
			int disc;

			if (hts != NULL)
				maxDiscrepancy = 4;

			else
				maxDiscrepancy = 2;

			for (disc=2; disc<=maxDiscrepancy; disc++) {

				FinalIterator.Reset ();
				tempSignalList.Clear ();
				toTheLeftList.Clear ();
				toTheRightList.Clear ();
				overFlow.Clear ();
				ctlInfo.mDiscrepancy = disc;

				while (nextSignal = (DataSignal*) FinalIterator ())
					tempSignalList.Append (nextSignal);

				if (hts != NULL) {

					tempIterator.Reset ();
					double testPeak;

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempIterator ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, 0, testPeak, MaxPeak, disc)) {

							tempIterator.RemoveCurrentItem ();

							/*nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
								"Peak out of place in final list:  interlocus peak");
							newNotice = new InterLocusLadderPeak;
							nextSignal->AddNoticeToList (newNotice);*/

							toTheLeftList.Append (nextSignal);
						}

						else
							break;
					}

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempSignalList.GetLast ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, Size - 1, testPeak, MaxPeak, disc)) {

							/*nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
								"Peak out of place in final list:  interlocus peak");
							newNotice = new InterLocusLadderPeak;
							nextSignal->AddNoticeToList (newNotice);*/

							toTheRightList.Prepend (nextSignal);
						}

						else {

							tempSignalList.Append (nextSignal);
							break;
						}
					}
				}

				recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (tempSignalList, overFlow, ctlInfo, correlation, 0.99, startPts, MaxPeak);

				if ((recursiveStatus < 0) || (correlation < 0.995)) {

					if (disc == maxDiscrepancy) {

						if (print)
							msg.WriteInsufficientPeaksForControlSet (FinalSignalList.Entries ());

						if (status == 0)
							ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";

						newNotice = new LocusHasTooFewPeaks;
						info = " with ";
						info << FinalSignalList.Entries () << " peaks out of " << Size;
						newNotice->AddDataItem (info);
						foundNotice = RemoveNotice (newNotice);
						delete foundNotice;
						AddNoticeToList (newNotice);
						overFlow.Clear ();
						cout << ErrorString << endl;

						//
						// Add msg if hts != NULL about relative sizes*************************
						//

						if (hts != NULL) {

							newNotice = new LadderRelativeHeightsInconsistent;
							AddNoticeToList (newNotice);
						}

						status = -1;
					}
				}

				else {

					overflowIterator.Reset ();
					double firstTime;
					double lastTime;
					double nextMean;
					tempIterator.Reset ();
					FinalSignalList.Clear ();

					while (nextSignal = (DataSignal*) tempSignalList.GetFirst ())
						FinalSignalList.Append (nextSignal);

					nextSignal = (DataSignal*) FinalSignalList.First ();
					firstTime = nextSignal->GetMean ();
					nextSignal = (DataSignal*) FinalSignalList.Last ();
					lastTime = nextSignal->GetMean ();

					//
					// if hts != NULL and disc > 2, add message about failure of relative height criteria ********************
					//

					if ((hts != NULL) && (disc > 2)) {

						newNotice = new LadderRelativeHeightsInconsistent;
						AddNoticeToList (newNotice);
					}
					
					while (nextSignal = (DataSignal*) overFlow.GetFirst ()) {

						nextMean = nextSignal->GetMean ();

						if (nextMean < firstTime) {

	//						nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
	//							"Peak out of place in final list:  interlocus peak");
							toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
	//						newNotice = new InterLocusLadderPeak;
	//						nextSignal->AddNoticeToList (newNotice);
						}

						else if (nextMean > lastTime) {

	//						nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
	//							"Peak out of place in final list:  interlocus peak");
							toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
						}

						else {

							nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
								"Peak out of place in final list:  uncategorized artifact");
							newNotice = new ExtraneousPeakInLadderLocus;
							nextSignal->AddNoticeToList (newNotice);
							mGridArtifactList.InsertWithNoReferenceDuplication (nextSignal);
						}
					}

	//				MergeListAIntoListB (overFlow, supplementalList);

					break;
				}

			}

//			if (hts != NULL) {
//
//				FinalIterator.Reset ();
//				double testPeak;
//
//				while (FinalSignalList.Entries () > Size) {
//
//					nextSignal = (DataSignal*) FinalIterator ();
//					testPeak = nextSignal->Peak ();
//
//					if (RecursiveInnerProduct::HeightOutsideLimit (hts, 0, testPeak, MaxPeak)) {
//
//						FinalIterator.RemoveCurrentItem ();
//						nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
//							"Peak out of place in final list:  interlocus peak");
//						newNotice = new InterLocusLadderPeak;
//						nextSignal->AddNoticeToList (newNotice);
//						toTheLeftList.Append (nextSignal);
//					}
//
//					else
//						break;
//				}
//
//				while (FinalSignalList.Entries () > Size) {
//
//					nextSignal = (DataSignal*) FinalSignalList.GetLast ();
//					testPeak = nextSignal->Peak ();
//
//					if (RecursiveInnerProduct::HeightOutsideLimit (hts, Size - 1, testPeak, MaxPeak)) {
//
//						nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
//							"Peak out of place in final list:  interlocus peak");
//						newNotice = new InterLocusLadderPeak;
//						nextSignal->AddNoticeToList (newNotice);
//						toTheRightList.Prepend (nextSignal);
//					}
//
//					else {
//
//						FinalSignalList.Append (nextSignal);
//						break;
//					}
//				}
//			}
//
//			if (PopulationMarkerSet::SearchRecursivelyForBestSubset (FinalSignalList, overFlow, ctlInfo, correlation, 0.99, startPts, MaxPeak) < 0) {
//
//
//				if (print)
//					msg.WriteInsufficientPeaksForControlSet (FinalSignalList.Entries ());
//
//				if (status == 0)
//					ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";
//
//				newNotice = new LocusHasTooFewPeaks;
//				info = " with ";
//				info << FinalSignalList.Entries () << " peaks out of " << Size;
//				newNotice->AddDataItem (info);
//				foundNotice = RemoveNotice (newNotice);
//				delete foundNotice;
//				AddNoticeToList (newNotice);
//				overFlow.Clear ();
//				cout << ErrorString << endl;
//
//				status = -1;
//			}
//
//			else {
//
//				overflowIterator.Reset ();
//				double firstTime;
//				double lastTime;
//				double nextMean;
//				nextSignal = (DataSignal*) FinalSignalList.First ();
//				firstTime = nextSignal->GetMean ();
//				nextSignal = (DataSignal*) FinalSignalList.Last ();
//				lastTime = nextSignal->GetMean ();
//				
//				while (nextSignal = (DataSignal*) overFlow.GetFirst ()) {
//
//					nextMean = nextSignal->GetMean ();
//
//					if (nextMean < firstTime) {
//
////						nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
////							"Peak out of place in final list:  interlocus peak");
//						toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
////						newNotice = new InterLocusLadderPeak;
////						nextSignal->AddNoticeToList (newNotice);
//					}
//
//					else if (nextMean > lastTime) {
//
////						nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
////							"Peak out of place in final list:  interlocus peak");
//						toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
//					}
//
//					else {
//
//						nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
//							"Peak out of place in final list:  uncategorized artifact");
//						newNotice = new PeakOutOfPlaceInLocus;
//						nextSignal->AddNoticeToList (newNotice);
//						mGridArtifactList.InsertWithNoReferenceDuplication (nextSignal);
//					}
//				}
//
////				MergeListAIntoListB (overFlow, supplementalList);
//			}
		}

		Displacement = 0;
	}

	if (correlation < 0.995) {

//		if (print)
//			msg.;

		if (status == 0)
			ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...Correlation, " << correlation << ", is too low.\n";

		newNotice = new PoorLocusMorphology;
		info = " with correlation = ";
		info << correlation << " (0.995 - 1.0)";
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);
		overFlow.Clear ();
		cout << ErrorString << endl;

		status = -1;
	}

	int CurrentIndex = 0;
	int TotalCandidates = FinalSignalList.Entries ();

	while (CurrentIndex < Displacement) {

		nextSignal = (DataSignal*)FinalSignalList.GetFirst ();
		nextSignal->AddNoticeToList (OutputLevelManager::PeakBelowLocus, "", "Peak represents measurement at base pair level below locus");
		toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
		CurrentIndex++;
	}

	while (FinalSignalList.Entries () > Size) {

		nextSignal = (DataSignal*)FinalSignalList.GetLast ();
		nextSignal->AddNoticeToList (OutputLevelManager::PeakAboveLocus, "", "Peak represents measurement at base pair level above locus");
		toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
	}

	if (toTheLeftList.Entries () > 0) {

//		newNotice = new InterlocusPeakToLeftOfLocus;
//		AddNoticeToList (newNotice);
		MergeListAIntoListB (toTheLeftList, originalList);
	}

	if (toTheRightList.Entries () > 0) {

//		newNotice = new InterlocusPeakToRightOfLocus;
//		AddNoticeToList (newNotice);
		MergeListAIntoListB (toTheRightList, originalList);
	}

	CurveIterator.Reset ();
//	ArtifactIterator.Reset ();
	FinalIterator.Reset ();
	int i = 0;
	int NumberOfAcceptedCurves = FinalSignalList.Entries ();

	if (NumberOfAcceptedCurves == 0) {

		ErrorString << "LOCUS " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS...There are no peaks within expected parameters.\n";
		newNotice = new LocusHasTooFewPeaks;
		info = " with ";
		info << FinalSignalList.Entries () << " peaks out of " << Size;
		newNotice->AddDataItem (info);
		foundNotice = RemoveNotice (newNotice);
		delete newNotice;
		AddNoticeToList (foundNotice);

		while (nextSignal = (DataSignal*) CurveIterator ())
			nextSignal->ReportNotices (ExcelText, "\t\t", "\t");

		return -1;
	}

	double* CurveNumbers = new double [NumberOfAcceptedCurves];
	double* Fits = new double [NumberOfAcceptedCurves];
	double* Peaks = new double [NumberOfAcceptedCurves];
	double* Sigmas = new double [NumberOfAcceptedCurves];
	double* SecondaryContent = new double [NumberOfAcceptedCurves];
	double* Means = new double [NumberOfAcceptedCurves];
	double TwoMass;
	double OneMass;
	CompoundSignalInfo* csi;
	CraterInducedByPullUp pullupNotice;
	PullUpPrimaryChannel primaryPullupNotice;
	PullUpFound pullup;

	while (nextSignal = (DataSignal*)FinalIterator ()) {

		CurveNumbers [i] = i + 1;
		Fits [i] = nextSignal->GetCurveFit ();
		Peaks [i] = nextSignal->Peak ();

		Sigmas [i] = nextSignal->GetStandardDeviation ();
		Means [i] = nextSignal->GetMean ();
		TwoMass = nextSignal->GetScale (2);
		OneMass = nextSignal->GetScale (1);
		SecondaryContent [i] = TwoMass / (TwoMass + OneMass);

		if (print)
			nextSignal->Report (text, "    ");

		i++;

		if (status == 0) {

			csi = nextSignal->RemoveSignalLink ();

			if (csi != NULL) {

				foundNotice = nextSignal->RemoveNotice (&pullupNotice);
				delete foundNotice;
				nextSignal->RemoveCrossChannelSignalLink (&primaryPullupNotice, &pullupNotice, NULL, NULL);
			}

			nextSignal->RemoveCrossChannelSignalLink (&primaryPullupNotice, &pullup, NULL, NULL);
		}
	}

	if (print) {

		msg.WriteEmptyLine ();
		msg.WriteMaximumCorrelationWithControlSet (0, correlation, Size, TotalCandidates);  // !!!!!
		msg.StartLine (0, "Curve Nos.", TRUE);
		msg.StartLine (1, "Fits", TRUE);
		msg.StartLine (2, "2AryContent", TRUE);
		msg.StartLine (3, "Means", TRUE);
		msg.StartLine (4, "Sigmas", TRUE);
		msg.StartLine (5, "Peaks", TRUE);

		for (int j=0; j<NumberOfAcceptedCurves; j++) {

			msg.AddToLine (0, CurveNumbers [j]);
			msg.AddToLine (1, Fits [j]);
			msg.AddToLine (2, SecondaryContent [j]);
			msg.AddToLine (3, Means [j]);
			msg.AddToLine (4, Sigmas [j]);
			msg.AddToLine (5, Peaks [j]);
		}

		msg.FlushLines (6);
	}

	delete[] CurveNumbers;
	delete[] Fits;
	delete[] SecondaryContent;
	delete[] Sigmas;
	delete[] Peaks;
	delete[] Means;

	if (InsertAlleleSignals (FinalSignalList) < 0) {

		ExcelText.Write (OutputLevelManager::LocusGridQuality, ErrorString);
		return -1;
	}

	if (status == 0)
		mIsOK = true;

	return status;
}


int Locus :: AnalyzeSampleLocus (ChannelData* lsData, RGDList& artifactList, RGDList& signalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	RGDListIterator CurveIterator (LocusSignalList);
	FinalSignalList.Clear ();
	DataSignal* nextSignal;
	int Size = mLink->GetLocusVectorSize ();
	msg.WriteEmptyLine ();
	msg.WriteLocusName (mLink->GetLocusName (), Size);

//	TestFractionalFilter (artifactList, signalList, supplementalList);
	PrecomputeAverages (LocusSignalList);
	RGDListIterator it (LocusSignalList);

	while (nextSignal = (DataSignal*) it())
		TestSampleAverages (lsData, nextSignal, Locus::TestRatio);
	
	return 0;
}


int Locus :: TestFractionalFilters (RGDList& artifactList, RGDList& supplementalList) {

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;

	Notice* newNotice;
	int location;
	PullUpFound pullupNotice;

	double maxPeak = 0.0;
	double peak;
	double fractionalThreshold = 0.0;
	double pullupFractionalThreshold = 0.0;
	double fractionalFilter = Locus::GetSampleFractionalFilter ();
	double pullupFractionalFilter = Locus::GetSamplePullupFractionalFilter ();
	bool peakIsLessThanFractionalThreshold;
	bool peakIsLessThanPullupFractionalThreshold;
//	unsigned signalId;
	
	if ((fractionalFilter > 0.0) || (pullupFractionalFilter > 0.0)) {
	
		while (nextSignal = (DataSignal*) it ()) {

			if (nextSignal->IsNoticeInList (&pullupNotice))
				continue;

			location = TestSignalPositionRelativeToLocus (nextSignal);

			if (location != 0)
				continue;

			peak = nextSignal->Peak ();

			if (peak > maxPeak)
				maxPeak = peak;
		}

		if (maxPeak <= 0.0)
			return 0;

		fractionalThreshold = fractionalFilter * maxPeak;
		pullupFractionalThreshold = pullupFractionalFilter * maxPeak;
		it.Reset ();

		while (nextSignal = (DataSignal*) it ()) {

			peak = nextSignal->Peak ();
			peakIsLessThanFractionalThreshold = (peak <= fractionalThreshold);
			peakIsLessThanPullupFractionalThreshold = (nextSignal->IsNoticeInList (&pullupNotice)) && (peak <= pullupFractionalThreshold);

			if (peakIsLessThanFractionalThreshold || peakIsLessThanPullupFractionalThreshold) {

//				signalId = nextSignal->GetSignalID ();
				location = TestSignalPositionRelativeToLocus (nextSignal);
				nextSignal->SetPossibleInterlocusAllele (-location, false); // if location is 1, this locus is to LEFT of signal, ...
				it.RemoveCurrentItem ();

				if (location == 0) {

					artifactList.InsertWithNoReferenceDuplication (nextSignal);
//					nextSignal->ClearNoticeObjects ();
					nextSignal->SetAlleleName ("");

					if (peakIsLessThanFractionalThreshold) {

						newNotice = new SignalPeakBelowFractionalFilterHeight;
						nextSignal->AddNoticeToList (newNotice);
					}

					if (peakIsLessThanPullupFractionalThreshold) {

						newNotice = new PeakBelowPullupFractionalFilter;
						nextSignal->AddNoticeToList (newNotice);
					}					
				}
				
				else {

					supplementalList.InsertWithNoReferenceDuplication (nextSignal);

					if (location < 0) {

						// Peak is to the left of locus so locus is to the RIGHT of peak!

						nextSignal->SetAlleleName ("", 1);
						mExtendedLeft.RemoveReference (nextSignal);

						if (peakIsLessThanFractionalThreshold) {

							newNotice = new PeakBelowFractionalFilterRight;
							nextSignal->AddNoticeToList (newNotice);
						}

						if (peakIsLessThanPullupFractionalThreshold) {

							newNotice = new PeakBelowPullupFractionalFilterRight;
							nextSignal->AddNoticeToList (newNotice);
						}			
					}

					else if (location > 0) {

						// Peak is to the right of locus so locus is to the LEFT of peak!

						nextSignal->SetAlleleName ("", -1);
						mExtendedRight.RemoveReference (nextSignal);

						if (peakIsLessThanFractionalThreshold) {

							newNotice = new PeakBelowFractionalFilterLeft;
							nextSignal->AddNoticeToList (newNotice);
						}

						if (peakIsLessThanPullupFractionalThreshold) {

							newNotice = new PeakBelowPullupFractionalFilterLeft;
							nextSignal->AddNoticeToList (newNotice);
						}		
					}

				}
			}
		}
	}

	return 0;
}


Boolean Locus :: ExtractGridSignals (RGDList& channelSignalList, const LaneStandard* ls, ChannelData* lsData) {

	Boolean status = mLink->ExtractGridSignals (channelSignalList, ls, LocusSignalList, lsData, (Locus*) this);

	if (!status)
		ErrorString += mLink->GetError ();

	return status;
}


Boolean Locus :: ExtractSampleSignals (RGDList& channelSignalList, Locus* gridLocus, CoordinateTransform* timeMap) {

	double mean;
	double gridTime;
	DataSignal* nextSignal;
	RGDListIterator it (channelSignalList);
	LocusSignalList.Clear ();
	FinalSignalList.Clear ();
	mSmartList.Clear ();
	bool haveFoundSignals = false;

	while (nextSignal = (DataSignal*) it()) {

		mean = nextSignal->GetMean ();
		gridTime = timeMap->EvaluateWithExtrapolation (mean);

		if (gridLocus->IsTimeWithinLocusSample (gridTime)) {

			LocusSignalList.Append (nextSignal);
			mSmartList.Append (nextSignal);
			nextSignal->SetBioID (gridLocus->GetBPFromTimeForAnalysis (gridTime));
			nextSignal->SetLocus ((Locus*)this, 0);

			it.RemoveCurrentItem ();
			haveFoundSignals = true;
		}

		else if (haveFoundSignals)
			break;
	}

	return TRUE;
}


Boolean Locus :: ExtractExtendedSampleSignals (RGDList& channelSignalList, Locus* gridLocus, CoordinateTransform* timeMap) {

	double mean;
	double gridTime;
	DataSignal* nextSignal;
	RGDListIterator it (channelSignalList);
	FinalSignalList.Clear ();
	double time;
	int location;
	bool haveFoundSignals = false;

	while (nextSignal = (DataSignal*) it()) {

		mean = nextSignal->GetMean ();
		gridTime = timeMap->EvaluateWithExtrapolation (mean);

		if (gridLocus->IsTimeWithinExtendedLocusSample (gridTime, location)) {

			LocusSignalList.InsertWithNoReferenceDuplication (nextSignal);
			mSmartList.InsertWithNoReferenceDuplication (nextSignal);
			nextSignal->SetBioID (gridLocus->GetBPFromTimeForAnalysis (gridTime), -location);
			nextSignal->SetLocus ((Locus*)this, -location);
			haveFoundSignals = true;

			if (location == 0)
				it.RemoveCurrentItem ();

			else if (location < 0) {

				mExtendedLeft.Append (nextSignal);
				nextSignal->SetPossibleInterlocusAllele (1, true);
			}

			else {

				mExtendedRight.Append (nextSignal);
				nextSignal->SetPossibleInterlocusAllele (-1, true);
			}
		}

		else if (haveFoundSignals)
			break;
	}

	nextSignal = (DataSignal*) LocusSignalList.First ();

	if (nextSignal != NULL) {

		location = - TestSignalPositionRelativeToLocus (nextSignal);
		mMinTimeForSample = nextSignal->GetMean ();
		time = gridLocus->GetTimeFromBP (nextSignal->GetBioID (location) - 1.0);
		mTimeForOneIDUnitLeft = mMinTimeForSample - time;

		nextSignal = (DataSignal*) LocusSignalList.Last ();
		location = - TestSignalPositionRelativeToLocus (nextSignal);
		mMaxTimeForSample = nextSignal->GetMean ();
		time = gridLocus->GetTimeFromBP (nextSignal->GetBioID (location) + 1.0);
		mTimeForOneIDUnitRight = time - mMaxTimeForSample;
	}

	return TRUE;
}

/*
Boolean Locus :: ExtractExtendedLadderSampleSignals (RGDList& channelSignalList, Locus* gridLocus, CoordinateTransform* timeMap) {

	double mean;
	double gridTime;
	DataSignal* nextSignal;
	RGDListIterator it (channelSignalList);

	while (nextSignal = (DataSignal*) it()) {

		mean = nextSignal->GetMean ();
		gridTime = timeMap->EvaluateWithExtrapolation (mean);

		if (gridLocus->IsTimeWithinExtendedLocusSample (gridTime)) {

			LocusSignalList.InsertWithNoReferenceDuplication (nextSignal);
			nextSignal->SetInterlocusBioID (gridLocus->GetBPFromTimeForAnalysis (gridTime), (RGPersistent*)this);
			nextSignal->SignalIsWithinExtendedLocus ();
		}
	}

	return TRUE;
}*/


int Locus :: TestInterlocusSignals (RGDList& signalSet, RGDList& artifacts, ChannelData* laneStd) {

	// We assume that this method tests samples; either use a different method or augment this one
	// to test ladder loci

	DataSignal* firstSignal;
	DataSignal* lastSignal;
	DataSignal* nextSignal;
	DataSignal* nextLocusSignal;
	int repeatNo = mLink->GetCoreNumber ();
	CoordinateTransform* globalSouthern = laneStd->GetIDMap ();
	RGDList tempLocus;

	if (globalSouthern == NULL)
		return -1;

	if (LocusSignalList.IsEmpty ())
		return 0;

	firstSignal = (DataSignal*) LocusSignalList.First ();
	double firstTime = firstSignal->GetMean ();
	double approxBioID1 = globalSouthern->EvaluateWithExtrapolation (firstTime);
	double bioID1 = firstSignal->GetBioID ();

	lastSignal = (DataSignal*) LocusSignalList.Last ();
	double lastTime = lastSignal->GetMean ();
	double approxBioID2 = globalSouthern->EvaluateWithExtrapolation (lastTime);
	double bioID2 = lastSignal->GetBioID ();

	RGDListIterator it (signalSet);
	double mean;
	Notice* newNotice;
	double nextPeak;
	double approxBioID;
	double delID;
	double bioID;
	int bioIDDifference;
	double delTime1;
	double delTime2;
	double locusBioID;
	double locusPeak;
	double stutterLimit = Locus::GetSampleStutterThreshold ();
	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();
	bool isExtraLocusAllele;
	RGString data;

	RGDListIterator itLocus (LocusSignalList);

	while (nextSignal = (DataSignal*) it ()) {

		isExtraLocusAllele = false;
		mean = nextSignal->GetMean ();
		approxBioID = globalSouthern->EvaluateWithExtrapolation (mean);
		delTime1 = fabs (firstTime - mean);
		delTime2 = fabs (lastTime - mean);
		
		if (delTime1 <= delTime2) {

			// use firstSignal data

			delID = approxBioID1 - approxBioID;
			bioID = bioID1 - delID;
		}

		else {

			// use lastSignal data

			delID = approxBioID2 - approxBioID;
			bioID = bioID2 - delID;
		}

		if (mean <= firstTime) {

			bioIDDifference = (int) floor (bioID1 - bioID + 0.5);

			if (bioIDDifference > repeatNo)
				continue;
		}

		else if (mean >= lastTime) {

			bioIDDifference = (int) floor (bioID - bioID2 + 0.5);

			if (bioIDDifference > repeatNo)
				break;
		}

		itLocus.Reset ();

		while (nextLocusSignal = (DataSignal*) itLocus ()) {

			locusBioID = nextLocusSignal->GetBioID ();

			if (locusBioID >= bioID) {

				bioIDDifference = (int) floor (locusBioID - bioID + 0.5);

				if (bioIDDifference > repeatNo)
					break;

				if (bioIDDifference == repeatNo) {

					// stutter, but which way?

					locusPeak = nextLocusSignal->Peak ();
					nextPeak = nextSignal->Peak ();

					if (nextPeak <= stutterLimit * locusPeak) {

						// OL peak is left stutter
					
						nextSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
						"Signal represents stutter for following peak");
						newNotice = new StutterFound;
						nextSignal->AddNoticeToList (newNotice);
						Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
						it.RemoveCurrentItem ();
						artifacts.InsertWithNoReferenceDuplication (nextSignal);
						break;
					}

					else if (locusPeak <= stutterLimit * nextPeak) {

						// Locus peak is right stutter to OL peak

						nextLocusSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", 
						"Signal represents stutter for preceding peak");
						newNotice = new StutterFound;
						nextLocusSignal->AddNoticeToList (newNotice);
						Locus::RemoveExtraneousNoticesFromSignal (nextLocusSignal);
						tempLocus.InsertWithNoReferenceDuplication (nextLocusSignal);
						nextLocusSignal->SetAlleleName ("");

						if (!isExtraLocusAllele) {

							isExtraLocusAllele = true;
							newNotice = new PossibleExtraLocusAllele;
							data = "(" + GetLocusName () + ")";
							newNotice->AddDataItem (data);
							nextSignal->AddNoticeToList (newNotice);
						}

						continue;
					}
				}

				else if (bioIDDifference == 1) {

					// adenylation, but which way?

					locusPeak = nextLocusSignal->Peak ();
					nextPeak = nextSignal->Peak ();

					if (nextPeak <= adenylationLimit * locusPeak) {

						// OL peak is left adenylation
					
						nextSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
							"Signal represents non-template addition (-A) for following peak");
						newNotice = new AdenylationFound;
						nextSignal->AddNoticeToList (newNotice);
						it.RemoveCurrentItem ();
						artifacts.InsertWithNoReferenceDuplication (nextSignal);
						Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
						break;
					}

					else if (locusPeak <= adenylationLimit * nextPeak) {

						// Locus peak is right adenylation to OL peak

						nextLocusSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", 
							"Signal represents non-template addition (+A) for previous peak");
						newNotice = new AdenylationFound;
						nextLocusSignal->AddNoticeToList (newNotice);
						tempLocus.InsertWithNoReferenceDuplication (nextLocusSignal);
						nextLocusSignal->SetAlleleName ("");
						Locus::RemoveExtraneousNoticesFromSignal (nextLocusSignal);

						if (!isExtraLocusAllele) {

							isExtraLocusAllele = true;
							newNotice = new PossibleExtraLocusAllele;
							data = "(" + GetLocusName () + ")";
							newNotice->AddDataItem (data);
							nextSignal->AddNoticeToList (newNotice);
						}

						continue;
					}
				}
			}

			else {

				bioIDDifference = (int) floor (bioID - locusBioID + 0.5);

				if (bioIDDifference > repeatNo)
					continue;

				if (bioIDDifference == repeatNo) {

					// stutter, but which way?

					locusPeak = nextLocusSignal->Peak ();
					nextPeak = nextSignal->Peak ();

					if (nextPeak <= stutterLimit * locusPeak) {

						// OL peak is right stutter
					
						nextSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", 
							"Signal represents stutter for preceding peak");
						newNotice = new StutterFound;
						nextSignal->AddNoticeToList (newNotice);
						Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
						it.RemoveCurrentItem ();
						artifacts.InsertWithNoReferenceDuplication (nextSignal);
						break;
					}

					else if (locusPeak <= stutterLimit * nextPeak) {

						// Locus peak is left stutter to OL peak

						nextLocusSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
							"Signal represents stutter for following peak");
						newNotice = new StutterFound;
						nextLocusSignal->AddNoticeToList (newNotice);
						Locus::RemoveExtraneousNoticesFromSignal (nextLocusSignal);
						tempLocus.InsertWithNoReferenceDuplication (nextLocusSignal);
						nextLocusSignal->SetAlleleName ("");

						if (!isExtraLocusAllele) {

							isExtraLocusAllele = true;
							newNotice = new PossibleExtraLocusAllele;
							data = "(" + GetLocusName () + ")";
							newNotice->AddDataItem (data);
							nextSignal->AddNoticeToList (newNotice);
						}

						continue;
					}
				}

				else if (bioIDDifference == 1) {

					// adenylation, but which way?

					locusPeak = nextLocusSignal->Peak ();
					nextPeak = nextSignal->Peak ();

					if (nextPeak <= adenylationLimit * locusPeak) {

						// OL peak is left adenylation
					
						nextSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", 
							"Signal represents non-template addition (+A) for preceding peak");
						newNotice = new AdenylationFound;
						nextSignal->AddNoticeToList (newNotice);
						Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
						it.RemoveCurrentItem ();
						artifacts.InsertWithNoReferenceDuplication (nextSignal);
						break;
					}

					else if (locusPeak <= adenylationLimit * nextPeak) {

						// Locus peak is right adenylation to OL peak

						nextLocusSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
							"Signal represents non-template addition (-A) for following peak");
						newNotice = new AdenylationFound;
						nextLocusSignal->AddNoticeToList (newNotice);
						tempLocus.InsertWithNoReferenceDuplication (nextLocusSignal);
						nextLocusSignal->SetAlleleName ("");
						Locus::RemoveExtraneousNoticesFromSignal (nextLocusSignal);

						if (!isExtraLocusAllele) {

							isExtraLocusAllele = true;
							newNotice = new PossibleExtraLocusAllele;
							data = "(" + GetLocusName () + ")";
							newNotice->AddDataItem (data);
							nextSignal->AddNoticeToList (newNotice);
						}

						continue;
					}
				}
			}
		}
	}

	if (tempLocus.Entries () > 0) {

		newNotice = new PossibleUnreportedOLAlleles;
		RGString dataString;
		dataString << "(" << tempLocus.Entries () << ")";
		newNotice->AddDataItem (dataString);
		AddNoticeToList (newNotice);
	}

	while (nextLocusSignal = (DataSignal*) tempLocus.GetFirst ()) {

		artifacts.InsertWithNoReferenceDuplication (nextLocusSignal);
		LocusSignalList.RemoveReference (nextLocusSignal);
		mSmartList.RemoveReference (nextLocusSignal);
	}

	return 0;
}


int Locus :: TestInterlocusSignals (RGDList& signalSet, RGDList& artifacts, RGDList& type1List, RGDList& type2List) {

	// We assume that this method tests samples; either use a different method or augment this one
	// to test ladder loci

	// This is a rewrite of the original TestInterlocusSignals to take account of interlocus shared extended signals

	DataSignal* firstSignal;
	DataSignal* lastSignal;
	DataSignal* nextSignal;
	int repeatNo = mLink->GetCoreNumber ();
	RGDList tempLocus;

	if (LocusSignalList.IsEmpty ())
		return 0;

	firstSignal = (DataSignal*) LocusSignalList.First ();
	double firstTime = firstSignal->GetMean ();

	lastSignal = (DataSignal*) LocusSignalList.Last ();
	double lastTime = lastSignal->GetMean ();

	RGDListIterator it (signalSet);
	double mean;
	Notice* newNotice;
	double stutterLimit = Locus::GetSampleStutterThreshold ();
	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();
	RGString data;
	PossibleUnreportedOLAlleles unreportedNotice;

	double timeForOneBPLeft = mTimeForOneIDUnitLeft;
	double timeForOneBPRight = mTimeForOneIDUnitRight;

	if ((timeForOneBPLeft <= 0.0) || (timeForOneBPRight <= 0.0))
		return -1;

	double timeForOneRepeatLeft = repeatNo * timeForOneBPLeft;
	double timeForOneRepeatRight = repeatNo * timeForOneBPRight;

	double minDelForOneBPLeft = 0.5 * timeForOneBPLeft;
	double maxDelForOneBPLeft = 1.5 * timeForOneBPLeft;
	double minDelForOneBPRight = 0.5 * timeForOneBPRight;
	double maxDelForOneBPRight = 1.5 * timeForOneBPRight;

	double minDelForOneRepeatLeft = timeForOneRepeatLeft - minDelForOneBPLeft;
	double maxDelForOneRepeatLeft = timeForOneRepeatLeft + minDelForOneBPLeft;
	double minDelForOneRepeatRight = timeForOneRepeatLeft - minDelForOneBPRight;
	double maxDelForOneRepeatRight = timeForOneRepeatLeft + minDelForOneBPRight;

	RGDList externalSignalsLeft;
	RGDList externalSignalsRight;
	double lowerLimit = firstTime - maxDelForOneRepeatLeft;
	double upperLimit = lastTime + maxDelForOneRepeatRight;

	while (nextSignal = (DataSignal*) it ()) {

		mean = nextSignal->GetMean ();

		if ((lowerLimit < mean) && (mean < upperLimit)) {

			if (mean < firstTime)
				externalSignalsLeft.Append (nextSignal);

			else if (mean > lastTime)
				externalSignalsRight.Append (nextSignal);
		}
	}

	if ((externalSignalsLeft.Entries () == 0) && (externalSignalsRight.Entries () == 0))
		return 0;

	RGDListIterator itL (externalSignalsLeft);
	RGDListIterator itR (externalSignalsRight);
	RGDListIterator itLocus (LocusSignalList);

	// Add test for dual extended loci in each of below...oops, no longer needed:  if a signal is in one of externalSignals lists, it cannot be in this locus' extended locus

	while (nextSignal = (DataSignal*) itL ()) {

		if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneBPLeft, maxDelForOneBPLeft, minDelForOneBPRight, maxDelForOneBPRight, adenylationLimit)) {

			Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
			type1List.InsertWithNoReferenceDuplication (nextSignal);
		}
	}

	while (nextSignal = (DataSignal*) itR ()) {

		if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneBPLeft, maxDelForOneBPLeft, minDelForOneBPRight, maxDelForOneBPRight, adenylationLimit)) {

			Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
			type1List.InsertWithNoReferenceDuplication (nextSignal);
		}
	}

	while (nextSignal = (DataSignal*) it ()) {

		if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneBPLeft, maxDelForOneBPLeft, minDelForOneBPRight, maxDelForOneBPRight, adenylationLimit)) {

			Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
			type1List.InsertWithNoReferenceDuplication (nextSignal);
			nextSignal->SetAlleleName ("");
		}
	}

	if (mIsAMEL)
		return 0;

	itL.Reset ();
	itR.Reset ();
	it.Reset ();

	while (nextSignal = (DataSignal*) itL ()) {

		if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneRepeatLeft, maxDelForOneRepeatLeft, minDelForOneRepeatRight, maxDelForOneRepeatRight, stutterLimit)) {

			Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
			type2List.InsertWithNoReferenceDuplication (nextSignal);
		}
	}

	while (nextSignal = (DataSignal*) itR ()) {

		if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneRepeatLeft, maxDelForOneRepeatLeft, minDelForOneRepeatRight, maxDelForOneRepeatRight, stutterLimit)) {

			Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
			type2List.InsertWithNoReferenceDuplication (nextSignal);
		}
	}

	while (nextSignal = (DataSignal*) it ()) {

		if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneRepeatLeft, maxDelForOneRepeatLeft, minDelForOneRepeatRight, maxDelForOneRepeatRight, stutterLimit)) {

			Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
			type2List.InsertWithNoReferenceDuplication (nextSignal);
			nextSignal->SetAlleleName ("");
			
			if (!IsNoticeInList (&unreportedNotice)) {

				newNotice = new PossibleUnreportedOLAlleles;
				AddNoticeToList (newNotice);
			}
		}
	}

	return 0;
}


int Locus :: TestAmelogenin () {

//	if (mLink->GetLocusName () != "AMEL")
	if (!mIsAMEL)
		return 0;

	Notice* newNotice;

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->GetAlleleName () == "1")
			return 0;
	}

	newNotice = new XPeakMissingFromAmelogenin;
	AddNoticeToList (newNotice);
	return -1;
}


int Locus :: TestPositiveControl (IndividualLocus* locus, RGDList& artifacts) {

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	IndividualAllele* newAllele;
	RGString alleleName;
	RGDList alleleSet;
	Notice* newNotice;
	Notice* newSignalNotice;

	while (nextSignal = (DataSignal*) it ()) {

		alleleName = nextSignal->GetAlleleName ();
		newAllele = new IndividualAllele;
		newAllele->SetName (alleleName);
		newAllele->SetAssociatedPeak (nextSignal);
		alleleSet.Append (newAllele);
	}

	RGDList extraAlleles;
	RGDList missingAlleles;
	SetDifference (alleleSet, locus->mAllelesByName, extraAlleles, missingAlleles);
	
	if ((extraAlleles.Entries () == 0) && (missingAlleles.Entries () == 0)) {

		alleleSet.ClearAndDelete ();
		return 0;
	}

	newNotice = new PosCtrlReqReview;
	RGString data;
	data = "Extra Allele(s):  ";
	int i = 0;

	if (extraAlleles.Entries () == 0)
		data << "None; ";

	else {

		while (newAllele = (IndividualAllele*) extraAlleles.GetFirst ()) {

			if (i == 0)
				data << newAllele->GetName ();

			else
				data << ", " << newAllele->GetName ();

			i++;
			nextSignal = newAllele->GetAssociatedPeak ();

			if (nextSignal != NULL) {

				newSignalNotice = new ExtraneousPositiveControlPeak;
				nextSignal->AddNoticeToList (newSignalNotice);
				artifacts.InsertWithNoReferenceDuplication (nextSignal);
			}
		}

		data << "; ";
	}

	data << "Missing Allele(s):  ";
	i = 0;

	if (missingAlleles.Entries () == 0)
		data << "None; ";

	else {

		while (newAllele = (IndividualAllele*) missingAlleles.GetFirst ()) {

			if (i == 0)
				data << newAllele->GetName ();

			else
				data << ", " << newAllele->GetName ();

			i++;
		}

		data << "; ";
	}

	newNotice->AddDataItem (data);
	alleleSet.ClearAndDelete ();
	AddNoticeToList (newNotice);
	return -1;
}


int Locus :: TestSignalPositionRelativeToLocus (DataSignal* signal, Locus* gridLocus) {

	double mean = signal->GetMean ();
	return gridLocus->TestSignalTimeRelativeToGridLocus (mean);
}


int Locus :: TestSignalPositionRelativeToLocus (DataSignal* signal) {

	if (mExtendedLeft.ContainsReference (signal))
		return -1;

	if (mExtendedRight.ContainsReference (signal))
		return 1;

	return 0;
}


int Locus :: TestSignalTimeRelativeToGridLocus (double time) {

	if (time < MinimumSampleTime)
		return -1;

	if (time > MaximumSampleTime)
		return 1;

	return 0;
}


double Locus :: GetMaximumHeight () {

	double maxHeight = 0.0;
	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	double peak;

	while (nextSignal = (DataSignal*) it ()) {

		peak = nextSignal->Peak ();

		if (peak > maxHeight)
			maxHeight = peak;
	}
	
	if (LocusSignalList.Entries () == 1)
		maxHeight *= 0.5;

	return maxHeight;
}


void Locus :: PrecomputeAverages (RGDList& set) {

	RGDListIterator it (set);
	DataSignal* nextSignal;
	averageWidth = 0.0;
	averagePeak = 0.0;
	int NumberOfSignals = set.Entries ();
	double left, right;

	while (nextSignal = (DataSignal*)it ()) {

		left = nextSignal->LeftEndPoint ();
		right = nextSignal->RightEndPoint ();
		averageWidth += nextSignal->GetStandardDeviation ();
		averagePeak += nextSignal->Peak ();
	}

	if (NumberOfSignals > 0) {

		averageWidth = averageWidth / NumberOfSignals;
		averagePeak = averagePeak / NumberOfSignals;
	}
}


int Locus :: TestNeighbors (DataSignal* previous, DataSignal* testSignal, DataSignal* following) {

	// We assume that this is a ladder test...AnalyzeGridLocus is the only method that calls this routine

	double Width = testSignal->GetStandardDeviation ();
	double peak = testSignal->Peak ();
	Boolean NotAcceptable = FALSE;
	Boolean Marginal = FALSE;
	Notice* newNotice;
	RGString info;

	double stutterLimit = Locus::GetGridStutterThreshold ();
	double adenylationLimit = Locus::GetGridAdenylationThreshold ();

/*	if (Width < Locus::MinimumFractionOfAverageWidth * averageWidth) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooNarrow, "", "Signal is too narrow for set");
	}*/
/*

	if (Width > Locus::MaximumMultipleOfAverageWidth * averageWidth) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooWide, "", "Signal is too wide for set");
		newNotice = new PoorPeakMorphology;
		info << "(Width)";
		newNotice->AddDataItem (info);
		testSignal->AddNoticeToList (newNotice);
	}

	if (peak < Locus::MinimumFractionOfAveragePeak * averagePeak) {

		Marginal = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooLow, "", "Signal is too low for set");
	}

	if ((Locus::MaximumMultipleOfAveragePeak > 0.0) && (peak > Locus::MaximumMultipleOfAveragePeak * averagePeak)) {

		Marginal = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooHigh, "", "Signal is too high for set");
	}
*/

	double ThisMean = testSignal->GetMean ();
	double ThisPeak = peak;  // = testSignal->Peak ();
	double ThisSigma = Width; // = testSignal->GetStandardDeviation ();
	double RepetitionWidth1 = 2.0 * (double)mLink->GetCoreNumber ();
	double RepetitionWidth2 = 6.0 * (double)mLink->GetCoreNumber ();

	//
	// test for non-template addition (-A and +A) and for stutter (n - CoreRepetition and n + CoreRepetition)
	//

	if (following != NULL) {

		double NextMean = following->GetMean ();
		double NextPeak = following->Peak ();

		if (NextMean - ThisMean < RepetitionWidth1 * ThisSigma) {

			if (ThisPeak < adenylationLimit * NextPeak) {

				NotAcceptable = TRUE;
				testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
					"Signal represents non-template addition (-A) for following peak");
				newNotice = new AdenylationFound;
				newNotice->AddDataItem ("(Following peak");
				testSignal->AddNoticeToList (newNotice);
				Locus::RemoveExtraneousNoticesFromSignal (testSignal);
			}
		}

		else if ((NextMean - ThisMean < RepetitionWidth2 * ThisSigma) && (ThisPeak < stutterLimit * NextPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
				"Signal represents stutter for following peak");
			newNotice = new StutterFound;
			newNotice->AddDataItem ("(Following peak");
			testSignal->AddNoticeToList (newNotice);
			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
		}
	}

	if (previous != NULL) {

		double PreviousMean = previous->GetMean ();
		double PreviousPeak = previous->Peak ();

		if ((ThisMean - PreviousMean < RepetitionWidth1 * ThisSigma) && (ThisPeak < adenylationLimit * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", "Signal represents non-template addition (+A) for previous peak");

			newNotice = new AdenylationFound;
			newNotice->AddDataItem ("(Preceding peak");
			testSignal->AddNoticeToList (newNotice);
			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
		}

		else if ((ThisMean - PreviousMean < RepetitionWidth2 * ThisSigma) && (ThisPeak < stutterLimit * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", "Signal represents stutter for previous peak");

			newNotice = new StutterFound;
			newNotice->AddDataItem ("(Preceding peak");
			testSignal->AddNoticeToList (newNotice);
			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
		}
	}

/*	if ((previous == NULL) && (following != NULL)) {

		if (ThisPeak <= 0.4 * mMaxPeak) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::SignalTooLow, "", "Signal is too low for set");
		}
	}

	else if ((following == NULL) && (previous != NULL)) {

		if (ThisPeak <= 0.33 * mMaxPeak) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::SignalTooLow, "", "Signal is too low for set");
		}
	}*/

	if (previous != NULL)
		testSignal->SetPreviousPeak (previous->Peak ());
	
	else
		testSignal->SetPreviousPeak (0.0);

	if (following != NULL)
		testSignal->SetNextPeak (following->Peak ());
	
	else
		testSignal->SetNextPeak (0.0);

	if (NotAcceptable)
		return -1;

	if (Marginal)
		return 0;

	return 1;
}


int Locus :: TestSampleNeighbors (DataSignal* previous, DataSignal* testSignal, DataSignal* following) {

	double peak = testSignal->Peak ();
	Boolean NotAcceptable = FALSE;
	Boolean Marginal = FALSE;
	Notice* newNotice;
//	Notice* newNotice2;
//	RGString info;

	double ThisMean = testSignal->GetMean ();
	double ThisPeak = testSignal->Peak ();
	double ThisSigma = testSignal->GetStandardDeviation ();

	double stutterLimit = Locus::GetSampleStutterThreshold ();
	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();

	//
	// test for non-template addition (-A and +A) and for stutter (n - 4 and n + 4)
	//

	if (following != NULL) {

		double NextMean = following->GetMean ();
		double NextPeak = following->Peak ();

		if (NextMean - ThisMean < 8.0 * ThisSigma) {

			if (ThisPeak < adenylationLimit * NextPeak) {

				NotAcceptable = TRUE;
				testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
					"Signal represents non-template addition (-A) for following peak");
				newNotice = new AdenylationFound;
				newNotice->AddDataItem ("(Following peak)");
				testSignal->AddNoticeToList (newNotice);
				Locus::RemoveExtraneousNoticesFromSignal (testSignal);
/*				newNotice2 = new AdenylationFound;
				newNotice2->AddDataItem ("(Following peak)");
				info = "at sample ";
				info << testSignal->GetMean ();
				newNotice2->AddDataItem (info);
				AddNoticeToList (newNotice2);*/
			}
		}

		else if ((NextMean - ThisMean < 24.0 * ThisSigma) && (ThisPeak < stutterLimit * NextPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
				"Signal represents stutter for following peak");
			newNotice = new StutterFound;
			newNotice->AddDataItem ("(Following peak");
			testSignal->AddNoticeToList (newNotice);
			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
/*			newNotice2 = new StutterFound;
			newNotice2->AddDataItem ("(Following peak");
			info = "at sample ";
			info << testSignal->GetMean ();
			newNotice2->AddDataItem (info);
			AddNoticeToList (newNotice2);*/
		}
	}

	if (previous != NULL) {

		double PreviousMean = previous->GetMean ();
		double PreviousPeak = previous->Peak ();

		if ((ThisMean - PreviousMean < 8.0 * ThisSigma) && (ThisPeak < adenylationLimit * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", 
				"Signal represents non-template addition (+A) for previous peak");
			newNotice = new AdenylationFound;
			newNotice->AddDataItem ("(Preceding peak)");
			testSignal->AddNoticeToList (newNotice);
			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
/*			newNotice2 = new AdenylationFound;
			newNotice2->AddDataItem ("(Preceding peak)");
			info = "at sample ";
			info << testSignal->GetMean ();
			newNotice2->AddDataItem (info);
			AddNoticeToList (newNotice2);*/
		}

		else if ((ThisMean - PreviousMean < 24.0 * ThisSigma) && (ThisPeak < stutterLimit * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", 
				"Signal represents stutter for previous peak");
			newNotice = new StutterFound;
			newNotice->AddDataItem ("(Preceding peak");
			testSignal->AddNoticeToList (newNotice);
			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
/*			newNotice2 = new StutterFound;
			newNotice2->AddDataItem ("(Preceding peak");
			info = "at sample ";
			info << testSignal->GetMean ();
			newNotice2->AddDataItem (info);
			AddNoticeToList (newNotice2);*/
		}
	}

	if (previous != NULL)
		testSignal->SetPreviousPeak (previous->Peak ());
	
	else
		testSignal->SetPreviousPeak (0.0);

	if (following != NULL)
		testSignal->SetNextPeak (following->Peak ());
	
	else
		testSignal->SetNextPeak (0.0);

	if (NotAcceptable)
		return -1;

	if (Marginal)
		return 0;

	return 1;
}


int Locus :: TestSampleNeighbors (RGDList& previousList, DataSignal* testSignal, RGDList& followingList) {

	// Assumes that previousList contains signals up to but not including testSignal and that followingList
	// contains signals immediately following testSignal

	//double peak = testSignal->Peak ();
	//Boolean NotAcceptable = FALSE;
	//Boolean Marginal = FALSE;
	//Notice* newNotice;

	//DataSignal* following;
	//DataSignal* previous;
	//RGDListIterator followingIterator (followingList);
	//RGDListIterator previousIterator (previousList);

	//double ThisMean = testSignal->GetMean ();
	//double ThisPeak = testSignal->Peak ();
	//double ThisSigma = testSignal->GetStandardDeviation ();
	//double NextMean;
	//double NextPeak;
	//double PreviousMean;
	//double PreviousPeak;
	//double RightAdenylationLimit = ThisMean + 8.0 * ThisSigma;
	//double RightStutterLimit = ThisMean + 24.0 * ThisSigma;
	//double LeftAdenylationLimit = ThisMean - 8.0 * ThisSigma;
	//double LeftStutterLimit = ThisMean - 24.0 * ThisSigma;
	//bool foundAdenylation = false;
	//bool foundStutter = false;

	//double stutterLimit = Locus::GetSampleStutterThreshold ();
	//double adenylationLimit = Locus::GetSampleAdenylationThreshold ();

	////
	//// test for non-template addition (-A and +A) and for stutter (n - 4 and n + 4)
	////

	//while (following = (DataSignal*) followingIterator()) {

	//	NextMean = following->GetMean ();
	//	NextPeak = following->Peak ();

	//	if (NextMean < RightAdenylationLimit) {

	//		if ((!foundAdenylation) && (ThisPeak < adenylationLimit * NextPeak)) {

	//			NotAcceptable = TRUE;
	//			testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
	//				"Signal represents non-template addition (-A) for following peak");
	//			newNotice = new AdenylationFound;
	//			newNotice->AddDataItem ("(Following peak)");
	//			testSignal->AddNoticeToList (newNotice);
	//			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
	//			foundAdenylation = true;
	//		}
	//	}

	//	else if (NextMean < RightStutterLimit) {

	//		if (ThisPeak < stutterLimit * NextPeak) {

	//			NotAcceptable = TRUE;
	//			testSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
	//				"Signal represents stutter for following peak");
	//			newNotice = new StutterFound;
	//			newNotice->AddDataItem ("(Following peak");
	//			testSignal->AddNoticeToList (newNotice);
	//			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
	//			foundStutter = true;
	//			break;
	//		}
	//	}

	//	else
	//		break;  // NextMean is larger than RightStutterLimit so stop looking
	//}

	//previousIterator.ResetToEnd ();
	//previous = (DataSignal*) previousIterator.CurrentItem ();

	//if (previous != NULL) {

	//	testSignal->SetPreviousPeak (previous->Peak ());
	//	previous->SetNextPeak (ThisPeak);
	//}
	//
	//else
	//	testSignal->SetPreviousPeak (0.0);

	//testSignal->SetNextPeak (0.0);

	//while (previous != NULL) {

	//	PreviousMean = previous->GetMean ();
	//	PreviousPeak = previous->Peak ();

	//	if (PreviousMean > LeftAdenylationLimit) {

	//		if ((!foundAdenylation) && (ThisPeak < adenylationLimit * PreviousPeak)) {

	//			NotAcceptable = TRUE;
	//			testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", 
	//				"Signal represents non-template addition (+A) for previous peak");
	//			newNotice = new AdenylationFound;
	//			newNotice->AddDataItem ("(Preceding peak)");
	//			testSignal->AddNoticeToList (newNotice);
	//			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
	//			foundAdenylation = true;
	//		}
	//	}

	//	else if ((!foundStutter) && (PreviousMean > LeftStutterLimit)) {

	//		if (ThisPeak < stutterLimit * PreviousPeak) {

	//			NotAcceptable = TRUE;
	//			testSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", 
	//				"Signal represents stutter for previous peak");
	//			newNotice = new StutterFound;
	//			newNotice->AddDataItem ("(Preceding peak");
	//			testSignal->AddNoticeToList (newNotice);
	//			Locus::RemoveExtraneousNoticesFromSignal (testSignal);
	//			break;
	//		}
	//	}

	//	else
	//		break;  // PreviousMean is less than LeftStutterLimit so stop looking

	//	--previousIterator;
	//	previous = (DataSignal*) previousIterator.CurrentItem ();
	//}

	//if (NotAcceptable)
	//	return -1;

	//if (Marginal)
	//	return 0;

	return 1;
}


int Locus :: TestSampleAverages (ChannelData* lsData, DataSignal* testSignal, Boolean testRatio) {

	double Width = testSignal->GetStandardDeviation ();
	double peak = testSignal->Peak ();
	double mean = testSignal->GetMean ();
	Notice* newNotice;
	RGString info;
	double targetWidth = lsData->GetWidthAtTime (mean);

	if (targetWidth < 0.0)
		return 1;

	double tooNarrow = 0.5 * targetWidth;
	double tooWide = 2.0 * targetWidth;

	if (Width < tooNarrow) {

		testSignal->AddNoticeToList (OutputLevelManager::SignalTooNarrow, "", "Signal is too narrow for set");
		newNotice = new PoorPeakMorphology;

		if (testSignal->IsNoticeInList (newNotice))
			delete newNotice;

		else {

			info = "(Too Narrow)";
			newNotice->AddDataItem (info);
			testSignal->AddNoticeToList (newNotice);
		}
	}

	if (Width > tooWide) {

		testSignal->AddNoticeToList (OutputLevelManager::SignalTooWide, "", "Signal is too wide for set");
		newNotice = new PoorPeakMorphology;

		if (testSignal->IsNoticeInList (newNotice))
			delete newNotice;

		else {

			info = "(Too Wide)";
			newNotice->AddDataItem (info);
			testSignal->AddNoticeToList (newNotice);
		}
	}

	return 1;
}


double Locus :: TotalPeakHeight (int& numberOfPeaks) {

	RGDListIterator it (LocusSignalList);
	double total = 0.0;
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) it ())
		total += nextSignal->Peak ();

	numberOfPeaks = LocusSignalList.Entries ();
	return total;
}


int Locus :: FinalTestForPeakSizeAndNumber (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes, RGDList& artifacts) {

	Notice* newNotice;
	RGString info;
	DataSignal* nextSignal;
	double peak;
	double peak2;
	double temp;
	double heteroLimit = Locus::GetHeterozygousImbalanceLimit ();
	double minBoundForHomozygote = Locus::GetMinBoundForHomozygote ();
	RGDList alleleList;
	IndividualAllele* newAllele;
	RGString alleleName;
	bool testForAcceptedTriallele;
	ThreeAllelesFound triAllele;
	bool criticalArtifactFound = false;
	int criticalLevel = Notice::GetMessageTrigger ();
	int nextSignalLevel;
	RGDListIterator it (LocusSignalList);
	RGString localAlleleName;
	int maxAllowedLevel;
	bool foundOLAllele = false;
	ResidualExceedsThreshold residualTarget;

	double minBioID = (double) CoreBioComponent::GetMinBioIDForArtifacts ();

	if (mIsAMEL) {

		while (nextSignal = (DataSignal*) it ()) {

			if (!nextSignal->IsOffGrid ())
				continue;

//			location = TestSignalPositionRelativeToLocus (nextSignal);
			it.RemoveCurrentItem ();
			artifacts.InsertWithNoReferenceDuplication (nextSignal);
			newNotice = nextSignal->RemoveNotice (&residualTarget);
			delete newNotice;
			newNotice = new OLPeakInAMEL;
			nextSignal->AddNoticeToList (newNotice);
			//alleleName = "";
			//nextSignal->SetAlleleName (alleleName, -location);

			if (!foundOLAllele) {

				newNotice = new ExtraneousPeakInAMEL;
				AddNoticeToList (newNotice);
			}

			foundOLAllele = true;
		}

		it.Reset ();
	}

	while (nextSignal = (DataSignal*) it ()) {

		if ((minBioID > 0.0) && (nextSignal->GetApproximateBioID () < minBioID)) {

			it.RemoveCurrentItem ();
			continue;
		}

		nextSignalLevel = nextSignal->GetHighestMessageLevel ();
		maxAllowedLevel = nextSignal->GetMaxAllowedMessageLevel ();

		if ((nextSignalLevel > 0) && (nextSignalLevel < maxAllowedLevel))
			nextSignalLevel = maxAllowedLevel;

		if ((nextSignalLevel > 0) && (nextSignalLevel <= criticalLevel)) {

			if (!criticalArtifactFound) {

				localAlleleName = nextSignal->GetAlleleName ();

				if (localAlleleName.Length () > 0) {

					criticalArtifactFound = true;
					newNotice = new LocusHasAllelesWithCriticalNotices;
				}
			}

			else
				info << ", ";

			if (mIsAMEL) {

				localAlleleName = nextSignal->GetAlleleName ();

				if (localAlleleName == "1")
					info << "X";

				else if (localAlleleName == "2")
					info << "Y";

				else
					info << localAlleleName;
			}
			
			else
				info << nextSignal->GetAlleleName ();
		}
	}

	int N = LocusSignalList.Entries ();

	if (criticalArtifactFound) {

		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);
	}

	if (LocusSignalList.Entries () <= 2) {

		newNotice = RemoveNotice (&triAllele);
		delete newNotice;
	}

	if (N == 0) {

		if (!isNegCntl) {

			newNotice = new NoGenotypeFound;
			AddNoticeToList (newNotice);
			return -1;
		}

		return 0;
	}

	if (isNegCntl) {

		newNotice = new UnexpectedNumberOfPeaks;
		AddNoticeToList (newNotice);
		return -1;
	}

	if (N == 1) {

		// Test for proper homozygote
		nextSignal = (DataSignal*) LocusSignalList.First ();
		peak = nextSignal->Peak ();

		if ((minBoundForHomozygote > 0.0) && (peak < minBoundForHomozygote)) {

			newNotice = new NotOKHomozygoteFound;
			nextSignal->AddNoticeToList (newNotice);
			artifacts.InsertWithNoReferenceDuplication (nextSignal);
			newNotice = new NotOKHomozygoteFound;
			AddNoticeToList (newNotice);
			return -1;
		}

		return 0;
	}

	if (N == 2) {

		nextSignal = (DataSignal*) LocusSignalList.First ();
		peak = nextSignal->Peak ();
		nextSignal = (DataSignal*) LocusSignalList.Last ();
		peak2 = nextSignal->Peak ();

		if (peak2 > peak) {

			temp = peak;
			peak = peak2;
			peak2 = temp;
		}
		
		if (peak2 < heteroLimit * peak) {

			newNotice = new HeteroImbalanceFound;
			info = "**Peaks = ";
			info << peak << ", ";
			newNotice->AddDataItem (info);
			info = "";
			info << peak2;
			newNotice->AddDataItem (info);
			AddNoticeToList (newNotice);
			return -1;
		}

		return 0;
	}

	if (N == 3) {

		it.Reset ();

		while (nextSignal = (DataSignal*) it ()) {

			alleleName = nextSignal->GetAlleleName ();
			newAllele = new IndividualAllele ();
			newAllele->SetName (alleleName);
			alleleList.Append (newAllele);
		}

		if (isPosCntl)
			testForAcceptedTriallele = pGenotypes->ContainsControlTriallele (GetLocusName (), alleleList);

		else
			testForAcceptedTriallele = pGenotypes->ContainsSampleTriallele (GetLocusName (), alleleList);
		
		alleleList.ClearAndDelete ();

		if (!testForAcceptedTriallele) {

			//if (mNumberOfCraters >= 1) {

			//	newNotice = RemoveNotice (&triAllele);
			//	delete newNotice;
			//	return -1;
			//}

			newNotice = new ThreeAllelesFound;
			AddNoticeToList (newNotice);
			return -1;
		}

		else {

			newNotice = RemoveNotice (&triAllele);
			delete newNotice;
		}

		return 0;
	}

	//  Later, come back and test the balance between these peaks.
	//  At this point, there are 4 or more alleles, so forget any more tests...except for craters...

	/*if (N - mNumberOfCraters <= 2) {

		newNotice = RemoveNotice (&triAllele);
		delete newNotice;
		return -1;
	}*/

	newNotice = new ThreeAllelesFound;
	AddNoticeToList (newNotice);
	return -1;
}


int Locus :: CorrectCrossChannelAnalyses (RGDList& artifacts, bool isNegCntl) {

	// We assume this method will be used for samples.  For ladders, either use a different method or augment
	// this one.
	
	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	double maxPeak = 0.0;
	double peak;
	StutterFound stutter;
	PullUpFound pullup;
	AdenylationFound adenylation;
	PullUpPrimaryChannel primary;
	RemovablePrimaryPullUp removable;
	LowOLAllelePullUp lowOL;
	IgnoreLowOnLadderAlleles lowOnGrid;
	LowTriAllelePullUp lowTri;
	AllelePullUp allelePullup;
	OLAlleleFound oLAllele;
//	Notice* replacement;
	double peakThreshold;
	double alleleThreshold;
	RGDListIterator it2 (LocusSignalList);
//	DataSignal* targetSignal;
//	int diff;
	Notice* newNotice;
//	int id;
//	int id2;

	double stutterLimit = Locus::GetSampleStutterThreshold ();
	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();
	double pullupFractionalLimit = Locus::GetSamplePullupFractionalFilter ();

	while (nextSignal = (DataSignal*) it ()) {

		peak = nextSignal->Peak ();
		//it2.Reset ();
		//id = (int) floor (nextSignal->GetBioID () + 0.5);

		//while (targetSignal = (DataSignal*) it2 ()) {

		//	id2 = (int) floor (targetSignal->GetBioID () + 0.5);
		//	diff = abs (id - id2);

		//	if ((diff == 1) && (peak < adenylationLimit * targetSignal->Peak ())) {

		//		// this is adenylation

		//		newNotice = new AdenylationFound;
		//		nextSignal->AddNoticeToList (newNotice);
		//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
		//		break;

		//	}

		//	else if ((diff == mLink->GetCoreNumber ()) && (peak < stutterLimit * targetSignal->Peak ())) {

		//		// this is stutter

		//		newNotice = new StutterFound;
		//		nextSignal->AddNoticeToList (newNotice);
		//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
		//		break;
		//	}
		//}

		if (peak > maxPeak)
			maxPeak = peak;
	}

	//it.Reset ();

	//while (nextSignal = (DataSignal*) it ()) {

	//	if ((nextSignal->IsNoticeInList (&stutter)) || (nextSignal->IsNoticeInList (&adenylation))) {

	//		it.RemoveCurrentItem ();
	//		artifacts.InsertWithNoReferenceDuplication (nextSignal);
	//		nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
	//		nextSignal->SetAlleleName ("");
	//	}
	//}

	it.Reset ();
	peakThreshold = pullupFractionalLimit * maxPeak;  // allows ignoring OL allele with "low" peak
	alleleThreshold = Locus::GetHeterozygousImbalanceLimit () * maxPeak;  // allows ignoring pull-up
	int N = LocusSignalList.Entries ();
	double p;
//	InterchannelLinkage* iChannel;

	while (nextSignal = (DataSignal*) it ()) {

		if (isNegCntl) {

			newNotice = new UnexpectedNumberOfPeaks;

			if (IsNoticeInList (newNotice))
				delete newNotice;

			else
				AddNoticeToList (newNotice);

			nextSignal->SetAlleleName ("");
			it.RemoveCurrentItem ();
			newNotice = new ExtraneousPeakInNegativeControl;
			nextSignal->AddNoticeToList (newNotice);
			artifacts.InsertWithNoReferenceDuplication (nextSignal);
		}

		else if (nextSignal->IsNoticeInList (&pullup)) {  // check instead for HasCrossChannelSignalLink or HasEverHadCrossChannelSignalLink?

			p = nextSignal->Peak ();

			if (p >= alleleThreshold) {

				nextSignal->ReplaceNotice (&pullup, &allelePullup);

				//iChannel = nextSignal->GetInterchannelLink ();

				//if (iChannel != NULL) {

				//	iChannel->RemoveDataSignal (nextSignal, &pullup, &allelePullup, &primary, &removable);

				//	if (iChannel->IsEmpty ())
				//		iChannel->RemoveAll (&primary, NULL, &pullup, NULL);

				//	else
				//		iChannel->RecalculatePrimarySignal (&pullup, &primary);
				//}

				//else
				//	nextSignal->RemoveCrossChannelSignalLink (&primary, &pullup, &removable, &allelePullup);
			}

			//else if ((pullupFractionalLimit > 0.0) && (p <= peakThreshold)) {

			//	nextSignal->ClearNoticeObjects ();
			//	nextSignal->RemoveCrossChannelSignalLink (&primary, &pullup, &removable, &lowTri);
			//	it.RemoveCurrentItem ();
			//	artifacts.InsertWithNoReferenceDuplication (nextSignal);
			//	nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
			//	nextSignal->SetAlleleName ("");
			//}
		}
	}

	//it.Reset ();

	//while (nextSignal = (DataSignal*) it ()) {

	//	if (nextSignal->IsNoticeInList (&primary)) {

	//		if ((nextSignal->IsNoticeInList (&stutter)) || (nextSignal->IsNoticeInList (&adenylation)))
	//			nextSignal->RemoveCrossChannelSignalLink (&primary, &pullup, NULL, NULL);
	//	}
	//}

	return 0;
}


int Locus :: CleanUpSignalList (RGDList& artifacts) {

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	Notice* newNotice;
	Notice* oldNotice;
	double residual;
	double residualThreshold = Locus::GetMaxResidualForAlleleCalls ();
	RGString data;
	Notice* locusLevelResidualMsg;
	bool addedResidualToLocus = false;
	int truncatedResidual;
	int msgLevel;
	double absResidual;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->IsDoNotCall ()) {

			nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
			nextSignal->SetAlleleName ("");
			it.RemoveCurrentItem ();
		}
	}

	RGDListIterator leftIt (mExtendedLeft);
	RGDListIterator rightIt (mExtendedRight);

	while (nextSignal = (DataSignal*) leftIt ()) {

		if (nextSignal->IsDoNotCall ()) {

			nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
			nextSignal->SetAlleleName ("");
			it.RemoveCurrentItem ();
		}
	}

	while (nextSignal = (DataSignal*) rightIt ()) {

		if (nextSignal->IsDoNotCall ()) {

			nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
			nextSignal->SetAlleleName ("");
			it.RemoveCurrentItem ();
		}
	}

	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		msgLevel = nextSignal->GetHighestMessageLevel ();

//		if ((msgLevel > 0) && (msgLevel <= Notice::GetSeverityTrigger ())) {
		if (msgLevel > 0) {

//			it.RemoveCurrentItem ();
			artifacts.InsertWithNoReferenceDuplication (nextSignal);
		}
	}

	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->IsOffGrid ()) {

			newNotice = new OLAlleleFound;
			oldNotice = GetNotice (newNotice);

			if (oldNotice != NULL) {

				delete newNotice;
				oldNotice->AddDataItem (nextSignal->GetAlleleName ());
			}

			else {

				newNotice->AddDataItem (nextSignal->GetAlleleName ());
				AddNoticeToList (newNotice);
			}
		}

		residual = nextSignal->GetBioIDResidual ();
		absResidual = fabs (residual);

		if ((residualThreshold > 0.0) && (absResidual > residualThreshold)) {

			newNotice = new ResidualExceedsThreshold;
			data = "(";
			data << residual << ")";
			newNotice->AddDataItem (data);
			nextSignal->AddNoticeToList (newNotice);
			artifacts.InsertWithNoReferenceDuplication (nextSignal);

			if (!addedResidualToLocus) {

				addedResidualToLocus = true;
				locusLevelResidualMsg = new LocusContainsPeaksWithExcessiveResiduals;
				AddNoticeToList (locusLevelResidualMsg);
				data = "";
			}

			else
				data = ", ";

			data << nextSignal->GetAlleleName ();
			truncatedResidual = (int) floor (1000.0 * absResidual + 0.5);

			if (residual > 0.0)
				data << " (0." << truncatedResidual << ")";

			else
				data << " (-0." << truncatedResidual << ")";

			locusLevelResidualMsg->AddDataItem (data);
		}
	}

/*	if (LocusSignalList.Entries () <= 2) {

		newNotice = RemoveNotice (&triAllele);
		delete newNotice;
	}

	else {

		newNotice = new ThreeAllelesFound;
		AddNoticeToList (newNotice);
	}*/

	return 0;
}


int Locus :: CorrectGridSignalList (RGDList& artifacts) {

	RGDListIterator it (FinalSignalList);
	DataSignal* nextSignal;
	PullUpFound pullup;
	PullUpPrimaryChannel primary;
	StutterFound stutter;
	AdenylationFound adenylation;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->IsNoticeInList (&pullup))
			nextSignal->RemoveCrossChannelSignalLink (&primary, &pullup, NULL, NULL);
	}

	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->IsNoticeInList (&pullup)) {

			if ((nextSignal->IsNoticeInList (&stutter)) || (nextSignal->IsNoticeInList (&adenylation)))
				nextSignal->RemoveCrossChannelSignalLink (&primary, &pullup, NULL, NULL);
		}
	}

	return 0;
}


int Locus :: CleanUpGridSignalList (RGDList& artifacts) {

	RGDListIterator it (FinalSignalList);
	DataSignal* nextSignal;
	int msgLevel;
	int trigger = Notice::GetMessageTrigger ();
	int returnValue = 0;

	while (nextSignal = (DataSignal*) it ()) {

		msgLevel = nextSignal->GetHighestMessageLevel ();

		if (msgLevel > 0) {

			artifacts.InsertWithNoReferenceDuplication (nextSignal);

			if (msgLevel <= trigger)
				returnValue = -1;
		}
	}

	return returnValue;
}


double Locus :: GetBPFromTimeForAnalysis (double time) {

	if (mSampleAnalysisMap != NULL)
		return mSampleAnalysisMap->EvaluateWithExtrapolation (time);

	cout << "Attempted to use Sample Analysis Map without initialization for locus named " << mLink->GetLocusName () << "\n";
	return -1.0;
}


double Locus :: GetTimeFromBP (double bp) {

	if (mSampleTimeFromBPMap != NULL)
		return mSampleTimeFromBPMap->EvaluateWithExtrapolation (bp);

	ErrorString << "Attempted to use Sample Time To BP Map without initialization for locus named " << mLink->GetLocusName () << "\n";
	return -1.0;
}


Boolean Locus :: GridLocusContainsID (double id) {

	if ((id >= mLink->GetMinGridID()) && (id <= mLink->GetMaxGridID ()))
		return TRUE;

	return FALSE;
}


Boolean Locus :: ExtendedLocusContainsID (double id) {

	if ((id >= mLink->GetMinimumBound ()) && (id <= mLink->GetMaximumBound ()))
		return TRUE;

	return FALSE;
}


Boolean Locus :: ExtendedLocusContainsID (double id, int& location) {

	if ((id >= mLink->GetMinimumBound ()) && (id <= mLink->GetMaximumBound ())) {

		if (id < mLink->GetMinGridID ())
			location = -1;

		else if (id > mLink->GetMaxGridID ())
			location = 1;

		else
			location = 0;

		return TRUE;
	}

	return FALSE;
}


int Locus :: DirectionOfTimeFromLocus (double time) {

	if (time < mMinTimeForRoundedCore)
		return -1;

	if (time > mMaxTimeForRoundedCore)
		return 1;

	return 0;
}


double Locus :: GridDistance (double id) {

	if (GridLocusContainsID (id))
		return 0.0;

	if (id < mLink->GetMinGridID ())
		return mLink->GetMinGridID () - id;

	return id - mLink->GetMaxGridID ();
}


double Locus :: ExtendedDistance (double id) {

	if (ExtendedLocusContainsID (id))
		return 0.0;

	if (id < mLink->GetMinimumBound ())
		return mLink->GetMinimumBound () - id;

	return id - mLink->GetMaximumBound ();
}


void Locus :: ReportSampleData (RGTextOutput& ExcelText, const RGString& indent) {

	Endl endLine;
	ExcelText.SetOutputLevel (OutputLevelManager::LocusSampleCharacteristics);
	ExcelText << endLine;
	ExcelText << "Samples for locus named:  " << mLink->GetLocusName () << endLine;
	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	RGString doubleIndent = indent + "     ";
	RGString tripleIndent = doubleIndent + "     ";
	double bp;
	int IntBP;

	while (nextSignal = (DataSignal*) it ()) {

		bp = nextSignal->GetBioID ();
		IntBP = (int) floor (bp + 0.5);
/*		alleleName = ReconstructAlleleName (IntBP, isOffGrid);
		nextSignal->SetAlleleName (alleleName);*/

//		ExcelText << doubleIndent << "Signal at " << bp << " base pairs" << endLine;
//		ExcelText.SetOutputLevel (OutputLevelManager::LocusSampleCharacteristics);

		ExcelText << doubleIndent << "Allele Name = " << nextSignal->GetAlleleName ();
		ExcelText << "   (bp = " << bp << ", disp = " << bp - IntBP << ")" << endLine;

		if (nextSignal->IsOffGrid ())
			ExcelText << doubleIndent << mLink->GetOffGridMessage ();

//		if (!nextSignal->ReportNotices (ExcelText, tripleIndent, "\t")) {

//			ExcelText.SetOutputLevel (OutputLevelManager::LocusSampleCharacteristics);
			nextSignal->ReportAbbreviated (ExcelText, tripleIndent);
//		}

		ExcelText << endLine;
	}

	ExcelText << endLine;
	ExcelText.ResetOutputLevel ();
}


RGString Locus :: BuildSampleString (const RGString& separator) {

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	RGString returnString;

	while (nextSignal = (DataSignal*) it ())
		nextSignal->BuildAlleleString (returnString, ",");

	SampleString = returnString;
	return returnString;
}


//int Locus :: CallAlleles (RGDList& completeList, bool isNegCntl) {
//
//	RGDListIterator it (LocusSignalList);
//	DataSignal* nextSignal;
//	DataSignal* prevSignal = NULL;
//	double bp;
//	int IntBP;
//	RGString alleleName;
//	RGString prevAlleleName;
//	Boolean isOffGrid;
//	Notice* newNotice;
////	CompoundSignalInfo* compound;
//	RGString msg;
//	bool prevSignalWasCrater = false;
//
//	while (nextSignal = (DataSignal*) it ()) {
//
//		bp = nextSignal->GetBioID ();
//		IntBP = (int) floor (bp + 0.5);
//		alleleName = ReconstructAlleleName (IntBP, isOffGrid);
//		nextSignal->SetBioIDResidual (bp - (double)IntBP);
//
//		if (isOffGrid)
//			nextSignal->SetOffGrid (true);
//
//		nextSignal->SetAlleleName (alleleName);
//
//		if (nextSignal->IsPartOfCompoundSignal ()) {
//
//			prevSignal = NULL;
//
//			if (prevSignalWasCrater)
//				prevSignalWasCrater = false;
//
//			else {
//
//				newNotice = new Crater;
//				msg = " at allele " + alleleName;
//				newNotice->AddDataItem (msg);
//				AddNoticeToList (newNotice);
//				prevSignalWasCrater = true;
//			}
//
//		}
//
//		if ((prevSignal != NULL) && (!alleleName.IsEmpty ()) && (prevAlleleName == alleleName)) {
// 
//			//compound = new CompoundSignalInfo (prevSignal, nextSignal);
//			//compound->alleleName = alleleName;
//			//prevSignal->SetSignalLink (compound);
//			//nextSignal->SetSignalLink (compound);
//			//mSignalLinkList.push_back (compound);
//
//			newNotice = new Crater;
//			msg = " at allele " + alleleName;
//			newNotice->AddDataItem (msg);
//			AddNoticeToList (newNotice);
//
//			newNotice = new Crater;
//			prevSignal->AddNoticeToList (newNotice);
//
//			newNotice = new Crater;
//			nextSignal->AddNoticeToList (newNotice);
//
//			prevSignal = NULL;
//			prevAlleleName = "";
//		}
//
//		else {
//
//			prevSignal = nextSignal;
//			prevAlleleName = alleleName;
//		}
//	}
//
////	MergeAndSaveListAWithListB (LocusSignalList, completeList);
//	return 0;
//}
//
//
//int Locus :: CallAlleles (RGDList& completeList, bool isNegCntl, GenotypesForAMarkerSet* pGenotypes, RGDList& artifacts, RGDList& signalList) {
//
//	RGDListIterator it (LocusSignalList);
//	DataSignal* nextSignal;
//	DataSignal* prevSignal = NULL;
//	double bp;
//	int IntBP;
//	RGString alleleName;
//	RGString prevAlleleName;
//	Boolean isOffGrid;
//	Notice* newNotice;
////	CompoundSignalInfo* compound;
//	CompoundSignalInfo* csi;
//	RGString msg;
//	bool prevSignalWasCrater = false;
//	RGString lName = GetLocusName ();
//	double residual;
//	bool foundOLAllele = false;
//	DataSignal* craterSignal;
//	RGDList tempList;
//	int prevIntBP;
//	double prevResidual;
//	double prevBP;
//	Notice* foundNotice;
//	CraterInducedByPullUp pullupNotice;
//	PullUpPrimaryChannel primaryPullupNotice;
//	PullUpFound priorPullupFound;
//	bool diffGreaterThanOne;
//	bool diffEqualOne;
//	bool diffEqualZero;
//	bool residualGreaterThanAveDist;
//	bool prevResidualGreaterThanAveDist;
//	int bpDiff;
//	double AveBP;
//	int intAveBP;
//	bool boolIsOffGrid;
//
//	while (nextSignal = (DataSignal*) it ()) {
//
//		bp = nextSignal->GetBioID ();
//		IntBP = (int) floor (bp + 0.5);
//		alleleName = ReconstructAlleleName (IntBP, isOffGrid);
//		residual = bp - (double)IntBP;
//		nextSignal->SetBioIDResidual (residual);
//
//		if (isOffGrid) {
//
//			if (!pGenotypes->ContainsOffLadderAllele (lName, alleleName))
//				nextSignal->SetOffGrid (true);
//
//			if (mIsAMEL) {
//
//				it.RemoveCurrentItem ();
//				artifacts.InsertWithNoReferenceDuplication (nextSignal);
//				newNotice = new OLPeakInAMEL;
//				nextSignal->AddNoticeToList (newNotice);
//				alleleName = "";
//
//				if (!foundOLAllele) {
//
//					newNotice = new ExtraneousPeakInAMEL;
//					AddNoticeToList (newNotice);
//				}
//			}
//
//			foundOLAllele = true;
//		}
//
//		nextSignal->SetAlleleName (alleleName);
//
//		if (nextSignal->IsPartOfCompoundSignal ()) {
//
///*			prevSignal = NULL;
//
//			if (prevSignalWasCrater)
//				prevSignalWasCrater = false;
//
//			else {
//
//				newNotice = new Crater;
//				msg = " at allele " + alleleName;
//				newNotice->AddDataItem (msg);
//				AddNoticeToList (newNotice);
//				prevSignalWasCrater = true;
//				mNumberOfCraters++;
//			}*/
//
//			if (prevSignal != NULL) {
//
//				if (prevSignalWasCrater) {
//
//					// figure out if this is really a crater (from pullup)
//
//					prevSignalWasCrater = false;
//
//
//					/*bool diffGreaterThanOne;
//					bool diffEqualOne;
//					bool diffEqualZero;
//					bool residualGreaterThanAveDist;
//					bool prevResidualGreaterThanAveDist;*/
//
//					bpDiff = IntBP - prevIntBP;
//					diffGreaterThanOne = (bpDiff >= 2);
//					diffEqualZero = (bpDiff == 0);
//					diffEqualOne = (bpDiff == 1);
//
//					AveBP = 0.5 * (bp + prevBP);
//					residualGreaterThanAveDist = (fabs (residual) > fabs (bp - AveBP));
//					prevResidualGreaterThanAveDist = (fabs (prevResidual) > fabs (prevBP - AveBP));
//
//					if (diffEqualZero || (diffEqualOne && (residualGreaterThanAveDist || prevResidualGreaterThanAveDist))) {
//
//						// This is a crater.  Move peaks to artifact list and add crater signal with appropriate notice.  End
//						// with prevSignal = NULL;  First, recalculate allele name
//
//						craterSignal = new CraterSignal (prevSignal, nextSignal);
//						newNotice = new CraterInducedByPullUp;
//						craterSignal->AddNoticeToList (newNotice);
//
//						completeList.Insert (craterSignal);
//						artifacts.Insert (craterSignal);
//
//						intAveBP = (int) floor (AveBP + 0.5);
//						craterSignal->SetBioID (AveBP);
//
//						if (intAveBP != IntBP) {
//
//							// Take allele name from "prev"
//
//							alleleName = prevSignal->GetAlleleName ();
//							boolIsOffGrid = prevSignal->IsOffGrid ();
//						}
//
//						else
//							boolIsOffGrid = nextSignal->IsOffGrid ();
//
//						craterSignal->SetAlleleName ("");
//						craterSignal->SetVirtualAlleleName (alleleName);
//						craterSignal->SetBioIDResidual (AveBP - (double)intAveBP);
//						craterSignal->SetOffGrid (boolIsOffGrid);
//
////						if (!(mIsAMEL && isOffGrid)) {
////
////							signalList.Insert (craterSignal);
////							LocusSignalList.Insert (craterSignal);
////						}
//
//						newNotice = new CraterInducedByPullUp;
//						msg = " (midpoint equivalent to allele " + alleleName;
//
//						if (isOffGrid)
//							msg += "OL)";
//
//						else
//							msg += ")";
//
//						newNotice->AddDataItem (msg);
//						AddNoticeToList (newNotice);
//
//						if (mIsAMEL && isOffGrid) {
//
//							newNotice = new OLPeakInAMEL;
//							craterSignal->AddNoticeToList (newNotice);
//							alleleName = "";
//							craterSignal->SetAlleleName (alleleName);
//						}
//
//						artifacts.InsertWithNoReferenceDuplication (prevSignal);
//						artifacts.InsertWithNoReferenceDuplication (nextSignal);
//
//						signalList.RemoveReference (prevSignal);
//						signalList.RemoveReference (nextSignal);
//
//						tempList.Append (prevSignal);
//						tempList.Append (nextSignal);
//
//						prevSignal = NULL;
//						prevAlleleName = "";
//
//						mNumberOfCraters++;
//					}
//
//					else {
//
//						// This is not a crater.  End with prevSignal = NULL;
//
//						csi = nextSignal->RemoveSignalLink ();
//
//						if (csi != NULL) {
//
//							foundNotice = nextSignal->RemoveNotice (&pullupNotice);
//							nextSignal->RemoveCrossChannelSignalLink (&primaryPullupNotice, &pullupNotice, NULL, NULL);
//							delete foundNotice;
//						}
//
//						csi = prevSignal->RemoveSignalLink ();
//
//						if (csi != NULL) {
//
//							foundNotice = prevSignal->RemoveNotice (&pullupNotice);
//							nextSignal->RemoveCrossChannelSignalLink (&primaryPullupNotice, &pullupNotice, NULL, NULL);
//							delete foundNotice;
//						}
//
//						prevSignal = NULL;
//						prevAlleleName = "";
//					}
//				}
//
//				else {
//
//					prevSignalWasCrater = true;
//					prevSignal = nextSignal;
//					prevAlleleName = alleleName;
//					prevIntBP = IntBP;
//					prevResidual = residual;
//					prevBP = bp;
//					continue;
//				}
//			}
//
//			else {
//
//				prevSignalWasCrater = true;
//				prevSignal = nextSignal;
//				prevAlleleName = alleleName;
//				prevIntBP = IntBP;
//				prevResidual = residual;
//				prevBP = bp;
//				continue;
//			}
//
//		}
//
//		Boolean previousPullUp;
//		Boolean nextPullUp;
//		Boolean onePullUp;
//
//		if ((prevSignal != NULL) && (!alleleName.IsEmpty ()) && (prevAlleleName == alleleName)) {
//
//			previousPullUp = prevSignal->IsNoticeInList (&priorPullupFound);
//			nextPullUp = nextSignal->IsNoticeInList (&priorPullupFound);
//			onePullUp = (previousPullUp && !nextPullUp) || (nextPullUp && !previousPullUp);
//
//			if (onePullUp) {
//
//				if (previousPullUp) {
//
//					artifacts.InsertWithNoReferenceDuplication (prevSignal);
//					signalList.RemoveReference (prevSignal);
//					tempList.Append (prevSignal);
//					prevSignal->SetVirtualAlleleName (prevSignal->GetAlleleName ());
//					prevSignal->SetAlleleName ("");
//					prevSignal = NULL;
//					prevAlleleName = "";					
//				}
//
//				else {
//
//					artifacts.InsertWithNoReferenceDuplication (nextSignal);
//					signalList.RemoveReference (nextSignal);
//					it.RemoveCurrentItem ();
//					prevSignal = NULL;
//					prevAlleleName = "";
//					nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
//					nextSignal->SetAlleleName ("");
//				}
//			}
//
//			else {
// 
//				//compound = new CompoundSignalInfo (prevSignal, nextSignal);
//				//compound->alleleName = alleleName;
//				//prevSignal->SetSignalLink (compound);
//				//nextSignal->SetSignalLink (compound);
//				//mSignalLinkList.push_back (compound);
//
//				craterSignal = new CraterSignal (prevSignal, nextSignal);
//				newNotice = new Crater;
//				craterSignal->AddNoticeToList (newNotice);
//
//				completeList.Insert (craterSignal);
//				artifacts.Insert (craterSignal);
//	//			signalList.Insert (craterSignal);
//	//			LocusSignalList.Insert (craterSignal);
//				craterSignal->SetAlleleName ("");
//				craterSignal->SetVirtualAlleleName (alleleName);
//
//				newNotice = new Crater;
//				msg = " (midpoint equivalent to allele " + alleleName;
//
//				if (isOffGrid)
//					msg += "OL)";
//
//				else
//					msg += ")";
//
//				newNotice->AddDataItem (msg);
//				AddNoticeToList (newNotice);
//
//				newNotice = new Crater;
//				prevSignal->AddNoticeToList (newNotice);
//
//				newNotice = new Crater;
//				nextSignal->AddNoticeToList (newNotice);
//
//				artifacts.InsertWithNoReferenceDuplication (prevSignal);
//				artifacts.InsertWithNoReferenceDuplication (nextSignal);
//
//				signalList.RemoveReference (prevSignal);
//				signalList.RemoveReference (nextSignal);
//
//				tempList.Append (prevSignal);
//				it.RemoveCurrentItem ();
//
//				prevSignal = NULL;
//				prevAlleleName = "";
//
//				mNumberOfCraters++;
//			}
//		}
//
//		else {
//
//			prevSignal = nextSignal;
//			prevAlleleName = alleleName;
//		}
//	}
//
//	while (nextSignal = (DataSignal*) tempList.GetFirst ())
//		LocusSignalList.RemoveReference (nextSignal);
//
//	return 0;
//}


bool Locus :: ComputeExtendedLocusTimes (CoreBioComponent* grid, CoordinateTransform* inverseTransform, int channelNumber, CoreBioComponent* associatedGrid) {

	Locus* locusGrid = grid->FindLocus (channelNumber, GetLocusName ());
	double time;
	bool status = true;
	time = locusGrid->GetTimeFromBP ((double)mLink->GetMinimumBound ());

	if (time > 0.0)
		mMinExtendedLocusTime = inverseTransform->EvaluateWithExtrapolation (time);

	else
		status = false;

	time = locusGrid->GetTimeFromBP ((double)mLink->GetMaximumBound ());

	if (time > 0.0)
		mMaxExtendedLocusTime = inverseTransform->EvaluateWithExtrapolation (time);

	else
		status = false;

	Locus* associatedLocusGrid = associatedGrid->FindLocus (channelNumber, GetLocusName ());
	associatedLocusGrid->SetMinExtendedLocusTime (mMinExtendedLocusTime);
	associatedLocusGrid->SetMaxExtendedLocusTime (mMaxExtendedLocusTime);

	return status;
}


int Locus :: CallAlleles (bool isNegCntl, GenotypesForAMarkerSet* pGenotypes, RGDList& artifacts, RGDList& signalList, RGDList& completeList) {

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	DataSignal* prevSignal = NULL;
	DataSignal* followingSignal = NULL;
	double bp;
	int IntBP;
	RGString alleleName;
	RGString prevAlleleName;
	Boolean isOffGrid;
	RGString msg;
	bool prevSignalWasCrater = false;
	RGString lName = GetLocusName ();
	double residual;
	bool foundOLAllele = false;
	RGDList SignalsToDeleteFromAll;
	RGDList SignalsToDeleteFromLocus;
	int location;
	bool testForNegative = false;
	bool locationNonZero;
	RGDList tempList;

	while (nextSignal = (DataSignal*) it ()) {

		prevSignal = nextSignal->GetPreviousLinkedSignal ();

		if (prevSignal != NULL)
			tempList.Append (prevSignal);

		tempList.Append (nextSignal);

		followingSignal = nextSignal->GetNextLinkedSignal ();

		if (followingSignal != NULL)
			tempList.Append (followingSignal);
	}

	while (nextSignal = (DataSignal*) tempList.GetFirst ()) {

		location = TestSignalPositionRelativeToLocus (nextSignal);

		if (location != 0)
			locationNonZero = true;

		bp = nextSignal->GetBioID (-location);
		IntBP = (int) floor (bp + 0.5);
		alleleName = ReconstructAlleleName (IntBP, isOffGrid);
		residual = bp - (double)IntBP;
		nextSignal->SetBioIDResidual (residual, -location);
		nextSignal->SetAlleleName (alleleName, -location);

		if (location == 0)
			nextSignal->SetVirtualAlleleName (alleleName);

		if (alleleName.GetCharacter (0) == '-')
			testForNegative = true;

		if (isOffGrid) {

			if (pGenotypes->ContainsOffLadderAllele (lName, nextSignal->GetAlleleName (-location))) {

				nextSignal->SetOffGrid (-location, false);
			}

			else
				nextSignal->SetOffGrid (-location, true);

			if (mIsAMEL) {

				alleleName = "";
				nextSignal->SetAlleleName (alleleName, -location);
			}
		}

		else
			nextSignal->SetOffGrid (-location, false);
	}

	return 0;
}


int Locus :: TestProximityArtifacts (RGDList& artifacts, RGDList& type1List, RGDList& type2List) {

	if (mIsAMEL)
		return 0;
	
	RGDListIterator it (LocusSignalList);
	RGDListIterator it2 (LocusSignalList);
	DataSignal* nextSignal;
	DataSignal* testSignal;
	double stutterPeakTotal;
	double adenylationPeakTotal;
	int nextBP;
	int testBP;
	int diff;
	int repeatNumber = mLink->GetCoreNumber ();
	double peak;
	bool hasStutter;
	bool hasAdenylation;
	bool removeSignal;
	Notice* newNotice;
	int location;
	bool removedExtraneousNotices;

	double stutterLimit = Locus::GetSampleStutterThreshold ();
	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();

	while (nextSignal = (DataSignal*) it ()) {

		it2.Reset ();
		stutterPeakTotal = adenylationPeakTotal = 0.0;
		nextBP = (int) floor (nextSignal->GetBioID () + 0.5);
		hasStutter = hasAdenylation = removeSignal = false;

		while (testSignal = (DataSignal*) it2 ()) {

			testBP = (int) floor (testSignal->GetBioID () + 0.5);

			if (testBP > nextBP + repeatNumber)
				break;

			diff = abs (nextBP - testBP);

			if (diff == 1) {

				hasAdenylation = true;
				adenylationPeakTotal += testSignal->Peak ();
			}
			
			else if (diff == repeatNumber) {

				hasStutter = true;
				stutterPeakTotal += testSignal->Peak ();
			}
		}

		if (hasAdenylation || hasStutter) {

			peak = nextSignal->Peak ();
			location = TestSignalPositionRelativeToLocus (nextSignal);
			removedExtraneousNotices = false;

			if (hasAdenylation) {

				if (peak < adenylationLimit * adenylationPeakTotal) {

					if (location == 0) {

						newNotice = new AdenylationFound;
						nextSignal->AddNoticeToList (newNotice);
						Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
						removedExtraneousNotices = true;
					}

					else
						type1List.InsertWithNoReferenceDuplication (nextSignal);

					removeSignal = true;
				}
			}

			if (hasStutter) {

				if (peak < stutterLimit * stutterPeakTotal) {

					if (location == 0) {

						newNotice = new StutterFound;
						nextSignal->AddNoticeToList (newNotice);
						
						if (!removedExtraneousNotices)
							Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
					}

					else
						type2List.InsertWithNoReferenceDuplication (nextSignal);

					removeSignal = true;
				}
			}

			if (removeSignal) {

				it.RemoveCurrentItem ();

				if (location == 0) {

					nextSignal->SetAlleleName ("");
					artifacts.InsertWithNoReferenceDuplication (nextSignal);
				}

				else if (location < 0)
					mExtendedLeft.RemoveReference (nextSignal);

				else
					mExtendedRight.RemoveReference (nextSignal);
			}
		}
	}

	return 0;
}


bool Locus :: TestForProximityArtifact (DataSignal* target, RGDList& externalLeft, RGDList& externalRight, double minTLeft, double maxTLeft, double minTRight, double maxTRight, double threshold) {

	//
	// This is only used for adenylation and thus, there is no accumulation and no right adenylation
	//

	double mean = target->GetMean ();
	double peak = target->Peak ();
	RGDListIterator itL (externalLeft);
	RGDListIterator itR (externalRight);
	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	double cumulativePeak = 0.0;
	double deltaTime;

	while (nextSignal = (DataSignal*) itL ()) {

		deltaTime = nextSignal->GetMean () - mean;

		if (deltaTime <= 0.0)
			continue;

		if ((minTLeft < deltaTime) && (deltaTime < maxTLeft))
			cumulativePeak += nextSignal->Peak ();
	}

	while (nextSignal = (DataSignal*) itR ()) {

		deltaTime = nextSignal->GetMean () - mean;

		if (deltaTime <= 0.0)
			continue;

		if ((minTRight < deltaTime) && (deltaTime < maxTRight))
			cumulativePeak += nextSignal->Peak ();
	}

	double aveMin = 0.5 * (minTLeft + minTRight);
	double aveMax = 0.5 * (maxTLeft + maxTRight);

	while (nextSignal = (DataSignal*) it ()) {

		deltaTime = nextSignal->GetMean () - mean;

		if (deltaTime <= 0.0)
			continue;

		if ((aveMin < deltaTime) && (deltaTime < aveMax))
			cumulativePeak += nextSignal->Peak ();
	}

	if (peak <= threshold * cumulativePeak)
		return true;

	return false;
}


bool Locus :: TestForProximityArtifact (DataSignal* target, RGDList& externalLeft, RGDList& externalRight, double minTLeft, double maxTLeft, double minTRight, double maxTRight, double thresholdLeft, double thresholdRight) {

	double mean = target->GetMean ();
	double peak = target->Peak ();
	RGDListIterator itL (externalLeft);
	RGDListIterator itR (externalRight);
//	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	double cumulativePeak = 0.0;
	double deltaTime;

	while (nextSignal = (DataSignal*) itL ()) {

		deltaTime = fabs (mean - nextSignal->GetMean ());

		if ((minTLeft < deltaTime) && (deltaTime < maxTLeft))
			cumulativePeak += thresholdLeft * nextSignal->Peak ();
	}

	while (nextSignal = (DataSignal*) itR ()) {

		deltaTime = fabs (mean - nextSignal->GetMean ());

		if ((minTRight < deltaTime) && (deltaTime < maxTRight))
			cumulativePeak += thresholdRight * nextSignal->Peak ();
	}

	//double aveMin = 0.5 * (minTLeft + minTRight);
	//double aveMax = 0.5 * (maxTLeft + maxTRight);

	//while (nextSignal = (DataSignal*) it ()) {

	//	deltaTime = fabs (mean - nextSignal->GetMean ());

	//	if ((aveMin < deltaTime) && (deltaTime < aveMax))
	//		cumulativePeak += nextSignal->Peak ();
	//}

	if (peak <= cumulativePeak)
		return true;

	return false;
}


int Locus :: TestForMultiSignals (RGDList& artifacts, RGDList& signalList, RGDList& completeList) {

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	DataSignal* prevSignal = NULL;
	DataSignal* followingSignal = NULL;
	double bp;
	int IntBP;
	RGString alleleName;
	RGString prevAlleleName;
	Notice* newNotice;
	RGString msg;
	bool prevSignalWasCrater = false;
	RGString lName = GetLocusName ();
	double residual;
	bool foundOLAllele = false;
	DataSignal* craterSignal;
	RGDList tempList;
	Notice* foundNotice;
//	CraterInducedByPullUp pullupNotice;
	PullUpPrimaryChannel primaryPullupNotice;
	PullUpFound pullupNotice;
	Crater craterNotice;
	RGDList SignalsToDeleteFromAll;
	RGDList SignalsToDeleteFromLocus;
	InterchannelLinkage* iChannel;
	int location;
//	CompoundSignalInfo* compound;
	bool isAmbiguous;

	while (nextSignal = (DataSignal*) it ()) {

//		if (nextSignal->LeftEndPoint () < -20.0) {
//
////			location = nextSignal->GetID ();
//			it.RemoveCurrentItem ();
//			
//			delete nextSignal;
//			continue;
//		}

		location = TestSignalPositionRelativeToLocus (nextSignal);
		isAmbiguous = (nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1));

		if (nextSignal->TestForMultipleSignals (prevSignal, followingSignal, location)) {

			if (!isAmbiguous) {

				SignalsToDeleteFromAll.Append (nextSignal);

				if (prevSignal != NULL) {

					foundNotice = prevSignal->RemoveNotice (&craterNotice);
					delete foundNotice;

					if (prevSignal->NumberOfNoticeObjects () == 0)
						artifacts.RemoveReference (prevSignal);
				}

				if (followingSignal != NULL) {

					foundNotice = followingSignal->RemoveNotice (&craterNotice);
					delete foundNotice;

					if (followingSignal->NumberOfNoticeObjects () == 0)
						artifacts.RemoveReference (followingSignal);
				}
			}
		}

		else if (!isAmbiguous) {

			if (prevSignal != NULL)
				SignalsToDeleteFromLocus.Append (prevSignal);

			if (followingSignal != NULL)
				SignalsToDeleteFromLocus.Append (followingSignal);
		}
	}

	while (nextSignal = (DataSignal*) SignalsToDeleteFromLocus.GetFirst ()) {

		nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
		nextSignal->SetAlleleName ("");
		RemoveSignalFromLocusList (nextSignal);
	}

	while (nextSignal = (DataSignal*) SignalsToDeleteFromAll.GetFirst ()) {

		RemoveSignalFromLocusList (nextSignal);
		nextSignal->RemoveFromAllLoci ();
		artifacts.RemoveReference (nextSignal);
		signalList.RemoveReference (nextSignal);
		completeList.RemoveReference (nextSignal);
		iChannel = nextSignal->GetInterchannelLink ();

		if (iChannel != NULL) {

			iChannel->RemoveDataSignal (nextSignal, &primaryPullupNotice, NULL, &pullupNotice, NULL);

			// Later, consider adding one of nextSignal's flanking peaks...need iChannel function to perform test, assuming
			// that primary Signal may be gone.  Then recalculate primary signal

			if (iChannel->IsEmpty ())
				iChannel->RemoveAll (&primaryPullupNotice, NULL, &pullupNotice, NULL);

			else
				iChannel->RecalculatePrimarySignal (&pullupNotice, &primaryPullupNotice);
		}

		delete nextSignal;
	}

	it.Reset ();
	Boolean previousPullUp;
	Boolean nextPullUp;
	Boolean onePullUp;
	prevSignal = NULL;
	prevAlleleName = "";

	while (nextSignal = (DataSignal*) it ()) {

		// test for consecutive signals with same call:  make it a crater (This should be done only for peaks that
		// are unique to this locus

		location = TestSignalPositionRelativeToLocus (nextSignal);
		alleleName = nextSignal->GetAlleleName (location);

		if ((prevSignal != NULL) && (!alleleName.IsEmpty ()) && (prevAlleleName == alleleName)) {

			previousPullUp = prevSignal->IsNoticeInList (&pullupNotice);
			nextPullUp = nextSignal->IsNoticeInList (&pullupNotice);
			onePullUp = (previousPullUp && !nextPullUp) || (nextPullUp && !previousPullUp);

			if (onePullUp) {

				if (previousPullUp) {

					artifacts.InsertWithNoReferenceDuplication (prevSignal);
					signalList.RemoveReference (prevSignal);
					tempList.Append (prevSignal);
					prevSignal->SetVirtualAlleleName (prevSignal->GetAlleleName ());
					prevSignal->SetAlleleName ("");
					prevSignal = nextSignal;   // changed so that we test nextSignal also
					prevAlleleName = alleleName;					
				}

				else {

					artifacts.InsertWithNoReferenceDuplication (nextSignal);
					signalList.RemoveReference (nextSignal);
					it.RemoveCurrentItem ();
//					prevSignal = NULL;  // changed so that nextSignal tested against current prevSignal
//					prevAlleleName = "";
					nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName (location));
					nextSignal->SetAlleleName ("");
				}
			}

			else {
 
				//compound = new CompoundSignalInfo (prevSignal, nextSignal);
				//compound->alleleName = alleleName;
				//prevSignal->SetSignalLink (compound);
				//nextSignal->SetSignalLink (compound);
				//mSignalLinkList.push_back (compound);

				craterSignal = new CraterSignal (prevSignal, nextSignal);
				newNotice = new Crater;
				craterSignal->AddNoticeToList (newNotice);

				completeList.Insert (craterSignal);
				artifacts.Insert (craterSignal);
//				signalList.Insert (craterSignal);   // what happens if we leave this on signalList and LocusSignalList????  Ans:  nothing good
				LocusSignalList.Insert (craterSignal);  // see above???
				craterSignal->SetAlleleName (alleleName);
				craterSignal->SetVirtualAlleleName (alleleName);
				bp = craterSignal->GetBioID (location);
				IntBP = (int) floor (bp + 0.5);
				residual = bp - (double)IntBP;
				craterSignal->SetBioIDResidual (residual);

				newNotice = new Crater;
				msg = " (midpoint equivalent to allele " + alleleName;

				if (prevSignal->IsOffGrid ()) {

					msg += "OL)";
					craterSignal->SetOffGrid (true);
				}

				else {

					msg += ")";
					craterSignal->SetOffGrid (false);
				}

				newNotice->AddDataItem (msg);
				AddNoticeToList (newNotice);

				newNotice = new Crater;
				prevSignal->AddNoticeToList (newNotice);

				newNotice = new Crater;
				nextSignal->AddNoticeToList (newNotice);

				prevSignal->SetVirtualAlleleName (prevSignal->GetAlleleName ());
				prevSignal->SetAlleleName ("");

				nextSignal->SetVirtualAlleleName (prevSignal->GetAlleleName ());
				nextSignal->SetAlleleName ("");

				artifacts.InsertWithNoReferenceDuplication (prevSignal);
				artifacts.InsertWithNoReferenceDuplication (nextSignal);

				signalList.RemoveReference (prevSignal);
				signalList.RemoveReference (nextSignal);

				tempList.Append (prevSignal);
				it.RemoveCurrentItem ();

				prevSignal = NULL;
				prevAlleleName = "";

				mNumberOfCraters++;
			}
		}

		else {

			prevSignal = nextSignal;
			prevAlleleName = alleleName;
		}
	}

	while (nextSignal = (DataSignal*) tempList.GetFirst ())
		LocusSignalList.RemoveReference (nextSignal);

//	TestProximityArtifacts (artifacts);
	return 0;
}


int Locus :: AddNoticeToList (Notice* newNotice) {

	if (NewNoticeList.Entries () == 0) {

		mHighestSeverityLevel = newNotice->GetSeverityLevel ();
		mHighestMessageLevel = newNotice->GetMessageLevel ();
		NewNoticeList.Append (newNotice);
	}

	else {

		int temp = newNotice->GetSeverityLevel ();

		if (mHighestSeverityLevel >= temp) {

			mHighestSeverityLevel = temp;
			NewNoticeList.Prepend (newNotice);
		}

		else {

			RGDListIterator it (NewNoticeList);
			Notice* nextNotice;
			bool noticeInserted = false;

			while (nextNotice = (Notice*) it ()) {

				if (temp < nextNotice->GetSeverityLevel ()) {

					--it;
					it.InsertAfterCurrentItem (newNotice);
					noticeInserted = true;
					break;
				}
			}

			if (!noticeInserted)
				NewNoticeList.Append (newNotice);
		}

		temp = newNotice->GetMessageLevel ();

		if (mHighestMessageLevel > temp)
			mHighestMessageLevel = temp;
	}

	return NewNoticeList.Entries ();
}



Boolean Locus :: IsNoticeInList (const Notice* target) {

	if (NewNoticeList.Find (target))
		return TRUE;

	return FALSE;
}


Notice* Locus :: GetNotice (const Notice* target) {

	return (Notice*) NewNoticeList.Find (target);
}



Boolean Locus :: ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink) {

	if (NewNoticeList.Entries () > 0) {

		RGDListIterator it (NewNoticeList);
		Notice* nextNotice;
		text.SetOutputLevel (mHighestMessageLevel);
		RGString indent2 = indent + "\t";

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			return FALSE;
		}

		Endl endLine;
		text << endLine;

		if (reportLink)
			text << mTableLink;

		text << indent;
		text << GetLocusName () << " Notices:  " << endLine;
		text.ResetOutputLevel ();

		while (nextNotice = (Notice*) it ())
			nextNotice->Report (text, indent2, delim);

		if (reportLink) {

			text.SetOutputLevel (mHighestMessageLevel);
			text << mTableLink;
			text.ResetOutputLevel ();
		}

		text.Write (1, "\n");
	}

	else
		return FALSE;

	return TRUE;
}


Boolean Locus :: ReportXMLNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	if (NewNoticeList.Entries () > 0) {

		RGDListIterator it (NewNoticeList);
		Notice* nextNotice;
		text.SetOutputLevel (mHighestMessageLevel);
		tempText.SetOutputLevel (1);
		int msgNum;
		int triggerLevel = Notice::GetMessageTrigger ();

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			tempText.ResetOutputLevel ();
			return FALSE;
		}

		text << CLevel (1) << "\t\t\t\t<LocusAlerts>\n" << PLevel ();

		while (nextNotice = (Notice*) it ()) {

			if (nextNotice->GetMessageLevel () <= triggerLevel) {

				msgNum = Notice::GetNextMessageNumber ();
				nextNotice->SetMessageNumber (msgNum);
				text << "\t\t\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
				tempText << "\t\t<Message>\n";
				tempText << "\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
				tempText << "\t\t\t<Index>" << nextNotice->GetID () << "</Index>\n";
				tempText << "\t\t\t<Text>" << (nextNotice->AssembleString (delim)).GetData () << "</Text>\n";
				tempText << "\t\t</Message>\n";
			}
		}

		text << CLevel (1) << "\t\t\t\t</LocusAlerts>\n" << PLevel (); 
		text.ResetOutputLevel ();
		tempText.ResetOutputLevel ();
	}

	else
		return FALSE;

	return TRUE;
}


void Locus :: ReportXMLGridTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	text << CLevel (1) << "\t\t\t<Locus>\n";
	text << "\t\t\t\t<LocusName>" << GetLocusName ().GetData () << "</LocusName>\n" << PLevel ();
	ReportXMLNoticeObjects (text, tempText, delim);
	text << CLevel (1) << "\t\t\t</Locus>\n" << PLevel ();
}



void Locus :: ReportXMLSampleTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	text << CLevel (1) << "\t\t\t<Locus>\n";
	text << "\t\t\t\t<LocusName>" << GetLocusName ().GetData () << "</LocusName>\n" << PLevel ();

	if ((mHighestMessageLevel > 0) && (mHighestMessageLevel <= Notice::GetMessageTrigger ()))
		ReportXMLNoticeObjects (text, tempText, delim);

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	double bp;
	int IntBP;
	text.SetOutputLevel (1);
	bool isHomozygote = false;

	if (LocusSignalList.Entries () == 1)
		isHomozygote = true;

	while (nextSignal = (DataSignal*) it ()) {

		bp = nextSignal->GetBioID ();
		IntBP = (int) floor (bp + 0.5);
		text << "\t\t\t\t<Allele>\n";
		text << "\t\t\t\t\t<Id>" << nextSignal->GetSignalID () << "</Id>\n";
		text << "\t\t\t\t\t<Name>" << nextSignal->GetAlleleName () << "</Name>\n";
		text << "\t\t\t\t\t<BPS>" << bp << "</BPS>\n";
		text << "\t\t\t\t\t<RFU>" << (int) floor (nextSignal->Peak () + 0.5) << "</RFU>\n";
		text << "\t\t\t\t\t<meanbps>" << nextSignal->GetApproximateBioID () << "</meanbps>\n";
		text << "\t\t\t\t\t<PeakArea>" << nextSignal->TheoreticalArea () << "</PeakArea>\n";
		text << "\t\t\t\t\t<Time>" << nextSignal->GetMean () << "</Time>\n";
		text << "\t\t\t\t\t<Fit>" << nextSignal->GetCurveFit () << "</Fit>\n";

		if (nextSignal->IsOffGrid ())
			text << "\t\t\t\t\t<OffLadder>true</OffLadder>\n";

		else if (nextSignal->IsAcceptedOffGrid ())
			text << "\t\t\t\t\t<OffLadder>accepted</OffLadder>\n";

		else
			text << "\t\t\t\t\t<OffLadder>false</OffLadder>\n";

		if (isHomozygote) {

			text << "\t\t\t\t\t<Count>2</Count>\n";
		}

		text << "\t\t\t\t</Allele>\n";
	}

	text << CLevel (1) << "\t\t\t</Locus>\n" << PLevel ();
}



void Locus :: ClearNoticeObjects () {

	NewNoticeList.ClearAndDelete ();
}



int Locus :: NumberOfNoticeObjects () {

	return NewNoticeList.Entries ();
}


Notice* Locus :: RemoveNotice (const Notice* target) {

	Notice* returnValue = (Notice*) NewNoticeList.Remove (target);

	RGDListIterator it (NewNoticeList);
	Notice* nextNotice;
	int level = 0;

	while (nextNotice = (Notice*) it ()) {

		if (level == 0)
			level = nextNotice->GetMessageLevel ();

		else if (nextNotice->GetMessageLevel () < level)
			level = nextNotice->GetMessageLevel ();
	}

	mHighestSeverityLevel = mHighestMessageLevel = level;
	return returnValue;
}


void Locus :: RetrieveNoticesFromGridArtifactList (ChannelData* laneStandard) {

	RGDListIterator it (mGridArtifactList);
	Notice* nextNotice;

	Notice* newNotice;
	DataSignal* nextSignal;
	RGString info;
	double bp;
	int ibp;
	CoordinateTransform* transform = laneStandard->GetIDMap ();
//	double mean;
	RGString localAlleleName;

	while (nextSignal = (DataSignal*) it ()) {

//		mean = nextSignal->GetMean ();
		nextSignal->ResetNotices ();

		while (nextNotice = nextSignal->GetNextNoticeObject ()) {

			newNotice = (Notice*)nextNotice->Copy ();
			info = "at location ~";
			bp = nextSignal->GetApproximateBioID ();
			ibp = (int) floor (bp + 0.5);
			info << ibp << " bps";
			newNotice->AddDataItem (info);
			AddNoticeToList (newNotice);
		}
	}

	RGDListIterator FinalIterator (FinalSignalList);
	bool criticalArtifactFound = false;
	int criticalLevel = Notice::GetMessageTrigger ();
	int nextSignalLevel;
	info = "";

	while (nextSignal = (DataSignal*) FinalIterator ()) {

		nextSignalLevel = nextSignal->GetHighestMessageLevel ();

		if ((nextSignalLevel > 0) && (nextSignalLevel <= criticalLevel)) {

			if (!criticalArtifactFound) {

				localAlleleName = nextSignal->GetAlleleName ();

				if (localAlleleName.Length () > 0) {

					criticalArtifactFound = true;
					newNotice = new LocusHasAllelesWithCriticalNotices;
				}
			}

			else
				info << ", ";

			if (mIsAMEL) {

				localAlleleName = nextSignal->GetAlleleName ();

				if (localAlleleName == "1")
					info << "X";

				else if (localAlleleName == "2")
					info << "Y";

				else
					info << localAlleleName;
			}
			
			else
				info << nextSignal->GetAlleleName ();
		}
	}

	if (criticalArtifactFound) {

		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);
	}
}


int Locus :: LocationOfSignal (DataSignal* signal) {

	if (mExtendedLeft.ContainsReference (signal))
		return -1;

	if (mExtendedRight.ContainsReference (signal))
		return 1;

	return 0;
}


double Locus :: GetLocusSpecificSampleStutterThreshold () const {

	if (mLink != NULL)
		return mLink->GetSampleStutterThreshold ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificSamplePlusStutterThreshold () const {

	if (mLink != NULL)
		return mLink->GetSamplePlusStutterThreshold ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificSampleAdenylationThreshold () const {

	if (mLink != NULL)
		return mLink->GetSampleAdenylationThreshold ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificSampleFractionalFilter () const {

	if (mLink != NULL)
		return mLink->GetSampleFractionalFilter ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificSamplePullupFractionalFilter () const {

	if (mLink != NULL)
		return mLink->GetSamplePullupFractionalFilter ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificSampleHeterozygousImbalanceThreshold () const {

	if (mLink != NULL)
		return mLink->GetSampleHeterozygousImbalanceThreshold ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificSampleMinBoundForHomozygote () const {

	if (mLink != NULL)
		return mLink->GetSampleMinBoundForHomozygote ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificLadderStutterThreshold () const {

	if (mLink != NULL)
		return mLink->GetLadderStutterThreshold ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificLadderAdenylationThreshold () const {

	if (mLink != NULL)
		return mLink->GetLadderAdenylationThreshold ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificLadderFractionalFilter () const {

	if (mLink != NULL)
		return mLink->GetLadderFractionalFilter ();

	else
		return -1.0;
}


double Locus :: GetLocusSpecificLadderPullupFractionalFilter () const {

	if (mLink != NULL)
		return mLink->GetLadderPullupFractionalFilter ();

	else
		return -1.0;
}



int Locus :: CompareTo (const RGPersistent* p) const {

	Locus* q = (Locus*) p;
	return mLink->CompareTo (q->mLink);
}


unsigned Locus :: HashNumber (unsigned long Base) const {

	return mLink->HashNumber (Base);
}



Boolean Locus :: IsEqualTo (const RGPersistent* p) const {

	Locus* q = (Locus*) p;
	return mLink->IsEqualTo (q->mLink);
}


void Locus :: Write (RGFile& textOutput, const RGString& indent) {

	textOutput << indent.GetData () << "Locus name = " << mLink->GetLocusName ().GetData ();
	RGString temp;
	temp.Convert (mLink->GetChannel (), 10);
	textOutput << ", Channel Number = " << temp.GetData () << "\n";
	temp.Convert (mLink->GetMinimumBound (), 10);
	textOutput << indent.GetData () << "     Min Bound = " << temp.GetData () << " and Max bound = ";
	temp.Convert (mLink->GetMaximumBound (), 10);
	textOutput << temp.GetData () << "\n";
	RGString indent2 = indent + "     ";
	Allele* allele;
	ResetAlleles ();

	while (allele = GetNextAllele ())
		allele->Write (textOutput, indent2);
}


void Locus :: WriteXMLInfo (RGTextOutput& text, const RGString& indent) {

	RGString indent2 = indent + "\t";
	Endl endLine;

/*	Allele* testAllele;
	const DataSignal* testSignal;
	double minTime;
	double maxTime;
	testAllele = (Allele*)AlleleList.First ();
	testSignal = testAllele->GetCurvePointer ();
	minTime = testSignal->GetMean ();
	testAllele = (Allele*)AlleleList.Last ();
	testSignal = testAllele->GetCurvePointer ();
	maxTime = testSignal->GetMean ();*/

	int minExtendedBP = mLink->GetMinimumBound ();
	int maxExtendedBP = mLink->GetMaximumBound ();

	int minSample = (int) floor (MinimumSampleTime);
	int maxSample = (int) floor (MaximumSampleTime + 0.5);
	int minExtendedSample = 0;
	int maxExtendedSample = 0;

	if (minSample < maxSample) {

		text << indent << "<locus>" << endLine;
		text << indent2 << "<channel>" << mLink->GetChannel () << "</channel>" << endLine;
		text << indent2 << "<name>" << mLink->GetLocusName () << "</name>" << endLine;
		text << indent2 << "<start>" << minSample << "</start>" << endLine;
		text << indent2 << "<end>" << maxSample << "</end>" << endLine;

		if ((mMinExtendedLocusTime > 0.0) && (mMaxExtendedLocusTime > mMinExtendedLocusTime)) {

			minExtendedSample = (int) floor (mMinExtendedLocusTime);
			maxExtendedSample = (int) floor (mMaxExtendedLocusTime + 0.5);
			text << indent2 << "<startExtended>" << minExtendedSample << "</startExtended>" << endLine;
			text << indent2 << "<endExtended>" << maxExtendedSample << "</endExtended>" << endLine;
		}

		text << indent << "</locus>" << endLine;
	}
}



bool Locus :: ExclusiveOr (bool arg1, bool arg2) {

	if (arg1)
		return !arg2;

	return arg2;
}


double Locus :: MaximumPeakHeight (RGDList& incomingList) {

	DataSignal* nextSignal;
	double maxPeak = 0.0;
	double currentPeak;
	RGDListIterator it (incomingList);
	PullUpFound pullup;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->IsNoticeInList (&pullup))
			continue;

		currentPeak = nextSignal->Peak ();

		if (currentPeak > maxPeak) {

			maxPeak = currentPeak;
		}
	}

	return maxPeak;
}


int Locus :: DivideListByPeakHeight (RGDList& incomingList, RGDList& newList, double maxPeak, double factor, bool removeLess) {

	// removes all peaks that are less than or equal to factor * maxPeak and puts them in newList
	
	RGDListIterator it (incomingList);
	double cutoffLevel = factor * maxPeak;
	bool decision;
	DataSignal* nextSignal;
	double peak;

	while (nextSignal = (DataSignal*) it ()) {

		peak = nextSignal->Peak ();
		decision = !ExclusiveOr ((peak <= cutoffLevel), removeLess);
		//  OK, here's what this means:  if removeLess is true and Peak <= cutoffLevel is true, then 
		// the exclusive or is false and decision is true.  On the other hand, if removeLess is true and Peak <= cutoffLevel is
		// false, then !decision is true, or decision is false.  If removeLess is false, then decision is true only if Peak > cutoffLevel

		if (decision) {

			it.RemoveCurrentItem ();
			newList.Append (nextSignal);
		}
	}

	return incomingList.Entries ();
}


int Locus :: DivideListByPeakHeight (RGDList& incomingList, RGDList& junkList, RGDList& individualList, double minValue, double maxValue, double factor, int size) {

	if (maxValue < 1.40 * minValue)
		return incomingList.Entries ();
	
	RGDListIterator it (incomingList);
	double cutoffLevel = minValue + factor * (maxValue - minValue);
	DataSignal* nextSignal;
	double peak;
	it.ResetToEnd ();
	nextSignal = (DataSignal*) it.CurrentItem ();
	int CurvesLeft = 0;
	int curveCount = 0;
	int halfIncoming = (int) floor (0.5 * (double)incomingList.Entries () + 0.5);
	int halfSize = (int) floor (0.5 * (double)size + 0.5);

	while (nextSignal) {

		peak = nextSignal->Peak ();

		if (peak < cutoffLevel) {

			--it;
			nextSignal = (DataSignal*) it.CurrentItem ();
			curveCount++;
		}

		else
			break;
	}

	if (curveCount < halfIncoming)
		return incomingList.Entries ();

	if (curveCount < size + halfSize)
		return incomingList.Entries ();

	it.ResetToEnd ();
	nextSignal = (DataSignal*) it.CurrentItem ();

	while (nextSignal) {

		peak = nextSignal->Peak ();
		
		if (peak < cutoffLevel) {

			nextSignal->SetRemainingSignals (CurvesLeft);
			CurvesLeft++;
			--it;
		}

		else {

			it.RemoveCurrentItem ();
			nextSignal->SetRemainingSignals (CurvesLeft);


//			if (CurvesLeft >= size) {

				junkList.Prepend (nextSignal);
				nextSignal->AddNoticeToList (1, "", "Signal to left of cutoff in ILS");
//			}

			/*else
				individualList.Prepend (nextSignal);*/
		}

//		--it;
		nextSignal = (DataSignal*) it.CurrentItem ();
	}

	it.Reset ();
	nextSignal = (DataSignal*) junkList.Last ();

	if (nextSignal == NULL)
		return incomingList.Entries ();

	double junkLimit = nextSignal->GetMean ();
	int testSize = 2 * size;

	while (nextSignal = (DataSignal*) it ()) {

		if (incomingList.Entries () <= testSize)
			break;

		if (nextSignal->GetMean () < junkLimit) {

			it.RemoveCurrentItem ();
			junkList.Insert (nextSignal);
			nextSignal->AddNoticeToList (1, "", "Signal to left of cutoff in ILS");
			
		}
	}

	return incomingList.Entries ();
}


bool Locus :: RemoveExtraneousNoticesFromSignal (DataSignal* ds) {

	//OsirisBelowMinRFUButRawDataOK target1;
	//RawDataBelowMinRFUButOsirisOK target2;
	//CurveFitTooLow target3;
	//Notice* foundNotice;
	bool returnValue = false;

	//foundNotice = ds->RemoveNotice (&target1);

	//if (foundNotice) {

	//	returnValue = true;
	//	delete foundNotice;
	//}

	//foundNotice = ds->RemoveNotice (&target2);

	//if (foundNotice) {

	//	returnValue = true;
	//	delete foundNotice;
	//}

	//foundNotice = ds->RemoveNotice (&target3);

	//if (foundNotice) {

	//	returnValue = true;
	//	delete foundNotice;
	//}

	return returnValue;
}


Boolean Locus :: BuildAlleleLists (const RGString& xmlString) {

	RGString XMLString (xmlString);
	RGBracketStringSearch AlleleToken ("<Allele>", "</Allele>", XMLString);
	RGString AlleleString;
	BaseAllele* baseAllele;
	Allele* allele;
	size_t EndPosition;
	size_t StartPosition = 0;
	ErrorString = "";
	Boolean validity = TRUE;

	while (AlleleToken.FindNextBracketedString (StartPosition, EndPosition, AlleleString)) {

		StartPosition = EndPosition;
		baseAllele = mLink->GetNewAllele (AlleleString);

		if (!baseAllele->IsValid ()) {

			validity = FALSE;
			ErrorString = "Could not parse base allele input.  ";
			cout << "Base allele with name " << baseAllele->GetAlleleName () << " is invalid" << endl;
		}

		allele = new Allele (baseAllele, AlleleString);

		if (!allele->IsValid ()) {

			validity = FALSE;
			ErrorString += "Could not parse allele input";
			cout << ErrorString << "    name:  " << allele->GetAlleleName () << endl;
		}

		BaseAllele::SetSearchByName ();
		AlleleListByName.Insert (allele);
		BaseAllele::SetSearchByCurve ();
		AlleleListByCurve.Insert (allele);
		AlleleList.Insert (allele);
	}

	int N = AlleleList.Entries () - 1;

	if (N < 1) {

		validity = FALSE;
		return FALSE;
	}

	mLink->SetLocusVector (AlleleList);

	if (!mLink->IsValid ())
		validity = FALSE;

	return validity;
}


Boolean Locus :: BuildMappings (RGDList& signalList) {

	Boolean validity = TRUE;
	int N = signalList.Entries ();


	if (N != mLink->GetLocusVectorSize ())
		return FALSE;

	double* TimeVector = new double [N];
	int i = 0;
	RGDListIterator it (signalList);
	DataSignal* signal;

	while (signal = (DataSignal*) it ()) {

		TimeVector [i] = signal->GetMean ();
		i++;
	}

	MaximumSampleTime = TimeVector [N-1];
	MinimumSampleTime = TimeVector [0];

	mSampleAnalysisMap = new CSplineTransform (TimeVector, mLink->GetLocusVector (), N);

	if (!mSampleAnalysisMap->IsValid ()) {
		
		delete[] TimeVector;
		return FALSE;
	}

	mSampleTimeFromBPMap = new CSplineTransform (mLink->GetLocusVector (), TimeVector, N);

	if (!mSampleTimeFromBPMap->IsValid ()) {
		
		delete[] TimeVector;
		return FALSE;
	}

//	const double* locusVector = mLink->GetLocusVector ();
//	double minLocus = locusVector [0] - 0.5;
//	double maxLocus = locusVector [N-1] + 0.5;
//	MaximumSampleTime = mSampleTimeFromBPMap->EvaluateWithExtrapolation (maxLocus);
//	MinimumSampleTime = mSampleTimeFromBPMap->EvaluateWithExtrapolation (minLocus);

	mMinExtendedLocusTime = mSampleTimeFromBPMap->EvaluateWithExtrapolation ((double)mLink->GetMinimumBound ());
	mMaxExtendedLocusTime = mSampleTimeFromBPMap->EvaluateWithExtrapolation ((double)mLink->GetMaximumBound ());
	mMinTimeForRoundedCore = mSampleTimeFromBPMap->EvaluateWithExtrapolation ((double)mLink->GetLocusVector ()[0] - 0.5);
	mMaxTimeForRoundedCore = mSampleTimeFromBPMap->EvaluateWithExtrapolation ((double)mLink->GetLocusVector ()[N-1] + 0.5);

	delete[] TimeVector;
	return TRUE;
}


LaneStandard :: LaneStandard () : RGPersistent (), Linked (FALSE), Valid (FALSE) {

	mLink = new BaseLaneStandard ();
	mLaneStandardTimes = NULL;
	mNumberOfCharacteristics = 0;
}



LaneStandard :: LaneStandard (BaseLaneStandard* link, const RGString& xmlString) : RGPersistent (),
mLink (link), Linked (TRUE) {

	Valid = mLink->IsValid ();
	mNumberOfCharacteristics = GetNumberOfCharacteristics ();

	if (mNumberOfCharacteristics > 1)
		mLaneStandardTimes = new double [mNumberOfCharacteristics];

	else
		mLaneStandardTimes = new double [2];

	if (!Valid)
		Msg += mLink->GetError ();
}



LaneStandard :: LaneStandard (const LaneStandard& std) : RGPersistent () {

	if (std.Linked) {

		mLink = std.mLink;
		Linked = TRUE;
		Valid = std.Valid;
		mNumberOfCharacteristics = GetNumberOfCharacteristics ();
		mLaneStandardTimes = new double [mNumberOfCharacteristics];

		if (!Valid)
			Msg = std.Msg;
	}

	else {

		mLink = new BaseLaneStandard ();
		Linked = FALSE;
		Valid = TRUE;
		mLaneStandardTimes = NULL;
		mNumberOfCharacteristics = 0;
	}
}


LaneStandard :: ~LaneStandard () {

	if (!Linked)
		delete mLink;

	delete mLaneStandardTimes;
}

void LaneStandard :: ClearAndDestroy () {

	delete mLink;
	delete mLaneStandardTimes;
	mLink = NULL;
	mLaneStandardTimes = NULL;
}

RGString LaneStandard :: GetLaneStandardName () const {

	return mLink->GetLaneStandardName ();
}


int LaneStandard :: GetNumberOfCharacteristics () const {

	return mLink->GetNumberOfCharacteristics ();
}


double LaneStandard :: GetMinimumCharacteristic () const {

	return mLink->GetMinimumCharacteristic ();
}


double LaneStandard :: GetMaximumCharacteristic () const {

	return mLink->GetMaximumCharacteristic ();
}


double LaneStandard :: GetLaneStandardTimeForCharacteristicNumber (int i) const {
	
	// Assume the first characteristic is number 1 (not 0)


	if (mLaneStandardTimes == NULL) {

//		Msg << "Internal lane standard named " << mLink->GetLaneStandardName () << " has no assigned time vector\n";
		return -1.0;
	}

	if (i < 1)
		return mLaneStandardTimes [0];

	if (i > mNumberOfCharacteristics)
		return mLaneStandardTimes [mNumberOfCharacteristics-1];

	return mLaneStandardTimes [i-1];
}


void LaneStandard :: SetLaneStandardName (const RGString& name) {

	mLink->SetName (name);
}


int LaneStandard :: SelectBestSubsetOfCharacteristics (RGDList& curveList, double& correlation) {
	
	// returns index of first (from 0) in contiguous list of characteristics
	const double* diffArray;
	int setSize = GetCharacteristicDifferenceArray (diffArray);  // This setSize is the size of the difference array!!!
	int displacement = PopulationMarkerSet::SelectBestSubsetOfCharacteristics (curveList, diffArray, setSize + 1, correlation);

	if (displacement < 0) {

		Msg << "Number of available characteristics less than Internal Lane Standard set size for ILS named " << mLink->GetLaneStandardName () << "\n";
		return -1;
	}

	RGDListIterator it (curveList);
	DataSignal* nextSignal;
  int i;
	for (i=0; i<displacement; i++)
		nextSignal = (DataSignal*)it ();

	for (i=0; i<mNumberOfCharacteristics; i++) {

		nextSignal = (DataSignal*)it ();
		mLaneStandardTimes [i] = nextSignal->GetMean ();
	}

	return displacement;
}


int LaneStandard :: AssignLaneStandardSignals (RGDList& curveList) {

	RGDListIterator it (curveList);
	DataSignal* nextSignal;
	
	for (int i=0; i<mNumberOfCharacteristics; i++) {

		nextSignal = (DataSignal*)it ();

		if (nextSignal == NULL)
			return -1;

		mLaneStandardTimes [i] = nextSignal->GetMean ();
	}

	return 0;
}


int LaneStandard :: GetCharacteristicArray (const double*& array) const {
	
	// returns array size or -1
	int status = mLink->GetCharacteristicArray (array);

//	if (status < 0)
//		Msg << "Characteristic array not available for Internal Lane Standard named " << mLink->GetLaneStandardName () << "\n";

	return status;
}


int LaneStandard :: GetCharacteristicDifferenceArray (const double*& array) const {
	
	// returns array size or -1
	int status = mLink->GetCharacteristicDifferenceArray (array);

//	if (status < 0)
//		Msg << "Characteristic array not available for Internal Lane Standard named " << mLink->GetLaneStandardName () << "\n";

	return status;
}


int LaneStandard :: GetUnnormalizedCharacteristicDifferenceArray (const double*& array) const {
	
	// returns array size or -1
	int status = mLink->GetUnnormalizedCharacteristicDifferenceArray (array);

//	if (status < 0)
//		Msg << "Characteristic array not available for Internal Lane Standard named " << mLink->GetLaneStandardName () << "\n";

	return status;
}


int LaneStandard :: GetLargeCharacteristicArray (const double*& array) const {

	// returns array size or -1
	int status = mLink->GetLargeCharacteristicArray (array);
	return status;
}


int LaneStandard :: GetLargeDifferenceArray (const double*& array) const {

	// returns array size or -1
	int status = mLink->GetLargeDifferenceArray (array);
	return status;
}


double LaneStandard :: GetFirstIntervalFraction () const {

	return mLink->GetFirstIntervalFraction ();
}


double LaneStandard :: GetSmallestIntervalFraction () const {

	return mLink->GetSmallestIntervalFraction ();
}


int LaneStandard :: GetMaxRelativeHeight () const {

	return mLink->GetMaxRelativeHeight ();
}


double LaneStandard :: GetCorrelationAcceptanceThreshold () const {

	return mLink->GetCorrelationAcceptanceThreshold ();
}


double LaneStandard :: GetCorrelationAutoAcceptanceThreshold () const {

	return mLink->GetCorrelationAutoAcceptanceThreshold ();
}


int LaneStandard :: GetNormsSquaredLeft (const double*& array) const {
	
	// returns array size or -1
	int status = mLink->GetNormsLeft (array);
	return status;
}


int LaneStandard :: GetNormsSquaredRight (const double*& array) const {
	
	// returns array size or -1
	int status = mLink->GetNormsRight (array);
	return status;
}



int LaneStandard :: GetRelativeHeights (const int*& array) const {
	
	// returns array size or -1
	const int* array1;
	int status = mLink->GetRelativeSizes (array1);

	if (status > 0) {

		for (int i=0; i<status; i++) {

			if (array1 [i] >= 0) {

				array = array1;
				return status;
			}
		}
	}

	array = NULL;
	return -1;
}


int LaneStandard :: GetOmissionArray (const double*& array) const {

	return mLink->GetOmissionArray (array);
}


int LaneStandard :: CompareTo (const RGPersistent* p) const {

	LaneStandard* q = (LaneStandard*) p;
	return mLink->CompareTo (q->mLink);
}


unsigned LaneStandard :: HashNumber (unsigned long Base) const {

	return mLink->HashNumber (Base);
}



Boolean LaneStandard :: IsEqualTo (const RGPersistent* p) const {

	LaneStandard* q = (LaneStandard*) p;
	return mLink->IsEqualTo (q->mLink);
}


void LaneStandard :: Write (RGFile& textOutput, const RGString& indent) {

	textOutput << indent.GetData () << "LaneStandard named:  " << GetLaneStandardName ().GetData () << "\n";
	const double* array;
	int size = GetCharacteristicArray (array);
	RGString temp;
	textOutput << indent.GetData () << indent.GetData () << "Characteristic array:  ";

	for (int i=0; i<size; i++) {

		temp.Convert ((int)array [i], 10);
		textOutput << temp.GetData () << "   ";
	}

	textOutput << "\n";
}



PopulationMarkerSet :: PopulationMarkerSet () : RGPersistent (), mLink (NULL), Valid (TRUE), 
Linked (FALSE), mLaneStandard (NULL) {

	mLink = new BasePopulationMarkerSet ();
	LocusIterator = new RGDListIterator (LocusList);
}


PopulationMarkerSet :: PopulationMarkerSet (BasePopulationMarkerSet* link, const RGString& xmlString) : RGPersistent (),
mLink (link), Valid (TRUE), Linked (TRUE), mLaneStandard (NULL) {

	Valid = BuildLocusList (xmlString);
	LocusIterator = new RGDListIterator (LocusList);
}


PopulationMarkerSet :: PopulationMarkerSet (const PopulationMarkerSet& markerSet) : RGPersistent (),
mLaneStandard (NULL) {
	
	if (markerSet.Linked) {

		mLink = markerSet.mLink;
		Linked = TRUE;
		Valid = markerSet.Valid;
		LocusList = markerSet.LocusList;
		Locus* nextLocus;
		LocusIterator = new RGDListIterator (LocusList);
		LocusIterator->Reset ();

		while (nextLocus = (Locus*)(*LocusIterator)())
			LocusListByName.Insert (nextLocus);

		if (markerSet.mLaneStandard != NULL)
			mLaneStandard = (LaneStandard*)markerSet.mLaneStandard->Copy ();
	}

	else {

		mLink = new BasePopulationMarkerSet ();
		LocusIterator = new RGDListIterator (LocusList);
		Linked = FALSE;
		Valid = TRUE;
	}
}


PopulationMarkerSet :: ~PopulationMarkerSet () {

	if (!Linked)
		delete mLink;

	delete mLaneStandard;
	LocusListByName.ClearAndDelete (); //????????????????????????
	LocusList.Clear ();
	delete LocusIterator;
}


void PopulationMarkerSet :: ClearAndDestroy () {

	RGHashTableIterator it (LocusListByName);
	Locus* nextLocus;

	while (nextLocus = (Locus*) it ())
		nextLocus->ClearAndDestroy ();
	
	delete mLink;
	mLink = NULL;
	LocusListByName.ClearAndDelete ();
	LocusList.Clear ();
}


Boolean PopulationMarkerSet :: RepresentsMarkerSet (const RGString& markerSet) const {

	return mLink->RepresentsMarkerSet (markerSet);
}


RGString PopulationMarkerSet :: GetMarkerSetName () const {

	return mLink->GetMarkerSetName ();

}


Locus* PopulationMarkerSet :: FindLocus (const RGString& name) {

	mTarget.SetLocusName (name);
	return (Locus*)LocusListByName.Find (&mTarget);
}


int PopulationMarkerSet :: GetLaneStandardChannel () const {

	return mLink->GetLaneStandardChannelNumber ();
}


int PopulationMarkerSet :: GetNumberOfChannels () const {

	return mLink->GetNumberOfChannels ();
}


void PopulationMarkerSet :: BuildChannelAndLocusListForHeader (RGString& headerList) {

	headerList = "";
	RGDListIterator it (LocusList);
	Locus* nextLocus;
	int currentChannel = 0;
	int locusChannel;
	RGString locusName;

	while (nextLocus = (Locus*) it ()) {

		locusChannel = nextLocus->GetLocusChannel ();

		if (locusChannel != currentChannel) {

			if (currentChannel != 0)
				headerList << "\t\t</Channel>\n";
			
			currentChannel = locusChannel;
			headerList << "\t\t<Channel>\n";
			headerList << "\t\t\t<ChannelNr>" << currentChannel << "</ChannelNr>\n";
		}

		headerList << "\t\t\t<LocusName>" << nextLocus->GetLocusName () << "</LocusName>\n";
	}

	if (currentChannel != 0)
		headerList << "\t\t</Channel>\n";
}


void PopulationMarkerSet :: SetMarkerSetName (const RGString& name) {

	mLink->SetMarkerSetName (name);
}


void PopulationMarkerSet :: ResetLocusList () {

	LocusIterator->Reset ();
}


Locus* PopulationMarkerSet :: GetNextLocus () {

	return (Locus*) (*LocusIterator) ();
}


Locus* PopulationMarkerSet :: GetNextLocusForChannel (int channel) {

	Locus* nextLocus;

	while (nextLocus = (Locus*) (*LocusIterator)()) {

		if (channel == nextLocus->GetLocusChannel ())
			return nextLocus;
	}

	return NULL;
}


void PopulationMarkerSet :: SetLaneStandard (LaneStandard* laneStd) {

	if (mLaneStandard != NULL)
		delete mLaneStandard;

	mLaneStandard = laneStd;
}


int PopulationMarkerSet :: AnalyzeLoci () {
	
	// ???

	return -1;
}


int PopulationMarkerSet :: GetLaneStandardCharacteristicArray (const double*& array) const {
	
	// returns array size or -1

	if (mLaneStandard == NULL) {

		array = NULL;
//		ErrorString << "Internal Lane Standard not specified for Population Marker Set named " << mLink->GetMarkerSetName () << "\n";
		return -1;
	}

	return mLaneStandard->GetCharacteristicArray (array);
}


int PopulationMarkerSet :: GetLaneStandardCharacteristicDifferenceArray (const double*& array) const {
	
	// returns array size or -1

	if (mLaneStandard == NULL) {

		array = NULL;
//		ErrorString << "Internal Lane Standard not specified for Population Marker Set named " << mLink->GetMarkerSetName () << "\n";
		return -1;
	}

	return mLaneStandard->GetCharacteristicDifferenceArray (array);
}


//	int BuildILSSplineBPToTime (const SupplementaryData& extraData);  // ???

const DataSignal* PopulationMarkerSet :: GetCurve (const RGString& locus, const RGString& allele, int& channel) {
	
	// ???

	mTarget.SetLocusName (locus);
	Locus* FoundLocus = (Locus*)LocusListByName.Find (&mTarget);

	if (FoundLocus == NULL) {

		ErrorString << "Could not find Locus named " << locus << " in Marker Set named " << mLink->GetMarkerSetName () << "\n";
		return NULL;
	}

	Allele* FoundAllele = FoundLocus->FindAllele (allele);

	if (FoundAllele == NULL) {

		ErrorString << "Could not find Allele named " << allele << " in Locus " << locus << ", in Marker Set named ";
		ErrorString << mLink->GetMarkerSetName () << "\n";
		return NULL;
	}

	channel = FoundLocus->GetLocusChannel ();
	return FoundAllele->GetCurvePointer ();
}



//	double GetTimeForCorrespondingBioID (int bioID, int channel);  // ????


int PopulationMarkerSet :: CompareTo (const RGPersistent* p) const {

	PopulationMarkerSet* q = (PopulationMarkerSet*) p;
	return mLink->CompareTo  (q->mLink);
}


unsigned PopulationMarkerSet :: HashNumber (unsigned long Base) const {

	return mLink->HashNumber (Base);
}


Boolean PopulationMarkerSet :: IsEqualTo (const RGPersistent* p) const {

	PopulationMarkerSet* q = (PopulationMarkerSet*) p;
	return mLink->IsEqualTo  (q->mLink);
}


void PopulationMarkerSet :: Write (RGFile& textOutput, const RGString& indent) {

	textOutput << "\n\n" << indent.GetData () << "Marker Set name = " << mLink->GetMarkerSetName ().GetData ();
	RGString temp;
	temp.Convert (mLink->GetNumberOfChannels (), 10);
	textOutput << ", Number of Channels = " << temp.GetData () << "\n";
	textOutput << indent.GetData () << "  Internal Lane Standard name = " << mLink->GetLaneStandardName ().GetData () << "\n";
	textOutput << indent.GetData () << "  Internal Lane Standard:\n";
	RGString indent2 = indent + "     ";
	mLaneStandard->Write (textOutput, indent2);
	textOutput << "\n";
	Locus* locus;
	RGHashTableIterator it (LocusListByName);

	while (locus = (Locus*)it ())
		locus->Write (textOutput, indent2);
}


int PopulationMarkerSet :: SelectBestSubsetOfCharacteristics (RGDList& set, const double* idealDifferences, int setSize, double& correlation) {

	int n = set.Entries ();

	if (n < setSize) {

		correlation = 0.0;
		return -1;
	}

	int n1 = n - 1;
	double* means = new double [n];
	DataSignal* nextSignal;
	RGDListIterator it (set);
	int i = 0;
	double* differences = new double [n1];

	while (nextSignal = (DataSignal*) it ()) {

		means [i] = nextSignal->GetMean ();
		i++;
	}

	for (i=0; i<n1; i++)
		differences [i] = means [i + 1] - means [i];

	int Displacement = -1;
	double MaxCorrelation = 0.0;
	double Correlation;
	int NSets = n - setSize + 1;
	int setSize1 = setSize - 1;

	for (int j=0; j<NSets; j++) {

		Correlation = VectorDotProductWithDifferences (differences + j, idealDifferences, setSize1);

		if (Correlation > MaxCorrelation) {

			MaxCorrelation = Correlation;
			Displacement = j;
		}
	}

	delete[] means;
	delete[] differences;

	correlation = MaxCorrelation;
	return Displacement;
}


double PopulationMarkerSet :: VectorDotProductWithDifferences (const double* array1, const double* idealNormalizedDifferences, int size) {

	//  We assume that idealDifferences are already normalized!!
	
	double Sum = 0.0;
	double norm = 0.0;
	double temp;

	for (int i=0; i<size; i++) {

		temp = array1 [i];
		Sum += temp * idealNormalizedDifferences [i];
		norm += temp * temp;
	}

	norm = sqrt (norm);

	if (norm > 0.0)
		Sum = Sum / norm;

	else
		Sum = 0.0;

	return Sum;
}


int PopulationMarkerSet :: SearchIncrementallyForBestSubsetOfCharacteristics (RGDList& set, RGDList& artifacts, const double* idealDifferences, int setSize, 
																			  const double* extremePoints, double& correlation, double correlationCriterion) {

	// This algorithm first locates what it considers the best fit of the right-most points.  Then, it incrementally adds a point until the set is filled, if possible.
	// It uses correlationCriterion as the initial criterion for good enough fit (at the first 3 points).  The ultimate criterion is up to the calling program.  In the 
	// first version of this program, March 4, 2008, the initial tests are not exhaustive.  They assume that the true and correct right-most 3 points can be chosen from
	// the last 4 points of the incoming set.  The list "set" must contain more peaks than setSize or we would not be in this subroutine.
	//  Test, just to make sure!

	if (set.Entries () < setSize)
		return -1;

	RGDList tempSet;
	RGDList tempArtifacts;
	ControlFit fit (idealDifferences, setSize);

	double extremes [2];
	extremes [0] = extremePoints [1] - extremePoints [0];
	extremes [1] = extremePoints [2] - extremePoints [1];
	ControlFit fit2 (extremes, 3);

	double lastPoints [4];
	RGDListIterator SetIterator (set);
	DataSignal* nextSignal;
	int extraPoints = set.Entries () - setSize;
	double firstAndLastPoints [3];
	nextSignal = (DataSignal*) set.First ();
	firstAndLastPoints [0] = nextSignal->GetMean ();
	double extremeCorrelations [4];

	SetIterator.ResetToEnd ();
	int i = 3;

	while (i >= 0) {

		nextSignal = (DataSignal*) SetIterator.CurrentItem ();
		lastPoints [i] = nextSignal->GetMean ();
		i--;
		--SetIterator;
	}

	double correlations [4];
	double points [3];
	int j;
	int k;

	for (i=0; i<4; i++) {

		k = 0;

		for (j=0; j<4; j++) {

			if (j != i) {

				points [k] = lastPoints [j];
				k++;
			}
		}

		correlations [i] = fit.Test3PointsOnRight (points);
		firstAndLastPoints [1] = points [1];
		firstAndLastPoints [2] = points [2];
		extremeCorrelations [i] = fit2.Test3PointsOnRight (firstAndLastPoints);
	}

	k = 0;
	double maxCorrelation = 0.0;
	int maxIndex;
	int bestIndexes [4];
	int maxExtremeIndex;

	for (i=0; i<4; i++) {

		if (correlations [i] > maxCorrelation) {
			
			maxIndex = i;
			maxCorrelation = correlations [i];
		}

		if (correlations [i] > correlationCriterion) {

			bestIndexes [k] = i;
			k++;
		}
	}

	if (k == 0) {

		correlation = 0.0;
		return -1;
	}

	if (k > 1) {

		// write this part later...assume for now it won't happen; oh, what the heck,..., let's write it now:
		
		maxCorrelation = 0.0;

		for (i=0; i<k; i++) {

			if (extremeCorrelations [i] > maxCorrelation) {

				maxCorrelation = extremeCorrelations [i];
				maxExtremeIndex = i;
			}
		}

		maxIndex = bestIndexes [maxExtremeIndex];
		maxCorrelation = correlations [maxExtremeIndex];
	}

	// Now move the 3 selected signals out of set and into tempSet.  If appropriate, move one signal into tempArtifact
	int removedSignalIndex = 3 - maxIndex;  // this is index from the end of removed curve.  If = 3, don't remove.
	SetIterator.ResetToEnd ();
	i = 3;
	j = 0;

	while (j < 3) {

		nextSignal = (DataSignal*) set.GetLast ();

		if (i == maxIndex) {

			tempArtifacts.Append (nextSignal);
		}

		else {

			tempSet.Prepend (nextSignal);
			points [i - 1] = nextSignal->GetMean ();
			j++;
		}

		i--;
	}

	extraPoints -= tempArtifacts.Entries ();
	fit.Set3PointsOnRight (points);
	int nPoints = 3;
	DataSignal* currentMaxSignal;
	double currentMaxCorrelation;
	double currentCorrelation;

	while (nPoints < setSize) {

		// Now we examine at most extraPoints signals to see which is the best addition
		// and then iteratively until tempSet has setSize entries

		j = 0;
		currentMaxCorrelation = 0.0;
		SetIterator.ResetToEnd ();

		while (j <= extraPoints) {

			nextSignal = (DataSignal*) SetIterator.CurrentItem ();
			currentCorrelation = fit.TestNewPoint (nextSignal->GetMean ());

			if (currentCorrelation > currentMaxCorrelation) {

				currentMaxCorrelation = currentCorrelation;
				currentMaxSignal = nextSignal;
			}

			j++;
			--SetIterator;
		}

		correlation = currentMaxCorrelation;

		while (true) {

			nextSignal = (DataSignal*) set.GetLast ();

			if (nextSignal == currentMaxSignal) {

				tempSet.Prepend (nextSignal);
				nPoints++;
				fit.AddNewPoint (nextSignal->GetMean ());
				break;
			}

			tempArtifacts.Prepend (nextSignal);
			extraPoints--;
		}

		if (extraPoints <= 0) {

			while (nextSignal = (DataSignal*) set.GetLast ()) {

				correlation = fit.TestNewPoint (nextSignal->GetMean ());
				fit.AddNewPoint (nextSignal->GetMean ());
				tempSet.Prepend (nextSignal);
				nPoints++;
			}

			if (nPoints < setSize)
				nPoints = setSize + 1;
		}
	}

	if (extraPoints > 0) {

		while (nextSignal = (DataSignal*) set.GetLast ())
			tempArtifacts.Prepend (nextSignal);

		extraPoints = 0;
	}

	while (nextSignal = (DataSignal*) tempSet.GetFirst ())
		set.Append (nextSignal);

	MergeListAIntoListB (tempArtifacts, artifacts);

	if (set.Entries () != setSize)
		return -1;

	return 0;
}


int PopulationMarkerSet :: SearchTwoSidedForBestSubsetOfCharacteristics (RGDList& set, RGDList& artifacts, const double* idealDifferences, int setSize, const double* setPoints, double& correlation, double correlationCriterion) {

	// This algorithm first locates what it considers the best fit of the right-most points.  Then, it incrementally adds a point until the set is filled, if possible.
	// It uses correlationCriterion as the initial criterion for good enough fit (at the first 3 points).  The ultimate criterion is up to the calling program.  In the 
	// first version of this program, March 4, 2008, the initial tests are not exhaustive.  They assume that the true and correct right-most 3 points can be chosen from
	// the last 4 points of the incoming set.  The list "set" must contain more peaks than setSize or we would not be in this subroutine.  Unlike SearchIncrementally..., above,
	// this method tests 3 points at a time, with an anchor in the right-most point, assumed already in the set.  The other two are allowed to float, including in their midst as
	// many as two omitted points.  Because the overall dot product becomes relatively insensitive as accepted points accumulate, in this method, only 3 points are considered at a
	// time.  Of the 3, then, only the middle one is added and that one becomes the new anchor for the incremental search.  With the assumption that there may be as many as two
	// extraneous points, there are, at most, 6 possible combinations to compare.  The largest (3-way) dot product wins.

	//  Test set size, just to make sure!

	if (set.Entries () < setSize)
		return -1;

	RGDList tempSetRight;
	RGDList tempSetLeft;
	RGDList tempArtifactsRight;
	RGDList tempArtifactsLeft;
	ControlFitTwoSided fit (setPoints, idealDifferences, setSize);

	double extremes [2];
	extremes [0] = setPoints [1] - setPoints [0];
	extremes [1] = setPoints [setSize - 1] - setPoints [1];
	ControlFit fit2 (extremes, 3);

	double lastPoints [4];
	RGDListIterator SetIterator (set);
	DataSignal* nextSignal;
	int extraPoints = set.Entries () - setSize;
	double firstAndLastPoints [3];
	nextSignal = (DataSignal*) set.First ();
	firstAndLastPoints [0] = nextSignal->GetMean ();
	double extremeCorrelations [4];

	SetIterator.ResetToEnd ();
	int i = 3;

	while (i >= 0) {

		nextSignal = (DataSignal*) SetIterator.CurrentItem ();
		lastPoints [i] = nextSignal->GetMean ();
		i--;
		--SetIterator;
	}

	double correlations [6];
	double points [6];
	int j;
	int k;

	for (i=0; i<4; i++) {

		k = 0;

		for (j=0; j<4; j++) {

			if (j != i) {

				points [k] = lastPoints [j];
				k++;
			}
		}

		correlations [i] = fit.Test3PointsOnRight (points);
		firstAndLastPoints [1] = points [1];
		firstAndLastPoints [2] = points [2];
		extremeCorrelations [i] = fit2.Test3PointsOnRight (firstAndLastPoints);
	}

	k = 0;
	double maxCorrelation = 0.0;
	int maxIndex;
	int bestIndexes [4];
	int maxExtremeIndex;

	for (i=0; i<4; i++) {

		if (correlations [i] > maxCorrelation) {
			
			maxIndex = i;
			maxCorrelation = correlations [i];
		}

		if (correlations [i] > correlationCriterion) {

			bestIndexes [k] = i;
			k++;
		}
	}

	if (k == 0) {

		correlation = 0.0;
		return -1;
	}

	if (k > 1) {

		// write this part later...assume for now it won't happen; oh, what the heck,..., let's write it now:
		
		maxCorrelation = 0.0;

		for (i=0; i<k; i++) {

			if (extremeCorrelations [i] > maxCorrelation) {

				maxCorrelation = extremeCorrelations [i];
				maxExtremeIndex = i;
			}
		}

		maxIndex = bestIndexes [maxExtremeIndex];
		maxCorrelation = correlations [maxExtremeIndex];
	}

	// Now move the 3 selected signals out of set and into tempSet.  If appropriate, move one signal into tempArtifact
	int removedSignalIndex = 3 - maxIndex;  // this is index from the end of removed curve.  If = 3, don't remove.
	SetIterator.ResetToEnd ();
	i = 3;
	j = 0;

	while (j < 3) {

		nextSignal = (DataSignal*) set.GetLast ();

		if (i == maxIndex) {

			tempArtifactsRight.Prepend (nextSignal);
		}

		else {

			tempSetRight.Prepend (nextSignal);
			points [i - 1] = nextSignal->GetMean ();
			j++;
		}

		i--;
	}

	extraPoints -= tempArtifactsRight.Entries ();
	fit.Set3PointsOnRight (points);
	int nPoints = 3;
	DataSignal* currentMaxSignal = NULL;
	double currentMaxCorrelation = 0.0;
	double currentCorrelation = 0.0;
	int currentMaxIndex;

	// Now find first 3 points!!  Here, we allow for as many as two artifacts among the first three
	// actual values.  In the future, we may revisit and allow more...

	DataSignal* leftSignals [6];
	SetIterator.Reset ();
	int nLeftSignals;
	int nLeftPatterns;
	int arrayPatterns [10][5] = {{1,1,1,0,0}, {1,1,0,1,0}, {1,0,1,1,0}, {0,1,1,1,0},
			{1,1,0,0,1}, {1,0,1,0,1}, {1,0,0,1,1}, {0,1,1,0,1}, {0,1,0,1,1}, {0,0,1,1,1}};

	if (extraPoints >= 2) {

		nLeftPatterns = 10;
		nLeftSignals = 5;
	}

	else if (extraPoints == 1) {

		nLeftPatterns = 4;
		nLeftSignals = 4;
	}

	else {

		nLeftPatterns = 1;
		nLeftSignals = 3;
	}

	if (nLeftSignals > 3) {

		for (i=0; i<nLeftSignals; i++) {

			nextSignal = (DataSignal*) SetIterator ();
			leftSignals [i] = nextSignal;
		}

		for (j=0; j<nLeftPatterns; j++) {

			k = 0;

			for (i=0; i<nLeftSignals; i++) {

				if (arrayPatterns [j][i] == 1) {

					points [k] = leftSignals [i]->GetMean ();
					k++;
				}
			}

			currentCorrelation = fit.Test3PointsOnLeft (points);

			if (currentCorrelation > currentMaxCorrelation) {

				currentMaxCorrelation = currentCorrelation;
				currentMaxIndex = j;
			}
		}

		k = 0;

		for (i=0; i<nLeftSignals; i++) {

			if (arrayPatterns [currentMaxIndex][i] == 1) {

				points [k] = leftSignals [i]->GetMean ();
				set.RemoveReference (leftSignals [i]);
				tempSetLeft.Append (leftSignals [i]);
				k++;

				if (k >= 3)
					break;
			}

			else {

				set.RemoveReference (leftSignals [i]);
				tempArtifactsLeft.Append (leftSignals [i]);
				extraPoints--;
			}
		}

		fit.Set3PointsOnLeft (points);
		nPoints += 3;
	}

	//  Now we have found the first three points.  First we check to see if there are any
	//  extra points left.  If not, we're all but done.  We just have to reorganize the lists
	//  If there are, we must continue until all points chosen or until all extra points are gone.

	if (extraPoints == 0) {

		// we're done!!!  But do a simple test...

		if (tempSetLeft.Entries () + tempSetRight.Entries () + set.Entries () != setSize) // oops!
			return -10;

		while (nextSignal = (DataSignal*) set.GetLast ()) {

			if (nPoints > 3)
				correlation = fit.TestNewPointOnRight (nextSignal->GetMean ());  // two-sided and equal to overall correlation at end

			else
				correlation = fit.TestNewPointOnRightOnly (nextSignal->GetMean ());

			fit.AddNewPointOnRight (nextSignal->GetMean ());
			tempSetRight.Prepend (nextSignal);
		} // this gives us the correct correlation to return

		while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
			set.Append (nextSignal);

		while (nextSignal = (DataSignal*) tempSetRight.GetFirst ())
			set.Append (nextSignal);

		MergeListAIntoListB (tempArtifactsLeft, artifacts);
		MergeListAIntoListB (tempArtifactsRight, artifacts);
		return 0;
	}

	//
	//  OK...here's where we pick it up!!!  Now we test and add new points from within the middle region...
	//  first right then left, etc., always alternating, always testing a point one sided, until the last one.
	//  On the last one, we test the point two-sided and then we will get the overall correlation!
	//

	bool addRight = true;
	bool notLastPoint = true;

	while (nPoints < setSize) {

		// Now we examine at most extraPoints + 1 signals to see which is the best addition
		// and then iteratively until tempSet has setSize entries, first right, then left, etc,
		// alternating until the last signal.  Before the last point to be added, we use the
		// one-sided inner product test to determine which signal to add because it is more
		// discriminating.  On the last point, the two-sided test is definitive because the 
		// result of the two-sided test is the total inner product that we are trying to maximize.

		if (nPoints == setSize - 1)
			notLastPoint = false;

		if (addRight) {  // test signals from the right

			j = 0;
			currentMaxCorrelation = 0.0;
			SetIterator.ResetToEnd ();

			while (j <= extraPoints) {

				nextSignal = (DataSignal*) SetIterator.CurrentItem ();  // this should never be null!!!

				if (notLastPoint)
					currentCorrelation = fit.TestNewPointOnRightOnly (nextSignal->GetMean ());

				else
					currentCorrelation = fit.TestNewPointOnRight (nextSignal->GetMean ()); // two sided test

				if (currentCorrelation > currentMaxCorrelation) {

					currentMaxCorrelation = currentCorrelation;
					currentMaxSignal = nextSignal;
				}

				j++;
				--SetIterator;
			}

			correlation = currentMaxCorrelation;

			while (true) {

				nextSignal = (DataSignal*) set.GetLast ();

				if (nextSignal == currentMaxSignal) {

					tempSetRight.Prepend (nextSignal);
					nPoints++;
					fit.AddNewPointOnRight (nextSignal->GetMean ());
					break;
				}

				tempArtifactsRight.Prepend (nextSignal);
				extraPoints--;
			}

			addRight = false;
		} // This is the end of the "add right" section

		else {  // test signals from the left

			j = 0;
			currentMaxCorrelation = 0.0;
			SetIterator.Reset ();

			while (j <= extraPoints) {

				nextSignal = (DataSignal*) SetIterator ();  // this should never be null!!!

				if (notLastPoint)
					currentCorrelation = fit.TestNewPointOnLeftOnly (nextSignal->GetMean ());

				else
					currentCorrelation = fit.TestNewPointOnLeft (nextSignal->GetMean ()); // two-sided test

				if (currentCorrelation > currentMaxCorrelation) {

					currentMaxCorrelation = currentCorrelation;
					currentMaxSignal = nextSignal;
				}

				j++;
			}

			correlation = currentMaxCorrelation;

			while (true) {

				nextSignal = (DataSignal*) set.GetFirst ();

				if (nextSignal == currentMaxSignal) {

					tempSetLeft.Append (nextSignal);
					nPoints++;
					fit.AddNewPointOnLeft (nextSignal->GetMean ());
					break;
				}

				tempArtifactsLeft.Append (nextSignal);
				extraPoints--;
			}

			addRight = true;
		} // This is the end of the "add left" section

		if (extraPoints <= 0) {

			while (nextSignal = (DataSignal*) set.GetLast ()) {

				correlation = fit.TestNewPointOnRight (nextSignal->GetMean ());
				fit.AddNewPointOnRight (nextSignal->GetMean ());
				tempSetRight.Prepend (nextSignal);
				nPoints++;
			}

			if (nPoints < setSize)
				nPoints = setSize + 1;
		}
	}

	if (extraPoints > 0) {

		while (nextSignal = (DataSignal*) set.GetLast ())
			tempArtifactsRight.Prepend (nextSignal);

		extraPoints = 0;
	}

	while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
		set.Append (nextSignal);

	while (nextSignal = (DataSignal*) tempSetRight.GetFirst ())
		set.Append (nextSignal);

	MergeListAIntoListB (tempArtifactsLeft, artifacts);
	MergeListAIntoListB (tempArtifactsRight, artifacts);

	if (set.Entries () != setSize)
		return -1;

	return 0;
}








int PopulationMarkerSet :: SearchTwoSidedWithLeadForBestSubsetOfCharacteristics (RGDList& set, RGDList& artifacts, const double* idealDifferences, int setSize, const double* setPoints, double& correlation, double correlationCriterion) {

	// This algorithm first locates what it considers the best fit of the right-most points.  Then, it incrementally adds a point until the set is filled, if possible.
	// It uses correlationCriterion as the initial criterion for good enough fit (at the first 3 points).  The ultimate criterion is up to the calling program.  In the 
	// first version of this program, March 4, 2008, the initial tests are not exhaustive.  They assume that the true and correct right-most 3 points can be chosen from
	// the last 4 points of the incoming set.  The list "set" must contain more peaks than setSize or we would not be in this subroutine.  Unlike SearchIncrementally..., above,
	// this method tests 3 points at a time, with an anchor in the right-most point, assumed already in the set.  The other two are allowed to float, including in their midst as
	// many as two omitted points.  Because the overall dot product becomes relatively insensitive as accepted points accumulate, in this method, only 3 points are considered at a
	// time.  Of the 3, then, only the middle one is added and that one becomes the new anchor for the incremental search.  With the assumption that there may be as many as two
	// extraneous points, there are, at most, 6 possible combinations to compare.  The largest (3-way) dot product wins.  This algorithm differs
	// from plain old "SearchTwoSided..." above because here, we allow an arbitrary number (up to the number of extra points) of leading peaks
	// that are not to be included in the final set.

	//  Test set size, just to make sure!

	if (set.Entries () < setSize)
		return -1;

	RGDList tempSetRight;
	RGDList tempSetLeft;
	RGDList tempArtifactsRight;
	RGDList tempArtifactsLeft;
	ControlFitTwoSided fit (setPoints, idealDifferences, setSize);

	double extremes [2];
	extremes [0] = setPoints [1] - setPoints [0];
	extremes [1] = setPoints [setSize - 1] - setPoints [1];
	ControlFit fit2 (extremes, 3);

	double lastPoints [4];
	RGDListIterator SetIterator (set);
	DataSignal* nextSignal;
	int extraPoints = set.Entries () - setSize;
	double firstAndLastPoints [3];
	nextSignal = (DataSignal*) set.First ();
	firstAndLastPoints [0] = nextSignal->GetMean ();
	double extremeCorrelations [4];

	SetIterator.ResetToEnd ();
	int i = 3;

	while (i >= 0) {

		nextSignal = (DataSignal*) SetIterator.CurrentItem ();
		lastPoints [i] = nextSignal->GetMean ();
		i--;
		--SetIterator;
	}

	double correlations [6];
	double points [6];

	for (i=0; i<6; i++)
		points [i] = 0.0;

	int j;
	int k;

	for (i=0; i<4; i++) {

		k = 0;

		for (j=0; j<4; j++) {

			if (j != i) {

				points [k] = lastPoints [j];
				k++;
			}
		}

		correlations [i] = fit.Test3PointsOnRight (points);
		firstAndLastPoints [1] = points [1];
		firstAndLastPoints [2] = points [2];
		extremeCorrelations [i] = fit2.Test3PointsOnRight (firstAndLastPoints);
	}

	k = 0;
	double maxCorrelation = 0.0;
	int maxIndex;
	int bestIndexes [4];

	for (i=0; i<4; i++)
		bestIndexes [i] = 0;

	int maxExtremeIndex;

	for (i=0; i<4; i++) {

		if (correlations [i] > maxCorrelation) {
			
			maxIndex = i;
			maxCorrelation = correlations [i];
		}

		if (correlations [i] > correlationCriterion) {

			bestIndexes [k] = i;
			k++;
		}
	}

	if (k == 0) {

		correlation = 0.0;
		return -1;
	}

	if (k > 1) {

		// write this part later...assume for now it won't happen; oh, what the heck,..., let's write it now:
		
		maxCorrelation = 0.0;

		for (i=0; i<k; i++) {

			if (extremeCorrelations [i] > maxCorrelation) {

				maxCorrelation = extremeCorrelations [i];
				maxExtremeIndex = i;
			}
		}

		maxIndex = bestIndexes [maxExtremeIndex];
		maxCorrelation = correlations [maxExtremeIndex];
	}

	// Now move the 3 selected signals out of set and into tempSet.  If appropriate, move one signal into tempArtifact
	int removedSignalIndex = 3 - maxIndex;  // this is index from the end of removed curve.  If = 3, don't remove.
	SetIterator.ResetToEnd ();
	i = 3;
	j = 0;
	double avePeak = 0.0;

	while (j < 3) {

		nextSignal = (DataSignal*) set.GetLast ();

		if (i == maxIndex) {

			tempArtifactsRight.Prepend (nextSignal);
		}

		else {

			tempSetRight.Prepend (nextSignal);
			points [2 - j] = nextSignal->GetMean ();
			avePeak += nextSignal->Peak ();
			j++;
		}

		i--;
	}

	avePeak = avePeak / 3.0;
	extraPoints -= tempArtifactsRight.Entries ();
	fit.Set3PointsOnRight (points);
	int nPoints = 3;
	DataSignal* currentMaxSignal = NULL;
	double currentMaxCorrelation = 0.0;
	double currentCorrelation = 0.0;
	int currentMaxIndex;
	int currentMaxLeadingBlanks;
	double fractionOfAvePeak = 0.35;
	double fractionalAvePeak = fractionOfAvePeak * avePeak;

	if (extraPoints > 0) {

		SetIterator.Reset ();

		while (nextSignal = (DataSignal*) SetIterator ()) {

			if (extraPoints == 0)
				break;

			if (nextSignal->Peak () < fractionalAvePeak) {

				SetIterator.RemoveCurrentItem ();
				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", "Peak out of place in control set: uncategorized artifact");
				tempArtifactsLeft.Append (nextSignal);
				extraPoints--;
			}
		}
	}

	if (extraPoints > 0) {

		// Now find first 3 points!!  Here, we allow for as many as two artifacts among the first three
		// actual values.  In the future, we may revisit and allow more...

		DataSignal** leftSignals = new DataSignal* [extraPoints + 4];
		DataSignal** startPermutations;
		int extra3 = extraPoints + 3;
		SetIterator.Reset ();

		for (i=0; i<extra3; i++)
			leftSignals [i] = (DataSignal*) SetIterator ();

		int nPermuteBlanks;
		startPermutations = leftSignals;
		int nLeftSignals;
		int nLeftPatterns;
		int leadingBlanks;
		int nLeftSignalsMax;
		int arrayPatterns [6][5] = {{1,1,1,0,0}, {1,1,0,1,0}, {1,0,1,1,0}, 
				{1,1,0,0,1}, {1,0,1,0,1}, {1,0,0,1,1}};

		for (leadingBlanks=0; leadingBlanks<=extraPoints; leadingBlanks++) {

			startPermutations = leftSignals + leadingBlanks;
			nPermuteBlanks = extraPoints - leadingBlanks;

			if (nPermuteBlanks >= 2) {

				nLeftPatterns = 6;
				nLeftSignals = 5;
			}

			else if (nPermuteBlanks == 1) {

				nLeftPatterns = 3;
				nLeftSignals = 4;
			}

			else {

				nLeftPatterns = 1;
				nLeftSignals = 3;
			}

			for (j=0; j<nLeftPatterns; j++) {

				k = 0;

				for (i=0; i<nLeftSignals; i++) {

					if (arrayPatterns [j][i] == 1) {

						points [k] = startPermutations [i]->GetMean ();
						k++;
					}
				}

				currentCorrelation = fit.Test3PointsOnLeft (points);

				if (currentCorrelation > currentMaxCorrelation) {

					currentMaxCorrelation = currentCorrelation;
					currentMaxIndex = j;
					currentMaxLeadingBlanks = leadingBlanks;
					nLeftSignalsMax = nLeftSignals;
				}
			}
		}

		for (i=0; i<currentMaxLeadingBlanks; i++) {

			nextSignal = (DataSignal*) set.GetFirst ();
			tempArtifactsLeft.Append (nextSignal);
		}

		k = 0;
		startPermutations = leftSignals + currentMaxLeadingBlanks;
		extraPoints -= currentMaxLeadingBlanks;

		for (i=0; i<nLeftSignalsMax; i++) {

			nextSignal = startPermutations [i];

			if (arrayPatterns [currentMaxIndex][i] == 1) {

				points [k] = nextSignal->GetMean ();
				set.RemoveReference (nextSignal);
				tempSetLeft.Append (nextSignal);
				k++;

				if (k >= 3)
					break;
			}

			else {

				set.RemoveReference (nextSignal);
				tempArtifactsLeft.Append (nextSignal);
				extraPoints--;
			}
		}

		fit.Set3PointsOnLeft (points);
		nPoints += 3;
		delete[] leftSignals;

		//  Now we have found the first three points.  First we check to see if there are any
		//  extra points left.  If not, we're all but done.  We just have to reorganize the lists
		//  If there are, we must continue until all points chosen or until all extra points are gone.
	}

	if (extraPoints == 0) {

		// we're done!!!  But do a simple test...

		if (tempSetLeft.Entries () + tempSetRight.Entries () + set.Entries () != setSize) // oops!
			return -10;

		while (nextSignal = (DataSignal*) set.GetLast ()) {

			if (nPoints > 3)
				correlation = fit.TestNewPointOnRight (nextSignal->GetMean ());  // two-sided and equal to overall correlation at end

			else
				correlation = fit.TestNewPointOnRightOnly (nextSignal->GetMean ());

			fit.AddNewPointOnRight (nextSignal->GetMean ());
			tempSetRight.Prepend (nextSignal);
		} // this gives us the correct correlation to return

		while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
			set.Append (nextSignal);

		while (nextSignal = (DataSignal*) tempSetRight.GetFirst ())
			set.Append (nextSignal);

		MergeListAIntoListB (tempArtifactsLeft, artifacts);
		MergeListAIntoListB (tempArtifactsRight, artifacts);
		return 0;
	}

	//
	//  OK...here's where we pick it up!!!  Now we test and add new points from within the middle region...
	//  first right then left, etc., always alternating, always testing a point one sided, until the last one.
	//  On the last one, we test the point two-sided and then we will get the overall correlation!
	//

	bool addRight = true;
	bool notLastPoint = true;

	while (nPoints < setSize) {

		// Now we examine at most extraPoints + 1 signals to see which is the best addition
		// and then iteratively until tempSet has setSize entries, first right, then left, etc,
		// alternating until the last signal.  Before the last point to be added, we use the
		// one-sided inner product test to determine which signal to add because it is more
		// discriminating.  On the last point, the two-sided test is definitive because the 
		// result of the two-sided test is the total inner product that we are trying to maximize.

		if (nPoints == setSize - 1)
			notLastPoint = false;

		if (addRight) {  // test signals from the right

			j = 0;
			currentMaxCorrelation = 0.0;
			SetIterator.ResetToEnd ();

			while (j <= extraPoints) {

				nextSignal = (DataSignal*) SetIterator.CurrentItem ();  // this should never be null!!!

				if (notLastPoint)
					currentCorrelation = fit.TestNewPointOnRightOnly (nextSignal->GetMean ());

				else
					currentCorrelation = fit.TestNewPointOnRight (nextSignal->GetMean ()); // two sided test

				if (currentCorrelation > currentMaxCorrelation) {

					currentMaxCorrelation = currentCorrelation;
					currentMaxSignal = nextSignal;
				}

				j++;
				--SetIterator;
			}

			correlation = currentMaxCorrelation;

			while (true) {

				nextSignal = (DataSignal*) set.GetLast ();

				if (nextSignal == currentMaxSignal) {

					tempSetRight.Prepend (nextSignal);
					nPoints++;
					fit.AddNewPointOnRight (nextSignal->GetMean ());
					break;
				}

				tempArtifactsRight.Prepend (nextSignal);
				extraPoints--;
			}

			addRight = false;
		} // This is the end of the "add right" section

		else {  // test signals from the left

			j = 0;
			currentMaxCorrelation = 0.0;
			SetIterator.Reset ();

			while (j <= extraPoints) {

				nextSignal = (DataSignal*) SetIterator ();  // this should never be null!!!

				if (notLastPoint)
					currentCorrelation = fit.TestNewPointOnLeftOnly (nextSignal->GetMean ());

				else
					currentCorrelation = fit.TestNewPointOnLeft (nextSignal->GetMean ()); // two-sided test

				if (currentCorrelation > currentMaxCorrelation) {

					currentMaxCorrelation = currentCorrelation;
					currentMaxSignal = nextSignal;
				}

				j++;
			}

			correlation = currentMaxCorrelation;

			while (true) {

				nextSignal = (DataSignal*) set.GetFirst ();

				if (nextSignal == currentMaxSignal) {

					tempSetLeft.Append (nextSignal);
					nPoints++;
					fit.AddNewPointOnLeft (nextSignal->GetMean ());
					break;
				}

				tempArtifactsLeft.Append (nextSignal);
				extraPoints--;
			}

			addRight = true;
		} // This is the end of the "add left" section

		if (extraPoints <= 0) {

			while (nextSignal = (DataSignal*) set.GetLast ()) {

				correlation = fit.TestNewPointOnRight (nextSignal->GetMean ());
				fit.AddNewPointOnRight (nextSignal->GetMean ());
				tempSetRight.Prepend (nextSignal);
				nPoints++;
			}

			if (nPoints < setSize)
				nPoints = setSize + 1;
		}
	}

	if (extraPoints > 0) {

		while (nextSignal = (DataSignal*) set.GetLast ())
			tempArtifactsRight.Prepend (nextSignal);

		extraPoints = 0;
	}

	while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
		set.Append (nextSignal);

	while (nextSignal = (DataSignal*) tempSetRight.GetFirst ())
		set.Append (nextSignal);

	MergeListAIntoListB (tempArtifactsLeft, artifacts);
	MergeListAIntoListB (tempArtifactsRight, artifacts);

	if (set.Entries () != setSize)
		return -1;

	return 0;
}








int PopulationMarkerSet :: SearchRecursivelyForBestSubset (RGDList& set, RGDList& artifacts, IdealControlSetInfo& ideal, double& correlation, double correlationCriterion, int nStartPts, double maxHeight) {

	// This algorithm first locates what it considers the best fit of the right-most points.  Then, it incrementally adds a point until the set is filled, if possible.
	// It uses correlationCriterion as the initial criterion for good enough fit (at the first 3 points).  The ultimate criterion is up to the calling program.  In the 
	// first version of this program, March 4, 2008, the initial tests are not exhaustive.  They assume that the true and correct right-most 3 points can be chosen from
	// the last 4 points of the incoming set.  The list "set" must contain more peaks than setSize or we would not be in this subroutine.  Unlike SearchIncrementally..., above,
	// this method tests 3 points at a time, with an anchor in the right-most point, assumed already in the set.  The other two are allowed to float, including in their midst as
	// many as two omitted points.  Because the overall dot product becomes relatively insensitive as accepted points accumulate, in this method, only 3 points are considered at a
	// time.  Of the 3, then, only the middle one is added and that one becomes the new anchor for the incremental search.  With the assumption that there may be as many as two
	// extraneous points, there are, at most, 6 possible combinations to compare.  The largest (3-way) dot product wins.  This algorithm differs
	// from plain old "SearchTwoSided..." above because here, we allow an arbitrary number (up to the number of extra points) of leading peaks
	// that are not to be included in the final set.

	// Currently, 04/30/2008, this algorithm does not check for relative peak heights, except for the first and last nStartPts points...
	// It's in now...

	//  Test set size, just to make sure!

	int NCandidates = set.Entries ();
	int setSize = ideal.mSetSize;
  int nPoints;
	correlation = 0.0;
	int status = 0;

	if (NCandidates < setSize)
		return -1;

	RGDList tempSetRight;
	RGDList tempSetLeft;
	RGDList tempArtifactsRight;
	RGDList tempArtifactsLeft;
	ControlFitTwoSided fit (ideal.mPts, ideal.mDiffs, setSize);

	int rightIdealIndex;
	int leftIdealIndex;

// following 5 lines were moved here from other locations in the code
// because a goto was jumping over them causing an error in gcc
//  djh 4/20/09

	bool addRight = true;
	bool notLastPoint = true;
	DataSignal* currentMaxSignal = NULL;
	double currentMaxCorrelation = 0.0;
	double currentCorrelation = 0.0;


	RGDListIterator SetIterator (set);
	DataSignal* nextSignal;
	int extraPoints = set.Entries () - setSize;
//	cout << "Starting extra points, before left analysis = " << extraPoints << endl;

	int i;
	int j;

	int* selectionArray = new int [NCandidates];
	double* selectedMeans = new double [nStartPts];

	for (i=0; i<NCandidates; i++)
		selectionArray [i] = 0;

	RecursiveInnerProduct rightRecursion (set, ideal.mPts, ideal.mDiffs, ideal.mHeightIndices, ideal.mRightNorm2s, setSize, maxHeight, ideal.mDiscrepancy);
	RecursiveInnerProduct leftRecursion (set, ideal.mPts, ideal.mDiffs, ideal.mHeightIndices, ideal.mLeftNorm2s, setSize, maxHeight, ideal.mDiscrepancy);
	rightRecursion.SetSignalIntervalIsAccurate (ideal.mSignalIntervalIsAccurate);
	leftRecursion.SetSignalIntervalIsAccurate (ideal.mSignalIntervalIsAccurate);

	if (leftRecursion.BuildFromLeft (nStartPts, extraPoints, correlationCriterion) < 0)
		goto ErrorReturn;

	// Now move the selected signals out of set and into tempSetLeft.  Move skipped signals into tempArtifactsLeft

	i = 0;
	j = 0;
	correlation = leftRecursion.GetCumulativeCorrelation ();
	leftRecursion.GetBestSelection (selectionArray);

//	if (correlation < correlationCriterion)
//		goto ErrorReturn;

	while (j < nStartPts) {

		nextSignal = (DataSignal*) set.GetFirst ();

		if (nextSignal == NULL)
			goto ErrorReturn;

		if (selectionArray [i] == 0) {

			tempArtifactsLeft.Append (nextSignal);
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
				"Peak out of place in control set: uncategorized artifact");
		}

		else {

			tempSetLeft.Append (nextSignal);
			selectedMeans [j] = nextSignal->GetMean ();
			j++;

			if (j >= nStartPts)
				break;
		}

		i++;

		if (i >= NCandidates)
			goto ErrorReturn;
	}

	extraPoints -= tempArtifactsLeft.Entries ();
	fit.SetNpointsOnLeft (selectedMeans, nStartPts);
	leftIdealIndex = nStartPts;
	nPoints = nStartPts;
//	cout << "Extra points after left fit = " << extraPoints << endl;

	//
	// Test here to see if fit all of the points.  If so, clean up and return
	//

	if (nStartPts == setSize) {

		// We're done!  Put the rest of set into artifacts and move selected peaks back into set.

		while (nextSignal = (DataSignal*) set.GetFirst ()) {

			tempArtifactsLeft.Append (nextSignal);
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
				"Peak out of place in control set: uncategorized artifact");
		}

		while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
			set.Append (nextSignal);

		MergeListAIntoListB (tempArtifactsLeft, artifacts);
		MergeListAIntoListB (tempArtifactsRight, artifacts);
		delete[] selectionArray;
		delete[] selectedMeans;
		return 0;
	}

	if (rightRecursion.BuildFromRight (nStartPts, extraPoints, correlationCriterion) < 0)
		goto ErrorReturn;

	// Now move the selected signals out of set and into tempSetRight.  Move skipped signals into tempArtifactsRight

	i = NCandidates - 1;
	j = nStartPts - 1;
	correlation = rightRecursion.GetCumulativeCorrelation ();
	rightRecursion.GetBestSelection (selectionArray);

//	if (correlation < correlationCriterion)
//		goto ErrorReturn;

	while (j >= 0) {

		nextSignal = (DataSignal*) set.GetLast ();

		if (nextSignal == NULL)
			goto ErrorReturn;

		if (selectionArray [i] == 0) {

			tempArtifactsRight.Prepend (nextSignal);
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
				"Peak out of place in control set: uncategorized artifact");
		}

		else {

			tempSetRight.Prepend (nextSignal);
			selectionArray [i] = 0;
			selectedMeans [j] = nextSignal->GetMean ();
			j--;

			if (j < 0)
				break;
		}

		i--;

		if (i < 0)
			goto ErrorReturn;
	}

	rightIdealIndex = setSize - nStartPts - 1;
	extraPoints -= tempArtifactsRight.Entries ();
	fit.SetNPointsOnRight (selectedMeans, nStartPts);
	
//	int currentMaxIndex;
//	int currentMaxLeadingBlanks;
//	double fractionOfAvePeak = 0.35;
//	double fractionalAvePeak = fractionOfAvePeak * avePeak;

	nPoints += nStartPts;
		
	//  Now we have found the first three points.  First we check to see if there are any
	//  extra points left.  If not, we're all but done.  We just have to reorganize the lists
	//  If there are, we must continue until all points chosen or until all extra points are gone.

	if (extraPoints == 0) {

		// we're essentially done!!!

		while (nextSignal = (DataSignal*) set.GetLast ()) {

			if (set.Entries () == 0)  // This is the last entry...
				correlation = fit.TestNewPointOnRight (nextSignal->GetMean ());  // two-sided and equal to overall correlation at end

			else
				correlation = fit.TestNewPointOnRightOnly (nextSignal->GetMean ());

			fit.AddNewPointOnRight (nextSignal->GetMean ());
			tempSetRight.Prepend (nextSignal);
		} // this gives us the correct correlation to return

		while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
			set.Append (nextSignal);

		while (nextSignal = (DataSignal*) tempSetRight.GetFirst ())
			set.Append (nextSignal);

		MergeListAIntoListB (tempArtifactsLeft, artifacts);
		MergeListAIntoListB (tempArtifactsRight, artifacts);
		delete[] selectionArray;
		delete[] selectedMeans;

		// This should never happen, but just in case, let's do a simple test...did we get them all???

		if (set.Entries () != setSize)
			return -10;

		return 0;
	}

	//
	//  OK...here's where we pick it up!!!  Now we test and add new points from within the middle region...
	//  first right then left, etc., always alternating, always testing a point one sided, until the last one.
	//  On the last one, we test the point two-sided and then we will get the overall correlation!
	//


	while (nPoints < setSize) {

		// Now we examine at most extraPoints + 1 signals to see which is the best addition
		// and then iteratively until tempSet has setSize entries, first right, then left, etc,
		// alternating until the last signal.  Before the last point to be added, we use the
		// one-sided inner product test to determine which signal to add because it is more
		// discriminating.  On the last point, the two-sided test is definitive because the 
		// result of the two-sided test is the total inner product that we are trying to maximize.

		if (nPoints == setSize - 1)
			notLastPoint = false;

		if (addRight) {  // test signals from the right

			j = 0;
			currentMaxCorrelation = 0.0;
			currentMaxSignal = NULL;
			SetIterator.ResetToEnd ();

			while (j <= extraPoints) {

				nextSignal = (DataSignal*) SetIterator.CurrentItem ();  // this should never be null!!!  Test, though, for safety

				if (nextSignal == NULL)
					goto ErrorReturn;

				// Test relative peak height here!!!

				if (RecursiveInnerProduct::HeightOutsideLimit (ideal.mHeightIndices, rightIdealIndex, nextSignal->Peak (), maxHeight, ideal.mDiscrepancy))
					currentCorrelation = 0.0;

				else if (notLastPoint)
					currentCorrelation = fit.TestNewPointOnRightOnly (nextSignal->GetMean ());

				else
					currentCorrelation = fit.TestNewPointOnRight (nextSignal->GetMean ()); // two sided test

				if (currentCorrelation > currentMaxCorrelation) {

					currentMaxCorrelation = currentCorrelation;
					currentMaxSignal = nextSignal;
				}

				j++;
				--SetIterator;
			}

			correlation = currentMaxCorrelation;

			if (currentMaxSignal == NULL)
				goto ErrorReturn;

			while (true) {

				nextSignal = (DataSignal*) set.GetLast ();

				if (nextSignal == currentMaxSignal) {

					tempSetRight.Prepend (nextSignal);
					nPoints++;
					fit.AddNewPointOnRight (nextSignal->GetMean ());
					rightIdealIndex--;
					break;
				}

				tempArtifactsRight.Prepend (nextSignal);
				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
					"Peak out of place in control set: uncategorized artifact");
				extraPoints--;
			}

			addRight = false;
		} // This is the end of the "add right" section

		else {  // test signals from the left

			j = 0;
			currentMaxCorrelation = 0.0;
			currentMaxSignal = NULL;
			SetIterator.Reset ();

			while (j <= extraPoints) {

				nextSignal = (DataSignal*) SetIterator ();  // this should never be null!!!  Test, though, for safety

				if (nextSignal == NULL)
					goto ErrorReturn;

				// Test relative peak height here!!!

				if (RecursiveInnerProduct::HeightOutsideLimit (ideal.mHeightIndices, leftIdealIndex, nextSignal->Peak (), maxHeight, ideal.mDiscrepancy))
					currentCorrelation = 0.0;

				else if (notLastPoint)
					currentCorrelation = fit.TestNewPointOnLeftOnly (nextSignal->GetMean ());

				else
					currentCorrelation = fit.TestNewPointOnLeft (nextSignal->GetMean ()); // two-sided test

				if (currentCorrelation > currentMaxCorrelation) {

					currentMaxCorrelation = currentCorrelation;
					currentMaxSignal = nextSignal;
				}

				j++;
			}

			correlation = currentMaxCorrelation;

			if (currentMaxSignal == NULL)
				goto ErrorReturn;

			while (true) {

				nextSignal = (DataSignal*) set.GetFirst ();

				if (nextSignal == currentMaxSignal) {

					tempSetLeft.Append (nextSignal);
					nPoints++;
					fit.AddNewPointOnLeft (nextSignal->GetMean ());
					leftIdealIndex++;
					break;
				}

				tempArtifactsLeft.Append (nextSignal);
				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
					"Peak out of place in control set: uncategorized artifact");
				extraPoints--;
			}

			addRight = true;
		} // This is the end of the "add left" section

/*		if (extraPoints <= 0) {

			while (nextSignal = (DataSignal*) set.GetLast ()) {

				correlation = fit.TestNewPointOnRight (nextSignal->GetMean ());
				fit.AddNewPointOnRight (nextSignal->GetMean ());
				tempSetRight.Prepend (nextSignal);
				nPoints++;
			}

			if (nPoints < setSize)
				nPoints = setSize + 1;
		}  */
	}

	if (extraPoints > 0) {

		while (nextSignal = (DataSignal*) set.GetLast ()) {

			tempArtifactsRight.Prepend (nextSignal);
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
				"Peak out of place in control set: uncategorized artifact");
		}

		extraPoints = 0;
	}

	while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
		set.Append (nextSignal);

	while (nextSignal = (DataSignal*) tempSetRight.GetFirst ())
		set.Append (nextSignal);

	MergeListAIntoListB (tempArtifactsLeft, artifacts);
	MergeListAIntoListB (tempArtifactsRight, artifacts);
	delete[] selectionArray;
	delete[] selectedMeans;

	if (set.Entries () != setSize)
		return -1;

	return 0;

ErrorReturn:
	MergeListAIntoListB (set, artifacts);

	while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
		set.Append (nextSignal);

	while (nextSignal = (DataSignal*) tempSetRight.GetFirst ())
		set.Append (nextSignal);

	MergeListAIntoListB (tempArtifactsLeft, artifacts);
	MergeListAIntoListB (tempArtifactsRight, artifacts);
	delete[] selectionArray;
	delete[] selectedMeans;
	return -1;
}







int PopulationMarkerSet :: SearchRecursivelyForBestLadderLocusSubset (RGDList& set, RGDList& artifacts, IdealControlSetInfo& ideal, double& correlation, double correlationCriterion, int nStartPts, double maxHeight) {

	// This algorithm first locates what it considers the best fit of the right-most points.  Then, it incrementally adds a point until the set is filled, if possible.
	// It uses correlationCriterion as the initial criterion for good enough fit (at the first 3 points).  The ultimate criterion is up to the calling program.  In the 
	// first version of this program, March 4, 2008, the initial tests are not exhaustive.  They assume that the true and correct right-most 3 points can be chosen from
	// the last 4 points of the incoming set.  The list "set" must contain more peaks than setSize or we would not be in this subroutine.  Unlike SearchIncrementally..., above,
	// this method tests 3 points at a time, with an anchor in the right-most point, assumed already in the set.  The other two are allowed to float, including in their midst as
	// many as two omitted points.  Because the overall dot product becomes relatively insensitive as accepted points accumulate, in this method, only 3 points are considered at a
	// time.  Of the 3, then, only the middle one is added and that one becomes the new anchor for the incremental search.  With the assumption that there may be as many as two
	// extraneous points, there are, at most, 6 possible combinations to compare.  The largest (3-way) dot product wins.  This algorithm differs
	// from plain old "SearchTwoSided..." above because here, we allow an arbitrary number (up to the number of extra points) of leading peaks
	// that are not to be included in the final set.

	// Currently, 04/30/2008, this algorithm does not check for relative peak heights, except for the first and last nStartPts points...
	// It's in now...

	//  Test set size, just to make sure!

	int NCandidates = set.Entries ();
	int setSize = ideal.mSetSize;
  int nPoints;
	correlation = 0.0;
	int status = 0;

	if (NCandidates < setSize)
		return -1;

	RGDList tempSetRight;
	RGDList tempSetLeft;
	RGDList tempArtifactsRight;
	RGDList tempArtifactsLeft;
	ControlFitTwoSided fit (ideal.mPts, ideal.mDiffs, setSize);

	int leftIdealIndex;

// following 5 lines were moved here from other locations in the code
// because a goto was jumping over them causing an error in gcc
//  djh 4/20/09

	bool addRight = true;
	bool notLastPoint = true;
	DataSignal* currentMaxSignal = NULL;
	double currentMaxCorrelation = 0.0;
	double currentCorrelation = 0.0;


	RGDListIterator SetIterator (set);
	DataSignal* nextSignal;
	int extraPoints = set.Entries () - setSize;
//	cout << "Starting extra points, before left analysis = " << extraPoints << endl;

	int i;
	int j;

	int* selectionArray = new int [NCandidates];
	double* selectedMeans = new double [nStartPts];

	for (i=0; i<NCandidates; i++)
		selectionArray [i] = 0;

	//GridAnalysis (RGDList& signalList, const double* idealPts, const double* idealDiffs, const int* htIndices, const double* idealNorm2s, int setSize, double maxHeight, int allowedDiscrepancy, double correlationThreshold)
	GridAnalysis leftRecursion (set, ideal.mPts, ideal.mDiffs, ideal.mHeightIndices, ideal.mLeftNorm2s, setSize, maxHeight, ideal.mDiscrepancy, correlationCriterion);

	if (leftRecursion.BuildFromLeft () < 0)
		goto ErrorReturn;

	// Now move the selected signals out of set and into tempSetLeft.  Move skipped signals into tempArtifactsLeft

	i = 0;
	j = 0;
	correlation = leftRecursion.GetBestSelection (selectionArray);

	if (correlation < correlationCriterion)
		goto ErrorReturn;

	while (j < nStartPts) {

		nextSignal = (DataSignal*) set.GetFirst ();

		if (nextSignal == NULL)
			goto ErrorReturn;

		if (selectionArray [i] == 0) {

			tempArtifactsLeft.Append (nextSignal);
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
				"Peak out of place in control set: uncategorized artifact");
		}

		else {

			tempSetLeft.Append (nextSignal);
			selectedMeans [j] = nextSignal->GetMean ();
			j++;

			if (j >= nStartPts)
				break;
		}

		i++;

		if (i >= NCandidates)
			goto ErrorReturn;
	}

	extraPoints -= tempArtifactsLeft.Entries ();
	fit.SetNpointsOnLeft (selectedMeans, nStartPts);
	leftIdealIndex = nStartPts;
	nPoints = nStartPts;
//	cout << "Extra points after left fit = " << extraPoints << endl;

	//
	// Test here to see if fit all of the points.  If so, clean up and return
	//

	if (nStartPts == setSize) {

		// We're done!  Put the rest of set into artifacts and move selected peaks back into set.

		while (nextSignal = (DataSignal*) set.GetFirst ()) {

			tempArtifactsLeft.Append (nextSignal);
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
				"Peak out of place in control set: uncategorized artifact");
		}

		while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
			set.Append (nextSignal);

		MergeListAIntoListB (tempArtifactsLeft, artifacts);
		MergeListAIntoListB (tempArtifactsRight, artifacts);
		delete[] selectionArray;
		delete[] selectedMeans;
		return 0;
	}

//	//
//	//  OK...here's where we pick it up!!!  Now we test and add new points from within the middle region...
//	//  first right then left, etc., always alternating, always testing a point one sided, until the last one.
//	//  On the last one, we test the point two-sided and then we will get the overall correlation!
//	//
//
//
//	while (nPoints < setSize) {
//
//		// Now we examine at most extraPoints + 1 signals to see which is the best addition
//		// and then iteratively until tempSet has setSize entries, first right, then left, etc,
//		// alternating until the last signal.  Before the last point to be added, we use the
//		// one-sided inner product test to determine which signal to add because it is more
//		// discriminating.  On the last point, the two-sided test is definitive because the 
//		// result of the two-sided test is the total inner product that we are trying to maximize.
//
//		if (nPoints == setSize - 1)
//			notLastPoint = false;
//
//		if (addRight) {  // test signals from the right
//
//			j = 0;
//			currentMaxCorrelation = 0.0;
//			currentMaxSignal = NULL;
//			SetIterator.ResetToEnd ();
//
//			while (j <= extraPoints) {
//
//				nextSignal = (DataSignal*) SetIterator.CurrentItem ();  // this should never be null!!!  Test, though, for safety
//
//				if (nextSignal == NULL)
//					goto ErrorReturn;
//
//				// Test relative peak height here!!!
//
//				if (RecursiveInnerProduct::HeightOutsideLimit (ideal.mHeightIndices, rightIdealIndex, nextSignal->Peak (), maxHeight, ideal.mDiscrepancy))
//					currentCorrelation = 0.0;
//
//				else if (notLastPoint)
//					currentCorrelation = fit.TestNewPointOnRightOnly (nextSignal->GetMean ());
//
//				else
//					currentCorrelation = fit.TestNewPointOnRight (nextSignal->GetMean ()); // two sided test
//
//				if (currentCorrelation > currentMaxCorrelation) {
//
//					currentMaxCorrelation = currentCorrelation;
//					currentMaxSignal = nextSignal;
//				}
//
//				j++;
//				--SetIterator;
//			}
//
//			correlation = currentMaxCorrelation;
//
//			if (currentMaxSignal == NULL)
//				goto ErrorReturn;
//
//			while (true) {
//
//				nextSignal = (DataSignal*) set.GetLast ();
//
//				if (nextSignal == currentMaxSignal) {
//
//					tempSetRight.Prepend (nextSignal);
//					nPoints++;
//					fit.AddNewPointOnRight (nextSignal->GetMean ());
//					rightIdealIndex--;
//					break;
//				}
//
//				tempArtifactsRight.Prepend (nextSignal);
//				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
//					"Peak out of place in control set: uncategorized artifact");
//				extraPoints--;
//			}
//
//			addRight = false;
//		} // This is the end of the "add right" section
//
//		else {  // test signals from the left
//
//			j = 0;
//			currentMaxCorrelation = 0.0;
//			currentMaxSignal = NULL;
//			SetIterator.Reset ();
//
//			while (j <= extraPoints) {
//
//				nextSignal = (DataSignal*) SetIterator ();  // this should never be null!!!  Test, though, for safety
//
//				if (nextSignal == NULL)
//					goto ErrorReturn;
//
//				// Test relative peak height here!!!
//
//				if (RecursiveInnerProduct::HeightOutsideLimit (ideal.mHeightIndices, leftIdealIndex, nextSignal->Peak (), maxHeight, ideal.mDiscrepancy))
//					currentCorrelation = 0.0;
//
//				else if (notLastPoint)
//					currentCorrelation = fit.TestNewPointOnLeftOnly (nextSignal->GetMean ());
//
//				else
//					currentCorrelation = fit.TestNewPointOnLeft (nextSignal->GetMean ()); // two-sided test
//
//				if (currentCorrelation > currentMaxCorrelation) {
//
//					currentMaxCorrelation = currentCorrelation;
//					currentMaxSignal = nextSignal;
//				}
//
//				j++;
//			}
//
//			correlation = currentMaxCorrelation;
//
//			if (currentMaxSignal == NULL)
//				goto ErrorReturn;
//
//			while (true) {
//
//				nextSignal = (DataSignal*) set.GetFirst ();
//
//				if (nextSignal == currentMaxSignal) {
//
//					tempSetLeft.Append (nextSignal);
//					nPoints++;
//					fit.AddNewPointOnLeft (nextSignal->GetMean ());
//					leftIdealIndex++;
//					break;
//				}
//
//				tempArtifactsLeft.Append (nextSignal);
//				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
//					"Peak out of place in control set: uncategorized artifact");
//				extraPoints--;
//			}
//
//			addRight = true;
//		} // This is the end of the "add left" section
//
///*		if (extraPoints <= 0) {
//
//			while (nextSignal = (DataSignal*) set.GetLast ()) {
//
//				correlation = fit.TestNewPointOnRight (nextSignal->GetMean ());
//				fit.AddNewPointOnRight (nextSignal->GetMean ());
//				tempSetRight.Prepend (nextSignal);
//				nPoints++;
//			}
//
//			if (nPoints < setSize)
//				nPoints = setSize + 1;
//		}  */
//	}
//
//	if (extraPoints > 0) {
//
//		while (nextSignal = (DataSignal*) set.GetLast ()) {
//
//			tempArtifactsRight.Prepend (nextSignal);
//			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
//				"Peak out of place in control set: uncategorized artifact");
//		}
//
//		extraPoints = 0;
//	}
//
//	while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
//		set.Append (nextSignal);
//
//	while (nextSignal = (DataSignal*) tempSetRight.GetFirst ())
//		set.Append (nextSignal);
//
//	MergeListAIntoListB (tempArtifactsLeft, artifacts);
//	MergeListAIntoListB (tempArtifactsRight, artifacts);
//	delete[] selectionArray;
//	delete[] selectedMeans;
//
//	if (set.Entries () != setSize)
//		return -1;
//
//	return 0;

ErrorReturn:
	MergeListAIntoListB (set, artifacts);

	while (nextSignal = (DataSignal*) tempSetLeft.GetFirst ())
		set.Append (nextSignal);

	while (nextSignal = (DataSignal*) tempSetRight.GetFirst ())
		set.Append (nextSignal);

	MergeListAIntoListB (tempArtifactsLeft, artifacts);
	MergeListAIntoListB (tempArtifactsRight, artifacts);
	delete[] selectionArray;
	delete[] selectedMeans;
	return -1;
}



int PopulationMarkerSet :: PareDownSignalListBasedOnHeight (RGDList& set, RGDList& artifacts, int remainingElements) {

	RGDListIterator it (set);
	DataSignal* nextSignal;
	int size = set.Entries ();
	int size1 = size - 1;

	if (size < remainingElements)
		return 0;

	double* signalPeaks = new double [size];
	int i = 0;
	double temp;
	int j;

	while (nextSignal = (DataSignal*) it ()) {

		signalPeaks [i] = nextSignal->Peak ();
		i++;
	}

	int nChanges;

	while (true) {

		nChanges = 0;
		
		for (i=0; i<size1; i++) {

			j = i + 1;

			if (signalPeaks [i] < signalPeaks [j]) {

				temp = signalPeaks [j];
				signalPeaks [j] = signalPeaks [i];
				signalPeaks [i] = temp;
				nChanges++;
			}
		}

		if (nChanges == 0)
			break;
	}

	temp = signalPeaks [remainingElements - 1];
	delete[] signalPeaks;

	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->Peak () < temp) {

			it.RemoveCurrentItem ();
			artifacts.InsertWithNoReferenceDuplication (nextSignal);
		}
	}

	if (set.Entries () > remainingElements + 2)
		return -1;

	return 0;
}


double PopulationMarkerSet :: RecalculateMaxPeakBasedOnRelativeHeightInfo (RGDList& set, double oldMaxPeak, const int* relativeHeights, int nILSSignals) {

	RGDListIterator it (set);
	DataSignal* nextSignal;
	int size = set.Entries ();
	int size1 = size - 1;
	double* signalPeaks = new double [size];
	int i = 0;
	double temp;
	int j;
	int numberOfHighsInILS = 0;

	for (j=0; j<nILSSignals; j++) {

		if (relativeHeights [j] == 5)
			numberOfHighsInILS++;
	}

	if (numberOfHighsInILS <= 1) {

		delete[] signalPeaks;
		return oldMaxPeak;
	}

	while (nextSignal = (DataSignal*) it ()) {

		signalPeaks [i] = nextSignal->Peak ();
		i++;
	}

	int nChanges;

	while (true) {

		nChanges = 0;
		
		for (i=0; i<size1; i++) {

			j = i + 1;

			if (signalPeaks [i] < signalPeaks [j]) {

				temp = signalPeaks [j];
				signalPeaks [j] = signalPeaks [i];
				signalPeaks [i] = temp;
				nChanges++;
			}
		}

		if (nChanges == 0)
			break;
	}

	size1 = size - numberOfHighsInILS;
	int offset = numberOfHighsInILS - 1;
	double testValue = 1.25 * oldMaxPeak;

	for (i=0; i<size1; i++) {

		temp = signalPeaks [i];

		if (temp > testValue)
			continue;

		if (signalPeaks [i + offset] >= 0.8 * temp) {

			delete[] signalPeaks;
			return temp;
		}
	}

	delete[] signalPeaks;
	return oldMaxPeak;
}


double PopulationMarkerSet :: RecalculateMaxPeakBasedOnRelativeHeightInfo (RGDList& set, double oldMaxPeak, const int* relativeHeights, int nILSSignals, int maxRelativeHeight, int nTargetCurves, double& minPeak) {

	RGDListIterator it (set);
	DataSignal* nextSignal;
	int size = set.Entries ();
	int size1 = size - 1;
	int i = 0;
	double temp;
	int j;
	int numberOfHighsInILS = 0;
	minPeak = -1.0;

	if (size == nILSSignals) {

		return oldMaxPeak;
	}

	for (j=0; j<nILSSignals; j++) {

		if (relativeHeights [j] >= maxRelativeHeight)
			numberOfHighsInILS++;
	}

	if (numberOfHighsInILS <= 1) {

		return oldMaxPeak;
	}

	vector<double> signalVector;

	while (nextSignal = (DataSignal*) it ())
		signalVector.push_back (nextSignal->Peak ());

	sort (signalVector.begin (), signalVector.end (), greater<double>());  // sorted in descending order

	size1 = size - numberOfHighsInILS;
	int offset = numberOfHighsInILS - 1;
	double highFactor = ((double)(maxRelativeHeight + 1)) / (double)maxRelativeHeight;
	double lowFactor = ((double)(maxRelativeHeight - 1)) / (double)maxRelativeHeight;
	double testValue = highFactor * oldMaxPeak;
	int firstPeak = 0;
	double returnValue = oldMaxPeak;

	if (size1 == 0) {

		returnValue = oldMaxPeak;
		firstPeak = 0;
		minPeak = -1.0;
	}

	for (i=0; i<size1; i++) {

//		firstPeak = i;

		if (size - i < nILSSignals) {

			firstPeak = 0;
			returnValue = oldMaxPeak;
			break;  // can't lower maxPeak because it would leave too few signals for ILS
		}

		temp = signalVector.at (i);

		if (temp > testValue)
			continue;

		if (signalVector.at (i + offset) > lowFactor * temp) {

			firstPeak = i;
			returnValue = temp;
			break;
		}
	}

	int minLocation = firstPeak + nTargetCurves;

	if (size - firstPeak > nTargetCurves) {

		if (minLocation < size) {

			minPeak = signalVector.at (minLocation);

			if ((minLocation + 2 < size) && (signalVector.at (minLocation + 2) >= minPeak))
				minPeak = signalVector.at (minLocation - 1);
		}

		else
			minPeak = -1.0;
	}

	signalVector.clear ();
	return returnValue;
}


void PopulationMarkerSet :: SetUserLaneStandardName (const RGString& name) {

	BasePopulationMarkerSet::SetUserLaneStandardName (name);
}


Boolean PopulationMarkerSet :: BuildLocusList (const RGString& xmlString) {

	RGString XMLString (xmlString);
	RGBracketStringSearch LocusToken ("<Locus>", "</Locus>", XMLString);
	RGString LocusString;
	BaseLocus* baseLocus;
	Locus* locus;
	size_t EndPosition;
	size_t StartPosition = 0;
//	ErrorString = "";
	Boolean validity = TRUE;

	while (LocusToken.FindNextBracketedString (StartPosition, EndPosition, LocusString)) {

		StartPosition = EndPosition;
		baseLocus = mLink->GetNewLocus (LocusString);

		if (!baseLocus->IsValid ()) {

			validity = FALSE;
			ErrorString << baseLocus->GetError ();
			ErrorString << "Could not parse base locus input for Marker Set named " << mLink->GetMarkerSetName () << "\n";
//			cout << "Base locus invalid...name:  " << baseLocus->GetLocusName () << endl;
		}

		locus = new Locus (baseLocus, LocusString);

		if (!locus->IsValid ()) {

			validity = FALSE;
			ErrorString << locus->GetErrorString ();
			ErrorString << "Could not parse locus input for Marker Set named " << mLink->GetMarkerSetName () << "\n";
//			cout << ErrorString << "    Name:  " << locus->GetLocusName () << endl;
		}

		LocusListByName.Insert (locus);
		LocusList.Append (locus);
	}

	return validity;
}


LaneStandardCollection :: LaneStandardCollection () : LaneStandards (20) {

}


LaneStandardCollection :: ~LaneStandardCollection () {

	RGHashTableIterator it (LaneStandards);
	LaneStandard* next;

	while (next = (LaneStandard*)it ())
		next->ClearAndDestroy ();

	LaneStandards.ClearAndDelete ();
}


int LaneStandardCollection :: Initialize (const RGString& xmlString) {

	Valid = BuildLaneStandards (xmlString);

	if (!Valid)
		return -1;

	return 0;
}


LaneStandard* LaneStandardCollection :: GetNamedLaneStandard (const RGString& laneStandardName) {

	mTarget.SetLaneStandardName (laneStandardName);
	LaneStandard* ls = (LaneStandard*)LaneStandards.Find (&mTarget);

	if (ls == NULL)
		return NULL;

	return (LaneStandard*)ls->Copy ();
}


BaseLaneStandard* LaneStandardCollection :: GetNewBaseLaneStandard (const RGString& input) {

	return new BaseLaneStandard (input);
}


void LaneStandardCollection :: Write (RGFile& textOutput, const RGString& indent) {

	RGHashTableIterator it (LaneStandards);
	LaneStandard* std;

	while (std = (LaneStandard*) it ()) {

		std->Write (textOutput, indent);
	}
}



Boolean LaneStandardCollection :: BuildLaneStandards (const RGString& xmlString) {

	RGString TextInput (xmlString);
	RGBracketStringSearch LaneStandardToken ("<LaneStandard>", "</LaneStandard>", TextInput);
	RGString LaneString;
	BaseLaneStandard* baseStd;
	LaneStandard* std;
	size_t EndPosition;
	size_t StartPosition = 0;
	ErrorString = "";
	Boolean validity = TRUE;

	while (LaneStandardToken.FindNextBracketedString (StartPosition, EndPosition, LaneString)) {

		StartPosition = EndPosition;
		baseStd = GetNewBaseLaneStandard (LaneString);

		if (!baseStd->IsValid ()) {

			validity = FALSE;
			ErrorString = "Could not parse base internal lane standard input.  ";
		}

		std = new LaneStandard (baseStd, LaneString);

		if (!std->IsValid ()) {

			validity = FALSE;
			ErrorString += "Could not parse internal lane standard input";
		}

		LaneStandards.Insert (std);
	}

	return validity;
}



PopulationCollection :: PopulationCollection (RGFile& inputFile) : Valid (TRUE), mLaneStandardCollection (NULL) {

	TheWholeSheBang.ReadTextFile (inputFile);
}


PopulationCollection :: PopulationCollection (const RGString& inputFileName) : Valid (TRUE), mLaneStandardCollection (NULL) {

	RGFile* inputFile = new RGFile (inputFileName, "rt");

	if (!inputFile->isValid ()) {

		Valid = FALSE;
		ErrorString = "Could not open population data input file:  " + inputFileName;
	}

	else {

		TheWholeSheBang.ReadTextFile (*inputFile);
	}

	delete inputFile;
}


PopulationCollection :: PopulationCollection (const RGString& inputDirectoryName, const RGString& markerSetName) : Valid (TRUE), mLaneStandardCollection (NULL) {

	// to be continued...
	RGString fullDirectoryName = inputDirectoryName + "/LadderSpecifications/";
	RGString ilsFileName = fullDirectoryName + "ILSAndLadderInfo.xml";
	RGString gridFileName;
	RGFile ilsInputFile (ilsFileName, "rt");

	if (!ilsInputFile.isValid ()) {

		Valid = FALSE;
		ErrorString = "Could not open ILS and Ladder Info file:  " + ilsFileName;
		return;
	}

	mILSData.ReadTextFile (ilsInputFile);
	size_t startIndex = 0;
	size_t endIndex;
	size_t setStartIndex = 0;
	size_t setEndIndex;
	RGString kitString;
	RGString setsString;
	RGString singleSetString;
	RGXMLTagSearch kitsSearch ("Kits", mILSData);
	bool foundKit = false;

	if (!kitsSearch.FindNextTag (startIndex, endIndex, setsString)) {

		Valid = FALSE;
		ErrorString = "Could not find collection of kit names and files in ILS and Ladder Info, file:  " + ilsFileName;
		return;
	}

	startIndex = 0;
	RGXMLTagSearch setSearch ("Set", setsString);
	RGXMLTagSearch kitNameSearch ("KitName", singleSetString);
	RGXMLTagSearch gridFileNameSearch ("FileName", singleSetString);

	RGString kitName;
	RGString kitFileName;
	RGString fullPathGridFileName;

	while (setSearch.FindNextTag (startIndex, endIndex, singleSetString)) {

		startIndex = endIndex;
		kitNameSearch.ResetSearch ();
		gridFileNameSearch.ResetSearch ();
		setStartIndex = 0;

		if (!kitNameSearch.FindNextTag (setStartIndex, setEndIndex, kitName)) {

			Valid = FALSE;
			ErrorString = "Marker set does not have a kit name, file:  " + ilsFileName;
		}

		else if (kitName == markerSetName) {

			setStartIndex = setEndIndex;

			if (!gridFileNameSearch.FindNextTag (setStartIndex, setEndIndex, kitFileName)) {

				Valid = FALSE;
				ErrorString = "Marker set with matching name does not have a kit filename, file:  " + ilsFileName;
				return;
			}

			fullPathGridFileName = fullDirectoryName + kitFileName;
			RGFile gridFile (fullPathGridFileName, "rt");

			if (!gridFile.isValid ()) {

				Valid = FALSE;
				ErrorString = "Could not open Ladder Data file:  " + fullPathGridFileName;
				return;
			}

			mGridData.ReadTextFile (gridFile);
			Valid = TRUE;
			foundKit = true;
		}
	}

	if (!foundKit) {

		Valid = FALSE;
		ErrorString = "Could not find matching kit name, file = " + ilsFileName + " and kit name = " + markerSetName;
	}
}


PopulationCollection :: ~PopulationCollection () {

	RGHashTableIterator it (MarkerSets);
	PopulationMarkerSet* nextSet;

	while (nextSet = (PopulationMarkerSet*)it ())
		nextSet->ClearAndDestroy ();

	MarkerSets.ClearAndDelete ();
	delete mLaneStandardCollection;
}


int PopulationCollection :: Initialize () {

	int returnValue = 0;
	mLaneStandardCollection = GetNewLaneStandardCollection (TheWholeSheBang);
	Valid = BuildMarkerSets (TheWholeSheBang);

	if (!Valid)
		returnValue = -1;

	return returnValue;
}


int PopulationCollection :: InitializeFromDifferentSets () {

	int returnValue = 0;
	mLaneStandardCollection = GetNewLaneStandardCollection (mILSData);
	Valid = BuildMarkerSets (mGridData);

	if (!Valid)
		returnValue = -1;

	return returnValue;
}


PopulationMarkerSet* PopulationCollection :: GetNamedPopulationMarkerSet (const RGString& markerSetName) {

	mTargetSet.SetMarkerSetName (markerSetName);
	PopulationMarkerSet* pms = (PopulationMarkerSet*)MarkerSets.Find (&mTargetSet);

	if (pms == NULL)
		return NULL;

	return (PopulationMarkerSet*)pms->Copy ();
}


LaneStandardCollection* PopulationCollection :: GetNewLaneStandardCollection (const RGString& textInput) {

	return new LaneStandardCollection ();
}



BasePopulationMarkerSet* PopulationCollection :: GetNewPopulationMarkerSet (const RGString& input) {

	return new BasePopulationMarkerSet (input);
}


void PopulationCollection :: Write (RGFile& textOutput, const RGString& indent) {

	textOutput << indent.GetData () << "Population Marker Set Collection:\n";
	textOutput << indent.GetData () << "  Internal Lane Standards:\n";
	mLaneStandardCollection->Write (textOutput, indent);
	RGString indent2 = indent + "     ";
	PopulationMarkerSet* markerSet;
	RGHashTableIterator it (MarkerSets);

	while (markerSet = (PopulationMarkerSet*)it ())
		markerSet->Write (textOutput, indent2);
}


Boolean PopulationCollection :: BuildMarkerSets (const RGString& textInput) {

	RGString TextInput (textInput);
	RGBracketStringSearch MarkerSetToken ("<Set>", "</Set>", TextInput);
	RGString SetString;
	BasePopulationMarkerSet* baseSet;
	PopulationMarkerSet* markerSet;
	size_t EndPosition;
	size_t StartPosition = 0;
	ErrorString = "";
	Boolean validity = TRUE;
	RGString LSName;
	LaneStandard* ls;

	while (MarkerSetToken.FindNextBracketedString (StartPosition, EndPosition, SetString)) {

		StartPosition = EndPosition;
		baseSet = GetNewPopulationMarkerSet (SetString);

		if (!baseSet->IsValid ()) {

			validity = FALSE;
			ErrorString = "Population collection could not parse base marker set input.\n";
		}

		LSName = baseSet->GetLaneStandardName ();
		ls = mLaneStandardCollection->GetNamedLaneStandard (LSName);
		markerSet = new PopulationMarkerSet (baseSet, SetString);
		markerSet->SetLaneStandard (ls);

		if (!markerSet->IsValid ()) {

			validity = FALSE;
			ErrorString += "Could not parse marker set input\n";
			ErrorString << markerSet->GetErrorString ();
		}

		MarkerSets.Insert (markerSet);
	}

	if (MarkerSets.Entries () == 0) {

		validity = FALSE;
		ErrorString << "Population Collection could not find any Marker Sets\n";
	}

	return validity;
}

