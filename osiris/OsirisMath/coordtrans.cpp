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
*  FileName: coordtrans.cpp
*  Author:   Robert Goor
*
*/
//
// Source for class CoordinateTransform and subclasses that perform coordinate transforms using different algorithms.  Allows evaluation on a one-time
// basis, as one of a sequence of calls at equal spacing, or, for maximum efficiency when multiple calls are required at equal spacing, allows evaluation
// of an array of transform values based on an initial value, a spacing and either a number of evaluations or a final value.
//

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "rgdefs.h"
#include "coordtrans.h"

using namespace std;

const double cForError = 5.0 / 384.0;

CoordinateTransform :: CoordinateTransform (const list<double>& coord1, const list<double>& coord2) : Spacing (0.0),
CurrentSequenceCount (-1),
TotalSequenceCount (-1),
ErrorFlag (0) {

	if ((coord1.size () < 2) || (coord2.size () < 2))
		ErrorFlag = 1;

	else {

		list<double>::const_iterator c1Iterator = coord1.begin ();
		Left = LastAbscissa = LastInSequenceAbscissa = *c1Iterator;
		c1Iterator = coord1.end ();
		c1Iterator--;
		Right = *c1Iterator;
		list<double>::const_iterator c2Iterator = coord2.begin ();
		LastValue = LastValueInSequence = *c2Iterator;
	}

	if (coord2.size () != coord1.size ())
		ErrorFlag = 1;
}


CoordinateTransform :: CoordinateTransform (const double* coord1, const double* coord2, int size) : Spacing (0.0),
CurrentSequenceCount (-1),
TotalSequenceCount (-1),
ErrorFlag (0) {

	if ((coord1 == NULL) || (coord2 == NULL) || (size < 2))
		ErrorFlag = 1;

	else {

		int size1 = size - 1;
		Left = LastAbscissa = LastInSequenceAbscissa = coord1 [0];
		Right = coord1 [size1];
		LastValue = LastValueInSequence = coord2 [0];
	}
}

//
// The following not needed yet:
//
/*CoordinateTransform :: CoordinateTransform (const list<double>& coord1, const list<double>& coord2, const CoefficientFactory& factory) :  
Spacing (0.0), ErrorFlag (0) {

	Left = LastAbscissa = LastInSequenceAbscissa = coord1.begin ();
	Right = coord1.end ();
	LastValue = LastValueInSequence = coord2.begin ();

	if (coord2.size () != coord1.size ())
		ErrorFlag = 1;
}*/


CoordinateTransform :: ~CoordinateTransform () {

}


double CoordinateTransform :: Evaluate (double abscissa) {
	
	// single evaluation, not part of a sequence
	LastAbscissa = abscissa;
	LastValue = DOUBLEMAX;
	return DOUBLEMAX;
}


double CoordinateTransform :: EvaluateWithExtrapolation (double abscissa) {

	return DOUBLEMAX;
}


double CoordinateTransform :: EvaluateWithExtrapolation (double abscissa, double& yPrime) {

	yPrime = 0.0;
	return DOUBLEMAX;
}


double CoordinateTransform :: EvaluateFirstDerivative (double abscissa) {

	return 0.0;
}


double CoordinateTransform :: EvaluateSequenceStart (double startAbscissa, double spacing) {

	LastInSequenceAbscissa = startAbscissa;
	Spacing = spacing;
	LastValueInSequence = DOUBLEMAX;
	return DOUBLEMAX;
}


double CoordinateTransform :: EvaluateSequenceNext () {
	
	//  evaluates an equally spaced sequence, one at a time
	LastInSequenceAbscissa += Spacing;
	LastValueInSequence = DOUBLEMAX;
	return DOUBLEMAX;
}


double CoordinateTransform :: EvaluateSequenceNext (double abscissaStart, int count) {

	//  evaluates an equally spaced sequence, one at a time
	LastInSequenceAbscissa = abscissaStart + count * Spacing;
	LastValueInSequence = DOUBLEMAX;
	return DOUBLEMAX;
}


// evaluates sequence of length 'size' and places in result; returns >= 0 if all OK
int CoordinateTransform :: EvaluateFullSequence (double* result, double startAbscissa, double spacing, int size) {

	return -1;
}


// evaluates sequence of length 'size' based on irregularly spaced abscissas in 'abscissas'; returns >=0 on OK
int CoordinateTransform :: EvaluateFullSequence (const double* abscissas, double* result, int size) {

	return -1;
}


double CoordinateTransform :: MaxSecondDerivative () const {

	return DOUBLEMAX;
}


double CoordinateTransform :: MaxDeltaThirdDerivative () const {

	return DOUBLEMAX;
}



CSplineTransform :: CSplineTransform (const list<double>& coord1, const list<double>& coord2) : CoordinateTransform (coord1, coord2),
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL), mIsHermite (false) {

	if (ErrorFlag == 0) {
	
		NumberOfKnots = (int)coord1.size (); // n+1
		NumberOfCubics = NumberOfKnots - 1; // n
		Knots = new double [NumberOfKnots];
		Ordinates = new double [NumberOfKnots];
		double* temp;
		list <double>::const_iterator c1Iterator;
		list <double>::const_iterator c2Iterator;

		A = new double [NumberOfCubics];
		B = new double [NumberOfCubics];
		C = new double [NumberOfCubics];
		D = new double [NumberOfCubics];

		temp = Knots;

		for (c1Iterator = coord1.begin (); c1Iterator != coord1.end (); c1Iterator++) {

			*temp = *c1Iterator;
			temp++;
		}

		Left = Knots [0];
		Right = Knots [NumberOfCubics];
		temp = Ordinates;

		for (c2Iterator = coord2.begin (); c2Iterator != coord2.end (); c2Iterator++) {

			*temp = *c2Iterator;
			temp++;
		}

		Initialize ();
	}

	else {

		NumberOfKnots = NumberOfCubics = 0;
	}
}


