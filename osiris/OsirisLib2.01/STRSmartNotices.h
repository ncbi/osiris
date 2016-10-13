//
// These are all subclasses of class SmartNotice, used for STR Osiris.  Each one corresponds
// to a particular issue or situation
//

#ifndef _STRSMARTNOTICES_H_
#define _STRSMARTNOTICES_H_

#include "SmartNotice.h"
#include "rgpersist.h"
#include "rgstring.h"


PERSISTENT_PREDECLARATION (smILSFailed)
PERSISTENT_PREDECLARATION (smLocusIsAMEL)
PERSISTENT_PREDECLARATION (smSampleIsLadder)
PERSISTENT_PREDECLARATION (smSampleIsPosCtrl)
PERSISTENT_PREDECLARATION (smSampleIsNegCtrl)
PERSISTENT_PREDECLARATION (smILSSpacingFailed)
PERSISTENT_PREDECLARATION (smNoLadderFile)
PERSISTENT_PREDECLARATION (smLadderTypeUnknown)
PERSISTENT_PREDECLARATION (smReagentKitUnknown)
PERSISTENT_PREDECLARATION (smMoreThanOneReagentKit)
PERSISTENT_PREDECLARATION (smCurveFitFailed)
PERSISTENT_PREDECLARATION (smCurveFitMarginal)
PERSISTENT_PREDECLARATION (smSpike)
PERSISTENT_PREDECLARATION (smStutter)
PERSISTENT_PREDECLARATION (smAdenylation)
PERSISTENT_PREDECLARATION (smBlob)
PERSISTENT_PREDECLARATION (smPullUp)
PERSISTENT_PREDECLARATION (smILSTooFewPeaks)
PERSISTENT_PREDECLARATION (smPeakOutsideILS)
PERSISTENT_PREDECLARATION (smMarkerSetNameUnknown)
PERSISTENT_PREDECLARATION (smNamedILSUnknown)
PERSISTENT_PREDECLARATION (smBelowMinRFU)
PERSISTENT_PREDECLARATION (smUnexpectedILSPeak)
PERSISTENT_PREDECLARATION (smNoPosCtrlFound)
PERSISTENT_PREDECLARATION (smNoNegCtrlFound)
PERSISTENT_PREDECLARATION (smNamedPosCtrlNotFound)
PERSISTENT_PREDECLARATION (smPeakHeightAboveMax)
PERSISTENT_PREDECLARATION (smCannotSeparateILSPrimerPeaks)
PERSISTENT_PREDECLARATION (smILSNeighborFilterTooAggressive)
PERSISTENT_PREDECLARATION (smILSFractionFilterTooAggressive)
PERSISTENT_PREDECLARATION (smILSRelativeHeightsInconsistent)
PERSISTENT_PREDECLARATION (smOsirisBelowMinRFUButRawDataAbove)
PERSISTENT_PREDECLARATION (smRawDataBelowMinRFUButOsirisAbove)
PERSISTENT_PREDECLARATION (smOsirisAboveMaxRFUButRawDataBelow)
PERSISTENT_PREDECLARATION (smRawDataAboveMaxRFUButOsirisBelow)
PERSISTENT_PREDECLARATION (smStage1Successful)
PERSISTENT_PREDECLARATION (smSignalIsCtrlPeak)
PERSISTENT_PREDECLARATION (smLocusTooFewPeaks)
PERSISTENT_PREDECLARATION (smInterlocusLadderPeak)
PERSISTENT_PREDECLARATION (smUnexpectedLadderLocusPeak)
PERSISTENT_PREDECLARATION (smPoorPeakMorphology)
PERSISTENT_PREDECLARATION (smPoorLocusMorphology)
PERSISTENT_PREDECLARATION (smLocusMayHaveUnreportedOLs)
PERSISTENT_PREDECLARATION (smNPeaksForLocusUnavailable)
PERSISTENT_PREDECLARATION (smRelativeHeightImbalance)
PERSISTENT_PREDECLARATION (smHeightBelowFractionalFilter)
PERSISTENT_PREDECLARATION (smPartOfExtendedLocusLeft)
PERSISTENT_PREDECLARATION (smPartOfExtendedLocusRight)
PERSISTENT_PREDECLARATION (smBelowFractionalFilterLeft)
PERSISTENT_PREDECLARATION (smBelowFractionalFilterRight)
PERSISTENT_PREDECLARATION (smStage2Successful)
PERSISTENT_PREDECLARATION (smPullUpAtControlPeak)
PERSISTENT_PREDECLARATION (smInterlocusPullUpWithPoorFit)
PERSISTENT_PREDECLARATION (smPossibleValidInterlocusLeft)
PERSISTENT_PREDECLARATION (smPossibleValidInterlocusRight)
PERSISTENT_PREDECLARATION (smUnassignedInterlocusPeak)
PERSISTENT_PREDECLARATION (smAmbiguousInterlocus)
PERSISTENT_PREDECLARATION (smTooLowInterlocusPeak)
PERSISTENT_PREDECLARATION (smPossibleInterlocusArtifactLeft)
PERSISTENT_PREDECLARATION (smPossibleInterlocusArtifactRight)
PERSISTENT_PREDECLARATION (smFirstAlleleEqFirstLadderPeak)
PERSISTENT_PREDECLARATION (smSignalOL)
PERSISTENT_PREDECLARATION (smAMELMisaligned)
PERSISTENT_PREDECLARATION (smCrater)
PERSISTENT_PREDECLARATION (smAcceptedOLLeft)
PERSISTENT_PREDECLARATION (smAcceptedOLRight)
PERSISTENT_PREDECLARATION (smStage3Successful)
PERSISTENT_PREDECLARATION (smValidOLInterlocusLeft)
PERSISTENT_PREDECLARATION (smValidOLInterlocusRight)
PERSISTENT_PREDECLARATION (smAcceptedInterlocusLeft)
PERSISTENT_PREDECLARATION (smAcceptedInterlocusRight)
PERSISTENT_PREDECLARATION (smNoXForAMEL)
PERSISTENT_PREDECLARATION (smSignalIsAllele)
PERSISTENT_PREDECLARATION (smPosCtrlLocusMismatch)
PERSISTENT_PREDECLARATION (smNoGenotypeFound)
PERSISTENT_PREDECLARATION (smThreeOrMoreAlleles)
PERSISTENT_PREDECLARATION (smTwoOrMoreAlleles)
PERSISTENT_PREDECLARATION (smHeterozygousImbalance)
PERSISTENT_PREDECLARATION (smOLAlleles)
PERSISTENT_PREDECLARATION (smHomozygoteHeightProblem)
PERSISTENT_PREDECLARATION (smPullUpAtAllele)
PERSISTENT_PREDECLARATION (smPosCtrlLocusNotFound)
PERSISTENT_PREDECLARATION (smBPResidualTooLarge)
PERSISTENT_PREDECLARATION (smExtraLadderAllele)
PERSISTENT_PREDECLARATION (smCriticalMessagesAtILS)
PERSISTENT_PREDECLARATION (smAMELHasExtraneousPeaks)
PERSISTENT_PREDECLARATION (smExtraneousAMELPeak)
PERSISTENT_PREDECLARATION (smNegCtrlChannelHasNoPrimerPeaks)
PERSISTENT_PREDECLARATION (smMinimumHeightForPrimerPeaks)
PERSISTENT_PREDECLARATION (smInterlocusPeaksToRight)
PERSISTENT_PREDECLARATION (smInterlocusPeaksToLeft)
PERSISTENT_PREDECLARATION (smLadderFailed)
PERSISTENT_PREDECLARATION (smSampleUnanalyzable)
PERSISTENT_PREDECLARATION (smCriticalMessagesAtAlleles)
PERSISTENT_PREDECLARATION (smHeightBelowPullupFractionalFilter)
PERSISTENT_PREDECLARATION (smBelowPullupFractionalFilterLeft)
PERSISTENT_PREDECLARATION (smBelowPullupFractionalFilterRight)
PERSISTENT_PREDECLARATION (smPossibleExtraLocusAllele)
PERSISTENT_PREDECLARATION (smUnexpectedPeakInNegativeControl)
PERSISTENT_PREDECLARATION (smLocusContainsPeaksWithExcessiveResiduals)
PERSISTENT_PREDECLARATION (smPrimaryInterchannelLink)
PERSISTENT_PREDECLARATION (smNotInterchannelLink)
PERSISTENT_PREDECLARATION (smHeightBelowInterlocusMinRFU)
PERSISTENT_PREDECLARATION (smFSAChannelUnreadable)
PERSISTENT_PREDECLARATION (smAmbiguousInterlocusCrater)
PERSISTENT_PREDECLARATION (smPossibleAmbiguousInterlocusAssignedToLocus)
PERSISTENT_PREDECLARATION (smWouldCauseHeterozygousImbalanceLeft)
PERSISTENT_PREDECLARATION (smWouldCauseHeterozygousImbalanceRight)
PERSISTENT_PREDECLARATION (smSignalOffGridLeft)
PERSISTENT_PREDECLARATION (smSignalOffGridRight)
PERSISTENT_PREDECLARATION (sm0UnambiguousPeaksLeft)
PERSISTENT_PREDECLARATION (sm0UnambiguousPeaksRight)
PERSISTENT_PREDECLARATION (sm1UnambiguousPeakLeft)
PERSISTENT_PREDECLARATION (sm1UnambiguousPeakRight)
PERSISTENT_PREDECLARATION (sm2PlusUnambiguousPeaksLeft)
PERSISTENT_PREDECLARATION (sm2PlusUnambiguousPeaksRight)
PERSISTENT_PREDECLARATION (sm0AmbiguousPeaksLeft)
PERSISTENT_PREDECLARATION (sm0AmbiguousPeaksRight)
PERSISTENT_PREDECLARATION (sm1AmbiguousPeakLeft)
PERSISTENT_PREDECLARATION (sm1AmbiguousPeakRight)
PERSISTENT_PREDECLARATION (sm2PlusAmbiguousPeaksLeft)
PERSISTENT_PREDECLARATION (sm2PlusAmbiguousPeaksRight)
PERSISTENT_PREDECLARATION (smCouldBelongToLocusLeft)
PERSISTENT_PREDECLARATION (smCouldBelongToLocusRight)
PERSISTENT_PREDECLARATION (smChannelIsNegControl)
PERSISTENT_PREDECLARATION (smLocusIsNegControl)
PERSISTENT_PREDECLARATION (smChannelIsPosControl)
PERSISTENT_PREDECLARATION (smLocusIsPosControl)
PERSISTENT_PREDECLARATION (smPurePullup)
PERSISTENT_PREDECLARATION (smChannelPeaksHaveCriticalArtifacts)
PERSISTENT_PREDECLARATION (smPreliminaryLocusHas2OrMoreAlleles)
PERSISTENT_PREDECLARATION (smCraterSidePeak)
PERSISTENT_PREDECLARATION (smSignalNotACrater)
PERSISTENT_PREDECLARATION (smSignalNotACraterSidePeak)
PERSISTENT_PREDECLARATION (smLocusIsHomozygous)
PERSISTENT_PREDECLARATION (smLocusMissingGenotype)
PERSISTENT_PREDECLARATION (smStage4Successful)
PERSISTENT_PREDECLARATION (smStage5Successful)
PERSISTENT_PREDECLARATION (smPeakInCoreLadderLocus)
PERSISTENT_PREDECLARATION (smPeakAboveOverloadLimit)
PERSISTENT_PREDECLARATION (smRestrictedPriorityPeak)
PERSISTENT_PREDECLARATION (smChannelIsILS)
PERSISTENT_PREDECLARATION (smILSSpacingIsMarginal)
PERSISTENT_PREDECLARATION (smPoorPeakMorphologyOrResolution)
PERSISTENT_PREDECLARATION (smPeakToRightOfILS)
PERSISTENT_PREDECLARATION (smPreliminaryLocusHasNoAlleles)
PERSISTENT_PREDECLARATION (smLocusHasPeaksBetweenDetectionAndAnalysisThresholds)
PERSISTENT_PREDECLARATION (smUnexpectedNoOfGridPeaks)
PERSISTENT_PREDECLARATION (smMoreThanThreeAlleles)
PERSISTENT_PREDECLARATION (smDefaultsAreOverridden)
PERSISTENT_PREDECLARATION (smAssociatedLadderIsCritical)
PERSISTENT_PREDECLARATION (smAssociatedLadderLocusIsCritical)
PERSISTENT_PREDECLARATION (smChannelHasRaisedBaseline)
PERSISTENT_PREDECLARATION (smChannelHasExcessiveNoiseAboveAnalysisThreshold)
PERSISTENT_PREDECLARATION (smChannelHasExcessiveNoiseAboveDetectionThreshold)
PERSISTENT_PREDECLARATION (smSampleChannelRaisedBaselineThreshold)
PERSISTENT_PREDECLARATION (smSampleILSChannelRaisedBaselineThreshold)
PERSISTENT_PREDECLARATION (smILSPeakMayBeIncorrect)
PERSISTENT_PREDECLARATION (smTestForColorCorrectionMatrixPreset)
PERSISTENT_PREDECLARATION (smColorCorrectionMatrixExpectedButNotFound)
PERSISTENT_PREDECLARATION (smColorCorrectionMatrixWrongSize)
PERSISTENT_PREDECLARATION (smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing)
PERSISTENT_PREDECLARATION (smMinimumNumberOfPrimerPeaksForNegativeControlChannel)
PERSISTENT_PREDECLARATION (smUseSampleNamesForControlSampleTestsPreset)
PERSISTENT_PREDECLARATION (smNoYForAMEL)
PERSISTENT_PREDECLARATION (smLocusIsYLinked)
PERSISTENT_PREDECLARATION (smNumberAllelesBelowExpectation)
PERSISTENT_PREDECLARATION (smBaselineRelativeStutter)
PERSISTENT_PREDECLARATION (smBaselineRelativeAdenylation)
PERSISTENT_PREDECLARATION (smBaselineRelativeBelowMinRFU)
PERSISTENT_PREDECLARATION (smBaselineRelativeBelowMinInterlocusRFU)
PERSISTENT_PREDECLARATION (smTestRelativeBaselinePreset)
PERSISTENT_PREDECLARATION (smIgnoreNegativeRelativeBaselinePreset)
PERSISTENT_PREDECLARATION (smNormalizeRawDataRelativeToBaselinePreset)
PERSISTENT_PREDECLARATION (smPercentOfStandardNoiseThreshold)
PERSISTENT_PREDECLARATION (smLadderFitThreshold)
PERSISTENT_PREDECLARATION (smSampleToLadderFitBelowExpectations)
PERSISTENT_PREDECLARATION (smEnableRawDataFilterForNormalizationPreset)
PERSISTENT_PREDECLARATION (smBaselineEstimationThreshold)
PERSISTENT_PREDECLARATION (smSinglePassFilterWindowWidth)
PERSISTENT_PREDECLARATION (smAmbiguousInterlocusWithPoorMorphology)
PERSISTENT_PREDECLARATION (smSigmoidalPullup)
PERSISTENT_PREDECLARATION (smMinPrimaryPullupThreshold)
PERSISTENT_PREDECLARATION (smExcessiveResidualDisplacement)
PERSISTENT_PREDECLARATION (smResidualDisplacementThreshold)
PERSISTENT_PREDECLARATION (smResidualDisplacementTestPreset)
PERSISTENT_PREDECLARATION (smExcessiveResidualDisplacementLeft)
PERSISTENT_PREDECLARATION (smExcessiveResidualDisplacementRight)
PERSISTENT_PREDECLARATION (smSampleSatisfiesPossibleMixtureIDCriteria)
PERSISTENT_PREDECLARATION (smDisableLowLevelFiltersForKnownMixturesPreset)
PERSISTENT_PREDECLARATION (smDisableStutterFilter)
PERSISTENT_PREDECLARATION (smDisableAdenylationFilter)
PERSISTENT_PREDECLARATION (smSelectTriplePassVsSinglePassFilterPreset)
PERSISTENT_PREDECLARATION (smTriplePassFilterWindowWidth)
PERSISTENT_PREDECLARATION (smLadderFitThresholdUsingMinError)
PERSISTENT_PREDECLARATION (smUseMaxSecondDerivativesForSampleToLadderFit)
PERSISTENT_PREDECLARATION (smPartOfDualSignal)
PERSISTENT_PREDECLARATION (smNegativePeak)
PERSISTENT_PREDECLARATION (smCallOnLadderAdenylationPreset)
PERSISTENT_PREDECLARATION (smSignalIsIntegralMultipleOfRepeatLeft)
PERSISTENT_PREDECLARATION (smSignalIsIntegralMultipleOfRepeatRight)
PERSISTENT_PREDECLARATION (smILSFilterLeftShoulderPeaksPreset)
PERSISTENT_PREDECLARATION (smILSShoulderPeakProximityThreshold)
PERSISTENT_PREDECLARATION (smILSShoulderFilterPercentThreshold)
PERSISTENT_PREDECLARATION (smILSShoulderPeak)
PERSISTENT_PREDECLARATION (smLadderPeakOutsideCoreLadder)
PERSISTENT_PREDECLARATION (smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset)
PERSISTENT_PREDECLARATION (smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset)
PERSISTENT_PREDECLARATION (smExtendLociEdgeToEdgePreset)
PERSISTENT_PREDECLARATION (smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset)
PERSISTENT_PREDECLARATION (smMaxILSBPForExtendedLocus)
PERSISTENT_PREDECLARATION (smIsQualityLocus)
PERSISTENT_PREDECLARATION (smScaleILSPrimerSearchPreset)
PERSISTENT_PREDECLARATION (smNumberOfLastILSPeaksInScale)
PERSISTENT_PREDECLARATION (smPercentLastILSPeaksInScaling)
PERSISTENT_PREDECLARATION (smLaserOffScale)
PERSISTENT_PREDECLARATION (smCalculatedPurePullup)
PERSISTENT_PREDECLARATION (smPartialPullupBelowMinRFU)
PERSISTENT_PREDECLARATION (smCallStutterPeaksPreset)
PERSISTENT_PREDECLARATION (smDoNotCallStutterPeaksForSingleSourceSamplesPreset)
PERSISTENT_PREDECLARATION (smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset)
PERSISTENT_PREDECLARATION (smDoNotCallAdenylationPeaksForSingleSourceSamplesPreset)
PERSISTENT_PREDECLARATION (smPlusStutter)
PERSISTENT_PREDECLARATION (smNonStandardStutter)
PERSISTENT_PREDECLARATION (smPlusNonStandardStutter)



