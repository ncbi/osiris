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
*  FileName: STRCoreBioComponentSM.cpp
*  Author:   Robert Goor
*
*/
//
//  class STRCoreBioComponent, a subclass of CoreBioComponent, and parent class to STRLadderBioComponent and STRSampleBioComponent:  Functions with
//  SmartMessages only
//


#include "STRCoreBioComponent.h"
#include "STRChannelData.h"
#include "GenotypeSpecs.h"
#include "DataSignal.h"
#include "RGTextOutput.h"
#include "fsaPeakData.h"
#include "ParameterServer.h"
#include "TracePrequalification.h"
#include "xmlwriter.h"
#include "SmartNotice.h"
#include "STRSmartNotices.h"
#include "DirectoryManager.h"
#include "rgtarray.h"
#include <set>
#include <string>
#include <iostream>

using namespace std;

// Smart Message related*********************************************************************************************************************************


int STRCoreBioComponent :: AnalyzeGridSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	return -10;
}


int STRCoreBioComponent :: AnalyzeCrossChannelSM () {

	// This is to be called after CoreBioComponent::FitAllCharacteristics...and BEFORE AnalyzeLaneStandardChannel
	int size = mNumberOfChannels + 1;
	DataSignal** OnDeck = new DataSignal* [size];
	DataSignal** BracketLeft = new DataSignal* [size];
	DataSignal** BracketRight = new DataSignal* [size];
	double* means = new double [size];
	int currentIndex;
	bool* isDone = new bool [size];
	RGDList OverallList;
	bool NowWeAreDone = false;
	DataSignal* nextSignal;
	DataSignal* nextSignal2;
	double minPeak;
	double maxPeak;
	double minWidth;
	double maxWidth;
	DataSignal* primeSignal;
	int maxIndex;
	double currentPeak;
	double currentWidth;
	double calculatedNormalWidth;
	double mTimeTolerance = 0.95;
	double mWidthMatchFraction = 0.1;  // Double it to get width fractional tolerance, currently 20%
	double mWidthToleranceForSpike = 2.1;  // Width must be less than this width to qualify as a spike
	double nSigmaForCraters = 2.0;		//Test...01/08/2014
	RGString info;
	DataSignal* testSignal;
	DataSignal* testSignal2;
	NowWeAreDone = true;
	double mean1;
	double mean2;
	int channel1;
	int channel2;
	int NPossibleCraters;
	int NCraters;
	int NCraterTests;
	double CraterLeftLimit;
	double CraterRightLimit;
	int LookCount;
	mNumberOfPullups = 0;
	mNumberOfPrimaryPullups = 0;
	int i;
	RGDList** TempList = new RGDList* [mNumberOfChannels + 1];
	DataSignal* prevSignal;
	//double saturationArtifactTolerance;
	//double deltaTime;
	//DataSignal* newCraterSignal;
	ChannelData* nextChannel;
	RGDList* nextList;

	smPullUp pullup;
	smPrimaryInterchannelLink primaryLink;
	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smSpike spike;
	smBelowMinRFU belowMinRFU;
	PullUpFound pullupNotice;
	PullUpPrimaryChannel primaryPullupNotice;

	ParameterServer* pServer = new ParameterServer;
	double measurementResolution = pServer->GetInterSampleSpacing ();
	double minPullupThreshold = 0.5 * measurementResolution;
	delete pServer;

	RGDList NewCraterSignalsToAdd;
	RGDList CraterSignalsToRemove;
	InterchannelLinkage* iChannel;

	//double peak1;
	//double peak2;
	int nInterchannelLinks = 0;

	for (i=1; i<=mNumberOfChannels; i++) {

		TempList [i] = new RGDList;
	}

	for (i=1; i<=mNumberOfChannels; i++)
		mDataChannels [i]->ResetCompleteIterator ();

	// Now copy from mDataChannels PreliminaryCurveList to TempList (while finding maxSignalPeak??)

	for (i=1; i<=mNumberOfChannels; i++) {

		prevSignal = NULL;
		nextChannel = mDataChannels [i];
		nextList = TempList [i];

//		while (nextSignal = nextChannel->GetNextPreliminaryCurve ()) {
		while (nextSignal = nextChannel->GetNextCompleteCurve ()) {

			if (nextSignal->GetMessageValue (belowMinRFU))
				continue;

			nextList->Append (nextSignal);


// Remove from here...

		//	if (prevSignal != NULL) {

		//		// test if prevSignal and nextSignal are "close".  If so, don't move them and create intermediate "crater" peak.  If
		//		// not, store prevSignal and overwrite with nextSignal

		//		saturationArtifactTolerance = 1.1 * (prevSignal->GetStandardDeviation () + nextSignal->GetStandardDeviation ());
		//		//deltaTime = nextSignal->GetMean () - prevSignal->GetMean ();
		//		deltaTime = nextSignal->GetApproximateBioID () - prevSignal->GetApproximateBioID ();
		//		peak1 = prevSignal->Peak ();
		//		peak2 = nextSignal->Peak ();

		//		if ((deltaTime > 0.75) || (peak1 < 0.7 * peak2) || (peak2 < 0.7 * peak1)) {
		//		//if ((deltaTime > saturationArtifactTolerance) || (peak1 < 0.7 * peak2) || (peak2 < 0.7 * peak1)) {

		//			//nextList->Append (prevSignal);
		//			prevSignal = nextSignal;
		//		}

		//		else {

		//			newCraterSignal = new CraterSignal (prevSignal, nextSignal);
		//			newCraterSignal->SetSaturationArtifact (true);
		//			newCraterSignal->SetMessageValue (crater, true);
		//			prevSignal->SetMessageValue (craterSidePeak, true);
		//			nextSignal->SetMessageValue (craterSidePeak, true);

		////			nextChannel->InsertIntoArtifactList (newCraterSignal);	// Need this??????????????????????????????????????????????????
		////			nextChannel->InsertIntoArtifactList (prevSignal);	// Need this??????????????????????????????????????????????????
		////			nextChannel->InsertIntoArtifactList (nextSignal);	// Need this??????????????????????????????????????????????????

		//			NewCraterSignalsToAdd.Append (newCraterSignal);
		//			prevSignal = NULL;
		//		}
		//	}

		//	else
		//		prevSignal = nextSignal;	//  ...to here...when crater formation below is fixed  01/08/2014
		}

		if (prevSignal != NULL)
			nextList->Append (prevSignal);

		while (nextSignal = (DataSignal*) NewCraterSignalsToAdd.GetFirst ()) {

			nextChannel->InsertIntoCompleteCurveList (nextSignal);
			nextChannel->InsertIntoPreliminaryCurveList (nextSignal);
			nextList->Insert (nextSignal);
		}
	}

	RGDListIterator* tempIterator;

	for (i=1; i<=mNumberOfChannels; i++) {

		tempIterator = new RGDListIterator (*TempList [i]);

		while (nextSignal = (DataSignal*) (*tempIterator) ()) {

			if (nextSignal->GetMessageValue (belowMinRFU))
				tempIterator->RemoveCurrentItem ();
		}

		delete tempIterator;
	}

	for (i=1; i<=mNumberOfChannels; i++) {
	
		OnDeck [i] = (DataSignal*) TempList [i]->GetFirst ();  //used to be:  mDataChannels [i]->GetNextPreliminaryCurve ();

		if (OnDeck [i] != NULL) {

			means [i] = OnDeck [i]->GetMean ();
			isDone [i] = false;
			OnDeck [i]->SetChannel (i);
		}

		else {

			means [i] = DOUBLEMAX;
			isDone [i] = true;
		}

		NowWeAreDone = NowWeAreDone && isDone [i];
	}

	if (!NowWeAreDone) {

		while (true) {

			NowWeAreDone = true;
			currentIndex = MinimumIndex (means, mNumberOfChannels);

			OverallList.Append (OnDeck [currentIndex]);
			OnDeck [currentIndex] = (DataSignal*) TempList [currentIndex]->GetFirst ();   //used to be:  mDataChannels [currentIndex]->GetNextPreliminaryCurve ();

			if (OnDeck [currentIndex] == NULL) {

				means [currentIndex] = DOUBLEMAX;
				isDone [currentIndex] = true;

				for (i=1; i<=mNumberOfChannels; i++)
					NowWeAreDone = NowWeAreDone && isDone [i];

				if (NowWeAreDone)
					break;
			}

			else {

				means [currentIndex] = OnDeck [currentIndex]->GetMean ();
				OnDeck [currentIndex]->SetChannel (currentIndex);
			}
		}
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		TempList [i]->Clear ();
		delete TempList [i];
	}

	delete[] TempList;

	RGDListIterator it (OverallList);

	//int kk = 0;
	//
	//while (nextSignal = (DataSignal*) it ()) {

	//	kk++;

	//	if (kk > 100)
	//		break;

	//	cout << "Signal = " << nextSignal->GetMean () << " , channel " << nextSignal->GetChannel () << endl;
	//}

	for (i=1; i<=mNumberOfChannels; i++) {

		BracketLeft [i] = NULL;
		BracketRight [i] = NULL;
	}

	bool debug = false;
	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		debug = false;

		nextSignal2 = (DataSignal*) it ();

		if (nextSignal2 == NULL)
			break;

		if (nextSignal->GetChannel () == nextSignal2->GetChannel ()) {

			--it;
			continue;
		}

		mean1 = nextSignal2->GetApproximateBioID ();
		mean2 = nextSignal->GetApproximateBioID ();

		//mTimeTolerance = nextSignal->GetMeanVariability () * nextSignal->GetStandardDeviation () + 
		//		nextSignal2->GetMeanVariability () * nextSignal2->GetStandardDeviation () + 2.0;

		//if (mTimeTolerance < minPullupThreshold)
		//	mTimeTolerance = minPullupThreshold;

		mTimeTolerance = 0.075;

		if (mean1 - mean2 >= mTimeTolerance) {

			BracketLeft [nextSignal->GetChannel ()] = nextSignal;
			if (debug) cout << "left bracket for channel " << nextSignal->GetChannel () << " has mean " << mean2 << endl;
			--it;
			continue;
		}

		if (BracketLeft [nextSignal->GetChannel ()] != NULL)
			BracketLeft [nextSignal->GetChannel ()] = NULL;

		if (BracketLeft [nextSignal2->GetChannel ()] != NULL)
			BracketLeft [nextSignal2->GetChannel ()] = NULL;

		for (i=1; i<=mNumberOfChannels; i++)
			OnDeck [i] = NULL;

		channel1 = nextSignal->GetChannel ();
		channel2 = nextSignal2->GetChannel ();
		OnDeck [channel1] = nextSignal;
		OnDeck [channel2] = nextSignal2;
		primeSignal = nextSignal2;

		while (true) {

			nextSignal = (DataSignal*) it ();

			if (nextSignal == NULL)
				break;

			if ((nextSignal->GetChannel () == channel1) || (nextSignal->GetChannel () == channel2)) {

				--it;
				break;
			}

			//mTimeTolerance = nextSignal->GetMeanVariability () * nextSignal->GetStandardDeviation () + 
			//	nextSignal2->GetMeanVariability () * nextSignal2->GetStandardDeviation ();

			//if (mTimeTolerance < minPullupThreshold)
			//	mTimeTolerance = minPullupThreshold;

			mTimeTolerance = 0.075;

			if (nextSignal->GetApproximateBioID () - nextSignal2->GetApproximateBioID () >= mTimeTolerance) {

				--it;
				break;
			}

			OnDeck [nextSignal->GetChannel ()] = nextSignal;
			nextSignal2 = nextSignal;
		}

		minPeak = maxPeak = primeSignal->Peak ();
		minWidth = maxWidth = primeSignal->GetStandardDeviation ();
		maxIndex = primeSignal->GetChannel ();
		int j;

		for (j=1; j<=mNumberOfChannels; j++) {
		
			testSignal = OnDeck [j];
			
			if (testSignal != NULL) {

				currentPeak = testSignal->Peak ();
				currentWidth = testSignal->GetStandardDeviation ();
				
				if (currentPeak > maxPeak) {

					maxPeak = currentPeak;
					maxIndex = j;
				}

				else if (currentPeak < minPeak)
					minPeak = currentPeak;

				if (currentWidth > maxWidth)
					maxWidth = currentWidth;

				else if (currentWidth < minWidth)
					minWidth = currentWidth;
			}
		}

		primeSignal = OnDeck [maxIndex]; // This is the primary bleed through or spike, whichever...
		mean1 = primeSignal->GetMean ();
		BracketLeft [maxIndex] = BracketRight [maxIndex] = NULL;

		for (j=1; j<=mNumberOfChannels; j++) {

			if ((j != maxIndex) && (OnDeck [j] != NULL)) {

				testSignal = OnDeck [j];
				mean2 = testSignal->GetMean ();

				if (mean2 < mean1)
					BracketLeft [j] = testSignal;

				else if (mean2 > mean1)
					BracketRight [j] = testSignal;

				else {

					if (BracketLeft [j] == NULL)
						BracketLeft [j] = testSignal;

					else
						BracketRight [j] = testSignal;
				}
			}
		}

		NPossibleCraters = NCraters = 0;

		for (j=1; j<=mNumberOfChannels; j++) {

			if (BracketLeft [j] != NULL) {

				NPossibleCraters++;

				if (BracketRight [j] != NULL)
					NCraters++;
			}
		}

		//  Now test worst case width variation:  NOTE!!!  WE MAY NEED TO PUT THIS BACK WHEN USING RAW DATA!!!!!!!!!!!!

//		if (maxWidth - minWidth > mWidthMatchFraction * (maxWidth + minWidth))  // if true, not uniform enough...skip it
//			continue;  // or add contingency here???

		// Now test if width small enough to be a spike...can we just test the width for nextSignal?  Let's assume so for now...

		calculatedNormalWidth = 3.0 + 2.5E-4 * primeSignal->GetMean ();  // This is VERY approximate...for now
		currentWidth = primeSignal->GetStandardDeviation ();

		if (maxWidth < mWidthToleranceForSpike) {

			LookCount = 0;

			for (i=1; i<=mNumberOfChannels; i++) {

				testSignal = OnDeck [i];

				if (testSignal != NULL)
					LookCount++;
			}

			if (LookCount > 1) {

				for (i=1; i<=mNumberOfChannels; i++) {

					testSignal = OnDeck [i];

					if (testSignal != NULL) {

			//			mDataChannels [i]->InsertIntoArtifactList (testSignal);	// Need this????????????????????????????????????????????????
						testSignal->SetMessageValue (spike, true);
						testSignal->AddNoticeToList (1, "", "Suspected spike");
					}
				}
			}

			// Reset Bracket array

			for (i=1; i<=mNumberOfChannels; i++)
				BracketLeft [i] = NULL;
		}

		else {

			// This is bleed-through or pull-up...whatever
			// Look for possible crater...but first eliminate signals that should be beneath the radar (or not?)

			LookCount = 0;

			for (i=1; i<=mNumberOfChannels; i++) {

				testSignal = OnDeck [i];

				if (testSignal != NULL)
					LookCount++;
			}

			NCraterTests = 0;

			if (LookCount > 0) {

				CraterLeftLimit = primeSignal->GetMean () - nSigmaForCraters * currentWidth;
				//CraterLeftLimit = primeSignal->GetApproximateBioID () - 0.375;

				for (i=1; i<=mNumberOfChannels; i++) {

					if (BracketLeft [i] != NULL) {

						if (BracketLeft [i]->GetMean () < CraterLeftLimit) {

							BracketLeft [i] = NULL;
							NPossibleCraters--;

							if (BracketRight [i] != NULL)
								NCraters--;
						}

						else
							NPossibleCraters++;
					}
				}

				if (NPossibleCraters > 0) {

					CraterRightLimit = primeSignal->GetMean () + nSigmaForCraters * currentWidth;
					//CraterRightLimit = primeSignal->GetApproximateBioID () + 0.375;

					while (true) {

						testSignal = (DataSignal*) it ();
						
						if (testSignal != NULL) {

							NCraterTests++;

							if (testSignal->GetMean () > CraterRightLimit)
								break;

							i = testSignal->GetChannel ();

							if (BracketLeft [i] == NULL)
								continue;

							if (BracketRight [i] == NULL) {

								BracketRight [i] = testSignal;
								NCraters++;
							}

							continue;
						}

						else
							break;
					}

					for (i=0; i<NCraterTests; i++) {

						--it;
						testSignal = (DataSignal*) it.CurrentItem ();

						if (testSignal->GetMean () <= CraterRightLimit) {

//							++it;
							break;
						}
					}

					for (i=1; i<=mNumberOfChannels; i++) {

						if (BracketRight [i] == NULL)
							BracketLeft [i] = NULL;
					}

					for (i=1; i<=mNumberOfChannels; i++) {

						if ((BracketLeft [i] != NULL) && (BracketRight [i] != NULL))
							OnDeck [i] = NULL;
					}
				}
			}

			nInterchannelLinks = 0;

			if ((LookCount > 1) || (NCraters > 0)) {

				primeSignal = OnDeck [maxIndex];
				iChannel = new STRInterchannelLinkage (mNumberOfChannels);
				mInterchannelLinkageList.push_back (iChannel);
				nInterchannelLinks = 1;

				for (i=1; i<=mNumberOfChannels; i++) {

					testSignal = OnDeck [i];

					if (testSignal == NULL)
						continue;

					iChannel->AddDataSignal (testSignal);
					testSignal->SetInterchannelLink (iChannel);
					
		//			mDataChannels [i]->InsertIntoArtifactList (testSignal);	// Need this????????????????????????????????????????????
					testSignal->SetMessageValue (pullup, true);
					testSignal->AddNoticeToList (1, "", "Suspected pull-up");
					mNumberOfPullups++;
				}
			}

			mNumberOfPrimaryPullups++;
			mNumberOfPullups--;

			if (NCraters > 0) {

				// add notice objects for pull-up craters!

				for (i=1; i<=mNumberOfChannels; i++) {

					testSignal = BracketLeft [i];
					testSignal2 = BracketRight [i];

					if ((testSignal == NULL) || (testSignal2 == NULL))
						continue;

					if ((testSignal->DontLook ()) || (testSignal2->DontLook ()))
						continue;

					if ((testSignal->GetMessageValue (spike)) || (testSignal2->GetMessageValue (spike)))
						continue;

					//   Create CraterSignal and set it all up
					testSignal->SetMessageValue (craterSidePeak, true);
			//		mDataChannels [i]->InsertIntoArtifactList (testSignal);	// Need this????????????????????????????????????????????
					testSignal->AddNoticeToList (1, "", "Suspected pull-up induced crater with next peak");

					testSignal2->SetMessageValue (craterSidePeak, true);
			//		mDataChannels [i]->InsertIntoArtifactList (testSignal2);	// Need this????????????????????????????????????????????
					testSignal2->AddNoticeToList (1, "", "Suspected pull-up induced crater with previous peak");
					mNumberOfPullups++;

					nextSignal = new CraterSignal (testSignal, testSignal2);
					nextSignal->SetMessageValue (crater, true);
					nextSignal->SetMessageValue (pullup, true);
					iChannel->AddDataSignal (nextSignal);
					nextSignal->SetInterchannelLink (iChannel);

			//		mDataChannels [i]->InsertIntoArtifactList (nextSignal);	// Need this????????????????????????????????????????????
					mDataChannels [i]->InsertIntoCompleteCurveList (nextSignal);
					mDataChannels [i]->InsertIntoPreliminaryCurveList (nextSignal);
				}
			}

			if (nInterchannelLinks > 0)
				iChannel->RecalculatePrimarySignalSM ();

			for (i=1; i<=mNumberOfChannels; i++) {

				BracketLeft [i] = NULL;
				BracketRight [i] = NULL;
			}
		}
	}

	OverallList.Clear ();

	delete[] OnDeck;
	delete[] means;
	delete[] isDone;
	delete[] BracketLeft;
	delete[] BracketRight;
		
	return 0;
}


