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
*  FileName: DataSignal.h
*  Author:   Robert Goor
*
*/
//
//   Header file for class DataSignal and subclasses.  These classes represent the measurement data and transformed 
// measurement data.  Methods allow simple transforms and basic searches for signatures
//

#ifndef _DATASIGNAL_H_
#define _DATASIGNAL_H_

#include "rgpersist.h"
#include "rgdlist.h"
#include "Notice.h"
#include "SmartNotice.h"
#include "SmartMessagingObject.h"


#include <list>
#include <set>
#include <math.h>

//
//   Required Parameters:
//       ParametricCurve::FitTolerance gives minimum inner product for matching standard deviation; default = .9995;
//       Gaussian::NumberOfSteps gives number of numerical integration steps for Gaussian integrals; default = 50;
//       NormalizedGaussian::SigmaTolerance gives distance between two standard deviation estimates for match; default = 1.5e-4;
//       Gaussian::SigmaWidth gives number of standard deviations on each side of mean for standard Gaussian and NormalizedGaussian; default = 10.0;
//       Gaussian::SampleSigmaWidth gives number of standard deviations on each side of mean for standard Gaussian and NormalizedGaussian for
//            building test intervals to search for characteristics; default = 3.5;
//       NormalizedGaussian::MaximumIterations gives the max number of iterations of Brent's algorithm for fitting standard deviation; default = 5;
//		 DoubleGaussian::DefaultSigmaRatio gives default value of ratio of sigmas for 2 Gaussians; can be overridden; default = 4.0;
//       DataSignal::SignalSpacing gives value of spacing between signal observations; default = 1.0;
//

const int _SAMPLEDDATA_ = 1001;
const int _PARAMETRICCURVE_ = 1002;
const int _GAUSSIAN_ = 1003;
const int _NORMALIZEDGAUSSIAN_ = 1004;
const int _COMPOSITECURVE_ = 1005;
const int _DOUBLEGAUSSIAN_ = 1009;
const int _SUPERGAUSSIAN_ = 1010;
const int _NORMALIZEDSUPERGAUSSIAN_ = 1011;
const int _DUALDOUBLEGAUSSIAN_ = 1012;
const int _CRATERSIGNAL_ = 1013;
const int _PEAKINFOFORCLUSTERS_ = 1021;
const int _SIMPLESIGMOIDSIGNAL_ = 1019;
const int _NEGATIVESIGNAL_ = 1020;
const int _NOISYPEAK_ = 1022;
const int _SPIKESIGNAL_ = 1023;

const double Pi = acos (-1.0);
const double sqrtPi = sqrt (Pi);
const double sqrtTwo = sqrt (2.0);
const double fourthRootOfPi = sqrt (sqrtPi);
const double halfPullupTolerance = 0.0375;
const double halfCraterPullupTolerance = 0.0525;
const double pullUpToleranceFactor = 7.0;  // makes fit factor approx. 1.0 at fit between 0.99 and 0.991

class RGFile;
class RGVinStream;
class RGVoutStream;
class TracePrequalification;
class DataInterval;
class DataSignal;
class NoiseInterval;
class RGTextOutput;
class RGString;
class SpecialLinearRegression;
class CoordinateTransform;
class Locus;
class RGHashTable;
class SmartMessageReporter;

PERSISTENT_PREDECLARATION (PeakInfoForClusters)

PERSISTENT_PREDECLARATION (SampledData)

PERSISTENT_PREDECLARATION (Gaussian)
PERSISTENT_PREDECLARATION (NormalizedGaussian)
PERSISTENT_PREDECLARATION (DoubleGaussian)
PERSISTENT_PREDECLARATION (SuperGaussian)
PERSISTENT_PREDECLARATION (NormalizedSuperGaussian)
PERSISTENT_PREDECLARATION (CompositeCurve)
PERSISTENT_PREDECLARATION (DualDoubleGaussian)
PERSISTENT_PREDECLARATION (CraterSignal)
PERSISTENT_PREDECLARATION (SimpleSigmoidSignal)
PERSISTENT_PREDECLARATION (NegativeSignal)
PERSISTENT_PREDECLARATION (NoisyPeak)
PERSISTENT_PREDECLARATION (SpikeSignal)


Boolean QuadraticRegression (const double* means, const double* sigmas, int N, double* parameters);
Boolean LinearRegression (const double* x, const double* y, int N, double* parameters);
int LessOrder (double a, double b, double c);


using namespace std;

struct SampleDataInfo {

	SampleDataInfo (const double* segL, const double* segC, const double* segR, int indL, int indC, int indR, int N, double spacing, double abscissaLeft);
	SampleDataInfo (const DataSignal& ds, int indL, int indC, int indR, int N);
	~SampleDataInfo ();

	const double* DataLeft;
	const double* DataCenter;
	const double* DataRight;
	int IndexLeft;
	int IndexCenter;
	int IndexRight;
	int NumberOfSamples;
	double Spacing;
	double AbscissaLeft;

	double GetMaxInInterval (int left, int right);
};


struct CompoundSignalInfo {

	DataSignal* first;
	DataSignal* last;
	bool xmlPeakInfoWritten;
	bool xmlArtifactInfoWritten;
	bool nameStringBuilt;
	RGString alleleName;
	int mean;
	int height;
	int bioID;

	CompoundSignalInfo (DataSignal* ds1, DataSignal* ds2);
};


struct ConcaveDownSet {

	ConcaveDownSet (int st, int en, double value, int pos) : mStart (st), mEnd (en), mMaxValue (value), mPosition (pos) {}
	~ConcaveDownSet () {}

	int mStart;
	int mEnd;
	double mMaxValue;
	int mPosition;
};


class InterchannelLinkage {

public:
	InterchannelLinkage (int nChannels);
	virtual ~InterchannelLinkage ();

	virtual bool AddDataSignal (DataSignal* newSignal);
	virtual void SetPrimaryDataSignal (DataSignal* primary) { mPrimarySignal = primary; }
	virtual bool RemoveDataSignal (DataSignal* oldSignal, Notice* primaryTarget, Notice* primaryReplace, Notice* secondaryTarget, Notice* secondaryReplace);
	virtual bool RecalculatePrimarySignal (Notice* primaryTarget, Notice* primaryReplace);
	virtual bool RemoveAll (Notice* primaryTarget, Notice* primaryReplace, Notice* secondaryTarget, Notice* secondaryReplace);

	virtual bool RemoveDataSignalSM (DataSignal* oldSignal, SmartNotice& primaryTarget, SmartNotice& primaryReplace, SmartNotice& secondaryTarget, SmartNotice& secondaryReplace);
	virtual bool RecalculatePrimarySignalSM (SmartNotice&  primaryTarget, SmartNotice&  primaryReplace);
	virtual bool RemoveAllSM (SmartNotice&  primaryTarget, SmartNotice&  primaryReplace, SmartNotice&  secondaryTarget, SmartNotice&  secondaryReplace);

	virtual bool RemoveDataSignalSM (DataSignal* oldSignal);
	virtual bool RemoveDataSignalFromSecondaryList (DataSignal* oldSignal);
	virtual bool RecalculatePrimarySignalSM ();
	virtual bool RemoveAllSM ();

	virtual bool RecalculatePrimarySignalBasedOnValiditySM ();
	virtual bool RemoveAllBasedOnValiditySM ();

	bool IsEmpty ();
	DataSignal* GetPrimarySignal ();
	void ResetSecondaryIterator () { mSecondaryIterator->Reset (); }
	DataSignal* GetNextSecondarySignal () { return (DataSignal*) (*mSecondaryIterator)(); }

	bool PrimarySignalHasChannel (int n) const;
	bool PrimaryHasLaserOffScaleSM () const;
	bool SecondaryHasLaserOffScaleSM ();
	bool AnySignalHasLaserOffScaleSM ();
	bool SecondaryIsSigmoidalSignalSM (int secondaryChannel);
	bool PossibleSecondaryPullupWithNoOffScaleSM (int primaryChannel, int secondaryChannel, double& secondaryRatio, bool& isSigmoidal);
	bool PossibleSecondaryPullupSM (int primaryChannel, int secondaryChannel, double& secondaryRatio, bool& isSigmoidal, DataSignal*& secondarySignal);
	int NumberOfSecondarySignals () const;
	int NumberOfSecondarySignalsAbovePrimaryThreshold (double threshold);
	int MapOutSignalProperties (double noiseMultiple, double primaryThreshold, double* channelNoiseLevels);
	DataSignal* GetSecondarySignalOnChannelIfNoSecondPrimary (int secondaryChannel);
	DataSignal* GetSecondarySignalOnChannel (int secondaryChannel);

protected:
	DataSignal* mPrimarySignal;
	RGDList mSecondarySignals;
	RGDListIterator* mSecondaryIterator;
	int mNChannels;

	int** mDirectedGraph;
	bool** mIsNegativePullup;
	double** mRatios;

	bool* mCanBePrimary;
	bool* mIsAboveNoiseThreshold;
	bool* mIsNegativePeak;
	double* mPeakHeights;
};


class PeakInfoForClusters : public RGPersistent {

	PERSISTENT_DECLARATION (PeakInfoForClusters)

public:
	PeakInfoForClusters ();
	PeakInfoForClusters (DataSignal* ds);
	PeakInfoForClusters (const PeakInfoForClusters& pifc);
	virtual ~PeakInfoForClusters ();

	double GetPeak () const { return mPeak; }
	double GetMean () const { return mMean; }
	bool IsPullup () const { return mIsPullup; }
	bool IsAboveMaxRFU () const { return mIsAboveMaxRFU; }
	DataSignal* GetDataSignal () const { return mParentSignal; }

	void SetInclude (bool inc) { mIncludeInBaseCluster = inc; }
	bool GetInclude () const { return mIncludeInBaseCluster; }

	void SetIndex (int index) { mIndex = index; }
	int GetIndex () const { return mIndex; }

	double Distance (const PeakInfoForClusters& pifc);
	double Distance (double peak, double mean);

	void SetChosen (bool value) { mChosen = value; }
	void SetProcessed (bool value) { mProcessed = value; }
	void ResetSelection () { mChosen = mProcessed = false; }

	bool IsChosen () const { return mChosen; }
	bool IsProcessed () const { return mProcessed; }
	bool IsChosenButNotProcessed () const { return mChosen && !mProcessed; }

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent* p) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	friend double GetDistance (const PeakInfoForClusters& p1, const PeakInfoForClusters& p2);

	static double GetHeightFactor () { return HeightFactor; }
	static void SetHeightFactor (double factor) { HeightFactor = factor; }

protected:
	double mPeak;
	double mMean;
	bool mIsPullup;
	bool mIsAboveMaxRFU;
	DataSignal* mParentSignal;
	bool mIncludeInBaseCluster;
	int mIndex;

	bool mChosen;
	bool mProcessed;

	static double HeightFactor;
};


bool LessHeight (const PeakInfoForClusters* p1, const PeakInfoForClusters* p2);
bool LessDistance (const PeakInfoForClusters* p1, const PeakInfoForClusters* p2);



class STRInterchannelLinkage : public InterchannelLinkage {

public:
	STRInterchannelLinkage (int nChannels);
	virtual ~STRInterchannelLinkage ();

	virtual bool RecalculatePrimarySignal (Notice* primaryTarget, Notice* primaryReplace);

};



