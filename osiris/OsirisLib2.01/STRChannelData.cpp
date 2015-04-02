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
*  FileName: STRChannelData.cpp
*  Author:   Robert Goor
*
*/
//
//     class STRChannelData is the prototype channel for STR forensics.  It maintains a SampledData object and analyzes it.  Then, there
//  are the subclasses for Internal Lane Standard channels, ladder channels and sample channels
//

#include "STRChannelData.h"
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
#include "rgpscalar.h"
#include "SmartNotice.h"
#include "STRSmartNotices.h"

bool STRChannelData::UseHermiteCubicSplineForNormalization = true;

double STRLaneStandardChannelData::minLaneStandardRFU = 150.0;
double STRLadderChannelData::minLadderRFU = 150.0;
double STRLadderChannelData::minInterlocusRFU = 150;
double STRSampleChannelData::minSampleRFU = 150.0;
double STRSampleChannelData::minInterlocusRFU = 150;
double STRSampleChannelData::sampleDetectionThreshold = 150.0;
bool STRSampleChannelData::UseOldBaselineEstimation = false;

double STRLaneStandardChannelData::maxLaneStandardRFU = -1.0;
double STRLadderChannelData::maxLadderRFU = -1.0;
double STRSampleChannelData::maxSampleRFU = -1.0;

double STRLaneStandardChannelData::ILSStutterThreshold;
double STRLaneStandardChannelData::ILSAdenylationThreshold;
double STRLaneStandardChannelData::ILSFractionalFilter = -1.0;


PERSISTENT_DEFINITION (STRChannelData, _STRCHANNELDATA_, "STRChannelData")
PERSISTENT_DEFINITION (STRLaneStandardChannelData, _STRLANESTANDARDCHANNELDATA_, "STRLaneStandardChannelData")
PERSISTENT_DEFINITION (STRLadderChannelData, _STRLADDERCHANNELDATA_, "STRLadderChannelData")
PERSISTENT_DEFINITION (STRSampleChannelData, _STRSAMPLECHANNELDATA_, "STRSampleChannelData")


bool cmpLess (const DistanceElement* a, const DistanceElement* b) {

	if (!a)
		return true;

	if (!b)
		return false;

	return a->mDistance < b->mDistance;
}


STRChannelData :: STRChannelData () : ChannelData (), MinimumFractionOfAverageWidth (0.25), MaximumMultipleOfAverageWidth (2.0), 
MinimumFractionOfAveragePeak (0.333), MaximumMultipleOfAveragePeak (-1.0) {

}


STRChannelData :: STRChannelData (int channel) : ChannelData (channel), MinimumFractionOfAverageWidth (0.25), MaximumMultipleOfAverageWidth (2.0), 
MinimumFractionOfAveragePeak (0.333), MaximumMultipleOfAveragePeak (-1.0) {

}

STRChannelData :: STRChannelData (int channel, LaneStandard* inputLS) : ChannelData (channel, inputLS), MinimumFractionOfAverageWidth (0.25), MaximumMultipleOfAverageWidth (2.0), 
MinimumFractionOfAveragePeak (0.333), MaximumMultipleOfAveragePeak (-1.0) {

}


STRChannelData :: STRChannelData (const STRChannelData& strCD) : ChannelData (strCD), MinimumFractionOfAverageWidth (strCD.MinimumFractionOfAverageWidth), 
MaximumMultipleOfAverageWidth (strCD.MaximumMultipleOfAverageWidth), 
MinimumFractionOfAveragePeak (strCD.MinimumFractionOfAveragePeak), 
MaximumMultipleOfAveragePeak (strCD.MaximumMultipleOfAveragePeak) {

}


STRChannelData :: STRChannelData (const STRChannelData& strCD, CoordinateTransform* trans) : ChannelData (strCD, trans), 
MinimumFractionOfAverageWidth (strCD.MinimumFractionOfAverageWidth), 
MaximumMultipleOfAverageWidth (strCD.MaximumMultipleOfAverageWidth), 
MinimumFractionOfAveragePeak (strCD.MinimumFractionOfAveragePeak), 
MaximumMultipleOfAveragePeak (strCD.MaximumMultipleOfAveragePeak) {

}


STRChannelData :: ~STRChannelData () {

}


int STRChannelData :: SetData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	return ChannelData::SetData (fileData, testControlPeak, testSamplePeak);
}


int STRChannelData :: SetRawData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	return ChannelData::SetRawData (fileData, testControlPeak, testSamplePeak);
}


void STRChannelData :: PrecomputeAverages (RGDList& set) {

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


int STRChannelData :: TestNeighbors (DataSignal* previous, DataSignal* testSignal, DataSignal* following) {

//	double Width = testSignal->GetStandardDeviation ();
	double peak = testSignal->Peak ();
	Boolean NotAcceptable = FALSE;
	Boolean Marginal = FALSE;
//	Notice* newNotice;
	RGString info;
	double stutterLimit = STRLaneStandardChannelData::GetILSStutterThreshold ();
	double adenylationLimit = STRLaneStandardChannelData::GetILSAdenylationThreshold ();
	PullUpFound pullup;
	

/*	if (Width < MinimumFractionOfAverageWidth * averageWidth) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooNarrow, "", "Signal is too narrow for set");
	}*/

/*
	if (Width > MaximumMultipleOfAverageWidth * averageWidth) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooWide, "", "Signal is too wide for set");
		newNotice = new PoorPeakMorphology;
		info << "(Width)";
		newNotice->AddDataItem (info);
		testSignal->AddNoticeToList (newNotice);
	}

	if (peak < MinimumFractionOfAveragePeak * averagePeak) {

		Marginal = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooLow, "", "Signal is too low for set");
	}

	if ((MaximumMultipleOfAveragePeak > 0.0) && (peak > MaximumMultipleOfAveragePeak * averagePeak)) {

		Marginal = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooHigh, "", "Signal is too high for set");
	}
*/

	double ThisMean = testSignal->GetMean ();
	double ThisPeak = testSignal->Peak ();
	double ThisSigma = testSignal->GetStandardDeviation ();

	//
	// test for non-template addition (-A and +A) and for stutter (n - 4 and n + 4)
	//

	if ((following != NULL) && (!following->IsNoticeInList (&pullup))) {

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

	if ((previous != NULL) && (!previous->IsNoticeInList (&pullup))) {

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


int STRChannelData :: AnalyzeCrossChannel (ChannelData** channels, int nChannels) {

	// We need to call this BEFORE ILS has been analyzed, to remove any bad peaks before analysis!!!

	//  2/22/2008:  This whole routine is overly complex and DOES NOT WORK!!  See STRCoreBioComponent
	//  for working version of AnalyzeCrossChannel;
	
/*	int channel = mChannel;
	DataSignal* nextSignal;
	DataSignal* testSignal;
	RGDListIterator it (PreliminaryCurveList);
	int i;
	int j;
	int returnValue = 1;
	int foundOneReturnValue;
	int channelCount;
	DataSignal** possibleBTs = new DataSignal* [nChannels + 1];
	possibleBTs [channel] = NULL;
	double maxPeak;
	double minPeak;
	double maxWidth;
	double minWidth;
	int maxIndex;
	RGString Msg;
	RGString BTMessage ("*****PROBABLE BLEED-THROUGH FROM ");
	RGString BTCauseMessage ("*****PROBABLE CAUSE OF BLEED-THROUGH IN SECONDARY CHANNELS*****\n");
	RGString SpikeMessage ("*****PROBABLE SPIKE ");
	double currentPeak;
	double currentWidth;
	double calculatedNormalWidth;
	double mTimeTolerance = 1.0;
	double mWidthMatchFraction = 0.1;  // Double it to get width fractional tolerance, currently 20%
	double mWidthFractionForSpike = 0.35;  // Width must be this fraction of expected width to qualify as spike
	Notice* newNotice;
	RGString info;
	unsigned long mReportLevel = OutputLevelManager::CurveIsAPullUp;

	foundOneReturnValue = 0;

	for (i=1; i<=nChannels; i++)
		channels [i]->ResetPreliminarySignalSearch ();

	while (nextSignal = (DataSignal*)it ()) {

		possibleBTs [channel] = nextSignal;

		for (i=1; i<=nChannels; i++) {

			if (i != channel)
				possibleBTs [i] = channels [i]->FindPreliminarySignalWithinToleranceOf (nextSignal, mTimeTolerance);
		}

		channelCount = 0;

		for (i=1; i<=nChannels; i++) {

			if (possibleBTs [i] != NULL)
				channelCount++;
		}

		if (channelCount >= 2) {

			// we may have a bleed-through or a spike, put secondary peaks into artifacts or into marginal list
			// First test for uniformity of width.  Then, which is it?

			// If not uniform width, ignore...continue
			
			minPeak = maxPeak = nextSignal->Peak ();
			minWidth = maxWidth = nextSignal->GetStandardDeviation ();

			for (j=1; j<=nChannels; j++) {

				if (j != channel) {
				
					testSignal = possibleBTs [j];
					
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
			}

			//  Now test worst case width variation:

			if (maxWidth - minWidth > mWidthMatchFraction * (maxWidth + minWidth))  // if true, not uniform enough...skip it
				continue;  // or add contingency here???

			// Now test if width small enough to be a spike...can we just test the width for nextSignal?  Let's assume so for now...
			calculatedNormalWidth = 3.0 + 2.5E-4 * nextSignal->GetMean ();  // This is VERY approximate...for now
			currentWidth = nextSignal->GetStandardDeviation ();

			if (currentWidth < mWidthFractionForSpike * calculatedNormalWidth) {

//				if (channelCount != nChannels) {

					// This is not a traditional spike...should hit all channels...add notices and escape?
//					continue;
//				}

				// Test if peaks are of same magnitude...

//				if (minPeak < minMaxPeakCompare * maxPeak) {

					// This is not a traditional spike...peaks too uneven...add notices and escape?
//					continue;
//				}

				// Add spike notices
				Msg = "ON ";
				Msg << channelCount << " CHANNELS, WITH HEIGHT VARIATION FROM " << minPeak << " TO " << maxPeak << "*****\n";

				for (i=1; i<=nChannels; i++) {

					testSignal = possibleBTs [i];

					if (testSignal != NULL) {

						testSignal->AddNoticeToList (mReportLevel, "", SpikeMessage + Msg);
						(channels [i]->GetPreliminaryCurveList ()).RemoveReference (testSignal);
						(channels [i]->GetArtifactList ()).Insert (testSignal);
						newNotice = new SpikeFound;
						info = " at sample ";
						info << testSignal->GetMean ();
						newNotice->AddDataItem (info);
						channels [i]->AddNoticeToList (newNotice);
						newNotice = new SpikeFound;
						testSignal->AddNoticeToList (newNotice);
						possibleBTs [i] = NULL;
					}
				}
			}

			else {

				// This is bleed-through or pull-up...whatever

				Msg = "CHANNEL ";
				Msg << maxIndex << "*****\n";

				for (i=1; i<=nChannels; i++) {

					testSignal = possibleBTs [i];

					if (testSignal == NULL)
						continue;
					
					if (i != maxIndex) {

						(channels [i]->GetPreliminaryCurveList ()).RemoveReference (testSignal);
						testSignal->AddNoticeToList (mReportLevel, "", BTMessage + Msg);
						newNotice = new PullUpFound;
						info = " at sample ";
						info << testSignal->GetMean ();
						newNotice->AddDataItem (info);
						channels [i]->AddNoticeToList (newNotice);
						newNotice = new PullUpFound;
						testSignal->AddNoticeToList (newNotice);

						(channels [i]->GetArtifactList ()).Insert (testSignal);
						possibleBTs [i] = NULL;
					}

					else {

						testSignal = possibleBTs [maxIndex];
						testSignal->AddNoticeToList (mReportLevel, "", BTCauseMessage);
						newNotice = new PullUpFound;
						info = " at sample ";
						info << testSignal->GetMean ();
						newNotice->AddDataItem (info);
						channels [maxIndex]->AddNoticeToList (newNotice);
						newNotice = new PullUpFound;
						testSignal->AddNoticeToList (newNotice);
						possibleBTs [maxIndex] = NULL;
					}
				}
			}

			returnValue = foundOneReturnValue;  // move this later
		}
	}

	return returnValue;*/

	return -1;
}


ChannelData* STRChannelData :: CreateNewTransformedChannel (const ChannelData& cd, CoordinateTransform* trans) {

	return new STRChannelData ((const STRChannelData&) cd, trans);
}


double STRChannelData :: GetMinimumHeight () const {

	return DataSignal::GetMinimumHeight ();
}


double STRChannelData :: GetMaximumHeight () const {

	return DataSignal::GetMaximumHeight ();
}


double STRChannelData :: GetDetectionThreshold () const {

	return DataSignal::GetMinimumHeight ();
}


int STRChannelData :: WritePeakInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& tagName) {

	RGDListIterator it (CompleteCurveList);
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) it ())
		nextSignal->WritePeakInfoToXML (text, indent, tagName, "meanbps");

	return 0;
}


int STRChannelData :: WriteArtifactInfoToXML (RGTextOutput& text, const RGString& indent, ChannelData* laneStd) {

	RGDListIterator it (ArtifactList);
//	RGDListIterator it2 (SupplementalArtifacts);
	DataSignal* nextSignal;
	double left;
	double right;

	if (laneStd != NULL) {

		left = laneStd->GetFirstAnalyzedMean ();
		right = laneStd->GetLastAnalyzedMean ();
	}

	else {

		left = 0.0;
		right = DOUBLEMAX;
	}

	double mean;

	if (right < -100.0)
		return -1;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->DontLook ())
			continue;

		mean = nextSignal->GetMean ();

		if (!CoreBioComponent::SignalIsWithinAnalysisRegion (nextSignal, left))	// modified 03/13/2015
			continue;

		if (mean > right)
			break;

		nextSignal->WriteArtifactInfoToXML (text, indent, "artifact", "meanbps");
	}

	return 0;
}


