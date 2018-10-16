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


DualPoint :: DualPoint (const DualPoint& pt) {

	mIndex1 = pt.mIndex1;
	mIndex2 = pt.mIndex2;
	mX = pt.mX;
	mY = pt.mY;
}


IntersectionPoint* DualLine :: FindIntercept (DualLine& otherLine) {

	double deltaM;
	double deltaI;

	if (mSlope == otherLine.mSlope)
		deltaM = 0.01;
	
	else
		deltaM = mSlope - otherLine.mSlope;

	deltaI = otherLine.mIntercept - mIntercept;
	double x = deltaI / deltaM;
	double y = mSlope * x + mIntercept;
	IntersectionPoint* result = new IntersectionPoint (x, y);
	return result;
}


double DualLine :: YAtXEquals (double x) {

	return mSlope * x + mIntercept;
}


IndexedDouble& IndexedDouble :: operator= (const IndexedDouble& is) {

	if (this == &is)
		return *this;

	mIndex = is.mIndex;
	mSlope = is.mSlope;
	return *this;
}


LeastMedianOfSquares :: LeastMedianOfSquares (int n, double* x, double* y) : mIsOK (true), mSize (n), mOutlierArray (NULL), mLeastMedianValue (0.0), mMedianResidual (0.0), mOutlierThreshold (0.0) {

	int i;
	mXvalues = new double [n];
	mYvalues = new double [n];
	mRatioArray = new double [n];
	mOutlierArray = new bool [n];
	double temp;

	for (i=0; i<n; i++) {

		mXvalues [i] = x [i];
		mYvalues [i] = y [i];
		mOutlierArray [i] = false;
		temp = x [i];

		if (temp == 0.0)
			temp = 0.0001;

		mRatioArray [i] = y [i] / x [i];
	}
}


LeastMedianOfSquares :: LeastMedianOfSquares (const list<double>& xValues, const list<double>& yValues) : mIsOK (true), mXvalues (NULL), mYvalues (NULL), mRatioArray (NULL), mOutlierArray (NULL), mLeastMedianValue (0.0), 
	mMedianResidual (0.0), mOutlierThreshold (0.0) {

	mSize = xValues.size ();

	if (yValues.size () != mSize) {

		mIsOK = false;
		return;
	}

	int i;
	mXvalues = new double [mSize];
	mYvalues = new double [mSize];
	mRatioArray = new double [mSize];
	mOutlierArray = new bool [mSize];
	double temp;

	list<double>::const_iterator xIterator;
	list<double>::const_iterator yIterator;

	yIterator = yValues.begin ();
	i = 0;

	for (xIterator = xValues.begin (); xIterator != xValues.end (); xIterator++) {

		temp = mXvalues [i] = *xIterator;
		mYvalues [i] = *yIterator;
		mOutlierArray [i] = false;

		if (temp == 0.0)
			temp = 0.0001;

		mRatioArray [i] = mYvalues [i] / temp;
		i++;
		yIterator++;
	}
}



LeastMedianOfSquares :: ~LeastMedianOfSquares () {

	delete[] mXvalues;
	delete[] mYvalues;
	delete[] mRatioArray;
	delete[] mOutlierArray;
}


bool LeastMedianOfSquares :: ElementIsOutlier (int i) const {

	if ((mOutlierArray != NULL) && (i < mSize))
		return mOutlierArray [i];

	return false;
}


LeastMedianOfSquares1D :: LeastMedianOfSquares1D (int n, double* x, double* y) : LeastMedianOfSquares (n, x, y) {

	if (n <= 3)
		mIsOK = false;
}



LeastMedianOfSquares1D :: LeastMedianOfSquares1D (const list<double>& xValues, const list<double>& yValues) : LeastMedianOfSquares (xValues, yValues) {

	if (mSize <= 3)
		mIsOK = false;
}



LeastMedianOfSquares1D :: ~LeastMedianOfSquares1D () {

}


