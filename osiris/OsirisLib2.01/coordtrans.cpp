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

#include "rgdefs.h"
#include "coordtrans.h"
#include <stdlib.h>
#include <math.h>

CoordinateTransform :: CoordinateTransform (const list<double>& coord1, const list<double>& coord2) : Spacing (0.0),
CurrentSequenceCount (-1),
TotalSequenceCount (-1),
ErrorFlag (0) {

	list<double>::const_iterator c1Iterator = coord1.begin ();
	Left = LastAbscissa = LastInSequenceAbscissa = *c1Iterator;
	c1Iterator = coord1.end ();
	c1Iterator--;
	Right = *c1Iterator;
	list<double>::const_iterator c2Iterator = coord2.begin ();
	LastValue = LastValueInSequence = *c2Iterator;

	if (coord2.size () != coord1.size ())
		ErrorFlag = 1;
}


CoordinateTransform :: CoordinateTransform (const double* coord1, const double* coord2, int size) : Spacing (0.0),
CurrentSequenceCount (-1),
TotalSequenceCount (-1),
ErrorFlag (0) {

	int size1 = size - 1;
	Left = LastAbscissa = LastInSequenceAbscissa = coord1 [0];
	Right = coord1 [size1];
	LastValue = LastValueInSequence = coord2 [0];
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
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL) {

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
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL) {

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
CurrentInterval (-1), CurrentSequenceInterval (-1), Knots (NULL), Ordinates (NULL), A (NULL), B (NULL), C (NULL), D (NULL) {

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

	for (int i=0; i<NumberOfCubics; i++) {

		temp = fabs (C [i]);

		if (temp > maxValue)
			maxValue = temp;
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
