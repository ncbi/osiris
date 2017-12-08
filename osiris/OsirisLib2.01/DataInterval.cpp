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
*  FileName: DataInterval.cpp
*  Author:   Robert Goor
*
*/
//
//  class DataInterval, which summarizes primitive information about an interval with significant data to interpret.
//  class NoiseInterval, which summarizes data about noise measurements in an interval:  assumes right to left scan
//

#include "DataInterval.h"
#include "DataSignal.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgdefs.h"
#include <list>

PERSISTENT_DEFINITION(DataInterval, _DATAINTERVAL_, "DataInterval")
PERSISTENT_DEFINITION(NoiseInterval, _NOISEINTERVAL_, "NoiseInterval")



DataInterval :: DataInterval () : RGPersistent (), Left (0), Right (0), Center (0), Mass (0.0), Height (0.0),
	Mode (0), MaxAtMode (-DOUBLEMAX), FixedLeft (FALSE), FixedRight (FALSE), LeftMinimum (0.0), RightMinimum (0.0), mNumberOfMinima (0), 
	mLocalMinimum (0), mLocalMinValue (0.0), mSecondaryMode (0), mMaxAtSecondaryMode (0.0), 
	mPureMinimum (FALSE), mValueLeftOfMax (0.0), mValueRightOfMax (0.0), mOKtoTestForSpike (FALSE), mSecondaryModeSet (false) {}


DataInterval :: DataInterval (int left, int center, int right) : RGPersistent (), Left (left), Right (right), 
	Center (center), Mass (0.0), Height (0.0), Mode (0), MaxAtMode (-DOUBLEMAX), FixedLeft (FALSE), FixedRight (FALSE), LeftMinimum (0.0), RightMinimum (0.0), mNumberOfMinima (0), 
	mLocalMinimum (0), mLocalMinValue (0.0), mSecondaryMode (0), mMaxAtSecondaryMode (0.0), mPureMinimum (FALSE), mValueLeftOfMax (0.0), mValueRightOfMax (0.0), mOKtoTestForSpike (FALSE), mSecondaryModeSet (false) {}

DataInterval :: DataInterval (const DataInterval& di) : RGPersistent (di), Left (di.Left), Right (di.Right), 
	Center (di.Center), Mass (di.Mass), Height (di.Height), Mode (di.Mode), MaxAtMode (di.MaxAtMode), FixedLeft (di.FixedLeft), FixedRight (di.FixedRight), LeftMinimum (di.LeftMinimum), RightMinimum (di.RightMinimum), mNumberOfMinima (di.mNumberOfMinima), 
	mLocalMinimum (di.mLocalMinimum), mLocalMinValue (di.mLocalMinValue), mSecondaryMode (di.mSecondaryMode), mMaxAtSecondaryMode (di.mMaxAtSecondaryMode), mPureMinimum (di.mPureMinimum), mValueLeftOfMax (di.mValueLeftOfMax), 
	mValueRightOfMax (di.mValueRightOfMax), mOKtoTestForSpike (di.mOKtoTestForSpike), mSecondaryModeSet (di.mSecondaryModeSet) {}


DataInterval :: ~DataInterval () {}


DataInterval* DataInterval :: Split (DataInterval*& secondInterval) const {

	if (mNumberOfMinima == 0) {

		secondInterval = NULL;
		return NULL;
	}

	int ctr = (Left + mLocalMinimum) / 2;
	DataInterval* rtnValue = new DataInterval (Left, ctr, mLocalMinimum);
	rtnValue->SetNumberOfMinima (0);
	rtnValue->SetFixedRightTrue ();

	if (FixedLeft)
		rtnValue->SetFixedLeftTrue ();

	ctr = (mLocalMinimum + Right) / 2;
	secondInterval = new DataInterval (mLocalMinimum, ctr, Right);
	secondInterval->SetNumberOfMinima (0);
	secondInterval->SetFixedLeftTrue ();

	if (FixedRight)
		secondInterval->SetFixedRightTrue ();

	// Set modes and value at modes for both and then return

	if (Mode < mLocalMinimum) {

		rtnValue->SetMode (Mode);
		rtnValue->SetMaxAtMode (MaxAtMode);
		secondInterval->SetMode (mSecondaryMode);
		secondInterval->SetMaxAtMode (mMaxAtSecondaryMode);
	}

	else {

		rtnValue->SetMode (mSecondaryMode);
		rtnValue->SetMaxAtMode (mMaxAtSecondaryMode);
		secondInterval->SetMode (Mode);
		secondInterval->SetMaxAtMode (MaxAtMode);
	}

	return rtnValue;
}


