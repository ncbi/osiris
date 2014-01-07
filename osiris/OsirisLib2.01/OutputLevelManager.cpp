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
*  FileName: OutputLevelManager.cpp
*  Author:   Robert Goor
*
*/
//
//  This class manages the output levels used by Osiris.  Initially, defaults will be set on instantiation.  Later,
//  we will read in the levels from an xml configuration file
//

#include "OutputLevelManager.h"


unsigned long OutputLevelManager::GeneticsInputMessages = 15;
unsigned long OutputLevelManager::LocusGridQuality = 1;
unsigned long OutputLevelManager::LocusSampleCharacteristics = 3;
unsigned long OutputLevelManager::LaneStandardQuality = 1;

unsigned long OutputLevelManager::PeakOutOfPlace = 3;
unsigned long OutputLevelManager::PeakOutsideLaneStandard = 3;
unsigned long OutputLevelManager::PeakBelowLocus = 3;
unsigned long OutputLevelManager::PeakAboveLocus = 3;
unsigned long OutputLevelManager::PeakBetweenLoci = 3;
unsigned long OutputLevelManager::SignalTooNarrow = 2;
unsigned long OutputLevelManager::SignalTooWide = 2;
unsigned long OutputLevelManager::SignalTooLow = 2;
unsigned long OutputLevelManager::SignalTooHigh = 2;
unsigned long OutputLevelManager::NonTemplateAdditionForFollowing = 1;
unsigned long OutputLevelManager::NonTemplateAdditionForPreceding = 1;
unsigned long OutputLevelManager::StutterForFollowing = 1;
unsigned long OutputLevelManager::StutterForPreceding = 1;
unsigned long OutputLevelManager::OffLadderSignalBetweenLoci = 1;
unsigned long OutputLevelManager::CurveFitUnacceptable = 1;
unsigned long OutputLevelManager::CurveFitWarrantsTestForArtifactSignature = 10;
unsigned long OutputLevelManager::PeakHeightBelowMinimumStandard = 1;
unsigned long OutputLevelManager::PeakHeightAboveMaximumStandard = 2;
unsigned long OutputLevelManager::CurveFitHasNegativeComponent = 10;
unsigned long OutputLevelManager::AbbreviatedPeakData = 2;
unsigned long OutputLevelManager::CurveIsABlob = 5;
unsigned long OutputLevelManager::CurveIsASpike = 2;
unsigned long OutputLevelManager::CurveIsAPullUp = 2;


OutputLevelManager :: OutputLevelManager () {

}


OutputLevelManager :: ~OutputLevelManager () {

}