class DataSignal : public SmartMessagingObject {

ABSTRACT_DECLARATION (DataSignal)

public:
	DataSignal () : SmartMessagingObject (), Left (0.0), Right (1.0), LeftSearch (0.0), RightSearch (0.0), Fit (0.0), ResidualPower (1.0),
	MeanVariability (0.0), BioID (0.0), ApproximateBioID (-1.0), mApproxBioIDPrime (0.0), mWidth (-1.0),
	mNoticeObjectIterator (NewNoticeList), markForDeletion (false), mOffGrid (false), mAcceptedOffGrid (false), signalLink (NULL), mPrimaryCrossChannelLink (NULL), 
	mDontLook (false), mTestLeftEndPoint (0.0), mTestRightEndPoint (0.0), mDataMode (0.0), mRawDataBelowMinHeight (false), mOsirisFitBelowMinHeight (false),
	mRawDataAboveMaxHeight (false), mOsirisFitAboveMaxHeight (false), mPossibleInterlocusAllele (false), 
	mLeftLocus (NULL), mRightLocus (NULL), mIsGraphable (true), mHasCrossChannelLink (false), mIsSaturationArtifact (false), mInterchannelLink (NULL),
	mCannotBePrimary (false), mBioIDLeft (0.0), mBioIDRight (0.0), mResidualLeft (0.0), mResidualRight (0.0), mPossibleInterAlleleLeft (false),
	mPossibleInterAlleleRight (false), mIsAcceptedTriAlleleLeft (false), mIsAcceptedTriAlleleRight (false), mIsOffGridLeft (false), mIsOffGridRight (false), mArea (0.0),
	mLocus (NULL), mMaxMessageLevel (1), mDoNotCall (false), mReportersAdded (false), mAllowPeakEdit (true), mCannotBePrimaryPullup (false), mMayBeUnacceptable (false),
	mHasRaisedBaseline (false), mBaseline (0.0), mIsNegativePeak (false), mPullupTolerance (halfPullupTolerance), mPrimaryRatios (NULL), mPullupCorrectionArray (NULL), 
	mPrimaryPullupInChannel (NULL), mPartOfCluster (false), mIsPossiblePullup (false), mIsNoisySidePeak (false), mNextSignal (NULL), mPreviousSignal (NULL), mCumulativeStutterThreshold (0.0), mIsShoulderSignal (false),
	mThisDataSegment (NULL), mWeakPullupVector (NULL), mIsPurePullup (NULL) {

		DataSignal::signalID++;
		mSignalID = DataSignal::signalID;
		InitializeSmartMessages ();
	}

	DataSignal (double left, double right) : SmartMessagingObject (), Left (left), Right (right), LeftSearch (left), 
	RightSearch (right), Fit (0.0), ResidualPower (1.0), MeanVariability (0.0), BioID (0.0), ApproximateBioID (-1.0), mApproxBioIDPrime (0.0), mWidth (-1.0), 
	mNoticeObjectIterator (NewNoticeList), markForDeletion (false), mOffGrid (false), mAcceptedOffGrid (false), signalLink (NULL), 
	mPrimaryCrossChannelLink (NULL), mDontLook (false), mTestLeftEndPoint (0.0), mTestRightEndPoint (0.0), mDataMode (0.0), mRawDataBelowMinHeight (false), 
	mOsirisFitBelowMinHeight (false), mRawDataAboveMaxHeight (false), mOsirisFitAboveMaxHeight (false), mPossibleInterlocusAllele (false), 
	mLeftLocus (NULL), mRightLocus (NULL), mIsGraphable (true), mHasCrossChannelLink (false), mIsSaturationArtifact (false), mInterchannelLink (NULL),
	mCannotBePrimary (false), mBioIDLeft (0.0), mBioIDRight (0.0), mResidualLeft (0.0), mResidualRight (0.0), mPossibleInterAlleleLeft (false),
	mPossibleInterAlleleRight (false), mIsAcceptedTriAlleleLeft (false), mIsAcceptedTriAlleleRight (false), mIsOffGridLeft (false), mIsOffGridRight (false), mArea (0.0),
	mLocus (NULL), mMaxMessageLevel (1), mDoNotCall (false), mReportersAdded (false), mAllowPeakEdit (true), mCannotBePrimaryPullup (false), mMayBeUnacceptable (false),
	mHasRaisedBaseline (false), mBaseline (0.0), mIsNegativePeak (false), mPullupTolerance (halfPullupTolerance), mPrimaryRatios (NULL), mPullupCorrectionArray (NULL), 
	mPrimaryPullupInChannel (NULL), mPartOfCluster (false), mIsPossiblePullup (false), mIsNoisySidePeak (false), mNextSignal (NULL), mPreviousSignal (NULL), mCumulativeStutterThreshold (0.0), mIsShoulderSignal (false),
	mThisDataSegment (NULL), mWeakPullupVector (NULL), mIsPurePullup (NULL) {

		DataSignal::signalID++;
		mSignalID = DataSignal::signalID;
		InitializeSmartMessages ();
	}



	DataSignal (const DataSignal& ds);
	DataSignal (const DataSignal& ds, CoordinateTransform* trans);
	virtual ~DataSignal () = 0;

	// Not message-related**********************************************************************************************************************************************************

	virtual DataSignal* MakeCopy (double mean) const { return NULL; }

	virtual DataSignal* GetPreviousLinkedSignal () { return NULL; }
	virtual DataSignal* GetNextLinkedSignal () { return NULL; }

	unsigned long GetSignalID () const { return mSignalID; }

	void MarkForDeletion (bool mark) { markForDeletion = mark; }
	bool GetMarkForDeletion () const { return markForDeletion; }

	void SetBaseline (double baseline) { mBaseline = baseline; }
	void SetHasRaisedBaseline (bool hasRaisedBaseline) { mHasRaisedBaseline = hasRaisedBaseline; }

	double GetBaseLine () const { return mBaseline; }
	bool HasRaisedBaseline () const { return mHasRaisedBaseline; }

	double GetBaselineRelativePeak () const { return (Peak () - mBaseline); }

	void ResetSignalLink ();
	void SetSignalLink (CompoundSignalInfo* link) { signalLink = link; }
	bool IsPartOfCompoundSignal () const { return signalLink != NULL; }
	CompoundSignalInfo* RemoveSignalLink ();

	void SetOffGrid (bool r);
	bool IsOffGrid () const { return mOffGrid; }

	void SetPeakIsNegative () { mIsNegativePeak = true; mIsGraphable = false; }
	bool IsNegativePeak () const { return mIsNegativePeak; }

	void SetMayBeUnacceptable (bool r) { mMayBeUnacceptable = r; }
	bool MayBeUnacceptable () const { return mMayBeUnacceptable; }

	void SetAcceptedOffGrid (bool r) { mAcceptedOffGrid = r; }
	bool IsAcceptedOffGrid () const { return mAcceptedOffGrid; }
	bool IsPartOfCluster () const { return mPartOfCluster; }
	void SetPartOfCluster (bool b) { mPartOfCluster = b; }

	int GetChannel () const { return mChannel; }
	void SetChannel (int channel) { mChannel = channel; }

	bool IsNoisySidePeak () const { return mIsNoisySidePeak; }
	void SetNoisySidePeak (bool v) { mIsNoisySidePeak = v; }

	double LeftEndPoint () const { return Left; }
	double RightEndPoint () const { return Right; }

	double GetBioID () const { return BioID; }
	void SetBioID (double id) { BioID = id; }

	double GetBioID (RGPersistent* locus) const;
//	int SetInterlocusBioID (double id, RGPersistent* locus);

	void SignalIsWithinExtendedLocus () { mPossibleInterlocusAllele = true; }
	bool IsWithinExtendedLocus () const { return mPossibleInterlocusAllele; }

	double GetBioIDResidual () const { return Residual; }
	void SetBioIDResidual (double residual) { Residual = residual; }

	double GetApproximateBioID () const { return ApproximateBioID; }
	void SetApproximateBioID (double id) { ApproximateBioID = id; }

	double GetApproxBioIDPrime () const { return mApproxBioIDPrime; }
	void SetApproxBioIDPrime (double idPrime) { mApproxBioIDPrime = idPrime; }

	void SetLeftEndPoint (double left) { Left = left; }
	void SetRightEndPoint (double right) { Right = right; }

	void SetTestLeftEndPoint (double left) { mTestLeftEndPoint = left; }
	void SetTestRightEndPoint (double right) { mTestRightEndPoint = right; }
	double LeftTestValue () const;
	double RightTestValue () const;

	double GetCurveFit () const { return Fit; }
	void SetCurveFit (double fit);
	double GetResidualPower () const { return ResidualPower; }
	double GetMeanVariability () const { return MeanVariability; }

	DataSignal* GetNextSignal () const { return mNextSignal; }
	DataSignal* GetPreviousSignal () const { return mPreviousSignal; }

	void SetNextSignal (DataSignal* ds) { mNextSignal = ds; }
	void SetPreviousSignal (DataSignal* ds) { mPreviousSignal = ds; }

	void SetNextPeak (double peak) { nextPeak = peak; }
	double GetNextPeak () const { return nextPeak; }

	void SetPreviousPeak (double peak) { previousPeak = peak; }
	double GetPreviousPeak () const { return previousPeak; }

	void SetDontLook (bool dl) { mDontLook = dl; }
	bool DontLook () const { return mDontLook; }

	void SetDataMode (double mode) { mDataMode = mode; }
	double GetDataMode () const { return mDataMode; }

	bool isOsirisPeakBelowMinimum () const { return mOsirisFitBelowMinHeight; }
	bool isRawDataBelowMinimum () const { return mRawDataBelowMinHeight; }

	void SetOsirisPeakBelowMinimum (bool belowMin) { mOsirisFitBelowMinHeight = belowMin; }
	void SetRawDataBelowMinimum (bool belowMin) { mRawDataBelowMinHeight = belowMin; }

	bool isOsirisPeakAboveMaximum () const { return mOsirisFitAboveMaxHeight; }
	bool isRawDataAboveMaximum () const { return mRawDataAboveMaxHeight; }

	void SetOsirisPeakAboveMaximum (bool aboveMax) { mOsirisFitAboveMaxHeight = aboveMax; }
	void SetRawDataAboveMaximum (bool aboveMax) { mRawDataAboveMaxHeight = aboveMax; }

	bool isMarginallyAboveMinimum () const;
	bool isMarginallyBelowMaximum () const;

	bool isSaturationArtifact () const { return mIsSaturationArtifact; }
	void SetSaturationArtifact (bool satArt) { mIsSaturationArtifact = satArt; }

	InterchannelLinkage* GetInterchannelLink () const { return mInterchannelLink; }
	void SetInterchannelLink (InterchannelLinkage* link) { mInterchannelLink = link; }

	bool cannotBePrimary () const { return mCannotBePrimary; }
	void SetCannotBePrimary (bool cannot) { mCannotBePrimary = cannot; }

	void SetPrimaryCrossChannelSignalLink (DataSignal* ds) { mPrimaryCrossChannelLink = ds; mHasCrossChannelLink = true;}
	void AddCrossChannelSignalLink (DataSignal* ds);
	DataSignal* GetPrimaryCrossChannelSignalLink () { return mPrimaryCrossChannelLink; }

	void SetShoulderSignal (bool val) { mIsShoulderSignal = val; }
	bool IsShoulderSignal () const { return mIsShoulderSignal; }

	void CreateWeakPullupVector (int numberOfChannels);
	bool HasWeakPullupInChannel (int channel);
	void RecordWeakPullupInChannel (int numberOfChannels, int weakPullupChannel);

	void RemoveAllCrossChannelSignalLinks ();

