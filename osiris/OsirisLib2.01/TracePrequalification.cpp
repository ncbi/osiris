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
*  FileName: TracePrequalification.cpp
*  Author:   Robert Goor
*
*/
//
//  class TracePrequalification is an abstract base class of classes used to prequalify part of a DataSignal for 
//  more in-depth characteristic search.  Specialized subclasses can define specific strategies for different types 
//  of forensic analysis.  TracePrequalification and subclasses are subclasses of RGPersistent.
//

#include "TracePrequalification.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "DataInterval.h"
#include "DataSignal.h"
#include "RGTextOutput.h"


double TracePrequalification::noiseThreshold = 400.0;
int TracePrequalification::windowWidth = 9;
double TracePrequalification::lowHeightThreshold = 0.1;
double TracePrequalification::lowSlopeThreshold = 0.1;
int TracePrequalification::minSamplesForSlopeRegression = 4;
double TracePrequalification::defaultNoiseThreshold = 400.0;
int TracePrequalification::defaultWindowWidth = 9;


ABSTRACT_DEFINITION (TracePrequalification)
PERSISTENT_DEFINITION (STRTracePrequalification, _STRTRACEPREQUALIFICATION_, "STRTracePrequal")


double LinearSlopeRegression (const double* y, double delt, int N1) {

	// returns slope; N1 is N-1

	double eN1 = (double)N1;
	double en = eN1 + 1.0;
	double xbar = eN1 * 0.5;
	double ybar = 0.0;
	double r = 0.0;
	double var = 0.0;
	double temp;
  int i;
	for (i=0; i<=N1; i++) {

		ybar += y [i];
	}

	ybar = ybar / en;

	for (i=0; i<=N1; i++) {

		temp = (double) i - xbar;
		r += temp * (y [i] - ybar);
		var += temp * temp;
	}

	if (var == 0.0)
		return 0.0;

	return r / (delt * var);
}


enum SEARCHMODE {LookingForMax, LookingForMin, LookingForFinalMin, UnravelingFinalCurve};


TracePrequalification :: ~TracePrequalification () {}


void TracePrequalification :: SetWindowWidth (int width) {

//	if (2 * (width / 2) < width)		//  !!! Changed to accomodate odd-valued WindowWidth's
//		windowWidth = width + 1;		//  !!! Changed to accomodate odd-valued WindowWidth's
	
//	else								//  !!! Changed to accomodate odd-valued WindowWidth's
		windowWidth = width;
}


void TracePrequalification :: SetDefaultWindowWidth (int width) {

//	if (2 * (width / 2) < width)		//  !!! Changed to accomodate odd-valued WindowWidth's
//		windowWidth = width + 1;		//  !!! Changed to accomodate odd-valued WindowWidth's
	
//	else								//  !!! Changed to accomodate odd-valued WindowWidth's
		defaultWindowWidth = width;
}


void TracePrequalification :: SetMinSamplesForSlopeRegression (int samples) {

	if (samples < 3)
		minSamplesForSlopeRegression = 3;

	else
		minSamplesForSlopeRegression = samples;
}


STRTracePrequalification :: STRTracePrequalification () : TracePrequalification (),
CumulativeNorm (0.0), Data (NULL), MaxIndex (-1), mConvolution (NULL), mSlopeFits (NULL) {

	WindowWidth = TracePrequalification::GetWindowWidth ();
	NoiseThreshold = TracePrequalification::GetNoiseThreshold ();
	Spacing = DataSignal::GetSampleSpacing ();
	HalfWindow = WindowWidth / 2;
}


STRTracePrequalification :: ~STRTracePrequalification () {

	delete[] mConvolution;
	delete[] mSlopeFits;
}


void STRTracePrequalification :: ResetSearch (const DataSignal* data, int size, double channelNoise) {

	Data = data;
	const double* dataArray = Data->GetData ();
	double delt = Data->GetSampleSpacing ();
	mChannelNoise = channelNoise;

	MaxIndex = size - 1;
	WindowWidth = TracePrequalification::GetWindowWidth ();
	NoiseThreshold = TracePrequalification::GetNoiseThreshold ();
	Spacing = DataSignal::GetSampleSpacing ();
	HalfWindow = WindowWidth / 2;

	mConvolution = new double [size];
	mSlopeFits = new double [size];

	double a = Data->Value (0);
	double z = Data->Value (MaxIndex);
	CumulativeNorm = 0.5 * (a * a + z * z);
	CurrentIndex = MaxIndex - 1;
	CurrentConvolution = 0.0;
	
	NewConvolution = 0.0;
	WindowLeft = CurrentIndex - HalfWindow;
	WindowRight = CurrentIndex + HalfWindow;

	if (WindowRight - WindowLeft < WindowWidth)		//  !!! Added to accomodate odd-valued WindowWidth's
		WindowRight++;								//  !!! Added to accomodate odd-valued WindowWidth's

	int halfWindow = WindowWidth / 2;
	int localMinSamplesForRegression = TracePrequalification::GetMinSamplesForSlopeRegression ();

	if (halfWindow < localMinSamplesForRegression)
		halfWindow = localMinSamplesForRegression;

	int halfWindow1 = halfWindow - 1;
	int halfHalfWindow = halfWindow / 2;
  int i;
	for (i=WindowLeft; i<=CurrentIndex; i++)
		CurrentConvolution += Data->Value (i);

	PreviousConvolution = CurrentConvolution + 1.0;
	mConvolution [CurrentIndex] = CurrentConvolution;
	mConvolution [MaxIndex] = PreviousConvolution;
	mSlopeFits [CurrentIndex] = mSlopeFits [MaxIndex] = 0.0;
	LastMinimum = CurrentConvolution;
	OldLeft = Data->Value (WindowLeft);
	OldRight = 0.0;

	double newL;
	double newR;
	double newCenter;
	double NewConvolution;

	for (i=CurrentIndex-1; i>0; i--) {

		newL = Data->Value (i - HalfWindow);
		newR = Data->Value (i + HalfWindow);
		newCenter = Data->Value (i);
		NewConvolution = CurrentConvolution + 0.5 * (OldLeft + newL - OldRight - newR);
		mConvolution [i] = NewConvolution;

		if (i - halfHalfWindow >= 0)
			mSlopeFits [i] = LinearSlopeRegression (dataArray + i - halfHalfWindow, delt, halfWindow1);

		else
			mSlopeFits [i] = 0.0;

		CumulativeNorm += newCenter * newCenter;
		OldLeft = newL;
		OldRight = newR;
		CurrentConvolution = NewConvolution;
	}
}


