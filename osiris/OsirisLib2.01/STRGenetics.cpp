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
*  FileName: STRGenetics.cpp
*  Author:   Robert Goor
*
*/
//
//  STR Genetics classes, subclasses of those in Genetics.h
//

#include "STRGenetics.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "DataSignal.h"
#include "rgtokenizer.h"
#include "rgpscalar.h"
#include "coordtrans.h"
#include "ChannelData.h"
#include "Notices.h"
#include "Genetics.h"
#include "SmartNotice.h"
#include "STRSmartNotices.h"
#include <iostream>

using namespace std;


PERSISTENT_DEFINITION (STRBaseAllele, _STRBASEALLELE_, "STRBaseAllele")
PERSISTENT_DEFINITION (STRBaseLocus, _STRBASELOCUS_, "STRBaseLocus")
PERSISTENT_DEFINITION (STRBasePopulationMarkerSet, _STRBASEPOPULATIONMARKERSET_, "STRBasePopulationMarkerSet")
PERSISTENT_DEFINITION (STRBaseLaneStandard, _STRBASELANESTANDARD_, "STRBaseLaneStandard")


STRBaseAllele :: STRBaseAllele () : BaseAllele (), BP (-1) {

}


STRBaseAllele :: STRBaseAllele (const RGString& alleleName, int curve, int bp) : BaseAllele (alleleName, curve),
BP (bp) {

}


STRBaseAllele :: STRBaseAllele (const RGString& xmlInput) : BaseAllele (xmlInput) {

	RGString Input (xmlInput);
	RGBracketStringSearch BPToken ("<BP>", "</BP>", Input);
	RGString bpString;
	size_t EndPosition;

	if (!BPToken.FindNextBracketedString (0, EndPosition, bpString)) {

		BP = -1;
		Valid = FALSE;
		Msg << "Allele invalid...could not find BP string for allele named " << AlleleName << "\n";
//		cout << "Allele invalid...could not find BP string...name:  " << AlleleName << endl;
	}

	else
		BP = bpString.ConvertToInteger ();
}


STRBaseAllele :: ~STRBaseAllele () {

}

void STRBaseAllele :: SetBioID (int id) {

	BP = id;
}


int STRBaseAllele :: GetBioID () const {

	return BP;
}


DataSignal* STRBaseAllele :: GetNewPrimarySignature (const DataSignal& ds, CoordinateTransform* trans) {

//	return new DoubleGaussian ((const DoubleGaussian&)ds, trans);

	double oldMean = ds.GetMean ();
	double newMean = trans->EvaluateWithExtrapolation (oldMean);
	return ds.MakeCopy (newMean);
}


void STRBaseAllele :: RestoreAll (RGFile& f) {

	BaseAllele::RestoreAll (f);
	f.Read (BP);
}


void STRBaseAllele :: RestoreAll (RGVInStream& f) {

	BaseAllele::RestoreAll (f);
	f >> BP;
}


void STRBaseAllele :: SaveAll (RGFile& f) const {

	BaseAllele::SaveAll (f);
	f.Write (BP);
}


void STRBaseAllele :: SaveAll (RGVOutStream& f) const {

	BaseAllele::SaveAll (f);
	f << BP;
}



STRBaseLocus :: STRBaseLocus () : BaseLocus (), MinimumBP (-1), MaximumBP (-1), LowerBoundGridLSIndex (-1.0),
UpperBoundGridLSIndex (-1.0), MinimumGridTime (-1.0), MaximumGridTime (-1.0), CoreRepeatNumber (4),
LowerBoundGridLSBasePair (-1.0), UpperBoundGridLSBasePair (-1.0), mNoExtension (false) {

	//mSampleLocusSpecificStutterThreshold = Locus::GetSampleStutterThreshold ();
	//mSampleLocusSpecificAdenylationThreshold = Locus::GetSampleAdenylationThreshold ();
	//mSampleLocusSpecificFractionalFilter = Locus::GetSampleFractionalFilter ();
	//mSampleLocusSpecificPullupFractionalFilter = Locus::GetSamplePullupFractionalFilter ();
	//mSampleLocusSpecificHeterozygousImbalanceThreshold = Locus::GetHeterozygousImbalanceLimit ();
	//mSampleLocusSpecificMinBoundForHomozygote = Locus::GetMinBoundForHomozygote ();

	//mLadderLocusSpecificStutterThreshold = Locus::GetGridStutterThreshold ();
	//mLadderLocusSpecificAdenylationThreshold = Locus::GetGridAdenylationThreshold ();
	//mLadderLocusSpecificFractionalFilter = Locus::GetGridFractionalFilter ();
	//mLadderLocusSpecificPullupFractionalFilter = Locus::GetGridPullupFractionalFilter ();

}


