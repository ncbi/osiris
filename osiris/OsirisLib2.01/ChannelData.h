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
*  FileName: ChannelData.h
*  Author:   Robert Goor
*
*/
//
//     Abstract base class ChannelData for retaining data pertaining to one data channel, such as the sampled data
//  associated with that channel
//

#ifndef _CHANNELDATA_H_
#define _CHANNELDATA_H_

#include "rgpersist.h"
#include "rgdefs.h"
#include "rgdlist.h"
#include "Genetics.h"
#include "coordtrans.h"
#include "DataSignal.h"
#include "Notice.h"
#include "SmartMessagingObject.h"
#include "rgtptrdlist.h"


class SampleData;
class TestCharacteristic;
class TestCharacteristicSet;
class RGTextOutput;
class OsirisMsg;
class CoreBioComponent;
class RGString;
class IndividualGenotype;
class SmartMessageReporter;


class RaisedBaseLineData {

public:
	RaisedBaseLineData () : mHeight (0.0), mLeft (0), mRight (0) {}
	RaisedBaseLineData (double height, int left, int right): mHeight (height), mLeft (left), mRight (right) {}
	~RaisedBaseLineData () {}

	friend bool operator== (const RaisedBaseLineData& first, const RaisedBaseLineData& second);

	double GetHeight () const { return mHeight; }
	int GetLeft () const { return mLeft; }
	int GetRight () const { return mRight; }

protected:
	double mHeight;
	int mLeft;
	int mRight;
};


class PoorFitPeakData {

public:
	PoorFitPeakData () : mHeight (0.0), mMode (0) {}
	PoorFitPeakData (double height, int mode): mHeight (height), mMode (mode) {}
	~PoorFitPeakData () {}

	friend bool operator== (const PoorFitPeakData& first, const PoorFitPeakData& second);

	double GetHeight () const { return mHeight; }
	int GetMode () const { return mMode; }

protected:
	double mHeight;
	int mMode;
};



class ChannelData : public SmartMessagingObject {

	ABSTRACT_DECLARATION (ChannelData)

public:
	ChannelData ();
	ChannelData (int channel);
	ChannelData (int channel, LaneStandard* inputLS);
	ChannelData (const ChannelData& cd);
	ChannelData (const ChannelData& cd, CoordinateTransform* trans);
	virtual ~ChannelData () = 0;


	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************

	void SetTableLink (int linkNumber);
	RGString GetTableLink () const { return mTableLink; }

	Boolean IsValid () const { return Valid; }
	RGString GetError () const { return ErrorString; }

	int GetChannel () const { return mChannel; }
	void SetChannel (int channel) { mChannel = channel; }
	void SetFsaChannel (int channel) { mFsaChannel = channel; }

	double GetFirstAnalyzedMean () const { if (Means != NULL) return Means [0]; return -DOUBLEMAX; }
	double GetLastAnalyzedMean () const { if (Means != NULL) return Means [NumberOfAcceptedCurves-1]; return -DOUBLEMAX; }

	RGDList& GetFinalCurveList () { return FinalCurveList; }
	RGDList& GetMarginalCurveList () { return MarginalCurveList; }
	RGDList& GetArtifactList () { return ArtifactList; }
	RGDList& GetCompleteCurveList () { return CompleteCurveList; }
	RGDList& GetBleedThroughCandidateList () { return BleedThroughCandidateList; }
	RGDList& GetPreliminaryCurveList () { return PreliminaryCurveList; }

	int GetNumberOfSamples () const;
	double GetLeftEndpoint () const;
	double GetRightEndpoint () const;
	int GetNumberOfSignals () const { return CompleteCurveList.Entries (); }
	bool HasPrimerPeaks (ChannelData* laneStd);

	void SetCompleteSignalListSequence ();

	int GetHighestMessageLevel () const { return mHighestMessageLevel; }

	double GetBeginAnalysisTime () const { return mBeginAnalysis; }
	double GetNoiseRange () const { return mData->GetNoiseRange (); }
	double EvaluateBaselineAtTime (double time);