int STRCoreBioComponent :: AnalyzeCrossChannelWithNegativePeaksSM () {

	//
	//  This is sample stage 1 - called after ILS is analyzed
	//

	int size = mNumberOfChannels + 1;
	DataSignal** OnDeck = new DataSignal* [size];
	double* noiseLevels = new double [size];
	double* means = new double [size];
	int currentIndex;
	bool* isDone = new bool [size];
	RGDList OverallList;
	bool NowWeAreDone = false;
	DataSignal* nextSignal;
	DataSignal* nextSignal2;
	double minPeak;
	double maxPeak;
	double minWidth;
	double maxWidth;
	DataSignal* primeSignal;
	//int maxIndex;
	//double currentPeak;
	double currentWidth;
	double calculatedNormalWidth;
	double mTimeTolerance = 0.0375;
	double mWidthMatchFraction = 0.1;  // Double it to get width fractional tolerance, currently 20%
	double mWidthToleranceForSpike = 2.1;  // Width must be less than this width to qualify as a spike
	double nSigmaForCraters = 2.0;
	RGString info;
	DataSignal* testSignal;
	DataSignal* testSignal2;
	NowWeAreDone = true;
	double mean1;
	double mean2;
	int channel1;
	int channel2;
	//int NPossibleCraters;
	//int NCraters;
	//int NCraterTests;
	//double CraterLeftLimit;
	//double CraterRightLimit;
	int LookCount;
	mNumberOfPullups = 0;
	mNumberOfPrimaryPullups = 0;
	int i;
	int n;
	RGDList** TempList = new RGDList* [mNumberOfChannels + 1];
	RGDList** TempMultiPeakList = new RGDList* [mNumberOfChannels + 1];
	RGDList** TempCraterPeakList = new RGDList* [mNumberOfChannels + 1];
	DataSignal* prevSignal;
//	double saturationArtifactTolerance;
//	double deltaTime;
//	DataSignal* newCraterSignal;
	ChannelData* nextChannel;
	RGDList* nextList;
	RGDList multiPeakSignals;

	smPullUp pullup;
	smPrimaryInterchannelLink primaryLink;
	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smSpike spike;
	smBelowMinRFU belowMinRFU;
	smSigmoidalPullup sigmoidalPullup;
	smMinPrimaryPullupThreshold primaryPullupThreshold;
	smSignalIsCtrlPeak isControlPeak;
	smLaserOffScale laserOffScale;
//	PullUpFound pullupNotice;
//	PullUpPrimaryChannel primaryPullupNotice;

	CoreBioComponent::minPrimaryPullupThreshold = (double) GetThreshold (primaryPullupThreshold);
	PreTestSignalsForLaserOffScaleSM ();

	ParameterServer* pServer = new ParameterServer;
	double measurementResolution = pServer->GetInterSampleSpacing ();
	double minPullupThreshold = 0.5 * measurementResolution;
	delete pServer;

	//RGDList NewCraterSignalsToAdd;
	RGDList CraterSignalsToRemove;
	InterchannelLinkage* iChannel;

	double first = mDataChannels [mLaneStandardChannel]->GetFirstAnalyzedMean ();

	//cout << "Found first time of ILS = " << first << endl;

//	double peak1;
//	double peak2;
	int nInterchannelLinks = 0;

	for (i=1; i<=mNumberOfChannels; i++) {

		TempList [i] = new RGDList;
		TempMultiPeakList [i] = new RGDList;
		TempCraterPeakList [i] = new RGDList;
		noiseLevels [i] = mDataChannels [i]->GetNoiseRange ();
	}

	for (i=1; i<=mNumberOfChannels; i++)
		mDataChannels [i]->ResetCompleteIterator ();

	for (i=1; i<=mNumberOfChannels; i++) {

		mDataChannels [i]->ResetPreliminaryIterator ();
		mDataChannels [i]->ResetNegativeCurveIterator ();
	}

	// Now copy from mDataChannels PreliminaryCurveList to TempList (while finding maxSignalPeak??)

	for (i=1; i<= mNumberOfChannels; i++) {

		prevSignal = NULL;
		nextChannel = mDataChannels [i];
		nextList = TempList [i];

		//
		//	Merge PreliminaryCurveList with NegativeCurveList into TempList [i]

//		while (nextSignal = nextChannel->GetNextPreliminaryCurve ()) {
////		while (nextSignal = nextChannel->GetNextCompleteCurve ()) {
//
//			//
//			//	Do we want to do this?  After all, we might get more reliable pull-up identification if we include peaks below minRFU, especially if
//			//	we test for valid primary pull-up
//			//
//
//			//if (nextSignal->GetMessageValue (belowMinRFU))	// 1/7/2014...omit for now
//			//	continue;
//
//			nextList->Append (nextSignal);
//		}

		testSignal = nextChannel->GetNextCompleteCurve ();
		testSignal2 = nextChannel->GetNextNegativeCurve ();

		while (true) {

			if (testSignal == NULL) {

				if (testSignal2 == NULL)
					break;

				if (!CoreBioComponent::SignalIsWithinAnalysisRegion (testSignal2, first)) {	// modified 03/13/2015

					testSignal2 = nextChannel->GetNextNegativeCurve ();
					continue;
				}

				nextList->Append (testSignal2);
				testSignal2->SetChannel (i);
				testSignal2 = nextChannel->GetNextNegativeCurve ();
			}

			else {

				if (testSignal2 == NULL) {

					if (!CoreBioComponent::SignalIsWithinAnalysisRegion (testSignal, first)) {	// modified 03/13/2015

						testSignal = nextChannel->GetNextCompleteCurve ();
						continue;
					}

					nextList->Append (testSignal);
					testSignal->SetChannel (i);
					testSignal = nextChannel->GetNextCompleteCurve ();
				}

				else {

					mean1 = testSignal->GetMean ();

					if (!CoreBioComponent::SignalIsWithinAnalysisRegion (testSignal, first)) {	// modified 03/13/2015

						testSignal = nextChannel->GetNextCompleteCurve ();
						continue;
					}

					mean2 = testSignal2->GetMean ();

					if (!CoreBioComponent::SignalIsWithinAnalysisRegion (testSignal2, first)) {	// modified 03/13/2015

						testSignal2 = nextChannel->GetNextNegativeCurve ();
						continue;
					}

					if (mean1 <= mean2) {

						nextList->Append (testSignal);
						testSignal->SetChannel (i);
						testSignal = nextChannel->GetNextCompleteCurve ();
					}

					else {

						nextList->Append (testSignal2);
						testSignal2->SetChannel (i);
						testSignal2 = nextChannel->GetNextNegativeCurve ();
					}
				}
			}
		}
	}

	//cout << "Done merging positive and negative curves into TempList's" << endl;

	RGDListIterator* tempIterator;
	DataSignal* savedSignal;
	RGDList newTempList;
	RGDList* nextMultiPeakList;
	RGDList* nextCraterPeakList;
	int buffer = 25;
	DataSignal** sequenceLineUp = new DataSignal* [buffer + 5];
	int j;
	int currentTestIndex;
	int nextSignalIndex;

	//
	//	Create new channel specific lists of multi-peaks and merge into TempLists****
	//

	for (i=1; i<=mNumberOfChannels; i++) {

		for (j=0; j<buffer; j++)
			sequenceLineUp [j] = NULL;

		nextChannel = mDataChannels [i];
		nextMultiPeakList = TempMultiPeakList [i];
		nextCraterPeakList = TempCraterPeakList [i];
		nextList = TempList [i];
		tempIterator = new RGDListIterator (*nextList);

		for (j=0; j<buffer; j++) {

			nextSignal = (DataSignal*)(*tempIterator)();
			sequenceLineUp [j] = nextSignal;
		}

		prevSignal = sequenceLineUp [0];
		currentTestIndex = 0;

		if (prevSignal == NULL)
			continue;

		while (true) {

			mean1 = prevSignal->GetApproximateBioID ();

			for (j=1; j<buffer; j++) {

				nextSignalIndex = (currentTestIndex + j)%buffer;
				nextSignal = sequenceLineUp [nextSignalIndex];

				if (nextSignal == NULL)
					break;

				mean2 = nextSignal->GetApproximateBioID ();

				if (mean2 - mean1 >= 0.85)		//check for value?
					break;

				//
				// Test for and add multipeak to newTempList.
				//

				if (prevSignal->IsNegativePeak () == nextSignal->IsNegativePeak ()) {

					if (prevSignal->IsNegativePeak ())
						continue;

					if ((prevSignal->Peak () < 0.7 * nextSignal->Peak ()) || (nextSignal->Peak () < 0.7 * prevSignal->Peak ()))
						continue;

					if (prevSignal->GetMessageValue (isControlPeak) || nextSignal->GetMessageValue (isControlPeak))
						continue;

					double sideMean = 0.5 * (prevSignal->GetMean () + nextSignal->GetMean ());

					// Now test to make sure this isn't two separate peaks, which would result in an exceptionally wide crater

					double width = mLSData->GetWidthAtTime (0.5 * (prevSignal->GetMean () + nextSignal->GetMean ()));

					if (prevSignal->GetWidth () + nextSignal->GetWidth () > 1.9 * width)
						continue;

					//cout << "New crater on channel " << prevSignal->GetChannel () << " at time " << 0.5 * (nextSignal->GetMean () + prevSignal->GetMean ());
					//cout << " at bp = " << 0.5 * (nextSignal->GetApproximateBioID () + prevSignal->GetApproximateBioID ()) << endl;

					testSignal = new CraterSignal (prevSignal, nextSignal);
					testSignal->SetChannel (i);

					if (CoreBioComponent::TestForOffScale (testSignal->GetMean ()))
						testSignal->SetMessageValue (laserOffScale, true);

					testSignal2 = (DataSignal*) nextCraterPeakList->Last ();

					if ((testSignal2 == NULL) || (testSignal->GetMean () > testSignal2->GetMean ()))
						nextCraterPeakList->Append (testSignal);

					else
						nextCraterPeakList->InsertWithNoReferenceDuplication (testSignal);

				}

				else {

					double posPeak;
					double negPeak;

					if (prevSignal->IsNegativePeak ()) {

						negPeak = prevSignal->Peak ();
						posPeak = nextSignal->Peak ();
					}

					else {

						negPeak = nextSignal->Peak ();
						posPeak = prevSignal->Peak ();
					}

					if (posPeak > 3.0 * negPeak)
						continue;

					if (posPeak >= CoreBioComponent::minPrimaryPullupThreshold)
						continue;

					testSignal = new SimpleSigmoidSignal (prevSignal, nextSignal);
					testSignal->SetChannel (i);
					testSignal->RecalculatePullupTolerance ();
					testSignal2 = (DataSignal*) nextMultiPeakList->Last ();
					//prevSignal->ResetPullupTolerance (-1.0);
					//nextSignal->ResetPullupTolerance (-1.0);

					if (CoreBioComponent::TestForOffScale (testSignal->GetMean ()))
						testSignal->SetMessageValue (laserOffScale, true);

					if ((testSignal2 == NULL) || (testSignal->GetMean () > testSignal2->GetMean ()))
						nextMultiPeakList->Append (testSignal);

					else
						nextMultiPeakList->InsertWithNoReferenceDuplication (testSignal);
				}
			}

			nextSignal = (DataSignal*) (*tempIterator) ();
			sequenceLineUp [currentTestIndex] = nextSignal;
			currentTestIndex = (currentTestIndex + 1)%buffer;
			prevSignal = sequenceLineUp [currentTestIndex];

			if (prevSignal == NULL)
				break;
		}

		//cout << "Found all potential craters/sigmoids" << endl;

		delete tempIterator;
		tempIterator = new RGDListIterator (*nextMultiPeakList);

		while (nextSignal = (DataSignal*) (*tempIterator) ())
			nextList->InsertWithNoReferenceDuplication (nextSignal);

		delete tempIterator;

		tempIterator = new RGDListIterator (*nextCraterPeakList);

		while (nextSignal = (DataSignal*) (*tempIterator) ())
			nextList->InsertWithNoReferenceDuplication (nextSignal);

		delete tempIterator;

		//
		//	Save all of the multi peak lists for later, to iterator thru and find multipeaks with no interchannel links
		//
	}

	delete[] sequenceLineUp;

	//
	//	Each TempList contains all CompleteList peaks and all NegativeCurvePeaks and all potential multipeaks.  Later, look for interchannel links and then remove crater/simplesigmoid code below
	//

	for (i=1; i<=mNumberOfChannels; i++) {
	
		OnDeck [i] = (DataSignal*) TempList [i]->GetFirst ();  //used to be:  mDataChannels [i]->GetNextPreliminaryCurve ();

		if (OnDeck [i] != NULL) {

			means [i] = OnDeck [i]->GetMean ();
			isDone [i] = false;
			OnDeck [i]->SetChannel (i);
		}

		else {

			means [i] = DOUBLEMAX;
			isDone [i] = true;
		}

		NowWeAreDone = NowWeAreDone && isDone [i];
	}

	if (!NowWeAreDone) {

		while (true) {

			NowWeAreDone = true;
			currentIndex = MinimumIndex (means, mNumberOfChannels);

			OverallList.Append (OnDeck [currentIndex]);
			OnDeck [currentIndex] = (DataSignal*) TempList [currentIndex]->GetFirst ();   //used to be:  mDataChannels [currentIndex]->GetNextPreliminaryCurve ();

			if (OnDeck [currentIndex] == NULL) {

				means [currentIndex] = DOUBLEMAX;
				isDone [currentIndex] = true;

				for (i=1; i<=mNumberOfChannels; i++)
					NowWeAreDone = NowWeAreDone && isDone [i];

				if (NowWeAreDone)
					break;
			}

			else {

				means [currentIndex] = OnDeck [currentIndex]->GetMean ();
				OnDeck [currentIndex]->SetChannel (currentIndex);
			}
		}
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		TempList [i]->Clear ();
		delete TempList [i];
	}

	delete[] TempList;
	
	RGDListIterator it (OverallList);
	cout << "Number of overall signals:  " << (int)OverallList.Entries() << endl;

	//
	//	Have looked for craters and other multi-signals above, having been careful to check for positive/negative peaks and multiple peaks within approximately 
	//	0.9 ILS-bps.  After all new multi-signals were found, added them to the various lists (or while finding them), but didn't remove any side
	//	peaks and didn't remove them from multi-signal list.  Now, look for pull-up/pull-down as before (and spikes), but not for craters (as before).
	//	Go back through craters/multi-signals and, if not from pull-up, remove them from all lists, i.e., keep only those multi-signals associated with
	//	a cross channel peak.  At some point, test each channel for largest peak not associated with a pull-up and remove pull-up designation from any
	//	primary pull-up peaks on that channel that are lower.  Such cross-channel links are not true pull-up, at least from that channel.  Also, remove 
	//	any primary pull-ups that are positive to another channel when any pull-up from another peak is to a negative peak.  Reevaluate all linkages with
	//	respect to validity.  This may have to be repeated once for each channel in the kit.  Add appropriate messages to all multi-signals and side peaks.
	//
	//	Broaden pull-up criterion when curve fit is unacceptable.  Also, base pair residual exceeds threshold...Within so many ILS-bps of other pull-up peak?  Has same
	//	call as other pull-up peak (a multi-peak)?  What other criteria for this?
	//
	//	Ideally, in the future, Osiris should be able to assess if a peak is purely pull-up or if it could be a mixture of pull-up and actual allele.  This
	//	may be extremely complex...
	//
	//	Revisit the estimate of crater peak height to take into account not just the peak heights of the side peaks, but also how far apart they are.  The 
	//	farther apart the side peaks, the taller the (missing) crater peak height.  01/16/2014 - this has been done by "flipping" the crater "minimum" about the side peak "peaks"
	//	and it gives a much more accurate estimate of true crater height.  Of course, estimating crater heights is really a black art...
	//
	//	Don't forget to reset it () when done
	//

	n = 0;

	for (i=1; i<=mNumberOfChannels; i++)
		n += TempMultiPeakList [i]->Entries ();

	cout << "Number of sigmoidal signals:  " << n << endl;

	n = 0;

	for (i=1; i<=mNumberOfChannels; i++)
		n += TempCraterPeakList [i]->Entries ();

	//cout << "Number of crater signals:  " << n << endl;

	it.Reset ();
	bool debug;

	RGDList peaksInSameChannel;
	RGDList probablePullupPeaks;
	RGDList pullupList;
	//bool primaryOK;
	RGDListIterator probableIt (probablePullupPeaks);
	double primaryThreshold = CoreBioComponent::minPrimaryPullupThreshold;

	while (nextSignal = (DataSignal*) it ()) {

		//cout << "Next signal ILS-bps = " << nextSignal->GetApproximateBioID () << endl;

		nextSignal2 = (DataSignal*) it ();

		if (nextSignal2 == NULL)
			break;

		//cout << "Next signal2 ILS-bps = " << nextSignal2->GetApproximateBioID () << endl;

		if (nextSignal->GetChannel () == nextSignal2->GetChannel ()) {

			--it;
			continue;
		}

		debug = true;
		mean1 = nextSignal2->GetApproximateBioID ();
		mean2 = nextSignal->GetApproximateBioID ();

		//if ((nextSignal->GetMean () > 1360.0) && (nextSignal->GetMean () < 1390.0))
		//	debug = true;

		//if (debug) cout << "first bp = " << mean2 << ", second bp = " << mean1 << endl;

		//mTimeTolerance = nextSignal->GetMeanVariability () * nextSignal->GetStandardDeviation () + 
		//		nextSignal2->GetMeanVariability () * nextSignal2->GetStandardDeviation () + 2.0;

		//if (mTimeTolerance < minPullupThreshold)
		//	mTimeTolerance = minPullupThreshold;

		//mTimeTolerance = nextSignal->GetPullupToleranceInBP () + nextSignal2->GetPullupToleranceInBP ();


		//mTimeTolerance = 0.075;

#ifdef _USENOISEINPULLUPANALYSIS_
		mTimeTolerance = nextSignal->GetPullupToleranceInBP (noiseLevels [nextSignal->GetChannel ()]) + nextSignal2->GetPullupToleranceInBP (noiseLevels [nextSignal2->GetChannel ()]);
#else
		mTimeTolerance = nextSignal->GetPullupToleranceInBP () + nextSignal2->GetPullupToleranceInBP ();	//check for value?
#endif

		//mean1 = nextSignal2->GetMean ();
		//mean2 = nextSignal->GetMean ();

		//mTimeTolerance = nextSignal->GetMeanVariability () * nextSignal->GetStandardDeviation () + 
		//		nextSignal2->GetMeanVariability () * nextSignal2->GetStandardDeviation ();

		//if (mTimeTolerance < minPullupThreshold)
		//	mTimeTolerance = minPullupThreshold;

		if (mean1 - mean2 >= mTimeTolerance) {

			--it;
			continue;
		}

		//if (debug) cout << "Peaks within pull-up tolerance..." << endl;

		for (i=1; i<=mNumberOfChannels; i++)
			OnDeck [i] = NULL;

		peaksInSameChannel.Clear ();
		probablePullupPeaks.Clear ();
		pullupList.Clear ();

		channel1 = nextSignal->GetChannel ();
		channel2 = nextSignal2->GetChannel ();
		//if (debug) cout << "first channel = " << channel1 << ", second channel = " << channel2 << endl;

		//if (debug) {

			//if (nextSignal2->IsNegativePeak ())
			//	//cout << "next signal2 is negative" << endl;

			//else
				//cout << "next signal2 is positive" << endl;
		//}

		probablePullupPeaks.Append (nextSignal);
		probablePullupPeaks.Append (nextSignal2);

		primeSignal = nextSignal2;
		savedSignal = nextSignal2;

		while (true) {

			nextSignal = (DataSignal*) it ();

			if (nextSignal == NULL)
				break;

			//mTimeTolerance = nextSignal->GetPullupToleranceInBP () + nextSignal2->GetPullupToleranceInBP ();
			//mTimeTolerance = 0.075;

#ifdef _USENOISEINPULLUPANALYSIS_
		mTimeTolerance = nextSignal->GetPullupToleranceInBP (noiseLevels [nextSignal->GetChannel ()]) + nextSignal2->GetPullupToleranceInBP (noiseLevels [nextSignal2->GetChannel ()]);
#else
		mTimeTolerance = nextSignal->GetPullupToleranceInBP () + nextSignal2->GetPullupToleranceInBP ();	//check for value?
#endif

			//mTimeTolerance = nextSignal->GetPullupToleranceInBP () + nextSignal2->GetPullupToleranceInBP ();	//check for value?

			if (nextSignal->GetApproximateBioID () - nextSignal2->GetApproximateBioID () >= mTimeTolerance) {

				--it;
				//if (debug) cout << "New peak outside pull-up tolerance at mean " << nextSignal->GetMean () << endl;
				break;
			}

			probablePullupPeaks.Append (nextSignal);

			//if (debug) cout << "Added new signal on channel " << nextSignal->GetChannel () << " at time " << nextSignal->GetMean () << endl;

			nextSignal2 = nextSignal;
		}

		minPeak = maxPeak = primeSignal->Peak ();
		minWidth = maxWidth = primeSignal->GetWidth ();
		double peak;
		probableIt.Reset ();

		while (nextSignal = (DataSignal*) probableIt ()) {

			if (nextSignal->IsNegativePeak ())
				continue;

			peak = nextSignal->Peak ();
			currentWidth = nextSignal->GetStandardDeviation ();

			if (peak < minPeak)
				minPeak = peak;

			if (currentWidth > maxWidth)
				maxWidth = currentWidth;

			else if (currentWidth < minWidth)
				minWidth = currentWidth;
		}

		maxPeak = FindPrimaryPeak (probablePullupPeaks, primeSignal);

		//if (debug) cout << "Primary peak height = " << maxPeak << endl;

		if (primeSignal == NULL)
			continue;

		if (maxPeak < primaryThreshold)
			continue;

		probableIt.Reset ();
		currentWidth = primeSignal->GetPrimaryPullupDisplacementThreshold ();
		peak = primeSignal->GetMean ();

		while (nextSignal = (DataSignal*) probableIt ()) {

			if (fabs (nextSignal->GetMean () - peak) > currentWidth)
				probableIt.RemoveCurrentItem ();
		}

		if (probablePullupPeaks.Entries () < 2)
			continue;

		//if (debug && (primeSignal != NULL)) cout << "Prime channel is " << primeSignal->GetChannel () << endl;

		//primeSignal = OnDeck [maxIndex]; // This is the primary bleed through or spike, whichever...

		//mean1 = primeSignal->GetApproximateBioID ();

//		if (maxWidth - minWidth > mWidthMatchFraction * (maxWidth + minWidth))  // if true, not uniform enough...skip it
//			continue;  // or add contingency here???
		//
		// Now test if width small enough to be a spike...can we just test the width for nextSignal?  Let's assume so for now...
		//

		calculatedNormalWidth = 3.0 + 2.5E-4 * primeSignal->GetMean ();  // This is VERY approximate...for now
		currentWidth = primeSignal->GetStandardDeviation ();

		if (maxWidth < mWidthToleranceForSpike) {

			LookCount = probablePullupPeaks.Entries ();

			if (LookCount > 1) {

				probableIt.Reset ();

				while (testSignal = (DataSignal*) probableIt ()) {

					testSignal->SetMessageValue (spike, true);
					testSignal->AddNoticeToList (1, "", "Suspected spike");
				}
			}
		}

		else {

			// This is bleed-through or pull-up...whatever

			LookCount = probablePullupPeaks.Entries ();

			//if (debug) cout << "Number of coinciding peaks = " << LookCount << endl;

			nInterchannelLinks = 0;

			while (true) {

				if (LookCount > 1) {

					probablePullupPeaks.RemoveReference (primeSignal);

					if (AssessPrimaryPullup (primeSignal, peaksInSameChannel, probablePullupPeaks, pullupList)) {

						iChannel = new STRInterchannelLinkage (mNumberOfChannels);
						mInterchannelLinkageList.push_back (iChannel);
						nInterchannelLinks = 1;

						while (testSignal = (DataSignal*) pullupList.GetFirst ()) {		// this was wrong...we want peaks from pullupList and they should removed from probablePullupPeaks

							probablePullupPeaks.RemoveReference (testSignal);

							//if (testSignal->IsNegativePeak ())
							//	continue;

							iChannel->AddDataSignal (testSignal);
							testSignal->SetInterchannelLink (iChannel);
							
							testSignal->SetMessageValue (pullup, true);
							testSignal->AddNoticeToList (1, "", "Suspected pull-up");
							mNumberOfPullups++;
						}

						iChannel->AddDataSignal (primeSignal);
						primeSignal->SetInterchannelLink (iChannel);
						primeSignal->SetMessageValue (primaryLink, true);
						mNumberOfPrimaryPullups++;
						iChannel->RecalculatePrimarySignalSM ();
					}

					else {

						//	Try next largest peak and continue until no chances left
					}

					maxPeak = FindPrimaryPeak (probablePullupPeaks, primeSignal);

					if (primeSignal == NULL)
						break;

					if (maxPeak < primaryThreshold)
						break;

					LookCount = probablePullupPeaks.Entries ();
				}

				else
					break;
			}

			probablePullupPeaks.Clear ();
			pullupList.Clear ();
			peaksInSameChannel.Clear ();
		}
	}

	peaksInSameChannel.Clear ();
	probablePullupPeaks.Clear ();
	pullupList.Clear ();

	//cout << "Done with crater/pullup analysis" << endl;

	//
	//	Remove multipeaks from multipeak list that are not pull-up and then insert remaining ones into complete and preliminary curve lists
	//

	//bool bool1;
	//bool bool2;
	double minRFU;

	for (i=1; i<= mNumberOfChannels; i++) {

		nextChannel = mDataChannels [i];
		nextMultiPeakList = TempMultiPeakList [i];
		minRFU = nextChannel->GetMinimumHeight ();

		while (nextSignal = (DataSignal*) nextMultiPeakList->GetFirst()) {

			testSignal = nextSignal->GetPreviousLinkedSignal ();
			testSignal2 = nextSignal->GetNextLinkedSignal ();

			if (nextSignal->HasCrossChannelSignalLink ()) {

				nextChannel->InsertIntoCompleteCurveList (nextSignal);
				nextChannel->InsertIntoPreliminaryCurveList (nextSignal);
				nextSignal2 = nextSignal->GetPreviousLinkedSignal ();
				nextSignal2->SetMessageValue (sigmoidalPullup, true);
				nextSignal2 = nextSignal->GetNextLinkedSignal ();
				nextSignal2->SetMessageValue (sigmoidalPullup, true);
				nextSignal->SetMessageValue (sigmoidalPullup, true);

				if (nextSignal->Peak () < minRFU)
					nextSignal->SetMessageValue (belowMinRFU, true);
			}

			//else if ((testSignal != NULL) && (testSignal2 != NULL)) {

			//	bool1 = testSignal->HasCrossChannelSignalLink ();
			//	bool2 = testSignal2->HasCrossChannelSignalLink ();

			//	if ((bool1 || bool2) && (!(bool1 && bool2))) {	// one has a cross channel link but not both

			//		if (bool1)
			//			iChannel = testSignal->GetInterchannelLink ();

			//		else
			//			iChannel = testSignal2->GetInterchannelLink ();

			//		nextChannel->InsertIntoCompleteCurveList (nextSignal);
			//		nextChannel->InsertIntoPreliminaryCurveList (nextSignal);
			//		iChannel->AddDataSignal (nextSignal);
			//		nextSignal->SetMessageValue (sigmoidalPullup, true);
			//		testSignal->SetMessageValue (sigmoidalPullup, true);
			//		testSignal2->SetMessageValue (sigmoidalPullup, true);

			//		if (bool1)
			//			iChannel->AddDataSignal (testSignal2);

			//		else
			//			iChannel->AddDataSignal (testSignal);
			//	}

			//	else
			//		delete nextSignal;
			//}

			else {

				delete nextSignal;
				nextSignal2 = nextSignal->GetPreviousLinkedSignal ();
				nextSignal2->SetPartOfCluster (false);
				nextSignal2 = nextSignal->GetNextLinkedSignal ();
				nextSignal2->SetPartOfCluster (false);
			}
		}

		delete nextMultiPeakList;
	}

	for (i=1; i<= mNumberOfChannels; i++) {

		nextChannel = mDataChannels [i];
		nextMultiPeakList = TempCraterPeakList [i];
		minRFU = nextChannel->GetMinimumHeight ();

		while (nextSignal = (DataSignal*) nextMultiPeakList->GetFirst()) {

			if (nextSignal->HasCrossChannelSignalLink ()) {

				nextChannel->InsertIntoCompleteCurveList (nextSignal);
				nextChannel->InsertIntoPreliminaryCurveList (nextSignal);
				nextSignal2 = nextSignal->GetPreviousLinkedSignal ();
				nextSignal2->SetMessageValue (craterSidePeak, true);
				nextSignal2 = nextSignal->GetNextLinkedSignal ();
				nextSignal2->SetMessageValue (craterSidePeak, true);
				nextSignal->SetMessageValue (crater, true);

				if (nextSignal->Peak () < minRFU)
					nextSignal->SetMessageValue (belowMinRFU, true);
			}

			else {

				delete nextSignal;
				nextSignal2 = nextSignal->GetPreviousLinkedSignal ();
				nextSignal2->SetPartOfCluster (false);
				nextSignal2 = nextSignal->GetNextLinkedSignal ();
				nextSignal2->SetPartOfCluster (false);
			}
		}

		delete nextMultiPeakList;
	}

	// Extract unlinked signals above primary peak threshold

	RGDList** notPrimaryLists = new RGDList* [mNumberOfChannels + 1];

	for (i=1; i<=mNumberOfChannels; i++)
		notPrimaryLists [i] = new RGDList;

	notPrimaryLists [0] = NULL;

	for (i=1; i<=mNumberOfChannels; i++) {

		nextChannel = mDataChannels [i];
		nextChannel->ResetPreliminaryIterator ();

		while (nextSignal = nextChannel->GetNextPreliminaryCurve ()) {

			if ((nextSignal->Peak () >= primaryThreshold) && !nextSignal->HasCrossChannelSignalLink ())
				notPrimaryLists [i]->Append (nextSignal);
		}
	}

	//while (nextSignal = (DataSignal*) it ()) {

	//	double tempPeak = nextSignal->Peak ();
	//	//if ((nextSignal->Peak () >= primaryThreshold) && !nextSignal->HasCrossChannelSignalLink () && !nextSignal->IsNegativePeak ())
	//	if (tempPeak >= primaryThreshold)
	//		//notPrimaryLists [nextSignal->GetChannel ()]->Append (nextSignal);
	//		continue;
	//}

	delete[] TempMultiPeakList;  // each individual list was already deleted
	delete[] TempCraterPeakList;
	OverallList.Clear ();

	delete[] OnDeck;
	delete[] noiseLevels;
	delete[] means;
	delete[] isDone;

	//TestSignalsForLaserOffScaleSM ();	// Added 09/09/2014 because AnalyzeCrossChannel... called in two places and want to make sure laser off scale tested no matter what
//	UseChannelPatternsToAssessCrossChannelWithNegativePeaksSM ();
		
	for (i=1; i<=mNumberOfChannels; i++) {

		notPrimaryLists [i]->Clear ();
		delete notPrimaryLists [i];
	}

	delete[] notPrimaryLists;
	cout << "Finished cross channel analysis" << endl;
	return 0;
}