STRBaseLocus :: STRBaseLocus (const RGString& xmlInput) : BaseLocus (xmlInput), LowerBoundGridLSIndex (-1.0),
UpperBoundGridLSIndex (-1.0), MinimumGridTime (-1.0), MaximumGridTime (-1.0), LowerBoundGridLSBasePair (-1.0), 
UpperBoundGridLSBasePair (-1.0), mNoExtension (false) {

	mSampleLocusSpecificStutterThreshold = Locus::GetSampleStutterThreshold ();
	mSampleLocusSpecificPlusStutterThreshold = Locus::GetSamplePlusStutterThreshold ();
	mSampleLocusSpecificAdenylationThreshold = Locus::GetSampleAdenylationThreshold ();
	mSampleLocusSpecificFractionalFilter = Locus::GetSampleFractionalFilter ();
	mSampleLocusSpecificPullupFractionalFilter = Locus::GetSamplePullupFractionalFilter ();
	mSampleLocusSpecificHeterozygousImbalanceThreshold = Locus::GetHeterozygousImbalanceLimit ();
	mSampleLocusSpecificMinBoundForHomozygote = Locus::GetMinBoundForHomozygote ();

	mLadderLocusSpecificStutterThreshold = Locus::GetGridStutterThreshold ();
	mLadderLocusSpecificAdenylationThreshold = Locus::GetGridAdenylationThreshold ();
	mLadderLocusSpecificFractionalFilter = Locus::GetGridFractionalFilter ();
	mLadderLocusSpecificPullupFractionalFilter = Locus::GetGridPullupFractionalFilter ();

	RGString Input (xmlInput);
	RGBracketStringSearch MinToken ("<MinBP>", "</MinBP>", Input);
	RGBracketStringSearch MaxToken ("<MaxBP>", "</MaxBP>", Input);
	RGBracketStringSearch LowerGridIndex ("<MinGridLSIndex>", "</MinGridLSIndex>", Input);
	RGBracketStringSearch UpperGridIndex ("<MaxGridLSIndex>", "</MaxGridLSIndex>", Input);
	RGBracketStringSearch CoreRepeatSearch ("<CoreRepeatNumber>", "</CoreRepeatNumber>", Input);
	RGBracketStringSearch LowerGridBasePair ("<MinGridLSBasePair>", "</MinGridLSBasePair>", Input);
	RGBracketStringSearch UpperGridBasePair ("<MaxGridLSBasePair>", "</MaxGridLSBasePair>", Input);

	RGBracketStringSearch NoExtensionSearch ("<NoExtension>", "</NoExtension>", Input);

	RGXMLTagSearch yLinkedSearch ("YLinked", Input);
	RGXMLTagSearch qualityLocusSearch ("QualityLocus", Input);
	RGXMLTagSearch maxExpectedAllelesSearch ("MaxExpectedAlleles", Input);
	RGXMLTagSearch minExpectedAllelesSearch ("MinExpectedAlleles", Input);
	size_t EndPosition;
	RGString BPString;
	RGString extString;
	bool lowerBoundFound = false;
	bool upperBoundFound = false;
	RGString trueString ("true");
	RGString familyName = BaseLocus::GetILSFamilyName ();
	//cout << "Current Family Name = " << familyName.GetData () << endl;  // This was a test to make sure the family name is known at this point.  Test succeeded 12/31/2015.
	bool useILSFamilies = PopulationCollection::UseILSFamiliesInLadderFile ();  // if true, use families; otherwise, use the old way.
	bool isValid = true;
	size_t StartPosition = 0;

	if (NoExtensionSearch.FindNextBracketedString (0, EndPosition, extString)) {

		if (extString.FindNextSubstringCaseIndependent (0, trueString, EndPosition))
			mNoExtension = true;

		else
			mNoExtension = false;
	}

	if (!MinToken.FindNextBracketedString (0, EndPosition, BPString)) {

		Valid = FALSE;
		Msg << "Locus named " << LocusName << " could not find Minimum Base Pair token\n";
		MinimumBP = -1;
	}

	else
		MinimumBP = BPString.ConvertToInteger ();

	if (!MaxToken.FindNextBracketedString (0, EndPosition, BPString)) {

		Valid = FALSE;
		Msg << "Locus named " << LocusName << " could not find Maximum Base Pair token\n";
		MaximumBP = -1;
	}

	else
		MaximumBP = BPString.ConvertToInteger ();

	if (LowerGridIndex.FindNextBracketedString (0, EndPosition, BPString)) {

		LowerBoundGridLSIndex = BPString.ConvertToDouble ();
		lowerBoundFound = true;
	}

	if (UpperGridIndex.FindNextBracketedString (0, EndPosition, BPString)) {

		UpperBoundGridLSIndex = BPString.ConvertToDouble ();
		upperBoundFound = true;
	}

	// test for ladder with ILS Family names here and, if found, use family name instead of the following four if's *******

	if (useILSFamilies) {

		isValid = GetLadderSearchRegion (StartPosition, Input, familyName);

		if (!isValid) {

			Valid = FALSE;
			Msg << "Locus named " << LocusName << " could not find family-based search region\n";
		}
	}

	else {

		if (LowerGridBasePair.FindNextBracketedString (0, EndPosition, BPString)) {

			LowerBoundGridLSBasePair = BPString.ConvertToDouble ();
			lowerBoundFound = true;
		}

		if (UpperGridBasePair.FindNextBracketedString (0, EndPosition, BPString)) {

			UpperBoundGridLSBasePair = BPString.ConvertToDouble ();
			upperBoundFound = true;
		}

		if (!lowerBoundFound) {

			Valid = FALSE;
			Msg << "Locus named " << LocusName << " could not find Minimum Grid Internal Lane Standard token, either index or base pair\n";
		}

		if (!upperBoundFound) {

			Valid = FALSE;
			Msg << "Locus named " << LocusName << " could not find Maximum Grid Internal Lane Standard token, either index or base pair\n";
		}
	}

	if (!CoreRepeatSearch.FindNextBracketedString (0, EndPosition, BPString))
		CoreRepeatNumber = 4;

	else
		CoreRepeatNumber = BPString.ConvertToInteger ();

	if (!yLinkedSearch.FindNextTag (0, EndPosition, BPString))
		mIsYLinked = false;

	else {

		if (BPString == "false")
			mIsYLinked = false;

		else
			mIsYLinked = true;
	}

	if (!qualityLocusSearch.FindNextTag (0, EndPosition, BPString))
		mIsQualityLocus = false;

	else {

		if (BPString == "false")
			mIsQualityLocus = false;

		else
			mIsQualityLocus = true;
	}

	if (!maxExpectedAllelesSearch.FindNextTag (0, EndPosition, BPString))
		mMaxExpectedAlleles = 2;

	else
		mMaxExpectedAlleles = BPString.ConvertToInteger ();

	if (!minExpectedAllelesSearch.FindNextTag (0, EndPosition, BPString))
		mMinExpectedAlleles = 1;

	else
		mMinExpectedAlleles = BPString.ConvertToInteger ();

	if (!Valid) {

		Msg << "XML Input:\n" << Input << "\n";
	}
}


