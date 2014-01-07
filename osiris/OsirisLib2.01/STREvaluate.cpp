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
*  FileName: STREvaluate.cpp
*  Author:   Robert Goor
*
*/
//
//  Subclasses of Evaluate for STR's
//

#include "STREvaluate.h"

PERSISTENT_DEFINITION (STRMinimumRFU, _STRMINIMUMRFU_, "STRMinimumRFU")
PERSISTENT_DEFINITION (STRCrossChannel, _STRCROSSCHANNEL_, "STRCrossChannel")


STRMinimumRFU :: STRMinimumRFU () : Evaluate (), mMinRFU (0.0) {

}


STRMinimumRFU :: STRMinimumRFU (int severity, int report, int timing, const RGString& name, int msgNumber, double minRFU) :
Evaluate (severity, report, timing, name, msgNumber),
mMinRFU (minRFU) {

}


STRMinimumRFU :: STRMinimumRFU (int severity, int report, int timing, int msgNumber, double minRFU) :
Evaluate (severity, report, timing, msgNumber),
mMinRFU (minRFU) {

}


STRMinimumRFU :: STRMinimumRFU (const RGString& xmlInput) :
Evaluate (xmlInput) {

}


STRMinimumRFU :: STRMinimumRFU (const STRMinimumRFU& eval) :
Evaluate ((const Evaluate&)eval),
mMinRFU (eval.mMinRFU) {

}


STRMinimumRFU :: ~STRMinimumRFU () {

}


int STRMinimumRFU :: Test (DataSignal* testSample) {

	if (testSample->Peak () >= mMinRFU)
		return 1;

	return 0;
}


int STRMinimumRFU :: Initialize (RGDList& evalList) {

	return 0;
}


double mWidthFractionForSpike;
	double mWidthMatchFraction;
	double mHeightMatchFractionForSpike;

STRCrossChannel :: STRCrossChannel () : Evaluate (), mTimeTolerance (0.0),
mWidthFractionForSpike (0.0),
mWidthMatchFraction (0.0),
mHeightMatchFractionForSpike (0.0) {

}


STRCrossChannel :: STRCrossChannel (int severity, int report, int timing, const RGString& name, int msgNumber, double timeTolerance,
		double widthFraction, double widthMatch, double heightMatch) :
Evaluate (severity, report, timing, name, msgNumber),
mTimeTolerance (timeTolerance),
mWidthFractionForSpike (widthFraction),
mWidthMatchFraction (widthMatch),
mHeightMatchFractionForSpike (heightMatch) {

}


STRCrossChannel :: STRCrossChannel (int severity, int report, int timing, int msgNumber, double timeTolerance,
		double widthFraction, double widthMatch, double heightMatch) :
Evaluate (severity, report, timing, msgNumber),
mTimeTolerance (timeTolerance),
mWidthFractionForSpike (widthFraction),
mWidthMatchFraction (widthMatch),
mHeightMatchFractionForSpike (heightMatch) {

}


STRCrossChannel :: STRCrossChannel (const RGString& xmlInput) :
Evaluate (xmlInput) {

}


STRCrossChannel :: STRCrossChannel (const STRCrossChannel& eval) :
Evaluate ((const Evaluate&)eval),
mTimeTolerance (eval.mTimeTolerance),
mWidthFractionForSpike (eval.mWidthFractionForSpike),
mWidthMatchFraction (eval.mWidthMatchFraction),
mHeightMatchFractionForSpike (eval.mHeightMatchFractionForSpike) {

}


STRCrossChannel :: ~STRCrossChannel () {

}


int STRCrossChannel :: Test (int channel, ChannelData** channels, int nChannels, CoordinateTransform* trans) {

	RGDList& pList = channels [channel]->GetPreliminaryCurveList ();
	DataSignal* nextSignal;
	DataSignal* testSignal;
	RGDListIterator it (pList);
	int i;
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
	double minMaxPeakCompare = 2.0 * mHeightMatchFractionForSpike - 1.0;

	if (mSeverityLevel == 0)
		foundOneReturnValue = 0;

	else
		foundOneReturnValue = -1;

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

			for (i=1; i<=nChannels; i++) {

				if (i != channel) {
				
					testSignal = possibleBTs [i];
					
					if (testSignal != NULL) {

						currentPeak = testSignal->Peak ();
						currentWidth = testSignal->GetStandardDeviation ();
						
						if (currentPeak > maxPeak) {

							maxPeak = currentPeak;
							maxIndex = i;
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
			calculatedNormalWidth = trans->EvaluateWithExtrapolation (nextSignal->GetMean ());
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

						if (mSeverityLevel == 0)
							(channels [i]->GetArtifactList ()).Insert (testSignal);

						else
							(channels [i]->GetMarginalCurveList ()).Insert (testSignal);

						possibleBTs [i] = NULL;
					}

					else {

						testSignal = possibleBTs [maxIndex];
						testSignal->AddNoticeToList (mReportLevel, "", BTCauseMessage);
						possibleBTs [maxIndex] = NULL;
					}
				}
			}

			returnValue = foundOneReturnValue;  // move this later
		}
	}

	return returnValue;
}


int STRCrossChannel :: Initialize (RGDList& evalList) {

	return 0;
}