STRLaneStandardChannelData :: STRLaneStandardChannelData () : STRChannelData (), mInverseSouthern (NULL), mGlobalSouthern (NULL) {

}


STRLaneStandardChannelData :: STRLaneStandardChannelData (int channel) : STRChannelData (channel), mInverseSouthern (NULL), 
mGlobalSouthern (NULL) {

}


STRLaneStandardChannelData :: STRLaneStandardChannelData (int channel, LaneStandard* inputLS) : STRChannelData (channel, inputLS), 
mInverseSouthern (NULL), mGlobalSouthern (NULL) {

}


STRLaneStandardChannelData :: STRLaneStandardChannelData (const STRLaneStandardChannelData& strLS) :
STRChannelData (strLS), mInverseSouthern (NULL), mGlobalSouthern (NULL) {

}


STRLaneStandardChannelData :: ~STRLaneStandardChannelData () {

	delete mInverseSouthern;
	delete mGlobalSouthern;
}


void STRLaneStandardChannelData :: ClearAndRepopulateFromList (RGDList& sourceList, RGDList& curveList, RGDList& overFlowList) {

	curveList.Clear ();
	overFlowList.Clear ();
	RGDListIterator it (sourceList);
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) it ())
		curveList.Append (nextSignal);
}


int STRLaneStandardChannelData :: SelectClusterSubset (RGDList& incomingList, RGDList& outgoingList, int Size, RGTextOutput& text, RGTextOutput& ExcelText) {

	RGDListIterator it (incomingList);
	DataSignal* nextSignal;
	PeakInfoForClusters** peakList;
	PeakInfoForClusters* nextPeak;
//	PeakInfoForClusters* prevPeak;
	int nPeaks = incomingList.Entries ();

	if (nPeaks < Size)
		return 0;

	peakList = new PeakInfoForClusters* [nPeaks];
	int i = 0;
	vector<PeakInfoForClusters*> peakVector;

	while (nextSignal = (DataSignal*) it ()) {

		nextPeak = new PeakInfoForClusters (nextSignal);
		peakList [i] = nextPeak;
		peakVector.push_back (nextPeak);
		nextPeak->SetIndex (i);
		i++;
	}

	double maxPeak = 0.0;
	double maxPeakFromRight = 0.0;
	double maxPeakFromLeft = 0.0;
	double currentPeak;
	double maxMean = peakList [nPeaks - 1]->GetMean ();
	double minMean = peakList [0]->GetMean ();
	double deltaMean = (maxMean - minMean);

	for (i=0; i<nPeaks; i++) {

		currentPeak = peakList [i]->GetPeak ();

		if (currentPeak > maxPeak)
			maxPeak = currentPeak;
	}

	double minPeak = maxPeak;

	for (i=0; i<nPeaks; i++) {

		currentPeak = peakList [i]->GetPeak ();

		if (currentPeak < minPeak)
			minPeak = currentPeak;
	}

	double deltaPeak = maxPeak - minPeak;

	if (deltaPeak > 0.0)
		PeakInfoForClusters::SetHeightFactor (deltaMean / deltaPeak);

	else
		PeakInfoForClusters::SetHeightFactor (1.0);

	for (i=0; i<Size; i++) {

		currentPeak = peakList [i]->GetPeak ();

		if (currentPeak > maxPeakFromLeft)
			maxPeakFromLeft = currentPeak;
	}

	int j = 0;
	i = nPeaks - 1;

	while (j < Size) {

		currentPeak = peakList [i]->GetPeak ();

		if (currentPeak > maxPeakFromRight)
			maxPeakFromRight = currentPeak;

		j++;
		i--;
	}

	bool thereArePrimerPeaks = true;

	if (maxPeakFromRight > maxPeakFromLeft)
		thereArePrimerPeaks = false;

	double** distanceMatrix = new double* [nPeaks];

	for (i=0; i<nPeaks; i++)
		distanceMatrix [i] = new double [nPeaks];

	sort (peakVector.begin (), peakVector.end (), LessHeight);

	
	vector<PeakInfoForClusters*>::iterator itPeaks;
	RGDList** clusterPtrs = new RGDList* [nPeaks];
	RGDList clusters1;
	RGDList* cluster;

	for (i=0; i<nPeaks; i++)
		clusterPtrs [i] = NULL;

	double cutoff1 = 0.65 * deltaPeak;

	//for (itPeaks=peakVector.begin (); itPeaks!=peakVector.end (); itPeaks++) {

	//	newDistance = (*itDistance);
	//	currentDistance = newDistance->mDistance;

	//	if (currentDistance > cutoff1)
	//		break;

	//	i = newDistance->mRow;
	//	j = newDistance->mColumn;
	//	listi = clusterPtrs [i];
	//	listj = clusterPtrs [j];

	//	if ((listi != NULL) && (listj != NULL) && (listi != listj)) {

	//		if (listi->Entries () < listj->Entries ()) {

	//			while (nextPeak = (PeakInfoForClusters*) listi->GetFirst ()) {

	//				listj->Insert (nextPeak);
	//				k = nextPeak->GetIndex ();
	//				clusterPtrs [k] = listj;
	//			}

	//			clusters1.RemoveReference (listi);
	//			delete listi;
	//		}

	//		else {

	//			while (nextPeak = (PeakInfoForClusters*) listj->GetFirst ()) {

	//				listi->Insert (nextPeak);
	//				k = nextPeak->GetIndex ();
	//				clusterPtrs [k] = listi;
	//			}

	//			clusters1.RemoveReference (listj);
	//			delete listj;
	//		}
	//	}

	//	else if ((listi == NULL) && (listj != NULL)) {

	//		listj->Insert (peakList [i]);
	//		clusterPtrs [i] = listj;
	//	}

	//	else if ((listj == NULL) && (listi != NULL)) {

	//		listi->Insert (peakList [j]);
	//		clusterPtrs [j] = listi;
	//	}

	//	else if ((listi == NULL) && (listj == NULL)) {

	//		cluster = new RGDList;
	//		cluster->Append (peakList [i]);
	//		cluster->Insert (peakList [j]);
	//		clusterPtrs [i] = clusterPtrs [j] = cluster;
	//		clusters1.Append (cluster);
	//	}
	//}

	RGDList tempList;
	int beginClusterIndex = -1;

	RGDListIterator clustersIt (clusters1);
	i = 0;
	Endl endline;
	RGDListIterator* tempIt;
	ExcelText.SetOutputLevel (1);

	while (cluster = (RGDList*) clustersIt ()) {

		i++;
		ExcelText << "Cluster Number:  \t" << i << endline;
		tempIt = new RGDListIterator (*cluster);
		
		while (nextPeak = (PeakInfoForClusters*) (*tempIt) ()) {

			ExcelText << "Mean = " << nextPeak->GetMean () << "\tPeak = " << nextPeak->GetPeak () << endline;
		}

		ExcelText << endline;
		delete tempIt;
		cluster->Clear ();  // This is temporary!
	}

	ExcelText.ResetOutputLevel ();
	clusters1.ClearAndDelete ();
	return outgoingList.Entries ();
}


int STRLaneStandardChannelData :: CalculateDistanceMatrix (PeakInfoForClusters** peaks, int nPeaks, double** peakMatrix) {

	int i;
	int j;

	PeakInfoForClusters* p1;
	PeakInfoForClusters* p2;

	for (i=0; i<nPeaks; i++) {

		p1 = peaks [i];

		for (j=i; j<nPeaks; j++) {

			if (j == i)
				peakMatrix [i][i] = 0.0;

			else {

				p2 = peaks [j];
				peakMatrix [i][j] = GetDistance (*p1, *p2);
			}
		}
	}

	return 0;
}


int STRLaneStandardChannelData :: PopulateAndSortDistanceElements (int nPeaks, double** peakMatrix, vector<DistanceElement*>& distanceArray) {

	int i;
	int j;
	int k = 0;
	DistanceElement* newDistance;
	int status = 0;

	for (i=0; i<nPeaks; i++) {

		for (j=i+1; j<nPeaks; j++) {

			newDistance = new DistanceElement (peakMatrix [i][j], i, j);
			distanceArray.push_back (newDistance);
			k++;
		}
	}

	if (2 * k != nPeaks * (nPeaks - 1))
		status = -1;

	// Now sort:
	sort (distanceArray.begin (), distanceArray.end (), cmpLess);

	return status;
}