CSplineTransform :: CSplineTransform (const double* coord1, const double* coord2, int size) : CoordinateTransform (coord1, coord2, size),
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL), mIsHermite (false) {

	if (ErrorFlag == 0) {
	
		NumberOfKnots = size; // n+1
		NumberOfCubics = NumberOfKnots - 1; // n
		Knots = new double [NumberOfKnots];
		Ordinates = new double [NumberOfKnots];

		A = new double [NumberOfCubics];
		B = new double [NumberOfCubics];
		C = new double [NumberOfCubics];
		D = new double [NumberOfCubics];

		for (int i=0; i<size; i++) {

			Knots [i] = coord1 [i];
			Ordinates [i] = coord2 [i];
		}

		Left = Knots [0];
		Right = Knots [NumberOfCubics];
		Initialize ();
	}

	else {

		NumberOfKnots = NumberOfCubics = 0;
	}
}


CSplineTransform :: CSplineTransform (const double* coord1, const double* coord2, int size, const SupplementaryData& ExtraData) : CoordinateTransform (coord1, coord2, size),
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL), mIsHermite (false) {

	if (ErrorFlag == 0) {

		double MinDistance = DOUBLEMAX;
		double temp;

		for (int k=1; k<size; k++) {

			temp = coord1 [k] - coord1 [k - 1];

			if (temp < MinDistance)
				MinDistance = temp;
		}

		double proximity = ExtraData.proximityMultiple * MinDistance;
	
		NumberOfKnots = size + ExtraData.size; // n+1
		NumberOfCubics = NumberOfKnots - 1; // n
		Knots = new double [NumberOfKnots];
		Ordinates = new double [NumberOfKnots];

		A = new double [NumberOfCubics];
		B = new double [NumberOfCubics];
		C = new double [NumberOfCubics];
		D = new double [NumberOfCubics];

		int i = 0;
		int j = 0;
		int Total = 0;
		double LastData = -DOUBLEMAX;
		double testC1 = coord1 [0];
		double testExtra;
		
		if (ExtraData.size > 0)
			testExtra = ExtraData.abscissas [0];

		else
			testExtra = DOUBLEMAX;

		while ((i < size) || (j < ExtraData.size)) {

			if (testC1 <= testExtra) {

				Knots [Total] = LastData = testC1;
				Ordinates [Total] = coord2 [i];
				Total++;
				i++;
			}

			else {

				if ((testExtra - LastData > proximity) && (testC1 - testExtra > proximity)) {

					Knots [Total] = testExtra;
					Ordinates [Total] = ExtraData.ordinates [j];
					Total++;
					j++;
				}

				else {

					j++;
				}
			}

			if (i >= size)
				testC1 = DOUBLEMAX;

			else
				testC1 = coord1 [i];

			if (j >= ExtraData.size)
				testExtra = DOUBLEMAX;

			else
				testExtra = ExtraData.abscissas [j];
		}

		NumberOfKnots = Total;
		NumberOfCubics = Total - 1;

		Left = Knots [0];
		Right = Knots [NumberOfCubics];
		LastValue = LastValueInSequence = Ordinates [0];
		Initialize ();
	}

	else {

		NumberOfKnots = NumberOfCubics = 0;
	}
}


CSplineTransform :: CSplineTransform (const double* coord1, const double* coord2, const double* derivs, int size, bool isHermite) : CoordinateTransform (coord1, coord2, size),
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL), mIsHermite (isHermite) {

	int i;

	if (ErrorFlag == 0) {
	
		NumberOfKnots = size; // n+1
		NumberOfCubics = NumberOfKnots - 1; // n
		Knots = new double [NumberOfKnots];
		Ordinates = new double [NumberOfKnots];

		A = new double [NumberOfCubics];
		B = new double [NumberOfCubics];
		C = new double [NumberOfCubics];
		D = new double [NumberOfCubics];

		for (i=0; i<size; i++) {

			Knots [i] = coord1 [i];
			Ordinates [i] = coord2 [i];
		}

		Left = Knots [0];
		Right = Knots [NumberOfCubics];

		if (isHermite)
			InitializeHermite (derivs);

		else
			Initialize ();
	}

	else {

		NumberOfKnots = NumberOfCubics = 0;
	}
}