	int CreateAndSubstituteSinglePassFilteredSignalForRawData (int window);
	int CreateAndSubstituteTriplePassFilteredSignalForRawData (int window);
	int RestoreRawDataAndDeleteFilteredSignal ();
	bool HasFilteredData () const;
	
	int GetHighestMessageLevelFromLoci ();
	Locus* FindLocus (const RGString& locusName);
	double TotalPeakHeight (int& numberOfPeaks);
	virtual int AddLocus (Locus* locus);
	virtual int SetAllApproximateIDs (ChannelData* laneStd);

	virtual bool ChannelIsValid () const { if (mData == NULL) return false; return true; }
	virtual bool IsControlChannel () const { return false; }

	virtual bool TestInterlocusSignalHeightBelowThreshold (DataSignal* signal);

	virtual double GetWidthAtTime (double t) const;
	virtual double GetSecondaryContentAtTime (double t) const;
	virtual double GetTimeForSpecifiedID (double id);
	virtual CoordinateTransform* GetIDMap ();
	virtual int FindAndRemoveFixedOffset ();
	virtual int ProjectNeighboringSignalsAndTest (double horizontalResolution, double verticalTolerance);
	
	virtual bool TestIfTimeIsLeftOfLocus (double time, Locus* locus, Locus* assocGridLocus);

	static void SetMinimumHeight (double height) { DataSignal::SetMinimumHeight (height); }
	virtual double GetMinimumHeight () const = 0;
	virtual double GetMaximumHeight () const = 0;
	virtual double GetFractionalFilter () const;
	virtual double GetDetectionThreshold () const = 0;

	virtual double GetMeasurementRatio () const;
	virtual void AppendAllBaseLoci (RGDList& locusList);

	void ResetPreliminaryIterator () { PreliminaryIterator.Reset (); }
	DataSignal* GetNextPreliminaryCurve () { return (DataSignal*) PreliminaryIterator (); }
	DataSignal* RemovePreliminaryCurveReference (DataSignal* target) { return (DataSignal*) PreliminaryCurveList.RemoveReference (target); }

	void ResetNegativeCurveIterator () { NegativeCurveIterator.Reset (); }
	DataSignal* GetNextNegativeCurve () { return (DataSignal*) NegativeCurveIterator (); }
	DataSignal* RemoveNegativeCurveReference (DataSignal* target) { return (DataSignal*) mNegativeCurveList.RemoveReference (target); }

	void ResetCompleteIterator () { CompleteIterator.Reset (); }
	DataSignal* GetNextCompleteCurve () { return (DataSignal*) CompleteIterator (); }
	DataSignal* RemoveCompleteCurveReference (DataSignal* target) { return (DataSignal*) CompleteCurveList.RemoveReference (target); }

	void InsertIntoArtifactList (DataSignal* target) { ArtifactList.InsertWithNoReferenceDuplication (target); }
	void InsertIntoPreliminaryCurveList (DataSignal* target) { PreliminaryCurveList.InsertWithNoReferenceDuplication (target); }
	void InsertIntoCompleteCurveList (DataSignal* target) { CompleteCurveList.InsertWithNoReferenceDuplication (target); }

	void ResetPreliminarySignalSearch () { PreliminaryIterator.Reset (); PreliminaryIterator (); }
	virtual DataSignal* FindPreliminarySignalWithinToleranceOf (DataSignal* target, double tolerance);
	virtual bool ComputeExtendedLocusTimes (CoreBioComponent* grid, CoordinateTransform* inverseTransform, CoreBioComponent* associatedGrid);

	virtual ChannelData* CreateNewTransformedChannel (const ChannelData& cd, CoordinateTransform* trans) = 0;

	virtual bool AddILSToHistoryList ();
	virtual ILSCandidate* TestILSStartAndEndSignals (DataSignal* startSignal, DataSignal* endSignal) { return NULL; }
	virtual bool TestAllILSStartAndEndSignals (RGDList& finalILSPeaks, double& correlation) { return false; }
	virtual void ResetBoundsUsingFactorToILSHistory (double factor) {};

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual int WriteRawDataAndFitCurveToOutputRows (RGTextOutput& text, const RGString& delim, const RGString& dyeName, const RGString& sampleName);
	virtual int WriteLine (RGTextOutput& text, const RGString& delim, const DataSignal* ds);
	virtual int WriteRawData (RGTextOutput& text, const RGString& delim);
	virtual int WriteFitData (RGTextOutput& text, const RGString& delim, bool useMaxValueMethod = false);
	virtual int WriteFitData (RGTextOutput& text, const RGString& delim, int numSamples, double left, double right, bool useMaxValueMethod = false);