int STRCoreBioComponent :: AnalyzeCrossChannelUsingPrimaryWidthAndNegativePeaksSM () {

	//
	//  This is sample stage 1 - called after ILS is analyzed
	//

	int size = mNumberOfChannels + 1;
	DataSignal::SetNumberOfChannels (mNumberOfChannels);
	DataSignal** OnDeck = new DataSignal* [size];
	double* noiseLevels = new double [size];
	double* means = new double [size];
	int currentIndex;
	bool* isDone = new bool [size];
	RGDList OverallList;
	bool NowWeAreDone = false;
	DataSignal* nextSignal;
	DataSignal* nextSignal2;
	DataSignal* primeSignal;
	double mTimeTolerance = 0.0375;
	double mWidthMatchFraction = 0.1;  // Double it to get width fractional tolerance, currently 20%
	double mWidthToleranceForSpike = 2.1;  // Width must be less than this width to qualify as a spike
	double nSigmaForCraters = 2.0;  //2.0;
	RGString info;
	DataSignal* testSignal;
	DataSignal* testSignal2;
	NowWeAreDone = true;
	double mean1;
	double mean2;
	mNumberOfPullups = 0;
	mNumberOfPrimaryPullups = 0;
	int i;
	int n;
	RGDList** TempList = new RGDList* [mNumberOfChannels + 1];
	RGDList** TempMultiPeakList = new RGDList* [mNumberOfChannels + 1];
	RGDList** TempCraterPeakList = new RGDList* [mNumberOfChannels + 1];
	double* maxNonLaserOffScalePeak = new double [mNumberOfChannels + 1];
	DataSignal* prevSignal;
	ChannelData* nextChannel;
	RGDList* nextList;
	RGDList multiPeakSignals;

	smPullUp pullup;
	smCalculatedPurePullup purePullup;
	smPartialPullupBelowMinRFU partialPullupBelowMin;
	smPrimaryInterchannelLink primaryLink;
	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smSpike spike;
	smBelowMinRFU belowMinRFU;
	smSigmoidalPullup sigmoidalPullup;
	smSigmoidalSidePeak sigmoidalSidePeak;
	smMinPrimaryPullupThreshold primaryPullupThreshold;
	smSignalIsCtrlPeak isControlPeak;
	smLaserOffScale laserOffScale;
	smPartOfDualSignal dualPeak;
	smPoorPeakMorphologyOrResolution poorPeakMorphologyOrResolution;
	smSelectUserSpecifiedMinRFUForPrimaryPeakPreset selectUserSpecifiedMinPrimary;

	bool testForPeaksTooCloseTogether = true;
	double dualPeakBPTolerance = 0.20;

	if (GetMessageValue (selectUserSpecifiedMinPrimary))
		CoreBioComponent::minPrimaryPullupThreshold = (double) GetThreshold (primaryPullupThreshold);
	
	else
		CoreBioComponent::minPrimaryPullupThreshold = 3.0;  //****This is new on 12/21/2018 to test the new algorithm for estimating the value for each channel pair

	double maxSigmoidalPositive = (double)GetThreshold (primaryPullupThreshold);
	PreTestSignalsForLaserOffScaleSM ();

	ParameterServer* pServer = new ParameterServer;
	double measurementResolution = pServer->GetInterSampleSpacing ();
	double minPullupThreshold = 0.5 * measurementResolution;
	delete pServer;

	RGDList CraterSignalsToRemove;
	InterchannelLinkage* iChannel;

	double first = mDataChannels [mLaneStandardChannel]->GetFirstAnalyzedMean ();

	//cout << "Found first time of ILS = " << first << endl;

	int nInterchannelLinks = 0;

	for (i=1; i<=mNumberOfChannels; i++) {

		TempList [i] = new RGDList;
		TempMultiPeakList [i] = new RGDList;
		TempCraterPeakList [i] = new RGDList;
		noiseLevels [i] = mDataChannels [i]->GetNoiseRange ();
		maxNonLaserOffScalePeak [i] = 0.0;
	}

	for (i=1; i<=mNumberOfChannels; i++)
		mDataChannels [i]->ResetCompleteIterator ();

	for (i=1; i<=mNumberOfChannels; i++) {

		mDataChannels [i]->ResetPreliminaryIterator ();
		mDataChannels [i]->ResetNegativeCurveIterator ();
	}

	// Now copy from mDataChannels CompleteCurveList and from Negative Curve List peaks that are within analysis region to channel specific TempList (while finding max non-LO signal peak: maxNonLaserOffScalePeak)

	for (i=1; i<= mNumberOfChannels; i++) {

		prevSignal = NULL;
		nextChannel = mDataChannels [i];
		nextList = TempList [i];

		//
		//	Merge PreliminaryCurveList with NegativeCurveList into TempList [i]
		//

		testSignal = nextChannel->GetNextCompleteCurve ();
		testSignal2 = nextChannel->GetNextNegativeCurve ();

		while (true) {

			if (testSignal == NULL) {

				if (testSignal2 == NULL)
					break;

				if (!CoreBioComponent::SignalIsWithinAnalysisRegion (testSignal2, first)) {	// modified 03/13/2015

					testSignal2 = nextChannel->GetNextNegativeCurve ();
					continue;
				}

				nextList->Append (testSignal2);
				testSignal2->SetChannel (i);
				testSignal2 = nextChannel->GetNextNegativeCurve ();
			}

			else {

				if (CoreBioComponent::SignalIsWithinAnalysisRegion (testSignal, first) && (!DataSignal::IsNegativeOrSigmoid (testSignal)) && (!testSignal->GetMessageValue (laserOffScale)) && (testSignal->Peak () > maxNonLaserOffScalePeak [i]))
					maxNonLaserOffScalePeak [i] = testSignal->Peak ();

				if (testSignal2 == NULL) {

					if (!CoreBioComponent::SignalIsWithinAnalysisRegion (testSignal, first)) {	// modified 03/13/2015

						testSignal = nextChannel->GetNextCompleteCurve ();
						continue;
					}

					nextList->Append (testSignal);
					testSignal->SetChannel (i);
					testSignal = nextChannel->GetNextCompleteCurve ();
				}

				else {

					mean1 = testSignal->GetMean ();

					if (!CoreBioComponent::SignalIsWithinAnalysisRegion (testSignal, first)) {	// modified 03/13/2015

						testSignal = nextChannel->GetNextCompleteCurve ();
						continue;
					}

					mean2 = testSignal2->GetMean ();

					if (!CoreBioComponent::SignalIsWithinAnalysisRegion (testSignal2, first)) {	// modified 03/13/2015

						testSignal2 = nextChannel->GetNextNegativeCurve ();
						continue;
					}

					if (mean1 <= mean2) {

						nextList->Append (testSignal);
						testSignal->SetChannel (i);
						testSignal = nextChannel->GetNextCompleteCurve ();
					}

					else {

						nextList->Append (testSignal2);
						testSignal2->SetChannel (i);
						testSignal2 = nextChannel->GetNextNegativeCurve ();
					}
				}
			}
		}
	}

	//  Done merging positive and negative curves into TempList's

	RGDListIterator* tempIterator;
	RGDList newTempList;
	RGDList* nextMultiPeakList;
	RGDList* nextCraterPeakList;
	int buffer = 25;
	DataSignal** sequenceLineUp = new DataSignal* [buffer + 5];
	int j;
	int currentTestIndex;
	int nextSignalIndex;

	//
	//	Create new channel specific lists of multi-peaks (Craters and sigmoids) and merge into TempLists****
	//

	for (i=1; i<=mNumberOfChannels; i++) {

		for (j=0; j<buffer; j++)
			sequenceLineUp [j] = NULL;

		nextChannel = mDataChannels [i];
		nextMultiPeakList = TempMultiPeakList [i];
		nextCraterPeakList = TempCraterPeakList [i];
		nextList = TempList [i];
		tempIterator = new RGDListIterator (*nextList);

		for (j=0; j<buffer; j++) {

			nextSignal = (DataSignal*)(*tempIterator)();
			sequenceLineUp [j] = nextSignal;
		}

		prevSignal = sequenceLineUp [0];
		currentTestIndex = 0;

		if (prevSignal == NULL)
			continue;

		while (true) {

			mean1 = prevSignal->GetApproximateBioID ();

			for (j=1; j<buffer; j++) {

				nextSignalIndex = (currentTestIndex + j)%buffer;
				nextSignal = sequenceLineUp [nextSignalIndex];

				if (nextSignal == NULL)
					break;

				mean2 = nextSignal->GetApproximateBioID ();

				//
				// Test for and add multipeak to newTempList.
				//

				if (prevSignal->IsNegativePeak () == nextSignal->IsNegativePeak ()) {

					//if (mean2 - mean1 >= 0.65)		//check for value?
					//	break;

					if (prevSignal->IsNegativePeak ())
						continue;

					// Now test to make sure this isn't two separate peaks, which would result in an exceptionally wide crater

					if (nextSignal->GetApproximateBioID () - prevSignal->GetApproximateBioID () > 1.0) // The two peaks are more than 1 bp apart, so, too wide
						break;

					if (prevSignal->GetMessageValue (isControlPeak) || nextSignal->GetMessageValue (isControlPeak))
						break;

					double halfWidth = 0.5 * mLSData->GetWidthAtTime (prevSignal->GetMean ());
					bool widthPrev = (prevSignal->GetWidth () > halfWidth);
					halfWidth = 0.5 * mLSData->GetWidthAtTime (nextSignal->GetMean ());
					bool widthNext = (nextSignal->GetWidth () > halfWidth);

					if ((prevSignal->Peak () < 0.7 * nextSignal->Peak ()) || (nextSignal->Peak () < 0.7 * prevSignal->Peak ())) {
						
						if (widthPrev || widthNext)
							break;
					}

					if ((prevSignal->Peak () < 0.5 * nextSignal->Peak ()) || (nextSignal->Peak () < 0.5 * prevSignal->Peak ())) {

							break;
					}

					if (prevSignal->GetMessageValue (laserOffScale) != nextSignal->GetMessageValue (laserOffScale))
						break;
					
					// Use CoreBioComponent::TestForOffScale (double time) to test for laser off-scale as part of decision

					//double width = mLSData->GetWidthAtTime (0.5 * (prevSignal->GetMean () + nextSignal->GetMean ()));
					//double estimatedSigma = 0.5 * ((nextSignal->GetMean () - prevSignal->GetMean ()) + prevSignal->GetStandardDeviation () + nextSignal->GetStandardDeviation ()); // We might want to scale by height to be more accurate

					//if (prevSignal->GetStandardDeviation () + nextSignal->GetStandardDeviation () > 1.9 * width)  // removed 10/15/2016 because partly redundant with mean distance above, but more conservative...plus, standard deviations unreliable width measures.
					//	continue;

					//if (estimatedSigma > 2.1 * width) // It's too wide to be a crater...test to make sure coefficient is ok
					//	break;

					//cout << "New crater on channel " << prevSignal->GetChannel () << " at time " << 0.5 * (nextSignal->GetMean () + prevSignal->GetMean ());
					//cout << " at bp = " << 0.5 * (nextSignal->GetApproximateBioID () + prevSignal->GetApproximateBioID ()) << endl;

					// It may still be too wide...test for width of crater and side peaks.  To be OK, the crater should be within width dicatated by ILS or it could be too wide if both side peaks are too narrow

					testSignal = new CraterSignal (prevSignal, nextSignal);
					testSignal->SetChannel (i);

					double width = testSignal->GetWidth ();
					double mean = testSignal->GetMean ();
					double targetWidth = mLSData->GetWidthAtTime (mean);
					double tooWide = 2.0 * targetWidth;

					if (width > tooWide) {

						mean = prevSignal->GetMean ();
						double tooNarrow = 0.5 *mLSData->GetWidthAtTime (mean);

						if (prevSignal->GetWidth () > tooNarrow) {

							delete testSignal;
							break;
						}

						mean = nextSignal->GetMean ();
						tooNarrow = 0.5 * mLSData->GetWidthAtTime (mean);

						if (nextSignal->GetWidth () > tooNarrow) {

							delete testSignal;
							break;
						}
					}

					testSignal2 = (DataSignal*) nextCraterPeakList->Last ();

					if (CoreBioComponent::TestForOffScale (testSignal->GetMean ()))
						testSignal->SetMessageValue (laserOffScale, true);

					if ((testSignal2 == NULL) || (testSignal->GetMean () > testSignal2->GetMean ()))
						nextCraterPeakList->Append (testSignal);

					else
						nextCraterPeakList->InsertWithNoReferenceDuplication (testSignal);

				}

				else {  // This may be a sigmoidal peak

					double posPeak;
					double negPeak;

					if (mean2 - mean1 >= 1.0)		//check for value?
						break;

					if (prevSignal->IsNegativePeak ()) {

						negPeak = prevSignal->Peak ();
						posPeak = nextSignal->Peak ();
					}

					else {

						negPeak = nextSignal->Peak ();
						posPeak = prevSignal->Peak ();
					}

					if (posPeak > 3.0 * negPeak)
						break;

					if (posPeak >= maxSigmoidalPositive)
						break;

					testSignal = new SimpleSigmoidSignal (prevSignal, nextSignal);
					testSignal->SetChannel (i);
					testSignal->RecalculatePullupTolerance ();
					testSignal2 = (DataSignal*) nextMultiPeakList->Last ();

					if (CoreBioComponent::TestForOffScale (testSignal->GetMean ()))
						testSignal->SetMessageValue (laserOffScale, true);

					if ((testSignal2 == NULL) || (testSignal->GetMean () > testSignal2->GetMean ()))
						nextMultiPeakList->Append (testSignal);

					else
						nextMultiPeakList->InsertWithNoReferenceDuplication (testSignal);
				}
			}

			nextSignal = (DataSignal*) (*tempIterator) ();
			sequenceLineUp [currentTestIndex] = nextSignal;
			currentTestIndex = (currentTestIndex + 1)%buffer;
			prevSignal = sequenceLineUp [currentTestIndex];

			if (prevSignal == NULL)
				break;
		}

		delete tempIterator;
		tempIterator = new RGDListIterator (*nextMultiPeakList);

		while (nextSignal = (DataSignal*) (*tempIterator) ())
			nextList->InsertWithNoReferenceDuplication (nextSignal);

		delete tempIterator;

		tempIterator = new RGDListIterator (*nextCraterPeakList);

		while (nextSignal = (DataSignal*) (*tempIterator) ())
			nextList->InsertWithNoReferenceDuplication (nextSignal);

		delete tempIterator;

		//
		//	Save all of the multi peak lists for later, to iterate thru and find multipeaks with no interchannel links
		//
	}

	cout << "Found all potential craters/sigmoids" << endl;
	delete[] sequenceLineUp;
	RGDListIterator** itArray = new RGDListIterator* [mNumberOfChannels + 1];

	for (i=1; i<=mNumberOfChannels; i++) {

		itArray [i] = new RGDListIterator (*TempList [i]);
	}

	//
	//	Each TempList contains all CompleteList peaks and all NegativeCurvePeaks and all potential multipeaks.  Later, look for interchannel links and then remove crater/simplesigmoid code below.
	//  Now, merge all channel lists into a single list, OverAllList, ordered by peak time (or mean).  Peaks that are potential pull-up/primary pull-up will appear consecutively in OverAllList.
	//

	for (i=1; i<=mNumberOfChannels; i++) {
	
		OnDeck [i] = (DataSignal*) (*(itArray [i])) ();   //used to be:  OnDeck [i] = (DataSignal*) TempList [i]->GetFirst ();  //used to be:  mDataChannels [i]->GetNextPreliminaryCurve ();

		if (OnDeck [i] != NULL) {

			means [i] = OnDeck [i]->GetMean ();
			isDone [i] = false;
			OnDeck [i]->SetChannel (i);
		}

		else {

			means [i] = DOUBLEMAX;
			isDone [i] = true;
		}

		NowWeAreDone = NowWeAreDone && isDone [i];
	}

	if (!NowWeAreDone) {

		while (true) {

			NowWeAreDone = true;
			currentIndex = MinimumIndex (means, mNumberOfChannels);

			OverallList.Append (OnDeck [currentIndex]);
			OnDeck [currentIndex] = (DataSignal*) (*(itArray [currentIndex])) ();   //used to be:  mDataChannels [currentIndex]->GetNextPreliminaryCurve ();

			if (OnDeck [currentIndex] == NULL) {

				means [currentIndex] = DOUBLEMAX;
				isDone [currentIndex] = true;

				for (i=1; i<=mNumberOfChannels; i++)
					NowWeAreDone = NowWeAreDone && isDone [i];

				if (NowWeAreDone)
					break;
			}

			else {

				means [currentIndex] = OnDeck [currentIndex]->GetMean ();
				OnDeck [currentIndex]->SetChannel (currentIndex);
			}
		}
	}

	// OverAllList contains all peaks, including positive and negative, and merged in order of increasing mean value.

	RGDListIterator it (OverallList);
	RGDListIterator Pos (OverallList);
	RGDListIterator Neg (OverallList);
	cout << "Number of overall signals:  " << (int)OverallList.Entries() << endl;
	RGDList peaksWithNonPositiveHeights;
	i = 0;

	while (nextSignal = (DataSignal*) it ()) {

		if (!(nextSignal->Peak () > 0.0)) {

			peaksWithNonPositiveHeights.Append (nextSignal);

			if (nextSignal->IsNegativePeak ())
				i++;
		}
	}

	if (peaksWithNonPositiveHeights.Entries () > 0) {

		cout << "Somehow we have identified " << peaksWithNonPositiveHeights.Entries () << " peaks with non-positive heights" << endl;
		cout << "Of these, " << i << " are from negative peak fits" << endl;
	}

	while (nextSignal = (DataSignal*) peaksWithNonPositiveHeights.GetFirst ())
		OverallList.RemoveReference (nextSignal);

	it.Reset ();

	//  10/1/2020:  Much of the below text is now out of date.  Osiris now discovers the pattern of pull-up between each pair of channels and, using least median of squares,
	//  determines the outliers (partial pull-ups) and the signals that follow the pattern (pure pull-ups).
	//
	//	Have looked for craters and other multi-signals above, having been careful to check for positive/negative peaks and multiple peaks within approximately 
	//	0.9 ILS-bps.  After all new multi-signals were found, added them to the various lists (or while finding them), but didn't remove any side
	//	peaks and didn't remove them from multi-signal list.  Now, look for pull-up/pull-down as before (and spikes), but not for craters (as before).
	//	Go back through craters/multi-signals and, if not from pull-up, remove them from all lists, i.e., keep only those multi-signals associated with
	//	a cross channel peak.  At some point, test each channel for largest peak not associated with a pull-up and remove pull-up designation from any
	//	primary pull-up peaks on that channel that are lower.  Such cross-channel links are not true pull-up, at least from that channel.  Also, remove 
	//	any primary pull-ups that are positive to another channel when any pull-up from another peak is to a negative peak.  Reevaluate all linkages with
	//	respect to validity.  This may have to be repeated once for each channel in the kit.  Add appropriate messages to all multi-signals and side peaks.
	//
	//	Broaden pull-up criterion when curve fit is unacceptable.  Also, base pair residual exceeds threshold...Within so many ILS-bps of other pull-up peak?  Has same
	//	call as other pull-up peak (a multi-peak)?  What other criteria for this?
	//
	//	Ideally, in the future, Osiris should be able to assess if a peak is purely pull-up or if it could be a mixture of pull-up and actual allele.  This
	//	may be extremely complex...
	//
	//	Revisit the estimate of crater peak height to take into account not just the peak heights of the side peaks, but also how far apart they are.  The 
	//	farther apart the side peaks, the taller the (missing) crater peak height.  01/16/2014 - this has been done by "flipping" the crater "minimum" about the side peak "peaks"
	//	and it gives a much more accurate estimate of true crater height.  Of course, estimating crater heights is really a black art...
	//
	//	Don't forget to reset it () when done
	//

	n = 0;

	for (i=1; i<=mNumberOfChannels; i++)
		n += TempMultiPeakList [i]->Entries ();

	cout << "Number of sigmoidal signals:  " << n << endl;

	n = 0;

	for (i=1; i<=mNumberOfChannels; i++)
		n += TempCraterPeakList [i]->Entries ();

	//cout << "Number of crater signals:  " << n << endl;

	it.Reset ();

	RGDList peaksInSameChannel;
	RGDList probablePullupPeaks;
	RGDList weakPullupPeaks;
	RGDList pullupList;
	RGDList signalsToRemove;
	RGDListIterator probableIt (probablePullupPeaks);
	RGDListIterator sameChannelIterator (peaksInSameChannel);
	double primaryThreshold = CoreBioComponent::minPrimaryPullupThreshold;  // This parameter should only be used to restrict primary pull-ups if specified by user.  See smSelectUserSpecifiedMinRFUForPrimaryPeakPreset
	cout << "Primary Threshold = " << primaryThreshold << " RFU" << endl;
	double nSigmasForPullup = 1.0;
	int primaryChannel;
	double primaryTolerance;
	double primaryMean;
	double rightLimit;
	double leftLimit;
	double primaryHeight;
	double primaryWidth;
	RGDList ignoreSidePeaks;
	double rightLimitPlus;
	double leftLimitPlus;
	bool laserStatus;
	smSelectUserSpecifiedMinRFUForPrimaryPeakPreset useSpecifiedMinRFUForPrimary;
	bool userSpecifiedMinRFUForPrimary = GetMessageValue (useSpecifiedMinRFUForPrimary);

	while (nextSignal = (DataSignal*) it ()) {

		// First test if above primaryThreshold and is not negative.  If so, search in vicinity using Pos and Neg to find peaks within region that could be pull-up

	//	report = false;

		if (ignoreSidePeaks.ContainsReference (nextSignal))
			continue;

		primaryHeight = nextSignal->Peak ();
		primaryChannel = nextSignal->GetChannel ();
		bool isOffScale = nextSignal->GetMessageValue (laserOffScale);

		//  *****!!!! 09/28/2020 This tests that the primary height is above the user-specified minimum primary threshold, but only if directed by user specification.
		//  The default value of userSpecifiedMinRFUForPrimary is false.  In either case, if the signal is negative, skip it.

		if ((userSpecifiedMinRFUForPrimary && (primaryHeight < primaryThreshold)) || (nextSignal->IsNegativePeak ()) || nextSignal->IsSigmoidalPeak () || (nextSignal->IsCraterPeak () && !isOffScale))
			continue;

		//  *****!!!! The next test involves the minRFU among minSampleRFU, minLadderRFU and minILSRFU.  The minimum height is the least of these.  
		//  Essentially, Osiris will not consider that a primary pull-up could be below this min height.  I think this may be an error as well!????  Leaving it for now (10/01/2020)

		if (primaryHeight < mDataChannels [nextSignal->GetChannel ()]->GetMinimumHeight ())
			continue;

		//if (nextSignal->GetMessageValue (laserOffScale)) {

		//	channelMaxNonLaserOffScaleHeight = maxNonLaserOffScalePeak [primaryChannel];

		//	if (primaryHeight < channelMaxNonLaserOffScaleHeight)
		//		continue;
		//}

		// nextSignal could be primary
		// pull-up tolerance from exact match of means between primary and pull-up depends on the type of primary peak and the width of the primary peak.  Scoop up
		// any peaks on other channels that fall within the pull-up tolerance of the primary peak's mean.  Laser off scale between possible primary and possible pull-up mush
		// match.  Ignore peaks on the same channel as the primary.  Peaks in pull-up channels must be of lesser height than primary peak.

		Pos.ResetTo (it);
		Neg.ResetTo (it);
		primeSignal = nextSignal;	
		primaryWidth = 0.5 * nextSignal->GetWidth ();
		primaryTolerance = nextSignal->GetPrimaryPullupDisplacementThreshold (nSigmasForPullup);
		primaryMean = nextSignal->GetMean ();
		rightLimit = primaryMean + primaryTolerance;
		leftLimit = primaryMean - primaryTolerance;
		rightLimitPlus = primaryMean + 5.0 * primaryTolerance;
		leftLimitPlus = primaryMean - 5.0 * primaryTolerance;
		probablePullupPeaks.Clear ();
		weakPullupPeaks.Clear ();
		laserStatus = primeSignal->GetMessageValue (laserOffScale);

		// Starting at the primary mean, search in the positive direction

		while (nextSignal2 = (DataSignal*)(++Pos)) {

			if (nextSignal2->GetMean () > rightLimit) {

				if (nextSignal2->GetMean () > rightLimitPlus)
					break;  // too far away; we're done in this direction

				if (nextSignal2->GetChannel () == primaryChannel)
					continue;

				//This next test is for a peak in the pull-up channel that interferes, above noise level, with primary without being close enough to be called pull-up
				//The existence of such peaks make the primary "occluded" in that pull-up channel

				if (TestForWeakPullup (primaryMean, nextSignal2))  
					weakPullupPeaks.Append (nextSignal2);

				continue;
			}

			if (nextSignal2->GetChannel () == primaryChannel)
				continue;  //  oops. same channel

			if (nextSignal2->IsNegativePeak ()) {

				probablePullupPeaks.Append (nextSignal2);  // height doesn't matter; negative peaks have to come from pull-up
				continue;
			}

			if (nextSignal2->GetMessageValue (laserOffScale) != laserStatus)
				continue;

			if (nextSignal2->Peak () < primaryHeight) {

				probablePullupPeaks.Append (nextSignal2);
				continue;
			}
		}

		// Starting at the primary mean, search in the negative direction

		while (nextSignal2 = (DataSignal*)(--Neg)) {

			if (nextSignal2->GetMean () < leftLimit) {

				if (nextSignal2->GetMean () < leftLimitPlus)
					break;  // too far away; we're done in this direction

				if (nextSignal2->GetChannel () == primaryChannel)
					continue;

				if (TestForWeakPullup (primaryMean, nextSignal2))
					weakPullupPeaks.Prepend (nextSignal2);

				continue;
			}

			if (nextSignal2->GetChannel () == primaryChannel)
				continue;  //  oops. same channel

			if (nextSignal2->IsNegativePeak ()) {

				probablePullupPeaks.Append (nextSignal2);  // height doesn't matter; negative peaks have to come from pull-up
				continue;
			}

			if (nextSignal2->GetMessageValue (laserOffScale) != laserStatus)
				continue;

			if (nextSignal2->Peak () < primaryHeight) {

				probablePullupPeaks.Append (nextSignal2);
				continue;
			}
		}

		if (weakPullupPeaks.Entries () > 0) {

			primeSignal->CreateWeakPullupVector (mNumberOfChannels);

			while (nextSignal2 = (DataSignal*)weakPullupPeaks.GetFirst ())
				primeSignal->RecordWeakPullupInChannel (mNumberOfChannels, nextSignal2->GetChannel ());
		}

		//cout << "Done with pull-up tests for peak at " << primaryMean << endl;

		if (probablePullupPeaks.IsEmpty ())
			continue;   // There are no probable pull-up peaks, so go on to next peak.

		// Now process peaks in list probablePullupPeaks...

		if (primeSignal->IsCraterPeak ()) {

			nextSignal2 = primeSignal->GetNextLinkedSignal ();

			if (nextSignal2 != NULL)
				ignoreSidePeaks.Prepend (nextSignal2);

			nextSignal2 = primeSignal->GetPreviousLinkedSignal ();

			if (nextSignal2 != NULL)
				ignoreSidePeaks.Prepend (nextSignal2);
		}

		// unsuccessful attempt to eliminate multiple peaks in a channel  07/06/2016...try eliminating this...

		//if (probablePullupPeaks.Entries () >= 2) {

		//	for (i=1; i<=mNumberOfChannels; i++) {

		//		if (i == primaryChannel)
		//			continue;

		//		probableIt.Reset ();
		//		peaksInSameChannel.Clear ();

		//		while (testSignal = (DataSignal*) probableIt ()) {

		//			if (testSignal->GetChannel () == i)
		//				peaksInSameChannel.Append (testSignal);
		//		}

		//		if (peaksInSameChannel.Entries () <= 1)
		//			continue;

		//		sameChannelIterator.Reset ();
		//		closestSignal = NULL;
		//		signalsToRemove.Clear ();

		//		while (testSignal = (DataSignal*) sameChannelIterator ()) {

		//			if (signalsToRemove.Contains (testSignal))
		//				continue;

		//			if (testSignal->IsCraterPeak ()) {

		//				prevSignal = testSignal->GetPreviousLinkedSignal ();
		//				nextSignal2 = testSignal->GetNextLinkedSignal ();
		//				bool containsWholeCrater = probablePullupPeaks.ContainsReference (prevSignal) && probablePullupPeaks.ContainsReference (nextSignal2);
		//				bool isSigmoid = prevSignal->IsNegativePeak () || nextSignal2->IsNegativePeak ();

		//				if (containsWholeCrater || isSigmoid) {

		//					// This may be valid pull-up crater.  Remove side peaks and verify that there are no more

		//					signalsToRemove.Append (prevSignal);
		//					signalsToRemove.Append (nextSignal2);
		//					probablePullupPeaks.RemoveReference (prevSignal);
		//					probablePullupPeaks.RemoveReference (nextSignal2);

		//					if (closestSignal == NULL) {

		//						closestSignal = testSignal;
		//						leastDistance = fabs (testSignal->GetMean () - primaryMean);
		//					}

		//					else {

		//						testDistance = fabs (testSignal->GetMean () - primaryMean);

		//						if (testDistance < leastDistance) {

		//							leastDistance = testDistance;
		//							signalsToRemove.Append (closestSignal);
		//							probablePullupPeaks.RemoveReference (closestSignal);
		//							closestSignal = testSignal;
		//						}
		//					}
		//				}

		//				else {

		//					signalsToRemove.Append (testSignal);
		//					probablePullupPeaks.RemoveReference (testSignal);
		//				}
		//			}

		//			else {

		//				if (closestSignal == NULL) {

		//					closestSignal = testSignal;
		//					leastDistance = fabs (testSignal->GetMean () - primaryMean);
		//				}

		//				else {

		//					testDistance = fabs (testSignal->GetMean () - primaryMean);

		//					if (testDistance < leastDistance) {

		//						leastDistance = testDistance;
		//						signalsToRemove.Append (closestSignal);
		//						probablePullupPeaks.RemoveReference (closestSignal);
		//						closestSignal = testSignal;
		//					}
		//				}
		//			}
		//		}
		//	}

		//	peaksInSameChannel.Clear ();
		//	signalsToRemove.Clear ();
		//}  // This ends section in which duplicate peaks in a channel are weeded out:  see above...commented out 07/06/2016

		//if (probablePullupPeaks.IsEmpty ())
		//	continue;

		// Now build STRInterChannelLinkage...look below for any important tests:  
		// If we're here, we know nextSignal = primeSignal is primary and there are some pull-up peaks within specified distance
		// First, test for spike

		// The following test needs to be moved until after channel-duplicates have been removed from list

		if (primaryWidth < mWidthToleranceForSpike) {  // primary is too narrow (width < 2.1) so it is a spike

			probableIt.Reset ();
			primeSignal->SetMessageValue (spike, true);

			DataSignal** pullupArray = CollectAndSortPullupPeaksSM (primeSignal, probablePullupPeaks);
			int kk;

			for (kk=1; kk<=mNumberOfChannels; kk++) {

				testSignal = pullupArray [kk];

				if (testSignal != NULL) {

					//if (testSignal->GetWidth () < mWidthToleranceForSpike) {  //***** Fixed 11/24/2020

						testSignal->SetMessageValue (spike, true);  // 10/01/2020:  this test makes calling a pull-up to a spike into a spike only if its width is < 2.1...maybe it should be a spike unconditionally?
						testSignal->SetCouldBePullup (true);
					//}
				}
			}

			probablePullupPeaks.Clear ();
			delete[] pullupArray;
		}

		else {

			//if (primeSignal->GetMessageValue (laserOffScale)) {

			//	double localMax = maxNonLaserOffScalePeak [primeSignal->GetChannel ()];

			//	if (primeSignal->Peak () < localMax) {

			//		probablePullupPeaks.Clear ();
			//		primeSignal->SetMessageValue (primaryLink, false);
			//		continue;
			//	}
			//}

			// Set up STRInterChannelLinkage and add to linkage list; this has to be modified to allow more than one pullUp linkage

			iChannel = new STRInterchannelLinkage (mNumberOfChannels);
			mInterchannelLinkageList.push_back (iChannel);
			iChannel->SetPrimaryDataSignal (primeSignal);

			DataSignal** pullupArray = CollectAndSortPullupPeaksSM (primeSignal, probablePullupPeaks);
			primeSignal->AddProbablePullups (probablePullupPeaks);
			int kk;

			for (kk=1; kk<=mNumberOfChannels; kk++) {  // this was wrong...we want peaks from pullupList and they should removed from probablePullupPeaks

				testSignal = pullupArray [kk];

				if (testSignal != NULL) {

					iChannel->AddDataSignal (testSignal);
					primeSignal->AddCrossChannelSignalLink (testSignal);
					testSignal->SetMessageValue (pullup, true);
					testSignal->SetCouldBePullup (true);
					testSignal->SetPrimarySignalFromChannel (primaryChannel, primeSignal, mNumberOfChannels);
					mNumberOfPullups++;  // fix this...we need to count pull-up peaks, not pull-up pairs...already have counting message which will be accurate and don't do anything with this anyway
				}
			}

			probablePullupPeaks.Clear ();
			delete[] pullupArray;

			primeSignal->SetInterchannelLink (iChannel);  // this is sort of ok.  There can only be one primary signal interchannel link per signal, but pull-up links not needed, so it's ok.
			primeSignal->SetMessageValue (primaryLink, true);
		}

		// We're done with this primary...go on to next
		
	}

	ignoreSidePeaks.Clear ();

	// Done finding all probable pull-ups and primary pull-ups.  Now edit previously created multi-peak list to remove those that are not really multi-peaks because they have no cross channel affect

	it.Reset ();

	peaksInSameChannel.Clear ();
	probablePullupPeaks.Clear ();
	pullupList.Clear ();

	//cout << "Done with crater/pullup analysis" << endl;

	//
	//	Remove multipeaks from multipeak list that are not pull-up and then insert remaining ones into complete and preliminary curve lists
	//

	double minRFU;
	bool sidePeaksArePullup;
	bool sidePeaksHaveSamePrimaryChannel;
	DataSignal* fixPullupPeak;
	RGDList removedPeakList;
	set<InterchannelLinkage*> channelRemoval;
	list<DataSignal*> postSigmoidList;
	bool sidePeakIsPrimary;

	for (i=1; i<= mNumberOfChannels; i++) {

		nextChannel = mDataChannels [i];
		nextMultiPeakList = TempMultiPeakList [i];
		minRFU = nextChannel->GetMinimumHeight ();

		while (nextSignal = (DataSignal*) nextMultiPeakList->GetFirst()) {

			testSignal = nextSignal->GetPreviousLinkedSignal ();
			testSignal2 = nextSignal->GetNextLinkedSignal ();
			
			if ((testSignal == NULL) || (testSignal2 == NULL)) {

				sidePeaksArePullup = true;
				sidePeaksHaveSamePrimaryChannel = false;
				sidePeakIsPrimary = true;
			}

			else {

				sidePeaksArePullup = (testSignal->GetMessageValue (pullup) && testSignal2->GetMessageValue (pullup));
				sidePeaksHaveSamePrimaryChannel = testSignal->HasPullupFromSameChannelAsSM (testSignal2, mNumberOfChannels);
				sidePeakIsPrimary = testSignal->HasCrossChannelSignalLink () || testSignal2->HasCrossChannelSignalLink ();
			}

			if (nextSignal->GetMessageValue (pullup) && !sidePeakIsPrimary && !(sidePeaksArePullup && sidePeaksHaveSamePrimaryChannel)) {  // Fixed And's and Or's 11/24/2020

				//  Actually, sigmoids should never be primary pull-up peaks!  ***Maybe we should take that out of the test!  Done:  11/24/2020
				nextChannel->InsertIntoCompleteCurveList (nextSignal);
				nextChannel->InsertIntoPreliminaryCurveList (nextSignal);
			//	OverallList.InsertWithNoReferenceDuplication (nextSignal);
				testSignal->SetMessageValue (sigmoidalSidePeak, true);
				testSignal2->SetMessageValue (sigmoidalSidePeak, true);
				nextSignal->SetMessageValue (sigmoidalPullup, true);
				postSigmoidList.push_back (nextSignal);

				if (nextSignal->Peak () < minRFU)
					nextSignal->SetMessageValue (belowMinRFU, true);

				//if (testSignal->GetMessageValue (pullup)) {  // Removed 04/29/2018:  we have found peaks that are pullup from one channel while being sigmoids from another

				//	testSignal->SetMessageValue (pullup, false);

				//	for (j=1; j<=mNumberOfChannels; j++) {

				//		if (j == testSignal->GetChannel ())
				//			continue;

				//		nextSignal2 = testSignal->HasPrimarySignalFromChannel (j);

				//		if (nextSignal2 == NULL)
				//			continue;

				//		testSignal->SetPrimarySignalFromChannel (j, NULL, mNumberOfChannels);
				//		iChannel = nextSignal2->GetInterchannelLink ();

				//		if (iChannel != NULL) {

				//			iChannel->RemoveDataSignalFromSecondaryList (testSignal);

				//			if (iChannel->IsEmpty ()) {

				//				nextSignal2->SetInterchannelLink (NULL);
				//				nextSignal2->SetMessageValue (primaryLink, false);
				//				channelRemoval.insert (iChannel);
				//				mInterchannelLinkageList.remove (iChannel);
				//			}
				//		}
				//	}
				//}

				//if (testSignal2->GetMessageValue (pullup)) {

				//	testSignal2->SetMessageValue (pullup, false);

				//	for (j=1; j<=mNumberOfChannels; j++) {

				//		if (j == testSignal2->GetChannel ())
				//			continue;

				//		nextSignal2 = testSignal2->HasPrimarySignalFromChannel (j);

				//		if (nextSignal2 == NULL)
				//			continue;

				//		testSignal2->SetPrimarySignalFromChannel (j, NULL, mNumberOfChannels);
				//		iChannel = nextSignal2->GetInterchannelLink ();

				//		if (iChannel != NULL) {

				//			iChannel->RemoveDataSignalFromSecondaryList (testSignal2);

				//			if (iChannel->IsEmpty ()) {

				//				nextSignal2->SetInterchannelLink (NULL);
				//				nextSignal2->SetMessageValue (primaryLink, false);
				//				channelRemoval.insert (iChannel);
				//				mInterchannelLinkageList.remove (iChannel);
				//			}
				//		}
				//	}
				//}
			}

			else {

				testSignal->SetMessageValue (craterSidePeak, false);
				testSignal->SetMessageValue (sigmoidalSidePeak, false);
				testSignal->SetMessageValue (sigmoidalPullup, false);
				testSignal2->SetMessageValue (craterSidePeak, false);
				testSignal2->SetMessageValue (sigmoidalSidePeak, false);
				testSignal2->SetMessageValue (sigmoidalPullup, false);
				OverallList.RemoveReference (nextSignal);
				//delete nextSignal;
				removedPeakList.Append (nextSignal);

				if (nextSignal->GetMessageValue (pullup)) {

					// remove all pullup links

					for (j=1; j<=mNumberOfChannels; j++) {

						if (j == i)
							continue;

						nextSignal2 = nextSignal->HasPrimarySignalFromChannel (j);

						if (nextSignal2 == NULL)
							continue;

						if (!nextSignal2->HasCrossChannelSignalLink ())
							continue;

						iChannel = nextSignal2->GetInterchannelLink ();

						if (iChannel == NULL)
							continue;

						nextSignal2->RemoveProbablePullup (nextSignal);
						iChannel->RemoveDataSignalFromSecondaryList (nextSignal);  // Consider testing side peaks as pullup candidates

						if (!nextSignal2->ReconfigurePullupFromLinkForChannel (i)) {

							if (iChannel->IsEmpty ()) {

								nextSignal2->SetInterchannelLink (NULL);
								nextSignal2->SetMessageValue (primaryLink, false);
								channelRemoval.insert (iChannel);
								mInterchannelLinkageList.remove (iChannel);
							}
						}
					}
				}

				//if (sidePeaksHaveSamePrimaryChannel) {

				//}

				//if (testSignal->GetMessageValue (pullup)) {

				//	for (j=1; j<=mNumberOfChannels; j++) {

				//		if (i == j)
				//			continue;

				//		nextSignal2 = testSignal->HasPrimarySignalFromChannel (j);

				//		if (nextSignal2 != NULL) {

				//			iChannel = nextSignal2->GetInterchannelLink ();

				//			if (iChannel != NULL) {

				//				if (iChannel->GetSecondarySignalOnChannel (i) (
				//			}
				//		}
				//	}
				//}

				//if (testSignal2->GetMessageValue (pullup)) {

				//}
			}
		}

		delete nextMultiPeakList;
	}

	InterchannelLinkage* iChannelPrimary;
	InterchannelLinkage* secondaryChannel1;
	InterchannelLinkage* secondaryChannel2;
	list<DataSignal*> finalCraterList;

	bool nextSignalIsPullup;
	bool nextSignalIsOffScale;
	double heightImbalanceThreshold = Locus::GetImbalanceThresholdForNoisyPeak ();

	for (i=1; i<= mNumberOfChannels; i++) {

		nextChannel = mDataChannels [i];
		nextMultiPeakList = TempCraterPeakList [i];
		minRFU = nextChannel->GetMinimumHeight ();

		while (nextSignal = (DataSignal*) nextMultiPeakList->GetFirst()) {

			testSignal = nextSignal->GetPreviousLinkedSignal ();
			testSignal2 = nextSignal->GetNextLinkedSignal (); 

			if ((testSignal == NULL) || (testSignal2 == NULL)) {

				sidePeaksArePullup = true;
				sidePeaksHaveSamePrimaryChannel = false;
			}

			else {

				sidePeaksArePullup = (testSignal->GetMessageValue (pullup) && testSignal2->GetMessageValue (pullup));
				sidePeaksHaveSamePrimaryChannel = testSignal->HasPullupFromSameChannelAsSM (testSignal2, mNumberOfChannels);
			}

			nextSignalIsPullup = nextSignal->GetMessageValue (pullup);
			nextSignalIsOffScale = nextSignal->GetMessageValue (laserOffScale);

			if ((nextSignal->HasCrossChannelSignalLink () || nextSignalIsPullup) && (!(sidePeaksArePullup && sidePeaksHaveSamePrimaryChannel) || nextSignalIsOffScale)) {  // And's and Or's realigned 11/24/2020

				nextChannel->InsertIntoCompleteCurveList (nextSignal);
				nextChannel->InsertIntoPreliminaryCurveList (nextSignal);
				nextSignal->SetMessageValue (crater, true);
				iChannelPrimary = nextSignal->GetInterchannelLink ();
				nextSignal2 = nextSignal->GetPreviousLinkedSignal ();
				nextSignal2->SetMessageValue (craterSidePeak, true);
				finalCraterList.push_back (nextSignal);

				//***
				iChannel = nextSignal2->GetInterchannelLink ();

				if (nextSignal2->HasCrossChannelSignalLink () && (iChannel != NULL)) {

					if (channelRemoval.find (iChannel) == channelRemoval.end ()) {  // this means that iChannel is not in channelRemoval

						//mInterchannelLinkageList.remove (iChannel);
						// remove pullup info as well

						iChannel->ResetSecondaryIterator ();

						while (fixPullupPeak = iChannel->GetNextSecondarySignal ()) {

							if ((iChannelPrimary != NULL) && (fixPullupPeak == iChannelPrimary->GetSecondarySignalOnChannel (fixPullupPeak->GetChannel ()))) {

								//fixPullupPeak->SetPullupFromChannel (i, fixPullup->Peak (), mNumberOfChannels);
								fixPullupPeak->SetPrimarySignalFromChannel (i, nextSignal, mNumberOfChannels);
							}

							else {

								//fixPullupPeak->SetPullupFromChannel (i, 0.0, mNumberOfChannels);
								fixPullupPeak->SetPrimarySignalFromChannel (i, NULL, mNumberOfChannels);

								if (!fixPullupPeak->HasAnyPrimarySignals (mNumberOfChannels))
									fixPullupPeak->SetMessageValue (pullup, false);
							}
						}

						nextSignal2->SetInterchannelLink (NULL);
						nextSignal2->SetMessageValue (primaryLink, false);
						channelRemoval.insert (iChannel);
					}
				}
				//***

				nextSignal2 = nextSignal->GetNextLinkedSignal ();
				nextSignal2->SetMessageValue (craterSidePeak, true);

				//***
				iChannel = nextSignal2->GetInterchannelLink ();

				if (nextSignal2->HasCrossChannelSignalLink () && (iChannel != NULL)) {

					if (channelRemoval.find (iChannel) == channelRemoval.end ()) {  // this means that iChannel is not in channelRemoval

						//mInterchannelLinkageList.remove (iChannel);
						// remove pullup info as well

						iChannel->ResetSecondaryIterator ();

						while (fixPullupPeak = iChannel->GetNextSecondarySignal ()) {

							if ((iChannelPrimary != NULL) && (fixPullupPeak == iChannelPrimary->GetSecondarySignalOnChannel (fixPullupPeak->GetChannel ()))) {

								//fixPullupPeak->SetPullupFromChannel (i, fixPullup->Peak (), mNumberOfChannels);
								fixPullupPeak->SetPrimarySignalFromChannel (i, nextSignal, mNumberOfChannels);
							}

							else {

								//fixPullupPeak->SetPullupFromChannel (i, 0.0, mNumberOfChannels);
								fixPullupPeak->SetPrimarySignalFromChannel (i, NULL, mNumberOfChannels);

								if (!fixPullupPeak->HasAnyPrimarySignals (mNumberOfChannels))
									fixPullupPeak->SetMessageValue (pullup, false);
							}
						}

						nextSignal2->SetInterchannelLink (NULL);
						nextSignal2->SetMessageValue (primaryLink, false);
						channelRemoval.insert (iChannel);
					}
				}
				//***

				nextSignal->SetMessageValue (crater, true);

				if (nextSignal->Peak () < minRFU)
					nextSignal->SetMessageValue (belowMinRFU, true);
			}

			else {

				testSignal = nextSignal->GetPreviousLinkedSignal ();
				testSignal->SetMessageValue (craterSidePeak, false);
				secondaryChannel1 = testSignal->GetInterchannelLink ();
				testSignal2 = nextSignal->GetNextLinkedSignal ();
				testSignal2->SetMessageValue (craterSidePeak, false);
				secondaryChannel2 = testSignal2->GetInterchannelLink ();
				OverallList.RemoveReference (nextSignal);
				iChannel = nextSignal->GetInterchannelLink ();
				double heightLeft = testSignal->Peak ();
				double heightRight = testSignal2->Peak ();

				bool sidePeaksTooClose = (fabs (testSignal2->GetApproximateBioID () - testSignal->GetApproximateBioID ()) < dualPeakBPTolerance);
				bool atLeastOneSidePeakIsPullup = (testSignal->GetMessageValue (pullup) || testSignal2->GetMessageValue (pullup));
				bool sidePeaksAreDualSignals = (testSignal->GetMessageValue (dualPeak) && testSignal2->GetMessageValue (dualPeak));
				bool sidePeakIsPrimary = ((testSignal->GetInterchannelLink () != NULL) || (testSignal2->GetInterchannelLink () != NULL));
				bool notTooImbalanced = (heightRight < heightImbalanceThreshold * heightLeft) || (heightLeft < heightImbalanceThreshold * heightRight);  // 11/25/2020:  changed from &&, which is always false, to || and changed > to <

				if (testForPeaksTooCloseTogether && (sidePeaksTooClose || sidePeaksAreDualSignals) && (!sidePeaksArePullup || sidePeaksHaveSamePrimaryChannel) && atLeastOneSidePeakIsPullup && !sidePeakIsPrimary && notTooImbalanced) {

					// Either only one side peak is pullup or both are and from same channel, plus, peaks are close and are dual signals, and test is enabled, and neither side peak is primary pullup...whew!
					// We wouldn't be here if nextSignal were either a primary peak or a pullup, so, don't have to worry about cross channel effects on nextSignal.
					// Create noisy peak and transfer pullups to the new peak from side peaks.  We know at least one of side peaks is a pullup.

					DataSignal* noisySignal = new NoisyPeak (testSignal, testSignal2, true);
					noisySignal->CaptureSmartMessages ();

					noisySignal->SetDontLook (false);
					noisySignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
					testSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
					testSignal2->SetMessageValue (poorPeakMorphologyOrResolution, true);
					noisySignal->SetMessageValue (pullup, true);

					//if (nextSignal->GetMessageValue (pullup)) {

					//	for (j=1; j<=mNumberOfChannels; j++) {

					//		if (i == j)
					//			continue;

					//		DataSignal* prevPrimary = testSignal->HasPrimarySignalFromChannel (j);

					//		if (prevPrimary != NULL) {

					//			noisySignal->SetPrimarySignalFromChannel (j, prevPrimary, mNumberOfChannels);
					//			InterchannelLinkage* prevLinkage = prevPrimary->GetInterchannelLink ();

					//			if (prevLinkage != NULL) {

					//				prevLinkage->RemoveDataSignalFromSecondaryList (testSignal);
					//				prevLinkage->AddDataSignal (noisySignal);
					//			}
					//		}
					//	}
					//}

					if (noisySignal->Peak () >= minRFU)
						noisySignal->SetMessageValue (belowMinRFU, false);

					else
						noisySignal->SetMessageValue (belowMinRFU, true);

					// Remove (?) testSignal and testSignal2 from PreliminaryCurveList for appropriate channel and add noisySignal to same list and to CompleteList
					nextChannel->InsertIntoCompleteCurveList (noisySignal);

					double detectionLimit = nextChannel->GetDetectionThreshold ();

					if (noisySignal->Peak () >= detectionLimit)
						nextChannel->InsertIntoPreliminaryCurveList (noisySignal);

					OverallList.InsertWithNoReferenceDuplication (noisySignal);
					nextChannel->RemovePreliminaryCurveReference (testSignal);
					nextChannel->RemovePreliminaryCurveReference (testSignal2);

					if (testSignal->GetMessageValue (pullup)) {

						DataSignal* prevPrimary;
						InterchannelLinkage* prevLinkage;
						noisySignal->SetMessageValue (pullup, true);

						for (j=1; j<=mNumberOfChannels; j++) {

							if (i == j)
								continue;

							prevPrimary = testSignal->HasPrimarySignalFromChannel (j);

							if (prevPrimary != NULL) {

								noisySignal->SetPrimarySignalFromChannel (j, prevPrimary, mNumberOfChannels);
								testSignal->SetPrimarySignalFromChannel (j, NULL, mNumberOfChannels);
								prevLinkage = prevPrimary->GetInterchannelLink ();

								if (prevLinkage != NULL) {

									prevLinkage->RemoveDataSignalFromSecondaryList (testSignal);
									prevLinkage->AddDataSignal (noisySignal);
								}
							}
						}
					}

					if (testSignal2->GetMessageValue (pullup)) {

						DataSignal* nextPrimary;
						InterchannelLinkage* nextLinkage;
						noisySignal->SetMessageValue (pullup, true);

						for (j=1; j<=mNumberOfChannels; j++) {

							if (i == j)
								continue;

							nextPrimary = testSignal2->HasPrimarySignalFromChannel (j);

							if ((nextPrimary != NULL)  && (noisySignal->HasPrimarySignalFromChannel (j) == NULL)) {

								noisySignal->SetPrimarySignalFromChannel (j, nextPrimary, mNumberOfChannels);
								testSignal2->SetPrimarySignalFromChannel (j, NULL, mNumberOfChannels);
								nextLinkage = nextPrimary->GetInterchannelLink ();

								if (nextLinkage != NULL) {

									nextLinkage->RemoveDataSignalFromSecondaryList (testSignal2);
									nextLinkage->AddDataSignal (noisySignal);
								}
							}
						}
					}

					testSignal->SetMessageValue (pullup, false);
					testSignal2->SetMessageValue (pullup, false);
				}

				else if (nextSignal->HasCrossChannelSignalLink () && (iChannel != NULL)) {

					if (channelRemoval.find (iChannel) == channelRemoval.end ()) {  // this means that iChannel is not in channelRemoval

						//mInterchannelLinkageList.remove (iChannel);
						// remove pullup info as well

						iChannel->ResetSecondaryIterator ();

						while (fixPullupPeak = iChannel->GetNextSecondarySignal ()) {

							if ((secondaryChannel1 != NULL) && (fixPullupPeak == secondaryChannel1->GetSecondarySignalOnChannel (fixPullupPeak->GetChannel ()))) {

								//fixPullupPeak->SetPullupFromChannel (i, fixPullupPeak->Peak (), mNumberOfChannels);
								fixPullupPeak->SetPrimarySignalFromChannel (i, testSignal, mNumberOfChannels);
							}

							else if ((secondaryChannel2 != NULL) && (fixPullupPeak == secondaryChannel2->GetSecondarySignalOnChannel (fixPullupPeak->GetChannel ()))) {

								//fixPullupPeak->SetPullupFromChannel (i, fixPullupPeak->Peak (), mNumberOfChannels);
								fixPullupPeak->SetPrimarySignalFromChannel (i, testSignal2, mNumberOfChannels);
							}

							else {

								//fixPullupPeak->SetPullupFromChannel (i, 0.0, mNumberOfChannels);
								fixPullupPeak->SetPrimarySignalFromChannel (i, NULL, mNumberOfChannels);

								if (!fixPullupPeak->HasAnyPrimarySignals (mNumberOfChannels))
									fixPullupPeak->SetMessageValue (pullup, false);
							}
						}

						nextSignal->SetInterchannelLink (NULL);
						nextSignal->SetMessageValue (primaryLink, false);
						channelRemoval.insert (iChannel);
					}
				}

				//delete nextSignal;
				removedPeakList.Append (nextSignal);
			}
		}

		delete nextMultiPeakList;
	}

	list<DataSignal*> postCraterList;

	while (!finalCraterList.empty ()) {

		nextSignal = finalCraterList.front ();
		finalCraterList.pop_front ();
		postCraterList.push_back (nextSignal);

		/*nextSignal2 = nextSignal->GetPreviousLinkedSignal ();

		if ((nextSignal2 != NULL) && (nextSignal2->GetMessageValue (pullup))) {

			nextSignal2->SetMessageValue (pullup, false);

			for (j=1; j<=mNumberOfChannels; j++) {

				if (j == nextSignal2->GetChannel ())
					continue;

				testSignal = nextSignal2->HasPrimarySignalFromChannel (j);

				if (testSignal == NULL)
					continue;

				if (!testSignal->HasCrossChannelSignalLink ())
					continue;

				iChannelPrimary = testSignal->GetInterchannelLink ();

				if (iChannelPrimary == NULL)
					continue;

				testSignal->RemoveProbablePullup (nextSignal2);
				iChannelPrimary->RemoveDataSignalFromSecondaryList (nextSignal2);
				nextSignal2->SetPrimarySignalFromChannel (j, NULL, mNumberOfChannels);
						
				if (nextSignal->IsInProbablePullupList (testSignal)) {

					testSignal2 = nextSignal->HasPrimarySignalFromChannel (j);

					if ((testSignal2 == NULL) || (testSignal2 == testSignal)) {

						iChannelPrimary->AddDataSignal (nextSignal);
						nextSignal->SetPrimarySignalFromChannel (j, testSignal, mNumberOfChannels);
						nextSignal->SetMessageValue (pullup, true);
						continue;
					}
				}

				if (iChannelPrimary->IsEmpty ()) {

					testSignal->SetInterchannelLink (NULL);
					testSignal->SetMessageValue (primaryLink, false);
					channelRemoval.insert (iChannel);
				}
			}
		}

		nextSignal2 = nextSignal->GetNextLinkedSignal ();

		if ((nextSignal2 != NULL) && (nextSignal2->GetMessageValue (pullup))) {

			nextSignal2->SetMessageValue (pullup, false);

			for (j=1; j<=mNumberOfChannels; j++) {

				if (j == nextSignal2->GetChannel ())
					continue;

				testSignal = nextSignal2->HasPrimarySignalFromChannel (j);

				if (testSignal == NULL)
					continue;

				if (!testSignal->HasCrossChannelSignalLink ())
					continue;

				iChannelPrimary = testSignal->GetInterchannelLink ();

				if (iChannelPrimary == NULL)
					continue;

				testSignal->RemoveProbablePullup (nextSignal2);
				iChannelPrimary->RemoveDataSignalFromSecondaryList (nextSignal2);
				nextSignal2->SetPrimarySignalFromChannel (j, NULL, mNumberOfChannels);
						
				if (nextSignal->IsInProbablePullupList (testSignal)) {

					testSignal2 = nextSignal->HasPrimarySignalFromChannel (j);

					if ((testSignal2 == NULL) || (testSignal2 == testSignal)) {

						iChannelPrimary->AddDataSignal (nextSignal);
						nextSignal->SetPrimarySignalFromChannel (j, testSignal, mNumberOfChannels);
						nextSignal->SetMessageValue (pullup, true);
						continue;
					}
				}

				if (iChannelPrimary->IsEmpty ()) {

					testSignal->SetInterchannelLink (NULL);
					testSignal->SetMessageValue (primaryLink, false);
					channelRemoval.insert (iChannel);
				}
			}
		}*/
	}

	list<InterchannelLinkage*>::iterator tempIt;

	// Remove multisignals (craters and sigmoids) with no cross channel effect

	for (tempIt=mInterchannelLinkageList.begin (); tempIt!=mInterchannelLinkageList.end (); tempIt++) {

		iChannel = *tempIt;

		if (channelRemoval.find (iChannel) != channelRemoval.end ()) {  // this means that iChannel is not in channelRemoval

			continue;
		}

		primeSignal = iChannel->GetPrimarySignal ();
		iChannel->ResetSecondaryIterator ();
		primaryChannel = primeSignal->GetChannel ();
		list<DataSignal*> localRemovePeaks;

		while (fixPullupPeak = iChannel->GetNextSecondarySignal ()) {

			if (removedPeakList.ContainsReference (fixPullupPeak)) {

				localRemovePeaks.push_back (fixPullupPeak);
				continue;
			}

			// test for fixPullupPeak intersecting primeSignal; if so, remove from list...if resulting list is empty, remove it.

			else if (fixPullupPeak->TestForIntersectionWithPrimary (primeSignal)) {

				// remove this peak:  it intersects the primary

				localRemovePeaks.push_back (fixPullupPeak);
				fixPullupPeak->SetPrimarySignalFromChannel (primaryChannel, NULL, mNumberOfChannels);

				if (!fixPullupPeak->HasAnyPrimarySignals (mNumberOfChannels))
					fixPullupPeak->SetMessageValue (pullup, false);

				continue;
			}
		
			fixPullupPeak->SetPrimarySignalFromChannel (primaryChannel, primeSignal, mNumberOfChannels);
			fixPullupPeak->SetMessageValue (pullup, true);
		}

		while (!localRemovePeaks.empty ()) {

			fixPullupPeak = localRemovePeaks.front ();
			localRemovePeaks.pop_front ();
			iChannel->RemoveDataSignalFromSecondaryList (fixPullupPeak);
		}

		if (iChannel->IsEmpty ()) {

			channelRemoval.insert (iChannel);
			primeSignal->SetMessageValue (primaryLink, false);
			primeSignal->SetInterchannelLink (NULL);
			//cout << "  Removing link for primary signal in channel " << primeSignal->GetChannel () << " at time " << primeSignal->GetMean () << endl;
		}
	}

	set<InterchannelLinkage*>::iterator rChannelIt;

	for (rChannelIt=channelRemoval.begin (); rChannelIt!=channelRemoval.end(); rChannelIt++) {

		iChannel = *rChannelIt;
		mInterchannelLinkageList.remove (iChannel);
		delete iChannel;
	}

	channelRemoval.clear ();

	cout << "Removed multi-signals with no cross channel effect" << endl;

	// Now extract unlinked signals above primary peak threshold

	RGDList*** notPrimaryLists = new RGDList** [mNumberOfChannels + 1];

	for (i=1; i<=mNumberOfChannels; i++)
		notPrimaryLists [i] = new RGDList* [mNumberOfChannels + 1];

	for (i=1; i<=mNumberOfChannels; i++) {

		for (j=1; j<=mNumberOfChannels; j++)
			notPrimaryLists [i][j] = new RGDList;
	}

	//notPrimaryLists [0] = NULL;

	bool noCrossChannelLink;
	bool linkButNotToJ;

	for (i=1; i<=mNumberOfChannels; i++) {

		nextChannel = mDataChannels [i];

		for (j=1; j<=mNumberOfChannels; j++) {

			if (i == j)
				continue;

			nextChannel->ResetCompleteIterator ();

			while (nextSignal = nextChannel->GetNextCompleteCurve ()) {

				if (nextSignal->GetMessageValue (laserOffScale))
					continue;

				iChannel = nextSignal->GetInterchannelLink ();

				if (iChannel == NULL) {

					noCrossChannelLink = true;
					linkButNotToJ = true;
				}

				else {

					noCrossChannelLink = false;

					if (iChannel->GetSecondarySignalOnChannel (j) == NULL)
						linkButNotToJ = true;

					else
						linkButNotToJ = false;
				}

				bool considerPossiblePrimaryBasedOnHeight = !userSpecifiedMinRFUForPrimary || (userSpecifiedMinRFUForPrimary && (nextSignal->Peak () >= primaryThreshold));

				//  Allow a peak to be considered as a valid non-primary if it within the analysis region and has no cross channel link with this channel...and if it is tall enough if
				// the user has specified a minimum RFU for primary peaks AND this peak is tall enough, OR (the default action), the user has not specified that a minimun height for primaries be
				// used.  The latter is the default.  And in the latter case, OSIRIS computes the minimum primary RFU based on a number for factors, including the pull-up percentages for the tallest peaks
				// in the primary channel and the static noise level in the pull-up channel.

				if ((CoreBioComponent::SignalIsWithinAnalysisRegion (nextSignal, first)) && considerPossiblePrimaryBasedOnHeight && (noCrossChannelLink || (!noCrossChannelLink && linkButNotToJ))) {

					notPrimaryLists [i][j]->Append (nextSignal);
				}
			}
		}
	}

	cout << "Ready to start assessing cross channel using channel patterns..." << endl;

	//
	//  Perform sample-level assessment of pull-up that goes beyond mere coincidence******************
	//  And, BTW, once this is done, do we ever need to reassess the pull-up call???  Don't think so...
	//

	UseChannelPatternsToAssessCrossChannelWithNegativePeaksSM (notPrimaryLists);  // As of 06/24/2016, this uses linear LMS to find outliers and then linear plus quadratic LS to find pullup coefficients

	for (i=1; i<=mNumberOfChannels; i++) {

		 for (j=1; j<=mNumberOfChannels; j++) {

			 notPrimaryLists [i][j]->Clear ();
			 delete notPrimaryLists [i][j];
		 }

		 delete[] notPrimaryLists [i];
	}

	delete[] notPrimaryLists;

	//
	// Go through list of pullup pairs and remove those for which corrected primary height is less than min primary threshold.
	//

	//*******September 19, 2016

	//*******January 23, 2019...this is a good idea, but this implementation doesn't do what it claims.  It needs to be rethought, especially in light
	// of new changes so that there is not a fixed primary pullup threshold anymore.

	//double pullupThreshold = CoreBioComponent::minPrimaryPullupThreshold;

	//for (tempIt=mInterchannelLinkageList.begin (); tempIt!=mInterchannelLinkageList.end (); tempIt++) {

	//	iChannel = *tempIt;
	//	primeSignal = iChannel->GetPrimarySignal ();
	//	i = primeSignal->GetChannel ();
	//	double analysisThreshold = mDataChannels [i]->GetMinimumHeight ();
	//	double primaryPeak = primeSignal->Peak ();

	//	if ((primaryPeak < pullupThreshold) || (primaryPeak < analysisThreshold)) {

	//		iChannel->ResetSecondaryIterator ();

	//		while (fixPullupPeak = iChannel->GetNextSecondarySignal ()) {

	//			iChannel->RemoveDataSignalFromSecondaryList (fixPullupPeak);		
	//			fixPullupPeak->SetPrimarySignalFromChannel (primaryChannel, NULL, mNumberOfChannels);

	//			if (!fixPullupPeak->HasAnyPrimarySignals (mNumberOfChannels)) {

	//				fixPullupPeak->SetMessageValue (pullup, false);
	//				fixPullupPeak->SetMessageValue (purePullup, false);
	//				fixPullupPeak->SetMessageValue (partialPullupBelowMin, false);
	//			}				
	//		}

	//		channelRemoval.insert (iChannel);
	//		primeSignal->SetMessageValue (primaryLink, false);
	//		primeSignal->SetInterchannelLink (NULL);
	//	}

	//}

	while (!postCraterList.empty ()) {

		primeSignal = postCraterList.front ();
		postCraterList.pop_front ();
		bool hasCrossChannelEffect = primeSignal->GetMessageValue (primaryLink) || primeSignal->HasAnyPrimarySignals (mNumberOfChannels);

		if (!hasCrossChannelEffect) {

			primeSignal->SetMessageValue (crater, false);
			prevSignal = primeSignal->GetPreviousLinkedSignal ();
			nextSignal = primeSignal->GetNextLinkedSignal ();
			
			if (prevSignal != NULL) {

				prevSignal->SetMessageValue (craterSidePeak, false);
				prevSignal->SetPartOfCluster (false);
				prevSignal->SetDoNotCall (false);
			}

			if (nextSignal != NULL) {

				nextSignal->SetMessageValue (craterSidePeak, false);
				nextSignal->SetPartOfCluster (false);
				nextSignal->SetDoNotCall (false);
			}

			nextChannel = mDataChannels [primeSignal->GetChannel ()];
			nextChannel->RemoveCompleteCurveReference (primeSignal);
			nextChannel->RemovePreliminaryCurveReference (primeSignal);
			OverallList.RemoveReference (primeSignal);
			removedPeakList.InsertWithNoReferenceDuplication (primeSignal);
		}
	}

	// add similar code to above for sigmoidal signals...

	while (!postSigmoidList.empty ()) {

		primeSignal = postSigmoidList.front ();
		postSigmoidList.pop_front ();
		bool hasCrossChannelEffect = primeSignal->HasAnyPrimarySignals (mNumberOfChannels);

		if (!hasCrossChannelEffect) {

			primeSignal->SetMessageValue (sigmoidalPullup, false);
			prevSignal = primeSignal->GetPreviousLinkedSignal ();
			nextSignal = primeSignal->GetNextLinkedSignal ();
			
			if (prevSignal != NULL) {

				prevSignal->SetMessageValue (sigmoidalSidePeak, false);
				prevSignal->SetPartOfCluster (false);
				prevSignal->SetDoNotCall (false);
			}

			if (nextSignal != NULL) {

				nextSignal->SetMessageValue (sigmoidalSidePeak, false);
				nextSignal->SetPartOfCluster (false);
				nextSignal->SetDoNotCall (false);
			}

			nextChannel = mDataChannels [primeSignal->GetChannel ()];
			nextChannel->RemoveCompleteCurveReference (primeSignal);
			nextChannel->RemovePreliminaryCurveReference (primeSignal);
			OverallList.RemoveReference (primeSignal);
			removedPeakList.InsertWithNoReferenceDuplication (primeSignal);
		}
	}

	//  See above January 23, 2019

	//for (rChannelIt=channelRemoval.begin (); rChannelIt!=channelRemoval.end(); rChannelIt++) {

	//	iChannel = *rChannelIt;
	//	mInterchannelLinkageList.remove (iChannel);
	//	delete iChannel;
	//}

	//
	// Next, with pull-up peaks remaining, associate associated primary pull-up data
	//

	it.Reset ();

	size_t position = 0;
	double testMean;

	while (nextSignal = (DataSignal*) it ()) {

		testMean = nextSignal->GetMean ();

		if (nextSignal->GetWidth () > mWidthToleranceForSpike)
			nextSignal->SetMessageValue (spike, false);

		nextSignal->SetPullupMessageDataSM (mNumberOfChannels);
		nextSignal->AssociateDataWithPullMessageSM (mNumberOfChannels);

		if (nextSignal->SetPrimaryPullupMessageDataSM (mNumberOfChannels))
			mNumberOfPrimaryPullups++;
	}

	cout << "Done finding all probable/possible pull-ups and primary pull-ups.  Number of primaries = " << mNumberOfPrimaryPullups << endl;

	delete[] TempMultiPeakList;  // each individual list was already deleted
	delete[] TempCraterPeakList;
	OverallList.Clear ();
	removedPeakList.ClearAndDelete ();

	delete[] OnDeck;
	delete[] noiseLevels;
	delete[] means;
	delete[] isDone;
	delete[] maxNonLaserOffScalePeak;

	//TestSignalsForLaserOffScaleSM ();	// Added 09/09/2014 because AnalyzeCrossChannel... called in two places and want to make sure laser off scale tested no matter what

	for (i=1; i<=mNumberOfChannels; i++) {

		TempList [i]->Clear ();
		delete TempList [i];
		delete itArray [i];
	}

	delete[] TempList;
	delete[] itArray;

//	delete[] notPrimaryLists;
	cout << "Finished cross channel analysis" << endl;
	return 0;
}