double LeastMedianOfSquares1D :: CalculateLMS () {

	int i;
	list<double> sortList;
	double* sortedArray = new double [mSize];

	for (i=0; i<mSize; i++)
		sortList.push_back (mRatioArray [i]);

	sortList.sort ();
	//cout << "Sorted list:  ";

	for (i=0; i<mSize; i++) {

		sortedArray [i] = sortList.front ();
		sortList.pop_front ();
	//	cout << sortedArray [i] << ", ";
	}

	//cout << "..." << endl;

	// sortList should now be empty.

	int midSize = ((mSize - 1) / 2);

	//if ((mSize%2 == 0) && (mSize > 2))   // commented out on 12/15/2016 to allow for each lot size (when mSize is even) to be exactly half of mSize.
	//	midSize++;   // For even values of mSize, this calls for a greater number of tested intervals, each of which is one item shorter

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
	mMedianResidual = 0.5 * Min;
	double s;

	// Below is the Rousseeuw and van Zomeren (Unmasking Multivariate Outliers and Leverage Points. Journal of the American Statistical Association 85,1990. 633-639.) threshold for outliers
	
	if (mSize > 2)
		s = 1.4826 * (1.0 + (5.0 / ((double)mSize - 2.0))) * mMedianResidual;

	else
		s = 14.826 * mMedianResidual;

	mOutlierThreshold = 2.5 * s;

	for (i=0; i<mSize; i++) {

		if (fabs (mRatioArray [i] - mLeastMedianValue) > mOutlierThreshold)
			mOutlierArray [i] = true;
	}

	return mLeastMedianValue;
}


LeastMedianOfSquares2DExhaustive :: LeastMedianOfSquares2DExhaustive (int n, double* x, double* y) : LeastMedianOfSquares (n, x, y), mLines (NULL), mSortedLines (NULL) {

	if (n <= 4) {

		mIsOK = false;
		return;
	}

	CreateDualLineArray ();
	mMedianNumber = (mSize / 2) + 1;
	mMedianIncrement = (mSize - 1) / 2;

	if (mSize%2 > 0)
		mMedianIncrement++;

	mMedianNumberLess2 = mMedianNumber - 2;
}


LeastMedianOfSquares2DExhaustive :: LeastMedianOfSquares2DExhaustive (const list<double>& xValues, const list<double>& yValues) : LeastMedianOfSquares (xValues, yValues) {

	if (mSize <= 4) {

		mIsOK = false;
		return;
	}

	CreateDualLineArray ();
	mMedianNumber = (mSize / 2) + 1;
	mMedianIncrement = (mSize - 1) / 2;

	if (mSize%2 > 0)
		mMedianIncrement++;

	mMedianNumberLess2 = mMedianNumber - 2;
}



LeastMedianOfSquares2DExhaustive :: ~LeastMedianOfSquares2DExhaustive () {

	int i;

	for (i=0; i<mSize; i++) {

		delete mLines [i];
		delete mSortedLines [i];
	}

	delete[] mLines;
	delete[] mSortedLines;
}



double LeastMedianOfSquares2DExhaustive :: CalculateLMS () {

	IntersectionPoint*** vertexMatrix = CalculateIntersectionMatrix ();
	int i;
	int j;
	double currentMedian;
	double leastMedian;
	IndexedDouble currentY;
	IndexedDouble bestY;
	bool firstTest = true;
	IntersectionPoint* currentVertex;
	IntersectionPoint* bestVertex;
	DualPoint* currentDualPoint;
	int iBest;
	int jBest;

	for (i=0; i<mSize; i++) {

		for (j=i+1; j<mSize; j++) {

			currentVertex = vertexMatrix [i][j];
			currentDualPoint = new DualPoint (*currentVertex, i, j);
			currentY = CalculateMedianLineSegment (currentDualPoint);
			currentMedian = fabs (currentY.mSlope - currentVertex->mY);

			if (!TestForValidity (currentDualPoint, currentY))
				continue;

			if (firstTest) {

				bestY = currentY;
				bestVertex = currentVertex;
				leastMedian = currentMedian;
				iBest = i;
				jBest = j;
				firstTest = false;
			}

			else {

				if (currentMedian < leastMedian) {

					bestY = currentY;
					bestVertex = currentVertex;
					leastMedian = currentMedian;
					iBest = i;
					jBest = j;
				}
			}
		}
	}

	if (firstTest) {

		DeleteVertexMatrix (vertexMatrix);
		return -1.0;
	}

	// use bestVertex and bestY to calculate median line and populate member data

	double slope;
	double intercept1;
	double intercept2;

	int n = mSortedLines [iBest]->GetIndex ();
	int m = mSortedLines [jBest]->GetIndex (); // these relationships allow us to reconnect to data values x and y.
	int k = mSortedLines [bestY.mIndex]->GetIndex ();  // this is the third data point involved

	slope = (mRatioArray [m] - mRatioArray [n]) / (mXvalues [m] - mXvalues [n]);
	intercept1 = mRatioArray [n] - slope * mXvalues [n];
	intercept2 = mRatioArray [k] - slope * mXvalues [k];
	mMedianResidual = 0.5 * leastMedian;
	mLeastMedianValue = 0.5 * (intercept1 + intercept2);
	mLeastMedianValue2 = slope;
	double s = 1.4826 * (1.0 + (5.0 / ((double)mSize - 2.0))) * mMedianResidual;
	mOutlierThreshold = 2.5 * s;

	for (i=0; i<mSize; i++) {

		if (fabs (mRatioArray [i] - (mLeastMedianValue + mLeastMedianValue2 * mXvalues [i])) > mOutlierThreshold)
			mOutlierArray [i] = true;
	}

	DeleteVertexMatrix (vertexMatrix);
	return mLeastMedianValue;
}


