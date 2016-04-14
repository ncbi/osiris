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


class LeastMedianOfSquares1D {

public:
	LeastMedianOfSquares1D (int n, double* x, double* y);
	LeastMedianOfSquares1D (int n, double* yOverXRatios);
	LeastMedianOfSquares1D (const list<double>& yOverXRatios);
	LeastMedianOfSquares1D (const list<double>& xValues, const list<double>& yValues);
	~LeastMedianOfSquares1D ();

	bool DataIsOK () const { return mIsOK; }
	double CalculateLMS ();
	bool ElementIsOutlier (int i) const;
	double GetLMSValue () const { return mLeastMedianValue; }
	double GetMedianSquaredForLMS () const { return mMedianResidual; }

protected:
	bool mIsOK;
	int mSize;
	double* mRatioArray;
	list<double> mRatioList;
	bool* mOutlierArray;
	double mLeastMedianValue;
	double mMedianResidual;
};


#endif /*  _LEASTMEDIANOFSQUARES_H_  */


