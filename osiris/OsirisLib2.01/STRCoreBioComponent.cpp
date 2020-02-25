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
*  FileName: STRCoreBioComponent.cpp
*  Author:   Robert Goor
*
*/
//
//  class STRCoreBioComponent, a subclass of CoreBioComponent, and parent class to STRLadderBioComponent and STRSampleBioComponent
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


int MinimumIndex (double* array, int size) {

	double minValue = array [1];
	int minIndex = 1;

	for (int i=2; i<=size; i++) {

		if (array [i] < minValue) {

			minValue = array [i];
			minIndex = i;
		}
	}

	return minIndex;
}



ABSTRACT_DEFINITION (STRCoreBioComponent)
PERSISTENT_DEFINITION (STRLadderCoreBioComponent, _STRLADDERCOREBIOCOMPONENT_, "STRLadderCoreBioComponent")
PERSISTENT_DEFINITION (STRSampleCoreBioComponent, _STRSAMPLECOREBIOCOMPONENT_, "STRSampleCoreBioComponent")



STRCoreBioComponent :: STRCoreBioComponent () : CoreBioComponent (), mNumberOfPullups (0), mNumberOfPrimaryPullups (0) {

}


STRCoreBioComponent :: STRCoreBioComponent (const RGString& name) : CoreBioComponent (name), mNumberOfPullups (0), 
mNumberOfPrimaryPullups (0) {

}


STRCoreBioComponent :: STRCoreBioComponent (const STRCoreBioComponent& str) : CoreBioComponent ((CoreBioComponent&)str), 
mNumberOfPullups (0), mNumberOfPrimaryPullups (0) {

}


STRCoreBioComponent :: ~STRCoreBioComponent () {

}


/*
int STRCoreBioComponent :: Initialize (SampleData& fileData, PopulationCollection* collection, const RGString& markerSetName) {

	int status = CoreBioComponent::Initialize (fileData, collection, markerSetName);

	if (status < 0)
		return -1;

	return 0;
}*/


int STRCoreBioComponent :: AnalyzeGrid (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	return -10;
}


ChannelData* STRCoreBioComponent :: GetNewLaneStandardChannel (int channel, LaneStandard* inputLS) {

	return new STRLaneStandardChannelData (channel, inputLS);
}


ChannelData* STRCoreBioComponent :: GetNewDataChannel (int channel, LaneStandard* inputLS) {

	return new STRSampleChannelData (channel, inputLS);
}


ChannelData* STRCoreBioComponent :: GetNewGridDataChannel (int channel, LaneStandard* inputLS) {

	return new STRLadderChannelData (channel, inputLS);
}