IntersectionPoint*** LeastMedianOfSquares2DExhaustive :: CalculateIntersectionMatrix () {

	IntersectionPoint*** result = new IntersectionPoint** [mSize];
	int i;
	int j;

	for (i=0; i<mSize; i++)
		result [i] = new IntersectionPoint* [mSize];

	for (i=0; i<mSize; i++) {

		for (j=i+1; j<mSize; j++)
			result [i][j] = mSortedLines [i]->FindIntercept (*mSortedLines [j]);
	}

	return result;
}


IndexedDouble LeastMedianOfSquares2DExhaustive :: CalculateMedianLineSegment (DualPoint* vertex) {
	
	// returns y coordinate of intersection with dual line.
	// Ultimately, the chosen vertex gives the slope of the median line because it is the intersection of 2
	// dual lines, implicating two measurement points. The y coordinate of the third dual line gives the y intercept
	// of the median line, which has the slope above and bisects the dual median line segment.

	int n1 = vertex->mIndex1;
	int n2 = vertex->mIndex2;
	int i;
	double vy = vertex->mY;
	double vx = vertex->mX;
	int size2 = mSize - 2;
	IndexedDouble Y;
	IndexedDouble* yArray = new IndexedDouble [size2];
	list<IndexedDouble> intersections;

	for (i=0; i<mSize; i++) {

		if (i == n1)
			continue;

		if (i == n2)
			continue;

		Y.mSlope = mSortedLines [i]->YAtXEquals (vx);
		Y.mIndex = i;
		intersections.push_back (Y);
	}

	intersections.sort ();
	i = 0;

	while (!intersections.empty ()) {

		Y = intersections.front ();
		intersections.pop_front ();
		yArray [i] = Y;
		i++;
	}

	int lastLess = 0;
	int firstGreater = -1;
	int firstEqual = -1;
	int lastEqual = -1;
	IndexedDouble VY;
	VY.mSlope = vy;

	for (i=0; i<size2; i++) {

		if (yArray [i] < VY)
			lastLess = i;

		else if (yArray [i] == VY) {

			if (firstEqual < 0)
				firstEqual = i;

			lastEqual = i;
		}

		else {

			if (firstGreater < 0) {

				firstGreater = i;
				break;
			}

			firstGreater = i;
			break;
		}
	}

	int nEqual;

	if ((firstEqual < 0) && (lastEqual < 0))
		nEqual = 0;

	else
		nEqual = lastEqual - firstEqual + 1;

	int nLessOrEqual = lastLess + nEqual;
	int nGreaterOrEqual = size2 - firstGreater + nEqual;
	double worst = yArray [size2 - 1].mSlope - yArray [0].mSlope;
	double bestLow = worst;
	double bestHigh = worst;
	IndexedDouble yReturnLow = yArray [0];
	IndexedDouble yReturnHigh = yArray [size2 - 1];

	if (nLessOrEqual >= mMedianNumberLess2) {

		if (nEqual > 0)
			yReturnLow = yArray [lastEqual - mMedianNumberLess2 + 1];
			
		else
			yReturnLow = yArray [lastLess - mMedianNumberLess2 + 1];

		bestLow = vy - yReturnLow.mSlope;
	}

	if (nGreaterOrEqual >= mMedianNumberLess2) {

		if (nEqual > 0)
			yReturnHigh = yArray [firstEqual + mMedianNumberLess2 - 1];

		else 
			yReturnHigh = yArray [firstGreater + mMedianNumberLess2 - 1];

		bestHigh = yReturnHigh.mSlope - vy;
	}

	IndexedDouble yReturn = yReturnLow;

	if (bestHigh < bestLow)
		yReturn = yReturnHigh;

	return yReturn;
}