int STRCoreBioComponent :: UseChannelPatternsToAssessCrossChannelWithNegativePeaksSM (RGDList*** notPrimaryLists) {

	int i;
	int j;
	bool newResult = false;
	bool individualResult;
	bool allOK = true;
	RGDList* currentNonPrimaryList;
	double linear;
	double quadratic;

	//for (i=1; i<=mNumberOfChannels; i++) {

	//	for (j=1; j<=mNumberOfChannels; j++) {

	//		if (j == i)
	//			continue;

	//		currentNonPrimaryList = notPrimaryLists [i][j];

	//		//cout << "Analyze negative peaks for non-laser-off-scale:  primary channel " << i << " and pullup channel " << j << endl;
	//		individualResult = CollectDataAndComputeCrossChannelEffectForChannelsSM (i, j, currentNonPrimaryList, linear, quadratic, false, true);
	//	}

	//	//cout << "\n";
	//}

	for (i=1; i<=mNumberOfChannels; i++) {

		for (j=1; j<=mNumberOfChannels; j++) {

			if (!mPullupTestedMatrix [i][j]) {

				if (j == i)
					continue;

				currentNonPrimaryList = notPrimaryLists [i][j];

				//cout << "Analyze positive peaks for non-laser-off-scale:  primary channel " << i << " and pullup channel " << j << endl;
			
				individualResult = CollectDataAndComputeCrossChannelEffectForChannelsSM (i, j, currentNonPrimaryList, linear, quadratic, false, false);
				newResult = newResult | individualResult;

				if (individualResult == false)
					allOK = false;

				//cout << "\n";
			}
		}
	}

	if (newResult && !allOK) {

		newResult = false;
		allOK = true;

		for (i=1; i<=mNumberOfChannels; i++) {

			for (j=1; j<=mNumberOfChannels; j++) {

				if (j == i)
					continue;

				currentNonPrimaryList = notPrimaryLists [i][j];

				if (!mPullupTestedMatrix [i][j]) {

					//cout << "Analyze positive peaks for non-laser-off-scale:  primary channel " << i << " and pullup channel " << j << endl;

					individualResult = CollectDataAndComputeCrossChannelEffectForChannelsSM (i, j, currentNonPrimaryList, linear, quadratic, false, false);
					newResult = newResult | individualResult;

					if (individualResult == false)
						allOK = false;

					//cout << "\n";
				}
			}
		}
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		for (j=1; j<=mNumberOfChannels; j++) {

			if (j == i)
				continue;

			AcknowledgePullupPeaksWhenThereIsNoPatternSM (i, j, false);
		}
	}

	double maxLinear = 0.0;
	double maxQuad = 0.0;
	double temp;

	for (i=1; i<=mNumberOfChannels; i++) {

		for (j=1; j<=mNumberOfChannels; j++) {

			temp = fabs (mLinearPullupMatrix [i][j]);

			if (temp > maxLinear)
				maxLinear = temp;

			temp = fabs (mQuadraticPullupMatrix [i][j]);

			if (temp > maxQuad)
				maxQuad = temp;

			mLinearInScalePullupMatrix [i] [j] = mLinearPullupMatrix [i] [j];
			mQuadraticInScalePullupMatrix [i] [j] = mQuadraticPullupMatrix [i] [j];
		}
	}

	mQC.mMaxLinearPullupCoefficient = maxLinear;
	mQC.mMaxNonlinearPullupCoefficient = maxQuad;

	ReportPullupMatrix (1);

	// Record all non-laser off-scale coefficients *****12/21/2016
	Endl endLine;
	ScavengePullupFromOtherChannelListLaserInScale ();

	if ((NonLaserOffScalePUCoefficients != NULL) && (NonLaserOffScalePUCoefficients->FileIsValid ())) {

		for (i=1; i<=mNumberOfChannels; i++) {

			for (j=1; j<=mNumberOfChannels; j++) {

				if (j == i)
					continue;

				if (!mPullupTestedMatrix [i][j])
					*NonLaserOffScalePUCoefficients << "?\t?\t";

				else {

					double lin = 100.0 * mLinearPullupMatrix [i][j];
					double quad = 100.0 * mQuadraticPullupMatrix [i][j];
					*NonLaserOffScalePUCoefficients << lin << "\t" << quad << "\t";
				}
			}
		}

		*NonLaserOffScalePUCoefficients << endLine;
	}

	// compute all pullup ratios, etc. and insert into smart data...is this necessary?  We should already have stored pullup values from each other channel

	// now repeat for laser off scale; but, first, reset the pull-up parameter matrices!!

	for (i=1; i<=mNumberOfChannels; i++) {

		for (j=1; j<=mNumberOfChannels; j++) {

			mPullupTestedMatrix [i][j] = false;
			mLinearPullupMatrix [i][j] = 0.0;
			mQuadraticPullupMatrix [i][j] = 0.0;
			mLeastMedianValue [i][j] = 0.0;
			mOutlierThreshold [i][j] = 0.0;
		}
	}

	//for (i=1; i<=mNumberOfChannels; i++) {

	//	for (j=1; j<=mNumberOfChannels; j++) {

	//		if (j == i)
	//			continue;

	//		currentNonPrimaryList = notPrimaryLists [i][j];

	//		if (!mPullupTestedMatrix [i][j]) {

	//			//cout << "Analyze negative peaks for laser-off-scale:  primary channel " << i << " and pullup channel " << j << endl;
	//			individualResult = CollectDataAndComputeCrossChannelEffectForChannelsSM (i, j, currentNonPrimaryList, linear, quadratic, true, true);
	//		}
	//	}

	//	//cout << "\n";
	//}

	newResult = false;
	allOK = true;

	for (i=1; i<=mNumberOfChannels; i++) {

		for (j=1; j<=mNumberOfChannels; j++) {

			if (j == i)
				continue;

			currentNonPrimaryList = notPrimaryLists [i][j];

			if (!mPullupTestedMatrix [i][j]) {

				//cout << "Analyze positive peaks for laser-off-scale:  primary channel " << i << " and pullup channel " << j << endl;

				individualResult = CollectDataAndComputeCrossChannelEffectForChannelsSM (i, j, currentNonPrimaryList, linear, quadratic, true, false);
				newResult = newResult | individualResult;

				if (individualResult == false)
					allOK = false;

				//cout << "\n";
			}
		}
	}

	if (newResult && !allOK) {

		newResult = false;
		allOK = true;

		for (i=1; i<=mNumberOfChannels; i++) {

			for (j=1; j<=mNumberOfChannels; j++) {

				if (j == i)
					continue;

				currentNonPrimaryList = notPrimaryLists [i][j];

				if (!mPullupTestedMatrix [i][j]) {

					//cout << "Analyze positive peaks for laser-off-scale:  primary channel " << i << " and pullup channel " << j << endl;

					individualResult = CollectDataAndComputeCrossChannelEffectForChannelsSM (i, j, currentNonPrimaryList, linear, quadratic, true, false);
					newResult = newResult | individualResult;

					if (individualResult == false)
						allOK = false;

					//cout << "\n";
				}
			}
		}
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		for (j=1; j<=mNumberOfChannels; j++) {

			if (j == i)
				continue;

			AcknowledgePullupPeaksWhenThereIsNoPatternSM (i, j, true);
		}
	}

	ScavengePullupFromOtherChannelListLaserOffScale ();
	ReportPullupMatrix (2);

	// compute all pullup ratios, etc. and insert into smart data...is this necessary?  We should already have stored pullup values from each other channel

	return 0;
}


