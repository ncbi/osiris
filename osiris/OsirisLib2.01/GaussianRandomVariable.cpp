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
*  FileName: GaussianRandomVariable.cpp
*  Author:   Robert Goor
*
*/
//
//  class GaussianRandomVariable, which uses the system rand () uniform random variable function call and converts it to a Gaussian
// random variable using the (approximate) inverse cumulative Gaussian distribution.
//

#include "GaussianRandomVariable.h"
#include "coordtrans.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

double GaussianRandomVariable::alpha = 0.5;


double* GenerateDiscreteNoise (int nSamples, GaussianRandomVariable* grv) {

	double alpha = 0.975;
	double* NoiseArray = new double [nSamples];
	NoiseArray [0] = grv->GetRandomNumber ();

	for (int j=1; j<nSamples; j++) {

		NoiseArray [j] = alpha * NoiseArray [j-1] + grv->GetRandomNumber ();
	}
	
	return NoiseArray;
}



GaussianRandomVariable :: GaussianRandomVariable (double sigma) : Mu (0.0) {

	Denominator = (double) 0x8001;
	Reserve = FALSE;
	AlreadyComputed = 0.0;
	Sigma = sigma;
}


GaussianRandomVariable :: GaussianRandomVariable (double mean, double sigma) : Sigma (sigma), Mu (mean) {

	Denominator = (double) 0x8001;
	Reserve = FALSE;
	AlreadyComputed = 0.0;
}



GaussianRandomVariable :: GaussianRandomVariable (unsigned int seed, double sigma) : Mu (0.0) {

	srand (seed);
	Denominator = (double) 0x8001;
	Reserve = FALSE;
	AlreadyComputed = 0.0;
	Sigma = sigma;
}


GaussianRandomVariable :: GaussianRandomVariable (unsigned int seed, double mean, double sigma) : 
Sigma (sigma), Mu (mean) {

	srand (seed);
	Denominator = (double) 0x8001;
	Reserve = FALSE;
	AlreadyComputed = 0.0;
}



GaussianRandomVariable :: ~GaussianRandomVariable () {

}


double GaussianRandomVariable :: GetRandomNumber () {

	double x1, x2, w;

	if (Reserve) {

		Reserve = FALSE;
		return AlreadyComputed;
	}

	w = 2.0;

	while (w >= 1.0) {

		x1 = 2.0 * UniformZeroOne () - 1.0;
		x2 = 2.0 * UniformZeroOne () - 1.0;
		w = x1 * x1 + x2 * x2;
	}

	if (w > 0.0)
		w = sqrt ((-2.0 * log (w)) / w);

	else
		w = 0.0;

	AlreadyComputed = w * x2 * Sigma + Mu;
	Reserve = TRUE;
	return w * x1 * Sigma + Mu;
}


double GaussianRandomVariable :: UniformZeroOne () {

	return (double)(rand () + 1) / Denominator;
}


double* GaussianRandomVariable :: GenerateDiscreteNoise (int nSamples, GaussianRandomVariable* grv) {

	double* NoiseArray = new double [nSamples];
	NoiseArray [0] = grv->GetRandomNumber ();
	double mu = grv->GetMu ();
	double alpha1 = 1.0 - alpha;

	for (int j=1; j<nSamples; j++) {

		NoiseArray [j] = alpha * NoiseArray [j-1] + grv->GetRandomNumber ();
	}
	
	return NoiseArray;
}

