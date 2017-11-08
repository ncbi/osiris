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
*  FileName: Genetics.h
*  Author:   Robert Goor
*
*/
//
//   These are the abstract base classes for the population genetic categories:  class BaseAllele, BaseLocus, PopulationMarkerSet and PopulationCollection,
// class Allele and Locus...  Classes including the word "Base" refer purely to input genetic information.  These classes input the genetic specifications 
// from XML (or other source) and retain it for use by other classes that may perform analysis of sample information.  These classes omit the word "Base" and
// incorporate a pointer to the parallel "Base" class for genetics reference.  Thus, there should be only one "Base" class for each data set and then that
// class is shared among the various analysis classes, of which there may be one for each active sample and control set, for example.
//
//   For example, class BaseAllele retains the basic information for an allele in a particular locus, such as allelic name and corresponding curve within the
// locus.  STRBaseAllele also contains the "BioID" or the number of repeat base pairs corresponding to the allele.  class Allele, which contains a pointer to
// an object of type BaseAllele, also has a pointer to a DataSignal, which is the corresponding curve to the allele from a control set.
//
//   To do:
//   Add constructor arguments to Allele, Locus and PopulationMarkerSet to initialize from xml input in addition to a pointer to the corresponding "Base" class
//   (Done:  Nov. 8, 2007)
//
//   Add copy constructors to Allele, Locus and PopulationMarkerSet (Done:  Nov. 9, 2007)
//
//   Add fixed target items:  add fixed Allele pointer to each Locus and fixed Locus pointer to each PopulationMarkerSet.  Make default constructor for each create
//   a non-NULL pointer to corresponding "base" object for target searching capabilities.  This will prevent a call to new and delete for each search.  
//   (Done:  Nov. 9, 2007)
//
//   Change Allele, Locus and PopulationMarkerSet to be non-Abstract (Done:  Nov. 14, 2007)
//

#ifndef _GENETICS_H_
#define _GENETICS_H_

#include "rgpersist.h"
#include "rgstring.h"
#include "rgpscalar.h"
#include "rgdefs.h"
#include "rghashtable.h"
#include "rgdlist.h"
#include "BaseGenetics.h"
#include "Notices.h"
#include "SmartMessagingObject.h"
#include "coordtrans.h"
#include "DataSignal.h"

#include <list>

class DataSignal;
class RGFile;
class RGVInStream;
class RGVOutStream;
struct SupplementaryData;
class CoordinateTransform;
class RGTextOutput;
class OsirisMsg;
class ChannelData;
class IndividualLocus;
class GenotypesForAMarkerSet;
struct CompoundSignalInfo;
class SmartMessage;
class SmartNotice;
class SmartMessageReporter;
class CoreBioComponent;

const int _ALLELE_ = 1038;
const int _LOCUS_ = 1039;
const int _POPULATIONMARKERSET_ = 1040;
const int _LANESTANDARD_ = 1041;

PERSISTENT_PREDECLARATION (Allele)
PERSISTENT_PREDECLARATION (Locus)
PERSISTENT_PREDECLARATION (PopulationMarkerSet)
PERSISTENT_PREDECLARATION (LaneStandard)


struct IdealControlSetInfo {

	const double* mPts;
	const double* mDiffs;
	const double* mLeftNorm2s;
	const double* mRightNorm2s;
	const int* mHeightIndices;
	const int* mHeightRangeInfo;
	int mSetSize;
	bool mCapMaxHeightIndex;
	bool mSignalIntervalIsAccurate;
	int mDiscrepancy;

	IdealControlSetInfo (const double* pts, const double* diffs, const double* leftNorms, const double* rightNorms, const int* htInd, int size, bool accurate);
};


struct ILSNameIndexes {

	ILSNameIndexes (const RGString& name, size_t s, size_t e, const RGString& dyeName) { mName = name; mStartIndex = s; mEndIndex = e; mDyeName = dyeName; }

	RGString mName;
	RGString mDyeName;
	size_t mStartIndex;
	size_t mEndIndex;
};


void SetDifference (RGDList& listA, RGDList& listB, RGDList& AButNotB, RGDList& BButNotA);



class Allele : public RGPersistent {

	PERSISTENT_DECLARATION (Allele)

public:
	Allele ();
	Allele (BaseAllele* link, const RGString& xmlString);
	Allele (const Allele& allele);
	Allele (const Allele& allele, CoordinateTransform* trans);
	virtual ~Allele ();

	void ClearAndDestroy ();

	Boolean IsValid () const { return Valid; }
	RGString GetAlleleName () const { return mLink->GetAlleleName (); }
	RGString GetError () const { return Msg; }
	
	int GetCurveNumber () const { return mLink->GetCurveNumber (); }
	const DataSignal* GetCurvePointer () const { return CorrespondingSignal; }

	void SetAlleleName (const RGString& name) { mLink->SetAlleleName (name); }
	void SetCurve (int curve) { mLink->SetCurve (curve); }

	void SetCurvePointer (DataSignal* ds);

	int GetRelativeHeight () const { return mLink->GetRelativeHeight (); }

	virtual void SetBioID (int id);  // virtual ???
	virtual int GetBioID () const;  // virtual ???
//	virtual const DataSignal* GetCurvePointer () const;
//	virtual void SetCurvePointer (DataSignal* ds);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void Write (RGFile& textOutput, const RGString& indent);

protected:
	BaseAllele* mLink;
	DataSignal* CorrespondingSignal;
	Boolean Linked;
	Boolean Valid;
	RGString Msg;
};


class Locus : public SmartMessagingObject {