void DataInterval :: ReassessRelativeMinimaGivenNoise (double noiseLevel, DataSignal* data) {

	if (mNumberOfMinima == 0)
		return;

	int i;
	SampledData* localData = (SampledData*) data;
	double current;
	double prev;
	double next;
	//prev = localData->Value (Left);
	//bool foundMinimum = false;
	//bool foundMax = false;
	//bool lookingForMin = false;
	//double currentMinValue;
	//double currentMaxValue = prev;
	//double leftMax;
	//double rightMax;
	bool foundRightMax = false;

	prev = mLocalMinValue;

	for (i=mLocalMinimum+1; i<Right; i++) {

		current = localData->Value (i);
		next = localData->Value (i + 1);

		if ((current >= prev) && (current > next)) {

			if (current - mLocalMinValue > noiseLevel) {

				foundRightMax = true;
				break;
			}

			mNumberOfMinima = 0;
			return;
		}

		prev = current;
	}

	if (!foundRightMax) {

		if (!(localData->Value (Right) - mLocalMinValue > noiseLevel)) {

			mNumberOfMinima = 0;
			return;
		}
	}

	next = mLocalMinValue;

	for (i=mLocalMinimum-1; i>Left; i--) {

		current = localData->Value (i);
		prev = localData->Value (i - 1);

		if ((current > prev) && (current >= next)) {

			if (current - mLocalMinValue > noiseLevel)
				return;

			mNumberOfMinima = 0;
			return;
		}

		next = current;
	}

	if (localData->Value (Left) - mLocalMinValue > noiseLevel)
		return;

	//for (i=Left+1; i<Right; i++) {

	//	current = localData->Value (i);
	//	next = localData->Value (i + 1);

	//	if (lookingForMin) {

	//		if ((current < prev) && (current < next)) {

	//			//  Found relative minimum
	//			foundMinimum = true;
	//			currentMinValue = current;

	//			if (foundMax) {

	//				if (currentMaxValue - current > noiseLevel)
	//					return;
	//			}

	//			foundMax = false;
	//			lookingForMin = false;
	//		}
	//	}

	//	else {	// looking for max

	//		if ((current > prev) && (current > next)) {

	//			//  Found relative max
	//			foundMax = true;
	//			currentMaxValue = current;

	//			if (foundMinimum) {

	//				if (current - currentMinValue > noiseLevel)
	//					return;
	//			}

	//			foundMinimum = false;
	//			lookingForMin = true;
	//		}
	//	}

	//	prev = current;
	//}

	mNumberOfMinima = 0;
}


void DataInterval :: RecomputeRelativeMinimum (const DataSignal* currentSignal, double mean, int localMax, double maxValue, const DataSignal* rawData) {

	if (mNumberOfMinima > 0)  // ???
		return;

	int localMin;
	int i;
	double localMinValue = localMax;
	double currentValue;
	bool foundMin = false;

	if (mean < localMax) {

		for (i=localMax; i>mean; i--) {

			currentValue = rawData->Value (i);

			if (currentValue < localMinValue) {

				foundMin = true;
				localMinValue = currentValue;
				localMin = i;
			}
		}
	}

	else {

		for (i=localMax; i<mean; i++) {

			currentValue = rawData->Value (i);

			if (currentValue < localMinValue) {

				foundMin = true;
				localMinValue = currentValue;
				localMin = i;
			}
		}
	}

	if (foundMin) {

		mLocalMinimum = localMin;
		mLocalMinValue = localMinValue;
	}

	else {

		mLocalMinimum = localMax;
		mLocalMinValue = maxValue;
	}

	//mLocalMinimum = (int) floor (0.5 * (mean + localMax) + 0.5);

	//mLocalMinValue = rawData->Value (mLocalMinimum);
	mNumberOfMinima = 1;
}


