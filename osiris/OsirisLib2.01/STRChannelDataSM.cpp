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
*  FileName: STRChannelDataSM.cpp
*  Author:   Robert Goor
*
*/
//
//     class STRChannelData is the prototype channel for STR forensics.  It maintains a SampledData object and analyzes it.  Then, there
//  are the subclasses for Internal Lane Standard channels, ladder channels and sample channels.  These are the Smart
//  Message functions
//

#include "STRChannelData.h"
#include "TracePrequalification.h"
#include "TestCharacteristic.h"
#include "Genetics.h"
#include "DataSignal.h"
#include "OsirisMsg.h"
#include "RGTextOutput.h"
#include "CoreBioComponent.h"
#include "STRCoreBioComponent.h"
#include "OutputLevelManager.h"
#include "Notices.h"
#include "ListFunctions.h"
#include "RecursiveInnerProduct.h"
#include "SmartNotice.h"
#include "STRSmartNotices.h"


bool STRChannelData :: FindLimitsOnPrimaryPullupPeaks () {

	// Add effects of negative pull-up, or pull-down, from improved CrossChannelAnalysisSM

	RGDListIterator it (SmartPeaks);
	DataSignal* nextSignal;
	double maxPeakNotPrimaryPullup = 0.0;
	double peak;

	smCrater crater;
	smPullUp pullup;
	smPrimaryInterchannelLink primaryPullup;

	while (nextSignal = (DataSignal*) it ()) {

		//if (nextSignal->GetMessageValue (crater) || nextSignal->GetMessageValue (pullup))
		//	continue;

		if (nextSignal->IsPrimaryCrossChannelSignalLink ())
			continue;

		peak = nextSignal->Peak ();

		if (peak > maxPeakNotPrimaryPullup)
			maxPeakNotPrimaryPullup = peak;
	}

	if (maxPeakNotPrimaryPullup == 0.0)
		return false;

	bool status = false;
	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		//if (!nextSignal->GetMessageValue (crater)) {

			if (nextSignal->Peak () <= maxPeakNotPrimaryPullup) {

				nextSignal->SetCannotBePrimary (true);
				status = true;
			}
		//}
	}

	return status;
}


bool STRChannelData :: ValidateAndCorrectCrossChannelAnalysisSM () {

	//RecalculatePrimarySignalBasedOnValiditySM

	RGDListIterator it (SmartPeaks);
	DataSignal* nextSignal;

	InterchannelLinkage* link;
	bool status = false;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->HasCrossChannelSignalLink ()) {

			link = nextSignal->GetInterchannelLink ();
			
			if (link->RecalculatePrimarySignalBasedOnValiditySM ())
				status = true;
		}
	}

	return status;
}


int STRChannelData :: WriteSmartPeakInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& tagName) {

	RGDListIterator it (CompleteCurveList);
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) it ())
		nextSignal->WriteSmartPeakInfoToXML (text, indent, tagName, "meanbps");

	return 0;
}


int STRChannelData :: WriteSmartArtifactInfoToXML (RGTextOutput& text, const RGString& indent, ChannelData* laneStd) {

	RGDListIterator it (SmartPeaks);
//	RGDListIterator it2 (SupplementalArtifacts);
	DataSignal* nextSignal;
	//double left;
	//double right;

	//if (laneStd != NULL) {

	//	left = laneStd->GetFirstAnalyzedMean ();
	//	right = laneStd->GetLastAnalyzedMean ();
	//}

	//else {

	//	left = 0.0;
	//	right = DOUBLEMAX;
	//}

	//double mean;

	//if (right < -100.0)
	//	return -1;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->DontLook ())
			continue;

		//mean = nextSignal->GetMean ();

		//if (mean < left)
		//	continue;

		//if (mean > right)
		//	break;

		nextSignal->WriteSmartArtifactInfoToXML (text, indent, "artifact", "meanbps");
	}

	return 0;
}


int STRChannelData :: AnalyzeDynamicBaselineSM (int startTime) {

	return 0;
}


int STRChannelData :: AnalyzeDynamicBaselineAndNormalizeRawDataSM (int startTime) {

	return 1;
}


int STRChannelData :: SetDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	return ChannelData::SetDataSM (fileData, testControlPeak, testSamplePeak);
}


int STRChannelData :: SetRawDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	return ChannelData::SetRawDataSM (fileData, testControlPeak, testSamplePeak);
}


int STRChannelData :: SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	return ChannelData::SetRawDataFromColorCorrectedArraySM (dataArray, arraySize, testControlPeak, testSamplePeak);
}