bool STRLaneStandardChannelData :: FoundPrimerPeaks (RGDList& peaks, double firstIntervalFraction, double smallestIntervalFraction, double& lastPrimerTime, int Size, double& maxPeak, RGTextOutput& ExcelText) {

	RGDListIterator it (peaks);
	DataSignal* nextSignal;
	PeakInfoForClusters** peakList;
	PeakInfoForClusters* nextPeak;
//	PeakInfoForClusters* prevPeak;
	int nPeaks = peaks.Entries ();
	lastPrimerTime = -1.0;
	maxPeak = 0.0;

	if (nPeaks < Size)
		return false;

	peakList = new PeakInfoForClusters* [nPeaks];
	int i = 0;
	vector<PeakInfoForClusters*> peakVector;

	while (nextSignal = (DataSignal*) it ()) {

		nextPeak = new PeakInfoForClusters (nextSignal);
		peakList [i] = nextPeak;
		peakVector.push_back (nextPeak);
		nextPeak->SetIndex (i);
		i++;
	}

	const double* largeCharacteristics;
	int nLargeCharacteristics = mLaneStandard->GetLargeCharacteristicArray (largeCharacteristics);

	int nTestIntervals;  // this will be the number of intervals in which we test peak density and max peak height.
	double startMean = peakList [0]->GetMean ();
	double endMean = peakList [nPeaks - 1]->GetMean ();
	double testDuration = endMean - startMean;
	double testInterval = smallestIntervalFraction * testDuration;

	if (smallestIntervalFraction > 0.0)
		nTestIntervals = (int) ceil (1.0 / smallestIntervalFraction);

	else {

		nTestIntervals = Size + 5;
		testInterval = testDuration / (double)nTestIntervals;
	}

	int* density = new int [nTestIntervals];
	double* localMaxPeak = new double [nTestIntervals];
	double firstGapInterval = firstIntervalFraction * testDuration;
	double currentIntervalEnd = startMean + testInterval;
	int j = 0;
	double maxLocalPeak;
	int nLocalPeaks;
	double currentMean;
	double currentPeak;
	double overallMaxPeak = 0.0;
	int overallMaxDensity = 0;

	vector<double> localPeakMaxima;
	vector<int> localDensities;

	for (i=0; i<nTestIntervals; i++) {

		maxLocalPeak = 0.0;
		nLocalPeaks = 0;

		while (j < nPeaks) {

			currentMean = peakList [j]->GetMean ();
			currentPeak = peakList [j]->GetPeak ();

			if (currentMean < currentIntervalEnd) {

				nLocalPeaks++;

				if (currentPeak > maxLocalPeak)
					maxLocalPeak = currentPeak;

				j++;
			}

			else
				break;
		}

		localMaxPeak [i] = maxLocalPeak;
		density [i] = nLocalPeaks;
		currentIntervalEnd += testInterval;
		localPeakMaxima.push_back (maxLocalPeak);
		localDensities.push_back (nLocalPeaks);

		if (maxLocalPeak > overallMaxPeak)
			overallMaxPeak = maxLocalPeak;

		if (nLocalPeaks > overallMaxDensity)
			overallMaxDensity = nLocalPeaks;
	}

	// Now analyze these arrays of data...
	sort (localPeakMaxima.begin (), localPeakMaxima.end ());
	sort (localDensities.begin (), localDensities.end ());

	// Let's try to localize the primer peaks

	j = 0;
	double peakCutoff = 0.4 * overallMaxPeak;
	double densityCutoff = 0.4 * (double)overallMaxDensity;
	int nIntervalsAbovePeakCutoff = 0;
	int nIntervalsAboveDensityCutoff = 0;

	int firstHighDensityInterval = 0;
	int firstNonHighDensityInterval = 0;
	bool foundFirstHighDensityInterval = false;
	bool quitLookingForHighDensity = false;

	int firstHighPeakInterval = 0;
	int firstNonHighPeakInterval = 0;
	bool foundFirstHighPeakInterval = false;
	bool quitLookingForHighPeak = false;

	int lastHighPeakInterval = 0;
	int lastHighDensityInterval = 0;

	for (i=0; i<nTestIntervals; i++) {

		if (localMaxPeak [i] > peakCutoff) {

			nIntervalsAbovePeakCutoff++;
			lastHighPeakInterval = i;

			if (!quitLookingForHighPeak) {

				if (!foundFirstHighPeakInterval) {

					firstHighPeakInterval = i;
					foundFirstHighPeakInterval = true;
				}
			}
		}

		else if (!quitLookingForHighPeak && foundFirstHighPeakInterval) {

			quitLookingForHighPeak = true;
			firstNonHighPeakInterval = i;
		}

		if (density [i] > densityCutoff) {

			nIntervalsAboveDensityCutoff++;
			lastHighDensityInterval = i;

			if (!quitLookingForHighDensity) {

				if (!foundFirstHighDensityInterval) {

					firstHighDensityInterval = i;
					foundFirstHighDensityInterval = true;
				}
			}
		}

		else if (!quitLookingForHighDensity && foundFirstHighDensityInterval) {

			quitLookingForHighDensity = true;
			firstNonHighDensityInterval = i;
		}
	}

	if (!quitLookingForHighPeak)
		firstNonHighPeakInterval = nTestIntervals;

	if (!quitLookingForHighDensity)
		firstNonHighDensityInterval = nTestIntervals;

	// Estimate the end of the primer peaks...if there are some!
	double endPrimers = startMean;
	bool foundPrimers = false;
	int halfWay = nTestIntervals / 2;

	if (lastHighPeakInterval < halfWay) {

		// At least the high peaks are at the beginning, not spread out, which indicates there are primer peaks

		if (firstHighPeakInterval == firstHighDensityInterval) {

			// this is a VERY good indication of primer peaks:  high peaks together with high density

			endPrimers = ((double)firstNonHighPeakInterval) * testInterval + startMean;
			foundPrimers = true;
		}

		else if (density [firstHighPeakInterval] > densityCutoff) {

			// this is also a VERY good indication of primer peaks:  high peaks together with high density

			endPrimers = ((double)firstNonHighPeakInterval) * testInterval + startMean;
			foundPrimers = true;
		}

		else if (density [firstHighPeakInterval] > 3) {

			// this is a pretty good indication of primer peaks:  high peaks together with higer than expected density

			endPrimers = ((double)firstNonHighPeakInterval) * testInterval + startMean;
			foundPrimers = true;
		}

		else {

			for (i=firstHighPeakInterval; i<firstNonHighPeakInterval; i++) {

				if (density [i] > densityCutoff) {

					endPrimers = ((double)firstNonHighPeakInterval) * testInterval + startMean;
					foundPrimers = true;
					break;
				}
			}
		}
	}

	else {

		// There are two possibilities:  either there are no primer peaks and the high peaks are truly the ILS peaks or
		// there is additional junk after the primer peaks.

		if (nLargeCharacteristics > 0) {

			if (nLargeCharacteristics < Size) {

				if ((nIntervalsAbovePeakCutoff < nLargeCharacteristics) && (firstHighPeakInterval < halfWay)) {

					// High probability that there are primer peaks...not enough tall peaks to make up ILS peaks

					endPrimers = ((double)firstNonHighPeakInterval) * testInterval + startMean;
					foundPrimers = true;
				}
			}

			else {

				if ((nIntervalsAbovePeakCutoff < nLargeCharacteristics - 2) && (firstHighPeakInterval < halfWay)) {

					// High probability that there are primer peaks...not enough tall peaks to make up ILS peaks

					endPrimers = ((double)firstNonHighPeakInterval) * testInterval + startMean;
					foundPrimers = true;
				}
			}
		}
	}

	double lastPrimerMean = startMean;
	double firstNonPrimerMean = startMean;
	double peakValue;
	double meanValue;
	int nCurvesRemoved = 0;

	if (foundPrimers) {

		for (i=0; i<nPeaks; i++) {

			meanValue = peakList [i]->GetMean ();

			if (meanValue >= endPrimers) {
				
				if (firstNonPrimerMean <= lastPrimerMean)
					firstNonPrimerMean = meanValue;

				break;
			}

			peakValue = peakList [i]->GetPeak ();

			if (peakValue > peakCutoff)
				lastPrimerMean = meanValue;

			else if (firstNonPrimerMean <= lastPrimerMean)
				firstNonPrimerMean = meanValue;
		}
	}

	endPrimers = 0.5 * (lastPrimerMean + firstNonPrimerMean);

	if (foundPrimers) {

		for (i=0; i<nPeaks; i++) {

			if (peakList [i]->GetMean () < endPrimers)
				nCurvesRemoved++;

			else
				break;
		}
	}

	if (nPeaks - nCurvesRemoved < Size) {

		foundPrimers = false;
		endPrimers = 0.0;
		nCurvesRemoved = 0;
	}

	Endl endline;

	ExcelText.SetOutputLevel (1);
	ExcelText << "Start mean = " << startMean << " and end mean = " << endMean << endline;
	ExcelText << "Test interval width = " << testInterval << endline;
	ExcelText << "Max Peak = " << overallMaxPeak << " and Max Density = " << overallMaxDensity << endline << endline;
	ExcelText << "Number of high peak intervals = " << nIntervalsAbovePeakCutoff << " out of " << nTestIntervals << endline;
	ExcelText << "First high peak interval = " << firstHighPeakInterval << " and first non high peak interval to follow = " << firstNonHighPeakInterval << endline;
	ExcelText << "Last high peak interval = " << lastHighPeakInterval << endline << endline;
	
	ExcelText << "Number of high density intervals = " << nIntervalsAboveDensityCutoff << " out of " << nTestIntervals << endline;
	ExcelText << "First high density interval = " << firstHighDensityInterval << " and first non high density interval to follow = " << firstNonHighDensityInterval << endline;
	ExcelText << "Last high density interval = " << lastHighDensityInterval << endline << endline;

	if (foundPrimers)
		ExcelText << "Found primer peaks to left of " << endPrimers << " removing " << nCurvesRemoved << " out of " << nPeaks << endline << endline;

	else
		ExcelText << "Did not find primer peaks" << endline << endline;

	int nIsolatedPeaks = 0;

	for (i=0; i<nPeaks; i++) {

		nextPeak = peakList [i];

		if (nextPeak->GetMean () < endPrimers)
			continue;

		if (nextPeak->GetPeak () > peakCutoff)
			nIsolatedPeaks++;
	}

	ExcelText << "Found " << nIsolatedPeaks << " isolated peaks" << endline;

	if (nLargeCharacteristics > 0) {

		if (nLargeCharacteristics < Size) {

			if (nIsolatedPeaks >= nLargeCharacteristics) {

				// High probability that the so-called isolated peaks are the large characteristics...

				nIsolatedPeaks = 0;
				ExcelText << "May have removed large characteristics.  Restoring..." << endline << endline;
			}
		}

		else {

			if (nIsolatedPeaks >= nLargeCharacteristics - 2) {

				// High probability that the so-called isolated peaks are the large characteristics...

				nIsolatedPeaks = 0;
				ExcelText << "May have removed large characteristics.  Restoring..." << endline << endline;
			}
		}
	}

	if (nPeaks - nCurvesRemoved - nIsolatedPeaks < Size) {

		ExcelText << "Removed too many peaks:  " << nCurvesRemoved + nIsolatedPeaks << " of " << nPeaks << ".  Restoring..." << endline << endline;
		nIsolatedPeaks = 0;
	}

	else
		ExcelText << "Removing " << nCurvesRemoved + nIsolatedPeaks << " of " << nPeaks << endline << endline;

	double tempMaxPeak = 0.0;

	if (nIsolatedPeaks > 0) {

		for (i=0; i<nPeaks; i++) {

			nextPeak = peakList [i];

			if (nextPeak->GetMean () < endPrimers)
				continue;

			peakValue = nextPeak->GetPeak ();

			if (peakValue > peakCutoff)
				continue;

			if (peakValue > tempMaxPeak)
				tempMaxPeak = peakValue;
		}
	}

	else {

		for (i=0; i<nPeaks; i++) {

			nextPeak = peakList [i];

			if (nextPeak->GetMean () < endPrimers)
				continue;

			peakValue = nextPeak->GetPeak ();

			if (peakValue > tempMaxPeak)
				tempMaxPeak = peakValue;
		}
	}

	maxPeak = tempMaxPeak;
	ExcelText << "Max Peak Allowed for ILS = " << maxPeak << endline << endline;
	lastPrimerTime = endPrimers;
	ExcelText.ResetOutputLevel ();

	for (i=0; i<nPeaks; i++)
		delete peakList [i];

	delete[] peakList;
	peakVector.clear ();
	localPeakMaxima.clear ();
	localDensities.clear ();
	delete[] density;
	delete[] localMaxPeak;
	return foundPrimers;
}


