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
*  FileName: CoreBioComponent.h
*  Author:   Robert Goor
*
*/
//
//     class CoreBioComponent and other abstract base classes that represent samples and control sets of various kinds
//

#ifndef _COREBIOCOMPONENT_H_
#define _COREBIOCOMPONENT_H_

#include "rgpersist.h"
#include "rgdefs.h"
#include "rgstring.h"
#include "rgdlist.h"

#include "PackedTime.h"
#include "PackedDate.h"
#include "ChannelData.h"
#include "Genetics.h"
#include "TestCharacteristic.h"
#include "RGTextOutput.h"
#include "OsirisMsg.h"
#include "coordtrans.h"
#include "Notice.h"
#include "SmartMessagingObject.h"

#include <list>

class RGFile;
class RGVInStream;
class RGVOutStream;
class DataSignal;
class SampleData;
class OsirisMsg;
class GenotypeSpecs;
class GenotypesForAMarkerSet;
class SmartMessage;
class SmartNotice;

#define _USENOISEINPULLUPANALYSIS_

//  The following two structs can be initialized once at the beginning of a run and reused for different input files...

struct GridDataStruct {

	PopulationCollection* mCollection;
	RGString mMarkerSetName;
	TestCharacteristic* mTestControlPeak;
	RGTextOutput& mText;
	RGTextOutput& mExcelText;
	OsirisMsg& mMsg;
	Boolean mPrint;

	GridDataStruct (PopulationCollection* collection, const RGString& markerSetName, TestCharacteristic* testPeak,
		RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print);
};


struct SampleDataStruct {

	PopulationCollection* mCollection;
	RGString mMarkerSetName;
	TestCharacteristic* mTestControlPeak;
	TestCharacteristic* mTestSamplePeak;
	RGTextOutput& mText;
	RGTextOutput& mExcelText;
	OsirisMsg& mMsg;
	Boolean mPrint;

	SampleDataStruct (PopulationCollection* collection, const RGString& markerSetName, TestCharacteristic* testControlPeak,
		TestCharacteristic* testSamplePeak, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print);
};


struct PullupPair {

	PullupPair () : mPrimary (NULL), mPullup (NULL), mPrimaryHeight (0.0), mPullupHeight (0.0), mIsOutlier (false) {}
	PullupPair (DataSignal* primary, DataSignal* pullup);
	PullupPair (DataSignal* primary);
	PullupPair (DataSignal* primary, double rawPullupPeak);
	PullupPair (DataSignal* primary, bool isDuplicate);
	PullupPair (const PullupPair& pup);
	~PullupPair () {}

	bool IsRawDataPullup () const;

	DataSignal* mPrimary;
	DataSignal* mPullup;
	double mPrimaryHeight;
	double mPullupHeight;
	bool mIsOutlier;
	bool mIsDuplicate;
};


struct QCData {

	QCData () : mMaxLinearPullupCoefficient (0.0), mMaxNonlinearPullupCoefficient (0.0), mMaxErrorInBP (0.0), mWidthOfLastILSPeak (0.0), mSampleLocusTotalAreaRatioMaxToMin (0.0), mSampleYLinkedLocusTotalAreaRatioMaxToMin (0.0), mStartingTemperature (0.0), mMaxMinusMinTemperature (0.0), 
		mStartingVoltage (0.0), mMaxMinusMinVoltage (0.0), mStartingCurrent (0.0), mMaxMinusMinCurrent (0.0), mStartingPower (0.0), mMaxMinusMinPower (0.0), mFirstILSTime (0.0), mLastILSTime (0.0), mNumberOfSamples (0) {}