int STRCoreBioComponent :: AnalyzeCrossChannel () {

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
	Notice* newNotice;
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
//	CompoundSignalInfo* compound;
	int LookCount;
	mNumberOfPullups = 0;
	mNumberOfPrimaryPullups = 0;
	int i;
	RGDList** TempList = new RGDList* [mNumberOfChannels + 1];
//	double maxSignalPeak;
	DataSignal* prevSignal;
	double saturationArtifactTolerance;
	double deltaTime;
	DataSignal* newCraterSignal;
	ChannelData* nextChannel;
	RGDList* nextList;
	PullUpFound pullupNotice;
	PullUpPrimaryChannel primaryPullupNotice;

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

//		maxSignalPeak = 0.0;
		prevSignal = NULL;
		nextChannel = mDataChannels [i];
		nextList = TempList [i];

		while (nextSignal = nextChannel->GetNextPreliminaryCurve ()) {
//		while (nextSignal = nextChannel->GetNextCompleteCurve ()) {

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

					newNotice = new Crater;
					newCraterSignal->AddNoticeToList (newNotice);

					newNotice = new Crater;
					prevSignal->AddNoticeToList (newNotice);

					newNotice = new Crater;
					nextSignal->AddNoticeToList (newNotice);

					nextChannel->InsertIntoArtifactList (newCraterSignal);
					nextChannel->InsertIntoArtifactList (prevSignal);
					nextChannel->InsertIntoArtifactList (nextSignal);

					//compound = new CompoundSignalInfo (prevSignal, nextSignal);
					//prevSignal->SetSignalLink (compound);
					//nextSignal->SetSignalLink (compound);
					//mSignalLinkList.push_back (compound);

					NewCraterSignalsToAdd.Append (newCraterSignal);
//					CraterSignalsToRemove.Append (prevSignal);   // For now, we'll leave these signals in PreliminaryCurveList
//					CraterSignalsToRemove.Append (nextSignal);   // For now, we'll leave these signals in PreliminaryCurveList

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

		//while (nextSignal = (DataSignal*) CraterSignalsToRemove.GetFirst ()) {   // For now, we'll leave these signals in PreliminaryCurveList

		//	nextChannel->RemovePreliminaryCurveReference (nextSignal);
		//}
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

//		if (nextSignal->DontLook ())
//			continue;
		
//		while (nextSignal2 = (DataSignal*) it ()) {
//
//			if (!nextSignal2->DontLook ())
//				break;
//		}

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

//			while (nextSignal = (DataSignal*) it ()) {
//
//				if (!nextSignal->DontLook ())
//					break;
//			}

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

				if (testSignal != NULL) {

					//if (testSignal->DontLook ())
					//	OnDeck [i] = NULL;

					//else
						LookCount++;
				}
			}

			if (LookCount > 1) {

				for (i=1; i<=mNumberOfChannels; i++) {

					testSignal = OnDeck [i];

					if (testSignal != NULL) {

//						mDataChannels [i]->RemovePreliminaryCurveReference (testSignal);
						mDataChannels [i]->InsertIntoArtifactList (testSignal);
						newNotice = new SpikeFound;
						testSignal->AddNoticeToList (newNotice);
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
			// Look for possible crater...but first eliminate signals that should be beneath the radar (or not?)

			LookCount = 0;

			for (i=1; i<=mNumberOfChannels; i++) {

				testSignal = OnDeck [i];

				if (testSignal != NULL) {

					//if (testSignal->DontLook ())
					//	OnDeck [i] = NULL;

					//else
						LookCount++;
				}
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
//							break;
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
					
//					if (i != maxIndex) {

//					mDataChannels [i]->RemovePreliminaryCurveReference (testSignal);
					mDataChannels [i]->InsertIntoArtifactList (testSignal);
					newNotice = new PullUpFound;
					testSignal->AddNoticeToList (newNotice);
					testSignal->AddNoticeToList (1, "", "Suspected pull-up");
					mNumberOfPullups++;
//					testSignal->SetPrimaryCrossChannelSignalLink (primeSignal);
//					primeSignal->AddCrossChannelSignalLink (testSignal);
//					}

					//else {

					//	newNotice = new PullUpPrimaryChannel;
					//	testSignal->AddNoticeToList (newNotice);
					//	testSignal->AddNoticeToList (1, "", "Suspected primary pull-up");
					//	mDataChannels [maxIndex]->InsertIntoArtifactList (testSignal);
					//	mNumberOfPrimaryPullups++;
					//}
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

					//   Create CraterSignal and set it all up
					newNotice = new Crater;
					testSignal->AddNoticeToList (newNotice);
			//		newNotice = new PullUpFound;
			//		testSignal->AddNoticeToList (newNotice);
					mDataChannels [i]->InsertIntoArtifactList (testSignal);
					testSignal->AddNoticeToList (1, "", "Suspected pull-up induced crater with next peak");
//					testSignal->SetPrimaryCrossChannelSignalLink (primeSignal);
//					primeSignal->AddCrossChannelSignalLink (testSignal);

					newNotice = new Crater;
					testSignal2->AddNoticeToList (newNotice);
			//		newNotice = new PullUpFound;
			//		testSignal2->AddNoticeToList (newNotice);
					mDataChannels [i]->InsertIntoArtifactList (testSignal2);
					testSignal2->AddNoticeToList (1, "", "Suspected pull-up induced crater with previous peak");
//					testSignal2->SetPrimaryCrossChannelSignalLink (primeSignal);
//					primeSignal->AddCrossChannelSignalLink (testSignal2);

					//compound = new CompoundSignalInfo (testSignal, testSignal2);
					//testSignal->SetSignalLink (compound);
					//testSignal2->SetSignalLink (compound);
					//mSignalLinkList.push_back (compound);
					mNumberOfPullups++;

					nextSignal = new CraterSignal (testSignal, testSignal2);
					newNotice = new Crater;
					nextSignal->AddNoticeToList (newNotice);
					newNotice = new PullUpFound;
					nextSignal->AddNoticeToList (newNotice);
					iChannel->AddDataSignal (nextSignal);
					nextSignal->SetInterchannelLink (iChannel);
//					nextSignal->SetCannotBePrimary (true);

					mDataChannels [i]->InsertIntoArtifactList (nextSignal);
					mDataChannels [i]->InsertIntoCompleteCurveList (nextSignal);
					mDataChannels [i]->InsertIntoPreliminaryCurveList (nextSignal);
//					mDataChannels [i]->RemovePreliminaryCurveReference (testSignal);   // For now, we'll leave these signals in PreliminaryCurveList
//					mDataChannels [i]->RemovePreliminaryCurveReference (testSignal2);   // For now, we'll leave these signals in PreliminaryCurveList
				}
			}

			if (nInterchannelLinks > 0)
				iChannel->RecalculatePrimarySignal (&pullupNotice, &primaryPullupNotice);

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


int STRCoreBioComponent :: WriteXMLGraphicData (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro) {

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
		mDataChannels [i]->WriteFitData (output, " ", false);
		output << "</analyzedPoints>" << endLine;
		
		if ((mAssociatedGrid != NULL) && (i != mLaneStandardChannel)) {

			output << indent2 << "<ladderPoints>";
			mAssociatedGrid->WriteFitDataForChannel (i, output, " ", mDataChannels [i]);
			output << "</ladderPoints>" << endLine;
		}

		WritePeakInfoToXMLForChannel (i, output, indent2, "samplePeak");
		WriteArtifactInfoToXMLForChannel (i, output, indent2);

		if ((mAssociatedGrid != NULL) && (i != mLaneStandardChannel))
			mAssociatedGrid->WritePeakInfoToXMLForChannel (i, output, indent2, "ladderPeak");

		output << indent << "</channel>" << endLine;
	}

	if (mAssociatedGrid != NULL)
		mAssociatedGrid->WriteLocusInfoToXML (output, indent);

	output << "</plt>" << endLine;
	outputFile.Flush ();

	return 0;
}





STRLadderCoreBioComponent :: STRLadderCoreBioComponent () : STRCoreBioComponent () {

}


STRLadderCoreBioComponent :: STRLadderCoreBioComponent (const RGString& name) : STRCoreBioComponent (name) {

}


STRLadderCoreBioComponent :: STRLadderCoreBioComponent (const STRLadderCoreBioComponent& ladder) : STRCoreBioComponent (ladder) {

}


STRLadderCoreBioComponent :: STRLadderCoreBioComponent (const STRLadderCoreBioComponent& ladder, CoordinateTransform* trans) :
STRCoreBioComponent (ladder, trans) {

}


STRLadderCoreBioComponent :: ~STRLadderCoreBioComponent () {

}


int STRLadderCoreBioComponent :: AnalyzeGridLoci (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	int status = 0;
	
	for (int i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			if (mDataChannels [i]->AnalyzeGridLoci (text, ExcelText, msg, print) < 0) {

				ErrorString << mDataChannels [i]->GetError ();
				status = -1;
			}
		}
	}

	return status;
}


int STRLadderCoreBioComponent :: AnalyzeGrid (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	int status;
	int defaultWindow = TracePrequalification::GetDefaultWindowWidth ();
	double defaultThreshold = TracePrequalification::GetDefaultNoiseThreshold ();
	int currentWindow = TracePrequalification::GetWindowWidth ();
	int recommendedWindow;
	double recommendedThreshold;
	RGString notice;
	Notice* newNotice;
	
	//status = FitAllCharacteristics (text, ExcelText, msg, print);

	//if (status < 0) {

	//	// This should not come here...FitAllCharacteristics only returns 0
	//	return status;
	//}

	//Progress = 3;
	//AnalyzeCrossChannel ();

	//if (AnalyzeLaneStandardChannel (text, ExcelText, msg, print) < 0) {

	//	// ErrorString already populated; cannot go on because need a healthy internal lane standard to do anything else
	//	return -2;
	//}

	status = FitLaneStandardCharacteristics (text, ExcelText, msg, print);

	if (status < 0) {

		// This should not come here...FitLaneStandardCharacteristics only returns 0?
		return status;
	}

	if (AnalyzeLaneStandardChannel (text, ExcelText, msg, print) < 0) {

		// ErrorString already populated; cannot go on because need a healthy internal lane standard to do anything else
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

	status = FitNonLaneStandardCharacteristics (text, ExcelText, msg, print);

	if (status < 0) {

		// This should not come here...FitNonLaneStandardCharacteristics only returns 0?
		return status;
	}

	AnalyzeCrossChannel ();
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
		newNotice = new ILSReqReview;
		notice = "Could not create ILS time to base pairs transform";
		newNotice->AddDataItem (notice);
		AddNoticeToList (newNotice);
		return -5;
	}

	Progress = 4;

	if (AssignCharacteristicsToLoci () < 0) {

		// ErrorString already populated; go on and print out what info there is
		status = -3;
	}

	if (status == 0)
		Progress = 5;

	if (AnalyzeGridLoci (text, ExcelText, msg, print) < 0) {

		// ErrorString already populated; if status is already negative, we'll come here anyway
		return -4;
	}

	if (status == 0)
		Progress = 6;

	return status;
}



int STRLadderCoreBioComponent :: GridQualityTest () {

/*	ParameterServer* pServer = new ParameterServer;
	int MaxPullups = pServer->GetMaxPullups ();
//	int MaxPrimaryPullups = pServer->GetMaxPullups ();
	int MaxStutter = pServer->GetMaxStutter ();
	int MaxAdenylation = pServer->GetMaxAdenylation ();
	int MaxOLAlleles = pServer->GetMaxOLAlleles ();
	delete pServer;
*/	

	if (Progress < 4)
		return -1;

	RGString noticeData;
	int status = 0;
//	UnexpectedNumberOfPeaks target;
	RGString data;

	double totalHeight = 0.0;
	int nPeaks = 0;
	int N;
	double averageHeight;

	double ilsLeft = mLSData->GetFirstAnalyzedMean ();
	double ilsRight = mLSData->GetLastAnalyzedMean ();
	int minBP = CoreBioComponent::GetMinBioIDForArtifacts ();
	double startTime;
	
	//  Following if...else clause added 03/13/2015

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
			mDataChannels [i]->TestInterlocusCharacteristics (effectiveLeft, ilsLeft, ilsRight, NULL, mLSData, false);
	}

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

		if (i != mLaneStandardChannel)
			mDataChannels [i]->CorrectGridLocusSignals ();
	}

	mLSData->CorrectLaneStandardCrossChannelAnalysis ();

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			if (mDataChannels [i]->CleanUpGridLocusSignals () < 0)
				status = -1;
		}
	}

	mMarkerSet->ResetLocusList ();
	Locus* nextLocus;

	while (nextLocus = mMarkerSet->GetNextLocus ())
		nextLocus->RetrieveNoticesFromGridArtifactList (mLSData);

	if (mLSData->FinalTestForCriticalLaneStandardNotices () < 0)
		status = -1;

	// Need something new here, which corrects for pullups that are actually alleles!

	// eliminated "CleanUpSignalLists" because that is for samples only...it deals with
	// off-ladder alleles and max residual errors, neither of which relate to ladders

