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
*  FileName: fsaPeakData.h
*  Author:   Robert Goor
*
*/
//
//  class fsaPeakData to encompass and manage data for fsaPeak structs; calculates standard statistics
//

#ifndef _FSAPEAKDATA_H_
#define _FSAPEAKDATA_H_

#include "rgpersist.h"
#include "rgdefs.h"
#include "rgstring.h"
#include "rgdlist.h"
#include "DataSignal.h"
#include "fsaFileData.h"
#include "fsaDirEntry.h"
#include "coordtrans.h"


class fsaPeakData : public RGPersistent {

ABSTRACT_DECLARATION (fsaPeakData)

public:
	fsaPeakData ();
	fsaPeakData (DataSignal* ds, CoordinateTransform* ILSMap);
	virtual ~fsaPeakData ();

	void EvaluateCurveFeatures (const DataSignal* ds);
	fsaPeak GetFSAPeakStructure () const;

	long GetBeginPosition () const { return beginPos; }
	long GetEndPosition () const { return endPos; }
	short GetBeginHeight () const { return beginHt; }
	short GetEndHeight () const { return endHt; }

	void ResetBeginPosition (long beginP, const DataSignal* ds);
	void ResetEndPosition (long endP, const DataSignal* ds);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	static void ResetBoundaries (fsaPeakData* prev, fsaPeakData* next, const DataSignal* noisySample);

protected:
	double tmean;  // in time
	double tleft;  // in time
	double tright;  // in time

	long position;  // = sample index (time in seconds)
	short height;  // evaluate after noise inserted

	long beginPos;  // = floor (tleft + 0.5)
	long endPos;  // = floor (tright + 0.5)

	short beginHt;  // evaluate after noise inserted
	short endHt;  // evaluate after noise inserted

	long area;  // = L1 norm
	long volume;  // = area * height? = 0
	float fragSize;  // = ILSMap (position) ~= ILSMap (tmean)

	short isEdited;  // = 0
	char label[64];  // = "" = '\0'

	double RandomFraction (double MaxFraction);
};


#endif  /*  _FSAPEAKDATA_H_  */