int STRChannelData :: TestNeighborsSM (DataSignal* previous, DataSignal* testSignal, DataSignal* following) {

	double peak = testSignal->Peak ();
	Boolean NotAcceptable = FALSE;
	Boolean Marginal = FALSE;
	RGString info;
	double stutterLimit = STRLaneStandardChannelData::GetILSStutterThreshold ();
	double adenylationLimit = STRLaneStandardChannelData::GetILSAdenylationThreshold ();
	smPullUp pullup;
	
	double ThisMean = testSignal->GetMean ();
	double ThisPeak = testSignal->Peak ();
	double ThisSigma = testSignal->GetStandardDeviation ();

	//
	// test for non-template addition (-A and +A) and for stutter (n - 4 and n + 4)
	//

	if ((following != NULL) && (!following->GetMessageValue (pullup))) {

		double NextMean = following->GetMean ();
		double NextPeak = following->Peak ();

		if (NextMean - ThisMean < 8.0 * ThisSigma) {

			if (ThisPeak < adenylationLimit * NextPeak) {

				NotAcceptable = TRUE;
				testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
					"Signal represents non-template addition (-A) for following peak");
			}
		}

		else if ((NextMean - ThisMean < 24.0 * ThisSigma) && (ThisPeak < stutterLimit * NextPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
				"Signal represents stutter for following peak");
		}
	}

	if ((previous != NULL) && (!previous->GetMessageValue (pullup))) {

		double PreviousMean = previous->GetMean ();
		double PreviousPeak = previous->Peak ();

		if ((ThisMean - PreviousMean < 8.0 * ThisSigma) && (ThisPeak < adenylationLimit * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", 
				"Signal represents non-template addition (+A) for previous peak");
		}

		else if ((ThisMean - PreviousMean < 24.0 * ThisSigma) && (ThisPeak < stutterLimit * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", 
				"Signal represents stutter for previous peak");
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


int STRChannelData :: TestSignalsForOffScaleSM () {

	RGDListIterator it (SmartPeaks);
	DataSignal* nextSignal;

	smCrater crater;
	smSignalNotACrater notCrater;
	smPrimaryInterchannelLink primaryPullup;
	smPullUp pullup;
	smNotInterchannelLink notInterchannel;

	smLaserOffScalePullupOrCraterNotPrimary laserOffScaleNotPrimary;
	smLaserOffScalePrimary laserOffScalePrimary;
	smLaserOffScaleNoInterchannelLink laserOffScaleNoInterchannelLink;

	bool isPullup;
	bool isCrater;
	bool isPrimary;
	bool isNotInterchannel;

	while (nextSignal = (DataSignal*) it ()) {

		if (CoreBioComponent::TestForOffScale (nextSignal->GetMean ())) {

			isPullup = nextSignal->GetMessageValue (pullup);
			isCrater = (nextSignal->GetMessageValue (crater) && !nextSignal->GetMessageValue (notCrater));
			isPrimary = nextSignal->GetMessageValue (primaryPullup);
			isNotInterchannel = (!isPullup || nextSignal->GetMessageValue (notInterchannel));

			if ((isPullup || isCrater) && !isPrimary && !isNotInterchannel)
				nextSignal->SetMessageValue (laserOffScaleNotPrimary, true);

			//else if (isPrimary  && !isNotInterchannel)
			else if (isPrimary)
				nextSignal->SetMessageValue (laserOffScalePrimary, true);

			else
				nextSignal->SetMessageValue (laserOffScaleNoInterchannelLink, true);
		}
	}

	return 0;
}


int STRLaneStandardChannelData :: SetDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	int status = STRChannelData::SetDataSM (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLaneStandardChannelData :: SetRawDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	int status = STRChannelData::SetRawDataSM (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLaneStandardChannelData :: SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	int status = STRChannelData::SetRawDataFromColorCorrectedArraySM (dataArray, arraySize, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLaneStandardChannelData :: AnalyzeLaneStandardChannelRecursivelySM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is ladder and sample stage 1
	//

	int status = 0;

	RGDListIterator CurveIterator (PreliminaryCurveList);
	FinalCurveList.Clear ();
	DataSignal* FollowingSignal;
	DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
	int TestResult;
	int Size = mLaneStandard->GetNumberOfCharacteristics ();
	RGString info;
	RGDList overFlow;
	double scaleFactor = 0.34;
	double maxPeak;
	double endTestFactor = 0.335;
	double junkEndTestProportion = 0.40;
	int curvesLeft;
	double lowLevelScaleFactor = 0.25;
	double additionalJunkInterval = 0.0;
	double lastPeak;
	RGDListIterator overflowIterator (overFlow);
	RGDList indivList;
	RGString data;
	RGDList tempCurveList;

	smPullUp pullup;
	smPrimaryInterchannelLink primaryPullup;
	smILSTooFewPeaks tooFewPeaks;
	smCannotSeparateILSPrimerPeaks initialFragments;
	smILSNeighborFilterTooAggressive neighborFilterTooAggressive;
	smHeightBelowFractionalFilter fractionalFilterHeight;
	smILSFractionFilterTooAggressive fractionalFilterTooAggressive;
	smUnexpectedILSPeak unexpectedPeak;
	smILSRelativeHeightsInconsistent relativeHeightsInconsistent;
	smILSSpacingFailed spacingFailed;
	smSignalIsCtrlPeak isControlPeak;
	smPeakHeightAboveMax peakAboveMax;
	smILSSpacingIsMarginal ilsSpacingIsMarginal;
	smSignalNotACrater notACrater;
	smSignalNotACraterSidePeak notACraterSidePeak;
	smCrater crater;
	smCraterSidePeak craterSidePeak;

	double maxAllowable = STRLaneStandardChannelData::GetMaxRFU ();

	RGDList totallyTempCurveList;

	double endOfJunk;

//	maxPeak = Locus::MaximumPeakHeight (PreliminaryCurveList);


	maxPeak = 0.0;
	double currentPeak;
	RGDListIterator itt (PreliminaryCurveList);

	while (nextSignal = (DataSignal*) itt ()) {

		if ((nextSignal->GetMessageValue (pullup)) && (!nextSignal->GetMessageValue (primaryPullup)))
			continue;

		currentPeak = nextSignal->Peak ();

		if ((maxAllowable > 0.0) && (currentPeak >= maxAllowable)) {

			nextSignal->SetMessageValue (peakAboveMax, true);
			continue;
		}

		if (currentPeak > maxPeak) {

			maxPeak = currentPeak;
		}
	}

	CurveIterator.ResetToEnd ();

	while (true) {

		nextSignal = (DataSignal*)CurveIterator.CurrentItem ();

		if (nextSignal == NULL)
			break;

		if ((nextSignal->GetMessageValue (pullup)) && (!nextSignal->GetMessageValue (primaryPullup))) {

			--CurveIterator;
			continue;
		}

		if (nextSignal->Peak () > maxPeak) {

			--CurveIterator;
			continue;
		}

		break;
	}

	itt.ResetToEnd ();
	int peaksProcessed = 0;
	int peaksChosen = 0;
	double maxPeakOnRight = 0.0;
	double avePeak = 0.0;
//	RGDList maxima;
//	RGPDouble* newDouble;
	double secondaryPeak = 0.0;

	while (peaksProcessed < Size) {

		nextSignal = (DataSignal*) itt.CurrentItem ();

		if (nextSignal == NULL)
			break;

		currentPeak = nextSignal->Peak ();
		peaksProcessed++;

//		newCurveList.Prepend (nextSignal);

		if ((nextSignal->GetMessageValue (pullup)) && (!nextSignal->GetMessageValue (primaryPullup)))
			continue;

		if (nextSignal->GetMessageValue (peakAboveMax))
			continue;

		--itt;
		peaksChosen++;
		avePeak += currentPeak;

		if (currentPeak > maxPeakOnRight) {

			secondaryPeak = maxPeakOnRight;
			maxPeakOnRight = currentPeak;
		}

		else if (currentPeak > secondaryPeak)
			secondaryPeak = currentPeak;
	}

	nextSignal = (DataSignal*)CurveIterator.CurrentItem ();

	if (Size <= 0) {

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are no expected peaks for the ILS.\n";
		status = -1;
		SetMessageValue (tooFewPeaks, true);
		AppendDataForSmartMessage (tooFewPeaks, PreliminaryCurveList.Entries ());
		AppendDataForSmartMessage (tooFewPeaks, Size);

		cout << ErrorString << endl;
		cout << "There are " << PreliminaryCurveList.Entries () << " curves needed out of " << Size << endl;
		return -50;
	}

	if (nextSignal == NULL) {

		if (print)
			msg.WriteInsufficientPeaksForILS ();

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";
		status = -1;
		SetMessageValue (tooFewPeaks, true);
		AppendDataForSmartMessage (tooFewPeaks, "0");
		AppendDataForSmartMessage (tooFewPeaks, Size);
		cout << ErrorString << endl;
		cout << "There are no peaks available in the ILS" << endl;
		return -50;
	}

	if (PreliminaryCurveList.Entries () < Size) {

		if (print)
			msg.WriteInsufficientPeaksForILS ();

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";
		status = -1;
		SetMessageValue (tooFewPeaks, true);
		AppendDataForSmartMessage (tooFewPeaks, PreliminaryCurveList.Entries ());
		AppendDataForSmartMessage (tooFewPeaks, Size);
		cout << ErrorString << endl;
		cout << "There are too few peaks available in the ILS:  " << PreliminaryCurveList.Entries () << " peaks out of " << Size << endl;
		return -50;
	}

	lastPeak = nextSignal->Peak ();
	CurveIterator.Reset ();

	if (peaksChosen > 0)
		avePeak = avePeak / (double)peaksChosen;

	int origNumberOfPeaks = PreliminaryCurveList.Entries ();
	curvesLeft = PreliminaryCurveList.Entries ();

	if ((secondaryPeak > 0) && (peaksChosen > 1) && (maxPeakOnRight > 3.0 * secondaryPeak))
		maxPeakOnRight = secondaryPeak;

	if (maxPeakOnRight > 3.0 * avePeak)
		maxPeakOnRight = 2.0 * avePeak;

	// move PreliminaryCurveList peaks to a temporary list so that this can be undone!

	ClearAndRepopulateFromList (PreliminaryCurveList, tempCurveList, overFlow);

	if (lastPeak <= endTestFactor * maxPeak) {

		curvesLeft = Locus::DivideListByPeakHeight (tempCurveList, overFlow, indivList, avePeak, maxPeak, scaleFactor, Size);
	}

	if (curvesLeft < Size) {

		// add message about dividing by peak height failure - and beginning "junk" - then restore and continue.  This failure may
		// predict overall failure, but it may just signal a problem with the thresholds used for dividing the data.  See earlier comment
		// about using a temporary list

		SetMessageValue (initialFragments, true);
	}

	// Regardless of how we got here, tempCurveList has the curves we want

	nextSignal = (DataSignal*) overFlow.Last ();

	if (nextSignal != NULL)
		endOfJunk = nextSignal->GetMean ();

	else
		endOfJunk = 0.0;

//	MergeListAIntoListB (overFlow, ArtifactList);
	overFlow.Clear ();

	PrecomputeAverages (tempCurveList);
	int TotalCandidates = tempCurveList.Entries ();

	double MaxPeak = 0.0;
	double peak;
	RGDListIterator tempCurveIterator (tempCurveList);
	int skipPeaks = (4 * TotalCandidates) / 10;
	int kk = 0;
	secondaryPeak = 0.0;

	while (nextSignal = (DataSignal*) tempCurveIterator ()) {

		peak = nextSignal->Peak ();

		if (peak > 1.6 * maxPeakOnRight) {

			tempCurveIterator.RemoveCurrentItem ();
			overFlow.Append (nextSignal);
		}

		kk++;

		if (kk < skipPeaks)
			continue;

		if ((nextSignal->GetMessageValue (pullup)) && (!nextSignal->GetMessageValue (primaryPullup)))
			continue;

		//if (nextSignal->GetMessageValue (primaryPullup))
		//	continue;

		if (peak > MaxPeak) {

			secondaryPeak = MaxPeak;
			MaxPeak = peak;
		}
	}

	if (MaxPeak > 4.0 * secondaryPeak)
		MaxPeak = secondaryPeak;

	if ((3 * peaksChosen > Size) && (maxPeakOnRight > 0.0))
		MaxPeak = maxPeakOnRight;	// This is a test!!!!!!!

	if (tempCurveList.Entries () < Size) {

		ClearAndRepopulateFromList (PreliminaryCurveList, tempCurveList, overFlow);
	}

	double fractionalThreshold = 0.0;
	double fractionalFilter = STRLaneStandardChannelData::GetILSFractionalFilter ();
	
	if (fractionalFilter > 0.0) {
	
		fractionalThreshold = fractionalFilter * MaxPeak;
	}

	// save current curves in totallyTempCurveList
	ClearAndRepopulateFromList (tempCurveList, totallyTempCurveList, overFlow);
	int nFilter = 0;

	while (nextSignal = (DataSignal*)tempCurveList.GetFirst ()) {

		// Test for out of bounds on various quantities...
		FollowingSignal = (DataSignal*)tempCurveList.First ();

		if ((fractionalFilter > 0.0) && (nextSignal->Peak () <= fractionalThreshold)) {

			nFilter++;
			indivList.Append (nextSignal);
		}

		else {

			TestResult = TestNeighborsSM (PreviousSignal, nextSignal, FollowingSignal);  // tests for stutter/adenylation

			if (TestResult <= 0)
				overFlow.Append (nextSignal);

			else
				FinalCurveList.Append (nextSignal);
		}

		PreviousSignal = nextSignal;
	}

	if (FinalCurveList.Entries () < Size) {

		if (FinalCurveList.Entries () + nFilter < Size) {
		
			SetMessageValue (neighborFilterTooAggressive, true);
			AppendDataForSmartMessage (neighborFilterTooAggressive, FinalCurveList.Entries ());
			AppendDataForSmartMessage (neighborFilterTooAggressive, Size);

			// Repopulate from saved list:  totallyTempCurveList
			ClearAndRepopulateFromList (totallyTempCurveList, FinalCurveList, overFlow);
			indivList.Clear ();
		}

		else {

			while (nextSignal = (DataSignal*) indivList.GetFirst ()) {

				nextSignal->SetMessageValue (fractionalFilterHeight, true);
				FinalCurveList.InsertWithNoReferenceDuplication (nextSignal);
			}

			SetMessageValue (fractionalFilterTooAggressive, true);
		}
	}

//	MergeListAIntoListB (overFlow, ArtifactList);  // overFlow is empty after this, whichever way we got here
//	MergeListAIntoListB (indivList, ArtifactList);
	overFlow.Clear ();
	indivList.Clear ();
	bool relativeHeightsFailed = false;
	bool testedRelativeHeights = false;

	// Now we use FinalCurveList, which has at least Size elements, whichever path got us here...
	// Save FinalCurveList in tempCurveList in case we need to try to fit twice...

	ClearAndRepopulateFromList (FinalCurveList, tempCurveList, overFlow);  // tempCurveList is our backup...

	RGDListIterator FinalIterator (FinalCurveList);
	RGDListIterator FollowingIterator (FinalCurveList);

	double correlationAcceptanceThreshold = mLaneStandard->GetCorrelationAcceptanceThreshold ();
	double correlationAutoAcceptanceThreshold = mLaneStandard->GetCorrelationAutoAcceptanceThreshold ();
	
	double correlation = 0.0;
	const double* differenceArray;
	mLaneStandard->GetUnnormalizedCharacteristicDifferenceArray (differenceArray);
	const double* actualArray;
	mLaneStandard->GetCharacteristicArray (actualArray);
	const double* leftNorm2s;
	mLaneStandard->GetNormsSquaredLeft (leftNorm2s);
	const double* rightNorm2s;
	mLaneStandard->GetNormsSquaredRight (rightNorm2s);
	const int* hts;
	mLaneStandard->GetRelativeHeights (hts);
	int recursiveStatus;
	int sizeFactor = 2 * Size;

	IdealControlSetInfo ctlInfo (actualArray, differenceArray, leftNorm2s, rightNorm2s, hts, Size, false);
	int startPts;

	if (FinalCurveList.Entries () == Size) {

		correlation = DotProductWithQuadraticFit (FinalCurveList, Size, actualArray, differenceArray, leftNorm2s [Size-2]);
	}

	else {

		if (hts != NULL) {

			FinalIterator.Reset ();
			double testPeak;
			testedRelativeHeights = true;

			// Recalculate MaxPeak based on FinalCurveList
			MaxPeak = PopulationMarkerSet::RecalculateMaxPeakBasedOnRelativeHeightInfo (FinalCurveList, MaxPeak, hts, Size);

			int maxRelativeHeight = mLaneStandard->GetMaxRelativeHeight ();

			if (maxRelativeHeight > 0)
				MaxPeak = 5.0 * MaxPeak / (double)maxRelativeHeight;

			while (FinalCurveList.Entries () > 0) { // allow list to go all the way down!

				nextSignal = (DataSignal*) FinalIterator ();
				testPeak = nextSignal->Peak ();

				if (RecursiveInnerProduct::HeightOutsideLimit (hts, 0, testPeak, MaxPeak)) {

					FinalIterator.RemoveCurrentItem ();
					nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
						"Peak out of place in final list:  relative height imbalance");
					nextSignal->SetMessageValue (unexpectedPeak, true);
					nextSignal->AppendDataForSmartMessage (unexpectedPeak, " (Relative height imbalance)");
	//				ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
				}

				else
					break;
			}

			while (FinalCurveList.Entries () > 0) { // allow list to go all the way down!

				nextSignal = (DataSignal*) FinalCurveList.GetLast ();
				testPeak = nextSignal->Peak ();

				if (RecursiveInnerProduct::HeightOutsideLimit (hts, Size - 1, testPeak, MaxPeak)) {

					nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
						"Peak out of place in final list:  relative height imbalance");
					nextSignal->SetMessageValue (unexpectedPeak, true);
					nextSignal->AppendDataForSmartMessage (unexpectedPeak, " (Relative height imbalance)");
	//				ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
				}

				else {

					FinalCurveList.Append (nextSignal);
					break;
				}
			}

			if (FinalCurveList.Entries () < Size) {

				SetMessageValue (relativeHeightsInconsistent, true);
				relativeHeightsFailed = true;
			}

			else {

				startPts = Size;
				overFlow.Clear ();

				if (FinalCurveList.Entries () <= sizeFactor)
					startPts = Size;  // See above

				else if (PopulationMarkerSet::PareDownSignalListBasedOnHeight (FinalCurveList, overFlow, sizeFactor) < 0) {

					startPts = (Size / 2);

					if (2 * startPts == Size)
						startPts--;

					if (startPts > 6)
						startPts = 6;

					cout << "Could not pare down ILS list sufficiently..." << endl;
				}

				//else
				//	startPts = Size;

				recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (FinalCurveList, overFlow, ctlInfo, correlation, 0.98, startPts, MaxPeak);

				if (recursiveStatus < 0) {

					SetMessageValue (relativeHeightsInconsistent, true);
					correlation = 0.0;
					relativeHeightsFailed = true;
				}

				else {

					correlation = DotProductWithQuadraticFit (FinalCurveList, Size, actualArray, differenceArray, leftNorm2s [Size-2]);

					if (correlation < correlationAcceptanceThreshold)
						relativeHeightsFailed = true;
				}
			}
		}

		bool searchForSubset = (testedRelativeHeights && relativeHeightsFailed) || (!testedRelativeHeights);

		if (testedRelativeHeights && relativeHeightsFailed) {

			ClearAndRepopulateFromList (tempCurveList, FinalCurveList, overFlow);
		}

		if (searchForSubset) {

			// The following setting for startPts has been removed on 07/01/2009 to make the algorithm somewhat more robust.  The relatively
			// new "SearchRecursivelyForBestSubset" below has (for some time) been revised to include an exhaustive search that is not too
			// time consuming.  This should improve the overall effectiveness of the algorithm when we set startPts = Size;
			//startPts = (Size / 2);

			//if (2 * startPts == Size)
			//	startPts--;

			//if (startPts > 6)
			//	startPts = 6;

			if (FinalCurveList.Entries () <= sizeFactor)
				startPts = Size;  // See above

			else {

				if (PopulationMarkerSet::PareDownSignalListBasedOnHeight (FinalCurveList, overFlow, sizeFactor) < 0) {

					startPts = (Size / 2);

					if (2 * startPts == Size)
						startPts--;

					if (startPts > 6)
						startPts = 6;

					cout << "Could not pare down ILS list sufficiently..." << endl;
				}

				else
					startPts = Size;
			}

			ctlInfo.mHeightIndices = NULL;
			recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (FinalCurveList, overFlow, ctlInfo, correlation, 0.98, startPts, MaxPeak);
		}
		
		overflowIterator.Reset ();
			
		while (nextSignal = (DataSignal*) overflowIterator ()) {

			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
				"Peak out of place in final list from 'Search Two Sided':  uncategorized artifact");
			nextSignal->SetMessageValue (unexpectedPeak, true);
		}

//		MergeListAIntoListB (overFlow, ArtifactList);
		overFlow.Clear ();

		if (recursiveStatus < 0) {

			if (print)
				msg.WriteInsufficientPeaksForILS ();

			if (status == 0)
				ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";

			SetMessageValue (tooFewPeaks, true);
			AppendDataForSmartMessage (tooFewPeaks, FinalCurveList.Entries ());
			AppendDataForSmartMessage (tooFewPeaks, Size);

			cout << "After recursive search, there are " << FinalCurveList.Entries () << " curves needed out of " << Size << endl;
			cout << "Original number = " << origNumberOfPeaks << " and after classifying by height, " << TotalCandidates << endl;

			status = -1;
			correlation = 0.0;
		}

		else
			correlation = DotProductWithQuadraticFit (FinalCurveList, Size, actualArray, differenceArray, leftNorm2s [Size-2]);
	}
	
	mLaneStandard->AssignLaneStandardSignals (FinalCurveList);
	FinalIterator.Reset ();

	if (correlation < correlationAcceptanceThreshold) {

		if (status == 0)
			ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...Correlation with ideal set " << correlation << " below " << correlationAcceptanceThreshold << ".\n";

		SetMessageValue (spacingFailed, true);
		AppendDataForSmartMessage (spacingFailed, correlation);

		cout << ErrorString << endl;

		status = -1;
	}

	tempCurveList.Clear ();
	totallyTempCurveList.Clear ();
	PreliminaryCurveList.Clear ();

	nextSignal = (DataSignal*)FinalCurveList.First ();
	mBeginAnalysis = endOfJunk;

	if (nextSignal != NULL)
		mBeginAnalysis = 0.9 * nextSignal->GetMean () + 0.1 * endOfJunk;

	CurveIterator.Reset ();
//	ArtifactIterator.Reset ();
	FinalIterator.Reset ();
	int i = 0;
	NumberOfAcceptedCurves = FinalCurveList.Entries ();
	RGDListIterator CompleteIterator (CompleteCurveList);

	if (NumberOfAcceptedCurves == 0) {

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are no peaks within expected parameters.\n";

		SetMessageValue (tooFewPeaks, true);
		AppendDataForSmartMessage (tooFewPeaks, "0");
		AppendDataForSmartMessage (tooFewPeaks, Size);

		while (nextSignal = (DataSignal*) CompleteIterator ())
			nextSignal->ReportNotices (ExcelText, "\t\t", "\t");

		cout << ErrorString << endl;
		return -1;
	}

	double tempCorrelation;

	if (correlation < correlationAutoAcceptanceThreshold) {

		tempCorrelation = ((int) (1000.0 * correlation)) * 0.001;
		SetMessageValue (ilsSpacingIsMarginal, true);
		AppendDataForSmartMessage (ilsSpacingIsMarginal, tempCorrelation);
		AppendDataForSmartMessage (ilsSpacingIsMarginal, correlationAutoAcceptanceThreshold);
	}

	double* CurveNumbers = new double [NumberOfAcceptedCurves];
	Fits = new double [NumberOfAcceptedCurves];
	Peaks = new double [NumberOfAcceptedCurves];
	Sigmas = new double [NumberOfAcceptedCurves];
	Means = new double [NumberOfAcceptedCurves];
	SecondaryContent = new double [NumberOfAcceptedCurves];
	double TwoMass;
	double OneMass;
	int bp;
	RGString allele;

	while (nextSignal = (DataSignal*)FinalIterator ()) {

		CurveNumbers [i] = i + 1;
		Fits [i] = nextSignal->GetCurveFit ();
		Peaks [i] = nextSignal->Peak ();

		Sigmas [i] = nextSignal->GetStandardDeviation ();
		Means [i] = nextSignal->GetMean ();
		TwoMass = nextSignal->GetScale (2);
		OneMass = nextSignal->GetScale (1);
		SecondaryContent [i] = TwoMass / (TwoMass + OneMass);

		if (actualArray != NULL) {

			bp = (int) floor (actualArray [i] + 0.5);
			allele.Convert (bp, 10);
			nextSignal->SetAlleleName (allele);
			nextSignal->SetBioID (actualArray [i]);
			nextSignal->SetMessageValue (unexpectedPeak, false);
			nextSignal->SetMessageValue (isControlPeak, true);

//			if (nextSignal->GetMessageValue (crater))
//				nextSignal->SetMessageValue (notACrater, true);

			if (nextSignal->GetMessageValue (craterSidePeak))
				nextSignal->SetMessageValue (notACraterSidePeak, true);
		}

		if (print)
			nextSignal->Report (text, "    ");

		i++;
	}

	double lowMean = Means [0];
	double highMean = Means [NumberOfAcceptedCurves - 1];
	double currentMean;

	while (nextSignal = (DataSignal*) SmartPeaks.GetLast ()) {

		if (nextSignal->GetMean () <= highMean) {

			SmartPeaks.Append (nextSignal);
			break;
		}
	}

	while (nextSignal = (DataSignal*) SmartPeaks.GetFirst ()) {

		if (nextSignal->GetMean () >= lowMean) {

			SmartPeaks.Prepend (nextSignal);
			break;
		}
	}

	QuadraticFit fit (Means, NumberOfAcceptedCurves);
	fit.Regress (Sigmas, QFit);

	QuadraticFit fit2 (Means, NumberOfAcceptedCurves);
	fit2.Regress (SecondaryContent, QFit2);
	mInverseSouthern = new CSplineTransform (actualArray, Means, NumberOfAcceptedCurves);
	mGlobalSouthern = new CSplineTransform (Means, actualArray, NumberOfAcceptedCurves);


	RGDListIterator SmartIt (SmartPeaks);
	DataSignal* prevSignal = NULL;
	double thisbp;
	double thisPeak;
	double bpDiff;
	double prevbp;
	double prevPeak;

	double stutterLimit = STRLaneStandardChannelData::GetILSStutterThreshold ();
	double adenylationLimit = STRLaneStandardChannelData::GetILSAdenylationThreshold ();
	smAdenylation adenylationFound;
	//smStutter stutterFound;
	
	while (nextSignal = (DataSignal*) SmartIt ()) {

		currentMean = nextSignal->GetMean ();
		thisbp = mGlobalSouthern->EvaluateWithExtrapolation (currentMean);
		thisPeak = nextSignal->Peak ();

		if (!nextSignal->GetMessageValue (isControlPeak)) {

			if (thisPeak <= fractionalThreshold) {

				nextSignal->SetMessageValue (fractionalFilterHeight, true);
			}

			else if ((maxAllowable > 0.0) && (thisPeak >= maxAllowable)) {

				nextSignal->SetMessageValue (peakAboveMax, true);
			}

			if (prevSignal != NULL) {

				bpDiff = (int) floor (fabs (thisbp - prevbp) + 0.5);

				if (bpDiff == 1) {

					// possible adenylation but must be -A

					//if ((thisPeak <= adenylationLimit * prevPeak) && (!prevSignal->GetMessageValue (pullup))) {

					//	nextSignal->SetMessageValue (adenylationFound, true);
					//}

					if ((prevPeak <= adenylationLimit * thisPeak) && (!nextSignal->GetMessageValue (pullup))) {

						prevSignal->SetMessageValue (adenylationFound, true);
					}
				}

				//	12/16/2013...ILS does not have stutter

				//else if (bpDiff == 4) {

				//	// possible stutter

				//	if ((thisPeak <= stutterLimit * prevPeak) && (!prevSignal->GetMessageValue (pullup))) {

				//		nextSignal->SetMessageValue (stutterFound, true);
				//	}

				//	else if ((prevPeak <= stutterLimit * thisPeak) && (!nextSignal->GetMessageValue (pullup))) {

				//		prevSignal->SetMessageValue (stutterFound, true);
				//	}
				//}
			}
		}

		prevSignal = nextSignal;
		prevbp = thisbp;
		prevPeak = thisPeak;
	}

	indivList.Clear ();
	overFlow.Clear ();

	if (!print) {

		delete[] CurveNumbers;
		return 0;
	}

	msg.WriteEmptyLine ();
	msg.WriteMaximumCorrelationWithILS14 (0, correlation, TotalCandidates);
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

	delete[] CurveNumbers;
	msg.FlushLines (6);

	Endl endLine;
	CompleteIterator.Reset ();

	while (nextSignal = (DataSignal*) CompleteIterator ()) {

		nextSignal->ReportNotices (ExcelText, "\t\t", "\t");
	}

	return status;
}


int STRLaneStandardChannelData :: AnalyzeLaneStandardChannelRecursivelyUsingDensitySM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is ladder and sample stage 1
	//

	int status = 0;

	RGDListIterator CurveIterator (PreliminaryCurveList);
	RGDListIterator SmartIt (SmartPeaks);
	FinalCurveList.Clear ();
//	DataSignal* FollowingSignal;
	DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
	smBelowMinRFU belowMinRFU;

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		if (nextSignal->GetMessageValue (belowMinRFU))
			CurveIterator.RemoveCurrentItem ();
	}

	CurveIterator.Reset ();
//	int TestResult;
	int Size = mLaneStandard->GetNumberOfCharacteristics ();
	RGString info;
	RGDList overFlow;
//	double scaleFactor = 0.34;
	double maxPeak;
//	double endTestFactor = 0.335;
//	double junkEndTestProportion = 0.40;
//	int curvesLeft;
//	double lowLevelScaleFactor = 0.25;
//	double additionalJunkInterval = 0.0;
	double lastPeak;
	RGDListIterator overflowIterator (overFlow);
	RGDList indivList;
	RGString data;
	RGDList tempCurveList;
	double mPrimerTime = 0.0;
	RGDListIterator FinalIterator (FinalCurveList);
	int origNumberOfPeaks = PreliminaryCurveList.Entries ();
	int TotalCandidates = PreliminaryCurveList.Entries ();

	smPullUp pullup;
	smPrimaryInterchannelLink primaryPullup;
	smILSTooFewPeaks tooFewPeaks;
	smCannotSeparateILSPrimerPeaks initialFragments;
	smILSNeighborFilterTooAggressive neighborFilterTooAggressive;
	smHeightBelowFractionalFilter fractionalFilterHeight;
	smILSFractionFilterTooAggressive fractionalFilterTooAggressive;
	smUnexpectedILSPeak unexpectedPeak;
	smILSRelativeHeightsInconsistent relativeHeightsInconsistent;
	smILSSpacingFailed spacingFailed;
	smSignalIsCtrlPeak isControlPeak;
	smPeakHeightAboveMax peakAboveMax;
	smAdenylation adenylationFound;
	//smStutter stutterFound;
	smILSSpacingIsMarginal ilsSpacingIsMarginal;
	smSignalNotACrater notACrater;
	smSignalNotACraterSidePeak notACraterSidePeak;
	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smILSPeakMayBeIncorrect iLSPeakMayBeIncorrect;

	if (Size <= 0) {

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are no expected peaks for the ILS.\n";
		status = -1;
		SetMessageValue (tooFewPeaks, true);
		AppendDataForSmartMessage (tooFewPeaks, PreliminaryCurveList.Entries ());
		AppendDataForSmartMessage (tooFewPeaks, Size);

		cout << ErrorString << endl;
		cout << "There are " << PreliminaryCurveList.Entries () << " curves needed out of " << Size << endl;
		return -50;
	}

	if (PreliminaryCurveList.Entries () < Size) {

		if (print)
			msg.WriteInsufficientPeaksForILS ();

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";
		status = -1;
		SetMessageValue (tooFewPeaks, true);
		AppendDataForSmartMessage (tooFewPeaks, PreliminaryCurveList.Entries ());
		AppendDataForSmartMessage (tooFewPeaks, Size);
		cout << ErrorString << endl;
		cout << "There are too few peaks available in the ILS:  " << PreliminaryCurveList.Entries () << " peaks out of " << Size << endl;
		return -50;
	}

	RGDList totallyTempCurveList;

	double primerTime;
	FoundPrimerPeaks (PreliminaryCurveList, mLaneStandard->GetFirstIntervalFraction (), mLaneStandard->GetSmallestIntervalFraction (), primerTime, Size, maxPeak, ExcelText);

	// Save peaks to tempCurveList and operate on that.  This algorithm must not change PreliminaryCurveList unless it finds a successful fit!

	ClearAndRepopulateFromList (PreliminaryCurveList, tempCurveList, overFlow);
	RGDListIterator itt (tempCurveList);
	SmartPeaks.Clear ();

	// Now remove primer peaks and isolated "large" peaks; FoundPrimerPeaks tests for removing too many peaks, so we don't have to do it here.

	while (nextSignal = (DataSignal*) itt ()) {

		if (nextSignal->GetMean () < primerTime)
			itt.RemoveCurrentItem ();

		else {

			nextSignal->SetMessageValue (unexpectedPeak, true);
			SmartPeaks.Append (nextSignal);

			if (nextSignal->Peak () > maxPeak) {

				itt.RemoveCurrentItem ();
				overFlow.Append (nextSignal);
			}
		}
	}