int STRLaneStandardChannelData :: AnalyzeLaneStandardChannelRecursivelyUsingDensity (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	int status = 0;

	RGDListIterator CurveIterator (PreliminaryCurveList);
	FinalCurveList.Clear ();
//	DataSignal* FollowingSignal;
	DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
//	int TestResult;
	int Size = mLaneStandard->GetNumberOfCharacteristics ();
	Notice* newNotice;
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
	PullUpFound pullup;
	PullUpPrimaryChannel primaryPullup;
	SignalPeakAboveMaxRFU signalAboveMaxRFU;
	double mPrimerTime = 0.0;
	RGDListIterator FinalIterator (FinalCurveList);
	int origNumberOfPeaks = PreliminaryCurveList.Entries ();
	int TotalCandidates = PreliminaryCurveList.Entries ();

	if (Size <= 0) {

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are no expected peaks for the ILS.\n";
		status = -1;
		newNotice = new ILSHasTooFewPeaks;
		info = " with ";
		info << PreliminaryCurveList.Entries () << " peaks out of " << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);

		cout << ErrorString << endl;
		cout << "There are " << PreliminaryCurveList.Entries () << " curves needed out of " << Size << endl;
		return -50;
	}

	if (PreliminaryCurveList.Entries () < Size) {

		if (print)
			msg.WriteInsufficientPeaksForILS ();

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";
		status = -1;
		newNotice = new ILSHasTooFewPeaks;
		info = " with ";
		info  << PreliminaryCurveList.Entries () << " peaks out of " << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);
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

	// Now remove primer peaks and isolated "large" peaks; FoundPrimerPeaks tests for removing too many peaks, so we don't have to do it here.

	while (nextSignal = (DataSignal*) itt ()) {

		if (nextSignal->GetMean () < primerTime)
			itt.RemoveCurrentItem ();

		else if (nextSignal->Peak () > maxPeak) {

			itt.RemoveCurrentItem ();
			overFlow.Append (nextSignal);
		}
	}

	MergeListAIntoListB (overFlow, ArtifactList);

	// Now we test to see if we have few enough peaks to just go ahead and fit the spacing.  We're not necessarily confident we have the "true" maxPeak, so
	// we're a little leary of using it for fractional filters, etc.  If we can get away without it, we will.  On the other hand, if we have too many peaks, we'll
	// try to reduce the number using the known number of large characteristics.  After we find the true set of ILS peaks, we'll use the maxPeak of those selected
	// to specify peaks that are below the fractional filter or are adenylation or stutter...First put all peaks in FinalCurveList, so we can go back to tempCurveList
	// if we need to.

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

			if (correlation < 0.993)
				relativeHeightsFailed = true;
		}

		bool searchForSubset = (testedRelativeHeights && relativeHeightsFailed) || (!testedRelativeHeights);

		if (testedRelativeHeights && relativeHeightsFailed) {

			//ClearAndRepopulateFromList (totallyTempCurveList, FinalCurveList, overFlow);	//??????????????????????????????????????????????????????????????????????????????????
			ClearAndRepopulateFromList (tempCurveList, FinalCurveList, overFlow);	//??????????????????????????????????????????????????????????????????????????????????
			newNotice = new ILSRelativeHeightsInconsistent;
			AddNoticeToList (newNotice);
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

	if (correlation < 0.993) {

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

	mLaneStandard->AssignLaneStandardSignals (FinalCurveList);
	FinalIterator.Reset ();

	nextSignal = (DataSignal*)FinalCurveList.First ();
	mBeginAnalysis = mPrimerTime;

	if (nextSignal != NULL)
		mBeginAnalysis = 0.9 * nextSignal->GetMean () + 0.1 * mPrimerTime;

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
	bool addedToList;
	bool prevAddedToList = false;
	bool prevJustAddedToList;
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
		addedToList = false;
		prevJustAddedToList = false;

		if ((currentMean < lowMean) || (currentMean > highMean)) {

			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
			"Peak represents measurement at base pair level outside internal lane standard");
			addedToList = true;
		}

		else {

			if (thisPeak <= fractionalThreshold) {

				newNotice = new SignalPeakBelowFractionalFilterHeight;
				nextSignal->AddNoticeToList (newNotice);
				addedToList = true;
			}

			else if ((maxAllowable > 0.0) && (thisPeak >= maxAllowable)) {

				newNotice = new SignalPeakAboveMaxRFU;
				nextSignal->AddNoticeToList (newNotice);
				addedToList = true;
			}

			if (prevSignal != NULL) {

				bpDiff = (int) floor (fabs (thisbp - prevbp) + 0.5);

				if (bpDiff == 1) {

					// possible adenylation

					if ((thisPeak <= adenylationLimit * prevPeak) && (!prevSignal->IsNoticeInList (&pullup))) {

						newNotice = new AdenylationFound;
						nextSignal->AddNoticeToList (newNotice);
						addedToList = true;
					}

					else if ((prevPeak <= adenylationLimit * thisPeak) && (!nextSignal->IsNoticeInList (&pullup))) {

						newNotice = new AdenylationFound;
						prevSignal->AddNoticeToList (newNotice);
						prevJustAddedToList = true;
					}
				}

				else if (bpDiff == 4) {

					// possible stutter

					if ((thisPeak <= stutterLimit * prevPeak) && (!prevSignal->IsNoticeInList (&pullup))) {

						newNotice = new StutterFound;
						nextSignal->AddNoticeToList (newNotice);
						addedToList = true;
					}

					else if ((prevPeak <= stutterLimit * thisPeak) && (!nextSignal->IsNoticeInList (&pullup))) {

						newNotice = new StutterFound;
						prevSignal->AddNoticeToList (newNotice);
						prevJustAddedToList = true;
					}
				}
			}
		}

		if (prevJustAddedToList && !prevAddedToList)
			indivList.Append (prevSignal);

		if (addedToList)
			indivList.Append (nextSignal);

		prevSignal = nextSignal;
		prevAddedToList = addedToList;
		prevbp = thisbp;
		prevPeak = thisPeak;
	}

	MergeListAIntoListB (indivList, ArtifactList);

	// Take care of craters and recheck ILS peaks for issues from above tests...

	// Label the rest of the peaks as "out of place in ILS

	overflowIterator.Reset ();
			
	while (nextSignal = (DataSignal*) overflowIterator ()) {

		nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
			"Peak out of place in final list from 'Search Two Sided':  uncategorized artifact");
		newNotice = new PeakOutOfPlaceInILS;
		nextSignal->AddNoticeToList (newNotice);
	}

	MergeListAIntoListB (overFlow, ArtifactList);

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


int STRLaneStandardChannelData :: HierarchicalLaneStandardChannelAnalysis (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	Endl endline;
	int status = AnalyzeLaneStandardChannelRecursivelyUsingDensity (text, ExcelText, msg, print);

	if (status == 0) {

//		cout << "Lane standard analyzed using density method:  Method 1" << endl;
		ExcelText.SetOutputLevel (1);
		ExcelText << "Lane standard analyzed using density method:  Method 1" << endline;
		ExcelText.ResetOutputLevel ();
	}

	else if (status == -50)
		return status;

	else {

		status = AnalyzeLaneStandardChannelRecursively (text, ExcelText, msg, print);

		if (status == 0) {

			cout << "Lane standard analyzed using recursive method:  Method 2" << endl;
			ExcelText.SetOutputLevel (1);
			ExcelText << "Lane standard analyzed using recursive method:  Method 2" << endline;
			ExcelText.ResetOutputLevel ();
		}
	}

	return status;
}


int STRLaneStandardChannelData :: SetData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	int status = STRChannelData::SetData (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLaneStandardChannelData :: SetRawData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	int status = STRChannelData::SetRawData (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLaneStandardChannelData :: AnalyzeLaneStandardChannel (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	// Change other "Analyze" methods to work through PopulationMarkerSet, so that they can be altered easily to vary, depending on locus

	// Move a lot of this to within LaneStandard so that time array is built there.  Then return final (truly final) curve list for final arrays
	
	int status = 0;

	RGDListIterator CurveIterator (PreliminaryCurveList);
	FinalCurveList.Clear ();
	DataSignal* FollowingSignal;
	DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
	int TestResult;
	int Size = mLaneStandard->GetNumberOfCharacteristics ();
	Notice* newNotice;
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

	double endOfJunk;

	maxPeak = Locus::MaximumPeakHeight (PreliminaryCurveList);
	CurveIterator.ResetToEnd ();

	nextSignal = (DataSignal*)CurveIterator.CurrentItem ();

	if (nextSignal == NULL) {

		if (print)
			msg.WriteInsufficientPeaksForILS ();

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";
		status = -1;
		newNotice = new ILSHasTooFewPeaks;
		info = " with ";
		info << "no peaks out of " << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);
		cout << ErrorString << endl;
		cout << "There are no peaks available in the ILS" << endl;
		return -50;
	}

	lastPeak = nextSignal->Peak ();
	CurveIterator.Reset ();
	double avePeak = 0.0;

	while (nextSignal = (DataSignal*) CurveIterator ())
		avePeak += nextSignal->Peak ();

	avePeak = avePeak / (double)PreliminaryCurveList.Entries ();
	int origNumberOfPeaks = PreliminaryCurveList.Entries ();

	if (lastPeak <= endTestFactor * maxPeak) {

		curvesLeft = Locus::DivideListByPeakHeight (PreliminaryCurveList, overFlow, indivList, avePeak, maxPeak, scaleFactor, Size);
	}

	nextSignal = (DataSignal*) overFlow.Last ();

	if (nextSignal != NULL)
		endOfJunk = nextSignal->GetMean ();

	else
		endOfJunk = 0.0;

	MergeListAIntoListB (overFlow, ArtifactList);

	PrecomputeAverages (PreliminaryCurveList);
	int TotalCandidates = PreliminaryCurveList.Entries ();

	double MaxPeak = 0.0;
	double peak;
	CurveIterator.Reset ();
	int skipPeaks = (4 * TotalCandidates) / 10;
	int kk = 0;

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		kk++;

		if (kk < skipPeaks)
			continue;

		peak = nextSignal->Peak ();

		if (peak > MaxPeak)
			MaxPeak = peak;
	}

	double fractionalThreshold = 0.0;
	double fractionalFilter = STRLaneStandardChannelData::GetILSFractionalFilter ();
	
	if (fractionalFilter > 0.0) {
	
		fractionalThreshold = fractionalFilter * MaxPeak;
	}

/*	while (nextSignal = (DataSignal*) PreliminaryCurveList.GetFirst ()) {

		if (nextSignal->Peak () <= 0.25 * MaxPeak)
			ArtifactList.InsertWithNoReferenceDuplication (nextSignal);

		else {

			PreliminaryCurveList.Prepend (nextSignal);
			break;
		}
	}

	while (nextSignal = (DataSignal*) PreliminaryCurveList.GetLast ()) {

		if (nextSignal->Peak () <= 0.25 * MaxPeak)
			ArtifactList.InsertWithNoReferenceDuplication (nextSignal);

		else {

			PreliminaryCurveList.Append (nextSignal);
			break;
		}
	}  */

/*	CurveIterator.Reset ();

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		if (nextSignal->Peak () <= 0.20 * MaxPeak) {

			newNotice = new PeakOutOfPlaceInILS;
			nextSignal->AddNoticeToList (newNotice);
			ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
			CurveIterator.RemoveCurrentItem ();
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
					"Peak out of place in final list:  uncategorized artifact");
		}
	}*/

	CurveIterator.Reset ();

	while (nextSignal = (DataSignal*)PreliminaryCurveList.GetFirst ()) {

		// Test for out of bounds on various quantities...
		FollowingSignal = (DataSignal*)PreliminaryCurveList.First ();

		if ((fractionalFilter > 0.0) && (nextSignal->Peak () <= fractionalThreshold))
			ArtifactList.InsertWithNoReferenceDuplication (nextSignal);

		else {

			TestResult = TestNeighbors (PreviousSignal, nextSignal, FollowingSignal);

			if (TestResult <= 0)
				ArtifactList.InsertWithNoReferenceDuplication (nextSignal);

			else
				FinalCurveList.Append (nextSignal);
		}

		PreviousSignal = nextSignal;
	}

	RGDListIterator FinalIterator (FinalCurveList);
	RGDListIterator FollowingIterator (FinalCurveList);
/*
	if ((Size > 0) && (FinalCurveList.Entries () > Size)) {

		PreviousSignal = (DataSignal*)FinalIterator ();
		FollowingIterator ();
		FollowingIterator ();
		FollowingSignal = (DataSignal*)FollowingIterator ();

		while (nextSignal = (DataSignal*)FinalIterator ()) {

			if (FollowingSignal == NULL)
				break;

			peak = nextSignal->Peak ();

			if ((peak <= 0.5 * PreviousSignal->Peak ()) && (peak <= 0.5 * FollowingSignal->Peak ())) {

				FinalIterator.RemoveCurrentItem ();
				ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
					"Peak out of place in final list:  uncategorized artifact");
				newNotice = new PeakOutOfPlaceInILS;
				nextSignal->AddNoticeToList (newNotice);
				FollowingSignal = (DataSignal*) FollowingIterator ();
			}

			else if ((peak >= 2.5 * PreviousSignal->Peak ()) && (peak >= 2.5 * FollowingSignal->Peak ())) {

				FinalIterator.RemoveCurrentItem ();
				ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
					"Peak out of place in final list:  uncategorized artifact");
				newNotice = new PeakOutOfPlaceInILS;
				nextSignal->AddNoticeToList (newNotice);
				FollowingSignal = (DataSignal*) FollowingIterator ();
			}

			else {

				PreviousSignal = nextSignal;
				FollowingSignal = (DataSignal*)FollowingIterator ();
			}

//			if (!nextSignal->FlankingPeakProportionExceeds (0.5)) {
//
//				FinalIterator.RemoveCurrentItem ();
//				ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
//				nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
//					"Peak out of place in final list:  uncategorized artifact");
//				newNotice = new PeakOutOfPlaceInILS;
//				nextSignal->AddNoticeToList (newNotice);
//			}
		}
	}
*/
	
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

	if (Size <= 0) {

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are no expected peaks for the ILS.\n";
		status = -1;
		newNotice = new ILSHasTooFewPeaks;
		info = " with ";
		info << FinalCurveList.Entries () << " peaks out of " << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);

		cout << ErrorString << endl;
		cout << "After testing neighbors, there are " << FinalCurveList.Entries () << " curves needed out of " << Size << endl;
		cout << "Original number = " << origNumberOfPeaks << " and after classifying by height, " << TotalCandidates << endl;
	}

	else if (FinalCurveList.Entries () < Size) {

		if (print)
			msg.WriteInsufficientPeaksForILS ();

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";
		status = -1;
		newNotice = new ILSHasTooFewPeaks;
		info = " with ";
		info << FinalCurveList.Entries () << " peaks out of " << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);

		cout << ErrorString << endl;
		cout << "After testing neighbors, there are " << FinalCurveList.Entries () << " curves needed out of " << Size << endl;
		cout << "Original number = " << origNumberOfPeaks << " and after classifying by height, " << TotalCandidates << endl;
	}

	else if (FinalCurveList.Entries () == Size) {

		correlation = DotProductWithQuadraticFit (FinalCurveList, Size, actualArray, differenceArray, leftNorm2s [Size-2]);
	}