CSplineTransform :: CSplineTransform (const double* coord1, const double* coord2, int size, bool isHermite) : CoordinateTransform (coord1, coord2, size),
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL), mIsHermite (isHermite) {

	int i;

	if (ErrorFlag == 0) {
	
		NumberOfKnots = size; // n+1
		NumberOfCubics = NumberOfKnots - 1; // n
		Knots = new double [NumberOfKnots];
		Ordinates = new double [NumberOfKnots];

		A = new double [NumberOfCubics];
		B = new double [NumberOfCubics];
		C = new double [NumberOfCubics];
		D = new double [NumberOfCubics];

		for (i=0; i<size; i++) {

			Knots [i] = coord1 [i];
			Ordinates [i] = coord2 [i];
		}

		Left = Knots [0];
		Right = Knots [NumberOfCubics];

		if (isHermite) {

			double* derivs = new double [NumberOfKnots];
			double* differences = new double [NumberOfCubics];

			for (i=0; i<NumberOfCubics; i++)
				differences [i] = (Ordinates [i+1] - Ordinates [i]) / (Knots [i+1] - Knots [i]);

			derivs [0] = differences [0];
			derivs [NumberOfCubics] = differences [NumberOfCubics-1];

			for (i=1; i<NumberOfCubics; i++)
				derivs [i] = 0.5 * (differences [i] + differences [i-1]);

			for (i=1; i<NumberOfKnots; i++) {

				if (Ordinates [i] == Ordinates [i-1])
					derivs [i] = derivs [i-1] = 0.0;
			}

			for (i=1; i<NumberOfCubics; i++) {

				if ((Ordinates [i] < Ordinates [i-1]) && (Ordinates [i] < Ordinates [i+1]))
					derivs [i] = 0.0;

				else if ((Ordinates [i] > Ordinates [i-1]) && (Ordinates [i] > Ordinates [i+1]))
					derivs [i] = 0.0;
			}

			InitializeHermite (derivs);
			delete[] derivs;
			delete[] differences;
		}

		else
			Initialize ();
	}

	else {

		NumberOfKnots = NumberOfCubics = 0;
	}
}


CSplineTransform :: CSplineTransform (const list<double>& coord1, const list<double>& coord2, bool isHermite) : CoordinateTransform (coord1, coord2),
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL), mIsHermite (isHermite) {

	int i;

	if (ErrorFlag == 0) {
	
		NumberOfKnots = (int)coord1.size (); // n+1
		NumberOfCubics = NumberOfKnots - 1; // n
		Knots = new double [NumberOfKnots];
		Ordinates = new double [NumberOfKnots];
		double* temp;
		list <double>::const_iterator c1Iterator;
		list <double>::const_iterator c2Iterator;

		A = new double [NumberOfCubics];
		B = new double [NumberOfCubics];
		C = new double [NumberOfCubics];
		D = new double [NumberOfCubics];

		temp = Knots;

		for (c1Iterator = coord1.begin (); c1Iterator != coord1.end (); c1Iterator++) {

			*temp = *c1Iterator;
			temp++;
		}

		Left = Knots [0];
		Right = Knots [NumberOfCubics];
		temp = Ordinates;

		for (c2Iterator = coord2.begin (); c2Iterator != coord2.end (); c2Iterator++) {

			*temp = *c2Iterator;
			temp++;
		}

		if (isHermite) {

			double* derivs = new double [NumberOfKnots];
			double* differences = new double [NumberOfCubics];

			for (i=0; i<NumberOfCubics; i++)
				differences [i] = (Ordinates [i+1] - Ordinates [i]) / (Knots [i+1] - Knots [i]);

			derivs [0] = differences [0];
			derivs [NumberOfCubics] = differences [NumberOfCubics-1];

			for (i=1; i<NumberOfCubics; i++)
				derivs [i] = 0.5 * (differences [i] + differences [i-1]);

			for (i=1; i<NumberOfKnots; i++) {

				if (Ordinates [i] == Ordinates [i-1])
					derivs [i] = derivs [i-1] = 0.0;
			}

			for (i=1; i<NumberOfCubics; i++) {

				if ((Ordinates [i] < Ordinates [i-1]) && (Ordinates [i] < Ordinates [i+1]))
					derivs [i] = 0.0;

				else if ((Ordinates [i] > Ordinates [i-1]) && (Ordinates [i] > Ordinates [i+1]))
					derivs [i] = 0.0;
			}

		//	cout << "Ready to initialize Hermite spline" << endl;
			InitializeHermite (derivs);
			delete[] derivs;
			delete[] differences;
		}

		else
			Initialize ();
	}

	else {

		NumberOfKnots = NumberOfCubics = 0;
	}
}



//
//  The next two constructors use a method described by M. Steffen in "A Simple Method for Monotonic Interpolation in One Dimension", Astronomy and Astrophysics
//  239, 443-450 (1990).  This method guarantees that each interpolating Hermite cubic is monotone, that the resulting spline is a continuous function of the input
//  points and that all spline effects are local
//



CSplineTransform :: CSplineTransform (const double* coord1, const double* coord2, int size, bool isHermite, bool useQuad) : CoordinateTransform (coord1, coord2, size),
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL), mIsHermite (isHermite) {

	int i;

	if (ErrorFlag == 0) {
	
		NumberOfKnots = size; // n+1
		NumberOfCubics = NumberOfKnots - 1; // n
		Knots = new double [NumberOfKnots];
		Ordinates = new double [NumberOfKnots];

		A = new double [NumberOfCubics];
		B = new double [NumberOfCubics];
		C = new double [NumberOfCubics];
		D = new double [NumberOfCubics];

		for (i=0; i<size; i++) {

			Knots [i] = coord1 [i];
			Ordinates [i] = coord2 [i];
		}

		Left = Knots [0];
		Right = Knots [NumberOfCubics];

		if (isHermite) {

			double* derivs = new double [NumberOfKnots];
			double* s = new double [NumberOfCubics];
			double* p = new double [NumberOfKnots];
			double* h = new double [NumberOfCubics];
			double m;
			double M;
			double temp;
			double sgn;

			for (i=0; i<NumberOfCubics; i++) {

				h [i] = Knots [i+1] - Knots [i];
				s [i] = (Ordinates [i+1] - Ordinates [i]) / h[i];
			}

			for (i=1; i<NumberOfCubics; i++)
				p [i] = (s [i-1] * h [i] + s [i] * h [i-1]) / (h [i-1] + h [i]);

			for (i=1; i<NumberOfCubics; i++) {

				if (s [i] * s [i-1] <= 0.0)
					derivs [i] = 0.0;

				else {

					m = M = fabs (s [i-1]);
					temp = fabs (s [i]);

					if (s [i] < 0.0)
						sgn = -1.0;

					else
						sgn = 1.0;
					
					if (temp < m)
						m = temp;

					else
						M = temp;

					if (fabs (p [i]) > 2.0 * M)
						derivs [i] = 2.0 * sgn * m;

					else
						derivs [i] = p [i];
				}
			}

			derivs [0] = 1.5 * s [0] - 0.5 * derivs [1];
			derivs [NumberOfCubics] = 1.5 * s [NumberOfCubics - 1] - 0.5 * derivs [NumberOfCubics - 2];

			InitializeHermite (derivs);
			delete[] derivs;
			delete[] s;
			delete[] p;
			delete[] h;
		}

		else
			Initialize ();
	}

	else {

		NumberOfKnots = NumberOfCubics = 0;
	}
}
	


