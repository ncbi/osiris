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
*  FileName: ControlFit.cpp
*  Author:   Robert Goor
*
*/
//
//  class ControlFit manages the search for the best subset of an array to maximize the dot product with a
//  specified target array.
//

#include "ControlFit.h"
#include "math.h"

ControlFit :: ControlFit (const double* target, int size) : 
mSize (size),
mCurrentFitIndex (size),
mCurrentFitIndex1 (size - 1),
mCurrentFitNormSquared (0.0),
mCurrentDotProduct (0.0) {
	
	// target is array of differences, so really of size-1
	mTarget = new double [size];
	mTargetNorms = new double [size];
	mTargetNormsByThrees = new double [size];
  int i;
	for (i=0; i<size; i++) {

		mTarget [i] = target [i];  // last one is irrelevant
	}

	int size1 = size - 2;
	double norm = target [size1] * target [size1];
	mTargetNorms [size1] = norm;
	double temp;

	for (i=size1-1; i>=0; i--) {

		temp = target [i];
		norm += temp * temp;
		mTargetNorms [i] = norm;
	}

	for (i=size1-2; i>0; i--) {

		temp = target [i];
		norm = temp * temp;
		temp = target [i-1];
		norm += temp * temp;
		mTargetNormsByThrees [i] = norm;
	}

	mTargetNormsByThrees [0] = target [0] * target [0];
}


ControlFit :: ~ControlFit () {

	delete[] mTarget;
	delete[] mTargetNorms;
	delete[] mTargetNormsByThrees;
}

double ControlFit :: Test3PointsOnRight (double* points) {
	
	// array size 3
	return NormalizedDotProductOfLast3Points (points);
}



double ControlFit :: Set3PointsOnRight (double* points) {
	
	// array size 3
	int inputEnd = mSize - 2;
	int inputIndex = inputEnd - 1;
	double* currentPoint = points;

	mLatestPoint = *points;

	double dot = 0.0;
	double norm1 = 0.0;
	double temp;

	for (int i=inputIndex; i<=inputEnd; i++) {

		temp = *(currentPoint+1) - *currentPoint;
		currentPoint++;
		dot += temp * mTarget [i];
		norm1 += temp * temp;
	}

	mCurrentDotProduct = dot;
	mCurrentFitNormSquared = norm1;
	mCurrentFitIndex = mSize - 4;
	mCurrentFitIndex1 = mCurrentFitIndex - 1;
	double norm2 = mTargetNorms [mSize - 3];
	return dot / sqrt (norm1 * norm2);
}


double ControlFit :: TestNewPoint (double value) {

	double norm2 = mTargetNorms [mCurrentFitIndex];
	double dot = mCurrentDotProduct;
	double norm1 = mCurrentFitNormSquared;
	double diff = mLatestPoint - value;
	dot += diff * mTarget [mCurrentFitIndex];
	norm1 += diff * diff;

	return dot / sqrt (norm1 * norm2);
}


double ControlFit :: TestNewPairOfPoints (double value1, double value2) {

	double norm2 = mTargetNormsByThrees [mCurrentFitIndex];
	double diff1 = value2 - value1;
	double diff2 = mLatestPoint - value2;
	double norm1 = diff1 * diff1 + diff2 * diff2;
	double dot = diff2 * mTarget [mCurrentFitIndex] + diff1 * mTarget [mCurrentFitIndex1];

	return dot / sqrt (norm1 * norm2);
}



int ControlFit :: AddNewPoint (double value) {
	
	// returns 0 if OK
	double diff = mLatestPoint - value;

	mCurrentDotProduct += diff * mTarget [mCurrentFitIndex];
	mCurrentFitNormSquared += diff * diff;
	mLatestPoint = value;
	mCurrentFitIndex--;
	mCurrentFitIndex1 = mCurrentFitIndex - 1;
	return 0;
}