//	int Displacement = mLaneStandard->SelectBestSubsetOfCharacteristics (FinalCurveList, correlation);

/*	if (Displacement < 0) {

		if (print)
			msg.WriteInsufficientPeaksForILS ();

		if (status == 0)
			ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";

		newNotice = new ILSHasTooFewPeaks;
		info = " with ";
		info << FinalCurveList.Entries () << " peaks out of " << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);

		cout << "Negative return from 'SelectBestSubset...' with " << FinalCurveList.Entries () << " curves available out of " << Size << endl;
		cout << "Original number = " << origNumberOfPeaks << " and after classifying by height, " << TotalCandidates << endl;

		status = -1;
	}
*/
	// if (correlation < 0.994) perform a more detailed analysis here!!  Separate into 2 lists and return displacement = 0

	else {

		if (hts != NULL) {

			FinalIterator.Reset ();
			double testPeak;

			while (FinalCurveList.Entries () > Size) {

				nextSignal = (DataSignal*) FinalIterator ();
				testPeak = nextSignal->Peak ();

				if (RecursiveInnerProduct::HeightOutsideLimit (hts, 0, testPeak, MaxPeak)) {

					FinalIterator.RemoveCurrentItem ();
					nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
						"Peak out of place in final list:  uncategorized artifact");
					newNotice = new PeakOutOfPlaceInILS;
					data = "(Relative height imbalance)";
					newNotice->AddDataItem (data);
					nextSignal->AddNoticeToList (newNotice);
					ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
				}

				else
					break;
			}

			while (FinalCurveList.Entries () > Size) {

				nextSignal = (DataSignal*) FinalCurveList.GetLast ();
				testPeak = nextSignal->Peak ();

				if (RecursiveInnerProduct::HeightOutsideLimit (hts, Size - 1, testPeak, MaxPeak)) {

					nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
						"Peak out of place in final list:  uncategorized artifact");
					newNotice = new PeakOutOfPlaceInILS;
					data = "(Relative height imbalance)";
					newNotice->AddDataItem (data);
					nextSignal->AddNoticeToList (newNotice);
					ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
				}

				else {

					FinalCurveList.Append (nextSignal);
					break;
				}
			}
		}

		overFlow.Clear ();
		
		IdealControlSetInfo ctlInfo (actualArray, differenceArray, leftNorm2s, rightNorm2s, hts, Size, false);
		int startPts;

		if (hts != NULL)
			startPts = Size;

		else {

			startPts = (Size / 2);

			if (2 * startPts == Size)
				startPts--;

			if (startPts > 6)
				startPts = 6;
		}

		recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (FinalCurveList, ArtifactList, ctlInfo, correlation, 0.98, startPts, MaxPeak);
		overflowIterator.Reset ();
			
		while (nextSignal = (DataSignal*) overflowIterator ()) {

			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
				"Peak out of place in final list from 'Search Two Sided':  uncategorized artifact");
			newNotice = new PeakOutOfPlaceInILS;
			nextSignal->AddNoticeToList (newNotice);
		}

		MergeListAIntoListB (overFlow, ArtifactList);

		if (recursiveStatus < 0) {

			if (print)
				msg.WriteInsufficientPeaksForILS ();

			if (status == 0)
				ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";

			newNotice = new ILSHasTooFewPeaks;
			info = " with ";
			info << FinalCurveList.Entries () << " peaks out of " << Size;
			newNotice->AddDataItem (info);
			AddNoticeToList (newNotice);

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

	if (correlation < 0.993) {

		if (status == 0)
			ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...Correlation with ideal set " << correlation << " below 0.993.\n";

		newNotice = new ILSSpacing;
		info = " Correlation = ";
		info << correlation << " (0.993 - 1.0)";
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);

		cout << ErrorString << endl;

		status = -1;
	}

/*	int CurrentIndex = 0;

	while (CurrentIndex < Displacement) {

		nextSignal = (DataSignal*)FinalCurveList.GetFirst ();
		nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
			"Peak represents measurement at base pair level below internal lane standard");
		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
		CurrentIndex++;
		newNotice = new PeakOutsideILSRange;
		nextSignal->AddNoticeToList (newNotice);
	}

	while (FinalCurveList.Entries () > Size) {

		nextSignal = (DataSignal*)FinalCurveList.GetLast ();
		nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
			"Peak represents measurement at base pair level above internal lane standard");
		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
		newNotice = new PeakOutsideILSRange;
		nextSignal->AddNoticeToList (newNotice);
	}
*/
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

		newNotice = new ILSHasTooFewPeaks;
		info = " with 0 peaks out of ";
		info << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);

		while (nextSignal = (DataSignal*) CompleteIterator ())
			nextSignal->ReportNotices (ExcelText, "\t\t", "\t");

		cout << ErrorString << endl;
		return -1;
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
		}

		if (print)
			nextSignal->Report (text, "    ");

		i++;
	}

	double lowMean = Means [0];
	double highMean = Means [NumberOfAcceptedCurves - 1];
	double currentMean;
	
	while (nextSignal = (DataSignal*) indivList.GetFirst ()) {

		currentMean = nextSignal->GetMean ();

		if ((currentMean < lowMean) || (currentMean > highMean))
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
			"Peak represents measurement at base pair level outside internal lane standard");

		else
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
			"Signal out of place in ILS - uncategorized artifact");

		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
	}

	QuadraticFit fit (Means, NumberOfAcceptedCurves);
	fit.Regress (Sigmas, QFit);

	QuadraticFit fit2 (Means, NumberOfAcceptedCurves);
	fit2.Regress (SecondaryContent, QFit2);
	mInverseSouthern = new CSplineTransform (actualArray, Means, NumberOfAcceptedCurves);
	mGlobalSouthern = new CSplineTransform (Means, actualArray, NumberOfAcceptedCurves);

	if (!print) {

		delete[] CurveNumbers;
		return 0;
	}

//	msg.WriteEmptyLine ();
//	msg.WriteEmptyLine ();
//	msg.WriteFileNumberAndName (NumFiles, FileName);
//	msg.WriteStartDate (StartDate);
//	msg.WriteStartTime (StartTime);
//	msg.WriteLaneNumber (LaneNumber);
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
//		ExcelText << "\t\tPeak = \t" << nextSignal->Peak () << endLine;
	}

	return status;
}


int STRLaneStandardChannelData :: AnalyzeLaneStandardChannelRecursively (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	int status = 0;

	RGDListIterator CurveIterator (PreliminaryCurveList);
	FinalCurveList.Clear ();
	DataSignal* FollowingSignal;
	DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
	int TestResult;
	int Size = mLaneStandard->GetNumberOfCharacteristics ();
	Notice* newNotice;
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
	PullUpFound pullup;
	PullUpPrimaryChannel primaryPullup;
	SignalPeakAboveMaxRFU signalAboveMaxRFU;

	RGDList totallyTempCurveList;

	//RGDList clusterList;
	//SelectClusterSubset (PreliminaryCurveList, clusterList, Size, text, ExcelText);
	//double primerTime;

	//FoundPrimerPeaks (PreliminaryCurveList, mLaneStandard->GetFirstIntervalFraction (), mLaneStandard->GetSmallestIntervalFraction (), primerTime, Size, maxPeak, ExcelText);

	double endOfJunk;

//	maxPeak = Locus::MaximumPeakHeight (PreliminaryCurveList);

	
	maxPeak = 0.0;
	double currentPeak;
	RGDListIterator itt (PreliminaryCurveList);

	while (nextSignal = (DataSignal*) itt ()) {

		if ((nextSignal->IsNoticeInList (&pullup)) && (!nextSignal->IsNoticeInList (&primaryPullup)))
			continue;

		if (nextSignal->IsNoticeInList (&signalAboveMaxRFU))
			continue;

		currentPeak = nextSignal->Peak ();

		if (currentPeak > maxPeak) {

			maxPeak = currentPeak;
		}
	}

	CurveIterator.ResetToEnd ();

	while (true) {

		nextSignal = (DataSignal*)CurveIterator.CurrentItem ();

		if (nextSignal == NULL)
			break;

		if ((nextSignal->IsNoticeInList (&pullup)) && (!nextSignal->IsNoticeInList (&primaryPullup))) {

			--CurveIterator;
			continue;
		}

		//if (nextSignal->IsNoticeInList (&primaryPullup)) {

		//	--CurveIterator;
		//	continue;
		//}

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

		if ((nextSignal->IsNoticeInList (&pullup)) && (!nextSignal->IsNoticeInList (&primaryPullup)))
			continue;

		if (nextSignal->IsNoticeInList (&signalAboveMaxRFU))
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
		newNotice = new ILSHasTooFewPeaks;
		info = " with ";
		info << PreliminaryCurveList.Entries () << " peaks out of " << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);

		cout << ErrorString << endl;
		cout << "There are " << PreliminaryCurveList.Entries () << " curves needed out of " << Size << endl;
		return -50;
	}

	if (nextSignal == NULL) {

		if (print)
			msg.WriteInsufficientPeaksForILS ();

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";
		status = -1;
		newNotice = new ILSHasTooFewPeaks;
		info = " with ";
		info << "no peaks out of " << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);
		cout << ErrorString << endl;
		cout << "There are no peaks available in the ILS" << endl;
		return -50;
	}

	if (PreliminaryCurveList.Entries () < Size) {

		if (print)
			msg.WriteInsufficientPeaksForILS ();

		ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";
		status = -1;
		newNotice = new ILSHasTooFewPeaks;
		info = " with ";
		info  << PreliminaryCurveList.Entries () << " peaks out of " << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);
		cout << ErrorString << endl;
		cout << "There are too few peaks available in the ILS:  " << PreliminaryCurveList.Entries () << " peaks out of " << Size << endl;
		return -50;
	}

	lastPeak = nextSignal->Peak ();
	CurveIterator.Reset ();
//	CurveIterator.ResetToEnd ();
	
	//int peaksProcessed = 0;
	//int peaksChosen = 0;
	//double maxPeakOnRight = 0.0;

	//while (peaksProcessed < Size) {

	//	nextSignal = (DataSignal*) CurveIterator.CurrentItem ();
	//	peaksProcessed++;

	//	if (nextSignal == NULL)
	//		break;

	//	if (nextSignal->IsNoticeInList (&pullup))
	//		continue;

	//	if (nextSignal->IsNoticeInList (&signalAboveMaxRFU))
	//		continue;

	//	currentPeak = nextSignal->Peak ();
	//	--CurveIterator;
	//	peaksChosen++;

	//	avePeak += currentPeak;

	//	if (currentPeak > maxPeakOnRight)
	//		maxPeakOnRight = currentPeak;
	//}

	// Testing....
	//while (nextSignal = (DataSignal*) CurveIterator ())
	//	avePeak += nextSignal->Peak ();

	//avePeak = avePeak / (double)PreliminaryCurveList.Entries ();

	if (peaksChosen > 0)
		avePeak = avePeak / (double)peaksChosen;

	int origNumberOfPeaks = PreliminaryCurveList.Entries ();
	curvesLeft = PreliminaryCurveList.Entries ();

	if ((secondaryPeak > 0) && (peaksChosen > 1) && (maxPeakOnRight > 3.0 * secondaryPeak))
		maxPeakOnRight = secondaryPeak;

	if (maxPeakOnRight > 3.0 * avePeak)
		maxPeakOnRight = 2.0 * avePeak;