	bool IsSecondaryCrossChannelSignalLink () const;
	bool IsPrimaryCrossChannelSignalLink () const;
	bool HasCrossChannelSignalLink () const;
	bool HasEverHadCrossChannelSignalLink () const { return mHasCrossChannelLink; }

	bool IsGraphable () const { return mIsGraphable; }

	bool CannotBePrimaryPullup () const { return mCannotBePrimaryPullup; }

	double GetPullupFromChannel (int i) const;
	double GetTotalPullupFromOtherChannels (int numberOfChannels) const;
	void SetPullupFromChannel (int i, double value, int numberOfChannels);

	void SetIsPurePullupFromChannel (int i, bool value, int numberOfChannels);
	bool GetIsPurePullupFromChannel (int i);

	DataSignal* HasPrimarySignalFromChannel (int i) const;
	void SetPrimarySignalFromChannel (int i, DataSignal* ds, int numberOfChannels);
	bool HasAnyPrimarySignals (int numberOfChannels) const;
	void AddUncertainPullupChannel (int channel);
	bool UncertainPullupChannelListIsEmpty ();
	bool PullupChannelIsUncertain (int channel);
	RGString CreateUncertainPullupString ();

	void AddProbablePullups (RGDList& prototype);
	void RemoveProbablePullup (const DataSignal* removeSignal);
	bool ReconfigurePullupFromLinkForChannel (int channel);
	bool IsInProbablePullupList (const DataSignal* testSignal);

//	int GetHighestSeverityLevel () const { return mHighestSeverityLevel; }
	int GetHighestMessageLevel () const { return mHighestMessageLevel; }
	int GetMaxAllowedMessageLevel () const { return mMaxMessageLevel; }
	bool IsDoNotCall () const { return mDoNotCall; }
	void SetDoNotCall (bool value) { mDoNotCall = value; }
	void SetIsPossiblePullup (bool value) { mIsPossiblePullup = value; }
	void ResetIgnoreWidthTest () { mIgnoreWidthTest = false; }
	bool IgnoreWidthTest () const { return mIgnoreWidthTest; }

	Boolean FlankingPeakProportionExceeds (double fraction) {
		if ((Peak() < fraction * nextPeak) && (Peak() < fraction * previousPeak))
			return FALSE;
		return TRUE;
	}

	Boolean FlankingPeakSumProportionExceeds (double fraction) {
		if (Peak() < fraction * (nextPeak + previousPeak))
			return FALSE;
		return TRUE;
	}

	void SetRemainingSignals (int remaining) { mRemainingSignals = remaining; }
	int GetRemainingSignals () const { return mRemainingSignals; }

	void SetBioID (double id, int position);
	void SetAlleleName (const RGString& name, int position);
	void SetBioIDResidual (double residual, int position);
	void SetLocus (Locus* locus, int position);

	virtual void SetAlleleInformation (int position);
	int RemoveFromOtherExtendedLocus (Locus* keep);
	int PromoteOtherExtendedLocus (Locus* dontKeep);
	int RemoveFromAllLoci ();

	void SetCurrentDataInterval (const DataInterval* di);
	DataInterval* GetCurrentDataInterval () { return mThisDataSegment; }

	//bool IsPossibleInterlocusAlleleLeft () const { return mPossibleInterAlleleLeft; }
	//bool IsPossibleInterlocusAlleleRight () const { return mPossibleInterAlleleRight; }
	//bool IsAcceptedTriAlleleLeft () const { return mIsAcceptedTriAlleleLeft; }
	//bool IsAcceptedTriAlleleRight () const { return mIsAcceptedTriAlleleRight; }
	//bool IsAcceptedOLAlleleLeft () const { return mIsOffGridLeft; }
	//bool IsAcceptedOLAlleleRight () const { return mIsOffGridRight; }

	bool IsPossibleInterlocusAllele (int position) const;
	bool IsAcceptedTriAllele (int position) const;
	bool IsOffGrid (int position) const;

	void SetPossibleInterlocusAllele (int position, bool value);
	void SetAcceptedTriAllele (int position, bool value);
	void SetOffGrid (int position, bool value);

	//void SetPossibleInterlocusAlleleLeft (bool value) { mPossibleInterAlleleLeft = value; }
	//void SetPossibleInterlocusAlleleRight (bool value) { mPossibleInterAlleleRight = value; }
	//void SetAcceptedTriAlleleLeft (bool value) { mIsAcceptedTriAlleleLeft = value; }
	//void SetAcceptedTriAlleleRight (bool value) { mIsAcceptedTriAlleleRight = value; }
	//void SetAcceptedOLAlleleLeft (bool value) { mIsOffGridLeft = value; }
	//void SetAcceptedOLAlleleRight (bool value) { mIsOffGridRight = value; }

	double GetBioID (int position) const;
	RGString GetAlleleName (int position) const;
	double GetBioIDResidual (int position) const;
	const Locus* GetLocus (int position) const;

	static void SetSampleSpacing (double spacing) { SignalSpacing = spacing; }
	static double GetSampleSpacing () { return SignalSpacing; }

	static void SetDebugFlag (Boolean flag) { DebugFlag = flag; }
	static Boolean GetDebugFlag () { return DebugFlag; }

	static void SetMinimumHeight (double height) { minHeight = height; }
	static double GetMinimumHeight () { return minHeight; }
	static void SetMaximumHeight (double height) { maxHeight = height; }
	static double GetMaximumHeight () { return maxHeight; }

	static void SetAllOLAllelesAccepted (bool b) { ConsiderAllOLAllelesAccepted = b; }
	static bool GetAllOLAllelesAccepted () { return ConsiderAllOLAllelesAccepted; } 

	virtual RGString GetSignalType () const;

	virtual void SetPullupRatio (int channel, double ratio, int nChannels);
	virtual double GetPullupRatio (int channel);
	virtual void SetDisplacement (double disp) = 0;
	virtual void SetScale (double scale) = 0;
	virtual void SetPeak (double peak) {}
	virtual void SetScale (int curve, double scale) {}
	virtual double Value (double x) const = 0;
	virtual double Value (int n) const = 0;
	virtual void SetOrthogonalScale (int curve, double scale) {}
	virtual double Peak () const { return -DOUBLEMAX; }
	virtual double TroughHeight () const { return Peak (); }
	virtual Boolean CanBeNegative () const { return FALSE; }
	virtual int AddToSample (double* sample, double sampleLeft, double sampleRight) const { return -1; }
	virtual int AddToSampleArray (double* sample, double sampleLeft, double sampleRight, double verticalResolution) const { return -1; }
	virtual void ComputeTails (double& tailLeft, double& tailRight) const { tailLeft = tailRight = 0.0; }
	virtual double GetMean () const { return -DOUBLEMAX; }
	virtual double GetStandardDeviation () const { return -DOUBLEMAX; }
	virtual double GetWidth ();
	virtual double GetVariance () const { return -DOUBLEMAX; }
	virtual double GetScale (int curve) const { return -DOUBLEMAX; }
	virtual double GetOrthogonalScale (int curve) const { return -DOUBLEMAX; }
	virtual void RestrictToMaximum (double MaxValue) {}
	virtual int FindAndRemoveFixedOffset ();
	virtual DataSignal* CreateMovingAverageFilteredSignal (int window);
	virtual DataSignal* CreateThreeMovingAverageFilteredSignal (int window);
	virtual DataSignal* CreateAveragingFilteredSignal (int nPasses, int halfWidth, double noiseLevel, double noiseLevelStart, bool* changeArray, double& fractionOfChangedData, double splitTime);
	virtual double GetPullupToleranceInBP () const { return (mPullupTolerance + (2.0 * sin (0.5 * acos (Fit)) / 4.47)); }  // The trig expression corrects for poor fit - this is proportional to Hilbert Space distance 07/22/2014;
	                                                                                                                       // with a proportionality coefficient of 1 / 4.47, so that a fit of 0.999 has a correction of 0.01 (changed from 1/10 07/23/2014)
	virtual double GetPullupToleranceInBP (double noise) const { return (mPullupTolerance + (2.0 * sin (0.5 * acos (Fit)) / 4.47)); }  // see above
	virtual double GetPrimaryPullupDisplacementThreshold () { return 2.0; }  // this is 2 seconds, but should never be called
	virtual double GetPrimaryPullupDisplacementThreshold (double nSigmas) { return 2.0; }  // this is 2 seconds, but should never be called
	virtual void RecalculatePullupTolerance () {}
	virtual void ResetPullupTolerance (double p) { mPullupTolerance = p; }

	virtual bool IsUnimodal () const { return false; }
	virtual bool IsCraterPeak () const { return false; }
	virtual bool IsSigmoidalPeak () const { return false; }

	virtual double GetNoiseRange () const { return 0.0; }
	virtual void SetNoiseRange (double noise) {}

	virtual void SetAlleleName (const RGString& name);
	virtual RGString GetAlleleName () const;
	virtual bool HasAlleleName () const;

	virtual bool TestForMultipleSignals (DataSignal*& prev, DataSignal*& next);
	virtual bool TestForMultipleSignals (DataSignal*& prev, DataSignal*& next, int location);

	virtual bool TestIfNeighboringDataWithinRange (int testPosition, int neighborLimit, double range) { return false; }

	virtual void SetVirtualAlleleName (const RGString& name);
	virtual RGString GetVirtualAlleleName () const;

	virtual double ValueFreeBound (int n) const;
	virtual double ValueFreeBound (double x) const;

	void AddPrimaryStutterSignalToList (DataSignal* primary, int dir);
	void AddLeftPrimaryStutterSignalToList (DataSignal* primary, int dir);
	void AddRightPrimaryStutterSignalToList (DataSignal* primary, int dir);
	void AddToCumulativeStutterThreshold (double add) { mCumulativeStutterThreshold += add; }
	Boolean HasNoStutterLinks () const { return mStutterPrimaryList.IsEmpty (); }
	void RemoveAllStutterLinks ();
	void AddDataToStutterArtifactSM ();

	void AddStandardStutterSignalToList (DataSignal* stutter, int dir);
	void AddStutterSignalToList (DataSignal* stutter) { mHasStutterList.Append (stutter); }
	void AddStutterSignalToListLeft (DataSignal* stutter) { mHasStutterLeftList.Append (stutter); }
	void AddStutterSignalToListRight (DataSignal* stutter) { mHasStutterRightList.Append (stutter); }
	void AddStutterDisplacement (int disp, int dir);

	Boolean SignalIsPrimaryStutter (DataSignal* ds) { return mStutterPrimaryList.ContainsReference (ds); }
	Boolean SignalIsStandardStutter (DataSignal* ds) { return mHasStandardStutterList.ContainsReference (ds); }
	Boolean SignalHasStutter () { return !mHasStutterList.IsEmpty (); }
	Boolean SignalHasStutterFromLocusToLeft () { return !mHasStutterLeftList.IsEmpty (); }
	Boolean SignalHasStutterFromLocusToRight () { return !mHasStutterRightList.IsEmpty (); }
	void RemoveStutterLink (DataSignal* ds);

	int GetNumberOfPrimaryStutterSignals () const { return mStutterPrimaryList.Entries (); }
	int GetNumberOfLeftPrimaryStutterSignals () const { return mLeftStutterPrimaryList.Entries (); }
	int GetNumberOfRightPrimaryStutterSignals () const { return mRightStutterPrimaryList.Entries (); }
	//RGString GetStutterRatio () const;
	//RGString GetLeftStutterRatio () const;
	//RGString GetRightStutterRatio () const;
	double GetCumulativeStutterThreshold () const { return mCumulativeStutterThreshold; }