/*
//	int NumberOfPrimaryPullups = 0;
	int NumberOfPullups = 0;
//	PullUpPrimaryChannel primaryTarget;
	PullUpFound pullupTarget;

	for (i=1; i<=mNumberOfChannels; i++) {

//		NumberOfPrimaryPullups += mDataChannels [i]->CountSignalsWithNotice (&primaryTarget, mLSData);
		NumberOfPullups += mDataChannels [i]->CountSignalsWithNotice (&pullupTarget, mLSData);
	}

	newNotice = new NumberOfPullUpsInSample;
	noticeData.Convert (NumberOfPullups, 10);
	newNotice->AddDataItem (noticeData);
	AddNoticeToList (newNotice);

	if ((MaxPullups > 0) && (NumberOfPullups > MaxPullups)) {

		newNotice = new NumberOfPullUpPeaksExceedsThreshold;
		noticeData << " (Max = " << MaxPullups << ")";
		newNotice->AddDataItem (noticeData);
		AddNoticeToList (newNotice);
		status = -1;
	}

	int nStutter = 0;
	int nAdenylation = 0;

	StutterFound stutterTarget;
	AdenylationFound adenylationTarget;

	for (i=1; i<=mNumberOfChannels; i++) {

		nStutter += mDataChannels [i]->CountSignalsWithNotice (&stutterTarget, mLSData);
		nAdenylation += mDataChannels [i]->CountSignalsWithNotice (&adenylationTarget, mLSData);
	}

	newNotice = new NumberOfStutterPeaksInSample;
	noticeData.Convert (nStutter, 10);
	newNotice->AddDataItem (noticeData);
	AddNoticeToList (newNotice);

	if ((MaxStutter > 0) && (nStutter > MaxStutter)) {

		newNotice = new NumberOfStutterPeaksExceedsThreshold;
		noticeData << " (Max = " << MaxStutter << ")";
		newNotice->AddDataItem (noticeData);
		AddNoticeToList (newNotice);
		status = -1;
	}

	newNotice = new NumberOfAdenylationsInSample;
	noticeData.Convert (nAdenylation, 10);
	newNotice->AddDataItem (noticeData);
	AddNoticeToList (newNotice);

	if ((MaxAdenylation > 0) && (nAdenylation > MaxAdenylation)) {

		newNotice = new NumberOfAdenylationPeaksExceedsThreshold;
		noticeData << " (Max = " << MaxAdenylation << ")";
		newNotice->AddDataItem (noticeData);
		AddNoticeToList (newNotice);
		status = -1;
	}
*/

	return status;
}


