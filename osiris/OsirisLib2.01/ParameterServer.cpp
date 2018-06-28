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
*  FileName: ParameterServer.cpp
*  Author:   Robert Goor
*
*/
//
//  Maintains connection to all Osiris-defined parameters and prints them out on request
//

//
//   Required Parameters:
//       ParametricCurve::FitTolerance gives minimum inner product for matching standard deviation; default = .9995;
//       Gaussian::NumberOfSteps gives number of numerical integration steps for Gaussian integrals; default = 50;
//       NormalizedGaussian::SigmaTolerance gives distance between two standard deviation estimates for match; default = 1.5e-4;
//       Gaussian::SigmaWidth gives number of standard deviations on each side of mean for standard Gaussian and NormalizedGaussian; default = 3.5;
//       NormalizedGaussian::MaximumIterations gives the max number of iterations of Brent's algorithm for fitting standard deviation; default = 5;
//		 DoubleGaussian::DefaultSigmaRatio gives default value of ratio of sigmas for 2 Gaussians; can be overridden; default = 4.0;
//       DataSignal::SignalSpacing gives value of spacing between signal observations; default = 1.0;
//

//
//    Required Parameters:
//       TracePrequalification::windowWidth gives number of samples of width of square wave for convolution; default = 20
//       TracePrequalification::noiseThreshold gives threshold on post-convolution peaks to consider them noise; default = 400.0;
//

#include "ParameterServer.h"
#include "TracePrequalification.h"
#include "DataSignal.h"
#include "RGTextOutput.h"
#include "rgtokenizer.h"
#include "STRChannelData.h"
#include "Genetics.h"
#include "TestCharacteristic.h"
#include "STRSmartMessage.h"
#include "DirectoryManager.h"


static ParameterServerKill g_kill;

int ParameterServer :: ReferenceCount = 0;
ParameterServer* ParameterServer :: OneAndOnlySelf = NULL;


void RFULimitsStruct :: Reset () {

	minRFU = 0.0;
	fractionOfMaxPeak = 0.0;
	pullupFractionOfMaxPeak = 0.0;
	stutterThreshold = 0.0;
	plusStutterThreshold = 0.0;
	adenylationThreshold = 0.0;
}


labNonRFULimitsStruct :: labNonRFULimitsStruct () {

	heterozygousImbalanceLimit = -1.0;
	minBoundForHomozygote = -1.0;
	maxNumberPullupsPerSample = -1;
	maxNumberStutterPerSample = -1;
	maxNumberSpikesPerSample = -1;
	maxNumberAdenylationPerSample = -1;
	maxNumberOLAllelesPerSample = -1;
	maxResidualForAlleleCall = -1.0;
	minBPSForArtifacts = -1;
	alleleRFUOverloadThreshold = -1.0;
	maxNumberExcessiveResidualsPerSample = -1;
}


locusSpecificLimitsStruct :: locusSpecificLimitsStruct () : 
fractionOfMaxPeak (-1.0),
pullupFractionalFilter (-1.0),
stutterThreshold (-1.0),
stutterThresholdRight (-1.0),
plusStutterThreshold (-1.0),
plusStutterThresholdRight (-1.0), 
adenylationThreshold (-1.0),
heterozygousImbalanceLimit (-1.0),
minBoundForHomozygote (-1.0) {

}


locusSpecificLimitsStruct :: locusSpecificLimitsStruct (const locusSpecificLimitsStruct& limits) :
fractionOfMaxPeak (limits.fractionOfMaxPeak),
pullupFractionalFilter (limits.pullupFractionalFilter),
stutterThreshold (limits.stutterThreshold),
stutterThresholdRight (limits.stutterThresholdRight), 
plusStutterThreshold (limits.plusStutterThreshold),
plusStutterThresholdRight (limits.plusStutterThresholdRight),
adenylationThreshold (limits.adenylationThreshold),
heterozygousImbalanceLimit (limits.heterozygousImbalanceLimit),
minBoundForHomozygote (limits.minBoundForHomozygote) {

	locusName = limits.locusName;
}


locusSpecificLimitsStruct :: ~locusSpecificLimitsStruct () {

}


void locusSpecificLimitsStruct :: CopyPositiveThresholdsFrom (const locusSpecificLimitsStruct& limits) {

	if (limits.fractionOfMaxPeak >= 0.0)
		fractionOfMaxPeak = limits.fractionOfMaxPeak;

	if (limits.pullupFractionalFilter >= 0.0)
		pullupFractionalFilter = limits.pullupFractionalFilter;

	if (limits.stutterThreshold >= 0.0)
		stutterThreshold = limits.stutterThreshold;

	if (limits.stutterThresholdRight >= 0.0)
		stutterThresholdRight = limits.stutterThresholdRight;

	if (limits.plusStutterThreshold >= 0.0)
		plusStutterThreshold = limits.plusStutterThreshold;

	if (limits.plusStutterThresholdRight >= 0.0)
		plusStutterThresholdRight = limits.plusStutterThresholdRight;

	if (limits.adenylationThreshold >= 0.0)
		adenylationThreshold  = limits.adenylationThreshold;

	if (limits.heterozygousImbalanceLimit >= 0.0)
		heterozygousImbalanceLimit = limits.heterozygousImbalanceLimit;

	if (limits.minBoundForHomozygote >= 0.0)
		minBoundForHomozygote = limits.minBoundForHomozygote;
}


locusSpecificNonStandardStutterStruct :: locusSpecificNonStandardStutterStruct () {

}



locusSpecificNonStandardStutterStruct :: locusSpecificNonStandardStutterStruct (const locusSpecificNonStandardStutterStruct& limits) {

	int max = (limits.mNegativeNonStandardStutter).Length ();
	int i;
	RGPDouble* pThreshold;
	locusName = limits.locusName;

	for (i=1; i<max; i++) {

		pThreshold = (RGPDouble*) limits.mNegativeNonStandardStutter.GetElementAt (i);

		if (pThreshold != NULL)
			SetNonStandardStutterThreshold (-i, pThreshold->GetDouble ());
	}

	max = (limits.mPositiveNonStandardStutter).Length ();

	for (i=1; i<max; i++) {

		pThreshold = (RGPDouble*) limits.mPositiveNonStandardStutter.GetElementAt (i);

		if (pThreshold != NULL)
			SetNonStandardStutterThreshold (i, pThreshold->GetDouble ());
	}
}



locusSpecificNonStandardStutterStruct :: ~locusSpecificNonStandardStutterStruct () {

}


void locusSpecificNonStandardStutterStruct :: SetNonStandardStutterThreshold (int bp, double threshold) {

	RGPDouble* newThreshold = new RGPDouble (threshold);

	if (bp >= 0)
		mPositiveNonStandardStutter.ReplaceElementAt (bp, newThreshold);

	else
		mNegativeNonStandardStutter.ReplaceElementAt (-bp, newThreshold);
}


void locusSpecificLimitsStruct :: Reset () {

	locusName = "";
	fractionOfMaxPeak = -1.0;
	pullupFractionalFilter = -1.0;
	stutterThreshold = -1.0;
	plusStutterThreshold = -1.0;
	adenylationThreshold = -1.0;
	heterozygousImbalanceLimit = -1.0;
	minBoundForHomozygote = -1.0;
}


ParameterServer :: ParameterServer () {

	if (ParameterServer::ReferenceCount == 0) {

		Changed = FALSE;
		mSet = NULL;
		mValid = true;
		LadderIDs = ControlIDs = PositiveControlIDs = NegativeControlIDs = mSingleSourceSampleIDs = mMixtureIDs = NULL;
		mStandardPositiveControlName = new RGString;
		mMarkerSetName = new RGString;
		mStandardSettingsString = new RGString;
		mLabSettingsString = new RGString;
		mSmartMessageThresholds = new RGDList;
		mLadderLocusSpecificThresholds = new list<locusSpecificLimitsStruct*>;
		mSampleLocusSpecificThresholds = new list<locusSpecificLimitsStruct*>;
		mSampleNonStandardStutterThresholds = new list<locusSpecificNonStandardStutterStruct*>;
		mAnalysisThresholds = new list<channelThreshold*>;
		mDetectionThresholds = new list<channelThreshold*>;
	}

	ParameterServer::ReferenceCount++;
}


ParameterServer :: ParameterServer (const RGString& xmlString) : mValid (true) {

	if (ParameterServer::ReferenceCount == 0) {

		Changed = TRUE;
		mStandardPositiveControlName = new RGString;
		mStandardSettingsString = new RGString;
		mMarkerSetName = new RGString;
		mLabSettingsString = new RGString;
		mSmartMessageThresholds = new RGDList;
		mLadderLocusSpecificThresholds = new list<locusSpecificLimitsStruct*>;
		mSampleLocusSpecificThresholds = new list<locusSpecificLimitsStruct*>;
		mSampleNonStandardStutterThresholds = new list<locusSpecificNonStandardStutterStruct*>;
	}

	ParameterServer::ReferenceCount++;

	RGString XMLString (xmlString);
	mSet = new GenotypeSet (xmlString, false);

	if (!mSet->isValid ())
		mValid = false;

	else if (!ReadCurveFitParameters (xmlString))
		mValid = false;

	LadderIDs = new SynonymList;
	ControlIDs = new SynonymList;
	PositiveControlIDs = new SynonymList;
	NegativeControlIDs = new SynonymList;
	mSingleSourceSampleIDs = new SynonymList;
	mMixtureIDs = new SynonymList;

	//
	//  Reformat XMLString a little and save into mStandardSettingsString
	//

	RGBracketStringSearch StdSettingsSearch ("<AlgorithmParameters>", "</StandardSettings>", XMLString);
	size_t startOffset = 0;
	size_t endOffset = 0;

	if (!StdSettingsSearch.FindNextBracketedString (startOffset, endOffset, *mStandardSettingsString))
		mValid = false;

	else {

		mStandardSettingsString->FindAndReplaceAllSubstrings ("\n", "\n\t\t");
		mStandardSettingsString->Prepend ("\t\t<StandardSettings>\n\t\t\t<AlgorithmParameters>");
		mStandardSettingsString->Append ("\t\t</StandardSettings>");
	}
}