	virtual int WriteBaselineData (RGTextOutput& text, const RGString& delim, const RGString& indent);
	
	virtual int WriteLocusInfoToXML (RGTextOutput& text, const RGString& indent);

	void ReportGridLocusRow (RGTextOutput& text);
	void ReportGridLocusRowWithLinks (RGTextOutput& text);

	// Pre-object Message functions************************************************************************************************************
	//*****************************************************************************************************************************************

	void ReportSampleData (RGTextOutput& ExcelText, const RGString& indent);
	void ReportArtifacts (RGTextOutput& ExcelText, const RGString& indent);


	// Unused...obsolete***********************************************************************************************************************
	//*****************************************************************************************************************************************

	RGString IsNoticeInAnyLocusList (const Notice* target);	// Not used

	virtual int AnalyzeLaneStandardChannel (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	// Not used

	virtual int TestArtifactListForNoticeWithinLaneStandard (const Notice* notice, ChannelData* laneStandard);	// Not used
	virtual int TestArtifactListForNoticesWithinLaneStandard (RGDList& listOfTargetNotices, ChannelData* laneStandard);	// Not used
	virtual int TestLociForInterLocusPeakWithinLaneStandard (DataSignal* signal, double left, double right, CoreBioComponent* assocGrid, CoordinateTransform* transform);	// Not used

	virtual int TestArtifactListForNotice (const Notice* notice);	// This is unused
	virtual int TestArtifactListForNotices (RGDList& listOfTargetNotices);	// This is unused
	virtual int AnalyzeCrossChannel (ChannelData** channels, int nChannels);	// Obsolete.  Replaced by CoreBioComponent method

	// Legacy Message functions*****************************************************************************************************************
	//******************************************************************************************************************************************

	int AddNoticeToList (Notice* newNotice);	//$
	Boolean IsNoticeInList (const Notice* target);	//$
	Notice* GetNotice (const Notice* target);
	Boolean ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink = FALSE);	//$
	void ClearNoticeObjects ();	//$
	int NumberOfNoticeObjects ();	//$

