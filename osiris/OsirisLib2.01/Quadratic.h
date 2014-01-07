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
*  FileName: Quadratic.h
*  Author:   Robert Goor
*
*/
//
//  Contains class QuadraticFit and class Quadratic.  The former fits a quadratic function to data (at least three points) and the
// latter evaluates a given quadratic function.
//

#ifndef _QUADRATICFIT_H_
#define _QUADRATICFIT_H_

#include "rgdefs.h"
#include <cstdlib>

class Quadratic {

public:
	Quadratic ();
	Quadratic (const double* coefficients);  // array should be of size (at least) 3
	~Quadratic () {}

	double operator()(double x) const {

		return (a[0] * x + a[1]) * x + a[2];
	}

	double Evaluate (double x) const {

		return (a[0] * x + a[1]) * x + a[2];
	}

	double& operator() (int i) { return a[i]; }

protected:
	double a[3];
};


class QuadraticFit {

public:
	QuadraticFit ();
	QuadraticFit (const double* abscissas, int size);
	~QuadraticFit ();

	Boolean IsValid () const { return Abscissas != NULL; }

	Boolean Regress (const double* data, Quadratic& polynomial);
	Boolean Regress (const double* abscissas, const double* data, int size, Quadratic& polynomial);

	Boolean Regress (const double* data, Quadratic& polynomial, double& maxPctResidual);
	Boolean Regress (const double* abscissas, const double* data, int size, Quadratic& polynomial, double& maxPctResidual);

	void Reset (const double* abscissas, int size);

protected:
	double* Abscissas;
	double* Abscissas2;
	int Size;
	double a11;
	double a22;
	double a33;
	double a12;
	double a13;
	double a23;

	double MaxPctResidual (const double* abscissas, const double* data, int size, const Quadratic& polynomial) const;
};


#endif  /*  _QUADRATICFIT_H_   */