int STRLadderCoreBioComponent :: AnalyzeCrossChannel () {

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
	Notice* newNotice;
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

//		if (nextSignal->DontLook ())
//			continue;
		
//		while (nextSignal2 = (DataSignal*) it ()) {
//
//			if (!nextSignal2->DontLook ())
//				break;
//		}

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

//			while (nextSignal = (DataSignal*) it ()) {
//
//				if (!nextSignal->DontLook ())
//					break;
//			}

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

//						mDataChannels [i]->RemovePreliminaryCurveReference (testSignal);
						mDataChannels [i]->InsertIntoArtifactList (testSignal);
						newNotice = new SpikeFound;
						testSignal->AddNoticeToList (newNotice);
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

	//					mDataChannels [i]->RemovePreliminaryCurveReference (testSignal);
						mDataChannels [i]->InsertIntoArtifactList (testSignal);
						newNotice = new PullUpFound;
						testSignal->AddNoticeToList (newNotice);
						testSignal->AddNoticeToList (1, "", "Suspected pull-up");
						mNumberOfPullups++;
						testSignal->SetPrimaryCrossChannelSignalLink (primeSignal);
						primeSignal->AddCrossChannelSignalLink (testSignal);
					}

					else {

						newNotice = new PullUpPrimaryChannel;
						testSignal->AddNoticeToList (newNotice);
						testSignal->AddNoticeToList (1, "", "Suspected primary pull-up");
						mDataChannels [maxIndex]->InsertIntoArtifactList (testSignal);
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

					newNotice = new CraterInducedByPullUp;
					testSignal->AddNoticeToList (newNotice);
					mDataChannels [i]->InsertIntoArtifactList (testSignal);
					testSignal->AddNoticeToList (1, "", "Suspected pull-up induced crater with next peak");
					testSignal->SetPrimaryCrossChannelSignalLink (primeSignal);
					primeSignal->AddCrossChannelSignalLink (testSignal);

					newNotice = new CraterInducedByPullUp;
					testSignal2->AddNoticeToList (newNotice);
					mDataChannels [i]->InsertIntoArtifactList (testSignal2);
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


int STRLadderCoreBioComponent :: WriteXMLGraphicData (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro) {

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
		mDataChannels [i]->WriteFitData (output, " ", false);
		output << "</analyzedPoints>" << endLine;
		WritePeakInfoToXMLForChannel (i, output, indent2, "samplePeak");
		WriteArtifactInfoToXMLForChannel (i, output, indent2);
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


int STRLadderCoreBioComponent::GetMinimumILSBPForLoci () {

	//  This function returns the least ILS BP of any locus in the ladder
	//  Phase 5 for ladders only

	int i;
	bool firstChannel = true;
	double leastBP = 0.0;
	double temp;
	int leastBPInt = 0;

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			temp = mDataChannels [i]->GetLeastILSBPForLadderLoci ();

			if (firstChannel) {

				leastBP = temp;
				firstChannel = false;
			}

			else {

				if (temp < leastBP)
					leastBP = temp;
			}
		}
	}

	return ((int)floor (leastBP) - 1);
}


