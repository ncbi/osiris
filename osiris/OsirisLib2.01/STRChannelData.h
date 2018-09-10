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
*  FileName: STRChannelData.h
*  Author:   Robert Goor
*
*/
//
//     class STRChannelData is the prototype channel for STR forensics.  It maintains a SampledData object and analyzes it.  Then, there
//  are the subclasses for Internal Lane Standard channels, ladder channels and sample channels
//

#ifndef _STRCHANNELDATA_H_
#define _STRCHANNELDATA_H_


#include "ChannelData.h"
#include "Genetics.h"
#include "Quadratic.h"
#include "coordtrans.h"
#include "ParameterServer.h"

#include <vector>
#include <functional>
#include <algorithm>

class TestCharacteristic;
class LaneStandard;
class CoreBioComponent;

const int _STRCHANNELDATA_ = 1050;
const int _STRLANESTANDARDCHANNELDATA_ = 1052;
const int _STRLADDERCHANNELDATA_ = 1053;
const int _STRSAMPLECHANNELDATA_ = 1054;


PERSISTENT_PREDECLARATION (STRChannelData)
PERSISTENT_PREDECLARATION (STRLaneStandardChannelData)
PERSISTENT_PREDECLARATION (STRLadderChannelData);
PERSISTENT_PREDECLARATION (STRSampleChannelData);


struct DistanceElement {

public:
	DistanceElement () : mDistance (0.0), mRow (0), mColumn (0) {}
	DistanceElement (double dist, int i, int j) : mDistance (dist), mRow (i), mColumn (j) {}

	double mDistance;
	int mRow;
	int mColumn;
};



bool cmpLess (const DistanceElement* a, const DistanceElement* b);



class STRChannelData : public ChannelData {

	PERSISTENT_DECLARATION (STRChannelData)

public:
	STRChannelData ();
	STRChannelData (int channel);
	STRChannelData (int channel, LaneStandard* inputLS);
	STRChannelData (const STRChannelData& strCD);
	STRChannelData (const STRChannelData& strCD, CoordinateTransform* trans);
	virtual ~STRChannelData ();

	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************
	
	void PrecomputeAverages (RGDList& set);
	virtual ChannelData* CreateNewTransformedChannel (const ChannelData& cd, CoordinateTransform* trans);
	virtual double GetMinimumHeight () const;
	virtual double GetMaximumHeight () const;
	virtual double GetDetectionThreshold () const;

	// Obsolete...not used**********************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int AnalyzeCrossChannel (ChannelData** channels, int nChannels);


	// Legacy Message functions*****************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int SetData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int SetRawData (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);	//$
	int TestNeighbors (DataSignal* previous, DataSignal* testSignal, DataSignal* following);	//$

	virtual int WritePeakInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& tagName);	//$
	virtual int WriteArtifactInfoToXML (RGTextOutput& text, const RGString& indent, ChannelData* laneStd);	//$


	//******************************************************************************************************************************************
	//******************************************************************************************************************************************

	// Smart Message functions******************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int SetDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	int TestNeighborsSM (DataSignal* previous, DataSignal* testSignal, DataSignal* following);

	virtual int TestSignalsForOffScaleSM ();
	virtual int PreTestForSignalOffScaleSM ();

	virtual bool FindLimitsOnPrimaryPullupPeaks ();	// returns true if it made a change in the channel
	virtual bool ValidateAndCorrectCrossChannelAnalysisSM ();	// returns true if it made a change in the channel

	virtual int WriteSmartPeakInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& tagName);
	virtual int WriteSmartArtifactInfoToXML (RGTextOutput& text, const RGString& indent, ChannelData* laneStd);

	virtual int AnalyzeDynamicBaselineSM (int startTime, double reportMinTime);
	virtual int AnalyzeDynamicBaselineAndNormalizeRawDataSM (int startTime, double reportMinTime);


	//******************************************************************************************************************************************


protected:
	double averageWidth;
	double averagePeak;
	double MinimumFractionOfAverageWidth;
	double MaximumMultipleOfAverageWidth; 
	double MinimumFractionOfAveragePeak;
	double MaximumMultipleOfAveragePeak;

	static bool UseHermiteCubicSplineForNormalization;
};