void LeastMedianOfSquares2DExhaustive :: CreateDualLineArray () {

	mLines = new DualLine* [mSize];
	mSortedLines = new DualLine* [mSize];
	int i;
	list<IndexedDouble> sortedList;
	IndexedDouble is;
	DualLine* next;

	for (i=0; i<mSize; i++) {

		mLines [i] = new DualLine (i, mXvalues [i], -mRatioArray [i]);
		is = *mLines [i];
		sortedList.push_back (is);
	}

	sortedList.sort ();
	i = 0;

	while (!sortedList.empty ()) {

		is = sortedList.front ();
		sortedList.pop_front ();
		next = new DualLine (*mLines [is.mIndex]);
		mSortedLines [i] = next;
		i++;
	}
}


bool LeastMedianOfSquares2DExhaustive :: TestForValidity (DualPoint* vertex, IndexedDouble& id) {

	double slope;
	double intercept1;
	double intercept2;
	int iBest = vertex->mIndex1;
	int jBest = vertex->mIndex2;

	int n = mSortedLines [iBest]->GetIndex ();
	int m = mSortedLines [jBest]->GetIndex (); // these relationships allow us to reconnect to data values x and y.
	int k = mSortedLines [id.mIndex]->GetIndex ();  // this is the third data point involved

	slope = (mRatioArray [m] - mRatioArray [n]) / (mXvalues [m] - mXvalues [n]);
	intercept1 = mRatioArray [n] - slope * mXvalues [n];
	intercept2 = mRatioArray [k] - slope * mXvalues [k];
	double intercept = 0.5 * (intercept1 + intercept2);

	if (slope * intercept >= 0.0)
		return true;

	return false;
}


void LeastMedianOfSquares2DExhaustive :: DeleteVertexMatrix (IntersectionPoint*** vertexMatrix) {

	int i;
	int j;

	for (i=0; i<mSize; i++) {
		for (j=i+1; j<mSize; j++)
			delete vertexMatrix [i][j];
	}

	for (i=0; i<mSize; i++)
		delete[] vertexMatrix [i];

	delete[] vertexMatrix;
}


LeastSquaresQuadraticModel :: LeastSquaresQuadraticModel (int n, double* x, double* y) : mIsOK (true), mSize (n), mXvalues (NULL), mYvalues (NULL), mX2values (NULL) {

	if (n < 4) {

		mIsOK = false;
		return;
	}

	int i;
	mXvalues = new double [n];
	mYvalues = new double [n];
	mX2values = new double [n];

	for (i=0; i<n; i++) {

		mXvalues [i] = x [i];
		mYvalues [i] = y [i];
	}

	TestXValuesForIndependence ();
	CreateArrayOfXSquares ();
}


LeastSquaresQuadraticModel :: LeastSquaresQuadraticModel (const list<double>& xValues, const list<double>& yValues) : mIsOK (true), mXvalues (NULL), mYvalues (NULL), mX2values (NULL) {

	mSize = xValues.size ();

	if ((mSize != yValues.size ()) || (mSize < 4)) {

		mIsOK = false;
		return;
	}

	int i;
	mXvalues = new double [mSize];
	mYvalues = new double [mSize];
	mX2values = new double [mSize];

	list<double>::const_iterator xIterator;
	list<double>::const_iterator yIterator;

	yIterator = yValues.begin ();
	i = 0;

	for (xIterator = xValues.begin (); xIterator != xValues.end (); xIterator++) {

		mXvalues [i] = *xIterator;
		mYvalues [i] = *yIterator;
		i++;
		yIterator++;
	}

	TestXValuesForIndependence ();
	CreateArrayOfXSquares ();
}



LeastSquaresQuadraticModel :: ~LeastSquaresQuadraticModel () {

	delete[] mXvalues;
	delete[] mYvalues;
	delete[] mX2values;
}


bool LeastSquaresQuadraticModel :: TestXValuesForIndependence () {

	int i;
	double max = mXvalues [0];
	double min = mXvalues [0];
	double temp;
	int minIndex = 0;
	int maxIndex = 0;

	for (i=1; i<mSize; i++) {

		temp = mXvalues [i];

		if (temp > max) {

			max = temp;
			maxIndex = i;
		}

		if (temp < min) {

			min = temp;
			minIndex = i;
		}
	}

	double diff = max - min;  // /and we know that each x must be positive for our application

	if (diff == 0.0) {

		mXvalues [0] -= 2.0;
		mXvalues [mSize - 1] += 2.0;
		return true;
	}

	double delta = 0.005 * fabs (max);

	if (delta == 0.0) {

		delta = 2.0;
	}

	if (diff <= 0.005 * fabs (max)) {

		mXvalues [maxIndex] += delta;
		mXvalues [minIndex] -= delta;
	}

	return true;
}