//	MergeListAIntoListB (overFlow, ArtifactList);
	overFlow.Clear ();

	// Now we test to see if we have few enough peaks to just go ahead and fit the spacing.  We're not necessarily confident we have the "true" maxPeak, so
	// we're a little leary of using it for fractional filters, etc.  If we can get away without it, we will.  On the other hand, if we have too many peaks, we'll
	// try to reduce the number using the known number of large characteristics.  After we find the true set of ILS peaks, we'll use the maxPeak of those selected
	// to specify peaks that are below the fractional filter or are adenylation or stutter...First put all peaks in FinalCurveList, so we can go back to tempCurveList
	// if we need to.

	double correlationAcceptanceThreshold = mLaneStandard->GetCorrelationAcceptanceThreshold ();
	double correlationAutoAcceptanceThreshold = mLaneStandard->GetCorrelationAutoAcceptanceThreshold ();

	double correlation = 0.0;
	const double* differenceArray;
	mLaneStandard->GetUnnormalizedCharacteristicDifferenceArray (differenceArray);
	const double* actualArray;
	mLaneStandard->GetCharacteristicArray (actualArray);
	const double* leftNorm2s;
	mLaneStandard->GetNormsSquaredLeft (leftNorm2s);
	const double* rightNorm2s;
	mLaneStandard->GetNormsSquaredRight (rightNorm2s);
	const int* hts;
	mLaneStandard->GetRelativeHeights (hts);
	int maxRelativeHeight = mLaneStandard->GetMaxRelativeHeight ();
	double heightFactor = 1.0;

	if (maxRelativeHeight < 5)
		heightFactor = 5.0 / (double)maxRelativeHeight;

	int recursiveStatus;
	int downsizeStatus = 0;
	int sizeFactor = 2 * Size;
	ClearAndRepopulateFromList (tempCurveList, FinalCurveList, overFlow);  // populates FinalCurveList
//	double newMaxPeak;
	double minPeak;
	int sizeFactor2 = sizeFactor + 2;
	RGDListIterator finalIterator (FinalCurveList);

	IdealControlSetInfo ctlInfo (actualArray, differenceArray, leftNorm2s, rightNorm2s, hts, Size, false);
	int startPts;
	bool testedRelativeHeights = false;
	bool relativeHeightsFailed = false;
	int reductionMax = (2 * Size) / 5;
	int reduction = 6;

	if (reduction > reductionMax)
		reduction = reductionMax;

	if (hts != NULL) {

		testedRelativeHeights = true;

		// Recalculate maxPeak based on FinalCurveList; calculate minPeak if needed to pare down number of curves; nothing is changed if number of peaks would be too small
		maxPeak = PopulationMarkerSet::RecalculateMaxPeakBasedOnRelativeHeightInfo (FinalCurveList, maxPeak, hts, Size, maxRelativeHeight, sizeFactor, minPeak) + 0.1;

		finalIterator.Reset ();

		if (FinalCurveList.Entries () > sizeFactor) {

			while (nextSignal = (DataSignal*) finalIterator ()) {

				lastPeak = nextSignal->Peak ();

				if (lastPeak > maxPeak) {

					finalIterator.RemoveCurrentItem ();
					overFlow.Append (nextSignal);
				}

				else if (lastPeak <= minPeak) {

					finalIterator.RemoveCurrentItem ();
					overFlow.Append (nextSignal);
				}
			}

			Endl endline;
			ExcelText.SetOutputLevel (1);
			ExcelText << "Reduced number of peaks to " << FinalCurveList.Entries () << " with min height = " << minPeak << " and max height = " << maxPeak << endline;
			ExcelText.ResetOutputLevel ();
		}
	}

	else if (FinalCurveList.Entries () > sizeFactor) {

		downsizeStatus = PopulationMarkerSet::PareDownSignalListBasedOnHeight (FinalCurveList, overFlow, sizeFactor - reduction);
	}

//	MergeListAIntoListB (overFlow, ArtifactList);  //????????????????????????????????????????????????????
	overFlow.Clear ();

	if (FinalCurveList.Entries () == Size) {

		correlation = DotProductWithQuadraticFit (FinalCurveList, Size, actualArray, differenceArray, leftNorm2s [Size-2]);
	}

	else {

		overFlow.Clear ();

		if (FinalCurveList.Entries () <= sizeFactor2)
			startPts = Size;  // See above

		else {

			startPts = (Size / 2);

			if (2 * startPts == Size)
				startPts--;

			if (startPts > 6)
				startPts = 6;

			cout << "Could not pare down ILS list sufficiently..." << endl;
		}

		ClearAndRepopulateFromList (FinalCurveList, totallyTempCurveList, overFlow);
		recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (FinalCurveList, overFlow, ctlInfo, correlation, 0.98, startPts, heightFactor * maxPeak);

		if (recursiveStatus < 0) {

			//newNotice = new ILSRelativeHeightsInconsistent;
			//AddNoticeToList (newNotice);
			correlation = 0.0;
			relativeHeightsFailed = true;
		}

		else {

			correlation = DotProductWithQuadraticFit (FinalCurveList, Size, actualArray, differenceArray, leftNorm2s [Size-2]);

			if (correlation < correlationAcceptanceThreshold)
				relativeHeightsFailed = true;
		}

		bool searchForSubset = (testedRelativeHeights && relativeHeightsFailed) || (!testedRelativeHeights);

		if (testedRelativeHeights && relativeHeightsFailed) {

			//ClearAndRepopulateFromList (totallyTempCurveList, FinalCurveList, overFlow);	//??????????????????????????????????????????????????????????????????????????????????
			ClearAndRepopulateFromList (tempCurveList, FinalCurveList, overFlow);	//??????????????????????????????????????????????????????????????????????????????????
			SetMessageValue (relativeHeightsInconsistent, true);
		}

		if (searchForSubset) {

			if (FinalCurveList.Entries () <= sizeFactor)
				startPts = Size;  // See above

			else {

				if (PopulationMarkerSet::PareDownSignalListBasedOnHeight (FinalCurveList, overFlow, sizeFactor - reduction) < 0) {

					startPts = (Size / 2);

					if (2 * startPts == Size)
						startPts--;

					if (startPts > 6)
						startPts = 6;

					cout << "Could not pare down ILS list sufficiently for Method 1..." << endl;
				}

				else
					startPts = Size;

				Endl endline;
				ExcelText.SetOutputLevel (1);
				ExcelText << "Finally reduced number of peaks to " << FinalCurveList.Entries () << endline;
				ExcelText.ResetOutputLevel ();
			}

			ctlInfo.mHeightIndices = NULL;
			recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (FinalCurveList, overFlow, ctlInfo, correlation, 0.98, startPts, heightFactor * maxPeak);
		}

		if (recursiveStatus < 0) {

			status = -1;
			correlation = 0.0;
			overFlow.Clear ();
		}

		else
			correlation = DotProductWithQuadraticFit (FinalCurveList, Size, actualArray, differenceArray, leftNorm2s [Size-2]);

	}

	if (correlation < correlationAcceptanceThreshold) {

		status = -1;
	}

	if (status < 0) {

		overFlow.Clear ();
		indivList.Clear ();
		tempCurveList.Clear ();
		totallyTempCurveList.Clear ();
		return status;
	}

	NumberOfAcceptedCurves = FinalCurveList.Entries ();

	if (NumberOfAcceptedCurves != Size) {

		overFlow.Clear ();
		indivList.Clear ();
		tempCurveList.Clear ();
		totallyTempCurveList.Clear ();
		return -1;
	}

	double tempCorrelation;

	if (correlation < correlationAutoAcceptanceThreshold) {

		tempCorrelation = ((int) (1000.0 * correlation)) * 0.001;
		SetMessageValue (ilsSpacingIsMarginal, true);
		AppendDataForSmartMessage (ilsSpacingIsMarginal, tempCorrelation);
		AppendDataForSmartMessage (ilsSpacingIsMarginal, correlationAutoAcceptanceThreshold);
	}

	//cout << "ILS Correlation = " << correlation << endl;

	mLaneStandard->AssignLaneStandardSignals (FinalCurveList);
	FinalIterator.Reset ();

	nextSignal = (DataSignal*)FinalCurveList.First ();
	mBeginAnalysis = primerTime;
	
	if (nextSignal != NULL)
		mBeginAnalysis = 0.9 * nextSignal->GetMean () + 0.1 * primerTime;

	CurveIterator.Reset ();
//	ArtifactIterator.Reset ();
	FinalIterator.Reset ();
	int i = 0;
	
	RGDListIterator CompleteIterator (CompleteCurveList);

	double* CurveNumbers = new double [NumberOfAcceptedCurves];
	Fits = new double [NumberOfAcceptedCurves];
	Peaks = new double [NumberOfAcceptedCurves];
	Sigmas = new double [NumberOfAcceptedCurves];
	Means = new double [NumberOfAcceptedCurves];
	SecondaryContent = new double [NumberOfAcceptedCurves];
	double TwoMass;
	double OneMass;
	int bp;
	RGString allele;
	double thisPeak;
	double finalMaxPeak = 0.0;

	while (nextSignal = (DataSignal*)FinalIterator ()) {

		CurveNumbers [i] = i + 1;
		Fits [i] = nextSignal->GetCurveFit ();
		thisPeak = Peaks [i] = nextSignal->Peak ();

		if (thisPeak > finalMaxPeak)
			finalMaxPeak = thisPeak;

		Sigmas [i] = nextSignal->GetStandardDeviation ();
		Means [i] = nextSignal->GetMean ();
		TwoMass = nextSignal->GetScale (2);
		OneMass = nextSignal->GetScale (1);
		SecondaryContent [i] = TwoMass / (TwoMass + OneMass);

		if (actualArray != NULL) {

			bp = (int) floor (actualArray [i] + 0.5);
			allele.Convert (bp, 10);
			nextSignal->SetAlleleName (allele);
			nextSignal->SetBioID (actualArray [i]);
			nextSignal->SetMessageValue (unexpectedPeak, false);
			nextSignal->SetMessageValue (isControlPeak, true);

//			if (nextSignal->GetMessageValue (crater))
//				nextSignal->SetMessageValue (notACrater, true);

			if (nextSignal->GetMessageValue (craterSidePeak))
				nextSignal->SetMessageValue (notACraterSidePeak, true);
		}

		if (print)
			nextSignal->Report (text, "    ");

		i++;
	}

	double lowMean = Means [0];
	double highMean = Means [NumberOfAcceptedCurves - 1];
	double currentMean;

	// The following is vacuous since indivList should be empty - it's never been populated with anything!!
	
	//while (nextSignal = (DataSignal*) indivList.GetFirst ()) {

	//	currentMean = nextSignal->GetMean ();

	//	if ((currentMean < lowMean) || (currentMean > highMean))
	//		nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
	//		"Peak represents measurement at base pair level outside internal lane standard");

	//	else
	//		nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
	//		"Signal out of place in ILS - uncategorized artifact");

	//	ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
	//}

	while (nextSignal = (DataSignal*) SmartPeaks.GetLast ()) {

		if (nextSignal->GetMean () <= highMean) {

			SmartPeaks.Append (nextSignal);
			break;
		}
	}

	while (nextSignal = (DataSignal*) SmartPeaks.GetFirst ()) {

		if (nextSignal->GetMean () >= lowMean) {

			SmartPeaks.Prepend (nextSignal);
			break;
		}
	}

	QuadraticFit fit (Means, NumberOfAcceptedCurves);
	fit.Regress (Sigmas, QFit);

	QuadraticFit fit2 (Means, NumberOfAcceptedCurves);
	fit2.Regress (SecondaryContent, QFit2);
	mInverseSouthern = new CSplineTransform (actualArray, Means, NumberOfAcceptedCurves);
	mGlobalSouthern = new CSplineTransform (Means, actualArray, NumberOfAcceptedCurves);

	CurveIterator.Reset ();
	DataSignal* prevSignal = NULL;
	double stutterLimit = STRLaneStandardChannelData::GetILSStutterThreshold ();
	double adenylationLimit = STRLaneStandardChannelData::GetILSAdenylationThreshold ();
	double prevPeak = 0.0;
	double maxAllowable = STRLaneStandardChannelData::GetMaxRFU ();
	double thisbp;
	double prevbp = 0.0;
	int bpDiff;

	double fractionalThreshold = 0.0;
	double fractionalFilter = STRLaneStandardChannelData::GetILSFractionalFilter ();
	
	if (fractionalFilter > 0.0) {
	
		fractionalThreshold = fractionalFilter * finalMaxPeak;
	}

	while (nextSignal = (DataSignal*)CurveIterator ()) {

		currentMean = nextSignal->GetMean ();
		thisbp = mGlobalSouthern->EvaluateWithExtrapolation (currentMean);
		thisPeak = nextSignal->Peak ();

		if ((currentMean < lowMean) || (currentMean > highMean)) {

			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
			"Peak represents measurement at base pair level outside internal lane standard");
		}

		else {

			if (thisPeak <= fractionalThreshold) {

				if (!nextSignal->GetMessageValue (isControlPeak))
					nextSignal->SetMessageValue (fractionalFilterHeight, true);
			}

			else if ((maxAllowable > 0.0) && (thisPeak >= maxAllowable)) {

				nextSignal->SetMessageValue (peakAboveMax, true);
			}

			if (prevSignal != NULL) {

				bpDiff = (int) floor (fabs (thisbp - prevbp) + 0.5);

				if (bpDiff == 1) {

					// possible adenylation

					//	12/16/2013...no such thing as +A adenylation

					//if ((thisPeak <= adenylationLimit * prevPeak) && (!prevSignal->GetMessageValue (pullup))) {

					//	if (nextSignal->GetMessageValue (isControlPeak))
					//		nextSignal->SetMessageValue (iLSPeakMayBeIncorrect, true);

					//	else
					//		nextSignal->SetMessageValue (adenylationFound, true);
					//}

					if ((prevPeak <= adenylationLimit * thisPeak) && (!nextSignal->GetMessageValue (pullup))) {

						if (prevSignal->GetMessageValue (isControlPeak))
							prevSignal->SetMessageValue (iLSPeakMayBeIncorrect, true);

						else
							prevSignal->SetMessageValue (adenylationFound, true);
					}
				}

				//	12/16/2013...ILS does not have stutter

				//else if (bpDiff == 4) {

				//	// possible stutter

				//	if ((thisPeak <= stutterLimit * prevPeak) && (!prevSignal->GetMessageValue (pullup))) {

				//		if (nextSignal->GetMessageValue (isControlPeak))
				//			nextSignal->SetMessageValue (iLSPeakMayBeIncorrect, true);

				//		else
				//			nextSignal->SetMessageValue (stutterFound, true);
				//	}

				//	else if ((prevPeak <= stutterLimit * thisPeak) && (!nextSignal->GetMessageValue (pullup))) {

				//		if (prevSignal->GetMessageValue (isControlPeak))
				//			prevSignal->SetMessageValue (iLSPeakMayBeIncorrect, true);

				//		else
				//			prevSignal->SetMessageValue (stutterFound, true);
				//	}
				//}
			}
		}

		prevSignal = nextSignal;
		prevbp = thisbp;
		prevPeak = thisPeak;
	}

	indivList.Clear ();

	// Take care of craters and recheck ILS peaks for issues from above tests...

	// Label the rest of the peaks as "out of place in ILS

	overflowIterator.Reset ();
			
	while (nextSignal = (DataSignal*) overflowIterator ()) {

		nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
			"Peak out of place in final list from 'Search Two Sided':  uncategorized artifact");
		nextSignal->SetMessageValue (unexpectedPeak, true);
	}

//	MergeListAIntoListB (overFlow, ArtifactList);

	if (print) {

		msg.WriteEmptyLine ();
		msg.WriteMaximumCorrelationWithILS14 (0, correlation, TotalCandidates);
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
	Endl endLine;
	CompleteIterator.Reset ();

	if (status == 0) {

		while (nextSignal = (DataSignal*) CompleteIterator ()) {

			nextSignal->ReportNotices (ExcelText, "\t\t", "\t");
		}

		PreliminaryCurveList.Clear ();
	}

	overFlow.Clear ();
	indivList.Clear ();
	tempCurveList.Clear ();
	totallyTempCurveList.Clear ();	

	return status;
}


int STRLaneStandardChannelData :: HierarchicalLaneStandardChannelAnalysisSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is ladder and sample stage 1
	//

	Endl endline;
	int status = AnalyzeLaneStandardChannelRecursivelyUsingDensitySM (text, ExcelText, msg, print);

	if (status == 0) {

//		cout << "Lane standard analyzed using density method:  Method 1" << endl;
		ExcelText.SetOutputLevel (1);
		ExcelText << "Lane standard analyzed using density method:  Method 1" << endline;
		ExcelText.ResetOutputLevel ();
	}

	else if (status == -50)
		return status;

	else {

		status = AnalyzeLaneStandardChannelRecursivelySM (text, ExcelText, msg, print);

		if (status == 0) {

			cout << "Lane standard analyzed using recursive method:  Method 2" << endl;
			ExcelText.SetOutputLevel (1);
			ExcelText << "Lane standard analyzed using recursive method:  Method 2" << endline;
			ExcelText.ResetOutputLevel ();
		}
	}

	if (status == 0)
		CorrectLaneStandardCrossChannelAnalysisSM ();

	return status;
}


int STRLaneStandardChannelData :: CorrectLaneStandardCrossChannelAnalysisSM () {

	//
	// This function used to be something else, but it has been commandeered to correct
	// for omitted ils peaks that have been designated and should not be the cause of alerts
	//

	//
	//  This is ladder and sample stage 1
	//

	smUnexpectedILSPeak ilsUnexpectedPeak;
	DataSignal* nextSignal;

	const double* omissionArray;
	int omissionSize = mLaneStandard->GetOmissionArray (omissionArray);
	int kk = 0;
	double testBP;
	double lowerTest;
	double upperTest;

	if (omissionSize > 0) {

		RGDListIterator artIt (SmartPeaks);
		double time1;
		nextSignal = (DataSignal*) FinalCurveList.First ();
		time1 = nextSignal->GetMean ();
		double localBp;
		testBP = omissionArray [kk];
		lowerTest = testBP - 10.0;
		upperTest = testBP + 10.0;

		while (nextSignal = (DataSignal*) artIt ()) {

			if (nextSignal->GetMean () <= time1)
				continue;

			localBp = nextSignal->GetApproximateBioID ();

			if (localBp > lowerTest) {

				if (localBp < upperTest)
					nextSignal->SetMessageValue (ilsUnexpectedPeak, false);

				else {

					kk++;

					if (kk >= omissionSize)
						break;

					testBP = omissionArray [kk];
					lowerTest = testBP - 10.0;
					upperTest = testBP + 10.0;
				}
			}
		}
	}

	return 0;
}