ParameterServer :: ~ParameterServer () {

	ParameterServer::ReferenceCount--;

	if (ReferenceCount <= 0) {

		delete mSet;
		delete LadderIDs;
		delete ControlIDs;
		delete PositiveControlIDs;
		delete NegativeControlIDs;
		delete mSingleSourceSampleIDs;
		delete mMixtureIDs;
		delete mStandardPositiveControlName;
		delete mMarkerSetName;
		delete mStandardSettingsString;
		delete mLabSettingsString;

		if (mSmartMessageThresholds != NULL)
			mSmartMessageThresholds->ClearAndDelete ();

		delete mSmartMessageThresholds;
		mSmartMessageThresholds = NULL;

		list<locusSpecificLimitsStruct*>::iterator c1Iterator;
		list<locusSpecificLimitsStruct*>::iterator c2Iterator;

		locusSpecificLimitsStruct* nextLink;

		for (c1Iterator = mLadderLocusSpecificThresholds->begin (); c1Iterator != mLadderLocusSpecificThresholds->end (); c1Iterator++) {

			nextLink = *c1Iterator;
			delete nextLink;
		}

		mLadderLocusSpecificThresholds->clear ();
		delete mLadderLocusSpecificThresholds;

		for (c2Iterator = mSampleLocusSpecificThresholds->begin (); c2Iterator != mSampleLocusSpecificThresholds->end (); c2Iterator++) {

			nextLink = *c2Iterator;
			delete nextLink;
		}

		mSampleLocusSpecificThresholds->clear ();
		delete mSampleLocusSpecificThresholds;
		mSampleNonStandardStutterThresholds->clear ();
		delete mSampleNonStandardStutterThresholds;

	}
}


void* ParameterServer :: operator new (size_t nbytes) {

	void* p;
	
	if (ParameterServer::ReferenceCount > 0) {
		
		return ParameterServer::OneAndOnlySelf;
	}
	
	p = malloc (nbytes);
	ParameterServer::OneAndOnlySelf = (ParameterServer*) p;
	return p;
}


void ParameterServer :: operator delete (void* ptr) {

	if (ParameterServer::ReferenceCount <= 0)
		free (ptr);
}


bool ParameterServer :: AddGenotypeCollection (const RGString& xmlString, bool isLabSettings) {

	bool returnValue = true;
	RFULimitsStruct rfuLimits;
	labNonRFULimitsStruct nonRFULimits;
	RGString XMLString (xmlString);
	RGString rfuString;
	RGString dataTypeString;
	RGXMLTagSearch FileTypeSearch ("DataFileType", XMLString);
	RGXMLTagSearch ladderRFUSearch ("LadderRFUTests", XMLString);
	RGXMLTagSearch laneStdRFUSearch ("LaneStandardRFUTests", XMLString);
	RGXMLTagSearch sampleRFUSearch ("SampleRFUTests", XMLString);
	size_t startOffset = 0;
	size_t endOffset = 0;
	double limit;
	
	if (isLabSettings) {

		if (FileTypeSearch.FindNextTag (startOffset, endOffset, dataTypeString)) {

			startOffset = endOffset;

			if (dataTypeString.Length () > 0)
				DirectoryManager::SetDataFileType (dataTypeString);
		}

		returnValue = ReadFileNameStrings (xmlString);

		if (!returnValue) {

			cout << "Could not read lab settings file name strings" << endl;
			return false;
		}

		if (!ladderRFUSearch.FindNextTag (startOffset, endOffset, rfuString)) {

			cout << "Could not read lab settings ladder RFU search strings" << endl;
			return false;
		}

		startOffset = endOffset;
		rfuLimits.Reset ();

		if (!ReadLadderLabLimits (rfuString, rfuLimits)) {

			cout << "Could not read lab settings ladder lab limits strings" << endl;
			return false;
		}

		STRLadderChannelData::SetLadderMaximumRFU (rfuLimits.maxRFU);
		Locus::SetGridStutterThreshold (rfuLimits.stutterThreshold);
		Locus::SetGridAdenylationThreshold (rfuLimits.adenylationThreshold);
		limit = rfuLimits.pullupFractionOfMaxPeak;

		if (limit > 1.0)
			limit *= 0.01;

		Locus::SetGridPullupFractionalFilter (limit);
		limit = rfuLimits.fractionOfMaxPeak;

		if (limit > 1.0)
			limit *= 0.01;

		Locus::SetGridFractionalFilter (limit);

		if (!laneStdRFUSearch.FindNextTag (startOffset, endOffset, rfuString)) {

			cout << "Could not read lab settings ILS strings" << endl;
			return false;
		}

		startOffset = endOffset;
		rfuLimits.Reset ();

		if (!ReadRFULimits (rfuString, rfuLimits)) {

			cout << "Could not read lab settings RFU limit strings" << endl;
			return false;
		}

		STRLaneStandardChannelData::SetLSMaximumRFU (rfuLimits.maxRFU);
		STRLaneStandardChannelData::SetILSStutterThreshold (rfuLimits.stutterThreshold);
		STRLaneStandardChannelData::SetILSAdenylationThreshold (rfuLimits.adenylationThreshold);
		limit = rfuLimits.fractionOfMaxPeak;

		if (limit > 1.0)
			limit *= 0.01;

		STRLaneStandardChannelData::SetILSFractionalFilter (limit);

		if (!sampleRFUSearch.FindNextTag (startOffset, endOffset, rfuString))
			return false;

		startOffset = endOffset;
		rfuLimits.Reset ();

		if (!ReadSampleLabLimits (rfuString, rfuLimits))
			return false;

		STRSampleChannelData::SetSampleMaximumRFU (rfuLimits.maxRFU);
		Locus::SetSampleStutterThreshold (rfuLimits.stutterThreshold);
		Locus::SetSamplePlusStutterThreshold (rfuLimits.plusStutterThreshold);
		Locus::SetSampleAdenylationThreshold (rfuLimits.adenylationThreshold);
		limit = rfuLimits.fractionOfMaxPeak;

		if (limit > 1.0)
			limit *= 0.01;

		Locus::SetSampleFractionalFilter (limit);
		limit = rfuLimits.pullupFractionOfMaxPeak;

		if (limit > 1.0)
			limit *= 0.01;

		Locus::SetSamplePullupFractionalFilter (limit);

		if (!ReadLabNonRFULimits (startOffset, endOffset, xmlString, nonRFULimits))	// Fix this to use a start index and an end index
			return false;

		mMaxPullups = nonRFULimits.maxNumberPullupsPerSample;
		mMaxStutter = nonRFULimits.maxNumberStutterPerSample;
		mMaxSpikes = nonRFULimits.maxNumberSpikesPerSample;
		mMaxAdenylations = nonRFULimits.maxNumberAdenylationPerSample;
		mMaxOLAlleles = nonRFULimits.maxNumberOLAllelesPerSample;
		mMaxExcessiveResiduals = nonRFULimits.maxNumberExcessiveResidualsPerSample;
		Locus::SetMaxResidualForAlleleCalls (nonRFULimits.maxResidualForAlleleCall);
		Locus::SetAlleleOverloadThreshold (nonRFULimits.alleleRFUOverloadThreshold);

		limit = nonRFULimits.heterozygousImbalanceLimit;

		if (limit > 1.0)
			limit = 0.01 * limit;  // assume an input error and this was actually a per cent

		Locus::SetHeterozygousImbalanceLimit (limit);
		Locus::SetMinBoundForHomozygote (nonRFULimits.minBoundForHomozygote);
		CoreBioComponent::SetMinBioIDForArtifacts (nonRFULimits.minBPSForArtifacts);

		//
		//  Reformat XMLString a little and save into mLabSettingsString
		//

		RGBracketStringSearch LabSettingsAttributeSearch ("<LabSettings", ">", XMLString);
		startOffset = 0;
		endOffset = 0;
		RGString tempString;

		if (!LabSettingsAttributeSearch.FindNextBracketedString (startOffset, endOffset, tempString))
			mValid = false;

		else
			XMLString.ReplaceSubstring (0, endOffset, "<LabSettings>");

		RGBracketStringSearch LabSettingsSearch ("<LabSettings>", "</LabSettings>", XMLString);
		startOffset = 0;
		endOffset = 0;

		if (!LabSettingsSearch.FindNextBracketedString (startOffset, endOffset, *mLabSettingsString))
			mValid = false;

		else {

			mLabSettingsString->Prepend ("\t\t<LabSettings>\n");
			mLabSettingsString->Append ("</LabSettings>");
			mLabSettingsString->FindAndReplaceAllSubstrings ("\n", "\n\t\t");
		}
	}
	
	if (mSet == NULL) {

		mSet = new GenotypeSet (xmlString, isLabSettings);
		return mSet->isValid ();
	}

	return mSet->AddGenotypes (xmlString, isLabSettings);		
}


int ParameterServer :: AddLadderLocusSpecificThreshold (const RGString& locusName, ThresholdIdEnum thresholdID, double value) {

	list<locusSpecificLimitsStruct*>::const_iterator c1Iterator;
	locusSpecificLimitsStruct* nextLink;

	for (c1Iterator = mLadderLocusSpecificThresholds->begin (); c1Iterator != mLadderLocusSpecificThresholds->end (); c1Iterator++) {

		nextLink = *c1Iterator;
		
		if (nextLink->locusName == locusName) {

			switch (thresholdID) {

				case FRACTIONOFMAXPEAK:

					nextLink->fractionOfMaxPeak = value;
					return 0;

				case PULLUPFRACTIONALFILTER:
					
					nextLink->pullupFractionalFilter = value;
					return 0;
					
				case STUTTERTHRESHOLD:

					nextLink->stutterThreshold = value;
					return 0;
						
				case ADENYLATIONTHRESHOLD:
					
					nextLink->adenylationThreshold = value;
					return 0;
					
				case HETEROZYGOUSIMBALANCELIMIT:
					
					nextLink->heterozygousImbalanceLimit = value;
					return 0;
					
				case MINBOUNDFORHOMOZYGOTE:

					nextLink->minBoundForHomozygote = value;
					return 0;

				default:
					return -1;
			}
		}
	}

	nextLink = new locusSpecificLimitsStruct;
	nextLink->locusName = locusName;
	mLadderLocusSpecificThresholds->push_back (nextLink);
	
	switch (thresholdID) {

		case FRACTIONOFMAXPEAK:

			nextLink->fractionOfMaxPeak = value;
			return 0;

		case PULLUPFRACTIONALFILTER:
			
			nextLink->pullupFractionalFilter = value;
			return 0;
			
		case STUTTERTHRESHOLD:

			nextLink->stutterThreshold = value;
			return 0;
				
		case ADENYLATIONTHRESHOLD:
			
			nextLink->adenylationThreshold = value;
			return 0;
			
		case HETEROZYGOUSIMBALANCELIMIT:
			
			nextLink->heterozygousImbalanceLimit = value;
			return 0;
			
		case MINBOUNDFORHOMOZYGOTE:

			nextLink->minBoundForHomozygote = value;
			return 0;

		default:
			return -1;
	}

	return -1;
}