	QCData (const QCData& data) : mMaxLinearPullupCoefficient (data.mMaxLinearPullupCoefficient), mMaxNonlinearPullupCoefficient (data.mMaxNonlinearPullupCoefficient), mMaxErrorInBP (data.mMaxErrorInBP), mWidthOfLastILSPeak (data.mWidthOfLastILSPeak), 
		mSampleLocusTotalAreaRatioMaxToMin (data.mSampleLocusTotalAreaRatioMaxToMin), mSampleYLinkedLocusTotalAreaRatioMaxToMin (data.mSampleYLinkedLocusTotalAreaRatioMaxToMin), mStartingTemperature (data.mStartingTemperature), mMaxMinusMinTemperature (data.mMaxMinusMinTemperature), mStartingVoltage (data.mStartingVoltage), 
		mMaxMinusMinVoltage (data.mStartingVoltage), mStartingCurrent (data.mStartingVoltage), mMaxMinusMinCurrent (data.mStartingVoltage), mStartingPower (data.mStartingVoltage), mMaxMinusMinPower (data.mStartingVoltage), mFirstILSTime (data.mFirstILSTime), mLastILSTime (data.mLastILSTime), 
		mNumberOfSamples (data.mNumberOfSamples) {}
	~QCData () {}

	double mMaxLinearPullupCoefficient;
	double mMaxNonlinearPullupCoefficient;
	double mMaxErrorInBP;
	double mWidthOfLastILSPeak;
	double mSampleLocusTotalAreaRatioMaxToMin;
	double mSampleYLinkedLocusTotalAreaRatioMaxToMin;
	double mStartingTemperature;
	double mMaxMinusMinTemperature;
	double mStartingVoltage;
	double mMaxMinusMinVoltage;
	double mStartingCurrent;
	double mMaxMinusMinCurrent;
	double mStartingPower;
	double mMaxMinusMinPower;
	double mFirstILSTime;
	double mLastILSTime;
	int mNumberOfSamples;
};



class CoreBioComponent : public SmartMessagingObject {

ABSTRACT_DECLARATION (CoreBioComponent)

public:
	CoreBioComponent ();
	CoreBioComponent (const RGString& name);
	CoreBioComponent (const CoreBioComponent& component);
	CoreBioComponent (const CoreBioComponent& component, CoordinateTransform* trans);
	virtual ~CoreBioComponent () = 0;

	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************

	void SetName (const RGString& name);
	void SetFileName (const RGString& name) { mFileName = name; }
	void SetSampleName (const RGString& sampleName) { mSampleName = sampleName; }
	void SetControlIdName (const RGString& name) { mControlIdName = name; }
	void SetTime (const RGString& time);
	void SetTime (const PackedTime& time);
	void SetProgress (int progress) { Progress = progress; }
	void ResetBoundsForILSUsingFactor (double factor) { if (mLaneStandard != NULL) mLaneStandard->ResetBoundsUsingFactorToILSHistory (factor); }
	void SetComments (const RGString& comment) { mComments = comment; }

//	int GetHighestSeverityLevel () const { return mHighestSeverityLevel; }
	int GetHighestMessageLevel () const { return mHighestMessageLevel; }

	RGString GetError () const { return ErrorString; }

	RGString GetSampleName () const { return mName; }
	RGString GetDataSampleName () const { return mSampleName; }
	RGString GetControlIdName () const { return mControlIdName; }
	PackedTime GetSampleTime () const { return mTime; }
	int GetNumberOfChannels () const { return mNumberOfChannels; }

	int GetAllAmbientData (SampleData* data);

	void SetTableLink (int linkNumber);
	RGString GetTableLink () const { return mTableLink; }

	int TimeSeparation (const CoreBioComponent* cbc);
	Locus* FindLocus (int channel, const RGString& locusName);
	Locus* FindLocus (const RGString& locusName);
	double GetTimeForSpecifiedID (int channel, double id);

	int CreateAndSubstituteFilteredDataSignalForRawDataNonILS ();
	int RestoreRawDataAndDeleteFilteredSignalNonILS ();

	int GetLocusAndChannelHighestMessageLevel ();
	Boolean PrepareLociForOutput ();

	bool ComputePullupParameters (list<PullupPair*>& pairList, double& linearPart, double& quadraticPart, double& lmValue, double& outlierThreshold);
	bool ComputeRefinedOutlierList (list<PullupPair*>& pairList, double& linearPart);
	bool ComputePullupParametersForNegativePeaks (int nNegatives, list<PullupPair*>& pairList, double& linearPart, double& quadraticPart, bool constrainLSQ);

	bool AddILSToHistory () { return mDataChannels [mLaneStandardChannel]->AddILSToHistoryList (); }

	bool PullupTestedMatrix (int i, int j);
	double LinearPullupCoefficient (int i, int j);
	double QuadraticPullupCoefficient (int i, int j);