int STRLaneStandardChannelData :: FinalTestForCriticalLaneStandardNoticesSM () {

	//
	//  This is ladder and sample stage 5
	//

	bool criticalArtifactFound = false;
	int criticalLevel = Notice::GetMessageTrigger ();
	int nextSignalLevel;
	RGDListIterator it (FinalCurveList);
	DataSignal* nextSignal;
//	RGString info;

	smCriticalMessagesAtILS criticalNotices;

	while (nextSignal = (DataSignal*) it ()) {

		nextSignalLevel = nextSignal->GetHighestMessageLevelWithRestrictionSM ();

		if ((nextSignalLevel > 0) && (nextSignalLevel <= criticalLevel)) {

			criticalArtifactFound = true;
			SetMessageValue (criticalNotices, true);
			AppendDataForSmartMessage (criticalNotices, nextSignal->GetAlleleName ());
		}
	}

	if (criticalArtifactFound)
		return -1;

	return 0;
}


int STRLaneStandardChannelData :: TestForRaisedBaselineAndExcessiveNoiseSM (double left, double report) {

	smChannelHasRaisedBaseline channelHasRaisedBaseline;
	smChannelHasExcessiveNoiseAboveAnalysisThreshold excessiveNoiseAboveAnalysis;
	smChannelHasExcessiveNoiseAboveDetectionThreshold excessiveNoiseBelowAnalysis;
	smSampleILSChannelRaisedBaselineThreshold raisedBaseline;

	double analysisThreshold = GetMinimumHeight ();
	double raisedBaselineThreshold = (double) GetThreshold (raisedBaseline);

	RaisedBaseLineData* rbld;
	PoorFitPeakData* pfpd;

	double minTest = left;
	double height;

	if (report > left)
		minTest = report;

	if (raisedBaselineThreshold > 0.0) {

		while (rbld = mRaisedBaseLines.RemoveFirst ()) {

			if (rbld->GetLeft () < minTest)
				continue;

			height = rbld->GetHeight ();

			if (height >= raisedBaselineThreshold) {

				SetMessageValue (channelHasRaisedBaseline, true);
				AppendDataForSmartMessage (channelHasRaisedBaseline, (int) floor ((double)height));
				AppendDataForSmartMessage (channelHasRaisedBaseline, (int) floor ((double)rbld->GetLeft ()));
				break;
			}
		}
	}

	mRaisedBaseLines.clearAndDestroy ();

	while (pfpd = mPoorFits.RemoveFirst ()) {

		if (pfpd->GetMode () < minTest)
			continue;

		height = pfpd->GetHeight ();

		if (height >= analysisThreshold) {

			SetMessageValue (excessiveNoiseAboveAnalysis, true);
			SetMessageValue (excessiveNoiseBelowAnalysis, true);
			break;
		}
	}

	mPoorFits.clearAndDestroy ();
	return 0;
}


// Smart Message related methods*****************************************************************************************************


