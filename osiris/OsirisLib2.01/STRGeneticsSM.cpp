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
*  FileName: STRGeneticsSM.cpp
*  Author:   Robert Goor
*
*/
//
//  STR Genetics classes, subclasses of those in Genetics.h:  SmartMessage-related functions only
//

#include "STRGenetics.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "DataSignal.h"
#include "rgtokenizer.h"
#include "rgpscalar.h"
#include "coordtrans.h"
#include "ChannelData.h"
#include "Notices.h"
#include "Genetics.h"
#include "SmartNotice.h"
#include "STRSmartNotices.h"
#include <iostream>

using namespace std;


Boolean STRBaseLocus :: ExtractGridSignalsSM (RGDList& channelSignalList, const LaneStandard* ls, RGDList& locusDataSignalList, ChannelData* lsData, Locus* locus) {

	//
	//  This is ladder stage 1
	//

	int lsSize = ls->GetNumberOfCharacteristics ();
	int i1;
	int i2;
	double c1;
	double c2;

	smNPeaksForLocusUnavailable nPeaksUnavailableForLocus;
	smLocusTooFewPeaks locusHasTooFewPeaks;

	if (lsSize < 0) {

		Msg << "Number of characteristics not available for Locus named " << LocusName << "\n";
		locus->SetMessageValue (nPeaksUnavailableForLocus, true);
		return FALSE;
	}

	if (LowerBoundGridLSBasePair < 0.0) {

		i1 = (int) floor (LowerBoundGridLSIndex);

		if (i1 < 0) {

			Msg << "Internal Lane Standard lower bound index for Grid is out of bounds for Locus named " << LocusName << "\n";
			return FALSE;
		}

		i2 = i1 + 1;
		c2 = LowerBoundGridLSIndex - i1;
		c1 = 1.0 - c2;
		MinimumGridTime = c1 * ls->GetLaneStandardTimeForCharacteristicNumber (i1) + 
			c2 * ls->GetLaneStandardTimeForCharacteristicNumber (i2);
//		cout << "Not using base pairs for minimum of locus " << LocusName << endl;
	}

	else
		MinimumGridTime = lsData->GetTimeForSpecifiedID (LowerBoundGridLSBasePair);

	if (UpperBoundGridLSBasePair < 0.0) {

		i1 = (int) floor (UpperBoundGridLSIndex);

		if (i1 < 0) {

			Msg << "Internal Lane Standard upper bound index for Grid is out of bounds for Locus named " << LocusName << "\n";
			return FALSE;
		}

		i2 = i1 + 1;
		c2 = UpperBoundGridLSIndex - i1;
		c1 = 1.0 - c2;
		MaximumGridTime = c1 * ls->GetLaneStandardTimeForCharacteristicNumber (i1) + 
			c2 * ls->GetLaneStandardTimeForCharacteristicNumber (i2);
//		cout << "Not using base pairs for maximum of locus " << LocusName << endl;
	}

	else
		MaximumGridTime = lsData->GetTimeForSpecifiedID (UpperBoundGridLSBasePair);

	RGDListIterator it (channelSignalList);
	DataSignal* nextSignal;
	double nextMean;
	locusDataSignalList.Clear ();

	// Assume channelSignalList is in increasing order by signal means

	while (nextSignal = (DataSignal*) it ()) {

		nextMean = nextSignal->GetMean ();

		if (nextMean >= MinimumGridTime) {

			if (nextMean <= MaximumGridTime) {

	//			it.RemoveCurrentItem ();
				locusDataSignalList.Append (nextSignal);
			}

			else
				break;
		}
	}

	if (locusDataSignalList.Entries () < mLocusSize) {

		Msg << "Locus named " << LocusName << " could not find the required number of unanalyzed peaks in range\n";
		locus->SetMessageValue (locusHasTooFewPeaks, true);
		return FALSE;
	}

	return TRUE;
}