	Boolean ReportAllNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink = FALSE);	//$
	Boolean ReportXMLNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);	//$
	Boolean ReportXMLILSNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);	//$

	int CountSignalsWithNotice (const Notice* target, ChannelData* laneStd);	//$

	virtual void MakePreliminaryCalls (bool isNegCntl, bool isPosCntl, GenotypesForAMarkerSet* pGenotypes);	//$
	virtual int SetData (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int SetRawData (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);	//$

	virtual int FitAllCharacteristics (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$

	virtual int AssignSampleCharacteristicsToLoci (CoreBioComponent* grid, CoordinateTransform* timeMap);	//$

	virtual int AnalyzeLaneStandardChannelRecursively (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual Boolean AssignCharacteristicsToLoci (ChannelData* lsData);	//$
	virtual int AnalyzeGridLoci (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int AnalyzeSampleLoci (ChannelData* lsData, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int TestFractionalFilters ();	//$
	virtual int FinalTestForPeakSizeAndNumber (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes);	//$
	virtual int FinalTestForCriticalLaneStandardNotices ();	//$

	virtual int HierarchicalLaneStandardChannelAnalysis (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	virtual bool TestForArtifacts (DataSignal* currentSignal, double fit);	//$
	virtual bool TestForArtifacts (DataSignal* currentSignal, double fit, int dualSignal);	//$
	virtual int TestForDualPeak (double minRFU, double maxRFU, DataSignal* currentSignal, double currentFit, RGDList& previous, bool originalUnacceptable = false);	//$

	virtual int TestArtifactListForNoticesWithinLaneStandard (ChannelData* laneStandard, CoreBioComponent* associatedGrid);	//$

	virtual int TestForMultiSignals ();	//$
	virtual int TestProximityArtifacts ();	//$

	virtual int AssignSignalToFirstLocusAndDeleteFromSecond (DataSignal* target, Locus* locus1, Locus* locus2);	//$
	virtual int RemoveSignalFromBothLoci (DataSignal* target, Locus* locus1, Locus* locus2);	//$
	virtual bool TestIfSignalBelongsToFirstLocus (DataSignal* target, Locus* locus1, Locus* locus2);	//$
	virtual int ResolveAmbiguousInterlocusSignals ();	//$
	virtual int RemoveSignalsOutsideLaneStandard (ChannelData* laneStandard);	//$

	virtual int CorrectCrossChannelAnalyses (bool isNegCntl);	//$
	virtual int CorrectLaneStandardCrossChannelAnalysis ();	//$
	virtual int CleanUpLocusSignalLists ();	//$
	virtual int CorrectGridLocusSignals ();	//$
	virtual int CleanUpGridLocusSignals ();	//$

	virtual int RemoveInterlocusSignals (bool isNegCntl);	//$
	virtual int RemoveInterlocusSignals (bool isNegCntl, double left, double ilsLeft, double right, CoreBioComponent* assocGrid, CoordinateTransform* transform);	//$
	virtual int TestInterlocusCharacteristics (double left, double ilsLeft, double right, CoreBioComponent* assocGrid, ChannelData* laneStd, bool isNegCntl);	//$
	virtual int TestForInterlocusProximityArtifacts ();	//$
	virtual int TestPositiveControl (IndividualGenotype* genotype);	//$
	virtual int FilterNoticesBelowMinimumBioID (ChannelData* laneStd, int minBioID);	//$

	virtual int WritePeakInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& tagName) = 0;	//$
	virtual int WriteArtifactInfoToXML (RGTextOutput& text, const RGString& indent, ChannelData* laneStd) = 0;	//$

	//******************************************************************************************************************************************
	//******************************************************************************************************************************************

	// Smart Message functions*******************************************************************************
	//*******************************************************************************************************

	virtual int GetObjectScope () const { return 3; }

	bool EvaluateSmartMessagesForStage (int stage, bool allMessages, bool signalsOnly);
	bool EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly);
	bool SetTriggersForAllMessages (bool* const higherMsgMatrix, int stage, bool allMessages, bool signalsOnly);
	bool SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly);
	bool EvaluateSmartMessagesAndTriggersForStage (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly);
	bool EvaluateAllReports (bool* const reportMatrix);
	bool TestAllMessagesForCall ();
	bool EvaluateAllReportLevels (int* const reportLevelMatrix);

	bool ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, bool reportLink = false);

	virtual int AddAllSmartMessageReporters ();
	virtual int AddAllSmartMessageReportersForSignals ();

	virtual int AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects);
	virtual int AddAllSmartMessageReportersForSignals (SmartMessagingComm& comm, int numHigherObjects);

	Boolean ReportAllSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink = FALSE);
	Boolean ReportXMLSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);
	Boolean ReportXMLILSSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);

	int CountSignalsWithNoticeSM (const SmartNotice& target, ChannelData* laneStd);

	void AccumulatePeakHeightsForChannelAndAddToTotalsSM (double* totalArray, int nChannels);

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);
	virtual RGString GetDebugIDIndent () const;

	virtual void ChannelIsPositiveControlSM (bool isPosControl);
	virtual void ChannelIsNegativeControlSM (bool isNegControl);
	virtual void MakeNonCoreLadderArtifactsNoncritical () {}

	virtual void MakePreliminaryCallsSM (bool isNegCntl, bool isPosCntl, GenotypesForAMarkerSet* pGenotypes);
	virtual int SetDataSM (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataSM (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	virtual void ClearAllPeaksBelowAnalysisThreshold ();
	virtual int RemoveStutterLinksFromNonStutterPeaksSM () { return -1; }

	virtual int TestFitCriteriaSM (DataSignal* signal);
	virtual int FitAllCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int FitAllNegativeCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	virtual int AssignSampleCharacteristicsToLociSM (CoreBioComponent* grid, CoordinateTransform* timeMap);

	virtual int AnalyzeLaneStandardChannelRecursivelySM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual Boolean AssignCharacteristicsToLociSM (ChannelData* lsData);
	virtual int AnalyzeGridLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AnalyzeSampleLociSM (ChannelData* lsData, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int TestFractionalFiltersSM ();
	virtual int FinalTestForPeakSizeAndNumberSM (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes);
	virtual int FinalTestForCriticalLaneStandardNoticesSM ();

	virtual int HierarchicalLaneStandardChannelAnalysisSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	virtual bool TestForArtifactsSM (DataSignal* currentSignal, double fit);
	virtual bool TestForArtifactsSM (DataSignal* currentSignal, double fit, int dualSignal);
	virtual int TestForDualPeakSM (double minRFU, double maxRFU, DataSignal* currentSignal, double currentFit, RGDList& previous, bool originalUnacceptable = false);

	virtual int TestArtifactListForNoticesWithinLaneStandardSM (ChannelData* laneStandard, CoreBioComponent* associatedGrid);

	virtual int TestForMultiSignalsSM ();
	virtual int TestForAlleleDuplicationSM ();
	virtual int TestProximityArtifactsSM ();

	virtual int AssignSignalToFirstLocusAndDeleteFromSecondSM (DataSignal* target, Locus* locus1, Locus* locus2);
	virtual int RemoveSignalFromBothLociSM (DataSignal* target, Locus* locus1, Locus* locus2);
	virtual bool TestIfSignalBelongsToFirstLocusSM (DataSignal* target, Locus* locus1, Locus* locus2);
	virtual int ResolveAmbiguousInterlocusSignalsSM ();
	virtual int MeasureInterlocusSignalAttributesSM ();
	virtual int ResolveAmbiguousInterlocusSignalsUsingSmartMessageDataSM ();
	virtual int RemoveSignalsOutsideLaneStandardSM (ChannelData* laneStandard);

	virtual bool FindLimitsOnPrimaryPullupPeaks ();	// returns true if it made a change in the channel
	virtual bool ValidateAndCorrectCrossChannelAnalysisSM ();	// returns true if it made a change in the channel

	virtual int CorrectCrossChannelAnalysesSM (bool isNegCntl);
	virtual int CorrectLaneStandardCrossChannelAnalysisSM ();
	virtual int CleanUpLocusSignalListsSM ();
	virtual int CorrectGridLocusSignalsSM ();
	virtual int CleanUpGridLocusSignalsSM ();

	virtual int RemoveInterlocusSignalsSM (bool isNegCntl);
	virtual int TestSampleInterlocusMinRFUSM (bool isNegCntl, double left, double ilsLeft, double right, CoreBioComponent* assocGrid, CoordinateTransform* transform);
	virtual int RemoveInterlocusSignalsSM (double left, double ilsLeft, double right, CoreBioComponent* assocGrid);
	virtual int TestInterlocusCharacteristicsSM (double left, double ilsLeft, double right, CoreBioComponent* assocGrid, ChannelData* laneStd, bool isNegCntl);
	virtual int TestForInterlocusProximityArtifactsSM ();
	virtual int TestPositiveControlSM (IndividualGenotype* genotype);
	virtual int FilterSmartNoticesBelowMinimumBioID (ChannelData* laneStd, int minBioID);
	virtual int AppendDataForStutterPeaksSM () { return -1; }

	virtual int WriteSmartPeakInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& tagName) = 0;
	virtual int WriteSmartArtifactInfoToXML (RGTextOutput& text, const RGString& indent, ChannelData* laneStd) = 0;
	virtual void TestAllelesAgainstOverloadThresholdSM ();

	virtual int TestSignalsForOffScaleSM ();
	virtual int PreTestForSignalOffScaleSM ();
	virtual int TestSignalsForRaisedBaseline (double left, double report);
	virtual int TestForRaisedBaselineAndExcessiveNoiseSM (double left, double report);

	virtual void InitializeMessageData ();

	virtual int AnalyzeDynamicBaselineSM (int startTime, double reportMinTime);
	virtual int AnalyzeDynamicBaselineAndNormalizeRawDataSM (int startTime, double reportMinTime);

	virtual void GetCharacteristicArray (const double*& array) const { if (mLaneStandard != NULL) mLaneStandard->GetCharacteristicArray (array); }


	//*******************************************************************************************************
	//*******************************************************************************************************

	friend CSplineTransform* TimeTransform (const ChannelData& cd1, const ChannelData& cd2);
	friend CSplineTransform* TimeTransform (const ChannelData& cd1, const ChannelData& cd2, double* firstDerivs, int size);
	friend CSplineTransform* TimeTransform (const ChannelData& cd1, const ChannelData& cd2, bool isHermite);

	static double GetMinimumDistanceBetweenPeaks () { return MinDistanceBetweenPeaks; }
	static void SetMinimumDistanceBetweenPeaks (double distance) { MinDistanceBetweenPeaks = distance; }
	static void CreateInitializationData (int scope);
	static void InitializeMessageMatrix (bool* matrix, int size);
	static void ClearInitializationMatrix () { delete[] InitialMatrix; InitialMatrix = NULL; }
	static int GetScope () { return 3; }

	static void SetAveSecondsPerBP (double s) { AveSecondsPerBP = s; }
	static double GetAveSecondsPerBP () { return AveSecondsPerBP; }

	static void SetUseNormalizationFilter (bool onOff) { UseFilterForNormalization = onOff; }
	static bool GetUseNormalizationFilter () { return UseFilterForNormalization; }

	static void SetDisableStutterFilter (bool s) { DisableStutterFilter = s; }
	static void SetDisableAdenylationFilter (bool s) { DisableAdenylationFilter = s; }

	static void SetTestForDualSignal (bool s) { TestForDualSignal = s; }
	static void SetUseILSHistory (bool s) { UseILSHistory = s; }
	static void SetLatitudeFactorForILSHistory (double factor) { LatitudeFactorForILSHistory = factor; }
	static double GetLatitudeFactorForILSHistory () { return LatitudeFactorForILSHistory; }

protected:
	int mChannel;
	DataSignal* mData;  // This is really SampledData*
	DataSignal* mBackupData;	// This is really SampledData*
	TestCharacteristic* mTestPeak;
	Boolean Valid;

	RGDList mLocusList;

	RGDList PreliminaryCurveList;
	RGDList FinalCurveList;
	RGDList MarginalCurveList;
	RGDList ArtifactList;
	RGDList CompleteCurveList;
	RGDList SmartPeaks;
	RGDList BleedThroughCandidateList;
	RGDList SupplementalArtifacts;
	RGDListIterator PreliminaryIterator;
	RGDListIterator CompleteIterator;
	RGDList mNegativeCurveList;
	RGDListIterator NegativeCurveIterator;

	int NumberOfAcceptedCurves;

	int SetSize;
	int MaxCorrelationIndex;

	double* Means;
	double* Sigmas;
	double* Fits;
	double* Peaks;
	double* SecondaryContent;

	LaneStandard* mLaneStandard;  // DON'T DELETE!!!
	Locus mTargetLocus;
	RGString ErrorString;

	double mBeginAnalysis;

	RGDList NewNoticeList;
	RGString mTableLink;
	bool mDeleteLoci;
	int mFsaChannel;

	RGTPtrDList<RaisedBaseLineData> mRaisedBaseLines;
	RGTPtrDList<PoorFitPeakData> mPoorFits;

	CSplineTransform* mBaseLine;
	int mBaselineStart;
	CoordinateTransform* mTimeMap;

	static double MinDistanceBetweenPeaks;
	static bool* InitialMatrix;
	static double AveSecondsPerBP;
	static bool UseFilterForNormalization;
	static bool DisableStutterFilter;
	static bool DisableAdenylationFilter;
	static bool TestForDualSignal;
	static bool UseILSHistory;
	static double LatitudeFactorForILSHistory;
};


#endif  /*  _CHANNELDATA_H_  */