CoreBioComponent* STRLadderCoreBioComponent :: CreateNewTransformedBioComponent (const CoreBioComponent& bc, CoordinateTransform* trans) {

	return new STRLadderCoreBioComponent ((const STRLadderCoreBioComponent&) bc, trans);
}




STRSampleCoreBioComponent :: STRSampleCoreBioComponent () : STRCoreBioComponent () {

}


STRSampleCoreBioComponent :: STRSampleCoreBioComponent (const RGString& name) : STRCoreBioComponent (name) {

}



STRSampleCoreBioComponent :: STRSampleCoreBioComponent (const STRSampleCoreBioComponent& sample) : STRCoreBioComponent (sample) {

}


STRSampleCoreBioComponent :: STRSampleCoreBioComponent (const STRSampleCoreBioComponent& sample, CoordinateTransform* trans) :
STRCoreBioComponent (sample, trans) {

}


STRSampleCoreBioComponent :: ~STRSampleCoreBioComponent () {

}


int STRSampleCoreBioComponent :: AnalyzeSampleLoci (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//  OK, so here is what we have to do.  First, we have to test neighbors (stutter/adenylation and
	//  (optional) ratio test.  Then, we test for heterozygous imbalance and homozygosity (using the average, above).  Then, we have 
	//  to gather some data that we'll need, particularly, the average height over all peaks accepted so far, across all 
	//  loci.  Then, we check spikes and bleed-through peaks to see if in-locus or between loci.  Then, we check for correct number of
	//  peaks within each locus, for off-ladder alleles and for inter-locus alleles.  That'll about do it!

//	double totalHeight = 0.0;
//	int nPeaks = 0;
//	int N;
//	double averageHeight;
	int status = 0;
//	Notice* newNotice;

	for (int i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			mDataChannels [i]->AnalyzeSampleLoci (mLSData, text, ExcelText, msg, print);
		}
	}

