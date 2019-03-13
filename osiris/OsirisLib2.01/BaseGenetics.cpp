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
*  FileName: BaseGenetics.cpp
*  Author:   Robert Goor
*
*/
//
//  Source file for classes BaseAllele, BaseLocus, BasePopulationMarkerSet...see Genetics.h and Genetics.cpp
//

#include "BaseGenetics.h"
#include "DataSignal.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgtokenizer.h"
#include "Genetics.h"
#include "coordtrans.h"
#include "ChannelData.h"
#include "CoreBioComponent.h"
#include "ParameterServer.h"


Boolean BaseAllele::SearchByName = TRUE;
bool* BaseLocus::InitialMatrix = NULL;
RGString BaseLocus::ILSFamilyName;
RGString BasePopulationMarkerSet::UserLaneStandardName;


ABSTRACT_DEFINITION (BaseAllele)
ABSTRACT_DEFINITION (BaseLocus)
ABSTRACT_DEFINITION (BasePopulationMarkerSet)
ABSTRACT_DEFINITION (BaseLaneStandard)


BaseAllele :: BaseAllele () : RGPersistent (), Curve (-1), Valid (FALSE), mRelativeHeight (-1) {

}


BaseAllele :: BaseAllele (const RGString& alleleName, int curve) : RGPersistent (),
AlleleName (alleleName), Curve (curve), Valid (TRUE), mRelativeHeight (-1) {

}


BaseAllele :: BaseAllele (const RGString& xmlInput) : RGPersistent (), Valid (TRUE) {

	RGString Input (xmlInput);
	RGBracketStringSearch NameToken ("<Name>", "</Name>", Input);
	RGBracketStringSearch CurveToken ("<CurveNo>", "</CurveNo>", Input);
	RGBracketStringSearch HeightToken ("<RelativeHeight>", "</RelativeHeight>", Input);
	RGString curveString;
	size_t FoundEnd;
	RGString heightString;

	if (!NameToken.FindNextBracketedString (0, FoundEnd, AlleleName)) {
		
		Msg += "Allele could not find Name token in string " + Input + "\n";
		Valid = FALSE;
	}

	if (!CurveToken.FindNextBracketedString (0, FoundEnd, curveString)) {

		Msg += "Allele could not find Curve Number token in string " + Input + "\n";
		Valid = FALSE;
	}

	if (!HeightToken.FindNextBracketedString (0, FoundEnd, heightString))
		mRelativeHeight = -1;

	else {

		heightString.ToUpper ();
		char first = heightString.GetFirstCharacter ();
		
		switch (first) {

			case 'H':
				mRelativeHeight = 5;
				break;

			case 'L':
				mRelativeHeight = 1;
				break;

			case 'M':
				if (heightString.Length () == 1)
					mRelativeHeight = 3;

				else if (heightString.GetLastCharacter () == 'H')
					mRelativeHeight = 4;

				else
					mRelativeHeight = 2;

				break;

			default:
				mRelativeHeight = -1;
		}
	}

	if (Valid)
		Curve = curveString.ConvertToInteger ();

	else
		Curve = -1;
}


BaseAllele :: ~BaseAllele ()  {
	
}


void BaseAllele :: SetBioID (int id) {

}


int BaseAllele :: GetBioID () const {

	return -1;
}


DataSignal* BaseAllele :: GetNewPrimarySignature (const DataSignal& ds, CoordinateTransform* trans) {

//	return new DoubleGaussian ((const DoubleGaussian&)ds, trans);
	double oldMean = ds.GetMean ();
	double newMean = trans->EvaluateWithExtrapolation (oldMean);
	return ds.MakeCopy (newMean);
}


int BaseAllele :: CompareTo (const RGPersistent* p) const {

	BaseAllele* q = (BaseAllele*)p;
	int tempID;

	if (BaseAllele::SearchByName)
		return AlleleName.CompareTo (&(q->AlleleName));

	tempID = Curve - q->Curve;

	if (tempID < 0)
		return -1;

	if (tempID == 0)
		return 0;

	return 1;
}