	double MaxLinearPullupCoefficient () const { return mQC.mMaxLinearPullupCoefficient; }
	double MaxNonlinearPullupCoefficient () const { return mQC.mMaxNonlinearPullupCoefficient; }

	void SetPullupTestedMatrix (int i, int j, bool value);
	void SetLinearPullupMatrix (int i, int j, double value);
	void SetQuadraticPullupMatrix (int i, int j, double value);
	void CalculatePullupCorrection (int i, int j, list<PullupPair*>& pairList, bool testLaserOffScale);

	bool TestMaxAbsoluteRawDataInInterval (int channel, double center, double halfWidth, double fractionNoiseRange, double& value) const;

	void ReportSampleTableRow (RGTextOutput& text);
	void ReportSampleTableRowWithLinks (RGTextOutput& text);
	void ReportGridTableRow (RGTextOutput& text);
	void ReportGridTableRowWithLinks (RGTextOutput& text);

	virtual bool SampleIsValid () const;

	virtual void AppendAllBaseLociToList (RGDList& locusList);

	virtual void WriteRawDataAndFitData (RGTextOutput& text, SampleData* data);
	virtual int WriteFitDataForChannel (int channelNum, RGTextOutput& text, const RGString& delim, ChannelData* cd);
	virtual int WriteLocusInfoToXML (RGTextOutput& text, const RGString& indent);

	virtual CoreBioComponent* GetBestGridBasedOnTimeForAnalysis (RGDList& gridList);
	virtual CoreBioComponent* GetBestGridBasedOnMax2DerivForAnalysis (RGDList& gridList, CSplineTransform*& timeMap);
	virtual CoreBioComponent* GetBestGridBasedOnLeastTransformError (RGDList& gridList, CSplineTransform*& timeMap, const double* characteristicArray);
	virtual CoreBioComponent* GetBestGridBasedOnMaxDelta3DerivForAnalysis (RGDList& gridList, CSplineTransform*& timeMap);

	virtual int FindAndRemoveFixedOffsets ();
	virtual int LocatePositiveControlName (GenotypesForAMarkerSet* genotypes);

	virtual bool ComputeExtendedLocusTimes (CoreBioComponent* grid, CoordinateTransform* inverseTransform);

	virtual double GetWidthAtTime (double t) const;
	virtual double GetSecondaryContentAtTime (double t) const;

	virtual ChannelData* GetNewLaneStandardChannel (int channel, LaneStandard* inputLS) = 0;
	virtual ChannelData* GetNewDataChannel (int channel, LaneStandard* inputLS) = 0;
	virtual ChannelData* GetNewGridDataChannel (int channel, LaneStandard* inputLS) = 0;
	virtual CoreBioComponent* CreateNewTransformedBioComponent (const CoreBioComponent& bc, CoordinateTransform* trans) = 0;

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;


	// Pre-Legacy Message functions*************************************************************************************************************
	//******************************************************************************************************************************************

	void ReportSampleData (RGTextOutput& ExcelText);


	// Legacy Message functions*****************************************************************************************************************
	//******************************************************************************************************************************************