void DataInterval :: AddSideValues (DataSignal* sampledData) {

	int N = sampledData->GetNumberOfSamples ();

	if ((Mode < 1) || (Mode > N - 2))
		return;

	mValueLeftOfMax = sampledData->Value (Mode - 1);
	mValueRightOfMax = sampledData->Value (Mode + 1);
	mOKtoTestForSpike = TRUE;
}


DataSignal* DataInterval :: TestForSpike (double& fit) const {

	DataSignal* value = NULL;

	double detectionRFU = SampledData::GetDetectionRFU ();
	double threshold = 0.07 * MaxAtMode;

	if (!mOKtoTestForSpike)
		return NULL;

	if ((MaxAtMode < detectionRFU) || (mValueLeftOfMax >= threshold) || (mValueRightOfMax >= threshold))
		return NULL;

	value = new SpikeSignal (Mode, MaxAtMode, mValueLeftOfMax, mValueRightOfMax);
	double denom = MaxAtMode * MaxAtMode;

	if (mValueLeftOfMax > 0.0)
		denom += + mValueLeftOfMax * mValueLeftOfMax;

	if (mValueRightOfMax > 0.0)
		denom += + mValueRightOfMax * mValueRightOfMax;

	fit = MaxAtMode / sqrt (denom);
	value->SetCurveFit (fit);
	return value;
}


size_t DataInterval :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += 4 * sizeof (int) + 5 * sizeof (double) + 2 * sizeof (Boolean);
	return size;
}


void DataInterval :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	f.Read (Left);
	f.Read (Right);
	f.Read (Center);
	f.Read (Mass);
	f.Read (Height);
	f.Read (Mode);
	f.Read (MaxAtMode);
	f.Read (FixedLeft);
	f.Read (FixedRight);
	f.Read (mValueLeftOfMax);
	f.Read (mValueRightOfMax);
}


void DataInterval :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
	f >> Left;
	f >> Right;
	f >> Center;
	f >> Mass;
	f >> Height;
	f >> Mode;
	f >> MaxAtMode;
	f >> FixedLeft;
	f >> FixedRight;
	f >> mValueLeftOfMax;
	f >> mValueRightOfMax;
}


void DataInterval :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	f.Write (Left);
	f.Write (Right);
	f.Write (Center);
	f.Write (Mass);
	f.Write (Height);
	f.Write (Mode);
	f.Write (MaxAtMode);
	f.Write (FixedLeft);
	f.Write (FixedRight);
	f.Write (mValueLeftOfMax);
	f.Write (mValueRightOfMax);
}


void DataInterval :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
	f << Left;
	f << Right;
	f << Center;
	f << Mass;
	f << Height;
	f << Mode;
	f << MaxAtMode;
	f << FixedLeft;
	f << FixedRight;
	f << mValueLeftOfMax;
	f << mValueRightOfMax;
}


NoiseInterval :: NoiseInterval () : RGPersistent (), Left (0), Right (0),
Maximum (-DOUBLEMAX), Minimum (DOUBLEMAX), Average (0.0), NumberOfMaxima (0), NumberOfMinima (0),
NumberOfSamples (0), Norm2 (0.0), CurrentValue (0.0), PreviousValue (0.0), 
AscendingMode (FALSE), RawAscendingMode (FALSE), EndOfInterval (FALSE) {

}


NoiseInterval :: NoiseInterval (double value, int right) : RGPersistent (), Left (right), Right (right),
Maximum (-DOUBLEMAX), Minimum (DOUBLEMAX), Average (0.0), NumberOfMaxima (0), NumberOfMinima (0),
NumberOfSamples (0), Norm2 (0.0), CurrentValue (value), PreviousValue (value), 
AscendingMode (FALSE), RawAscendingMode (FALSE), EndOfInterval (FALSE) {

}