unsigned BaseAllele :: HashNumber (unsigned long Base) const {

	if (BaseAllele::SearchByName)
		return AlleleName.HashNumber (Base);

	return Curve%Base;
}



Boolean BaseAllele :: IsEqualTo (const RGPersistent* p) const {

	BaseAllele* q = (BaseAllele*)p;

	if (BaseAllele::SearchByName)
		return AlleleName.IsEqualTo (&(q->AlleleName));

	return (Curve == q->Curve);
}


void BaseAllele :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	AlleleName.RestoreAll (f);
	f.Read (Curve);
}


void BaseAllele :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
	AlleleName.RestoreAll (f);
	f >> Curve;
}


void BaseAllele :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	AlleleName.SaveAll (f);
	f.Write (Curve);
}


void BaseAllele :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
	AlleleName.SaveAll (f);
	f << Curve;
}



BaseLocus :: BaseLocus () : SmartMessagingObject (), Channel (-1), Valid (FALSE), mDifferenceVector (NULL), mLocusVector (NULL),
mUnnormalizedDifferenceVector (NULL), mRelativeHeights (NULL), mNormsLeft (NULL), mNormsRight (NULL) {

	InitializeSmartMessages ();
}


BaseLocus :: BaseLocus (const RGString& xmlInput) : SmartMessagingObject (), Valid (TRUE), mDifferenceVector (NULL), mLocusVector (NULL),
mUnnormalizedDifferenceVector (NULL), mRelativeHeights (NULL), mNormsLeft (NULL), mNormsRight (NULL) {

	RGString Input (xmlInput);
	RGBracketStringSearch NameToken ("<Name>", "</Name>", Input);
	RGBracketStringSearch ChannelToken ("<Channel>", "</Channel>", Input);
	RGString channelString;
	size_t FoundEnd;
	size_t StartSearch = 0;

	if (!NameToken.FindNextBracketedString (0, FoundEnd, LocusName)) {

		Msg += "Locus could not find Name token in string " + Input + "\n";
		Valid = FALSE;
	}

	if (!ChannelToken.FindNextBracketedString (0, FoundEnd, channelString)) {

		Msg += "Locus could not find Channel token in string " + Input + "\n";
		Valid = FALSE;
	}

	if (Valid)
		Channel = channelString.ConvertToInteger ();

	else
		Channel = -1;

	InitializeSmartMessages ();
}


BaseLocus :: ~BaseLocus () {

	delete[] mDifferenceVector;
	delete[] mLocusVector;
	delete[] mUnnormalizedDifferenceVector;
	delete[] mRelativeHeights;
	delete[] mNormsLeft;
	delete[] mNormsRight;
}


void BaseLocus :: SetTableLink (int linkNumber) {

	RGString temp;
	temp.Convert (linkNumber, 10);
	mTableLink = "&" + temp + "&";
}


double BaseLocus :: GetNonStandardStutterThreshold (int bp) {

	RGPDouble* threshold;

	if (bp >= 0)
		threshold = (RGPDouble*)mPositiveNonStandardStutter.GetElementAt (bp);

	else
		threshold = (RGPDouble*)mNegativeNonStandardStutter.GetElementAt (-bp);

	if (threshold == NULL)
		return -1.0;

	if (threshold->IsNullElement ())
		return -1.0;

	return threshold->GetDouble ();
}


void BaseLocus :: SetNonStandardStutterThreshold (int bp, double threshold) {

	RGPDouble* newThreshold = new RGPDouble (threshold);

	if (bp >= 0)
		mPositiveNonStandardStutter.ReplaceElementAt (bp, newThreshold);

	else
		mNegativeNonStandardStutter.ReplaceElementAt (-bp, newThreshold);
}