STRBaseLocus :: ~STRBaseLocus () {

}


bool STRBaseLocus :: GetLadderSearchRegion (size_t& startIndex, RGString& input, const RGString& familyName) {

	RGBracketStringSearch searchRegionsSearch ("<SearchRegions>", "</SearchRegions>", input);
	RGString searchRegion;
	RGString searchDetails;
	RGBracketStringSearch individualRegionsSearch ("<Region>", "</Region>", searchRegion);
	RGBracketStringSearch familyNameSearch ("<ILSName>", "</ILSName>", searchDetails);
	RGBracketStringSearch minILSGridSearch ("<MinGrid>", "</MinGrid>", searchDetails);
	RGBracketStringSearch maxILSGridSearch ("<MaxGrid>", "</MaxGrid>", searchDetails);
	size_t start = startIndex;
	size_t end;
	size_t individualEnd;
	bool foundData = false;
	RGString searchName;
	RGString number;

	if (!searchRegionsSearch.FindNextBracketedString (start, end, searchRegion))
		return false;

	startIndex = end;
	individualRegionsSearch.ResetSearch ();
	start = 0;

	while (individualRegionsSearch.FindNextBracketedString (start, end, searchDetails)) {

		familyNameSearch.ResetSearch ();

		if (!familyNameSearch.FindNextBracketedString (0, individualEnd, searchName))
			return false;

		if (searchName == familyName) {

			minILSGridSearch.ResetSearch ();
			maxILSGridSearch.ResetSearch ();

			if (!minILSGridSearch.FindNextBracketedString (0, individualEnd, number))
				return false;

			LowerBoundGridLSBasePair = number.ConvertToDouble ();

			if (!maxILSGridSearch.FindNextBracketedString (0, individualEnd, number))
				return false;

			UpperBoundGridLSBasePair = number.ConvertToDouble ();
			return true;
		}

		start = end;
	}

	return false;
}


int STRBaseLocus :: GetMinimumBound () const {

	return MinimumBP;
}


int STRBaseLocus :: GetMaximumBound () const {

	return MaximumBP;
}


