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
*  FileName: Evaluate.h
*  Author:   Robert Goor
*
*/
//
//  Class Evaluate, the abstract base class of all test processes, designed to make all testing data driven.  Each
//  Test function returns 1 for 'true' or passed, 0 for 'false' or failed, and -1 for 'marginal'.  Function value
//  Test routines store the value of numerical return under a name, assigned to the object, and stored in a 
//  TestStack object (not yet created as of 2/2/08).  Named logical objects store the -1, 0, 1 values under their name
//  in the TestStack object.
//

#ifndef _EVALUATE_H_
#define _EVALUATE_H_

#include "DataSignal.h"
#include "ChannelData.h"
#include "rgdlist.h"
#include "rgstring.h"
#include "rgpersist.h"
#include "coordtrans.h"


class Evaluate : public RGPersistent {

ABSTRACT_DECLARATION (Evaluate)

public:
	Evaluate ();
	Evaluate (int severity, int report, int timing, const RGString& name, int msgNumber);
	Evaluate (int severity, int report, int timing, int msgNumber);
	Evaluate (const RGString& xmlInput);
	Evaluate (const Evaluate& eval);
	virtual ~Evaluate () = 0;

	Boolean isValid () const { return mValid; }

//	virtual int Test (DataSignal* testSample, DataSignal* previous, DataSignal* following, RGDList& allSamples, int channel,
//		ChannelData** channels, int nChannels);
	virtual int Test (DataSignal* testSample);
	virtual int Test (DataSignal* testSample, DataSignal* previous, DataSignal* following);
	virtual int Test (int channel, ChannelData** channels, int nChannels, CoordinateTransform* trans);
	virtual int Initialize (RGDList& evalList);

protected:
	int mSeverityLevel;
	int mReportLevel;
	unsigned int mTimingLevel;
	RGString mName;
	RGString mReport;
	int mReportMessageNumber;
	Boolean mValid;
};


#endif  /*  _EVALUATE_H_  */