void ControlFit :: Reset () {

	mCurrentFitIndex = mSize - 4;
	mCurrentFitIndex1 = mCurrentFitIndex - 1;
	mCurrentFitNormSquared = 0.0;
	mCurrentDotProduct = 0.0;
}



double ControlFit :: NormalizedDotProductOfLast3Points (double* points) {
		
	double dot = 0.0;
	double norm1 = 0.0;
	int size = mSize - 3;
	double norm2 = mTargetNorms [size];
	double* currentPoint = points;
	double* currentTarget = mTarget + size;
	double temp;

	for (int i=0; i<2; i++) {

		temp = *(currentPoint+1) - *currentPoint;
		dot += temp * (*currentTarget);
		norm1 += temp * temp;
		currentPoint++;
		currentTarget++;
	}

	return dot / sqrt (norm1 * norm2);
}


ControlFitTwoSided :: ControlFitTwoSided (const double* values, const double* target, int size) :
mSize (size) {

	// target = array of differences
	mTarget = new double [size];
	mTargetNormsRight = new double [size];
	mTargetNormsLeft = new double [size];
	mValues = new double [size];
  int i;
	for (i=0; i<size; i++) {

		mValues [i] = values [i];
		mTarget [i] = target [i];
	}

	int size1 = size - 2;
	double norm = target [size1] * target [size1];
	mTargetNormsRight [size1] = norm;

	double temp;

	for (i=size1-1; i>=0; i--) {

		temp = target [i];
		norm += temp * temp;
		mTargetNormsRight [i] = norm;
	}

	norm = target [0] * target [0];
	mTargetNormsLeft [0] = norm;

	for (i=1; i<=size1; i++) {

		temp = target [i];
		norm += temp * temp;
		mTargetNormsLeft [i] = norm;
	}
}

ControlFitTwoSided :: ~ControlFitTwoSided () {

	delete[] mValues;
	delete[] mTarget;
	delete[] mTargetNormsRight;
	delete[] mTargetNormsLeft;
}



double ControlFitTwoSided :: Test3PointsOnRight (double* points) {
	
	// array size 3
	return NormalizedDotProductOfLast3Points (points);
}



double ControlFitTwoSided :: Set3PointsOnRight (double* points) {
	
	// array size 3
	int inputEnd = mSize - 2;
	int inputIndex = inputEnd - 1;
	double* currentPoint = points;

	mLatestPointRight = *points;

	double dot = 0.0;
	double norm1 = 0.0;
	double temp;

	for (int i=inputIndex; i<=inputEnd; i++) {

		temp = *(currentPoint+1) - *currentPoint;
		currentPoint++;
		dot += temp * mTarget [i];
		norm1 += temp * temp;
	}

	mCurrentDotProductRight = dot;
	mCurrentFitNormSquaredRight = norm1;
	mCurrentFitIndexRight = mSize - 4;
	mLastAddedIndexRight = mSize - 3;
	double norm2 = mTargetNormsRight [mSize - 3];
	mCumulativeNormRight = norm2;
	return dot / sqrt (norm1 * norm2);
}





double ControlFitTwoSided :: Test3PointsOnLeft (double* points) {
	
	// array size 3
	return NormalizedDotProductOfFirst3Points (points);
}



double ControlFitTwoSided :: Set3PointsOnLeft (double* points) {
	
	// array size 3

	double* currentPoint = points;
	mLatestPointLeft = *(points + 2);

	double dot = 0.0;
	double norm1 = 0.0;
	double temp;

	for (int i=0; i<=1; i++) {

		temp = *(currentPoint+1) - *currentPoint;
		currentPoint++;
		dot += temp * mTarget [i];
		norm1 += temp * temp;
	}

	mCurrentDotProductLeft = dot;
	mCurrentFitNormSquaredLeft = norm1;
	mCurrentFitIndexLeft = 3;
	mLastAddedIndexLeft = 2;
	double norm2 = mTargetNormsLeft [1];
	mCumulativeNormLeft = norm2;
	return dot / sqrt (norm1 * norm2);
}