int ParameterServer :: AddSampleLocusSpecificThreshold (const RGString& locusName, ThresholdIdEnum thresholdID, double value) {

	list<locusSpecificLimitsStruct*>::const_iterator c1Iterator;
	locusSpecificLimitsStruct* nextLink;

	if (locusName == "D21S11") {
		bool stopHere = true;
	}

	for (c1Iterator = mSampleLocusSpecificThresholds->begin (); c1Iterator != mSampleLocusSpecificThresholds->end (); c1Iterator++) {

		nextLink = *c1Iterator;
		
		if (nextLink->locusName == locusName) {

			switch (thresholdID) {

				case FRACTIONOFMAXPEAK:

					nextLink->fractionOfMaxPeak = value;
					return 0;

				case PULLUPFRACTIONALFILTER:
					
					nextLink->pullupFractionalFilter = value;
					return 0;
					
				case STUTTERTHRESHOLD:

					nextLink->stutterThreshold = value;
					return 0;
						
				case ADENYLATIONTHRESHOLD:
					
					nextLink->adenylationThreshold = value;
					return 0;
					
				case HETEROZYGOUSIMBALANCELIMIT:
					
					nextLink->heterozygousImbalanceLimit = value;
					return 0;
					
				case MINBOUNDFORHOMOZYGOTE:

					nextLink->minBoundForHomozygote = value;
					return 0;

				default:
					return -1;
			}
		}
	}

	nextLink = new locusSpecificLimitsStruct;
	nextLink->locusName = locusName;
	mSampleLocusSpecificThresholds->push_back (nextLink);
	
	switch (thresholdID) {

		case FRACTIONOFMAXPEAK:

			nextLink->fractionOfMaxPeak = value;
			return 0;

		case PULLUPFRACTIONALFILTER:
			
			nextLink->pullupFractionalFilter = value;
			return 0;
			
		case STUTTERTHRESHOLD:

			nextLink->stutterThreshold = value;
			return 0;
				
		case ADENYLATIONTHRESHOLD:
			
			nextLink->adenylationThreshold = value;
			return 0;
			
		case HETEROZYGOUSIMBALANCELIMIT:
			
			nextLink->heterozygousImbalanceLimit = value;
			return 0;
			
		case MINBOUNDFORHOMOZYGOTE:

			nextLink->minBoundForHomozygote = value;
			return 0;

		default:
			return -1;
	}

	return -1;
}


int ParameterServer :: AddLadderLocusSpecificThreshold (const locusSpecificLimitsStruct& limits) {

	list<locusSpecificLimitsStruct*>::const_iterator c1Iterator;
	locusSpecificLimitsStruct* nextLink;

	for (c1Iterator = mLadderLocusSpecificThresholds->begin (); c1Iterator != mLadderLocusSpecificThresholds->end (); c1Iterator++) {

		nextLink = *c1Iterator;
		
		if (nextLink->locusName == limits.locusName) {

			nextLink->CopyPositiveThresholdsFrom (limits);
			return 0;
		}
	}

	nextLink = new locusSpecificLimitsStruct (limits);
	mLadderLocusSpecificThresholds->push_back (nextLink);
	return 0;
}


int ParameterServer :: AddSampleLocusSpecificThreshold (const locusSpecificLimitsStruct& limits) {

	list<locusSpecificLimitsStruct*>::const_iterator c1Iterator;
	locusSpecificLimitsStruct* nextLink;

	for (c1Iterator = mSampleLocusSpecificThresholds->begin (); c1Iterator != mSampleLocusSpecificThresholds->end (); c1Iterator++) {

		nextLink = *c1Iterator;
		
		if (nextLink->locusName == limits.locusName) {

			nextLink->CopyPositiveThresholdsFrom (limits);
			return 0;
		}
	}

	nextLink = new locusSpecificLimitsStruct (limits);
	mSampleLocusSpecificThresholds->push_back (nextLink);
	return 0;
}


int ParameterServer :: AddSampleLocusSpecificNonStandardStutterCollection (const locusSpecificNonStandardStutterStruct& nsLocusStutter) {

	locusSpecificNonStandardStutterStruct* newStruct = new locusSpecificNonStandardStutterStruct (nsLocusStutter);
	mSampleNonStandardStutterThresholds->push_back (newStruct);
	return 0;
}


int ParameterServer :: SetSampleLocusSpecificThresholds (Locus* locus, locusSpecificLimitsStruct* limits) {

	if (limits->stutterThreshold >= 0.0) {

		locus->SetLocusSpecificSampleStutterThreshold (limits->stutterThreshold);

		if (limits->stutterThresholdRight >= limits->stutterThreshold)
			locus->SetLocusSpecificSampleStutterThresholdRight (limits->stutterThresholdRight);
	}

	else
		locus->SetLocusSpecificSampleStutterThreshold (Locus::GetSampleStutterThreshold ());

	if (limits->plusStutterThreshold >= 0.0) {

		locus->SetLocusSpecificSamplePlusStutterThreshold (limits->plusStutterThreshold);

		if (limits->plusStutterThresholdRight >= limits->plusStutterThreshold)
			locus->SetLocusSpecificSamplePlusStutterThresholdRight (limits->plusStutterThresholdRight);
	}

	else
		locus->SetLocusSpecificSamplePlusStutterThreshold (Locus::GetSamplePlusStutterThreshold ());

	if (limits->adenylationThreshold >= 0.0)
		locus->SetLocusSpecificSampleAdenylationThreshold (limits->adenylationThreshold);

	else
		locus->SetLocusSpecificSampleAdenylationThreshold (Locus::GetSampleAdenylationThreshold ());

	if (limits->fractionOfMaxPeak >= 0.0)
		locus->SetLocusSpecificSampleFractionalFilter (limits->fractionOfMaxPeak);

	else
		locus->SetLocusSpecificSampleFractionalFilter (Locus::GetSampleFractionalFilter ());

	if (limits->pullupFractionalFilter >= 0.0)
		locus->SetLocusSpecificSamplePullupFractionalFilter (limits->pullupFractionalFilter);

	else
		locus->SetLocusSpecificSamplePullupFractionalFilter (Locus::GetSamplePullupFractionalFilter ());

	if (limits->heterozygousImbalanceLimit >= 0.0)
		locus->SetLocusSpecificSampleHeterozygousImbalanceThreshold (limits->heterozygousImbalanceLimit);

	else
		locus->SetLocusSpecificSampleHeterozygousImbalanceThreshold (Locus::GetHeterozygousImbalanceLimit ());

	if (limits->minBoundForHomozygote > 0.0) {

		double bound = limits->minBoundForHomozygote;
		double rfuBound;

		if (Locus::GetExpectRFUUnitsForHomozygoteBound ())
			rfuBound = bound;
		
		else {

			if (bound < 10.0)
				rfuBound = bound * STRSampleChannelData::GetMinRFU ();

			else
				rfuBound = 0.01 * bound * STRSampleChannelData::GetMinRFU ();
		}			

		locus->SetLocusSpecificSampleMinBoundForHomozygote (rfuBound);
	}

	else
		locus->SetLocusSpecificSampleMinBoundForHomozygote (Locus::GetMinBoundForHomozygote ());

	return 0;
}


int ParameterServer :: SetLadderLocusSpecificThresholds (Locus* locus, locusSpecificLimitsStruct* limits) {

	if (limits->stutterThreshold >= 0.0)
		locus->SetLocusSpecificLadderStutterThreshold (limits->stutterThreshold);

	else
		locus->SetLocusSpecificLadderStutterThreshold (Locus::GetGridStutterThreshold ());

	if (limits->adenylationThreshold >= 0.0)
		locus->SetLocusSpecificLadderAdenylationThreshold (limits->adenylationThreshold);

	else
		locus->SetLocusSpecificLadderAdenylationThreshold (Locus::GetGridAdenylationThreshold ());

	if (limits->fractionOfMaxPeak >= 0.0)
		locus->SetLocusSpecificLadderFractionalFilter (limits->fractionOfMaxPeak);

	else
		locus->SetLocusSpecificLadderFractionalFilter (Locus::GetGridFractionalFilter ());

	if (limits->pullupFractionalFilter >= 0.0)
		locus->SetLocusSpecificLadderPullupFractionalFilter (limits->pullupFractionalFilter);

	else
		locus->SetLocusSpecificLadderPullupFractionalFilter (Locus::GetGridPullupFractionalFilter ());

	return 0;
}


int ParameterServer :: SetAllLocusSpecificThresholds (PopulationCollection* collection) {

	PopulationMarkerSet* mSet = collection->GetNamedPopulationMarkerSet (*mMarkerSetName);
	int status = 0;

	if (mSet == NULL) {

//		cout << "Parameter server could not find population marker set named:  " << mMarkerSetName.GetData () << endl;
		return -1;
	}

	int nChannels = mSet->GetNumberOfChannels ();
	mNumberOfChannels = nChannels;
	STRSampleChannelData::InitializeChannelSpecificThresholds (nChannels, mAnalysisThresholds, mDetectionThresholds);

	list<locusSpecificLimitsStruct*>::iterator c1Iterator;
	list<locusSpecificLimitsStruct*>::iterator c2Iterator;
	locusSpecificLimitsStruct* nextLink;
	Locus* locus;

	for (c1Iterator = mLadderLocusSpecificThresholds->begin (); c1Iterator != mLadderLocusSpecificThresholds->end (); c1Iterator++) {

		nextLink = *c1Iterator;
		locus = mSet->FindLocus (nextLink->locusName);

		if (locus != NULL)
			SetLadderLocusSpecificThresholds (locus, nextLink);

		else
			status = -1;
	}

	for (c2Iterator = mSampleLocusSpecificThresholds->begin (); c2Iterator != mSampleLocusSpecificThresholds->end (); c2Iterator++) {

		nextLink = *c2Iterator;
		locus = mSet->FindLocus (nextLink->locusName);

		if (locus != NULL)
			SetSampleLocusSpecificThresholds (locus, nextLink);

		else
			status = -1;
	}

	list<locusSpecificNonStandardStutterStruct*>::iterator it;
	locusSpecificNonStandardStutterStruct* nextNSLink;

	for (it = mSampleNonStandardStutterThresholds->begin (); it != mSampleNonStandardStutterThresholds->end (); it++) {

		nextNSLink = *it;
		locus = mSet->FindLocus (nextNSLink->locusName);

		if (locus != NULL)
			locus->SetLocusSpecificNonStandardStutterArray (*nextNSLink);

		else
			status = -1;
	}


	return status;
}


void ParameterServer :: SetParametricFitTolerance (double fit) {

	ParametricCurve::SetFitTolerance (fit);
	Changed = TRUE;
}


double ParameterServer :: GetParametricFitTolerance () const {

	return ParametricCurve::GetFitTolerance ();
}


void ParameterServer :: SetNumberOfIntegrationSteps (int steps) {

	Gaussian::SetNumberOfIntegrationSteps (steps);
	SuperGaussian::SetNumberOfIntegrationSteps (steps);
	Changed = TRUE;
}


int ParameterServer :: GetNumberOfIntegrationSteps () const {

	return Gaussian::GetNumberOfIntegrationSteps ();
}


void ParameterServer :: SetGaussianSigmaTolerance (double tol) {

	NormalizedGaussian::SetSigmaTolerance (tol);
	Changed = TRUE;
}