Boolean STRBaseLocus :: ExtractGridSignals (RGDList& channelSignalList, const LaneStandard* ls, RGDList& locusDataSignalList, ChannelData* lsData, Locus* locus) {

	int lsSize = ls->GetNumberOfCharacteristics ();
	int i1;
	int i2;
	double c1;
	double c2;
	Notice* newNotice;

	if (lsSize < 0) {

		Msg << "Number of characteristics not available for Locus named " << LocusName << "\n";
		newNotice = new SetupErrorNumberPeaksUnavailableForLocus;
		locus->AddNoticeToList (newNotice);
		return FALSE;
	}

	if (LowerBoundGridLSBasePair < 0.0) {

		i1 = (int) floor (LowerBoundGridLSIndex);

		if (i1 < 0) {

			Msg << "Internal Lane Standard lower bound index for Grid is out of bounds for Locus named " << LocusName << "\n";
			return FALSE;
		}

		i2 = i1 + 1;
		c2 = LowerBoundGridLSIndex - i1;
		c1 = 1.0 - c2;
		MinimumGridTime = c1 * ls->GetLaneStandardTimeForCharacteristicNumber (i1) + 
			c2 * ls->GetLaneStandardTimeForCharacteristicNumber (i2);
//		cout << "Not using base pairs for minimum of locus " << LocusName << endl;
	}

	else
		MinimumGridTime = lsData->GetTimeForSpecifiedID (LowerBoundGridLSBasePair);

	if (UpperBoundGridLSBasePair < 0.0) {

		i1 = (int) floor (UpperBoundGridLSIndex);

		if (i1 < 0) {

			Msg << "Internal Lane Standard upper bound index for Grid is out of bounds for Locus named " << LocusName << "\n";
			return FALSE;
		}

		i2 = i1 + 1;
		c2 = UpperBoundGridLSIndex - i1;
		c1 = 1.0 - c2;
		MaximumGridTime = c1 * ls->GetLaneStandardTimeForCharacteristicNumber (i1) + 
			c2 * ls->GetLaneStandardTimeForCharacteristicNumber (i2);
//		cout << "Not using base pairs for maximum of locus " << LocusName << endl;
	}

	else
		MaximumGridTime = lsData->GetTimeForSpecifiedID (UpperBoundGridLSBasePair);

	RGDListIterator it (channelSignalList);
	DataSignal* nextSignal;
	double nextMean;
	locusDataSignalList.Clear ();

	// Assume channelSignalList is in increasing order by signal means

	while (nextSignal = (DataSignal*) it ()) {

		nextMean = nextSignal->GetMean ();

		if (nextMean >= MinimumGridTime) {

			if (nextMean <= MaximumGridTime) {

				it.RemoveCurrentItem ();
				locusDataSignalList.Append (nextSignal);
			}

			else
				break;
		}
	}

	if (locusDataSignalList.Entries () < mLocusSize) {

		Msg << "Locus named " << LocusName << " could not find the required number of unanalyzed peaks in range\n";
		newNotice = new LocusHasTooFewPeaks;
		locus->AddNoticeToList (newNotice);
		return FALSE;
	}

	return TRUE;
}


RGString STRBaseLocus :: ReconstructAlleleName (int id, Allele* nearAllele) {

	int difference = id - nearAllele->GetBioID ();
	RGString nearName = nearAllele->GetAlleleName ();

	if (difference == 0)
		return nearName;

	int KK = CoreRepeatNumber;
	double nearDouble;
	int nearInt;
	int temp;
	RGString intString;
	RGString fractString;
	RGString returnValue;
	nearDouble = nearName.ConvertToDouble ();
	nearInt = nearName.ConvertToInteger ();
	temp = (int) floor (10 * (nearDouble - nearInt) + 0.5);
	// temp is fractional part...should be integer mod 4

	if (difference > 0) {

		nearInt += (difference / KK);
		temp += difference%KK;
		nearInt += (temp / KK);
		temp = temp%KK;
	}

	else {

		difference = - difference;
		nearInt -= (difference / KK);
		temp -= difference%KK;

		if (temp < 0) {

			temp += KK;
			nearInt--;
		}
	}

	if (nearInt < 1) {

		returnValue = "$";
		return returnValue;
	}

	intString.Convert (nearInt, 10);

	if (temp == 0)
		return intString;

	fractString.Convert (temp, 10);
	returnValue = intString + "." + fractString;
	return returnValue;
}


RGString STRBaseLocus :: GetOffGridMessage () const {

	return "PEAK IS OFF-LADDER...\n";
}


int STRBaseLocus :: GetCoreNumber () const {

	return CoreRepeatNumber;
}


bool STRBaseLocus :: AllowsNoExtension () const {

	return mNoExtension;
}


void STRBaseLocus :: RestoreAll (RGFile& f) {

	BaseLocus::RestoreAll (f);
	f.Read (MinimumBP);
	f.Read (MaximumBP);
}


void STRBaseLocus :: RestoreAll (RGVInStream& f) {

	BaseLocus::RestoreAll (f);
	f >> MinimumBP;
	f >> MaximumBP;
}


void STRBaseLocus :: SaveAll (RGFile& f) const {

	BaseLocus::SaveAll (f);
	f.Write (MinimumBP);
	f.Write (MaximumBP);
}


void STRBaseLocus :: SaveAll (RGVOutStream& f) const {

	BaseLocus::SaveAll (f);
	f << MinimumBP;
	f << MaximumBP;
}



BaseAllele* STRBaseLocus :: GetNewAllele (const RGString& input) {

	return new STRBaseAllele (input);
}


STRBasePopulationMarkerSet :: STRBasePopulationMarkerSet () : BasePopulationMarkerSet () {

}


STRBasePopulationMarkerSet :: STRBasePopulationMarkerSet (const RGString& xmlString) :
BasePopulationMarkerSet (xmlString) {

}


STRBasePopulationMarkerSet :: ~STRBasePopulationMarkerSet () {

}


BaseLocus* STRBasePopulationMarkerSet :: GetNewLocus (const RGString& input) {

	return new STRBaseLocus (input);
}


STRPopulationCollection :: STRPopulationCollection (RGFile& inputFile) : PopulationCollection (inputFile) {

}


STRPopulationCollection :: STRPopulationCollection (const RGString& inputFileName) : PopulationCollection (inputFileName) {

	if (PopulationCollection::Initialize () < 0)
		Valid = FALSE;
}


STRPopulationCollection :: STRPopulationCollection (const RGString& inputDirectoryName, const RGString& markerSetName) : PopulationCollection (inputDirectoryName, markerSetName) {

	if (Valid) {

		if (PopulationCollection::InitializeFromDifferentSets () < 0)
			Valid = FALSE;
	}
}