void BaseLocus :: SetNonStandardStutterArray (const locusSpecificNonStandardStutterStruct& limits) {

	int max = (limits.mNegativeNonStandardStutter).Length ();
	int i;
	RGPDouble* pThreshold;

	for (i=1; i<max; i++) {

		pThreshold = (RGPDouble*) limits.mNegativeNonStandardStutter.GetElementAt (i);

		if ((pThreshold != NULL) && (!pThreshold->IsNullElement ()))
			SetNonStandardStutterThreshold (-i, pThreshold->GetDouble ());
	}

	max = (limits.mPositiveNonStandardStutter).Length ();

	for (i=1; i<max; i++) {

		pThreshold = (RGPDouble*) limits.mPositiveNonStandardStutter.GetElementAt (i);

		if ((pThreshold != NULL) && (!pThreshold->IsNullElement ()))
			SetNonStandardStutterThreshold (i, pThreshold->GetDouble ());
	}
}


void BaseLocus :: SetLocusVector (RGDList& alleleList) {

	mLocusSize = alleleList.Entries ();
	mDifferenceSize = mLocusSize - 1;
	Allele* allele;
	RGDListIterator it (alleleList);
	mLocusVector = new double [mLocusSize];
	mDifferenceVector = new double [mDifferenceSize];
	mUnnormalizedDifferenceVector = new double [mDifferenceSize];
	mRelativeHeights = new int [mLocusSize];
	mNormsLeft = new double [mDifferenceSize];
	mNormsRight = new double [mDifferenceSize];
	int i = 0;
	allele = (Allele*) it ();
	bool hasHeightInfo = false;

	if (allele == NULL) {

		Valid = FALSE;
		Msg << "Locus named " << LocusName << " has no alleles\n";
		return;
	}

	double previous = (double)allele->GetBioID ();
	mRelativeHeights [0] = allele->GetRelativeHeight ();

	if (mRelativeHeights [0] > 0)
		hasHeightInfo = true;

	double current;
	double temp;
	double cumulative;
	mLocusVector [0] = previous;
	i++;
	double Norm = 0.0;

	while (allele = (Allele*) it ()) {

		current = (double)allele->GetBioID ();
		mLocusVector [i] = current;
		mDifferenceVector [i-1] = temp = current - previous;
		mUnnormalizedDifferenceVector [i-1] = temp;
		Norm += temp * temp;

		mRelativeHeights [i] = allele->GetRelativeHeight ();

		if (mRelativeHeights [i] > 0)
			hasHeightInfo = true;

		i++;
		previous = current;
	}

	if (Norm > 0.0) {

		Norm = sqrt (Norm);

		for (i=0; i<mDifferenceSize; i++)
			mDifferenceVector [i] /= Norm;
	}

	else {

		Valid = FALSE;
		Msg << "Allele difference vector for locus named " << LocusName << " has 0 norm\n";
	}

	if (!hasHeightInfo) {

		delete[] mRelativeHeights;
		mRelativeHeights = NULL;
	}

	cumulative = 0.0;

	for (i=0; i<mDifferenceSize; i++) {

		temp = mUnnormalizedDifferenceVector [i];
		cumulative += temp * temp;
		mNormsLeft [i] = cumulative;
	}

	cumulative = 0.0;

	for (i=mDifferenceSize-1; i>=0; i--) {

		temp = mUnnormalizedDifferenceVector [i];
		cumulative += temp * temp;
		mNormsRight [i] = cumulative;
	}

	mMinGridID = mLocusVector [0];
	mMaxGridID = mLocusVector [mDifferenceSize];
}


int BaseLocus :: SelectBestSubsetOfCharacteristics (RGDList& curveList, double& correlation) {
	
	// returns index of first (from 0) in contiguous list of characteristics
//	int setSize = GetCharacteristicDifferenceArray (diffArray);
	int status = PopulationMarkerSet::SelectBestSubsetOfCharacteristics (curveList, mDifferenceVector, mLocusSize, correlation);

	if (status < 0)
		Msg << "For locus named " << LocusName << ", number of characteristics found is less than expected\n";

	return status;
}


Boolean BaseLocus :: ExtractGridSignals (RGDList& channelSignalList, const LaneStandard* ls, RGDList& locusDataSignalList, ChannelData* lsData, Locus* locus) {

	Msg << "Locus:  base class called incorrectly\n";
	return FALSE;
}