int STRLadderChannelData :: SetDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder stage 1
	//

	int status = STRChannelData::SetDataSM (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLadderChannelData :: SetRawDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder stage 1
	//

	int status = STRChannelData::SetRawDataSM (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLadderChannelData :: SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder stage 1
	//

	int status = STRChannelData::SetRawDataFromColorCorrectedArraySM (dataArray, arraySize, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLadderChannelData :: AnalyzeGridLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is ladder stage 2
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	int status = 0;
	DataSignal* nextSignal;
	Endl endLine;
	ExcelText << CLevel (1) << endLine << "Ladder data for Channel Number " << mChannel << endLine << PLevel ();
	smInterlocusLadderPeak interlocusPeak;
	smHeightBelowFractionalFilter belowFractionalFilter;
	smHeightBelowInterlocusMinRFU belowInterlocusMinRFU;
	double maxPeak = 0.0;
	double maxLocusPeak;
	double currentPeak;

	//  All the characteristics we are looking for are in the PreliminaryCurveList

	while (nextLocus = (Locus*) it()) {

		//if (nextLocus->AnalyzeGridLocusAndAllowForOverlapSM (ArtifactList, PreliminaryCurveList, SupplementalArtifacts, text, ExcelText, msg, print) < 0) {

		//	status = -1;
		//}

		if (nextLocus->AnalyzeGridLocusAndAllowForOverlapUsingBPsSM (ArtifactList, PreliminaryCurveList, SupplementalArtifacts, text, ExcelText, msg, print) < 0) {

			status = -1;
		}

		else {

			maxLocusPeak = nextLocus->GetMaximumPeak ();

			if (maxLocusPeak > maxPeak)
				maxPeak = maxLocusPeak;
		}
	}

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->TestGridInterlocusThresholds (PreliminaryCurveList);

	RGDListIterator PreliminaryIterator (PreliminaryCurveList);
	double ladderInterlocusMinRFU = STRLadderChannelData::GetMinInterlocusRFU ();
	double ladderFractionalFilter = Locus::GetGridFractionalFilter ();
	double interlocusFractionalThreshold = maxPeak * ladderFractionalFilter;
	bool removeSignal;

	while (nextSignal = (DataSignal*) PreliminaryIterator ()) {

		currentPeak = nextSignal->Peak ();
		removeSignal = false;

		if (currentPeak <= ladderInterlocusMinRFU) {

			removeSignal = true;
			nextSignal->SetMessageValue (belowInterlocusMinRFU, true);
		}

		if (currentPeak <= interlocusFractionalThreshold) {

			removeSignal = true;
			nextSignal->SetMessageValue (belowFractionalFilter, true);
		}

		if (removeSignal)
			PreliminaryIterator.RemoveCurrentItem ();

		nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", "Peak out of place in final list:  interlocus peak");
		nextSignal->SetMessageValue (interlocusPeak, true);
	}

	RGDListIterator CompleteIterator (SmartPeaks);

	while (nextSignal = (DataSignal*) CompleteIterator ()) {

		nextSignal->ReportNotices (ExcelText, "\t\t", "\t");
//		ExcelText << "\t\tPeak = \t" << nextSignal->Peak () << endLine;
	}

	return status;
}


//***********************************************************************************************************************************


// Smart Message related functions*******************************************************************************************************************
//***************************************************************************************************************************************************


int STRSampleChannelData :: SetDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is sample stage 1
	//

	int status = STRChannelData::SetDataSM (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testSamplePeak;
	return 0;
}


int STRSampleChannelData :: SetRawDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	int status = STRChannelData::SetRawDataSM (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRSampleChannelData :: SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	int status = STRChannelData::SetRawDataFromColorCorrectedArraySM (dataArray, arraySize, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRSampleChannelData :: AssignSampleCharacteristicsToLociSM (CoreBioComponent* grid, CoordinateTransform* timeMap) {

	//  Assume sample characteristics are in PreminaryCurveList.
	//
	//  Get each locus and find corresponding locus in grid.  Then, extract signals for that locus.
	//

	//
	//  This is sample stage 1
	//

	Locus* nextLocus;
	Locus* gridLocus;
	RGDListIterator it (mLocusList);
	smBelowMinRFU belowMinRFU;

	//while (nextLocus = (Locus*) it()) {

	//	gridLocus = grid->FindLocus (mChannel, nextLocus->GetLocusName ());

	//	if (gridLocus == NULL)
	//		return -1;  // this should never happen...it means that the channel has a locus that the grid has never heard of, but have to test...

	//	nextLocus->ExtractSampleSignals (PreliminaryCurveList, gridLocus, timeMap);
	//}

	//it.Reset ();

	while (nextLocus = (Locus*) it ()) {

		gridLocus = grid->FindLocus (mChannel, nextLocus->GetLocusName ());

		if (gridLocus == NULL)
			return -1;  // this should never happen...it means that the channel has a locus that the grid has never heard of, but have to test...

		nextLocus->ExtractExtendedSampleSignalsSM (PreliminaryCurveList, gridLocus, timeMap);
	}

	// Done extracting all signals from list that lie within a locus.  What remains is inside the internal lane standard but outside all loci.  Now remove
	// any signals remaining in PreliminaryCurveList that lie in any extended locus

	DataSignal* nextSignal;
	RGDListIterator pIt (PreliminaryCurveList);
	bool isCore;

	while (nextSignal = (DataSignal*) pIt ()) {

		isCore = (nextSignal->GetLocus (0) != NULL);

		if (nextSignal->IsPossibleInterlocusAllele (1) || nextSignal->IsPossibleInterlocusAllele (-1) || isCore)
			pIt.RemoveCurrentItem ();

		else if (nextSignal->GetMessageValue (belowMinRFU))
			pIt.RemoveCurrentItem ();
	}

	return 0;
}


int STRSampleChannelData :: AnalyzeSampleLociSM (ChannelData* lsData, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is sample stage 2
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;

	while (nextLocus = (Locus*) it ())
		nextLocus->AnalyzeSampleLocusSM (lsData, ArtifactList, PreliminaryCurveList, text, ExcelText, msg, print);

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->PromoteNonAmbiguousSignalsToAlleles (PreliminaryCurveList);

	return 0;
}


int STRSampleChannelData :: TestFractionalFiltersSM () {

	//
	//  This is sample stage 2
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	RGDList fractionalFilterList;
	double maxPeak = 0.0;
	double peak;
	DataSignal* nextSignal;
	RGDListIterator cIt (PreliminaryCurveList);

	double fractionalFilter = Locus::GetSampleFractionalFilter ();
	double pullupFractionalFilter = Locus::GetSamplePullupFractionalFilter ();

	smPartOfExtendedLocusLeft extendedLocusLeft;
	smPartOfExtendedLocusRight extendedLocusRight;
	smPullUp pullup;
	smHeightBelowFractionalFilter belowFractionalFilter;
	smHeightBelowPullupFractionalFilter belowPullupFractionalFilter;

//	TestProximityArtifactsSM ();
	
	while (nextSignal = (DataSignal*) cIt ()) {

		peak = nextSignal->Peak ();

		if (peak > maxPeak)
			maxPeak = peak;
	}

	while (nextLocus = (Locus*) it ()) {

		nextLocus->TestFractionalFiltersSM (ArtifactList, fractionalFilterList);
		peak = nextLocus->GetMaximumPeak ();

		if (peak > maxPeak)
			maxPeak = peak;
	}

	double fractionalThreshold = fractionalFilter * maxPeak;
	double pullupFractionalThreshold = pullupFractionalFilter * maxPeak;

	if ((maxPeak < 0.0) || (fractionalFilter < 0.0))
		fractionalThreshold = 0.0;

	if ((maxPeak < 0.0) || (pullupFractionalFilter < 0.0))
		pullupFractionalThreshold = 0.0;

	while (nextSignal = (DataSignal*) fractionalFilterList.GetFirst ()) {

		if ((!nextSignal->IsPossibleInterlocusAllele (-1)) && (!nextSignal->IsPossibleInterlocusAllele (1))) {
		
//			ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this????????????????????????????????????????????????????????????????
			PreliminaryCurveList.RemoveReference (nextSignal);
//testing			nextSignal->SetAlleleName ("");
		}
	}

	cIt.Reset ();
//	bool removeSignal;

	while (nextSignal = (DataSignal*) cIt ()) {

		if (nextSignal->GetMessageValue (extendedLocusLeft) || nextSignal->GetMessageValue (extendedLocusRight))
			continue;

		peak = nextSignal->Peak ();
//		removeSignal = false;

		if (peak < fractionalThreshold) {

//			removeSignal = true;
			nextSignal->SetMessageValue (belowFractionalFilter, true);
		}

		if (nextSignal->GetMessageValue (pullup) && (peak < pullupFractionalThreshold)) {

//			removeSignal = true;
			nextSignal->SetMessageValue (belowPullupFractionalFilter, true);
		}

//		if (removeSignal)
//			cIt.RemoveCurrentItem ();
	}

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->PromoteNonAmbiguousSignalsToAlleles (PreliminaryCurveList);

	return 0;
}


int STRSampleChannelData :: TestForInterlocusProximityArtifactsSM () {

	//
	//  This is sample stage 3
	//

	if (DisableStutterFilter && DisableAdenylationFilter)
		return 0;

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	RGDList adenylationList;
	RGDList stutterList;

	smStutter stutter;
	smAdenylation adenylation;

	while (nextLocus = (Locus*) it ())
		nextLocus->TestInterlocusSignalsSM (PreliminaryCurveList, ArtifactList, adenylationList, stutterList);

	// Now clean up artifact lists, then test remaining signals for adenylation

	DataSignal* nextSignal;
	DataSignal* testSignal;

	while (nextSignal = (DataSignal*) adenylationList.GetFirst ()) {

		nextSignal->SetMessageValue (adenylation, true);
//		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this????????????????????????????????????????????????
		PreliminaryCurveList.RemoveReference (nextSignal);
//testing		nextSignal->SetAlleleName ("");

		if (nextSignal->IsPossibleInterlocusAllele (-1)) {

			nextSignal->SetPossibleInterlocusAllele (-1, false);
			nextLocus = (Locus*) nextSignal->GetLocus (-1);

			if (nextLocus != NULL)
				nextLocus->RemoveSignalFromLocusList (nextSignal);
		}

		if (nextSignal->IsPossibleInterlocusAllele (1)) {

			nextSignal->SetPossibleInterlocusAllele (1, false);
			nextLocus = (Locus*) nextSignal->GetLocus (1);

			if (nextLocus != NULL)
				nextLocus->RemoveSignalFromLocusList (nextSignal);
		}
	}

	while (nextSignal = (DataSignal*) stutterList.GetFirst ()) {

		nextSignal->SetMessageValue (stutter, true);
//		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this????????????????????????????????????????????????
		PreliminaryCurveList.RemoveReference (nextSignal);
//testing		nextSignal->SetAlleleName ("");

		if (nextSignal->IsPossibleInterlocusAllele (-1)) {

			nextSignal->SetPossibleInterlocusAllele (-1, false);
			nextLocus = (Locus*) nextSignal->GetLocus (-1);

			if (nextLocus != NULL)
				nextLocus->RemoveSignalFromLocusList (nextSignal);
		}

		if (nextSignal->IsPossibleInterlocusAllele (1)) {

			nextSignal->SetPossibleInterlocusAllele (1, false);
			nextLocus = (Locus*) nextSignal->GetLocus (1);

			if (nextLocus != NULL)
				nextLocus->RemoveSignalFromLocusList (nextSignal);
		}
	}

	if (DisableAdenylationFilter)
		return 0;

	RGDListIterator signalIterator (PreliminaryCurveList);  // signals outside lane standard limits should already be gone, so no need to test...right?
	RGDListIterator adenylationTest (PreliminaryCurveList);
	double peak;
	double cumulativePeak;
	double approxID;
	double testID;
	double leftLimit;
	double rightLimit;
	double leftUpperLimit;
	double rightLowerLimit;
	double threshold = Locus::GetSampleAdenylationThreshold ();

	while (nextSignal = (DataSignal*) signalIterator ()) {

		adenylationTest.Reset ();
		peak = nextSignal->Peak ();
		cumulativePeak = 0.0;
		approxID = nextSignal->GetApproximateBioID ();
		leftLimit = approxID - 1.1;
		leftUpperLimit = approxID - 0.9;
		rightLimit = approxID + 1.1;
		rightLowerLimit = approxID + 0.9;

		while (testSignal = (DataSignal*) adenylationTest ()) {

			testID = testSignal->GetApproximateBioID ();

			if (testID < approxID)	//	12/15/2013...changed to eliminate +A
				continue;

			if (testID > rightLimit)
				break;

			//if (testID < leftUpperLimit)
			//	cumulativePeak += testSignal->Peak ();

			if (testID > rightLowerLimit)
				cumulativePeak += testSignal->Peak ();
		}

		if (peak <= threshold * cumulativePeak) {

			signalIterator.RemoveCurrentItem ();
			adenylationList.Append (nextSignal);
		}
	}

	while (nextSignal = (DataSignal*) adenylationList.GetFirst ()) {

		nextSignal->SetMessageValue (adenylation, true);
//		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this????????????????????????????????????
//testing		nextSignal->SetAlleleName ("");

		if (nextSignal->IsPossibleInterlocusAllele (-1)) {

			nextSignal->SetPossibleInterlocusAllele (-1, false);
			nextLocus = (Locus*) nextSignal->GetLocus (-1);

			if (nextLocus != NULL)
				nextLocus->RemoveSignalFromLocusList (nextSignal);
		}

		if (nextSignal->IsPossibleInterlocusAllele (1)) {

			nextSignal->SetPossibleInterlocusAllele (1, false);
			nextLocus = (Locus*) nextSignal->GetLocus (1);

			if (nextLocus != NULL)
				nextLocus->RemoveSignalFromLocusList (nextSignal);
		}
	}

	return 0;
}


int STRSampleChannelData :: FinalTestForPeakSizeAndNumberSM (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes) {

	//
	//  This is sample stage 5
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	int status = 0;

	while (nextLocus = (Locus*) it ()) {

		if (nextLocus->FinalTestForPeakSizeAndNumberSM (averageHeight, isNegCntl, isPosCntl, pGenotypes, ArtifactList) < 0)
			status = -1;
	}

	//if (isNegCntl) {

	//	it.Reset ();

	//	while (nextLocus = (Locus*) it ()) {

	//		if (nextLocus->GetMessageValue (unexpectedPeaksInLocus)) {

	//			SetMessageValue (unexpectedPeaksInChannel, true);
	//			break;
	//		}
	//	}
	//}

	return status;
}


int STRSampleChannelData :: TestForMultiSignalsSM () {

	// All signals outside lane standard have already been removed.  Also, all signals definitely assigned to a locus are gone.
	// What's left are ambiguous signals and also those that are truly interlocus.  Test the ambiguous ones for possible craters...

	//
	//  This is sample stage 3
	//

	RGDListIterator it (PreliminaryCurveList);
	DataSignal* nextSignal;
	DataSignal* prevSignal = NULL;

	DataSignal* prevCrater;
	DataSignal* nextCrater;
//	DataSignal* craterSignal;

	Locus* nextLocus;
	const Locus* flankingLocusLeft;
	const Locus* flankingLocusRight;
	RGDListIterator locusIt (mLocusList);

	bool multipleSignalsLeft;
	bool multipleSignalsRight;

	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smSignalNotACrater notACrater;
	smSignalNotACraterSidePeak notASidePeak;
	smPullUp pullup;
	smPrimaryInterchannelLink primaryLink;
	smAmbiguousInterlocusCrater ambiguousCrater;
	InterchannelLinkage* iChannel;

	RGDList SignalsToDeleteFromSignalList;
	RGDList SignalsToDeleteFromAll;

	while (nextLocus = (Locus*) locusIt ())
		nextLocus->PromoteNonAmbiguousSignalsToAlleles (PreliminaryCurveList);

	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		// First check for false craters

		multipleSignalsLeft = nextSignal->TestForMultipleSignals (prevCrater, nextCrater, -1);
		multipleSignalsRight = nextSignal->TestForMultipleSignals (prevCrater, nextCrater, 1);

		if (multipleSignalsLeft || multipleSignalsRight) {

			SignalsToDeleteFromAll.Append (nextSignal);
			nextSignal->SetMessageValue (notACrater, true);

			if (prevCrater != NULL) {

				prevCrater->SetMessageValue (notASidePeak, true);
	//			prevCrater->RemoveSignalLink ();
			}

			if (nextCrater != NULL) {

				nextCrater->SetMessageValue (notASidePeak, true);
	//			nextCrater->RemoveSignalLink ();
			}

			if (!multipleSignalsLeft || !multipleSignalsRight) {

				// This is a true ambiguous because the interpeak spacing varies with locus.  Undo the crater as above
				// and add both flanking peaks to artifact list with appropriate notices.  Remove flanking peaks from
				// loci.

				if (prevCrater != NULL) {

					prevCrater->SetMessageValue (ambiguousCrater, true);
					SignalsToDeleteFromSignalList.Append (prevCrater);
				}

				if (nextCrater != NULL) {

					nextCrater->SetMessageValue (ambiguousCrater, true);
					SignalsToDeleteFromSignalList.Append (nextCrater);
				}
			}
		}

		else {

			// If prevCrater != NULL and/or nextCrater != NULL, this is a true crater...remove the flanking signals

			if (prevCrater != NULL)
				SignalsToDeleteFromSignalList.Append (prevCrater);

			if (nextCrater != NULL)
				SignalsToDeleteFromSignalList.Append (nextCrater);
		}
	}

	// Now remove all remaining crater flanking signals prior to testing for undiagnosed craters

	while (nextSignal = (DataSignal*) SignalsToDeleteFromSignalList.GetFirst ()) {

//testing		nextSignal->SetVirtualAlleleName ("");
//testing		nextSignal->SetAlleleName ("");
		PreliminaryCurveList.RemoveReference (nextSignal);
//		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this??????????????????????????????????????????????????????????
		nextLocus = (Locus*) nextSignal->GetLocus (-1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);

		nextLocus = (Locus*) nextSignal->GetLocus (1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);
	}

	while (nextSignal = (DataSignal*) SignalsToDeleteFromAll.GetFirst ()) {

		PreliminaryCurveList.RemoveReference (nextSignal);
		ArtifactList.RemoveReference (nextSignal);	// May need this to prevent ArtifactList from containing deleted elements
//		CompleteCurveList.RemoveReference (nextSignal);
//		SmartPeaks.RemoveReference (nextSignal);
		nextLocus = (Locus*) nextSignal->GetLocus (-1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);

		nextLocus = (Locus*) nextSignal->GetLocus (1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);

		iChannel = nextSignal->GetInterchannelLink ();

		if (iChannel != NULL) {

			iChannel->RemoveDataSignalSM (nextSignal);

			// Later, consider adding one of nextSignal's flanking peaks...need iChannel function to perform test, assuming
			// that primary Signal may be gone.  Then recalculate primary signal

			if (iChannel->IsEmpty ())
				iChannel->RemoveAllSM ();

			else
				iChannel->RecalculatePrimarySignalSM ();
		}

	//	delete nextSignal;
	}

	it.Reset ();
	smPoorPeakMorphologyOrResolution poorPeakMorphologyOrResolution;
	smAmbiguousInterlocusWithPoorMorphology ambiguousInterlocusWithPoorMorphology;

	while (nextSignal = (DataSignal*) it ()) {

		// Now check for undiagnosed craters

		if (prevSignal != NULL) {

			if (!prevSignal->IsPossibleInterlocusAllele (-1)) {

				prevSignal = nextSignal;
				continue;
			}

			if (!nextSignal->IsPossibleInterlocusAllele (-1)) {

				prevSignal = nextSignal;
				continue;
			}

			flankingLocusLeft = prevSignal->GetLocus (-1);
			flankingLocusRight = prevSignal->GetLocus (1);

			if ((flankingLocusLeft == NULL) || (flankingLocusRight == NULL)) {

				prevSignal = nextSignal;
				continue;
			}

			if ((flankingLocusLeft != nextSignal->GetLocus (-1)) || (flankingLocusRight != nextSignal->GetLocus (1))) {

				prevSignal = nextSignal;
				continue;
			}

			if (prevSignal->GetAlleleName (-1) == nextSignal->GetAlleleName (-1)) {

				if (prevSignal->GetAlleleName (1) == nextSignal->GetAlleleName (1)) {

					// This is not a crater though...01/01/2014

					if ((prevSignal->Peak () > nextSignal->Peak ()) || nextSignal->GetMessageValue (pullup)) {

						nextSignal->SetMessageValue (ambiguousInterlocusWithPoorMorphology, true);
						SignalsToDeleteFromSignalList.Append (nextSignal);
					}

					else if ((nextSignal->Peak () > prevSignal->Peak ()) || prevSignal->GetMessageValue (pullup)) {

						prevSignal->SetMessageValue (ambiguousInterlocusWithPoorMorphology, true);
						SignalsToDeleteFromSignalList.Append (prevSignal);
					}

					else {

						nextSignal->SetMessageValue (ambiguousInterlocusWithPoorMorphology, true);
						SignalsToDeleteFromSignalList.Append (nextSignal);
						prevSignal->SetMessageValue (ambiguousInterlocusWithPoorMorphology, true);
						SignalsToDeleteFromSignalList.Append (prevSignal);
					}

					prevSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
					nextSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
					
					prevSignal = nextSignal;
				}

				else {

					// These are ambiguous signals...close enough to be called the same in locus to left but not to right

					nextLocus = (Locus*) nextSignal->GetLocus (-1);

					if (nextLocus != NULL) {

						nextLocus->RemoveSignalFromLocusList (nextSignal);
						nextLocus->RemoveSignalFromLocusList (prevSignal);
					}

					prevSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
					nextSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);

					prevSignal = nextSignal;
				}
			}

			else if (prevSignal->GetAlleleName (1) == nextSignal->GetAlleleName (1)) {

				// These are ambiguous signals...close enough to be called the same in locus to right but not to left

				nextLocus = (Locus*) nextSignal->GetLocus (1);

				if (nextLocus != NULL) {

					nextLocus->RemoveSignalFromLocusList (nextSignal);
					nextLocus->RemoveSignalFromLocusList (prevSignal);
				}

				prevSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
				nextSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);

				prevSignal = nextSignal;
			}

			else {

				prevSignal = nextSignal;
				continue;
			}
		}

		else
			prevSignal = nextSignal;
	}

	while (nextSignal = (DataSignal*) SignalsToDeleteFromSignalList.GetFirst ()) {

//testing		nextSignal->SetVirtualAlleleName ("");
//testing		nextSignal->SetAlleleName ("");
		PreliminaryCurveList.RemoveReference (nextSignal);
//		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this???????????????????????????????????????????????
		nextLocus = (Locus*) nextSignal->GetLocus (-1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);

		nextLocus = (Locus*) nextSignal->GetLocus (1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);
	}

	return 0;
}


int STRSampleChannelData :: TestProximityArtifactsSM () {

	//
	//  This is sample stage 3
	//

	if (DisableStutterFilter && DisableAdenylationFilter)
		return 0;

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	RGDList stutterList;
	RGDList adenylationList;

	while (nextLocus = (Locus*) it ())
		nextLocus->TestProximityArtifactsUsingLocusBasePairsSM (ArtifactList, adenylationList, stutterList);

	DataSignal* nextSignal;

	smStutter stutter;
	smAdenylation adenylation;
	smPartOfExtendedLocusLeft partOfExtendedLocusLeft;
	smPartOfExtendedLocusRight partOfExtendedLocusRight;

	while (nextSignal = (DataSignal*) stutterList.GetFirst ()) {

//		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this??????????????????????????????????????????????????????????????
		PreliminaryCurveList.RemoveReference (nextSignal);
		nextSignal->SetMessageValue (stutter, true);

		if (adenylationList.RemoveReference (nextSignal) != NULL)
			nextSignal->SetMessageValue (adenylation, true);
	}

	while (nextSignal = (DataSignal*) adenylationList.GetFirst ()) {

//		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this??????????????????????????????????????????????????????????????
		PreliminaryCurveList.RemoveReference (nextSignal);
		nextSignal->SetMessageValue (adenylation, true);
	}

	return 0;
}


int STRSampleChannelData :: AssignSignalToFirstLocusAndDeleteFromSecondSM (DataSignal* target, Locus* locus1, Locus* locus2) {

	//
	//  This is sample stage 4
	//

	smPossibleAmbiguousInterlocusAssignedToLocus ambiguousInterlocus;
	
//	ArtifactList.InsertWithNoReferenceDuplication (target);	// Need this??????????????????????????????????????????????????????????????????
	locus1->PromoteSignalToAllele (target);
	locus2->RemoveSignalFromLocusList (target);
	target->SetMessageValue (ambiguousInterlocus, true);
	return 0;
}


int STRSampleChannelData :: RemoveSignalFromBothLociSM (DataSignal* target, Locus* locus1, Locus* locus2) {

	//
	//  This is sample stage 4
	//

	smAmbiguousInterlocus ambiguousInterlocus;

//	ArtifactList.InsertWithNoReferenceDuplication (target);	// Need this??????????????????????????????????????????????????????????????????
	locus1->RemoveSignalFromLocusList (target);
	locus2->RemoveSignalFromLocusList (target);
	target->SetMessageValue (ambiguousInterlocus, true);
	return 0;
}


bool STRSampleChannelData :: TestIfSignalBelongsToFirstLocusSM (DataSignal* target, Locus* locus1, Locus* locus2) {

	int nAmb1 = locus1->NumberOfAmbiguousAlleles ();
	int nAmb2 = locus2->NumberOfAmbiguousAlleles ();
	int nUnamb1 = locus1->NumberOfAlleles () - nAmb1;
	int nUnamb2 = locus2->NumberOfAlleles () - nAmb2;
	bool isOffGrid1;
	bool isOffGrid2;
	bool acceptedTriAllele1;
	bool acceptedTriAllele2;
	int position1;
	int position2;
	smPullUp pullup;
	bool hasPullup;

	if (locus1 = (Locus*) target->GetLocus (-1))
		position1 = -1;

	else
		position1 = 1;

	position2 = - position1;

	isOffGrid1 = target->IsOffGrid (position1);
	isOffGrid2 = target->IsOffGrid (position2);

	acceptedTriAllele1 = target->IsAcceptedTriAllele (position1);
	acceptedTriAllele2 = target->IsAcceptedTriAllele (position2);

	if (target->GetMessageValue (pullup))
		hasPullup = true;

	else
		hasPullup = false;

	if (nUnamb1 == 1) {

		if ((nUnamb2 >= 2) && (!acceptedTriAllele2))
			return true;

		if ((!isOffGrid1) && (nUnamb2 == 1) && isOffGrid2 && (!acceptedTriAllele2))
			return true;

		if (acceptedTriAllele1 && (nUnamb2 == 1) && isOffGrid2 && (!acceptedTriAllele2))
			return true;
	}

	else if (nUnamb1 == 0) {

		if (nUnamb2 >= 2)
			return true;

		if ((nAmb1 == 1) && (nUnamb2 == 1))
			return true;

		if ((nAmb1 >= 2) && (!isOffGrid1) && (nUnamb2 == 1) && isOffGrid2 && (!acceptedTriAllele2))
			return true;

		if ((nAmb1 >= 2) && acceptedTriAllele1 && (nUnamb2 == 1) && isOffGrid2 && (!acceptedTriAllele2))
			return true;
	}

	else if (nUnamb1 >= 2) {

		if ((nUnamb2 >= 2) && isOffGrid2 && (!acceptedTriAllele2) && (!hasPullup)) {

			if (!isOffGrid1 || acceptedTriAllele1)
				return true;
		}
	}

	return false;
}


int STRSampleChannelData :: ResolveAmbiguousInterlocusSignalsSM () {

	RGDListIterator it (PreliminaryCurveList);
	DataSignal* nextSignal;
	bool wouldCauseHeterozygousImbalance1;
	bool wouldCauseHeterozygousImbalance2;
	Locus* locus1;
	Locus* locus2;
	bool locus1IsIt;
	bool locus2IsIt;

	while (nextSignal = (DataSignal*) it ()) {

		if ((nextSignal->IsPossibleInterlocusAllele (-1)) && (nextSignal->IsPossibleInterlocusAllele (1))) {

			locus1 = (Locus*) nextSignal->GetLocus (-1);
			locus2 = (Locus*) nextSignal->GetLocus (1);

			locus1IsIt = locus2IsIt = false;

			if ((locus1 == NULL) && (locus2 == NULL))
				continue;

			if ((locus1 != NULL) && (locus2 == NULL)) {

				locus1->PromoteSignalToAllele (nextSignal);
				it.RemoveCurrentItem ();
				continue;
			}

			if ((locus2 != NULL) && (locus1 == NULL)) {

				locus2->PromoteSignalToAllele (nextSignal);
				it.RemoveCurrentItem ();
				continue;
			}

			locus1->HasHeightRatioWithExclusiveMaxPeak (nextSignal, wouldCauseHeterozygousImbalance1);
			locus2->HasHeightRatioWithExclusiveMaxPeak (nextSignal, wouldCauseHeterozygousImbalance2);

			if ((!wouldCauseHeterozygousImbalance1) || (!wouldCauseHeterozygousImbalance2)) {

				if (wouldCauseHeterozygousImbalance1)
					locus2IsIt = true;

				else if (wouldCauseHeterozygousImbalance2)
					locus1IsIt = true;

				else {

					// Neither would cause heterozygous imbalance...we must distinguish using other criteria

					locus1IsIt = TestIfSignalBelongsToFirstLocusSM (nextSignal, locus1, locus2);
					locus2IsIt = TestIfSignalBelongsToFirstLocusSM (nextSignal, locus2, locus1);

					if (locus1IsIt && locus2IsIt) {

						locus1IsIt = false;
						locus2IsIt = false;
					}
				}
			}

			it.RemoveCurrentItem ();

			if (locus1IsIt)
				AssignSignalToFirstLocusAndDeleteFromSecondSM (nextSignal, locus1, locus2);

			else if (locus2IsIt)
				AssignSignalToFirstLocusAndDeleteFromSecondSM (nextSignal, locus2, locus1);

			else
				RemoveSignalFromBothLociSM (nextSignal, locus1, locus2);
		}
	}

	//?????????????????????????????????????????????????????????

	// Replace below with report condition on residual error

	//RGDListIterator artIt (ArtifactList);
	//smBPResidualTooLarge residualTooLarge;

	//while (nextSignal = (DataSignal*) artIt ()) {

	//	if (nextSignal->GetAlleleName () == "")
	//		nextSignal->SetMessageValue (residualTooLarge, false);
	//}

	return 0;
}


int STRSampleChannelData :: MeasureInterlocusSignalAttributesSM () {

	//
	//  This is sample stage 3
	//

	RGDListIterator it (PreliminaryCurveList);
	DataSignal* nextSignal;
	bool wouldCauseHeterozygousImbalance1;
	bool wouldCauseHeterozygousImbalance2;
	Locus* locus1;
	Locus* locus2;
	bool isOffGrid1;
	bool isOffGrid2;

	smWouldCauseHeterozygousImbalanceLeft wouldCauseHeterozygousImbalanceLeft;
	smWouldCauseHeterozygousImbalanceRight wouldCauseHeterozygousImbalanceRight;
	smSignalOffGridLeft signalOffGridLeft;
	smSignalOffGridRight signalOffGridRight;
	sm0UnambiguousPeaksLeft are0UnambiguousPeaksLeft;
	sm0UnambiguousPeaksRight are0UnambiguousPeaksRight;
	sm1UnambiguousPeakLeft is1UnambiguousPeakLeft;
	sm1UnambiguousPeakRight is1UnambiguousPeakRight;
	sm2PlusUnambiguousPeaksLeft are2PlusUnambiguousPeaksLeft;
	sm2PlusUnambiguousPeaksRight are2PlusUnambiguousPeaksRight;
	sm0AmbiguousPeaksLeft are0AmbiguousPeaksLeft;
	sm0AmbiguousPeaksRight are0AmbiguousPeaksRight;
	sm1AmbiguousPeakLeft is1AmbiguousPeakLeft;
	sm1AmbiguousPeakRight is1AmbiguousPeakRight;
	sm2PlusAmbiguousPeaksLeft are2PlusAmbiguousPeaksLeft;
	sm2PlusAmbiguousPeaksRight are2PlusAmbiguousPeaksRight;
	int nAmb1;
	int nAmb2;
	int nUnamb1;
	int nUnamb2;

	while (nextSignal = (DataSignal*) it ()) {

		if ((nextSignal->IsPossibleInterlocusAllele (-1)) && (nextSignal->IsPossibleInterlocusAllele (1))) {

			locus1 = (Locus*) nextSignal->GetLocus (-1);
			locus2 = (Locus*) nextSignal->GetLocus (1);

			if ((locus1 == NULL) && (locus2 == NULL))
				continue;

			if ((locus1 != NULL) && (locus2 == NULL)) {

				locus1->PromoteSignalToAllele (nextSignal);
				it.RemoveCurrentItem ();
				continue;
			}

			if ((locus2 != NULL) && (locus1 == NULL)) {

				locus2->PromoteSignalToAllele (nextSignal);
				it.RemoveCurrentItem ();
				continue;
			}

			locus1->HasHeightRatioWithExclusiveMaxPeak (nextSignal, wouldCauseHeterozygousImbalance1);
			locus2->HasHeightRatioWithExclusiveMaxPeak (nextSignal, wouldCauseHeterozygousImbalance2);

			if (wouldCauseHeterozygousImbalance1)
				nextSignal->SetMessageValue (wouldCauseHeterozygousImbalanceLeft, true);

			if (wouldCauseHeterozygousImbalance2)
				nextSignal->SetMessageValue (wouldCauseHeterozygousImbalanceRight, true);

			nAmb1 = locus1->NumberOfAmbiguousAlleles ();
			nAmb2 = locus2->NumberOfAmbiguousAlleles ();
			nUnamb1 = locus1->NumberOfAlleles () - nAmb1;
			nUnamb2 = locus2->NumberOfAlleles () - nAmb2;

			isOffGrid1 = nextSignal->IsOffGrid (-1);
			isOffGrid2 = nextSignal->IsOffGrid (1);

			if (nUnamb1 == 0)
				nextSignal->SetMessageValue (are0UnambiguousPeaksLeft, true);

			else if (nUnamb1 == 1)
				nextSignal->SetMessageValue (is1UnambiguousPeakLeft, true);

			else
				nextSignal->SetMessageValue (are2PlusUnambiguousPeaksLeft, true);

			if (nAmb1 == 0)
				nextSignal->SetMessageValue (are0AmbiguousPeaksLeft, true);

			else if (nAmb1 == 1)
				nextSignal->SetMessageValue (is1AmbiguousPeakLeft, true);

			else
				nextSignal->SetMessageValue (are2PlusAmbiguousPeaksLeft, true);

			if (nUnamb2 == 0)
				nextSignal->SetMessageValue (are0UnambiguousPeaksRight, true);

			else if (nUnamb2 == 1)
				nextSignal->SetMessageValue (is1UnambiguousPeakRight, true);

			else
				nextSignal->SetMessageValue (are2PlusUnambiguousPeaksRight, true);

			if (nAmb2 == 0)
				nextSignal->SetMessageValue (are0AmbiguousPeaksRight, true);

			else if (nAmb2 == 1)
				nextSignal->SetMessageValue (is1AmbiguousPeakRight, true);

			else
				nextSignal->SetMessageValue (are2PlusAmbiguousPeaksRight, true);

			if (isOffGrid1)
				nextSignal->SetMessageValue (signalOffGridLeft, true);

			if (isOffGrid2)
				nextSignal->SetMessageValue (signalOffGridRight, true);
		}
	}

	return 0;
}


int STRSampleChannelData :: ResolveAmbiguousInterlocusSignalsUsingSmartMessageDataSM () {

	//
	//  This is sample stage 4
	//

	RGDListIterator it (PreliminaryCurveList);
	DataSignal* nextSignal;
	smCouldBelongToLocusLeft couldBelongToLocusLeft;
	smCouldBelongToLocusRight couldBelongToLocusRight;
	smAmbiguousInterlocus ambiguousInterlocus;
	Locus* locus1;
	Locus* locus2;
	bool locus1IsIt;
	bool locus2IsIt;

	while (nextSignal = (DataSignal*) it ()) {

		if ((nextSignal->IsPossibleInterlocusAllele (-1)) && (nextSignal->IsPossibleInterlocusAllele (1))) {

			locus1 = (Locus*) nextSignal->GetLocus (-1);
			locus2 = (Locus*) nextSignal->GetLocus (1);

			if ((locus1 == NULL) && (locus2 == NULL))
				continue;

			if ((locus1 != NULL) && (locus2 == NULL)) {

				locus1->PromoteSignalToAllele (nextSignal);
				it.RemoveCurrentItem ();
				continue;
			}

			if ((locus2 != NULL) && (locus1 == NULL)) {

				locus2->PromoteSignalToAllele (nextSignal);
				it.RemoveCurrentItem ();
				continue;
			}

			locus1IsIt = nextSignal->GetMessageValue (couldBelongToLocusLeft);	//These should have been set by end of stage 3 evaluation and triggers
			locus2IsIt = nextSignal->GetMessageValue (couldBelongToLocusRight);

			if (locus1IsIt && !locus2IsIt) {

				AssignSignalToFirstLocusAndDeleteFromSecondSM (nextSignal, locus1, locus2);
				it.RemoveCurrentItem ();
			}

			else if (locus2IsIt && !locus1IsIt) {

				AssignSignalToFirstLocusAndDeleteFromSecondSM (nextSignal, locus2, locus1);
				it.RemoveCurrentItem ();
				continue;
			}

			else
				RemoveSignalFromBothLociSM (nextSignal, locus1, locus2);	// it's either in neither or both, so it has to go
		}
	}

	return 0;
}


int STRSampleChannelData :: TestSignalsForRaisedBaseline (double left, double report) {

	return -1;
}


int STRSampleChannelData :: TestForRaisedBaselineAndExcessiveNoiseSM (double left, double report) {

	smChannelHasRaisedBaseline channelHasRaisedBaseline;
	smChannelHasExcessiveNoiseAboveAnalysisThreshold excessiveNoiseAboveAnalysis;
	smChannelHasExcessiveNoiseAboveDetectionThreshold excessiveNoiseBelowAnalysis;
	smSampleChannelRaisedBaselineThreshold raisedBaseline;

	double analysisThreshold = GetMinimumHeight ();
	double detectionThreshold = GetDetectionThreshold ();
	double raisedBaselineThreshold = (double) GetThreshold (raisedBaseline);

	RaisedBaseLineData* rbld;
	PoorFitPeakData* pfpd;

	double minTest = left;
	double height;

	if (report > left)
		minTest = report;

	if (raisedBaselineThreshold > 0.0) {

		while (rbld = mRaisedBaseLines.RemoveFirst ()) {

			if (rbld->GetLeft () < minTest)
				continue;

			height = rbld->GetHeight ();

			if (height >= raisedBaselineThreshold) {

				SetMessageValue (channelHasRaisedBaseline, true);
				AppendDataForSmartMessage (channelHasRaisedBaseline, (int) floor ((double)height));
				AppendDataForSmartMessage (channelHasRaisedBaseline, (int) floor ((double)rbld->GetLeft ()));
				break;
			}
		}
	}

	mRaisedBaseLines.clearAndDestroy ();

	while (pfpd = mPoorFits.RemoveFirst ()) {

		if (pfpd->GetMode () < minTest)
			continue;

		height = pfpd->GetHeight ();

		if (height >= analysisThreshold) {

			SetMessageValue (excessiveNoiseAboveAnalysis, true);
			SetMessageValue (excessiveNoiseBelowAnalysis, true);
			break;
		}

		if (height >= detectionThreshold)
			SetMessageValue (excessiveNoiseBelowAnalysis, true);
	}

	mPoorFits.clearAndDestroy ();
	return 0;
}


int STRSampleChannelData :: FitAllNegativeCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	
	//
	//  This is sample stage 1
	//
	
	STRTracePrequalification trace;
	DataSignal* nextSignal;
	double fit;
	DataSignal* signature;
	double minRFU = GetMinimumHeight ();
	double maxRFU = GetMaximumHeight ();
	double detectionRFU = GetDetectionThreshold ();
	double minAcceptableFit = ParametricCurve::GetMinimumFitThreshold ();
	double minFitForArtifactTest = ParametricCurve::GetTriggerForArtifactTest ();
	double minFit = minFitForArtifactTest;
	double absoluteMinFit = ParametricCurve::GetAbsoluteMinimumFit ();
	int i;

	if (minAcceptableFit > minFit)
		minFit = minAcceptableFit;

	if (CoreBioComponent::GetGaussianSignature ())
		signature = new NormalizedGaussian (0.0, ParametricCurve::GetSigmaForSignature ());

	else
		signature = new DoubleGaussian (0.0, ParametricCurve::GetSigmaForSignature ());

	mNegativeCurveList.ClearAndDelete ();
	double lineFit;
	SampledData* negativePeaks = new SampledData (*(SampledData*)mData);
	double* negArray = (double*)negativePeaks->GetData ();
	int n = negativePeaks->GetNumberOfSamples ();

	for (i=0; i<n; i++) {

		if (negArray [i] > 0.0)
			negArray [i] = 0.0;

		else
			negArray [i] = -negArray [i];
	}

	negativePeaks->ResetCharacteristicsFromRight (trace, text, detectionRFU, print);

	Endl endLine;
	ExcelText.SetOutputLevel (1);
	ExcelText << "Using minimum RFU = " << minRFU << endLine;
	ExcelText.ResetOutputLevel ();
	double absoluteMinFitLessEpsilon = absoluteMinFit - 0.01;

	double constantHeight;
	int leftEndPoint;
	int rightEndPoint;
	//cout << "Finding next char. from right (2)" << endl;

	while (nextSignal = negativePeaks->FindNextCharacteristicFromRight (*signature, fit, mNegativeCurveList)) {

		//secondaryContent = fabs(nextSignal->GetScale (2));
		//double mean = nextSignal->GetMean ();
		lineFit = negativePeaks->TestConstantCharacteristicRetry (constantHeight, leftEndPoint, rightEndPoint);

		if (lineFit > minFitForArtifactTest) {

			delete nextSignal;
			continue;
		}

		nextSignal->SetPeakIsNegative ();
		nextSignal->SetCannotBePrimary (true);
		mNegativeCurveList.Prepend (nextSignal);

	}   //  We are done finding negative characteristics

	//cout << "Done finding characteristics" << endl;
	DataSignal* prevSignal = NULL;
	RGDList tempList;
	prevSignal = (DataSignal*)mNegativeCurveList.GetFirst ();

	if (prevSignal == NULL) {

		delete signature;
		return 0;
	}

	double minDistance = ChannelData::GetMinimumDistanceBetweenPeaks ();
	//cout << "Eliminating neg peaks that are too close (2)" << endl;

	//
	//	Do we really want to do this???
	//

	while (nextSignal = (DataSignal*) mNegativeCurveList.GetFirst ()) {

		if (prevSignal != NULL) {

			if (fabs(prevSignal->GetMean () - nextSignal->GetMean ()) < minDistance) {

				// "get rid" of the one that fits least well and use the other for the next test.
				// later, if we want, we can add redundant signal to artifact list with a notice...

				if (prevSignal->GetCurveFit () > nextSignal->GetCurveFit ()) {

					// keep prevSignal and "lose" nextSignal
		//			ArtifactList.RemoveReference (nextSignal);
					delete nextSignal;
					continue;
				}

				else {

		//			ArtifactList.RemoveReference (prevSignal);
					delete prevSignal;
					prevSignal = nextSignal;
					continue;
				}
			}

			else {

				tempList.Append (prevSignal);
				prevSignal = nextSignal;
			}
		}
	}

	if (prevSignal != NULL)
		tempList.Append (prevSignal);

	while (nextSignal = (DataSignal*) tempList.GetFirst ())
		mNegativeCurveList.Append (nextSignal);

	//cout << "Done testing 'too close'" << endl;

	delete signature;
//	ProjectNeighboringSignalsAndTest (1.0, 1.0);
	return 0;
}