DataInterval* STRTracePrequalification :: GetNextDataInterval (NoiseInterval*& CurrentNoiseInterval, RGTextOutput& text, double minRFU, DataInterval* prevInterval, Boolean print) {

	// Perform convolution with square wave of width WindowWidth until hit peak (and
	// beyond), to capture whole data interval.  Return DataInterval with center at 
	// convolution peak and left and right end points determined by WindowWidth.  Norm
	// of convolution is value of convolution at local peak.  Convolution is performed
	// right to left.

	double PeakHeight;
	double PeakMass;
	int PeakRight;
	int PeakLeft;
	int PeakCenter;
	DataInterval* dataInterval;
	Endl endLine;
	int currentMode;
	double currentMaxAtMode = -DOUBLEMAX;
	double peakValueLeft;

	int nMinima;
	int localMin;
	double localMinValue;
	int secondaryStart;
	int secondaryEnd;
	double secondaryMax;
	double temp;
	int currentSecondaryMode;
	int halfWindow = WindowWidth / 2;
	int localMinSamplesForRegression = TracePrequalification::GetMinSamplesForSlopeRegression ();

	if (halfWindow < localMinSamplesForRegression)
		halfWindow = localMinSamplesForRegression;

	if (CurrentIndex <= 0) {

		CurrentNoiseInterval = NULL;
		return NULL;
	}
	
	int SearchMode = LookingForMax;

//	if ((CurrentIndex >= 1) && (Data->Value (CurrentIndex - 1) < Data->Value (CurrentIndex)))
//		SearchMode = LookingForMin;

	int LocationOfLastMin;
	int LocationOfLastMax = -1;
	int LocationOfBeginConstant = -1;
	int CurrentEndNoise = CurrentIndex;

	CurrentNoiseInterval = new NoiseInterval (Data->Value (CurrentIndex), CurrentIndex);
	CurrentNoiseInterval->SetModeAscending ();

	double newL;
	double newR;
	double newCenter;
	double minRFUApproximate = 0.90 * minRFU;

	double localLowHeightThreshold = TracePrequalification::GetLowHeightThreshold ();
	double localLowSlopeThreshold = TracePrequalification::GetLowSlopeThreshold ();
	double maxSlope = 0.0;
	double currentSlope;
	double absCurrentSlope;
	const double* dataArray = Data->GetData ();
	double delt = Data->GetSampleSpacing ();
	int halfHalfWindow = halfWindow / 2;
	int halfWindow1 = halfWindow - 1;
	bool slopeExceededThresholdOnDescent = false;
	bool slopeBelowThresholdOnFinalDescent = false;
	bool convolutionReversal = false;
	bool peakValueNegative = false;
	bool peakValueBelowThreshold = false;
	bool slopeReversal = false;
	double slopeThreshold;
	bool foundLocalMin;
	int prevPeakLeft;
	double rightLocalMin;

	double temp1;
//	double temp2;
	double slope1;
//	double slope2;

	if (localLowHeightThreshold < 0.0)
		localLowHeightThreshold = 0.0;

	if (localLowSlopeThreshold < 0.0)
		localLowSlopeThreshold = 0.0;

	//
	// Add code here to mark possible right end of peak (it will move as we move)
	//

	LocationOfLastMin = CurrentIndex;

	while (CurrentIndex > 0) {

		newL = Data->Value (WindowLeft);
		newR = Data->Value (WindowRight);
		newCenter = Data->Value (CurrentIndex);
		NewConvolution = CurrentConvolution + 0.5 * (OldLeft + newL - OldRight - newR);
		CumulativeNorm += newCenter * newCenter;

		if (newCenter > currentMaxAtMode) {

			currentMaxAtMode = newCenter;
			currentMode = CurrentIndex;
		}

		currentSlope = LinearSlopeRegression (dataArray + CurrentIndex - halfHalfWindow, delt, halfWindow1);
		absCurrentSlope = fabs (currentSlope);

		if (absCurrentSlope > maxSlope)
			maxSlope = absCurrentSlope;

		//
		// Now test for maximum at CurrentConvolution; if not perform another iteration; if
		// so, start looking for relative minimum to bracket peak unless too low, in which case
		// add to noise and continue; in any case, bump WindowLeft, etc and Convolution values
		// to get ready for next try.
		//

		switch (SearchMode) {

			case LookingForMax:

				// It may be noise...add to CurrentNoiseInterval and we'll judge later
				CurrentNoiseInterval->AddPoint (newCenter, CurrentIndex);

				// Test for new value < 0 and if so, reset NewConvolution to zero (?) and break

				if (NewConvolution < CurrentConvolution) {

					//
					// Found one!  Test to see if it's above NoiseThreshold.  If not, add to noise
					// and continue.  If so, start to look for next minimum to add to DataInterval
					//

					if ((CurrentConvolution < NoiseThreshold) && (currentMaxAtMode < minRFUApproximate)) {

						// The max is noise; reset CurrentNoiseInterval to descending mode
						CurrentNoiseInterval->SetModeDescending ();
						LocationOfLastMax = CurrentIndex;
						LocationOfBeginConstant = -1;
						SearchMode = LookingForMin;
					}

					else {

						// It's a real maximum.  This is it, boys!  We've found another peak!
						PeakRight = LocationOfLastMin;

						// Since we have found a real max, no point in collecting more noise data
						CurrentNoiseInterval->EndNoiseInterval ();

						if (LocationOfBeginConstant > 0)
							PeakCenter = (LocationOfBeginConstant + CurrentIndex) / 2;

						else
							PeakCenter = CurrentIndex;

						PeakHeight = Data->Value (PeakCenter);
						PeakMass = CurrentConvolution;
						LocationOfLastMax = PeakCenter;
						LocationOfBeginConstant = -1;
						
						// Now set up to look for next local minimum

						SearchMode = LookingForFinalMin;
						slopeExceededThresholdOnDescent = false;
					}
				}

				else if (NewConvolution == CurrentConvolution) {

					if (LocationOfBeginConstant < 0)
						LocationOfBeginConstant = CurrentIndex;
				}

				else {

					// Not at a maximum yet...keep looking
					LocationOfBeginConstant = -1;
				}

				break;

			case LookingForMin:

				CurrentNoiseInterval->AddPoint (newCenter, CurrentIndex);

				// Test if new value < 0 and if so, reset NewConvolution to 0 (?) and break...or maybe no change...?

				if (NewConvolution > CurrentConvolution) {

					//
					// Found one!  It must be below NoiseThreshold, or previous maximum would have been
					// above, too.  In that case we would have been LookingForFinalMin!
					//

					// The min is noise; reset CurrentNoiseInterval to ascending mode
					CurrentNoiseInterval->SetModeAscending ();

					LocationOfLastMin = CurrentIndex;
					LocationOfBeginConstant = -1;
					SearchMode = LookingForMax;
					currentMaxAtMode = newCenter;
				}

				else if (NewConvolution == CurrentConvolution) {

					if (LocationOfBeginConstant < 0)
						LocationOfBeginConstant = CurrentIndex;
				}

				else {

					// Not at a minimum yet...keep looking
					LocationOfBeginConstant = -1;
				}

				break;

			case LookingForFinalMin:

				// Test if new value < 0 and if so, declare Final Min found!
				peakValueLeft = Data->Value (CurrentIndex);
//				currentSlope = LinearSlopeRegression (dataArray + CurrentIndex - halfHalfWindow, delt, halfWindow1);  // Use PeakLeft???
//				absCurrentSlope = abs (currentSlope);

//				if ((currentSlope > 0.0) && (currentSlope > maxSlope))
//					maxSlope = currentSlope;

				if (!slopeExceededThresholdOnDescent) {

					if ((currentSlope > 0.0) && (absCurrentSlope >= 3.0 * localLowSlopeThreshold * maxSlope)) {

						slopeExceededThresholdOnDescent = true;
						slopeBelowThresholdOnFinalDescent = false;
					}
				}

				else if (absCurrentSlope <= localLowSlopeThreshold * maxSlope)
					slopeBelowThresholdOnFinalDescent = true;

				else
					slopeBelowThresholdOnFinalDescent = false;

				convolutionReversal = (NewConvolution > CurrentConvolution);
				peakValueNegative = (peakValueLeft < 0.0);
				peakValueBelowThreshold = (peakValueLeft <= localLowHeightThreshold * currentMaxAtMode);
				slopeReversal = (currentSlope <= 0.0);

				if (CurrentIndex >= 1)
					foundLocalMin = (peakValueLeft <= Data->Value (CurrentIndex - 1));

				else
					foundLocalMin = false;

				if (convolutionReversal || peakValueNegative || slopeBelowThresholdOnFinalDescent || peakValueBelowThreshold || slopeReversal || foundLocalMin) {

					//
					// Found it!  It could be above or below NoiseThreshold.  If above, the very
					// next peak will be a data peak.  Either way, don't add to CurrentNoiseInterval.
					// If it's below, it will be added next time.  If above, there will be an
					// "empty" NoiseInterval, which will tell that there is a problem between these 2
					// peaks, this and the next one.
					//
					
					LocationOfLastMin = CurrentIndex;
					double maxThreshold = localLowHeightThreshold * currentMaxAtMode;
					int lastIndex = PeakRight;
					int ii;

//					if (peakValueNegative || slopeReversal)
//						PeakLeft = CurrentIndex + 1;

//					else
						PeakLeft = CurrentIndex;

					/*nMinima = FindInteriorLocalMinimum (PeakLeft, PeakRight, localMin, localMinValue);

					if (currentMode < localMin) {

						currentSecondaryMode = secondaryStart = localMin;
						secondaryEnd = PeakRight;
					}

					else {

						currentSecondaryMode = secondaryStart = PeakLeft;
						secondaryEnd = localMin;
					}

					secondaryMax = -DOUBLEMAX;

					for (int ii=secondaryStart; ii<=secondaryEnd; ii++) {

						temp = Data->Value (ii);

						if (temp > secondaryMax) {

							secondaryMax = temp;
							currentSecondaryMode = ii;
						}
					}*/

					lastIndex = PeakLeft;
					slopeThreshold = localLowSlopeThreshold * maxSlope;

					for (ii=PeakLeft; ii < currentMode; ii++) {

						temp = Data->Value (ii);
						temp1 = Data->Value (ii - 1);
	//					temp2 = Data->Value (ii - 2);
						slope1 = (temp - temp1) / delt;
	//					slope2 = 0.5 * (temp - temp2) / delt;

						if (slope1 <= 0.0)
							lastIndex = ii + 1;

						else if (fabs (slope1) <= slopeThreshold)
							lastIndex = ii;

	//					else if ((slope2 <= 0.0) || (abs (slope2) <= slopeThreshold))
	//						lastIndex = ii + 1;

						else
							break;
					}

					PeakLeft = lastIndex;
					lastIndex = PeakRight;

					/*if ((nMinima > 0) && (currentMode < localMin)) {

						maxThreshold = localLowHeightThreshold * secondaryMax;
					}*/

					if (prevInterval != NULL) {

						rightLocalMin = currentMaxAtMode;
						prevPeakLeft = prevInterval->GetLeft ();

						for (ii=PeakRight; ii<prevPeakLeft; ii++) {

							temp = Data->Value (ii);

							if (temp < rightLocalMin) {

								rightLocalMin = temp;
								lastIndex = ii;
							}

							else
								break;
						}

						PeakRight = lastIndex;
					}

					for (int i=PeakRight; i>currentMode; i--) {

						temp = Data->Value (i);
						temp1 = Data->Value (i + 1);
//						temp2 = Data->Value (i + 2);
						slope1 = (temp1 - temp) / delt;
//						slope2 = 0.5 * (temp2 - temp) / delt;

						if (temp < maxThreshold)
							lastIndex = i;

						else if ((slope1 >= 0.0) || (fabs (slope1) <= slopeThreshold))
							lastIndex = i;

//						else if ((slope2 >= 0.0) || (fabs (slope2) <= slopeThreshold))
//							lastIndex = i - 1;

						else
							break;
					}
					
					PeakRight = lastIndex;

					nMinima = FindInteriorLocalMinimum (PeakLeft, PeakRight, localMin, localMinValue);

					if (currentMode < localMin) {

						currentSecondaryMode = secondaryStart = localMin;
						secondaryEnd = PeakRight;
					}

					else {

						currentSecondaryMode = secondaryStart = PeakLeft;
						secondaryEnd = localMin;
					}

					secondaryMax = -DOUBLEMAX;

					for (ii=secondaryStart; ii<=secondaryEnd; ii++) {

						temp = Data->Value (ii);

						if (temp > secondaryMax) {

							secondaryMax = temp;
							currentSecondaryMode = ii;
						}
					}

					dataInterval = new DataInterval (PeakLeft, currentMode, PeakRight);
					dataInterval->SetHeight (PeakHeight);
					dataInterval->SetMass (PeakMass);
					dataInterval->SetLeftMinimum (Data->Value (PeakLeft));
					dataInterval->SetRightMinimum (Data->Value (PeakRight));
					dataInterval->SetMode (currentMode);
					dataInterval->SetMaxAtMode (currentMaxAtMode);

					
					dataInterval->SetNumberOfMinima (nMinima);
					dataInterval->SetLocalMinimum (localMin);
					dataInterval->SetLocalMinValue (localMinValue);

					dataInterval->SetSecondaryMode (currentSecondaryMode);
					dataInterval->SetMaxAtSecondaryMode (secondaryMax);


//					if ((currentMode == PeakLeft) || (currentMode == PeakRight)) {

						// Invent a test for a pure minimum:  maybe this is enough of a test, but, for now
						// we'll just omit it until and if we need it.
//					}

					PreviousConvolution = CurrentConvolution;
					CurrentConvolution = NewConvolution;
					CurrentIndex--;
					OldLeft = newL;
					OldRight = newR;
					WindowLeft--;
					WindowRight--;
					LocationOfBeginConstant = -1;

					if (print) {

						text << "Found data interval with left = " << PeakLeft << ", right = " << PeakRight << " and center = " << PeakCenter << endLine;
						text << "     Value at center = " << Data->Value (PeakCenter) << endLine;
						text << "     Convolution = " << PeakMass << endLine;
					}

					if ((slopeReversal) || (PeakLeft > CurrentIndex)) {

						dataInterval->SetFixedLeftTrue ();
//						return dataInterval;
					}

					if (convolutionReversal)
						return dataInterval;

					SearchMode = UnravelingFinalCurve;
					break;
				}

				else if (NewConvolution == CurrentConvolution) {

					if (LocationOfBeginConstant < 0)
						LocationOfBeginConstant = CurrentIndex;
				}

				else {

					// Not at a minimum yet...keep looking
					LocationOfBeginConstant = -1;
				}

				break;

			case UnravelingFinalCurve:

				peakValueLeft = Data->Value (CurrentIndex);
				convolutionReversal = (NewConvolution > CurrentConvolution);
				slopeReversal = (currentSlope <= 0.0);
				peakValueNegative = (peakValueLeft < 0.0);

				if (CurrentIndex >= 1)
					foundLocalMin = (peakValueLeft <= Data->Value (CurrentIndex - 1));

				else
					foundLocalMin = false;

				if (convolutionReversal || slopeReversal || foundLocalMin || peakValueNegative) {

					PreviousConvolution = CurrentConvolution;
					CurrentConvolution = NewConvolution;
					CurrentIndex--;
					OldLeft = newL;
					OldRight = newR;
					WindowLeft--;
					WindowRight--;
					LocationOfBeginConstant = -1;
					return dataInterval;
				}

				break;
		}

		PreviousConvolution = CurrentConvolution;
		CurrentConvolution = NewConvolution;
		CurrentIndex--;
		OldLeft = newL;
		OldRight = newR;
		WindowLeft--;
		WindowRight--;
	}

	if ((SearchMode == LookingForFinalMin) || (SearchMode == UnravelingFinalCurve)) {

		LocationOfLastMin = CurrentIndex;
					
		PeakLeft = CurrentIndex;
		dataInterval = new DataInterval (PeakLeft, PeakCenter, PeakRight);
		dataInterval->SetHeight (PeakHeight);
		dataInterval->SetMass (PeakMass);
		dataInterval->SetNumberOfMinima (0);
		dataInterval->SetMode (currentMode);
		dataInterval->SetMaxAtMode (currentMaxAtMode);

		PreviousConvolution = CurrentConvolution;
		CurrentConvolution = NewConvolution;
		CurrentIndex--;
		OldLeft = newL;
		OldRight = newR;
		WindowLeft--;
		WindowRight--;
		LocationOfBeginConstant = -1;

		if (print) {

			text << "Found data interval with left = " << PeakLeft << ", right = " << PeakRight << " and center = " << PeakCenter << endLine;
			text << "     Value at center = " << Data->Value (PeakCenter) << endLine;
			text << "     Convolution = " << PeakMass << endLine;
		}

		return dataInterval;
	}

	return NULL;
}