STRPopulationCollection :: ~STRPopulationCollection () {

}


BasePopulationMarkerSet* STRPopulationCollection :: GetNewPopulationMarkerSet (const RGString& input) {

	return new STRBasePopulationMarkerSet (input);
}



LaneStandardCollection* STRPopulationCollection :: GetNewLaneStandardCollection (const RGString& textInput) {

	return new STRLaneStandardCollection (textInput);
}


/*
int mLastHalfSize;
	double* mLastHalfCharacteristicArray;
	double* mLastHalfDifferenceArray;
	int* mLastHalfRelativeSizes;
	double* mLastHalfNormsLeft;
	double* mLastHalfNormsRight;
	int mNumberOfLargePeaks;
	double* mLargeCharacteristicArray;
	double* mLargeDifferenceArray;
	int mNumberOfLargeDifferences;
	double mFirstIntervalFraction;
	double mSmallestIntervalFraction;
	int mMaxRelativeHeight;
	*/

STRBaseLaneStandard :: STRBaseLaneStandard () : BaseLaneStandard (), mCharacteristicArray (NULL), 
mDifferenceArray (NULL), mUnnormalizedDifferenceArray(0), 
mNormsLeft (NULL), mNormsRight (NULL), mRelativeSizes (NULL), mSize (0), mDifferenceSize (0), mOmissionSize (0),
mOmissionArray (NULL), mNumberOfLargePeaks (0), mLargeCharacteristicArray (NULL), mLargeDifferenceArray (NULL),
mNumberOfLargeDifferences (0), mFirstIntervalFraction (-1.0), mSmallestIntervalFraction (-1.0), mMaxRelativeHeight (-1),
mCorrelationAcceptanceThreshold (0.993), mCorrelationAutoAcceptanceThreshold (0.9975)
//
//, mLastHalfSize (0), mLastHalfCharacteristicArray (NULL), mLastHalfUnnormalizedDifferenceArray (NULL),
//mLastHalfRelativeSizes (NULL), mLastHalfDifferenceSize (0), mLastHalfNormsLeft (NULL), mLastHalfNormsRight (NULL) 

{

}