	int AddNoticeToList (Notice* newNotice);	//$
	Boolean IsNoticeInList (const Notice* target);	//$
	Notice* GetNotice (const Notice* target);	//$
	Boolean ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink = FALSE);	//$
	Boolean ReportXMLNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);	//$
	void ClearNoticeObjects ();	//$
	int NumberOfNoticeObjects ();	//$
	RGString IsNoticeInAnyLocusList (const Notice* target);	// Not used
	RGString IsNoticeInAnyChannelList (const Notice* target);	// Not used

	void UpdateNoticesFromLoci ();	// Not used
	Boolean ReportAllNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLinks = FALSE);	//$
	virtual int MakePreliminaryCalls (GenotypesForAMarkerSet* pGenotypes);	//$

	void SetNegativeControlTrue () { mIsNegativeControl = true; }	//$
	void SetNegativeControlFalse () { mIsNegativeControl = false; }	//$

	void SetPositiveControlTrue () { mIsPositiveControl = true; }	//$
	void SetPositiveControlFalse () { mIsPositiveControl = false; }	//$

	void ReportXMLGridTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText);	//$
	void ReportXMLSampleTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText);	//$
	void ReportPullupMatrix (int pass); //$

	virtual int Initialize (SampleData& fileData, PopulationCollection* collection, const RGString& markerSetName, Boolean isGrid);	//$
	virtual int SetAllData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int SetAllRawData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);	//$

	virtual int FitAllCharacteristics (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int FitNonLaneStandardCharacteristics (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int AssignSampleCharacteristicsToLoci (CoreBioComponent* grid, CoordinateTransform* timeMap);	//$

	virtual int AnalyzeLaneStandardChannel (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int AssignCharacteristicsToLoci ();	//$
	virtual int AnalyzeGridLoci (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int AnalyzeSampleLoci (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int AnalyzeCrossChannel ();	//$
	virtual int OrganizeNoticeObjects ();	//$

	virtual int SetLaneStandardData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int FitLaneStandardCharacteristics (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$

	//  The following method assumes that Initialize and SetAllData have both been called.  These should become the public interface!!!
	virtual int AnalyzeGrid (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$

	//  The following two methods are complete.  No prior CoreBioComponent methods need to be called.  All that is required is that
	//  the structs gridData or sampleData be initialized.  These structs can be initialized once, independently of the input file
	//  data.
	virtual int AnalyzeGrid (SampleData& fileData, GridDataStruct* gridData);	//$
	virtual int PrepareSampleForAnalysis (SampleData& fileData, SampleDataStruct* sampleData);	//$

	virtual int ResolveAmbiguousInterlocusSignals ();	//$
	virtual int SampleQualityTest (GenotypesForAMarkerSet* genotypes);	//$
	virtual int TestPositiveControl (GenotypesForAMarkerSet* genotypes);	//$
	
	virtual int GridQualityTest ();	//$
	virtual int FilterNoticesBelowMinBioID ();	//$
	virtual int RemoveAllSignalsOutsideLaneStandard ();	//$
	virtual int PreliminarySampleAnalysis (RGDList& gridList, SampleDataStruct* sampleData);	//$

	virtual int WriteXMLGraphicData (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro);	//$
	virtual int WritePeakInfoToXMLForChannel (int channel, RGTextOutput& text, const RGString& indent, const RGString& tagName);	//$
	virtual int WriteArtifactInfoToXMLForChannel (int channel, RGTextOutput& text, const RGString& indent);	//$

	

	//******************************************************************************************************************************************
	//******************************************************************************************************************************************

	// Could be made into Smart Message Version, but not needed at present**********************************************************************
	//******************************************************************************************************************************************

	virtual int PrepareForILSAnalysis (SampleData& fileData, SampleDataStruct* sampleData);

	//******************************************************************************************************************************************
	//******************************************************************************************************************************************
	// Smart Message Functions************************************************************************************************************
	//************************************************************************************************************************************

	virtual int GetObjectScope () const { return 4; }

	bool EvaluateSmartMessagesForStage (int stage, bool allMessages, bool signalsOnly);
	bool EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly);
	bool EvaluateSmartMessagesAndTriggersForStage (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly);
	bool SetTriggersForAllMessages (bool* const higherMsgMatrix, int stage, bool allMessages, bool signalsOnly);
	bool SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly);
	bool EvaluateAllReports (bool* const reportMatrix);
	bool TestAllMessagesForCall ();
	bool EvaluateAllReportLevels (int* const reportLevelMatrix);

	Boolean ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLinks = FALSE);
	Boolean ReportAllSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink = FALSE);
	bool ReportXMLSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);

	virtual int AddAllSmartMessageReporters ();
	virtual int AddAllSmartMessageReportersForSignals ();

	virtual int AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects);
	virtual int AddAllSmartMessageReportersForSignals (SmartMessagingComm& comm, int numHigherObjects);

	void SetNegativeControlTrueSM ();
	void SetNegativeControlFalseSM ();

	void SetPositiveControlTrueSM ();
	void SetPositiveControlFalseSM ();

	void SetPossibleMixtureIDTrueSM () { mSatisfiesPossibleMixtureCriteria = true; }
	void SetPossibleMixtureIDFalseSM () { mSatisfiesPossibleMixtureCriteria = false; }

	virtual int TestFractionalFiltersSM ();
	virtual int MakePreliminaryCallsSM (GenotypesForAMarkerSet* pGenotypes);

	void ReportXMLSmartGridTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText);
	void ReportXMLSmartSampleTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText);
	void ReportXMLSampleInfoBlock (const RGString& indent, RGTextOutput& text);

	bool GetIgnoreNoiseAboveDetectionInSmoothingFlag () const;

	void WriteDataToHeightFileSM ();

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);

	virtual void MakeNonCoreLadderArtifactsNoncritical () {}

	virtual int InitializeSM (SampleData& fileData, PopulationCollection* collection, const RGString& markerSetName, Boolean isGrid);
	virtual int SetAllDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);
	virtual int SetAllRawDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);
	virtual int SetAllRawDataWithMatrixSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);

	virtual int FitAllCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int FitNonLaneStandardCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int FitNonLaneStandardNegativeCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AssignSampleCharacteristicsToLociSM (CoreBioComponent* grid, CoordinateTransform* timeMap);

	virtual int AnalyzeLaneStandardChannelSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AssignCharacteristicsToLociSM ();
	virtual int AnalyzeGridLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AnalyzeSampleLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AnalyzeCrossChannelSM ();
	virtual int AnalyzeCrossChannelWithNegativePeaksSM ();
	virtual int AnalyzeCrossChannelUsingPrimaryWidthAndNegativePeaksSM ();
	virtual int UseChannelPatternsToAssessCrossChannelWithNegativePeaksSM (RGDList*** notPrimaryLists);
	virtual bool CollectDataAndComputeCrossChannelEffectForChannelsSM (int primaryChannel, int pullupChannel, RGDList* primaryChannelPeaks, double& linearPart, double& quadraticPart, bool testLaserOffScale, bool testNegativePUOnly);
	virtual bool NegatePullupForChannelsSM (int primaryChannel, int pullupChannel, list<PullupPair*>& pairList, bool testLaserOffScale);
	virtual DataSignal** CollectAndSortPullupPeaksSM (DataSignal* primarySignal, RGDList& pullupSignals);
	virtual bool AcknowledgePullupPeaksWhenThereIsNoPatternSM (int primaryChannel, int secondaryChannel, bool testLaserOffScale);

	virtual int OrganizeNoticeObjectsSM ();
	virtual int TestSignalsForLaserOffScaleSM ();
	virtual int PreTestSignalsForLaserOffScaleSM ();

	virtual void ReevaluateNoiseThresholdBasedOnMachineType (const RGString& machine) {;}

	void RemovePrimaryLinksAndSecondaryLinksFrom (DataSignal* ds);
	virtual bool ValidateAndCorrectCrossChannelAnalysesSM ();
	void RemovePeakAsPrimarySM (DataSignal* ds);

	virtual int SetLaneStandardDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);
	virtual int FitLaneStandardCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int FitAllSampleCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	//  The following method assumes that Initialize and SetAllData have both been called.  These should become the public interface!!!
	virtual int AnalyzeGridSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	//  The following two methods are complete.  No prior CoreBioComponent methods need to be called.  All that is required is that
	//  the structs gridData or sampleData be initialized.  These structs can be initialized once, independently of the input file
	//  data.
	virtual int AnalyzeGridSM (SampleData& fileData, GridDataStruct* gridData);
	virtual int PrepareSampleForAnalysisSM (SampleData& fileData, SampleDataStruct* sampleData);
	virtual int NormalizeBaselineForNonILSChannelsSM ();

	virtual int ResolveAmbiguousInterlocusSignalsSM ();
	virtual int SampleQualityTestSM (GenotypesForAMarkerSet* genotypes);
	virtual int SignalQualityTestSM ();
	virtual bool IsLabPositiveControl (const RGString& name, GenotypesForAMarkerSet* genotypes);
	virtual int TestPositiveControlSM (GenotypesForAMarkerSet* genotypes);
	virtual int GridQualityTestSM ();
	virtual int GridQualityTestSMPart2 (SmartMessagingComm& comm, int numHigherObjects);
	virtual int FilterSmartNoticesBelowMinBioID ();
	virtual int RemoveAllSignalsOutsideLaneStandardSM ();
	virtual int PreliminarySampleAnalysisSM (RGDList& gridList, SampleDataStruct* sampleData);
	virtual int MeasureAllInterlocusSignalAttributesSM ();
	virtual int ResolveAmbiguousInterlocusSignalsUsingSmartMessageDataSM ();
	virtual int RemoveInterlocusSignalsSM ();

	virtual int WriteXMLGraphicDataSM (const RGString& graphicDirectory, const RGString& localFileName, SampleData* data, int analysisStage, const RGString& intro);
	virtual int WriteSmartPeakInfoToXMLForChannel (int channel, RGTextOutput& text, const RGString& indent, const RGString& tagName);
	virtual int WriteSmartArtifactInfoToXMLForChannel (int channel, RGTextOutput& text, const RGString& indent);

	virtual void InitializeMessageData ();


	//************************************************************************************************************************************
	//************************************************************************************************************************************

	// Static and friend functions********************************************************************************************************

	static void SetSearchByName () { SearchByName = TRUE; }
	static void SetSearchByTime () { SearchByName = FALSE; }

	static void SetGaussianSignature () { GaussianSignature = TRUE; }
	static void SetOtherSignature () { GaussianSignature = FALSE; }
	static Boolean GetGaussianSignature () { return GaussianSignature; }

	static void SetUseRawData () { UseRawData = TRUE; }
	static void DontUseRawData () { UseRawData = FALSE; }
	static Boolean GetUseRawData () { return UseRawData; }

	static void AddNoticeToChannelArtifactTestList (Notice* target) { testChannelArtifactNoticeList.Append (target); }
	static void ClearAllTestNotices () { testChannelArtifactNoticeList.ClearAndDelete (); }

	static int GetMinBioIDForArtifacts () { return minBioIDForArtifacts; }
	static void SetMinBioIDForArtifacts (int minBioID) { minBioIDForArtifacts = minBioID; }
	static double GetMinBioIDForLadderLoci () { return minBioIDForLadderLoci; }
	static void SetMinBioIDForLadderLoci (double minBioID) { minBioIDForLadderLoci = minBioID; }
	static bool SignalIsWithinAnalysisRegion (DataSignal* testSignal, double firstILSTime);

	friend CSplineTransform* TimeTransform (const CoreBioComponent& cd1, const CoreBioComponent& cd2);
	friend CSplineTransform* TimeTransform (const CoreBioComponent& cd1, const CoreBioComponent& cd2, bool useHermiteSplines);
	friend CSplineTransform* TimeTransform (const CoreBioComponent& cd1, const CoreBioComponent& cd2, bool useHermiteSplines, bool useChords);

	static void CreateInitializationData (int scope);
	static void InitializeMessageMatrix (bool* matrix, int size);
	static void ClearInitializationMatrix () { delete[] InitialMatrix; InitialMatrix = NULL; }
	static int GetScope () { return 4; }
	static bool TestForOffScale (double time);

	static void SetILSDyeName (const RGString& name) { ILSDyeName = name; }
	static RGString GetILSDyeName () { return ILSDyeName; }
	static void BuildDyeNameArray (int nChannels) { DyeNames = new RGString [nChannels + 1]; }
	static void SetDyeName (int i, const RGString& name) { DyeNames [i] = name; }
	static RGString GetDyeName (int i) { return DyeNames [i]; }
	static bool DyeNamesUnset () { return (DyeNames == NULL); }

	static void SetOffScaleDataLength (int length) { OffScaleDataLength = length; }
	static void SetHeightFile (RGTextOutput* hf) { HeightFile = hf; }
	static void SetNonLaserOffScalePUCoeffsFile (RGTextOutput* puf) { NonLaserOffScalePUCoefficients = puf; }
	static void SetPullupMatrixFile (RGTextOutput* pumf) { pullUpMatrixFile = pumf; }

	//************************************************************************************************************************************