	PERSISTENT_DECLARATION (Locus)

public:
	Locus ();
	Locus (BaseLocus* link, const RGString& xmlString);
	Locus (const Locus& locus);
	Locus (const Locus& locus, CoordinateTransform* trans);
	virtual ~Locus ();

	// Not message-related**********************************************************************************************************************************************************

	
	void ClearAndDestroy ();

	double GetMinTimeForSample () const { return mMinTimeForSample; }
	double GetMaxTimeForSample () const { return mMaxTimeForSample; }
	double GetTimeForOneIDUnitLeft () const { return mTimeForOneIDUnitLeft; }
	double GetTimeForOneIDUnitRight () const { return mTimeForOneIDUnitRight; }

	double GetMaximumPeak () const { return mMaxPeak; }

	void SetTableLink (int linkNumber);
	RGString GetTableLink () const { return mTableLink; }

	bool IsQualityLocus () { if (mLink == NULL) return false; return mLink->isQualityLocus (); }

//	int GetHighestSeverityLevel () const { return mHighestSeverityLevel; }
	int GetHighestMessageLevel () const { return mHighestMessageLevel; }
	RGString GetSampleString () const { return SampleString; }
	
	RGDList& GetFinalSignalList () { return FinalSignalList; }

	Boolean IsValid () const { return Valid; }
	RGString GetErrorString () const { return ErrorString; }

	double GetFirstTimeForLadderLocus () const { return mFirstTime; }
	double GetLastTimeForLadderLocus () const { return mLastTime; }

	RGString GetLocusName () const;
	void SetLocusName (const RGString& name);
	int GetLocusChannel () const;
	void SetLocusChannel (int channel);
	int NumberOfAlleles () const { return AlleleList.Entries (); }
	int NumberOfAmbiguousAlleles ();
	const int* GetRelativeHeights () const;
	virtual void AppendBaseLocusToList (RGDList& locusList);

	void SetMinExtendedLocusTime (double time) { mMinExtendedLocusTime = time; }
	void SetMaxExtendedLocusTime (double time) { mMaxExtendedLocusTime = time; }

	void ResetAlleles ();
	Allele* GetNextAllele ();

	Allele* FindAllele (const RGString& name);
	Allele* FindAllele (int curve);

	Allele* FindAlleleByID (int id);
	Allele* FindNearestAllele (int id);

	bool SignalIsIntegralMultipleOfRepeatAboveLadder (DataSignal* nextSignal);
	bool SignalIsIntegralMultipleOfRepeatBelowLadder (DataSignal* nextSignal);

	double GetNonStandardStutterThreshold (int bp) { return mLink->GetNonStandardStutterThreshold (bp); }

	RGString ReconstructAlleleName (int id, Boolean& isOffGrid);
	virtual int InsertAlleleSignals (RGDList& curveList);  // virtual ???
	virtual const DataSignal* GetCurve (const RGString& alleleName);  // virtual ???

	Boolean IsTimeWithinLocusSample (double t);
	Boolean IsTimeWithinExtendedLocusSample (double t);
	Boolean IsTimeWithinExtendedLocusSample (double t, int& location);

	DataSignal* RemoveSignalFromLocusList (DataSignal* target);
	int PromoteSignalToAllele (DataSignal* target);
	int PromoteNonAmbiguousSignalsToAlleles (RGDList& signalList);
	int PromoteOwnCoreSignalsToAllelesAndRemoveOthers (RGDList& signalList);
	int PromoteSignalToLeftExtendedAllele (DataSignal* target);
	int PromoteSignalToRightExtendedAllele (DataSignal* target);
	int InsertSignalExtendedLeft (DataSignal* newSignal);
	int InsertSignalExtendedRight (DataSignal* newSignal);

	double HasHeightRatioWithExclusiveMaxPeak (DataSignal* target, bool& causesHeightImbalance);
	int TestForAcceptedTriAllele (Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes);

	virtual Boolean ExtractSampleSignals (RGDList& channelSignalList, Locus* gridLocus, CoordinateTransform* timeMap);
//	virtual Boolean ExtractExtendedLadderSampleSignals (RGDList& channelSignalList, Locus* gridLocus, CoordinateTransform* timeMap);
	virtual bool ComputeExtendedLocusTimes (CoreBioComponent* grid, CoordinateTransform* inverseTransform, int channelNumber, CoreBioComponent* associatedGrid);

	int CallAlleles (bool isNegCntl, GenotypesForAMarkerSet* pGenotypes, RGDList& artifacts, RGDList& signalList, RGDList& completeList);  // the latest, assuming CraterSignals

	virtual int TestSignalPositionRelativeToLocus (DataSignal* signal, Locus* gridLocus);
	virtual int TestSignalPositionRelativeToLocus (DataSignal* signal);
	virtual int TestSignalTimeRelativeToGridLocus (double time);

	int TestSampleNeighbors (RGDList& previousList, DataSignal* testSignal, RGDList& followingList);

	virtual double GetMaximumHeight ();
	void PrecomputeAverages (RGDList& set);
	double TotalPeakHeight (int& numberOfPeaks);

	double GetBPFromTimeForAnalysis (double time);
	double GetTimeFromBP (double bp);

	Boolean GridLocusContainsID (double id);
	Boolean ExtendedLocusContainsID (double id);
	Boolean ExtendedLocusContainsID (double id, int& location);
	int DirectionOfTimeFromLocus (double time);
	double GridDistance (double id);
	double ExtendedDistance (double id);