	void SetTempDataForOccludedPrimary (const RGString& str) { mTempDataForOccudedPrimary = str; }
	RGString GetTempDataForOccludedPrimary () const { return mTempDataForOccudedPrimary; }

	void SetTempDataForPrimaryNoPullup (const RGString& str) { mTempDataForPrimaryNoPullup = str; }
	RGString GetTempDataForPrimaryNoPullup () const { return mTempDataForPrimaryNoPullup; }

	void SetTempDataForPrimaryRawDataPullup (const RGString& str) { mTempDataForPrimaryRawDataPullup = str; }
	RGString GetTempDataForPrimaryRawDataPullup () const { return mTempDataForPrimaryRawDataPullup; }

	RGString CalculateAlleleNameFromILSBP_LF ();

	virtual bool LiesBelowHeightAt (double x, double height);
	virtual bool TestForIntersectionWithPrimary (DataSignal* primary);

	virtual const double* GetData () const { return NULL; }
	virtual int GetNumberOfSamples () const { return 0; }

	virtual double GetModeHeightAndLocationFromDataInterval (int& location) { location = 0; return 0.0; }
	virtual DataInterval* GetCurrentIntervalFromList () { return NULL; }

	virtual DataSignal* Project (double left, double right) const { return NULL; }
	virtual DataSignal* Project (const DataSignal* target) const { return NULL; }
	virtual void ProjectNeighboringSignals (double horizontalResolution, double verticalTolerance) {}

	virtual const DataSignal* BuildOverlappedSignal (double left, double right, RGDList& previous, const DataSignal* target) const {
		return target;
	}

	virtual void ResetCharacteristicsFromRight (TracePrequalification& trace, RGTextOutput& text, double minRFU, Boolean print = TRUE);
	virtual void ResetCharacteristicsFromLeft (TracePrequalification& trace, RGTextOutput& text, double minRFU, Boolean print = TRUE);

	virtual DataSignal* FindNextCharacteristicFromRight (const DataSignal& Signature, 
		double& fit, RGDList& previous) = 0;
	virtual DataSignal* FindNextCharacteristicFromLeft (const DataSignal& Signature, 
		double& fit, RGDList& previous) = 0;

	virtual const DataSignal* FindCharacteristicBetweenTwoPeaks (DataSignal* prevSignal, DataSignal* nextSignal, const DataSignal& signature, double& fit, double detectionThreshold, double analysisThreshold, double noiseThreshold) { return NULL; }

	virtual DataSignal* FindNextCharacteristicRetry (const DataSignal& Signature, double& fit, RGDList& previous) {
		return NULL;
	}

	virtual DataSignal* FindNextCharacteristicRetry (const DataSignal& Signature, double& fit, RGDList& previous, int dualCurve) {
		return NULL;
	}

	virtual DataSignal* FindCharacteristicAsymmetric (const DataSignal* Target, const DataInterval* Segment, 
		int windowSize, double& fit, RGDList& previous) const { return NULL; }

	virtual double TestConstantCharacteristicRetry () { return 0.0; }
	virtual double TestConstantCharacteristicRetry (double& height, int& left, int& right) { return 0.0; }
	virtual double TestConstantCharacteristicRetry (DataSignal* currentSignal, double& height, int& left, int& right) { return 0.0; }
	virtual double InnerProductWithConstantFunction (int left, int right, double& height) const { return 0.0; }
	virtual bool HasAtLeastOneLocalMinimum () { return false; }
	virtual bool TestForBiasedFit (const DataSignal* currentSignal, double limit) { return false; }
	virtual DataSignal* FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, 
		int windowSize, double& fit, RGDList& previous) const;

	virtual NoiseInterval* GetNextNoiseInterval () { return NULL; }

	virtual double Centroid () const { return -DOUBLEMAX; }
	virtual double Centroid (double left, double right) const { return -DOUBLEMAX; }
	virtual double SecondMoment (double left, double right) const { return -DOUBLEMAX; }
	virtual double ThirdMoment (double left, double right) const { return 0.0; }

	virtual void ExtendDomain (double left, double right) = 0;
	virtual void ExtendDomain (const DataSignal* model) = 0;

	virtual DataSignal* GetResidual (const DataSignal* base, double coeff) = 0;
	virtual int Add (list<DataSignal*>& WaveList) = 0;  // list of pointers to DataSignal
	virtual int TruncateToResolution (double resolution) = 0;
	virtual DataSignal* Digitize (int numSamples, double left, double resolution) = 0;
	virtual DataSignal* Digitize (int numSamples, double left) { return NULL; }
	virtual DataSignal* BuildSample (int numSamples, double left, double resolution) = 0;

	virtual double ResetWindowedNorm2 (int nleft, int nright) { return -1.0; }
	virtual double AdvanceWindowedNorm2Left () { return -1.0; }
	virtual double AdvanceWindowedNorm2Right () { return -1.0; }

	virtual int SampleAndSave () { return -1; }
//	virtual double InnerProductWithOffset (double offset, double* coeffs, const SampleDataInfo& info) const { return -DOUBLEMAX; }
//	virtual double InnerProductWithStandardSpacing (double* coeffs, const SampleDataInfo& info) const { return -DOUBLEMAX; }

	// the following are all L2 norms
	virtual double Norm () = 0;
	virtual double Norm (double left, double right) = 0;
	virtual double Norm2 () = 0;  // norm squared
	virtual double Norm2 (double left, double right) = 0;
	virtual DataSignal* Normalize (double& norm) { return NULL; }
	virtual DataSignal* Normalize (double left, double right, double& norm) { return NULL; }

	virtual double OneNorm () { return -1.0; }
	virtual double OneNorm (double left, double right) { return -1.0; }
	virtual int FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean) { return -1; }
	virtual int SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment) { return -1; }

	virtual double TheoreticalArea () { return mArea; }
	virtual void CalculateTheoreticalArea () {;}

	virtual int FirstMoment (double left, double right, double& twoNorm2, double& mean) { return -1; }
	virtual int SecondMoment (double left, double right, double& twoNorm2, double& mean, double& twoMoment) const { return -1; }

	virtual void Report (RGTextOutput& text, const RGString& indent) {}
	virtual void ReportAbbreviated (RGTextOutput& text, const RGString& indent);

	virtual void BuildAlleleString (RGString& alleleString, const RGString& delim);

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;


	//******************************************************************************************************************************************************************************

	// Pre-object Message-Related***************************************************************************************************************************************************

	virtual int AddNoticeToList (unsigned long level, const RGString& label, const RGString& notice);
	virtual Boolean ReportNotices (RGTextOutput& text, const RGString& indent, const RGString& delim);
	virtual Boolean ReportNotices (RGFile& file, const RGString& indent, const RGString& delim);
	virtual void ClearNotices ();
	virtual int NumberOfNotices ();


	//******************************************************************************************************************************************************************************


	// Legacy Message-Related*******************************************************************************************************************************************************

	void RemoveCrossChannelSignalLink (Notice* primaryTarget, Notice* secondaryTarget, Notice* primaryReplace, Notice* secondaryReplace);  // from outside	//$

	void RemovePrimaryCrossChannelSignalLink (Notice* target, Notice* replace, const DataSignal* remove);  // for primary, from secondary	//$
	void RemoveSecondaryCrossChannelSignalLink (Notice* target, Notice* replace);  // for secondary, from Primary	//$
	void ReplaceNotice (Notice* target, Notice* replace);	//$

	virtual int AddNoticeToList (Notice* newNotice);	//$
	virtual Boolean IsNoticeInList (const Notice* target);	//$
	virtual Notice* GetNotice (const Notice* target);	//$
	virtual Boolean ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim);	//$
	virtual void ClearNoticeObjects ();	//$
	virtual int NumberOfNoticeObjects ();	//$
	virtual Notice* RemoveNotice (const Notice* target);	//$

	void ResetNotices () { mNoticeObjectIterator.Reset (); }	//$
	Notice* GetNextNoticeObject () { return (Notice*)mNoticeObjectIterator (); }	//$

	virtual void WritePeakInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& bracketTag, const RGString& locationTag);	//$
	virtual void WriteArtifactInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& bracketTag, const RGString& locationTag);	//$
	virtual void WriteTableArtifactInfoToXML (RGTextOutput& text, RGTextOutput& tempText, const RGString& indent, const RGString& bracketTag, const RGString& locationTag);	//$

	//******************************************************************************************************************************************************************************


	// Smart Message functions*******************************************************************************
	//*******************************************************************************************************

	virtual int GetObjectScope () const { return 1; }

	virtual void SetMessageValue (int scope, int location, bool value);
	virtual void SetMessageValue (const SmartNotice& notice, bool value);
	virtual void SetMessageValue (int scope, int location, bool value, bool useVirtualMethod);

	bool EvaluateSmartMessagesForStage (int stage);
	bool EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage);
	bool SetTriggersForAllMessages (bool* const higherMsgMatrix, RGHashTable* messageDataTable, int higherScope, int stage);
	bool SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage);
	bool EvaluateAllReports (bool* const reportMatrix);
	bool TestAllMessagesForCall ();
	bool EvaluateAllReportLevels (int* const reportLevelMatrix);

	void SetAllowPeakEdit (bool allow) { mAllowPeakEdit = allow; }
	void CapturePullupDataFromSM (DataSignal* prevSignal, DataSignal* nextSignal);

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);
	virtual RGString GetDebugIDIndent () const;

	virtual void AssociateDataWithPullMessageSM (int nChannels);

	virtual void CaptureSmartMessages (const DataSignal* signal);
	virtual void CaptureSmartMessages ();

	virtual bool TestForMultipleSignalsWithinLocus (DataSignal*& prev, DataSignal*& next, int location, bool isAmel, double adenylationLimit);
	virtual bool TestSignalGroupsWithinILS (double ilsLeft, double ilsRight, double minBioID);

	virtual void WriteSmartPeakInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& bracketTag, const RGString& locationTag);
	virtual void WriteSmartArtifactInfoToXML (RGTextOutput& text, const RGString& indent, const RGString& bracketTag, const RGString& locationTag);
	virtual void WriteSmartTableArtifactInfoToXML (RGTextOutput& text, RGTextOutput& tempText, const RGString& indent, const RGString& bracketTag, const RGString& locationTag);

	virtual bool ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim);

	virtual int AddAllSmartMessageReporters ();
	virtual int AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects);
	virtual void InitializeMessageData ();

	void RemoveCrossChannelSignalLinkSM ();
	void RemovePrimaryCrossChannelSignalLinkSM (DataSignal* remove);
	void RemoveSecondaryCrossChannelSignalLinkSM ();
	void RemoveAllCrossChannelSignalLinksSM ();
	bool IsPullupFromChannelsOtherThan (int primaryChannel, int numberOfChannels) const;
	bool SetPullupMessageDataSM (int numberOfChannels);
	bool SetPrimaryPullupMessageDataSM (int numberOfChannels);
	bool HasPullupFromSameChannelAsSM (DataSignal* ds, int numberOfChannels);

	RGString GetDataForNoticeSM (SmartNotice& sn);
	void CapturePullupCorrections (DataSignal* ds);

	static void CreateInitializationData (int scope);
	static void InitializeMessageMatrix (bool* matrix, int size);
	static void ClearInitializationMatrix () { delete[] InitialMatrix; InitialMatrix = NULL; }
	static int GetScope () { return 1; }
	static void SetNumberOfChannels (int n) { NumberOfChannels = n; }
	static bool IsNegativeOrSigmoid (DataSignal* ds);
	static bool PeakCannotBePurePullup (DataSignal* pullup, DataSignal* primary);
	static void SetNumberOfIntervalsForConcaveDownAlgorithm (int n) { NumberOfIntervalsForConcaveDownAlgorithm = n; }


	//*******************************************************************************************************
	//*******************************************************************************************************