DataInterval* STRTracePrequalification :: GetNextDataIntervalWithPrecomputedConvolution (NoiseInterval*& CurrentNoiseInterval, RGTextOutput& text, double minRFU, DataInterval* prevInterval, Boolean print) {

	// Perform convolution with square wave of width WindowWidth until hit peak (and
	// beyond), to capture whole data interval.  Return DataInterval with center at 
	// convolution peak and left and right end points determined by WindowWidth.  Norm
	// of convolution is value of convolution at local peak.  Convolution is performed
	// right to left.

	double PeakHeight;
	double PeakMass;
	int PeakRight;
	int PeakLeft;
	int PeakCenter;
	DataInterval* dataInterval;
	Endl endLine;
	int currentMode;
	double currentMaxAtMode = -DOUBLEMAX;
	double peakValueLeft;
	double peakValueRight;

	int nMinima;
	int localMin;
	double localMinValue;
	int secondaryStart;
	int secondaryEnd;
	double secondaryMax;
	double temp;
	int currentSecondaryMode;
//	int halfWindow = WindowWidth / 2;
//	int localMinSamplesForRegression = TracePrequalification::GetMinSamplesForSlopeRegression ();

//	if (halfWindow < localMinSamplesForRegression)
//		halfWindow = localMinSamplesForRegression;

	if (CurrentIndex <= 0) {

		CurrentNoiseInterval = NULL;
		return NULL;
	}
	
	PeakRight = CurrentIndex;
	peakValueRight = Data->Value (PeakRight);
	int SearchMode = LookingForMax;

//	if ((CurrentIndex >= 1) && (Data->Value (CurrentIndex - 1) < Data->Value (CurrentIndex)))
//		SearchMode = LookingForMin;

//	if (mConvolution [CurrentIndex] <= mConvolution [CurrentIndex + 1])
//		SearchMode = LookingForMin;



	int LocationOfLastMin;
	int LocationOfLastMax = -1;
	int LocationOfBeginConstant = -1;
//	int CurrentEndNoise = CurrentIndex;

	CurrentNoiseInterval = new NoiseInterval (Data->Value (CurrentIndex), CurrentIndex);
	CurrentNoiseInterval->SetModeAscending ();

//	double newL;
//	double newR;
	double newCenter;
	double minRFUApproximate = 0.90 * minRFU;

	double localLowHeightThreshold = TracePrequalification::GetLowHeightThreshold ();
	double localLowSlopeThreshold = TracePrequalification::GetLowSlopeThreshold ();
	double maxSlope = 0.0;
	double currentSlope;
	double absCurrentSlope;
//	const double* dataArray = Data->GetData ();
	double delt = Data->GetSampleSpacing ();
//	int halfHalfWindow = halfWindow / 2;
//	int halfWindow1 = halfWindow - 1;
	bool slopeExceededThresholdOnDescent = false;
	bool slopeBelowThresholdOnFinalDescent = false;
	bool convolutionReversal = false;
	bool peakValueNegative = false;
	bool peakValueBelowThreshold = false;
	bool slopeReversal = false;
	double slopeThreshold;
	bool foundLocalMin;
	int prevPeakLeft;
	double rightLocalMin;

	double temp1;
//	double temp2;
	double slope1;
//	double slope2;
	int nPoints = 0;

	if (localLowHeightThreshold < 0.0)
		localLowHeightThreshold = 0.0;

	if (localLowSlopeThreshold < 0.0)
		localLowSlopeThreshold = 0.0;

	//
	// Add code here to mark possible right end of peak (it will move as we move)
	//

	LocationOfLastMin = CurrentIndex;

	while (CurrentIndex > 0) {

//		newL = Data->Value (WindowLeft);
//		newR = Data->Value (WindowRight);
		PeakLeft = CurrentIndex;
		newCenter = peakValueLeft = Data->Value (CurrentIndex);
//		NewConvolution = CurrentConvolution + 0.5 * (OldLeft + newL - OldRight - newR);
		NewConvolution = mConvolution [CurrentIndex];
		CurrentConvolution = mConvolution [CurrentIndex + 1];

//		CumulativeNorm += newCenter * newCenter;

		if (newCenter > currentMaxAtMode) {

			currentMaxAtMode = newCenter;
			currentMode = CurrentIndex;
		}

//		currentSlope = LinearSlopeRegression (dataArray + CurrentIndex - halfHalfWindow, delt, halfWindow1);
		currentSlope = mSlopeFits [CurrentIndex];
		absCurrentSlope = fabs (currentSlope);

		if (absCurrentSlope > maxSlope)
			maxSlope = absCurrentSlope;

		//
		// Now test for maximum at CurrentConvolution; if not perform another iteration; if
		// so, start looking for relative minimum to bracket peak unless too low, in which case
		// add to noise and continue; in any case, bump WindowLeft, etc and Convolution values
		// to get ready for next try.
		//

		switch (SearchMode) {

			case LookingForMax:

				// It may be noise...add to CurrentNoiseInterval and we'll judge later
				CurrentNoiseInterval->AddPoint (newCenter, CurrentIndex);

				// Test for new value < 0 and if so, reset NewConvolution to zero (?) and break

				if ((NewConvolution < CurrentConvolution) && (nPoints >= 2)) {

					//
					// Found one!  Test to see if it's above NoiseThreshold.  If not, add to noise
					// and continue.  If so, start to look for next minimum to add to DataInterval
					//

					if ((CurrentConvolution < NoiseThreshold) && (currentMaxAtMode < minRFUApproximate)) {

						// The max is noise; reset CurrentNoiseInterval to descending mode
						CurrentNoiseInterval->SetModeDescending ();
						LocationOfLastMax = CurrentIndex;
						LocationOfBeginConstant = -1;
						
						SearchMode = LookingForMin;
					}

					else {

						// It's a real maximum.  This is it, boys!  We've found another peak!
						PeakRight = LocationOfLastMin;
						peakValueRight = Data->Value (PeakRight);

						// Since we have found a real max, no point in collecting more noise data
						CurrentNoiseInterval->EndNoiseInterval ();

						if (LocationOfBeginConstant > 0)
							PeakCenter = (LocationOfBeginConstant + CurrentIndex) / 2;

						else
							PeakCenter = CurrentIndex;

						PeakHeight = Data->Value (PeakCenter);
						PeakMass = CurrentConvolution;
						LocationOfLastMax = PeakCenter;
						LocationOfBeginConstant = -1;
						
						// Now set up to look for next local minimum

						SearchMode = LookingForFinalMin;
						slopeExceededThresholdOnDescent = false;
					}
				}

				else if (NewConvolution == CurrentConvolution) {

					if (LocationOfBeginConstant < 0)
						LocationOfBeginConstant = CurrentIndex;
				}

				else {

					// Not at a maximum yet...keep looking
					LocationOfBeginConstant = -1;
				}

				CurrentIndex--;
				break;

			case LookingForMin:

				CurrentNoiseInterval->AddPoint (newCenter, CurrentIndex);

				// Test if new value < 0 and if so, reset NewConvolution to 0 (?) and break...or maybe no change...?

				if (NewConvolution > CurrentConvolution) {

					//
					// Found one!  It must be below NoiseThreshold, or previous maximum would have been
					// above, too.  In that case we would have been LookingForFinalMin!
					//

					// The min is noise; reset CurrentNoiseInterval to ascending mode
					CurrentNoiseInterval->SetModeAscending ();

					LocationOfLastMin = CurrentIndex;
					LocationOfBeginConstant = -1;
					SearchMode = LookingForMax;
					currentMaxAtMode = newCenter;
				}

				else if (NewConvolution == CurrentConvolution) {

					if (LocationOfBeginConstant < 0)
						LocationOfBeginConstant = CurrentIndex;
				}

				else {

					// Not at a minimum yet...keep looking
					LocationOfBeginConstant = -1;
				}

				CurrentIndex--;
				break;

			case LookingForFinalMin:

				// Test if new value < 0 and if so, declare Final Min found!
				peakValueLeft = Data->Value (CurrentIndex);
//				currentSlope = LinearSlopeRegression (dataArray + CurrentIndex - halfHalfWindow, delt, halfWindow1);  // Use PeakLeft???
//				absCurrentSlope = abs (currentSlope);

//				if ((currentSlope > 0.0) && (currentSlope > maxSlope))
//					maxSlope = currentSlope;

				if (!slopeExceededThresholdOnDescent) {

					if ((currentSlope > 0.0) && (absCurrentSlope >= 3.0 * localLowSlopeThreshold * maxSlope)) {

						slopeExceededThresholdOnDescent = true;
						slopeBelowThresholdOnFinalDescent = false;
					}
				}

				else if (absCurrentSlope <= localLowSlopeThreshold * maxSlope)
					slopeBelowThresholdOnFinalDescent = true;

				else
					slopeBelowThresholdOnFinalDescent = false;

				convolutionReversal = (NewConvolution > CurrentConvolution);
				peakValueNegative = (peakValueLeft < 0.0);
				peakValueBelowThreshold = (peakValueLeft <= localLowHeightThreshold * currentMaxAtMode);
				slopeReversal = (currentSlope <= 0.0);

				if (CurrentIndex >= 1)
					foundLocalMin = (peakValueLeft <= Data->Value (CurrentIndex - 1));

				else
					foundLocalMin = false;

				foundLocalMin = false;  // *****!!!! This is a test!!!! *****
				slopeReversal = false;  // *****!!!! This is a test!!!! *****

				if (convolutionReversal || peakValueNegative || slopeBelowThresholdOnFinalDescent || peakValueBelowThreshold || slopeReversal || foundLocalMin) {

					//
					// Found it!  It could be above or below NoiseThreshold.  If above, the very
					// next peak will be a data peak.  Either way, don't add to CurrentNoiseInterval.
					// If it's below, it will be added next time.  If above, there will be an
					// "empty" NoiseInterval, which will tell that there is a problem between these 2
					// peaks, this and the next one.
					//
					
					LocationOfLastMin = CurrentIndex;
					double maxThreshold = localLowHeightThreshold * currentMaxAtMode;
					int lastIndex = PeakRight;
					int ii;

//					if (peakValueNegative || slopeReversal)
//						PeakLeft = CurrentIndex + 1;

//					else
						PeakLeft = CurrentIndex;
						peakValueLeft = Data->Value (PeakLeft);

					/*nMinima = FindInteriorLocalMinimum (PeakLeft, PeakRight, localMin, localMinValue);

					if (currentMode < localMin) {

						currentSecondaryMode = secondaryStart = localMin;
						secondaryEnd = PeakRight;
					}

					else {

						currentSecondaryMode = secondaryStart = PeakLeft;
						secondaryEnd = localMin;
					}

					secondaryMax = -DOUBLEMAX;

					for (int ii=secondaryStart; ii<=secondaryEnd; ii++) {

						temp = Data->Value (ii);

						if (temp > secondaryMax) {

							secondaryMax = temp;
							currentSecondaryMode = ii;
						}
					}*/

					lastIndex = PeakLeft;
					slopeThreshold = localLowSlopeThreshold * maxSlope;

					for (ii=PeakLeft; ii < currentMode; ii++) {

						temp = Data->Value (ii);
						temp1 = Data->Value (ii - 1);
	//					temp2 = Data->Value (ii - 2);
						slope1 = (temp - temp1) / delt;
	//					slope2 = 0.5 * (temp - temp2) / delt;

						if (slope1 <= 0.0)
							lastIndex = ii + 1;

						else if (fabs (slope1) <= slopeThreshold)
							lastIndex = ii;

	//					else if ((slope2 <= 0.0) || (abs (slope2) <= slopeThreshold))
	//						lastIndex = ii + 1;

						else
							break;
					}

					PeakLeft = lastIndex;
					peakValueLeft = Data->Value (PeakLeft);
					lastIndex = PeakRight;

					/*if ((nMinima > 0) && (currentMode < localMin)) {

						maxThreshold = localLowHeightThreshold * secondaryMax;
					}*/

					if (prevInterval != NULL) {

						rightLocalMin = currentMaxAtMode;
						prevPeakLeft = prevInterval->GetLeft ();

						for (ii=PeakRight; ii<prevPeakLeft; ii++) {

							temp = Data->Value (ii);

							if (temp < rightLocalMin) {

								rightLocalMin = temp;
								lastIndex = ii;
							}

							else
								break;
						}

						PeakRight = lastIndex;
						peakValueRight = Data->Value (PeakRight);
					}

					for (int i=PeakRight; i>currentMode; i--) {

						temp = Data->Value (i);
						temp1 = Data->Value (i + 1);
						slope1 = (temp1 - temp) / delt;

						if (temp < maxThreshold)
							lastIndex = i;

						else if ((slope1 >= 0.0) || (fabs (slope1) <= slopeThreshold))
							lastIndex = i;

						else
							break;
					}

					PeakRight = lastIndex;
					peakValueRight = Data->Value (PeakRight);

					/*slopeExceededThresholdOnDescent = false;

					for (int i=currentMode+1; i<=PeakRight; i++) {

						temp = Data->Value (i);
						currentSlope = mSlopeFits [i + 1];
						absCurrentSlope = abs (currentSlope);
						temp1 = Data->Value (i + 1);
						slope1 = (temp1 - temp) / delt;

						if (temp < maxThreshold) {

							lastIndex = i;
							break;
						}

						if (!slopeExceededThresholdOnDescent) {

							if ((currentSlope <= 0.0) && (absCurrentSlope >= 3.0 * slopeThreshold))
								slopeExceededThresholdOnDescent = true;
						}

						else if ((currentSlope >= 0.0) || (absCurrentSlope <= slopeThreshold)) {

							lastIndex = i;
							break;
						}

						else if ((slope1 >= 0.0) || (abs (slope1) <= slopeThreshold)) {

							lastIndex = i;
							break;
						}
					}
					
					PeakRight = lastIndex;*/

					nMinima = FindInteriorLocalMinimum (PeakLeft, PeakRight, localMin, localMinValue);

					if (currentMode < localMin) {

						currentSecondaryMode = secondaryStart = localMin;
						secondaryEnd = PeakRight;
					}

					else {

						currentSecondaryMode = secondaryStart = PeakLeft;
						secondaryEnd = localMin;
					}

					secondaryMax = -DOUBLEMAX;

					for (ii=secondaryStart; ii<=secondaryEnd; ii++) {

						temp = Data->Value (ii);

						if (temp > secondaryMax) {

							secondaryMax = temp;
							currentSecondaryMode = ii;
						}
					}

					dataInterval = new DataInterval (PeakLeft, currentMode, PeakRight);
					dataInterval->SetHeight (PeakHeight);
					dataInterval->SetMass (PeakMass);
					dataInterval->SetLeftMinimum (Data->Value (PeakLeft));
					dataInterval->SetRightMinimum (Data->Value (PeakRight));
					dataInterval->SetMode (currentMode);
					dataInterval->SetMaxAtMode (currentMaxAtMode);

					
					dataInterval->SetNumberOfMinima (nMinima);
					dataInterval->SetLocalMinimum (localMin);
					dataInterval->SetLocalMinValue (localMinValue);

					dataInterval->SetSecondaryMode (currentSecondaryMode);
					dataInterval->SetMaxAtSecondaryMode (secondaryMax);


//					if ((currentMode == PeakLeft) || (currentMode == PeakRight)) {

						// Invent a test for a pure minimum:  maybe this is enough of a test, but, for now
						// we'll just omit it until and if we need it.
//					}

//					PreviousConvolution = CurrentConvolution;
//					CurrentConvolution = NewConvolution;
//					CurrentIndex--;    // ??????????????????
//					OldLeft = newL;
//					OldRight = newR;
//					WindowLeft--;
//					WindowRight--;
					LocationOfBeginConstant = -1;

					if (print) {

						text << "Found data interval with left = " << PeakLeft << ", right = " << PeakRight << " and center = " << PeakCenter << endLine;
						text << "     Value at center = " << Data->Value (PeakCenter) << endLine;
						text << "     Convolution = " << PeakMass << endLine;
					}

					if (foundLocalMin) {

						dataInterval->SetFixedLeftTrue ();
						return dataInterval;
					}

					if ((slopeReversal) || (PeakLeft > CurrentIndex) || convolutionReversal) {

						dataInterval->SetFixedLeftTrue ();
						
						/*lastIndex = CurrentIndex;

						for (ii=CurrentIndex; ii<=PeakLeft; ii++) {

							if (Data->Value (ii) >= Data->Value (ii + 1))
								lastIndex = ii;

							else
								break;
						}

						CurrentIndex = lastIndex;*/
						CurrentIndex--;
						return dataInterval;
					}

//					if (convolutionReversal)
//						return dataInterval;

					SearchMode = UnravelingFinalCurve;
					CurrentIndex--;
					break;
				}

				else if (NewConvolution == CurrentConvolution) {

					if (LocationOfBeginConstant < 0)
						LocationOfBeginConstant = CurrentIndex;

					CurrentIndex--;
				}

				else {

					// Not at a minimum yet...keep looking
					LocationOfBeginConstant = -1;
					CurrentIndex--;
				}

				break;

			case UnravelingFinalCurve:

				peakValueLeft = Data->Value (CurrentIndex);
				convolutionReversal = (NewConvolution > CurrentConvolution);
				slopeReversal = (currentSlope <= 0.0);
				peakValueNegative = (peakValueLeft < 0.0);

				if (CurrentIndex >= 1)
					foundLocalMin = (peakValueLeft <= Data->Value (CurrentIndex - 1));

				else
					foundLocalMin = false;

				if (slopeReversal || foundLocalMin || peakValueNegative) {

					LocationOfBeginConstant = -1;

					if (peakValueNegative)
						CurrentIndex--;

					return dataInterval;
				}

				if (convolutionReversal) {

					CurrentIndex++;
					return dataInterval;
				}

				CurrentIndex--;
				break;
		}

		/*PreviousConvolution = CurrentConvolution;
		CurrentConvolution = NewConvolution;*/

//		CurrentIndex--;    //???????????????????????

		/*OldLeft = newL;
		OldRight = newR;
		WindowLeft--;
		WindowRight--;*/

		nPoints++;
	}

	if ((SearchMode == LookingForFinalMin) || (SearchMode == UnravelingFinalCurve)) {

		LocationOfLastMin = CurrentIndex;
					
		PeakLeft = CurrentIndex;
		peakValueLeft = Data->Value (PeakLeft);
		dataInterval = new DataInterval (PeakLeft, PeakCenter, PeakRight);
		dataInterval->SetHeight (PeakHeight);
		dataInterval->SetMass (PeakMass);
		dataInterval->SetNumberOfMinima (0);
		dataInterval->SetMode (currentMode);
		dataInterval->SetMaxAtMode (currentMaxAtMode);
		dataInterval->SetLeftMinimum (peakValueLeft);
		dataInterval->SetRightMinimum (Data->Value (PeakRight));

		/*PreviousConvolution = CurrentConvolution;
		CurrentConvolution = NewConvolution;*/

		CurrentIndex--;

		/*OldLeft = newL;
		OldRight = newR;
		WindowLeft--;
		WindowRight--;*/
		LocationOfBeginConstant = -1;

		if (print) {

			text << "Found data interval with left = " << PeakLeft << ", right = " << PeakRight << " and center = " << PeakCenter << endLine;
			text << "     Value at center = " << Data->Value (PeakCenter) << endLine;
			text << "     Convolution = " << PeakMass << endLine;
		}

		return dataInterval;
	}

	return NULL;
}



