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
	mCount = 0;
}



RecursiveInnerProduct :: RecursiveInnerProduct (DataSignal** signalArray, int arraySize, const double* idealPts, const double* idealDiffs, const int* htIndices, const double* idealNorm2s, int setSize, double maxHeight, int allowedDiscrepancy) :
mSignalArray (signalArray), mArraySize (arraySize), mMaximumHeight (maxHeight), mIdealPts (idealPts), mIdealDiffs (idealDiffs), mHeightIndices (htIndices), mIdealNorm2s (idealNorm2s), mSetSize (setSize), mClearSignalArray (false),
mIsLastIP (false), mSignalIntervalIsAccurate (true), mMinimumSignalRatio (0.7), mMaximumSignalRatio (1.2), mAllowedHeightDiscrepancy (allowedDiscrepancy) {

	mDiffSize = setSize - 1;
	mIPs = NULL;
	mNumberOfIPs = 0;
	mCount = 0;
}


RecursiveInnerProduct :: ~RecursiveInnerProduct () {

	int n = mNumberOfIPs;

	if (mCount != mNumberOfIPs) {

		cout << "Recursive clean-up, count = " << mCount << " and number of IPs = " << mNumberOfIPs << endl;

		if (mCount < mNumberOfIPs)
			n = mCount;
	}

	//for (int i=0; i<mNumberOfIPs; i++)
	for (int i=0; i<n; i++)
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
//	mIPs = new RecursiveInnerProduct* [totalNewPts];
	mIPs = NEW((size_t)totalNewPts);
	mNumberOfIPs = totalNewPts;
	mCurrentSignalIndex = -1;
	double clim2 = clim * clim;
	mCount = 0;

	for (int i=0; i<totalNewPts; i++) {

		extra = extraPts - i;
		newSignalIndex--;
		mIPs [i] = new RecursiveInnerProduct (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mIdealNorm2s, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy);
		mIPs [i]->SetSignalIntervalIsAccurate (mSignalIntervalIsAccurate);
		mIPs [i]->BuildFromRight (0.0, 0.0, nPoints, extra, -1, -1, newSignalIndex, clim2, newSignalIndex);
		mCount++;
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
//	mIPs = new RecursiveInnerProduct* [totalNewPts];
	mIPs = NEW((size_t)totalNewPts);
	mNumberOfIPs = totalNewPts;
	mCurrentSignalIndex = -1;
	double clim2 = clim * clim;
	mCount = 0;

	for (int i=0; i<totalNewPts; i++) {

		extra = extraPts - i;
		mIPs [i] = new RecursiveInnerProduct (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mIdealNorm2s, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy);
		mIPs [i]->SetSignalIntervalIsAccurate (mSignalIntervalIsAccurate);
		mIPs [i]->BuildFromLeft (0.0, 0.0, nPoints, extra, -1, -1, newSignalIndex, clim2, newSignalIndex);
		newSignalIndex++;
		mCount++;
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

	else if (totalNewPts <= 0) {

		mIsLastIP = true;
		mCurrentIP = mCurrentIP / sqrt (mCurrentNorm2 * mIdealNorm2s [mCurrentIdealIndex]);
	}

	else {

//		mIPs = new RecursiveInnerProduct* [totalNewPts];
		mIPs = NEW((size_t)totalNewPts);
		mNumberOfIPs = totalNewPts;
		mCount = 0;

		for (int i=0; i<totalNewPts; i++) {

			extra = extraPts - i;
			newSignalIndex--;
			mIPs [i] = new RecursiveInnerProduct (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mIdealNorm2s, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy);
			mIPs [i]->SetSignalIntervalIsAccurate (mSignalIntervalIsAccurate);
			mIPs [i]->BuildFromRight (mCurrentNorm2, mCurrentIP, nPoints, extra, mCurrentSignalIndex, mCurrentIdealIndex, newSignalIndex, clim2, startSignalIndex);
			mCount++;
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

	else if (totalNewPts <= 0) {

		mIsLastIP = true;
		mCurrentIP = mCurrentIP / sqrt (mCurrentNorm2 * mIdealNorm2s [mCurrentIdealIndex]);
	}

	else {

//		mIPs = new RecursiveInnerProduct* [totalNewPts];
	mIPs = NEW((size_t)totalNewPts);
		mNumberOfIPs = totalNewPts;
		mCount = 0;

		for (int i=0; i<totalNewPts; i++) {

			extra = extraPts - i;
			newSignalIndex++;
			mIPs [i] = new RecursiveInnerProduct (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mIdealNorm2s, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy);
			mIPs [i]->SetSignalIntervalIsAccurate (mSignalIntervalIsAccurate);
			mIPs [i]->BuildFromLeft (mCurrentNorm2, mCurrentIP, nPoints, extra, mCurrentSignalIndex, mCurrentIdealIndex, newSignalIndex, clim2, startSignalIndex);
			mCount++;
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



BuildGridLeft :: BuildGridLeft (DataSignal** signalArray, int nSignals, const double* idealPts, const double* idealDiffs, const int* htIndices, int setSize, double maxHeight, int allowedDiscrepancy, int startSignal, double idealNorm) :
mStartSignal (startSignal), mSignalArray (signalArray), mArraySize (nSignals), mMaximumHeight (maxHeight), mIdealNorm (idealNorm), mIdealPts (idealPts), mIdealDiffs (idealDiffs), mHeightIndices (htIndices), mSetSize (setSize),
mAllowedDiscrepancy (allowedDiscrepancy), mCorrelation (0.0), mUseEarlierPeak (true), mDispEqual (false) {

}


BuildGridLeft :: BuildGridLeft (DataSignal** signalArray, int nSignals, const double* idealPts, const double* idealDiffs, const int* htIndices, int setSize, double maxHeight, int allowedDiscrepancy, int startSignal, double idealNorm, bool useEarlierPeak) :
mStartSignal (startSignal), mSignalArray (signalArray), mArraySize (nSignals), mMaximumHeight (maxHeight), mIdealNorm (idealNorm), mIdealPts (idealPts), mIdealDiffs (idealDiffs), mHeightIndices (htIndices), mSetSize (setSize),
mAllowedDiscrepancy (allowedDiscrepancy), mCorrelation (0.0), mUseEarlierPeak (useEarlierPeak), mDispEqual (false) {

}


BuildGridLeft :: ~BuildGridLeft () {

	mSelectedSignals.Clear ();
	mSelectedSignalList.clear ();
}


double BuildGridLeft :: Build () {

	int remainingSignals = mArraySize - mStartSignal;  // this includes the start signal

	if (remainingSignals < mSetSize)
		return 0.0;

	mSelectedSignals.Append (mSignalArray [mStartSignal]);
	mSelectedSignalList.push_back (mStartSignal);
	int currentSignal = mStartSignal;
	int i = currentSignal + 1;
	int j;
	int currentIdealSignalIndex = 0;	//???????
	double nextRatio;
	double nextRatio2;
	int setSize1 = mSetSize - 1;
	bool foundMin;
	int currentBestEstimate;

	while ((i < mArraySize) && (currentIdealSignalIndex <= setSize1) && (mSelectedSignals.Entries () < mSetSize)) {

		j = TestRelativeHeight (currentIdealSignalIndex + 1, i);  //?????

		if (j < 0)
			break;

		i = j;
		nextRatio = DistanceRatio (currentSignal, currentIdealSignalIndex, i);
		foundMin = false;
		currentBestEstimate = i;
		i++;

		if (i >= mArraySize) {

			currentIdealSignalIndex++;
			mSelectedSignals.Append (mSignalArray [currentBestEstimate]);
			mSelectedSignalList.push_back (currentBestEstimate);
			currentSignal = currentBestEstimate;
			i = currentSignal + 1;
			foundMin = true;
			break;
		}

		while (i < mArraySize) {

			j = TestRelativeHeight (currentIdealSignalIndex + 1, i);

			if (j < 0) {

				// use current target: currentBestEstimate

				currentIdealSignalIndex++;
				mSelectedSignals.Append (mSignalArray [currentBestEstimate]);
				mSelectedSignalList.push_back (currentBestEstimate);
				currentSignal = currentBestEstimate;
				i = currentSignal + 1;
				foundMin = true;
				break;
			}

			i = j;
			nextRatio2 = DistanceRatio (currentSignal, currentIdealSignalIndex, i);

			if (nextRatio2 > nextRatio) {

				//  use previous signal:  currentBestEstimate
				currentIdealSignalIndex++;
				mSelectedSignals.Append (mSignalArray [currentBestEstimate]);
				mSelectedSignalList.push_back (currentBestEstimate);
				currentSignal = currentBestEstimate;
				i = currentSignal + 1;
				foundMin = true;
				break;
			}

			else if (nextRatio2 < nextRatio) {

				nextRatio = nextRatio2;
				currentBestEstimate = i;
				i++;

				if (i >= mArraySize) {

					currentIdealSignalIndex++;
					mSelectedSignals.Append (mSignalArray [currentBestEstimate]);
					mSelectedSignalList.push_back (currentBestEstimate);
					currentSignal = currentBestEstimate;
					i = currentSignal + 1;
					foundMin = true;
				}
			}

			else {

				if (mUseEarlierPeak) {

					currentIdealSignalIndex++;
					mSelectedSignals.Append (mSignalArray [currentBestEstimate]);
					mSelectedSignalList.push_back (currentBestEstimate);
					currentSignal = currentBestEstimate;
					i = currentSignal + 1;
					foundMin = true;
				}

				else {

					currentIdealSignalIndex++;
					mSelectedSignals.Append (mSignalArray [i]);
					mSelectedSignalList.push_back (i);
					currentSignal = i;
					// increment i in this case
					i = currentSignal + 1;
					foundMin = true;
				}

				mDispEqual = true;
				break;
			}
		}

		if (!foundMin)
			break;
	}

	if (mSelectedSignals.Entries () < mSetSize)
		return 0.0;

	double sumIP = 0.0;
	double sumNorm = 0.0;
	RGDListIterator it (mSelectedSignals);
	DataSignal* nextSignal = (DataSignal*) it ();
	DataSignal* nextSignal2;
	double diff;
	i = 0;

	while (nextSignal2 = (DataSignal*) it ()) {

		diff = nextSignal2->GetMean () - nextSignal->GetMean ();
		sumIP += diff * mIdealDiffs [i];
		sumNorm += diff * diff;
		nextSignal = nextSignal2;
		i++;
	}

	sumNorm = sqrt (sumNorm);

	if (sumNorm == 0.0)
		return 0.0;

	if (mIdealNorm == 0.0)
		return 0.0;
	mCorrelation = sumIP / (sumNorm * mIdealNorm);
	return mCorrelation;
}



int BuildGridLeft :: SelectedSignals (DataSignal** selectedSignals) {

	if (mSelectedSignals.Entries () != mSetSize)
		return -1;

	RGDListIterator it (mSelectedSignals);
	DataSignal* nextSignal;
	int i = 0;

	while (nextSignal = (DataSignal*) it ()) {

		selectedSignals [i] = nextSignal;
		i++;
	}

	return 0;
}


double BuildGridLeft :: GetBestSelection (int* indexVector) const {

	if (mSelectedSignals.Entries () != mSetSize)
		return 0.0;

	list<int>::const_iterator cIt;
	int nextIndex;

	for (cIt = mSelectedSignalList.begin (); cIt != mSelectedSignalList.end (); cIt++) {

		nextIndex = *cIt;
		indexVector [nextIndex] = 1;
	}

	return mCorrelation;
}


double BuildGridLeft :: DistanceRatio (int currentSignal, int currentIdealSignal, int testSignal) {

	//if (currentIdealSignal == 0)
	//	return 0.0;

	if (currentIdealSignal >= mSetSize - 1)
		return 0.0;

	double ideal = mIdealPts [currentIdealSignal + 1] - mIdealPts [currentIdealSignal];
	double actual = mSignalArray [testSignal]->GetApproximateBioID () - mSignalArray [currentSignal]->GetApproximateBioID ();
	double ratio = actual / ideal;
	double answer = fabs (ratio - 1.0);
	
	return answer;
}


int BuildGridLeft :: TestRelativeHeight (int currentIdealSignalIndex, int startTargetIndex) {

	int i = startTargetIndex;

	while (i < mArraySize) {

		if (RecursiveInnerProduct::HeightOutsideLimit (mHeightIndices, currentIdealSignalIndex, mSignalArray [i]->Peak (), mMaximumHeight, mAllowedDiscrepancy))
			i++;

		else
			return i;
	}

	return -1;
}


double BuildGridLeft :: GetFirstPeakHeight () const {

	DataSignal* first = (DataSignal*) mSelectedSignals.First ();

	if (first == NULL)
		return -1.0;

	return first->Peak ();
}


/*
DataSignal** mSignalArray;
	list<BuildGridLeft*> mSets;
	int mArraySize;
	double mMaximumHeight;

	const double* mIdealPts;
	const double* mIdealDiffs;
	const int* mHeightIndices;
	const double* mIdealNorm2s;
	int mSetSize;
	int mDiffSize;

	int mMaxIndex;
	int mAllowedHeightDiscrepancy;
	double mCorrelationThreshold;
*/






GridAnalysis :: GridAnalysis (RGDList& signalList, const double* idealPts, const double* idealDiffs, const int* htIndices, const double* idealNorm2s, int setSize, double maxHeight, int allowedDiscrepancy, double correlationThreshold) : mMaximumHeight (maxHeight),
mIdealPts (idealPts), mIdealDiffs (idealDiffs), mHeightIndices (htIndices), mIdealNorm2s (idealNorm2s), mSetSize (setSize), mDiffSize (setSize - 1), mAllowedHeightDiscrepancy (allowedDiscrepancy), mCorrelationThreshold (correlationThreshold) {

	mArraySize = signalList.Entries ();
	mSignalArray = new DataSignal* [mArraySize];
	int i = 0;
	RGDListIterator it (signalList);
	DataSignal* nextSignal;
	mBestGrid = NULL;

	while (nextSignal = (DataSignal*) it ()) {

		mSignalArray [i] = nextSignal;
		i++;
	}
}


GridAnalysis :: ~GridAnalysis () {

	delete[] mSignalArray;
	list<BuildGridLeft*>::const_iterator cIt;
	BuildGridLeft* nextGrid;

	for (cIt = mSets.begin (); cIt != mSets.end (); cIt++) {

		nextGrid = *cIt;
		delete nextGrid;
	}

	mSets.clear ();
}



int GridAnalysis :: BuildFromLeft () {

	int nStartPoints = mArraySize - mSetSize + 1;
	BuildGridLeft* nextGrid;
	BuildGridLeft* otherNextGrid;
	double firstPeakHeight;
	DataSignal* nextSignal;
	int i;
	double correlation;
	double otherCorrelation;

	double idealNorm = 0.0;
	int setSize1 = mSetSize - 1;
	double temp;

	for (i=0; i<setSize1; i++) {

		temp = mIdealDiffs [i];
		idealNorm += temp * temp;
	}

	idealNorm = sqrt (idealNorm);

	for (i=0; i<nStartPoints; i++) {

		nextSignal = mSignalArray [i];
		firstPeakHeight = nextSignal->Peak ();

		if (RecursiveInnerProduct::HeightOutsideLimit (mHeightIndices, 0, firstPeakHeight, mMaximumHeight, mAllowedHeightDiscrepancy))
			continue;

		nextGrid = new BuildGridLeft (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy, i, idealNorm);
		correlation = nextGrid->Build ();

		if (nextGrid->HasEqualDisp ()) {

			otherNextGrid = new BuildGridLeft (mSignalArray, mArraySize, mIdealPts, mIdealDiffs, mHeightIndices, mSetSize, mMaximumHeight, mAllowedHeightDiscrepancy, i, idealNorm, true);
			otherCorrelation = otherNextGrid->Build ();

			if (otherCorrelation < mCorrelationThreshold)
				delete otherNextGrid;

			else
				mSets.push_back (otherNextGrid);
		}

		if (correlation < mCorrelationThreshold)
			delete nextGrid;

		else
			mSets.push_back (nextGrid);
	}

	if (mSets.size () == 0)
		return -1;

	if (mSets.size () == 1) {

		mBestGrid = mSets.front ();
		mSets.pop_front ();
		return 0;
	}

	bool idealEvenlySpaced = true;
	double diff = mIdealDiffs [0];
	double peak1;
	double peak2;

	for (i=1; i<setSize1; i++) {

		if (mIdealDiffs [i] != diff) {

			idealEvenlySpaced = false;
			break;
		}
	}

	if (idealEvenlySpaced && (mSets.size () == 2)) {

		nextGrid = mSets.front ();
		peak1 = nextGrid->GetFirstPeakHeight ();
		otherNextGrid = mSets.back ();
		peak2 = otherNextGrid->GetFirstPeakHeight ();

		if (peak1 < 0.5 * peak2) {

			mBestGrid = otherNextGrid; // use otherNextGrid
			mSets.pop_front ();
			delete nextGrid;
			return 0;
		}

		else if (peak2 < 0.5 * peak1) {

			// use nextGrid

			mBestGrid = nextGrid;
			mSets.pop_back ();
			delete otherNextGrid;
			return 0;
		}

		else if (mSetSize == 2) {	//  This is AMEL

			if (peak1 <= peak2) {

				mBestGrid = otherNextGrid; // use otherNextGrid
				mSets.pop_front ();
				delete nextGrid;
				return 0;
			}

			else if (peak2 < peak1) {

				// use nextGrid

				mBestGrid = nextGrid;
				mSets.pop_back ();
				delete otherNextGrid;
				return 0;
			}
		}
	}

	double maxCorrelation = 0.0;
	BuildGridLeft* maxNextGrid = NULL;
	list<BuildGridLeft*>::const_iterator cIt;

	while (mSets.size () > 0) {

		nextGrid = mSets.front ();
		mSets.pop_front ();
		correlation = nextGrid->GetCorrelation ();

		if (correlation >= maxCorrelation) {

			delete maxNextGrid;
			maxCorrelation = correlation;
			maxNextGrid = nextGrid;
		}

		else
			delete nextGrid;
	}

	mBestGrid = maxNextGrid;
	return 0;
}



double GridAnalysis :: GetBestSelection (int* indexVector) const {

	if (mBestGrid == NULL)
		return 0.0;

	return mBestGrid->GetBestSelection (indexVector);
}