double ParameterServer :: GetGaussianSigmaTolerance () const {

	return NormalizedGaussian::GetSigmaTolerance ();
}


void ParameterServer :: SetGaussianSigmaWidth (double nSigmas) {

	NormalizedGaussian::SetSigmaWidth (nSigmas);
	Gaussian::SetSigmaWidth (nSigmas);
	Changed = TRUE;
}


double ParameterServer :: GetGaussianSigmaWidth () const {

	return NormalizedGaussian::GetSigmaWidth ();
}


void ParameterServer :: SetMaximumSearchIterations (int iterations) {

	NormalizedGaussian::SetMaximumIterations (iterations);
	Changed = TRUE;
}


int ParameterServer :: GetMaximumSearchIterations () const {

	return NormalizedGaussian::GetMaximumIterations ();
}


void ParameterServer :: SetDoubleGaussianSigmaRatio (double ratio) {

	DoubleGaussian::SetDefaultSigmaRatio (ratio);
	Changed = TRUE;
}


double ParameterServer :: GetDoubleGaussianSigmaRatio () const {

	return DoubleGaussian::GetDefaultSigmaRatio ();
}


void ParameterServer :: SetInterSampleSpacing (double spacing) {

	DataSignal::SetSampleSpacing (spacing);
	Changed = TRUE;
}


double ParameterServer :: GetInterSampleSpacing () const {

	return DataSignal::GetSampleSpacing ();
}



void ParameterServer :: SetSquareWaveWindowWidth (int width) {

	TracePrequalification::SetWindowWidth (width);
	TracePrequalification::SetDefaultWindowWidth (width);
	Changed = TRUE;
}


int ParameterServer :: GetSquareWaveWindowWidth () const {

	return TracePrequalification::GetDefaultWindowWidth ();
}



void ParameterServer :: SetConvolutionNoiseThreshold (double threshold) {

	TracePrequalification::SetNoiseThreshold (threshold);
	TracePrequalification::SetDefaultNoiseThreshold (threshold);
	Changed = TRUE;
}


double ParameterServer :: GetConvolutionNoiseThreshold () const {

	return TracePrequalification::GetDefaultNoiseThreshold ();
}


void ParameterServer :: Report (RGTextOutput& text, const RGString& indent) {

	Changed = FALSE;
	Endl endLine;
	text << endLine;
	text << indent << "***********************************************************************************" << endLine;
	text << indent << "*    OSIRIS PARAMETERS:" << endLine;
	text << indent << "*       Parametric Fit Tolerance:  " << "\t\t\t\t\t" << GetParametricFitTolerance () << endLine;
	text << indent << "*       Number of Integration Steps:  " << "\t\t\t\t\t" << GetNumberOfIntegrationSteps () << endLine;
	text << indent << "*       Gaussian Sigma Tolerance:  " << "\t\t\t\t\t" << GetGaussianSigmaTolerance () << endLine;
	text << indent << "*       Gaussian Sigma Width:  " << "\t\t\t\t\t" << GetGaussianSigmaWidth () << endLine;
	text << indent << "*       Maximum Number of Search Iterations:  " << "\t\t\t\t\t" << GetMaximumSearchIterations () << endLine;
	text << indent << "*       Double Gaussian Sigma Ratio:  " << "\t\t\t\t\t" << GetDoubleGaussianSigmaRatio () << endLine;
	text << indent << "*       Inter-sample Spacing:  " << "\t\t\t\t\t" << GetInterSampleSpacing () << endLine;
	text << indent << "*       Square Wave Convolution Window Width:  " << "\t\t\t\t\t" << GetSquareWaveWindowWidth () << endLine;
	text << indent << "*       Convolution Noise Threshold:  " << "\t\t\t\t\t" << GetConvolutionNoiseThreshold () << endLine;
	text << indent << "***********************************************************************************" << endLine << endLine;
}


void ParameterServer :: ReportIfChanged (RGTextOutput& text, const RGString& indent) {

	if (Changed)
		Report (text, indent);
}


void ParameterServer :: WriteSettingsToDotOAR (RGLogBook& outputFile) {

	//
	// Write lab settings first
	//

	outputFile.SetOutputLevel (1);
	Endl endLine;
	outputFile << "\n" << mLabSettingsString->GetData () << endLine;
	outputFile << mStandardSettingsString->GetData () << endLine;
	outputFile.ResetOutputLevel ();
}


RGString ParameterServer :: BuildChannelThresholdOverridesForOAR () {

	//   Requires 3 tabs

	RGString params;
	RGString tabs3 = "\t\t\t";
	RGString tabs4 = tabs3 + "\t";
	RGString tabs5 = tabs4 + "\t";

	if ((mAnalysisThresholds->size () == 0) && (mDetectionThresholds->size () == 0))
		return params;

	int* analysis = new int [mNumberOfChannels + 1];
	int* detection = new int [mNumberOfChannels + 1];
	int i;
	list<channelThreshold*>::iterator c1Iterator;
	list<channelThreshold*>::iterator c2Iterator;
	channelThreshold* nextThreshold;
	int channel;

	for (i=1; i<=mNumberOfChannels; i++) {

		analysis [i] = detection [i] = -1;
	}

	for (c1Iterator = mAnalysisThresholds->begin (); c1Iterator != mAnalysisThresholds->end (); c1Iterator++) {

		nextThreshold = *c1Iterator;
		channel = nextThreshold->mChannel;

		if ((channel >= 1) && (channel <= mNumberOfChannels))
			analysis [channel] = (int) nextThreshold->mThreshold;
	}

	for (c2Iterator = mDetectionThresholds->begin (); c2Iterator != mDetectionThresholds->end (); c2Iterator++) {

		nextThreshold = *c2Iterator;
		channel = nextThreshold->mChannel;

		if ((channel >= 1) && (channel <= mNumberOfChannels))
			detection [channel] = (int) nextThreshold->mThreshold;
	}

	//params << tabs3 << "<ChannelThresholdOverrides>\n";

	//for (i=1; i<=mNumberOfChannels; i++) {

	//	channelAnalysis = analysis [i];
	//	channelDetection = detection [i];

	//	if ((channelAnalysis <= 0.0) && (channelDetection <= 0.0))
	//		continue;

	//	params << tabs4 << "<Channel>\n";
	//	params << tabs5 << "<Number>" << i << "</Number>\n";
	//	
	//	if (channelAnalysis > 0.0)
	//		params << tabs5 << "<AnalysisThreshold>" << channelAnalysis << "</AnalysisThreshold>\n";

	//	if (channelDetection > 0.0)
	//		params << tabs5 << "<DetectionThreshold>" << channelDetection << "</DetectionThreshold>\n";

	//	params << tabs4 << "</Channel>\n";
	//

	//params << tabs3 << "</ChannelThresholdOverrides>\n";

	params << tabs3 << "<ChannelRFU>";

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i > 1)
			params << " ";

		params << analysis [i];
	}

	params << "</ChannelRFU>\n";
	params << tabs3 << "<ChannelDetection>";

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i > 1)
			params << " ";

		params << detection [i];
	}

	params << "</ChannelDetection>\n";
	delete[] analysis;
	delete[] detection;

	return params;
}



RGString ParameterServer :: BuildChannelThresholdOverridesForPLT () {

	//   Requires 2 tabs

	RGString params;
	RGString tabs3 = "\t\t";
	RGString tabs4 = tabs3 + "\t";
	RGString tabs5 = tabs4 + "\t";

	if ((mAnalysisThresholds->size () == 0) && (mDetectionThresholds->size () == 0))
		return params;

	int* analysis = new int [mNumberOfChannels + 1];
	int* detection = new int [mNumberOfChannels + 1];
	int i;
	list<channelThreshold*>::iterator c1Iterator;
	list<channelThreshold*>::iterator c2Iterator;
	channelThreshold* nextThreshold;
	int channel;

	for (i=1; i<=mNumberOfChannels; i++) {

		analysis [i] = detection [i] = -1;
	}

	for (c1Iterator = mAnalysisThresholds->begin (); c1Iterator != mAnalysisThresholds->end (); c1Iterator++) {

		nextThreshold = *c1Iterator;
		channel = nextThreshold->mChannel;

		if ((channel >= 1) && (channel <= mNumberOfChannels))
			analysis [channel] = (int) nextThreshold->mThreshold;
	}

	for (c2Iterator = mDetectionThresholds->begin (); c2Iterator != mDetectionThresholds->end (); c2Iterator++) {

		nextThreshold = *c2Iterator;
		channel = nextThreshold->mChannel;

		if ((channel >= 1) && (channel <= mNumberOfChannels))
			detection [channel] = (int) nextThreshold->mThreshold;
	}

	//params << tabs3 << "<ChannelThresholdOverrides>\n";

	//for (i=1; i<=mNumberOfChannels; i++) {

	//	channelAnalysis = analysis [i];
	//	channelDetection = detection [i];

	//	if ((channelAnalysis <= 0.0) && (channelDetection <= 0.0))
	//		continue;

	//	params << tabs4 << "<Channel>\n";
	//	params << tabs5 << "<Number>" << i << "</Number>\n";
	//	
	//	if (channelAnalysis > 0.0)
	//		params << tabs5 << "<AnalysisThreshold>" << channelAnalysis << "</AnalysisThreshold>\n";

	//	if (channelDetection > 0.0)
	//		params << tabs5 << "<DetectionThreshold>" << channelDetection << "</DetectionThreshold>\n";

	//	params << tabs4 << "</Channel>\n";
	//}

	//params << tabs3 << "</ChannelThresholdOverrides>\n";

	params << tabs3 << "<ChannelRFU>";

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i > 1)
			params << " ";

		params << analysis [i];
	}

	params << "</ChannelRFU>\n";
	params << tabs3 << "<ChannelDetection>";

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i > 1)
			params << " ";

		params << detection [i];
	}

	params << "</ChannelDetection>\n";
	delete[] analysis;
	delete[] detection;
	return params;
}


bool ParameterServer :: ReadAlgorithmParameters (const RGString& xmlString) {

	RGString XMLString (xmlString);
	RGXMLTagSearch curveFitSearch ("CurveFitParameters", XMLString);
	RGXMLTagSearch laneStdSearch ("LaneStandardSearch", XMLString);
	RGXMLTagSearch ladderLocusSearch ("LadderLocusSearch", XMLString);
	RGString curveFit;
	RGString laneStd;
	RGString ladderLocus;
	bool returnValue = true;
	size_t startOffset = 0;
	size_t endOffset = 0;

	if (!curveFitSearch.FindNextTag (startOffset, endOffset, curveFit))
		returnValue = false;

	startOffset = endOffset;

	if (!laneStdSearch.FindNextTag (startOffset, endOffset, laneStd))
		returnValue = false;

	startOffset = endOffset;

	if (!ladderLocusSearch.FindNextTag (startOffset, endOffset, ladderLocus))
		returnValue = false;

	return returnValue;
}