double STRTracePrequalification :: GetNorm () const {

	return CumulativeNorm;
}


int STRTracePrequalification :: FindInteriorLocalMinimum (int left, int right, int& localMin, double& localMinValue) {

	int nMinima = 0;
	int current = left + 3;
	int endPt = right - 3;
	int next = current + 1;
	int prev = current - 1;
	double prevValue = Data->Value (prev);
	double nextValue = Data->Value (next);
	double currValue = Data->Value (current);
	double currMinValue = 0.0;
	int currMin = 0;
	double mass = 0.0;
	double centerOfMass = 0.0;
	double temp;

	for (int i=left; i<=right; i++) {

		temp = Data->Value (i);
		mass += temp;
		centerOfMass += (double)i * temp;
	}

	if (mass == 0.0) {

		localMin = 0;
		localMinValue = 0.0;
		return 0;
	}

	centerOfMass = centerOfMass / mass;

	if ((centerOfMass < left) || (centerOfMass > right)) {

		localMin = 0;
		localMinValue = 0.0;
		return 0;
	}

	double currentDistanceToCenter = 0.0;

	while (current < endPt) {

		if ((currValue < prevValue) && (currValue < nextValue)) {

			temp = fabs ((double)current - centerOfMass);

			if (nMinima == 0) {

				currMinValue = currValue;
				currMin = current;
				currentDistanceToCenter = temp;
			}

			else if (temp < currentDistanceToCenter) {

				currMinValue = currValue;
				currMin = current;
				currentDistanceToCenter = temp;
			}

			nMinima++;
		}

		current++;
		next++;
		prev++;
		prevValue = currValue;
		currValue = nextValue;
		nextValue = Data->Value (next);
	}

	localMin = currMin;
	localMinValue = currMinValue;
	return nMinima;
}


size_t STRTracePrequalification :: StoreSize () const {

	size_t size = TracePrequalification::StoreSize ();
	size += sizeof (WindowWidth);
	return size;
}


void STRTracePrequalification :: RestoreAll (RGFile& f) {

	TracePrequalification::RestoreAll (f);
	f.Read (WindowWidth);
}


void STRTracePrequalification :: RestoreAll (RGVInStream& f) {

	TracePrequalification::RestoreAll (f);
	f >> WindowWidth;
}


void STRTracePrequalification :: SaveAll (RGFile& f) const {

	TracePrequalification::SaveAll (f);
	f.Write (WindowWidth);
}


void STRTracePrequalification :: SaveAll (RGVOutStream& f) const {

	TracePrequalification::SaveAll (f);
	f << WindowWidth;
}