CSplineTransform :: CSplineTransform (const list<double>& coord1, const list<double>& coord2, bool isHermite, bool useQuad) : CoordinateTransform (coord1, coord2),
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL), mIsHermite (isHermite) {

	int i;

	if (ErrorFlag == 0) {
	
		NumberOfKnots = (int)coord1.size (); // n+1
		NumberOfCubics = NumberOfKnots - 1; // n
		Knots = new double [NumberOfKnots];
		Ordinates = new double [NumberOfKnots];
		double* temp;
		list <double>::const_iterator c1Iterator;
		list <double>::const_iterator c2Iterator;

		A = new double [NumberOfCubics];
		B = new double [NumberOfCubics];
		C = new double [NumberOfCubics];
		D = new double [NumberOfCubics];

		temp = Knots;

		for (c1Iterator = coord1.begin (); c1Iterator != coord1.end (); c1Iterator++) {

			*temp = *c1Iterator;
			temp++;
		}

		Left = Knots [0];
		Right = Knots [NumberOfCubics];
		temp = Ordinates;

		for (c2Iterator = coord2.begin (); c2Iterator != coord2.end (); c2Iterator++) {

			*temp = *c2Iterator;
			temp++;
		}

		if (isHermite) {

			double* derivs = new double [NumberOfKnots];
			double* s = new double [NumberOfCubics];
			double* p = new double [NumberOfKnots];
			double* h = new double [NumberOfCubics];
			double m;
			double M;
			double temp1;
			double sgn;

			for (i=0; i<NumberOfCubics; i++) {

				h [i] = Knots [i+1] - Knots [i];
				s [i] = (Ordinates [i+1] - Ordinates [i]) / h[i];
			}

			for (i=1; i<NumberOfCubics; i++)
				p [i] = (s [i-1] * h [i] + s [i] * h [i-1]) / (h [i-1] + h [i]);

			for (i=1; i<NumberOfCubics; i++) {

				if (s [i] * s [i-1] <= 0.0)
					derivs [i] = 0.0;

				else {

					m = M = fabs (s [i-1]);
					temp1 = fabs (s [i]);

					if (s [i] < 0.0)
						sgn = -1.0;

					else
						sgn = 1.0;
					
					if (temp1 < m)
						m = temp1;

					else
						M = temp1;

					if (fabs (p [i]) > 2.0 * M)
						derivs [i] = 2.0 * sgn * m;

					else
						derivs [i] = p [i];
				}
			}

			derivs [0] = 1.5 * s [0] - 0.5 * derivs [1];
			derivs [NumberOfCubics] = 1.5 * s [NumberOfCubics - 1] - 0.5 * derivs [NumberOfCubics - 2];
			InitializeHermite (derivs);
			delete[] derivs;
			delete[] s;
			delete[] p;
			delete[] h;
		}

		else
			Initialize ();
	}

	else {

		NumberOfKnots = NumberOfCubics = 0;
	}
}