RGString BaseLocus :: ReconstructAlleleName (int id, Allele* nearAllele) {

	RGString retValue;
	Msg << "Locus:  base class called incorrectly\n";
	return retValue;
}


RGString BaseLocus :: GetOffGridMessage () const {

	return "PEAK IS OFF-GRID...\n";
}


int BaseLocus :: GetMinimumBound () const {

	return -1;
}


int BaseLocus :: GetMaximumBound () const {

	return -1;
}


double BaseLocus :: GetMinGridID () const {

	return mMinGridID;
}


double BaseLocus :: GetMaxGridID () const {

	return mMaxGridID;
}


int BaseLocus :: GetCoreNumber () const {

	return 0;
}


bool BaseLocus :: AllowsNoExtension () const {

	return false;
}


BaseAllele* BaseLocus :: GetNewAllele (const RGString& input) {

	return new BaseAllele (input);
}


int BaseLocus :: CompareTo (const RGPersistent* p) const {

	BaseLocus* q = (BaseLocus*) p;
	return LocusName.CompareTo (&q->LocusName);
}



unsigned BaseLocus :: HashNumber (unsigned long Base) const {

	return LocusName.HashNumber (Base);
}



Boolean BaseLocus :: IsEqualTo (const RGPersistent* p) const {

	BaseLocus* q = (BaseLocus*) p;
	return LocusName.IsEqualTo (&q->LocusName);
}


void BaseLocus :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	LocusName.RestoreAll (f);
	f.Read (Channel);
}


void BaseLocus :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
	LocusName.RestoreAll (f);
	f >> Channel;
}


void BaseLocus :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	LocusName.SaveAll (f);
	f.Write (Channel);
}


void BaseLocus :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
	LocusName.SaveAll (f);
	f  << Channel;
}


BasePopulationMarkerSet :: BasePopulationMarkerSet () : RGPersistent (), NChannels (0), 
LSChannelNumber (0), Valid (FALSE), mChannelMap (NULL) {

}