int STRSampleChannelData :: AnalyzeDynamicBaselineSM (int startTime) {

	//
	//	Sample Stage 1
	//	Put Baseline Analysis here...
	//

	smTestRelativeBaselinePreset testRelativeBaseline;
	smIgnoreNegativeRelativeBaselinePreset ignoreNegativeBaselineMsg;
	bool ignoreNegativeBaseline = false;

	if (mBaseLine != NULL)
		return 1;

	if (!GetMessageValue (testRelativeBaseline))
		return 0;

	if (mData == NULL)
		return 0;

//	cout << "Start time = " << startTime << endl;
	int numSamples = mData->GetNumberOfSamples ();

	RGDList tempList;
	RGDListIterator itC (CompleteCurveList);
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) itC ()) {

		if (nextSignal->GetMean () < startTime)
			continue;

		tempList.Append (nextSignal);
	}

	RGDListIterator itt (tempList);
	double previousBP = -3.0;
	double currentBP;

	while (nextSignal = (DataSignal*) itt ()) {

		currentBP = nextSignal->GetApproximateBioID ();

		if (CurveIsBaselineFit (nextSignal)) {

			if (currentBP - previousBP < 1.5) {

				previousBP = currentBP;
				continue;
			}

			itt.RemoveCurrentItem ();
			previousBP = currentBP;
		}
	}

	EditOutFitBaselineForNormalization (tempList, mData);	// Added 4:53 pm 08/06/2013
	DataSignal* FitCurve = new CompositeCurve (mData->LeftEndPoint (), mData->RightEndPoint (), tempList);
	FitCurve->ProjectNeighboringSignals (1.0, 1.0);
	DataSignal* FitData = FitCurve->Digitize (numSamples, mData->LeftEndPoint (), 1.0);
	delete FitCurve;
	tempList.Clear ();
	//cout << "Making list of neg peaks for analyzing neg baseline" << endl;
	RGDListIterator itneg (mNegativeCurveList);

	while (nextSignal = (DataSignal*) itneg ()) {

		if (nextSignal->GetMean () < startTime)
			continue;

		tempList.Append (nextSignal);
	}

	itneg.Reset ();
	previousBP = -3.0;
	//cout << "Removing neg peaks that are skewed left" << endl;

	while (nextSignal = (DataSignal*) itneg ()) {

		currentBP = nextSignal->GetApproximateBioID ();

		if (CurveIsBaselineFit (nextSignal)) {

			if (currentBP - previousBP < 1.5) {

				previousBP = currentBP;
				continue;
			}

			itneg.RemoveCurrentItem ();
			previousBP = currentBP;
		}
	}

	//cout << "Creating neg composite curve" << endl;

	DataSignal* FitNegCurve = new CompositeCurve (mData->LeftEndPoint (), mData->RightEndPoint (), tempList);
	FitNegCurve->ProjectNeighboringSignals (1.0, 1.0);
	//cout << "Creating digitized neg curve" << endl;
	DataSignal* FitNegData = FitNegCurve->Digitize (numSamples, mData->LeftEndPoint (), 1.0);
	delete FitNegCurve;

	int left = startTime;
	int end = (int)floor (mData->RightEndPoint ());
	int localLeft;
	int localRight;
	smBaselineEstimationThreshold baselineThreshold;
	double rfuThreshold;		// = 0.1 * STRSampleChannelData::GetMinRFU ();  // changed 12/31/2013
	rfuThreshold = (double) GetThreshold (baselineThreshold);

	if (rfuThreshold <= 0.0)
		rfuThreshold = 1.0;

	list<int> lefts;
	list<int> rights;
	double mean;
	//cout << "Finding valid intervals using both pos and neg data" << endl;

	while (FindNextFitDataIntervalBelowThreshold (rfuThreshold, left, end, localLeft, localRight, FitData, FitNegData)) {

		lefts.push_back (localLeft);
		rights.push_back (localRight);
		left = localRight + 2;
	}

	//cout << "Done finding valid intervals" << endl;

	list<double> knotTimes;
	list<double> knotValues;
	list<bool> firstList;
	list<bool> lastList;
	double temp;
	bool firstInterval = true;

	while (!lefts.empty ()) {

		localLeft = lefts.front ();
		localRight = rights.front ();
		lefts.pop_front ();
		rights.pop_front ();
		AppendKnotDataWithEditingToLists (localLeft, localRight, knotTimes, knotValues, firstList, lastList, firstInterval, mData);
//		AppendKnotDataToLists (localLeft, localRight, knotTimes, knotValues, mData);
		firstInterval = false;
	}

	temp = mData->RightEndPoint ();
	knotTimes.push_back (temp + 100.0);
	knotTimes.push_back (temp + 200.0);
	knotTimes.push_back (temp + 300.0);
	knotValues.push_back (0.0);
	knotValues.push_back (0.0);
	knotValues.push_back (0.0);
	firstList.push_back (true);
	firstList.push_back (false);
	firstList.push_back (false);
	lastList.push_back (false);
	lastList.push_back (false);
	lastList.push_back (true);
	//temp = knotTimes.front ();

	//if (temp > startTime + 15) {

	//	temp = BaselineAverage (startTime - 7, startTime + 7, mData, 15.0);
	//	knotValues.push_front (temp);
	//	knotTimes.push_front ((double)startTime);
	//}

	temp = knotValues.front ();
	knotTimes.push_front ((double)(startTime / 2));
	knotValues.push_front (temp);
	knotTimes.push_front (0.0);
	knotValues.push_front (0.0);
	firstList.push_front (false);
	lastList.push_front (true);

	knotTimes.push_front (-100.0);
	knotTimes.push_front (-200.0);
	knotTimes.push_front (-300.0);
	knotValues.push_front (0.0);
	knotValues.push_front (0.0);
	knotValues.push_front (0.0);
	firstList.push_front (false);
	firstList.push_front (false);
	firstList.push_front (true);
	lastList.push_front (false);
	lastList.push_front (false);
	lastList.push_front (false);

	double lastTime = -500.0;
	list<double> knotTimes2;
	list<double> knotValues2;
	double tempValue;

	if (STRSampleChannelData::UseOldBaselineEstimation)
		ShapeBaselineData (knotTimes, knotValues);  // commented and added below 08/26/2013

	else {

		ShapeBaselineData (knotTimes, knotValues, FitData, FitNegData, rfuThreshold);
		EditPeaksForOutOfRange (knotTimes, knotValues, FitNegData, rfuThreshold);
	}

	while (!knotTimes.empty ()) {

		temp = knotTimes.front ();
		tempValue = knotValues.front ();
		knotTimes.pop_front ();
		knotValues.pop_front ();

		if (temp > lastTime + 2.0) {

			knotTimes2.push_back (temp);
			knotValues2.push_back (tempValue);
			lastTime = temp;
		}
	}

	delete FitData;
	delete FitNegData;
	mBaseLine = new CSplineTransform (knotTimes2, knotValues2);
	knotTimes.clear ();
	knotValues.clear ();
	firstList.clear ();
	lastList.clear ();

	if (!mBaseLine->IsValid ()) {

		delete mBaseLine;
		mBaseLine = NULL;
//		cout << "Baseline analysis failed..." << endl;
		return 0;
	}

//	cout << "Baseline analysis succeeded..." << endl;
	mBaselineStart = 0;

	if (GetMessageValue (ignoreNegativeBaselineMsg))
		ignoreNegativeBaseline = true;

//	DataSignal* nextSignal;
	RGDListIterator it (PreliminaryCurveList);
	double dynamicBaseline;
	double minRFU = STRSampleChannelData::GetMinRFU ();
	smBaselineRelativeBelowMinRFU belowRelativeMinRFU;
	bool firstPeakChanged = false;

	while (nextSignal = (DataSignal*) it ()) {

		mean = nextSignal->GetMean ();

		if (mean < startTime)
			continue;

		dynamicBaseline = mBaseLine->EvaluateWithExtrapolation (mean);

		if (!firstPeakChanged) {

			firstPeakChanged = true;
	//		cout << "First peak with relative baseline...mean = " << mean << ".  Baseline = " << dynamicBaseline << endl;
		}

		if ((dynamicBaseline <= 0.0) && ignoreNegativeBaseline)
			continue;

		if (fabs (dynamicBaseline) <= rfuThreshold)
			continue;

		nextSignal->SetBaseline (dynamicBaseline);
		nextSignal->SetHasRaisedBaseline (true);

		

		if (nextSignal->GetBaselineRelativePeak () <= minRFU) {

			nextSignal->SetMessageValue (belowRelativeMinRFU, true);
		}
	}

	return 1;
}


int STRSampleChannelData :: AnalyzeDynamicBaselineAndNormalizeRawDataSM (int startTime) {

	//
	//	Sample Stage 1
	//	Put Baseline Analysis here...
	//

	smIgnoreNegativeRelativeBaselinePreset ignoreNegativeBaselineMsg;
	bool ignoreNegativeBaseline = false;

	if (mData == NULL)
		return 0;

	RGDList tempList;
	RGDListIterator it (CompleteCurveList);
	DataSignal* nextSignal;
	double previousBP;
	double currentBP;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->GetMean () < startTime)
			continue;

		tempList.Append (nextSignal);
	}

//	cout << "Start time = " << startTime << endl;
	int numSamples = mData->GetNumberOfSamples ();

	RGDListIterator itt (tempList);
	previousBP = -3.0;

	while (nextSignal = (DataSignal*) itt ()) {

		currentBP = nextSignal->GetApproximateBioID ();

		if (CurveIsBaselineFit (nextSignal)) {

			if (currentBP - previousBP < 1.5) {

				previousBP = currentBP;
				continue;
			}

			itt.RemoveCurrentItem ();
			previousBP = currentBP;
		}
	}

	EditOutFitBaselineForNormalization (tempList, mData);	// Added 4:53 pm 08/06/2013
	DataSignal* FitCurve = new CompositeCurve (mData->LeftEndPoint (), mData->RightEndPoint (), tempList);
	FitCurve->ProjectNeighboringSignals (1.0, 1.0);
	DataSignal* FitData = FitCurve->Digitize (numSamples, mData->LeftEndPoint (), 1.0);
	tempList.Clear ();

	//cout << "Making list of neg peaks for analyzing neg baseline (norm)" << endl;
	RGDListIterator itneg (mNegativeCurveList);
	previousBP = -3.0;

	while (nextSignal = (DataSignal*) itneg ()) {

		if (nextSignal->GetMean () < startTime)
			continue;

		tempList.Append (nextSignal);
	}

	itt.Reset ();
	previousBP = -3.0;
	//cout << "Removing neg skewed neg peaks (norm)" << endl;

	while (nextSignal = (DataSignal*) itt ()) {

		currentBP = nextSignal->GetApproximateBioID ();

		if (CurveIsBaselineFit (nextSignal)) {

			if (currentBP - previousBP < 1.5) {

				previousBP = currentBP;
				continue;
			}

			itt.RemoveCurrentItem ();
			previousBP = currentBP;
		}
	}

	//cout << "Forming neg composite curve (norm)" << endl;
	DataSignal* FitNegCurve = new CompositeCurve (mData->LeftEndPoint (), mData->RightEndPoint (), tempList);
	FitNegCurve->ProjectNeighboringSignals (1.0, 1.0);
	//cout << "Digitizing neg peaks (norm)" << endl;
	DataSignal* FitNegData = FitNegCurve->Digitize (numSamples, mData->LeftEndPoint (), 1.0);
	delete FitNegCurve;
	delete FitCurve;
	tempList.Clear ();

	int left = startTime;
	int end = (int)floor (mData->RightEndPoint ());
	int localLeft;
	int localRight;
	smBaselineEstimationThreshold baselineThreshold;
	double rfuThreshold;		// = 0.1 * STRSampleChannelData::GetMinRFU ();  // changed 12/31/2013
	rfuThreshold = (double) GetThreshold (baselineThreshold);

	if (rfuThreshold <= 0.0)
		rfuThreshold = 1.0;

	//if (rfuThreshold < 5.0)
	//	rfuThreshold = 5.0;

	list<int> lefts;
	list<int> rights;