//	if (maxPeakOnRight < 2.0 * avePeak)
//		avePeak = maxPeakOnRight;

	// Maybe use 2.0 * avePeak as maximum allowed peak and remove all peaks above that????

	// move PreliminaryCurveList peaks to a temporary list so that this can be undone!

	ClearAndRepopulateFromList (PreliminaryCurveList, tempCurveList, overFlow);

	if (lastPeak <= endTestFactor * maxPeak) {

		curvesLeft = Locus::DivideListByPeakHeight (tempCurveList, overFlow, indivList, avePeak, maxPeak, scaleFactor, Size);
	}

	if (curvesLeft < Size) {

		// add message about dividing by peak height failure - and beginning "junk" - then restore and continue.  This failure may
		// predict overall failure, but it may just signal a problem with the thresholds used for dividing the data.  See earlier comment
		// about using a temporary list

		newNotice = new CannotSeparateILSInitialFragments;
		AddNoticeToList (newNotice);
		ClearAndRepopulateFromList (PreliminaryCurveList, tempCurveList, overFlow);
		indivList.Clear ();
		overFlow.Clear ();
	}

	// Regardless of how we got here, tempCurveList has the curves we want

	nextSignal = (DataSignal*) overFlow.Last ();

	if (nextSignal != NULL)
		endOfJunk = nextSignal->GetMean ();

	else
		endOfJunk = 0.0;

	MergeListAIntoListB (overFlow, ArtifactList);

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

		if ((nextSignal->IsNoticeInList (&pullup)) && (!nextSignal->IsNoticeInList (&primaryPullup)))
			continue;

		//if (nextSignal->IsNoticeInList (&primaryPullup))
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

			TestResult = TestNeighbors (PreviousSignal, nextSignal, FollowingSignal);  // tests for stutter/adenylation

			if (TestResult <= 0)
				overFlow.Append (nextSignal);

			else
				FinalCurveList.Append (nextSignal);
		}

		PreviousSignal = nextSignal;
	}

	if (FinalCurveList.Entries () < Size) {

		if (FinalCurveList.Entries () + nFilter < Size) {
		
			newNotice = new ILSNeighborFiltersRemoveTooManyPeaks;
			info = "Remaining peaks = ";
			info << FinalCurveList.Entries () << " out of " << Size;
			newNotice->AddDataItem (info);
			AddNoticeToList (newNotice);

			// Repopulate from saved list:  totallyTempCurveList
			ClearAndRepopulateFromList (totallyTempCurveList, FinalCurveList, overFlow);
			indivList.Clear ();
		}

		else {

			while (nextSignal = (DataSignal*) indivList.GetFirst ()) {

				newNotice = new SignalPeakBelowFractionalFilterHeight;
				nextSignal->AddNoticeToList (newNotice);
				FinalCurveList.InsertWithNoReferenceDuplication (nextSignal);
			}

			newNotice = new ILSHeightFiltersRemoveTooManyPeaks;
			AddNoticeToList (newNotice);
		}
	}

	MergeListAIntoListB (overFlow, ArtifactList);  // overFlow is empty after this, whichever way we got here
	MergeListAIntoListB (indivList, ArtifactList);
	bool relativeHeightsFailed = false;
	bool testedRelativeHeights = false;

	// Now we use FinalCurveList, which has at least Size elements, whichever path got us here...
	// Save FinalCurveList in tempCurveList in case we need to try to fit twice...

	ClearAndRepopulateFromList (FinalCurveList, tempCurveList, overFlow);  // tempCurveList is our backup...

	RGDListIterator FinalIterator (FinalCurveList);
	RGDListIterator FollowingIterator (FinalCurveList);
	
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
					newNotice = new PeakOutOfPlaceInILS;
					data = "(Relative height imbalance)";
					newNotice->AddDataItem (data);
					nextSignal->AddNoticeToList (newNotice);
					ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
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
					newNotice = new PeakOutOfPlaceInILS;
					data = "(Relative height imbalance)";
					newNotice->AddDataItem (data);
					nextSignal->AddNoticeToList (newNotice);
					ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
				}

				else {

					FinalCurveList.Append (nextSignal);
					break;
				}
			}

			if (FinalCurveList.Entries () < Size) {

				newNotice = new ILSRelativeHeightsInconsistent;
				AddNoticeToList (newNotice);
//				ClearAndRepopulateFromList (tempCurveList, FinalCurveList, overFlow);
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

//					ClearAndRepopulateFromList (tempCurveList, FinalCurveList, overFlow);
					correlation = 0.0;
					relativeHeightsFailed = true;
				}

				else {

					correlation = DotProductWithQuadraticFit (FinalCurveList, Size, actualArray, differenceArray, leftNorm2s [Size-2]);

					if (correlation < 0.993)
						relativeHeightsFailed = true;
				}
			}
		}

		bool searchForSubset = (testedRelativeHeights && relativeHeightsFailed) || (!testedRelativeHeights);

		if (testedRelativeHeights && relativeHeightsFailed) {

			ClearAndRepopulateFromList (tempCurveList, FinalCurveList, overFlow);
			newNotice = new ILSRelativeHeightsInconsistent;
			AddNoticeToList (newNotice);
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

			//if (recursiveStatus >= 0)
			//	correlation = DotProductWithQuadraticFit (FinalCurveList, Size, actualArray, differenceArray, leftNorm2s [Size-2]);

			//if ((recursiveStatus < 0) || (correlation < 0.993)) {

			//	startPts = Size;
			//	ClearAndRepopulateFromList (tempCurveList, FinalCurveList, overFlow);

			//	// remove all pullups in desperation!!!
			//	FinalIterator.Reset ();

			//	while (nextSignal = (DataSignal*) FinalIterator ()) {

			//		if (nextSignal->IsNoticeInList (&pullup)) {

			//			FinalIterator.RemoveCurrentItem ();
			//			overFlow.Append (nextSignal);
			//		}
			//	}

			//	if (FinalCurveList.Entries () <= sizeFactor)
			//		startPts = Size;

			//	else {

			//		startPts = (Size / 2);

			//		if (2 * startPts == Size)
			//			startPts--;

			//		if (startPts > 6)
			//			startPts = 6;
			//	}

			//	recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (FinalCurveList, overFlow, ctlInfo, correlation, 0.98, startPts, MaxPeak);
			//}
		}
		
		overflowIterator.Reset ();
			
		while (nextSignal = (DataSignal*) overflowIterator ()) {

			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
				"Peak out of place in final list from 'Search Two Sided':  uncategorized artifact");
			newNotice = new PeakOutOfPlaceInILS;
			nextSignal->AddNoticeToList (newNotice);
		}

		MergeListAIntoListB (overFlow, ArtifactList);

		if (recursiveStatus < 0) {

			if (print)
				msg.WriteInsufficientPeaksForILS ();

			if (status == 0)
				ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";

			newNotice = new ILSHasTooFewPeaks;
			info = " with ";
			info << FinalCurveList.Entries () << " peaks out of " << Size;
			newNotice->AddDataItem (info);
			AddNoticeToList (newNotice);

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

	if (correlation < 0.993) {

		if (status == 0)
			ErrorString << "INTERNAL LANE STANDARD DOES NOT MEET EXPECTATIONS...Correlation with ideal set " << correlation << " below 0.993.\n";

		newNotice = new ILSSpacing;
		info = " Correlation = ";
		info << correlation << " (0.993 - 1.0)";
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);

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

		newNotice = new ILSHasTooFewPeaks;
		info = " with 0 peaks out of ";
		info << Size;
		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);

		while (nextSignal = (DataSignal*) CompleteIterator ())
			nextSignal->ReportNotices (ExcelText, "\t\t", "\t");

		cout << ErrorString << endl;
		return -1;
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
		}

		if (print)
			nextSignal->Report (text, "    ");

		i++;
	}

	double lowMean = Means [0];
	double highMean = Means [NumberOfAcceptedCurves - 1];
	double currentMean;
	
	while (nextSignal = (DataSignal*) indivList.GetFirst ()) {

		currentMean = nextSignal->GetMean ();

		if ((currentMean < lowMean) || (currentMean > highMean))
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
			"Peak represents measurement at base pair level outside internal lane standard");

		else
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", 
			"Signal out of place in ILS - uncategorized artifact");

		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
	}

	QuadraticFit fit (Means, NumberOfAcceptedCurves);
	fit.Regress (Sigmas, QFit);

	QuadraticFit fit2 (Means, NumberOfAcceptedCurves);
	fit2.Regress (SecondaryContent, QFit2);
	mInverseSouthern = new CSplineTransform (actualArray, Means, NumberOfAcceptedCurves);
	mGlobalSouthern = new CSplineTransform (Means, actualArray, NumberOfAcceptedCurves);

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


int STRLaneStandardChannelData :: CorrectLaneStandardCrossChannelAnalysis () {

	RGDListIterator it (ArtifactList);
	DataSignal* nextSignal;
	StutterFound stutter;
	PullUpFound pullup;
	AdenylationFound adenylation;
	PullUpPrimaryChannel primary;
	RemovablePrimaryPullUp removable;
	AllelePullUp allelePullup;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->IsNoticeInList (&pullup)) {

			if ((nextSignal->IsNoticeInList (&stutter)) || (nextSignal->IsNoticeInList (&adenylation)))
				nextSignal->RemoveCrossChannelSignalLink (&primary, &pullup, &removable, NULL);
		}
	}

	RGDListIterator ilsIterator (FinalCurveList);

	while (nextSignal = (DataSignal*) ilsIterator ()) {

		if (nextSignal->IsNoticeInList (&pullup)) {

//			nextSignal->RemoveCrossChannelSignalLink (&primary, &pullup, &removable, &allelePullup);
			nextSignal->RemoveCrossChannelSignalLink (&primary, &pullup, NULL, NULL);
		}
	}

	return 0;
}