protected:
	RGString mName;
	RGString mFileName;
	RGString mSampleName;
	RGString mControlIdName;
	PackedTime mTime;
	PackedDate mDate;
	RGString mRunStart;
	ChannelData** mDataChannels;
	int mNumberOfChannels;
	PopulationMarkerSet* mMarkerSet;  // this should be a new copy
	int mLaneStandardChannel;
	TestCharacteristic* mTest;  // ???????
	ChannelData* mLSData;

	LaneStandard* mLaneStandard;  // DON'T DELETE!!!

	RGString ErrorString;
	int Progress;

	RGDList NewNoticeList;
	RGString mTableLink;

	bool mIsNegativeControl;
	bool mIsPositiveControl;
	bool mSatisfiesPossibleMixtureCriteria;

	CoreBioComponent* mAssociatedGrid;
	list<CompoundSignalInfo*> mSignalLinkList;

	RGString mPositiveControlName;
	list<InterchannelLinkage*> mInterchannelLinkageList;

	bool** mPullupTestedMatrix;
	double** mLinearPullupMatrix;
	double** mQuadraticPullupMatrix;
	double** mLeastMedianValue;
	double** mOutlierThreshold;

	CSplineTransform* mTimeMap;

	// QC metrics

	QCData mQC;
	RGString mComments;

	//double mMaxLinearPullupCoefficient;
	//double mMaxNonlinearPullupCoefficient;
	//double mMaxErrorInBP;
	//double mWidthOfLastILSPeak;
	//double mSampleLocusTotalAreaRatioMaxToMin;
	//double mStartingTemperature;
	//double mMaxMinusMinTemperature;
	//double mStartingVoltage;
	//double mMaxMinusMinVoltage;
	//double mStartingCurrent;
	//double mMaxMinusMinCurrent;
	//double mStartingPower;
	//double mMaxMinusMinPower;

	// Smart Message Data*****************************************************************************************************************
	//************************************************************************************************************************************


	RGDList mChannelList;

	//************************************************************************************************************************************
	//************************************************************************************************************************************

	static Boolean SearchByName;
	static Boolean GaussianSignature;
	static Boolean UseRawData;
	static RGDList testChannelArtifactNoticeList;
	static int minBioIDForArtifacts;
	static double minBioIDForLadderLoci;
	static bool* InitialMatrix;
	static bool* OffScaleData;
	static int OffScaleDataLength;
	static double minPrimaryPullupThreshold;
	static bool UseHermiteTimeTransforms;
	static bool UseNaturalCubicSplineTimeTransform;
	static RGString ILSDyeName;
	static RGString* DyeNames;
	static RGTextOutput* HeightFile;
	static RGTextOutput* NonLaserOffScalePUCoefficients;
	static RGTextOutput* pullUpMatrixFile;

	static int InitializeOffScaleData (SampleData& sd);
	static void ReleaseOffScaleData ();


	// Smart Message Functions************************************************************************************************************
	//************************************************************************************************************************************


	//************************************************************************************************************************************
	//************************************************************************************************************************************
};


