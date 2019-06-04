//
// These are all subclasses of class SmartNotice, used for STR Osiris.  Each one corresponds
// to a particular issue or situation
//

#include "STRSmartNotices.h"


RGString smILSFailed::sName = "smILSFailed";
int smILSFailed::sSubject = smILSFailed::LoadType ();
int smILSFailed::sMessageIndex = 0;
int smILSFailed::sMessageScope = 0;


RGString smLocusIsAMEL::sName = "smLocusIsAMEL";
int smLocusIsAMEL::sSubject = smLocusIsAMEL::LoadType ();
int smLocusIsAMEL::sMessageIndex = 0;
int smLocusIsAMEL::sMessageScope = 0;


RGString smSampleIsLadder::sName = "smSampleIsLadder";
int smSampleIsLadder::sSubject = smSampleIsLadder::LoadType ();
int smSampleIsLadder::sMessageIndex = 0;
int smSampleIsLadder::sMessageScope = 0;


RGString smSampleIsPosCtrl::sName = "smSampleIsPosCtrl";
int smSampleIsPosCtrl::sSubject = smSampleIsPosCtrl::LoadType ();
int smSampleIsPosCtrl::sMessageIndex = 0;
int smSampleIsPosCtrl::sMessageScope = 0;


RGString smSampleIsNegCtrl::sName = "smSampleIsNegCtrl";
int smSampleIsNegCtrl::sSubject = smSampleIsNegCtrl::LoadType ();
int smSampleIsNegCtrl::sMessageIndex = 0;
int smSampleIsNegCtrl::sMessageScope = 0;


RGString smILSSpacingFailed::sName = "smILSSpacingFailed";
int smILSSpacingFailed::sSubject = smILSSpacingFailed::LoadType ();
int smILSSpacingFailed::sMessageIndex = 0;
int smILSSpacingFailed::sMessageScope = 0;


RGString smNoLadderFile::sName = "smNoLadderFile";
int smNoLadderFile::sSubject = smNoLadderFile::LoadType ();
int smNoLadderFile::sMessageIndex = 0;
int smNoLadderFile::sMessageScope = 0;


RGString smLadderTypeUnknown::sName = "smLadderTypeUnknown";
int smLadderTypeUnknown::sSubject = smLadderTypeUnknown::LoadType ();
int smLadderTypeUnknown::sMessageIndex = 0;
int smLadderTypeUnknown::sMessageScope = 0;


RGString smReagentKitUnknown::sName = "smReagentKitUnknown";
int smReagentKitUnknown::sSubject = smReagentKitUnknown::LoadType ();
int smReagentKitUnknown::sMessageIndex = 0;
int smReagentKitUnknown::sMessageScope = 0;


RGString smMoreThanOneReagentKit::sName = "smMoreThanOneReagentKit";
int smMoreThanOneReagentKit::sSubject = smMoreThanOneReagentKit::LoadType ();
int smMoreThanOneReagentKit::sMessageIndex = 0;
int smMoreThanOneReagentKit::sMessageScope = 0;


RGString smCurveFitFailed::sName = "smCurveFitFailed";
int smCurveFitFailed::sSubject = smCurveFitFailed::LoadType ();
int smCurveFitFailed::sMessageIndex = 0;
int smCurveFitFailed::sMessageScope = 0;


RGString smCurveFitMarginal::sName = "smCurveFitMarginal";
int smCurveFitMarginal::sSubject = smCurveFitMarginal::LoadType ();
int smCurveFitMarginal::sMessageIndex = 0;
int smCurveFitMarginal::sMessageScope = 0;


RGString smSpike::sName = "smSpike";
int smSpike::sSubject = smSpike::LoadType ();
int smSpike::sMessageIndex = 0;
int smSpike::sMessageScope = 0;


RGString smStutter::sName = "smStutter";
int smStutter::sSubject = smStutter::LoadType ();
int smStutter::sMessageIndex = 0;
int smStutter::sMessageScope = 0;


RGString smAdenylation::sName = "smAdenylation";
int smAdenylation::sSubject = smAdenylation::LoadType ();
int smAdenylation::sMessageIndex = 0;
int smAdenylation::sMessageScope = 0;


RGString smBlob::sName = "smBlob";
int smBlob::sSubject = smBlob::LoadType ();
int smBlob::sMessageIndex = 0;
int smBlob::sMessageScope = 0;


RGString smPullUp::sName = "smPullUp";
int smPullUp::sSubject = smPullUp::LoadType ();
int smPullUp::sMessageIndex = 0;
int smPullUp::sMessageScope = 0;


RGString smILSTooFewPeaks::sName = "smILSTooFewPeaks";
int smILSTooFewPeaks::sSubject = smILSTooFewPeaks::LoadType ();
int smILSTooFewPeaks::sMessageIndex = 0;
int smILSTooFewPeaks::sMessageScope = 0;


RGString smPeakOutsideILS::sName = "smPeakOutsideILS";
int smPeakOutsideILS::sSubject = smPeakOutsideILS::LoadType ();
int smPeakOutsideILS::sMessageIndex = 0;
int smPeakOutsideILS::sMessageScope = 0;


RGString smMarkerSetNameUnknown::sName = "smMarkerSetNameUnknown";
int smMarkerSetNameUnknown::sSubject = smMarkerSetNameUnknown::LoadType ();
int smMarkerSetNameUnknown::sMessageIndex = 0;
int smMarkerSetNameUnknown::sMessageScope = 0;


RGString smNamedILSUnknown::sName = "smNamedILSUnknown";
int smNamedILSUnknown::sSubject = smNamedILSUnknown::LoadType ();
int smNamedILSUnknown::sMessageIndex = 0;
int smNamedILSUnknown::sMessageScope = 0;


RGString smBelowMinRFU::sName = "smBelowMinRFU";
int smBelowMinRFU::sSubject = smBelowMinRFU::LoadType ();
int smBelowMinRFU::sMessageIndex = 0;
int smBelowMinRFU::sMessageScope = 0;


RGString smUnexpectedILSPeak::sName = "smUnexpectedILSPeak";
int smUnexpectedILSPeak::sSubject = smUnexpectedILSPeak::LoadType ();
int smUnexpectedILSPeak::sMessageIndex = 0;
int smUnexpectedILSPeak::sMessageScope = 0;


RGString smNoPosCtrlFound::sName = "smNoPosCtrlFound";
int smNoPosCtrlFound::sSubject = smNoPosCtrlFound::LoadType ();
int smNoPosCtrlFound::sMessageIndex = 0;
int smNoPosCtrlFound::sMessageScope = 0;


RGString smNoNegCtrlFound::sName = "smNoNegCtrlFound";
int smNoNegCtrlFound::sSubject = smNoNegCtrlFound::LoadType ();
int smNoNegCtrlFound::sMessageIndex = 0;
int smNoNegCtrlFound::sMessageScope = 0;


RGString smNamedPosCtrlNotFound::sName = "smNamedPosCtrlNotFound";
int smNamedPosCtrlNotFound::sSubject = smNamedPosCtrlNotFound::LoadType ();
int smNamedPosCtrlNotFound::sMessageIndex = 0;
int smNamedPosCtrlNotFound::sMessageScope = 0;


RGString smPeakHeightAboveMax::sName = "smPeakHeightAboveMax";
int smPeakHeightAboveMax::sSubject = smPeakHeightAboveMax::LoadType ();
int smPeakHeightAboveMax::sMessageIndex = 0;
int smPeakHeightAboveMax::sMessageScope = 0;


RGString smCannotSeparateILSPrimerPeaks::sName = "smCannotSeparateILSPrimerPeaks";
int smCannotSeparateILSPrimerPeaks::sSubject = smCannotSeparateILSPrimerPeaks::LoadType ();
int smCannotSeparateILSPrimerPeaks::sMessageIndex = 0;
int smCannotSeparateILSPrimerPeaks::sMessageScope = 0;


RGString smILSNeighborFilterTooAggressive::sName = "smILSNeighborFilterTooAggressive";
int smILSNeighborFilterTooAggressive::sSubject = smILSNeighborFilterTooAggressive::LoadType ();
int smILSNeighborFilterTooAggressive::sMessageIndex = 0;
int smILSNeighborFilterTooAggressive::sMessageScope = 0;


RGString smILSFractionFilterTooAggressive::sName = "smILSFractionFilterTooAggressive";
int smILSFractionFilterTooAggressive::sSubject = smILSFractionFilterTooAggressive::LoadType ();
int smILSFractionFilterTooAggressive::sMessageIndex = 0;
int smILSFractionFilterTooAggressive::sMessageScope = 0;


RGString smILSRelativeHeightsInconsistent::sName = "smILSRelativeHeightsInconsistent";
int smILSRelativeHeightsInconsistent::sSubject = smILSRelativeHeightsInconsistent::LoadType ();
int smILSRelativeHeightsInconsistent::sMessageIndex = 0;
int smILSRelativeHeightsInconsistent::sMessageScope = 0;


RGString smOsirisBelowMinRFUButRawDataAbove::sName = "smOsirisBelowMinRFUButRawDataAbove";
int smOsirisBelowMinRFUButRawDataAbove::sSubject = smOsirisBelowMinRFUButRawDataAbove::LoadType ();
int smOsirisBelowMinRFUButRawDataAbove::sMessageIndex = 0;
int smOsirisBelowMinRFUButRawDataAbove::sMessageScope = 0;


RGString smRawDataBelowMinRFUButOsirisAbove::sName = "smRawDataBelowMinRFUButOsirisAbove";
int smRawDataBelowMinRFUButOsirisAbove::sSubject = smRawDataBelowMinRFUButOsirisAbove::LoadType ();
int smRawDataBelowMinRFUButOsirisAbove::sMessageIndex = 0;
int smRawDataBelowMinRFUButOsirisAbove::sMessageScope = 0;


RGString smOsirisAboveMaxRFUButRawDataBelow::sName = "smOsirisAboveMaxRFUButRawDataBelow";
int smOsirisAboveMaxRFUButRawDataBelow::sSubject = smOsirisAboveMaxRFUButRawDataBelow::LoadType ();
int smOsirisAboveMaxRFUButRawDataBelow::sMessageIndex = 0;
int smOsirisAboveMaxRFUButRawDataBelow::sMessageScope = 0;


RGString smRawDataAboveMaxRFUButOsirisBelow::sName = "smRawDataAboveMaxRFUButOsirisBelow";
int smRawDataAboveMaxRFUButOsirisBelow::sSubject = smRawDataAboveMaxRFUButOsirisBelow::LoadType ();
int smRawDataAboveMaxRFUButOsirisBelow::sMessageIndex = 0;
int smRawDataAboveMaxRFUButOsirisBelow::sMessageScope = 0;


RGString smStage1Successful::sName = "smStage1Successful";
int smStage1Successful::sSubject = smStage1Successful::LoadType ();
int smStage1Successful::sMessageIndex = 0;
int smStage1Successful::sMessageScope = 0;


RGString smSignalIsCtrlPeak::sName = "smSignalIsCtrlPeak";
int smSignalIsCtrlPeak::sSubject = smSignalIsCtrlPeak::LoadType ();
int smSignalIsCtrlPeak::sMessageIndex = 0;
int smSignalIsCtrlPeak::sMessageScope = 0;


RGString smLocusTooFewPeaks::sName = "smLocusTooFewPeaks";
int smLocusTooFewPeaks::sSubject = smLocusTooFewPeaks::LoadType ();
int smLocusTooFewPeaks::sMessageIndex = 0;
int smLocusTooFewPeaks::sMessageScope = 0;


RGString smInterlocusLadderPeak::sName = "smInterlocusLadderPeak";
int smInterlocusLadderPeak::sSubject = smInterlocusLadderPeak::LoadType ();
int smInterlocusLadderPeak::sMessageIndex = 0;
int smInterlocusLadderPeak::sMessageScope = 0;


RGString smUnexpectedLadderLocusPeak::sName = "smUnexpectedLadderLocusPeak";
int smUnexpectedLadderLocusPeak::sSubject = smUnexpectedLadderLocusPeak::LoadType ();
int smUnexpectedLadderLocusPeak::sMessageIndex = 0;
int smUnexpectedLadderLocusPeak::sMessageScope = 0;


RGString smPoorPeakMorphology::sName = "smPoorPeakMorphology";
int smPoorPeakMorphology::sSubject = smPoorPeakMorphology::LoadType ();
int smPoorPeakMorphology::sMessageIndex = 0;
int smPoorPeakMorphology::sMessageScope = 0;


RGString smPoorLocusMorphology::sName = "smPoorLocusMorphology";
int smPoorLocusMorphology::sSubject = smPoorLocusMorphology::LoadType ();
int smPoorLocusMorphology::sMessageIndex = 0;
int smPoorLocusMorphology::sMessageScope = 0;


RGString smLocusMayHaveUnreportedOLs::sName = "smLocusMayHaveUnreportedOLs";
int smLocusMayHaveUnreportedOLs::sSubject = smLocusMayHaveUnreportedOLs::LoadType ();
int smLocusMayHaveUnreportedOLs::sMessageIndex = 0;
int smLocusMayHaveUnreportedOLs::sMessageScope = 0;


RGString smNPeaksForLocusUnavailable::sName = "smNPeaksForLocusUnavailable";
int smNPeaksForLocusUnavailable::sSubject = smNPeaksForLocusUnavailable::LoadType ();
int smNPeaksForLocusUnavailable::sMessageIndex = 0;
int smNPeaksForLocusUnavailable::sMessageScope = 0;


RGString smRelativeHeightImbalance::sName = "smRelativeHeightImbalance";
int smRelativeHeightImbalance::sSubject = smRelativeHeightImbalance::LoadType ();
int smRelativeHeightImbalance::sMessageIndex = 0;
int smRelativeHeightImbalance::sMessageScope = 0;


RGString smHeightBelowFractionalFilter::sName = "smHeightBelowFractionalFilter";
int smHeightBelowFractionalFilter::sSubject = smHeightBelowFractionalFilter::LoadType ();
int smHeightBelowFractionalFilter::sMessageIndex = 0;
int smHeightBelowFractionalFilter::sMessageScope = 0;


RGString smPartOfExtendedLocusLeft::sName = "smPartOfExtendedLocusLeft";
int smPartOfExtendedLocusLeft::sSubject = smPartOfExtendedLocusLeft::LoadType ();
int smPartOfExtendedLocusLeft::sMessageIndex = 0;
int smPartOfExtendedLocusLeft::sMessageScope = 0;


RGString smPartOfExtendedLocusRight::sName = "smPartOfExtendedLocusRight";
int smPartOfExtendedLocusRight::sSubject = smPartOfExtendedLocusRight::LoadType ();
int smPartOfExtendedLocusRight::sMessageIndex = 0;
int smPartOfExtendedLocusRight::sMessageScope = 0;


RGString smBelowFractionalFilterLeft::sName = "smBelowFractionalFilterLeft";
int smBelowFractionalFilterLeft::sSubject = smBelowFractionalFilterLeft::LoadType ();
int smBelowFractionalFilterLeft::sMessageIndex = 0;
int smBelowFractionalFilterLeft::sMessageScope = 0;


RGString smBelowFractionalFilterRight::sName = "smBelowFractionalFilterRight";
int smBelowFractionalFilterRight::sSubject = smBelowFractionalFilterRight::LoadType ();
int smBelowFractionalFilterRight::sMessageIndex = 0;
int smBelowFractionalFilterRight::sMessageScope = 0;


RGString smStage2Successful::sName = "smStage2Successful";
int smStage2Successful::sSubject = smStage2Successful::LoadType ();
int smStage2Successful::sMessageIndex = 0;
int smStage2Successful::sMessageScope = 0;


RGString smPullUpAtControlPeak::sName = "smPullUpAtControlPeak";
int smPullUpAtControlPeak::sSubject = smPullUpAtControlPeak::LoadType ();
int smPullUpAtControlPeak::sMessageIndex = 0;
int smPullUpAtControlPeak::sMessageScope = 0;


RGString smInterlocusPullUpWithPoorFit::sName = "smInterlocusPullUpWithPoorFit";
int smInterlocusPullUpWithPoorFit::sSubject = smInterlocusPullUpWithPoorFit::LoadType ();
int smInterlocusPullUpWithPoorFit::sMessageIndex = 0;
int smInterlocusPullUpWithPoorFit::sMessageScope = 0;


RGString smPossibleValidInterlocusLeft::sName = "smPossibleValidInterlocusLeft";
int smPossibleValidInterlocusLeft::sSubject = smPossibleValidInterlocusLeft::LoadType ();
int smPossibleValidInterlocusLeft::sMessageIndex = 0;
int smPossibleValidInterlocusLeft::sMessageScope = 0;


RGString smPossibleValidInterlocusRight::sName = "smPossibleValidInterlocusRight";
int smPossibleValidInterlocusRight::sSubject = smPossibleValidInterlocusRight::LoadType ();
int smPossibleValidInterlocusRight::sMessageIndex = 0;
int smPossibleValidInterlocusRight::sMessageScope = 0;


RGString smUnassignedInterlocusPeak::sName = "smUnassignedInterlocusPeak";
int smUnassignedInterlocusPeak::sSubject = smUnassignedInterlocusPeak::LoadType ();
int smUnassignedInterlocusPeak::sMessageIndex = 0;
int smUnassignedInterlocusPeak::sMessageScope = 0;


RGString smAmbiguousInterlocus::sName = "smAmbiguousInterlocus";
int smAmbiguousInterlocus::sSubject = smAmbiguousInterlocus::LoadType ();
int smAmbiguousInterlocus::sMessageIndex = 0;
int smAmbiguousInterlocus::sMessageScope = 0;


RGString smTooLowInterlocusPeak::sName = "smTooLowInterlocusPeak";
int smTooLowInterlocusPeak::sSubject = smTooLowInterlocusPeak::LoadType ();
int smTooLowInterlocusPeak::sMessageIndex = 0;
int smTooLowInterlocusPeak::sMessageScope = 0;


RGString smPossibleInterlocusArtifactLeft::sName = "smPossibleInterlocusArtifactLeft";
int smPossibleInterlocusArtifactLeft::sSubject = smPossibleInterlocusArtifactLeft::LoadType ();
int smPossibleInterlocusArtifactLeft::sMessageIndex = 0;
int smPossibleInterlocusArtifactLeft::sMessageScope = 0;


RGString smPossibleInterlocusArtifactRight::sName = "smPossibleInterlocusArtifactRight";
int smPossibleInterlocusArtifactRight::sSubject = smPossibleInterlocusArtifactRight::LoadType ();
int smPossibleInterlocusArtifactRight::sMessageIndex = 0;
int smPossibleInterlocusArtifactRight::sMessageScope = 0;


RGString smFirstAlleleEqFirstLadderPeak::sName = "smFirstAlleleEqFirstLadderPeak";
int smFirstAlleleEqFirstLadderPeak::sSubject = smFirstAlleleEqFirstLadderPeak::LoadType ();
int smFirstAlleleEqFirstLadderPeak::sMessageIndex = 0;
int smFirstAlleleEqFirstLadderPeak::sMessageScope = 0;


RGString smSignalOL::sName = "smSignalOL";
int smSignalOL::sSubject = smSignalOL::LoadType ();
int smSignalOL::sMessageIndex = 0;
int smSignalOL::sMessageScope = 0;


RGString smAMELMisaligned::sName = "smAMELMisaligned";
int smAMELMisaligned::sSubject = smAMELMisaligned::LoadType ();
int smAMELMisaligned::sMessageIndex = 0;
int smAMELMisaligned::sMessageScope = 0;


RGString smCrater::sName = "smCrater";
int smCrater::sSubject = smCrater::LoadType ();
int smCrater::sMessageIndex = 0;
int smCrater::sMessageScope = 0;


RGString smAcceptedOLLeft::sName = "smAcceptedOLLeft";
int smAcceptedOLLeft::sSubject = smAcceptedOLLeft::LoadType ();
int smAcceptedOLLeft::sMessageIndex = 0;
int smAcceptedOLLeft::sMessageScope = 0;


RGString smAcceptedOLRight::sName = "smAcceptedOLRight";
int smAcceptedOLRight::sSubject = smAcceptedOLRight::LoadType ();
int smAcceptedOLRight::sMessageIndex = 0;
int smAcceptedOLRight::sMessageScope = 0;


RGString smStage3Successful::sName = "smStage3Successful";
int smStage3Successful::sSubject = smStage3Successful::LoadType ();
int smStage3Successful::sMessageIndex = 0;
int smStage3Successful::sMessageScope = 0;


RGString smValidOLInterlocusLeft::sName = "smValidOLInterlocusLeft";
int smValidOLInterlocusLeft::sSubject = smValidOLInterlocusLeft::LoadType ();
int smValidOLInterlocusLeft::sMessageIndex = 0;
int smValidOLInterlocusLeft::sMessageScope = 0;


RGString smValidOLInterlocusRight::sName = "smValidOLInterlocusRight";
int smValidOLInterlocusRight::sSubject = smValidOLInterlocusRight::LoadType ();
int smValidOLInterlocusRight::sMessageIndex = 0;
int smValidOLInterlocusRight::sMessageScope = 0;


RGString smAcceptedInterlocusLeft::sName = "smAcceptedInterlocusLeft";
int smAcceptedInterlocusLeft::sSubject = smAcceptedInterlocusLeft::LoadType ();
int smAcceptedInterlocusLeft::sMessageIndex = 0;
int smAcceptedInterlocusLeft::sMessageScope = 0;


RGString smAcceptedInterlocusRight::sName = "smAcceptedInterlocusRight";
int smAcceptedInterlocusRight::sSubject = smAcceptedInterlocusRight::LoadType ();
int smAcceptedInterlocusRight::sMessageIndex = 0;
int smAcceptedInterlocusRight::sMessageScope = 0;


RGString smNoXForAMEL::sName = "smNoXForAMEL";
int smNoXForAMEL::sSubject = smNoXForAMEL::LoadType ();
int smNoXForAMEL::sMessageIndex = 0;
int smNoXForAMEL::sMessageScope = 0;


RGString smSignalIsAllele::sName = "smSignalIsAllele";
int smSignalIsAllele::sSubject = smSignalIsAllele::LoadType ();
int smSignalIsAllele::sMessageIndex = 0;
int smSignalIsAllele::sMessageScope = 0;


RGString smPosCtrlLocusMismatch::sName = "smPosCtrlLocusMismatch";
int smPosCtrlLocusMismatch::sSubject = smPosCtrlLocusMismatch::LoadType ();
int smPosCtrlLocusMismatch::sMessageIndex = 0;
int smPosCtrlLocusMismatch::sMessageScope = 0;


RGString smNoGenotypeFound::sName = "smNoGenotypeFound";
int smNoGenotypeFound::sSubject = smNoGenotypeFound::LoadType ();
int smNoGenotypeFound::sMessageIndex = 0;
int smNoGenotypeFound::sMessageScope = 0;


RGString smThreeOrMoreAlleles::sName = "smThreeOrMoreAlleles";
int smThreeOrMoreAlleles::sSubject = smThreeOrMoreAlleles::LoadType ();
int smThreeOrMoreAlleles::sMessageIndex = 0;
int smThreeOrMoreAlleles::sMessageScope = 0;


RGString smTwoOrMoreAlleles::sName = "smTwoOrMoreAlleles";
int smTwoOrMoreAlleles::sSubject = smTwoOrMoreAlleles::LoadType ();
int smTwoOrMoreAlleles::sMessageIndex = 0;
int smTwoOrMoreAlleles::sMessageScope = 0;


RGString smHeterozygousImbalance::sName = "smHeterozygousImbalance";
int smHeterozygousImbalance::sSubject = smHeterozygousImbalance::LoadType ();
int smHeterozygousImbalance::sMessageIndex = 0;
int smHeterozygousImbalance::sMessageScope = 0;


RGString smOLAlleles::sName = "smOLAlleles";
int smOLAlleles::sSubject = smOLAlleles::LoadType ();
int smOLAlleles::sMessageIndex = 0;
int smOLAlleles::sMessageScope = 0;


RGString smHomozygoteHeightProblem::sName = "smHomozygoteHeightProblem";
int smHomozygoteHeightProblem::sSubject = smHomozygoteHeightProblem::LoadType ();
int smHomozygoteHeightProblem::sMessageIndex = 0;
int smHomozygoteHeightProblem::sMessageScope = 0;


RGString smPullUpAtAllele::sName = "smPullUpAtAllele";
int smPullUpAtAllele::sSubject = smPullUpAtAllele::LoadType ();
int smPullUpAtAllele::sMessageIndex = 0;
int smPullUpAtAllele::sMessageScope = 0;


RGString smPosCtrlLocusNotFound::sName = "smPosCtrlLocusNotFound";
int smPosCtrlLocusNotFound::sSubject = smPosCtrlLocusNotFound::LoadType ();
int smPosCtrlLocusNotFound::sMessageIndex = 0;
int smPosCtrlLocusNotFound::sMessageScope = 0;


RGString smBPResidualTooLarge::sName = "smBPResidualTooLarge";
int smBPResidualTooLarge::sSubject = smBPResidualTooLarge::LoadType ();
int smBPResidualTooLarge::sMessageIndex = 0;
int smBPResidualTooLarge::sMessageScope = 0;


RGString smExtraLadderAllele::sName = "smExtraLadderAllele";
int smExtraLadderAllele::sSubject = smExtraLadderAllele::LoadType ();
int smExtraLadderAllele::sMessageIndex = 0;
int smExtraLadderAllele::sMessageScope = 0;


RGString smCriticalMessagesAtILS::sName = "smCriticalMessagesAtILS";
int smCriticalMessagesAtILS::sSubject = smCriticalMessagesAtILS::LoadType ();
int smCriticalMessagesAtILS::sMessageIndex = 0;
int smCriticalMessagesAtILS::sMessageScope = 0;


RGString smAMELHasExtraneousPeaks::sName = "smAMELHasExtraneousPeaks";
int smAMELHasExtraneousPeaks::sSubject = smAMELHasExtraneousPeaks::LoadType ();
int smAMELHasExtraneousPeaks::sMessageIndex = 0;
int smAMELHasExtraneousPeaks::sMessageScope = 0;


RGString smExtraneousAMELPeak::sName = "smExtraneousAMELPeak";
int smExtraneousAMELPeak::sSubject = smExtraneousAMELPeak::LoadType ();
int smExtraneousAMELPeak::sMessageIndex = 0;
int smExtraneousAMELPeak::sMessageScope = 0;


RGString smNegCtrlChannelHasNoPrimerPeaks::sName = "smNegCtrlChannelHasNoPrimerPeaks";
int smNegCtrlChannelHasNoPrimerPeaks::sSubject = smNegCtrlChannelHasNoPrimerPeaks::LoadType ();
int smNegCtrlChannelHasNoPrimerPeaks::sMessageIndex = 0;
int smNegCtrlChannelHasNoPrimerPeaks::sMessageScope = 0;


RGString smMinimumHeightForPrimerPeaks::sName = "smMinimumHeightForPrimerPeaks";
int smMinimumHeightForPrimerPeaks::sSubject = smMinimumHeightForPrimerPeaks::LoadType ();
int smMinimumHeightForPrimerPeaks::sMessageIndex = 0;
int smMinimumHeightForPrimerPeaks::sMessageScope = 0;


RGString smInterlocusPeaksToRight::sName = "smInterlocusPeaksToRight";
int smInterlocusPeaksToRight::sSubject = smInterlocusPeaksToRight::LoadType ();
int smInterlocusPeaksToRight::sMessageIndex = 0;
int smInterlocusPeaksToRight::sMessageScope = 0;


RGString smInterlocusPeaksToLeft::sName = "smInterlocusPeaksToLeft";
int smInterlocusPeaksToLeft::sSubject = smInterlocusPeaksToLeft::LoadType ();
int smInterlocusPeaksToLeft::sMessageIndex = 0;
int smInterlocusPeaksToLeft::sMessageScope = 0;


RGString smLadderFailed::sName = "smLadderFailed";
int smLadderFailed::sSubject = smLadderFailed::LoadType ();
int smLadderFailed::sMessageIndex = 0;
int smLadderFailed::sMessageScope = 0;


RGString smSampleUnanalyzable::sName = "smSampleUnanalyzable";
int smSampleUnanalyzable::sSubject = smSampleUnanalyzable::LoadType ();
int smSampleUnanalyzable::sMessageIndex = 0;
int smSampleUnanalyzable::sMessageScope = 0;


RGString smCriticalMessagesAtAlleles::sName = "smCriticalMessagesAtAlleles";
int smCriticalMessagesAtAlleles::sSubject = smCriticalMessagesAtAlleles::LoadType ();
int smCriticalMessagesAtAlleles::sMessageIndex = 0;
int smCriticalMessagesAtAlleles::sMessageScope = 0;


RGString smHeightBelowPullupFractionalFilter::sName = "smHeightBelowPullupFractionalFilter";
int smHeightBelowPullupFractionalFilter::sSubject = smHeightBelowPullupFractionalFilter::LoadType ();
int smHeightBelowPullupFractionalFilter::sMessageIndex = 0;
int smHeightBelowPullupFractionalFilter::sMessageScope = 0;


RGString smBelowPullupFractionalFilterLeft::sName = "smBelowPullupFractionalFilterLeft";
int smBelowPullupFractionalFilterLeft::sSubject = smBelowPullupFractionalFilterLeft::LoadType ();
int smBelowPullupFractionalFilterLeft::sMessageIndex = 0;
int smBelowPullupFractionalFilterLeft::sMessageScope = 0;


RGString smBelowPullupFractionalFilterRight::sName = "smBelowPullupFractionalFilterRight";
int smBelowPullupFractionalFilterRight::sSubject = smBelowPullupFractionalFilterRight::LoadType ();
int smBelowPullupFractionalFilterRight::sMessageIndex = 0;
int smBelowPullupFractionalFilterRight::sMessageScope = 0;


RGString smPossibleExtraLocusAllele::sName = "smPossibleExtraLocusAllele";
int smPossibleExtraLocusAllele::sSubject = smPossibleExtraLocusAllele::LoadType ();
int smPossibleExtraLocusAllele::sMessageIndex = 0;
int smPossibleExtraLocusAllele::sMessageScope = 0;


RGString smUnexpectedPeakInNegativeControl::sName = "smUnexpectedPeakInNegativeControl";
int smUnexpectedPeakInNegativeControl::sSubject = smUnexpectedPeakInNegativeControl::LoadType ();
int smUnexpectedPeakInNegativeControl::sMessageIndex = 0;
int smUnexpectedPeakInNegativeControl::sMessageScope = 0;


RGString smLocusContainsPeaksWithExcessiveResiduals::sName = "smLocusContainsPeaksWithExcessiveResiduals";
int smLocusContainsPeaksWithExcessiveResiduals::sSubject = smLocusContainsPeaksWithExcessiveResiduals::LoadType ();
int smLocusContainsPeaksWithExcessiveResiduals::sMessageIndex = 0;
int smLocusContainsPeaksWithExcessiveResiduals::sMessageScope = 0;


RGString smPrimaryInterchannelLink::sName = "smPrimaryInterchannelLink";
int smPrimaryInterchannelLink::sSubject = smPrimaryInterchannelLink::LoadType ();
int smPrimaryInterchannelLink::sMessageIndex = 0;
int smPrimaryInterchannelLink::sMessageScope = 0;


RGString smNotInterchannelLink::sName = "smNotInterchannelLink";
int smNotInterchannelLink::sSubject = smNotInterchannelLink::LoadType ();
int smNotInterchannelLink::sMessageIndex = 0;
int smNotInterchannelLink::sMessageScope = 0;


RGString smHeightBelowInterlocusMinRFU::sName = "smHeightBelowInterlocusMinRFU";
int smHeightBelowInterlocusMinRFU::sSubject = smHeightBelowInterlocusMinRFU::LoadType ();
int smHeightBelowInterlocusMinRFU::sMessageIndex = 0;
int smHeightBelowInterlocusMinRFU::sMessageScope = 0;


RGString smFSAChannelUnreadable::sName = "smFSAChannelUnreadable";
int smFSAChannelUnreadable::sSubject = smFSAChannelUnreadable::LoadType ();
int smFSAChannelUnreadable::sMessageIndex = 0;
int smFSAChannelUnreadable::sMessageScope = 0;


RGString smAmbiguousInterlocusCrater::sName = "smAmbiguousInterlocusCrater";
int smAmbiguousInterlocusCrater::sSubject = smAmbiguousInterlocusCrater::LoadType ();
int smAmbiguousInterlocusCrater::sMessageIndex = 0;
int smAmbiguousInterlocusCrater::sMessageScope = 0;


RGString smPossibleAmbiguousInterlocusAssignedToLocus::sName = "smPossibleAmbiguousInterlocusAssignedToLocus";
int smPossibleAmbiguousInterlocusAssignedToLocus::sSubject = smPossibleAmbiguousInterlocusAssignedToLocus::LoadType ();
int smPossibleAmbiguousInterlocusAssignedToLocus::sMessageIndex = 0;
int smPossibleAmbiguousInterlocusAssignedToLocus::sMessageScope = 0;


RGString smWouldCauseHeterozygousImbalanceLeft::sName = "smWouldCauseHeterozygousImbalanceLeft";
int smWouldCauseHeterozygousImbalanceLeft::sSubject = smWouldCauseHeterozygousImbalanceLeft::LoadType ();
int smWouldCauseHeterozygousImbalanceLeft::sMessageIndex = 0;
int smWouldCauseHeterozygousImbalanceLeft::sMessageScope = 0;


RGString smWouldCauseHeterozygousImbalanceRight::sName = "smWouldCauseHeterozygousImbalanceRight";
int smWouldCauseHeterozygousImbalanceRight::sSubject = smWouldCauseHeterozygousImbalanceRight::LoadType ();
int smWouldCauseHeterozygousImbalanceRight::sMessageIndex = 0;
int smWouldCauseHeterozygousImbalanceRight::sMessageScope = 0;


RGString smSignalOffGridLeft::sName = "smSignalOffGridLeft";
int smSignalOffGridLeft::sSubject = smSignalOffGridLeft::LoadType ();
int smSignalOffGridLeft::sMessageIndex = 0;
int smSignalOffGridLeft::sMessageScope = 0;


RGString smSignalOffGridRight::sName = "smSignalOffGridRight";
int smSignalOffGridRight::sSubject = smSignalOffGridRight::LoadType ();
int smSignalOffGridRight::sMessageIndex = 0;
int smSignalOffGridRight::sMessageScope = 0;


RGString sm0UnambiguousPeaksLeft::sName = "sm0UnambiguousPeaksLeft";
int sm0UnambiguousPeaksLeft::sSubject = sm0UnambiguousPeaksLeft::LoadType ();
int sm0UnambiguousPeaksLeft::sMessageIndex = 0;
int sm0UnambiguousPeaksLeft::sMessageScope = 0;


RGString sm0UnambiguousPeaksRight::sName = "sm0UnambiguousPeaksRight";
int sm0UnambiguousPeaksRight::sSubject = sm0UnambiguousPeaksRight::LoadType ();
int sm0UnambiguousPeaksRight::sMessageIndex = 0;
int sm0UnambiguousPeaksRight::sMessageScope = 0;


RGString sm1UnambiguousPeakLeft::sName = "sm1UnambiguousPeakLeft";
int sm1UnambiguousPeakLeft::sSubject = sm1UnambiguousPeakLeft::LoadType ();
int sm1UnambiguousPeakLeft::sMessageIndex = 0;
int sm1UnambiguousPeakLeft::sMessageScope = 0;


RGString sm1UnambiguousPeakRight::sName = "sm1UnambiguousPeakRight";
int sm1UnambiguousPeakRight::sSubject = sm1UnambiguousPeakRight::LoadType ();
int sm1UnambiguousPeakRight::sMessageIndex = 0;
int sm1UnambiguousPeakRight::sMessageScope = 0;


RGString sm2PlusUnambiguousPeaksLeft::sName = "sm2PlusUnambiguousPeaksLeft";
int sm2PlusUnambiguousPeaksLeft::sSubject = sm2PlusUnambiguousPeaksLeft::LoadType ();
int sm2PlusUnambiguousPeaksLeft::sMessageIndex = 0;
int sm2PlusUnambiguousPeaksLeft::sMessageScope = 0;


RGString sm2PlusUnambiguousPeaksRight::sName = "sm2PlusUnambiguousPeaksRight";
int sm2PlusUnambiguousPeaksRight::sSubject = sm2PlusUnambiguousPeaksRight::LoadType ();
int sm2PlusUnambiguousPeaksRight::sMessageIndex = 0;
int sm2PlusUnambiguousPeaksRight::sMessageScope = 0;


RGString sm0AmbiguousPeaksLeft::sName = "sm0AmbiguousPeaksLeft";
int sm0AmbiguousPeaksLeft::sSubject = sm0AmbiguousPeaksLeft::LoadType ();
int sm0AmbiguousPeaksLeft::sMessageIndex = 0;
int sm0AmbiguousPeaksLeft::sMessageScope = 0;


RGString sm0AmbiguousPeaksRight::sName = "sm0AmbiguousPeaksRight";
int sm0AmbiguousPeaksRight::sSubject = sm0AmbiguousPeaksRight::LoadType ();
int sm0AmbiguousPeaksRight::sMessageIndex = 0;
int sm0AmbiguousPeaksRight::sMessageScope = 0;


RGString sm1AmbiguousPeakLeft::sName = "sm1AmbiguousPeakLeft";
int sm1AmbiguousPeakLeft::sSubject = sm1AmbiguousPeakLeft::LoadType ();
int sm1AmbiguousPeakLeft::sMessageIndex = 0;
int sm1AmbiguousPeakLeft::sMessageScope = 0;


RGString sm1AmbiguousPeakRight::sName = "sm1AmbiguousPeakRight";
int sm1AmbiguousPeakRight::sSubject = sm1AmbiguousPeakRight::LoadType ();
int sm1AmbiguousPeakRight::sMessageIndex = 0;
int sm1AmbiguousPeakRight::sMessageScope = 0;


RGString sm2PlusAmbiguousPeaksLeft::sName = "sm2PlusAmbiguousPeaksLeft";
int sm2PlusAmbiguousPeaksLeft::sSubject = sm2PlusAmbiguousPeaksLeft::LoadType ();
int sm2PlusAmbiguousPeaksLeft::sMessageIndex = 0;
int sm2PlusAmbiguousPeaksLeft::sMessageScope = 0;


RGString sm2PlusAmbiguousPeaksRight::sName = "sm2PlusAmbiguousPeaksRight";
int sm2PlusAmbiguousPeaksRight::sSubject = sm2PlusAmbiguousPeaksRight::LoadType ();
int sm2PlusAmbiguousPeaksRight::sMessageIndex = 0;
int sm2PlusAmbiguousPeaksRight::sMessageScope = 0;


RGString smCouldBelongToLocusLeft::sName = "smCouldBelongToLocusLeft";
int smCouldBelongToLocusLeft::sSubject = smCouldBelongToLocusLeft::LoadType ();
int smCouldBelongToLocusLeft::sMessageIndex = 0;
int smCouldBelongToLocusLeft::sMessageScope = 0;


RGString smCouldBelongToLocusRight::sName = "smCouldBelongToLocusRight";
int smCouldBelongToLocusRight::sSubject = smCouldBelongToLocusRight::LoadType ();
int smCouldBelongToLocusRight::sMessageIndex = 0;
int smCouldBelongToLocusRight::sMessageScope = 0;


RGString smChannelIsNegControl::sName = "smChannelIsNegControl";
int smChannelIsNegControl::sSubject = smChannelIsNegControl::LoadType ();
int smChannelIsNegControl::sMessageIndex = 0;
int smChannelIsNegControl::sMessageScope = 0;


RGString smLocusIsNegControl::sName = "smLocusIsNegControl";
int smLocusIsNegControl::sSubject = smLocusIsNegControl::LoadType ();
int smLocusIsNegControl::sMessageIndex = 0;
int smLocusIsNegControl::sMessageScope = 0;


RGString smChannelIsPosControl::sName = "smChannelIsPosControl";
int smChannelIsPosControl::sSubject = smChannelIsPosControl::LoadType ();
int smChannelIsPosControl::sMessageIndex = 0;
int smChannelIsPosControl::sMessageScope = 0;


RGString smLocusIsPosControl::sName = "smLocusIsPosControl";
int smLocusIsPosControl::sSubject = smLocusIsPosControl::LoadType ();
int smLocusIsPosControl::sMessageIndex = 0;
int smLocusIsPosControl::sMessageScope = 0;


RGString smPurePullup::sName = "smPurePullup";
int smPurePullup::sSubject = smPurePullup::LoadType ();
int smPurePullup::sMessageIndex = 0;
int smPurePullup::sMessageScope = 0;


RGString smChannelPeaksHaveCriticalArtifacts::sName = "smChannelPeaksHaveCriticalArtifacts";
int smChannelPeaksHaveCriticalArtifacts::sSubject = smChannelPeaksHaveCriticalArtifacts::LoadType ();
int smChannelPeaksHaveCriticalArtifacts::sMessageIndex = 0;
int smChannelPeaksHaveCriticalArtifacts::sMessageScope = 0;


RGString smPreliminaryLocusHas2OrMoreAlleles::sName = "smPreliminaryLocusHas2OrMoreAlleles";
int smPreliminaryLocusHas2OrMoreAlleles::sSubject = smPreliminaryLocusHas2OrMoreAlleles::LoadType ();
int smPreliminaryLocusHas2OrMoreAlleles::sMessageIndex = 0;
int smPreliminaryLocusHas2OrMoreAlleles::sMessageScope = 0;


RGString smCraterSidePeak::sName = "smCraterSidePeak";
int smCraterSidePeak::sSubject = smCraterSidePeak::LoadType ();
int smCraterSidePeak::sMessageIndex = 0;
int smCraterSidePeak::sMessageScope = 0;


RGString smSignalNotACrater::sName = "smSignalNotACrater";
int smSignalNotACrater::sSubject = smSignalNotACrater::LoadType ();
int smSignalNotACrater::sMessageIndex = 0;
int smSignalNotACrater::sMessageScope = 0;


RGString smSignalNotACraterSidePeak::sName = "smSignalNotACraterSidePeak";
int smSignalNotACraterSidePeak::sSubject = smSignalNotACraterSidePeak::LoadType ();
int smSignalNotACraterSidePeak::sMessageIndex = 0;
int smSignalNotACraterSidePeak::sMessageScope = 0;


RGString smLocusIsHomozygous::sName = "smLocusIsHomozygous";
int smLocusIsHomozygous::sSubject = smLocusIsHomozygous::LoadType ();
int smLocusIsHomozygous::sMessageIndex = 0;
int smLocusIsHomozygous::sMessageScope = 0;


RGString smLocusMissingGenotype::sName = "smLocusMissingGenotype";
int smLocusMissingGenotype::sSubject = smLocusMissingGenotype::LoadType ();
int smLocusMissingGenotype::sMessageIndex = 0;
int smLocusMissingGenotype::sMessageScope = 0;


RGString smStage4Successful::sName = "smStage4Successful";
int smStage4Successful::sSubject = smStage4Successful::LoadType ();
int smStage4Successful::sMessageIndex = 0;
int smStage4Successful::sMessageScope = 0;


RGString smStage5Successful::sName = "smStage5Successful";
int smStage5Successful::sSubject = smStage5Successful::LoadType ();
int smStage5Successful::sMessageIndex = 0;
int smStage5Successful::sMessageScope = 0;


RGString smPeakInCoreLadderLocus::sName = "smPeakInCoreLadderLocus";
int smPeakInCoreLadderLocus::sSubject = smPeakInCoreLadderLocus::LoadType ();
int smPeakInCoreLadderLocus::sMessageIndex = 0;
int smPeakInCoreLadderLocus::sMessageScope = 0;


RGString smPeakAboveOverloadLimit::sName = "smPeakAboveOverloadLimit";
int smPeakAboveOverloadLimit::sSubject = smPeakAboveOverloadLimit::LoadType ();
int smPeakAboveOverloadLimit::sMessageIndex = 0;
int smPeakAboveOverloadLimit::sMessageScope = 0;


RGString smRestrictedPriorityPeak::sName = "smRestrictedPriorityPeak";
int smRestrictedPriorityPeak::sSubject = smRestrictedPriorityPeak::LoadType ();
int smRestrictedPriorityPeak::sMessageIndex = 0;
int smRestrictedPriorityPeak::sMessageScope = 0;


RGString smChannelIsILS::sName = "smChannelIsILS";
int smChannelIsILS::sSubject = smChannelIsILS::LoadType ();
int smChannelIsILS::sMessageIndex = 0;
int smChannelIsILS::sMessageScope = 0;


RGString smILSSpacingIsMarginal::sName = "smILSSpacingIsMarginal";
int smILSSpacingIsMarginal::sSubject = smILSSpacingIsMarginal::LoadType ();
int smILSSpacingIsMarginal::sMessageIndex = 0;
int smILSSpacingIsMarginal::sMessageScope = 0;


RGString smPoorPeakMorphologyOrResolution::sName = "smPoorPeakMorphologyOrResolution";
int smPoorPeakMorphologyOrResolution::sSubject = smPoorPeakMorphologyOrResolution::LoadType ();
int smPoorPeakMorphologyOrResolution::sMessageIndex = 0;
int smPoorPeakMorphologyOrResolution::sMessageScope = 0;


RGString smPeakToRightOfILS::sName = "smPeakToRightOfILS";
int smPeakToRightOfILS::sSubject = smPeakToRightOfILS::LoadType ();
int smPeakToRightOfILS::sMessageIndex = 0;
int smPeakToRightOfILS::sMessageScope = 0;


RGString smPreliminaryLocusHasNoAlleles::sName = "smPreliminaryLocusHasNoAlleles";
int smPreliminaryLocusHasNoAlleles::sSubject = smPreliminaryLocusHasNoAlleles::LoadType ();
int smPreliminaryLocusHasNoAlleles::sMessageIndex = 0;
int smPreliminaryLocusHasNoAlleles::sMessageScope = 0;


RGString smLocusHasPeaksBetweenDetectionAndAnalysisThresholds::sName = "smLocusHasPeaksBetweenDetectionAndAnalysisThresholds";
int smLocusHasPeaksBetweenDetectionAndAnalysisThresholds::sSubject = smLocusHasPeaksBetweenDetectionAndAnalysisThresholds::LoadType ();
int smLocusHasPeaksBetweenDetectionAndAnalysisThresholds::sMessageIndex = 0;
int smLocusHasPeaksBetweenDetectionAndAnalysisThresholds::sMessageScope = 0;


RGString smUnexpectedNoOfGridPeaks::sName = "smUnexpectedNoOfGridPeaks";
int smUnexpectedNoOfGridPeaks::sSubject = smUnexpectedNoOfGridPeaks::LoadType ();
int smUnexpectedNoOfGridPeaks::sMessageIndex = 0;
int smUnexpectedNoOfGridPeaks::sMessageScope = 0;


RGString smMoreThanThreeAlleles::sName = "smMoreThanThreeAlleles";
int smMoreThanThreeAlleles::sSubject = smMoreThanThreeAlleles::LoadType ();
int smMoreThanThreeAlleles::sMessageIndex = 0;
int smMoreThanThreeAlleles::sMessageScope = 0;


RGString smDefaultsAreOverridden::sName = "smDefaultsAreOverridden";
int smDefaultsAreOverridden::sSubject = smDefaultsAreOverridden::LoadType ();
int smDefaultsAreOverridden::sMessageIndex = 0;
int smDefaultsAreOverridden::sMessageScope = 0;


RGString smAssociatedLadderIsCritical::sName = "smAssociatedLadderIsCritical";
int smAssociatedLadderIsCritical::sSubject = smAssociatedLadderIsCritical::LoadType ();
int smAssociatedLadderIsCritical::sMessageIndex = 0;
int smAssociatedLadderIsCritical::sMessageScope = 0;


RGString smAssociatedLadderLocusIsCritical::sName = "smAssociatedLadderLocusIsCritical";
int smAssociatedLadderLocusIsCritical::sSubject = smAssociatedLadderLocusIsCritical::LoadType ();
int smAssociatedLadderLocusIsCritical::sMessageIndex = 0;
int smAssociatedLadderLocusIsCritical::sMessageScope = 0;


RGString smChannelHasRaisedBaseline::sName = "smChannelHasRaisedBaseline";
int smChannelHasRaisedBaseline::sSubject = smChannelHasRaisedBaseline::LoadType ();
int smChannelHasRaisedBaseline::sMessageIndex = 0;
int smChannelHasRaisedBaseline::sMessageScope = 0;


RGString smChannelHasExcessiveNoiseAboveAnalysisThreshold::sName = "smChannelHasExcessiveNoiseAboveAnalysisThreshold";
int smChannelHasExcessiveNoiseAboveAnalysisThreshold::sSubject = smChannelHasExcessiveNoiseAboveAnalysisThreshold::LoadType ();
int smChannelHasExcessiveNoiseAboveAnalysisThreshold::sMessageIndex = 0;
int smChannelHasExcessiveNoiseAboveAnalysisThreshold::sMessageScope = 0;


RGString smChannelHasExcessiveNoiseAboveDetectionThreshold::sName = "smChannelHasExcessiveNoiseAboveDetectionThreshold";
int smChannelHasExcessiveNoiseAboveDetectionThreshold::sSubject = smChannelHasExcessiveNoiseAboveDetectionThreshold::LoadType ();
int smChannelHasExcessiveNoiseAboveDetectionThreshold::sMessageIndex = 0;
int smChannelHasExcessiveNoiseAboveDetectionThreshold::sMessageScope = 0;


RGString smSampleChannelRaisedBaselineThreshold::sName = "smSampleChannelRaisedBaselineThreshold";
int smSampleChannelRaisedBaselineThreshold::sSubject = smSampleChannelRaisedBaselineThreshold::LoadType ();
int smSampleChannelRaisedBaselineThreshold::sMessageIndex = 0;
int smSampleChannelRaisedBaselineThreshold::sMessageScope = 0;


RGString smSampleILSChannelRaisedBaselineThreshold::sName = "smSampleILSChannelRaisedBaselineThreshold";
int smSampleILSChannelRaisedBaselineThreshold::sSubject = smSampleILSChannelRaisedBaselineThreshold::LoadType ();
int smSampleILSChannelRaisedBaselineThreshold::sMessageIndex = 0;
int smSampleILSChannelRaisedBaselineThreshold::sMessageScope = 0;


RGString smILSPeakMayBeIncorrect::sName = "smILSPeakMayBeIncorrect";
int smILSPeakMayBeIncorrect::sSubject = smILSPeakMayBeIncorrect::LoadType ();
int smILSPeakMayBeIncorrect::sMessageIndex = 0;
int smILSPeakMayBeIncorrect::sMessageScope = 0;


RGString smTestForColorCorrectionMatrixPreset::sName = "smTestForColorCorrectionMatrixPreset";
int smTestForColorCorrectionMatrixPreset::sSubject = smTestForColorCorrectionMatrixPreset::LoadType ();
int smTestForColorCorrectionMatrixPreset::sMessageIndex = 0;
int smTestForColorCorrectionMatrixPreset::sMessageScope = 0;


RGString smColorCorrectionMatrixExpectedButNotFound::sName = "smColorCorrectionMatrixExpectedButNotFound";
int smColorCorrectionMatrixExpectedButNotFound::sSubject = smColorCorrectionMatrixExpectedButNotFound::LoadType ();
int smColorCorrectionMatrixExpectedButNotFound::sMessageIndex = 0;
int smColorCorrectionMatrixExpectedButNotFound::sMessageScope = 0;


RGString smColorCorrectionMatrixWrongSize::sName = "smColorCorrectionMatrixWrongSize";
int smColorCorrectionMatrixWrongSize::sSubject = smColorCorrectionMatrixWrongSize::LoadType ();
int smColorCorrectionMatrixWrongSize::sMessageIndex = 0;
int smColorCorrectionMatrixWrongSize::sMessageScope = 0;


RGString smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing::sName = "smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing";
int smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing::sSubject = smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing::LoadType ();
int smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing::sMessageIndex = 0;
int smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing::sMessageScope = 0;


RGString smMinimumNumberOfPrimerPeaksForNegativeControlChannel::sName = "smMinimumNumberOfPrimerPeaksForNegativeControlChannel";
int smMinimumNumberOfPrimerPeaksForNegativeControlChannel::sSubject = smMinimumNumberOfPrimerPeaksForNegativeControlChannel::LoadType ();
int smMinimumNumberOfPrimerPeaksForNegativeControlChannel::sMessageIndex = 0;
int smMinimumNumberOfPrimerPeaksForNegativeControlChannel::sMessageScope = 0;


RGString smUseSampleNamesForControlSampleTestsPreset::sName = "smUseSampleNamesForControlSampleTestsPreset";
int smUseSampleNamesForControlSampleTestsPreset::sSubject = smUseSampleNamesForControlSampleTestsPreset::LoadType ();
int smUseSampleNamesForControlSampleTestsPreset::sMessageIndex = 0;
int smUseSampleNamesForControlSampleTestsPreset::sMessageScope = 0;


RGString smNoYForAMEL::sName = "smNoYForAMEL";
int smNoYForAMEL::sSubject = smNoYForAMEL::LoadType ();
int smNoYForAMEL::sMessageIndex = 0;
int smNoYForAMEL::sMessageScope = 0;


RGString smLocusIsYLinked::sName = "smLocusIsYLinked";
int smLocusIsYLinked::sSubject = smLocusIsYLinked::LoadType ();
int smLocusIsYLinked::sMessageIndex = 0;
int smLocusIsYLinked::sMessageScope = 0;


RGString smNumberAllelesBelowExpectation::sName = "smNumberAllelesBelowExpectation";
int smNumberAllelesBelowExpectation::sSubject = smNumberAllelesBelowExpectation::LoadType ();
int smNumberAllelesBelowExpectation::sMessageIndex = 0;
int smNumberAllelesBelowExpectation::sMessageScope = 0;


RGString smBaselineRelativeStutter::sName = "smBaselineRelativeStutter";
int smBaselineRelativeStutter::sSubject = smBaselineRelativeStutter::LoadType ();
int smBaselineRelativeStutter::sMessageIndex = 0;
int smBaselineRelativeStutter::sMessageScope = 0;


RGString smBaselineRelativeAdenylation::sName = "smBaselineRelativeAdenylation";
int smBaselineRelativeAdenylation::sSubject = smBaselineRelativeAdenylation::LoadType ();
int smBaselineRelativeAdenylation::sMessageIndex = 0;
int smBaselineRelativeAdenylation::sMessageScope = 0;


RGString smBaselineRelativeBelowMinRFU::sName = "smBaselineRelativeBelowMinRFU";
int smBaselineRelativeBelowMinRFU::sSubject = smBaselineRelativeBelowMinRFU::LoadType ();
int smBaselineRelativeBelowMinRFU::sMessageIndex = 0;
int smBaselineRelativeBelowMinRFU::sMessageScope = 0;


RGString smBaselineRelativeBelowMinInterlocusRFU::sName = "smBaselineRelativeBelowMinInterlocusRFU";
int smBaselineRelativeBelowMinInterlocusRFU::sSubject = smBaselineRelativeBelowMinInterlocusRFU::LoadType ();
int smBaselineRelativeBelowMinInterlocusRFU::sMessageIndex = 0;
int smBaselineRelativeBelowMinInterlocusRFU::sMessageScope = 0;


RGString smTestRelativeBaselinePreset::sName = "smTestRelativeBaselinePreset";
int smTestRelativeBaselinePreset::sSubject = smTestRelativeBaselinePreset::LoadType ();
int smTestRelativeBaselinePreset::sMessageIndex = 0;
int smTestRelativeBaselinePreset::sMessageScope = 0;


RGString smIgnoreNegativeRelativeBaselinePreset::sName = "smIgnoreNegativeRelativeBaselinePreset";
int smIgnoreNegativeRelativeBaselinePreset::sSubject = smIgnoreNegativeRelativeBaselinePreset::LoadType ();
int smIgnoreNegativeRelativeBaselinePreset::sMessageIndex = 0;
int smIgnoreNegativeRelativeBaselinePreset::sMessageScope = 0;


RGString smNormalizeRawDataRelativeToBaselinePreset::sName = "smNormalizeRawDataRelativeToBaselinePreset";
int smNormalizeRawDataRelativeToBaselinePreset::sSubject = smNormalizeRawDataRelativeToBaselinePreset::LoadType ();
int smNormalizeRawDataRelativeToBaselinePreset::sMessageIndex = 0;
int smNormalizeRawDataRelativeToBaselinePreset::sMessageScope = 0;


RGString smPercentOfStandardNoiseThreshold::sName = "smPercentOfStandardNoiseThreshold";
int smPercentOfStandardNoiseThreshold::sSubject = smPercentOfStandardNoiseThreshold::LoadType ();
int smPercentOfStandardNoiseThreshold::sMessageIndex = 0;
int smPercentOfStandardNoiseThreshold::sMessageScope = 0;


RGString smLadderFitThreshold::sName = "smLadderFitThreshold";
int smLadderFitThreshold::sSubject = smLadderFitThreshold::LoadType ();
int smLadderFitThreshold::sMessageIndex = 0;
int smLadderFitThreshold::sMessageScope = 0;


RGString smSampleToLadderFitBelowExpectations::sName = "smSampleToLadderFitBelowExpectations";
int smSampleToLadderFitBelowExpectations::sSubject = smSampleToLadderFitBelowExpectations::LoadType ();
int smSampleToLadderFitBelowExpectations::sMessageIndex = 0;
int smSampleToLadderFitBelowExpectations::sMessageScope = 0;


RGString smEnableRawDataFilterForNormalizationPreset::sName = "smEnableRawDataFilterForNormalizationPreset";
int smEnableRawDataFilterForNormalizationPreset::sSubject = smEnableRawDataFilterForNormalizationPreset::LoadType ();
int smEnableRawDataFilterForNormalizationPreset::sMessageIndex = 0;
int smEnableRawDataFilterForNormalizationPreset::sMessageScope = 0;


RGString smBaselineEstimationThreshold::sName = "smBaselineEstimationThreshold";
int smBaselineEstimationThreshold::sSubject = smBaselineEstimationThreshold::LoadType ();
int smBaselineEstimationThreshold::sMessageIndex = 0;
int smBaselineEstimationThreshold::sMessageScope = 0;


RGString smSinglePassFilterWindowWidth::sName = "smSinglePassFilterWindowWidth";
int smSinglePassFilterWindowWidth::sSubject = smSinglePassFilterWindowWidth::LoadType ();
int smSinglePassFilterWindowWidth::sMessageIndex = 0;
int smSinglePassFilterWindowWidth::sMessageScope = 0;


RGString smAmbiguousInterlocusWithPoorMorphology::sName = "smAmbiguousInterlocusWithPoorMorphology";
int smAmbiguousInterlocusWithPoorMorphology::sSubject = smAmbiguousInterlocusWithPoorMorphology::LoadType ();
int smAmbiguousInterlocusWithPoorMorphology::sMessageIndex = 0;
int smAmbiguousInterlocusWithPoorMorphology::sMessageScope = 0;


RGString smSigmoidalPullup::sName = "smSigmoidalPullup";
int smSigmoidalPullup::sSubject = smSigmoidalPullup::LoadType ();
int smSigmoidalPullup::sMessageIndex = 0;
int smSigmoidalPullup::sMessageScope = 0;


RGString smMinPrimaryPullupThreshold::sName = "smMinPrimaryPullupThreshold";
int smMinPrimaryPullupThreshold::sSubject = smMinPrimaryPullupThreshold::LoadType ();
int smMinPrimaryPullupThreshold::sMessageIndex = 0;
int smMinPrimaryPullupThreshold::sMessageScope = 0;


RGString smExcessiveResidualDisplacement::sName = "smExcessiveResidualDisplacement";
int smExcessiveResidualDisplacement::sSubject = smExcessiveResidualDisplacement::LoadType ();
int smExcessiveResidualDisplacement::sMessageIndex = 0;
int smExcessiveResidualDisplacement::sMessageScope = 0;


RGString smResidualDisplacementThreshold::sName = "smResidualDisplacementThreshold";
int smResidualDisplacementThreshold::sSubject = smResidualDisplacementThreshold::LoadType ();
int smResidualDisplacementThreshold::sMessageIndex = 0;
int smResidualDisplacementThreshold::sMessageScope = 0;


RGString smResidualDisplacementTestPreset::sName = "smResidualDisplacementTestPreset";
int smResidualDisplacementTestPreset::sSubject = smResidualDisplacementTestPreset::LoadType ();
int smResidualDisplacementTestPreset::sMessageIndex = 0;
int smResidualDisplacementTestPreset::sMessageScope = 0;


RGString smExcessiveResidualDisplacementLeft::sName = "smExcessiveResidualDisplacementLeft";
int smExcessiveResidualDisplacementLeft::sSubject = smExcessiveResidualDisplacementLeft::LoadType ();
int smExcessiveResidualDisplacementLeft::sMessageIndex = 0;
int smExcessiveResidualDisplacementLeft::sMessageScope = 0;


RGString smExcessiveResidualDisplacementRight::sName = "smExcessiveResidualDisplacementRight";
int smExcessiveResidualDisplacementRight::sSubject = smExcessiveResidualDisplacementRight::LoadType ();
int smExcessiveResidualDisplacementRight::sMessageIndex = 0;
int smExcessiveResidualDisplacementRight::sMessageScope = 0;


RGString smSampleSatisfiesPossibleMixtureIDCriteria::sName = "smSampleSatisfiesPossibleMixtureIDCriteria";
int smSampleSatisfiesPossibleMixtureIDCriteria::sSubject = smSampleSatisfiesPossibleMixtureIDCriteria::LoadType ();
int smSampleSatisfiesPossibleMixtureIDCriteria::sMessageIndex = 0;
int smSampleSatisfiesPossibleMixtureIDCriteria::sMessageScope = 0;


RGString smDisableLowLevelFiltersForKnownMixturesPreset::sName = "smDisableLowLevelFiltersForKnownMixturesPreset";
int smDisableLowLevelFiltersForKnownMixturesPreset::sSubject = smDisableLowLevelFiltersForKnownMixturesPreset::LoadType ();
int smDisableLowLevelFiltersForKnownMixturesPreset::sMessageIndex = 0;
int smDisableLowLevelFiltersForKnownMixturesPreset::sMessageScope = 0;


RGString smDisableStutterFilter::sName = "smDisableStutterFilter";
int smDisableStutterFilter::sSubject = smDisableStutterFilter::LoadType ();
int smDisableStutterFilter::sMessageIndex = 0;
int smDisableStutterFilter::sMessageScope = 0;


RGString smDisableAdenylationFilter::sName = "smDisableAdenylationFilter";
int smDisableAdenylationFilter::sSubject = smDisableAdenylationFilter::LoadType ();
int smDisableAdenylationFilter::sMessageIndex = 0;
int smDisableAdenylationFilter::sMessageScope = 0;


RGString smSelectTriplePassVsSinglePassFilterPreset::sName = "smSelectTriplePassVsSinglePassFilterPreset";
int smSelectTriplePassVsSinglePassFilterPreset::sSubject = smSelectTriplePassVsSinglePassFilterPreset::LoadType ();
int smSelectTriplePassVsSinglePassFilterPreset::sMessageIndex = 0;
int smSelectTriplePassVsSinglePassFilterPreset::sMessageScope = 0;


RGString smTriplePassFilterWindowWidth::sName = "smTriplePassFilterWindowWidth";
int smTriplePassFilterWindowWidth::sSubject = smTriplePassFilterWindowWidth::LoadType ();
int smTriplePassFilterWindowWidth::sMessageIndex = 0;
int smTriplePassFilterWindowWidth::sMessageScope = 0;


RGString smLadderFitThresholdUsingMinError::sName = "smLadderFitThresholdUsingMinError";
int smLadderFitThresholdUsingMinError::sSubject = smLadderFitThresholdUsingMinError::LoadType ();
int smLadderFitThresholdUsingMinError::sMessageIndex = 0;
int smLadderFitThresholdUsingMinError::sMessageScope = 0;


RGString smUseMaxSecondDerivativesForSampleToLadderFit::sName = "smUseMaxSecondDerivativesForSampleToLadderFit";
int smUseMaxSecondDerivativesForSampleToLadderFit::sSubject = smUseMaxSecondDerivativesForSampleToLadderFit::LoadType ();
int smUseMaxSecondDerivativesForSampleToLadderFit::sMessageIndex = 0;
int smUseMaxSecondDerivativesForSampleToLadderFit::sMessageScope = 0;


RGString smPartOfDualSignal::sName = "smPartOfDualSignal";
int smPartOfDualSignal::sSubject = smPartOfDualSignal::LoadType ();
int smPartOfDualSignal::sMessageIndex = 0;
int smPartOfDualSignal::sMessageScope = 0;


RGString smNegativePeak::sName = "smNegativePeak";
int smNegativePeak::sSubject = smNegativePeak::LoadType ();
int smNegativePeak::sMessageIndex = 0;
int smNegativePeak::sMessageScope = 0;


RGString smCallOnLadderAdenylationPreset::sName = "smCallOnLadderAdenylationPreset";
int smCallOnLadderAdenylationPreset::sSubject = smCallOnLadderAdenylationPreset::LoadType ();
int smCallOnLadderAdenylationPreset::sMessageIndex = 0;
int smCallOnLadderAdenylationPreset::sMessageScope = 0;


RGString smSignalIsIntegralMultipleOfRepeatLeft::sName = "smSignalIsIntegralMultipleOfRepeatLeft";
int smSignalIsIntegralMultipleOfRepeatLeft::sSubject = smSignalIsIntegralMultipleOfRepeatLeft::LoadType ();
int smSignalIsIntegralMultipleOfRepeatLeft::sMessageIndex = 0;
int smSignalIsIntegralMultipleOfRepeatLeft::sMessageScope = 0;


RGString smSignalIsIntegralMultipleOfRepeatRight::sName = "smSignalIsIntegralMultipleOfRepeatRight";
int smSignalIsIntegralMultipleOfRepeatRight::sSubject = smSignalIsIntegralMultipleOfRepeatRight::LoadType ();
int smSignalIsIntegralMultipleOfRepeatRight::sMessageIndex = 0;
int smSignalIsIntegralMultipleOfRepeatRight::sMessageScope = 0;


RGString smILSFilterLeftShoulderPeaksPreset::sName = "smILSFilterLeftShoulderPeaksPreset";
int smILSFilterLeftShoulderPeaksPreset::sSubject = smILSFilterLeftShoulderPeaksPreset::LoadType ();
int smILSFilterLeftShoulderPeaksPreset::sMessageIndex = 0;
int smILSFilterLeftShoulderPeaksPreset::sMessageScope = 0;


RGString smILSShoulderPeakProximityThreshold::sName = "smILSShoulderPeakProximityThreshold";
int smILSShoulderPeakProximityThreshold::sSubject = smILSShoulderPeakProximityThreshold::LoadType ();
int smILSShoulderPeakProximityThreshold::sMessageIndex = 0;
int smILSShoulderPeakProximityThreshold::sMessageScope = 0;


RGString smILSShoulderFilterPercentThreshold::sName = "smILSShoulderFilterPercentThreshold";
int smILSShoulderFilterPercentThreshold::sSubject = smILSShoulderFilterPercentThreshold::LoadType ();
int smILSShoulderFilterPercentThreshold::sMessageIndex = 0;
int smILSShoulderFilterPercentThreshold::sMessageScope = 0;


RGString smILSShoulderPeak::sName = "smILSShoulderPeak";
int smILSShoulderPeak::sSubject = smILSShoulderPeak::LoadType ();
int smILSShoulderPeak::sMessageIndex = 0;
int smILSShoulderPeak::sMessageScope = 0;


RGString smLadderPeakOutsideCoreLadder::sName = "smLadderPeakOutsideCoreLadder";
int smLadderPeakOutsideCoreLadder::sSubject = smLadderPeakOutsideCoreLadder::LoadType ();
int smLadderPeakOutsideCoreLadder::sMessageIndex = 0;
int smLadderPeakOutsideCoreLadder::sMessageScope = 0;


RGString smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset::sName = "smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset";
int smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset::sSubject = smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset::LoadType ();
int smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset::sMessageIndex = 0;
int smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset::sMessageScope = 0;


RGString smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset::sName = "smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset";
int smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset::sSubject = smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset::LoadType ();
int smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset::sMessageIndex = 0;
int smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset::sMessageScope = 0;


RGString smExtendLociEdgeToEdgePreset::sName = "smExtendLociEdgeToEdgePreset";
int smExtendLociEdgeToEdgePreset::sSubject = smExtendLociEdgeToEdgePreset::LoadType ();
int smExtendLociEdgeToEdgePreset::sMessageIndex = 0;
int smExtendLociEdgeToEdgePreset::sMessageScope = 0;


RGString smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset::sName = "smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset";
int smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset::sSubject = smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset::LoadType ();
int smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset::sMessageIndex = 0;
int smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset::sMessageScope = 0;


RGString smMaxILSBPForExtendedLocus::sName = "smMaxILSBPForExtendedLocus";
int smMaxILSBPForExtendedLocus::sSubject = smMaxILSBPForExtendedLocus::LoadType ();
int smMaxILSBPForExtendedLocus::sMessageIndex = 0;
int smMaxILSBPForExtendedLocus::sMessageScope = 0;


RGString smIsQualityLocus::sName = "smIsQualityLocus";
int smIsQualityLocus::sSubject = smIsQualityLocus::LoadType ();
int smIsQualityLocus::sMessageIndex = 0;
int smIsQualityLocus::sMessageScope = 0;


RGString smScaleILSPrimerSearchPreset::sName = "smScaleILSPrimerSearchPreset";
int smScaleILSPrimerSearchPreset::sSubject = smScaleILSPrimerSearchPreset::LoadType ();
int smScaleILSPrimerSearchPreset::sMessageIndex = 0;
int smScaleILSPrimerSearchPreset::sMessageScope = 0;


RGString smNumberOfLastILSPeaksInScale::sName = "smNumberOfLastILSPeaksInScale";
int smNumberOfLastILSPeaksInScale::sSubject = smNumberOfLastILSPeaksInScale::LoadType ();
int smNumberOfLastILSPeaksInScale::sMessageIndex = 0;
int smNumberOfLastILSPeaksInScale::sMessageScope = 0;


RGString smPercentLastILSPeaksInScaling::sName = "smPercentLastILSPeaksInScaling";
int smPercentLastILSPeaksInScaling::sSubject = smPercentLastILSPeaksInScaling::LoadType ();
int smPercentLastILSPeaksInScaling::sMessageIndex = 0;
int smPercentLastILSPeaksInScaling::sMessageScope = 0;


RGString smLaserOffScale::sName = "smLaserOffScale";
int smLaserOffScale::sSubject = smLaserOffScale::LoadType ();
int smLaserOffScale::sMessageIndex = 0;
int smLaserOffScale::sMessageScope = 0;


RGString smCalculatedPurePullup::sName = "smCalculatedPurePullup";
int smCalculatedPurePullup::sSubject = smCalculatedPurePullup::LoadType ();
int smCalculatedPurePullup::sMessageIndex = 0;
int smCalculatedPurePullup::sMessageScope = 0;


RGString smPartialPullupBelowMinRFU::sName = "smPartialPullupBelowMinRFU";
int smPartialPullupBelowMinRFU::sSubject = smPartialPullupBelowMinRFU::LoadType ();
int smPartialPullupBelowMinRFU::sMessageIndex = 0;
int smPartialPullupBelowMinRFU::sMessageScope = 0;


RGString smSigmoidalSidePeak::sName = "smSigmoidalSidePeak";
int smSigmoidalSidePeak::sSubject = smSigmoidalSidePeak::LoadType ();
int smSigmoidalSidePeak::sMessageIndex = 0;
int smSigmoidalSidePeak::sMessageScope = 0;


RGString smDoNotCallStutterPeaksForSingleSourceSamplesPreset::sName = "smDoNotCallStutterPeaksForSingleSourceSamplesPreset";
int smDoNotCallStutterPeaksForSingleSourceSamplesPreset::sSubject = smDoNotCallStutterPeaksForSingleSourceSamplesPreset::LoadType ();
int smDoNotCallStutterPeaksForSingleSourceSamplesPreset::sMessageIndex = 0;
int smDoNotCallStutterPeaksForSingleSourceSamplesPreset::sMessageScope = 0;


RGString smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset::sName = "smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset";
int smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset::sSubject = smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset::LoadType ();
int smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset::sMessageIndex = 0;
int smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset::sMessageScope = 0;


RGString smCallStutterPeaksPreset::sName = "smCallStutterPeaksPreset";
int smCallStutterPeaksPreset::sSubject = smCallStutterPeaksPreset::LoadType ();
int smCallStutterPeaksPreset::sMessageIndex = 0;
int smCallStutterPeaksPreset::sMessageScope = 0;


RGString smIsAcceptedOLAllele::sName = "smIsAcceptedOLAllele";
int smIsAcceptedOLAllele::sSubject = smIsAcceptedOLAllele::LoadType ();
int smIsAcceptedOLAllele::sMessageIndex = 0;
int smIsAcceptedOLAllele::sMessageScope = 0;


RGString smSaveLadderILSHistoryPreset::sName = "smSaveLadderILSHistoryPreset";
int smSaveLadderILSHistoryPreset::sSubject = smSaveLadderILSHistoryPreset::LoadType ();
int smSaveLadderILSHistoryPreset::sMessageIndex = 0;
int smSaveLadderILSHistoryPreset::sMessageScope = 0;


RGString smLatitudeForILSFit::sName = "smLatitudeForILSFit";
int smLatitudeForILSFit::sSubject = smLatitudeForILSFit::LoadType ();
int smLatitudeForILSFit::sMessageIndex = 0;
int smLatitudeForILSFit::sMessageScope = 0;


RGString smUseLadderEndPointILSAlgorithmPreset::sName = "smUseLadderEndPointILSAlgorithmPreset";
int smUseLadderEndPointILSAlgorithmPreset::sSubject = smUseLadderEndPointILSAlgorithmPreset::LoadType ();
int smUseLadderEndPointILSAlgorithmPreset::sMessageIndex = 0;
int smUseLadderEndPointILSAlgorithmPreset::sMessageScope = 0;


RGString smPlusLatitudeForLadderEndPointILSFit::sName = "smPlusLatitudeForLadderEndPointILSFit";
int smPlusLatitudeForLadderEndPointILSFit::sSubject = smPlusLatitudeForLadderEndPointILSFit::LoadType ();
int smPlusLatitudeForLadderEndPointILSFit::sMessageIndex = 0;
int smPlusLatitudeForLadderEndPointILSFit::sMessageScope = 0;


RGString smApplyEnhancedShoulderFittingAlgorithmPreset::sName = "smApplyEnhancedShoulderFittingAlgorithmPreset";
int smApplyEnhancedShoulderFittingAlgorithmPreset::sSubject = smApplyEnhancedShoulderFittingAlgorithmPreset::LoadType ();
int smApplyEnhancedShoulderFittingAlgorithmPreset::sMessageIndex = 0;
int smApplyEnhancedShoulderFittingAlgorithmPreset::sMessageScope = 0;


RGString smNoiseFactorForShoulderAcceptanceThreshold::sName = "smNoiseFactorForShoulderAcceptanceThreshold";
int smNoiseFactorForShoulderAcceptanceThreshold::sSubject = smNoiseFactorForShoulderAcceptanceThreshold::LoadType ();
int smNoiseFactorForShoulderAcceptanceThreshold::sMessageIndex = 0;
int smNoiseFactorForShoulderAcceptanceThreshold::sMessageScope = 0;


RGString smConcaveDownAcceptanceThreshold::sName = "smConcaveDownAcceptanceThreshold";
int smConcaveDownAcceptanceThreshold::sSubject = smConcaveDownAcceptanceThreshold::LoadType ();
int smConcaveDownAcceptanceThreshold::sMessageIndex = 0;
int smConcaveDownAcceptanceThreshold::sMessageScope = 0;


RGString smCorePeakSharesAlleleBin::sName = "smCorePeakSharesAlleleBin";
int smCorePeakSharesAlleleBin::sSubject = smCorePeakSharesAlleleBin::LoadType ();
int smCorePeakSharesAlleleBin::sMessageIndex = 0;
int smCorePeakSharesAlleleBin::sMessageScope = 0;


RGString smMinImbalanceThresholdForCreatingNoisyPeak::sName = "smMinImbalanceThresholdForCreatingNoisyPeak";
int smMinImbalanceThresholdForCreatingNoisyPeak::sSubject = smMinImbalanceThresholdForCreatingNoisyPeak::LoadType ();
int smMinImbalanceThresholdForCreatingNoisyPeak::sMessageIndex = 0;
int smMinImbalanceThresholdForCreatingNoisyPeak::sMessageScope = 0;


RGString smPeakSharesAlleleBinLeft::sName = "smPeakSharesAlleleBinLeft";
int smPeakSharesAlleleBinLeft::sSubject = smPeakSharesAlleleBinLeft::LoadType ();
int smPeakSharesAlleleBinLeft::sMessageIndex = 0;
int smPeakSharesAlleleBinLeft::sMessageScope = 0;


RGString smPeakSharesAlleleBinRight::sName = "smPeakSharesAlleleBinRight";
int smPeakSharesAlleleBinRight::sSubject = smPeakSharesAlleleBinRight::LoadType ();
int smPeakSharesAlleleBinRight::sMessageIndex = 0;
int smPeakSharesAlleleBinRight::sMessageScope = 0;


RGString smConstrainPullupPatternAnalysisPreset::sName = "smConstrainPullupPatternAnalysisPreset";
int smConstrainPullupPatternAnalysisPreset::sSubject = smConstrainPullupPatternAnalysisPreset::LoadType ();
int smConstrainPullupPatternAnalysisPreset::sMessageIndex = 0;
int smConstrainPullupPatternAnalysisPreset::sMessageScope = 0;


RGString smOverrideNoiseLevelPercentsInSmoothing::sName = "smOverrideNoiseLevelPercentsInSmoothing";
int smOverrideNoiseLevelPercentsInSmoothing::sSubject = smOverrideNoiseLevelPercentsInSmoothing::LoadType ();
int smOverrideNoiseLevelPercentsInSmoothing::sMessageIndex = 0;
int smOverrideNoiseLevelPercentsInSmoothing::sMessageScope = 0;


RGString smPercentOverrideOfNoiseThresholdForNormalizationPhase::sName = "smPercentOverrideOfNoiseThresholdForNormalizationPhase";
int smPercentOverrideOfNoiseThresholdForNormalizationPhase::sSubject = smPercentOverrideOfNoiseThresholdForNormalizationPhase::LoadType ();
int smPercentOverrideOfNoiseThresholdForNormalizationPhase::sMessageIndex = 0;
int smPercentOverrideOfNoiseThresholdForNormalizationPhase::sMessageScope = 0;


RGString smPercentOverrideOfNoiseThresholdForFinalPhase::sName = "smPercentOverrideOfNoiseThresholdForFinalPhase";
int smPercentOverrideOfNoiseThresholdForFinalPhase::sSubject = smPercentOverrideOfNoiseThresholdForFinalPhase::LoadType ();
int smPercentOverrideOfNoiseThresholdForFinalPhase::sMessageIndex = 0;
int smPercentOverrideOfNoiseThresholdForFinalPhase::sMessageScope = 0;


RGString smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset::sName = "smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset";
int smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset::sSubject = smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset::LoadType ();
int smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset::sMessageIndex = 0;
int smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset::sMessageScope = 0;


RGString smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset::sName = "smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset";
int smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset::sSubject = smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset::LoadType ();
int smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset::sMessageIndex = 0;
int smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset::sMessageScope = 0;


RGString smDisplaySigmoidalPeaksPreset::sName = "smDisplaySigmoidalPeaksPreset";
int smDisplaySigmoidalPeaksPreset::sSubject = smDisplaySigmoidalPeaksPreset::LoadType ();
int smDisplaySigmoidalPeaksPreset::sMessageIndex = 0;
int smDisplaySigmoidalPeaksPreset::sMessageScope = 0;


RGString smResidualDisplacementFractionalLimit::sName = "smResidualDisplacementFractionalLimit";
int smResidualDisplacementFractionalLimit::sSubject = smResidualDisplacementFractionalLimit::LoadType ();
int smResidualDisplacementFractionalLimit::sMessageIndex = 0;
int smResidualDisplacementFractionalLimit::sMessageScope = 0;


RGString smSelectAveragingInPlaceFilterPreset::sName = "smSelectAveragingInPlaceFilterPreset";
int smSelectAveragingInPlaceFilterPreset::sSubject = smSelectAveragingInPlaceFilterPreset::LoadType ();
int smSelectAveragingInPlaceFilterPreset::sMessageIndex = 0;
int smSelectAveragingInPlaceFilterPreset::sMessageScope = 0;


RGString smAveragingInPlaceFilterWindowWidth::sName = "smAveragingInPlaceFilterWindowWidth";
int smAveragingInPlaceFilterWindowWidth::sSubject = smAveragingInPlaceFilterWindowWidth::LoadType ();
int smAveragingInPlaceFilterWindowWidth::sMessageIndex = 0;
int smAveragingInPlaceFilterWindowWidth::sMessageScope = 0;


RGString smUseProximityToPeaksOnlyPreset::sName = "smUseProximityToPeaksOnlyPreset";
int smUseProximityToPeaksOnlyPreset::sSubject = smUseProximityToPeaksOnlyPreset::LoadType ();
int smUseProximityToPeaksOnlyPreset::sMessageIndex = 0;
int smUseProximityToPeaksOnlyPreset::sMessageScope = 0;


RGString smDistanceFromPeakThreshold::sName = "smDistanceFromPeakThreshold";
int smDistanceFromPeakThreshold::sSubject = smDistanceFromPeakThreshold::LoadType ();
int smDistanceFromPeakThreshold::sMessageIndex = 0;
int smDistanceFromPeakThreshold::sMessageScope = 0;


RGString smUseProximityBothToPeaksAndLevelChangePreset::sName = "smUseProximityBothToPeaksAndLevelChangePreset";
int smUseProximityBothToPeaksAndLevelChangePreset::sSubject = smUseProximityBothToPeaksAndLevelChangePreset::LoadType ();
int smUseProximityBothToPeaksAndLevelChangePreset::sMessageIndex = 0;
int smUseProximityBothToPeaksAndLevelChangePreset::sMessageScope = 0;


RGString smDistanceFromLevelChange::sName = "smDistanceFromLevelChange";
int smDistanceFromLevelChange::sSubject = smDistanceFromLevelChange::LoadType ();
int smDistanceFromLevelChange::sMessageIndex = 0;
int smDistanceFromLevelChange::sMessageScope = 0;


RGString smPostPrimerPercentOfNoiseRangeForLevelChange::sName = "smPostPrimerPercentOfNoiseRangeForLevelChange";
int smPostPrimerPercentOfNoiseRangeForLevelChange::sSubject = smPostPrimerPercentOfNoiseRangeForLevelChange::LoadType ();
int smPostPrimerPercentOfNoiseRangeForLevelChange::sMessageIndex = 0;
int smPostPrimerPercentOfNoiseRangeForLevelChange::sMessageScope = 0;


RGString smMaxPercentLevelChangeToUseLevelChangeProximity::sName = "smMaxPercentLevelChangeToUseLevelChangeProximity";
int smMaxPercentLevelChangeToUseLevelChangeProximity::sSubject = smMaxPercentLevelChangeToUseLevelChangeProximity::LoadType ();
int smMaxPercentLevelChangeToUseLevelChangeProximity::sMessageIndex = 0;
int smMaxPercentLevelChangeToUseLevelChangeProximity::sMessageScope = 0;


RGString smPostPrimerPercentOfNoiseRangeToBeConsideredPeak::sName = "smPostPrimerPercentOfNoiseRangeToBeConsideredPeak";
int smPostPrimerPercentOfNoiseRangeToBeConsideredPeak::sSubject = smPostPrimerPercentOfNoiseRangeToBeConsideredPeak::LoadType ();
int smPostPrimerPercentOfNoiseRangeToBeConsideredPeak::sMessageIndex = 0;
int smPostPrimerPercentOfNoiseRangeToBeConsideredPeak::sMessageScope = 0;


RGString smILSBPForEndOfPrimerPeaks::sName = "smILSBPForEndOfPrimerPeaks";
int smILSBPForEndOfPrimerPeaks::sSubject = smILSBPForEndOfPrimerPeaks::LoadType ();
int smILSBPForEndOfPrimerPeaks::sMessageIndex = 0;
int smILSBPForEndOfPrimerPeaks::sMessageScope = 0;


RGString smPrePrimerPercentOfNoiseRangeToBeConsideredPeak::sName = "smPrePrimerPercentOfNoiseRangeToBeConsideredPeak";
int smPrePrimerPercentOfNoiseRangeToBeConsideredPeak::sSubject = smPrePrimerPercentOfNoiseRangeToBeConsideredPeak::LoadType ();
int smPrePrimerPercentOfNoiseRangeToBeConsideredPeak::sMessageIndex = 0;
int smPrePrimerPercentOfNoiseRangeToBeConsideredPeak::sMessageScope = 0;


RGString smPrePrimerPercentOfNoiseRangeForLevelChange::sName = "smPrePrimerPercentOfNoiseRangeForLevelChange";
int smPrePrimerPercentOfNoiseRangeForLevelChange::sSubject = smPrePrimerPercentOfNoiseRangeForLevelChange::LoadType ();
int smPrePrimerPercentOfNoiseRangeForLevelChange::sMessageIndex = 0;
int smPrePrimerPercentOfNoiseRangeForLevelChange::sMessageScope = 0;


RGString smWeakPrimaryInterchannelLink::sName = "smWeakPrimaryInterchannelLink";
int smWeakPrimaryInterchannelLink::sSubject = smWeakPrimaryInterchannelLink::LoadType ();
int smWeakPrimaryInterchannelLink::sMessageIndex = 0;
int smWeakPrimaryInterchannelLink::sMessageScope = 0;


RGString smZeroPullupPrimaryInterchannelLink::sName = "smZeroPullupPrimaryInterchannelLink";
int smZeroPullupPrimaryInterchannelLink::sSubject = smZeroPullupPrimaryInterchannelLink::LoadType ();
int smZeroPullupPrimaryInterchannelLink::sMessageIndex = 0;
int smZeroPullupPrimaryInterchannelLink::sMessageScope = 0;


RGString smRawDataPrimaryInterchannelLink::sName = "smRawDataPrimaryInterchannelLink";
int smRawDataPrimaryInterchannelLink::sSubject = smRawDataPrimaryInterchannelLink::LoadType ();
int smRawDataPrimaryInterchannelLink::sMessageIndex = 0;
int smRawDataPrimaryInterchannelLink::sMessageScope = 0;


RGString smThreePointPeak::sName = "smThreePointPeak";
int smThreePointPeak::sSubject = smThreePointPeak::LoadType ();
int smThreePointPeak::sMessageIndex = 0;
int smThreePointPeak::sMessageScope = 0;


RGString smIncludeNonStandardStutterInStutterOverlapPreset::sName = "smIncludeNonStandardStutterInStutterOverlapPreset";
int smIncludeNonStandardStutterInStutterOverlapPreset::sSubject = smIncludeNonStandardStutterInStutterOverlapPreset::LoadType ();
int smIncludeNonStandardStutterInStutterOverlapPreset::sMessageIndex = 0;
int smIncludeNonStandardStutterInStutterOverlapPreset::sMessageScope = 0;


RGString smNoNegativeDataPreset::sName = "smNoNegativeDataPreset";
int smNoNegativeDataPreset::sSubject = smNoNegativeDataPreset::LoadType ();
int smNoNegativeDataPreset::sMessageIndex = 0;
int smNoNegativeDataPreset::sMessageScope = 0;


RGString smTestPullupCorrectedHeightsPreset::sName = "smTestPullupCorrectedHeightsPreset";
int smTestPullupCorrectedHeightsPreset::sSubject = smTestPullupCorrectedHeightsPreset::LoadType ();
int smTestPullupCorrectedHeightsPreset::sMessageIndex = 0;
int smTestPullupCorrectedHeightsPreset::sMessageScope = 0;


RGString smTailHeightFittingThresholdFactor::sName = "smTailHeightFittingThresholdFactor";
int smTailHeightFittingThresholdFactor::sSubject = smTailHeightFittingThresholdFactor::LoadType ();
int smTailHeightFittingThresholdFactor::sMessageIndex = 0;
int smTailHeightFittingThresholdFactor::sMessageScope = 0;


RGString smTailSlopeFittingThresholdFactor::sName = "smTailSlopeFittingThresholdFactor";
int smTailSlopeFittingThresholdFactor::sSubject = smTailSlopeFittingThresholdFactor::LoadType ();
int smTailSlopeFittingThresholdFactor::sMessageIndex = 0;
int smTailSlopeFittingThresholdFactor::sMessageScope = 0;


RGString smSelectUserSpecifiedMinRFUForPrimaryPeakPreset::sName = "smSelectUserSpecifiedMinRFUForPrimaryPeakPreset";
int smSelectUserSpecifiedMinRFUForPrimaryPeakPreset::sSubject = smSelectUserSpecifiedMinRFUForPrimaryPeakPreset::LoadType ();
int smSelectUserSpecifiedMinRFUForPrimaryPeakPreset::sMessageIndex = 0;
int smSelectUserSpecifiedMinRFUForPrimaryPeakPreset::sMessageScope = 0;


RGString smRedundantPeak::sName = "smRedundantPeak";
int smRedundantPeak::sSubject = smRedundantPeak::LoadType ();
int smRedundantPeak::sMessageIndex = 0;
int smRedundantPeak::sMessageScope = 0;


PERSISTENT_DEFINITION (smILSFailed, 2051, "smILSFailed")
PERSISTENT_DEFINITION (smLocusIsAMEL, 2052, "smLocusIsAMEL")
PERSISTENT_DEFINITION (smSampleIsLadder, 2053, "smSampleIsLadder")
PERSISTENT_DEFINITION (smSampleIsPosCtrl, 2054, "smSampleIsPosCtrl")
PERSISTENT_DEFINITION (smSampleIsNegCtrl, 2055, "smSampleIsNegCtrl")
PERSISTENT_DEFINITION (smILSSpacingFailed, 2056, "smILSSpacingFailed")
PERSISTENT_DEFINITION (smNoLadderFile, 2057, "smNoLadderFile")
PERSISTENT_DEFINITION (smLadderTypeUnknown, 2058, "smLadderTypeUnknown")
PERSISTENT_DEFINITION (smReagentKitUnknown, 2059, "smReagentKitUnknown")
PERSISTENT_DEFINITION (smMoreThanOneReagentKit, 2060, "smMoreThanOneReagentKit")
PERSISTENT_DEFINITION (smCurveFitFailed, 2061, "smCurveFitFailed")
PERSISTENT_DEFINITION (smCurveFitMarginal, 2062, "smCurveFitMarginal")
PERSISTENT_DEFINITION (smSpike, 2063, "smSpike")
PERSISTENT_DEFINITION (smStutter, 2064, "smStutter")
PERSISTENT_DEFINITION (smAdenylation, 2065, "smAdenylation")
PERSISTENT_DEFINITION (smBlob, 2066, "smBlob")
PERSISTENT_DEFINITION (smPullUp, 2067, "smPullUp")
PERSISTENT_DEFINITION (smILSTooFewPeaks, 2068, "smILSTooFewPeaks")
PERSISTENT_DEFINITION (smPeakOutsideILS, 2070, "smPeakOutsideILS")
PERSISTENT_DEFINITION (smMarkerSetNameUnknown, 2071, "smMarkerSetNameUnknown")
PERSISTENT_DEFINITION (smNamedILSUnknown, 2072, "smNamedILSUnknown")
PERSISTENT_DEFINITION (smBelowMinRFU, 2073, "smBelowMinRFU")
PERSISTENT_DEFINITION (smUnexpectedILSPeak, 2074, "smUnexpectedILSPeak")
PERSISTENT_DEFINITION (smNoPosCtrlFound, 2077, "smNoPosCtrlFound")
PERSISTENT_DEFINITION (smNoNegCtrlFound, 2078, "smNoNegCtrlFound")
PERSISTENT_DEFINITION (smNamedPosCtrlNotFound, 2079, "smNamedPosCtrlNotFound")
PERSISTENT_DEFINITION (smPeakHeightAboveMax, 2080, "smPeakHeightAboveMax")
PERSISTENT_DEFINITION (smCannotSeparateILSPrimerPeaks, 2082, "smCannotSeparateILSPrimerPeaks")
PERSISTENT_DEFINITION (smILSNeighborFilterTooAggressive, 2083, "smILSNeighborFilterTooAggressive")
PERSISTENT_DEFINITION (smILSFractionFilterTooAggressive, 2084, "smILSFractionFilterTooAggressive")
PERSISTENT_DEFINITION (smILSRelativeHeightsInconsistent, 2085, "smILSRelativeHeightsInconsistent")
PERSISTENT_DEFINITION (smOsirisBelowMinRFUButRawDataAbove, 2086, "smOsirisBelowMinRFUButRawDataAbove")
PERSISTENT_DEFINITION (smRawDataBelowMinRFUButOsirisAbove, 2087, "smRawDataBelowMinRFUButOsirisAbove")
PERSISTENT_DEFINITION (smOsirisAboveMaxRFUButRawDataBelow, 2088, "smOsirisAboveMaxRFUButRawDataBelow")
PERSISTENT_DEFINITION (smRawDataAboveMaxRFUButOsirisBelow, 2089, "smRawDataAboveMaxRFUButOsirisBelow")
PERSISTENT_DEFINITION (smStage1Successful, 2090, "smStage1Successful")
PERSISTENT_DEFINITION (smSignalIsCtrlPeak, 2091, "smSignalIsCtrlPeak")
PERSISTENT_DEFINITION (smLocusTooFewPeaks, 2093, "smLocusTooFewPeaks")
PERSISTENT_DEFINITION (smInterlocusLadderPeak, 2094, "smInterlocusLadderPeak")
PERSISTENT_DEFINITION (smUnexpectedLadderLocusPeak, 2095, "smUnexpectedLadderLocusPeak")
PERSISTENT_DEFINITION (smPoorPeakMorphology, 2097, "smPoorPeakMorphology")
PERSISTENT_DEFINITION (smPoorLocusMorphology, 2098, "smPoorLocusMorphology")
PERSISTENT_DEFINITION (smLocusMayHaveUnreportedOLs, 2099, "smLocusMayHaveUnreportedOLs")
PERSISTENT_DEFINITION (smNPeaksForLocusUnavailable, 2100, "smNPeaksForLocusUnavailable")
PERSISTENT_DEFINITION (smRelativeHeightImbalance, 2101, "smRelativeHeightImbalance")
PERSISTENT_DEFINITION (smHeightBelowFractionalFilter, 2102, "smHeightBelowFractionalFilter")
PERSISTENT_DEFINITION (smPartOfExtendedLocusLeft, 2103, "smPartOfExtendedLocusLeft")
PERSISTENT_DEFINITION (smPartOfExtendedLocusRight, 2104, "smPartOfExtendedLocusRight")
PERSISTENT_DEFINITION (smBelowFractionalFilterLeft, 2105, "smBelowFractionalFilterLeft")
PERSISTENT_DEFINITION (smBelowFractionalFilterRight, 2106, "smBelowFractionalFilterRight")
PERSISTENT_DEFINITION (smStage2Successful, 2107, "smStage2Successful")
PERSISTENT_DEFINITION (smPullUpAtControlPeak, 2108, "smPullUpAtControlPeak")
PERSISTENT_DEFINITION (smInterlocusPullUpWithPoorFit, 2109, "smInterlocusPullUpWithPoorFit")
PERSISTENT_DEFINITION (smPossibleValidInterlocusLeft, 2110, "smPossibleValidInterlocusLeft")
PERSISTENT_DEFINITION (smPossibleValidInterlocusRight, 2111, "smPossibleValidInterlocusRight")
PERSISTENT_DEFINITION (smUnassignedInterlocusPeak, 2112, "smUnassignedInterlocusPeak")
PERSISTENT_DEFINITION (smAmbiguousInterlocus, 2113, "smAmbiguousInterlocus")
PERSISTENT_DEFINITION (smTooLowInterlocusPeak, 2114, "smTooLowInterlocusPeak")
PERSISTENT_DEFINITION (smPossibleInterlocusArtifactLeft, 2115, "smPossibleInterlocusArtifactLeft")
PERSISTENT_DEFINITION (smPossibleInterlocusArtifactRight, 2116, "smPossibleInterlocusArtifactRight")
PERSISTENT_DEFINITION (smFirstAlleleEqFirstLadderPeak, 2119, "smFirstAlleleEqFirstLadderPeak")
PERSISTENT_DEFINITION (smSignalOL, 2120, "smSignalOL")
PERSISTENT_DEFINITION (smAMELMisaligned, 2121, "smAMELMisaligned")
PERSISTENT_DEFINITION (smCrater, 2122, "smCrater")
PERSISTENT_DEFINITION (smAcceptedOLLeft, 2124, "smAcceptedOLLeft")
PERSISTENT_DEFINITION (smAcceptedOLRight, 2125, "smAcceptedOLRight")
PERSISTENT_DEFINITION (smStage3Successful, 2126, "smStage3Successful")
PERSISTENT_DEFINITION (smValidOLInterlocusLeft, 2127, "smValidOLInterlocusLeft")
PERSISTENT_DEFINITION (smValidOLInterlocusRight, 2128, "smValidOLInterlocusRight")
PERSISTENT_DEFINITION (smAcceptedInterlocusLeft, 2129, "smAcceptedInterlocusLeft")
PERSISTENT_DEFINITION (smAcceptedInterlocusRight, 2130, "smAcceptedInterlocusRight")
PERSISTENT_DEFINITION (smNoXForAMEL, 2131, "smNoXForAMEL")
PERSISTENT_DEFINITION (smSignalIsAllele, 2132, "smSignalIsAllele")
PERSISTENT_DEFINITION (smPosCtrlLocusMismatch, 2133, "smPosCtrlLocusMismatch")
PERSISTENT_DEFINITION (smNoGenotypeFound, 2134, "smNoGenotypeFound")
PERSISTENT_DEFINITION (smThreeOrMoreAlleles, 2135, "smThreeOrMoreAlleles")
PERSISTENT_DEFINITION (smTwoOrMoreAlleles, 2137, "smTwoOrMoreAlleles")
PERSISTENT_DEFINITION (smHeterozygousImbalance, 2138, "smHeterozygousImbalance")
PERSISTENT_DEFINITION (smOLAlleles, 2139, "smOLAlleles")
PERSISTENT_DEFINITION (smHomozygoteHeightProblem, 2140, "smHomozygoteHeightProblem")
PERSISTENT_DEFINITION (smPullUpAtAllele, 2141, "smPullUpAtAllele")
PERSISTENT_DEFINITION (smPosCtrlLocusNotFound, 2152, "smPosCtrlLocusNotFound")
PERSISTENT_DEFINITION (smBPResidualTooLarge, 2153, "smBPResidualTooLarge")
PERSISTENT_DEFINITION (smExtraLadderAllele, 2154, "smExtraLadderAllele")
PERSISTENT_DEFINITION (smCriticalMessagesAtILS, 2155, "smCriticalMessagesAtILS")
PERSISTENT_DEFINITION (smAMELHasExtraneousPeaks, 2156, "smAMELHasExtraneousPeaks")
PERSISTENT_DEFINITION (smExtraneousAMELPeak, 2157, "smExtraneousAMELPeak")
PERSISTENT_DEFINITION (smNegCtrlChannelHasNoPrimerPeaks, 2158, "smNegCtrlChannelHasNoPrimerPeaks")
PERSISTENT_DEFINITION (smMinimumHeightForPrimerPeaks, 2159, "smMinimumHeightForPrimerPeaks")
PERSISTENT_DEFINITION (smInterlocusPeaksToRight, 2160, "smInterlocusPeaksToRight")
PERSISTENT_DEFINITION (smInterlocusPeaksToLeft, 2161, "smInterlocusPeaksToLeft")
PERSISTENT_DEFINITION (smLadderFailed, 2163, "smLadderFailed")
PERSISTENT_DEFINITION (smSampleUnanalyzable, 2164, "smSampleUnanalyzable")
PERSISTENT_DEFINITION (smCriticalMessagesAtAlleles, 2166, "smCriticalMessagesAtAlleles")
PERSISTENT_DEFINITION (smHeightBelowPullupFractionalFilter, 2167, "smHeightBelowPullupFractionalFilter")
PERSISTENT_DEFINITION (smBelowPullupFractionalFilterLeft, 2168, "smBelowPullupFractionalFilterLeft")
PERSISTENT_DEFINITION (smBelowPullupFractionalFilterRight, 2169, "smBelowPullupFractionalFilterRight")
PERSISTENT_DEFINITION (smPossibleExtraLocusAllele, 2170, "smPossibleExtraLocusAllele")
PERSISTENT_DEFINITION (smUnexpectedPeakInNegativeControl, 2172, "smUnexpectedPeakInNegativeControl")
PERSISTENT_DEFINITION (smLocusContainsPeaksWithExcessiveResiduals, 2174, "smLocusContainsPeaksWithExcessiveResiduals")
PERSISTENT_DEFINITION (smPrimaryInterchannelLink, 2176, "smPrimaryInterchannelLink")
PERSISTENT_DEFINITION (smNotInterchannelLink, 2177, "smNotInterchannelLink")
PERSISTENT_DEFINITION (smHeightBelowInterlocusMinRFU, 2178, "smHeightBelowInterlocusMinRFU")
PERSISTENT_DEFINITION (smFSAChannelUnreadable, 2180, "smFSAChannelUnreadable")
PERSISTENT_DEFINITION (smAmbiguousInterlocusCrater, 2181, "smAmbiguousInterlocusCrater")
PERSISTENT_DEFINITION (smPossibleAmbiguousInterlocusAssignedToLocus, 2182, "smPossibleAmbiguousInterlocusAssignedToLocus")
PERSISTENT_DEFINITION (smWouldCauseHeterozygousImbalanceLeft, 2500, "smWouldCauseHeterozygousImbalanceLeft")
PERSISTENT_DEFINITION (smWouldCauseHeterozygousImbalanceRight, 2501, "smWouldCauseHeterozygousImbalanceRight")
PERSISTENT_DEFINITION (smSignalOffGridLeft, 2502, "smSignalOffGridLeft")
PERSISTENT_DEFINITION (smSignalOffGridRight, 2503, "smSignalOffGridRight")
PERSISTENT_DEFINITION (sm0UnambiguousPeaksLeft, 2506, "sm0UnambiguousPeaksLeft")
PERSISTENT_DEFINITION (sm0UnambiguousPeaksRight, 2507, "sm0UnambiguousPeaksRight")
PERSISTENT_DEFINITION (sm1UnambiguousPeakLeft, 2508, "sm1UnambiguousPeakLeft")
PERSISTENT_DEFINITION (sm1UnambiguousPeakRight, 2509, "sm1UnambiguousPeakRight")
PERSISTENT_DEFINITION (sm2PlusUnambiguousPeaksLeft, 2510, "sm2PlusUnambiguousPeaksLeft")
PERSISTENT_DEFINITION (sm2PlusUnambiguousPeaksRight, 2511, "sm2PlusUnambiguousPeaksRight")
PERSISTENT_DEFINITION (sm0AmbiguousPeaksLeft, 2512, "sm0AmbiguousPeaksLeft")
PERSISTENT_DEFINITION (sm0AmbiguousPeaksRight, 2513, "sm0AmbiguousPeaksRight")
PERSISTENT_DEFINITION (sm1AmbiguousPeakLeft, 2514, "sm1AmbiguousPeakLeft")
PERSISTENT_DEFINITION (sm1AmbiguousPeakRight, 2515, "sm1AmbiguousPeakRight")
PERSISTENT_DEFINITION (sm2PlusAmbiguousPeaksLeft, 2516, "sm2PlusAmbiguousPeaksLeft")
PERSISTENT_DEFINITION (sm2PlusAmbiguousPeaksRight, 2517, "sm2PlusAmbiguousPeaksRight")
PERSISTENT_DEFINITION (smCouldBelongToLocusLeft, 2518, "smCouldBelongToLocusLeft")
PERSISTENT_DEFINITION (smCouldBelongToLocusRight, 2519, "smCouldBelongToLocusRight")
PERSISTENT_DEFINITION (smChannelIsNegControl, 2520, "smChannelIsNegControl")
PERSISTENT_DEFINITION (smLocusIsNegControl, 2521, "smLocusIsNegControl")
PERSISTENT_DEFINITION (smChannelIsPosControl, 2522, "smChannelIsPosControl")
PERSISTENT_DEFINITION (smLocusIsPosControl, 2523, "smLocusIsPosControl")
PERSISTENT_DEFINITION (smPurePullup, 2525, "smPurePullup")
PERSISTENT_DEFINITION (smChannelPeaksHaveCriticalArtifacts, 2526, "smChannelPeaksHaveCriticalArtifacts")
PERSISTENT_DEFINITION (smPreliminaryLocusHas2OrMoreAlleles, 2527, "smPreliminaryLocusHas2OrMoreAlleles")
PERSISTENT_DEFINITION (smCraterSidePeak, 2528, "smCraterSidePeak")
PERSISTENT_DEFINITION (smSignalNotACrater, 2529, "smSignalNotACrater")
PERSISTENT_DEFINITION (smSignalNotACraterSidePeak, 2530, "smSignalNotACraterSidePeak")
PERSISTENT_DEFINITION (smLocusIsHomozygous, 2531, "smLocusIsHomozygous")
PERSISTENT_DEFINITION (smLocusMissingGenotype, 2532, "smLocusMissingGenotype")
PERSISTENT_DEFINITION (smStage4Successful, 2536, "smStage4Successful")
PERSISTENT_DEFINITION (smStage5Successful, 2537, "smStage5Successful")
PERSISTENT_DEFINITION (smPeakInCoreLadderLocus, 2538, "smPeakInCoreLadderLocus")
PERSISTENT_DEFINITION (smPeakAboveOverloadLimit, 2539, "smPeakAboveOverloadLimit")
PERSISTENT_DEFINITION (smRestrictedPriorityPeak, 2540, "smRestrictedPriorityPeak")
PERSISTENT_DEFINITION (smChannelIsILS, 2541, "smChannelIsILS")
PERSISTENT_DEFINITION (smILSSpacingIsMarginal, 2542, "smILSSpacingIsMarginal")
PERSISTENT_DEFINITION (smPoorPeakMorphologyOrResolution, 2543, "smPoorPeakMorphologyOrResolution")
PERSISTENT_DEFINITION (smPeakToRightOfILS, 2544, "smPeakToRightOfILS")
PERSISTENT_DEFINITION (smPreliminaryLocusHasNoAlleles, 2545, "smPreliminaryLocusHasNoAlleles")
PERSISTENT_DEFINITION (smLocusHasPeaksBetweenDetectionAndAnalysisThresholds, 2546, "smLocusHasPeaksBetweenDetectionAndAnalysisThresholds")
PERSISTENT_DEFINITION (smUnexpectedNoOfGridPeaks, 2547, "smUnexpectedNoOfGridPeaks")
PERSISTENT_DEFINITION (smMoreThanThreeAlleles, 2548, "smMoreThanThreeAlleles")
PERSISTENT_DEFINITION (smDefaultsAreOverridden, 2549, "smDefaultsAreOverridden")
PERSISTENT_DEFINITION (smAssociatedLadderIsCritical, 2553, "smAssociatedLadderIsCritical")
PERSISTENT_DEFINITION (smAssociatedLadderLocusIsCritical, 2554, "smAssociatedLadderLocusIsCritical")
PERSISTENT_DEFINITION (smChannelHasRaisedBaseline, 2555, "smChannelHasRaisedBaseline")
PERSISTENT_DEFINITION (smChannelHasExcessiveNoiseAboveAnalysisThreshold, 2556, "smChannelHasExcessiveNoiseAboveAnalysisThreshold")
PERSISTENT_DEFINITION (smChannelHasExcessiveNoiseAboveDetectionThreshold, 2557, "smChannelHasExcessiveNoiseAboveDetectionThreshold")
PERSISTENT_DEFINITION (smSampleChannelRaisedBaselineThreshold, 2558, "smSampleChannelRaisedBaselineThreshold")
PERSISTENT_DEFINITION (smSampleILSChannelRaisedBaselineThreshold, 2559, "smSampleILSChannelRaisedBaselineThreshold")
PERSISTENT_DEFINITION (smILSPeakMayBeIncorrect, 2560, "smILSPeakMayBeIncorrect")
PERSISTENT_DEFINITION (smTestForColorCorrectionMatrixPreset, 2561, "smTestForColorCorrectionMatrixPreset")
PERSISTENT_DEFINITION (smColorCorrectionMatrixExpectedButNotFound, 2562, "smColorCorrectionMatrixExpectedButNotFound")
PERSISTENT_DEFINITION (smColorCorrectionMatrixWrongSize, 2563, "smColorCorrectionMatrixWrongSize")
PERSISTENT_DEFINITION (smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing, 2564, "smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing")
PERSISTENT_DEFINITION (smMinimumNumberOfPrimerPeaksForNegativeControlChannel, 2565, "smMinimumNumberOfPrimerPeaksForNegativeControlChannel")
PERSISTENT_DEFINITION (smUseSampleNamesForControlSampleTestsPreset, 2566, "smUseSampleNamesForControlSampleTestsPreset")
PERSISTENT_DEFINITION (smNoYForAMEL, 2567, "smNoYForAMEL")
PERSISTENT_DEFINITION (smLocusIsYLinked, 2568, "smLocusIsYLinked")
PERSISTENT_DEFINITION (smNumberAllelesBelowExpectation, 2569, "smNumberAllelesBelowExpectation")
PERSISTENT_DEFINITION (smBaselineRelativeStutter, 2570, "smBaselineRelativeStutter")
PERSISTENT_DEFINITION (smBaselineRelativeAdenylation, 2571, "smBaselineRelativeAdenylation")
PERSISTENT_DEFINITION (smBaselineRelativeBelowMinRFU, 2574, "smBaselineRelativeBelowMinRFU")
PERSISTENT_DEFINITION (smBaselineRelativeBelowMinInterlocusRFU, 2575, "smBaselineRelativeBelowMinInterlocusRFU")
PERSISTENT_DEFINITION (smTestRelativeBaselinePreset, 2576, "smTestRelativeBaselinePreset")
PERSISTENT_DEFINITION (smIgnoreNegativeRelativeBaselinePreset, 2577, "smIgnoreNegativeRelativeBaselinePreset")
PERSISTENT_DEFINITION (smNormalizeRawDataRelativeToBaselinePreset, 2578, "smNormalizeRawDataRelativeToBaselinePreset")
PERSISTENT_DEFINITION (smPercentOfStandardNoiseThreshold, 2579, "smPercentOfStandardNoiseThreshold")
PERSISTENT_DEFINITION (smLadderFitThreshold, 2580, "smLadderFitThreshold")
PERSISTENT_DEFINITION (smSampleToLadderFitBelowExpectations, 2581, "smSampleToLadderFitBelowExpectations")
PERSISTENT_DEFINITION (smEnableRawDataFilterForNormalizationPreset, 2582, "smEnableRawDataFilterForNormalizationPreset")
PERSISTENT_DEFINITION (smBaselineEstimationThreshold, 2583, "smBaselineEstimationThreshold")
PERSISTENT_DEFINITION (smSinglePassFilterWindowWidth, 2584, "smSinglePassFilterWindowWidth")
PERSISTENT_DEFINITION (smAmbiguousInterlocusWithPoorMorphology, 2585, "smAmbiguousInterlocusWithPoorMorphology")
PERSISTENT_DEFINITION (smSigmoidalPullup, 2586, "smSigmoidalPullup")
PERSISTENT_DEFINITION (smMinPrimaryPullupThreshold, 2587, "smMinPrimaryPullupThreshold")
PERSISTENT_DEFINITION (smExcessiveResidualDisplacement, 2588, "smExcessiveResidualDisplacement")
PERSISTENT_DEFINITION (smResidualDisplacementThreshold, 2589, "smResidualDisplacementThreshold")
PERSISTENT_DEFINITION (smResidualDisplacementTestPreset, 2590, "smResidualDisplacementTestPreset")
PERSISTENT_DEFINITION (smExcessiveResidualDisplacementLeft, 2591, "smExcessiveResidualDisplacementLeft")
PERSISTENT_DEFINITION (smExcessiveResidualDisplacementRight, 2592, "smExcessiveResidualDisplacementRight")
PERSISTENT_DEFINITION (smSampleSatisfiesPossibleMixtureIDCriteria, 2593, "smSampleSatisfiesPossibleMixtureIDCriteria")
PERSISTENT_DEFINITION (smDisableLowLevelFiltersForKnownMixturesPreset, 2594, "smDisableLowLevelFiltersForKnownMixturesPreset")
PERSISTENT_DEFINITION (smDisableStutterFilter, 2595, "smDisableStutterFilter")
PERSISTENT_DEFINITION (smDisableAdenylationFilter, 2596, "smDisableAdenylationFilter")
PERSISTENT_DEFINITION (smSelectTriplePassVsSinglePassFilterPreset, 2597, "smSelectTriplePassVsSinglePassFilterPreset")
PERSISTENT_DEFINITION (smTriplePassFilterWindowWidth, 2598, "smTriplePassFilterWindowWidth")
PERSISTENT_DEFINITION (smLadderFitThresholdUsingMinError, 2599, "smLadderFitThresholdUsingMinError")
PERSISTENT_DEFINITION (smUseMaxSecondDerivativesForSampleToLadderFit, 2600, "smUseMaxSecondDerivativesForSampleToLadderFit")
PERSISTENT_DEFINITION (smPartOfDualSignal, 2601, "smPartOfDualSignal")
PERSISTENT_DEFINITION (smNegativePeak, 2602, "smNegativePeak")
PERSISTENT_DEFINITION (smCallOnLadderAdenylationPreset, 2603, "smCallOnLadderAdenylationPreset")
PERSISTENT_DEFINITION (smSignalIsIntegralMultipleOfRepeatLeft, 2604, "smSignalIsIntegralMultipleOfRepeatLeft")
PERSISTENT_DEFINITION (smSignalIsIntegralMultipleOfRepeatRight, 2605, "smSignalIsIntegralMultipleOfRepeatRight")
PERSISTENT_DEFINITION (smILSFilterLeftShoulderPeaksPreset, 2606, "smILSFilterLeftShoulderPeaksPreset")
PERSISTENT_DEFINITION (smILSShoulderPeakProximityThreshold, 2607, "smILSShoulderPeakProximityThreshold")
PERSISTENT_DEFINITION (smILSShoulderFilterPercentThreshold, 2608, "smILSShoulderFilterPercentThreshold")
PERSISTENT_DEFINITION (smILSShoulderPeak, 2609, "smILSShoulderPeak")
PERSISTENT_DEFINITION (smLadderPeakOutsideCoreLadder, 2610, "smLadderPeakOutsideCoreLadder")
PERSISTENT_DEFINITION (smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset, 2611, "smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset")
PERSISTENT_DEFINITION (smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset, 2612, "smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset")
PERSISTENT_DEFINITION (smExtendLociEdgeToEdgePreset, 2613, "smExtendLociEdgeToEdgePreset")
PERSISTENT_DEFINITION (smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset, 2614, "smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset")
PERSISTENT_DEFINITION (smMaxILSBPForExtendedLocus, 2615, "smMaxILSBPForExtendedLocus")
PERSISTENT_DEFINITION (smIsQualityLocus, 2616, "smIsQualityLocus")
PERSISTENT_DEFINITION (smScaleILSPrimerSearchPreset, 2617, "smScaleILSPrimerSearchPreset")
PERSISTENT_DEFINITION (smNumberOfLastILSPeaksInScale, 2618, "smNumberOfLastILSPeaksInScale")
PERSISTENT_DEFINITION (smPercentLastILSPeaksInScaling, 2619, "smPercentLastILSPeaksInScaling")
PERSISTENT_DEFINITION (smLaserOffScale, 2620, "smLaserOffScale")
PERSISTENT_DEFINITION (smCalculatedPurePullup, 2621, "smCalculatedPurePullup")
PERSISTENT_DEFINITION (smPartialPullupBelowMinRFU, 2622, "smPartialPullupBelowMinRFU")
PERSISTENT_DEFINITION (smSigmoidalSidePeak, 2623, "smSigmoidalSidePeak")
PERSISTENT_DEFINITION (smDoNotCallStutterPeaksForSingleSourceSamplesPreset, 2624, "smDoNotCallStutterPeaksForSingleSourceSamplesPreset")
PERSISTENT_DEFINITION (smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset, 2625, "smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset")
PERSISTENT_DEFINITION (smCallStutterPeaksPreset, 2626, "smCallStutterPeaksPreset")
PERSISTENT_DEFINITION (smIsAcceptedOLAllele, 2627, "smIsAcceptedOLAllele")
PERSISTENT_DEFINITION (smSaveLadderILSHistoryPreset, 2628, "smSaveLadderILSHistoryPreset")
PERSISTENT_DEFINITION (smLatitudeForILSFit, 2629, "smLatitudeForILSFit")
PERSISTENT_DEFINITION (smUseLadderEndPointILSAlgorithmPreset, 2630, "smUseLadderEndPointILSAlgorithmPreset")
PERSISTENT_DEFINITION (smPlusLatitudeForLadderEndPointILSFit, 2631, "smPlusLatitudeForLadderEndPointILSFit")
PERSISTENT_DEFINITION (smApplyEnhancedShoulderFittingAlgorithmPreset, 2632, "smApplyEnhancedShoulderFittingAlgorithmPreset")
PERSISTENT_DEFINITION (smNoiseFactorForShoulderAcceptanceThreshold, 2633, "smNoiseFactorForShoulderAcceptanceThreshold")
PERSISTENT_DEFINITION (smConcaveDownAcceptanceThreshold, 2634, "smConcaveDownAcceptanceThreshold")
PERSISTENT_DEFINITION (smCorePeakSharesAlleleBin, 2635, "smCorePeakSharesAlleleBin")
PERSISTENT_DEFINITION (smMinImbalanceThresholdForCreatingNoisyPeak, 2636, "smMinImbalanceThresholdForCreatingNoisyPeak")
PERSISTENT_DEFINITION (smPeakSharesAlleleBinLeft, 2637, "smPeakSharesAlleleBinLeft")
PERSISTENT_DEFINITION (smPeakSharesAlleleBinRight, 2638, "smPeakSharesAlleleBinRight")
PERSISTENT_DEFINITION (smConstrainPullupPatternAnalysisPreset, 2639, "smConstrainPullupPatternAnalysisPreset")
PERSISTENT_DEFINITION (smOverrideNoiseLevelPercentsInSmoothing, 2640, "smOverrideNoiseLevelPercentsInSmoothing")
PERSISTENT_DEFINITION (smPercentOverrideOfNoiseThresholdForNormalizationPhase, 2641, "smPercentOverrideOfNoiseThresholdForNormalizationPhase")
PERSISTENT_DEFINITION (smPercentOverrideOfNoiseThresholdForFinalPhase, 2642, "smPercentOverrideOfNoiseThresholdForFinalPhase")
PERSISTENT_DEFINITION (smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset, 2643, "smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset")
PERSISTENT_DEFINITION (smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset, 2644, "smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset")
PERSISTENT_DEFINITION (smDisplaySigmoidalPeaksPreset, 2645, "smDisplaySigmoidalPeaksPreset")
PERSISTENT_DEFINITION (smResidualDisplacementFractionalLimit, 2646, "smResidualDisplacementFractionalLimit")
PERSISTENT_DEFINITION (smSelectAveragingInPlaceFilterPreset, 2647, "smSelectAveragingInPlaceFilterPreset")
PERSISTENT_DEFINITION (smAveragingInPlaceFilterWindowWidth, 2648, "smAveragingInPlaceFilterWindowWidth")
PERSISTENT_DEFINITION (smUseProximityToPeaksOnlyPreset, 2649, "smUseProximityToPeaksOnlyPreset")
PERSISTENT_DEFINITION (smDistanceFromPeakThreshold, 2650, "smDistanceFromPeakThreshold")
PERSISTENT_DEFINITION (smUseProximityBothToPeaksAndLevelChangePreset, 2651, "smUseProximityBothToPeaksAndLevelChangePreset")
PERSISTENT_DEFINITION (smDistanceFromLevelChange, 2652, "smDistanceFromLevelChange")
PERSISTENT_DEFINITION (smPostPrimerPercentOfNoiseRangeForLevelChange, 2653, "smPostPrimerPercentOfNoiseRangeForLevelChange")
PERSISTENT_DEFINITION (smMaxPercentLevelChangeToUseLevelChangeProximity, 2654, "smMaxPercentLevelChangeToUseLevelChangeProximity")
PERSISTENT_DEFINITION (smPostPrimerPercentOfNoiseRangeToBeConsideredPeak, 2655, "smPostPrimerPercentOfNoiseRangeToBeConsideredPeak")
PERSISTENT_DEFINITION (smILSBPForEndOfPrimerPeaks, 2656, "smILSBPForEndOfPrimerPeaks")
PERSISTENT_DEFINITION (smPrePrimerPercentOfNoiseRangeToBeConsideredPeak, 2657, "smPrePrimerPercentOfNoiseRangeToBeConsideredPeak")
PERSISTENT_DEFINITION (smPrePrimerPercentOfNoiseRangeForLevelChange, 2658, "smPrePrimerPercentOfNoiseRangeForLevelChange")
PERSISTENT_DEFINITION (smWeakPrimaryInterchannelLink, 2659, "smWeakPrimaryInterchannelLink")
PERSISTENT_DEFINITION (smZeroPullupPrimaryInterchannelLink, 2660, "smZeroPullupPrimaryInterchannelLink")
PERSISTENT_DEFINITION (smRawDataPrimaryInterchannelLink, 2661, "smRawDataPrimaryInterchannelLink")
PERSISTENT_DEFINITION (smThreePointPeak, 2662, "smThreePointPeak")
PERSISTENT_DEFINITION (smIncludeNonStandardStutterInStutterOverlapPreset, 2663, "smIncludeNonStandardStutterInStutterOverlapPreset")
PERSISTENT_DEFINITION (smNoNegativeDataPreset, 2664, "smNoNegativeDataPreset")
PERSISTENT_DEFINITION (smTestPullupCorrectedHeightsPreset, 2665, "smTestPullupCorrectedHeightsPreset")
PERSISTENT_DEFINITION (smTailHeightFittingThresholdFactor, 2666, "smTailHeightFittingThresholdFactor")
PERSISTENT_DEFINITION (smTailSlopeFittingThresholdFactor, 2667, "smTailSlopeFittingThresholdFactor")
PERSISTENT_DEFINITION (smSelectUserSpecifiedMinRFUForPrimaryPeakPreset, 2668, "smSelectUserSpecifiedMinRFUForPrimaryPeakPreset")
PERSISTENT_DEFINITION (smRedundantPeak, 2669, "smRedundantPeak")




smILSFailed :: smILSFailed () : SmartNotice () {

}


smILSFailed :: smILSFailed (const smILSFailed& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSFailed :: ~smILSFailed () {

}


int smILSFailed :: GetSubject () const {

	return smILSFailed::sSubject;
}


void smILSFailed :: SetIndexAndScope (int index, int scope) const {

	smILSFailed::sMessageIndex = index;
	smILSFailed::sMessageScope = scope;
}


int smILSFailed :: GetMessageIndex () const {

	return smILSFailed :: sMessageIndex;
}


int smILSFailed :: GetScope () const {

	return smILSFailed :: sMessageScope;
}


RGString smILSFailed :: GetName () const {

	return smILSFailed :: sName;
}



int smILSFailed :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSFailed* noticeType = new smILSFailed;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusIsAMEL :: smLocusIsAMEL () : SmartNotice () {

}


smLocusIsAMEL :: smLocusIsAMEL (const smLocusIsAMEL& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusIsAMEL :: ~smLocusIsAMEL () {

}


int smLocusIsAMEL :: GetSubject () const {

	return smLocusIsAMEL::sSubject;
}


void smLocusIsAMEL :: SetIndexAndScope (int index, int scope) const {

	smLocusIsAMEL::sMessageIndex = index;
	smLocusIsAMEL::sMessageScope = scope;
}


int smLocusIsAMEL :: GetMessageIndex () const {

	return smLocusIsAMEL :: sMessageIndex;
}


int smLocusIsAMEL :: GetScope () const {

	return smLocusIsAMEL :: sMessageScope;
}


RGString smLocusIsAMEL :: GetName () const {

	return smLocusIsAMEL :: sName;
}



int smLocusIsAMEL :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusIsAMEL* noticeType = new smLocusIsAMEL;
	warehouse->AddType (noticeType);
	return 1;
}





smSampleIsLadder :: smSampleIsLadder () : SmartNotice () {

}


smSampleIsLadder :: smSampleIsLadder (const smSampleIsLadder& note) : SmartNotice ((const SmartNotice&) note) {

}



smSampleIsLadder :: ~smSampleIsLadder () {

}


int smSampleIsLadder :: GetSubject () const {

	return smSampleIsLadder::sSubject;
}


void smSampleIsLadder :: SetIndexAndScope (int index, int scope) const {

	smSampleIsLadder::sMessageIndex = index;
	smSampleIsLadder::sMessageScope = scope;
}


int smSampleIsLadder :: GetMessageIndex () const {

	return smSampleIsLadder :: sMessageIndex;
}


int smSampleIsLadder :: GetScope () const {

	return smSampleIsLadder :: sMessageScope;
}


RGString smSampleIsLadder :: GetName () const {

	return smSampleIsLadder :: sName;
}



int smSampleIsLadder :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSampleIsLadder* noticeType = new smSampleIsLadder;
	warehouse->AddType (noticeType);
	return 1;
}





smSampleIsPosCtrl :: smSampleIsPosCtrl () : SmartNotice () {

}


smSampleIsPosCtrl :: smSampleIsPosCtrl (const smSampleIsPosCtrl& note) : SmartNotice ((const SmartNotice&) note) {

}



smSampleIsPosCtrl :: ~smSampleIsPosCtrl () {

}


int smSampleIsPosCtrl :: GetSubject () const {

	return smSampleIsPosCtrl::sSubject;
}


void smSampleIsPosCtrl :: SetIndexAndScope (int index, int scope) const {

	smSampleIsPosCtrl::sMessageIndex = index;
	smSampleIsPosCtrl::sMessageScope = scope;
}


int smSampleIsPosCtrl :: GetMessageIndex () const {

	return smSampleIsPosCtrl :: sMessageIndex;
}


int smSampleIsPosCtrl :: GetScope () const {

	return smSampleIsPosCtrl :: sMessageScope;
}


RGString smSampleIsPosCtrl :: GetName () const {

	return smSampleIsPosCtrl :: sName;
}



int smSampleIsPosCtrl :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSampleIsPosCtrl* noticeType = new smSampleIsPosCtrl;
	warehouse->AddType (noticeType);
	return 1;
}





smSampleIsNegCtrl :: smSampleIsNegCtrl () : SmartNotice () {

}


smSampleIsNegCtrl :: smSampleIsNegCtrl (const smSampleIsNegCtrl& note) : SmartNotice ((const SmartNotice&) note) {

}



smSampleIsNegCtrl :: ~smSampleIsNegCtrl () {

}


int smSampleIsNegCtrl :: GetSubject () const {

	return smSampleIsNegCtrl::sSubject;
}


void smSampleIsNegCtrl :: SetIndexAndScope (int index, int scope) const {

	smSampleIsNegCtrl::sMessageIndex = index;
	smSampleIsNegCtrl::sMessageScope = scope;
}


int smSampleIsNegCtrl :: GetMessageIndex () const {

	return smSampleIsNegCtrl :: sMessageIndex;
}


int smSampleIsNegCtrl :: GetScope () const {

	return smSampleIsNegCtrl :: sMessageScope;
}


RGString smSampleIsNegCtrl :: GetName () const {

	return smSampleIsNegCtrl :: sName;
}



int smSampleIsNegCtrl :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSampleIsNegCtrl* noticeType = new smSampleIsNegCtrl;
	warehouse->AddType (noticeType);
	return 1;
}





smILSSpacingFailed :: smILSSpacingFailed () : SmartNotice () {

}


smILSSpacingFailed :: smILSSpacingFailed (const smILSSpacingFailed& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSSpacingFailed :: ~smILSSpacingFailed () {

}


int smILSSpacingFailed :: GetSubject () const {

	return smILSSpacingFailed::sSubject;
}


void smILSSpacingFailed :: SetIndexAndScope (int index, int scope) const {

	smILSSpacingFailed::sMessageIndex = index;
	smILSSpacingFailed::sMessageScope = scope;
}


int smILSSpacingFailed :: GetMessageIndex () const {

	return smILSSpacingFailed :: sMessageIndex;
}


int smILSSpacingFailed :: GetScope () const {

	return smILSSpacingFailed :: sMessageScope;
}


RGString smILSSpacingFailed :: GetName () const {

	return smILSSpacingFailed :: sName;
}



int smILSSpacingFailed :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSSpacingFailed* noticeType = new smILSSpacingFailed;
	warehouse->AddType (noticeType);
	return 1;
}





smNoLadderFile :: smNoLadderFile () : SmartNotice () {

}


smNoLadderFile :: smNoLadderFile (const smNoLadderFile& note) : SmartNotice ((const SmartNotice&) note) {

}



smNoLadderFile :: ~smNoLadderFile () {

}


int smNoLadderFile :: GetSubject () const {

	return smNoLadderFile::sSubject;
}


void smNoLadderFile :: SetIndexAndScope (int index, int scope) const {

	smNoLadderFile::sMessageIndex = index;
	smNoLadderFile::sMessageScope = scope;
}


int smNoLadderFile :: GetMessageIndex () const {

	return smNoLadderFile :: sMessageIndex;
}


int smNoLadderFile :: GetScope () const {

	return smNoLadderFile :: sMessageScope;
}


RGString smNoLadderFile :: GetName () const {

	return smNoLadderFile :: sName;
}



int smNoLadderFile :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNoLadderFile* noticeType = new smNoLadderFile;
	warehouse->AddType (noticeType);
	return 1;
}





smLadderTypeUnknown :: smLadderTypeUnknown () : SmartNotice () {

}


smLadderTypeUnknown :: smLadderTypeUnknown (const smLadderTypeUnknown& note) : SmartNotice ((const SmartNotice&) note) {

}



smLadderTypeUnknown :: ~smLadderTypeUnknown () {

}


int smLadderTypeUnknown :: GetSubject () const {

	return smLadderTypeUnknown::sSubject;
}


void smLadderTypeUnknown :: SetIndexAndScope (int index, int scope) const {

	smLadderTypeUnknown::sMessageIndex = index;
	smLadderTypeUnknown::sMessageScope = scope;
}


int smLadderTypeUnknown :: GetMessageIndex () const {

	return smLadderTypeUnknown :: sMessageIndex;
}


int smLadderTypeUnknown :: GetScope () const {

	return smLadderTypeUnknown :: sMessageScope;
}


RGString smLadderTypeUnknown :: GetName () const {

	return smLadderTypeUnknown :: sName;
}



int smLadderTypeUnknown :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLadderTypeUnknown* noticeType = new smLadderTypeUnknown;
	warehouse->AddType (noticeType);
	return 1;
}





smReagentKitUnknown :: smReagentKitUnknown () : SmartNotice () {

}


smReagentKitUnknown :: smReagentKitUnknown (const smReagentKitUnknown& note) : SmartNotice ((const SmartNotice&) note) {

}



smReagentKitUnknown :: ~smReagentKitUnknown () {

}


int smReagentKitUnknown :: GetSubject () const {

	return smReagentKitUnknown::sSubject;
}


void smReagentKitUnknown :: SetIndexAndScope (int index, int scope) const {

	smReagentKitUnknown::sMessageIndex = index;
	smReagentKitUnknown::sMessageScope = scope;
}


int smReagentKitUnknown :: GetMessageIndex () const {

	return smReagentKitUnknown :: sMessageIndex;
}


int smReagentKitUnknown :: GetScope () const {

	return smReagentKitUnknown :: sMessageScope;
}


RGString smReagentKitUnknown :: GetName () const {

	return smReagentKitUnknown :: sName;
}



int smReagentKitUnknown :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smReagentKitUnknown* noticeType = new smReagentKitUnknown;
	warehouse->AddType (noticeType);
	return 1;
}





smMoreThanOneReagentKit :: smMoreThanOneReagentKit () : SmartNotice () {

}


smMoreThanOneReagentKit :: smMoreThanOneReagentKit (const smMoreThanOneReagentKit& note) : SmartNotice ((const SmartNotice&) note) {

}



smMoreThanOneReagentKit :: ~smMoreThanOneReagentKit () {

}


int smMoreThanOneReagentKit :: GetSubject () const {

	return smMoreThanOneReagentKit::sSubject;
}


void smMoreThanOneReagentKit :: SetIndexAndScope (int index, int scope) const {

	smMoreThanOneReagentKit::sMessageIndex = index;
	smMoreThanOneReagentKit::sMessageScope = scope;
}


int smMoreThanOneReagentKit :: GetMessageIndex () const {

	return smMoreThanOneReagentKit :: sMessageIndex;
}


int smMoreThanOneReagentKit :: GetScope () const {

	return smMoreThanOneReagentKit :: sMessageScope;
}


RGString smMoreThanOneReagentKit :: GetName () const {

	return smMoreThanOneReagentKit :: sName;
}



int smMoreThanOneReagentKit :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMoreThanOneReagentKit* noticeType = new smMoreThanOneReagentKit;
	warehouse->AddType (noticeType);
	return 1;
}





smCurveFitFailed :: smCurveFitFailed () : SmartNotice () {

}


smCurveFitFailed :: smCurveFitFailed (const smCurveFitFailed& note) : SmartNotice ((const SmartNotice&) note) {

}



smCurveFitFailed :: ~smCurveFitFailed () {

}


int smCurveFitFailed :: GetSubject () const {

	return smCurveFitFailed::sSubject;
}


void smCurveFitFailed :: SetIndexAndScope (int index, int scope) const {

	smCurveFitFailed::sMessageIndex = index;
	smCurveFitFailed::sMessageScope = scope;
}


int smCurveFitFailed :: GetMessageIndex () const {

	return smCurveFitFailed :: sMessageIndex;
}


int smCurveFitFailed :: GetScope () const {

	return smCurveFitFailed :: sMessageScope;
}


RGString smCurveFitFailed :: GetName () const {

	return smCurveFitFailed :: sName;
}



int smCurveFitFailed :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCurveFitFailed* noticeType = new smCurveFitFailed;
	warehouse->AddType (noticeType);
	return 1;
}





smCurveFitMarginal :: smCurveFitMarginal () : SmartNotice () {

}


smCurveFitMarginal :: smCurveFitMarginal (const smCurveFitMarginal& note) : SmartNotice ((const SmartNotice&) note) {

}



smCurveFitMarginal :: ~smCurveFitMarginal () {

}


int smCurveFitMarginal :: GetSubject () const {

	return smCurveFitMarginal::sSubject;
}


void smCurveFitMarginal :: SetIndexAndScope (int index, int scope) const {

	smCurveFitMarginal::sMessageIndex = index;
	smCurveFitMarginal::sMessageScope = scope;
}


int smCurveFitMarginal :: GetMessageIndex () const {

	return smCurveFitMarginal :: sMessageIndex;
}


int smCurveFitMarginal :: GetScope () const {

	return smCurveFitMarginal :: sMessageScope;
}


RGString smCurveFitMarginal :: GetName () const {

	return smCurveFitMarginal :: sName;
}



int smCurveFitMarginal :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCurveFitMarginal* noticeType = new smCurveFitMarginal;
	warehouse->AddType (noticeType);
	return 1;
}





smSpike :: smSpike () : SmartNotice () {

}


smSpike :: smSpike (const smSpike& note) : SmartNotice ((const SmartNotice&) note) {

}



smSpike :: ~smSpike () {

}


int smSpike :: GetSubject () const {

	return smSpike::sSubject;
}


void smSpike :: SetIndexAndScope (int index, int scope) const {

	smSpike::sMessageIndex = index;
	smSpike::sMessageScope = scope;
}


int smSpike :: GetMessageIndex () const {

	return smSpike :: sMessageIndex;
}


int smSpike :: GetScope () const {

	return smSpike :: sMessageScope;
}


RGString smSpike :: GetName () const {

	return smSpike :: sName;
}



int smSpike :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSpike* noticeType = new smSpike;
	warehouse->AddType (noticeType);
	return 1;
}





smStutter :: smStutter () : SmartNotice () {

}


smStutter :: smStutter (const smStutter& note) : SmartNotice ((const SmartNotice&) note) {

}



smStutter :: ~smStutter () {

}


int smStutter :: GetSubject () const {

	return smStutter::sSubject;
}


void smStutter :: SetIndexAndScope (int index, int scope) const {

	smStutter::sMessageIndex = index;
	smStutter::sMessageScope = scope;
}


int smStutter :: GetMessageIndex () const {

	return smStutter :: sMessageIndex;
}


int smStutter :: GetScope () const {

	return smStutter :: sMessageScope;
}


RGString smStutter :: GetName () const {

	return smStutter :: sName;
}



int smStutter :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smStutter* noticeType = new smStutter;
	warehouse->AddType (noticeType);
	return 1;
}





smAdenylation :: smAdenylation () : SmartNotice () {

}


smAdenylation :: smAdenylation (const smAdenylation& note) : SmartNotice ((const SmartNotice&) note) {

}



smAdenylation :: ~smAdenylation () {

}


int smAdenylation :: GetSubject () const {

	return smAdenylation::sSubject;
}


void smAdenylation :: SetIndexAndScope (int index, int scope) const {

	smAdenylation::sMessageIndex = index;
	smAdenylation::sMessageScope = scope;
}


int smAdenylation :: GetMessageIndex () const {

	return smAdenylation :: sMessageIndex;
}


int smAdenylation :: GetScope () const {

	return smAdenylation :: sMessageScope;
}


RGString smAdenylation :: GetName () const {

	return smAdenylation :: sName;
}



int smAdenylation :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAdenylation* noticeType = new smAdenylation;
	warehouse->AddType (noticeType);
	return 1;
}





smBlob :: smBlob () : SmartNotice () {

}


smBlob :: smBlob (const smBlob& note) : SmartNotice ((const SmartNotice&) note) {

}



smBlob :: ~smBlob () {

}


int smBlob :: GetSubject () const {

	return smBlob::sSubject;
}


void smBlob :: SetIndexAndScope (int index, int scope) const {

	smBlob::sMessageIndex = index;
	smBlob::sMessageScope = scope;
}


int smBlob :: GetMessageIndex () const {

	return smBlob :: sMessageIndex;
}


int smBlob :: GetScope () const {

	return smBlob :: sMessageScope;
}


RGString smBlob :: GetName () const {

	return smBlob :: sName;
}



int smBlob :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBlob* noticeType = new smBlob;
	warehouse->AddType (noticeType);
	return 1;
}





smPullUp :: smPullUp () : SmartNotice () {

}


smPullUp :: smPullUp (const smPullUp& note) : SmartNotice ((const SmartNotice&) note) {

}



smPullUp :: ~smPullUp () {

}


int smPullUp :: GetSubject () const {

	return smPullUp::sSubject;
}


void smPullUp :: SetIndexAndScope (int index, int scope) const {

	smPullUp::sMessageIndex = index;
	smPullUp::sMessageScope = scope;
}


int smPullUp :: GetMessageIndex () const {

	return smPullUp :: sMessageIndex;
}


int smPullUp :: GetScope () const {

	return smPullUp :: sMessageScope;
}


RGString smPullUp :: GetName () const {

	return smPullUp :: sName;
}



int smPullUp :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPullUp* noticeType = new smPullUp;
	warehouse->AddType (noticeType);
	return 1;
}





smILSTooFewPeaks :: smILSTooFewPeaks () : SmartNotice () {

}


smILSTooFewPeaks :: smILSTooFewPeaks (const smILSTooFewPeaks& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSTooFewPeaks :: ~smILSTooFewPeaks () {

}


int smILSTooFewPeaks :: GetSubject () const {

	return smILSTooFewPeaks::sSubject;
}


void smILSTooFewPeaks :: SetIndexAndScope (int index, int scope) const {

	smILSTooFewPeaks::sMessageIndex = index;
	smILSTooFewPeaks::sMessageScope = scope;
}


int smILSTooFewPeaks :: GetMessageIndex () const {

	return smILSTooFewPeaks :: sMessageIndex;
}


int smILSTooFewPeaks :: GetScope () const {

	return smILSTooFewPeaks :: sMessageScope;
}


RGString smILSTooFewPeaks :: GetName () const {

	return smILSTooFewPeaks :: sName;
}



int smILSTooFewPeaks :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSTooFewPeaks* noticeType = new smILSTooFewPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





smPeakOutsideILS :: smPeakOutsideILS () : SmartNotice () {

}


smPeakOutsideILS :: smPeakOutsideILS (const smPeakOutsideILS& note) : SmartNotice ((const SmartNotice&) note) {

}



smPeakOutsideILS :: ~smPeakOutsideILS () {

}


int smPeakOutsideILS :: GetSubject () const {

	return smPeakOutsideILS::sSubject;
}


void smPeakOutsideILS :: SetIndexAndScope (int index, int scope) const {

	smPeakOutsideILS::sMessageIndex = index;
	smPeakOutsideILS::sMessageScope = scope;
}


int smPeakOutsideILS :: GetMessageIndex () const {

	return smPeakOutsideILS :: sMessageIndex;
}


int smPeakOutsideILS :: GetScope () const {

	return smPeakOutsideILS :: sMessageScope;
}


RGString smPeakOutsideILS :: GetName () const {

	return smPeakOutsideILS :: sName;
}



int smPeakOutsideILS :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPeakOutsideILS* noticeType = new smPeakOutsideILS;
	warehouse->AddType (noticeType);
	return 1;
}





smMarkerSetNameUnknown :: smMarkerSetNameUnknown () : SmartNotice () {

}


smMarkerSetNameUnknown :: smMarkerSetNameUnknown (const smMarkerSetNameUnknown& note) : SmartNotice ((const SmartNotice&) note) {

}



smMarkerSetNameUnknown :: ~smMarkerSetNameUnknown () {

}


int smMarkerSetNameUnknown :: GetSubject () const {

	return smMarkerSetNameUnknown::sSubject;
}


void smMarkerSetNameUnknown :: SetIndexAndScope (int index, int scope) const {

	smMarkerSetNameUnknown::sMessageIndex = index;
	smMarkerSetNameUnknown::sMessageScope = scope;
}


int smMarkerSetNameUnknown :: GetMessageIndex () const {

	return smMarkerSetNameUnknown :: sMessageIndex;
}


int smMarkerSetNameUnknown :: GetScope () const {

	return smMarkerSetNameUnknown :: sMessageScope;
}


RGString smMarkerSetNameUnknown :: GetName () const {

	return smMarkerSetNameUnknown :: sName;
}



int smMarkerSetNameUnknown :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMarkerSetNameUnknown* noticeType = new smMarkerSetNameUnknown;
	warehouse->AddType (noticeType);
	return 1;
}





smNamedILSUnknown :: smNamedILSUnknown () : SmartNotice () {

}


smNamedILSUnknown :: smNamedILSUnknown (const smNamedILSUnknown& note) : SmartNotice ((const SmartNotice&) note) {

}



smNamedILSUnknown :: ~smNamedILSUnknown () {

}


int smNamedILSUnknown :: GetSubject () const {

	return smNamedILSUnknown::sSubject;
}


void smNamedILSUnknown :: SetIndexAndScope (int index, int scope) const {

	smNamedILSUnknown::sMessageIndex = index;
	smNamedILSUnknown::sMessageScope = scope;
}


int smNamedILSUnknown :: GetMessageIndex () const {

	return smNamedILSUnknown :: sMessageIndex;
}


int smNamedILSUnknown :: GetScope () const {

	return smNamedILSUnknown :: sMessageScope;
}


RGString smNamedILSUnknown :: GetName () const {

	return smNamedILSUnknown :: sName;
}



int smNamedILSUnknown :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNamedILSUnknown* noticeType = new smNamedILSUnknown;
	warehouse->AddType (noticeType);
	return 1;
}





smBelowMinRFU :: smBelowMinRFU () : SmartNotice () {

}


smBelowMinRFU :: smBelowMinRFU (const smBelowMinRFU& note) : SmartNotice ((const SmartNotice&) note) {

}



smBelowMinRFU :: ~smBelowMinRFU () {

}


int smBelowMinRFU :: GetSubject () const {

	return smBelowMinRFU::sSubject;
}


void smBelowMinRFU :: SetIndexAndScope (int index, int scope) const {

	smBelowMinRFU::sMessageIndex = index;
	smBelowMinRFU::sMessageScope = scope;
}


int smBelowMinRFU :: GetMessageIndex () const {

	return smBelowMinRFU :: sMessageIndex;
}


int smBelowMinRFU :: GetScope () const {

	return smBelowMinRFU :: sMessageScope;
}


RGString smBelowMinRFU :: GetName () const {

	return smBelowMinRFU :: sName;
}



int smBelowMinRFU :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBelowMinRFU* noticeType = new smBelowMinRFU;
	warehouse->AddType (noticeType);
	return 1;
}





smUnexpectedILSPeak :: smUnexpectedILSPeak () : SmartNotice () {

}


smUnexpectedILSPeak :: smUnexpectedILSPeak (const smUnexpectedILSPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smUnexpectedILSPeak :: ~smUnexpectedILSPeak () {

}


int smUnexpectedILSPeak :: GetSubject () const {

	return smUnexpectedILSPeak::sSubject;
}


void smUnexpectedILSPeak :: SetIndexAndScope (int index, int scope) const {

	smUnexpectedILSPeak::sMessageIndex = index;
	smUnexpectedILSPeak::sMessageScope = scope;
}


int smUnexpectedILSPeak :: GetMessageIndex () const {

	return smUnexpectedILSPeak :: sMessageIndex;
}


int smUnexpectedILSPeak :: GetScope () const {

	return smUnexpectedILSPeak :: sMessageScope;
}


RGString smUnexpectedILSPeak :: GetName () const {

	return smUnexpectedILSPeak :: sName;
}



int smUnexpectedILSPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUnexpectedILSPeak* noticeType = new smUnexpectedILSPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smNoPosCtrlFound :: smNoPosCtrlFound () : SmartNotice () {

}


smNoPosCtrlFound :: smNoPosCtrlFound (const smNoPosCtrlFound& note) : SmartNotice ((const SmartNotice&) note) {

}



smNoPosCtrlFound :: ~smNoPosCtrlFound () {

}


int smNoPosCtrlFound :: GetSubject () const {

	return smNoPosCtrlFound::sSubject;
}


void smNoPosCtrlFound :: SetIndexAndScope (int index, int scope) const {

	smNoPosCtrlFound::sMessageIndex = index;
	smNoPosCtrlFound::sMessageScope = scope;
}


int smNoPosCtrlFound :: GetMessageIndex () const {

	return smNoPosCtrlFound :: sMessageIndex;
}


int smNoPosCtrlFound :: GetScope () const {

	return smNoPosCtrlFound :: sMessageScope;
}


RGString smNoPosCtrlFound :: GetName () const {

	return smNoPosCtrlFound :: sName;
}



int smNoPosCtrlFound :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNoPosCtrlFound* noticeType = new smNoPosCtrlFound;
	warehouse->AddType (noticeType);
	return 1;
}





smNoNegCtrlFound :: smNoNegCtrlFound () : SmartNotice () {

}


smNoNegCtrlFound :: smNoNegCtrlFound (const smNoNegCtrlFound& note) : SmartNotice ((const SmartNotice&) note) {

}



smNoNegCtrlFound :: ~smNoNegCtrlFound () {

}


int smNoNegCtrlFound :: GetSubject () const {

	return smNoNegCtrlFound::sSubject;
}


void smNoNegCtrlFound :: SetIndexAndScope (int index, int scope) const {

	smNoNegCtrlFound::sMessageIndex = index;
	smNoNegCtrlFound::sMessageScope = scope;
}


int smNoNegCtrlFound :: GetMessageIndex () const {

	return smNoNegCtrlFound :: sMessageIndex;
}


int smNoNegCtrlFound :: GetScope () const {

	return smNoNegCtrlFound :: sMessageScope;
}


RGString smNoNegCtrlFound :: GetName () const {

	return smNoNegCtrlFound :: sName;
}



int smNoNegCtrlFound :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNoNegCtrlFound* noticeType = new smNoNegCtrlFound;
	warehouse->AddType (noticeType);
	return 1;
}





smNamedPosCtrlNotFound :: smNamedPosCtrlNotFound () : SmartNotice () {

}


smNamedPosCtrlNotFound :: smNamedPosCtrlNotFound (const smNamedPosCtrlNotFound& note) : SmartNotice ((const SmartNotice&) note) {

}



smNamedPosCtrlNotFound :: ~smNamedPosCtrlNotFound () {

}


int smNamedPosCtrlNotFound :: GetSubject () const {

	return smNamedPosCtrlNotFound::sSubject;
}


void smNamedPosCtrlNotFound :: SetIndexAndScope (int index, int scope) const {

	smNamedPosCtrlNotFound::sMessageIndex = index;
	smNamedPosCtrlNotFound::sMessageScope = scope;
}


int smNamedPosCtrlNotFound :: GetMessageIndex () const {

	return smNamedPosCtrlNotFound :: sMessageIndex;
}


int smNamedPosCtrlNotFound :: GetScope () const {

	return smNamedPosCtrlNotFound :: sMessageScope;
}


RGString smNamedPosCtrlNotFound :: GetName () const {

	return smNamedPosCtrlNotFound :: sName;
}



int smNamedPosCtrlNotFound :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNamedPosCtrlNotFound* noticeType = new smNamedPosCtrlNotFound;
	warehouse->AddType (noticeType);
	return 1;
}





smPeakHeightAboveMax :: smPeakHeightAboveMax () : SmartNotice () {

}


smPeakHeightAboveMax :: smPeakHeightAboveMax (const smPeakHeightAboveMax& note) : SmartNotice ((const SmartNotice&) note) {

}



smPeakHeightAboveMax :: ~smPeakHeightAboveMax () {

}


int smPeakHeightAboveMax :: GetSubject () const {

	return smPeakHeightAboveMax::sSubject;
}


void smPeakHeightAboveMax :: SetIndexAndScope (int index, int scope) const {

	smPeakHeightAboveMax::sMessageIndex = index;
	smPeakHeightAboveMax::sMessageScope = scope;
}


int smPeakHeightAboveMax :: GetMessageIndex () const {

	return smPeakHeightAboveMax :: sMessageIndex;
}


int smPeakHeightAboveMax :: GetScope () const {

	return smPeakHeightAboveMax :: sMessageScope;
}


RGString smPeakHeightAboveMax :: GetName () const {

	return smPeakHeightAboveMax :: sName;
}



int smPeakHeightAboveMax :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPeakHeightAboveMax* noticeType = new smPeakHeightAboveMax;
	warehouse->AddType (noticeType);
	return 1;
}





smCannotSeparateILSPrimerPeaks :: smCannotSeparateILSPrimerPeaks () : SmartNotice () {

}


smCannotSeparateILSPrimerPeaks :: smCannotSeparateILSPrimerPeaks (const smCannotSeparateILSPrimerPeaks& note) : SmartNotice ((const SmartNotice&) note) {

}



smCannotSeparateILSPrimerPeaks :: ~smCannotSeparateILSPrimerPeaks () {

}


int smCannotSeparateILSPrimerPeaks :: GetSubject () const {

	return smCannotSeparateILSPrimerPeaks::sSubject;
}


void smCannotSeparateILSPrimerPeaks :: SetIndexAndScope (int index, int scope) const {

	smCannotSeparateILSPrimerPeaks::sMessageIndex = index;
	smCannotSeparateILSPrimerPeaks::sMessageScope = scope;
}


int smCannotSeparateILSPrimerPeaks :: GetMessageIndex () const {

	return smCannotSeparateILSPrimerPeaks :: sMessageIndex;
}


int smCannotSeparateILSPrimerPeaks :: GetScope () const {

	return smCannotSeparateILSPrimerPeaks :: sMessageScope;
}


RGString smCannotSeparateILSPrimerPeaks :: GetName () const {

	return smCannotSeparateILSPrimerPeaks :: sName;
}



int smCannotSeparateILSPrimerPeaks :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCannotSeparateILSPrimerPeaks* noticeType = new smCannotSeparateILSPrimerPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





smILSNeighborFilterTooAggressive :: smILSNeighborFilterTooAggressive () : SmartNotice () {

}


smILSNeighborFilterTooAggressive :: smILSNeighborFilterTooAggressive (const smILSNeighborFilterTooAggressive& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSNeighborFilterTooAggressive :: ~smILSNeighborFilterTooAggressive () {

}


int smILSNeighborFilterTooAggressive :: GetSubject () const {

	return smILSNeighborFilterTooAggressive::sSubject;
}


void smILSNeighborFilterTooAggressive :: SetIndexAndScope (int index, int scope) const {

	smILSNeighborFilterTooAggressive::sMessageIndex = index;
	smILSNeighborFilterTooAggressive::sMessageScope = scope;
}


int smILSNeighborFilterTooAggressive :: GetMessageIndex () const {

	return smILSNeighborFilterTooAggressive :: sMessageIndex;
}


int smILSNeighborFilterTooAggressive :: GetScope () const {

	return smILSNeighborFilterTooAggressive :: sMessageScope;
}


RGString smILSNeighborFilterTooAggressive :: GetName () const {

	return smILSNeighborFilterTooAggressive :: sName;
}



int smILSNeighborFilterTooAggressive :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSNeighborFilterTooAggressive* noticeType = new smILSNeighborFilterTooAggressive;
	warehouse->AddType (noticeType);
	return 1;
}





smILSFractionFilterTooAggressive :: smILSFractionFilterTooAggressive () : SmartNotice () {

}


smILSFractionFilterTooAggressive :: smILSFractionFilterTooAggressive (const smILSFractionFilterTooAggressive& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSFractionFilterTooAggressive :: ~smILSFractionFilterTooAggressive () {

}


int smILSFractionFilterTooAggressive :: GetSubject () const {

	return smILSFractionFilterTooAggressive::sSubject;
}


void smILSFractionFilterTooAggressive :: SetIndexAndScope (int index, int scope) const {

	smILSFractionFilterTooAggressive::sMessageIndex = index;
	smILSFractionFilterTooAggressive::sMessageScope = scope;
}


int smILSFractionFilterTooAggressive :: GetMessageIndex () const {

	return smILSFractionFilterTooAggressive :: sMessageIndex;
}


int smILSFractionFilterTooAggressive :: GetScope () const {

	return smILSFractionFilterTooAggressive :: sMessageScope;
}


RGString smILSFractionFilterTooAggressive :: GetName () const {

	return smILSFractionFilterTooAggressive :: sName;
}



int smILSFractionFilterTooAggressive :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSFractionFilterTooAggressive* noticeType = new smILSFractionFilterTooAggressive;
	warehouse->AddType (noticeType);
	return 1;
}





smILSRelativeHeightsInconsistent :: smILSRelativeHeightsInconsistent () : SmartNotice () {

}


smILSRelativeHeightsInconsistent :: smILSRelativeHeightsInconsistent (const smILSRelativeHeightsInconsistent& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSRelativeHeightsInconsistent :: ~smILSRelativeHeightsInconsistent () {

}


int smILSRelativeHeightsInconsistent :: GetSubject () const {

	return smILSRelativeHeightsInconsistent::sSubject;
}


void smILSRelativeHeightsInconsistent :: SetIndexAndScope (int index, int scope) const {

	smILSRelativeHeightsInconsistent::sMessageIndex = index;
	smILSRelativeHeightsInconsistent::sMessageScope = scope;
}


int smILSRelativeHeightsInconsistent :: GetMessageIndex () const {

	return smILSRelativeHeightsInconsistent :: sMessageIndex;
}


int smILSRelativeHeightsInconsistent :: GetScope () const {

	return smILSRelativeHeightsInconsistent :: sMessageScope;
}


RGString smILSRelativeHeightsInconsistent :: GetName () const {

	return smILSRelativeHeightsInconsistent :: sName;
}



int smILSRelativeHeightsInconsistent :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSRelativeHeightsInconsistent* noticeType = new smILSRelativeHeightsInconsistent;
	warehouse->AddType (noticeType);
	return 1;
}





smOsirisBelowMinRFUButRawDataAbove :: smOsirisBelowMinRFUButRawDataAbove () : SmartNotice () {

}


smOsirisBelowMinRFUButRawDataAbove :: smOsirisBelowMinRFUButRawDataAbove (const smOsirisBelowMinRFUButRawDataAbove& note) : SmartNotice ((const SmartNotice&) note) {

}



smOsirisBelowMinRFUButRawDataAbove :: ~smOsirisBelowMinRFUButRawDataAbove () {

}


int smOsirisBelowMinRFUButRawDataAbove :: GetSubject () const {

	return smOsirisBelowMinRFUButRawDataAbove::sSubject;
}


void smOsirisBelowMinRFUButRawDataAbove :: SetIndexAndScope (int index, int scope) const {

	smOsirisBelowMinRFUButRawDataAbove::sMessageIndex = index;
	smOsirisBelowMinRFUButRawDataAbove::sMessageScope = scope;
}


int smOsirisBelowMinRFUButRawDataAbove :: GetMessageIndex () const {

	return smOsirisBelowMinRFUButRawDataAbove :: sMessageIndex;
}


int smOsirisBelowMinRFUButRawDataAbove :: GetScope () const {

	return smOsirisBelowMinRFUButRawDataAbove :: sMessageScope;
}


RGString smOsirisBelowMinRFUButRawDataAbove :: GetName () const {

	return smOsirisBelowMinRFUButRawDataAbove :: sName;
}



int smOsirisBelowMinRFUButRawDataAbove :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smOsirisBelowMinRFUButRawDataAbove* noticeType = new smOsirisBelowMinRFUButRawDataAbove;
	warehouse->AddType (noticeType);
	return 1;
}





smRawDataBelowMinRFUButOsirisAbove :: smRawDataBelowMinRFUButOsirisAbove () : SmartNotice () {

}


smRawDataBelowMinRFUButOsirisAbove :: smRawDataBelowMinRFUButOsirisAbove (const smRawDataBelowMinRFUButOsirisAbove& note) : SmartNotice ((const SmartNotice&) note) {

}



smRawDataBelowMinRFUButOsirisAbove :: ~smRawDataBelowMinRFUButOsirisAbove () {

}


int smRawDataBelowMinRFUButOsirisAbove :: GetSubject () const {

	return smRawDataBelowMinRFUButOsirisAbove::sSubject;
}


void smRawDataBelowMinRFUButOsirisAbove :: SetIndexAndScope (int index, int scope) const {

	smRawDataBelowMinRFUButOsirisAbove::sMessageIndex = index;
	smRawDataBelowMinRFUButOsirisAbove::sMessageScope = scope;
}


int smRawDataBelowMinRFUButOsirisAbove :: GetMessageIndex () const {

	return smRawDataBelowMinRFUButOsirisAbove :: sMessageIndex;
}


int smRawDataBelowMinRFUButOsirisAbove :: GetScope () const {

	return smRawDataBelowMinRFUButOsirisAbove :: sMessageScope;
}


RGString smRawDataBelowMinRFUButOsirisAbove :: GetName () const {

	return smRawDataBelowMinRFUButOsirisAbove :: sName;
}



int smRawDataBelowMinRFUButOsirisAbove :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smRawDataBelowMinRFUButOsirisAbove* noticeType = new smRawDataBelowMinRFUButOsirisAbove;
	warehouse->AddType (noticeType);
	return 1;
}





smOsirisAboveMaxRFUButRawDataBelow :: smOsirisAboveMaxRFUButRawDataBelow () : SmartNotice () {

}


smOsirisAboveMaxRFUButRawDataBelow :: smOsirisAboveMaxRFUButRawDataBelow (const smOsirisAboveMaxRFUButRawDataBelow& note) : SmartNotice ((const SmartNotice&) note) {

}



smOsirisAboveMaxRFUButRawDataBelow :: ~smOsirisAboveMaxRFUButRawDataBelow () {

}


int smOsirisAboveMaxRFUButRawDataBelow :: GetSubject () const {

	return smOsirisAboveMaxRFUButRawDataBelow::sSubject;
}


void smOsirisAboveMaxRFUButRawDataBelow :: SetIndexAndScope (int index, int scope) const {

	smOsirisAboveMaxRFUButRawDataBelow::sMessageIndex = index;
	smOsirisAboveMaxRFUButRawDataBelow::sMessageScope = scope;
}


int smOsirisAboveMaxRFUButRawDataBelow :: GetMessageIndex () const {

	return smOsirisAboveMaxRFUButRawDataBelow :: sMessageIndex;
}


int smOsirisAboveMaxRFUButRawDataBelow :: GetScope () const {

	return smOsirisAboveMaxRFUButRawDataBelow :: sMessageScope;
}


RGString smOsirisAboveMaxRFUButRawDataBelow :: GetName () const {

	return smOsirisAboveMaxRFUButRawDataBelow :: sName;
}



int smOsirisAboveMaxRFUButRawDataBelow :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smOsirisAboveMaxRFUButRawDataBelow* noticeType = new smOsirisAboveMaxRFUButRawDataBelow;
	warehouse->AddType (noticeType);
	return 1;
}





smRawDataAboveMaxRFUButOsirisBelow :: smRawDataAboveMaxRFUButOsirisBelow () : SmartNotice () {

}


smRawDataAboveMaxRFUButOsirisBelow :: smRawDataAboveMaxRFUButOsirisBelow (const smRawDataAboveMaxRFUButOsirisBelow& note) : SmartNotice ((const SmartNotice&) note) {

}



smRawDataAboveMaxRFUButOsirisBelow :: ~smRawDataAboveMaxRFUButOsirisBelow () {

}


int smRawDataAboveMaxRFUButOsirisBelow :: GetSubject () const {

	return smRawDataAboveMaxRFUButOsirisBelow::sSubject;
}


void smRawDataAboveMaxRFUButOsirisBelow :: SetIndexAndScope (int index, int scope) const {

	smRawDataAboveMaxRFUButOsirisBelow::sMessageIndex = index;
	smRawDataAboveMaxRFUButOsirisBelow::sMessageScope = scope;
}


int smRawDataAboveMaxRFUButOsirisBelow :: GetMessageIndex () const {

	return smRawDataAboveMaxRFUButOsirisBelow :: sMessageIndex;
}


int smRawDataAboveMaxRFUButOsirisBelow :: GetScope () const {

	return smRawDataAboveMaxRFUButOsirisBelow :: sMessageScope;
}


RGString smRawDataAboveMaxRFUButOsirisBelow :: GetName () const {

	return smRawDataAboveMaxRFUButOsirisBelow :: sName;
}



int smRawDataAboveMaxRFUButOsirisBelow :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smRawDataAboveMaxRFUButOsirisBelow* noticeType = new smRawDataAboveMaxRFUButOsirisBelow;
	warehouse->AddType (noticeType);
	return 1;
}





smStage1Successful :: smStage1Successful () : SmartNotice () {

}


smStage1Successful :: smStage1Successful (const smStage1Successful& note) : SmartNotice ((const SmartNotice&) note) {

}



smStage1Successful :: ~smStage1Successful () {

}


int smStage1Successful :: GetSubject () const {

	return smStage1Successful::sSubject;
}


void smStage1Successful :: SetIndexAndScope (int index, int scope) const {

	smStage1Successful::sMessageIndex = index;
	smStage1Successful::sMessageScope = scope;
}


int smStage1Successful :: GetMessageIndex () const {

	return smStage1Successful :: sMessageIndex;
}


int smStage1Successful :: GetScope () const {

	return smStage1Successful :: sMessageScope;
}


RGString smStage1Successful :: GetName () const {

	return smStage1Successful :: sName;
}



int smStage1Successful :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smStage1Successful* noticeType = new smStage1Successful;
	warehouse->AddType (noticeType);
	return 1;
}





smSignalIsCtrlPeak :: smSignalIsCtrlPeak () : SmartNotice () {

}


smSignalIsCtrlPeak :: smSignalIsCtrlPeak (const smSignalIsCtrlPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smSignalIsCtrlPeak :: ~smSignalIsCtrlPeak () {

}


int smSignalIsCtrlPeak :: GetSubject () const {

	return smSignalIsCtrlPeak::sSubject;
}


void smSignalIsCtrlPeak :: SetIndexAndScope (int index, int scope) const {

	smSignalIsCtrlPeak::sMessageIndex = index;
	smSignalIsCtrlPeak::sMessageScope = scope;
}


int smSignalIsCtrlPeak :: GetMessageIndex () const {

	return smSignalIsCtrlPeak :: sMessageIndex;
}


int smSignalIsCtrlPeak :: GetScope () const {

	return smSignalIsCtrlPeak :: sMessageScope;
}


RGString smSignalIsCtrlPeak :: GetName () const {

	return smSignalIsCtrlPeak :: sName;
}



int smSignalIsCtrlPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSignalIsCtrlPeak* noticeType = new smSignalIsCtrlPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusTooFewPeaks :: smLocusTooFewPeaks () : SmartNotice () {

}


smLocusTooFewPeaks :: smLocusTooFewPeaks (const smLocusTooFewPeaks& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusTooFewPeaks :: ~smLocusTooFewPeaks () {

}


int smLocusTooFewPeaks :: GetSubject () const {

	return smLocusTooFewPeaks::sSubject;
}


void smLocusTooFewPeaks :: SetIndexAndScope (int index, int scope) const {

	smLocusTooFewPeaks::sMessageIndex = index;
	smLocusTooFewPeaks::sMessageScope = scope;
}


int smLocusTooFewPeaks :: GetMessageIndex () const {

	return smLocusTooFewPeaks :: sMessageIndex;
}


int smLocusTooFewPeaks :: GetScope () const {

	return smLocusTooFewPeaks :: sMessageScope;
}


RGString smLocusTooFewPeaks :: GetName () const {

	return smLocusTooFewPeaks :: sName;
}



int smLocusTooFewPeaks :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusTooFewPeaks* noticeType = new smLocusTooFewPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





smInterlocusLadderPeak :: smInterlocusLadderPeak () : SmartNotice () {

}


smInterlocusLadderPeak :: smInterlocusLadderPeak (const smInterlocusLadderPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smInterlocusLadderPeak :: ~smInterlocusLadderPeak () {

}


int smInterlocusLadderPeak :: GetSubject () const {

	return smInterlocusLadderPeak::sSubject;
}


void smInterlocusLadderPeak :: SetIndexAndScope (int index, int scope) const {

	smInterlocusLadderPeak::sMessageIndex = index;
	smInterlocusLadderPeak::sMessageScope = scope;
}


int smInterlocusLadderPeak :: GetMessageIndex () const {

	return smInterlocusLadderPeak :: sMessageIndex;
}


int smInterlocusLadderPeak :: GetScope () const {

	return smInterlocusLadderPeak :: sMessageScope;
}


RGString smInterlocusLadderPeak :: GetName () const {

	return smInterlocusLadderPeak :: sName;
}



int smInterlocusLadderPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smInterlocusLadderPeak* noticeType = new smInterlocusLadderPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smUnexpectedLadderLocusPeak :: smUnexpectedLadderLocusPeak () : SmartNotice () {

}


smUnexpectedLadderLocusPeak :: smUnexpectedLadderLocusPeak (const smUnexpectedLadderLocusPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smUnexpectedLadderLocusPeak :: ~smUnexpectedLadderLocusPeak () {

}


int smUnexpectedLadderLocusPeak :: GetSubject () const {

	return smUnexpectedLadderLocusPeak::sSubject;
}


void smUnexpectedLadderLocusPeak :: SetIndexAndScope (int index, int scope) const {

	smUnexpectedLadderLocusPeak::sMessageIndex = index;
	smUnexpectedLadderLocusPeak::sMessageScope = scope;
}


int smUnexpectedLadderLocusPeak :: GetMessageIndex () const {

	return smUnexpectedLadderLocusPeak :: sMessageIndex;
}


int smUnexpectedLadderLocusPeak :: GetScope () const {

	return smUnexpectedLadderLocusPeak :: sMessageScope;
}


RGString smUnexpectedLadderLocusPeak :: GetName () const {

	return smUnexpectedLadderLocusPeak :: sName;
}



int smUnexpectedLadderLocusPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUnexpectedLadderLocusPeak* noticeType = new smUnexpectedLadderLocusPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smPoorPeakMorphology :: smPoorPeakMorphology () : SmartNotice () {

}


smPoorPeakMorphology :: smPoorPeakMorphology (const smPoorPeakMorphology& note) : SmartNotice ((const SmartNotice&) note) {

}



smPoorPeakMorphology :: ~smPoorPeakMorphology () {

}


int smPoorPeakMorphology :: GetSubject () const {

	return smPoorPeakMorphology::sSubject;
}


void smPoorPeakMorphology :: SetIndexAndScope (int index, int scope) const {

	smPoorPeakMorphology::sMessageIndex = index;
	smPoorPeakMorphology::sMessageScope = scope;
}


int smPoorPeakMorphology :: GetMessageIndex () const {

	return smPoorPeakMorphology :: sMessageIndex;
}


int smPoorPeakMorphology :: GetScope () const {

	return smPoorPeakMorphology :: sMessageScope;
}


RGString smPoorPeakMorphology :: GetName () const {

	return smPoorPeakMorphology :: sName;
}



int smPoorPeakMorphology :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPoorPeakMorphology* noticeType = new smPoorPeakMorphology;
	warehouse->AddType (noticeType);
	return 1;
}





smPoorLocusMorphology :: smPoorLocusMorphology () : SmartNotice () {

}


smPoorLocusMorphology :: smPoorLocusMorphology (const smPoorLocusMorphology& note) : SmartNotice ((const SmartNotice&) note) {

}



smPoorLocusMorphology :: ~smPoorLocusMorphology () {

}


int smPoorLocusMorphology :: GetSubject () const {

	return smPoorLocusMorphology::sSubject;
}


void smPoorLocusMorphology :: SetIndexAndScope (int index, int scope) const {

	smPoorLocusMorphology::sMessageIndex = index;
	smPoorLocusMorphology::sMessageScope = scope;
}


int smPoorLocusMorphology :: GetMessageIndex () const {

	return smPoorLocusMorphology :: sMessageIndex;
}


int smPoorLocusMorphology :: GetScope () const {

	return smPoorLocusMorphology :: sMessageScope;
}


RGString smPoorLocusMorphology :: GetName () const {

	return smPoorLocusMorphology :: sName;
}



int smPoorLocusMorphology :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPoorLocusMorphology* noticeType = new smPoorLocusMorphology;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusMayHaveUnreportedOLs :: smLocusMayHaveUnreportedOLs () : SmartNotice () {

}


smLocusMayHaveUnreportedOLs :: smLocusMayHaveUnreportedOLs (const smLocusMayHaveUnreportedOLs& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusMayHaveUnreportedOLs :: ~smLocusMayHaveUnreportedOLs () {

}


int smLocusMayHaveUnreportedOLs :: GetSubject () const {

	return smLocusMayHaveUnreportedOLs::sSubject;
}


void smLocusMayHaveUnreportedOLs :: SetIndexAndScope (int index, int scope) const {

	smLocusMayHaveUnreportedOLs::sMessageIndex = index;
	smLocusMayHaveUnreportedOLs::sMessageScope = scope;
}


int smLocusMayHaveUnreportedOLs :: GetMessageIndex () const {

	return smLocusMayHaveUnreportedOLs :: sMessageIndex;
}


int smLocusMayHaveUnreportedOLs :: GetScope () const {

	return smLocusMayHaveUnreportedOLs :: sMessageScope;
}


RGString smLocusMayHaveUnreportedOLs :: GetName () const {

	return smLocusMayHaveUnreportedOLs :: sName;
}



int smLocusMayHaveUnreportedOLs :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusMayHaveUnreportedOLs* noticeType = new smLocusMayHaveUnreportedOLs;
	warehouse->AddType (noticeType);
	return 1;
}





smNPeaksForLocusUnavailable :: smNPeaksForLocusUnavailable () : SmartNotice () {

}


smNPeaksForLocusUnavailable :: smNPeaksForLocusUnavailable (const smNPeaksForLocusUnavailable& note) : SmartNotice ((const SmartNotice&) note) {

}



smNPeaksForLocusUnavailable :: ~smNPeaksForLocusUnavailable () {

}


int smNPeaksForLocusUnavailable :: GetSubject () const {

	return smNPeaksForLocusUnavailable::sSubject;
}


void smNPeaksForLocusUnavailable :: SetIndexAndScope (int index, int scope) const {

	smNPeaksForLocusUnavailable::sMessageIndex = index;
	smNPeaksForLocusUnavailable::sMessageScope = scope;
}


int smNPeaksForLocusUnavailable :: GetMessageIndex () const {

	return smNPeaksForLocusUnavailable :: sMessageIndex;
}


int smNPeaksForLocusUnavailable :: GetScope () const {

	return smNPeaksForLocusUnavailable :: sMessageScope;
}


RGString smNPeaksForLocusUnavailable :: GetName () const {

	return smNPeaksForLocusUnavailable :: sName;
}



int smNPeaksForLocusUnavailable :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNPeaksForLocusUnavailable* noticeType = new smNPeaksForLocusUnavailable;
	warehouse->AddType (noticeType);
	return 1;
}





smRelativeHeightImbalance :: smRelativeHeightImbalance () : SmartNotice () {

}


smRelativeHeightImbalance :: smRelativeHeightImbalance (const smRelativeHeightImbalance& note) : SmartNotice ((const SmartNotice&) note) {

}



smRelativeHeightImbalance :: ~smRelativeHeightImbalance () {

}


int smRelativeHeightImbalance :: GetSubject () const {

	return smRelativeHeightImbalance::sSubject;
}


void smRelativeHeightImbalance :: SetIndexAndScope (int index, int scope) const {

	smRelativeHeightImbalance::sMessageIndex = index;
	smRelativeHeightImbalance::sMessageScope = scope;
}


int smRelativeHeightImbalance :: GetMessageIndex () const {

	return smRelativeHeightImbalance :: sMessageIndex;
}


int smRelativeHeightImbalance :: GetScope () const {

	return smRelativeHeightImbalance :: sMessageScope;
}


RGString smRelativeHeightImbalance :: GetName () const {

	return smRelativeHeightImbalance :: sName;
}



int smRelativeHeightImbalance :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smRelativeHeightImbalance* noticeType = new smRelativeHeightImbalance;
	warehouse->AddType (noticeType);
	return 1;
}





smHeightBelowFractionalFilter :: smHeightBelowFractionalFilter () : SmartNotice () {

}


smHeightBelowFractionalFilter :: smHeightBelowFractionalFilter (const smHeightBelowFractionalFilter& note) : SmartNotice ((const SmartNotice&) note) {

}



smHeightBelowFractionalFilter :: ~smHeightBelowFractionalFilter () {

}


int smHeightBelowFractionalFilter :: GetSubject () const {

	return smHeightBelowFractionalFilter::sSubject;
}


void smHeightBelowFractionalFilter :: SetIndexAndScope (int index, int scope) const {

	smHeightBelowFractionalFilter::sMessageIndex = index;
	smHeightBelowFractionalFilter::sMessageScope = scope;
}


int smHeightBelowFractionalFilter :: GetMessageIndex () const {

	return smHeightBelowFractionalFilter :: sMessageIndex;
}


int smHeightBelowFractionalFilter :: GetScope () const {

	return smHeightBelowFractionalFilter :: sMessageScope;
}


RGString smHeightBelowFractionalFilter :: GetName () const {

	return smHeightBelowFractionalFilter :: sName;
}



int smHeightBelowFractionalFilter :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smHeightBelowFractionalFilter* noticeType = new smHeightBelowFractionalFilter;
	warehouse->AddType (noticeType);
	return 1;
}





smPartOfExtendedLocusLeft :: smPartOfExtendedLocusLeft () : SmartNotice () {

}


smPartOfExtendedLocusLeft :: smPartOfExtendedLocusLeft (const smPartOfExtendedLocusLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smPartOfExtendedLocusLeft :: ~smPartOfExtendedLocusLeft () {

}


int smPartOfExtendedLocusLeft :: GetSubject () const {

	return smPartOfExtendedLocusLeft::sSubject;
}


void smPartOfExtendedLocusLeft :: SetIndexAndScope (int index, int scope) const {

	smPartOfExtendedLocusLeft::sMessageIndex = index;
	smPartOfExtendedLocusLeft::sMessageScope = scope;
}


int smPartOfExtendedLocusLeft :: GetMessageIndex () const {

	return smPartOfExtendedLocusLeft :: sMessageIndex;
}


int smPartOfExtendedLocusLeft :: GetScope () const {

	return smPartOfExtendedLocusLeft :: sMessageScope;
}


RGString smPartOfExtendedLocusLeft :: GetName () const {

	return smPartOfExtendedLocusLeft :: sName;
}



int smPartOfExtendedLocusLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPartOfExtendedLocusLeft* noticeType = new smPartOfExtendedLocusLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smPartOfExtendedLocusRight :: smPartOfExtendedLocusRight () : SmartNotice () {

}


smPartOfExtendedLocusRight :: smPartOfExtendedLocusRight (const smPartOfExtendedLocusRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smPartOfExtendedLocusRight :: ~smPartOfExtendedLocusRight () {

}


int smPartOfExtendedLocusRight :: GetSubject () const {

	return smPartOfExtendedLocusRight::sSubject;
}


void smPartOfExtendedLocusRight :: SetIndexAndScope (int index, int scope) const {

	smPartOfExtendedLocusRight::sMessageIndex = index;
	smPartOfExtendedLocusRight::sMessageScope = scope;
}


int smPartOfExtendedLocusRight :: GetMessageIndex () const {

	return smPartOfExtendedLocusRight :: sMessageIndex;
}


int smPartOfExtendedLocusRight :: GetScope () const {

	return smPartOfExtendedLocusRight :: sMessageScope;
}


RGString smPartOfExtendedLocusRight :: GetName () const {

	return smPartOfExtendedLocusRight :: sName;
}



int smPartOfExtendedLocusRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPartOfExtendedLocusRight* noticeType = new smPartOfExtendedLocusRight;
	warehouse->AddType (noticeType);
	return 1;
}





smBelowFractionalFilterLeft :: smBelowFractionalFilterLeft () : SmartNotice () {

}


smBelowFractionalFilterLeft :: smBelowFractionalFilterLeft (const smBelowFractionalFilterLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smBelowFractionalFilterLeft :: ~smBelowFractionalFilterLeft () {

}


int smBelowFractionalFilterLeft :: GetSubject () const {

	return smBelowFractionalFilterLeft::sSubject;
}


void smBelowFractionalFilterLeft :: SetIndexAndScope (int index, int scope) const {

	smBelowFractionalFilterLeft::sMessageIndex = index;
	smBelowFractionalFilterLeft::sMessageScope = scope;
}


int smBelowFractionalFilterLeft :: GetMessageIndex () const {

	return smBelowFractionalFilterLeft :: sMessageIndex;
}


int smBelowFractionalFilterLeft :: GetScope () const {

	return smBelowFractionalFilterLeft :: sMessageScope;
}


RGString smBelowFractionalFilterLeft :: GetName () const {

	return smBelowFractionalFilterLeft :: sName;
}



int smBelowFractionalFilterLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBelowFractionalFilterLeft* noticeType = new smBelowFractionalFilterLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smBelowFractionalFilterRight :: smBelowFractionalFilterRight () : SmartNotice () {

}


smBelowFractionalFilterRight :: smBelowFractionalFilterRight (const smBelowFractionalFilterRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smBelowFractionalFilterRight :: ~smBelowFractionalFilterRight () {

}


int smBelowFractionalFilterRight :: GetSubject () const {

	return smBelowFractionalFilterRight::sSubject;
}


void smBelowFractionalFilterRight :: SetIndexAndScope (int index, int scope) const {

	smBelowFractionalFilterRight::sMessageIndex = index;
	smBelowFractionalFilterRight::sMessageScope = scope;
}


int smBelowFractionalFilterRight :: GetMessageIndex () const {

	return smBelowFractionalFilterRight :: sMessageIndex;
}


int smBelowFractionalFilterRight :: GetScope () const {

	return smBelowFractionalFilterRight :: sMessageScope;
}


RGString smBelowFractionalFilterRight :: GetName () const {

	return smBelowFractionalFilterRight :: sName;
}



int smBelowFractionalFilterRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBelowFractionalFilterRight* noticeType = new smBelowFractionalFilterRight;
	warehouse->AddType (noticeType);
	return 1;
}





smStage2Successful :: smStage2Successful () : SmartNotice () {

}


smStage2Successful :: smStage2Successful (const smStage2Successful& note) : SmartNotice ((const SmartNotice&) note) {

}



smStage2Successful :: ~smStage2Successful () {

}


int smStage2Successful :: GetSubject () const {

	return smStage2Successful::sSubject;
}


void smStage2Successful :: SetIndexAndScope (int index, int scope) const {

	smStage2Successful::sMessageIndex = index;
	smStage2Successful::sMessageScope = scope;
}


int smStage2Successful :: GetMessageIndex () const {

	return smStage2Successful :: sMessageIndex;
}


int smStage2Successful :: GetScope () const {

	return smStage2Successful :: sMessageScope;
}


RGString smStage2Successful :: GetName () const {

	return smStage2Successful :: sName;
}



int smStage2Successful :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smStage2Successful* noticeType = new smStage2Successful;
	warehouse->AddType (noticeType);
	return 1;
}





smPullUpAtControlPeak :: smPullUpAtControlPeak () : SmartNotice () {

}


smPullUpAtControlPeak :: smPullUpAtControlPeak (const smPullUpAtControlPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smPullUpAtControlPeak :: ~smPullUpAtControlPeak () {

}


int smPullUpAtControlPeak :: GetSubject () const {

	return smPullUpAtControlPeak::sSubject;
}


void smPullUpAtControlPeak :: SetIndexAndScope (int index, int scope) const {

	smPullUpAtControlPeak::sMessageIndex = index;
	smPullUpAtControlPeak::sMessageScope = scope;
}


int smPullUpAtControlPeak :: GetMessageIndex () const {

	return smPullUpAtControlPeak :: sMessageIndex;
}


int smPullUpAtControlPeak :: GetScope () const {

	return smPullUpAtControlPeak :: sMessageScope;
}


RGString smPullUpAtControlPeak :: GetName () const {

	return smPullUpAtControlPeak :: sName;
}



int smPullUpAtControlPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPullUpAtControlPeak* noticeType = new smPullUpAtControlPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smInterlocusPullUpWithPoorFit :: smInterlocusPullUpWithPoorFit () : SmartNotice () {

}


smInterlocusPullUpWithPoorFit :: smInterlocusPullUpWithPoorFit (const smInterlocusPullUpWithPoorFit& note) : SmartNotice ((const SmartNotice&) note) {

}



smInterlocusPullUpWithPoorFit :: ~smInterlocusPullUpWithPoorFit () {

}


int smInterlocusPullUpWithPoorFit :: GetSubject () const {

	return smInterlocusPullUpWithPoorFit::sSubject;
}


void smInterlocusPullUpWithPoorFit :: SetIndexAndScope (int index, int scope) const {

	smInterlocusPullUpWithPoorFit::sMessageIndex = index;
	smInterlocusPullUpWithPoorFit::sMessageScope = scope;
}


int smInterlocusPullUpWithPoorFit :: GetMessageIndex () const {

	return smInterlocusPullUpWithPoorFit :: sMessageIndex;
}


int smInterlocusPullUpWithPoorFit :: GetScope () const {

	return smInterlocusPullUpWithPoorFit :: sMessageScope;
}


RGString smInterlocusPullUpWithPoorFit :: GetName () const {

	return smInterlocusPullUpWithPoorFit :: sName;
}



int smInterlocusPullUpWithPoorFit :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smInterlocusPullUpWithPoorFit* noticeType = new smInterlocusPullUpWithPoorFit;
	warehouse->AddType (noticeType);
	return 1;
}





smPossibleValidInterlocusLeft :: smPossibleValidInterlocusLeft () : SmartNotice () {

}


smPossibleValidInterlocusLeft :: smPossibleValidInterlocusLeft (const smPossibleValidInterlocusLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smPossibleValidInterlocusLeft :: ~smPossibleValidInterlocusLeft () {

}


int smPossibleValidInterlocusLeft :: GetSubject () const {

	return smPossibleValidInterlocusLeft::sSubject;
}


void smPossibleValidInterlocusLeft :: SetIndexAndScope (int index, int scope) const {

	smPossibleValidInterlocusLeft::sMessageIndex = index;
	smPossibleValidInterlocusLeft::sMessageScope = scope;
}


int smPossibleValidInterlocusLeft :: GetMessageIndex () const {

	return smPossibleValidInterlocusLeft :: sMessageIndex;
}


int smPossibleValidInterlocusLeft :: GetScope () const {

	return smPossibleValidInterlocusLeft :: sMessageScope;
}


RGString smPossibleValidInterlocusLeft :: GetName () const {

	return smPossibleValidInterlocusLeft :: sName;
}



int smPossibleValidInterlocusLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPossibleValidInterlocusLeft* noticeType = new smPossibleValidInterlocusLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smPossibleValidInterlocusRight :: smPossibleValidInterlocusRight () : SmartNotice () {

}


smPossibleValidInterlocusRight :: smPossibleValidInterlocusRight (const smPossibleValidInterlocusRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smPossibleValidInterlocusRight :: ~smPossibleValidInterlocusRight () {

}


int smPossibleValidInterlocusRight :: GetSubject () const {

	return smPossibleValidInterlocusRight::sSubject;
}


void smPossibleValidInterlocusRight :: SetIndexAndScope (int index, int scope) const {

	smPossibleValidInterlocusRight::sMessageIndex = index;
	smPossibleValidInterlocusRight::sMessageScope = scope;
}


int smPossibleValidInterlocusRight :: GetMessageIndex () const {

	return smPossibleValidInterlocusRight :: sMessageIndex;
}


int smPossibleValidInterlocusRight :: GetScope () const {

	return smPossibleValidInterlocusRight :: sMessageScope;
}


RGString smPossibleValidInterlocusRight :: GetName () const {

	return smPossibleValidInterlocusRight :: sName;
}



int smPossibleValidInterlocusRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPossibleValidInterlocusRight* noticeType = new smPossibleValidInterlocusRight;
	warehouse->AddType (noticeType);
	return 1;
}





smUnassignedInterlocusPeak :: smUnassignedInterlocusPeak () : SmartNotice () {

}


smUnassignedInterlocusPeak :: smUnassignedInterlocusPeak (const smUnassignedInterlocusPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smUnassignedInterlocusPeak :: ~smUnassignedInterlocusPeak () {

}


int smUnassignedInterlocusPeak :: GetSubject () const {

	return smUnassignedInterlocusPeak::sSubject;
}


void smUnassignedInterlocusPeak :: SetIndexAndScope (int index, int scope) const {

	smUnassignedInterlocusPeak::sMessageIndex = index;
	smUnassignedInterlocusPeak::sMessageScope = scope;
}


int smUnassignedInterlocusPeak :: GetMessageIndex () const {

	return smUnassignedInterlocusPeak :: sMessageIndex;
}


int smUnassignedInterlocusPeak :: GetScope () const {

	return smUnassignedInterlocusPeak :: sMessageScope;
}


RGString smUnassignedInterlocusPeak :: GetName () const {

	return smUnassignedInterlocusPeak :: sName;
}



int smUnassignedInterlocusPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUnassignedInterlocusPeak* noticeType = new smUnassignedInterlocusPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smAmbiguousInterlocus :: smAmbiguousInterlocus () : SmartNotice () {

}


smAmbiguousInterlocus :: smAmbiguousInterlocus (const smAmbiguousInterlocus& note) : SmartNotice ((const SmartNotice&) note) {

}



smAmbiguousInterlocus :: ~smAmbiguousInterlocus () {

}


int smAmbiguousInterlocus :: GetSubject () const {

	return smAmbiguousInterlocus::sSubject;
}


void smAmbiguousInterlocus :: SetIndexAndScope (int index, int scope) const {

	smAmbiguousInterlocus::sMessageIndex = index;
	smAmbiguousInterlocus::sMessageScope = scope;
}


int smAmbiguousInterlocus :: GetMessageIndex () const {

	return smAmbiguousInterlocus :: sMessageIndex;
}


int smAmbiguousInterlocus :: GetScope () const {

	return smAmbiguousInterlocus :: sMessageScope;
}


RGString smAmbiguousInterlocus :: GetName () const {

	return smAmbiguousInterlocus :: sName;
}



int smAmbiguousInterlocus :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAmbiguousInterlocus* noticeType = new smAmbiguousInterlocus;
	warehouse->AddType (noticeType);
	return 1;
}





smTooLowInterlocusPeak :: smTooLowInterlocusPeak () : SmartNotice () {

}


smTooLowInterlocusPeak :: smTooLowInterlocusPeak (const smTooLowInterlocusPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smTooLowInterlocusPeak :: ~smTooLowInterlocusPeak () {

}


int smTooLowInterlocusPeak :: GetSubject () const {

	return smTooLowInterlocusPeak::sSubject;
}


void smTooLowInterlocusPeak :: SetIndexAndScope (int index, int scope) const {

	smTooLowInterlocusPeak::sMessageIndex = index;
	smTooLowInterlocusPeak::sMessageScope = scope;
}


int smTooLowInterlocusPeak :: GetMessageIndex () const {

	return smTooLowInterlocusPeak :: sMessageIndex;
}


int smTooLowInterlocusPeak :: GetScope () const {

	return smTooLowInterlocusPeak :: sMessageScope;
}


RGString smTooLowInterlocusPeak :: GetName () const {

	return smTooLowInterlocusPeak :: sName;
}



int smTooLowInterlocusPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smTooLowInterlocusPeak* noticeType = new smTooLowInterlocusPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smPossibleInterlocusArtifactLeft :: smPossibleInterlocusArtifactLeft () : SmartNotice () {

}


smPossibleInterlocusArtifactLeft :: smPossibleInterlocusArtifactLeft (const smPossibleInterlocusArtifactLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smPossibleInterlocusArtifactLeft :: ~smPossibleInterlocusArtifactLeft () {

}


int smPossibleInterlocusArtifactLeft :: GetSubject () const {

	return smPossibleInterlocusArtifactLeft::sSubject;
}


void smPossibleInterlocusArtifactLeft :: SetIndexAndScope (int index, int scope) const {

	smPossibleInterlocusArtifactLeft::sMessageIndex = index;
	smPossibleInterlocusArtifactLeft::sMessageScope = scope;
}


int smPossibleInterlocusArtifactLeft :: GetMessageIndex () const {

	return smPossibleInterlocusArtifactLeft :: sMessageIndex;
}


int smPossibleInterlocusArtifactLeft :: GetScope () const {

	return smPossibleInterlocusArtifactLeft :: sMessageScope;
}


RGString smPossibleInterlocusArtifactLeft :: GetName () const {

	return smPossibleInterlocusArtifactLeft :: sName;
}



int smPossibleInterlocusArtifactLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPossibleInterlocusArtifactLeft* noticeType = new smPossibleInterlocusArtifactLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smPossibleInterlocusArtifactRight :: smPossibleInterlocusArtifactRight () : SmartNotice () {

}


smPossibleInterlocusArtifactRight :: smPossibleInterlocusArtifactRight (const smPossibleInterlocusArtifactRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smPossibleInterlocusArtifactRight :: ~smPossibleInterlocusArtifactRight () {

}


int smPossibleInterlocusArtifactRight :: GetSubject () const {

	return smPossibleInterlocusArtifactRight::sSubject;
}


void smPossibleInterlocusArtifactRight :: SetIndexAndScope (int index, int scope) const {

	smPossibleInterlocusArtifactRight::sMessageIndex = index;
	smPossibleInterlocusArtifactRight::sMessageScope = scope;
}


int smPossibleInterlocusArtifactRight :: GetMessageIndex () const {

	return smPossibleInterlocusArtifactRight :: sMessageIndex;
}


int smPossibleInterlocusArtifactRight :: GetScope () const {

	return smPossibleInterlocusArtifactRight :: sMessageScope;
}


RGString smPossibleInterlocusArtifactRight :: GetName () const {

	return smPossibleInterlocusArtifactRight :: sName;
}



int smPossibleInterlocusArtifactRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPossibleInterlocusArtifactRight* noticeType = new smPossibleInterlocusArtifactRight;
	warehouse->AddType (noticeType);
	return 1;
}





smFirstAlleleEqFirstLadderPeak :: smFirstAlleleEqFirstLadderPeak () : SmartNotice () {

}


smFirstAlleleEqFirstLadderPeak :: smFirstAlleleEqFirstLadderPeak (const smFirstAlleleEqFirstLadderPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smFirstAlleleEqFirstLadderPeak :: ~smFirstAlleleEqFirstLadderPeak () {

}


int smFirstAlleleEqFirstLadderPeak :: GetSubject () const {

	return smFirstAlleleEqFirstLadderPeak::sSubject;
}


void smFirstAlleleEqFirstLadderPeak :: SetIndexAndScope (int index, int scope) const {

	smFirstAlleleEqFirstLadderPeak::sMessageIndex = index;
	smFirstAlleleEqFirstLadderPeak::sMessageScope = scope;
}


int smFirstAlleleEqFirstLadderPeak :: GetMessageIndex () const {

	return smFirstAlleleEqFirstLadderPeak :: sMessageIndex;
}


int smFirstAlleleEqFirstLadderPeak :: GetScope () const {

	return smFirstAlleleEqFirstLadderPeak :: sMessageScope;
}


RGString smFirstAlleleEqFirstLadderPeak :: GetName () const {

	return smFirstAlleleEqFirstLadderPeak :: sName;
}



int smFirstAlleleEqFirstLadderPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smFirstAlleleEqFirstLadderPeak* noticeType = new smFirstAlleleEqFirstLadderPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smSignalOL :: smSignalOL () : SmartNotice () {

}


smSignalOL :: smSignalOL (const smSignalOL& note) : SmartNotice ((const SmartNotice&) note) {

}



smSignalOL :: ~smSignalOL () {

}


int smSignalOL :: GetSubject () const {

	return smSignalOL::sSubject;
}


void smSignalOL :: SetIndexAndScope (int index, int scope) const {

	smSignalOL::sMessageIndex = index;
	smSignalOL::sMessageScope = scope;
}


int smSignalOL :: GetMessageIndex () const {

	return smSignalOL :: sMessageIndex;
}


int smSignalOL :: GetScope () const {

	return smSignalOL :: sMessageScope;
}


RGString smSignalOL :: GetName () const {

	return smSignalOL :: sName;
}



int smSignalOL :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSignalOL* noticeType = new smSignalOL;
	warehouse->AddType (noticeType);
	return 1;
}





smAMELMisaligned :: smAMELMisaligned () : SmartNotice () {

}


smAMELMisaligned :: smAMELMisaligned (const smAMELMisaligned& note) : SmartNotice ((const SmartNotice&) note) {

}



smAMELMisaligned :: ~smAMELMisaligned () {

}


int smAMELMisaligned :: GetSubject () const {

	return smAMELMisaligned::sSubject;
}


void smAMELMisaligned :: SetIndexAndScope (int index, int scope) const {

	smAMELMisaligned::sMessageIndex = index;
	smAMELMisaligned::sMessageScope = scope;
}


int smAMELMisaligned :: GetMessageIndex () const {

	return smAMELMisaligned :: sMessageIndex;
}


int smAMELMisaligned :: GetScope () const {

	return smAMELMisaligned :: sMessageScope;
}


RGString smAMELMisaligned :: GetName () const {

	return smAMELMisaligned :: sName;
}



int smAMELMisaligned :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAMELMisaligned* noticeType = new smAMELMisaligned;
	warehouse->AddType (noticeType);
	return 1;
}





smCrater :: smCrater () : SmartNotice () {

}


smCrater :: smCrater (const smCrater& note) : SmartNotice ((const SmartNotice&) note) {

}



smCrater :: ~smCrater () {

}


int smCrater :: GetSubject () const {

	return smCrater::sSubject;
}


void smCrater :: SetIndexAndScope (int index, int scope) const {

	smCrater::sMessageIndex = index;
	smCrater::sMessageScope = scope;
}


int smCrater :: GetMessageIndex () const {

	return smCrater :: sMessageIndex;
}


int smCrater :: GetScope () const {

	return smCrater :: sMessageScope;
}


RGString smCrater :: GetName () const {

	return smCrater :: sName;
}



int smCrater :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCrater* noticeType = new smCrater;
	warehouse->AddType (noticeType);
	return 1;
}





smAcceptedOLLeft :: smAcceptedOLLeft () : SmartNotice () {

}


smAcceptedOLLeft :: smAcceptedOLLeft (const smAcceptedOLLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smAcceptedOLLeft :: ~smAcceptedOLLeft () {

}


int smAcceptedOLLeft :: GetSubject () const {

	return smAcceptedOLLeft::sSubject;
}


void smAcceptedOLLeft :: SetIndexAndScope (int index, int scope) const {

	smAcceptedOLLeft::sMessageIndex = index;
	smAcceptedOLLeft::sMessageScope = scope;
}


int smAcceptedOLLeft :: GetMessageIndex () const {

	return smAcceptedOLLeft :: sMessageIndex;
}


int smAcceptedOLLeft :: GetScope () const {

	return smAcceptedOLLeft :: sMessageScope;
}


RGString smAcceptedOLLeft :: GetName () const {

	return smAcceptedOLLeft :: sName;
}



int smAcceptedOLLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAcceptedOLLeft* noticeType = new smAcceptedOLLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smAcceptedOLRight :: smAcceptedOLRight () : SmartNotice () {

}


smAcceptedOLRight :: smAcceptedOLRight (const smAcceptedOLRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smAcceptedOLRight :: ~smAcceptedOLRight () {

}


int smAcceptedOLRight :: GetSubject () const {

	return smAcceptedOLRight::sSubject;
}


void smAcceptedOLRight :: SetIndexAndScope (int index, int scope) const {

	smAcceptedOLRight::sMessageIndex = index;
	smAcceptedOLRight::sMessageScope = scope;
}


int smAcceptedOLRight :: GetMessageIndex () const {

	return smAcceptedOLRight :: sMessageIndex;
}


int smAcceptedOLRight :: GetScope () const {

	return smAcceptedOLRight :: sMessageScope;
}


RGString smAcceptedOLRight :: GetName () const {

	return smAcceptedOLRight :: sName;
}



int smAcceptedOLRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAcceptedOLRight* noticeType = new smAcceptedOLRight;
	warehouse->AddType (noticeType);
	return 1;
}





smStage3Successful :: smStage3Successful () : SmartNotice () {

}


smStage3Successful :: smStage3Successful (const smStage3Successful& note) : SmartNotice ((const SmartNotice&) note) {

}



smStage3Successful :: ~smStage3Successful () {

}


int smStage3Successful :: GetSubject () const {

	return smStage3Successful::sSubject;
}


void smStage3Successful :: SetIndexAndScope (int index, int scope) const {

	smStage3Successful::sMessageIndex = index;
	smStage3Successful::sMessageScope = scope;
}


int smStage3Successful :: GetMessageIndex () const {

	return smStage3Successful :: sMessageIndex;
}


int smStage3Successful :: GetScope () const {

	return smStage3Successful :: sMessageScope;
}


RGString smStage3Successful :: GetName () const {

	return smStage3Successful :: sName;
}



int smStage3Successful :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smStage3Successful* noticeType = new smStage3Successful;
	warehouse->AddType (noticeType);
	return 1;
}





smValidOLInterlocusLeft :: smValidOLInterlocusLeft () : SmartNotice () {

}


smValidOLInterlocusLeft :: smValidOLInterlocusLeft (const smValidOLInterlocusLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smValidOLInterlocusLeft :: ~smValidOLInterlocusLeft () {

}


int smValidOLInterlocusLeft :: GetSubject () const {

	return smValidOLInterlocusLeft::sSubject;
}


void smValidOLInterlocusLeft :: SetIndexAndScope (int index, int scope) const {

	smValidOLInterlocusLeft::sMessageIndex = index;
	smValidOLInterlocusLeft::sMessageScope = scope;
}


int smValidOLInterlocusLeft :: GetMessageIndex () const {

	return smValidOLInterlocusLeft :: sMessageIndex;
}


int smValidOLInterlocusLeft :: GetScope () const {

	return smValidOLInterlocusLeft :: sMessageScope;
}


RGString smValidOLInterlocusLeft :: GetName () const {

	return smValidOLInterlocusLeft :: sName;
}



int smValidOLInterlocusLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smValidOLInterlocusLeft* noticeType = new smValidOLInterlocusLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smValidOLInterlocusRight :: smValidOLInterlocusRight () : SmartNotice () {

}


smValidOLInterlocusRight :: smValidOLInterlocusRight (const smValidOLInterlocusRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smValidOLInterlocusRight :: ~smValidOLInterlocusRight () {

}


int smValidOLInterlocusRight :: GetSubject () const {

	return smValidOLInterlocusRight::sSubject;
}


void smValidOLInterlocusRight :: SetIndexAndScope (int index, int scope) const {

	smValidOLInterlocusRight::sMessageIndex = index;
	smValidOLInterlocusRight::sMessageScope = scope;
}


int smValidOLInterlocusRight :: GetMessageIndex () const {

	return smValidOLInterlocusRight :: sMessageIndex;
}


int smValidOLInterlocusRight :: GetScope () const {

	return smValidOLInterlocusRight :: sMessageScope;
}


RGString smValidOLInterlocusRight :: GetName () const {

	return smValidOLInterlocusRight :: sName;
}



int smValidOLInterlocusRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smValidOLInterlocusRight* noticeType = new smValidOLInterlocusRight;
	warehouse->AddType (noticeType);
	return 1;
}





smAcceptedInterlocusLeft :: smAcceptedInterlocusLeft () : SmartNotice () {

}


smAcceptedInterlocusLeft :: smAcceptedInterlocusLeft (const smAcceptedInterlocusLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smAcceptedInterlocusLeft :: ~smAcceptedInterlocusLeft () {

}


int smAcceptedInterlocusLeft :: GetSubject () const {

	return smAcceptedInterlocusLeft::sSubject;
}


void smAcceptedInterlocusLeft :: SetIndexAndScope (int index, int scope) const {

	smAcceptedInterlocusLeft::sMessageIndex = index;
	smAcceptedInterlocusLeft::sMessageScope = scope;
}


int smAcceptedInterlocusLeft :: GetMessageIndex () const {

	return smAcceptedInterlocusLeft :: sMessageIndex;
}


int smAcceptedInterlocusLeft :: GetScope () const {

	return smAcceptedInterlocusLeft :: sMessageScope;
}


RGString smAcceptedInterlocusLeft :: GetName () const {

	return smAcceptedInterlocusLeft :: sName;
}



int smAcceptedInterlocusLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAcceptedInterlocusLeft* noticeType = new smAcceptedInterlocusLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smAcceptedInterlocusRight :: smAcceptedInterlocusRight () : SmartNotice () {

}


smAcceptedInterlocusRight :: smAcceptedInterlocusRight (const smAcceptedInterlocusRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smAcceptedInterlocusRight :: ~smAcceptedInterlocusRight () {

}


int smAcceptedInterlocusRight :: GetSubject () const {

	return smAcceptedInterlocusRight::sSubject;
}


void smAcceptedInterlocusRight :: SetIndexAndScope (int index, int scope) const {

	smAcceptedInterlocusRight::sMessageIndex = index;
	smAcceptedInterlocusRight::sMessageScope = scope;
}


int smAcceptedInterlocusRight :: GetMessageIndex () const {

	return smAcceptedInterlocusRight :: sMessageIndex;
}


int smAcceptedInterlocusRight :: GetScope () const {

	return smAcceptedInterlocusRight :: sMessageScope;
}


RGString smAcceptedInterlocusRight :: GetName () const {

	return smAcceptedInterlocusRight :: sName;
}



int smAcceptedInterlocusRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAcceptedInterlocusRight* noticeType = new smAcceptedInterlocusRight;
	warehouse->AddType (noticeType);
	return 1;
}





smNoXForAMEL :: smNoXForAMEL () : SmartNotice () {

}


smNoXForAMEL :: smNoXForAMEL (const smNoXForAMEL& note) : SmartNotice ((const SmartNotice&) note) {

}



smNoXForAMEL :: ~smNoXForAMEL () {

}


int smNoXForAMEL :: GetSubject () const {

	return smNoXForAMEL::sSubject;
}


void smNoXForAMEL :: SetIndexAndScope (int index, int scope) const {

	smNoXForAMEL::sMessageIndex = index;
	smNoXForAMEL::sMessageScope = scope;
}


int smNoXForAMEL :: GetMessageIndex () const {

	return smNoXForAMEL :: sMessageIndex;
}


int smNoXForAMEL :: GetScope () const {

	return smNoXForAMEL :: sMessageScope;
}


RGString smNoXForAMEL :: GetName () const {

	return smNoXForAMEL :: sName;
}



int smNoXForAMEL :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNoXForAMEL* noticeType = new smNoXForAMEL;
	warehouse->AddType (noticeType);
	return 1;
}





smSignalIsAllele :: smSignalIsAllele () : SmartNotice () {

}


smSignalIsAllele :: smSignalIsAllele (const smSignalIsAllele& note) : SmartNotice ((const SmartNotice&) note) {

}



smSignalIsAllele :: ~smSignalIsAllele () {

}


int smSignalIsAllele :: GetSubject () const {

	return smSignalIsAllele::sSubject;
}


void smSignalIsAllele :: SetIndexAndScope (int index, int scope) const {

	smSignalIsAllele::sMessageIndex = index;
	smSignalIsAllele::sMessageScope = scope;
}


int smSignalIsAllele :: GetMessageIndex () const {

	return smSignalIsAllele :: sMessageIndex;
}


int smSignalIsAllele :: GetScope () const {

	return smSignalIsAllele :: sMessageScope;
}


RGString smSignalIsAllele :: GetName () const {

	return smSignalIsAllele :: sName;
}



int smSignalIsAllele :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSignalIsAllele* noticeType = new smSignalIsAllele;
	warehouse->AddType (noticeType);
	return 1;
}





smPosCtrlLocusMismatch :: smPosCtrlLocusMismatch () : SmartNotice () {

}


smPosCtrlLocusMismatch :: smPosCtrlLocusMismatch (const smPosCtrlLocusMismatch& note) : SmartNotice ((const SmartNotice&) note) {

}



smPosCtrlLocusMismatch :: ~smPosCtrlLocusMismatch () {

}


int smPosCtrlLocusMismatch :: GetSubject () const {

	return smPosCtrlLocusMismatch::sSubject;
}


void smPosCtrlLocusMismatch :: SetIndexAndScope (int index, int scope) const {

	smPosCtrlLocusMismatch::sMessageIndex = index;
	smPosCtrlLocusMismatch::sMessageScope = scope;
}


int smPosCtrlLocusMismatch :: GetMessageIndex () const {

	return smPosCtrlLocusMismatch :: sMessageIndex;
}


int smPosCtrlLocusMismatch :: GetScope () const {

	return smPosCtrlLocusMismatch :: sMessageScope;
}


RGString smPosCtrlLocusMismatch :: GetName () const {

	return smPosCtrlLocusMismatch :: sName;
}



int smPosCtrlLocusMismatch :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPosCtrlLocusMismatch* noticeType = new smPosCtrlLocusMismatch;
	warehouse->AddType (noticeType);
	return 1;
}





smNoGenotypeFound :: smNoGenotypeFound () : SmartNotice () {

}


smNoGenotypeFound :: smNoGenotypeFound (const smNoGenotypeFound& note) : SmartNotice ((const SmartNotice&) note) {

}



smNoGenotypeFound :: ~smNoGenotypeFound () {

}


int smNoGenotypeFound :: GetSubject () const {

	return smNoGenotypeFound::sSubject;
}


void smNoGenotypeFound :: SetIndexAndScope (int index, int scope) const {

	smNoGenotypeFound::sMessageIndex = index;
	smNoGenotypeFound::sMessageScope = scope;
}


int smNoGenotypeFound :: GetMessageIndex () const {

	return smNoGenotypeFound :: sMessageIndex;
}


int smNoGenotypeFound :: GetScope () const {

	return smNoGenotypeFound :: sMessageScope;
}


RGString smNoGenotypeFound :: GetName () const {

	return smNoGenotypeFound :: sName;
}



int smNoGenotypeFound :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNoGenotypeFound* noticeType = new smNoGenotypeFound;
	warehouse->AddType (noticeType);
	return 1;
}





smThreeOrMoreAlleles :: smThreeOrMoreAlleles () : SmartNotice () {

}


smThreeOrMoreAlleles :: smThreeOrMoreAlleles (const smThreeOrMoreAlleles& note) : SmartNotice ((const SmartNotice&) note) {

}



smThreeOrMoreAlleles :: ~smThreeOrMoreAlleles () {

}


int smThreeOrMoreAlleles :: GetSubject () const {

	return smThreeOrMoreAlleles::sSubject;
}


void smThreeOrMoreAlleles :: SetIndexAndScope (int index, int scope) const {

	smThreeOrMoreAlleles::sMessageIndex = index;
	smThreeOrMoreAlleles::sMessageScope = scope;
}


int smThreeOrMoreAlleles :: GetMessageIndex () const {

	return smThreeOrMoreAlleles :: sMessageIndex;
}


int smThreeOrMoreAlleles :: GetScope () const {

	return smThreeOrMoreAlleles :: sMessageScope;
}


RGString smThreeOrMoreAlleles :: GetName () const {

	return smThreeOrMoreAlleles :: sName;
}



int smThreeOrMoreAlleles :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smThreeOrMoreAlleles* noticeType = new smThreeOrMoreAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





smTwoOrMoreAlleles :: smTwoOrMoreAlleles () : SmartNotice () {

}


smTwoOrMoreAlleles :: smTwoOrMoreAlleles (const smTwoOrMoreAlleles& note) : SmartNotice ((const SmartNotice&) note) {

}



smTwoOrMoreAlleles :: ~smTwoOrMoreAlleles () {

}


int smTwoOrMoreAlleles :: GetSubject () const {

	return smTwoOrMoreAlleles::sSubject;
}


void smTwoOrMoreAlleles :: SetIndexAndScope (int index, int scope) const {

	smTwoOrMoreAlleles::sMessageIndex = index;
	smTwoOrMoreAlleles::sMessageScope = scope;
}


int smTwoOrMoreAlleles :: GetMessageIndex () const {

	return smTwoOrMoreAlleles :: sMessageIndex;
}


int smTwoOrMoreAlleles :: GetScope () const {

	return smTwoOrMoreAlleles :: sMessageScope;
}


RGString smTwoOrMoreAlleles :: GetName () const {

	return smTwoOrMoreAlleles :: sName;
}



int smTwoOrMoreAlleles :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smTwoOrMoreAlleles* noticeType = new smTwoOrMoreAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





smHeterozygousImbalance :: smHeterozygousImbalance () : SmartNotice () {

}


smHeterozygousImbalance :: smHeterozygousImbalance (const smHeterozygousImbalance& note) : SmartNotice ((const SmartNotice&) note) {

}



smHeterozygousImbalance :: ~smHeterozygousImbalance () {

}


int smHeterozygousImbalance :: GetSubject () const {

	return smHeterozygousImbalance::sSubject;
}


void smHeterozygousImbalance :: SetIndexAndScope (int index, int scope) const {

	smHeterozygousImbalance::sMessageIndex = index;
	smHeterozygousImbalance::sMessageScope = scope;
}


int smHeterozygousImbalance :: GetMessageIndex () const {

	return smHeterozygousImbalance :: sMessageIndex;
}


int smHeterozygousImbalance :: GetScope () const {

	return smHeterozygousImbalance :: sMessageScope;
}


RGString smHeterozygousImbalance :: GetName () const {

	return smHeterozygousImbalance :: sName;
}



int smHeterozygousImbalance :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smHeterozygousImbalance* noticeType = new smHeterozygousImbalance;
	warehouse->AddType (noticeType);
	return 1;
}





smOLAlleles :: smOLAlleles () : SmartNotice () {

}


smOLAlleles :: smOLAlleles (const smOLAlleles& note) : SmartNotice ((const SmartNotice&) note) {

}



smOLAlleles :: ~smOLAlleles () {

}


int smOLAlleles :: GetSubject () const {

	return smOLAlleles::sSubject;
}


void smOLAlleles :: SetIndexAndScope (int index, int scope) const {

	smOLAlleles::sMessageIndex = index;
	smOLAlleles::sMessageScope = scope;
}


int smOLAlleles :: GetMessageIndex () const {

	return smOLAlleles :: sMessageIndex;
}


int smOLAlleles :: GetScope () const {

	return smOLAlleles :: sMessageScope;
}


RGString smOLAlleles :: GetName () const {

	return smOLAlleles :: sName;
}



int smOLAlleles :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smOLAlleles* noticeType = new smOLAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





smHomozygoteHeightProblem :: smHomozygoteHeightProblem () : SmartNotice () {

}


smHomozygoteHeightProblem :: smHomozygoteHeightProblem (const smHomozygoteHeightProblem& note) : SmartNotice ((const SmartNotice&) note) {

}



smHomozygoteHeightProblem :: ~smHomozygoteHeightProblem () {

}


int smHomozygoteHeightProblem :: GetSubject () const {

	return smHomozygoteHeightProblem::sSubject;
}


void smHomozygoteHeightProblem :: SetIndexAndScope (int index, int scope) const {

	smHomozygoteHeightProblem::sMessageIndex = index;
	smHomozygoteHeightProblem::sMessageScope = scope;
}


int smHomozygoteHeightProblem :: GetMessageIndex () const {

	return smHomozygoteHeightProblem :: sMessageIndex;
}


int smHomozygoteHeightProblem :: GetScope () const {

	return smHomozygoteHeightProblem :: sMessageScope;
}


RGString smHomozygoteHeightProblem :: GetName () const {

	return smHomozygoteHeightProblem :: sName;
}



int smHomozygoteHeightProblem :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smHomozygoteHeightProblem* noticeType = new smHomozygoteHeightProblem;
	warehouse->AddType (noticeType);
	return 1;
}





smPullUpAtAllele :: smPullUpAtAllele () : SmartNotice () {

}


smPullUpAtAllele :: smPullUpAtAllele (const smPullUpAtAllele& note) : SmartNotice ((const SmartNotice&) note) {

}



smPullUpAtAllele :: ~smPullUpAtAllele () {

}


int smPullUpAtAllele :: GetSubject () const {

	return smPullUpAtAllele::sSubject;
}


void smPullUpAtAllele :: SetIndexAndScope (int index, int scope) const {

	smPullUpAtAllele::sMessageIndex = index;
	smPullUpAtAllele::sMessageScope = scope;
}


int smPullUpAtAllele :: GetMessageIndex () const {

	return smPullUpAtAllele :: sMessageIndex;
}


int smPullUpAtAllele :: GetScope () const {

	return smPullUpAtAllele :: sMessageScope;
}


RGString smPullUpAtAllele :: GetName () const {

	return smPullUpAtAllele :: sName;
}



int smPullUpAtAllele :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPullUpAtAllele* noticeType = new smPullUpAtAllele;
	warehouse->AddType (noticeType);
	return 1;
}





smPosCtrlLocusNotFound :: smPosCtrlLocusNotFound () : SmartNotice () {

}


smPosCtrlLocusNotFound :: smPosCtrlLocusNotFound (const smPosCtrlLocusNotFound& note) : SmartNotice ((const SmartNotice&) note) {

}



smPosCtrlLocusNotFound :: ~smPosCtrlLocusNotFound () {

}


int smPosCtrlLocusNotFound :: GetSubject () const {

	return smPosCtrlLocusNotFound::sSubject;
}


void smPosCtrlLocusNotFound :: SetIndexAndScope (int index, int scope) const {

	smPosCtrlLocusNotFound::sMessageIndex = index;
	smPosCtrlLocusNotFound::sMessageScope = scope;
}


int smPosCtrlLocusNotFound :: GetMessageIndex () const {

	return smPosCtrlLocusNotFound :: sMessageIndex;
}


int smPosCtrlLocusNotFound :: GetScope () const {

	return smPosCtrlLocusNotFound :: sMessageScope;
}


RGString smPosCtrlLocusNotFound :: GetName () const {

	return smPosCtrlLocusNotFound :: sName;
}



int smPosCtrlLocusNotFound :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPosCtrlLocusNotFound* noticeType = new smPosCtrlLocusNotFound;
	warehouse->AddType (noticeType);
	return 1;
}





smBPResidualTooLarge :: smBPResidualTooLarge () : SmartNotice () {

}


smBPResidualTooLarge :: smBPResidualTooLarge (const smBPResidualTooLarge& note) : SmartNotice ((const SmartNotice&) note) {

}



smBPResidualTooLarge :: ~smBPResidualTooLarge () {

}


int smBPResidualTooLarge :: GetSubject () const {

	return smBPResidualTooLarge::sSubject;
}


void smBPResidualTooLarge :: SetIndexAndScope (int index, int scope) const {

	smBPResidualTooLarge::sMessageIndex = index;
	smBPResidualTooLarge::sMessageScope = scope;
}


int smBPResidualTooLarge :: GetMessageIndex () const {

	return smBPResidualTooLarge :: sMessageIndex;
}


int smBPResidualTooLarge :: GetScope () const {

	return smBPResidualTooLarge :: sMessageScope;
}


RGString smBPResidualTooLarge :: GetName () const {

	return smBPResidualTooLarge :: sName;
}



int smBPResidualTooLarge :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBPResidualTooLarge* noticeType = new smBPResidualTooLarge;
	warehouse->AddType (noticeType);
	return 1;
}





smExtraLadderAllele :: smExtraLadderAllele () : SmartNotice () {

}


smExtraLadderAllele :: smExtraLadderAllele (const smExtraLadderAllele& note) : SmartNotice ((const SmartNotice&) note) {

}



smExtraLadderAllele :: ~smExtraLadderAllele () {

}


int smExtraLadderAllele :: GetSubject () const {

	return smExtraLadderAllele::sSubject;
}


void smExtraLadderAllele :: SetIndexAndScope (int index, int scope) const {

	smExtraLadderAllele::sMessageIndex = index;
	smExtraLadderAllele::sMessageScope = scope;
}


int smExtraLadderAllele :: GetMessageIndex () const {

	return smExtraLadderAllele :: sMessageIndex;
}


int smExtraLadderAllele :: GetScope () const {

	return smExtraLadderAllele :: sMessageScope;
}


RGString smExtraLadderAllele :: GetName () const {

	return smExtraLadderAllele :: sName;
}



int smExtraLadderAllele :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smExtraLadderAllele* noticeType = new smExtraLadderAllele;
	warehouse->AddType (noticeType);
	return 1;
}





smCriticalMessagesAtILS :: smCriticalMessagesAtILS () : SmartNotice () {

}


smCriticalMessagesAtILS :: smCriticalMessagesAtILS (const smCriticalMessagesAtILS& note) : SmartNotice ((const SmartNotice&) note) {

}



smCriticalMessagesAtILS :: ~smCriticalMessagesAtILS () {

}


int smCriticalMessagesAtILS :: GetSubject () const {

	return smCriticalMessagesAtILS::sSubject;
}


void smCriticalMessagesAtILS :: SetIndexAndScope (int index, int scope) const {

	smCriticalMessagesAtILS::sMessageIndex = index;
	smCriticalMessagesAtILS::sMessageScope = scope;
}


int smCriticalMessagesAtILS :: GetMessageIndex () const {

	return smCriticalMessagesAtILS :: sMessageIndex;
}


int smCriticalMessagesAtILS :: GetScope () const {

	return smCriticalMessagesAtILS :: sMessageScope;
}


RGString smCriticalMessagesAtILS :: GetName () const {

	return smCriticalMessagesAtILS :: sName;
}



int smCriticalMessagesAtILS :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCriticalMessagesAtILS* noticeType = new smCriticalMessagesAtILS;
	warehouse->AddType (noticeType);
	return 1;
}





smAMELHasExtraneousPeaks :: smAMELHasExtraneousPeaks () : SmartNotice () {

}


smAMELHasExtraneousPeaks :: smAMELHasExtraneousPeaks (const smAMELHasExtraneousPeaks& note) : SmartNotice ((const SmartNotice&) note) {

}



smAMELHasExtraneousPeaks :: ~smAMELHasExtraneousPeaks () {

}


int smAMELHasExtraneousPeaks :: GetSubject () const {

	return smAMELHasExtraneousPeaks::sSubject;
}


void smAMELHasExtraneousPeaks :: SetIndexAndScope (int index, int scope) const {

	smAMELHasExtraneousPeaks::sMessageIndex = index;
	smAMELHasExtraneousPeaks::sMessageScope = scope;
}


int smAMELHasExtraneousPeaks :: GetMessageIndex () const {

	return smAMELHasExtraneousPeaks :: sMessageIndex;
}


int smAMELHasExtraneousPeaks :: GetScope () const {

	return smAMELHasExtraneousPeaks :: sMessageScope;
}


RGString smAMELHasExtraneousPeaks :: GetName () const {

	return smAMELHasExtraneousPeaks :: sName;
}



int smAMELHasExtraneousPeaks :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAMELHasExtraneousPeaks* noticeType = new smAMELHasExtraneousPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





smExtraneousAMELPeak :: smExtraneousAMELPeak () : SmartNotice () {

}


smExtraneousAMELPeak :: smExtraneousAMELPeak (const smExtraneousAMELPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smExtraneousAMELPeak :: ~smExtraneousAMELPeak () {

}


int smExtraneousAMELPeak :: GetSubject () const {

	return smExtraneousAMELPeak::sSubject;
}


void smExtraneousAMELPeak :: SetIndexAndScope (int index, int scope) const {

	smExtraneousAMELPeak::sMessageIndex = index;
	smExtraneousAMELPeak::sMessageScope = scope;
}


int smExtraneousAMELPeak :: GetMessageIndex () const {

	return smExtraneousAMELPeak :: sMessageIndex;
}


int smExtraneousAMELPeak :: GetScope () const {

	return smExtraneousAMELPeak :: sMessageScope;
}


RGString smExtraneousAMELPeak :: GetName () const {

	return smExtraneousAMELPeak :: sName;
}



int smExtraneousAMELPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smExtraneousAMELPeak* noticeType = new smExtraneousAMELPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smNegCtrlChannelHasNoPrimerPeaks :: smNegCtrlChannelHasNoPrimerPeaks () : SmartNotice () {

}


smNegCtrlChannelHasNoPrimerPeaks :: smNegCtrlChannelHasNoPrimerPeaks (const smNegCtrlChannelHasNoPrimerPeaks& note) : SmartNotice ((const SmartNotice&) note) {

}



smNegCtrlChannelHasNoPrimerPeaks :: ~smNegCtrlChannelHasNoPrimerPeaks () {

}


int smNegCtrlChannelHasNoPrimerPeaks :: GetSubject () const {

	return smNegCtrlChannelHasNoPrimerPeaks::sSubject;
}


void smNegCtrlChannelHasNoPrimerPeaks :: SetIndexAndScope (int index, int scope) const {

	smNegCtrlChannelHasNoPrimerPeaks::sMessageIndex = index;
	smNegCtrlChannelHasNoPrimerPeaks::sMessageScope = scope;
}


int smNegCtrlChannelHasNoPrimerPeaks :: GetMessageIndex () const {

	return smNegCtrlChannelHasNoPrimerPeaks :: sMessageIndex;
}


int smNegCtrlChannelHasNoPrimerPeaks :: GetScope () const {

	return smNegCtrlChannelHasNoPrimerPeaks :: sMessageScope;
}


RGString smNegCtrlChannelHasNoPrimerPeaks :: GetName () const {

	return smNegCtrlChannelHasNoPrimerPeaks :: sName;
}



int smNegCtrlChannelHasNoPrimerPeaks :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNegCtrlChannelHasNoPrimerPeaks* noticeType = new smNegCtrlChannelHasNoPrimerPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





smMinimumHeightForPrimerPeaks :: smMinimumHeightForPrimerPeaks () : SmartNotice () {

}


smMinimumHeightForPrimerPeaks :: smMinimumHeightForPrimerPeaks (const smMinimumHeightForPrimerPeaks& note) : SmartNotice ((const SmartNotice&) note) {

}



smMinimumHeightForPrimerPeaks :: ~smMinimumHeightForPrimerPeaks () {

}


int smMinimumHeightForPrimerPeaks :: GetSubject () const {

	return smMinimumHeightForPrimerPeaks::sSubject;
}


void smMinimumHeightForPrimerPeaks :: SetIndexAndScope (int index, int scope) const {

	smMinimumHeightForPrimerPeaks::sMessageIndex = index;
	smMinimumHeightForPrimerPeaks::sMessageScope = scope;
}


int smMinimumHeightForPrimerPeaks :: GetMessageIndex () const {

	return smMinimumHeightForPrimerPeaks :: sMessageIndex;
}


int smMinimumHeightForPrimerPeaks :: GetScope () const {

	return smMinimumHeightForPrimerPeaks :: sMessageScope;
}


RGString smMinimumHeightForPrimerPeaks :: GetName () const {

	return smMinimumHeightForPrimerPeaks :: sName;
}



int smMinimumHeightForPrimerPeaks :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMinimumHeightForPrimerPeaks* noticeType = new smMinimumHeightForPrimerPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





smInterlocusPeaksToRight :: smInterlocusPeaksToRight () : SmartNotice () {

}


smInterlocusPeaksToRight :: smInterlocusPeaksToRight (const smInterlocusPeaksToRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smInterlocusPeaksToRight :: ~smInterlocusPeaksToRight () {

}


int smInterlocusPeaksToRight :: GetSubject () const {

	return smInterlocusPeaksToRight::sSubject;
}


void smInterlocusPeaksToRight :: SetIndexAndScope (int index, int scope) const {

	smInterlocusPeaksToRight::sMessageIndex = index;
	smInterlocusPeaksToRight::sMessageScope = scope;
}


int smInterlocusPeaksToRight :: GetMessageIndex () const {

	return smInterlocusPeaksToRight :: sMessageIndex;
}


int smInterlocusPeaksToRight :: GetScope () const {

	return smInterlocusPeaksToRight :: sMessageScope;
}


RGString smInterlocusPeaksToRight :: GetName () const {

	return smInterlocusPeaksToRight :: sName;
}



int smInterlocusPeaksToRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smInterlocusPeaksToRight* noticeType = new smInterlocusPeaksToRight;
	warehouse->AddType (noticeType);
	return 1;
}





smInterlocusPeaksToLeft :: smInterlocusPeaksToLeft () : SmartNotice () {

}


smInterlocusPeaksToLeft :: smInterlocusPeaksToLeft (const smInterlocusPeaksToLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smInterlocusPeaksToLeft :: ~smInterlocusPeaksToLeft () {

}


int smInterlocusPeaksToLeft :: GetSubject () const {

	return smInterlocusPeaksToLeft::sSubject;
}


void smInterlocusPeaksToLeft :: SetIndexAndScope (int index, int scope) const {

	smInterlocusPeaksToLeft::sMessageIndex = index;
	smInterlocusPeaksToLeft::sMessageScope = scope;
}


int smInterlocusPeaksToLeft :: GetMessageIndex () const {

	return smInterlocusPeaksToLeft :: sMessageIndex;
}


int smInterlocusPeaksToLeft :: GetScope () const {

	return smInterlocusPeaksToLeft :: sMessageScope;
}


RGString smInterlocusPeaksToLeft :: GetName () const {

	return smInterlocusPeaksToLeft :: sName;
}



int smInterlocusPeaksToLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smInterlocusPeaksToLeft* noticeType = new smInterlocusPeaksToLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smLadderFailed :: smLadderFailed () : SmartNotice () {

}


smLadderFailed :: smLadderFailed (const smLadderFailed& note) : SmartNotice ((const SmartNotice&) note) {

}



smLadderFailed :: ~smLadderFailed () {

}


int smLadderFailed :: GetSubject () const {

	return smLadderFailed::sSubject;
}


void smLadderFailed :: SetIndexAndScope (int index, int scope) const {

	smLadderFailed::sMessageIndex = index;
	smLadderFailed::sMessageScope = scope;
}


int smLadderFailed :: GetMessageIndex () const {

	return smLadderFailed :: sMessageIndex;
}


int smLadderFailed :: GetScope () const {

	return smLadderFailed :: sMessageScope;
}


RGString smLadderFailed :: GetName () const {

	return smLadderFailed :: sName;
}



int smLadderFailed :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLadderFailed* noticeType = new smLadderFailed;
	warehouse->AddType (noticeType);
	return 1;
}





smSampleUnanalyzable :: smSampleUnanalyzable () : SmartNotice () {

}


smSampleUnanalyzable :: smSampleUnanalyzable (const smSampleUnanalyzable& note) : SmartNotice ((const SmartNotice&) note) {

}



smSampleUnanalyzable :: ~smSampleUnanalyzable () {

}


int smSampleUnanalyzable :: GetSubject () const {

	return smSampleUnanalyzable::sSubject;
}


void smSampleUnanalyzable :: SetIndexAndScope (int index, int scope) const {

	smSampleUnanalyzable::sMessageIndex = index;
	smSampleUnanalyzable::sMessageScope = scope;
}


int smSampleUnanalyzable :: GetMessageIndex () const {

	return smSampleUnanalyzable :: sMessageIndex;
}


int smSampleUnanalyzable :: GetScope () const {

	return smSampleUnanalyzable :: sMessageScope;
}


RGString smSampleUnanalyzable :: GetName () const {

	return smSampleUnanalyzable :: sName;
}



int smSampleUnanalyzable :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSampleUnanalyzable* noticeType = new smSampleUnanalyzable;
	warehouse->AddType (noticeType);
	return 1;
}





smCriticalMessagesAtAlleles :: smCriticalMessagesAtAlleles () : SmartNotice () {

}


smCriticalMessagesAtAlleles :: smCriticalMessagesAtAlleles (const smCriticalMessagesAtAlleles& note) : SmartNotice ((const SmartNotice&) note) {

}



smCriticalMessagesAtAlleles :: ~smCriticalMessagesAtAlleles () {

}


int smCriticalMessagesAtAlleles :: GetSubject () const {

	return smCriticalMessagesAtAlleles::sSubject;
}


void smCriticalMessagesAtAlleles :: SetIndexAndScope (int index, int scope) const {

	smCriticalMessagesAtAlleles::sMessageIndex = index;
	smCriticalMessagesAtAlleles::sMessageScope = scope;
}


int smCriticalMessagesAtAlleles :: GetMessageIndex () const {

	return smCriticalMessagesAtAlleles :: sMessageIndex;
}


int smCriticalMessagesAtAlleles :: GetScope () const {

	return smCriticalMessagesAtAlleles :: sMessageScope;
}


RGString smCriticalMessagesAtAlleles :: GetName () const {

	return smCriticalMessagesAtAlleles :: sName;
}



int smCriticalMessagesAtAlleles :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCriticalMessagesAtAlleles* noticeType = new smCriticalMessagesAtAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





smHeightBelowPullupFractionalFilter :: smHeightBelowPullupFractionalFilter () : SmartNotice () {

}


smHeightBelowPullupFractionalFilter :: smHeightBelowPullupFractionalFilter (const smHeightBelowPullupFractionalFilter& note) : SmartNotice ((const SmartNotice&) note) {

}



smHeightBelowPullupFractionalFilter :: ~smHeightBelowPullupFractionalFilter () {

}


int smHeightBelowPullupFractionalFilter :: GetSubject () const {

	return smHeightBelowPullupFractionalFilter::sSubject;
}


void smHeightBelowPullupFractionalFilter :: SetIndexAndScope (int index, int scope) const {

	smHeightBelowPullupFractionalFilter::sMessageIndex = index;
	smHeightBelowPullupFractionalFilter::sMessageScope = scope;
}


int smHeightBelowPullupFractionalFilter :: GetMessageIndex () const {

	return smHeightBelowPullupFractionalFilter :: sMessageIndex;
}


int smHeightBelowPullupFractionalFilter :: GetScope () const {

	return smHeightBelowPullupFractionalFilter :: sMessageScope;
}


RGString smHeightBelowPullupFractionalFilter :: GetName () const {

	return smHeightBelowPullupFractionalFilter :: sName;
}



int smHeightBelowPullupFractionalFilter :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smHeightBelowPullupFractionalFilter* noticeType = new smHeightBelowPullupFractionalFilter;
	warehouse->AddType (noticeType);
	return 1;
}





smBelowPullupFractionalFilterLeft :: smBelowPullupFractionalFilterLeft () : SmartNotice () {

}


smBelowPullupFractionalFilterLeft :: smBelowPullupFractionalFilterLeft (const smBelowPullupFractionalFilterLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smBelowPullupFractionalFilterLeft :: ~smBelowPullupFractionalFilterLeft () {

}


int smBelowPullupFractionalFilterLeft :: GetSubject () const {

	return smBelowPullupFractionalFilterLeft::sSubject;
}


void smBelowPullupFractionalFilterLeft :: SetIndexAndScope (int index, int scope) const {

	smBelowPullupFractionalFilterLeft::sMessageIndex = index;
	smBelowPullupFractionalFilterLeft::sMessageScope = scope;
}


int smBelowPullupFractionalFilterLeft :: GetMessageIndex () const {

	return smBelowPullupFractionalFilterLeft :: sMessageIndex;
}


int smBelowPullupFractionalFilterLeft :: GetScope () const {

	return smBelowPullupFractionalFilterLeft :: sMessageScope;
}


RGString smBelowPullupFractionalFilterLeft :: GetName () const {

	return smBelowPullupFractionalFilterLeft :: sName;
}



int smBelowPullupFractionalFilterLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBelowPullupFractionalFilterLeft* noticeType = new smBelowPullupFractionalFilterLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smBelowPullupFractionalFilterRight :: smBelowPullupFractionalFilterRight () : SmartNotice () {

}


smBelowPullupFractionalFilterRight :: smBelowPullupFractionalFilterRight (const smBelowPullupFractionalFilterRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smBelowPullupFractionalFilterRight :: ~smBelowPullupFractionalFilterRight () {

}


int smBelowPullupFractionalFilterRight :: GetSubject () const {

	return smBelowPullupFractionalFilterRight::sSubject;
}


void smBelowPullupFractionalFilterRight :: SetIndexAndScope (int index, int scope) const {

	smBelowPullupFractionalFilterRight::sMessageIndex = index;
	smBelowPullupFractionalFilterRight::sMessageScope = scope;
}


int smBelowPullupFractionalFilterRight :: GetMessageIndex () const {

	return smBelowPullupFractionalFilterRight :: sMessageIndex;
}


int smBelowPullupFractionalFilterRight :: GetScope () const {

	return smBelowPullupFractionalFilterRight :: sMessageScope;
}


RGString smBelowPullupFractionalFilterRight :: GetName () const {

	return smBelowPullupFractionalFilterRight :: sName;
}



int smBelowPullupFractionalFilterRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBelowPullupFractionalFilterRight* noticeType = new smBelowPullupFractionalFilterRight;
	warehouse->AddType (noticeType);
	return 1;
}





smPossibleExtraLocusAllele :: smPossibleExtraLocusAllele () : SmartNotice () {

}


smPossibleExtraLocusAllele :: smPossibleExtraLocusAllele (const smPossibleExtraLocusAllele& note) : SmartNotice ((const SmartNotice&) note) {

}



smPossibleExtraLocusAllele :: ~smPossibleExtraLocusAllele () {

}


int smPossibleExtraLocusAllele :: GetSubject () const {

	return smPossibleExtraLocusAllele::sSubject;
}


void smPossibleExtraLocusAllele :: SetIndexAndScope (int index, int scope) const {

	smPossibleExtraLocusAllele::sMessageIndex = index;
	smPossibleExtraLocusAllele::sMessageScope = scope;
}


int smPossibleExtraLocusAllele :: GetMessageIndex () const {

	return smPossibleExtraLocusAllele :: sMessageIndex;
}


int smPossibleExtraLocusAllele :: GetScope () const {

	return smPossibleExtraLocusAllele :: sMessageScope;
}


RGString smPossibleExtraLocusAllele :: GetName () const {

	return smPossibleExtraLocusAllele :: sName;
}



int smPossibleExtraLocusAllele :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPossibleExtraLocusAllele* noticeType = new smPossibleExtraLocusAllele;
	warehouse->AddType (noticeType);
	return 1;
}





smUnexpectedPeakInNegativeControl :: smUnexpectedPeakInNegativeControl () : SmartNotice () {

}


smUnexpectedPeakInNegativeControl :: smUnexpectedPeakInNegativeControl (const smUnexpectedPeakInNegativeControl& note) : SmartNotice ((const SmartNotice&) note) {

}



smUnexpectedPeakInNegativeControl :: ~smUnexpectedPeakInNegativeControl () {

}


int smUnexpectedPeakInNegativeControl :: GetSubject () const {

	return smUnexpectedPeakInNegativeControl::sSubject;
}


void smUnexpectedPeakInNegativeControl :: SetIndexAndScope (int index, int scope) const {

	smUnexpectedPeakInNegativeControl::sMessageIndex = index;
	smUnexpectedPeakInNegativeControl::sMessageScope = scope;
}


int smUnexpectedPeakInNegativeControl :: GetMessageIndex () const {

	return smUnexpectedPeakInNegativeControl :: sMessageIndex;
}


int smUnexpectedPeakInNegativeControl :: GetScope () const {

	return smUnexpectedPeakInNegativeControl :: sMessageScope;
}


RGString smUnexpectedPeakInNegativeControl :: GetName () const {

	return smUnexpectedPeakInNegativeControl :: sName;
}



int smUnexpectedPeakInNegativeControl :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUnexpectedPeakInNegativeControl* noticeType = new smUnexpectedPeakInNegativeControl;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusContainsPeaksWithExcessiveResiduals :: smLocusContainsPeaksWithExcessiveResiduals () : SmartNotice () {

}


smLocusContainsPeaksWithExcessiveResiduals :: smLocusContainsPeaksWithExcessiveResiduals (const smLocusContainsPeaksWithExcessiveResiduals& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusContainsPeaksWithExcessiveResiduals :: ~smLocusContainsPeaksWithExcessiveResiduals () {

}


int smLocusContainsPeaksWithExcessiveResiduals :: GetSubject () const {

	return smLocusContainsPeaksWithExcessiveResiduals::sSubject;
}


void smLocusContainsPeaksWithExcessiveResiduals :: SetIndexAndScope (int index, int scope) const {

	smLocusContainsPeaksWithExcessiveResiduals::sMessageIndex = index;
	smLocusContainsPeaksWithExcessiveResiduals::sMessageScope = scope;
}


int smLocusContainsPeaksWithExcessiveResiduals :: GetMessageIndex () const {

	return smLocusContainsPeaksWithExcessiveResiduals :: sMessageIndex;
}


int smLocusContainsPeaksWithExcessiveResiduals :: GetScope () const {

	return smLocusContainsPeaksWithExcessiveResiduals :: sMessageScope;
}


RGString smLocusContainsPeaksWithExcessiveResiduals :: GetName () const {

	return smLocusContainsPeaksWithExcessiveResiduals :: sName;
}



int smLocusContainsPeaksWithExcessiveResiduals :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusContainsPeaksWithExcessiveResiduals* noticeType = new smLocusContainsPeaksWithExcessiveResiduals;
	warehouse->AddType (noticeType);
	return 1;
}





smPrimaryInterchannelLink :: smPrimaryInterchannelLink () : SmartNotice () {

}


smPrimaryInterchannelLink :: smPrimaryInterchannelLink (const smPrimaryInterchannelLink& note) : SmartNotice ((const SmartNotice&) note) {

}



smPrimaryInterchannelLink :: ~smPrimaryInterchannelLink () {

}


int smPrimaryInterchannelLink :: GetSubject () const {

	return smPrimaryInterchannelLink::sSubject;
}


void smPrimaryInterchannelLink :: SetIndexAndScope (int index, int scope) const {

	smPrimaryInterchannelLink::sMessageIndex = index;
	smPrimaryInterchannelLink::sMessageScope = scope;
}


int smPrimaryInterchannelLink :: GetMessageIndex () const {

	return smPrimaryInterchannelLink :: sMessageIndex;
}


int smPrimaryInterchannelLink :: GetScope () const {

	return smPrimaryInterchannelLink :: sMessageScope;
}


RGString smPrimaryInterchannelLink :: GetName () const {

	return smPrimaryInterchannelLink :: sName;
}



int smPrimaryInterchannelLink :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPrimaryInterchannelLink* noticeType = new smPrimaryInterchannelLink;
	warehouse->AddType (noticeType);
	return 1;
}





smNotInterchannelLink :: smNotInterchannelLink () : SmartNotice () {

}


smNotInterchannelLink :: smNotInterchannelLink (const smNotInterchannelLink& note) : SmartNotice ((const SmartNotice&) note) {

}



smNotInterchannelLink :: ~smNotInterchannelLink () {

}


int smNotInterchannelLink :: GetSubject () const {

	return smNotInterchannelLink::sSubject;
}


void smNotInterchannelLink :: SetIndexAndScope (int index, int scope) const {

	smNotInterchannelLink::sMessageIndex = index;
	smNotInterchannelLink::sMessageScope = scope;
}


int smNotInterchannelLink :: GetMessageIndex () const {

	return smNotInterchannelLink :: sMessageIndex;
}


int smNotInterchannelLink :: GetScope () const {

	return smNotInterchannelLink :: sMessageScope;
}


RGString smNotInterchannelLink :: GetName () const {

	return smNotInterchannelLink :: sName;
}



int smNotInterchannelLink :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNotInterchannelLink* noticeType = new smNotInterchannelLink;
	warehouse->AddType (noticeType);
	return 1;
}





smHeightBelowInterlocusMinRFU :: smHeightBelowInterlocusMinRFU () : SmartNotice () {

}


smHeightBelowInterlocusMinRFU :: smHeightBelowInterlocusMinRFU (const smHeightBelowInterlocusMinRFU& note) : SmartNotice ((const SmartNotice&) note) {

}



smHeightBelowInterlocusMinRFU :: ~smHeightBelowInterlocusMinRFU () {

}


int smHeightBelowInterlocusMinRFU :: GetSubject () const {

	return smHeightBelowInterlocusMinRFU::sSubject;
}


void smHeightBelowInterlocusMinRFU :: SetIndexAndScope (int index, int scope) const {

	smHeightBelowInterlocusMinRFU::sMessageIndex = index;
	smHeightBelowInterlocusMinRFU::sMessageScope = scope;
}


int smHeightBelowInterlocusMinRFU :: GetMessageIndex () const {

	return smHeightBelowInterlocusMinRFU :: sMessageIndex;
}


int smHeightBelowInterlocusMinRFU :: GetScope () const {

	return smHeightBelowInterlocusMinRFU :: sMessageScope;
}


RGString smHeightBelowInterlocusMinRFU :: GetName () const {

	return smHeightBelowInterlocusMinRFU :: sName;
}



int smHeightBelowInterlocusMinRFU :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smHeightBelowInterlocusMinRFU* noticeType = new smHeightBelowInterlocusMinRFU;
	warehouse->AddType (noticeType);
	return 1;
}





smFSAChannelUnreadable :: smFSAChannelUnreadable () : SmartNotice () {

}


smFSAChannelUnreadable :: smFSAChannelUnreadable (const smFSAChannelUnreadable& note) : SmartNotice ((const SmartNotice&) note) {

}



smFSAChannelUnreadable :: ~smFSAChannelUnreadable () {

}


int smFSAChannelUnreadable :: GetSubject () const {

	return smFSAChannelUnreadable::sSubject;
}


void smFSAChannelUnreadable :: SetIndexAndScope (int index, int scope) const {

	smFSAChannelUnreadable::sMessageIndex = index;
	smFSAChannelUnreadable::sMessageScope = scope;
}


int smFSAChannelUnreadable :: GetMessageIndex () const {

	return smFSAChannelUnreadable :: sMessageIndex;
}


int smFSAChannelUnreadable :: GetScope () const {

	return smFSAChannelUnreadable :: sMessageScope;
}


RGString smFSAChannelUnreadable :: GetName () const {

	return smFSAChannelUnreadable :: sName;
}



int smFSAChannelUnreadable :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smFSAChannelUnreadable* noticeType = new smFSAChannelUnreadable;
	warehouse->AddType (noticeType);
	return 1;
}





smAmbiguousInterlocusCrater :: smAmbiguousInterlocusCrater () : SmartNotice () {

}


smAmbiguousInterlocusCrater :: smAmbiguousInterlocusCrater (const smAmbiguousInterlocusCrater& note) : SmartNotice ((const SmartNotice&) note) {

}



smAmbiguousInterlocusCrater :: ~smAmbiguousInterlocusCrater () {

}


int smAmbiguousInterlocusCrater :: GetSubject () const {

	return smAmbiguousInterlocusCrater::sSubject;
}


void smAmbiguousInterlocusCrater :: SetIndexAndScope (int index, int scope) const {

	smAmbiguousInterlocusCrater::sMessageIndex = index;
	smAmbiguousInterlocusCrater::sMessageScope = scope;
}


int smAmbiguousInterlocusCrater :: GetMessageIndex () const {

	return smAmbiguousInterlocusCrater :: sMessageIndex;
}


int smAmbiguousInterlocusCrater :: GetScope () const {

	return smAmbiguousInterlocusCrater :: sMessageScope;
}


RGString smAmbiguousInterlocusCrater :: GetName () const {

	return smAmbiguousInterlocusCrater :: sName;
}



int smAmbiguousInterlocusCrater :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAmbiguousInterlocusCrater* noticeType = new smAmbiguousInterlocusCrater;
	warehouse->AddType (noticeType);
	return 1;
}





smPossibleAmbiguousInterlocusAssignedToLocus :: smPossibleAmbiguousInterlocusAssignedToLocus () : SmartNotice () {

}


smPossibleAmbiguousInterlocusAssignedToLocus :: smPossibleAmbiguousInterlocusAssignedToLocus (const smPossibleAmbiguousInterlocusAssignedToLocus& note) : SmartNotice ((const SmartNotice&) note) {

}



smPossibleAmbiguousInterlocusAssignedToLocus :: ~smPossibleAmbiguousInterlocusAssignedToLocus () {

}


int smPossibleAmbiguousInterlocusAssignedToLocus :: GetSubject () const {

	return smPossibleAmbiguousInterlocusAssignedToLocus::sSubject;
}


void smPossibleAmbiguousInterlocusAssignedToLocus :: SetIndexAndScope (int index, int scope) const {

	smPossibleAmbiguousInterlocusAssignedToLocus::sMessageIndex = index;
	smPossibleAmbiguousInterlocusAssignedToLocus::sMessageScope = scope;
}


int smPossibleAmbiguousInterlocusAssignedToLocus :: GetMessageIndex () const {

	return smPossibleAmbiguousInterlocusAssignedToLocus :: sMessageIndex;
}


int smPossibleAmbiguousInterlocusAssignedToLocus :: GetScope () const {

	return smPossibleAmbiguousInterlocusAssignedToLocus :: sMessageScope;
}


RGString smPossibleAmbiguousInterlocusAssignedToLocus :: GetName () const {

	return smPossibleAmbiguousInterlocusAssignedToLocus :: sName;
}



int smPossibleAmbiguousInterlocusAssignedToLocus :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPossibleAmbiguousInterlocusAssignedToLocus* noticeType = new smPossibleAmbiguousInterlocusAssignedToLocus;
	warehouse->AddType (noticeType);
	return 1;
}





smWouldCauseHeterozygousImbalanceLeft :: smWouldCauseHeterozygousImbalanceLeft () : SmartNotice () {

}


smWouldCauseHeterozygousImbalanceLeft :: smWouldCauseHeterozygousImbalanceLeft (const smWouldCauseHeterozygousImbalanceLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smWouldCauseHeterozygousImbalanceLeft :: ~smWouldCauseHeterozygousImbalanceLeft () {

}


int smWouldCauseHeterozygousImbalanceLeft :: GetSubject () const {

	return smWouldCauseHeterozygousImbalanceLeft::sSubject;
}


void smWouldCauseHeterozygousImbalanceLeft :: SetIndexAndScope (int index, int scope) const {

	smWouldCauseHeterozygousImbalanceLeft::sMessageIndex = index;
	smWouldCauseHeterozygousImbalanceLeft::sMessageScope = scope;
}


int smWouldCauseHeterozygousImbalanceLeft :: GetMessageIndex () const {

	return smWouldCauseHeterozygousImbalanceLeft :: sMessageIndex;
}


int smWouldCauseHeterozygousImbalanceLeft :: GetScope () const {

	return smWouldCauseHeterozygousImbalanceLeft :: sMessageScope;
}


RGString smWouldCauseHeterozygousImbalanceLeft :: GetName () const {

	return smWouldCauseHeterozygousImbalanceLeft :: sName;
}



int smWouldCauseHeterozygousImbalanceLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smWouldCauseHeterozygousImbalanceLeft* noticeType = new smWouldCauseHeterozygousImbalanceLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smWouldCauseHeterozygousImbalanceRight :: smWouldCauseHeterozygousImbalanceRight () : SmartNotice () {

}


smWouldCauseHeterozygousImbalanceRight :: smWouldCauseHeterozygousImbalanceRight (const smWouldCauseHeterozygousImbalanceRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smWouldCauseHeterozygousImbalanceRight :: ~smWouldCauseHeterozygousImbalanceRight () {

}


int smWouldCauseHeterozygousImbalanceRight :: GetSubject () const {

	return smWouldCauseHeterozygousImbalanceRight::sSubject;
}


void smWouldCauseHeterozygousImbalanceRight :: SetIndexAndScope (int index, int scope) const {

	smWouldCauseHeterozygousImbalanceRight::sMessageIndex = index;
	smWouldCauseHeterozygousImbalanceRight::sMessageScope = scope;
}


int smWouldCauseHeterozygousImbalanceRight :: GetMessageIndex () const {

	return smWouldCauseHeterozygousImbalanceRight :: sMessageIndex;
}


int smWouldCauseHeterozygousImbalanceRight :: GetScope () const {

	return smWouldCauseHeterozygousImbalanceRight :: sMessageScope;
}


RGString smWouldCauseHeterozygousImbalanceRight :: GetName () const {

	return smWouldCauseHeterozygousImbalanceRight :: sName;
}



int smWouldCauseHeterozygousImbalanceRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smWouldCauseHeterozygousImbalanceRight* noticeType = new smWouldCauseHeterozygousImbalanceRight;
	warehouse->AddType (noticeType);
	return 1;
}





smSignalOffGridLeft :: smSignalOffGridLeft () : SmartNotice () {

}


smSignalOffGridLeft :: smSignalOffGridLeft (const smSignalOffGridLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smSignalOffGridLeft :: ~smSignalOffGridLeft () {

}


int smSignalOffGridLeft :: GetSubject () const {

	return smSignalOffGridLeft::sSubject;
}


void smSignalOffGridLeft :: SetIndexAndScope (int index, int scope) const {

	smSignalOffGridLeft::sMessageIndex = index;
	smSignalOffGridLeft::sMessageScope = scope;
}


int smSignalOffGridLeft :: GetMessageIndex () const {

	return smSignalOffGridLeft :: sMessageIndex;
}


int smSignalOffGridLeft :: GetScope () const {

	return smSignalOffGridLeft :: sMessageScope;
}


RGString smSignalOffGridLeft :: GetName () const {

	return smSignalOffGridLeft :: sName;
}



int smSignalOffGridLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSignalOffGridLeft* noticeType = new smSignalOffGridLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smSignalOffGridRight :: smSignalOffGridRight () : SmartNotice () {

}


smSignalOffGridRight :: smSignalOffGridRight (const smSignalOffGridRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smSignalOffGridRight :: ~smSignalOffGridRight () {

}


int smSignalOffGridRight :: GetSubject () const {

	return smSignalOffGridRight::sSubject;
}


void smSignalOffGridRight :: SetIndexAndScope (int index, int scope) const {

	smSignalOffGridRight::sMessageIndex = index;
	smSignalOffGridRight::sMessageScope = scope;
}


int smSignalOffGridRight :: GetMessageIndex () const {

	return smSignalOffGridRight :: sMessageIndex;
}


int smSignalOffGridRight :: GetScope () const {

	return smSignalOffGridRight :: sMessageScope;
}


RGString smSignalOffGridRight :: GetName () const {

	return smSignalOffGridRight :: sName;
}



int smSignalOffGridRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSignalOffGridRight* noticeType = new smSignalOffGridRight;
	warehouse->AddType (noticeType);
	return 1;
}





sm0UnambiguousPeaksLeft :: sm0UnambiguousPeaksLeft () : SmartNotice () {

}


sm0UnambiguousPeaksLeft :: sm0UnambiguousPeaksLeft (const sm0UnambiguousPeaksLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



sm0UnambiguousPeaksLeft :: ~sm0UnambiguousPeaksLeft () {

}


int sm0UnambiguousPeaksLeft :: GetSubject () const {

	return sm0UnambiguousPeaksLeft::sSubject;
}


void sm0UnambiguousPeaksLeft :: SetIndexAndScope (int index, int scope) const {

	sm0UnambiguousPeaksLeft::sMessageIndex = index;
	sm0UnambiguousPeaksLeft::sMessageScope = scope;
}


int sm0UnambiguousPeaksLeft :: GetMessageIndex () const {

	return sm0UnambiguousPeaksLeft :: sMessageIndex;
}


int sm0UnambiguousPeaksLeft :: GetScope () const {

	return sm0UnambiguousPeaksLeft :: sMessageScope;
}


RGString sm0UnambiguousPeaksLeft :: GetName () const {

	return sm0UnambiguousPeaksLeft :: sName;
}



int sm0UnambiguousPeaksLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm0UnambiguousPeaksLeft* noticeType = new sm0UnambiguousPeaksLeft;
	warehouse->AddType (noticeType);
	return 1;
}





sm0UnambiguousPeaksRight :: sm0UnambiguousPeaksRight () : SmartNotice () {

}


sm0UnambiguousPeaksRight :: sm0UnambiguousPeaksRight (const sm0UnambiguousPeaksRight& note) : SmartNotice ((const SmartNotice&) note) {

}



sm0UnambiguousPeaksRight :: ~sm0UnambiguousPeaksRight () {

}


int sm0UnambiguousPeaksRight :: GetSubject () const {

	return sm0UnambiguousPeaksRight::sSubject;
}


void sm0UnambiguousPeaksRight :: SetIndexAndScope (int index, int scope) const {

	sm0UnambiguousPeaksRight::sMessageIndex = index;
	sm0UnambiguousPeaksRight::sMessageScope = scope;
}


int sm0UnambiguousPeaksRight :: GetMessageIndex () const {

	return sm0UnambiguousPeaksRight :: sMessageIndex;
}


int sm0UnambiguousPeaksRight :: GetScope () const {

	return sm0UnambiguousPeaksRight :: sMessageScope;
}


RGString sm0UnambiguousPeaksRight :: GetName () const {

	return sm0UnambiguousPeaksRight :: sName;
}



int sm0UnambiguousPeaksRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm0UnambiguousPeaksRight* noticeType = new sm0UnambiguousPeaksRight;
	warehouse->AddType (noticeType);
	return 1;
}





sm1UnambiguousPeakLeft :: sm1UnambiguousPeakLeft () : SmartNotice () {

}


sm1UnambiguousPeakLeft :: sm1UnambiguousPeakLeft (const sm1UnambiguousPeakLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



sm1UnambiguousPeakLeft :: ~sm1UnambiguousPeakLeft () {

}


int sm1UnambiguousPeakLeft :: GetSubject () const {

	return sm1UnambiguousPeakLeft::sSubject;
}


void sm1UnambiguousPeakLeft :: SetIndexAndScope (int index, int scope) const {

	sm1UnambiguousPeakLeft::sMessageIndex = index;
	sm1UnambiguousPeakLeft::sMessageScope = scope;
}


int sm1UnambiguousPeakLeft :: GetMessageIndex () const {

	return sm1UnambiguousPeakLeft :: sMessageIndex;
}


int sm1UnambiguousPeakLeft :: GetScope () const {

	return sm1UnambiguousPeakLeft :: sMessageScope;
}


RGString sm1UnambiguousPeakLeft :: GetName () const {

	return sm1UnambiguousPeakLeft :: sName;
}



int sm1UnambiguousPeakLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm1UnambiguousPeakLeft* noticeType = new sm1UnambiguousPeakLeft;
	warehouse->AddType (noticeType);
	return 1;
}





sm1UnambiguousPeakRight :: sm1UnambiguousPeakRight () : SmartNotice () {

}


sm1UnambiguousPeakRight :: sm1UnambiguousPeakRight (const sm1UnambiguousPeakRight& note) : SmartNotice ((const SmartNotice&) note) {

}



sm1UnambiguousPeakRight :: ~sm1UnambiguousPeakRight () {

}


int sm1UnambiguousPeakRight :: GetSubject () const {

	return sm1UnambiguousPeakRight::sSubject;
}


void sm1UnambiguousPeakRight :: SetIndexAndScope (int index, int scope) const {

	sm1UnambiguousPeakRight::sMessageIndex = index;
	sm1UnambiguousPeakRight::sMessageScope = scope;
}


int sm1UnambiguousPeakRight :: GetMessageIndex () const {

	return sm1UnambiguousPeakRight :: sMessageIndex;
}


int sm1UnambiguousPeakRight :: GetScope () const {

	return sm1UnambiguousPeakRight :: sMessageScope;
}


RGString sm1UnambiguousPeakRight :: GetName () const {

	return sm1UnambiguousPeakRight :: sName;
}



int sm1UnambiguousPeakRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm1UnambiguousPeakRight* noticeType = new sm1UnambiguousPeakRight;
	warehouse->AddType (noticeType);
	return 1;
}





sm2PlusUnambiguousPeaksLeft :: sm2PlusUnambiguousPeaksLeft () : SmartNotice () {

}


sm2PlusUnambiguousPeaksLeft :: sm2PlusUnambiguousPeaksLeft (const sm2PlusUnambiguousPeaksLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



sm2PlusUnambiguousPeaksLeft :: ~sm2PlusUnambiguousPeaksLeft () {

}


int sm2PlusUnambiguousPeaksLeft :: GetSubject () const {

	return sm2PlusUnambiguousPeaksLeft::sSubject;
}


void sm2PlusUnambiguousPeaksLeft :: SetIndexAndScope (int index, int scope) const {

	sm2PlusUnambiguousPeaksLeft::sMessageIndex = index;
	sm2PlusUnambiguousPeaksLeft::sMessageScope = scope;
}


int sm2PlusUnambiguousPeaksLeft :: GetMessageIndex () const {

	return sm2PlusUnambiguousPeaksLeft :: sMessageIndex;
}


int sm2PlusUnambiguousPeaksLeft :: GetScope () const {

	return sm2PlusUnambiguousPeaksLeft :: sMessageScope;
}


RGString sm2PlusUnambiguousPeaksLeft :: GetName () const {

	return sm2PlusUnambiguousPeaksLeft :: sName;
}



int sm2PlusUnambiguousPeaksLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm2PlusUnambiguousPeaksLeft* noticeType = new sm2PlusUnambiguousPeaksLeft;
	warehouse->AddType (noticeType);
	return 1;
}





sm2PlusUnambiguousPeaksRight :: sm2PlusUnambiguousPeaksRight () : SmartNotice () {

}


sm2PlusUnambiguousPeaksRight :: sm2PlusUnambiguousPeaksRight (const sm2PlusUnambiguousPeaksRight& note) : SmartNotice ((const SmartNotice&) note) {

}



sm2PlusUnambiguousPeaksRight :: ~sm2PlusUnambiguousPeaksRight () {

}


int sm2PlusUnambiguousPeaksRight :: GetSubject () const {

	return sm2PlusUnambiguousPeaksRight::sSubject;
}


void sm2PlusUnambiguousPeaksRight :: SetIndexAndScope (int index, int scope) const {

	sm2PlusUnambiguousPeaksRight::sMessageIndex = index;
	sm2PlusUnambiguousPeaksRight::sMessageScope = scope;
}


int sm2PlusUnambiguousPeaksRight :: GetMessageIndex () const {

	return sm2PlusUnambiguousPeaksRight :: sMessageIndex;
}


int sm2PlusUnambiguousPeaksRight :: GetScope () const {

	return sm2PlusUnambiguousPeaksRight :: sMessageScope;
}


RGString sm2PlusUnambiguousPeaksRight :: GetName () const {

	return sm2PlusUnambiguousPeaksRight :: sName;
}



int sm2PlusUnambiguousPeaksRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm2PlusUnambiguousPeaksRight* noticeType = new sm2PlusUnambiguousPeaksRight;
	warehouse->AddType (noticeType);
	return 1;
}





sm0AmbiguousPeaksLeft :: sm0AmbiguousPeaksLeft () : SmartNotice () {

}


sm0AmbiguousPeaksLeft :: sm0AmbiguousPeaksLeft (const sm0AmbiguousPeaksLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



sm0AmbiguousPeaksLeft :: ~sm0AmbiguousPeaksLeft () {

}


int sm0AmbiguousPeaksLeft :: GetSubject () const {

	return sm0AmbiguousPeaksLeft::sSubject;
}


void sm0AmbiguousPeaksLeft :: SetIndexAndScope (int index, int scope) const {

	sm0AmbiguousPeaksLeft::sMessageIndex = index;
	sm0AmbiguousPeaksLeft::sMessageScope = scope;
}


int sm0AmbiguousPeaksLeft :: GetMessageIndex () const {

	return sm0AmbiguousPeaksLeft :: sMessageIndex;
}


int sm0AmbiguousPeaksLeft :: GetScope () const {

	return sm0AmbiguousPeaksLeft :: sMessageScope;
}


RGString sm0AmbiguousPeaksLeft :: GetName () const {

	return sm0AmbiguousPeaksLeft :: sName;
}



int sm0AmbiguousPeaksLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm0AmbiguousPeaksLeft* noticeType = new sm0AmbiguousPeaksLeft;
	warehouse->AddType (noticeType);
	return 1;
}





sm0AmbiguousPeaksRight :: sm0AmbiguousPeaksRight () : SmartNotice () {

}


sm0AmbiguousPeaksRight :: sm0AmbiguousPeaksRight (const sm0AmbiguousPeaksRight& note) : SmartNotice ((const SmartNotice&) note) {

}



sm0AmbiguousPeaksRight :: ~sm0AmbiguousPeaksRight () {

}


int sm0AmbiguousPeaksRight :: GetSubject () const {

	return sm0AmbiguousPeaksRight::sSubject;
}


void sm0AmbiguousPeaksRight :: SetIndexAndScope (int index, int scope) const {

	sm0AmbiguousPeaksRight::sMessageIndex = index;
	sm0AmbiguousPeaksRight::sMessageScope = scope;
}


int sm0AmbiguousPeaksRight :: GetMessageIndex () const {

	return sm0AmbiguousPeaksRight :: sMessageIndex;
}


int sm0AmbiguousPeaksRight :: GetScope () const {

	return sm0AmbiguousPeaksRight :: sMessageScope;
}


RGString sm0AmbiguousPeaksRight :: GetName () const {

	return sm0AmbiguousPeaksRight :: sName;
}



int sm0AmbiguousPeaksRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm0AmbiguousPeaksRight* noticeType = new sm0AmbiguousPeaksRight;
	warehouse->AddType (noticeType);
	return 1;
}





sm1AmbiguousPeakLeft :: sm1AmbiguousPeakLeft () : SmartNotice () {

}


sm1AmbiguousPeakLeft :: sm1AmbiguousPeakLeft (const sm1AmbiguousPeakLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



sm1AmbiguousPeakLeft :: ~sm1AmbiguousPeakLeft () {

}


int sm1AmbiguousPeakLeft :: GetSubject () const {

	return sm1AmbiguousPeakLeft::sSubject;
}


void sm1AmbiguousPeakLeft :: SetIndexAndScope (int index, int scope) const {

	sm1AmbiguousPeakLeft::sMessageIndex = index;
	sm1AmbiguousPeakLeft::sMessageScope = scope;
}


int sm1AmbiguousPeakLeft :: GetMessageIndex () const {

	return sm1AmbiguousPeakLeft :: sMessageIndex;
}


int sm1AmbiguousPeakLeft :: GetScope () const {

	return sm1AmbiguousPeakLeft :: sMessageScope;
}


RGString sm1AmbiguousPeakLeft :: GetName () const {

	return sm1AmbiguousPeakLeft :: sName;
}



int sm1AmbiguousPeakLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm1AmbiguousPeakLeft* noticeType = new sm1AmbiguousPeakLeft;
	warehouse->AddType (noticeType);
	return 1;
}





sm1AmbiguousPeakRight :: sm1AmbiguousPeakRight () : SmartNotice () {

}


sm1AmbiguousPeakRight :: sm1AmbiguousPeakRight (const sm1AmbiguousPeakRight& note) : SmartNotice ((const SmartNotice&) note) {

}



sm1AmbiguousPeakRight :: ~sm1AmbiguousPeakRight () {

}


int sm1AmbiguousPeakRight :: GetSubject () const {

	return sm1AmbiguousPeakRight::sSubject;
}


void sm1AmbiguousPeakRight :: SetIndexAndScope (int index, int scope) const {

	sm1AmbiguousPeakRight::sMessageIndex = index;
	sm1AmbiguousPeakRight::sMessageScope = scope;
}


int sm1AmbiguousPeakRight :: GetMessageIndex () const {

	return sm1AmbiguousPeakRight :: sMessageIndex;
}


int sm1AmbiguousPeakRight :: GetScope () const {

	return sm1AmbiguousPeakRight :: sMessageScope;
}


RGString sm1AmbiguousPeakRight :: GetName () const {

	return sm1AmbiguousPeakRight :: sName;
}



int sm1AmbiguousPeakRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm1AmbiguousPeakRight* noticeType = new sm1AmbiguousPeakRight;
	warehouse->AddType (noticeType);
	return 1;
}





sm2PlusAmbiguousPeaksLeft :: sm2PlusAmbiguousPeaksLeft () : SmartNotice () {

}


sm2PlusAmbiguousPeaksLeft :: sm2PlusAmbiguousPeaksLeft (const sm2PlusAmbiguousPeaksLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



sm2PlusAmbiguousPeaksLeft :: ~sm2PlusAmbiguousPeaksLeft () {

}


int sm2PlusAmbiguousPeaksLeft :: GetSubject () const {

	return sm2PlusAmbiguousPeaksLeft::sSubject;
}


void sm2PlusAmbiguousPeaksLeft :: SetIndexAndScope (int index, int scope) const {

	sm2PlusAmbiguousPeaksLeft::sMessageIndex = index;
	sm2PlusAmbiguousPeaksLeft::sMessageScope = scope;
}


int sm2PlusAmbiguousPeaksLeft :: GetMessageIndex () const {

	return sm2PlusAmbiguousPeaksLeft :: sMessageIndex;
}


int sm2PlusAmbiguousPeaksLeft :: GetScope () const {

	return sm2PlusAmbiguousPeaksLeft :: sMessageScope;
}


RGString sm2PlusAmbiguousPeaksLeft :: GetName () const {

	return sm2PlusAmbiguousPeaksLeft :: sName;
}



int sm2PlusAmbiguousPeaksLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm2PlusAmbiguousPeaksLeft* noticeType = new sm2PlusAmbiguousPeaksLeft;
	warehouse->AddType (noticeType);
	return 1;
}





sm2PlusAmbiguousPeaksRight :: sm2PlusAmbiguousPeaksRight () : SmartNotice () {

}


sm2PlusAmbiguousPeaksRight :: sm2PlusAmbiguousPeaksRight (const sm2PlusAmbiguousPeaksRight& note) : SmartNotice ((const SmartNotice&) note) {

}



sm2PlusAmbiguousPeaksRight :: ~sm2PlusAmbiguousPeaksRight () {

}


int sm2PlusAmbiguousPeaksRight :: GetSubject () const {

	return sm2PlusAmbiguousPeaksRight::sSubject;
}


void sm2PlusAmbiguousPeaksRight :: SetIndexAndScope (int index, int scope) const {

	sm2PlusAmbiguousPeaksRight::sMessageIndex = index;
	sm2PlusAmbiguousPeaksRight::sMessageScope = scope;
}


int sm2PlusAmbiguousPeaksRight :: GetMessageIndex () const {

	return sm2PlusAmbiguousPeaksRight :: sMessageIndex;
}


int sm2PlusAmbiguousPeaksRight :: GetScope () const {

	return sm2PlusAmbiguousPeaksRight :: sMessageScope;
}


RGString sm2PlusAmbiguousPeaksRight :: GetName () const {

	return sm2PlusAmbiguousPeaksRight :: sName;
}



int sm2PlusAmbiguousPeaksRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	sm2PlusAmbiguousPeaksRight* noticeType = new sm2PlusAmbiguousPeaksRight;
	warehouse->AddType (noticeType);
	return 1;
}





smCouldBelongToLocusLeft :: smCouldBelongToLocusLeft () : SmartNotice () {

}


smCouldBelongToLocusLeft :: smCouldBelongToLocusLeft (const smCouldBelongToLocusLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smCouldBelongToLocusLeft :: ~smCouldBelongToLocusLeft () {

}


int smCouldBelongToLocusLeft :: GetSubject () const {

	return smCouldBelongToLocusLeft::sSubject;
}


void smCouldBelongToLocusLeft :: SetIndexAndScope (int index, int scope) const {

	smCouldBelongToLocusLeft::sMessageIndex = index;
	smCouldBelongToLocusLeft::sMessageScope = scope;
}


int smCouldBelongToLocusLeft :: GetMessageIndex () const {

	return smCouldBelongToLocusLeft :: sMessageIndex;
}


int smCouldBelongToLocusLeft :: GetScope () const {

	return smCouldBelongToLocusLeft :: sMessageScope;
}


RGString smCouldBelongToLocusLeft :: GetName () const {

	return smCouldBelongToLocusLeft :: sName;
}



int smCouldBelongToLocusLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCouldBelongToLocusLeft* noticeType = new smCouldBelongToLocusLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smCouldBelongToLocusRight :: smCouldBelongToLocusRight () : SmartNotice () {

}


smCouldBelongToLocusRight :: smCouldBelongToLocusRight (const smCouldBelongToLocusRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smCouldBelongToLocusRight :: ~smCouldBelongToLocusRight () {

}


int smCouldBelongToLocusRight :: GetSubject () const {

	return smCouldBelongToLocusRight::sSubject;
}


void smCouldBelongToLocusRight :: SetIndexAndScope (int index, int scope) const {

	smCouldBelongToLocusRight::sMessageIndex = index;
	smCouldBelongToLocusRight::sMessageScope = scope;
}


int smCouldBelongToLocusRight :: GetMessageIndex () const {

	return smCouldBelongToLocusRight :: sMessageIndex;
}


int smCouldBelongToLocusRight :: GetScope () const {

	return smCouldBelongToLocusRight :: sMessageScope;
}


RGString smCouldBelongToLocusRight :: GetName () const {

	return smCouldBelongToLocusRight :: sName;
}



int smCouldBelongToLocusRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCouldBelongToLocusRight* noticeType = new smCouldBelongToLocusRight;
	warehouse->AddType (noticeType);
	return 1;
}





smChannelIsNegControl :: smChannelIsNegControl () : SmartNotice () {

}


smChannelIsNegControl :: smChannelIsNegControl (const smChannelIsNegControl& note) : SmartNotice ((const SmartNotice&) note) {

}



smChannelIsNegControl :: ~smChannelIsNegControl () {

}


int smChannelIsNegControl :: GetSubject () const {

	return smChannelIsNegControl::sSubject;
}


void smChannelIsNegControl :: SetIndexAndScope (int index, int scope) const {

	smChannelIsNegControl::sMessageIndex = index;
	smChannelIsNegControl::sMessageScope = scope;
}


int smChannelIsNegControl :: GetMessageIndex () const {

	return smChannelIsNegControl :: sMessageIndex;
}


int smChannelIsNegControl :: GetScope () const {

	return smChannelIsNegControl :: sMessageScope;
}


RGString smChannelIsNegControl :: GetName () const {

	return smChannelIsNegControl :: sName;
}



int smChannelIsNegControl :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smChannelIsNegControl* noticeType = new smChannelIsNegControl;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusIsNegControl :: smLocusIsNegControl () : SmartNotice () {

}


smLocusIsNegControl :: smLocusIsNegControl (const smLocusIsNegControl& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusIsNegControl :: ~smLocusIsNegControl () {

}


int smLocusIsNegControl :: GetSubject () const {

	return smLocusIsNegControl::sSubject;
}


void smLocusIsNegControl :: SetIndexAndScope (int index, int scope) const {

	smLocusIsNegControl::sMessageIndex = index;
	smLocusIsNegControl::sMessageScope = scope;
}


int smLocusIsNegControl :: GetMessageIndex () const {

	return smLocusIsNegControl :: sMessageIndex;
}


int smLocusIsNegControl :: GetScope () const {

	return smLocusIsNegControl :: sMessageScope;
}


RGString smLocusIsNegControl :: GetName () const {

	return smLocusIsNegControl :: sName;
}



int smLocusIsNegControl :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusIsNegControl* noticeType = new smLocusIsNegControl;
	warehouse->AddType (noticeType);
	return 1;
}





smChannelIsPosControl :: smChannelIsPosControl () : SmartNotice () {

}


smChannelIsPosControl :: smChannelIsPosControl (const smChannelIsPosControl& note) : SmartNotice ((const SmartNotice&) note) {

}



smChannelIsPosControl :: ~smChannelIsPosControl () {

}


int smChannelIsPosControl :: GetSubject () const {

	return smChannelIsPosControl::sSubject;
}


void smChannelIsPosControl :: SetIndexAndScope (int index, int scope) const {

	smChannelIsPosControl::sMessageIndex = index;
	smChannelIsPosControl::sMessageScope = scope;
}


int smChannelIsPosControl :: GetMessageIndex () const {

	return smChannelIsPosControl :: sMessageIndex;
}


int smChannelIsPosControl :: GetScope () const {

	return smChannelIsPosControl :: sMessageScope;
}


RGString smChannelIsPosControl :: GetName () const {

	return smChannelIsPosControl :: sName;
}



int smChannelIsPosControl :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smChannelIsPosControl* noticeType = new smChannelIsPosControl;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusIsPosControl :: smLocusIsPosControl () : SmartNotice () {

}


smLocusIsPosControl :: smLocusIsPosControl (const smLocusIsPosControl& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusIsPosControl :: ~smLocusIsPosControl () {

}


int smLocusIsPosControl :: GetSubject () const {

	return smLocusIsPosControl::sSubject;
}


void smLocusIsPosControl :: SetIndexAndScope (int index, int scope) const {

	smLocusIsPosControl::sMessageIndex = index;
	smLocusIsPosControl::sMessageScope = scope;
}


int smLocusIsPosControl :: GetMessageIndex () const {

	return smLocusIsPosControl :: sMessageIndex;
}


int smLocusIsPosControl :: GetScope () const {

	return smLocusIsPosControl :: sMessageScope;
}


RGString smLocusIsPosControl :: GetName () const {

	return smLocusIsPosControl :: sName;
}



int smLocusIsPosControl :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusIsPosControl* noticeType = new smLocusIsPosControl;
	warehouse->AddType (noticeType);
	return 1;
}





smPurePullup :: smPurePullup () : SmartNotice () {

}


smPurePullup :: smPurePullup (const smPurePullup& note) : SmartNotice ((const SmartNotice&) note) {

}



smPurePullup :: ~smPurePullup () {

}


int smPurePullup :: GetSubject () const {

	return smPurePullup::sSubject;
}


void smPurePullup :: SetIndexAndScope (int index, int scope) const {

	smPurePullup::sMessageIndex = index;
	smPurePullup::sMessageScope = scope;
}


int smPurePullup :: GetMessageIndex () const {

	return smPurePullup :: sMessageIndex;
}


int smPurePullup :: GetScope () const {

	return smPurePullup :: sMessageScope;
}


RGString smPurePullup :: GetName () const {

	return smPurePullup :: sName;
}



int smPurePullup :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPurePullup* noticeType = new smPurePullup;
	warehouse->AddType (noticeType);
	return 1;
}





smChannelPeaksHaveCriticalArtifacts :: smChannelPeaksHaveCriticalArtifacts () : SmartNotice () {

}


smChannelPeaksHaveCriticalArtifacts :: smChannelPeaksHaveCriticalArtifacts (const smChannelPeaksHaveCriticalArtifacts& note) : SmartNotice ((const SmartNotice&) note) {

}



smChannelPeaksHaveCriticalArtifacts :: ~smChannelPeaksHaveCriticalArtifacts () {

}


int smChannelPeaksHaveCriticalArtifacts :: GetSubject () const {

	return smChannelPeaksHaveCriticalArtifacts::sSubject;
}


void smChannelPeaksHaveCriticalArtifacts :: SetIndexAndScope (int index, int scope) const {

	smChannelPeaksHaveCriticalArtifacts::sMessageIndex = index;
	smChannelPeaksHaveCriticalArtifacts::sMessageScope = scope;
}


int smChannelPeaksHaveCriticalArtifacts :: GetMessageIndex () const {

	return smChannelPeaksHaveCriticalArtifacts :: sMessageIndex;
}


int smChannelPeaksHaveCriticalArtifacts :: GetScope () const {

	return smChannelPeaksHaveCriticalArtifacts :: sMessageScope;
}


RGString smChannelPeaksHaveCriticalArtifacts :: GetName () const {

	return smChannelPeaksHaveCriticalArtifacts :: sName;
}



int smChannelPeaksHaveCriticalArtifacts :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smChannelPeaksHaveCriticalArtifacts* noticeType = new smChannelPeaksHaveCriticalArtifacts;
	warehouse->AddType (noticeType);
	return 1;
}





smPreliminaryLocusHas2OrMoreAlleles :: smPreliminaryLocusHas2OrMoreAlleles () : SmartNotice () {

}


smPreliminaryLocusHas2OrMoreAlleles :: smPreliminaryLocusHas2OrMoreAlleles (const smPreliminaryLocusHas2OrMoreAlleles& note) : SmartNotice ((const SmartNotice&) note) {

}



smPreliminaryLocusHas2OrMoreAlleles :: ~smPreliminaryLocusHas2OrMoreAlleles () {

}


int smPreliminaryLocusHas2OrMoreAlleles :: GetSubject () const {

	return smPreliminaryLocusHas2OrMoreAlleles::sSubject;
}


void smPreliminaryLocusHas2OrMoreAlleles :: SetIndexAndScope (int index, int scope) const {

	smPreliminaryLocusHas2OrMoreAlleles::sMessageIndex = index;
	smPreliminaryLocusHas2OrMoreAlleles::sMessageScope = scope;
}


int smPreliminaryLocusHas2OrMoreAlleles :: GetMessageIndex () const {

	return smPreliminaryLocusHas2OrMoreAlleles :: sMessageIndex;
}


int smPreliminaryLocusHas2OrMoreAlleles :: GetScope () const {

	return smPreliminaryLocusHas2OrMoreAlleles :: sMessageScope;
}


RGString smPreliminaryLocusHas2OrMoreAlleles :: GetName () const {

	return smPreliminaryLocusHas2OrMoreAlleles :: sName;
}



int smPreliminaryLocusHas2OrMoreAlleles :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPreliminaryLocusHas2OrMoreAlleles* noticeType = new smPreliminaryLocusHas2OrMoreAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





smCraterSidePeak :: smCraterSidePeak () : SmartNotice () {

}


smCraterSidePeak :: smCraterSidePeak (const smCraterSidePeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smCraterSidePeak :: ~smCraterSidePeak () {

}


int smCraterSidePeak :: GetSubject () const {

	return smCraterSidePeak::sSubject;
}


void smCraterSidePeak :: SetIndexAndScope (int index, int scope) const {

	smCraterSidePeak::sMessageIndex = index;
	smCraterSidePeak::sMessageScope = scope;
}


int smCraterSidePeak :: GetMessageIndex () const {

	return smCraterSidePeak :: sMessageIndex;
}


int smCraterSidePeak :: GetScope () const {

	return smCraterSidePeak :: sMessageScope;
}


RGString smCraterSidePeak :: GetName () const {

	return smCraterSidePeak :: sName;
}



int smCraterSidePeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCraterSidePeak* noticeType = new smCraterSidePeak;
	warehouse->AddType (noticeType);
	return 1;
}





smSignalNotACrater :: smSignalNotACrater () : SmartNotice () {

}


smSignalNotACrater :: smSignalNotACrater (const smSignalNotACrater& note) : SmartNotice ((const SmartNotice&) note) {

}



smSignalNotACrater :: ~smSignalNotACrater () {

}


int smSignalNotACrater :: GetSubject () const {

	return smSignalNotACrater::sSubject;
}


void smSignalNotACrater :: SetIndexAndScope (int index, int scope) const {

	smSignalNotACrater::sMessageIndex = index;
	smSignalNotACrater::sMessageScope = scope;
}


int smSignalNotACrater :: GetMessageIndex () const {

	return smSignalNotACrater :: sMessageIndex;
}


int smSignalNotACrater :: GetScope () const {

	return smSignalNotACrater :: sMessageScope;
}


RGString smSignalNotACrater :: GetName () const {

	return smSignalNotACrater :: sName;
}



int smSignalNotACrater :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSignalNotACrater* noticeType = new smSignalNotACrater;
	warehouse->AddType (noticeType);
	return 1;
}





smSignalNotACraterSidePeak :: smSignalNotACraterSidePeak () : SmartNotice () {

}


smSignalNotACraterSidePeak :: smSignalNotACraterSidePeak (const smSignalNotACraterSidePeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smSignalNotACraterSidePeak :: ~smSignalNotACraterSidePeak () {

}


int smSignalNotACraterSidePeak :: GetSubject () const {

	return smSignalNotACraterSidePeak::sSubject;
}


void smSignalNotACraterSidePeak :: SetIndexAndScope (int index, int scope) const {

	smSignalNotACraterSidePeak::sMessageIndex = index;
	smSignalNotACraterSidePeak::sMessageScope = scope;
}


int smSignalNotACraterSidePeak :: GetMessageIndex () const {

	return smSignalNotACraterSidePeak :: sMessageIndex;
}


int smSignalNotACraterSidePeak :: GetScope () const {

	return smSignalNotACraterSidePeak :: sMessageScope;
}


RGString smSignalNotACraterSidePeak :: GetName () const {

	return smSignalNotACraterSidePeak :: sName;
}



int smSignalNotACraterSidePeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSignalNotACraterSidePeak* noticeType = new smSignalNotACraterSidePeak;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusIsHomozygous :: smLocusIsHomozygous () : SmartNotice () {

}


smLocusIsHomozygous :: smLocusIsHomozygous (const smLocusIsHomozygous& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusIsHomozygous :: ~smLocusIsHomozygous () {

}


int smLocusIsHomozygous :: GetSubject () const {

	return smLocusIsHomozygous::sSubject;
}


void smLocusIsHomozygous :: SetIndexAndScope (int index, int scope) const {

	smLocusIsHomozygous::sMessageIndex = index;
	smLocusIsHomozygous::sMessageScope = scope;
}


int smLocusIsHomozygous :: GetMessageIndex () const {

	return smLocusIsHomozygous :: sMessageIndex;
}


int smLocusIsHomozygous :: GetScope () const {

	return smLocusIsHomozygous :: sMessageScope;
}


RGString smLocusIsHomozygous :: GetName () const {

	return smLocusIsHomozygous :: sName;
}



int smLocusIsHomozygous :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusIsHomozygous* noticeType = new smLocusIsHomozygous;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusMissingGenotype :: smLocusMissingGenotype () : SmartNotice () {

}


smLocusMissingGenotype :: smLocusMissingGenotype (const smLocusMissingGenotype& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusMissingGenotype :: ~smLocusMissingGenotype () {

}


int smLocusMissingGenotype :: GetSubject () const {

	return smLocusMissingGenotype::sSubject;
}


void smLocusMissingGenotype :: SetIndexAndScope (int index, int scope) const {

	smLocusMissingGenotype::sMessageIndex = index;
	smLocusMissingGenotype::sMessageScope = scope;
}


int smLocusMissingGenotype :: GetMessageIndex () const {

	return smLocusMissingGenotype :: sMessageIndex;
}


int smLocusMissingGenotype :: GetScope () const {

	return smLocusMissingGenotype :: sMessageScope;
}


RGString smLocusMissingGenotype :: GetName () const {

	return smLocusMissingGenotype :: sName;
}



int smLocusMissingGenotype :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusMissingGenotype* noticeType = new smLocusMissingGenotype;
	warehouse->AddType (noticeType);
	return 1;
}





smStage4Successful :: smStage4Successful () : SmartNotice () {

}


smStage4Successful :: smStage4Successful (const smStage4Successful& note) : SmartNotice ((const SmartNotice&) note) {

}



smStage4Successful :: ~smStage4Successful () {

}


int smStage4Successful :: GetSubject () const {

	return smStage4Successful::sSubject;
}


void smStage4Successful :: SetIndexAndScope (int index, int scope) const {

	smStage4Successful::sMessageIndex = index;
	smStage4Successful::sMessageScope = scope;
}


int smStage4Successful :: GetMessageIndex () const {

	return smStage4Successful :: sMessageIndex;
}


int smStage4Successful :: GetScope () const {

	return smStage4Successful :: sMessageScope;
}


RGString smStage4Successful :: GetName () const {

	return smStage4Successful :: sName;
}



int smStage4Successful :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smStage4Successful* noticeType = new smStage4Successful;
	warehouse->AddType (noticeType);
	return 1;
}





smStage5Successful :: smStage5Successful () : SmartNotice () {

}


smStage5Successful :: smStage5Successful (const smStage5Successful& note) : SmartNotice ((const SmartNotice&) note) {

}



smStage5Successful :: ~smStage5Successful () {

}


int smStage5Successful :: GetSubject () const {

	return smStage5Successful::sSubject;
}


void smStage5Successful :: SetIndexAndScope (int index, int scope) const {

	smStage5Successful::sMessageIndex = index;
	smStage5Successful::sMessageScope = scope;
}


int smStage5Successful :: GetMessageIndex () const {

	return smStage5Successful :: sMessageIndex;
}


int smStage5Successful :: GetScope () const {

	return smStage5Successful :: sMessageScope;
}


RGString smStage5Successful :: GetName () const {

	return smStage5Successful :: sName;
}



int smStage5Successful :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smStage5Successful* noticeType = new smStage5Successful;
	warehouse->AddType (noticeType);
	return 1;
}





smPeakInCoreLadderLocus :: smPeakInCoreLadderLocus () : SmartNotice () {

}


smPeakInCoreLadderLocus :: smPeakInCoreLadderLocus (const smPeakInCoreLadderLocus& note) : SmartNotice ((const SmartNotice&) note) {

}



smPeakInCoreLadderLocus :: ~smPeakInCoreLadderLocus () {

}


int smPeakInCoreLadderLocus :: GetSubject () const {

	return smPeakInCoreLadderLocus::sSubject;
}


void smPeakInCoreLadderLocus :: SetIndexAndScope (int index, int scope) const {

	smPeakInCoreLadderLocus::sMessageIndex = index;
	smPeakInCoreLadderLocus::sMessageScope = scope;
}


int smPeakInCoreLadderLocus :: GetMessageIndex () const {

	return smPeakInCoreLadderLocus :: sMessageIndex;
}


int smPeakInCoreLadderLocus :: GetScope () const {

	return smPeakInCoreLadderLocus :: sMessageScope;
}


RGString smPeakInCoreLadderLocus :: GetName () const {

	return smPeakInCoreLadderLocus :: sName;
}



int smPeakInCoreLadderLocus :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPeakInCoreLadderLocus* noticeType = new smPeakInCoreLadderLocus;
	warehouse->AddType (noticeType);
	return 1;
}





smPeakAboveOverloadLimit :: smPeakAboveOverloadLimit () : SmartNotice () {

}


smPeakAboveOverloadLimit :: smPeakAboveOverloadLimit (const smPeakAboveOverloadLimit& note) : SmartNotice ((const SmartNotice&) note) {

}



smPeakAboveOverloadLimit :: ~smPeakAboveOverloadLimit () {

}


int smPeakAboveOverloadLimit :: GetSubject () const {

	return smPeakAboveOverloadLimit::sSubject;
}


void smPeakAboveOverloadLimit :: SetIndexAndScope (int index, int scope) const {

	smPeakAboveOverloadLimit::sMessageIndex = index;
	smPeakAboveOverloadLimit::sMessageScope = scope;
}


int smPeakAboveOverloadLimit :: GetMessageIndex () const {

	return smPeakAboveOverloadLimit :: sMessageIndex;
}


int smPeakAboveOverloadLimit :: GetScope () const {

	return smPeakAboveOverloadLimit :: sMessageScope;
}


RGString smPeakAboveOverloadLimit :: GetName () const {

	return smPeakAboveOverloadLimit :: sName;
}



int smPeakAboveOverloadLimit :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPeakAboveOverloadLimit* noticeType = new smPeakAboveOverloadLimit;
	warehouse->AddType (noticeType);
	return 1;
}





smRestrictedPriorityPeak :: smRestrictedPriorityPeak () : SmartNotice () {

}


smRestrictedPriorityPeak :: smRestrictedPriorityPeak (const smRestrictedPriorityPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smRestrictedPriorityPeak :: ~smRestrictedPriorityPeak () {

}


int smRestrictedPriorityPeak :: GetSubject () const {

	return smRestrictedPriorityPeak::sSubject;
}


void smRestrictedPriorityPeak :: SetIndexAndScope (int index, int scope) const {

	smRestrictedPriorityPeak::sMessageIndex = index;
	smRestrictedPriorityPeak::sMessageScope = scope;
}


int smRestrictedPriorityPeak :: GetMessageIndex () const {

	return smRestrictedPriorityPeak :: sMessageIndex;
}


int smRestrictedPriorityPeak :: GetScope () const {

	return smRestrictedPriorityPeak :: sMessageScope;
}


RGString smRestrictedPriorityPeak :: GetName () const {

	return smRestrictedPriorityPeak :: sName;
}



int smRestrictedPriorityPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smRestrictedPriorityPeak* noticeType = new smRestrictedPriorityPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smChannelIsILS :: smChannelIsILS () : SmartNotice () {

}


smChannelIsILS :: smChannelIsILS (const smChannelIsILS& note) : SmartNotice ((const SmartNotice&) note) {

}



smChannelIsILS :: ~smChannelIsILS () {

}


int smChannelIsILS :: GetSubject () const {

	return smChannelIsILS::sSubject;
}


void smChannelIsILS :: SetIndexAndScope (int index, int scope) const {

	smChannelIsILS::sMessageIndex = index;
	smChannelIsILS::sMessageScope = scope;
}


int smChannelIsILS :: GetMessageIndex () const {

	return smChannelIsILS :: sMessageIndex;
}


int smChannelIsILS :: GetScope () const {

	return smChannelIsILS :: sMessageScope;
}


RGString smChannelIsILS :: GetName () const {

	return smChannelIsILS :: sName;
}



int smChannelIsILS :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smChannelIsILS* noticeType = new smChannelIsILS;
	warehouse->AddType (noticeType);
	return 1;
}





smILSSpacingIsMarginal :: smILSSpacingIsMarginal () : SmartNotice () {

}


smILSSpacingIsMarginal :: smILSSpacingIsMarginal (const smILSSpacingIsMarginal& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSSpacingIsMarginal :: ~smILSSpacingIsMarginal () {

}


int smILSSpacingIsMarginal :: GetSubject () const {

	return smILSSpacingIsMarginal::sSubject;
}


void smILSSpacingIsMarginal :: SetIndexAndScope (int index, int scope) const {

	smILSSpacingIsMarginal::sMessageIndex = index;
	smILSSpacingIsMarginal::sMessageScope = scope;
}


int smILSSpacingIsMarginal :: GetMessageIndex () const {

	return smILSSpacingIsMarginal :: sMessageIndex;
}


int smILSSpacingIsMarginal :: GetScope () const {

	return smILSSpacingIsMarginal :: sMessageScope;
}


RGString smILSSpacingIsMarginal :: GetName () const {

	return smILSSpacingIsMarginal :: sName;
}



int smILSSpacingIsMarginal :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSSpacingIsMarginal* noticeType = new smILSSpacingIsMarginal;
	warehouse->AddType (noticeType);
	return 1;
}





smPoorPeakMorphologyOrResolution :: smPoorPeakMorphologyOrResolution () : SmartNotice () {

}


smPoorPeakMorphologyOrResolution :: smPoorPeakMorphologyOrResolution (const smPoorPeakMorphologyOrResolution& note) : SmartNotice ((const SmartNotice&) note) {

}



smPoorPeakMorphologyOrResolution :: ~smPoorPeakMorphologyOrResolution () {

}


int smPoorPeakMorphologyOrResolution :: GetSubject () const {

	return smPoorPeakMorphologyOrResolution::sSubject;
}


void smPoorPeakMorphologyOrResolution :: SetIndexAndScope (int index, int scope) const {

	smPoorPeakMorphologyOrResolution::sMessageIndex = index;
	smPoorPeakMorphologyOrResolution::sMessageScope = scope;
}


int smPoorPeakMorphologyOrResolution :: GetMessageIndex () const {

	return smPoorPeakMorphologyOrResolution :: sMessageIndex;
}


int smPoorPeakMorphologyOrResolution :: GetScope () const {

	return smPoorPeakMorphologyOrResolution :: sMessageScope;
}


RGString smPoorPeakMorphologyOrResolution :: GetName () const {

	return smPoorPeakMorphologyOrResolution :: sName;
}



int smPoorPeakMorphologyOrResolution :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPoorPeakMorphologyOrResolution* noticeType = new smPoorPeakMorphologyOrResolution;
	warehouse->AddType (noticeType);
	return 1;
}





smPeakToRightOfILS :: smPeakToRightOfILS () : SmartNotice () {

}


smPeakToRightOfILS :: smPeakToRightOfILS (const smPeakToRightOfILS& note) : SmartNotice ((const SmartNotice&) note) {

}



smPeakToRightOfILS :: ~smPeakToRightOfILS () {

}


int smPeakToRightOfILS :: GetSubject () const {

	return smPeakToRightOfILS::sSubject;
}


void smPeakToRightOfILS :: SetIndexAndScope (int index, int scope) const {

	smPeakToRightOfILS::sMessageIndex = index;
	smPeakToRightOfILS::sMessageScope = scope;
}


int smPeakToRightOfILS :: GetMessageIndex () const {

	return smPeakToRightOfILS :: sMessageIndex;
}


int smPeakToRightOfILS :: GetScope () const {

	return smPeakToRightOfILS :: sMessageScope;
}


RGString smPeakToRightOfILS :: GetName () const {

	return smPeakToRightOfILS :: sName;
}



int smPeakToRightOfILS :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPeakToRightOfILS* noticeType = new smPeakToRightOfILS;
	warehouse->AddType (noticeType);
	return 1;
}





smPreliminaryLocusHasNoAlleles :: smPreliminaryLocusHasNoAlleles () : SmartNotice () {

}


smPreliminaryLocusHasNoAlleles :: smPreliminaryLocusHasNoAlleles (const smPreliminaryLocusHasNoAlleles& note) : SmartNotice ((const SmartNotice&) note) {

}



smPreliminaryLocusHasNoAlleles :: ~smPreliminaryLocusHasNoAlleles () {

}


int smPreliminaryLocusHasNoAlleles :: GetSubject () const {

	return smPreliminaryLocusHasNoAlleles::sSubject;
}


void smPreliminaryLocusHasNoAlleles :: SetIndexAndScope (int index, int scope) const {

	smPreliminaryLocusHasNoAlleles::sMessageIndex = index;
	smPreliminaryLocusHasNoAlleles::sMessageScope = scope;
}


int smPreliminaryLocusHasNoAlleles :: GetMessageIndex () const {

	return smPreliminaryLocusHasNoAlleles :: sMessageIndex;
}


int smPreliminaryLocusHasNoAlleles :: GetScope () const {

	return smPreliminaryLocusHasNoAlleles :: sMessageScope;
}


RGString smPreliminaryLocusHasNoAlleles :: GetName () const {

	return smPreliminaryLocusHasNoAlleles :: sName;
}



int smPreliminaryLocusHasNoAlleles :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPreliminaryLocusHasNoAlleles* noticeType = new smPreliminaryLocusHasNoAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: smLocusHasPeaksBetweenDetectionAndAnalysisThresholds () : SmartNotice () {

}


smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: smLocusHasPeaksBetweenDetectionAndAnalysisThresholds (const smLocusHasPeaksBetweenDetectionAndAnalysisThresholds& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: ~smLocusHasPeaksBetweenDetectionAndAnalysisThresholds () {

}


int smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: GetSubject () const {

	return smLocusHasPeaksBetweenDetectionAndAnalysisThresholds::sSubject;
}


void smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: SetIndexAndScope (int index, int scope) const {

	smLocusHasPeaksBetweenDetectionAndAnalysisThresholds::sMessageIndex = index;
	smLocusHasPeaksBetweenDetectionAndAnalysisThresholds::sMessageScope = scope;
}


int smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: GetMessageIndex () const {

	return smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: sMessageIndex;
}


int smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: GetScope () const {

	return smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: sMessageScope;
}


RGString smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: GetName () const {

	return smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: sName;
}



int smLocusHasPeaksBetweenDetectionAndAnalysisThresholds :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusHasPeaksBetweenDetectionAndAnalysisThresholds* noticeType = new smLocusHasPeaksBetweenDetectionAndAnalysisThresholds;
	warehouse->AddType (noticeType);
	return 1;
}





smUnexpectedNoOfGridPeaks :: smUnexpectedNoOfGridPeaks () : SmartNotice () {

}


smUnexpectedNoOfGridPeaks :: smUnexpectedNoOfGridPeaks (const smUnexpectedNoOfGridPeaks& note) : SmartNotice ((const SmartNotice&) note) {

}



smUnexpectedNoOfGridPeaks :: ~smUnexpectedNoOfGridPeaks () {

}


int smUnexpectedNoOfGridPeaks :: GetSubject () const {

	return smUnexpectedNoOfGridPeaks::sSubject;
}


void smUnexpectedNoOfGridPeaks :: SetIndexAndScope (int index, int scope) const {

	smUnexpectedNoOfGridPeaks::sMessageIndex = index;
	smUnexpectedNoOfGridPeaks::sMessageScope = scope;
}


int smUnexpectedNoOfGridPeaks :: GetMessageIndex () const {

	return smUnexpectedNoOfGridPeaks :: sMessageIndex;
}


int smUnexpectedNoOfGridPeaks :: GetScope () const {

	return smUnexpectedNoOfGridPeaks :: sMessageScope;
}


RGString smUnexpectedNoOfGridPeaks :: GetName () const {

	return smUnexpectedNoOfGridPeaks :: sName;
}



int smUnexpectedNoOfGridPeaks :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUnexpectedNoOfGridPeaks* noticeType = new smUnexpectedNoOfGridPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





smMoreThanThreeAlleles :: smMoreThanThreeAlleles () : SmartNotice () {

}


smMoreThanThreeAlleles :: smMoreThanThreeAlleles (const smMoreThanThreeAlleles& note) : SmartNotice ((const SmartNotice&) note) {

}



smMoreThanThreeAlleles :: ~smMoreThanThreeAlleles () {

}


int smMoreThanThreeAlleles :: GetSubject () const {

	return smMoreThanThreeAlleles::sSubject;
}


void smMoreThanThreeAlleles :: SetIndexAndScope (int index, int scope) const {

	smMoreThanThreeAlleles::sMessageIndex = index;
	smMoreThanThreeAlleles::sMessageScope = scope;
}


int smMoreThanThreeAlleles :: GetMessageIndex () const {

	return smMoreThanThreeAlleles :: sMessageIndex;
}


int smMoreThanThreeAlleles :: GetScope () const {

	return smMoreThanThreeAlleles :: sMessageScope;
}


RGString smMoreThanThreeAlleles :: GetName () const {

	return smMoreThanThreeAlleles :: sName;
}



int smMoreThanThreeAlleles :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMoreThanThreeAlleles* noticeType = new smMoreThanThreeAlleles;
	warehouse->AddType (noticeType);
	return 1;
}





smDefaultsAreOverridden :: smDefaultsAreOverridden () : SmartNotice () {

}


smDefaultsAreOverridden :: smDefaultsAreOverridden (const smDefaultsAreOverridden& note) : SmartNotice ((const SmartNotice&) note) {

}



smDefaultsAreOverridden :: ~smDefaultsAreOverridden () {

}


int smDefaultsAreOverridden :: GetSubject () const {

	return smDefaultsAreOverridden::sSubject;
}


void smDefaultsAreOverridden :: SetIndexAndScope (int index, int scope) const {

	smDefaultsAreOverridden::sMessageIndex = index;
	smDefaultsAreOverridden::sMessageScope = scope;
}


int smDefaultsAreOverridden :: GetMessageIndex () const {

	return smDefaultsAreOverridden :: sMessageIndex;
}


int smDefaultsAreOverridden :: GetScope () const {

	return smDefaultsAreOverridden :: sMessageScope;
}


RGString smDefaultsAreOverridden :: GetName () const {

	return smDefaultsAreOverridden :: sName;
}



int smDefaultsAreOverridden :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smDefaultsAreOverridden* noticeType = new smDefaultsAreOverridden;
	warehouse->AddType (noticeType);
	return 1;
}





smAssociatedLadderIsCritical :: smAssociatedLadderIsCritical () : SmartNotice () {

}


smAssociatedLadderIsCritical :: smAssociatedLadderIsCritical (const smAssociatedLadderIsCritical& note) : SmartNotice ((const SmartNotice&) note) {

}



smAssociatedLadderIsCritical :: ~smAssociatedLadderIsCritical () {

}


int smAssociatedLadderIsCritical :: GetSubject () const {

	return smAssociatedLadderIsCritical::sSubject;
}


void smAssociatedLadderIsCritical :: SetIndexAndScope (int index, int scope) const {

	smAssociatedLadderIsCritical::sMessageIndex = index;
	smAssociatedLadderIsCritical::sMessageScope = scope;
}


int smAssociatedLadderIsCritical :: GetMessageIndex () const {

	return smAssociatedLadderIsCritical :: sMessageIndex;
}


int smAssociatedLadderIsCritical :: GetScope () const {

	return smAssociatedLadderIsCritical :: sMessageScope;
}


RGString smAssociatedLadderIsCritical :: GetName () const {

	return smAssociatedLadderIsCritical :: sName;
}



int smAssociatedLadderIsCritical :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAssociatedLadderIsCritical* noticeType = new smAssociatedLadderIsCritical;
	warehouse->AddType (noticeType);
	return 1;
}





smAssociatedLadderLocusIsCritical :: smAssociatedLadderLocusIsCritical () : SmartNotice () {

}


smAssociatedLadderLocusIsCritical :: smAssociatedLadderLocusIsCritical (const smAssociatedLadderLocusIsCritical& note) : SmartNotice ((const SmartNotice&) note) {

}



smAssociatedLadderLocusIsCritical :: ~smAssociatedLadderLocusIsCritical () {

}


int smAssociatedLadderLocusIsCritical :: GetSubject () const {

	return smAssociatedLadderLocusIsCritical::sSubject;
}


void smAssociatedLadderLocusIsCritical :: SetIndexAndScope (int index, int scope) const {

	smAssociatedLadderLocusIsCritical::sMessageIndex = index;
	smAssociatedLadderLocusIsCritical::sMessageScope = scope;
}


int smAssociatedLadderLocusIsCritical :: GetMessageIndex () const {

	return smAssociatedLadderLocusIsCritical :: sMessageIndex;
}


int smAssociatedLadderLocusIsCritical :: GetScope () const {

	return smAssociatedLadderLocusIsCritical :: sMessageScope;
}


RGString smAssociatedLadderLocusIsCritical :: GetName () const {

	return smAssociatedLadderLocusIsCritical :: sName;
}



int smAssociatedLadderLocusIsCritical :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAssociatedLadderLocusIsCritical* noticeType = new smAssociatedLadderLocusIsCritical;
	warehouse->AddType (noticeType);
	return 1;
}





smChannelHasRaisedBaseline :: smChannelHasRaisedBaseline () : SmartNotice () {

}


smChannelHasRaisedBaseline :: smChannelHasRaisedBaseline (const smChannelHasRaisedBaseline& note) : SmartNotice ((const SmartNotice&) note) {

}



smChannelHasRaisedBaseline :: ~smChannelHasRaisedBaseline () {

}


int smChannelHasRaisedBaseline :: GetSubject () const {

	return smChannelHasRaisedBaseline::sSubject;
}


void smChannelHasRaisedBaseline :: SetIndexAndScope (int index, int scope) const {

	smChannelHasRaisedBaseline::sMessageIndex = index;
	smChannelHasRaisedBaseline::sMessageScope = scope;
}


int smChannelHasRaisedBaseline :: GetMessageIndex () const {

	return smChannelHasRaisedBaseline :: sMessageIndex;
}


int smChannelHasRaisedBaseline :: GetScope () const {

	return smChannelHasRaisedBaseline :: sMessageScope;
}


RGString smChannelHasRaisedBaseline :: GetName () const {

	return smChannelHasRaisedBaseline :: sName;
}



int smChannelHasRaisedBaseline :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smChannelHasRaisedBaseline* noticeType = new smChannelHasRaisedBaseline;
	warehouse->AddType (noticeType);
	return 1;
}





smChannelHasExcessiveNoiseAboveAnalysisThreshold :: smChannelHasExcessiveNoiseAboveAnalysisThreshold () : SmartNotice () {

}


smChannelHasExcessiveNoiseAboveAnalysisThreshold :: smChannelHasExcessiveNoiseAboveAnalysisThreshold (const smChannelHasExcessiveNoiseAboveAnalysisThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smChannelHasExcessiveNoiseAboveAnalysisThreshold :: ~smChannelHasExcessiveNoiseAboveAnalysisThreshold () {

}


int smChannelHasExcessiveNoiseAboveAnalysisThreshold :: GetSubject () const {

	return smChannelHasExcessiveNoiseAboveAnalysisThreshold::sSubject;
}


void smChannelHasExcessiveNoiseAboveAnalysisThreshold :: SetIndexAndScope (int index, int scope) const {

	smChannelHasExcessiveNoiseAboveAnalysisThreshold::sMessageIndex = index;
	smChannelHasExcessiveNoiseAboveAnalysisThreshold::sMessageScope = scope;
}


int smChannelHasExcessiveNoiseAboveAnalysisThreshold :: GetMessageIndex () const {

	return smChannelHasExcessiveNoiseAboveAnalysisThreshold :: sMessageIndex;
}


int smChannelHasExcessiveNoiseAboveAnalysisThreshold :: GetScope () const {

	return smChannelHasExcessiveNoiseAboveAnalysisThreshold :: sMessageScope;
}


RGString smChannelHasExcessiveNoiseAboveAnalysisThreshold :: GetName () const {

	return smChannelHasExcessiveNoiseAboveAnalysisThreshold :: sName;
}



int smChannelHasExcessiveNoiseAboveAnalysisThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smChannelHasExcessiveNoiseAboveAnalysisThreshold* noticeType = new smChannelHasExcessiveNoiseAboveAnalysisThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smChannelHasExcessiveNoiseAboveDetectionThreshold :: smChannelHasExcessiveNoiseAboveDetectionThreshold () : SmartNotice () {

}


smChannelHasExcessiveNoiseAboveDetectionThreshold :: smChannelHasExcessiveNoiseAboveDetectionThreshold (const smChannelHasExcessiveNoiseAboveDetectionThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smChannelHasExcessiveNoiseAboveDetectionThreshold :: ~smChannelHasExcessiveNoiseAboveDetectionThreshold () {

}


int smChannelHasExcessiveNoiseAboveDetectionThreshold :: GetSubject () const {

	return smChannelHasExcessiveNoiseAboveDetectionThreshold::sSubject;
}


void smChannelHasExcessiveNoiseAboveDetectionThreshold :: SetIndexAndScope (int index, int scope) const {

	smChannelHasExcessiveNoiseAboveDetectionThreshold::sMessageIndex = index;
	smChannelHasExcessiveNoiseAboveDetectionThreshold::sMessageScope = scope;
}


int smChannelHasExcessiveNoiseAboveDetectionThreshold :: GetMessageIndex () const {

	return smChannelHasExcessiveNoiseAboveDetectionThreshold :: sMessageIndex;
}


int smChannelHasExcessiveNoiseAboveDetectionThreshold :: GetScope () const {

	return smChannelHasExcessiveNoiseAboveDetectionThreshold :: sMessageScope;
}


RGString smChannelHasExcessiveNoiseAboveDetectionThreshold :: GetName () const {

	return smChannelHasExcessiveNoiseAboveDetectionThreshold :: sName;
}



int smChannelHasExcessiveNoiseAboveDetectionThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smChannelHasExcessiveNoiseAboveDetectionThreshold* noticeType = new smChannelHasExcessiveNoiseAboveDetectionThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smSampleChannelRaisedBaselineThreshold :: smSampleChannelRaisedBaselineThreshold () : SmartNotice () {

}


smSampleChannelRaisedBaselineThreshold :: smSampleChannelRaisedBaselineThreshold (const smSampleChannelRaisedBaselineThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smSampleChannelRaisedBaselineThreshold :: ~smSampleChannelRaisedBaselineThreshold () {

}


int smSampleChannelRaisedBaselineThreshold :: GetSubject () const {

	return smSampleChannelRaisedBaselineThreshold::sSubject;
}


void smSampleChannelRaisedBaselineThreshold :: SetIndexAndScope (int index, int scope) const {

	smSampleChannelRaisedBaselineThreshold::sMessageIndex = index;
	smSampleChannelRaisedBaselineThreshold::sMessageScope = scope;
}


int smSampleChannelRaisedBaselineThreshold :: GetMessageIndex () const {

	return smSampleChannelRaisedBaselineThreshold :: sMessageIndex;
}


int smSampleChannelRaisedBaselineThreshold :: GetScope () const {

	return smSampleChannelRaisedBaselineThreshold :: sMessageScope;
}


RGString smSampleChannelRaisedBaselineThreshold :: GetName () const {

	return smSampleChannelRaisedBaselineThreshold :: sName;
}



int smSampleChannelRaisedBaselineThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSampleChannelRaisedBaselineThreshold* noticeType = new smSampleChannelRaisedBaselineThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smSampleILSChannelRaisedBaselineThreshold :: smSampleILSChannelRaisedBaselineThreshold () : SmartNotice () {

}


smSampleILSChannelRaisedBaselineThreshold :: smSampleILSChannelRaisedBaselineThreshold (const smSampleILSChannelRaisedBaselineThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smSampleILSChannelRaisedBaselineThreshold :: ~smSampleILSChannelRaisedBaselineThreshold () {

}


int smSampleILSChannelRaisedBaselineThreshold :: GetSubject () const {

	return smSampleILSChannelRaisedBaselineThreshold::sSubject;
}


void smSampleILSChannelRaisedBaselineThreshold :: SetIndexAndScope (int index, int scope) const {

	smSampleILSChannelRaisedBaselineThreshold::sMessageIndex = index;
	smSampleILSChannelRaisedBaselineThreshold::sMessageScope = scope;
}


int smSampleILSChannelRaisedBaselineThreshold :: GetMessageIndex () const {

	return smSampleILSChannelRaisedBaselineThreshold :: sMessageIndex;
}


int smSampleILSChannelRaisedBaselineThreshold :: GetScope () const {

	return smSampleILSChannelRaisedBaselineThreshold :: sMessageScope;
}


RGString smSampleILSChannelRaisedBaselineThreshold :: GetName () const {

	return smSampleILSChannelRaisedBaselineThreshold :: sName;
}



int smSampleILSChannelRaisedBaselineThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSampleILSChannelRaisedBaselineThreshold* noticeType = new smSampleILSChannelRaisedBaselineThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smILSPeakMayBeIncorrect :: smILSPeakMayBeIncorrect () : SmartNotice () {

}


smILSPeakMayBeIncorrect :: smILSPeakMayBeIncorrect (const smILSPeakMayBeIncorrect& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSPeakMayBeIncorrect :: ~smILSPeakMayBeIncorrect () {

}


int smILSPeakMayBeIncorrect :: GetSubject () const {

	return smILSPeakMayBeIncorrect::sSubject;
}


void smILSPeakMayBeIncorrect :: SetIndexAndScope (int index, int scope) const {

	smILSPeakMayBeIncorrect::sMessageIndex = index;
	smILSPeakMayBeIncorrect::sMessageScope = scope;
}


int smILSPeakMayBeIncorrect :: GetMessageIndex () const {

	return smILSPeakMayBeIncorrect :: sMessageIndex;
}


int smILSPeakMayBeIncorrect :: GetScope () const {

	return smILSPeakMayBeIncorrect :: sMessageScope;
}


RGString smILSPeakMayBeIncorrect :: GetName () const {

	return smILSPeakMayBeIncorrect :: sName;
}



int smILSPeakMayBeIncorrect :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSPeakMayBeIncorrect* noticeType = new smILSPeakMayBeIncorrect;
	warehouse->AddType (noticeType);
	return 1;
}





smTestForColorCorrectionMatrixPreset :: smTestForColorCorrectionMatrixPreset () : SmartNotice () {

}


smTestForColorCorrectionMatrixPreset :: smTestForColorCorrectionMatrixPreset (const smTestForColorCorrectionMatrixPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smTestForColorCorrectionMatrixPreset :: ~smTestForColorCorrectionMatrixPreset () {

}


int smTestForColorCorrectionMatrixPreset :: GetSubject () const {

	return smTestForColorCorrectionMatrixPreset::sSubject;
}


void smTestForColorCorrectionMatrixPreset :: SetIndexAndScope (int index, int scope) const {

	smTestForColorCorrectionMatrixPreset::sMessageIndex = index;
	smTestForColorCorrectionMatrixPreset::sMessageScope = scope;
}


int smTestForColorCorrectionMatrixPreset :: GetMessageIndex () const {

	return smTestForColorCorrectionMatrixPreset :: sMessageIndex;
}


int smTestForColorCorrectionMatrixPreset :: GetScope () const {

	return smTestForColorCorrectionMatrixPreset :: sMessageScope;
}


RGString smTestForColorCorrectionMatrixPreset :: GetName () const {

	return smTestForColorCorrectionMatrixPreset :: sName;
}



int smTestForColorCorrectionMatrixPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smTestForColorCorrectionMatrixPreset* noticeType = new smTestForColorCorrectionMatrixPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smColorCorrectionMatrixExpectedButNotFound :: smColorCorrectionMatrixExpectedButNotFound () : SmartNotice () {

}


smColorCorrectionMatrixExpectedButNotFound :: smColorCorrectionMatrixExpectedButNotFound (const smColorCorrectionMatrixExpectedButNotFound& note) : SmartNotice ((const SmartNotice&) note) {

}



smColorCorrectionMatrixExpectedButNotFound :: ~smColorCorrectionMatrixExpectedButNotFound () {

}


int smColorCorrectionMatrixExpectedButNotFound :: GetSubject () const {

	return smColorCorrectionMatrixExpectedButNotFound::sSubject;
}


void smColorCorrectionMatrixExpectedButNotFound :: SetIndexAndScope (int index, int scope) const {

	smColorCorrectionMatrixExpectedButNotFound::sMessageIndex = index;
	smColorCorrectionMatrixExpectedButNotFound::sMessageScope = scope;
}


int smColorCorrectionMatrixExpectedButNotFound :: GetMessageIndex () const {

	return smColorCorrectionMatrixExpectedButNotFound :: sMessageIndex;
}


int smColorCorrectionMatrixExpectedButNotFound :: GetScope () const {

	return smColorCorrectionMatrixExpectedButNotFound :: sMessageScope;
}


RGString smColorCorrectionMatrixExpectedButNotFound :: GetName () const {

	return smColorCorrectionMatrixExpectedButNotFound :: sName;
}



int smColorCorrectionMatrixExpectedButNotFound :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smColorCorrectionMatrixExpectedButNotFound* noticeType = new smColorCorrectionMatrixExpectedButNotFound;
	warehouse->AddType (noticeType);
	return 1;
}





smColorCorrectionMatrixWrongSize :: smColorCorrectionMatrixWrongSize () : SmartNotice () {

}


smColorCorrectionMatrixWrongSize :: smColorCorrectionMatrixWrongSize (const smColorCorrectionMatrixWrongSize& note) : SmartNotice ((const SmartNotice&) note) {

}



smColorCorrectionMatrixWrongSize :: ~smColorCorrectionMatrixWrongSize () {

}


int smColorCorrectionMatrixWrongSize :: GetSubject () const {

	return smColorCorrectionMatrixWrongSize::sSubject;
}


void smColorCorrectionMatrixWrongSize :: SetIndexAndScope (int index, int scope) const {

	smColorCorrectionMatrixWrongSize::sMessageIndex = index;
	smColorCorrectionMatrixWrongSize::sMessageScope = scope;
}


int smColorCorrectionMatrixWrongSize :: GetMessageIndex () const {

	return smColorCorrectionMatrixWrongSize :: sMessageIndex;
}


int smColorCorrectionMatrixWrongSize :: GetScope () const {

	return smColorCorrectionMatrixWrongSize :: sMessageScope;
}


RGString smColorCorrectionMatrixWrongSize :: GetName () const {

	return smColorCorrectionMatrixWrongSize :: sName;
}



int smColorCorrectionMatrixWrongSize :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smColorCorrectionMatrixWrongSize* noticeType = new smColorCorrectionMatrixWrongSize;
	warehouse->AddType (noticeType);
	return 1;
}





smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing () : SmartNotice () {

}


smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing (const smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing& note) : SmartNotice ((const SmartNotice&) note) {

}



smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: ~smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing () {

}


int smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: GetSubject () const {

	return smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing::sSubject;
}


void smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: SetIndexAndScope (int index, int scope) const {

	smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing::sMessageIndex = index;
	smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing::sMessageScope = scope;
}


int smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: GetMessageIndex () const {

	return smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: sMessageIndex;
}


int smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: GetScope () const {

	return smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: sMessageScope;
}


RGString smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: GetName () const {

	return smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: sName;
}



int smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing* noticeType = new smIgnoreNoiseAnalysisAboveDetectionThresholdInSmoothing;
	warehouse->AddType (noticeType);
	return 1;
}





smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: smMinimumNumberOfPrimerPeaksForNegativeControlChannel () : SmartNotice () {

}


smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: smMinimumNumberOfPrimerPeaksForNegativeControlChannel (const smMinimumNumberOfPrimerPeaksForNegativeControlChannel& note) : SmartNotice ((const SmartNotice&) note) {

}



smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: ~smMinimumNumberOfPrimerPeaksForNegativeControlChannel () {

}


int smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: GetSubject () const {

	return smMinimumNumberOfPrimerPeaksForNegativeControlChannel::sSubject;
}


void smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: SetIndexAndScope (int index, int scope) const {

	smMinimumNumberOfPrimerPeaksForNegativeControlChannel::sMessageIndex = index;
	smMinimumNumberOfPrimerPeaksForNegativeControlChannel::sMessageScope = scope;
}


int smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: GetMessageIndex () const {

	return smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: sMessageIndex;
}


int smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: GetScope () const {

	return smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: sMessageScope;
}


RGString smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: GetName () const {

	return smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: sName;
}



int smMinimumNumberOfPrimerPeaksForNegativeControlChannel :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMinimumNumberOfPrimerPeaksForNegativeControlChannel* noticeType = new smMinimumNumberOfPrimerPeaksForNegativeControlChannel;
	warehouse->AddType (noticeType);
	return 1;
}





smUseSampleNamesForControlSampleTestsPreset :: smUseSampleNamesForControlSampleTestsPreset () : SmartNotice () {

}


smUseSampleNamesForControlSampleTestsPreset :: smUseSampleNamesForControlSampleTestsPreset (const smUseSampleNamesForControlSampleTestsPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smUseSampleNamesForControlSampleTestsPreset :: ~smUseSampleNamesForControlSampleTestsPreset () {

}


int smUseSampleNamesForControlSampleTestsPreset :: GetSubject () const {

	return smUseSampleNamesForControlSampleTestsPreset::sSubject;
}


void smUseSampleNamesForControlSampleTestsPreset :: SetIndexAndScope (int index, int scope) const {

	smUseSampleNamesForControlSampleTestsPreset::sMessageIndex = index;
	smUseSampleNamesForControlSampleTestsPreset::sMessageScope = scope;
}


int smUseSampleNamesForControlSampleTestsPreset :: GetMessageIndex () const {

	return smUseSampleNamesForControlSampleTestsPreset :: sMessageIndex;
}


int smUseSampleNamesForControlSampleTestsPreset :: GetScope () const {

	return smUseSampleNamesForControlSampleTestsPreset :: sMessageScope;
}


RGString smUseSampleNamesForControlSampleTestsPreset :: GetName () const {

	return smUseSampleNamesForControlSampleTestsPreset :: sName;
}



int smUseSampleNamesForControlSampleTestsPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUseSampleNamesForControlSampleTestsPreset* noticeType = new smUseSampleNamesForControlSampleTestsPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smNoYForAMEL :: smNoYForAMEL () : SmartNotice () {

}


smNoYForAMEL :: smNoYForAMEL (const smNoYForAMEL& note) : SmartNotice ((const SmartNotice&) note) {

}



smNoYForAMEL :: ~smNoYForAMEL () {

}


int smNoYForAMEL :: GetSubject () const {

	return smNoYForAMEL::sSubject;
}


void smNoYForAMEL :: SetIndexAndScope (int index, int scope) const {

	smNoYForAMEL::sMessageIndex = index;
	smNoYForAMEL::sMessageScope = scope;
}


int smNoYForAMEL :: GetMessageIndex () const {

	return smNoYForAMEL :: sMessageIndex;
}


int smNoYForAMEL :: GetScope () const {

	return smNoYForAMEL :: sMessageScope;
}


RGString smNoYForAMEL :: GetName () const {

	return smNoYForAMEL :: sName;
}



int smNoYForAMEL :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNoYForAMEL* noticeType = new smNoYForAMEL;
	warehouse->AddType (noticeType);
	return 1;
}





smLocusIsYLinked :: smLocusIsYLinked () : SmartNotice () {

}


smLocusIsYLinked :: smLocusIsYLinked (const smLocusIsYLinked& note) : SmartNotice ((const SmartNotice&) note) {

}



smLocusIsYLinked :: ~smLocusIsYLinked () {

}


int smLocusIsYLinked :: GetSubject () const {

	return smLocusIsYLinked::sSubject;
}


void smLocusIsYLinked :: SetIndexAndScope (int index, int scope) const {

	smLocusIsYLinked::sMessageIndex = index;
	smLocusIsYLinked::sMessageScope = scope;
}


int smLocusIsYLinked :: GetMessageIndex () const {

	return smLocusIsYLinked :: sMessageIndex;
}


int smLocusIsYLinked :: GetScope () const {

	return smLocusIsYLinked :: sMessageScope;
}


RGString smLocusIsYLinked :: GetName () const {

	return smLocusIsYLinked :: sName;
}



int smLocusIsYLinked :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLocusIsYLinked* noticeType = new smLocusIsYLinked;
	warehouse->AddType (noticeType);
	return 1;
}





smNumberAllelesBelowExpectation :: smNumberAllelesBelowExpectation () : SmartNotice () {

}


smNumberAllelesBelowExpectation :: smNumberAllelesBelowExpectation (const smNumberAllelesBelowExpectation& note) : SmartNotice ((const SmartNotice&) note) {

}



smNumberAllelesBelowExpectation :: ~smNumberAllelesBelowExpectation () {

}


int smNumberAllelesBelowExpectation :: GetSubject () const {

	return smNumberAllelesBelowExpectation::sSubject;
}


void smNumberAllelesBelowExpectation :: SetIndexAndScope (int index, int scope) const {

	smNumberAllelesBelowExpectation::sMessageIndex = index;
	smNumberAllelesBelowExpectation::sMessageScope = scope;
}


int smNumberAllelesBelowExpectation :: GetMessageIndex () const {

	return smNumberAllelesBelowExpectation :: sMessageIndex;
}


int smNumberAllelesBelowExpectation :: GetScope () const {

	return smNumberAllelesBelowExpectation :: sMessageScope;
}


RGString smNumberAllelesBelowExpectation :: GetName () const {

	return smNumberAllelesBelowExpectation :: sName;
}



int smNumberAllelesBelowExpectation :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNumberAllelesBelowExpectation* noticeType = new smNumberAllelesBelowExpectation;
	warehouse->AddType (noticeType);
	return 1;
}





smBaselineRelativeStutter :: smBaselineRelativeStutter () : SmartNotice () {

}


smBaselineRelativeStutter :: smBaselineRelativeStutter (const smBaselineRelativeStutter& note) : SmartNotice ((const SmartNotice&) note) {

}



smBaselineRelativeStutter :: ~smBaselineRelativeStutter () {

}


int smBaselineRelativeStutter :: GetSubject () const {

	return smBaselineRelativeStutter::sSubject;
}


void smBaselineRelativeStutter :: SetIndexAndScope (int index, int scope) const {

	smBaselineRelativeStutter::sMessageIndex = index;
	smBaselineRelativeStutter::sMessageScope = scope;
}


int smBaselineRelativeStutter :: GetMessageIndex () const {

	return smBaselineRelativeStutter :: sMessageIndex;
}


int smBaselineRelativeStutter :: GetScope () const {

	return smBaselineRelativeStutter :: sMessageScope;
}


RGString smBaselineRelativeStutter :: GetName () const {

	return smBaselineRelativeStutter :: sName;
}



int smBaselineRelativeStutter :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBaselineRelativeStutter* noticeType = new smBaselineRelativeStutter;
	warehouse->AddType (noticeType);
	return 1;
}





smBaselineRelativeAdenylation :: smBaselineRelativeAdenylation () : SmartNotice () {

}


smBaselineRelativeAdenylation :: smBaselineRelativeAdenylation (const smBaselineRelativeAdenylation& note) : SmartNotice ((const SmartNotice&) note) {

}



smBaselineRelativeAdenylation :: ~smBaselineRelativeAdenylation () {

}


int smBaselineRelativeAdenylation :: GetSubject () const {

	return smBaselineRelativeAdenylation::sSubject;
}


void smBaselineRelativeAdenylation :: SetIndexAndScope (int index, int scope) const {

	smBaselineRelativeAdenylation::sMessageIndex = index;
	smBaselineRelativeAdenylation::sMessageScope = scope;
}


int smBaselineRelativeAdenylation :: GetMessageIndex () const {

	return smBaselineRelativeAdenylation :: sMessageIndex;
}


int smBaselineRelativeAdenylation :: GetScope () const {

	return smBaselineRelativeAdenylation :: sMessageScope;
}


RGString smBaselineRelativeAdenylation :: GetName () const {

	return smBaselineRelativeAdenylation :: sName;
}



int smBaselineRelativeAdenylation :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBaselineRelativeAdenylation* noticeType = new smBaselineRelativeAdenylation;
	warehouse->AddType (noticeType);
	return 1;
}





smBaselineRelativeBelowMinRFU :: smBaselineRelativeBelowMinRFU () : SmartNotice () {

}


smBaselineRelativeBelowMinRFU :: smBaselineRelativeBelowMinRFU (const smBaselineRelativeBelowMinRFU& note) : SmartNotice ((const SmartNotice&) note) {

}



smBaselineRelativeBelowMinRFU :: ~smBaselineRelativeBelowMinRFU () {

}


int smBaselineRelativeBelowMinRFU :: GetSubject () const {

	return smBaselineRelativeBelowMinRFU::sSubject;
}


void smBaselineRelativeBelowMinRFU :: SetIndexAndScope (int index, int scope) const {

	smBaselineRelativeBelowMinRFU::sMessageIndex = index;
	smBaselineRelativeBelowMinRFU::sMessageScope = scope;
}


int smBaselineRelativeBelowMinRFU :: GetMessageIndex () const {

	return smBaselineRelativeBelowMinRFU :: sMessageIndex;
}


int smBaselineRelativeBelowMinRFU :: GetScope () const {

	return smBaselineRelativeBelowMinRFU :: sMessageScope;
}


RGString smBaselineRelativeBelowMinRFU :: GetName () const {

	return smBaselineRelativeBelowMinRFU :: sName;
}



int smBaselineRelativeBelowMinRFU :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBaselineRelativeBelowMinRFU* noticeType = new smBaselineRelativeBelowMinRFU;
	warehouse->AddType (noticeType);
	return 1;
}





smBaselineRelativeBelowMinInterlocusRFU :: smBaselineRelativeBelowMinInterlocusRFU () : SmartNotice () {

}


smBaselineRelativeBelowMinInterlocusRFU :: smBaselineRelativeBelowMinInterlocusRFU (const smBaselineRelativeBelowMinInterlocusRFU& note) : SmartNotice ((const SmartNotice&) note) {

}



smBaselineRelativeBelowMinInterlocusRFU :: ~smBaselineRelativeBelowMinInterlocusRFU () {

}


int smBaselineRelativeBelowMinInterlocusRFU :: GetSubject () const {

	return smBaselineRelativeBelowMinInterlocusRFU::sSubject;
}


void smBaselineRelativeBelowMinInterlocusRFU :: SetIndexAndScope (int index, int scope) const {

	smBaselineRelativeBelowMinInterlocusRFU::sMessageIndex = index;
	smBaselineRelativeBelowMinInterlocusRFU::sMessageScope = scope;
}


int smBaselineRelativeBelowMinInterlocusRFU :: GetMessageIndex () const {

	return smBaselineRelativeBelowMinInterlocusRFU :: sMessageIndex;
}


int smBaselineRelativeBelowMinInterlocusRFU :: GetScope () const {

	return smBaselineRelativeBelowMinInterlocusRFU :: sMessageScope;
}


RGString smBaselineRelativeBelowMinInterlocusRFU :: GetName () const {

	return smBaselineRelativeBelowMinInterlocusRFU :: sName;
}



int smBaselineRelativeBelowMinInterlocusRFU :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBaselineRelativeBelowMinInterlocusRFU* noticeType = new smBaselineRelativeBelowMinInterlocusRFU;
	warehouse->AddType (noticeType);
	return 1;
}





smTestRelativeBaselinePreset :: smTestRelativeBaselinePreset () : SmartNotice () {

}


smTestRelativeBaselinePreset :: smTestRelativeBaselinePreset (const smTestRelativeBaselinePreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smTestRelativeBaselinePreset :: ~smTestRelativeBaselinePreset () {

}


int smTestRelativeBaselinePreset :: GetSubject () const {

	return smTestRelativeBaselinePreset::sSubject;
}


void smTestRelativeBaselinePreset :: SetIndexAndScope (int index, int scope) const {

	smTestRelativeBaselinePreset::sMessageIndex = index;
	smTestRelativeBaselinePreset::sMessageScope = scope;
}


int smTestRelativeBaselinePreset :: GetMessageIndex () const {

	return smTestRelativeBaselinePreset :: sMessageIndex;
}


int smTestRelativeBaselinePreset :: GetScope () const {

	return smTestRelativeBaselinePreset :: sMessageScope;
}


RGString smTestRelativeBaselinePreset :: GetName () const {

	return smTestRelativeBaselinePreset :: sName;
}



int smTestRelativeBaselinePreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smTestRelativeBaselinePreset* noticeType = new smTestRelativeBaselinePreset;
	warehouse->AddType (noticeType);
	return 1;
}





smIgnoreNegativeRelativeBaselinePreset :: smIgnoreNegativeRelativeBaselinePreset () : SmartNotice () {

}


smIgnoreNegativeRelativeBaselinePreset :: smIgnoreNegativeRelativeBaselinePreset (const smIgnoreNegativeRelativeBaselinePreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smIgnoreNegativeRelativeBaselinePreset :: ~smIgnoreNegativeRelativeBaselinePreset () {

}


int smIgnoreNegativeRelativeBaselinePreset :: GetSubject () const {

	return smIgnoreNegativeRelativeBaselinePreset::sSubject;
}


void smIgnoreNegativeRelativeBaselinePreset :: SetIndexAndScope (int index, int scope) const {

	smIgnoreNegativeRelativeBaselinePreset::sMessageIndex = index;
	smIgnoreNegativeRelativeBaselinePreset::sMessageScope = scope;
}


int smIgnoreNegativeRelativeBaselinePreset :: GetMessageIndex () const {

	return smIgnoreNegativeRelativeBaselinePreset :: sMessageIndex;
}


int smIgnoreNegativeRelativeBaselinePreset :: GetScope () const {

	return smIgnoreNegativeRelativeBaselinePreset :: sMessageScope;
}


RGString smIgnoreNegativeRelativeBaselinePreset :: GetName () const {

	return smIgnoreNegativeRelativeBaselinePreset :: sName;
}



int smIgnoreNegativeRelativeBaselinePreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smIgnoreNegativeRelativeBaselinePreset* noticeType = new smIgnoreNegativeRelativeBaselinePreset;
	warehouse->AddType (noticeType);
	return 1;
}





smNormalizeRawDataRelativeToBaselinePreset :: smNormalizeRawDataRelativeToBaselinePreset () : SmartNotice () {

}


smNormalizeRawDataRelativeToBaselinePreset :: smNormalizeRawDataRelativeToBaselinePreset (const smNormalizeRawDataRelativeToBaselinePreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smNormalizeRawDataRelativeToBaselinePreset :: ~smNormalizeRawDataRelativeToBaselinePreset () {

}


int smNormalizeRawDataRelativeToBaselinePreset :: GetSubject () const {

	return smNormalizeRawDataRelativeToBaselinePreset::sSubject;
}


void smNormalizeRawDataRelativeToBaselinePreset :: SetIndexAndScope (int index, int scope) const {

	smNormalizeRawDataRelativeToBaselinePreset::sMessageIndex = index;
	smNormalizeRawDataRelativeToBaselinePreset::sMessageScope = scope;
}


int smNormalizeRawDataRelativeToBaselinePreset :: GetMessageIndex () const {

	return smNormalizeRawDataRelativeToBaselinePreset :: sMessageIndex;
}


int smNormalizeRawDataRelativeToBaselinePreset :: GetScope () const {

	return smNormalizeRawDataRelativeToBaselinePreset :: sMessageScope;
}


RGString smNormalizeRawDataRelativeToBaselinePreset :: GetName () const {

	return smNormalizeRawDataRelativeToBaselinePreset :: sName;
}



int smNormalizeRawDataRelativeToBaselinePreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNormalizeRawDataRelativeToBaselinePreset* noticeType = new smNormalizeRawDataRelativeToBaselinePreset;
	warehouse->AddType (noticeType);
	return 1;
}





smPercentOfStandardNoiseThreshold :: smPercentOfStandardNoiseThreshold () : SmartNotice () {

}


smPercentOfStandardNoiseThreshold :: smPercentOfStandardNoiseThreshold (const smPercentOfStandardNoiseThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smPercentOfStandardNoiseThreshold :: ~smPercentOfStandardNoiseThreshold () {

}


int smPercentOfStandardNoiseThreshold :: GetSubject () const {

	return smPercentOfStandardNoiseThreshold::sSubject;
}


void smPercentOfStandardNoiseThreshold :: SetIndexAndScope (int index, int scope) const {

	smPercentOfStandardNoiseThreshold::sMessageIndex = index;
	smPercentOfStandardNoiseThreshold::sMessageScope = scope;
}


int smPercentOfStandardNoiseThreshold :: GetMessageIndex () const {

	return smPercentOfStandardNoiseThreshold :: sMessageIndex;
}


int smPercentOfStandardNoiseThreshold :: GetScope () const {

	return smPercentOfStandardNoiseThreshold :: sMessageScope;
}


RGString smPercentOfStandardNoiseThreshold :: GetName () const {

	return smPercentOfStandardNoiseThreshold :: sName;
}



int smPercentOfStandardNoiseThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPercentOfStandardNoiseThreshold* noticeType = new smPercentOfStandardNoiseThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smLadderFitThreshold :: smLadderFitThreshold () : SmartNotice () {

}


smLadderFitThreshold :: smLadderFitThreshold (const smLadderFitThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smLadderFitThreshold :: ~smLadderFitThreshold () {

}


int smLadderFitThreshold :: GetSubject () const {

	return smLadderFitThreshold::sSubject;
}


void smLadderFitThreshold :: SetIndexAndScope (int index, int scope) const {

	smLadderFitThreshold::sMessageIndex = index;
	smLadderFitThreshold::sMessageScope = scope;
}


int smLadderFitThreshold :: GetMessageIndex () const {

	return smLadderFitThreshold :: sMessageIndex;
}


int smLadderFitThreshold :: GetScope () const {

	return smLadderFitThreshold :: sMessageScope;
}


RGString smLadderFitThreshold :: GetName () const {

	return smLadderFitThreshold :: sName;
}



int smLadderFitThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLadderFitThreshold* noticeType = new smLadderFitThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smSampleToLadderFitBelowExpectations :: smSampleToLadderFitBelowExpectations () : SmartNotice () {

}


smSampleToLadderFitBelowExpectations :: smSampleToLadderFitBelowExpectations (const smSampleToLadderFitBelowExpectations& note) : SmartNotice ((const SmartNotice&) note) {

}



smSampleToLadderFitBelowExpectations :: ~smSampleToLadderFitBelowExpectations () {

}


int smSampleToLadderFitBelowExpectations :: GetSubject () const {

	return smSampleToLadderFitBelowExpectations::sSubject;
}


void smSampleToLadderFitBelowExpectations :: SetIndexAndScope (int index, int scope) const {

	smSampleToLadderFitBelowExpectations::sMessageIndex = index;
	smSampleToLadderFitBelowExpectations::sMessageScope = scope;
}


int smSampleToLadderFitBelowExpectations :: GetMessageIndex () const {

	return smSampleToLadderFitBelowExpectations :: sMessageIndex;
}


int smSampleToLadderFitBelowExpectations :: GetScope () const {

	return smSampleToLadderFitBelowExpectations :: sMessageScope;
}


RGString smSampleToLadderFitBelowExpectations :: GetName () const {

	return smSampleToLadderFitBelowExpectations :: sName;
}



int smSampleToLadderFitBelowExpectations :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSampleToLadderFitBelowExpectations* noticeType = new smSampleToLadderFitBelowExpectations;
	warehouse->AddType (noticeType);
	return 1;
}





smEnableRawDataFilterForNormalizationPreset :: smEnableRawDataFilterForNormalizationPreset () : SmartNotice () {

}


smEnableRawDataFilterForNormalizationPreset :: smEnableRawDataFilterForNormalizationPreset (const smEnableRawDataFilterForNormalizationPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smEnableRawDataFilterForNormalizationPreset :: ~smEnableRawDataFilterForNormalizationPreset () {

}


int smEnableRawDataFilterForNormalizationPreset :: GetSubject () const {

	return smEnableRawDataFilterForNormalizationPreset::sSubject;
}


void smEnableRawDataFilterForNormalizationPreset :: SetIndexAndScope (int index, int scope) const {

	smEnableRawDataFilterForNormalizationPreset::sMessageIndex = index;
	smEnableRawDataFilterForNormalizationPreset::sMessageScope = scope;
}


int smEnableRawDataFilterForNormalizationPreset :: GetMessageIndex () const {

	return smEnableRawDataFilterForNormalizationPreset :: sMessageIndex;
}


int smEnableRawDataFilterForNormalizationPreset :: GetScope () const {

	return smEnableRawDataFilterForNormalizationPreset :: sMessageScope;
}


RGString smEnableRawDataFilterForNormalizationPreset :: GetName () const {

	return smEnableRawDataFilterForNormalizationPreset :: sName;
}



int smEnableRawDataFilterForNormalizationPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smEnableRawDataFilterForNormalizationPreset* noticeType = new smEnableRawDataFilterForNormalizationPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smBaselineEstimationThreshold :: smBaselineEstimationThreshold () : SmartNotice () {

}


smBaselineEstimationThreshold :: smBaselineEstimationThreshold (const smBaselineEstimationThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smBaselineEstimationThreshold :: ~smBaselineEstimationThreshold () {

}


int smBaselineEstimationThreshold :: GetSubject () const {

	return smBaselineEstimationThreshold::sSubject;
}


void smBaselineEstimationThreshold :: SetIndexAndScope (int index, int scope) const {

	smBaselineEstimationThreshold::sMessageIndex = index;
	smBaselineEstimationThreshold::sMessageScope = scope;
}


int smBaselineEstimationThreshold :: GetMessageIndex () const {

	return smBaselineEstimationThreshold :: sMessageIndex;
}


int smBaselineEstimationThreshold :: GetScope () const {

	return smBaselineEstimationThreshold :: sMessageScope;
}


RGString smBaselineEstimationThreshold :: GetName () const {

	return smBaselineEstimationThreshold :: sName;
}



int smBaselineEstimationThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smBaselineEstimationThreshold* noticeType = new smBaselineEstimationThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smSinglePassFilterWindowWidth :: smSinglePassFilterWindowWidth () : SmartNotice () {

}


smSinglePassFilterWindowWidth :: smSinglePassFilterWindowWidth (const smSinglePassFilterWindowWidth& note) : SmartNotice ((const SmartNotice&) note) {

}



smSinglePassFilterWindowWidth :: ~smSinglePassFilterWindowWidth () {

}


int smSinglePassFilterWindowWidth :: GetSubject () const {

	return smSinglePassFilterWindowWidth::sSubject;
}


void smSinglePassFilterWindowWidth :: SetIndexAndScope (int index, int scope) const {

	smSinglePassFilterWindowWidth::sMessageIndex = index;
	smSinglePassFilterWindowWidth::sMessageScope = scope;
}


int smSinglePassFilterWindowWidth :: GetMessageIndex () const {

	return smSinglePassFilterWindowWidth :: sMessageIndex;
}


int smSinglePassFilterWindowWidth :: GetScope () const {

	return smSinglePassFilterWindowWidth :: sMessageScope;
}


RGString smSinglePassFilterWindowWidth :: GetName () const {

	return smSinglePassFilterWindowWidth :: sName;
}



int smSinglePassFilterWindowWidth :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSinglePassFilterWindowWidth* noticeType = new smSinglePassFilterWindowWidth;
	warehouse->AddType (noticeType);
	return 1;
}





smAmbiguousInterlocusWithPoorMorphology :: smAmbiguousInterlocusWithPoorMorphology () : SmartNotice () {

}


smAmbiguousInterlocusWithPoorMorphology :: smAmbiguousInterlocusWithPoorMorphology (const smAmbiguousInterlocusWithPoorMorphology& note) : SmartNotice ((const SmartNotice&) note) {

}



smAmbiguousInterlocusWithPoorMorphology :: ~smAmbiguousInterlocusWithPoorMorphology () {

}


int smAmbiguousInterlocusWithPoorMorphology :: GetSubject () const {

	return smAmbiguousInterlocusWithPoorMorphology::sSubject;
}


void smAmbiguousInterlocusWithPoorMorphology :: SetIndexAndScope (int index, int scope) const {

	smAmbiguousInterlocusWithPoorMorphology::sMessageIndex = index;
	smAmbiguousInterlocusWithPoorMorphology::sMessageScope = scope;
}


int smAmbiguousInterlocusWithPoorMorphology :: GetMessageIndex () const {

	return smAmbiguousInterlocusWithPoorMorphology :: sMessageIndex;
}


int smAmbiguousInterlocusWithPoorMorphology :: GetScope () const {

	return smAmbiguousInterlocusWithPoorMorphology :: sMessageScope;
}


RGString smAmbiguousInterlocusWithPoorMorphology :: GetName () const {

	return smAmbiguousInterlocusWithPoorMorphology :: sName;
}



int smAmbiguousInterlocusWithPoorMorphology :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAmbiguousInterlocusWithPoorMorphology* noticeType = new smAmbiguousInterlocusWithPoorMorphology;
	warehouse->AddType (noticeType);
	return 1;
}





smSigmoidalPullup :: smSigmoidalPullup () : SmartNotice () {

}


smSigmoidalPullup :: smSigmoidalPullup (const smSigmoidalPullup& note) : SmartNotice ((const SmartNotice&) note) {

}



smSigmoidalPullup :: ~smSigmoidalPullup () {

}


int smSigmoidalPullup :: GetSubject () const {

	return smSigmoidalPullup::sSubject;
}


void smSigmoidalPullup :: SetIndexAndScope (int index, int scope) const {

	smSigmoidalPullup::sMessageIndex = index;
	smSigmoidalPullup::sMessageScope = scope;
}


int smSigmoidalPullup :: GetMessageIndex () const {

	return smSigmoidalPullup :: sMessageIndex;
}


int smSigmoidalPullup :: GetScope () const {

	return smSigmoidalPullup :: sMessageScope;
}


RGString smSigmoidalPullup :: GetName () const {

	return smSigmoidalPullup :: sName;
}



int smSigmoidalPullup :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSigmoidalPullup* noticeType = new smSigmoidalPullup;
	warehouse->AddType (noticeType);
	return 1;
}





smMinPrimaryPullupThreshold :: smMinPrimaryPullupThreshold () : SmartNotice () {

}


smMinPrimaryPullupThreshold :: smMinPrimaryPullupThreshold (const smMinPrimaryPullupThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smMinPrimaryPullupThreshold :: ~smMinPrimaryPullupThreshold () {

}


int smMinPrimaryPullupThreshold :: GetSubject () const {

	return smMinPrimaryPullupThreshold::sSubject;
}


void smMinPrimaryPullupThreshold :: SetIndexAndScope (int index, int scope) const {

	smMinPrimaryPullupThreshold::sMessageIndex = index;
	smMinPrimaryPullupThreshold::sMessageScope = scope;
}


int smMinPrimaryPullupThreshold :: GetMessageIndex () const {

	return smMinPrimaryPullupThreshold :: sMessageIndex;
}


int smMinPrimaryPullupThreshold :: GetScope () const {

	return smMinPrimaryPullupThreshold :: sMessageScope;
}


RGString smMinPrimaryPullupThreshold :: GetName () const {

	return smMinPrimaryPullupThreshold :: sName;
}



int smMinPrimaryPullupThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMinPrimaryPullupThreshold* noticeType = new smMinPrimaryPullupThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smExcessiveResidualDisplacement :: smExcessiveResidualDisplacement () : SmartNotice () {

}


smExcessiveResidualDisplacement :: smExcessiveResidualDisplacement (const smExcessiveResidualDisplacement& note) : SmartNotice ((const SmartNotice&) note) {

}



smExcessiveResidualDisplacement :: ~smExcessiveResidualDisplacement () {

}


int smExcessiveResidualDisplacement :: GetSubject () const {

	return smExcessiveResidualDisplacement::sSubject;
}


void smExcessiveResidualDisplacement :: SetIndexAndScope (int index, int scope) const {

	smExcessiveResidualDisplacement::sMessageIndex = index;
	smExcessiveResidualDisplacement::sMessageScope = scope;
}


int smExcessiveResidualDisplacement :: GetMessageIndex () const {

	return smExcessiveResidualDisplacement :: sMessageIndex;
}


int smExcessiveResidualDisplacement :: GetScope () const {

	return smExcessiveResidualDisplacement :: sMessageScope;
}


RGString smExcessiveResidualDisplacement :: GetName () const {

	return smExcessiveResidualDisplacement :: sName;
}



int smExcessiveResidualDisplacement :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smExcessiveResidualDisplacement* noticeType = new smExcessiveResidualDisplacement;
	warehouse->AddType (noticeType);
	return 1;
}





smResidualDisplacementThreshold :: smResidualDisplacementThreshold () : SmartNotice () {

}


smResidualDisplacementThreshold :: smResidualDisplacementThreshold (const smResidualDisplacementThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smResidualDisplacementThreshold :: ~smResidualDisplacementThreshold () {

}


int smResidualDisplacementThreshold :: GetSubject () const {

	return smResidualDisplacementThreshold::sSubject;
}


void smResidualDisplacementThreshold :: SetIndexAndScope (int index, int scope) const {

	smResidualDisplacementThreshold::sMessageIndex = index;
	smResidualDisplacementThreshold::sMessageScope = scope;
}


int smResidualDisplacementThreshold :: GetMessageIndex () const {

	return smResidualDisplacementThreshold :: sMessageIndex;
}


int smResidualDisplacementThreshold :: GetScope () const {

	return smResidualDisplacementThreshold :: sMessageScope;
}


RGString smResidualDisplacementThreshold :: GetName () const {

	return smResidualDisplacementThreshold :: sName;
}



int smResidualDisplacementThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smResidualDisplacementThreshold* noticeType = new smResidualDisplacementThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smResidualDisplacementTestPreset :: smResidualDisplacementTestPreset () : SmartNotice () {

}


smResidualDisplacementTestPreset :: smResidualDisplacementTestPreset (const smResidualDisplacementTestPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smResidualDisplacementTestPreset :: ~smResidualDisplacementTestPreset () {

}


int smResidualDisplacementTestPreset :: GetSubject () const {

	return smResidualDisplacementTestPreset::sSubject;
}


void smResidualDisplacementTestPreset :: SetIndexAndScope (int index, int scope) const {

	smResidualDisplacementTestPreset::sMessageIndex = index;
	smResidualDisplacementTestPreset::sMessageScope = scope;
}


int smResidualDisplacementTestPreset :: GetMessageIndex () const {

	return smResidualDisplacementTestPreset :: sMessageIndex;
}


int smResidualDisplacementTestPreset :: GetScope () const {

	return smResidualDisplacementTestPreset :: sMessageScope;
}


RGString smResidualDisplacementTestPreset :: GetName () const {

	return smResidualDisplacementTestPreset :: sName;
}



int smResidualDisplacementTestPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smResidualDisplacementTestPreset* noticeType = new smResidualDisplacementTestPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smExcessiveResidualDisplacementLeft :: smExcessiveResidualDisplacementLeft () : SmartNotice () {

}


smExcessiveResidualDisplacementLeft :: smExcessiveResidualDisplacementLeft (const smExcessiveResidualDisplacementLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smExcessiveResidualDisplacementLeft :: ~smExcessiveResidualDisplacementLeft () {

}


int smExcessiveResidualDisplacementLeft :: GetSubject () const {

	return smExcessiveResidualDisplacementLeft::sSubject;
}


void smExcessiveResidualDisplacementLeft :: SetIndexAndScope (int index, int scope) const {

	smExcessiveResidualDisplacementLeft::sMessageIndex = index;
	smExcessiveResidualDisplacementLeft::sMessageScope = scope;
}


int smExcessiveResidualDisplacementLeft :: GetMessageIndex () const {

	return smExcessiveResidualDisplacementLeft :: sMessageIndex;
}


int smExcessiveResidualDisplacementLeft :: GetScope () const {

	return smExcessiveResidualDisplacementLeft :: sMessageScope;
}


RGString smExcessiveResidualDisplacementLeft :: GetName () const {

	return smExcessiveResidualDisplacementLeft :: sName;
}



int smExcessiveResidualDisplacementLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smExcessiveResidualDisplacementLeft* noticeType = new smExcessiveResidualDisplacementLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smExcessiveResidualDisplacementRight :: smExcessiveResidualDisplacementRight () : SmartNotice () {

}


smExcessiveResidualDisplacementRight :: smExcessiveResidualDisplacementRight (const smExcessiveResidualDisplacementRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smExcessiveResidualDisplacementRight :: ~smExcessiveResidualDisplacementRight () {

}


int smExcessiveResidualDisplacementRight :: GetSubject () const {

	return smExcessiveResidualDisplacementRight::sSubject;
}


void smExcessiveResidualDisplacementRight :: SetIndexAndScope (int index, int scope) const {

	smExcessiveResidualDisplacementRight::sMessageIndex = index;
	smExcessiveResidualDisplacementRight::sMessageScope = scope;
}


int smExcessiveResidualDisplacementRight :: GetMessageIndex () const {

	return smExcessiveResidualDisplacementRight :: sMessageIndex;
}


int smExcessiveResidualDisplacementRight :: GetScope () const {

	return smExcessiveResidualDisplacementRight :: sMessageScope;
}


RGString smExcessiveResidualDisplacementRight :: GetName () const {

	return smExcessiveResidualDisplacementRight :: sName;
}



int smExcessiveResidualDisplacementRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smExcessiveResidualDisplacementRight* noticeType = new smExcessiveResidualDisplacementRight;
	warehouse->AddType (noticeType);
	return 1;
}





smSampleSatisfiesPossibleMixtureIDCriteria :: smSampleSatisfiesPossibleMixtureIDCriteria () : SmartNotice () {

}


smSampleSatisfiesPossibleMixtureIDCriteria :: smSampleSatisfiesPossibleMixtureIDCriteria (const smSampleSatisfiesPossibleMixtureIDCriteria& note) : SmartNotice ((const SmartNotice&) note) {

}



smSampleSatisfiesPossibleMixtureIDCriteria :: ~smSampleSatisfiesPossibleMixtureIDCriteria () {

}


int smSampleSatisfiesPossibleMixtureIDCriteria :: GetSubject () const {

	return smSampleSatisfiesPossibleMixtureIDCriteria::sSubject;
}


void smSampleSatisfiesPossibleMixtureIDCriteria :: SetIndexAndScope (int index, int scope) const {

	smSampleSatisfiesPossibleMixtureIDCriteria::sMessageIndex = index;
	smSampleSatisfiesPossibleMixtureIDCriteria::sMessageScope = scope;
}


int smSampleSatisfiesPossibleMixtureIDCriteria :: GetMessageIndex () const {

	return smSampleSatisfiesPossibleMixtureIDCriteria :: sMessageIndex;
}


int smSampleSatisfiesPossibleMixtureIDCriteria :: GetScope () const {

	return smSampleSatisfiesPossibleMixtureIDCriteria :: sMessageScope;
}


RGString smSampleSatisfiesPossibleMixtureIDCriteria :: GetName () const {

	return smSampleSatisfiesPossibleMixtureIDCriteria :: sName;
}



int smSampleSatisfiesPossibleMixtureIDCriteria :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSampleSatisfiesPossibleMixtureIDCriteria* noticeType = new smSampleSatisfiesPossibleMixtureIDCriteria;
	warehouse->AddType (noticeType);
	return 1;
}





smDisableLowLevelFiltersForKnownMixturesPreset :: smDisableLowLevelFiltersForKnownMixturesPreset () : SmartNotice () {

}


smDisableLowLevelFiltersForKnownMixturesPreset :: smDisableLowLevelFiltersForKnownMixturesPreset (const smDisableLowLevelFiltersForKnownMixturesPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smDisableLowLevelFiltersForKnownMixturesPreset :: ~smDisableLowLevelFiltersForKnownMixturesPreset () {

}


int smDisableLowLevelFiltersForKnownMixturesPreset :: GetSubject () const {

	return smDisableLowLevelFiltersForKnownMixturesPreset::sSubject;
}


void smDisableLowLevelFiltersForKnownMixturesPreset :: SetIndexAndScope (int index, int scope) const {

	smDisableLowLevelFiltersForKnownMixturesPreset::sMessageIndex = index;
	smDisableLowLevelFiltersForKnownMixturesPreset::sMessageScope = scope;
}


int smDisableLowLevelFiltersForKnownMixturesPreset :: GetMessageIndex () const {

	return smDisableLowLevelFiltersForKnownMixturesPreset :: sMessageIndex;
}


int smDisableLowLevelFiltersForKnownMixturesPreset :: GetScope () const {

	return smDisableLowLevelFiltersForKnownMixturesPreset :: sMessageScope;
}


RGString smDisableLowLevelFiltersForKnownMixturesPreset :: GetName () const {

	return smDisableLowLevelFiltersForKnownMixturesPreset :: sName;
}



int smDisableLowLevelFiltersForKnownMixturesPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smDisableLowLevelFiltersForKnownMixturesPreset* noticeType = new smDisableLowLevelFiltersForKnownMixturesPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smDisableStutterFilter :: smDisableStutterFilter () : SmartNotice () {

}


smDisableStutterFilter :: smDisableStutterFilter (const smDisableStutterFilter& note) : SmartNotice ((const SmartNotice&) note) {

}



smDisableStutterFilter :: ~smDisableStutterFilter () {

}


int smDisableStutterFilter :: GetSubject () const {

	return smDisableStutterFilter::sSubject;
}


void smDisableStutterFilter :: SetIndexAndScope (int index, int scope) const {

	smDisableStutterFilter::sMessageIndex = index;
	smDisableStutterFilter::sMessageScope = scope;
}


int smDisableStutterFilter :: GetMessageIndex () const {

	return smDisableStutterFilter :: sMessageIndex;
}


int smDisableStutterFilter :: GetScope () const {

	return smDisableStutterFilter :: sMessageScope;
}


RGString smDisableStutterFilter :: GetName () const {

	return smDisableStutterFilter :: sName;
}



int smDisableStutterFilter :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smDisableStutterFilter* noticeType = new smDisableStutterFilter;
	warehouse->AddType (noticeType);
	return 1;
}





smDisableAdenylationFilter :: smDisableAdenylationFilter () : SmartNotice () {

}


smDisableAdenylationFilter :: smDisableAdenylationFilter (const smDisableAdenylationFilter& note) : SmartNotice ((const SmartNotice&) note) {

}



smDisableAdenylationFilter :: ~smDisableAdenylationFilter () {

}


int smDisableAdenylationFilter :: GetSubject () const {

	return smDisableAdenylationFilter::sSubject;
}


void smDisableAdenylationFilter :: SetIndexAndScope (int index, int scope) const {

	smDisableAdenylationFilter::sMessageIndex = index;
	smDisableAdenylationFilter::sMessageScope = scope;
}


int smDisableAdenylationFilter :: GetMessageIndex () const {

	return smDisableAdenylationFilter :: sMessageIndex;
}


int smDisableAdenylationFilter :: GetScope () const {

	return smDisableAdenylationFilter :: sMessageScope;
}


RGString smDisableAdenylationFilter :: GetName () const {

	return smDisableAdenylationFilter :: sName;
}



int smDisableAdenylationFilter :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smDisableAdenylationFilter* noticeType = new smDisableAdenylationFilter;
	warehouse->AddType (noticeType);
	return 1;
}





smSelectTriplePassVsSinglePassFilterPreset :: smSelectTriplePassVsSinglePassFilterPreset () : SmartNotice () {

}


smSelectTriplePassVsSinglePassFilterPreset :: smSelectTriplePassVsSinglePassFilterPreset (const smSelectTriplePassVsSinglePassFilterPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smSelectTriplePassVsSinglePassFilterPreset :: ~smSelectTriplePassVsSinglePassFilterPreset () {

}


int smSelectTriplePassVsSinglePassFilterPreset :: GetSubject () const {

	return smSelectTriplePassVsSinglePassFilterPreset::sSubject;
}


void smSelectTriplePassVsSinglePassFilterPreset :: SetIndexAndScope (int index, int scope) const {

	smSelectTriplePassVsSinglePassFilterPreset::sMessageIndex = index;
	smSelectTriplePassVsSinglePassFilterPreset::sMessageScope = scope;
}


int smSelectTriplePassVsSinglePassFilterPreset :: GetMessageIndex () const {

	return smSelectTriplePassVsSinglePassFilterPreset :: sMessageIndex;
}


int smSelectTriplePassVsSinglePassFilterPreset :: GetScope () const {

	return smSelectTriplePassVsSinglePassFilterPreset :: sMessageScope;
}


RGString smSelectTriplePassVsSinglePassFilterPreset :: GetName () const {

	return smSelectTriplePassVsSinglePassFilterPreset :: sName;
}



int smSelectTriplePassVsSinglePassFilterPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSelectTriplePassVsSinglePassFilterPreset* noticeType = new smSelectTriplePassVsSinglePassFilterPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smTriplePassFilterWindowWidth :: smTriplePassFilterWindowWidth () : SmartNotice () {

}


smTriplePassFilterWindowWidth :: smTriplePassFilterWindowWidth (const smTriplePassFilterWindowWidth& note) : SmartNotice ((const SmartNotice&) note) {

}



smTriplePassFilterWindowWidth :: ~smTriplePassFilterWindowWidth () {

}


int smTriplePassFilterWindowWidth :: GetSubject () const {

	return smTriplePassFilterWindowWidth::sSubject;
}


void smTriplePassFilterWindowWidth :: SetIndexAndScope (int index, int scope) const {

	smTriplePassFilterWindowWidth::sMessageIndex = index;
	smTriplePassFilterWindowWidth::sMessageScope = scope;
}


int smTriplePassFilterWindowWidth :: GetMessageIndex () const {

	return smTriplePassFilterWindowWidth :: sMessageIndex;
}


int smTriplePassFilterWindowWidth :: GetScope () const {

	return smTriplePassFilterWindowWidth :: sMessageScope;
}


RGString smTriplePassFilterWindowWidth :: GetName () const {

	return smTriplePassFilterWindowWidth :: sName;
}



int smTriplePassFilterWindowWidth :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smTriplePassFilterWindowWidth* noticeType = new smTriplePassFilterWindowWidth;
	warehouse->AddType (noticeType);
	return 1;
}





smLadderFitThresholdUsingMinError :: smLadderFitThresholdUsingMinError () : SmartNotice () {

}


smLadderFitThresholdUsingMinError :: smLadderFitThresholdUsingMinError (const smLadderFitThresholdUsingMinError& note) : SmartNotice ((const SmartNotice&) note) {

}



smLadderFitThresholdUsingMinError :: ~smLadderFitThresholdUsingMinError () {

}


int smLadderFitThresholdUsingMinError :: GetSubject () const {

	return smLadderFitThresholdUsingMinError::sSubject;
}


void smLadderFitThresholdUsingMinError :: SetIndexAndScope (int index, int scope) const {

	smLadderFitThresholdUsingMinError::sMessageIndex = index;
	smLadderFitThresholdUsingMinError::sMessageScope = scope;
}


int smLadderFitThresholdUsingMinError :: GetMessageIndex () const {

	return smLadderFitThresholdUsingMinError :: sMessageIndex;
}


int smLadderFitThresholdUsingMinError :: GetScope () const {

	return smLadderFitThresholdUsingMinError :: sMessageScope;
}


RGString smLadderFitThresholdUsingMinError :: GetName () const {

	return smLadderFitThresholdUsingMinError :: sName;
}



int smLadderFitThresholdUsingMinError :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLadderFitThresholdUsingMinError* noticeType = new smLadderFitThresholdUsingMinError;
	warehouse->AddType (noticeType);
	return 1;
}





smUseMaxSecondDerivativesForSampleToLadderFit :: smUseMaxSecondDerivativesForSampleToLadderFit () : SmartNotice () {

}


smUseMaxSecondDerivativesForSampleToLadderFit :: smUseMaxSecondDerivativesForSampleToLadderFit (const smUseMaxSecondDerivativesForSampleToLadderFit& note) : SmartNotice ((const SmartNotice&) note) {

}



smUseMaxSecondDerivativesForSampleToLadderFit :: ~smUseMaxSecondDerivativesForSampleToLadderFit () {

}


int smUseMaxSecondDerivativesForSampleToLadderFit :: GetSubject () const {

	return smUseMaxSecondDerivativesForSampleToLadderFit::sSubject;
}


void smUseMaxSecondDerivativesForSampleToLadderFit :: SetIndexAndScope (int index, int scope) const {

	smUseMaxSecondDerivativesForSampleToLadderFit::sMessageIndex = index;
	smUseMaxSecondDerivativesForSampleToLadderFit::sMessageScope = scope;
}


int smUseMaxSecondDerivativesForSampleToLadderFit :: GetMessageIndex () const {

	return smUseMaxSecondDerivativesForSampleToLadderFit :: sMessageIndex;
}


int smUseMaxSecondDerivativesForSampleToLadderFit :: GetScope () const {

	return smUseMaxSecondDerivativesForSampleToLadderFit :: sMessageScope;
}


RGString smUseMaxSecondDerivativesForSampleToLadderFit :: GetName () const {

	return smUseMaxSecondDerivativesForSampleToLadderFit :: sName;
}



int smUseMaxSecondDerivativesForSampleToLadderFit :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUseMaxSecondDerivativesForSampleToLadderFit* noticeType = new smUseMaxSecondDerivativesForSampleToLadderFit;
	warehouse->AddType (noticeType);
	return 1;
}





smPartOfDualSignal :: smPartOfDualSignal () : SmartNotice () {

}


smPartOfDualSignal :: smPartOfDualSignal (const smPartOfDualSignal& note) : SmartNotice ((const SmartNotice&) note) {

}



smPartOfDualSignal :: ~smPartOfDualSignal () {

}


int smPartOfDualSignal :: GetSubject () const {

	return smPartOfDualSignal::sSubject;
}


void smPartOfDualSignal :: SetIndexAndScope (int index, int scope) const {

	smPartOfDualSignal::sMessageIndex = index;
	smPartOfDualSignal::sMessageScope = scope;
}


int smPartOfDualSignal :: GetMessageIndex () const {

	return smPartOfDualSignal :: sMessageIndex;
}


int smPartOfDualSignal :: GetScope () const {

	return smPartOfDualSignal :: sMessageScope;
}


RGString smPartOfDualSignal :: GetName () const {

	return smPartOfDualSignal :: sName;
}



int smPartOfDualSignal :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPartOfDualSignal* noticeType = new smPartOfDualSignal;
	warehouse->AddType (noticeType);
	return 1;
}





smNegativePeak :: smNegativePeak () : SmartNotice () {

}


smNegativePeak :: smNegativePeak (const smNegativePeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smNegativePeak :: ~smNegativePeak () {

}


int smNegativePeak :: GetSubject () const {

	return smNegativePeak::sSubject;
}


void smNegativePeak :: SetIndexAndScope (int index, int scope) const {

	smNegativePeak::sMessageIndex = index;
	smNegativePeak::sMessageScope = scope;
}


int smNegativePeak :: GetMessageIndex () const {

	return smNegativePeak :: sMessageIndex;
}


int smNegativePeak :: GetScope () const {

	return smNegativePeak :: sMessageScope;
}


RGString smNegativePeak :: GetName () const {

	return smNegativePeak :: sName;
}



int smNegativePeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNegativePeak* noticeType = new smNegativePeak;
	warehouse->AddType (noticeType);
	return 1;
}





smCallOnLadderAdenylationPreset :: smCallOnLadderAdenylationPreset () : SmartNotice () {

}


smCallOnLadderAdenylationPreset :: smCallOnLadderAdenylationPreset (const smCallOnLadderAdenylationPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smCallOnLadderAdenylationPreset :: ~smCallOnLadderAdenylationPreset () {

}


int smCallOnLadderAdenylationPreset :: GetSubject () const {

	return smCallOnLadderAdenylationPreset::sSubject;
}


void smCallOnLadderAdenylationPreset :: SetIndexAndScope (int index, int scope) const {

	smCallOnLadderAdenylationPreset::sMessageIndex = index;
	smCallOnLadderAdenylationPreset::sMessageScope = scope;
}


int smCallOnLadderAdenylationPreset :: GetMessageIndex () const {

	return smCallOnLadderAdenylationPreset :: sMessageIndex;
}


int smCallOnLadderAdenylationPreset :: GetScope () const {

	return smCallOnLadderAdenylationPreset :: sMessageScope;
}


RGString smCallOnLadderAdenylationPreset :: GetName () const {

	return smCallOnLadderAdenylationPreset :: sName;
}



int smCallOnLadderAdenylationPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCallOnLadderAdenylationPreset* noticeType = new smCallOnLadderAdenylationPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smSignalIsIntegralMultipleOfRepeatLeft :: smSignalIsIntegralMultipleOfRepeatLeft () : SmartNotice () {

}


smSignalIsIntegralMultipleOfRepeatLeft :: smSignalIsIntegralMultipleOfRepeatLeft (const smSignalIsIntegralMultipleOfRepeatLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smSignalIsIntegralMultipleOfRepeatLeft :: ~smSignalIsIntegralMultipleOfRepeatLeft () {

}


int smSignalIsIntegralMultipleOfRepeatLeft :: GetSubject () const {

	return smSignalIsIntegralMultipleOfRepeatLeft::sSubject;
}


void smSignalIsIntegralMultipleOfRepeatLeft :: SetIndexAndScope (int index, int scope) const {

	smSignalIsIntegralMultipleOfRepeatLeft::sMessageIndex = index;
	smSignalIsIntegralMultipleOfRepeatLeft::sMessageScope = scope;
}


int smSignalIsIntegralMultipleOfRepeatLeft :: GetMessageIndex () const {

	return smSignalIsIntegralMultipleOfRepeatLeft :: sMessageIndex;
}


int smSignalIsIntegralMultipleOfRepeatLeft :: GetScope () const {

	return smSignalIsIntegralMultipleOfRepeatLeft :: sMessageScope;
}


RGString smSignalIsIntegralMultipleOfRepeatLeft :: GetName () const {

	return smSignalIsIntegralMultipleOfRepeatLeft :: sName;
}



int smSignalIsIntegralMultipleOfRepeatLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSignalIsIntegralMultipleOfRepeatLeft* noticeType = new smSignalIsIntegralMultipleOfRepeatLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smSignalIsIntegralMultipleOfRepeatRight :: smSignalIsIntegralMultipleOfRepeatRight () : SmartNotice () {

}


smSignalIsIntegralMultipleOfRepeatRight :: smSignalIsIntegralMultipleOfRepeatRight (const smSignalIsIntegralMultipleOfRepeatRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smSignalIsIntegralMultipleOfRepeatRight :: ~smSignalIsIntegralMultipleOfRepeatRight () {

}


int smSignalIsIntegralMultipleOfRepeatRight :: GetSubject () const {

	return smSignalIsIntegralMultipleOfRepeatRight::sSubject;
}


void smSignalIsIntegralMultipleOfRepeatRight :: SetIndexAndScope (int index, int scope) const {

	smSignalIsIntegralMultipleOfRepeatRight::sMessageIndex = index;
	smSignalIsIntegralMultipleOfRepeatRight::sMessageScope = scope;
}


int smSignalIsIntegralMultipleOfRepeatRight :: GetMessageIndex () const {

	return smSignalIsIntegralMultipleOfRepeatRight :: sMessageIndex;
}


int smSignalIsIntegralMultipleOfRepeatRight :: GetScope () const {

	return smSignalIsIntegralMultipleOfRepeatRight :: sMessageScope;
}


RGString smSignalIsIntegralMultipleOfRepeatRight :: GetName () const {

	return smSignalIsIntegralMultipleOfRepeatRight :: sName;
}



int smSignalIsIntegralMultipleOfRepeatRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSignalIsIntegralMultipleOfRepeatRight* noticeType = new smSignalIsIntegralMultipleOfRepeatRight;
	warehouse->AddType (noticeType);
	return 1;
}





smILSFilterLeftShoulderPeaksPreset :: smILSFilterLeftShoulderPeaksPreset () : SmartNotice () {

}


smILSFilterLeftShoulderPeaksPreset :: smILSFilterLeftShoulderPeaksPreset (const smILSFilterLeftShoulderPeaksPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSFilterLeftShoulderPeaksPreset :: ~smILSFilterLeftShoulderPeaksPreset () {

}


int smILSFilterLeftShoulderPeaksPreset :: GetSubject () const {

	return smILSFilterLeftShoulderPeaksPreset::sSubject;
}


void smILSFilterLeftShoulderPeaksPreset :: SetIndexAndScope (int index, int scope) const {

	smILSFilterLeftShoulderPeaksPreset::sMessageIndex = index;
	smILSFilterLeftShoulderPeaksPreset::sMessageScope = scope;
}


int smILSFilterLeftShoulderPeaksPreset :: GetMessageIndex () const {

	return smILSFilterLeftShoulderPeaksPreset :: sMessageIndex;
}


int smILSFilterLeftShoulderPeaksPreset :: GetScope () const {

	return smILSFilterLeftShoulderPeaksPreset :: sMessageScope;
}


RGString smILSFilterLeftShoulderPeaksPreset :: GetName () const {

	return smILSFilterLeftShoulderPeaksPreset :: sName;
}



int smILSFilterLeftShoulderPeaksPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSFilterLeftShoulderPeaksPreset* noticeType = new smILSFilterLeftShoulderPeaksPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smILSShoulderPeakProximityThreshold :: smILSShoulderPeakProximityThreshold () : SmartNotice () {

}


smILSShoulderPeakProximityThreshold :: smILSShoulderPeakProximityThreshold (const smILSShoulderPeakProximityThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSShoulderPeakProximityThreshold :: ~smILSShoulderPeakProximityThreshold () {

}


int smILSShoulderPeakProximityThreshold :: GetSubject () const {

	return smILSShoulderPeakProximityThreshold::sSubject;
}


void smILSShoulderPeakProximityThreshold :: SetIndexAndScope (int index, int scope) const {

	smILSShoulderPeakProximityThreshold::sMessageIndex = index;
	smILSShoulderPeakProximityThreshold::sMessageScope = scope;
}


int smILSShoulderPeakProximityThreshold :: GetMessageIndex () const {

	return smILSShoulderPeakProximityThreshold :: sMessageIndex;
}


int smILSShoulderPeakProximityThreshold :: GetScope () const {

	return smILSShoulderPeakProximityThreshold :: sMessageScope;
}


RGString smILSShoulderPeakProximityThreshold :: GetName () const {

	return smILSShoulderPeakProximityThreshold :: sName;
}



int smILSShoulderPeakProximityThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSShoulderPeakProximityThreshold* noticeType = new smILSShoulderPeakProximityThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smILSShoulderFilterPercentThreshold :: smILSShoulderFilterPercentThreshold () : SmartNotice () {

}


smILSShoulderFilterPercentThreshold :: smILSShoulderFilterPercentThreshold (const smILSShoulderFilterPercentThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSShoulderFilterPercentThreshold :: ~smILSShoulderFilterPercentThreshold () {

}


int smILSShoulderFilterPercentThreshold :: GetSubject () const {

	return smILSShoulderFilterPercentThreshold::sSubject;
}


void smILSShoulderFilterPercentThreshold :: SetIndexAndScope (int index, int scope) const {

	smILSShoulderFilterPercentThreshold::sMessageIndex = index;
	smILSShoulderFilterPercentThreshold::sMessageScope = scope;
}


int smILSShoulderFilterPercentThreshold :: GetMessageIndex () const {

	return smILSShoulderFilterPercentThreshold :: sMessageIndex;
}


int smILSShoulderFilterPercentThreshold :: GetScope () const {

	return smILSShoulderFilterPercentThreshold :: sMessageScope;
}


RGString smILSShoulderFilterPercentThreshold :: GetName () const {

	return smILSShoulderFilterPercentThreshold :: sName;
}



int smILSShoulderFilterPercentThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSShoulderFilterPercentThreshold* noticeType = new smILSShoulderFilterPercentThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smILSShoulderPeak :: smILSShoulderPeak () : SmartNotice () {

}


smILSShoulderPeak :: smILSShoulderPeak (const smILSShoulderPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSShoulderPeak :: ~smILSShoulderPeak () {

}


int smILSShoulderPeak :: GetSubject () const {

	return smILSShoulderPeak::sSubject;
}


void smILSShoulderPeak :: SetIndexAndScope (int index, int scope) const {

	smILSShoulderPeak::sMessageIndex = index;
	smILSShoulderPeak::sMessageScope = scope;
}


int smILSShoulderPeak :: GetMessageIndex () const {

	return smILSShoulderPeak :: sMessageIndex;
}


int smILSShoulderPeak :: GetScope () const {

	return smILSShoulderPeak :: sMessageScope;
}


RGString smILSShoulderPeak :: GetName () const {

	return smILSShoulderPeak :: sName;
}



int smILSShoulderPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSShoulderPeak* noticeType = new smILSShoulderPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smLadderPeakOutsideCoreLadder :: smLadderPeakOutsideCoreLadder () : SmartNotice () {

}


smLadderPeakOutsideCoreLadder :: smLadderPeakOutsideCoreLadder (const smLadderPeakOutsideCoreLadder& note) : SmartNotice ((const SmartNotice&) note) {

}



smLadderPeakOutsideCoreLadder :: ~smLadderPeakOutsideCoreLadder () {

}


int smLadderPeakOutsideCoreLadder :: GetSubject () const {

	return smLadderPeakOutsideCoreLadder::sSubject;
}


void smLadderPeakOutsideCoreLadder :: SetIndexAndScope (int index, int scope) const {

	smLadderPeakOutsideCoreLadder::sMessageIndex = index;
	smLadderPeakOutsideCoreLadder::sMessageScope = scope;
}


int smLadderPeakOutsideCoreLadder :: GetMessageIndex () const {

	return smLadderPeakOutsideCoreLadder :: sMessageIndex;
}


int smLadderPeakOutsideCoreLadder :: GetScope () const {

	return smLadderPeakOutsideCoreLadder :: sMessageScope;
}


RGString smLadderPeakOutsideCoreLadder :: GetName () const {

	return smLadderPeakOutsideCoreLadder :: sName;
}



int smLadderPeakOutsideCoreLadder :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLadderPeakOutsideCoreLadder* noticeType = new smLadderPeakOutsideCoreLadder;
	warehouse->AddType (noticeType);
	return 1;
}





smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset () : SmartNotice () {

}


smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset (const smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: ~smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset () {

}


int smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: GetSubject () const {

	return smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset::sSubject;
}


void smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: SetIndexAndScope (int index, int scope) const {

	smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset::sMessageIndex = index;
	smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset::sMessageScope = scope;
}


int smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: GetMessageIndex () const {

	return smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: sMessageIndex;
}


int smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: GetScope () const {

	return smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: sMessageScope;
}


RGString smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: GetName () const {

	return smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: sName;
}



int smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset* noticeType = new smMakeLadderArtifactsLeftOfCoreLadderNonCriticalPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset () : SmartNotice () {

}


smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset (const smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: ~smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset () {

}


int smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: GetSubject () const {

	return smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset::sSubject;
}


void smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: SetIndexAndScope (int index, int scope) const {

	smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset::sMessageIndex = index;
	smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset::sMessageScope = scope;
}


int smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: GetMessageIndex () const {

	return smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: sMessageIndex;
}


int smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: GetScope () const {

	return smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: sMessageScope;
}


RGString smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: GetName () const {

	return smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: sName;
}



int smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset* noticeType = new smMakeLadderArtifactsRightOfCoreLadderNonCriticalPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smExtendLociEdgeToEdgePreset :: smExtendLociEdgeToEdgePreset () : SmartNotice () {

}


smExtendLociEdgeToEdgePreset :: smExtendLociEdgeToEdgePreset (const smExtendLociEdgeToEdgePreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smExtendLociEdgeToEdgePreset :: ~smExtendLociEdgeToEdgePreset () {

}


int smExtendLociEdgeToEdgePreset :: GetSubject () const {

	return smExtendLociEdgeToEdgePreset::sSubject;
}


void smExtendLociEdgeToEdgePreset :: SetIndexAndScope (int index, int scope) const {

	smExtendLociEdgeToEdgePreset::sMessageIndex = index;
	smExtendLociEdgeToEdgePreset::sMessageScope = scope;
}


int smExtendLociEdgeToEdgePreset :: GetMessageIndex () const {

	return smExtendLociEdgeToEdgePreset :: sMessageIndex;
}


int smExtendLociEdgeToEdgePreset :: GetScope () const {

	return smExtendLociEdgeToEdgePreset :: sMessageScope;
}


RGString smExtendLociEdgeToEdgePreset :: GetName () const {

	return smExtendLociEdgeToEdgePreset :: sName;
}



int smExtendLociEdgeToEdgePreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smExtendLociEdgeToEdgePreset* noticeType = new smExtendLociEdgeToEdgePreset;
	warehouse->AddType (noticeType);
	return 1;
}





smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset () : SmartNotice () {

}


smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset (const smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: ~smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset () {

}


int smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: GetSubject () const {

	return smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset::sSubject;
}


void smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: SetIndexAndScope (int index, int scope) const {

	smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset::sMessageIndex = index;
	smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset::sMessageScope = scope;
}


int smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: GetMessageIndex () const {

	return smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: sMessageIndex;
}


int smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: GetScope () const {

	return smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: sMessageScope;
}


RGString smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: GetName () const {

	return smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: sName;
}



int smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset* noticeType = new smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset;
	warehouse->AddType (noticeType);
	return 1;
}





smMaxILSBPForExtendedLocus :: smMaxILSBPForExtendedLocus () : SmartNotice () {

}


smMaxILSBPForExtendedLocus :: smMaxILSBPForExtendedLocus (const smMaxILSBPForExtendedLocus& note) : SmartNotice ((const SmartNotice&) note) {

}



smMaxILSBPForExtendedLocus :: ~smMaxILSBPForExtendedLocus () {

}


int smMaxILSBPForExtendedLocus :: GetSubject () const {

	return smMaxILSBPForExtendedLocus::sSubject;
}


void smMaxILSBPForExtendedLocus :: SetIndexAndScope (int index, int scope) const {

	smMaxILSBPForExtendedLocus::sMessageIndex = index;
	smMaxILSBPForExtendedLocus::sMessageScope = scope;
}


int smMaxILSBPForExtendedLocus :: GetMessageIndex () const {

	return smMaxILSBPForExtendedLocus :: sMessageIndex;
}


int smMaxILSBPForExtendedLocus :: GetScope () const {

	return smMaxILSBPForExtendedLocus :: sMessageScope;
}


RGString smMaxILSBPForExtendedLocus :: GetName () const {

	return smMaxILSBPForExtendedLocus :: sName;
}



int smMaxILSBPForExtendedLocus :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMaxILSBPForExtendedLocus* noticeType = new smMaxILSBPForExtendedLocus;
	warehouse->AddType (noticeType);
	return 1;
}





smIsQualityLocus :: smIsQualityLocus () : SmartNotice () {

}


smIsQualityLocus :: smIsQualityLocus (const smIsQualityLocus& note) : SmartNotice ((const SmartNotice&) note) {

}



smIsQualityLocus :: ~smIsQualityLocus () {

}


int smIsQualityLocus :: GetSubject () const {

	return smIsQualityLocus::sSubject;
}


void smIsQualityLocus :: SetIndexAndScope (int index, int scope) const {

	smIsQualityLocus::sMessageIndex = index;
	smIsQualityLocus::sMessageScope = scope;
}


int smIsQualityLocus :: GetMessageIndex () const {

	return smIsQualityLocus :: sMessageIndex;
}


int smIsQualityLocus :: GetScope () const {

	return smIsQualityLocus :: sMessageScope;
}


RGString smIsQualityLocus :: GetName () const {

	return smIsQualityLocus :: sName;
}



int smIsQualityLocus :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smIsQualityLocus* noticeType = new smIsQualityLocus;
	warehouse->AddType (noticeType);
	return 1;
}





smScaleILSPrimerSearchPreset :: smScaleILSPrimerSearchPreset () : SmartNotice () {

}


smScaleILSPrimerSearchPreset :: smScaleILSPrimerSearchPreset (const smScaleILSPrimerSearchPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smScaleILSPrimerSearchPreset :: ~smScaleILSPrimerSearchPreset () {

}


int smScaleILSPrimerSearchPreset :: GetSubject () const {

	return smScaleILSPrimerSearchPreset::sSubject;
}


void smScaleILSPrimerSearchPreset :: SetIndexAndScope (int index, int scope) const {

	smScaleILSPrimerSearchPreset::sMessageIndex = index;
	smScaleILSPrimerSearchPreset::sMessageScope = scope;
}


int smScaleILSPrimerSearchPreset :: GetMessageIndex () const {

	return smScaleILSPrimerSearchPreset :: sMessageIndex;
}


int smScaleILSPrimerSearchPreset :: GetScope () const {

	return smScaleILSPrimerSearchPreset :: sMessageScope;
}


RGString smScaleILSPrimerSearchPreset :: GetName () const {

	return smScaleILSPrimerSearchPreset :: sName;
}



int smScaleILSPrimerSearchPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smScaleILSPrimerSearchPreset* noticeType = new smScaleILSPrimerSearchPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smNumberOfLastILSPeaksInScale :: smNumberOfLastILSPeaksInScale () : SmartNotice () {

}


smNumberOfLastILSPeaksInScale :: smNumberOfLastILSPeaksInScale (const smNumberOfLastILSPeaksInScale& note) : SmartNotice ((const SmartNotice&) note) {

}



smNumberOfLastILSPeaksInScale :: ~smNumberOfLastILSPeaksInScale () {

}


int smNumberOfLastILSPeaksInScale :: GetSubject () const {

	return smNumberOfLastILSPeaksInScale::sSubject;
}


void smNumberOfLastILSPeaksInScale :: SetIndexAndScope (int index, int scope) const {

	smNumberOfLastILSPeaksInScale::sMessageIndex = index;
	smNumberOfLastILSPeaksInScale::sMessageScope = scope;
}


int smNumberOfLastILSPeaksInScale :: GetMessageIndex () const {

	return smNumberOfLastILSPeaksInScale :: sMessageIndex;
}


int smNumberOfLastILSPeaksInScale :: GetScope () const {

	return smNumberOfLastILSPeaksInScale :: sMessageScope;
}


RGString smNumberOfLastILSPeaksInScale :: GetName () const {

	return smNumberOfLastILSPeaksInScale :: sName;
}



int smNumberOfLastILSPeaksInScale :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNumberOfLastILSPeaksInScale* noticeType = new smNumberOfLastILSPeaksInScale;
	warehouse->AddType (noticeType);
	return 1;
}





smPercentLastILSPeaksInScaling :: smPercentLastILSPeaksInScaling () : SmartNotice () {

}


smPercentLastILSPeaksInScaling :: smPercentLastILSPeaksInScaling (const smPercentLastILSPeaksInScaling& note) : SmartNotice ((const SmartNotice&) note) {

}



smPercentLastILSPeaksInScaling :: ~smPercentLastILSPeaksInScaling () {

}


int smPercentLastILSPeaksInScaling :: GetSubject () const {

	return smPercentLastILSPeaksInScaling::sSubject;
}


void smPercentLastILSPeaksInScaling :: SetIndexAndScope (int index, int scope) const {

	smPercentLastILSPeaksInScaling::sMessageIndex = index;
	smPercentLastILSPeaksInScaling::sMessageScope = scope;
}


int smPercentLastILSPeaksInScaling :: GetMessageIndex () const {

	return smPercentLastILSPeaksInScaling :: sMessageIndex;
}


int smPercentLastILSPeaksInScaling :: GetScope () const {

	return smPercentLastILSPeaksInScaling :: sMessageScope;
}


RGString smPercentLastILSPeaksInScaling :: GetName () const {

	return smPercentLastILSPeaksInScaling :: sName;
}



int smPercentLastILSPeaksInScaling :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPercentLastILSPeaksInScaling* noticeType = new smPercentLastILSPeaksInScaling;
	warehouse->AddType (noticeType);
	return 1;
}





smLaserOffScale :: smLaserOffScale () : SmartNotice () {

}


smLaserOffScale :: smLaserOffScale (const smLaserOffScale& note) : SmartNotice ((const SmartNotice&) note) {

}



smLaserOffScale :: ~smLaserOffScale () {

}


int smLaserOffScale :: GetSubject () const {

	return smLaserOffScale::sSubject;
}


void smLaserOffScale :: SetIndexAndScope (int index, int scope) const {

	smLaserOffScale::sMessageIndex = index;
	smLaserOffScale::sMessageScope = scope;
}


int smLaserOffScale :: GetMessageIndex () const {

	return smLaserOffScale :: sMessageIndex;
}


int smLaserOffScale :: GetScope () const {

	return smLaserOffScale :: sMessageScope;
}


RGString smLaserOffScale :: GetName () const {

	return smLaserOffScale :: sName;
}



int smLaserOffScale :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLaserOffScale* noticeType = new smLaserOffScale;
	warehouse->AddType (noticeType);
	return 1;
}





smCalculatedPurePullup :: smCalculatedPurePullup () : SmartNotice () {

}


smCalculatedPurePullup :: smCalculatedPurePullup (const smCalculatedPurePullup& note) : SmartNotice ((const SmartNotice&) note) {

}



smCalculatedPurePullup :: ~smCalculatedPurePullup () {

}


int smCalculatedPurePullup :: GetSubject () const {

	return smCalculatedPurePullup::sSubject;
}


void smCalculatedPurePullup :: SetIndexAndScope (int index, int scope) const {

	smCalculatedPurePullup::sMessageIndex = index;
	smCalculatedPurePullup::sMessageScope = scope;
}


int smCalculatedPurePullup :: GetMessageIndex () const {

	return smCalculatedPurePullup :: sMessageIndex;
}


int smCalculatedPurePullup :: GetScope () const {

	return smCalculatedPurePullup :: sMessageScope;
}


RGString smCalculatedPurePullup :: GetName () const {

	return smCalculatedPurePullup :: sName;
}



int smCalculatedPurePullup :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCalculatedPurePullup* noticeType = new smCalculatedPurePullup;
	warehouse->AddType (noticeType);
	return 1;
}





smPartialPullupBelowMinRFU :: smPartialPullupBelowMinRFU () : SmartNotice () {

}


smPartialPullupBelowMinRFU :: smPartialPullupBelowMinRFU (const smPartialPullupBelowMinRFU& note) : SmartNotice ((const SmartNotice&) note) {

}



smPartialPullupBelowMinRFU :: ~smPartialPullupBelowMinRFU () {

}


int smPartialPullupBelowMinRFU :: GetSubject () const {

	return smPartialPullupBelowMinRFU::sSubject;
}


void smPartialPullupBelowMinRFU :: SetIndexAndScope (int index, int scope) const {

	smPartialPullupBelowMinRFU::sMessageIndex = index;
	smPartialPullupBelowMinRFU::sMessageScope = scope;
}


int smPartialPullupBelowMinRFU :: GetMessageIndex () const {

	return smPartialPullupBelowMinRFU :: sMessageIndex;
}


int smPartialPullupBelowMinRFU :: GetScope () const {

	return smPartialPullupBelowMinRFU :: sMessageScope;
}


RGString smPartialPullupBelowMinRFU :: GetName () const {

	return smPartialPullupBelowMinRFU :: sName;
}



int smPartialPullupBelowMinRFU :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPartialPullupBelowMinRFU* noticeType = new smPartialPullupBelowMinRFU;
	warehouse->AddType (noticeType);
	return 1;
}





smSigmoidalSidePeak :: smSigmoidalSidePeak () : SmartNotice () {

}


smSigmoidalSidePeak :: smSigmoidalSidePeak (const smSigmoidalSidePeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smSigmoidalSidePeak :: ~smSigmoidalSidePeak () {

}


int smSigmoidalSidePeak :: GetSubject () const {

	return smSigmoidalSidePeak::sSubject;
}


void smSigmoidalSidePeak :: SetIndexAndScope (int index, int scope) const {

	smSigmoidalSidePeak::sMessageIndex = index;
	smSigmoidalSidePeak::sMessageScope = scope;
}


int smSigmoidalSidePeak :: GetMessageIndex () const {

	return smSigmoidalSidePeak :: sMessageIndex;
}


int smSigmoidalSidePeak :: GetScope () const {

	return smSigmoidalSidePeak :: sMessageScope;
}


RGString smSigmoidalSidePeak :: GetName () const {

	return smSigmoidalSidePeak :: sName;
}



int smSigmoidalSidePeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSigmoidalSidePeak* noticeType = new smSigmoidalSidePeak;
	warehouse->AddType (noticeType);
	return 1;
}





smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: smDoNotCallStutterPeaksForSingleSourceSamplesPreset () : SmartNotice () {

}


smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: smDoNotCallStutterPeaksForSingleSourceSamplesPreset (const smDoNotCallStutterPeaksForSingleSourceSamplesPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: ~smDoNotCallStutterPeaksForSingleSourceSamplesPreset () {

}


int smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: GetSubject () const {

	return smDoNotCallStutterPeaksForSingleSourceSamplesPreset::sSubject;
}


void smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: SetIndexAndScope (int index, int scope) const {

	smDoNotCallStutterPeaksForSingleSourceSamplesPreset::sMessageIndex = index;
	smDoNotCallStutterPeaksForSingleSourceSamplesPreset::sMessageScope = scope;
}


int smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: GetMessageIndex () const {

	return smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: sMessageIndex;
}


int smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: GetScope () const {

	return smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: sMessageScope;
}


RGString smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: GetName () const {

	return smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: sName;
}



int smDoNotCallStutterPeaksForSingleSourceSamplesPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smDoNotCallStutterPeaksForSingleSourceSamplesPreset* noticeType = new smDoNotCallStutterPeaksForSingleSourceSamplesPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset () : SmartNotice () {

}


smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset (const smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: ~smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset () {

}


int smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: GetSubject () const {

	return smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset::sSubject;
}


void smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: SetIndexAndScope (int index, int scope) const {

	smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset::sMessageIndex = index;
	smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset::sMessageScope = scope;
}


int smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: GetMessageIndex () const {

	return smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: sMessageIndex;
}


int smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: GetScope () const {

	return smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: sMessageScope;
}


RGString smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: GetName () const {

	return smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: sName;
}



int smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset* noticeType = new smCallAdenylationPeaksWithArtifactForAcceptedOnladderPeaksPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smCallStutterPeaksPreset :: smCallStutterPeaksPreset () : SmartNotice () {

}


smCallStutterPeaksPreset :: smCallStutterPeaksPreset (const smCallStutterPeaksPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smCallStutterPeaksPreset :: ~smCallStutterPeaksPreset () {

}


int smCallStutterPeaksPreset :: GetSubject () const {

	return smCallStutterPeaksPreset::sSubject;
}


void smCallStutterPeaksPreset :: SetIndexAndScope (int index, int scope) const {

	smCallStutterPeaksPreset::sMessageIndex = index;
	smCallStutterPeaksPreset::sMessageScope = scope;
}


int smCallStutterPeaksPreset :: GetMessageIndex () const {

	return smCallStutterPeaksPreset :: sMessageIndex;
}


int smCallStutterPeaksPreset :: GetScope () const {

	return smCallStutterPeaksPreset :: sMessageScope;
}


RGString smCallStutterPeaksPreset :: GetName () const {

	return smCallStutterPeaksPreset :: sName;
}



int smCallStutterPeaksPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCallStutterPeaksPreset* noticeType = new smCallStutterPeaksPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smIsAcceptedOLAllele :: smIsAcceptedOLAllele () : SmartNotice () {

}


smIsAcceptedOLAllele :: smIsAcceptedOLAllele (const smIsAcceptedOLAllele& note) : SmartNotice ((const SmartNotice&) note) {

}



smIsAcceptedOLAllele :: ~smIsAcceptedOLAllele () {

}


int smIsAcceptedOLAllele :: GetSubject () const {

	return smIsAcceptedOLAllele::sSubject;
}


void smIsAcceptedOLAllele :: SetIndexAndScope (int index, int scope) const {

	smIsAcceptedOLAllele::sMessageIndex = index;
	smIsAcceptedOLAllele::sMessageScope = scope;
}


int smIsAcceptedOLAllele :: GetMessageIndex () const {

	return smIsAcceptedOLAllele :: sMessageIndex;
}


int smIsAcceptedOLAllele :: GetScope () const {

	return smIsAcceptedOLAllele :: sMessageScope;
}


RGString smIsAcceptedOLAllele :: GetName () const {

	return smIsAcceptedOLAllele :: sName;
}



int smIsAcceptedOLAllele :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smIsAcceptedOLAllele* noticeType = new smIsAcceptedOLAllele;
	warehouse->AddType (noticeType);
	return 1;
}





smSaveLadderILSHistoryPreset :: smSaveLadderILSHistoryPreset () : SmartNotice () {

}


smSaveLadderILSHistoryPreset :: smSaveLadderILSHistoryPreset (const smSaveLadderILSHistoryPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smSaveLadderILSHistoryPreset :: ~smSaveLadderILSHistoryPreset () {

}


int smSaveLadderILSHistoryPreset :: GetSubject () const {

	return smSaveLadderILSHistoryPreset::sSubject;
}


void smSaveLadderILSHistoryPreset :: SetIndexAndScope (int index, int scope) const {

	smSaveLadderILSHistoryPreset::sMessageIndex = index;
	smSaveLadderILSHistoryPreset::sMessageScope = scope;
}


int smSaveLadderILSHistoryPreset :: GetMessageIndex () const {

	return smSaveLadderILSHistoryPreset :: sMessageIndex;
}


int smSaveLadderILSHistoryPreset :: GetScope () const {

	return smSaveLadderILSHistoryPreset :: sMessageScope;
}


RGString smSaveLadderILSHistoryPreset :: GetName () const {

	return smSaveLadderILSHistoryPreset :: sName;
}



int smSaveLadderILSHistoryPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSaveLadderILSHistoryPreset* noticeType = new smSaveLadderILSHistoryPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smLatitudeForILSFit :: smLatitudeForILSFit () : SmartNotice () {

}


smLatitudeForILSFit :: smLatitudeForILSFit (const smLatitudeForILSFit& note) : SmartNotice ((const SmartNotice&) note) {

}



smLatitudeForILSFit :: ~smLatitudeForILSFit () {

}


int smLatitudeForILSFit :: GetSubject () const {

	return smLatitudeForILSFit::sSubject;
}


void smLatitudeForILSFit :: SetIndexAndScope (int index, int scope) const {

	smLatitudeForILSFit::sMessageIndex = index;
	smLatitudeForILSFit::sMessageScope = scope;
}


int smLatitudeForILSFit :: GetMessageIndex () const {

	return smLatitudeForILSFit :: sMessageIndex;
}


int smLatitudeForILSFit :: GetScope () const {

	return smLatitudeForILSFit :: sMessageScope;
}


RGString smLatitudeForILSFit :: GetName () const {

	return smLatitudeForILSFit :: sName;
}



int smLatitudeForILSFit :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smLatitudeForILSFit* noticeType = new smLatitudeForILSFit;
	warehouse->AddType (noticeType);
	return 1;
}





smUseLadderEndPointILSAlgorithmPreset :: smUseLadderEndPointILSAlgorithmPreset () : SmartNotice () {

}


smUseLadderEndPointILSAlgorithmPreset :: smUseLadderEndPointILSAlgorithmPreset (const smUseLadderEndPointILSAlgorithmPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smUseLadderEndPointILSAlgorithmPreset :: ~smUseLadderEndPointILSAlgorithmPreset () {

}


int smUseLadderEndPointILSAlgorithmPreset :: GetSubject () const {

	return smUseLadderEndPointILSAlgorithmPreset::sSubject;
}


void smUseLadderEndPointILSAlgorithmPreset :: SetIndexAndScope (int index, int scope) const {

	smUseLadderEndPointILSAlgorithmPreset::sMessageIndex = index;
	smUseLadderEndPointILSAlgorithmPreset::sMessageScope = scope;
}


int smUseLadderEndPointILSAlgorithmPreset :: GetMessageIndex () const {

	return smUseLadderEndPointILSAlgorithmPreset :: sMessageIndex;
}


int smUseLadderEndPointILSAlgorithmPreset :: GetScope () const {

	return smUseLadderEndPointILSAlgorithmPreset :: sMessageScope;
}


RGString smUseLadderEndPointILSAlgorithmPreset :: GetName () const {

	return smUseLadderEndPointILSAlgorithmPreset :: sName;
}



int smUseLadderEndPointILSAlgorithmPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUseLadderEndPointILSAlgorithmPreset* noticeType = new smUseLadderEndPointILSAlgorithmPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smPlusLatitudeForLadderEndPointILSFit :: smPlusLatitudeForLadderEndPointILSFit () : SmartNotice () {

}


smPlusLatitudeForLadderEndPointILSFit :: smPlusLatitudeForLadderEndPointILSFit (const smPlusLatitudeForLadderEndPointILSFit& note) : SmartNotice ((const SmartNotice&) note) {

}



smPlusLatitudeForLadderEndPointILSFit :: ~smPlusLatitudeForLadderEndPointILSFit () {

}


int smPlusLatitudeForLadderEndPointILSFit :: GetSubject () const {

	return smPlusLatitudeForLadderEndPointILSFit::sSubject;
}


void smPlusLatitudeForLadderEndPointILSFit :: SetIndexAndScope (int index, int scope) const {

	smPlusLatitudeForLadderEndPointILSFit::sMessageIndex = index;
	smPlusLatitudeForLadderEndPointILSFit::sMessageScope = scope;
}


int smPlusLatitudeForLadderEndPointILSFit :: GetMessageIndex () const {

	return smPlusLatitudeForLadderEndPointILSFit :: sMessageIndex;
}


int smPlusLatitudeForLadderEndPointILSFit :: GetScope () const {

	return smPlusLatitudeForLadderEndPointILSFit :: sMessageScope;
}


RGString smPlusLatitudeForLadderEndPointILSFit :: GetName () const {

	return smPlusLatitudeForLadderEndPointILSFit :: sName;
}



int smPlusLatitudeForLadderEndPointILSFit :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPlusLatitudeForLadderEndPointILSFit* noticeType = new smPlusLatitudeForLadderEndPointILSFit;
	warehouse->AddType (noticeType);
	return 1;
}





smApplyEnhancedShoulderFittingAlgorithmPreset :: smApplyEnhancedShoulderFittingAlgorithmPreset () : SmartNotice () {

}


smApplyEnhancedShoulderFittingAlgorithmPreset :: smApplyEnhancedShoulderFittingAlgorithmPreset (const smApplyEnhancedShoulderFittingAlgorithmPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smApplyEnhancedShoulderFittingAlgorithmPreset :: ~smApplyEnhancedShoulderFittingAlgorithmPreset () {

}


int smApplyEnhancedShoulderFittingAlgorithmPreset :: GetSubject () const {

	return smApplyEnhancedShoulderFittingAlgorithmPreset::sSubject;
}


void smApplyEnhancedShoulderFittingAlgorithmPreset :: SetIndexAndScope (int index, int scope) const {

	smApplyEnhancedShoulderFittingAlgorithmPreset::sMessageIndex = index;
	smApplyEnhancedShoulderFittingAlgorithmPreset::sMessageScope = scope;
}


int smApplyEnhancedShoulderFittingAlgorithmPreset :: GetMessageIndex () const {

	return smApplyEnhancedShoulderFittingAlgorithmPreset :: sMessageIndex;
}


int smApplyEnhancedShoulderFittingAlgorithmPreset :: GetScope () const {

	return smApplyEnhancedShoulderFittingAlgorithmPreset :: sMessageScope;
}


RGString smApplyEnhancedShoulderFittingAlgorithmPreset :: GetName () const {

	return smApplyEnhancedShoulderFittingAlgorithmPreset :: sName;
}



int smApplyEnhancedShoulderFittingAlgorithmPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smApplyEnhancedShoulderFittingAlgorithmPreset* noticeType = new smApplyEnhancedShoulderFittingAlgorithmPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smNoiseFactorForShoulderAcceptanceThreshold :: smNoiseFactorForShoulderAcceptanceThreshold () : SmartNotice () {

}


smNoiseFactorForShoulderAcceptanceThreshold :: smNoiseFactorForShoulderAcceptanceThreshold (const smNoiseFactorForShoulderAcceptanceThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smNoiseFactorForShoulderAcceptanceThreshold :: ~smNoiseFactorForShoulderAcceptanceThreshold () {

}


int smNoiseFactorForShoulderAcceptanceThreshold :: GetSubject () const {

	return smNoiseFactorForShoulderAcceptanceThreshold::sSubject;
}


void smNoiseFactorForShoulderAcceptanceThreshold :: SetIndexAndScope (int index, int scope) const {

	smNoiseFactorForShoulderAcceptanceThreshold::sMessageIndex = index;
	smNoiseFactorForShoulderAcceptanceThreshold::sMessageScope = scope;
}


int smNoiseFactorForShoulderAcceptanceThreshold :: GetMessageIndex () const {

	return smNoiseFactorForShoulderAcceptanceThreshold :: sMessageIndex;
}


int smNoiseFactorForShoulderAcceptanceThreshold :: GetScope () const {

	return smNoiseFactorForShoulderAcceptanceThreshold :: sMessageScope;
}


RGString smNoiseFactorForShoulderAcceptanceThreshold :: GetName () const {

	return smNoiseFactorForShoulderAcceptanceThreshold :: sName;
}



int smNoiseFactorForShoulderAcceptanceThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNoiseFactorForShoulderAcceptanceThreshold* noticeType = new smNoiseFactorForShoulderAcceptanceThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smConcaveDownAcceptanceThreshold :: smConcaveDownAcceptanceThreshold () : SmartNotice () {

}


smConcaveDownAcceptanceThreshold :: smConcaveDownAcceptanceThreshold (const smConcaveDownAcceptanceThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smConcaveDownAcceptanceThreshold :: ~smConcaveDownAcceptanceThreshold () {

}


int smConcaveDownAcceptanceThreshold :: GetSubject () const {

	return smConcaveDownAcceptanceThreshold::sSubject;
}


void smConcaveDownAcceptanceThreshold :: SetIndexAndScope (int index, int scope) const {

	smConcaveDownAcceptanceThreshold::sMessageIndex = index;
	smConcaveDownAcceptanceThreshold::sMessageScope = scope;
}


int smConcaveDownAcceptanceThreshold :: GetMessageIndex () const {

	return smConcaveDownAcceptanceThreshold :: sMessageIndex;
}


int smConcaveDownAcceptanceThreshold :: GetScope () const {

	return smConcaveDownAcceptanceThreshold :: sMessageScope;
}


RGString smConcaveDownAcceptanceThreshold :: GetName () const {

	return smConcaveDownAcceptanceThreshold :: sName;
}



int smConcaveDownAcceptanceThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smConcaveDownAcceptanceThreshold* noticeType = new smConcaveDownAcceptanceThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smCorePeakSharesAlleleBin :: smCorePeakSharesAlleleBin () : SmartNotice () {

}


smCorePeakSharesAlleleBin :: smCorePeakSharesAlleleBin (const smCorePeakSharesAlleleBin& note) : SmartNotice ((const SmartNotice&) note) {

}



smCorePeakSharesAlleleBin :: ~smCorePeakSharesAlleleBin () {

}


int smCorePeakSharesAlleleBin :: GetSubject () const {

	return smCorePeakSharesAlleleBin::sSubject;
}


void smCorePeakSharesAlleleBin :: SetIndexAndScope (int index, int scope) const {

	smCorePeakSharesAlleleBin::sMessageIndex = index;
	smCorePeakSharesAlleleBin::sMessageScope = scope;
}


int smCorePeakSharesAlleleBin :: GetMessageIndex () const {

	return smCorePeakSharesAlleleBin :: sMessageIndex;
}


int smCorePeakSharesAlleleBin :: GetScope () const {

	return smCorePeakSharesAlleleBin :: sMessageScope;
}


RGString smCorePeakSharesAlleleBin :: GetName () const {

	return smCorePeakSharesAlleleBin :: sName;
}



int smCorePeakSharesAlleleBin :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smCorePeakSharesAlleleBin* noticeType = new smCorePeakSharesAlleleBin;
	warehouse->AddType (noticeType);
	return 1;
}





smMinImbalanceThresholdForCreatingNoisyPeak :: smMinImbalanceThresholdForCreatingNoisyPeak () : SmartNotice () {

}


smMinImbalanceThresholdForCreatingNoisyPeak :: smMinImbalanceThresholdForCreatingNoisyPeak (const smMinImbalanceThresholdForCreatingNoisyPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smMinImbalanceThresholdForCreatingNoisyPeak :: ~smMinImbalanceThresholdForCreatingNoisyPeak () {

}


int smMinImbalanceThresholdForCreatingNoisyPeak :: GetSubject () const {

	return smMinImbalanceThresholdForCreatingNoisyPeak::sSubject;
}


void smMinImbalanceThresholdForCreatingNoisyPeak :: SetIndexAndScope (int index, int scope) const {

	smMinImbalanceThresholdForCreatingNoisyPeak::sMessageIndex = index;
	smMinImbalanceThresholdForCreatingNoisyPeak::sMessageScope = scope;
}


int smMinImbalanceThresholdForCreatingNoisyPeak :: GetMessageIndex () const {

	return smMinImbalanceThresholdForCreatingNoisyPeak :: sMessageIndex;
}


int smMinImbalanceThresholdForCreatingNoisyPeak :: GetScope () const {

	return smMinImbalanceThresholdForCreatingNoisyPeak :: sMessageScope;
}


RGString smMinImbalanceThresholdForCreatingNoisyPeak :: GetName () const {

	return smMinImbalanceThresholdForCreatingNoisyPeak :: sName;
}



int smMinImbalanceThresholdForCreatingNoisyPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMinImbalanceThresholdForCreatingNoisyPeak* noticeType = new smMinImbalanceThresholdForCreatingNoisyPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smPeakSharesAlleleBinLeft :: smPeakSharesAlleleBinLeft () : SmartNotice () {

}


smPeakSharesAlleleBinLeft :: smPeakSharesAlleleBinLeft (const smPeakSharesAlleleBinLeft& note) : SmartNotice ((const SmartNotice&) note) {

}



smPeakSharesAlleleBinLeft :: ~smPeakSharesAlleleBinLeft () {

}


int smPeakSharesAlleleBinLeft :: GetSubject () const {

	return smPeakSharesAlleleBinLeft::sSubject;
}


void smPeakSharesAlleleBinLeft :: SetIndexAndScope (int index, int scope) const {

	smPeakSharesAlleleBinLeft::sMessageIndex = index;
	smPeakSharesAlleleBinLeft::sMessageScope = scope;
}


int smPeakSharesAlleleBinLeft :: GetMessageIndex () const {

	return smPeakSharesAlleleBinLeft :: sMessageIndex;
}


int smPeakSharesAlleleBinLeft :: GetScope () const {

	return smPeakSharesAlleleBinLeft :: sMessageScope;
}


RGString smPeakSharesAlleleBinLeft :: GetName () const {

	return smPeakSharesAlleleBinLeft :: sName;
}



int smPeakSharesAlleleBinLeft :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPeakSharesAlleleBinLeft* noticeType = new smPeakSharesAlleleBinLeft;
	warehouse->AddType (noticeType);
	return 1;
}





smPeakSharesAlleleBinRight :: smPeakSharesAlleleBinRight () : SmartNotice () {

}


smPeakSharesAlleleBinRight :: smPeakSharesAlleleBinRight (const smPeakSharesAlleleBinRight& note) : SmartNotice ((const SmartNotice&) note) {

}



smPeakSharesAlleleBinRight :: ~smPeakSharesAlleleBinRight () {

}


int smPeakSharesAlleleBinRight :: GetSubject () const {

	return smPeakSharesAlleleBinRight::sSubject;
}


void smPeakSharesAlleleBinRight :: SetIndexAndScope (int index, int scope) const {

	smPeakSharesAlleleBinRight::sMessageIndex = index;
	smPeakSharesAlleleBinRight::sMessageScope = scope;
}


int smPeakSharesAlleleBinRight :: GetMessageIndex () const {

	return smPeakSharesAlleleBinRight :: sMessageIndex;
}


int smPeakSharesAlleleBinRight :: GetScope () const {

	return smPeakSharesAlleleBinRight :: sMessageScope;
}


RGString smPeakSharesAlleleBinRight :: GetName () const {

	return smPeakSharesAlleleBinRight :: sName;
}



int smPeakSharesAlleleBinRight :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPeakSharesAlleleBinRight* noticeType = new smPeakSharesAlleleBinRight;
	warehouse->AddType (noticeType);
	return 1;
}





smConstrainPullupPatternAnalysisPreset :: smConstrainPullupPatternAnalysisPreset () : SmartNotice () {

}


smConstrainPullupPatternAnalysisPreset :: smConstrainPullupPatternAnalysisPreset (const smConstrainPullupPatternAnalysisPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smConstrainPullupPatternAnalysisPreset :: ~smConstrainPullupPatternAnalysisPreset () {

}


int smConstrainPullupPatternAnalysisPreset :: GetSubject () const {

	return smConstrainPullupPatternAnalysisPreset::sSubject;
}


void smConstrainPullupPatternAnalysisPreset :: SetIndexAndScope (int index, int scope) const {

	smConstrainPullupPatternAnalysisPreset::sMessageIndex = index;
	smConstrainPullupPatternAnalysisPreset::sMessageScope = scope;
}


int smConstrainPullupPatternAnalysisPreset :: GetMessageIndex () const {

	return smConstrainPullupPatternAnalysisPreset :: sMessageIndex;
}


int smConstrainPullupPatternAnalysisPreset :: GetScope () const {

	return smConstrainPullupPatternAnalysisPreset :: sMessageScope;
}


RGString smConstrainPullupPatternAnalysisPreset :: GetName () const {

	return smConstrainPullupPatternAnalysisPreset :: sName;
}



int smConstrainPullupPatternAnalysisPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smConstrainPullupPatternAnalysisPreset* noticeType = new smConstrainPullupPatternAnalysisPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smOverrideNoiseLevelPercentsInSmoothing :: smOverrideNoiseLevelPercentsInSmoothing () : SmartNotice () {

}


smOverrideNoiseLevelPercentsInSmoothing :: smOverrideNoiseLevelPercentsInSmoothing (const smOverrideNoiseLevelPercentsInSmoothing& note) : SmartNotice ((const SmartNotice&) note) {

}



smOverrideNoiseLevelPercentsInSmoothing :: ~smOverrideNoiseLevelPercentsInSmoothing () {

}


int smOverrideNoiseLevelPercentsInSmoothing :: GetSubject () const {

	return smOverrideNoiseLevelPercentsInSmoothing::sSubject;
}


void smOverrideNoiseLevelPercentsInSmoothing :: SetIndexAndScope (int index, int scope) const {

	smOverrideNoiseLevelPercentsInSmoothing::sMessageIndex = index;
	smOverrideNoiseLevelPercentsInSmoothing::sMessageScope = scope;
}


int smOverrideNoiseLevelPercentsInSmoothing :: GetMessageIndex () const {

	return smOverrideNoiseLevelPercentsInSmoothing :: sMessageIndex;
}


int smOverrideNoiseLevelPercentsInSmoothing :: GetScope () const {

	return smOverrideNoiseLevelPercentsInSmoothing :: sMessageScope;
}


RGString smOverrideNoiseLevelPercentsInSmoothing :: GetName () const {

	return smOverrideNoiseLevelPercentsInSmoothing :: sName;
}



int smOverrideNoiseLevelPercentsInSmoothing :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smOverrideNoiseLevelPercentsInSmoothing* noticeType = new smOverrideNoiseLevelPercentsInSmoothing;
	warehouse->AddType (noticeType);
	return 1;
}





smPercentOverrideOfNoiseThresholdForNormalizationPhase :: smPercentOverrideOfNoiseThresholdForNormalizationPhase () : SmartNotice () {

}


smPercentOverrideOfNoiseThresholdForNormalizationPhase :: smPercentOverrideOfNoiseThresholdForNormalizationPhase (const smPercentOverrideOfNoiseThresholdForNormalizationPhase& note) : SmartNotice ((const SmartNotice&) note) {

}



smPercentOverrideOfNoiseThresholdForNormalizationPhase :: ~smPercentOverrideOfNoiseThresholdForNormalizationPhase () {

}


int smPercentOverrideOfNoiseThresholdForNormalizationPhase :: GetSubject () const {

	return smPercentOverrideOfNoiseThresholdForNormalizationPhase::sSubject;
}


void smPercentOverrideOfNoiseThresholdForNormalizationPhase :: SetIndexAndScope (int index, int scope) const {

	smPercentOverrideOfNoiseThresholdForNormalizationPhase::sMessageIndex = index;
	smPercentOverrideOfNoiseThresholdForNormalizationPhase::sMessageScope = scope;
}


int smPercentOverrideOfNoiseThresholdForNormalizationPhase :: GetMessageIndex () const {

	return smPercentOverrideOfNoiseThresholdForNormalizationPhase :: sMessageIndex;
}


int smPercentOverrideOfNoiseThresholdForNormalizationPhase :: GetScope () const {

	return smPercentOverrideOfNoiseThresholdForNormalizationPhase :: sMessageScope;
}


RGString smPercentOverrideOfNoiseThresholdForNormalizationPhase :: GetName () const {

	return smPercentOverrideOfNoiseThresholdForNormalizationPhase :: sName;
}



int smPercentOverrideOfNoiseThresholdForNormalizationPhase :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPercentOverrideOfNoiseThresholdForNormalizationPhase* noticeType = new smPercentOverrideOfNoiseThresholdForNormalizationPhase;
	warehouse->AddType (noticeType);
	return 1;
}





smPercentOverrideOfNoiseThresholdForFinalPhase :: smPercentOverrideOfNoiseThresholdForFinalPhase () : SmartNotice () {

}


smPercentOverrideOfNoiseThresholdForFinalPhase :: smPercentOverrideOfNoiseThresholdForFinalPhase (const smPercentOverrideOfNoiseThresholdForFinalPhase& note) : SmartNotice ((const SmartNotice&) note) {

}



smPercentOverrideOfNoiseThresholdForFinalPhase :: ~smPercentOverrideOfNoiseThresholdForFinalPhase () {

}


int smPercentOverrideOfNoiseThresholdForFinalPhase :: GetSubject () const {

	return smPercentOverrideOfNoiseThresholdForFinalPhase::sSubject;
}


void smPercentOverrideOfNoiseThresholdForFinalPhase :: SetIndexAndScope (int index, int scope) const {

	smPercentOverrideOfNoiseThresholdForFinalPhase::sMessageIndex = index;
	smPercentOverrideOfNoiseThresholdForFinalPhase::sMessageScope = scope;
}


int smPercentOverrideOfNoiseThresholdForFinalPhase :: GetMessageIndex () const {

	return smPercentOverrideOfNoiseThresholdForFinalPhase :: sMessageIndex;
}


int smPercentOverrideOfNoiseThresholdForFinalPhase :: GetScope () const {

	return smPercentOverrideOfNoiseThresholdForFinalPhase :: sMessageScope;
}


RGString smPercentOverrideOfNoiseThresholdForFinalPhase :: GetName () const {

	return smPercentOverrideOfNoiseThresholdForFinalPhase :: sName;
}



int smPercentOverrideOfNoiseThresholdForFinalPhase :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPercentOverrideOfNoiseThresholdForFinalPhase* noticeType = new smPercentOverrideOfNoiseThresholdForFinalPhase;
	warehouse->AddType (noticeType);
	return 1;
}





smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset () : SmartNotice () {

}


smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset (const smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: ~smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset () {

}


int smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: GetSubject () const {

	return smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset::sSubject;
}


void smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: SetIndexAndScope (int index, int scope) const {

	smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset::sMessageIndex = index;
	smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset::sMessageScope = scope;
}


int smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: GetMessageIndex () const {

	return smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: sMessageIndex;
}


int smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: GetScope () const {

	return smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: sMessageScope;
}


RGString smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: GetName () const {

	return smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: sName;
}



int smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset* noticeType = new smSuppressCriticalPeakLevelArtifactsForLadderAllelesPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset () : SmartNotice () {

}


smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset (const smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: ~smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset () {

}


int smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: GetSubject () const {

	return smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset::sSubject;
}


void smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: SetIndexAndScope (int index, int scope) const {

	smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset::sMessageIndex = index;
	smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset::sMessageScope = scope;
}


int smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: GetMessageIndex () const {

	return smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: sMessageIndex;
}


int smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: GetScope () const {

	return smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: sMessageScope;
}


RGString smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: GetName () const {

	return smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: sName;
}



int smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset* noticeType = new smSuppressCriticalPeakLevelArtifactsForILSPeaksPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smDisplaySigmoidalPeaksPreset :: smDisplaySigmoidalPeaksPreset () : SmartNotice () {

}


smDisplaySigmoidalPeaksPreset :: smDisplaySigmoidalPeaksPreset (const smDisplaySigmoidalPeaksPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smDisplaySigmoidalPeaksPreset :: ~smDisplaySigmoidalPeaksPreset () {

}


int smDisplaySigmoidalPeaksPreset :: GetSubject () const {

	return smDisplaySigmoidalPeaksPreset::sSubject;
}


void smDisplaySigmoidalPeaksPreset :: SetIndexAndScope (int index, int scope) const {

	smDisplaySigmoidalPeaksPreset::sMessageIndex = index;
	smDisplaySigmoidalPeaksPreset::sMessageScope = scope;
}


int smDisplaySigmoidalPeaksPreset :: GetMessageIndex () const {

	return smDisplaySigmoidalPeaksPreset :: sMessageIndex;
}


int smDisplaySigmoidalPeaksPreset :: GetScope () const {

	return smDisplaySigmoidalPeaksPreset :: sMessageScope;
}


RGString smDisplaySigmoidalPeaksPreset :: GetName () const {

	return smDisplaySigmoidalPeaksPreset :: sName;
}



int smDisplaySigmoidalPeaksPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smDisplaySigmoidalPeaksPreset* noticeType = new smDisplaySigmoidalPeaksPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smResidualDisplacementFractionalLimit :: smResidualDisplacementFractionalLimit () : SmartNotice () {

}


smResidualDisplacementFractionalLimit :: smResidualDisplacementFractionalLimit (const smResidualDisplacementFractionalLimit& note) : SmartNotice ((const SmartNotice&) note) {

}



smResidualDisplacementFractionalLimit :: ~smResidualDisplacementFractionalLimit () {

}


int smResidualDisplacementFractionalLimit :: GetSubject () const {

	return smResidualDisplacementFractionalLimit::sSubject;
}


void smResidualDisplacementFractionalLimit :: SetIndexAndScope (int index, int scope) const {

	smResidualDisplacementFractionalLimit::sMessageIndex = index;
	smResidualDisplacementFractionalLimit::sMessageScope = scope;
}


int smResidualDisplacementFractionalLimit :: GetMessageIndex () const {

	return smResidualDisplacementFractionalLimit :: sMessageIndex;
}


int smResidualDisplacementFractionalLimit :: GetScope () const {

	return smResidualDisplacementFractionalLimit :: sMessageScope;
}


RGString smResidualDisplacementFractionalLimit :: GetName () const {

	return smResidualDisplacementFractionalLimit :: sName;
}



int smResidualDisplacementFractionalLimit :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smResidualDisplacementFractionalLimit* noticeType = new smResidualDisplacementFractionalLimit;
	warehouse->AddType (noticeType);
	return 1;
}





smSelectAveragingInPlaceFilterPreset :: smSelectAveragingInPlaceFilterPreset () : SmartNotice () {

}


smSelectAveragingInPlaceFilterPreset :: smSelectAveragingInPlaceFilterPreset (const smSelectAveragingInPlaceFilterPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smSelectAveragingInPlaceFilterPreset :: ~smSelectAveragingInPlaceFilterPreset () {

}


int smSelectAveragingInPlaceFilterPreset :: GetSubject () const {

	return smSelectAveragingInPlaceFilterPreset::sSubject;
}


void smSelectAveragingInPlaceFilterPreset :: SetIndexAndScope (int index, int scope) const {

	smSelectAveragingInPlaceFilterPreset::sMessageIndex = index;
	smSelectAveragingInPlaceFilterPreset::sMessageScope = scope;
}


int smSelectAveragingInPlaceFilterPreset :: GetMessageIndex () const {

	return smSelectAveragingInPlaceFilterPreset :: sMessageIndex;
}


int smSelectAveragingInPlaceFilterPreset :: GetScope () const {

	return smSelectAveragingInPlaceFilterPreset :: sMessageScope;
}


RGString smSelectAveragingInPlaceFilterPreset :: GetName () const {

	return smSelectAveragingInPlaceFilterPreset :: sName;
}



int smSelectAveragingInPlaceFilterPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSelectAveragingInPlaceFilterPreset* noticeType = new smSelectAveragingInPlaceFilterPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smAveragingInPlaceFilterWindowWidth :: smAveragingInPlaceFilterWindowWidth () : SmartNotice () {

}


smAveragingInPlaceFilterWindowWidth :: smAveragingInPlaceFilterWindowWidth (const smAveragingInPlaceFilterWindowWidth& note) : SmartNotice ((const SmartNotice&) note) {

}



smAveragingInPlaceFilterWindowWidth :: ~smAveragingInPlaceFilterWindowWidth () {

}


int smAveragingInPlaceFilterWindowWidth :: GetSubject () const {

	return smAveragingInPlaceFilterWindowWidth::sSubject;
}


void smAveragingInPlaceFilterWindowWidth :: SetIndexAndScope (int index, int scope) const {

	smAveragingInPlaceFilterWindowWidth::sMessageIndex = index;
	smAveragingInPlaceFilterWindowWidth::sMessageScope = scope;
}


int smAveragingInPlaceFilterWindowWidth :: GetMessageIndex () const {

	return smAveragingInPlaceFilterWindowWidth :: sMessageIndex;
}


int smAveragingInPlaceFilterWindowWidth :: GetScope () const {

	return smAveragingInPlaceFilterWindowWidth :: sMessageScope;
}


RGString smAveragingInPlaceFilterWindowWidth :: GetName () const {

	return smAveragingInPlaceFilterWindowWidth :: sName;
}



int smAveragingInPlaceFilterWindowWidth :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smAveragingInPlaceFilterWindowWidth* noticeType = new smAveragingInPlaceFilterWindowWidth;
	warehouse->AddType (noticeType);
	return 1;
}





smUseProximityToPeaksOnlyPreset :: smUseProximityToPeaksOnlyPreset () : SmartNotice () {

}


smUseProximityToPeaksOnlyPreset :: smUseProximityToPeaksOnlyPreset (const smUseProximityToPeaksOnlyPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smUseProximityToPeaksOnlyPreset :: ~smUseProximityToPeaksOnlyPreset () {

}


int smUseProximityToPeaksOnlyPreset :: GetSubject () const {

	return smUseProximityToPeaksOnlyPreset::sSubject;
}


void smUseProximityToPeaksOnlyPreset :: SetIndexAndScope (int index, int scope) const {

	smUseProximityToPeaksOnlyPreset::sMessageIndex = index;
	smUseProximityToPeaksOnlyPreset::sMessageScope = scope;
}


int smUseProximityToPeaksOnlyPreset :: GetMessageIndex () const {

	return smUseProximityToPeaksOnlyPreset :: sMessageIndex;
}


int smUseProximityToPeaksOnlyPreset :: GetScope () const {

	return smUseProximityToPeaksOnlyPreset :: sMessageScope;
}


RGString smUseProximityToPeaksOnlyPreset :: GetName () const {

	return smUseProximityToPeaksOnlyPreset :: sName;
}



int smUseProximityToPeaksOnlyPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUseProximityToPeaksOnlyPreset* noticeType = new smUseProximityToPeaksOnlyPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smDistanceFromPeakThreshold :: smDistanceFromPeakThreshold () : SmartNotice () {

}


smDistanceFromPeakThreshold :: smDistanceFromPeakThreshold (const smDistanceFromPeakThreshold& note) : SmartNotice ((const SmartNotice&) note) {

}



smDistanceFromPeakThreshold :: ~smDistanceFromPeakThreshold () {

}


int smDistanceFromPeakThreshold :: GetSubject () const {

	return smDistanceFromPeakThreshold::sSubject;
}


void smDistanceFromPeakThreshold :: SetIndexAndScope (int index, int scope) const {

	smDistanceFromPeakThreshold::sMessageIndex = index;
	smDistanceFromPeakThreshold::sMessageScope = scope;
}


int smDistanceFromPeakThreshold :: GetMessageIndex () const {

	return smDistanceFromPeakThreshold :: sMessageIndex;
}


int smDistanceFromPeakThreshold :: GetScope () const {

	return smDistanceFromPeakThreshold :: sMessageScope;
}


RGString smDistanceFromPeakThreshold :: GetName () const {

	return smDistanceFromPeakThreshold :: sName;
}



int smDistanceFromPeakThreshold :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smDistanceFromPeakThreshold* noticeType = new smDistanceFromPeakThreshold;
	warehouse->AddType (noticeType);
	return 1;
}





smUseProximityBothToPeaksAndLevelChangePreset :: smUseProximityBothToPeaksAndLevelChangePreset () : SmartNotice () {

}


smUseProximityBothToPeaksAndLevelChangePreset :: smUseProximityBothToPeaksAndLevelChangePreset (const smUseProximityBothToPeaksAndLevelChangePreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smUseProximityBothToPeaksAndLevelChangePreset :: ~smUseProximityBothToPeaksAndLevelChangePreset () {

}


int smUseProximityBothToPeaksAndLevelChangePreset :: GetSubject () const {

	return smUseProximityBothToPeaksAndLevelChangePreset::sSubject;
}


void smUseProximityBothToPeaksAndLevelChangePreset :: SetIndexAndScope (int index, int scope) const {

	smUseProximityBothToPeaksAndLevelChangePreset::sMessageIndex = index;
	smUseProximityBothToPeaksAndLevelChangePreset::sMessageScope = scope;
}


int smUseProximityBothToPeaksAndLevelChangePreset :: GetMessageIndex () const {

	return smUseProximityBothToPeaksAndLevelChangePreset :: sMessageIndex;
}


int smUseProximityBothToPeaksAndLevelChangePreset :: GetScope () const {

	return smUseProximityBothToPeaksAndLevelChangePreset :: sMessageScope;
}


RGString smUseProximityBothToPeaksAndLevelChangePreset :: GetName () const {

	return smUseProximityBothToPeaksAndLevelChangePreset :: sName;
}



int smUseProximityBothToPeaksAndLevelChangePreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smUseProximityBothToPeaksAndLevelChangePreset* noticeType = new smUseProximityBothToPeaksAndLevelChangePreset;
	warehouse->AddType (noticeType);
	return 1;
}





smDistanceFromLevelChange :: smDistanceFromLevelChange () : SmartNotice () {

}


smDistanceFromLevelChange :: smDistanceFromLevelChange (const smDistanceFromLevelChange& note) : SmartNotice ((const SmartNotice&) note) {

}



smDistanceFromLevelChange :: ~smDistanceFromLevelChange () {

}


int smDistanceFromLevelChange :: GetSubject () const {

	return smDistanceFromLevelChange::sSubject;
}


void smDistanceFromLevelChange :: SetIndexAndScope (int index, int scope) const {

	smDistanceFromLevelChange::sMessageIndex = index;
	smDistanceFromLevelChange::sMessageScope = scope;
}


int smDistanceFromLevelChange :: GetMessageIndex () const {

	return smDistanceFromLevelChange :: sMessageIndex;
}


int smDistanceFromLevelChange :: GetScope () const {

	return smDistanceFromLevelChange :: sMessageScope;
}


RGString smDistanceFromLevelChange :: GetName () const {

	return smDistanceFromLevelChange :: sName;
}



int smDistanceFromLevelChange :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smDistanceFromLevelChange* noticeType = new smDistanceFromLevelChange;
	warehouse->AddType (noticeType);
	return 1;
}





smPostPrimerPercentOfNoiseRangeForLevelChange :: smPostPrimerPercentOfNoiseRangeForLevelChange () : SmartNotice () {

}


smPostPrimerPercentOfNoiseRangeForLevelChange :: smPostPrimerPercentOfNoiseRangeForLevelChange (const smPostPrimerPercentOfNoiseRangeForLevelChange& note) : SmartNotice ((const SmartNotice&) note) {

}



smPostPrimerPercentOfNoiseRangeForLevelChange :: ~smPostPrimerPercentOfNoiseRangeForLevelChange () {

}


int smPostPrimerPercentOfNoiseRangeForLevelChange :: GetSubject () const {

	return smPostPrimerPercentOfNoiseRangeForLevelChange::sSubject;
}


void smPostPrimerPercentOfNoiseRangeForLevelChange :: SetIndexAndScope (int index, int scope) const {

	smPostPrimerPercentOfNoiseRangeForLevelChange::sMessageIndex = index;
	smPostPrimerPercentOfNoiseRangeForLevelChange::sMessageScope = scope;
}


int smPostPrimerPercentOfNoiseRangeForLevelChange :: GetMessageIndex () const {

	return smPostPrimerPercentOfNoiseRangeForLevelChange :: sMessageIndex;
}


int smPostPrimerPercentOfNoiseRangeForLevelChange :: GetScope () const {

	return smPostPrimerPercentOfNoiseRangeForLevelChange :: sMessageScope;
}


RGString smPostPrimerPercentOfNoiseRangeForLevelChange :: GetName () const {

	return smPostPrimerPercentOfNoiseRangeForLevelChange :: sName;
}



int smPostPrimerPercentOfNoiseRangeForLevelChange :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPostPrimerPercentOfNoiseRangeForLevelChange* noticeType = new smPostPrimerPercentOfNoiseRangeForLevelChange;
	warehouse->AddType (noticeType);
	return 1;
}





smMaxPercentLevelChangeToUseLevelChangeProximity :: smMaxPercentLevelChangeToUseLevelChangeProximity () : SmartNotice () {

}


smMaxPercentLevelChangeToUseLevelChangeProximity :: smMaxPercentLevelChangeToUseLevelChangeProximity (const smMaxPercentLevelChangeToUseLevelChangeProximity& note) : SmartNotice ((const SmartNotice&) note) {

}



smMaxPercentLevelChangeToUseLevelChangeProximity :: ~smMaxPercentLevelChangeToUseLevelChangeProximity () {

}


int smMaxPercentLevelChangeToUseLevelChangeProximity :: GetSubject () const {

	return smMaxPercentLevelChangeToUseLevelChangeProximity::sSubject;
}


void smMaxPercentLevelChangeToUseLevelChangeProximity :: SetIndexAndScope (int index, int scope) const {

	smMaxPercentLevelChangeToUseLevelChangeProximity::sMessageIndex = index;
	smMaxPercentLevelChangeToUseLevelChangeProximity::sMessageScope = scope;
}


int smMaxPercentLevelChangeToUseLevelChangeProximity :: GetMessageIndex () const {

	return smMaxPercentLevelChangeToUseLevelChangeProximity :: sMessageIndex;
}


int smMaxPercentLevelChangeToUseLevelChangeProximity :: GetScope () const {

	return smMaxPercentLevelChangeToUseLevelChangeProximity :: sMessageScope;
}


RGString smMaxPercentLevelChangeToUseLevelChangeProximity :: GetName () const {

	return smMaxPercentLevelChangeToUseLevelChangeProximity :: sName;
}



int smMaxPercentLevelChangeToUseLevelChangeProximity :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smMaxPercentLevelChangeToUseLevelChangeProximity* noticeType = new smMaxPercentLevelChangeToUseLevelChangeProximity;
	warehouse->AddType (noticeType);
	return 1;
}





smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: smPostPrimerPercentOfNoiseRangeToBeConsideredPeak () : SmartNotice () {

}


smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: smPostPrimerPercentOfNoiseRangeToBeConsideredPeak (const smPostPrimerPercentOfNoiseRangeToBeConsideredPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: ~smPostPrimerPercentOfNoiseRangeToBeConsideredPeak () {

}


int smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: GetSubject () const {

	return smPostPrimerPercentOfNoiseRangeToBeConsideredPeak::sSubject;
}


void smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: SetIndexAndScope (int index, int scope) const {

	smPostPrimerPercentOfNoiseRangeToBeConsideredPeak::sMessageIndex = index;
	smPostPrimerPercentOfNoiseRangeToBeConsideredPeak::sMessageScope = scope;
}


int smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: GetMessageIndex () const {

	return smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: sMessageIndex;
}


int smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: GetScope () const {

	return smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: sMessageScope;
}


RGString smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: GetName () const {

	return smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: sName;
}



int smPostPrimerPercentOfNoiseRangeToBeConsideredPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPostPrimerPercentOfNoiseRangeToBeConsideredPeak* noticeType = new smPostPrimerPercentOfNoiseRangeToBeConsideredPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smILSBPForEndOfPrimerPeaks :: smILSBPForEndOfPrimerPeaks () : SmartNotice () {

}


smILSBPForEndOfPrimerPeaks :: smILSBPForEndOfPrimerPeaks (const smILSBPForEndOfPrimerPeaks& note) : SmartNotice ((const SmartNotice&) note) {

}



smILSBPForEndOfPrimerPeaks :: ~smILSBPForEndOfPrimerPeaks () {

}


int smILSBPForEndOfPrimerPeaks :: GetSubject () const {

	return smILSBPForEndOfPrimerPeaks::sSubject;
}


void smILSBPForEndOfPrimerPeaks :: SetIndexAndScope (int index, int scope) const {

	smILSBPForEndOfPrimerPeaks::sMessageIndex = index;
	smILSBPForEndOfPrimerPeaks::sMessageScope = scope;
}


int smILSBPForEndOfPrimerPeaks :: GetMessageIndex () const {

	return smILSBPForEndOfPrimerPeaks :: sMessageIndex;
}


int smILSBPForEndOfPrimerPeaks :: GetScope () const {

	return smILSBPForEndOfPrimerPeaks :: sMessageScope;
}


RGString smILSBPForEndOfPrimerPeaks :: GetName () const {

	return smILSBPForEndOfPrimerPeaks :: sName;
}



int smILSBPForEndOfPrimerPeaks :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smILSBPForEndOfPrimerPeaks* noticeType = new smILSBPForEndOfPrimerPeaks;
	warehouse->AddType (noticeType);
	return 1;
}





smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: smPrePrimerPercentOfNoiseRangeToBeConsideredPeak () : SmartNotice () {

}


smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: smPrePrimerPercentOfNoiseRangeToBeConsideredPeak (const smPrePrimerPercentOfNoiseRangeToBeConsideredPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: ~smPrePrimerPercentOfNoiseRangeToBeConsideredPeak () {

}


int smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: GetSubject () const {

	return smPrePrimerPercentOfNoiseRangeToBeConsideredPeak::sSubject;
}


void smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: SetIndexAndScope (int index, int scope) const {

	smPrePrimerPercentOfNoiseRangeToBeConsideredPeak::sMessageIndex = index;
	smPrePrimerPercentOfNoiseRangeToBeConsideredPeak::sMessageScope = scope;
}


int smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: GetMessageIndex () const {

	return smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: sMessageIndex;
}


int smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: GetScope () const {

	return smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: sMessageScope;
}


RGString smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: GetName () const {

	return smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: sName;
}



int smPrePrimerPercentOfNoiseRangeToBeConsideredPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPrePrimerPercentOfNoiseRangeToBeConsideredPeak* noticeType = new smPrePrimerPercentOfNoiseRangeToBeConsideredPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smPrePrimerPercentOfNoiseRangeForLevelChange :: smPrePrimerPercentOfNoiseRangeForLevelChange () : SmartNotice () {

}


smPrePrimerPercentOfNoiseRangeForLevelChange :: smPrePrimerPercentOfNoiseRangeForLevelChange (const smPrePrimerPercentOfNoiseRangeForLevelChange& note) : SmartNotice ((const SmartNotice&) note) {

}



smPrePrimerPercentOfNoiseRangeForLevelChange :: ~smPrePrimerPercentOfNoiseRangeForLevelChange () {

}


int smPrePrimerPercentOfNoiseRangeForLevelChange :: GetSubject () const {

	return smPrePrimerPercentOfNoiseRangeForLevelChange::sSubject;
}


void smPrePrimerPercentOfNoiseRangeForLevelChange :: SetIndexAndScope (int index, int scope) const {

	smPrePrimerPercentOfNoiseRangeForLevelChange::sMessageIndex = index;
	smPrePrimerPercentOfNoiseRangeForLevelChange::sMessageScope = scope;
}


int smPrePrimerPercentOfNoiseRangeForLevelChange :: GetMessageIndex () const {

	return smPrePrimerPercentOfNoiseRangeForLevelChange :: sMessageIndex;
}


int smPrePrimerPercentOfNoiseRangeForLevelChange :: GetScope () const {

	return smPrePrimerPercentOfNoiseRangeForLevelChange :: sMessageScope;
}


RGString smPrePrimerPercentOfNoiseRangeForLevelChange :: GetName () const {

	return smPrePrimerPercentOfNoiseRangeForLevelChange :: sName;
}



int smPrePrimerPercentOfNoiseRangeForLevelChange :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smPrePrimerPercentOfNoiseRangeForLevelChange* noticeType = new smPrePrimerPercentOfNoiseRangeForLevelChange;
	warehouse->AddType (noticeType);
	return 1;
}





smWeakPrimaryInterchannelLink :: smWeakPrimaryInterchannelLink () : SmartNotice () {

}


smWeakPrimaryInterchannelLink :: smWeakPrimaryInterchannelLink (const smWeakPrimaryInterchannelLink& note) : SmartNotice ((const SmartNotice&) note) {

}



smWeakPrimaryInterchannelLink :: ~smWeakPrimaryInterchannelLink () {

}


int smWeakPrimaryInterchannelLink :: GetSubject () const {

	return smWeakPrimaryInterchannelLink::sSubject;
}


void smWeakPrimaryInterchannelLink :: SetIndexAndScope (int index, int scope) const {

	smWeakPrimaryInterchannelLink::sMessageIndex = index;
	smWeakPrimaryInterchannelLink::sMessageScope = scope;
}


int smWeakPrimaryInterchannelLink :: GetMessageIndex () const {

	return smWeakPrimaryInterchannelLink :: sMessageIndex;
}


int smWeakPrimaryInterchannelLink :: GetScope () const {

	return smWeakPrimaryInterchannelLink :: sMessageScope;
}


RGString smWeakPrimaryInterchannelLink :: GetName () const {

	return smWeakPrimaryInterchannelLink :: sName;
}



int smWeakPrimaryInterchannelLink :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smWeakPrimaryInterchannelLink* noticeType = new smWeakPrimaryInterchannelLink;
	warehouse->AddType (noticeType);
	return 1;
}





smZeroPullupPrimaryInterchannelLink :: smZeroPullupPrimaryInterchannelLink () : SmartNotice () {

}


smZeroPullupPrimaryInterchannelLink :: smZeroPullupPrimaryInterchannelLink (const smZeroPullupPrimaryInterchannelLink& note) : SmartNotice ((const SmartNotice&) note) {

}



smZeroPullupPrimaryInterchannelLink :: ~smZeroPullupPrimaryInterchannelLink () {

}


int smZeroPullupPrimaryInterchannelLink :: GetSubject () const {

	return smZeroPullupPrimaryInterchannelLink::sSubject;
}


void smZeroPullupPrimaryInterchannelLink :: SetIndexAndScope (int index, int scope) const {

	smZeroPullupPrimaryInterchannelLink::sMessageIndex = index;
	smZeroPullupPrimaryInterchannelLink::sMessageScope = scope;
}


int smZeroPullupPrimaryInterchannelLink :: GetMessageIndex () const {

	return smZeroPullupPrimaryInterchannelLink :: sMessageIndex;
}


int smZeroPullupPrimaryInterchannelLink :: GetScope () const {

	return smZeroPullupPrimaryInterchannelLink :: sMessageScope;
}


RGString smZeroPullupPrimaryInterchannelLink :: GetName () const {

	return smZeroPullupPrimaryInterchannelLink :: sName;
}



int smZeroPullupPrimaryInterchannelLink :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smZeroPullupPrimaryInterchannelLink* noticeType = new smZeroPullupPrimaryInterchannelLink;
	warehouse->AddType (noticeType);
	return 1;
}





smRawDataPrimaryInterchannelLink :: smRawDataPrimaryInterchannelLink () : SmartNotice () {

}


smRawDataPrimaryInterchannelLink :: smRawDataPrimaryInterchannelLink (const smRawDataPrimaryInterchannelLink& note) : SmartNotice ((const SmartNotice&) note) {

}



smRawDataPrimaryInterchannelLink :: ~smRawDataPrimaryInterchannelLink () {

}


int smRawDataPrimaryInterchannelLink :: GetSubject () const {

	return smRawDataPrimaryInterchannelLink::sSubject;
}


void smRawDataPrimaryInterchannelLink :: SetIndexAndScope (int index, int scope) const {

	smRawDataPrimaryInterchannelLink::sMessageIndex = index;
	smRawDataPrimaryInterchannelLink::sMessageScope = scope;
}


int smRawDataPrimaryInterchannelLink :: GetMessageIndex () const {

	return smRawDataPrimaryInterchannelLink :: sMessageIndex;
}


int smRawDataPrimaryInterchannelLink :: GetScope () const {

	return smRawDataPrimaryInterchannelLink :: sMessageScope;
}


RGString smRawDataPrimaryInterchannelLink :: GetName () const {

	return smRawDataPrimaryInterchannelLink :: sName;
}



int smRawDataPrimaryInterchannelLink :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smRawDataPrimaryInterchannelLink* noticeType = new smRawDataPrimaryInterchannelLink;
	warehouse->AddType (noticeType);
	return 1;
}





smThreePointPeak :: smThreePointPeak () : SmartNotice () {

}


smThreePointPeak :: smThreePointPeak (const smThreePointPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smThreePointPeak :: ~smThreePointPeak () {

}


int smThreePointPeak :: GetSubject () const {

	return smThreePointPeak::sSubject;
}


void smThreePointPeak :: SetIndexAndScope (int index, int scope) const {

	smThreePointPeak::sMessageIndex = index;
	smThreePointPeak::sMessageScope = scope;
}


int smThreePointPeak :: GetMessageIndex () const {

	return smThreePointPeak :: sMessageIndex;
}


int smThreePointPeak :: GetScope () const {

	return smThreePointPeak :: sMessageScope;
}


RGString smThreePointPeak :: GetName () const {

	return smThreePointPeak :: sName;
}



int smThreePointPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smThreePointPeak* noticeType = new smThreePointPeak;
	warehouse->AddType (noticeType);
	return 1;
}





smIncludeNonStandardStutterInStutterOverlapPreset :: smIncludeNonStandardStutterInStutterOverlapPreset () : SmartNotice () {

}


smIncludeNonStandardStutterInStutterOverlapPreset :: smIncludeNonStandardStutterInStutterOverlapPreset (const smIncludeNonStandardStutterInStutterOverlapPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smIncludeNonStandardStutterInStutterOverlapPreset :: ~smIncludeNonStandardStutterInStutterOverlapPreset () {

}


int smIncludeNonStandardStutterInStutterOverlapPreset :: GetSubject () const {

	return smIncludeNonStandardStutterInStutterOverlapPreset::sSubject;
}


void smIncludeNonStandardStutterInStutterOverlapPreset :: SetIndexAndScope (int index, int scope) const {

	smIncludeNonStandardStutterInStutterOverlapPreset::sMessageIndex = index;
	smIncludeNonStandardStutterInStutterOverlapPreset::sMessageScope = scope;
}


int smIncludeNonStandardStutterInStutterOverlapPreset :: GetMessageIndex () const {

	return smIncludeNonStandardStutterInStutterOverlapPreset :: sMessageIndex;
}


int smIncludeNonStandardStutterInStutterOverlapPreset :: GetScope () const {

	return smIncludeNonStandardStutterInStutterOverlapPreset :: sMessageScope;
}


RGString smIncludeNonStandardStutterInStutterOverlapPreset :: GetName () const {

	return smIncludeNonStandardStutterInStutterOverlapPreset :: sName;
}



int smIncludeNonStandardStutterInStutterOverlapPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smIncludeNonStandardStutterInStutterOverlapPreset* noticeType = new smIncludeNonStandardStutterInStutterOverlapPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smNoNegativeDataPreset :: smNoNegativeDataPreset () : SmartNotice () {

}


smNoNegativeDataPreset :: smNoNegativeDataPreset (const smNoNegativeDataPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smNoNegativeDataPreset :: ~smNoNegativeDataPreset () {

}


int smNoNegativeDataPreset :: GetSubject () const {

	return smNoNegativeDataPreset::sSubject;
}


void smNoNegativeDataPreset :: SetIndexAndScope (int index, int scope) const {

	smNoNegativeDataPreset::sMessageIndex = index;
	smNoNegativeDataPreset::sMessageScope = scope;
}


int smNoNegativeDataPreset :: GetMessageIndex () const {

	return smNoNegativeDataPreset :: sMessageIndex;
}


int smNoNegativeDataPreset :: GetScope () const {

	return smNoNegativeDataPreset :: sMessageScope;
}


RGString smNoNegativeDataPreset :: GetName () const {

	return smNoNegativeDataPreset :: sName;
}



int smNoNegativeDataPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smNoNegativeDataPreset* noticeType = new smNoNegativeDataPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smTestPullupCorrectedHeightsPreset :: smTestPullupCorrectedHeightsPreset () : SmartNotice () {

}


smTestPullupCorrectedHeightsPreset :: smTestPullupCorrectedHeightsPreset (const smTestPullupCorrectedHeightsPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smTestPullupCorrectedHeightsPreset :: ~smTestPullupCorrectedHeightsPreset () {

}


int smTestPullupCorrectedHeightsPreset :: GetSubject () const {

	return smTestPullupCorrectedHeightsPreset::sSubject;
}


void smTestPullupCorrectedHeightsPreset :: SetIndexAndScope (int index, int scope) const {

	smTestPullupCorrectedHeightsPreset::sMessageIndex = index;
	smTestPullupCorrectedHeightsPreset::sMessageScope = scope;
}


int smTestPullupCorrectedHeightsPreset :: GetMessageIndex () const {

	return smTestPullupCorrectedHeightsPreset :: sMessageIndex;
}


int smTestPullupCorrectedHeightsPreset :: GetScope () const {

	return smTestPullupCorrectedHeightsPreset :: sMessageScope;
}


RGString smTestPullupCorrectedHeightsPreset :: GetName () const {

	return smTestPullupCorrectedHeightsPreset :: sName;
}



int smTestPullupCorrectedHeightsPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smTestPullupCorrectedHeightsPreset* noticeType = new smTestPullupCorrectedHeightsPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smTailHeightFittingThresholdFactor :: smTailHeightFittingThresholdFactor () : SmartNotice () {

}


smTailHeightFittingThresholdFactor :: smTailHeightFittingThresholdFactor (const smTailHeightFittingThresholdFactor& note) : SmartNotice ((const SmartNotice&) note) {

}



smTailHeightFittingThresholdFactor :: ~smTailHeightFittingThresholdFactor () {

}


int smTailHeightFittingThresholdFactor :: GetSubject () const {

	return smTailHeightFittingThresholdFactor::sSubject;
}


void smTailHeightFittingThresholdFactor :: SetIndexAndScope (int index, int scope) const {

	smTailHeightFittingThresholdFactor::sMessageIndex = index;
	smTailHeightFittingThresholdFactor::sMessageScope = scope;
}


int smTailHeightFittingThresholdFactor :: GetMessageIndex () const {

	return smTailHeightFittingThresholdFactor :: sMessageIndex;
}


int smTailHeightFittingThresholdFactor :: GetScope () const {

	return smTailHeightFittingThresholdFactor :: sMessageScope;
}


RGString smTailHeightFittingThresholdFactor :: GetName () const {

	return smTailHeightFittingThresholdFactor :: sName;
}



int smTailHeightFittingThresholdFactor :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smTailHeightFittingThresholdFactor* noticeType = new smTailHeightFittingThresholdFactor;
	warehouse->AddType (noticeType);
	return 1;
}





smTailSlopeFittingThresholdFactor :: smTailSlopeFittingThresholdFactor () : SmartNotice () {

}


smTailSlopeFittingThresholdFactor :: smTailSlopeFittingThresholdFactor (const smTailSlopeFittingThresholdFactor& note) : SmartNotice ((const SmartNotice&) note) {

}



smTailSlopeFittingThresholdFactor :: ~smTailSlopeFittingThresholdFactor () {

}


int smTailSlopeFittingThresholdFactor :: GetSubject () const {

	return smTailSlopeFittingThresholdFactor::sSubject;
}


void smTailSlopeFittingThresholdFactor :: SetIndexAndScope (int index, int scope) const {

	smTailSlopeFittingThresholdFactor::sMessageIndex = index;
	smTailSlopeFittingThresholdFactor::sMessageScope = scope;
}


int smTailSlopeFittingThresholdFactor :: GetMessageIndex () const {

	return smTailSlopeFittingThresholdFactor :: sMessageIndex;
}


int smTailSlopeFittingThresholdFactor :: GetScope () const {

	return smTailSlopeFittingThresholdFactor :: sMessageScope;
}


RGString smTailSlopeFittingThresholdFactor :: GetName () const {

	return smTailSlopeFittingThresholdFactor :: sName;
}



int smTailSlopeFittingThresholdFactor :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smTailSlopeFittingThresholdFactor* noticeType = new smTailSlopeFittingThresholdFactor;
	warehouse->AddType (noticeType);
	return 1;
}





smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: smSelectUserSpecifiedMinRFUForPrimaryPeakPreset () : SmartNotice () {

}


smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: smSelectUserSpecifiedMinRFUForPrimaryPeakPreset (const smSelectUserSpecifiedMinRFUForPrimaryPeakPreset& note) : SmartNotice ((const SmartNotice&) note) {

}



smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: ~smSelectUserSpecifiedMinRFUForPrimaryPeakPreset () {

}


int smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: GetSubject () const {

	return smSelectUserSpecifiedMinRFUForPrimaryPeakPreset::sSubject;
}


void smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: SetIndexAndScope (int index, int scope) const {

	smSelectUserSpecifiedMinRFUForPrimaryPeakPreset::sMessageIndex = index;
	smSelectUserSpecifiedMinRFUForPrimaryPeakPreset::sMessageScope = scope;
}


int smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: GetMessageIndex () const {

	return smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: sMessageIndex;
}


int smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: GetScope () const {

	return smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: sMessageScope;
}


RGString smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: GetName () const {

	return smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: sName;
}



int smSelectUserSpecifiedMinRFUForPrimaryPeakPreset :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smSelectUserSpecifiedMinRFUForPrimaryPeakPreset* noticeType = new smSelectUserSpecifiedMinRFUForPrimaryPeakPreset;
	warehouse->AddType (noticeType);
	return 1;
}





smRedundantPeak :: smRedundantPeak () : SmartNotice () {

}


smRedundantPeak :: smRedundantPeak (const smRedundantPeak& note) : SmartNotice ((const SmartNotice&) note) {

}



smRedundantPeak :: ~smRedundantPeak () {

}


int smRedundantPeak :: GetSubject () const {

	return smRedundantPeak::sSubject;
}


void smRedundantPeak :: SetIndexAndScope (int index, int scope) const {

	smRedundantPeak::sMessageIndex = index;
	smRedundantPeak::sMessageScope = scope;
}


int smRedundantPeak :: GetMessageIndex () const {

	return smRedundantPeak :: sMessageIndex;
}


int smRedundantPeak :: GetScope () const {

	return smRedundantPeak :: sMessageScope;
}


RGString smRedundantPeak :: GetName () const {

	return smRedundantPeak :: sName;
}



int smRedundantPeak :: LoadType () {

	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;
	smRedundantPeak* noticeType = new smRedundantPeak;
	warehouse->AddType (noticeType);
	return 1;
}