//	double mean;
//	int lastLeft = -5;
	//cout << "Finding valid intervals using pos and neg (norm)" << endl;

	while (FindNextFitDataIntervalBelowThreshold (rfuThreshold, left, end, localLeft, localRight, FitData, FitNegData)) {

		//if (localLeft < lastLeft + 2) {

		//	left = localRight + 2;
		//	continue;
		//}
		
		lefts.push_back (localLeft);
		rights.push_back (localRight);
		left = localRight + 2;
//		lastLeft = localLeft;
	}

	list<double> knotTimes;
	list<double> knotValues;
	list<bool> firstList;
	list<bool> lastList;
	double temp;
	bool firstInterval = true;
	//cout << "Done finding valid intervals (norm)" << endl;

	//if (ChannelData::GetUseNormalizationFilter ()) {

	//	cout << "Adding knots assuming filter has been run" << endl;

	//	while (!lefts.empty ()) {

	//		localLeft = lefts.front ();
	//		localRight = rights.front ();
	//		lefts.pop_front ();
	//		rights.pop_front ();
	//		AppendKnotDataWithEditingToListsAfterFiltering (localLeft, localRight, knotTimes, knotValues, firstList, lastList, firstInterval, mData);
	////		AppendKnotDataToLists (localLeft, localRight, knotTimes, knotValues, mData);
	//		firstInterval = false;
	//	}
	//}

	//else {

	while (!lefts.empty ()) {

		localLeft = lefts.front ();
		localRight = rights.front ();
		lefts.pop_front ();
		rights.pop_front ();
		//AppendKnotDataWithEditingToListsAfterFiltering (localLeft, localRight, knotTimes, knotValues, firstList, lastList, firstInterval, mData);
		AppendKnotDataToLists (localLeft, localRight, knotTimes, knotValues, mData);
		firstInterval = false;
	}
	//}

	temp = mData->RightEndPoint ();
	knotTimes.push_back (temp + 100.0);
	knotTimes.push_back (temp + 200.0);
	knotTimes.push_back (temp + 300.0);
	knotValues.push_back (0.0);
	knotValues.push_back (0.0);
	knotValues.push_back (0.0);
	firstList.push_back (true);
	firstList.push_back (false);
	firstList.push_back (false);
	lastList.push_back (false);
	lastList.push_back (false);
	lastList.push_back (true);

	//temp = knotTimes.front ();

	//if (temp > startTime + 15) {

	//	temp = BaselineAverage (startTime - 7, startTime + 7, mData, 15.0);
	//	knotValues.push_front (temp);
	//	knotTimes.push_front ((double)startTime);
	//}

	if (!knotValues.empty ())
		temp = knotValues.front ();

	else {

		temp = 0.0;
		cout << "No knot values..." << endl;
	}

//	cout << "First calculated knot height = " << temp << ".  Start Time = " << startTime << endl;

	knotTimes.push_front ((double)(startTime / 2));
	knotValues.push_front (temp);
	knotTimes.push_front (0.0);
	knotValues.push_front (0.0);
	firstList.push_front (false);
	lastList.push_front (true);

	knotTimes.push_front (-100.0);
	knotTimes.push_front (-200.0);
	knotTimes.push_front (-300.0);
	knotValues.push_front (0.0);
	knotValues.push_front (0.0);
	knotValues.push_front (0.0);
	firstList.push_front (false);
	firstList.push_front (false);
	firstList.push_front (true);
	lastList.push_front (false);
	lastList.push_front (false);
	lastList.push_front (false);

	list<double>::iterator itTimes;
	list<double>::iterator itValues;

	//cout << "Knot Times";
	//int n = 0;
	double lastTime = -500.0;
	list<double> knotTimes2;
	list<double> knotValues2;
	double tempValue;

	if (STRSampleChannelData::UseOldBaselineEstimation)
		ShapeBaselineData (knotTimes, knotValues);  // commented and added below 08/26/2013

	else {

		ShapeBaselineData (knotTimes, knotValues, FitData, FitNegData, rfuThreshold);	 // Commented out 02/03/2014
		//ShapeBaselineData (knotTimes, knotValues);
		EditPeaksForOutOfRange (knotTimes, knotValues, FitNegData, rfuThreshold);	// 02/03/2014:  This function causes a crash under some conditions...Fixed 02/04/2014
	}

	while (!knotTimes.empty ()) {

		temp = knotTimes.front ();
		tempValue = knotValues.front ();
		knotTimes.pop_front ();
		knotValues.pop_front ();

		if (temp > lastTime + 2.0) {

			knotTimes2.push_back (temp);
			knotValues2.push_back (tempValue);
			lastTime = temp;
		}
	}

	//for (itTimes=knotTimes2.begin (); itTimes!=knotTimes2.end (); itTimes++) {

	//	temp = *itTimes;

	//	if (n == 0)
	//		cout << ":  ";

	//	else
	//		cout << ", ";

	//	cout << temp;
	//	n++;
	//}

	//cout << endl << endl;

	//cout << "Knot Values";
	//n = 0;

	//for (itValues=knotValues2.begin (); itValues!=knotValues2.end (); itValues++) {

	//	temp = *itValues;

	//	if (n == 0)
	//		cout << ":  ";

	//	else
	//		cout << ", ";

	//	cout << temp;
	//	n++;
	//}

	//cout << endl << endl;

	//if (startTime > 100) {

	//	knotTimes.push_front ((double)(startTime / 2));
	//	knotValues.push_front (temp);
	//	knotTimes.push_front (0.0);
	//	knotValues.push_front (0.0);
	//}

	//else if (startTime > 50) {

	//	knotTimes.push_front (0.0);
	//	knotValues.push_front (temp);
	//}

	delete FitData;
	delete FitNegData;
	mBaseLine = new CSplineTransform (knotTimes2, knotValues2);
	knotTimes2.clear ();
	knotValues2.clear ();
	firstList.clear ();
	lastList.clear ();

	if (!mBaseLine->IsValid ()) {

		delete mBaseLine;
		mBaseLine = NULL;
		cout << "Baseline analysis failed..." << endl;
		return 0;
	}

//	cout << "Baseline analysis succeeded..." << endl;
	mBaselineStart = 0;

	if (GetMessageValue (ignoreNegativeBaselineMsg))
		ignoreNegativeBaseline = true;

	double dynamicBaseline;

	//
	// Insert restoration of raw data here and delete filtered data
	//

	//cout << "Restoring raw data for channel " << mChannel << endl;

	if (HasFilteredData ())
		RestoreRawDataAndDeleteFilteredSignal ();

	//cout << "Raw data restored for channel " << mChannel << endl;
	double* sampleData = (double*)mData->GetData ();
	int i;

	//for (i=startTime-1; i>=0; --i) {

	//	temp *= 0.97;
	//	dynamicBaseline = temp;

	//	if ((dynamicBaseline <= 0.0) && ignoreNegativeBaseline)
	//		continue;

	//	sampleData [i] -= dynamicBaseline;
	//}

	dynamicBaseline = mBaseLine->EvaluateSequenceStart (0.0, 1.0);
	bool useNegativeBaseline = !ignoreNegativeBaseline;

	for (i=0; i<numSamples; ++i) {

		if ((dynamicBaseline > 0.0) || useNegativeBaseline)
			sampleData [i] -= dynamicBaseline;

		dynamicBaseline = mBaseLine->EvaluateSequenceNext ();
	}

	return 1;
}


int STRSampleChannelData :: ShapeBaselineData (list<double>& knotTimes, list<double>& knotValues) {

	int n = knotTimes.size ();
	double* originalTimes = new double [n];
	double* originalValues = new double [n];
	int i = 0;
	double time;
	double value;
	double delt;
	double delv;
	double slope;
	double factor = 0.75;
	double timeThreshold = 300.0;
	double increment = 20.0;

	while (!knotTimes.empty ()) {

		time = knotTimes.front ();
		value = knotValues.front ();
		knotTimes.pop_front ();
		knotValues.pop_front ();
		originalTimes [i] = time;
		originalValues [i] = value;
		i++;
	}

	for (i=0; i<6; i++) {

		if (i >= n)
			break;

		knotTimes.push_back (originalTimes [i]);
		knotValues.push_back (originalValues [i]);
	}

	for (i=6; i<n; i++) {

		delt = originalTimes [i] - originalTimes [i - 1];

		if (delt > timeThreshold) {

			// pad knot i - 1 and knot i
			
			time = originalTimes [i - 1] + increment;
			delv = originalValues [i] - originalValues [i - 1];
			slope = delv / delt;
			value = originalValues [i - 1] + factor * increment * slope;
			knotTimes.push_back (time);
			knotValues.push_back (value);

			time = originalTimes [i] - increment;
			value = originalValues [i] - factor * increment * slope;
			knotTimes.push_back (time);
			knotValues.push_back (value);
		}

		knotTimes.push_back (originalTimes [i]);
		knotValues.push_back (originalValues [i]);
	}

	//cout << "Added " << knotTimes.size () - n << " points to baseline estimate" << endl;
	delete [] originalTimes;
	delete [] originalValues;
	return 0;
}


int STRSampleChannelData :: ShapeBaselineData (list<double>& knotTimes, list<double>& knotValues, DataSignal* fitDataPositive, DataSignal* fitDataNegative, double threshold) {

	int n = knotTimes.size ();
	double* originalTimes = new double [n];
	double* originalValues = new double [n];
	int i = 0;
	double time;
	double value;
	double delt;
	double delv;
	double slope;
	double factor = 1.0;	//0.75;
	double timeThreshold = 300.0;
	double increment = 20.0;
	double lowTimeThreshold = 60.0;

	if (ChannelData::GetUseNormalizationFilter ()) {

		timeThreshold = 100.0;
		lowTimeThreshold = 40.0;
	}

	while (!knotTimes.empty ()) {

		time = knotTimes.front ();
		value = knotValues.front ();
		knotTimes.pop_front ();
		knotValues.pop_front ();
		originalTimes [i] = time;
		originalValues [i] = value;
		i++;
	}

	for (i=0; i<6; i++) {

		if (i >= n)
			break;

		knotTimes.push_back (originalTimes [i]);
		knotValues.push_back (originalValues [i]);
	}

//	int first;	// removed 02/07/2014 beause not used anymore:  AddRelativeMinima was removed some time ago
//	int last;
	double centerDel3;

	for (i=6; i<n; i++) {

		delt = originalTimes [i] - originalTimes [i - 1];

		if (delt > lowTimeThreshold) {

			if (delt > timeThreshold) {

				// pad knot i - 1 and knot i
				
				time = originalTimes [i - 1] + increment;
				delv = originalValues [i] - originalValues [i - 1];
				slope = delv / delt;
				value = originalValues [i - 1] + factor * increment * slope;
				knotTimes.push_back (time);
				knotValues.push_back (value);
		//		first = (int) floor (time + 0.5);
		//		last = (int) floor (time);

				//
				//	Add knots 1/3 and 2/3 of the way in between
				//

				centerDel3 = (delv - 2.0 * increment) / 3.0;
				time += centerDel3 - 5.0;
				value += (centerDel3 - 5.0) * slope;
				knotTimes.push_back (time);
				knotValues.push_back (value);
				time += 10.0;
				value += 10.0 * slope;
				knotTimes.push_back (time);
				knotValues.push_back (value);

				time += centerDel3 - 10.0;
				value += (centerDel3 - 10.0) * slope;
				knotTimes.push_back (time);
				knotValues.push_back (value);
				time += 10.0;
				value += 10.0 * slope;
				knotTimes.push_back (time);
				knotValues.push_back (value);

			//	AddRelativeMinima (first, last, knotTimes, knotValues, fitDataPositive, fitDataNegative, threshold);
				time = originalTimes [i] - increment;
				value = originalValues [i] - factor * increment * slope;
				knotTimes.push_back (time);
				knotValues.push_back (value);
			}

			//else {

			//	first = (int) floor (originalTimes [i - 1] + 0.5);
			//	last = (int) floor (originalTimes [i]);
			////	AddRelativeMinima (first, last, knotTimes, knotValues, fitDataPositive, fitDataNegative, threshold);
			//}
		}

		knotTimes.push_back (originalTimes [i]);
		knotValues.push_back (originalValues [i]);
	}

	//cout << "Added " << knotTimes.size () - n << " points to baseline estimate" << endl;
	delete [] originalTimes;
	delete [] originalValues;
	return 0;
}



//***************************************************************************************************************************************************


bool STRSampleChannelData :: FindNextFitDataIntervalBelowThreshold (double threshold, int start, int end, int& beginInterval, int& endInterval, DataSignal* fitData) {

	//
	//	beginInterval and endInterval are inclusive; finds interval of length at least 20
	//

	if (start > end - 19)
		return false;

	int i;
	double value;
	bool intervalStarted = false;
	int intervalLength = 0;
	int localBegin = start;

	for (i=start; i<=end; ++i) {

		value = fitData->ValueFreeBound (i);

		if (fabs (value) <= threshold) {

			if (intervalStarted) {

				++intervalLength;
			}

			else {

				localBegin = i;
				intervalStarted = true;
				intervalLength = 1;
			}
		}

		else {

			if (intervalStarted) {

				if (intervalLength >= 15) {

					// we're done
					beginInterval = localBegin;
					endInterval = localBegin + intervalLength - 1;
					return true;
				}

				else {

					localBegin = i + 1;
					intervalStarted = false;
					intervalLength = 0;
				}
			}
		}
	}

	if (intervalStarted && (intervalLength >= 15)) {

		beginInterval = localBegin;
		endInterval = end;
		return true;
	}

	return false;
}


bool STRSampleChannelData :: FindNextFitDataIntervalBelowThreshold (double threshold, int start, int end, int& beginInterval, int& endInterval, DataSignal* fitData, DataSignal* fitNegData) {

	//
	//	beginInterval and endInterval are inclusive; finds interval of length at least 20
	//

	if (start > end - 19)
		return false;

	int i;
	double value;
	double negValue;
	bool intervalStarted = false;
	int intervalLength = 0;
	int localBegin = start;

	for (i=start; i<=end; ++i) {

		value = fitData->ValueFreeBound (i);
		negValue = fitNegData->ValueFreeBound (i);

		if ((fabs (value) <= threshold) && (fabs (negValue) <= threshold)) {

			if (intervalStarted) {

				++intervalLength;
			}

			else {

				localBegin = i;
				intervalStarted = true;
				intervalLength = 1;
			}
		}

		else {

			if (intervalStarted) {

				if (intervalLength >= 15) {

					// we're done
					beginInterval = localBegin;
					endInterval = localBegin + intervalLength - 1;
					return true;
				}

				else {

					localBegin = i + 1;
					intervalStarted = false;
					intervalLength = 0;
				}
			}
		}
	}

	if (intervalStarted && (intervalLength >= 15)) {

		beginInterval = localBegin;
		endInterval = end;
		return true;
	}

	return false;
}


void STRSampleChannelData :: AppendKnotDataToLists (int intervalLeft, int intervalRight, list<double>& times, list<double>& values, DataSignal* fitData) {

	int length = intervalRight - intervalLeft + 1;
	int center;
	int left;
	int right;
	double time;
	double value;
	int i;
	int nSeg;
	int segLength;

	if (length < 30) {

		center = (intervalRight + intervalLeft) / 2;
		left = center - 7;
		right = center + 7;
		value = BaselineAverage (left, right, fitData, 15.0);
		time = (double) center;
		times.push_back (time);
		values.push_back (value);
		return;
	}

	left = intervalLeft;
	right = intervalLeft + 14;
	time = (double) (intervalLeft + 7);
	value = BaselineAverage (left, right, fitData, 15.0);
	times.push_back (time);
	values.push_back (value);

	if (length > 75) {

		nSeg = length / 75;
		segLength = length / (nSeg + 1);
		center = left;

		for (i=0; i<nSeg; i++) {

			center += segLength;
			left = center - 7;
			right = center + 7;
			value = BaselineAverage (left, right, fitData, 15.0);
			time = (double) center;
			times.push_back (time);
			values.push_back (value);
		}

		//center = (intervalRight + intervalLeft) / 2;
		//left = center - 25;
		//right = center + 25;
		//value = BaselineAverage (left, right, fitData, 51.0);
		//time = (double) center;
		//times.push_back (time);
		//values.push_back (value);
	}

	right = intervalRight;
	left = intervalRight - 14;
	time = (double) (left + 7);
	value = BaselineAverage (left, right, fitData, 15.0);
	times.push_back (time);
	values.push_back (value);
}