/*	for (i=1; i<=mNumberOfChannels; i++) {

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

			if (mDataChannels [i]->FinalTestForPeakSizeAndNumber (averageHeight, ExcelText, mIsNegativeControl, mIsPositiveControl, pGenotypes) < 0)
				status = -1;
		}
	}*/

/*	if (status < 0) {

		newNotice = new SampleReqReview;

		if (!IsNoticeInList (newNotice))
			AddNoticeToList (newNotice);

		else
			delete newNotice;
	}*/

	return status;
}



int STRSampleCoreBioComponent :: ResolveAmbiguousInterlocusSignals () {

	for (int j=1; j<=mNumberOfChannels; j++) {

		if (j != mLaneStandardChannel)
			mDataChannels [j]->ResolveAmbiguousInterlocusSignals ();
	}
	
	return 0;
}


int STRSampleCoreBioComponent :: SampleQualityTest (GenotypesForAMarkerSet* genotypes) {

	if (Progress < 4)
		return -1;
	
	ParameterServer* pServer = new ParameterServer;
	int MaxPullups = pServer->GetMaxPullups ();
//	int MaxPrimaryPullups = pServer->GetMaxPullups ();
	int MaxStutter = pServer->GetMaxStutter ();
	int MaxAdenylation = pServer->GetMaxAdenylation ();
	int MaxOLAlleles = pServer->GetMaxOLAlleles ();
	int MaxExcessiveResiduals = pServer->GetMaxExcessiveResiduals ();
	delete pServer;
	
	Notice* newNotice;
	RGString noticeData;
	int status = 0;
	UnexpectedNumberOfPeaks target;
	RGString data;
	NegCtrlReqReview negCtlTarget;
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

	Locus* amelLocus;
	
	if (!mIsNegativeControl) {

		amelLocus = FindLocus ("AMEL");

		if (amelLocus != NULL)
			amelLocus->TestAmelogenin ();
	}
  int i;
	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->TestInterlocusCharacteristics (effectiveLeft, ilsLeft, ilsRight, mAssociatedGrid, mLSData, mIsNegativeControl);
	}

	bool addedPrimerPeaksMsg = false;

	if (mIsNegativeControl) {

		for (i=1; i<=mNumberOfChannels; i++) {

			if (mDataChannels [i]->GetNumberOfSignals () == 0) {

				newNotice = new NoPrimerPeaksInNegativeControlChannel;
				mDataChannels [i]->AddNoticeToList (newNotice);

				if (!addedPrimerPeaksMsg) {

					addedPrimerPeaksMsg = true;
					newNotice = new NoPrimerPeaksInOneOrMoreNegControlChannel;
					AddNoticeToList (newNotice);
				}
			}
		}
	}

