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
*  FileName: SpecialLinearRegression.h
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

#ifndef _SPECIALLINEARREGRESSION_H_
#define _SPECIALLINEARREGRESSION_H_



class SpecialLinearRegression {

public:
	SpecialLinearRegression (int numberOfPoints);
	~SpecialLinearRegression ();

	double RegressForwardFrom (double* array, double& averageY, double& range);
	double RegressBackwardFrom (double* array, double& averageY, double& range);

protected:
	int mNumberOfPoints;
	double mS;
	double mSx;
	double mSxx;
	double mDel;
};


#endif  /*  _SPECIALLINEARREGRESSION_H_  */

