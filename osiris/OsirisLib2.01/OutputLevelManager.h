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
*  FileName: OutputLevelManager.h
*  Author:   Robert Goor
*
*/
//
//  This class manages the output levels used by Osiris.  Initially, defaults will be set on instantiation.  Later,
//  we will read in the levels from an xml configuration file
//

#ifndef _OUTPUTLEVELMANAGER_H_
#define _OUTPUTLEVELMANAGER_H_

#include "rgstring.h"


class OutputLevelManager {

public:
	OutputLevelManager ();
	~OutputLevelManager ();

	static unsigned long GeneticsInputMessages;
	static unsigned long LocusGridQuality;
	static unsigned long LocusSampleCharacteristics;
	static unsigned long LaneStandardQuality;

	static unsigned long PeakOutOfPlace;
	static unsigned long PeakOutsideLaneStandard;
	static unsigned long PeakBelowLocus;
	static unsigned long PeakAboveLocus;
	static unsigned long PeakBetweenLoci;
	static unsigned long SignalTooNarrow;
	static unsigned long SignalTooWide;
	static unsigned long SignalTooLow;
	static unsigned long SignalTooHigh;
	static unsigned long NonTemplateAdditionForFollowing;
	static unsigned long NonTemplateAdditionForPreceding;
	static unsigned long StutterForFollowing;
	static unsigned long StutterForPreceding;
	static unsigned long OffLadderSignalBetweenLoci;
	static unsigned long CurveFitUnacceptable;
	static unsigned long CurveFitWarrantsTestForArtifactSignature;
	static unsigned long PeakHeightBelowMinimumStandard;
	static unsigned long PeakHeightAboveMaximumStandard;
	static unsigned long CurveFitHasNegativeComponent;
	static unsigned long AbbreviatedPeakData;
	static unsigned long CurveIsABlob;
	static unsigned long CurveIsASpike;
	static unsigned long CurveIsAPullUp;

protected:
};



#endif  /*  _OUTPUTLEVELMANAGER_H_  */