int STRLaneStandardChannelData :: FinalTestForCriticalLaneStandardNotices () {

	bool criticalArtifactFound = false;
	int criticalLevel = Notice::GetMessageTrigger ();
	int nextSignalLevel;
	RGDListIterator it (FinalCurveList);
	DataSignal* nextSignal;
	Notice* newNotice;
	RGString info;
	PeakOutOfPlaceInILS removeNotice;

	while (nextSignal = (DataSignal*) it ()) {

		newNotice = nextSignal->RemoveNotice (&removeNotice);
		delete newNotice;
	}

	it.Reset ();

	const double* omissionArray;
	int omissionSize = mLaneStandard->GetOmissionArray (omissionArray);
	int kk = 0;
	double testBP;
	double lowerTest;
	double upperTest;

	if (omissionSize > 0) {

		RGDListIterator artIt (ArtifactList);
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

				if (localBp < upperTest) {

					newNotice = nextSignal->RemoveNotice (&removeNotice);
					delete newNotice;
//					nextSignal->SetAlleleName ("(250)");
				}

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

	while (nextSignal = (DataSignal*) it ()) {

		nextSignalLevel = nextSignal->GetHighestMessageLevel ();

		if ((nextSignalLevel > 0) && (nextSignalLevel <= criticalLevel)) {

			if (!criticalArtifactFound) {

				criticalArtifactFound = true;
				newNotice = new ILSHasPeaksWithCriticalNotices;
			}

			else
				info << ", ";

			info << nextSignal->GetAlleleName () << " bps";
		}
	}

	if (criticalArtifactFound) {

		newNotice->AddDataItem (info);
		AddNoticeToList (newNotice);
		return -1;
	}

	return 0;
}


double STRLaneStandardChannelData :: GetWidthAtTime (double t) const {

	return QFit (t);
}


double STRLaneStandardChannelData :: GetSecondaryContentAtTime (double t) const {

	return QFit2 (t);
}


double STRLaneStandardChannelData :: GetTimeForSpecifiedID (double id) {

	if (mInverseSouthern == NULL)
		return 0.0;
	
	return mInverseSouthern->EvaluateWithExtrapolation (id);
}


CoordinateTransform* STRLaneStandardChannelData :: GetIDMap () {

	return mGlobalSouthern;
}


double STRLaneStandardChannelData :: GetMinimumHeight () const {

	return STRLaneStandardChannelData::minLaneStandardRFU;
}


double STRLaneStandardChannelData :: GetMaximumHeight () const {

	return STRLaneStandardChannelData::maxLaneStandardRFU;
}


double STRLaneStandardChannelData :: GetDetectionThreshold () const {

	return STRLaneStandardChannelData::minLaneStandardRFU;
}


double STRLaneStandardChannelData :: GetMeasurementRatio () const {

	const double* actualArray;
	mLaneStandard->GetCharacteristicArray (actualArray);
	int Size = FinalCurveList.Entries ();
	double maxDeltaActual = actualArray [Size - 1] - actualArray [0];
	DataSignal* firstSignal = (DataSignal*) FinalCurveList.First ();
	DataSignal* lastSignal = (DataSignal*) FinalCurveList.Last ();

	if ((firstSignal == NULL) || (lastSignal == NULL))
		return 0.0;

	double maxDeltaMeans = lastSignal->GetMean () - firstSignal->GetMean ();

	if (maxDeltaActual <= 0.0)
		return 0.0;

	if (maxDeltaMeans < 2.5 * maxDeltaActual)
		return 0.0;

	return maxDeltaMeans / maxDeltaActual;
}


double STRLaneStandardChannelData :: DotProductWithQuadraticFit (RGDList& set, int size, const double* idealValues, const double* idealDifferences, double idealNorm2) {

	QuadraticFit fit;
	double* dataArray = new double [size];
	double* transformedDifferences = new double [size];
	RGDListIterator it (set);
	DataSignal* nextSignal;
	int i = 0;
	Quadratic Q;
	int size1 = size - 1;
	bool qWorked = true;

	for (i=0; i<size; i++) {

		nextSignal = (DataSignal*) it ();

		if (nextSignal == NULL) {

			dataArray [i] = 0.0;
			qWorked = false;
		}

		else
			dataArray [i] = nextSignal->GetMean ();
	}

	Boolean itWorked;
	
	if (qWorked)
		itWorked = fit.Regress (dataArray, idealValues, size, Q);

	else
		itWorked = FALSE;

	if (!itWorked) {

		// don't transform because it's basically linear
		
		for (i=0; i<size1; i++)
			transformedDifferences [i] = dataArray [i+1] - dataArray [i];
	}

	else {

		double lastTransform = Q.Evaluate (dataArray [0]);
		double thisTransform;

		for (i=0; i<size1; i++) {

			thisTransform = Q.Evaluate (dataArray [i+1]);
			transformedDifferences [i] = thisTransform - lastTransform;
			lastTransform = thisTransform;
		}
	}

	double norm2 = 0.0;
	double temp;
	double dot = 0.0;

	for (i=0; i<size1; i++) {

		temp = transformedDifferences [i];
		norm2 += temp * temp;
		dot += temp * idealDifferences [i];
	}

	dot = dot / sqrt (idealNorm2 * norm2);
	delete[] transformedDifferences;
	delete[] dataArray;
	return dot;
}


STRLadderChannelData :: STRLadderChannelData () : STRChannelData () {

}


STRLadderChannelData :: STRLadderChannelData (int channel) : STRChannelData (channel) {

}


STRLadderChannelData :: STRLadderChannelData (int channel, LaneStandard* inputLS) : STRChannelData (channel, inputLS) {

}


STRLadderChannelData :: STRLadderChannelData (const STRLadderChannelData& strLS) :
STRChannelData (strLS) {

}


STRLadderChannelData :: STRLadderChannelData (const STRLadderChannelData& strLadder, CoordinateTransform* trans) :
STRChannelData (strLadder, trans) {

}


STRLadderChannelData :: ~STRLadderChannelData () {

}

int STRLadderChannelData :: SetData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	int status = STRChannelData::SetData (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLadderChannelData :: SetRawData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	int status = STRChannelData::SetRawData (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRLadderChannelData :: AnalyzeGridLoci (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	int status = 0;
	DataSignal* nextSignal;
	Endl endLine;
	ExcelText << CLevel (1) << endLine << "Ladder data for Channel Number " << mChannel << endLine << PLevel ();

	//  All the characteristics we are looking for are in the PreliminaryCurveList

	while (nextLocus = (Locus*) it()) {

		if (nextLocus->AnalyzeGridLocus (ArtifactList, PreliminaryCurveList, SupplementalArtifacts, text, ExcelText, msg, print) < 0) {

			status = -1;
		}
	}

	RGDListIterator PreliminaryIterator (PreliminaryCurveList);

	while (nextSignal = (DataSignal*) PreliminaryIterator ()) {

		nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
							"Peak out of place in final list:  interlocus peak");
	}

	RGDListIterator CompleteIterator (CompleteCurveList);

	while (nextSignal = (DataSignal*) CompleteIterator ()) {

		nextSignal->ReportNotices (ExcelText, "\t\t", "\t");
//		ExcelText << "\t\tPeak = \t" << nextSignal->Peak () << endLine;
	}

	return status;
}



bool STRLadderChannelData :: TestIfTimeIsLeftOfLocus (double time, Locus* locus, Locus* assocGridLocus) {

	if (locus == NULL)
		return false;
	
	if (locus->TestSignalTimeRelativeToGridLocus (time) < 0)
		return true;

	return false;
}


ChannelData* STRLadderChannelData :: CreateNewTransformedChannel (const ChannelData& cd, CoordinateTransform* trans) {

	return new STRLadderChannelData ((const STRLadderChannelData&) cd, trans);
}


double STRLadderChannelData :: GetMinimumHeight () const {

	return STRLadderChannelData::minLadderRFU;
}


double STRLadderChannelData :: GetMaximumHeight () const {

	return STRLadderChannelData::maxLadderRFU;
}


double STRLadderChannelData :: GetFractionalFilter () const {

	return Locus::GetGridFractionalFilter ();
}


double STRLadderChannelData :: GetDetectionThreshold () const {

	return STRLadderChannelData::minLadderRFU;
}


STRSampleChannelData :: STRSampleChannelData () : STRChannelData () {

}


STRSampleChannelData :: STRSampleChannelData (int channel) : STRChannelData (channel) {

}


STRSampleChannelData :: STRSampleChannelData (int channel, LaneStandard* inputLS) : STRChannelData (channel, inputLS) {

}


STRSampleChannelData :: STRSampleChannelData (const STRSampleChannelData& strLS) :
STRChannelData (strLS) {

}


STRSampleChannelData :: STRSampleChannelData (const STRSampleChannelData& strSample, CoordinateTransform* trans) :
STRChannelData (strSample, trans) {

}


STRSampleChannelData :: ~STRSampleChannelData () {

}

int STRSampleChannelData :: SetData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	int status = STRChannelData::SetData (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testSamplePeak;
	return 0;
}


int STRSampleChannelData :: SetRawData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak) {

	int status = STRChannelData::SetRawData (fileData, testControlPeak, testSamplePeak);
	
	if (status < 0)
		return status;

	mTestPeak = testControlPeak;
	return 0;
}


int STRSampleChannelData :: AnalyzeSampleLoci (ChannelData* lsData, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;

	while (nextLocus = (Locus*) it ())
		nextLocus->AnalyzeSampleLocus (lsData, ArtifactList, PreliminaryCurveList, text, ExcelText, msg, print);

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->PromoteNonAmbiguousSignalsToAlleles (PreliminaryCurveList);

	return 0;
}


int STRSampleChannelData :: TestFractionalFilters () {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	RGDList fractionalFilterList;

	while (nextLocus = (Locus*) it ())
		nextLocus->TestFractionalFilters (ArtifactList, fractionalFilterList);

	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) fractionalFilterList.GetFirst ()) {

		if ((!nextSignal->IsPossibleInterlocusAllele (-1)) && (!nextSignal->IsPossibleInterlocusAllele (1))) {
		
			ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
			PreliminaryCurveList.RemoveReference (nextSignal);
			nextSignal->SetAlleleName ("");
		}
	}

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->PromoteNonAmbiguousSignalsToAlleles (PreliminaryCurveList);

	return 0;
}


