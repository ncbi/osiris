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
*  FileName: STREvaluate.h
*  Author:   Robert Goor
*
*/
//
//  Subclasses of Evaluate for STR's
//

#ifndef _STREVALUATE_H_
#define _STREVALUATE_H_

#include "Evaluate.h"

const int _STRMINIMUMRFU_ = 1101;
const int _STRCROSSCHANNEL_ = 1102;

PERSISTENT_PREDECLARATION (STRMinimumRFU)
PERSISTENT_PREDECLARATION (STRCrossChannel)


class STRMinimumRFU : public Evaluate {

PERSISTENT_DECLARATION (STRMinimumRFU)

public:
	STRMinimumRFU ();
	STRMinimumRFU (int severity, int report, int timing, const RGString& name, int msgNumber, double minRFU);
	STRMinimumRFU (int severity, int report, int timing, int msgNumber, double minRFU);
	STRMinimumRFU (const RGString& xmlInput);
	STRMinimumRFU (const STRMinimumRFU& eval);
	virtual ~STRMinimumRFU ();

	virtual int Test (DataSignal* testSample);
	virtual int Initialize (RGDList& evalList);

protected:
	double mMinRFU;
};


class STRCrossChannel : public Evaluate {

PERSISTENT_DECLARATION (STRCrossChannel)

public:
	STRCrossChannel ();
	STRCrossChannel (int severity, int report, int timing, const RGString& name, int msgNumber, double timeTolerance,
		double widthFraction, double widthMatch, double heightMatch);
	STRCrossChannel (int severity, int report, int timing, int msgNumber, double timeTolerance,
		double widthFraction, double widthMatch, double heightMatch);
	STRCrossChannel (const RGString& xmlInput);
	STRCrossChannel (const STRCrossChannel& eval);
	virtual ~STRCrossChannel ();

	virtual int Test (int channel, ChannelData** channels, int nChannels, CoordinateTransform* trans);
	virtual int Initialize (RGDList& evalList);

protected:
	double mTimeTolerance;
	double mWidthFractionForSpike;
	double mWidthMatchFraction;
	double mHeightMatchFractionForSpike;
};


#endif  /*  _STREVALUATE_H_  */