STRBaseLaneStandard :: STRBaseLaneStandard (const RGString& xmlString) : BaseLaneStandard (xmlString),
mCharacteristicArray (NULL), mDifferenceArray (NULL), mUnnormalizedDifferenceArray(0), 
mNormsLeft (NULL), mNormsRight (NULL), mRelativeSizes (NULL), 
mSize (0), mDifferenceSize (0), mOmissionSize (0), mOmissionArray (NULL), mNumberOfLargePeaks (0), 
mLargeCharacteristicArray (NULL), mLargeDifferenceArray (NULL), mNumberOfLargeDifferences (0), 
mFirstIntervalFraction (-1.0), mSmallestIntervalFraction (-1.0), mMaxRelativeHeight (-1),
mCorrelationAcceptanceThreshold (0.993), mCorrelationAutoAcceptanceThreshold (0.9975)
//
//, mLastHalfSize (0), mLastHalfCharacteristicArray (NULL), mLastHalfUnnormalizedDifferenceArray (NULL),
//mLastHalfRelativeSizes (NULL), mLastHalfDifferenceSize (0), mLastHalfNormsLeft (NULL), mLastHalfNormsRight (NULL)
{

	RGString TextInput (xmlString);
	RGBracketStringSearch CharacteristicsToken ("<Characteristics>", "</Characteristics>", TextInput);
	RGString CharacteristicsString;
	RGString HeightString;
	RGBracketStringSearch HeightToken ("<RelativeHeights>", "</RelativeHeights>", TextInput);
	RGBracketStringSearch OmissionSearch ("<Omissions>", "</Omissions>", TextInput);
	RGBracketStringSearch AcceptanceThresholdSearch ("<CorrelationAcceptanceThreshold>", "</CorrelationAcceptanceThreshold>", TextInput);
	RGBracketStringSearch AutoAcceptanceThresholdSearch ("<CorrelationAutoAcceptanceThreshold>", "</CorrelationAutoAcceptanceThreshold>", TextInput);
	RGString OmissionString;
	size_t endPosition;
	RGString token;
	RGString delim;
	RGString numberString;

	double minChar;
	double maxChar;
	RGDList tokens;
	double value;
	RGPDouble* valuePtr;

	double previous;
	double next;
	double cumulative;
	int i = 0;
	double temp;
	int k;
	int j;

	if (!CharacteristicsToken.FindNextBracketedString (0, endPosition, CharacteristicsString)) {

		Msg << "Internal Lane Standard named " << Name << " could not find Characteristics token from string\n";
		Msg << TextInput << "\n";
		Valid = FALSE;
	}

	else {

		RGStringTokenizer data (CharacteristicsString);
		data.AddDelimiter (' ');
		data.AddDelimiter ("  ");
		data.AddDelimiter ("   ");
		data.AddDelimiter ("    ");
		data.AddDelimiter ("     ");
		data.AddRemoveItem (',');
		data.AddDelimiter ('\t');
		data.AddDelimiter ('\n');

		while (data.NextToken (token, delim)) {

			if (token.Length () > 0) {

				value = token.ConvertToDouble ();
				valuePtr = new RGPDouble (value);

				if (tokens.Entries () == 0) {

					minChar = maxChar = value;
					tokens.Append (valuePtr);
				}

				else if (value < minChar) {

					minChar = value;
					tokens.Prepend (valuePtr);
				}

				else if (value > maxChar) {

					maxChar = value;
					tokens.Append (valuePtr);
				}

				else
					tokens.Insert (valuePtr);
			}
		}

		mSize = (int)tokens.Entries ();
		NCharacteristics = mSize;
		mCharacteristicArray = new double [mSize];
		mDifferenceSize = mSize - 1;
		mDifferenceArray = new double [mDifferenceSize];
		mUnnormalizedDifferenceArray = new double [mDifferenceSize];
		mNormsLeft = new double [mDifferenceSize];
		mNormsRight = new double [mDifferenceSize];

		//mLastHalfSize = mSize / 2;

		//if (2 * mLastHalfSize < mSize)
		//	mLastHalfSize += 1;

		//mLastHalfDifferenceSize = mLastHalfSize - 1;
		//mLastHalfCharacteristicArray = new double [mLastHalfSize];
		//mLastHalfUnnormalizedDifferenceArray = new double [mLastHalfDifferenceSize];
		//mLastHalfNormsLeft = new double [mLastHalfDifferenceSize];
		//mLastHalfNormsRight = new double [mLastHalfDifferenceSize];

		while (valuePtr = (RGPDouble*)tokens.GetFirst ()) {

			mCharacteristicArray [i] = valuePtr->GetDouble ();
			delete valuePtr;
			i++;
		}

		//int displacement = mSize - mLastHalfSize;	// +0, +1, -1?????

		//for (i=0; i<mLastHalfSize; i++)
		//	mLastHalfCharacteristicArray [i] = mCharacteristicArray [i + displacement];

		previous = mCharacteristicArray [0];
		
		for (i=0; i<mDifferenceSize; i++) {

			next = mCharacteristicArray [i+1];
			mUnnormalizedDifferenceArray [i] = mDifferenceArray [i] = next - previous;
			previous = next;
		}

		//for (i=0; i<mLastHalfSize; i++)
		//	mLastHalfUnnormalizedDifferenceArray [i] = mUnnormalizedDifferenceArray [i + displacement];

		double norm = 0.0;

		for (i=0; i<mDifferenceSize; i++)
			norm += mDifferenceArray [i] * mDifferenceArray [i];

		norm = sqrt (norm);

		if (norm > 0.0) {

			for (i=0; i<mDifferenceSize; i++)
				mDifferenceArray [i] /= norm;
		}

		cumulative = 0.0;

		for (i=0; i<mDifferenceSize; i++) {

			temp = mUnnormalizedDifferenceArray [i];
			cumulative += temp * temp;
			mNormsLeft [i] = cumulative;
		}

		cumulative = 0.0;

		for (i=mDifferenceSize-1; i>=0; i--) {

			temp = mUnnormalizedDifferenceArray [i];
			cumulative += temp * temp;
			mNormsRight [i] = cumulative;
		}

		//cumulative = 0.0;

		//for (i=0; i<mLastHalfDifferenceSize; i++) {

		//	temp = mLastHalfUnnormalizedDifferenceArray [i];
		//	cumulative += temp * temp;
		//	mLastHalfNormsLeft [i] = cumulative;
		//}

		//cumulative = 0.0;

		//for (i=mLastHalfDifferenceSize-1; i>=0; i--) {

		//	temp = mLastHalfUnnormalizedDifferenceArray [i];
		//	cumulative += temp * temp;
		//	mLastHalfNormsRight [i] = cumulative;
		//}

		MaxCharacteristic = maxChar;
		MinCharacteristic = minChar;
		double width = mCharacteristicArray [mSize - 1] - mCharacteristicArray [0];

		if (width > 0.0)
			mFirstIntervalFraction = mUnnormalizedDifferenceArray [0] / width;

		else
			mFirstIntervalFraction = 1.0;

		double temp = mUnnormalizedDifferenceArray [0];
		double value;

		for (i=0; i<mDifferenceSize; i++) {

			value = mUnnormalizedDifferenceArray [i];

			if (value < temp)
				temp = value;
		}

		if (width > 0.0)
			mSmallestIntervalFraction = temp / width;

		else
			mSmallestIntervalFraction = 1.0;

		if (!HeightToken.FindNextBracketedString (0, endPosition, HeightString)) {

			mRelativeSizes = NULL;
//			mLastHalfRelativeSizes = NULL;
		}

		else {

			RGStringTokenizer data2 (HeightString);
			data2.AddDelimiter (' ');
			data2.AddDelimiter ("  ");
			data2.AddDelimiter ("   ");
			data2.AddDelimiter ("    ");
			data2.AddDelimiter ("     ");
			data2.AddRemoveItem (',');
			data2.AddDelimiter ('\t');
			data2.AddDelimiter ('\n');

			mRelativeSizes = new int [mSize];
	    
			for (k=0; k<mSize; k++)
				mRelativeSizes [k] = -1;

			k = 0;

			while (data2.NextToken (token, delim)) {

				if (token.Length () > 0) {

					token.ToUpper ();
					char first = token.GetFirstCharacter ();
					
					switch (first) {

						case 'H':
							mRelativeSizes [k] = 5;
							break;

						case 'L':
							mRelativeSizes [k] = 1;
							break;

						case 'M':
							if (token.Length () == 1)
								mRelativeSizes [k] = 3;

							else if (token.GetLastCharacter () == 'H')
								mRelativeSizes [k] = 4;

							else
								mRelativeSizes [k] = 2;

							break;

						default:
							mRelativeSizes [k] = -1;
					}
				}

				k++;

				if (k >= mSize)
					break;
			}

			//mLastHalfRelativeSizes = new int [mLastHalfSize];

			//for (i=0; i<mLastHalfSize; i++)
			//	mLastHalfRelativeSizes [i] = mRelativeSizes [i + displacement];

			int maxRelativeSize = 0;
			int relativeSize;

			for (j=0; j<mSize; j++) {

				relativeSize = mRelativeSizes [j];

				if (relativeSize > maxRelativeSize)
					maxRelativeSize = relativeSize;
			}

			int numberOfHighsInILS = 0;
			mMaxRelativeHeight = maxRelativeSize;

			for (j=0; j<mSize; j++) {

				if (mRelativeSizes [j] == maxRelativeSize)
					numberOfHighsInILS++;
			}

			mNumberOfLargePeaks = numberOfHighsInILS;
			mNumberOfLargeDifferences = numberOfHighsInILS - 1;
			mLargeCharacteristicArray = new double [numberOfHighsInILS];

			if (mNumberOfLargeDifferences > 0)
				mLargeDifferenceArray = new double [mNumberOfLargeDifferences];

			k = 0;
			
			for (j=0; j<mSize; j++) {

				if (mRelativeSizes [j] == maxRelativeSize) {

					mLargeCharacteristicArray [k] = mCharacteristicArray [j];
					k++;
				}
			}

			if (mLargeDifferenceArray != NULL) {

				for (j=0; j<mNumberOfLargeDifferences; j++)
					mLargeDifferenceArray [j] = mLargeCharacteristicArray [j + 1] - mLargeCharacteristicArray [j];
			}
		}

		if (OmissionSearch.FindNextBracketedString (0, endPosition, OmissionString)) {

			RGStringTokenizer omissionData (OmissionString);
			omissionData.AddDelimiter (' ');
			omissionData.AddDelimiter ("  ");
			omissionData.AddDelimiter ("   ");
			omissionData.AddDelimiter ("    ");
			omissionData.AddDelimiter ("     ");
			omissionData.AddRemoveItem (',');
			omissionData.AddDelimiter ('\t');
			omissionData.AddDelimiter ('\n');

			while (omissionData.NextToken (token, delim)) {

				if (token.Length () > 0) {

					value = token.ConvertToDouble ();
					valuePtr = new RGPDouble (value);

					if (tokens.Entries () == 0) {

						minChar = maxChar = value;
						tokens.Append (valuePtr);
					}

					else if (value < minChar) {

						minChar = value;
						tokens.Prepend (valuePtr);
					}

					else if (value > maxChar) {

						maxChar = value;
						tokens.Append (valuePtr);
					}

					else
						tokens.Insert (valuePtr);
				}
			}

			mOmissionSize = (int)tokens.Entries ();

			if (mOmissionSize == 0)
				return;

			mOmissionArray = new double [mOmissionSize];

			i = 0;

			while (valuePtr = (RGPDouble*)tokens.GetFirst ()) {

				mOmissionArray [i] = valuePtr->GetDouble ();
				delete valuePtr;
				i++;
			}
		}
	}

	cout << "ILS Name = " << Name.GetData () << endl;

	if (!AcceptanceThresholdSearch.FindNextBracketedString (0, endPosition, numberString))
		cout << "\tCould not find ILS acceptance threshold...using " << mCorrelationAcceptanceThreshold << endl;

	else {

		mCorrelationAcceptanceThreshold = numberString.ConvertToDouble ();
		cout << "\tFrom ILS settings...ILS acceptance threshold = " << mCorrelationAcceptanceThreshold << endl;
	}

	if (!AutoAcceptanceThresholdSearch.FindNextBracketedString (0, endPosition, numberString))
		cout << "\tCould not find ILS auto acceptance threshold...using default value = " << mCorrelationAutoAcceptanceThreshold << endl << endl;

	else {

		mCorrelationAutoAcceptanceThreshold = numberString.ConvertToDouble ();
		cout << "\tFrom ILS settings...ILS auto acceptance threshold = " << mCorrelationAutoAcceptanceThreshold << endl << endl;
	}
}



