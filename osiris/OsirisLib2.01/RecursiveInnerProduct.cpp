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
*  FileName: RecursiveInnerProduct.cpp
*  Author:   Robert Goor
*
*/
//
//  class RecursiveInnerProduct is designed to manage trees of selected subsets of a set of characteristics with the overall goal of finding the
//  subset that maximizes the fit.
//

#include "RecursiveInnerProduct.h"


RecursiveInnerProduct :: RecursiveInnerProduct (RGDList& signalList, const double* idealPts, const double* idealDiffs, const int* htIndices, const double* idealNorm2s, int setSize, double maxHeight, int allowedDiscrepancy) :
mArraySize (signalList.Entries ()), mMaximumHeight (maxHeight), mIdealPts (idealPts), mIdealDiffs (idealDiffs), mHeightIndices (htIndices), mIdealNorm2s (idealNorm2s), mSetSize (setSize), mClearSignalArray (true),
mIsLastIP (false), mSignalIntervalIsAccurate (true), mMinimumSignalRatio (0.7), mMaximumSignalRatio (1.2), mAllowedHeightDiscrepancy (allowedDiscrepancy) {

	mDiffSize = setSize - 1;
	mSignalArray = new DataSignal* [mArraySize];
	RGDListIterator it (signalList);
	DataSignal* nextSignal;
	int i = 0;

	while (nextSignal = (DataSignal*) it ()) {

		mSignalArray [i] = nextSignal;
		i++;
	}

	mIPs = NULL;
	mNumberOfIPs = 0;
}



RecursiveInnerProduct :: RecursiveInnerProduct (DataSignal** signalArray, int arraySize, const double* idealPts, const double* idealDiffs, const int* htIndices, const double* idealNorm2s, int setSize, double maxHeight, int allowedDiscrepancy) :
mSignalArray (signalArray), mArraySize (arraySize), mMaximumHeight (maxHeight), mIdealPts (idealPts), mIdealDiffs (idealDiffs), mHeightIndices (htIndices), mIdealNorm2s (idealNorm2s), mSetSize (setSize), mClearSignalArray (false),
mIsLastIP (false), mSignalIntervalIsAccurate (true), mMinimumSignalRatio (0.7), mMaximumSignalRatio (1.2), mAllowedHeightDiscrepancy (allowedDiscrepancy) {

	mDiffSize = setSize - 1;
	mIPs = NULL;
	mNumberOfIPs = 0;
}


RecursiveInnerProduct :: ~RecursiveInnerProduct () {

	for (int i=0; i<mNumberOfIPs; i++)
		delete mIPs [i];
	
	if (mNumberOfIPs > 1)
		delete[] mIPs;

	else
		delete mIPs;

	if (mClearSignalArray)
		delete[] mSignalArray;
}


int RecursiveInnerProduct :: BuildFromRight (int ptsRemaining, int extraPts, double clim) {

	if (ptsRemaining < 2)
		return -1;

	if ((ptsRemaining == 2) && (mHeightIndices == NULL))
		return -1;

	mFromLeft = false;

	int totalNewPts = extraPts + 1;
	int extra;
	int nPoints = ptsRemaining;
	int newSignalIndex = mArraySize;
	mIPs = new RecursiveInnerProduct* [totalNewPts];
	mNumberOfIPs = totalNewPts;
	mCurrentSignalIndex = -1;
	double clim2 = clim * clim;

	for (int i=0; i<totalNewPts; i++) {

		extra = extraPts - i;
		newSignalIndex--;
		mIPs [i] = new RecursiveInnerProduct (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mIdealNorm2s, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy);
		mIPs [i]->SetSignalIntervalIsAccurate (mSignalIntervalIsAccurate);
		mIPs [i]->BuildFromRight (0.0, 0.0, nPoints, extra, -1, -1, newSignalIndex, clim2, newSignalIndex);
	}

	return 0;
}