BasePopulationMarkerSet :: BasePopulationMarkerSet (const RGString& xmlString) : RGPersistent (), NChannels (0), 
LSChannelNumber (0), Valid (TRUE), mChannelMap (NULL) {

	RGString Input (xmlString);
	RGBracketStringSearch NameToken ("<Name>", "</Name>", Input);
	RGBracketStringSearch NChannelToken ("<NChannels>", "</NChannels>", Input);
	RGBracketStringSearch LSNameToken ("<LSName>", "</LSName>", Input);
	RGBracketStringSearch LSChannelNoToken ("<ChannelNo>", "</ChannelNo>", Input);
	RGBracketStringSearch FileNameSuffixSearch ("<FileNameSuffix>", "</FileNameSuffix>", Input);
	RGBracketStringSearch GenotypeSuffixSearch ("<GenotypeSuffix>", "</GenotypeSuffix>", Input);
	RGBracketStringSearch DirectorySearchStringSearch ("<DirectorySearchString>", "</DirectorySearchString>", Input);

	RGString channelMapString;
	RGString allOLAccepted;
	RGBracketStringSearch FsaChannelMapSearch ("<FsaChannelMap>", "</FsaChannelMap>", Input);
	RGBracketStringSearch KitChannelSearch ("<KitChannelNumber>", "</KitChannelNumber>", channelMapString);
	RGBracketStringSearch FsaChannelSearch ("<fsaChannelNumber>", "</fsaChannelNumber>", channelMapString);
	RGBracketStringSearch DyeNameSearch ("<DyeName>", "</DyeName>", channelMapString);
	RGBracketStringSearch allOLAllelesAcceptedSearch ("<AllOLAllelesAccepted>", "</AllOLAllelesAccepted>", Input);
	size_t startMapSearch = 0;
	size_t endMapSearch;

	size_t FoundEnd;
	RGString ChannelString;
	size_t StartSearch = 0;
	int i;
	int index;
	int fsaIndex;
	RGString dyeName;

	if (!NameToken.FindNextBracketedString (0, FoundEnd, MarkerSetName)) {

		Valid = FALSE;
		Msg << "Population Marker Set could not find Name token in string " << Input << "\n";
	}

	if (!NChannelToken.FindNextBracketedString (0, FoundEnd, ChannelString)) {

		Msg << "Population Marker Set could not find NChannels token in string " << Input << "\n";
		Valid = FALSE;
	}

	else {

		NChannels = ChannelString.ConvertToInteger ();
		mChannelMap = new int [NChannels + 1];
		CoreBioComponent::BuildDyeNameArray (NChannels);

		for (i=0; i<=NChannels; i++)
			mChannelMap [i] = i;
	}

	LaneStandardName = BasePopulationMarkerSet::GetUserLaneStandardName ();

	if (LaneStandardName.Length () == 0) {

		if (!LSNameToken.FindNextBracketedString (0, FoundEnd, LaneStandardName)) {

			Valid = FALSE;
			Msg << "Population Marker Set could not find Internal Lane Standard Name token in string " << Input << "\n";
		}
	}

	if (!LSChannelNoToken.FindNextBracketedString (0, FoundEnd, ChannelString)) {

		Valid = FALSE;
		Msg << "Population Marker Set could not find (lane std) ChannelNo token in string " << Input << "\n";
	}

	else
		LSChannelNumber = ChannelString.ConvertToInteger ();

	if (!FileNameSuffixSearch.FindNextBracketedString (0, FoundEnd, FileNameSuffix)) {

		Msg << "Population Marker Set could not find File Name Suffix token in string " << Input << "\n";
		Valid = FALSE;
	}

	if (!GenotypeSuffixSearch.FindNextBracketedString (0, FoundEnd, GenotypeSuffix)) {

		Msg << "Population Marker Set could not find Genotype Suffix token in string " << Input << "\n";
		Valid = FALSE;
	}

	if (!DirectorySearchStringSearch.FindNextBracketedString (0, FoundEnd, DirectorySearchString)) {

		Msg << "Population Marker Set could not find Directory Search String token in string " << Input << "\n";
		Valid = FALSE;
	}

	LCMarkerSetName = MarkerSetName;
	LCMarkerSetName.ToLower ();

	if (Valid && FsaChannelMapSearch.FindNextBracketedString (0, FoundEnd, channelMapString)) {

		KitChannelSearch.ResetSearch ();
		FsaChannelSearch.ResetSearch ();
		DyeNameSearch.ResetSearch ();

		while (true) {

			if (!KitChannelSearch.FindNextBracketedString (startMapSearch, endMapSearch, ChannelString))
				break;

			startMapSearch = endMapSearch;
			index = ChannelString.ConvertToInteger ();

			if (!FsaChannelSearch.FindNextBracketedString (startMapSearch, endMapSearch, ChannelString)) {
			
				Msg << "Population Marker Set had mismatch between channel index and fsa channel index " << Input << "\n";
				Valid = FALSE;
				break;
			}

			startMapSearch = endMapSearch;
			fsaIndex = ChannelString.ConvertToInteger ();
			mChannelMap [index] = fsaIndex;

			if (!DyeNameSearch.FindNextBracketedString (startMapSearch, endMapSearch, dyeName)) {

				continue;
			}

			startMapSearch = endMapSearch;

			if (index != LSChannelNumber)
				CoreBioComponent::SetDyeName (index, dyeName);

			else if (!CoreBioComponent::GetILSDyeName ().IsEmpty ())
				CoreBioComponent::SetDyeName (index, CoreBioComponent::GetILSDyeName ());
		}
	}

	DataSignal::SetAllOLAllelesAccepted (false);

	if (allOLAllelesAcceptedSearch.FindNextBracketedString (0, FoundEnd, allOLAccepted)) {

		if (allOLAccepted == "true")
			DataSignal::SetAllOLAllelesAccepted (true);
	}

/*	if (!Valid) {

		cout << "Population marker set is invalid:" << endl;
		cout << "     Name:  " << MarkerSetName << endl;
	}*/

	// Now search LaneStandardList for LaneStandard pointer the matches LSName!!!!!!!!
}


BasePopulationMarkerSet :: ~BasePopulationMarkerSet () {

	delete[] mChannelMap;
}