bool ParameterServer :: ReadCurveFitParameters (const RGString& xmlString) {

	RGString XMLString (xmlString);
	RGString numString;
	bool returnValue = true;
	size_t startOffset = 0;
	size_t endOffset = 0;
	double numValue;
	int intNumValue;

	RGXMLTagSearch noiseSearch ("NoiseThreshold", XMLString);
	RGXMLTagSearch windowWidthSearch ("WindowWidth", XMLString);
	RGXMLTagSearch normalPeakSearch ("MinFitForNormalPeak", XMLString);
	RGXMLTagSearch artifactSearch ("TriggerFitForArtifact", XMLString);
	RGXMLTagSearch absoluteMinFitSearch ("AbsoluteMinimumFit", XMLString);
	RGXMLTagSearch secondaryContentSearch ("TestForNegSecondaryContent", XMLString);
	RGXMLTagSearch parametricFitSearch ("ParametricFitTolerance", XMLString);
	RGXMLTagSearch integrationStepsSearch ("NumberOfIntegrationSteps", XMLString);
	RGXMLTagSearch sigmaToleranceSearch ("SigmaTolerance", XMLString);
	RGXMLTagSearch sigmaWidthSearch ("SigmaWidth", XMLString);
	RGXMLTagSearch maxIterationsSearch ("MaximumIterations", XMLString);
	RGXMLTagSearch sigmaRatioSearch ("SigmaRatio", XMLString);
	RGXMLTagSearch spacingSearch ("InterSampleSpacing", XMLString);
	RGXMLTagSearch signatureSigmaSearch ("SignatureSigma", XMLString);
	RGXMLTagSearch blobDegreeSearch ("BlobDegree", XMLString);
	RGXMLTagSearch minPeakDistanceSearch ("MinDistanceBetweenPeaks", XMLString);
	RGXMLTagSearch endPointFractionSearch ("PeakFractionForEndPtTest", XMLString);
	RGXMLTagSearch endPointLevelSearch ("PeakLevelForEndPtTest", XMLString);

	RGXMLTagSearch lowTailHeightSearch ("LowTailHeightThreshold", XMLString);
	RGXMLTagSearch lowTailSlopeSearch ("LowTailSlopeThreshold", XMLString);
	RGXMLTagSearch minRegressionSamplesSearch ("MinSamplesForSlopeRegression", XMLString);


	if (!noiseSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		TracePrequalification::SetNoiseThreshold (numValue);
		TracePrequalification::SetDefaultNoiseThreshold (numValue);
	}

	startOffset = endOffset;

	if (!windowWidthSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		intNumValue = numString.ConvertToInteger ();
		TracePrequalification::SetWindowWidth (intNumValue);
		TracePrequalification::SetDefaultWindowWidth (intNumValue);
	}

	startOffset = endOffset;

	if (!normalPeakSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ParametricCurve::SetMinimumFitThreshold (numValue);
	}

	startOffset = endOffset;

	if (!artifactSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ParametricCurve::SetTriggerForArtifactTest (numValue);
	}

	startOffset = endOffset;

	if (!absoluteMinFitSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ParametricCurve::SetAbsoluteMinimumFit (numValue);
	}

	startOffset = endOffset;

	if (!secondaryContentSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numString.ToLower ();

		if (numString == "true")
			TestCharacteristic::SetGlobalTestForNegative (true);

		else
			TestCharacteristic::SetGlobalTestForNegative (false);
	}

	startOffset = endOffset;

	if (!parametricFitSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ParametricCurve::SetFitTolerance (numValue);
	}

	startOffset = endOffset;

	if (!integrationStepsSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		intNumValue = numString.ConvertToInteger ();
		ParameterServer::SetNumberOfIntegrationSteps (intNumValue);
	}

	startOffset = endOffset;

	if (!sigmaToleranceSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ParameterServer::SetGaussianSigmaTolerance (numValue);
	}

	startOffset = endOffset;

	if (!sigmaWidthSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ParameterServer::SetGaussianSigmaWidth (numValue);
	}

	startOffset = endOffset;

	if (!maxIterationsSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		intNumValue = numString.ConvertToInteger ();
		ParameterServer::SetMaximumSearchIterations (intNumValue);
	}

	startOffset = endOffset;

	if (!sigmaRatioSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ParameterServer::SetDoubleGaussianSigmaRatio (numValue);
	}

	startOffset = endOffset;

	if (!spacingSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ParameterServer::SetInterSampleSpacing (numValue);
	}

	startOffset = endOffset;

	if (!signatureSigmaSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ParametricCurve::SetSigmaForSignature (numValue);
	}

	startOffset = endOffset;

	if (!blobDegreeSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		intNumValue = numString.ConvertToInteger ();
		SuperGaussian::SetBlobDegree (intNumValue);
	}

	startOffset = endOffset;

	if (!minPeakDistanceSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		ChannelData::SetMinimumDistanceBetweenPeaks (numValue);
	}

	startOffset = endOffset;

	if (!endPointFractionSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		SampledData::SetPeakFractionForFlatCurveTest (numValue);
	}

	startOffset = endOffset;

	if (!endPointLevelSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		SampledData::SetPeakLevelForFlatCurveTest (numValue);
	}

	startOffset = endOffset;

	if (!lowTailHeightSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		TracePrequalification::SetLowHeightThreshold (numValue);
	}

	startOffset = endOffset;

	if (!lowTailSlopeSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		numValue = numString.ConvertToDouble ();
		TracePrequalification::SetLowSlopeThreshold (numValue);
	}

	startOffset = endOffset;

	if (!minRegressionSamplesSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else {

		intNumValue = numString.ConvertToInteger ();
		TracePrequalification::SetMinSamplesForSlopeRegression (intNumValue);
	}

	return returnValue;
}


//bool ParameterServer :: ReadLaneStdSearchParameters (const RGString& xmlString);


bool ParameterServer :: ReadLadderLocusSearchParameters (const RGString& xmlString) {

	RGString XMLString (xmlString);
	RGString numString;
	bool returnValue = true;
	size_t startOffset = 0;
	size_t endOffset = 0;
	RGString spacingString;
	double minSpacingMultiple;
	double maxSpacingMultiple;

	RGXMLTagSearch minCorrelationSearch ("LadderLociMinimumCorrelationWithIdeal", XMLString);
	RGXMLTagSearch locusSpacingSearch ("LadderLocusSearchSpacing", XMLString);
	RGXMLTagSearch startPointsSearch ("MaxStartPtsForPartialSearch", XMLString);

	if (!minCorrelationSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	startOffset = endOffset;

	if (!locusSpacingSearch.FindNextTag (startOffset, endOffset, spacingString))
		returnValue = false;

	else {

		if (!ReadSearchSpacingParameters (spacingString, minSpacingMultiple, maxSpacingMultiple))
			returnValue = false;
	}

	startOffset = endOffset;

	if (!startPointsSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	return returnValue;
}


bool ParameterServer :: ReadSearchSpacingParameters  (const RGString& xmlString, double& minMultiple, double& maxMultiple) {

	RGString XMLString (xmlString);
	RGString numString;
	bool returnValue = true;
	size_t startOffset = 0;
	size_t endOffset = 0;
	minMultiple = maxMultiple = 0.0;

	RGXMLTagSearch minSearch ("MinSpacingMultiple", XMLString);
	RGXMLTagSearch maxSearch ("MaxSpacingMultiple", XMLString);

	if (!maxSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else
		maxMultiple = numString.ConvertToDouble ();

	startOffset = endOffset;

	if (!minSearch.FindNextTag (startOffset, endOffset, numString))
		returnValue = false;

	else
		minMultiple = numString.ConvertToDouble ();

	return returnValue;
}


bool ParameterServer :: ReadRFULimits (const RGString& xmlString, RFULimitsStruct& rfuLimits) {

	RGString XMLString (xmlString);
	RGString result;
	RGXMLTagSearch minRFUSearch ("MinimumRFU", XMLString);
	RGXMLTagSearch maxRFUSearch ("MaximumRFU", XMLString);
	RGXMLTagSearch fractionOfMaxRFUSearch ("FractionOfMaxPeak", XMLString);
	RGXMLTagSearch pullupFractionOfMaxRFUSearch ("PullupFractionalFilter", XMLString);
	RGXMLTagSearch stutterThresholdSearch ("StutterThreshold", XMLString);
	RGXMLTagSearch adenylationThresholdSearch ("AdenylationThreshold", XMLString);
	size_t startOffset = 0;
	size_t endOffset = 0;

	if (minRFUSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		rfuLimits.minRFU = result.ConvertToDouble ();
	}

	else
		rfuLimits.minRFU = 1.0;

	if (!maxRFUSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	rfuLimits.maxRFU = result.ConvertToDouble ();

	if (!fractionOfMaxRFUSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	rfuLimits.fractionOfMaxPeak = result.ConvertToDouble ();

	if (pullupFractionOfMaxRFUSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		rfuLimits.pullupFractionOfMaxPeak = result.ConvertToDouble ();
	}

	else
		rfuLimits.pullupFractionOfMaxPeak = -1.0;

	if (!stutterThresholdSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	rfuLimits.stutterThreshold = result.ConvertToDouble ();

	if (!adenylationThresholdSearch.FindNextTag (startOffset, endOffset, result))
		return false;

//	startOffset = endOffset;
	rfuLimits.adenylationThreshold = result.ConvertToDouble ();
	return true;
}


bool ParameterServer :: ReadLadderLabLimits (const RGString& xmlString, RFULimitsStruct& rfuLimits) {

	RGString XMLString (xmlString);
	RGString result;
	RGString locusThresholdString;
	locusSpecificLimitsStruct limits;

	RGXMLTagSearch minRFUSearch ("MinimumRFU", XMLString);
	RGXMLTagSearch maxRFUSearch ("MaximumRFU", XMLString);
	RGXMLTagSearch fractionOfMaxRFUSearch ("FractionOfMaxPeak", XMLString);
	RGXMLTagSearch pullupFractionOfMaxRFUSearch ("PullupFractionalFilter", XMLString);
	RGXMLTagSearch stutterThresholdSearch ("StutterThreshold", XMLString);
	RGXMLTagSearch adenylationThresholdSearch ("AdenylationThreshold", XMLString);

	RGXMLTagSearch locusThresholdSearch ("LocusThreshold", XMLString);
	RGXMLTagSearch locusNameSearch ("LocusName", locusThresholdString);
	RGXMLTagSearch locusFractionalFilterSearch ("FractionOfMaxPeak", locusThresholdString);
	RGXMLTagSearch locusPullupFractionSearch ("PullupFractionalFilter", locusThresholdString);
	RGXMLTagSearch locusStutterSearch ("StutterThreshold", locusThresholdString);
	RGXMLTagSearch locusAdenylationSearch ("AdenylationThreshold", locusThresholdString);
	
	size_t startOffset = 0;
	size_t endOffset = 0;

	if (!minRFUSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	rfuLimits.minRFU = result.ConvertToDouble ();

	if (!maxRFUSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	rfuLimits.maxRFU = result.ConvertToDouble ();

	if (!fractionOfMaxRFUSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	rfuLimits.fractionOfMaxPeak = result.ConvertToDouble ();

	if (pullupFractionOfMaxRFUSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		rfuLimits.pullupFractionOfMaxPeak = result.ConvertToDouble ();
	}

	else
		rfuLimits.pullupFractionOfMaxPeak = -1.0;

	if (!stutterThresholdSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	rfuLimits.stutterThreshold = result.ConvertToDouble ();

	if (!adenylationThresholdSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	rfuLimits.adenylationThreshold = result.ConvertToDouble ();
	size_t startLocusOffset;
	size_t endLocusOffset;

	while (locusThresholdSearch.FindNextTag (startOffset, endOffset, locusThresholdString)) {

		startOffset = endOffset;
		startLocusOffset = 0;
		locusNameSearch.ResetSearch ();
		locusFractionalFilterSearch.ResetSearch ();
		locusPullupFractionSearch.ResetSearch ();
		locusStutterSearch.ResetSearch ();
		locusAdenylationSearch.ResetSearch ();

		if (!locusNameSearch.FindNextTag (startLocusOffset, endLocusOffset, result))
			return false;

		startLocusOffset = endLocusOffset;
		limits.Reset ();
		limits.locusName = result;

		if (locusFractionalFilterSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.fractionOfMaxPeak = result.ConvertToDouble ();
		}

		if (locusPullupFractionSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.pullupFractionalFilter = result.ConvertToDouble ();
		}

		if (locusStutterSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.stutterThreshold = result.ConvertToDouble ();
		}

		if (locusAdenylationSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.adenylationThreshold = result.ConvertToDouble ();
		}

		AddLadderLocusSpecificThreshold (limits);
	}

	return true;
}


bool ParameterServer :: ReadSampleLabLimits (const RGString& xmlString, RFULimitsStruct& rfuLimits) {

	RGString XMLString (xmlString);
	RGString result;
	RGString locusThresholdString;
	locusSpecificLimitsStruct limits;

	RGXMLTagSearch minRFUSearch ("MinimumRFU", XMLString);
	RGXMLTagSearch maxRFUSearch ("MaximumRFU", XMLString);
	RGXMLTagSearch fractionOfMaxRFUSearch ("FractionOfMaxPeak", XMLString);
	RGXMLTagSearch pullupFractionOfMaxRFUSearch ("PullupFractionalFilter", XMLString);
	RGXMLTagSearch stutterThresholdSearch ("StutterThreshold", XMLString);
	RGXMLTagSearch plusStutterThresholdSearch ("PlusStutterThreshold", XMLString);
	RGXMLTagSearch adenylationThresholdSearch ("AdenylationThreshold", XMLString);

	RGXMLTagSearch locusThresholdSearch ("LocusThreshold", XMLString);
	RGXMLTagSearch locusNameSearch ("LocusName", locusThresholdString);
	RGXMLTagSearch locusFractionalFilterSearch ("FractionOfMaxPeak", locusThresholdString);
	RGXMLTagSearch locusPullupFractionSearch ("PullupFractionalFilter", locusThresholdString);
	RGXMLTagSearch locusStutterSearch ("StutterThreshold", locusThresholdString);
	RGXMLTagSearch locusStutterSearchRight ("StutterThresholdRight", locusThresholdString);
	RGXMLTagSearch locusPlusStutterSearch ("PlusStutterThreshold", locusThresholdString);
	RGXMLTagSearch locusPlusStutterSearchRight ("PlusStutterThresholdRight", locusThresholdString);
	RGXMLTagSearch locusAdenylationSearch ("AdenylationThreshold", locusThresholdString);
	RGXMLTagSearch locusHeterozygousImbalanceSearch ("HeterozygousImbalanceLimit", locusThresholdString);
	RGXMLTagSearch locusBoundForHomozygoteSearch ("MinBoundForHomozygote", locusThresholdString);

	int totalLength = XMLString.Length ();
	size_t endDefaults;

	if (!XMLString.FindSubstring ("<LocusThreshold>", endDefaults))
		endDefaults = totalLength;
	
	size_t startOffset = 0;
	size_t endOffset = 0;

	if (minRFUSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		rfuLimits.minRFU = result.ConvertToDouble ();
	}

	else
		rfuLimits.minRFU = 1.0;

	if (!maxRFUSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	if (endOffset < endDefaults) {

		startOffset = endOffset;
		rfuLimits.maxRFU = result.ConvertToDouble ();
	}

	else
		rfuLimits.maxRFU = -1.0;

	if (!fractionOfMaxRFUSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	if (endOffset < endDefaults) {

		startOffset = endOffset;
		rfuLimits.fractionOfMaxPeak = result.ConvertToDouble ();
	}

	if (pullupFractionOfMaxRFUSearch.FindNextTag (startOffset, endOffset, result)) {

		if (endOffset < endDefaults) {

			startOffset = endOffset;
			rfuLimits.pullupFractionOfMaxPeak = result.ConvertToDouble ();
		}

		else
			rfuLimits.pullupFractionOfMaxPeak = -1.0;
	}

	else
		rfuLimits.pullupFractionOfMaxPeak = -1.0;

	if (!stutterThresholdSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	if (endOffset < endDefaults) {

		startOffset = endOffset;
		rfuLimits.stutterThreshold = result.ConvertToDouble ();
	}

	else
		rfuLimits.stutterThreshold = -1.0;

	if (plusStutterThresholdSearch.FindNextTag (startOffset, endOffset, result)) {

		if (endOffset < endDefaults) {

			startOffset = endOffset;
			rfuLimits.plusStutterThreshold = result.ConvertToDouble ();
		}

		else
			rfuLimits.plusStutterThreshold = -1.0;
	}

	else
		rfuLimits.plusStutterThreshold = -1.0;

	if (!adenylationThresholdSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	if (endOffset < endDefaults) {

		startOffset = endOffset;
		rfuLimits.adenylationThreshold = result.ConvertToDouble ();
	}

	else
		rfuLimits.adenylationThreshold = -1.0;

	size_t startLocusOffset;
	size_t endLocusOffset;

	while (locusThresholdSearch.FindNextTag (startOffset, endOffset, locusThresholdString)) {

		startOffset = endOffset;
		startLocusOffset = 0;
		locusNameSearch.ResetSearch ();
		locusFractionalFilterSearch.ResetSearch ();
		locusPullupFractionSearch.ResetSearch ();
		locusStutterSearch.ResetSearch ();
		locusStutterSearchRight.ResetSearch ();
		locusPlusStutterSearch.ResetSearch ();
		locusPlusStutterSearchRight.ResetSearch ();
		locusAdenylationSearch.ResetSearch ();
		locusHeterozygousImbalanceSearch.ResetSearch ();
		locusBoundForHomozygoteSearch.ResetSearch ();

		if (!locusNameSearch.FindNextTag (startLocusOffset, endLocusOffset, result))
			return false;

		if (result == "D21S11") {
			bool stopHere = true;
	}

		startLocusOffset = endLocusOffset;
		limits.Reset ();
		limits.locusName = result;

		if (locusFractionalFilterSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.fractionOfMaxPeak = result.ConvertToDouble ();
		}

		if (locusPullupFractionSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.pullupFractionalFilter = result.ConvertToDouble ();
		}

		if (locusStutterSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.stutterThreshold = result.ConvertToDouble ();

			if (locusStutterSearchRight.FindNextTag (startLocusOffset, endLocusOffset, result)) {

				startLocusOffset = endLocusOffset;
				limits.stutterThresholdRight = result.ConvertToDouble ();
			}
		}

		if (locusPlusStutterSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.plusStutterThreshold = result.ConvertToDouble ();

			if (locusPlusStutterSearchRight.FindNextTag (startLocusOffset, endLocusOffset, result)) {

				startLocusOffset = endLocusOffset;
				limits.plusStutterThresholdRight = result.ConvertToDouble ();
			}
		}

		if (locusAdenylationSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.adenylationThreshold = result.ConvertToDouble ();
		}

		if (locusHeterozygousImbalanceSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.heterozygousImbalanceLimit = result.ConvertToDouble ();
		}

		if (locusBoundForHomozygoteSearch.FindNextTag (startLocusOffset, endLocusOffset, result)) {

			startLocusOffset = endLocusOffset;
			limits.minBoundForHomozygote = result.ConvertToDouble ();
		}

		AddSampleLocusSpecificThreshold (limits);
	}

	RGString nsStutterThresholdString;
	RGString nsStutterLocusThresholdString;
	RGString nsRatioString;
	locusSpecificNonStandardStutterStruct nsLocusStutter;

	RGXMLTagSearch nonStandardStutterSearch ("NsStutterThresholds", XMLString);
	RGXMLTagSearch nonStandardLocusSearch ("Locus", nsStutterThresholdString);
	RGXMLTagSearch nonStandardLocusNameSearch ("Name", nsStutterLocusThresholdString);
	RGXMLTagSearch nonStandardLocusThresholdSearch ("Threshold", nsStutterLocusThresholdString);
	RGXMLTagSearch nonStandardLocusBpSearch ("Bps", nsRatioString);
	RGXMLTagSearch nonStandardLocusRatioSearch ("Ratio", nsRatioString);

	cout << "Start offset for non-standard stutter = " << startOffset << endl;

	if (nonStandardStutterSearch.FindNextTag (startOffset, endOffset, nsStutterThresholdString)) {

		startOffset = endOffset;
		startLocusOffset = endLocusOffset = 0;
		nonStandardLocusSearch.ResetSearch ();

		while (nonStandardLocusSearch.FindNextTag (startLocusOffset, endLocusOffset, nsStutterLocusThresholdString)) {

			startLocusOffset = endLocusOffset;
			nonStandardLocusNameSearch.ResetSearch ();
			nonStandardLocusThresholdSearch.ResetSearch ();
			size_t locusThresholdStartOffset = 0;
			size_t locusThresholdEndOffset = 0;
			nsLocusStutter.Reset ();

			if (!nonStandardLocusNameSearch.FindNextNonemptyTag (locusThresholdStartOffset, locusThresholdEndOffset, result)) {

				cout << "Could not find locus name in non-standard stutter set-up" << endl;
				return false;
			}

			locusThresholdStartOffset = locusThresholdEndOffset;
			nsLocusStutter.locusName = result;

			while (nonStandardLocusThresholdSearch.FindNextTag (locusThresholdStartOffset, locusThresholdEndOffset, nsRatioString)) {

				locusThresholdStartOffset = locusThresholdEndOffset;
				nonStandardLocusBpSearch.ResetSearch ();
				nonStandardLocusRatioSearch.ResetSearch ();
				size_t thisStart = 0;
				size_t thisEnd = 0;
				int bp;
				double threshold;

				if (!nonStandardLocusBpSearch.FindNextTag (thisStart, thisEnd, result)) {

					cout << "Could not find bps in non-standard stutter search for locus named " << (char*) nsLocusStutter.locusName.GetData () << endl;
					return false;
				}

				bp = result.ConvertToInteger ();

				if (!nonStandardLocusRatioSearch.FindNextTag (thisStart, thisEnd, result)) {

					cout << "Could not find ratio in non-standard stutter search for locus named " << (char*) nsLocusStutter.locusName.GetData () << endl;
					return false;
				}

				threshold = result.ConvertToDouble ();
				nsLocusStutter.SetNonStandardStutterThreshold (bp, threshold);
				cout << "Found non-standard stutter threshold for locus named " << (char*) nsLocusStutter.locusName.GetData () << " with base pair displacement = " << bp << " and threshold = " << threshold << endl;
			}

			AddSampleLocusSpecificNonStandardStutterCollection (nsLocusStutter);
		}		
	}

	return true;
}


bool ParameterServer :: ReadLabNonRFULimits (size_t startOffset, size_t& endOffset, const RGString& xmlString, labNonRFULimitsStruct& nonRFULimits) {

	RGString XMLString (xmlString);
	RGString result;
	RGXMLTagSearch heteroImbalanceSearch ("HeterozygousImbalanceLimit", XMLString);
	RGXMLTagSearch minBoundForHomozygoteSearch ("MinBoundForHomozygote", XMLString);
	RGXMLTagSearch maxNumberPullupsSearch ("MaxNumberOfPullupsPerSample", XMLString);	// SM: Optional
	RGXMLTagSearch maxNumberStutterSearch ("MaxNumberOfStutterPeaksPerSample", XMLString);	// SM: Optional
	RGXMLTagSearch maxNumberSpikesSearch ("MaxNumberOfSpikesPerSample", XMLString);	// SM: Optional
	RGXMLTagSearch maxNumberAdenylationSearch ("MaxNumberOfAdenylationsPerSample", XMLString);	// SM: Optional
	RGXMLTagSearch maxNumberOLAllelesSearch ("MaxNumberOfOLAllelesPerSample", XMLString);	// SM: Optional
	RGXMLTagSearch maxResidualSearch ("MaxResidualForAlleleCall", XMLString);
	RGXMLTagSearch minBPSForArtifactSearch ("MinBPSForArtifacts", XMLString);
	RGXMLTagSearch boundForOverloadSearch ("AlleleRFUOverloadThreshold", XMLString);
	RGXMLTagSearch minBoundForHomozygoteUnitSearch ("MinBoundHomozygoteUnit", XMLString);
	RGXMLTagSearch maxNumberExcessiveResidualsSearch ("MaxExcessiveResidual", XMLString);	// SM: Optional
//	size_t startOffset = 0;
//	size_t endOffset = 0;

	RGString msgThresholdList;
	size_t localStartOffset;
	size_t localEndOffset;
	RGXMLTagSearch thresholdsSearch ("SmartMessageThresholds", XMLString);
	RGXMLTagSearch messageThresholdSearch ("MessageThreshold", msgThresholdList);

	RGString messageThresholdString;
	size_t msgStartOffset;
	size_t msgEndOffset;
	RGXMLTagSearch smartMessageNameSearch ("MsgName", messageThresholdString);
	RGXMLTagSearch smartMessageThresholdSearch ("MsgThreshold", messageThresholdString);

	RGString nameString;
	int threshold;

	STRBaseSmartMessage* nextSMThreshold;	// just for name and messageNumber

	if (!heteroImbalanceSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	nonRFULimits.heterozygousImbalanceLimit = result.ConvertToDouble ();

	if (!minBoundForHomozygoteSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	double temp;
//	nonRFULimits.minBoundForHomozygote = result.ConvertToDouble ();
	temp = result.ConvertToDouble ();

	if (minBoundForHomozygoteUnitSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;

		if (result == "%") {

			Locus::SetExpectRFUUnitsForHomozygoteBound (false);

			if (temp < 10.0)
				nonRFULimits.minBoundForHomozygote = temp * STRSampleChannelData::GetMinRFU ();

			else
				nonRFULimits.minBoundForHomozygote = 0.01 * temp * STRSampleChannelData::GetMinRFU ();
		}

		else {

			nonRFULimits.minBoundForHomozygote = temp;
			Locus::SetExpectRFUUnitsForHomozygoteBound (true);
		}
	}

	else
		nonRFULimits.minBoundForHomozygote = temp;

	if (maxNumberPullupsSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		nonRFULimits.maxNumberPullupsPerSample = result.ConvertToInteger ();
		threshold = result.ConvertToInteger ();

		// as in succeeding statements, "threshold" is stored as "message number" for retrieval later
		nextSMThreshold = new STRBaseSmartMessage (threshold, "smNumberOfPullUpsInSample");

		if (nextSMThreshold != NULL)
			mSmartMessageThresholds->Append (nextSMThreshold);
	}

	if (maxNumberStutterSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		nonRFULimits.maxNumberStutterPerSample = result.ConvertToInteger ();
		threshold = result.ConvertToInteger ();

		nextSMThreshold = new STRBaseSmartMessage (threshold, "smNumberOfStuttersInSample");

		if (nextSMThreshold != NULL)
			mSmartMessageThresholds->Append (nextSMThreshold);
	}

	if (maxNumberSpikesSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		nonRFULimits.maxNumberSpikesPerSample = result.ConvertToInteger ();

		threshold = result.ConvertToInteger ();

		nextSMThreshold = new STRBaseSmartMessage (threshold, "smNumberOfSpikesInSample");

		if (nextSMThreshold != NULL)
			mSmartMessageThresholds->Append (nextSMThreshold);
	}

	if (maxNumberAdenylationSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		nonRFULimits.maxNumberAdenylationPerSample = result.ConvertToInteger ();
		threshold = result.ConvertToInteger ();

		nextSMThreshold = new STRBaseSmartMessage (threshold, "smNumberOfAdenylationsInSample");

		if (nextSMThreshold != NULL)
			mSmartMessageThresholds->Append (nextSMThreshold);
	}

	if (maxNumberOLAllelesSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		nonRFULimits.maxNumberOLAllelesPerSample = result.ConvertToInteger ();
		threshold = result.ConvertToInteger ();

		nextSMThreshold = new STRBaseSmartMessage (threshold, "smNumberOfOffLadderAllelesInSample");

		if (nextSMThreshold != NULL)
			mSmartMessageThresholds->Append (nextSMThreshold);
	}

	if (!maxResidualSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	nonRFULimits.maxResidualForAlleleCall = result.ConvertToDouble ();

	if (maxNumberExcessiveResidualsSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		nonRFULimits.maxNumberExcessiveResidualsPerSample = result.ConvertToInteger ();
		threshold = result.ConvertToInteger ();

		nextSMThreshold = new STRBaseSmartMessage (threshold, "smNumberOfExcessiveResidualsInSample");

		if (nextSMThreshold != NULL)
			mSmartMessageThresholds->Append (nextSMThreshold);
	}

	else
		nonRFULimits.maxNumberExcessiveResidualsPerSample = -1;

	if (!minBPSForArtifactSearch.FindNextTag (startOffset, endOffset, result))
		return false;

	startOffset = endOffset;
	nonRFULimits.minBPSForArtifacts = result.ConvertToInteger ();

	if (boundForOverloadSearch.FindNextTag (startOffset, endOffset, result)) {

		startOffset = endOffset;
		nonRFULimits.alleleRFUOverloadThreshold = result.ConvertToDouble ();
//		cout << "Allele Overload Threshold = " << rfuLimits.alleleRFUOverloadThreshold << endl;
	}

	else
		nonRFULimits.alleleRFUOverloadThreshold = -1.0;

	if (thresholdsSearch.FindNextTag (startOffset, endOffset, msgThresholdList)) {

		startOffset = endOffset;
		messageThresholdSearch.ResetSearch ();
		localStartOffset = 0;

		while (messageThresholdSearch.FindNextTag (localStartOffset, localEndOffset, messageThresholdString)) {

			localStartOffset = localEndOffset;
			msgStartOffset = 0;
			smartMessageNameSearch.ResetSearch ();
			smartMessageThresholdSearch.ResetSearch ();

			if (!smartMessageNameSearch.FindNextTag (msgStartOffset, msgEndOffset, nameString)) {

				cout << "Could not read smart message name in lab settings" << endl;
				cout << "String = " << messageThresholdString.GetData () << endl;
				return false;
			}

			msgStartOffset = msgEndOffset;
	//		cout << "Found threshold name = " << nameString.GetData () << endl;

			if (!smartMessageThresholdSearch.FindNextTag (msgStartOffset, msgEndOffset, result)) {

				cout << "Could not read smart message threshold for message named " << nameString.GetData () << " in lab settings" << endl;
				return false;
			}

	//		cout << "Found threshold = " << result.GetData () << endl;
			msgStartOffset = msgEndOffset;
			threshold = result.ConvertToInteger ();

			nextSMThreshold = new STRBaseSmartMessage (threshold, nameString);
			mSmartMessageThresholds->Append (nextSMThreshold);
		}
	}

	return true;
}


//int ParameterServer :: ReadSampleTypeSpecifications (size_t startOffset, size_t& endOffset, const RGString& xmlString) {
//
//	RGString XMLString (xmlString);
//	RGString result;
//	RGXMLTagSearch categorySearch ("SpecimenCategory", XMLString);
//	size_t localStart = startOffset;
//	endOffset = startOffset;
//	int scanResult;
//
//	while (true) {
//
//		if (!categorySearch.FindNextTag (localStart, endOffset, result)) {
//
//			localStart = endOffset;
//
//			if (!mSingleSourceSampleIDs->IsEmpty ()) {
//
//				if (!mMixtureIDs->IsEmpty ()) {
//
//					delete mMixtureIDs;
//					mMixtureIDs = new SynonymList;
//				}
//
//				return 1;
//			}
//
//			if (!mMixtureIDs->IsEmpty ())
//				return 1;
//
//			return 0;
//		}
//
//		localStart = endOffset;
//		scanResult = ReadSampleTypeSpec (result);
//
//		if (scanResult == -1)
//			return -1;
//	}
//
//	return 0;
//}
//
//
//int ParameterServer :: ReadSampleTypeSpec (const RGString& xmlString) {
//
//	RGString XMLString (xmlString);
//	RGString result;
//	RGXMLTagSearch typeSearch ("Type", XMLString);
//	RGXMLTagSearch synonymSearch ("Synonym", XMLString);
//	size_t startOffset = 0;
//	size_t endOffset = 0;
//	int i = 0;
//	int type = 0;
//
//	if (!typeSearch.FindNextTag (startOffset, endOffset, result))
//		return -1;
//
//	startOffset = endOffset;
//
//	if (result == "Single Source")
//		type = 1;
//
//	else if (result == "Possible Mixture")
//		type = 2;
//
//	else
//		return 0;
//
//	//synonymSearch.ResetSearch ();
//
//	while (synonymSearch.FindNextTag (startOffset, endOffset, result)) {
//
//		startOffset = endOffset;
//		i++;
//		ParameterServer::UnescapeXML (result);
//
//		if (type == 1)
//			mSingleSourceSampleIDs->AddSynonym (result);
//
//		else
//			mMixtureIDs->AddSynonym (result);
//	}
//
//	if (i == 0)
//		return -1;
//
//	return 1;
//}


bool ParameterServer :: SetAllSmartMessageThresholds () {

	STRBaseSmartMessage* nextLink;
	RGDListIterator it (*mSmartMessageThresholds);
	RGString name;
	int threshold;
	SmartMessage* sm;
	STRBaseSmartMessage bsm;

	while (nextLink = (STRBaseSmartMessage*) it ()) {

		name = nextLink->GetName ();
		threshold = nextLink->GetIntegerData ();

		sm = bsm.FindMessageByName (name);

		if (sm != NULL) {

			sm->SetThreshold (threshold);

			if (name == "smSuppressAlleleAndArtifactCallsRightOfMaxPreset") {

				cout << "Setting threshold for " << name.GetData () << ":  " << threshold << endl;
				cout << "Initial value = " << sm->GetInitialValue () << endl;
				cout << "Scope = " << sm->GetScope () << endl;
			}
		}

		else
			cout << "Threshold message named " << name.GetData () << " not found...assumed no longer in MessageBook" << endl;
	}

	return true;
}


bool ParameterServer :: LadderDoesTargetStringEqualASynonymCaseIndep (const RGString& target) {

	return LadderIDs->DoesTargetStringEqualASynonymCaseIndep (target);
}


bool ParameterServer :: LadderDoesTargetStringContainASynonymCaseIndep (const RGString& target) {

	return LadderIDs->DoesTargetStringContainASynonymCaseIndep (target);
}

bool ParameterServer :: ControlDoesTargetStringEqualASynonymCaseIndep (const RGString& target) {

	return ControlIDs->DoesTargetStringEqualASynonymCaseIndep (target);
}


bool ParameterServer :: ControlDoesTargetStringContainASynonymCaseIndep (const RGString& target) {

	return ControlIDs->DoesTargetStringContainASynonymCaseIndep (target);
}


bool ParameterServer :: AddLabPositiveControlsToControlStrings (GenotypesForAMarkerSet* genotypes) {

	genotypes->AddLabPositiveControlStringsToControlLists (ControlIDs, PositiveControlIDs);
	return true;
}


bool ParameterServer :: PosControlDoesTargetStringEqualASynonymCaseIndep (const RGString& target) {

	return PositiveControlIDs->DoesTargetStringEqualASynonymCaseIndep (target);
}


bool ParameterServer :: PosControlDoesTargetStringContainASynonymCaseIndep (const RGString& target) {

	return PositiveControlIDs->DoesTargetStringContainASynonymCaseIndep (target);
}


bool ParameterServer :: NegControlDoesTargetStringEqualASynonymCaseIndep (const RGString& target) {

	return NegativeControlIDs->DoesTargetStringEqualASynonymCaseIndep (target);
}


bool ParameterServer :: NegControlDoesTargetStringContainASynonymCaseIndep (const RGString& target) {

	return NegativeControlIDs->DoesTargetStringContainASynonymCaseIndep (target);
}


bool ParameterServer :: DoesTargetStringEqualMixtureCriteriaCaseIndep (const RGString& target) {

	if (mSingleSourceSampleIDs->IsEmpty () && mMixtureIDs->IsEmpty ())
		return false;

	if (!mSingleSourceSampleIDs->IsEmpty ()) {

		if (mSingleSourceSampleIDs->DoesTargetStringEqualASynonymCaseIndep (target))
			return false;

		return true;	// We can do this because we don't get here if sample is positive or negative control
	}

	if (mMixtureIDs->DoesTargetStringEqualASynonymCaseIndep (target))
		return true;

	return false;
}


bool ParameterServer :: DoesTargetStringContainMixtureCriteriaCaseIndep (const RGString& target) {

	if (mSingleSourceSampleIDs->IsEmpty () && mMixtureIDs->IsEmpty ())
		return false;

	if (!mSingleSourceSampleIDs->IsEmpty ()) {

		if (mSingleSourceSampleIDs->DoesTargetStringContainASynonymCaseIndep (target))
			return false;

		return true;	// We can do this because we don't get here if sample is positive or negative control
	}

	if (mMixtureIDs->DoesTargetStringContainASynonymCaseIndep (target))
		return true;

	return false;
}


void ParameterServer :: UnescapeXML (RGString& target) {

	target.FindAndReplaceAllSubstrings ("&quot;", "\"");
	target.FindAndReplaceAllSubstrings ("&gt;", ">");
	target.FindAndReplaceAllSubstrings ("&lt;", "<");
	target.FindAndReplaceAllSubstrings ("amp;", "&");
}


bool ParameterServer :: ReadFileNameStrings (const RGString& xmlString) {

	RGString XMLString (xmlString);
	RGString nameString;
	RGString synonymString;
	RGXMLTagSearch nameStringSearch ("NameStrings", XMLString);
	RGXMLTagSearch ladderStringSearch ("LadderStrings", nameString);
	RGXMLTagSearch posCtrlStringSearch ("StdPositveControlStrings", nameString);
	RGXMLTagSearch negCtrlStringSearch ("NegativeControlStrings", nameString);
	RGXMLTagSearch singleSourceStringSearch ("SingleSourceStrings", nameString);
	RGXMLTagSearch possibleMixtureStringSearch ("PossibleMixtureStrings", nameString);
	RGXMLTagSearch stdControlNameSearch ("StandardControlName", nameString);
	RGXMLTagSearch SynonymSearch ("Synonym", synonymString);
	size_t startOffset = 0;
	size_t endOffset = 0;

	if (!nameStringSearch.FindNextTag (startOffset, endOffset, nameString)) {

		return false;
	}

	ladderStringSearch.ResetSearch ();
	posCtrlStringSearch.ResetSearch ();
	negCtrlStringSearch.ResetSearch ();
	stdControlNameSearch.ResetSearch ();
	singleSourceStringSearch.ResetSearch ();
	possibleMixtureStringSearch.ResetSearch ();

	startOffset = 0;
	endOffset = 0;
	int i = 0;
	RGString individualSynonymString;
	size_t synStartOffset = 0;
	size_t synEndOffset = 0;

	if (ladderStringSearch.FindNextTag (startOffset, endOffset, synonymString)) {

		SynonymSearch.ResetSearch ();

		while (SynonymSearch.FindNextTag (synStartOffset, synEndOffset, individualSynonymString)) {

			synStartOffset = synEndOffset;
			i++;
			ParameterServer::UnescapeXML (individualSynonymString);
			LadderIDs->AddSynonym (individualSynonymString);
		}

		if (i == 0)
			return false;
	}

	else
		return false;

	startOffset = endOffset;

	// now do positive controls and negative controls...

	if (posCtrlStringSearch.FindNextTag (startOffset, endOffset, synonymString)) {

		SynonymSearch.ResetSearch ();
		synStartOffset = synEndOffset = 0;
		i = 0;

		while (SynonymSearch.FindNextTag (synStartOffset, synEndOffset, individualSynonymString)) {

			synStartOffset = synEndOffset;
			i++;
			ParameterServer::UnescapeXML (individualSynonymString);
			PositiveControlIDs->AddSynonym (individualSynonymString);
			ControlIDs->AddSynonym (individualSynonymString);
		}

		if (i == 0)
			return false;
	}

	else
		return false;

	startOffset = endOffset;

	if (negCtrlStringSearch.FindNextTag (startOffset, endOffset, synonymString)) {

		SynonymSearch.ResetSearch ();
		synStartOffset = synEndOffset = 0;
		i = 0;

		while (SynonymSearch.FindNextTag (synStartOffset, synEndOffset, individualSynonymString)) {

			synStartOffset = synEndOffset;
			i++;
			ParameterServer::UnescapeXML (individualSynonymString);
			NegativeControlIDs->AddSynonym (individualSynonymString);
			ControlIDs->AddSynonym (individualSynonymString);
		}

		if (i == 0)
			return false;
	}

	else
		return false;

	startOffset = endOffset;
	cout << "Starting to read single source vs mixture strings...." << endl;

	if (singleSourceStringSearch.FindNextTag (startOffset, endOffset, synonymString)) {

		SynonymSearch.ResetSearch ();
		synStartOffset = synEndOffset = 0;
		cout << "Found single source tag name..." << endl;

		while (SynonymSearch.FindNextTag (synStartOffset, synEndOffset, individualSynonymString)) {

			cout << "Found single source string:  " << (char*)individualSynonymString.GetData () << endl;
			synStartOffset = synEndOffset;
			ParameterServer::UnescapeXML (individualSynonymString);
			mSingleSourceSampleIDs->AddSynonym (individualSynonymString);
		}
	}

	startOffset = endOffset;
	cout << "Starting to read mixture strings...." << endl;

	if (possibleMixtureStringSearch.FindNextTag (startOffset, endOffset, synonymString)) {

		SynonymSearch.ResetSearch ();
		synStartOffset = synEndOffset = 0;
		cout << "Found mixture tag name..." << endl;

		while (SynonymSearch.FindNextTag (synStartOffset, synEndOffset, individualSynonymString)) {

			cout << "Found mixture string:  " << (char*)individualSynonymString.GetData () << endl;
			synStartOffset = synEndOffset;
			ParameterServer::UnescapeXML (individualSynonymString);
			mMixtureIDs->AddSynonym (individualSynonymString);
		}
	}

	if (!mSingleSourceSampleIDs->IsEmpty ())
		cout << "Found single source synonyms" << endl;

	if (!mMixtureIDs->IsEmpty ())
		cout << "Found mixture synonyms" << endl;

	if (!mSingleSourceSampleIDs->IsEmpty ()) {

		if (!mMixtureIDs->IsEmpty ()) {

			delete mMixtureIDs;
			mMixtureIDs = new SynonymList;
		}
	}

	RGString tempName;

	if (!stdControlNameSearch.FindNextTag (startOffset, endOffset, tempName))
		return false;

	startOffset = endOffset;
	ParameterServer::UnescapeXML (tempName);
	*mStandardPositiveControlName = tempName;

	//if (ReadSampleTypeSpecifications (startOffset, endOffset, XMLString) < 0)
	//	return false;

	//startOffset = endOffset;
	return true;
}

