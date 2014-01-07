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
*  FileName: GaussianRandomVariable.h
*  Author:   Robert Goor
*
*/
//
//  class GaussianRandomVariable, which uses the system rand () uniform random variable function call and converts it to a Gaussian
// random variable using the (approximate) inverse cumulative Gaussian distribution.
//

#ifndef _GAUSSIANRANDOMVARIABLE_H_
#define _GAUSSIANRANDOMVARIABLE_H_

#include "rgdefs.h"

class GaussianRandomVariable;

double* GenerateDiscreteNoise (int nSamples, GaussianRandomVariable* grv);


class GaussianRandomVariable {

public:
	GaussianRandomVariable (double sigma);
	GaussianRandomVariable (double mean, double sigma);
	GaussianRandomVariable (unsigned int seed, double sigma);
	GaussianRandomVariable (unsigned int seed, double mean, double sigma);
	~GaussianRandomVariable ();

	double GetRandomNumber ();
	double UniformZeroOne ();

	double GetMu () const { return Mu; }

	static void SetAlpha (double newAlpha) { alpha = newAlpha; }
	static double GetAlpha () { return alpha; }
	static double* GenerateDiscreteNoise (int nSamples, GaussianRandomVariable* grv);

protected:
	double Denominator;
	Boolean Reserve;
	double AlreadyComputed;
	double Sigma;
	double Mu;

	static double alpha;
};


#endif  /*  _GAUSSIANRANDOMVARIABLE_H_  */