/*
class KnownBioSample : public CoreBioComponent {

	ABSTRACT_DECLARATION (KnownBioSample)

public:
	KnownBioSample ();
	virtual ~KnownBioSample () = 0;

//	virtual int CompareTo (const RGPersistent* p) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:

};


class NegativeControl : public KnownBioSample {

	ABSTRACT_DECLARATION (NegativeControl)

public:
	NegativeControl ();
	virtual ~NegativeControl () = 0;

//	virtual int CompareTo (const RGPersistent* p) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:

};


class PositiveControl : public KnownBioSample {

	ABSTRACT_DECLARATION (PositiveControl)

public:
	PositiveControl ();
	virtual ~PositiveControl () = 0;

//	virtual int CompareTo (const RGPersistent* p) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:

};


class OtherKnownSample : public KnownBioSample {

	ABSTRACT_DECLARATION (OtherKnownSample)

public:
	OtherKnownSample ();
	virtual ~OtherKnownSample () = 0;

//	virtual int CompareTo (const RGPersistent* p) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:

};


class ControlGrid : public KnownBioSample {

	ABSTRACT_DECLARATION (ControlGrid)

public:
	ControlGrid ();
	virtual ~ControlGrid () = 0;

//	virtual int CompareTo (const RGPersistent* p) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:

};


class UnknownBioSample : public CoreBioComponent {

	ABSTRACT_DECLARATION (UnknownBioSample)

public:
	UnknownBioSample ();
	virtual ~UnknownBioSample () = 0;

//	virtual int CompareTo (const RGPersistent* p) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:

};*/


#endif  /*  _COREBIOCOMPONENT_H_  */

