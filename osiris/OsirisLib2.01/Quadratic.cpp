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
*  FileName: Quadratic.cpp
*  Author:   Robert Goor
*
*/
//
//  Contains class QuadraticFit and class Quadratic.  The former fits a quadratic function to data (at least three points) and the
// latter evaluates a given quadratic function.
//


#include "Quadratic.h"
#include <math.h>
#include <stdlib.h>


Quadratic :: Quadratic () {

	a[0] = a[1] = a[2] = 0.0;
}


Quadratic :: Quadratic (const double* coefficients) {

	a[0] = *coefficients;
	a[1] = *(coefficients + 1);
	a[2] = *(coefficients + 2);
}


QuadraticFit :: QuadraticFit () : Abscissas (NULL), Abscissas2 (NULL), Size (0) {}



QuadraticFit :: QuadraticFit (const double* abscissas, int size) : Abscissas (NULL), Abscissas2 (NULL) {

	Reset (abscissas, size);
}



QuadraticFit :: ~QuadraticFit () {

	delete [] Abscissas;
	delete [] Abscissas2;
}


Boolean QuadraticFit :: Regress (const double* data, Quadratic& polynomial) {

	if (Abscissas == NULL)
		return FALSE;
	
	double V0 = 0.0;
	double V1 = 0.0;
	double V2 = 0.0;
	double temp;
	double temps;

	for (int i=0; i<Size; i++) {

		temp = Abscissas [i];
		temps = data [i];

		V0 += temps;  // Sum of data [i]
		V1 += Abscissas [i] * temps;  // Sum of Abscissas * data
		V2 += Abscissas2 [i] * temps;  // Sum of Abscissas^2 * data
	}

	polynomial (0) = a11 * V2 + a12 * V1 + a13 * V0;
	polynomial (1) = a12 * V2 + a22 * V1 + a23 * V0;
	polynomial (2) = a13 * V2 + a23 * V1 + a33 * V0;

	return TRUE;
}


Boolean QuadraticFit :: Regress (const double* abscissas, const double* data, int size, Quadratic& polynomial) {

	if (size < 3)
		return FALSE;
	
	double U1 = 0.0;
	double U2 = 0.0;
	double U3 = 0.0;
	double U4 = 0.0;
	double V0 = 0.0;
	double V1 = 0.0;
	double V2 = 0.0;
	double temp;
	double temp2;
	double temp3;
	double temps;

	for (int i=0; i<size; i++) {

		temp = abscissas [i];
		temps = data [i];
		U1 += temp;  // Sum of means
		temp2 = temp * temp;
		temp3 = temp * temp2;
		U2 += temp2;  // Sum of means^2
		U3 += temp3;  // Sum of means^3
		U4 += temp3 * temp;  // Sum of means^4

		V0 +=temps;  // Sum of sigmas
		V1 += temp * temps;  // Sum of means * sigmas
		V2 += temp2 * temps;  // Sum of means^2 * sigmas
	}

	double U12 = U1 * U1;
	double U22 = U2 * U2;
	double U32 = U3 * U3;

	double U1U2 = U1 * U2;
	double U1U3 = U1 * U3;
	double U2U3 = U2 * U3;

	double NU2minusU12 = size * U2 - U12;
	double NU4minusU22 = size * U4 - U22;
	double U2U4minusU32 = U2 * U4 - U32;
	double NU3minusU1U2 = size * U3 - U1U2;
	double U1U4minusU2U3 = U1 * U4 - U2U3;
	double U1U3minusU22 = U1 * U3 - U22;

	double detA = size * U2 * U4 + 2.0 * U1 * U2 * U3 - (U22 * U2 + size * U32 + U12 * U4);

	if (detA == 0.0)
		return FALSE;

	polynomial (0) = (NU2minusU12 * V2 - NU3minusU1U2 * V1 + U1U3minusU22 * V0) / detA;
	polynomial (1) = (- NU3minusU1U2 * V2 + NU4minusU22 * V1 - U1U4minusU2U3 * V0) / detA;
	polynomial (2) = (U1U3minusU22 * V2 - U1U4minusU2U3 * V1 + U2U4minusU32 * V0) / detA;

	return TRUE;
}