int STRCoreBioComponent :: WriteXMLGraphicDataSM (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro) {

	RGString fileName = localFileName;
	int begin;
	RGString pResult;

	if (fileName.ExtractLastCharacters (4) == DirectoryManager::GetDataFileTypeWithDot ())
		fileName.ExtractAndRemoveLastCharacters (4);

	RGString fullPath = graphicDirectory + "/" + fileName + ".plt";	
	RGFile outputFile (fullPath, "wt");
	RGTextOutput output (&outputFile, FALSE);

	if (!output.FileIsValid ()) {

		//cout << "Could not write graphic file:  " << fullPath << endl; 
		return -1;
	}

	RGString indent = "\t";
	Endl endLine;
	output << "<plt>" << endLine;
	output << intro;
	output << indent << "<filename>" << xmlwriter::EscAscii (fileName, &pResult) << "</filename>" << endLine;
	output << indent << "<start>0</start>" << endLine;

	if (Progress >= 4) {

		// we have an ILS!
		begin = (int) floor (mLSData->GetBeginAnalysisTime () + 0.5);
		output << indent << "<begin>" << begin << "</begin>" << endLine;
	}

	else
		output << indent << "<begin>0</begin>" << endLine;

	output << indent << "<interval>1</interval>" << endLine;
	output << indent << "<end>" << mDataChannels [1]->GetNumberOfSamples () << "</end>" << endLine;
	output << indent << "<kit>" << xmlwriter::EscAscii (mMarkerSet->GetMarkerSetName (), &pResult) << "</kit>" << endLine;
	output << indent << "<ilsChannel>" << mLaneStandardChannel << "</ilsChannel>" << endLine;

	if (mAssociatedGrid != NULL)
		output << indent << "<associatedLadder>" << xmlwriter::EscAscii (mAssociatedGrid->GetSampleName (), &pResult) << "</associatedLadder>" << endLine;

	else
		output << indent << "<associatedLadder></associatedLadder>" << endLine;

	RGString indent2 = indent + indent;

	for (int i=1; i<=mNumberOfChannels; i++) {

		output << indent << "<channel>" << endLine;
		output << indent2 << "<nr>" << i << "</nr>" << endLine;
		output << indent2 << "<minRFU>" << mDataChannels [i]->GetMinimumHeight () << "</minRFU>" << endLine;
		output << indent2 << "<rawPoints>";
		mDataChannels [i]->WriteRawData (output, " ");
		output << "</rawPoints>" << endLine;
		output << indent2 << "<analyzedPoints>";
		mDataChannels [i]->WriteFitData (output, " ");
		output << "</analyzedPoints>" << endLine;
		
		if ((mAssociatedGrid != NULL) && (i != mLaneStandardChannel)) {

			output << indent2 << "<ladderPoints>";
			mAssociatedGrid->WriteFitDataForChannel (i, output, " ", mDataChannels [i]);
			output << "</ladderPoints>" << endLine;
		}

		//
		//	Write Baseline data, if available
		//

		mDataChannels [i]->WriteBaselineData (output, " ", indent2);

		WriteSmartPeakInfoToXMLForChannel (i, output, indent2, "samplePeak");
		WriteSmartArtifactInfoToXMLForChannel (i, output, indent2);

		if ((mAssociatedGrid != NULL) && (i != mLaneStandardChannel))
			mAssociatedGrid->WriteSmartPeakInfoToXMLForChannel (i, output, indent2, "ladderPeak");

		output << indent << "</channel>" << endLine;
	}

	if (mAssociatedGrid != NULL)
		mAssociatedGrid->WriteLocusInfoToXML (output, indent);	// This should be OK - not message-related

	output << "</plt>" << endLine;
	outputFile.Flush ();

	return 0;
}