class STRLaneStandardChannelData : public STRChannelData {

	PERSISTENT_DECLARATION (STRLaneStandardChannelData)

public:
	STRLaneStandardChannelData ();
	STRLaneStandardChannelData (int channel);
	STRLaneStandardChannelData (int channel, LaneStandard* inputLS);
	STRLaneStandardChannelData (const STRLaneStandardChannelData& strLS);
	virtual ~STRLaneStandardChannelData ();

	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************

	void ClearAndRepopulateFromList (RGDList& sourceList, RGDList& curveList, RGDList& overFlowList);
	int SelectClusterSubset (RGDList& incomingList, RGDList& outgoingList, int Size, RGTextOutput& text, RGTextOutput& ExcelText);
	int CalculateDistanceMatrix (PeakInfoForClusters** peaks, int nPeaks, double** peakMatrix);
	int PopulateAndSortDistanceElements (int nPeaks, double** peakMatrix, vector<DistanceElement*>& distanceArray);

	bool FoundPrimerPeaks (RGDList& peakList, double firstIntervalFraction, double smallestIntervalFraction, double& lastPrimerTime, int Size, double& maxPeak, RGTextOutput& ExcelText);
	virtual double GetWidthAtTime (double t) const;
	virtual double GetSecondaryContentAtTime (double t) const;
	virtual double GetTimeForSpecifiedID (double id);
	virtual CoordinateTransform* GetIDMap ();

	virtual bool AddILSToHistoryList ();
	virtual ILSCandidate* TestILSStartAndEndSignals (DataSignal* startSignal, DataSignal* endSignal);
	virtual ILSCandidate* TestLadderILSStartAndEndSignals (DataSignal* startSignal, DataSignal* endSignal);
	virtual bool TestAllILSStartAndEndSignals (RGDList& finalILSPeaks, double& correlation);
	virtual bool TestAllLadderILSStartAndEndSignals (RGDList& finalILSPeaks, double& correlation);
	virtual void ResetBoundsUsingFactorToILSHistory (double factor) { if (mLaneStandard != NULL) mLaneStandard->ResetBoundsUsingFactorToILSHistory (factor); }

	virtual double GetMinimumHeight () const;
	virtual double GetMaximumHeight () const;
	virtual double GetDetectionThreshold () const;

	virtual double GetMeasurementRatio () const;
	virtual bool IsControlChannel () const { return true; }
	virtual int WriteBaselineData (RGTextOutput& text, const RGString& delim, const RGString& indent);


	// Obsolete...not used**********************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int AnalyzeLaneStandardChannel (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);


	// Legacy Message functions*****************************************************************************************************************
	//******************************************************************************************************************************************