int RecursiveInnerProduct :: BuildFromLeft (int ptsRemaining, int extraPts, double clim) {

	if (ptsRemaining < 2)
		return -1;

	if ((ptsRemaining == 2) && (mHeightIndices == NULL))
		return -1;

	mFromLeft = true;

	int totalNewPts = extraPts + 1;
	int extra;
	int nPoints = ptsRemaining;
	int newSignalIndex = 0;
	mIPs = new RecursiveInnerProduct* [totalNewPts];
	mNumberOfIPs = totalNewPts;
	mCurrentSignalIndex = -1;
	double clim2 = clim * clim;

	for (int i=0; i<totalNewPts; i++) {

		extra = extraPts - i;
		mIPs [i] = new RecursiveInnerProduct (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mIdealNorm2s, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy);
		mIPs [i]->SetSignalIntervalIsAccurate (mSignalIntervalIsAccurate);
		mIPs [i]->BuildFromLeft (0.0, 0.0, nPoints, extra, -1, -1, newSignalIndex, clim2, newSignalIndex);
		newSignalIndex++;
	}

	return 0;
}


void RecursiveInnerProduct :: SetSignalIntervalIsAccurate (bool accurate) {

	mSignalIntervalIsAccurate = accurate;
	
	if (accurate) {

		mMinimumSignalRatio = 0.5;
		mMaximumSignalRatio = 1.5;
	}

	else {

		mMinimumSignalRatio = 0.45;
		mMaximumSignalRatio = 1.5;
	}
}


int RecursiveInnerProduct :: BuildFromRight (double cumNorm2, double cumIP, int ptsRemaining, int extraPts, int lastSignalIndex, int lastIdealIndex, int currentSignalIndex, double clim2, int startSignalIndex) {

	mCurrentSignalIndex = currentSignalIndex;
	mLastSignalIndex = lastSignalIndex;
	mLastIdealIndex = lastIdealIndex;
	mFromLeft = false;

	double diff;
	double norm2;
	double temp;

	if (lastIdealIndex == -1) {

		// This is the first point to be added; ignore lastSignalIndex and cumNorm2, cumIP
		
		mCurrentNorm2 = mCurrentIP = 0.0;
		mCurrentIdealIndex = mSetSize - 1;

		if (ptsRemaining < 2)
			return -1;
	}

	else {

		mCurrentIdealIndex = lastIdealIndex - 1;
		diff = mSignalArray [lastSignalIndex]->GetMean () - mSignalArray [currentSignalIndex]->GetMean ();

		if (ptsRemaining > 1) {

			double lambda = mIdealDiffs [mCurrentIdealIndex] / (mIdealPts [mSetSize-1] - mIdealPts [0]);
			double gamma = diff / lambda;
			double firstMean = mSignalArray [0]->GetMean ();
			double delFull = mSignalArray [mArraySize-1]->GetMean () - firstMean;

			if ((gamma <= mMinimumSignalRatio * delFull) || (gamma >= mMaximumSignalRatio * delFull)) {

				// Signal interval is either too small or too large for region

				mCurrentIP = 0.0;
				mIsLastIP = true;
				return 0;
			}

			double rightCandidate = mSignalArray [startSignalIndex]->GetMean ();
			
			if (gamma >= mMaximumSignalRatio * (rightCandidate - firstMean)) {

				// Signal interval is too far left

				mCurrentIP = 0.0;
				mIsLastIP = true;
				return 0;
			}
		}

		norm2 = diff * diff;
		mCurrentNorm2 = norm2 + cumNorm2;
		temp = mCurrentIP = diff * mIdealDiffs [mCurrentIdealIndex] + cumIP;

		if ((ptsRemaining > 1) && (temp * temp <= clim2 * mCurrentNorm2 * mIdealNorm2s [mCurrentIdealIndex])) {

			mCurrentIP = 0.0;
			mIsLastIP = true;
		}
	}

//	temp = (5.0 * mSignalArray [currentSignalIndex]->Peak ()) / mMaximumHeight;
//	int level = (int) ceil (temp);

//	if (level > 5)
//		level = 5;

	int totalNewPts = extraPts + 1;
	int extra;
	int nPoints = ptsRemaining - 1;
	int newSignalIndex = mCurrentSignalIndex;
	double peak = mSignalArray [currentSignalIndex]->Peak ();

	if (RecursiveInnerProduct::HeightOutsideLimit (mHeightIndices, mCurrentIdealIndex, peak, mMaximumHeight, mAllowedHeightDiscrepancy)) {

		mCurrentIP = 0.0;
		mIsLastIP = true;
	}

	else if (ptsRemaining <= 1) {

		mIsLastIP = true;
		mCurrentIP = mCurrentIP / sqrt (mCurrentNorm2 * mIdealNorm2s [mCurrentIdealIndex]);
	}

	else {

		mIPs = new RecursiveInnerProduct* [totalNewPts];
		mNumberOfIPs = totalNewPts;

		for (int i=0; i<totalNewPts; i++) {

			extra = extraPts - i;
			newSignalIndex--;
			mIPs [i] = new RecursiveInnerProduct (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mIdealNorm2s, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy);
			mIPs [i]->SetSignalIntervalIsAccurate (mSignalIntervalIsAccurate);
			mIPs [i]->BuildFromRight (mCurrentNorm2, mCurrentIP, nPoints, extra, mCurrentSignalIndex, mCurrentIdealIndex, newSignalIndex, clim2, startSignalIndex);
		}
	}

	return 0;
}



