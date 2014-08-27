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
*  FileName: DataInterval.h
*  Author:   Robert Goor
*
*/
//
//  class DataInterval, which summarizes primitive information about an interval with significant data to interpret.
//  class NoiseInterval, which summarizes data about noise measurements in an interval:  assumes right to left scan
//

#ifndef _DATAINTERVAL_H_
#define _DATAINTERVAL_H_

#include "rgpersist.h"

const int _DATAINTERVAL_ = 1007;
const int _NOISEINTERVAL_ = 1008;

class RGFile;
class RGVInStream;
class RGVOutStream;
class DataSignal;

PERSISTENT_PREDECLARATION (DataInterval)
PERSISTENT_PREDECLARATION (NoiseInterval)

class DataInterval : public RGPersistent {

PERSISTENT_DECLARATION (DataInterval)

public:
	DataInterval ();
	DataInterval (int left, int center, int right);
	virtual ~DataInterval ();

	DataInterval* Split (DataInterval*& secondInterval) const;

	void SetMass (double mass) { Mass = mass; }
	void SetHeight (double height) { Height = height; }
	void SetMode (int mode) { Mode = mode; }
	void SetFixedLeftTrue () { FixedLeft = TRUE; }
	void SetFixedRightTrue () { FixedRight = TRUE; }
	void SetLeftMinimum (double minimum) { LeftMinimum = minimum; }
	void SetRightMinimum (double minimum) { RightMinimum = minimum; }
	void SetMaxAtMode (double maximum) { MaxAtMode = maximum; }
	void SetMaxAtSecondaryMode (double maximum) { mMaxAtSecondaryMode = maximum; }
	void SetPureMinimumTrue () { mPureMinimum = TRUE; }

	void SetNumberOfMinima (int minima) { mNumberOfMinima = minima; }
	void SetLocalMinimum (int localMin) { mLocalMinimum = localMin; }
	void SetLocalMinValue (double val) { mLocalMinValue = val; }
	void SetSecondaryMode (int mode) { mSecondaryMode = mode; }

	int GetLeft () const { return Left; }
	int GetRight () const { return Right; }
	int GetCenter () const { return Center; }
	double GetHeight () const { return Height; }
	double GetMass () const { return Mass; }
	double GetMode () const { return Mode; }
	double GetLeftMinimum () const { return LeftMinimum; }
	double GetRightMinimum () const { return RightMinimum; }
	double GetMaxAtMode () const { return MaxAtMode; }
	double GetMaxAtSecondaryMode () const { return mMaxAtSecondaryMode; }

	Boolean HasAFixedEndPoint () const { return FixedLeft || FixedRight; }
	Boolean HasFixedLeftEndPoint () const { return FixedLeft; }
	Boolean HasFixedRightEndPoint () const { return FixedRight; }
	Boolean IsPureMinimum () const { return mPureMinimum; }

	int GetNumberOfMinima () const { return mNumberOfMinima; }
	int GetLocalMinimum () const { return mLocalMinimum; }
	double GetLocalMinValue () const { return mLocalMinValue; }

	void ReassessRelativeMinimaGivenNoise (double noiseLevel, DataSignal* data);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	int Left;
	int Right;
	int Center;
	double Mass;
	double Height;
	int Mode;
	double MaxAtMode;
	Boolean FixedLeft;
	Boolean FixedRight;
	double LeftMinimum;
	double RightMinimum;
	int mNumberOfMinima;
	int mLocalMinimum;
	double mLocalMinValue;
	int mSecondaryMode;
	double mMaxAtSecondaryMode;
	Boolean mPureMinimum;
};


class NoiseInterval : public RGPersistent {

PERSISTENT_DECLARATION (NoiseInterval)

public:
	NoiseInterval ();
	NoiseInterval (double value, int right);
	virtual ~NoiseInterval ();

	void SetLeft (int left) { Left = left; }

	void AddPoint (double value, int location);
	void SetModeAscending ();
	void SetModeDescending ();
	void EndNoiseInterval ();

	double GetMaximum () const { return Maximum; }
	double GetMinimum () const { return Minimum; }
	double GetAverage () const { return Average / NumberOfSamples; }
	int GetNumberOfTurningPoints () const { return NumberOfMaxima + NumberOfMinima; }
	int GetNumberOfSamples () const { return NumberOfSamples; }
	Boolean HasSufficientSamples (int minimum) const { return (NumberOfSamples >= minimum); }

	double GetNorm2 (double spacing) const;
	double GetNorm2Rate () const;

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	int Left;
	int Right;
	double Maximum;
	double Minimum;
	double Average;
	int NumberOfMaxima;
	int NumberOfMinima;
	int NumberOfSamples;
	double Norm2;

	double AscendingNorm2;
	double AscendingAverage;
	double AscendingMaximum;
	double AscendingMinimum;
	int AscendingNumberOfMinima;
	int AscendingNumberOfMaxima;
	int AscendingLeft;
	int AscendingNumberOfSamples;

	double CurrentValue;
	double PreviousValue;

	Boolean AscendingMode;
	Boolean RawAscendingMode;
	Boolean EndOfInterval;
};


#endif  /*  _DATAINTERVAL_H_  */

