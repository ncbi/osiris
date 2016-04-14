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
*  FileName: LeastMedianOfSquares.h
*  Author:   Robert Goor
*
*/
//
//     class LeastMedianOfSquares1D and other classes that calculate least median of squares for data.  
//	LeastMedianOfSquares1D calculates LMS for a 1 parameter model.  Once the LMS is calculated, outliers are
//	indicated based on formula
//


#include "LeastMedianOfSquares.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;


LeastMedianOfSquares1D :: LeastMedianOfSquares1D (int n, double* x, double* y) : mIsOK (true), mSize (n), mRatioArray (NULL), mOutlierArray (NULL), mLeastMedianValue (0.0), mMedianResidual (0.0) {

	int i;
	double temp;
	double ratio;

	if (n <= 2) {

		mIsOK = false;
		return;
	}

	mRatioArray = new double [mSize];
	mOutlierArray = new bool [mSize];

	for (i=0; i<mSize; i++) {

		temp = x [i];

		if (temp == 0.0)
			temp = 0.00001;

		ratio = y [i] / temp;
		mRatioList.push_back (ratio);
		mRatioArray [i] = ratio;
		mOutlierArray [i] = false;
	}
}


LeastMedianOfSquares1D :: LeastMedianOfSquares1D (int n, double* yOverXRatios) : mIsOK (true), mSize (n), mRatioArray (NULL), mOutlierArray (NULL), mLeastMedianValue (0.0), mMedianResidual (0.0) {

	int i;

	if (n <= 2) {

		mIsOK = false;
		return;
	}

	mRatioArray = new double [mSize];
	mOutlierArray = new bool [mSize];
	double ratio;

	for (i=0; i<mSize; i++) {

		ratio = yOverXRatios [i];
		mRatioList.push_back (ratio);
		mRatioArray [i] = ratio;
		mOutlierArray [i] = false;
	}
}


LeastMedianOfSquares1D :: LeastMedianOfSquares1D (const list<double>& yOverXRatios) : mIsOK (true), mRatioArray (NULL), mOutlierArray (NULL), mLeastMedianValue (0.0), mMedianResidual (0.0) {
	
	int i;
	mSize = yOverXRatios.size ();

	if (mSize <= 2) {

		mIsOK = false;
		return;
	}

	mRatioArray = new double [mSize];
	mOutlierArray = new bool [mSize];

	list<double>::const_iterator yxIterator;
	double ratio;
	i = 0;

	for (yxIterator = yOverXRatios.begin (); yxIterator != yOverXRatios.end (); yxIterator++) {

		ratio = *yxIterator;
		mRatioList.push_back (ratio);
		mRatioArray [i] = ratio;
		mOutlierArray [i] = false;
		i++;
	}
}



LeastMedianOfSquares1D :: LeastMedianOfSquares1D (const list<double>& xValues, const list<double>& yValues) : mIsOK (true), mRatioArray (NULL), mOutlierArray (NULL), mLeastMedianValue (0.0), mMedianResidual (0.0) {

	int i;
	mSize = xValues.size ();

	if ((yValues.size () != mSize) || (mSize <= 2)) {

		mIsOK = false;
		return;
	}

	mRatioArray = new double [mSize];
	mOutlierArray = new bool [mSize];

	list<double>::const_iterator xIterator;
	list<double>::const_iterator yIterator;
	double ratio;
	double x;
	double y;
	double temp;
	i = 0;
	yIterator = yValues.begin ();

	for (xIterator = xValues.begin (); xIterator != xValues.end (); xIterator++) {

		x = *xIterator;
		y = *yIterator;
		temp = x;

		if (temp == 0.0)
			temp = 0.00001;

		ratio = y / temp;
		mRatioList.push_back (ratio);
		mRatioArray [i] = ratio;
		mOutlierArray [i] = false;
		i++;
		yIterator++;
	}
}



LeastMedianOfSquares1D :: ~LeastMedianOfSquares1D () {

	delete[] mRatioArray;
	delete[] mOutlierArray;
	mRatioList.clear ();
}


double LeastMedianOfSquares1D :: CalculateLMS () {

	int i;
	list<double> sortList;
	double* sortedArray = new double [mSize];

	for (i=0; i<mSize; i++)
		sortList.push_back (mRatioArray [i]);

	sortList.sort ();

	for (i=0; i<mSize; i++) {

		sortedArray [i] = sortList.front ();
		sortList.pop_front ();
	}

	// sortList should now be empty.

	int midSize = ((mSize - 1) / 2);
	double* low = sortedArray;
	double* high = sortedArray + midSize;
	double* upperBound = sortedArray + mSize;
	double Min = *high - *low;
	double delta;
	i = 0;
	int lowIndex = 0;
	double average = 0.5 * (*high + *low);

	while (true) {

		if (high == upperBound)
			break;

		delta = *high - *low;

		if (delta < Min) {

			Min = delta;
			lowIndex = i;
			average = 0.5 * (*high + *low);
		}

		i++;
		low++;
		high++;
	}

	delete[] sortedArray;
	mLeastMedianValue = average;
	mMedianResidual = *high - average;
	double s;

	// Below is the Rousseeuw and van Zomeren (Unmasking Multivariate Outliers and Leverage Points. Journal of the American Statistical Association 85,1990. 633-639.) threshold for outliers
	
	if (mSize > 2)
		s = 1.4826 * (1.0 + (5.0 / ((double)mSize - 2.0))) * mMedianResidual;

	else
		s = 14.826 * mMedianResidual;

	s = 2.5 * s;

	for (i=0; i<mSize; i++) {

		if (fabs (mRatioArray [i] - mLeastMedianValue) > s)
			mOutlierArray [i] = true;
	}

	return mLeastMedianValue;
}



bool LeastMedianOfSquares1D :: ElementIsOutlier (int i) const {

	if ((mOutlierArray != NULL) && (i < mSize))
		return mOutlierArray [i];

	return false;
}