protected:
	double Left;
	double Right;
	double LeftSearch;
	double RightSearch;
	double Fit;
	double ResidualPower;
	double MeanVariability;
	RGDList NoticeList;
	double BioID;
	double ApproximateBioID;
	double mApproxBioIDPrime;
	double Residual;
	double mWidth;

	double nextPeak;
	double previousPeak;

	RGDList NewNoticeList;
	RGDListIterator mNoticeObjectIterator;
	int mChannel;
	int mRemainingSignals;
	bool markForDeletion;
	bool mOffGrid;
	bool mAcceptedOffGrid;

	CompoundSignalInfo* signalLink;
	DataSignal* mPrimaryCrossChannelLink;
	RGDList mCrossChannelSignalLinks;
	bool mDontLook;
	double mTestLeftEndPoint;
	double mTestRightEndPoint;
	double mDataMode;

	bool mRawDataBelowMinHeight;
	bool mOsirisFitBelowMinHeight;

	bool mRawDataAboveMaxHeight;
	bool mOsirisFitAboveMaxHeight;
	bool mPossibleInterlocusAllele;

	Locus* mLeftLocus;
	Locus* mRightLocus;

	bool mIsGraphable;
	bool mHasCrossChannelLink;

	bool mIsSaturationArtifact;
	InterchannelLinkage* mInterchannelLink;
	bool mCannotBePrimary;

	double mBioIDLeft;
	double mBioIDRight;
	RGString mAlleleNameLeft;
	RGString mAlleleNameRight;
	double mResidualLeft;
	double mResidualRight;
	bool mPossibleInterAlleleLeft;  // within extended locus and above fractional filter
	bool mPossibleInterAlleleRight;  // within extended locus and above fractional filter
	bool mIsAcceptedTriAlleleLeft;
	bool mIsAcceptedTriAlleleRight;
	RGString mAlleleName;
	bool mIsOffGridLeft;
	bool mIsOffGridRight;
	unsigned long mSignalID;
	double mArea;
	Locus* mLocus;
	int mMaxMessageLevel;
	bool mDoNotCall;
	bool mReportersAdded;
	bool mAllowPeakEdit;
	bool mCannotBePrimaryPullup;
	bool mMayBeUnacceptable;

	bool mHasRaisedBaseline;
	double mBaseline;
	bool mIsNegativePeak;

	double mPullupTolerance;
	double* mPrimaryRatios;
	double* mPullupCorrectionArray;
	DataSignal** mPrimaryPullupInChannel;
	bool mPartOfCluster;
	bool mIsPossiblePullup;
	bool mIgnoreWidthTest;

	set<int> mUncertainPullupChannels;
	RGDList mProbablePullupPeaks;
	bool mIsNoisySidePeak;

	DataSignal* mNextSignal;
	DataSignal* mPreviousSignal;

	RGDList mStutterPrimaryList;
	RGDList mLeftStutterPrimaryList;
	RGDList mRightStutterPrimaryList;
	RGDList mHasStandardStutterList;
	RGDList mHasStutterList;
	RGDList mHasStutterLeftList;
	RGDList mHasStutterRightList;
	list<int> mStutterDisplacements;
	double mCumulativeStutterThreshold;
	bool mIsShoulderSignal;
	DataInterval* mThisDataSegment;
	bool* mWeakPullupVector;
	list<int> mWeakPullupChannels;
	bool* mIsPurePullup;

	RGString mTempDataForOccudedPrimary;
	RGString mTempDataForPrimaryNoPullup;
	RGString mTempDataForPrimaryRawDataPullup;

	static double SignalSpacing;
	static Boolean DebugFlag;
	static double minHeight;
	static double maxHeight;
	static unsigned long signalID;
	static bool* InitialMatrix;
	static bool ConsiderAllOLAllelesAccepted;
	static int NumberOfChannels;
	static int NumberOfIntervalsForConcaveDownAlgorithm;

	// Smart Message functions*******************************************************************************
	//*******************************************************************************************************

};


class SampledData : public DataSignal {

PERSISTENT_DECLARATION (SampledData)

public:
	SampledData ();
	SampledData (int numSamples, double left, double right, double* samples, bool deleteArray = true);
	SampledData (const DataSignal& base, const DataSignal& second, double baseCoeff, double left, double right);
	SampledData (const SampledData& sd);
	virtual ~SampledData ();

	virtual RGString GetSignalType () const;

	virtual void SetDisplacement (double disp);
	virtual void SetScale (double scale);
	virtual double Value (double x) const;
	virtual double Value (int n) const;
    virtual int AddToSample (double* sample, double sampleLeft, double sampleRight) const;
	virtual double GetNoiseRange () const { return mNoiseRange; }
	virtual void SetNoiseRange (double noise) { mNoiseRange = noise; }


	virtual double ValueFreeBound (int n) const;
	virtual double ValueFreeBound (double x) const;
	virtual const double* GetData () const;
	virtual int GetNumberOfSamples () const;
	virtual void RestrictToMaximum (double MaxValue);

	virtual int FindAndRemoveFixedOffset ();
	double* CreateMovingAverageFilteredArray (int window, double* inputArray);
	virtual DataSignal* CreateMovingAverageFilteredSignal (int window);
	virtual DataSignal* CreateThreeMovingAverageFilteredSignal (int minWindow);
	virtual DataSignal* CreateAveragingFilteredSignal (int nPasses, int halfWidth, double noiseLevel, double noiseLevelStart, bool* changeArray, double& fractionOfChangedData, double splitTime);

	virtual DataSignal* Project (double left, double right) const;
	virtual DataSignal* Project (const DataSignal* target) const;

	virtual void ResetCharacteristicsFromRight (TracePrequalification& trace, RGTextOutput& text, double minRFU, Boolean print = TRUE);
	virtual void ResetCharacteristicsFromLeft (TracePrequalification& trace, RGTextOutput& text, double minRFU, Boolean print = TRUE);

	virtual DataSignal* FindNextCharacteristicFromRight (const DataSignal& Signature, 
		double& fit, RGDList& previous);
	virtual DataSignal* FindNextCharacteristicFromLeft (const DataSignal& Signature, 
		double& fit, RGDList& previous);
	virtual const DataSignal* FindCharacteristicBetweenTwoPeaks (DataSignal* prevSignal, DataSignal* nextSignal, const DataSignal& signature, double& fit, double detectionThreshold, double analysisThreshold, double noiseThreshold);

	virtual DataSignal* FindNextCharacteristicRetry (const DataSignal& Signature, double& fit, RGDList& previous);
	virtual DataSignal* FindNextCharacteristicRetry (const DataSignal& Signature, double& fit, RGDList& previous, int dualCurve);

	virtual double GetModeHeightAndLocationFromDataInterval (int& location);
	virtual DataInterval* GetCurrentIntervalFromList ();
	virtual double TestConstantCharacteristicRetry ();
	virtual double InnerProductWithConstantFunction (int left, int right) const;

	virtual double TestConstantCharacteristicRetry (double& height, int& left, int& right);
	virtual double TestConstantCharacteristicRetry (DataSignal* currentSignal, double& height, int& left, int& right);
	virtual bool HasAtLeastOneLocalMinimum ();
	virtual bool TestForBiasedFit (const DataSignal* currentSignal, double limit);
	double InnerProductWithConstantFunction (int left, int right, double& height) const;

	virtual bool TestIfNeighboringDataWithinRange (int testPosition, int neighborLimit, double range);

	virtual NoiseInterval* GetNextNoiseInterval ();

	virtual double Centroid () const;
	virtual double Centroid (double left, double right) const;
	virtual double SecondMoment (double left, double right) const;
	virtual double ThirdMoment (double left, double right) const;

	virtual void ExtendDomain (double left, double right);
	virtual void ExtendDomain (const DataSignal* model);

	virtual DataSignal* GetResidual (const DataSignal* base, double coeff);
	virtual int Add (list<DataSignal*>& WaveList);  // list of pointers to DataSignal
	virtual int TruncateToResolution (double resolution);
	virtual DataSignal* Digitize (int numSamples, double left, double resolution);
	virtual DataSignal* BuildSample (int numSamples, double left, double resolution);

	virtual double ResetWindowedNorm2 (int nleft, int nright);
	virtual double AdvanceWindowedNorm2Left ();
	virtual double AdvanceWindowedNorm2Right ();

	virtual double Norm ();
	virtual double Norm (double left, double right);
	virtual double Norm2 ();  // norm squared
	virtual double Norm2 (double left, double right);
	virtual DataSignal* Normalize (double& norm);
	virtual DataSignal* Normalize (double left, double right, double& norm);

	virtual double OneNorm ();
	virtual double OneNorm (double left, double right);
	virtual int FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean);
	virtual int SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment);

	virtual int FirstMoment (double left, double right, double& twoNorm2, double& mean);
	virtual int SecondMoment (double left, double right, double& twoNorm2, double& mean, double& twoMoment) const;

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	static void SetPeakFractionForFlatCurveTest (double fraction) { PeakFractionForFlatCurveTest = fraction; }
	static double GetPeakFractionForFlatCurveTest () { return PeakFractionForFlatCurveTest; }

	static void SetPeakLevelForFlatCurveTest (double level) { PeakLevelForFlatCurveTest = level; }
	static double GetPeakLevelForFlatCurveTest () { return PeakLevelForFlatCurveTest; }
	static void SetIgnoreNoiseAnalysisAboveDetectionInSmoothing (bool ignore) { IgnoreNoiseAnalysisAboveDetectionInSmoothing = ignore; }
	static bool GetIgnoreNoiseAnalysisAboveDetectionInSmoothing () { return IgnoreNoiseAnalysisAboveDetectionInSmoothing; }

	static void SetDetectionRFU (double d) { DetectionRFU = d; }
	static double GetDetectionRFU () { return DetectionRFU; }

protected:
	int NumberOfSamples;
	double* Measurements;
	double Spacing;

	double WindowedNorm;
	int WindowLeft;
	int WindowRight;
	double OldLeft;
	double OldRight;
	int TraceWindowSize;

	double norm2;

	RGDList PeakList;
	RGDList NoiseList;
	RGDListIterator* PeakIterator;
	RGDListIterator* NoiseIterator;
	double NoiseThreshold;
	SpecialLinearRegression* slr;
	bool mDeleteArray;
	double mNoiseRange;

	static double PeakFractionForFlatCurveTest;
	static double PeakLevelForFlatCurveTest;
	static bool IgnoreNoiseAnalysisAboveDetectionInSmoothing;
	static double DetectionRFU;

	int GetSampleNumber (double abscissa) const;
	double GetNorm2 (int nleft, int nright);
	double FindApproximateIntersection (DataSignal* prevSignal, DataSignal* nextSignal);
};


class ParametricCurve : public DataSignal {

ABSTRACT_DECLARATION (ParametricCurve)

public:
	ParametricCurve ();
	ParametricCurve (double left, double right);
	ParametricCurve (const ParametricCurve& pc);
	ParametricCurve (const ParametricCurve& pc, CoordinateTransform* trans);
	virtual ~ParametricCurve () = 0;

