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
*  FileName: STRNotices.h
*  Author:   Robert Goor
*
*/
//
// These are all subclasses of class Notice, used for STR Osiris.  Each one corresponds
// to a particular issue or situation
//

#ifndef _STRNOTICES_H_
#define _STRNOTICES_H_

#include "Notice.h"
#include "rgpersist.h"
#include "rgdlist.h"
#include "rgstring.h"


PERSISTENT_PREDECLARATION (SampleReqReview)
PERSISTENT_PREDECLARATION (PosCtrlReqReview)
PERSISTENT_PREDECLARATION (NegCtrlReqReview)
PERSISTENT_PREDECLARATION (ILSReqReview)
PERSISTENT_PREDECLARATION (LadderReqReview)
PERSISTENT_PREDECLARATION (ILSSpacing)
PERSISTENT_PREDECLARATION (LocusSpacing)
PERSISTENT_PREDECLARATION (NoLadderFile)
PERSISTENT_PREDECLARATION (LadderTypeUnknown)
PERSISTENT_PREDECLARATION (ReagentKitUnknown)
PERSISTENT_PREDECLARATION (TooManyReagentKits)
PERSISTENT_PREDECLARATION (NoPosCtrl)
PERSISTENT_PREDECLARATION (NoNegCtrl)
PERSISTENT_PREDECLARATION (NoSample)
PERSISTENT_PREDECLARATION (CurveFitTooLow)
PERSISTENT_PREDECLARATION (CurveFitMarginal)
PERSISTENT_PREDECLARATION (ArtifactFound)
PERSISTENT_PREDECLARATION (SpikeFound)
PERSISTENT_PREDECLARATION (StutterFound)
PERSISTENT_PREDECLARATION (AdenylationFound)
PERSISTENT_PREDECLARATION (BlobFound)
PERSISTENT_PREDECLARATION (PullUpFound)
PERSISTENT_PREDECLARATION (NoGenotypeFound)
PERSISTENT_PREDECLARATION (ThreeAllelesFound)
PERSISTENT_PREDECLARATION (UnexpectedNumberOfPeaks)
PERSISTENT_PREDECLARATION (InterLocusPeak)
PERSISTENT_PREDECLARATION (HeteroImbalanceFound)
PERSISTENT_PREDECLARATION (OLAlleleFound)
PERSISTENT_PREDECLARATION (NotOKHomozygoteFound)
PERSISTENT_PREDECLARATION (OsirisNotEqualABI)
PERSISTENT_PREDECLARATION (ILSHasTooFewPeaks)
PERSISTENT_PREDECLARATION (LocusHasTooFewPeaks)
PERSISTENT_PREDECLARATION (DirectoryInfoInsufficient)
PERSISTENT_PREDECLARATION (CouldNotReadFSA)
PERSISTENT_PREDECLARATION (NoInputDirectory)
PERSISTENT_PREDECLARATION (CouldNotOpenOutputFile)
PERSISTENT_PREDECLARATION (LadderDataFormatUnknown)
PERSISTENT_PREDECLARATION (PervasiveStutter)
PERSISTENT_PREDECLARATION (PeakOutsideILSRange)
PERSISTENT_PREDECLARATION (InterLocusLadderPeak)
PERSISTENT_PREDECLARATION (NoNamedMarkerSet)
PERSISTENT_PREDECLARATION (NoNamedLaneStandard)
PERSISTENT_PREDECLARATION (SignalTooLowForLocus)
PERSISTENT_PREDECLARATION (SignalPeakBelowMinRFU)
PERSISTENT_PREDECLARATION (PeakOutOfPlaceInILS)
PERSISTENT_PREDECLARATION (PeakOutOfPlaceInLocus)
PERSISTENT_PREDECLARATION (PullUpPrimaryChannel)
PERSISTENT_PREDECLARATION (AllelePullUp)
PERSISTENT_PREDECLARATION (HeteroImbalancePullUp)
PERSISTENT_PREDECLARATION (LowTriAllelePullUp)
PERSISTENT_PREDECLARATION (LowOLAllelePullUp)
PERSISTENT_PREDECLARATION (PeakMisorderPullUp)
PERSISTENT_PREDECLARATION (IgnoreLowOLAlleles)
PERSISTENT_PREDECLARATION (PeakMisorder)
PERSISTENT_PREDECLARATION (ControlPeakPullUp)
PERSISTENT_PREDECLARATION (OLBetweenLociPullUp)
PERSISTENT_PREDECLARATION (IgnoreLowOnLadderAlleles)
PERSISTENT_PREDECLARATION (CraterInducedByPullUp)
PERSISTENT_PREDECLARATION (CraterInducedByPullUpBetweenLoci)
PERSISTENT_PREDECLARATION (NumberOfPullUpsInSample)
PERSISTENT_PREDECLARATION (NumberOfAdenylationsInSample)
PERSISTENT_PREDECLARATION (NumberOfStutterPeaksInSample)
PERSISTENT_PREDECLARATION (NumberOfPrimaryPullUpsInSample)
PERSISTENT_PREDECLARATION (NumberOfOLAllelesWithinLociInSample)
PERSISTENT_PREDECLARATION (NumberOfStutterPeaksExceedsThreshold)
PERSISTENT_PREDECLARATION (NumberOfAdenylationPeaksExceedsThreshold)
PERSISTENT_PREDECLARATION (NumberOfPullUpPeaksExceedsThreshold)
PERSISTENT_PREDECLARATION (NumberOfOLAllelesExceedsThreshold)
PERSISTENT_PREDECLARATION (NumberOfPrimaryPullUpPeaksExceedsThreshold)
PERSISTENT_PREDECLARATION (RemovablePrimaryPullUp)
PERSISTENT_PREDECLARATION (Crater)
PERSISTENT_PREDECLARATION (CraterBetweenLoci)
PERSISTENT_PREDECLARATION (PoorPeakMorphology)
PERSISTENT_PREDECLARATION (PoorLocusMorphology)
PERSISTENT_PREDECLARATION (PossibleUnreportedOLAlleles)
PERSISTENT_PREDECLARATION (XPeakMissingFromAmelogenin)
PERSISTENT_PREDECLARATION (ExtraneousPositiveControlPeak)
PERSISTENT_PREDECLARATION (NoPositiveControlInDirectory)
PERSISTENT_PREDECLARATION (NoNegativeControlInDirectory)
PERSISTENT_PREDECLARATION (NamedPositiveControlNotFound)
PERSISTENT_PREDECLARATION (PositiveCOntrolLocusNotFound)
PERSISTENT_PREDECLARATION (SignalPeakAboveMaxRFU)
PERSISTENT_PREDECLARATION (OneOrMorePeaksAboveMaxRFU)
PERSISTENT_PREDECLARATION (ResidualExceedsThreshold)
PERSISTENT_PREDECLARATION (LocusContainsPeaksWithExcessiveResiduals)
PERSISTENT_PREDECLARATION (SetupErrorNumberPeaksUnavailableForLocus)
PERSISTENT_PREDECLARATION (PossibleExtraLocusAllele)
PERSISTENT_PREDECLARATION (OsirisBelowMinRFUButRawDataOK)
PERSISTENT_PREDECLARATION (RawDataBelowMinRFUButOsirisOK)
PERSISTENT_PREDECLARATION (OsirisAboveMaxRFUButRawDataOK)
PERSISTENT_PREDECLARATION (RawDataAboveMaxRFUButOsirisOK)
PERSISTENT_PREDECLARATION (LocusHasAllelesWithCriticalNotices)
PERSISTENT_PREDECLARATION (ILSHasPeaksWithCriticalNotices)
PERSISTENT_PREDECLARATION (CannotSeparateILSInitialFragments)
PERSISTENT_PREDECLARATION (ILSNeighborFiltersRemoveTooManyPeaks)
PERSISTENT_PREDECLARATION (ILSHeightFiltersRemoveTooManyPeaks)
PERSISTENT_PREDECLARATION (SignalPeakBelowFractionalFilterHeight)
PERSISTENT_PREDECLARATION (ILSRelativeHeightsInconsistent)
PERSISTENT_PREDECLARATION (ExtraneousPeakInAMEL)
PERSISTENT_PREDECLARATION (OLPeakInAMEL)
PERSISTENT_PREDECLARATION (NoPrimerPeaksInNegativeControlChannel)
PERSISTENT_PREDECLARATION (NoPrimerPeaksInOneOrMoreNegControlChannel)
PERSISTENT_PREDECLARATION (HeightImbalance)
PERSISTENT_PREDECLARATION (InterlocusPeakToRightOfLocus)
PERSISTENT_PREDECLARATION (InterlocusPeakToLeftOfLocus)
PERSISTENT_PREDECLARATION (SignalPeakBelowFractionalFilterHeightPlus)
PERSISTENT_PREDECLARATION (LadderRelativeHeightsInconsistent)
PERSISTENT_PREDECLARATION (AmbiguousInterlocusCraterAnalysis)
PERSISTENT_PREDECLARATION (PeakBelowPullupFractionalFilter)
PERSISTENT_PREDECLARATION (PeakBelowPullupFractionalFilterLeft)
PERSISTENT_PREDECLARATION (PeakBelowPullupFractionalFilterRight)
PERSISTENT_PREDECLARATION (PeakBelowFractionalFilterLeft)
PERSISTENT_PREDECLARATION (PeakBelowFractionalFilterRight)
PERSISTENT_PREDECLARATION (PossibleAmbiguousInterlocusPeakAssignedToLocus)
PERSISTENT_PREDECLARATION (AmbiguousInterlocusPeak)
PERSISTENT_PREDECLARATION (ExtraneousPeakInNegativeControl)
PERSISTENT_PREDECLARATION (ExtraneousPeakInLadderLocus)
PERSISTENT_PREDECLARATION (NumberOfExcessiveResidualsExceedsThreshold)
PERSISTENT_PREDECLARATION (NumberOfExcessiveResidualsInSample)
PERSISTENT_PREDECLARATION (InterlocusPeakBelowInterlocusMinRFU)

