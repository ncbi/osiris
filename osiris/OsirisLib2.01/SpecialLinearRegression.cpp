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
*  FileName: SpecialLinearRegression.cpp
*  Author:   Robert Goor
*
*/
//
//  class SpecialLinearRegression performs linear regression on a predetermined number of points but returns only the linear term, and the
//  data average, but not the constant term.  This will be useful in dealing with STR raw data from .fsa files.  This class uses the 
//  formulas in "Numerical Recipes" by Press, Flannery, Teukolsky, and Vetterling, p. 505.  The class assumes that the "x" values are:
//  1, 2, 3,...., N, where N is the numberOfPoints set on construction.  Regression can occur forward or backward from the indicated array
//  pointer that is input to the regression calculation.
//

#include "SpecialLinearRegression.h"

SpecialLinearRegression :: SpecialLinearRegression (int numberOfPoints) : mNumberOfPoints (numberOfPoints) {

	mS = (double)numberOfPoints;
	mSx = (mS * (mS + 1.0)) / 2.0;
	mSxx = (mS * (mS + 1.0) * (2.0 * mS + 1.0)) / 6.0;
	mDel = mS * mSxx - (mSx * mSx);
}



SpecialLinearRegression :: ~SpecialLinearRegression () {

}


double SpecialLinearRegression :: RegressForwardFrom (double* array, double& averageY, double& range) {

	double Sy = 0.0;
	double Sxy = 0.0;
	double* ptr = array;
	double maxValue = array [0];
	double minValue = array [0];
	range = 0.0;

	for (int i=1; i<=mNumberOfPoints; i++) {

		Sy += *ptr;
		Sxy += ((double)i) * (*ptr);

		if (*ptr > maxValue)
			maxValue = *ptr;

		if (*ptr < minValue)
			minValue = *ptr;

		ptr++;
	}

	if (mNumberOfPoints > 0)
		averageY = Sy / mS;

	else {

		averageY = 0.0;
		return -100000.0;
	}

	range = maxValue - minValue;
	return (mS * Sxy - mSx * Sy) / mDel;  // This is equation 14.2.6 on p. 505
}


double SpecialLinearRegression :: RegressBackwardFrom (double* array, double& averageY, double& range) {

	double Sy = 0.0;
	double Sxy = 0.0;
	double* ptr = array;
	double maxValue = array [0];
	double minValue = array [0];
	range = 0.0;

	for (int i=mNumberOfPoints; i>=1; i--) {

		Sy += *ptr;
		Sxy += ((double)i) * (*ptr);

		if (*ptr > maxValue)
			maxValue = *ptr;

		if (*ptr < minValue)
			minValue = *ptr;

		ptr--;
	}

	if (mNumberOfPoints > 0)
		averageY = Sy / mS;

	else {

		averageY = 0.0;
		return -100000.0;
	}

	range = maxValue - minValue;
	return (mS * Sxy - mSx * Sy) / mDel;  // This is equation 14.2.6 on p. 505
}


