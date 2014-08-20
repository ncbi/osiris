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
*  FileName: STRNotices.cpp
*  Author:   Robert Goor
*
*/
//
// These are all subclasses of class Notice, used for STR Osiris.  Each one corresponds
// to a particular issue or situation
//

#include "STRNotices.h"
#include "rgtokenizer.h"




RGDList SampleReqReview::sMessageList;
int SampleReqReview::sMessageNumber = 0;
int SampleReqReview::sSubject = SampleReqReview::LoadType ();
int SampleReqReview::sMessageLevel = 1;
RGString** SampleReqReview::sMessageArray = NULL;
RGString SampleReqReview::sLabel;
bool SampleReqReview::sTriggerLowPriority = false;
bool SampleReqReview::sDoNotCall = false;

int SampleReqReview::sMessageIndex = 0;
int SampleReqReview::sMessageScope = 0;
RGDList PosCtrlReqReview::sMessageList;
int PosCtrlReqReview::sMessageNumber = 1;
int PosCtrlReqReview::sSubject = PosCtrlReqReview::LoadType ();
int PosCtrlReqReview::sMessageLevel = 1;
RGString** PosCtrlReqReview::sMessageArray = NULL;
RGString PosCtrlReqReview::sLabel;
bool PosCtrlReqReview::sTriggerLowPriority = false;
bool PosCtrlReqReview::sDoNotCall = false;

int PosCtrlReqReview::sMessageIndex = 0;
int PosCtrlReqReview::sMessageScope = 0;
RGDList NegCtrlReqReview::sMessageList;
int NegCtrlReqReview::sMessageNumber = 2;
int NegCtrlReqReview::sSubject = NegCtrlReqReview::LoadType ();
int NegCtrlReqReview::sMessageLevel = 1;
RGString** NegCtrlReqReview::sMessageArray = NULL;
RGString NegCtrlReqReview::sLabel;
bool NegCtrlReqReview::sTriggerLowPriority = false;
bool NegCtrlReqReview::sDoNotCall = false;

int NegCtrlReqReview::sMessageIndex = 0;
int NegCtrlReqReview::sMessageScope = 0;
RGDList ILSReqReview::sMessageList;
int ILSReqReview::sMessageNumber = 3;
int ILSReqReview::sSubject = ILSReqReview::LoadType ();
int ILSReqReview::sMessageLevel = 1;
RGString** ILSReqReview::sMessageArray = NULL;
RGString ILSReqReview::sLabel;
bool ILSReqReview::sTriggerLowPriority = false;
bool ILSReqReview::sDoNotCall = false;

int ILSReqReview::sMessageIndex = 0;
int ILSReqReview::sMessageScope = 0;
RGDList LadderReqReview::sMessageList;
int LadderReqReview::sMessageNumber = 4;
int LadderReqReview::sSubject = LadderReqReview::LoadType ();
int LadderReqReview::sMessageLevel = 1;
RGString** LadderReqReview::sMessageArray = NULL;
RGString LadderReqReview::sLabel;
bool LadderReqReview::sTriggerLowPriority = false;
bool LadderReqReview::sDoNotCall = false;

int LadderReqReview::sMessageIndex = 0;
int LadderReqReview::sMessageScope = 0;
RGDList ILSSpacing::sMessageList;
int ILSSpacing::sMessageNumber = 90;
int ILSSpacing::sSubject = ILSSpacing::LoadType ();
int ILSSpacing::sMessageLevel = 1;
RGString** ILSSpacing::sMessageArray = NULL;
RGString ILSSpacing::sLabel;
bool ILSSpacing::sTriggerLowPriority = false;
bool ILSSpacing::sDoNotCall = false;

int ILSSpacing::sMessageIndex = 0;
int ILSSpacing::sMessageScope = 0;
RGDList LocusSpacing::sMessageList;
int LocusSpacing::sMessageNumber = 91;
int LocusSpacing::sSubject = LocusSpacing::LoadType ();
int LocusSpacing::sMessageLevel = 1;
RGString** LocusSpacing::sMessageArray = NULL;
RGString LocusSpacing::sLabel;
bool LocusSpacing::sTriggerLowPriority = false;
bool LocusSpacing::sDoNotCall = false;

int LocusSpacing::sMessageIndex = 0;
int LocusSpacing::sMessageScope = 0;
RGDList NoLadderFile::sMessageList;
int NoLadderFile::sMessageNumber = 100;
int NoLadderFile::sSubject = NoLadderFile::LoadType ();
int NoLadderFile::sMessageLevel = 1;
RGString** NoLadderFile::sMessageArray = NULL;
RGString NoLadderFile::sLabel;
bool NoLadderFile::sTriggerLowPriority = false;
bool NoLadderFile::sDoNotCall = false;

int NoLadderFile::sMessageIndex = 0;
int NoLadderFile::sMessageScope = 0;
RGDList LadderTypeUnknown::sMessageList;
int LadderTypeUnknown::sMessageNumber = 101;
int LadderTypeUnknown::sSubject = LadderTypeUnknown::LoadType ();
int LadderTypeUnknown::sMessageLevel = 1;
RGString** LadderTypeUnknown::sMessageArray = NULL;
RGString LadderTypeUnknown::sLabel;
bool LadderTypeUnknown::sTriggerLowPriority = false;
bool LadderTypeUnknown::sDoNotCall = false;

int LadderTypeUnknown::sMessageIndex = 0;
int LadderTypeUnknown::sMessageScope = 0;
RGDList ReagentKitUnknown::sMessageList;
int ReagentKitUnknown::sMessageNumber = 104;
int ReagentKitUnknown::sSubject = ReagentKitUnknown::LoadType ();
int ReagentKitUnknown::sMessageLevel = 1;
RGString** ReagentKitUnknown::sMessageArray = NULL;
RGString ReagentKitUnknown::sLabel;
bool ReagentKitUnknown::sTriggerLowPriority = false;
bool ReagentKitUnknown::sDoNotCall = false;

int ReagentKitUnknown::sMessageIndex = 0;
int ReagentKitUnknown::sMessageScope = 0;
RGDList TooManyReagentKits::sMessageList;
int TooManyReagentKits::sMessageNumber = 105;
int TooManyReagentKits::sSubject = TooManyReagentKits::LoadType ();
int TooManyReagentKits::sMessageLevel = 1;
RGString** TooManyReagentKits::sMessageArray = NULL;
RGString TooManyReagentKits::sLabel;
bool TooManyReagentKits::sTriggerLowPriority = false;
bool TooManyReagentKits::sDoNotCall = false;

int TooManyReagentKits::sMessageIndex = 0;
int TooManyReagentKits::sMessageScope = 0;
RGDList NoPosCtrl::sMessageList;
int NoPosCtrl::sMessageNumber = 106;
int NoPosCtrl::sSubject = NoPosCtrl::LoadType ();
int NoPosCtrl::sMessageLevel = 1;
RGString** NoPosCtrl::sMessageArray = NULL;
RGString NoPosCtrl::sLabel;
bool NoPosCtrl::sTriggerLowPriority = false;
bool NoPosCtrl::sDoNotCall = false;

int NoPosCtrl::sMessageIndex = 0;
int NoPosCtrl::sMessageScope = 0;
RGDList NoNegCtrl::sMessageList;
int NoNegCtrl::sMessageNumber = 107;
int NoNegCtrl::sSubject = NoNegCtrl::LoadType ();
int NoNegCtrl::sMessageLevel = 1;
RGString** NoNegCtrl::sMessageArray = NULL;
RGString NoNegCtrl::sLabel;
bool NoNegCtrl::sTriggerLowPriority = false;
bool NoNegCtrl::sDoNotCall = false;

int NoNegCtrl::sMessageIndex = 0;
int NoNegCtrl::sMessageScope = 0;
RGDList NoSample::sMessageList;
int NoSample::sMessageNumber = 108;
int NoSample::sSubject = NoSample::LoadType ();
int NoSample::sMessageLevel = 1;
RGString** NoSample::sMessageArray = NULL;
RGString NoSample::sLabel;
bool NoSample::sTriggerLowPriority = false;
bool NoSample::sDoNotCall = false;

int NoSample::sMessageIndex = 0;
int NoSample::sMessageScope = 0;
RGDList CurveFitTooLow::sMessageList;
int CurveFitTooLow::sMessageNumber = 301;
int CurveFitTooLow::sSubject = CurveFitTooLow::LoadType ();
int CurveFitTooLow::sMessageLevel = 1;
RGString** CurveFitTooLow::sMessageArray = NULL;
RGString CurveFitTooLow::sLabel;
bool CurveFitTooLow::sTriggerLowPriority = false;
bool CurveFitTooLow::sDoNotCall = false;

int CurveFitTooLow::sMessageIndex = 0;
int CurveFitTooLow::sMessageScope = 0;
RGDList CurveFitMarginal::sMessageList;
int CurveFitMarginal::sMessageNumber = 302;
int CurveFitMarginal::sSubject = CurveFitMarginal::LoadType ();
int CurveFitMarginal::sMessageLevel = 1;
RGString** CurveFitMarginal::sMessageArray = NULL;
RGString CurveFitMarginal::sLabel;
bool CurveFitMarginal::sTriggerLowPriority = false;
bool CurveFitMarginal::sDoNotCall = false;

int CurveFitMarginal::sMessageIndex = 0;
int CurveFitMarginal::sMessageScope = 0;
RGDList ArtifactFound::sMessageList;
int ArtifactFound::sMessageNumber = 303;
int ArtifactFound::sSubject = ArtifactFound::LoadType ();
int ArtifactFound::sMessageLevel = 1;
RGString** ArtifactFound::sMessageArray = NULL;
RGString ArtifactFound::sLabel;
bool ArtifactFound::sTriggerLowPriority = false;
bool ArtifactFound::sDoNotCall = false;

int ArtifactFound::sMessageIndex = 0;
int ArtifactFound::sMessageScope = 0;
RGDList SpikeFound::sMessageList;
int SpikeFound::sMessageNumber = 304;
int SpikeFound::sSubject = SpikeFound::LoadType ();
int SpikeFound::sMessageLevel = 1;
RGString** SpikeFound::sMessageArray = NULL;
RGString SpikeFound::sLabel;
bool SpikeFound::sTriggerLowPriority = false;
bool SpikeFound::sDoNotCall = false;

int SpikeFound::sMessageIndex = 0;
int SpikeFound::sMessageScope = 0;
RGDList StutterFound::sMessageList;
int StutterFound::sMessageNumber = 305;
int StutterFound::sSubject = StutterFound::LoadType ();
int StutterFound::sMessageLevel = 1;
RGString** StutterFound::sMessageArray = NULL;
RGString StutterFound::sLabel;
bool StutterFound::sTriggerLowPriority = false;
bool StutterFound::sDoNotCall = false;

int StutterFound::sMessageIndex = 0;
int StutterFound::sMessageScope = 0;
RGDList AdenylationFound::sMessageList;
int AdenylationFound::sMessageNumber = 306;
int AdenylationFound::sSubject = AdenylationFound::LoadType ();
int AdenylationFound::sMessageLevel = 1;
RGString** AdenylationFound::sMessageArray = NULL;
RGString AdenylationFound::sLabel;
bool AdenylationFound::sTriggerLowPriority = false;
bool AdenylationFound::sDoNotCall = false;

int AdenylationFound::sMessageIndex = 0;
int AdenylationFound::sMessageScope = 0;
RGDList BlobFound::sMessageList;
int BlobFound::sMessageNumber = 307;
int BlobFound::sSubject = BlobFound::LoadType ();
int BlobFound::sMessageLevel = 1;
RGString** BlobFound::sMessageArray = NULL;
RGString BlobFound::sLabel;
bool BlobFound::sTriggerLowPriority = false;
bool BlobFound::sDoNotCall = false;

int BlobFound::sMessageIndex = 0;
int BlobFound::sMessageScope = 0;
RGDList PullUpFound::sMessageList;
int PullUpFound::sMessageNumber = 308;
int PullUpFound::sSubject = PullUpFound::LoadType ();
int PullUpFound::sMessageLevel = 1;
RGString** PullUpFound::sMessageArray = NULL;
RGString PullUpFound::sLabel;
bool PullUpFound::sTriggerLowPriority = false;
bool PullUpFound::sDoNotCall = false;

int PullUpFound::sMessageIndex = 0;
int PullUpFound::sMessageScope = 0;
RGDList NoGenotypeFound::sMessageList;
int NoGenotypeFound::sMessageNumber = 310;
int NoGenotypeFound::sSubject = NoGenotypeFound::LoadType ();
int NoGenotypeFound::sMessageLevel = 1;
RGString** NoGenotypeFound::sMessageArray = NULL;
RGString NoGenotypeFound::sLabel;
bool NoGenotypeFound::sTriggerLowPriority = false;
bool NoGenotypeFound::sDoNotCall = false;

int NoGenotypeFound::sMessageIndex = 0;
int NoGenotypeFound::sMessageScope = 0;
RGDList ThreeAllelesFound::sMessageList;
int ThreeAllelesFound::sMessageNumber = 311;
int ThreeAllelesFound::sSubject = ThreeAllelesFound::LoadType ();
int ThreeAllelesFound::sMessageLevel = 1;
RGString** ThreeAllelesFound::sMessageArray = NULL;
RGString ThreeAllelesFound::sLabel;
bool ThreeAllelesFound::sTriggerLowPriority = false;
bool ThreeAllelesFound::sDoNotCall = false;

int ThreeAllelesFound::sMessageIndex = 0;
int ThreeAllelesFound::sMessageScope = 0;
RGDList UnexpectedNumberOfPeaks::sMessageList;
int UnexpectedNumberOfPeaks::sMessageNumber = 312;
int UnexpectedNumberOfPeaks::sSubject = UnexpectedNumberOfPeaks::LoadType ();
int UnexpectedNumberOfPeaks::sMessageLevel = 1;
RGString** UnexpectedNumberOfPeaks::sMessageArray = NULL;
RGString UnexpectedNumberOfPeaks::sLabel;
bool UnexpectedNumberOfPeaks::sTriggerLowPriority = false;
bool UnexpectedNumberOfPeaks::sDoNotCall = false;

int UnexpectedNumberOfPeaks::sMessageIndex = 0;
int UnexpectedNumberOfPeaks::sMessageScope = 0;
RGDList InterLocusPeak::sMessageList;
int InterLocusPeak::sMessageNumber = 324;
int InterLocusPeak::sSubject = InterLocusPeak::LoadType ();
int InterLocusPeak::sMessageLevel = 1;
RGString** InterLocusPeak::sMessageArray = NULL;
RGString InterLocusPeak::sLabel;
bool InterLocusPeak::sTriggerLowPriority = false;
bool InterLocusPeak::sDoNotCall = false;

int InterLocusPeak::sMessageIndex = 0;
int InterLocusPeak::sMessageScope = 0;
RGDList HeteroImbalanceFound::sMessageList;
int HeteroImbalanceFound::sMessageNumber = 326;
int HeteroImbalanceFound::sSubject = HeteroImbalanceFound::LoadType ();
int HeteroImbalanceFound::sMessageLevel = 1;
RGString** HeteroImbalanceFound::sMessageArray = NULL;
RGString HeteroImbalanceFound::sLabel;
bool HeteroImbalanceFound::sTriggerLowPriority = false;
bool HeteroImbalanceFound::sDoNotCall = false;

int HeteroImbalanceFound::sMessageIndex = 0;
int HeteroImbalanceFound::sMessageScope = 0;
RGDList OLAlleleFound::sMessageList;
int OLAlleleFound::sMessageNumber = 330;
int OLAlleleFound::sSubject = OLAlleleFound::LoadType ();
int OLAlleleFound::sMessageLevel = 1;
RGString** OLAlleleFound::sMessageArray = NULL;
RGString OLAlleleFound::sLabel;
bool OLAlleleFound::sTriggerLowPriority = false;
bool OLAlleleFound::sDoNotCall = false;

int OLAlleleFound::sMessageIndex = 0;
int OLAlleleFound::sMessageScope = 0;
RGDList NotOKHomozygoteFound::sMessageList;
int NotOKHomozygoteFound::sMessageNumber = 356;
int NotOKHomozygoteFound::sSubject = NotOKHomozygoteFound::LoadType ();
int NotOKHomozygoteFound::sMessageLevel = 1;
RGString** NotOKHomozygoteFound::sMessageArray = NULL;
RGString NotOKHomozygoteFound::sLabel;
bool NotOKHomozygoteFound::sTriggerLowPriority = false;
bool NotOKHomozygoteFound::sDoNotCall = false;

int NotOKHomozygoteFound::sMessageIndex = 0;
int NotOKHomozygoteFound::sMessageScope = 0;
RGDList OsirisNotEqualABI::sMessageList;
int OsirisNotEqualABI::sMessageNumber = 359;
int OsirisNotEqualABI::sSubject = OsirisNotEqualABI::LoadType ();
int OsirisNotEqualABI::sMessageLevel = 1;
RGString** OsirisNotEqualABI::sMessageArray = NULL;
RGString OsirisNotEqualABI::sLabel;
bool OsirisNotEqualABI::sTriggerLowPriority = false;
bool OsirisNotEqualABI::sDoNotCall = false;

int OsirisNotEqualABI::sMessageIndex = 0;
int OsirisNotEqualABI::sMessageScope = 0;
RGDList ILSHasTooFewPeaks::sMessageList;
int ILSHasTooFewPeaks::sMessageNumber = 501;
int ILSHasTooFewPeaks::sSubject = ILSHasTooFewPeaks::LoadType ();
int ILSHasTooFewPeaks::sMessageLevel = 1;
RGString** ILSHasTooFewPeaks::sMessageArray = NULL;
RGString ILSHasTooFewPeaks::sLabel;
bool ILSHasTooFewPeaks::sTriggerLowPriority = false;
bool ILSHasTooFewPeaks::sDoNotCall = false;

int ILSHasTooFewPeaks::sMessageIndex = 0;
int ILSHasTooFewPeaks::sMessageScope = 0;
RGDList LocusHasTooFewPeaks::sMessageList;
int LocusHasTooFewPeaks::sMessageNumber = 502;
int LocusHasTooFewPeaks::sSubject = LocusHasTooFewPeaks::LoadType ();
int LocusHasTooFewPeaks::sMessageLevel = 1;
RGString** LocusHasTooFewPeaks::sMessageArray = NULL;
RGString LocusHasTooFewPeaks::sLabel;
bool LocusHasTooFewPeaks::sTriggerLowPriority = false;
bool LocusHasTooFewPeaks::sDoNotCall = false;

int LocusHasTooFewPeaks::sMessageIndex = 0;
int LocusHasTooFewPeaks::sMessageScope = 0;
RGDList DirectoryInfoInsufficient::sMessageList;
int DirectoryInfoInsufficient::sMessageNumber = 520;
int DirectoryInfoInsufficient::sSubject = DirectoryInfoInsufficient::LoadType ();
int DirectoryInfoInsufficient::sMessageLevel = 1;
RGString** DirectoryInfoInsufficient::sMessageArray = NULL;
RGString DirectoryInfoInsufficient::sLabel;
bool DirectoryInfoInsufficient::sTriggerLowPriority = false;
bool DirectoryInfoInsufficient::sDoNotCall = false;

int DirectoryInfoInsufficient::sMessageIndex = 0;
int DirectoryInfoInsufficient::sMessageScope = 0;
RGDList CouldNotReadFSA::sMessageList;
int CouldNotReadFSA::sMessageNumber = 701;
int CouldNotReadFSA::sSubject = CouldNotReadFSA::LoadType ();
int CouldNotReadFSA::sMessageLevel = 1;
RGString** CouldNotReadFSA::sMessageArray = NULL;
RGString CouldNotReadFSA::sLabel;
bool CouldNotReadFSA::sTriggerLowPriority = false;
bool CouldNotReadFSA::sDoNotCall = false;

int CouldNotReadFSA::sMessageIndex = 0;
int CouldNotReadFSA::sMessageScope = 0;
RGDList NoInputDirectory::sMessageList;
int NoInputDirectory::sMessageNumber = 702;
int NoInputDirectory::sSubject = NoInputDirectory::LoadType ();
int NoInputDirectory::sMessageLevel = 1;
RGString** NoInputDirectory::sMessageArray = NULL;
RGString NoInputDirectory::sLabel;
bool NoInputDirectory::sTriggerLowPriority = false;
bool NoInputDirectory::sDoNotCall = false;

int NoInputDirectory::sMessageIndex = 0;
int NoInputDirectory::sMessageScope = 0;
RGDList CouldNotOpenOutputFile::sMessageList;
int CouldNotOpenOutputFile::sMessageNumber = 703;
int CouldNotOpenOutputFile::sSubject = CouldNotOpenOutputFile::LoadType ();
int CouldNotOpenOutputFile::sMessageLevel = 1;
RGString** CouldNotOpenOutputFile::sMessageArray = NULL;
RGString CouldNotOpenOutputFile::sLabel;
bool CouldNotOpenOutputFile::sTriggerLowPriority = false;
bool CouldNotOpenOutputFile::sDoNotCall = false;

int CouldNotOpenOutputFile::sMessageIndex = 0;
int CouldNotOpenOutputFile::sMessageScope = 0;
RGDList LadderDataFormatUnknown::sMessageList;
int LadderDataFormatUnknown::sMessageNumber = 704;
int LadderDataFormatUnknown::sSubject = LadderDataFormatUnknown::LoadType ();
int LadderDataFormatUnknown::sMessageLevel = 1;
RGString** LadderDataFormatUnknown::sMessageArray = NULL;
RGString LadderDataFormatUnknown::sLabel;
bool LadderDataFormatUnknown::sTriggerLowPriority = false;
bool LadderDataFormatUnknown::sDoNotCall = false;

int LadderDataFormatUnknown::sMessageIndex = 0;
int LadderDataFormatUnknown::sMessageScope = 0;
RGDList PervasiveStutter::sMessageList;
int PervasiveStutter::sMessageNumber = 705;
int PervasiveStutter::sSubject = PervasiveStutter::LoadType ();
int PervasiveStutter::sMessageLevel = 1;
RGString** PervasiveStutter::sMessageArray = NULL;
RGString PervasiveStutter::sLabel;
bool PervasiveStutter::sTriggerLowPriority = false;
bool PervasiveStutter::sDoNotCall = false;

int PervasiveStutter::sMessageIndex = 0;
int PervasiveStutter::sMessageScope = 0;
RGDList PeakOutsideILSRange::sMessageList;
int PeakOutsideILSRange::sMessageNumber = 706;
int PeakOutsideILSRange::sSubject = PeakOutsideILSRange::LoadType ();
int PeakOutsideILSRange::sMessageLevel = 1;
RGString** PeakOutsideILSRange::sMessageArray = NULL;
RGString PeakOutsideILSRange::sLabel;
bool PeakOutsideILSRange::sTriggerLowPriority = false;
bool PeakOutsideILSRange::sDoNotCall = false;

int PeakOutsideILSRange::sMessageIndex = 0;
int PeakOutsideILSRange::sMessageScope = 0;
RGDList InterLocusLadderPeak::sMessageList;
int InterLocusLadderPeak::sMessageNumber = 707;
int InterLocusLadderPeak::sSubject = InterLocusLadderPeak::LoadType ();
int InterLocusLadderPeak::sMessageLevel = 1;
RGString** InterLocusLadderPeak::sMessageArray = NULL;
RGString InterLocusLadderPeak::sLabel;
bool InterLocusLadderPeak::sTriggerLowPriority = false;
bool InterLocusLadderPeak::sDoNotCall = false;

int InterLocusLadderPeak::sMessageIndex = 0;
int InterLocusLadderPeak::sMessageScope = 0;
RGDList NoNamedMarkerSet::sMessageList;
int NoNamedMarkerSet::sMessageNumber = 708;
int NoNamedMarkerSet::sSubject = NoNamedMarkerSet::LoadType ();
int NoNamedMarkerSet::sMessageLevel = 1;
RGString** NoNamedMarkerSet::sMessageArray = NULL;
RGString NoNamedMarkerSet::sLabel;
bool NoNamedMarkerSet::sTriggerLowPriority = false;
bool NoNamedMarkerSet::sDoNotCall = false;

int NoNamedMarkerSet::sMessageIndex = 0;
int NoNamedMarkerSet::sMessageScope = 0;
RGDList NoNamedLaneStandard::sMessageList;
int NoNamedLaneStandard::sMessageNumber = 709;
int NoNamedLaneStandard::sSubject = NoNamedLaneStandard::LoadType ();
int NoNamedLaneStandard::sMessageLevel = 1;
RGString** NoNamedLaneStandard::sMessageArray = NULL;
RGString NoNamedLaneStandard::sLabel;
bool NoNamedLaneStandard::sTriggerLowPriority = false;
bool NoNamedLaneStandard::sDoNotCall = false;

int NoNamedLaneStandard::sMessageIndex = 0;
int NoNamedLaneStandard::sMessageScope = 0;
RGDList SignalTooLowForLocus::sMessageList;
int SignalTooLowForLocus::sMessageNumber = 710;
int SignalTooLowForLocus::sSubject = SignalTooLowForLocus::LoadType ();
int SignalTooLowForLocus::sMessageLevel = 1;
RGString** SignalTooLowForLocus::sMessageArray = NULL;
RGString SignalTooLowForLocus::sLabel;
bool SignalTooLowForLocus::sTriggerLowPriority = false;
bool SignalTooLowForLocus::sDoNotCall = false;

int SignalTooLowForLocus::sMessageIndex = 0;
int SignalTooLowForLocus::sMessageScope = 0;
RGDList SignalPeakBelowMinRFU::sMessageList;
int SignalPeakBelowMinRFU::sMessageNumber = 711;
int SignalPeakBelowMinRFU::sSubject = SignalPeakBelowMinRFU::LoadType ();
int SignalPeakBelowMinRFU::sMessageLevel = 1;
RGString** SignalPeakBelowMinRFU::sMessageArray = NULL;
RGString SignalPeakBelowMinRFU::sLabel;
bool SignalPeakBelowMinRFU::sTriggerLowPriority = false;
bool SignalPeakBelowMinRFU::sDoNotCall = false;

int SignalPeakBelowMinRFU::sMessageIndex = 0;
int SignalPeakBelowMinRFU::sMessageScope = 0;
RGDList PeakOutOfPlaceInILS::sMessageList;
int PeakOutOfPlaceInILS::sMessageNumber = 712;
int PeakOutOfPlaceInILS::sSubject = PeakOutOfPlaceInILS::LoadType ();
int PeakOutOfPlaceInILS::sMessageLevel = 1;
RGString** PeakOutOfPlaceInILS::sMessageArray = NULL;
RGString PeakOutOfPlaceInILS::sLabel;
bool PeakOutOfPlaceInILS::sTriggerLowPriority = false;
bool PeakOutOfPlaceInILS::sDoNotCall = false;

int PeakOutOfPlaceInILS::sMessageIndex = 0;
int PeakOutOfPlaceInILS::sMessageScope = 0;
RGDList PeakOutOfPlaceInLocus::sMessageList;
int PeakOutOfPlaceInLocus::sMessageNumber = 713;
int PeakOutOfPlaceInLocus::sSubject = PeakOutOfPlaceInLocus::LoadType ();
int PeakOutOfPlaceInLocus::sMessageLevel = 1;
RGString** PeakOutOfPlaceInLocus::sMessageArray = NULL;
RGString PeakOutOfPlaceInLocus::sLabel;
bool PeakOutOfPlaceInLocus::sTriggerLowPriority = false;
bool PeakOutOfPlaceInLocus::sDoNotCall = false;

int PeakOutOfPlaceInLocus::sMessageIndex = 0;
int PeakOutOfPlaceInLocus::sMessageScope = 0;
RGDList PullUpPrimaryChannel::sMessageList;
int PullUpPrimaryChannel::sMessageNumber = 714;
int PullUpPrimaryChannel::sSubject = PullUpPrimaryChannel::LoadType ();
int PullUpPrimaryChannel::sMessageLevel = 1;
RGString** PullUpPrimaryChannel::sMessageArray = NULL;
RGString PullUpPrimaryChannel::sLabel;
bool PullUpPrimaryChannel::sTriggerLowPriority = false;
bool PullUpPrimaryChannel::sDoNotCall = false;

int PullUpPrimaryChannel::sMessageIndex = 0;
int PullUpPrimaryChannel::sMessageScope = 0;
RGDList AllelePullUp::sMessageList;
int AllelePullUp::sMessageNumber = 715;
int AllelePullUp::sSubject = AllelePullUp::LoadType ();
int AllelePullUp::sMessageLevel = 1;
RGString** AllelePullUp::sMessageArray = NULL;
RGString AllelePullUp::sLabel;
bool AllelePullUp::sTriggerLowPriority = false;
bool AllelePullUp::sDoNotCall = false;

int AllelePullUp::sMessageIndex = 0;
int AllelePullUp::sMessageScope = 0;
RGDList HeteroImbalancePullUp::sMessageList;
int HeteroImbalancePullUp::sMessageNumber = 716;
int HeteroImbalancePullUp::sSubject = HeteroImbalancePullUp::LoadType ();
int HeteroImbalancePullUp::sMessageLevel = 1;
RGString** HeteroImbalancePullUp::sMessageArray = NULL;
RGString HeteroImbalancePullUp::sLabel;
bool HeteroImbalancePullUp::sTriggerLowPriority = false;
bool HeteroImbalancePullUp::sDoNotCall = false;

int HeteroImbalancePullUp::sMessageIndex = 0;
int HeteroImbalancePullUp::sMessageScope = 0;
RGDList LowTriAllelePullUp::sMessageList;
int LowTriAllelePullUp::sMessageNumber = 717;
int LowTriAllelePullUp::sSubject = LowTriAllelePullUp::LoadType ();
int LowTriAllelePullUp::sMessageLevel = 1;
RGString** LowTriAllelePullUp::sMessageArray = NULL;
RGString LowTriAllelePullUp::sLabel;
bool LowTriAllelePullUp::sTriggerLowPriority = false;
bool LowTriAllelePullUp::sDoNotCall = false;

int LowTriAllelePullUp::sMessageIndex = 0;
int LowTriAllelePullUp::sMessageScope = 0;
RGDList LowOLAllelePullUp::sMessageList;
int LowOLAllelePullUp::sMessageNumber = 718;
int LowOLAllelePullUp::sSubject = LowOLAllelePullUp::LoadType ();
int LowOLAllelePullUp::sMessageLevel = 1;
RGString** LowOLAllelePullUp::sMessageArray = NULL;
RGString LowOLAllelePullUp::sLabel;
bool LowOLAllelePullUp::sTriggerLowPriority = false;
bool LowOLAllelePullUp::sDoNotCall = false;

int LowOLAllelePullUp::sMessageIndex = 0;
int LowOLAllelePullUp::sMessageScope = 0;
RGDList PeakMisorderPullUp::sMessageList;
int PeakMisorderPullUp::sMessageNumber = 719;
int PeakMisorderPullUp::sSubject = PeakMisorderPullUp::LoadType ();
int PeakMisorderPullUp::sMessageLevel = 1;
RGString** PeakMisorderPullUp::sMessageArray = NULL;
RGString PeakMisorderPullUp::sLabel;
bool PeakMisorderPullUp::sTriggerLowPriority = false;
bool PeakMisorderPullUp::sDoNotCall = false;

int PeakMisorderPullUp::sMessageIndex = 0;
int PeakMisorderPullUp::sMessageScope = 0;
RGDList IgnoreLowOLAlleles::sMessageList;
int IgnoreLowOLAlleles::sMessageNumber = 720;
int IgnoreLowOLAlleles::sSubject = IgnoreLowOLAlleles::LoadType ();
int IgnoreLowOLAlleles::sMessageLevel = 1;
RGString** IgnoreLowOLAlleles::sMessageArray = NULL;
RGString IgnoreLowOLAlleles::sLabel;
bool IgnoreLowOLAlleles::sTriggerLowPriority = false;
bool IgnoreLowOLAlleles::sDoNotCall = false;

int IgnoreLowOLAlleles::sMessageIndex = 0;
int IgnoreLowOLAlleles::sMessageScope = 0;
RGDList PeakMisorder::sMessageList;
int PeakMisorder::sMessageNumber = 721;
int PeakMisorder::sSubject = PeakMisorder::LoadType ();
int PeakMisorder::sMessageLevel = 1;
RGString** PeakMisorder::sMessageArray = NULL;
RGString PeakMisorder::sLabel;
bool PeakMisorder::sTriggerLowPriority = false;
bool PeakMisorder::sDoNotCall = false;

int PeakMisorder::sMessageIndex = 0;
int PeakMisorder::sMessageScope = 0;
RGDList ControlPeakPullUp::sMessageList;
int ControlPeakPullUp::sMessageNumber = 722;
int ControlPeakPullUp::sSubject = ControlPeakPullUp::LoadType ();
int ControlPeakPullUp::sMessageLevel = 1;
RGString** ControlPeakPullUp::sMessageArray = NULL;
RGString ControlPeakPullUp::sLabel;
bool ControlPeakPullUp::sTriggerLowPriority = false;
bool ControlPeakPullUp::sDoNotCall = false;

int ControlPeakPullUp::sMessageIndex = 0;
int ControlPeakPullUp::sMessageScope = 0;
RGDList OLBetweenLociPullUp::sMessageList;
int OLBetweenLociPullUp::sMessageNumber = 723;
int OLBetweenLociPullUp::sSubject = OLBetweenLociPullUp::LoadType ();
int OLBetweenLociPullUp::sMessageLevel = 1;
RGString** OLBetweenLociPullUp::sMessageArray = NULL;
RGString OLBetweenLociPullUp::sLabel;
bool OLBetweenLociPullUp::sTriggerLowPriority = false;
bool OLBetweenLociPullUp::sDoNotCall = false;

int OLBetweenLociPullUp::sMessageIndex = 0;
int OLBetweenLociPullUp::sMessageScope = 0;
RGDList IgnoreLowOnLadderAlleles::sMessageList;
int IgnoreLowOnLadderAlleles::sMessageNumber = 724;
int IgnoreLowOnLadderAlleles::sSubject = IgnoreLowOnLadderAlleles::LoadType ();
int IgnoreLowOnLadderAlleles::sMessageLevel = 1;
RGString** IgnoreLowOnLadderAlleles::sMessageArray = NULL;
RGString IgnoreLowOnLadderAlleles::sLabel;
bool IgnoreLowOnLadderAlleles::sTriggerLowPriority = false;
bool IgnoreLowOnLadderAlleles::sDoNotCall = false;

int IgnoreLowOnLadderAlleles::sMessageIndex = 0;
int IgnoreLowOnLadderAlleles::sMessageScope = 0;
RGDList CraterInducedByPullUp::sMessageList;
int CraterInducedByPullUp::sMessageNumber = 725;
int CraterInducedByPullUp::sSubject = CraterInducedByPullUp::LoadType ();
int CraterInducedByPullUp::sMessageLevel = 1;
RGString** CraterInducedByPullUp::sMessageArray = NULL;
RGString CraterInducedByPullUp::sLabel;
bool CraterInducedByPullUp::sTriggerLowPriority = false;
bool CraterInducedByPullUp::sDoNotCall = false;

int CraterInducedByPullUp::sMessageIndex = 0;
int CraterInducedByPullUp::sMessageScope = 0;
RGDList CraterInducedByPullUpBetweenLoci::sMessageList;
int CraterInducedByPullUpBetweenLoci::sMessageNumber = 726;
int CraterInducedByPullUpBetweenLoci::sSubject = CraterInducedByPullUpBetweenLoci::LoadType ();
int CraterInducedByPullUpBetweenLoci::sMessageLevel = 1;
RGString** CraterInducedByPullUpBetweenLoci::sMessageArray = NULL;
RGString CraterInducedByPullUpBetweenLoci::sLabel;
bool CraterInducedByPullUpBetweenLoci::sTriggerLowPriority = false;
bool CraterInducedByPullUpBetweenLoci::sDoNotCall = false;

int CraterInducedByPullUpBetweenLoci::sMessageIndex = 0;
int CraterInducedByPullUpBetweenLoci::sMessageScope = 0;
RGDList NumberOfPullUpsInSample::sMessageList;
int NumberOfPullUpsInSample::sMessageNumber = 727;
int NumberOfPullUpsInSample::sSubject = NumberOfPullUpsInSample::LoadType ();
int NumberOfPullUpsInSample::sMessageLevel = 1;
RGString** NumberOfPullUpsInSample::sMessageArray = NULL;
RGString NumberOfPullUpsInSample::sLabel;
bool NumberOfPullUpsInSample::sTriggerLowPriority = false;
bool NumberOfPullUpsInSample::sDoNotCall = false;

int NumberOfPullUpsInSample::sMessageIndex = 0;
int NumberOfPullUpsInSample::sMessageScope = 0;
RGDList NumberOfAdenylationsInSample::sMessageList;
int NumberOfAdenylationsInSample::sMessageNumber = 728;
int NumberOfAdenylationsInSample::sSubject = NumberOfAdenylationsInSample::LoadType ();
int NumberOfAdenylationsInSample::sMessageLevel = 1;
RGString** NumberOfAdenylationsInSample::sMessageArray = NULL;
RGString NumberOfAdenylationsInSample::sLabel;
bool NumberOfAdenylationsInSample::sTriggerLowPriority = false;
bool NumberOfAdenylationsInSample::sDoNotCall = false;

int NumberOfAdenylationsInSample::sMessageIndex = 0;
int NumberOfAdenylationsInSample::sMessageScope = 0;
RGDList NumberOfStutterPeaksInSample::sMessageList;
int NumberOfStutterPeaksInSample::sMessageNumber = 729;
int NumberOfStutterPeaksInSample::sSubject = NumberOfStutterPeaksInSample::LoadType ();
int NumberOfStutterPeaksInSample::sMessageLevel = 1;
RGString** NumberOfStutterPeaksInSample::sMessageArray = NULL;
RGString NumberOfStutterPeaksInSample::sLabel;
bool NumberOfStutterPeaksInSample::sTriggerLowPriority = false;
bool NumberOfStutterPeaksInSample::sDoNotCall = false;

int NumberOfStutterPeaksInSample::sMessageIndex = 0;
int NumberOfStutterPeaksInSample::sMessageScope = 0;
RGDList NumberOfPrimaryPullUpsInSample::sMessageList;
int NumberOfPrimaryPullUpsInSample::sMessageNumber = 730;
int NumberOfPrimaryPullUpsInSample::sSubject = NumberOfPrimaryPullUpsInSample::LoadType ();
int NumberOfPrimaryPullUpsInSample::sMessageLevel = 1;
RGString** NumberOfPrimaryPullUpsInSample::sMessageArray = NULL;
RGString NumberOfPrimaryPullUpsInSample::sLabel;
bool NumberOfPrimaryPullUpsInSample::sTriggerLowPriority = false;
bool NumberOfPrimaryPullUpsInSample::sDoNotCall = false;

int NumberOfPrimaryPullUpsInSample::sMessageIndex = 0;
int NumberOfPrimaryPullUpsInSample::sMessageScope = 0;
RGDList NumberOfOLAllelesWithinLociInSample::sMessageList;
int NumberOfOLAllelesWithinLociInSample::sMessageNumber = 731;
int NumberOfOLAllelesWithinLociInSample::sSubject = NumberOfOLAllelesWithinLociInSample::LoadType ();
int NumberOfOLAllelesWithinLociInSample::sMessageLevel = 1;
RGString** NumberOfOLAllelesWithinLociInSample::sMessageArray = NULL;
RGString NumberOfOLAllelesWithinLociInSample::sLabel;
bool NumberOfOLAllelesWithinLociInSample::sTriggerLowPriority = false;
bool NumberOfOLAllelesWithinLociInSample::sDoNotCall = false;

int NumberOfOLAllelesWithinLociInSample::sMessageIndex = 0;
int NumberOfOLAllelesWithinLociInSample::sMessageScope = 0;
RGDList NumberOfStutterPeaksExceedsThreshold::sMessageList;
int NumberOfStutterPeaksExceedsThreshold::sMessageNumber = 732;
int NumberOfStutterPeaksExceedsThreshold::sSubject = NumberOfStutterPeaksExceedsThreshold::LoadType ();
int NumberOfStutterPeaksExceedsThreshold::sMessageLevel = 1;
RGString** NumberOfStutterPeaksExceedsThreshold::sMessageArray = NULL;
RGString NumberOfStutterPeaksExceedsThreshold::sLabel;
bool NumberOfStutterPeaksExceedsThreshold::sTriggerLowPriority = false;
bool NumberOfStutterPeaksExceedsThreshold::sDoNotCall = false;

int NumberOfStutterPeaksExceedsThreshold::sMessageIndex = 0;
int NumberOfStutterPeaksExceedsThreshold::sMessageScope = 0;
RGDList NumberOfAdenylationPeaksExceedsThreshold::sMessageList;
int NumberOfAdenylationPeaksExceedsThreshold::sMessageNumber = 733;
int NumberOfAdenylationPeaksExceedsThreshold::sSubject = NumberOfAdenylationPeaksExceedsThreshold::LoadType ();
int NumberOfAdenylationPeaksExceedsThreshold::sMessageLevel = 1;
RGString** NumberOfAdenylationPeaksExceedsThreshold::sMessageArray = NULL;
RGString NumberOfAdenylationPeaksExceedsThreshold::sLabel;
bool NumberOfAdenylationPeaksExceedsThreshold::sTriggerLowPriority = false;
bool NumberOfAdenylationPeaksExceedsThreshold::sDoNotCall = false;

int NumberOfAdenylationPeaksExceedsThreshold::sMessageIndex = 0;
int NumberOfAdenylationPeaksExceedsThreshold::sMessageScope = 0;
RGDList NumberOfPullUpPeaksExceedsThreshold::sMessageList;
int NumberOfPullUpPeaksExceedsThreshold::sMessageNumber = 734;
int NumberOfPullUpPeaksExceedsThreshold::sSubject = NumberOfPullUpPeaksExceedsThreshold::LoadType ();
int NumberOfPullUpPeaksExceedsThreshold::sMessageLevel = 1;
RGString** NumberOfPullUpPeaksExceedsThreshold::sMessageArray = NULL;
RGString NumberOfPullUpPeaksExceedsThreshold::sLabel;
bool NumberOfPullUpPeaksExceedsThreshold::sTriggerLowPriority = false;
bool NumberOfPullUpPeaksExceedsThreshold::sDoNotCall = false;

int NumberOfPullUpPeaksExceedsThreshold::sMessageIndex = 0;
int NumberOfPullUpPeaksExceedsThreshold::sMessageScope = 0;
RGDList NumberOfOLAllelesExceedsThreshold::sMessageList;
int NumberOfOLAllelesExceedsThreshold::sMessageNumber = 735;
int NumberOfOLAllelesExceedsThreshold::sSubject = NumberOfOLAllelesExceedsThreshold::LoadType ();
int NumberOfOLAllelesExceedsThreshold::sMessageLevel = 1;
RGString** NumberOfOLAllelesExceedsThreshold::sMessageArray = NULL;
RGString NumberOfOLAllelesExceedsThreshold::sLabel;
bool NumberOfOLAllelesExceedsThreshold::sTriggerLowPriority = false;
bool NumberOfOLAllelesExceedsThreshold::sDoNotCall = false;

int NumberOfOLAllelesExceedsThreshold::sMessageIndex = 0;
int NumberOfOLAllelesExceedsThreshold::sMessageScope = 0;
RGDList NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageList;
int NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageNumber = 736;
int NumberOfPrimaryPullUpPeaksExceedsThreshold::sSubject = NumberOfPrimaryPullUpPeaksExceedsThreshold::LoadType ();
int NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageLevel = 1;
RGString** NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageArray = NULL;
RGString NumberOfPrimaryPullUpPeaksExceedsThreshold::sLabel;
bool NumberOfPrimaryPullUpPeaksExceedsThreshold::sTriggerLowPriority = false;
bool NumberOfPrimaryPullUpPeaksExceedsThreshold::sDoNotCall = false;

int NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageIndex = 0;
int NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageScope = 0;
RGDList RemovablePrimaryPullUp::sMessageList;
int RemovablePrimaryPullUp::sMessageNumber = 737;
int RemovablePrimaryPullUp::sSubject = RemovablePrimaryPullUp::LoadType ();
int RemovablePrimaryPullUp::sMessageLevel = 1;
RGString** RemovablePrimaryPullUp::sMessageArray = NULL;
RGString RemovablePrimaryPullUp::sLabel;
bool RemovablePrimaryPullUp::sTriggerLowPriority = false;
bool RemovablePrimaryPullUp::sDoNotCall = false;

int RemovablePrimaryPullUp::sMessageIndex = 0;
int RemovablePrimaryPullUp::sMessageScope = 0;
RGDList Crater::sMessageList;
int Crater::sMessageNumber = 738;
int Crater::sSubject = Crater::LoadType ();
int Crater::sMessageLevel = 1;
RGString** Crater::sMessageArray = NULL;
RGString Crater::sLabel;
bool Crater::sTriggerLowPriority = false;
bool Crater::sDoNotCall = false;

int Crater::sMessageIndex = 0;
int Crater::sMessageScope = 0;
RGDList CraterBetweenLoci::sMessageList;
int CraterBetweenLoci::sMessageNumber = 739;
int CraterBetweenLoci::sSubject = CraterBetweenLoci::LoadType ();
int CraterBetweenLoci::sMessageLevel = 1;
RGString** CraterBetweenLoci::sMessageArray = NULL;
RGString CraterBetweenLoci::sLabel;
bool CraterBetweenLoci::sTriggerLowPriority = false;
bool CraterBetweenLoci::sDoNotCall = false;

int CraterBetweenLoci::sMessageIndex = 0;
int CraterBetweenLoci::sMessageScope = 0;
RGDList PoorPeakMorphology::sMessageList;
int PoorPeakMorphology::sMessageNumber = 740;
int PoorPeakMorphology::sSubject = PoorPeakMorphology::LoadType ();
int PoorPeakMorphology::sMessageLevel = 1;
RGString** PoorPeakMorphology::sMessageArray = NULL;
RGString PoorPeakMorphology::sLabel;
bool PoorPeakMorphology::sTriggerLowPriority = false;
bool PoorPeakMorphology::sDoNotCall = false;

int PoorPeakMorphology::sMessageIndex = 0;
int PoorPeakMorphology::sMessageScope = 0;
RGDList PoorLocusMorphology::sMessageList;
int PoorLocusMorphology::sMessageNumber = 741;
int PoorLocusMorphology::sSubject = PoorLocusMorphology::LoadType ();
int PoorLocusMorphology::sMessageLevel = 1;
RGString** PoorLocusMorphology::sMessageArray = NULL;
RGString PoorLocusMorphology::sLabel;
bool PoorLocusMorphology::sTriggerLowPriority = false;
bool PoorLocusMorphology::sDoNotCall = false;

int PoorLocusMorphology::sMessageIndex = 0;
int PoorLocusMorphology::sMessageScope = 0;
RGDList PossibleUnreportedOLAlleles::sMessageList;
int PossibleUnreportedOLAlleles::sMessageNumber = 742;
int PossibleUnreportedOLAlleles::sSubject = PossibleUnreportedOLAlleles::LoadType ();
int PossibleUnreportedOLAlleles::sMessageLevel = 1;
RGString** PossibleUnreportedOLAlleles::sMessageArray = NULL;
RGString PossibleUnreportedOLAlleles::sLabel;
bool PossibleUnreportedOLAlleles::sTriggerLowPriority = false;
bool PossibleUnreportedOLAlleles::sDoNotCall = false;

int PossibleUnreportedOLAlleles::sMessageIndex = 0;
int PossibleUnreportedOLAlleles::sMessageScope = 0;
RGDList XPeakMissingFromAmelogenin::sMessageList;
int XPeakMissingFromAmelogenin::sMessageNumber = 743;
int XPeakMissingFromAmelogenin::sSubject = XPeakMissingFromAmelogenin::LoadType ();
int XPeakMissingFromAmelogenin::sMessageLevel = 1;
RGString** XPeakMissingFromAmelogenin::sMessageArray = NULL;
RGString XPeakMissingFromAmelogenin::sLabel;
bool XPeakMissingFromAmelogenin::sTriggerLowPriority = false;
bool XPeakMissingFromAmelogenin::sDoNotCall = false;

int XPeakMissingFromAmelogenin::sMessageIndex = 0;
int XPeakMissingFromAmelogenin::sMessageScope = 0;
RGDList ExtraneousPositiveControlPeak::sMessageList;
int ExtraneousPositiveControlPeak::sMessageNumber = 744;
int ExtraneousPositiveControlPeak::sSubject = ExtraneousPositiveControlPeak::LoadType ();
int ExtraneousPositiveControlPeak::sMessageLevel = 1;
RGString** ExtraneousPositiveControlPeak::sMessageArray = NULL;
RGString ExtraneousPositiveControlPeak::sLabel;
bool ExtraneousPositiveControlPeak::sTriggerLowPriority = false;
bool ExtraneousPositiveControlPeak::sDoNotCall = false;

int ExtraneousPositiveControlPeak::sMessageIndex = 0;
int ExtraneousPositiveControlPeak::sMessageScope = 0;
RGDList NoPositiveControlInDirectory::sMessageList;
int NoPositiveControlInDirectory::sMessageNumber = 745;
int NoPositiveControlInDirectory::sSubject = NoPositiveControlInDirectory::LoadType ();
int NoPositiveControlInDirectory::sMessageLevel = 1;
RGString** NoPositiveControlInDirectory::sMessageArray = NULL;
RGString NoPositiveControlInDirectory::sLabel;
bool NoPositiveControlInDirectory::sTriggerLowPriority = false;
bool NoPositiveControlInDirectory::sDoNotCall = false;

int NoPositiveControlInDirectory::sMessageIndex = 0;
int NoPositiveControlInDirectory::sMessageScope = 0;
RGDList NoNegativeControlInDirectory::sMessageList;
int NoNegativeControlInDirectory::sMessageNumber = 746;
int NoNegativeControlInDirectory::sSubject = NoNegativeControlInDirectory::LoadType ();
int NoNegativeControlInDirectory::sMessageLevel = 1;
RGString** NoNegativeControlInDirectory::sMessageArray = NULL;
RGString NoNegativeControlInDirectory::sLabel;
bool NoNegativeControlInDirectory::sTriggerLowPriority = false;
bool NoNegativeControlInDirectory::sDoNotCall = false;

int NoNegativeControlInDirectory::sMessageIndex = 0;
int NoNegativeControlInDirectory::sMessageScope = 0;
RGDList NamedPositiveControlNotFound::sMessageList;
int NamedPositiveControlNotFound::sMessageNumber = 747;
int NamedPositiveControlNotFound::sSubject = NamedPositiveControlNotFound::LoadType ();
int NamedPositiveControlNotFound::sMessageLevel = 1;
RGString** NamedPositiveControlNotFound::sMessageArray = NULL;
RGString NamedPositiveControlNotFound::sLabel;
bool NamedPositiveControlNotFound::sTriggerLowPriority = false;
bool NamedPositiveControlNotFound::sDoNotCall = false;

int NamedPositiveControlNotFound::sMessageIndex = 0;
int NamedPositiveControlNotFound::sMessageScope = 0;
RGDList PositiveCOntrolLocusNotFound::sMessageList;
int PositiveCOntrolLocusNotFound::sMessageNumber = 748;
int PositiveCOntrolLocusNotFound::sSubject = PositiveCOntrolLocusNotFound::LoadType ();
int PositiveCOntrolLocusNotFound::sMessageLevel = 1;
RGString** PositiveCOntrolLocusNotFound::sMessageArray = NULL;
RGString PositiveCOntrolLocusNotFound::sLabel;
bool PositiveCOntrolLocusNotFound::sTriggerLowPriority = false;
bool PositiveCOntrolLocusNotFound::sDoNotCall = false;

int PositiveCOntrolLocusNotFound::sMessageIndex = 0;
int PositiveCOntrolLocusNotFound::sMessageScope = 0;
RGDList SignalPeakAboveMaxRFU::sMessageList;
int SignalPeakAboveMaxRFU::sMessageNumber = 749;
int SignalPeakAboveMaxRFU::sSubject = SignalPeakAboveMaxRFU::LoadType ();
int SignalPeakAboveMaxRFU::sMessageLevel = 1;
RGString** SignalPeakAboveMaxRFU::sMessageArray = NULL;
RGString SignalPeakAboveMaxRFU::sLabel;
bool SignalPeakAboveMaxRFU::sTriggerLowPriority = false;
bool SignalPeakAboveMaxRFU::sDoNotCall = false;

int SignalPeakAboveMaxRFU::sMessageIndex = 0;
int SignalPeakAboveMaxRFU::sMessageScope = 0;
RGDList OneOrMorePeaksAboveMaxRFU::sMessageList;
int OneOrMorePeaksAboveMaxRFU::sMessageNumber = 750;
int OneOrMorePeaksAboveMaxRFU::sSubject = OneOrMorePeaksAboveMaxRFU::LoadType ();
int OneOrMorePeaksAboveMaxRFU::sMessageLevel = 1;
RGString** OneOrMorePeaksAboveMaxRFU::sMessageArray = NULL;
RGString OneOrMorePeaksAboveMaxRFU::sLabel;
bool OneOrMorePeaksAboveMaxRFU::sTriggerLowPriority = false;
bool OneOrMorePeaksAboveMaxRFU::sDoNotCall = false;

int OneOrMorePeaksAboveMaxRFU::sMessageIndex = 0;
int OneOrMorePeaksAboveMaxRFU::sMessageScope = 0;
RGDList ResidualExceedsThreshold::sMessageList;
int ResidualExceedsThreshold::sMessageNumber = 751;
int ResidualExceedsThreshold::sSubject = ResidualExceedsThreshold::LoadType ();
int ResidualExceedsThreshold::sMessageLevel = 1;
RGString** ResidualExceedsThreshold::sMessageArray = NULL;
RGString ResidualExceedsThreshold::sLabel;
bool ResidualExceedsThreshold::sTriggerLowPriority = false;
bool ResidualExceedsThreshold::sDoNotCall = false;

int ResidualExceedsThreshold::sMessageIndex = 0;
int ResidualExceedsThreshold::sMessageScope = 0;
RGDList LocusContainsPeaksWithExcessiveResiduals::sMessageList;
int LocusContainsPeaksWithExcessiveResiduals::sMessageNumber = 752;
int LocusContainsPeaksWithExcessiveResiduals::sSubject = LocusContainsPeaksWithExcessiveResiduals::LoadType ();
int LocusContainsPeaksWithExcessiveResiduals::sMessageLevel = 1;
RGString** LocusContainsPeaksWithExcessiveResiduals::sMessageArray = NULL;
RGString LocusContainsPeaksWithExcessiveResiduals::sLabel;
bool LocusContainsPeaksWithExcessiveResiduals::sTriggerLowPriority = false;
bool LocusContainsPeaksWithExcessiveResiduals::sDoNotCall = false;

int LocusContainsPeaksWithExcessiveResiduals::sMessageIndex = 0;
int LocusContainsPeaksWithExcessiveResiduals::sMessageScope = 0;
RGDList SetupErrorNumberPeaksUnavailableForLocus::sMessageList;
int SetupErrorNumberPeaksUnavailableForLocus::sMessageNumber = 753;
int SetupErrorNumberPeaksUnavailableForLocus::sSubject = SetupErrorNumberPeaksUnavailableForLocus::LoadType ();
int SetupErrorNumberPeaksUnavailableForLocus::sMessageLevel = 1;
RGString** SetupErrorNumberPeaksUnavailableForLocus::sMessageArray = NULL;
RGString SetupErrorNumberPeaksUnavailableForLocus::sLabel;
bool SetupErrorNumberPeaksUnavailableForLocus::sTriggerLowPriority = false;
bool SetupErrorNumberPeaksUnavailableForLocus::sDoNotCall = false;

int SetupErrorNumberPeaksUnavailableForLocus::sMessageIndex = 0;
int SetupErrorNumberPeaksUnavailableForLocus::sMessageScope = 0;
RGDList PossibleExtraLocusAllele::sMessageList;
int PossibleExtraLocusAllele::sMessageNumber = 754;
int PossibleExtraLocusAllele::sSubject = PossibleExtraLocusAllele::LoadType ();
int PossibleExtraLocusAllele::sMessageLevel = 1;
RGString** PossibleExtraLocusAllele::sMessageArray = NULL;
RGString PossibleExtraLocusAllele::sLabel;
bool PossibleExtraLocusAllele::sTriggerLowPriority = false;
bool PossibleExtraLocusAllele::sDoNotCall = false;

int PossibleExtraLocusAllele::sMessageIndex = 0;
int PossibleExtraLocusAllele::sMessageScope = 0;
RGDList OsirisBelowMinRFUButRawDataOK::sMessageList;
int OsirisBelowMinRFUButRawDataOK::sMessageNumber = 755;
int OsirisBelowMinRFUButRawDataOK::sSubject = OsirisBelowMinRFUButRawDataOK::LoadType ();
int OsirisBelowMinRFUButRawDataOK::sMessageLevel = 1;
RGString** OsirisBelowMinRFUButRawDataOK::sMessageArray = NULL;
RGString OsirisBelowMinRFUButRawDataOK::sLabel;
bool OsirisBelowMinRFUButRawDataOK::sTriggerLowPriority = false;
bool OsirisBelowMinRFUButRawDataOK::sDoNotCall = false;

int OsirisBelowMinRFUButRawDataOK::sMessageIndex = 0;
int OsirisBelowMinRFUButRawDataOK::sMessageScope = 0;
RGDList RawDataBelowMinRFUButOsirisOK::sMessageList;
int RawDataBelowMinRFUButOsirisOK::sMessageNumber = 756;
int RawDataBelowMinRFUButOsirisOK::sSubject = RawDataBelowMinRFUButOsirisOK::LoadType ();
int RawDataBelowMinRFUButOsirisOK::sMessageLevel = 1;
RGString** RawDataBelowMinRFUButOsirisOK::sMessageArray = NULL;
RGString RawDataBelowMinRFUButOsirisOK::sLabel;
bool RawDataBelowMinRFUButOsirisOK::sTriggerLowPriority = false;
bool RawDataBelowMinRFUButOsirisOK::sDoNotCall = false;

int RawDataBelowMinRFUButOsirisOK::sMessageIndex = 0;
int RawDataBelowMinRFUButOsirisOK::sMessageScope = 0;
RGDList OsirisAboveMaxRFUButRawDataOK::sMessageList;
int OsirisAboveMaxRFUButRawDataOK::sMessageNumber = 757;
int OsirisAboveMaxRFUButRawDataOK::sSubject = OsirisAboveMaxRFUButRawDataOK::LoadType ();
int OsirisAboveMaxRFUButRawDataOK::sMessageLevel = 1;
RGString** OsirisAboveMaxRFUButRawDataOK::sMessageArray = NULL;
RGString OsirisAboveMaxRFUButRawDataOK::sLabel;
bool OsirisAboveMaxRFUButRawDataOK::sTriggerLowPriority = false;
bool OsirisAboveMaxRFUButRawDataOK::sDoNotCall = false;

int OsirisAboveMaxRFUButRawDataOK::sMessageIndex = 0;
int OsirisAboveMaxRFUButRawDataOK::sMessageScope = 0;
RGDList RawDataAboveMaxRFUButOsirisOK::sMessageList;
int RawDataAboveMaxRFUButOsirisOK::sMessageNumber = 758;
int RawDataAboveMaxRFUButOsirisOK::sSubject = RawDataAboveMaxRFUButOsirisOK::LoadType ();
int RawDataAboveMaxRFUButOsirisOK::sMessageLevel = 1;
RGString** RawDataAboveMaxRFUButOsirisOK::sMessageArray = NULL;
RGString RawDataAboveMaxRFUButOsirisOK::sLabel;
bool RawDataAboveMaxRFUButOsirisOK::sTriggerLowPriority = false;
bool RawDataAboveMaxRFUButOsirisOK::sDoNotCall = false;

int RawDataAboveMaxRFUButOsirisOK::sMessageIndex = 0;
int RawDataAboveMaxRFUButOsirisOK::sMessageScope = 0;
RGDList LocusHasAllelesWithCriticalNotices::sMessageList;
int LocusHasAllelesWithCriticalNotices::sMessageNumber = 759;
int LocusHasAllelesWithCriticalNotices::sSubject = LocusHasAllelesWithCriticalNotices::LoadType ();
int LocusHasAllelesWithCriticalNotices::sMessageLevel = 1;
RGString** LocusHasAllelesWithCriticalNotices::sMessageArray = NULL;
RGString LocusHasAllelesWithCriticalNotices::sLabel;
bool LocusHasAllelesWithCriticalNotices::sTriggerLowPriority = false;
bool LocusHasAllelesWithCriticalNotices::sDoNotCall = false;

int LocusHasAllelesWithCriticalNotices::sMessageIndex = 0;
int LocusHasAllelesWithCriticalNotices::sMessageScope = 0;
RGDList ILSHasPeaksWithCriticalNotices::sMessageList;
int ILSHasPeaksWithCriticalNotices::sMessageNumber = 760;
int ILSHasPeaksWithCriticalNotices::sSubject = ILSHasPeaksWithCriticalNotices::LoadType ();
int ILSHasPeaksWithCriticalNotices::sMessageLevel = 1;
RGString** ILSHasPeaksWithCriticalNotices::sMessageArray = NULL;
RGString ILSHasPeaksWithCriticalNotices::sLabel;
bool ILSHasPeaksWithCriticalNotices::sTriggerLowPriority = false;
bool ILSHasPeaksWithCriticalNotices::sDoNotCall = false;

int ILSHasPeaksWithCriticalNotices::sMessageIndex = 0;
int ILSHasPeaksWithCriticalNotices::sMessageScope = 0;
RGDList CannotSeparateILSInitialFragments::sMessageList;
int CannotSeparateILSInitialFragments::sMessageNumber = 761;
int CannotSeparateILSInitialFragments::sSubject = CannotSeparateILSInitialFragments::LoadType ();
int CannotSeparateILSInitialFragments::sMessageLevel = 1;
RGString** CannotSeparateILSInitialFragments::sMessageArray = NULL;
RGString CannotSeparateILSInitialFragments::sLabel;
bool CannotSeparateILSInitialFragments::sTriggerLowPriority = false;
bool CannotSeparateILSInitialFragments::sDoNotCall = false;

int CannotSeparateILSInitialFragments::sMessageIndex = 0;
int CannotSeparateILSInitialFragments::sMessageScope = 0;
RGDList ILSNeighborFiltersRemoveTooManyPeaks::sMessageList;
int ILSNeighborFiltersRemoveTooManyPeaks::sMessageNumber = 762;
int ILSNeighborFiltersRemoveTooManyPeaks::sSubject = ILSNeighborFiltersRemoveTooManyPeaks::LoadType ();
int ILSNeighborFiltersRemoveTooManyPeaks::sMessageLevel = 1;
RGString** ILSNeighborFiltersRemoveTooManyPeaks::sMessageArray = NULL;
RGString ILSNeighborFiltersRemoveTooManyPeaks::sLabel;
bool ILSNeighborFiltersRemoveTooManyPeaks::sTriggerLowPriority = false;
bool ILSNeighborFiltersRemoveTooManyPeaks::sDoNotCall = false;

int ILSNeighborFiltersRemoveTooManyPeaks::sMessageIndex = 0;
int ILSNeighborFiltersRemoveTooManyPeaks::sMessageScope = 0;
RGDList ILSHeightFiltersRemoveTooManyPeaks::sMessageList;
int ILSHeightFiltersRemoveTooManyPeaks::sMessageNumber = 763;
int ILSHeightFiltersRemoveTooManyPeaks::sSubject = ILSHeightFiltersRemoveTooManyPeaks::LoadType ();
int ILSHeightFiltersRemoveTooManyPeaks::sMessageLevel = 1;
RGString** ILSHeightFiltersRemoveTooManyPeaks::sMessageArray = NULL;
RGString ILSHeightFiltersRemoveTooManyPeaks::sLabel;
bool ILSHeightFiltersRemoveTooManyPeaks::sTriggerLowPriority = false;
bool ILSHeightFiltersRemoveTooManyPeaks::sDoNotCall = false;

int ILSHeightFiltersRemoveTooManyPeaks::sMessageIndex = 0;
int ILSHeightFiltersRemoveTooManyPeaks::sMessageScope = 0;
RGDList SignalPeakBelowFractionalFilterHeight::sMessageList;
int SignalPeakBelowFractionalFilterHeight::sMessageNumber = 764;
int SignalPeakBelowFractionalFilterHeight::sSubject = SignalPeakBelowFractionalFilterHeight::LoadType ();
int SignalPeakBelowFractionalFilterHeight::sMessageLevel = 1;
RGString** SignalPeakBelowFractionalFilterHeight::sMessageArray = NULL;
RGString SignalPeakBelowFractionalFilterHeight::sLabel;
bool SignalPeakBelowFractionalFilterHeight::sTriggerLowPriority = false;
bool SignalPeakBelowFractionalFilterHeight::sDoNotCall = false;

int SignalPeakBelowFractionalFilterHeight::sMessageIndex = 0;
int SignalPeakBelowFractionalFilterHeight::sMessageScope = 0;
RGDList ILSRelativeHeightsInconsistent::sMessageList;
int ILSRelativeHeightsInconsistent::sMessageNumber = 765;
int ILSRelativeHeightsInconsistent::sSubject = ILSRelativeHeightsInconsistent::LoadType ();
int ILSRelativeHeightsInconsistent::sMessageLevel = 1;
RGString** ILSRelativeHeightsInconsistent::sMessageArray = NULL;
RGString ILSRelativeHeightsInconsistent::sLabel;
bool ILSRelativeHeightsInconsistent::sTriggerLowPriority = false;
bool ILSRelativeHeightsInconsistent::sDoNotCall = false;

int ILSRelativeHeightsInconsistent::sMessageIndex = 0;
int ILSRelativeHeightsInconsistent::sMessageScope = 0;
RGDList ExtraneousPeakInAMEL::sMessageList;
int ExtraneousPeakInAMEL::sMessageNumber = 766;
int ExtraneousPeakInAMEL::sSubject = ExtraneousPeakInAMEL::LoadType ();
int ExtraneousPeakInAMEL::sMessageLevel = 1;
RGString** ExtraneousPeakInAMEL::sMessageArray = NULL;
RGString ExtraneousPeakInAMEL::sLabel;
bool ExtraneousPeakInAMEL::sTriggerLowPriority = false;
bool ExtraneousPeakInAMEL::sDoNotCall = false;

int ExtraneousPeakInAMEL::sMessageIndex = 0;
int ExtraneousPeakInAMEL::sMessageScope = 0;
RGDList OLPeakInAMEL::sMessageList;
int OLPeakInAMEL::sMessageNumber = 767;
int OLPeakInAMEL::sSubject = OLPeakInAMEL::LoadType ();
int OLPeakInAMEL::sMessageLevel = 1;
RGString** OLPeakInAMEL::sMessageArray = NULL;
RGString OLPeakInAMEL::sLabel;
bool OLPeakInAMEL::sTriggerLowPriority = false;
bool OLPeakInAMEL::sDoNotCall = false;

int OLPeakInAMEL::sMessageIndex = 0;
int OLPeakInAMEL::sMessageScope = 0;
RGDList NoPrimerPeaksInNegativeControlChannel::sMessageList;
int NoPrimerPeaksInNegativeControlChannel::sMessageNumber = 768;
int NoPrimerPeaksInNegativeControlChannel::sSubject = NoPrimerPeaksInNegativeControlChannel::LoadType ();
int NoPrimerPeaksInNegativeControlChannel::sMessageLevel = 1;
RGString** NoPrimerPeaksInNegativeControlChannel::sMessageArray = NULL;
RGString NoPrimerPeaksInNegativeControlChannel::sLabel;
bool NoPrimerPeaksInNegativeControlChannel::sTriggerLowPriority = false;
bool NoPrimerPeaksInNegativeControlChannel::sDoNotCall = false;

int NoPrimerPeaksInNegativeControlChannel::sMessageIndex = 0;
int NoPrimerPeaksInNegativeControlChannel::sMessageScope = 0;
RGDList NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageList;
int NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageNumber = 769;
int NoPrimerPeaksInOneOrMoreNegControlChannel::sSubject = NoPrimerPeaksInOneOrMoreNegControlChannel::LoadType ();
int NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageLevel = 1;
RGString** NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageArray = NULL;
RGString NoPrimerPeaksInOneOrMoreNegControlChannel::sLabel;
bool NoPrimerPeaksInOneOrMoreNegControlChannel::sTriggerLowPriority = false;
bool NoPrimerPeaksInOneOrMoreNegControlChannel::sDoNotCall = false;

int NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageIndex = 0;
int NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageScope = 0;
RGDList HeightImbalance::sMessageList;
int HeightImbalance::sMessageNumber = 770;
int HeightImbalance::sSubject = HeightImbalance::LoadType ();
int HeightImbalance::sMessageLevel = 1;
RGString** HeightImbalance::sMessageArray = NULL;
RGString HeightImbalance::sLabel;
bool HeightImbalance::sTriggerLowPriority = false;
bool HeightImbalance::sDoNotCall = false;

int HeightImbalance::sMessageIndex = 0;
int HeightImbalance::sMessageScope = 0;
RGDList InterlocusPeakToRightOfLocus::sMessageList;
int InterlocusPeakToRightOfLocus::sMessageNumber = 771;
int InterlocusPeakToRightOfLocus::sSubject = InterlocusPeakToRightOfLocus::LoadType ();
int InterlocusPeakToRightOfLocus::sMessageLevel = 1;
RGString** InterlocusPeakToRightOfLocus::sMessageArray = NULL;
RGString InterlocusPeakToRightOfLocus::sLabel;
bool InterlocusPeakToRightOfLocus::sTriggerLowPriority = false;
bool InterlocusPeakToRightOfLocus::sDoNotCall = false;

int InterlocusPeakToRightOfLocus::sMessageIndex = 0;
int InterlocusPeakToRightOfLocus::sMessageScope = 0;
RGDList InterlocusPeakToLeftOfLocus::sMessageList;
int InterlocusPeakToLeftOfLocus::sMessageNumber = 772;
int InterlocusPeakToLeftOfLocus::sSubject = InterlocusPeakToLeftOfLocus::LoadType ();
int InterlocusPeakToLeftOfLocus::sMessageLevel = 1;
RGString** InterlocusPeakToLeftOfLocus::sMessageArray = NULL;
RGString InterlocusPeakToLeftOfLocus::sLabel;
bool InterlocusPeakToLeftOfLocus::sTriggerLowPriority = false;
bool InterlocusPeakToLeftOfLocus::sDoNotCall = false;

int InterlocusPeakToLeftOfLocus::sMessageIndex = 0;
int InterlocusPeakToLeftOfLocus::sMessageScope = 0;
RGDList SignalPeakBelowFractionalFilterHeightPlus::sMessageList;
int SignalPeakBelowFractionalFilterHeightPlus::sMessageNumber = 773;
int SignalPeakBelowFractionalFilterHeightPlus::sSubject = SignalPeakBelowFractionalFilterHeightPlus::LoadType ();
int SignalPeakBelowFractionalFilterHeightPlus::sMessageLevel = 1;
RGString** SignalPeakBelowFractionalFilterHeightPlus::sMessageArray = NULL;
RGString SignalPeakBelowFractionalFilterHeightPlus::sLabel;
bool SignalPeakBelowFractionalFilterHeightPlus::sTriggerLowPriority = false;
bool SignalPeakBelowFractionalFilterHeightPlus::sDoNotCall = false;

int SignalPeakBelowFractionalFilterHeightPlus::sMessageIndex = 0;
int SignalPeakBelowFractionalFilterHeightPlus::sMessageScope = 0;
RGDList LadderRelativeHeightsInconsistent::sMessageList;
int LadderRelativeHeightsInconsistent::sMessageNumber = 774;
int LadderRelativeHeightsInconsistent::sSubject = LadderRelativeHeightsInconsistent::LoadType ();
int LadderRelativeHeightsInconsistent::sMessageLevel = 1;
RGString** LadderRelativeHeightsInconsistent::sMessageArray = NULL;
RGString LadderRelativeHeightsInconsistent::sLabel;
bool LadderRelativeHeightsInconsistent::sTriggerLowPriority = false;
bool LadderRelativeHeightsInconsistent::sDoNotCall = false;

int LadderRelativeHeightsInconsistent::sMessageIndex = 0;
int LadderRelativeHeightsInconsistent::sMessageScope = 0;
RGDList AmbiguousInterlocusCraterAnalysis::sMessageList;
int AmbiguousInterlocusCraterAnalysis::sMessageNumber = 775;
int AmbiguousInterlocusCraterAnalysis::sSubject = AmbiguousInterlocusCraterAnalysis::LoadType ();
int AmbiguousInterlocusCraterAnalysis::sMessageLevel = 1;
RGString** AmbiguousInterlocusCraterAnalysis::sMessageArray = NULL;
RGString AmbiguousInterlocusCraterAnalysis::sLabel;
bool AmbiguousInterlocusCraterAnalysis::sTriggerLowPriority = false;
bool AmbiguousInterlocusCraterAnalysis::sDoNotCall = false;

int AmbiguousInterlocusCraterAnalysis::sMessageIndex = 0;
int AmbiguousInterlocusCraterAnalysis::sMessageScope = 0;
RGDList PeakBelowPullupFractionalFilter::sMessageList;
int PeakBelowPullupFractionalFilter::sMessageNumber = 776;
int PeakBelowPullupFractionalFilter::sSubject = PeakBelowPullupFractionalFilter::LoadType ();
int PeakBelowPullupFractionalFilter::sMessageLevel = 1;
RGString** PeakBelowPullupFractionalFilter::sMessageArray = NULL;
RGString PeakBelowPullupFractionalFilter::sLabel;
bool PeakBelowPullupFractionalFilter::sTriggerLowPriority = false;
bool PeakBelowPullupFractionalFilter::sDoNotCall = false;

int PeakBelowPullupFractionalFilter::sMessageIndex = 0;
int PeakBelowPullupFractionalFilter::sMessageScope = 0;
RGDList PeakBelowPullupFractionalFilterLeft::sMessageList;
int PeakBelowPullupFractionalFilterLeft::sMessageNumber = 777;
int PeakBelowPullupFractionalFilterLeft::sSubject = PeakBelowPullupFractionalFilterLeft::LoadType ();
int PeakBelowPullupFractionalFilterLeft::sMessageLevel = 1;
RGString** PeakBelowPullupFractionalFilterLeft::sMessageArray = NULL;
RGString PeakBelowPullupFractionalFilterLeft::sLabel;
bool PeakBelowPullupFractionalFilterLeft::sTriggerLowPriority = false;
bool PeakBelowPullupFractionalFilterLeft::sDoNotCall = false;

int PeakBelowPullupFractionalFilterLeft::sMessageIndex = 0;
int PeakBelowPullupFractionalFilterLeft::sMessageScope = 0;
RGDList PeakBelowPullupFractionalFilterRight::sMessageList;
int PeakBelowPullupFractionalFilterRight::sMessageNumber = 778;
int PeakBelowPullupFractionalFilterRight::sSubject = PeakBelowPullupFractionalFilterRight::LoadType ();
int PeakBelowPullupFractionalFilterRight::sMessageLevel = 1;
RGString** PeakBelowPullupFractionalFilterRight::sMessageArray = NULL;
RGString PeakBelowPullupFractionalFilterRight::sLabel;
bool PeakBelowPullupFractionalFilterRight::sTriggerLowPriority = false;
bool PeakBelowPullupFractionalFilterRight::sDoNotCall = false;

int PeakBelowPullupFractionalFilterRight::sMessageIndex = 0;
int PeakBelowPullupFractionalFilterRight::sMessageScope = 0;
RGDList PeakBelowFractionalFilterLeft::sMessageList;
int PeakBelowFractionalFilterLeft::sMessageNumber = 779;
int PeakBelowFractionalFilterLeft::sSubject = PeakBelowFractionalFilterLeft::LoadType ();
int PeakBelowFractionalFilterLeft::sMessageLevel = 1;
RGString** PeakBelowFractionalFilterLeft::sMessageArray = NULL;
RGString PeakBelowFractionalFilterLeft::sLabel;
bool PeakBelowFractionalFilterLeft::sTriggerLowPriority = false;
bool PeakBelowFractionalFilterLeft::sDoNotCall = false;

int PeakBelowFractionalFilterLeft::sMessageIndex = 0;
int PeakBelowFractionalFilterLeft::sMessageScope = 0;
RGDList PeakBelowFractionalFilterRight::sMessageList;
int PeakBelowFractionalFilterRight::sMessageNumber = 780;
int PeakBelowFractionalFilterRight::sSubject = PeakBelowFractionalFilterRight::LoadType ();
int PeakBelowFractionalFilterRight::sMessageLevel = 1;
RGString** PeakBelowFractionalFilterRight::sMessageArray = NULL;
RGString PeakBelowFractionalFilterRight::sLabel;
bool PeakBelowFractionalFilterRight::sTriggerLowPriority = false;
bool PeakBelowFractionalFilterRight::sDoNotCall = false;

int PeakBelowFractionalFilterRight::sMessageIndex = 0;
int PeakBelowFractionalFilterRight::sMessageScope = 0;
RGDList PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageList;
int PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageNumber = 781;
int PossibleAmbiguousInterlocusPeakAssignedToLocus::sSubject = PossibleAmbiguousInterlocusPeakAssignedToLocus::LoadType ();
int PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageLevel = 1;
RGString** PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageArray = NULL;
RGString PossibleAmbiguousInterlocusPeakAssignedToLocus::sLabel;
bool PossibleAmbiguousInterlocusPeakAssignedToLocus::sTriggerLowPriority = false;
bool PossibleAmbiguousInterlocusPeakAssignedToLocus::sDoNotCall = false;

int PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageIndex = 0;
int PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageScope = 0;
RGDList AmbiguousInterlocusPeak::sMessageList;
int AmbiguousInterlocusPeak::sMessageNumber = 782;
int AmbiguousInterlocusPeak::sSubject = AmbiguousInterlocusPeak::LoadType ();
int AmbiguousInterlocusPeak::sMessageLevel = 1;
RGString** AmbiguousInterlocusPeak::sMessageArray = NULL;
RGString AmbiguousInterlocusPeak::sLabel;
bool AmbiguousInterlocusPeak::sTriggerLowPriority = false;
bool AmbiguousInterlocusPeak::sDoNotCall = false;

int AmbiguousInterlocusPeak::sMessageIndex = 0;
int AmbiguousInterlocusPeak::sMessageScope = 0;
RGDList ExtraneousPeakInNegativeControl::sMessageList;
int ExtraneousPeakInNegativeControl::sMessageNumber = 783;
int ExtraneousPeakInNegativeControl::sSubject = ExtraneousPeakInNegativeControl::LoadType ();
int ExtraneousPeakInNegativeControl::sMessageLevel = 1;
RGString** ExtraneousPeakInNegativeControl::sMessageArray = NULL;
RGString ExtraneousPeakInNegativeControl::sLabel;
bool ExtraneousPeakInNegativeControl::sTriggerLowPriority = false;
bool ExtraneousPeakInNegativeControl::sDoNotCall = false;

int ExtraneousPeakInNegativeControl::sMessageIndex = 0;
int ExtraneousPeakInNegativeControl::sMessageScope = 0;
RGDList ExtraneousPeakInLadderLocus::sMessageList;
int ExtraneousPeakInLadderLocus::sMessageNumber = 784;
int ExtraneousPeakInLadderLocus::sSubject = ExtraneousPeakInLadderLocus::LoadType ();
int ExtraneousPeakInLadderLocus::sMessageLevel = 1;
RGString** ExtraneousPeakInLadderLocus::sMessageArray = NULL;
RGString ExtraneousPeakInLadderLocus::sLabel;
bool ExtraneousPeakInLadderLocus::sTriggerLowPriority = false;
bool ExtraneousPeakInLadderLocus::sDoNotCall = false;

int ExtraneousPeakInLadderLocus::sMessageIndex = 0;
int ExtraneousPeakInLadderLocus::sMessageScope = 0;
RGDList NumberOfExcessiveResidualsExceedsThreshold::sMessageList;
int NumberOfExcessiveResidualsExceedsThreshold::sMessageNumber = 785;
int NumberOfExcessiveResidualsExceedsThreshold::sSubject = NumberOfExcessiveResidualsExceedsThreshold::LoadType ();
int NumberOfExcessiveResidualsExceedsThreshold::sMessageLevel = 1;
RGString** NumberOfExcessiveResidualsExceedsThreshold::sMessageArray = NULL;
RGString NumberOfExcessiveResidualsExceedsThreshold::sLabel;
bool NumberOfExcessiveResidualsExceedsThreshold::sTriggerLowPriority = false;
bool NumberOfExcessiveResidualsExceedsThreshold::sDoNotCall = false;

int NumberOfExcessiveResidualsExceedsThreshold::sMessageIndex = 0;
int NumberOfExcessiveResidualsExceedsThreshold::sMessageScope = 0;
RGDList NumberOfExcessiveResidualsInSample::sMessageList;
int NumberOfExcessiveResidualsInSample::sMessageNumber = 786;
int NumberOfExcessiveResidualsInSample::sSubject = NumberOfExcessiveResidualsInSample::LoadType ();
int NumberOfExcessiveResidualsInSample::sMessageLevel = 1;
RGString** NumberOfExcessiveResidualsInSample::sMessageArray = NULL;
RGString NumberOfExcessiveResidualsInSample::sLabel;
bool NumberOfExcessiveResidualsInSample::sTriggerLowPriority = false;
bool NumberOfExcessiveResidualsInSample::sDoNotCall = false;

int NumberOfExcessiveResidualsInSample::sMessageIndex = 0;
int NumberOfExcessiveResidualsInSample::sMessageScope = 0;
RGDList InterlocusPeakBelowInterlocusMinRFU::sMessageList;
int InterlocusPeakBelowInterlocusMinRFU::sMessageNumber = 787;
int InterlocusPeakBelowInterlocusMinRFU::sSubject = InterlocusPeakBelowInterlocusMinRFU::LoadType ();
int InterlocusPeakBelowInterlocusMinRFU::sMessageLevel = 1;
RGString** InterlocusPeakBelowInterlocusMinRFU::sMessageArray = NULL;
RGString InterlocusPeakBelowInterlocusMinRFU::sLabel;
bool InterlocusPeakBelowInterlocusMinRFU::sTriggerLowPriority = false;
bool InterlocusPeakBelowInterlocusMinRFU::sDoNotCall = false;

int InterlocusPeakBelowInterlocusMinRFU::sMessageIndex = 0;
int InterlocusPeakBelowInterlocusMinRFU::sMessageScope = 0;

PERSISTENT_DEFINITION (SampleReqReview, 1201, "SampleReqReview")
PERSISTENT_DEFINITION (PosCtrlReqReview, 1202, "PosCtrlReqReview")
PERSISTENT_DEFINITION (NegCtrlReqReview, 1203, "NegCtrlReqReview")
PERSISTENT_DEFINITION (ILSReqReview, 1204, "ILSReqReview")
PERSISTENT_DEFINITION (LadderReqReview, 1205, "LadderReqReview")
PERSISTENT_DEFINITION (ILSSpacing, 1206, "ILSSpacing")
PERSISTENT_DEFINITION (LocusSpacing, 1207, "LocusSpacing")
PERSISTENT_DEFINITION (NoLadderFile, 1208, "NoLadderFile")
PERSISTENT_DEFINITION (LadderTypeUnknown, 1209, "LadderTypeUnknown")
PERSISTENT_DEFINITION (ReagentKitUnknown, 1210, "ReagentKitUnknown")
PERSISTENT_DEFINITION (TooManyReagentKits, 1211, "TooManyReagentKits")
PERSISTENT_DEFINITION (NoPosCtrl, 1212, "NoPosCtrl")
PERSISTENT_DEFINITION (NoNegCtrl, 1213, "NoNegCtrl")
PERSISTENT_DEFINITION (NoSample, 1214, "NoSample")
PERSISTENT_DEFINITION (CurveFitTooLow, 1215, "CurveFitTooLow")
PERSISTENT_DEFINITION (CurveFitMarginal, 1216, "CurveFitMarginal")
PERSISTENT_DEFINITION (ArtifactFound, 1217, "ArtifactFound")
PERSISTENT_DEFINITION (SpikeFound, 1218, "SpikeFound")
PERSISTENT_DEFINITION (StutterFound, 1219, "StutterFound")
PERSISTENT_DEFINITION (AdenylationFound, 1220, "AdenylationFound")
PERSISTENT_DEFINITION (BlobFound, 1221, "BlobFound")
PERSISTENT_DEFINITION (PullUpFound, 1222, "PullUpFound")
PERSISTENT_DEFINITION (NoGenotypeFound, 1223, "NoGenotypeFound")
PERSISTENT_DEFINITION (ThreeAllelesFound, 1224, "ThreeAllelesFound")
PERSISTENT_DEFINITION (UnexpectedNumberOfPeaks, 1225, "UnexpectedNumberOfPeaks")
PERSISTENT_DEFINITION (InterLocusPeak, 1226, "InterLocusPeak")
PERSISTENT_DEFINITION (HeteroImbalanceFound, 1227, "HeteroImbalanceFound")
PERSISTENT_DEFINITION (OLAlleleFound, 1228, "OLAlleleFound")
PERSISTENT_DEFINITION (NotOKHomozygoteFound, 1229, "NotOKHomozygoteFound")
PERSISTENT_DEFINITION (OsirisNotEqualABI, 1230, "OsirisNotEqualABI")
PERSISTENT_DEFINITION (ILSHasTooFewPeaks, 1231, "ILSHasTooFewPeaks")
PERSISTENT_DEFINITION (LocusHasTooFewPeaks, 1232, "LocusHasTooFewPeaks")
PERSISTENT_DEFINITION (DirectoryInfoInsufficient, 1233, "DirectoryInfoInsufficient")
PERSISTENT_DEFINITION (CouldNotReadFSA, 1234, "CouldNotReadFSA")
PERSISTENT_DEFINITION (NoInputDirectory, 1235, "NoInputDirectory")
PERSISTENT_DEFINITION (CouldNotOpenOutputFile, 1236, "CouldNotOpenOutputFile")
PERSISTENT_DEFINITION (LadderDataFormatUnknown, 1237, "LadderDataFormatUnknown")
PERSISTENT_DEFINITION (PervasiveStutter, 1238, "PervasiveStutter")
PERSISTENT_DEFINITION (PeakOutsideILSRange, 1239, "PeakOutsideILSRange")
PERSISTENT_DEFINITION (InterLocusLadderPeak, 1240, "InterLocusLadderPeak")
PERSISTENT_DEFINITION (NoNamedMarkerSet, 1241, "NoNamedMarkerSet")
PERSISTENT_DEFINITION (NoNamedLaneStandard, 1242, "NoNamedLaneStandard")
PERSISTENT_DEFINITION (SignalTooLowForLocus, 1243, "SignalTooLowForLocus")
PERSISTENT_DEFINITION (SignalPeakBelowMinRFU, 1244, "SignalPeakBelowMinRFU")
PERSISTENT_DEFINITION (PeakOutOfPlaceInILS, 1245, "PeakOutOfPlaceInILS")
PERSISTENT_DEFINITION (PeakOutOfPlaceInLocus, 1246, "PeakOutOfPlaceInLocus")
PERSISTENT_DEFINITION (PullUpPrimaryChannel, 1247, "PullUpPrimaryChannel")
PERSISTENT_DEFINITION (AllelePullUp, 1248, "AllelePullUp")
PERSISTENT_DEFINITION (HeteroImbalancePullUp, 1249, "HeteroImbalancePullUp")
PERSISTENT_DEFINITION (LowTriAllelePullUp, 1250, "LowTriAllelePullUp")
PERSISTENT_DEFINITION (LowOLAllelePullUp, 1251, "LowOLAllelePullUp")
PERSISTENT_DEFINITION (PeakMisorderPullUp, 1252, "PeakMisorderPullUp")
PERSISTENT_DEFINITION (IgnoreLowOLAlleles, 1253, "IgnoreLowOLAlleles")
PERSISTENT_DEFINITION (PeakMisorder, 1254, "PeakMisorder")
PERSISTENT_DEFINITION (ControlPeakPullUp, 1255, "ControlPeakPullUp")
PERSISTENT_DEFINITION (OLBetweenLociPullUp, 1256, "OLBetweenLociPullUp")
PERSISTENT_DEFINITION (IgnoreLowOnLadderAlleles, 1257, "IgnoreLowOnLadderAlleles")
PERSISTENT_DEFINITION (CraterInducedByPullUp, 1258, "CraterInducedByPullUp")
PERSISTENT_DEFINITION (CraterInducedByPullUpBetweenLoci, 1259, "CraterInducedByPullUpBetweenLoci")
PERSISTENT_DEFINITION (NumberOfPullUpsInSample, 1260, "NumberOfPullUpsInSample")
PERSISTENT_DEFINITION (NumberOfAdenylationsInSample, 1261, "NumberOfAdenylationsInSample")
PERSISTENT_DEFINITION (NumberOfStutterPeaksInSample, 1262, "NumberOfStutterPeaksInSample")
PERSISTENT_DEFINITION (NumberOfPrimaryPullUpsInSample, 1263, "NumberOfPrimaryPullUpsInSample")
PERSISTENT_DEFINITION (NumberOfOLAllelesWithinLociInSample, 1264, "NumberOfOLAllelesWithinLociInSample")
PERSISTENT_DEFINITION (NumberOfStutterPeaksExceedsThreshold, 1265, "NumberOfStutterPeaksExceedsThreshold")
PERSISTENT_DEFINITION (NumberOfAdenylationPeaksExceedsThreshold, 1266, "NumberOfAdenylationPeaksExceedsThreshold")
PERSISTENT_DEFINITION (NumberOfPullUpPeaksExceedsThreshold, 1267, "NumberOfPullUpPeaksExceedsThreshold")
PERSISTENT_DEFINITION (NumberOfOLAllelesExceedsThreshold, 1268, "NumberOfOLAllelesExceedsThreshold")
PERSISTENT_DEFINITION (NumberOfPrimaryPullUpPeaksExceedsThreshold, 1269, "NumberOfPrimaryPullUpPeaksExceedsThreshold")
PERSISTENT_DEFINITION (RemovablePrimaryPullUp, 1270, "RemovablePrimaryPullUp")
PERSISTENT_DEFINITION (Crater, 1271, "Crater")
PERSISTENT_DEFINITION (CraterBetweenLoci, 1272, "CraterBetweenLoci")
PERSISTENT_DEFINITION (PoorPeakMorphology, 1273, "PoorPeakMorphology")
PERSISTENT_DEFINITION (PoorLocusMorphology, 1274, "PoorLocusMorphology")
PERSISTENT_DEFINITION (PossibleUnreportedOLAlleles, 1275, "PossibleUnreportedOLAlleles")
PERSISTENT_DEFINITION (XPeakMissingFromAmelogenin, 1276, "XPeakMissingFromAmelogenin")
PERSISTENT_DEFINITION (ExtraneousPositiveControlPeak, 1277, "ExtraneousPositiveControlPeak")
PERSISTENT_DEFINITION (NoPositiveControlInDirectory, 1278, "NoPositiveControlInDirectory")
PERSISTENT_DEFINITION (NoNegativeControlInDirectory, 1279, "NoNegativeControlInDirectory")
PERSISTENT_DEFINITION (NamedPositiveControlNotFound, 1280, "NamedPositiveControlNotFound")
PERSISTENT_DEFINITION (PositiveCOntrolLocusNotFound, 1281, "PositiveCOntrolLocusNotFound")
PERSISTENT_DEFINITION (SignalPeakAboveMaxRFU, 1282, "SignalPeakAboveMaxRFU")
PERSISTENT_DEFINITION (OneOrMorePeaksAboveMaxRFU, 1283, "OneOrMorePeaksAboveMaxRFU")
PERSISTENT_DEFINITION (ResidualExceedsThreshold, 1284, "ResidualExceedsThreshold")
PERSISTENT_DEFINITION (LocusContainsPeaksWithExcessiveResiduals, 1285, "LocusContainsPeaksWithExcessiveResiduals")
PERSISTENT_DEFINITION (SetupErrorNumberPeaksUnavailableForLocus, 1286, "SetupErrorNumberPeaksUnavailableForLocus")
PERSISTENT_DEFINITION (PossibleExtraLocusAllele, 1287, "PossibleExtraLocusAllele")
PERSISTENT_DEFINITION (OsirisBelowMinRFUButRawDataOK, 1288, "OsirisBelowMinRFUButRawDataOK")
PERSISTENT_DEFINITION (RawDataBelowMinRFUButOsirisOK, 1289, "RawDataBelowMinRFUButOsirisOK")
PERSISTENT_DEFINITION (OsirisAboveMaxRFUButRawDataOK, 1290, "OsirisAboveMaxRFUButRawDataOK")
PERSISTENT_DEFINITION (RawDataAboveMaxRFUButOsirisOK, 1291, "RawDataAboveMaxRFUButOsirisOK")
PERSISTENT_DEFINITION (LocusHasAllelesWithCriticalNotices, 1292, "LocusHasAllelesWithCriticalNotices")
PERSISTENT_DEFINITION (ILSHasPeaksWithCriticalNotices, 1293, "ILSHasPeaksWithCriticalNotices")
PERSISTENT_DEFINITION (CannotSeparateILSInitialFragments, 1294, "CannotSeparateILSInitialFragments")
PERSISTENT_DEFINITION (ILSNeighborFiltersRemoveTooManyPeaks, 1295, "ILSNeighborFiltersRemoveTooManyPeaks")
PERSISTENT_DEFINITION (ILSHeightFiltersRemoveTooManyPeaks, 1296, "ILSHeightFiltersRemoveTooManyPeaks")
PERSISTENT_DEFINITION (SignalPeakBelowFractionalFilterHeight, 1297, "SignalPeakBelowFractionalFilterHeight")
PERSISTENT_DEFINITION (ILSRelativeHeightsInconsistent, 1298, "ILSRelativeHeightsInconsistent")
PERSISTENT_DEFINITION (ExtraneousPeakInAMEL, 1299, "ExtraneousPeakInAMEL")
PERSISTENT_DEFINITION (OLPeakInAMEL, 1300, "OLPeakInAMEL")
PERSISTENT_DEFINITION (NoPrimerPeaksInNegativeControlChannel, 1301, "NoPrimerPeaksInNegativeControlChannel")
PERSISTENT_DEFINITION (NoPrimerPeaksInOneOrMoreNegControlChannel, 1302, "NoPrimerPeaksInOneOrMoreNegControlChannel")
PERSISTENT_DEFINITION (HeightImbalance, 1303, "HeightImbalance")
PERSISTENT_DEFINITION (InterlocusPeakToRightOfLocus, 1304, "InterlocusPeakToRightOfLocus")
PERSISTENT_DEFINITION (InterlocusPeakToLeftOfLocus, 1305, "InterlocusPeakToLeftOfLocus")
PERSISTENT_DEFINITION (SignalPeakBelowFractionalFilterHeightPlus, 1306, "SignalPeakBelowFractionalFilterHeightPlus")
PERSISTENT_DEFINITION (LadderRelativeHeightsInconsistent, 1307, "LadderRelativeHeightsInconsistent")
PERSISTENT_DEFINITION (AmbiguousInterlocusCraterAnalysis, 1308, "AmbiguousInterlocusCraterAnalysis")
PERSISTENT_DEFINITION (PeakBelowPullupFractionalFilter, 1309, "PeakBelowPullupFractionalFilter")
PERSISTENT_DEFINITION (PeakBelowPullupFractionalFilterLeft, 1310, "PeakBelowPullupFractionalFilterLeft")
PERSISTENT_DEFINITION (PeakBelowPullupFractionalFilterRight, 1311, "PeakBelowPullupFractionalFilterRight")
PERSISTENT_DEFINITION (PeakBelowFractionalFilterLeft, 1312, "PeakBelowFractionalFilterLeft")
PERSISTENT_DEFINITION (PeakBelowFractionalFilterRight, 1313, "PeakBelowFractionalFilterRight")
PERSISTENT_DEFINITION (PossibleAmbiguousInterlocusPeakAssignedToLocus, 1314, "PossibleAmbiguousInterlocusPeakAssignedToLocus")
PERSISTENT_DEFINITION (AmbiguousInterlocusPeak, 1315, "AmbiguousInterlocusPeak")
PERSISTENT_DEFINITION (ExtraneousPeakInNegativeControl, 1316, "ExtraneousPeakInNegativeControl")
PERSISTENT_DEFINITION (ExtraneousPeakInLadderLocus, 1317, "ExtraneousPeakInLadderLocus")
PERSISTENT_DEFINITION (NumberOfExcessiveResidualsExceedsThreshold, 1318, "NumberOfExcessiveResidualsExceedsThreshold")
PERSISTENT_DEFINITION (NumberOfExcessiveResidualsInSample, 1319, "NumberOfExcessiveResidualsInSample")
PERSISTENT_DEFINITION (InterlocusPeakBelowInterlocusMinRFU, 1320, "InterlocusPeakBelowInterlocusMinRFU")




SampleReqReview :: SampleReqReview () : Notice () {

}


SampleReqReview :: SampleReqReview (const SampleReqReview& note) : Notice ((const Notice&) note) {

}



SampleReqReview :: ~SampleReqReview () {

}



int SampleReqReview :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (SampleReqReview::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		SampleReqReview::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SampleReqReview::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SampleReqReview::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		SampleReqReview::sLabel = temp;

	else
		SampleReqReview::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SampleReqReview::sTriggerLowPriority = true;

		else
			SampleReqReview::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SampleReqReview::sDoNotCall = true;

		else
			SampleReqReview::sDoNotCall = false;
	}


	int N = SampleReqReview::sMessageList.Entries ();

	if (N == 1)
		N++;

	SampleReqReview::sMessageArray = new RGString* [N];
	RGDListIterator it (SampleReqReview::sMessageList);

	for (int i=0; i<N; i++)
		SampleReqReview::sMessageArray [i] = (RGString*) it ();

	return status;
}


int SampleReqReview :: GetSubject () const {

	return SampleReqReview::sSubject;
}


RGString SampleReqReview :: GetLabel () const {

	return SampleReqReview::sLabel;
}


bool SampleReqReview :: IsTriggerForLowPriority () const {

	return SampleReqReview::sTriggerLowPriority;
}


bool SampleReqReview :: IsDoNotCall () const {

	return SampleReqReview::sDoNotCall;
}



RGString SampleReqReview :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= SampleReqReview::sMessageList.Entries ()))
		return returnValue;

	returnValue = *SampleReqReview::sMessageArray [i];
	return returnValue;
}



int SampleReqReview :: GetMessageLevel () const {

	return SampleReqReview::sMessageLevel;
}



int SampleReqReview :: GetNumberOfMessageParts () const {

	return SampleReqReview::sMessageList.Entries ();
}


int SampleReqReview :: GetSeverityLevel () const {

	return SampleReqReview::sMessageLevel;
}


void SampleReqReview :: SetIndexAndScope (int index, int scope) const {

	SampleReqReview::sMessageIndex = index;
	SampleReqReview::sMessageScope = scope;
}


int SampleReqReview :: GetMessageIndex () const {

	return SampleReqReview :: sMessageIndex;
}



int SampleReqReview :: GetScope () const {

	return SampleReqReview :: sMessageScope;
}



int SampleReqReview :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	SampleReqReview* noticeType = new SampleReqReview;
	warehouse->AddType (noticeType);
	return 1;
}





PosCtrlReqReview :: PosCtrlReqReview () : Notice () {

}


PosCtrlReqReview :: PosCtrlReqReview (const PosCtrlReqReview& note) : Notice ((const Notice&) note) {

}



PosCtrlReqReview :: ~PosCtrlReqReview () {

}



int PosCtrlReqReview :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PosCtrlReqReview::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PosCtrlReqReview::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PosCtrlReqReview::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PosCtrlReqReview::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PosCtrlReqReview::sLabel = temp;

	else
		PosCtrlReqReview::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PosCtrlReqReview::sTriggerLowPriority = true;

		else
			PosCtrlReqReview::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PosCtrlReqReview::sDoNotCall = true;

		else
			PosCtrlReqReview::sDoNotCall = false;
	}


	int N = PosCtrlReqReview::sMessageList.Entries ();

	if (N == 1)
		N++;

	PosCtrlReqReview::sMessageArray = new RGString* [N];
	RGDListIterator it (PosCtrlReqReview::sMessageList);

	for (int i=0; i<N; i++)
		PosCtrlReqReview::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PosCtrlReqReview :: GetSubject () const {

	return PosCtrlReqReview::sSubject;
}


RGString PosCtrlReqReview :: GetLabel () const {

	return PosCtrlReqReview::sLabel;
}


bool PosCtrlReqReview :: IsTriggerForLowPriority () const {

	return PosCtrlReqReview::sTriggerLowPriority;
}


bool PosCtrlReqReview :: IsDoNotCall () const {

	return PosCtrlReqReview::sDoNotCall;
}



RGString PosCtrlReqReview :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PosCtrlReqReview::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PosCtrlReqReview::sMessageArray [i];
	return returnValue;
}



int PosCtrlReqReview :: GetMessageLevel () const {

	return PosCtrlReqReview::sMessageLevel;
}



int PosCtrlReqReview :: GetNumberOfMessageParts () const {

	return PosCtrlReqReview::sMessageList.Entries ();
}


int PosCtrlReqReview :: GetSeverityLevel () const {

	return PosCtrlReqReview::sMessageLevel;
}


void PosCtrlReqReview :: SetIndexAndScope (int index, int scope) const {

	PosCtrlReqReview::sMessageIndex = index;
	PosCtrlReqReview::sMessageScope = scope;
}


int PosCtrlReqReview :: GetMessageIndex () const {

	return PosCtrlReqReview :: sMessageIndex;
}



int PosCtrlReqReview :: GetScope () const {

	return PosCtrlReqReview :: sMessageScope;
}



int PosCtrlReqReview :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PosCtrlReqReview* noticeType = new PosCtrlReqReview;
	warehouse->AddType (noticeType);
	return 1;
}





NegCtrlReqReview :: NegCtrlReqReview () : Notice () {

}


NegCtrlReqReview :: NegCtrlReqReview (const NegCtrlReqReview& note) : Notice ((const Notice&) note) {

}



NegCtrlReqReview :: ~NegCtrlReqReview () {

}



int NegCtrlReqReview :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NegCtrlReqReview::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NegCtrlReqReview::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NegCtrlReqReview::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NegCtrlReqReview::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NegCtrlReqReview::sLabel = temp;

	else
		NegCtrlReqReview::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NegCtrlReqReview::sTriggerLowPriority = true;

		else
			NegCtrlReqReview::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NegCtrlReqReview::sDoNotCall = true;

		else
			NegCtrlReqReview::sDoNotCall = false;
	}


	int N = NegCtrlReqReview::sMessageList.Entries ();

	if (N == 1)
		N++;

	NegCtrlReqReview::sMessageArray = new RGString* [N];
	RGDListIterator it (NegCtrlReqReview::sMessageList);

	for (int i=0; i<N; i++)
		NegCtrlReqReview::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NegCtrlReqReview :: GetSubject () const {

	return NegCtrlReqReview::sSubject;
}


RGString NegCtrlReqReview :: GetLabel () const {

	return NegCtrlReqReview::sLabel;
}


bool NegCtrlReqReview :: IsTriggerForLowPriority () const {

	return NegCtrlReqReview::sTriggerLowPriority;
}


bool NegCtrlReqReview :: IsDoNotCall () const {

	return NegCtrlReqReview::sDoNotCall;
}



RGString NegCtrlReqReview :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NegCtrlReqReview::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NegCtrlReqReview::sMessageArray [i];
	return returnValue;
}



int NegCtrlReqReview :: GetMessageLevel () const {

	return NegCtrlReqReview::sMessageLevel;
}



int NegCtrlReqReview :: GetNumberOfMessageParts () const {

	return NegCtrlReqReview::sMessageList.Entries ();
}


int NegCtrlReqReview :: GetSeverityLevel () const {

	return NegCtrlReqReview::sMessageLevel;
}


void NegCtrlReqReview :: SetIndexAndScope (int index, int scope) const {

	NegCtrlReqReview::sMessageIndex = index;
	NegCtrlReqReview::sMessageScope = scope;
}


int NegCtrlReqReview :: GetMessageIndex () const {

	return NegCtrlReqReview :: sMessageIndex;
}



int NegCtrlReqReview :: GetScope () const {

	return NegCtrlReqReview :: sMessageScope;
}



int NegCtrlReqReview :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NegCtrlReqReview* noticeType = new NegCtrlReqReview;
	warehouse->AddType (noticeType);
	return 1;
}





ILSReqReview :: ILSReqReview () : Notice () {

}


ILSReqReview :: ILSReqReview (const ILSReqReview& note) : Notice ((const Notice&) note) {

}



ILSReqReview :: ~ILSReqReview () {

}



int ILSReqReview :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ILSReqReview::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ILSReqReview::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSReqReview::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSReqReview::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ILSReqReview::sLabel = temp;

	else
		ILSReqReview::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSReqReview::sTriggerLowPriority = true;

		else
			ILSReqReview::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSReqReview::sDoNotCall = true;

		else
			ILSReqReview::sDoNotCall = false;
	}


	int N = ILSReqReview::sMessageList.Entries ();

	if (N == 1)
		N++;

	ILSReqReview::sMessageArray = new RGString* [N];
	RGDListIterator it (ILSReqReview::sMessageList);

	for (int i=0; i<N; i++)
		ILSReqReview::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ILSReqReview :: GetSubject () const {

	return ILSReqReview::sSubject;
}


RGString ILSReqReview :: GetLabel () const {

	return ILSReqReview::sLabel;
}


bool ILSReqReview :: IsTriggerForLowPriority () const {

	return ILSReqReview::sTriggerLowPriority;
}


bool ILSReqReview :: IsDoNotCall () const {

	return ILSReqReview::sDoNotCall;
}



RGString ILSReqReview :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ILSReqReview::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ILSReqReview::sMessageArray [i];
	return returnValue;
}



int ILSReqReview :: GetMessageLevel () const {

	return ILSReqReview::sMessageLevel;
}



int ILSReqReview :: GetNumberOfMessageParts () const {

	return ILSReqReview::sMessageList.Entries ();
}


int ILSReqReview :: GetSeverityLevel () const {

	return ILSReqReview::sMessageLevel;
}


void ILSReqReview :: SetIndexAndScope (int index, int scope) const {

	ILSReqReview::sMessageIndex = index;
	ILSReqReview::sMessageScope = scope;
}


int ILSReqReview :: GetMessageIndex () const {

	return ILSReqReview :: sMessageIndex;
}



int ILSReqReview :: GetScope () const {

	return ILSReqReview :: sMessageScope;
}



int ILSReqReview :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ILSReqReview* noticeType = new ILSReqReview;
	warehouse->AddType (noticeType);
	return 1;
}





LadderReqReview :: LadderReqReview () : Notice () {

}


LadderReqReview :: LadderReqReview (const LadderReqReview& note) : Notice ((const Notice&) note) {

}



LadderReqReview :: ~LadderReqReview () {

}



int LadderReqReview :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LadderReqReview::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LadderReqReview::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LadderReqReview::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LadderReqReview::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LadderReqReview::sLabel = temp;

	else
		LadderReqReview::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LadderReqReview::sTriggerLowPriority = true;

		else
			LadderReqReview::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LadderReqReview::sDoNotCall = true;

		else
			LadderReqReview::sDoNotCall = false;
	}


	int N = LadderReqReview::sMessageList.Entries ();

	if (N == 1)
		N++;

	LadderReqReview::sMessageArray = new RGString* [N];
	RGDListIterator it (LadderReqReview::sMessageList);

	for (int i=0; i<N; i++)
		LadderReqReview::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LadderReqReview :: GetSubject () const {

	return LadderReqReview::sSubject;
}


RGString LadderReqReview :: GetLabel () const {

	return LadderReqReview::sLabel;
}


bool LadderReqReview :: IsTriggerForLowPriority () const {

	return LadderReqReview::sTriggerLowPriority;
}


bool LadderReqReview :: IsDoNotCall () const {

	return LadderReqReview::sDoNotCall;
}



RGString LadderReqReview :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LadderReqReview::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LadderReqReview::sMessageArray [i];
	return returnValue;
}



int LadderReqReview :: GetMessageLevel () const {

	return LadderReqReview::sMessageLevel;
}



int LadderReqReview :: GetNumberOfMessageParts () const {

	return LadderReqReview::sMessageList.Entries ();
}


int LadderReqReview :: GetSeverityLevel () const {

	return LadderReqReview::sMessageLevel;
}


void LadderReqReview :: SetIndexAndScope (int index, int scope) const {

	LadderReqReview::sMessageIndex = index;
	LadderReqReview::sMessageScope = scope;
}


int LadderReqReview :: GetMessageIndex () const {

	return LadderReqReview :: sMessageIndex;
}



int LadderReqReview :: GetScope () const {

	return LadderReqReview :: sMessageScope;
}



int LadderReqReview :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LadderReqReview* noticeType = new LadderReqReview;
	warehouse->AddType (noticeType);
	return 1;
}





ILSSpacing :: ILSSpacing () : Notice () {

}


ILSSpacing :: ILSSpacing (const ILSSpacing& note) : Notice ((const Notice&) note) {

}



ILSSpacing :: ~ILSSpacing () {

}



int ILSSpacing :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ILSSpacing::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ILSSpacing::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSSpacing::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSSpacing::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ILSSpacing::sLabel = temp;

	else
		ILSSpacing::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSSpacing::sTriggerLowPriority = true;

		else
			ILSSpacing::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSSpacing::sDoNotCall = true;

		else
			ILSSpacing::sDoNotCall = false;
	}


	int N = ILSSpacing::sMessageList.Entries ();

	if (N == 1)
		N++;

	ILSSpacing::sMessageArray = new RGString* [N];
	RGDListIterator it (ILSSpacing::sMessageList);

	for (int i=0; i<N; i++)
		ILSSpacing::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ILSSpacing :: GetSubject () const {

	return ILSSpacing::sSubject;
}


RGString ILSSpacing :: GetLabel () const {

	return ILSSpacing::sLabel;
}


bool ILSSpacing :: IsTriggerForLowPriority () const {

	return ILSSpacing::sTriggerLowPriority;
}


bool ILSSpacing :: IsDoNotCall () const {

	return ILSSpacing::sDoNotCall;
}



RGString ILSSpacing :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ILSSpacing::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ILSSpacing::sMessageArray [i];
	return returnValue;
}



int ILSSpacing :: GetMessageLevel () const {

	return ILSSpacing::sMessageLevel;
}



int ILSSpacing :: GetNumberOfMessageParts () const {

	return ILSSpacing::sMessageList.Entries ();
}


int ILSSpacing :: GetSeverityLevel () const {

	return ILSSpacing::sMessageLevel;
}


void ILSSpacing :: SetIndexAndScope (int index, int scope) const {

	ILSSpacing::sMessageIndex = index;
	ILSSpacing::sMessageScope = scope;
}


int ILSSpacing :: GetMessageIndex () const {

	return ILSSpacing :: sMessageIndex;
}



int ILSSpacing :: GetScope () const {

	return ILSSpacing :: sMessageScope;
}



int ILSSpacing :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ILSSpacing* noticeType = new ILSSpacing;
	warehouse->AddType (noticeType);
	return 1;
}





LocusSpacing :: LocusSpacing () : Notice () {

}


LocusSpacing :: LocusSpacing (const LocusSpacing& note) : Notice ((const Notice&) note) {

}



LocusSpacing :: ~LocusSpacing () {

}



int LocusSpacing :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LocusSpacing::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LocusSpacing::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LocusSpacing::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LocusSpacing::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LocusSpacing::sLabel = temp;

	else
		LocusSpacing::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LocusSpacing::sTriggerLowPriority = true;

		else
			LocusSpacing::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LocusSpacing::sDoNotCall = true;

		else
			LocusSpacing::sDoNotCall = false;
	}


	int N = LocusSpacing::sMessageList.Entries ();

	if (N == 1)
		N++;

	LocusSpacing::sMessageArray = new RGString* [N];
	RGDListIterator it (LocusSpacing::sMessageList);

	for (int i=0; i<N; i++)
		LocusSpacing::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LocusSpacing :: GetSubject () const {

	return LocusSpacing::sSubject;
}


RGString LocusSpacing :: GetLabel () const {

	return LocusSpacing::sLabel;
}


bool LocusSpacing :: IsTriggerForLowPriority () const {

	return LocusSpacing::sTriggerLowPriority;
}


bool LocusSpacing :: IsDoNotCall () const {

	return LocusSpacing::sDoNotCall;
}



RGString LocusSpacing :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LocusSpacing::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LocusSpacing::sMessageArray [i];
	return returnValue;
}



int LocusSpacing :: GetMessageLevel () const {

	return LocusSpacing::sMessageLevel;
}



int LocusSpacing :: GetNumberOfMessageParts () const {

	return LocusSpacing::sMessageList.Entries ();
}


int LocusSpacing :: GetSeverityLevel () const {

	return LocusSpacing::sMessageLevel;
}


void LocusSpacing :: SetIndexAndScope (int index, int scope) const {

	LocusSpacing::sMessageIndex = index;
	LocusSpacing::sMessageScope = scope;
}


int LocusSpacing :: GetMessageIndex () const {

	return LocusSpacing :: sMessageIndex;
}



int LocusSpacing :: GetScope () const {

	return LocusSpacing :: sMessageScope;
}



int LocusSpacing :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LocusSpacing* noticeType = new LocusSpacing;
	warehouse->AddType (noticeType);
	return 1;
}





NoLadderFile :: NoLadderFile () : Notice () {

}


NoLadderFile :: NoLadderFile (const NoLadderFile& note) : Notice ((const Notice&) note) {

}



NoLadderFile :: ~NoLadderFile () {

}



int NoLadderFile :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoLadderFile::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoLadderFile::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoLadderFile::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoLadderFile::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoLadderFile::sLabel = temp;

	else
		NoLadderFile::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoLadderFile::sTriggerLowPriority = true;

		else
			NoLadderFile::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoLadderFile::sDoNotCall = true;

		else
			NoLadderFile::sDoNotCall = false;
	}


	int N = NoLadderFile::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoLadderFile::sMessageArray = new RGString* [N];
	RGDListIterator it (NoLadderFile::sMessageList);

	for (int i=0; i<N; i++)
		NoLadderFile::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoLadderFile :: GetSubject () const {

	return NoLadderFile::sSubject;
}


RGString NoLadderFile :: GetLabel () const {

	return NoLadderFile::sLabel;
}


bool NoLadderFile :: IsTriggerForLowPriority () const {

	return NoLadderFile::sTriggerLowPriority;
}


bool NoLadderFile :: IsDoNotCall () const {

	return NoLadderFile::sDoNotCall;
}



RGString NoLadderFile :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoLadderFile::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoLadderFile::sMessageArray [i];
	return returnValue;
}



int NoLadderFile :: GetMessageLevel () const {

	return NoLadderFile::sMessageLevel;
}



int NoLadderFile :: GetNumberOfMessageParts () const {

	return NoLadderFile::sMessageList.Entries ();
}


int NoLadderFile :: GetSeverityLevel () const {

	return NoLadderFile::sMessageLevel;
}


void NoLadderFile :: SetIndexAndScope (int index, int scope) const {

	NoLadderFile::sMessageIndex = index;
	NoLadderFile::sMessageScope = scope;
}


int NoLadderFile :: GetMessageIndex () const {

	return NoLadderFile :: sMessageIndex;
}



int NoLadderFile :: GetScope () const {

	return NoLadderFile :: sMessageScope;
}



int NoLadderFile :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoLadderFile* noticeType = new NoLadderFile;
	warehouse->AddType (noticeType);
	return 1;
}





LadderTypeUnknown :: LadderTypeUnknown () : Notice () {

}


LadderTypeUnknown :: LadderTypeUnknown (const LadderTypeUnknown& note) : Notice ((const Notice&) note) {

}



LadderTypeUnknown :: ~LadderTypeUnknown () {

}



int LadderTypeUnknown :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LadderTypeUnknown::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LadderTypeUnknown::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LadderTypeUnknown::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LadderTypeUnknown::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LadderTypeUnknown::sLabel = temp;

	else
		LadderTypeUnknown::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LadderTypeUnknown::sTriggerLowPriority = true;

		else
			LadderTypeUnknown::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LadderTypeUnknown::sDoNotCall = true;

		else
			LadderTypeUnknown::sDoNotCall = false;
	}


	int N = LadderTypeUnknown::sMessageList.Entries ();

	if (N == 1)
		N++;

	LadderTypeUnknown::sMessageArray = new RGString* [N];
	RGDListIterator it (LadderTypeUnknown::sMessageList);

	for (int i=0; i<N; i++)
		LadderTypeUnknown::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LadderTypeUnknown :: GetSubject () const {

	return LadderTypeUnknown::sSubject;
}


RGString LadderTypeUnknown :: GetLabel () const {

	return LadderTypeUnknown::sLabel;
}


bool LadderTypeUnknown :: IsTriggerForLowPriority () const {

	return LadderTypeUnknown::sTriggerLowPriority;
}


bool LadderTypeUnknown :: IsDoNotCall () const {

	return LadderTypeUnknown::sDoNotCall;
}



RGString LadderTypeUnknown :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LadderTypeUnknown::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LadderTypeUnknown::sMessageArray [i];
	return returnValue;
}



int LadderTypeUnknown :: GetMessageLevel () const {

	return LadderTypeUnknown::sMessageLevel;
}



int LadderTypeUnknown :: GetNumberOfMessageParts () const {

	return LadderTypeUnknown::sMessageList.Entries ();
}


int LadderTypeUnknown :: GetSeverityLevel () const {

	return LadderTypeUnknown::sMessageLevel;
}


void LadderTypeUnknown :: SetIndexAndScope (int index, int scope) const {

	LadderTypeUnknown::sMessageIndex = index;
	LadderTypeUnknown::sMessageScope = scope;
}


int LadderTypeUnknown :: GetMessageIndex () const {

	return LadderTypeUnknown :: sMessageIndex;
}



int LadderTypeUnknown :: GetScope () const {

	return LadderTypeUnknown :: sMessageScope;
}



int LadderTypeUnknown :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LadderTypeUnknown* noticeType = new LadderTypeUnknown;
	warehouse->AddType (noticeType);
	return 1;
}





ReagentKitUnknown :: ReagentKitUnknown () : Notice () {

}


ReagentKitUnknown :: ReagentKitUnknown (const ReagentKitUnknown& note) : Notice ((const Notice&) note) {

}



ReagentKitUnknown :: ~ReagentKitUnknown () {

}



int ReagentKitUnknown :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ReagentKitUnknown::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ReagentKitUnknown::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ReagentKitUnknown::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ReagentKitUnknown::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ReagentKitUnknown::sLabel = temp;

	else
		ReagentKitUnknown::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ReagentKitUnknown::sTriggerLowPriority = true;

		else
			ReagentKitUnknown::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ReagentKitUnknown::sDoNotCall = true;

		else
			ReagentKitUnknown::sDoNotCall = false;
	}


	int N = ReagentKitUnknown::sMessageList.Entries ();

	if (N == 1)
		N++;

	ReagentKitUnknown::sMessageArray = new RGString* [N];
	RGDListIterator it (ReagentKitUnknown::sMessageList);

	for (int i=0; i<N; i++)
		ReagentKitUnknown::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ReagentKitUnknown :: GetSubject () const {

	return ReagentKitUnknown::sSubject;
}


RGString ReagentKitUnknown :: GetLabel () const {

	return ReagentKitUnknown::sLabel;
}


bool ReagentKitUnknown :: IsTriggerForLowPriority () const {

	return ReagentKitUnknown::sTriggerLowPriority;
}


bool ReagentKitUnknown :: IsDoNotCall () const {

	return ReagentKitUnknown::sDoNotCall;
}



RGString ReagentKitUnknown :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ReagentKitUnknown::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ReagentKitUnknown::sMessageArray [i];
	return returnValue;
}



int ReagentKitUnknown :: GetMessageLevel () const {

	return ReagentKitUnknown::sMessageLevel;
}



int ReagentKitUnknown :: GetNumberOfMessageParts () const {

	return ReagentKitUnknown::sMessageList.Entries ();
}


int ReagentKitUnknown :: GetSeverityLevel () const {

	return ReagentKitUnknown::sMessageLevel;
}


void ReagentKitUnknown :: SetIndexAndScope (int index, int scope) const {

	ReagentKitUnknown::sMessageIndex = index;
	ReagentKitUnknown::sMessageScope = scope;
}


int ReagentKitUnknown :: GetMessageIndex () const {

	return ReagentKitUnknown :: sMessageIndex;
}



int ReagentKitUnknown :: GetScope () const {

	return ReagentKitUnknown :: sMessageScope;
}



int ReagentKitUnknown :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ReagentKitUnknown* noticeType = new ReagentKitUnknown;
	warehouse->AddType (noticeType);
	return 1;
}





TooManyReagentKits :: TooManyReagentKits () : Notice () {

}


TooManyReagentKits :: TooManyReagentKits (const TooManyReagentKits& note) : Notice ((const Notice&) note) {

}



TooManyReagentKits :: ~TooManyReagentKits () {

}



int TooManyReagentKits :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (TooManyReagentKits::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		TooManyReagentKits::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	TooManyReagentKits::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	TooManyReagentKits::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		TooManyReagentKits::sLabel = temp;

	else
		TooManyReagentKits::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			TooManyReagentKits::sTriggerLowPriority = true;

		else
			TooManyReagentKits::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			TooManyReagentKits::sDoNotCall = true;

		else
			TooManyReagentKits::sDoNotCall = false;
	}


	int N = TooManyReagentKits::sMessageList.Entries ();

	if (N == 1)
		N++;

	TooManyReagentKits::sMessageArray = new RGString* [N];
	RGDListIterator it (TooManyReagentKits::sMessageList);

	for (int i=0; i<N; i++)
		TooManyReagentKits::sMessageArray [i] = (RGString*) it ();

	return status;
}


int TooManyReagentKits :: GetSubject () const {

	return TooManyReagentKits::sSubject;
}


RGString TooManyReagentKits :: GetLabel () const {

	return TooManyReagentKits::sLabel;
}


bool TooManyReagentKits :: IsTriggerForLowPriority () const {

	return TooManyReagentKits::sTriggerLowPriority;
}


bool TooManyReagentKits :: IsDoNotCall () const {

	return TooManyReagentKits::sDoNotCall;
}



RGString TooManyReagentKits :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= TooManyReagentKits::sMessageList.Entries ()))
		return returnValue;

	returnValue = *TooManyReagentKits::sMessageArray [i];
	return returnValue;
}



int TooManyReagentKits :: GetMessageLevel () const {

	return TooManyReagentKits::sMessageLevel;
}



int TooManyReagentKits :: GetNumberOfMessageParts () const {

	return TooManyReagentKits::sMessageList.Entries ();
}


int TooManyReagentKits :: GetSeverityLevel () const {

	return TooManyReagentKits::sMessageLevel;
}


void TooManyReagentKits :: SetIndexAndScope (int index, int scope) const {

	TooManyReagentKits::sMessageIndex = index;
	TooManyReagentKits::sMessageScope = scope;
}


int TooManyReagentKits :: GetMessageIndex () const {

	return TooManyReagentKits :: sMessageIndex;
}



int TooManyReagentKits :: GetScope () const {

	return TooManyReagentKits :: sMessageScope;
}



int TooManyReagentKits :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	TooManyReagentKits* noticeType = new TooManyReagentKits;
	warehouse->AddType (noticeType);
	return 1;
}





NoPosCtrl :: NoPosCtrl () : Notice () {

}


NoPosCtrl :: NoPosCtrl (const NoPosCtrl& note) : Notice ((const Notice&) note) {

}



NoPosCtrl :: ~NoPosCtrl () {

}



int NoPosCtrl :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoPosCtrl::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoPosCtrl::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoPosCtrl::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoPosCtrl::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoPosCtrl::sLabel = temp;

	else
		NoPosCtrl::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoPosCtrl::sTriggerLowPriority = true;

		else
			NoPosCtrl::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoPosCtrl::sDoNotCall = true;

		else
			NoPosCtrl::sDoNotCall = false;
	}


	int N = NoPosCtrl::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoPosCtrl::sMessageArray = new RGString* [N];
	RGDListIterator it (NoPosCtrl::sMessageList);

	for (int i=0; i<N; i++)
		NoPosCtrl::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoPosCtrl :: GetSubject () const {

	return NoPosCtrl::sSubject;
}


RGString NoPosCtrl :: GetLabel () const {

	return NoPosCtrl::sLabel;
}


bool NoPosCtrl :: IsTriggerForLowPriority () const {

	return NoPosCtrl::sTriggerLowPriority;
}


bool NoPosCtrl :: IsDoNotCall () const {

	return NoPosCtrl::sDoNotCall;
}



RGString NoPosCtrl :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoPosCtrl::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoPosCtrl::sMessageArray [i];
	return returnValue;
}



int NoPosCtrl :: GetMessageLevel () const {

	return NoPosCtrl::sMessageLevel;
}



int NoPosCtrl :: GetNumberOfMessageParts () const {

	return NoPosCtrl::sMessageList.Entries ();
}


int NoPosCtrl :: GetSeverityLevel () const {

	return NoPosCtrl::sMessageLevel;
}


void NoPosCtrl :: SetIndexAndScope (int index, int scope) const {

	NoPosCtrl::sMessageIndex = index;
	NoPosCtrl::sMessageScope = scope;
}


int NoPosCtrl :: GetMessageIndex () const {

	return NoPosCtrl :: sMessageIndex;
}



int NoPosCtrl :: GetScope () const {

	return NoPosCtrl :: sMessageScope;
}



int NoPosCtrl :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoPosCtrl* noticeType = new NoPosCtrl;
	warehouse->AddType (noticeType);
	return 1;
}





NoNegCtrl :: NoNegCtrl () : Notice () {

}


NoNegCtrl :: NoNegCtrl (const NoNegCtrl& note) : Notice ((const Notice&) note) {

}



NoNegCtrl :: ~NoNegCtrl () {

}



int NoNegCtrl :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoNegCtrl::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoNegCtrl::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoNegCtrl::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoNegCtrl::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoNegCtrl::sLabel = temp;

	else
		NoNegCtrl::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoNegCtrl::sTriggerLowPriority = true;

		else
			NoNegCtrl::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoNegCtrl::sDoNotCall = true;

		else
			NoNegCtrl::sDoNotCall = false;
	}


	int N = NoNegCtrl::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoNegCtrl::sMessageArray = new RGString* [N];
	RGDListIterator it (NoNegCtrl::sMessageList);

	for (int i=0; i<N; i++)
		NoNegCtrl::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoNegCtrl :: GetSubject () const {

	return NoNegCtrl::sSubject;
}


RGString NoNegCtrl :: GetLabel () const {

	return NoNegCtrl::sLabel;
}


bool NoNegCtrl :: IsTriggerForLowPriority () const {

	return NoNegCtrl::sTriggerLowPriority;
}


bool NoNegCtrl :: IsDoNotCall () const {

	return NoNegCtrl::sDoNotCall;
}



RGString NoNegCtrl :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoNegCtrl::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoNegCtrl::sMessageArray [i];
	return returnValue;
}



int NoNegCtrl :: GetMessageLevel () const {

	return NoNegCtrl::sMessageLevel;
}



int NoNegCtrl :: GetNumberOfMessageParts () const {

	return NoNegCtrl::sMessageList.Entries ();
}


int NoNegCtrl :: GetSeverityLevel () const {

	return NoNegCtrl::sMessageLevel;
}


void NoNegCtrl :: SetIndexAndScope (int index, int scope) const {

	NoNegCtrl::sMessageIndex = index;
	NoNegCtrl::sMessageScope = scope;
}


int NoNegCtrl :: GetMessageIndex () const {

	return NoNegCtrl :: sMessageIndex;
}



int NoNegCtrl :: GetScope () const {

	return NoNegCtrl :: sMessageScope;
}



int NoNegCtrl :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoNegCtrl* noticeType = new NoNegCtrl;
	warehouse->AddType (noticeType);
	return 1;
}





NoSample :: NoSample () : Notice () {

}


NoSample :: NoSample (const NoSample& note) : Notice ((const Notice&) note) {

}



NoSample :: ~NoSample () {

}



int NoSample :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoSample::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoSample::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoSample::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoSample::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoSample::sLabel = temp;

	else
		NoSample::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoSample::sTriggerLowPriority = true;

		else
			NoSample::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoSample::sDoNotCall = true;

		else
			NoSample::sDoNotCall = false;
	}


	int N = NoSample::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoSample::sMessageArray = new RGString* [N];
	RGDListIterator it (NoSample::sMessageList);

	for (int i=0; i<N; i++)
		NoSample::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoSample :: GetSubject () const {

	return NoSample::sSubject;
}


RGString NoSample :: GetLabel () const {

	return NoSample::sLabel;
}


bool NoSample :: IsTriggerForLowPriority () const {

	return NoSample::sTriggerLowPriority;
}


bool NoSample :: IsDoNotCall () const {

	return NoSample::sDoNotCall;
}



RGString NoSample :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoSample::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoSample::sMessageArray [i];
	return returnValue;
}



int NoSample :: GetMessageLevel () const {

	return NoSample::sMessageLevel;
}



int NoSample :: GetNumberOfMessageParts () const {

	return NoSample::sMessageList.Entries ();
}


int NoSample :: GetSeverityLevel () const {

	return NoSample::sMessageLevel;
}


void NoSample :: SetIndexAndScope (int index, int scope) const {

	NoSample::sMessageIndex = index;
	NoSample::sMessageScope = scope;
}


int NoSample :: GetMessageIndex () const {

	return NoSample :: sMessageIndex;
}



int NoSample :: GetScope () const {

	return NoSample :: sMessageScope;
}



int NoSample :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoSample* noticeType = new NoSample;
	warehouse->AddType (noticeType);
	return 1;
}





CurveFitTooLow :: CurveFitTooLow () : Notice () {

}


CurveFitTooLow :: CurveFitTooLow (const CurveFitTooLow& note) : Notice ((const Notice&) note) {

}



CurveFitTooLow :: ~CurveFitTooLow () {

}



int CurveFitTooLow :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (CurveFitTooLow::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		CurveFitTooLow::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CurveFitTooLow::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CurveFitTooLow::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		CurveFitTooLow::sLabel = temp;

	else
		CurveFitTooLow::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CurveFitTooLow::sTriggerLowPriority = true;

		else
			CurveFitTooLow::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CurveFitTooLow::sDoNotCall = true;

		else
			CurveFitTooLow::sDoNotCall = false;
	}


	int N = CurveFitTooLow::sMessageList.Entries ();

	if (N == 1)
		N++;

	CurveFitTooLow::sMessageArray = new RGString* [N];
	RGDListIterator it (CurveFitTooLow::sMessageList);

	for (int i=0; i<N; i++)
		CurveFitTooLow::sMessageArray [i] = (RGString*) it ();

	return status;
}


int CurveFitTooLow :: GetSubject () const {

	return CurveFitTooLow::sSubject;
}


RGString CurveFitTooLow :: GetLabel () const {

	return CurveFitTooLow::sLabel;
}


bool CurveFitTooLow :: IsTriggerForLowPriority () const {

	return CurveFitTooLow::sTriggerLowPriority;
}


bool CurveFitTooLow :: IsDoNotCall () const {

	return CurveFitTooLow::sDoNotCall;
}



RGString CurveFitTooLow :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= CurveFitTooLow::sMessageList.Entries ()))
		return returnValue;

	returnValue = *CurveFitTooLow::sMessageArray [i];
	return returnValue;
}



int CurveFitTooLow :: GetMessageLevel () const {

	return CurveFitTooLow::sMessageLevel;
}



int CurveFitTooLow :: GetNumberOfMessageParts () const {

	return CurveFitTooLow::sMessageList.Entries ();
}


int CurveFitTooLow :: GetSeverityLevel () const {

	return CurveFitTooLow::sMessageLevel;
}


void CurveFitTooLow :: SetIndexAndScope (int index, int scope) const {

	CurveFitTooLow::sMessageIndex = index;
	CurveFitTooLow::sMessageScope = scope;
}


int CurveFitTooLow :: GetMessageIndex () const {

	return CurveFitTooLow :: sMessageIndex;
}



int CurveFitTooLow :: GetScope () const {

	return CurveFitTooLow :: sMessageScope;
}



int CurveFitTooLow :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	CurveFitTooLow* noticeType = new CurveFitTooLow;
	warehouse->AddType (noticeType);
	return 1;
}





CurveFitMarginal :: CurveFitMarginal () : Notice () {

}


CurveFitMarginal :: CurveFitMarginal (const CurveFitMarginal& note) : Notice ((const Notice&) note) {

}



CurveFitMarginal :: ~CurveFitMarginal () {

}



int CurveFitMarginal :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (CurveFitMarginal::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		CurveFitMarginal::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CurveFitMarginal::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CurveFitMarginal::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		CurveFitMarginal::sLabel = temp;

	else
		CurveFitMarginal::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CurveFitMarginal::sTriggerLowPriority = true;

		else
			CurveFitMarginal::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CurveFitMarginal::sDoNotCall = true;

		else
			CurveFitMarginal::sDoNotCall = false;
	}


	int N = CurveFitMarginal::sMessageList.Entries ();

	if (N == 1)
		N++;

	CurveFitMarginal::sMessageArray = new RGString* [N];
	RGDListIterator it (CurveFitMarginal::sMessageList);

	for (int i=0; i<N; i++)
		CurveFitMarginal::sMessageArray [i] = (RGString*) it ();

	return status;
}


int CurveFitMarginal :: GetSubject () const {

	return CurveFitMarginal::sSubject;
}


RGString CurveFitMarginal :: GetLabel () const {

	return CurveFitMarginal::sLabel;
}


bool CurveFitMarginal :: IsTriggerForLowPriority () const {

	return CurveFitMarginal::sTriggerLowPriority;
}


bool CurveFitMarginal :: IsDoNotCall () const {

	return CurveFitMarginal::sDoNotCall;
}



RGString CurveFitMarginal :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= CurveFitMarginal::sMessageList.Entries ()))
		return returnValue;

	returnValue = *CurveFitMarginal::sMessageArray [i];
	return returnValue;
}



int CurveFitMarginal :: GetMessageLevel () const {

	return CurveFitMarginal::sMessageLevel;
}



int CurveFitMarginal :: GetNumberOfMessageParts () const {

	return CurveFitMarginal::sMessageList.Entries ();
}


int CurveFitMarginal :: GetSeverityLevel () const {

	return CurveFitMarginal::sMessageLevel;
}


void CurveFitMarginal :: SetIndexAndScope (int index, int scope) const {

	CurveFitMarginal::sMessageIndex = index;
	CurveFitMarginal::sMessageScope = scope;
}


int CurveFitMarginal :: GetMessageIndex () const {

	return CurveFitMarginal :: sMessageIndex;
}



int CurveFitMarginal :: GetScope () const {

	return CurveFitMarginal :: sMessageScope;
}



int CurveFitMarginal :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	CurveFitMarginal* noticeType = new CurveFitMarginal;
	warehouse->AddType (noticeType);
	return 1;
}





ArtifactFound :: ArtifactFound () : Notice () {

}


ArtifactFound :: ArtifactFound (const ArtifactFound& note) : Notice ((const Notice&) note) {

}



ArtifactFound :: ~ArtifactFound () {

}



int ArtifactFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ArtifactFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ArtifactFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ArtifactFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ArtifactFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ArtifactFound::sLabel = temp;

	else
		ArtifactFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ArtifactFound::sTriggerLowPriority = true;

		else
			ArtifactFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ArtifactFound::sDoNotCall = true;

		else
			ArtifactFound::sDoNotCall = false;
	}


	int N = ArtifactFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	ArtifactFound::sMessageArray = new RGString* [N];
	RGDListIterator it (ArtifactFound::sMessageList);

	for (int i=0; i<N; i++)
		ArtifactFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ArtifactFound :: GetSubject () const {

	return ArtifactFound::sSubject;
}


RGString ArtifactFound :: GetLabel () const {

	return ArtifactFound::sLabel;
}


bool ArtifactFound :: IsTriggerForLowPriority () const {

	return ArtifactFound::sTriggerLowPriority;
}


bool ArtifactFound :: IsDoNotCall () const {

	return ArtifactFound::sDoNotCall;
}



RGString ArtifactFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ArtifactFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ArtifactFound::sMessageArray [i];
	return returnValue;
}



int ArtifactFound :: GetMessageLevel () const {

	return ArtifactFound::sMessageLevel;
}



int ArtifactFound :: GetNumberOfMessageParts () const {

	return ArtifactFound::sMessageList.Entries ();
}


int ArtifactFound :: GetSeverityLevel () const {

	return ArtifactFound::sMessageLevel;
}


void ArtifactFound :: SetIndexAndScope (int index, int scope) const {

	ArtifactFound::sMessageIndex = index;
	ArtifactFound::sMessageScope = scope;
}


int ArtifactFound :: GetMessageIndex () const {

	return ArtifactFound :: sMessageIndex;
}



int ArtifactFound :: GetScope () const {

	return ArtifactFound :: sMessageScope;
}



int ArtifactFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ArtifactFound* noticeType = new ArtifactFound;
	warehouse->AddType (noticeType);
	return 1;
}





SpikeFound :: SpikeFound () : Notice () {

}


SpikeFound :: SpikeFound (const SpikeFound& note) : Notice ((const Notice&) note) {

}



SpikeFound :: ~SpikeFound () {

}



int SpikeFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (SpikeFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		SpikeFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SpikeFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SpikeFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		SpikeFound::sLabel = temp;

	else
		SpikeFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SpikeFound::sTriggerLowPriority = true;

		else
			SpikeFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SpikeFound::sDoNotCall = true;

		else
			SpikeFound::sDoNotCall = false;
	}


	int N = SpikeFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	SpikeFound::sMessageArray = new RGString* [N];
	RGDListIterator it (SpikeFound::sMessageList);

	for (int i=0; i<N; i++)
		SpikeFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int SpikeFound :: GetSubject () const {

	return SpikeFound::sSubject;
}


RGString SpikeFound :: GetLabel () const {

	return SpikeFound::sLabel;
}


bool SpikeFound :: IsTriggerForLowPriority () const {

	return SpikeFound::sTriggerLowPriority;
}


bool SpikeFound :: IsDoNotCall () const {

	return SpikeFound::sDoNotCall;
}



RGString SpikeFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= SpikeFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *SpikeFound::sMessageArray [i];
	return returnValue;
}



int SpikeFound :: GetMessageLevel () const {

	return SpikeFound::sMessageLevel;
}



int SpikeFound :: GetNumberOfMessageParts () const {

	return SpikeFound::sMessageList.Entries ();
}


int SpikeFound :: GetSeverityLevel () const {

	return SpikeFound::sMessageLevel;
}


void SpikeFound :: SetIndexAndScope (int index, int scope) const {

	SpikeFound::sMessageIndex = index;
	SpikeFound::sMessageScope = scope;
}


int SpikeFound :: GetMessageIndex () const {

	return SpikeFound :: sMessageIndex;
}



int SpikeFound :: GetScope () const {

	return SpikeFound :: sMessageScope;
}



int SpikeFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	SpikeFound* noticeType = new SpikeFound;
	warehouse->AddType (noticeType);
	return 1;
}





StutterFound :: StutterFound () : Notice () {

}


StutterFound :: StutterFound (const StutterFound& note) : Notice ((const Notice&) note) {

}



StutterFound :: ~StutterFound () {

}



int StutterFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (StutterFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		StutterFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	StutterFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	StutterFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		StutterFound::sLabel = temp;

	else
		StutterFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			StutterFound::sTriggerLowPriority = true;

		else
			StutterFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			StutterFound::sDoNotCall = true;

		else
			StutterFound::sDoNotCall = false;
	}


	int N = StutterFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	StutterFound::sMessageArray = new RGString* [N];
	RGDListIterator it (StutterFound::sMessageList);

	for (int i=0; i<N; i++)
		StutterFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int StutterFound :: GetSubject () const {

	return StutterFound::sSubject;
}


RGString StutterFound :: GetLabel () const {

	return StutterFound::sLabel;
}


bool StutterFound :: IsTriggerForLowPriority () const {

	return StutterFound::sTriggerLowPriority;
}


bool StutterFound :: IsDoNotCall () const {

	return StutterFound::sDoNotCall;
}



RGString StutterFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= StutterFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *StutterFound::sMessageArray [i];
	return returnValue;
}



int StutterFound :: GetMessageLevel () const {

	return StutterFound::sMessageLevel;
}



int StutterFound :: GetNumberOfMessageParts () const {

	return StutterFound::sMessageList.Entries ();
}


int StutterFound :: GetSeverityLevel () const {

	return StutterFound::sMessageLevel;
}


void StutterFound :: SetIndexAndScope (int index, int scope) const {

	StutterFound::sMessageIndex = index;
	StutterFound::sMessageScope = scope;
}


int StutterFound :: GetMessageIndex () const {

	return StutterFound :: sMessageIndex;
}



int StutterFound :: GetScope () const {

	return StutterFound :: sMessageScope;
}



int StutterFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	StutterFound* noticeType = new StutterFound;
	warehouse->AddType (noticeType);
	return 1;
}





AdenylationFound :: AdenylationFound () : Notice () {

}


AdenylationFound :: AdenylationFound (const AdenylationFound& note) : Notice ((const Notice&) note) {

}



AdenylationFound :: ~AdenylationFound () {

}



int AdenylationFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (AdenylationFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		AdenylationFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	AdenylationFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	AdenylationFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		AdenylationFound::sLabel = temp;

	else
		AdenylationFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			AdenylationFound::sTriggerLowPriority = true;

		else
			AdenylationFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			AdenylationFound::sDoNotCall = true;

		else
			AdenylationFound::sDoNotCall = false;
	}


	int N = AdenylationFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	AdenylationFound::sMessageArray = new RGString* [N];
	RGDListIterator it (AdenylationFound::sMessageList);

	for (int i=0; i<N; i++)
		AdenylationFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int AdenylationFound :: GetSubject () const {

	return AdenylationFound::sSubject;
}


RGString AdenylationFound :: GetLabel () const {

	return AdenylationFound::sLabel;
}


bool AdenylationFound :: IsTriggerForLowPriority () const {

	return AdenylationFound::sTriggerLowPriority;
}


bool AdenylationFound :: IsDoNotCall () const {

	return AdenylationFound::sDoNotCall;
}



RGString AdenylationFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= AdenylationFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *AdenylationFound::sMessageArray [i];
	return returnValue;
}



int AdenylationFound :: GetMessageLevel () const {

	return AdenylationFound::sMessageLevel;
}



int AdenylationFound :: GetNumberOfMessageParts () const {

	return AdenylationFound::sMessageList.Entries ();
}


int AdenylationFound :: GetSeverityLevel () const {

	return AdenylationFound::sMessageLevel;
}


void AdenylationFound :: SetIndexAndScope (int index, int scope) const {

	AdenylationFound::sMessageIndex = index;
	AdenylationFound::sMessageScope = scope;
}


int AdenylationFound :: GetMessageIndex () const {

	return AdenylationFound :: sMessageIndex;
}



int AdenylationFound :: GetScope () const {

	return AdenylationFound :: sMessageScope;
}



int AdenylationFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	AdenylationFound* noticeType = new AdenylationFound;
	warehouse->AddType (noticeType);
	return 1;
}





BlobFound :: BlobFound () : Notice () {

}


BlobFound :: BlobFound (const BlobFound& note) : Notice ((const Notice&) note) {

}



BlobFound :: ~BlobFound () {

}



int BlobFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (BlobFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		BlobFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	BlobFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	BlobFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		BlobFound::sLabel = temp;

	else
		BlobFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			BlobFound::sTriggerLowPriority = true;

		else
			BlobFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			BlobFound::sDoNotCall = true;

		else
			BlobFound::sDoNotCall = false;
	}


	int N = BlobFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	BlobFound::sMessageArray = new RGString* [N];
	RGDListIterator it (BlobFound::sMessageList);

	for (int i=0; i<N; i++)
		BlobFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int BlobFound :: GetSubject () const {

	return BlobFound::sSubject;
}


RGString BlobFound :: GetLabel () const {

	return BlobFound::sLabel;
}


bool BlobFound :: IsTriggerForLowPriority () const {

	return BlobFound::sTriggerLowPriority;
}


bool BlobFound :: IsDoNotCall () const {

	return BlobFound::sDoNotCall;
}



RGString BlobFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= BlobFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *BlobFound::sMessageArray [i];
	return returnValue;
}



int BlobFound :: GetMessageLevel () const {

	return BlobFound::sMessageLevel;
}



int BlobFound :: GetNumberOfMessageParts () const {

	return BlobFound::sMessageList.Entries ();
}


int BlobFound :: GetSeverityLevel () const {

	return BlobFound::sMessageLevel;
}


void BlobFound :: SetIndexAndScope (int index, int scope) const {

	BlobFound::sMessageIndex = index;
	BlobFound::sMessageScope = scope;
}


int BlobFound :: GetMessageIndex () const {

	return BlobFound :: sMessageIndex;
}



int BlobFound :: GetScope () const {

	return BlobFound :: sMessageScope;
}



int BlobFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	BlobFound* noticeType = new BlobFound;
	warehouse->AddType (noticeType);
	return 1;
}





PullUpFound :: PullUpFound () : Notice () {

}


PullUpFound :: PullUpFound (const PullUpFound& note) : Notice ((const Notice&) note) {

}



PullUpFound :: ~PullUpFound () {

}



int PullUpFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PullUpFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PullUpFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PullUpFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PullUpFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PullUpFound::sLabel = temp;

	else
		PullUpFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PullUpFound::sTriggerLowPriority = true;

		else
			PullUpFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PullUpFound::sDoNotCall = true;

		else
			PullUpFound::sDoNotCall = false;
	}


	int N = PullUpFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	PullUpFound::sMessageArray = new RGString* [N];
	RGDListIterator it (PullUpFound::sMessageList);

	for (int i=0; i<N; i++)
		PullUpFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PullUpFound :: GetSubject () const {

	return PullUpFound::sSubject;
}


RGString PullUpFound :: GetLabel () const {

	return PullUpFound::sLabel;
}


bool PullUpFound :: IsTriggerForLowPriority () const {

	return PullUpFound::sTriggerLowPriority;
}


bool PullUpFound :: IsDoNotCall () const {

	return PullUpFound::sDoNotCall;
}



RGString PullUpFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PullUpFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PullUpFound::sMessageArray [i];
	return returnValue;
}



int PullUpFound :: GetMessageLevel () const {

	return PullUpFound::sMessageLevel;
}



int PullUpFound :: GetNumberOfMessageParts () const {

	return PullUpFound::sMessageList.Entries ();
}


int PullUpFound :: GetSeverityLevel () const {

	return PullUpFound::sMessageLevel;
}


void PullUpFound :: SetIndexAndScope (int index, int scope) const {

	PullUpFound::sMessageIndex = index;
	PullUpFound::sMessageScope = scope;
}


int PullUpFound :: GetMessageIndex () const {

	return PullUpFound :: sMessageIndex;
}



int PullUpFound :: GetScope () const {

	return PullUpFound :: sMessageScope;
}



int PullUpFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PullUpFound* noticeType = new PullUpFound;
	warehouse->AddType (noticeType);
	return 1;
}





NoGenotypeFound :: NoGenotypeFound () : Notice () {

}


NoGenotypeFound :: NoGenotypeFound (const NoGenotypeFound& note) : Notice ((const Notice&) note) {

}



NoGenotypeFound :: ~NoGenotypeFound () {

}



int NoGenotypeFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoGenotypeFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoGenotypeFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoGenotypeFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoGenotypeFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoGenotypeFound::sLabel = temp;

	else
		NoGenotypeFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoGenotypeFound::sTriggerLowPriority = true;

		else
			NoGenotypeFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoGenotypeFound::sDoNotCall = true;

		else
			NoGenotypeFound::sDoNotCall = false;
	}


	int N = NoGenotypeFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoGenotypeFound::sMessageArray = new RGString* [N];
	RGDListIterator it (NoGenotypeFound::sMessageList);

	for (int i=0; i<N; i++)
		NoGenotypeFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoGenotypeFound :: GetSubject () const {

	return NoGenotypeFound::sSubject;
}


RGString NoGenotypeFound :: GetLabel () const {

	return NoGenotypeFound::sLabel;
}


bool NoGenotypeFound :: IsTriggerForLowPriority () const {

	return NoGenotypeFound::sTriggerLowPriority;
}


bool NoGenotypeFound :: IsDoNotCall () const {

	return NoGenotypeFound::sDoNotCall;
}



RGString NoGenotypeFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoGenotypeFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoGenotypeFound::sMessageArray [i];
	return returnValue;
}



int NoGenotypeFound :: GetMessageLevel () const {

	return NoGenotypeFound::sMessageLevel;
}



int NoGenotypeFound :: GetNumberOfMessageParts () const {

	return NoGenotypeFound::sMessageList.Entries ();
}


int NoGenotypeFound :: GetSeverityLevel () const {

	return NoGenotypeFound::sMessageLevel;
}


void NoGenotypeFound :: SetIndexAndScope (int index, int scope) const {

	NoGenotypeFound::sMessageIndex = index;
	NoGenotypeFound::sMessageScope = scope;
}


int NoGenotypeFound :: GetMessageIndex () const {

	return NoGenotypeFound :: sMessageIndex;
}



int NoGenotypeFound :: GetScope () const {

	return NoGenotypeFound :: sMessageScope;
}



int NoGenotypeFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoGenotypeFound* noticeType = new NoGenotypeFound;
	warehouse->AddType (noticeType);
	return 1;
}





ThreeAllelesFound :: ThreeAllelesFound () : Notice () {

}


ThreeAllelesFound :: ThreeAllelesFound (const ThreeAllelesFound& note) : Notice ((const Notice&) note) {

}



ThreeAllelesFound :: ~ThreeAllelesFound () {

}



int ThreeAllelesFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ThreeAllelesFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ThreeAllelesFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ThreeAllelesFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ThreeAllelesFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ThreeAllelesFound::sLabel = temp;

	else
		ThreeAllelesFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ThreeAllelesFound::sTriggerLowPriority = true;

		else
			ThreeAllelesFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ThreeAllelesFound::sDoNotCall = true;

		else
			ThreeAllelesFound::sDoNotCall = false;
	}


	int N = ThreeAllelesFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	ThreeAllelesFound::sMessageArray = new RGString* [N];
	RGDListIterator it (ThreeAllelesFound::sMessageList);

	for (int i=0; i<N; i++)
		ThreeAllelesFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ThreeAllelesFound :: GetSubject () const {

	return ThreeAllelesFound::sSubject;
}


RGString ThreeAllelesFound :: GetLabel () const {

	return ThreeAllelesFound::sLabel;
}


bool ThreeAllelesFound :: IsTriggerForLowPriority () const {

	return ThreeAllelesFound::sTriggerLowPriority;
}


bool ThreeAllelesFound :: IsDoNotCall () const {

	return ThreeAllelesFound::sDoNotCall;
}



RGString ThreeAllelesFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ThreeAllelesFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ThreeAllelesFound::sMessageArray [i];
	return returnValue;
}



int ThreeAllelesFound :: GetMessageLevel () const {

	return ThreeAllelesFound::sMessageLevel;
}



int ThreeAllelesFound :: GetNumberOfMessageParts () const {

	return ThreeAllelesFound::sMessageList.Entries ();
}


int ThreeAllelesFound :: GetSeverityLevel () const {

	return ThreeAllelesFound::sMessageLevel;
}


void ThreeAllelesFound :: SetIndexAndScope (int index, int scope) const {

	ThreeAllelesFound::sMessageIndex = index;
	ThreeAllelesFound::sMessageScope = scope;
}


int ThreeAllelesFound :: GetMessageIndex () const {

	return ThreeAllelesFound :: sMessageIndex;
}



int ThreeAllelesFound :: GetScope () const {

	return ThreeAllelesFound :: sMessageScope;
}



int ThreeAllelesFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ThreeAllelesFound* noticeType = new ThreeAllelesFound;
	warehouse->AddType (noticeType);
	return 1;
}





UnexpectedNumberOfPeaks :: UnexpectedNumberOfPeaks () : Notice () {

}


UnexpectedNumberOfPeaks :: UnexpectedNumberOfPeaks (const UnexpectedNumberOfPeaks& note) : Notice ((const Notice&) note) {

}



UnexpectedNumberOfPeaks :: ~UnexpectedNumberOfPeaks () {

}



int UnexpectedNumberOfPeaks :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (UnexpectedNumberOfPeaks::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		UnexpectedNumberOfPeaks::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	UnexpectedNumberOfPeaks::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	UnexpectedNumberOfPeaks::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		UnexpectedNumberOfPeaks::sLabel = temp;

	else
		UnexpectedNumberOfPeaks::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			UnexpectedNumberOfPeaks::sTriggerLowPriority = true;

		else
			UnexpectedNumberOfPeaks::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			UnexpectedNumberOfPeaks::sDoNotCall = true;

		else
			UnexpectedNumberOfPeaks::sDoNotCall = false;
	}


	int N = UnexpectedNumberOfPeaks::sMessageList.Entries ();

	if (N == 1)
		N++;

	UnexpectedNumberOfPeaks::sMessageArray = new RGString* [N];
	RGDListIterator it (UnexpectedNumberOfPeaks::sMessageList);

	for (int i=0; i<N; i++)
		UnexpectedNumberOfPeaks::sMessageArray [i] = (RGString*) it ();

	return status;
}


int UnexpectedNumberOfPeaks :: GetSubject () const {

	return UnexpectedNumberOfPeaks::sSubject;
}


RGString UnexpectedNumberOfPeaks :: GetLabel () const {

	return UnexpectedNumberOfPeaks::sLabel;
}


bool UnexpectedNumberOfPeaks :: IsTriggerForLowPriority () const {

	return UnexpectedNumberOfPeaks::sTriggerLowPriority;
}


bool UnexpectedNumberOfPeaks :: IsDoNotCall () const {

	return UnexpectedNumberOfPeaks::sDoNotCall;
}



RGString UnexpectedNumberOfPeaks :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= UnexpectedNumberOfPeaks::sMessageList.Entries ()))
		return returnValue;

	returnValue = *UnexpectedNumberOfPeaks::sMessageArray [i];
	return returnValue;
}



int UnexpectedNumberOfPeaks :: GetMessageLevel () const {

	return UnexpectedNumberOfPeaks::sMessageLevel;
}



int UnexpectedNumberOfPeaks :: GetNumberOfMessageParts () const {

	return UnexpectedNumberOfPeaks::sMessageList.Entries ();
}


int UnexpectedNumberOfPeaks :: GetSeverityLevel () const {

	return UnexpectedNumberOfPeaks::sMessageLevel;
}


void UnexpectedNumberOfPeaks :: SetIndexAndScope (int index, int scope) const {

	UnexpectedNumberOfPeaks::sMessageIndex = index;
	UnexpectedNumberOfPeaks::sMessageScope = scope;
}


int UnexpectedNumberOfPeaks :: GetMessageIndex () const {

	return UnexpectedNumberOfPeaks :: sMessageIndex;
}



int UnexpectedNumberOfPeaks :: GetScope () const {

	return UnexpectedNumberOfPeaks :: sMessageScope;
}



int UnexpectedNumberOfPeaks :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	UnexpectedNumberOfPeaks* noticeType = new UnexpectedNumberOfPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





InterLocusPeak :: InterLocusPeak () : Notice () {

}


InterLocusPeak :: InterLocusPeak (const InterLocusPeak& note) : Notice ((const Notice&) note) {

}



InterLocusPeak :: ~InterLocusPeak () {

}



int InterLocusPeak :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (InterLocusPeak::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		InterLocusPeak::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterLocusPeak::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterLocusPeak::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		InterLocusPeak::sLabel = temp;

	else
		InterLocusPeak::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterLocusPeak::sTriggerLowPriority = true;

		else
			InterLocusPeak::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterLocusPeak::sDoNotCall = true;

		else
			InterLocusPeak::sDoNotCall = false;
	}


	int N = InterLocusPeak::sMessageList.Entries ();

	if (N == 1)
		N++;

	InterLocusPeak::sMessageArray = new RGString* [N];
	RGDListIterator it (InterLocusPeak::sMessageList);

	for (int i=0; i<N; i++)
		InterLocusPeak::sMessageArray [i] = (RGString*) it ();

	return status;
}


int InterLocusPeak :: GetSubject () const {

	return InterLocusPeak::sSubject;
}


RGString InterLocusPeak :: GetLabel () const {

	return InterLocusPeak::sLabel;
}


bool InterLocusPeak :: IsTriggerForLowPriority () const {

	return InterLocusPeak::sTriggerLowPriority;
}


bool InterLocusPeak :: IsDoNotCall () const {

	return InterLocusPeak::sDoNotCall;
}



RGString InterLocusPeak :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= InterLocusPeak::sMessageList.Entries ()))
		return returnValue;

	returnValue = *InterLocusPeak::sMessageArray [i];
	return returnValue;
}



int InterLocusPeak :: GetMessageLevel () const {

	return InterLocusPeak::sMessageLevel;
}



int InterLocusPeak :: GetNumberOfMessageParts () const {

	return InterLocusPeak::sMessageList.Entries ();
}


int InterLocusPeak :: GetSeverityLevel () const {

	return InterLocusPeak::sMessageLevel;
}


void InterLocusPeak :: SetIndexAndScope (int index, int scope) const {

	InterLocusPeak::sMessageIndex = index;
	InterLocusPeak::sMessageScope = scope;
}


int InterLocusPeak :: GetMessageIndex () const {

	return InterLocusPeak :: sMessageIndex;
}



int InterLocusPeak :: GetScope () const {

	return InterLocusPeak :: sMessageScope;
}



int InterLocusPeak :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	InterLocusPeak* noticeType = new InterLocusPeak;
	warehouse->AddType (noticeType);
	return 1;
}





HeteroImbalanceFound :: HeteroImbalanceFound () : Notice () {

}


HeteroImbalanceFound :: HeteroImbalanceFound (const HeteroImbalanceFound& note) : Notice ((const Notice&) note) {

}



HeteroImbalanceFound :: ~HeteroImbalanceFound () {

}



int HeteroImbalanceFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (HeteroImbalanceFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		HeteroImbalanceFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	HeteroImbalanceFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	HeteroImbalanceFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		HeteroImbalanceFound::sLabel = temp;

	else
		HeteroImbalanceFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			HeteroImbalanceFound::sTriggerLowPriority = true;

		else
			HeteroImbalanceFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			HeteroImbalanceFound::sDoNotCall = true;

		else
			HeteroImbalanceFound::sDoNotCall = false;
	}


	int N = HeteroImbalanceFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	HeteroImbalanceFound::sMessageArray = new RGString* [N];
	RGDListIterator it (HeteroImbalanceFound::sMessageList);

	for (int i=0; i<N; i++)
		HeteroImbalanceFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int HeteroImbalanceFound :: GetSubject () const {

	return HeteroImbalanceFound::sSubject;
}


RGString HeteroImbalanceFound :: GetLabel () const {

	return HeteroImbalanceFound::sLabel;
}


bool HeteroImbalanceFound :: IsTriggerForLowPriority () const {

	return HeteroImbalanceFound::sTriggerLowPriority;
}


bool HeteroImbalanceFound :: IsDoNotCall () const {

	return HeteroImbalanceFound::sDoNotCall;
}



RGString HeteroImbalanceFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= HeteroImbalanceFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *HeteroImbalanceFound::sMessageArray [i];
	return returnValue;
}



int HeteroImbalanceFound :: GetMessageLevel () const {

	return HeteroImbalanceFound::sMessageLevel;
}



int HeteroImbalanceFound :: GetNumberOfMessageParts () const {

	return HeteroImbalanceFound::sMessageList.Entries ();
}


int HeteroImbalanceFound :: GetSeverityLevel () const {

	return HeteroImbalanceFound::sMessageLevel;
}


void HeteroImbalanceFound :: SetIndexAndScope (int index, int scope) const {

	HeteroImbalanceFound::sMessageIndex = index;
	HeteroImbalanceFound::sMessageScope = scope;
}


int HeteroImbalanceFound :: GetMessageIndex () const {

	return HeteroImbalanceFound :: sMessageIndex;
}



int HeteroImbalanceFound :: GetScope () const {

	return HeteroImbalanceFound :: sMessageScope;
}



int HeteroImbalanceFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	HeteroImbalanceFound* noticeType = new HeteroImbalanceFound;
	warehouse->AddType (noticeType);
	return 1;
}





OLAlleleFound :: OLAlleleFound () : Notice () {

}


OLAlleleFound :: OLAlleleFound (const OLAlleleFound& note) : Notice ((const Notice&) note) {

}



OLAlleleFound :: ~OLAlleleFound () {

}



int OLAlleleFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (OLAlleleFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		OLAlleleFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OLAlleleFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OLAlleleFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		OLAlleleFound::sLabel = temp;

	else
		OLAlleleFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OLAlleleFound::sTriggerLowPriority = true;

		else
			OLAlleleFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OLAlleleFound::sDoNotCall = true;

		else
			OLAlleleFound::sDoNotCall = false;
	}


	int N = OLAlleleFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	OLAlleleFound::sMessageArray = new RGString* [N];
	RGDListIterator it (OLAlleleFound::sMessageList);

	for (int i=0; i<N; i++)
		OLAlleleFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int OLAlleleFound :: GetSubject () const {

	return OLAlleleFound::sSubject;
}


RGString OLAlleleFound :: GetLabel () const {

	return OLAlleleFound::sLabel;
}


bool OLAlleleFound :: IsTriggerForLowPriority () const {

	return OLAlleleFound::sTriggerLowPriority;
}


bool OLAlleleFound :: IsDoNotCall () const {

	return OLAlleleFound::sDoNotCall;
}



RGString OLAlleleFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= OLAlleleFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *OLAlleleFound::sMessageArray [i];
	return returnValue;
}



int OLAlleleFound :: GetMessageLevel () const {

	return OLAlleleFound::sMessageLevel;
}



int OLAlleleFound :: GetNumberOfMessageParts () const {

	return OLAlleleFound::sMessageList.Entries ();
}


int OLAlleleFound :: GetSeverityLevel () const {

	return OLAlleleFound::sMessageLevel;
}


void OLAlleleFound :: SetIndexAndScope (int index, int scope) const {

	OLAlleleFound::sMessageIndex = index;
	OLAlleleFound::sMessageScope = scope;
}


int OLAlleleFound :: GetMessageIndex () const {

	return OLAlleleFound :: sMessageIndex;
}



int OLAlleleFound :: GetScope () const {

	return OLAlleleFound :: sMessageScope;
}



int OLAlleleFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	OLAlleleFound* noticeType = new OLAlleleFound;
	warehouse->AddType (noticeType);
	return 1;
}





NotOKHomozygoteFound :: NotOKHomozygoteFound () : Notice () {

}


NotOKHomozygoteFound :: NotOKHomozygoteFound (const NotOKHomozygoteFound& note) : Notice ((const Notice&) note) {

}



NotOKHomozygoteFound :: ~NotOKHomozygoteFound () {

}



int NotOKHomozygoteFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NotOKHomozygoteFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NotOKHomozygoteFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NotOKHomozygoteFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NotOKHomozygoteFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NotOKHomozygoteFound::sLabel = temp;

	else
		NotOKHomozygoteFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NotOKHomozygoteFound::sTriggerLowPriority = true;

		else
			NotOKHomozygoteFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NotOKHomozygoteFound::sDoNotCall = true;

		else
			NotOKHomozygoteFound::sDoNotCall = false;
	}


	int N = NotOKHomozygoteFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	NotOKHomozygoteFound::sMessageArray = new RGString* [N];
	RGDListIterator it (NotOKHomozygoteFound::sMessageList);

	for (int i=0; i<N; i++)
		NotOKHomozygoteFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NotOKHomozygoteFound :: GetSubject () const {

	return NotOKHomozygoteFound::sSubject;
}


RGString NotOKHomozygoteFound :: GetLabel () const {

	return NotOKHomozygoteFound::sLabel;
}


bool NotOKHomozygoteFound :: IsTriggerForLowPriority () const {

	return NotOKHomozygoteFound::sTriggerLowPriority;
}


bool NotOKHomozygoteFound :: IsDoNotCall () const {

	return NotOKHomozygoteFound::sDoNotCall;
}



RGString NotOKHomozygoteFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NotOKHomozygoteFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NotOKHomozygoteFound::sMessageArray [i];
	return returnValue;
}



int NotOKHomozygoteFound :: GetMessageLevel () const {

	return NotOKHomozygoteFound::sMessageLevel;
}



int NotOKHomozygoteFound :: GetNumberOfMessageParts () const {

	return NotOKHomozygoteFound::sMessageList.Entries ();
}


int NotOKHomozygoteFound :: GetSeverityLevel () const {

	return NotOKHomozygoteFound::sMessageLevel;
}


void NotOKHomozygoteFound :: SetIndexAndScope (int index, int scope) const {

	NotOKHomozygoteFound::sMessageIndex = index;
	NotOKHomozygoteFound::sMessageScope = scope;
}


int NotOKHomozygoteFound :: GetMessageIndex () const {

	return NotOKHomozygoteFound :: sMessageIndex;
}



int NotOKHomozygoteFound :: GetScope () const {

	return NotOKHomozygoteFound :: sMessageScope;
}



int NotOKHomozygoteFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NotOKHomozygoteFound* noticeType = new NotOKHomozygoteFound;
	warehouse->AddType (noticeType);
	return 1;
}





OsirisNotEqualABI :: OsirisNotEqualABI () : Notice () {

}


OsirisNotEqualABI :: OsirisNotEqualABI (const OsirisNotEqualABI& note) : Notice ((const Notice&) note) {

}



OsirisNotEqualABI :: ~OsirisNotEqualABI () {

}



int OsirisNotEqualABI :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (OsirisNotEqualABI::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		OsirisNotEqualABI::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OsirisNotEqualABI::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OsirisNotEqualABI::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		OsirisNotEqualABI::sLabel = temp;

	else
		OsirisNotEqualABI::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OsirisNotEqualABI::sTriggerLowPriority = true;

		else
			OsirisNotEqualABI::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OsirisNotEqualABI::sDoNotCall = true;

		else
			OsirisNotEqualABI::sDoNotCall = false;
	}


	int N = OsirisNotEqualABI::sMessageList.Entries ();

	if (N == 1)
		N++;

	OsirisNotEqualABI::sMessageArray = new RGString* [N];
	RGDListIterator it (OsirisNotEqualABI::sMessageList);

	for (int i=0; i<N; i++)
		OsirisNotEqualABI::sMessageArray [i] = (RGString*) it ();

	return status;
}


int OsirisNotEqualABI :: GetSubject () const {

	return OsirisNotEqualABI::sSubject;
}


RGString OsirisNotEqualABI :: GetLabel () const {

	return OsirisNotEqualABI::sLabel;
}


bool OsirisNotEqualABI :: IsTriggerForLowPriority () const {

	return OsirisNotEqualABI::sTriggerLowPriority;
}


bool OsirisNotEqualABI :: IsDoNotCall () const {

	return OsirisNotEqualABI::sDoNotCall;
}



RGString OsirisNotEqualABI :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= OsirisNotEqualABI::sMessageList.Entries ()))
		return returnValue;

	returnValue = *OsirisNotEqualABI::sMessageArray [i];
	return returnValue;
}



int OsirisNotEqualABI :: GetMessageLevel () const {

	return OsirisNotEqualABI::sMessageLevel;
}



int OsirisNotEqualABI :: GetNumberOfMessageParts () const {

	return OsirisNotEqualABI::sMessageList.Entries ();
}


int OsirisNotEqualABI :: GetSeverityLevel () const {

	return OsirisNotEqualABI::sMessageLevel;
}


void OsirisNotEqualABI :: SetIndexAndScope (int index, int scope) const {

	OsirisNotEqualABI::sMessageIndex = index;
	OsirisNotEqualABI::sMessageScope = scope;
}


int OsirisNotEqualABI :: GetMessageIndex () const {

	return OsirisNotEqualABI :: sMessageIndex;
}



int OsirisNotEqualABI :: GetScope () const {

	return OsirisNotEqualABI :: sMessageScope;
}



int OsirisNotEqualABI :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	OsirisNotEqualABI* noticeType = new OsirisNotEqualABI;
	warehouse->AddType (noticeType);
	return 1;
}





ILSHasTooFewPeaks :: ILSHasTooFewPeaks () : Notice () {

}


ILSHasTooFewPeaks :: ILSHasTooFewPeaks (const ILSHasTooFewPeaks& note) : Notice ((const Notice&) note) {

}



ILSHasTooFewPeaks :: ~ILSHasTooFewPeaks () {

}



int ILSHasTooFewPeaks :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ILSHasTooFewPeaks::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ILSHasTooFewPeaks::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSHasTooFewPeaks::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSHasTooFewPeaks::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ILSHasTooFewPeaks::sLabel = temp;

	else
		ILSHasTooFewPeaks::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSHasTooFewPeaks::sTriggerLowPriority = true;

		else
			ILSHasTooFewPeaks::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSHasTooFewPeaks::sDoNotCall = true;

		else
			ILSHasTooFewPeaks::sDoNotCall = false;
	}


	int N = ILSHasTooFewPeaks::sMessageList.Entries ();

	if (N == 1)
		N++;

	ILSHasTooFewPeaks::sMessageArray = new RGString* [N];
	RGDListIterator it (ILSHasTooFewPeaks::sMessageList);

	for (int i=0; i<N; i++)
		ILSHasTooFewPeaks::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ILSHasTooFewPeaks :: GetSubject () const {

	return ILSHasTooFewPeaks::sSubject;
}


RGString ILSHasTooFewPeaks :: GetLabel () const {

	return ILSHasTooFewPeaks::sLabel;
}


bool ILSHasTooFewPeaks :: IsTriggerForLowPriority () const {

	return ILSHasTooFewPeaks::sTriggerLowPriority;
}


bool ILSHasTooFewPeaks :: IsDoNotCall () const {

	return ILSHasTooFewPeaks::sDoNotCall;
}



RGString ILSHasTooFewPeaks :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ILSHasTooFewPeaks::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ILSHasTooFewPeaks::sMessageArray [i];
	return returnValue;
}



int ILSHasTooFewPeaks :: GetMessageLevel () const {

	return ILSHasTooFewPeaks::sMessageLevel;
}



int ILSHasTooFewPeaks :: GetNumberOfMessageParts () const {

	return ILSHasTooFewPeaks::sMessageList.Entries ();
}


int ILSHasTooFewPeaks :: GetSeverityLevel () const {

	return ILSHasTooFewPeaks::sMessageLevel;
}


void ILSHasTooFewPeaks :: SetIndexAndScope (int index, int scope) const {

	ILSHasTooFewPeaks::sMessageIndex = index;
	ILSHasTooFewPeaks::sMessageScope = scope;
}


int ILSHasTooFewPeaks :: GetMessageIndex () const {

	return ILSHasTooFewPeaks :: sMessageIndex;
}



int ILSHasTooFewPeaks :: GetScope () const {

	return ILSHasTooFewPeaks :: sMessageScope;
}



int ILSHasTooFewPeaks :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ILSHasTooFewPeaks* noticeType = new ILSHasTooFewPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





LocusHasTooFewPeaks :: LocusHasTooFewPeaks () : Notice () {

}


LocusHasTooFewPeaks :: LocusHasTooFewPeaks (const LocusHasTooFewPeaks& note) : Notice ((const Notice&) note) {

}



LocusHasTooFewPeaks :: ~LocusHasTooFewPeaks () {

}



int LocusHasTooFewPeaks :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LocusHasTooFewPeaks::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LocusHasTooFewPeaks::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LocusHasTooFewPeaks::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LocusHasTooFewPeaks::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LocusHasTooFewPeaks::sLabel = temp;

	else
		LocusHasTooFewPeaks::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LocusHasTooFewPeaks::sTriggerLowPriority = true;

		else
			LocusHasTooFewPeaks::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LocusHasTooFewPeaks::sDoNotCall = true;

		else
			LocusHasTooFewPeaks::sDoNotCall = false;
	}


	int N = LocusHasTooFewPeaks::sMessageList.Entries ();

	if (N == 1)
		N++;

	LocusHasTooFewPeaks::sMessageArray = new RGString* [N];
	RGDListIterator it (LocusHasTooFewPeaks::sMessageList);

	for (int i=0; i<N; i++)
		LocusHasTooFewPeaks::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LocusHasTooFewPeaks :: GetSubject () const {

	return LocusHasTooFewPeaks::sSubject;
}


RGString LocusHasTooFewPeaks :: GetLabel () const {

	return LocusHasTooFewPeaks::sLabel;
}


bool LocusHasTooFewPeaks :: IsTriggerForLowPriority () const {

	return LocusHasTooFewPeaks::sTriggerLowPriority;
}


bool LocusHasTooFewPeaks :: IsDoNotCall () const {

	return LocusHasTooFewPeaks::sDoNotCall;
}



RGString LocusHasTooFewPeaks :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LocusHasTooFewPeaks::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LocusHasTooFewPeaks::sMessageArray [i];
	return returnValue;
}



int LocusHasTooFewPeaks :: GetMessageLevel () const {

	return LocusHasTooFewPeaks::sMessageLevel;
}



int LocusHasTooFewPeaks :: GetNumberOfMessageParts () const {

	return LocusHasTooFewPeaks::sMessageList.Entries ();
}


int LocusHasTooFewPeaks :: GetSeverityLevel () const {

	return LocusHasTooFewPeaks::sMessageLevel;
}


void LocusHasTooFewPeaks :: SetIndexAndScope (int index, int scope) const {

	LocusHasTooFewPeaks::sMessageIndex = index;
	LocusHasTooFewPeaks::sMessageScope = scope;
}


int LocusHasTooFewPeaks :: GetMessageIndex () const {

	return LocusHasTooFewPeaks :: sMessageIndex;
}



int LocusHasTooFewPeaks :: GetScope () const {

	return LocusHasTooFewPeaks :: sMessageScope;
}



int LocusHasTooFewPeaks :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LocusHasTooFewPeaks* noticeType = new LocusHasTooFewPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





DirectoryInfoInsufficient :: DirectoryInfoInsufficient () : Notice () {

}


DirectoryInfoInsufficient :: DirectoryInfoInsufficient (const DirectoryInfoInsufficient& note) : Notice ((const Notice&) note) {

}



DirectoryInfoInsufficient :: ~DirectoryInfoInsufficient () {

}



int DirectoryInfoInsufficient :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (DirectoryInfoInsufficient::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		DirectoryInfoInsufficient::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	DirectoryInfoInsufficient::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	DirectoryInfoInsufficient::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		DirectoryInfoInsufficient::sLabel = temp;

	else
		DirectoryInfoInsufficient::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			DirectoryInfoInsufficient::sTriggerLowPriority = true;

		else
			DirectoryInfoInsufficient::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			DirectoryInfoInsufficient::sDoNotCall = true;

		else
			DirectoryInfoInsufficient::sDoNotCall = false;
	}


	int N = DirectoryInfoInsufficient::sMessageList.Entries ();

	if (N == 1)
		N++;

	DirectoryInfoInsufficient::sMessageArray = new RGString* [N];
	RGDListIterator it (DirectoryInfoInsufficient::sMessageList);

	for (int i=0; i<N; i++)
		DirectoryInfoInsufficient::sMessageArray [i] = (RGString*) it ();

	return status;
}


int DirectoryInfoInsufficient :: GetSubject () const {

	return DirectoryInfoInsufficient::sSubject;
}


RGString DirectoryInfoInsufficient :: GetLabel () const {

	return DirectoryInfoInsufficient::sLabel;
}


bool DirectoryInfoInsufficient :: IsTriggerForLowPriority () const {

	return DirectoryInfoInsufficient::sTriggerLowPriority;
}


bool DirectoryInfoInsufficient :: IsDoNotCall () const {

	return DirectoryInfoInsufficient::sDoNotCall;
}



RGString DirectoryInfoInsufficient :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= DirectoryInfoInsufficient::sMessageList.Entries ()))
		return returnValue;

	returnValue = *DirectoryInfoInsufficient::sMessageArray [i];
	return returnValue;
}



int DirectoryInfoInsufficient :: GetMessageLevel () const {

	return DirectoryInfoInsufficient::sMessageLevel;
}



int DirectoryInfoInsufficient :: GetNumberOfMessageParts () const {

	return DirectoryInfoInsufficient::sMessageList.Entries ();
}


int DirectoryInfoInsufficient :: GetSeverityLevel () const {

	return DirectoryInfoInsufficient::sMessageLevel;
}


void DirectoryInfoInsufficient :: SetIndexAndScope (int index, int scope) const {

	DirectoryInfoInsufficient::sMessageIndex = index;
	DirectoryInfoInsufficient::sMessageScope = scope;
}


int DirectoryInfoInsufficient :: GetMessageIndex () const {

	return DirectoryInfoInsufficient :: sMessageIndex;
}



int DirectoryInfoInsufficient :: GetScope () const {

	return DirectoryInfoInsufficient :: sMessageScope;
}



int DirectoryInfoInsufficient :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	DirectoryInfoInsufficient* noticeType = new DirectoryInfoInsufficient;
	warehouse->AddType (noticeType);
	return 1;
}





CouldNotReadFSA :: CouldNotReadFSA () : Notice () {

}


CouldNotReadFSA :: CouldNotReadFSA (const CouldNotReadFSA& note) : Notice ((const Notice&) note) {

}



CouldNotReadFSA :: ~CouldNotReadFSA () {

}



int CouldNotReadFSA :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (CouldNotReadFSA::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		CouldNotReadFSA::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CouldNotReadFSA::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CouldNotReadFSA::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		CouldNotReadFSA::sLabel = temp;

	else
		CouldNotReadFSA::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CouldNotReadFSA::sTriggerLowPriority = true;

		else
			CouldNotReadFSA::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CouldNotReadFSA::sDoNotCall = true;

		else
			CouldNotReadFSA::sDoNotCall = false;
	}


	int N = CouldNotReadFSA::sMessageList.Entries ();

	if (N == 1)
		N++;

	CouldNotReadFSA::sMessageArray = new RGString* [N];
	RGDListIterator it (CouldNotReadFSA::sMessageList);

	for (int i=0; i<N; i++)
		CouldNotReadFSA::sMessageArray [i] = (RGString*) it ();

	return status;
}


int CouldNotReadFSA :: GetSubject () const {

	return CouldNotReadFSA::sSubject;
}


RGString CouldNotReadFSA :: GetLabel () const {

	return CouldNotReadFSA::sLabel;
}


bool CouldNotReadFSA :: IsTriggerForLowPriority () const {

	return CouldNotReadFSA::sTriggerLowPriority;
}


bool CouldNotReadFSA :: IsDoNotCall () const {

	return CouldNotReadFSA::sDoNotCall;
}



RGString CouldNotReadFSA :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= CouldNotReadFSA::sMessageList.Entries ()))
		return returnValue;

	returnValue = *CouldNotReadFSA::sMessageArray [i];
	return returnValue;
}



int CouldNotReadFSA :: GetMessageLevel () const {

	return CouldNotReadFSA::sMessageLevel;
}



int CouldNotReadFSA :: GetNumberOfMessageParts () const {

	return CouldNotReadFSA::sMessageList.Entries ();
}


int CouldNotReadFSA :: GetSeverityLevel () const {

	return CouldNotReadFSA::sMessageLevel;
}


void CouldNotReadFSA :: SetIndexAndScope (int index, int scope) const {

	CouldNotReadFSA::sMessageIndex = index;
	CouldNotReadFSA::sMessageScope = scope;
}


int CouldNotReadFSA :: GetMessageIndex () const {

	return CouldNotReadFSA :: sMessageIndex;
}



int CouldNotReadFSA :: GetScope () const {

	return CouldNotReadFSA :: sMessageScope;
}



int CouldNotReadFSA :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	CouldNotReadFSA* noticeType = new CouldNotReadFSA;
	warehouse->AddType (noticeType);
	return 1;
}





NoInputDirectory :: NoInputDirectory () : Notice () {

}


NoInputDirectory :: NoInputDirectory (const NoInputDirectory& note) : Notice ((const Notice&) note) {

}



NoInputDirectory :: ~NoInputDirectory () {

}



int NoInputDirectory :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoInputDirectory::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoInputDirectory::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoInputDirectory::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoInputDirectory::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoInputDirectory::sLabel = temp;

	else
		NoInputDirectory::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoInputDirectory::sTriggerLowPriority = true;

		else
			NoInputDirectory::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoInputDirectory::sDoNotCall = true;

		else
			NoInputDirectory::sDoNotCall = false;
	}


	int N = NoInputDirectory::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoInputDirectory::sMessageArray = new RGString* [N];
	RGDListIterator it (NoInputDirectory::sMessageList);

	for (int i=0; i<N; i++)
		NoInputDirectory::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoInputDirectory :: GetSubject () const {

	return NoInputDirectory::sSubject;
}


RGString NoInputDirectory :: GetLabel () const {

	return NoInputDirectory::sLabel;
}


bool NoInputDirectory :: IsTriggerForLowPriority () const {

	return NoInputDirectory::sTriggerLowPriority;
}


bool NoInputDirectory :: IsDoNotCall () const {

	return NoInputDirectory::sDoNotCall;
}



RGString NoInputDirectory :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoInputDirectory::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoInputDirectory::sMessageArray [i];
	return returnValue;
}



int NoInputDirectory :: GetMessageLevel () const {

	return NoInputDirectory::sMessageLevel;
}



int NoInputDirectory :: GetNumberOfMessageParts () const {

	return NoInputDirectory::sMessageList.Entries ();
}


int NoInputDirectory :: GetSeverityLevel () const {

	return NoInputDirectory::sMessageLevel;
}


void NoInputDirectory :: SetIndexAndScope (int index, int scope) const {

	NoInputDirectory::sMessageIndex = index;
	NoInputDirectory::sMessageScope = scope;
}


int NoInputDirectory :: GetMessageIndex () const {

	return NoInputDirectory :: sMessageIndex;
}



int NoInputDirectory :: GetScope () const {

	return NoInputDirectory :: sMessageScope;
}



int NoInputDirectory :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoInputDirectory* noticeType = new NoInputDirectory;
	warehouse->AddType (noticeType);
	return 1;
}





CouldNotOpenOutputFile :: CouldNotOpenOutputFile () : Notice () {

}


CouldNotOpenOutputFile :: CouldNotOpenOutputFile (const CouldNotOpenOutputFile& note) : Notice ((const Notice&) note) {

}



CouldNotOpenOutputFile :: ~CouldNotOpenOutputFile () {

}



int CouldNotOpenOutputFile :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (CouldNotOpenOutputFile::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		CouldNotOpenOutputFile::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CouldNotOpenOutputFile::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CouldNotOpenOutputFile::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		CouldNotOpenOutputFile::sLabel = temp;

	else
		CouldNotOpenOutputFile::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CouldNotOpenOutputFile::sTriggerLowPriority = true;

		else
			CouldNotOpenOutputFile::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CouldNotOpenOutputFile::sDoNotCall = true;

		else
			CouldNotOpenOutputFile::sDoNotCall = false;
	}


	int N = CouldNotOpenOutputFile::sMessageList.Entries ();

	if (N == 1)
		N++;

	CouldNotOpenOutputFile::sMessageArray = new RGString* [N];
	RGDListIterator it (CouldNotOpenOutputFile::sMessageList);

	for (int i=0; i<N; i++)
		CouldNotOpenOutputFile::sMessageArray [i] = (RGString*) it ();

	return status;
}


int CouldNotOpenOutputFile :: GetSubject () const {

	return CouldNotOpenOutputFile::sSubject;
}


RGString CouldNotOpenOutputFile :: GetLabel () const {

	return CouldNotOpenOutputFile::sLabel;
}


bool CouldNotOpenOutputFile :: IsTriggerForLowPriority () const {

	return CouldNotOpenOutputFile::sTriggerLowPriority;
}


bool CouldNotOpenOutputFile :: IsDoNotCall () const {

	return CouldNotOpenOutputFile::sDoNotCall;
}



RGString CouldNotOpenOutputFile :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= CouldNotOpenOutputFile::sMessageList.Entries ()))
		return returnValue;

	returnValue = *CouldNotOpenOutputFile::sMessageArray [i];
	return returnValue;
}



int CouldNotOpenOutputFile :: GetMessageLevel () const {

	return CouldNotOpenOutputFile::sMessageLevel;
}



int CouldNotOpenOutputFile :: GetNumberOfMessageParts () const {

	return CouldNotOpenOutputFile::sMessageList.Entries ();
}


int CouldNotOpenOutputFile :: GetSeverityLevel () const {

	return CouldNotOpenOutputFile::sMessageLevel;
}


void CouldNotOpenOutputFile :: SetIndexAndScope (int index, int scope) const {

	CouldNotOpenOutputFile::sMessageIndex = index;
	CouldNotOpenOutputFile::sMessageScope = scope;
}


int CouldNotOpenOutputFile :: GetMessageIndex () const {

	return CouldNotOpenOutputFile :: sMessageIndex;
}



int CouldNotOpenOutputFile :: GetScope () const {

	return CouldNotOpenOutputFile :: sMessageScope;
}



int CouldNotOpenOutputFile :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	CouldNotOpenOutputFile* noticeType = new CouldNotOpenOutputFile;
	warehouse->AddType (noticeType);
	return 1;
}





LadderDataFormatUnknown :: LadderDataFormatUnknown () : Notice () {

}


LadderDataFormatUnknown :: LadderDataFormatUnknown (const LadderDataFormatUnknown& note) : Notice ((const Notice&) note) {

}



LadderDataFormatUnknown :: ~LadderDataFormatUnknown () {

}



int LadderDataFormatUnknown :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LadderDataFormatUnknown::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LadderDataFormatUnknown::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LadderDataFormatUnknown::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LadderDataFormatUnknown::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LadderDataFormatUnknown::sLabel = temp;

	else
		LadderDataFormatUnknown::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LadderDataFormatUnknown::sTriggerLowPriority = true;

		else
			LadderDataFormatUnknown::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LadderDataFormatUnknown::sDoNotCall = true;

		else
			LadderDataFormatUnknown::sDoNotCall = false;
	}


	int N = LadderDataFormatUnknown::sMessageList.Entries ();

	if (N == 1)
		N++;

	LadderDataFormatUnknown::sMessageArray = new RGString* [N];
	RGDListIterator it (LadderDataFormatUnknown::sMessageList);

	for (int i=0; i<N; i++)
		LadderDataFormatUnknown::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LadderDataFormatUnknown :: GetSubject () const {

	return LadderDataFormatUnknown::sSubject;
}


RGString LadderDataFormatUnknown :: GetLabel () const {

	return LadderDataFormatUnknown::sLabel;
}


bool LadderDataFormatUnknown :: IsTriggerForLowPriority () const {

	return LadderDataFormatUnknown::sTriggerLowPriority;
}


bool LadderDataFormatUnknown :: IsDoNotCall () const {

	return LadderDataFormatUnknown::sDoNotCall;
}



RGString LadderDataFormatUnknown :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LadderDataFormatUnknown::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LadderDataFormatUnknown::sMessageArray [i];
	return returnValue;
}



int LadderDataFormatUnknown :: GetMessageLevel () const {

	return LadderDataFormatUnknown::sMessageLevel;
}



int LadderDataFormatUnknown :: GetNumberOfMessageParts () const {

	return LadderDataFormatUnknown::sMessageList.Entries ();
}


int LadderDataFormatUnknown :: GetSeverityLevel () const {

	return LadderDataFormatUnknown::sMessageLevel;
}


void LadderDataFormatUnknown :: SetIndexAndScope (int index, int scope) const {

	LadderDataFormatUnknown::sMessageIndex = index;
	LadderDataFormatUnknown::sMessageScope = scope;
}


int LadderDataFormatUnknown :: GetMessageIndex () const {

	return LadderDataFormatUnknown :: sMessageIndex;
}



int LadderDataFormatUnknown :: GetScope () const {

	return LadderDataFormatUnknown :: sMessageScope;
}



int LadderDataFormatUnknown :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LadderDataFormatUnknown* noticeType = new LadderDataFormatUnknown;
	warehouse->AddType (noticeType);
	return 1;
}





PervasiveStutter :: PervasiveStutter () : Notice () {

}


PervasiveStutter :: PervasiveStutter (const PervasiveStutter& note) : Notice ((const Notice&) note) {

}



PervasiveStutter :: ~PervasiveStutter () {

}



int PervasiveStutter :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PervasiveStutter::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PervasiveStutter::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PervasiveStutter::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PervasiveStutter::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PervasiveStutter::sLabel = temp;

	else
		PervasiveStutter::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PervasiveStutter::sTriggerLowPriority = true;

		else
			PervasiveStutter::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PervasiveStutter::sDoNotCall = true;

		else
			PervasiveStutter::sDoNotCall = false;
	}


	int N = PervasiveStutter::sMessageList.Entries ();

	if (N == 1)
		N++;

	PervasiveStutter::sMessageArray = new RGString* [N];
	RGDListIterator it (PervasiveStutter::sMessageList);

	for (int i=0; i<N; i++)
		PervasiveStutter::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PervasiveStutter :: GetSubject () const {

	return PervasiveStutter::sSubject;
}


RGString PervasiveStutter :: GetLabel () const {

	return PervasiveStutter::sLabel;
}


bool PervasiveStutter :: IsTriggerForLowPriority () const {

	return PervasiveStutter::sTriggerLowPriority;
}


bool PervasiveStutter :: IsDoNotCall () const {

	return PervasiveStutter::sDoNotCall;
}



RGString PervasiveStutter :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PervasiveStutter::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PervasiveStutter::sMessageArray [i];
	return returnValue;
}



int PervasiveStutter :: GetMessageLevel () const {

	return PervasiveStutter::sMessageLevel;
}



int PervasiveStutter :: GetNumberOfMessageParts () const {

	return PervasiveStutter::sMessageList.Entries ();
}


int PervasiveStutter :: GetSeverityLevel () const {

	return PervasiveStutter::sMessageLevel;
}


void PervasiveStutter :: SetIndexAndScope (int index, int scope) const {

	PervasiveStutter::sMessageIndex = index;
	PervasiveStutter::sMessageScope = scope;
}


int PervasiveStutter :: GetMessageIndex () const {

	return PervasiveStutter :: sMessageIndex;
}



int PervasiveStutter :: GetScope () const {

	return PervasiveStutter :: sMessageScope;
}



int PervasiveStutter :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PervasiveStutter* noticeType = new PervasiveStutter;
	warehouse->AddType (noticeType);
	return 1;
}





PeakOutsideILSRange :: PeakOutsideILSRange () : Notice () {

}


PeakOutsideILSRange :: PeakOutsideILSRange (const PeakOutsideILSRange& note) : Notice ((const Notice&) note) {

}



PeakOutsideILSRange :: ~PeakOutsideILSRange () {

}



int PeakOutsideILSRange :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakOutsideILSRange::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakOutsideILSRange::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakOutsideILSRange::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakOutsideILSRange::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakOutsideILSRange::sLabel = temp;

	else
		PeakOutsideILSRange::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakOutsideILSRange::sTriggerLowPriority = true;

		else
			PeakOutsideILSRange::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakOutsideILSRange::sDoNotCall = true;

		else
			PeakOutsideILSRange::sDoNotCall = false;
	}


	int N = PeakOutsideILSRange::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakOutsideILSRange::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakOutsideILSRange::sMessageList);

	for (int i=0; i<N; i++)
		PeakOutsideILSRange::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakOutsideILSRange :: GetSubject () const {

	return PeakOutsideILSRange::sSubject;
}


RGString PeakOutsideILSRange :: GetLabel () const {

	return PeakOutsideILSRange::sLabel;
}


bool PeakOutsideILSRange :: IsTriggerForLowPriority () const {

	return PeakOutsideILSRange::sTriggerLowPriority;
}


bool PeakOutsideILSRange :: IsDoNotCall () const {

	return PeakOutsideILSRange::sDoNotCall;
}



RGString PeakOutsideILSRange :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakOutsideILSRange::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakOutsideILSRange::sMessageArray [i];
	return returnValue;
}



int PeakOutsideILSRange :: GetMessageLevel () const {

	return PeakOutsideILSRange::sMessageLevel;
}



int PeakOutsideILSRange :: GetNumberOfMessageParts () const {

	return PeakOutsideILSRange::sMessageList.Entries ();
}


int PeakOutsideILSRange :: GetSeverityLevel () const {

	return PeakOutsideILSRange::sMessageLevel;
}


void PeakOutsideILSRange :: SetIndexAndScope (int index, int scope) const {

	PeakOutsideILSRange::sMessageIndex = index;
	PeakOutsideILSRange::sMessageScope = scope;
}


int PeakOutsideILSRange :: GetMessageIndex () const {

	return PeakOutsideILSRange :: sMessageIndex;
}



int PeakOutsideILSRange :: GetScope () const {

	return PeakOutsideILSRange :: sMessageScope;
}



int PeakOutsideILSRange :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakOutsideILSRange* noticeType = new PeakOutsideILSRange;
	warehouse->AddType (noticeType);
	return 1;
}





InterLocusLadderPeak :: InterLocusLadderPeak () : Notice () {

}


InterLocusLadderPeak :: InterLocusLadderPeak (const InterLocusLadderPeak& note) : Notice ((const Notice&) note) {

}



InterLocusLadderPeak :: ~InterLocusLadderPeak () {

}



int InterLocusLadderPeak :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (InterLocusLadderPeak::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		InterLocusLadderPeak::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterLocusLadderPeak::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterLocusLadderPeak::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		InterLocusLadderPeak::sLabel = temp;

	else
		InterLocusLadderPeak::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterLocusLadderPeak::sTriggerLowPriority = true;

		else
			InterLocusLadderPeak::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterLocusLadderPeak::sDoNotCall = true;

		else
			InterLocusLadderPeak::sDoNotCall = false;
	}


	int N = InterLocusLadderPeak::sMessageList.Entries ();

	if (N == 1)
		N++;

	InterLocusLadderPeak::sMessageArray = new RGString* [N];
	RGDListIterator it (InterLocusLadderPeak::sMessageList);

	for (int i=0; i<N; i++)
		InterLocusLadderPeak::sMessageArray [i] = (RGString*) it ();

	return status;
}


int InterLocusLadderPeak :: GetSubject () const {

	return InterLocusLadderPeak::sSubject;
}


RGString InterLocusLadderPeak :: GetLabel () const {

	return InterLocusLadderPeak::sLabel;
}


bool InterLocusLadderPeak :: IsTriggerForLowPriority () const {

	return InterLocusLadderPeak::sTriggerLowPriority;
}


bool InterLocusLadderPeak :: IsDoNotCall () const {

	return InterLocusLadderPeak::sDoNotCall;
}



RGString InterLocusLadderPeak :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= InterLocusLadderPeak::sMessageList.Entries ()))
		return returnValue;

	returnValue = *InterLocusLadderPeak::sMessageArray [i];
	return returnValue;
}



int InterLocusLadderPeak :: GetMessageLevel () const {

	return InterLocusLadderPeak::sMessageLevel;
}



int InterLocusLadderPeak :: GetNumberOfMessageParts () const {

	return InterLocusLadderPeak::sMessageList.Entries ();
}


int InterLocusLadderPeak :: GetSeverityLevel () const {

	return InterLocusLadderPeak::sMessageLevel;
}


void InterLocusLadderPeak :: SetIndexAndScope (int index, int scope) const {

	InterLocusLadderPeak::sMessageIndex = index;
	InterLocusLadderPeak::sMessageScope = scope;
}


int InterLocusLadderPeak :: GetMessageIndex () const {

	return InterLocusLadderPeak :: sMessageIndex;
}



int InterLocusLadderPeak :: GetScope () const {

	return InterLocusLadderPeak :: sMessageScope;
}



int InterLocusLadderPeak :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	InterLocusLadderPeak* noticeType = new InterLocusLadderPeak;
	warehouse->AddType (noticeType);
	return 1;
}





NoNamedMarkerSet :: NoNamedMarkerSet () : Notice () {

}


NoNamedMarkerSet :: NoNamedMarkerSet (const NoNamedMarkerSet& note) : Notice ((const Notice&) note) {

}



NoNamedMarkerSet :: ~NoNamedMarkerSet () {

}



int NoNamedMarkerSet :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoNamedMarkerSet::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoNamedMarkerSet::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoNamedMarkerSet::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoNamedMarkerSet::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoNamedMarkerSet::sLabel = temp;

	else
		NoNamedMarkerSet::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoNamedMarkerSet::sTriggerLowPriority = true;

		else
			NoNamedMarkerSet::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoNamedMarkerSet::sDoNotCall = true;

		else
			NoNamedMarkerSet::sDoNotCall = false;
	}


	int N = NoNamedMarkerSet::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoNamedMarkerSet::sMessageArray = new RGString* [N];
	RGDListIterator it (NoNamedMarkerSet::sMessageList);

	for (int i=0; i<N; i++)
		NoNamedMarkerSet::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoNamedMarkerSet :: GetSubject () const {

	return NoNamedMarkerSet::sSubject;
}


RGString NoNamedMarkerSet :: GetLabel () const {

	return NoNamedMarkerSet::sLabel;
}


bool NoNamedMarkerSet :: IsTriggerForLowPriority () const {

	return NoNamedMarkerSet::sTriggerLowPriority;
}


bool NoNamedMarkerSet :: IsDoNotCall () const {

	return NoNamedMarkerSet::sDoNotCall;
}



RGString NoNamedMarkerSet :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoNamedMarkerSet::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoNamedMarkerSet::sMessageArray [i];
	return returnValue;
}



int NoNamedMarkerSet :: GetMessageLevel () const {

	return NoNamedMarkerSet::sMessageLevel;
}



int NoNamedMarkerSet :: GetNumberOfMessageParts () const {

	return NoNamedMarkerSet::sMessageList.Entries ();
}


int NoNamedMarkerSet :: GetSeverityLevel () const {

	return NoNamedMarkerSet::sMessageLevel;
}


void NoNamedMarkerSet :: SetIndexAndScope (int index, int scope) const {

	NoNamedMarkerSet::sMessageIndex = index;
	NoNamedMarkerSet::sMessageScope = scope;
}


int NoNamedMarkerSet :: GetMessageIndex () const {

	return NoNamedMarkerSet :: sMessageIndex;
}



int NoNamedMarkerSet :: GetScope () const {

	return NoNamedMarkerSet :: sMessageScope;
}



int NoNamedMarkerSet :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoNamedMarkerSet* noticeType = new NoNamedMarkerSet;
	warehouse->AddType (noticeType);
	return 1;
}





NoNamedLaneStandard :: NoNamedLaneStandard () : Notice () {

}


NoNamedLaneStandard :: NoNamedLaneStandard (const NoNamedLaneStandard& note) : Notice ((const Notice&) note) {

}



NoNamedLaneStandard :: ~NoNamedLaneStandard () {

}



int NoNamedLaneStandard :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoNamedLaneStandard::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoNamedLaneStandard::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoNamedLaneStandard::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoNamedLaneStandard::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoNamedLaneStandard::sLabel = temp;

	else
		NoNamedLaneStandard::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoNamedLaneStandard::sTriggerLowPriority = true;

		else
			NoNamedLaneStandard::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoNamedLaneStandard::sDoNotCall = true;

		else
			NoNamedLaneStandard::sDoNotCall = false;
	}


	int N = NoNamedLaneStandard::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoNamedLaneStandard::sMessageArray = new RGString* [N];
	RGDListIterator it (NoNamedLaneStandard::sMessageList);

	for (int i=0; i<N; i++)
		NoNamedLaneStandard::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoNamedLaneStandard :: GetSubject () const {

	return NoNamedLaneStandard::sSubject;
}


RGString NoNamedLaneStandard :: GetLabel () const {

	return NoNamedLaneStandard::sLabel;
}


bool NoNamedLaneStandard :: IsTriggerForLowPriority () const {

	return NoNamedLaneStandard::sTriggerLowPriority;
}


bool NoNamedLaneStandard :: IsDoNotCall () const {

	return NoNamedLaneStandard::sDoNotCall;
}



RGString NoNamedLaneStandard :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoNamedLaneStandard::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoNamedLaneStandard::sMessageArray [i];
	return returnValue;
}



int NoNamedLaneStandard :: GetMessageLevel () const {

	return NoNamedLaneStandard::sMessageLevel;
}



int NoNamedLaneStandard :: GetNumberOfMessageParts () const {

	return NoNamedLaneStandard::sMessageList.Entries ();
}


int NoNamedLaneStandard :: GetSeverityLevel () const {

	return NoNamedLaneStandard::sMessageLevel;
}


void NoNamedLaneStandard :: SetIndexAndScope (int index, int scope) const {

	NoNamedLaneStandard::sMessageIndex = index;
	NoNamedLaneStandard::sMessageScope = scope;
}


int NoNamedLaneStandard :: GetMessageIndex () const {

	return NoNamedLaneStandard :: sMessageIndex;
}



int NoNamedLaneStandard :: GetScope () const {

	return NoNamedLaneStandard :: sMessageScope;
}



int NoNamedLaneStandard :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoNamedLaneStandard* noticeType = new NoNamedLaneStandard;
	warehouse->AddType (noticeType);
	return 1;
}





SignalTooLowForLocus :: SignalTooLowForLocus () : Notice () {

}


SignalTooLowForLocus :: SignalTooLowForLocus (const SignalTooLowForLocus& note) : Notice ((const Notice&) note) {

}



SignalTooLowForLocus :: ~SignalTooLowForLocus () {

}



int SignalTooLowForLocus :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (SignalTooLowForLocus::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		SignalTooLowForLocus::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalTooLowForLocus::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalTooLowForLocus::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		SignalTooLowForLocus::sLabel = temp;

	else
		SignalTooLowForLocus::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalTooLowForLocus::sTriggerLowPriority = true;

		else
			SignalTooLowForLocus::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalTooLowForLocus::sDoNotCall = true;

		else
			SignalTooLowForLocus::sDoNotCall = false;
	}


	int N = SignalTooLowForLocus::sMessageList.Entries ();

	if (N == 1)
		N++;

	SignalTooLowForLocus::sMessageArray = new RGString* [N];
	RGDListIterator it (SignalTooLowForLocus::sMessageList);

	for (int i=0; i<N; i++)
		SignalTooLowForLocus::sMessageArray [i] = (RGString*) it ();

	return status;
}


int SignalTooLowForLocus :: GetSubject () const {

	return SignalTooLowForLocus::sSubject;
}


RGString SignalTooLowForLocus :: GetLabel () const {

	return SignalTooLowForLocus::sLabel;
}


bool SignalTooLowForLocus :: IsTriggerForLowPriority () const {

	return SignalTooLowForLocus::sTriggerLowPriority;
}


bool SignalTooLowForLocus :: IsDoNotCall () const {

	return SignalTooLowForLocus::sDoNotCall;
}



RGString SignalTooLowForLocus :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= SignalTooLowForLocus::sMessageList.Entries ()))
		return returnValue;

	returnValue = *SignalTooLowForLocus::sMessageArray [i];
	return returnValue;
}



int SignalTooLowForLocus :: GetMessageLevel () const {

	return SignalTooLowForLocus::sMessageLevel;
}



int SignalTooLowForLocus :: GetNumberOfMessageParts () const {

	return SignalTooLowForLocus::sMessageList.Entries ();
}


int SignalTooLowForLocus :: GetSeverityLevel () const {

	return SignalTooLowForLocus::sMessageLevel;
}


void SignalTooLowForLocus :: SetIndexAndScope (int index, int scope) const {

	SignalTooLowForLocus::sMessageIndex = index;
	SignalTooLowForLocus::sMessageScope = scope;
}


int SignalTooLowForLocus :: GetMessageIndex () const {

	return SignalTooLowForLocus :: sMessageIndex;
}



int SignalTooLowForLocus :: GetScope () const {

	return SignalTooLowForLocus :: sMessageScope;
}



int SignalTooLowForLocus :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	SignalTooLowForLocus* noticeType = new SignalTooLowForLocus;
	warehouse->AddType (noticeType);
	return 1;
}





SignalPeakBelowMinRFU :: SignalPeakBelowMinRFU () : Notice () {

}


SignalPeakBelowMinRFU :: SignalPeakBelowMinRFU (const SignalPeakBelowMinRFU& note) : Notice ((const Notice&) note) {

}



SignalPeakBelowMinRFU :: ~SignalPeakBelowMinRFU () {

}



int SignalPeakBelowMinRFU :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (SignalPeakBelowMinRFU::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		SignalPeakBelowMinRFU::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalPeakBelowMinRFU::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalPeakBelowMinRFU::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		SignalPeakBelowMinRFU::sLabel = temp;

	else
		SignalPeakBelowMinRFU::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalPeakBelowMinRFU::sTriggerLowPriority = true;

		else
			SignalPeakBelowMinRFU::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalPeakBelowMinRFU::sDoNotCall = true;

		else
			SignalPeakBelowMinRFU::sDoNotCall = false;
	}


	int N = SignalPeakBelowMinRFU::sMessageList.Entries ();

	if (N == 1)
		N++;

	SignalPeakBelowMinRFU::sMessageArray = new RGString* [N];
	RGDListIterator it (SignalPeakBelowMinRFU::sMessageList);

	for (int i=0; i<N; i++)
		SignalPeakBelowMinRFU::sMessageArray [i] = (RGString*) it ();

	return status;
}


int SignalPeakBelowMinRFU :: GetSubject () const {

	return SignalPeakBelowMinRFU::sSubject;
}


RGString SignalPeakBelowMinRFU :: GetLabel () const {

	return SignalPeakBelowMinRFU::sLabel;
}


bool SignalPeakBelowMinRFU :: IsTriggerForLowPriority () const {

	return SignalPeakBelowMinRFU::sTriggerLowPriority;
}


bool SignalPeakBelowMinRFU :: IsDoNotCall () const {

	return SignalPeakBelowMinRFU::sDoNotCall;
}



RGString SignalPeakBelowMinRFU :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= SignalPeakBelowMinRFU::sMessageList.Entries ()))
		return returnValue;

	returnValue = *SignalPeakBelowMinRFU::sMessageArray [i];
	return returnValue;
}



int SignalPeakBelowMinRFU :: GetMessageLevel () const {

	return SignalPeakBelowMinRFU::sMessageLevel;
}



int SignalPeakBelowMinRFU :: GetNumberOfMessageParts () const {

	return SignalPeakBelowMinRFU::sMessageList.Entries ();
}


int SignalPeakBelowMinRFU :: GetSeverityLevel () const {

	return SignalPeakBelowMinRFU::sMessageLevel;
}


void SignalPeakBelowMinRFU :: SetIndexAndScope (int index, int scope) const {

	SignalPeakBelowMinRFU::sMessageIndex = index;
	SignalPeakBelowMinRFU::sMessageScope = scope;
}


int SignalPeakBelowMinRFU :: GetMessageIndex () const {

	return SignalPeakBelowMinRFU :: sMessageIndex;
}



int SignalPeakBelowMinRFU :: GetScope () const {

	return SignalPeakBelowMinRFU :: sMessageScope;
}



int SignalPeakBelowMinRFU :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	SignalPeakBelowMinRFU* noticeType = new SignalPeakBelowMinRFU;
	warehouse->AddType (noticeType);
	return 1;
}





PeakOutOfPlaceInILS :: PeakOutOfPlaceInILS () : Notice () {

}


PeakOutOfPlaceInILS :: PeakOutOfPlaceInILS (const PeakOutOfPlaceInILS& note) : Notice ((const Notice&) note) {

}



PeakOutOfPlaceInILS :: ~PeakOutOfPlaceInILS () {

}



int PeakOutOfPlaceInILS :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakOutOfPlaceInILS::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakOutOfPlaceInILS::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakOutOfPlaceInILS::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakOutOfPlaceInILS::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakOutOfPlaceInILS::sLabel = temp;

	else
		PeakOutOfPlaceInILS::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakOutOfPlaceInILS::sTriggerLowPriority = true;

		else
			PeakOutOfPlaceInILS::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakOutOfPlaceInILS::sDoNotCall = true;

		else
			PeakOutOfPlaceInILS::sDoNotCall = false;
	}


	int N = PeakOutOfPlaceInILS::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakOutOfPlaceInILS::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakOutOfPlaceInILS::sMessageList);

	for (int i=0; i<N; i++)
		PeakOutOfPlaceInILS::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakOutOfPlaceInILS :: GetSubject () const {

	return PeakOutOfPlaceInILS::sSubject;
}


RGString PeakOutOfPlaceInILS :: GetLabel () const {

	return PeakOutOfPlaceInILS::sLabel;
}


bool PeakOutOfPlaceInILS :: IsTriggerForLowPriority () const {

	return PeakOutOfPlaceInILS::sTriggerLowPriority;
}


bool PeakOutOfPlaceInILS :: IsDoNotCall () const {

	return PeakOutOfPlaceInILS::sDoNotCall;
}



RGString PeakOutOfPlaceInILS :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakOutOfPlaceInILS::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakOutOfPlaceInILS::sMessageArray [i];
	return returnValue;
}



int PeakOutOfPlaceInILS :: GetMessageLevel () const {

	return PeakOutOfPlaceInILS::sMessageLevel;
}



int PeakOutOfPlaceInILS :: GetNumberOfMessageParts () const {

	return PeakOutOfPlaceInILS::sMessageList.Entries ();
}


int PeakOutOfPlaceInILS :: GetSeverityLevel () const {

	return PeakOutOfPlaceInILS::sMessageLevel;
}


void PeakOutOfPlaceInILS :: SetIndexAndScope (int index, int scope) const {

	PeakOutOfPlaceInILS::sMessageIndex = index;
	PeakOutOfPlaceInILS::sMessageScope = scope;
}


int PeakOutOfPlaceInILS :: GetMessageIndex () const {

	return PeakOutOfPlaceInILS :: sMessageIndex;
}



int PeakOutOfPlaceInILS :: GetScope () const {

	return PeakOutOfPlaceInILS :: sMessageScope;
}



int PeakOutOfPlaceInILS :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakOutOfPlaceInILS* noticeType = new PeakOutOfPlaceInILS;
	warehouse->AddType (noticeType);
	return 1;
}





PeakOutOfPlaceInLocus :: PeakOutOfPlaceInLocus () : Notice () {

}


PeakOutOfPlaceInLocus :: PeakOutOfPlaceInLocus (const PeakOutOfPlaceInLocus& note) : Notice ((const Notice&) note) {

}



PeakOutOfPlaceInLocus :: ~PeakOutOfPlaceInLocus () {

}



int PeakOutOfPlaceInLocus :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakOutOfPlaceInLocus::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakOutOfPlaceInLocus::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakOutOfPlaceInLocus::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakOutOfPlaceInLocus::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakOutOfPlaceInLocus::sLabel = temp;

	else
		PeakOutOfPlaceInLocus::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakOutOfPlaceInLocus::sTriggerLowPriority = true;

		else
			PeakOutOfPlaceInLocus::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakOutOfPlaceInLocus::sDoNotCall = true;

		else
			PeakOutOfPlaceInLocus::sDoNotCall = false;
	}


	int N = PeakOutOfPlaceInLocus::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakOutOfPlaceInLocus::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakOutOfPlaceInLocus::sMessageList);

	for (int i=0; i<N; i++)
		PeakOutOfPlaceInLocus::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakOutOfPlaceInLocus :: GetSubject () const {

	return PeakOutOfPlaceInLocus::sSubject;
}


RGString PeakOutOfPlaceInLocus :: GetLabel () const {

	return PeakOutOfPlaceInLocus::sLabel;
}


bool PeakOutOfPlaceInLocus :: IsTriggerForLowPriority () const {

	return PeakOutOfPlaceInLocus::sTriggerLowPriority;
}


bool PeakOutOfPlaceInLocus :: IsDoNotCall () const {

	return PeakOutOfPlaceInLocus::sDoNotCall;
}



RGString PeakOutOfPlaceInLocus :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakOutOfPlaceInLocus::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakOutOfPlaceInLocus::sMessageArray [i];
	return returnValue;
}



int PeakOutOfPlaceInLocus :: GetMessageLevel () const {

	return PeakOutOfPlaceInLocus::sMessageLevel;
}



int PeakOutOfPlaceInLocus :: GetNumberOfMessageParts () const {

	return PeakOutOfPlaceInLocus::sMessageList.Entries ();
}


int PeakOutOfPlaceInLocus :: GetSeverityLevel () const {

	return PeakOutOfPlaceInLocus::sMessageLevel;
}


void PeakOutOfPlaceInLocus :: SetIndexAndScope (int index, int scope) const {

	PeakOutOfPlaceInLocus::sMessageIndex = index;
	PeakOutOfPlaceInLocus::sMessageScope = scope;
}


int PeakOutOfPlaceInLocus :: GetMessageIndex () const {

	return PeakOutOfPlaceInLocus :: sMessageIndex;
}



int PeakOutOfPlaceInLocus :: GetScope () const {

	return PeakOutOfPlaceInLocus :: sMessageScope;
}



int PeakOutOfPlaceInLocus :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakOutOfPlaceInLocus* noticeType = new PeakOutOfPlaceInLocus;
	warehouse->AddType (noticeType);
	return 1;
}





PullUpPrimaryChannel :: PullUpPrimaryChannel () : Notice () {

}


PullUpPrimaryChannel :: PullUpPrimaryChannel (const PullUpPrimaryChannel& note) : Notice ((const Notice&) note) {

}



PullUpPrimaryChannel :: ~PullUpPrimaryChannel () {

}



int PullUpPrimaryChannel :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PullUpPrimaryChannel::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PullUpPrimaryChannel::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PullUpPrimaryChannel::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PullUpPrimaryChannel::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PullUpPrimaryChannel::sLabel = temp;

	else
		PullUpPrimaryChannel::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PullUpPrimaryChannel::sTriggerLowPriority = true;

		else
			PullUpPrimaryChannel::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PullUpPrimaryChannel::sDoNotCall = true;

		else
			PullUpPrimaryChannel::sDoNotCall = false;
	}


	int N = PullUpPrimaryChannel::sMessageList.Entries ();

	if (N == 1)
		N++;

	PullUpPrimaryChannel::sMessageArray = new RGString* [N];
	RGDListIterator it (PullUpPrimaryChannel::sMessageList);

	for (int i=0; i<N; i++)
		PullUpPrimaryChannel::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PullUpPrimaryChannel :: GetSubject () const {

	return PullUpPrimaryChannel::sSubject;
}


RGString PullUpPrimaryChannel :: GetLabel () const {

	return PullUpPrimaryChannel::sLabel;
}


bool PullUpPrimaryChannel :: IsTriggerForLowPriority () const {

	return PullUpPrimaryChannel::sTriggerLowPriority;
}


bool PullUpPrimaryChannel :: IsDoNotCall () const {

	return PullUpPrimaryChannel::sDoNotCall;
}



RGString PullUpPrimaryChannel :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PullUpPrimaryChannel::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PullUpPrimaryChannel::sMessageArray [i];
	return returnValue;
}



int PullUpPrimaryChannel :: GetMessageLevel () const {

	return PullUpPrimaryChannel::sMessageLevel;
}



int PullUpPrimaryChannel :: GetNumberOfMessageParts () const {

	return PullUpPrimaryChannel::sMessageList.Entries ();
}


int PullUpPrimaryChannel :: GetSeverityLevel () const {

	return PullUpPrimaryChannel::sMessageLevel;
}


void PullUpPrimaryChannel :: SetIndexAndScope (int index, int scope) const {

	PullUpPrimaryChannel::sMessageIndex = index;
	PullUpPrimaryChannel::sMessageScope = scope;
}


int PullUpPrimaryChannel :: GetMessageIndex () const {

	return PullUpPrimaryChannel :: sMessageIndex;
}



int PullUpPrimaryChannel :: GetScope () const {

	return PullUpPrimaryChannel :: sMessageScope;
}



int PullUpPrimaryChannel :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PullUpPrimaryChannel* noticeType = new PullUpPrimaryChannel;
	warehouse->AddType (noticeType);
	return 1;
}





AllelePullUp :: AllelePullUp () : Notice () {

}


AllelePullUp :: AllelePullUp (const AllelePullUp& note) : Notice ((const Notice&) note) {

}



AllelePullUp :: ~AllelePullUp () {

}



int AllelePullUp :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (AllelePullUp::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		AllelePullUp::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	AllelePullUp::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	AllelePullUp::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		AllelePullUp::sLabel = temp;

	else
		AllelePullUp::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			AllelePullUp::sTriggerLowPriority = true;

		else
			AllelePullUp::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			AllelePullUp::sDoNotCall = true;

		else
			AllelePullUp::sDoNotCall = false;
	}


	int N = AllelePullUp::sMessageList.Entries ();

	if (N == 1)
		N++;

	AllelePullUp::sMessageArray = new RGString* [N];
	RGDListIterator it (AllelePullUp::sMessageList);

	for (int i=0; i<N; i++)
		AllelePullUp::sMessageArray [i] = (RGString*) it ();

	return status;
}


int AllelePullUp :: GetSubject () const {

	return AllelePullUp::sSubject;
}


RGString AllelePullUp :: GetLabel () const {

	return AllelePullUp::sLabel;
}


bool AllelePullUp :: IsTriggerForLowPriority () const {

	return AllelePullUp::sTriggerLowPriority;
}


bool AllelePullUp :: IsDoNotCall () const {

	return AllelePullUp::sDoNotCall;
}



RGString AllelePullUp :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= AllelePullUp::sMessageList.Entries ()))
		return returnValue;

	returnValue = *AllelePullUp::sMessageArray [i];
	return returnValue;
}



int AllelePullUp :: GetMessageLevel () const {

	return AllelePullUp::sMessageLevel;
}



int AllelePullUp :: GetNumberOfMessageParts () const {

	return AllelePullUp::sMessageList.Entries ();
}


int AllelePullUp :: GetSeverityLevel () const {

	return AllelePullUp::sMessageLevel;
}


void AllelePullUp :: SetIndexAndScope (int index, int scope) const {

	AllelePullUp::sMessageIndex = index;
	AllelePullUp::sMessageScope = scope;
}


int AllelePullUp :: GetMessageIndex () const {

	return AllelePullUp :: sMessageIndex;
}



int AllelePullUp :: GetScope () const {

	return AllelePullUp :: sMessageScope;
}



int AllelePullUp :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	AllelePullUp* noticeType = new AllelePullUp;
	warehouse->AddType (noticeType);
	return 1;
}





HeteroImbalancePullUp :: HeteroImbalancePullUp () : Notice () {

}


HeteroImbalancePullUp :: HeteroImbalancePullUp (const HeteroImbalancePullUp& note) : Notice ((const Notice&) note) {

}



HeteroImbalancePullUp :: ~HeteroImbalancePullUp () {

}



int HeteroImbalancePullUp :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (HeteroImbalancePullUp::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		HeteroImbalancePullUp::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	HeteroImbalancePullUp::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	HeteroImbalancePullUp::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		HeteroImbalancePullUp::sLabel = temp;

	else
		HeteroImbalancePullUp::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			HeteroImbalancePullUp::sTriggerLowPriority = true;

		else
			HeteroImbalancePullUp::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			HeteroImbalancePullUp::sDoNotCall = true;

		else
			HeteroImbalancePullUp::sDoNotCall = false;
	}


	int N = HeteroImbalancePullUp::sMessageList.Entries ();

	if (N == 1)
		N++;

	HeteroImbalancePullUp::sMessageArray = new RGString* [N];
	RGDListIterator it (HeteroImbalancePullUp::sMessageList);

	for (int i=0; i<N; i++)
		HeteroImbalancePullUp::sMessageArray [i] = (RGString*) it ();

	return status;
}


int HeteroImbalancePullUp :: GetSubject () const {

	return HeteroImbalancePullUp::sSubject;
}


RGString HeteroImbalancePullUp :: GetLabel () const {

	return HeteroImbalancePullUp::sLabel;
}


bool HeteroImbalancePullUp :: IsTriggerForLowPriority () const {

	return HeteroImbalancePullUp::sTriggerLowPriority;
}


bool HeteroImbalancePullUp :: IsDoNotCall () const {

	return HeteroImbalancePullUp::sDoNotCall;
}



RGString HeteroImbalancePullUp :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= HeteroImbalancePullUp::sMessageList.Entries ()))
		return returnValue;

	returnValue = *HeteroImbalancePullUp::sMessageArray [i];
	return returnValue;
}



int HeteroImbalancePullUp :: GetMessageLevel () const {

	return HeteroImbalancePullUp::sMessageLevel;
}



int HeteroImbalancePullUp :: GetNumberOfMessageParts () const {

	return HeteroImbalancePullUp::sMessageList.Entries ();
}


int HeteroImbalancePullUp :: GetSeverityLevel () const {

	return HeteroImbalancePullUp::sMessageLevel;
}


void HeteroImbalancePullUp :: SetIndexAndScope (int index, int scope) const {

	HeteroImbalancePullUp::sMessageIndex = index;
	HeteroImbalancePullUp::sMessageScope = scope;
}


int HeteroImbalancePullUp :: GetMessageIndex () const {

	return HeteroImbalancePullUp :: sMessageIndex;
}



int HeteroImbalancePullUp :: GetScope () const {

	return HeteroImbalancePullUp :: sMessageScope;
}



int HeteroImbalancePullUp :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	HeteroImbalancePullUp* noticeType = new HeteroImbalancePullUp;
	warehouse->AddType (noticeType);
	return 1;
}





LowTriAllelePullUp :: LowTriAllelePullUp () : Notice () {

}


LowTriAllelePullUp :: LowTriAllelePullUp (const LowTriAllelePullUp& note) : Notice ((const Notice&) note) {

}



LowTriAllelePullUp :: ~LowTriAllelePullUp () {

}



int LowTriAllelePullUp :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LowTriAllelePullUp::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LowTriAllelePullUp::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LowTriAllelePullUp::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LowTriAllelePullUp::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LowTriAllelePullUp::sLabel = temp;

	else
		LowTriAllelePullUp::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LowTriAllelePullUp::sTriggerLowPriority = true;

		else
			LowTriAllelePullUp::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LowTriAllelePullUp::sDoNotCall = true;

		else
			LowTriAllelePullUp::sDoNotCall = false;
	}


	int N = LowTriAllelePullUp::sMessageList.Entries ();

	if (N == 1)
		N++;

	LowTriAllelePullUp::sMessageArray = new RGString* [N];
	RGDListIterator it (LowTriAllelePullUp::sMessageList);

	for (int i=0; i<N; i++)
		LowTriAllelePullUp::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LowTriAllelePullUp :: GetSubject () const {

	return LowTriAllelePullUp::sSubject;
}


RGString LowTriAllelePullUp :: GetLabel () const {

	return LowTriAllelePullUp::sLabel;
}


bool LowTriAllelePullUp :: IsTriggerForLowPriority () const {

	return LowTriAllelePullUp::sTriggerLowPriority;
}


bool LowTriAllelePullUp :: IsDoNotCall () const {

	return LowTriAllelePullUp::sDoNotCall;
}



RGString LowTriAllelePullUp :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LowTriAllelePullUp::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LowTriAllelePullUp::sMessageArray [i];
	return returnValue;
}



int LowTriAllelePullUp :: GetMessageLevel () const {

	return LowTriAllelePullUp::sMessageLevel;
}



int LowTriAllelePullUp :: GetNumberOfMessageParts () const {

	return LowTriAllelePullUp::sMessageList.Entries ();
}


int LowTriAllelePullUp :: GetSeverityLevel () const {

	return LowTriAllelePullUp::sMessageLevel;
}


void LowTriAllelePullUp :: SetIndexAndScope (int index, int scope) const {

	LowTriAllelePullUp::sMessageIndex = index;
	LowTriAllelePullUp::sMessageScope = scope;
}


int LowTriAllelePullUp :: GetMessageIndex () const {

	return LowTriAllelePullUp :: sMessageIndex;
}



int LowTriAllelePullUp :: GetScope () const {

	return LowTriAllelePullUp :: sMessageScope;
}



int LowTriAllelePullUp :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LowTriAllelePullUp* noticeType = new LowTriAllelePullUp;
	warehouse->AddType (noticeType);
	return 1;
}





LowOLAllelePullUp :: LowOLAllelePullUp () : Notice () {

}


LowOLAllelePullUp :: LowOLAllelePullUp (const LowOLAllelePullUp& note) : Notice ((const Notice&) note) {

}



LowOLAllelePullUp :: ~LowOLAllelePullUp () {

}



int LowOLAllelePullUp :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LowOLAllelePullUp::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LowOLAllelePullUp::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LowOLAllelePullUp::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LowOLAllelePullUp::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LowOLAllelePullUp::sLabel = temp;

	else
		LowOLAllelePullUp::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LowOLAllelePullUp::sTriggerLowPriority = true;

		else
			LowOLAllelePullUp::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LowOLAllelePullUp::sDoNotCall = true;

		else
			LowOLAllelePullUp::sDoNotCall = false;
	}


	int N = LowOLAllelePullUp::sMessageList.Entries ();

	if (N == 1)
		N++;

	LowOLAllelePullUp::sMessageArray = new RGString* [N];
	RGDListIterator it (LowOLAllelePullUp::sMessageList);

	for (int i=0; i<N; i++)
		LowOLAllelePullUp::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LowOLAllelePullUp :: GetSubject () const {

	return LowOLAllelePullUp::sSubject;
}


RGString LowOLAllelePullUp :: GetLabel () const {

	return LowOLAllelePullUp::sLabel;
}


bool LowOLAllelePullUp :: IsTriggerForLowPriority () const {

	return LowOLAllelePullUp::sTriggerLowPriority;
}


bool LowOLAllelePullUp :: IsDoNotCall () const {

	return LowOLAllelePullUp::sDoNotCall;
}



RGString LowOLAllelePullUp :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LowOLAllelePullUp::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LowOLAllelePullUp::sMessageArray [i];
	return returnValue;
}



int LowOLAllelePullUp :: GetMessageLevel () const {

	return LowOLAllelePullUp::sMessageLevel;
}



int LowOLAllelePullUp :: GetNumberOfMessageParts () const {

	return LowOLAllelePullUp::sMessageList.Entries ();
}


int LowOLAllelePullUp :: GetSeverityLevel () const {

	return LowOLAllelePullUp::sMessageLevel;
}


void LowOLAllelePullUp :: SetIndexAndScope (int index, int scope) const {

	LowOLAllelePullUp::sMessageIndex = index;
	LowOLAllelePullUp::sMessageScope = scope;
}


int LowOLAllelePullUp :: GetMessageIndex () const {

	return LowOLAllelePullUp :: sMessageIndex;
}



int LowOLAllelePullUp :: GetScope () const {

	return LowOLAllelePullUp :: sMessageScope;
}



int LowOLAllelePullUp :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LowOLAllelePullUp* noticeType = new LowOLAllelePullUp;
	warehouse->AddType (noticeType);
	return 1;
}





PeakMisorderPullUp :: PeakMisorderPullUp () : Notice () {

}


PeakMisorderPullUp :: PeakMisorderPullUp (const PeakMisorderPullUp& note) : Notice ((const Notice&) note) {

}



PeakMisorderPullUp :: ~PeakMisorderPullUp () {

}



int PeakMisorderPullUp :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakMisorderPullUp::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakMisorderPullUp::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakMisorderPullUp::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakMisorderPullUp::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakMisorderPullUp::sLabel = temp;

	else
		PeakMisorderPullUp::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakMisorderPullUp::sTriggerLowPriority = true;

		else
			PeakMisorderPullUp::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakMisorderPullUp::sDoNotCall = true;

		else
			PeakMisorderPullUp::sDoNotCall = false;
	}


	int N = PeakMisorderPullUp::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakMisorderPullUp::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakMisorderPullUp::sMessageList);

	for (int i=0; i<N; i++)
		PeakMisorderPullUp::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakMisorderPullUp :: GetSubject () const {

	return PeakMisorderPullUp::sSubject;
}


RGString PeakMisorderPullUp :: GetLabel () const {

	return PeakMisorderPullUp::sLabel;
}


bool PeakMisorderPullUp :: IsTriggerForLowPriority () const {

	return PeakMisorderPullUp::sTriggerLowPriority;
}


bool PeakMisorderPullUp :: IsDoNotCall () const {

	return PeakMisorderPullUp::sDoNotCall;
}



RGString PeakMisorderPullUp :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakMisorderPullUp::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakMisorderPullUp::sMessageArray [i];
	return returnValue;
}



int PeakMisorderPullUp :: GetMessageLevel () const {

	return PeakMisorderPullUp::sMessageLevel;
}



int PeakMisorderPullUp :: GetNumberOfMessageParts () const {

	return PeakMisorderPullUp::sMessageList.Entries ();
}


int PeakMisorderPullUp :: GetSeverityLevel () const {

	return PeakMisorderPullUp::sMessageLevel;
}


void PeakMisorderPullUp :: SetIndexAndScope (int index, int scope) const {

	PeakMisorderPullUp::sMessageIndex = index;
	PeakMisorderPullUp::sMessageScope = scope;
}


int PeakMisorderPullUp :: GetMessageIndex () const {

	return PeakMisorderPullUp :: sMessageIndex;
}



int PeakMisorderPullUp :: GetScope () const {

	return PeakMisorderPullUp :: sMessageScope;
}



int PeakMisorderPullUp :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakMisorderPullUp* noticeType = new PeakMisorderPullUp;
	warehouse->AddType (noticeType);
	return 1;
}





IgnoreLowOLAlleles :: IgnoreLowOLAlleles () : Notice () {

}


IgnoreLowOLAlleles :: IgnoreLowOLAlleles (const IgnoreLowOLAlleles& note) : Notice ((const Notice&) note) {

}



IgnoreLowOLAlleles :: ~IgnoreLowOLAlleles () {

}



int IgnoreLowOLAlleles :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (IgnoreLowOLAlleles::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		IgnoreLowOLAlleles::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	IgnoreLowOLAlleles::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	IgnoreLowOLAlleles::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		IgnoreLowOLAlleles::sLabel = temp;

	else
		IgnoreLowOLAlleles::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			IgnoreLowOLAlleles::sTriggerLowPriority = true;

		else
			IgnoreLowOLAlleles::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			IgnoreLowOLAlleles::sDoNotCall = true;

		else
			IgnoreLowOLAlleles::sDoNotCall = false;
	}


	int N = IgnoreLowOLAlleles::sMessageList.Entries ();

	if (N == 1)
		N++;

	IgnoreLowOLAlleles::sMessageArray = new RGString* [N];
	RGDListIterator it (IgnoreLowOLAlleles::sMessageList);

	for (int i=0; i<N; i++)
		IgnoreLowOLAlleles::sMessageArray [i] = (RGString*) it ();

	return status;
}


int IgnoreLowOLAlleles :: GetSubject () const {

	return IgnoreLowOLAlleles::sSubject;
}


RGString IgnoreLowOLAlleles :: GetLabel () const {

	return IgnoreLowOLAlleles::sLabel;
}


bool IgnoreLowOLAlleles :: IsTriggerForLowPriority () const {

	return IgnoreLowOLAlleles::sTriggerLowPriority;
}


bool IgnoreLowOLAlleles :: IsDoNotCall () const {

	return IgnoreLowOLAlleles::sDoNotCall;
}



RGString IgnoreLowOLAlleles :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= IgnoreLowOLAlleles::sMessageList.Entries ()))
		return returnValue;

	returnValue = *IgnoreLowOLAlleles::sMessageArray [i];
	return returnValue;
}



int IgnoreLowOLAlleles :: GetMessageLevel () const {

	return IgnoreLowOLAlleles::sMessageLevel;
}



int IgnoreLowOLAlleles :: GetNumberOfMessageParts () const {

	return IgnoreLowOLAlleles::sMessageList.Entries ();
}


int IgnoreLowOLAlleles :: GetSeverityLevel () const {

	return IgnoreLowOLAlleles::sMessageLevel;
}


void IgnoreLowOLAlleles :: SetIndexAndScope (int index, int scope) const {

	IgnoreLowOLAlleles::sMessageIndex = index;
	IgnoreLowOLAlleles::sMessageScope = scope;
}


int IgnoreLowOLAlleles :: GetMessageIndex () const {

	return IgnoreLowOLAlleles :: sMessageIndex;
}



int IgnoreLowOLAlleles :: GetScope () const {

	return IgnoreLowOLAlleles :: sMessageScope;
}



int IgnoreLowOLAlleles :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	IgnoreLowOLAlleles* noticeType = new IgnoreLowOLAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





PeakMisorder :: PeakMisorder () : Notice () {

}


PeakMisorder :: PeakMisorder (const PeakMisorder& note) : Notice ((const Notice&) note) {

}



PeakMisorder :: ~PeakMisorder () {

}



int PeakMisorder :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakMisorder::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakMisorder::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakMisorder::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakMisorder::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakMisorder::sLabel = temp;

	else
		PeakMisorder::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakMisorder::sTriggerLowPriority = true;

		else
			PeakMisorder::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakMisorder::sDoNotCall = true;

		else
			PeakMisorder::sDoNotCall = false;
	}


	int N = PeakMisorder::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakMisorder::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakMisorder::sMessageList);

	for (int i=0; i<N; i++)
		PeakMisorder::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakMisorder :: GetSubject () const {

	return PeakMisorder::sSubject;
}


RGString PeakMisorder :: GetLabel () const {

	return PeakMisorder::sLabel;
}


bool PeakMisorder :: IsTriggerForLowPriority () const {

	return PeakMisorder::sTriggerLowPriority;
}


bool PeakMisorder :: IsDoNotCall () const {

	return PeakMisorder::sDoNotCall;
}



RGString PeakMisorder :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakMisorder::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakMisorder::sMessageArray [i];
	return returnValue;
}



int PeakMisorder :: GetMessageLevel () const {

	return PeakMisorder::sMessageLevel;
}



int PeakMisorder :: GetNumberOfMessageParts () const {

	return PeakMisorder::sMessageList.Entries ();
}


int PeakMisorder :: GetSeverityLevel () const {

	return PeakMisorder::sMessageLevel;
}


void PeakMisorder :: SetIndexAndScope (int index, int scope) const {

	PeakMisorder::sMessageIndex = index;
	PeakMisorder::sMessageScope = scope;
}


int PeakMisorder :: GetMessageIndex () const {

	return PeakMisorder :: sMessageIndex;
}



int PeakMisorder :: GetScope () const {

	return PeakMisorder :: sMessageScope;
}



int PeakMisorder :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakMisorder* noticeType = new PeakMisorder;
	warehouse->AddType (noticeType);
	return 1;
}





ControlPeakPullUp :: ControlPeakPullUp () : Notice () {

}


ControlPeakPullUp :: ControlPeakPullUp (const ControlPeakPullUp& note) : Notice ((const Notice&) note) {

}



ControlPeakPullUp :: ~ControlPeakPullUp () {

}



int ControlPeakPullUp :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ControlPeakPullUp::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ControlPeakPullUp::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ControlPeakPullUp::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ControlPeakPullUp::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ControlPeakPullUp::sLabel = temp;

	else
		ControlPeakPullUp::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ControlPeakPullUp::sTriggerLowPriority = true;

		else
			ControlPeakPullUp::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ControlPeakPullUp::sDoNotCall = true;

		else
			ControlPeakPullUp::sDoNotCall = false;
	}


	int N = ControlPeakPullUp::sMessageList.Entries ();

	if (N == 1)
		N++;

	ControlPeakPullUp::sMessageArray = new RGString* [N];
	RGDListIterator it (ControlPeakPullUp::sMessageList);

	for (int i=0; i<N; i++)
		ControlPeakPullUp::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ControlPeakPullUp :: GetSubject () const {

	return ControlPeakPullUp::sSubject;
}


RGString ControlPeakPullUp :: GetLabel () const {

	return ControlPeakPullUp::sLabel;
}


bool ControlPeakPullUp :: IsTriggerForLowPriority () const {

	return ControlPeakPullUp::sTriggerLowPriority;
}


bool ControlPeakPullUp :: IsDoNotCall () const {

	return ControlPeakPullUp::sDoNotCall;
}



RGString ControlPeakPullUp :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ControlPeakPullUp::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ControlPeakPullUp::sMessageArray [i];
	return returnValue;
}



int ControlPeakPullUp :: GetMessageLevel () const {

	return ControlPeakPullUp::sMessageLevel;
}



int ControlPeakPullUp :: GetNumberOfMessageParts () const {

	return ControlPeakPullUp::sMessageList.Entries ();
}


int ControlPeakPullUp :: GetSeverityLevel () const {

	return ControlPeakPullUp::sMessageLevel;
}


void ControlPeakPullUp :: SetIndexAndScope (int index, int scope) const {

	ControlPeakPullUp::sMessageIndex = index;
	ControlPeakPullUp::sMessageScope = scope;
}


int ControlPeakPullUp :: GetMessageIndex () const {

	return ControlPeakPullUp :: sMessageIndex;
}



int ControlPeakPullUp :: GetScope () const {

	return ControlPeakPullUp :: sMessageScope;
}



int ControlPeakPullUp :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ControlPeakPullUp* noticeType = new ControlPeakPullUp;
	warehouse->AddType (noticeType);
	return 1;
}





OLBetweenLociPullUp :: OLBetweenLociPullUp () : Notice () {

}


OLBetweenLociPullUp :: OLBetweenLociPullUp (const OLBetweenLociPullUp& note) : Notice ((const Notice&) note) {

}



OLBetweenLociPullUp :: ~OLBetweenLociPullUp () {

}



int OLBetweenLociPullUp :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (OLBetweenLociPullUp::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		OLBetweenLociPullUp::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OLBetweenLociPullUp::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OLBetweenLociPullUp::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		OLBetweenLociPullUp::sLabel = temp;

	else
		OLBetweenLociPullUp::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OLBetweenLociPullUp::sTriggerLowPriority = true;

		else
			OLBetweenLociPullUp::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OLBetweenLociPullUp::sDoNotCall = true;

		else
			OLBetweenLociPullUp::sDoNotCall = false;
	}


	int N = OLBetweenLociPullUp::sMessageList.Entries ();

	if (N == 1)
		N++;

	OLBetweenLociPullUp::sMessageArray = new RGString* [N];
	RGDListIterator it (OLBetweenLociPullUp::sMessageList);

	for (int i=0; i<N; i++)
		OLBetweenLociPullUp::sMessageArray [i] = (RGString*) it ();

	return status;
}


int OLBetweenLociPullUp :: GetSubject () const {

	return OLBetweenLociPullUp::sSubject;
}


RGString OLBetweenLociPullUp :: GetLabel () const {

	return OLBetweenLociPullUp::sLabel;
}


bool OLBetweenLociPullUp :: IsTriggerForLowPriority () const {

	return OLBetweenLociPullUp::sTriggerLowPriority;
}


bool OLBetweenLociPullUp :: IsDoNotCall () const {

	return OLBetweenLociPullUp::sDoNotCall;
}



RGString OLBetweenLociPullUp :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= OLBetweenLociPullUp::sMessageList.Entries ()))
		return returnValue;

	returnValue = *OLBetweenLociPullUp::sMessageArray [i];
	return returnValue;
}



int OLBetweenLociPullUp :: GetMessageLevel () const {

	return OLBetweenLociPullUp::sMessageLevel;
}



int OLBetweenLociPullUp :: GetNumberOfMessageParts () const {

	return OLBetweenLociPullUp::sMessageList.Entries ();
}


int OLBetweenLociPullUp :: GetSeverityLevel () const {

	return OLBetweenLociPullUp::sMessageLevel;
}


void OLBetweenLociPullUp :: SetIndexAndScope (int index, int scope) const {

	OLBetweenLociPullUp::sMessageIndex = index;
	OLBetweenLociPullUp::sMessageScope = scope;
}


int OLBetweenLociPullUp :: GetMessageIndex () const {

	return OLBetweenLociPullUp :: sMessageIndex;
}



int OLBetweenLociPullUp :: GetScope () const {

	return OLBetweenLociPullUp :: sMessageScope;
}



int OLBetweenLociPullUp :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	OLBetweenLociPullUp* noticeType = new OLBetweenLociPullUp;
	warehouse->AddType (noticeType);
	return 1;
}





IgnoreLowOnLadderAlleles :: IgnoreLowOnLadderAlleles () : Notice () {

}


IgnoreLowOnLadderAlleles :: IgnoreLowOnLadderAlleles (const IgnoreLowOnLadderAlleles& note) : Notice ((const Notice&) note) {

}



IgnoreLowOnLadderAlleles :: ~IgnoreLowOnLadderAlleles () {

}



int IgnoreLowOnLadderAlleles :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (IgnoreLowOnLadderAlleles::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		IgnoreLowOnLadderAlleles::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	IgnoreLowOnLadderAlleles::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	IgnoreLowOnLadderAlleles::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		IgnoreLowOnLadderAlleles::sLabel = temp;

	else
		IgnoreLowOnLadderAlleles::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			IgnoreLowOnLadderAlleles::sTriggerLowPriority = true;

		else
			IgnoreLowOnLadderAlleles::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			IgnoreLowOnLadderAlleles::sDoNotCall = true;

		else
			IgnoreLowOnLadderAlleles::sDoNotCall = false;
	}


	int N = IgnoreLowOnLadderAlleles::sMessageList.Entries ();

	if (N == 1)
		N++;

	IgnoreLowOnLadderAlleles::sMessageArray = new RGString* [N];
	RGDListIterator it (IgnoreLowOnLadderAlleles::sMessageList);

	for (int i=0; i<N; i++)
		IgnoreLowOnLadderAlleles::sMessageArray [i] = (RGString*) it ();

	return status;
}


int IgnoreLowOnLadderAlleles :: GetSubject () const {

	return IgnoreLowOnLadderAlleles::sSubject;
}


RGString IgnoreLowOnLadderAlleles :: GetLabel () const {

	return IgnoreLowOnLadderAlleles::sLabel;
}


bool IgnoreLowOnLadderAlleles :: IsTriggerForLowPriority () const {

	return IgnoreLowOnLadderAlleles::sTriggerLowPriority;
}


bool IgnoreLowOnLadderAlleles :: IsDoNotCall () const {

	return IgnoreLowOnLadderAlleles::sDoNotCall;
}



RGString IgnoreLowOnLadderAlleles :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= IgnoreLowOnLadderAlleles::sMessageList.Entries ()))
		return returnValue;

	returnValue = *IgnoreLowOnLadderAlleles::sMessageArray [i];
	return returnValue;
}



int IgnoreLowOnLadderAlleles :: GetMessageLevel () const {

	return IgnoreLowOnLadderAlleles::sMessageLevel;
}



int IgnoreLowOnLadderAlleles :: GetNumberOfMessageParts () const {

	return IgnoreLowOnLadderAlleles::sMessageList.Entries ();
}


int IgnoreLowOnLadderAlleles :: GetSeverityLevel () const {

	return IgnoreLowOnLadderAlleles::sMessageLevel;
}


void IgnoreLowOnLadderAlleles :: SetIndexAndScope (int index, int scope) const {

	IgnoreLowOnLadderAlleles::sMessageIndex = index;
	IgnoreLowOnLadderAlleles::sMessageScope = scope;
}


int IgnoreLowOnLadderAlleles :: GetMessageIndex () const {

	return IgnoreLowOnLadderAlleles :: sMessageIndex;
}



int IgnoreLowOnLadderAlleles :: GetScope () const {

	return IgnoreLowOnLadderAlleles :: sMessageScope;
}



int IgnoreLowOnLadderAlleles :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	IgnoreLowOnLadderAlleles* noticeType = new IgnoreLowOnLadderAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





CraterInducedByPullUp :: CraterInducedByPullUp () : Notice () {

}


CraterInducedByPullUp :: CraterInducedByPullUp (const CraterInducedByPullUp& note) : Notice ((const Notice&) note) {

}



CraterInducedByPullUp :: ~CraterInducedByPullUp () {

}



int CraterInducedByPullUp :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (CraterInducedByPullUp::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		CraterInducedByPullUp::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CraterInducedByPullUp::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CraterInducedByPullUp::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		CraterInducedByPullUp::sLabel = temp;

	else
		CraterInducedByPullUp::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CraterInducedByPullUp::sTriggerLowPriority = true;

		else
			CraterInducedByPullUp::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CraterInducedByPullUp::sDoNotCall = true;

		else
			CraterInducedByPullUp::sDoNotCall = false;
	}


	int N = CraterInducedByPullUp::sMessageList.Entries ();

	if (N == 1)
		N++;

	CraterInducedByPullUp::sMessageArray = new RGString* [N];
	RGDListIterator it (CraterInducedByPullUp::sMessageList);

	for (int i=0; i<N; i++)
		CraterInducedByPullUp::sMessageArray [i] = (RGString*) it ();

	return status;
}


int CraterInducedByPullUp :: GetSubject () const {

	return CraterInducedByPullUp::sSubject;
}


RGString CraterInducedByPullUp :: GetLabel () const {

	return CraterInducedByPullUp::sLabel;
}


bool CraterInducedByPullUp :: IsTriggerForLowPriority () const {

	return CraterInducedByPullUp::sTriggerLowPriority;
}


bool CraterInducedByPullUp :: IsDoNotCall () const {

	return CraterInducedByPullUp::sDoNotCall;
}



RGString CraterInducedByPullUp :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= CraterInducedByPullUp::sMessageList.Entries ()))
		return returnValue;

	returnValue = *CraterInducedByPullUp::sMessageArray [i];
	return returnValue;
}



int CraterInducedByPullUp :: GetMessageLevel () const {

	return CraterInducedByPullUp::sMessageLevel;
}



int CraterInducedByPullUp :: GetNumberOfMessageParts () const {

	return CraterInducedByPullUp::sMessageList.Entries ();
}


int CraterInducedByPullUp :: GetSeverityLevel () const {

	return CraterInducedByPullUp::sMessageLevel;
}


void CraterInducedByPullUp :: SetIndexAndScope (int index, int scope) const {

	CraterInducedByPullUp::sMessageIndex = index;
	CraterInducedByPullUp::sMessageScope = scope;
}


int CraterInducedByPullUp :: GetMessageIndex () const {

	return CraterInducedByPullUp :: sMessageIndex;
}



int CraterInducedByPullUp :: GetScope () const {

	return CraterInducedByPullUp :: sMessageScope;
}



int CraterInducedByPullUp :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	CraterInducedByPullUp* noticeType = new CraterInducedByPullUp;
	warehouse->AddType (noticeType);
	return 1;
}





CraterInducedByPullUpBetweenLoci :: CraterInducedByPullUpBetweenLoci () : Notice () {

}


CraterInducedByPullUpBetweenLoci :: CraterInducedByPullUpBetweenLoci (const CraterInducedByPullUpBetweenLoci& note) : Notice ((const Notice&) note) {

}



CraterInducedByPullUpBetweenLoci :: ~CraterInducedByPullUpBetweenLoci () {

}



int CraterInducedByPullUpBetweenLoci :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (CraterInducedByPullUpBetweenLoci::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		CraterInducedByPullUpBetweenLoci::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CraterInducedByPullUpBetweenLoci::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CraterInducedByPullUpBetweenLoci::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		CraterInducedByPullUpBetweenLoci::sLabel = temp;

	else
		CraterInducedByPullUpBetweenLoci::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CraterInducedByPullUpBetweenLoci::sTriggerLowPriority = true;

		else
			CraterInducedByPullUpBetweenLoci::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CraterInducedByPullUpBetweenLoci::sDoNotCall = true;

		else
			CraterInducedByPullUpBetweenLoci::sDoNotCall = false;
	}


	int N = CraterInducedByPullUpBetweenLoci::sMessageList.Entries ();

	if (N == 1)
		N++;

	CraterInducedByPullUpBetweenLoci::sMessageArray = new RGString* [N];
	RGDListIterator it (CraterInducedByPullUpBetweenLoci::sMessageList);

	for (int i=0; i<N; i++)
		CraterInducedByPullUpBetweenLoci::sMessageArray [i] = (RGString*) it ();

	return status;
}


int CraterInducedByPullUpBetweenLoci :: GetSubject () const {

	return CraterInducedByPullUpBetweenLoci::sSubject;
}


RGString CraterInducedByPullUpBetweenLoci :: GetLabel () const {

	return CraterInducedByPullUpBetweenLoci::sLabel;
}


bool CraterInducedByPullUpBetweenLoci :: IsTriggerForLowPriority () const {

	return CraterInducedByPullUpBetweenLoci::sTriggerLowPriority;
}


bool CraterInducedByPullUpBetweenLoci :: IsDoNotCall () const {

	return CraterInducedByPullUpBetweenLoci::sDoNotCall;
}



RGString CraterInducedByPullUpBetweenLoci :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= CraterInducedByPullUpBetweenLoci::sMessageList.Entries ()))
		return returnValue;

	returnValue = *CraterInducedByPullUpBetweenLoci::sMessageArray [i];
	return returnValue;
}



int CraterInducedByPullUpBetweenLoci :: GetMessageLevel () const {

	return CraterInducedByPullUpBetweenLoci::sMessageLevel;
}



int CraterInducedByPullUpBetweenLoci :: GetNumberOfMessageParts () const {

	return CraterInducedByPullUpBetweenLoci::sMessageList.Entries ();
}


int CraterInducedByPullUpBetweenLoci :: GetSeverityLevel () const {

	return CraterInducedByPullUpBetweenLoci::sMessageLevel;
}


void CraterInducedByPullUpBetweenLoci :: SetIndexAndScope (int index, int scope) const {

	CraterInducedByPullUpBetweenLoci::sMessageIndex = index;
	CraterInducedByPullUpBetweenLoci::sMessageScope = scope;
}


int CraterInducedByPullUpBetweenLoci :: GetMessageIndex () const {

	return CraterInducedByPullUpBetweenLoci :: sMessageIndex;
}



int CraterInducedByPullUpBetweenLoci :: GetScope () const {

	return CraterInducedByPullUpBetweenLoci :: sMessageScope;
}



int CraterInducedByPullUpBetweenLoci :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	CraterInducedByPullUpBetweenLoci* noticeType = new CraterInducedByPullUpBetweenLoci;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfPullUpsInSample :: NumberOfPullUpsInSample () : Notice () {

}


NumberOfPullUpsInSample :: NumberOfPullUpsInSample (const NumberOfPullUpsInSample& note) : Notice ((const Notice&) note) {

}



NumberOfPullUpsInSample :: ~NumberOfPullUpsInSample () {

}



int NumberOfPullUpsInSample :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfPullUpsInSample::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfPullUpsInSample::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfPullUpsInSample::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfPullUpsInSample::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfPullUpsInSample::sLabel = temp;

	else
		NumberOfPullUpsInSample::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfPullUpsInSample::sTriggerLowPriority = true;

		else
			NumberOfPullUpsInSample::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfPullUpsInSample::sDoNotCall = true;

		else
			NumberOfPullUpsInSample::sDoNotCall = false;
	}


	int N = NumberOfPullUpsInSample::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfPullUpsInSample::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfPullUpsInSample::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfPullUpsInSample::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfPullUpsInSample :: GetSubject () const {

	return NumberOfPullUpsInSample::sSubject;
}


RGString NumberOfPullUpsInSample :: GetLabel () const {

	return NumberOfPullUpsInSample::sLabel;
}


bool NumberOfPullUpsInSample :: IsTriggerForLowPriority () const {

	return NumberOfPullUpsInSample::sTriggerLowPriority;
}


bool NumberOfPullUpsInSample :: IsDoNotCall () const {

	return NumberOfPullUpsInSample::sDoNotCall;
}



RGString NumberOfPullUpsInSample :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfPullUpsInSample::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfPullUpsInSample::sMessageArray [i];
	return returnValue;
}



int NumberOfPullUpsInSample :: GetMessageLevel () const {

	return NumberOfPullUpsInSample::sMessageLevel;
}



int NumberOfPullUpsInSample :: GetNumberOfMessageParts () const {

	return NumberOfPullUpsInSample::sMessageList.Entries ();
}


int NumberOfPullUpsInSample :: GetSeverityLevel () const {

	return NumberOfPullUpsInSample::sMessageLevel;
}


void NumberOfPullUpsInSample :: SetIndexAndScope (int index, int scope) const {

	NumberOfPullUpsInSample::sMessageIndex = index;
	NumberOfPullUpsInSample::sMessageScope = scope;
}


int NumberOfPullUpsInSample :: GetMessageIndex () const {

	return NumberOfPullUpsInSample :: sMessageIndex;
}



int NumberOfPullUpsInSample :: GetScope () const {

	return NumberOfPullUpsInSample :: sMessageScope;
}



int NumberOfPullUpsInSample :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfPullUpsInSample* noticeType = new NumberOfPullUpsInSample;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfAdenylationsInSample :: NumberOfAdenylationsInSample () : Notice () {

}


NumberOfAdenylationsInSample :: NumberOfAdenylationsInSample (const NumberOfAdenylationsInSample& note) : Notice ((const Notice&) note) {

}



NumberOfAdenylationsInSample :: ~NumberOfAdenylationsInSample () {

}



int NumberOfAdenylationsInSample :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfAdenylationsInSample::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfAdenylationsInSample::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfAdenylationsInSample::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfAdenylationsInSample::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfAdenylationsInSample::sLabel = temp;

	else
		NumberOfAdenylationsInSample::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfAdenylationsInSample::sTriggerLowPriority = true;

		else
			NumberOfAdenylationsInSample::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfAdenylationsInSample::sDoNotCall = true;

		else
			NumberOfAdenylationsInSample::sDoNotCall = false;
	}


	int N = NumberOfAdenylationsInSample::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfAdenylationsInSample::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfAdenylationsInSample::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfAdenylationsInSample::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfAdenylationsInSample :: GetSubject () const {

	return NumberOfAdenylationsInSample::sSubject;
}


RGString NumberOfAdenylationsInSample :: GetLabel () const {

	return NumberOfAdenylationsInSample::sLabel;
}


bool NumberOfAdenylationsInSample :: IsTriggerForLowPriority () const {

	return NumberOfAdenylationsInSample::sTriggerLowPriority;
}


bool NumberOfAdenylationsInSample :: IsDoNotCall () const {

	return NumberOfAdenylationsInSample::sDoNotCall;
}



RGString NumberOfAdenylationsInSample :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfAdenylationsInSample::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfAdenylationsInSample::sMessageArray [i];
	return returnValue;
}



int NumberOfAdenylationsInSample :: GetMessageLevel () const {

	return NumberOfAdenylationsInSample::sMessageLevel;
}



int NumberOfAdenylationsInSample :: GetNumberOfMessageParts () const {

	return NumberOfAdenylationsInSample::sMessageList.Entries ();
}


int NumberOfAdenylationsInSample :: GetSeverityLevel () const {

	return NumberOfAdenylationsInSample::sMessageLevel;
}


void NumberOfAdenylationsInSample :: SetIndexAndScope (int index, int scope) const {

	NumberOfAdenylationsInSample::sMessageIndex = index;
	NumberOfAdenylationsInSample::sMessageScope = scope;
}


int NumberOfAdenylationsInSample :: GetMessageIndex () const {

	return NumberOfAdenylationsInSample :: sMessageIndex;
}



int NumberOfAdenylationsInSample :: GetScope () const {

	return NumberOfAdenylationsInSample :: sMessageScope;
}



int NumberOfAdenylationsInSample :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfAdenylationsInSample* noticeType = new NumberOfAdenylationsInSample;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfStutterPeaksInSample :: NumberOfStutterPeaksInSample () : Notice () {

}


NumberOfStutterPeaksInSample :: NumberOfStutterPeaksInSample (const NumberOfStutterPeaksInSample& note) : Notice ((const Notice&) note) {

}



NumberOfStutterPeaksInSample :: ~NumberOfStutterPeaksInSample () {

}



int NumberOfStutterPeaksInSample :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfStutterPeaksInSample::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfStutterPeaksInSample::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfStutterPeaksInSample::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfStutterPeaksInSample::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfStutterPeaksInSample::sLabel = temp;

	else
		NumberOfStutterPeaksInSample::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfStutterPeaksInSample::sTriggerLowPriority = true;

		else
			NumberOfStutterPeaksInSample::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfStutterPeaksInSample::sDoNotCall = true;

		else
			NumberOfStutterPeaksInSample::sDoNotCall = false;
	}


	int N = NumberOfStutterPeaksInSample::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfStutterPeaksInSample::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfStutterPeaksInSample::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfStutterPeaksInSample::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfStutterPeaksInSample :: GetSubject () const {

	return NumberOfStutterPeaksInSample::sSubject;
}


RGString NumberOfStutterPeaksInSample :: GetLabel () const {

	return NumberOfStutterPeaksInSample::sLabel;
}


bool NumberOfStutterPeaksInSample :: IsTriggerForLowPriority () const {

	return NumberOfStutterPeaksInSample::sTriggerLowPriority;
}


bool NumberOfStutterPeaksInSample :: IsDoNotCall () const {

	return NumberOfStutterPeaksInSample::sDoNotCall;
}



RGString NumberOfStutterPeaksInSample :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfStutterPeaksInSample::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfStutterPeaksInSample::sMessageArray [i];
	return returnValue;
}



int NumberOfStutterPeaksInSample :: GetMessageLevel () const {

	return NumberOfStutterPeaksInSample::sMessageLevel;
}



int NumberOfStutterPeaksInSample :: GetNumberOfMessageParts () const {

	return NumberOfStutterPeaksInSample::sMessageList.Entries ();
}


int NumberOfStutterPeaksInSample :: GetSeverityLevel () const {

	return NumberOfStutterPeaksInSample::sMessageLevel;
}


void NumberOfStutterPeaksInSample :: SetIndexAndScope (int index, int scope) const {

	NumberOfStutterPeaksInSample::sMessageIndex = index;
	NumberOfStutterPeaksInSample::sMessageScope = scope;
}


int NumberOfStutterPeaksInSample :: GetMessageIndex () const {

	return NumberOfStutterPeaksInSample :: sMessageIndex;
}



int NumberOfStutterPeaksInSample :: GetScope () const {

	return NumberOfStutterPeaksInSample :: sMessageScope;
}



int NumberOfStutterPeaksInSample :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfStutterPeaksInSample* noticeType = new NumberOfStutterPeaksInSample;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfPrimaryPullUpsInSample :: NumberOfPrimaryPullUpsInSample () : Notice () {

}


NumberOfPrimaryPullUpsInSample :: NumberOfPrimaryPullUpsInSample (const NumberOfPrimaryPullUpsInSample& note) : Notice ((const Notice&) note) {

}



NumberOfPrimaryPullUpsInSample :: ~NumberOfPrimaryPullUpsInSample () {

}



int NumberOfPrimaryPullUpsInSample :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfPrimaryPullUpsInSample::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfPrimaryPullUpsInSample::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfPrimaryPullUpsInSample::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfPrimaryPullUpsInSample::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfPrimaryPullUpsInSample::sLabel = temp;

	else
		NumberOfPrimaryPullUpsInSample::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfPrimaryPullUpsInSample::sTriggerLowPriority = true;

		else
			NumberOfPrimaryPullUpsInSample::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfPrimaryPullUpsInSample::sDoNotCall = true;

		else
			NumberOfPrimaryPullUpsInSample::sDoNotCall = false;
	}


	int N = NumberOfPrimaryPullUpsInSample::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfPrimaryPullUpsInSample::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfPrimaryPullUpsInSample::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfPrimaryPullUpsInSample::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfPrimaryPullUpsInSample :: GetSubject () const {

	return NumberOfPrimaryPullUpsInSample::sSubject;
}


RGString NumberOfPrimaryPullUpsInSample :: GetLabel () const {

	return NumberOfPrimaryPullUpsInSample::sLabel;
}


bool NumberOfPrimaryPullUpsInSample :: IsTriggerForLowPriority () const {

	return NumberOfPrimaryPullUpsInSample::sTriggerLowPriority;
}


bool NumberOfPrimaryPullUpsInSample :: IsDoNotCall () const {

	return NumberOfPrimaryPullUpsInSample::sDoNotCall;
}



RGString NumberOfPrimaryPullUpsInSample :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfPrimaryPullUpsInSample::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfPrimaryPullUpsInSample::sMessageArray [i];
	return returnValue;
}



int NumberOfPrimaryPullUpsInSample :: GetMessageLevel () const {

	return NumberOfPrimaryPullUpsInSample::sMessageLevel;
}



int NumberOfPrimaryPullUpsInSample :: GetNumberOfMessageParts () const {

	return NumberOfPrimaryPullUpsInSample::sMessageList.Entries ();
}


int NumberOfPrimaryPullUpsInSample :: GetSeverityLevel () const {

	return NumberOfPrimaryPullUpsInSample::sMessageLevel;
}


void NumberOfPrimaryPullUpsInSample :: SetIndexAndScope (int index, int scope) const {

	NumberOfPrimaryPullUpsInSample::sMessageIndex = index;
	NumberOfPrimaryPullUpsInSample::sMessageScope = scope;
}


int NumberOfPrimaryPullUpsInSample :: GetMessageIndex () const {

	return NumberOfPrimaryPullUpsInSample :: sMessageIndex;
}



int NumberOfPrimaryPullUpsInSample :: GetScope () const {

	return NumberOfPrimaryPullUpsInSample :: sMessageScope;
}



int NumberOfPrimaryPullUpsInSample :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfPrimaryPullUpsInSample* noticeType = new NumberOfPrimaryPullUpsInSample;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfOLAllelesWithinLociInSample :: NumberOfOLAllelesWithinLociInSample () : Notice () {

}


NumberOfOLAllelesWithinLociInSample :: NumberOfOLAllelesWithinLociInSample (const NumberOfOLAllelesWithinLociInSample& note) : Notice ((const Notice&) note) {

}



NumberOfOLAllelesWithinLociInSample :: ~NumberOfOLAllelesWithinLociInSample () {

}



int NumberOfOLAllelesWithinLociInSample :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfOLAllelesWithinLociInSample::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfOLAllelesWithinLociInSample::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfOLAllelesWithinLociInSample::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfOLAllelesWithinLociInSample::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfOLAllelesWithinLociInSample::sLabel = temp;

	else
		NumberOfOLAllelesWithinLociInSample::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfOLAllelesWithinLociInSample::sTriggerLowPriority = true;

		else
			NumberOfOLAllelesWithinLociInSample::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfOLAllelesWithinLociInSample::sDoNotCall = true;

		else
			NumberOfOLAllelesWithinLociInSample::sDoNotCall = false;
	}


	int N = NumberOfOLAllelesWithinLociInSample::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfOLAllelesWithinLociInSample::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfOLAllelesWithinLociInSample::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfOLAllelesWithinLociInSample::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfOLAllelesWithinLociInSample :: GetSubject () const {

	return NumberOfOLAllelesWithinLociInSample::sSubject;
}


RGString NumberOfOLAllelesWithinLociInSample :: GetLabel () const {

	return NumberOfOLAllelesWithinLociInSample::sLabel;
}


bool NumberOfOLAllelesWithinLociInSample :: IsTriggerForLowPriority () const {

	return NumberOfOLAllelesWithinLociInSample::sTriggerLowPriority;
}


bool NumberOfOLAllelesWithinLociInSample :: IsDoNotCall () const {

	return NumberOfOLAllelesWithinLociInSample::sDoNotCall;
}



RGString NumberOfOLAllelesWithinLociInSample :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfOLAllelesWithinLociInSample::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfOLAllelesWithinLociInSample::sMessageArray [i];
	return returnValue;
}



int NumberOfOLAllelesWithinLociInSample :: GetMessageLevel () const {

	return NumberOfOLAllelesWithinLociInSample::sMessageLevel;
}



int NumberOfOLAllelesWithinLociInSample :: GetNumberOfMessageParts () const {

	return NumberOfOLAllelesWithinLociInSample::sMessageList.Entries ();
}


int NumberOfOLAllelesWithinLociInSample :: GetSeverityLevel () const {

	return NumberOfOLAllelesWithinLociInSample::sMessageLevel;
}


void NumberOfOLAllelesWithinLociInSample :: SetIndexAndScope (int index, int scope) const {

	NumberOfOLAllelesWithinLociInSample::sMessageIndex = index;
	NumberOfOLAllelesWithinLociInSample::sMessageScope = scope;
}


int NumberOfOLAllelesWithinLociInSample :: GetMessageIndex () const {

	return NumberOfOLAllelesWithinLociInSample :: sMessageIndex;
}



int NumberOfOLAllelesWithinLociInSample :: GetScope () const {

	return NumberOfOLAllelesWithinLociInSample :: sMessageScope;
}



int NumberOfOLAllelesWithinLociInSample :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfOLAllelesWithinLociInSample* noticeType = new NumberOfOLAllelesWithinLociInSample;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfStutterPeaksExceedsThreshold :: NumberOfStutterPeaksExceedsThreshold () : Notice () {

}


NumberOfStutterPeaksExceedsThreshold :: NumberOfStutterPeaksExceedsThreshold (const NumberOfStutterPeaksExceedsThreshold& note) : Notice ((const Notice&) note) {

}



NumberOfStutterPeaksExceedsThreshold :: ~NumberOfStutterPeaksExceedsThreshold () {

}



int NumberOfStutterPeaksExceedsThreshold :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfStutterPeaksExceedsThreshold::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfStutterPeaksExceedsThreshold::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfStutterPeaksExceedsThreshold::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfStutterPeaksExceedsThreshold::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfStutterPeaksExceedsThreshold::sLabel = temp;

	else
		NumberOfStutterPeaksExceedsThreshold::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfStutterPeaksExceedsThreshold::sTriggerLowPriority = true;

		else
			NumberOfStutterPeaksExceedsThreshold::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfStutterPeaksExceedsThreshold::sDoNotCall = true;

		else
			NumberOfStutterPeaksExceedsThreshold::sDoNotCall = false;
	}


	int N = NumberOfStutterPeaksExceedsThreshold::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfStutterPeaksExceedsThreshold::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfStutterPeaksExceedsThreshold::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfStutterPeaksExceedsThreshold::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfStutterPeaksExceedsThreshold :: GetSubject () const {

	return NumberOfStutterPeaksExceedsThreshold::sSubject;
}


RGString NumberOfStutterPeaksExceedsThreshold :: GetLabel () const {

	return NumberOfStutterPeaksExceedsThreshold::sLabel;
}


bool NumberOfStutterPeaksExceedsThreshold :: IsTriggerForLowPriority () const {

	return NumberOfStutterPeaksExceedsThreshold::sTriggerLowPriority;
}


bool NumberOfStutterPeaksExceedsThreshold :: IsDoNotCall () const {

	return NumberOfStutterPeaksExceedsThreshold::sDoNotCall;
}



RGString NumberOfStutterPeaksExceedsThreshold :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfStutterPeaksExceedsThreshold::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfStutterPeaksExceedsThreshold::sMessageArray [i];
	return returnValue;
}



int NumberOfStutterPeaksExceedsThreshold :: GetMessageLevel () const {

	return NumberOfStutterPeaksExceedsThreshold::sMessageLevel;
}



int NumberOfStutterPeaksExceedsThreshold :: GetNumberOfMessageParts () const {

	return NumberOfStutterPeaksExceedsThreshold::sMessageList.Entries ();
}


int NumberOfStutterPeaksExceedsThreshold :: GetSeverityLevel () const {

	return NumberOfStutterPeaksExceedsThreshold::sMessageLevel;
}


void NumberOfStutterPeaksExceedsThreshold :: SetIndexAndScope (int index, int scope) const {

	NumberOfStutterPeaksExceedsThreshold::sMessageIndex = index;
	NumberOfStutterPeaksExceedsThreshold::sMessageScope = scope;
}


int NumberOfStutterPeaksExceedsThreshold :: GetMessageIndex () const {

	return NumberOfStutterPeaksExceedsThreshold :: sMessageIndex;
}



int NumberOfStutterPeaksExceedsThreshold :: GetScope () const {

	return NumberOfStutterPeaksExceedsThreshold :: sMessageScope;
}



int NumberOfStutterPeaksExceedsThreshold :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfStutterPeaksExceedsThreshold* noticeType = new NumberOfStutterPeaksExceedsThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfAdenylationPeaksExceedsThreshold :: NumberOfAdenylationPeaksExceedsThreshold () : Notice () {

}


NumberOfAdenylationPeaksExceedsThreshold :: NumberOfAdenylationPeaksExceedsThreshold (const NumberOfAdenylationPeaksExceedsThreshold& note) : Notice ((const Notice&) note) {

}



NumberOfAdenylationPeaksExceedsThreshold :: ~NumberOfAdenylationPeaksExceedsThreshold () {

}



int NumberOfAdenylationPeaksExceedsThreshold :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfAdenylationPeaksExceedsThreshold::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfAdenylationPeaksExceedsThreshold::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfAdenylationPeaksExceedsThreshold::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfAdenylationPeaksExceedsThreshold::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfAdenylationPeaksExceedsThreshold::sLabel = temp;

	else
		NumberOfAdenylationPeaksExceedsThreshold::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfAdenylationPeaksExceedsThreshold::sTriggerLowPriority = true;

		else
			NumberOfAdenylationPeaksExceedsThreshold::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfAdenylationPeaksExceedsThreshold::sDoNotCall = true;

		else
			NumberOfAdenylationPeaksExceedsThreshold::sDoNotCall = false;
	}


	int N = NumberOfAdenylationPeaksExceedsThreshold::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfAdenylationPeaksExceedsThreshold::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfAdenylationPeaksExceedsThreshold::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfAdenylationPeaksExceedsThreshold::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfAdenylationPeaksExceedsThreshold :: GetSubject () const {

	return NumberOfAdenylationPeaksExceedsThreshold::sSubject;
}


RGString NumberOfAdenylationPeaksExceedsThreshold :: GetLabel () const {

	return NumberOfAdenylationPeaksExceedsThreshold::sLabel;
}


bool NumberOfAdenylationPeaksExceedsThreshold :: IsTriggerForLowPriority () const {

	return NumberOfAdenylationPeaksExceedsThreshold::sTriggerLowPriority;
}


bool NumberOfAdenylationPeaksExceedsThreshold :: IsDoNotCall () const {

	return NumberOfAdenylationPeaksExceedsThreshold::sDoNotCall;
}



RGString NumberOfAdenylationPeaksExceedsThreshold :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfAdenylationPeaksExceedsThreshold::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfAdenylationPeaksExceedsThreshold::sMessageArray [i];
	return returnValue;
}



int NumberOfAdenylationPeaksExceedsThreshold :: GetMessageLevel () const {

	return NumberOfAdenylationPeaksExceedsThreshold::sMessageLevel;
}



int NumberOfAdenylationPeaksExceedsThreshold :: GetNumberOfMessageParts () const {

	return NumberOfAdenylationPeaksExceedsThreshold::sMessageList.Entries ();
}


int NumberOfAdenylationPeaksExceedsThreshold :: GetSeverityLevel () const {

	return NumberOfAdenylationPeaksExceedsThreshold::sMessageLevel;
}


void NumberOfAdenylationPeaksExceedsThreshold :: SetIndexAndScope (int index, int scope) const {

	NumberOfAdenylationPeaksExceedsThreshold::sMessageIndex = index;
	NumberOfAdenylationPeaksExceedsThreshold::sMessageScope = scope;
}


int NumberOfAdenylationPeaksExceedsThreshold :: GetMessageIndex () const {

	return NumberOfAdenylationPeaksExceedsThreshold :: sMessageIndex;
}



int NumberOfAdenylationPeaksExceedsThreshold :: GetScope () const {

	return NumberOfAdenylationPeaksExceedsThreshold :: sMessageScope;
}



int NumberOfAdenylationPeaksExceedsThreshold :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfAdenylationPeaksExceedsThreshold* noticeType = new NumberOfAdenylationPeaksExceedsThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfPullUpPeaksExceedsThreshold :: NumberOfPullUpPeaksExceedsThreshold () : Notice () {

}


NumberOfPullUpPeaksExceedsThreshold :: NumberOfPullUpPeaksExceedsThreshold (const NumberOfPullUpPeaksExceedsThreshold& note) : Notice ((const Notice&) note) {

}



NumberOfPullUpPeaksExceedsThreshold :: ~NumberOfPullUpPeaksExceedsThreshold () {

}



int NumberOfPullUpPeaksExceedsThreshold :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfPullUpPeaksExceedsThreshold::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfPullUpPeaksExceedsThreshold::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfPullUpPeaksExceedsThreshold::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfPullUpPeaksExceedsThreshold::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfPullUpPeaksExceedsThreshold::sLabel = temp;

	else
		NumberOfPullUpPeaksExceedsThreshold::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfPullUpPeaksExceedsThreshold::sTriggerLowPriority = true;

		else
			NumberOfPullUpPeaksExceedsThreshold::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfPullUpPeaksExceedsThreshold::sDoNotCall = true;

		else
			NumberOfPullUpPeaksExceedsThreshold::sDoNotCall = false;
	}


	int N = NumberOfPullUpPeaksExceedsThreshold::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfPullUpPeaksExceedsThreshold::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfPullUpPeaksExceedsThreshold::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfPullUpPeaksExceedsThreshold::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfPullUpPeaksExceedsThreshold :: GetSubject () const {

	return NumberOfPullUpPeaksExceedsThreshold::sSubject;
}


RGString NumberOfPullUpPeaksExceedsThreshold :: GetLabel () const {

	return NumberOfPullUpPeaksExceedsThreshold::sLabel;
}


bool NumberOfPullUpPeaksExceedsThreshold :: IsTriggerForLowPriority () const {

	return NumberOfPullUpPeaksExceedsThreshold::sTriggerLowPriority;
}


bool NumberOfPullUpPeaksExceedsThreshold :: IsDoNotCall () const {

	return NumberOfPullUpPeaksExceedsThreshold::sDoNotCall;
}



RGString NumberOfPullUpPeaksExceedsThreshold :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfPullUpPeaksExceedsThreshold::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfPullUpPeaksExceedsThreshold::sMessageArray [i];
	return returnValue;
}



int NumberOfPullUpPeaksExceedsThreshold :: GetMessageLevel () const {

	return NumberOfPullUpPeaksExceedsThreshold::sMessageLevel;
}



int NumberOfPullUpPeaksExceedsThreshold :: GetNumberOfMessageParts () const {

	return NumberOfPullUpPeaksExceedsThreshold::sMessageList.Entries ();
}


int NumberOfPullUpPeaksExceedsThreshold :: GetSeverityLevel () const {

	return NumberOfPullUpPeaksExceedsThreshold::sMessageLevel;
}


void NumberOfPullUpPeaksExceedsThreshold :: SetIndexAndScope (int index, int scope) const {

	NumberOfPullUpPeaksExceedsThreshold::sMessageIndex = index;
	NumberOfPullUpPeaksExceedsThreshold::sMessageScope = scope;
}


int NumberOfPullUpPeaksExceedsThreshold :: GetMessageIndex () const {

	return NumberOfPullUpPeaksExceedsThreshold :: sMessageIndex;
}



int NumberOfPullUpPeaksExceedsThreshold :: GetScope () const {

	return NumberOfPullUpPeaksExceedsThreshold :: sMessageScope;
}



int NumberOfPullUpPeaksExceedsThreshold :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfPullUpPeaksExceedsThreshold* noticeType = new NumberOfPullUpPeaksExceedsThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfOLAllelesExceedsThreshold :: NumberOfOLAllelesExceedsThreshold () : Notice () {

}


NumberOfOLAllelesExceedsThreshold :: NumberOfOLAllelesExceedsThreshold (const NumberOfOLAllelesExceedsThreshold& note) : Notice ((const Notice&) note) {

}



NumberOfOLAllelesExceedsThreshold :: ~NumberOfOLAllelesExceedsThreshold () {

}



int NumberOfOLAllelesExceedsThreshold :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfOLAllelesExceedsThreshold::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfOLAllelesExceedsThreshold::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfOLAllelesExceedsThreshold::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfOLAllelesExceedsThreshold::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfOLAllelesExceedsThreshold::sLabel = temp;

	else
		NumberOfOLAllelesExceedsThreshold::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfOLAllelesExceedsThreshold::sTriggerLowPriority = true;

		else
			NumberOfOLAllelesExceedsThreshold::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfOLAllelesExceedsThreshold::sDoNotCall = true;

		else
			NumberOfOLAllelesExceedsThreshold::sDoNotCall = false;
	}


	int N = NumberOfOLAllelesExceedsThreshold::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfOLAllelesExceedsThreshold::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfOLAllelesExceedsThreshold::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfOLAllelesExceedsThreshold::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfOLAllelesExceedsThreshold :: GetSubject () const {

	return NumberOfOLAllelesExceedsThreshold::sSubject;
}


RGString NumberOfOLAllelesExceedsThreshold :: GetLabel () const {

	return NumberOfOLAllelesExceedsThreshold::sLabel;
}


bool NumberOfOLAllelesExceedsThreshold :: IsTriggerForLowPriority () const {

	return NumberOfOLAllelesExceedsThreshold::sTriggerLowPriority;
}


bool NumberOfOLAllelesExceedsThreshold :: IsDoNotCall () const {

	return NumberOfOLAllelesExceedsThreshold::sDoNotCall;
}



RGString NumberOfOLAllelesExceedsThreshold :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfOLAllelesExceedsThreshold::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfOLAllelesExceedsThreshold::sMessageArray [i];
	return returnValue;
}



int NumberOfOLAllelesExceedsThreshold :: GetMessageLevel () const {

	return NumberOfOLAllelesExceedsThreshold::sMessageLevel;
}



int NumberOfOLAllelesExceedsThreshold :: GetNumberOfMessageParts () const {

	return NumberOfOLAllelesExceedsThreshold::sMessageList.Entries ();
}


int NumberOfOLAllelesExceedsThreshold :: GetSeverityLevel () const {

	return NumberOfOLAllelesExceedsThreshold::sMessageLevel;
}


void NumberOfOLAllelesExceedsThreshold :: SetIndexAndScope (int index, int scope) const {

	NumberOfOLAllelesExceedsThreshold::sMessageIndex = index;
	NumberOfOLAllelesExceedsThreshold::sMessageScope = scope;
}


int NumberOfOLAllelesExceedsThreshold :: GetMessageIndex () const {

	return NumberOfOLAllelesExceedsThreshold :: sMessageIndex;
}



int NumberOfOLAllelesExceedsThreshold :: GetScope () const {

	return NumberOfOLAllelesExceedsThreshold :: sMessageScope;
}



int NumberOfOLAllelesExceedsThreshold :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfOLAllelesExceedsThreshold* noticeType = new NumberOfOLAllelesExceedsThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfPrimaryPullUpPeaksExceedsThreshold :: NumberOfPrimaryPullUpPeaksExceedsThreshold () : Notice () {

}


NumberOfPrimaryPullUpPeaksExceedsThreshold :: NumberOfPrimaryPullUpPeaksExceedsThreshold (const NumberOfPrimaryPullUpPeaksExceedsThreshold& note) : Notice ((const Notice&) note) {

}



NumberOfPrimaryPullUpPeaksExceedsThreshold :: ~NumberOfPrimaryPullUpPeaksExceedsThreshold () {

}



int NumberOfPrimaryPullUpPeaksExceedsThreshold :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfPrimaryPullUpPeaksExceedsThreshold::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfPrimaryPullUpPeaksExceedsThreshold::sLabel = temp;

	else
		NumberOfPrimaryPullUpPeaksExceedsThreshold::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfPrimaryPullUpPeaksExceedsThreshold::sTriggerLowPriority = true;

		else
			NumberOfPrimaryPullUpPeaksExceedsThreshold::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfPrimaryPullUpPeaksExceedsThreshold::sDoNotCall = true;

		else
			NumberOfPrimaryPullUpPeaksExceedsThreshold::sDoNotCall = false;
	}


	int N = NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfPrimaryPullUpPeaksExceedsThreshold :: GetSubject () const {

	return NumberOfPrimaryPullUpPeaksExceedsThreshold::sSubject;
}


RGString NumberOfPrimaryPullUpPeaksExceedsThreshold :: GetLabel () const {

	return NumberOfPrimaryPullUpPeaksExceedsThreshold::sLabel;
}


bool NumberOfPrimaryPullUpPeaksExceedsThreshold :: IsTriggerForLowPriority () const {

	return NumberOfPrimaryPullUpPeaksExceedsThreshold::sTriggerLowPriority;
}


bool NumberOfPrimaryPullUpPeaksExceedsThreshold :: IsDoNotCall () const {

	return NumberOfPrimaryPullUpPeaksExceedsThreshold::sDoNotCall;
}



RGString NumberOfPrimaryPullUpPeaksExceedsThreshold :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageArray [i];
	return returnValue;
}



int NumberOfPrimaryPullUpPeaksExceedsThreshold :: GetMessageLevel () const {

	return NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageLevel;
}



int NumberOfPrimaryPullUpPeaksExceedsThreshold :: GetNumberOfMessageParts () const {

	return NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageList.Entries ();
}


int NumberOfPrimaryPullUpPeaksExceedsThreshold :: GetSeverityLevel () const {

	return NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageLevel;
}


void NumberOfPrimaryPullUpPeaksExceedsThreshold :: SetIndexAndScope (int index, int scope) const {

	NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageIndex = index;
	NumberOfPrimaryPullUpPeaksExceedsThreshold::sMessageScope = scope;
}


int NumberOfPrimaryPullUpPeaksExceedsThreshold :: GetMessageIndex () const {

	return NumberOfPrimaryPullUpPeaksExceedsThreshold :: sMessageIndex;
}



int NumberOfPrimaryPullUpPeaksExceedsThreshold :: GetScope () const {

	return NumberOfPrimaryPullUpPeaksExceedsThreshold :: sMessageScope;
}



int NumberOfPrimaryPullUpPeaksExceedsThreshold :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfPrimaryPullUpPeaksExceedsThreshold* noticeType = new NumberOfPrimaryPullUpPeaksExceedsThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





RemovablePrimaryPullUp :: RemovablePrimaryPullUp () : Notice () {

}


RemovablePrimaryPullUp :: RemovablePrimaryPullUp (const RemovablePrimaryPullUp& note) : Notice ((const Notice&) note) {

}



RemovablePrimaryPullUp :: ~RemovablePrimaryPullUp () {

}



int RemovablePrimaryPullUp :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (RemovablePrimaryPullUp::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		RemovablePrimaryPullUp::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	RemovablePrimaryPullUp::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	RemovablePrimaryPullUp::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		RemovablePrimaryPullUp::sLabel = temp;

	else
		RemovablePrimaryPullUp::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			RemovablePrimaryPullUp::sTriggerLowPriority = true;

		else
			RemovablePrimaryPullUp::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			RemovablePrimaryPullUp::sDoNotCall = true;

		else
			RemovablePrimaryPullUp::sDoNotCall = false;
	}


	int N = RemovablePrimaryPullUp::sMessageList.Entries ();

	if (N == 1)
		N++;

	RemovablePrimaryPullUp::sMessageArray = new RGString* [N];
	RGDListIterator it (RemovablePrimaryPullUp::sMessageList);

	for (int i=0; i<N; i++)
		RemovablePrimaryPullUp::sMessageArray [i] = (RGString*) it ();

	return status;
}


int RemovablePrimaryPullUp :: GetSubject () const {

	return RemovablePrimaryPullUp::sSubject;
}


RGString RemovablePrimaryPullUp :: GetLabel () const {

	return RemovablePrimaryPullUp::sLabel;
}


bool RemovablePrimaryPullUp :: IsTriggerForLowPriority () const {

	return RemovablePrimaryPullUp::sTriggerLowPriority;
}


bool RemovablePrimaryPullUp :: IsDoNotCall () const {

	return RemovablePrimaryPullUp::sDoNotCall;
}



RGString RemovablePrimaryPullUp :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= RemovablePrimaryPullUp::sMessageList.Entries ()))
		return returnValue;

	returnValue = *RemovablePrimaryPullUp::sMessageArray [i];
	return returnValue;
}



int RemovablePrimaryPullUp :: GetMessageLevel () const {

	return RemovablePrimaryPullUp::sMessageLevel;
}



int RemovablePrimaryPullUp :: GetNumberOfMessageParts () const {

	return RemovablePrimaryPullUp::sMessageList.Entries ();
}


int RemovablePrimaryPullUp :: GetSeverityLevel () const {

	return RemovablePrimaryPullUp::sMessageLevel;
}


void RemovablePrimaryPullUp :: SetIndexAndScope (int index, int scope) const {

	RemovablePrimaryPullUp::sMessageIndex = index;
	RemovablePrimaryPullUp::sMessageScope = scope;
}


int RemovablePrimaryPullUp :: GetMessageIndex () const {

	return RemovablePrimaryPullUp :: sMessageIndex;
}



int RemovablePrimaryPullUp :: GetScope () const {

	return RemovablePrimaryPullUp :: sMessageScope;
}



int RemovablePrimaryPullUp :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	RemovablePrimaryPullUp* noticeType = new RemovablePrimaryPullUp;
	warehouse->AddType (noticeType);
	return 1;
}





Crater :: Crater () : Notice () {

}


Crater :: Crater (const Crater& note) : Notice ((const Notice&) note) {

}



Crater :: ~Crater () {

}



int Crater :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (Crater::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		Crater::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	Crater::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	Crater::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		Crater::sLabel = temp;

	else
		Crater::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			Crater::sTriggerLowPriority = true;

		else
			Crater::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			Crater::sDoNotCall = true;

		else
			Crater::sDoNotCall = false;
	}


	int N = Crater::sMessageList.Entries ();

	if (N == 1)
		N++;

	Crater::sMessageArray = new RGString* [N];
	RGDListIterator it (Crater::sMessageList);

	for (int i=0; i<N; i++)
		Crater::sMessageArray [i] = (RGString*) it ();

	return status;
}


int Crater :: GetSubject () const {

	return Crater::sSubject;
}


RGString Crater :: GetLabel () const {

	return Crater::sLabel;
}


bool Crater :: IsTriggerForLowPriority () const {

	return Crater::sTriggerLowPriority;
}


bool Crater :: IsDoNotCall () const {

	return Crater::sDoNotCall;
}



RGString Crater :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= Crater::sMessageList.Entries ()))
		return returnValue;

	returnValue = *Crater::sMessageArray [i];
	return returnValue;
}



int Crater :: GetMessageLevel () const {

	return Crater::sMessageLevel;
}



int Crater :: GetNumberOfMessageParts () const {

	return Crater::sMessageList.Entries ();
}


int Crater :: GetSeverityLevel () const {

	return Crater::sMessageLevel;
}


void Crater :: SetIndexAndScope (int index, int scope) const {

	Crater::sMessageIndex = index;
	Crater::sMessageScope = scope;
}


int Crater :: GetMessageIndex () const {

	return Crater :: sMessageIndex;
}



int Crater :: GetScope () const {

	return Crater :: sMessageScope;
}



int Crater :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	Crater* noticeType = new Crater;
	warehouse->AddType (noticeType);
	return 1;
}





CraterBetweenLoci :: CraterBetweenLoci () : Notice () {

}


CraterBetweenLoci :: CraterBetweenLoci (const CraterBetweenLoci& note) : Notice ((const Notice&) note) {

}



CraterBetweenLoci :: ~CraterBetweenLoci () {

}



int CraterBetweenLoci :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (CraterBetweenLoci::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		CraterBetweenLoci::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CraterBetweenLoci::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CraterBetweenLoci::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		CraterBetweenLoci::sLabel = temp;

	else
		CraterBetweenLoci::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CraterBetweenLoci::sTriggerLowPriority = true;

		else
			CraterBetweenLoci::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CraterBetweenLoci::sDoNotCall = true;

		else
			CraterBetweenLoci::sDoNotCall = false;
	}


	int N = CraterBetweenLoci::sMessageList.Entries ();

	if (N == 1)
		N++;

	CraterBetweenLoci::sMessageArray = new RGString* [N];
	RGDListIterator it (CraterBetweenLoci::sMessageList);

	for (int i=0; i<N; i++)
		CraterBetweenLoci::sMessageArray [i] = (RGString*) it ();

	return status;
}


int CraterBetweenLoci :: GetSubject () const {

	return CraterBetweenLoci::sSubject;
}


RGString CraterBetweenLoci :: GetLabel () const {

	return CraterBetweenLoci::sLabel;
}


bool CraterBetweenLoci :: IsTriggerForLowPriority () const {

	return CraterBetweenLoci::sTriggerLowPriority;
}


bool CraterBetweenLoci :: IsDoNotCall () const {

	return CraterBetweenLoci::sDoNotCall;
}



RGString CraterBetweenLoci :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= CraterBetweenLoci::sMessageList.Entries ()))
		return returnValue;

	returnValue = *CraterBetweenLoci::sMessageArray [i];
	return returnValue;
}



int CraterBetweenLoci :: GetMessageLevel () const {

	return CraterBetweenLoci::sMessageLevel;
}



int CraterBetweenLoci :: GetNumberOfMessageParts () const {

	return CraterBetweenLoci::sMessageList.Entries ();
}


int CraterBetweenLoci :: GetSeverityLevel () const {

	return CraterBetweenLoci::sMessageLevel;
}


void CraterBetweenLoci :: SetIndexAndScope (int index, int scope) const {

	CraterBetweenLoci::sMessageIndex = index;
	CraterBetweenLoci::sMessageScope = scope;
}


int CraterBetweenLoci :: GetMessageIndex () const {

	return CraterBetweenLoci :: sMessageIndex;
}



int CraterBetweenLoci :: GetScope () const {

	return CraterBetweenLoci :: sMessageScope;
}



int CraterBetweenLoci :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	CraterBetweenLoci* noticeType = new CraterBetweenLoci;
	warehouse->AddType (noticeType);
	return 1;
}





PoorPeakMorphology :: PoorPeakMorphology () : Notice () {

}


PoorPeakMorphology :: PoorPeakMorphology (const PoorPeakMorphology& note) : Notice ((const Notice&) note) {

}



PoorPeakMorphology :: ~PoorPeakMorphology () {

}



int PoorPeakMorphology :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PoorPeakMorphology::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PoorPeakMorphology::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PoorPeakMorphology::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PoorPeakMorphology::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PoorPeakMorphology::sLabel = temp;

	else
		PoorPeakMorphology::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PoorPeakMorphology::sTriggerLowPriority = true;

		else
			PoorPeakMorphology::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PoorPeakMorphology::sDoNotCall = true;

		else
			PoorPeakMorphology::sDoNotCall = false;
	}


	int N = PoorPeakMorphology::sMessageList.Entries ();

	if (N == 1)
		N++;

	PoorPeakMorphology::sMessageArray = new RGString* [N];
	RGDListIterator it (PoorPeakMorphology::sMessageList);

	for (int i=0; i<N; i++)
		PoorPeakMorphology::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PoorPeakMorphology :: GetSubject () const {

	return PoorPeakMorphology::sSubject;
}


RGString PoorPeakMorphology :: GetLabel () const {

	return PoorPeakMorphology::sLabel;
}


bool PoorPeakMorphology :: IsTriggerForLowPriority () const {

	return PoorPeakMorphology::sTriggerLowPriority;
}


bool PoorPeakMorphology :: IsDoNotCall () const {

	return PoorPeakMorphology::sDoNotCall;
}



RGString PoorPeakMorphology :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PoorPeakMorphology::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PoorPeakMorphology::sMessageArray [i];
	return returnValue;
}



int PoorPeakMorphology :: GetMessageLevel () const {

	return PoorPeakMorphology::sMessageLevel;
}



int PoorPeakMorphology :: GetNumberOfMessageParts () const {

	return PoorPeakMorphology::sMessageList.Entries ();
}


int PoorPeakMorphology :: GetSeverityLevel () const {

	return PoorPeakMorphology::sMessageLevel;
}


void PoorPeakMorphology :: SetIndexAndScope (int index, int scope) const {

	PoorPeakMorphology::sMessageIndex = index;
	PoorPeakMorphology::sMessageScope = scope;
}


int PoorPeakMorphology :: GetMessageIndex () const {

	return PoorPeakMorphology :: sMessageIndex;
}



int PoorPeakMorphology :: GetScope () const {

	return PoorPeakMorphology :: sMessageScope;
}



int PoorPeakMorphology :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PoorPeakMorphology* noticeType = new PoorPeakMorphology;
	warehouse->AddType (noticeType);
	return 1;
}





PoorLocusMorphology :: PoorLocusMorphology () : Notice () {

}


PoorLocusMorphology :: PoorLocusMorphology (const PoorLocusMorphology& note) : Notice ((const Notice&) note) {

}



PoorLocusMorphology :: ~PoorLocusMorphology () {

}



int PoorLocusMorphology :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PoorLocusMorphology::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PoorLocusMorphology::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PoorLocusMorphology::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PoorLocusMorphology::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PoorLocusMorphology::sLabel = temp;

	else
		PoorLocusMorphology::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PoorLocusMorphology::sTriggerLowPriority = true;

		else
			PoorLocusMorphology::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PoorLocusMorphology::sDoNotCall = true;

		else
			PoorLocusMorphology::sDoNotCall = false;
	}


	int N = PoorLocusMorphology::sMessageList.Entries ();

	if (N == 1)
		N++;

	PoorLocusMorphology::sMessageArray = new RGString* [N];
	RGDListIterator it (PoorLocusMorphology::sMessageList);

	for (int i=0; i<N; i++)
		PoorLocusMorphology::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PoorLocusMorphology :: GetSubject () const {

	return PoorLocusMorphology::sSubject;
}


RGString PoorLocusMorphology :: GetLabel () const {

	return PoorLocusMorphology::sLabel;
}


bool PoorLocusMorphology :: IsTriggerForLowPriority () const {

	return PoorLocusMorphology::sTriggerLowPriority;
}


bool PoorLocusMorphology :: IsDoNotCall () const {

	return PoorLocusMorphology::sDoNotCall;
}



RGString PoorLocusMorphology :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PoorLocusMorphology::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PoorLocusMorphology::sMessageArray [i];
	return returnValue;
}



int PoorLocusMorphology :: GetMessageLevel () const {

	return PoorLocusMorphology::sMessageLevel;
}



int PoorLocusMorphology :: GetNumberOfMessageParts () const {

	return PoorLocusMorphology::sMessageList.Entries ();
}


int PoorLocusMorphology :: GetSeverityLevel () const {

	return PoorLocusMorphology::sMessageLevel;
}


void PoorLocusMorphology :: SetIndexAndScope (int index, int scope) const {

	PoorLocusMorphology::sMessageIndex = index;
	PoorLocusMorphology::sMessageScope = scope;
}


int PoorLocusMorphology :: GetMessageIndex () const {

	return PoorLocusMorphology :: sMessageIndex;
}



int PoorLocusMorphology :: GetScope () const {

	return PoorLocusMorphology :: sMessageScope;
}



int PoorLocusMorphology :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PoorLocusMorphology* noticeType = new PoorLocusMorphology;
	warehouse->AddType (noticeType);
	return 1;
}





PossibleUnreportedOLAlleles :: PossibleUnreportedOLAlleles () : Notice () {

}


PossibleUnreportedOLAlleles :: PossibleUnreportedOLAlleles (const PossibleUnreportedOLAlleles& note) : Notice ((const Notice&) note) {

}



PossibleUnreportedOLAlleles :: ~PossibleUnreportedOLAlleles () {

}



int PossibleUnreportedOLAlleles :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PossibleUnreportedOLAlleles::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PossibleUnreportedOLAlleles::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PossibleUnreportedOLAlleles::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PossibleUnreportedOLAlleles::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PossibleUnreportedOLAlleles::sLabel = temp;

	else
		PossibleUnreportedOLAlleles::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PossibleUnreportedOLAlleles::sTriggerLowPriority = true;

		else
			PossibleUnreportedOLAlleles::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PossibleUnreportedOLAlleles::sDoNotCall = true;

		else
			PossibleUnreportedOLAlleles::sDoNotCall = false;
	}


	int N = PossibleUnreportedOLAlleles::sMessageList.Entries ();

	if (N == 1)
		N++;

	PossibleUnreportedOLAlleles::sMessageArray = new RGString* [N];
	RGDListIterator it (PossibleUnreportedOLAlleles::sMessageList);

	for (int i=0; i<N; i++)
		PossibleUnreportedOLAlleles::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PossibleUnreportedOLAlleles :: GetSubject () const {

	return PossibleUnreportedOLAlleles::sSubject;
}


RGString PossibleUnreportedOLAlleles :: GetLabel () const {

	return PossibleUnreportedOLAlleles::sLabel;
}


bool PossibleUnreportedOLAlleles :: IsTriggerForLowPriority () const {

	return PossibleUnreportedOLAlleles::sTriggerLowPriority;
}


bool PossibleUnreportedOLAlleles :: IsDoNotCall () const {

	return PossibleUnreportedOLAlleles::sDoNotCall;
}



RGString PossibleUnreportedOLAlleles :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PossibleUnreportedOLAlleles::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PossibleUnreportedOLAlleles::sMessageArray [i];
	return returnValue;
}



int PossibleUnreportedOLAlleles :: GetMessageLevel () const {

	return PossibleUnreportedOLAlleles::sMessageLevel;
}



int PossibleUnreportedOLAlleles :: GetNumberOfMessageParts () const {

	return PossibleUnreportedOLAlleles::sMessageList.Entries ();
}


int PossibleUnreportedOLAlleles :: GetSeverityLevel () const {

	return PossibleUnreportedOLAlleles::sMessageLevel;
}


void PossibleUnreportedOLAlleles :: SetIndexAndScope (int index, int scope) const {

	PossibleUnreportedOLAlleles::sMessageIndex = index;
	PossibleUnreportedOLAlleles::sMessageScope = scope;
}


int PossibleUnreportedOLAlleles :: GetMessageIndex () const {

	return PossibleUnreportedOLAlleles :: sMessageIndex;
}



int PossibleUnreportedOLAlleles :: GetScope () const {

	return PossibleUnreportedOLAlleles :: sMessageScope;
}



int PossibleUnreportedOLAlleles :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PossibleUnreportedOLAlleles* noticeType = new PossibleUnreportedOLAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





XPeakMissingFromAmelogenin :: XPeakMissingFromAmelogenin () : Notice () {

}


XPeakMissingFromAmelogenin :: XPeakMissingFromAmelogenin (const XPeakMissingFromAmelogenin& note) : Notice ((const Notice&) note) {

}



XPeakMissingFromAmelogenin :: ~XPeakMissingFromAmelogenin () {

}



int XPeakMissingFromAmelogenin :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (XPeakMissingFromAmelogenin::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		XPeakMissingFromAmelogenin::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	XPeakMissingFromAmelogenin::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	XPeakMissingFromAmelogenin::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		XPeakMissingFromAmelogenin::sLabel = temp;

	else
		XPeakMissingFromAmelogenin::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			XPeakMissingFromAmelogenin::sTriggerLowPriority = true;

		else
			XPeakMissingFromAmelogenin::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			XPeakMissingFromAmelogenin::sDoNotCall = true;

		else
			XPeakMissingFromAmelogenin::sDoNotCall = false;
	}


	int N = XPeakMissingFromAmelogenin::sMessageList.Entries ();

	if (N == 1)
		N++;

	XPeakMissingFromAmelogenin::sMessageArray = new RGString* [N];
	RGDListIterator it (XPeakMissingFromAmelogenin::sMessageList);

	for (int i=0; i<N; i++)
		XPeakMissingFromAmelogenin::sMessageArray [i] = (RGString*) it ();

	return status;
}


int XPeakMissingFromAmelogenin :: GetSubject () const {

	return XPeakMissingFromAmelogenin::sSubject;
}


RGString XPeakMissingFromAmelogenin :: GetLabel () const {

	return XPeakMissingFromAmelogenin::sLabel;
}


bool XPeakMissingFromAmelogenin :: IsTriggerForLowPriority () const {

	return XPeakMissingFromAmelogenin::sTriggerLowPriority;
}


bool XPeakMissingFromAmelogenin :: IsDoNotCall () const {

	return XPeakMissingFromAmelogenin::sDoNotCall;
}



RGString XPeakMissingFromAmelogenin :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= XPeakMissingFromAmelogenin::sMessageList.Entries ()))
		return returnValue;

	returnValue = *XPeakMissingFromAmelogenin::sMessageArray [i];
	return returnValue;
}



int XPeakMissingFromAmelogenin :: GetMessageLevel () const {

	return XPeakMissingFromAmelogenin::sMessageLevel;
}



int XPeakMissingFromAmelogenin :: GetNumberOfMessageParts () const {

	return XPeakMissingFromAmelogenin::sMessageList.Entries ();
}


int XPeakMissingFromAmelogenin :: GetSeverityLevel () const {

	return XPeakMissingFromAmelogenin::sMessageLevel;
}


void XPeakMissingFromAmelogenin :: SetIndexAndScope (int index, int scope) const {

	XPeakMissingFromAmelogenin::sMessageIndex = index;
	XPeakMissingFromAmelogenin::sMessageScope = scope;
}


int XPeakMissingFromAmelogenin :: GetMessageIndex () const {

	return XPeakMissingFromAmelogenin :: sMessageIndex;
}



int XPeakMissingFromAmelogenin :: GetScope () const {

	return XPeakMissingFromAmelogenin :: sMessageScope;
}



int XPeakMissingFromAmelogenin :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	XPeakMissingFromAmelogenin* noticeType = new XPeakMissingFromAmelogenin;
	warehouse->AddType (noticeType);
	return 1;
}





ExtraneousPositiveControlPeak :: ExtraneousPositiveControlPeak () : Notice () {

}


ExtraneousPositiveControlPeak :: ExtraneousPositiveControlPeak (const ExtraneousPositiveControlPeak& note) : Notice ((const Notice&) note) {

}



ExtraneousPositiveControlPeak :: ~ExtraneousPositiveControlPeak () {

}



int ExtraneousPositiveControlPeak :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ExtraneousPositiveControlPeak::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ExtraneousPositiveControlPeak::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ExtraneousPositiveControlPeak::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ExtraneousPositiveControlPeak::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ExtraneousPositiveControlPeak::sLabel = temp;

	else
		ExtraneousPositiveControlPeak::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ExtraneousPositiveControlPeak::sTriggerLowPriority = true;

		else
			ExtraneousPositiveControlPeak::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ExtraneousPositiveControlPeak::sDoNotCall = true;

		else
			ExtraneousPositiveControlPeak::sDoNotCall = false;
	}


	int N = ExtraneousPositiveControlPeak::sMessageList.Entries ();

	if (N == 1)
		N++;

	ExtraneousPositiveControlPeak::sMessageArray = new RGString* [N];
	RGDListIterator it (ExtraneousPositiveControlPeak::sMessageList);

	for (int i=0; i<N; i++)
		ExtraneousPositiveControlPeak::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ExtraneousPositiveControlPeak :: GetSubject () const {

	return ExtraneousPositiveControlPeak::sSubject;
}


RGString ExtraneousPositiveControlPeak :: GetLabel () const {

	return ExtraneousPositiveControlPeak::sLabel;
}


bool ExtraneousPositiveControlPeak :: IsTriggerForLowPriority () const {

	return ExtraneousPositiveControlPeak::sTriggerLowPriority;
}


bool ExtraneousPositiveControlPeak :: IsDoNotCall () const {

	return ExtraneousPositiveControlPeak::sDoNotCall;
}



RGString ExtraneousPositiveControlPeak :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ExtraneousPositiveControlPeak::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ExtraneousPositiveControlPeak::sMessageArray [i];
	return returnValue;
}



int ExtraneousPositiveControlPeak :: GetMessageLevel () const {

	return ExtraneousPositiveControlPeak::sMessageLevel;
}



int ExtraneousPositiveControlPeak :: GetNumberOfMessageParts () const {

	return ExtraneousPositiveControlPeak::sMessageList.Entries ();
}


int ExtraneousPositiveControlPeak :: GetSeverityLevel () const {

	return ExtraneousPositiveControlPeak::sMessageLevel;
}


void ExtraneousPositiveControlPeak :: SetIndexAndScope (int index, int scope) const {

	ExtraneousPositiveControlPeak::sMessageIndex = index;
	ExtraneousPositiveControlPeak::sMessageScope = scope;
}


int ExtraneousPositiveControlPeak :: GetMessageIndex () const {

	return ExtraneousPositiveControlPeak :: sMessageIndex;
}



int ExtraneousPositiveControlPeak :: GetScope () const {

	return ExtraneousPositiveControlPeak :: sMessageScope;
}



int ExtraneousPositiveControlPeak :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ExtraneousPositiveControlPeak* noticeType = new ExtraneousPositiveControlPeak;
	warehouse->AddType (noticeType);
	return 1;
}





NoPositiveControlInDirectory :: NoPositiveControlInDirectory () : Notice () {

}


NoPositiveControlInDirectory :: NoPositiveControlInDirectory (const NoPositiveControlInDirectory& note) : Notice ((const Notice&) note) {

}



NoPositiveControlInDirectory :: ~NoPositiveControlInDirectory () {

}



int NoPositiveControlInDirectory :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoPositiveControlInDirectory::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoPositiveControlInDirectory::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoPositiveControlInDirectory::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoPositiveControlInDirectory::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoPositiveControlInDirectory::sLabel = temp;

	else
		NoPositiveControlInDirectory::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoPositiveControlInDirectory::sTriggerLowPriority = true;

		else
			NoPositiveControlInDirectory::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoPositiveControlInDirectory::sDoNotCall = true;

		else
			NoPositiveControlInDirectory::sDoNotCall = false;
	}


	int N = NoPositiveControlInDirectory::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoPositiveControlInDirectory::sMessageArray = new RGString* [N];
	RGDListIterator it (NoPositiveControlInDirectory::sMessageList);

	for (int i=0; i<N; i++)
		NoPositiveControlInDirectory::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoPositiveControlInDirectory :: GetSubject () const {

	return NoPositiveControlInDirectory::sSubject;
}


RGString NoPositiveControlInDirectory :: GetLabel () const {

	return NoPositiveControlInDirectory::sLabel;
}


bool NoPositiveControlInDirectory :: IsTriggerForLowPriority () const {

	return NoPositiveControlInDirectory::sTriggerLowPriority;
}


bool NoPositiveControlInDirectory :: IsDoNotCall () const {

	return NoPositiveControlInDirectory::sDoNotCall;
}



RGString NoPositiveControlInDirectory :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoPositiveControlInDirectory::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoPositiveControlInDirectory::sMessageArray [i];
	return returnValue;
}



int NoPositiveControlInDirectory :: GetMessageLevel () const {

	return NoPositiveControlInDirectory::sMessageLevel;
}



int NoPositiveControlInDirectory :: GetNumberOfMessageParts () const {

	return NoPositiveControlInDirectory::sMessageList.Entries ();
}


int NoPositiveControlInDirectory :: GetSeverityLevel () const {

	return NoPositiveControlInDirectory::sMessageLevel;
}


void NoPositiveControlInDirectory :: SetIndexAndScope (int index, int scope) const {

	NoPositiveControlInDirectory::sMessageIndex = index;
	NoPositiveControlInDirectory::sMessageScope = scope;
}


int NoPositiveControlInDirectory :: GetMessageIndex () const {

	return NoPositiveControlInDirectory :: sMessageIndex;
}



int NoPositiveControlInDirectory :: GetScope () const {

	return NoPositiveControlInDirectory :: sMessageScope;
}



int NoPositiveControlInDirectory :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoPositiveControlInDirectory* noticeType = new NoPositiveControlInDirectory;
	warehouse->AddType (noticeType);
	return 1;
}





NoNegativeControlInDirectory :: NoNegativeControlInDirectory () : Notice () {

}


NoNegativeControlInDirectory :: NoNegativeControlInDirectory (const NoNegativeControlInDirectory& note) : Notice ((const Notice&) note) {

}



NoNegativeControlInDirectory :: ~NoNegativeControlInDirectory () {

}



int NoNegativeControlInDirectory :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoNegativeControlInDirectory::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoNegativeControlInDirectory::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoNegativeControlInDirectory::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoNegativeControlInDirectory::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoNegativeControlInDirectory::sLabel = temp;

	else
		NoNegativeControlInDirectory::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoNegativeControlInDirectory::sTriggerLowPriority = true;

		else
			NoNegativeControlInDirectory::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoNegativeControlInDirectory::sDoNotCall = true;

		else
			NoNegativeControlInDirectory::sDoNotCall = false;
	}


	int N = NoNegativeControlInDirectory::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoNegativeControlInDirectory::sMessageArray = new RGString* [N];
	RGDListIterator it (NoNegativeControlInDirectory::sMessageList);

	for (int i=0; i<N; i++)
		NoNegativeControlInDirectory::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoNegativeControlInDirectory :: GetSubject () const {

	return NoNegativeControlInDirectory::sSubject;
}


RGString NoNegativeControlInDirectory :: GetLabel () const {

	return NoNegativeControlInDirectory::sLabel;
}


bool NoNegativeControlInDirectory :: IsTriggerForLowPriority () const {

	return NoNegativeControlInDirectory::sTriggerLowPriority;
}


bool NoNegativeControlInDirectory :: IsDoNotCall () const {

	return NoNegativeControlInDirectory::sDoNotCall;
}



RGString NoNegativeControlInDirectory :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoNegativeControlInDirectory::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoNegativeControlInDirectory::sMessageArray [i];
	return returnValue;
}



int NoNegativeControlInDirectory :: GetMessageLevel () const {

	return NoNegativeControlInDirectory::sMessageLevel;
}



int NoNegativeControlInDirectory :: GetNumberOfMessageParts () const {

	return NoNegativeControlInDirectory::sMessageList.Entries ();
}


int NoNegativeControlInDirectory :: GetSeverityLevel () const {

	return NoNegativeControlInDirectory::sMessageLevel;
}


void NoNegativeControlInDirectory :: SetIndexAndScope (int index, int scope) const {

	NoNegativeControlInDirectory::sMessageIndex = index;
	NoNegativeControlInDirectory::sMessageScope = scope;
}


int NoNegativeControlInDirectory :: GetMessageIndex () const {

	return NoNegativeControlInDirectory :: sMessageIndex;
}



int NoNegativeControlInDirectory :: GetScope () const {

	return NoNegativeControlInDirectory :: sMessageScope;
}



int NoNegativeControlInDirectory :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoNegativeControlInDirectory* noticeType = new NoNegativeControlInDirectory;
	warehouse->AddType (noticeType);
	return 1;
}





NamedPositiveControlNotFound :: NamedPositiveControlNotFound () : Notice () {

}


NamedPositiveControlNotFound :: NamedPositiveControlNotFound (const NamedPositiveControlNotFound& note) : Notice ((const Notice&) note) {

}



NamedPositiveControlNotFound :: ~NamedPositiveControlNotFound () {

}



int NamedPositiveControlNotFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NamedPositiveControlNotFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NamedPositiveControlNotFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NamedPositiveControlNotFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NamedPositiveControlNotFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NamedPositiveControlNotFound::sLabel = temp;

	else
		NamedPositiveControlNotFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NamedPositiveControlNotFound::sTriggerLowPriority = true;

		else
			NamedPositiveControlNotFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NamedPositiveControlNotFound::sDoNotCall = true;

		else
			NamedPositiveControlNotFound::sDoNotCall = false;
	}


	int N = NamedPositiveControlNotFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	NamedPositiveControlNotFound::sMessageArray = new RGString* [N];
	RGDListIterator it (NamedPositiveControlNotFound::sMessageList);

	for (int i=0; i<N; i++)
		NamedPositiveControlNotFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NamedPositiveControlNotFound :: GetSubject () const {

	return NamedPositiveControlNotFound::sSubject;
}


RGString NamedPositiveControlNotFound :: GetLabel () const {

	return NamedPositiveControlNotFound::sLabel;
}


bool NamedPositiveControlNotFound :: IsTriggerForLowPriority () const {

	return NamedPositiveControlNotFound::sTriggerLowPriority;
}


bool NamedPositiveControlNotFound :: IsDoNotCall () const {

	return NamedPositiveControlNotFound::sDoNotCall;
}



RGString NamedPositiveControlNotFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NamedPositiveControlNotFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NamedPositiveControlNotFound::sMessageArray [i];
	return returnValue;
}



int NamedPositiveControlNotFound :: GetMessageLevel () const {

	return NamedPositiveControlNotFound::sMessageLevel;
}



int NamedPositiveControlNotFound :: GetNumberOfMessageParts () const {

	return NamedPositiveControlNotFound::sMessageList.Entries ();
}


int NamedPositiveControlNotFound :: GetSeverityLevel () const {

	return NamedPositiveControlNotFound::sMessageLevel;
}


void NamedPositiveControlNotFound :: SetIndexAndScope (int index, int scope) const {

	NamedPositiveControlNotFound::sMessageIndex = index;
	NamedPositiveControlNotFound::sMessageScope = scope;
}


int NamedPositiveControlNotFound :: GetMessageIndex () const {

	return NamedPositiveControlNotFound :: sMessageIndex;
}



int NamedPositiveControlNotFound :: GetScope () const {

	return NamedPositiveControlNotFound :: sMessageScope;
}



int NamedPositiveControlNotFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NamedPositiveControlNotFound* noticeType = new NamedPositiveControlNotFound;
	warehouse->AddType (noticeType);
	return 1;
}





PositiveCOntrolLocusNotFound :: PositiveCOntrolLocusNotFound () : Notice () {

}


PositiveCOntrolLocusNotFound :: PositiveCOntrolLocusNotFound (const PositiveCOntrolLocusNotFound& note) : Notice ((const Notice&) note) {

}



PositiveCOntrolLocusNotFound :: ~PositiveCOntrolLocusNotFound () {

}



int PositiveCOntrolLocusNotFound :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PositiveCOntrolLocusNotFound::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PositiveCOntrolLocusNotFound::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PositiveCOntrolLocusNotFound::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PositiveCOntrolLocusNotFound::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PositiveCOntrolLocusNotFound::sLabel = temp;

	else
		PositiveCOntrolLocusNotFound::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PositiveCOntrolLocusNotFound::sTriggerLowPriority = true;

		else
			PositiveCOntrolLocusNotFound::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PositiveCOntrolLocusNotFound::sDoNotCall = true;

		else
			PositiveCOntrolLocusNotFound::sDoNotCall = false;
	}


	int N = PositiveCOntrolLocusNotFound::sMessageList.Entries ();

	if (N == 1)
		N++;

	PositiveCOntrolLocusNotFound::sMessageArray = new RGString* [N];
	RGDListIterator it (PositiveCOntrolLocusNotFound::sMessageList);

	for (int i=0; i<N; i++)
		PositiveCOntrolLocusNotFound::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PositiveCOntrolLocusNotFound :: GetSubject () const {

	return PositiveCOntrolLocusNotFound::sSubject;
}


RGString PositiveCOntrolLocusNotFound :: GetLabel () const {

	return PositiveCOntrolLocusNotFound::sLabel;
}


bool PositiveCOntrolLocusNotFound :: IsTriggerForLowPriority () const {

	return PositiveCOntrolLocusNotFound::sTriggerLowPriority;
}


bool PositiveCOntrolLocusNotFound :: IsDoNotCall () const {

	return PositiveCOntrolLocusNotFound::sDoNotCall;
}



RGString PositiveCOntrolLocusNotFound :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PositiveCOntrolLocusNotFound::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PositiveCOntrolLocusNotFound::sMessageArray [i];
	return returnValue;
}



int PositiveCOntrolLocusNotFound :: GetMessageLevel () const {

	return PositiveCOntrolLocusNotFound::sMessageLevel;
}



int PositiveCOntrolLocusNotFound :: GetNumberOfMessageParts () const {

	return PositiveCOntrolLocusNotFound::sMessageList.Entries ();
}


int PositiveCOntrolLocusNotFound :: GetSeverityLevel () const {

	return PositiveCOntrolLocusNotFound::sMessageLevel;
}


void PositiveCOntrolLocusNotFound :: SetIndexAndScope (int index, int scope) const {

	PositiveCOntrolLocusNotFound::sMessageIndex = index;
	PositiveCOntrolLocusNotFound::sMessageScope = scope;
}


int PositiveCOntrolLocusNotFound :: GetMessageIndex () const {

	return PositiveCOntrolLocusNotFound :: sMessageIndex;
}



int PositiveCOntrolLocusNotFound :: GetScope () const {

	return PositiveCOntrolLocusNotFound :: sMessageScope;
}



int PositiveCOntrolLocusNotFound :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PositiveCOntrolLocusNotFound* noticeType = new PositiveCOntrolLocusNotFound;
	warehouse->AddType (noticeType);
	return 1;
}





SignalPeakAboveMaxRFU :: SignalPeakAboveMaxRFU () : Notice () {

}


SignalPeakAboveMaxRFU :: SignalPeakAboveMaxRFU (const SignalPeakAboveMaxRFU& note) : Notice ((const Notice&) note) {

}



SignalPeakAboveMaxRFU :: ~SignalPeakAboveMaxRFU () {

}



int SignalPeakAboveMaxRFU :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (SignalPeakAboveMaxRFU::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		SignalPeakAboveMaxRFU::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalPeakAboveMaxRFU::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalPeakAboveMaxRFU::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		SignalPeakAboveMaxRFU::sLabel = temp;

	else
		SignalPeakAboveMaxRFU::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalPeakAboveMaxRFU::sTriggerLowPriority = true;

		else
			SignalPeakAboveMaxRFU::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalPeakAboveMaxRFU::sDoNotCall = true;

		else
			SignalPeakAboveMaxRFU::sDoNotCall = false;
	}


	int N = SignalPeakAboveMaxRFU::sMessageList.Entries ();

	if (N == 1)
		N++;

	SignalPeakAboveMaxRFU::sMessageArray = new RGString* [N];
	RGDListIterator it (SignalPeakAboveMaxRFU::sMessageList);

	for (int i=0; i<N; i++)
		SignalPeakAboveMaxRFU::sMessageArray [i] = (RGString*) it ();

	return status;
}


int SignalPeakAboveMaxRFU :: GetSubject () const {

	return SignalPeakAboveMaxRFU::sSubject;
}


RGString SignalPeakAboveMaxRFU :: GetLabel () const {

	return SignalPeakAboveMaxRFU::sLabel;
}


bool SignalPeakAboveMaxRFU :: IsTriggerForLowPriority () const {

	return SignalPeakAboveMaxRFU::sTriggerLowPriority;
}


bool SignalPeakAboveMaxRFU :: IsDoNotCall () const {

	return SignalPeakAboveMaxRFU::sDoNotCall;
}



RGString SignalPeakAboveMaxRFU :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= SignalPeakAboveMaxRFU::sMessageList.Entries ()))
		return returnValue;

	returnValue = *SignalPeakAboveMaxRFU::sMessageArray [i];
	return returnValue;
}



int SignalPeakAboveMaxRFU :: GetMessageLevel () const {

	return SignalPeakAboveMaxRFU::sMessageLevel;
}



int SignalPeakAboveMaxRFU :: GetNumberOfMessageParts () const {

	return SignalPeakAboveMaxRFU::sMessageList.Entries ();
}


int SignalPeakAboveMaxRFU :: GetSeverityLevel () const {

	return SignalPeakAboveMaxRFU::sMessageLevel;
}


void SignalPeakAboveMaxRFU :: SetIndexAndScope (int index, int scope) const {

	SignalPeakAboveMaxRFU::sMessageIndex = index;
	SignalPeakAboveMaxRFU::sMessageScope = scope;
}


int SignalPeakAboveMaxRFU :: GetMessageIndex () const {

	return SignalPeakAboveMaxRFU :: sMessageIndex;
}



int SignalPeakAboveMaxRFU :: GetScope () const {

	return SignalPeakAboveMaxRFU :: sMessageScope;
}



int SignalPeakAboveMaxRFU :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	SignalPeakAboveMaxRFU* noticeType = new SignalPeakAboveMaxRFU;
	warehouse->AddType (noticeType);
	return 1;
}





OneOrMorePeaksAboveMaxRFU :: OneOrMorePeaksAboveMaxRFU () : Notice () {

}


OneOrMorePeaksAboveMaxRFU :: OneOrMorePeaksAboveMaxRFU (const OneOrMorePeaksAboveMaxRFU& note) : Notice ((const Notice&) note) {

}



OneOrMorePeaksAboveMaxRFU :: ~OneOrMorePeaksAboveMaxRFU () {

}



int OneOrMorePeaksAboveMaxRFU :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (OneOrMorePeaksAboveMaxRFU::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		OneOrMorePeaksAboveMaxRFU::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OneOrMorePeaksAboveMaxRFU::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OneOrMorePeaksAboveMaxRFU::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		OneOrMorePeaksAboveMaxRFU::sLabel = temp;

	else
		OneOrMorePeaksAboveMaxRFU::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OneOrMorePeaksAboveMaxRFU::sTriggerLowPriority = true;

		else
			OneOrMorePeaksAboveMaxRFU::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OneOrMorePeaksAboveMaxRFU::sDoNotCall = true;

		else
			OneOrMorePeaksAboveMaxRFU::sDoNotCall = false;
	}


	int N = OneOrMorePeaksAboveMaxRFU::sMessageList.Entries ();

	if (N == 1)
		N++;

	OneOrMorePeaksAboveMaxRFU::sMessageArray = new RGString* [N];
	RGDListIterator it (OneOrMorePeaksAboveMaxRFU::sMessageList);

	for (int i=0; i<N; i++)
		OneOrMorePeaksAboveMaxRFU::sMessageArray [i] = (RGString*) it ();

	return status;
}


int OneOrMorePeaksAboveMaxRFU :: GetSubject () const {

	return OneOrMorePeaksAboveMaxRFU::sSubject;
}


RGString OneOrMorePeaksAboveMaxRFU :: GetLabel () const {

	return OneOrMorePeaksAboveMaxRFU::sLabel;
}


bool OneOrMorePeaksAboveMaxRFU :: IsTriggerForLowPriority () const {

	return OneOrMorePeaksAboveMaxRFU::sTriggerLowPriority;
}


bool OneOrMorePeaksAboveMaxRFU :: IsDoNotCall () const {

	return OneOrMorePeaksAboveMaxRFU::sDoNotCall;
}



RGString OneOrMorePeaksAboveMaxRFU :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= OneOrMorePeaksAboveMaxRFU::sMessageList.Entries ()))
		return returnValue;

	returnValue = *OneOrMorePeaksAboveMaxRFU::sMessageArray [i];
	return returnValue;
}



int OneOrMorePeaksAboveMaxRFU :: GetMessageLevel () const {

	return OneOrMorePeaksAboveMaxRFU::sMessageLevel;
}



int OneOrMorePeaksAboveMaxRFU :: GetNumberOfMessageParts () const {

	return OneOrMorePeaksAboveMaxRFU::sMessageList.Entries ();
}


int OneOrMorePeaksAboveMaxRFU :: GetSeverityLevel () const {

	return OneOrMorePeaksAboveMaxRFU::sMessageLevel;
}


void OneOrMorePeaksAboveMaxRFU :: SetIndexAndScope (int index, int scope) const {

	OneOrMorePeaksAboveMaxRFU::sMessageIndex = index;
	OneOrMorePeaksAboveMaxRFU::sMessageScope = scope;
}


int OneOrMorePeaksAboveMaxRFU :: GetMessageIndex () const {

	return OneOrMorePeaksAboveMaxRFU :: sMessageIndex;
}



int OneOrMorePeaksAboveMaxRFU :: GetScope () const {

	return OneOrMorePeaksAboveMaxRFU :: sMessageScope;
}



int OneOrMorePeaksAboveMaxRFU :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	OneOrMorePeaksAboveMaxRFU* noticeType = new OneOrMorePeaksAboveMaxRFU;
	warehouse->AddType (noticeType);
	return 1;
}





ResidualExceedsThreshold :: ResidualExceedsThreshold () : Notice () {

}


ResidualExceedsThreshold :: ResidualExceedsThreshold (const ResidualExceedsThreshold& note) : Notice ((const Notice&) note) {

}



ResidualExceedsThreshold :: ~ResidualExceedsThreshold () {

}



int ResidualExceedsThreshold :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ResidualExceedsThreshold::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ResidualExceedsThreshold::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ResidualExceedsThreshold::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ResidualExceedsThreshold::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ResidualExceedsThreshold::sLabel = temp;

	else
		ResidualExceedsThreshold::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ResidualExceedsThreshold::sTriggerLowPriority = true;

		else
			ResidualExceedsThreshold::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ResidualExceedsThreshold::sDoNotCall = true;

		else
			ResidualExceedsThreshold::sDoNotCall = false;
	}


	int N = ResidualExceedsThreshold::sMessageList.Entries ();

	if (N == 1)
		N++;

	ResidualExceedsThreshold::sMessageArray = new RGString* [N];
	RGDListIterator it (ResidualExceedsThreshold::sMessageList);

	for (int i=0; i<N; i++)
		ResidualExceedsThreshold::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ResidualExceedsThreshold :: GetSubject () const {

	return ResidualExceedsThreshold::sSubject;
}


RGString ResidualExceedsThreshold :: GetLabel () const {

	return ResidualExceedsThreshold::sLabel;
}


bool ResidualExceedsThreshold :: IsTriggerForLowPriority () const {

	return ResidualExceedsThreshold::sTriggerLowPriority;
}


bool ResidualExceedsThreshold :: IsDoNotCall () const {

	return ResidualExceedsThreshold::sDoNotCall;
}



RGString ResidualExceedsThreshold :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ResidualExceedsThreshold::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ResidualExceedsThreshold::sMessageArray [i];
	return returnValue;
}



int ResidualExceedsThreshold :: GetMessageLevel () const {

	return ResidualExceedsThreshold::sMessageLevel;
}



int ResidualExceedsThreshold :: GetNumberOfMessageParts () const {

	return ResidualExceedsThreshold::sMessageList.Entries ();
}


int ResidualExceedsThreshold :: GetSeverityLevel () const {

	return ResidualExceedsThreshold::sMessageLevel;
}


void ResidualExceedsThreshold :: SetIndexAndScope (int index, int scope) const {

	ResidualExceedsThreshold::sMessageIndex = index;
	ResidualExceedsThreshold::sMessageScope = scope;
}


int ResidualExceedsThreshold :: GetMessageIndex () const {

	return ResidualExceedsThreshold :: sMessageIndex;
}



int ResidualExceedsThreshold :: GetScope () const {

	return ResidualExceedsThreshold :: sMessageScope;
}



int ResidualExceedsThreshold :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ResidualExceedsThreshold* noticeType = new ResidualExceedsThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





LocusContainsPeaksWithExcessiveResiduals :: LocusContainsPeaksWithExcessiveResiduals () : Notice () {

}


LocusContainsPeaksWithExcessiveResiduals :: LocusContainsPeaksWithExcessiveResiduals (const LocusContainsPeaksWithExcessiveResiduals& note) : Notice ((const Notice&) note) {

}



LocusContainsPeaksWithExcessiveResiduals :: ~LocusContainsPeaksWithExcessiveResiduals () {

}



int LocusContainsPeaksWithExcessiveResiduals :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LocusContainsPeaksWithExcessiveResiduals::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LocusContainsPeaksWithExcessiveResiduals::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LocusContainsPeaksWithExcessiveResiduals::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LocusContainsPeaksWithExcessiveResiduals::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LocusContainsPeaksWithExcessiveResiduals::sLabel = temp;

	else
		LocusContainsPeaksWithExcessiveResiduals::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LocusContainsPeaksWithExcessiveResiduals::sTriggerLowPriority = true;

		else
			LocusContainsPeaksWithExcessiveResiduals::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LocusContainsPeaksWithExcessiveResiduals::sDoNotCall = true;

		else
			LocusContainsPeaksWithExcessiveResiduals::sDoNotCall = false;
	}


	int N = LocusContainsPeaksWithExcessiveResiduals::sMessageList.Entries ();

	if (N == 1)
		N++;

	LocusContainsPeaksWithExcessiveResiduals::sMessageArray = new RGString* [N];
	RGDListIterator it (LocusContainsPeaksWithExcessiveResiduals::sMessageList);

	for (int i=0; i<N; i++)
		LocusContainsPeaksWithExcessiveResiduals::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LocusContainsPeaksWithExcessiveResiduals :: GetSubject () const {

	return LocusContainsPeaksWithExcessiveResiduals::sSubject;
}


RGString LocusContainsPeaksWithExcessiveResiduals :: GetLabel () const {

	return LocusContainsPeaksWithExcessiveResiduals::sLabel;
}


bool LocusContainsPeaksWithExcessiveResiduals :: IsTriggerForLowPriority () const {

	return LocusContainsPeaksWithExcessiveResiduals::sTriggerLowPriority;
}


bool LocusContainsPeaksWithExcessiveResiduals :: IsDoNotCall () const {

	return LocusContainsPeaksWithExcessiveResiduals::sDoNotCall;
}



RGString LocusContainsPeaksWithExcessiveResiduals :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LocusContainsPeaksWithExcessiveResiduals::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LocusContainsPeaksWithExcessiveResiduals::sMessageArray [i];
	return returnValue;
}



int LocusContainsPeaksWithExcessiveResiduals :: GetMessageLevel () const {

	return LocusContainsPeaksWithExcessiveResiduals::sMessageLevel;
}



int LocusContainsPeaksWithExcessiveResiduals :: GetNumberOfMessageParts () const {

	return LocusContainsPeaksWithExcessiveResiduals::sMessageList.Entries ();
}


int LocusContainsPeaksWithExcessiveResiduals :: GetSeverityLevel () const {

	return LocusContainsPeaksWithExcessiveResiduals::sMessageLevel;
}


void LocusContainsPeaksWithExcessiveResiduals :: SetIndexAndScope (int index, int scope) const {

	LocusContainsPeaksWithExcessiveResiduals::sMessageIndex = index;
	LocusContainsPeaksWithExcessiveResiduals::sMessageScope = scope;
}


int LocusContainsPeaksWithExcessiveResiduals :: GetMessageIndex () const {

	return LocusContainsPeaksWithExcessiveResiduals :: sMessageIndex;
}



int LocusContainsPeaksWithExcessiveResiduals :: GetScope () const {

	return LocusContainsPeaksWithExcessiveResiduals :: sMessageScope;
}



int LocusContainsPeaksWithExcessiveResiduals :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LocusContainsPeaksWithExcessiveResiduals* noticeType = new LocusContainsPeaksWithExcessiveResiduals;
	warehouse->AddType (noticeType);
	return 1;
}





SetupErrorNumberPeaksUnavailableForLocus :: SetupErrorNumberPeaksUnavailableForLocus () : Notice () {

}


SetupErrorNumberPeaksUnavailableForLocus :: SetupErrorNumberPeaksUnavailableForLocus (const SetupErrorNumberPeaksUnavailableForLocus& note) : Notice ((const Notice&) note) {

}



SetupErrorNumberPeaksUnavailableForLocus :: ~SetupErrorNumberPeaksUnavailableForLocus () {

}



int SetupErrorNumberPeaksUnavailableForLocus :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (SetupErrorNumberPeaksUnavailableForLocus::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		SetupErrorNumberPeaksUnavailableForLocus::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SetupErrorNumberPeaksUnavailableForLocus::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SetupErrorNumberPeaksUnavailableForLocus::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		SetupErrorNumberPeaksUnavailableForLocus::sLabel = temp;

	else
		SetupErrorNumberPeaksUnavailableForLocus::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SetupErrorNumberPeaksUnavailableForLocus::sTriggerLowPriority = true;

		else
			SetupErrorNumberPeaksUnavailableForLocus::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SetupErrorNumberPeaksUnavailableForLocus::sDoNotCall = true;

		else
			SetupErrorNumberPeaksUnavailableForLocus::sDoNotCall = false;
	}


	int N = SetupErrorNumberPeaksUnavailableForLocus::sMessageList.Entries ();

	if (N == 1)
		N++;

	SetupErrorNumberPeaksUnavailableForLocus::sMessageArray = new RGString* [N];
	RGDListIterator it (SetupErrorNumberPeaksUnavailableForLocus::sMessageList);

	for (int i=0; i<N; i++)
		SetupErrorNumberPeaksUnavailableForLocus::sMessageArray [i] = (RGString*) it ();

	return status;
}


int SetupErrorNumberPeaksUnavailableForLocus :: GetSubject () const {

	return SetupErrorNumberPeaksUnavailableForLocus::sSubject;
}


RGString SetupErrorNumberPeaksUnavailableForLocus :: GetLabel () const {

	return SetupErrorNumberPeaksUnavailableForLocus::sLabel;
}


bool SetupErrorNumberPeaksUnavailableForLocus :: IsTriggerForLowPriority () const {

	return SetupErrorNumberPeaksUnavailableForLocus::sTriggerLowPriority;
}


bool SetupErrorNumberPeaksUnavailableForLocus :: IsDoNotCall () const {

	return SetupErrorNumberPeaksUnavailableForLocus::sDoNotCall;
}



RGString SetupErrorNumberPeaksUnavailableForLocus :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= SetupErrorNumberPeaksUnavailableForLocus::sMessageList.Entries ()))
		return returnValue;

	returnValue = *SetupErrorNumberPeaksUnavailableForLocus::sMessageArray [i];
	return returnValue;
}



int SetupErrorNumberPeaksUnavailableForLocus :: GetMessageLevel () const {

	return SetupErrorNumberPeaksUnavailableForLocus::sMessageLevel;
}



int SetupErrorNumberPeaksUnavailableForLocus :: GetNumberOfMessageParts () const {

	return SetupErrorNumberPeaksUnavailableForLocus::sMessageList.Entries ();
}


int SetupErrorNumberPeaksUnavailableForLocus :: GetSeverityLevel () const {

	return SetupErrorNumberPeaksUnavailableForLocus::sMessageLevel;
}


void SetupErrorNumberPeaksUnavailableForLocus :: SetIndexAndScope (int index, int scope) const {

	SetupErrorNumberPeaksUnavailableForLocus::sMessageIndex = index;
	SetupErrorNumberPeaksUnavailableForLocus::sMessageScope = scope;
}


int SetupErrorNumberPeaksUnavailableForLocus :: GetMessageIndex () const {

	return SetupErrorNumberPeaksUnavailableForLocus :: sMessageIndex;
}



int SetupErrorNumberPeaksUnavailableForLocus :: GetScope () const {

	return SetupErrorNumberPeaksUnavailableForLocus :: sMessageScope;
}



int SetupErrorNumberPeaksUnavailableForLocus :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	SetupErrorNumberPeaksUnavailableForLocus* noticeType = new SetupErrorNumberPeaksUnavailableForLocus;
	warehouse->AddType (noticeType);
	return 1;
}





PossibleExtraLocusAllele :: PossibleExtraLocusAllele () : Notice () {

}


PossibleExtraLocusAllele :: PossibleExtraLocusAllele (const PossibleExtraLocusAllele& note) : Notice ((const Notice&) note) {

}



PossibleExtraLocusAllele :: ~PossibleExtraLocusAllele () {

}



int PossibleExtraLocusAllele :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PossibleExtraLocusAllele::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PossibleExtraLocusAllele::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PossibleExtraLocusAllele::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PossibleExtraLocusAllele::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PossibleExtraLocusAllele::sLabel = temp;

	else
		PossibleExtraLocusAllele::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PossibleExtraLocusAllele::sTriggerLowPriority = true;

		else
			PossibleExtraLocusAllele::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PossibleExtraLocusAllele::sDoNotCall = true;

		else
			PossibleExtraLocusAllele::sDoNotCall = false;
	}


	int N = PossibleExtraLocusAllele::sMessageList.Entries ();

	if (N == 1)
		N++;

	PossibleExtraLocusAllele::sMessageArray = new RGString* [N];
	RGDListIterator it (PossibleExtraLocusAllele::sMessageList);

	for (int i=0; i<N; i++)
		PossibleExtraLocusAllele::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PossibleExtraLocusAllele :: GetSubject () const {

	return PossibleExtraLocusAllele::sSubject;
}


RGString PossibleExtraLocusAllele :: GetLabel () const {

	return PossibleExtraLocusAllele::sLabel;
}


bool PossibleExtraLocusAllele :: IsTriggerForLowPriority () const {

	return PossibleExtraLocusAllele::sTriggerLowPriority;
}


bool PossibleExtraLocusAllele :: IsDoNotCall () const {

	return PossibleExtraLocusAllele::sDoNotCall;
}



RGString PossibleExtraLocusAllele :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PossibleExtraLocusAllele::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PossibleExtraLocusAllele::sMessageArray [i];
	return returnValue;
}



int PossibleExtraLocusAllele :: GetMessageLevel () const {

	return PossibleExtraLocusAllele::sMessageLevel;
}



int PossibleExtraLocusAllele :: GetNumberOfMessageParts () const {

	return PossibleExtraLocusAllele::sMessageList.Entries ();
}


int PossibleExtraLocusAllele :: GetSeverityLevel () const {

	return PossibleExtraLocusAllele::sMessageLevel;
}


void PossibleExtraLocusAllele :: SetIndexAndScope (int index, int scope) const {

	PossibleExtraLocusAllele::sMessageIndex = index;
	PossibleExtraLocusAllele::sMessageScope = scope;
}


int PossibleExtraLocusAllele :: GetMessageIndex () const {

	return PossibleExtraLocusAllele :: sMessageIndex;
}



int PossibleExtraLocusAllele :: GetScope () const {

	return PossibleExtraLocusAllele :: sMessageScope;
}



int PossibleExtraLocusAllele :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PossibleExtraLocusAllele* noticeType = new PossibleExtraLocusAllele;
	warehouse->AddType (noticeType);
	return 1;
}





OsirisBelowMinRFUButRawDataOK :: OsirisBelowMinRFUButRawDataOK () : Notice () {

}


OsirisBelowMinRFUButRawDataOK :: OsirisBelowMinRFUButRawDataOK (const OsirisBelowMinRFUButRawDataOK& note) : Notice ((const Notice&) note) {

}



OsirisBelowMinRFUButRawDataOK :: ~OsirisBelowMinRFUButRawDataOK () {

}



int OsirisBelowMinRFUButRawDataOK :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (OsirisBelowMinRFUButRawDataOK::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		OsirisBelowMinRFUButRawDataOK::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OsirisBelowMinRFUButRawDataOK::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OsirisBelowMinRFUButRawDataOK::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		OsirisBelowMinRFUButRawDataOK::sLabel = temp;

	else
		OsirisBelowMinRFUButRawDataOK::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OsirisBelowMinRFUButRawDataOK::sTriggerLowPriority = true;

		else
			OsirisBelowMinRFUButRawDataOK::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OsirisBelowMinRFUButRawDataOK::sDoNotCall = true;

		else
			OsirisBelowMinRFUButRawDataOK::sDoNotCall = false;
	}


	int N = OsirisBelowMinRFUButRawDataOK::sMessageList.Entries ();

	if (N == 1)
		N++;

	OsirisBelowMinRFUButRawDataOK::sMessageArray = new RGString* [N];
	RGDListIterator it (OsirisBelowMinRFUButRawDataOK::sMessageList);

	for (int i=0; i<N; i++)
		OsirisBelowMinRFUButRawDataOK::sMessageArray [i] = (RGString*) it ();

	return status;
}


int OsirisBelowMinRFUButRawDataOK :: GetSubject () const {

	return OsirisBelowMinRFUButRawDataOK::sSubject;
}


RGString OsirisBelowMinRFUButRawDataOK :: GetLabel () const {

	return OsirisBelowMinRFUButRawDataOK::sLabel;
}


bool OsirisBelowMinRFUButRawDataOK :: IsTriggerForLowPriority () const {

	return OsirisBelowMinRFUButRawDataOK::sTriggerLowPriority;
}


bool OsirisBelowMinRFUButRawDataOK :: IsDoNotCall () const {

	return OsirisBelowMinRFUButRawDataOK::sDoNotCall;
}



RGString OsirisBelowMinRFUButRawDataOK :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= OsirisBelowMinRFUButRawDataOK::sMessageList.Entries ()))
		return returnValue;

	returnValue = *OsirisBelowMinRFUButRawDataOK::sMessageArray [i];
	return returnValue;
}



int OsirisBelowMinRFUButRawDataOK :: GetMessageLevel () const {

	return OsirisBelowMinRFUButRawDataOK::sMessageLevel;
}



int OsirisBelowMinRFUButRawDataOK :: GetNumberOfMessageParts () const {

	return OsirisBelowMinRFUButRawDataOK::sMessageList.Entries ();
}


int OsirisBelowMinRFUButRawDataOK :: GetSeverityLevel () const {

	return OsirisBelowMinRFUButRawDataOK::sMessageLevel;
}


void OsirisBelowMinRFUButRawDataOK :: SetIndexAndScope (int index, int scope) const {

	OsirisBelowMinRFUButRawDataOK::sMessageIndex = index;
	OsirisBelowMinRFUButRawDataOK::sMessageScope = scope;
}


int OsirisBelowMinRFUButRawDataOK :: GetMessageIndex () const {

	return OsirisBelowMinRFUButRawDataOK :: sMessageIndex;
}



int OsirisBelowMinRFUButRawDataOK :: GetScope () const {

	return OsirisBelowMinRFUButRawDataOK :: sMessageScope;
}



int OsirisBelowMinRFUButRawDataOK :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	OsirisBelowMinRFUButRawDataOK* noticeType = new OsirisBelowMinRFUButRawDataOK;
	warehouse->AddType (noticeType);
	return 1;
}





RawDataBelowMinRFUButOsirisOK :: RawDataBelowMinRFUButOsirisOK () : Notice () {

}


RawDataBelowMinRFUButOsirisOK :: RawDataBelowMinRFUButOsirisOK (const RawDataBelowMinRFUButOsirisOK& note) : Notice ((const Notice&) note) {

}



RawDataBelowMinRFUButOsirisOK :: ~RawDataBelowMinRFUButOsirisOK () {

}



int RawDataBelowMinRFUButOsirisOK :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (RawDataBelowMinRFUButOsirisOK::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		RawDataBelowMinRFUButOsirisOK::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	RawDataBelowMinRFUButOsirisOK::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	RawDataBelowMinRFUButOsirisOK::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		RawDataBelowMinRFUButOsirisOK::sLabel = temp;

	else
		RawDataBelowMinRFUButOsirisOK::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			RawDataBelowMinRFUButOsirisOK::sTriggerLowPriority = true;

		else
			RawDataBelowMinRFUButOsirisOK::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			RawDataBelowMinRFUButOsirisOK::sDoNotCall = true;

		else
			RawDataBelowMinRFUButOsirisOK::sDoNotCall = false;
	}


	int N = RawDataBelowMinRFUButOsirisOK::sMessageList.Entries ();

	if (N == 1)
		N++;

	RawDataBelowMinRFUButOsirisOK::sMessageArray = new RGString* [N];
	RGDListIterator it (RawDataBelowMinRFUButOsirisOK::sMessageList);

	for (int i=0; i<N; i++)
		RawDataBelowMinRFUButOsirisOK::sMessageArray [i] = (RGString*) it ();

	return status;
}


int RawDataBelowMinRFUButOsirisOK :: GetSubject () const {

	return RawDataBelowMinRFUButOsirisOK::sSubject;
}


RGString RawDataBelowMinRFUButOsirisOK :: GetLabel () const {

	return RawDataBelowMinRFUButOsirisOK::sLabel;
}


bool RawDataBelowMinRFUButOsirisOK :: IsTriggerForLowPriority () const {

	return RawDataBelowMinRFUButOsirisOK::sTriggerLowPriority;
}


bool RawDataBelowMinRFUButOsirisOK :: IsDoNotCall () const {

	return RawDataBelowMinRFUButOsirisOK::sDoNotCall;
}



RGString RawDataBelowMinRFUButOsirisOK :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= RawDataBelowMinRFUButOsirisOK::sMessageList.Entries ()))
		return returnValue;

	returnValue = *RawDataBelowMinRFUButOsirisOK::sMessageArray [i];
	return returnValue;
}



int RawDataBelowMinRFUButOsirisOK :: GetMessageLevel () const {

	return RawDataBelowMinRFUButOsirisOK::sMessageLevel;
}



int RawDataBelowMinRFUButOsirisOK :: GetNumberOfMessageParts () const {

	return RawDataBelowMinRFUButOsirisOK::sMessageList.Entries ();
}


int RawDataBelowMinRFUButOsirisOK :: GetSeverityLevel () const {

	return RawDataBelowMinRFUButOsirisOK::sMessageLevel;
}


void RawDataBelowMinRFUButOsirisOK :: SetIndexAndScope (int index, int scope) const {

	RawDataBelowMinRFUButOsirisOK::sMessageIndex = index;
	RawDataBelowMinRFUButOsirisOK::sMessageScope = scope;
}


int RawDataBelowMinRFUButOsirisOK :: GetMessageIndex () const {

	return RawDataBelowMinRFUButOsirisOK :: sMessageIndex;
}



int RawDataBelowMinRFUButOsirisOK :: GetScope () const {

	return RawDataBelowMinRFUButOsirisOK :: sMessageScope;
}



int RawDataBelowMinRFUButOsirisOK :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	RawDataBelowMinRFUButOsirisOK* noticeType = new RawDataBelowMinRFUButOsirisOK;
	warehouse->AddType (noticeType);
	return 1;
}





OsirisAboveMaxRFUButRawDataOK :: OsirisAboveMaxRFUButRawDataOK () : Notice () {

}


OsirisAboveMaxRFUButRawDataOK :: OsirisAboveMaxRFUButRawDataOK (const OsirisAboveMaxRFUButRawDataOK& note) : Notice ((const Notice&) note) {

}



OsirisAboveMaxRFUButRawDataOK :: ~OsirisAboveMaxRFUButRawDataOK () {

}



int OsirisAboveMaxRFUButRawDataOK :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (OsirisAboveMaxRFUButRawDataOK::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		OsirisAboveMaxRFUButRawDataOK::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OsirisAboveMaxRFUButRawDataOK::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OsirisAboveMaxRFUButRawDataOK::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		OsirisAboveMaxRFUButRawDataOK::sLabel = temp;

	else
		OsirisAboveMaxRFUButRawDataOK::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OsirisAboveMaxRFUButRawDataOK::sTriggerLowPriority = true;

		else
			OsirisAboveMaxRFUButRawDataOK::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OsirisAboveMaxRFUButRawDataOK::sDoNotCall = true;

		else
			OsirisAboveMaxRFUButRawDataOK::sDoNotCall = false;
	}


	int N = OsirisAboveMaxRFUButRawDataOK::sMessageList.Entries ();

	if (N == 1)
		N++;

	OsirisAboveMaxRFUButRawDataOK::sMessageArray = new RGString* [N];
	RGDListIterator it (OsirisAboveMaxRFUButRawDataOK::sMessageList);

	for (int i=0; i<N; i++)
		OsirisAboveMaxRFUButRawDataOK::sMessageArray [i] = (RGString*) it ();

	return status;
}


int OsirisAboveMaxRFUButRawDataOK :: GetSubject () const {

	return OsirisAboveMaxRFUButRawDataOK::sSubject;
}


RGString OsirisAboveMaxRFUButRawDataOK :: GetLabel () const {

	return OsirisAboveMaxRFUButRawDataOK::sLabel;
}


bool OsirisAboveMaxRFUButRawDataOK :: IsTriggerForLowPriority () const {

	return OsirisAboveMaxRFUButRawDataOK::sTriggerLowPriority;
}


bool OsirisAboveMaxRFUButRawDataOK :: IsDoNotCall () const {

	return OsirisAboveMaxRFUButRawDataOK::sDoNotCall;
}



RGString OsirisAboveMaxRFUButRawDataOK :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= OsirisAboveMaxRFUButRawDataOK::sMessageList.Entries ()))
		return returnValue;

	returnValue = *OsirisAboveMaxRFUButRawDataOK::sMessageArray [i];
	return returnValue;
}



int OsirisAboveMaxRFUButRawDataOK :: GetMessageLevel () const {

	return OsirisAboveMaxRFUButRawDataOK::sMessageLevel;
}



int OsirisAboveMaxRFUButRawDataOK :: GetNumberOfMessageParts () const {

	return OsirisAboveMaxRFUButRawDataOK::sMessageList.Entries ();
}


int OsirisAboveMaxRFUButRawDataOK :: GetSeverityLevel () const {

	return OsirisAboveMaxRFUButRawDataOK::sMessageLevel;
}


void OsirisAboveMaxRFUButRawDataOK :: SetIndexAndScope (int index, int scope) const {

	OsirisAboveMaxRFUButRawDataOK::sMessageIndex = index;
	OsirisAboveMaxRFUButRawDataOK::sMessageScope = scope;
}


int OsirisAboveMaxRFUButRawDataOK :: GetMessageIndex () const {

	return OsirisAboveMaxRFUButRawDataOK :: sMessageIndex;
}



int OsirisAboveMaxRFUButRawDataOK :: GetScope () const {

	return OsirisAboveMaxRFUButRawDataOK :: sMessageScope;
}



int OsirisAboveMaxRFUButRawDataOK :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	OsirisAboveMaxRFUButRawDataOK* noticeType = new OsirisAboveMaxRFUButRawDataOK;
	warehouse->AddType (noticeType);
	return 1;
}





RawDataAboveMaxRFUButOsirisOK :: RawDataAboveMaxRFUButOsirisOK () : Notice () {

}


RawDataAboveMaxRFUButOsirisOK :: RawDataAboveMaxRFUButOsirisOK (const RawDataAboveMaxRFUButOsirisOK& note) : Notice ((const Notice&) note) {

}



RawDataAboveMaxRFUButOsirisOK :: ~RawDataAboveMaxRFUButOsirisOK () {

}



int RawDataAboveMaxRFUButOsirisOK :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (RawDataAboveMaxRFUButOsirisOK::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		RawDataAboveMaxRFUButOsirisOK::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	RawDataAboveMaxRFUButOsirisOK::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	RawDataAboveMaxRFUButOsirisOK::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		RawDataAboveMaxRFUButOsirisOK::sLabel = temp;

	else
		RawDataAboveMaxRFUButOsirisOK::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			RawDataAboveMaxRFUButOsirisOK::sTriggerLowPriority = true;

		else
			RawDataAboveMaxRFUButOsirisOK::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			RawDataAboveMaxRFUButOsirisOK::sDoNotCall = true;

		else
			RawDataAboveMaxRFUButOsirisOK::sDoNotCall = false;
	}


	int N = RawDataAboveMaxRFUButOsirisOK::sMessageList.Entries ();

	if (N == 1)
		N++;

	RawDataAboveMaxRFUButOsirisOK::sMessageArray = new RGString* [N];
	RGDListIterator it (RawDataAboveMaxRFUButOsirisOK::sMessageList);

	for (int i=0; i<N; i++)
		RawDataAboveMaxRFUButOsirisOK::sMessageArray [i] = (RGString*) it ();

	return status;
}


int RawDataAboveMaxRFUButOsirisOK :: GetSubject () const {

	return RawDataAboveMaxRFUButOsirisOK::sSubject;
}


RGString RawDataAboveMaxRFUButOsirisOK :: GetLabel () const {

	return RawDataAboveMaxRFUButOsirisOK::sLabel;
}


bool RawDataAboveMaxRFUButOsirisOK :: IsTriggerForLowPriority () const {

	return RawDataAboveMaxRFUButOsirisOK::sTriggerLowPriority;
}


bool RawDataAboveMaxRFUButOsirisOK :: IsDoNotCall () const {

	return RawDataAboveMaxRFUButOsirisOK::sDoNotCall;
}



RGString RawDataAboveMaxRFUButOsirisOK :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= RawDataAboveMaxRFUButOsirisOK::sMessageList.Entries ()))
		return returnValue;

	returnValue = *RawDataAboveMaxRFUButOsirisOK::sMessageArray [i];
	return returnValue;
}



int RawDataAboveMaxRFUButOsirisOK :: GetMessageLevel () const {

	return RawDataAboveMaxRFUButOsirisOK::sMessageLevel;
}



int RawDataAboveMaxRFUButOsirisOK :: GetNumberOfMessageParts () const {

	return RawDataAboveMaxRFUButOsirisOK::sMessageList.Entries ();
}


int RawDataAboveMaxRFUButOsirisOK :: GetSeverityLevel () const {

	return RawDataAboveMaxRFUButOsirisOK::sMessageLevel;
}


void RawDataAboveMaxRFUButOsirisOK :: SetIndexAndScope (int index, int scope) const {

	RawDataAboveMaxRFUButOsirisOK::sMessageIndex = index;
	RawDataAboveMaxRFUButOsirisOK::sMessageScope = scope;
}


int RawDataAboveMaxRFUButOsirisOK :: GetMessageIndex () const {

	return RawDataAboveMaxRFUButOsirisOK :: sMessageIndex;
}



int RawDataAboveMaxRFUButOsirisOK :: GetScope () const {

	return RawDataAboveMaxRFUButOsirisOK :: sMessageScope;
}



int RawDataAboveMaxRFUButOsirisOK :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	RawDataAboveMaxRFUButOsirisOK* noticeType = new RawDataAboveMaxRFUButOsirisOK;
	warehouse->AddType (noticeType);
	return 1;
}





LocusHasAllelesWithCriticalNotices :: LocusHasAllelesWithCriticalNotices () : Notice () {

}


LocusHasAllelesWithCriticalNotices :: LocusHasAllelesWithCriticalNotices (const LocusHasAllelesWithCriticalNotices& note) : Notice ((const Notice&) note) {

}



LocusHasAllelesWithCriticalNotices :: ~LocusHasAllelesWithCriticalNotices () {

}



int LocusHasAllelesWithCriticalNotices :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LocusHasAllelesWithCriticalNotices::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LocusHasAllelesWithCriticalNotices::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LocusHasAllelesWithCriticalNotices::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LocusHasAllelesWithCriticalNotices::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LocusHasAllelesWithCriticalNotices::sLabel = temp;

	else
		LocusHasAllelesWithCriticalNotices::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LocusHasAllelesWithCriticalNotices::sTriggerLowPriority = true;

		else
			LocusHasAllelesWithCriticalNotices::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LocusHasAllelesWithCriticalNotices::sDoNotCall = true;

		else
			LocusHasAllelesWithCriticalNotices::sDoNotCall = false;
	}


	int N = LocusHasAllelesWithCriticalNotices::sMessageList.Entries ();

	if (N == 1)
		N++;

	LocusHasAllelesWithCriticalNotices::sMessageArray = new RGString* [N];
	RGDListIterator it (LocusHasAllelesWithCriticalNotices::sMessageList);

	for (int i=0; i<N; i++)
		LocusHasAllelesWithCriticalNotices::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LocusHasAllelesWithCriticalNotices :: GetSubject () const {

	return LocusHasAllelesWithCriticalNotices::sSubject;
}


RGString LocusHasAllelesWithCriticalNotices :: GetLabel () const {

	return LocusHasAllelesWithCriticalNotices::sLabel;
}


bool LocusHasAllelesWithCriticalNotices :: IsTriggerForLowPriority () const {

	return LocusHasAllelesWithCriticalNotices::sTriggerLowPriority;
}


bool LocusHasAllelesWithCriticalNotices :: IsDoNotCall () const {

	return LocusHasAllelesWithCriticalNotices::sDoNotCall;
}



RGString LocusHasAllelesWithCriticalNotices :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LocusHasAllelesWithCriticalNotices::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LocusHasAllelesWithCriticalNotices::sMessageArray [i];
	return returnValue;
}



int LocusHasAllelesWithCriticalNotices :: GetMessageLevel () const {

	return LocusHasAllelesWithCriticalNotices::sMessageLevel;
}



int LocusHasAllelesWithCriticalNotices :: GetNumberOfMessageParts () const {

	return LocusHasAllelesWithCriticalNotices::sMessageList.Entries ();
}


int LocusHasAllelesWithCriticalNotices :: GetSeverityLevel () const {

	return LocusHasAllelesWithCriticalNotices::sMessageLevel;
}


void LocusHasAllelesWithCriticalNotices :: SetIndexAndScope (int index, int scope) const {

	LocusHasAllelesWithCriticalNotices::sMessageIndex = index;
	LocusHasAllelesWithCriticalNotices::sMessageScope = scope;
}


int LocusHasAllelesWithCriticalNotices :: GetMessageIndex () const {

	return LocusHasAllelesWithCriticalNotices :: sMessageIndex;
}



int LocusHasAllelesWithCriticalNotices :: GetScope () const {

	return LocusHasAllelesWithCriticalNotices :: sMessageScope;
}



int LocusHasAllelesWithCriticalNotices :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LocusHasAllelesWithCriticalNotices* noticeType = new LocusHasAllelesWithCriticalNotices;
	warehouse->AddType (noticeType);
	return 1;
}





ILSHasPeaksWithCriticalNotices :: ILSHasPeaksWithCriticalNotices () : Notice () {

}


ILSHasPeaksWithCriticalNotices :: ILSHasPeaksWithCriticalNotices (const ILSHasPeaksWithCriticalNotices& note) : Notice ((const Notice&) note) {

}



ILSHasPeaksWithCriticalNotices :: ~ILSHasPeaksWithCriticalNotices () {

}



int ILSHasPeaksWithCriticalNotices :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ILSHasPeaksWithCriticalNotices::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ILSHasPeaksWithCriticalNotices::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSHasPeaksWithCriticalNotices::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSHasPeaksWithCriticalNotices::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ILSHasPeaksWithCriticalNotices::sLabel = temp;

	else
		ILSHasPeaksWithCriticalNotices::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSHasPeaksWithCriticalNotices::sTriggerLowPriority = true;

		else
			ILSHasPeaksWithCriticalNotices::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSHasPeaksWithCriticalNotices::sDoNotCall = true;

		else
			ILSHasPeaksWithCriticalNotices::sDoNotCall = false;
	}


	int N = ILSHasPeaksWithCriticalNotices::sMessageList.Entries ();

	if (N == 1)
		N++;

	ILSHasPeaksWithCriticalNotices::sMessageArray = new RGString* [N];
	RGDListIterator it (ILSHasPeaksWithCriticalNotices::sMessageList);

	for (int i=0; i<N; i++)
		ILSHasPeaksWithCriticalNotices::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ILSHasPeaksWithCriticalNotices :: GetSubject () const {

	return ILSHasPeaksWithCriticalNotices::sSubject;
}


RGString ILSHasPeaksWithCriticalNotices :: GetLabel () const {

	return ILSHasPeaksWithCriticalNotices::sLabel;
}


bool ILSHasPeaksWithCriticalNotices :: IsTriggerForLowPriority () const {

	return ILSHasPeaksWithCriticalNotices::sTriggerLowPriority;
}


bool ILSHasPeaksWithCriticalNotices :: IsDoNotCall () const {

	return ILSHasPeaksWithCriticalNotices::sDoNotCall;
}



RGString ILSHasPeaksWithCriticalNotices :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ILSHasPeaksWithCriticalNotices::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ILSHasPeaksWithCriticalNotices::sMessageArray [i];
	return returnValue;
}



int ILSHasPeaksWithCriticalNotices :: GetMessageLevel () const {

	return ILSHasPeaksWithCriticalNotices::sMessageLevel;
}



int ILSHasPeaksWithCriticalNotices :: GetNumberOfMessageParts () const {

	return ILSHasPeaksWithCriticalNotices::sMessageList.Entries ();
}


int ILSHasPeaksWithCriticalNotices :: GetSeverityLevel () const {

	return ILSHasPeaksWithCriticalNotices::sMessageLevel;
}


void ILSHasPeaksWithCriticalNotices :: SetIndexAndScope (int index, int scope) const {

	ILSHasPeaksWithCriticalNotices::sMessageIndex = index;
	ILSHasPeaksWithCriticalNotices::sMessageScope = scope;
}


int ILSHasPeaksWithCriticalNotices :: GetMessageIndex () const {

	return ILSHasPeaksWithCriticalNotices :: sMessageIndex;
}



int ILSHasPeaksWithCriticalNotices :: GetScope () const {

	return ILSHasPeaksWithCriticalNotices :: sMessageScope;
}



int ILSHasPeaksWithCriticalNotices :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ILSHasPeaksWithCriticalNotices* noticeType = new ILSHasPeaksWithCriticalNotices;
	warehouse->AddType (noticeType);
	return 1;
}





CannotSeparateILSInitialFragments :: CannotSeparateILSInitialFragments () : Notice () {

}


CannotSeparateILSInitialFragments :: CannotSeparateILSInitialFragments (const CannotSeparateILSInitialFragments& note) : Notice ((const Notice&) note) {

}



CannotSeparateILSInitialFragments :: ~CannotSeparateILSInitialFragments () {

}



int CannotSeparateILSInitialFragments :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (CannotSeparateILSInitialFragments::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		CannotSeparateILSInitialFragments::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CannotSeparateILSInitialFragments::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	CannotSeparateILSInitialFragments::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		CannotSeparateILSInitialFragments::sLabel = temp;

	else
		CannotSeparateILSInitialFragments::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CannotSeparateILSInitialFragments::sTriggerLowPriority = true;

		else
			CannotSeparateILSInitialFragments::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			CannotSeparateILSInitialFragments::sDoNotCall = true;

		else
			CannotSeparateILSInitialFragments::sDoNotCall = false;
	}


	int N = CannotSeparateILSInitialFragments::sMessageList.Entries ();

	if (N == 1)
		N++;

	CannotSeparateILSInitialFragments::sMessageArray = new RGString* [N];
	RGDListIterator it (CannotSeparateILSInitialFragments::sMessageList);

	for (int i=0; i<N; i++)
		CannotSeparateILSInitialFragments::sMessageArray [i] = (RGString*) it ();

	return status;
}


int CannotSeparateILSInitialFragments :: GetSubject () const {

	return CannotSeparateILSInitialFragments::sSubject;
}


RGString CannotSeparateILSInitialFragments :: GetLabel () const {

	return CannotSeparateILSInitialFragments::sLabel;
}


bool CannotSeparateILSInitialFragments :: IsTriggerForLowPriority () const {

	return CannotSeparateILSInitialFragments::sTriggerLowPriority;
}


bool CannotSeparateILSInitialFragments :: IsDoNotCall () const {

	return CannotSeparateILSInitialFragments::sDoNotCall;
}



RGString CannotSeparateILSInitialFragments :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= CannotSeparateILSInitialFragments::sMessageList.Entries ()))
		return returnValue;

	returnValue = *CannotSeparateILSInitialFragments::sMessageArray [i];
	return returnValue;
}



int CannotSeparateILSInitialFragments :: GetMessageLevel () const {

	return CannotSeparateILSInitialFragments::sMessageLevel;
}



int CannotSeparateILSInitialFragments :: GetNumberOfMessageParts () const {

	return CannotSeparateILSInitialFragments::sMessageList.Entries ();
}


int CannotSeparateILSInitialFragments :: GetSeverityLevel () const {

	return CannotSeparateILSInitialFragments::sMessageLevel;
}


void CannotSeparateILSInitialFragments :: SetIndexAndScope (int index, int scope) const {

	CannotSeparateILSInitialFragments::sMessageIndex = index;
	CannotSeparateILSInitialFragments::sMessageScope = scope;
}


int CannotSeparateILSInitialFragments :: GetMessageIndex () const {

	return CannotSeparateILSInitialFragments :: sMessageIndex;
}



int CannotSeparateILSInitialFragments :: GetScope () const {

	return CannotSeparateILSInitialFragments :: sMessageScope;
}



int CannotSeparateILSInitialFragments :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	CannotSeparateILSInitialFragments* noticeType = new CannotSeparateILSInitialFragments;
	warehouse->AddType (noticeType);
	return 1;
}





ILSNeighborFiltersRemoveTooManyPeaks :: ILSNeighborFiltersRemoveTooManyPeaks () : Notice () {

}


ILSNeighborFiltersRemoveTooManyPeaks :: ILSNeighborFiltersRemoveTooManyPeaks (const ILSNeighborFiltersRemoveTooManyPeaks& note) : Notice ((const Notice&) note) {

}



ILSNeighborFiltersRemoveTooManyPeaks :: ~ILSNeighborFiltersRemoveTooManyPeaks () {

}



int ILSNeighborFiltersRemoveTooManyPeaks :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ILSNeighborFiltersRemoveTooManyPeaks::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ILSNeighborFiltersRemoveTooManyPeaks::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSNeighborFiltersRemoveTooManyPeaks::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSNeighborFiltersRemoveTooManyPeaks::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ILSNeighborFiltersRemoveTooManyPeaks::sLabel = temp;

	else
		ILSNeighborFiltersRemoveTooManyPeaks::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSNeighborFiltersRemoveTooManyPeaks::sTriggerLowPriority = true;

		else
			ILSNeighborFiltersRemoveTooManyPeaks::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSNeighborFiltersRemoveTooManyPeaks::sDoNotCall = true;

		else
			ILSNeighborFiltersRemoveTooManyPeaks::sDoNotCall = false;
	}


	int N = ILSNeighborFiltersRemoveTooManyPeaks::sMessageList.Entries ();

	if (N == 1)
		N++;

	ILSNeighborFiltersRemoveTooManyPeaks::sMessageArray = new RGString* [N];
	RGDListIterator it (ILSNeighborFiltersRemoveTooManyPeaks::sMessageList);

	for (int i=0; i<N; i++)
		ILSNeighborFiltersRemoveTooManyPeaks::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ILSNeighborFiltersRemoveTooManyPeaks :: GetSubject () const {

	return ILSNeighborFiltersRemoveTooManyPeaks::sSubject;
}


RGString ILSNeighborFiltersRemoveTooManyPeaks :: GetLabel () const {

	return ILSNeighborFiltersRemoveTooManyPeaks::sLabel;
}


bool ILSNeighborFiltersRemoveTooManyPeaks :: IsTriggerForLowPriority () const {

	return ILSNeighborFiltersRemoveTooManyPeaks::sTriggerLowPriority;
}


bool ILSNeighborFiltersRemoveTooManyPeaks :: IsDoNotCall () const {

	return ILSNeighborFiltersRemoveTooManyPeaks::sDoNotCall;
}



RGString ILSNeighborFiltersRemoveTooManyPeaks :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ILSNeighborFiltersRemoveTooManyPeaks::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ILSNeighborFiltersRemoveTooManyPeaks::sMessageArray [i];
	return returnValue;
}



int ILSNeighborFiltersRemoveTooManyPeaks :: GetMessageLevel () const {

	return ILSNeighborFiltersRemoveTooManyPeaks::sMessageLevel;
}



int ILSNeighborFiltersRemoveTooManyPeaks :: GetNumberOfMessageParts () const {

	return ILSNeighborFiltersRemoveTooManyPeaks::sMessageList.Entries ();
}


int ILSNeighborFiltersRemoveTooManyPeaks :: GetSeverityLevel () const {

	return ILSNeighborFiltersRemoveTooManyPeaks::sMessageLevel;
}


void ILSNeighborFiltersRemoveTooManyPeaks :: SetIndexAndScope (int index, int scope) const {

	ILSNeighborFiltersRemoveTooManyPeaks::sMessageIndex = index;
	ILSNeighborFiltersRemoveTooManyPeaks::sMessageScope = scope;
}


int ILSNeighborFiltersRemoveTooManyPeaks :: GetMessageIndex () const {

	return ILSNeighborFiltersRemoveTooManyPeaks :: sMessageIndex;
}



int ILSNeighborFiltersRemoveTooManyPeaks :: GetScope () const {

	return ILSNeighborFiltersRemoveTooManyPeaks :: sMessageScope;
}



int ILSNeighborFiltersRemoveTooManyPeaks :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ILSNeighborFiltersRemoveTooManyPeaks* noticeType = new ILSNeighborFiltersRemoveTooManyPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





ILSHeightFiltersRemoveTooManyPeaks :: ILSHeightFiltersRemoveTooManyPeaks () : Notice () {

}


ILSHeightFiltersRemoveTooManyPeaks :: ILSHeightFiltersRemoveTooManyPeaks (const ILSHeightFiltersRemoveTooManyPeaks& note) : Notice ((const Notice&) note) {

}



ILSHeightFiltersRemoveTooManyPeaks :: ~ILSHeightFiltersRemoveTooManyPeaks () {

}



int ILSHeightFiltersRemoveTooManyPeaks :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ILSHeightFiltersRemoveTooManyPeaks::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ILSHeightFiltersRemoveTooManyPeaks::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSHeightFiltersRemoveTooManyPeaks::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSHeightFiltersRemoveTooManyPeaks::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ILSHeightFiltersRemoveTooManyPeaks::sLabel = temp;

	else
		ILSHeightFiltersRemoveTooManyPeaks::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSHeightFiltersRemoveTooManyPeaks::sTriggerLowPriority = true;

		else
			ILSHeightFiltersRemoveTooManyPeaks::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSHeightFiltersRemoveTooManyPeaks::sDoNotCall = true;

		else
			ILSHeightFiltersRemoveTooManyPeaks::sDoNotCall = false;
	}


	int N = ILSHeightFiltersRemoveTooManyPeaks::sMessageList.Entries ();

	if (N == 1)
		N++;

	ILSHeightFiltersRemoveTooManyPeaks::sMessageArray = new RGString* [N];
	RGDListIterator it (ILSHeightFiltersRemoveTooManyPeaks::sMessageList);

	for (int i=0; i<N; i++)
		ILSHeightFiltersRemoveTooManyPeaks::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ILSHeightFiltersRemoveTooManyPeaks :: GetSubject () const {

	return ILSHeightFiltersRemoveTooManyPeaks::sSubject;
}


RGString ILSHeightFiltersRemoveTooManyPeaks :: GetLabel () const {

	return ILSHeightFiltersRemoveTooManyPeaks::sLabel;
}


bool ILSHeightFiltersRemoveTooManyPeaks :: IsTriggerForLowPriority () const {

	return ILSHeightFiltersRemoveTooManyPeaks::sTriggerLowPriority;
}


bool ILSHeightFiltersRemoveTooManyPeaks :: IsDoNotCall () const {

	return ILSHeightFiltersRemoveTooManyPeaks::sDoNotCall;
}



RGString ILSHeightFiltersRemoveTooManyPeaks :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ILSHeightFiltersRemoveTooManyPeaks::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ILSHeightFiltersRemoveTooManyPeaks::sMessageArray [i];
	return returnValue;
}



int ILSHeightFiltersRemoveTooManyPeaks :: GetMessageLevel () const {

	return ILSHeightFiltersRemoveTooManyPeaks::sMessageLevel;
}



int ILSHeightFiltersRemoveTooManyPeaks :: GetNumberOfMessageParts () const {

	return ILSHeightFiltersRemoveTooManyPeaks::sMessageList.Entries ();
}


int ILSHeightFiltersRemoveTooManyPeaks :: GetSeverityLevel () const {

	return ILSHeightFiltersRemoveTooManyPeaks::sMessageLevel;
}


void ILSHeightFiltersRemoveTooManyPeaks :: SetIndexAndScope (int index, int scope) const {

	ILSHeightFiltersRemoveTooManyPeaks::sMessageIndex = index;
	ILSHeightFiltersRemoveTooManyPeaks::sMessageScope = scope;
}


int ILSHeightFiltersRemoveTooManyPeaks :: GetMessageIndex () const {

	return ILSHeightFiltersRemoveTooManyPeaks :: sMessageIndex;
}



int ILSHeightFiltersRemoveTooManyPeaks :: GetScope () const {

	return ILSHeightFiltersRemoveTooManyPeaks :: sMessageScope;
}



int ILSHeightFiltersRemoveTooManyPeaks :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ILSHeightFiltersRemoveTooManyPeaks* noticeType = new ILSHeightFiltersRemoveTooManyPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





SignalPeakBelowFractionalFilterHeight :: SignalPeakBelowFractionalFilterHeight () : Notice () {

}


SignalPeakBelowFractionalFilterHeight :: SignalPeakBelowFractionalFilterHeight (const SignalPeakBelowFractionalFilterHeight& note) : Notice ((const Notice&) note) {

}



SignalPeakBelowFractionalFilterHeight :: ~SignalPeakBelowFractionalFilterHeight () {

}



int SignalPeakBelowFractionalFilterHeight :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (SignalPeakBelowFractionalFilterHeight::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		SignalPeakBelowFractionalFilterHeight::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalPeakBelowFractionalFilterHeight::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalPeakBelowFractionalFilterHeight::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		SignalPeakBelowFractionalFilterHeight::sLabel = temp;

	else
		SignalPeakBelowFractionalFilterHeight::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalPeakBelowFractionalFilterHeight::sTriggerLowPriority = true;

		else
			SignalPeakBelowFractionalFilterHeight::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalPeakBelowFractionalFilterHeight::sDoNotCall = true;

		else
			SignalPeakBelowFractionalFilterHeight::sDoNotCall = false;
	}


	int N = SignalPeakBelowFractionalFilterHeight::sMessageList.Entries ();

	if (N == 1)
		N++;

	SignalPeakBelowFractionalFilterHeight::sMessageArray = new RGString* [N];
	RGDListIterator it (SignalPeakBelowFractionalFilterHeight::sMessageList);

	for (int i=0; i<N; i++)
		SignalPeakBelowFractionalFilterHeight::sMessageArray [i] = (RGString*) it ();

	return status;
}


int SignalPeakBelowFractionalFilterHeight :: GetSubject () const {

	return SignalPeakBelowFractionalFilterHeight::sSubject;
}


RGString SignalPeakBelowFractionalFilterHeight :: GetLabel () const {

	return SignalPeakBelowFractionalFilterHeight::sLabel;
}


bool SignalPeakBelowFractionalFilterHeight :: IsTriggerForLowPriority () const {

	return SignalPeakBelowFractionalFilterHeight::sTriggerLowPriority;
}


bool SignalPeakBelowFractionalFilterHeight :: IsDoNotCall () const {

	return SignalPeakBelowFractionalFilterHeight::sDoNotCall;
}



RGString SignalPeakBelowFractionalFilterHeight :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= SignalPeakBelowFractionalFilterHeight::sMessageList.Entries ()))
		return returnValue;

	returnValue = *SignalPeakBelowFractionalFilterHeight::sMessageArray [i];
	return returnValue;
}



int SignalPeakBelowFractionalFilterHeight :: GetMessageLevel () const {

	return SignalPeakBelowFractionalFilterHeight::sMessageLevel;
}



int SignalPeakBelowFractionalFilterHeight :: GetNumberOfMessageParts () const {

	return SignalPeakBelowFractionalFilterHeight::sMessageList.Entries ();
}


int SignalPeakBelowFractionalFilterHeight :: GetSeverityLevel () const {

	return SignalPeakBelowFractionalFilterHeight::sMessageLevel;
}


void SignalPeakBelowFractionalFilterHeight :: SetIndexAndScope (int index, int scope) const {

	SignalPeakBelowFractionalFilterHeight::sMessageIndex = index;
	SignalPeakBelowFractionalFilterHeight::sMessageScope = scope;
}


int SignalPeakBelowFractionalFilterHeight :: GetMessageIndex () const {

	return SignalPeakBelowFractionalFilterHeight :: sMessageIndex;
}



int SignalPeakBelowFractionalFilterHeight :: GetScope () const {

	return SignalPeakBelowFractionalFilterHeight :: sMessageScope;
}



int SignalPeakBelowFractionalFilterHeight :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	SignalPeakBelowFractionalFilterHeight* noticeType = new SignalPeakBelowFractionalFilterHeight;
	warehouse->AddType (noticeType);
	return 1;
}





ILSRelativeHeightsInconsistent :: ILSRelativeHeightsInconsistent () : Notice () {

}


ILSRelativeHeightsInconsistent :: ILSRelativeHeightsInconsistent (const ILSRelativeHeightsInconsistent& note) : Notice ((const Notice&) note) {

}



ILSRelativeHeightsInconsistent :: ~ILSRelativeHeightsInconsistent () {

}



int ILSRelativeHeightsInconsistent :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ILSRelativeHeightsInconsistent::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ILSRelativeHeightsInconsistent::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSRelativeHeightsInconsistent::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ILSRelativeHeightsInconsistent::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ILSRelativeHeightsInconsistent::sLabel = temp;

	else
		ILSRelativeHeightsInconsistent::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSRelativeHeightsInconsistent::sTriggerLowPriority = true;

		else
			ILSRelativeHeightsInconsistent::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ILSRelativeHeightsInconsistent::sDoNotCall = true;

		else
			ILSRelativeHeightsInconsistent::sDoNotCall = false;
	}


	int N = ILSRelativeHeightsInconsistent::sMessageList.Entries ();

	if (N == 1)
		N++;

	ILSRelativeHeightsInconsistent::sMessageArray = new RGString* [N];
	RGDListIterator it (ILSRelativeHeightsInconsistent::sMessageList);

	for (int i=0; i<N; i++)
		ILSRelativeHeightsInconsistent::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ILSRelativeHeightsInconsistent :: GetSubject () const {

	return ILSRelativeHeightsInconsistent::sSubject;
}


RGString ILSRelativeHeightsInconsistent :: GetLabel () const {

	return ILSRelativeHeightsInconsistent::sLabel;
}


bool ILSRelativeHeightsInconsistent :: IsTriggerForLowPriority () const {

	return ILSRelativeHeightsInconsistent::sTriggerLowPriority;
}


bool ILSRelativeHeightsInconsistent :: IsDoNotCall () const {

	return ILSRelativeHeightsInconsistent::sDoNotCall;
}



RGString ILSRelativeHeightsInconsistent :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ILSRelativeHeightsInconsistent::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ILSRelativeHeightsInconsistent::sMessageArray [i];
	return returnValue;
}



int ILSRelativeHeightsInconsistent :: GetMessageLevel () const {

	return ILSRelativeHeightsInconsistent::sMessageLevel;
}



int ILSRelativeHeightsInconsistent :: GetNumberOfMessageParts () const {

	return ILSRelativeHeightsInconsistent::sMessageList.Entries ();
}


int ILSRelativeHeightsInconsistent :: GetSeverityLevel () const {

	return ILSRelativeHeightsInconsistent::sMessageLevel;
}


void ILSRelativeHeightsInconsistent :: SetIndexAndScope (int index, int scope) const {

	ILSRelativeHeightsInconsistent::sMessageIndex = index;
	ILSRelativeHeightsInconsistent::sMessageScope = scope;
}


int ILSRelativeHeightsInconsistent :: GetMessageIndex () const {

	return ILSRelativeHeightsInconsistent :: sMessageIndex;
}



int ILSRelativeHeightsInconsistent :: GetScope () const {

	return ILSRelativeHeightsInconsistent :: sMessageScope;
}



int ILSRelativeHeightsInconsistent :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ILSRelativeHeightsInconsistent* noticeType = new ILSRelativeHeightsInconsistent;
	warehouse->AddType (noticeType);
	return 1;
}





ExtraneousPeakInAMEL :: ExtraneousPeakInAMEL () : Notice () {

}


ExtraneousPeakInAMEL :: ExtraneousPeakInAMEL (const ExtraneousPeakInAMEL& note) : Notice ((const Notice&) note) {

}



ExtraneousPeakInAMEL :: ~ExtraneousPeakInAMEL () {

}



int ExtraneousPeakInAMEL :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ExtraneousPeakInAMEL::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ExtraneousPeakInAMEL::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ExtraneousPeakInAMEL::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ExtraneousPeakInAMEL::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ExtraneousPeakInAMEL::sLabel = temp;

	else
		ExtraneousPeakInAMEL::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ExtraneousPeakInAMEL::sTriggerLowPriority = true;

		else
			ExtraneousPeakInAMEL::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ExtraneousPeakInAMEL::sDoNotCall = true;

		else
			ExtraneousPeakInAMEL::sDoNotCall = false;
	}


	int N = ExtraneousPeakInAMEL::sMessageList.Entries ();

	if (N == 1)
		N++;

	ExtraneousPeakInAMEL::sMessageArray = new RGString* [N];
	RGDListIterator it (ExtraneousPeakInAMEL::sMessageList);

	for (int i=0; i<N; i++)
		ExtraneousPeakInAMEL::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ExtraneousPeakInAMEL :: GetSubject () const {

	return ExtraneousPeakInAMEL::sSubject;
}


RGString ExtraneousPeakInAMEL :: GetLabel () const {

	return ExtraneousPeakInAMEL::sLabel;
}


bool ExtraneousPeakInAMEL :: IsTriggerForLowPriority () const {

	return ExtraneousPeakInAMEL::sTriggerLowPriority;
}


bool ExtraneousPeakInAMEL :: IsDoNotCall () const {

	return ExtraneousPeakInAMEL::sDoNotCall;
}



RGString ExtraneousPeakInAMEL :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ExtraneousPeakInAMEL::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ExtraneousPeakInAMEL::sMessageArray [i];
	return returnValue;
}



int ExtraneousPeakInAMEL :: GetMessageLevel () const {

	return ExtraneousPeakInAMEL::sMessageLevel;
}



int ExtraneousPeakInAMEL :: GetNumberOfMessageParts () const {

	return ExtraneousPeakInAMEL::sMessageList.Entries ();
}


int ExtraneousPeakInAMEL :: GetSeverityLevel () const {

	return ExtraneousPeakInAMEL::sMessageLevel;
}


void ExtraneousPeakInAMEL :: SetIndexAndScope (int index, int scope) const {

	ExtraneousPeakInAMEL::sMessageIndex = index;
	ExtraneousPeakInAMEL::sMessageScope = scope;
}


int ExtraneousPeakInAMEL :: GetMessageIndex () const {

	return ExtraneousPeakInAMEL :: sMessageIndex;
}



int ExtraneousPeakInAMEL :: GetScope () const {

	return ExtraneousPeakInAMEL :: sMessageScope;
}



int ExtraneousPeakInAMEL :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ExtraneousPeakInAMEL* noticeType = new ExtraneousPeakInAMEL;
	warehouse->AddType (noticeType);
	return 1;
}





OLPeakInAMEL :: OLPeakInAMEL () : Notice () {

}


OLPeakInAMEL :: OLPeakInAMEL (const OLPeakInAMEL& note) : Notice ((const Notice&) note) {

}



OLPeakInAMEL :: ~OLPeakInAMEL () {

}



int OLPeakInAMEL :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (OLPeakInAMEL::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		OLPeakInAMEL::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OLPeakInAMEL::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	OLPeakInAMEL::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		OLPeakInAMEL::sLabel = temp;

	else
		OLPeakInAMEL::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OLPeakInAMEL::sTriggerLowPriority = true;

		else
			OLPeakInAMEL::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			OLPeakInAMEL::sDoNotCall = true;

		else
			OLPeakInAMEL::sDoNotCall = false;
	}


	int N = OLPeakInAMEL::sMessageList.Entries ();

	if (N == 1)
		N++;

	OLPeakInAMEL::sMessageArray = new RGString* [N];
	RGDListIterator it (OLPeakInAMEL::sMessageList);

	for (int i=0; i<N; i++)
		OLPeakInAMEL::sMessageArray [i] = (RGString*) it ();

	return status;
}


int OLPeakInAMEL :: GetSubject () const {

	return OLPeakInAMEL::sSubject;
}


RGString OLPeakInAMEL :: GetLabel () const {

	return OLPeakInAMEL::sLabel;
}


bool OLPeakInAMEL :: IsTriggerForLowPriority () const {

	return OLPeakInAMEL::sTriggerLowPriority;
}


bool OLPeakInAMEL :: IsDoNotCall () const {

	return OLPeakInAMEL::sDoNotCall;
}



RGString OLPeakInAMEL :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= OLPeakInAMEL::sMessageList.Entries ()))
		return returnValue;

	returnValue = *OLPeakInAMEL::sMessageArray [i];
	return returnValue;
}



int OLPeakInAMEL :: GetMessageLevel () const {

	return OLPeakInAMEL::sMessageLevel;
}



int OLPeakInAMEL :: GetNumberOfMessageParts () const {

	return OLPeakInAMEL::sMessageList.Entries ();
}


int OLPeakInAMEL :: GetSeverityLevel () const {

	return OLPeakInAMEL::sMessageLevel;
}


void OLPeakInAMEL :: SetIndexAndScope (int index, int scope) const {

	OLPeakInAMEL::sMessageIndex = index;
	OLPeakInAMEL::sMessageScope = scope;
}


int OLPeakInAMEL :: GetMessageIndex () const {

	return OLPeakInAMEL :: sMessageIndex;
}



int OLPeakInAMEL :: GetScope () const {

	return OLPeakInAMEL :: sMessageScope;
}



int OLPeakInAMEL :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	OLPeakInAMEL* noticeType = new OLPeakInAMEL;
	warehouse->AddType (noticeType);
	return 1;
}





NoPrimerPeaksInNegativeControlChannel :: NoPrimerPeaksInNegativeControlChannel () : Notice () {

}


NoPrimerPeaksInNegativeControlChannel :: NoPrimerPeaksInNegativeControlChannel (const NoPrimerPeaksInNegativeControlChannel& note) : Notice ((const Notice&) note) {

}



NoPrimerPeaksInNegativeControlChannel :: ~NoPrimerPeaksInNegativeControlChannel () {

}



int NoPrimerPeaksInNegativeControlChannel :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoPrimerPeaksInNegativeControlChannel::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoPrimerPeaksInNegativeControlChannel::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoPrimerPeaksInNegativeControlChannel::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoPrimerPeaksInNegativeControlChannel::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoPrimerPeaksInNegativeControlChannel::sLabel = temp;

	else
		NoPrimerPeaksInNegativeControlChannel::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoPrimerPeaksInNegativeControlChannel::sTriggerLowPriority = true;

		else
			NoPrimerPeaksInNegativeControlChannel::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoPrimerPeaksInNegativeControlChannel::sDoNotCall = true;

		else
			NoPrimerPeaksInNegativeControlChannel::sDoNotCall = false;
	}


	int N = NoPrimerPeaksInNegativeControlChannel::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoPrimerPeaksInNegativeControlChannel::sMessageArray = new RGString* [N];
	RGDListIterator it (NoPrimerPeaksInNegativeControlChannel::sMessageList);

	for (int i=0; i<N; i++)
		NoPrimerPeaksInNegativeControlChannel::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoPrimerPeaksInNegativeControlChannel :: GetSubject () const {

	return NoPrimerPeaksInNegativeControlChannel::sSubject;
}


RGString NoPrimerPeaksInNegativeControlChannel :: GetLabel () const {

	return NoPrimerPeaksInNegativeControlChannel::sLabel;
}


bool NoPrimerPeaksInNegativeControlChannel :: IsTriggerForLowPriority () const {

	return NoPrimerPeaksInNegativeControlChannel::sTriggerLowPriority;
}


bool NoPrimerPeaksInNegativeControlChannel :: IsDoNotCall () const {

	return NoPrimerPeaksInNegativeControlChannel::sDoNotCall;
}



RGString NoPrimerPeaksInNegativeControlChannel :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoPrimerPeaksInNegativeControlChannel::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoPrimerPeaksInNegativeControlChannel::sMessageArray [i];
	return returnValue;
}



int NoPrimerPeaksInNegativeControlChannel :: GetMessageLevel () const {

	return NoPrimerPeaksInNegativeControlChannel::sMessageLevel;
}



int NoPrimerPeaksInNegativeControlChannel :: GetNumberOfMessageParts () const {

	return NoPrimerPeaksInNegativeControlChannel::sMessageList.Entries ();
}


int NoPrimerPeaksInNegativeControlChannel :: GetSeverityLevel () const {

	return NoPrimerPeaksInNegativeControlChannel::sMessageLevel;
}


void NoPrimerPeaksInNegativeControlChannel :: SetIndexAndScope (int index, int scope) const {

	NoPrimerPeaksInNegativeControlChannel::sMessageIndex = index;
	NoPrimerPeaksInNegativeControlChannel::sMessageScope = scope;
}


int NoPrimerPeaksInNegativeControlChannel :: GetMessageIndex () const {

	return NoPrimerPeaksInNegativeControlChannel :: sMessageIndex;
}



int NoPrimerPeaksInNegativeControlChannel :: GetScope () const {

	return NoPrimerPeaksInNegativeControlChannel :: sMessageScope;
}



int NoPrimerPeaksInNegativeControlChannel :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoPrimerPeaksInNegativeControlChannel* noticeType = new NoPrimerPeaksInNegativeControlChannel;
	warehouse->AddType (noticeType);
	return 1;
}





NoPrimerPeaksInOneOrMoreNegControlChannel :: NoPrimerPeaksInOneOrMoreNegControlChannel () : Notice () {

}


NoPrimerPeaksInOneOrMoreNegControlChannel :: NoPrimerPeaksInOneOrMoreNegControlChannel (const NoPrimerPeaksInOneOrMoreNegControlChannel& note) : Notice ((const Notice&) note) {

}



NoPrimerPeaksInOneOrMoreNegControlChannel :: ~NoPrimerPeaksInOneOrMoreNegControlChannel () {

}



int NoPrimerPeaksInOneOrMoreNegControlChannel :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoPrimerPeaksInOneOrMoreNegControlChannel::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NoPrimerPeaksInOneOrMoreNegControlChannel::sLabel = temp;

	else
		NoPrimerPeaksInOneOrMoreNegControlChannel::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoPrimerPeaksInOneOrMoreNegControlChannel::sTriggerLowPriority = true;

		else
			NoPrimerPeaksInOneOrMoreNegControlChannel::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NoPrimerPeaksInOneOrMoreNegControlChannel::sDoNotCall = true;

		else
			NoPrimerPeaksInOneOrMoreNegControlChannel::sDoNotCall = false;
	}


	int N = NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageList.Entries ();

	if (N == 1)
		N++;

	NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageArray = new RGString* [N];
	RGDListIterator it (NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageList);

	for (int i=0; i<N; i++)
		NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NoPrimerPeaksInOneOrMoreNegControlChannel :: GetSubject () const {

	return NoPrimerPeaksInOneOrMoreNegControlChannel::sSubject;
}


RGString NoPrimerPeaksInOneOrMoreNegControlChannel :: GetLabel () const {

	return NoPrimerPeaksInOneOrMoreNegControlChannel::sLabel;
}


bool NoPrimerPeaksInOneOrMoreNegControlChannel :: IsTriggerForLowPriority () const {

	return NoPrimerPeaksInOneOrMoreNegControlChannel::sTriggerLowPriority;
}


bool NoPrimerPeaksInOneOrMoreNegControlChannel :: IsDoNotCall () const {

	return NoPrimerPeaksInOneOrMoreNegControlChannel::sDoNotCall;
}



RGString NoPrimerPeaksInOneOrMoreNegControlChannel :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageArray [i];
	return returnValue;
}



int NoPrimerPeaksInOneOrMoreNegControlChannel :: GetMessageLevel () const {

	return NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageLevel;
}



int NoPrimerPeaksInOneOrMoreNegControlChannel :: GetNumberOfMessageParts () const {

	return NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageList.Entries ();
}


int NoPrimerPeaksInOneOrMoreNegControlChannel :: GetSeverityLevel () const {

	return NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageLevel;
}


void NoPrimerPeaksInOneOrMoreNegControlChannel :: SetIndexAndScope (int index, int scope) const {

	NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageIndex = index;
	NoPrimerPeaksInOneOrMoreNegControlChannel::sMessageScope = scope;
}


int NoPrimerPeaksInOneOrMoreNegControlChannel :: GetMessageIndex () const {

	return NoPrimerPeaksInOneOrMoreNegControlChannel :: sMessageIndex;
}



int NoPrimerPeaksInOneOrMoreNegControlChannel :: GetScope () const {

	return NoPrimerPeaksInOneOrMoreNegControlChannel :: sMessageScope;
}



int NoPrimerPeaksInOneOrMoreNegControlChannel :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NoPrimerPeaksInOneOrMoreNegControlChannel* noticeType = new NoPrimerPeaksInOneOrMoreNegControlChannel;
	warehouse->AddType (noticeType);
	return 1;
}





HeightImbalance :: HeightImbalance () : Notice () {

}


HeightImbalance :: HeightImbalance (const HeightImbalance& note) : Notice ((const Notice&) note) {

}



HeightImbalance :: ~HeightImbalance () {

}



int HeightImbalance :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (HeightImbalance::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		HeightImbalance::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	HeightImbalance::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	HeightImbalance::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		HeightImbalance::sLabel = temp;

	else
		HeightImbalance::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			HeightImbalance::sTriggerLowPriority = true;

		else
			HeightImbalance::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			HeightImbalance::sDoNotCall = true;

		else
			HeightImbalance::sDoNotCall = false;
	}


	int N = HeightImbalance::sMessageList.Entries ();

	if (N == 1)
		N++;

	HeightImbalance::sMessageArray = new RGString* [N];
	RGDListIterator it (HeightImbalance::sMessageList);

	for (int i=0; i<N; i++)
		HeightImbalance::sMessageArray [i] = (RGString*) it ();

	return status;
}


int HeightImbalance :: GetSubject () const {

	return HeightImbalance::sSubject;
}


RGString HeightImbalance :: GetLabel () const {

	return HeightImbalance::sLabel;
}


bool HeightImbalance :: IsTriggerForLowPriority () const {

	return HeightImbalance::sTriggerLowPriority;
}


bool HeightImbalance :: IsDoNotCall () const {

	return HeightImbalance::sDoNotCall;
}



RGString HeightImbalance :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= HeightImbalance::sMessageList.Entries ()))
		return returnValue;

	returnValue = *HeightImbalance::sMessageArray [i];
	return returnValue;
}



int HeightImbalance :: GetMessageLevel () const {

	return HeightImbalance::sMessageLevel;
}



int HeightImbalance :: GetNumberOfMessageParts () const {

	return HeightImbalance::sMessageList.Entries ();
}


int HeightImbalance :: GetSeverityLevel () const {

	return HeightImbalance::sMessageLevel;
}


void HeightImbalance :: SetIndexAndScope (int index, int scope) const {

	HeightImbalance::sMessageIndex = index;
	HeightImbalance::sMessageScope = scope;
}


int HeightImbalance :: GetMessageIndex () const {

	return HeightImbalance :: sMessageIndex;
}



int HeightImbalance :: GetScope () const {

	return HeightImbalance :: sMessageScope;
}



int HeightImbalance :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	HeightImbalance* noticeType = new HeightImbalance;
	warehouse->AddType (noticeType);
	return 1;
}





InterlocusPeakToRightOfLocus :: InterlocusPeakToRightOfLocus () : Notice () {

}


InterlocusPeakToRightOfLocus :: InterlocusPeakToRightOfLocus (const InterlocusPeakToRightOfLocus& note) : Notice ((const Notice&) note) {

}



InterlocusPeakToRightOfLocus :: ~InterlocusPeakToRightOfLocus () {

}



int InterlocusPeakToRightOfLocus :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (InterlocusPeakToRightOfLocus::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		InterlocusPeakToRightOfLocus::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterlocusPeakToRightOfLocus::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterlocusPeakToRightOfLocus::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		InterlocusPeakToRightOfLocus::sLabel = temp;

	else
		InterlocusPeakToRightOfLocus::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterlocusPeakToRightOfLocus::sTriggerLowPriority = true;

		else
			InterlocusPeakToRightOfLocus::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterlocusPeakToRightOfLocus::sDoNotCall = true;

		else
			InterlocusPeakToRightOfLocus::sDoNotCall = false;
	}


	int N = InterlocusPeakToRightOfLocus::sMessageList.Entries ();

	if (N == 1)
		N++;

	InterlocusPeakToRightOfLocus::sMessageArray = new RGString* [N];
	RGDListIterator it (InterlocusPeakToRightOfLocus::sMessageList);

	for (int i=0; i<N; i++)
		InterlocusPeakToRightOfLocus::sMessageArray [i] = (RGString*) it ();

	return status;
}


int InterlocusPeakToRightOfLocus :: GetSubject () const {

	return InterlocusPeakToRightOfLocus::sSubject;
}


RGString InterlocusPeakToRightOfLocus :: GetLabel () const {

	return InterlocusPeakToRightOfLocus::sLabel;
}


bool InterlocusPeakToRightOfLocus :: IsTriggerForLowPriority () const {

	return InterlocusPeakToRightOfLocus::sTriggerLowPriority;
}


bool InterlocusPeakToRightOfLocus :: IsDoNotCall () const {

	return InterlocusPeakToRightOfLocus::sDoNotCall;
}



RGString InterlocusPeakToRightOfLocus :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= InterlocusPeakToRightOfLocus::sMessageList.Entries ()))
		return returnValue;

	returnValue = *InterlocusPeakToRightOfLocus::sMessageArray [i];
	return returnValue;
}



int InterlocusPeakToRightOfLocus :: GetMessageLevel () const {

	return InterlocusPeakToRightOfLocus::sMessageLevel;
}



int InterlocusPeakToRightOfLocus :: GetNumberOfMessageParts () const {

	return InterlocusPeakToRightOfLocus::sMessageList.Entries ();
}


int InterlocusPeakToRightOfLocus :: GetSeverityLevel () const {

	return InterlocusPeakToRightOfLocus::sMessageLevel;
}


void InterlocusPeakToRightOfLocus :: SetIndexAndScope (int index, int scope) const {

	InterlocusPeakToRightOfLocus::sMessageIndex = index;
	InterlocusPeakToRightOfLocus::sMessageScope = scope;
}


int InterlocusPeakToRightOfLocus :: GetMessageIndex () const {

	return InterlocusPeakToRightOfLocus :: sMessageIndex;
}



int InterlocusPeakToRightOfLocus :: GetScope () const {

	return InterlocusPeakToRightOfLocus :: sMessageScope;
}



int InterlocusPeakToRightOfLocus :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	InterlocusPeakToRightOfLocus* noticeType = new InterlocusPeakToRightOfLocus;
	warehouse->AddType (noticeType);
	return 1;
}





InterlocusPeakToLeftOfLocus :: InterlocusPeakToLeftOfLocus () : Notice () {

}


InterlocusPeakToLeftOfLocus :: InterlocusPeakToLeftOfLocus (const InterlocusPeakToLeftOfLocus& note) : Notice ((const Notice&) note) {

}



InterlocusPeakToLeftOfLocus :: ~InterlocusPeakToLeftOfLocus () {

}



int InterlocusPeakToLeftOfLocus :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (InterlocusPeakToLeftOfLocus::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		InterlocusPeakToLeftOfLocus::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterlocusPeakToLeftOfLocus::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterlocusPeakToLeftOfLocus::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		InterlocusPeakToLeftOfLocus::sLabel = temp;

	else
		InterlocusPeakToLeftOfLocus::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterlocusPeakToLeftOfLocus::sTriggerLowPriority = true;

		else
			InterlocusPeakToLeftOfLocus::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterlocusPeakToLeftOfLocus::sDoNotCall = true;

		else
			InterlocusPeakToLeftOfLocus::sDoNotCall = false;
	}


	int N = InterlocusPeakToLeftOfLocus::sMessageList.Entries ();

	if (N == 1)
		N++;

	InterlocusPeakToLeftOfLocus::sMessageArray = new RGString* [N];
	RGDListIterator it (InterlocusPeakToLeftOfLocus::sMessageList);

	for (int i=0; i<N; i++)
		InterlocusPeakToLeftOfLocus::sMessageArray [i] = (RGString*) it ();

	return status;
}


int InterlocusPeakToLeftOfLocus :: GetSubject () const {

	return InterlocusPeakToLeftOfLocus::sSubject;
}


RGString InterlocusPeakToLeftOfLocus :: GetLabel () const {

	return InterlocusPeakToLeftOfLocus::sLabel;
}


bool InterlocusPeakToLeftOfLocus :: IsTriggerForLowPriority () const {

	return InterlocusPeakToLeftOfLocus::sTriggerLowPriority;
}


bool InterlocusPeakToLeftOfLocus :: IsDoNotCall () const {

	return InterlocusPeakToLeftOfLocus::sDoNotCall;
}



RGString InterlocusPeakToLeftOfLocus :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= InterlocusPeakToLeftOfLocus::sMessageList.Entries ()))
		return returnValue;

	returnValue = *InterlocusPeakToLeftOfLocus::sMessageArray [i];
	return returnValue;
}



int InterlocusPeakToLeftOfLocus :: GetMessageLevel () const {

	return InterlocusPeakToLeftOfLocus::sMessageLevel;
}



int InterlocusPeakToLeftOfLocus :: GetNumberOfMessageParts () const {

	return InterlocusPeakToLeftOfLocus::sMessageList.Entries ();
}


int InterlocusPeakToLeftOfLocus :: GetSeverityLevel () const {

	return InterlocusPeakToLeftOfLocus::sMessageLevel;
}


void InterlocusPeakToLeftOfLocus :: SetIndexAndScope (int index, int scope) const {

	InterlocusPeakToLeftOfLocus::sMessageIndex = index;
	InterlocusPeakToLeftOfLocus::sMessageScope = scope;
}


int InterlocusPeakToLeftOfLocus :: GetMessageIndex () const {

	return InterlocusPeakToLeftOfLocus :: sMessageIndex;
}



int InterlocusPeakToLeftOfLocus :: GetScope () const {

	return InterlocusPeakToLeftOfLocus :: sMessageScope;
}



int InterlocusPeakToLeftOfLocus :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	InterlocusPeakToLeftOfLocus* noticeType = new InterlocusPeakToLeftOfLocus;
	warehouse->AddType (noticeType);
	return 1;
}





SignalPeakBelowFractionalFilterHeightPlus :: SignalPeakBelowFractionalFilterHeightPlus () : Notice () {

}


SignalPeakBelowFractionalFilterHeightPlus :: SignalPeakBelowFractionalFilterHeightPlus (const SignalPeakBelowFractionalFilterHeightPlus& note) : Notice ((const Notice&) note) {

}



SignalPeakBelowFractionalFilterHeightPlus :: ~SignalPeakBelowFractionalFilterHeightPlus () {

}



int SignalPeakBelowFractionalFilterHeightPlus :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (SignalPeakBelowFractionalFilterHeightPlus::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		SignalPeakBelowFractionalFilterHeightPlus::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalPeakBelowFractionalFilterHeightPlus::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	SignalPeakBelowFractionalFilterHeightPlus::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		SignalPeakBelowFractionalFilterHeightPlus::sLabel = temp;

	else
		SignalPeakBelowFractionalFilterHeightPlus::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalPeakBelowFractionalFilterHeightPlus::sTriggerLowPriority = true;

		else
			SignalPeakBelowFractionalFilterHeightPlus::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			SignalPeakBelowFractionalFilterHeightPlus::sDoNotCall = true;

		else
			SignalPeakBelowFractionalFilterHeightPlus::sDoNotCall = false;
	}


	int N = SignalPeakBelowFractionalFilterHeightPlus::sMessageList.Entries ();

	if (N == 1)
		N++;

	SignalPeakBelowFractionalFilterHeightPlus::sMessageArray = new RGString* [N];
	RGDListIterator it (SignalPeakBelowFractionalFilterHeightPlus::sMessageList);

	for (int i=0; i<N; i++)
		SignalPeakBelowFractionalFilterHeightPlus::sMessageArray [i] = (RGString*) it ();

	return status;
}


int SignalPeakBelowFractionalFilterHeightPlus :: GetSubject () const {

	return SignalPeakBelowFractionalFilterHeightPlus::sSubject;
}


RGString SignalPeakBelowFractionalFilterHeightPlus :: GetLabel () const {

	return SignalPeakBelowFractionalFilterHeightPlus::sLabel;
}


bool SignalPeakBelowFractionalFilterHeightPlus :: IsTriggerForLowPriority () const {

	return SignalPeakBelowFractionalFilterHeightPlus::sTriggerLowPriority;
}


bool SignalPeakBelowFractionalFilterHeightPlus :: IsDoNotCall () const {

	return SignalPeakBelowFractionalFilterHeightPlus::sDoNotCall;
}



RGString SignalPeakBelowFractionalFilterHeightPlus :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= SignalPeakBelowFractionalFilterHeightPlus::sMessageList.Entries ()))
		return returnValue;

	returnValue = *SignalPeakBelowFractionalFilterHeightPlus::sMessageArray [i];
	return returnValue;
}



int SignalPeakBelowFractionalFilterHeightPlus :: GetMessageLevel () const {

	return SignalPeakBelowFractionalFilterHeightPlus::sMessageLevel;
}



int SignalPeakBelowFractionalFilterHeightPlus :: GetNumberOfMessageParts () const {

	return SignalPeakBelowFractionalFilterHeightPlus::sMessageList.Entries ();
}


int SignalPeakBelowFractionalFilterHeightPlus :: GetSeverityLevel () const {

	return SignalPeakBelowFractionalFilterHeightPlus::sMessageLevel;
}


void SignalPeakBelowFractionalFilterHeightPlus :: SetIndexAndScope (int index, int scope) const {

	SignalPeakBelowFractionalFilterHeightPlus::sMessageIndex = index;
	SignalPeakBelowFractionalFilterHeightPlus::sMessageScope = scope;
}


int SignalPeakBelowFractionalFilterHeightPlus :: GetMessageIndex () const {

	return SignalPeakBelowFractionalFilterHeightPlus :: sMessageIndex;
}



int SignalPeakBelowFractionalFilterHeightPlus :: GetScope () const {

	return SignalPeakBelowFractionalFilterHeightPlus :: sMessageScope;
}



int SignalPeakBelowFractionalFilterHeightPlus :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	SignalPeakBelowFractionalFilterHeightPlus* noticeType = new SignalPeakBelowFractionalFilterHeightPlus;
	warehouse->AddType (noticeType);
	return 1;
}





LadderRelativeHeightsInconsistent :: LadderRelativeHeightsInconsistent () : Notice () {

}


LadderRelativeHeightsInconsistent :: LadderRelativeHeightsInconsistent (const LadderRelativeHeightsInconsistent& note) : Notice ((const Notice&) note) {

}



LadderRelativeHeightsInconsistent :: ~LadderRelativeHeightsInconsistent () {

}



int LadderRelativeHeightsInconsistent :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (LadderRelativeHeightsInconsistent::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		LadderRelativeHeightsInconsistent::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LadderRelativeHeightsInconsistent::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	LadderRelativeHeightsInconsistent::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		LadderRelativeHeightsInconsistent::sLabel = temp;

	else
		LadderRelativeHeightsInconsistent::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LadderRelativeHeightsInconsistent::sTriggerLowPriority = true;

		else
			LadderRelativeHeightsInconsistent::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			LadderRelativeHeightsInconsistent::sDoNotCall = true;

		else
			LadderRelativeHeightsInconsistent::sDoNotCall = false;
	}


	int N = LadderRelativeHeightsInconsistent::sMessageList.Entries ();

	if (N == 1)
		N++;

	LadderRelativeHeightsInconsistent::sMessageArray = new RGString* [N];
	RGDListIterator it (LadderRelativeHeightsInconsistent::sMessageList);

	for (int i=0; i<N; i++)
		LadderRelativeHeightsInconsistent::sMessageArray [i] = (RGString*) it ();

	return status;
}


int LadderRelativeHeightsInconsistent :: GetSubject () const {

	return LadderRelativeHeightsInconsistent::sSubject;
}


RGString LadderRelativeHeightsInconsistent :: GetLabel () const {

	return LadderRelativeHeightsInconsistent::sLabel;
}


bool LadderRelativeHeightsInconsistent :: IsTriggerForLowPriority () const {

	return LadderRelativeHeightsInconsistent::sTriggerLowPriority;
}


bool LadderRelativeHeightsInconsistent :: IsDoNotCall () const {

	return LadderRelativeHeightsInconsistent::sDoNotCall;
}



RGString LadderRelativeHeightsInconsistent :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= LadderRelativeHeightsInconsistent::sMessageList.Entries ()))
		return returnValue;

	returnValue = *LadderRelativeHeightsInconsistent::sMessageArray [i];
	return returnValue;
}



int LadderRelativeHeightsInconsistent :: GetMessageLevel () const {

	return LadderRelativeHeightsInconsistent::sMessageLevel;
}



int LadderRelativeHeightsInconsistent :: GetNumberOfMessageParts () const {

	return LadderRelativeHeightsInconsistent::sMessageList.Entries ();
}


int LadderRelativeHeightsInconsistent :: GetSeverityLevel () const {

	return LadderRelativeHeightsInconsistent::sMessageLevel;
}


void LadderRelativeHeightsInconsistent :: SetIndexAndScope (int index, int scope) const {

	LadderRelativeHeightsInconsistent::sMessageIndex = index;
	LadderRelativeHeightsInconsistent::sMessageScope = scope;
}


int LadderRelativeHeightsInconsistent :: GetMessageIndex () const {

	return LadderRelativeHeightsInconsistent :: sMessageIndex;
}



int LadderRelativeHeightsInconsistent :: GetScope () const {

	return LadderRelativeHeightsInconsistent :: sMessageScope;
}



int LadderRelativeHeightsInconsistent :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	LadderRelativeHeightsInconsistent* noticeType = new LadderRelativeHeightsInconsistent;
	warehouse->AddType (noticeType);
	return 1;
}





AmbiguousInterlocusCraterAnalysis :: AmbiguousInterlocusCraterAnalysis () : Notice () {

}


AmbiguousInterlocusCraterAnalysis :: AmbiguousInterlocusCraterAnalysis (const AmbiguousInterlocusCraterAnalysis& note) : Notice ((const Notice&) note) {

}



AmbiguousInterlocusCraterAnalysis :: ~AmbiguousInterlocusCraterAnalysis () {

}



int AmbiguousInterlocusCraterAnalysis :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (AmbiguousInterlocusCraterAnalysis::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		AmbiguousInterlocusCraterAnalysis::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	AmbiguousInterlocusCraterAnalysis::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	AmbiguousInterlocusCraterAnalysis::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		AmbiguousInterlocusCraterAnalysis::sLabel = temp;

	else
		AmbiguousInterlocusCraterAnalysis::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			AmbiguousInterlocusCraterAnalysis::sTriggerLowPriority = true;

		else
			AmbiguousInterlocusCraterAnalysis::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			AmbiguousInterlocusCraterAnalysis::sDoNotCall = true;

		else
			AmbiguousInterlocusCraterAnalysis::sDoNotCall = false;
	}


	int N = AmbiguousInterlocusCraterAnalysis::sMessageList.Entries ();

	if (N == 1)
		N++;

	AmbiguousInterlocusCraterAnalysis::sMessageArray = new RGString* [N];
	RGDListIterator it (AmbiguousInterlocusCraterAnalysis::sMessageList);

	for (int i=0; i<N; i++)
		AmbiguousInterlocusCraterAnalysis::sMessageArray [i] = (RGString*) it ();

	return status;
}


int AmbiguousInterlocusCraterAnalysis :: GetSubject () const {

	return AmbiguousInterlocusCraterAnalysis::sSubject;
}


RGString AmbiguousInterlocusCraterAnalysis :: GetLabel () const {

	return AmbiguousInterlocusCraterAnalysis::sLabel;
}


bool AmbiguousInterlocusCraterAnalysis :: IsTriggerForLowPriority () const {

	return AmbiguousInterlocusCraterAnalysis::sTriggerLowPriority;
}


bool AmbiguousInterlocusCraterAnalysis :: IsDoNotCall () const {

	return AmbiguousInterlocusCraterAnalysis::sDoNotCall;
}



RGString AmbiguousInterlocusCraterAnalysis :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= AmbiguousInterlocusCraterAnalysis::sMessageList.Entries ()))
		return returnValue;

	returnValue = *AmbiguousInterlocusCraterAnalysis::sMessageArray [i];
	return returnValue;
}



int AmbiguousInterlocusCraterAnalysis :: GetMessageLevel () const {

	return AmbiguousInterlocusCraterAnalysis::sMessageLevel;
}



int AmbiguousInterlocusCraterAnalysis :: GetNumberOfMessageParts () const {

	return AmbiguousInterlocusCraterAnalysis::sMessageList.Entries ();
}


int AmbiguousInterlocusCraterAnalysis :: GetSeverityLevel () const {

	return AmbiguousInterlocusCraterAnalysis::sMessageLevel;
}


void AmbiguousInterlocusCraterAnalysis :: SetIndexAndScope (int index, int scope) const {

	AmbiguousInterlocusCraterAnalysis::sMessageIndex = index;
	AmbiguousInterlocusCraterAnalysis::sMessageScope = scope;
}


int AmbiguousInterlocusCraterAnalysis :: GetMessageIndex () const {

	return AmbiguousInterlocusCraterAnalysis :: sMessageIndex;
}



int AmbiguousInterlocusCraterAnalysis :: GetScope () const {

	return AmbiguousInterlocusCraterAnalysis :: sMessageScope;
}



int AmbiguousInterlocusCraterAnalysis :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	AmbiguousInterlocusCraterAnalysis* noticeType = new AmbiguousInterlocusCraterAnalysis;
	warehouse->AddType (noticeType);
	return 1;
}





PeakBelowPullupFractionalFilter :: PeakBelowPullupFractionalFilter () : Notice () {

}


PeakBelowPullupFractionalFilter :: PeakBelowPullupFractionalFilter (const PeakBelowPullupFractionalFilter& note) : Notice ((const Notice&) note) {

}



PeakBelowPullupFractionalFilter :: ~PeakBelowPullupFractionalFilter () {

}



int PeakBelowPullupFractionalFilter :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakBelowPullupFractionalFilter::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakBelowPullupFractionalFilter::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowPullupFractionalFilter::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowPullupFractionalFilter::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakBelowPullupFractionalFilter::sLabel = temp;

	else
		PeakBelowPullupFractionalFilter::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowPullupFractionalFilter::sTriggerLowPriority = true;

		else
			PeakBelowPullupFractionalFilter::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowPullupFractionalFilter::sDoNotCall = true;

		else
			PeakBelowPullupFractionalFilter::sDoNotCall = false;
	}


	int N = PeakBelowPullupFractionalFilter::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakBelowPullupFractionalFilter::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakBelowPullupFractionalFilter::sMessageList);

	for (int i=0; i<N; i++)
		PeakBelowPullupFractionalFilter::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakBelowPullupFractionalFilter :: GetSubject () const {

	return PeakBelowPullupFractionalFilter::sSubject;
}


RGString PeakBelowPullupFractionalFilter :: GetLabel () const {

	return PeakBelowPullupFractionalFilter::sLabel;
}


bool PeakBelowPullupFractionalFilter :: IsTriggerForLowPriority () const {

	return PeakBelowPullupFractionalFilter::sTriggerLowPriority;
}


bool PeakBelowPullupFractionalFilter :: IsDoNotCall () const {

	return PeakBelowPullupFractionalFilter::sDoNotCall;
}



RGString PeakBelowPullupFractionalFilter :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakBelowPullupFractionalFilter::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakBelowPullupFractionalFilter::sMessageArray [i];
	return returnValue;
}



int PeakBelowPullupFractionalFilter :: GetMessageLevel () const {

	return PeakBelowPullupFractionalFilter::sMessageLevel;
}



int PeakBelowPullupFractionalFilter :: GetNumberOfMessageParts () const {

	return PeakBelowPullupFractionalFilter::sMessageList.Entries ();
}


int PeakBelowPullupFractionalFilter :: GetSeverityLevel () const {

	return PeakBelowPullupFractionalFilter::sMessageLevel;
}


void PeakBelowPullupFractionalFilter :: SetIndexAndScope (int index, int scope) const {

	PeakBelowPullupFractionalFilter::sMessageIndex = index;
	PeakBelowPullupFractionalFilter::sMessageScope = scope;
}


int PeakBelowPullupFractionalFilter :: GetMessageIndex () const {

	return PeakBelowPullupFractionalFilter :: sMessageIndex;
}



int PeakBelowPullupFractionalFilter :: GetScope () const {

	return PeakBelowPullupFractionalFilter :: sMessageScope;
}



int PeakBelowPullupFractionalFilter :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakBelowPullupFractionalFilter* noticeType = new PeakBelowPullupFractionalFilter;
	warehouse->AddType (noticeType);
	return 1;
}





PeakBelowPullupFractionalFilterLeft :: PeakBelowPullupFractionalFilterLeft () : Notice () {

}


PeakBelowPullupFractionalFilterLeft :: PeakBelowPullupFractionalFilterLeft (const PeakBelowPullupFractionalFilterLeft& note) : Notice ((const Notice&) note) {

}



PeakBelowPullupFractionalFilterLeft :: ~PeakBelowPullupFractionalFilterLeft () {

}



int PeakBelowPullupFractionalFilterLeft :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakBelowPullupFractionalFilterLeft::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakBelowPullupFractionalFilterLeft::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowPullupFractionalFilterLeft::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowPullupFractionalFilterLeft::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakBelowPullupFractionalFilterLeft::sLabel = temp;

	else
		PeakBelowPullupFractionalFilterLeft::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowPullupFractionalFilterLeft::sTriggerLowPriority = true;

		else
			PeakBelowPullupFractionalFilterLeft::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowPullupFractionalFilterLeft::sDoNotCall = true;

		else
			PeakBelowPullupFractionalFilterLeft::sDoNotCall = false;
	}


	int N = PeakBelowPullupFractionalFilterLeft::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakBelowPullupFractionalFilterLeft::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakBelowPullupFractionalFilterLeft::sMessageList);

	for (int i=0; i<N; i++)
		PeakBelowPullupFractionalFilterLeft::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakBelowPullupFractionalFilterLeft :: GetSubject () const {

	return PeakBelowPullupFractionalFilterLeft::sSubject;
}


RGString PeakBelowPullupFractionalFilterLeft :: GetLabel () const {

	return PeakBelowPullupFractionalFilterLeft::sLabel;
}


bool PeakBelowPullupFractionalFilterLeft :: IsTriggerForLowPriority () const {

	return PeakBelowPullupFractionalFilterLeft::sTriggerLowPriority;
}


bool PeakBelowPullupFractionalFilterLeft :: IsDoNotCall () const {

	return PeakBelowPullupFractionalFilterLeft::sDoNotCall;
}



RGString PeakBelowPullupFractionalFilterLeft :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakBelowPullupFractionalFilterLeft::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakBelowPullupFractionalFilterLeft::sMessageArray [i];
	return returnValue;
}



int PeakBelowPullupFractionalFilterLeft :: GetMessageLevel () const {

	return PeakBelowPullupFractionalFilterLeft::sMessageLevel;
}



int PeakBelowPullupFractionalFilterLeft :: GetNumberOfMessageParts () const {

	return PeakBelowPullupFractionalFilterLeft::sMessageList.Entries ();
}


int PeakBelowPullupFractionalFilterLeft :: GetSeverityLevel () const {

	return PeakBelowPullupFractionalFilterLeft::sMessageLevel;
}


void PeakBelowPullupFractionalFilterLeft :: SetIndexAndScope (int index, int scope) const {

	PeakBelowPullupFractionalFilterLeft::sMessageIndex = index;
	PeakBelowPullupFractionalFilterLeft::sMessageScope = scope;
}


int PeakBelowPullupFractionalFilterLeft :: GetMessageIndex () const {

	return PeakBelowPullupFractionalFilterLeft :: sMessageIndex;
}



int PeakBelowPullupFractionalFilterLeft :: GetScope () const {

	return PeakBelowPullupFractionalFilterLeft :: sMessageScope;
}



int PeakBelowPullupFractionalFilterLeft :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakBelowPullupFractionalFilterLeft* noticeType = new PeakBelowPullupFractionalFilterLeft;
	warehouse->AddType (noticeType);
	return 1;
}





PeakBelowPullupFractionalFilterRight :: PeakBelowPullupFractionalFilterRight () : Notice () {

}


PeakBelowPullupFractionalFilterRight :: PeakBelowPullupFractionalFilterRight (const PeakBelowPullupFractionalFilterRight& note) : Notice ((const Notice&) note) {

}



PeakBelowPullupFractionalFilterRight :: ~PeakBelowPullupFractionalFilterRight () {

}



int PeakBelowPullupFractionalFilterRight :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakBelowPullupFractionalFilterRight::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakBelowPullupFractionalFilterRight::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowPullupFractionalFilterRight::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowPullupFractionalFilterRight::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakBelowPullupFractionalFilterRight::sLabel = temp;

	else
		PeakBelowPullupFractionalFilterRight::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowPullupFractionalFilterRight::sTriggerLowPriority = true;

		else
			PeakBelowPullupFractionalFilterRight::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowPullupFractionalFilterRight::sDoNotCall = true;

		else
			PeakBelowPullupFractionalFilterRight::sDoNotCall = false;
	}


	int N = PeakBelowPullupFractionalFilterRight::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakBelowPullupFractionalFilterRight::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakBelowPullupFractionalFilterRight::sMessageList);

	for (int i=0; i<N; i++)
		PeakBelowPullupFractionalFilterRight::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakBelowPullupFractionalFilterRight :: GetSubject () const {

	return PeakBelowPullupFractionalFilterRight::sSubject;
}


RGString PeakBelowPullupFractionalFilterRight :: GetLabel () const {

	return PeakBelowPullupFractionalFilterRight::sLabel;
}


bool PeakBelowPullupFractionalFilterRight :: IsTriggerForLowPriority () const {

	return PeakBelowPullupFractionalFilterRight::sTriggerLowPriority;
}


bool PeakBelowPullupFractionalFilterRight :: IsDoNotCall () const {

	return PeakBelowPullupFractionalFilterRight::sDoNotCall;
}



RGString PeakBelowPullupFractionalFilterRight :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakBelowPullupFractionalFilterRight::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakBelowPullupFractionalFilterRight::sMessageArray [i];
	return returnValue;
}



int PeakBelowPullupFractionalFilterRight :: GetMessageLevel () const {

	return PeakBelowPullupFractionalFilterRight::sMessageLevel;
}



int PeakBelowPullupFractionalFilterRight :: GetNumberOfMessageParts () const {

	return PeakBelowPullupFractionalFilterRight::sMessageList.Entries ();
}


int PeakBelowPullupFractionalFilterRight :: GetSeverityLevel () const {

	return PeakBelowPullupFractionalFilterRight::sMessageLevel;
}


void PeakBelowPullupFractionalFilterRight :: SetIndexAndScope (int index, int scope) const {

	PeakBelowPullupFractionalFilterRight::sMessageIndex = index;
	PeakBelowPullupFractionalFilterRight::sMessageScope = scope;
}


int PeakBelowPullupFractionalFilterRight :: GetMessageIndex () const {

	return PeakBelowPullupFractionalFilterRight :: sMessageIndex;
}



int PeakBelowPullupFractionalFilterRight :: GetScope () const {

	return PeakBelowPullupFractionalFilterRight :: sMessageScope;
}



int PeakBelowPullupFractionalFilterRight :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakBelowPullupFractionalFilterRight* noticeType = new PeakBelowPullupFractionalFilterRight;
	warehouse->AddType (noticeType);
	return 1;
}





PeakBelowFractionalFilterLeft :: PeakBelowFractionalFilterLeft () : Notice () {

}


PeakBelowFractionalFilterLeft :: PeakBelowFractionalFilterLeft (const PeakBelowFractionalFilterLeft& note) : Notice ((const Notice&) note) {

}



PeakBelowFractionalFilterLeft :: ~PeakBelowFractionalFilterLeft () {

}



int PeakBelowFractionalFilterLeft :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakBelowFractionalFilterLeft::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakBelowFractionalFilterLeft::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowFractionalFilterLeft::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowFractionalFilterLeft::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakBelowFractionalFilterLeft::sLabel = temp;

	else
		PeakBelowFractionalFilterLeft::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowFractionalFilterLeft::sTriggerLowPriority = true;

		else
			PeakBelowFractionalFilterLeft::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowFractionalFilterLeft::sDoNotCall = true;

		else
			PeakBelowFractionalFilterLeft::sDoNotCall = false;
	}


	int N = PeakBelowFractionalFilterLeft::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakBelowFractionalFilterLeft::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakBelowFractionalFilterLeft::sMessageList);

	for (int i=0; i<N; i++)
		PeakBelowFractionalFilterLeft::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakBelowFractionalFilterLeft :: GetSubject () const {

	return PeakBelowFractionalFilterLeft::sSubject;
}


RGString PeakBelowFractionalFilterLeft :: GetLabel () const {

	return PeakBelowFractionalFilterLeft::sLabel;
}


bool PeakBelowFractionalFilterLeft :: IsTriggerForLowPriority () const {

	return PeakBelowFractionalFilterLeft::sTriggerLowPriority;
}


bool PeakBelowFractionalFilterLeft :: IsDoNotCall () const {

	return PeakBelowFractionalFilterLeft::sDoNotCall;
}



RGString PeakBelowFractionalFilterLeft :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakBelowFractionalFilterLeft::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakBelowFractionalFilterLeft::sMessageArray [i];
	return returnValue;
}



int PeakBelowFractionalFilterLeft :: GetMessageLevel () const {

	return PeakBelowFractionalFilterLeft::sMessageLevel;
}



int PeakBelowFractionalFilterLeft :: GetNumberOfMessageParts () const {

	return PeakBelowFractionalFilterLeft::sMessageList.Entries ();
}


int PeakBelowFractionalFilterLeft :: GetSeverityLevel () const {

	return PeakBelowFractionalFilterLeft::sMessageLevel;
}


void PeakBelowFractionalFilterLeft :: SetIndexAndScope (int index, int scope) const {

	PeakBelowFractionalFilterLeft::sMessageIndex = index;
	PeakBelowFractionalFilterLeft::sMessageScope = scope;
}


int PeakBelowFractionalFilterLeft :: GetMessageIndex () const {

	return PeakBelowFractionalFilterLeft :: sMessageIndex;
}



int PeakBelowFractionalFilterLeft :: GetScope () const {

	return PeakBelowFractionalFilterLeft :: sMessageScope;
}



int PeakBelowFractionalFilterLeft :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakBelowFractionalFilterLeft* noticeType = new PeakBelowFractionalFilterLeft;
	warehouse->AddType (noticeType);
	return 1;
}





PeakBelowFractionalFilterRight :: PeakBelowFractionalFilterRight () : Notice () {

}


PeakBelowFractionalFilterRight :: PeakBelowFractionalFilterRight (const PeakBelowFractionalFilterRight& note) : Notice ((const Notice&) note) {

}



PeakBelowFractionalFilterRight :: ~PeakBelowFractionalFilterRight () {

}



int PeakBelowFractionalFilterRight :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PeakBelowFractionalFilterRight::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PeakBelowFractionalFilterRight::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowFractionalFilterRight::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PeakBelowFractionalFilterRight::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PeakBelowFractionalFilterRight::sLabel = temp;

	else
		PeakBelowFractionalFilterRight::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowFractionalFilterRight::sTriggerLowPriority = true;

		else
			PeakBelowFractionalFilterRight::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PeakBelowFractionalFilterRight::sDoNotCall = true;

		else
			PeakBelowFractionalFilterRight::sDoNotCall = false;
	}


	int N = PeakBelowFractionalFilterRight::sMessageList.Entries ();

	if (N == 1)
		N++;

	PeakBelowFractionalFilterRight::sMessageArray = new RGString* [N];
	RGDListIterator it (PeakBelowFractionalFilterRight::sMessageList);

	for (int i=0; i<N; i++)
		PeakBelowFractionalFilterRight::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PeakBelowFractionalFilterRight :: GetSubject () const {

	return PeakBelowFractionalFilterRight::sSubject;
}


RGString PeakBelowFractionalFilterRight :: GetLabel () const {

	return PeakBelowFractionalFilterRight::sLabel;
}


bool PeakBelowFractionalFilterRight :: IsTriggerForLowPriority () const {

	return PeakBelowFractionalFilterRight::sTriggerLowPriority;
}


bool PeakBelowFractionalFilterRight :: IsDoNotCall () const {

	return PeakBelowFractionalFilterRight::sDoNotCall;
}



RGString PeakBelowFractionalFilterRight :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PeakBelowFractionalFilterRight::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PeakBelowFractionalFilterRight::sMessageArray [i];
	return returnValue;
}



int PeakBelowFractionalFilterRight :: GetMessageLevel () const {

	return PeakBelowFractionalFilterRight::sMessageLevel;
}



int PeakBelowFractionalFilterRight :: GetNumberOfMessageParts () const {

	return PeakBelowFractionalFilterRight::sMessageList.Entries ();
}


int PeakBelowFractionalFilterRight :: GetSeverityLevel () const {

	return PeakBelowFractionalFilterRight::sMessageLevel;
}


void PeakBelowFractionalFilterRight :: SetIndexAndScope (int index, int scope) const {

	PeakBelowFractionalFilterRight::sMessageIndex = index;
	PeakBelowFractionalFilterRight::sMessageScope = scope;
}


int PeakBelowFractionalFilterRight :: GetMessageIndex () const {

	return PeakBelowFractionalFilterRight :: sMessageIndex;
}



int PeakBelowFractionalFilterRight :: GetScope () const {

	return PeakBelowFractionalFilterRight :: sMessageScope;
}



int PeakBelowFractionalFilterRight :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PeakBelowFractionalFilterRight* noticeType = new PeakBelowFractionalFilterRight;
	warehouse->AddType (noticeType);
	return 1;
}





PossibleAmbiguousInterlocusPeakAssignedToLocus :: PossibleAmbiguousInterlocusPeakAssignedToLocus () : Notice () {

}


PossibleAmbiguousInterlocusPeakAssignedToLocus :: PossibleAmbiguousInterlocusPeakAssignedToLocus (const PossibleAmbiguousInterlocusPeakAssignedToLocus& note) : Notice ((const Notice&) note) {

}



PossibleAmbiguousInterlocusPeakAssignedToLocus :: ~PossibleAmbiguousInterlocusPeakAssignedToLocus () {

}



int PossibleAmbiguousInterlocusPeakAssignedToLocus :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PossibleAmbiguousInterlocusPeakAssignedToLocus::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		PossibleAmbiguousInterlocusPeakAssignedToLocus::sLabel = temp;

	else
		PossibleAmbiguousInterlocusPeakAssignedToLocus::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PossibleAmbiguousInterlocusPeakAssignedToLocus::sTriggerLowPriority = true;

		else
			PossibleAmbiguousInterlocusPeakAssignedToLocus::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			PossibleAmbiguousInterlocusPeakAssignedToLocus::sDoNotCall = true;

		else
			PossibleAmbiguousInterlocusPeakAssignedToLocus::sDoNotCall = false;
	}


	int N = PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageList.Entries ();

	if (N == 1)
		N++;

	PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageArray = new RGString* [N];
	RGDListIterator it (PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageList);

	for (int i=0; i<N; i++)
		PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageArray [i] = (RGString*) it ();

	return status;
}


int PossibleAmbiguousInterlocusPeakAssignedToLocus :: GetSubject () const {

	return PossibleAmbiguousInterlocusPeakAssignedToLocus::sSubject;
}


RGString PossibleAmbiguousInterlocusPeakAssignedToLocus :: GetLabel () const {

	return PossibleAmbiguousInterlocusPeakAssignedToLocus::sLabel;
}


bool PossibleAmbiguousInterlocusPeakAssignedToLocus :: IsTriggerForLowPriority () const {

	return PossibleAmbiguousInterlocusPeakAssignedToLocus::sTriggerLowPriority;
}


bool PossibleAmbiguousInterlocusPeakAssignedToLocus :: IsDoNotCall () const {

	return PossibleAmbiguousInterlocusPeakAssignedToLocus::sDoNotCall;
}



RGString PossibleAmbiguousInterlocusPeakAssignedToLocus :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageList.Entries ()))
		return returnValue;

	returnValue = *PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageArray [i];
	return returnValue;
}



int PossibleAmbiguousInterlocusPeakAssignedToLocus :: GetMessageLevel () const {

	return PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageLevel;
}



int PossibleAmbiguousInterlocusPeakAssignedToLocus :: GetNumberOfMessageParts () const {

	return PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageList.Entries ();
}


int PossibleAmbiguousInterlocusPeakAssignedToLocus :: GetSeverityLevel () const {

	return PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageLevel;
}


void PossibleAmbiguousInterlocusPeakAssignedToLocus :: SetIndexAndScope (int index, int scope) const {

	PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageIndex = index;
	PossibleAmbiguousInterlocusPeakAssignedToLocus::sMessageScope = scope;
}


int PossibleAmbiguousInterlocusPeakAssignedToLocus :: GetMessageIndex () const {

	return PossibleAmbiguousInterlocusPeakAssignedToLocus :: sMessageIndex;
}



int PossibleAmbiguousInterlocusPeakAssignedToLocus :: GetScope () const {

	return PossibleAmbiguousInterlocusPeakAssignedToLocus :: sMessageScope;
}



int PossibleAmbiguousInterlocusPeakAssignedToLocus :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	PossibleAmbiguousInterlocusPeakAssignedToLocus* noticeType = new PossibleAmbiguousInterlocusPeakAssignedToLocus;
	warehouse->AddType (noticeType);
	return 1;
}





AmbiguousInterlocusPeak :: AmbiguousInterlocusPeak () : Notice () {

}


AmbiguousInterlocusPeak :: AmbiguousInterlocusPeak (const AmbiguousInterlocusPeak& note) : Notice ((const Notice&) note) {

}



AmbiguousInterlocusPeak :: ~AmbiguousInterlocusPeak () {

}



int AmbiguousInterlocusPeak :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (AmbiguousInterlocusPeak::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		AmbiguousInterlocusPeak::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	AmbiguousInterlocusPeak::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	AmbiguousInterlocusPeak::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		AmbiguousInterlocusPeak::sLabel = temp;

	else
		AmbiguousInterlocusPeak::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			AmbiguousInterlocusPeak::sTriggerLowPriority = true;

		else
			AmbiguousInterlocusPeak::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			AmbiguousInterlocusPeak::sDoNotCall = true;

		else
			AmbiguousInterlocusPeak::sDoNotCall = false;
	}


	int N = AmbiguousInterlocusPeak::sMessageList.Entries ();

	if (N == 1)
		N++;

	AmbiguousInterlocusPeak::sMessageArray = new RGString* [N];
	RGDListIterator it (AmbiguousInterlocusPeak::sMessageList);

	for (int i=0; i<N; i++)
		AmbiguousInterlocusPeak::sMessageArray [i] = (RGString*) it ();

	return status;
}


int AmbiguousInterlocusPeak :: GetSubject () const {

	return AmbiguousInterlocusPeak::sSubject;
}


RGString AmbiguousInterlocusPeak :: GetLabel () const {

	return AmbiguousInterlocusPeak::sLabel;
}


bool AmbiguousInterlocusPeak :: IsTriggerForLowPriority () const {

	return AmbiguousInterlocusPeak::sTriggerLowPriority;
}


bool AmbiguousInterlocusPeak :: IsDoNotCall () const {

	return AmbiguousInterlocusPeak::sDoNotCall;
}



RGString AmbiguousInterlocusPeak :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= AmbiguousInterlocusPeak::sMessageList.Entries ()))
		return returnValue;

	returnValue = *AmbiguousInterlocusPeak::sMessageArray [i];
	return returnValue;
}



int AmbiguousInterlocusPeak :: GetMessageLevel () const {

	return AmbiguousInterlocusPeak::sMessageLevel;
}



int AmbiguousInterlocusPeak :: GetNumberOfMessageParts () const {

	return AmbiguousInterlocusPeak::sMessageList.Entries ();
}


int AmbiguousInterlocusPeak :: GetSeverityLevel () const {

	return AmbiguousInterlocusPeak::sMessageLevel;
}


void AmbiguousInterlocusPeak :: SetIndexAndScope (int index, int scope) const {

	AmbiguousInterlocusPeak::sMessageIndex = index;
	AmbiguousInterlocusPeak::sMessageScope = scope;
}


int AmbiguousInterlocusPeak :: GetMessageIndex () const {

	return AmbiguousInterlocusPeak :: sMessageIndex;
}



int AmbiguousInterlocusPeak :: GetScope () const {

	return AmbiguousInterlocusPeak :: sMessageScope;
}



int AmbiguousInterlocusPeak :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	AmbiguousInterlocusPeak* noticeType = new AmbiguousInterlocusPeak;
	warehouse->AddType (noticeType);
	return 1;
}





ExtraneousPeakInNegativeControl :: ExtraneousPeakInNegativeControl () : Notice () {

}


ExtraneousPeakInNegativeControl :: ExtraneousPeakInNegativeControl (const ExtraneousPeakInNegativeControl& note) : Notice ((const Notice&) note) {

}



ExtraneousPeakInNegativeControl :: ~ExtraneousPeakInNegativeControl () {

}



int ExtraneousPeakInNegativeControl :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ExtraneousPeakInNegativeControl::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ExtraneousPeakInNegativeControl::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ExtraneousPeakInNegativeControl::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ExtraneousPeakInNegativeControl::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ExtraneousPeakInNegativeControl::sLabel = temp;

	else
		ExtraneousPeakInNegativeControl::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ExtraneousPeakInNegativeControl::sTriggerLowPriority = true;

		else
			ExtraneousPeakInNegativeControl::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ExtraneousPeakInNegativeControl::sDoNotCall = true;

		else
			ExtraneousPeakInNegativeControl::sDoNotCall = false;
	}


	int N = ExtraneousPeakInNegativeControl::sMessageList.Entries ();

	if (N == 1)
		N++;

	ExtraneousPeakInNegativeControl::sMessageArray = new RGString* [N];
	RGDListIterator it (ExtraneousPeakInNegativeControl::sMessageList);

	for (int i=0; i<N; i++)
		ExtraneousPeakInNegativeControl::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ExtraneousPeakInNegativeControl :: GetSubject () const {

	return ExtraneousPeakInNegativeControl::sSubject;
}


RGString ExtraneousPeakInNegativeControl :: GetLabel () const {

	return ExtraneousPeakInNegativeControl::sLabel;
}


bool ExtraneousPeakInNegativeControl :: IsTriggerForLowPriority () const {

	return ExtraneousPeakInNegativeControl::sTriggerLowPriority;
}


bool ExtraneousPeakInNegativeControl :: IsDoNotCall () const {

	return ExtraneousPeakInNegativeControl::sDoNotCall;
}



RGString ExtraneousPeakInNegativeControl :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ExtraneousPeakInNegativeControl::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ExtraneousPeakInNegativeControl::sMessageArray [i];
	return returnValue;
}



int ExtraneousPeakInNegativeControl :: GetMessageLevel () const {

	return ExtraneousPeakInNegativeControl::sMessageLevel;
}



int ExtraneousPeakInNegativeControl :: GetNumberOfMessageParts () const {

	return ExtraneousPeakInNegativeControl::sMessageList.Entries ();
}


int ExtraneousPeakInNegativeControl :: GetSeverityLevel () const {

	return ExtraneousPeakInNegativeControl::sMessageLevel;
}


void ExtraneousPeakInNegativeControl :: SetIndexAndScope (int index, int scope) const {

	ExtraneousPeakInNegativeControl::sMessageIndex = index;
	ExtraneousPeakInNegativeControl::sMessageScope = scope;
}


int ExtraneousPeakInNegativeControl :: GetMessageIndex () const {

	return ExtraneousPeakInNegativeControl :: sMessageIndex;
}



int ExtraneousPeakInNegativeControl :: GetScope () const {

	return ExtraneousPeakInNegativeControl :: sMessageScope;
}



int ExtraneousPeakInNegativeControl :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ExtraneousPeakInNegativeControl* noticeType = new ExtraneousPeakInNegativeControl;
	warehouse->AddType (noticeType);
	return 1;
}





ExtraneousPeakInLadderLocus :: ExtraneousPeakInLadderLocus () : Notice () {

}


ExtraneousPeakInLadderLocus :: ExtraneousPeakInLadderLocus (const ExtraneousPeakInLadderLocus& note) : Notice ((const Notice&) note) {

}



ExtraneousPeakInLadderLocus :: ~ExtraneousPeakInLadderLocus () {

}



int ExtraneousPeakInLadderLocus :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (ExtraneousPeakInLadderLocus::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		ExtraneousPeakInLadderLocus::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ExtraneousPeakInLadderLocus::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	ExtraneousPeakInLadderLocus::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		ExtraneousPeakInLadderLocus::sLabel = temp;

	else
		ExtraneousPeakInLadderLocus::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ExtraneousPeakInLadderLocus::sTriggerLowPriority = true;

		else
			ExtraneousPeakInLadderLocus::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			ExtraneousPeakInLadderLocus::sDoNotCall = true;

		else
			ExtraneousPeakInLadderLocus::sDoNotCall = false;
	}


	int N = ExtraneousPeakInLadderLocus::sMessageList.Entries ();

	if (N == 1)
		N++;

	ExtraneousPeakInLadderLocus::sMessageArray = new RGString* [N];
	RGDListIterator it (ExtraneousPeakInLadderLocus::sMessageList);

	for (int i=0; i<N; i++)
		ExtraneousPeakInLadderLocus::sMessageArray [i] = (RGString*) it ();

	return status;
}


int ExtraneousPeakInLadderLocus :: GetSubject () const {

	return ExtraneousPeakInLadderLocus::sSubject;
}


RGString ExtraneousPeakInLadderLocus :: GetLabel () const {

	return ExtraneousPeakInLadderLocus::sLabel;
}


bool ExtraneousPeakInLadderLocus :: IsTriggerForLowPriority () const {

	return ExtraneousPeakInLadderLocus::sTriggerLowPriority;
}


bool ExtraneousPeakInLadderLocus :: IsDoNotCall () const {

	return ExtraneousPeakInLadderLocus::sDoNotCall;
}



RGString ExtraneousPeakInLadderLocus :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= ExtraneousPeakInLadderLocus::sMessageList.Entries ()))
		return returnValue;

	returnValue = *ExtraneousPeakInLadderLocus::sMessageArray [i];
	return returnValue;
}



int ExtraneousPeakInLadderLocus :: GetMessageLevel () const {

	return ExtraneousPeakInLadderLocus::sMessageLevel;
}



int ExtraneousPeakInLadderLocus :: GetNumberOfMessageParts () const {

	return ExtraneousPeakInLadderLocus::sMessageList.Entries ();
}


int ExtraneousPeakInLadderLocus :: GetSeverityLevel () const {

	return ExtraneousPeakInLadderLocus::sMessageLevel;
}


void ExtraneousPeakInLadderLocus :: SetIndexAndScope (int index, int scope) const {

	ExtraneousPeakInLadderLocus::sMessageIndex = index;
	ExtraneousPeakInLadderLocus::sMessageScope = scope;
}


int ExtraneousPeakInLadderLocus :: GetMessageIndex () const {

	return ExtraneousPeakInLadderLocus :: sMessageIndex;
}



int ExtraneousPeakInLadderLocus :: GetScope () const {

	return ExtraneousPeakInLadderLocus :: sMessageScope;
}



int ExtraneousPeakInLadderLocus :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	ExtraneousPeakInLadderLocus* noticeType = new ExtraneousPeakInLadderLocus;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfExcessiveResidualsExceedsThreshold :: NumberOfExcessiveResidualsExceedsThreshold () : Notice () {

}


NumberOfExcessiveResidualsExceedsThreshold :: NumberOfExcessiveResidualsExceedsThreshold (const NumberOfExcessiveResidualsExceedsThreshold& note) : Notice ((const Notice&) note) {

}



NumberOfExcessiveResidualsExceedsThreshold :: ~NumberOfExcessiveResidualsExceedsThreshold () {

}



int NumberOfExcessiveResidualsExceedsThreshold :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfExcessiveResidualsExceedsThreshold::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfExcessiveResidualsExceedsThreshold::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfExcessiveResidualsExceedsThreshold::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfExcessiveResidualsExceedsThreshold::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfExcessiveResidualsExceedsThreshold::sLabel = temp;

	else
		NumberOfExcessiveResidualsExceedsThreshold::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfExcessiveResidualsExceedsThreshold::sTriggerLowPriority = true;

		else
			NumberOfExcessiveResidualsExceedsThreshold::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfExcessiveResidualsExceedsThreshold::sDoNotCall = true;

		else
			NumberOfExcessiveResidualsExceedsThreshold::sDoNotCall = false;
	}


	int N = NumberOfExcessiveResidualsExceedsThreshold::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfExcessiveResidualsExceedsThreshold::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfExcessiveResidualsExceedsThreshold::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfExcessiveResidualsExceedsThreshold::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfExcessiveResidualsExceedsThreshold :: GetSubject () const {

	return NumberOfExcessiveResidualsExceedsThreshold::sSubject;
}


RGString NumberOfExcessiveResidualsExceedsThreshold :: GetLabel () const {

	return NumberOfExcessiveResidualsExceedsThreshold::sLabel;
}


bool NumberOfExcessiveResidualsExceedsThreshold :: IsTriggerForLowPriority () const {

	return NumberOfExcessiveResidualsExceedsThreshold::sTriggerLowPriority;
}


bool NumberOfExcessiveResidualsExceedsThreshold :: IsDoNotCall () const {

	return NumberOfExcessiveResidualsExceedsThreshold::sDoNotCall;
}



RGString NumberOfExcessiveResidualsExceedsThreshold :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfExcessiveResidualsExceedsThreshold::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfExcessiveResidualsExceedsThreshold::sMessageArray [i];
	return returnValue;
}



int NumberOfExcessiveResidualsExceedsThreshold :: GetMessageLevel () const {

	return NumberOfExcessiveResidualsExceedsThreshold::sMessageLevel;
}



int NumberOfExcessiveResidualsExceedsThreshold :: GetNumberOfMessageParts () const {

	return NumberOfExcessiveResidualsExceedsThreshold::sMessageList.Entries ();
}


int NumberOfExcessiveResidualsExceedsThreshold :: GetSeverityLevel () const {

	return NumberOfExcessiveResidualsExceedsThreshold::sMessageLevel;
}


void NumberOfExcessiveResidualsExceedsThreshold :: SetIndexAndScope (int index, int scope) const {

	NumberOfExcessiveResidualsExceedsThreshold::sMessageIndex = index;
	NumberOfExcessiveResidualsExceedsThreshold::sMessageScope = scope;
}


int NumberOfExcessiveResidualsExceedsThreshold :: GetMessageIndex () const {

	return NumberOfExcessiveResidualsExceedsThreshold :: sMessageIndex;
}



int NumberOfExcessiveResidualsExceedsThreshold :: GetScope () const {

	return NumberOfExcessiveResidualsExceedsThreshold :: sMessageScope;
}



int NumberOfExcessiveResidualsExceedsThreshold :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfExcessiveResidualsExceedsThreshold* noticeType = new NumberOfExcessiveResidualsExceedsThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





NumberOfExcessiveResidualsInSample :: NumberOfExcessiveResidualsInSample () : Notice () {

}


NumberOfExcessiveResidualsInSample :: NumberOfExcessiveResidualsInSample (const NumberOfExcessiveResidualsInSample& note) : Notice ((const Notice&) note) {

}



NumberOfExcessiveResidualsInSample :: ~NumberOfExcessiveResidualsInSample () {

}



int NumberOfExcessiveResidualsInSample :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (NumberOfExcessiveResidualsInSample::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		NumberOfExcessiveResidualsInSample::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfExcessiveResidualsInSample::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	NumberOfExcessiveResidualsInSample::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		NumberOfExcessiveResidualsInSample::sLabel = temp;

	else
		NumberOfExcessiveResidualsInSample::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfExcessiveResidualsInSample::sTriggerLowPriority = true;

		else
			NumberOfExcessiveResidualsInSample::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			NumberOfExcessiveResidualsInSample::sDoNotCall = true;

		else
			NumberOfExcessiveResidualsInSample::sDoNotCall = false;
	}


	int N = NumberOfExcessiveResidualsInSample::sMessageList.Entries ();

	if (N == 1)
		N++;

	NumberOfExcessiveResidualsInSample::sMessageArray = new RGString* [N];
	RGDListIterator it (NumberOfExcessiveResidualsInSample::sMessageList);

	for (int i=0; i<N; i++)
		NumberOfExcessiveResidualsInSample::sMessageArray [i] = (RGString*) it ();

	return status;
}


int NumberOfExcessiveResidualsInSample :: GetSubject () const {

	return NumberOfExcessiveResidualsInSample::sSubject;
}


RGString NumberOfExcessiveResidualsInSample :: GetLabel () const {

	return NumberOfExcessiveResidualsInSample::sLabel;
}


bool NumberOfExcessiveResidualsInSample :: IsTriggerForLowPriority () const {

	return NumberOfExcessiveResidualsInSample::sTriggerLowPriority;
}


bool NumberOfExcessiveResidualsInSample :: IsDoNotCall () const {

	return NumberOfExcessiveResidualsInSample::sDoNotCall;
}



RGString NumberOfExcessiveResidualsInSample :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= NumberOfExcessiveResidualsInSample::sMessageList.Entries ()))
		return returnValue;

	returnValue = *NumberOfExcessiveResidualsInSample::sMessageArray [i];
	return returnValue;
}



int NumberOfExcessiveResidualsInSample :: GetMessageLevel () const {

	return NumberOfExcessiveResidualsInSample::sMessageLevel;
}



int NumberOfExcessiveResidualsInSample :: GetNumberOfMessageParts () const {

	return NumberOfExcessiveResidualsInSample::sMessageList.Entries ();
}


int NumberOfExcessiveResidualsInSample :: GetSeverityLevel () const {

	return NumberOfExcessiveResidualsInSample::sMessageLevel;
}


void NumberOfExcessiveResidualsInSample :: SetIndexAndScope (int index, int scope) const {

	NumberOfExcessiveResidualsInSample::sMessageIndex = index;
	NumberOfExcessiveResidualsInSample::sMessageScope = scope;
}


int NumberOfExcessiveResidualsInSample :: GetMessageIndex () const {

	return NumberOfExcessiveResidualsInSample :: sMessageIndex;
}



int NumberOfExcessiveResidualsInSample :: GetScope () const {

	return NumberOfExcessiveResidualsInSample :: sMessageScope;
}



int NumberOfExcessiveResidualsInSample :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	NumberOfExcessiveResidualsInSample* noticeType = new NumberOfExcessiveResidualsInSample;
	warehouse->AddType (noticeType);
	return 1;
}





InterlocusPeakBelowInterlocusMinRFU :: InterlocusPeakBelowInterlocusMinRFU () : Notice () {

}


InterlocusPeakBelowInterlocusMinRFU :: InterlocusPeakBelowInterlocusMinRFU (const InterlocusPeakBelowInterlocusMinRFU& note) : Notice ((const Notice&) note) {

}



InterlocusPeakBelowInterlocusMinRFU :: ~InterlocusPeakBelowInterlocusMinRFU () {

}



int InterlocusPeakBelowInterlocusMinRFU :: Initialize (NoticeBook& nBook) {

	RGString NoticeString = nBook.GetNoticeStringForIndex (InterlocusPeakBelowInterlocusMinRFU::sMessageNumber);

	if (NoticeString.Length () == 0)
		return -1;

	int status = 0;
	RGBracketStringSearch TextSearch ("<Text>", "</Text>", NoticeString);
	size_t startPos = 0;
	size_t endPos;
	RGString Text;
	RGString* newText;
	RGString temp;
	RGBracketStringSearch SubjectSearch ("<Subject>", "</Subject>", NoticeString);
	RGBracketStringSearch MessageLevelSearch ("<MessageLevel>", "</MessageLevel>", NoticeString);
	RGBracketStringSearch LabelSearch ("<Label>", "</Label>", NoticeString);
	RGBracketStringSearch TriggerSearch ("<TriggersLowPrioritySignal>", "</TriggersLowPrioritySignal>", NoticeString);
	RGBracketStringSearch DoNotCallSearch ("<DoNotCall>", "</DoNotCall>", NoticeString);


	while (TextSearch.FindNextBracketedString (startPos, endPos,Text)) {

		newText = new RGString (Text);
		InterlocusPeakBelowInterlocusMinRFU::sMessageList.Append (newText);
		startPos = endPos;
	}

	if (!SubjectSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterlocusPeakBelowInterlocusMinRFU::sSubject = temp.ConvertToInteger ();

	if (!MessageLevelSearch.FindNextBracketedString (0, endPos, temp))
		status = -1;

	InterlocusPeakBelowInterlocusMinRFU::sMessageLevel = temp.ConvertToInteger ();

	if (LabelSearch.FindNextBracketedString (0, endPos, temp))
		InterlocusPeakBelowInterlocusMinRFU::sLabel = temp;

	else
		InterlocusPeakBelowInterlocusMinRFU::sLabel = "";

	if (TriggerSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterlocusPeakBelowInterlocusMinRFU::sTriggerLowPriority = true;

		else
			InterlocusPeakBelowInterlocusMinRFU::sTriggerLowPriority = false;
	}

	if (DoNotCallSearch.FindNextBracketedString (0, endPos, temp)) {

		if (temp == "true")
			InterlocusPeakBelowInterlocusMinRFU::sDoNotCall = true;

		else
			InterlocusPeakBelowInterlocusMinRFU::sDoNotCall = false;
	}


	int N = InterlocusPeakBelowInterlocusMinRFU::sMessageList.Entries ();

	if (N == 1)
		N++;

	InterlocusPeakBelowInterlocusMinRFU::sMessageArray = new RGString* [N];
	RGDListIterator it (InterlocusPeakBelowInterlocusMinRFU::sMessageList);

	for (int i=0; i<N; i++)
		InterlocusPeakBelowInterlocusMinRFU::sMessageArray [i] = (RGString*) it ();

	return status;
}


int InterlocusPeakBelowInterlocusMinRFU :: GetSubject () const {

	return InterlocusPeakBelowInterlocusMinRFU::sSubject;
}


RGString InterlocusPeakBelowInterlocusMinRFU :: GetLabel () const {

	return InterlocusPeakBelowInterlocusMinRFU::sLabel;
}


bool InterlocusPeakBelowInterlocusMinRFU :: IsTriggerForLowPriority () const {

	return InterlocusPeakBelowInterlocusMinRFU::sTriggerLowPriority;
}


bool InterlocusPeakBelowInterlocusMinRFU :: IsDoNotCall () const {

	return InterlocusPeakBelowInterlocusMinRFU::sDoNotCall;
}



RGString InterlocusPeakBelowInterlocusMinRFU :: GetMessagePart (int i) const {

	RGString returnValue;

	if ((i < 0) || (i >= InterlocusPeakBelowInterlocusMinRFU::sMessageList.Entries ()))
		return returnValue;

	returnValue = *InterlocusPeakBelowInterlocusMinRFU::sMessageArray [i];
	return returnValue;
}



int InterlocusPeakBelowInterlocusMinRFU :: GetMessageLevel () const {

	return InterlocusPeakBelowInterlocusMinRFU::sMessageLevel;
}



int InterlocusPeakBelowInterlocusMinRFU :: GetNumberOfMessageParts () const {

	return InterlocusPeakBelowInterlocusMinRFU::sMessageList.Entries ();
}


int InterlocusPeakBelowInterlocusMinRFU :: GetSeverityLevel () const {

	return InterlocusPeakBelowInterlocusMinRFU::sMessageLevel;
}


void InterlocusPeakBelowInterlocusMinRFU :: SetIndexAndScope (int index, int scope) const {

	InterlocusPeakBelowInterlocusMinRFU::sMessageIndex = index;
	InterlocusPeakBelowInterlocusMinRFU::sMessageScope = scope;
}


int InterlocusPeakBelowInterlocusMinRFU :: GetMessageIndex () const {

	return InterlocusPeakBelowInterlocusMinRFU :: sMessageIndex;
}



int InterlocusPeakBelowInterlocusMinRFU :: GetScope () const {

	return InterlocusPeakBelowInterlocusMinRFU :: sMessageScope;
}



int InterlocusPeakBelowInterlocusMinRFU :: LoadType () {

	NoticeWarehouse* warehouse = new NoticeWarehouse;
	InterlocusPeakBelowInterlocusMinRFU* noticeType = new InterlocusPeakBelowInterlocusMinRFU;
	warehouse->AddType (noticeType);
	return 1;
}