void STRCoreBioComponent :: ReevaluateNoiseThresholdBasedOnMachineType (const RGString& machine) {

	double fractionOfStandard = 1.0;
	double machineMultiplier = 1.0;
	int presetFraction;
	size_t position = 0;

	if (machine.FindSubstring ("3500", position))
		machineMultiplier = 4.0;

	else if (machine.FindSubstring ("3730", position))
		machineMultiplier = 4.0;

	smPercentOfStandardNoiseThreshold percentStdNoiseThreshold;
	presetFraction = GetThreshold (percentStdNoiseThreshold);

	if (presetFraction > 0) {

		fractionOfStandard = 0.01 * (double)presetFraction;
		machineMultiplier *= fractionOfStandard;
	}

	double defaultNoise = TracePrequalification::GetDefaultNoiseThreshold ();
	defaultNoise *= machineMultiplier;
	TracePrequalification::SetDefaultNoiseThreshold (defaultNoise);
	TracePrequalification::SetNoiseThreshold (defaultNoise);
	cout << "Machine dependent noise threshold = " << defaultNoise << endl;
}


bool STRCoreBioComponent :: AssessPrimaryPullup (DataSignal* primaryPullup, RGDList& peaksInSameChannel, RGDList& probablePullup, RGDList& pullupList) {

	double threshold = CoreBioComponent::minPrimaryPullupThreshold;
	double* noiseLevels = new double [mNumberOfChannels + 1];

	if (primaryPullup->Peak () < threshold)
		return false;

	if (primaryPullup->IsNegativePeak ())
		return false;

	if (probablePullup.Entries () == 0)
		return false;

	DataSignal** selectedPullup = new DataSignal* [mNumberOfChannels + 1];
	int i;

	for (i=1; i<=mNumberOfChannels; i++) {

		selectedPullup [i] = NULL;
		noiseLevels [i] = mDataChannels [i]->GetNoiseRange ();
	}

	DataSignal* nextSignal;
	//DataSignal* prevSignal;
	int primaryChannel = primaryPullup->GetChannel ();
	double primaryAppBioID = primaryPullup->GetApproximateBioID ();
	RGDListIterator probableIt (probablePullup);
	RGDList sameChannel;
	RGDList cannotBePullup;
	int currentChannel;
	double del1;
	double del2;
	double tempTol;

	while (nextSignal = (DataSignal*) probableIt ()) {

		currentChannel = nextSignal->GetChannel ();

		if (currentChannel == primaryChannel)
			continue;

		del1 = fabs (primaryAppBioID - nextSignal->GetApproximateBioID ());

#ifdef _USENOISEINPULLUPANALYSIS_
		tempTol = nextSignal->GetPullupToleranceInBP (noiseLevels [nextSignal->GetChannel ()]) + primaryPullup->GetPullupToleranceInBP (noiseLevels [primaryPullup->GetChannel ()]);
#else
		tempTol = nextSignal->GetPullupToleranceInBP () + nextSignal2->GetPullupToleranceInBP ();	//check for value?
#endif

		//if (del1 < 0.075) {
		if (del1 < tempTol) {

			if (selectedPullup [currentChannel] == NULL)
				selectedPullup [currentChannel] = nextSignal;

			else {

				del2 = fabs (primaryAppBioID - selectedPullup [currentChannel]->GetApproximateBioID ());

				if (del1 < del2)
					selectedPullup [currentChannel] = nextSignal;
			}
		}
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		if (selectedPullup [i] != NULL)
			pullupList.Append (selectedPullup [i]);
	}

	delete[] selectedPullup;
	delete[] noiseLevels;

	if (pullupList.Entries () == 0)
		return false;

	return true;
}