int CSplineTransform :: Initialize () {

	int j;
	double alpha;
	double p;

	double* lambda = new double [NumberOfKnots];
	double* mu = new double [NumberOfKnots];
	double* h = new double [NumberOfKnots];
	double* d = new double [NumberOfKnots];
	double* moments = new double [NumberOfKnots];
	double* q = new double [NumberOfKnots];
	double* u = new double [NumberOfKnots];
	double* del = new double [NumberOfKnots];
	double* gamma = new double [NumberOfKnots];

	//
	// Now we set up the cubic coefficients for the natural spline.  Algorithm taken from:
	// "Introduction to Numerical Analysis", Stoer, J. and Bulirsch, R., Second Edition, Springer-Verlag,
	// NY, Inc., Texts in Applied Mathematics 12, translated by R. Bartels, W. Gautschi, and C. Witzgall, 
	// 1993:  pages 97 - 102.
	//
	// Commented lines are replaced successively by reorganized code to improve operational efficiency.
	//

	for (j=0; j<NumberOfCubics; j++) {  // from 0 to n-1

		h[j+1] = Knots[j+1] - Knots[j];
		del[j+1] = Ordinates[j+1] - Ordinates[j];
		gamma[j+1] = del[j+1] / h[j+1];
	}

	for (j=1; j<NumberOfCubics; j++) {  // from 1 to n-1

		alpha = 1.0 / (h[j] + h[j+1]);
//		lambda[j] = h[j+1] / (h[j] + h[j+1]);
		lambda[j] = h[j+1] * alpha;
		mu[j] = 1.0 - lambda[j];
//		d[j] = (6.0 / (h[j] + h[j+1])) * (((Ordinates[j+1] - Ordinates[j]) / h[j+1]) - ((Ordinates[j] - Ordinates[j-1]) / h[j]));
//		d[j] = (6.0 / (h[j] + h[j+1])) * ((del[j+1] / h[j+1]) - (del[j] / h[j]));
//		d[j] = 6.0 * alpha * ((del[j+1] / h[j+1]) - (del[j] / h[j]));
		d[j] = 6.0 * alpha * (gamma[j+1] - gamma[j]);
	}

	lambda[0] = d[0] = mu[NumberOfCubics] = d[NumberOfCubics] = 0.0;
	q[0] = - 0.5 * lambda[0];
	u[0] = 0.5 * d[0];
	lambda [NumberOfCubics] = 0.0;

	for (j=1; j<=NumberOfCubics; j++) {

//		p[j] = mu[j] * q[j-1] + 2.0;
//		q[j] = - lambda[j] / p[j];
//		u[j] = (d[j] - mu[j] * u[j-1]) / p[j];
		p = mu[j] * q[j-1] + 2.0;
		q[j] = - lambda[j] / p;
		u[j] = (d[j] - mu[j] * u[j-1]) / p;
	}

	moments [NumberOfCubics] = u[NumberOfCubics];
	double OneSixth = 1.0 / 6.0;

	for (j=NumberOfCubics-1; j>=0; j--)
		moments[j] = q[j] * moments[j+1] + u[j];

	for (j=0; j<NumberOfCubics; j++) {

		// These are coefficient for polynomial between Knots [j] and Knots [j+1]
		A[j] = Ordinates[j];
		C[j] = 0.5 * moments[j];
//		B[j] = ((Ordinates[j+1] - Ordinates[j]) / h[j+1]) - (2.0 * moments[j] + moments[j+1]) * OneSixth * h[j+1];
//		B[j] = (del[j+1] / h[j+1]) - (2.0 * moments[j] + moments[j+1]) * OneSixth * h[j+1];
		B[j] = gamma[j+1] - (2.0 * moments[j] + moments[j+1]) * OneSixth * h[j+1];
		D[j] = (moments[j+1] - moments[j]) * OneSixth / h[j+1];
	}

//	mLeft = (3.0 * D [0] * Left + 2.0 * C [0]) * Left + B [0];  // This is an error...B[0] is the left-most 1st derivative
	mLeft = B[0];
	bLeft = Ordinates [0];

	int LastCubic = NumberOfCubics - 1;
//	mRight = (3.0 * D [LastCubic] * Right + 2.0 * C [LastCubic]) * Right + B [LastCubic];  // This is an error.  The deriv must be calculated
	// relative to the next to the last knot, not in absolute terms.
	double x = Right - Knots [NumberOfCubics-1];
	mRight = (3.0 * D [LastCubic] * x + 2.0 * C [LastCubic]) * x + B [LastCubic];
	bRight = Ordinates [NumberOfCubics];

	delete[] lambda;
	delete[] mu;
	delete[] h;
	delete[] d;
	delete[] moments;
	delete[] q;
	delete[] u;
	delete[] del;
	delete[] gamma;
	return 0;
}


int CSplineTransform :: InitializeHermite (const double* derivs) {

	int j;

	double* h = new double [NumberOfKnots];

	//
	// Now we set up the cubic coefficients for the Hermite cubic spline.  Algorithm taken from:
	// Wikipedia article entitled "Cubic Hermite Spline", 04/08/2014
	//
	// Commented lines are replaced successively by reorganized code to improve operational efficiency.
	//

	for (j=0; j<NumberOfCubics; j++) {  // from 0 to n-1

		h[j] = Knots[j+1] - Knots[j];
	}
	double hInv;
	double hInv2;
	double p0;
	double m0;
	double p1;
	double m1;
	double T;
	double hh;

	for (j=0; j<NumberOfCubics; j++) {

		// These are coefficient for polynomial between Knots [j] and Knots [j+1]
		hInv = 1.0 / h[j];
		hInv2 = hInv * hInv;
		hh = h [j];
		p0 = Ordinates [j];
		p1 = Ordinates [j+1];
		m0 = derivs [j];
		m1 = derivs [j+1];

		A [j] = p0;
		B [j] = m0;
		T = (m1 - m0) * hh - 2.0 * (p1 - p0 - m0 * hh);
		D [j] = T * hInv * hInv2;
		C [j] = (p1 - p0 - m0 * hh - T) * hInv2;
	}

//	mLeft = (3.0 * D [0] * Left + 2.0 * C [0]) * Left + B [0];  // This is an error...B[0] is the left-most 1st derivative
	mLeft = B[0];
	bLeft = Ordinates [0];

	mRight = derivs [NumberOfCubics];
	bRight = Ordinates [NumberOfCubics];

	delete[] h;
	return 0;
}



//
//  The following not needed yet:
//
//CSplineTransform :: CSplineTransform (const list<double>& coord1, const list<double>& coord2, const CoefficientFactory& factory);


CSplineTransform :: ~CSplineTransform () {

	delete[] Knots;
	delete[] Ordinates;
	delete[] A;
	delete[] B;
	delete[] C;
	delete[] D;
}


double CSplineTransform :: Evaluate (double abscissa) {
	
	// single evaluation, not part of a sequence
	int Interval = SearchForInterval (abscissa);

	if (Interval < 0)
		return DOUBLEMAX;

	LastAbscissa = abscissa;
	
	if (CurrentInterval != Interval)
		CurrentInterval = Interval;

	double y = CalculateCubic (abscissa, Interval);
	LastValue = y;
	return y;
}