//	mDataChannels [mLaneStandardChannel]->RemoveInterlocusSignals ();

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

		if (i != mLaneStandardChannel)
			mDataChannels [i]->CorrectCrossChannelAnalyses (mIsNegativeControl);
	}

	mLSData->CorrectLaneStandardCrossChannelAnalysis ();

	// clean up locus lists here

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			mDataChannels [i]->CleanUpLocusSignalLists ();
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			if (mDataChannels [i]->FinalTestForPeakSizeAndNumber (averageHeight, mIsNegativeControl, mIsPositiveControl, genotypes) < 0)
				status = -1;
		}
	}

	if (mLSData->FinalTestForCriticalLaneStandardNotices () < 0)
		status = -1;

	for (i=1; i<= mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel) {

			if (mDataChannels [i]->IsNoticeInList (&target)) {

				status = -1;

				if (!IsNoticeInList (&negCtlTarget)) {

					newNotice = new NegCtrlReqReview;
					data = "Found unexpected peaks in or between loci";
					newNotice->AddDataItem (data);
					AddNoticeToList (newNotice);
				}
			}
		}
	}

//	int NumberOfPrimaryPullups = 0;
	int NumberOfPullups = 0;
//	PullUpPrimaryChannel primaryTarget;
	PullUpFound pullupTarget;

	for (i=1; i<=mNumberOfChannels; i++) {

//		NumberOfPrimaryPullups += mDataChannels [i]->CountSignalsWithNotice (&primaryTarget, mLSData);
		NumberOfPullups += mDataChannels [i]->CountSignalsWithNotice (&pullupTarget, mLSData);
	}

	newNotice = new NumberOfPullUpsInSample;
	noticeData.Convert (NumberOfPullups, 10);
	newNotice->AddDataItem (noticeData);
	AddNoticeToList (newNotice);

	if ((MaxPullups > 0) && (NumberOfPullups >= MaxPullups)) {

		newNotice = new NumberOfPullUpPeaksExceedsThreshold;
		noticeData << " (Max = " << MaxPullups << ")";
		newNotice->AddDataItem (noticeData);
		AddNoticeToList (newNotice);
		status = -1;
	}
