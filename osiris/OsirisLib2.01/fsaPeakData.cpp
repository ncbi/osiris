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
*  FileName: fsaPeakData.cpp
*  Author:   Robert Goor
*
*/
//
//  class fsaPeakData to encompass and manage data for fsaPeak structs; calculates standard statistics
//

#include "fsaPeakData.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>


using namespace std;


ABSTRACT_DEFINITION (fsaPeakData)


fsaPeakData :: fsaPeakData () : RGPersistent (),
tmean (0.0),
tleft (0.0),
tright (0.0),
position (0),
height (0),
beginPos (0),
endPos (0),
beginHt (0),
endHt (0),
area (0),
volume (0),
fragSize (0.0),
isEdited (0) {

	label [0] = '\0';
}


fsaPeakData :: fsaPeakData (DataSignal* ds, CoordinateTransform* ILSMap) : RGPersistent () {

	tmean = ds->GetMean ();
//	mean = ILSMap->EvaluateWithExtrapolation (tmean);
	double sigma = ds->GetStandardDeviation ();
	sigma *= 4.0;
//	double v1 = sigma * (1.0 + RandomFraction (0.2));
//	double v2 = sigma * (1.0 + RandomFraction (0.2));
	double v1 = sigma;
	double v2 = sigma;
	tleft = tmean - v1;
	tright = tmean + v2;

	position = (long) floor (tmean + 0.5);
	height = 0;

	beginPos = (long) floor (tleft + 0.5);
	endPos = (long) floor (tright + 0.5);

	endHt = beginHt = 0;
	area = (long) ds->OneNorm ();
	volume = 0;

	fragSize = (float) ILSMap->EvaluateWithExtrapolation (tmean);

	isEdited = 0;
	label [0] = '\0';
}


fsaPeakData :: ~fsaPeakData () {

}


void fsaPeakData :: EvaluateCurveFeatures (const DataSignal* ds) {

	//  evaluate height, volume, endHt, beginHt

	height = (short) floor (ds->Value ((int)position));
//	volume = height * area;
	volume = 0;

	endHt = (short) floor (ds->Value ((int)endPos));
	beginHt = (short) floor (ds->Value ((int)beginPos));
}


fsaPeak fsaPeakData :: GetFSAPeakStructure () const {

	fsaPeak Ans;

	Ans.position = position;
	Ans.height = height;
	Ans.beginPos = beginPos;
	Ans.endPos = endPos;
	Ans.beginHt = beginHt;
	Ans.endHt = endHt;
	Ans.area = area;
	Ans.volume = volume;
	Ans.fragSize = fragSize;
	Ans.isEdited = isEdited;

	for (int i=0; i<64; i++)
		Ans.label [i] = '\0';

	return Ans;
}


void fsaPeakData :: ResetBeginPosition (long beginP, const DataSignal* ds) {

	beginPos = beginP;
	beginHt = (short) floor (ds->Value ((int)beginPos));
}


void fsaPeakData :: ResetEndPosition (long endP, const DataSignal* ds) {

	endPos = endP;
	endHt = (short) floor (ds->Value ((int)endPos));
}


int fsaPeakData :: CompareTo (const RGPersistent* p) const {

	fsaPeakData* q = (fsaPeakData*) p;

	if (position < q->position)
		return -1;

	if (position > q->position)
		return 1;

	return 0;
}


Boolean fsaPeakData :: IsEqualTo (const RGPersistent* p) const {

	fsaPeakData* q = (fsaPeakData*) p;
	return (position == q->position);
}



double fsaPeakData :: RandomFraction (double MaxFraction) {

	double x = rand ();
	return -MaxFraction + ((2.0 * MaxFraction) / (double)RAND_MAX) * x;
}



void fsaPeakData :: ResetBoundaries (fsaPeakData* prev, fsaPeakData* next, const DataSignal* noisySample) {

	long left = next->GetBeginPosition ();
	long right = prev->GetEndPosition ();
	long middle;
	double leftValue = noisySample->Value ((int)left);
	double rightValue = noisySample->Value ((int)right);
	double middleValue;
	bool LeftIsLarger = true;

	if (leftValue < rightValue)
		LeftIsLarger = false;

	while (right - left > 2) {

		middle = (left + right) / 2;
		middleValue = noisySample->Value ((int)middle);

		if (leftValue < rightValue) {

			right = middle;
			rightValue = middleValue;
		}

		else {

			left = middle;
			leftValue = middleValue;
		}
	}

	middle = (left + right) / 2;

	if (LeftIsLarger) {

		prev->ResetEndPosition (middle, noisySample);
		next->ResetBeginPosition (middle + 1, noisySample);
	}

	else {

		prev->ResetEndPosition (middle - 1, noisySample);
		next->ResetBeginPosition (middle, noisySample);
	}
}

