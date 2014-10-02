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
*  FileName: coordtrans.h
*  Author:   Robert Goor
*
*/
//
// Header for class CoordinateTransform and subclasses that perform coordinate transforms using different algorithms.  Allows evaluation on a one-time
// basis, as one of a sequence of calls at equal spacing, or, for maximum efficiency when multiple calls are required at equal spacing, allows evaluation
// of an array of transform values based on an initial value, a spacing and either a number of evaluations or a final value.
//

#ifndef _COORDTRANS_H_
#define _COORDTRANS_H_

#include <list>


using namespace std;


struct SupplementaryData {

	const double* abscissas;
	const double* ordinates;
	int size;
	double proximityMultiple;
};


class CoordinateTransform {

public:
	CoordinateTransform (const list<double>& coord1, const list<double>& coord2);
	CoordinateTransform (const double* coord1, const double* coord2, int size);
//	CoordinateTransform (const list<double>& coord1, const list<double>& coord2, const CoefficientFactory& factory);  // not needed yet
	virtual ~CoordinateTransform ();

	bool IsValid () const { return ErrorFlag == 0; }

	virtual double Evaluate (double abscissa);  // single evaluation, not part of a sequence
	virtual double EvaluateWithExtrapolation (double abscissa);
	virtual double EvaluateWithExtrapolation (double abscissa, double& yPrime);
	virtual double EvaluateFirstDerivative (double abscissa);  // single evaluation, not part of a sequence

	virtual double EvaluateSequenceStart (double startAbscissa, double spacing);
	virtual double EvaluateSequenceNext ();  //  evaluates an equally spaced sequence, one at a time
	virtual double EvaluateSequenceNext (double abscissaStart, int count);  // evaluates next in equally spaced sequence

	// evaluates sequence of length 'size' and places in result; returns >= 0 if all OK
	virtual int EvaluateFullSequence (double* result, double startAbscissa, double spacing, int size);

	// evaluates sequence of length 'size' based on irregularly spaced, ORDERED abscissas in 'abscissas'; returns >=0 on OK
	virtual int EvaluateFullSequence (const double* abscissas, double* result, int size);
	virtual double MaxSecondDerivative () const;
	virtual double MaxDeltaThirdDerivative () const;
	virtual int GetFirstDerivativeAtKnots (double*& firstDerivs) { firstDerivs = NULL; return 0; }

	double GetLastValue () const;
	double GetLastInSequenceValue () const;

	double GetLeftAbscissa () const { return Left; }
	double GetRightAbscissa () const { return Right; }

protected:
	double LastInSequenceAbscissa;
	double Spacing;
	double Left;
	double Right;
	double LastValueInSequence;
	double LastAbscissa;
	double LastValue;
	int CurrentSequenceCount;
	int TotalSequenceCount;
	int ErrorFlag;
};


class CSplineTransform : public CoordinateTransform {

public:
	CSplineTransform (const list<double>& coord1, const list<double>& coord2);
	CSplineTransform (const double* coord1, const double* coord2, int size);
	CSplineTransform (const double* coord1, const double* coord2, int size, const SupplementaryData& ExtraData);
	CSplineTransform (const double* coord1, const double* coord2, const double* derivs, int size, bool isHermite);	// this is primarily for Hermite cubic splines
	CSplineTransform (const double* coord1, const double* coord2, int size, bool isHermite);	// this is primarily for Hermite cubic splines, but using finite differences for derivatives
	CSplineTransform (const list<double>& coord1, const list<double>& coord2, bool isHermite);	// this is primarily for Hermite cubic splines, but using finite differences for derivatives
//	CSplineTransform (const list<double>& coord1, const list<double>& coord2, const CoefficientFactory& factory);  // not needed yet
	virtual ~CSplineTransform ();

	virtual double Evaluate (double abscissa);  // single evaluation, not part of a sequence
	virtual double EvaluateWithExtrapolation (double abscissa);
	virtual double EvaluateWithExtrapolation (double abscissa, double& yPrime);
	virtual double EvaluateFirstDerivative (double abscissa);  // single evaluation, not part of a sequence; automatically extrapolates as constant outside knot boundary

	virtual double EvaluateSequenceStart (double startAbscissa, double spacing);
	virtual double EvaluateSequenceNext ();  //  evaluates an equally spaced sequence, one at a time
	virtual double EvaluateSequenceNext (double abscissaStart, int count);  // evaluates next in equally spaced sequence

	// evaluates sequence of length 'size' and places in result; returns >= 0 if all OK
	virtual int EvaluateFullSequence (double* result, double startAbscissa, double spacing, int size);

	// evaluates sequence of length 'size' based on irregularly spaced, ORDERED abscissas in 'abscissas'; returns >=0 on OK
	virtual int EvaluateFullSequence (const double* abscissas, double* result, int size);

	virtual double MaxSecondDerivative () const;
	virtual double MaxDeltaThirdDerivative () const;
	virtual int GetFirstDerivativeAtKnots (double*& firstDerivs);

	int GetThirdDerivatives (double* thirdDerivs);	// returns size of array
	int GetFourthDerivatives (double* fourthDerivs);	// returns size of array
	int GetFourthDerivativesFromSplineOfFirstDerivs (double* fourthDerivs);
	double GetMaxErrors (double* fourthDerivs, double* errors);	// returns max error in seconds
	double GetMaxErrorsInBPs (double* timeErrors, double* bpErrors, const double* characteristicArray);	// returns max error in bps
	int OutputHighDerivativesAndErrors (const double* characteristicArray);
	double GetMaximumErrorOfInterpolation (const double* characteristicArray);

private:
	int NumberOfKnots;
	int CurrentInterval;
	int CurrentSequenceInterval;
	int NumberOfCubics;

	double* Knots;
	double* Ordinates;

	// cubic coefficients:  jth coefficients for interval between Knots[j] and Knots[j+1]
	double* A;
	double* B;
	double* C;
	double* D;

	double mLeft;
	double bLeft;
	double mRight;
	double bRight;

	int Initialize ();
	int InitializeHermite (const double* derivs);
	int SearchForInterval (double abscissa);
	int SearchForInterval (double abscissa, double start, int startInterval);

	bool mIsHermite;

	double CalculateCubic (double abscissa, int interval);
	double CalculateFirstDerivativeAtKnot (int knot);
	double CalulateFirstDerivative (double abscissa, int interval);
};


//
//  The following is commented out because it is not thought to be needed as of 03/22/07.  If
// that changes, it can be restored.
//

/*
class CoefficientFactory {

public:
	CoefficientFactory (const list<double>& coords);
	virtual ~CoefficientFactory ();

	virtual int GetMoments (const double* targetCoordData, double*& moments) const;
	int GetNumberOfCoordinates () const { return NumberOfCoordinates; }

protected:
	int NumberOfCoordinates;
};


class CSplineCoefficientFactory : public CoefficientFactory {

public:
	CSplineCoefficientFactory (const list<double>& coords);
	virtual ~CSplineCoefficientFactory ();

	virtual int GetMoments (const double* targetCoordData, double*& moments) const;

protected:
	double** FactoryMatrix;
};
*/

#endif  /*  _COORDTRANS_H_  */
