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
*  FileName: SingularValueDecomposition.h
*  Author:   Robert Goor
*
*/
//
//  class SingularValueDecomposition computes the SVD of a large rectangular matrix and returns the parts.
// It also uses the computed SVD to solve the multiple linear least squares regression problem.
//

#ifndef _SINGULARVALUEDECOMPOSITION_H_
#define _SINGULARVALUEDECOMPOSITION_H_

#include "SampleData.h"


class SingularValueDecomposition {

public:
	SingularValueDecomposition (int rows, int columns);  // this is also ...int nDataSamples, int nChannels, hence
			// many rows and few columns (e.g., 7500 x 4
	SingularValueDecomposition (SampleData* data);  // extracts row and column info from "data"
	~SingularValueDecomposition ();

	bool isSVDComputed () const { return mSVDComputed; }
	bool isWInverseComputed () const { return mWInverseComputed; }

	void AddColumn (int columnNumber, double* column);
	void AddColumn (int columnNumber, int* column);
	void AddColumn (int columnNumber);  // uses SampleData from constructor
	void AddUnitColumn (int columnNumber);
	void PopulateDataMatrix ();  // uses SampleData from constructor

	double& A (int row, int column) { return mAU [column][row]; }
	double W (int diagonalEntry) const { return mW [diagonalEntry]; }  // up to 'columns' values
	double WInverse (int diagonalEntry) const { return mWInverse [diagonalEntry]; }
	double V (int row, int column) const { return mV [row][column]; }  // V is 'columns' by 'columns' (from constructor)
	double U (int row, int column) const { return mAU [column][row]; }

	bool ComputeSVD ();
	int CalculateWInverse ();
	bool PerformRegression (double* const coefficients, const double* const inputData);
	bool PerformRegression (double**& matrix);

	static double GetWCoefficientThreshold () { return sWCoefficientThreshold; }
	static void SetWCoefficientThreshold (double threshold) { sWCoefficientThreshold = threshold; }

protected:
	double** mAU;  // mAU is 'columns' by 'rows', e.g., 4 x 7500
	double* mW;
	double* mWInverse;
	double** mV;
	bool mSVDComputed;
	int mRows;
	int mColumns;
	bool* mCheckIn;
	bool mWInverseComputed;
	SampleData* mData;

	static double sWCoefficientThreshold;
	double sign (double x, double y);
	double dmax (double x, double y);
};

#endif  /*  _SINGULARVALUEDECOMPOSITION_H_  */