double STRCoreBioComponent :: FindPrimaryPeak (RGDList& peakList, DataSignal*& primarySignal) {

	double maxPeak = 0.0;
	double currentPeak;
	RGDListIterator it (peakList);
	primarySignal = NULL;
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->IsNegativePeak ())
			continue;

		currentPeak = nextSignal->Peak ();

		if (currentPeak > maxPeak) {

			maxPeak = currentPeak;
			primarySignal = nextSignal;
		}
	}

	return maxPeak;
}


bool STRCoreBioComponent :: TestForWeakPullup (double testMean, DataSignal* testSignal) {

	double noiseLevel = mDataChannels [testSignal->GetChannel ()]->GetNoiseRange ();

	if (testSignal->Value (testMean) > noiseLevel)
		return true;

	return false;
}


//*******************************************************************************************************************************************************


// Smart Message functions***************************************************************************************************************
//***************************************************************************************************************************************


int STRLadderCoreBioComponent :: AnalyzeGridLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is ladder stage 2
	//

	int status = 0;
	
	for (int i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			if (mDataChannels [i]->AnalyzeGridLociSM (text, ExcelText, msg, print) < 0) {

				ErrorString << mDataChannels [i]->GetError ();
				status = -1;
			}
		}
	}

	return status;
}


int STRLadderCoreBioComponent :: AnalyzeGridSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is ladder stage 1
	//

	int status;
	int defaultWindow = TracePrequalification::GetDefaultWindowWidth ();
	double defaultThreshold = TracePrequalification::GetDefaultNoiseThreshold ();
	int currentWindow = TracePrequalification::GetWindowWidth ();
	int recommendedWindow;
	double recommendedThreshold;
	RGString notice;
	smILSFailed ilsFailed;

	ChannelData::SetBeginAnalysisTime (-1.0);
	status = FitLaneStandardCharacteristicsSM (text, ExcelText, msg, print);

	if (status < 0) {

		// This should not come here...FitLaneStandardCharacteristics only returns 0?
		return status;
	}

//	mLSData->ClearAllPeaksBelowAnalysisThreshold ();

	if (AnalyzeLaneStandardChannelSM (text, ExcelText, msg, print) < 0) {

		// ErrorString already populated; cannot go on because need a healthy internal lane standard to do anything else
		SetMessageValue (ilsFailed, true);
		notice = ":  Could not analyze ladder ILS";
		AppendDataForSmartMessage (ilsFailed, notice);
		return -2;
	}

	Progress = 3;
	double ratio = mDataChannels [mLaneStandardChannel]->GetMeasurementRatio ();
	int intRatio = (int) floor (ratio + 0.5);
	recommendedWindow = intRatio - 1;

	if (recommendedWindow < 4)
		recommendedWindow = 4;

	else if (recommendedWindow > defaultWindow)
		recommendedWindow = defaultWindow;

	if (recommendedWindow < currentWindow) {

		TracePrequalification::SetWindowWidth (recommendedWindow);
		recommendedThreshold = ((double)recommendedWindow) * defaultThreshold / ((double)defaultWindow);
		TracePrequalification::SetNoiseThreshold (recommendedThreshold);
	}

	cout << "Window width = " << TracePrequalification::GetWindowWidth () << endl;
	cout << "Noise threshold = " << TracePrequalification::GetNoiseThreshold () << endl;

	ChannelData::SetUseEnhancedShoulderAlgorithm (true);  //  Use of the algorithm still depends on the user preset setting
	status = FitNonLaneStandardCharacteristicsSM (text, ExcelText, msg, print);

	if (status < 0) {

		// This should not come here...FitNonLaneStandardCharacteristics only returns 0?
		return status;
	}

	cout << "Ready to look for ladder pullup..." << endl;

	//AnalyzeCrossChannelSM ();   // 12/10/2017 ***** this is a test

	//
	// Call function to flag all peaks outside lane standard and above user-specified error-reporting threshold
	//

	status = 0;
	int j;
	//double largestTime = 0.0;
	//double currentLast;

	for (j=1; j<=mNumberOfChannels; j++) {

		if (mDataChannels [j]->SetAllApproximateIDs (mLSData) < 0)
			status = -1;

		//if (j != mLaneStandardChannel) {

		//	currentLast = mDataChannels [j]->GetLastTime ();

		//	if (currentLast > largestTime)
		//		largestTime = currentLast;
		//}
	}

	//if (largestTime > mQC.mLastILSTime)
	//	mQC.mLastILSTime = largestTime;

	if (status < 0) {

		notice << "LADDER BIOCOMPONENT COULD NOT UTILIZE INTERNAL LANE STANDARD.  Skipping...";
		cout << notice << endl;
		SetMessageValue (ilsFailed, true);
		notice = ":  Could not create ILS time to base pairs transform";
		AppendDataForSmartMessage (ilsFailed, notice);
		return -5;
	}

	RemoveAllSignalsOutsideLaneStandardSM ();  // Moved to follow setting of all approximate bioID's, above, or nothing is saved in SmartPeaks!

	//
	//  Remove shoulder signals that are "too close" to primary signal on each non-ILS channel...need a new function for this, say 'RemoveAllShouldersTooCloseToPrimary'.
	//  This function will have to call function by the same name that is specialized for each channel.  Because these functions are called here, after all approximate ID's (ILS-BPs)
	//  have been set, we can use that info to determine "too close".  Because these will only be called for ladders, should we also have a relative height criterion for those peaks that are
	//  far enough away from the primary, but are much smaller in height.  This would weed out the noisy peaks as well...
	//

	for (j=1; j<=mNumberOfChannels; j++) {

		if (j != mLaneStandardChannel)
			mDataChannels [j]->RemoveAllShouldersTooCloseToPrimary ();
	}

	Progress = 4;

	if (AssignCharacteristicsToLociSM () < 0) {

		// ErrorString already populated; go on and print out what info there is
		status = -3;
	}

	if (status == 0)
		Progress = 5;

	return status;
}


void STRLadderCoreBioComponent :: MakeNonCoreLadderArtifactsNoncritical () {

	//
	//	This is ladder stage 2
	//

	int i;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i == mLaneStandardChannel)
			continue;

		mDataChannels [i]->MakeNonCoreLadderArtifactsNoncritical ();
	}
}


int STRLadderCoreBioComponent :: AnalyzeCrossChannelSM () {

	//
	//  This is ladder stage 1
	//

	// This is to be called after CoreBioComponent::FitAllCharacteristicsSM...and BEFORE AnalyzeLaneStandardChannelSM
	int size = mNumberOfChannels + 1;
	DataSignal** OnDeck = new DataSignal* [size];
	DataSignal** BracketLeft = new DataSignal* [size];
	DataSignal** BracketRight = new DataSignal* [size];
	double* means = new double [size];
	int currentIndex;
	bool* isDone = new bool [size];
	RGDList OverallList;
	bool NowWeAreDone = false;
	DataSignal* nextSignal;
	DataSignal* nextSignal2;
	double minPeak;
	double maxPeak;
	double minWidth;
	double maxWidth;
	DataSignal* primeSignal;
	int maxIndex;
	double currentPeak;
	double currentWidth;
	double calculatedNormalWidth;
	double mTimeTolerance = 0.95;
	double mWidthMatchFraction = 0.1;  // Double it to get width fractional tolerance, currently 20%
	double mWidthToleranceForSpike = 1.0;  // Width must be less than this width to qualify as a spike
	double nSigmaForCraters = 2.0;
//	Notice* newNotice;
	RGString info;
	DataSignal* testSignal;
	DataSignal* testSignal2;
	NowWeAreDone = true;
	double mean1;
	double mean2;
	int channel1;
	int channel2;
	int NPossibleCraters;
	int NCraters;
	int NCraterTests;
	double CraterLeftLimit;
	double CraterRightLimit;
	CompoundSignalInfo* compound;
	int LookCount;
	mNumberOfPullups = 0;
	mNumberOfPrimaryPullups = 0;
	int i;
	smSpike spike;
	smPullUp pullup;
	smPrimaryInterchannelLink primaryPullup;
	smCrater crater;
	smCraterSidePeak craterSidePeak;

	for (i=1; i<=mNumberOfChannels; i++)
		mDataChannels [i]->ResetPreliminaryIterator ();

	for (i=1; i<=mNumberOfChannels; i++) {
	
		OnDeck [i] = mDataChannels [i]->GetNextPreliminaryCurve ();

		if (OnDeck [i] != NULL) {

			means [i] = OnDeck [i]->GetMean ();
			isDone [i] = false;
			OnDeck [i]->SetChannel (i);
		}

		else {

			means [i] = DOUBLEMAX;
			isDone [i] = true;
		}

		NowWeAreDone = NowWeAreDone && isDone [i];
	}

	if (!NowWeAreDone) {

		while (true) {

			NowWeAreDone = true;
			currentIndex = MinimumIndex (means, mNumberOfChannels);
			OverallList.Append (OnDeck [currentIndex]);
			OnDeck [currentIndex] = mDataChannels [currentIndex]->GetNextPreliminaryCurve ();

			if (OnDeck [currentIndex] == NULL) {

				means [currentIndex] = DOUBLEMAX;
				isDone [currentIndex] = true;

				for (i=1; i<=mNumberOfChannels; i++)
					NowWeAreDone = NowWeAreDone && isDone [i];

				if (NowWeAreDone)
					break;
			}

			else {

				means [currentIndex] = OnDeck [currentIndex]->GetMean ();
				OnDeck [currentIndex]->SetChannel (currentIndex);
			}
		}
	}

	RGDListIterator it (OverallList);

	for (i=1; i<=mNumberOfChannels; i++) {

		BracketLeft [i] = NULL;
		BracketRight [i] = NULL;
	}

	while (nextSignal = (DataSignal*) it ()) {

		nextSignal2 = (DataSignal*) it ();

		if (nextSignal2 == NULL)
			break;

		if (nextSignal->GetChannel () == nextSignal2->GetChannel ()) {

			--it;
			continue;
		}

		mean1 = nextSignal2->GetMean ();
		mean2 = nextSignal->GetMean ();
		mTimeTolerance = nextSignal->GetMeanVariability () * nextSignal->GetStandardDeviation () + 
				nextSignal2->GetMeanVariability () * nextSignal2->GetStandardDeviation ();

		if (mean1 - mean2 >= mTimeTolerance) {

			BracketLeft [nextSignal->GetChannel ()] = nextSignal;
			--it;
			continue;
		}

		if (BracketLeft [nextSignal->GetChannel ()] != NULL)
			BracketLeft [nextSignal->GetChannel ()] = NULL;

		if (BracketLeft [nextSignal2->GetChannel ()] != NULL)
			BracketLeft [nextSignal2->GetChannel ()] = NULL;

		for (i=1; i<=mNumberOfChannels; i++)
			OnDeck [i] = NULL;

		channel1 = nextSignal->GetChannel ();
		channel2 = nextSignal2->GetChannel ();
		OnDeck [channel1] = nextSignal;
		OnDeck [channel2] = nextSignal2;
		primeSignal = nextSignal2;

		while (true) {

			nextSignal = (DataSignal*) it ();

			if (nextSignal == NULL) {

				break;
			}

			mTimeTolerance = nextSignal->GetMeanVariability () * nextSignal->GetStandardDeviation () + 
				nextSignal2->GetMeanVariability () * nextSignal2->GetStandardDeviation ();

			if (nextSignal->GetMean () - nextSignal2->GetMean () >= mTimeTolerance) {

				--it;
				break;
			}

			OnDeck [nextSignal->GetChannel ()] = nextSignal;
			nextSignal2 = nextSignal;
		}

		minPeak = maxPeak = primeSignal->Peak ();
		minWidth = maxWidth = primeSignal->GetStandardDeviation ();
		maxIndex = primeSignal->GetChannel ();

		for (int j=1; j<=mNumberOfChannels; j++) {
		
			testSignal = OnDeck [j];
			
			if (testSignal != NULL) {

				currentPeak = testSignal->Peak ();
				currentWidth = testSignal->GetStandardDeviation ();
				
				if (currentPeak > maxPeak) {

					maxPeak = currentPeak;
					maxIndex = j;
				}

				else if (currentPeak < minPeak)
					minPeak = currentPeak;

				if (currentWidth > maxWidth)
					maxWidth = currentWidth;

				else if (currentWidth < minWidth)
					minWidth = currentWidth;
			}
		}

		primeSignal = OnDeck [maxIndex]; // This is the primary bleed through or spike, whichever...

		//  Now test worst case width variation:  NOTE!!!  WE MAY NEED TO PUT THIS BACK WHEN USING RAW DATA!!!!!!!!!!!!

//		if (maxWidth - minWidth > mWidthMatchFraction * (maxWidth + minWidth))  // if true, not uniform enough...skip it
//			continue;  // or add contingency here???

		// Now test if width small enough to be a spike...can we just test the width for nextSignal?  Let's assume so for now...
		calculatedNormalWidth = 3.0 + 2.5E-4 * primeSignal->GetMean ();  // This is VERY approximate...for now
		currentWidth = primeSignal->GetStandardDeviation ();

		if (maxWidth < mWidthToleranceForSpike) {

			LookCount = 0;

			for (i=1; i<=mNumberOfChannels; i++) {

				testSignal = OnDeck [i];

				if (testSignal != NULL) {

					if (testSignal->DontLook ())
						OnDeck [i] = NULL;

					else
						LookCount++;
				}
			}

			if (LookCount > 1) {

				for (i=1; i<=mNumberOfChannels; i++) {

					testSignal = OnDeck [i];

					if (testSignal != NULL) {

						if (testSignal->GetWidth () < mWidthToleranceForSpike) {

							testSignal->SetMessageValue (spike, true);
							testSignal->AddNoticeToList (1, "", "Suspected spike");
						}
					}
				}
			}

			// Reset Bracket array

			for (i=1; i<=mNumberOfChannels; i++) {

				BracketLeft [i] = NULL;
			}
		}

		else {

			// This is bleed-through or pull-up...whatever
			// Look for possible crater...but first eliminate signals that should be beneath the radar

			LookCount = 0;

			for (i=1; i<=mNumberOfChannels; i++) {

				testSignal = OnDeck [i];

				if (testSignal != NULL) {

					if (testSignal->DontLook ())
						OnDeck [i] = NULL;

					else
						LookCount++;
				}
			}

			NPossibleCraters = NCraterTests = NCraters = 0;

			if (LookCount > 0) {

				CraterLeftLimit = primeSignal->GetMean () - nSigmaForCraters * currentWidth;

				for (i=1; i<=mNumberOfChannels; i++) {

					if (BracketLeft [i] != NULL) {

						if ((BracketLeft [i]->GetMean () < CraterLeftLimit) || (OnDeck [i] != NULL))
							BracketLeft [i] = NULL;

						else
							NPossibleCraters++;
					}
				}

				if (NPossibleCraters > 0) {

					CraterRightLimit = primeSignal->GetMean () + nSigmaForCraters * currentWidth;

					while (true) {

						testSignal = (DataSignal*) it ();
						
						if (testSignal != NULL) {

							NCraterTests++;

							if (testSignal->GetMean () > CraterRightLimit)
								break;

							if (BracketLeft [testSignal->GetChannel ()] == NULL)
								continue;

							BracketRight [testSignal->GetChannel ()] = testSignal;
							NCraters++;
							break;
						}

						else
							break;
					}

					for (i=0; i<NCraterTests; i++)
						--it;

					for (i=1; i<=mNumberOfChannels; i++) {

						if (BracketRight [i] == NULL)
							BracketLeft [i] = NULL;
					}
				}
			}

			if ((LookCount > 1) || (NCraters > 0)) {

				primeSignal = OnDeck [maxIndex];

				for (i=1; i<=mNumberOfChannels; i++) {

					testSignal = OnDeck [i];

					if (testSignal == NULL)
						continue;
					
					if (i != maxIndex) {

	//					mDataChannels [i]->InsertIntoArtifactList (testSignal);
						testSignal->SetMessageValue (pullup, true);
						testSignal->AddNoticeToList (1, "", "Suspected pull-up");
						mNumberOfPullups++;
						testSignal->SetPrimaryCrossChannelSignalLink (primeSignal);
						primeSignal->AddCrossChannelSignalLink (testSignal);
					}

					else {

						testSignal->SetMessageValue (primaryPullup, true);
						testSignal->AddNoticeToList (1, "", "Suspected primary pull-up");
	//					mDataChannels [maxIndex]->InsertIntoArtifactList (testSignal);
						mNumberOfPrimaryPullups++;
					}
				}
			}

			if (NCraters > 0) {

				// add notice objects for pull-up craters!

				for (i=1; i<=mNumberOfChannels; i++) {

					testSignal = BracketLeft [i];
					testSignal2 = BracketRight [i];

					if ((testSignal == NULL) || (testSignal2 == NULL))
						continue;

	//				mDataChannels [i]->InsertIntoArtifactList (testSignal);
					testSignal->SetMessageValue (craterSidePeak, true);	// crater induced by pullup?
					testSignal->AddNoticeToList (1, "", "Suspected pull-up induced crater with next peak");
					testSignal->SetPrimaryCrossChannelSignalLink (primeSignal);
					primeSignal->AddCrossChannelSignalLink (testSignal);

	//				mDataChannels [i]->InsertIntoArtifactList (testSignal2);
					testSignal2->SetMessageValue (craterSidePeak, true);	// crater induced by pullup?
					testSignal2->AddNoticeToList (1, "", "Suspected pull-up induced crater with previous peak");
					testSignal2->SetPrimaryCrossChannelSignalLink (primeSignal);
					primeSignal->AddCrossChannelSignalLink (testSignal2);

					compound = new CompoundSignalInfo (testSignal, testSignal2);
					testSignal->SetSignalLink (compound);
					testSignal2->SetSignalLink (compound);
					mSignalLinkList.push_back (compound);
					mNumberOfPullups++;
				}
			}

			for (i=1; i<=mNumberOfChannels; i++) {

				BracketLeft [i] = NULL;
				BracketRight [i] = NULL;
			}
		}
	}

	cout << "Ladder cross channel analysis complete..." << endl;
	OverallList.Clear ();

	delete[] OnDeck;
	delete[] means;
	delete[] isDone;
	delete[] BracketLeft;
	delete[] BracketRight;
		
	return 0;
}