double CSplineTransform :: EvaluateWithExtrapolation (double abscissa) {

	if (abscissa < Left)
		return (abscissa - Left) * mLeft + bLeft;

	if (abscissa > Right)
		return (abscissa - Right) * mRight + bRight;

	return Evaluate (abscissa);
}


double CSplineTransform :: EvaluateWithExtrapolation (double abscissa, double& yPrime) {

	// returns value and sets yPrime to first derivative at abscissa

	if (abscissa < Left) {

		yPrime = mLeft;
		return (abscissa - Left) * mLeft + bLeft;
	}

	if (abscissa > Right) {

		yPrime = mRight;
		return (abscissa - Right) * mRight + bRight;
	}

	int Interval = SearchForInterval (abscissa);

	if (Interval < 0) {

		yPrime = 0.0;
		return DOUBLEMAX;
	}

	LastAbscissa = abscissa;
	
	if (CurrentInterval != Interval)
		CurrentInterval = Interval;

	double y = CalculateCubic (abscissa, Interval);
	yPrime = CalulateFirstDerivative (abscissa, Interval);
	LastValue = y;
	return y;
}


double CSplineTransform :: EvaluateFirstDerivative (double abscissa) {

	if (abscissa <= Left)
		return mLeft;

	if (abscissa > Right)
		return mRight;

	int Interval = SearchForInterval (abscissa);

	if (Interval < 0)
		return 0.0;

	double yPrime = CalulateFirstDerivative (abscissa, Interval);
	return yPrime;
}


double CSplineTransform :: EvaluateSequenceStart (double startAbscissa, double spacing) {

	int Interval = SearchForInterval (startAbscissa);

	if (Interval < 0)
		return DOUBLEMAX;
	
	Spacing = spacing;
	LastInSequenceAbscissa = startAbscissa;

	if (CurrentSequenceInterval != Interval)
		CurrentSequenceInterval = Interval;


	double y = CalculateCubic (startAbscissa, Interval);
	LastValueInSequence = y;
	return y;
}


double CSplineTransform :: EvaluateSequenceNext () {
	
	//  evaluates an equally spaced sequence, one at a time

	double nextAbscissa = LastInSequenceAbscissa + Spacing;
	int Interval = SearchForInterval (nextAbscissa, LastInSequenceAbscissa, CurrentSequenceInterval);

	if (Interval < 0)
		return DOUBLEMAX;

	if (CurrentSequenceInterval != Interval)
		CurrentSequenceInterval = Interval;


	double y = CalculateCubic (nextAbscissa, Interval);
	LastInSequenceAbscissa = nextAbscissa;
	LastValueInSequence = y;
	return y;
}


double CSplineTransform :: EvaluateSequenceNext (double abscissaStart, int count) {
	
	//  evaluates an equally spaced sequence, one at a time

	double nextAbscissa = abscissaStart + count * Spacing;
	int Interval = SearchForInterval (nextAbscissa, LastInSequenceAbscissa, CurrentSequenceInterval);

	if (Interval < 0)
		return DOUBLEMAX;

	if (CurrentSequenceInterval != Interval)
		CurrentSequenceInterval = Interval;


	double y = CalculateCubic (nextAbscissa, Interval);
	LastInSequenceAbscissa = nextAbscissa;
	LastValueInSequence = y;
	return y;
}


// evaluates sequence of length 'size' and places in result; returns >= 0 if all OK
int CSplineTransform :: EvaluateFullSequence (double* result, double startAbscissa, double spacing, int size) {

	int k = 1;
	double y = EvaluateSequenceStart (startAbscissa, spacing);
	CurrentSequenceCount = 0;
	TotalSequenceCount = size;
	double* temp = result;

	if (y >= DOUBLEMAX)
		return -1;

	*temp = y; 

	while (k < size) {

		temp++;
		y = EvaluateSequenceNext (startAbscissa, k);

		if (y >= DOUBLEMAX)
			return - 1 - k;

		*temp = y;
		k++;
	}

	return 0;
}


// evaluates sequence of length 'size' based on irregularly spaced abscissas in 'abscissas'; returns >=0 on OK
int CSplineTransform :: EvaluateFullSequence (const double* abscissas, double* result, int size) {

	int k = 0;
	double* temp = (double*)abscissas;
	double* tempResult = result;

	double currentAbscissa;

	int Interval;
	int LastInterval;
	double previousAbscissa;

	while (k < size) {

		currentAbscissa = *temp;

		if (k == 0)
			Interval = SearchForInterval (currentAbscissa);

		else
			Interval = SearchForInterval (currentAbscissa, previousAbscissa, LastInterval);

		if (Interval < 0) {

			*tempResult = DOUBLEMAX;
			return - k - 1;
		}

		*tempResult = CalculateCubic (currentAbscissa, Interval);
		LastInterval = Interval;
		previousAbscissa = currentAbscissa;
		k++;
		temp++;
		tempResult++;
	}

	return 0;
}


double CSplineTransform :: MaxSecondDerivative () const {

	double maxValue = 0.0;
	double temp;
	int i;

	for (i=0; i<NumberOfCubics; i++) {

		temp = fabs (C [i]);

		if (temp > maxValue)
			maxValue = temp;
	}

	int n = NumberOfCubics - 1;
	temp = fabs (C [n] + (Knots [NumberOfCubics] - Knots [n]) * D [n]);

	if (temp > maxValue)
		maxValue = temp;

	if (mIsHermite) {

		for (i=0; i<n; i++) {

			temp = fabs (C [i] + (Knots [i+1] - Knots [i]) * D [i]);

			if (temp > maxValue)
				maxValue = temp;
		}
	}

	return 2.0 * maxValue;
}