STRBaseLaneStandard :: ~STRBaseLaneStandard () {

	delete[] mCharacteristicArray;
	delete[] mDifferenceArray;
	delete[] mUnnormalizedDifferenceArray;
	delete[] mNormsLeft;
	delete[] mNormsRight;
	delete[] mRelativeSizes;
	delete[] mOmissionArray;
	delete[] mLargeCharacteristicArray;
	delete[] mLargeDifferenceArray;

	//delete[] mLastHalfCharacteristicArray;
	//delete[] mLastHalfUnnormalizedDifferenceArray;
	//delete[] mLastHalfRelativeSizes;
	//delete[] mLastHalfNormsLeft;
	//delete[] mLastHalfNormsRight;
}


int STRBaseLaneStandard :: GetCharacteristicArray (const double*& array) const {
	
	// returns array size or -1
	array = mCharacteristicArray;

	if (mCharacteristicArray == NULL)
		return -1;
	
	return mSize;
}



int STRBaseLaneStandard :: GetCharacteristicDifferenceArray (const double*& array) const {
	
	// returns array size or -1
	array = mDifferenceArray;

	if (mDifferenceArray == NULL)
		return -1;

	return mDifferenceSize;
}


int STRBaseLaneStandard :: GetUnnormalizedCharacteristicDifferenceArray (const double*& array) const {

	// returns array size or -1
	array = mUnnormalizedDifferenceArray;

	if (mUnnormalizedDifferenceArray == NULL)
		return -1;

	return mDifferenceSize;
}