int STRLadderCoreBioComponent :: GridQualityTestSM () {

	//
	//  This is ladder stage 3
	//

	if (Progress < 4)
		return -1;

	//RGString noticeData;
	int status = 0;
	//RGString data;

	//double totalHeight = 0.0;
	//int nPeaks = 0;
	//int N;
	//double averageHeight;

	//double ilsLeft = mLSData->GetFirstAnalyzedMean ();
	//double ilsRight = mLSData->GetLastAnalyzedMean ();
	//int minBP = CoreBioComponent::GetMinBioIDForArtifacts ();
	//double startTime = mLSData->GetTimeForSpecifiedID ((double) minBP);
	//double effectiveLeft = ilsLeft;

	//if (startTime > ilsLeft)
	//	effectiveLeft = startTime;
	
	int i;

	//  Below:  already done in stage 2...
	//for (i=1; i<=mNumberOfChannels; i++) {

	//	if (i != mLaneStandardChannel)
	//		mDataChannels [i]->TestInterlocusCharacteristicsSM (effectiveLeft, ilsLeft, ilsRight, NULL, mLSData, false);
	//}

	//for (i=1; i<=mNumberOfChannels; i++) {

	//	if (i != mLaneStandardChannel) {

	//		totalHeight += mDataChannels [i]->TotalPeakHeight (N);
	//		nPeaks += N;
	//	}
	//}

	//if (nPeaks > 0)
	//	averageHeight = totalHeight / (double) nPeaks;

	//else
	//	averageHeight = 0.0;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->CorrectGridLocusSignalsSM ();
	}

	mLSData->CorrectLaneStandardCrossChannelAnalysisSM ();	// Need this call so that intentionally omitted ILS peaks are not artifacts

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			if (mDataChannels [i]->CleanUpGridLocusSignalsSM () < 0)
				status = -1;
		}
	}

	return status;
}


int STRLadderCoreBioComponent :: GridQualityTestSMPart2 (SmartMessagingComm& comm, int numHigherObjects) {

	//
	//  This is ladder stage 5
	//

	mMarkerSet->ResetLocusList ();
	Locus* nextLocus;
	int status = 0;

	//
	// add all SmartMessageReporters for signals only; that sets up the Retrieve... below to function correctly
	//

	AddAllSmartMessageReportersForSignals (comm, numHigherObjects);	// have to do this here to prepare for next statements:  'retrieving' notices from samples into loci

	while (nextLocus = mMarkerSet->GetNextLocus ())
		nextLocus->RetrieveSmartNoticesFromGridArtifactList (mLSData);

	if (mLSData->FinalTestForCriticalLaneStandardNoticesSM () < 0)
		status = -1;

	return status;
}


int STRLadderCoreBioComponent :: RemoveInterlocusSignalsSM () {

	//
	//  This is ladder stage 5
	//

	double ilsLeft = mLSData->GetFirstAnalyzedMean ();
	double ilsRight = mLSData->GetLastAnalyzedMean ();
	int minBP = CoreBioComponent::GetMinBioIDForArtifacts ();
	double startTime;

	// following if...else clause added 03/13/2015

	if (minBP > 0.0)
		startTime = mLSData->GetTimeForSpecifiedID ((double) minBP);

	else
		startTime = -1.0;

	double effectiveLeft = ilsLeft;

	if (startTime > 0.0)	// modified 03/13/2015
		effectiveLeft = startTime;

	int i;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->RemoveInterlocusSignalsSM (effectiveLeft, ilsLeft, ilsRight, NULL);
	}

	return 0;
}


int STRLadderCoreBioComponent :: WriteXMLGraphicDataSM (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro) {

	RGString fileName = localFileName;
	int begin;
	RGString pResult;

	if (fileName.ExtractLastCharacters (4) == DirectoryManager::GetDataFileTypeWithDot ())
		fileName.ExtractAndRemoveLastCharacters (4);

	RGString fullPath = graphicDirectory + "/" + fileName + ".plt";	
	RGFile outputFile (fullPath, "wt");
	RGTextOutput output (&outputFile, FALSE);

	if (!output.FileIsValid ()) {

		cout << "Could not write graphic file:  " << fullPath << endl; 
		return -1;
	}

	RGString indent = "\t";
	Endl endLine;
	output << "<plt>" << endLine;
	output << intro;
	output << indent << "<filename>" << xmlwriter::EscAscii (fileName, &pResult) << "</filename>" << endLine;
	output << indent << "<start>0</start>" << endLine;

	if (Progress >= 4) {

		// we have an ILS!
		begin = (int) floor (mLSData->GetBeginAnalysisTime () + 0.5);
		output << indent << "<begin>" << begin << "</begin>" << endLine;
	}

	else
		output << indent << "<begin>0</begin>" << endLine;

	output << indent << "<interval>1</interval>" << endLine;
	output << indent << "<end>" << mDataChannels [1]->GetNumberOfSamples () << "</end>" << endLine;
	output << indent << "<kit>" << xmlwriter::EscAscii (mMarkerSet->GetMarkerSetName (), &pResult) << "</kit>" << endLine;
	output << indent << "<ilsChannel>" << mLaneStandardChannel << "</ilsChannel>" << endLine;
	output << indent << "<associatedLadder></associatedLadder>" << endLine;
	RGString indent2 = indent + indent;
	int i;

	for (i=1; i<=mNumberOfChannels; i++) {

		output << indent << "<channel>" << endLine;
		output << indent2 << "<nr>" << i << "</nr>" << endLine;
		output << indent2 << "<minRFU>" << mDataChannels [i]->GetMinimumHeight () << "</minRFU>" << endLine;
		output << indent2 << "<rawPoints>";
		mDataChannels [i]->WriteRawData (output, " ");
		output << "</rawPoints>" << endLine;
		output << indent2 << "<analyzedPoints>";
		mDataChannels [i]->WriteFitData (output, " ");
		output << "</analyzedPoints>" << endLine;
		WriteSmartPeakInfoToXMLForChannel (i, output, indent2, "samplePeak");
		WriteSmartArtifactInfoToXMLForChannel (i, output, indent2);
		output << indent << "</channel>" << endLine;
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->WriteLocusInfoToXML (output, indent);
	}

	output << "</plt>" << endLine;
	outputFile.Flush ();

	return 0;
}


// Smart Message functions******************************************************************************************************************
//******************************************************************************************************************************************


int STRSampleCoreBioComponent :: AnalyzeSampleLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//  OK, so here is what we have to do.  First, we have to test neighbors (stutter/adenylation and
	//  (optional) ratio test.  Then, we test for heterozygous imbalance and homozygosity (using the average, above).  Then, we have 
	//  to gather some data that we'll need, particularly, the average height over all peaks accepted so far, across all 
	//  loci.  Then, we check spikes and bleed-through peaks to see if in-locus or between loci.  Then, we check for correct number of
	//  peaks within each locus, for off-ladder alleles and for inter-locus alleles.  That'll about do it!

	//
	//  This is sample stage 2
	//

	int status = 0;

	for (int i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->AnalyzeSampleLociSM (mLSData, text, ExcelText, msg, print);
	}

	return status;
}


int STRSampleCoreBioComponent :: ResolveAmbiguousInterlocusSignalsSM () {

	for (int j=1; j<=mNumberOfChannels; j++) {

		if (j != mLaneStandardChannel)
			mDataChannels [j]->ResolveAmbiguousInterlocusSignalsSM ();
	}
	
	return 0;
}


int STRSampleCoreBioComponent :: SampleQualityTestSM (GenotypesForAMarkerSet* genotypes) {

	//
	//  This is sample stage 5
	//

	smNoYForAMEL noYForAMEL;

	if (Progress < 4)
		return -1;
	
	int status = 0;

	double ilsLeft = mLSData->GetFirstAnalyzedMean ();
	double ilsRight = mLSData->GetLastAnalyzedMean ();
	int minBP = CoreBioComponent::GetMinBioIDForArtifacts ();
	double startTime;
	
	// following if...else clause added 03/13/2015

	if (minBP > 0.0)
		startTime = mLSData->GetTimeForSpecifiedID ((double) minBP);

	else
		startTime = -1.0;

	double effectiveLeft = ilsLeft;

	if (startTime > 0.0)	// modified 03/13/2015
		effectiveLeft = startTime;

	double totalHeight = 0.0;
	int nPeaks = 0;
	int N;
	double averageHeight;
	int i;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			totalHeight += mDataChannels [i]->TotalPeakHeight (N);
			nPeaks += N;
		}
	}

	if (nPeaks > 0)
		averageHeight = totalHeight / (double) nPeaks;

	else
		averageHeight = 0.0;

	double maxArea = 0.0;
	double minArea = 0.0;
	double maxYLinkedArea = 0.0;
	double minYLinkedArea = 0.0;
	double temp;
	ChannelData* thisChannel;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			thisChannel = mDataChannels [i];

			if (mDataChannels [i]->FinalTestForPeakSizeAndNumberSM (averageHeight, mIsNegativeControl, mIsPositiveControl, genotypes) < 0)
				status = -1;

			temp = thisChannel->GetMaxLocusArea ();

			if (temp > maxArea)
				maxArea = temp;

			temp = thisChannel->GetMinLocusArea ();

			if (minArea == 0.0)
				minArea = temp;

			else if ((temp > 0.0) && (temp < minArea))
				minArea = temp;

			temp = thisChannel->GetMaxYLinkedLocusArea ();

			if (temp > maxYLinkedArea)
				maxYLinkedArea = temp;

			temp = thisChannel->GetMinYLinkedLocusArea ();

			if (minYLinkedArea == 0.0)
				minYLinkedArea = temp;

			else if ((temp > 0.0) && (temp < minYLinkedArea))
				minYLinkedArea = temp;
		}
	}

	if (minYLinkedArea == 0.0)
		mQC.mSampleYLinkedLocusTotalAreaRatioMaxToMin = 0.0;

	else
		mQC.mSampleYLinkedLocusTotalAreaRatioMaxToMin = maxYLinkedArea / minYLinkedArea;

	if (minArea == 0.0)
		mQC.mSampleLocusTotalAreaRatioMaxToMin = 0.0;

	else
		mQC.mSampleLocusTotalAreaRatioMaxToMin = maxArea / minArea;

	if (Locus::IsNoYForAMEL ())
		SetMessageValue (noYForAMEL, true);

	if (mLSData->FinalTestForCriticalLaneStandardNoticesSM () < 0)
		status = -1;

	//if (mIsNegativeControl) {

	//	for (i=1; i<= mNumberOfChannels; i++) {

	//		if (i != mLaneStandardChannel) {

	//			if (mDataChannels [i]->GetMessageValue (unexpectedPeaksInChannel)) {

	//				status = -1;
	//				SetMessageValue (unexpectedPeakInNegativeCtrl, true);
	//				SetMessageValue (negativeControlFailed, true);
	//			}
	//		}
	//	}
	//}

	return status;
}


int STRSampleCoreBioComponent :: SampleQualityTestSMLF () {

	//
	//  This is ladder free sample stage 5
	//

	if (Progress < 4)
		return -1;
	
	int status = 0;
	ChannelData* thisChannel;
	int i;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			thisChannel = mDataChannels [i];

			if (mDataChannels [i]->FinalTestForPeakSizeSMLF (mIsNegativeControl, mIsPositiveControl) < 0)
				status = -1;
		}
	}

	return status;
}


int STRSampleCoreBioComponent :: SignalQualityTestSM () {

	//
	//  This is sample stage 4
	//

	if (Progress < 4)
		return -1;
	
	smNegCtrlChannelHasNoPrimerPeaks noPrimerPeaksInChannel;
//	smNegCtrlChannelsHaveNoPrimerPeaks noPrimerPeaksInSomeChannels;

	double ilsLeft = mLSData->GetFirstAnalyzedMean ();
	double ilsRight = mLSData->GetLastAnalyzedMean ();
	int minBP = CoreBioComponent::GetMinBioIDForArtifacts ();
	double startTime;

	// following if...else clause added 03/13/2015

	if (minBP > 0.0)		
		startTime = mLSData->GetTimeForSpecifiedID ((double) minBP);

	else
		startTime = -1.0;

	double effectiveLeft = ilsLeft;

	if (startTime > 0.0)	// modified 03/13/2015
		effectiveLeft = startTime;

	//Locus* amelLocus;
	//
	//if (!mIsNegativeControl) {

	//	amelLocus = FindLocus ("AMEL");

	//	if (amelLocus != NULL)
	//		amelLocus->TestAmelogeninSM ();
	//}

	int i;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->TestInterlocusCharacteristicsSM (effectiveLeft, ilsLeft, ilsRight, mAssociatedGrid, mLSData, mIsNegativeControl);
	}

	if (mIsNegativeControl) {

		for (i=1; i<=mNumberOfChannels; i++) {

			if (i != mLaneStandardChannel) {

				if (!mDataChannels [i]->HasPrimerPeaks (mLSData)) {

					mDataChannels [i]->SetMessageValue (noPrimerPeaksInChannel, true);
//					SetMessageValue (noPrimerPeaksInSomeChannels, true);
				}
			}
		}
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->CorrectCrossChannelAnalysesSM (mIsNegativeControl);
	}

	mLSData->CorrectLaneStandardCrossChannelAnalysisSM ();	// Need this call so that intentionally omitted ILS peaks are not artifacts

	// clean up locus lists here

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			mDataChannels [i]->CleanUpLocusSignalListsSM ();
			mDataChannels [i]->TestAllelesAgainstOverloadThresholdSM ();
		}
	}

	return 0;
}


int STRSampleCoreBioComponent :: FitAllSampleCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is sample stage 1
	//

	int status;
	int defaultWindow = TracePrequalification::GetDefaultWindowWidth ();
	double defaultThreshold = TracePrequalification::GetDefaultNoiseThreshold ();
//	int currentWindow = TracePrequalification::GetWindowWidth ();
	TracePrequalification::SetWindowWidth (defaultWindow);
	TracePrequalification::SetNoiseThreshold (defaultThreshold);
	int currentWindow = defaultWindow;
	int recommendedWindow;
	double recommendedThreshold;
	RGString notice;
	smILSFailed ilsFailed;
	smNormalizeRawDataRelativeToBaselinePreset normalizeRawData;
	smEnableRawDataFilterForNormalizationPreset enableFilteringForNormalization;

	ChannelData::SetBeginAnalysisTime (-1.0);
	status = FitLaneStandardCharacteristicsSM (text, ExcelText, msg, print);

	if (status < 0) {

		// This should not come here...FitLaneStandardCharacteristics only returns 0?
		return status;
	}

//	mLSData->ClearAllPeaksBelowAnalysisThreshold ();

	status = AnalyzeLaneStandardChannelSM (text, ExcelText, msg, print);

	if (status < 0) {

		// ErrorString already populated; cannot go on because need a healthy internal lane standard to do anything else
		SetMessageValue (ilsFailed, true);
		cout << "ILS failed with status = " << status << "\n";
		notice = ":  Could not analyze ladder ILS";
		AppendDataForSmartMessage (ilsFailed, notice);
		return -2;
	}

//	Progress = 3;
	double ratio = mDataChannels [mLaneStandardChannel]->GetMeasurementRatio ();
	int intRatio = (int) floor (ratio + 0.5);
	recommendedWindow = intRatio - 1;

	if (recommendedWindow < 4)
		recommendedWindow = 4;

	else if (recommendedWindow > defaultWindow)
		recommendedWindow = defaultWindow;

//	if (recommendedWindow < currentWindow) {

		TracePrequalification::SetWindowWidth (recommendedWindow);
		recommendedThreshold = ((double)recommendedWindow) * defaultThreshold / ((double)defaultWindow);
		TracePrequalification::SetNoiseThreshold (recommendedThreshold);
//	}

	cout << "Window width = " << TracePrequalification::GetWindowWidth () << endl;
	cout << "Noise threshold = " << TracePrequalification::GetNoiseThreshold () << endl;

	if (GetMessageValue (normalizeRawData) && GetMessageValue (enableFilteringForNormalization))
		CreateAndSubstituteFilteredDataSignalForRawDataNonILS ();

	status = FitNonLaneStandardCharacteristicsSM (text, ExcelText, msg, print);
	//cout << "Fitting all non-lane standard neg. peaks (2)" << endl;
	//if (FitNonLaneStandardNegativeCharacteristicsSM (text, ExcelText, msg, print) < 0)
	//	cout << "Negative characteristics did not analyze correctly" << endl;

	FitNonLaneStandardNegativeCharacteristicsSM (text, ExcelText, msg, print);
	//cout << "Done fitting all non-lane standard neg. peaks" << endl;
	mDataChannels [mLaneStandardChannel]->FitAllNegativeCharacteristicsSM (text, ExcelText, msg, print);

	if (status < 0) {

		// This should not come here...FitNonLaneStandardCharacteristics only returns 0?
		return status;
	}

	//cout << "Finished fitting all characteristics" << endl;
	return status;
}


int STRSampleCoreBioComponent :: RemoveInterlocusSignalsSM () {

	//
	//  This is sample stage 5
	//

	if (Progress < 4)
		return -1;
	
	double ilsLeft = mLSData->GetFirstAnalyzedMean ();
	double ilsRight = mLSData->GetLastAnalyzedMean ();
	int minBP = CoreBioComponent::GetMinBioIDForArtifacts ();
	double startTime;

	// following if...else clause added 03/13/2015

	if (minBP > 0.0)
		startTime = mLSData->GetTimeForSpecifiedID ((double) minBP);

	else
		startTime = -1.0;

	double effectiveLeft = ilsLeft;

	if (startTime > 0.0)	// modified 03/13/2015
		effectiveLeft = startTime;

	int i;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->RemoveInterlocusSignalsSM (effectiveLeft, ilsLeft, ilsRight, mAssociatedGrid);
	}

	return 0;
}


//******************************************************************************************************************************************


