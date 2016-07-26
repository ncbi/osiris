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



#ifndef _LEASTMEDIANOFSQUARES_H_
#define _LEASTMEDIANOFSQUARES_H_



#include <list>

using namespace std;


struct IntersectionPoint {

	IntersectionPoint (double x, double y) : mX (x), mY (y) {}
	~IntersectionPoint () {}

	double mX;
	double mY;
};


struct DualPoint {

	DualPoint (int line1, int line2) : mIndex1 (line1), mIndex2 (line2) {}
	DualPoint (const IntersectionPoint& pt, int line1, int line2) : mIndex1 (line1), mIndex2 (line2), mX (pt.mX), mY (pt.mY) {}
	DualPoint (const DualPoint& pt);
	~DualPoint () {}

	int mIndex1;
	int mIndex2;
	double mX;
	double mY;
};


class DualLine {

public:
	DualLine (int index, double slope, double intercept) : mIndex (index), mSlope (slope), mIntercept (intercept) {}
	DualLine (const DualLine& ol) { mIndex = ol.mIndex; mSlope = ol.mSlope; mIntercept = ol.mIntercept; }
	~DualLine () {}

	IntersectionPoint* FindIntercept (DualLine& otherLine);
	double YAtXEquals (double x);
	int GetIndex () const { return mIndex; }
	double GetSlope () const { return mSlope; }

protected:
	int mIndex;
	double mSlope;
	double mIntercept;
};


class MedianLineSegment {

public:
	MedianLineSegment (const DualPoint& vertex);
	~MedianLineSegment ();

	double CalculateMedianRange (const DualLine* sortedLines, DualLine* includedLines, DualLine& boundaryLine);

protected:
	DualPoint mVertex;
};


struct IndexedDouble {

	IndexedDouble& operator= (const DualLine& l) { mIndex = l.GetIndex (); mSlope = l.GetSlope (); return *this;}
	IndexedDouble& operator= (IndexedDouble& is);
	bool operator< (IndexedDouble& s) { return (mSlope < s.mSlope); }
	bool operator== (IndexedDouble& s) { return (mSlope == s.mSlope); }

	int mIndex;
	double mSlope;
};



class LeastMedianOfSquares {

public:
	LeastMedianOfSquares (int n, double* x, double* y);
	LeastMedianOfSquares (const list<double>& xValues, const list<double>& yValues);
	virtual ~LeastMedianOfSquares () = 0;

	bool DataIsOK () const { return mIsOK; }
	virtual double CalculateLMS () = 0;
	bool ElementIsOutlier (int i) const;
	double GetLMSValue () const { return mLeastMedianValue; }
	virtual double GetLMSValue2 () const { return 0.0; }
	double GetMedianSquaredForLMS () const { return mMedianResidual; }
	double GetOutlierThreshold () { return mOutlierThreshold; }

protected:
	bool mIsOK;
	int mSize;
	double* mXvalues;
	double* mYvalues;
	double* mRatioArray;
	bool* mOutlierArray;
	double mLeastMedianValue;
	double mMedianResidual;
	double mOutlierThreshold;
};


class LeastMedianOfSquares1D : public LeastMedianOfSquares {

public:
	LeastMedianOfSquares1D (int n, double* x, double* y);
	LeastMedianOfSquares1D (const list<double>& xValues, const list<double>& yValues);
	virtual ~LeastMedianOfSquares1D ();

	 virtual double CalculateLMS ();

protected:
};


class LeastMedianOfSquares2DExhaustive : public LeastMedianOfSquares {

public:
	LeastMedianOfSquares2DExhaustive (int n, double* x, double* y);
	LeastMedianOfSquares2DExhaustive (const list<double>& xValues, const list<double>& yValues);
	virtual ~LeastMedianOfSquares2DExhaustive ();

	 virtual double CalculateLMS ();
	 virtual double GetLMSValue2 () const { return mLeastMedianValue2; }

protected:
	double mLeastMedianValue2;
	int mMedianNumber;
	int mMedianIncrement;
	int mMedianNumberLess2;
	DualLine** mLines;
	DualLine** mSortedLines;

	IntersectionPoint*** CalculateIntersectionMatrix ();
	IndexedDouble CalculateMedianLineSegment (DualPoint* vertex);   // returns y coordinate of intersection with dual line.
	void CreateDualLineArray ();
	bool TestForValidity (DualPoint* vertex, IndexedDouble& id);
	void DeleteVertexMatrix (IntersectionPoint*** vertexMatrix);
};


class LeastSquaresQuadraticModel {

public:
	LeastSquaresQuadraticModel (int n, double* x, double* y);
	LeastSquaresQuadraticModel (const list<double>& xValues, const list<double>& yValues);
	~LeastSquaresQuadraticModel ();

	bool DataIsOK () const { return mIsOK; }
	double GetLinearTerm () const { return mLinearTerm; }
	double GetQuadraticTerm () const { return mQuadraticTerm; }
	double GetLeastSumOfSquares () const { return mLeastSquare; }

	bool TestXValuesForIndependence ();
	bool CreateArrayOfXSquares ();

	double CalculateLeastSquare (double& linearTerm, double& quadTerm);

protected:
	bool mIsOK;
	int mSize;
	double* mXvalues;
	double* mYvalues;
	double* mX2values;
	double mLinearTerm;
	double mQuadraticTerm;
	double mLeastSquare;
	
	double CalculateLeastSquareWithOneTermZero (double& linearTerm, double& quadTerm);
};


#endif /*  _LEASTMEDIANOFSQUARES_H_  */