bool LeastSquaresQuadraticModel :: CreateArrayOfXSquares () {

	int i;
	double temp;

	for (i=0; i<mSize; i++) {

		temp = mXvalues [i];
		mX2values [i] = temp * temp;
	}

	return true;
}



double LeastSquaresQuadraticModel :: CalculateLeastSquare (double& linearTerm, double& quadTerm, bool constrainLSQ) {

	// First calculate unconstrained least square solution.  If linear and quadratic coefficients have opposite signs, 
	// recompute with quadratic term = 0 (unconstrained, single coefficient) and recompute with linear term = 0.  Use whichever
	// of those two has smallest sum of residuals squared.  Otherwise, use unconstrained 2-parameter LS solution above.

	// First compute nx2 matrix X of x and x**2, and nx1 matrix Y of y, and then compute M - X(trans)*X, which is 2x2.  Invert and multiply by X(trans)* Y.
	// This will give the two unconstrained fit parameters.  If both have same sign, we're done.

	double M [2][2];
	double XTy [2];
	double sum2 = 0.0;
	double sum3 = 0.0;
	double sum4 = 0.0;
	double sumxy = 0.0;
	double sumx2y = 0.0;
	double temp;
	double temp2;
	double tempy;
	int i;

	for (i=0; i<mSize; i++) {

		temp = mXvalues [i];
		tempy = mYvalues [i];
		temp2 = mX2values [i];
		sum2 += temp2;
		sum3 += temp * temp2;
		sum4 += temp2 * temp2;
		sumxy += temp * tempy;
		sumx2y += temp2 * tempy;
	}

	M [0][0] = sum2;
	M [0][1] = M [1][0] = sum3;
	M [1][1] = sum4;

	XTy [0] = sumxy;
	XTy [1] = sumx2y;

	double determinant = sum2 * sum4 - sum3 * sum3;
	double MInv [2][2];
	MInv [0][0] = sum4 / determinant;
	MInv [1][1] = sum2 / determinant;
	MInv [0][1] = MInv [1][0] = - sum3 / determinant;
	linearTerm = mLinearTerm = MInv [0][0] * XTy [0] + MInv [0][1] * XTy [1];
	quadTerm = mQuadraticTerm = MInv [1][0] * XTy [0] + MInv [1][1] * XTy [1];

	if (constrainLSQ && (linearTerm * quadTerm < 0.0)) {

		// Try again with linear term = 0 and then with quadratic term = 0;
		return CalculateLeastSquareWithOneTermZero (linearTerm, quadTerm);
	}

	sum2 = 0.0;

	for (i=0; i<mSize; i++) {

		temp = mYvalues [i] - linearTerm * mXvalues [i] - quadTerm * mX2values [i];
		sum2 += temp * temp;
	}

	mLeastSquare = sum2;
	return sum2;
}


double LeastSquaresQuadraticModel :: CalculateLeastSquareWithOneTermZero (double& linearTerm, double& quadTerm) {

	// First, try linear only

	double alpha = 0.0;
	double num = 0.0;
	double denom = 0.0;
	double temp;
	int i;
	double answer;

	for (i=0; i<mSize; i++) {

		temp = mXvalues [i];
		num += temp * mYvalues [i];
		denom += temp * temp;
	}

	alpha = num / denom;
	double sum = 0.0;

	for (i=0; i<mSize; i++) {

		temp = mYvalues [i] - alpha * mXvalues [i];
		sum += temp * temp;
	}

	double sum2 = 0.0;
	double beta;

	// Now try quadratic only

	for (i=0; i<mSize; i++) {

		temp = mX2values [i];
		num += temp * mYvalues [i];
		denom += temp * temp;
	}

	beta = num / denom;

	for (i=0; i<mSize; i++) {

		temp = mYvalues [i] - beta * mX2values [i];
		sum2 += temp * temp;
	}

	// Which is better

	if (sum2 < sum) {

		linearTerm = mLinearTerm = 0.0;
		quadTerm = mQuadraticTerm = beta;
		answer = mLeastSquare = sum2;
	}

	else {

		quadTerm = mQuadraticTerm = 0.0;
		linearTerm = mLinearTerm = alpha;
		answer = mLeastSquare = sum;
	}

	return answer;
}