	bool TestForProximityArtifact (DataSignal* target, RGDList& externalLeft, RGDList& externalRight, double minTLeft, double maxTLeft, double minTRight, double maxTRight, double threshold);
	bool TestForProximityArtifact (DataSignal* target, RGDList& externalLeft, RGDList& externalRight, double minTLeft, double maxTLeft, double minTRight, double maxTRight, double thresholdLeft, double thresholdRight);
	int LocationOfSignal (DataSignal* signal);

	void SetLocusSpecificSampleStutterThreshold (double limit) { if (mLink != NULL) mLink->SetSampleStutterThreshold (limit); }
	void SetLocusSpecificSamplePlusStutterThreshold (double limit) { if (mLink != NULL) mLink->SetSamplePlusStutterThreshold (limit); }
	void SetLocusSpecificSampleAdenylationThreshold (double limit) { if (mLink != NULL) mLink->SetSampleAdenylationThreshold (limit); }
	void SetLocusSpecificSampleFractionalFilter (double limit) { if (mLink != NULL) mLink->SetSampleFractionalFilter (limit); }
	void SetLocusSpecificSamplePullupFractionalFilter (double limit) { if (mLink != NULL) mLink->SetSamplePullupFractionalFilter (limit); }
	void SetLocusSpecificSampleHeterozygousImbalanceThreshold (double limit) { if (mLink != NULL) mLink->SetSampleHeterozygousImbalanceThreshold (limit); }
	void SetLocusSpecificSampleMinBoundForHomozygote (double limit) { if (mLink != NULL) mLink->SetSampleMinBoundForHomozygote (limit); }
	void SetLocusSpecificNonStandardStutterArray (const locusSpecificNonStandardStutterStruct& limits) { if (mLink != NULL) mLink->SetNonStandardStutterArray (limits); }

	void SetLocusSpecificLadderStutterThreshold (double limit) { if (mLink != NULL) mLink->SetLadderStutterThreshold (limit); }
	void SetLocusSpecificLadderAdenylationThreshold (double limit) { if (mLink != NULL) mLink->SetLadderAdenylationThreshold (limit); }
	void SetLocusSpecificLadderFractionalFilter (double limit) { if (mLink != NULL) mLink->SetLadderFractionalFilter (limit); }
	void SetLocusSpecificLadderPullupFractionalFilter (double limit) { if (mLink != NULL) mLink->SetLadderPullupFractionalFilter (limit); }

	double GetLocusSpecificSampleStutterThreshold () const;
	double GetLocusSpecificSamplePlusStutterThreshold () const;
	double GetLocusSpecificSampleAdenylationThreshold () const;
	double GetLocusSpecificSampleFractionalFilter () const;
	double GetLocusSpecificSamplePullupFractionalFilter () const;
	double GetLocusSpecificSampleHeterozygousImbalanceThreshold () const;
	double GetLocusSpecificSampleMinBoundForHomozygote () const;

	double GetLocusSpecificLadderStutterThreshold () const;
	double GetLocusSpecificLadderAdenylationThreshold () const;
	double GetLocusSpecificLadderFractionalFilter () const;
	double GetLocusSpecificLadderPullupFractionalFilter () const;

	double GetMinimumILSBP () const { return mLink->GetMinimumBPILS (); }

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void Write (RGFile& textOutput, const RGString& indent);
	virtual void WriteXMLInfo (RGTextOutput& text, const RGString& indent);

//	virtual void RestoreAll (RGFile&);
//	virtual void RestoreAll (RGVInStream&);
//	virtual void SaveAll (RGFile&) const;
//	virtual void SaveAll (RGVOutStream&) const;



	//******************************************************************************************************************************************************************************
	
	// Pre-object Message-Related***************************************************************************************************************************************************


	void ReportSampleData (RGTextOutput& ExcelText, const RGString& indent);
	RGString BuildSampleString (const RGString& separator);


	//******************************************************************************************************************************************************************************

	// Legacy Message-Related*******************************************************************************************************************************************************