class smILSFailed : public SmartNotice {

PERSISTENT_DECLARATION (smILSFailed)

public:
	smILSFailed ();
	smILSFailed (const smILSFailed& note);
	virtual ~smILSFailed ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusIsAMEL : public SmartNotice {

PERSISTENT_DECLARATION (smLocusIsAMEL)

public:
	smLocusIsAMEL ();
	smLocusIsAMEL (const smLocusIsAMEL& note);
	virtual ~smLocusIsAMEL ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSampleIsLadder : public SmartNotice {

PERSISTENT_DECLARATION (smSampleIsLadder)

public:
	smSampleIsLadder ();
	smSampleIsLadder (const smSampleIsLadder& note);
	virtual ~smSampleIsLadder ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSampleIsPosCtrl : public SmartNotice {

PERSISTENT_DECLARATION (smSampleIsPosCtrl)

public:
	smSampleIsPosCtrl ();
	smSampleIsPosCtrl (const smSampleIsPosCtrl& note);
	virtual ~smSampleIsPosCtrl ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSampleIsNegCtrl : public SmartNotice {

PERSISTENT_DECLARATION (smSampleIsNegCtrl)

public:
	smSampleIsNegCtrl ();
	smSampleIsNegCtrl (const smSampleIsNegCtrl& note);
	virtual ~smSampleIsNegCtrl ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSSpacingFailed : public SmartNotice {

PERSISTENT_DECLARATION (smILSSpacingFailed)

public:
	smILSSpacingFailed ();
	smILSSpacingFailed (const smILSSpacingFailed& note);
	virtual ~smILSSpacingFailed ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNoLadderFile : public SmartNotice {

PERSISTENT_DECLARATION (smNoLadderFile)

public:
	smNoLadderFile ();
	smNoLadderFile (const smNoLadderFile& note);
	virtual ~smNoLadderFile ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLadderTypeUnknown : public SmartNotice {

PERSISTENT_DECLARATION (smLadderTypeUnknown)

public:
	smLadderTypeUnknown ();
	smLadderTypeUnknown (const smLadderTypeUnknown& note);
	virtual ~smLadderTypeUnknown ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smReagentKitUnknown : public SmartNotice {

PERSISTENT_DECLARATION (smReagentKitUnknown)

public:
	smReagentKitUnknown ();
	smReagentKitUnknown (const smReagentKitUnknown& note);
	virtual ~smReagentKitUnknown ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smMoreThanOneReagentKit : public SmartNotice {

PERSISTENT_DECLARATION (smMoreThanOneReagentKit)

public:
	smMoreThanOneReagentKit ();
	smMoreThanOneReagentKit (const smMoreThanOneReagentKit& note);
	virtual ~smMoreThanOneReagentKit ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCurveFitFailed : public SmartNotice {

PERSISTENT_DECLARATION (smCurveFitFailed)

public:
	smCurveFitFailed ();
	smCurveFitFailed (const smCurveFitFailed& note);
	virtual ~smCurveFitFailed ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCurveFitMarginal : public SmartNotice {

PERSISTENT_DECLARATION (smCurveFitMarginal)

public:
	smCurveFitMarginal ();
	smCurveFitMarginal (const smCurveFitMarginal& note);
	virtual ~smCurveFitMarginal ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSpike : public SmartNotice {

PERSISTENT_DECLARATION (smSpike)

public:
	smSpike ();
	smSpike (const smSpike& note);
	virtual ~smSpike ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smStutter : public SmartNotice {

PERSISTENT_DECLARATION (smStutter)

public:
	smStutter ();
	smStutter (const smStutter& note);
	virtual ~smStutter ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAdenylation : public SmartNotice {

PERSISTENT_DECLARATION (smAdenylation)

public:
	smAdenylation ();
	smAdenylation (const smAdenylation& note);
	virtual ~smAdenylation ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBlob : public SmartNotice {

PERSISTENT_DECLARATION (smBlob)

public:
	smBlob ();
	smBlob (const smBlob& note);
	virtual ~smBlob ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPullUp : public SmartNotice {

PERSISTENT_DECLARATION (smPullUp)

public:
	smPullUp ();
	smPullUp (const smPullUp& note);
	virtual ~smPullUp ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSTooFewPeaks : public SmartNotice {

PERSISTENT_DECLARATION (smILSTooFewPeaks)

public:
	smILSTooFewPeaks ();
	smILSTooFewPeaks (const smILSTooFewPeaks& note);
	virtual ~smILSTooFewPeaks ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPeakOutsideILS : public SmartNotice {

PERSISTENT_DECLARATION (smPeakOutsideILS)

public:
	smPeakOutsideILS ();
	smPeakOutsideILS (const smPeakOutsideILS& note);
	virtual ~smPeakOutsideILS ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smMarkerSetNameUnknown : public SmartNotice {

PERSISTENT_DECLARATION (smMarkerSetNameUnknown)

public:
	smMarkerSetNameUnknown ();
	smMarkerSetNameUnknown (const smMarkerSetNameUnknown& note);
	virtual ~smMarkerSetNameUnknown ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNamedILSUnknown : public SmartNotice {

PERSISTENT_DECLARATION (smNamedILSUnknown)

public:
	smNamedILSUnknown ();
	smNamedILSUnknown (const smNamedILSUnknown& note);
	virtual ~smNamedILSUnknown ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBelowMinRFU : public SmartNotice {

PERSISTENT_DECLARATION (smBelowMinRFU)

public:
	smBelowMinRFU ();
	smBelowMinRFU (const smBelowMinRFU& note);
	virtual ~smBelowMinRFU ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smUnexpectedILSPeak : public SmartNotice {

PERSISTENT_DECLARATION (smUnexpectedILSPeak)

public:
	smUnexpectedILSPeak ();
	smUnexpectedILSPeak (const smUnexpectedILSPeak& note);
	virtual ~smUnexpectedILSPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNoPosCtrlFound : public SmartNotice {

PERSISTENT_DECLARATION (smNoPosCtrlFound)

public:
	smNoPosCtrlFound ();
	smNoPosCtrlFound (const smNoPosCtrlFound& note);
	virtual ~smNoPosCtrlFound ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNoNegCtrlFound : public SmartNotice {

PERSISTENT_DECLARATION (smNoNegCtrlFound)

public:
	smNoNegCtrlFound ();
	smNoNegCtrlFound (const smNoNegCtrlFound& note);
	virtual ~smNoNegCtrlFound ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNamedPosCtrlNotFound : public SmartNotice {

PERSISTENT_DECLARATION (smNamedPosCtrlNotFound)

public:
	smNamedPosCtrlNotFound ();
	smNamedPosCtrlNotFound (const smNamedPosCtrlNotFound& note);
	virtual ~smNamedPosCtrlNotFound ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPeakHeightAboveMax : public SmartNotice {

PERSISTENT_DECLARATION (smPeakHeightAboveMax)

public:
	smPeakHeightAboveMax ();
	smPeakHeightAboveMax (const smPeakHeightAboveMax& note);
	virtual ~smPeakHeightAboveMax ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCannotSeparateILSPrimerPeaks : public SmartNotice {

PERSISTENT_DECLARATION (smCannotSeparateILSPrimerPeaks)

public:
	smCannotSeparateILSPrimerPeaks ();
	smCannotSeparateILSPrimerPeaks (const smCannotSeparateILSPrimerPeaks& note);
	virtual ~smCannotSeparateILSPrimerPeaks ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSNeighborFilterTooAggressive : public SmartNotice {

PERSISTENT_DECLARATION (smILSNeighborFilterTooAggressive)

public:
	smILSNeighborFilterTooAggressive ();
	smILSNeighborFilterTooAggressive (const smILSNeighborFilterTooAggressive& note);
	virtual ~smILSNeighborFilterTooAggressive ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSFractionFilterTooAggressive : public SmartNotice {

PERSISTENT_DECLARATION (smILSFractionFilterTooAggressive)

public:
	smILSFractionFilterTooAggressive ();
	smILSFractionFilterTooAggressive (const smILSFractionFilterTooAggressive& note);
	virtual ~smILSFractionFilterTooAggressive ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSRelativeHeightsInconsistent : public SmartNotice {

PERSISTENT_DECLARATION (smILSRelativeHeightsInconsistent)

public:
	smILSRelativeHeightsInconsistent ();
	smILSRelativeHeightsInconsistent (const smILSRelativeHeightsInconsistent& note);
	virtual ~smILSRelativeHeightsInconsistent ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smOsirisBelowMinRFUButRawDataAbove : public SmartNotice {

PERSISTENT_DECLARATION (smOsirisBelowMinRFUButRawDataAbove)

public:
	smOsirisBelowMinRFUButRawDataAbove ();
	smOsirisBelowMinRFUButRawDataAbove (const smOsirisBelowMinRFUButRawDataAbove& note);
	virtual ~smOsirisBelowMinRFUButRawDataAbove ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smRawDataBelowMinRFUButOsirisAbove : public SmartNotice {

PERSISTENT_DECLARATION (smRawDataBelowMinRFUButOsirisAbove)

public:
	smRawDataBelowMinRFUButOsirisAbove ();
	smRawDataBelowMinRFUButOsirisAbove (const smRawDataBelowMinRFUButOsirisAbove& note);
	virtual ~smRawDataBelowMinRFUButOsirisAbove ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smOsirisAboveMaxRFUButRawDataBelow : public SmartNotice {

PERSISTENT_DECLARATION (smOsirisAboveMaxRFUButRawDataBelow)

public:
	smOsirisAboveMaxRFUButRawDataBelow ();
	smOsirisAboveMaxRFUButRawDataBelow (const smOsirisAboveMaxRFUButRawDataBelow& note);
	virtual ~smOsirisAboveMaxRFUButRawDataBelow ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smRawDataAboveMaxRFUButOsirisBelow : public SmartNotice {

PERSISTENT_DECLARATION (smRawDataAboveMaxRFUButOsirisBelow)

public:
	smRawDataAboveMaxRFUButOsirisBelow ();
	smRawDataAboveMaxRFUButOsirisBelow (const smRawDataAboveMaxRFUButOsirisBelow& note);
	virtual ~smRawDataAboveMaxRFUButOsirisBelow ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smStage1Successful : public SmartNotice {

PERSISTENT_DECLARATION (smStage1Successful)

public:
	smStage1Successful ();
	smStage1Successful (const smStage1Successful& note);
	virtual ~smStage1Successful ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSignalIsCtrlPeak : public SmartNotice {

PERSISTENT_DECLARATION (smSignalIsCtrlPeak)

public:
	smSignalIsCtrlPeak ();
	smSignalIsCtrlPeak (const smSignalIsCtrlPeak& note);
	virtual ~smSignalIsCtrlPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusTooFewPeaks : public SmartNotice {

PERSISTENT_DECLARATION (smLocusTooFewPeaks)

public:
	smLocusTooFewPeaks ();
	smLocusTooFewPeaks (const smLocusTooFewPeaks& note);
	virtual ~smLocusTooFewPeaks ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smInterlocusLadderPeak : public SmartNotice {

PERSISTENT_DECLARATION (smInterlocusLadderPeak)

public:
	smInterlocusLadderPeak ();
	smInterlocusLadderPeak (const smInterlocusLadderPeak& note);
	virtual ~smInterlocusLadderPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smUnexpectedLadderLocusPeak : public SmartNotice {

PERSISTENT_DECLARATION (smUnexpectedLadderLocusPeak)

public:
	smUnexpectedLadderLocusPeak ();
	smUnexpectedLadderLocusPeak (const smUnexpectedLadderLocusPeak& note);
	virtual ~smUnexpectedLadderLocusPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPoorPeakMorphology : public SmartNotice {

PERSISTENT_DECLARATION (smPoorPeakMorphology)

public:
	smPoorPeakMorphology ();
	smPoorPeakMorphology (const smPoorPeakMorphology& note);
	virtual ~smPoorPeakMorphology ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPoorLocusMorphology : public SmartNotice {

PERSISTENT_DECLARATION (smPoorLocusMorphology)

public:
	smPoorLocusMorphology ();
	smPoorLocusMorphology (const smPoorLocusMorphology& note);
	virtual ~smPoorLocusMorphology ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusMayHaveUnreportedOLs : public SmartNotice {

PERSISTENT_DECLARATION (smLocusMayHaveUnreportedOLs)

public:
	smLocusMayHaveUnreportedOLs ();
	smLocusMayHaveUnreportedOLs (const smLocusMayHaveUnreportedOLs& note);
	virtual ~smLocusMayHaveUnreportedOLs ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNPeaksForLocusUnavailable : public SmartNotice {

PERSISTENT_DECLARATION (smNPeaksForLocusUnavailable)

public:
	smNPeaksForLocusUnavailable ();
	smNPeaksForLocusUnavailable (const smNPeaksForLocusUnavailable& note);
	virtual ~smNPeaksForLocusUnavailable ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smRelativeHeightImbalance : public SmartNotice {

PERSISTENT_DECLARATION (smRelativeHeightImbalance)

public:
	smRelativeHeightImbalance ();
	smRelativeHeightImbalance (const smRelativeHeightImbalance& note);
	virtual ~smRelativeHeightImbalance ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smHeightBelowFractionalFilter : public SmartNotice {

PERSISTENT_DECLARATION (smHeightBelowFractionalFilter)

public:
	smHeightBelowFractionalFilter ();
	smHeightBelowFractionalFilter (const smHeightBelowFractionalFilter& note);
	virtual ~smHeightBelowFractionalFilter ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPartOfExtendedLocusLeft : public SmartNotice {

PERSISTENT_DECLARATION (smPartOfExtendedLocusLeft)

public:
	smPartOfExtendedLocusLeft ();
	smPartOfExtendedLocusLeft (const smPartOfExtendedLocusLeft& note);
	virtual ~smPartOfExtendedLocusLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPartOfExtendedLocusRight : public SmartNotice {

PERSISTENT_DECLARATION (smPartOfExtendedLocusRight)

public:
	smPartOfExtendedLocusRight ();
	smPartOfExtendedLocusRight (const smPartOfExtendedLocusRight& note);
	virtual ~smPartOfExtendedLocusRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBelowFractionalFilterLeft : public SmartNotice {

PERSISTENT_DECLARATION (smBelowFractionalFilterLeft)

public:
	smBelowFractionalFilterLeft ();
	smBelowFractionalFilterLeft (const smBelowFractionalFilterLeft& note);
	virtual ~smBelowFractionalFilterLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBelowFractionalFilterRight : public SmartNotice {

PERSISTENT_DECLARATION (smBelowFractionalFilterRight)

public:
	smBelowFractionalFilterRight ();
	smBelowFractionalFilterRight (const smBelowFractionalFilterRight& note);
	virtual ~smBelowFractionalFilterRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smStage2Successful : public SmartNotice {

PERSISTENT_DECLARATION (smStage2Successful)

public:
	smStage2Successful ();
	smStage2Successful (const smStage2Successful& note);
	virtual ~smStage2Successful ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPullUpAtControlPeak : public SmartNotice {

PERSISTENT_DECLARATION (smPullUpAtControlPeak)

public:
	smPullUpAtControlPeak ();
	smPullUpAtControlPeak (const smPullUpAtControlPeak& note);
	virtual ~smPullUpAtControlPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smInterlocusPullUpWithPoorFit : public SmartNotice {

PERSISTENT_DECLARATION (smInterlocusPullUpWithPoorFit)

public:
	smInterlocusPullUpWithPoorFit ();
	smInterlocusPullUpWithPoorFit (const smInterlocusPullUpWithPoorFit& note);
	virtual ~smInterlocusPullUpWithPoorFit ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPossibleValidInterlocusLeft : public SmartNotice {

PERSISTENT_DECLARATION (smPossibleValidInterlocusLeft)

public:
	smPossibleValidInterlocusLeft ();
	smPossibleValidInterlocusLeft (const smPossibleValidInterlocusLeft& note);
	virtual ~smPossibleValidInterlocusLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPossibleValidInterlocusRight : public SmartNotice {

PERSISTENT_DECLARATION (smPossibleValidInterlocusRight)

public:
	smPossibleValidInterlocusRight ();
	smPossibleValidInterlocusRight (const smPossibleValidInterlocusRight& note);
	virtual ~smPossibleValidInterlocusRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smUnassignedInterlocusPeak : public SmartNotice {

PERSISTENT_DECLARATION (smUnassignedInterlocusPeak)

public:
	smUnassignedInterlocusPeak ();
	smUnassignedInterlocusPeak (const smUnassignedInterlocusPeak& note);
	virtual ~smUnassignedInterlocusPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAmbiguousInterlocus : public SmartNotice {

PERSISTENT_DECLARATION (smAmbiguousInterlocus)

public:
	smAmbiguousInterlocus ();
	smAmbiguousInterlocus (const smAmbiguousInterlocus& note);
	virtual ~smAmbiguousInterlocus ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smTooLowInterlocusPeak : public SmartNotice {

PERSISTENT_DECLARATION (smTooLowInterlocusPeak)

public:
	smTooLowInterlocusPeak ();
	smTooLowInterlocusPeak (const smTooLowInterlocusPeak& note);
	virtual ~smTooLowInterlocusPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPossibleInterlocusArtifactLeft : public SmartNotice {

PERSISTENT_DECLARATION (smPossibleInterlocusArtifactLeft)

public:
	smPossibleInterlocusArtifactLeft ();
	smPossibleInterlocusArtifactLeft (const smPossibleInterlocusArtifactLeft& note);
	virtual ~smPossibleInterlocusArtifactLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPossibleInterlocusArtifactRight : public SmartNotice {

PERSISTENT_DECLARATION (smPossibleInterlocusArtifactRight)

public:
	smPossibleInterlocusArtifactRight ();
	smPossibleInterlocusArtifactRight (const smPossibleInterlocusArtifactRight& note);
	virtual ~smPossibleInterlocusArtifactRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smFirstAlleleEqFirstLadderPeak : public SmartNotice {

PERSISTENT_DECLARATION (smFirstAlleleEqFirstLadderPeak)

public:
	smFirstAlleleEqFirstLadderPeak ();
	smFirstAlleleEqFirstLadderPeak (const smFirstAlleleEqFirstLadderPeak& note);
	virtual ~smFirstAlleleEqFirstLadderPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSignalOL : public SmartNotice {

PERSISTENT_DECLARATION (smSignalOL)

public:
	smSignalOL ();
	smSignalOL (const smSignalOL& note);
	virtual ~smSignalOL ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAMELMisaligned : public SmartNotice {

PERSISTENT_DECLARATION (smAMELMisaligned)

public:
	smAMELMisaligned ();
	smAMELMisaligned (const smAMELMisaligned& note);
	virtual ~smAMELMisaligned ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCrater : public SmartNotice {

PERSISTENT_DECLARATION (smCrater)

public:
	smCrater ();
	smCrater (const smCrater& note);
	virtual ~smCrater ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAcceptedOLLeft : public SmartNotice {

PERSISTENT_DECLARATION (smAcceptedOLLeft)

public:
	smAcceptedOLLeft ();
	smAcceptedOLLeft (const smAcceptedOLLeft& note);
	virtual ~smAcceptedOLLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAcceptedOLRight : public SmartNotice {

PERSISTENT_DECLARATION (smAcceptedOLRight)

public:
	smAcceptedOLRight ();
	smAcceptedOLRight (const smAcceptedOLRight& note);
	virtual ~smAcceptedOLRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smStage3Successful : public SmartNotice {

PERSISTENT_DECLARATION (smStage3Successful)

public:
	smStage3Successful ();
	smStage3Successful (const smStage3Successful& note);
	virtual ~smStage3Successful ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smValidOLInterlocusLeft : public SmartNotice {

PERSISTENT_DECLARATION (smValidOLInterlocusLeft)

public:
	smValidOLInterlocusLeft ();
	smValidOLInterlocusLeft (const smValidOLInterlocusLeft& note);
	virtual ~smValidOLInterlocusLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smValidOLInterlocusRight : public SmartNotice {

PERSISTENT_DECLARATION (smValidOLInterlocusRight)

public:
	smValidOLInterlocusRight ();
	smValidOLInterlocusRight (const smValidOLInterlocusRight& note);
	virtual ~smValidOLInterlocusRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAcceptedInterlocusLeft : public SmartNotice {

PERSISTENT_DECLARATION (smAcceptedInterlocusLeft)

public:
	smAcceptedInterlocusLeft ();
	smAcceptedInterlocusLeft (const smAcceptedInterlocusLeft& note);
	virtual ~smAcceptedInterlocusLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAcceptedInterlocusRight : public SmartNotice {

PERSISTENT_DECLARATION (smAcceptedInterlocusRight)

public:
	smAcceptedInterlocusRight ();
	smAcceptedInterlocusRight (const smAcceptedInterlocusRight& note);
	virtual ~smAcceptedInterlocusRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNoXForAMEL : public SmartNotice {

PERSISTENT_DECLARATION (smNoXForAMEL)

public:
	smNoXForAMEL ();
	smNoXForAMEL (const smNoXForAMEL& note);
	virtual ~smNoXForAMEL ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSignalIsAllele : public SmartNotice {

PERSISTENT_DECLARATION (smSignalIsAllele)

public:
	smSignalIsAllele ();
	smSignalIsAllele (const smSignalIsAllele& note);
	virtual ~smSignalIsAllele ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPosCtrlLocusMismatch : public SmartNotice {

PERSISTENT_DECLARATION (smPosCtrlLocusMismatch)

public:
	smPosCtrlLocusMismatch ();
	smPosCtrlLocusMismatch (const smPosCtrlLocusMismatch& note);
	virtual ~smPosCtrlLocusMismatch ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNoGenotypeFound : public SmartNotice {

PERSISTENT_DECLARATION (smNoGenotypeFound)

public:
	smNoGenotypeFound ();
	smNoGenotypeFound (const smNoGenotypeFound& note);
	virtual ~smNoGenotypeFound ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smThreeOrMoreAlleles : public SmartNotice {

PERSISTENT_DECLARATION (smThreeOrMoreAlleles)

public:
	smThreeOrMoreAlleles ();
	smThreeOrMoreAlleles (const smThreeOrMoreAlleles& note);
	virtual ~smThreeOrMoreAlleles ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smTwoOrMoreAlleles : public SmartNotice {

PERSISTENT_DECLARATION (smTwoOrMoreAlleles)

public:
	smTwoOrMoreAlleles ();
	smTwoOrMoreAlleles (const smTwoOrMoreAlleles& note);
	virtual ~smTwoOrMoreAlleles ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smHeterozygousImbalance : public SmartNotice {

PERSISTENT_DECLARATION (smHeterozygousImbalance)

public:
	smHeterozygousImbalance ();
	smHeterozygousImbalance (const smHeterozygousImbalance& note);
	virtual ~smHeterozygousImbalance ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smOLAlleles : public SmartNotice {

PERSISTENT_DECLARATION (smOLAlleles)

public:
	smOLAlleles ();
	smOLAlleles (const smOLAlleles& note);
	virtual ~smOLAlleles ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smHomozygoteHeightProblem : public SmartNotice {

PERSISTENT_DECLARATION (smHomozygoteHeightProblem)

public:
	smHomozygoteHeightProblem ();
	smHomozygoteHeightProblem (const smHomozygoteHeightProblem& note);
	virtual ~smHomozygoteHeightProblem ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPullUpAtAllele : public SmartNotice {

PERSISTENT_DECLARATION (smPullUpAtAllele)

public:
	smPullUpAtAllele ();
	smPullUpAtAllele (const smPullUpAtAllele& note);
	virtual ~smPullUpAtAllele ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPosCtrlLocusNotFound : public SmartNotice {

PERSISTENT_DECLARATION (smPosCtrlLocusNotFound)

public:
	smPosCtrlLocusNotFound ();
	smPosCtrlLocusNotFound (const smPosCtrlLocusNotFound& note);
	virtual ~smPosCtrlLocusNotFound ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBPResidualTooLarge : public SmartNotice {

PERSISTENT_DECLARATION (smBPResidualTooLarge)

public:
	smBPResidualTooLarge ();
	smBPResidualTooLarge (const smBPResidualTooLarge& note);
	virtual ~smBPResidualTooLarge ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smExtraLadderAllele : public SmartNotice {

PERSISTENT_DECLARATION (smExtraLadderAllele)

public:
	smExtraLadderAllele ();
	smExtraLadderAllele (const smExtraLadderAllele& note);
	virtual ~smExtraLadderAllele ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCriticalMessagesAtILS : public SmartNotice {

PERSISTENT_DECLARATION (smCriticalMessagesAtILS)

public:
	smCriticalMessagesAtILS ();
	smCriticalMessagesAtILS (const smCriticalMessagesAtILS& note);
	virtual ~smCriticalMessagesAtILS ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAMELHasExtraneousPeaks : public SmartNotice {

PERSISTENT_DECLARATION (smAMELHasExtraneousPeaks)

public:
	smAMELHasExtraneousPeaks ();
	smAMELHasExtraneousPeaks (const smAMELHasExtraneousPeaks& note);
	virtual ~smAMELHasExtraneousPeaks ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smExtraneousAMELPeak : public SmartNotice {

PERSISTENT_DECLARATION (smExtraneousAMELPeak)

public:
	smExtraneousAMELPeak ();
	smExtraneousAMELPeak (const smExtraneousAMELPeak& note);
	virtual ~smExtraneousAMELPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNegCtrlChannelHasNoPrimerPeaks : public SmartNotice {

PERSISTENT_DECLARATION (smNegCtrlChannelHasNoPrimerPeaks)

public:
	smNegCtrlChannelHasNoPrimerPeaks ();
	smNegCtrlChannelHasNoPrimerPeaks (const smNegCtrlChannelHasNoPrimerPeaks& note);
	virtual ~smNegCtrlChannelHasNoPrimerPeaks ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smMinimumHeightForPrimerPeaks : public SmartNotice {

PERSISTENT_DECLARATION (smMinimumHeightForPrimerPeaks)

public:
	smMinimumHeightForPrimerPeaks ();
	smMinimumHeightForPrimerPeaks (const smMinimumHeightForPrimerPeaks& note);
	virtual ~smMinimumHeightForPrimerPeaks ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smInterlocusPeaksToRight : public SmartNotice {

PERSISTENT_DECLARATION (smInterlocusPeaksToRight)

public:
	smInterlocusPeaksToRight ();
	smInterlocusPeaksToRight (const smInterlocusPeaksToRight& note);
	virtual ~smInterlocusPeaksToRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smInterlocusPeaksToLeft : public SmartNotice {

PERSISTENT_DECLARATION (smInterlocusPeaksToLeft)

public:
	smInterlocusPeaksToLeft ();
	smInterlocusPeaksToLeft (const smInterlocusPeaksToLeft& note);
	virtual ~smInterlocusPeaksToLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLadderFailed : public SmartNotice {

PERSISTENT_DECLARATION (smLadderFailed)

public:
	smLadderFailed ();
	smLadderFailed (const smLadderFailed& note);
	virtual ~smLadderFailed ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSampleUnanalyzable : public SmartNotice {

PERSISTENT_DECLARATION (smSampleUnanalyzable)

public:
	smSampleUnanalyzable ();
	smSampleUnanalyzable (const smSampleUnanalyzable& note);
	virtual ~smSampleUnanalyzable ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCriticalMessagesAtAlleles : public SmartNotice {

PERSISTENT_DECLARATION (smCriticalMessagesAtAlleles)

public:
	smCriticalMessagesAtAlleles ();
	smCriticalMessagesAtAlleles (const smCriticalMessagesAtAlleles& note);
	virtual ~smCriticalMessagesAtAlleles ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smHeightBelowPullupFractionalFilter : public SmartNotice {

PERSISTENT_DECLARATION (smHeightBelowPullupFractionalFilter)

public:
	smHeightBelowPullupFractionalFilter ();
	smHeightBelowPullupFractionalFilter (const smHeightBelowPullupFractionalFilter& note);
	virtual ~smHeightBelowPullupFractionalFilter ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBelowPullupFractionalFilterLeft : public SmartNotice {

PERSISTENT_DECLARATION (smBelowPullupFractionalFilterLeft)

public:
	smBelowPullupFractionalFilterLeft ();
	smBelowPullupFractionalFilterLeft (const smBelowPullupFractionalFilterLeft& note);
	virtual ~smBelowPullupFractionalFilterLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBelowPullupFractionalFilterRight : public SmartNotice {

PERSISTENT_DECLARATION (smBelowPullupFractionalFilterRight)

public:
	smBelowPullupFractionalFilterRight ();
	smBelowPullupFractionalFilterRight (const smBelowPullupFractionalFilterRight& note);
	virtual ~smBelowPullupFractionalFilterRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPossibleExtraLocusAllele : public SmartNotice {

PERSISTENT_DECLARATION (smPossibleExtraLocusAllele)

public:
	smPossibleExtraLocusAllele ();
	smPossibleExtraLocusAllele (const smPossibleExtraLocusAllele& note);
	virtual ~smPossibleExtraLocusAllele ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smUnexpectedPeakInNegativeControl : public SmartNotice {

PERSISTENT_DECLARATION (smUnexpectedPeakInNegativeControl)

public:
	smUnexpectedPeakInNegativeControl ();
	smUnexpectedPeakInNegativeControl (const smUnexpectedPeakInNegativeControl& note);
	virtual ~smUnexpectedPeakInNegativeControl ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusContainsPeaksWithExcessiveResiduals : public SmartNotice {

PERSISTENT_DECLARATION (smLocusContainsPeaksWithExcessiveResiduals)

public:
	smLocusContainsPeaksWithExcessiveResiduals ();
	smLocusContainsPeaksWithExcessiveResiduals (const smLocusContainsPeaksWithExcessiveResiduals& note);
	virtual ~smLocusContainsPeaksWithExcessiveResiduals ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPrimaryInterchannelLink : public SmartNotice {

PERSISTENT_DECLARATION (smPrimaryInterchannelLink)

public:
	smPrimaryInterchannelLink ();
	smPrimaryInterchannelLink (const smPrimaryInterchannelLink& note);
	virtual ~smPrimaryInterchannelLink ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNotInterchannelLink : public SmartNotice {

PERSISTENT_DECLARATION (smNotInterchannelLink)

public:
	smNotInterchannelLink ();
	smNotInterchannelLink (const smNotInterchannelLink& note);
	virtual ~smNotInterchannelLink ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smHeightBelowInterlocusMinRFU : public SmartNotice {

PERSISTENT_DECLARATION (smHeightBelowInterlocusMinRFU)

public:
	smHeightBelowInterlocusMinRFU ();
	smHeightBelowInterlocusMinRFU (const smHeightBelowInterlocusMinRFU& note);
	virtual ~smHeightBelowInterlocusMinRFU ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smFSAChannelUnreadable : public SmartNotice {

PERSISTENT_DECLARATION (smFSAChannelUnreadable)

public:
	smFSAChannelUnreadable ();
	smFSAChannelUnreadable (const smFSAChannelUnreadable& note);
	virtual ~smFSAChannelUnreadable ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAmbiguousInterlocusCrater : public SmartNotice {

PERSISTENT_DECLARATION (smAmbiguousInterlocusCrater)

public:
	smAmbiguousInterlocusCrater ();
	smAmbiguousInterlocusCrater (const smAmbiguousInterlocusCrater& note);
	virtual ~smAmbiguousInterlocusCrater ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPossibleAmbiguousInterlocusAssignedToLocus : public SmartNotice {

PERSISTENT_DECLARATION (smPossibleAmbiguousInterlocusAssignedToLocus)

public:
	smPossibleAmbiguousInterlocusAssignedToLocus ();
	smPossibleAmbiguousInterlocusAssignedToLocus (const smPossibleAmbiguousInterlocusAssignedToLocus& note);
	virtual ~smPossibleAmbiguousInterlocusAssignedToLocus ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smWouldCauseHeterozygousImbalanceLeft : public SmartNotice {

PERSISTENT_DECLARATION (smWouldCauseHeterozygousImbalanceLeft)

public:
	smWouldCauseHeterozygousImbalanceLeft ();
	smWouldCauseHeterozygousImbalanceLeft (const smWouldCauseHeterozygousImbalanceLeft& note);
	virtual ~smWouldCauseHeterozygousImbalanceLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smWouldCauseHeterozygousImbalanceRight : public SmartNotice {

PERSISTENT_DECLARATION (smWouldCauseHeterozygousImbalanceRight)

public:
	smWouldCauseHeterozygousImbalanceRight ();
	smWouldCauseHeterozygousImbalanceRight (const smWouldCauseHeterozygousImbalanceRight& note);
	virtual ~smWouldCauseHeterozygousImbalanceRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSignalOffGridLeft : public SmartNotice {

PERSISTENT_DECLARATION (smSignalOffGridLeft)

public:
	smSignalOffGridLeft ();
	smSignalOffGridLeft (const smSignalOffGridLeft& note);
	virtual ~smSignalOffGridLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSignalOffGridRight : public SmartNotice {

PERSISTENT_DECLARATION (smSignalOffGridRight)

public:
	smSignalOffGridRight ();
	smSignalOffGridRight (const smSignalOffGridRight& note);
	virtual ~smSignalOffGridRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm0UnambiguousPeaksLeft : public SmartNotice {

PERSISTENT_DECLARATION (sm0UnambiguousPeaksLeft)

public:
	sm0UnambiguousPeaksLeft ();
	sm0UnambiguousPeaksLeft (const sm0UnambiguousPeaksLeft& note);
	virtual ~sm0UnambiguousPeaksLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm0UnambiguousPeaksRight : public SmartNotice {

PERSISTENT_DECLARATION (sm0UnambiguousPeaksRight)

public:
	sm0UnambiguousPeaksRight ();
	sm0UnambiguousPeaksRight (const sm0UnambiguousPeaksRight& note);
	virtual ~sm0UnambiguousPeaksRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm1UnambiguousPeakLeft : public SmartNotice {

PERSISTENT_DECLARATION (sm1UnambiguousPeakLeft)

public:
	sm1UnambiguousPeakLeft ();
	sm1UnambiguousPeakLeft (const sm1UnambiguousPeakLeft& note);
	virtual ~sm1UnambiguousPeakLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm1UnambiguousPeakRight : public SmartNotice {

PERSISTENT_DECLARATION (sm1UnambiguousPeakRight)

public:
	sm1UnambiguousPeakRight ();
	sm1UnambiguousPeakRight (const sm1UnambiguousPeakRight& note);
	virtual ~sm1UnambiguousPeakRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm2PlusUnambiguousPeaksLeft : public SmartNotice {

PERSISTENT_DECLARATION (sm2PlusUnambiguousPeaksLeft)

public:
	sm2PlusUnambiguousPeaksLeft ();
	sm2PlusUnambiguousPeaksLeft (const sm2PlusUnambiguousPeaksLeft& note);
	virtual ~sm2PlusUnambiguousPeaksLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm2PlusUnambiguousPeaksRight : public SmartNotice {

PERSISTENT_DECLARATION (sm2PlusUnambiguousPeaksRight)

public:
	sm2PlusUnambiguousPeaksRight ();
	sm2PlusUnambiguousPeaksRight (const sm2PlusUnambiguousPeaksRight& note);
	virtual ~sm2PlusUnambiguousPeaksRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm0AmbiguousPeaksLeft : public SmartNotice {

PERSISTENT_DECLARATION (sm0AmbiguousPeaksLeft)

public:
	sm0AmbiguousPeaksLeft ();
	sm0AmbiguousPeaksLeft (const sm0AmbiguousPeaksLeft& note);
	virtual ~sm0AmbiguousPeaksLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm0AmbiguousPeaksRight : public SmartNotice {

PERSISTENT_DECLARATION (sm0AmbiguousPeaksRight)

public:
	sm0AmbiguousPeaksRight ();
	sm0AmbiguousPeaksRight (const sm0AmbiguousPeaksRight& note);
	virtual ~sm0AmbiguousPeaksRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm1AmbiguousPeakLeft : public SmartNotice {

PERSISTENT_DECLARATION (sm1AmbiguousPeakLeft)

public:
	sm1AmbiguousPeakLeft ();
	sm1AmbiguousPeakLeft (const sm1AmbiguousPeakLeft& note);
	virtual ~sm1AmbiguousPeakLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm1AmbiguousPeakRight : public SmartNotice {

PERSISTENT_DECLARATION (sm1AmbiguousPeakRight)

public:
	sm1AmbiguousPeakRight ();
	sm1AmbiguousPeakRight (const sm1AmbiguousPeakRight& note);
	virtual ~sm1AmbiguousPeakRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm2PlusAmbiguousPeaksLeft : public SmartNotice {

PERSISTENT_DECLARATION (sm2PlusAmbiguousPeaksLeft)

public:
	sm2PlusAmbiguousPeaksLeft ();
	sm2PlusAmbiguousPeaksLeft (const sm2PlusAmbiguousPeaksLeft& note);
	virtual ~sm2PlusAmbiguousPeaksLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class sm2PlusAmbiguousPeaksRight : public SmartNotice {

PERSISTENT_DECLARATION (sm2PlusAmbiguousPeaksRight)

public:
	sm2PlusAmbiguousPeaksRight ();
	sm2PlusAmbiguousPeaksRight (const sm2PlusAmbiguousPeaksRight& note);
	virtual ~sm2PlusAmbiguousPeaksRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCouldBelongToLocusLeft : public SmartNotice {

PERSISTENT_DECLARATION (smCouldBelongToLocusLeft)

public:
	smCouldBelongToLocusLeft ();
	smCouldBelongToLocusLeft (const smCouldBelongToLocusLeft& note);
	virtual ~smCouldBelongToLocusLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCouldBelongToLocusRight : public SmartNotice {

PERSISTENT_DECLARATION (smCouldBelongToLocusRight)

public:
	smCouldBelongToLocusRight ();
	smCouldBelongToLocusRight (const smCouldBelongToLocusRight& note);
	virtual ~smCouldBelongToLocusRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smChannelIsNegControl : public SmartNotice {

PERSISTENT_DECLARATION (smChannelIsNegControl)

public:
	smChannelIsNegControl ();
	smChannelIsNegControl (const smChannelIsNegControl& note);
	virtual ~smChannelIsNegControl ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusIsNegControl : public SmartNotice {

PERSISTENT_DECLARATION (smLocusIsNegControl)

public:
	smLocusIsNegControl ();
	smLocusIsNegControl (const smLocusIsNegControl& note);
	virtual ~smLocusIsNegControl ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smChannelIsPosControl : public SmartNotice {

PERSISTENT_DECLARATION (smChannelIsPosControl)

public:
	smChannelIsPosControl ();
	smChannelIsPosControl (const smChannelIsPosControl& note);
	virtual ~smChannelIsPosControl ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusIsPosControl : public SmartNotice {

PERSISTENT_DECLARATION (smLocusIsPosControl)

public:
	smLocusIsPosControl ();
	smLocusIsPosControl (const smLocusIsPosControl& note);
	virtual ~smLocusIsPosControl ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPurePullup : public SmartNotice {

PERSISTENT_DECLARATION (smPurePullup)

public:
	smPurePullup ();
	smPurePullup (const smPurePullup& note);
	virtual ~smPurePullup ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smChannelPeaksHaveCriticalArtifacts : public SmartNotice {

PERSISTENT_DECLARATION (smChannelPeaksHaveCriticalArtifacts)

public:
	smChannelPeaksHaveCriticalArtifacts ();
	smChannelPeaksHaveCriticalArtifacts (const smChannelPeaksHaveCriticalArtifacts& note);
	virtual ~smChannelPeaksHaveCriticalArtifacts ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPreliminaryLocusHas2OrMoreAlleles : public SmartNotice {

PERSISTENT_DECLARATION (smPreliminaryLocusHas2OrMoreAlleles)

public:
	smPreliminaryLocusHas2OrMoreAlleles ();
	smPreliminaryLocusHas2OrMoreAlleles (const smPreliminaryLocusHas2OrMoreAlleles& note);
	virtual ~smPreliminaryLocusHas2OrMoreAlleles ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCraterSidePeak : public SmartNotice {

PERSISTENT_DECLARATION (smCraterSidePeak)

public:
	smCraterSidePeak ();
	smCraterSidePeak (const smCraterSidePeak& note);
	virtual ~smCraterSidePeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSignalNotACrater : public SmartNotice {

PERSISTENT_DECLARATION (smSignalNotACrater)

public:
	smSignalNotACrater ();
	smSignalNotACrater (const smSignalNotACrater& note);
	virtual ~smSignalNotACrater ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSignalNotACraterSidePeak : public SmartNotice {

PERSISTENT_DECLARATION (smSignalNotACraterSidePeak)

public:
	smSignalNotACraterSidePeak ();
	smSignalNotACraterSidePeak (const smSignalNotACraterSidePeak& note);
	virtual ~smSignalNotACraterSidePeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusIsHomozygous : public SmartNotice {

PERSISTENT_DECLARATION (smLocusIsHomozygous)

public:
	smLocusIsHomozygous ();
	smLocusIsHomozygous (const smLocusIsHomozygous& note);
	virtual ~smLocusIsHomozygous ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusMissingGenotype : public SmartNotice {

PERSISTENT_DECLARATION (smLocusMissingGenotype)

public:
	smLocusMissingGenotype ();
	smLocusMissingGenotype (const smLocusMissingGenotype& note);
	virtual ~smLocusMissingGenotype ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smStage4Successful : public SmartNotice {

PERSISTENT_DECLARATION (smStage4Successful)

public:
	smStage4Successful ();
	smStage4Successful (const smStage4Successful& note);
	virtual ~smStage4Successful ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smStage5Successful : public SmartNotice {

PERSISTENT_DECLARATION (smStage5Successful)

public:
	smStage5Successful ();
	smStage5Successful (const smStage5Successful& note);
	virtual ~smStage5Successful ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPeakInCoreLadderLocus : public SmartNotice {

PERSISTENT_DECLARATION (smPeakInCoreLadderLocus)

public:
	smPeakInCoreLadderLocus ();
	smPeakInCoreLadderLocus (const smPeakInCoreLadderLocus& note);
	virtual ~smPeakInCoreLadderLocus ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPeakAboveOverloadLimit : public SmartNotice {

PERSISTENT_DECLARATION (smPeakAboveOverloadLimit)

public:
	smPeakAboveOverloadLimit ();
	smPeakAboveOverloadLimit (const smPeakAboveOverloadLimit& note);
	virtual ~smPeakAboveOverloadLimit ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smRestrictedPriorityPeak : public SmartNotice {

PERSISTENT_DECLARATION (smRestrictedPriorityPeak)

public:
	smRestrictedPriorityPeak ();
	smRestrictedPriorityPeak (const smRestrictedPriorityPeak& note);
	virtual ~smRestrictedPriorityPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smChannelIsILS : public SmartNotice {

PERSISTENT_DECLARATION (smChannelIsILS)

public:
	smChannelIsILS ();
	smChannelIsILS (const smChannelIsILS& note);
	virtual ~smChannelIsILS ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSSpacingIsMarginal : public SmartNotice {

PERSISTENT_DECLARATION (smILSSpacingIsMarginal)

public:
	smILSSpacingIsMarginal ();
	smILSSpacingIsMarginal (const smILSSpacingIsMarginal& note);
	virtual ~smILSSpacingIsMarginal ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPoorPeakMorphologyOrResolution : public SmartNotice {

PERSISTENT_DECLARATION (smPoorPeakMorphologyOrResolution)

public:
	smPoorPeakMorphologyOrResolution ();
	smPoorPeakMorphologyOrResolution (const smPoorPeakMorphologyOrResolution& note);
	virtual ~smPoorPeakMorphologyOrResolution ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPeakToRightOfILS : public SmartNotice {

PERSISTENT_DECLARATION (smPeakToRightOfILS)

public:
	smPeakToRightOfILS ();
	smPeakToRightOfILS (const smPeakToRightOfILS& note);
	virtual ~smPeakToRightOfILS ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPreliminaryLocusHasNoAlleles : public SmartNotice {

PERSISTENT_DECLARATION (smPreliminaryLocusHasNoAlleles)

public:
	smPreliminaryLocusHasNoAlleles ();
	smPreliminaryLocusHasNoAlleles (const smPreliminaryLocusHasNoAlleles& note);
	virtual ~smPreliminaryLocusHasNoAlleles ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusHasPeaksBetweenDetectionAndAnalysisThresholds : public SmartNotice {

PERSISTENT_DECLARATION (smLocusHasPeaksBetweenDetectionAndAnalysisThresholds)

public:
	smLocusHasPeaksBetweenDetectionAndAnalysisThresholds ();
	smLocusHasPeaksBetweenDetectionAndAnalysisThresholds (const smLocusHasPeaksBetweenDetectionAndAnalysisThresholds& note);
	virtual ~smLocusHasPeaksBetweenDetectionAndAnalysisThresholds ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smUnexpectedNoOfGridPeaks : public SmartNotice {

PERSISTENT_DECLARATION (smUnexpectedNoOfGridPeaks)

public:
	smUnexpectedNoOfGridPeaks ();
	smUnexpectedNoOfGridPeaks (const smUnexpectedNoOfGridPeaks& note);
	virtual ~smUnexpectedNoOfGridPeaks ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smMoreThanThreeAlleles : public SmartNotice {

PERSISTENT_DECLARATION (smMoreThanThreeAlleles)

public:
	smMoreThanThreeAlleles ();
	smMoreThanThreeAlleles (const smMoreThanThreeAlleles& note);
	virtual ~smMoreThanThreeAlleles ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smDefaultsAreOverridden : public SmartNotice {

PERSISTENT_DECLARATION (smDefaultsAreOverridden)

public:
	smDefaultsAreOverridden ();
	smDefaultsAreOverridden (const smDefaultsAreOverridden& note);
	virtual ~smDefaultsAreOverridden ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAssociatedLadderIsCritical : public SmartNotice {

PERSISTENT_DECLARATION (smAssociatedLadderIsCritical)

public:
	smAssociatedLadderIsCritical ();
	smAssociatedLadderIsCritical (const smAssociatedLadderIsCritical& note);
	virtual ~smAssociatedLadderIsCritical ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAssociatedLadderLocusIsCritical : public SmartNotice {

PERSISTENT_DECLARATION (smAssociatedLadderLocusIsCritical)

public:
	smAssociatedLadderLocusIsCritical ();
	smAssociatedLadderLocusIsCritical (const smAssociatedLadderLocusIsCritical& note);
	virtual ~smAssociatedLadderLocusIsCritical ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smChannelHasRaisedBaseline : public SmartNotice {

PERSISTENT_DECLARATION (smChannelHasRaisedBaseline)

public:
	smChannelHasRaisedBaseline ();
	smChannelHasRaisedBaseline (const smChannelHasRaisedBaseline& note);
	virtual ~smChannelHasRaisedBaseline ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smChannelHasExcessiveNoiseAboveAnalysisThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smChannelHasExcessiveNoiseAboveAnalysisThreshold)

public:
	smChannelHasExcessiveNoiseAboveAnalysisThreshold ();
	smChannelHasExcessiveNoiseAboveAnalysisThreshold (const smChannelHasExcessiveNoiseAboveAnalysisThreshold& note);
	virtual ~smChannelHasExcessiveNoiseAboveAnalysisThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smChannelHasExcessiveNoiseAboveDetectionThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smChannelHasExcessiveNoiseAboveDetectionThreshold)

public:
	smChannelHasExcessiveNoiseAboveDetectionThreshold ();
	smChannelHasExcessiveNoiseAboveDetectionThreshold (const smChannelHasExcessiveNoiseAboveDetectionThreshold& note);
	virtual ~smChannelHasExcessiveNoiseAboveDetectionThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSampleChannelRaisedBaselineThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smSampleChannelRaisedBaselineThreshold)

public:
	smSampleChannelRaisedBaselineThreshold ();
	smSampleChannelRaisedBaselineThreshold (const smSampleChannelRaisedBaselineThreshold& note);
	virtual ~smSampleChannelRaisedBaselineThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSampleILSChannelRaisedBaselineThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smSampleILSChannelRaisedBaselineThreshold)

public:
	smSampleILSChannelRaisedBaselineThreshold ();
	smSampleILSChannelRaisedBaselineThreshold (const smSampleILSChannelRaisedBaselineThreshold& note);
	virtual ~smSampleILSChannelRaisedBaselineThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSPeakMayBeIncorrect : public SmartNotice {

PERSISTENT_DECLARATION (smILSPeakMayBeIncorrect)

public:
	smILSPeakMayBeIncorrect ();
	smILSPeakMayBeIncorrect (const smILSPeakMayBeIncorrect& note);
	virtual ~smILSPeakMayBeIncorrect ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smTestForColorCorrectionMatrixPreset : public SmartNotice {

PERSISTENT_DECLARATION (smTestForColorCorrectionMatrixPreset)

public:
	smTestForColorCorrectionMatrixPreset ();
	smTestForColorCorrectionMatrixPreset (const smTestForColorCorrectionMatrixPreset& note);
	virtual ~smTestForColorCorrectionMatrixPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smColorCorrectionMatrixExpectedButNotFound : public SmartNotice {

PERSISTENT_DECLARATION (smColorCorrectionMatrixExpectedButNotFound)

public:
	smColorCorrectionMatrixExpectedButNotFound ();
	smColorCorrectionMatrixExpectedButNotFound (const smColorCorrectionMatrixExpectedButNotFound& note);
	virtual ~smColorCorrectionMatrixExpectedButNotFound ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smColorCorrectionMatrixWrongSize : public SmartNotice {

PERSISTENT_DECLARATION (smColorCorrectionMatrixWrongSize)

public:
	smColorCorrectionMatrixWrongSize ();
	smColorCorrectionMatrixWrongSize (const smColorCorrectionMatrixWrongSize& note);
	virtual ~smColorCorrectionMatrixWrongSize ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing : public SmartNotice {

PERSISTENT_DECLARATION (smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing)

public:
	smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing ();
	smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing (const smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing& note);
	virtual ~smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smMinimumNumberOfPrimerPeaksForNegativeControlChannel : public SmartNotice {

PERSISTENT_DECLARATION (smMinimumNumberOfPrimerPeaksForNegativeControlChannel)

public:
	smMinimumNumberOfPrimerPeaksForNegativeControlChannel ();
	smMinimumNumberOfPrimerPeaksForNegativeControlChannel (const smMinimumNumberOfPrimerPeaksForNegativeControlChannel& note);
	virtual ~smMinimumNumberOfPrimerPeaksForNegativeControlChannel ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smUseSampleNamesForControlSampleTestsPreset : public SmartNotice {

PERSISTENT_DECLARATION (smUseSampleNamesForControlSampleTestsPreset)

public:
	smUseSampleNamesForControlSampleTestsPreset ();
	smUseSampleNamesForControlSampleTestsPreset (const smUseSampleNamesForControlSampleTestsPreset& note);
	virtual ~smUseSampleNamesForControlSampleTestsPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNoYForAMEL : public SmartNotice {

PERSISTENT_DECLARATION (smNoYForAMEL)

public:
	smNoYForAMEL ();
	smNoYForAMEL (const smNoYForAMEL& note);
	virtual ~smNoYForAMEL ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLocusIsYLinked : public SmartNotice {

PERSISTENT_DECLARATION (smLocusIsYLinked)

public:
	smLocusIsYLinked ();
	smLocusIsYLinked (const smLocusIsYLinked& note);
	virtual ~smLocusIsYLinked ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNumberAllelesBelowExpectation : public SmartNotice {

PERSISTENT_DECLARATION (smNumberAllelesBelowExpectation)

public:
	smNumberAllelesBelowExpectation ();
	smNumberAllelesBelowExpectation (const smNumberAllelesBelowExpectation& note);
	virtual ~smNumberAllelesBelowExpectation ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBaselineRelativeStutter : public SmartNotice {

PERSISTENT_DECLARATION (smBaselineRelativeStutter)

public:
	smBaselineRelativeStutter ();
	smBaselineRelativeStutter (const smBaselineRelativeStutter& note);
	virtual ~smBaselineRelativeStutter ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBaselineRelativeAdenylation : public SmartNotice {

PERSISTENT_DECLARATION (smBaselineRelativeAdenylation)

public:
	smBaselineRelativeAdenylation ();
	smBaselineRelativeAdenylation (const smBaselineRelativeAdenylation& note);
	virtual ~smBaselineRelativeAdenylation ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBaselineRelativeBelowMinRFU : public SmartNotice {

PERSISTENT_DECLARATION (smBaselineRelativeBelowMinRFU)

public:
	smBaselineRelativeBelowMinRFU ();
	smBaselineRelativeBelowMinRFU (const smBaselineRelativeBelowMinRFU& note);
	virtual ~smBaselineRelativeBelowMinRFU ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBaselineRelativeBelowMinInterlocusRFU : public SmartNotice {

PERSISTENT_DECLARATION (smBaselineRelativeBelowMinInterlocusRFU)

public:
	smBaselineRelativeBelowMinInterlocusRFU ();
	smBaselineRelativeBelowMinInterlocusRFU (const smBaselineRelativeBelowMinInterlocusRFU& note);
	virtual ~smBaselineRelativeBelowMinInterlocusRFU ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smTestRelativeBaselinePreset : public SmartNotice {

PERSISTENT_DECLARATION (smTestRelativeBaselinePreset)

public:
	smTestRelativeBaselinePreset ();
	smTestRelativeBaselinePreset (const smTestRelativeBaselinePreset& note);
	virtual ~smTestRelativeBaselinePreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smIgnoreNegativeRelativeBaselinePreset : public SmartNotice {

PERSISTENT_DECLARATION (smIgnoreNegativeRelativeBaselinePreset)

public:
	smIgnoreNegativeRelativeBaselinePreset ();
	smIgnoreNegativeRelativeBaselinePreset (const smIgnoreNegativeRelativeBaselinePreset& note);
	virtual ~smIgnoreNegativeRelativeBaselinePreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNormalizeRawDataRelativeToBaselinePreset : public SmartNotice {

PERSISTENT_DECLARATION (smNormalizeRawDataRelativeToBaselinePreset)

public:
	smNormalizeRawDataRelativeToBaselinePreset ();
	smNormalizeRawDataRelativeToBaselinePreset (const smNormalizeRawDataRelativeToBaselinePreset& note);
	virtual ~smNormalizeRawDataRelativeToBaselinePreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPercentOfStandardNoiseThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smPercentOfStandardNoiseThreshold)

public:
	smPercentOfStandardNoiseThreshold ();
	smPercentOfStandardNoiseThreshold (const smPercentOfStandardNoiseThreshold& note);
	virtual ~smPercentOfStandardNoiseThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLadderFitThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smLadderFitThreshold)

public:
	smLadderFitThreshold ();
	smLadderFitThreshold (const smLadderFitThreshold& note);
	virtual ~smLadderFitThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSampleToLadderFitBelowExpectations : public SmartNotice {

PERSISTENT_DECLARATION (smSampleToLadderFitBelowExpectations)

public:
	smSampleToLadderFitBelowExpectations ();
	smSampleToLadderFitBelowExpectations (const smSampleToLadderFitBelowExpectations& note);
	virtual ~smSampleToLadderFitBelowExpectations ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smEnableRawDataFilterForNormalizationPreset : public SmartNotice {

PERSISTENT_DECLARATION (smEnableRawDataFilterForNormalizationPreset)

public:
	smEnableRawDataFilterForNormalizationPreset ();
	smEnableRawDataFilterForNormalizationPreset (const smEnableRawDataFilterForNormalizationPreset& note);
	virtual ~smEnableRawDataFilterForNormalizationPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smBaselineEstimationThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smBaselineEstimationThreshold)

public:
	smBaselineEstimationThreshold ();
	smBaselineEstimationThreshold (const smBaselineEstimationThreshold& note);
	virtual ~smBaselineEstimationThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSinglePassFilterWindowWidth : public SmartNotice {

PERSISTENT_DECLARATION (smSinglePassFilterWindowWidth)

public:
	smSinglePassFilterWindowWidth ();
	smSinglePassFilterWindowWidth (const smSinglePassFilterWindowWidth& note);
	virtual ~smSinglePassFilterWindowWidth ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAmbiguousInterlocusWithPoorMorphology : public SmartNotice {

PERSISTENT_DECLARATION (smAmbiguousInterlocusWithPoorMorphology)

public:
	smAmbiguousInterlocusWithPoorMorphology ();
	smAmbiguousInterlocusWithPoorMorphology (const smAmbiguousInterlocusWithPoorMorphology& note);
	virtual ~smAmbiguousInterlocusWithPoorMorphology ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSigmoidalPullup : public SmartNotice {

PERSISTENT_DECLARATION (smSigmoidalPullup)

public:
	smSigmoidalPullup ();
	smSigmoidalPullup (const smSigmoidalPullup& note);
	virtual ~smSigmoidalPullup ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smMinPrimaryPullupThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smMinPrimaryPullupThreshold)

public:
	smMinPrimaryPullupThreshold ();
	smMinPrimaryPullupThreshold (const smMinPrimaryPullupThreshold& note);
	virtual ~smMinPrimaryPullupThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smExcessiveResidualDisplacement : public SmartNotice {

PERSISTENT_DECLARATION (smExcessiveResidualDisplacement)

public:
	smExcessiveResidualDisplacement ();
	smExcessiveResidualDisplacement (const smExcessiveResidualDisplacement& note);
	virtual ~smExcessiveResidualDisplacement ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smResidualDisplacementThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smResidualDisplacementThreshold)

public:
	smResidualDisplacementThreshold ();
	smResidualDisplacementThreshold (const smResidualDisplacementThreshold& note);
	virtual ~smResidualDisplacementThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smResidualDisplacementTestPreset : public SmartNotice {

PERSISTENT_DECLARATION (smResidualDisplacementTestPreset)

public:
	smResidualDisplacementTestPreset ();
	smResidualDisplacementTestPreset (const smResidualDisplacementTestPreset& note);
	virtual ~smResidualDisplacementTestPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smExcessiveResidualDisplacementLeft : public SmartNotice {

PERSISTENT_DECLARATION (smExcessiveResidualDisplacementLeft)

public:
	smExcessiveResidualDisplacementLeft ();
	smExcessiveResidualDisplacementLeft (const smExcessiveResidualDisplacementLeft& note);
	virtual ~smExcessiveResidualDisplacementLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smExcessiveResidualDisplacementRight : public SmartNotice {

PERSISTENT_DECLARATION (smExcessiveResidualDisplacementRight)

public:
	smExcessiveResidualDisplacementRight ();
	smExcessiveResidualDisplacementRight (const smExcessiveResidualDisplacementRight& note);
	virtual ~smExcessiveResidualDisplacementRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSampleSatisfiesPossibleMixtureIDCriteria : public SmartNotice {

PERSISTENT_DECLARATION (smSampleSatisfiesPossibleMixtureIDCriteria)

public:
	smSampleSatisfiesPossibleMixtureIDCriteria ();
	smSampleSatisfiesPossibleMixtureIDCriteria (const smSampleSatisfiesPossibleMixtureIDCriteria& note);
	virtual ~smSampleSatisfiesPossibleMixtureIDCriteria ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smDisableLowLevelFiltersForKnownMixturesPreset : public SmartNotice {

PERSISTENT_DECLARATION (smDisableLowLevelFiltersForKnownMixturesPreset)

public:
	smDisableLowLevelFiltersForKnownMixturesPreset ();
	smDisableLowLevelFiltersForKnownMixturesPreset (const smDisableLowLevelFiltersForKnownMixturesPreset& note);
	virtual ~smDisableLowLevelFiltersForKnownMixturesPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smDisableStutterFilter : public SmartNotice {

PERSISTENT_DECLARATION (smDisableStutterFilter)

public:
	smDisableStutterFilter ();
	smDisableStutterFilter (const smDisableStutterFilter& note);
	virtual ~smDisableStutterFilter ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smDisableAdenylationFilter : public SmartNotice {

PERSISTENT_DECLARATION (smDisableAdenylationFilter)

public:
	smDisableAdenylationFilter ();
	smDisableAdenylationFilter (const smDisableAdenylationFilter& note);
	virtual ~smDisableAdenylationFilter ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSelectTriplePassVsSinglePassFilterPreset : public SmartNotice {

PERSISTENT_DECLARATION (smSelectTriplePassVsSinglePassFilterPreset)

public:
	smSelectTriplePassVsSinglePassFilterPreset ();
	smSelectTriplePassVsSinglePassFilterPreset (const smSelectTriplePassVsSinglePassFilterPreset& note);
	virtual ~smSelectTriplePassVsSinglePassFilterPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smTriplePassFilterWindowWidth : public SmartNotice {

PERSISTENT_DECLARATION (smTriplePassFilterWindowWidth)

public:
	smTriplePassFilterWindowWidth ();
	smTriplePassFilterWindowWidth (const smTriplePassFilterWindowWidth& note);
	virtual ~smTriplePassFilterWindowWidth ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLadderFitThresholdUsingMinError : public SmartNotice {

PERSISTENT_DECLARATION (smLadderFitThresholdUsingMinError)

public:
	smLadderFitThresholdUsingMinError ();
	smLadderFitThresholdUsingMinError (const smLadderFitThresholdUsingMinError& note);
	virtual ~smLadderFitThresholdUsingMinError ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smUseMaxSecondDerivativesForSampleToLadderFit : public SmartNotice {

PERSISTENT_DECLARATION (smUseMaxSecondDerivativesForSampleToLadderFit)

public:
	smUseMaxSecondDerivativesForSampleToLadderFit ();
	smUseMaxSecondDerivativesForSampleToLadderFit (const smUseMaxSecondDerivativesForSampleToLadderFit& note);
	virtual ~smUseMaxSecondDerivativesForSampleToLadderFit ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPartOfDualSignal : public SmartNotice {

PERSISTENT_DECLARATION (smPartOfDualSignal)

public:
	smPartOfDualSignal ();
	smPartOfDualSignal (const smPartOfDualSignal& note);
	virtual ~smPartOfDualSignal ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNegativePeak : public SmartNotice {

PERSISTENT_DECLARATION (smNegativePeak)

public:
	smNegativePeak ();
	smNegativePeak (const smNegativePeak& note);
	virtual ~smNegativePeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCallOnLadderAdenylationPreset : public SmartNotice {

PERSISTENT_DECLARATION (smCallOnLadderAdenylationPreset)

public:
	smCallOnLadderAdenylationPreset ();
	smCallOnLadderAdenylationPreset (const smCallOnLadderAdenylationPreset& note);
	virtual ~smCallOnLadderAdenylationPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSignalIsIntegralMultipleOfRepeatLeft : public SmartNotice {

PERSISTENT_DECLARATION (smSignalIsIntegralMultipleOfRepeatLeft)

public:
	smSignalIsIntegralMultipleOfRepeatLeft ();
	smSignalIsIntegralMultipleOfRepeatLeft (const smSignalIsIntegralMultipleOfRepeatLeft& note);
	virtual ~smSignalIsIntegralMultipleOfRepeatLeft ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smSignalIsIntegralMultipleOfRepeatRight : public SmartNotice {

PERSISTENT_DECLARATION (smSignalIsIntegralMultipleOfRepeatRight)

public:
	smSignalIsIntegralMultipleOfRepeatRight ();
	smSignalIsIntegralMultipleOfRepeatRight (const smSignalIsIntegralMultipleOfRepeatRight& note);
	virtual ~smSignalIsIntegralMultipleOfRepeatRight ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSFilterLeftShoulderPeaksPreset : public SmartNotice {

PERSISTENT_DECLARATION (smILSFilterLeftShoulderPeaksPreset)

public:
	smILSFilterLeftShoulderPeaksPreset ();
	smILSFilterLeftShoulderPeaksPreset (const smILSFilterLeftShoulderPeaksPreset& note);
	virtual ~smILSFilterLeftShoulderPeaksPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSShoulderPeakProximityThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smILSShoulderPeakProximityThreshold)

public:
	smILSShoulderPeakProximityThreshold ();
	smILSShoulderPeakProximityThreshold (const smILSShoulderPeakProximityThreshold& note);
	virtual ~smILSShoulderPeakProximityThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSShoulderFilterPercentThreshold : public SmartNotice {

PERSISTENT_DECLARATION (smILSShoulderFilterPercentThreshold)

public:
	smILSShoulderFilterPercentThreshold ();
	smILSShoulderFilterPercentThreshold (const smILSShoulderFilterPercentThreshold& note);
	virtual ~smILSShoulderFilterPercentThreshold ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smILSShoulderPeak : public SmartNotice {

PERSISTENT_DECLARATION (smILSShoulderPeak)

public:
	smILSShoulderPeak ();
	smILSShoulderPeak (const smILSShoulderPeak& note);
	virtual ~smILSShoulderPeak ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLadderPeakOutsideCoreLadder : public SmartNotice {

PERSISTENT_DECLARATION (smLadderPeakOutsideCoreLadder)

public:
	smLadderPeakOutsideCoreLadder ();
	smLadderPeakOutsideCoreLadder (const smLadderPeakOutsideCoreLadder& note);
	virtual ~smLadderPeakOutsideCoreLadder ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset : public SmartNotice {

PERSISTENT_DECLARATION (smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset)

public:
	smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset ();
	smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset (const smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset& note);
	virtual ~smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset : public SmartNotice {

PERSISTENT_DECLARATION (smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset)

public:
	smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset ();
	smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset (const smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset& note);
	virtual ~smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smExtendLociEdgeToEdgePreset : public SmartNotice {

PERSISTENT_DECLARATION (smExtendLociEdgeToEdgePreset)

public:
	smExtendLociEdgeToEdgePreset ();
	smExtendLociEdgeToEdgePreset (const smExtendLociEdgeToEdgePreset& note);
	virtual ~smExtendLociEdgeToEdgePreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset : public SmartNotice {

PERSISTENT_DECLARATION (smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset)

public:
	smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset ();
	smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset (const smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset& note);
	virtual ~smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smMaxILSBPForExtendedLocus : public SmartNotice {

PERSISTENT_DECLARATION (smMaxILSBPForExtendedLocus)

public:
	smMaxILSBPForExtendedLocus ();
	smMaxILSBPForExtendedLocus (const smMaxILSBPForExtendedLocus& note);
	virtual ~smMaxILSBPForExtendedLocus ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smIsQualityLocus : public SmartNotice {

PERSISTENT_DECLARATION (smIsQualityLocus)

public:
	smIsQualityLocus ();
	smIsQualityLocus (const smIsQualityLocus& note);
	virtual ~smIsQualityLocus ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smScaleILSPrimerSearchPreset : public SmartNotice {

PERSISTENT_DECLARATION (smScaleILSPrimerSearchPreset)

public:
	smScaleILSPrimerSearchPreset ();
	smScaleILSPrimerSearchPreset (const smScaleILSPrimerSearchPreset& note);
	virtual ~smScaleILSPrimerSearchPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNumberOfLastILSPeaksInScale : public SmartNotice {

PERSISTENT_DECLARATION (smNumberOfLastILSPeaksInScale)

public:
	smNumberOfLastILSPeaksInScale ();
	smNumberOfLastILSPeaksInScale (const smNumberOfLastILSPeaksInScale& note);
	virtual ~smNumberOfLastILSPeaksInScale ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPercentLastILSPeaksInScaling : public SmartNotice {

PERSISTENT_DECLARATION (smPercentLastILSPeaksInScaling)

public:
	smPercentLastILSPeaksInScaling ();
	smPercentLastILSPeaksInScaling (const smPercentLastILSPeaksInScaling& note);
	virtual ~smPercentLastILSPeaksInScaling ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smLaserOffScale : public SmartNotice {

PERSISTENT_DECLARATION (smLaserOffScale)

public:
	smLaserOffScale ();
	smLaserOffScale (const smLaserOffScale& note);
	virtual ~smLaserOffScale ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCalculatedPurePullup : public SmartNotice {

PERSISTENT_DECLARATION (smCalculatedPurePullup)

public:
	smCalculatedPurePullup ();
	smCalculatedPurePullup (const smCalculatedPurePullup& note);
	virtual ~smCalculatedPurePullup ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPartialPullupBelowMinRFU : public SmartNotice {

PERSISTENT_DECLARATION (smPartialPullupBelowMinRFU)

public:
	smPartialPullupBelowMinRFU ();
	smPartialPullupBelowMinRFU (const smPartialPullupBelowMinRFU& note);
	virtual ~smPartialPullupBelowMinRFU ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCallStutterPeaksPreset : public SmartNotice {

PERSISTENT_DECLARATION (smCallStutterPeaksPreset)

public:
	smCallStutterPeaksPreset ();
	smCallStutterPeaksPreset (const smCallStutterPeaksPreset& note);
	virtual ~smCallStutterPeaksPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smDoNotCallStutterPeaksForSingleSourceSamplesPreset : public SmartNotice {

PERSISTENT_DECLARATION (smDoNotCallStutterPeaksForSingleSourceSamplesPreset)

public:
	smDoNotCallStutterPeaksForSingleSourceSamplesPreset ();
	smDoNotCallStutterPeaksForSingleSourceSamplesPreset (const smDoNotCallStutterPeaksForSingleSourceSamplesPreset& note);
	virtual ~smDoNotCallStutterPeaksForSingleSourceSamplesPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset : public SmartNotice {

PERSISTENT_DECLARATION (smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset)

public:
	smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset ();
	smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset (const smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset& note);
	virtual ~smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smDoNotCallAdenylationPeaksForSingleSourceSamplesPreset : public SmartNotice {

PERSISTENT_DECLARATION (smDoNotCallAdenylationPeaksForSingleSourceSamplesPreset)

public:
	smDoNotCallAdenylationPeaksForSingleSourceSamplesPreset ();
	smDoNotCallAdenylationPeaksForSingleSourceSamplesPreset (const smDoNotCallAdenylationPeaksForSingleSourceSamplesPreset& note);
	virtual ~smDoNotCallAdenylationPeaksForSingleSourceSamplesPreset ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPlusStutter : public SmartNotice {

PERSISTENT_DECLARATION (smPlusStutter)

public:
	smPlusStutter ();
	smPlusStutter (const smPlusStutter& note);
	virtual ~smPlusStutter ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smNonStandardStutter : public SmartNotice {

PERSISTENT_DECLARATION (smNonStandardStutter)

public:
	smNonStandardStutter ();
	smNonStandardStutter (const smNonStandardStutter& note);
	virtual ~smNonStandardStutter ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};




class smPlusNonStandardStutter : public SmartNotice {

PERSISTENT_DECLARATION (smPlusNonStandardStutter)

public:
	smPlusNonStandardStutter ();
	smPlusNonStandardStutter (const smPlusNonStandardStutter& note);
	virtual ~smPlusNonStandardStutter ();

	virtual int GetSubject () const;

	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;
	virtual RGString GetName () const;

	static int LoadType ();

protected:
	static RGString sName;
	static int sSubject;
	static int sMessageIndex;
	static int sMessageScope;
};

#endif /* _STRSMARTNOTICES_H_ */