class SampleReqReview : public Notice {

PERSISTENT_DECLARATION (SampleReqReview)

public:
	SampleReqReview ();
	SampleReqReview (const SampleReqReview& note);
	virtual ~SampleReqReview ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PosCtrlReqReview : public Notice {

PERSISTENT_DECLARATION (PosCtrlReqReview)

public:
	PosCtrlReqReview ();
	PosCtrlReqReview (const PosCtrlReqReview& note);
	virtual ~PosCtrlReqReview ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NegCtrlReqReview : public Notice {

PERSISTENT_DECLARATION (NegCtrlReqReview)

public:
	NegCtrlReqReview ();
	NegCtrlReqReview (const NegCtrlReqReview& note);
	virtual ~NegCtrlReqReview ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ILSReqReview : public Notice {

PERSISTENT_DECLARATION (ILSReqReview)

public:
	ILSReqReview ();
	ILSReqReview (const ILSReqReview& note);
	virtual ~ILSReqReview ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LadderReqReview : public Notice {

PERSISTENT_DECLARATION (LadderReqReview)

public:
	LadderReqReview ();
	LadderReqReview (const LadderReqReview& note);
	virtual ~LadderReqReview ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ILSSpacing : public Notice {

PERSISTENT_DECLARATION (ILSSpacing)

public:
	ILSSpacing ();
	ILSSpacing (const ILSSpacing& note);
	virtual ~ILSSpacing ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LocusSpacing : public Notice {

PERSISTENT_DECLARATION (LocusSpacing)

public:
	LocusSpacing ();
	LocusSpacing (const LocusSpacing& note);
	virtual ~LocusSpacing ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoLadderFile : public Notice {

PERSISTENT_DECLARATION (NoLadderFile)

public:
	NoLadderFile ();
	NoLadderFile (const NoLadderFile& note);
	virtual ~NoLadderFile ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LadderTypeUnknown : public Notice {

PERSISTENT_DECLARATION (LadderTypeUnknown)

public:
	LadderTypeUnknown ();
	LadderTypeUnknown (const LadderTypeUnknown& note);
	virtual ~LadderTypeUnknown ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ReagentKitUnknown : public Notice {

PERSISTENT_DECLARATION (ReagentKitUnknown)

public:
	ReagentKitUnknown ();
	ReagentKitUnknown (const ReagentKitUnknown& note);
	virtual ~ReagentKitUnknown ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class TooManyReagentKits : public Notice {

PERSISTENT_DECLARATION (TooManyReagentKits)

public:
	TooManyReagentKits ();
	TooManyReagentKits (const TooManyReagentKits& note);
	virtual ~TooManyReagentKits ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoPosCtrl : public Notice {

PERSISTENT_DECLARATION (NoPosCtrl)

public:
	NoPosCtrl ();
	NoPosCtrl (const NoPosCtrl& note);
	virtual ~NoPosCtrl ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoNegCtrl : public Notice {

PERSISTENT_DECLARATION (NoNegCtrl)

public:
	NoNegCtrl ();
	NoNegCtrl (const NoNegCtrl& note);
	virtual ~NoNegCtrl ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoSample : public Notice {

PERSISTENT_DECLARATION (NoSample)

public:
	NoSample ();
	NoSample (const NoSample& note);
	virtual ~NoSample ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class CurveFitTooLow : public Notice {

PERSISTENT_DECLARATION (CurveFitTooLow)

public:
	CurveFitTooLow ();
	CurveFitTooLow (const CurveFitTooLow& note);
	virtual ~CurveFitTooLow ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class CurveFitMarginal : public Notice {

PERSISTENT_DECLARATION (CurveFitMarginal)

public:
	CurveFitMarginal ();
	CurveFitMarginal (const CurveFitMarginal& note);
	virtual ~CurveFitMarginal ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ArtifactFound : public Notice {

PERSISTENT_DECLARATION (ArtifactFound)

public:
	ArtifactFound ();
	ArtifactFound (const ArtifactFound& note);
	virtual ~ArtifactFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class SpikeFound : public Notice {

PERSISTENT_DECLARATION (SpikeFound)

public:
	SpikeFound ();
	SpikeFound (const SpikeFound& note);
	virtual ~SpikeFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class StutterFound : public Notice {

PERSISTENT_DECLARATION (StutterFound)

public:
	StutterFound ();
	StutterFound (const StutterFound& note);
	virtual ~StutterFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class AdenylationFound : public Notice {

PERSISTENT_DECLARATION (AdenylationFound)

public:
	AdenylationFound ();
	AdenylationFound (const AdenylationFound& note);
	virtual ~AdenylationFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class BlobFound : public Notice {

PERSISTENT_DECLARATION (BlobFound)

public:
	BlobFound ();
	BlobFound (const BlobFound& note);
	virtual ~BlobFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PullUpFound : public Notice {

PERSISTENT_DECLARATION (PullUpFound)

public:
	PullUpFound ();
	PullUpFound (const PullUpFound& note);
	virtual ~PullUpFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoGenotypeFound : public Notice {

PERSISTENT_DECLARATION (NoGenotypeFound)

public:
	NoGenotypeFound ();
	NoGenotypeFound (const NoGenotypeFound& note);
	virtual ~NoGenotypeFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ThreeAllelesFound : public Notice {

PERSISTENT_DECLARATION (ThreeAllelesFound)

public:
	ThreeAllelesFound ();
	ThreeAllelesFound (const ThreeAllelesFound& note);
	virtual ~ThreeAllelesFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class UnexpectedNumberOfPeaks : public Notice {

PERSISTENT_DECLARATION (UnexpectedNumberOfPeaks)

public:
	UnexpectedNumberOfPeaks ();
	UnexpectedNumberOfPeaks (const UnexpectedNumberOfPeaks& note);
	virtual ~UnexpectedNumberOfPeaks ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class InterLocusPeak : public Notice {

PERSISTENT_DECLARATION (InterLocusPeak)

public:
	InterLocusPeak ();
	InterLocusPeak (const InterLocusPeak& note);
	virtual ~InterLocusPeak ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class HeteroImbalanceFound : public Notice {

PERSISTENT_DECLARATION (HeteroImbalanceFound)

public:
	HeteroImbalanceFound ();
	HeteroImbalanceFound (const HeteroImbalanceFound& note);
	virtual ~HeteroImbalanceFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class OLAlleleFound : public Notice {

PERSISTENT_DECLARATION (OLAlleleFound)

public:
	OLAlleleFound ();
	OLAlleleFound (const OLAlleleFound& note);
	virtual ~OLAlleleFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NotOKHomozygoteFound : public Notice {

PERSISTENT_DECLARATION (NotOKHomozygoteFound)

public:
	NotOKHomozygoteFound ();
	NotOKHomozygoteFound (const NotOKHomozygoteFound& note);
	virtual ~NotOKHomozygoteFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class OsirisNotEqualABI : public Notice {

PERSISTENT_DECLARATION (OsirisNotEqualABI)

public:
	OsirisNotEqualABI ();
	OsirisNotEqualABI (const OsirisNotEqualABI& note);
	virtual ~OsirisNotEqualABI ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ILSHasTooFewPeaks : public Notice {

PERSISTENT_DECLARATION (ILSHasTooFewPeaks)

public:
	ILSHasTooFewPeaks ();
	ILSHasTooFewPeaks (const ILSHasTooFewPeaks& note);
	virtual ~ILSHasTooFewPeaks ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LocusHasTooFewPeaks : public Notice {

PERSISTENT_DECLARATION (LocusHasTooFewPeaks)

public:
	LocusHasTooFewPeaks ();
	LocusHasTooFewPeaks (const LocusHasTooFewPeaks& note);
	virtual ~LocusHasTooFewPeaks ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class DirectoryInfoInsufficient : public Notice {

PERSISTENT_DECLARATION (DirectoryInfoInsufficient)

public:
	DirectoryInfoInsufficient ();
	DirectoryInfoInsufficient (const DirectoryInfoInsufficient& note);
	virtual ~DirectoryInfoInsufficient ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class CouldNotReadFSA : public Notice {

PERSISTENT_DECLARATION (CouldNotReadFSA)

public:
	CouldNotReadFSA ();
	CouldNotReadFSA (const CouldNotReadFSA& note);
	virtual ~CouldNotReadFSA ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoInputDirectory : public Notice {

PERSISTENT_DECLARATION (NoInputDirectory)

public:
	NoInputDirectory ();
	NoInputDirectory (const NoInputDirectory& note);
	virtual ~NoInputDirectory ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class CouldNotOpenOutputFile : public Notice {

PERSISTENT_DECLARATION (CouldNotOpenOutputFile)

public:
	CouldNotOpenOutputFile ();
	CouldNotOpenOutputFile (const CouldNotOpenOutputFile& note);
	virtual ~CouldNotOpenOutputFile ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LadderDataFormatUnknown : public Notice {

PERSISTENT_DECLARATION (LadderDataFormatUnknown)

public:
	LadderDataFormatUnknown ();
	LadderDataFormatUnknown (const LadderDataFormatUnknown& note);
	virtual ~LadderDataFormatUnknown ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PervasiveStutter : public Notice {

PERSISTENT_DECLARATION (PervasiveStutter)

public:
	PervasiveStutter ();
	PervasiveStutter (const PervasiveStutter& note);
	virtual ~PervasiveStutter ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakOutsideILSRange : public Notice {

PERSISTENT_DECLARATION (PeakOutsideILSRange)

public:
	PeakOutsideILSRange ();
	PeakOutsideILSRange (const PeakOutsideILSRange& note);
	virtual ~PeakOutsideILSRange ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class InterLocusLadderPeak : public Notice {

PERSISTENT_DECLARATION (InterLocusLadderPeak)

public:
	InterLocusLadderPeak ();
	InterLocusLadderPeak (const InterLocusLadderPeak& note);
	virtual ~InterLocusLadderPeak ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoNamedMarkerSet : public Notice {

PERSISTENT_DECLARATION (NoNamedMarkerSet)

public:
	NoNamedMarkerSet ();
	NoNamedMarkerSet (const NoNamedMarkerSet& note);
	virtual ~NoNamedMarkerSet ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoNamedLaneStandard : public Notice {

PERSISTENT_DECLARATION (NoNamedLaneStandard)

public:
	NoNamedLaneStandard ();
	NoNamedLaneStandard (const NoNamedLaneStandard& note);
	virtual ~NoNamedLaneStandard ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class SignalTooLowForLocus : public Notice {

PERSISTENT_DECLARATION (SignalTooLowForLocus)

public:
	SignalTooLowForLocus ();
	SignalTooLowForLocus (const SignalTooLowForLocus& note);
	virtual ~SignalTooLowForLocus ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class SignalPeakBelowMinRFU : public Notice {

PERSISTENT_DECLARATION (SignalPeakBelowMinRFU)

public:
	SignalPeakBelowMinRFU ();
	SignalPeakBelowMinRFU (const SignalPeakBelowMinRFU& note);
	virtual ~SignalPeakBelowMinRFU ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakOutOfPlaceInILS : public Notice {

PERSISTENT_DECLARATION (PeakOutOfPlaceInILS)

public:
	PeakOutOfPlaceInILS ();
	PeakOutOfPlaceInILS (const PeakOutOfPlaceInILS& note);
	virtual ~PeakOutOfPlaceInILS ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakOutOfPlaceInLocus : public Notice {

PERSISTENT_DECLARATION (PeakOutOfPlaceInLocus)

public:
	PeakOutOfPlaceInLocus ();
	PeakOutOfPlaceInLocus (const PeakOutOfPlaceInLocus& note);
	virtual ~PeakOutOfPlaceInLocus ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PullUpPrimaryChannel : public Notice {

PERSISTENT_DECLARATION (PullUpPrimaryChannel)

public:
	PullUpPrimaryChannel ();
	PullUpPrimaryChannel (const PullUpPrimaryChannel& note);
	virtual ~PullUpPrimaryChannel ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class AllelePullUp : public Notice {

PERSISTENT_DECLARATION (AllelePullUp)

public:
	AllelePullUp ();
	AllelePullUp (const AllelePullUp& note);
	virtual ~AllelePullUp ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class HeteroImbalancePullUp : public Notice {

PERSISTENT_DECLARATION (HeteroImbalancePullUp)

public:
	HeteroImbalancePullUp ();
	HeteroImbalancePullUp (const HeteroImbalancePullUp& note);
	virtual ~HeteroImbalancePullUp ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LowTriAllelePullUp : public Notice {

PERSISTENT_DECLARATION (LowTriAllelePullUp)

public:
	LowTriAllelePullUp ();
	LowTriAllelePullUp (const LowTriAllelePullUp& note);
	virtual ~LowTriAllelePullUp ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LowOLAllelePullUp : public Notice {

PERSISTENT_DECLARATION (LowOLAllelePullUp)

public:
	LowOLAllelePullUp ();
	LowOLAllelePullUp (const LowOLAllelePullUp& note);
	virtual ~LowOLAllelePullUp ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakMisorderPullUp : public Notice {

PERSISTENT_DECLARATION (PeakMisorderPullUp)

public:
	PeakMisorderPullUp ();
	PeakMisorderPullUp (const PeakMisorderPullUp& note);
	virtual ~PeakMisorderPullUp ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class IgnoreLowOLAlleles : public Notice {

PERSISTENT_DECLARATION (IgnoreLowOLAlleles)

public:
	IgnoreLowOLAlleles ();
	IgnoreLowOLAlleles (const IgnoreLowOLAlleles& note);
	virtual ~IgnoreLowOLAlleles ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakMisorder : public Notice {

PERSISTENT_DECLARATION (PeakMisorder)

public:
	PeakMisorder ();
	PeakMisorder (const PeakMisorder& note);
	virtual ~PeakMisorder ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ControlPeakPullUp : public Notice {

PERSISTENT_DECLARATION (ControlPeakPullUp)

public:
	ControlPeakPullUp ();
	ControlPeakPullUp (const ControlPeakPullUp& note);
	virtual ~ControlPeakPullUp ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class OLBetweenLociPullUp : public Notice {

PERSISTENT_DECLARATION (OLBetweenLociPullUp)

public:
	OLBetweenLociPullUp ();
	OLBetweenLociPullUp (const OLBetweenLociPullUp& note);
	virtual ~OLBetweenLociPullUp ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class IgnoreLowOnLadderAlleles : public Notice {

PERSISTENT_DECLARATION (IgnoreLowOnLadderAlleles)

public:
	IgnoreLowOnLadderAlleles ();
	IgnoreLowOnLadderAlleles (const IgnoreLowOnLadderAlleles& note);
	virtual ~IgnoreLowOnLadderAlleles ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class CraterInducedByPullUp : public Notice {

PERSISTENT_DECLARATION (CraterInducedByPullUp)

public:
	CraterInducedByPullUp ();
	CraterInducedByPullUp (const CraterInducedByPullUp& note);
	virtual ~CraterInducedByPullUp ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class CraterInducedByPullUpBetweenLoci : public Notice {

PERSISTENT_DECLARATION (CraterInducedByPullUpBetweenLoci)

public:
	CraterInducedByPullUpBetweenLoci ();
	CraterInducedByPullUpBetweenLoci (const CraterInducedByPullUpBetweenLoci& note);
	virtual ~CraterInducedByPullUpBetweenLoci ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfPullUpsInSample : public Notice {

PERSISTENT_DECLARATION (NumberOfPullUpsInSample)

public:
	NumberOfPullUpsInSample ();
	NumberOfPullUpsInSample (const NumberOfPullUpsInSample& note);
	virtual ~NumberOfPullUpsInSample ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfAdenylationsInSample : public Notice {

PERSISTENT_DECLARATION (NumberOfAdenylationsInSample)

public:
	NumberOfAdenylationsInSample ();
	NumberOfAdenylationsInSample (const NumberOfAdenylationsInSample& note);
	virtual ~NumberOfAdenylationsInSample ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfStutterPeaksInSample : public Notice {

PERSISTENT_DECLARATION (NumberOfStutterPeaksInSample)

public:
	NumberOfStutterPeaksInSample ();
	NumberOfStutterPeaksInSample (const NumberOfStutterPeaksInSample& note);
	virtual ~NumberOfStutterPeaksInSample ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfPrimaryPullUpsInSample : public Notice {

PERSISTENT_DECLARATION (NumberOfPrimaryPullUpsInSample)

public:
	NumberOfPrimaryPullUpsInSample ();
	NumberOfPrimaryPullUpsInSample (const NumberOfPrimaryPullUpsInSample& note);
	virtual ~NumberOfPrimaryPullUpsInSample ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfOLAllelesWithinLociInSample : public Notice {

PERSISTENT_DECLARATION (NumberOfOLAllelesWithinLociInSample)

public:
	NumberOfOLAllelesWithinLociInSample ();
	NumberOfOLAllelesWithinLociInSample (const NumberOfOLAllelesWithinLociInSample& note);
	virtual ~NumberOfOLAllelesWithinLociInSample ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfStutterPeaksExceedsThreshold : public Notice {

PERSISTENT_DECLARATION (NumberOfStutterPeaksExceedsThreshold)

public:
	NumberOfStutterPeaksExceedsThreshold ();
	NumberOfStutterPeaksExceedsThreshold (const NumberOfStutterPeaksExceedsThreshold& note);
	virtual ~NumberOfStutterPeaksExceedsThreshold ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfAdenylationPeaksExceedsThreshold : public Notice {

PERSISTENT_DECLARATION (NumberOfAdenylationPeaksExceedsThreshold)

public:
	NumberOfAdenylationPeaksExceedsThreshold ();
	NumberOfAdenylationPeaksExceedsThreshold (const NumberOfAdenylationPeaksExceedsThreshold& note);
	virtual ~NumberOfAdenylationPeaksExceedsThreshold ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfPullUpPeaksExceedsThreshold : public Notice {

PERSISTENT_DECLARATION (NumberOfPullUpPeaksExceedsThreshold)

public:
	NumberOfPullUpPeaksExceedsThreshold ();
	NumberOfPullUpPeaksExceedsThreshold (const NumberOfPullUpPeaksExceedsThreshold& note);
	virtual ~NumberOfPullUpPeaksExceedsThreshold ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfOLAllelesExceedsThreshold : public Notice {

PERSISTENT_DECLARATION (NumberOfOLAllelesExceedsThreshold)

public:
	NumberOfOLAllelesExceedsThreshold ();
	NumberOfOLAllelesExceedsThreshold (const NumberOfOLAllelesExceedsThreshold& note);
	virtual ~NumberOfOLAllelesExceedsThreshold ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfPrimaryPullUpPeaksExceedsThreshold : public Notice {

PERSISTENT_DECLARATION (NumberOfPrimaryPullUpPeaksExceedsThreshold)

public:
	NumberOfPrimaryPullUpPeaksExceedsThreshold ();
	NumberOfPrimaryPullUpPeaksExceedsThreshold (const NumberOfPrimaryPullUpPeaksExceedsThreshold& note);
	virtual ~NumberOfPrimaryPullUpPeaksExceedsThreshold ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class RemovablePrimaryPullUp : public Notice {

PERSISTENT_DECLARATION (RemovablePrimaryPullUp)

public:
	RemovablePrimaryPullUp ();
	RemovablePrimaryPullUp (const RemovablePrimaryPullUp& note);
	virtual ~RemovablePrimaryPullUp ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class Crater : public Notice {

PERSISTENT_DECLARATION (Crater)

public:
	Crater ();
	Crater (const Crater& note);
	virtual ~Crater ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class CraterBetweenLoci : public Notice {

PERSISTENT_DECLARATION (CraterBetweenLoci)

public:
	CraterBetweenLoci ();
	CraterBetweenLoci (const CraterBetweenLoci& note);
	virtual ~CraterBetweenLoci ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PoorPeakMorphology : public Notice {

PERSISTENT_DECLARATION (PoorPeakMorphology)

public:
	PoorPeakMorphology ();
	PoorPeakMorphology (const PoorPeakMorphology& note);
	virtual ~PoorPeakMorphology ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PoorLocusMorphology : public Notice {

PERSISTENT_DECLARATION (PoorLocusMorphology)

public:
	PoorLocusMorphology ();
	PoorLocusMorphology (const PoorLocusMorphology& note);
	virtual ~PoorLocusMorphology ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PossibleUnreportedOLAlleles : public Notice {

PERSISTENT_DECLARATION (PossibleUnreportedOLAlleles)

public:
	PossibleUnreportedOLAlleles ();
	PossibleUnreportedOLAlleles (const PossibleUnreportedOLAlleles& note);
	virtual ~PossibleUnreportedOLAlleles ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class XPeakMissingFromAmelogenin : public Notice {

PERSISTENT_DECLARATION (XPeakMissingFromAmelogenin)

public:
	XPeakMissingFromAmelogenin ();
	XPeakMissingFromAmelogenin (const XPeakMissingFromAmelogenin& note);
	virtual ~XPeakMissingFromAmelogenin ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ExtraneousPositiveControlPeak : public Notice {

PERSISTENT_DECLARATION (ExtraneousPositiveControlPeak)

public:
	ExtraneousPositiveControlPeak ();
	ExtraneousPositiveControlPeak (const ExtraneousPositiveControlPeak& note);
	virtual ~ExtraneousPositiveControlPeak ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoPositiveControlInDirectory : public Notice {

PERSISTENT_DECLARATION (NoPositiveControlInDirectory)

public:
	NoPositiveControlInDirectory ();
	NoPositiveControlInDirectory (const NoPositiveControlInDirectory& note);
	virtual ~NoPositiveControlInDirectory ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoNegativeControlInDirectory : public Notice {

PERSISTENT_DECLARATION (NoNegativeControlInDirectory)

public:
	NoNegativeControlInDirectory ();
	NoNegativeControlInDirectory (const NoNegativeControlInDirectory& note);
	virtual ~NoNegativeControlInDirectory ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NamedPositiveControlNotFound : public Notice {

PERSISTENT_DECLARATION (NamedPositiveControlNotFound)

public:
	NamedPositiveControlNotFound ();
	NamedPositiveControlNotFound (const NamedPositiveControlNotFound& note);
	virtual ~NamedPositiveControlNotFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PositiveCOntrolLocusNotFound : public Notice {

PERSISTENT_DECLARATION (PositiveCOntrolLocusNotFound)

public:
	PositiveCOntrolLocusNotFound ();
	PositiveCOntrolLocusNotFound (const PositiveCOntrolLocusNotFound& note);
	virtual ~PositiveCOntrolLocusNotFound ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class SignalPeakAboveMaxRFU : public Notice {

PERSISTENT_DECLARATION (SignalPeakAboveMaxRFU)

public:
	SignalPeakAboveMaxRFU ();
	SignalPeakAboveMaxRFU (const SignalPeakAboveMaxRFU& note);
	virtual ~SignalPeakAboveMaxRFU ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class OneOrMorePeaksAboveMaxRFU : public Notice {

PERSISTENT_DECLARATION (OneOrMorePeaksAboveMaxRFU)

public:
	OneOrMorePeaksAboveMaxRFU ();
	OneOrMorePeaksAboveMaxRFU (const OneOrMorePeaksAboveMaxRFU& note);
	virtual ~OneOrMorePeaksAboveMaxRFU ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ResidualExceedsThreshold : public Notice {

PERSISTENT_DECLARATION (ResidualExceedsThreshold)

public:
	ResidualExceedsThreshold ();
	ResidualExceedsThreshold (const ResidualExceedsThreshold& note);
	virtual ~ResidualExceedsThreshold ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LocusContainsPeaksWithExcessiveResiduals : public Notice {

PERSISTENT_DECLARATION (LocusContainsPeaksWithExcessiveResiduals)

public:
	LocusContainsPeaksWithExcessiveResiduals ();
	LocusContainsPeaksWithExcessiveResiduals (const LocusContainsPeaksWithExcessiveResiduals& note);
	virtual ~LocusContainsPeaksWithExcessiveResiduals ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class SetupErrorNumberPeaksUnavailableForLocus : public Notice {

PERSISTENT_DECLARATION (SetupErrorNumberPeaksUnavailableForLocus)

public:
	SetupErrorNumberPeaksUnavailableForLocus ();
	SetupErrorNumberPeaksUnavailableForLocus (const SetupErrorNumberPeaksUnavailableForLocus& note);
	virtual ~SetupErrorNumberPeaksUnavailableForLocus ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PossibleExtraLocusAllele : public Notice {

PERSISTENT_DECLARATION (PossibleExtraLocusAllele)

public:
	PossibleExtraLocusAllele ();
	PossibleExtraLocusAllele (const PossibleExtraLocusAllele& note);
	virtual ~PossibleExtraLocusAllele ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class OsirisBelowMinRFUButRawDataOK : public Notice {

PERSISTENT_DECLARATION (OsirisBelowMinRFUButRawDataOK)

public:
	OsirisBelowMinRFUButRawDataOK ();
	OsirisBelowMinRFUButRawDataOK (const OsirisBelowMinRFUButRawDataOK& note);
	virtual ~OsirisBelowMinRFUButRawDataOK ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class RawDataBelowMinRFUButOsirisOK : public Notice {

PERSISTENT_DECLARATION (RawDataBelowMinRFUButOsirisOK)

public:
	RawDataBelowMinRFUButOsirisOK ();
	RawDataBelowMinRFUButOsirisOK (const RawDataBelowMinRFUButOsirisOK& note);
	virtual ~RawDataBelowMinRFUButOsirisOK ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class OsirisAboveMaxRFUButRawDataOK : public Notice {

PERSISTENT_DECLARATION (OsirisAboveMaxRFUButRawDataOK)

public:
	OsirisAboveMaxRFUButRawDataOK ();
	OsirisAboveMaxRFUButRawDataOK (const OsirisAboveMaxRFUButRawDataOK& note);
	virtual ~OsirisAboveMaxRFUButRawDataOK ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class RawDataAboveMaxRFUButOsirisOK : public Notice {

PERSISTENT_DECLARATION (RawDataAboveMaxRFUButOsirisOK)

public:
	RawDataAboveMaxRFUButOsirisOK ();
	RawDataAboveMaxRFUButOsirisOK (const RawDataAboveMaxRFUButOsirisOK& note);
	virtual ~RawDataAboveMaxRFUButOsirisOK ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LocusHasAllelesWithCriticalNotices : public Notice {

PERSISTENT_DECLARATION (LocusHasAllelesWithCriticalNotices)

public:
	LocusHasAllelesWithCriticalNotices ();
	LocusHasAllelesWithCriticalNotices (const LocusHasAllelesWithCriticalNotices& note);
	virtual ~LocusHasAllelesWithCriticalNotices ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ILSHasPeaksWithCriticalNotices : public Notice {

PERSISTENT_DECLARATION (ILSHasPeaksWithCriticalNotices)

public:
	ILSHasPeaksWithCriticalNotices ();
	ILSHasPeaksWithCriticalNotices (const ILSHasPeaksWithCriticalNotices& note);
	virtual ~ILSHasPeaksWithCriticalNotices ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class CannotSeparateILSInitialFragments : public Notice {

PERSISTENT_DECLARATION (CannotSeparateILSInitialFragments)

public:
	CannotSeparateILSInitialFragments ();
	CannotSeparateILSInitialFragments (const CannotSeparateILSInitialFragments& note);
	virtual ~CannotSeparateILSInitialFragments ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ILSNeighborFiltersRemoveTooManyPeaks : public Notice {

PERSISTENT_DECLARATION (ILSNeighborFiltersRemoveTooManyPeaks)

public:
	ILSNeighborFiltersRemoveTooManyPeaks ();
	ILSNeighborFiltersRemoveTooManyPeaks (const ILSNeighborFiltersRemoveTooManyPeaks& note);
	virtual ~ILSNeighborFiltersRemoveTooManyPeaks ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ILSHeightFiltersRemoveTooManyPeaks : public Notice {

PERSISTENT_DECLARATION (ILSHeightFiltersRemoveTooManyPeaks)

public:
	ILSHeightFiltersRemoveTooManyPeaks ();
	ILSHeightFiltersRemoveTooManyPeaks (const ILSHeightFiltersRemoveTooManyPeaks& note);
	virtual ~ILSHeightFiltersRemoveTooManyPeaks ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class SignalPeakBelowFractionalFilterHeight : public Notice {

PERSISTENT_DECLARATION (SignalPeakBelowFractionalFilterHeight)

public:
	SignalPeakBelowFractionalFilterHeight ();
	SignalPeakBelowFractionalFilterHeight (const SignalPeakBelowFractionalFilterHeight& note);
	virtual ~SignalPeakBelowFractionalFilterHeight ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ILSRelativeHeightsInconsistent : public Notice {

PERSISTENT_DECLARATION (ILSRelativeHeightsInconsistent)

public:
	ILSRelativeHeightsInconsistent ();
	ILSRelativeHeightsInconsistent (const ILSRelativeHeightsInconsistent& note);
	virtual ~ILSRelativeHeightsInconsistent ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ExtraneousPeakInAMEL : public Notice {

PERSISTENT_DECLARATION (ExtraneousPeakInAMEL)

public:
	ExtraneousPeakInAMEL ();
	ExtraneousPeakInAMEL (const ExtraneousPeakInAMEL& note);
	virtual ~ExtraneousPeakInAMEL ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class OLPeakInAMEL : public Notice {

PERSISTENT_DECLARATION (OLPeakInAMEL)

public:
	OLPeakInAMEL ();
	OLPeakInAMEL (const OLPeakInAMEL& note);
	virtual ~OLPeakInAMEL ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoPrimerPeaksInNegativeControlChannel : public Notice {

PERSISTENT_DECLARATION (NoPrimerPeaksInNegativeControlChannel)

public:
	NoPrimerPeaksInNegativeControlChannel ();
	NoPrimerPeaksInNegativeControlChannel (const NoPrimerPeaksInNegativeControlChannel& note);
	virtual ~NoPrimerPeaksInNegativeControlChannel ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NoPrimerPeaksInOneOrMoreNegControlChannel : public Notice {

PERSISTENT_DECLARATION (NoPrimerPeaksInOneOrMoreNegControlChannel)

public:
	NoPrimerPeaksInOneOrMoreNegControlChannel ();
	NoPrimerPeaksInOneOrMoreNegControlChannel (const NoPrimerPeaksInOneOrMoreNegControlChannel& note);
	virtual ~NoPrimerPeaksInOneOrMoreNegControlChannel ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class HeightImbalance : public Notice {

PERSISTENT_DECLARATION (HeightImbalance)

public:
	HeightImbalance ();
	HeightImbalance (const HeightImbalance& note);
	virtual ~HeightImbalance ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class InterlocusPeakToRightOfLocus : public Notice {

PERSISTENT_DECLARATION (InterlocusPeakToRightOfLocus)

public:
	InterlocusPeakToRightOfLocus ();
	InterlocusPeakToRightOfLocus (const InterlocusPeakToRightOfLocus& note);
	virtual ~InterlocusPeakToRightOfLocus ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class InterlocusPeakToLeftOfLocus : public Notice {

PERSISTENT_DECLARATION (InterlocusPeakToLeftOfLocus)

public:
	InterlocusPeakToLeftOfLocus ();
	InterlocusPeakToLeftOfLocus (const InterlocusPeakToLeftOfLocus& note);
	virtual ~InterlocusPeakToLeftOfLocus ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class SignalPeakBelowFractionalFilterHeightPlus : public Notice {

PERSISTENT_DECLARATION (SignalPeakBelowFractionalFilterHeightPlus)

public:
	SignalPeakBelowFractionalFilterHeightPlus ();
	SignalPeakBelowFractionalFilterHeightPlus (const SignalPeakBelowFractionalFilterHeightPlus& note);
	virtual ~SignalPeakBelowFractionalFilterHeightPlus ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class LadderRelativeHeightsInconsistent : public Notice {

PERSISTENT_DECLARATION (LadderRelativeHeightsInconsistent)

public:
	LadderRelativeHeightsInconsistent ();
	LadderRelativeHeightsInconsistent (const LadderRelativeHeightsInconsistent& note);
	virtual ~LadderRelativeHeightsInconsistent ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class AmbiguousInterlocusCraterAnalysis : public Notice {

PERSISTENT_DECLARATION (AmbiguousInterlocusCraterAnalysis)

public:
	AmbiguousInterlocusCraterAnalysis ();
	AmbiguousInterlocusCraterAnalysis (const AmbiguousInterlocusCraterAnalysis& note);
	virtual ~AmbiguousInterlocusCraterAnalysis ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakBelowPullupFractionalFilter : public Notice {

PERSISTENT_DECLARATION (PeakBelowPullupFractionalFilter)

public:
	PeakBelowPullupFractionalFilter ();
	PeakBelowPullupFractionalFilter (const PeakBelowPullupFractionalFilter& note);
	virtual ~PeakBelowPullupFractionalFilter ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakBelowPullupFractionalFilterLeft : public Notice {

PERSISTENT_DECLARATION (PeakBelowPullupFractionalFilterLeft)

public:
	PeakBelowPullupFractionalFilterLeft ();
	PeakBelowPullupFractionalFilterLeft (const PeakBelowPullupFractionalFilterLeft& note);
	virtual ~PeakBelowPullupFractionalFilterLeft ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakBelowPullupFractionalFilterRight : public Notice {

PERSISTENT_DECLARATION (PeakBelowPullupFractionalFilterRight)

public:
	PeakBelowPullupFractionalFilterRight ();
	PeakBelowPullupFractionalFilterRight (const PeakBelowPullupFractionalFilterRight& note);
	virtual ~PeakBelowPullupFractionalFilterRight ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakBelowFractionalFilterLeft : public Notice {

PERSISTENT_DECLARATION (PeakBelowFractionalFilterLeft)

public:
	PeakBelowFractionalFilterLeft ();
	PeakBelowFractionalFilterLeft (const PeakBelowFractionalFilterLeft& note);
	virtual ~PeakBelowFractionalFilterLeft ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PeakBelowFractionalFilterRight : public Notice {

PERSISTENT_DECLARATION (PeakBelowFractionalFilterRight)

public:
	PeakBelowFractionalFilterRight ();
	PeakBelowFractionalFilterRight (const PeakBelowFractionalFilterRight& note);
	virtual ~PeakBelowFractionalFilterRight ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class PossibleAmbiguousInterlocusPeakAssignedToLocus : public Notice {

PERSISTENT_DECLARATION (PossibleAmbiguousInterlocusPeakAssignedToLocus)

public:
	PossibleAmbiguousInterlocusPeakAssignedToLocus ();
	PossibleAmbiguousInterlocusPeakAssignedToLocus (const PossibleAmbiguousInterlocusPeakAssignedToLocus& note);
	virtual ~PossibleAmbiguousInterlocusPeakAssignedToLocus ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class AmbiguousInterlocusPeak : public Notice {

PERSISTENT_DECLARATION (AmbiguousInterlocusPeak)

public:
	AmbiguousInterlocusPeak ();
	AmbiguousInterlocusPeak (const AmbiguousInterlocusPeak& note);
	virtual ~AmbiguousInterlocusPeak ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ExtraneousPeakInNegativeControl : public Notice {

PERSISTENT_DECLARATION (ExtraneousPeakInNegativeControl)

public:
	ExtraneousPeakInNegativeControl ();
	ExtraneousPeakInNegativeControl (const ExtraneousPeakInNegativeControl& note);
	virtual ~ExtraneousPeakInNegativeControl ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class ExtraneousPeakInLadderLocus : public Notice {

PERSISTENT_DECLARATION (ExtraneousPeakInLadderLocus)

public:
	ExtraneousPeakInLadderLocus ();
	ExtraneousPeakInLadderLocus (const ExtraneousPeakInLadderLocus& note);
	virtual ~ExtraneousPeakInLadderLocus ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfExcessiveResidualsExceedsThreshold : public Notice {

PERSISTENT_DECLARATION (NumberOfExcessiveResidualsExceedsThreshold)

public:
	NumberOfExcessiveResidualsExceedsThreshold ();
	NumberOfExcessiveResidualsExceedsThreshold (const NumberOfExcessiveResidualsExceedsThreshold& note);
	virtual ~NumberOfExcessiveResidualsExceedsThreshold ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class NumberOfExcessiveResidualsInSample : public Notice {

PERSISTENT_DECLARATION (NumberOfExcessiveResidualsInSample)

public:
	NumberOfExcessiveResidualsInSample ();
	NumberOfExcessiveResidualsInSample (const NumberOfExcessiveResidualsInSample& note);
	virtual ~NumberOfExcessiveResidualsInSample ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};




class InterlocusPeakBelowInterlocusMinRFU : public Notice {

PERSISTENT_DECLARATION (InterlocusPeakBelowInterlocusMinRFU)

public:
	InterlocusPeakBelowInterlocusMinRFU ();
	InterlocusPeakBelowInterlocusMinRFU (const InterlocusPeakBelowInterlocusMinRFU& note);
	virtual ~InterlocusPeakBelowInterlocusMinRFU ();

	virtual int Initialize (NoticeBook& nBook);
	virtual int GetSubject () const;
	virtual RGString GetLabel () const;
	virtual bool IsTriggerForLowPriority () const;
	virtual bool IsDoNotCall () const;

	virtual RGString GetMessagePart (int i) const;
	virtual int GetMessageLevel () const;
	virtual int GetNumberOfMessageParts () const;
	virtual int GetSeverityLevel () const;

//	virtual RGString AssembleString (const RGString& separator);
//	virtual RGString AssembleStringOfDataOnly (const RGString& separator);
	virtual void SetIndexAndScope (int index, int scope) const;
	virtual int GetMessageIndex () const;
	virtual int GetScope () const;

	static int LoadType ();

protected:
	static RGDList sMessageList;
	static int sMessageNumber;
	static int sSubject;
	static int sMessageLevel;
	static RGString** sMessageArray;
	static RGString sLabel;
	static bool sTriggerLowPriority;
	static bool sDoNotCall;

	static int sMessageIndex;
	static int sMessageScope;
};



#endif /* _STRNOTICES_H_ */