	virtual int AnalyzeGridLocus (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$
	virtual int AnalyzeSampleLocus (ChannelData* lsData, RGDList& artifactList, RGDList& signalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);	//$

	virtual int TestFractionalFilters (RGDList& artifactList, RGDList& supplementalList);	//$

	virtual Boolean ExtractGridSignals (RGDList& channelSignalList, const LaneStandard* ls, ChannelData* lsData);	//$
	virtual int TestInterlocusSignals (RGDList& signalSet, RGDList& artifacts, ChannelData* laneStd);	//$
	virtual int TestInterlocusSignals (RGDList& signalSet, RGDList& artifacts, RGDList& type1List, RGDList& type2List);	//$
	virtual int TestAmelogenin ();	//$
	virtual int TestPositiveControl (IndividualLocus* locus, RGDList& artifacts);	//$

	int TestNeighbors (DataSignal* previous, DataSignal* testSignal, DataSignal* following);	//$
	int TestSampleNeighbors (DataSignal* previous, DataSignal* testSignal, DataSignal* following);	//$
	
	int TestSampleAverages (ChannelData* lsData, DataSignal* testSignal, Boolean testRatio = TRUE);	//$
	virtual Boolean ExtractExtendedSampleSignals (RGDList& channelSignalList, Locus* gridLocus, CoordinateTransform* timeMap);	//$

	virtual int FinalTestForPeakSizeAndNumber (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes, RGDList& artifacts);	//$
	int CorrectCrossChannelAnalyses (RGDList& artifacts, bool isNegCntl);	//$
	int CleanUpSignalList (RGDList& artifacts);	//$
	int CorrectGridSignalList (RGDList& artifacts);	//$
	int CleanUpGridSignalList (RGDList& artifacts);	//$

	int TestProximityArtifacts (RGDList& artifacts, RGDList& type1List, RGDList& type2List);	//$
	int TestForMultiSignals (RGDList& artifacts, RGDList& signalList, RGDList& completeList);	//$

	int AddNoticeToList (Notice* newNotice);	//$
	Boolean IsNoticeInList (const Notice* target);	//$
	Notice* GetNotice (const Notice* target);	//$

	Boolean ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink = FALSE);	//$
	Boolean ReportXMLNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);	//$
	void ReportXMLGridTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);	//$
	void ReportXMLSampleTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);	//$
	void ClearNoticeObjects ();	//$
	int NumberOfNoticeObjects ();	//$
	Notice* RemoveNotice (const Notice* target);	//$
	void RetrieveNoticesFromGridArtifactList (ChannelData* laneStandard);	//$


	//******************************************************************************************************************************************************************************

	// Smart Message Functions*******************************************************************************
	//*******************************************************************************************************

	virtual int GetObjectScope () const { return 2; }

	bool EvaluateSmartMessagesForStage (int stage);
	bool EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage);
	bool EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly);
	bool SetTriggersForAllMessages (bool* const higherMsgMatrix, int stage, bool allMessages, bool signalsOnly);
	bool SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly);
	bool EvaluateAllReports (bool* const reportMatrix);
	bool TestAllMessagesForCall ();
	bool EvaluateAllReportLevels (int* const reportLevelMatrix);

	bool ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, bool reportLink = false);

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);
	virtual RGString GetDebugIDIndent () const; 
	virtual int AddAllSmartMessageReporters ();
	virtual int AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects);
	virtual int AddAllSmartMessageReportersForSignals (SmartMessagingComm& comm, int numHigherObjects);

	Boolean ReportXMLSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);
	void ReportXMLSmartGridTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);
	void ReportXMLSmartSampleTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);

	virtual int AnalyzeGridLocusSM (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AnalyzeGridLocusAndApplyThresholdsSM (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AnalyzeGridLocusAndAllowForOverlapSM (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AnalyzeGridLocusAndAllowForOverlapUsingBPsSM (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int AnalyzeSampleLocusSM (ChannelData* lsData, RGDList& artifactList, RGDList& signalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print = TRUE);
	virtual int TestGridInterlocusThresholds (RGDList& originalList);

	int CallAllelesSM (bool isNegCntl, GenotypesForAMarkerSet* pGenotypes, RGDList& artifacts, RGDList& signalList, RGDList& completeList);  // the latest, assuming CraterSignals
	virtual DataSignal* GetLargestPeak ();
	virtual int TestResidualDisplacement ();

	virtual int TestFractionalFiltersSM (RGDList& artifactList, RGDList& supplementalList);

	virtual Boolean ExtractGridSignalsSM (RGDList& channelSignalList, const LaneStandard* ls, ChannelData* lsData);
	virtual int TestInterlocusSignalsSM (RGDList& signalSet, RGDList& artifacts, ChannelData* laneStd);
	virtual int TestInterlocusSignalsSM (RGDList& signalSet, RGDList& artifacts, RGDList& type1List, RGDList& type2List);
	virtual int TestAmelogeninSM ();
	virtual int TestPositiveControlSM (IndividualLocus* locus, RGDList& artifacts);

	virtual void InitializeMessageData ();

	int TestNeighborsSM (DataSignal* previous, DataSignal* testSignal, DataSignal* following);
	int TestSimpleNeighborsForGridSM (RGDList& preceedingSignals, DataSignal* testSignal, RGDList& followingSignals);
	int TestComplexNeighborsForGridSM (DataSignal* testSignal, RGDList& comparisonSignals);
	int TestSampleNeighborsSM (DataSignal* previous, DataSignal* testSignal, DataSignal* following);
	int TestSampleAveragesSM (ChannelData* lsData, DataSignal* testSignal, Boolean testRatio = TRUE);
	virtual Boolean ExtractExtendedSampleSignalsSM (RGDList& channelSignalList, Locus* gridLocus, CoordinateTransform* timeMap, Locus* prevGridLocus, Locus* followingGridLocus);
	int MeasureInterlocusSignalAttributesSM ();

	virtual int FinalTestForPeakSizeAndNumberSM (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes, RGDList& artifacts);
	int CorrectCrossChannelAnalysesSM (RGDList& artifacts, bool isNegCntl);
	int CleanUpSignalListSM (RGDList& artifacts);
	int CorrectGridSignalListSM (RGDList& artifacts);
	int CleanUpGridSignalListSM (RGDList& artifacts);

	int TestProximityArtifactsSM (RGDList& artifacts, RGDList& type1List, RGDList& type2List);
	int TestProximityArtifactsUsingLocusBasePairsSM (CoordinateTransform* timeMap);
	int TestProximityArtifactsUsingLocusBasePairsSM (RGDList& artifacts, RGDList& type1List, RGDList& type2List);
	int TestForMultiSignalsSM (RGDList& artifacts, RGDList& signalList, RGDList& completeList, RGDList& smartPeaks, GenotypesForAMarkerSet* pGenotypes);
	int TestForDuplicateAllelesSM (RGDList& artifacts, RGDList& signalList, RGDList& completeList, RGDList& smartPeaks, GenotypesForAMarkerSet* pGenotypes);
	void RetrieveSmartNoticesFromGridArtifactList (ChannelData* laneStandard);

	void TestAllelesAgainstOverloadThresholdSM ();
	void ResolveAmbiguousExtendedLocusPeaksSM ();

	//*******************************************************************************************************
	//*******************************************************************************************************

	static Boolean GetTestRatio () { return TestRatio; }
	static void SetTestRatio (Boolean test) { TestRatio = test; }

	static bool ExclusiveOr (bool arg1, bool arg2);
	static double MaximumPeakHeight (RGDList& incomingList);
	static int DivideListByPeakHeight (RGDList& incomingList, RGDList& newList, double maxPeak, double factor, bool removeLess);
	static int DivideListByPeakHeight (RGDList& incomingList, RGDList& junkList, RGDList& individualList,
		double minValue, double maxValue, double factor, int size);

	static void SetHeterozygousImbalanceLimit (double limit) { HeterozygousImbalanceLimit = limit; }
	static double GetHeterozygousImbalanceLimit () { return HeterozygousImbalanceLimit; }
	static void SetMinBoundForHomozygote (double minBound) { MinBoundForHomozygote = minBound; }
	static double GetMinBoundForHomozygote () { return MinBoundForHomozygote; }

	static void SetSampleStutterThreshold (double limit) { if (limit > 0.0) SampleStutterThreshold = limit;  else SampleStutterThreshold = 0.0; }
	static double GetSampleStutterThreshold () { return SampleStutterThreshold; }
	static void SetSamplePlusStutterThreshold (double limit) { if (limit > 0.0) SamplePlusStutterThreshold = limit;  else SamplePlusStutterThreshold = 0.0; }
	static double GetSamplePlusStutterThreshold () { return SamplePlusStutterThreshold; }
	static void SetGridStutterThreshold (double limit) { GridStutterThreshold = limit; }
	static double GetGridStutterThreshold () { return GridStutterThreshold; }

	static void SetSampleAdenylationThreshold (double limit) { SampleAdenylationThreshold = limit; }
	static double GetSampleAdenylationThreshold () { return SampleAdenylationThreshold; }
	static void SetGridAdenylationThreshold (double limit) { GridAdenylationThreshold = limit; }
	static double GetGridAdenylationThreshold () { return GridAdenylationThreshold; }

	static void SetSampleFractionalFilter (double limit) { SampleFractionalFilter = limit; }
	static double GetSampleFractionalFilter () { return SampleFractionalFilter; }
	static void SetGridFractionalFilter (double limit) { GridFractionalFilter = limit; }
	static double GetGridFractionalFilter () { return GridFractionalFilter; }

	static void SetSamplePullupFractionalFilter (double limit) { SamplePullupFractionalFilter = limit; }
	static double GetSamplePullupFractionalFilter () { return SamplePullupFractionalFilter; }
	static void SetGridPullupFractionalFilter (double limit) { GridPullupFractionalFilter = limit; }
	static double GetGridPullupFractionalFilter () { return GridPullupFractionalFilter; }

	static void SetExpectRFUUnitsForHomozygoteBound (bool truth) { ExpectRFUUnitsForHomozygoteBound = truth; }
	static bool GetExpectRFUUnitsForHomozygoteBound () { return ExpectRFUUnitsForHomozygoteBound; }

	static void SetMaxResidualForAlleleCalls (double limit) { MaxResidualForAlleleCalls = limit; }
	static double GetMaxResidualForAlleleCalls () { return MaxResidualForAlleleCalls; }

	static void SetAlleleOverloadThreshold (double limit) { AlleleOverloadThreshold = limit; }
	static double GetAlleleOverloadThreshold () { return AlleleOverloadThreshold; }
	static bool RemoveExtraneousNoticesFromSignal (DataSignal* ds);

	static void CreateInitializationData (int scope);
	static void InitializeMessageMatrix (bool* matrix, int size);
	static void ClearInitializationMatrix () { delete[] InitialMatrix; InitialMatrix = NULL; }
	static int GetScope () { return 2; }

	static void SetNoYForAMEL (bool value) { NoYForAMEL = value; }
	static bool IsNoYForAMEL () { return NoYForAMEL; }

	static void SetDisableStutterFilter (bool s) { DisableStutterFilter = s; }
	static void SetDisableAdenylationFilter (bool s) { DisableAdenylationFilter = s; }
	static bool GetDisableStutterFilter () { return DisableStutterFilter; }
	static bool GetDisableAdenylationFilter () { return DisableAdenylationFilter; }

	static void SetCallOnLadderAdenylation (bool s) { CallOnLadderAdenylation = s; }
	static bool GetCallOnLadderAdenylation () { return CallOnLadderAdenylation; }

	static void SetNumberOfChannels (int n) { NumberOfChannels = n; }
	static void SetSingleSourceSample (bool d) { IsSingleSourceSample = d; }
	static void SetControlSample (bool d) { IsControlSample = d; }

protected:
	BaseLocus* mLink;
	RGHashTable AlleleListByName;   // items of type Allele*
	RGHashTable AlleleListByCurve;   // items of type Allele*
	RGDList AlleleList;
	RGDList LocusSignalList;
	RGDList FinalSignalList;
	RGDList mSmartList;
	RGHashTable NonStandardAlleleList;
	RGDListIterator* AlleleIterator;
	Boolean Valid;
	RGString ErrorString;
	Boolean Linked;
	Allele mTarget;
	CoordinateTransform* mSampleAnalysisMap;  // finds bp as a function of time
	CoordinateTransform* mSampleTimeFromBPMap;  // finds time as a function of bp

	double averageWidth;
	double averagePeak;

	double MaximumSampleTime;
	double MinimumSampleTime;

	double mMinTimeForRoundedCore;
	double mMaxTimeForRoundedCore;

	RGDList NewNoticeList;
	RGString mTableLink;
	RGString SampleString;

	list<CompoundSignalInfo*> mSignalLinkList;

	double mMinTimeForSample;
	double mMaxTimeForSample;
	double mTimeForOneIDUnitLeft;
	double mTimeForOneIDUnitRight;

	int mNumberOfCraters;
	bool mIsOK;

	RGDList mGridArtifactList;
	bool mIsAMEL;

	double mMinExtendedLocusTime;
	double mMaxExtendedLocusTime;

	RGDList mExtendedLeft;
	RGDList mExtendedRight;

	double mMaxPeak;
	DataSignal* mLargestPeak;

	double mFirstTime;
	double mLastTime;
	Locus* mGridLocus;

	// Smart Message data************************************************************************************
	//*******************************************************************************************************


	//*******************************************************************************************************
	//*******************************************************************************************************

	static double MinimumFractionOfAverageWidth;
	static double MaximumMultipleOfAverageWidth; 
	static double MinimumFractionOfAveragePeak;
	static double MaximumMultipleOfAveragePeak;

	static double MinimumFractionOfAverageSampleWidth;
	static double MaximumMultipleOfAverageSampleWidth; 
	static double MinimumFractionOfAverageSamplePeak;
	static double MaximumMultipleOfAverageSamplePeak;
	static Boolean TestRatio;
	static double HeterozygousImbalanceLimit;
	static double MinBoundForHomozygote;
	static double SampleStutterThreshold;
	static double SamplePlusStutterThreshold;
	static double SampleAdenylationThreshold;
	static double GridStutterThreshold;
	static double GridAdenylationThreshold;

	static double SampleFractionalFilter;
	static double GridFractionalFilter;

	static double SamplePullupFractionalFilter;
	static double GridPullupFractionalFilter;

	static bool ExpectRFUUnitsForHomozygoteBound;

	static double MaxResidualForAlleleCalls;
	static double AlleleOverloadThreshold;
	static bool* InitialMatrix;
	static bool NoYForAMEL;
	static bool DisableStutterFilter;
	static bool DisableAdenylationFilter;
	static bool CallOnLadderAdenylation;
	static int NumberOfChannels;
	static bool IsSingleSourceSample;
	static bool IsControlSample;

	Boolean BuildAlleleLists (const RGString& xmlString);
	Boolean BuildMappings (RGDList& signalList);

	// Smart Message functions*******************************************************************************
	//*******************************************************************************************************


	//*******************************************************************************************************
	//*******************************************************************************************************
};


class ILSHistory {

public:
	ILSHistory ();
	~ILSHistory ();

	void SetNumberOfCharacteristics (int n);
	bool AddILS (double* times);

	double GetMinWidth () const { return mMinWidth; }
	double GetMaxWidth () const { return mMaxWidth; }
	double* GetNormalizedDifferences () const { return mNormalizedDifferences; }
	double* GetLadderNormalizedDifferences () const { return mNormalizedCharacteristicDifferences; }

	void ResetIdealCharacteristicsAndIntervalsForLadderILS (const double* actualArray, const double* differenceArray, double factor);
	void ResetBoundsUsingFactor (double factor);
	void ResetStartAndEndTimesForILSTests (double startC, double endC, DataSignal* startSignal);
	void ResetStartAndEndTimesForLadderILSTests (double startC, double endC, DataSignal* startSignal);
	int TestILS (int index, DataSignal* candidate);
	int TestLadderILS (int index, DataSignal* candidate);
	bool FindAndTestLadderILS (int index, DataSignal* startCandidate, DataSignal*& firstPeakFound);
	bool FindAndTestILS (int index, DataSignal* startCandidate, DataSignal*& mostAveragePeak);
	bool FindAndTestILS (int index, DataSignal* startCandidate, RGDList& foundPeaks, DataSignal*& mostAveragePeak);

protected:
	int mNumberOfCharacteristics;
	int mNum1;
	double* mILSLowBounds;
	double* mILSHighBounds;
	double* mILSAverage;
	double* mNormalizedDifferences;
	bool mSampleAdded;
	double mStart;
	double mEnd;
	double mWidth;
	double mCurrentStartForTest;
	double mCurrentEndForTest;
	double mCurrentWidthForTest;
	double mCurrentSlopeForTest;
	double mCurrentInterceptForTest;
	DataSignal* mStartSignalForTests;
	double mCurrentDistance;
	DataSignal* mClosestSignal;
	double mMaxWidth;
	double mMinWidth;
	double* mCharacteristicArray;
	double* mNormalizedCharacteristicDifferences;
	double* mLadderILSTargets;
	double* mLadderILSLowBounds;
	double* mLadderILSHighBounds;
	double mLadderWidth;
	double mLadderStart;
	double mLadderEnd;
	double mLadderFactor;
};


class ILSCandidate {

public:
	ILSCandidate (RGDList& peakList);
	~ILSCandidate ();

	double GetFirstTime () const { return mPeakList [0]->GetMean (); }
	double GetLastTime () const { return mPeakList [mSize - 1]->GetMean (); }

	double GetTimeSpan () const { return mWidth; }
	double CalculateNormalizedDotProduct (double* normalizedSpacing);
	void SaveSignalsToList (RGDList& finalCurveList);

protected:
	int mSize;
	DataSignal** mPeakList;
	double mWidth;
};


class LaneStandard : public RGPersistent {

	PERSISTENT_DECLARATION (LaneStandard)

public:
	LaneStandard ();
	LaneStandard (BaseLaneStandard* link, const RGString& xmlString);
	LaneStandard (const LaneStandard& std);
	virtual ~LaneStandard ();

	void ClearAndDestroy ();

	Boolean IsValid () const { return Valid; }
	RGString GetError () const { return Msg; }
	RGString GetLaneStandardName () const;
	RGString GetFamilyName () const { return mFamilyName; }
	RGString GetDyeName () const { return mDyeName; }
	int GetNumberOfCharacteristics () const;
	double GetMinimumCharacteristic () const;
	double GetMaximumCharacteristic () const;
	double GetLaneStandardTimeForCharacteristicNumber (int i) const;  // Assume the first characteristic is number 1 (not 0)
	const double* GetLaneStandardVector () const { return mLaneStandardTimes; }

	double* GetAltSpacingArray () { return mLink->GetAltSpacingArray (); }

	void SetLaneStandardName (const RGString& name);
	void SetFamilyName (const RGString& name) { mFamilyName = name; }
	void SetDyeName (const RGString& name) { mDyeName = name; }
	int SelectBestSubsetOfCharacteristics (RGDList& curveList, double& correlation);  // returns index of first (from 0) in contiguous list of characteristics
	int AssignLaneStandardSignals (RGDList& curveList);   // takes first mNumberOfCharacteristics elements

	double GetMaxILSWidth () const { return mILSHistory.GetMaxWidth (); }
	double GetMinILSWidth () const { return mILSHistory.GetMinWidth (); }
	double* GetILSNormalizedDifferences () const { return mILSHistory.GetNormalizedDifferences (); }
	double* GetLadderILSNormalizedDifferences () const { return mLadderILSHistory.GetLadderNormalizedDifferences (); }
	bool AddILSToHistory (double* times) { return mILSHistory.AddILS (times); }
	void ResetBoundsUsingFactorToILSHistory (double factor) { mILSHistory.ResetBoundsUsingFactor (factor); }
	void ResetStartAndEndTimesForILSTests (double startC, double endC, DataSignal* startSignal) { mILSHistory.ResetStartAndEndTimesForILSTests (startC, endC, startSignal); }
	void ResetStartAndEndTimesForLadderILSTests (double startC, double endC, DataSignal* startSignal) { mLadderILSHistory.ResetStartAndEndTimesForLadderILSTests (startC, endC, startSignal); }
	void ResetIdealCharacteristicsAndIntervalsForLadderILS (const double* actualArray, const double* differenceArray, double factor) { mLadderILSHistory.ResetIdealCharacteristicsAndIntervalsForLadderILS (actualArray, differenceArray, factor); }
	int TestILSUsingHistory (int index, DataSignal* candidate) { return mILSHistory.TestILS (index, candidate); }
	bool FindAndTestILS (int index, DataSignal* startCandidate, DataSignal*& mostAveragePeak) { return mILSHistory.FindAndTestILS (index, startCandidate, mostAveragePeak); }
	bool FindAndTestLadderILS (int index, DataSignal* startCandidate, DataSignal*& firstPeakFound) {  return mLadderILSHistory.FindAndTestLadderILS (index, startCandidate, firstPeakFound); }

	virtual int GetCharacteristicArray (const double*& array) const;  // returns array size or -1
	virtual int GetCharacteristicDifferenceArray (const double*& array) const;  // returns array size or -1
	virtual int GetUnnormalizedCharacteristicDifferenceArray (const double*& array) const;  // returns array size or -1
	virtual int GetLargeCharacteristicArray (const double*& array) const;  // returns array size or -1
	virtual int GetLargeDifferenceArray (const double*& array) const;  // returns array size or -1

	virtual double GetFirstIntervalFraction () const;
	virtual double GetSmallestIntervalFraction () const;

	virtual int GetMaxRelativeHeight () const;
	virtual double GetCorrelationAcceptanceThreshold () const;
	virtual double GetCorrelationAutoAcceptanceThreshold () const;

	virtual int GetNormsSquaredLeft (const double*& array) const;  // returns array size or -1
	virtual int GetNormsSquaredRight (const double*& array) const;  // returns array size or -1
	virtual int GetRelativeHeights (const int*& array) const;  // returns array size or -1
	virtual int GetOmissionArray (const double*& array) const;   // returns array size or -1

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void Write (RGFile& textOutput, const RGString& indent);
	static void SetILSHistoryNumberOfCharacteristics (int n) { mILSHistory.SetNumberOfCharacteristics (n); mLadderILSHistory.SetNumberOfCharacteristics (n); }

protected:
	BaseLaneStandard* mLink;
	Boolean Linked;
	Boolean Valid;
	double* mLaneStandardTimes;
	int mNumberOfCharacteristics;
	RGString Msg;
	RGString mFamilyName;
	RGString mDyeName;
	static ILSHistory mILSHistory;
	static ILSHistory mLadderILSHistory;
};


class PopulationMarkerSet : public RGPersistent {

	PERSISTENT_DECLARATION (PopulationMarkerSet)

public:
	PopulationMarkerSet ();
	PopulationMarkerSet (BasePopulationMarkerSet* link, const RGString& xmlString);
	PopulationMarkerSet (const PopulationMarkerSet& markerSet);
	virtual ~PopulationMarkerSet ();

	void ClearAndDestroy ();

	Boolean RepresentsMarkerSet (const RGString& markerSet) const;
	RGString GetMarkerSetName () const;
	Boolean IsValid () const { return Valid; }
	RGString GetErrorString () const { return ErrorString; }
	Locus* FindLocus (const RGString& name);
	int GetLaneStandardChannel () const;
	int GetNumberOfChannels () const;
	double GetMinimumILSBPSearchRegion ();

	void BuildChannelAndLocusListForHeader (RGString& headerList);

	void SetMarkerSetName (const RGString& name);
	void SetLaneStandard (LaneStandard* laneStd);

	void ResetLocusList ();
	Locus* GetNextLocus ();
	Locus* GetNextLocusForChannel (int channel);
	LaneStandard* GetLaneStandard () { return mLaneStandard; }
	RGString GetLaneStandardName () const { return mLink->GetLaneStandardName (); }
	const int* GetChannelMap () const { return mLink->GetChannelMap (); }

	RGString GetFileNameSuffix () const { return mLink->GetFileNameSuffix (); }
	RGString GetGenotypeSuffix () const { return mLink->GetGenotypeSuffix (); }
	RGString GetDirectorySearchString () const { return mLink->GetDirectorySearchString (); }

	virtual int AnalyzeLoci ();  // virtual ???
//	int BuildILSSplineBPToTime (const SupplementaryData& extraData);  // ???
	
	virtual const DataSignal* GetCurve (const RGString& locus, const RGString& allele, int& channel);  // virtual ???
//	double GetTimeForCorrespondingBioID (int bioID, int channel);  // ????

	virtual int GetLaneStandardCharacteristicArray (const double*& array) const;  // returns array size or -1
	virtual int GetLaneStandardCharacteristicDifferenceArray (const double*& array) const;  // returns array size or -1

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void Write (RGFile& textOutput, const RGString& indent);
	static int SelectBestSubsetOfCharacteristics (RGDList& set, const double* idealDifferences, int setSize, double& correlation);  // returns displacement in list
	static double VectorDotProductWithDifferences (const double* array1, const double* idealNormalizedDifferences, int size);
	static int SearchIncrementallyForBestSubsetOfCharacteristics (RGDList& set, RGDList& artifacts, const double* idealDifferences, int setSize, const double* extremePoints, double& correlation,
		double correlationCriterion);
	static int SearchTwoSidedForBestSubsetOfCharacteristics (RGDList& set, RGDList& artifacts, const double* idealDifferences, int setSize, const double* setPoints, double& correlation,
		double correlationCriterion);
	static int SearchTwoSidedWithLeadForBestSubsetOfCharacteristics (RGDList& set, RGDList& artifacts, const double* idealDifferences, int setSize, const double* setPoints, double& correlation,
		double correlationCriterion);
	static int SearchRecursivelyForBestSubset (RGDList& set, RGDList& artifacts, IdealControlSetInfo& ideal, double& correlation,
		double correlationCriterion, int rightAndLeftStartPts, double maxHeight);
	static int SearchRecursivelyForBestLadderLocusSubset (RGDList& set, RGDList& artifacts, IdealControlSetInfo& ideal, double& correlation,
		double correlationCriterion, int rightAndLeftStartPts, double maxHeight);

	static int PareDownSignalListBasedOnHeight (RGDList& set, RGDList& artifacts, int remainingElements);
	static double RecalculateMaxPeakBasedOnRelativeHeightInfo (RGDList& set, double oldMaxPeak, const int* relativeHeights, int nILSSignals);
	static double RecalculateMaxPeakBasedOnRelativeHeightInfo (RGDList& set, double oldMaxPeak, const int* relativeHeights, int nILSSignals, int maxRelativeHeight, int nTargetCurves, double& minPeak);

	static void SetUserLaneStandardName (const RGString& name);

protected:
	BasePopulationMarkerSet* mLink;  // determines type of Marker Set, so no subclassing needed
	RGHashTable LocusListByName;  // contains objects of type Locus*
	RGDList LocusList;
	RGDListIterator* LocusIterator;
	Boolean Valid;
	RGString ErrorString;
	Boolean Linked;
	Locus mTarget;
	LaneStandard* mLaneStandard;

	Boolean BuildLocusList (const RGString& xmlString);
};



class LaneStandardCollection {

public:
	LaneStandardCollection ();
	virtual ~LaneStandardCollection ();

	Boolean IsValid () const { return Valid; }
	RGString GetErrorString () const { return ErrorString; }

	virtual int Initialize (const RGString& xmlString);

	LaneStandard* GetNamedLaneStandard (const RGString& laneStandardName);
	virtual BaseLaneStandard* GetNewBaseLaneStandard (const RGString& input);
	virtual void Write (RGFile& textOutput, const RGString& indent);

protected:
	RGHashTable LaneStandards;
	Boolean Valid;
	RGString ErrorString;
	LaneStandard mTarget;

	Boolean BuildLaneStandards (const RGString& xmlString);
};



class PopulationCollection {

public:
	PopulationCollection (RGFile& inputFile);
	PopulationCollection (const RGString& inputFileName);
	PopulationCollection (const RGString& inputDirectoryName, const RGString& markerSetName);
	virtual ~PopulationCollection ();

	Boolean IsValid () const { return Valid; }
	RGString GetErrorString () const { return ErrorString; }
	RGString GetGridData () const { return mGridData; }

	virtual int Initialize ();
	virtual int InitializeFromDifferentSets ();

	PopulationMarkerSet* GetNamedPopulationMarkerSet (const RGString& markerSetName);
	virtual BasePopulationMarkerSet* GetNewPopulationMarkerSet (const RGString& input);
	virtual LaneStandardCollection* GetNewLaneStandardCollection (const RGString& textInput);

	virtual void Write (RGFile& textOutput, const RGString& indent);

	static bool UseILSFamiliesInLadderFile () { return UseILSFamilies; }

protected:
	RGHashTable MarkerSets;  // contains objects of type PopulationMarkerSet*
	Boolean Valid;
	RGString ErrorString;
	PopulationMarkerSet mTargetSet;
	LaneStandardCollection* mLaneStandardCollection;
	RGString TheWholeSheBang;
	RGString mILSData;
	RGString mGridData;
	static bool UseILSFamilies;

	Boolean BuildMarkerSets (const RGString& textInput);
};


#endif  /*  _GENETICS_H_  */