/*
	newNotice = new NumberOfPrimaryPullUpsInSample;
	noticeData.Convert (NumberOfPrimaryPullups, 10);
	newNotice->AddDataItem (noticeData);
	AddNoticeToList (newNotice);

	if (NumberOfPrimaryPullups > MaxPrimaryPullups) {

		newNotice = new NumberOfPrimaryPullUpPeaksExceedsThreshold;
		noticeData << " (Max = " << MaxPrimaryPullups << ")";
		newNotice->AddDataItem (noticeData);
		AddNoticeToList (newNotice);
		status = -1;
	}
*/
	int nStutter = 0;
	int nAdenylation = 0;
	int nOLAlleles = 0;
	int nExcessResiduals = 0;

	StutterFound stutterTarget;
	AdenylationFound adenylationTarget;
	OLAlleleFound OLAlleleTarget;
	ResidualExceedsThreshold residualTarget;

	for (i=1; i<=mNumberOfChannels; i++) {

		nStutter += mDataChannels [i]->CountSignalsWithNotice (&stutterTarget, mLSData);
		nAdenylation += mDataChannels [i]->CountSignalsWithNotice (&adenylationTarget, mLSData);
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			nOLAlleles += mDataChannels [i]->CountSignalsWithNotice (&OLAlleleTarget, mLSData);
	}

	for (i=1; i<=mNumberOfChannels; i++) {

		if (i != mLaneStandardChannel)
			nExcessResiduals += mDataChannels [i]->CountSignalsWithNotice (&residualTarget, mLSData);
	}

	newNotice = new NumberOfStutterPeaksInSample;
	noticeData.Convert (nStutter, 10);
	newNotice->AddDataItem (noticeData);
	AddNoticeToList (newNotice);

	if ((MaxStutter > 0) && (nStutter >= MaxStutter)) {

		newNotice = new NumberOfStutterPeaksExceedsThreshold;
		noticeData << " (Max = " << MaxStutter << ")";
		newNotice->AddDataItem (noticeData);
		AddNoticeToList (newNotice);
		status = -1;
	}

	newNotice = new NumberOfAdenylationsInSample;
	noticeData.Convert (nAdenylation, 10);
	newNotice->AddDataItem (noticeData);
	AddNoticeToList (newNotice);

	if ((MaxAdenylation > 0) && (nAdenylation >= MaxAdenylation)) {

		newNotice = new NumberOfAdenylationPeaksExceedsThreshold;
		noticeData << " (Max = " << MaxAdenylation << ")";
		newNotice->AddDataItem (noticeData);
		AddNoticeToList (newNotice);
		status = -1;
	}

	newNotice = new NumberOfOLAllelesWithinLociInSample;
	noticeData.Convert (nOLAlleles, 10);
	newNotice->AddDataItem (noticeData);
	AddNoticeToList (newNotice);

	if ((MaxOLAlleles > 0) && (nOLAlleles >= MaxOLAlleles)) {

		newNotice = new NumberOfOLAllelesExceedsThreshold;
		noticeData << " (Max = " << MaxOLAlleles << ")";
		newNotice->AddDataItem (noticeData);
		AddNoticeToList (newNotice);
		status = -1;
	}

	newNotice = new NumberOfExcessiveResidualsInSample;
	noticeData.Convert (nExcessResiduals, 10);
	newNotice->AddDataItem (noticeData);
	AddNoticeToList (newNotice);

	if ((MaxExcessiveResiduals > 0) && (nExcessResiduals >= MaxExcessiveResiduals)) {

		newNotice = new NumberOfExcessiveResidualsExceedsThreshold;
		noticeData << " (Max = " << MaxExcessiveResiduals << ")";
		newNotice->AddDataItem (noticeData);
		AddNoticeToList (newNotice);
		status = -1;
	}

	return status;
}


CoreBioComponent* STRSampleCoreBioComponent :: CreateNewTransformedBioComponent (const CoreBioComponent& bc, CoordinateTransform* trans) {

	return new STRSampleCoreBioComponent ((const STRSampleCoreBioComponent&) bc, trans);
}