	virtual int SetData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int SetRawData (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int AnalyzeLaneStandardChannelRecursively (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	int AnalyzeLaneStandardChannelRecursivelyUsingDensity (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int HierarchicalLaneStandardChannelAnalysis (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	virtual int CorrectLaneStandardCrossChannelAnalysis ();	//$
	virtual int FinalTestForCriticalLaneStandardNotices ();	//$



	//******************************************************************************************************************************************
	//******************************************************************************************************************************************

	// Smart Message functions******************************************************************************************************************
	//******************************************************************************************************************************************

	bool FoundPrimerPeaksSM (RGDList& peakList, double firstIntervalFraction, double smallestIntervalFraction, double& lastPrimerTime, int Size, double& maxPeak, RGTextOutput& ExcelText);

	virtual int SetDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataSM (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	virtual int AnalyzeLaneStandardChannelRecursivelySM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	int AnalyzeLaneStandardChannelRecursivelyUsingDensitySM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int HierarchicalLaneStandardChannelAnalysisSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	virtual int TestArtifactListForNoticesWithinLaneStandardSM (ChannelData* laneStandard, CoreBioComponent* associatedGrid);

	virtual int CorrectLaneStandardCrossChannelAnalysisSM ();	//This no longer does anything
	virtual int FinalTestForCriticalLaneStandardNoticesSM ();
	virtual int TestForRaisedBaselineAndExcessiveNoiseSM (double left, double report);

	virtual int FitAllCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int FitAllNegativeCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	virtual int TestSignalsForOffScaleSM ();
	virtual int PreTestForSignalOffScaleSM ();


	//******************************************************************************************************************************************

	static void SetLSMinimumRFU (double rfu) { minLaneStandardRFU = rfu; }
	static double GetMinRFU () { return minLaneStandardRFU; }
	static void SetLSMaximumRFU (double rfu) { maxLaneStandardRFU = rfu; }
	static double GetMaxRFU () { return maxLaneStandardRFU; }

	static double GetILSStutterThreshold () { return ILSStutterThreshold; }
	static void SetILSStutterThreshold (double limit) {ILSStutterThreshold = limit; }
	static double GetILSAdenylationThreshold () { return ILSAdenylationThreshold; }
	static void SetILSAdenylationThreshold (double limit) {ILSAdenylationThreshold = limit; }

	static void SetILSFractionalFilter (double limit) { ILSFractionalFilter = limit; }
	static double GetILSFractionalFilter () { return ILSFractionalFilter; }

protected:
	Quadratic QFit;
	Quadratic QFit2;
	CSplineTransform* mInverseSouthern;
	CSplineTransform* mGlobalSouthern;

	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************

	double DotProductWithQuadraticFit (RGDList& set, int size, const double* idealValues, const double* idealDifferences, double idealNorm2);

	//******************************************************************************************************************************************************************************

	static double minLaneStandardRFU;
	static double maxLaneStandardRFU;
	static double ILSStutterThreshold;
	static double ILSAdenylationThreshold;
	static double ILSFractionalFilter;
	static bool TestIsOn;
};


class STRLadderChannelData : public STRChannelData {

	PERSISTENT_DECLARATION (STRLadderChannelData)

public:
	STRLadderChannelData ();
	STRLadderChannelData (int channel);
	STRLadderChannelData (int channel, LaneStandard* inputLS);
	STRLadderChannelData (const STRLadderChannelData& strLadder);
	STRLadderChannelData (const STRLadderChannelData& strLadder, CoordinateTransform* trans);
	virtual ~STRLadderChannelData ();

	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************

	virtual bool TestIfTimeIsLeftOfLocus (double time, Locus* locus, Locus* assocGridLocus);

	virtual ChannelData* CreateNewTransformedChannel (const ChannelData& cd, CoordinateTransform* trans);
	virtual double GetMinimumHeight () const;
	virtual double GetMaximumHeight () const;
	virtual double GetFractionalFilter () const;
	virtual double GetDetectionThreshold () const;

	virtual bool IsControlChannel () const { return true; }


	// Legacy Message functions*****************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int SetData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int SetRawData (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int AnalyzeGridLoci (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$


	//******************************************************************************************************************************************
	//******************************************************************************************************************************************

	// Smart Message functions******************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int FitAllCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int SetDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataSM (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	virtual int AnalyzeGridLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual void MakeNonCoreLadderArtifactsNoncritical ();


	//******************************************************************************************************************************************

	
	static void SetLadderMinimumRFU (double rfu) { minLadderRFU = rfu; }
	static double GetMinRFU () { return minLadderRFU; }
	static void SetLadderMaximumRFU (double rfu) { maxLadderRFU = rfu; }
	static double GetMaxRFU () { return maxLadderRFU; }
	static void SetMinInterlocusRFU (double rfu) { minInterlocusRFU = rfu; }
	static double GetMinInterlocusRFU () { return minInterlocusRFU; }

protected:
	static double minLadderRFU;
	static double maxLadderRFU;
	static double minInterlocusRFU;
};


class STRSampleChannelData : public STRChannelData {

	PERSISTENT_DECLARATION (STRSampleChannelData)

public:
	STRSampleChannelData ();
	STRSampleChannelData (int channel);
	STRSampleChannelData (int channel, LaneStandard* inputLS);
	STRSampleChannelData (const STRSampleChannelData& strSample);
	STRSampleChannelData (const STRSampleChannelData& strSample, CoordinateTransform* trans);
	virtual ~STRSampleChannelData ();

	// Not message-related**********************************************************************************************************************************************************
	//******************************************************************************************************************************************************************************

	virtual bool TestIfTimeIsLeftOfLocus (double time, Locus* locus, Locus* assocGridLocus);
	virtual bool TestInterlocusSignalHeightBelowThreshold (DataSignal* signal);
	virtual ChannelData* CreateNewTransformedChannel (const ChannelData& cd, CoordinateTransform* trans);

	virtual double GetMinimumHeight () const;
	virtual double GetMaximumHeight () const;
	virtual double GetFractionalFilter () const;
	virtual double GetDetectionThreshold () const;

	static void SetSampleMinimumRFU (double rfu) { minSampleRFU = rfu; }
	static double GetMinRFU () { return minSampleRFU; }
	static void SetSampleMaximumRFU (double rfu) { maxSampleRFU = rfu; }
	static double GetMaxRFU () { return maxSampleRFU; }
	static void SetMinInterlocusRFU (double rfu) { minInterlocusRFU = rfu; }
	static double GetMinInterlocusRFU () { return minInterlocusRFU; }
	static void SetDetectionThreshold (double rfu) { sampleDetectionThreshold = rfu; }
	static double GetSampleDetectionThreshold () { return sampleDetectionThreshold; }
	static void InitializeChannelSpecificThresholds (int nChannels, list<channelThreshold*>* analysisLimits, list<channelThreshold*>* detectionLimits);
	static int GetAnalysisThresholdOverride (int channel) { return ChannelSpecificMinRFUOverrides [channel]; }
	static int GetDetectionThresholdOverride (int channel) { return ChannelSpecificDetectionOverrides [channel]; }


	// Legacy Message functions*****************************************************************************************************************
	//******************************************************************************************************************************************

	virtual int SetData (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int SetRawData (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);	//$
	virtual int AssignSampleCharacteristicsToLoci (CoreBioComponent* grid, CoordinateTransform* timeMap);	//$
	virtual int AnalyzeSampleLoci (ChannelData* lsData, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int TestFractionalFilters ();	//$
	virtual int TestForInterlocusProximityArtifacts ();	//$
	virtual int FinalTestForPeakSizeAndNumber (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes);	//$
	virtual int TestForMultiSignals ();	//$
	virtual int TestProximityArtifacts ();	//$
	virtual int AssignSignalToFirstLocusAndDeleteFromSecond (DataSignal* target, Locus* locus1, Locus* locus2);	//$
	virtual int RemoveSignalFromBothLoci (DataSignal* target, Locus* locus1, Locus* locus2);	//$
	virtual bool TestIfSignalBelongsToFirstLocus (DataSignal* target, Locus* locus1, Locus* locus2);	//$
	virtual int ResolveAmbiguousInterlocusSignals ();	//$


	//******************************************************************************************************************************************
	//******************************************************************************************************************************************

	// Smart Message functions******************************************************************************************************************
	//******************************************************************************************************************************************


	virtual int SetDataSM (SampleData& fileData, TestCharacteristic* testControlPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataSM (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	virtual int SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak);
	virtual int AssignSampleCharacteristicsToLociSM (CoreBioComponent* grid, CoordinateTransform* timeMap);
	virtual int AssignSampleCharacteristicsToLociSMLF ();
	virtual int AnalyzeSampleLociSM (ChannelData* lsData, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int TestFractionalFiltersSM ();
	virtual int TestForInterlocusProximityArtifactsSM ();
	virtual int FinalTestForPeakSizeAndNumberSM (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes);
	virtual int TestForMultiSignalsSM ();
	virtual int TestProximityArtifactsSM ();
	virtual int TestForAlleleDuplicationSM ();
	virtual int RemoveStutterLinksFromNonStutterPeaksSM ();
	virtual int AssignSignalToFirstLocusAndDeleteFromSecondSM (DataSignal* target, Locus* locus1, Locus* locus2);
	virtual int RemoveSignalFromBothLociSM (DataSignal* target, Locus* locus1, Locus* locus2);
	virtual bool TestIfSignalBelongsToFirstLocusSM (DataSignal* target, Locus* locus1, Locus* locus2);
	virtual int ResolveAmbiguousInterlocusSignalsSM ();
	virtual int MeasureInterlocusSignalAttributesSM ();
	virtual int ResolveAmbiguousInterlocusSignalsUsingSmartMessageDataSM ();
	virtual int TestSignalsForRaisedBaseline (double left, double report);
	virtual int TestForRaisedBaselineAndExcessiveNoiseSM (double left, double report);
	virtual int AppendDataForStutterPeaksSM ();

	virtual int FitAllNegativeCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);

	virtual int AnalyzeDynamicBaselineSM (int startTime, double reportMinTime);
	virtual int AnalyzeDynamicBaselineAndNormalizeRawDataSM (int startTime, double reportMinTime);
	bool TestForRawDataPeakDuringNormalization (bool usePeakProximity, bool useBoth, int currentTime, int distanceFromPeak, int distanceFromLevelChange, double fractionOfNoiseRangeToCallPeak, double fractionOfMeasurementsToAllowLevelChange);
	int ShapeBaselineData (list<double>& knotTimes, list<double>& knotValues);
	int ShapeBaselineData (list<double>& knotTimes, list<double>& knotValues, DataSignal* fitDataPositive, DataSignal* fitDataNegative, double threshold);


	//******************************************************************************************************************************************

protected:
	list<ProspectiveIntervalForNormalization*> mProspectiveList;
	static double minSampleRFU;
	static double maxSampleRFU;
	static double minInterlocusRFU;
	static double sampleDetectionThreshold;
	static bool UseOldBaselineEstimation;
	static double* ChannelSpecificMinRFU;
	static double* ChannelSpecificDetectionThresholds;
	static int* ChannelSpecificMinRFUOverrides;
	static int* ChannelSpecificDetectionOverrides;

	bool FindNextFitDataIntervalBelowThreshold (double threshold, int start, int end, int& beginInterval, int& endInterval, DataSignal* fitData);
	bool FindNextFitDataIntervalBelowThreshold (double threshold, int start, int end, int& beginInterval, int& endInterval, DataSignal* fitData, DataSignal* fitNegData);
	void AppendKnotDataToLists (int intervalLeft, int intervalRight, list<double>& times, list<double>& values, DataSignal* fitData);
	void AppendKnotDataToLists (int intervalLeft, int intervalRight, list<double>& times, list<double>& values, list<bool>& firsts, list<bool>& lasts, DataSignal* rawData);
	void AppendKnotDataWithEditingToLists (int intervalLeft, int intervalRight, list<double>& times, list<double>& values, list<bool>& firsts, list<bool>& lasts, bool isFirstInterval, DataSignal* rawData);
	void AppendKnotDataWithEditingToListsAfterFiltering (int intervalLeft, int intervalRight, list<double>& times, list<double>& values, list<bool>& firsts, list<bool>& lasts, bool isFirstInterval, DataSignal* rawData);
	double BaselineAverage (int left, int right, DataSignal* fitData, double denominator);
	double BaselineMax (int left, int right, DataSignal* fitData);
	int EditOutFitBaselineForNormalization (RGDList& fitPeaks, DataSignal* rawData);
	int AddRelativeMinima (int intervalStart, int intervalEnd, list<double>& times, list<double>& values, DataSignal* fitDataPositive, DataSignal* fitDataNegative, double threshold);
	int EditPeaksForOutOfRange (list<double>& times, list<double>& values, DataSignal* fitDataNegative, double threshold);
	void ScanRawDataForMinimaLeftAndRight (int time, double& leftMin, double& rightMin, DataSignal* fitDataNegative, double threshold);
	bool FindRawDataMinimumOnInterval (double& minimum, int left, int right, DataSignal* fitDataNegative, double threshold);
	bool CurveIsBaselineFit (DataSignal* signal);
};


#endif  /*  _STRCHANNELDATA_H_  */