Boolean QuadraticFit :: Regress (const double* data, Quadratic& polynomial, double& maxPctResidual) {

	if (!Regress (data, polynomial))
		return FALSE;

	maxPctResidual = MaxPctResidual (Abscissas, data, Size, polynomial);
	return TRUE;
}



Boolean QuadraticFit :: Regress (const double* abscissas, const double* data, int size, Quadratic& polynomial, double& maxPctResidual) {

	if (!Regress (abscissas, data, size, polynomial))
		return FALSE;

	maxPctResidual = MaxPctResidual (abscissas, data, size, polynomial);
	return TRUE;
}



void QuadraticFit :: Reset (const double* abscissas, int size) {

	delete [] Abscissas;
	delete [] Abscissas2;
	Size = size;

	if (size < 3) {

		Size = 0;
		return;
	}

	Abscissas = new double [size];
	Abscissas2 = new double [size];

	double U1 = 0.0;
	double U2 = 0.0;
	double U3 = 0.0;
	double U4 = 0.0;
	double temp;
	double temp2;
	double temp3;

	for (int i=0; i<size; i++) {

		Abscissas [i] = temp = abscissas [i];
		U1 += temp;  // Sum of abscissas
		Abscissas2 [i] = temp2 = temp * temp;
		temp3 = temp * temp2;
		U2 += temp2;  // Sum of abscissas^2
		U3 += temp3;  // Sum of abscissas^3
		U4 += temp3 * temp;  // Sum of abscissas^4
	}

	double U12 = U1 * U1;
	double U22 = U2 * U2;
	double U32 = U3 * U3;

	double U1U2 = U1 * U2;
	double U1U3 = U1 * U3;
	double U2U3 = U2 * U3;

	double NU2minusU12 = size * U2 - U12;
	double NU4minusU22 = size * U4 - U22;
	double U2U4minusU32 = U2 * U4 - U32;
	double NU3minusU1U2 = size * U3 - U1U2;
	double U1U4minusU2U3 = U1 * U4 - U2U3;
	double U1U3minusU22 = U1 * U3 - U22;

	double detA = size * U2 * U4 + 2.0 * U1 * U2 * U3 - (U22 * U2 + size * U32 + U12 * U4);

	if (detA == 0.0) {

//		delete [] Abscissas;
//		delete [] Abscissas2;
		Size = 0;
		return;
	}

//	double ReciprocalDetA = 1.0 / detA;

//	parameters [0] = (NU2minusU12 * V2 - NU3minusU1U2 * V1 + U1U3minusU22 * V0) / detA;
//	parameters [1] = (- NU3minusU1U2 * V2 + NU4minusU22 * V1 - U1U4minusU2U3 * V0) / detA;
//	parameters [2] = (U1U3minusU22 * V2 - U1U4minusU2U3 * V1 + U2U4minusU32 * V0) / detA;

//	a11 = NU2minusU12 * ReciprocalDetA;
//	a22 = NU4minusU22 * ReciprocalDetA;
//	a33 = U2U4minusU32 * ReciprocalDetA;
//	a12 = - NU3minusU1U2 * ReciprocalDetA;
//	a13 = U1U3minusU22 * ReciprocalDetA;
//	a23 = - U1U4minusU2U3 * ReciprocalDetA;

	a11 = NU2minusU12 / detA;
	a22 = NU4minusU22 / detA;
	a33 = U2U4minusU32 / detA;
	a12 = - NU3minusU1U2 / detA;
	a13 = U1U3minusU22 / detA;
	a23 = - U1U4minusU2U3 / detA;
}


double QuadraticFit :: MaxPctResidual (const double* abscissas, const double* data, int size, const Quadratic& polynomial) const {

	double MaxResidual = 0.0;
	double Residual;
	double y;

	for (int i=0; i<size; i++) {

		y = data [i];
		Residual = abs (y - polynomial (abscissas [i]));
		y = abs (y);

		if ((Residual <= 1.0e50 * y) && (Residual != 0.0)) {

			Residual = Residual / y;

			if (Residual > MaxResidual)
				MaxResidual = Residual;
		}
	}

	return MaxResidual;
}