void STRSampleChannelData :: AppendKnotDataWithEditingToLists (int intervalLeft, int intervalRight, list<double>& times, list<double>& values, list<bool>& firsts, list<bool>& lasts, bool isFirstInterval, DataSignal* rawData) {

	int length = intervalRight - intervalLeft + 1;
	int center;
	int left;
	int right;
	double time;
	double value;
	int i;
	int nSeg;
	int segLength;
	bool hasFirstKnot = false;
//	double lastTime;

	//if (times.empty ())
	//	lastTime = -5.0;

	//else
	//	lastTime = *times.end () + 1.0;

	if (length < 51) {

		if (isFirstInterval) {

			center = (intervalRight + intervalLeft) / 2;
			left = center - 12;
			right = center + 2;
			value = BaselineAverage (left, right, rawData, 15.0);
			time = (double) (center - 5);
			times.push_back (time);
			values.push_back (value);
			firsts.push_back (true);
			lasts.push_back (false);

			left = center - 2;
			right = center + 12;
			value = BaselineAverage (left, right, rawData, 15.0);
			time = (double) (center + 5);
			times.push_back (time);
			values.push_back (value);
			firsts.push_back (false);
			lasts.push_back (true);
		}

		return;
	}

	left = intervalLeft;
	right = intervalLeft + 50;

	if (isFirstInterval) {

		time = (double) (intervalLeft + 15);
		right = intervalLeft + 30;
		value = BaselineAverage (left, right, rawData, 31.0);
		times.push_back (time);
		values.push_back (value);
		firsts.push_back (true);
		lasts.push_back (false);
		hasFirstKnot = true;

		if (length <= 150) {

			time = (double) (intervalRight - 15);
			right = intervalRight;
			left = intervalRight - 30;
			value = BaselineAverage (left, right, rawData, 31.0);
			times.push_back (time);
			values.push_back (value);
			firsts.push_back (false);
			lasts.push_back (false);
			return;
		}
	}

	else { //if (!STRSampleChannelData::UseOldBaselineEstimation) {

		time = (double) (intervalLeft + 15);		// 02/03/2014:  This body of code can cause baseline to go awry under some conditions...
		right = intervalLeft + 30;
		value = BaselineAverage (left, right, rawData, 31.0);
		times.push_back (time);
		values.push_back (value);
		firsts.push_back (true);
		lasts.push_back (false);
		hasFirstKnot = true;

		if ((mChannel == 1) && (intervalLeft > 1600) && (intervalRight < 2700)) {

			cout << "Left interval = " << intervalLeft << "; Right interval = " << intervalRight;
			cout << "; Sample time = " << time << "; Sample Value = " << value << endl;
		}
	}

	if (length > 150) {

		nSeg = (length / 150) + 1;
		segLength = length / (nSeg + 1);

		left = intervalLeft;
		right = intervalLeft + 50;
		time = (double) (left + 25);
		value = BaselineAverage (left, right, rawData, 51.0);
		times.push_back (time);
		values.push_back (value);
		firsts.push_back (true);
		lasts.push_back (false);
			
		center = left;

		for (i=0; i<nSeg; i++) {

			center += segLength;
			left = center - 25;
			right = center + 25;
			value = BaselineAverage (left, right, rawData, 51.0);
			time = (double) center;

			//if (time <= lastTime)
			//	continue;

			times.push_back (time);
			values.push_back (value);
	//		lastTime = time + 1.0;

			if (!hasFirstKnot) {

				firsts.push_back (true);
				hasFirstKnot = true;
			}

			else
				firsts.push_back (false);

			lasts.push_back (false);
		}
	}

	right = intervalRight;
	left = intervalRight - 50;
	time = (double) (left + 25);
	value = BaselineAverage (left, right, rawData, 51.0);

	times.push_back (time);
	values.push_back (value);

	if (hasFirstKnot)
		firsts.push_back (false);

	else
		firsts.push_back (true);

	lasts.push_back (true);
}


void STRSampleChannelData :: AppendKnotDataWithEditingToListsAfterFiltering (int intervalLeft, int intervalRight, list<double>& times, list<double>& values, list<bool>& firsts, list<bool>& lasts, bool isFirstInterval, DataSignal* rawData) {

	//  no longer in use...02/04/2014

	int length = intervalRight - intervalLeft + 1;
	int center;
	int left;
	int right;
	double time;
	double value;
	int i;
	int nSeg;
	int segLength;
	bool hasFirstKnot = false;

	if (mChannel == 1)
		cout << "Left interval = " << intervalLeft << ", Right interval = " << intervalRight << endl;

	if (length < 20) {

		// Only enough space for one sample

		center = (intervalRight + intervalLeft) / 2;
		left = intervalLeft;
		right = intervalRight;
		value = BaselineAverage (left, right, rawData, intervalRight - intervalLeft + 1);
		time = (double) center;
		times.push_back (time);
		values.push_back (value);
		firsts.push_back (true);
		lasts.push_back (false);
		return;
	}

	else if (length < 40) {

		center = (intervalRight + intervalLeft) / 2;
		left = center - 12;
		right = center + 2;
		value = BaselineAverage (left, right, rawData, 15.0);
		time = (double) (center - 5);
		times.push_back (time);
		values.push_back (value);
		firsts.push_back (true);
		lasts.push_back (false);

		left = center - 2;
		right = center + 12;
		value = BaselineAverage (left, right, rawData, 15.0);
		time = (double) (center + 5);
		times.push_back (time);
		values.push_back (value);
		firsts.push_back (false);
		lasts.push_back (true);
		return;
	}

	else if (length < 75) {

		time = (double) (intervalLeft + 12);
		left = intervalLeft;
		right = intervalLeft + 24;
		value = BaselineAverage (left, right, rawData, 25.0);
		times.push_back (time);
		values.push_back (value);
		firsts.push_back (true);
		lasts.push_back (false);
		hasFirstKnot = true;

		center = (intervalRight + intervalLeft) / 2;
		left = center - 12;
		right = center + 12;
		value = BaselineAverage (left, right, rawData, 25.0);
		time = (double) center;
		times.push_back (time);
		values.push_back (value);
		firsts.push_back (true);
		lasts.push_back (false);

		time = (double) (intervalRight - 12);
		right = intervalRight;
		left = intervalRight - 24;
		value = BaselineAverage (left, right, rawData, 25.0);
		times.push_back (time);
		values.push_back (value);
		firsts.push_back (false);
		lasts.push_back (false);
		return;
	}

	left = intervalLeft;
	right = intervalLeft + 50;

	nSeg = (length / 75) + 1;
	segLength = length / (nSeg + 1);

	left = intervalLeft;
	right = intervalLeft + 24;
	time = (double) (left + 12);
	value = BaselineAverage (left, right, rawData, 25.0);
	times.push_back (time);
	values.push_back (value);
	firsts.push_back (true);
	lasts.push_back (false);
			
	center = left;

	for (i=0; i<nSeg; i++) {

		center += segLength;
		left = center - 12;
		right = center + 12;
		value = BaselineAverage (left, right, rawData, 25.0);
		time = (double) center;

		times.push_back (time);
		values.push_back (value);

		if (!hasFirstKnot) {

			firsts.push_back (true);
			hasFirstKnot = true;
		}

		else
			firsts.push_back (false);

		lasts.push_back (false);
	}

	right = intervalRight;
	left = intervalRight - 24;
	time = (double) (left + 12);
	value = BaselineAverage (left, right, rawData, 25.0);

	times.push_back (time);
	values.push_back (value);

	if (hasFirstKnot)
		firsts.push_back (false);

	else
		firsts.push_back (true);

	lasts.push_back (true);
}


double STRSampleChannelData :: BaselineAverage (int left, int right, DataSignal* fitData, double denominator) {

	double sum = 0.0;
	int i;

	for (i=left; i<=right; ++i)
		sum += fitData->ValueFreeBound (i);

	return sum / denominator;
}


double STRSampleChannelData :: BaselineMax (int left, int right, DataSignal* fitData) {

	double Max = 0.0;
	double temp;
	int i;

	for (i=left; i<=right; i++) {

		temp = fitData->ValueFreeBound (i);

		if (temp > Max)
			Max = temp;
	}

	return Max;
}


int STRSampleChannelData :: EditOutFitBaselineForNormalization (RGDList& fitPeaks, DataSignal* rawData) {

	RGDListIterator it (fitPeaks);
	DataSignal* nextSignal;
	double detectionThreshold = 0.6 * STRSampleChannelData::sampleDetectionThreshold;
	double peakHeight;
	//double rawAverage;
	//double rawMinimum;
	double leftValue;
	double rightValue;
	double left;
	double right;

	while (nextSignal = (DataSignal*) it ()) {

		left = nextSignal->LeftEndPoint ();
		right = nextSignal->RightEndPoint ();
		peakHeight = nextSignal->Peak ();
		leftValue = nextSignal->Value (left);
		rightValue = nextSignal->Value (right);

		if ((peakHeight - leftValue < detectionThreshold) && (peakHeight - rightValue < detectionThreshold))
			it.RemoveCurrentItem ();
	}

	return 0;

//	int nPeaks = 7;
//
//	if (fitPeaks.Entries () < nPeaks) {
//
//		//cout << "Too few peaks" << endl;
//		return 0;
//	}
//
//	RGDListIterator itStart (fitPeaks);
//	RGDListIterator itEnd (fitPeaks);
//	DataSignal* firstSignal;
//	DataSignal* lastSignal;
//	DataSignal* nextSignal;
//	double meanFirst;
//	double meanLast;
//	int i;
//	double length = 150.1;
//	RGDList temp;
//	int rawStart;
//	int rawEnd;
//	int n = rawData->GetNumberOfSamples ();
//	double detectionThreshold = STRSampleChannelData::sampleDetectionThreshold;
//	double averageRawData;
//	double maxRawData;
//	double denominator;
//	int k = 0;
//
////	itStart ();
//
//	for (i=1; i<nPeaks; i++)
//		itEnd ();
//
//	while (true) {
//
//		firstSignal = (DataSignal*) itStart ();
//		lastSignal = (DataSignal*) itEnd ();
//		k++;
//
//		//if (k > 15)
//		//	break;
//
//		//cout << "Iteration " << k << endl;
//
//		if (firstSignal == NULL) {
//
//			//cout << "First signal null" << endl;
//			break;
//		}
//
//		if (lastSignal == NULL) {
//
//			//cout << "Last signal null:  copy and return" << endl;
//
//			while (firstSignal != NULL) {
//
//				temp.Append (firstSignal);
//				firstSignal = (DataSignal*) itStart ();
//			}
//
//			break;
//		}
//
//		meanFirst = firstSignal->GetMean ();
//		meanLast = lastSignal->GetMean ();
//		//cout << "Means:  first = " << meanFirst << "; last = " << meanLast << endl;
//
//		if (meanLast - meanFirst > length) {
//
//			for (i=1; i<nPeaks; i++) {
//
//				temp.Append (firstSignal);
//				firstSignal = (DataSignal*) itStart ();
//				lastSignal = (DataSignal*) itEnd ();
//
//				if (firstSignal == NULL)
//					break;
//			}
//
//			if (firstSignal != NULL)
//				temp.Append (firstSignal);
//
//			//cout << "Bump nPeaks" << endl;
//			continue;
//		}
//
//		rawStart = (int) floor (meanFirst);
//		rawEnd = (int) floor (meanLast + 1.0);
//
//		if (rawStart < 0)
//			rawStart = 0;
//
//		if (rawEnd >= n)
//			rawEnd = n - 1;
//
//		//cout << "Raw start:  " << rawStart << "; raw end:  " << rawEnd << endl;
//		denominator = (double) (rawEnd - rawStart + 1);
//		averageRawData = BaselineAverage (rawStart, rawEnd, rawData, denominator);
//		maxRawData = BaselineMax (rawStart, rawEnd, rawData);
//		//cout << "Ave:  " << averageRawData << "; Max:  " << maxRawData << endl;
//
//		if (maxRawData - averageRawData >= detectionThreshold) {
//
//			for (i=1; i<nPeaks; i++) {
//
//				temp.Append (firstSignal);
//				firstSignal = (DataSignal*) itStart ();
//				lastSignal = (DataSignal*) itEnd ();
//
//				if (firstSignal == NULL)
//					break;
//			}
//
//			if (firstSignal != NULL)
//				temp.Append (firstSignal);
//
//			//cout << "Max peak high:  append and bump nPeaks" << endl;
//			continue;
//		}
//
//		//cout << "Skip peaks and start from there" << endl;
//
//		for (i=1; i<=nPeaks; i++) {
//
//			itStart ();
//			itEnd ();
//		}
//	}
//
//	//return 0;
//
//	if (temp.Entries () == 0)
//		return 0;
//
//	if (temp.Entries () == fitPeaks.Entries ()) {
//
//		temp.Clear ();
//		return 0;
//	}
//
//	fitPeaks.Clear ();
//
//	while (nextSignal = (DataSignal*) temp.GetFirst ())
//		fitPeaks.Append (nextSignal);

//	return 0;
}


int STRSampleChannelData :: AddRelativeMinima (int intervalStart, int intervalEnd, list<double>& times, list<double>& values, DataSignal* fitDataPositive, DataSignal* fitDataNegative, double threshold) {

	if (intervalStart < 0)
		return 0;

	if (intervalEnd > mData->GetNumberOfSamples ())
		return 0;
	
	int i;
	list<int> minTimes;
	list<double> minValues;
	double previous;
	double next = -1.0;
	double current;
	previous = fitDataPositive->ValueFreeBound (intervalStart + 2);
	current = fitDataPositive->ValueFreeBound (intervalStart + 3);
	int last = intervalEnd - 2;
	double absoluteMin = 50.0 * threshold;
	double factor = 1.0;
	double currentRawValue;

	for (i=intervalStart+4; i<=last; i++) {

		next = fitDataPositive->ValueFreeBound (i);

		if ((current < next) && (current < previous)) {

			if (current >= threshold) {

				minTimes.push_back (i - 1);
				currentRawValue = BaselineAverage (i - 6, i + 4, mData, 11.0);
			//	currentRawValue = mData->ValueFreeBound (i - 1);
				minValues.push_back (currentRawValue);

				if (currentRawValue < absoluteMin)
					absoluteMin = currentRawValue;
			}
		}

		previous = current;
		current = next;
	}

	if ((!minTimes.empty ()) && (minTimes.size () < 4)) {

		minTimes.clear ();
		minValues.clear ();
	}

	if (minTimes.empty ()) {

		factor = -1.0;
		previous = fitDataNegative->ValueFreeBound (intervalStart + 2);
		current = fitDataNegative->ValueFreeBound (intervalStart + 3);
		absoluteMin = 50.0 * threshold;

		for (i=intervalStart+4; i<=last; i++) {

			next = fitDataNegative->ValueFreeBound (i);

			if ((current < next) && (current < previous)) {

				if (current >= threshold) {

					minTimes.push_back (i - 1);
					currentRawValue = BaselineAverage (i - 6, i + 4, mData, 11.0);
				//	currentRawValue = mData->ValueFreeBound (i - 1);
					minValues.push_back (currentRawValue);

					if (currentRawValue < absoluteMin)
						absoluteMin = currentRawValue;
				}
			}

			previous = current;
			current = next;
		}
	}

	if (minTimes.empty ())
		return 0;

	if (minTimes.size () < 4) {

		minTimes.clear ();
		minValues.clear ();
		return 0;
	}

	i = minTimes.front ();

	if (i - intervalStart < 10) {

		minTimes.pop_front ();
		minValues.pop_front ();
	}

	if (minTimes.empty ())
		return 0;

	i = minTimes.back ();

	if (intervalEnd - i < 10) {

		minTimes.pop_back ();
		minValues.pop_back ();
	}

	if (minTimes.empty ())
		return 0;

	if (minTimes.size () < 4) {

		minTimes.clear ();
		minValues.clear ();
		return 0;
	}

	while (!minTimes.empty ()) {

		i = minTimes.front ();
		current = minValues.front ();
		minTimes.pop_front ();
		minValues.pop_front ();

		times.push_back ((double)i);
		values.push_back (factor * current);

		if (minTimes.empty ())
			return 0;

		minTimes.pop_front ();
		minValues.pop_front ();
	}

	return 0;
}


int STRSampleChannelData :: EditPeaksForOutOfRange (list<double>& times, list<double>& values, DataSignal* fitDataNegative, double threshold) {

	// Tested corrections/deletions are questionable because of limits...use max slope instead?
	int n = times.size ();

	if (n < 8)
		return 0;

	double* originalTimes = new double [n];
	double* originalValues = new double [n];
	int i = 0;
	double time;
	double value;
	double leftMin;
	double rightMin;
	int n1 = n - 1;
	int currentTime;
	double averageMin;
	double maxOfMins;
	double minOfMins;
	double delta;
	double upperTest;
	double lowerTest;
	double testFactor = 2.5;
	double endTime = mData->RightEndPoint ();

	while (!times.empty ()) {

		time = times.front ();
		value = values.front ();
		times.pop_front ();
		values.pop_front ();
		originalTimes [i] = time;
		originalValues [i] = value;
		i++;
	}

	for (i=0; i<6; i++) {

		times.push_back (originalTimes [i]);
		values.push_back (originalValues [i]);
	}

	double prevValue = originalValues [5];
	double nextValue = originalValues [7];
	double currentValue;

	for (i=6; i<n1; i++) {

		currentValue = originalValues [i];
		currentTime = (int) floor (originalTimes [i] + 0.5);

		if (currentTime < endTime) {

			ScanRawDataForMinimaLeftAndRight (currentTime, leftMin, rightMin, fitDataNegative, threshold);
			averageMin = 0.5 * (leftMin + rightMin);
			maxOfMins = leftMin;
			minOfMins = leftMin;

			if (maxOfMins < rightMin)
				maxOfMins = rightMin;

			if (minOfMins > rightMin)
				minOfMins = rightMin;

			if (maxOfMins < 0.0) {

				if (maxOfMins > -10.0)
					delta = 10.0;

				else
					delta = -maxOfMins;
			}

			else if (maxOfMins < 10.0)
				delta = 10.0;

			else
				delta = maxOfMins;

			upperTest = maxOfMins + testFactor * delta;
			lowerTest = minOfMins - testFactor * delta;

			if ((currentValue <= upperTest) && (currentValue >= lowerTest)) {

				times.push_back (originalTimes [i]);
				values.push_back (originalValues [i]);
			}
		}

		else {

			times.push_back (originalTimes [i]);
			values.push_back (originalValues [i]);
		}
	}

	times.push_back (originalTimes [n1]);
	values.push_back (originalValues [n1]);

	delete[] originalTimes;
	delete[] originalValues;
	return 0;
}



void STRSampleChannelData :: ScanRawDataForMinimaLeftAndRight (int time, double& leftMin, double& rightMin, DataSignal* fitDataNegative, double threshold) {

	double startValue = mData->ValueFreeBound (time);
	leftMin = startValue;
	rightMin = startValue;
	int left = time - 25;
	int right = time;
	int n = 0;

	while (!FindRawDataMinimumOnInterval (leftMin, left, right, fitDataNegative, threshold)) {

		if (n > 3)
			break;

		left -= 25;
		right -= 25;
		n++;
	}

	n = 0;
	left = time;
	right = time + 25;

	while (!FindRawDataMinimumOnInterval (rightMin, left, right, fitDataNegative, threshold)) {

		if (n > 3)
			break;

		left += 25;
		right += 25;
		n++;
	}
}


bool STRSampleChannelData :: FindRawDataMinimumOnInterval (double& minimum, int left, int right, DataSignal* fitDataNegative, double threshold) {

	bool madeComparison = false;
	int i;
	double temp;
	int maxTime = mData->GetNumberOfSamples ();

	if (right < 0)
		return false;

	for (i=left; i<=right; i++) {

		if (i > maxTime)
			break;

		if (i < 0)
			continue;

		if (fitDataNegative->ValueFreeBound (i) < threshold) {

			madeComparison = true;
			temp = mData->ValueFreeBound (i);

			if (temp < minimum)
				minimum = temp;
		}
	}

	return madeComparison;
}


bool STRSampleChannelData :: CurveIsBaselineFit (DataSignal* signal) {

	double secs = ChannelData::GetAveSecondsPerBP ();
	double peak = signal->Peak ();
	double mean = signal->GetMean ();
	double left = signal->Value (mean - secs);
	double right = signal->Value (mean + secs);

	double maxOffPeak = left;

	if (right > maxOffPeak)
		maxOffPeak = right;

	if (maxOffPeak <= 0.0)
		return false;

	if (peak <= 0.0)
		return false;

	if (maxOffPeak / peak < .8825)
		return false;

	return true;
}