//int STRBaseLaneStandard :: GetLastHalfCharacteristicArray (const double*& array) const {
//	
//	// returns array size or -1
//	array = mLastHalfCharacteristicArray;
//
//	if (mLastHalfCharacteristicArray == NULL)
//		return -1;
//
//	return mLastHalfSize;
//}
//
//
//int STRBaseLaneStandard :: GetLastHalfUnnormalizedCharacteristicDifferenceArray (const double*& array) const {
//	
//	// returns array size or -1
//	array = mLastHalfUnnormalizedDifferenceArray;
//
//	if (mLastHalfUnnormalizedDifferenceArray == NULL)
//		return -1;
//
//	return mLastHalfDifferenceSize;
//}
//
//
//
//int STRBaseLaneStandard :: GetLastHalfRelativeSizes (const int*& array) const {
//	
//	// returns array size, or -1
//	array = mLastHalfRelativeSizes;
//
//	if (mLastHalfRelativeSizes == NULL)
//		return -1;
//
//	return mLastHalfSize;
//}
//
//
//int STRBaseLaneStandard :: GetLastHalfNormsLeft (const double*& array) const {
//	
//	array = mLastHalfNormsLeft;
//	
//	if (mNormsLeft == NULL)
//		return -1;
//
//	return mLastHalfDifferenceSize;
//}
//
//
//int STRBaseLaneStandard :: GetLastHalfNormsRight (const double*& array) const {
//
//	array = mLastHalfNormsRight;
//	
//	if (mNormsRight == NULL)
//		return -1;
//
//	return mLastHalfDifferenceSize;
//}


int STRBaseLaneStandard :: GetLargeCharacteristicArray (const double*& array) const {
	
	// returns array size or -1

	array = mLargeCharacteristicArray;

	if (array == NULL)
		return -1;

	return mNumberOfLargePeaks;
}



int STRBaseLaneStandard :: GetLargeDifferenceArray (const double*& array) const {
	
	// returns array size or -1

	array = mLargeDifferenceArray;

	if (array == NULL)
		return -1;

	return mNumberOfLargeDifferences;
}


double STRBaseLaneStandard :: GetFirstIntervalFraction () const {

	return mFirstIntervalFraction;
}



double STRBaseLaneStandard :: GetSmallestIntervalFraction () const {

	return mSmallestIntervalFraction;
}


int STRBaseLaneStandard :: GetMaxRelativeHeight () const {

	return mMaxRelativeHeight;
}


double STRBaseLaneStandard :: GetCorrelationAcceptanceThreshold () const {

	return mCorrelationAcceptanceThreshold;
}


double STRBaseLaneStandard :: GetCorrelationAutoAcceptanceThreshold () const {

	return mCorrelationAutoAcceptanceThreshold;
}



int STRBaseLaneStandard :: GetNormsLeft (const double*& array) const {
	
	array = mNormsLeft;
	
	if (mNormsLeft == NULL)
		return -1;

	return mDifferenceSize;
}


int STRBaseLaneStandard :: GetNormsRight (const double*& array) const {

	array = mNormsRight;
	
	if (mNormsRight == NULL)
		return -1;

	return mDifferenceSize;
}


int STRBaseLaneStandard :: GetRelativeSizes (const int*& array) const {

	array = mRelativeSizes;
	
	if (mRelativeSizes == NULL)
		return -1;

	return mDifferenceSize;
}


int STRBaseLaneStandard :: GetOmissionArray (const double*& array) const {

	array = mOmissionArray;
	
	if (mOmissionArray == NULL)
		return -1;

	return mOmissionSize;
}


STRLaneStandardCollection :: STRLaneStandardCollection (const RGString& xmlString) : LaneStandardCollection () {

	if (LaneStandardCollection::Initialize (xmlString) < 0)
		Valid = FALSE;
}


STRLaneStandardCollection :: ~STRLaneStandardCollection () {

}


BaseLaneStandard* STRLaneStandardCollection :: GetNewBaseLaneStandard (const RGString& input) {

	return new STRBaseLaneStandard (input);
}

