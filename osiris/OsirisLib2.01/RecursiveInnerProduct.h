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
*  FileName: RecursiveInnerProduct.h
*  Author:   Robert Goor
*
*/
//
//  class RecursiveInnerProduct is designed to manage trees of selected subsets of a set of characteristics with the overall goal of finding the
//  subset that maximizes the fit.
//

#ifndef _RECURSIVEINNERPRODUCT_H_
#define _RECURSIVEINNERPRODUCT_H_

#include "rgdlist.h"
#include "DataSignal.h"


class RecursiveInnerProduct {

public:
	RecursiveInnerProduct (RGDList& signalList, const double* idealPts, const double* idealDiffs, const int* htIndices, const double* idealNorm2s, int setSize, double maxHeight, int allowedDiscrepancy);
//	RecursiveInnerProduct (DataSignal** signalArray, int arraySize, const double* idealPts, const double* idealDiffs, const int* htIndices, const double* idealNorm2s, int setSize, double maxHeight);
	~RecursiveInnerProduct ();

	int BuildFromRight (int ptsRemaining, int extraPts, double clim);
	int BuildFromLeft (int ptsRemaining, int extraPts, double clim);
	void SetSignalIntervalIsAccurate (bool accurate);

//	int BuildFromRight (double cumNorm2, double cumIP, int ptsRemaining, int extraPts, int lastSignalIndex, int lastIdealIndex, int currentSignalIndex);
//	int BuildFromLeft (double cumNorm2, double cumIP, int ptsRemaining, int extraPts, int lastSignalIndex, int lastIdealIndex, int currentSignalIndex);

	double GetBestSelection (int* indexVector) const;  // returns correlation
	double GetCumulativeCorrelation ();

	static bool HeightOutsideLimit (const int* htIndices, int index, double peak, double maxPeak);
	static bool HeightOutsideLimit (const int* htIndices, int index, double peak, double maxPeak, int discrepancy);

protected:
	DataSignal** mSignalArray;
	int mArraySize;
	double mMaximumHeight;

	const double* mIdealPts;
	const double* mIdealDiffs;
	const int* mHeightIndices;
	const double* mIdealNorm2s;
	int mSetSize;
	int mDiffSize;

	int mLastSignalIndex;
	int mLastIdealIndex;
	int mCurrentSignalIndex;
	int mCurrentIdealIndex;
	double mCurrentIP;
	double mCurrentNorm2;

	bool mFromLeft;
	bool mClearSignalArray;
	bool mIsLastIP;

	RecursiveInnerProduct** mIPs;
	int mNumberOfIPs;
	int mMaxIndex;
	bool mSignalIntervalIsAccurate;
	double mMinimumSignalRatio;
	double mMaximumSignalRatio;
	int mAllowedHeightDiscrepancy;

	RecursiveInnerProduct (DataSignal** signalArray, int arraySize, const double* idealPts, const double* idealDiffs, const int* htIndices, const double* idealNorm2s, int setSize, double maxHeight, int allowedDiscrepancy);

	int BuildFromRight (double cumNorm2, double cumIP, int ptsRemaining, int extraPts, int lastSignalIndex, int lastIdealIndex, int currentSignalIndex, double clim2, int startSignalIndex);
	int BuildFromLeft (double cumNorm2, double cumIP, int ptsRemaining, int extraPts, int lastSignalIndex, int lastIdealIndex, int currentSignalIndex, double clim2, int startSignalIndex);
};


#endif  /*   _RECURSIVEINNERPRODUCT_H_   */