double ControlFitTwoSided :: SetNPointsOnRight (double* points, int N) {
	
	// array size N

	int MN = mSize - N;

	int inputEnd = mSize - 2;
	int inputIndex = MN;
	double* currentPoint = points;

	mLatestPointRight = *points;

	double dot = 0.0;
	double norm1 = 0.0;
	double temp;

	for (int i=inputIndex; i<=inputEnd; i++) {

		temp = *(currentPoint+1) - *currentPoint;
		currentPoint++;
		dot += temp * mTarget [i];
		norm1 += temp * temp;
	}

	mCurrentDotProductRight = dot;
	mCurrentFitNormSquaredRight = norm1;
	mCurrentFitIndexRight = MN - 1;
	mLastAddedIndexRight = MN;
	double norm2 = mTargetNormsRight [MN];
	mCumulativeNormRight = norm2;
	return dot / sqrt (norm1 * norm2);
}



double ControlFitTwoSided :: SetNpointsOnLeft (double* points, int N) {
	
	// array size N

	int N1 = N - 1;
	int N2 = N - 2;

	double* currentPoint = points;
	mLatestPointLeft = *(points + N1);

	double dot = 0.0;
	double norm1 = 0.0;
	double temp;

	for (int i=0; i<=N2; i++) {

		temp = *(currentPoint+1) - *currentPoint;
		currentPoint++;
		dot += temp * mTarget [i];
		norm1 += temp * temp;
	}

	mCurrentDotProductLeft = dot;
	mCurrentFitNormSquaredLeft = norm1;
	mCurrentFitIndexLeft = N;
	mLastAddedIndexLeft = N1;
	double norm2 = mTargetNormsLeft [N2];
	mCumulativeNormLeft = norm2;
	return dot / sqrt (norm1 * norm2);
}



double ControlFitTwoSided :: TestNewPointOnRight (double value) {

	// Assume point(s) have already been added on left
	double norm2 = mCumulativeNormRight + mCumulativeNormLeft;
	double dot = mCurrentDotProductRight + mCurrentDotProductLeft;
	double norm1 = mCurrentFitNormSquaredRight + mCurrentFitNormSquaredLeft;
	double diff1 = mLatestPointRight - value;
	double diff2 = value - mLatestPointLeft;

	double vdiff1 = mValues [mLastAddedIndexRight] - mValues [mLastAddedIndexRight - 1];
	dot += diff1 * vdiff1;
	norm2 += vdiff1 * vdiff1;

	double vdiff2 = mValues [mLastAddedIndexRight - 1] - mValues [mLastAddedIndexLeft];
	dot += diff2 * vdiff2;
	norm2 += vdiff2 * vdiff2;

	norm1 += diff1 * diff1 + diff2 * diff2;
	return dot / sqrt (norm1 * norm2);
}



double ControlFitTwoSided :: TestNewPointOnLeft (double value) {

	double norm2 = mCumulativeNormRight + mCumulativeNormLeft;
	double dot = mCurrentDotProductRight + mCurrentDotProductLeft;
	double norm1 = mCurrentFitNormSquaredRight + mCurrentFitNormSquaredLeft;
	double diff1 = mLatestPointRight - value;
	double diff2 = value - mLatestPointLeft;
	
	double vdiff1 = mValues [mLastAddedIndexRight] - mValues [mLastAddedIndexLeft + 1];
	double vdiff2;
	dot += diff1 * vdiff1;
	norm2 += vdiff1 * vdiff1;

	vdiff2 = mValues [mLastAddedIndexLeft + 1] - mValues [mLastAddedIndexLeft];
	dot += diff2 * vdiff2;
	norm2 += vdiff2 * vdiff2;

	norm1 += diff1 * diff1 + diff2 * diff2;
	return dot / sqrt (norm1 * norm2);
}