int RecursiveInnerProduct :: BuildFromLeft (double cumNorm2, double cumIP, int ptsRemaining, int extraPts, int lastSignalIndex, int lastIdealIndex, int currentSignalIndex, double clim2, int startSignalIndex) {

	mCurrentSignalIndex = currentSignalIndex;
	mLastSignalIndex = lastSignalIndex;
	mLastIdealIndex = lastIdealIndex;
	mFromLeft = true;

	double diff;
	double norm2;
	double temp;

	if (lastIdealIndex == -1) {

		// This is the first point to be added; ignore lastSignalIndex and cumNorm2, cumIP
		
		mCurrentNorm2 = mCurrentIP = 0.0;
		mCurrentIdealIndex = 0;

		if (ptsRemaining < 2)
			return -1;
	}

	else {

		mCurrentIdealIndex = lastIdealIndex + 1;
		diff = mSignalArray [currentSignalIndex]->GetMean () - mSignalArray [lastSignalIndex]->GetMean ();

		if (ptsRemaining > 1) {

			double lambda = mIdealDiffs [mLastIdealIndex] / (mIdealPts [mSetSize-1] - mIdealPts [0]);
			double gamma = diff / lambda;
			double lastMean = mSignalArray [mArraySize-1]->GetMean ();
			double delFull = lastMean - mSignalArray [0]->GetMean ();

			if ((gamma <= mMinimumSignalRatio * delFull) || (gamma >= mMaximumSignalRatio * delFull)) {

				// Signal interval is either too small or too large for region
				
				mCurrentIP = 0.0;
				mIsLastIP = true;
				return 0;
			}

			double leftCandidate = mSignalArray [startSignalIndex]->GetMean ();
			
			if (gamma >= mMaximumSignalRatio * (lastMean - leftCandidate)) {

				// Signal interval is too far right

				mCurrentIP = 0.0;
				mIsLastIP = true;
				return 0;
			}
		}

		norm2 = diff * diff;
		mCurrentNorm2 = norm2 + cumNorm2;
		temp = mCurrentIP = diff * mIdealDiffs [mLastIdealIndex] + cumIP;

		if ((ptsRemaining > 1) && (temp * temp <= clim2 * mCurrentNorm2 * mIdealNorm2s [mLastIdealIndex])) {

			mCurrentIP = 0.0;
			mIsLastIP = true;
		}
	}

//	temp = (5.0 * mSignalArray [currentSignalIndex]->Peak ()) / mMaximumHeight;
//	int level = (int) ceil (temp);

//	if (level > 5)
//		level = 5;

	int totalNewPts = extraPts + 1;
	int extra;
	int nPoints = ptsRemaining - 1;
	int newSignalIndex = mCurrentSignalIndex;
	double peak = mSignalArray [currentSignalIndex]->Peak ();

	if (RecursiveInnerProduct::HeightOutsideLimit (mHeightIndices, mCurrentIdealIndex, peak, mMaximumHeight, mAllowedHeightDiscrepancy)) {

		mCurrentIP = 0.0;
		mIsLastIP = true;
	}

	else if (ptsRemaining <= 1) {

		mIsLastIP = true;
		mCurrentIP = mCurrentIP / sqrt (mCurrentNorm2 * mIdealNorm2s [mLastIdealIndex]);
	}

	else {

		mIPs = new RecursiveInnerProduct* [totalNewPts];
		mNumberOfIPs = totalNewPts;

		for (int i=0; i<totalNewPts; i++) {

			extra = extraPts - i;
			newSignalIndex++;
			mIPs [i] = new RecursiveInnerProduct (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mIdealNorm2s, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy);
			mIPs [i]->SetSignalIntervalIsAccurate (mSignalIntervalIsAccurate);
			mIPs [i]->BuildFromLeft (mCurrentNorm2, mCurrentIP, nPoints, extra, mCurrentSignalIndex, mCurrentIdealIndex, newSignalIndex, clim2, startSignalIndex);
		}
	}

	return 0;
}