int STRSampleChannelData :: TestForInterlocusProximityArtifacts () {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	RGDList adenylationList;
	RGDList stutterList;
	//unsigned signalId;

	while (nextLocus = (Locus*) it ())
		nextLocus->TestInterlocusSignals (PreliminaryCurveList, ArtifactList, adenylationList, stutterList);

	// Now clean up artifact lists, then test remaining signals for adenylation

	DataSignal* nextSignal;
	DataSignal* testSignal;
	Notice* newNotice;

	while (nextSignal = (DataSignal*) adenylationList.GetFirst ()) {

		//signalId = nextSignal->GetSignalID ();
		newNotice = new AdenylationFound;
		nextSignal->AddNoticeToList (newNotice);
		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
		PreliminaryCurveList.RemoveReference (nextSignal);
		nextSignal->SetAlleleName ("");

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

		//signalId = nextSignal->GetSignalID ();
		newNotice = new StutterFound;
		nextSignal->AddNoticeToList (newNotice);
		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
		PreliminaryCurveList.RemoveReference (nextSignal);
		nextSignal->SetAlleleName ("");

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

			if (testID < leftLimit)
				continue;

			if (testID > rightLimit)
				break;

			if (testID < leftUpperLimit)
				cumulativePeak += testSignal->Peak ();

			else if (testID > rightLowerLimit)
				cumulativePeak += testSignal->Peak ();
		}

		if (peak <= threshold * cumulativePeak) {

			signalIterator.RemoveCurrentItem ();
			adenylationList.Append (nextSignal);
		}
	}

	while (nextSignal = (DataSignal*) adenylationList.GetFirst ()) {

		//signalId = nextSignal->GetSignalID ();
		newNotice = new AdenylationFound;
		nextSignal->AddNoticeToList (newNotice);
		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
		nextSignal->SetAlleleName ("");

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


int STRSampleChannelData :: AssignSampleCharacteristicsToLoci (CoreBioComponent* grid, CoordinateTransform* timeMap) {

	//  Assume sample characteristics are in PreminaryCurveList.
	//
	//  Get each locus and find corresponding locus in grid.  Then, extract signals for that locus.
	//

	Locus* nextLocus;
	Locus* gridLocus;
	RGDListIterator it (mLocusList);

	while (nextLocus = (Locus*) it()) {

		gridLocus = grid->FindLocus (mChannel, nextLocus->GetLocusName ());

		if (gridLocus == NULL)
			return -1;  // this should never happen...it means that the channel has a locus that the grid has never heard of, but have to test...

		nextLocus->ExtractSampleSignals (PreliminaryCurveList, gridLocus, timeMap);
	}

	it.Reset ();

	while (nextLocus = (Locus*) it ()) {

		gridLocus = grid->FindLocus (mChannel, nextLocus->GetLocusName ());

		if (gridLocus == NULL)
			return -1;  // this should never happen...it means that the channel has a locus that the grid has never heard of, but have to test...

		nextLocus->ExtractExtendedSampleSignals (PreliminaryCurveList, gridLocus, timeMap);
	}

	// Done extracting all signals from list that lie within a locus.  What remains is inside the internal lane standard but outside all loci.  For now,
	// add to artifact list...

	//it.Reset ();

	//while (nextLocus = (Locus*) it()) {

	//	gridLocus = grid->FindLocus (mChannel, nextLocus->GetLocusName ());

	//	if (gridLocus == NULL)
	//		return -1;  // this should never happen...it means that the channel has a locus that the grid has never heard of, but have to test...

	//	nextLocus->ExtractExtendedLadderSampleSignals (PreliminaryCurveList, gridLocus, timeMap);
	//}

	// Now we have extracted interlocus peaks that may belong to the extended loci.  There may be some overlap, so we have to watch out!  

	return 0;
}


int STRSampleChannelData :: FinalTestForPeakSizeAndNumber (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes) {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	int status = 0;
	UnexpectedNumberOfPeaks target;
	Notice* newNotice;

	while (nextLocus = (Locus*) it ()) {

		if (nextLocus->FinalTestForPeakSizeAndNumber (averageHeight, isNegCntl, isPosCntl, pGenotypes, ArtifactList) < 0)
			status = -1;
	}

	if (isNegCntl) {

		it.Reset ();

		while (nextLocus = (Locus*) it ()) {

			if (nextLocus->IsNoticeInList (&target)) {

				if (!IsNoticeInList (&target)) {

					newNotice = new UnexpectedNumberOfPeaks;
					AddNoticeToList (newNotice);
				}
			}
		}
	}

	return status;
}


bool STRSampleChannelData :: TestIfTimeIsLeftOfLocus (double time, Locus* locus, Locus* assocGridLocus) {

	if (assocGridLocus == NULL)
		return false;
	
	return (assocGridLocus->TestSignalTimeRelativeToGridLocus (time) < 0);
}


int STRSampleChannelData :: TestForMultiSignals () {

	// All signals outside lane standard have already been removed.  Also, all signals definitely assigned to a locus are gone.
	// What's left are ambiguous signals and also those that are truly interlocus.  Test the ambiguous ones for possible craters...

	RGDListIterator it (PreliminaryCurveList);
	DataSignal* nextSignal;
	DataSignal* prevSignal = NULL;

	DataSignal* prevCrater;
	DataSignal* nextCrater;
	DataSignal* craterSignal;

	Locus* nextLocus;
	const Locus* flankingLocusLeft;
	const Locus* flankingLocusRight;
	RGDListIterator locusIt (mLocusList);

	bool multipleSignalsLeft;
	bool multipleSignalsRight;

	//unsigned signalId;
	//unsigned signalIdLeft;
	//unsigned signalIdRight;

	Crater craterNotice;
	Notice* retrievedNotice;
	Notice* newNotice;
	InterchannelLinkage* iChannel;

	RGDList SignalsToDeleteFromSignalList;
	RGDList SignalsToDeleteFromAll;

	PullUpFound pullupNotice;
	PullUpPrimaryChannel primaryPullupNotice;

	while (nextLocus = (Locus*) locusIt ())
		nextLocus->PromoteNonAmbiguousSignalsToAlleles (PreliminaryCurveList);

	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		// First check for false craters
		//signalId = nextSignal->GetSignalID ();

		multipleSignalsLeft = nextSignal->TestForMultipleSignals (prevCrater, nextCrater, -1);
		multipleSignalsRight = nextSignal->TestForMultipleSignals (prevCrater, nextCrater, 1);

		//if (prevCrater != NULL)
		//	signalIdLeft = prevCrater->GetSignalID ();

		//else
		//	signalIdLeft = 0;

		//if (nextCrater != NULL)
		//	signalIdRight = nextCrater->GetSignalID ();

		//else
		//	signalIdRight = 0;

		if (multipleSignalsLeft || multipleSignalsRight) {

			SignalsToDeleteFromAll.Append (nextSignal);

			if (prevCrater != NULL) {

				retrievedNotice = prevCrater->RemoveNotice (&craterNotice);
				delete retrievedNotice;
				prevCrater->RemoveSignalLink ();
			}

			if (nextCrater != NULL) {

				retrievedNotice = nextCrater->RemoveNotice (&craterNotice);
				delete retrievedNotice;
				nextCrater->RemoveSignalLink ();
			}

			if (multipleSignalsLeft && multipleSignalsRight) {

				// This is a true multiple...undo the crater signal and delete it from both loci and all other lists and remove
				// crater notice from "flanking" signals

				if (prevCrater != NULL) {

					if (prevCrater->NumberOfNoticeObjects () == 0)
						ArtifactList.RemoveReference (prevCrater);
				}

				if (nextCrater != NULL) {

					if (nextCrater->NumberOfNoticeObjects () == 0)
						ArtifactList.RemoveReference (nextCrater);
				}
			}

			else {

				// This is a true ambiguous because the interpeak spacing varies with locus.  Undo the crater as above
				// and add both flanking peaks to artifact list with appropriate notices.  Remove flanking peaks from
				// loci.

				if (prevCrater != NULL) {

					newNotice = new AmbiguousInterlocusCraterAnalysis;
					prevCrater->AddNoticeToList (newNotice);
					SignalsToDeleteFromSignalList.Append (prevCrater);
				}

				if (nextCrater != NULL) {

					newNotice = new AmbiguousInterlocusCraterAnalysis;
					nextCrater->AddNoticeToList (newNotice);
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

		nextSignal->SetVirtualAlleleName ("");
		nextSignal->SetAlleleName ("");
		PreliminaryCurveList.RemoveReference (nextSignal);
		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
		nextLocus = (Locus*) nextSignal->GetLocus (-1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);

		nextLocus = (Locus*) nextSignal->GetLocus (1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);
	}

	while (nextSignal = (DataSignal*) SignalsToDeleteFromAll.GetFirst ()) {

		PreliminaryCurveList.RemoveReference (nextSignal);
		ArtifactList.RemoveReference (nextSignal);
		CompleteCurveList.RemoveReference (nextSignal);
		nextLocus = (Locus*) nextSignal->GetLocus (-1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);

		nextLocus = (Locus*) nextSignal->GetLocus (1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);

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

					// This is a crater...

					newNotice = new Crater;
					prevSignal->AddNoticeToList (newNotice);
					newNotice = new Crater;
					nextSignal->AddNoticeToList (newNotice);
					ArtifactList.InsertWithNoReferenceDuplication (prevSignal);
					ArtifactList.InsertWithNoReferenceDuplication (nextSignal);

					SignalsToDeleteFromSignalList.Append (prevSignal);
					SignalsToDeleteFromSignalList.Append (nextSignal);

					craterSignal = new CraterSignal (prevSignal, nextSignal);
					newNotice = new Crater;
					craterSignal->AddNoticeToList (newNotice);
					ArtifactList.InsertWithNoReferenceDuplication (craterSignal);
					CompleteCurveList.InsertWithNoReferenceDuplication (craterSignal);
					PreliminaryCurveList.InsertWithNoReferenceDuplication (craterSignal);

					nextLocus = (Locus*) flankingLocusLeft;
					nextLocus->InsertSignalExtendedRight (craterSignal);

					nextLocus = (Locus*) flankingLocusRight;
					nextLocus->InsertSignalExtendedLeft (craterSignal);

					prevSignal = NULL;
				}

				else {

					// These are ambiguous signals...close enough to be called the same in one locus but not another

					newNotice = new AmbiguousInterlocusCraterAnalysis;
					prevSignal->AddNoticeToList (newNotice);
					SignalsToDeleteFromSignalList.Append (prevSignal);

					newNotice = new AmbiguousInterlocusCraterAnalysis;
					nextSignal->AddNoticeToList (newNotice);
					SignalsToDeleteFromSignalList.Append (nextSignal);

					prevSignal = NULL;
				}
			}

			else if (prevSignal->GetAlleleName (1) == nextSignal->GetAlleleName (1)) {

				// These are ambiguous signals...close enough to be called the same in one locus but not another

				newNotice = new AmbiguousInterlocusCraterAnalysis;
				prevSignal->AddNoticeToList (newNotice);
				SignalsToDeleteFromSignalList.Append (prevSignal);

				newNotice = new AmbiguousInterlocusCraterAnalysis;
				nextSignal->AddNoticeToList (newNotice);
				SignalsToDeleteFromSignalList.Append (nextSignal);

				prevSignal = NULL;
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

		nextSignal->SetVirtualAlleleName ("");
		nextSignal->SetAlleleName ("");
		PreliminaryCurveList.RemoveReference (nextSignal);
		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
		nextLocus = (Locus*) nextSignal->GetLocus (-1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);

		nextLocus = (Locus*) nextSignal->GetLocus (1);

		if (nextLocus != NULL)
			nextLocus->RemoveSignalFromLocusList (nextSignal);
	}

	return 0;
}


int STRSampleChannelData :: TestProximityArtifacts () {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	RGDList stutterList;
	RGDList adenylationList;
	Notice* newNotice;

	while (nextLocus = (Locus*) it ())
		nextLocus->TestProximityArtifacts (ArtifactList, adenylationList, stutterList);

	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) stutterList.GetFirst ()) {

		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
		PreliminaryCurveList.RemoveReference (nextSignal);
		nextSignal->SetAlleleName ("");
		newNotice = new StutterFound;
		nextSignal->AddNoticeToList (newNotice);
		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);

		if (adenylationList.RemoveReference (nextSignal) != NULL) {

			newNotice = new AdenylationFound;
			nextSignal->AddNoticeToList (newNotice);
		}

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

	while (nextSignal = (DataSignal*) adenylationList.GetFirst ()) {

		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
		PreliminaryCurveList.RemoveReference (nextSignal);
		nextSignal->SetAlleleName ("");
		newNotice = new AdenylationFound;
		nextSignal->AddNoticeToList (newNotice);
		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);

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

	// Test interlocus peaks for their proximity to each locus

//	it.Reset ();

//	while (nextLocus = (Locus*) it ())
//		nextLocus->PromoteNonAmbiguousSignalsToAlleles (PreliminaryCurveList);

	return 0;
}


int STRSampleChannelData :: AssignSignalToFirstLocusAndDeleteFromSecond (DataSignal* target, Locus* locus1, Locus* locus2) {

	Notice* newNotice;
	ArtifactList.InsertWithNoReferenceDuplication (target);
	locus1->PromoteSignalToAllele (target);
	locus2->RemoveSignalFromLocusList (target);
	newNotice = new PossibleAmbiguousInterlocusPeakAssignedToLocus;
	target->AddNoticeToList (newNotice);
	return 0;
}


int STRSampleChannelData :: RemoveSignalFromBothLoci (DataSignal* target, Locus* locus1, Locus* locus2) {

	Notice* newNotice;
	ArtifactList.InsertWithNoReferenceDuplication (target);
	locus1->RemoveSignalFromLocusList (target);
	locus2->RemoveSignalFromLocusList (target);
	newNotice = new AmbiguousInterlocusPeak;
	target->AddNoticeToList (newNotice);
	return 0;
}


bool STRSampleChannelData :: TestIfSignalBelongsToFirstLocus (DataSignal* target, Locus* locus1, Locus* locus2) {

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
	PullUpFound pullup;
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

	if (target->IsNoticeInList (&pullup))
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


int STRSampleChannelData :: ResolveAmbiguousInterlocusSignals () {

	RGDListIterator it (PreliminaryCurveList);
	DataSignal* nextSignal;
	bool wouldCauseHeterozygousImbalance1;
	bool wouldCauseHeterozygousImbalance2;
	Locus* locus1;
	Locus* locus2;
	bool locus1IsIt;
	bool locus2IsIt;
	//unsigned signalId;

	while (nextSignal = (DataSignal*) it ()) {

		//signalId = nextSignal->GetSignalID ();

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

					locus1IsIt = TestIfSignalBelongsToFirstLocus (nextSignal, locus1, locus2);
					locus2IsIt = TestIfSignalBelongsToFirstLocus (nextSignal, locus2, locus1);

					if (locus1IsIt && locus2IsIt) {

						locus1IsIt = false;
						locus2IsIt = false;
					}
				}
			}

			it.RemoveCurrentItem ();

			if (locus1IsIt)
				AssignSignalToFirstLocusAndDeleteFromSecond (nextSignal, locus1, locus2);

			else if (locus2IsIt)
				AssignSignalToFirstLocusAndDeleteFromSecond (nextSignal, locus2, locus1);

			else
				RemoveSignalFromBothLoci (nextSignal, locus1, locus2);
		}
	}

	RGDListIterator artIt (ArtifactList);
	ResidualExceedsThreshold residualTarget;
	Notice* newNotice;

	while (nextSignal = (DataSignal*) artIt ()) {

		if (nextSignal->GetAlleleName () == "") {

			newNotice = nextSignal->RemoveNotice (&residualTarget);
			delete newNotice;
		}
	}

	return 0;
}


bool STRSampleChannelData :: TestInterlocusSignalHeightBelowThreshold (DataSignal* signal) {

	double minInterlocusRFU = STRSampleChannelData::GetMinInterlocusRFU ();

	if (signal->Peak () <= minInterlocusRFU)
		return true;

	return false;
}


ChannelData* STRSampleChannelData :: CreateNewTransformedChannel (const ChannelData& cd, CoordinateTransform* trans) {

	return new STRSampleChannelData ((const STRSampleChannelData&) cd, trans);
}


double STRSampleChannelData :: GetMinimumHeight () const {

	return STRSampleChannelData::minSampleRFU;
}


double STRSampleChannelData :: GetMaximumHeight () const {

	return STRSampleChannelData::maxSampleRFU;
}


double STRSampleChannelData :: GetFractionalFilter () const {

	return Locus::GetSampleFractionalFilter ();
}


double STRSampleChannelData :: GetDetectionThreshold () const {

	return STRSampleChannelData::sampleDetectionThreshold;
}