Boolean BasePopulationMarkerSet :: RepresentsMarkerSet (const RGString& markerSet) const {

	RGString inputSet (markerSet);
	inputSet.ToLower ();
	return LCMarkerSetName == inputSet;
}


//	double GetTimeForCorrespondingBioID (int bioID, int channel);  // ????

int BasePopulationMarkerSet :: CompareTo (const RGPersistent* p) const {

	BasePopulationMarkerSet* q = (BasePopulationMarkerSet*) p;

	return LCMarkerSetName.CompareTo (&q->LCMarkerSetName);
}


unsigned BasePopulationMarkerSet :: HashNumber (unsigned long Base) const {

	return LCMarkerSetName.HashNumber (Base);
}


Boolean BasePopulationMarkerSet :: IsEqualTo (const RGPersistent* p) const {

	BasePopulationMarkerSet* q = (BasePopulationMarkerSet*) p;

	return LCMarkerSetName.IsEqualTo (&q->LCMarkerSetName);
}



BaseLocus* BasePopulationMarkerSet :: GetNewLocus (const RGString& input) {

	return new BaseLocus (input);
}


BaseLaneStandard :: BaseLaneStandard () : RGPersistent (), NCharacteristics (0), 
MaxCharacteristic (0.0), MinCharacteristic (0.0), Valid (TRUE) {

}



BaseLaneStandard :: BaseLaneStandard (const RGString& xmlString) : RGPersistent (), NCharacteristics (0), 
MaxCharacteristic (0.0), MinCharacteristic (0.0), mChannel (-1), Valid (TRUE) {

	RGString Input (xmlString);
	RGBracketStringSearch NameToken ("<Name>", "</Name>", Input);
	size_t FoundEnd;

	if (!NameToken.FindNextBracketedString (0, FoundEnd, Name)) {
		
		Msg << "Internal Lane Standard could not find Name token in string " << Input << "\n";
		Valid = FALSE;
	}
}


BaseLaneStandard :: ~BaseLaneStandard () {

}


int BaseLaneStandard :: GetCharacteristicArray (const double*& array) const {
	
	// returns array size or -1
	return -1;
}


int BaseLaneStandard :: GetCharacteristicDifferenceArray (const double*& array) const {
	
	// returns array size or -1
	return -1;
}


int BaseLaneStandard :: GetUnnormalizedCharacteristicDifferenceArray (const double*& array) const {

	return -1;
}


int BaseLaneStandard :: GetLargeCharacteristicArray (const double*& array) const {
	
	return -1;
}



int BaseLaneStandard :: GetLargeDifferenceArray (const double*& array) const {

	return -1;
}


double BaseLaneStandard :: GetFirstIntervalFraction () const {

	return -1.0;
}


double BaseLaneStandard :: GetSmallestIntervalFraction () const {

	return -1.0;
}


int BaseLaneStandard :: GetMaxRelativeHeight () const {

	return -1;
}


double BaseLaneStandard :: GetCorrelationAcceptanceThreshold () const {

	return 0.99;
}


double BaseLaneStandard :: GetCorrelationAutoAcceptanceThreshold () const {

	return 0.995;
}


int BaseLaneStandard :: GetNormsLeft (const double*& array) const {
	
	return -1;
}


int BaseLaneStandard :: GetNormsRight (const double*& array) const {

	return -1;
}


int BaseLaneStandard :: GetRelativeSizes (const int*& array) const {

	return -1;
}


int BaseLaneStandard :: GetOmissionArray (const double*& array) const {

	return -1;
}


int BaseLaneStandard :: CompareTo (const RGPersistent* p) const {

	BaseLaneStandard* q = (BaseLaneStandard*) p;
	return Name.CompareTo (&q->Name);
}


unsigned BaseLaneStandard :: HashNumber (unsigned long Base) const {

	return Name.HashNumber (Base);
}


Boolean BaseLaneStandard :: IsEqualTo (const RGPersistent* p) const {

	BaseLaneStandard* q = (BaseLaneStandard*) p;
	return Name.IsEqualTo (&q->Name);
}