double RecursiveInnerProduct :: GetBestSelection (int* indexVector) const {
	
	// returns correlation

	if (mIsLastIP && (mCurrentIP > 0.0)) {

		indexVector [mCurrentSignalIndex] = 1;
		return mCurrentIP;
	}

	if (mMaxIndex < 0)
		return 0.0;

	double correlation = mIPs [mMaxIndex]->GetBestSelection (indexVector);

	if (mCurrentSignalIndex >= 0)
		indexVector [mCurrentSignalIndex] = 1;

	return correlation;
}



double RecursiveInnerProduct :: GetCumulativeCorrelation () {

	if (mIsLastIP)
		return mCurrentIP;

	double maxCorrelation = 0.0;
	double correlation;
	mMaxIndex = -1;

	for (int i=0; i<mNumberOfIPs; i++) {

		correlation = mIPs [i]->GetCumulativeCorrelation ();

		if (correlation > maxCorrelation) {

			maxCorrelation = correlation;
			mMaxIndex = i;
		}
	}

	if (mMaxIndex < 0)
		return 0.0;

	return maxCorrelation;
}


bool RecursiveInnerProduct :: HeightOutsideLimit (const int* htIndices, int index, double peak, double maxPeak) {

	if (htIndices == NULL)
		return false;

	double temp = (5.0 * peak) / maxPeak;
	int level = (int) ceil (temp);

	if (level > 5)
		level = 5;

	int htIndex = htIndices [index];

	if ((htIndex >= 0) && (abs (level - htIndex) >= 2))
		return true;

	return false;
}


bool RecursiveInnerProduct :: HeightOutsideLimit (const int* htIndices, int index, double peak, double maxPeak, int discrepancy) {

	if (htIndices == NULL)
		return false;

	double temp = (5.0 * peak) / maxPeak;
	int level = (int) ceil (temp);
	int disc;

	if (discrepancy >= 2)
		disc = discrepancy;

	else
		disc = 2;

	if (level > 5)
		level = 5;

	int htIndex = htIndices [index];

	if ((htIndex >= 0) && (abs (level - htIndex) >= disc))
		return true;

	return false;
}