double CSplineTransform :: MaxDeltaThirdDerivative () const {

	double maxValue = 0.0;
	double temp;

	for (int i=1; i<NumberOfCubics; i++) {

		temp = fabs (D [i] - D [i-1]);

		if (temp > maxValue)
			maxValue = temp;
	}

	return 6.0 * maxValue;
}


int CSplineTransform :: GetFirstDerivativeAtKnots (double*& firstDerivs) {

	int i;
	firstDerivs = new double [NumberOfKnots];

	for (i=0; i<NumberOfKnots; i++)
		firstDerivs [i] = CalculateFirstDerivativeAtKnot (i);

	return NumberOfKnots;
}


int CSplineTransform :: GetThirdDerivatives (double* thirdDerivs) {

	// returns size of array

	int i;

	for (i=0; i<NumberOfCubics; i++)
		thirdDerivs [i] = 6.0 * D [i];

	return NumberOfCubics;
}



int CSplineTransform :: GetFourthDerivatives (double* fourthDerivs) {
	
	// returns size of array

	int i;
	int size = NumberOfCubics - 1;

	for (i=0; i<size; i++)
		fourthDerivs [i] = 6.0 * (D [i+1] - D [i]) / (Knots [i+1] - Knots [i]);

	fourthDerivs [size] = 0.0;
	return size;
}


int CSplineTransform :: GetFourthDerivativesFromSplineOfFirstDerivs (double* fourthDerivs) {

	double* firstDerivs = new double [NumberOfKnots];
	int i;

	for (i=0; i<NumberOfKnots; i++)
		firstDerivs [i] = CalculateFirstDerivativeAtKnot (i);

	CSplineTransform* CFirstDerivs = new CSplineTransform (Knots, firstDerivs, NumberOfKnots);
	CFirstDerivs->GetThirdDerivatives (fourthDerivs);

	delete CFirstDerivs;
	delete[] firstDerivs;
	return NumberOfKnots;
}



double CSplineTransform :: GetMaxErrors (double* fourthDerivs, double* errors) {
	
	// returns max error in seconds

	int i;

	double temp;
	double tempError;
	double absError;
	double max = 0.0;
	
	for (i=0; i<NumberOfCubics; i++) {

		temp = Knots [i+1] - Knots [i];
		temp = temp * temp;
		errors [i] = tempError = cForError * fourthDerivs [i] * temp * temp;
		absError = fabs (tempError);

		if (absError > max)
			max = absError;
	}

	return max;
}


double CSplineTransform :: GetMaxErrorsInBPs (double* timeErrors, double* bpErrors, const double* characteristicArray) {
	
	// returns max error in bps

	int i;
	double tempError;
	double absError;
	double max = 0.0;
	double maxTime = 0.0;

	if (mIsHermite) {

		for (i=0; i<NumberOfCubics; i++) {

			bpErrors [i] = tempError = timeErrors [i] * ((characteristicArray [i+1] - characteristicArray [i]) / (Ordinates [i+1] - Ordinates [i]));
			absError = fabs (tempError);

			if (absError > max)
				max = absError;
		}

		return max;
	}

	for (i=0; i<NumberOfCubics; i++) {

		tempError = fabs (timeErrors [i]);

		if (tempError > maxTime)
			maxTime = tempError;
	}

	for (i=0; i<NumberOfCubics; i++) {

		bpErrors [i] = tempError = maxTime * ((characteristicArray [i+1] - characteristicArray [i]) / (Ordinates [i+1] - Ordinates [i]));
		absError = fabs (tempError);

		if (absError > max)
			max = absError;
	}

	return max;
}


double CSplineTransform :: OutputHighDerivativesAndErrors (const double* characteristicArray) {

	double* derivs3 = new double [NumberOfCubics];
	double* derivs4 = new double [NumberOfCubics];
	double maxError;
	double maxErrorInBP2;
	double maxErrorInBP;
	double* errors = new double [NumberOfCubics];
	double* bpErrors = new double [NumberOfCubics];
	double* derivs4From1stDerivs = new double [NumberOfCubics];
	double* bpErrorsFrom1stDerivs = new double [NumberOfCubics];
	double* errors2 = new double [NumberOfCubics];

	GetThirdDerivatives (derivs3);
	GetFourthDerivatives (derivs4);
	maxError = GetMaxErrors (derivs4, errors);
	maxErrorInBP = GetMaxErrorsInBPs (errors, bpErrors, characteristicArray);

	GetFourthDerivativesFromSplineOfFirstDerivs (derivs4From1stDerivs);
	GetMaxErrors (derivs4From1stDerivs, errors2);
	maxErrorInBP2 = GetMaxErrorsInBPs (errors2, bpErrorsFrom1stDerivs, characteristicArray);

//	int i;

	cout << "Max Error Original (bps) = " << maxErrorInBP << ".  Max Error From 1st Derivs (bps) = " << maxErrorInBP2 << "\n";
	//cout << "Third derivatives: ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << derivs3 [i];

	//cout << endl;
	//cout << "Fourth derivatives Original: ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << derivs4 [i];

	//cout << endl;
	//cout << "Errors (bbs): ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << bpErrors [i];

	//cout << endl;
	//cout << "Fourth derivatives From 1st Derivs: ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << derivs4From1stDerivs [i];

	//cout << endl;
	//cout << "Errors from 1st Derivs (bbs): ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << bpErrorsFrom1stDerivs [i];

	cout << "\n";
	delete[] derivs3;
	delete[] derivs4;
	delete[] errors;
	delete[] bpErrors;

	delete[] derivs4From1stDerivs;
	delete[] errors2;
	delete[] bpErrorsFrom1stDerivs;
	return maxErrorInBP2;
}


