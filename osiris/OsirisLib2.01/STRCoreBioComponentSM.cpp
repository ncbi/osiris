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


// Smart Message related*********************************************************************************************************************************


int STRCoreBioComponent :: AnalyzeGridSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	return -10;
}


int STRCoreBioComponent :: AnalyzeCrossChannelSM () {

	//
	//  This is sample stage 1
	//

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
	double mWidthToleranceForSpike = 1.0;  // Width must be less than this width to qualify as a spike
	double nSigmaForCraters = 2.0;
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
	double saturationArtifactTolerance;
	double deltaTime;
	DataSignal* newCraterSignal;
	ChannelData* nextChannel;
	RGDList* nextList;

	smPullUp pullup;
	smPrimaryInterchannelLink primaryLink;
	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smSpike spike;
	smBelowMinRFU belowMinRFU;
//	PullUpFound pullupNotice;
//	PullUpPrimaryChannel primaryPullupNotice;

	ParameterServer* pServer = new ParameterServer;
	double measurementResolution = pServer->GetInterSampleSpacing ();
	double minPullupThreshold = 0.5 * measurementResolution;
	delete pServer;

	RGDList NewCraterSignalsToAdd;
	RGDList CraterSignalsToRemove;
	InterchannelLinkage* iChannel;

	double peak1;
	double peak2;
	int nInterchannelLinks = 0;

	for (i=1; i<=mNumberOfChannels; i++) {

		TempList [i] = new RGDList;
	}

	for (i=1; i<=mNumberOfChannels; i++)
		mDataChannels [i]->ResetCompleteIterator ();

	// Now copy from mDataChannels PreliminaryCurveList to TempList (while finding maxSignalPeak??)

	for (i=1; i<= mNumberOfChannels; i++) {

		prevSignal = NULL;
		nextChannel = mDataChannels [i];
		nextList = TempList [i];

		while (nextSignal = nextChannel->GetNextPreliminaryCurve ()) {
//		while (nextSignal = nextChannel->GetNextCompleteCurve ()) {

			if (nextSignal->GetMessageValue (belowMinRFU))
				continue;

			if (prevSignal != NULL) {

				// test if prevSignal and nextSignal are "close".  If so, don't move them and create intermediate "crater" peak.  If
				// not, store prevSignal and overwrite with nextSignal

				saturationArtifactTolerance = 1.1 * (prevSignal->GetStandardDeviation () + nextSignal->GetStandardDeviation ());
				deltaTime = nextSignal->GetMean () - prevSignal->GetMean ();
				peak1 = prevSignal->Peak ();
				peak2 = nextSignal->Peak ();

				if ((deltaTime > saturationArtifactTolerance) || (peak1 < 0.7 * peak2) || (peak2 < 0.7 * peak1)) {

					nextList->Append (prevSignal);
					prevSignal = nextSignal;
				}

				else {

					newCraterSignal = new CraterSignal (prevSignal, nextSignal);
					newCraterSignal->SetSaturationArtifact (true);
					newCraterSignal->SetMessageValue (crater, true);
					prevSignal->SetMessageValue (craterSidePeak, true);
					nextSignal->SetMessageValue (craterSidePeak, true);

		//			nextChannel->InsertIntoArtifactList (newCraterSignal);	// Need this??????????????????????????????????????????????????
		//			nextChannel->InsertIntoArtifactList (prevSignal);	// Need this??????????????????????????????????????????????????
		//			nextChannel->InsertIntoArtifactList (nextSignal);	// Need this??????????????????????????????????????????????????

					NewCraterSignalsToAdd.Append (newCraterSignal);
					prevSignal = NULL;
				}
			}

			else
				prevSignal = nextSignal;
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

		if (mTimeTolerance < minPullupThreshold)
			mTimeTolerance = minPullupThreshold;

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

			if (nextSignal == NULL)
				break;

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
				iChannel = new STRInterchannelLinkage ();
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

	status = FitNonLaneStandardCharacteristicsSM (text, ExcelText, msg, print);

	if (status < 0) {

		// This should not come here...FitNonLaneStandardCharacteristics only returns 0?
		return status;
	}

	AnalyzeCrossChannelSM ();

	//
	// Call function to flag all peaks outside lane standard and above user-specified error-reporting threshold
	//

	status = 0;

	for (int j=1; j<=mNumberOfChannels; j++) {

//		if (j != mLaneStandardChannel) {

			if (mDataChannels [j]->SetAllApproximateIDs (mLSData) < 0)
				status = -1;
//		}
	}

	if (status < 0) {

		notice << "LADDER BIOCOMPONENT COULD NOT UTILIZE INTERNAL LANE STANDARD.  Skipping...";
		cout << notice << endl;
		SetMessageValue (ilsFailed, true);
		notice = ":  Could not create ILS time to base pairs transform";
		AppendDataForSmartMessage (ilsFailed, notice);
		return -5;
	}

	RemoveAllSignalsOutsideLaneStandardSM ();  // Moved to follow setting of all approximate bioID's, above, or nothing is saved in SmartPeaks!
	Progress = 4;

	if (AssignCharacteristicsToLociSM () < 0) {

		// ErrorString already populated; go on and print out what info there is
		status = -3;
	}

	if (status == 0)
		Progress = 5;

	return status;
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

						testSignal->SetMessageValue (spike, true);
						testSignal->AddNoticeToList (1, "", "Suspected spike");
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

//	mLSData->CorrectLaneStandardCrossChannelAnalysisSM ();

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
	double startTime = mLSData->GetTimeForSpecifiedID ((double) minBP);
	double effectiveLeft = ilsLeft;

	if (startTime > ilsLeft)
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
	double startTime = mLSData->GetTimeForSpecifiedID ((double) minBP);
	double effectiveLeft = ilsLeft;

	if (startTime > ilsLeft)
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

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			if (mDataChannels [i]->FinalTestForPeakSizeAndNumberSM (averageHeight, mIsNegativeControl, mIsPositiveControl, genotypes) < 0)
				status = -1;
		}
	}

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
	double startTime = mLSData->GetTimeForSpecifiedID ((double) minBP);
	double effectiveLeft = ilsLeft;

	if (startTime > ilsLeft)
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

//	mLSData->CorrectLaneStandardCrossChannelAnalysisSM ();

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

	status = FitNonLaneStandardCharacteristicsSM (text, ExcelText, msg, print);
	//cout << "Fitting all non-lane standard neg. peaks (2)" << endl;
	FitNonLaneStandardNegativeCharacteristicsSM (text, ExcelText, msg, print);
	//cout << "Done fitting all non-lane standard neg. peaks" << endl;

	if (status < 0) {

		// This should not come here...FitNonLaneStandardCharacteristics only returns 0?
		return status;
	}

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
	double startTime = mLSData->GetTimeForSpecifiedID ((double) minBP);
	double effectiveLeft = ilsLeft;

	if (startTime > ilsLeft)
		effectiveLeft = startTime;

	int i;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->RemoveInterlocusSignalsSM (effectiveLeft, ilsLeft, ilsRight, mAssociatedGrid);
	}

	return 0;
}


//******************************************************************************************************************************************