	static double GetFitTolerance () { return FitTolerance; }
	static void SetFitTolerance (double tol) { FitTolerance = tol; }

	static double GetTriggerForArtifactTest () { return TriggerForArtifactTest; }
	static void SetTriggerForArtifactTest (double tol) { TriggerForArtifactTest = tol; }

	static double GetMinimumFitThreshold () { return MinimumFitThreshold; }
	static void SetMinimumFitThreshold (double tol) { MinimumFitThreshold = tol; }

	static double GetAbsoluteMinimumFit () { return AbsoluteMinimumFit; }
	static void SetAbsoluteMinimumFit (double tol) { AbsoluteMinimumFit = tol; }

	static double GetSigmaForSignature () { return SigmaForSignature; }
	static void SetSigmaForSignature (double sigma) { SigmaForSignature = sigma; }

	virtual double Value (double x) const = 0;
	virtual double Value (int n) const { return -1.0; }
	virtual int AddToSample (double* sample, double sampleLeft, double sampleRight) const;
	virtual int AddToSampleArray (double* sample, double sampleLeft, double sampleRight, double verticalResolution) const;
	virtual void ComputeTails (double& tailLeft, double& tailRight) const { tailLeft = tailRight = 0.0; }

	virtual void SetDisplacement (double disp);
	virtual void SetScale (double scale);
	virtual double GetScale (int curve) const;
	virtual bool IsUnimodal () const { return true; }

	virtual bool HasAlleleName () const;

	virtual void SetVirtualAlleleName (const RGString& name);
	virtual RGString GetVirtualAlleleName () const;

	virtual double Centroid () const;

	virtual void ExtendDomain (double left, double right);
	virtual void ExtendDomain (const DataSignal* model);

	virtual DataSignal* GetResidual (const DataSignal* base, double coeff);
	virtual int Add (list<DataSignal*>& WaveList);  // list of pointers to DataSignal

	virtual int TruncateToResolution (double resolution);
	virtual DataSignal* Digitize (int numSamples, double left, double resolution);
	virtual DataSignal* BuildSample (int numSamples, double left, double resolution);
	virtual const DataSignal* BuildOverlappedSignal (double left, double right, RGDList& previous, const DataSignal* target) const;

//	virtual int SampleAndSave () { return -1; }
//	virtual double InnerProductWithOffset (double offset, double* coeffs, const SampleDataInfo& info) const { return -DOUBLEMAX; }
//	virtual double InnerProductWithStandardSpacing (double* coeffs, const SampleDataInfo& info) const { return -DOUBLEMAX; }

	virtual DataSignal* FindNextCharacteristicFromRight (const DataSignal& Signature, 
		double& fit, RGDList& previous);
	virtual DataSignal* FindNextCharacteristicFromLeft (const DataSignal& Signature, 
		double& fit, RGDList& previous);

	virtual int FirstMoment (double left, double right, double& twoNorm2, double& mean) { return -1; }
	virtual int SecondMoment (double left, double right, double& twoNorm2, double& mean, double& twoMoment) const { return -1; }

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	double Displacement;
	double Scale;
	RGString mWouldBeAlleleName;
	static double FitTolerance;

	static double TriggerForArtifactTest;
	static double MinimumFitThreshold;
	static double SigmaForSignature;
	static double AbsoluteMinimumFit;

	double TruncateWithResolution (double value, double resolution);
};


class Gaussian : public ParametricCurve {

PERSISTENT_DECLARATION (Gaussian)

public:
	Gaussian ();
	Gaussian (double mean, double standardDeviation);
	Gaussian (double left, double right, double mean, double standardDeviation);
	Gaussian (const Gaussian& gau);
	Gaussian (double mean, const Gaussian& gau);
	virtual ~Gaussian ();

	virtual DataSignal* MakeCopy (double mean) const;

	virtual RGString GetSignalType () const;

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);
	virtual double GetPullupToleranceInBP (double noise) const;
	virtual double GetPrimaryPullupDisplacementThreshold () { return 3.0 * StandardDeviation; }
	virtual double GetPrimaryPullupDisplacementThreshold (double nSigmas) { return nSigmas * StandardDeviation; }

	static double GetSigmaWidth () { return SigmaWidth; }
	static void SetSigmaWidth (double width) { SigmaWidth = width; }

	static double GetSampleSigmaWidth () { return SampleSigmaWidth; }
	static void SetSampleSigmaWidth (double width) { SampleSigmaWidth = width; }

	static int GetNumberOfIntegrationSteps () { return NumberOfSteps; }
	static void SetNumberOfIntegrationSteps (int steps) { NumberOfSteps = steps; }

	virtual void SetDisplacement (double disp);
	virtual void SetScale (double scale);
	virtual double Value (double x) const;
	virtual double Peak () const;
	virtual double GetMean () const;
	virtual double GetStandardDeviation () const;
	virtual double GetVariance () const;
	virtual void SetPeak (double peak);
	virtual void ComputeTails (double& tailLeft, double& tailRight) const;

	virtual DataSignal* Project (double left, double right) const;
	virtual DataSignal* Project (const DataSignal* target) const;

	virtual double Centroid () const;
	virtual double Centroid (double left, double right) const;
	virtual double SecondMoment (double left, double right) const;

	virtual double Norm ();
	virtual double Norm (double left, double right);
	virtual double Norm2 ();  // norm squared
	virtual double Norm2 (double left, double right);
	virtual DataSignal* Normalize (double& norm);
	virtual DataSignal* Normalize (double left, double right, double& norm);

	virtual void CalculateTheoreticalArea ();

	virtual double OneNorm ();
	virtual double OneNorm (double left, double right);
	virtual int FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean);
	virtual int SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment);

	virtual void Report (RGTextOutput& text, const RGString& indent);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	double Mean;
	double StandardDeviation;

	static int NumberOfSteps;
	static double SigmaWidth;
	static double SampleSigmaWidth;
};


class NormalizedGaussian : public Gaussian {

PERSISTENT_DECLARATION (NormalizedGaussian)

public:
	NormalizedGaussian ();
	NormalizedGaussian (double mean, double standardDeviation);
	NormalizedGaussian (const NormalizedGaussian& ng);
	NormalizedGaussian (double mean, const NormalizedGaussian& ng);
	virtual ~NormalizedGaussian ();

	virtual DataSignal* MakeCopy (double mean) const;

	virtual RGString GetSignalType () const;

	static double GetSigmaTolerance () { return SigmaTolerance; }
	static void SetSigmaTolerance (double tol) { SigmaTolerance = tol; }

	static int GetMaximumIterations () { return MaximumIterations; }
	static void SetMaximumIterations (int maxIterations) { MaximumIterations = maxIterations; }

//	virtual void SetDisplacement (double disp);
	virtual void SetScale (double scale);
//	virtual double Value (double x) const;

	virtual DataSignal* FindNextCharacteristicFromRight (const DataSignal& Signature, 
		double& fit, RGDList& previous);
	virtual DataSignal* FindNextCharacteristicFromLeft (const DataSignal& Signature, 
		double& fit, RGDList& previous);

	virtual DataSignal* FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, 
		int windowSize, double& fit, RGDList& previous) const;

	virtual DataSignal* GetResidual (const DataSignal* base, double coeff);
	virtual int Add (list<DataSignal*>& WaveList);  // list of pointers to DataSignal

//	virtual double InnerProductWithOffset (double offset, double* coeffs, const SampleDataInfo& info) const;
//	virtual double InnerProductWithStandardSpacing (double* coeffs, const SampleDataInfo& info) const;
	double InnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* coeffs, const SampleDataInfo& info) const;
	double AsymmetricInnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* coeffs, const SampleDataInfo& info) const;

	virtual double Norm ();
	virtual double Norm (double left, double right);
	virtual double Norm2 ();  // norm squared
	virtual double Norm2 (double left, double right);
	virtual DataSignal* Normalize (double& norm);
	virtual DataSignal* Normalize (double left, double right, double& norm);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	double SampleSpacing;

	static double SigmaTolerance;
	static int MaximumIterations;
};


class DoubleGaussian : public ParametricCurve {

PERSISTENT_DECLARATION (DoubleGaussian)

public:
	DoubleGaussian ();
	DoubleGaussian (double mean, double standardDeviation);
	DoubleGaussian (double mean, double standardDeviation, double sigmaRatio);
	DoubleGaussian (const DoubleGaussian& dg);
	DoubleGaussian (double mean, const DoubleGaussian& dg);
	DoubleGaussian (const DoubleGaussian& dg, CoordinateTransform* trans);
	virtual ~DoubleGaussian ();

	virtual DataSignal* MakeCopy (double mean) const;

	virtual RGString GetSignalType () const;

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);

	static void SetDefaultSigmaRatio (double ratio) { DefaultSigmaRatio = ratio; DefaultSqrtSigmaRatio = sqrt (ratio); }
	static double GetDefaultSigmaRatio () { return DefaultSigmaRatio; }
	static double GetDefaultSqrtSigmaRatio () { return DefaultSqrtSigmaRatio; }

	virtual void SetScale (double scale);
	virtual void SetScale (int curve, double scale);
	virtual void SetOrthogonalScale (int curve, double scale);
	virtual void SetPeak (double peak);
	virtual double Value (double x) const;
	virtual double Peak () const;
	virtual Boolean CanBeNegative () const;
	virtual double GetMean () const;
	virtual double GetStandardDeviation () const;
	virtual double GetVariance () const;
	virtual double GetScale (int curve) const;
	virtual double GetOrthogonalScale (int curve) const;
	virtual void ComputeTails (double& tailLeft, double& tailRight) const;
	virtual bool IsUnimodal () const;
	virtual double GetPullupToleranceInBP (double noise) const;
	virtual double GetPrimaryPullupDisplacementThreshold () { return 0.5 * GetWidth (); }
	virtual double GetPrimaryPullupDisplacementThreshold (double nSigmas) { return 0.5 * nSigmas * GetWidth (); }

	virtual DataSignal* FindNextCharacteristicFromRight (const DataSignal& Signature, 
		double& fit, RGDList& previous);
	virtual DataSignal* FindNextCharacteristicFromLeft (const DataSignal& Signature, 
		double& fit, RGDList& previous);

	virtual DataSignal* FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, 
		int windowSize, double& fit, RGDList& previous) const;

	virtual DataSignal* FindCharacteristicAsymmetric (const DataSignal* Target, const DataInterval* Segment, 
		int windowSize, double& fit, RGDList& previous) const;

	virtual DataSignal* GetResidual (const DataSignal* base, double coeff);
	virtual int Add (list<DataSignal*>& WaveList);  // list of pointers to DataSignal

//	virtual int SampleAndSave ();
//	virtual double InnerProductWithOffset (double offset, double* ogCoeffs, const SampleDataInfo& info) const;
//	virtual double InnerProductWithStandardSpacing (double* ogCoeffs, const SampleDataInfo& info) const;
	double InnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* ogCoeffs, const SampleDataInfo& info) const;
	double AsymmetricInnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* ogCoeffs, const SampleDataInfo& info) const;

	virtual double Norm ();
	virtual double Norm (double left, double right);
	virtual double Norm2 ();  // norm squared
	virtual double Norm2 (double left, double right);
	virtual DataSignal* Normalize (double& norm);
	virtual DataSignal* Normalize (double left, double right, double& norm);

	virtual double Centroid () const;
	virtual double Centroid (double left, double right) const;
	virtual double SecondMoment (double left, double right) const;

	virtual double OneNorm ();
	virtual double OneNorm (double left, double right);
	virtual int FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean);
	virtual int SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment);

	virtual double TheoreticalArea ();
	virtual void CalculateTheoreticalArea ();

	virtual void Report (RGTextOutput& text, const RGString& indent);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	double SampleSpacing;
	double PrimaryScale;
	double SecondaryScale;
	double SigmaRatio;
	double SqrtSigmaRatio;
	double OrthogonalPrimaryScale;
	double OrthogonalSecondaryScale;
	double Correlation;
	double NormOfSimpleResidual;   // norm of residual of secondary less fitted primary
	double PrimaryNorm2;   // norm^2 of primary curve
	double SecondaryNorm2;

	NormalizedGaussian* PrimaryCurve;
	NormalizedGaussian* SecondaryCurve;

	double Mean;
	double StandardDeviation;

	static double DefaultSigmaRatio;
	static double DefaultSqrtSigmaRatio;
};