double ControlFitTwoSided :: TestNewPointOnRightOnly (double value) {

	double norm2 = mCumulativeNormRight;
	double dot = mCurrentDotProductRight;
	double norm1 = mCurrentFitNormSquaredRight;
	double diff1 = mLatestPointRight - value;

	double vdiff1 = mValues [mLastAddedIndexRight] - mValues [mLastAddedIndexRight - 1];
	dot += diff1 * vdiff1;
	norm2 += vdiff1 * vdiff1;

	norm1 += diff1 * diff1;
	return dot / sqrt (norm1 * norm2);
}



double ControlFitTwoSided :: TestNewPointOnLeftOnly (double value) {

	double norm2 = mCumulativeNormLeft;
	double dot = mCurrentDotProductLeft;
	double norm1 = mCurrentFitNormSquaredLeft;
	double diff2 = value - mLatestPointLeft;
	
	double vdiff2 = mValues [mLastAddedIndexLeft + 1] - mValues [mLastAddedIndexLeft];
	dot += diff2 * vdiff2;
	norm2 += vdiff2 * vdiff2;

	norm1 += diff2 * diff2;
	return dot / sqrt (norm1 * norm2);
}



int ControlFitTwoSided :: AddNewPointOnRight (double value) {
	
	// returns 0 if OK
	double vdiff = mValues [mLastAddedIndexRight] - mValues [mLastAddedIndexRight - 1];
	double diff = mLatestPointRight - value;

	mCurrentDotProductRight += diff * vdiff;
	mCurrentFitNormSquaredRight += diff * diff;
	mLatestPointRight = value;
	mCurrentFitIndexRight--;
	mLastAddedIndexRight--;
	mCumulativeNormRight += vdiff * vdiff;
	return 0;
}



int ControlFitTwoSided :: AddNewPointOnLeft (double value) {

	// returns 0 if OK
	double vdiff = mValues [mLastAddedIndexLeft + 1] - mValues [mLastAddedIndexLeft];
	double diff = value - mLatestPointLeft;

	mCurrentDotProductLeft += diff * vdiff;
	mCurrentFitNormSquaredLeft += diff * diff;
	mLatestPointLeft = value;
	mCurrentFitIndexLeft++;
	mLastAddedIndexLeft++;
	mCumulativeNormLeft += vdiff * vdiff;
	return 0;
}



void ControlFitTwoSided :: Reset () {

	mCurrentFitIndexRight = mSize - 4;
	mCurrentFitNormSquaredRight = mCurrentFitNormSquaredLeft = 0.0;
	mCurrentDotProductRight = mCurrentDotProductLeft = 0.0;
	mCumulativeNormRight = mCumulativeNormLeft = 0.0;
	mLastAddedIndexLeft = 2;
	mLastAddedIndexRight = mSize - 3;
	mCurrentFitIndexLeft = 0;
}


double ControlFitTwoSided :: NormalizedDotProductOfLast3Points (double* points) {
		
	double dot = 0.0;
	double norm1 = 0.0;
	int size = mSize - 3;
	double norm2 = mTargetNormsRight [size];
	double* currentPoint = points;
	double* currentTarget = mTarget + size;
	double temp;

	for (int i=0; i<2; i++) {

		temp = *(currentPoint+1) - *currentPoint;
		dot += temp * (*currentTarget);
		norm1 += temp * temp;
		currentPoint++;
		currentTarget++;
	}

	return dot / sqrt (norm1 * norm2);
}


double ControlFitTwoSided :: NormalizedDotProductOfFirst3Points (double* points) {

	double dot;
	double norm1;
	double norm2 = mTargetNormsLeft [1];
	double* currentPoint = points;
	double* currentTarget = mTarget;
	double temp;

	temp = *(currentPoint+1) - *currentPoint;
	dot = temp * (*currentTarget);
	norm1 = temp * temp;
	currentPoint++;
	currentTarget++;

	temp = *(currentPoint+1) - *currentPoint;
	dot += temp * (*currentTarget);
	norm1 += temp * temp;

	return dot / sqrt (norm1 * norm2);
}