double CSplineTransform :: GetMaximumErrorOfInterpolation (const double* characteristicArray) {

	double maxErrorInBP2;
	double* derivs4From1stDerivs = new double [NumberOfCubics];
	double* bpErrorsFrom1stDerivs = new double [NumberOfCubics];
	double* errors2 = new double [NumberOfCubics];

	GetFourthDerivativesFromSplineOfFirstDerivs (derivs4From1stDerivs);
	GetMaxErrors (derivs4From1stDerivs, errors2);
	maxErrorInBP2 = GetMaxErrorsInBPs (errors2, bpErrorsFrom1stDerivs, characteristicArray);

	//int i;

	//cout << "Max Error Original (bps) = " << maxErrorInBP << ".  Max Error From 1st Derivs (bps) = " << maxErrorInBP2 << endl;
	//cout << "Third derivatives: ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << derivs3 [i];

	//cout << endl;
	//cout << "Fourth derivatives Original: ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << derivs4 [i];

	//cout << endl;
	//cout << "Errors (bbs): ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << bpErrors [i];

	//cout << endl;
	//cout << "Fourth derivatives From 1st Derivs: ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << derivs4From1stDerivs [i];

	//cout << endl;
	//cout << "Errors from 1st Derivs (bbs): ";

	//for (i=0; i<NumberOfCubics; i++)
	//	cout << " " << bpErrorsFrom1stDerivs [i];

	//cout << endl;
	//delete[] derivs3;
	//delete[] derivs4;
	//delete[] errors;
	//delete[] bpErrors;

	delete[] derivs4From1stDerivs;
	delete[] errors2;
	delete[] bpErrorsFrom1stDerivs;
	return maxErrorInBP2;
}


double* CSplineTransform :: GetBPAsAFunctionOfTime (double* ilsTimes, double* ilsBP, int numberILSPeaks, int numberOfTimePoints) {

	CSplineTransform* timeToBP = new CSplineTransform (ilsTimes, ilsBP, numberILSPeaks);

	if (!timeToBP->IsValid ())
		return NULL;

	double* array = new double [numberOfTimePoints];
	double startSequence = ceil (ilsTimes [0]);
	double endSequence = floor (ilsTimes [numberILSPeaks - 1]);
	int k = (int) startSequence;
	int kk = (int) endSequence;
	array [k] = timeToBP->EvaluateSequenceStart (startSequence, 1.0);
	k++;
	double temp;

	// First while loop evaluates spline by interpolation between first and last knot

	while (k <= kk) {

		array [k] = timeToBP->EvaluateSequenceNext ();
		k++;
	}

	// second while loop evaluates spline by extrapolation to the right of the spline

	while (k < numberOfTimePoints) {

		array [k] = timeToBP->EvaluateWithExtrapolation ((double) k);
		k++;
	}

	// third while loop evaluates spline by extrapolation to the left of the spline, up to a point.

	k = (int) startSequence - 1;
	double lowBP = ilsBP [0];
	double testBP = lowBP - 30.0;
	double halfBP = 0.5 * lowBP;

	if ((testBP <= 0.0) || (testBP > halfBP))
		testBP = halfBP;

	while (k >= 0) {

		temp = timeToBP->EvaluateWithExtrapolation ((double) k);

		if (temp < testBP)
			break;

		array [k] = temp;
		k--;
	}

	if (k == 0) {

		array [0] = temp;
	}

	else if (k > 0) {

		double slope = temp / (double)k;

		while (k >= 0) {

			array [k] = (double)k * slope;
			k--;
		}
	}

	delete timeToBP;
	return array;
}



int CSplineTransform :: SearchForInterval (double abscissa) {

	//  Use binary search technique - start in middle interval

	int Low = 0;
	int High = NumberOfKnots + 1;
	int Middle;

	if (abscissa > Right)
		return -2;

	if (abscissa < Left)
		return -1;

	while (High - Low > 1) {

		Middle = (Low + High) / 2;

		if (abscissa > Knots [Middle])
			Low = Middle;

		else
			High = Middle;
	}

	return Low;
}


int CSplineTransform :: SearchForInterval (double abscissa, double start, int startInterval) {

	int Interval = startInterval;
	int NextInterval;

	if (abscissa > Right)
		return -2;

	if (abscissa < Left)
		return -1;

	while (Interval < NumberOfCubics) {

		NextInterval = Interval + 1;
		
		if (abscissa <= Knots [NextInterval])
			return Interval;

		Interval++;
	}

	return -2;
}


double CSplineTransform :: CalculateCubic (double abscissa, int interval) {

	double x = abscissa - Knots[interval];
	return (((D[interval] * x + C[interval]) * x) + B[interval]) * x + A[interval];
}


double CSplineTransform :: CalculateFirstDerivativeAtKnot (int knot) {

	if (knot < NumberOfKnots - 1)
		return B[knot];

	double x = Knots [knot] - Knots [knot - 1];
	int i = knot - 1;
	return (3.0 * D[i] * x + 2.0 * C[i]) * x + B[i];
}


double CSplineTransform :: CalulateFirstDerivative (double abscissa, int interval) {

	double x = abscissa - Knots[interval];
	return (3.0 * D[interval] * x + 2.0 * C[interval]) * x + B[interval];
}


