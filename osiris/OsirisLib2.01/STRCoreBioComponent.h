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
*  FileName: STRCoreBioComponent.h
*  Author:   Robert Goor
*
*/
//
//  class STRCoreBioComponent, a subclass of CoreBioComponent, and parent class to STRLadderBioComponent and STRSampleBioComponent
//

#ifndef _STRCOREBIOCOMPONENT_H_
#define _STRCOREBIOCOMPONENT_H_

#include "CoreBioComponent.h"


const int _STRLADDERCOREBIOCOMPONENT_ = 1070;
const int _STRSAMPLECOREBIOCOMPONENT_= 1071;

PERSISTENT_PREDECLARATION (STRLadderCoreBioComponent)
PERSISTENT_PREDECLARATION (STRSampleCoreBioComponent)

int MinimumIndex (double* array, int size);


class STRCoreBioComponent : public CoreBioComponent {

ABSTRACT_DECLARATION (STRCoreBioComponent)

public:
	STRCoreBioComponent ();
	STRCoreBioComponent (const RGString& name);
	STRCoreBioComponent (const STRCoreBioComponent& str);
	STRCoreBioComponent (const STRCoreBioComponent& str, CoordinateTransform* trans) : CoreBioComponent (str, trans) {}
	virtual ~STRCoreBioComponent () = 0;

	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************

	virtual ChannelData* GetNewLaneStandardChannel (int channel, LaneStandard* inputLS);
	virtual ChannelData* GetNewDataChannel (int channel, LaneStandard* inputLS);
	virtual ChannelData* GetNewGridDataChannel (int channel, LaneStandard* inputLS);
	virtual CoreBioComponent* CreateNewTransformedBioComponent (const CoreBioComponent& bc, CoordinateTransform* trans) = 0;

	// Legacy Message functions*****************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int AnalyzeGrid (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int AnalyzeCrossChannel ();	//$
	virtual int WriteXMLGraphicData (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro);	//$

	//******************************************************************************************************************************************
	//******************************************************************************************************************************************

	// Smart Message functions******************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int AnalyzeGridSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AnalyzeCrossChannelSM ();
	virtual int AnalyzeCrossChannelWithNegativePeaksSM ();
	virtual int UseChannelPatternsToAssessCrossChannelWithNegativePeaksSM ();
	virtual int WriteXMLGraphicDataSM (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro);
	virtual void ReevaluateNoiseThresholdBasedOnMachineType (const RGString& machine);

	//******************************************************************************************************************************************

/*	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;*/

protected:
/*	RGString mName;
	PackedTime mTime;
	ChannelData** mDataChannels;
	int mNumberOfChannels;
	PopulationMarkerSet* mMarkerSet;  // this should be a new copy
	int mLaneStandardChannel;
	TestCharacteristic* mTest;
	ChannelData* mLSData;

	static Boolean SearchByName;*/
	int mNumberOfPullups;
	int mNumberOfPrimaryPullups;

	bool AssessPrimaryPullup (DataSignal* primaryPullup, RGDList& peaksInSameChannel, RGDList& probablePullup, RGDList& pullupList);
	double FindPrimaryPeak (RGDList& peakList, DataSignal*& primarySignal);
};


class STRLadderCoreBioComponent : public STRCoreBioComponent {

	PERSISTENT_DECLARATION (STRLadderCoreBioComponent)

public:
	STRLadderCoreBioComponent ();
	STRLadderCoreBioComponent (const RGString& name);
	STRLadderCoreBioComponent (const STRLadderCoreBioComponent& ladder);
	STRLadderCoreBioComponent (const STRLadderCoreBioComponent& ladder, CoordinateTransform* trans);
	virtual ~STRLadderCoreBioComponent ();

	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************

	virtual CoreBioComponent* CreateNewTransformedBioComponent (const CoreBioComponent& bc, CoordinateTransform* trans);

	// Legacy Message functions*****************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int AnalyzeGridLoci (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int AnalyzeGrid (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//incomplete

	virtual int AnalyzeCrossChannel ();	//incomplete
	virtual int GridQualityTest ();	//incomplete
	virtual int WriteXMLGraphicData (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro);	//$

	//******************************************************************************************************************************************
	//******************************************************************************************************************************************

	// Smart Message functions******************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int AnalyzeGridLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AnalyzeGridSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	virtual void MakeNonCoreLadderArtifactsNoncritical ();

	virtual int AnalyzeCrossChannelSM ();
	virtual int GridQualityTestSM ();
	virtual int GridQualityTestSMPart2 (SmartMessagingComm& comm, int numHigherObjects);
	virtual int RemoveInterlocusSignalsSM ();
	virtual int WriteXMLGraphicDataSM (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro);

	//******************************************************************************************************************************************

//	virtual int Initialize (SampleData& fileData, PopulationCollection* collection, const RGString& markerSetName);
};


class STRSampleCoreBioComponent : public STRCoreBioComponent {

	PERSISTENT_DECLARATION (STRSampleCoreBioComponent)

public:
	STRSampleCoreBioComponent ();
	STRSampleCoreBioComponent (const RGString& name);
	STRSampleCoreBioComponent (const STRSampleCoreBioComponent& sample);
	STRSampleCoreBioComponent (const STRSampleCoreBioComponent& sample, CoordinateTransform* trans);
	virtual ~STRSampleCoreBioComponent ();

	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************

	virtual CoreBioComponent* CreateNewTransformedBioComponent (const CoreBioComponent& bc, CoordinateTransform* trans);

	// Legacy Message functions*****************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int AnalyzeSampleLoci (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int ResolveAmbiguousInterlocusSignals ();	//$
	virtual int SampleQualityTest (GenotypesForAMarkerSet* genotypes);	//$

	//******************************************************************************************************************************************
	//******************************************************************************************************************************************

	// Smart Message functions******************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int AnalyzeSampleLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int ResolveAmbiguousInterlocusSignalsSM ();
	virtual int SampleQualityTestSM (GenotypesForAMarkerSet* genotypes);
	virtual int SignalQualityTestSM ();
	virtual int RemoveInterlocusSignalsSM ();
	virtual int FitAllSampleCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	//******************************************************************************************************************************************

//	virtual int Initialize (SampleData& fileData, PopulationCollection* collection, const RGString& markerSetName);
};


#endif  /*   _STRCOREBIOCOMPONENT_H_  */