class SuperGaussian : public ParametricCurve {

PERSISTENT_DECLARATION (SuperGaussian)

public:
	SuperGaussian ();
	SuperGaussian (double mean, double standardDeviation, int degree);
	SuperGaussian (double left, double right, double mean, double standardDeviation, int degree);
	SuperGaussian (const SuperGaussian& sg);
	SuperGaussian (double mean, const SuperGaussian& dg);
	virtual ~SuperGaussian ();

	virtual DataSignal* MakeCopy (double mean) const;

	virtual RGString GetSignalType () const;

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);
	virtual double GetPullupToleranceInBP (double noise) const;
	virtual double GetPrimaryPullupDisplacementThreshold () { return 0.5 * GetWidth (); }
	virtual double GetPrimaryPullupDisplacementThreshold (double nSigmas) { return 0.5 * nSigmas * GetWidth (); }

	static double GetSigmaWidth () { return SigmaWidth; }
	static void SetSigmaWidth (double width) { SigmaWidth = width; }

	static int GetNumberOfIntegrationSteps () { return NumberOfSteps; }
	static void SetNumberOfIntegrationSteps (int steps) { NumberOfSteps = steps; }

	static int GetBlobDegree () { return BlobDegree; }
	static void SetBlobDegree (int degree) { BlobDegree = degree; }

	virtual void SetDisplacement (double disp);
	virtual void SetScale (double scale);
	virtual double Value (double x) const;
	virtual double Peak () const;
	virtual double GetMean () const;
	virtual double GetStandardDeviation () const;
	virtual double GetVariance () const;
	virtual void SetPeak (double peak);
	virtual void ComputeTails (double& tailLeft, double& tailRight) const;

	virtual DataSignal* Project (double left, double right) const;
	virtual DataSignal* Project (const DataSignal* target) const;

	virtual double Centroid () const;
	virtual double Centroid (double left, double right) const;
	virtual double SecondMoment (double left, double right) const;

	virtual double Norm ();
	virtual double Norm (double left, double right);
	virtual double Norm2 ();  // norm squared
	virtual double Norm2 (double left, double right);
	virtual DataSignal* Normalize (double& norm);
	virtual DataSignal* Normalize (double left, double right, double& norm);

	virtual void CalculateTheoreticalArea ();

	virtual double OneNorm ();
	virtual double OneNorm (double left, double right);
	virtual int FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean);
	virtual int SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment);

	virtual void Report (RGTextOutput& text, const RGString& indent);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	double Mean;
	double StandardDeviation;
	int Degree;

	double SuperNorm;
	double RootTwo;
	double TwoNorm2WithoutScale;
	double Sigma2;
	double Sigma1;
	double SuperSigma2;

	static int NumberOfSteps;
	static double SigmaWidth;
	static double SuperNorms [8];
	static double SuperSigma2s [8];
	static double RootTwos [8];
	static int BlobDegree;

	double UnscaledValue (double x) const;
};


class NormalizedSuperGaussian : public SuperGaussian {

PERSISTENT_DECLARATION (NormalizedSuperGaussian)

public:
	NormalizedSuperGaussian ();
	NormalizedSuperGaussian (double mean, double standardDeviation, int degree);
	NormalizedSuperGaussian (const NormalizedSuperGaussian& nsg);
	NormalizedSuperGaussian (double mean, const NormalizedSuperGaussian& nsg);
	virtual ~NormalizedSuperGaussian ();

	virtual DataSignal* MakeCopy (double mean) const;

	virtual RGString GetSignalType () const;

//	virtual void SetDisplacement (double disp);
	virtual void SetScale (double scale);
//	virtual double Value (double x) const;

	virtual DataSignal* FindNextCharacteristicFromRight (const DataSignal& Signature, 
		double& fit, RGDList& previous);
	virtual DataSignal* FindNextCharacteristicFromLeft (const DataSignal& Signature, 
		double& fit, RGDList& previous);

	virtual DataSignal* FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, 
		int windowSize, double& fit, RGDList& previous) const;

	virtual DataSignal* GetResidual (const DataSignal* base, double coeff);
	virtual int Add (list<DataSignal*>& WaveList);  // list of pointers to DataSignal

//	virtual int SampleAndSave ();
//	virtual double InnerProductWithOffset (double offset, double* coeffs, const SampleDataInfo& info) const;
//	virtual double InnerProductWithStandardSpacing (double* coeffs, const SampleDataInfo& info) const;
	double InnerProductWithOffsetAndStandardDeviation (double offset, double sigma, double* coeffs, const SampleDataInfo& info) const;

	virtual double Norm ();
	virtual double Norm (double left, double right);
	virtual double Norm2 ();  // norm squared
	virtual double Norm2 (double left, double right);
	virtual DataSignal* Normalize (double& norm);
	virtual DataSignal* Normalize (double left, double right, double& norm);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	double SampleSpacing;

	double nthRoot (double x) const;
	double UnscaledValueWithOffsetAndSigma (double x, double offset, double sigma) const;
};



class CompositeCurve : public ParametricCurve {

PERSISTENT_DECLARATION (CompositeCurve)

public:
	CompositeCurve ();
	CompositeCurve (double left, double right, const RGDList& Curves);
	CompositeCurve (const CompositeCurve& cc);
	virtual ~CompositeCurve ();

//	virtual void SetDisplacement (double disp);
//	virtual void SetScale (double scale);


	virtual double Value (double x) const;
	virtual RGString GetSignalType () const;
	virtual bool IsUnimodal () const { return false; }

//	virtual DataSignal* Project (double left, double right) const;
//	virtual DataSignal* Project (const DataSignal* target) const;

//	virtual DataSignal* FindNextCharacteristicFromRight (const DataSignal& Signature, 
//		double& fit, RGDList& previous);
//	virtual DataSignal* FindNextCharacteristicFromLeft (const DataSignal& Signature, 
//		double& fit, RGDList& previous);

//	virtual double Centroid () const;
//	virtual double Centroid (double left, double right) const;
//	virtual double SecondMoment (double left, double right) const;

//	virtual void ExtendDomain (double left, double right);
//	virtual void ExtendDomain (const DataSignal* model);

//	virtual DataSignal* GetResidual (const DataSignal* base, double coeff);
//	virtual int Add (list<DataSignal*>& WaveList);  // list of pointers to DataSignal
	virtual DataSignal* Digitize (int numSamples, double left, double resolution);
	virtual DataSignal* Digitize (int numSamples, double left);
	virtual DataSignal* BuildSample (int numSamples, double left, double resolution);
	virtual int SampleAndSave ();
	virtual void ProjectNeighboringSignals (double horizontalResolution, double verticalTolerance);

	virtual double Norm ();
	virtual double Norm (double left, double right);
	virtual double Norm2 ();  // norm squared
	virtual double Norm2 (double left, double right);
	virtual double OneNorm ();
	virtual double OneNorm (double left, double right);
//	virtual DataSignal* Normalize (double& norm);
//	virtual DataSignal* Normalize (double left, double right, double& norm);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	RGDList CurveList;
	RGDListIterator* CurveIterator;
	SampledData* SampledVersion;
	RGDList mTempCurveList;
};



class DualDoubleGaussian : public ParametricCurve {

PERSISTENT_DECLARATION (DualDoubleGaussian)

public:
	DualDoubleGaussian ();
	DualDoubleGaussian (const DualDoubleGaussian& dg);
	DualDoubleGaussian (double mean, const DualDoubleGaussian& dg);
	virtual ~DualDoubleGaussian ();

	virtual DataSignal* MakeCopy (double mean) const;

	virtual double Peak () const { return mPeak; }
	virtual double GetMean () const { return mMean; }
	virtual double GetStandardDeviation () const { return mSigma; }

	virtual RGString GetSignalType () const;
	virtual bool IsUnimodal () const { return false; }

	virtual double Value (double x) const;
	virtual double Norm ();
	virtual double Norm (double left, double right);
	virtual double Norm2 ();  // norm squared
	virtual double Norm2 (double left, double right);

	virtual DataSignal* FindCharacteristic (const DataSignal* Target, const DataInterval* Segment, 
		int windowSize, double& fit, RGDList& previous) const;

	void SetFirstCurve (DoubleGaussian* dg) { PrimaryCurve = dg; }
	void SetSecondCurve (DoubleGaussian* dg) { SecondaryCurve = dg; }
	void SetInfo ();
	DataSignal* GetFirstCurve ();
	DataSignal* GetSecondCurve ();

protected:
	DoubleGaussian* PrimaryCurve;
	DoubleGaussian* SecondaryCurve;
	double mMean;
	double mSigma;
	double mPeak;
};


class CraterSignal : public ParametricCurve {

PERSISTENT_DECLARATION (CraterSignal)

public:
	CraterSignal ();
	CraterSignal (DataSignal* prev, DataSignal* next, bool assignByProportion = false);
	CraterSignal (DataSignal* prev, DataSignal* next, DataSignal* primaryLink, bool assignByProportion = false);
	CraterSignal (const CraterSignal& c);
	CraterSignal (double mean, const CraterSignal& c);
	CraterSignal (const CraterSignal& c, CoordinateTransform* trans);
	virtual ~CraterSignal ();

	virtual DataSignal* MakeCopy (double mean) const;

	virtual DataSignal* GetPreviousLinkedSignal () { return mPrevious; }
	virtual DataSignal* GetNextLinkedSignal () { return mNext; }
	virtual double GetPullupToleranceInBP () const { return 0.0425; }
	virtual double GetPullupToleranceInBP (double noise) const;
	virtual double GetPrimaryPullupDisplacementThreshold ();
	virtual double GetPrimaryPullupDisplacementThreshold (double nSigmas);

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);

	virtual RGString GetSignalType () const;
	virtual bool IsUnimodal () const { return false; }
	virtual bool IsCraterPeak () const { return true; }
	virtual double TroughHeight () const { return mTroughHeight; }
	virtual void CalculateTheoreticalArea ();

	virtual bool LiesBelowHeightAt (double x, double height);
	virtual bool TestForIntersectionWithPrimary (DataSignal* primary);

	virtual int AddNoticeToList (Notice* newNotice);

	virtual void CaptureSmartMessages ();

	virtual bool TestForMultipleSignals (DataSignal*& prev, DataSignal*& next);
	virtual bool TestForMultipleSignals (DataSignal*& prev, DataSignal*& next, int location);
	virtual bool TestForMultipleSignalsWithinLocus (DataSignal*& prev, DataSignal*& next, int location, bool isAmel, double adenylationLimit);

	virtual bool TestSignalGroupsWithinILS (double ilsLeft, double ilsRight, double minBioID);

	virtual void SetAlleleInformation (int position);

	virtual double Value (double x) const;
	virtual double Value (int n) const;
	virtual double Norm (double a, double b);
	virtual double Norm ();
	virtual double Norm2 (double a, double b);
	virtual double Norm2 ();
	virtual double Centroid () const;

	virtual double Peak () const;
	virtual double GetMean () const;
	virtual double GetStandardDeviation () const;
	virtual double GetWidth ();
	virtual double GetVariance () const;

	virtual void SetMessageValue (int scope, int location, bool value);
	virtual void SetMessageValue (const SmartNotice& notice, bool value);
