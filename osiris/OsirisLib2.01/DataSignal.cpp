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
*  FileName: DataSignal.cpp
*  Author:   Robert Goor
*
*/
//
//   Source file for class DataSignal and subclasses.  These classes represent the measurement data and transformed 
// measurement data.  Methods allow simple transforms and basic searches for signatures
//



#include "DataSignal.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgdefs.h"
#include "TracePrequalification.h"
#include "DataInterval.h"
#include "RGTextOutput.h"
#include "RGLogBook.h"
#include "rgstring.h"
#include "OsirisMsg.h"
#include "rgindexedlabel.h"
#include "OutputLevelManager.h"
#include "SpecialLinearRegression.h"
#include "coordtrans.h"
#include "Genetics.h"
#include "SmartMessage.h"
#include "STRSmartNotices.h"
#include "OsirisPosix.h"
#include <cmath>
#include <limits>

using namespace::std;

double PeakInfoForClusters::HeightFactor = 1.0;
Boolean DataSignal :: DebugFlag = FALSE;
double DataSignal :: SignalSpacing = 1.0;
double DataSignal :: minHeight = 150.0;
double DataSignal :: maxHeight = -1.0;
unsigned long DataSignal :: signalID = 0;
bool* DataSignal::InitialMatrix = NULL;
bool DataSignal::ConsiderAllOLAllelesAccepted = false;
int DataSignal::NumberOfChannels = 0;
int DataSignal::NumberOfIntervalsForConcaveDownAlgorithm = 3;

const double two_thirds = 2.0 / 3.0;
const double sqRoot_two_thirds = sqrt (two_thirds);

double SampledData::PeakFractionForFlatCurveTest = 0.25;
double SampledData::PeakLevelForFlatCurveTest = 60.0;
bool SampledData::IgnoreNoiseAnalysisAboveDetectionInSmoothing = false;
double SampledData::DetectionRFU = 1;

double ParametricCurve::FitTolerance = 0.999995;
double ParametricCurve::TriggerForArtifactTest = 0.992;
double ParametricCurve::MinimumFitThreshold = 0.99;
double ParametricCurve::SigmaForSignature = 2.0;
double ParametricCurve::AbsoluteMinimumFit = 0.85;

double Gaussian :: SigmaWidth = 10.0;
double Gaussian :: SampleSigmaWidth = 3.5;
int Gaussian :: NumberOfSteps = 50;

int NormalizedGaussian::MaximumIterations = 5;
double NormalizedGaussian::SigmaTolerance = 1.5e-4;

double DoubleGaussian :: DefaultSigmaRatio = 4.5;
double DoubleGaussian :: DefaultSqrtSigmaRatio = sqrt (DoubleGaussian::DefaultSigmaRatio);

int SuperGaussian :: NumberOfSteps = 50;
double SuperGaussian :: SigmaWidth = 2.1;

double SuperGaussian :: SuperNorms [8] = {1.785928938266120E+00,
1.812804952863950E+00,
1.836337486880030E+00,
1.855438667260080E+00,
1.870875125769150E+00,
1.883485399547420E+00,
1.893930701187330E+00,
1.902701574449640E+00,
};

double SuperGaussian :: SuperSigma2s [8] = {3.731545017189410E-01,
3.379890974138610E-01,
3.243831287918070E-01,
3.184249421589860E-01,
3.157440216174210E-01,
3.146342659288960E-01,
3.143302312270560E-01,
3.144549304168940E-01,
};

double SuperGaussian :: RootTwos [8] = {1.259921049894870E+00,
1.189207115002720E+00,
1.148698354997040E+00,
1.122462048309370E+00,
1.104089513673810E+00,
1.090507732665260E+00,
1.080059738892310E+00,
1.071773462536290E+00,
};

int SuperGaussian::BlobDegree = 6;

int MaxIndex (double* array, int N);
int MinIndex (double* array, int N);


double BrentStep (double* abscissas, double* ordinates) {

	double delLeft = abscissas[1] - abscissas[0];
	double delRight = abscissas[1] - abscissas[2];
	double delLeft2 = delLeft * delLeft;
	double delRight2 = delRight * delRight;
	double delFLeft = ordinates[1] - ordinates[0];
	double delFRight = ordinates[1] - ordinates[2];
	double Numerator = (delLeft2 * delFRight - delRight2 * delFLeft);
	double Denominator = (delLeft * delFRight - delRight * delFLeft);
	double value;
	int MaxValueIndex;
	int MaxAbscissa;
	int MinAbscissa;

	if (fabs (Numerator) > fabs (Denominator) * DOUBLEMAX) {

		MaxValueIndex = MaxIndex (ordinates, 3);
		MaxAbscissa = MaxIndex (abscissas, 3);
		MinAbscissa = MinIndex (abscissas, 3);

		if ((MaxValueIndex == MaxAbscissa) || (MaxValueIndex == MinAbscissa))
			return abscissas [MaxValueIndex];

		value = 0.61803 * abscissas [MaxValueIndex] + 0.38197 * abscissas [MinAbscissa];
	}

	else
		value = abscissas[1] - 0.5 * Numerator / Denominator; // test denominator first!

	return value;
}

int MaxIndex (double* array, int N) {

	int MaxInd = 0;
	double MaxValue = array [0];

	for (int i=1; i<N; i++) {

		if (array [i] > MaxValue) {

			MaxInd = i;
			MaxValue = array [i];
		}
	}

	return MaxInd;
}

int MinIndex (double* array, int N) {

	int MinInd = 0;
	double MinValue = array [0];

	for (int i=1; i<N; i++) {

		if (array [i] < MinValue) {

			MinInd = i;
			MinValue = array [i];
		}
	}

	return MinInd;
}



Boolean QuadraticRegression (const double* means, const double* sigmas, int N, double* parameters) {

	// parameters [0] is coefficient of quadratic term
	// parameters [1] is coefficient of linear term
	// parameters [2] is constant term
	
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

	for (int i=0; i<N; i++) {

		temp = means [i];
		temps = sigmas [i];
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

	double NU2minusU12 = N * U2 - U12;
	double NU4minusU22 = N * U4 - U22;
	double U2U4minusU32 = U2 * U4 - U32;
	double NU3minusU1U2 = N * U3 - U1U2;
	double U1U4minusU2U3 = U1 * U4 - U2U3;
	double U1U3minusU22 = U1 * U3 - U22;

	double detA = N * U2 * U4 + 2.0 * U1 * U2 * U3 - (U22 * U2 + N * U32 + U12 * U4);

	if (detA == 0.0)
		return FALSE;

	parameters [0] = (NU2minusU12 * V2 - NU3minusU1U2 * V1 + U1U3minusU22 * V0) / detA;
	parameters [1] = (- NU3minusU1U2 * V2 + NU4minusU22 * V1 - U1U4minusU2U3 * V0) / detA;
	parameters [2] = (U1U3minusU22 * V2 - U1U4minusU2U3 * V1 + U2U4minusU32 * V0) / detA;
	return TRUE;
}


Boolean LinearRegression (const double* x, const double* y, int N, double* parameters) {

	// parameters [0] is linear coefficient
	// parameters [1] is constant term

	double xbar = 0.0;
	double ybar = 0.0;
	double r = 0.0;
	double var = 0.0;
	double temp;
  int i;
	for (i=0; i<N; i++) {

		xbar += x [i];
		ybar += y [i];
	}

	xbar = xbar / N;
	ybar = ybar / N;

	for (i=0; i<N; i++) {

		temp = x [i] - xbar;
		r += temp * (y [i] - ybar);
		var += temp * temp;
	}

	if (var == 0.0)
		return FALSE;

	parameters [0] = r / var;
	parameters [1] = ybar - parameters [0] * xbar;
	return TRUE;
}


int LessOrder (double a, double b, double c) {

	double minimum = c;
	int status = 4;

	if (a < minimum) {

		minimum = a;
		status = 1;
	}

	if (b < minimum) {

		minimum = b;
		status = 2;
	}

	switch (status) {

		case 1:
			
			if (b == minimum)
				return 3;

			return 1;

		case 2:
			return 2;

		case 4:

			if (a == minimum)
				status += 1;

			if (b == minimum)
				status += 2;
	}

	return status;
}


Boolean IsOutsideInterval (double testValue, double val1, double val2, double val3) {

	if ((testValue < val1) && (testValue < val2) && (testValue < val3))
		return TRUE;

	if ((testValue > val1) && (testValue > val2) && (testValue > val3))
		return TRUE;

	return FALSE;
}


PERSISTENT_DEFINITION (PeakInfoForClusters, _PEAKINFOFORCLUSTERS_, "PeakInfoForClusters")
ABSTRACT_DEFINITION (DataSignal)
ABSTRACT_DEFINITION (ParametricCurve)
PERSISTENT_DEFINITION (SampledData, _SAMPLEDDATA_, "SampledData")

//PERSISTENT_DEFINITION (ParametricCurve, _PARAMETRICCURVE_, "ParametricCurve")

PERSISTENT_DEFINITION (Gaussian, _GAUSSIAN_, "Gaussian")
PERSISTENT_DEFINITION (NormalizedGaussian, _NORMALIZEDGAUSSIAN_, "NormalizedGaussian")
PERSISTENT_DEFINITION (DoubleGaussian, _DOUBLEGAUSSIAN_, "DoubleGaussian")
PERSISTENT_DEFINITION (SuperGaussian, _SUPERGAUSSIAN_, "SuperGaussian")
PERSISTENT_DEFINITION (NormalizedSuperGaussian, _NORMALIZEDSUPERGAUSSIAN_, "NormalizedSuperGaussian")
PERSISTENT_DEFINITION (CompositeCurve, _COMPOSITECURVE_, "CompositeCurve")
PERSISTENT_DEFINITION (DualDoubleGaussian, _DUALDOUBLEGAUSSIAN_, "DualDoubleGaussian")
PERSISTENT_DEFINITION (CraterSignal, _CRATERSIGNAL_, "CraterSignal")
PERSISTENT_DEFINITION (SimpleSigmoidSignal, _SIMPLESIGMOIDSIGNAL_, "SimpleSigmoidSignal")
PERSISTENT_DEFINITION (NegativeSignal, _NEGATIVESIGNAL_, "NegativeSignal")
PERSISTENT_DEFINITION (NoisyPeak, _NOISYPEAK_, "NoisyPeak")
PERSISTENT_DEFINITION (SpikeSignal, _SPIKESIGNAL_, "SpikeSignal")


SampleDataInfo :: SampleDataInfo (const double* segL, const double* segC, const double* segR, int indL, int indC, int indR, int N, 
double spacing, double abscissaLeft) :
DataLeft (segL), DataCenter (segC), DataRight (segR), IndexLeft (indL), IndexCenter (indC), IndexRight (indR), NumberOfSamples (N),
Spacing (spacing), AbscissaLeft (abscissaLeft) {}


SampleDataInfo :: SampleDataInfo (const DataSignal& ds, int indL, int indC, int indR, int N) :
IndexLeft (indL), IndexCenter (indC), IndexRight (indR), NumberOfSamples (N) {

	const double* data = ds.GetData ();
	Spacing = DataSignal::GetSampleSpacing ();
	int N1 = ds.GetNumberOfSamples () - 1;
	int NLeft;
	int NRight;
	int LCenter, RCenter;

	if ((indL < 0) || (indR > N1)) {

		if (indR > N1)
			NRight = N1;

		else
			NRight = indR;

		if (indL < 0)
			NLeft = 0;

		else
			NLeft = indL;

		LCenter = indC - NLeft;
		RCenter = NRight - indC;

		if (LCenter <= RCenter) {

			IndexLeft = NLeft;
			IndexRight = indC + LCenter;
		}

		else {

			IndexRight = NRight;
			IndexLeft = indC - RCenter;
		}
	}

	if (data != NULL) {

		DataLeft = data + IndexLeft;
		DataCenter = data + IndexCenter;
		DataRight = data + IndexRight;
	}

	else {

		DataLeft = NULL;
		DataCenter = NULL;
		DataRight = NULL;
	}

	AbscissaLeft = ds.LeftEndPoint ();
}


SampleDataInfo :: ~SampleDataInfo () {

}


double SampleDataInfo :: GetMaxInInterval (int left, int right) {

	const double* next;
	double max = 0.0;
	int leftDiff = left - IndexLeft;

	if (leftDiff < 0)
		leftDiff = 0;

	int rightDiff = IndexRight - right;

	if (rightDiff < 0)
		rightDiff = 0;

	const double* start = DataLeft + leftDiff;
	const double* end = DataRight - rightDiff;

	for (next=start; next<=end; next++) {

		if (*next > max)
			max = *next;
	}

	return max;
}


CompoundSignalInfo :: CompoundSignalInfo (DataSignal* ds1, DataSignal* ds2) : first (ds1), 
last (ds2), xmlPeakInfoWritten (false), xmlArtifactInfoWritten (false),	nameStringBuilt (false) {

	mean = (int) floor (0.5 * (ds1->GetMean () + ds2->GetMean ()) + 0.5);
	double peak1 = ds1->Peak ();
	double peak2 = ds2->Peak ();
	double max;

	if (peak1 > peak2)
		max = peak1;

	else
		max = peak2;

	height = (int) floor (1.2 * max + 0.5);
	bioID = (int) floor (ds2->GetBioID () + 0.5);
}



InterchannelLinkage :: InterchannelLinkage (int nChannels) : mPrimarySignal (NULL), mNChannels (nChannels) {

	int channels1 = nChannels + 1;
	mDirectedGraph = new int* [channels1];
	
	mRatios = new double* [channels1];
	mIsNegativePullup = new bool* [channels1];

	mCanBePrimary = new bool [channels1];
	mIsAboveNoiseThreshold = new bool [channels1];
	mIsNegativePeak = new bool [channels1];
	mPeakHeights = new double [channels1];
	mSecondaryIterator = new RGDListIterator (mSecondarySignals);

	int i;
	int j;
	mDirectedGraph [0] = NULL;
	mRatios [0] = NULL;
	mIsNegativePullup [0] = NULL;
	mPeakHeights [0] = 0.0;

	mCanBePrimary [0] = false;
	mIsAboveNoiseThreshold [0] = false;

	for (i=1; i<=nChannels; i++) {

		mDirectedGraph [i] = new int [channels1];
		mRatios [i] = new double [channels1];
		mIsNegativePullup [i] = new bool [channels1];

		mCanBePrimary [i] = false;
		mIsAboveNoiseThreshold [i] = false;
		mIsNegativePeak [i] = false;
		mPeakHeights [i] = 0.0;
	}

	for (i=1; i<=nChannels; i++) {

		for (j=0; j<=nChannels; j++) {

			mDirectedGraph [i][j] = 0;
			mRatios [i][j] = 0.0;
			mIsNegativePullup [i][j] = false;
		}
	}
}


InterchannelLinkage :: ~InterchannelLinkage () {

	mSecondarySignals.Clear ();
	delete mSecondaryIterator;

	int i;

	for (i=1; i<=mNChannels; i++) {

		delete[] mDirectedGraph [i];
		delete[] mRatios [i];
		delete[] mIsNegativePullup [i];
	}

	delete[] mDirectedGraph;
	delete[] mRatios;
	delete[] mIsNegativePullup;

	delete[] mCanBePrimary;
	delete[] mIsAboveNoiseThreshold;
	delete[] mPeakHeights;
	delete[] mIsNegativePeak;
}


bool InterchannelLinkage :: AddDataSignal (DataSignal* newSignal) {

	if (mSecondarySignals.InsertWithNoReferenceDuplication (newSignal) == NULL)
		return false;

	return true;
}


bool InterchannelLinkage :: RemoveDataSignal (DataSignal* oldSignal, Notice* primaryTarget, Notice* primaryReplace, Notice* secondaryTarget, Notice* secondaryReplace) {

	// Add notice objects for test and replacement on removal

	bool wasPrimary = false;

	if (mPrimarySignal == oldSignal) {

		mPrimarySignal->ReplaceNotice (primaryTarget, primaryReplace);
		mPrimarySignal = NULL;
		wasPrimary = true;
	}
	
	if (mSecondarySignals.RemoveReference (oldSignal) == NULL)
		return false;

	else if (!wasPrimary) {

		oldSignal->ReplaceNotice (secondaryTarget, secondaryReplace);

		if (mPrimarySignal != NULL)
			mPrimarySignal->RemovePrimaryCrossChannelSignalLink (primaryTarget, primaryReplace, oldSignal);
	}

	return true;
}


bool InterchannelLinkage :: RecalculatePrimarySignal (Notice* primaryTarget, Notice* primaryReplace) {

	// Add notice objects for test and replacement on removal

	if (mSecondarySignals.Entries () <= 1)
		return false;

	if (mPrimarySignal != NULL)
		return true;
	
	DataSignal* nextSignal;
	RGDListIterator it (mSecondarySignals);
	DataSignal* maxSignal = NULL;
	double maxPeak = 0.0;
	double thisPeak;

	while (nextSignal = (DataSignal*) it ()) {

		thisPeak = nextSignal->Peak ();

		if (thisPeak > maxPeak) {

			maxPeak = thisPeak;
			maxSignal = nextSignal;
		}
	}

	if (maxSignal == NULL)
		return false;

	mPrimarySignal = maxSignal;
	mPrimarySignal->ReplaceNotice (primaryTarget, primaryReplace);

	it.Reset ();
	mPrimarySignal->RemoveAllCrossChannelSignalLinks ();

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal != mPrimarySignal) {

			nextSignal->SetPrimaryCrossChannelSignalLink (mPrimarySignal);
			mPrimarySignal->AddCrossChannelSignalLink (nextSignal);
		}
	}

	return true;
}


bool InterchannelLinkage :: RemoveAll (Notice* primaryTarget, Notice* primaryReplace, Notice* secondaryTarget, Notice* secondaryReplace) {

	// Remove meessages

	DataSignal* nextSignal;

	if (mPrimarySignal != NULL)
		mPrimarySignal->RemoveCrossChannelSignalLink (primaryTarget, secondaryTarget, primaryReplace, secondaryReplace);

	else {

		while (nextSignal = (DataSignal*) mSecondarySignals.GetFirst ()) {

			nextSignal->RemoveSecondaryCrossChannelSignalLink (secondaryTarget, secondaryReplace);
			// These signals are all stored on other lists, so can't delete them here.
		}

		mPrimarySignal = NULL;
	}

	return true;
}


bool InterchannelLinkage :: IsEmpty () {

	if (mSecondarySignals.IsEmpty ())
		return true;

	return false;
}


DataSignal* InterchannelLinkage :: GetPrimarySignal () {

	return mPrimarySignal;
}


bool InterchannelLinkage :: PrimarySignalHasChannel (int n) const {

	if (mPrimarySignal == NULL)
		return false;

	return (mPrimarySignal->GetChannel () == n);
}


int InterchannelLinkage :: NumberOfSecondarySignals () const {

	return mSecondarySignals.Entries ();
}



int InterchannelLinkage :: NumberOfSecondarySignalsAbovePrimaryThreshold (double threshold) {

	RGDListIterator it (mSecondarySignals);
	DataSignal* nextSignal;
	int count = 0;

	while (nextSignal = (DataSignal*) it ()) {

		if ((!nextSignal->IsNegativePeak ()) && (nextSignal->Peak () >= threshold))
			count++;
	}

	return count;
}



int InterchannelLinkage :: MapOutSignalProperties (double noiseMultiple, double primaryThreshold, double* channelNoiseLevels) {

	//  Assumes that mPrimarySignal has highest peak height of all peaks, if there is a primary

	int i;
	int j;
	RGDListIterator it (mSecondarySignals);
	DataSignal* nextSignal;
	double nextPeakHeight;
	int nextChannel;

	if (mPrimarySignal == NULL)
		return -1;

	if (mSecondarySignals.Entries () <= 1)
		return -1;

	int primaryChannel = mPrimarySignal->GetChannel ();

	double primaryHeight = mPeakHeights [primaryChannel] = mPrimarySignal->Peak ();

	if (primaryHeight < primaryThreshold)
		return -1;

	mCanBePrimary [primaryChannel] = true;

	while (nextSignal = (DataSignal*) it()) {

		if (nextSignal == mPrimarySignal)
			continue;

		nextChannel = nextSignal->GetChannel ();
		nextPeakHeight = mPeakHeights [nextChannel] = nextSignal->Peak ();
		mDirectedGraph [primaryChannel][nextChannel] = 1;
		mRatios [primaryChannel][nextChannel] = nextPeakHeight / primaryHeight;

		if (nextSignal->IsNegativePeak ()) {

			mIsNegativePullup [primaryChannel][nextChannel] = true;
			mIsNegativePeak [nextChannel] = true;
			nextSignal->SetCannotBePrimary (true);
		}

		else if (nextPeakHeight >= primaryThreshold) {

			mCanBePrimary [nextChannel] = true;
			nextSignal->SetCannotBePrimary (false);
		}

		else
			nextSignal->SetCannotBePrimary (true);

		if (nextPeakHeight >= channelNoiseLevels [nextChannel])
			mIsAboveNoiseThreshold [nextChannel] = true;
	}

	//  This takes care of current primay relationships.  Now fill in potential secondary ones.

	for (i=1; i<=mNChannels; i++) {

		if (i == primaryChannel)
			continue;

		if (mCanBePrimary [i]) {

			//  This is a non-primary channel that could be primary, so add info to arrays and matrices

			for (j=1; j<=mNChannels; j++) {

				if (j == primaryChannel)
					continue;

				if (i == j)
					continue;

				if (mPeakHeights [j] >= mPeakHeights [i])
					continue;

				if (mIsNegativePeak [j])
					mIsNegativePullup [i][j] = true;

				mDirectedGraph [i][j] = 1;
				mRatios [i][j] = mPeakHeights [j] / mPeakHeights [i];
			}
		}
	}

	return 0;
}


DataSignal* InterchannelLinkage :: GetSecondarySignalOnChannelIfNoSecondPrimary (int secondaryChannel) {

	DataSignal* nextSignal;
	DataSignal* returnSignal = NULL;
	RGDListIterator it (mSecondarySignals);
	double maxHeightForOtherPrimaries;
	int nPrimaries = 0;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal == mPrimarySignal)
			continue;

		if (nextSignal->GetChannel () == secondaryChannel)
			returnSignal = nextSignal;

		if (nextSignal->HasCrossChannelSignalLink ())
			nPrimaries++;
	}

	if (returnSignal == NULL)
		return NULL;

	if (nPrimaries == 0)
		return returnSignal;

	if ((nPrimaries == 1) && (returnSignal->HasCrossChannelSignalLink ()))
		return returnSignal;

	if (!returnSignal->HasCrossChannelSignalLink ())
		return NULL;

	it.Reset ();
	maxHeightForOtherPrimaries = returnSignal->Peak ();

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal == mPrimarySignal)
			continue;

		if (nextSignal == returnSignal)
			continue;

		if (nextSignal->HasCrossChannelSignalLink ()) {

			if (nextSignal->Peak () > maxHeightForOtherPrimaries)
				return NULL;
		}
	}

	return returnSignal;	
}


DataSignal* InterchannelLinkage :: GetSecondarySignalOnChannel (int secondaryChannel) {

	DataSignal* nextSignal;
	RGDListIterator it (mSecondarySignals);

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal == mPrimarySignal)
			continue;

		if (nextSignal->GetChannel () == secondaryChannel)
			return nextSignal;
	}

	return NULL;
}



STRInterchannelLinkage :: STRInterchannelLinkage (int nChannels) : InterchannelLinkage (nChannels) {

}


STRInterchannelLinkage :: ~STRInterchannelLinkage () {

}


bool STRInterchannelLinkage :: RecalculatePrimarySignal (Notice* primaryTarget, Notice* primaryReplace) {

	// Add notice objects for test and replacement on removal

	if (mSecondarySignals.Entries () <= 1)
		return false;

	if (mPrimarySignal != NULL)
		return true;
	
	DataSignal* nextSignal;
	RGDListIterator it (mSecondarySignals);
	DataSignal* maxSignal = NULL;
	double maxPeak = 0.0;
	double thisPeak;
//	bool hasSaturationArtifacts = false;

	/*while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->isSaturationArtifact ()) {

			hasSaturationArtifacts = true;
			break;
		}
	}

	it.Reset ();*/

	while (nextSignal = (DataSignal*) it ()) {

//		if (hasSaturationArtifacts && !nextSignal->isSaturationArtifact ())
//			continue;

//		if (nextSignal->cannotBePrimary ())  // we should check this out????
//			continue;
		
		thisPeak = nextSignal->Peak ();

		if (thisPeak > maxPeak) {

			maxPeak = thisPeak;
			maxSignal = nextSignal;
		}
	}

	if (maxSignal == NULL)
		return false;

	mPrimarySignal = maxSignal;
	mPrimarySignal->ReplaceNotice (primaryTarget, primaryReplace);
	mPrimarySignal->RemoveAllCrossChannelSignalLinks ();

	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal != mPrimarySignal) {

			nextSignal->SetPrimaryCrossChannelSignalLink (mPrimarySignal);
			mPrimarySignal->AddCrossChannelSignalLink (nextSignal);
		}
	}

	return true;
}


PeakInfoForClusters :: PeakInfoForClusters () : RGPersistent (), 
mPeak (0.0), mMean (0.0), mIsPullup (false), mIsAboveMaxRFU (false), mParentSignal (NULL), mIncludeInBaseCluster (true),
mIndex (0), mChosen (false), mProcessed (false) {

}



PeakInfoForClusters :: PeakInfoForClusters (DataSignal* ds) : RGPersistent (),
mPeak (ds->Peak ()), mMean (ds->GetMean ()), mIsPullup (false), mIsAboveMaxRFU (false), mParentSignal (ds), mIncludeInBaseCluster (true),
mIndex (0), mChosen (false), mProcessed (false) {

	PullUpFound pullup;
	PullUpPrimaryChannel primary;
	SignalPeakAboveMaxRFU aboveMax;

	if ((ds->IsNoticeInList (&pullup)) && (!ds->IsNoticeInList (&primary)))
		mIsPullup = true;

	if (ds->IsNoticeInList (&aboveMax))
		mIsAboveMaxRFU = true;
}



PeakInfoForClusters :: PeakInfoForClusters (const PeakInfoForClusters& pifc) : RGPersistent (pifc),
mPeak (pifc.mPeak),
mMean (pifc.mMean),
mIsPullup (pifc.mIsPullup),
mIsAboveMaxRFU (pifc.mIsAboveMaxRFU),
mParentSignal (pifc.mParentSignal), 
mIncludeInBaseCluster (pifc.mIncludeInBaseCluster),
mIndex (pifc.mIndex), 
mChosen (pifc.mChosen), 
mProcessed (pifc.mProcessed) {

}



PeakInfoForClusters :: ~PeakInfoForClusters () {

}


double PeakInfoForClusters :: Distance (const PeakInfoForClusters& pifc) {

	double peakDiff = PeakInfoForClusters::HeightFactor * (mPeak - pifc.GetPeak ());
	double meanDiff = mMean - pifc.GetMean ();
	return sqrt (peakDiff * peakDiff + meanDiff * meanDiff);
}



double PeakInfoForClusters :: Distance (double peak, double mean) {

	double peakDiff = PeakInfoForClusters::HeightFactor * (mPeak - peak);
	double meanDiff = mMean - mean;
	return sqrt (peakDiff * peakDiff + meanDiff * meanDiff);
}


size_t PeakInfoForClusters :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += 2 * sizeof (double) + 2 * sizeof (bool);
	return size;
}



int PeakInfoForClusters :: CompareTo (const RGPersistent* p) const {

	PeakInfoForClusters* q = (PeakInfoForClusters*) p;

	if (mMean < q->mMean)
		return -1;

	if (mMean > q->mMean)
		return 1;

	if (mPeak < q->mPeak)
		return -1;

	if (mPeak > q->mPeak)
		return 1;

	return 0;
}



Boolean PeakInfoForClusters :: IsEqualTo (const RGPersistent* p) const {

	PeakInfoForClusters* q = (PeakInfoForClusters*) p;

	if ((mMean == q->mMean) && (mPeak == q->mPeak))
		return true;

	return false;
}


double GetDistance (const PeakInfoForClusters& p1, const PeakInfoForClusters& p2) {

	double peakDiff = PeakInfoForClusters::GetHeightFactor () * (p1.mPeak - p2.mPeak);
	double meanDiff = p1.mMean - p2.mMean;
	return sqrt (peakDiff * peakDiff + meanDiff * meanDiff);
	//return abs (p1.mPeak - p2.mPeak);
}


bool LessHeight (const PeakInfoForClusters* p1, const PeakInfoForClusters* p2) {

	if (!p1)
		return true;

	if (!p2)
		return false;

	return (p1->GetPeak () < p2->GetPeak ());
}


bool LessDistance (const PeakInfoForClusters* p1, const PeakInfoForClusters* p2) {

	if (!p1)
		return true;

	if (!p2)
		return false;

	return (p1->GetMean () < p2->GetMean ());
}


DataSignal :: DataSignal (const DataSignal& ds) : SmartMessagingObject ((SmartMessagingObject&)ds), Left (ds.Left), Right (ds.Right), LeftSearch (ds.LeftSearch),
		RightSearch (ds.RightSearch), Fit (ds.Fit), ResidualPower (ds.ResidualPower), MeanVariability (ds.MeanVariability), BioID (ds.BioID), 
		ApproximateBioID (ds.ApproximateBioID), mApproxBioIDPrime (ds.mApproxBioIDPrime), mWidth (-1.0), mNoticeObjectIterator (NewNoticeList), markForDeletion (ds.markForDeletion), mOffGrid (ds.mOffGrid), mAcceptedOffGrid (ds.mAcceptedOffGrid), signalLink (NULL), 
		mPrimaryCrossChannelLink (NULL), mDontLook (ds.mDontLook), mTestLeftEndPoint (ds.mTestLeftEndPoint), mTestRightEndPoint (ds.mTestRightEndPoint), 
		mDataMode (ds.mDataMode), mRawDataBelowMinHeight (ds.mRawDataBelowMinHeight), mOsirisFitBelowMinHeight (ds.mOsirisFitBelowMinHeight),
		mRawDataAboveMaxHeight (ds.mRawDataAboveMaxHeight), mOsirisFitAboveMaxHeight (ds.mOsirisFitAboveMaxHeight), mPossibleInterlocusAllele (ds.mPossibleInterlocusAllele), 
		mLeftLocus (ds.mLeftLocus), mRightLocus (ds.mRightLocus), mIsGraphable (ds.mIsGraphable), 
		mHasCrossChannelLink (ds.mHasCrossChannelLink), mIsSaturationArtifact (ds.mIsSaturationArtifact), mInterchannelLink (ds.mInterchannelLink),
		mCannotBePrimary (ds.mCannotBePrimary), mBioIDLeft (ds.mBioIDLeft), mBioIDRight (ds.mBioIDRight), mAlleleNameLeft (ds.mAlleleNameLeft), mAlleleNameRight (ds.mAlleleNameRight), 
		mResidualLeft (ds.mResidualLeft), mResidualRight (ds.mResidualRight), mPossibleInterAlleleLeft (ds.mPossibleInterAlleleLeft), 
		mPossibleInterAlleleRight (ds.mPossibleInterAlleleRight), mIsAcceptedTriAlleleLeft (ds.mIsAcceptedTriAlleleLeft), mIsAcceptedTriAlleleRight (ds.mIsAcceptedTriAlleleRight), 
		mAlleleName (ds.mAlleleName), mIsOffGridLeft (ds.mIsOffGridLeft), mIsOffGridRight (ds.mIsOffGridRight), mSignalID (ds.mSignalID), mArea (ds.mArea), mLocus (ds.mLocus), 
		mMaxMessageLevel (ds.mMaxMessageLevel), mDoNotCall (ds.mDoNotCall), mReportersAdded (false), mAllowPeakEdit (ds.mAllowPeakEdit), mCannotBePrimaryPullup (ds.mCannotBePrimaryPullup), 
		mMayBeUnacceptable (ds.mMayBeUnacceptable), mHasRaisedBaseline (ds.mHasRaisedBaseline), mBaseline (ds.mBaseline), mIsNegativePeak (ds.mIsNegativePeak), mPullupTolerance (ds.mPullupTolerance), 
		mPrimaryRatios (NULL), mPullupCorrectionArray (NULL), mPrimaryPullupInChannel (NULL), mPartOfCluster (ds.mPartOfCluster), mIsPossiblePullup (ds.mIsPossiblePullup), mIsNoisySidePeak (ds.mIsNoisySidePeak), mNextSignal (NULL), 
		mPreviousSignal (NULL), mCumulativeStutterThreshold (0.0), mIsShoulderSignal (ds.IsShoulderSignal ()) {

		NoticeList = ds.NoticeList;
		NewNoticeList = ds.NewNoticeList;
		InitializeSmartMessages (ds);

		if (ds.mThisDataSegment != NULL)
			mThisDataSegment = new DataInterval (*ds.mThisDataSegment);

		else
			mThisDataSegment = NULL;
	}


DataSignal :: DataSignal (const DataSignal& ds, CoordinateTransform* trans) : SmartMessagingObject ((SmartMessagingObject&)ds), LeftSearch (ds.LeftSearch),
RightSearch (ds.RightSearch), Fit (ds.Fit), ResidualPower (ds.ResidualPower), MeanVariability (ds.MeanVariability), BioID (ds.BioID), 
ApproximateBioID (ds.ApproximateBioID), mApproxBioIDPrime (ds.mApproxBioIDPrime), mWidth (-1.0), mNoticeObjectIterator (NewNoticeList), markForDeletion (ds.markForDeletion), mOffGrid (ds.mOffGrid), mAcceptedOffGrid (ds.mAcceptedOffGrid), 
signalLink (NULL), mPrimaryCrossChannelLink (NULL), mDontLook (ds.mDontLook), mTestLeftEndPoint (ds.mTestLeftEndPoint), mTestRightEndPoint (ds.mTestRightEndPoint), 
mDataMode (ds.mDataMode), mRawDataBelowMinHeight (ds.mRawDataBelowMinHeight), mOsirisFitBelowMinHeight (ds.mOsirisFitBelowMinHeight),
mRawDataAboveMaxHeight (ds.mRawDataAboveMaxHeight), mOsirisFitAboveMaxHeight (ds.mOsirisFitAboveMaxHeight), mIsGraphable (ds.mIsGraphable), 
mHasCrossChannelLink (ds.mHasCrossChannelLink), mIsSaturationArtifact (ds.mIsSaturationArtifact), mInterchannelLink (ds.mInterchannelLink),
mCannotBePrimary (ds.mCannotBePrimary), mBioIDLeft (ds.mBioIDLeft), mBioIDRight (ds.mBioIDRight), mAlleleNameLeft (ds.mAlleleNameLeft), mAlleleNameRight (ds.mAlleleNameRight), 
mResidualLeft (ds.mResidualLeft), mResidualRight (ds.mResidualRight), mPossibleInterAlleleLeft (ds.mPossibleInterAlleleLeft), 
mPossibleInterAlleleRight (ds.mPossibleInterAlleleRight), mIsAcceptedTriAlleleLeft (ds.mIsAcceptedTriAlleleLeft), mIsAcceptedTriAlleleRight (ds.mIsAcceptedTriAlleleRight), 
mAlleleName (ds.mAlleleName), mIsOffGridLeft (ds.mIsOffGridLeft), mIsOffGridRight (ds.mIsOffGridRight), mSignalID (ds.mSignalID), mArea (ds.mArea), mLocus (ds.mLocus), 
mMaxMessageLevel (ds.mMaxMessageLevel), mDoNotCall (ds.mDoNotCall), mReportersAdded (false), mAllowPeakEdit (ds.mAllowPeakEdit), mCannotBePrimaryPullup (ds.mCannotBePrimaryPullup), 
mMayBeUnacceptable (ds.mMayBeUnacceptable), mHasRaisedBaseline (ds.mHasRaisedBaseline), mBaseline (ds.mBaseline), mIsNegativePeak (ds.mIsNegativePeak), mPullupTolerance (ds.mPullupTolerance), mPrimaryRatios (NULL), 
mPullupCorrectionArray (NULL), mPrimaryPullupInChannel (NULL), mPartOfCluster (ds.mPartOfCluster), mIsPossiblePullup (ds.mIsPossiblePullup), mIsNoisySidePeak (ds.mIsNoisySidePeak), mNextSignal (NULL), 
mPreviousSignal (NULL), mCumulativeStutterThreshold (0.0), mIsShoulderSignal (ds.IsShoulderSignal ()) {

	Left = trans->EvaluateWithExtrapolation (ds.Left);
	Right = trans->EvaluateWithExtrapolation (ds.Right);
	NoticeList = ds.NoticeList;
	NewNoticeList = ds.NewNoticeList;
	InitializeSmartMessages (ds);

	if (ds.mThisDataSegment != NULL)
		mThisDataSegment = new DataInterval (*ds.mThisDataSegment);

	else
		mThisDataSegment = NULL;
}



DataSignal :: ~DataSignal () {

	NoticeList.ClearAndDelete ();
	NewNoticeList.ClearAndDelete ();
	mCrossChannelSignalLinks.Clear ();
	delete[] mPrimaryRatios;
	delete[] mPullupCorrectionArray;
	delete[] mPrimaryPullupInChannel;
	mUncertainPullupChannels.clear ();
	mProbablePullupPeaks.Clear ();
	mStutterPrimaryList.Clear ();
	mLeftStutterPrimaryList.Clear ();
	mRightStutterPrimaryList.Clear ();

	mHasStandardStutterList.Clear ();
	mHasStutterList.Clear ();
	mHasStutterLeftList.Clear ();
	mHasStutterRightList.Clear ();

	delete mThisDataSegment;
}


double DataSignal :: LeftTestValue () const {

	return Value (mTestLeftEndPoint);
}


double DataSignal :: RightTestValue () const {

	return Value (mTestRightEndPoint);
}


bool DataSignal :: isMarginallyAboveMinimum () const {

	bool A = mRawDataBelowMinHeight && (!mOsirisFitBelowMinHeight);
	bool B = (!mRawDataBelowMinHeight) && mOsirisFitBelowMinHeight;
	return A || B;
}



bool DataSignal :: isMarginallyBelowMaximum () const {

	bool A = mRawDataAboveMaxHeight && (!mOsirisFitAboveMaxHeight);
	bool B = (!mRawDataAboveMaxHeight) && mOsirisFitAboveMaxHeight;
	return A || B;
}



void DataSignal :: AddCrossChannelSignalLink (DataSignal* ds) {

	mCrossChannelSignalLinks.Append (ds);
}


void DataSignal :: RemovePrimaryCrossChannelSignalLink (Notice* target, Notice* replace, const DataSignal* remove) {

	// for primary, from secondary
//	Notice* newNotice;

	if (mCrossChannelSignalLinks.Entries () > 0) {

		mCrossChannelSignalLinks.RemoveReference (remove);
		
		if (mCrossChannelSignalLinks.Entries () == 0)
			ReplaceNotice (target, replace);

		/*else {   // This section needlessly adds a notice that may not be founded!  At least, so I think (02/03/2009)

			if ((replace != NULL) && (!IsNoticeInList (replace))) {

				newNotice = (Notice*) replace->Copy ();
				AddNoticeToList (newNotice);
			}
		}*/
	}
}


void DataSignal :: RemoveSecondaryCrossChannelSignalLink (Notice* target, Notice* replace) {
	
	// for secondary, from Primary

	if (mPrimaryCrossChannelLink != NULL) {

		mPrimaryCrossChannelLink = NULL;
		ReplaceNotice (target, replace);
	}
}


void DataSignal :: ReplaceNotice (Notice* target, Notice* replace) {

	if (replace == NULL) {

		RemoveNotice (target);
		return;
	}
	
	Notice* oldNotice = RemoveNotice (target);
	Notice* newNotice = (Notice*) replace->Copy ();

	if (oldNotice != NULL) {

		int N = oldNotice->GetNumberOfDataItems ();

		for (int i=0; i<N; i++)
			newNotice->AddDataItem (oldNotice->GetDataItem (i));

		delete oldNotice;
	}

	AddNoticeToList (newNotice);
}


void DataSignal :: RemoveAllCrossChannelSignalLinks () {

	mCrossChannelSignalLinks.Clear ();
	mPrimaryCrossChannelLink = NULL;
}


void DataSignal :: RemoveCrossChannelSignalLink (Notice* primaryTarget, Notice* secondaryTarget, Notice* primaryReplace, Notice* secondaryReplace) {

	if (mPrimaryCrossChannelLink != NULL) {

		// This is a secondary link

		mPrimaryCrossChannelLink->RemovePrimaryCrossChannelSignalLink (primaryTarget, primaryReplace, this);
		mPrimaryCrossChannelLink = NULL;
		ReplaceNotice (secondaryTarget, secondaryReplace);
	}

	else if (mCrossChannelSignalLinks.Entries () > 0) {

		// This is a primary link

		DataSignal* nextSignal;

		while (nextSignal = (DataSignal*) mCrossChannelSignalLinks.GetFirst ())
			nextSignal->RemoveSecondaryCrossChannelSignalLink (secondaryTarget, secondaryReplace);

		ReplaceNotice (primaryTarget, primaryReplace);
	}
}


bool DataSignal :: IsSecondaryCrossChannelSignalLink () const {

	if (mPrimaryCrossChannelLink != NULL)
		return true;

	return false;
}


bool DataSignal :: IsPrimaryCrossChannelSignalLink () const {

	if (mCrossChannelSignalLinks.Entries () > 0)
		return true;

	return false;
}


bool DataSignal :: HasCrossChannelSignalLink () const {

	return IsSecondaryCrossChannelSignalLink () || IsPrimaryCrossChannelSignalLink ();
}


double DataSignal :: GetPullupFromChannel (int i) const {

	if (mPullupCorrectionArray == NULL)
		return 0.0;

	return mPullupCorrectionArray [i];
}


double DataSignal :: GetTotalPullupFromOtherChannels (int numberOfChannels) const {

	if (mPullupCorrectionArray == NULL)
		return 0.0;

	int i;
	double total = 0.0;

	for (i=1; i<=numberOfChannels; i++)
		total += GetPullupFromChannel (i);

	return total;
}


void DataSignal :: SetPullupFromChannel (int i, double value, int numberOfChannels) {

	if (mPullupCorrectionArray == NULL) {

		mPullupCorrectionArray = new double [numberOfChannels + 1];
		int j;

		for (j=1; j<=numberOfChannels; j++)
			mPullupCorrectionArray [j] = 0.0;
	}

	mPullupCorrectionArray [i] = value;
}


DataSignal* DataSignal :: HasPrimarySignalFromChannel (int i) const {

	if (mPrimaryPullupInChannel == NULL)
		return NULL;

	return mPrimaryPullupInChannel [i];
}


void DataSignal :: SetPrimarySignalFromChannel (int i, DataSignal* ds, int numberOfChannels) {

	if (mPrimaryPullupInChannel == NULL) {

		mPrimaryPullupInChannel = new DataSignal* [numberOfChannels + 1];
		int j;

		for (j=1; j<=numberOfChannels; j++)
			mPrimaryPullupInChannel [j] = NULL;
	}

	mPrimaryPullupInChannel [i] = ds;
}


bool DataSignal :: HasAnyPrimarySignals (int numberOfChannels) const {

	int i;
	int myChannel = GetChannel ();

	if (mPrimaryPullupInChannel == NULL)
		return false;

	for (i=1; i<=numberOfChannels; i++) {

		if (myChannel == i)
			continue;

		if (mPrimaryPullupInChannel [i] != NULL)
			return true;
	}

	return false;
}


void DataSignal :: AddUncertainPullupChannel (int channel) {

	mUncertainPullupChannels.insert (channel);
}


bool DataSignal :: UncertainPullupChannelListIsEmpty () {

	return (mUncertainPullupChannels.empty ());
}



bool DataSignal :: PullupChannelIsUncertain (int channel) {

	return (mUncertainPullupChannels.find (channel) != mUncertainPullupChannels.end ());
}



RGString DataSignal :: CreateUncertainPullupString () {

	RGString result;

	if (mUncertainPullupChannels.empty ())
		return result;

	set<int>::iterator it;
	int i = 0;
	int channel;

	for (it=mUncertainPullupChannels.begin (); it!=mUncertainPullupChannels.end (); it++) {

		channel = *it;

		if (i > 0)
			result << ", ";

		result << channel;
	}

	return result;
}


void DataSignal :: AddProbablePullups (RGDList& prototype) {

	RGDListIterator it (prototype);
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) it ())
		mProbablePullupPeaks.InsertWithNoReferenceDuplication (nextSignal);
}


void DataSignal :: RemoveProbablePullup (const DataSignal* removeSignal) {

	mProbablePullupPeaks.RemoveReference (removeSignal);
}


bool DataSignal :: ReconfigurePullupFromLinkForChannel (int channel) {

	RGDListIterator it (mProbablePullupPeaks);
	DataSignal* currentPullup = NULL;
	DataSignal* nextPullup;
	double mu = GetMean ();
	double distance;
	double currentDistance;

	while (nextPullup = (DataSignal*) it ()) {

		if (nextPullup->GetChannel () == channel) {

			distance = fabs (nextPullup->GetMean () - mu);

			if (currentPullup == NULL) {

				currentPullup = nextPullup;
				currentDistance = distance;
			}

			else {

				if (distance <= currentDistance) {

					currentPullup = nextPullup;
					currentDistance = distance;
				}
			}
		}
	}

	if (currentPullup == NULL)
		return false;

	InterchannelLinkage* iChannel = GetInterchannelLink ();

	if (iChannel == NULL)
		return false;

	iChannel->AddDataSignal (currentPullup);
	currentPullup->SetPrimarySignalFromChannel (GetChannel (), (DataSignal*) this, NumberOfChannels);
	return true;
}


bool DataSignal :: IsInProbablePullupList (const DataSignal* testSignal) {

	if (mProbablePullupPeaks.ContainsReference (testSignal))
		return true;

	return false;
}



void DataSignal :: SetBioID (double id, int position) {

	if (position < 0)
		mBioIDLeft = id;

	else if (position > 0)
		mBioIDRight = id;

	else
		BioID = id;
}



void DataSignal :: SetAlleleName (const RGString& name, int position) {

	if (position < 0)
		mAlleleNameLeft = name;

	else if (position > 0)
		mAlleleNameRight = name;

	else
		mAlleleName = name;
}



void DataSignal :: SetBioIDResidual (double residual, int position) {

	if (position < 0)
		mResidualLeft = residual;

	else if (position > 0)
		mResidualRight = residual;

	else
		Residual = residual;
}


void DataSignal :: SetLocus (Locus* locus, int position) {

	if (position < 0)
		mLeftLocus = locus;

	else if (position > 0)
		mRightLocus = locus;

	else
		mLocus = locus;
}


void DataSignal :: SetAlleleInformation (int position) {

	smAcceptedOLLeft acceptedOLLeft;
	smAcceptedOLRight acceptedOLRight;
	smExcessiveResidualDisplacement excessiveResidualDisplacement;
	smExcessiveResidualDisplacementLeft excessiveResidualDisplacementLeft;
	smExcessiveResidualDisplacementRight excessiveResidualDisplacementRight;

	if (position < 0) {

		//if (mAlleleNameLeft.Length () == 0)
		//	return;

		BioID = mBioIDLeft;
		mAlleleName = mAlleleNameLeft;
		Residual = mResidualLeft;
		mOffGrid = mIsOffGridLeft;
		mLocus = mLeftLocus;
		SetVirtualAlleleName (mAlleleName);

		if (GetMessageValue (acceptedOLLeft))
			mAcceptedOffGrid = true;

		SetMessageValue (excessiveResidualDisplacement, GetMessageValue (excessiveResidualDisplacementLeft));
		
	}

	else if (position > 0) {

		//if (mAlleleNameRight.Length () == 0)
		//	return;

		BioID = mBioIDRight;
		mAlleleName = mAlleleNameRight;
		Residual = mResidualRight;
		mOffGrid = mIsOffGridRight;
		mLocus = mRightLocus;
		SetVirtualAlleleName (mAlleleName);

		if (GetMessageValue (acceptedOLRight))
			mAcceptedOffGrid = true;

		SetMessageValue (excessiveResidualDisplacement, GetMessageValue (excessiveResidualDisplacementRight));
	}
}


int DataSignal :: RemoveFromOtherExtendedLocus (Locus* keep) {

	if (mLeftLocus == keep) {

		if (mRightLocus != NULL) {

			mRightLocus->RemoveSignalFromLocusList ((DataSignal*) this);
			return 0;
		}

		return -1;
	}

	if (mRightLocus == keep) {

		if (mLeftLocus != NULL) {

			mLeftLocus->RemoveSignalFromLocusList ((DataSignal*) this);
			return 0;
		}

		return -1;
	}

	return -1;
}


int DataSignal :: PromoteOtherExtendedLocus (Locus* dontKeep) {

	if (mLeftLocus == dontKeep) {

		if (mRightLocus != NULL) {

			mRightLocus->PromoteSignalToLeftExtendedAllele ((DataSignal*) this);
			return 0;
		}

		return -1;
	}

	if (mRightLocus == dontKeep) {

		if (mLeftLocus != NULL) {

			mLeftLocus->PromoteSignalToRightExtendedAllele ((DataSignal*) this);
			return 0;
		}

		return -1;
	}

	return -1;
}


int DataSignal :: RemoveFromAllLoci () {

	if (mRightLocus != NULL)
			mRightLocus->RemoveSignalFromLocusList ((DataSignal*) this);

	if (mLeftLocus != NULL)
			mLeftLocus->RemoveSignalFromLocusList ((DataSignal*) this);

	return 0;
}


void DataSignal :: SetCurrentDataInterval (const DataInterval* di) {
	
	if (di != NULL)
		mThisDataSegment = new DataInterval (*di);

	else
		di = NULL;
}



bool DataSignal :: IsPossibleInterlocusAllele (int position) const {

	if (position < 0)
		return mPossibleInterAlleleLeft;

	if (position > 0)
		return mPossibleInterAlleleRight;

	return false;
}


bool DataSignal :: IsAcceptedTriAllele (int position) const {

	if (position < 0)
		return mIsAcceptedTriAlleleLeft;

	if (position > 0)
		return mIsAcceptedTriAlleleRight;

	return false;
}


bool DataSignal :: IsOffGrid (int position) const {

	if (position < 0)
		return mIsOffGridLeft;

	if (position > 0)
		return mIsOffGridRight;

	return mOffGrid;
}

void DataSignal :: SetPossibleInterlocusAllele (int position, bool value) {

	if (position < 0)
		mPossibleInterAlleleLeft = value;

	else if (position > 0)
		mPossibleInterAlleleRight = value;
}


void DataSignal :: SetAcceptedTriAllele (int position, bool value) {

	if (position < 0)
		mIsAcceptedTriAlleleLeft = value;

	else if (position > 0)
		mIsAcceptedTriAlleleRight = value;
}


void DataSignal :: SetOffGrid (int position, bool value) {

	bool actualValue = value;

	if (ConsiderAllOLAllelesAccepted)
		actualValue = false;

	if (position < 0)
		mIsOffGridLeft = actualValue;

	else if (position > 0)
		mIsOffGridRight = actualValue;

	else
		mOffGrid = actualValue;
}


double DataSignal :: GetBioID (int position) const {

	if (position < 0)
		return mBioIDLeft;

	else if (position > 0)
		return mBioIDRight;

	return BioID;
}



RGString DataSignal :: GetAlleleName (int position) const {

	if (position < 0)
		return mAlleleNameLeft;

	else if (position > 0)
		return mAlleleNameRight;

	return mAlleleName;
}



double DataSignal :: GetBioIDResidual (int position) const {

	if (position < 0)
		return mResidualLeft;

	else if (position > 0)
		return mResidualRight;

	return Residual;
}


const Locus* DataSignal :: GetLocus (int position) const {

	if (position < 0)
		return mLeftLocus;

	else if (position > 0)
		return mRightLocus;

	return mLocus;
}


void DataSignal :: ResetSignalLink () {

	if (signalLink != NULL) {

		signalLink->nameStringBuilt = false;
		signalLink->xmlArtifactInfoWritten = false;
		signalLink->xmlPeakInfoWritten = false;
	}
}


CompoundSignalInfo* DataSignal :: RemoveSignalLink () {

	if (signalLink == NULL)
		return NULL;

	CompoundSignalInfo* csi = signalLink;
	signalLink = NULL;
	return csi;	
}


void DataSignal :: SetOffGrid (bool r) {

	if (ConsiderAllOLAllelesAccepted)
		mOffGrid = false;

	else
		mOffGrid = r;
}


void DataSignal :: SetCurveFit (double fit) {

	Fit = fit;

	// The following formula is based on the dot product of two identical
	// Gaussian curves that are offset.  Based on the dot product, we can
	// calculate the offset

	if (fit > 0.0) {

		if (fit < 1.0) {

			MeanVariability = 0.2 + 3.0 * sqrt (-log (fit));
			ResidualPower = sqrt (1.0 - fit * fit);
		}

		else {

			MeanVariability = 0.0;
			ResidualPower = 0.0;
		}
	}

	else {

		MeanVariability = 50.0;
		ResidualPower = 1.0;
	}
}



RGString DataSignal :: GetSignalType () const {

	return RGString ("DataSignal");
}


void DataSignal :: SetPullupRatio (int channel, double ratio, int nChannels) {

	int i;

	if (mPrimaryRatios == NULL) {

		mPrimaryRatios = new double [nChannels + 1];

		for (i=1; i<=nChannels; i++)
			mPrimaryRatios [i] = 0.0;
	}

	mPrimaryRatios [channel] = ratio;
}


double DataSignal :: GetPullupRatio (int channel) {

	if (mPrimaryRatios == NULL)
		return 0.0;

	return mPrimaryRatios [channel];
}


double DataSignal :: GetWidth () {

	if (mWidth > 0.0)
		return mWidth;

	double leftSearch = GetMean ();
	double mu = leftSearch;
	double sigma = GetStandardDeviation ();

	if (sigma < 1.0) {

		sigma = 1.0;
	}

	double targetWidth = 0.01 * sigma;
	double rightSearch = leftSearch + sigma;
	int i;
	double targetValue = 0.5 * Peak ();
	double leftValue;
	double rightValue;
	double midPoint;
	double midValue;
	bool foundInterval = false;

	for (i=0; i<10; i++) {

		leftValue = Value (leftSearch);
		rightValue = Value (rightSearch);

		if (leftValue == targetValue) {

			mWidth = 2.0 * (leftSearch - mu);
			return mWidth;
		}

		if (rightValue == targetValue) {

			mWidth = 2.0 * (rightSearch - mu);
			return mWidth;
		}

		if (rightValue > targetValue) {  // bump interval

			leftSearch = rightSearch;
			rightSearch += sigma;
		}

		else {  // we found a bracketing interval!

			foundInterval = true;
			break;
		}
	}

	if (!foundInterval) {

		cout << "Could not find width bracketing interval for peak at " << GetMean () << " with sigma = " << GetStandardDeviation () << " and peak = " << Peak () << endl;
		mWidth = 3.14159;
		return mWidth;
	}

	midPoint = 0.5 * (leftSearch + rightSearch);
	i = 0;

	while (true) {

		midValue = Value (midPoint);

		if (midValue == targetValue) {

			mWidth = 2.0 * (midPoint - mu);
			return mWidth;
		}

		if (midValue > targetValue)  // replace leftSearch
			leftSearch = midPoint;

		else
			rightSearch = midPoint;

		midPoint = 0.5 * (leftSearch + rightSearch);

		if (fabs (rightSearch - leftSearch) < targetWidth) {

			mWidth = 2.0 * (midPoint - mu);
			return mWidth;
		}

		i++;

		if (i > 10)
			break;
	}

	cout << "Could not terminate binary search for peak of type " << GetSignalType () << " at " << GetMean () << " with sigma = " << GetStandardDeviation () << " and peak = " << Peak () << endl;
	mWidth = 3.14159;
	return mWidth;
}


int DataSignal :: FindAndRemoveFixedOffset () {

	return -1;
}


DataSignal* DataSignal :: CreateMovingAverageFilteredSignal (int window) {

	return NULL;
}


DataSignal* DataSignal :: CreateThreeMovingAverageFilteredSignal (int window) {

	return NULL;
}


void DataSignal :: SetAlleleName (const RGString& name) {

	mAlleleName = name;

	if (signalLink != NULL)
		signalLink->alleleName = name;
}


RGString DataSignal :: GetAlleleName () const {

	return mAlleleName;
}


bool DataSignal :: HasAlleleName () const {

	return false;
}


bool DataSignal :: TestForMultipleSignals (DataSignal*& prev, DataSignal*& next) {

	prev = next = NULL;
	return false;
}


bool DataSignal :: TestForMultipleSignals (DataSignal*& prev, DataSignal*& next, int location) {

	prev = next = NULL;
	return false;
}


void DataSignal :: SetVirtualAlleleName (const RGString& name) {

}


RGString DataSignal :: GetVirtualAlleleName () const {

	return "";
}


int DataSignal :: AddNoticeToList (unsigned long level, const RGString& label, const RGString& notice) {

	RGString localNotice (notice);

	if (notice.GetLastCharacter () != '\n')
		localNotice += "\n";

	RGIndexedLabel* newNotice = new RGIndexedLabel (level, label, localNotice);
	RGIndexedLabel::SetComparisonToIndex ();
	NoticeList.Insert (newNotice);
	return NoticeList.Entries ();
}



Boolean DataSignal :: ReportNotices (RGTextOutput& text, const RGString& indent, const RGString& delim) {

	if (NoticeList.Entries () > 0) {

		RGDListIterator it (NoticeList);
		RGIndexedLabel* nextNotice = (RGIndexedLabel*) it ();
		text.SetOutputLevel ((int)nextNotice->GetIndex ());

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			return FALSE;
		}

		Endl endLine;
		text << endLine;
		text << indent << "Notices for curve with (Mean, Sigma, Peak, 2Content, Fit) = " << delim << delim << delim << delim << delim << delim;
		text << GetMean () << delim << GetStandardDeviation () << delim << Peak () << delim << GetScale (2) << delim << Fit << endLine;
		text.ResetOutputLevel ();
		RGString notice = indent + delim + nextNotice->GetText ();
		text.Write (nextNotice->GetIndex (), notice);

		while (nextNotice = (RGIndexedLabel*) it ()){

			notice = indent + delim + nextNotice->GetText ();
			text.Write (nextNotice->GetIndex (), notice);;
		}

		text.Write (1, "\n");
	}

	else
		return FALSE;

	return TRUE;
}


Boolean DataSignal :: ReportNotices (RGFile& file, const RGString& indent, const RGString& delim) {
	
	if (NoticeList.Entries () > 0) {

		RGDListIterator it (NoticeList);
		RGIndexedLabel* nextLabel;
		RGString msg = "";
		msg.WriteTextLine (file);
		
		int res = OsirisMsg::GetDoubleResolution ();
		char buff [30];
		double Where = GetMean ();
		double Width = GetStandardDeviation ();
		double peak = Peak ();
		double fit = GetCurveFit ();

		msg = indent + "Notices for curve with (Mean, Sigma, Peak, 2Content, Fit) = " + delim + delim + delim + delim + delim << delim;
		msg += OsirisGcvt (Where, res, buff) + delim;
		msg += OsirisGcvt (Width, res, buff) + delim;
		msg += OsirisGcvt (peak, res, buff) + delim;
		msg += OsirisGcvt (GetScale (2), res, buff) + delim;
		msg += OsirisGcvt (fit, res, buff);
		msg.WriteTextLine (file);

		while (nextLabel = (RGIndexedLabel*) it ()) {

			msg = indent + delim + nextLabel->GetText ();
			msg.WriteTextLine (file);
		}
	}

	else
		return FALSE;

	return TRUE;
}



void DataSignal :: ClearNotices () {

	NoticeList.ClearAndDelete ();
}


int DataSignal :: NumberOfNotices () {

	return NoticeList.Entries ();
}


int DataSignal :: AddNoticeToList (Notice* newNotice) {

	int msgLevel;
	
	if (IsNoticeInList (newNotice)) {

		delete newNotice;
		return NewNoticeList.Entries ();
	}

	msgLevel = newNotice->GetMessageLevel ();

	if (NewNoticeList.Entries () == 0) {

		mHighestSeverityLevel = newNotice->GetSeverityLevel ();
		mHighestMessageLevel = msgLevel;
		NewNoticeList.Append (newNotice);
	}

	else {

		int temp = newNotice->GetSeverityLevel ();

		if (mHighestSeverityLevel >= temp) {

			mHighestSeverityLevel = temp;
			NewNoticeList.Prepend (newNotice);
		}

		else {

			RGDListIterator it (NewNoticeList);
			Notice* nextNotice;
			bool noticeInserted = false;

			while (nextNotice = (Notice*) it ()) {

				if (temp < nextNotice->GetSeverityLevel ()) {

					--it;
					it.InsertAfterCurrentItem (newNotice);
					noticeInserted = true;
					break;
				}
			}

			if (!noticeInserted)
				NewNoticeList.Append (newNotice);
		}

		temp = newNotice->GetMessageLevel ();

		if (mHighestMessageLevel > temp)
			mHighestMessageLevel = temp;
	}

	if (newNotice->IsTriggerForLowPriority ()) {

		if (msgLevel > mMaxMessageLevel)
			mMaxMessageLevel = msgLevel;
	}

	if (newNotice->IsDoNotCall ())
		mDoNotCall = true;

	return NewNoticeList.Entries ();
}



Boolean DataSignal :: IsNoticeInList (const Notice* target) {

	if (NewNoticeList.Find (target))
		return TRUE;

	return FALSE;
}


Notice* DataSignal :: GetNotice (const Notice* target) {

	return (Notice*) NewNoticeList.Find (target);
}



Boolean DataSignal :: ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim) {

	if (NewNoticeList.Entries () > 0) {

		RGDListIterator it (NewNoticeList);
		Notice* nextNotice;
		text.SetOutputLevel (mHighestMessageLevel);

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			return FALSE;
		}

		Endl endLine;
		text << endLine;
		text << indent << "Notices for curve with (Mean, Sigma, Peak, 2Content, Fit) = " << delim << delim << delim << delim << delim << delim;
		text << GetMean () << delim << GetStandardDeviation () << delim << Peak () << delim << GetScale (2) << delim << Fit << endLine;
		text.ResetOutputLevel ();

		while (nextNotice = (Notice*) it ())
			nextNotice->Report (text, indent, delim);

		text.Write (1, "\n");
	}

	else
		return FALSE;

	return TRUE;
}


void DataSignal :: ClearNoticeObjects () {

	NewNoticeList.ClearAndDelete ();
	mHighestSeverityLevel = mHighestMessageLevel = -1;
}



int DataSignal :: NumberOfNoticeObjects () {

	return NewNoticeList.Entries ();
}


Notice* DataSignal :: RemoveNotice (const Notice* target) {

	Notice* returnValue = (Notice*) NewNoticeList.Remove (target);

	RGDListIterator it (NewNoticeList);
	Notice* nextNotice;
	int level = 0;
	mMaxMessageLevel = 1;
	int msgLevel;

	while (nextNotice = (Notice*) it ()) {

		msgLevel = nextNotice->GetMessageLevel ();

		if (level == 0)
			level = msgLevel;

		else if (msgLevel < level)
			level = msgLevel;

		if (nextNotice->IsTriggerForLowPriority ()) {

			if (msgLevel > mMaxMessageLevel)
				mMaxMessageLevel = msgLevel;
		}
	}

	mHighestSeverityLevel = mHighestMessageLevel = level;
	return returnValue;
}


double DataSignal :: ValueFreeBound (int n) const {

	return -3.14159;
}


double DataSignal :: ValueFreeBound (double x) const {

	return -3.14159;
}


void DataSignal :: AddPrimaryStutterSignalToList (DataSignal* primary, int dir) {

	mStutterPrimaryList.InsertWithNoReferenceDuplication (primary);
	//
	//if (dir)
	//	mStutterPrimaryList.Append (primary); 
	//
	//else
	//	mStutterPrimaryList.Prepend (primary);
}



void DataSignal :: AddLeftPrimaryStutterSignalToList (DataSignal* primary, int dir) {
	
	if (dir)
		mLeftStutterPrimaryList.Append (primary);
	
	else
		mLeftStutterPrimaryList.Prepend (primary);
}



void DataSignal :: AddRightPrimaryStutterSignalToList (DataSignal* primary, int dir) {
	
	if (dir)
		mRightStutterPrimaryList.Append (primary);
	
	else
		mRightStutterPrimaryList.Prepend (primary);
}



void DataSignal :: RemoveAllStutterLinks () {

	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) mStutterPrimaryList.GetFirst ()) {

		nextSignal->RemoveStutterLink ((DataSignal*)this);
	}

	while (nextSignal = (DataSignal*) mLeftStutterPrimaryList.GetFirst ()) {

		nextSignal->RemoveStutterLink ((DataSignal*)this);
	}

	while (nextSignal = (DataSignal*) mRightStutterPrimaryList.GetFirst ()) {

		nextSignal->RemoveStutterLink ((DataSignal*)this);
	}
}



void DataSignal :: AddStandardStutterSignalToList (DataSignal* stutter, int dir) {
	
	if (dir)
		mHasStandardStutterList.Append (stutter);
	
	else
		mHasStandardStutterList.Prepend (stutter);
}



void DataSignal :: AddStutterDisplacement (int disp, int dir) {
	
	if (dir)
		mStutterDisplacements.push_back (disp);
	
	else
		mStutterDisplacements.push_front (disp);
}


void DataSignal :: RemoveStutterLink (DataSignal* ds) {

	mHasStandardStutterList.RemoveReference (ds);
	mHasStutterList.RemoveReference (ds);
	mHasStutterLeftList.RemoveReference (ds);
	mHasStutterRightList.RemoveReference (ds);

}


bool DataSignal :: LiesBelowHeightAt (double x, double height) {

	if (Value (x) <= height)
		return true;

	return false;
}


bool DataSignal :: TestForIntersectionWithPrimary (DataSignal* primary) {

	int i = 0;
	double sigma = GetStandardDeviation ();
	double mu = GetMean ();
	double peakTest = 0.25 * Peak ();
	double testPlus;
	double testMinus;
	double heightPlus;
	double heightMinus;
	double delta;

	if (primary->LiesBelowHeightAt (mu, Value (mu)))
		return true;

	while (true) {

		i++;
		delta = (double)i * sigma;
		testPlus = mu + delta;
		testMinus = mu - delta;
		heightPlus = Value (testPlus);
		heightMinus = Value (testMinus);

		if ((heightPlus < peakTest) || (heightMinus < peakTest))
			break;

		if (primary->LiesBelowHeightAt (testPlus, heightPlus))
			return true;

		if (primary->LiesBelowHeightAt (testMinus, heightMinus))
			return true;

		if (i > 5)
			break;
	}

	return false;
}


void DataSignal :: ResetCharacteristicsFromRight (TracePrequalification& trace, RGTextOutput& text, double minRFU, Boolean print) {
	
	RightSearch = Right;
}


void DataSignal :: ResetCharacteristicsFromLeft (TracePrequalification& trace, RGTextOutput& text, double minRFU, Boolean print) {
	
	LeftSearch = Left;
}


DataSignal* DataSignal :: FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, 
int windowSize, double& fit, RGDList& previous) const {
	
	return NULL;
}


void DataSignal :: ReportAbbreviated (RGTextOutput& text, const RGString& indent) {

	Endl endLine;
	text.SetOutputLevel (OutputLevelManager::AbbreviatedPeakData);
	text << indent <<  GetSignalType () << " with (Mean, Sigma, Peak, Fit)\t\t\t\t\t\t";
	text << GetMean () << "\t";
	text << GetStandardDeviation () << "\t";
	text << Peak () << "\t";
	text << GetCurveFit () << endLine;
	text.ResetOutputLevel ();
}


void DataSignal :: BuildAlleleString (RGString& alleleString, const RGString& delim) {

	if (signalLink == NULL) {

		if (alleleString.Length () > 0)
			alleleString += delim + GetAlleleName ();

		else
			alleleString = GetAlleleName ();
	}

	else if (!signalLink->nameStringBuilt) {

		signalLink->nameStringBuilt = true;

		if (alleleString.Length () > 0)
			alleleString += delim + signalLink->alleleName;

		else
			alleleString = signalLink->alleleName;
	}
}


void DataSignal :: WritePeakInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& bracketTag, const RGString& locationTag) {

	int peak;
	Endl endLine;
	RGString suffix;
	double totalCorrection;
	
	if (signalLink == NULL) {

		if (HasAlleleName ()) {

			peak = (int) floor (Peak () + 0.5);

			//if (NumberOfNoticeObjects () > 0)
			//	peak++;

			if (mOffGrid)
				suffix = " OL";

			text << indent << "<" << bracketTag << ">" << endLine;
			text << indent << "\t<mean>" << GetMean () << "</mean>" << endLine;
			text << indent << "\t<height>" << peak << "</height>" << endLine;

			totalCorrection = GetTotalPullupFromOtherChannels (NumberOfChannels);

			if (totalCorrection != 0.0)
				text << indent << "\t<PullupHeightCorrection>" << totalCorrection << "</PullupHeightCorrection>\n";

			text << indent << "\t<BPS>" << GetBioID () << "</BPS>" << endLine;
	//		text << indent << "\t<" << locationTag << ">" << (int) floor (GetApproximateBioID () + 0.5) << "</" << locationTag << ">" << endLine;
			text << indent << "\t<" << locationTag << ">" << GetApproximateBioID () << "</" << locationTag << ">" << endLine;
			text << indent << "\t<allele>" << GetAlleleName () << suffix << "</allele>" << endLine;
			text << indent << "\t<fit>" << GetCurveFit () << "</fit>" << endLine;
			text << indent << "</" + bracketTag << ">" << endLine;
		}
	}

	//else if (signalLink->alleleName.Length () > 0) {

	//	if (!signalLink->xmlPeakInfoWritten) {

	//		peak = signalLink->height + 1;
	//		signalLink->xmlPeakInfoWritten = true;

	//		if (mOffGrid)
	//			suffix = " OL";

	//		text << indent << "<" << bracketTag << ">" << endLine;
	//		text << indent << "\t<mean>" << signalLink->mean << "</mean>" << endLine;
	//		text << indent << "\t<height>" << peak << "</height>" << endLine;
	//		text << indent << "\t<" << locationTag << ">" << signalLink->bioID << "</" << locationTag << ">" << endLine;
	//		text << indent << "\t<allele>" << signalLink->alleleName << suffix << "</allele>" << endLine;
	//		text << indent << "\t<fit>" << GetCurveFit () << "</fit>" << endLine;
	//		text << indent << "</" + bracketTag << ">" << endLine;
	//	}
	//}
}


void DataSignal :: WriteArtifactInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& bracketTag, const RGString& locationTag) {

	int peak;
	Endl endLine;
	RGString suffix;
	RGString label;
	Notice* notice;
	int i;
	RGString virtualAllele;
	int highestMessageLevel = GetHighestMessageLevel ();
	int maxMessageLevel = GetMaxAllowedMessageLevel ();
	int reportedMessageLevel;
	double totalCorrection;

	if (highestMessageLevel > maxMessageLevel)
		reportedMessageLevel = highestMessageLevel;

	else
		reportedMessageLevel = maxMessageLevel;

	if ((!DontLook ()) && (NumberOfNoticeObjects () != 0)) {
	
		peak = (int) floor (Peak () + 0.5);

		if (mOffGrid)
			suffix = " OL";

		virtualAllele = GetVirtualAlleleName ();

		text << indent << "<" << bracketTag << ">" << endLine;
		text << indent << "\t<level>" << reportedMessageLevel << "</level>" << endLine;
		text << indent << "\t<mean>" << GetMean () << "</mean>" << endLine;
		text << indent << "\t<height>" << peak << "</height>" << endLine;

		totalCorrection = GetTotalPullupFromOtherChannels (NumberOfChannels);

		if (totalCorrection != 0.0)
			text << indent << "\t<PullupHeightCorrection>" << totalCorrection << "</PullupHeightCorrection>\n";

//		text << indent << "\t<" << locationTag << ">" << (int) floor (GetApproximateBioID () + 0.5) << "</" << locationTag << ">" << endLine;
		text << indent << "\t<" << locationTag << ">" << GetApproximateBioID () << "</" << locationTag << ">" << endLine;

		if (virtualAllele.Length () > 0)
			text << indent << "\t<equivAllele>" << virtualAllele << suffix << "</equivAllele>" << endLine;

		text << indent << "\t<fit>" << GetCurveFit () << "</fit>" << endLine;
		label = indent + "\t<label>";
		mNoticeObjectIterator.Reset ();
		i = 0;

		while (notice = (Notice*) mNoticeObjectIterator ()) {

			if (i > 0)
				label << "&#10;";

			label += notice->GetLabel ();
			i++;
		}

		RGString temp = GetVirtualAlleleName () + suffix;

		if (temp.Length () > 0) {

			if (i > 0)
				label << "&#10;";

			label += "Allele " + temp;
		}

		label << "</label>";
		text << label << endLine;
		text << indent << "</" + bracketTag << ">" << endLine;

		/*if ((signalLink != NULL) && (!signalLink->xmlArtifactInfoWritten)) {

			signalLink->xmlArtifactInfoWritten = true;
			peak = signalLink->height;

			text << indent << "<" << bracketTag << ">" << endLine;
			text << indent << "\t<mean>" << signalLink->mean << "</mean>" << endLine;
			text << indent << "\t<height>" << peak << "</height>" << endLine;
			text << indent << "\t<" << locationTag << ">" << signalLink->bioID << "</" << locationTag << ">" << endLine;
			text << indent << "\t<fit>" << GetCurveFit () << "</fit>" << endLine;
			label = indent + "\t<label>";
			mNoticeObjectIterator.Reset ();
			i = 0;

			while (notice = (Notice*) mNoticeObjectIterator ()) {

				if (i > 0)
					label << "&#10;";

				label += notice->GetLabel ();
				i++;
			}

			if (temp.Length () > 0) {

				if (i > 0)
					label << "&#10;";

				label += "Allele " + temp;
			}

			label << "</label>";
			text << label << endLine;
			text << indent << "</" + bracketTag << ">" << endLine;
		}*/
	}
}


void DataSignal :: WriteTableArtifactInfoToXML (RGTextOutput& text, RGTextOutput& tempText, const RGString& indent, const RGString& bracketTag, const RGString& locationTag) {

	int peak;
	Endl endLine;
	RGString suffix;
	RGString label;
	Notice* notice;
	int i;
	RGString virtualAllele;
	Notice* nextNotice;
	text.SetOutputLevel (1);
	int msgNum;
	int highestMessageLevel = GetHighestMessageLevel ();
	int maxMessageLevel = GetMaxAllowedMessageLevel ();
	int reportedMessageLevel;
	bool hasThreeLoci;
	bool needLocus0;
	double totalCorrection;

	smAcceptedOLLeft acceptedOLLeft;
	smAcceptedOLRight acceptedOLRight;

	if (highestMessageLevel > maxMessageLevel)
		reportedMessageLevel = highestMessageLevel;

	else
		reportedMessageLevel = maxMessageLevel;

	if ((!DontLook ()) && (NumberOfNoticeObjects () != 0)) {
	
		peak = (int) floor (Peak () + 0.5);
		virtualAllele = GetVirtualAlleleName ();

		text << indent << "<" << bracketTag << ">" << endLine;  // Should be <Artifact>
		text << indent << "\t<Id>" << GetSignalID () << "</Id>" << endLine;
		text << indent << "\t<Level>" << reportedMessageLevel << "</Level>" << endLine;
		text << indent << "\t<RFU>" << peak << "</RFU>" << endLine;

		totalCorrection = GetTotalPullupFromOtherChannels (NumberOfChannels);

		if (totalCorrection != 0.0)
			text << indent << "\t<PullupHeightCorrection>" << totalCorrection << "</PullupHeightCorrection>" << endLine;

		text << indent << "\t<" << locationTag << ">" << GetApproximateBioID () << "</" << locationTag << ">" << endLine;
		text << indent << "\t<PeakArea>" << TheoreticalArea () << "</PeakArea>" << endLine;
		text << indent << "\t<Width>" << GetWidth () << "</Width>" << endLine;
		text << indent << "\t<Time>" << GetMean () << "</Time>" << endLine;
		text << indent << "\t<Fit>" << GetCurveFit () << "</Fit>" << endLine;

		label = indent + "\t<Label>";
		mNoticeObjectIterator.Reset ();
		i = 0;

		while (notice = (Notice*) mNoticeObjectIterator ()) {

			if (i > 0)
				label << "&#10;";

			label += notice->GetLabel ();
			i++;
		}

		label << "</Label>";
		text << label << endLine;

		// Now add list of notices...

		mNoticeObjectIterator.Reset ();

		while (nextNotice = (Notice*) mNoticeObjectIterator ()) {

			msgNum = Notice::GetNextMessageNumber ();
			nextNotice->SetMessageNumber (msgNum);
			text << indent << "\t<MessageNumber>" << msgNum << "</MessageNumber>" << endLine;
			tempText << "\t\t<Message>\n";
			tempText << "\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
			tempText << "\t\t\t<Index>" << nextNotice->GetID () << "</Index>\n";
			tempText << "\t\t\t<Text>" << (nextNotice->AssembleString (" ")).GetData () << "</Text>\n";
			tempText << "\t\t</Message>\n";
		}

		// Now add list of alleles

		hasThreeLoci = (mLocus != NULL) && (mLeftLocus != NULL) && (mRightLocus != NULL);

		if (mLocus != NULL) {

			if ((mLeftLocus == NULL) && (mRightLocus == NULL))
				needLocus0 = true;

			else
				needLocus0 = false;
		}

		else
			needLocus0 = false;

		needLocus0 = (!hasThreeLoci) && ((mLocus != mLeftLocus) || (mLocus != mRightLocus));

		if (needLocus0) {

			suffix = GetAlleleName (0);

			if ((suffix.Length () > 0) || (virtualAllele.Length () > 0)) {

				text << indent << "\t<Allele>" << endLine;

				if (suffix.Length () > 0)
					text << indent << "\t\t<Name>" << suffix << "</Name>" << endLine;

				else
					text << indent << "\t\t<Name>" << virtualAllele << "</Name>" << endLine;

				if (mOffGrid)
					suffix = "true";

				else if (mAcceptedOffGrid)
					suffix = "accepted";

				else
					suffix = "false";

				text << indent << "\t\t<OffLadder>" << suffix << "</OffLadder>" << endLine;
				text << indent << "\t\t<BPS>" << GetBioID (0) << "</BPS>" << endLine;
				text << indent << "\t\t<Locus>" << mLocus->GetLocusName () << "</Locus>" << endLine;
				text << indent << "\t\t<Location>0</Location>" << endLine;
				text << indent << "\t</Allele>" << endLine;
			}
		}

		if (mLeftLocus != NULL) {

			if (mAlleleNameLeft.Length () > 0) {

				text << indent << "\t<Allele>" << endLine;
				text << indent << "\t\t<Name>" << mAlleleNameLeft << "</Name>" << endLine;

				if (mIsOffGridLeft)
					suffix = "true";

				else if (GetMessageValue (acceptedOLLeft))
					suffix = "accepted";

				else
					suffix = "false";

				text << indent << "\t\t<OffLadder>" << suffix << "</OffLadder>" << endLine;
				text << indent << "\t\t<BPS>" << GetBioID (-1) << "</BPS>" << endLine;
				text << indent << "\t\t<Locus>" << mLeftLocus->GetLocusName () << "</Locus>" << endLine;
				text << indent << "\t\t<Location>-1</Location>" << endLine;
				text << indent << "\t</Allele>" << endLine;
			}
		}

		if (mRightLocus != NULL) {

			if (mAlleleNameRight.Length () > 0) {

				text << indent << "\t<Allele>" << endLine;
				text << indent << "\t\t<Name>" << mAlleleNameRight << "</Name>" << endLine;

				if (mIsOffGridRight)
					suffix = "true";

				else if (GetMessageValue (acceptedOLRight))
					suffix = "accepted";

				else
					suffix = "false";

				text << indent << "\t\t<OffLadder>" << suffix << "</OffLadder>" << endLine;
				text << indent << "\t\t<BPS>" << GetBioID (1) << "</BPS>" << endLine;
				text << indent << "\t\t<Locus>" << mRightLocus->GetLocusName () << "</Locus>" << endLine;
				text << indent << "\t\t<Location>1</Location>" << endLine;
				text << indent << "\t</Allele>" << endLine;
			}
		}

		text << indent << "</" + bracketTag << ">" << endLine;
	}

	text.ResetOutputLevel ();
}


size_t DataSignal :: StoreSize () const {

	return RGPersistent::StoreSize () + 3 * sizeof (double);
}


int DataSignal :: CompareTo (const RGPersistent* p) const {

	return -1;
}


Boolean DataSignal :: IsEqualTo (const RGPersistent* p) const {

	return FALSE;
}


void DataSignal :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	f.Read (Left);
	f.Read (Right);
	f.Read (Fit);
	LeftSearch = Left;
	RightSearch = Right;
}


void DataSignal :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
	f >> Left;
	f >> Right;
	f >> Fit;
	LeftSearch = Left;
	RightSearch = Right;
}


void DataSignal :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	f.Write (Left);
	f.Write (Right);
	f.Write (Fit);
}


void DataSignal :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
	f << Left;
	f << Right;
	f << Fit;
}


SampledData :: SampledData () : DataSignal (), NumberOfSamples (0), Measurements (NULL), norm2 (0.0), mDeleteArray (true), mNoiseRange (0.0) {

	Spacing = DataSignal::GetSampleSpacing ();
	NoiseThreshold = TracePrequalification::GetNoiseThreshold ();
	PeakIterator = new RGDListIterator (PeakList);
	NoiseIterator = new RGDListIterator (NoiseList);
	slr = new SpecialLinearRegression (50);
}


SampledData :: SampledData (int numSamples, double left, double right, double* samples, bool deleteArray) : DataSignal (left, right),
NumberOfSamples (numSamples), Measurements (samples), norm2 (0.0), mDeleteArray (deleteArray), mNoiseRange (0.0) {

	Spacing = DataSignal::GetSampleSpacing ();
	NoiseThreshold = TracePrequalification::GetNoiseThreshold ();
	PeakIterator = new RGDListIterator (PeakList);
	NoiseIterator = new RGDListIterator (NoiseList);
	slr = new SpecialLinearRegression (50);
}


SampledData :: SampledData (const DataSignal& base, const DataSignal& second, double baseCoeff, double left, double right) : DataSignal (left, right),
norm2 (0.0), mDeleteArray (true), mNoiseRange (0.0) {

	Spacing = DataSignal::GetSampleSpacing ();
	NoiseThreshold = TracePrequalification::GetNoiseThreshold ();
	PeakIterator = new RGDListIterator (PeakList);
	NoiseIterator = new RGDListIterator (NoiseList);

	NumberOfSamples = (int)floor (((right - left) / Spacing) + 0.5);
	Measurements = new double [NumberOfSamples];
	double abscissa;

	for (int i=0; i<NumberOfSamples; i++) {

		abscissa = left + i * Spacing;
		Measurements [i] = second.Value (abscissa) - baseCoeff * base.Value (abscissa);
	}

	slr = new SpecialLinearRegression (50);
}


SampledData :: SampledData (const SampledData& sd) : DataSignal (sd.Left, sd.Right), NumberOfSamples (sd.NumberOfSamples), norm2 (sd.norm2), 
mDeleteArray (true), mNoiseRange (sd.mNoiseRange) {

	Spacing = DataSignal::GetSampleSpacing ();
	NoiseThreshold = TracePrequalification::GetNoiseThreshold ();
	PeakIterator = new RGDListIterator (PeakList);
	NoiseIterator = new RGDListIterator (NoiseList);

	Measurements = new double [NumberOfSamples];

	for (int i=0; i<NumberOfSamples; i++)
		Measurements [i] = sd.Measurements [i];

	slr = new SpecialLinearRegression (50);
}


SampledData :: ~SampledData () {

	if (mDeleteArray)
		delete[] Measurements;

	PeakList.ClearAndDelete ();
	NoiseList.ClearAndDelete ();
	delete PeakIterator;
	delete NoiseIterator;
	delete slr;
}


RGString SampledData :: GetSignalType () const {

	return RGString ("SampledData");
}


void SampledData :: SetDisplacement (double disp) {}


void SampledData :: SetScale (double scale) {}


double SampledData :: Value (double x) const {

	if ((x < Left) || (x > Right))
		return 0.0;

	if (x == Right)
		return Measurements [NumberOfSamples - 1];
	
	double location = (x - Left) / Spacing;
	int interval = (int)floor (location);

	double lambda = (location - interval) / Spacing;

	return Measurements [interval] * (1.0 - lambda) + Measurements [interval + 1] * lambda;
}


double SampledData :: Value (int n) const {

	if ((n < 0) || (n >= NumberOfSamples))
		return 0.0;

	return Measurements [n];
}


int SampledData :: AddToSample (double* sample, double sampleLeft, double sampleRight) const {

	// We assume, and it had better be true (!!!), that array sample is generated on the same
	// spacing and with a common start point to this SampledData
	
	int LocalStart;
	int SampleStart;
	int Count;

	if (sampleLeft >= Left) {

		SampleStart = 0;
		LocalStart = (int)floor (((sampleLeft - Left) / Spacing) + 0.001);
	}

	else {

		LocalStart = 0;
		SampleStart = (int)floor (((Left - sampleLeft) / Spacing) + 0.001);
	}

	Count = (int) floor (((sampleRight - sampleLeft) / Spacing) + 1.001);

	double* m = Measurements + LocalStart;
	double* s = sample + SampleStart;

	double* MEnd = m + Count;
	double* MeasurementEnd = Measurements + NumberOfSamples;

	if (MEnd > MeasurementEnd)
		MEnd = MeasurementEnd;

	for (m = Measurements + LocalStart; m < MEnd; m++) {

		*m += *s;
		s++;
	}

	return Count;
}


double SampledData :: ValueFreeBound (int n) const {

	return Measurements [n];
}


double SampledData :: ValueFreeBound (double x) const {

	double location = (x - Left) / Spacing;
	int interval = (int)floor (location);

	double lambda = (location - interval) / Spacing;

	return Measurements [interval] * lambda + Measurements [interval + 1] * (1.0 - lambda);
}


const double* SampledData :: GetData () const {

	return Measurements;
}


int SampledData :: GetNumberOfSamples () const {

	return NumberOfSamples;
}


void SampledData :: RestrictToMaximum (double MaxValue) {

	for (int i=0; i<NumberOfSamples; i++) {

		if (Measurements [i] > MaxValue)
			Measurements [i] = MaxValue;
	}
}


int SampledData :: FindAndRemoveFixedOffset () {

	double minB;
	double ave;
	double currentAve;
	double temp;
	double* CurrentPtr = Measurements + (NumberOfSamples - 1);
	int MaxTests = 8;
	double noiseRange = 0.0;
	minB = fabs (slr->RegressBackwardFrom (CurrentPtr, currentAve, mNoiseRange));
	CurrentPtr -= 25;

	if (minB < -99999.0)
		return -1;

	for (int i=1; i<MaxTests; i++) {

		temp = fabs (slr->RegressBackwardFrom (CurrentPtr, ave, noiseRange));

		if (temp < minB) {

			minB = temp;
			currentAve = ave;

//			if (noiseRange > mNoiseRange)
				mNoiseRange = noiseRange;
		}

		CurrentPtr -= 25;
	}

//	cout << "Minimum b = " << minB << endl;
	double* endPtr = Measurements + NumberOfSamples;

	for (CurrentPtr=Measurements; CurrentPtr<endPtr; CurrentPtr++)
		*CurrentPtr -= currentAve;

	return 0;
}


double* SampledData :: CreateMovingAverageFilteredArray (int window, double* inputArray) {

	double* newMeasurements = new double [NumberOfSamples];
	int i;
	double temp;
	double factor;
	int win = window;
	int halfWindow;

	if (window > 0) {

		if (win%2 == 0)
			win++;

		factor = 1.0 / (double)win;
	}

	else {

		win = 1;
		factor = 1.0;
	}

	halfWindow = win / 2;
	temp = 0.0;
	double currentAve;

	for (i=0; i<=halfWindow; i++) {

		temp += inputArray [i];
	}

	currentAve = newMeasurements [0] = temp * factor;
	double oldValue;
	double nextValue;
	int indexNext = halfWindow;
	int indexOld = -halfWindow - 1;

	for (i=1; i<NumberOfSamples; i++) {

		indexNext++;
		indexOld++;

		if (indexNext < NumberOfSamples)
			nextValue = inputArray [indexNext];

		else
			nextValue = 0.0;

		if (indexOld >= 0)
			oldValue = inputArray [indexOld];

		else
			oldValue = 0.0;

		currentAve += factor * (nextValue - oldValue);
		newMeasurements [i] = currentAve;
	}

	//for (i=0; i<NumberOfSamples; i++) {

	//	temp = 0.0;

	//	for (j=-halfWindow; j<=halfWindow; j++) {

	//		index = i + j;

	//		if (index < 0)
	//			continue;

	//		if (index >= NumberOfSamples)
	//			break;

	//		temp += Measurements [index];
	//	}

	//	newMeasurements [i] = temp * factor;
	//}

	return newMeasurements;
}


DataSignal* SampledData :: CreateMovingAverageFilteredSignal (int window) {

	double* newMeasurements = new double [NumberOfSamples];
	int i;
	double temp;
	double factor;
	int win = window;
	int halfWindow;

	if (window > 0) {

		if (win%2 == 0)
			win++;

		factor = 1.0 / (double)win;
	}

	else {

		win = 1;
		factor = 1.0;
	}

	halfWindow = win / 2;
	temp = 0.0;
	double currentAve;

	for (i=0; i<=halfWindow; i++) {

		temp += Measurements [i];
	}

	currentAve = newMeasurements [0] = temp * factor;
	double oldValue;
	double nextValue;
	int indexNext = halfWindow;
	int indexOld = -halfWindow - 1;

	for (i=1; i<NumberOfSamples; i++) {

		indexNext++;
		indexOld++;

		if (indexNext < NumberOfSamples)
			nextValue = Measurements [indexNext];

		else
			nextValue = 0.0;

		if (indexOld >= 0)
			oldValue = Measurements [indexOld];

		else
			oldValue = 0.0;

		currentAve += factor * (nextValue - oldValue);
		newMeasurements [i] = currentAve;
	}

	//for (i=0; i<NumberOfSamples; i++) {

	//	temp = 0.0;

	//	for (j=-halfWindow; j<=halfWindow; j++) {

	//		index = i + j;

	//		if (index < 0)
	//			continue;

	//		if (index >= NumberOfSamples)
	//			break;

	//		temp += Measurements [index];
	//	}

	//	newMeasurements [i] = temp * factor;
	//}

	DataSignal* filteredSignal = new SampledData (NumberOfSamples, Left, Right, newMeasurements, true);
	return filteredSignal;
}


DataSignal* SampledData :: CreateThreeMovingAverageFilteredSignal (int minWindow) {

	//
	//  First find the three windows assuming input window is minimum.  Then create temporary filter arrays and perform three filterings.  Clean up all but last array and 
	//  create filtered signal, as above, and return
	//

	if (minWindow == 1)
		return CreateMovingAverageFilteredSignal (minWindow);

	int win3 = minWindow;

	if (win3%2 == 0)
		win3++;

	double temp = 1.4303 * (double)win3;
	int high = (int)ceil (temp);
	int low = (int)floor (temp);
	int win2, win1;

	if (high%2 == 0) {

		if (low%2 == 0)
			win2 = low + 1;

		else
			win2 = low;
	}

	else
		win2 = high;

	temp = 1.4303 * (double)win2;
	high = (int)ceil (temp);
	low = (int)floor (temp);

	if (high%2 == 0) {

		if (low%2 == 0)
			win1 = low + 1;

		else
			win1 = low;
	}

	else
		win1 = high;

	//cout << "Filter windows = " << win1 << ", " << win2 << ", and " << win3 << endl;

	double* filterOut1 = CreateMovingAverageFilteredArray (win1, Measurements);
	double* filterOut2 = CreateMovingAverageFilteredArray (win2, filterOut1);
	double* filterOut3 = CreateMovingAverageFilteredArray (win3, filterOut2);
	delete[] filterOut1;
	delete[] filterOut2;

	DataSignal* filteredSignal = new SampledData (NumberOfSamples, Left, Right, filterOut3, true);
	return filteredSignal;
}


DataSignal* SampledData :: Project (double left, double right) const {
	
	int nleft, nright;

	nleft = GetSampleNumber (left);
	nright = GetSampleNumber (right);

	if (nright <= nleft)
		return NULL;

	int NewNumberOfSamples = nright - nleft + 1;
	double* NewSample = new double [NewNumberOfSamples];

	for (int i=0; i<NewNumberOfSamples; i++)
		NewSample [i] = Measurements [nleft + i];

	double l = Left + nleft * Spacing;
	double r = Left + nright * Spacing;

	DataSignal* NewSignal = new SampledData (NewNumberOfSamples, l, r, NewSample);
	return NewSignal;
}


DataSignal* SampledData :: Project (const DataSignal* target) const {

	double left = target->LeftEndPoint ();
	double right = target->RightEndPoint ();
	
	return Project (left, right);
}


void SampledData :: ResetCharacteristicsFromRight (TracePrequalification& trace, RGTextOutput& text, double minRFU, Boolean print) {

	DataSignal::ResetCharacteristicsFromRight (trace, text, minRFU, print);
	PeakList.ClearAndDelete ();
	NoiseList.ClearAndDelete ();
	trace.ResetSearch (this, NumberOfSamples, mNoiseRange);
	DataInterval* NextDataInterval;
	NoiseInterval* NextNoiseInterval;
	DataInterval* PreviousDataInterval = NULL;
	double minFraction = SampledData::GetPeakFractionForFlatCurveTest ();
	double absMaxTolerance = SampledData::GetPeakLevelForFlatCurveTest ();
	bool ignoreNoise = SampledData::GetIgnoreNoiseAnalysisAboveDetectionInSmoothing ();

	double leftMin;
	double rightMin;
	double maxAtMode;
	double minFractionTimesMax;
	double upperLimit;
	double upperTolerance;
	double noiseRange = 1.5 * mNoiseRange;
	int leftTime;

	while (NextDataInterval = trace.GetNextDataIntervalWithPrecomputedConvolution (NextNoiseInterval, text, minRFU, PreviousDataInterval, print)) {

		leftMin = NextDataInterval->GetLeftMinimum ();
		rightMin = NextDataInterval->GetRightMinimum ();
		maxAtMode = NextDataInterval->GetMaxAtMode ();
		NoiseList.Append (NextNoiseInterval);
		leftTime = NextDataInterval->GetLeft ();

		if ((maxAtMode - leftMin < noiseRange) && (maxAtMode - rightMin < noiseRange)) {

			if (!ignoreNoise || (ignoreNoise && (maxAtMode < minRFU))) {

				delete NextDataInterval;
				NextDataInterval = NULL;
				continue;
			}
		}

		NextDataInterval->ReassessRelativeMinimaGivenNoise (mNoiseRange, (SampledData*)this);

		//
		//	Add test for noise correction to relative minima here...********07/01/2014********
		//

		PeakList.Append (NextDataInterval);

		if (PreviousDataInterval != NULL) {

			if (PreviousDataInterval->HasFixedLeftEndPoint ())
				NextDataInterval->SetFixedRightTrue ();
		}

		minFractionTimesMax = minFraction * maxAtMode;
		upperTolerance = absMaxTolerance;

		upperLimit = maxAtMode - upperTolerance;

		if ((leftMin > minFractionTimesMax) && (leftMin < upperLimit))
			NextDataInterval->SetFixedLeftTrue ();

		if ((rightMin > minFractionTimesMax) && (rightMin < upperLimit)) {

			NextDataInterval->SetFixedRightTrue ();

			if (PreviousDataInterval != NULL)
				PreviousDataInterval->SetFixedLeftTrue ();
		}

		PreviousDataInterval = NextDataInterval;
	}

	//
	//	Add algorithm to "stitch together" adjacent data intervals as appropriate here...12/18/2014
	// create DataInterval constructor that takes two DataInterval objects and merges them, thus replacing
	// both of the merged DataIntervals in the PeakList.
	//

	if (NextNoiseInterval != NULL)
		NoiseList.Append (NextNoiseInterval);

	norm2 = trace.GetNorm ();
	PeakIterator->Reset ();
	NoiseIterator->Reset ();
	TraceWindowSize = trace.GetWindowWidth ();
}


void SampledData :: ResetCharacteristicsFromLeft (TracePrequalification& trace, RGTextOutput& text, double minRFU, Boolean print) {

	DataSignal::ResetCharacteristicsFromLeft (trace, text, minRFU, print);
	PeakList.ClearAndDelete ();
	NoiseList.ClearAndDelete ();
	trace.ResetSearch (this, NumberOfSamples, mNoiseRange);
	DataInterval* NextDataInterval;
	NoiseInterval* NextNoiseInterval;
	DataInterval* PreviousDataInterval = NULL;
	double minFraction = SampledData::GetPeakFractionForFlatCurveTest ();
	double maxFraction = 0.10;
	double absMaxTolerance = SampledData::GetPeakLevelForFlatCurveTest ();

	double leftMin;
	double rightMin;
	double maxAtMode;
	double minFractionTimesMax;
	double maxFractionTimesMax;
	double upperLimit;
	double upperTolerance;
	double noiseRange = 1.5 * mNoiseRange;

	while (NextDataInterval = trace.GetNextDataInterval (NextNoiseInterval, text, minRFU, PreviousDataInterval, print)) {

		leftMin = NextDataInterval->GetLeftMinimum ();
		rightMin = NextDataInterval->GetRightMinimum ();
		maxAtMode = NextDataInterval->GetMaxAtMode ();
		
		NoiseList.Prepend (NextNoiseInterval);

		if ((maxAtMode - leftMin < noiseRange) && (maxAtMode - rightMin < noiseRange)) {

			delete NextDataInterval;
			NextDataInterval = NULL;
			continue;
		}

		PeakList.Prepend (NextDataInterval);

		if (PreviousDataInterval != NULL) {

			if (PreviousDataInterval->HasFixedRightEndPoint ())
				NextDataInterval->SetFixedLeftTrue ();
		}

		minFractionTimesMax = minFraction * maxAtMode;
		maxFractionTimesMax = maxFraction * maxAtMode;

//		if (maxFractionTimesMax > absMaxTolerance)
			upperTolerance = absMaxTolerance;

//		else
//			upperTolerance = maxFractionTimesMax;

		upperLimit = maxAtMode - upperTolerance;

		if ((rightMin > minFractionTimesMax) && (rightMin < upperLimit))
			NextDataInterval->SetFixedRightTrue ();

		if ((leftMin > minFractionTimesMax) && (leftMin < upperLimit)) {

			NextDataInterval->SetFixedLeftTrue ();

			if (PreviousDataInterval != NULL)
				PreviousDataInterval->SetFixedRightTrue ();
		}

		PreviousDataInterval = NextDataInterval;
	}

	if (NextNoiseInterval != NULL)
		NoiseList.Prepend (NextNoiseInterval);

	norm2 = trace.GetNorm ();
	PeakIterator->Reset ();
	NoiseIterator->Reset ();
	TraceWindowSize = trace.GetWindowWidth ();
}


DataSignal* SampledData :: FindNextCharacteristicFromRight (const DataSignal& Signature, double& fit, RGDList& previous) {
	
	DataInterval* nextInterval = (DataInterval*)(*PeakIterator) ();

	if (nextInterval == NULL)
		return NULL;

	nextInterval->AddSideValues (this);
	DataSignal* value = NULL;

	while (value == NULL) {

		value = Signature.FindCharacteristic (this, nextInterval, TraceWindowSize, fit, previous);

		if (value != NULL) {

			if (value->GetStandardDeviation () < 0.14) {

				delete value;
				value = new SpikeSignal (nextInterval->GetMode (), nextInterval->GetMaxAtMode (), 0.0, 0.0);
				value->SetCurveFit (1.0);
				fit = 1.0;
			}

			value->SetDataMode (nextInterval->GetMaxAtMode ());
			break;
		}

		nextInterval = (DataInterval*)(*PeakIterator) ();

		if (nextInterval == NULL)
			return NULL;
	}

	return value;
}



DataSignal* SampledData :: FindNextCharacteristicFromLeft (const DataSignal& Signature, double& fit, RGDList& previous) {
	
	DataInterval* nextInterval = (DataInterval*)(*PeakIterator) ();

	if (nextInterval == NULL)
		return NULL;

	nextInterval->AddSideValues (this);
	DataSignal* value = Signature.FindCharacteristic (this, nextInterval, TraceWindowSize, fit, previous);

	if (value != NULL)
		value->SetDataMode (nextInterval->GetMaxAtMode ());

	return value;
}


double SampledData :: GetModeHeightAndLocationFromDataInterval (int& location) {

	DataInterval* nextInterval = (DataInterval*)PeakIterator->CurrentItem ();

	if (nextInterval == NULL) {

		location = 0;
		return 0.0;
	}

	location = (int) floor (nextInterval->GetMode () + 0.5);
	return nextInterval->GetMaxAtMode ();
}


DataInterval* SampledData :: GetCurrentIntervalFromList () {

	return (DataInterval*) PeakIterator->CurrentItem ();
}


const DataSignal* SampledData :: FindCharacteristicBetweenTwoPeaks (DataSignal* prevSignal, DataSignal* nextSignal, const DataSignal& signature, double& fit, double detectionThreshold, double analysisThreshold, double noiseThreshold) {

	int startTime = (int) ceil (prevSignal->GetMean () + 1.0);
	int endTime = (int) floor (nextSignal->GetMean () - 1.0);
	double maxDifference = 0.0;
	int mode = 0;
	int i;
	double maxFunctionValue;
	double minValue;
	double temp;
	double prevHalfWidth = 0.5 * prevSignal->GetWidth ();
	double nextHalfWidth = 0.5 * nextSignal->GetWidth ();
	int lowerLimit = (int) ceil (prevSignal->GetMean () + prevHalfWidth) - 1;
	int upperLimit = (int) floor (nextSignal->GetMean () - nextHalfWidth) + 1;

	//for (i=startTime; i<=endTime; i++) {

	//	maxFunctionValue = prevSignal->Value ((double)i);
	//	temp = nextSignal->Value ((double)i);

	//	if (temp > maxFunctionValue)
	//		maxFunctionValue = temp;

	//	temp = Value (i) - maxFunctionValue;

	//	if (temp > maxDifference) {

	//		maxDifference = temp;
	//		mode = i;
	//	}
	//}

	//if (maxDifference <= 0.0)
	//	return NULL;

	//if (maxDifference < analysisThreshold)
	//	return NULL;

	//for (i=mode; i<=endTime; i++) {

	//	maxFunctionValue = prevSignal->Value ((double)i);
	//	temp = nextSignal->Value ((double)i);

	//	if (temp > maxFunctionValue)
	//		maxFunctionValue = temp;

	//	temp = Value (i) - maxFunctionValue;

	//	if (temp <= 0.0) {

	//		endTime = i - 1;
	//		break;
	//	}
	//}

	//for (i=mode; i>=startTime; i--) {

	//	maxFunctionValue = prevSignal->Value ((double)i);
	//	temp = nextSignal->Value ((double)i);

	//	if (temp > maxFunctionValue)
	//		maxFunctionValue = temp;

	//	temp = Value (i) - maxFunctionValue;

	//	if (temp <= 0.0) {

	//		startTime = i + 1;
	//		break;
	//	}
	//}

	maxFunctionValue = 0.0;
	int halfTime = (endTime - startTime) / 2;
	bool foundLocalMax = false;

	// Concave down search variables:
	bool foundConcaveDown = false;
	int numberPtsConcaveDown = 0;
	bool foundEnoughPtsConcaveDown = false;
	int enoughConcaveDownPoints = DataSignal::NumberOfIntervalsForConcaveDownAlgorithm;
	double temp0;
	double temp1;
	list<ConcaveDownSet*> CDList;
	ConcaveDownSet* nextSet = NULL;
	ConcaveDownSet* bestSet = NULL;
	double tempMax;
	int tempPos;
	double totalMaxValue = 0.0;

	for (i=startTime+1; i<endTime; i++) {

		// search for true mode and if too near end points, quit

		temp = Value (i);
		temp0 = Value (i - 1);
		temp1 = Value (i + 1);

		if ((temp >= temp0) && (temp >= temp1) && (temp > maxFunctionValue)) {

			if ((i >= lowerLimit) && (i <= upperLimit)) {

				maxFunctionValue = temp;
				mode = i;
				foundLocalMax = true;
			}
		}

		if (2.0 * temp >= temp0 + temp1) {

			// Concave down at this point

			if (!foundConcaveDown) {

				foundConcaveDown = true;
				numberPtsConcaveDown = 1;
				tempMax = temp;
				tempPos = i;

				if (temp0 > tempMax) {

					tempMax = temp0;
					tempPos = i - 1;
				}

				if (temp1 > tempMax) {

					tempMax = temp1;
					tempPos = i + 1;
				}

				nextSet = new ConcaveDownSet (i - 1, i + 1, tempMax, tempPos);
			}

			else {

				numberPtsConcaveDown++;
				tempMax = nextSet->mMaxValue;

				if (temp1 > tempMax) {

					nextSet->mMaxValue = temp1;
					nextSet->mPosition = i + 1;
				}

				nextSet->mEnd = i + 1;
			}
		}

		else if (foundConcaveDown) {

			if (numberPtsConcaveDown >= enoughConcaveDownPoints) {

				if (nextSet->mStart < lowerLimit) {

					delete nextSet;
					nextSet = NULL;
					numberPtsConcaveDown = 0;
					foundConcaveDown = false;
					continue;
				}

				if (nextSet->mEnd > upperLimit) {

					delete nextSet;
					nextSet = NULL;
					numberPtsConcaveDown = 0;
					foundConcaveDown = false;
					continue;
				}

				CDList.push_back (nextSet);

				if (nextSet->mMaxValue > totalMaxValue) {

					totalMaxValue = nextSet->mMaxValue;
					bestSet = nextSet;
					foundEnoughPtsConcaveDown = true;
				}

				numberPtsConcaveDown = 0;
				foundConcaveDown = false;
				nextSet = NULL;
			}

			else {

				delete nextSet;
				nextSet = NULL;
				numberPtsConcaveDown = 0;
				foundConcaveDown = false;
			}
		}
	}

	if (foundConcaveDown) {

		if (numberPtsConcaveDown >= enoughConcaveDownPoints) {

			if (nextSet->mStart < lowerLimit) {

				delete nextSet;
				nextSet = NULL;
				numberPtsConcaveDown = 0;
				foundConcaveDown = false;
			}

			else if (nextSet->mEnd > upperLimit) {

				delete nextSet;
				nextSet = NULL;
				numberPtsConcaveDown = 0;
				foundConcaveDown = false;
			}

			else {

				CDList.push_back (nextSet);

				if (nextSet->mMaxValue > totalMaxValue) {

					totalMaxValue = nextSet->mMaxValue;
					bestSet = nextSet;
					foundEnoughPtsConcaveDown = true;
				}

				foundConcaveDown = false;
				nextSet = NULL;
			}
		}

		else {

			delete nextSet;
			nextSet = NULL;
			numberPtsConcaveDown = 0;
			foundConcaveDown = false;
		}
	}

	double maxAtMode = maxFunctionValue;
	bool noShoulder1 = (maxFunctionValue < analysisThreshold);
	bool noShoulder2 = (totalMaxValue < analysisThreshold);

	if (noShoulder1 && noShoulder2) {

		while (!CDList.empty ()) {

			nextSet = CDList.front ();
			CDList.pop_front ();
			delete nextSet;
		}

		return NULL;
	}

	if (noShoulder1 || (mode <= startTime) || (mode >= endTime)) {

		if (CDList.empty ())
			return NULL;

		if (noShoulder2) {

			while (!CDList.empty ()) {

				nextSet = CDList.front ();
				CDList.pop_front ();
				delete nextSet;
			}

			return NULL;
		}

		while (!CDList.empty ()) {

			nextSet = CDList.front ();
			CDList.pop_front ();

			if (nextSet != bestSet)
				delete nextSet;
		}

		if (bestSet == NULL)
			return NULL;

		mode = bestSet->mPosition;
		maxAtMode = bestSet->mMaxValue + 0.000001;
		delete bestSet;

		if ((mode - startTime < 5) || (endTime - mode < 5))
			return NULL;
	}

	maxFunctionValue = prevSignal->Value ((double)mode);
	temp = nextSignal->Value ((double)mode);

	if (temp > maxFunctionValue)
		maxFunctionValue = temp;

	if (maxAtMode - maxFunctionValue < noiseThreshold)
		return NULL;
	
	minValue = maxAtMode;
	int minTime = startTime;

	for (i=startTime; i<=mode; i++) {

		temp = Value (i);

		if (temp < minValue) {

			minValue = temp;
			minTime = i;
		}
	}

	startTime = minTime;
	minValue = maxAtMode;
	minTime = mode;

	for (i=mode; i<=endTime; i++) {

		temp = Value (i);

		if (temp < minValue) {

			minValue = temp;
			minTime = i;
		}
	}

	endTime = minTime;

	if (endTime - startTime < 3)
		return NULL;

	//if (endTime - startTime <= 3) {

	//	// This is a spike...maybe we should ignore it?  Or try to fit?
	//}

	// Create a DataInterval using this start and end time and try to fit various curves

	int channel = prevSignal->GetChannel ();
	//int centerTime = (startTime + endTime) / 2;
	int centerTime = mode;
	DataInterval* shoulderInterval = new DataInterval (startTime, centerTime, endTime);
	shoulderInterval->SetMode (mode);
	shoulderInterval->SetMaxAtMode (Value (mode));
	shoulderInterval->AddSideValues (this);

	int unUsedWindowSize = 0;
	RGDList unUsedList;

	DataSignal* newSignal = signature.FindCharacteristicAsymmetric (this, shoulderInterval, unUsedWindowSize, fit, unUsedList);

	if (newSignal == NULL)
		return NULL;

	if ((fit > 1.0000001) || (fit < 0.85)) {

		delete newSignal;
		return NULL;
	}

	double sigma = newSignal->GetStandardDeviation ();
	double height = newSignal->Peak ();

	if (ISNAN (sigma) || ISNAN (height) || (sigma == numeric_limits<double>::infinity()) || (height == numeric_limits<double>::infinity())) {

		delete newSignal;
		return NULL;
	}

	if ((sigma > 2.0 * nextSignal->GetStandardDeviation ()) || (sigma > 2.0 * prevSignal->GetStandardDeviation ())) {

		delete newSignal;
		return NULL;
	}

	DataSignal* copySignal = newSignal->MakeCopy (newSignal->GetMean ());
	copySignal->SetCurveFit (fit);
	copySignal->SetCurrentDataInterval (NULL);
	delete newSignal;

	if (copySignal != NULL)
		copySignal->SetDataMode (maxAtMode);

	delete shoulderInterval;
	return copySignal;
}


DataSignal* SampledData :: FindNextCharacteristicRetry (const DataSignal& Signature, double& fit, RGDList& previous) {

	DataInterval* nextInterval = (DataInterval*)PeakIterator->CurrentItem ();

	if (nextInterval == NULL)
		return NULL;

	DataSignal* value = Signature.FindCharacteristic (this, nextInterval, TraceWindowSize, fit, previous);

	if (value != NULL) {

		double sigma = value->GetStandardDeviation ();
		double height = value->Peak ();
		double mean = value->GetMean ();

		//if (ISNAN (sigma) || ISNAN (height) || (sigma == numeric_limits<double>::infinity()) || (height == numeric_limits<double>::infinity()) || (sigma < 0.0) || (mean >= NumberOfSamples) || (sigma > 0.2 * (double)NumberOfSamples)) {

		//	if (mean >= NumberOfSamples)
		//		cout << "Found a bad peak on channel " << mChannel << ":  Data in interval (" << nextInterval->GetLeft () << ", " << nextInterval->GetRight () << ") with mean = " << mean << " and sigma = " << sigma << "\n";

		//	delete value;
		//	return NULL;
		//}

		if (sigma < 0.14) {

			delete value;
			value = new SpikeSignal (nextInterval->GetMode (), nextInterval->GetMaxAtMode (), 0.0, 0.0);
			value->SetCurveFit (1.0);
			fit = 1.0;
		}

		value->SetDataMode (nextInterval->GetMaxAtMode ());
	}

	return value;
}


double SampledData :: TestConstantCharacteristicRetry () {

	DataInterval* nextInterval = (DataInterval*)PeakIterator->CurrentItem ();

	if (nextInterval == NULL)
		return 0.0;

	double ans = InnerProductWithConstantFunction (nextInterval->GetLeft (), nextInterval->GetRight ());
	return ans;
}


double SampledData :: InnerProductWithConstantFunction (int left, int right) const {

	int N = right - left;
	double sum = 0.0;
	double sum2 = 0.0;
	int i;
	int right1 = right - 1;
	double temp;

	for (i=left+1; i<right; i++) {

		temp = Value (i);
		sum += temp;
		sum2 += temp * temp;
	}

	temp = Value (left);
	sum += 0.5 * temp;
	sum2 += 0.5 * temp * temp;

	temp = Value (right);
	sum += 0.5 * temp;
	sum2 += 0.5 * temp * temp;

	sum2 = sqrt (sum2);  // 2-norm of sample
	double oneNorm = sqrt ((double)N);
	double sampleSpacing = DataSignal::GetSampleSpacing ();
	
	if ((sum2 > 0.0) && (oneNorm > 0.0))
		return sampleSpacing * sum / (sum2 * oneNorm);

	return 0.0;
}


double SampledData :: TestConstantCharacteristicRetry (double& height, int& left, int& right) {

	DataInterval* nextInterval = (DataInterval*)PeakIterator->CurrentItem ();

	if (nextInterval == NULL)
		return 0.0;

	left = nextInterval->GetLeft ();
	right = nextInterval->GetRight ();

	double ans = InnerProductWithConstantFunction (nextInterval->GetLeft (), nextInterval->GetRight (), height);
	return ans;
}


double SampledData :: TestConstantCharacteristicRetry (DataSignal* currentSignal, double& height, int& left, int& right) {

	DataInterval* nextInterval = currentSignal->GetCurrentDataInterval ();

	if (nextInterval == NULL)
		return 0.0;

	left = nextInterval->GetLeft ();
	right = nextInterval->GetRight ();

	double ans = InnerProductWithConstantFunction (nextInterval->GetLeft (), nextInterval->GetRight (), height);
	return ans;
}


bool SampledData :: HasAtLeastOneLocalMinimum () {

	DataInterval* nextInterval = (DataInterval*)PeakIterator->CurrentItem ();

	if (nextInterval == NULL)
		return false;

	if (nextInterval->GetNumberOfMinima () > 0)
		return true;

	return false;
}


bool SampledData :: TestForBiasedFit (const DataSignal* currentSignal, double limit) {

	DataInterval* nextInterval = (DataInterval*)PeakIterator->CurrentItem ();
	int intLeft = nextInterval->GetLeft ();
	int intRight = nextInterval->GetRight ();
	double value = currentSignal->Value ((double)intRight);
	//double minDisp = Measurements [intRight] - value;
	double maxDisp = 0.0;
	double currentDisp;
	//int locMin;
	int locMax;
	locMax = intRight;

	int i;
	double mu = currentSignal->GetMean ();
	double ratio = (mu - (double)intLeft) / (double)(intRight - intLeft);

	//if ((ratio > 0.45) && (ratio < 0.55))
	//	return false;

	double sigma = currentSignal->GetStandardDeviation ();
	double leftUpper = mu - sigma;
	double rightLower = mu + sigma;

	if (ratio >= 0.62) {

		for (i=intLeft; i<leftUpper; i++) {

			currentDisp = Measurements [i] - currentSignal->Value ((double)i);

			if (currentDisp > maxDisp) {

				locMax = i;
				maxDisp = currentDisp;
			}
		}
	}

	else if (ratio <= 0.38) {

		for (i=intRight; i>rightLower; i--) {

			currentDisp = Measurements [i] - currentSignal->Value ((double)i);

			if (currentDisp > maxDisp) {

				locMax = i;
				maxDisp = currentDisp;
			}
		}
	}

	if (maxDisp < limit)
		return false;

	double midDistance = 0.5 * fabs ((double)locMax - mu);
	double endSearch;
	double testMin = 0.5 * maxDisp;
	bool secondaryPeak = false;
	double aveHeight;
	double lineFit;

	if (locMax < mu) {

		// Search left of locMax for low "enough" heights (or displacements); alternatively,
		// test left of locMax for high enough inner product with constant

		endSearch = locMax - midDistance;

		if (endSearch < 0.0)
			endSearch = 0.0;

		for (i=locMax; i>=endSearch; i--) {

			currentDisp = Measurements [i] - currentSignal->Value ((double)i);

			if (currentDisp < testMin) {

				secondaryPeak = true;
				break;
			}
		}

		if (!secondaryPeak) {



			lineFit = InnerProductWithConstantFunction ((int)floor (endSearch), locMax, aveHeight);

			if (lineFit < 0.95)
				secondaryPeak = true;

			else if (aveHeight < 0.7 * Measurements [locMax])
				secondaryPeak = true;
		}
	}

	else {

		// Search right of locMax for low "enough" heights (or displacements); alternatively,
		// test right of locMax for high enough inner product with constant

		endSearch = locMax + midDistance;

		if (endSearch >= NumberOfSamples - 1)
			endSearch = (double) (NumberOfSamples - 1);

		for (i=locMax; i<=endSearch; i++) {

			currentDisp = Measurements [i] - currentSignal->Value ((double)i);

			if (currentDisp < testMin) {

				secondaryPeak = true;
				break;
			}
		}

		if (!secondaryPeak) {

			lineFit = InnerProductWithConstantFunction (locMax, (int)ceil (endSearch), aveHeight);

			if (lineFit < 0.95)
				secondaryPeak = true;

			else if (aveHeight < 0.7 * Measurements [locMax])
				secondaryPeak = true;
		}
	}

	if (!secondaryPeak)
		return false;

	// OK, so it seems we can't ignore the unmodeled part of this segment.  Let's try to find a division point

	nextInterval->RecomputeRelativeMinimum (currentSignal, mu, locMax, maxDisp, this);
	return true;
}



double SampledData :: InnerProductWithConstantFunction (int left, int right, double& height) const {

	int N = right - left;
	double sum = 0.0;
	double sum2 = 0.0;
	int i;
	int right1 = right - 1;
	double temp;

	for (i=left+1; i<right; i++) {

		temp = Value (i);
		sum += temp;
		sum2 += temp * temp;
	}

	temp = Value (left);
	sum += 0.5 * temp;
	sum2 += 0.5 * temp * temp;

	temp = Value (right);
	sum += 0.5 * temp;
	sum2 += 0.5 * temp * temp;

	sum2 = sqrt (sum2);  // 2-norm of sample
	double oneNorm = sqrt ((double)N);
	double sampleSpacing = DataSignal::GetSampleSpacing ();
	
	if ((sum2 > 0.0) && (oneNorm > 0.0)) {

		height = sampleSpacing * sum / (double (N));
		return sampleSpacing * sum / (sum2 * oneNorm);
	}

	return 0.0;
}


DataSignal* SampledData :: FindNextCharacteristicRetry (const DataSignal& Signature, double& fit, RGDList& previous, int dualCurve) {

	if (dualCurve == 0)
		return FindNextCharacteristicRetry (Signature, fit, previous);
	
	DataInterval* nextInterval = (DataInterval*)PeakIterator->CurrentItem ();

	if (nextInterval == NULL)
		return NULL;

	if (nextInterval->GetNumberOfMinima () == 0)
		return NULL;
	
	DataInterval* leftInterval;
	DataInterval* rightInterval;
	DataInterval* testInterval;

	leftInterval = nextInterval->Split (rightInterval);

	if ((leftInterval == NULL) || (rightInterval == NULL)) {

		delete leftInterval;
		delete rightInterval;
		return NULL;
	}

	if (dualCurve == 1) {

		testInterval = leftInterval;
		delete rightInterval;
	}

	else {

		testInterval = rightInterval;
		delete leftInterval;
	}

	DataSignal* value = Signature.FindCharacteristic (this, testInterval, TraceWindowSize, fit, previous);

	if (value != NULL) {

		value->SetDataMode (nextInterval->GetMaxAtMode ());
	}

	delete testInterval;
	return value;
}


NoiseInterval* SampledData :: GetNextNoiseInterval () {

	NoiseInterval* nextInterval = (NoiseInterval*)(*NoiseIterator) ();
	return nextInterval;
}


double SampledData :: Centroid () const {

	return Centroid (Left, Right);
}


double SampledData :: Centroid (double left, double right) const {

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);

	if (nright < nleft)
		return -DOUBLEMAX;

	if (nright == nleft)
		return Left + nright * Spacing;

	double Sum1 = 0.0;
	double Sum2 = 0.0;
	double Sum;
	double M2;
	double M1;

	for (int i=nleft+1; i<nright; i++) {

		M2 = Measurements [i];
		M2 *= M2;
		Sum1 += M2;
		Sum2 += M2 * i;
	}

	Sum = Sum1 * Left + Sum2 * Spacing;  // This works out to SUM (Measurements [i]^2 * (Left + i * Spacing)) but saves ops.
	M1 = Measurements [nleft];
	M2 = Measurements [nright];
	M1 *= M1;
	M2 *= M2;
	Sum += 0.5 * (M1 * (Left + nleft * Spacing) + M2 * (Left + nright * Spacing));
	Sum1 += 0.5 * (M1 + M2);

	if (Sum1 == 0.0) // The Norm2 is zero
		return -DOUBLEMAX;  // There is no signal

	return Sum / Sum1;

	//
	// Sum is the integration of Measurements [x] ^2 * x using the trapezoidal rule and
	// Sum1 is the Norm2 using the trapezoidal rule
	//
}


double SampledData :: SecondMoment (double left, double right) const {

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);

	if (nright < nleft)
		return -DOUBLEMAX;

	if (nright == nleft)
		return 0.0;

	double Sum1 = 0.0;
	double Sum2 = 0.0;
	double Sum;
	double M1;
	double M2;

	for (int i=nleft+1; i<nright; i++) {

		M2 = Measurements [i];
		M1 = M2 * (Left + i * Spacing);
		M2 *= M2;
		M1 *= M1;
		Sum1 += M1;  // running sum of (Measurements [x] * x) ^ 2
		Sum2 += M2;  // running sum of Measurements [x]^2
	}

	Sum = Sum2;
	M2 = Measurements [nleft];
	M1 = Measurements [nleft] * (Left + nleft * Spacing);
	M1 *= M1;
	M2 *= M2;
	Sum1 += 0.5 * M1;
	Sum += 0.5 * M2;

	M2 = Measurements [nright];
	M1 = Measurements [nright] * (Left + nright * Spacing);
	M1 *= M1;
	M2 *= M2;
	Sum1 += 0.5 * M1;
	Sum += 0.5 * M2;

	if (Sum == 0.0)  // The Norm2 is zero
		return -DOUBLEMAX;  // There is no signal

	double mean = Centroid (left, right);
	return (Sum1 / Sum) - mean * mean;
}


double SampledData :: ThirdMoment (double left, double right) const {

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);
	double mean = Centroid (left, right);

	if (nright < nleft)
		return -DOUBLEMAX;

	if (nright == nleft)
		return 0.0;

	double Sum1 = 0.0;
	double Sum2 = 0.0;
	double Sum;
	double M1;
	double M2;
	double x;

	for (int i=nleft+1; i<nright; i++) {

		M2 = Measurements [i];
		x = Left + i * Spacing - mean;
		M1 = M2 * x;
		M2 *= M2;
		M1 *= M1;
		M1 *= x;
		Sum1 += M1;  // running sum of Measurements [x]^2 * x^3
		Sum2 += M2;  // running sum of Measurements [x]^2
	}

	Sum = Sum2;
	M2 = Measurements [nleft];
	x = Left + nleft * Spacing - mean;
	M1 = Measurements [nleft] * x;
	M2 *= M2;
	M1 *= M1;
	M1 *= x;
	Sum1 += 0.5 * M1;
	Sum += 0.5 * M2;

	M2 = Measurements [nright];
	x = Left + nright * Spacing - mean;
	M1 = Measurements [nright] * x;
	M2 *= M2;
	M1 *= M1;
	M1 *= x;
	Sum1 += 0.5 * M1;
	Sum += 0.5 * M2;

	if (Sum == 0.0)  // The Norm2 is zero
		return -DOUBLEMAX;  // There is no signal

	return (Sum1 / Sum);
}


void SampledData :: ExtendDomain (double left, double right) {}
void SampledData :: ExtendDomain (const DataSignal* model) {}

DataSignal* SampledData :: GetResidual (const DataSignal* base, double coeff) { return NULL; }
int SampledData :: Add (list<DataSignal*>& WaveList) { return 0; }  // list of pointers to DataSignal


int SampledData :: TruncateToResolution (double resolution) {

	double r = 1.0 / resolution;
	
	for (int i=0; i<NumberOfSamples; i++)
		Measurements [i] = resolution * floor (r * Measurements [i]);

	return 0;
}



DataSignal* SampledData :: Digitize (int numSamples, double left, double resolution) {

	// For now, assume left = Left and numSamples = NumberOfSamples, so we can just 
	// duplicate the existing SampledData

	SampledData* sd = new SampledData (*this);
	sd->TruncateToResolution (resolution);
	return sd;
}


DataSignal* SampledData :: BuildSample (int numSamples, double left, double resolution) {

	return SampledData::Digitize (numSamples, left, resolution);
}


double SampledData :: ResetWindowedNorm2 (int nleft, int nright) {

	WindowedNorm = GetNorm2 (nleft, nright);
	WindowLeft = nleft;
	WindowRight = nright;
	OldLeft = Value (nleft);
	OldRight = Value (nright);
	return WindowedNorm;
}


double SampledData :: AdvanceWindowedNorm2Left () {

	int NewLeft = WindowLeft - 1;
	int NewRight = WindowRight - 1;

	double newL = Value (NewLeft);
	double newR = Value (NewRight);

	WindowedNorm += 0.5 * (OldLeft * OldLeft + newL * newL);
	WindowedNorm -= 0.5 * (OldRight * OldRight + newR * newR);
	WindowLeft = NewLeft;
	WindowRight = NewRight;
	OldLeft = newL;
	OldRight = newR;
	return WindowedNorm;
}


double SampledData :: AdvanceWindowedNorm2Right () {

	int NewLeft = WindowLeft + 1;
	int NewRight = WindowRight + 1;

	double newL = Value (NewLeft);
	double newR = Value (NewRight);

	WindowedNorm -= 0.5 * (OldLeft * OldLeft + newL * newL);
	WindowedNorm += 0.5 * (OldRight * OldRight + newR * newR);
	WindowLeft = NewLeft;
	WindowRight = NewRight;
	OldLeft = newL;
	OldRight = newR;
	return WindowedNorm;
}

double SampledData :: Norm () {

	return sqrt (Norm2 ());
}


double SampledData :: Norm (double left, double right) {

	return sqrt (Norm2 (left, right));
}


double SampledData :: Norm2 () {
	
	// norm squared

	int nright = NumberOfSamples - 1;

	double Sum1 = 0.0;
	double M2;
	double M1;

	for (int i=1; i<nright; i++) {

		M2 = Measurements [i];
		M2 *= M2;
		Sum1 += M2;
	}

	M1 = Measurements [0];
	M2 = Measurements [nright];
	M1 *= M1;
	M2 *= M2;
	Sum1 += 0.5 * (M1 + M2);
	Sum1 *= Spacing;

	return Sum1;
}


double SampledData :: Norm2 (double left, double right) {

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);

	if (nright < nleft)
		return -DOUBLEMAX;

	if (nright == nleft)
		return 0.0;

	double Sum1 = 0.0;
	double M2;
	double M1;

	for (int i=nleft+1; i<nright; i++) {

		M2 = Measurements [i];
		M2 *= M2;
		Sum1 += M2;
	}

	M1 = Measurements [nleft];
	M2 = Measurements [nright];
	M1 *= M1;
	M2 *= M2;
	Sum1 += 0.5 * (M1 + M2);
	Sum1 *= Spacing;

	return Sum1;

}


DataSignal* SampledData :: Normalize (double& norm) {

	norm = Norm ();
	double* NewData = new double [NumberOfSamples];

	for (int i=0; i<NumberOfSamples; i++)
		NewData [i] = Measurements [i] / norm;

	DataSignal* ds = new SampledData (NumberOfSamples, Left, Right, NewData);
	return ds;
}


DataSignal* SampledData :: Normalize (double left, double right, double& norm) {

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);

	if (nright <= nleft)
		return NULL;

	norm = Norm (left, right);
	int NewNumber = nright - nleft + 1;
	double* NewData = new double [NewNumber];

	for (int i=nleft; i<=nright; i++)
		NewData [i - nleft] = Measurements [i] / norm;

	DataSignal* ds = new SampledData (NewNumber, left, right, NewData);
	return ds;
}


double SampledData :: OneNorm () {

	double Sum = 0.0;
	double M2;
	double M1;

	for (int i=1; i<NumberOfSamples-1; i++) {

		M1 = fabs (Measurements [i]);
		Sum += M1;
	}

	M1 = fabs (Measurements [0]);
	M2 = fabs (Measurements [NumberOfSamples - 1]);
	Sum += 0.5 * (M1 + M2);  // This completes the integration via trapezoidal rule
	Sum *= Spacing;

	//
	// Sum is the integration of |Measurements [x]| using the trapezoidal rule
	//

	return Sum;
}


double SampledData :: OneNorm (double left, double right) {

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);

	if (nright < nleft)
		return -DOUBLEMAX;

	if (nright == nleft)
		return 0.0;

	double Sum = 0.0;
	double M2;
	double M1;

	for (int i=nleft+1; i<nright; i++) {

		M1 = fabs (Measurements [i]);
		Sum += M1;
	}

	M1 = fabs (Measurements [nleft]);
	M2 = fabs (Measurements [nright]);
	Sum += 0.5 * (M1 + M2);  // This completes the integration via trapezoidal rule
	Sum *= Spacing;

	//
	// Sum is the integration of |Measurements [x]| using the trapezoidal rule
	//

	return Sum;
}


int SampledData :: FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean) {

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);

	if (nright < nleft) {

		oneNorm = 0.0;
		mean = -DOUBLEMAX;
		return -1;
	}

	if (nright == nleft) {

		oneNorm = 0.0;
		mean = Left + nright * Spacing;
		return 1;
	}

	double Sum1 = 0.0;
	double Sum2 = 0.0;
	double Sum;
	double M2;
	double M1;

	for (int i=nleft+1; i<nright; i++) {

		M1 = fabs (Measurements [i]);
		Sum1 += M1;  // this is for oneNorm
		Sum2 += M1 * i;  // this is for mean
	}

	Sum = Sum1 * Left + Sum2 * Spacing;  // This works out to SUM (|Measurements [i]| * (Left + i * Spacing)) but saves ops.
	M1 = fabs (Measurements [nleft]);
	M2 = fabs (Measurements [nright]);
	Sum += 0.5 * (M1 * (Left + nleft * Spacing) + M2 * (Left + nright * Spacing));
	Sum1 += 0.5 * (M1 + M2);
	Sum *= Spacing;
	Sum1 *= Spacing;
	oneNorm = Sum1;
    
	if (Sum1 == 0.0) {
		
		// The oneNorm is zero.  There is no signal
		oneNorm = 0.0;
		mean = -DOUBLEMAX;
		return -1;
	}

	mean = Sum / Sum1;

	//
	// Sum is the integration of Measurements [x] ^2 * x using the trapezoidal rule and
	// Sum1 is the Norm2 using the trapezoidal rule
	//

	return 0;
}



int SampledData :: SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment) {

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);

	if (nright < nleft) {

		oneNorm = 0.0;
		mean = -DOUBLEMAX;
		secondMoment = -DOUBLEMAX;
		return -1;
	}

	if (nright == nleft) {

		oneNorm = 0.0;
		mean = Left + nright * Spacing;
		secondMoment = 0.0;
		return 1;
	}

	double Sum1 = 0.0;
	double Sum2 = 0.0;
	double Sum3 = 0.0;
	double OldSum2;
	double M1;
	double M2;
	double M3;

	for (int i=nleft+1; i<nright; i++) {

		M1 = fabs (Measurements [i]);
		M2 = M1 * i;
		M3 = M2 * i;
		Sum1 += M1;  // running sum of |Measurements [x]|
		Sum2 += M2;  // running sum of |Measurements [x]| * x
		Sum3 += M3;  // running sum of |Measurements [x]| * x ^ 2
	}

	M1 = 0.5 * fabs (Measurements [nleft]);
	M2 = M1 * nleft;
	M3 = M2 * nleft;
	Sum1 += M1;
	Sum2 += M2;
	Sum3 += M3;

	M1 = 0.5 * fabs (Measurements [nright]);
	M2 = M1 * nright;
	M3 = M2 * nright;
	Sum1 += M1;
	Sum2 += M2;
	Sum3 += M3;

	if (Sum1 == 0.0) {
		
		// The Norm2 is zero.  There is no signal
		oneNorm = 0.0;
		mean = -DOUBLEMAX;
		secondMoment = 0.0;
		return -1;
	}

	oneNorm = Sum1 * Spacing;
	OldSum2 = Sum2;
	Sum2 = Sum1 * Left + Sum2 * Spacing;  // works out to SUM (M[i]*(Left + i*Spacing)
	mean = Sum2 / Sum1;
	Sum3 = Sum1 * Left * Left + 2.0 * Spacing * Left * OldSum2 + Sum3 * Spacing * Spacing;  // works out to
				// SUM (M[i}*(Left + i*Spacing)^2
	secondMoment = (Sum3 / Sum1) - mean * mean;
	return 0;
}


int SampledData :: FirstMoment (double left, double right, double& twoNorm2, double& mean) {

	//
	// Uses the 2-Norm
	//

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);

	if (nright < nleft) {

		twoNorm2 = 0.0;
		mean = -DOUBLEMAX;
		return -1;
	}

	if (nright == nleft) {

		twoNorm2 = 0.0;
		mean = Left + nright * Spacing;
		return 1;
	}

	double Sum1 = 0.0;
	double Sum2 = 0.0;
	double Sum;
	double M2;
	double M1;

	for (int i=nleft+1; i<nright; i++) {

		M1 = Measurements [i];
		M1 *= M1;
		Sum1 += M1;  // this is for twoNorm2
		Sum2 += M1 * i;  // this is for mean
	}

	Sum = Sum1 * Left + Sum2 * Spacing;  // This works out to SUM ((Measurements [i])^2 * (Left + i * Spacing)) but saves ops.
	M1 = Measurements [nleft];
	M2 = Measurements [nright];
	M1 *= M1;
	M2 *= M2;
	Sum += 0.5 * (M1 * (Left + nleft * Spacing) + M2 * (Left + nright * Spacing));
	Sum1 += 0.5 * (M1 + M2);
	twoNorm2 = Sum1 * Spacing;
    
	if (Sum1 == 0.0) {
		
		// The oneNorm is zero.  There is no signal
		twoNorm2 = 0.0;
		mean = -DOUBLEMAX;
		return -1;
	}

	mean = Sum / Sum1;

	//
	// Sum is the integration of Measurements [x] ^2 * x using the trapezoidal rule and
	// Sum1 is the L-2 Norm squared using the trapezoidal rule (modulo Spacing)
	//

	return 0;
}


int SampledData :: SecondMoment (double left, double right, double& twoNorm2, double& mean, double& secondMoment) const {

	int nleft = GetSampleNumber (left);
	int nright = GetSampleNumber (right);

	if (nright < nleft) {

		twoNorm2 = 0.0;
		mean = -DOUBLEMAX;
		secondMoment = -DOUBLEMAX;
		return -1;
	}

	if (nright == nleft) {

		twoNorm2 = 0.0;
		mean = Left + nright * Spacing;
		secondMoment = 0.0;
		return 1;
	}

	double Sum1 = 0.0;
	double Sum2 = 0.0;
	double Sum3 = 0.0;
	double OldSum2;
	double M1;
	double M2;
	double M3;

	for (int i=nleft+1; i<nright; i++) {

		M1 = Measurements [i];
		M1 *= M1;
		M2 = M1 * i;
		M3 = M2 * i;
		Sum1 += M1;  // running sum of Measurements [x]^2
		Sum2 += M2;  // running sum of Measurements [x]^2 * x
		Sum3 += M3;  // running sum of Measurements [x]^2 * x ^ 2
	}

	M1 = Measurements [nleft];
	M1 *= 0.5 * M1;
	M2 = M1 * nleft;
	M3 = M2 * nleft;
	Sum1 += M1;
	Sum2 += M2;
	Sum3 += M3;

	M1 = Measurements [nright];
	M1 *= 0.5 * M1;
	M2 = M1 * nright;
	M3 = M2 * nright;
	Sum1 += M1;
	Sum2 += M2;
	Sum3 += M3;

	if (Sum1 == 0.0) {
		
		// The Norm2 is zero.  There is no signal
		twoNorm2 = 0.0;
		mean = -DOUBLEMAX;
		secondMoment = 0.0;
		return -1;
	}

	twoNorm2 = Sum1 * Spacing;
	OldSum2 = Sum2;
	Sum2 = Sum1 * Left + Sum2 * Spacing;  // works out to SUM (M[i]^2*(Left + i*Spacing)
	mean = Sum2 / Sum1;
	Sum3 = Sum1 * Left * Left + 2.0 * Spacing * Left * OldSum2 + Sum3 * Spacing * Spacing;  // works out to
				// SUM (M[i}^2*(Left + i*Spacing)^2
	secondMoment = (Sum3 / Sum1) - mean * mean;
	return 0;
}


size_t SampledData :: StoreSize () const {
	
	size_t size = DataSignal::StoreSize ();
	size += sizeof (NumberOfSamples) + sizeof (Spacing);
	size += NumberOfSamples * sizeof (Spacing);
	return size;
}

void SampledData :: RestoreAll (RGFile& f) {

	DataSignal::RestoreAll (f);
	f.Read (NumberOfSamples);
	delete[] Measurements;
	Measurements = new double [NumberOfSamples];
	f.Read (Measurements, (size_t)NumberOfSamples);
	f.Read (Spacing);
}


void SampledData :: RestoreAll (RGVInStream& f) {

	DataSignal::RestoreAll (f);
	f >> NumberOfSamples;
	delete[] Measurements;
	Measurements = new double [NumberOfSamples];
	f.Get (Measurements, (size_t)NumberOfSamples);
	f >> Spacing;
}


void SampledData :: SaveAll (RGFile& f) const {

	DataSignal::SaveAll (f);
	f.Write (NumberOfSamples);
	f.Write (Measurements, (size_t)NumberOfSamples);
	f.Write (Spacing);
}


void SampledData :: SaveAll (RGVOutStream& f) const {

	DataSignal::SaveAll (f);
	f << NumberOfSamples;
	f.put (Measurements, (size_t)NumberOfSamples);
	f << Spacing;
}


int SampledData :: GetSampleNumber (double abscissa) const {

	int nleft = (int)floor(((abscissa - Left) / Spacing) + 0.5);

	if (nleft <= 0)
		return 0;

	if (nleft >= NumberOfSamples)
		return NumberOfSamples - 1;

	return nleft;
}


double SampledData :: GetNorm2 (int nleft, int nright) {

	if (nright < nleft)
		return -1.0;

	if (nright == nleft)
		return 0.0;

	double Sum1 = 0.0;
	double M2;
	double M1;

	for (int i=nleft+1; i<nright; i++) {

		M2 = Measurements [i];
		M2 *= M2;
		Sum1 += M2;
	}

	M1 = Measurements [nleft];
	M2 = Measurements [nright];
	M1 *= M1;
	M2 *= M2;
	Sum1 += 0.5 * (M1 + M2);
	Sum1 *= Spacing;

	return Sum1;
}



ParametricCurve :: ParametricCurve () : DataSignal (), Displacement (0.0), Scale (1.0) {}

ParametricCurve :: ParametricCurve (double left, double right) : DataSignal (left, right), Displacement (0.0),
Scale (1.0) {}


ParametricCurve :: ParametricCurve (const ParametricCurve& pc) : DataSignal ((DataSignal&)pc), Displacement (pc.Displacement),
Scale (pc.Scale) {}


ParametricCurve :: ParametricCurve (const ParametricCurve& pc, CoordinateTransform* trans) : DataSignal (pc, trans), Scale (pc.Scale) {

	Displacement = trans->EvaluateWithExtrapolation (pc.Displacement);
}


ParametricCurve :: ~ParametricCurve () {}

void ParametricCurve :: SetDisplacement (double disp) { 
	
	Displacement = disp;
}


int ParametricCurve :: AddToSample (double* sample, double sampleLeft, double sampleRight) const {

	int Low;
	int High;
	double spacing = DataSignal::GetSampleSpacing ();
	double right;
	double x;

	if (Left <= sampleLeft)
		Low = 0;

	else
		Low = (int) ceil ((Left - sampleLeft) / spacing);

	if (Right >= sampleRight)
		right = sampleRight;

	else
		right = Right;

	High = (int) floor ((right - sampleLeft) / spacing);

	for (int i=Low; i<=High; i++) {

		x = sampleLeft + i * spacing;
		double temp;

		if (GetStandardDeviation () < 0.14) {

			double mean = floor (GetMean () + 0.5);

			if (x <= mean - 1.0)
				temp = 0.0;

			else if (x >= mean + 1.0)
				temp = 0.0;

			else
				temp = Peak ();
		}

		else
			temp = Value (x);

		sample [i] = temp;
	}

	return High - Low;
}


int ParametricCurve :: AddToSampleArray (double* sample, double sampleLeft, double sampleRight, double verticalResolution) const {

	int Low;
	int High;
	double spacing = DataSignal::GetSampleSpacing ();
	double right;
	double x;
	double tailLeft;
	double tailRight;

	ComputeTails (tailLeft, tailRight);

	if (tailLeft <= sampleLeft)
		Low = 0;

	else
		Low = (int) ceil ((tailLeft - sampleLeft) / spacing);

	if (tailRight >= sampleRight)
		right = sampleRight;

	else
		right = tailRight;

	High = (int) floor ((right - sampleLeft) / spacing);
	double temp;
	double* temp2;

	for (int i=Low; i<=High; i++) {

		x = sampleLeft + i * spacing;
		temp = Value (x);
		temp2 = sample + i;

		if (temp > *temp2)
			*temp2 = temp;
	}

	return High - Low;
}


void ParametricCurve :: SetScale (double scale) {
	
	Scale = scale;
}


double ParametricCurve :: GetScale (int curve) const {

	return Scale;
}


bool ParametricCurve :: HasAlleleName () const {

	return !mAlleleName.IsEmpty ();
}


void ParametricCurve :: SetVirtualAlleleName (const RGString& name) {

	if (name.Length () > 0)  //testing
		mWouldBeAlleleName = name;
}


RGString ParametricCurve :: GetVirtualAlleleName () const {

	return mWouldBeAlleleName;
}


double ParametricCurve :: Centroid () const {

	return GetMean ();
}



void ParametricCurve :: ExtendDomain (double left, double right) {

	double l = left;

	if (Left < l)
		l = Left;

	double r = right;

	if (Right > r)
		r = Right;

	if (l < r) {

		Left = l;
		Right = r;
	}
}


void ParametricCurve :: ExtendDomain (const DataSignal* model) {

	double left = model->LeftEndPoint ();
	double right = model->RightEndPoint ();
	ExtendDomain (left, right);
}


DataSignal* ParametricCurve :: GetResidual (const DataSignal* base, double coeff) {

	return NULL;
}


int ParametricCurve :: Add (list<DataSignal*>& WaveList) {
	
	// list of pointers to DataSignal
	return -1;
}


int ParametricCurve :: TruncateToResolution (double resolution) {

	return -1;
}


DataSignal* ParametricCurve :: Digitize (int numSamples, double left, double resolution) {

	if (numSamples < 2)
		return NULL;

	double spacing = DataSignal::GetSampleSpacing ();
	double right = left + (numSamples - 1) * spacing;
	
	double x;
	double* data = new double [numSamples];

	for (int i=0; i<numSamples; i++) {

		x = left + i * spacing;
		data [i] = TruncateWithResolution (Value (x), resolution);
	}

	DataSignal* ds = new SampledData (numSamples, left, right, data);
	return ds;
}


DataSignal* ParametricCurve :: BuildSample (int numSamples, double left, double resolution) {

	return ParametricCurve::Digitize (numSamples, left, resolution);
}


const DataSignal* ParametricCurve :: BuildOverlappedSignal (double left, double right, RGDList& previous, const DataSignal* target) const {

	return target;
	/*double* LocalMeasurements;
	int NSamples;
	double tempValue;
	DataSignal* TestSignal;
	RGDListIterator it (previous);
	DataSignal* PrevSignal = (DataSignal*)it ();
	double newLeft;
	double newRight;

	//return target;

	if (PrevSignal == NULL)
		return target;

	double LeastLeft = PrevSignal->LeftEndPoint ();
	double GreatestRight = PrevSignal->RightEndPoint ();

	if (LeastLeft > right)
		return target;

	if (GreatestRight < left)
		return target;

	int NSignals = 1;
	int j;
	double x;

	while (PrevSignal = (DataSignal*)it ()) {

		newLeft = PrevSignal->LeftEndPoint ();

		if (newLeft > right)
			break;

		newRight = PrevSignal->RightEndPoint ();

		if (newRight < left)
			break;

		if (newLeft < LeastLeft)
			LeastLeft = newLeft;

		if (newRight > GreatestRight)
			GreatestRight = newRight;

		NSignals++;
	}

	double ISpacing = DataSignal::GetSampleSpacing ();

	if (right - left <= 2.0 * ISpacing)
		return target;

	TestSignal = target->Project (left, right);
	LocalMeasurements = (double*)TestSignal->GetData ();
	NSamples = TestSignal->GetNumberOfSamples ();
	double OldValue;

	if (NSignals > 1)
		NSignals = 1;

	for (int i=0; i<NSamples; i++) {

		x = left + i * ISpacing;

		if ((x >= LeastLeft) && (x <= GreatestRight)) {

			tempValue = OldValue = LocalMeasurements [i];
			it.Reset ();

			for (j=0; j<NSignals; j++) {

				PrevSignal = (DataSignal*) it();

				if (PrevSignal == NULL)
					break;

				tempValue -= PrevSignal->Value (x);
			}
			
			if (tempValue * OldValue < 0.0)
				tempValue = 0.0;

			else if (tempValue < 0.0)
				tempValue = 0.0;

			else if (abs(tempValue) > abs (OldValue))
				tempValue = OldValue;

			LocalMeasurements [i] = tempValue;
		}
	}

	return TestSignal;*/
}


DataSignal* ParametricCurve :: FindNextCharacteristicFromRight (const DataSignal& Signature, double& fit, RGDList& previous) {

	return NULL;
}


DataSignal* ParametricCurve :: FindNextCharacteristicFromLeft (const DataSignal& Signature, double& fit, RGDList& previous) {

	return NULL;
}


size_t ParametricCurve :: StoreSize () const {

	size_t size = DataSignal::StoreSize ();
	size += sizeof (Displacement) + sizeof (Scale);
	return size;
}


int ParametricCurve :: CompareTo (const RGPersistent* p) const {

	ParametricCurve* q = (ParametricCurve*) p;
	double MyMean = Centroid ();
	double pMean = q->Centroid ();

	if (MyMean < pMean)
		return -1;

	if (MyMean > pMean)
		return 1;

	return 0;
}


Boolean ParametricCurve :: IsEqualTo (const RGPersistent* p) const {

	ParametricCurve* q = (ParametricCurve*) p;
	double MyMean = Centroid ();
	double pMean = q->Centroid ();

	if (MyMean == pMean)
		return TRUE;

	return FALSE;
}


void ParametricCurve :: RestoreAll (RGFile& f) {

	DataSignal::RestoreAll (f);
	f.Read (Displacement);
	f.Read (Scale);
}


void ParametricCurve :: RestoreAll (RGVInStream& f) {

	DataSignal::RestoreAll (f);
	f >> Displacement;
	f >> Scale;
}


void ParametricCurve :: SaveAll (RGFile& f) const {

	DataSignal::SaveAll (f);
	f.Write (Displacement);
	f.Write (Scale);
}


void ParametricCurve :: SaveAll (RGVOutStream& f) const {

	DataSignal::SaveAll (f);
	f << Displacement;
	f << Scale;
}


double ParametricCurve :: TruncateWithResolution (double value, double resolution) {

//	int i;
	double y;
	
	if (value >= 0) {

//		i = value / resolution;
		y = floor (value / resolution) * resolution;
		return y;
	}

//	i = abs (value) / resolution;
//	i++;
	y = - (floor (fabs (value) / resolution) + 1.0) * resolution;
	return y;
}


Gaussian :: Gaussian () : ParametricCurve (-Gaussian::SigmaWidth, Gaussian::SigmaWidth), Mean (0.0), StandardDeviation (1.0) {}


Gaussian :: Gaussian (double mean, double standardDeviation) : 
ParametricCurve (mean - Gaussian::SigmaWidth * standardDeviation, mean + Gaussian::SigmaWidth * standardDeviation), Mean (mean), 
StandardDeviation (standardDeviation) {
																				
}



Gaussian :: Gaussian (double left, double right, double mean, double standardDeviation) :
ParametricCurve (left, right), Mean (mean), StandardDeviation (standardDeviation) {

}


Gaussian :: Gaussian (const Gaussian& gau) : ParametricCurve (gau), Mean (gau.Mean), StandardDeviation (gau.StandardDeviation) {

}


Gaussian :: Gaussian (double mean, const Gaussian& gau) : 
ParametricCurve (mean - Gaussian::SigmaWidth * gau.StandardDeviation, mean + Gaussian::SigmaWidth * gau.StandardDeviation),
Mean (mean), StandardDeviation (gau.StandardDeviation) {

}


Gaussian :: ~Gaussian () {

}


DataSignal* Gaussian :: MakeCopy (double mean) const {

	DataSignal* newSignal = new Gaussian (mean, *this);
	return newSignal;
}


RGString Gaussian :: GetSignalType () const {

	return RGString ("Gaussian");
}


void Gaussian :: SetDisplacement (double disp) {

	Mean += disp;
}


void Gaussian :: SetScale (double scale) {

	Scale = scale;
}


void Gaussian :: SetPeak (double peak) {

	Scale = peak;
}


void Gaussian :: ComputeTails (double& tailLeft, double& tailRight) const {

	double F;
	double spacing = DataSignal::GetSampleSpacing ();
	
	if (Scale <= 0.0)
		F = 0.0;

	else
		F = StandardDeviation * sqrt (2.0 * log (Scale)) + spacing;

	tailLeft = Mean - F;
	tailRight = Mean + F;
}


double Gaussian :: Value (double x) const {

	double temp = (x - Mean) / StandardDeviation;
	temp *= 0.5 * temp;
	return Scale * exp (-temp);
}


double Gaussian :: Peak () const {

	return Scale;
}


double Gaussian :: GetMean () const {

	return Mean;
}


double Gaussian :: GetStandardDeviation () const {

	return StandardDeviation;
}


double Gaussian :: GetVariance () const {

	return StandardDeviation * StandardDeviation;
}


DataSignal* Gaussian :: Project (double left, double right) const {

	double l = left;

	if (Left > l)
		l = Left;

	double r = right;

	if (Right < r)
		r = Right;

	if (r <= l)
		return NULL;

	if ((r <= Mean) || (Mean <= l))
		return NULL;

	DataSignal* ds = new Gaussian (l, r, Mean, StandardDeviation);
	return ds;
}


DataSignal* Gaussian :: Project (const DataSignal* target) const {

	double left = target->LeftEndPoint ();
	double right = target->RightEndPoint ();
	return Project (left, right);
}


double Gaussian :: Centroid () const {

	return Mean;
}


double Gaussian :: Centroid (double left, double right) const {

	return Mean;
}


double Gaussian :: SecondMoment (double left, double right) const {

	return StandardDeviation * StandardDeviation;
}


double Gaussian :: Norm () {

	return sqrt (Norm2 ());
}


double Gaussian :: Norm (double left, double right) {

	return sqrt (Norm2 (left, right));
}


double Gaussian :: Norm2 () {
	
	// norm squared
	return Scale * Scale * StandardDeviation * sqrtPi;
}


double Gaussian :: Norm2 (double left, double right) {

	if ((left <= Left) && (right >= Right))
		return Norm2 ();

	double sum = 0.0;
	double width = (right - left) / Gaussian::NumberOfSteps;
	double temp;
	double x;
	int Total = Gaussian::NumberOfSteps;

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		temp = (x - Mean) / StandardDeviation;
		temp *= temp;
		sum += exp (-temp);
	}

	temp = (left - Mean) / StandardDeviation;
	temp *= temp;
	sum += 0.5 * exp (-temp);

	temp = (right - Mean) / StandardDeviation;
	temp *= temp;
	sum += 0.5 * exp (-temp);

	return width * sum * Scale * Scale;
}


DataSignal* Gaussian :: Normalize (double& norm) {

	norm = Norm ();
	double NewScale = Scale / norm;
	DataSignal* ds = new Gaussian (Left, Right, Mean, StandardDeviation);
	ds->SetDisplacement (Displacement);
	ds->SetScale (NewScale);
	return ds;
}


DataSignal* Gaussian :: Normalize (double left, double right, double& norm) {

	norm = Norm (left, right);
	double NewScale = Scale / norm;
	DataSignal* ds = new Gaussian (left, right, Mean, StandardDeviation);

	ds->SetDisplacement (Displacement);
	ds->SetScale (NewScale);
	return ds;
}


void Gaussian :: CalculateTheoreticalArea () {

	double first = Peak ();
	double Cumulative = first;
	double last;
	double mean = GetMean ();
	double sigma = GetStandardDeviation ();
	double abscissa = mean;
	double temp1;
	double prevLast = first;
	double cutOff = 0.01 * first;
	double step = 0.5 * sigma;

	for (int i=1; i<=20; i++) {

		abscissa += step;
		last = Value (abscissa);

		if ((last <= cutOff) && (i > 1))
			break;

		Cumulative += last;
		prevLast = last;
	}

	temp1 = step * (2.0 * Cumulative - first - prevLast);
	mArea = temp1;
}


double Gaussian :: OneNorm () {

	return Scale * sqrtTwo * sqrtPi * StandardDeviation;
}


double Gaussian :: OneNorm (double left, double right) {

	if ((left <= Left) && (right >= Right))
		return OneNorm ();

	double sum = 0.0;
	double width = (right - left) / Gaussian::NumberOfSteps;
	double temp;
	double x;
	int Total = Gaussian::NumberOfSteps;

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		temp = (x - Mean) / StandardDeviation;
		temp *= 0.5 * temp;
		sum += exp (-temp);
	}

	temp = (left - Mean) / StandardDeviation;
	temp *= 0.5 * temp;
	sum += 0.5 * exp (-temp);

	temp = (right - Mean) / StandardDeviation;
	temp *= 0.5 * temp;
	sum += 0.5 * exp (-temp);

	return width * sum * Scale;
}


int Gaussian :: FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean) {

	if ((left <= Left) && (right >= Right)) {

		oneNorm = OneNorm ();
		mean = Mean;
		return 0;
	}

	double sum = 0.0;
	double sum1 = 0.0;
	double width = (right - left) / Gaussian::NumberOfSteps;
	double temp;
	double tempExp;
	double x;
	int Total = Gaussian::NumberOfSteps;

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		temp = (x - Mean) / StandardDeviation;
		temp *= 0.5 * temp;
		tempExp = exp (-temp);
		sum += tempExp;
		sum1 += x * tempExp;
	}

	temp = (left - Mean) / StandardDeviation;
	temp *= 0.5 * temp;
	tempExp = 0.5 * exp (-temp);
	sum += tempExp;
	sum1 += left * tempExp;

	temp = (right - Mean) / StandardDeviation;
	temp *= 0.5 * temp;
	tempExp = 0.5 * exp (-temp);
	sum += tempExp;
	sum1 += right * tempExp;

	oneNorm = width * sum * Scale;

	if (oneNorm == 0.0) {

		mean = DOUBLEMAX;
		return -1;
	}

	mean = sum1 / sum;
	return 0;
}



int Gaussian :: SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment) {

	if ((left <= Left) && (right >= Right)) {

		oneNorm = OneNorm ();
		mean = Mean;
		secondMoment = StandardDeviation * StandardDeviation;
		return 0;
	}

	double sum = 0.0;
	double sum1 = 0.0;
	double sum2 = 0.0;
	double width = (right - left) / Gaussian::NumberOfSteps;
	double temp;
	double tempExp;
	double x;
	int Total = Gaussian::NumberOfSteps;

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		temp = (x - Mean) / StandardDeviation;
		temp *= 0.5 * temp;
		tempExp = exp (-temp);
		sum += tempExp;
		sum1 += x * tempExp;
		sum2 += x * x * tempExp;
	}

	temp = (left - Mean) / StandardDeviation;
	temp *= 0.5 * temp;
	tempExp = 0.5 * exp (-temp);
	sum += tempExp;
	sum1 += left * tempExp;
	sum2 += left * left * tempExp;

	temp = (right - Mean) / StandardDeviation;
	temp *= 0.5 * temp;
	tempExp = 0.5 * exp (-temp);
	sum += tempExp;
	sum1 += right * tempExp;
	sum2 += right * right * tempExp;

	oneNorm = width * sum * Scale;

	if (oneNorm == 0.0) {

		mean = DOUBLEMAX;
		secondMoment = DOUBLEMAX;
		return -1;
	}

	mean = sum1 / sum;
	secondMoment = (sum2 / sum) - mean * mean;
	return 0;
}


void Gaussian :: Report (RGTextOutput& text, const RGString& indent) {

	Endl endLine;
	text << indent << "Gaussian with parameters:\n" << endLine;
	text << indent << "   Mean  = " << Mean << endLine;
	text << indent << "   Sigma = " << StandardDeviation << endLine;
	text << indent << "   Scale = " << Scale << endLine;
	double mass = OneNorm ();
	text << indent << "   Mass  = " << mass << endLine;
	text << indent << "   Fit   = " << Fit << endLine;
}


size_t Gaussian :: StoreSize () const {

	size_t size = ParametricCurve::StoreSize ();
	size += 2 * sizeof (double);
	return size;
}

void Gaussian :: RestoreAll (RGFile& f) {

	ParametricCurve::RestoreAll (f);
	f.Read (Mean);
	f.Read (StandardDeviation);
}


void Gaussian :: RestoreAll (RGVInStream& f) {

	ParametricCurve::RestoreAll (f);
	f >> Mean;
	f >> StandardDeviation;
}


void Gaussian :: SaveAll (RGFile& f) const {

	ParametricCurve::SaveAll (f);
	f.Write (Mean);
	f.Write (StandardDeviation);
}


void Gaussian :: SaveAll (RGVOutStream& f) const {

	ParametricCurve::SaveAll (f);
	f << Mean;
	f << StandardDeviation;
}


NormalizedGaussian :: NormalizedGaussian () : Gaussian () {

	Scale = 1.0 / (sqrt (sqrtPi * StandardDeviation));
	SampleSpacing = SampledData::GetSampleSpacing ();
}


NormalizedGaussian :: NormalizedGaussian (double mean, double standardDeviation) : Gaussian (mean, standardDeviation) {

	Scale = 1.0 / (sqrt (sqrtPi * StandardDeviation));
	SampleSpacing = SampledData::GetSampleSpacing ();
}


NormalizedGaussian :: NormalizedGaussian (const NormalizedGaussian& ng) : Gaussian (ng), SampleSpacing (ng.SampleSpacing) {

}


NormalizedGaussian :: NormalizedGaussian (double mean, const NormalizedGaussian& ng) : Gaussian (mean, ng), SampleSpacing (ng.SampleSpacing) {

}


NormalizedGaussian :: ~NormalizedGaussian () {

}


DataSignal* NormalizedGaussian :: MakeCopy (double mean) const {

	DataSignal* newSignal = new NormalizedGaussian (mean, *this);
	return newSignal;
}


RGString NormalizedGaussian :: GetSignalType () const {

	return RGString ("Normalized Gaussian");
}


void NormalizedGaussian :: SetScale (double scale) {

}


DataSignal* NormalizedGaussian :: FindNextCharacteristicFromRight (const DataSignal& Signature, double& fit, RGDList& previous) {

	return NULL;
}


DataSignal* NormalizedGaussian :: FindNextCharacteristicFromLeft (const DataSignal& Signature, double& fit, RGDList& previous) {

	return NULL;
}


DataSignal* NormalizedGaussian :: FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, 
int windowSize, double& fit, RGDList& previous) const {

	//
	//  OK, here's the skinny.  First, assume that the standard deviation, sigma (sigma0) of this Gaussian,
	// is less than that for any of the curves to fit.  Given that, then here's what to do:
	// 1)  Find the interval of definition of the Target from Segment, refine it and make it symmetric about 
	//     the center, using windowSize; 
	// 2)  Calculate the sample mean and sample standard deviation (sigma1) on the refined interval; 
	// 3)  Calculate the Dot Product with the native NormalizedGaussian using the offset of the sample mean; 
	// 4)  Calculate the predicted sigma (sigma2) based on the formula and the dot product; 
	// 5)  Using starting sigma (sigma0) for 3rd sigma, perform inverse parabolic interpolation to refine 
	//     sigma (1 - 5 iterations, max?); 
	// 6)  Create new NormalizedGaussian (Gaussian?) with appropriate parameters and return...fit = Dot Product.
	//

	// 1:	
	int SegmentLeft = Segment->GetLeft ();
	int SegmentRight = Segment->GetRight ();
	int SegmentCenter = Segment->GetCenter ();
	int SegmentWindow = SegmentRight - SegmentLeft;
	SegmentWindow -= windowSize;
	double coeffs [3];
	double scaleFactors [3];
	double newScale;

	DataSignal* possibleSpike = Segment->TestForSpike (fit);

	if (possibleSpike != NULL)
		return possibleSpike;

	int MaxHalfWindow = SegmentCenter - SegmentLeft;   // Added to make sure we don't exceed Segment boundary (by much)
	int tempSeg = SegmentRight - SegmentCenter;   //  ditto
	
	if ((tempSeg < MaxHalfWindow) && (tempSeg > 0))   //  ditto
		MaxHalfWindow = tempSeg;   //  ditto

	int MaxWindow = 2 * MaxHalfWindow;   //  ditto

	SegmentWindow = 2 * ((SegmentWindow + 1) / 2);  // Gets SegmentWindow, if even, and SegmentWindow+1 if not

	if (SegmentWindow > MaxWindow)   // Added to make sure we don't exceed Segment boundary (by much)
		SegmentWindow = MaxWindow;   // Added to make sure we don't exceed Segment boundary (by much)

	// Get window for target Gaussian and compare; take whichever is larger
	double IntervalSpacing = DataSignal::GetSampleSpacing ();
	double HalfTargetWindow = Gaussian::GetSigmaWidth() * StandardDeviation;
	int HalfIntervalWindow = (int)floor((HalfTargetWindow / IntervalSpacing) + 0.9);
	int IntervalWindow = 2 * HalfIntervalWindow;

	if (IntervalWindow > SegmentWindow)
		SegmentWindow = IntervalWindow;

	int HalfSegmentWindow = SegmentWindow / 2;
	SegmentLeft = SegmentCenter - HalfSegmentWindow;
	SegmentRight = SegmentCenter + HalfSegmentWindow;

	int OriginalHalfSegmentWindow = HalfSegmentWindow;  // This is as large as we ever want this.  Don't make it bigger!

	// 2:
	double left = Target->LeftEndPoint () + SegmentLeft * IntervalSpacing;
	double right = Target->LeftEndPoint () + SegmentRight * IntervalSpacing;
	double center = Target->LeftEndPoint () + SegmentCenter * IntervalSpacing;

	int OldSegmentLeft = SegmentLeft;

	if (HalfSegmentWindow = 0)
		return NULL;

	const DataSignal* Overlapped = BuildOverlappedSignal (left, right, previous, Target);

	double sampleTwoNorm2;
	double sampleMean;
	double sampleTwoMoment;
	
	if (Target->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment) != 0)
		return NULL;
	
	if (Overlapped->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment) != 0) {
		
		if (Overlapped != Target) {
			delete Overlapped;
			Overlapped = BuildOverlappedSignal (left, right, previous, Target);
		}

		return NULL;
	}

	double sigma1 = sqrtTwo * sqrt (sampleTwoMoment);  // assume sampleTwoMoment is variance:  mean^2 already subtracted
	double sampleTwoNorm = sqrt (sampleTwoNorm2);

	//
	// The following is new and the purpose is to adjust the window to be more appropriate to the expected curve:  not too large
	//

	double OldLeft = left;
	double OldRight = right;

	HalfTargetWindow = Gaussian::GetSigmaWidth() * sigma1;
	HalfSegmentWindow = (int)floor((HalfTargetWindow / IntervalSpacing) + 0.9999);

	if (HalfSegmentWindow > OriginalHalfSegmentWindow)
		HalfSegmentWindow = OriginalHalfSegmentWindow;  // We want this process to DECREASE segment window, not increase it!!

	SegmentCenter = (int) floor ((sampleMean / IntervalSpacing) + 0.5);
	IntervalWindow = 2 * HalfIntervalWindow;
	SegmentLeft = SegmentCenter - HalfSegmentWindow;
	SegmentRight = SegmentCenter + HalfSegmentWindow;
	SegmentWindow = SegmentRight - SegmentLeft;
	SegmentWindow = 2 * ((SegmentWindow + 1) / 2);  // Gets SegmentWindow, if even, and SegmentWindow+1 if not
	left = Target->LeftEndPoint () + SegmentLeft * IntervalSpacing;
	right = Target->LeftEndPoint () + SegmentRight * IntervalSpacing;

	if ((left < OldLeft) || (right > OldRight)) {

		if (Overlapped != Target)
			delete Overlapped;

		OldSegmentLeft = SegmentLeft;
		Overlapped = BuildOverlappedSignal (left, right, previous, Target);
	}

	// This calculates mean, variance and norm for new interval

	if (Overlapped->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment) != 0) {
		
		if (Overlapped != Target) {
			delete Overlapped;
			Overlapped = BuildOverlappedSignal (left, right, previous, Target);
		}
		
		return NULL;
	}

	sigma1 = sqrtTwo * sqrt (sampleTwoMoment);  // assume sampleTwoMoment is variance:  mean^2 already subtracted
	sampleTwoNorm = sqrt (sampleTwoNorm2);

	// 3:
	int i;
	int NSamples = SegmentWindow + 1;

	//  The following should be normalized and between 0 and 1

	if (Overlapped != Target) {

		SegmentRight -= OldSegmentLeft;
		SegmentCenter -= OldSegmentLeft;
		SegmentLeft -= OldSegmentLeft;
	}

	SampleDataInfo SampleInfo (*Overlapped, SegmentLeft, SegmentCenter, SegmentRight, NSamples);

	double sigma0 = StandardDeviation * sqrtTwo;  // sigma0, sigma1 and sigma2 should bracket true sigma, but maybe not!?

	// We're going to use our prediction formula here.  This formula gives the true (relative) standard deviation
	// based on the dot product, assuming we have the sample mean correct.  To do this, we need a function that only
	// performs the dot product on the primary Gaussian.  That maintains the validity of the formula (assuming, of 
	// course, that the sample mean is the "true" mean).

	//  The following should be normalized and between 0 and 1

	double dotProduct = AsymmetricInnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

	if (dotProduct > 1.0)
		dotProduct = 1.0;  // Just for safety...those numbers!  They sure can play tricks!

	// 4:
	// Ready or not, here comes the formula

	double temp2 = dotProduct * dotProduct;
	double temp = temp2 * temp2;  // This is dotProduct^4
	double lambda1, lambda2;
	temp = 1.0 - temp;  // This is 1 - dotProduct^4
	
	if (temp <= 0.0)
		temp = 0.0;  // There you go again...those pesky numbers!

	else
		temp = sqrt (temp);

	lambda1 = (1.0 - temp) / temp2;
	lambda2 = (1.0 + temp) / temp2;
	double sigma2;

	if (sigma1 > sigma0)
		sigma2 = lambda2 * sigma0;  // This is what we expect, since we started small

	else
		sigma2 = lambda1 * sigma0;

	// OK, it's getting a little confusing.  Here's what we have.  sigma1 is the sample standard deviation,
	// sigma0 is the original (too narrow?) standard deviation and sigma2 is the formula prediction of the 
	// correct standard deviation based on the dot product of sample with original trial Gaussian.  Before we
	// start iteratively improving our estimate, let's change sigma0 to be the average of sigma1 and sigma2...

//	sigma0 = 0.5 * (sigma1 + sigma2);
//	dotProduct = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

	double mean0 = sampleMean;
	double mean1 = center;   // ?????
	double mean2;
	double sigmas [3];
	double dotProducts [3];
	double means [3];

	int maxIterations = NormalizedGaussian::GetMaximumIterations ();
	int j;
	int MinimumValue;
	int WeAreDone = 0;

	double fitTolerance = ParametricCurve::GetFitTolerance ();
	double sigmaTolerance = NormalizedGaussian::GetSigmaTolerance ();
/*
	if (center < sampleMean)
		mean2 = center + IntervalSpacing;

	else
		mean2 = center - IntervalSpacing;

	means [1] = mean1;
	means [2] = mean2;
	means [0] = mean0;
	dotProducts [0] = dotProduct;
	dotProducts [1] = InnerProductWithOffsetAndStandardDeviation (mean1, sigma0, coeffs, SampleInfo) / sampleTwoNorm;
	dotProducts [2] = InnerProductWithOffsetAndStandardDeviation (mean2, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

	for (i=0; i<maxIterations; i++) {

		mean0 = BrentStep (means, dotProducts);
		dotProduct = InnerProductWithOffsetAndStandardDeviation (mean0, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

		// which do we replace?

		MinimumValue = MinIndex (dotProducts, 3);

		//  MimimumValue will be replaced by the new value

		if (dotProduct >= dotProducts [MinimumValue]) {

			means [MinimumValue] = mean0;
			dotProducts [MinimumValue] = dotProduct;

			if (dotProduct > fitTolerance) {

				// done!!
				break;
			}

			for (j=0; j<3; j++) {

				if (j != MinimumValue) {

					if (abs (means [j] - mean0) < mean0 * 1.0e-8) {

						WeAreDone = 1;
						break;
					}
				}
			}

			if (WeAreDone)
				break;
		}

		else {

			// done!!!
			break;
		}
	}*/

	int MaximumValue;  // = MaxIndex (dotProducts, 3);
//	sampleMean = means [MaximumValue];

	// 5:

	// Need to get the dot products and put everything into an array...then iterate (5 times max).
	// The one for sigma0 is in dotProduct, but must compute the other two.

	sigmas [0] = sigma0;
	dotProducts [0] = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;
	scaleFactors [0] = coeffs [0];

	sigmas [1] = sigma1;
	sigmas [2] = sigma2;

	dotProducts [1] = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma1, coeffs, SampleInfo) / sampleTwoNorm;
	scaleFactors [1] = coeffs [0];
	dotProducts [2] = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma2, coeffs, SampleInfo) / sampleTwoNorm;
	scaleFactors [2] = coeffs [0];
	WeAreDone = 0;

	MaximumValue = MaxIndex (dotProducts, 3);

//	if (dotProducts [MaximumValue] < ParametricCurve::GetAbsoluteMinimumFit ()) {
//
//		// Pack it in...there's no way to recover from this poor a start
//
//		fit = dotProducts [MaximumValue];
////		sampleMean = means [MaximumValue];
//		sigma0 = sigmas [MaximumValue];
//		DataSignal* returnValue = new Gaussian (left, right, sampleMean, sigma0); //  (left, right, sampleMean, sigma0);
//		returnValue->SetCurveFit (fit);
//
//		if (Overlapped != Target)
//			delete Overlapped;
//
//		return returnValue;
//	}

	for (i=0; i<maxIterations; i++) {

		sigma0 = BrentStep (sigmas, dotProducts);

		if (sigma0 <= 0.0) {

			MinimumValue = MinIndex (sigmas, 3);
			sigma0 = 0.5 * sigmas [MinimumValue];
		}

		dotProduct = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

		// which do we replace?

		MinimumValue = MinIndex (dotProducts, 3);

		//  MimimumValue will be replaced by the new value

		if (dotProduct >= dotProducts [MinimumValue]) {

			sigmas [MinimumValue] = sigma0;
			dotProducts [MinimumValue] = dotProduct;
			scaleFactors [MinimumValue] = coeffs [0];

			if (dotProduct > fitTolerance) {

				// done!!
				break;
			}

			for (j=0; j<3; j++) {

				if (j != MinimumValue) {

					if (fabs (sigmas [j] - sigma0) < sigma0 * sigmaTolerance) {

						WeAreDone = 1;
						break;
					}
				}
			}

			if (WeAreDone)
				break;
		}

		else {

			// done!!!
			break;
		}
	}

	MaximumValue = MaxIndex (dotProducts, 3);

	fit = dotProducts [MaximumValue];
	sigma0 = sigmas [MaximumValue];
	newScale = scaleFactors [MaximumValue];

	// The following was entered in order to improve fit by iterating on the mean.  The effect was
	//  too small and potentially unpredictable.  To make any true improvement at this point, it
	//  would probably be necessary to iterate on both sigma and mean simultaneously.

	if (fit < 0.999) {

		/*if (center < sampleMean)
			mean2 = center + 2.0 * IntervalSpacing;

		else
			mean2 = center - 2.0 * IntervalSpacing;*/

		mean2 = sampleMean + 4.0 * IntervalSpacing;
//		mean2 = sampleMean + IntervalSpacing;

		means [1] = mean1;
		means [2] = mean2;
		means [0] = mean0;
		dotProducts [0] = fit;
		scaleFactors [0] = newScale;
		dotProducts [1] = InnerProductWithOffsetAndStandardDeviation (mean1, sigma0, coeffs, SampleInfo) / sampleTwoNorm;
		scaleFactors [1] = coeffs [0];
		dotProducts [2] = InnerProductWithOffsetAndStandardDeviation (mean2, sigma0, coeffs, SampleInfo) / sampleTwoNorm;
		scaleFactors [2] = coeffs [0];
		WeAreDone = 0;

		for (i=0; i<maxIterations; i++) {

			mean0 = BrentStep (means, dotProducts);

			if (IsOutsideInterval (mean0, means [0], means [1], means [2]))
				break;

			dotProduct = InnerProductWithOffsetAndStandardDeviation (mean0, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

			// which do we replace?

			MinimumValue = MinIndex (dotProducts, 3);

			//  MimimumValue will be replaced by the new value

			if (dotProduct >= dotProducts [MinimumValue]) {

				means [MinimumValue] = mean0;
				dotProducts [MinimumValue] = dotProduct;
				scaleFactors [MinimumValue] = coeffs [0];

				if (dotProduct > fitTolerance) {

					// done!!
					break;
				}

				for (j=0; j<3; j++) {

					if (j != MinimumValue) {

						if (fabs (means [j] - mean0) < mean0 * 1.0e-8) {

							WeAreDone = 1;
							break;
						}
					}
				}

				if (WeAreDone)
					break;
			}

			else {

				// done!!!
				break;
			}
		}

		MaximumValue = MaxIndex (dotProducts, 3);

		fit = dotProducts [MaximumValue];
		sampleMean = means [MaximumValue];
		newScale = scaleFactors [MaximumValue];
	}

	newScale = fit * sampleTwoNorm / (sqrt (sqrtPi * sigma0));

	DataSignal* returnValue = new Gaussian (left, right, sampleMean, sigma0);
	returnValue->SetScale (newScale);
	returnValue->SetLeftEndPoint (left);
	returnValue->SetRightEndPoint (right);
	returnValue->SetCurrentDataInterval (Segment);

	if ((sampleMean < (double)Segment->GetLeft ()) || (sampleMean > (double)Segment->GetRight ())) {

		returnValue->SetMayBeUnacceptable (true);
		//fit = 0.0;
		//returnValue->SetCurveFit (0.0);
	}

	//if (fabs (sampleMean - (double)Segment->GetMode ()) > 5.0) {

	//	fit = 0.0;
	//	returnValue->SetCurveFit (0.0);
	//}

	// Artificial spike fix 10/04/2012

	double segPeak = Segment->GetMaxAtMode ();
	bool shortSegment = (Segment->GetRight () - Segment->GetLeft () <= 5);
	bool peakMismatch = (fabs (returnValue->Peak () - segPeak) > 0.3 * segPeak);

	if (shortSegment || peakMismatch) {

		//left = sampleMean - 3.0 * sigma0;
		//right = sampleMean + 3.0 * sigma0;
		//Target->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment);

		//	//double OGCoeff1 = Segment->GetMaxAtMode ();
		//double lowDataFactor = fit * sqrt (sampleTwoNorm2) / returnValue->Norm (left, right);
		//	//double peak = returnValue->Peak ();
		//returnValue->SetPeak (lowDataFactor * returnValue->Peak ());
			returnValue->SetPeak (segPeak);
		//	cout << "Gaussian peak with low data: Left = " << left << ". Right = " << right << ". Factor = " << segPeak << ".  Fit = " << fit << endl;
//		}
	}

	return returnValue;
}


DataSignal* NormalizedGaussian :: GetResidual (const DataSignal* base, double coeff) { return NULL; }


int NormalizedGaussian :: Add (list<DataSignal*>& WaveList) { return -1; }  // list of pointers to DataSignal


/*
double NormalizedGaussian :: InnerProductWithOffset (double offset, double* coeffs, const SampleDataInfo& info) const {

	//
	//  This is a "dot" product based on integration using the trapezoidal rule:  assumes
	//  target is a sampled curve and it is to be "convoluted" with a normalized Gaussian
	//  that is sampled at its center plus an input offset.  The purpose is to refine the
	//  Gaussian fit in the neighborhood of the sampled max.  This should not have to be
	//  evaluated many times - use Brent's method or some other involving iterative inverse
	//  parabolic interpolation.
	//
	//  Note:  double* target should be midpoint of array!!!
	//
	
	const double* ptargetPlus = info.DataCenter;
	const double* ptargetPlusEnd = info.DataRight;
	const double* ptargetMinus = ptargetPlus;
	double sampleSpacing = info.Spacing;
	double center = info.AbscissaLeft + info.IndexCenter * sampleSpacing;
	double x = (center - offset) / StandardDeviation;
	x *= 0.5 * x;

	double sum = exp (-x) * (*ptargetPlus);
//	int size = sampleSize - 1;
	int j = 0;
	double refRight;
	double refLeft;
	double y;
	double temp;

	for (ptargetPlus=info.DataCenter+1; ptargetPlus<ptargetPlusEnd; ptargetPlus++) {

		j++;
		ptargetMinus--;
		temp = j * sampleSpacing;
		refRight = center + temp;
		refLeft = center - temp;
		x = (refRight - offset) / StandardDeviation;
		y = (refLeft - offset) / StandardDeviation;
		x *= 0.5 * x;
		y *= 0.5 * y;
		sum += (*ptargetPlus) * exp (-x) + (*ptargetMinus) * exp (-y);
	}

	j++;
	ptargetPlus++;
	ptargetMinus--;
	temp = j * sampleSpacing;
	refRight = center + temp;
	refLeft = center - temp;
	x = (refRight - offset) / StandardDeviation;
	y = (refLeft - offset) / StandardDeviation;
	x *= 0.5 * x;
	y *= 0.5 * y;
	sum += 0.5 * ((*ptargetPlus) * exp (-x) + (*ptargetMinus) * exp (-y));

	sum *= sampleSpacing * Scale;
	return sum;
}*/

/*
double NormalizedGaussian :: InnerProductWithStandardSpacing (double* coeffs, const SampleDataInfo& info) const {

	//
	//  This is a "dot" product based on integration using the trapezoidal rule:  assumes
	//  target is a sampled curve and it is to be "convoluted" with a normalized Gaussian
	//  that is sampled at its center using no offset and based on previously sampled values.
	//
	
	if (Sample == NULL)
		return -2.0;

	const double* ptargetPlus = info.DataCenter;
	const double* ptargetPlusEnd = info.DataRight;
	const double* ptargetMinus = ptargetPlus;
	double* pplus = Sample + MidPoint;
	double sum = *pplus * (*ptargetPlus);
//	int size = SampleSize - 1;

	for (ptargetPlus=info.DataCenter+1; ptargetPlus<ptargetPlusEnd; ptargetPlus++) {

		pplus++;
		ptargetMinus--;
		sum += (*ptargetPlus + *ptargetMinus) * (*pplus);
	}

	pplus++;
	ptargetPlus++;
	ptargetMinus--;
	sum += 0.5 * (*ptargetPlus + *ptargetMinus) * (*pplus);
	sum *= SampleSpacing;
	return sum;
}*/


double NormalizedGaussian :: Norm () {

	return 1.0;
}


double NormalizedGaussian :: Norm (double left, double right) {

	if ((left <= Left) && (right >= Right))
		return 1.0;
	
	return Gaussian::Norm (left, right);
}


double NormalizedGaussian :: Norm2 () {
	
	// norm squared
	return 1.0;
}


double NormalizedGaussian :: Norm2 (double left, double right) {

	if ((left <= Left) && (right >= Right))
		return 1.0;

	return Gaussian::Norm2 (left, right);
}


DataSignal* NormalizedGaussian :: Normalize (double& norm) {

	norm = 1.0;
	DataSignal* ds = new NormalizedGaussian (Mean, StandardDeviation);
	return ds;
}


DataSignal* NormalizedGaussian :: Normalize (double left, double right, double& norm) {

	return Gaussian::Normalize (left, right, norm);
}


size_t NormalizedGaussian :: StoreSize () const {

	return Gaussian::StoreSize ();
}


void NormalizedGaussian :: RestoreAll (RGFile& f) {

	Gaussian::RestoreAll (f);
}


void NormalizedGaussian :: RestoreAll (RGVInStream& f) {

	Gaussian::RestoreAll (f);
}


void NormalizedGaussian :: SaveAll (RGFile& f) const {

	Gaussian::SaveAll (f);
}


void NormalizedGaussian :: SaveAll (RGVOutStream& f) const {

	Gaussian::SaveAll (f);
}


double NormalizedGaussian :: InnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* coeffs, const SampleDataInfo& info) const {

	//
	//  This is a "dot" product based on integration using the trapezoidal rule:  assumes
	//  target is a sampled curve and it is to be "convoluted" with a normalized Gaussian
	//  that is sampled at its center plus an input offset with input standard deviation, sigma.  
	//  The purpose is to refine the Gaussian fit in the neighborhood of the sampled max.  This 
	//  should not have to be evaluated many times - use Brent's method or some other involving iterative 
	//  inverse parabolic interpolation.
	//
	//  Note:  double* target should be midpoint of array!!!
	//
	
	const double* ptargetPlus = info.DataCenter;
	const double* ptargetPlusEnd = info.DataRight;
	const double* ptargetMinus = ptargetPlus;
	double sampleSpacing = info.Spacing;
	double center = info.AbscissaLeft + info.IndexCenter * sampleSpacing;
	double x = (center - offset) / sigma;
	x *= 0.5 * x;
	double sum = exp (-x) * (*ptargetPlus);
//	int size = sampleSize - 1;
	int j = 0;
	double refRight;
	double refLeft;
	double y;
	double temp;

	for (ptargetPlus=info.DataCenter+1; ptargetPlus<ptargetPlusEnd; ptargetPlus++) {

		j++;
		ptargetMinus--;
		temp = j * sampleSpacing;
		refRight = center + temp;
		refLeft = center - temp;
		x = (refRight - offset) / sigma;
		y = (refLeft - offset) / sigma;
		x *= 0.5 * x;
		y *= 0.5 * y;
		sum += (*ptargetPlus) * exp (-x) + (*ptargetMinus) * exp (-y);
	}

	j++;
	ptargetMinus--;
	temp = j * sampleSpacing;
	refRight = center + temp;
	refLeft = center - temp;
	x = (refRight - offset) / sigma;
	y = (refLeft - offset) / sigma;
	x *= 0.5 * x;
	y *= 0.5 * y;
	sum += 0.5 * ((*ptargetPlus) * exp (-x) + (*ptargetMinus) * exp (-y));

	sum *= sampleSpacing / (sqrt (sqrtPi * sigma));
	return sum;
}


double NormalizedGaussian :: AsymmetricInnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* coeffs, const SampleDataInfo& info) const {

	const double* ptargetPlus = info.DataLeft;
	const double* ptargetPlusEnd = info.DataRight;
	double sampleSpacing = info.Spacing;
	double left = info.AbscissaLeft + info.Spacing * info.IndexLeft;
	double x = (left - offset) / sigma;
	x *= 0.5 * x;
	double sum = 0.5 * exp (-x) * (*ptargetPlus);
	int j = 0;
	double temp;
	double ref;

	for (ptargetPlus=info.DataLeft+1; ptargetPlus<ptargetPlusEnd; ptargetPlus++) {

		j++;
		temp = j * sampleSpacing;
		ref = left + temp;
		x = (ref - offset) / sigma;
		x *= 0.5 * x;
		sum += (*ptargetPlus) * exp (-x);
	}

	j++;
	temp = j * sampleSpacing;
	ref = left + temp;
	x = (ref - offset) / sigma;
	x *= 0.5 * x;
	sum += 0.5 * (*ptargetPlus) * exp (-x);

	sum *= sampleSpacing / (sqrt (sqrtPi * sigma));  // this is inner product of sample with first Gaussian
	*coeffs = sum / (sqrt (sqrtPi * sigma));  // This coefficient must be scaled by the sample two norm
	return sum;
}


DoubleGaussian :: DoubleGaussian () : ParametricCurve (), PrimaryScale (1.0), SecondaryScale (1.0), Mean (0.0), 
StandardDeviation (1.0) {

	SigmaRatio = 1.0;
	SqrtSigmaRatio = 1.0;

	PrimaryCurve = NULL;
	SecondaryCurve = NULL;

	Left = -3.5;
	Right = 3.5;

	Correlation = 1.0;
	PrimaryNorm2 = SecondaryNorm2 = 1.0;
	OrthogonalPrimaryScale = 1.0;
	OrthogonalSecondaryScale = 1.0;
	NormOfSimpleResidual = 1.0;
}


DoubleGaussian :: DoubleGaussian (double mean, double standardDeviation) : 
ParametricCurve (mean - Gaussian::GetSigmaWidth()*standardDeviation, mean + Gaussian::GetSigmaWidth()*standardDeviation), 
PrimaryScale (1.0), SecondaryScale (1.0), Mean (mean), StandardDeviation (standardDeviation) {

	SigmaRatio = DoubleGaussian::GetDefaultSigmaRatio ();
	SqrtSigmaRatio = DoubleGaussian::GetDefaultSqrtSigmaRatio ();

	PrimaryCurve = new NormalizedGaussian (mean, standardDeviation);
	SecondaryCurve = new NormalizedGaussian (mean, SigmaRatio * standardDeviation);

	Correlation = sqrt ((2.0 * SigmaRatio) / (1.0 + SigmaRatio * SigmaRatio));
	PrimaryNorm2 = PrimaryCurve->Norm2 ();
	SecondaryNorm2 = SecondaryCurve->Norm2 (Left, Right);
	NormOfSimpleResidual = SecondaryNorm2 - (Correlation * Correlation / PrimaryNorm2);
	OrthogonalPrimaryScale = PrimaryScale + (SecondaryScale * Correlation / PrimaryNorm2);
	OrthogonalSecondaryScale = SecondaryScale * NormOfSimpleResidual;
}


DoubleGaussian :: DoubleGaussian (double mean, double standardDeviation, double sigmaRatio) :
ParametricCurve (mean - Gaussian::GetSigmaWidth()*standardDeviation, mean + Gaussian::GetSigmaWidth()*standardDeviation), 
PrimaryScale (1.0), SecondaryScale (1.0), Mean (mean), StandardDeviation (standardDeviation) {

	SigmaRatio = sigmaRatio;
	SqrtSigmaRatio = sqrt (sigmaRatio);

	PrimaryCurve = new NormalizedGaussian (mean, standardDeviation);
	SecondaryCurve = new NormalizedGaussian (mean, SigmaRatio * standardDeviation);

	Correlation = sqrt ((2.0 * SigmaRatio) / (1.0 + SigmaRatio * SigmaRatio));
	PrimaryNorm2 = PrimaryCurve->Norm2 ();
	SecondaryNorm2 = SecondaryCurve->Norm2 ();
	NormOfSimpleResidual = SecondaryNorm2 - (Correlation * Correlation / PrimaryNorm2);
	OrthogonalPrimaryScale = PrimaryScale + (SecondaryScale * Correlation / PrimaryNorm2);
	OrthogonalSecondaryScale = SecondaryScale * NormOfSimpleResidual;
}


DoubleGaussian :: DoubleGaussian (const DoubleGaussian& dg) : ParametricCurve (dg), SampleSpacing (dg.SampleSpacing), 
PrimaryScale (dg.PrimaryScale), SecondaryScale (dg.SecondaryScale), SigmaRatio (dg.SigmaRatio), SqrtSigmaRatio (dg.SqrtSigmaRatio),
OrthogonalPrimaryScale (dg.OrthogonalPrimaryScale), OrthogonalSecondaryScale (dg.OrthogonalSecondaryScale),
Correlation (dg.Correlation), NormOfSimpleResidual (dg.NormOfSimpleResidual), PrimaryNorm2 (dg.PrimaryNorm2),
SecondaryNorm2 (dg.SecondaryNorm2), Mean (dg.Mean), StandardDeviation (dg.StandardDeviation) {

	PrimaryCurve = new NormalizedGaussian (Mean, StandardDeviation);
	SecondaryCurve = new NormalizedGaussian (Mean, SigmaRatio * StandardDeviation);
}


DoubleGaussian :: DoubleGaussian (double mean, const DoubleGaussian& dg) : 
ParametricCurve (mean - Gaussian::GetSigmaWidth()*dg.StandardDeviation, mean + Gaussian::GetSigmaWidth()*dg.StandardDeviation), 
SampleSpacing (dg.SampleSpacing), PrimaryScale (dg.PrimaryScale), SecondaryScale (dg.SecondaryScale), SigmaRatio (dg.SigmaRatio), SqrtSigmaRatio (dg.SqrtSigmaRatio), 
OrthogonalPrimaryScale (dg.OrthogonalPrimaryScale), OrthogonalSecondaryScale (dg.OrthogonalSecondaryScale),
Correlation (dg.Correlation), NormOfSimpleResidual (dg.NormOfSimpleResidual), PrimaryNorm2 (dg.PrimaryNorm2),
SecondaryNorm2 (dg.SecondaryNorm2), Mean (mean), StandardDeviation (dg.StandardDeviation) {

	PrimaryCurve = new NormalizedGaussian (Mean, StandardDeviation);
	SecondaryCurve = new NormalizedGaussian (Mean, SigmaRatio * StandardDeviation);
}


DoubleGaussian :: DoubleGaussian (const DoubleGaussian& dg, CoordinateTransform* trans) : ParametricCurve (dg, trans),
SampleSpacing (dg.SampleSpacing), PrimaryScale (dg.PrimaryScale), SecondaryScale (dg.SecondaryScale), SigmaRatio (dg.SigmaRatio), SqrtSigmaRatio (dg.SqrtSigmaRatio), 
OrthogonalPrimaryScale (dg.OrthogonalPrimaryScale), OrthogonalSecondaryScale (dg.OrthogonalSecondaryScale),
Correlation (dg.Correlation), NormOfSimpleResidual (dg.NormOfSimpleResidual), PrimaryNorm2 (dg.PrimaryNorm2),
SecondaryNorm2 (dg.SecondaryNorm2) {

	// This is a double Gaussian transformed in time from an existing one.  It has the same peak, though...

	Mean = trans->EvaluateWithExtrapolation (dg.Mean);
	double meanPlusSigma = trans->EvaluateWithExtrapolation (dg.Mean + dg.StandardDeviation);
	StandardDeviation = meanPlusSigma - Mean;
	PrimaryCurve = new NormalizedGaussian (Mean, StandardDeviation);
	SecondaryCurve = new NormalizedGaussian (Mean, SigmaRatio * StandardDeviation);

	// We should really recompute Correlation and all the Norm's, but we don't intend to use them.  If it turns out that we need
	// them, this is where we should calculate them.  Right now, these are approximate values.
}


DoubleGaussian :: ~DoubleGaussian () {

	delete PrimaryCurve;
	delete SecondaryCurve;
}


DataSignal* DoubleGaussian :: MakeCopy (double mean) const {

	DataSignal* newSignal = new DoubleGaussian (mean, *this);
	return newSignal;
}


RGString DoubleGaussian :: GetSignalType () const {

	return RGString ("Double Gaussian");
}


void DoubleGaussian :: SetScale (double scale) {

	//  Correlation, PrimaryNorm2 (which should be 1) and NormOfSimpleResidual are all 
	// intrinsic, because 'primary' and 'secondary' are unit vectors.  Their coefficients
	// are external.
	
	PrimaryScale = scale;
	OrthogonalPrimaryScale = PrimaryScale + (SecondaryScale * Correlation / PrimaryNorm2);
}


void DoubleGaussian :: SetScale (int curve, double scale) {

	//  Correlation, PrimaryNorm2 (which should be 1) and NormOfSimpleResidual are all 
	// intrinsic, because 'primary' and 'secondary' are unit vectors.  Their coefficients
	// are external.

	if (curve <= 1) {

		SetScale (scale);
	}

	else {

		SecondaryScale = scale;
		OrthogonalPrimaryScale = PrimaryScale + (SecondaryScale * Correlation / PrimaryNorm2);
		OrthogonalSecondaryScale = SecondaryScale * NormOfSimpleResidual;
	}
}


void DoubleGaussian :: SetOrthogonalScale (int curve, double scale) {

	if (curve <= 1) {

		OrthogonalPrimaryScale = scale;
		PrimaryScale = OrthogonalPrimaryScale - (OrthogonalSecondaryScale * Correlation / (NormOfSimpleResidual * PrimaryNorm2));
	}

	else {

		OrthogonalSecondaryScale = scale;
		PrimaryScale = OrthogonalPrimaryScale - (OrthogonalSecondaryScale * Correlation / (NormOfSimpleResidual * PrimaryNorm2));
		SecondaryScale = OrthogonalSecondaryScale / NormOfSimpleResidual;
	}
}


void DoubleGaussian :: SetPeak (double peak) {

	double lambda = peak / Peak ();
	SetScale (1, PrimaryScale * lambda);
	SetScale (2, SecondaryScale * lambda);
}


double DoubleGaussian :: Value (double x) const {

	return PrimaryScale * PrimaryCurve->Value (x) + SecondaryScale * SecondaryCurve->Value (x);
}


double DoubleGaussian :: Peak () const {

	return PrimaryScale * PrimaryCurve->Peak () + SecondaryScale * SecondaryCurve->Peak ();
}


Boolean DoubleGaussian :: CanBeNegative () const {

	if (SecondaryScale < 0.0)
		return TRUE;

	return FALSE;
}


double DoubleGaussian :: GetMean () const {

	return Mean;
}


double DoubleGaussian :: GetStandardDeviation () const {

	return StandardDeviation;
}


double DoubleGaussian :: GetVariance () const {

	return StandardDeviation * StandardDeviation;
}



double DoubleGaussian :: GetScale (int curve) const {

	if (curve <= 1)
		return PrimaryScale / (sqrt (sqrtPi * StandardDeviation));

	return SecondaryScale / (sqrt (sqrtPi * SigmaRatio * StandardDeviation));
}


double DoubleGaussian :: GetOrthogonalScale (int curve) const {

	if (curve <= 1)
		return OrthogonalPrimaryScale;

	return OrthogonalSecondaryScale;
}


void DoubleGaussian :: ComputeTails (double& tailLeft, double& tailRight) const {

	double F1;
	double F2;
	double spacing = DataSignal::GetSampleSpacing ();
	double peak1 = PrimaryCurve->Peak ();
	double peak2 = SecondaryCurve->Peak ();

	if ((PrimaryScale <= 0.0) || (peak1 <= 0.0))
		F1 = 0.0;

	else	
		F1 = StandardDeviation * sqrt (2.0 * log (2.0 * PrimaryScale * peak1)) + spacing;

	if ((SecondaryScale <= 0.0) || (peak2 <= 0.0))
		F2 = 0.0;

	else
		F2 = SigmaRatio * StandardDeviation * sqrt (2.0 * log (2.0 * SecondaryScale * peak2)) + spacing;

	double F = F2;

	if (F1 > F2)
		F = F1;
	
	tailLeft = Mean - F;
	tailRight = Mean + F;
}


bool DoubleGaussian :: IsUnimodal () const {

	if ((PrimaryScale >= 0.0) && (SecondaryScale >= 0.0)) {

		if ((PrimaryScale > 0.0) || (SecondaryScale > 0.0))
			return true;

		return false;
	}

	double s1 = GetScale (1);
	double s2 = GetScale (2);
	double r2 = SigmaRatio * SigmaRatio;
	double p1 = s1 + s2 / r2;

	if (p1 > 0.0)
		return true;

	else if (p1 < 0.0)
		return false;

	if (s2 < 0.0)
		return true;

	return false;
}


double DoubleGaussian :: GetPullupToleranceInBP (double noise) const {

	//pullUpToleranceFactor
	double P = Peak ();

	if (P <= 0.0)
		return (mPullupTolerance + (2.0 * sin (0.5 * acos (Fit)) / 4.47));

	double localFit = Fit;

	if (localFit < 0.1)
		localFit = 0.1;

	double localNoise = noise;
	
	double LN = 0.95 * P;

	if (localNoise > LN)
		localNoise = LN;

	double temp1 = 1.0 / localFit;
	double temp = 2.0 * (temp1 * temp1 - 1.0) * StandardDeviation * log (P / (P - localNoise));
	return (mPullupTolerance + pullUpToleranceFactor * mApproxBioIDPrime * sqrt (temp));
}



DataSignal* DoubleGaussian :: FindNextCharacteristicFromRight (const DataSignal& Signature, double& fit, RGDList& previous) {

	return NULL;
}


DataSignal* DoubleGaussian :: FindNextCharacteristicFromLeft (const DataSignal& Signature, double& fit, RGDList& previous) {

	return NULL;
}


DataSignal* DoubleGaussian :: FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, 
int windowSize, double& fit, RGDList& previous) const {

	//
	//  OK, so this is similar to NormalizedGaussian.  First, assume that the standard deviation, sigma (sigma0) of this 
	// DoubleGaussian, is less than that for any of the curves to fit.  Given that, then here's what to do:
	// 1)  Find the interval of definition of the Target from Segment, refine it and make it symmetric about 
	//     the center, using windowSize; 
	// 2)  Calculate the sample mean and sample standard deviation (sigma1) on the refined interval; remember that
	//     the value sigma1 will be simultaneously shared at different levels by two Gaussians;
	// 3)  Calculate the Dot Product with the native DoubleGaussian using the offset of the sample mean; 
	// 4)  Calculate the predicted sigma (sigma2) based on the formula and the dot product; the formula is:
	//     for b = dot product, ratio r between true sigma and tested sigma satisfies b^2 * r^2 - 2 * r + b^2 = 0,
	//     which can be solved using the quadratic formula to give r = (1 +- sqrt (1 - b^4)) / b^2.  The two solutions,
	//     r1 and r2 satisfy r1 * r2 = 1;
	// 5)  Using starting sigma (sigma0) for 3rd sigma, perform inverse parabolic interpolation to refine 
	//     sigma (1 - 5 iterations, max?); 
	// 6)  Create new DoubleGaussian with appropriate parameters and return...fit = Dot Product.
	//

	// 1:

	DataSignal* possibleSpike = Segment->TestForSpike (fit);

	if (possibleSpike != NULL)
		return possibleSpike;

	if (Segment->HasAFixedEndPoint ())
		return FindCharacteristicAsymmetric (Target, Segment, windowSize, fit, previous);

	double IntervalSpacing = DataSignal::GetSampleSpacing ();

	int SegmentLeft = Segment->GetLeft ();
	int SegmentRight = Segment->GetRight ();
	int SegmentCenter = Segment->GetCenter ();
	int SegmentWindow = SegmentRight - SegmentLeft;
	SegmentWindow -= windowSize;
	double coeffs [3];

	double minMu = Target->LeftEndPoint () + SegmentLeft * IntervalSpacing + 0.5 * IntervalSpacing;
	double maxMu = Target->LeftEndPoint () + SegmentRight * IntervalSpacing - 0.5 * IntervalSpacing;

	int MaxHalfWindow = SegmentCenter - SegmentLeft;   // Added to make sure we don't exceed Segment boundary (by much)
	int tempSeg = SegmentRight - SegmentCenter;   //  ditto
	
	if ((tempSeg < MaxHalfWindow) && (tempSeg > 0))   //  ditto
		MaxHalfWindow = tempSeg;   //  ditto

	int MaxWindow = 2 * MaxHalfWindow;   //  ditto

	SegmentWindow = 2 * ((SegmentWindow + 1) / 2);  // Gets SegmentWindow, if even, and SegmentWindow+1 if not

	if (SegmentWindow > MaxWindow)   // Added to make sure we don't exceed Segment boundary (by much)
		SegmentWindow = MaxWindow;   // Added to make sure we don't exceed Segment boundary (by much)

	// Get window for target Gaussian and compare; take whichever is larger
	
	double HalfTargetWindow = Gaussian::GetSampleSigmaWidth() * StandardDeviation;
	int HalfIntervalWindow = (int)floor((HalfTargetWindow / IntervalSpacing) + 0.9);
	int IntervalWindow = 2 * HalfIntervalWindow;

	if (IntervalWindow > SegmentWindow)
		SegmentWindow = IntervalWindow;

	int HalfSegmentWindow = SegmentWindow / 2;
	SegmentLeft = SegmentCenter - HalfSegmentWindow;
	SegmentRight = SegmentCenter + HalfSegmentWindow;

	int OriginalHalfSegmentWindow = HalfSegmentWindow;  // This is as large as we ever want this.  Don't make it bigger!

	if (HalfSegmentWindow = 0)
		return NULL;

	// 2:
	double left = Target->LeftEndPoint () + SegmentLeft * IntervalSpacing;
	double right = Target->LeftEndPoint () + SegmentRight * IntervalSpacing;
	double center = Target->LeftEndPoint () + SegmentCenter * IntervalSpacing;

	int OldSegmentLeft = SegmentLeft;

	if (left < 0.0)
		cout << "Original left is < 0:  " << left << "\n";

	const DataSignal* Overlapped = BuildOverlappedSignal (left, right, previous, Target);

	double sampleTwoNorm2;
	double sampleMean;
	double sampleTwoMoment;
	
	if (Overlapped->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment) != 0) {
		
		if (Overlapped != Target) {
			delete Overlapped;
			Overlapped = BuildOverlappedSignal (left, right, previous, Target);
		}

		return NULL;
	}

	if (sampleMean < minMu)
		sampleMean = minMu;

	else if (sampleMean > maxMu)
		sampleMean = maxMu;

	double sigma1 = sqrtTwo * sqrt (sampleTwoMoment);  // assume sampleTwoMoment is variance:  mean^2 already subtracted
	double sampleTwoNorm = sqrt (sampleTwoNorm2);

	//
	// The following is new and the purpose is to adjust the window to be more appropriate to the expected curve:  i.e., not too large
	//   Therefore, if it dictates an increase, don't do it!!!
	//

	double OldLeft = left;
	double OldRight = right;

	HalfTargetWindow = Gaussian::GetSampleSigmaWidth() * sigma1;
	HalfSegmentWindow = (int)floor((HalfTargetWindow / IntervalSpacing) + 0.9999);

	if (HalfSegmentWindow > OriginalHalfSegmentWindow)
		HalfSegmentWindow = OriginalHalfSegmentWindow;  // We want this process to DECREASE segment window, not increase it!!

	SegmentCenter = (int) floor ((sampleMean / IntervalSpacing) + 0.5);  // round sampleMean to get better estimate of sample center
//	IntervalWindow = 2 * HalfIntervalWindow;
	SegmentLeft = SegmentCenter - HalfSegmentWindow;  // We may have moved the segment a little but it is better centered now
	SegmentRight = SegmentCenter + HalfSegmentWindow;
	SegmentWindow = SegmentRight - SegmentLeft;
	SegmentWindow = 2 * ((SegmentWindow + 1) / 2);  // Gets SegmentWindow, if even, and SegmentWindow+1 if not
	left = Target->LeftEndPoint () + SegmentLeft * IntervalSpacing;
	right = Target->LeftEndPoint () + SegmentRight * IntervalSpacing;

	if ((SegmentLeft < 0.0) || (left < 0.0))
		cout << "Segment left or transformed left < 0:  " << left << ", " << SegmentLeft << "\n";

	if ((left < OldLeft) || (right > OldRight)) {

		if (Overlapped != Target)
			delete Overlapped;

		OldSegmentLeft = SegmentLeft;
		Overlapped = BuildOverlappedSignal (left, right, previous, Target);
	}

	// This calculates mean, variance and norm for new interval

	if (Overlapped->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment) != 0) {
		
		if (Overlapped != Target) {
			delete Overlapped;
			Overlapped = BuildOverlappedSignal (left, right, previous, Target);
		}
		
		return NULL;
	}

	if (sampleMean < minMu)
		sampleMean = minMu;

	else if (sampleMean > maxMu)
		sampleMean = maxMu;

	sigma1 = sqrtTwo * sqrt (sampleTwoMoment);  // assume sampleTwoMoment is variance:  mean^2 already subtracted
	sampleTwoNorm = sqrt (sampleTwoNorm2);

	// 3:
	int i;
	int NSamples = SegmentWindow + 1;

	if (Overlapped != Target) {

		SegmentRight -= OldSegmentLeft;
		SegmentCenter -= OldSegmentLeft;
		SegmentLeft -= OldSegmentLeft;
	}

	if (SegmentLeft < 0.0)
		cout << "Final Segment Left < 0:  " << SegmentLeft << endl;

	if (SegmentLeft < 10.0)
		cout << "DoubleGaussian with left segment = " << SegmentLeft << "\n";

	SampleDataInfo SampleInfo (*Overlapped, SegmentLeft, SegmentCenter, SegmentRight, NSamples);

	double sigma0 = StandardDeviation * sqrtTwo;  // sigma0, sigma1 and sigma2 should bracket true sigma, but maybe not!?

	// We're going to use our prediction formula here.  This formula gives the true (relative) standard deviation
	// based on the dot product, assuming we have the sample mean correct.  To do this, we need a function that only
	// performs the dot product on the primary Gaussian.  That maintains the validity of the formula (assuming, of 
	// course, that the sample mean is the "true" mean).

	//  The following should be normalized and between 0 and 1

	double dotProduct = PrimaryCurve->InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

	if (dotProduct > 1.0)
		dotProduct = 1.0;  // Just for safety...those numbers!  They sure can play tricks!

	// 4:

	// Ready or not, here comes the formula
	double temp2 = dotProduct * dotProduct;
	double temp = temp2 * temp2;  // This is dotProduct^4
	double lambda1, lambda2;
	temp = 1.0 - temp;  // This is 1 - dotProduct^4
	
	if (temp <= 0.0)
		temp = 0.0;  // There you go again...those pesky numbers!

	else
		temp = sqrt (temp);

	lambda1 = (1.0 - temp) / temp2;
	lambda2 = (1.0 + temp) / temp2;
	double sigma2;

	if (sigma1 > sigma0)
		sigma2 = lambda2 * sigma0;  // This is what we expect, since we started small

	else
		sigma2 = lambda1 * sigma0;  // Just in case, we include the other possibility

	// OK, it's getting a little confusing.  Here's what we have.  sigma1 is the sample standard deviation,
	// sigma0 is the original (too narrow?) standard deviation and sigma2 is the formula prediction of the 
	// correct standard deviation based on the dot product of sample with original trial Gaussian.  Before we
	// start iteratively improving our estimate, let's change sigma0 to be the average of sigma1 and sigma2...

//	sigma0 = 0.5 * (sigma1 + sigma2);
//  Don't need the following, with other commented out code which iterates on sampleMean...it's now redundant...see later...
//	dotProduct = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

	double mean0 = sampleMean;
	double mean1 = sampleMean - 4.0 * IntervalSpacing;

	if (mean1 < minMu)
		mean1 = minMu;

//	double mean1 = sampleMean - IntervalSpacing;
//	double mean2;
	double sigmas [3];
	double dotProducts [3];
	double* OGCoeffs [3];
	double OG0 [3];
	double OG1 [3];
	double OG2 [3];
	double OGNew [3];
	double* OGTemp;
	double means [3];

	int maxIterations = NormalizedGaussian::GetMaximumIterations ();
	int j;
	int MinimumValue;
	int WeAreDone = 0;

	double fitTolerance = ParametricCurve::GetFitTolerance ();
	double sigmaTolerance = NormalizedGaussian::GetSigmaTolerance ();
/*
	if (center < sampleMean)
		mean2 = center + IntervalSpacing;

	else
		mean2 = center - IntervalSpacing;

	means [1] = mean1;
	means [2] = mean2;
	means [0] = mean0;
	dotProducts [0] = dotProduct;
	dotProducts [1] = InnerProductWithOffsetAndStandardDeviation (mean1, sigma0, coeffs, SampleInfo) / sampleTwoNorm;
	dotProducts [2] = InnerProductWithOffsetAndStandardDeviation (mean2, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

	for (i=0; i<maxIterations; i++) {

		mean0 = BrentStep (means, dotProducts);
		dotProduct = InnerProductWithOffsetAndStandardDeviation (mean0, sigma0, double* coeffs, SampleInfo) / sampleTwoNorm;

		// which do we replace?

		MinimumValue = MinIndex (dotProducts, 3);

		//  MimimumValue will be replaced by the new value

		if (dotProduct >= dotProducts [MinimumValue]) {

			means [MinimumValue] = mean0;
			dotProducts [MinimumValue] = dotProduct;

			if (dotProduct > fitTolerance) {

				// done!!
				break;
			}

			for (j=0; j<3; j++) {

				if (j != MinimumValue) {

					if (abs (means [j] - mean0) < mean0 * 1.0e-8) {

						WeAreDone = 1;
						break;
					}
				}
			}

			if (WeAreDone)
				break;
		}

		else {

			// done!!!
			break;
		}
	}*/

	int MaximumValue;  // = MaxIndex (dotProducts, 3);
//	sampleMean = means [MaximumValue];

	// 5:

	// Need to get the dot products and put everything into an array...then iterate (NormalizedGaussian::MaximumIterations times max).
	// The one for sigma0 is in dotProduct, but must compute the other two.

	sigmas [0] = sigma0;
	dotProducts [0] = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, OG0, SampleInfo) / sampleTwoNorm;
	OGCoeffs [0] = OG0;

	sigmas [1] = sigma1;
	sigmas [2] = sigma2;

	dotProducts [1] = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma1, OG1, SampleInfo) / sampleTwoNorm;
	dotProducts [2] = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma2, OG2, SampleInfo) / sampleTwoNorm;

	OGCoeffs [1] = OG1;
	OGCoeffs [2] = OG2;
	WeAreDone = 0;

	MaximumValue = MaxIndex (dotProducts, 3);

//	if (dotProducts [MaximumValue] < ParametricCurve::GetAbsoluteMinimumFit ()) {
//
//		// Pack it in...there's no way to recover from this poor a start
//
//		fit = dotProducts [MaximumValue];
////		sampleMean = means [MaximumValue];
//		sigma0 = sigmas [MaximumValue];
//		OGTemp = OGCoeffs [MaximumValue];
//		double OGCoeff1 = *OGTemp;
//		double OGCoeff2 = *(OGTemp + 1);
//		DataSignal* returnValue = new DoubleGaussian (sampleMean, sigma0, SigmaRatio); //  (left, right, sampleMean, sigma0);
//		returnValue->SetScale (1, OGCoeff1);
//		returnValue->SetScale (2, OGCoeff2);
//		returnValue->SetCurveFit (fit);
//
//		if (Overlapped != Target)
//			delete Overlapped;
//
//		return returnValue;
//	}

	for (i=0; i<maxIterations; i++) {

		sigma0 = BrentStep (sigmas, dotProducts);

		if (sigma0 <= 0.0) {

			MinimumValue = MinIndex (sigmas, 3);
			sigma0 = 0.5 * sigmas [MinimumValue];
		}

		dotProduct = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, OGNew, SampleInfo) / sampleTwoNorm;

		// which do we replace?

		MinimumValue = MinIndex (dotProducts, 3);

		//  MimimumValue will be replaced by the new value

		if (dotProduct >= dotProducts [MinimumValue]) {

			sigmas [MinimumValue] = sigma0;
			dotProducts [MinimumValue] = dotProduct;
			OGTemp = OGCoeffs [MinimumValue];
			OGTemp [0] = OGNew [0];
			OGTemp [1] = OGNew [1];
			OGTemp [2] = OGNew [2];

			if (dotProduct > fitTolerance) {

				// done!!
				break;
			}

			for (j=0; j<3; j++) {

				if (j != MinimumValue) {

					if (fabs (sigmas [j] - sigma0) < sigma0 * sigmaTolerance) {

						WeAreDone = 1;
						break;
					}
				}
			}

			if (WeAreDone)
				break;
		}

		else {

			// done!!!
			break;
		}
	}

	MaximumValue = MaxIndex (dotProducts, 3);

	fit = dotProducts [MaximumValue];
	sigma0 = sigmas [MaximumValue];
	OGTemp = OGCoeffs [MaximumValue];
	double OGCoeff1 = *OGTemp;
	double OGCoeff2 = *(OGTemp + 1);

	// The following was entered in order to improve fit by iterating on the mean.  The effect was
	//  too small and potentially unpredictable.  To make any true improvement at this point, it
	//  would probably be necessary to iterate on both sigma and mean simultaneously.

	double mean2;

	if (fit < 0.999) {

		/*if (center < sampleMean)
			mean2 = center + 2.0 * IntervalSpacing;

		else
			mean2 = center - 2.0 * IntervalSpacing;*/

		mean2 = sampleMean + 4.0 * IntervalSpacing;

		if (mean2 > maxMu)
			mean2 = maxMu;

//		mean2 = sampleMean + IntervalSpacing;

		means [1] = mean1;
		means [2] = mean2;
		means [0] = mean0;
		dotProducts [0] = fit;
		OG0 [0] = OGCoeff1;
		OG0 [1] = OGCoeff2;
		dotProducts [1] = InnerProductWithOffsetAndStandardDeviation (mean1, sigma0, OG1, SampleInfo) / sampleTwoNorm;
		dotProducts [2] = InnerProductWithOffsetAndStandardDeviation (mean2, sigma0, OG2, SampleInfo) / sampleTwoNorm;
		WeAreDone = 0;

		for (i=0; i<maxIterations; i++) {

			mean0 = BrentStep (means, dotProducts);

			if (IsOutsideInterval (mean0, means [0], means [1], means [2]))
				break;

			dotProduct = InnerProductWithOffsetAndStandardDeviation (mean0, sigma0, OGNew, SampleInfo) / sampleTwoNorm;

			// which do we replace?

			MinimumValue = MinIndex (dotProducts, 3);

			//  MimimumValue will be replaced by the new value

			if (dotProduct >= dotProducts [MinimumValue]) {

				means [MinimumValue] = mean0;
				dotProducts [MinimumValue] = dotProduct;
				OGTemp = OGCoeffs [MinimumValue];
				OGTemp [0] = OGNew [0];
				OGTemp [1] = OGNew [1];
				OGTemp [2] = OGNew [2];

				if (dotProduct > fitTolerance) {

					// done!!
					break;
				}

				for (j=0; j<3; j++) {

					if (j != MinimumValue) {

						if (fabs (means [j] - mean0) < mean0 * 1.0e-8) {

							WeAreDone = 1;
							break;
						}
					}
				}

				if (WeAreDone)
					break;
			}

			else {

				// done!!!
				break;
			}
		}

		MaximumValue = MaxIndex (dotProducts, 3);

		fit = dotProducts [MaximumValue];
		sampleMean = means [MaximumValue];
		OGTemp = OGCoeffs [MaximumValue];
		double OGCoeff1 = *OGTemp;
		double OGCoeff2 = *(OGTemp + 1);
	}

	//CHANGE WHAT FOLLOWS!!  Done...but better check it out...
//	double newScale = fit * sampleTwoNorm / (sqrt (sqrtPi * sigma0));  This is old...for Gaussians

	//  In instantiation below, do we need to communicate left and right?????

	DataSignal* returnValue = new DoubleGaussian (sampleMean, sigma0, SigmaRatio); //  (left, right, sampleMean, sigma0);
	returnValue->SetScale (1, OGCoeff1);
	returnValue->SetScale (2, OGCoeff2);
	returnValue->SetCurveFit (fit);
	returnValue->SetCurrentDataInterval (Segment);

	// These two lines below are removed because they truncate the curve too abruptly.  We want to fit curves based on a fairly narrow 
	// interval because the center of the sample curve contains the most reliable information, but then we want to carry the entire 
	// fitted curve (or very nearly) so we want to use the entire DoubleGaussian

//	returnValue->SetLeftEndPoint (left);
//	returnValue->SetRightEndPoint (right);

	if (Overlapped != Target)
		delete Overlapped;

	if ((sampleMean < (double)Segment->GetLeft ()) || (sampleMean > (double)Segment->GetRight ())) {

		returnValue->SetMayBeUnacceptable (true);
		//returnValue->SetBackupFit (fit);
		//fit = 0.0;
		//returnValue->SetCurveFit (0.0);
	}

	//if (fabs (sampleMean - (double)Segment->GetMode ()) > 5.0) {

	//	fit = 0.0;
	//	returnValue->SetCurveFit (0.0);
	//}

	// Artificial spike fix 10/04/2012

	double segPeak = Segment->GetMaxAtMode ();
	bool shortSegment = (Segment->GetRight () - Segment->GetLeft () <= 5);
	bool peakMismatch = (fabs (returnValue->Peak () - segPeak) > 0.3 * segPeak);

	if (shortSegment || peakMismatch) {

		//if (fit > 0.0) {
		//left = sampleMean - 3.0 * sigma0;
		//right = sampleMean + 3.0 * sigma0;
		//Target->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment);

		//	//double OGCoeff1 = Segment->GetMaxAtMode ();
		//double lowDataFactor = fit * sqrt (sampleTwoNorm2) / returnValue->Norm (left, right);
		//	//double peak = returnValue->Peak ();
		//returnValue->SetPeak (lowDataFactor * returnValue->Peak ());
			returnValue->SetPeak (segPeak);
		//	cout << " Double Gaussian peak with low data: Left = " << left << ". Right = " << right << ". Factor = " << segPeak << ".  Fit = " << fit << endl;
		//}
	}

	return returnValue;
}


DataSignal* DoubleGaussian :: FindCharacteristicAsymmetric (const DataSignal* Target, const DataInterval* Segment, int windowSize, double& fit, RGDList& previous) const {

	//
	//  OK, so this is similar to DoubleGaussian::FindCharacteristic except that we accept the fit boundaries as given.  
	//  First, assume that the standard deviation, sigma (sigma0) of this 
	// DoubleGaussian, is less than that for any of the curves to fit.  Given that, then here's what to do:
	// 1)  Leave the window for the fit unchanged; 
	// 2)  The sample mean and sample standard deviation (sigma1) on the interval are irrelevant; use the sample mode; remember that
	//     the value sigma1 will be simultaneously shared at different levels by two Gaussians;
	// 3)  Calculate the Dot Product with the native DoubleGaussian using the offset of the sample mean; 
	// 4)  Calculate the predicted sigma (sigma2) based on the formula and the dot product; the formula is:
	//     for b = dot product, ratio r between true sigma and tested sigma satisfies b^2 * r^2 - 2 * r + b^2 = 0,
	//     which can be solved using the quadratic formula to give r = (1 +- sqrt (1 - b^4)) / b^2.  The two solutions,
	//     r1 and r2 satisfy r1 * r2 = 1;  Note, however, that this formula was derived for the dot product of two
	//     FULL (double) Gaussians.  It probably does not apply to this partial dot product, but we are trying it as an
	//     act of faith (and hope) until we can derive another formula.
	// 5)  Using starting sigma (sigma0) for 3rd sigma, perform inverse parabolic interpolation to refine 
	//     sigma (1 - 5 iterations, max?); 
	// 6)  Create new DoubleGaussian with appropriate parameters and return...fit = Dot Product.
	//

	// 1:	

	DataSignal* possibleSpike = Segment->TestForSpike (fit);

	if (possibleSpike != NULL)
		return possibleSpike;

	int SegmentLeft = Segment->GetLeft ();
	int SegmentRight = Segment->GetRight ();
	int SegmentMode =  (int) Segment->GetMode ();
	int SegmentWindow = SegmentRight - SegmentLeft;
	double coeffs [3];
	
	double IntervalSpacing = DataSignal::GetSampleSpacing ();

	// 2:
	double left = Target->LeftEndPoint () + SegmentLeft * IntervalSpacing;
	double right = Target->LeftEndPoint () + SegmentRight * IntervalSpacing;
	double mode = Target->LeftEndPoint () + SegmentMode * IntervalSpacing;

//	const DataSignal* Overlapped = BuildOverlappedSignal (left, right, previous, Target);


	//
	// Compute/assign sampleMean, sampleTwoMoment and sampleTwoNorm here...
	//

	double sampleTwoNorm2 = 0.0;
	double sampleMean = mode;
	double sampleTwoMoment = 0.0;
	const double* data = Target->GetData ();
	const double* sampleValue = data + SegmentLeft + 1;
	double temp;
	double temp2;
  int i;
	for (i=1; i<SegmentWindow; i++) {

		temp = *sampleValue;
		sampleTwoNorm2 += temp * temp;
		temp2 = (left + i * IntervalSpacing - mode) * temp;
		sampleTwoMoment += temp2 * temp2;
		sampleValue++;
	}

	sampleValue = data + SegmentLeft;
	temp = *sampleValue;
	sampleTwoNorm2 += 0.5 * temp * temp;
	temp2 = (left - mode) * temp;
	sampleTwoMoment += 0.5 * temp2 * temp2;

	sampleValue = data + SegmentRight;
	temp = *sampleValue;
	sampleTwoNorm2 += 0.5 * temp * temp;
	temp2 = (right - mode) * temp;
	sampleTwoMoment += 0.5 * temp2 * temp2;

	if (sampleTwoNorm2 == 0.0)
		return NULL; // there is no data!

	sampleTwoMoment = (sampleTwoMoment / sampleTwoNorm2);

	//
	//  Since the sample is not a full curve, and asymmetrically represented at that, sampleTwoMoment will be too small to be used as an estimate
	//  for the true second moment
	//

	double sigma1 = sqrtTwo * sqrt (sampleTwoMoment);  // assume sampleTwoMoment is variance:  mean^2 already subtracted...it's too small but we'll deal
	double sampleTwoNorm = sqrt (sampleTwoNorm2);  // we'll see how this goes

	// 3:
	
	int NSamples = SegmentWindow + 1;
	SampleDataInfo SampleInfo (*Target, SegmentLeft, SegmentMode, SegmentRight, NSamples);

	double sigma0 = StandardDeviation * sqrtTwo;  // sigma0, sigma1 and sigma2 should bracket true sigma, but maybe not!?

	// We're going to use our prediction formula here.  This formula gives the true (relative) standard deviation
	// based on the dot product, assuming we have the sample mean correct.  To do this, we need a function that only
	// performs the dot product on the primary Gaussian.  That maintains the validity of the formula (assuming, of 
	// course, that the sample mean is the "true" mean).

	//  The following should be normalized and between 0 and 1, but now we need a Gaussian::InnerProductWith...Asymmetric

	double dotProduct = PrimaryCurve->AsymmetricInnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm; // fix this to reflect asymmetric data

	if (dotProduct > 1.0)
		dotProduct = 1.0;  // Just for safety...those numbers!  They sure can play tricks!

	// 4:

	// Ready or not, here comes the formula
	temp2 = dotProduct * dotProduct;
	temp = temp2 * temp2;  // This is dotProduct^4
	double lambda1, lambda2;
	temp = 1.0 - temp;  // This is 1 - dotProduct^4
	
	if (temp <= 0.0)
		temp = 0.0;  // There you go again...those pesky numbers!

	else
		temp = sqrt (temp);

	lambda1 = (1.0 - temp) / temp2;
	lambda2 = (1.0 + temp) / temp2;
	double sigma2;

	if (sigma1 > sigma0)
		sigma2 = lambda2 * sigma0;  // This is what we expect, since we started small

	else
		sigma2 = lambda1 * sigma0;  // Just in case, we include the other possibility

	// OK, it's getting a little confusing.  Here's what we have.  sigma1 is the sample standard deviation,
	// sigma0 is the original (too narrow?) standard deviation and sigma2 is the formula prediction of the 
	// correct standard deviation based on the dot product of sample with original trial Gaussian.  

	double mean0 = sampleMean;
	double mean1;
	double mean2;
	double width;

	if (SegmentMode == SegmentLeft) {

		width = right - left;
		mean1 = mean0 + 0.33 * width;
		mean2 = mean0 + 0.67 * width;
	}

	else if (SegmentMode == SegmentRight) {

		width = right - left;
		mean1 = mean0 - 0.67 * width;
		mean2 = mean0 - 0.33 * width;
	}

	else {

//		mean1 = 0.5 * (mean0 + left);
//		mean2 = 0.5 * (mean0 + right);
		mean1 = mean0 - 0.25 * (mean0 - left);
		mean2 = mean0 + 0.25 * (right - mean0);
	}

//	double mean1 = sampleMean - IntervalSpacing;
//	double mean2;
	double sigmas [3];
	double dotProducts [3];
	double* OGCoeffs [3];
	double OG0 [3];
	double OG1 [3];
	double OG2 [3];
	double OGNew [3];
	double* OGTemp;
	double means [3];

	int maxIterations = NormalizedGaussian::GetMaximumIterations ();
	int j;
	int MinimumValue;
	int WeAreDone = 0;

	double fitTolerance = ParametricCurve::GetFitTolerance ();
	double sigmaTolerance = NormalizedGaussian::GetSigmaTolerance ();

	int MaximumValue;  // = MaxIndex (dotProducts, 3);

	// 5:

	// Need to get the dot products and put everything into an array...then iterate (NormalizedGaussian::MaximumIterations times max).
	// The one for sigma0 is in dotProduct, but must compute the other two.

	sigmas [0] = sigma0;
	dotProducts [0] = AsymmetricInnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, OG0, SampleInfo) / sampleTwoNorm;
	OGCoeffs [0] = OG0;

	sigmas [1] = sigma1;
	sigmas [2] = sigma2;

	dotProducts [1] = AsymmetricInnerProductWithOffsetAndStandardDeviation (sampleMean, sigma1, OG1, SampleInfo) / sampleTwoNorm;
	dotProducts [2] = AsymmetricInnerProductWithOffsetAndStandardDeviation (sampleMean, sigma2, OG2, SampleInfo) / sampleTwoNorm;

	OGCoeffs [1] = OG1;
	OGCoeffs [2] = OG2;
	WeAreDone = 0;

	MaximumValue = MaxIndex (dotProducts, 3);

//	if (dotProducts [MaximumValue] < ParametricCurve::GetAbsoluteMinimumFit ()) {
//
//		// Pack it in...there's no way to recover from this poor a start
//
//		fit = dotProducts [MaximumValue];
////		sampleMean = means [MaximumValue];
//		sigma0 = sigmas [MaximumValue];
//		OGTemp = OGCoeffs [MaximumValue];
//		double OGCoeff1 = *OGTemp;
//		double OGCoeff2 = *(OGTemp + 1);
//		DataSignal* returnValue = new DoubleGaussian (sampleMean, sigma0, SigmaRatio); //  (left, right, sampleMean, sigma0);
//		returnValue->SetScale (1, OGCoeff1);
//		returnValue->SetScale (2, OGCoeff2);
//		returnValue->SetCurveFit (fit);
//
//		return returnValue;
//	}

	for (i=0; i<maxIterations; i++) {

		sigma0 = BrentStep (sigmas, dotProducts);

		if (sigma0 <= 0.0) {

			MinimumValue = MinIndex (sigmas, 3);
			sigma0 = 0.5 * sigmas [MinimumValue];
		}

		dotProduct = AsymmetricInnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, OGNew, SampleInfo) / sampleTwoNorm;

		// which do we replace?

		MinimumValue = MinIndex (dotProducts, 3);

		//  MimimumValue will be replaced by the new value

		if (dotProduct >= dotProducts [MinimumValue]) {

			sigmas [MinimumValue] = sigma0;
			dotProducts [MinimumValue] = dotProduct;
			OGTemp = OGCoeffs [MinimumValue];
			OGTemp [0] = OGNew [0];
			OGTemp [1] = OGNew [1];
			OGTemp [2] = OGNew [2];

			if (dotProduct > fitTolerance) {

				// done!!
				break;
			}

			for (j=0; j<3; j++) {

				if (j != MinimumValue) {

					if (fabs (sigmas [j] - sigma0) < sigma0 * sigmaTolerance) {

						WeAreDone = 1;
						break;
					}
				}
			}

			if (WeAreDone)
				break;
		}

		else {

			// done!!!
			break;
		}
	}

	MaximumValue = MaxIndex (dotProducts, 3);

	fit = dotProducts [MaximumValue];
	sigma0 = sigmas [MaximumValue];
	OGTemp = OGCoeffs [MaximumValue];
	double OGCoeff1 = *OGTemp;
	double OGCoeff2 = *(OGTemp + 1);

	// The following was entered in order to improve fit by iterating on the mean.  The effect was
	//  too small and potentially unpredictable.  To make any true improvement at this point, it
	//  would probably be necessary to iterate on both sigma and mean simultaneously.  However, this was
	// for the full symmetric fit.  With this one, we could be off by several seconds, so it's worth it!!!

	if (fit < 0.999) {

//		mean2 = sampleMean + IntervalSpacing;

		means [1] = mean1;
		means [2] = mean2;
		means [0] = mean0;
		dotProducts [0] = fit;
		OG0 [0] = OGCoeff1;
		OG0 [1] = OGCoeff2;
		dotProducts [1] = AsymmetricInnerProductWithOffsetAndStandardDeviation (mean1, sigma0, OG1, SampleInfo) / sampleTwoNorm;
		dotProducts [2] = AsymmetricInnerProductWithOffsetAndStandardDeviation (mean2, sigma0, OG2, SampleInfo) / sampleTwoNorm;
		WeAreDone = 0;

		for (i=0; i<maxIterations; i++) {

			mean0 = BrentStep (means, dotProducts);

			if (IsOutsideInterval (mean0, means [0], means [1], means [2]))
				break;

			dotProduct = AsymmetricInnerProductWithOffsetAndStandardDeviation (mean0, sigma0, OGNew, SampleInfo) / sampleTwoNorm;

			// which do we replace?

			MinimumValue = MinIndex (dotProducts, 3);

			//  MimimumValue will be replaced by the new value

			if (dotProduct >= dotProducts [MinimumValue]) {

				means [MinimumValue] = mean0;
				dotProducts [MinimumValue] = dotProduct;
				OGTemp = OGCoeffs [MinimumValue];
				OGTemp [0] = OGNew [0];
				OGTemp [1] = OGNew [1];
				OGTemp [2] = OGNew [2];

				if (dotProduct > fitTolerance) {

					// done!!
					break;
				}

				for (j=0; j<3; j++) {

					if (j != MinimumValue) {

						if (fabs (means [j] - mean0) < mean0 * 1.0e-8) {

							WeAreDone = 1;
							break;
						}
					}
				}

				if (WeAreDone)
					break;
			}

			else {

				// done!!!
				break;
			}
		}

		MaximumValue = MaxIndex (dotProducts, 3);

		fit = dotProducts [MaximumValue];
		sampleMean = means [MaximumValue];
		OGTemp = OGCoeffs [MaximumValue];
		double OGCoeff1 = *OGTemp;
		double OGCoeff2 = *(OGTemp + 1);
	}

	//CHANGE WHAT FOLLOWS!!  Done...but better check it out...
//	double newScale = fit * sampleTwoNorm / (sqrt (sqrtPi * sigma0));  This is old...for Gaussians

	//  In instantiation below, do we need to communicate left and right?????

	DataSignal* returnValue = new DoubleGaussian (sampleMean, sigma0, SigmaRatio); //  (left, right, sampleMean, sigma0);
	returnValue->SetScale (1, OGCoeff1);
	returnValue->SetScale (2, OGCoeff2);
	returnValue->SetCurveFit (fit);
	returnValue->SetCurrentDataInterval (Segment);

	// These two lines below are removed because they truncate the curve too abruptly.  We want to fit curves based on a fairly narrow 
	// interval because the center of the sample curve contains the most reliable information, but then we want to carry the entire 
	// fitted curve (or very nearly) so we want to use the entire DoubleGaussian

	returnValue->SetLeftEndPoint (left);
	returnValue->SetRightEndPoint (right);

	// Artificial spike fix 10/04/2012

	double segPeak = Segment->GetMaxAtMode ();
	bool shortSegment = (Segment->GetRight () - Segment->GetLeft () <= 5);
	bool peakMismatch = (fabs (returnValue->Peak () - segPeak) > 0.3 * segPeak);

	if (shortSegment || peakMismatch) {

		//if (fit > 0.0) {

		//left = sampleMean - 3.0 * sigma0;
		//right = sampleMean + 3.0 * sigma0;
		//Target->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment);

		//	//double OGCoeff1 = Segment->GetMaxAtMode ();
		//double lowDataFactor = fit * sqrt (sampleTwoNorm2) / returnValue->Norm (left, right);
		//	//double peak = returnValue->Peak ();
		//returnValue->SetPeak (lowDataFactor * returnValue->Peak ());
			returnValue->SetPeak (segPeak);
		//	cout << "Double Gaussian peak (asymmetric) with low data: Left = " << left << ". Right = " << right << ". Factor = " << segPeak << ".  Fit = " << fit << endl;
		//}
	}

	//double segPeak = Segment->GetMaxAtMode ();
	//bool shortSegment = (Segment->GetRight () - Segment->GetLeft () <= 5);
	//bool peakMismatch = (returnValue->Peak () > 1.3 * segPeak);

	//if (shortSegment || peakMismatch) {

	//	delete returnValue;
	//	DataSignal* NG = new NormalizedGaussian (0.0, ParametricCurve::GetSigmaForSignature ());
	//	DataSignal* ngReturnValue = NG->FindCharacteristic (Target, Segment, windowSize, fit, previous);
	//	cout << "Switched to Gaussian...left = " << left << ".  Right = " << right << endl;
	//	delete NG;
	//	return ngReturnValue;
	//}

	return returnValue;
}



DataSignal* DoubleGaussian :: GetResidual (const DataSignal* base, double coeff) {

	return NULL;
}


int DoubleGaussian :: Add (list<DataSignal*>& WaveList) {
	
	// list of pointers to DataSignal
	return -1;
}

/*
int DoubleGaussian :: SampleAndSave () {

	double spacing = SampledData::GetSampleSpacing ();

	int size = (int)floor (((Right - Left) / spacing) + 1.0);
	size++;

	if (2 * (size / 2) == size)
		size++;

	delete[] Sample;
	SampleSpacing = spacing;
	SampleSize = size;
	Sample = new double [size];
	double x;

	double* pplus = Sample;

	for (int i=0; i<SampleSize; i++) {

		x = Left + i * spacing;
		*pplus = Value (x);
		pplus++;
	}

	return SampleSize;
}*/


/*double DoubleGaussian :: InnerProductWithOffset (double offset, double* ogCoeffs, const SampleDataInfo& info) const {

	double coeffs [2];
	double InnerProduct1 = PrimaryCurve->InnerProductWithOffset (offset, coeffs, info);
	double InnerProduct2 = SecondaryCurve->InnerProductWithOffset (offset, coeffs, info);
	double norm1 = PrimaryCurve->Norm2 ();  // this is 2-norm of primary squared:  should be 1
	double norm2 = SecondaryCurve->Norm2 ();  // this is 2-norm of secondary squared:  should be 1
	*ogCoeffs = InnerProduct1;
	double* second = ogCoeffs + 1;
	double temp;
	*second = temp = (InnerProduct2 - (Correlation * InnerProduct1 / norm1)) / (sqrt (norm2 - ((Correlation * Correlation) / norm1)));
	return sqrt (InnerProduct1 * InnerProduct1 + temp * temp);
}*/

/*
double DoubleGaussian :: InnerProductWithStandardSpacing (double* ogCoeffs, const SampleDataInfo& info) const {

	double coeffs [2];
	double InnerProduct1 = PrimaryCurve->InnerProductWithStandardSpacing (coeffs, info);
	double InnerProduct2 = SecondaryCurve->InnerProductWithStandardSpacing (coeffs, info);
	double norm1 = PrimaryCurve->Norm2 ();  // this is 2-norm of primary squared:  should be 1
	double norm2 = SecondaryCurve->Norm2 ();  // this is 2-norm of secondary squared:  should be 1
	*ogCoeffs = InnerProduct1;
	double* second = ogCoeffs + 1;
	double temp;
	*second = temp = (InnerProduct2 - (Correlation * InnerProduct1 / norm1)) / (sqrt (norm2 - ((Correlation * Correlation) / norm1)));
	return sqrt (InnerProduct1 * InnerProduct1 + temp * temp);
}
*/


double DoubleGaussian :: InnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* ogCoeffs, const SampleDataInfo& info) const {

	double sigma2 = SigmaRatio * sigma;
	double SigmaCorrection = 1.0 / (SigmaRatio * SigmaRatio);

	const double* ptargetPlus = info.DataCenter;
	const double* ptargetPlusEnd = info.DataRight;
	const double* ptargetMinus = ptargetPlus;
	double sampleSpacing = info.Spacing;
	double center = info.AbscissaLeft + info.IndexCenter * sampleSpacing;
	double x = (center - offset) / sigma;
	double z;
	x *= 0.5 * x;
	z = x * SigmaCorrection;
	double tempx, tempy;
	tempx = exp (-x);
	double sum = tempx * (*ptargetPlus);
	double tempz, tempw;
	tempz = exp (-z);
	double sum2 = tempz * (*ptargetPlus);
	double sum3 = tempz * tempz;

	double sumv1 = tempx * tempx;
	double sumv1v2 = tempx * tempz;

//	int size = sampleSize - 1;
	int j = 0;
	double refRight;
	double refLeft;
	double y;
	double w;
	double temp;

	for (ptargetPlus=info.DataCenter+1; ptargetPlus<ptargetPlusEnd; ptargetPlus++) {

		j++;
		ptargetMinus--;
		temp = j * sampleSpacing;
		refRight = center + temp;
		refLeft = center - temp;
		x = (refRight - offset) / sigma;
		y = (refLeft - offset) / sigma;
		x *= 0.5 * x;
		y *= 0.5 * y;
		w = x * SigmaCorrection;
		z = y * SigmaCorrection;
		tempw = exp (-w);
		tempz = exp (-z);
		tempx = exp (-x);
		tempy = exp (-y);
		sum += (*ptargetPlus) * tempx + (*ptargetMinus) * tempy;
		sum2 += (*ptargetPlus) * tempw + (*ptargetMinus) * tempz;
		sum3 += tempz * tempz + tempw * tempw;
		sumv1 += tempx * tempx + tempy * tempy;
		sumv1v2 += tempx * tempw + tempy * tempz;
	}

	j++;
	ptargetMinus--;
	temp = j * sampleSpacing;
	refRight = center + temp;
	refLeft = center - temp;
	x = (refRight - offset) / sigma;
	y = (refLeft - offset) / sigma;
	x *= 0.5 * x;
	y *= 0.5 * y;
	w = x * SigmaCorrection;
	z = y * SigmaCorrection;
	tempw = exp (-w);
	tempz = exp (-z);
	tempx = exp (-x);
	tempy = exp (-y);
	sum += 0.5 * ((*ptargetPlus) * tempx + (*ptargetMinus) * tempy);
	sum2 += 0.5 * ((*ptargetPlus) * tempw + (*ptargetMinus) * tempz);
	sum3 += 0.5 * (tempz * tempz + tempw * tempw);
	sumv1 += 0.5 * (tempx * tempx + tempy * tempy);
	sumv1v2 += 0.5 * (tempx * tempw + tempy * tempz);

	sum *= sampleSpacing / (sqrt (sqrtPi * sigma));  // this is inner product of sample with first Gaussian
	sum2 *= sampleSpacing / (sqrt (sqrtPi * sigma2));  // this is inner product of (projected) sample with (projected) second Gaussian
	sum3 *= sampleSpacing / (sqrtPi * sigma2);  // this is norm^2 for second Gaussian
	sumv1 *= sampleSpacing / (sqrtPi * sigma);
	sumv1v2 *= sampleSpacing / (sqrtPi * sqrt (sigma * sigma2));

//	*ogCoeffs = sum;
	double* second = ogCoeffs + 1;
	double alpha1;
	double normv1;

	if (sumv1 * sum3 <= sumv1v2 * sumv1v2) {

		if (sumv1 <= 0.0)
			normv1 = alpha1 = *ogCoeffs = 0.0;
		
		else {

			normv1 = sqrt (sumv1);
			alpha1 = sum / normv1;
			*ogCoeffs = (alpha1 / normv1);
		}

		*second = 0.0;
		*(second + 1) = 1.0;

		return fabs (alpha1);
	}

	tempz = sum3 - ((sumv1v2 * sumv1v2) / sumv1);  // This should now be positive

/*	if (tempz <= 0.0) {

//		if (DataSignal::DebugFlag)
//		cout << "Normalization problem!!!  Sample Number = " << *info.DataCenter << ", sigma = " << sigma << ", v1 norm2 = " << sumv1 << endl;
		normv1 = sqrt (sumv1);
		alpha1 = sum / normv1;
		*ogCoeffs = (alpha1 / normv1);
		*second = 0.0;
		*(second + 1) = 1.0;

		return abs (alpha1);
	}*/

	double Radical = sqrt (tempz);
	normv1 = sqrt (sumv1);

//	*second = temp = (sum2 - Correlation * sum) / sqrt (tempz);
	double* third = second + 1;  // We'd better make sure that the calling program passes a pointer to 3 doubles
	*third = sum3;

	alpha1 = sum / normv1;
	double alpha2 = (sum2 / Radical) - ((sumv1v2 * sum) / (sumv1 * Radical));
	double pctAlpha1 = 0.05 * alpha1;

	if ((alpha2 < 0.0) && (fabs (alpha2) > pctAlpha1))
		alpha2 = pctAlpha1;

	// The coefficients below have changed from the initial version:  they now represent the coefficients of the actual
	// Gaussians, not the orthonormalized pair of functions

	*ogCoeffs = (alpha1 / normv1) - ((alpha2 * sumv1v2) / (sumv1 * Radical));  // This is now a1, the coeff. of primary Gaussian
	*second = alpha2 / Radical;   // This is now a2, the coeff. of secondary Gaussian

	return sqrt (alpha1 * alpha1 + alpha2 * alpha2);  // This is the inner product of the sum of the two Gaussians with the 
	       // sample.  It uses the two components of the perpendicular projections.
}


double DoubleGaussian :: AsymmetricInnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* ogCoeffs, const SampleDataInfo& info) const {

	double sigma2 = SigmaRatio * sigma;
	double SigmaCorrection = 1.0 / (SigmaRatio * SigmaRatio);

	const double* ptargetPlus = info.DataLeft;
	const double* ptargetPlusEnd = info.DataRight;
	double sampleSpacing = info.Spacing;
	double left = info.AbscissaLeft + info.Spacing * info.IndexLeft;
	double x = (left - offset) / sigma;
	x *= 0.5 * x;
	double w;
	w = x * SigmaCorrection;
	double tempx;
	tempx = exp (-x);
	double sum = 0.5 * tempx * (*ptargetPlus);
	double tempw;
	tempw = exp (-w);
	double sum2 = 0.5 * tempw * (*ptargetPlus);
	double sum3 = 0.5 * tempw * tempw;

	double sumv1 = 0.5 * tempx * tempx;
	double sumv1v2 = 0.5 * tempx * tempw;

	int j = 0;
	double temp;
	double ref;

	for (ptargetPlus=info.DataLeft+1; ptargetPlus<ptargetPlusEnd; ptargetPlus++) {

		j++;
		temp = j * sampleSpacing;
		ref = left + temp;
		x = (ref - offset) / sigma;
		x *= 0.5 * x;
		w = x * SigmaCorrection;
		tempw = exp (-w);
		tempx = exp (-x);
		sum += (*ptargetPlus) * tempx;
		sum2 += (*ptargetPlus) * tempw;
		sum3 += tempw * tempw;
		sumv1 += tempx * tempx;
		sumv1v2 += tempx * tempw;
	}

	j++;
	temp = j * sampleSpacing;
	ref = left + temp;
	x = (ref - offset) / sigma;
	x *= 0.5 * x;
	w = x * SigmaCorrection;
	tempw = exp (-w);
	tempx = exp (-x);
	sum += 0.5 * (*ptargetPlus) * tempx;
	sum2 += 0.5 * (*ptargetPlus) * tempw;
	sum3 += 0.5 * (tempw * tempw);
	sumv1 += 0.5 * (tempx * tempx);
	sumv1v2 += 0.5 * (tempx * tempw);

	sum *= sampleSpacing / (sqrt (sqrtPi * sigma));  // this is inner product of sample with first Gaussian
	sum2 *= sampleSpacing / (sqrt (sqrtPi * sigma2));  // this is inner product of (projected) sample with (projected) second Gaussian
	sum3 *= sampleSpacing / (sqrtPi * sigma2);  // this is norm^2 for second Gaussian
	sumv1 *= sampleSpacing / (sqrtPi * sigma);
	sumv1v2 *= sampleSpacing / (sqrtPi * sqrt (sigma * sigma2));

//	*ogCoeffs = sum;
	double* second = ogCoeffs + 1;
	double alpha1;
	double normv1;

	if (sumv1 * sum3 <= sumv1v2 * sumv1v2) {

		if (sumv1 <= 0.0)
			normv1 = alpha1 = *ogCoeffs = 0.0;
		
		else {

			normv1 = sqrt (sumv1);
			alpha1 = sum / normv1;
			*ogCoeffs = (alpha1 / normv1);
		}

		*second = 0.0;
		*(second + 1) = 1.0;

		return fabs (alpha1);
	}

	tempw = sum3 - ((sumv1v2 * sumv1v2) / sumv1);  // This should now be positive

/*	if (tempw <= 0.0) {

//		if (DataSignal::DebugFlag)
//		cout << "Normalization problem!!!  Sample Number = " << *info.DataCenter << ", sigma = " << sigma << ", v1 norm2 = " << sumv1 << endl;
		normv1 = sqrt (sumv1);
		alpha1 = sum / normv1;
		*ogCoeffs = (alpha1 / normv1);
		*second = 0.0;
		*(second + 1) = 1.0;

		return abs (alpha1);
	}*/

	double Radical = sqrt (tempw);
	normv1 = sqrt (sumv1);

//	*second = temp = (sum2 - Correlation * sum) / sqrt (tempz);
	double* third = second + 1;  // We'd better make sure that the calling program passes a pointer to 3 doubles
	*third = sum3;

	alpha1 = sum / normv1;
	double alpha2 = (sum2 / Radical) - ((sumv1v2 * sum) / (sumv1 * Radical));
	double pctAlpha1 = 0.05 * alpha1;

	if ((alpha2 < 0.0) && (fabs (alpha2) > pctAlpha1))
		alpha2 = pctAlpha1;

	// The coefficients below have changed from the initial version:  they now represent the coefficients of the actual
	// Gaussians, not the orthonormalized pair of functions

	*ogCoeffs = (alpha1 / normv1) - ((alpha2 * sumv1v2) / (sumv1 * Radical));  // This is now a1, the coeff. of primary Gaussian
	*second = alpha2 / Radical;   // This is now a2, the coeff. of secondary Gaussian

	return sqrt (alpha1 * alpha1 + alpha2 * alpha2);  // This is the inner product of the sum of the two Gaussians with the 
	       // sample.  It uses the two components of the perpendicular projections.
}


double DoubleGaussian :: Norm () {

	return sqrt (Norm2 ());
}


double DoubleGaussian :: Norm (double left, double right) {

	return sqrt (Norm2 (left, right));
}


double DoubleGaussian :: Norm2 () {
	
	// norm squared
	return OrthogonalPrimaryScale * OrthogonalPrimaryScale + OrthogonalSecondaryScale * OrthogonalSecondaryScale;
}


double DoubleGaussian :: Norm2 (double left, double right) {

	if ((left <= Left) && (right >= Right))
		return Norm2 ();

	double sum = 0.0;
	double width = (right - left) / Gaussian::GetNumberOfIntegrationSteps ();
	double temp;
	double x;
	int Total = Gaussian::GetNumberOfIntegrationSteps ();

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		temp = Value (x);
		temp *= temp;
		sum += temp;
	}

	x = left;
	temp = Value (x);
	temp *= 0.5 * temp;
	sum += temp;

	x = right;
	temp = Value (x);
	temp *= 0.5 * temp;
	sum += temp;

	return width * sum;
}


DataSignal* DoubleGaussian :: Normalize (double& norm) {

	norm = OrthogonalPrimaryScale * OrthogonalPrimaryScale + OrthogonalSecondaryScale * OrthogonalSecondaryScale;
	norm = sqrt (norm);

	DataSignal* ds = new DoubleGaussian (Mean, StandardDeviation, SigmaRatio);
	ds->SetOrthogonalScale (1, OrthogonalPrimaryScale / norm);
	ds->SetOrthogonalScale (2, OrthogonalSecondaryScale / norm);
	return ds;
}


DataSignal* DoubleGaussian :: Normalize (double left, double right, double& norm) {

	return NULL;
}


double DoubleGaussian :: Centroid () const {

	return Mean;
}


double DoubleGaussian :: Centroid (double left, double right) const {

	return Mean;
}


double DoubleGaussian :: SecondMoment (double left, double right) const {

	return StandardDeviation * StandardDeviation;
}


double DoubleGaussian :: OneNorm () {

	return OneNorm (Left, Right);
}


double DoubleGaussian :: OneNorm (double left, double right) {

	double sum = 0.0;
	double width = (right - left) / Gaussian::GetNumberOfIntegrationSteps ();
	double x;
	int Total = Gaussian::GetNumberOfIntegrationSteps ();

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		sum += Value (x);
	}

	sum += 0.5 * (Value (left) + Value (right));
	return width * sum * Scale;
}


int DoubleGaussian :: FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean) {

	if ((left <= Left) && (right >= Right)) {

		oneNorm = OneNorm ();
		mean = Mean;
		return 0;
	}

	double sum = 0.0;
	double sum1 = 0.0;
	double width = (right - left) / Gaussian::GetNumberOfIntegrationSteps ();
	double tempExp;
	double x;
	int Total = Gaussian::GetNumberOfIntegrationSteps ();

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		tempExp = Value (x);
		sum += tempExp;
		sum1 += x * tempExp;
	}

	tempExp = 0.5 * Value (left);
	sum += tempExp;
	sum1 += left * tempExp;

	tempExp = 0.5 * Value (right);
	sum += tempExp;
	sum1 += right * tempExp;

	oneNorm = width * sum * Scale;

	if (oneNorm == 0.0) {

		mean = DOUBLEMAX;
		return -1;
	}

	mean = sum1 / sum;
	return 0;
}



int DoubleGaussian :: SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment) {

	double sum = 0.0;
	double sum1 = 0.0;
	double sum2 = 0.0;
	double width = (right - left) / Gaussian::GetNumberOfIntegrationSteps ();
	double tempExp;
	double x;
	int Total = Gaussian::GetNumberOfIntegrationSteps ();

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		tempExp = Value (x);
		sum += tempExp;
		sum1 += x * tempExp;
		sum2 += x * x * tempExp;
	}

	tempExp = 0.5 * Value (left);
	sum += tempExp;
	sum1 += left * tempExp;
	sum2 += left * left * tempExp;

	tempExp = 0.5 * Value (right);
	sum += tempExp;
	sum1 += right * tempExp;
	sum2 += right * right * tempExp;

	oneNorm = width * sum * Scale;

	if (oneNorm == 0.0) {

		mean = DOUBLEMAX;
		secondMoment = DOUBLEMAX;
		return -1;
	}

	mean = sum1 / sum;
	secondMoment = (sum2 / sum) - mean * mean;
	return 0;
}


double DoubleGaussian :: TheoreticalArea () {

	return mArea;
}


void DoubleGaussian :: CalculateTheoreticalArea () {

	//double temp1 = SqrtSigmaRatio * SecondaryScale;
	//double temp = PrimaryScale + temp1;
	//double temp2 = 3.0 * Peak () * PrimaryCurve->GetStandardDeviation ();
	//double temp3 = temp * sqrtTwo * sqrt (PrimaryCurve->GetStandardDeviation ()) * fourthRootOfPi;

	//if (temp3 > temp2)
	//	mArea = temp3;

	//else
	//	mArea = temp2;

	double first = Peak ();
	double Cumulative = first;
	double last;
	double mean = GetMean ();
	double sigma = PrimaryCurve->GetStandardDeviation ();
	double abscissa = mean;
//	double temp2;
	double temp1;
	double prevLast = first;
	double cutOff = 0.01 * first;
	double step = 0.5 * sigma;

	for (int i=1; i<=20; i++) {

		abscissa += step;
		last = Value (abscissa);

		if ((last <= cutOff) && (i > 1))
			break;

		Cumulative += last;
		prevLast = last;
	}

	temp1 = step * (2.0 * Cumulative - first - prevLast);
//	temp2 = 3.0 * peak * sigma;

	//if (temp2 > temp1)
	//	mArea = temp2;

	//else
		mArea = temp1;
}


void DoubleGaussian :: Report (RGTextOutput& text, const RGString& indent) {

	Endl endLine;
	RGString Indent = indent + "    ";
	text << indent << "Double Gaussian fit with parameters:" << endLine;
	PrimaryCurve->Report (text, Indent);
	text << endLine;
	SecondaryCurve->Report (text, Indent);
	text << endLine;

	double temp = OrthogonalPrimaryScale * OrthogonalPrimaryScale + OrthogonalSecondaryScale * OrthogonalSecondaryScale;
	temp = sqrt (temp);
	double Mass = OneNorm ();

	text << indent << "    Curve Coefficients:" << endLine;
	text << indent << "        Primary Coefficient:  " << PrimaryScale << endLine;
	text << indent << "        Secondary Coefficient:  " << SecondaryScale << endLine;
	text << indent << "        Orthogonal Coefficients:  "  << OrthogonalPrimaryScale << " and " << OrthogonalSecondaryScale << endLine;
	text << indent << "        Orthogonal Component Ratios:  " << OrthogonalPrimaryScale / temp << " and " << OrthogonalSecondaryScale / temp << endLine;
	text << indent << "        Peak:  " << PrimaryScale / sqrt (sqrtPi * StandardDeviation) + SecondaryScale / sqrt (sqrtPi * StandardDeviation * SigmaRatio) << endLine;
	text << indent << "        Mass:  " << Mass << endLine;
	text << indent << "        Fit:   " << Fit << endLine;
}


size_t DoubleGaussian :: StoreSize () const {

	size_t size = ParametricCurve::StoreSize ();
	return size;
}


void DoubleGaussian :: RestoreAll (RGFile& f) {

	ParametricCurve::RestoreAll (f);
}


void DoubleGaussian :: RestoreAll (RGVInStream& f) {

	ParametricCurve::RestoreAll (f);
}


void DoubleGaussian :: SaveAll (RGFile& f) const {

	ParametricCurve::SaveAll (f);
}


void DoubleGaussian :: SaveAll (RGVOutStream& f) const {

	ParametricCurve::SaveAll (f);
}



SuperGaussian :: SuperGaussian () : ParametricCurve (-SuperGaussian::SigmaWidth, SuperGaussian::SigmaWidth), Mean (0.0), StandardDeviation (1.0), Degree (3) {

	SuperNorm = SuperGaussian::SuperNorms [0];
	RootTwo = SuperGaussian::RootTwos [0];
	TwoNorm2WithoutScale = StandardDeviation * SuperNorm / RootTwo;
	SuperSigma2 = SuperGaussian::SuperSigma2s [0];
	Sigma2 = (StandardDeviation / RootTwo);
	Sigma2 *= Sigma2;
	Sigma2 *= SuperSigma2 / SuperNorm;
	Sigma1 = sqrt (Sigma2);
	DataSignal::AddNoticeToList (OutputLevelManager::CurveIsABlob, "", "Curve is a level 3 blob");
}


SuperGaussian :: SuperGaussian (double mean, double standardDeviation, int degree) : 
ParametricCurve (mean - SuperGaussian::SigmaWidth * standardDeviation, mean + SuperGaussian::SigmaWidth * standardDeviation), Mean (mean), 
StandardDeviation (standardDeviation), Degree (degree) {

	if (Degree > 10)
		Degree = 10;

	if (Degree < 3)
		Degree = 3;

	SuperNorm = SuperGaussian::SuperNorms [Degree - 3];
	RootTwo = SuperGaussian::RootTwos [Degree - 3];
	TwoNorm2WithoutScale = StandardDeviation * SuperNorm / RootTwo;
	SuperSigma2 = SuperGaussian::SuperSigma2s [Degree - 3];
	Sigma2 = (StandardDeviation / RootTwo);
	Sigma2 *= Sigma2;
	Sigma2 *= SuperSigma2 / SuperNorm;
	Sigma1 = sqrt (Sigma2);
	RGString temp;
	temp.Convert (degree, 10);
	RGString notice ("Curve is a level ");
	notice += temp + " blob";
	DataSignal::AddNoticeToList (OutputLevelManager::CurveIsABlob, "", notice);
}



SuperGaussian :: SuperGaussian (double left, double right, double mean, double standardDeviation, int degree) :
ParametricCurve (left, right), Mean (mean), StandardDeviation (standardDeviation), Degree (degree) {

	if (Degree > 10)
		Degree = 10;

	if (Degree < 3)
		Degree = 3;

	SuperNorm = SuperGaussian::SuperNorms [Degree - 3];
	RootTwo = SuperGaussian::RootTwos [Degree - 3];
	TwoNorm2WithoutScale = StandardDeviation * SuperNorm / RootTwo;
	SuperSigma2 = SuperGaussian::SuperSigma2s [Degree - 3];
	Sigma2 = (StandardDeviation / RootTwo);
	Sigma2 *= Sigma2;
	Sigma2 *= SuperSigma2 / SuperNorm;
	Sigma1 = sqrt (Sigma2);
	RGString temp;
	temp.Convert (degree, 10);
	RGString notice ("Curve is a level ");
	notice += temp + " blob";
	DataSignal::AddNoticeToList (OutputLevelManager::CurveIsABlob, "", notice);
}


SuperGaussian :: SuperGaussian (const SuperGaussian& sg) : ParametricCurve (sg), Mean (sg.Mean), 
StandardDeviation (sg.StandardDeviation), Degree (sg.Degree), SuperNorm (sg.SuperNorm),
RootTwo (sg.RootTwo), TwoNorm2WithoutScale (sg.TwoNorm2WithoutScale), Sigma2 (sg.Sigma2), Sigma1 (sg.Sigma1),
SuperSigma2 (sg.SuperSigma2) {

}


SuperGaussian :: SuperGaussian (double mean, const SuperGaussian& sg) : 
ParametricCurve (mean - SuperGaussian::SigmaWidth * sg.StandardDeviation, mean + SuperGaussian::SigmaWidth * sg.StandardDeviation), Mean (mean),
StandardDeviation (sg.StandardDeviation), Degree (sg.Degree), SuperNorm (sg.SuperNorm),
RootTwo (sg.RootTwo), TwoNorm2WithoutScale (sg.TwoNorm2WithoutScale), Sigma2 (sg.Sigma2), Sigma1 (sg.Sigma1),
SuperSigma2 (sg.SuperSigma2) {

}



SuperGaussian :: ~SuperGaussian () {

}


DataSignal* SuperGaussian :: MakeCopy (double mean) const {

	DataSignal* newSignal = new SuperGaussian (mean, *this);
	return newSignal;
}


RGString SuperGaussian :: GetSignalType () const {

	return RGString ("Super Gaussian");
}


void SuperGaussian :: SetDisplacement (double disp) {

	Mean += disp;
}


void SuperGaussian :: SetScale (double scale) {

	Scale = scale;
}


double SuperGaussian :: Value (double x) const {

	double temp = fabs ((x - Mean) / StandardDeviation);
	double exponent = 1.0;

	for (int i=0; i<Degree; i++)
		exponent *= temp;

	return Scale * exp (-exponent);
}


double SuperGaussian :: UnscaledValue (double x) const {

	double temp = fabs ((x - Mean) / StandardDeviation);
	double exponent = 1.0;

	for (int i=0; i<Degree; i++)
		exponent *= temp;

	return exp (-exponent);
}


double SuperGaussian :: Peak () const {

	return Scale;
}


double SuperGaussian :: GetMean () const {

	return Mean;
}


double SuperGaussian :: GetStandardDeviation () const {

	return Sigma1;
}


double SuperGaussian :: GetVariance () const {

	return Sigma2;
}


void SuperGaussian :: SetPeak (double peak) {

	Scale = peak;
}


void SuperGaussian :: ComputeTails (double& tailLeft, double& tailRight) const {

	double F;
	double spacing = DataSignal::GetSampleSpacing ();
	
	if (Scale <= 0.0)
		F = 0.0;

	else
		F = StandardDeviation * exp (log (log (Scale)) / (double)Degree) + spacing;

	tailLeft = Mean - F;
	tailRight = Mean + F;
}


DataSignal* SuperGaussian :: Project (double left, double right) const {

	double l = left;

	if (Left > l)
		l = Left;

	double r = right;

	if (Right < r)
		r = Right;

	if (r <= l)
		return NULL;

	if ((r <= Mean) || (Mean <= l))
		return NULL;

	DataSignal* ds = new SuperGaussian (l, r, Mean, StandardDeviation, Degree);
	return ds;
}


DataSignal* SuperGaussian :: Project (const DataSignal* target) const {

	double left = target->LeftEndPoint ();
	double right = target->RightEndPoint ();
	return Project (left, right);
}


double SuperGaussian :: Centroid () const {

	return Mean;
}


double SuperGaussian :: Centroid (double left, double right) const {

	return Mean;
}


double SuperGaussian :: SecondMoment (double left, double right) const {

	//  This should be OK now

	return Sigma2;
}


double SuperGaussian :: Norm () {

	return sqrt (Norm2 ());
}


double SuperGaussian :: Norm (double left, double right) {

	return sqrt (Norm2 (left, right));
}


double SuperGaussian :: Norm2 () {
	
	// norm squared
	return Scale * Scale * TwoNorm2WithoutScale;   // This should be OK now!!!
}


double SuperGaussian :: Norm2 (double left, double right) {

	if ((left <= Left) && (right >= Right))
		return Norm2 ();

	double sum = 0.0;
	double width = (right - left) / SuperGaussian::NumberOfSteps;
	double temp;
	double x;
	int Total = SuperGaussian::NumberOfSteps;

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		temp = UnscaledValue (x);
		sum += temp * temp;
	}

	temp = UnscaledValue (left);
	sum += 0.5 * temp * temp;
	temp = UnscaledValue (right);
	sum += 0.5 * temp * temp;

	return width * sum * Scale * Scale;
}


DataSignal* SuperGaussian :: Normalize (double& norm) {

	norm = Norm ();
	double NewScale = Scale / norm;
	DataSignal* ds = new SuperGaussian (Left, Right, Mean, StandardDeviation, Degree);
	ds->SetDisplacement (Displacement);
	ds->SetScale (NewScale);
	return ds;
}


DataSignal* SuperGaussian :: Normalize (double left, double right, double& norm) {

	norm = Norm (left, right);
	double NewScale = Scale / norm;
	DataSignal* ds = new SuperGaussian (left, right, Mean, StandardDeviation, Degree);

	ds->SetDisplacement (Displacement);
	ds->SetScale (NewScale);
	return ds;
}


void SuperGaussian :: CalculateTheoreticalArea () {

	double first = Peak ();
	double Cumulative = first;
	double last;
	double mean = GetMean ();
	double sigma = GetStandardDeviation ();
	double abscissa = mean;
	double temp1;
	double prevLast = first;
	double cutOff = 0.01 * first;
	double step = 0.5 * sigma;

	for (int i=1; i<=20; i++) {

		abscissa += step;
		last = Value (abscissa);

		if ((last <= cutOff) && (i > 1))
			break;

		Cumulative += last;
		prevLast = last;
	}

	temp1 = step * (2.0 * Cumulative - first - prevLast);
	mArea = temp1;
}


double SuperGaussian :: OneNorm () {

	return Scale * StandardDeviation * SuperNorm;  // This should be OK now
}


double SuperGaussian :: OneNorm (double left, double right) {

	if ((left <= Left) && (right >= Right))
		return OneNorm ();

	double sum = 0.0;
	double width = (right - left) / SuperGaussian::NumberOfSteps;
	double x;
	int Total = SuperGaussian::NumberOfSteps;

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		sum += UnscaledValue (x);
	}

	sum += 0.5 * UnscaledValue (left);
	sum += 0.5 * UnscaledValue (right);

	return width * sum * Scale;
}


int SuperGaussian :: FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean) {

	if ((left <= Left) && (right >= Right)) {

		oneNorm = OneNorm ();
		mean = Mean;
		return 0;
	}

	double sum = 0.0;
	double sum1 = 0.0;
	double width = (right - left) / SuperGaussian::NumberOfSteps;
	double tempExp;
	double x;
	int Total = SuperGaussian::NumberOfSteps;

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		tempExp = UnscaledValue (x);
		sum += tempExp;
		sum1 += x * tempExp;
	}

	tempExp = 0.5 * UnscaledValue (left);
	sum += tempExp;
	sum1 += left * tempExp;

	tempExp = 0.5 * UnscaledValue (right);
	sum += tempExp;
	sum1 += right * tempExp;

	oneNorm = width * sum * Scale;

	if (oneNorm == 0.0) {

		mean = DOUBLEMAX;
		return -1;
	}

	mean = sum1 / sum;
	return 0;
}



int SuperGaussian :: SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment) {

	if ((left <= Left) && (right >= Right)) {

		oneNorm = OneNorm ();
		mean = Mean;
		secondMoment = Sigma2;   // This should be OK now
		return 0;
	}

	double sum = 0.0;
	double sum1 = 0.0;
	double sum2 = 0.0;
	double width = (right - left) / SuperGaussian::NumberOfSteps;
	double temp;
	double tempExp;
	double x;
	int Total = SuperGaussian::NumberOfSteps;

	for (int i=1; i<Total; i++) {

		x = left + width * i;
		temp = (x - Mean) / StandardDeviation;
		temp *= 0.5 * temp;
		tempExp = exp (-temp);
		sum += tempExp;
		sum1 += x * tempExp;
		sum2 += x * x * tempExp;
	}

	temp = (left - Mean) / StandardDeviation;
	temp *= 0.5 * temp;
	tempExp = 0.5 * exp (-temp);
	sum += tempExp;
	sum1 += left * tempExp;
	sum2 += left * left * tempExp;

	temp = (right - Mean) / StandardDeviation;
	temp *= 0.5 * temp;
	tempExp = 0.5 * exp (-temp);
	sum += tempExp;
	sum1 += right * tempExp;
	sum2 += right * right * tempExp;

	oneNorm = width * sum * Scale;

	if (oneNorm == 0.0) {

		mean = DOUBLEMAX;
		secondMoment = DOUBLEMAX;
		return -1;
	}

	mean = sum1 / sum;
	secondMoment = (sum2 / sum) - mean * mean;
	return 0;
}


void SuperGaussian :: Report (RGTextOutput& text, const RGString& indent) {

	Endl endLine;
	text << indent << "SuperGaussian with parameters:\n" << endLine;
	text << indent << "   Mean  = " << Mean << endLine;
	text << indent << "   Sigma = " << StandardDeviation << endLine;
	text << indent << "   Scale = " << Scale << endLine;
	text << indent << "   Degree = " << Degree << endLine;
	text << indent << "   Standard Deviation = " << sqrt (Sigma2) << endLine;
	double mass = OneNorm ();
	text << indent << "   Mass  = " << mass << endLine;
	text << indent << "   Fit   = " << Fit << endLine;
}


size_t SuperGaussian :: StoreSize () const {

	size_t size = ParametricCurve::StoreSize ();
	size += 2 * sizeof (double) + sizeof (int);
	return size;
}

void SuperGaussian :: RestoreAll (RGFile& f) {

	ParametricCurve::RestoreAll (f);
	f.Read (Mean);
	f.Read (StandardDeviation);
	f.Read (Degree);

	if (Degree > 10)
		Degree = 10;

	if (Degree < 3)
		Degree = 3;

	SuperNorm = SuperGaussian::SuperNorms [Degree - 3];
	RootTwo = SuperGaussian::RootTwos [Degree - 3];
	TwoNorm2WithoutScale = StandardDeviation * SuperNorm / RootTwo;
	Sigma2 = (StandardDeviation / RootTwo);
	Sigma2 *= Sigma2;
	Sigma2 *= SuperGaussian::SuperSigma2s [Degree - 3] / SuperNorm;
	Sigma1 = sqrt (Sigma2);
}


void SuperGaussian :: RestoreAll (RGVInStream& f) {

	ParametricCurve::RestoreAll (f);
	f >> Mean;
	f >> StandardDeviation;
	f >> Degree;

	if (Degree > 10)
		Degree = 10;

	if (Degree < 3)
		Degree = 3;

	SuperNorm = SuperGaussian::SuperNorms [Degree - 3];
	RootTwo = SuperGaussian::RootTwos [Degree - 3];
	TwoNorm2WithoutScale = StandardDeviation * SuperNorm / RootTwo;
	Sigma2 = (StandardDeviation / RootTwo);
	Sigma2 *= Sigma2;
	Sigma2 *= SuperGaussian::SuperSigma2s [Degree - 3] / SuperNorm;
	Sigma1 = sqrt (Sigma2);
}


void SuperGaussian :: SaveAll (RGFile& f) const {

	ParametricCurve::SaveAll (f);
	f.Write (Mean);
	f.Write (StandardDeviation);
	f.Write (Degree);
}


void SuperGaussian :: SaveAll (RGVOutStream& f) const {

	ParametricCurve::SaveAll (f);
	f << Mean;
	f << StandardDeviation;
	f << Degree;
}



NormalizedSuperGaussian :: NormalizedSuperGaussian () : SuperGaussian () {

	SampleSpacing = SampledData::GetSampleSpacing ();
	Scale = 1.0 / (SampleSpacing * sqrt (TwoNorm2WithoutScale));
}


NormalizedSuperGaussian :: NormalizedSuperGaussian (double mean, double standardDeviation, int degree) : SuperGaussian (mean, standardDeviation, degree) {

	SampleSpacing = SampledData::GetSampleSpacing ();
	Scale = 1.0 / (SampleSpacing * sqrt (TwoNorm2WithoutScale));
}


NormalizedSuperGaussian :: NormalizedSuperGaussian (const NormalizedSuperGaussian& nsg) : SuperGaussian (nsg), SampleSpacing (nsg.SampleSpacing) {

}


NormalizedSuperGaussian :: NormalizedSuperGaussian (double mean, const NormalizedSuperGaussian& nsg) : SuperGaussian (mean, nsg), 
SampleSpacing (nsg.SampleSpacing) {

}


NormalizedSuperGaussian :: ~NormalizedSuperGaussian () {

}


DataSignal* NormalizedSuperGaussian :: MakeCopy (double mean) const {

	DataSignal* newSignal = new NormalizedSuperGaussian (mean, *this);
	return newSignal;
}


RGString NormalizedSuperGaussian :: GetSignalType () const {

	return RGString ("Normalized Super Gaussian");
}


void NormalizedSuperGaussian :: SetScale (double scale) {

}


DataSignal* NormalizedSuperGaussian :: FindNextCharacteristicFromRight (const DataSignal& Signature, double& fit, RGDList& previous) {

	return NULL;
}


DataSignal* NormalizedSuperGaussian :: FindNextCharacteristicFromLeft (const DataSignal& Signature, double& fit, RGDList& previous) {

	return NULL;
}


DataSignal* NormalizedSuperGaussian :: FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, 
int windowSize, double& fit, RGDList& previous) const {

	// Note:  have to fix scaling and relationship between StandardDeviation (sample) and 'StandardDeviation', the
	// parameter in the SuperGaussian formula.  We may also have to derive a new relationship for the dot product
	// of two SuperGaussians of different sigmas and the same mean.  We should add the part at the end where we 
	// optionally iterate to find the best mean.

	//
	//  OK, here's the skinny.  First, assume that the standard deviation, sigma (sigma0) of this SuperGaussian,
	// is less than that for any of the curves to fit.  Given that, then here's what to do:
	// 1)  Find the interval of definition of the Target from Segment, refine it and make it symmetric about 
	//     the center, using windowSize; 
	// 2)  Calculate the sample mean and sample standard deviation (sigma1) on the refined interval; 
	// 3)  Calculate the Dot Product with the native NormalizedSuperGaussian using the offset of the sample mean; 
	// 4)  Calculate the predicted sigma (sigma2) based on the formula and the dot product; 
	// 5)  Using starting sigma (sigma0) for 3rd sigma, perform inverse parabolic interpolation to refine 
	//     sigma (1 - 5 iterations, max?); 
	// 6)  Create new SuperGaussian with appropriate parameters and return...fit = Dot Product.
	//

	// 1:	

	DataSignal* possibleSpike = Segment->TestForSpike (fit);

	if (possibleSpike != NULL)
		return possibleSpike;

	int SegmentLeft = Segment->GetLeft ();
	int SegmentRight = Segment->GetRight ();
	int SegmentCenter = Segment->GetCenter ();
	int SegmentWindow = SegmentRight - SegmentLeft;
	SegmentWindow -= windowSize;
	double* coeffs = NULL;

	int MaxHalfWindow = SegmentCenter - SegmentLeft;   // Added to make sure we don't exceed Segment boundary (by much)
	int tempSeg = SegmentRight - SegmentCenter;   //  ditto
	
	if (tempSeg < MaxHalfWindow)   //  ditto
		MaxHalfWindow = tempSeg;   //  ditto

	int MaxWindow = 2 * MaxHalfWindow;   //  ditto

	SegmentWindow = 2 * ((SegmentWindow + 1) / 2);  // Gets SegmentWindow, if even, and SegmentWindow+1 if not

	if (SegmentWindow > MaxWindow)   // Added to make sure we don't exceed Segment boundary (by much)
		SegmentWindow = MaxWindow;   // Added to make sure we don't exceed Segment boundary (by much)

	// Get window for target Gaussian and compare; take whichever is larger
	double IntervalSpacing = DataSignal::GetSampleSpacing ();
	double HalfTargetWindow = SuperGaussian::GetSigmaWidth() * StandardDeviation;
	int HalfIntervalWindow = (int)floor((HalfTargetWindow / IntervalSpacing) + 0.9);
	int IntervalWindow = 2 * HalfIntervalWindow;

	if (IntervalWindow > SegmentWindow)
		SegmentWindow = IntervalWindow;

	int HalfSegmentWindow = SegmentWindow / 2;
	SegmentLeft = SegmentCenter - HalfSegmentWindow;
	SegmentRight = SegmentCenter + HalfSegmentWindow;

	int OriginalHalfSegmentWindow = HalfSegmentWindow;  // This is as large as we ever want this.  Don't make it bigger!

	// 2:
	double left = Target->LeftEndPoint () + SegmentLeft * IntervalSpacing;
	double right = Target->LeftEndPoint () + SegmentRight * IntervalSpacing;
	double center = Target->LeftEndPoint () + SegmentCenter * IntervalSpacing;

	int OldSegmentLeft = SegmentLeft;

	const DataSignal* Overlapped = BuildOverlappedSignal (left, right, previous, Target);

	double sampleTwoNorm2;
	double sampleMean;
	double sampleTwoMoment;
	
	if (Overlapped->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment) != 0) {
		
		if (Overlapped != Target)
			delete Overlapped;

		return NULL;
	}

	double sigma1 = RootTwo * sqrt (sampleTwoMoment * SuperNorm / SuperSigma2);  // assume sampleTwoMoment is variance:  mean^2 already subtracted
	double sampleTwoNorm = sqrt (sampleTwoNorm2);

	//
	// The following is new and the purpose is to adjust the window to be more appropriate to the expected curve:  not too large
	//

	double OldLeft = left;
	double OldRight = right;

	HalfTargetWindow = SuperGaussian::GetSigmaWidth() * sigma1;
	HalfSegmentWindow = (int)floor((HalfTargetWindow / IntervalSpacing) + 0.9999);

	if (HalfSegmentWindow > OriginalHalfSegmentWindow)
		HalfSegmentWindow = OriginalHalfSegmentWindow;  // we want this process to DECREASE segment window, not increase it!

	SegmentCenter = (int) floor ((sampleMean / IntervalSpacing) + 0.5);
	IntervalWindow = 2 * HalfIntervalWindow;
	SegmentLeft = SegmentCenter - HalfSegmentWindow;  // the window should be better centered now
	SegmentRight = SegmentCenter + HalfSegmentWindow;
	SegmentWindow = SegmentRight - SegmentLeft;
	SegmentWindow = 2 * ((SegmentWindow + 1) / 2);  // Gets SegmentWindow, if even, and SegmentWindow+1 if not
	left = Target->LeftEndPoint () + SegmentLeft * IntervalSpacing;
	right = Target->LeftEndPoint () + SegmentRight * IntervalSpacing;

	if ((left < OldLeft) || (right > OldRight)) {

		//  If we're here, the interval has expanded, which should not happen

		if (Overlapped != Target)
			delete Overlapped;

		OldSegmentLeft = SegmentLeft;
		Overlapped = BuildOverlappedSignal (left, right, previous, Target);
	}

	// This calculates mean, variance and norm for new interval

	if (Overlapped->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment) != 0) {
		
		if (Overlapped != Target)
			delete Overlapped;

		return NULL;
	}

	sigma1 = RootTwo * sqrt (sampleTwoMoment * SuperNorm / SuperSigma2);  // assume sampleTwoMoment is variance:  mean^2 already subtracted
	sampleTwoNorm = sqrt (sampleTwoNorm2);

	// 3:
	int i;
	int NSamples = SegmentWindow + 1;

	if (Overlapped != Target) {

		SegmentRight -= OldSegmentLeft;
		SegmentCenter -= OldSegmentLeft;
		SegmentLeft -= OldSegmentLeft;
	}

	SampleDataInfo SampleInfo (*Overlapped, SegmentLeft, SegmentCenter, SegmentRight, NSamples);

	//  The following should be normalized and between 0 and 1

	double dotProducts [3];
	double sigma0 = StandardDeviation;  // sigma0, sigma1 and sigma2 should bracket true sigma
	double dotProduct = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

	if (dotProduct > 1.0)
		dotProduct = 1.0;  // Just for safety...those numbers!  They sure can play tricks!

	dotProducts [0] = dotProduct;

	// 4:
	double temp2 = dotProduct;  // Take this to the Degree'th power

	for (i=1; i<Degree; i++)
		temp2 *= dotProduct;

	double temp = temp2 * temp2;  // This is dotProduct^(2 * Degree)
	double lambda;
	double beta;
	temp = 1.0 - temp;  // This is 1 - dotProduct^(2 * Degree)
	
	if (temp <= 0.0)
		temp = 0.0;  // There you go again...those pesky numbers!

	else
		temp = sqrt (temp);

	beta = (1.0 + temp) / temp2;
	beta *= beta;
	lambda = nthRoot (beta);
	double sigma2;

	if (sigma1 > sigma0)
		sigma2 = lambda * sigma0;  // This is what we expect, since we started small

	else
		sigma2 = sigma0 / lambda;

//	sigma0 = 0.5 * (sigma1 + sigma2);
//	dotProduct = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

	double mean0 = sampleMean;
	double mean1 = center;
//	double mean2;
	double sigmas [3];
	
//	double means [3];

	int maxIterations = NormalizedGaussian::GetMaximumIterations ();
	int j;
	int MinimumValue;
	int WeAreDone = 0;

	double fitTolerance = ParametricCurve::GetFitTolerance ();
	double sigmaTolerance = NormalizedGaussian::GetSigmaTolerance ();
/*
	if (center < sampleMean)
		mean2 = center + IntervalSpacing;

	else
		mean2 = center - IntervalSpacing;

	means [1] = mean1;
	means [2] = mean2;
	means [0] = mean0;
	dotProducts [0] = dotProduct;
	dotProducts [1] = InnerProductWithOffsetAndStandardDeviation (mean1, sigma0, coeffs, SampleInfo) / sampleTwoNorm;
	dotProducts [2] = InnerProductWithOffsetAndStandardDeviation (mean2, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

	for (i=0; i<maxIterations; i++) {

		mean0 = BrentStep (means, dotProducts);
		dotProduct = InnerProductWithOffsetAndStandardDeviation (mean0, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

		// which do we replace?

		MinimumValue = MinIndex (dotProducts, 3);

		//  MimimumValue will be replaced by the new value

		if (dotProduct >= dotProducts [MinimumValue]) {

			means [MinimumValue] = mean0;
			dotProducts [MinimumValue] = dotProduct;

			if (dotProduct > fitTolerance) {

				// done!!
				break;
			}

			for (j=0; j<3; j++) {

				if (j != MinimumValue) {

					if (abs (means [j] - mean0) < mean0 * 1.0e-8) {

						WeAreDone = 1;
						break;
					}
				}
			}

			if (WeAreDone)
				break;
		}

		else {

			// done!!!
			break;
		}
	}*/

	int MaximumValue;  // = MaxIndex (dotProducts, 3);
//	sampleMean = means [MaximumValue];

	// 5:

	// Need to get the dot products and put everything into an array...then iterate (5 times max).
	// The one for sigma0 is in dotProduct, but must compute the other two.

	sigmas [0] = sigma0;
//	dotProducts [0] = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;  // already done

	sigmas [1] = sigma1;
	sigmas [2] = sigma2;

	dotProducts [1] = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma1, coeffs, SampleInfo) / sampleTwoNorm;
	dotProducts [2] = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma2, coeffs, SampleInfo) / sampleTwoNorm;
	WeAreDone = 0;

	for (i=0; i<maxIterations; i++) {

		sigma0 = BrentStep (sigmas, dotProducts);
		dotProduct = InnerProductWithOffsetAndStandardDeviation (sampleMean, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

		// which do we replace?

		MinimumValue = MinIndex (dotProducts, 3);

		//  MimimumValue will be replaced by the new value

		if (dotProduct >= dotProducts [MinimumValue]) {

			sigmas [MinimumValue] = sigma0;
			dotProducts [MinimumValue] = dotProduct;

			if (dotProduct > fitTolerance) {

				// done!!
				break;
			}

			for (j=0; j<3; j++) {

				if (j != MinimumValue) {

					if (fabs (sigmas [j] - sigma0) < sigma0 * sigmaTolerance) {

						WeAreDone = 1;
						break;
					}
				}
			}

			if (WeAreDone)
				break;
		}

		else {

			// done!!!
			break;
		}
	}

	MaximumValue = MaxIndex (dotProducts, 3);

	fit = dotProducts [MaximumValue];
	sigma0 = sigmas [MaximumValue];

	// The following was entered in order to improve fit by iterating on the mean.  The effect was
	//  too small and potentially unpredictable.  To make any true improvement at this point, it
	//  would probably be necessary to iterate on both sigma and mean simultaneously.

//	double mean2;

	// We don't need the following for blobs - we don't have to locate them so accurately!

/*	if (fit < 0.999) {

		mean2 = sampleMean + 2.0 * IntervalSpacing;

		means [1] = mean1;
		means [2] = mean2;
		means [0] = mean0;
		dotProducts [0] = fit;
		dotProducts [1] = InnerProductWithOffsetAndStandardDeviation (mean1, sigma0, coeffs, SampleInfo) / sampleTwoNorm;
		dotProducts [2] = InnerProductWithOffsetAndStandardDeviation (mean2, sigma0, coeffs, SampleInfo) / sampleTwoNorm;
		WeAreDone = 0;

		for (i=0; i<maxIterations; i++) {

			mean0 = BrentStep (means, dotProducts);
			dotProduct = InnerProductWithOffsetAndStandardDeviation (mean0, sigma0, coeffs, SampleInfo) / sampleTwoNorm;

			// which do we replace?

			MinimumValue = MinIndex (dotProducts, 3);

			//  MimimumValue will be replaced by the new value

			if (dotProduct >= dotProducts [MinimumValue]) {

				means [MinimumValue] = mean0;
				dotProducts [MinimumValue] = dotProduct;

				if (dotProduct > fitTolerance) {

					// done!!
					break;
				}

				for (j=0; j<3; j++) {

					if (j != MinimumValue) {

						if (abs (means [j] - mean0) < mean0 * 1.0e-8) {

							WeAreDone = 1;
							break;
						}
					}
				}

				if (WeAreDone)
					break;
			}

			else {

				// done!!!
				break;
			}
		}

		MaximumValue = MaxIndex (dotProducts, 3);

		fit = dotProducts [MaximumValue];
		sampleMean = means [MaximumValue];
	}  */

	double Normalization2Norm = IntervalSpacing * sqrt (sigma0 * SuperNorm / RootTwo);
	double newScale = fit * sampleTwoNorm / Normalization2Norm;   //  This should be OK now.

	DataSignal* returnValue = new SuperGaussian (left, right, sampleMean, sigma0, Degree);
	returnValue->SetScale (newScale);
	returnValue->SetLeftEndPoint (left);
	returnValue->SetRightEndPoint (right);
	returnValue->SetCurveFit (fit);
	returnValue->SetCurrentDataInterval (Segment);

	if (Overlapped != Target)
		delete Overlapped;

	if ((sampleMean < (double)Segment->GetLeft ()) || (sampleMean > (double)Segment->GetRight ())) {

		returnValue->SetMayBeUnacceptable (true);
		//fit = 0.0;
		//returnValue->SetCurveFit (0.0);
	}

	//if (fabs (sampleMean - (double)Segment->GetMode ()) > 5.0) {

	//	fit = 0.0;
	//	returnValue->SetCurveFit (0.0);
	//}

	// Artificial spike fix 10/04/2012

	double segPeak = Segment->GetMaxAtMode ();
	bool shortSegment = (Segment->GetRight () - Segment->GetLeft () <= 5);
	bool peakMismatch = (fabs (returnValue->Peak () - segPeak) > 0.3 * segPeak);

	if (shortSegment || peakMismatch) {

		//left = sampleMean - 3.0 * sigma0;
		//right = sampleMean + 3.0 * sigma0;
		//Target->SecondMoment (left, right, sampleTwoNorm2, sampleMean, sampleTwoMoment);

		//	//double OGCoeff1 = Segment->GetMaxAtMode ();
		//double lowDataFactor = fit * sqrt (sampleTwoNorm2) / returnValue->Norm (left, right);
		//	//double peak = returnValue->Peak ();
		//returnValue->SetPeak (lowDataFactor * returnValue->Peak ());
			returnValue->SetPeak (segPeak);
		//	cout << "Super Gaussian peak with low data: Left = " << left << ". Right = " << right << ". Factor = " << segPeak << ".  Fit = " << fit << endl;
		//}
	}

	return returnValue;
}


DataSignal* NormalizedSuperGaussian :: GetResidual (const DataSignal* base, double coeff) { return NULL; }


int NormalizedSuperGaussian :: Add (list<DataSignal*>& WaveList) { return -1; }  // list of pointers to DataSignal


/*
int NormalizedSuperGaussian :: SampleAndSave () {

	double spacing = SampledData::GetSampleSpacing ();

	int size = (int)floor (((Right - Left) / spacing) + 1.0);
	size++;

	if (2 * (size / 2) == size)
		size++;

	delete[] Sample;
	SampleSpacing = spacing;
	SampleSize = size;
	Sample = new double [size];
	double x;

	double* pplus = Sample;

	for (int i=0; i<SampleSize; i++) {

		x = Left + i * spacing;
		*pplus = Value (x);
		pplus++;
	}

	return SampleSize;
}*/

/*
double NormalizedSuperGaussian :: InnerProductWithOffset (double offset, double* coeffs, const SampleDataInfo& info) const {

	//
	//  This is a "dot" product based on integration using the trapezoidal rule:  assumes
	//  target is a sampled curve and it is to be "convoluted" with a normalized Gaussian
	//  that is sampled at its center plus an input offset.  The purpose is to refine the
	//  Gaussian fit in the neighborhood of the sampled max.  This should not have to be
	//  evaluated many times - use Brent's method or some other involving iterative inverse
	//  parabolic interpolation.
	//
	//  Note:  double* target should be midpoint of array!!!
	//
	
	const double* ptargetPlus = info.DataCenter;
	const double* ptargetPlusEnd = info.DataRight;
	const double* ptargetMinus = ptargetPlus;
	double sampleSpacing = info.Spacing;
	double center = info.AbscissaLeft + info.IndexCenter * sampleSpacing;
	double x = (center - offset) / StandardDeviation;
	x *= 0.5 * x;

	double sum = exp (-x) * (*ptargetPlus);
//	int size = sampleSize - 1;
	int j = 0;
	double refRight;
	double refLeft;
	double y;
	double temp;

	for (ptargetPlus=info.DataCenter+1; ptargetPlus<ptargetPlusEnd; ptargetPlus++) {

		j++;
		ptargetMinus--;
		temp = j * sampleSpacing;
		refRight = center + temp;
		refLeft = center - temp;
		x = (refRight - offset) / StandardDeviation;
		y = (refLeft - offset) / StandardDeviation;
		x *= 0.5 * x;
		y *= 0.5 * y;
		sum += (*ptargetPlus) * exp (-x) + (*ptargetMinus) * exp (-y);
	}

	j++;
	ptargetPlus++;
	ptargetMinus--;
	temp = j * sampleSpacing;
	refRight = center + temp;
	refLeft = center - temp;
	x = (refRight - offset) / StandardDeviation;
	y = (refLeft - offset) / StandardDeviation;
	x *= 0.5 * x;
	y *= 0.5 * y;
	sum += 0.5 * ((*ptargetPlus) * exp (-x) + (*ptargetMinus) * exp (-y));

	sum *= sampleSpacing * Scale;
	return sum;
}*/

/*
double NormalizedSuperGaussian :: InnerProductWithStandardSpacing (double* coeffs, const SampleDataInfo& info) const {

	//
	//  This is a "dot" product based on integration using the trapezoidal rule:  assumes
	//  target is a sampled curve and it is to be "convoluted" with a normalized Gaussian
	//  that is sampled at its center using no offset and based on previously sampled values.
	//
	
	if (Sample == NULL)
		return -2.0;

	const double* ptargetPlus = info.DataCenter;
	const double* ptargetPlusEnd = info.DataRight;
	const double* ptargetMinus = ptargetPlus;
	double* pplus = Sample + MidPoint;
	double sum = *pplus * (*ptargetPlus);
//	int size = SampleSize - 1;

	for (ptargetPlus=info.DataCenter+1; ptargetPlus<ptargetPlusEnd; ptargetPlus++) {

		pplus++;
		ptargetMinus--;
		sum += (*ptargetPlus + *ptargetMinus) * (*pplus);
	}

	pplus++;
	ptargetPlus++;
	ptargetMinus--;
	sum += 0.5 * (*ptargetPlus + *ptargetMinus) * (*pplus);
	sum *= SampleSpacing;
	return sum;
}*/


double NormalizedSuperGaussian :: Norm () {

	return 1.0;
}


double NormalizedSuperGaussian :: Norm (double left, double right) {

	if ((left <= Left) && (right >= Right))
		return 1.0;
	
	return SuperGaussian::Norm (left, right);
}


double NormalizedSuperGaussian :: Norm2 () {
	
	// norm squared
	return 1.0;
}


double NormalizedSuperGaussian :: Norm2 (double left, double right) {

	if ((left <= Left) && (right >= Right))
		return 1.0;

	return SuperGaussian::Norm2 (left, right);
}


DataSignal* NormalizedSuperGaussian :: Normalize (double& norm) {

	norm = 1.0;
	DataSignal* ds = new NormalizedSuperGaussian (Mean, StandardDeviation, Degree);
	return ds;
}


DataSignal* NormalizedSuperGaussian :: Normalize (double left, double right, double& norm) {

	return SuperGaussian::Normalize (left, right, norm);
}


size_t NormalizedSuperGaussian :: StoreSize () const {

	return SuperGaussian::StoreSize ();
}


void NormalizedSuperGaussian :: RestoreAll (RGFile& f) {

	SuperGaussian::RestoreAll (f);
}


void NormalizedSuperGaussian :: RestoreAll (RGVInStream& f) {

	SuperGaussian::RestoreAll (f);
}


void NormalizedSuperGaussian :: SaveAll (RGFile& f) const {

	SuperGaussian::SaveAll (f);
}


void NormalizedSuperGaussian :: SaveAll (RGVOutStream& f) const {

	SuperGaussian::SaveAll (f);
}


double NormalizedSuperGaussian :: InnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* coeffs, const SampleDataInfo& info) const {

	//
	//  This is a "dot" product based on integration using the trapezoidal rule:  assumes
	//  target is a sampled curve and it is to be "convoluted" with a normalized Gaussian
	//  that is sampled at its center plus an input offset with input standard deviation, sigma.  
	//  The purpose is to refine the Gaussian fit in the neighborhood of the sampled max.  This 
	//  should not have to be evaluated many times - use Brent's method or some other involving iterative 
	//  inverse parabolic interpolation.
	//
	//  Note:  double* target should be midpoint of array!!!
	//
	
	const double* ptargetPlus = info.DataCenter;
	const double* ptargetPlusEnd = info.DataRight;
	const double* ptargetMinus = ptargetPlus;
	double ValueLeft, ValueRight;
	double sampleSpacing = info.Spacing;
	double center = info.AbscissaLeft + info.IndexCenter * sampleSpacing;
	ValueLeft = UnscaledValueWithOffsetAndSigma (center, offset, sigma);
	double sum = ValueLeft * (*ptargetPlus);
//	int size = sampleSize - 1;
	int j = 0;
	double refRight;
	double refLeft;
	double temp;

	for (ptargetPlus=info.DataCenter+1; ptargetPlus<ptargetPlusEnd; ptargetPlus++) {

		j++;
		ptargetMinus--;
		temp = j * sampleSpacing;
		refRight = center + temp;
		refLeft = center - temp;
		ValueLeft = UnscaledValueWithOffsetAndSigma (refLeft, offset, sigma);
		ValueRight = UnscaledValueWithOffsetAndSigma (refRight, offset, sigma);
		sum += (*ptargetPlus) * ValueRight + (*ptargetMinus) * ValueLeft;
	}

	j++;
	ptargetMinus--;
	temp = j * sampleSpacing;
	refRight = center + temp;
	refLeft = center - temp;
	ValueLeft = UnscaledValueWithOffsetAndSigma (refLeft, offset, sigma);
	ValueRight = UnscaledValueWithOffsetAndSigma (refRight, offset, sigma);
	sum += 0.5 * ((*ptargetPlus) * ValueRight + (*ptargetMinus) * ValueLeft);

	sum *= sqrt (RootTwo / (sigma * SuperNorm));  // This should be a normalization factor
	return sum;
}


double NormalizedSuperGaussian :: nthRoot (double x) const {

	// Uses Newton's method to find Degree'th root of x - assumes that x > 0.
	// Preferably, x > 1!

	double y = fabs (x);
	double xn;
	double xn1;
	int j;
	int Degree1 = Degree - 1;

	if (y == 0.0)
		return 0.0;

	int NSqrts = (Degree + 1) / 2;
	xn = y;
	double Increment;
	double xnToPowerDegreeMinusOne;
  int i;
	for (i=0; i<NSqrts; i++)
		xn = sqrt (xn);

	for (i=0; i<10; i++) {

		xnToPowerDegreeMinusOne = 1.0;

		for (j=0; j<Degree1; j++)
			xnToPowerDegreeMinusOne *= xn;

		Increment = (xn * xnToPowerDegreeMinusOne - y) / (Degree * xnToPowerDegreeMinusOne);
		xn1 = xn - Increment;

		if (Increment < 1.0e-13 * xn)
			break;
	}

	return xn1;
}


double NormalizedSuperGaussian :: UnscaledValueWithOffsetAndSigma (double x, double offset, double sigma) const {

	double temp = fabs ((x - offset) / sigma);
	double exponent = 1.0;

	for (int i=0; i<Degree; i++)
		exponent *= temp;

	return exp (-exponent);
}



CompositeCurve :: CompositeCurve () : ParametricCurve (), SampledVersion (NULL) {

	CurveIterator = new RGDListIterator (CurveList);
}


CompositeCurve :: CompositeCurve (double left, double right, const RGDList& Curves) : ParametricCurve (left, right),
CurveList (Curves), SampledVersion (NULL) {

	CurveIterator = new RGDListIterator (CurveList);
}


CompositeCurve :: CompositeCurve (const CompositeCurve& cc) : ParametricCurve (cc), CurveList (cc.CurveList), SampledVersion (NULL) {

	CurveIterator = new RGDListIterator (CurveList);

	if (cc.SampledVersion != NULL) {

		SampledVersion = new SampledData (*cc.SampledVersion);
	}
}



CompositeCurve :: ~CompositeCurve () {

	CurveList.ClearAndDelete ();
	delete SampledVersion;
	delete CurveIterator;
	mTempCurveList.Clear ();
}


//	void CompositeCurve :: SetDisplacement (double disp);
//	void CompositeCurve :: SetScale (double scale);


double CompositeCurve :: Value (double x) const {

	if (SampledVersion != NULL) {

		return SampledVersion->Value (x);
	}
	
	CurveIterator->Reset ();
	DataSignal* nextSignal;
	double value = 0.0;

	while (nextSignal = (DataSignal*)(*CurveIterator)())
		value += nextSignal->Value (x);

	return value;
}


RGString CompositeCurve :: GetSignalType () const {

	return RGString ("Composite Curve");
}


//	DataSignal* CompositeCurve :: Project (double left, double right) const;
//	DataSignal* CompositeCurve :: Project (const DataSignal* target) const;

//	DataSignal* CompositeCurve :: FindNextCharacteristicFromRight (const DataSignal& Signature, 
//		double& fit, RGDList& previous);
//	DataSignal* CompositeCurve :: FindNextCharacteristicFromLeft (const DataSignal& Signature, 
//		double& fit, RGDList& previous);

//	double CompositeCurve :: Centroid () const;
//	double CompositeCurve :: Centroid (double left, double right) const;
//	double CompositeCurve :: SecondMoment (double left, double right) const;

//	void CompositeCurve :: ExtendDomain (double left, double right);
//	void CompositeCurve :: ExtendDomain (const DataSignal* model);

//	DataSignal* CompositeCurve :: GetResidual (const DataSignal* base, double coeff);
//	int CompositeCurve :: Add (list<DataSignal*>& WaveList);  // list of pointers to DataSignal



DataSignal* CompositeCurve :: Digitize (int numSamples, double left, double resolution) {

	SampledData* sd = (SampledData*)Digitize (numSamples, left);
	sd->TruncateToResolution (resolution);
	return sd;
}


DataSignal* CompositeCurve :: Digitize (int numSamples, double left) {

	double spacing = SampledData::GetSampleSpacing ();

	double* sample = new double [numSamples];
	double* lastPoint = sample + numSamples;
	double right = left + (numSamples - 1) * spacing;

	double* pplus;

	for (pplus = sample; pplus < lastPoint; pplus++) {

		*pplus = 0.0;
	}
	
	RGDListIterator it (mTempCurveList);
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*)it ()) {

		nextSignal->AddToSample (sample, left, right);
	}

	SampledData* sd = new SampledData (numSamples, left, right, sample);
	return sd;
}


DataSignal* CompositeCurve :: BuildSample (int numSamples, double left, double resolution) {

	double spacing = SampledData::GetSampleSpacing ();

	double* sample = new double [numSamples];
	double* lastPoint = sample + numSamples;
	double right = left + (numSamples - 1) * spacing;

	double* pplus;

	for (pplus = sample; pplus < lastPoint; pplus++) {

		*pplus = 0.0;
	}
	
	CurveIterator->Reset ();
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*)(*CurveIterator)()) {

		if (nextSignal->IsGraphable ())
			nextSignal->AddToSampleArray (sample, left, right, resolution);
	}

	SampledData* sd = new SampledData (numSamples, left, right, sample);
	sd->TruncateToResolution (resolution);
	return sd;
}


int CompositeCurve :: SampleAndSave () {

	double spacing = SampledData::GetSampleSpacing ();

	int size = (int)floor (((Right - Left) / spacing) + 1.0);

	delete SampledVersion;
	double* sample = new double [size];
	double* lastPoint = sample + size;

	double* pplus;

	for (pplus = sample; pplus < lastPoint; pplus++) {

		*pplus = 0.0;
	}
	
	CurveIterator->Reset ();
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*)(*CurveIterator)()) {

		nextSignal->AddToSample (sample, Left, Right);
	}

	SampledVersion = new SampledData (size, Left, Right, sample);
	return size;
}


void CompositeCurve :: ProjectNeighboringSignals (double horizontalResolution, double verticalTolerance) {

	DataSignal* nextSignal;
	DataSignal* previousSignal = NULL;
	DataSignal* prev;
	DataSignal* next;
	CurveIterator->Reset ();
	double mean;
	double nextMean;
	double peak;
	double nextPeak;
	double tail;
	double nextTail;
	double spacing = DataSignal::GetSampleSpacing ();
	mTempCurveList.Clear ();
	double Right5 = 0.2 * Right;

	double f0, f1, x0, x1, xm, fm;

	while (nextSignal = (DataSignal*)(*CurveIterator)()) {

//		if (!nextSignal->IsGraphable ())
		if (nextSignal->TestForMultipleSignals (prev, next))
			continue;

		else {

			if (nextSignal != previousSignal)
				mTempCurveList.Append (nextSignal);

			previousSignal = nextSignal;
		}
	}

	RGDListIterator it (mTempCurveList);
	previousSignal = NULL;

	while (nextSignal = (DataSignal*)it ()) {

		if (previousSignal != NULL) {

			if (nextSignal == previousSignal) {

				it.RemoveCurrentItem ();
				continue;
			}
		}

		previousSignal = nextSignal;
	}

	previousSignal = NULL;
	it.Reset ();

	while (nextSignal = (DataSignal*)it ()) {

		double sigma = nextSignal->GetStandardDeviation ();
		double height = nextSignal->Peak ();

		if (ISNAN (sigma) || ISNAN (height) || (sigma == numeric_limits<double>::infinity()) || (height == numeric_limits<double>::infinity()) || (sigma > Right5)) {

			nextSignal->MarkForDeletion (true);
			continue;
		}

		if (previousSignal != NULL) {

			mean = previousSignal->GetMean ();
			nextMean = nextSignal->GetMean ();

			if (previousSignal->TestForMultipleSignals (prev, next))
				peak = previousSignal->Value (mean);

			else
				peak = previousSignal->Peak ();

			if (nextSignal->TestForMultipleSignals (prev, next))
				nextPeak = nextSignal->Value (nextMean);

			else
				nextPeak = nextSignal->Peak ();

			tail = previousSignal->Value (nextMean);
			nextTail = nextSignal->Value (mean);

			if (nextPeak <= tail)
				nextSignal->MarkForDeletion (true);

			else if (peak <= nextTail)
				previousSignal->MarkForDeletion (true);
		}

		previousSignal = nextSignal;
	}

	it.Reset ();
//	CurveIterator->Reset ();

	while (nextSignal = (DataSignal*)it ()) {

		if (nextSignal->GetMarkForDeletion ()) {

			it.RemoveCurrentItem ();
			nextSignal->MarkForDeletion (false);
		}
	}

	//CurveIterator->Reset ();
	//previousSignal = NULL;

	//while (nextSignal = (DataSignal*)(*CurveIterator)()) {

	//	if (nextSignal->IsGraphable ()) {

	//		if (previousSignal != NULL) {

	//			mean = previousSignal->GetMean ();
	//			nextMean = nextSignal->GetMean ();
	//			peak = previousSignal->Peak ();
	//			nextPeak = nextSignal->Peak ();
	//			tail = previousSignal->Value (nextMean);
	//			nextTail = nextSignal->Value (mean);

	//			if (nextPeak <= tail)
	//				nextSignal->MarkForDeletion (true);

	//			if (peak <= nextTail)
	//				previousSignal->MarkForDeletion (true);
	//		}
	//	}

	//	previousSignal = nextSignal;
	//}

	//CurveIterator->Reset ();

	//while (nextSignal = (DataSignal*)(*CurveIterator)()) {

	//	if (nextSignal->GetMarkForDeletion ()) {

	//		CurveIterator->RemoveCurrentItem ();
	//		delete nextSignal;
	//	}
	//}

	//CurveIterator->Reset ();
	//previousSignal = NULL;

	//while (nextSignal = (DataSignal*)(*CurveIterator)()) {

	//	if (nextSignal->IsGraphable ()) {

	//		if (previousSignal != NULL) {

	//			mean = previousSignal->GetMean ();
	//			nextMean = nextSignal->GetMean ();
	//			peak = previousSignal->Peak ();
	//			nextPeak = nextSignal->Peak ();
	//			tail = previousSignal->Value (nextMean);
	//			nextTail = nextSignal->Value (mean);

	//			if (nextPeak <= tail)
	//				nextSignal->MarkForDeletion (true);

	//			if (peak <= nextTail)
	//				previousSignal->MarkForDeletion (true);
	//		}
	//	}

	//	previousSignal = nextSignal;
	//}

	//CurveIterator->Reset ();

	//while (nextSignal = (DataSignal*)(*CurveIterator)()) {

	//	if (nextSignal->GetMarkForDeletion ()) {

	//		CurveIterator->RemoveCurrentItem ();
	//		delete nextSignal;
	//	}
	//}

	it.Reset ();
//	CurveIterator->Reset ();
	previousSignal = NULL;

	while (nextSignal = (DataSignal*)it ()) {

		if (previousSignal != NULL) {

			x0 = mean = previousSignal->GetMean ();
			x1 = nextMean = nextSignal->GetMean ();
//			peak = previousSignal->Peak ();

			if (previousSignal->TestForMultipleSignals (prev, next))
				peak = previousSignal->Value (mean);

			else
				peak = previousSignal->Peak ();

//			nextPeak = nextSignal->Peak ();

			if (nextSignal->TestForMultipleSignals (prev, next))
				nextPeak = nextSignal->Value (nextMean);

			else
				nextPeak = nextSignal->Peak ();

			tail = previousSignal->Value (nextMean);
			nextTail = nextSignal->Value (mean);

			if (mean > nextMean)
				continue;

//				if (((tail > verticalTolerance) || (nextTail > verticalTolerance)) && (nextPeak >= tail) && (peak >= nextTail)) {
			if ((nextPeak >= tail) && (peak >= nextTail)) {

				// Search for the intersection point of the two curves and make that point
				// the dividing line between the two

				x0 = mean;
				x1 = nextMean;
				f0 = peak - nextTail;
				f1 = tail - nextPeak;
				xm = 0.5 * (x0 + x1);

				while (x1 - x0 > horizontalResolution) {

//					xm = x0 - f0 * ((x1 - x0) / (f1 - f0));
					xm = 0.5 * (x0 + x1);
					fm = previousSignal->Value (xm) - nextSignal->Value (xm);

					if (fabs (fm) < verticalTolerance)
						break;

					if (fm > 0.0) {

						x0 = xm;
						f0 = fm;
					}

					else {

						x1 = xm;
						f1 = fm;
					}
				}

//				xm = spacing * (floor ((xm - Left) / spacing) + 0.5) + Left;
				previousSignal->SetRightEndPoint (xm);
				nextSignal->SetLeftEndPoint (xm);
			}

			else {

				xm = 0.5 * (x0 + x1);
				previousSignal->SetRightEndPoint (xm);
				nextSignal->SetLeftEndPoint (xm);
			}
		}

		previousSignal = nextSignal;
	}

	nextSignal = (DataSignal*)mTempCurveList.First ();

	if (nextSignal != NULL)
		nextSignal->SetLeftEndPoint (Left);

	nextSignal = (DataSignal*)mTempCurveList.Last ();

	if (nextSignal != NULL)
		nextSignal->SetRightEndPoint (Right);
}


double CompositeCurve :: Norm () {

	if (SampledVersion == NULL)
		SampleAndSave ();

	return SampledVersion->Norm ();
}


double CompositeCurve :: Norm (double left, double right) {

	if (SampledVersion == NULL)
		SampleAndSave ();

	return SampledVersion->Norm (left, right);
}


double CompositeCurve :: Norm2 () {
	
	// norm squared

	if (SampledVersion == NULL)
		SampleAndSave ();

	return SampledVersion->Norm2 ();
}


double CompositeCurve :: Norm2 (double left, double right) {

	if (SampledVersion == NULL)
		SampleAndSave ();

	return SampledVersion->Norm2 (left, right);
}


//	DataSignal* CompositeCurve :: Normalize (double& norm);
//	DataSignal* CompositeCurve :: Normalize (double left, double right, double& norm);


double CompositeCurve :: OneNorm () {

	if (SampledVersion == NULL)
		SampleAndSave ();

	return SampledVersion->OneNorm ();
}


double CompositeCurve :: OneNorm (double left, double right) {

	if (SampledVersion == NULL)
		SampleAndSave ();

	return SampledVersion->OneNorm (left, right);
}


size_t CompositeCurve :: StoreSize () const {

	size_t size = ParametricCurve::StoreSize ();
	size += CurveList.StoreSize ();
	return size;
}


void CompositeCurve :: RestoreAll (RGFile& f) {

	ParametricCurve::RestoreAll (f);
	CurveList.RestoreAll (f);
}


void CompositeCurve :: RestoreAll (RGVInStream& f) {

	ParametricCurve::RestoreAll (f);
	CurveList.RestoreAll (f);
}


void CompositeCurve :: SaveAll (RGFile& f) const {

	ParametricCurve::SaveAll (f);
	CurveList.SaveAll (f);
}


void CompositeCurve :: SaveAll (RGVOutStream& f) const {

	ParametricCurve::SaveAll (f);
	CurveList.SaveAll (f);
}


DualDoubleGaussian :: DualDoubleGaussian () : ParametricCurve (), PrimaryCurve (NULL), SecondaryCurve (NULL), mMean (0.0), mSigma (0.0), mPeak (0.0) {

}


DualDoubleGaussian :: DualDoubleGaussian (const DualDoubleGaussian& ddg) : ParametricCurve (ddg), PrimaryCurve (NULL), SecondaryCurve (NULL), mMean (ddg.mMean), mSigma (ddg.mSigma), mPeak (ddg.mPeak) {

	if (ddg.PrimaryCurve != NULL)
		PrimaryCurve = new DoubleGaussian (*ddg.PrimaryCurve);

	if (ddg.SecondaryCurve != NULL)
		SecondaryCurve = new DoubleGaussian (*ddg.SecondaryCurve);
}


DualDoubleGaussian :: DualDoubleGaussian (double mean, const DualDoubleGaussian& ddg) : ParametricCurve (ddg), PrimaryCurve (NULL), SecondaryCurve (NULL), mMean (mean), mSigma (ddg.mSigma), mPeak (ddg.mPeak) {

	if (ddg.PrimaryCurve != NULL)
		PrimaryCurve = new DoubleGaussian (mean, *ddg.PrimaryCurve);

	if (ddg.SecondaryCurve != NULL)
		SecondaryCurve = new DoubleGaussian (mean, *ddg.SecondaryCurve);
}


DualDoubleGaussian :: ~DualDoubleGaussian () {

	delete PrimaryCurve;
	delete SecondaryCurve;
}


DataSignal* DualDoubleGaussian :: MakeCopy (double mean) const {

	DataSignal* newSignal = new DualDoubleGaussian (mean, *this);
	return newSignal;
}



RGString DualDoubleGaussian :: GetSignalType () const {

	return RGString ("DualDoubleGaussian");
}


double DualDoubleGaussian :: Value (double x) const {

	if ((PrimaryCurve == NULL) || (SecondaryCurve == NULL))
		return -DOUBLEMAX;

	if (x <= PrimaryCurve->RightEndPoint ())
		return PrimaryCurve->Value (x);

	return SecondaryCurve->Value (x);
}


double DualDoubleGaussian :: Norm () {

	return 0.0;
}



double DualDoubleGaussian :: Norm (double left, double right) {

	return 0.0;
}



double DualDoubleGaussian :: Norm2 () {
	
	// norm squared
	return 0.0;
}


double DualDoubleGaussian :: Norm2 (double left, double right) {

	return 0.0;
}


DataSignal* DualDoubleGaussian :: FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, int windowSize, double& fit, RGDList& previous) const {

	if (Segment->GetNumberOfMinima () == 0) {
	
		fit = 0.0;
		return NULL;
	}

	DataInterval* segmentLeft;
	DataInterval* segmentRight;
	segmentLeft = Segment->Split (segmentRight);

	if ((segmentLeft == NULL) || (segmentRight == NULL)) {

		fit = 0.0;
		delete segmentLeft;
		delete segmentRight;
		return NULL;
	}

	DoubleGaussian signature;
	double fitLeft;
	double fitRight;
	
	DoubleGaussian* first = (DoubleGaussian*) signature.FindCharacteristicAsymmetric (Target, segmentLeft, windowSize, fitLeft, previous);
	DoubleGaussian* second = (DoubleGaussian*) signature.FindCharacteristicAsymmetric (Target, segmentRight, windowSize, fitRight, previous);
	
	first->SetDataMode (segmentLeft->GetMaxAtMode ());
	second->SetDataMode (segmentRight->GetMaxAtMode ());
	first->SetCurveFit (fitLeft);
	second->SetCurveFit (fitRight);
	first->SetCurrentDataInterval (segmentLeft);
	second->SetCurrentDataInterval (segmentRight);

	double sigma;
	double height;
	double mean;
	double mode;

	sigma = first->GetStandardDeviation ();
	height = first->Peak ();
	mean = first->GetMean ();

	if (ISNAN (sigma) || ISNAN (height) || (sigma == numeric_limits<double>::infinity()) || (height == numeric_limits<double>::infinity()) || (sigma < 0.0)) {

		delete first;
		delete second;
		delete segmentLeft;
		delete segmentRight;
		return NULL;
	}

	if (sigma < 0.14) {

		delete first;
		mode = floor (mean + 0.5);
		first = new DoubleGaussian (mode, 1.0);
		first->SetScale (1, height);
		first->SetScale (2, 0.0);
		first->SetLeftEndPoint (mode - 2.0);
		first->SetRightEndPoint (mode + 2.0);
		first->SetCurveFit (fitLeft);
		first->SetCurrentDataInterval (segmentLeft);
	}

	sigma = second->GetStandardDeviation ();
	height = second->Peak ();
	mean = second->GetMean ();

	if (sigma < 0.14) {

		delete second;
		mode = floor (mean + 0.5);
		second = new DoubleGaussian (mode, 1.0);
		second->SetScale (1, height);
		second->SetScale (2, 0.0);
		second->SetLeftEndPoint (mode - 2.0);
		second->SetRightEndPoint (mode + 2.0);
		second->SetCurveFit (fitRight);
		second->SetCurrentDataInterval (segmentRight);
	}

	DualDoubleGaussian* rtnValue = new DualDoubleGaussian;
	rtnValue->SetFirstCurve (first);
	rtnValue->SetSecondCurve (second);
	rtnValue->SetInfo ();

	// calculate fit and delete subsegments

	double totalMag2 = 0.0;
	double leftMag2 = 0.0;
	double rightMag2 = 0.0;
	int localMin = Segment->GetLocalMinimum ();
	int segmentStart = Segment->GetLeft ();
	int segmentEnd = Segment->GetRight ();
	double startValue = Target->Value (segmentStart);
	double endValue = Target->Value (segmentEnd);
	double currentValue;
	startValue *= startValue;
	endValue *= endValue;
	leftMag2 = 0.5 * startValue;
	rightMag2 = 0.5 * endValue;
	totalMag2 = leftMag2 + rightMag2;

	for (int i=segmentStart+1; i<segmentEnd; i++) {

		currentValue = Target->Value (i);
		currentValue *= currentValue;
		totalMag2 += currentValue;

		if (i == localMin) {

			currentValue *= 0.5;
			leftMag2 += currentValue;
			rightMag2 += currentValue;
		}

		else if (i < localMin)
			leftMag2 += currentValue;

		else if (i > localMin)
			rightMag2 += currentValue;
	}

	if (totalMag2 == 0.0)
		fit = 0.0;

	else
		fit = (fitLeft * leftMag2 + fitRight * rightMag2) / totalMag2;

	delete segmentLeft;
	delete segmentRight;
	rtnValue->SetCurveFit (fit);

	return rtnValue;
}


void DualDoubleGaussian :: SetInfo () {

	if ((PrimaryCurve == NULL) || (SecondaryCurve == NULL))
		return;

	mMean = 0.5 * (PrimaryCurve->GetMean () + SecondaryCurve->GetMean ());
	mPeak = PrimaryCurve->Peak ();
	double peak = SecondaryCurve->Peak ();

	if (peak > mPeak)
		mPeak = peak;

	mSigma = 0.5 * (PrimaryCurve->GetStandardDeviation () + SecondaryCurve->GetStandardDeviation ());
}


DataSignal* DualDoubleGaussian :: GetFirstCurve () {

	DoubleGaussian* rtn = new DoubleGaussian (*PrimaryCurve);
	return rtn;
}


DataSignal* DualDoubleGaussian :: GetSecondCurve () {

	DoubleGaussian* rtn = new DoubleGaussian (*SecondaryCurve);
	return rtn;
}


CraterSignal :: CraterSignal () : ParametricCurve (), mMean (0.0), mSigma (1.0), mHeight (0.0), mTroughHeight (0.0), mPrevious (NULL), mNext (NULL) {

	mIsGraphable = false;
	mPullupTolerance = halfCraterPullupTolerance;
	mPartOfCluster = true;
}


CraterSignal :: CraterSignal (DataSignal* prev, DataSignal* next, bool assignByProportion) : ParametricCurve (), mPrevious (prev), mNext (next) {

	// This is modified on 02/10/2016 so we can use proportional estimation of mean, etc instead of a fixed 50%.  Also, improved estimate of 
	// standard deviation for crater.
	
	double peak1 = prev->Peak ();
	double peak2 = next->Peak ();
	double d = peak1 + peak2;
	double l1;
	double l2;
	bool useRightPeak;
	mPartOfCluster = true;
//	prev->SetPartOfCluster (true);
//	next->SetPartOfCluster (true);
	bool useCalculatedProportion = true;

	if (assignByProportion) {

		if (ISNAN(peak1) || (peak1 == numeric_limits<double>::infinity( )))
			useCalculatedProportion = false;

		else if (ISNAN(peak2) || (peak2 == numeric_limits<double>::infinity( )))
			useCalculatedProportion = false;
	}


	if (peak2 >= peak1)
		useRightPeak = true;

	else
		useRightPeak = false;

	if (assignByProportion && useCalculatedProportion && (d > 0.0)) {

		l1 = peak1 / d;
		l2 = peak2 / d;
	}

	else {

		l1 = l2 = 0.5;
	}
	
	mMean = l1 * prev->GetMean () + l2 * next->GetMean ();

	double v1 = prev->Value (mMean);
	double v2 = next->Value (mMean);
	double est1 = peak1 + (peak1 - v1);
	double est2 = peak2 + (peak2 - v2);
	double max;
	mPullupTolerance = halfCraterPullupTolerance;

	if (peak1 >= peak2)
		max = peak1;

	else
		max = peak2;

	mHeight = 0.5 * (est1 + est2);
	mTroughHeight = 0.5 * (v1 + v2);

	if (mHeight < max)
		mHeight = max;

	if (useRightPeak)
		mSigma = (next->GetMean () - mMean) + next->GetStandardDeviation () * (peak2 / mHeight);

	else
		mSigma = (mMean - prev->GetMean ()) + prev->GetStandardDeviation () * (peak1 / mHeight);

	BioID = l2 * next->GetBioID () + l1 * prev->GetBioID ();
	SetChannel (prev->GetChannel ());
	ApproximateBioID = l2 * next->GetApproximateBioID () + l1 * prev->GetApproximateBioID ();
	mApproxBioIDPrime = l2 * next->GetApproxBioIDPrime () + l1 * prev->GetApproxBioIDPrime ();
	int IntBP = (int) floor (BioID + 0.5);
	Residual = BioID - (double)IntBP;

	Fit = prev->GetCurveFit ();
	double temp = next->GetCurveFit ();

	if (temp < Fit)
		Fit = temp;

	mIsGraphable = false;

	mBioIDLeft = l2 * next->GetBioID (-1) + l1 * prev->GetBioID (-1);
	mAlleleNameLeft = next->GetAlleleName (-1);
	mResidualLeft = l2 * next->GetBioIDResidual (-1) + l1 * prev->GetBioIDResidual (-1);
	mPossibleInterAlleleLeft = next->IsPossibleInterlocusAllele (-1);  // within extended locus and above fractional filter
	mIsAcceptedTriAlleleLeft = next->IsAcceptedTriAllele (-1);
	mAlleleName = next->GetAlleleName ();
	mIsOffGridLeft = next->IsOffGrid (-1);

	mBioIDRight = l2 * next->GetBioID (1) + l1 * prev->GetBioID (1);
	mAlleleNameRight = next->GetAlleleName (1);
	mResidualRight = l2 * next->GetBioIDResidual (1) + l1 * prev->GetBioIDResidual (1);
	mPossibleInterAlleleRight = next->IsPossibleInterlocusAllele (1);  // within extended locus and above fractional filter
	mIsAcceptedTriAlleleRight = next->IsAcceptedTriAllele (1);
	mIsOffGridRight = next->IsOffGrid (1);
}


CraterSignal :: CraterSignal (DataSignal* prev, DataSignal* next, DataSignal* primaryLink, bool assignByProportion) : ParametricCurve (), mPrevious (prev), mNext (next) {

	// This is modified on 02/10/2016 so we can use proportional estimation of mean, etc instead of a fixed 50%.  Also, improved estimate of 
	// standard deviation for crater.
	
	double peak1 = prev->Peak ();
	double peak2 = next->Peak ();
	double d = peak1 + peak2;
	double l1;
	double l2;
	bool useRightPeak;
	mPartOfCluster = true;
//	prev->SetPartOfCluster (true);
//	next->SetPartOfCluster (true);

	bool useCalculatedProportion = true;

	if (assignByProportion) {

		if (ISNAN(peak1) || (peak1 == numeric_limits<double>::infinity( )))
			useCalculatedProportion = false;

		else if (ISNAN(peak2) || (peak2 == numeric_limits<double>::infinity( )))
			useCalculatedProportion = false;
	}

	if (peak2 >= peak1)
		useRightPeak = true;

	else
		useRightPeak = false;

	if (assignByProportion && useCalculatedProportion && (d > 0.0)) {

		l1 = peak1 / d;
		l2 = peak2 / d;
	}

	else {

		l1 = l2 = 0.5;
	}
	
	mMean = l1 * prev->GetMean () + l2 * next->GetMean ();

	double v1 = prev->Value (mMean);
	double v2 = next->Value (mMean);
	double est1 = peak1 + (peak1 - v1);
	double est2 = peak2 + (peak2 - v2);
	double max;
	mPullupTolerance = halfCraterPullupTolerance;

	if (peak1 >= peak2)
		max = peak1;

	else
		max = peak2;

	mHeight = 0.5 * (est1 + est2);
	mTroughHeight = 0.5 * (v1 + v2);

	if (mHeight < max)
		mHeight = max;

	if (useRightPeak)
		mSigma = (next->GetMean () - mMean) + next->GetStandardDeviation () * (peak2 / mHeight);

	else
		mSigma = (mMean - prev->GetMean ()) + prev->GetStandardDeviation () * (peak1 / mHeight);

	BioID = l2 * next->GetBioID () + l1 * prev->GetBioID ();
	SetChannel (prev->GetChannel ());
	ApproximateBioID = l2 * next->GetApproximateBioID () + l1 * prev->GetApproximateBioID ();
	mApproxBioIDPrime = l2 * next->GetApproxBioIDPrime () + l1 * prev->GetApproxBioIDPrime ();
	int IntBP = (int) floor (BioID + 0.5);
	Residual = BioID - (double)IntBP;

	Fit = prev->GetCurveFit ();
	double temp = next->GetCurveFit ();

	if (temp < Fit)
		Fit = temp;

	SetPrimaryCrossChannelSignalLink (primaryLink);
	primaryLink->AddCrossChannelSignalLink ((DataSignal*)this);
	mIsGraphable = false;

	mBioIDLeft = l2 * next->GetBioID (-1) + l1 * prev->GetBioID (-1);
	mAlleleNameLeft = next->GetAlleleName (-1);
	mResidualLeft = l2 * next->GetBioIDResidual (-1) + l1 * prev->GetBioIDResidual (-1);
	mPossibleInterAlleleLeft = next->IsPossibleInterlocusAllele (-1);  // within extended locus and above fractional filter
	mIsAcceptedTriAlleleLeft = next->IsAcceptedTriAllele (-1);
	mAlleleName = next->GetAlleleName ();
	mIsOffGridLeft = next->IsOffGrid (-1);

	mBioIDRight = l2 * next->GetBioID (1) + l1 * prev->GetBioID (1);
	mAlleleNameRight = next->GetAlleleName (1);
	mResidualRight = l2 * next->GetBioIDResidual (1) + l1 * prev->GetBioIDResidual (1);
	mPossibleInterAlleleRight = next->IsPossibleInterlocusAllele (1);  // within extended locus and above fractional filter
	mIsAcceptedTriAlleleRight = next->IsAcceptedTriAllele (1);
	mIsOffGridRight = next->IsOffGrid (1);
}



CraterSignal :: CraterSignal (const CraterSignal& c) : ParametricCurve (c), mMean (c.mMean), mSigma (c.mSigma), mHeight (c.mHeight),
mTroughHeight (c.mTroughHeight), mPrevious (c.mPrevious), mNext (c.mNext) {

	mIsGraphable = false;
	ApproximateBioID = c.ApproximateBioID;
	mApproxBioIDPrime = c.mApproxBioIDPrime;
	SetChannel (c.GetChannel ());
	mPullupTolerance = c.mPullupTolerance;
}


CraterSignal :: CraterSignal (double mean, const CraterSignal& c) : ParametricCurve (c), mMean (mean), mSigma (c.mSigma), mHeight (c.mHeight),
mTroughHeight (c.mTroughHeight), mPrevious (c.mPrevious), mNext (c.mNext) {

	mIsGraphable = false;
	ApproximateBioID = c.ApproximateBioID;
	mApproxBioIDPrime = c.mApproxBioIDPrime;
	SetChannel (c.GetChannel ());
	mPullupTolerance = c.mPullupTolerance;
}



CraterSignal :: CraterSignal (const CraterSignal& c, CoordinateTransform* trans) : ParametricCurve (c), mHeight (c.mHeight), mTroughHeight (c.mTroughHeight),
mPrevious (c.mPrevious), mNext (c.mNext)  {

	mMean = trans->EvaluateWithExtrapolation (c.mMean);
	double meanPlusSigma = trans->EvaluateWithExtrapolation (c.mMean + c.mSigma);
	mSigma = meanPlusSigma - mMean;
	mIsGraphable = false;
	ApproximateBioID = c.ApproximateBioID;
	mApproxBioIDPrime = c.mApproxBioIDPrime;
	SetChannel (c.GetChannel ());
	mPullupTolerance = c.mPullupTolerance;
}



CraterSignal :: ~CraterSignal () {

}



DataSignal* CraterSignal :: MakeCopy (double mean) const {

	DataSignal* newSignal = new CraterSignal (mean, *this);
	return newSignal;
}



RGString CraterSignal :: GetSignalType () const {

	return RGString ("CraterSignal");
}


int CraterSignal :: AddNoticeToList (Notice* newNotice) {

	Notice* localNotice = (Notice*)newNotice->Copy ();
	Notice* localNotice2 = (Notice*)newNotice->Copy ();

	int result = DataSignal::AddNoticeToList (newNotice);

	if (localNotice->IsTriggerForLowPriority ()) {

		if (mPrevious != NULL)
			mPrevious->AddNoticeToList (localNotice);

		else
			delete localNotice;

		if (mNext != NULL)
			mNext->AddNoticeToList (localNotice2);

		else
			delete localNotice2;
	}

	else {

		delete localNotice;
		delete localNotice2;
	}

	return result;
}


void CraterSignal :: CaptureSmartMessages () {

	if (mPrevious != NULL)
		DataSignal::CaptureSmartMessages (mPrevious);

	if (mNext != NULL)
		DataSignal::CaptureSmartMessages (mNext);
}


bool CraterSignal :: TestForMultipleSignals (DataSignal*& prev, DataSignal*& next) {

	// Assumes that both mPrevious and mNext have been "called"

	DataSignal* tempPrev = mPrevious;
	DataSignal* tempNext = mNext;
	
	prev = tempPrev;
	next = tempNext;
	
	if ((mNext == NULL) || (mPrevious == NULL))
		return false;

	double appBpPrev = mPrevious->GetApproximateBioID ();
	double appBpNext = mNext->GetApproximateBioID ();
	double absDiff = fabs (appBpNext - appBpPrev);

	double bpPrev = mPrevious->GetBioID ();
	double bpNext = mNext->GetBioID ();
	int IntBPPrev = (int) floor (bpPrev + 0.5);
	int IntBPNext = (int) floor (bpNext + 0.5);

	if ((IntBPPrev > 0) && (IntBPNext > 0)) {

		if ((IntBPPrev != IntBPNext) && (absDiff > 0.5))
			return true;

		return false;
	}

	if (absDiff > 0.75)
		return true;

	return false;
}


bool CraterSignal :: TestForMultipleSignals (DataSignal*& prev, DataSignal*& next, int location) {

	// Assumes that both mPrevious and mNext have been "called"

	DataSignal* tempPrev = mPrevious;
	DataSignal* tempNext = mNext;
	
	prev = tempPrev;
	next = tempNext;
	
	if ((mNext == NULL) || (mPrevious == NULL))
		return false;

	double appBpPrev = mPrevious->GetApproximateBioID ();
	double appBpNext = mNext->GetApproximateBioID ();
	double absDiff = fabs (appBpNext - appBpPrev);

	if (prev->IsPossibleInterlocusAllele (location) && next->IsPossibleInterlocusAllele (location)) {

		double bpPrev = mPrevious->GetBioID (location);
		double bpNext = mNext->GetBioID (location);
		int IntBPPrev = (int) floor (bpPrev + 0.5);
		int IntBPNext = (int) floor (bpNext + 0.5);

		if ((IntBPPrev != IntBPNext) && (absDiff > 0.75))
			return true;

		return false;
	}

	if (absDiff > 0.75)
		return true;

	return false;
}


void CraterSignal :: SetAlleleInformation (int position) {

	DataSignal::SetAlleleInformation (position);

	if (mPrevious != NULL)
		mPrevious->SetAlleleInformation (position);

	if (mNext != NULL)
		mNext->SetAlleleInformation (position);
}


double CraterSignal :: Value (double x) const {

	double leftValue = 0.0;
	double rightValue = 0.0;

	if (mPrevious != NULL)
		leftValue = mPrevious->Value (x);

	if (mNext != NULL)
		rightValue = mNext->Value (x);

	if (rightValue > leftValue)
		leftValue = rightValue;

	return leftValue;
}


double CraterSignal :: Value (int n) const {

	double leftValue = 0.0;
	double rightValue = 0.0;

	if (mPrevious != NULL)
		leftValue = mPrevious->Value (n);

	if (mNext != NULL)
		rightValue = mNext->Value (n);

	if (rightValue > leftValue)
		leftValue = rightValue;

	return leftValue;
}


double CraterSignal :: Norm (double a, double b) {

	return 0.0;
}


double CraterSignal :: Norm () {

	return 0.0;
}


double CraterSignal :: Norm2 (double a, double b) {

	return 0.0;
}


double CraterSignal :: Norm2 () {

	return 0.0;
}


double CraterSignal :: Centroid () const {

	return mMean;
}


double CraterSignal :: Peak () const  {

	return mHeight;
}



double CraterSignal :: GetMean () const {

	return mMean;
}


double CraterSignal :: GetStandardDeviation () const {

	return mSigma;
}


double CraterSignal :: GetWidth () {

	if (mWidth > 0.0)
		return mWidth;

	mWidth = mNext->GetMean () - mPrevious->GetMean () + 0.5 * (mPrevious->GetWidth () + mNext->GetWidth ());
	return mWidth;
}


double CraterSignal :: GetVariance () const {

	return mSigma * mSigma;
}



void CraterSignal :: SetMessageValue (int scope, int location, bool value) {

	int myScope = GetObjectScope ();

	if (myScope == scope) {

		SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, location);
		DataSignal::SetMessageValue (scope, location, value);
//		msg->SetMessageValue (mMessageArray, mValueArray, location, value);

		if (msg->GetShareWithCluster ()) {

			if (mPrevious != NULL)
				mPrevious->SetMessageValue (scope, location, value);

			if (mNext != NULL)
				mNext->SetMessageValue (scope, location, value);
		}
	}
}


void CraterSignal :: SetMessageValue (const SmartNotice& notice, bool value) {

	if (notice.GetScope () == GetObjectScope ()) {

//		mMessageArray [notice.GetMessageIndex ()] = value;
		DataSignal::SetMessageValue (notice, value);

		int location = notice.GetMessageIndex ();
		int scope = GetObjectScope ();
		SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, location);

		if (msg->GetShareWithCluster ()) {

			if (mPrevious != NULL)
				mPrevious->SetMessageValue (notice, value);

			if (mNext != NULL)
				mNext->SetMessageValue (notice, value);
		}
	}
	
}


//testmsgvoid CraterSignal :: SetMessageValue (const SmartNotice& notice, bool value, bool useVirtualMethod) {
//testmsg
	//testmsgif (useVirtualMethod)
	//testmsg	SetMessageValue (notice.GetScope (), notice.GetMessageIndex (), value);

	//testmsg	else
	//testmsg	SetMessageValue (notice, value);
//testmsg}


void CraterSignal :: SetMessageValue (int scope, int location, bool value, bool useVirtualMethod) {

	if (useVirtualMethod)
		SetMessageValue (scope, location, value);

	else if (scope == GetObjectScope ()) {

//		mMessageArray [location] = value;
		DataSignal::SetMessageValue (scope, location, value);
		SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, location);

		if (msg->GetShareWithCluster ()) {

			if (mPrevious != NULL)
				mPrevious->SetMessageValue (scope, location, value);

			if (mNext != NULL)
				mNext->SetMessageValue (scope, location, value);
		}
	}
}



void CraterSignal :: Report (RGTextOutput& text, const RGString& indent) {

}


size_t CraterSignal :: StoreSize () const {

	size_t size = ParametricCurve::StoreSize ();
	return size;
}


void CraterSignal :: RestoreAll (RGFile& f) {

	ParametricCurve::RestoreAll (f);
}


void CraterSignal :: RestoreAll (RGVInStream& f) {

	ParametricCurve::RestoreAll (f);
}


void CraterSignal :: SaveAll (RGFile& f) const {

	ParametricCurve::SaveAll (f);
}


void CraterSignal :: SaveAll (RGVOutStream& f) const {

	ParametricCurve::SaveAll (f);
}


SimpleSigmoidSignal :: SimpleSigmoidSignal () : CraterSignal () {

	SetDoNotCall (true);
}



SimpleSigmoidSignal :: SimpleSigmoidSignal (DataSignal* prev, DataSignal* next) : CraterSignal () {

	mPrevious = prev;
	mNext = next;
	SetDoNotCall (true);
	double P1 = fabs (prev->Peak ());
	double P2 = fabs (next->Peak ());

	//cout << "Sigmoid P1 = " << P1 << endl;
	//cout << "Sigmoid P2 = " << P2 << endl;

	double denom = P1 + P2;
	double lambda;

	//cout << "Crater p1 + p2 = " << denom << endl;
	//cout << "Prev mean = " << prev->GetMean () << endl;
	//cout << "Next mean = " << next->GetMean () << endl;
	//cout << "Prev fit = " << prev->GetCurveFit () << endl;
	//cout << "Next fit = " << next->GetCurveFit () << endl;

	if (denom == 0.0)
		lambda = 0.5;

	else if (ISNAN(P1) || ISNAN(P2) || (P1 == numeric_limits<double>::infinity( )) || (P2 == numeric_limits<double>::infinity( )))
		lambda = 0.5;

	//else if ((prev->GetCurveFit () < 0.97) || (next->GetCurveFit () < 0.97))
	//	lambda = 0.5;

	else
		lambda = P2 / denom;

	if (lambda < 0.1)
		lambda = 0.1;

	if (lambda > 0.9)
		lambda = 0.9;

	//cout << "lambda = " << lambda << endl;

	double lambda1 = 1.0 - lambda;

	mMean = lambda * prev->GetMean () + lambda1 * next->GetMean ();
	mSigma = lambda * prev->GetStandardDeviation () + lambda1 * next->GetStandardDeviation ();
	mHeight = 1.0;

	ApproximateBioID = lambda1 * next->GetApproximateBioID () + lambda * prev->GetApproximateBioID ();
	mApproxBioIDPrime = lambda1 * next->GetApproxBioIDPrime () + lambda * prev->GetApproxBioIDPrime ();
	SetChannel (prev->GetChannel ());
	Fit = prev->GetCurveFit ();
	double temp = next->GetCurveFit ();

	if (temp < Fit)
		Fit = temp;

	mIsGraphable = false;
}



SimpleSigmoidSignal :: SimpleSigmoidSignal (DataSignal* prev, DataSignal* next, DataSignal* primaryLink) : CraterSignal () {

	mPrevious = prev;
	mNext = next;
	SetDoNotCall (true);
	double P1 = fabs (prev->Peak ());
	double P2 = fabs (next->Peak ());

	//cout << "Sigmoid P1 = " << P1 << endl;
	//cout << "Sigmoid P2 = " << P2 << endl;

	double denom = P1 + P2;
	double lambda;

	//cout << "Crater p1 + p2 = " << denom << endl;
	//cout << "Prev mean = " << prev->GetMean () << endl;

	if (denom == 0.0)
		lambda = 0.5;

	else if (ISNAN(P1) || ISNAN(P2) || (P1 == numeric_limits<double>::infinity( )) || (P2 == numeric_limits<double>::infinity( )))
		lambda = 0.5;

	//else if ((prev->GetCurveFit () < 0.97) || (next->GetCurveFit () < 0.97))
	//	lambda = 0.5;

	else
		lambda = P2 / denom;

	if (lambda < 0.1)
		lambda = 0.1;

	if (lambda > 0.9)
		lambda = 0.9;

	//cout << "lambda = " << lambda << endl;

	double lambda1 = 1.0 - lambda;

	mMean = lambda * prev->GetMean () + lambda1 * next->GetMean ();
	mSigma = lambda * prev->GetStandardDeviation () + lambda1 * next->GetStandardDeviation ();
	mHeight = 1.0;

	ApproximateBioID = lambda1 * next->GetApproximateBioID () + lambda * prev->GetApproximateBioID ();
	mApproxBioIDPrime = lambda1 * next->GetApproxBioIDPrime () + lambda * prev->GetApproxBioIDPrime ();
	SetChannel (prev->GetChannel ());
	Fit = prev->GetCurveFit ();
	double temp = next->GetCurveFit ();

	if (temp < Fit)
		Fit = temp;

	mIsGraphable = false;
	SetPrimaryCrossChannelSignalLink (primaryLink);
	primaryLink->AddCrossChannelSignalLink ((DataSignal*)this);
}



SimpleSigmoidSignal :: SimpleSigmoidSignal (const SimpleSigmoidSignal& c) : CraterSignal () {

	mPrevious = c.mPrevious;
	mNext = c.mNext;
	mMean = c.mMean;
	mSigma = c.mSigma;
	mHeight = c.mHeight;
	mIsGraphable = false;
	ApproximateBioID = c.ApproximateBioID;
	mApproxBioIDPrime = c.mApproxBioIDPrime;
	SetChannel (c.GetChannel ());
	SetDoNotCall (true);
	Fit = c.Fit;
}



SimpleSigmoidSignal :: SimpleSigmoidSignal (double mean, const SimpleSigmoidSignal& c) : CraterSignal () {

	mPrevious = c.mPrevious;
	mNext = c.mNext;
	mMean = mean;
	mSigma = c.mSigma;
	mHeight = c.mHeight;
	mIsGraphable = false;
	ApproximateBioID = c.ApproximateBioID;
	mApproxBioIDPrime = c.mApproxBioIDPrime;
	SetChannel (c.GetChannel ());
	SetDoNotCall (true);
	Fit = c.Fit;
}



SimpleSigmoidSignal :: SimpleSigmoidSignal (const SimpleSigmoidSignal& c, CoordinateTransform* trans) : CraterSignal ()  {

	mPrevious = c.mPrevious;
	mNext = c.mNext;
	mHeight = c.mHeight;
	mMean = trans->EvaluateWithExtrapolation (c.mMean);
	double meanPlusSigma = trans->EvaluateWithExtrapolation (c.mMean + c.mSigma);
	mSigma = meanPlusSigma - mMean;
	mIsGraphable = false;
	ApproximateBioID = c.ApproximateBioID;
	mApproxBioIDPrime = c.mApproxBioIDPrime;
	SetChannel (c.GetChannel ());
	SetDoNotCall (true);
	Fit = c.Fit;
}



SimpleSigmoidSignal :: ~SimpleSigmoidSignal () {

}


double SimpleSigmoidSignal :: GetWidth () {

	if (mWidth > 0.0)
		return mWidth;

	mWidth = mNext->GetMean () - mPrevious->GetMean ();
	return mWidth;
}


bool SimpleSigmoidSignal :: LiesBelowHeightAt (double x, double height) {

	return true;
}



bool SimpleSigmoidSignal :: TestForIntersectionWithPrimary (DataSignal* primary) {

	if ((mPrevious == NULL) || (mNext == NULL))
		return false;

	double mu1 = mPrevious->GetMean ();
	double mu2 = mNext->GetMean ();
	double p1 = mPrevious->Value (mu1);
	double p2 = mNext->Value (mu2);

	if (primary->LiesBelowHeightAt (mu1, p1))
		return true;

	if (primary->LiesBelowHeightAt (mu2, p2))
		return true;

	return false;
}


DataSignal* SimpleSigmoidSignal :: MakeCopy (double mean) const {

	DataSignal* newSignal = new SimpleSigmoidSignal (mean, *this);
	return newSignal;
}



RGString SimpleSigmoidSignal :: GetSignalType () const {

	return RGString ("SimpleSigmoidSignal");
}



int SimpleSigmoidSignal :: AddNoticeToList (Notice* newNotice) {

	Notice* localNotice = (Notice*)newNotice->Copy ();
	Notice* localNotice2 = (Notice*)newNotice->Copy ();

	int result = DataSignal::AddNoticeToList (newNotice);

	if (localNotice->IsTriggerForLowPriority ()) {

		if ((mPrevious != NULL) && !mPrevious->IsNegativePeak ())
			mPrevious->AddNoticeToList (localNotice);

		else
			delete localNotice;

		if ((mNext != NULL) && !mNext->IsNegativePeak ())
			mNext->AddNoticeToList (localNotice2);

		else
			delete localNotice2;
	}

	else {

		delete localNotice;
		delete localNotice2;
	}

	return result;
}


bool SimpleSigmoidSignal :: TestForMultipleSignals (DataSignal*& prev, DataSignal*& next) {

	prev = mPrevious;
	next = mNext;

	//DataSignal* tempPrev = mPrevious;
	//DataSignal* tempNext = mNext;
	
	//prev = tempPrev;
	//next = tempNext;
	
	//if ((mNext == NULL) || (mPrevious == NULL))
	//	return false;

	//double appBpPrev = mPrevious->GetApproximateBioID ();
	//double appBpNext = mNext->GetApproximateBioID ();
	//double absDiff = fabs (appBpNext - appBpPrev);

	//if (absDiff > 0.75)
	//	return true;

	return false;
}



bool SimpleSigmoidSignal :: TestForMultipleSignals (DataSignal*& prev, DataSignal*& next, int location) {

	prev = mPrevious;
	next = mNext;

	//DataSignal* tempPrev = mPrevious;
	//DataSignal* tempNext = mNext;
	//
	//prev = tempPrev;
	//next = tempNext;
	
	//if ((mNext == NULL) || (mPrevious == NULL))
	//	return false;

	//double appBpPrev = mPrevious->GetApproximateBioID ();
	//double appBpNext = mNext->GetApproximateBioID ();
	//double absDiff = fabs (appBpNext - appBpPrev);

	//if (absDiff > 0.75)
	//	return true;

	return false;
}



bool SimpleSigmoidSignal :: TestForMultipleSignalsWithinLocus (DataSignal*& prev, DataSignal*& next, int location, bool isAmel, double adenylationLimit) {

	prev = mPrevious;
	next = mNext;

	//prev = mPrevious;
	//next = mNext;
	
	//if ((mNext == NULL) || (mPrevious == NULL))
	//	return false;

	//double appBpPrev = mPrevious->GetApproximateBioID ();
	//double appBpNext = mNext->GetApproximateBioID ();
	//double absDiff = fabs (appBpNext - appBpPrev);

	//if (absDiff > 0.75)
	//	return true;		

	return false;
}



double SimpleSigmoidSignal :: Value (double x) const {

	if (mPrevious->IsNegativePeak ())
		return mNext->Value (x);

	return mPrevious->Value (x);
}


double SimpleSigmoidSignal :: Value (int n) const {

	if (mPrevious->IsNegativePeak ())
		return mNext->Value (n);

	return mPrevious->Value (n);
}


double SimpleSigmoidSignal :: Norm (double a, double b) {

	if (mPrevious->IsNegativePeak ())
		return mNext->Norm (a, b);

	return mPrevious->Norm (a, b);
}


double SimpleSigmoidSignal :: Norm () {

	if (mPrevious->IsNegativePeak ())
		return mNext->Norm ();

	return mPrevious->Norm ();
}


double SimpleSigmoidSignal :: Norm2 (double a, double b) {

	if (mPrevious->IsNegativePeak ())
		return mNext->Norm2 (a, b);

	return mPrevious->Norm2 (a, b);
}


double SimpleSigmoidSignal :: Norm2 () {

	if (mPrevious->IsNegativePeak ())
		return mNext->Norm2 ();

	return mPrevious->Norm2 ();
}


void SimpleSigmoidSignal :: SetMessageValue (int scope, int location, bool value) {

	int myScope = GetObjectScope ();

	if (myScope == scope) {

		SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, location);
		DataSignal::SetMessageValue (scope, location, value);
//		msg->SetMessageValue (mMessageArray, mValueArray, location, value);

		if (msg->GetShareWithCluster ()) {

			if ((mPrevious != NULL) && mPrevious->IsNegativePeak ()) {

				if (mNext != NULL)
					mNext->SetMessageValue (scope, location, value);
			}

			if ((mNext != NULL) && mNext->IsNegativePeak ()) {

				if (mPrevious != NULL)
					mPrevious->SetMessageValue (scope, location, value);
			}
		}
	}
}



void SimpleSigmoidSignal :: SetMessageValue (const SmartNotice& notice, bool value) {

	if (notice.GetScope () == GetObjectScope ()) {

//		mMessageArray [notice.GetMessageIndex ()] = value;
		DataSignal::SetMessageValue (notice, value);

		int location = notice.GetMessageIndex ();
		int scope = GetObjectScope ();
		SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, location);

		if (msg->GetShareWithCluster ()) {

			if ((mPrevious != NULL) && mPrevious->IsNegativePeak ()) {

				if (mNext != NULL)
					mNext->SetMessageValue (scope, location, value);
			}

			if ((mNext != NULL) && mNext->IsNegativePeak ()) {

				if (mPrevious != NULL)
					mPrevious->SetMessageValue (scope, location, value);
			}
		}
	}
}



void SimpleSigmoidSignal :: SetMessageValue (int scope, int location, bool value, bool useVirtualMethod) {

	if (useVirtualMethod)
		SetMessageValue (scope, location, value);

	else if (scope == GetObjectScope ()) {

//		mMessageArray [location] = value;
		DataSignal::SetMessageValue (scope, location, value);
		SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, location);

		if (msg->GetShareWithCluster ()) {

			if ((mPrevious != NULL) && mPrevious->IsNegativePeak ()) {

				if (mNext != NULL)
					mNext->SetMessageValue (scope, location, value);
			}

			if ((mNext != NULL) && mNext->IsNegativePeak ()) {

				if (mPrevious != NULL)
					mPrevious->SetMessageValue (scope, location, value);
			}
		}
	}
}


NoisyPeak :: NoisyPeak () : CraterSignal () {}


NoisyPeak :: NoisyPeak (DataSignal* prev, DataSignal* next, bool assignByProportion) : CraterSignal (prev, next, true) {

	double peak1 = prev->Peak ();
	double peak2 = next->Peak ();
	double d = peak1 + peak2;
	bool useRightPeak;
	prev->SetDoNotCall (true);
	next->SetDoNotCall (true);
	prev->SetDontLook (true);
	next->SetDontLook (true);
	prev->SetNoisySidePeak (true);
	next->SetNoisySidePeak (true);

	if (peak2 >= peak1)
		useRightPeak = true;

	else
		useRightPeak = false;

	if (d > 0.0) {

		mL1 = peak1 / d;
		mL2 = peak2 / d;
	}

	else {

		mL1 = mL2 = 0.5;
	}
}


NoisyPeak :: NoisyPeak (DataSignal* prev, DataSignal* next, DataSignal* primaryLink, bool assignByProportion) : CraterSignal (prev, next, primaryLink, true) {

	double peak1 = prev->Peak ();
	double peak2 = next->Peak ();
	double d = peak1 + peak2;
	bool useRightPeak;
	prev->SetDoNotCall (true);
	next->SetDoNotCall (true);
	prev->SetDontLook (true);
	next->SetDontLook (true);
	prev->SetNoisySidePeak (true);
	next->SetNoisySidePeak (true);

	if (peak2 >= peak1)
		useRightPeak = true;

	else
		useRightPeak = false;

	if (d > 0.0) {

		mL1 = peak1 / d;
		mL2 = peak2 / d;
	}

	else {

		mL1 = mL2 = 0.5;
	}
}


NoisyPeak :: NoisyPeak (const NoisyPeak& c) : CraterSignal ((CraterSignal&) c), mL1 (c.mL1), mL2 (c.mL2) {

}



NoisyPeak :: NoisyPeak (double mean, const NoisyPeak& c) : CraterSignal (mean, (CraterSignal&)c), mL1 (c.mL1), mL2 (c.mL2) {

}



NoisyPeak :: NoisyPeak (const NoisyPeak& c, CoordinateTransform* trans) : CraterSignal ((CraterSignal&)c, trans), mL1 (c.mL1), mL2 (c.mL2) {

}


NoisyPeak :: ~NoisyPeak () {

}


double NoisyPeak :: GetWidth () {

	return CraterSignal::GetWidth ();
}


DataSignal* NoisyPeak :: MakeCopy (double mean) const {

	DataSignal* newSignal = new NoisyPeak (mean, *this);
	return newSignal;
}



double NoisyPeak :: GetPrimaryPullupDisplacementThreshold () {

	if ((mNext == NULL) || (mPrevious == NULL))
		return 2.0;

	return mSigma;
}



double NoisyPeak :: GetPrimaryPullupDisplacementThreshold (double nSigmas) {

	return nSigmas * mSigma;
}



RGString NoisyPeak :: GetSignalType () const {

	return RGString ("NoisySignal");
}



bool NoisyPeak :: TestForMultipleSignals (DataSignal*& prev, DataSignal*& next) {

	prev = mPrevious;
	next = mNext;
	return false;
}


bool NoisyPeak :: TestForMultipleSignals (DataSignal*& prev, DataSignal*& next, int location) {

	prev = mPrevious;
	next = mNext;
	return false;
}


bool NoisyPeak :: TestForMultipleSignalsWithinLocus (DataSignal*& prev, DataSignal*& next, int location, bool isAmel, double adenylationLimit) {

	prev = mPrevious;
	next = mNext;
	return false;
}



bool NoisyPeak :: TestSignalGroupsWithinILS (double ilsLeft, double ilsRight, double minBioID) {

	return true;
}

void NoisyPeak :: SetAlleleInformation (int position) {

	DataSignal::SetAlleleInformation (position);

	if (mPrevious != NULL)
		mPrevious->SetAlleleInformation (position);

	if (mNext != NULL)
		mNext->SetAlleleInformation (position);
}


SpikeSignal :: SpikeSignal () : ParametricCurve (-1.0, 1.0), mMean (0.0), mPeak (1.0) {}


SpikeSignal :: SpikeSignal (double mean, double peak, double leftValue, double rightValue) : ParametricCurve (-1.0, 1.0), mMean (mean), mPeak (peak) {}


SpikeSignal :: SpikeSignal (const SpikeSignal& spike) : ParametricCurve ((ParametricCurve&)spike), mMean (spike.mMean), mPeak (spike.mPeak) {}


SpikeSignal :: SpikeSignal (double mean, const SpikeSignal& spike) : ParametricCurve ((ParametricCurve&)spike), mMean (mean), mPeak (spike.mPeak) {

}


SpikeSignal :: ~SpikeSignal () {

}


DataSignal* SpikeSignal :: MakeCopy (double mean) const {

	DataSignal* newSignal = new SpikeSignal (mean, *this);
	return newSignal;
}


RGString SpikeSignal :: GetSignalType () const {

	return RGString ("SpikeSignal");
}


double SpikeSignal :: GetWidth () {

	return 1.0;
}



double SpikeSignal :: GetPullupToleranceInBP (double noise) const {

	//pullUpToleranceFactor
	double P = mPeak;

	if (P <= 0.0)
		return (mPullupTolerance + (2.0 * sin (0.5 * acos (Fit)) / 4.47));

	double localFit = Fit;

	if (localFit < 0.1)
		localFit = 0.1;

	double localNoise = noise;
	
	double LN = 0.95 * P;

	if (localNoise > LN)
		localNoise = LN;

	double temp1 = 1.0 / localFit;
	double temp = 2.0 * (temp1 * temp1 - 1.0) * sqRoot_two_thirds * log (P / (P - localNoise));
	return (mPullupTolerance + pullUpToleranceFactor * mApproxBioIDPrime * sqrt (temp));
}


void SpikeSignal :: SetDisplacement (double disp) {

	mMean += disp;
}


double SpikeSignal :: Value (double x) const {

	if (x <= mMean) {

		if (x <= mMean - 1.0)
			return 0.0;

		return mPeak * (x + 1.0 - mMean);
	}

	else {

		if (x > mMean + 1.0)
			return 0.0;

		return mPeak * (mMean + 1.0 - x);
	}
}


double SpikeSignal :: Value (int x) const {

	int mean = (int) mMean;

	if (x <= mean - 1)
		return 0.0;

	if (x >= mean + 1)
		return 0.0;

	return mPeak;
}


double SpikeSignal :: Peak () const {

	return mPeak;
}


double SpikeSignal :: GetMean () const {

	return mMean;
}


double SpikeSignal :: GetStandardDeviation () const {

	return sqRoot_two_thirds;
}


double SpikeSignal :: GetVariance () const {

	return two_thirds;
}


void SpikeSignal :: SetPeak (double peak) {

	mPeak = peak;
}



void SpikeSignal :: ComputeTails (double& tailLeft, double& tailRight) const {

	tailLeft = mMean - 1.0;
	tailRight = mMean + 1.0;
}


//DataSignal* SpikeSignal :: Project (double left, double right) const {
//
//}
//
//
//
//DataSignal* Project (const DataSignal* target) const {
//
//}


double SpikeSignal :: Centroid () const {

	return mMean;
}


double SpikeSignal :: Norm () {

	return mPeak;
}


double SpikeSignal :: Norm (double left, double right) {

	return Norm ();
}


double SpikeSignal :: Norm2 () {  // norm squared

	return mPeak * mPeak;
}


double SpikeSignal :: Norm2 (double left, double right) {

	return Norm2 ();
}


DataSignal* SpikeSignal :: Normalize (double& norm) {

	norm = Norm ();
	DataSignal* newSpike = new SpikeSignal (*this);
	newSpike->SetPeak (1.0);
	return newSpike;
}



DataSignal* SpikeSignal :: Normalize (double left, double right, double& norm) {

	return Normalize (norm);
}



void SpikeSignal :: CalculateTheoreticalArea () {

	mArea = mPeak;
}



double SpikeSignal :: OneNorm () {

	return mPeak;
}


double SpikeSignal :: OneNorm (double left, double right) {

	return mPeak;
}


int SpikeSignal :: FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean) {

	oneNorm = OneNorm ();
	mean = mMean;
	return 0;
}


int SpikeSignal :: SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment) {

	oneNorm = OneNorm ();
	mean = mMean;
	secondMoment = Norm2 ();
	return 0;
}

void SpikeSignal :: Report (RGTextOutput& text, const RGString& indent) {

	Endl endLine;
	text << indent << "Spike Signal with parameters:\n" << endLine;
	text << indent << "   Mean  = " << mMean << endLine;
	text << indent << "   Height = " << mPeak << endLine;
	double mass = OneNorm ();
	text << indent << "   Mass  = " << mass << endLine;
	text << indent << "   Fit   = " << Fit << endLine;
}



size_t SpikeSignal :: StoreSize () const {

	size_t size = ParametricCurve::StoreSize ();
	size += 2 * sizeof (double);
	return size;
}

void SpikeSignal :: RestoreAll (RGFile& f) {

	ParametricCurve::RestoreAll (f);
	f.Read (mMean);
	f.Read (mPeak);
}


void SpikeSignal :: RestoreAll (RGVInStream& r) {

	ParametricCurve::RestoreAll (r);
	r >> mMean;
	r >> mPeak;
}


void SpikeSignal :: SaveAll (RGFile& f) const {

	ParametricCurve::SaveAll (f);
	f.Write (mMean);
	f.Write (mPeak);
}


void SpikeSignal :: SaveAll (RGVOutStream& r) const {

	ParametricCurve::SaveAll (r);
	r << mMean;
	r << mPeak;
}



