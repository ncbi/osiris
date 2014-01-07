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
*  FileName: ControlFit.h
*  Author:   Robert Goor
*
*/
//
//  class ControlFit manages the search for the best subset of an array to maximize the dot product with a
//  specified target array.
//

#ifndef _CONTROLFIT_H_
#define _CONTROLFIT_H_

class ControlFit {

public:
	ControlFit (const double* target, int size);  // array of differences
	virtual ~ControlFit ();

	double Test3PointsOnRight (double* points);  // array size 3
	double Set3PointsOnRight (double* points);  // array size 3
	double TestNewPoint (double value);
	double TestNewPairOfPoints (double value1, double value2);
	int AddNewPoint (double value);  // returns 0 if OK
	void Reset ();

protected:
	double* mTarget;
	int mSize;
	int mCurrentFitIndex;
	int mCurrentFitIndex1;
	double mCurrentFitNormSquared;
	double* mTargetNorms;
	double* mTargetNormsByThrees;
	double mCurrentDotProduct;
	double mLatestPoint;

	double NormalizedDotProductOfLast3Points (double* points);
};



class ControlFitTwoSided {

public:
	ControlFitTwoSided (const double* values, const double* target, int size);  // target = array of differences
	virtual ~ControlFitTwoSided ();

	double Test3PointsOnRight (double* points);  // array size 3
	double Set3PointsOnRight (double* points);  // array size 3
	double Test3PointsOnLeft (double* points);  // array size 3
	double Set3PointsOnLeft (double* points);  // array size 3

	double SetNPointsOnRight (double* points, int N);  // array size N
	double SetNpointsOnLeft (double* points, int N);   // array size N

	double TestNewPointOnRight (double value);
	double TestNewPointOnLeft (double value);
	double TestNewPointOnRightOnly (double value);
	double TestNewPointOnLeftOnly (double value);
	int AddNewPointOnRight (double value);  // returns 0 if OK
	int AddNewPointOnLeft (double value);
	void Reset ();

protected:
	double* mValues;
	double* mTarget;
	int mSize;
	int mCurrentFitIndexRight;
	int mCurrentFitIndexLeft;
	double mCurrentFitNormSquaredLeft;
	double mCurrentFitNormSquaredRight;
	double* mTargetNormsRight;
	double* mTargetNormsLeft;
	double mCurrentDotProductLeft;
	double mCurrentDotProductRight;
	double mLatestPointRight;
	double mLatestPointLeft;
	int mLastAddedIndexRight;
	int mLastAddedIndexLeft;
	double mCumulativeNormLeft;
	double mCumulativeNormRight;

	double NormalizedDotProductOfLast3Points (double* points);
	double NormalizedDotProductOfFirst3Points (double* points);
};

#endif  /*  _CONTROLFIT_H_  */