//testmsg	virtual void SetMessageValue (const SmartNotice& notice, bool value, bool useVirtualMethod);
	virtual void SetMessageValue (int scope, int location, bool value, bool useVirtualMethod);

	virtual void Report (RGTextOutput& text, const RGString& indent);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	double mMean;
	double mSigma;
	double mHeight;
	double mTroughHeight;
	DataSignal* mPrevious;
	DataSignal* mNext;
};


class SimpleSigmoidSignal : public CraterSignal {

PERSISTENT_DECLARATION (SimpleSigmoidSignal)

public:
	SimpleSigmoidSignal ();
	SimpleSigmoidSignal (DataSignal* prev, DataSignal* next);
	SimpleSigmoidSignal (DataSignal* prev, DataSignal* next, DataSignal* primaryLink);
	SimpleSigmoidSignal (const SimpleSigmoidSignal& c);
	SimpleSigmoidSignal (double mean, const SimpleSigmoidSignal& c);
	SimpleSigmoidSignal (const SimpleSigmoidSignal& c, CoordinateTransform* trans);
	virtual ~SimpleSigmoidSignal ();

	virtual DataSignal* MakeCopy (double mean) const;
	virtual void RecalculatePullupTolerance ();
	virtual double GetPullupToleranceInBP (double noise) const;
	virtual double GetPrimaryPullupDisplacementThreshold () { return 0.0; }
	virtual double GetPrimaryPullupDisplacementThreshold (double nSigmas) { return 0.0; }  // should never be called
	virtual bool IsSigmoidalPeak () const { return true; }
	virtual double TroughHeight () const { return Peak (); }
	virtual double GetWidth ();
	virtual bool LiesBelowHeightAt (double x, double height);
	virtual bool TestForIntersectionWithPrimary (DataSignal* primary);

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);

	virtual RGString GetSignalType () const;

	virtual int AddNoticeToList (Notice* newNotice);

	virtual void CaptureSmartMessages ();

	virtual bool TestForMultipleSignals (DataSignal*& prev, DataSignal*& next);
	virtual bool TestForMultipleSignals (DataSignal*& prev, DataSignal*& next, int location);
	virtual bool TestForMultipleSignalsWithinLocus (DataSignal*& prev, DataSignal*& next, int location, bool isAmel, double adenylationLimit);

	virtual bool TestSignalGroupsWithinILS (double ilsLeft, double ilsRight, double minBioID);

	virtual double Value (double x) const;
	virtual double Value (int n) const;
	virtual double Norm (double a, double b);
	virtual double Norm ();
	virtual double Norm2 (double a, double b);
	virtual double Norm2 ();
	//virtual double Centroid () const;

	//virtual double Peak () const;
	//virtual double GetMean () const;
	//virtual double GetStandardDeviation () const;
	//virtual double GetVariance () const;

	virtual void SetMessageValue (int scope, int location, bool value);
	virtual void SetMessageValue (const SmartNotice& notice, bool value);
	virtual void SetMessageValue (int scope, int location, bool value, bool useVirtualMethod);

	//virtual void Report (RGTextOutput& text, const RGString& indent);

	//virtual size_t StoreSize () const;

	//virtual void RestoreAll (RGFile&);
	//virtual void RestoreAll (RGVInStream&);
	//virtual void SaveAll (RGFile&) const;
	//virtual void SaveAll (RGVOutStream&) const;

protected:

};


class NoisyPeak : public CraterSignal {

PERSISTENT_DECLARATION (NoisyPeak)

public:
	NoisyPeak ();
	NoisyPeak (DataSignal* prev, DataSignal* next, bool assignByProportion = false);
	NoisyPeak (DataSignal* prev, DataSignal* next, DataSignal* primaryLink, bool assignByProportion = false);
	NoisyPeak (const NoisyPeak& c);
	NoisyPeak (double mean, const NoisyPeak& c);
	NoisyPeak (const NoisyPeak& c, CoordinateTransform* trans);
	virtual ~NoisyPeak ();

	virtual DataSignal* MakeCopy (double mean) const;

	virtual double GetPullupToleranceInBP () const { return 0.0425; }
	virtual double GetPullupToleranceInBP (double noise) const;
	virtual double GetPrimaryPullupDisplacementThreshold ();
	virtual double GetPrimaryPullupDisplacementThreshold (double nSigmas);
	virtual double TroughHeight () const { return Peak (); }
	virtual double GetWidth ();

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);

	virtual RGString GetSignalType () const;
	virtual bool IsUnimodal () const { return false; }
	virtual bool IsCraterPeak () const { return false; }

	//virtual int AddNoticeToList (Notice* newNotice);

	virtual void CaptureSmartMessages ();

	virtual bool TestForMultipleSignals (DataSignal*& prev, DataSignal*& next);
	virtual bool TestForMultipleSignals (DataSignal*& prev, DataSignal*& next, int location);
	virtual bool TestForMultipleSignalsWithinLocus (DataSignal*& prev, DataSignal*& next, int location, bool isAmel, double adenylationLimit);

	virtual bool TestSignalGroupsWithinILS (double ilsLeft, double ilsRight, double minBioID);

	virtual void SetAlleleInformation (int position);

	//virtual double Value (double x) const;
	//virtual double Value (int n) const;
	//virtual double Norm (double a, double b);
	//virtual double Norm ();
	//virtual double Norm2 (double a, double b);
	//virtual double Norm2 ();
	//virtual double Centroid () const;

	//virtual double Peak () const;
	//virtual double GetMean () const;
	//virtual double GetStandardDeviation () const;
	//virtual double GetVariance () const;

	//virtual void SetMessageValue (int scope, int location, bool value);
	//virtual void SetMessageValue (const SmartNotice& notice, bool value);

	//virtual void SetMessageValue (int scope, int location, bool value, bool useVirtualMethod);

	//virtual void Report (RGTextOutput& text, const RGString& indent);

	//virtual size_t StoreSize () const;

	//virtual void RestoreAll (RGFile&);
	//virtual void RestoreAll (RGVInStream&);
	//virtual void SaveAll (RGFile&) const;
	//virtual void SaveAll (RGVOutStream&) const;

protected:
	double mL1;
	double mL2;
};


class NegativeSignal : public ParametricCurve {

PERSISTENT_DECLARATION (NegativeSignal)

public:
	NegativeSignal ();
	NegativeSignal (const NegativeSignal& neg);
	NegativeSignal (double mean, double peak, const ParametricCurve& sig);
	virtual ~NegativeSignal ();

	virtual double Peak () const { return -mOriginal->Peak (); }
	virtual double GetMean () const { return mOriginal->GetMean (); }

	virtual double Value (double t) { return -mOriginal->Value (t); }
	virtual double Norm (double left, double right) { return mOriginal->Norm (left, right); }
	virtual double Norm2 (double left, double right) { return mOriginal->Norm2 (left, right); }
	virtual double Norm2 () { return mOriginal->Norm2 (); }
	virtual double Value (double t) const { return mOriginal->Value (t); }

	virtual void SetDisplacement (double disp) { mOriginal->SetDisplacement (disp); }
	virtual void SetScale (double scale) { mOriginal->SetScale (scale); }
	virtual double GetStandardDeviation () const { return mOriginal->GetStandardDeviation (); }
	virtual double GetWidth () { return mOriginal->GetWidth (); }
	virtual double GetVariance () const { return mOriginal->GetVariance (); }
	virtual void SetPeak (double peak) { mOriginal->SetPeak (peak); }
	virtual void ComputeTails (double& tailLeft, double& tailRight) const { mOriginal->ComputeTails (tailLeft, tailRight); }

	virtual DataSignal* Project (double left, double right) const { return mOriginal->Project (left, right); }
	virtual DataSignal* Project (const DataSignal* target) const  { return mOriginal->Project (target); }

	virtual double Centroid () const { return mOriginal->Centroid (); }
	virtual double Centroid (double left, double right) const { return mOriginal->Centroid (left, right); }
	virtual double SecondMoment (double left, double right) const { return mOriginal->SecondMoment (left, right); }

	virtual double Norm () { return mOriginal->Norm (); }
	virtual DataSignal* Normalize (double& norm) { return mOriginal->Normalize (norm); }
	virtual DataSignal* Normalize (double left, double right, double& norm) { return mOriginal->Normalize (left, right, norm); }

	virtual void CalculateTheoreticalArea () { mOriginal->CalculateTheoreticalArea (); }

	virtual double OneNorm () { return mOriginal->OneNorm (); }
	virtual double OneNorm (double left, double right) { return mOriginal->OneNorm (left, right); }
	virtual int FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean) { return mOriginal->FirstMomentForOneNorm (left, right, oneNorm, mean); }
	virtual int SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment) { return mOriginal->SecondMomentForOneNorm (left, right, oneNorm, mean, secondMoment); }

protected:

	DataSignal* mOriginal;
};


class SpikeSignal : public ParametricCurve {

PERSISTENT_DECLARATION (SpikeSignal)

public:
	SpikeSignal ();
	SpikeSignal (double mean, double peak, double leftValue, double rightValue);
	SpikeSignal (const SpikeSignal& spike);
	SpikeSignal (double mean, const SpikeSignal& spike);
	virtual ~SpikeSignal ();

	virtual DataSignal* MakeCopy (double mean) const;

	virtual RGString GetSignalType () const;

	virtual double GetWidth ();

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);
	virtual double GetPullupToleranceInBP (double noise) const;
	virtual double GetPrimaryPullupDisplacementThreshold () { return 1.0; }
	virtual double GetPrimaryPullupDisplacementThreshold (double nSigmas) { return 1.0; }

	virtual void SetDisplacement (double disp);
	virtual double Value (double x) const;
	virtual double Value (int x) const;
	virtual double Peak () const;
	virtual double GetMean () const;
	virtual double GetStandardDeviation () const;
	virtual double GetVariance () const;
	virtual void SetPeak (double peak);
	virtual void ComputeTails (double& tailLeft, double& tailRight) const;

	//virtual DataSignal* Project (double left, double right) const;
	//virtual DataSignal* Project (const DataSignal* target) const;

	virtual double Centroid () const;
//	virtual double Centroid (double left, double right) const;
//	virtual double SecondMoment (double left, double right) const;

	virtual double Norm ();
	virtual double Norm (double left, double right);
	virtual double Norm2 ();  // norm squared
	virtual double Norm2 (double left, double right);
	virtual DataSignal* Normalize (double& norm);
	virtual DataSignal* Normalize (double left, double right, double& norm);

	virtual void CalculateTheoreticalArea ();

	virtual double OneNorm ();
	virtual double OneNorm (double left, double right);
	virtual int FirstMomentForOneNorm (double left, double right, double& oneNorm, double& mean);
	virtual int SecondMomentForOneNorm (double left, double right, double& oneNorm, double& mean, double& secondMoment);

	virtual void Report (RGTextOutput& text, const RGString& indent);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	double mMean;
	double mPeak;
};


#endif  /*  _DATASIGNAL_H_  */
