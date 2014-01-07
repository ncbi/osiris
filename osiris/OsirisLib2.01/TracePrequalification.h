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
*  FileName: TracePrequalification.h
*  Author:   Robert Goor
*
*/
//
//  class TracePrequalification is an abstract base class of classes used to prequalify part of a DataSignal for 
//  more in-depth characteristic search.  Specialized subclasses can define specific strategies for different types 
//  of forensic analysis.  TracePrequalification and subclasses are subclasses of RGPersistent.
//

#ifndef _TRACEPREQUALIFICATION_H_
#define _TRACEPREQUALIFICATION_H_

#include "rgpersist.h"

class RGFile;
class RGVInStream;
class RGVOutStream;
class DataInterval;
class NoiseInterval;
class DataSignal;
class RGTextOutput;

//
//    Required Parameters:
//       TracePrequalification::windowWidth gives number of samples of width of square wave for convolution; default = 20
//       TracePrequalification::noiseThreshold gives threshold on post-convolution peaks to consider them noise; default = 400.0;
//


const int _STRTRACEPREQUALIFICATION_ = 1006;

PERSISTENT_PREDECLARATION (STRTracePrequalification)

double LinearSlopeRegression (const double* y, double delt, int N1);


class TracePrequalification : public RGPersistent {

ABSTRACT_DECLARATION (TracePrequalification)

public:
	TracePrequalification () : RGPersistent () {}
	virtual ~TracePrequalification () = 0;

	virtual void ResetSearch (const DataSignal* data, int size) = 0;
	virtual DataInterval* GetNextDataInterval (NoiseInterval*& noiseInterval, RGTextOutput& text, double minRFU, DataInterval* prevInterval, Boolean print = TRUE) = 0;
	virtual DataInterval* GetNextDataIntervalWithPrecomputedConvolution (NoiseInterval*& noiseInterval, RGTextOutput& text, double minRFU, DataInterval* prevInterval, Boolean print = TRUE) = 0;
	virtual double GetNorm () const = 0;

	static void SetNoiseThreshold (double threshold) { noiseThreshold = threshold; }
	static double GetNoiseThreshold () { return noiseThreshold; }

	static void SetDefaultNoiseThreshold (double threshold) { defaultNoiseThreshold = threshold; }
	static double GetDefaultNoiseThreshold () { return defaultNoiseThreshold; }

	static void SetWindowWidth (int width);
	static int GetWindowWidth () { return windowWidth; }

	static void SetDefaultWindowWidth (int width);
	static int GetDefaultWindowWidth () { return defaultWindowWidth; }

	static void SetLowHeightThreshold (double height) { lowHeightThreshold = height; }
	static double GetLowHeightThreshold () { return lowHeightThreshold; }

	static void SetLowSlopeThreshold (double slope) { lowSlopeThreshold = slope; }
	static double GetLowSlopeThreshold () { return lowSlopeThreshold; }

	static void SetMinSamplesForSlopeRegression (int samples);
	static int GetMinSamplesForSlopeRegression () { return minSamplesForSlopeRegression; }

private:
	static double noiseThreshold;
	static int windowWidth;
	static double lowHeightThreshold;
	static double lowSlopeThreshold;
	static int minSamplesForSlopeRegression;

	static double defaultNoiseThreshold;
	static int defaultWindowWidth;
};


class STRTracePrequalification : public TracePrequalification {

PERSISTENT_DECLARATION (STRTracePrequalification)

public:
	STRTracePrequalification ();
	virtual ~STRTracePrequalification ();

	virtual void ResetSearch (const DataSignal* data, int size);
	virtual DataInterval* GetNextDataInterval (NoiseInterval*& noiseInterval, RGTextOutput& text, double minRFU, DataInterval* prevInterval, Boolean print = TRUE);
	virtual DataInterval* GetNextDataIntervalWithPrecomputedConvolution (NoiseInterval*& noiseInterval, RGTextOutput& text, double minRFU, DataInterval* prevInterval, Boolean print = TRUE);
	virtual double GetNorm () const;
	virtual int FindInteriorLocalMinimum (int left, int right, int& localMin, double& localMinValue);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	int WindowWidth;
	double CumulativeNorm;
	const DataSignal* Data;
	int MaxIndex;
	double Spacing;
	double NoiseThreshold;
	int CurrentIndex;
	double CurrentConvolution;
	double PreviousConvolution;
	double NewConvolution;
	int HalfWindow;
	int WindowLeft;
	int WindowRight;
	double OldLeft;
	double OldRight;
	double LastMinimum;
	double* mConvolution;
	double* mSlopeFits;
};

#endif  /*  _TRACEPREQUALIFICATION_H_  */