NoiseInterval :: ~NoiseInterval () {

}


void NoiseInterval :: AddPoint (double value, int location) {

	if (EndOfInterval)
		return;
	
	PreviousValue = CurrentValue;
	CurrentValue = value;
	
	if (AscendingMode) {

		AscendingLeft = location;

		if (value > AscendingMaximum)
			AscendingMaximum = value;

		if (value < AscendingMinimum)
			AscendingMinimum = value;

		AscendingAverage += value;
		AscendingNorm2 += value * value;

		if (RawAscendingMode) {

			if (value < PreviousValue) {

				RawAscendingMode = FALSE;
				AscendingNumberOfMaxima++;
			}
		}

		else {  // descending

			if (value > PreviousValue) {

				RawAscendingMode = TRUE;
				AscendingNumberOfMinima++;
			}
		}

		AscendingNumberOfSamples++;
	}

	else {

		Left = location;
		
		if (value > Maximum)
			Maximum = value;

		if (value < Minimum)
			Minimum = value;

		Average += value;
		Norm2 += value * value;

		if (RawAscendingMode) {

			if (value < PreviousValue) {

				RawAscendingMode = FALSE;
				NumberOfMaxima++;
			}
		}

		else {  // descending

			if (value > PreviousValue) {

				RawAscendingMode = TRUE;
				NumberOfMinima++;
			}
		}

		NumberOfSamples++;
	}
}


void NoiseInterval :: SetModeAscending () {

	if ((!AscendingMode) && (!EndOfInterval)) {

		AscendingLeft = Left;
		AscendingMaximum = Maximum;
		AscendingMinimum = Minimum;
		AscendingAverage = Average;
		AscendingNorm2 = Norm2;
		AscendingNumberOfMaxima = NumberOfMaxima;
		AscendingNumberOfMinima = NumberOfMinima;
		AscendingNumberOfSamples = NumberOfSamples;
		AscendingMode = TRUE;
	}
}


void NoiseInterval :: SetModeDescending () {

	if ((AscendingMode) && (!EndOfInterval)) {

		Left = AscendingLeft;
		Maximum = AscendingMaximum;
		Minimum = AscendingMinimum;
		Average = AscendingAverage;
		Norm2 = AscendingNorm2;
		NumberOfMaxima = AscendingNumberOfMaxima;
		NumberOfMinima = AscendingNumberOfMinima;
		NumberOfSamples = AscendingNumberOfSamples;
		AscendingMode = FALSE;
	}
}


void NoiseInterval :: EndNoiseInterval () {

	EndOfInterval = TRUE;
}


double NoiseInterval :: GetNorm2 (double spacing) const {

	return Norm2 * spacing;
}


double NoiseInterval :: GetNorm2Rate () const {

	return Norm2 / (double)NumberOfSamples;
}


size_t NoiseInterval :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += 5 * sizeof (int) + 4 * sizeof (double);
	return size;
}



void NoiseInterval :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	f.Read (Left);
	f.Read (Right);
	f.Read (Maximum);
	f.Read (Minimum);
	f.Read (Average);
	f.Read (NumberOfMaxima);
	f.Read (NumberOfMinima);
	f.Read (NumberOfSamples);
	f.Read (Norm2);
}


void NoiseInterval :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
	f >> Left;
	f >> Right;
	f >> Maximum;
	f >> Minimum;
	f >> Average;
	f >> NumberOfMaxima;
	f >> NumberOfMinima;
	f >> NumberOfSamples;
	f >> Norm2;
}


void NoiseInterval :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	f.Write (Left);
	f.Write (Right);
	f.Write (Maximum);
	f.Write (Minimum);
	f.Write (Average);
	f.Write (NumberOfMaxima);
	f.Write (NumberOfMinima);
	f.Write (NumberOfSamples);
	f.Write (Norm2);
}


void NoiseInterval :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
	f << Left;
	f << Right;
	f << Maximum;
	f << Minimum;
	f << Average;
	f << NumberOfMaxima;
	f << NumberOfMinima;
	f << NumberOfSamples;
	f << Norm2;
}

