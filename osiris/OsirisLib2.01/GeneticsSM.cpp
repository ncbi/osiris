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
*  FileName: GeneticsSM.cpp
*  Author:   Robert Goor
*
*/
//
//   These are the abstract base classes for the population genetic categories:  class BaseAllele, BaseLocus, PopulationMarkerSet and PopulationCollection,
// class Allele and Locus...These are the "SM", or SmartMessage functions
//

#include "Genetics.h"
#include "DataSignal.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgtokenizer.h"
#include "coordtrans.h"
#include "RGTextOutput.h"
#include "OsirisMsg.h"
#include "OutputLevelManager.h"
#include "STRNotices.h"
#include "ControlFit.h"
#include "ListFunctions.h"
#include "RecursiveInnerProduct.h"
#include "ChannelData.h"
#include "IndividualGenotype.h"
#include "SmartMessage.h"
#include "SmartNotice.h"
#include "STRSmartNotices.h"

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;


// Smart Message Functions*************************************************************************
//*************************************************************************************************



bool Locus :: EvaluateSmartMessagesForStage (int stage) {

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) it ())
		nextSignal->EvaluateSmartMessagesForStage (stage);

	return SmartMessage::EvaluateAllMessages (mMessageArray, LocusSignalList, stage, GetObjectScope ());
}


bool Locus :: EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage) {

	RGDListIterator it (mSmartList);
	DataSignal* nextSignal;
	comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) this;
	int topNum = numHigherObjects + 1;

	while (nextSignal = (DataSignal*) it ())
		nextSignal->EvaluateSmartMessagesForStage (comm, topNum, stage);

	return SmartMessage::EvaluateAllMessages (comm, topNum, stage, GetObjectScope ());
}


bool Locus :: EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly) {

	RGDListIterator it (mSmartList);
	DataSignal* nextSignal;
	comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) this;
	int topNum = numHigherObjects + 1;
	bool evaluateNonSignals = allMessages || (!signalsOnly);
	bool evaluateSignals = allMessages || signalsOnly;
	bool isAmbiguous;
	bool isCore;
	bool isInLocus;
	const Locus* coreLocus;

	if (evaluateSignals) {

		while (nextSignal = (DataSignal*) it ()) {

			isAmbiguous = nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1);
			coreLocus = nextSignal->GetLocus (0);
			isCore = (coreLocus != NULL);
			isInLocus = (coreLocus == this);

			if (isCore && isInLocus)
				nextSignal->EvaluateSmartMessagesForStage (comm, topNum, stage);

			else if (isCore)
				continue;

			else if (!isAmbiguous)
				nextSignal->EvaluateSmartMessagesForStage (comm, topNum, stage);
		}
	}

	if (evaluateNonSignals)
		SmartMessage::EvaluateAllMessages (comm, topNum, stage, GetObjectScope ());

	return true;
}


bool Locus :: SetTriggersForAllMessages (bool* const higherMsgMatrix, int stage, bool allMessages, bool signalsOnly) {

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	bool status = true;
	bool evaluateNonSignals = allMessages || (!signalsOnly);
	bool evaluateSignals = allMessages || signalsOnly;

	if (evaluateSignals) {

		while (nextSignal = (DataSignal*) it ())
			nextSignal->SetTriggersForAllMessages (mMessageArray, mMessageDataTable, GetObjectScope (), stage);
	}

	if (evaluateNonSignals)
		status = SmartMessage::SetTriggersForAllMessages (mMessageArray, higherMsgMatrix, stage, GetObjectScope ());

	return status;
}


bool Locus :: SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly) {

	RGDListIterator it (mSmartList);
	DataSignal* nextSignal;
	bool status = true;
	bool evaluateNonSignals = allMessages || (!signalsOnly);
	bool evaluateSignals = allMessages || signalsOnly;
	comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) mLink;
	comm.SMOStack [numHigherObjects + 1] = (SmartMessagingObject*) this;
	int newNum = numHigherObjects + 2;
	bool isAmbiguous;
	bool isCore;
	bool isInLocus;
	const Locus* coreLocus;

	if (evaluateSignals) {

		while (nextSignal = (DataSignal*) it ()) {

			isAmbiguous = nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1);
			coreLocus = nextSignal->GetLocus (0);
			isCore = (coreLocus != NULL);
			isInLocus = (coreLocus == this);

			if (isCore && isInLocus)
				nextSignal->SetTriggersForAllMessages (comm, newNum, stage);

			else if (isCore)
				continue;

			else if (!isAmbiguous)
				nextSignal->SetTriggersForAllMessages (comm, newNum, stage);
		}
	}

	if (evaluateNonSignals)
		status = SmartMessage::SetTriggersForAllMessages (comm, newNum, stage, GetObjectScope ());

	return status;
}


bool Locus :: EvaluateAllReports (bool* const reportMatrix) {

	return SmartMessage::EvaluateAllReports (mMessageArray, reportMatrix, GetObjectScope ());
}


bool Locus :: TestAllMessagesForCall () {

	return SmartMessage::TestAllMessagesForCall (mMessageArray, GetObjectScope ());
}


bool Locus :: EvaluateAllReportLevels (int* const reportLevelMatrix) {

	return SmartMessage::EvaluateAllReportLevels (mMessageArray, reportLevelMatrix, GetObjectScope ());
}


bool Locus :: ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, bool reportLink) {

	if (NumberOfSmartNoticeObjects () > 0) {

		int msgLevel = GetHighestMessageLevelWithRestrictionSM ();
		RGDListIterator it (*mSmartMessageReporters);
		SmartMessageReporter* nextNotice;
		text.SetOutputLevel (msgLevel);
		RGString indent2 = indent + "\t";

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			return false;
		}

		Endl endLine;
		text << endLine;

		if (reportLink)
			text << mTableLink;

		text << indent;
		text << GetLocusName () << " Notices:  " << endLine;

		while (nextNotice = (SmartMessageReporter*) it ())
			text << indent2 << nextNotice->GetMessage () << nextNotice->GetMessageData () << endLine;

		text.ResetOutputLevel ();

		if (reportLink) {

			text.SetOutputLevel (msgLevel);
			text << mTableLink;
			text.ResetOutputLevel ();
		}

		text.Write (1, "\n");
	}

	else
		return false;

	return true;
}


void Locus :: OutputDebugID (SmartMessagingComm& comm, int numHigherObjects) {

	RGString idData;
	int higherObjectIndex = numHigherObjects - 2;
	SmartMessagingObject* higherObj = comm.SMOStack [higherObjectIndex];
	higherObj->OutputDebugID (comm, higherObjectIndex + 1);
	idData << "\t\tLocus:  " << mLink->GetLocusName ();
	SmartMessage::OutputDebugString (idData);
}


RGString Locus :: GetDebugIDIndent () const {

	return "\t\t";
}


int Locus :: AddAllSmartMessageReporters () {

	int k = GetObjectScope ();
	int size = SmartMessage::GetSizeOfArrayForScope (k);
	int i;
	int nMsgs = 0;
	SmartMessageReporter* newMsg;
	SmartMessage* nextSmartMsg;
	SmartMessageData target;
	SmartMessageData* smd;

	for (i=0; i<size; i++) {

		if (!mMessageArray [i])
			continue;

		nextSmartMsg = SmartMessage::GetSmartMessageForScopeAndElement (k, i);

		if (!nextSmartMsg->EvaluateReport (mMessageArray))
			continue;

		target.SetIndex (i);
		smd = (SmartMessageData*) mMessageDataTable->Find (&target);
		newMsg = new SmartMessageReporter;
		newMsg->SetSmartMessage (nextSmartMsg);
		
		if (smd != NULL)
			newMsg->SetData (smd);

		newMsg->SetPriorityLevel (nextSmartMsg->EvaluateReportLevel (mMessageArray));
		newMsg->SetRestrictionLevel (nextSmartMsg->EvaluateRestrictionLevel (mMessageArray));
		nMsgs = AddSmartMessageReporter (newMsg);
	}

	MergeAllSmartMessageReporters ();
	return nMsgs;
}


int Locus :: AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects) {

	int k = GetObjectScope ();
	int size = SmartMessage::GetSizeOfArrayForScope (k);
	int i;
	int nMsgs = 0;
	SmartMessageReporter* newMsg;
	SmartMessage* nextSmartMsg;
	SmartMessageData target;
	SmartMessageData* smd;
	bool editable;
	bool enabled;
	bool hasExportProtocolInfo;
	bool report;
	bool mirror;
	bool displayExport;

	comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) this;
	int topNum = numHigherObjects + 1;

	for (i=0; i<size; i++) {

		nextSmartMsg = SmartMessage::GetSmartMessageForScopeAndElement (k, i);
		editable = nextSmartMsg->IsEditable ();
		hasExportProtocolInfo = nextSmartMsg->HasExportProtocolInfo ();

		if (!mMessageArray [i]) {

			enabled = false;

			if (!editable)
				continue;

			if (!hasExportProtocolInfo)
				continue;
		}

		else
			enabled = true;

		report = nextSmartMsg->EvaluateReportContingent (comm, topNum);
		mirror = nextSmartMsg->UseDefaultExportDisplayMode ();

		if (mirror)
			displayExport = report;

		else
			displayExport = nextSmartMsg->DisplayExportInfo ();

		if (!report && !displayExport)
			continue;

		target.SetIndex (i);
		smd = (SmartMessageData*) mMessageDataTable->Find (&target);
		newMsg = new SmartMessageReporter;
		newMsg->SetSmartMessage (nextSmartMsg);
		
		if (smd != NULL)
			newMsg->SetData (smd);

		newMsg->SetPriorityLevel (nextSmartMsg->EvaluateReportLevel (comm, topNum));
		newMsg->SetRestrictionLevel (nextSmartMsg->EvaluateRestrictionLevel (comm, topNum));
		newMsg->SetEditable (editable);
		newMsg->SetEnabled (enabled);
		newMsg->SetDisplayExportInfo (displayExport);
		newMsg->SetDisplayOsirisInfo (report);

		if (hasExportProtocolInfo) {
			
			newMsg->SetExportProtocolInformation (nextSmartMsg->GetExportProtocolList ());
			SmartMessagingObject::InsertExportSpecificationsIntoTable (nextSmartMsg);
		}

		newMsg->ComputeViabilityOfExportInfo ();
		nMsgs = AddSmartMessageReporter (newMsg);
	}

	MergeAllSmartMessageReporters ();
	return nMsgs;
}


int Locus :: AddAllSmartMessageReportersForSignals (SmartMessagingComm& comm, int numHigherObjects) {

	int nMsgs = 0;

//test	RGDListIterator signals (LocusSignalList);
	RGDListIterator signals (mSmartList);
	DataSignal* nextSignal;
	smPeakOutsideILS peakOutsideILS;
	comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) this;
	int topNum = numHigherObjects + 1;

	bool isAmbiguous;
	bool isCore;
	bool isInLocus;
	const Locus* coreLocus;

	while (nextSignal = (DataSignal*) signals ()) {

	//	if (!nextSignal->GetMessageValue (peakOutsideILS)) {

			isAmbiguous = nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1);
			coreLocus = nextSignal->GetLocus (0);
			isCore = (coreLocus != NULL);
			isInLocus = (coreLocus == this);

			if (isCore && isInLocus)
				nMsgs += nextSignal->AddAllSmartMessageReporters (comm, topNum);

			else if (isCore)
				continue;

			else if (!isAmbiguous)
				nMsgs += nextSignal->AddAllSmartMessageReporters (comm, topNum);
		//}
	}

	return nMsgs;
}


Boolean Locus :: ReportXMLSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	if (NumberOfSmartNoticeObjects () > 0) {

		int msgLevel = GetHighestMessageLevelWithRestrictionSM ();
		RGDListIterator it (*mSmartMessageReporters);
		SmartMessageReporter* nextNotice;
		text.SetOutputLevel (msgLevel);
		tempText.SetOutputLevel (1);
		int msgNum;
		int triggerLevel = Notice::GetMessageTrigger ();
		bool includesExportInfo = false;
		bool viable;

		while (nextNotice = (SmartMessageReporter*) it ()) {

			if (nextNotice->HasViableExportInfo ()) {

				includesExportInfo = true;
				break;
			}
		}

		if (!text.TestCurrentLevel () && !includesExportInfo) {

			text.ResetOutputLevel ();
			tempText.ResetOutputLevel ();
			return FALSE;
		}

		text.ResetOutputLevel ();
		text << CLevel (1) << "\t\t\t\t<LocusAlerts>\n" << PLevel ();
		text.SetOutputLevel (1);
		it.Reset ();

//		if ((msgLevel > 0) && (msgLevel <= triggerLevel)) {

		while (nextNotice = (SmartMessageReporter*) it ()) {

			viable = nextNotice->HasViableExportInfo ();
			msgLevel = nextNotice->GetMessagePriority ();

			if (((msgLevel > 0) && (msgLevel <= triggerLevel)) || viable) {

				msgNum = Notice::GetNextMessageNumber ();
				nextNotice->SetMessageCount (msgNum);
				text << "\t\t\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
				tempText << "\t\t<Message>\n";
				tempText << "\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
				tempText << "\t\t\t<Text>" << nextNotice->GetMessage () << nextNotice->GetMessageData () << "</Text>\n";

				if (viable) {

					if (nextNotice->IsEnabled ())
						tempText << "\t\t\t<Hidden>false</Hidden>\n";

					else
						tempText << "\t\t\t<Hidden>true</Hidden>\n";

					if (!nextNotice->IsCritical ())
						tempText << "\t\t\t<Critical>false</Critical>\n";

					if (nextNotice->IsEnabled ())
						tempText << "\t\t\t<Enabled>true</Enabled>\n";

					else
						tempText << "\t\t\t<Enabled>false</Enabled>\n";

					if (!nextNotice->IsEditable ())
						tempText << "\t\t\t<Editable>false</Editable>\n";

					if (nextNotice->GetDisplayExportInfo ())
						tempText << "\t\t\t<DisplayExportInfo>true</DisplayExportInfo>\n";

					else
						tempText << "\t\t\t<DisplayExportInfo>false</DisplayExportInfo>\n";

					if (!nextNotice->GetDisplayOsirisInfo ())
						tempText << "\t\t\t<DisplayOsirisInfo>false</DisplayOsirisInfo>\n";

					tempText << "\t\t\t<MsgName>" << nextNotice->GetMessageName () << "</MsgName>\n";

					//tempText << "\t\t\t<ExportProtocolList>";
					//tempText << "\t\t\t" << nextNotice->GetExportProtocolInformation ();
					//tempText << "\t\t\t</ExportProtocolList>\n";
				}

				else
					tempText << "\t\t\t<MsgName>" << nextNotice->GetMessageName () << "</MsgName>\n";

				tempText << "\t\t</Message>\n";
			}
		}
//		}

		text.ResetOutputLevel ();
		text << CLevel (1) << "\t\t\t\t</LocusAlerts>\n" << PLevel (); 
		tempText.ResetOutputLevel ();
	}

	else
		return FALSE;

	return TRUE;
}


void Locus :: ReportXMLSmartGridTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	text << CLevel (1) << "\t\t\t<Locus>\n";
	text << "\t\t\t\t<LocusName>" << GetLocusName ().GetData () << "</LocusName>\n" << PLevel ();
	ReportXMLSmartNoticeObjects (text, tempText, delim);
	text << CLevel (1) << "\t\t\t</Locus>\n" << PLevel ();
}



void Locus :: ReportXMLSmartSampleTableRowWithLinks (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	text << CLevel (1) << "\t\t\t<Locus>\n";
	text << "\t\t\t\t<LocusName>" << GetLocusName ().GetData () << "</LocusName>\n" << PLevel ();
	int msgLevel = GetHighestMessageLevelWithRestrictionSM ();

//	if ((msgLevel > 0) && (msgLevel <= Notice::GetMessageTrigger ()))

	ReportXMLSmartNoticeObjects (text, tempText, delim);

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	double bp;
	int IntBP;
	text.SetOutputLevel (1);
	bool isHomozygote = false;
	double totalCorrection;

	if ((LocusSignalList.Entries () == 1) && (!mLink->isYLinked ()))
		isHomozygote = true;

	while (nextSignal = (DataSignal*) it ()) {

		bp = nextSignal->GetBioID ();
		IntBP = (int) floor (bp + 0.5);
		text << "\t\t\t\t<Allele>\n";
		text << "\t\t\t\t\t<Id>" << nextSignal->GetSignalID () << "</Id>\n";
		text << "\t\t\t\t\t<Name>" << nextSignal->GetAlleleName () << "</Name>\n";
		text << "\t\t\t\t\t<BPS>" << bp << "</BPS>\n";
		text << "\t\t\t\t\t<RFU>" << (int) floor (nextSignal->Peak () + 0.5) << "</RFU>\n";

		totalCorrection = nextSignal->GetTotalPullupFromOtherChannels (NumberOfChannels);

		if (totalCorrection != 0.0)
			text << "\t\t\t\t\t<PullupHeightCorrection>" << totalCorrection << "</PullupHeightCorrection>\n";

		text << "\t\t\t\t\t<meanbps>" << nextSignal->GetApproximateBioID () << "</meanbps>\n";
		text << "\t\t\t\t\t<PeakArea>" << nextSignal->TheoreticalArea () << "</PeakArea>\n";
		text << "\t\t\t\t\t<Width>" << nextSignal->GetWidth () << "</Width>\n";
		text << "\t\t\t\t\t<Time>" << nextSignal->GetMean () << "</Time>\n";
		text << "\t\t\t\t\t<Fit>" << nextSignal->GetCurveFit () << "</Fit>\n";

		if (nextSignal->IsOffGrid ())
			text << "\t\t\t\t\t<OffLadder>true</OffLadder>\n";

		else if (nextSignal->IsAcceptedOffGrid ())
			text << "\t\t\t\t\t<OffLadder>accepted</OffLadder>\n";

		else
			text << "\t\t\t\t\t<OffLadder>false</OffLadder>\n";

		if (isHomozygote) {

			text << "\t\t\t\t\t<Count>2</Count>\n";
		}

		text << "\t\t\t\t</Allele>\n";
	}

	text.ResetOutputLevel ();
	text << CLevel (1) << "\t\t\t</Locus>\n" << PLevel ();
}



int Locus :: AnalyzeGridLocusSM (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	PrecomputeAverages (LocusSignalList);

	RGDListIterator CurveIterator (LocusSignalList);
	FinalSignalList.Clear ();
	DataSignal* FollowingSignal;
	DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
	int TestResult;
	int Size = mLink->GetLocusVectorSize ();
	msg.WriteEmptyLine ();
	msg.WriteLocusName (mLink->GetLocusName (), Size);
	RGString eString;
	int status = 0;

//	Notice* newNotice;
//	Notice* foundNotice;
	RGString dataForMsg;
	smHeightBelowFractionalFilter fractionalFilterNotice;
	smLocusTooFewPeaks locusTooFewPeaks;
	smRelativeHeightImbalance relativeHeightImbalance;
	smUnexpectedLadderLocusPeak extraneousLadderLocusPeak;
	smPoorLocusMorphology poorLocusMorphology;
	smSignalIsCtrlPeak signalIsCtrlPeak;
	smUnexpectedNoOfGridPeaks unexpectedNumberOfPeaks;
	smCrater crater;
	smInterlocusLadderPeak interlocusPeak;

	RGDList overFlow;
	RGDListIterator overflowIterator (overFlow);
	RGString info;
	int recursiveStatus;

	RGDList toTheLeftList;
	RGDList toTheRightList;
	RGDList tempSignalList;
	RGDListIterator tempIterator (tempSignalList);

	double MaxPeak = 0.0;
	double peak;
	CurveIterator.Reset ();

	bool isAMEL = mIsAMEL;

	/*if (mLink->GetLocusName () == "AMEL")
		isAMEL = true;*/

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		peak = nextSignal->Peak ();

		if (peak > MaxPeak)
			MaxPeak = peak;
	}

	double fractionalThreshold = 0.0;
//	double fractionalFilter = Locus::GetGridFractionalFilter ();
	double fractionalFilter = GetLocusSpecificLadderFractionalFilter ();
	
	if (fractionalFilter > 0.0) {
	
		fractionalThreshold = fractionalFilter * MaxPeak;
	}

	CurveIterator.Reset ();

	while (nextSignal = (DataSignal*)LocusSignalList.GetFirst ()) {

		// Test for out of bounds on various quantities...
		FollowingSignal = (DataSignal*)LocusSignalList.First ();

		if ((fractionalFilter > 0.0) && (nextSignal->Peak () <= fractionalThreshold)) {

//			artifactList.InsertWithNoReferenceDuplication (nextSignal);
			nextSignal->SetMessageValue (fractionalFilterNotice, true);
		}

		else {

			TestResult = TestNeighborsSM (PreviousSignal, nextSignal, FollowingSignal);

			//if (TestResult <= 0)
			//	artifactList.InsertWithNoReferenceDuplication (nextSignal);

			//else
			if (TestResult > 0)
				FinalSignalList.Append (nextSignal);
		}

		PreviousSignal = (DataSignal*) FinalSignalList.Last ();
	}

	RGDListIterator FinalIterator (FinalSignalList);

	double correlation = 0.0;
	int Displacement = 0;  // = mLink->SelectBestSubsetOfCharacteristics (FinalSignalList, correlation);
	FinalIterator.Reset ();
	DataSignal* prevSignal;
	double bp;
	int ibp;

	if ((Size > 0) && (FinalSignalList.Entries () < Size)) {

		int nCurves = FinalSignalList.Entries ();
		eString << nCurves << " available out of " << Size << " required for Locus..." << "\n";
		eString << "LOCUS NAMED " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS\n";
		ExcelText.Write (OutputLevelManager::LocusGridQuality, eString);

		SetMessageValue (locusTooFewPeaks, true);
		status = -1;
	}

	else if ((Size > 0) && (FinalSignalList.Entries () == Size)) {

		// Just compute the correlation and call it a day!

		double* signalArray = new double [Size];
		int iii = 0;
		prevSignal = (DataSignal*) FinalIterator ();

		while (nextSignal = (DataSignal*) FinalIterator ()) {

			signalArray [iii] = nextSignal->GetMean () - prevSignal->GetMean ();
			prevSignal = nextSignal;
			iii++;
		}

		const double* idealNormalizedDifferences = mLink->GetDifferenceVector ();
		correlation = PopulationMarkerSet::VectorDotProductWithDifferences (signalArray, idealNormalizedDifferences, Size-1);
		delete[] signalArray;
	}

	else {

		if (FinalSignalList.Entries () >= Size) {

			overFlow.Clear ();
			const double* differenceArray = mLink->GetUnnormalizedDifferenceVector ();
			double extremePoints [3];
			const double* actualArray = mLink->GetLocusVector ();
			extremePoints [0] = actualArray [0];
			extremePoints [2] = actualArray [Size - 1];
			extremePoints [1] = actualArray [Size - 2];
			const double* leftNorm2s = mLink->GetNormsLeft ();
			const double* rightNorm2s = mLink->GetNormsRight ();
			const int* hts = mLink->GetRelativeHeights ();
			IdealControlSetInfo ctlInfo (actualArray, differenceArray, leftNorm2s, rightNorm2s, hts, Size, true);
			int startLimit = (Size / 2) - 1;
			int startPts;

			startPts = Size;
			int maxDiscrepancy;
			int disc;

			if (hts != NULL)
				maxDiscrepancy = 4;

			else
				maxDiscrepancy = 2;

			for (disc=2; disc<=maxDiscrepancy; disc++) {

				FinalIterator.Reset ();
				tempSignalList.Clear ();
				toTheLeftList.Clear ();
				toTheRightList.Clear ();
				overFlow.Clear ();
				ctlInfo.mDiscrepancy = disc;

				while (nextSignal = (DataSignal*) FinalIterator ())
					tempSignalList.Append (nextSignal);

				if (hts != NULL) {

					tempIterator.Reset ();
					double testPeak;

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempIterator ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, 0, testPeak, MaxPeak, disc)) {

							tempIterator.RemoveCurrentItem ();
							toTheLeftList.Append (nextSignal);
						}

						else
							break;
					}

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempSignalList.GetLast ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, Size - 1, testPeak, MaxPeak, disc))
							toTheRightList.Prepend (nextSignal);

						else {

							tempSignalList.Append (nextSignal);
							break;
						}
					}
				}

				recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (tempSignalList, overFlow, ctlInfo, correlation, 0.99, startPts, MaxPeak);

				if ((recursiveStatus < 0) || (correlation < 0.995)) {

					if (disc == maxDiscrepancy) {

						if (print)
							msg.WriteInsufficientPeaksForControlSet (FinalSignalList.Entries ());

						if (status == 0)
							ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";

						SetMessageValue (locusTooFewPeaks, true);
						//info = " with ";  //*******************************************************************************add as begin text to SmartMessage
	//					info << FinalSignalList.Entries () << " peaks out of " << Size;  //**********************************add as separator to SmartMessage
						AppendDataForSmartMessage (locusTooFewPeaks, FinalSignalList.Entries ());
						AppendDataForSmartMessage (locusTooFewPeaks, Size);

						overFlow.Clear ();
						cout << ErrorString << endl;

						//
						// Add msg if hts != NULL about relative sizes*************************
						//

						if (hts != NULL) {

							SetMessageValue (relativeHeightImbalance, true);
						}

						status = -1;
					}
				}

				else {

					overflowIterator.Reset ();
					double firstTime;
					double lastTime;
					double nextMean;
					tempIterator.Reset ();
					FinalSignalList.Clear ();

					while (nextSignal = (DataSignal*) tempSignalList.GetFirst ())
						FinalSignalList.Append (nextSignal);

					nextSignal = (DataSignal*) FinalSignalList.First ();
					firstTime = nextSignal->GetMean ();
					nextSignal = (DataSignal*) FinalSignalList.Last ();
					lastTime = nextSignal->GetMean ();

					//
					// if hts != NULL and disc > 2, add message about failure of relative height criteria ********************
					//

					if ((hts != NULL) && (disc > 2)) {

						SetMessageValue (relativeHeightImbalance, true);
					}
					
					while (nextSignal = (DataSignal*) overFlow.GetFirst ()) {

						nextMean = nextSignal->GetMean ();

						if (nextMean < firstTime)
							toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);

						else if (nextMean > lastTime)
							toTheRightList.InsertWithNoReferenceDuplication (nextSignal);

						else {

							nextSignal->SetMessageValue (extraneousLadderLocusPeak, true);
							nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
								"Peak out of place in final list:  uncategorized artifact");
							bp = nextSignal->GetApproximateBioID ();
							ibp = (int) floor (bp + 0.5);
							SetMessageValue (unexpectedNumberOfPeaks, true);
							AppendDataForSmartMessage (unexpectedNumberOfPeaks, ibp);
						}
					}

					break;
				}
			}
		}

		Displacement = 0;
	}

	if (correlation < 0.995) {

		if (status == 0)
			ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...Correlation, " << correlation << ", is too low.\n";

		SetMessageValue (poorLocusMorphology, true);

//		info = " with correlation = ";  //***********************************fix begin text and end text********************************
//		info << correlation << " (0.995 - 1.0)";
		AppendDataForSmartMessage (poorLocusMorphology, correlation);
		overFlow.Clear ();
		cout << ErrorString << endl;

		status = -1;
	}

	int CurrentIndex = 0;
	int TotalCandidates = FinalSignalList.Entries ();

	while (CurrentIndex < Displacement) {

		nextSignal = (DataSignal*)FinalSignalList.GetFirst ();
		nextSignal->AddNoticeToList (OutputLevelManager::PeakBelowLocus, "", "Peak represents measurement at base pair level below locus");
		nextSignal->SetMessageValue (interlocusPeak, true);
		toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
		CurrentIndex++;
	}

	while (FinalSignalList.Entries () > Size) {

		nextSignal = (DataSignal*)FinalSignalList.GetLast ();
		nextSignal->AddNoticeToList (OutputLevelManager::PeakAboveLocus, "", "Peak represents measurement at base pair level above locus");
		nextSignal->SetMessageValue (interlocusPeak, true);
		toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
	}

	if (toTheLeftList.Entries () > 0) {

		MergeListAIntoListB (toTheLeftList, originalList);
	}

	if (toTheRightList.Entries () > 0) {

		MergeListAIntoListB (toTheRightList, originalList);
	}

	CurveIterator.Reset ();
	FinalIterator.Reset ();
	int i = 0;
	int NumberOfAcceptedCurves = FinalSignalList.Entries ();

	if (NumberOfAcceptedCurves == 0) {

		ErrorString << "LOCUS " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS...There are no peaks within expected parameters.\n";

		SetMessageValue (locusTooFewPeaks, true);
		//info = " with ";  //*******************************************************************************add as begin text to SmartMessage
		//info << FinalSignalList.Entries () << " peaks out of " << Size;  //**********************************add as separator to SmartMessage
		AppendDataForSmartMessage (locusTooFewPeaks, FinalSignalList.Entries ());
		AppendDataForSmartMessage (locusTooFewPeaks, Size);

		while (nextSignal = (DataSignal*) CurveIterator ())
			nextSignal->ReportNotices (ExcelText, "\t\t", "\t");

		return -1;
	}

	double* CurveNumbers = new double [NumberOfAcceptedCurves];
	double* Fits = new double [NumberOfAcceptedCurves];
	double* Peaks = new double [NumberOfAcceptedCurves];
	double* Sigmas = new double [NumberOfAcceptedCurves];
	double* SecondaryContent = new double [NumberOfAcceptedCurves];
	double* Means = new double [NumberOfAcceptedCurves];
	double TwoMass;
	double OneMass;
	//CompoundSignalInfo* csi;
	//CraterInducedByPullUp pullupNotice;
	//PullUpPrimaryChannel primaryPullupNotice;
	//PullUpFound pullup;

	while (nextSignal = (DataSignal*)FinalIterator ()) {

		CurveNumbers [i] = i + 1;
		Fits [i] = nextSignal->GetCurveFit ();
		Peaks [i] = nextSignal->Peak ();

		Sigmas [i] = nextSignal->GetStandardDeviation ();
		Means [i] = nextSignal->GetMean ();
		TwoMass = nextSignal->GetScale (2);
		OneMass = nextSignal->GetScale (1);
		SecondaryContent [i] = TwoMass / (TwoMass + OneMass);

		if (print)
			nextSignal->Report (text, "    ");

		i++;

		if (status == 0) {

			nextSignal->SetMessageValue (signalIsCtrlPeak, true);
			nextSignal->SetMessageValue (crater, false);
			nextSignal->SetDontLook (false);

			//csi = nextSignal->RemoveSignalLink ();
			nextSignal->RemoveAllCrossChannelSignalLinksSM ();

			//if (csi != NULL) {

			//	nextSignal->RemoveCrossChannelSignalLink (&primaryPullupNotice, &pullupNotice, NULL, NULL);
			//}

			//nextSignal->RemoveCrossChannelSignalLink (&primaryPullupNotice, &pullup, NULL, NULL);
		}
	}

	if (print) {

		msg.WriteEmptyLine ();
		msg.WriteMaximumCorrelationWithControlSet (0, correlation, Size, TotalCandidates);  // !!!!!
		msg.StartLine (0, "Curve Nos.", TRUE);
		msg.StartLine (1, "Fits", TRUE);
		msg.StartLine (2, "2AryContent", TRUE);
		msg.StartLine (3, "Means", TRUE);
		msg.StartLine (4, "Sigmas", TRUE);
		msg.StartLine (5, "Peaks", TRUE);

		for (int j=0; j<NumberOfAcceptedCurves; j++) {

			msg.AddToLine (0, CurveNumbers [j]);
			msg.AddToLine (1, Fits [j]);
			msg.AddToLine (2, SecondaryContent [j]);
			msg.AddToLine (3, Means [j]);
			msg.AddToLine (4, Sigmas [j]);
			msg.AddToLine (5, Peaks [j]);
		}

		msg.FlushLines (6);
	}

	delete[] CurveNumbers;
	delete[] Fits;
	delete[] SecondaryContent;
	delete[] Sigmas;
	delete[] Peaks;
	delete[] Means;

	if (InsertAlleleSignals (FinalSignalList) < 0) {

		ExcelText.Write (OutputLevelManager::LocusGridQuality, ErrorString);
		return -1;
	}

	if (status == 0)
		mIsOK = true;

	return status;
}


int Locus :: AnalyzeGridLocusAndApplyThresholdsSM (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is ladder stage 2...no longer used (2/22/14)
	//

	PrecomputeAverages (LocusSignalList);

	RGDListIterator CurveIterator (LocusSignalList);
	FinalSignalList.Clear ();
	//DataSignal* FollowingSignal;
	//DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
	int TestResult;
	int Size = mLink->GetLocusVectorSize ();
	msg.WriteEmptyLine ();
	msg.WriteLocusName (mLink->GetLocusName (), Size);
	RGString eString;
	int status = 0;

	RGString dataForMsg;
	smHeightBelowFractionalFilter fractionalFilterNotice;
	smLocusTooFewPeaks locusTooFewPeaks;
	smRelativeHeightImbalance relativeHeightImbalance;
	smUnexpectedLadderLocusPeak extraneousLadderLocusPeak;
	smPoorLocusMorphology poorLocusMorphology;
	smSignalIsCtrlPeak signalIsCtrlPeak;
	smUnexpectedNoOfGridPeaks unexpectedNumberOfPeaks;
	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smInterlocusLadderPeak interlocusPeak;
	smPullUp pullup;
	smPrimaryInterchannelLink primaryPullup;
	smHeightBelowPullupFractionalFilter pullupFractionalFilterNotice;

	RGDList overFlow;
	RGDListIterator overflowIterator (overFlow);
	RGString info;
	int recursiveStatus;

	RGDList toTheLeftList;
	RGDList toTheRightList;
	RGDList tempSignalList;
	RGDListIterator tempIterator (tempSignalList);

	double MaxPeak = 0.0;
	double peak;
	CurveIterator.Reset ();

	bool isAMEL = mIsAMEL;

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		peak = nextSignal->Peak ();

		if (peak > MaxPeak)
			MaxPeak = peak;
	}

	mMaxPeak = MaxPeak;

	double fractionalThreshold = 0.0;
	double pullupFractionalThreshold = 0.0;
	double fractionalFilter = GetLocusSpecificLadderFractionalFilter ();
	double pullupFractionalFilter = GetLocusSpecificLadderPullupFractionalFilter ();
	
	if (fractionalFilter > 0.0)	
		fractionalThreshold = fractionalFilter * MaxPeak;

	if (pullupFractionalFilter > 0.0)
		pullupFractionalThreshold = pullupFractionalFilter * MaxPeak;

	CurveIterator.Reset ();

	// Skip testing fractional filters here and do it later.  For now, keep the old test of stutter and adenylation, even
	// though both are flawed.  Future:  consider fixing them (this function and prototype [AnalyzeGridLocusSM] are the only functions that 
	// call TestNeighborsSM).  Add better test for stutter and adenylation, taking into account
	// multiple peaks, and perform fractional filter tests, after selection of peaks!

	while (nextSignal = (DataSignal*)LocusSignalList.GetFirst ()) {

		TestResult = TestSimpleNeighborsForGridSM (FinalSignalList, nextSignal, LocusSignalList);

		if (TestResult > 0)
			FinalSignalList.Append (nextSignal);
	}

	RGDListIterator FinalIterator (FinalSignalList);

	double correlation = 0.0;
	int Displacement = 0;  // = mLink->SelectBestSubsetOfCharacteristics (FinalSignalList, correlation);
	FinalIterator.Reset ();
	DataSignal* prevSignal;
	double bp;
	int ibp;
	int numberNoncritical = 0;
	RGDList stillCriticalList;

	if ((Size > 0) && (FinalSignalList.Entries () < Size)) {

		int nCurves = FinalSignalList.Entries ();
		eString << nCurves << " available out of " << Size << " required for Locus..." << "\n";
		eString << "LOCUS NAMED " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS\n";
		ExcelText.Write (OutputLevelManager::LocusGridQuality, eString);

		SetMessageValue (locusTooFewPeaks, true);
		status = -1;
	}

	else if ((Size > 0) && (FinalSignalList.Entries () == Size)) {

		// Just compute the correlation and call it a day!

		double* signalArray = new double [Size];
		int iii = 0;
		prevSignal = (DataSignal*) FinalIterator ();

		while (nextSignal = (DataSignal*) FinalIterator ()) {

			signalArray [iii] = nextSignal->GetMean () - prevSignal->GetMean ();
			prevSignal = nextSignal;
			iii++;
		}

		const double* idealNormalizedDifferences = mLink->GetDifferenceVector ();
		correlation = PopulationMarkerSet::VectorDotProductWithDifferences (signalArray, idealNormalizedDifferences, Size-1);
		delete[] signalArray;
	}

	else {

		if (FinalSignalList.Entries () >= Size) {

			overFlow.Clear ();
			const double* differenceArray = mLink->GetUnnormalizedDifferenceVector ();
			double extremePoints [3];
			const double* actualArray = mLink->GetLocusVector ();
			extremePoints [0] = actualArray [0];
			extremePoints [2] = actualArray [Size - 1];
			extremePoints [1] = actualArray [Size - 2];
			const double* leftNorm2s = mLink->GetNormsLeft ();
			const double* rightNorm2s = mLink->GetNormsRight ();
			const int* hts = mLink->GetRelativeHeights ();
			IdealControlSetInfo ctlInfo (actualArray, differenceArray, leftNorm2s, rightNorm2s, hts, Size, true);
			int startLimit = (Size / 2) - 1;
			int startPts;

			startPts = Size;
			int maxDiscrepancy;
			int disc;

			if (hts != NULL)
				maxDiscrepancy = 4;

			else
				maxDiscrepancy = 2;

			for (disc=2; disc<=maxDiscrepancy; disc++) {

				FinalIterator.Reset ();
				tempSignalList.Clear ();
				toTheLeftList.Clear ();
				toTheRightList.Clear ();
				overFlow.Clear ();
				ctlInfo.mDiscrepancy = disc;

				while (nextSignal = (DataSignal*) FinalIterator ())
					tempSignalList.Append (nextSignal);

				if (hts != NULL) {

					tempIterator.Reset ();
					double testPeak;

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempIterator ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, 0, testPeak, MaxPeak, disc)) {

							tempIterator.RemoveCurrentItem ();
							toTheLeftList.Append (nextSignal);
						}

						else
							break;
					}

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempSignalList.GetLast ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, Size - 1, testPeak, MaxPeak, disc))
							toTheRightList.Prepend (nextSignal);

						else {

							tempSignalList.Append (nextSignal);
							break;
						}
					}
				}

				recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (tempSignalList, overFlow, ctlInfo, correlation, 0.99, startPts, MaxPeak);

				if ((recursiveStatus < 0) || (correlation < 0.995)) {

					if (disc == maxDiscrepancy) {

						if (print)
							msg.WriteInsufficientPeaksForControlSet (FinalSignalList.Entries ());

						if (status == 0)
							ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";

						SetMessageValue (locusTooFewPeaks, true);
						//info = " with ";  //*******************************************************************************add as begin text to SmartMessage
	//					info << FinalSignalList.Entries () << " peaks out of " << Size;  //**********************************add as separator to SmartMessage
						AppendDataForSmartMessage (locusTooFewPeaks, FinalSignalList.Entries ());
						AppendDataForSmartMessage (locusTooFewPeaks, Size);

						overFlow.Clear ();
						cout << ErrorString << endl;

						//
						// Add msg if hts != NULL about relative sizes*************************
						//

						if (hts != NULL) {

							SetMessageValue (relativeHeightImbalance, true);
						}

						status = -1;
					}
				}

				else {

					overflowIterator.Reset ();
					double firstTime;
					double lastTime;
					double nextMean;
					tempIterator.Reset ();
					FinalSignalList.Clear ();

					while (nextSignal = (DataSignal*) tempSignalList.GetFirst ())
						FinalSignalList.Append (nextSignal);

					nextSignal = (DataSignal*) FinalSignalList.First ();
					firstTime = nextSignal->GetMean ();
					nextSignal = (DataSignal*) FinalSignalList.Last ();
					lastTime = nextSignal->GetMean ();

					//
					// if hts != NULL and disc > 2, add message about failure of relative height criteria ********************
					//

					if ((hts != NULL) && (disc > 2)) {

						SetMessageValue (relativeHeightImbalance, true);
					}
					
					while (nextSignal = (DataSignal*) overFlow.GetFirst ()) {

						nextMean = nextSignal->GetMean ();

						if (nextMean < firstTime) {

							TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

							if (TestResult > 0)
								toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
						}

						else if (nextMean > lastTime) {

							TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

							if (TestResult > 0)
								toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
						}

						else {

							//nextSignal->SetMessageValue (extraneousLadderLocusPeak, true);
							//nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
							//	"Peak out of place in final list:  uncategorized artifact");
							//bp = nextSignal->GetApproximateBioID ();
							//ibp = (int) floor (bp + 0.5);
							//SetMessageValue (unexpectedNumberOfPeaks, true);
							//AppendDataForSmartMessage (unexpectedNumberOfPeaks, ibp);
							bool test1 = (nextSignal->Peak () <= fractionalThreshold);
							bool test2 = (nextSignal->GetMessageValue (pullup) && !nextSignal->GetMessageValue (primaryPullup) && (nextSignal->Peak () <= pullupFractionalThreshold));
							bool test3 = false;

							// any are true if signal is non-critical artifact; so, all are false if signal is critical because out of place

							TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

							if (TestResult <= 0)
								test3 = true;

							if (test1)
								nextSignal->SetMessageValue (fractionalFilterNotice, true);

							if (test2)
								nextSignal->SetMessageValue (pullupFractionalFilterNotice, true);

							if (test1 || test2 || test3)
								numberNoncritical++;

							else
								stillCriticalList.Append (nextSignal);

							// Add in test for "complex" stutter and adenylation here.
						}
					}

					if (stillCriticalList.Entries () > 0) {

						SetMessageValue (unexpectedNumberOfPeaks, true);

						while (nextSignal = (DataSignal*) stillCriticalList.GetFirst ()) {

							nextSignal->SetMessageValue (extraneousLadderLocusPeak, true);
							nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
								"Peak out of place in final list:  uncategorized artifact");
							bp = nextSignal->GetApproximateBioID ();
							ibp = (int) floor (bp + 0.5);
							AppendDataForSmartMessage (unexpectedNumberOfPeaks, ibp);
						}

						stillCriticalList.Clear ();
					}

					break;
				}
			}
		}

		Displacement = 0;
	}

	if (correlation < 0.995) {

		if (status == 0)
			ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...Correlation, " << correlation << ", is too low.\n";

		SetMessageValue (poorLocusMorphology, true);

//		info = " with correlation = ";  //***********************************fix begin text and end text********************************
//		info << correlation << " (0.995 - 1.0)";
		AppendDataForSmartMessage (poorLocusMorphology, correlation);
		overFlow.Clear ();
		cout << ErrorString << endl;

		status = -1;
	}

	int CurrentIndex = 0;
	int TotalCandidates = FinalSignalList.Entries ();

	while (CurrentIndex < Displacement) {

		nextSignal = (DataSignal*)FinalSignalList.GetFirst ();
		TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		if (TestResult > 0) {

			nextSignal->AddNoticeToList (OutputLevelManager::PeakBelowLocus, "", "Peak represents measurement at base pair level below locus");
			nextSignal->SetMessageValue (interlocusPeak, true);
		}

		toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
		CurrentIndex++;
	}

	while (FinalSignalList.Entries () > Size) {

		nextSignal = (DataSignal*)FinalSignalList.GetLast ();
		TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		if (TestResult > 0) {

			nextSignal->AddNoticeToList (OutputLevelManager::PeakAboveLocus, "", "Peak represents measurement at base pair level above locus");
			nextSignal->SetMessageValue (interlocusPeak, true);
		}

		toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
	}

	if (toTheLeftList.Entries () > 0) {

		MergeListAIntoListB (toTheLeftList, originalList);
	}

	if (toTheRightList.Entries () > 0) {

		MergeListAIntoListB (toTheRightList, originalList);
	}

	CurveIterator.Reset ();
	FinalIterator.Reset ();
	int i = 0;
	int NumberOfAcceptedCurves = FinalSignalList.Entries ();

	if (NumberOfAcceptedCurves == 0) {

		ErrorString << "LOCUS " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS...There are no peaks within expected parameters.\n";

		SetMessageValue (locusTooFewPeaks, true);
		//info = " with ";  //*******************************************************************************add as begin text to SmartMessage
		//info << FinalSignalList.Entries () << " peaks out of " << Size;  //**********************************add as separator to SmartMessage
		AppendDataForSmartMessage (locusTooFewPeaks, FinalSignalList.Entries ());
		AppendDataForSmartMessage (locusTooFewPeaks, Size);

		while (nextSignal = (DataSignal*) CurveIterator ())
			nextSignal->ReportNotices (ExcelText, "\t\t", "\t");

		return -1;
	}

	double* CurveNumbers = new double [NumberOfAcceptedCurves];
	double* Fits = new double [NumberOfAcceptedCurves];
	double* Peaks = new double [NumberOfAcceptedCurves];
	double* Sigmas = new double [NumberOfAcceptedCurves];
	double* SecondaryContent = new double [NumberOfAcceptedCurves];
	double* Means = new double [NumberOfAcceptedCurves];
	double TwoMass;
	double OneMass;
	//CompoundSignalInfo* csi;
	//CraterInducedByPullUp pullupNotice;
	//PullUpPrimaryChannel primaryPullupNotice;
	//PullUpFound pullup;

	while (nextSignal = (DataSignal*)FinalIterator ()) {

		CurveNumbers [i] = i + 1;
		Fits [i] = nextSignal->GetCurveFit ();
		Peaks [i] = nextSignal->Peak ();

		Sigmas [i] = nextSignal->GetStandardDeviation ();
		Means [i] = nextSignal->GetMean ();
		TwoMass = nextSignal->GetScale (2);
		OneMass = nextSignal->GetScale (1);
		SecondaryContent [i] = TwoMass / (TwoMass + OneMass);

		if (print)
			nextSignal->Report (text, "    ");

		i++;

		if (status == 0) {

			nextSignal->SetMessageValue (signalIsCtrlPeak, true);
			nextSignal->SetMessageValue (crater, false);
			nextSignal->SetMessageValue (craterSidePeak, false);
			nextSignal->SetDontLook (false);
			nextSignal->RemoveAllCrossChannelSignalLinksSM ();
		}
	}

	if (print) {

		msg.WriteEmptyLine ();
		msg.WriteMaximumCorrelationWithControlSet (0, correlation, Size, TotalCandidates);  // !!!!!
		msg.StartLine (0, "Curve Nos.", TRUE);
		msg.StartLine (1, "Fits", TRUE);
		msg.StartLine (2, "2AryContent", TRUE);
		msg.StartLine (3, "Means", TRUE);
		msg.StartLine (4, "Sigmas", TRUE);
		msg.StartLine (5, "Peaks", TRUE);

		for (int j=0; j<NumberOfAcceptedCurves; j++) {

			msg.AddToLine (0, CurveNumbers [j]);
			msg.AddToLine (1, Fits [j]);
			msg.AddToLine (2, SecondaryContent [j]);
			msg.AddToLine (3, Means [j]);
			msg.AddToLine (4, Sigmas [j]);
			msg.AddToLine (5, Peaks [j]);
		}

		msg.FlushLines (6);
	}

	delete[] CurveNumbers;
	delete[] Fits;
	delete[] SecondaryContent;
	delete[] Sigmas;
	delete[] Peaks;
	delete[] Means;

	if (InsertAlleleSignals (FinalSignalList) < 0) {

		ExcelText.Write (OutputLevelManager::LocusGridQuality, ErrorString);
		return -1;
	}

	if (status == 0)
		mIsOK = true;

	return status;
}


int Locus :: AnalyzeGridLocusAndAllowForOverlapSM (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is ladder stage 2...obsolete as of 02/26/2014
	//

	PrecomputeAverages (LocusSignalList);

	RGDListIterator CurveIterator (LocusSignalList);
	FinalSignalList.Clear ();
	//DataSignal* FollowingSignal;
	//DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
	smBelowMinRFU belowMinRFU;
	smSignalIsCtrlPeak signalIsCtrlPeak;

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		if (nextSignal->GetMessageValue (belowMinRFU))
			CurveIterator.RemoveCurrentItem ();

		else if (nextSignal->GetMessageValue (signalIsCtrlPeak))
			CurveIterator.RemoveCurrentItem ();
	}

	CurveIterator.Reset ();
	int TestResult;
	int Size = mLink->GetLocusVectorSize ();
	msg.WriteEmptyLine ();
	msg.WriteLocusName (mLink->GetLocusName (), Size);
	RGString eString;
	int status = 0;

	RGString dataForMsg;
	smHeightBelowFractionalFilter fractionalFilterNotice;
	smLocusTooFewPeaks locusTooFewPeaks;
	smRelativeHeightImbalance relativeHeightImbalance;
	smUnexpectedLadderLocusPeak extraneousLadderLocusPeak;
	smPoorLocusMorphology poorLocusMorphology;
	
	smUnexpectedNoOfGridPeaks unexpectedNumberOfPeaks;
	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smInterlocusLadderPeak interlocusPeak;
	smPullUp pullup;
	smPrimaryInterchannelLink primaryPullup;
	smHeightBelowPullupFractionalFilter pullupFractionalFilterNotice;

	RGDList overFlow;
	RGDListIterator overflowIterator (overFlow);
	RGString info;
	int recursiveStatus;

	RGDList toTheLeftList;
	RGDList toTheRightList;
	RGDList tempSignalList;
	RGDListIterator tempIterator (tempSignalList);

	double MaxPeak = 0.0;
	double peak;
	CurveIterator.Reset ();

	bool isAMEL = mIsAMEL;

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		peak = nextSignal->Peak ();

		if (peak > MaxPeak)
			MaxPeak = peak;
	}

	mMaxPeak = MaxPeak;

	double fractionalThreshold = 0.0;
	double pullupFractionalThreshold = 0.0;
	double fractionalFilter = GetLocusSpecificLadderFractionalFilter ();
	double pullupFractionalFilter = GetLocusSpecificLadderPullupFractionalFilter ();
	
	if (fractionalFilter > 0.0)	
		fractionalThreshold = fractionalFilter * MaxPeak;

	if (pullupFractionalFilter > 0.0)
		pullupFractionalThreshold = pullupFractionalFilter * MaxPeak;

	CurveIterator.Reset ();

	// Skip testing fractional filters here and do it later.  For now, keep the old test of stutter and adenylation, even
	// though both are flawed.  Future:  consider fixing them (this function and prototype [AnalyzeGridLocusSM] are the only functions that 
	// call TestNeighborsSM).  Add better test for stutter and adenylation, taking into account
	// multiple peaks, and perform fractional filter tests, after selection of peaks!

	while (nextSignal = (DataSignal*)LocusSignalList.GetFirst ()) {

		TestResult = TestSimpleNeighborsForGridSM (FinalSignalList, nextSignal, LocusSignalList);

		if (TestResult > 0)
			FinalSignalList.Append (nextSignal);
	}

	RGDListIterator FinalIterator (FinalSignalList);

	double correlation = 0.0;
	int Displacement = 0;  // = mLink->SelectBestSubsetOfCharacteristics (FinalSignalList, correlation);
	FinalIterator.Reset ();
	DataSignal* prevSignal;
	double bp;
	int ibp;
	int numberNoncritical = 0;
	RGDList stillCriticalList;

	if ((Size > 0) && (FinalSignalList.Entries () < Size)) {

		int nCurves = FinalSignalList.Entries ();
		eString << nCurves << " available out of " << Size << " required for Locus..." << "\n";
		eString << "LOCUS NAMED " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS\n";
		ExcelText.Write (OutputLevelManager::LocusGridQuality, eString);

		SetMessageValue (locusTooFewPeaks, true);
		status = -1;
	}

	else if ((Size > 0) && (FinalSignalList.Entries () == Size)) {

		// Just compute the correlation and call it a day!

		double* signalArray = new double [Size];
		int iii = 0;
		prevSignal = (DataSignal*) FinalIterator ();

		while (nextSignal = (DataSignal*) FinalIterator ()) {

			signalArray [iii] = nextSignal->GetMean () - prevSignal->GetMean ();
			prevSignal = nextSignal;
			iii++;
		}

		const double* idealNormalizedDifferences = mLink->GetDifferenceVector ();
		correlation = PopulationMarkerSet::VectorDotProductWithDifferences (signalArray, idealNormalizedDifferences, Size-1);
		delete[] signalArray;
	}

	else {

		if (FinalSignalList.Entries () >= Size) {

			overFlow.Clear ();
			const double* differenceArray = mLink->GetUnnormalizedDifferenceVector ();
			double extremePoints [3];
			const double* actualArray = mLink->GetLocusVector ();
			extremePoints [0] = actualArray [0];
			extremePoints [2] = actualArray [Size - 1];
			extremePoints [1] = actualArray [Size - 2];
			const double* leftNorm2s = mLink->GetNormsLeft ();
			const double* rightNorm2s = mLink->GetNormsRight ();
			const int* hts = mLink->GetRelativeHeights ();
			IdealControlSetInfo ctlInfo (actualArray, differenceArray, leftNorm2s, rightNorm2s, hts, Size, true);
			int startLimit = (Size / 2) - 1;
			int startPts;

			startPts = Size;
			int maxDiscrepancy;
			int disc;

			if (hts != NULL)
				maxDiscrepancy = 4;

			else
				maxDiscrepancy = 2;

			for (disc=2; disc<=maxDiscrepancy; disc++) {

				FinalIterator.Reset ();
				tempSignalList.Clear ();
				toTheLeftList.Clear ();
				toTheRightList.Clear ();
				overFlow.Clear ();
				ctlInfo.mDiscrepancy = disc;

				while (nextSignal = (DataSignal*) FinalIterator ())
					tempSignalList.Append (nextSignal);

				if (hts != NULL) {

					tempIterator.Reset ();
					double testPeak;

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempIterator ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, 0, testPeak, MaxPeak, disc)) {

							tempIterator.RemoveCurrentItem ();
							toTheLeftList.Append (nextSignal);
						}

						else
							break;
					}

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempSignalList.GetLast ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, Size - 1, testPeak, MaxPeak, disc))
							toTheRightList.Prepend (nextSignal);

						else {

							tempSignalList.Append (nextSignal);
							break;
						}
					}
				}

				recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestSubset (tempSignalList, overFlow, ctlInfo, correlation, 0.99, startPts, MaxPeak);

				if ((recursiveStatus < 0) || (correlation < 0.995)) {

					if (disc == maxDiscrepancy) {

						if (print)
							msg.WriteInsufficientPeaksForControlSet (FinalSignalList.Entries ());

						if (status == 0)
							ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";

						SetMessageValue (locusTooFewPeaks, true);
						//info = " with ";  //*******************************************************************************add as begin text to SmartMessage
	//					info << FinalSignalList.Entries () << " peaks out of " << Size;  //**********************************add as separator to SmartMessage
						AppendDataForSmartMessage (locusTooFewPeaks, FinalSignalList.Entries ());
						AppendDataForSmartMessage (locusTooFewPeaks, Size);

						overFlow.Clear ();
						cout << ErrorString << endl;

						//
						// Add msg if hts != NULL about relative sizes*************************
						//

						if (hts != NULL) {

							SetMessageValue (relativeHeightImbalance, true);
						}

						status = -1;
					}
				}

				else {

					overflowIterator.Reset ();
					double firstTime;
					double lastTime;
					double nextMean;
					tempIterator.Reset ();
					FinalSignalList.Clear ();

					while (nextSignal = (DataSignal*) tempSignalList.GetFirst ())
						FinalSignalList.Append (nextSignal);

					nextSignal = (DataSignal*) FinalSignalList.First ();
					firstTime = nextSignal->GetMean ();
					nextSignal = (DataSignal*) FinalSignalList.Last ();
					lastTime = nextSignal->GetMean ();

					//
					// if hts != NULL and disc > 2, add message about failure of relative height criteria ********************
					//

					if ((hts != NULL) && (disc > 2)) {

						SetMessageValue (relativeHeightImbalance, true);
					}
					
					while (nextSignal = (DataSignal*) overFlow.GetFirst ()) {

						nextMean = nextSignal->GetMean ();

						if (nextMean < firstTime) {

							// this peak is left of core ladder locus, so, with overlap, might belong to another locus

		//					TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		//					if (TestResult > 0)
								toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
						}

						else if (nextMean > lastTime) {

							// this peak is right of core ladder locus, so, with overlap, might belong to another locus

		//					TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		//					if (TestResult > 0)
								toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
						}

						else {

							// this peak is within core ladder locus, so test for non-critical artifact
							bool test1 = (nextSignal->Peak () <= fractionalThreshold);
							bool test2 = (nextSignal->GetMessageValue (pullup) && !nextSignal->GetMessageValue (primaryPullup) && (nextSignal->Peak () <= pullupFractionalThreshold));
							bool test3 = false;

							// any are true if signal is non-critical artifact; so, all are false if signal is critical because out of place

							TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

							if (TestResult <= 0)
								test3 = true;

							if (test1)
								nextSignal->SetMessageValue (fractionalFilterNotice, true);

							if (test2)
								nextSignal->SetMessageValue (pullupFractionalFilterNotice, true);

							if (test1 || test2 || test3)
								numberNoncritical++;

							else
								stillCriticalList.Append (nextSignal);

							// Add in test for "complex" stutter and adenylation here.
						}
					}

					if (stillCriticalList.Entries () > 0) {

						SetMessageValue (unexpectedNumberOfPeaks, true);

						while (nextSignal = (DataSignal*) stillCriticalList.GetFirst ()) {

							nextSignal->SetMessageValue (extraneousLadderLocusPeak, true);
							nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
								"Peak out of place in final list:  uncategorized artifact");
							bp = nextSignal->GetApproximateBioID ();
							ibp = (int) floor (bp + 0.5);
							AppendDataForSmartMessage (unexpectedNumberOfPeaks, ibp);
							originalList.RemoveReference (nextSignal);

							// should we remove from originalList???????????
						}

						stillCriticalList.Clear ();
					}

					break;
				}
			}
		}

		Displacement = 0;
	}

	if (correlation < 0.995) {

		if (status == 0)
			ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...Correlation, " << correlation << ", is too low.\n";

		SetMessageValue (poorLocusMorphology, true);
		AppendDataForSmartMessage (poorLocusMorphology, correlation);
		overFlow.Clear ();
		cout << ErrorString << endl;

		status = -1;
	}

	int CurrentIndex = 0;
	int TotalCandidates = FinalSignalList.Entries ();

	while (CurrentIndex < Displacement) {

		// these signals are left of core ladder locus, but might be in another locus

		nextSignal = (DataSignal*)FinalSignalList.GetFirst ();
		//TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		//if (TestResult > 0) {

		//	nextSignal->AddNoticeToList (OutputLevelManager::PeakBelowLocus, "", "Peak represents measurement at base pair level below locus");
		//	nextSignal->SetMessageValue (interlocusPeak, true);
		//}

		toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
		CurrentIndex++;
	}

	while (FinalSignalList.Entries () > Size) {

		// these signals are right of core ladder locus, but might be in another locus

		nextSignal = (DataSignal*)FinalSignalList.GetLast ();
		//TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		//if (TestResult > 0) {

		//	nextSignal->AddNoticeToList (OutputLevelManager::PeakAboveLocus, "", "Peak represents measurement at base pair level above locus");
		//	nextSignal->SetMessageValue (interlocusPeak, true);
		//}

		toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
	}

	// Put toTheLeftList and toTheRightList into LocusSignalList for safe-keeping until next round, when can test
	
	LocusSignalList.Clear ();

	if (toTheLeftList.Entries () > 0) {

		MergeListAIntoListB (toTheLeftList, LocusSignalList);
	}

	if (toTheRightList.Entries () > 0) {

		MergeListAIntoListB (toTheRightList, LocusSignalList);
	}

	CurveIterator.Reset ();
	FinalIterator.Reset ();
	int i = 0;
	int NumberOfAcceptedCurves = FinalSignalList.Entries ();

	if (NumberOfAcceptedCurves == 0) {

		ErrorString << "LOCUS " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS...There are no peaks within expected parameters.\n";

		SetMessageValue (locusTooFewPeaks, true);
		//info = " with ";  //*******************************************************************************add as begin text to SmartMessage
		//info << FinalSignalList.Entries () << " peaks out of " << Size;  //**********************************add as separator to SmartMessage
		AppendDataForSmartMessage (locusTooFewPeaks, FinalSignalList.Entries ());
		AppendDataForSmartMessage (locusTooFewPeaks, Size);

		while (nextSignal = (DataSignal*) CurveIterator ())
			nextSignal->ReportNotices (ExcelText, "\t\t", "\t");

		return -1;
	}

	double* CurveNumbers = new double [NumberOfAcceptedCurves];
	double* Fits = new double [NumberOfAcceptedCurves];
	double* Peaks = new double [NumberOfAcceptedCurves];
	double* Sigmas = new double [NumberOfAcceptedCurves];
	double* SecondaryContent = new double [NumberOfAcceptedCurves];
	double* Means = new double [NumberOfAcceptedCurves];
	double TwoMass;
	double OneMass;

	while (nextSignal = (DataSignal*)FinalIterator ()) {

		CurveNumbers [i] = i + 1;
		Fits [i] = nextSignal->GetCurveFit ();
		Peaks [i] = nextSignal->Peak ();

		Sigmas [i] = nextSignal->GetStandardDeviation ();
		Means [i] = nextSignal->GetMean ();
		TwoMass = nextSignal->GetScale (2);
		OneMass = nextSignal->GetScale (1);
		SecondaryContent [i] = TwoMass / (TwoMass + OneMass);

		if (print)
			nextSignal->Report (text, "    ");

		i++;

		if (status == 0) {

			nextSignal->SetMessageValue (signalIsCtrlPeak, true);
			nextSignal->SetMessageValue (crater, false);
			nextSignal->SetMessageValue (craterSidePeak, false);
			nextSignal->SetDontLook (false);
			nextSignal->RemoveAllCrossChannelSignalLinksSM ();
			originalList.RemoveReference (nextSignal);
		}
	}

	if (print) {

		msg.WriteEmptyLine ();
		msg.WriteMaximumCorrelationWithControlSet (0, correlation, Size, TotalCandidates);  // !!!!!
		msg.StartLine (0, "Curve Nos.", TRUE);
		msg.StartLine (1, "Fits", TRUE);
		msg.StartLine (2, "2AryContent", TRUE);
		msg.StartLine (3, "Means", TRUE);
		msg.StartLine (4, "Sigmas", TRUE);
		msg.StartLine (5, "Peaks", TRUE);

		for (int j=0; j<NumberOfAcceptedCurves; j++) {

			msg.AddToLine (0, CurveNumbers [j]);
			msg.AddToLine (1, Fits [j]);
			msg.AddToLine (2, SecondaryContent [j]);
			msg.AddToLine (3, Means [j]);
			msg.AddToLine (4, Sigmas [j]);
			msg.AddToLine (5, Peaks [j]);
		}

		msg.FlushLines (6);
	}

	delete[] CurveNumbers;
	delete[] Fits;
	delete[] SecondaryContent;
	delete[] Sigmas;
	delete[] Peaks;
	delete[] Means;

	if (InsertAlleleSignals (FinalSignalList) < 0) {

		ExcelText.Write (OutputLevelManager::LocusGridQuality, ErrorString);
		return -1;
	}

	if (status == 0)
		mIsOK = true;

	return status;
}






int Locus :: AnalyzeGridLocusAndAllowForOverlapUsingBPsSM (RGDList& artifactList, RGDList& originalList, RGDList& supplementalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is ladder stage 2...New as of 02/26/2014
	//

	PrecomputeAverages (LocusSignalList);

	RGDListIterator CurveIterator (LocusSignalList);
	FinalSignalList.Clear ();
	//DataSignal* FollowingSignal;
	//DataSignal* PreviousSignal = NULL;
	DataSignal* nextSignal;
	smBelowMinRFU belowMinRFU;
	smSignalIsCtrlPeak signalIsCtrlPeak;

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		if (nextSignal->GetMessageValue (belowMinRFU))
			CurveIterator.RemoveCurrentItem ();

		else if (nextSignal->GetMessageValue (signalIsCtrlPeak))
			CurveIterator.RemoveCurrentItem ();
	}

	CurveIterator.Reset ();
	int TestResult;
	int Size = mLink->GetLocusVectorSize ();
	msg.WriteEmptyLine ();
	msg.WriteLocusName (mLink->GetLocusName (), Size);
	RGString eString;
	int status = 0;

	RGString dataForMsg;
	smHeightBelowFractionalFilter fractionalFilterNotice;
	smLocusTooFewPeaks locusTooFewPeaks;
	smRelativeHeightImbalance relativeHeightImbalance;
	smUnexpectedLadderLocusPeak extraneousLadderLocusPeak;
	smPoorLocusMorphology poorLocusMorphology;
	
	smUnexpectedNoOfGridPeaks unexpectedNumberOfPeaks;
	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smInterlocusLadderPeak interlocusPeak;
	smPullUp pullup;
	smPrimaryInterchannelLink primaryPullup;
	smHeightBelowPullupFractionalFilter pullupFractionalFilterNotice;

	RGDList overFlow;
	RGDListIterator overflowIterator (overFlow);
	RGString info;
	int recursiveStatus;

	RGDList toTheLeftList;
	RGDList toTheRightList;
	RGDList tempSignalList;
	RGDListIterator tempIterator (tempSignalList);

	double MaxPeak = 0.0;
	double peak;
	CurveIterator.Reset ();

	bool isAMEL = mIsAMEL;

	while (nextSignal = (DataSignal*) CurveIterator ()) {

		peak = nextSignal->Peak ();

		if (peak > MaxPeak)
			MaxPeak = peak;
	}

	mMaxPeak = MaxPeak;

	double fractionalThreshold = 0.0;
	double pullupFractionalThreshold = 0.0;
	double fractionalFilter = GetLocusSpecificLadderFractionalFilter ();
	double pullupFractionalFilter = GetLocusSpecificLadderPullupFractionalFilter ();
	
	if (fractionalFilter > 0.0)	
		fractionalThreshold = fractionalFilter * MaxPeak;

	if (pullupFractionalFilter > 0.0)
		pullupFractionalThreshold = pullupFractionalFilter * MaxPeak;

	CurveIterator.Reset ();

	// Skip testing fractional filters here and do it later.  For now, keep the old test of stutter and adenylation, even
	// though both are flawed.  Future:  consider fixing them (this function and prototype [AnalyzeGridLocusSM] are the only functions that 
	// call TestNeighborsSM).  Add better test for stutter and adenylation, taking into account
	// multiple peaks, and perform fractional filter tests, after selection of peaks!

	while (nextSignal = (DataSignal*)LocusSignalList.GetFirst ()) {

		TestResult = TestSimpleNeighborsForGridSM (FinalSignalList, nextSignal, LocusSignalList);

		if (TestResult > 0)
			FinalSignalList.Append (nextSignal);
	}

	RGDListIterator FinalIterator (FinalSignalList);

	double correlation = 0.0;
	int Displacement = 0;  // = mLink->SelectBestSubsetOfCharacteristics (FinalSignalList, correlation);
	FinalIterator.Reset ();
	DataSignal* prevSignal;
	double bp;
	int ibp;
	int numberNoncritical = 0;
	RGDList stillCriticalList;

	if ((Size > 0) && (FinalSignalList.Entries () < Size)) {

		int nCurves = FinalSignalList.Entries ();
		eString << nCurves << " available out of " << Size << " required for Locus..." << "\n";
		eString << "LOCUS NAMED " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS\n";
		ExcelText.Write (OutputLevelManager::LocusGridQuality, eString);

		SetMessageValue (locusTooFewPeaks, true);
		status = -1;
	}

	else if ((Size > 0) && (FinalSignalList.Entries () == Size)) {

		// Just compute the correlation and call it a day!

		double* signalArray = new double [Size];
		int iii = 0;
		prevSignal = (DataSignal*) FinalIterator ();

		while (nextSignal = (DataSignal*) FinalIterator ()) {

			signalArray [iii] = nextSignal->GetMean () - prevSignal->GetMean ();
			prevSignal = nextSignal;
			iii++;
		}

		const double* idealNormalizedDifferences = mLink->GetDifferenceVector ();
		correlation = PopulationMarkerSet::VectorDotProductWithDifferences (signalArray, idealNormalizedDifferences, Size-1);
		delete[] signalArray;
	}

	else {

		if (FinalSignalList.Entries () >= Size) {

			overFlow.Clear ();
			const double* differenceArray = mLink->GetUnnormalizedDifferenceVector ();
			double extremePoints [3];
			const double* actualArray = mLink->GetLocusVector ();
			extremePoints [0] = actualArray [0];
			extremePoints [2] = actualArray [Size - 1];
			extremePoints [1] = actualArray [Size - 2];
			const double* leftNorm2s = mLink->GetNormsLeft ();
			const double* rightNorm2s = mLink->GetNormsRight ();
			const int* hts = mLink->GetRelativeHeights ();
			IdealControlSetInfo ctlInfo (actualArray, differenceArray, leftNorm2s, rightNorm2s, hts, Size, true);
			int startLimit = (Size / 2) - 1;
			int startPts;

			startPts = Size;
			int maxDiscrepancy;
			int disc;

			if (hts != NULL)
				maxDiscrepancy = 4;

			else
				maxDiscrepancy = 2;

			for (disc=2; disc<=maxDiscrepancy; disc++) {

				FinalIterator.Reset ();
				tempSignalList.Clear ();
				toTheLeftList.Clear ();
				toTheRightList.Clear ();
				overFlow.Clear ();
				ctlInfo.mDiscrepancy = disc;

				while (nextSignal = (DataSignal*) FinalIterator ())
					tempSignalList.Append (nextSignal);

				if (hts != NULL) {

					tempIterator.Reset ();
					double testPeak;

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempIterator ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, 0, testPeak, MaxPeak, disc)) {

							tempIterator.RemoveCurrentItem ();
							toTheLeftList.Append (nextSignal);
						}

						else
							break;
					}

					while (tempSignalList.Entries () > Size) {

						nextSignal = (DataSignal*) tempSignalList.GetLast ();
						testPeak = nextSignal->Peak ();

						if (RecursiveInnerProduct::HeightOutsideLimit (hts, Size - 1, testPeak, MaxPeak, disc))
							toTheRightList.Prepend (nextSignal);

						else {

							tempSignalList.Append (nextSignal);
							break;
						}
					}
				}

				recursiveStatus = PopulationMarkerSet::SearchRecursivelyForBestLadderLocusSubset (tempSignalList, overFlow, ctlInfo, correlation, 0.995, startPts, MaxPeak);

				if ((recursiveStatus < 0) || (correlation < 0.995)) {

					if (disc == maxDiscrepancy) {

						if (print)
							msg.WriteInsufficientPeaksForControlSet (FinalSignalList.Entries ());

						if (status == 0)
							ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...There are too few peaks within expected parameters.\n";

						SetMessageValue (locusTooFewPeaks, true);
						//info = " with ";  //*******************************************************************************add as begin text to SmartMessage
	//					info << FinalSignalList.Entries () << " peaks out of " << Size;  //**********************************add as separator to SmartMessage
						AppendDataForSmartMessage (locusTooFewPeaks, FinalSignalList.Entries ());
						AppendDataForSmartMessage (locusTooFewPeaks, Size);

						overFlow.Clear ();
						cout << ErrorString << endl;

						//
						// Add msg if hts != NULL about relative sizes*************************
						//

						if (hts != NULL) {

							SetMessageValue (relativeHeightImbalance, true);
						}

						status = -1;
					}
				}

				else {

					overflowIterator.Reset ();
					double nextMean;
					tempIterator.Reset ();
					FinalSignalList.Clear ();

					while (nextSignal = (DataSignal*) tempSignalList.GetFirst ())
						FinalSignalList.Append (nextSignal);

					nextSignal = (DataSignal*) FinalSignalList.First ();
					mFirstTime = nextSignal->GetMean ();
					nextSignal = (DataSignal*) FinalSignalList.Last ();
					mLastTime = nextSignal->GetMean ();

					//
					// if hts != NULL and disc > 2, add message about failure of relative height criteria ********************
					//

					if ((hts != NULL) && (disc > 2)) {

						SetMessageValue (relativeHeightImbalance, true);
					}
					
					while (nextSignal = (DataSignal*) overFlow.GetFirst ()) {

						nextMean = nextSignal->GetMean ();

						if (nextMean < mFirstTime) {

							// this peak is left of core ladder locus, so, with overlap, might belong to another locus

		//					TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		//					if (TestResult > 0)
								toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
						}

						else if (nextMean > mLastTime) {

							// this peak is right of core ladder locus, so, with overlap, might belong to another locus

		//					TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		//					if (TestResult > 0)
								toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
						}

						else {

							// this peak is within core ladder locus, so test for non-critical artifact
							bool test1 = (nextSignal->Peak () <= fractionalThreshold);
							bool test2 = (nextSignal->GetMessageValue (pullup) && !nextSignal->GetMessageValue (primaryPullup) && (nextSignal->Peak () <= pullupFractionalThreshold));
							bool test3 = false;

							// any are true if signal is non-critical artifact; so, all are false if signal is critical because out of place

							TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

							if (TestResult <= 0)
								test3 = true;

							if (test1)
								nextSignal->SetMessageValue (fractionalFilterNotice, true);

							if (test2)
								nextSignal->SetMessageValue (pullupFractionalFilterNotice, true);

							if (test1 || test2 || test3)
								numberNoncritical++;

							else
								stillCriticalList.Append (nextSignal);

							// Add in test for "complex" stutter and adenylation here.
						}
					}

					if (stillCriticalList.Entries () > 0) {

						SetMessageValue (unexpectedNumberOfPeaks, true);

						while (nextSignal = (DataSignal*) stillCriticalList.GetFirst ()) {

							nextSignal->SetMessageValue (extraneousLadderLocusPeak, true);
							nextSignal->AddNoticeToList (OutputLevelManager::PeakOutOfPlace, "", 
								"Peak out of place in final list:  uncategorized artifact");
							bp = nextSignal->GetApproximateBioID ();
							ibp = (int) floor (bp + 0.5);
							AppendDataForSmartMessage (unexpectedNumberOfPeaks, ibp);
							originalList.RemoveReference (nextSignal);

							// should we remove from originalList???????????
						}

						stillCriticalList.Clear ();
					}

					break;
				}
			}
		}

		Displacement = 0;
	}

	if (correlation < 0.995) {

		if (status == 0)
			ErrorString << "LOCUS DOES NOT MEET EXPECTATIONS...Correlation, " << correlation << ", is too low.\n";

		SetMessageValue (poorLocusMorphology, true);
		AppendDataForSmartMessage (poorLocusMorphology, correlation);
		overFlow.Clear ();
		cout << ErrorString << endl;

		status = -1;
	}

	int CurrentIndex = 0;
	int TotalCandidates = FinalSignalList.Entries ();

	while (CurrentIndex < Displacement) {

		// these signals are left of core ladder locus, but might be in another locus

		nextSignal = (DataSignal*)FinalSignalList.GetFirst ();
		//TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		//if (TestResult > 0) {

		//	nextSignal->AddNoticeToList (OutputLevelManager::PeakBelowLocus, "", "Peak represents measurement at base pair level below locus");
		//	nextSignal->SetMessageValue (interlocusPeak, true);
		//}

		toTheLeftList.InsertWithNoReferenceDuplication (nextSignal);
		CurrentIndex++;
	}

	while (FinalSignalList.Entries () > Size) {

		// these signals are right of core ladder locus, but might be in another locus

		nextSignal = (DataSignal*)FinalSignalList.GetLast ();
		//TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		//if (TestResult > 0) {

		//	nextSignal->AddNoticeToList (OutputLevelManager::PeakAboveLocus, "", "Peak represents measurement at base pair level above locus");
		//	nextSignal->SetMessageValue (interlocusPeak, true);
		//}

		toTheRightList.InsertWithNoReferenceDuplication (nextSignal);
	}

	// Put toTheLeftList and toTheRightList into LocusSignalList for safe-keeping until next round, when can test
	
	LocusSignalList.Clear ();

	if (toTheLeftList.Entries () > 0) {

		MergeListAIntoListB (toTheLeftList, LocusSignalList);
	}

	if (toTheRightList.Entries () > 0) {

		MergeListAIntoListB (toTheRightList, LocusSignalList);
	}

	CurveIterator.Reset ();
	FinalIterator.Reset ();
	int i = 0;
	int NumberOfAcceptedCurves = FinalSignalList.Entries ();

	if (NumberOfAcceptedCurves == 0) {

		ErrorString << "LOCUS " << mLink->GetLocusName () << " DOES NOT MEET EXPECTATIONS...There are no peaks within expected parameters.\n";

		SetMessageValue (locusTooFewPeaks, true);
		//info = " with ";  //*******************************************************************************add as begin text to SmartMessage
		//info << FinalSignalList.Entries () << " peaks out of " << Size;  //**********************************add as separator to SmartMessage
		AppendDataForSmartMessage (locusTooFewPeaks, FinalSignalList.Entries ());
		AppendDataForSmartMessage (locusTooFewPeaks, Size);

		while (nextSignal = (DataSignal*) CurveIterator ())
			nextSignal->ReportNotices (ExcelText, "\t\t", "\t");

		return -1;
	}

	double* CurveNumbers = new double [NumberOfAcceptedCurves];
	double* Fits = new double [NumberOfAcceptedCurves];
	double* Peaks = new double [NumberOfAcceptedCurves];
	double* Sigmas = new double [NumberOfAcceptedCurves];
	double* SecondaryContent = new double [NumberOfAcceptedCurves];
	double* Means = new double [NumberOfAcceptedCurves];
	double TwoMass;
	double OneMass;

	nextSignal = (DataSignal*) FinalSignalList.First ();
	mFirstTime = nextSignal->GetMean ();
	nextSignal = (DataSignal*) FinalSignalList.Last ();
	mLastTime = nextSignal->GetMean ();

	while (nextSignal = (DataSignal*)FinalIterator ()) {

		CurveNumbers [i] = i + 1;
		Fits [i] = nextSignal->GetCurveFit ();
		Peaks [i] = nextSignal->Peak ();

		Sigmas [i] = nextSignal->GetStandardDeviation ();
		Means [i] = nextSignal->GetMean ();
		TwoMass = nextSignal->GetScale (2);
		OneMass = nextSignal->GetScale (1);
		SecondaryContent [i] = TwoMass / (TwoMass + OneMass);

		if (print)
			nextSignal->Report (text, "    ");

		i++;

		if (status == 0) {

			nextSignal->SetMessageValue (signalIsCtrlPeak, true);
			nextSignal->SetMessageValue (crater, false);
			nextSignal->SetMessageValue (craterSidePeak, false);
			nextSignal->SetDontLook (false);
			nextSignal->RemoveAllCrossChannelSignalLinksSM ();
			originalList.RemoveReference (nextSignal);
		}
	}

	if (print) {

		msg.WriteEmptyLine ();
		msg.WriteMaximumCorrelationWithControlSet (0, correlation, Size, TotalCandidates);  // !!!!!
		msg.StartLine (0, "Curve Nos.", TRUE);
		msg.StartLine (1, "Fits", TRUE);
		msg.StartLine (2, "2AryContent", TRUE);
		msg.StartLine (3, "Means", TRUE);
		msg.StartLine (4, "Sigmas", TRUE);
		msg.StartLine (5, "Peaks", TRUE);

		for (int j=0; j<NumberOfAcceptedCurves; j++) {

			msg.AddToLine (0, CurveNumbers [j]);
			msg.AddToLine (1, Fits [j]);
			msg.AddToLine (2, SecondaryContent [j]);
			msg.AddToLine (3, Means [j]);
			msg.AddToLine (4, Sigmas [j]);
			msg.AddToLine (5, Peaks [j]);
		}

		msg.FlushLines (6);
	}

	delete[] CurveNumbers;
	delete[] Fits;
	delete[] SecondaryContent;
	delete[] Sigmas;
	delete[] Peaks;
	delete[] Means;

	if (InsertAlleleSignals (FinalSignalList) < 0) {

		ExcelText.Write (OutputLevelManager::LocusGridQuality, ErrorString);
		return -1;
	}

	if (status == 0)
		mIsOK = true;

	return status;
}


int Locus :: AnalyzeSampleLocusSM (ChannelData* lsData, RGDList& artifactList, RGDList& signalList, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	//
	//  This is sample stage 2
	//

	RGDListIterator CurveIterator (LocusSignalList);
	FinalSignalList.Clear ();
	DataSignal* nextSignal;
	int Size = mLink->GetLocusVectorSize ();
	msg.WriteEmptyLine ();
	msg.WriteLocusName (mLink->GetLocusName (), Size);

//	TestFractionalFilter (artifactList, signalList, supplementalList);
	PrecomputeAverages (LocusSignalList);
	RGDListIterator it (LocusSignalList);

	while (nextSignal = (DataSignal*) it())
		TestSampleAveragesSM (lsData, nextSignal, Locus::TestRatio);
	
	return 0;
}


int Locus :: TestGridInterlocusThresholds (RGDList& originalList) {

	//
	//  This is ladder stage 2
	//

	DataSignal* nextSignal;
	int TestResult;
	//double ladderInterlocusMinRFU = STRLadderChannelData::GetMinInterlocusRFU ();

	smInterlocusLadderPeak interlocusPeak;
	//smHeightBelowInterlocusMinRFU belowInterlocusMinRFU;

	RGDListIterator it (originalList);
	double testTime;

	nextSignal = (DataSignal*) FinalSignalList.First ();
	double firstTime = nextSignal->GetMean ();

	nextSignal = (DataSignal*) FinalSignalList.Last ();
	double lastTime = nextSignal->GetMean ();

	while (nextSignal = (DataSignal*) it ()) {

		testTime = nextSignal->GetMean ();

		if ((testTime <= lastTime) && (testTime >= firstTime)) {

			it.RemoveCurrentItem ();
			nextSignal->SetMessageValue (interlocusPeak, false);
			continue;
		}
	}

	if (LocusSignalList.Entries () == 0)
		return 0;

	nextSignal = (DataSignal*) LocusSignalList.First ();
	double firstInterlocus;
	
	if (nextSignal != NULL)
		firstInterlocus = nextSignal->GetMean ();

	else
		firstInterlocus = firstTime;

	nextSignal = (DataSignal*) LocusSignalList.Last ();
	double lastInterlocus;

	if (nextSignal != NULL)
		lastInterlocus = nextSignal->GetMean ();

	else
		lastInterlocus = lastTime;

	while (nextSignal = (DataSignal*) it ()) {

		testTime = nextSignal->GetMean ();

		if (testTime < firstInterlocus)
			continue;

		if (testTime > lastInterlocus)
			break;

		TestResult = TestComplexNeighborsForGridSM (nextSignal, FinalSignalList);

		if (TestResult > 0) {

			if (testTime < firstTime)
				nextSignal->AddNoticeToList (OutputLevelManager::PeakBelowLocus, "", "Peak represents measurement at base pair level below locus");

			else if (testTime > lastTime)
				nextSignal->AddNoticeToList (OutputLevelManager::PeakBelowLocus, "", "Peak represents measurement at base pair level above locus");

			else
				continue;

			nextSignal->SetMessageValue (interlocusPeak, true);

			//if (nextSignal->Peak () <= ladderInterlocusMinRFU)
			//	nextSignal->SetMessageValue (belowInterlocusMinRFU, true);
		}
	}

	LocusSignalList.Clear ();
	return 0;
}


int Locus :: CallAllelesSM (bool isNegCntl, GenotypesForAMarkerSet* pGenotypes, RGDList& artifacts, RGDList& signalList, RGDList& completeList) {

	//
	//  This is sample stage 3
	//

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	DataSignal* prevSignal = NULL;
	DataSignal* followingSignal = NULL;
	double bp;
	int IntBP;
	RGString alleleName;
	RGString prevAlleleName;
	Boolean isOffGrid;
	RGString msg;
	bool prevSignalWasCrater = false;
	RGString lName = GetLocusName ();
	double residual;
	bool foundOLAllele = false;
	RGDList SignalsToDeleteFromAll;
	RGDList SignalsToDeleteFromLocus;
	int location;
	bool testForNegative = false;
	bool locationNonZero;
	RGDList tempList;

	smAcceptedOLLeft acceptedOLLeft;
	smAcceptedOLRight acceptedOLRight;
	smSignalOL offLadder;

	while (nextSignal = (DataSignal*) it ()) {

		prevSignal = nextSignal->GetPreviousLinkedSignal ();

		if ((prevSignal != NULL) && !prevSignal->IsNegativePeak ())
			tempList.Append (prevSignal);

		tempList.Append (nextSignal);

		followingSignal = nextSignal->GetNextLinkedSignal ();

		if ((followingSignal != NULL) && !followingSignal->IsNegativePeak ())
			tempList.Append (followingSignal);
	}

	while (nextSignal = (DataSignal*) tempList.GetFirst ()) {

		location = TestSignalPositionRelativeToLocus (nextSignal);

		if (location != 0)
			locationNonZero = true;

		bp = nextSignal->GetBioID (-location);
		IntBP = (int) floor (bp + 0.5);
		alleleName = ReconstructAlleleName (IntBP, isOffGrid);
		residual = bp - (double)IntBP;
		nextSignal->SetBioIDResidual (residual, -location);
		nextSignal->SetAlleleName (alleleName, -location);

		if (location == 0)
			nextSignal->SetVirtualAlleleName (alleleName);

		if (alleleName.GetCharacter (0) == '-')
			testForNegative = true;

		if (isOffGrid) {

			if (pGenotypes->ContainsOffLadderAllele (lName, nextSignal->GetAlleleName (-location))) {

				nextSignal->SetOffGrid (-location, false);
				
				if (location > 0)
					nextSignal->SetMessageValue (acceptedOLLeft, true);

				else if (location < 0)
					nextSignal->SetMessageValue (acceptedOLRight, true);

				else {

					nextSignal->SetAcceptedOffGrid (true);
					nextSignal->SetMessageValue (offLadder, false);
				}
			}

			else {

				nextSignal->SetOffGrid (-location, true);

				if (location == 0)
					nextSignal->SetMessageValue (offLadder, true);
			}

			if (mIsAMEL) {

				alleleName = "";
				nextSignal->SetAlleleName (alleleName, -location);
			}
		}

		else {

			nextSignal->SetOffGrid (-location, false);

			if (location == 0)
				nextSignal->SetMessageValue (offLadder, false);
		}
	}

	smPreliminaryLocusHasNoAlleles noAlleles;
	smBelowMinRFU belowAnalysisThreshold;
	smLocusHasPeaksBetweenDetectionAndAnalysisThresholds locusHasPeaksTooLow;
	int countPeaksAboveAnalysisThreshold = 0;

	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		if (!nextSignal->GetMessageValue (belowAnalysisThreshold))
			countPeaksAboveAnalysisThreshold++;
	}

	if (countPeaksAboveAnalysisThreshold == 0)
		SetMessageValue (noAlleles, true);

	it.Reset ();
	RGString lowPeakName;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->GetMessageValue (belowAnalysisThreshold)) {

			//it.RemoveCurrentItem ();
			//signalList.RemoveReference (nextSignal);
			//mExtendedLeft.RemoveReference (nextSignal);
			//mExtendedRight.RemoveReference (nextSignal);
			SetMessageValue (locusHasPeaksTooLow, true);
			location = TestSignalPositionRelativeToLocus (nextSignal);
			lowPeakName = nextSignal->GetAlleleName (-location);
			AppendDataForSmartMessage (locusHasPeaksTooLow, lowPeakName);
		}
	}

	return 0;
}


DataSignal*  Locus :: GetLargestPeak () {

	smPullUp pullupPeak;
	smSpike spike;
	smBlob blob;
	smCurveFitFailed curveFitFailed;
	smPeakInCoreLadderLocus peakInCoreLadderLocus;
	
	DataSignal* nextSignal;
	DataSignal* maxSignal = NULL;
	DataSignal* secondSignal = NULL;
	bool ambiguous;
	bool a1;
	bool am1;
	bool a0;
	bool locationNonZero;
	int location;
	mLargestPeak = NULL;

	if (mIsAMEL)
		return NULL;

	double maxHeight = 0.0;
	double secondHeight = 0.0;
	double nextHeight;
	RGDListIterator it (LocusSignalList);

	while (nextSignal = (DataSignal*) it ()) {

		location = -TestSignalPositionRelativeToLocus (nextSignal);
		a0 = nextSignal->GetMessageValue (peakInCoreLadderLocus);
		a1 = nextSignal->IsPossibleInterlocusAllele (1);
		am1 = nextSignal->IsPossibleInterlocusAllele (-1);
		locationNonZero = (location != 0);
		ambiguous = (locationNonZero && a0) || (a1 && am1);

		if (ambiguous)
			continue;

		nextHeight = nextSignal->Peak ();

		if (nextHeight >= maxHeight) {

			secondHeight = maxHeight;
			secondSignal = maxSignal;
			maxHeight = nextHeight;
			mLargestPeak = maxSignal = nextSignal;
		}

		else if (nextHeight > secondHeight) {

			secondHeight = nextHeight;
			secondSignal = nextSignal;
		}
	}

	if (maxSignal == NULL)
		return NULL;

	if (maxSignal->GetMessageValue (pullupPeak) || maxSignal->GetMessageValue (curveFitFailed) || maxSignal->GetMessageValue (spike) || maxSignal->GetMessageValue (blob)) {

		if (secondSignal == NULL) {

			mLargestPeak = NULL;
			return NULL;
		}

		if (secondSignal->GetMessageValue (pullupPeak) || secondSignal->GetMessageValue (curveFitFailed) || secondSignal->GetMessageValue (spike) || secondSignal->GetMessageValue (blob)) {

			mLargestPeak = NULL;
			return NULL;
		}

		if (secondHeight <= 0.5 * maxHeight) {

			mLargestPeak = NULL;
			return NULL;
		}

		mLargestPeak = secondSignal;
		return secondSignal;
	}

	return maxSignal;
}


int Locus :: TestResidualDisplacement () {

	//
	// Sample stage 3
	//

	if (mIsAMEL)
		return 0;

	if (LocusSignalList.Entries () == 1)
		return 0;

	DataSignal* maxSignal = mLargestPeak;
	DataSignal* nextSignal;
	int nextLocation;
	double nextResidual;
	double residualDisplacement;
	smExcessiveResidualDisplacement excessiveResidualDisplacement;
	smResidualDisplacementThreshold residualDisplacementThreshold;
	smExcessiveResidualDisplacementLeft excessiveResidualDisplacementLeft;
	smExcessiveResidualDisplacementRight excessiveResidualDisplacementRight;

	if (maxSignal == NULL)
		return -1;

	int location = -TestSignalPositionRelativeToLocus (maxSignal);  // return value is signal position relative to locus...reverse it to get locus position relative to signal
	double maxResidual = maxSignal->GetBioIDResidual (location);
	RGDListIterator it (LocusSignalList);
	double threshold = 0.01 * (double)GetThreshold (residualDisplacementThreshold);

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal == maxSignal)
			continue;

		nextLocation = -TestSignalPositionRelativeToLocus (nextSignal);
		nextResidual = nextSignal->GetBioIDResidual (nextLocation);
		residualDisplacement = fabs (nextResidual - maxResidual);

		if (residualDisplacement > threshold) {

			if (nextLocation == 0)
				nextSignal->SetMessageValue (excessiveResidualDisplacement, true);

			else if (nextLocation < 0)
				nextSignal->SetMessageValue (excessiveResidualDisplacementLeft, true);

			else
				nextSignal->SetMessageValue (excessiveResidualDisplacementRight, true);
		}
	}

	return 0;
}


int Locus :: TestFractionalFiltersSM (RGDList& artifactList, RGDList& supplementalList) {

	//
	//  This is sample stage 2
	//

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;

	int location;
//	PullUpFound pullupNotice;
	smPullUp pullUp;
	smCalculatedPurePullup purePullup;
	smPrimaryInterchannelLink primaryPullup;
	smHeightBelowFractionalFilter belowFractionalFilter;
	smHeightBelowPullupFractionalFilter belowPullupFractionalFilter;

	smBelowFractionalFilterLeft	belowFractionalFilterLeft;
	smBelowFractionalFilterRight belowFractionalFilterRight;
	smBelowPullupFractionalFilterLeft belowPullupFFLeft;
	smBelowPullupFractionalFilterRight belowPullupFFRight;

	double maxPeak = 0.0;
	double peak;
	double fractionalThreshold = 0.0;
	double pullupFractionalThreshold = 0.0;
//	double fractionalFilter = Locus::GetSampleFractionalFilter ();
//	double pullupFractionalFilter = Locus::GetSamplePullupFractionalFilter ();
	double fractionalFilter = GetLocusSpecificSampleFractionalFilter ();
	double pullupFractionalFilter = GetLocusSpecificSamplePullupFractionalFilter ();
	bool peakIsLessThanFractionalThreshold;
	bool peakIsLessThanPullupFractionalThreshold;
	bool peakIsPullup;

	smPartOfExtendedLocusLeft partOfExtendedLocusLeft;
	smPartOfExtendedLocusRight partOfExtendedLocusRight;
//	unsigned signalId;
	
	if ((fractionalFilter > 0.0) || (pullupFractionalFilter > 0.0)) {
	
		while (nextSignal = (DataSignal*) it ()) {

			if (nextSignal->GetMessageValue (pullUp) && !nextSignal->GetMessageValue (primaryPullup))
				continue;

			location = TestSignalPositionRelativeToLocus (nextSignal);

			if (location != 0)
				continue;

			peak = nextSignal->Peak ();

			if (peak > maxPeak)
				maxPeak = peak;
		}

		mMaxPeak = maxPeak;

		if (maxPeak <= 0.0)
			return 0;

		fractionalThreshold = fractionalFilter * maxPeak;
		pullupFractionalThreshold = pullupFractionalFilter * maxPeak;
		it.Reset ();

		while (nextSignal = (DataSignal*) it ()) {

			peak = nextSignal->Peak ();
			peakIsLessThanFractionalThreshold = (peak <= fractionalThreshold);
			peakIsPullup = nextSignal->GetMessageValue (pullUp) || nextSignal->GetMessageValue (purePullup);
			peakIsLessThanPullupFractionalThreshold = (peakIsPullup && !nextSignal->GetMessageValue (primaryPullup)) && (peak <= pullupFractionalThreshold);

			if (peakIsLessThanFractionalThreshold || peakIsLessThanPullupFractionalThreshold) {

				location = TestSignalPositionRelativeToLocus (nextSignal);
//testing				nextSignal->SetPossibleInterlocusAllele (-location, false); // if location is 1, this locus is to LEFT of signal, ...
	//			it.RemoveCurrentItem ();	Test!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				if (location == 0) {

	//				artifactList.InsertWithNoReferenceDuplication (nextSignal);		//Need this?????????????????????????????????????????
	//				nextSignal->SetAlleleName ("");		//Need this?????????????????????????????????????????

					if (peakIsLessThanFractionalThreshold)
						nextSignal->SetMessageValue (belowFractionalFilter, true);

					if (peakIsLessThanPullupFractionalThreshold)
						nextSignal->SetMessageValue (belowPullupFractionalFilter, true);
				}
				
				else {

					supplementalList.InsertWithNoReferenceDuplication (nextSignal);

					if (location < 0) {

						// Peak is to the left of locus so locus is to the RIGHT of peak!

//testing						nextSignal->SetAlleleName ("", 1);
//testing						mExtendedLeft.RemoveReference (nextSignal);
		//				nextSignal->SetMessageValue (partOfExtendedLocusRight, false);	// what's left of locus regards locus as to right!

						if (peakIsLessThanFractionalThreshold)
							nextSignal->SetMessageValue (belowFractionalFilterRight, true);

						if (peakIsLessThanPullupFractionalThreshold)
							nextSignal->SetMessageValue (belowPullupFFRight, true);
					}

					else if (location > 0) {

						// Peak is to the right of locus so locus is to the LEFT of peak!

//testing						nextSignal->SetAlleleName ("", -1);
//testing						mExtendedRight.RemoveReference (nextSignal);
		//				nextSignal->SetMessageValue (partOfExtendedLocusLeft, false);	// what's right of locus regards locus as to left!

						if (peakIsLessThanFractionalThreshold)
							nextSignal->SetMessageValue (belowFractionalFilterLeft, true);

						if (peakIsLessThanPullupFractionalThreshold)
							nextSignal->SetMessageValue (belowPullupFFLeft, true);
					}
				}
			}
		}
	}

	return 0;
}


Boolean Locus :: ExtractGridSignalsSM (RGDList& channelSignalList, const LaneStandard* ls, ChannelData* lsData) {

	//
	//  This is ladder stage 1
	//

	smLocusIsAMEL locusIsAMEL;
	smLocusIsYLinked locusIsYLinked;

	if (mIsAMEL)
		SetMessageValue (locusIsAMEL, true);

	if (mLink->isYLinked ())
		SetMessageValue (locusIsYLinked, true);

	Boolean status = mLink->ExtractGridSignalsSM (channelSignalList, ls, LocusSignalList, lsData, (Locus*) this);

	if (!status)
		ErrorString += mLink->GetError ();

	return status;
}


int Locus :: TestInterlocusSignalsSM (RGDList& signalSet, RGDList& artifacts, ChannelData* laneStd) {

	// We assume that this method tests samples; either use a different method or augment this one
	// to test ladder loci
	// Note:  This function does not seem to be in use...04/16/2014

	DataSignal* firstSignal;
	DataSignal* lastSignal;
	DataSignal* nextSignal;
	DataSignal* nextLocusSignal;
	int repeatNo = mLink->GetCoreNumber ();
	CoordinateTransform* globalSouthern = laneStd->GetIDMap ();
	RGDList tempLocus;

	if (globalSouthern == NULL)
		return -1;

	if (LocusSignalList.IsEmpty ())
		return 0;

	firstSignal = (DataSignal*) LocusSignalList.First ();
	double firstTime = firstSignal->GetMean ();
	double approxBioID1 = globalSouthern->EvaluateWithExtrapolation (firstTime);
	double bioID1 = firstSignal->GetBioID ();

	lastSignal = (DataSignal*) LocusSignalList.Last ();
	double lastTime = lastSignal->GetMean ();
	double approxBioID2 = globalSouthern->EvaluateWithExtrapolation (lastTime);
	double bioID2 = lastSignal->GetBioID ();

	RGDListIterator it (signalSet);
	double mean;
//	Notice* newNotice;
	double nextPeak;
	double approxBioID;
	double delID;
	double bioID;
	int bioIDDifference;
	double delTime1;
	double delTime2;
	double locusBioID;
	double locusPeak;
//	double stutterLimit = Locus::GetSampleStutterThreshold ();
	double stutterLimit = GetLocusSpecificSampleStutterThreshold ();
//	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();
	double adenylationLimit = GetLocusSpecificSampleAdenylationThreshold ();	
	bool isExtraLocusAllele;
	RGString data;

	smStutter stutterFound;
	smAdenylation adenylationFound;
	smPossibleExtraLocusAllele unreportedOL;
	smLocusMayHaveUnreportedOLs locusUnreportedOLs;

	RGDListIterator itLocus (LocusSignalList);

	while (nextSignal = (DataSignal*) it ()) {

		isExtraLocusAllele = false;
		mean = nextSignal->GetMean ();
		approxBioID = globalSouthern->EvaluateWithExtrapolation (mean);
		delTime1 = fabs (firstTime - mean);
		delTime2 = fabs (lastTime - mean);
		
		if (delTime1 <= delTime2) {

			// use firstSignal data

			delID = approxBioID1 - approxBioID;
			bioID = bioID1 - delID;
		}

		else {

			// use lastSignal data

			delID = approxBioID2 - approxBioID;
			bioID = bioID2 - delID;
		}

		if (mean <= firstTime) {

			bioIDDifference = (int) floor (bioID1 - bioID + 0.5);

			if (bioIDDifference > repeatNo)
				continue;
		}

		else if (mean >= lastTime) {

			bioIDDifference = (int) floor (bioID - bioID2 + 0.5);

			if (bioIDDifference > repeatNo)
				break;
		}

		itLocus.Reset ();

		while (nextLocusSignal = (DataSignal*) itLocus ()) {

			locusBioID = nextLocusSignal->GetBioID ();

			if (locusBioID >= bioID) {

				bioIDDifference = (int) floor (locusBioID - bioID + 0.5);

				if (bioIDDifference > repeatNo)
					break;

				if (!DisableStutterFilter && (bioIDDifference == repeatNo)) {

					// stutter, but which way?

					locusPeak = nextLocusSignal->Peak ();
					nextPeak = nextSignal->Peak ();

					if (nextPeak <= stutterLimit * locusPeak) {

						// OL peak is left stutter
					
						nextSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
						"Signal represents stutter for following peak");
						
						nextSignal->SetMessageValue (stutterFound, true);
				//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
						it.RemoveCurrentItem ();
				//		artifacts.InsertWithNoReferenceDuplication (nextSignal);	//Need this??????????????????????????????????????
						break;
					}

					else if (locusPeak <= stutterLimit * nextPeak) {

						// Locus peak is right stutter to OL peak

						nextLocusSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", 
						"Signal represents stutter for preceding peak");
						nextLocusSignal->SetMessageValue (stutterFound, true);

			//			Locus::RemoveExtraneousNoticesFromSignal (nextLocusSignal);
						tempLocus.InsertWithNoReferenceDuplication (nextLocusSignal);
	//testing					nextLocusSignal->SetAlleleName ("");

						if (!isExtraLocusAllele) {

							isExtraLocusAllele = true;
							nextSignal->SetMessageValue (unreportedOL, true);
							nextSignal->AppendDataForSmartMessage (unreportedOL, GetLocusName ());
						}

						continue;
					}
				}

				else if (!DisableAdenylationFilter && (bioIDDifference == 1)) {

					// adenylation, but which way?

					locusPeak = nextLocusSignal->Peak ();
					nextPeak = nextSignal->Peak ();

					if (nextPeak <= adenylationLimit * locusPeak) {

						// OL peak is left adenylation
					
						nextSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
							"Signal represents non-template addition (-A) for following peak");
						nextSignal->SetMessageValue (adenylationFound, true);
						it.RemoveCurrentItem ();
			//			artifacts.InsertWithNoReferenceDuplication (nextSignal);	//Need this?????????????????????????????????????????
			//			Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
						break;
					}

					// Changed 12/15/2013...there is no +A adenylation

	//				else if (locusPeak <= adenylationLimit * nextPeak) {

	//					// Locus peak is right adenylation to OL peak

	//					nextLocusSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", 
	//						"Signal represents non-template addition (+A) for previous peak");
	//					nextLocusSignal->SetMessageValue (adenylationFound, true);
	//					tempLocus.InsertWithNoReferenceDuplication (nextLocusSignal);
	////testing					nextLocusSignal->SetAlleleName ("");
	//		//			Locus::RemoveExtraneousNoticesFromSignal (nextLocusSignal);

	//					if (!isExtraLocusAllele) {

	//						isExtraLocusAllele = true;
	//						nextSignal->SetMessageValue (unreportedOL, true);
	//						nextSignal->AppendDataForSmartMessage (unreportedOL, GetLocusName ());
	//					}

	//					continue;
	//				}
				}
			}

			else {

				bioIDDifference = (int) floor (bioID - locusBioID + 0.5);

				if (bioIDDifference > repeatNo)
					continue;

				if (!DisableStutterFilter && (bioIDDifference == repeatNo)) {

					// stutter, but which way?

					locusPeak = nextLocusSignal->Peak ();
					nextPeak = nextSignal->Peak ();

					if (nextPeak <= stutterLimit * locusPeak) {

						// OL peak is right stutter
					
						nextSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", 
							"Signal represents stutter for preceding peak");
						nextSignal->SetMessageValue (stutterFound, true);
			//			Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
						it.RemoveCurrentItem ();
			//			artifacts.InsertWithNoReferenceDuplication (nextSignal);	// Need this???????????????????????????????????????????????
						break;
					}

					else if (locusPeak <= stutterLimit * nextPeak) {

						// Locus peak is left stutter to OL peak

						nextLocusSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
							"Signal represents stutter for following peak");
						nextLocusSignal->SetMessageValue (stutterFound, true);
			//			Locus::RemoveExtraneousNoticesFromSignal (nextLocusSignal);
						tempLocus.InsertWithNoReferenceDuplication (nextLocusSignal);
	//testing					nextLocusSignal->SetAlleleName ("");

						if (!isExtraLocusAllele) {

							isExtraLocusAllele = true;
							nextSignal->SetMessageValue (unreportedOL, true);
							nextSignal->AppendDataForSmartMessage (unreportedOL, GetLocusName ());
						}

						continue;
					}
				}

				else if (!DisableAdenylationFilter && (bioIDDifference == 1)) {

					// adenylation, but which way?

					locusPeak = nextLocusSignal->Peak ();
					nextPeak = nextSignal->Peak ();

					if (nextPeak <= adenylationLimit * locusPeak) {

						// OL peak is left adenylation
					
						nextSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", 
							"Signal represents non-template addition (+A) for preceding peak");
						nextSignal->SetMessageValue (adenylationFound, true);
				//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
						it.RemoveCurrentItem ();
				//		artifacts.InsertWithNoReferenceDuplication (nextSignal);	// Need this????????????????????????????????????
						break;
					}

					else if (locusPeak <= adenylationLimit * nextPeak) {

						// Locus peak is right adenylation to OL peak

						nextLocusSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
							"Signal represents non-template addition (-A) for following peak");
						nextLocusSignal->SetMessageValue (adenylationFound, true);
						tempLocus.InsertWithNoReferenceDuplication (nextLocusSignal);
		//testing				nextLocusSignal->SetAlleleName ("");
				//		Locus::RemoveExtraneousNoticesFromSignal (nextLocusSignal);

						if (!isExtraLocusAllele) {

							isExtraLocusAllele = true;
							nextSignal->SetMessageValue (unreportedOL, true);
							nextSignal->AppendDataForSmartMessage (unreportedOL, GetLocusName ());
						}

						continue;
					}
				}
			}
		}
	}

	if (tempLocus.Entries () > 0) {

		SetMessageValue (locusUnreportedOLs, true);
		AppendDataForSmartMessage (locusUnreportedOLs, tempLocus.Entries ());
	}

	while (nextLocusSignal = (DataSignal*) tempLocus.GetFirst ()) {

//		artifacts.InsertWithNoReferenceDuplication (nextLocusSignal);	// Need this??????????????????????????????????????????????????????????????
		LocusSignalList.RemoveReference (nextLocusSignal);
	}

	return 0;
}


int Locus :: TestInterlocusSignalsSM (RGDList& signalSet, RGDList& artifacts, RGDList& type1List, RGDList& type2List) {

	// We assume that this method tests samples; either use a different method or augment this one
	// to test ladder loci

	// This is a rewrite of the original TestInterlocusSignals to take account of interlocus shared extended signals

	//
	//  This is sample stage 3
	//

	if (DisableStutterFilter && DisableAdenylationFilter)
		return 0;

	DataSignal* firstSignal;
	DataSignal* lastSignal;
	DataSignal* nextSignal;
	int repeatNo = mLink->GetCoreNumber ();
	RGDList tempLocus;

	if (LocusSignalList.IsEmpty ())
		return 0;

	firstSignal = (DataSignal*) LocusSignalList.First ();
	double firstTime = firstSignal->GetMean ();

	lastSignal = (DataSignal*) LocusSignalList.Last ();
	double lastTime = lastSignal->GetMean ();

	RGDListIterator it (signalSet);
	double mean;
//	double stutterLimit = Locus::GetSampleStutterThreshold ();
	double stutterLimit = GetLocusSpecificSampleStutterThreshold ();
	double plusStutterLimit = GetLocusSpecificSamplePlusStutterThreshold ();
//	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();
	double adenylationLimit = GetLocusSpecificSampleAdenylationThreshold ();
	RGString data;

	double timeForOneBPLeft = mTimeForOneIDUnitLeft;
	double timeForOneBPRight = mTimeForOneIDUnitRight;

	if ((timeForOneBPLeft <= 0.0) || (timeForOneBPRight <= 0.0))
		return -1;

	double timeForOneRepeatLeft = repeatNo * timeForOneBPLeft;
	double timeForOneRepeatRight = repeatNo * timeForOneBPRight;

	double minDelForOneBPLeft = 0.5 * timeForOneBPLeft;
	double maxDelForOneBPLeft = 1.5 * timeForOneBPLeft;
	double minDelForOneBPRight = 0.5 * timeForOneBPRight;
	double maxDelForOneBPRight = 1.5 * timeForOneBPRight;

	double minDelForOneRepeatLeft = timeForOneRepeatLeft - minDelForOneBPLeft;
	double maxDelForOneRepeatLeft = timeForOneRepeatLeft + minDelForOneBPLeft;
	double minDelForOneRepeatRight = timeForOneRepeatLeft - minDelForOneBPRight;
	double maxDelForOneRepeatRight = timeForOneRepeatLeft + minDelForOneBPRight;

	RGDList externalSignalsLeft;
	RGDList externalSignalsRight;
	double lowerLimit = firstTime - maxDelForOneRepeatLeft;
	double upperLimit = lastTime + maxDelForOneRepeatRight;

	smLocusMayHaveUnreportedOLs locusUnreportedOLs;

	while (nextSignal = (DataSignal*) it ()) {

		mean = nextSignal->GetMean ();

		if ((lowerLimit < mean) && (mean < upperLimit)) {

			if (mean < firstTime)
				externalSignalsLeft.Append (nextSignal);

			else if (mean > lastTime)
				externalSignalsRight.Append (nextSignal);
		}
	}

	if ((externalSignalsLeft.Entries () == 0) && (externalSignalsRight.Entries () == 0))
		return 0;

	RGDListIterator itL (externalSignalsLeft);
	RGDListIterator itR (externalSignalsRight);
	RGDListIterator itLocus (LocusSignalList);

	smStutter stutter;
	smAdenylation adenylation;

	// Add test for dual extended loci in each of below...oops, no longer needed:  if a signal is in one of externalSignals lists, it cannot be in this locus' extended locus

	if (!DisableAdenylationFilter) {

		while (nextSignal = (DataSignal*) itL ()) {

			if (nextSignal->GetMessageValue (adenylation))
				continue;

			if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneBPLeft, maxDelForOneBPLeft, minDelForOneBPRight, maxDelForOneBPRight, adenylationLimit)) {

		//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
				type1List.InsertWithNoReferenceDuplication (nextSignal);
			}
		}

		while (nextSignal = (DataSignal*) itR ()) {

			if (nextSignal->GetMessageValue (adenylation))
				continue;

			if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneBPLeft, maxDelForOneBPLeft, minDelForOneBPRight, maxDelForOneBPRight, adenylationLimit)) {

		//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
				type1List.InsertWithNoReferenceDuplication (nextSignal);
			}
		}

		while (nextSignal = (DataSignal*) it ()) {

			if (nextSignal->GetMessageValue (adenylation))
				continue;

			if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneBPLeft, maxDelForOneBPLeft, minDelForOneBPRight, maxDelForOneBPRight, adenylationLimit)) {

		//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
				type1List.InsertWithNoReferenceDuplication (nextSignal);
		//testing			nextSignal->SetAlleleName ("");
			}
		}
	}

	if (mIsAMEL)
		return 0;

	itL.Reset ();
	itR.Reset ();
	it.Reset ();

	if (!DisableStutterFilter) {

		while (nextSignal = (DataSignal*) itL ()) {

			if (nextSignal->GetMessageValue (stutter))
				continue;

			if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneRepeatLeft, maxDelForOneRepeatLeft, minDelForOneRepeatRight, maxDelForOneRepeatRight, stutterLimit, plusStutterLimit)) {

		//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
				type2List.InsertWithNoReferenceDuplication (nextSignal);
			}
		}

		while (nextSignal = (DataSignal*) itR ()) {

			if (nextSignal->GetMessageValue (stutter))
				continue;

			if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneRepeatLeft, maxDelForOneRepeatLeft, minDelForOneRepeatRight, maxDelForOneRepeatRight, stutterLimit, plusStutterLimit)) {

		//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
				type2List.InsertWithNoReferenceDuplication (nextSignal);
			}
		}

		while (nextSignal = (DataSignal*) it ()) {

			if (nextSignal->GetMessageValue (stutter))
				continue;

			if (TestForProximityArtifact (nextSignal, externalSignalsLeft, externalSignalsRight, minDelForOneRepeatLeft, maxDelForOneRepeatLeft, minDelForOneRepeatRight, maxDelForOneRepeatRight, stutterLimit, plusStutterLimit)) {

		//		Locus::RemoveExtraneousNoticesFromSignal (nextSignal);
				type2List.InsertWithNoReferenceDuplication (nextSignal);
	//testing			nextSignal->SetAlleleName ("");
			
				if (!GetMessageValue (locusUnreportedOLs)) {

					SetMessageValue (locusUnreportedOLs, true);
					AppendDataForSmartMessage (locusUnreportedOLs, tempLocus.Entries ());
				}
			}
		}
	}

	return 0;
}


int Locus :: TestAmelogeninSM () {

	//
	//  This is sample stage 5
	//

//	if (mLink->GetLocusName () != "AMEL")
	if (!mIsAMEL)
		return 0;

	smNoXForAMEL noX;
	smNoYForAMEL noY;

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	bool foundX = false;
	bool foundY = false;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->GetAlleleName () == "1")
			foundX = true;

		if (nextSignal->GetAlleleName () == "2")
			foundY = true;
	}

	if (!foundY) {

		SetMessageValue (noY, true);
		Locus::SetNoYForAMEL (true);
	}

	else
		Locus::SetNoYForAMEL (false);

	if (!foundX) {

		SetMessageValue (noX, true);
		return -1;
	}

	return 0;
}


int Locus :: TestPositiveControlSM (IndividualLocus* locus, RGDList& artifacts) {

	//
	//  This is sample stage 5
	//

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	IndividualAllele* newAllele;
	RGString alleleName;
	RGDList alleleSet;
//	Notice* newNotice;
//	Notice* newSignalNotice;
	smPosCtrlLocusMismatch posCtrlLocusMismatch;

	if (mLink->isQualityLocus ())
		return 0;

	while (nextSignal = (DataSignal*) it ()) {

		alleleName = nextSignal->GetAlleleName ();
		newAllele = new IndividualAllele;
		newAllele->SetName (alleleName);
		newAllele->SetAssociatedPeak (nextSignal);
		alleleSet.Append (newAllele);
	}

	RGDList extraAlleles;
	RGDList missingAlleles;
	SetDifference (alleleSet, locus->mAllelesByName, extraAlleles, missingAlleles);
	
	if ((extraAlleles.Entries () == 0) && (missingAlleles.Entries () == 0)) {

		alleleSet.ClearAndDelete ();
		return 0;
	}

	SetMessageValue (posCtrlLocusMismatch, true);
	RGString data;
	data = "Extra Allele(s):  ";
	int i = 0;

	if (extraAlleles.Entries () == 0)
		data << "None; ";

	else {

		while (newAllele = (IndividualAllele*) extraAlleles.GetFirst ()) {

			if (i == 0)
				data << newAllele->GetName ();

			else
				data << ", " << newAllele->GetName ();

			i++;
//			nextSignal = newAllele->GetAssociatedPeak ();
//
//			if (nextSignal != NULL) {
//
//				nextSignal->SetMessageValue (extraneousPosCtrlPeak, true);
////				artifacts.InsertWithNoReferenceDuplication (nextSignal);	// Need this????????????????????????????????????????
//			}
		}

		data << "; ";
	}

	data << "Missing Allele(s):  ";
	i = 0;

	if (missingAlleles.Entries () == 0)
		data << "None; ";

	else {

		while (newAllele = (IndividualAllele*) missingAlleles.GetFirst ()) {

			if (i == 0)
				data << newAllele->GetName ();

			else
				data << ", " << newAllele->GetName ();

			i++;
		}

		data << "; ";
	}

	AppendDataForSmartMessage (posCtrlLocusMismatch, data);
	alleleSet.ClearAndDelete ();
	return -1;
}


void Locus :: InitializeMessageData () {

	int size = SmartMessage::GetSizeOfArrayForScope (GetObjectScope ());
	Locus::InitializeMessageMatrix (mMessageArray, size);
}


int Locus :: TestNeighborsSM (DataSignal* previous, DataSignal* testSignal, DataSignal* following) {

	// We assume that this is a ladder test...AnalyzeGridLocus is the only method that calls this routine

	double Width = testSignal->GetStandardDeviation ();
	double peak = testSignal->Peak ();
	Boolean NotAcceptable = FALSE;
	Boolean Marginal = FALSE;
	RGString info;

//	double stutterLimit = Locus::GetGridStutterThreshold ();
//	double adenylationLimit = Locus::GetGridAdenylationThreshold ();
	double stutterLimit = GetLocusSpecificLadderStutterThreshold ();
	double adenylationLimit = GetLocusSpecificLadderAdenylationThreshold ();

	smStutter stutterFound;
	smAdenylation adenylationFound;

	double ThisMean = testSignal->GetMean ();
	double ThisPeak = peak;  // = testSignal->Peak ();
	double ThisSigma = Width; // = testSignal->GetStandardDeviation ();
	double RepetitionWidth1 = 2.0 * (double)mLink->GetCoreNumber ();
	double RepetitionWidth2 = 6.0 * (double)mLink->GetCoreNumber ();

	//
	// test for non-template addition (-A and +A) and for stutter (n - CoreRepetition and n + CoreRepetition)
	//

	if (following != NULL) {

		double NextMean = following->GetMean ();
		double NextPeak = following->Peak ();

		if (NextMean - ThisMean < RepetitionWidth1 * ThisSigma) {

			if (ThisPeak < adenylationLimit * NextPeak) {

				NotAcceptable = TRUE;
				testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
					"Signal represents non-template addition (-A) for following peak");
				testSignal->SetMessageValue (adenylationFound, true);
		//		Locus::RemoveExtraneousNoticesFromSignal (testSignal);
			}
		}

		else if ((NextMean - ThisMean < RepetitionWidth2 * ThisSigma) && (ThisPeak < stutterLimit * NextPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
				"Signal represents stutter for following peak");
			testSignal->SetMessageValue (stutterFound, true);
		//	Locus::RemoveExtraneousNoticesFromSignal (testSignal);
		}
	}

	if (previous != NULL) {

		double PreviousMean = previous->GetMean ();
		double PreviousPeak = previous->Peak ();

		if ((ThisMean - PreviousMean < RepetitionWidth1 * ThisSigma) && (ThisPeak < adenylationLimit * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", "Signal represents non-template addition (+A) for previous peak");

			testSignal->SetMessageValue (adenylationFound, true);
		//	Locus::RemoveExtraneousNoticesFromSignal (testSignal);
		}

		else if ((ThisMean - PreviousMean < RepetitionWidth2 * ThisSigma) && (ThisPeak < stutterLimit * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", "Signal represents stutter for previous peak");
			testSignal->SetMessageValue (stutterFound, true);
		//	Locus::RemoveExtraneousNoticesFromSignal (testSignal);
		}
	}

	if (previous != NULL)
		testSignal->SetPreviousPeak (previous->Peak ());
	
	else
		testSignal->SetPreviousPeak (0.0);

	if (following != NULL)
		testSignal->SetNextPeak (following->Peak ());
	
	else
		testSignal->SetNextPeak (0.0);

	if (NotAcceptable)
		return -1;

	if (Marginal)
		return 0;

	return 1;
}


int Locus :: TestSimpleNeighborsForGridSM (RGDList& preceedingSignals, DataSignal* testSignal, RGDList& followingSignals) {

	//
	//  This is ladder stage 2
	//

	// We assume that this is a ladder test...AnalyzeGridLocusAndApplyThresholdsSM is the only method that calls this routine

	double peak = testSignal->Peak ();
	Boolean NotAcceptable = FALSE;
	Boolean Marginal = FALSE;
	RGString info;

	double stutterLimit = GetLocusSpecificLadderStutterThreshold ();
	double adenylationLimit = GetLocusSpecificLadderAdenylationThreshold ();

	smStutter stutterFound;
	smAdenylation adenylationFound;

	double testILSRef = testSignal->GetApproximateBioID ();
	double ThisPeak = peak;  // = testSignal->Peak ();
	double coreDistance = (double)mLink->GetCoreNumber ();
	double adenylationDistance = 1.0;
	double previousAdenylationComparison = 0.0;
	double previousStutterComparison = 0.0;
	double followingAdenylationComparison = 0.0;
	double followingStutterComparison = 0.0;
	double nextILSRef;
	double distance;

	RGDListIterator preceeding (preceedingSignals);
	RGDListIterator following (followingSignals);
	preceeding.ResetToEnd ();
	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) preceeding.CurrentItem ()) {

		nextILSRef = nextSignal->GetApproximateBioID ();
		distance = (double) floor (testILSRef - nextILSRef + 0.5);

		if (distance > coreDistance)
			break;

		if (distance == coreDistance) {

			previousStutterComparison = nextSignal->Peak ();
			break;
		}

		//	Changed 12/15/2013 - there is no +A adenylation

		//if (distance == adenylationDistance)
		//	previousAdenylationComparison = nextSignal->Peak ();

		--preceeding;
	}

	while (nextSignal = (DataSignal*) following ()) {

		nextILSRef = nextSignal->GetApproximateBioID ();
		distance = (double) floor (nextILSRef - testILSRef + 0.5);

		if (distance > coreDistance)
			break;

		if (distance == coreDistance) {

			followingStutterComparison = nextSignal->Peak ();
			break;
		}

		if (distance == adenylationDistance)
			followingAdenylationComparison = nextSignal->Peak ();
	}

	//
	// test for non-template addition (-A and +A) and for stutter (n - CoreRepetition and n + CoreRepetition)
	//

	if (ThisPeak <= stutterLimit * previousStutterComparison) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", "Signal represents stutter for previous peak");
		testSignal->SetMessageValue (stutterFound, true);
	}

	if (ThisPeak <= stutterLimit * followingStutterComparison) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", "Signal represents stutter for following peak");
		testSignal->SetMessageValue (stutterFound, true);
	}

	//	Changed 12/15/2013 - there is no +A adenylation
	//if (ThisPeak <= adenylationLimit * previousAdenylationComparison) {

	//	NotAcceptable = TRUE;
	//	testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", "Signal represents non-template addition (+A) for previous peak");
	//	testSignal->SetMessageValue (adenylationFound, true);
	//}

	if (ThisPeak <= adenylationLimit * followingAdenylationComparison) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", "Signal represents non-template addition (-A) for following peak");
		testSignal->SetMessageValue (adenylationFound, true);
	}

	if (NotAcceptable)
		return -1;

	if (Marginal)
		return 0;

	return 1;
}


int Locus :: TestComplexNeighborsForGridSM (DataSignal* testSignal, RGDList& comparisonSignals) {

	//
	//  This is ladder stage 2 and stage 3
	//

	Boolean NotAcceptable = FALSE;
	Boolean Marginal = FALSE;

	double stutterLimit = GetLocusSpecificLadderStutterThreshold ();
	double adenylationLimit = GetLocusSpecificLadderAdenylationThreshold ();

	smStutter stutterFound;
	smAdenylation adenylationFound;

	double testILSRef = testSignal->GetApproximateBioID ();
	double thisPeak = testSignal->Peak ();
	double coreDistance = (double)mLink->GetCoreNumber ();
	double adenylationDistance = 1.0;
	double targetILSRef;
	double bpDifference;
	double coreDifference;
	double absDiff;

	if (coreDistance == 0.0)
		return 1;

	RGDListIterator comparisonIt (comparisonSignals);
	DataSignal* comparisonSignal;

	double stutterComparison = 0.0;
	double adenylationComparison = 0.0;
	double numberOfRepeats;
	double geometricFactor;

	while (comparisonSignal = (DataSignal*) comparisonIt ()) {

		if (comparisonSignal == testSignal)
			continue;

		targetILSRef = comparisonSignal->GetApproximateBioID ();
		absDiff = fabs (targetILSRef - testILSRef);
		bpDifference = fabs (floor (absDiff + 0.5));

		if ((bpDifference == adenylationDistance) && (targetILSRef > testILSRef))	// Added second test 12/15/2013 to prevent adding fictitious +A
			adenylationComparison += comparisonSignal->Peak ();

		else {

			coreDifference = bpDifference / coreDistance;
			numberOfRepeats = floor (coreDifference);

			if ((numberOfRepeats * coreDistance == bpDifference) && (bpDifference > 0.0)) {

				geometricFactor = pow (stutterLimit, numberOfRepeats);
				stutterComparison += geometricFactor * comparisonSignal->Peak ();
			}
		}
	}

	if (thisPeak <= adenylationLimit * adenylationComparison) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", "Signal represents non-template addition for one or more peaks");
		testSignal->SetMessageValue (adenylationFound, true);
	}

	if (thisPeak <= stutterComparison) {	// the stutterLimit has already been factored into the stutterComparison

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", "Signal represents stutter for one or more peaks");
		testSignal->SetMessageValue (stutterFound, true);
	}

	if (NotAcceptable)
		return -1;

	if (Marginal)
		return 0;

	return 1;
}


int Locus :: TestSampleNeighborsSM (DataSignal* previous, DataSignal* testSignal, DataSignal* following) {

	//
	//	Not currently in use...Noted 04/16/2014

	double peak = testSignal->Peak ();
	Boolean NotAcceptable = FALSE;
	Boolean Marginal = FALSE;

	double ThisMean = testSignal->GetMean ();
	double ThisPeak = testSignal->Peak ();
	double ThisSigma = testSignal->GetStandardDeviation ();

//	double stutterLimit = Locus::GetSampleStutterThreshold ();
	double stutterLimit = GetLocusSpecificSampleStutterThreshold ();
//	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();
	double adenylationLimit = GetLocusSpecificSampleAdenylationThreshold ();

	smStutter stutterFound;
	smAdenylation adenylationFound;

	//
	// test for non-template addition (-A and +A) and for stutter (n - 4 and n + 4)
	//

	if (following != NULL) {

		double NextMean = following->GetMean ();
		double NextPeak = following->Peak ();

		if (NextMean - ThisMean < 8.0 * ThisSigma) {

			if (ThisPeak < adenylationLimit * NextPeak) {

				NotAcceptable = TRUE;
				testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
					"Signal represents non-template addition (-A) for following peak");
				testSignal->SetMessageValue (adenylationFound, true);
			//	Locus::RemoveExtraneousNoticesFromSignal (testSignal);
			}
		}

		else if ((NextMean - ThisMean < 24.0 * ThisSigma) && (ThisPeak < stutterLimit * NextPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
				"Signal represents stutter for following peak");
			testSignal->SetMessageValue (stutterFound, true);
	//		Locus::RemoveExtraneousNoticesFromSignal (testSignal);
		}
	}

	if (previous != NULL) {

		double PreviousMean = previous->GetMean ();
		double PreviousPeak = previous->Peak ();

		// 12/15/2103...there is no +A

	//	if ((ThisMean - PreviousMean < 8.0 * ThisSigma) && (ThisPeak < adenylationLimit * PreviousPeak)) {

	//		NotAcceptable = TRUE;
	//		testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", 
	//			"Signal represents non-template addition (+A) for previous peak");
	//		testSignal->SetMessageValue (adenylationFound, true);
	////		Locus::RemoveExtraneousNoticesFromSignal (testSignal);
	//	}

		if ((ThisMean - PreviousMean < 24.0 * ThisSigma) && (ThisPeak < stutterLimit * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", 
				"Signal represents stutter for previous peak");
			testSignal->SetMessageValue (stutterFound, true);
	//		Locus::RemoveExtraneousNoticesFromSignal (testSignal);
		}
	}

	if (previous != NULL)
		testSignal->SetPreviousPeak (previous->Peak ());
	
	else
		testSignal->SetPreviousPeak (0.0);

	if (following != NULL)
		testSignal->SetNextPeak (following->Peak ());
	
	else
		testSignal->SetNextPeak (0.0);

	if (NotAcceptable)
		return -1;

	if (Marginal)
		return 0;

	return 1;
}


int Locus :: TestSampleAveragesSM (ChannelData* lsData, DataSignal* testSignal, Boolean testRatio) {

	//
	//  This is sample stage 2
	//

	double Width = testSignal->GetStandardDeviation ();
	double peak = testSignal->Peak ();
	double mean = testSignal->GetMean ();
	RGString info;
	smPoorPeakMorphology poorPeakMorphology;
	double targetWidth = lsData->GetWidthAtTime (mean);

	if (targetWidth < 0.0)
		return 1;

	double tooNarrow = 0.5 * targetWidth;
	double tooWide = 2.0 * targetWidth;

	if (Width < tooNarrow) {

		testSignal->AddNoticeToList (OutputLevelManager::SignalTooNarrow, "", "Signal is too narrow for set");
		
		if (!testSignal->GetMessageValue (poorPeakMorphology)) {

			testSignal->SetMessageValue (poorPeakMorphology, true);
			testSignal->AppendDataForSmartMessage (poorPeakMorphology, "(Too Narrow)");
		}
	}

	if (Width > tooWide) {

		testSignal->AddNoticeToList (OutputLevelManager::SignalTooWide, "", "Signal is too wide for set");

		if (!testSignal->GetMessageValue (poorPeakMorphology)) {

			testSignal->SetMessageValue (poorPeakMorphology, true);
			testSignal->AppendDataForSmartMessage (poorPeakMorphology, "(Too Wide)");
		}
	}

	return 1;
}


Boolean Locus :: ExtractExtendedSampleSignalsSM (RGDList& channelSignalList, Locus* gridLocus, CoordinateTransform* timeMap, Locus* prevGridLocus, Locus* followingGridLocus) {

	//
	//  This is sample stage 1
	//

	double mean;
	double gridTime;
	DataSignal* nextSignal;
	RGDListIterator it (channelSignalList);
	LocusSignalList.Clear ();
	FinalSignalList.Clear ();
	mSmartList.Clear ();
	double time;
	int location;
	bool haveFoundSignals = false;
	smPartOfExtendedLocusLeft partOfExtendedLocusLeft;
	smPartOfExtendedLocusRight partOfExtendedLocusRight;
	smPeakInCoreLadderLocus peakInCoreLadderLocus;
	smAssociatedLadderLocusIsCritical associatedLadderLocusIsCritical;
	smLocusIsAMEL locusIsAMEL;
	smLocusIsYLinked locusIsYLinked;
	smIsQualityLocus qualityLocus;

	double bpNext;
	//double bpPrev;
	//double bpFollowing;
	int IntBP;
	RGString alleleName;
	Boolean isOffGrid;

	smExtendLociEdgeToEdgePreset extendLociEdgeToEdgePreset;
	smAllowCoreLocusOverlapsToOverrideEdgeToEdgePreset allowCoreLocusOverlapsPreset;
	smMaxILSBPForExtendedLocus maxILSBPForExtendedLocusThreshold;

	bool extendLociEdgeToEdge = false; 
	
	if (GetMessageValue (extendLociEdgeToEdgePreset))
		extendLociEdgeToEdge = true;

	bool allowCoreLocusOverlaps = false;
	
	if (GetMessageValue (allowCoreLocusOverlapsPreset))
		allowCoreLocusOverlaps = true;

	double maxILSBPForLocus = (double)GetThreshold (maxILSBPForExtendedLocusThreshold);
	bool edgeToEdge = (extendLociEdgeToEdge || allowCoreLocusOverlaps);
	Boolean inExtendedLocus;
	bool addSignalToLocus;
	int neighborLocation;
	bool noMaxILSBP = (maxILSBPForLocus < 0.0);

	int gridArtifactLevel = gridLocus->GetHighestMessageLevelWithRestrictionSM ();

	if ((gridArtifactLevel > 0) && (gridArtifactLevel <= Notice::GetSeverityTrigger ()))
		SetMessageValue (associatedLadderLocusIsCritical, true);

	if (mIsAMEL)
		SetMessageValue (locusIsAMEL, true);

	if (mLink->isYLinked ())
		SetMessageValue (locusIsYLinked, true);

	if (mLink->isQualityLocus ())
		SetMessageValue (qualityLocus, true);

	DataSignal* prevSignal;
	DataSignal* followingSignal;

	RGDListIterator locusIterator (LocusSignalList);

	RGString locusName = GetLocusName ();

	//while (nextSignal = (DataSignal*) locusIterator ())
	//	nextSignal->SetMessageValue (peakInCoreLadderLocus, true);

	// if edgeToEdge is false, execute the following.  Otherwise, execute new code below.

	if (!edgeToEdge) {

		while (nextSignal = (DataSignal*) it()) {

			mean = nextSignal->GetMean ();
			gridTime = timeMap->EvaluateWithExtrapolation (mean);

			if (gridLocus->IsTimeWithinExtendedLocusSample (gridTime, location)) {

				LocusSignalList.Append (nextSignal);
				mSmartList.Append (nextSignal);
				nextSignal->SetBioID (gridLocus->GetBPFromTimeForAnalysis (gridTime), -location);
				nextSignal->SetLocus ((Locus*)this, -location);
				haveFoundSignals = true;

				if (location == 0) {

					nextSignal->SetMessageValue (peakInCoreLadderLocus, true);
				}

				else if (location < 0) {

					mExtendedLeft.Append (nextSignal);
					nextSignal->SetPossibleInterlocusAllele (1, true);
					nextSignal->SetMessageValue (partOfExtendedLocusRight, true);
				}

				else {

					mExtendedRight.Append (nextSignal);
					nextSignal->SetPossibleInterlocusAllele (-1, true);
					nextSignal->SetMessageValue (partOfExtendedLocusLeft, true);
				}

				prevSignal = nextSignal->GetPreviousLinkedSignal ();

				if (prevSignal != NULL) {

					mean = prevSignal->GetMean ();
					gridTime = timeMap->EvaluateWithExtrapolation (mean);
					prevSignal->SetBioID (gridLocus->GetBPFromTimeForAnalysis (gridTime), -location);
					prevSignal->SetLocus ((Locus*)this, -location);

					if (location == 0)
						prevSignal->SetMessageValue (peakInCoreLadderLocus, true);

					else if (location < 0) {

						prevSignal->SetPossibleInterlocusAllele (1, true);
						prevSignal->SetMessageValue (partOfExtendedLocusRight, true);
					}

					else {

						prevSignal->SetPossibleInterlocusAllele (-1, true);
						prevSignal->SetMessageValue (partOfExtendedLocusLeft, true);
					}
				}

				followingSignal = nextSignal->GetNextLinkedSignal ();

				if (followingSignal != NULL) {

					mean = followingSignal->GetMean ();
					gridTime = timeMap->EvaluateWithExtrapolation (mean);
					followingSignal->SetBioID (gridLocus->GetBPFromTimeForAnalysis (gridTime), -location);
					followingSignal->SetLocus ((Locus*)this, -location);

					if (location == 0)
						followingSignal->SetMessageValue (peakInCoreLadderLocus, true);

					else if (location < 0) {

						followingSignal->SetPossibleInterlocusAllele (1, true);
						followingSignal->SetMessageValue (partOfExtendedLocusRight, true);
					}

					else {

						followingSignal->SetPossibleInterlocusAllele (-1, true);
						followingSignal->SetMessageValue (partOfExtendedLocusLeft, true);
					}
				}
			}

			else if (haveFoundSignals)
				break;
		}
	}

	else {   // In some way, we're doing edge to edge here

		while (nextSignal = (DataSignal*) it()) {

			mean = nextSignal->GetMean ();
			gridTime = timeMap->EvaluateWithExtrapolation (mean);
		//	bpNext = (int)gridLocus->GetBPFromTimeForAnalysis (gridTime);
			addSignalToLocus = false;
			inExtendedLocus = gridLocus->IsTimeWithinExtendedLocusSample (gridTime, location) && allowCoreLocusOverlaps;
			location = gridLocus->DirectionOfTimeFromLocus (gridTime);

			//if (allowCoreLocusOverlaps)
			//	inExtendedLocus = gridLocus->IsTimeWithinExtendedLocusSample (gridTime, location);

			//else
			//	inExtendedLocus = FALSE;

			//location = gridLocus->DirectionOfIDFromLocus (bpNext);

			if (location == 0) {

				addSignalToLocus = true;
			}

			else if (location < 0) {

				if (mLink->AllowsNoExtension ()) {

					addSignalToLocus = false;
					continue;
				}

				bpNext = gridLocus->GetBPFromTimeForAnalysis (gridTime);
				IntBP = (int) floor (bpNext + 0.5);
				alleleName = ReconstructAlleleName (IntBP, isOffGrid);

				if (alleleName == "$")
					addSignalToLocus = false;

				else if (prevGridLocus == NULL) {

					// Test against minBP for artifacts should be unnecessary.  We shouldn't be
					// seeing any peaks below minBP
						
					addSignalToLocus = true;
				}

				else {

				//	bpPrev = prevGridLocus->GetBPFromTimeForAnalysis (gridTime);
					neighborLocation = prevGridLocus->DirectionOfTimeFromLocus (gridTime);
				//	prevGridLocus->IsTimeWithinExtendedLocusSample (gridTime, neighborLocation);

					if (neighborLocation > 0)
						addSignalToLocus = true;

					else if ((neighborLocation == 0) && inExtendedLocus)
						addSignalToLocus = true;

					else
						addSignalToLocus = false;
				}
			}

			else { // it's to the right of the locus, i.e., location > 0

				if (mLink->AllowsNoExtension ()) {

					addSignalToLocus = false;
					break;
				}

				if (followingGridLocus == NULL) {

					// Test against maxBP for analysis

					if (noMaxILSBP || (nextSignal->GetApproximateBioID () <= maxILSBPForLocus) || inExtendedLocus)
						addSignalToLocus = true;

					else {

						addSignalToLocus = false;
						break;
					}
				}

				else {

			//		bpFollowing = followingGridLocus->GetBPFromTimeForAnalysis (gridTime);
					neighborLocation = followingGridLocus->DirectionOfTimeFromLocus (gridTime);
			//		followingGridLocus->IsTimeWithinExtendedLocusSample (gridTime, neighborLocation);

					if (neighborLocation < 0)
						addSignalToLocus = true;

					else if ((neighborLocation == 0) && inExtendedLocus)
						addSignalToLocus = true;

					else {

						addSignalToLocus = false;
						break;
					}
				}
			}

			if (addSignalToLocus) {

				LocusSignalList.Append (nextSignal);
				mSmartList.Append (nextSignal);
				nextSignal->SetBioID (gridLocus->GetBPFromTimeForAnalysis (gridTime), -location);
				nextSignal->SetLocus ((Locus*)this, -location);
				haveFoundSignals = true;

				if (location == 0) {

					nextSignal->SetMessageValue (peakInCoreLadderLocus, true);
				}

				else if (location < 0) {

					mExtendedLeft.Append (nextSignal);
					nextSignal->SetPossibleInterlocusAllele (1, true);
					nextSignal->SetMessageValue (partOfExtendedLocusRight, true);
				}

				else {

					mExtendedRight.Append (nextSignal);
					nextSignal->SetPossibleInterlocusAllele (-1, true);
					nextSignal->SetMessageValue (partOfExtendedLocusLeft, true);
				}

				prevSignal = nextSignal->GetPreviousLinkedSignal ();

				if (prevSignal != NULL) {

					mean = prevSignal->GetMean ();
					gridTime = timeMap->EvaluateWithExtrapolation (mean);
					prevSignal->SetBioID (gridLocus->GetBPFromTimeForAnalysis (gridTime), -location);
					prevSignal->SetLocus ((Locus*)this, -location);

					if (location == 0)
						prevSignal->SetMessageValue (peakInCoreLadderLocus, true);

					else if (location < 0) {

						prevSignal->SetPossibleInterlocusAllele (1, true);
						prevSignal->SetMessageValue (partOfExtendedLocusRight, true);
					}

					else {

						prevSignal->SetPossibleInterlocusAllele (-1, true);
						prevSignal->SetMessageValue (partOfExtendedLocusLeft, true);
					}
				}

				followingSignal = nextSignal->GetNextLinkedSignal ();

				if (followingSignal != NULL) {

					mean = followingSignal->GetMean ();
					gridTime = timeMap->EvaluateWithExtrapolation (mean);
					followingSignal->SetBioID (gridLocus->GetBPFromTimeForAnalysis (gridTime), -location);
					followingSignal->SetLocus ((Locus*)this, -location);

					if (location == 0)
						followingSignal->SetMessageValue (peakInCoreLadderLocus, true);

					else if (location < 0) {

						followingSignal->SetPossibleInterlocusAllele (1, true);
						followingSignal->SetMessageValue (partOfExtendedLocusRight, true);
					}

					else {

						followingSignal->SetPossibleInterlocusAllele (-1, true);
						followingSignal->SetMessageValue (partOfExtendedLocusLeft, true);
					}
				}
			}

			else if (haveFoundSignals)
				break;
		}
	}

	nextSignal = (DataSignal*) LocusSignalList.First ();

	if (nextSignal != NULL) {

		location = - TestSignalPositionRelativeToLocus (nextSignal);
		mMinTimeForSample = nextSignal->GetMean ();
		time = gridLocus->GetTimeFromBP (nextSignal->GetBioID (location) - 1.0);
		mTimeForOneIDUnitLeft = mMinTimeForSample - time;

		nextSignal = (DataSignal*) LocusSignalList.Last ();
		location = - TestSignalPositionRelativeToLocus (nextSignal);
		mMaxTimeForSample = nextSignal->GetMean ();
		time = gridLocus->GetTimeFromBP (nextSignal->GetBioID (location) + 1.0);
		mTimeForOneIDUnitRight = time - mMaxTimeForSample;
	}

	return TRUE;
}


void Locus :: CreateInitializationData (int scope) {

	int size = SmartMessage::GetSizeOfArrayForScope (scope);
	int i;
	SmartMessage* msg;
	delete[] InitialMatrix;
	InitialMatrix = new bool [size];

	for (i=0; i<size; i++) {

		msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, i);

		if (msg != NULL) {

			InitialMatrix [i] = msg->GetInitialValue ();
		}
	}
}


void Locus :: InitializeMessageMatrix (bool* matrix, int size) {

	int i;

	for (i=0; i<size; i++)
		matrix [i] = InitialMatrix [i];
}


int Locus :: MeasureInterlocusSignalAttributesSM () {

	//
	//  This is sample stage 3
	//

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	bool wouldCauseHeterozygousImbalance1;
	bool wouldCauseHeterozygousImbalance2;
	Locus* locus1;
	Locus* locus2;

	smWouldCauseHeterozygousImbalanceLeft wouldCauseHeterozygousImbalanceLeft;
	smWouldCauseHeterozygousImbalanceRight wouldCauseHeterozygousImbalanceRight;
	smSignalOffGridLeft signalOffGridLeft;
	smSignalOffGridRight signalOffGridRight;
	sm0UnambiguousPeaksLeft are0UnambiguousPeaksLeft;
	sm0UnambiguousPeaksRight are0UnambiguousPeaksRight;
	sm1UnambiguousPeakLeft is1UnambiguousPeakLeft;
	sm1UnambiguousPeakRight is1UnambiguousPeakRight;
	sm2PlusUnambiguousPeaksLeft are2PlusUnambiguousPeaksLeft;
	sm2PlusUnambiguousPeaksRight are2PlusUnambiguousPeaksRight;
	sm0AmbiguousPeaksLeft are0AmbiguousPeaksLeft;
	sm0AmbiguousPeaksRight are0AmbiguousPeaksRight;
	sm1AmbiguousPeakLeft is1AmbiguousPeakLeft;
	sm1AmbiguousPeakRight is1AmbiguousPeakRight;
	sm2PlusAmbiguousPeaksLeft are2PlusAmbiguousPeaksLeft;
	sm2PlusAmbiguousPeaksRight are2PlusAmbiguousPeaksRight;
	smSignalIsIntegralMultipleOfRepeatLeft signalIsIntegralMultipleOfRepeatLeft;
	smSignalIsIntegralMultipleOfRepeatRight signalIsIntegralMultipleOfRepeatRight;
	int nAmb = 0;
	int nUnamb;
	int nTotal = 0;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->IsDoNotCall ())
			continue;

		nTotal++;

		if ((nextSignal->IsPossibleInterlocusAllele (-1)) && (nextSignal->IsPossibleInterlocusAllele (1)))
			nAmb++;
	}

	nUnamb = nTotal - nAmb;
	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		if ((nextSignal->IsPossibleInterlocusAllele (-1)) && (nextSignal->IsPossibleInterlocusAllele (1))) {

			locus1 = (Locus*) nextSignal->GetLocus (-1);
			locus2 = (Locus*) nextSignal->GetLocus (1);

			if ((locus1 == NULL) && (locus2 == NULL))
				continue;

			if (locus1 == (Locus*)this) {

				// This is locus to left of peak

				if (locus2 == NULL) {

					locus1->PromoteSignalToAllele (nextSignal);
					continue;
				}

				locus1->HasHeightRatioWithExclusiveMaxPeak (nextSignal, wouldCauseHeterozygousImbalance1);

				if (wouldCauseHeterozygousImbalance1)
					nextSignal->SetMessageValue (wouldCauseHeterozygousImbalanceLeft, true);

				if (nextSignal->IsOffGrid (-1))
					nextSignal->SetMessageValue (signalOffGridLeft, true);

				if (nUnamb == 0)
					nextSignal->SetMessageValue (are0UnambiguousPeaksLeft, true);

				else if (nUnamb == 1)
					nextSignal->SetMessageValue (is1UnambiguousPeakLeft, true);

				else
					nextSignal->SetMessageValue (are2PlusUnambiguousPeaksLeft, true);

				if (nAmb == 0)
					nextSignal->SetMessageValue (are0AmbiguousPeaksLeft, true);

				else if (nAmb == 1)
					nextSignal->SetMessageValue (is1AmbiguousPeakLeft, true);

				else
					nextSignal->SetMessageValue (are2PlusAmbiguousPeaksLeft, true);

				if (SignalIsIntegralMultipleOfRepeatAboveLadder (nextSignal))
					nextSignal->SetMessageValue (signalIsIntegralMultipleOfRepeatLeft, true);
			}

			if (locus2 == (Locus*)this) {

				// This is locus to right of peak

				if (locus1 == NULL) {
				
					locus2->PromoteSignalToAllele (nextSignal);
					continue;
				}

				locus2->HasHeightRatioWithExclusiveMaxPeak (nextSignal, wouldCauseHeterozygousImbalance2);

				if (wouldCauseHeterozygousImbalance2)
					nextSignal->SetMessageValue (wouldCauseHeterozygousImbalanceRight, true);

				if (nextSignal->IsOffGrid (1))
					nextSignal->SetMessageValue (signalOffGridRight, true);

				if (nUnamb == 0)
					nextSignal->SetMessageValue (are0UnambiguousPeaksRight, true);

				else if (nUnamb == 1)
					nextSignal->SetMessageValue (is1UnambiguousPeakRight, true);

				else
					nextSignal->SetMessageValue (are2PlusUnambiguousPeaksRight, true);

				if (nAmb == 0)
					nextSignal->SetMessageValue (are0AmbiguousPeaksRight, true);

				else if (nAmb == 1)
					nextSignal->SetMessageValue (is1AmbiguousPeakRight, true);

				else
					nextSignal->SetMessageValue (are2PlusAmbiguousPeaksRight, true);

				if (SignalIsIntegralMultipleOfRepeatBelowLadder (nextSignal))
					nextSignal->SetMessageValue (signalIsIntegralMultipleOfRepeatRight, true);
			}
		}
	}

	return 0;
}


int Locus :: FinalTestForPeakSizeAndNumberSM (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes, RGDList& artifacts) {

	//
	//  This is sample stage 5
	//

	RGString info;
	DataSignal* nextSignal;
	DataSignal* prevSignal;
	double peak;
	double peak2;
	double temp;
//	double heteroLimit = Locus::GetHeterozygousImbalanceLimit ();
//	double minBoundForHomozygote = Locus::GetMinBoundForHomozygote ();
	double heteroLimit = GetLocusSpecificSampleHeterozygousImbalanceThreshold ();
	double minBoundForHomozygote = GetLocusSpecificSampleMinBoundForHomozygote ();
	RGDList alleleList;
	IndividualAllele* newAllele;
	RGString alleleName;
	bool testForAcceptedTriallele;
	bool criticalArtifactFound = false;
	int criticalLevel = Notice::GetMessageTrigger ();
	int nextSignalLevel;
//	RGDListIterator it (LocusSignalList);
	RGDListIterator it (mSmartList);
	RGString localAlleleName;

	smCriticalMessagesAtAlleles locusPeaksHaveCriticalMsgs;
	smThreeOrMoreAlleles triAllele;
	smMoreThanThreeAlleles moreThanThressAlleles;
	smNoGenotypeFound noGenotype;
	smHomozygoteHeightProblem homozygoteProblem;
	smHeterozygousImbalance heterozygousImbalance;
	smLocusIsHomozygous locusIsHomozygous;
	smNumberAllelesBelowExpectation numberOfAllelesBelowExpectation;
	int retValue = 0;

	double minBioID = (double) CoreBioComponent::GetMinBioIDForArtifacts ();
	
	//
	// Consider eliminating the whole next while loop!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//

	while (nextSignal = (DataSignal*) it ()) {

		if ((minBioID > 0.0) && (nextSignal->GetApproximateBioID () < minBioID)) {

			it.RemoveCurrentItem ();
			LocusSignalList.RemoveReference (nextSignal);	//!!!!!!!!!
			continue;
		}

		if (nextSignal->IsDoNotCall ()) {

	//		it.RemoveCurrentItem ();
			LocusSignalList.RemoveReference (nextSignal);
			continue;
		}
	}

	RGDListIterator it2 (LocusSignalList);

	while (nextSignal = (DataSignal*) it2()) {

		nextSignalLevel = nextSignal->GetHighestMessageLevelWithRestrictionSM ();

		if ((nextSignalLevel > 0) && (nextSignalLevel <= criticalLevel)) {

			if (!criticalArtifactFound) {

				localAlleleName = nextSignal->GetAlleleName ();

				if (localAlleleName.Length () > 0) {

					criticalArtifactFound = true;
					SetMessageValue (locusPeaksHaveCriticalMsgs, true);
				}

				else {

					it2.RemoveCurrentItem ();
					continue;
				}
			}

			if (mIsAMEL) {

				localAlleleName = nextSignal->GetAlleleName ();

				if (localAlleleName == "1")
					AppendDataForSmartMessage (locusPeaksHaveCriticalMsgs, "X");

				else if (localAlleleName == "2")
					AppendDataForSmartMessage (locusPeaksHaveCriticalMsgs, "Y");

				else
					AppendDataForSmartMessage (locusPeaksHaveCriticalMsgs, localAlleleName);
			}
			
			else
				AppendDataForSmartMessage (locusPeaksHaveCriticalMsgs, nextSignal->GetAlleleName ());
		}
	}

	if (mIsAMEL)
		TestAmelogeninSM ();

	int N = LocusSignalList.Entries ();

	if (LocusSignalList.Entries () <= mLink->GetMaxExpectedAlleles ())
		SetMessageValue (triAllele, false);

	if (N == 0) {

		if (!isNegCntl) {

			SetMessageValue (noGenotype, true);
			return -1;
		}

		return 0;
	}

	if (isNegCntl) {

		if (mLink->isQualityLocus ())
			return 0;

		return -1;
	}

	if (N == 1) {

		if (mLink->GetMaxExpectedAlleles () == 1)
			return 0;

		if (mLink->GetMinExpectedAlleles () > 1) {

			SetMessageValue (numberOfAllelesBelowExpectation, true);
			return -1;
		}

		// Test for proper homozygote
		nextSignal = (DataSignal*) LocusSignalList.First ();
		peak = nextSignal->Peak ();
		SetMessageValue (locusIsHomozygous, true);

		if ((minBoundForHomozygote > 0.0) && (peak < minBoundForHomozygote)) {

			SetMessageValue (homozygoteProblem, true);
			return -1;
		}

		return 0;
	}

	if (N == 2) {

		nextSignal = (DataSignal*) LocusSignalList.First ();
		peak = nextSignal->Peak ();
		prevSignal = nextSignal;
		nextSignal = (DataSignal*) LocusSignalList.Last ();
		peak2 = nextSignal->Peak ();

		if (peak2 > peak) {

			temp = peak;
			peak = peak2;
			peak2 = temp;
		}
		
		if (peak2 < heteroLimit * peak) {

			SetMessageValue (heterozygousImbalance, true);
			AppendDataForSmartMessage (heterozygousImbalance, peak2 / peak);
			retValue = -1;
		}

		if (mLink->GetMaxExpectedAlleles () < 2) {

			it.Reset ();

			while (nextSignal = (DataSignal*) it ()) {

				alleleName = nextSignal->GetAlleleName ();
				newAllele = new IndividualAllele ();
				newAllele->SetName (alleleName);
				alleleList.Append (newAllele);
			}

//			if (isPosCntl)		// eliminated 08/22/2014 because positive controls cannot report this message anymore (via MessageBook)
//				testForAcceptedTriallele = pGenotypes->ContainsControlTriallele (GetLocusName (), alleleList);

			if (!isPosCntl)
				testForAcceptedTriallele = pGenotypes->ContainsSampleTriallele (GetLocusName (), alleleList);
				
			alleleList.ClearAndDelete ();

			if (!testForAcceptedTriallele) {

				SetMessageValue (triAllele, true);
				retValue = -1;
			}

			else {

				SetMessageValue (triAllele, false);
			}

			return retValue;
		}

		return retValue;
	}

	if (N == 3) {

		it.Reset ();

		while (nextSignal = (DataSignal*) it ()) {

			alleleName = nextSignal->GetAlleleName ();
			newAllele = new IndividualAllele ();
			newAllele->SetName (alleleName);
			alleleList.Append (newAllele);
		}

//		if (isPosCntl)		// eliminated 08/22/2014 because positive controls cannot report this message anymore (via MessageBook)
//			testForAcceptedTriallele = pGenotypes->ContainsControlTriallele (GetLocusName (), alleleList);

		if (!isPosCntl)
			testForAcceptedTriallele = pGenotypes->ContainsSampleTriallele (GetLocusName (), alleleList);
		
		alleleList.ClearAndDelete ();

		if (!testForAcceptedTriallele) {

			if (mLink->GetMaxExpectedAlleles () == 2)
				SetMessageValue (triAllele, true);

			else if (mLink->GetMaxExpectedAlleles () == 1)
				SetMessageValue (moreThanThressAlleles, true);

			return -1;
		}

		else
			SetMessageValue (triAllele, false);

		return 0;
	}

	SetMessageValue (moreThanThressAlleles, true);
	return -1;
}


int Locus :: CorrectCrossChannelAnalysesSM (RGDList& artifacts, bool isNegCntl) {

	// We assume this method will be used for samples.  For ladders, either use a different method or augment
	// this one.

	//
	//  This is sample stage 4:  all commented out 10/28/2015 because smPullupAtAllele is the result of an expression in 
	//                           the MessageBook and should not be set in software.  Also, this particular logic is not universally valid
	//
	
//	RGDListIterator it (LocusSignalList);
//	DataSignal* nextSignal;
//	double maxPeak = 0.0;
//	double peak;
//	double peakThreshold;
//	double alleleThreshold;
//	RGDListIterator it2 (LocusSignalList);
//
//	smPurePullup pullUp;
//	smPullUpAtAllele allelePullup;
//
////	double stutterLimit = Locus::GetSampleStutterThreshold ();
//	double stutterLimit = GetLocusSpecificSampleStutterThreshold ();
////	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();
//	double adenylationLimit = GetLocusSpecificSampleAdenylationThreshold ();
////	double pullupFractionalLimit = Locus::GetSamplePullupFractionalFilter ();
//	double pullupFractionalLimit = GetLocusSpecificSamplePullupFractionalFilter ();
//
//	while (nextSignal = (DataSignal*) it ()) {
//
//		peak = nextSignal->Peak ();
//
//		if (peak > maxPeak)
//			maxPeak = peak;
//	}
//
//	it.Reset ();
//	peakThreshold = pullupFractionalLimit * maxPeak;  // allows ignoring OL allele with "low" peak
////	alleleThreshold = Locus::GetHeterozygousImbalanceLimit () * maxPeak;  // allows ignoring pull-up
//	alleleThreshold = GetLocusSpecificSampleHeterozygousImbalanceThreshold () * maxPeak;  // allows ignoring pull-up
//	int N = LocusSignalList.Entries ();
//	double p;
//
//	while (nextSignal = (DataSignal*) it ()) {
//
//	//	if (isNegCntl) {	//  Commented out 07/29/2014 in order to allow negative control peaks to be called as alleles, in addition to the extraneous peak in negative control artifact
//
//	////		SetMessageValue (unexpectedNumberOfPeaks, true);
//	//		nextSignal->SetAlleleName ("");
//	//		it.RemoveCurrentItem ();
//	////		nextSignal->SetMessageValue (extraPeakInNegCtrl, true);
//	////		artifacts.InsertWithNoReferenceDuplication (nextSignal);	//Need this????????????????????????????????????????????????????????????
//	//	}
//
//	//	else
//
//		if (nextSignal->GetMessageValue (pullUp)) {  // check instead for HasCrossChannelSignalLink or HasEverHadCrossChannelSignalLink?
//
//			p = nextSignal->Peak ();
//
//			if (p >= alleleThreshold)
//				nextSignal->SetMessageValue (allelePullup, true);
//		}
//	}

	return 0;
}


int Locus :: CleanUpSignalListSM (RGDList& artifacts) {

	//
	//  This is sample stage 4
	//

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	double residual;
	double residualThreshold = Locus::GetMaxResidualForAlleleCalls ();
	RGString data;
	bool addedResidualToLocus = false;
	double absResidual;
	//bool foundOLAllele = false;

	smBPResidualTooLarge residualTooLarge;
	smSignalOL offLadder;
	smSignalIsAllele isAllele;
	smExtraneousAMELPeak extraneousPeakInAMEL;
	smAMELHasExtraneousPeaks extraPeaksInAMEL;

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->IsDoNotCall ()) {

//testing			nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
//testing			nextSignal->SetAlleleName ("");
	//		nextSignal->SetMessageValue (offLadder, false);
			nextSignal->SetMessageValue (isAllele, false);
			it.RemoveCurrentItem ();
		}
	}

	RGDListIterator leftIt (mExtendedLeft);
	RGDListIterator rightIt (mExtendedRight);

	while (nextSignal = (DataSignal*) leftIt ()) {

		if (nextSignal->IsDoNotCall ()) {

//testing			nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
//testing			nextSignal->SetAlleleName ("");
	//		nextSignal->SetMessageValue (offLadder, false);
			nextSignal->SetMessageValue (isAllele, false);
			leftIt.RemoveCurrentItem ();
		}
	}

	while (nextSignal = (DataSignal*) rightIt ()) {

		if (nextSignal->IsDoNotCall ()) {

//testing			nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
//testing			nextSignal->SetAlleleName ("");
	//		nextSignal->SetMessageValue (offLadder, false);
			nextSignal->SetMessageValue (isAllele, false);
			rightIt.RemoveCurrentItem ();
		}
	}

	it.Reset ();

//	while (nextSignal = (DataSignal*) it ()) {		//Need this??????????????????????????????????????????????
//
//		msgLevel = nextSignal->GetHighestMessageLevel ();
//
////		if ((msgLevel > 0) && (msgLevel <= Notice::GetSeverityTrigger ())) {
//		if (msgLevel > 0) {
//
////			it.RemoveCurrentItem ();
////			artifacts.InsertWithNoReferenceDuplication (nextSignal);		//Need this??????????????????????????????????????????????
//		}
//	}		//Need this??????????????????????????????????????????????
//
//	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		// The following is unnecessary because of changes to data from triggers
		//if (nextSignal->IsOffGrid ()) {

		//	SetMessageValue (olAlleles, true);
		//	AppendDataForSmartMessage (olAlleles, nextSignal->GetAlleleName ());
		//}

		residual = nextSignal->GetBioIDResidual ();
		absResidual = fabs (residual);

		if ((residualThreshold > 0.0) && (absResidual > residualThreshold)) {

			nextSignal->SetMessageValue (residualTooLarge, true);
			nextSignal->AppendDataForSmartMessage (residualTooLarge, residual);
	//		artifacts.InsertWithNoReferenceDuplication (nextSignal);	//Need this?????????????????????????????????????????????????????????

			// The following is unnecessary because of changes to data from triggers
			//if (!addedResidualToLocus) {

			//	addedResidualToLocus = true;
			//	SetMessageValue (peaksHaveExcessiveResiduals, true);
			//}

			//data = nextSignal->GetAlleleName ();
			//truncatedResidual = (int) floor (1000.0 * absResidual + 0.5);

			//if (residual > 0.0)
			//	data << " (0." << truncatedResidual << ")";

			//else
			//	data << " (-0." << truncatedResidual << ")";

			//AppendDataForSmartMessage (peaksHaveExcessiveResiduals, data);
		}
	}

	if (mIsAMEL) {

		it.Reset ();

		while (nextSignal = (DataSignal*) it ()) {

			if (!nextSignal->IsOffGrid ())
				continue;

			it.RemoveCurrentItem ();		
			nextSignal->SetMessageValue (extraneousPeakInAMEL, true);

			//if (!foundOLAllele)
			//	SetMessageValue (extraPeaksInAMEL, true);

			//foundOLAllele = true;
		}
	}

	it.Reset ();

	while (nextSignal = (DataSignal*) it ()) {

		nextSignal->SetMessageValue (isAllele, true);

		if (nextSignal->IsOffGrid ())
			nextSignal->SetMessageValue (offLadder, true);
	}

	smPreliminaryLocusHas2OrMoreAlleles twoOrMore;

	if (LocusSignalList.Entries () > 1)
		SetMessageValue (twoOrMore, true);

	return 0;
}


int Locus :: CorrectGridSignalListSM (RGDList& artifacts) {

	//
	//  This is ladder stage 3
	//

	return 0;
}


int Locus :: CleanUpGridSignalListSM (RGDList& artifacts) {

	//
	//  This is ladder stage 3
	//

	RGDListIterator it (FinalSignalList);
	DataSignal* nextSignal;
	int msgLevel;
	int trigger = Notice::GetMessageTrigger ();
	int returnValue = 0;

	while (nextSignal = (DataSignal*) it ()) {

		msgLevel = nextSignal->GetHighestMessageLevelWithRestrictionSM ();

		if (msgLevel > 0) {

	//		artifacts.InsertWithNoReferenceDuplication (nextSignal);	// Need this????????????????????????????????????????????????

			if (msgLevel <= trigger)
				returnValue = -1;
		}

		// Change to:
		// if ((msgLevel > 0) && (msgLevel <= trigger))
		//		return -1;
	}

	return returnValue;
}


int Locus :: TestProximityArtifactsSM (RGDList& artifacts, RGDList& type1List, RGDList& type2List) {

	//
	//  This is sample stage 2:  No longer used as of 03/26/2012 and in Version 2.0
	//

	if (mIsAMEL)
		return 0;
	
	RGDListIterator it (LocusSignalList);
	RGDListIterator it2 (LocusSignalList);
	DataSignal* nextSignal;
	DataSignal* testSignal;
	double stutterPeakTotal;
	double adenylationPeakTotal;
	int nextBP;
	int testBP;
	int diff;
	int repeatNumber = mLink->GetCoreNumber ();
	double peak;
	bool hasStutter;
	bool hasAdenylation;
	bool removeSignal;
	int location;

//	double stutterLimit = Locus::GetSampleStutterThreshold ();
	double stutterLimit = GetLocusSpecificSampleStutterThreshold ();
//	double adenylationLimit = Locus::GetSampleAdenylationThreshold ();
	double adenylationLimit = GetLocusSpecificSampleAdenylationThreshold ();
	smStutter stutterFound;
	smAdenylation adenylationFound;

	while (nextSignal = (DataSignal*) it ()) {

		it2.Reset ();
		stutterPeakTotal = adenylationPeakTotal = 0.0;
		nextBP = (int) floor (nextSignal->GetApproximateBioID () + 0.5);
		hasStutter = hasAdenylation = removeSignal = false;

		while (testSignal = (DataSignal*) it2 ()) {

			testBP = (int) floor (testSignal->GetApproximateBioID () + 0.5);

			if (testBP > nextBP + repeatNumber)
				break;

			diff = abs (nextBP - testBP);

			if (diff == 1) {

				hasAdenylation = true;
				adenylationPeakTotal += testSignal->Peak ();
			}
			
			else if (diff == repeatNumber) {

				hasStutter = true;
				stutterPeakTotal += testSignal->Peak ();
			}
		}

		if (hasAdenylation || hasStutter) {

			peak = nextSignal->Peak ();
			location = TestSignalPositionRelativeToLocus (nextSignal);

			if (hasAdenylation) {

				if (peak < adenylationLimit * adenylationPeakTotal) {

					if (location == 0)
						nextSignal->SetMessageValue (adenylationFound, true);

					else
						type1List.InsertWithNoReferenceDuplication (nextSignal);

					removeSignal = true;
				}
			}

			if (hasStutter) {

				if (peak < stutterLimit * stutterPeakTotal) {

					if (location == 0)
						nextSignal->SetMessageValue (stutterFound, true);

					else
						type2List.InsertWithNoReferenceDuplication (nextSignal);

					removeSignal = true;
				}
			}

			if (removeSignal) {

				it.RemoveCurrentItem ();

				if (location == 0) {

					continue;
	//testing				nextSignal->SetAlleleName ("");
		//			artifacts.InsertWithNoReferenceDuplication (nextSignal);	//Need this????????????????????????????????????????????????????
				}

				else if (location < 0)
					mExtendedLeft.RemoveReference (nextSignal);

				else
					mExtendedRight.RemoveReference (nextSignal);
			}
		}
	}

	return 0;
}


int Locus :: TestProximityArtifactsUsingLocusBasePairsSM (RGDList& artifacts, RGDList& type1List, RGDList& type2List) {

	//
	//  This is sample stage 3
	//

	if (mIsAMEL)
		return 0;

	if (DisableStutterFilter && DisableAdenylationFilter)
		return 0;
	
	RGDListIterator it (LocusSignalList);
	RGDListIterator it2 (LocusSignalList);
	DataSignal* nextSignal;
	DataSignal* testSignal;
	double stutterPeakTotal;
	double adenylationPeakTotal;
	double relativeStutterPeakTotal;
	double relativeAdenylationPeakTotal;
	int nextBP;
	int testBP;
	int diff;
	int repeatNumber = mLink->GetCoreNumber ();
	double peak;
	bool hasStutter;
	bool hasAdenylation;
	int location1;
	int location2;

	double stutterLimit = GetLocusSpecificSampleStutterThreshold ();
	double plusStutterLimit = GetLocusSpecificSamplePlusStutterThreshold ();
	double adenylationLimit = GetLocusSpecificSampleAdenylationThreshold ();
	smStutter stutterFound;
	smBaselineRelativeStutter baselineRelativeStutter;
	smAdenylation adenylationFound;
	smBaselineRelativeAdenylation baselineRelativeAdenylation;

	smAcceptedOLLeft acceptedOLLeft;
	smAcceptedOLRight acceptedOLRight;
	smSignalOL offLadder;

	bool ignoreAdenylation;
	bool onLadderInLocus;

	while (nextSignal = (DataSignal*) it ()) {

		it2.Reset ();
		stutterPeakTotal = adenylationPeakTotal = 0.0;
		relativeStutterPeakTotal = relativeAdenylationPeakTotal = 0.0;
		location1 = TestSignalPositionRelativeToLocus (nextSignal);
		nextBP = (int) floor (nextSignal->GetBioID (-location1) + 0.5);	// location1 is relative to locus; must reverse to make relative to nextSignal (03/23/2012)
		hasStutter = hasAdenylation = false;

		if (location1 == 0)
			onLadderInLocus = !nextSignal->GetMessageValue (offLadder);

		else if (location1 < 0)
			onLadderInLocus = nextSignal->GetMessageValue (acceptedOLRight);  // location1 < 0 means locus is to right of peak

		else
			onLadderInLocus = nextSignal->GetMessageValue (acceptedOLLeft);  //  location1 > 0 means locus is to left of peak

		ignoreAdenylation = (DisableAdenylationFilter || (CallOnLadderAdenylation && onLadderInLocus));

		while (testSignal = (DataSignal*) it2 ()) {

			location2 = TestSignalPositionRelativeToLocus (testSignal);
			testBP = (int) floor (testSignal->GetBioID (-location2) + 0.5);	// location2 is relative to locus; must reverse to make relative to testSignal (03/23/2012)

			if (testBP > nextBP + repeatNumber)
				break;

			diff = abs (nextBP - testBP);

			if (!ignoreAdenylation && (diff == 1) && (testBP > nextBP)) {	// 12/15/2013...don't allow fictitious +A

				hasAdenylation = true;
				adenylationPeakTotal += testSignal->Peak ();
				relativeAdenylationPeakTotal += testSignal->GetBaselineRelativePeak ();
			}
			
			else if (!DisableStutterFilter && (diff == repeatNumber)) {

				hasStutter = true;

				if (nextBP > testBP) {	// this is plus stutter

					stutterPeakTotal += plusStutterLimit * testSignal->Peak ();
					relativeStutterPeakTotal += plusStutterLimit * testSignal->GetBaselineRelativePeak ();
				}

				else {	// this is minus stutter

					stutterPeakTotal += stutterLimit * testSignal->Peak ();
					relativeStutterPeakTotal += stutterLimit * testSignal->GetBaselineRelativePeak ();
				}
			}
		}

		if (hasAdenylation || hasStutter) {

			peak = nextSignal->Peak ();

			if (hasAdenylation) {

				if (peak < adenylationLimit * adenylationPeakTotal) {

					//if (location1 == 0)
					//	nextSignal->SetMessageValue (adenylationFound, true);

					//else
						type1List.InsertWithNoReferenceDuplication (nextSignal);
				}

				else if (nextSignal->HasRaisedBaseline ()) {

					if (nextSignal->GetBaselineRelativePeak () <= adenylationLimit * relativeAdenylationPeakTotal)
						nextSignal->SetMessageValue (baselineRelativeAdenylation, true);
				}
			}

			if (hasStutter) {

				if (peak < stutterPeakTotal) {

					//if (location1 == 0)
					//	nextSignal->SetMessageValue (stutterFound, true);

					//else
						type2List.InsertWithNoReferenceDuplication (nextSignal);
				}

				else if (nextSignal->HasRaisedBaseline ()) {

					if (nextSignal->GetBaselineRelativePeak () <= relativeStutterPeakTotal)
						nextSignal->SetMessageValue (baselineRelativeStutter, true);
				}
			}
		}
	}

	return 0;
}


int Locus :: TestForMultiSignalsSM (RGDList& artifacts, RGDList& signalList, RGDList& completeList, RGDList& smartPeaks, GenotypesForAMarkerSet* pGenotypes) {

	//
	//  This is no longer sample stage 3:  obsolete 02/14/2016
	//

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	DataSignal* prevSignal = NULL;
	DataSignal* followingSignal = NULL;
//	double bp;
//	int IntBP;
	RGString alleleName;
	RGString prevAlleleName;
//	RGString msg;
	bool prevSignalWasCrater = false;
	RGString lName = GetLocusName ();
//	double residual;
	bool foundOLAllele = false;
//	DataSignal* craterSignal;
	RGDList tempList;
	RGDList SignalsToDeleteFromAll;
	RGDList SignalsToDeleteFromLocus;
	InterchannelLinkage* iChannel;
	int location;
	bool isAmbiguous;
//	int isOffGrid;

	smCrater crater;
	smCraterSidePeak craterSidePeak;
	smSignalNotACraterSidePeak notASidePeak;
	smSignalNotACrater notACrater;
	smPurePullup pullup;
	smAdenylation adenylationFound;
	smExtraneousAMELPeak extraneousPeakInAMEL;
	smPoorPeakMorphologyOrResolution poorPeakMorphologyOrResolution;
	smPeakInCoreLadderLocus peakInCoreLadderLocus;
	bool savePrev;
	bool saveNext;

	double adenylationLimit = GetLocusSpecificSampleAdenylationThreshold ();

	//****02/09/2016 Probably need to eliminate the following section because craters have already been validated.  Instead, just test for two peaks with identical call and create NoisyPeak to replace.


	while (nextSignal = (DataSignal*) it ()) {

		location = TestSignalPositionRelativeToLocus (nextSignal);
		isAmbiguous = (nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1));

		if (nextSignal->TestForMultipleSignalsWithinLocus (prevSignal, followingSignal, location, mIsAMEL, adenylationLimit)) {

			if (mIsAMEL) {

				SignalsToDeleteFromAll.Append (nextSignal);
				nextSignal->SetMessageValue (notACrater, true);

				if ((prevSignal != NULL) && (followingSignal != NULL)) {

					double prevResidual = fabs (prevSignal->GetBioIDResidual (-location));	// location is relative to locus; must reverse to make relative to prevSignal (03/26/2012)
					double nextResidual = fabs (followingSignal->GetBioIDResidual (-location));	// location is relative to locus; must reverse to make relative to followingSignal (03/26/2012)
					prevSignal->SetMessageValue (notASidePeak, true);
					followingSignal->SetMessageValue (notASidePeak, true);

					if (prevResidual < nextResidual)
						SignalsToDeleteFromLocus.Append (followingSignal);

					else
						SignalsToDeleteFromLocus.Append (prevSignal);
				}

				else if (prevSignal != NULL)
					prevSignal->SetMessageValue (notASidePeak, true);

				else if (followingSignal != NULL)
					followingSignal->SetMessageValue (notASidePeak, true);
			}

			else if (!isAmbiguous) {

				SignalsToDeleteFromLocus.Append (nextSignal);
				nextSignal->SetMessageValue (notACrater, true);

				if (prevSignal != NULL) {

					prevSignal->SetMessageValue (notASidePeak, true);
					location = TestSignalPositionRelativeToLocus (prevSignal);

					if (location != 0)
						prevSignal->SetAlleleInformation (-location);

	//				if (prevSignal->GetMessageValue (adenylationFound))	//  Test!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//					SignalsToDeleteFromLocus.Append (prevSignal);

		//			if (prevSignal->NumberOfNoticeObjects () == 0)	// Need this??????????????????????????????????????????????
		//				artifacts.RemoveReference (prevSignal);
				}

				if (followingSignal != NULL) {

					followingSignal->SetMessageValue (notASidePeak, true);
					location = TestSignalPositionRelativeToLocus (followingSignal);

					if (location != 0)
						followingSignal->SetAlleleInformation (-location);

	//				if (followingSignal->GetMessageValue (adenylationFound))	//  Test!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//					SignalsToDeleteFromLocus.Append (followingSignal);

		//			if (followingSignal->NumberOfNoticeObjects () == 0)	// Need this??????????????????????????????????????????????
		//				artifacts.RemoveReference (followingSignal);
				}
			}
		}

		else if (!isAmbiguous) {

			if (prevSignal != NULL)
				SignalsToDeleteFromLocus.Append (prevSignal);

			if (followingSignal != NULL)
				SignalsToDeleteFromLocus.Append (followingSignal);
		}
	}

	while (nextSignal = (DataSignal*) SignalsToDeleteFromLocus.GetFirst ()) {

//testing		nextSignal->SetVirtualAlleleName (nextSignal->GetAlleleName ());
//testing		nextSignal->SetAlleleName ("");
		RemoveSignalFromLocusList (nextSignal);
	}

	while (nextSignal = (DataSignal*) SignalsToDeleteFromAll.GetFirst ()) {

		RemoveSignalFromLocusList (nextSignal);
		nextSignal->RemoveFromAllLoci ();
		artifacts.RemoveReference (nextSignal);	// need this to make sure artifact list has no deleted signals!!
		signalList.RemoveReference (nextSignal);
	//	completeList.RemoveReference (nextSignal);
	//	mSmartList.RemoveReference (nextSignal);
	//	smartPeaks.RemoveReference (nextSignal);
		iChannel = nextSignal->GetInterchannelLink ();

		if (iChannel != NULL) {

			iChannel->RemoveDataSignalSM (nextSignal);	//???????????????????????????????????????????????????????????????

			// Later, consider adding one of nextSignal's flanking peaks...need iChannel function to perform test, assuming
			// that primary Signal may be gone.  Then recalculate primary signal

			if (iChannel->IsEmpty ())
				iChannel->RemoveAllSM ();	//??????????????????????????????????????????????????????????????????

			else
				iChannel->RecalculatePrimarySignalSM ();	//????????????????????????????????????????????????????????????????????????
		}

	//	delete nextSignal;
	}



	it.Reset ();
//	bool previousPullUp;
//	bool nextPullUp;
//	bool onePullUp;
	prevSignal = NULL;
	prevAlleleName = "";
	double prevResidual;
	double nextResidual;
	int prevLocation = 0;
	double prevResidualDisplacement;
	double nextResidualDisplacement;
	double maxPeakResidual;

	if (mLargestPeak != NULL)
		maxPeakResidual = mLargestPeak->GetBioIDResidual ();

	while (nextSignal = (DataSignal*) it ()) {

		// test for consecutive signals with same call:  make it a crater (This should be done only for peaks that
		// are unique to this locus

		location = TestSignalPositionRelativeToLocus (nextSignal);
		alleleName = nextSignal->GetAlleleName (-location);	// location is relative to locus; must reverse to make relative to nextSignal (03/26/2012)

		if ((prevSignal != NULL) && (!alleleName.IsEmpty ()) && (prevAlleleName == alleleName)) {

			if (mIsAMEL) {

				nextResidual = fabs (nextSignal->GetBioIDResidual (-location));	// location is relative to locus; must reverse to make relative to nextSignal (03/26/2012)
				prevResidual = fabs (prevSignal->GetBioIDResidual (-prevLocation));	// location is relative to locus; must reverse to make relative to prevSignal (03/26/2012)

				if (prevResidual < nextResidual) {

					it.RemoveCurrentItem ();
					nextSignal->SetMessageValue (extraneousPeakInAMEL, true);
	//testing				nextSignal->SetVirtualAlleleName (alleleName);
	//testing				nextSignal->SetAlleleName ("");
				}

				else {

					tempList.Append (prevSignal);
					prevSignal->SetMessageValue (extraneousPeakInAMEL, true);
	//testing				prevSignal->SetVirtualAlleleName (prevAlleleName);
	//testing				prevSignal->SetAlleleName ("");
					prevSignal = nextSignal;   // changed so that we test nextSignal also
					prevAlleleName = alleleName;
					prevLocation = location;
				}

				continue;
			}

			if (prevSignal->IsDoNotCall () || nextSignal->IsDoNotCall ()) {

				prevSignal = nextSignal;   // changed so that we test nextSignal also
				prevAlleleName = alleleName;
				prevLocation = location;
				continue;
			}

			prevSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
			nextSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
			prevResidual = fabs (prevSignal->GetBioIDResidual (-prevLocation));
			nextResidual = fabs (nextSignal->GetBioIDResidual (-location));
			savePrev = saveNext = false;

			if (prevSignal->Peak () <= 0.95 * nextSignal->Peak ())
				saveNext = true;

			else if (nextSignal->Peak () <= 0.95 * prevSignal->Peak ())
				savePrev = true;

			else if (mLargestPeak != NULL) {

				prevResidualDisplacement = fabs (prevResidual - maxPeakResidual);
				nextResidualDisplacement = fabs (nextResidual - maxPeakResidual);

				if (prevResidualDisplacement < nextResidualDisplacement)
					savePrev = true;

				else if (nextResidualDisplacement < prevResidualDisplacement)
					saveNext = true;

				else {

					if (prevSignal->Peak () < nextSignal->Peak ())
						saveNext = true;

					else if (nextSignal->Peak () < prevSignal->Peak ())
						savePrev = true;
				}
			}

			if (!(saveNext || savePrev)) {

				if (prevResidual < nextResidual)
					savePrev = true;

				else if (nextResidual < prevResidual)
					saveNext = true;

				else {

					if (prevSignal->Peak () < nextSignal->Peak ())
						saveNext = true;

					else if (nextSignal->Peak () < prevSignal->Peak ())
						savePrev = true;

					else
						saveNext = true;
				}
			}

			if (savePrev) {

				signalList.RemoveReference (nextSignal);
				tempList.Append (nextSignal);
			//	nextSignal->SetMessageValue (callPoorPeakMorphology, false);
			//	prevSignal->SetMessageValue (callPoorPeakMorphology, true);
			//	nextSignal->SetDoNotCall (true);
			}

			else {

				signalList.RemoveReference (prevSignal);
				tempList.Append (prevSignal);
				prevSignal = nextSignal;
				prevLocation = location;
				prevAlleleName = alleleName;
			//	nextSignal->SetMessageValue (callPoorPeakMorphology, true);
			//	prevSignal->SetMessageValue (callPoorPeakMorphology, false);
			//	prevSignal->SetDoNotCall (true);
			}

			//if (prevResidual < nextResidual) {

			//	signalList.RemoveReference (nextSignal);
			//	tempList.Append (prevSignal);
			//}

			//else {

			//	signalList.RemoveReference (prevSignal);
			//	tempList.Append (prevSignal);
			//	prevSignal = nextSignal;
			//	prevLocation = location;
			//	prevAlleleName = alleleName;
			//}
		}

		else {

			prevSignal = nextSignal;
			prevLocation = location;
			prevAlleleName = alleleName;
		}
	}

	while (nextSignal = (DataSignal*) tempList.GetFirst ())
		LocusSignalList.RemoveReference (nextSignal);

	return 0;
}


int Locus :: TestForDuplicateAllelesSM (RGDList& artifacts, RGDList& signalList, RGDList& completeList, RGDList& smartPeaks, GenotypesForAMarkerSet* pGenotypes) {

	//
	//  This is sample stage 3
	//

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	DataSignal* prevSignal = NULL;
	DataSignal* followingSignal = NULL;
	DataSignal* currentSignal;
	RGString alleleName;
	RGString prevAlleleName;
	bool prevSignalWasCrater = false;
	RGString lName = GetLocusName ();
	bool foundOLAllele = false;
	RGDList tempList;
	RGDList SignalsToDeleteFromAll;
	RGDList SignalsToDeleteFromLocus;
	int location;

	smExtraneousAMELPeak extraneousPeakInAMEL;
	smPoorPeakMorphologyOrResolution poorPeakMorphologyOrResolution;
	smPeakInCoreLadderLocus peakInCoreLadderLocus;
	smPullUp pullup;
	smPrimaryInterchannelLink primaryPullup;
	smCalculatedPurePullup purePullup;
	smBelowMinRFU belowMinRFU;
	smCrater crater;

	bool prevBelowMinRFU;
	bool nextBelowMinRFU;

	it.Reset ();

	prevSignal = NULL;
	prevAlleleName = "";
	double prevResidual;
	double nextResidual;
	int prevLocation = 0;

	while (nextSignal = (DataSignal*) it ()) {

		// test for consecutive signals with same call:  make it a NoisyPeak (This should be done only for peaks that
		// are unique to this locus

		location = TestSignalPositionRelativeToLocus (nextSignal);
		alleleName = nextSignal->GetAlleleName (-location);	// location is relative to locus; must reverse to make relative to nextSignal (03/26/2012)

		if ((prevSignal != NULL) && (!alleleName.IsEmpty ()) && (prevAlleleName == alleleName)) {

			if (mIsAMEL) {

				nextResidual = fabs (nextSignal->GetBioIDResidual (-location));	// location is relative to locus; must reverse to make relative to nextSignal (03/26/2012)
				prevResidual = fabs (prevSignal->GetBioIDResidual (-prevLocation));	// location is relative to locus; must reverse to make relative to prevSignal (03/26/2012)

				if (prevResidual < nextResidual) {

					it.RemoveCurrentItem ();
					nextSignal->SetMessageValue (extraneousPeakInAMEL, true);
				}

				else {

					tempList.Append (prevSignal);
					prevSignal->SetMessageValue (extraneousPeakInAMEL, true);
					prevSignal = nextSignal;   // changed so that we test nextSignal also
					prevAlleleName = alleleName;
					prevLocation = location;
				}

				continue;
			}

			//if (prevSignal->IsDoNotCall () || nextSignal->IsDoNotCall ()) {	// Let's try not using this to see what happens 02/25/2016*******

			//	prevSignal = nextSignal;   // changed so that we test nextSignal also
			//	prevAlleleName = alleleName;
			//	prevLocation = location;
			//	continue;
			//}

			if (prevSignal->IsPartOfCluster () || nextSignal->IsPartOfCluster ()) {

				//if (report)
				//	cout << "A signal at mean " << prevSignal->GetMean () << " is part of cluster" << endl;

				prevSignal = nextSignal;
				prevAlleleName = alleleName;
				prevLocation = location;
				continue;
			}

			if (prevSignal->GetMessageValue (crater) || nextSignal->GetMessageValue (crater)) {

				//if (report)
				if ((nextSignal->GetMean () > 5570.0) && (nextSignal->GetMean () < 5572.0))
					cout << "A signal at mean " << prevSignal->GetMean () << " or at mean " << nextSignal->GetMean () << " is part of a crater" << endl;

				prevSignal = nextSignal;
				prevAlleleName = alleleName;
				prevLocation = location;
				continue;
			}

			if (prevSignal->GetMessageValue (purePullup) || nextSignal->GetMessageValue (purePullup)) {

				prevSignal = nextSignal;
				prevAlleleName = alleleName;
				prevLocation = location;
				continue;
			}

			if (prevSignal->GetMessageValue (primaryPullup) && !nextSignal->GetMessageValue (primaryPullup)) {

				nextSignal->SetDontLook (true);
				nextSignal->SetDoNotCall (true);
				signalList.RemoveReference (nextSignal);
				continue;
			}

			else if (nextSignal->GetMessageValue (primaryPullup) && !prevSignal->GetMessageValue (primaryPullup)) {

				prevSignal->SetDontLook (true);
				prevSignal->SetDoNotCall (true);
				signalList.RemoveReference (prevSignal);
				prevSignal = nextSignal;
				prevAlleleName = alleleName;
				prevLocation = location;
				continue;
			}

			if (prevSignal->GetMessageValue (pullup) && !nextSignal->GetMessageValue (pullup)) {

				nextSignal->SetDontLook (true);
				nextSignal->SetDoNotCall (true);
				signalList.RemoveReference (nextSignal);
				continue;
			}

			else if (nextSignal->GetMessageValue (pullup) && !prevSignal->GetMessageValue (pullup)) {

				prevSignal->SetDontLook (true);
				prevSignal->SetDoNotCall (true);
				signalList.RemoveReference (prevSignal);
				prevSignal = nextSignal;
				prevAlleleName = alleleName;
				prevLocation = location;
				continue;
			}

			else {

				prevBelowMinRFU = prevSignal->GetMessageValue (belowMinRFU);
				nextBelowMinRFU = nextSignal->GetMessageValue (belowMinRFU);

				if (prevBelowMinRFU && !nextBelowMinRFU) {

					prevSignal = nextSignal;
					prevAlleleName = alleleName;
					prevLocation = location;
					continue;
				}

				else if (!prevBelowMinRFU && nextBelowMinRFU)
					continue;

				else if (prevBelowMinRFU && nextBelowMinRFU) {

					prevSignal = nextSignal;
					prevAlleleName = alleleName;
					prevLocation = location;
					continue;
				}
			}

			prevSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
			nextSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
			currentSignal = new NoisyPeak (prevSignal, nextSignal, true);
			currentSignal->CaptureSmartMessages ();

			currentSignal->SetDontLook (false);
			currentSignal->SetMessageValue (poorPeakMorphologyOrResolution, true);
			tempList.Append (currentSignal);
			signalList.RemoveReference (prevSignal);
			signalList.RemoveReference (nextSignal);
			prevLocation = location;
			prevAlleleName = alleleName;
			prevSignal = currentSignal;
		}

		else {

			prevSignal = nextSignal;
			prevLocation = location;
			prevAlleleName = alleleName;
		}
	}

	while (nextSignal = (DataSignal*) tempList.GetFirst ()) {

		// Add new signals to all lists
		signalList.InsertWithNoReferenceDuplication (nextSignal);
		completeList.InsertWithNoReferenceDuplication (nextSignal);
		smartPeaks.InsertWithNoReferenceDuplication (nextSignal);
		LocusSignalList.InsertWithNoReferenceDuplication (nextSignal);
		mSmartList.InsertWithNoReferenceDuplication (nextSignal);
		//artifacts.InsertWithNoReferenceDuplication (nextSignal);
	}
	
	return 0;
}


void Locus :: RetrieveSmartNoticesFromGridArtifactList (ChannelData* laneStandard) {

	//
	// This is ladder stage 5
	//

	DataSignal* nextSignal;
	RGString info;
	RGString localAlleleName;
	smCriticalMessagesAtAlleles criticalMessagesAtAlleles;

	RGDListIterator FinalIterator (FinalSignalList);
	bool criticalArtifactFound = false;
	int criticalLevel = Notice::GetMessageTrigger ();
	int nextSignalLevel;

	while (nextSignal = (DataSignal*) FinalIterator ()) {

		nextSignalLevel = nextSignal->GetHighestMessageLevelWithRestrictionSM ();

		if (((nextSignalLevel > 0) && (nextSignalLevel <= criticalLevel)) || (nextSignal->IsDoNotCall ())) {

			if (!criticalArtifactFound) {

				localAlleleName = nextSignal->GetAlleleName ();

				if (localAlleleName.Length () > 0) {

					criticalArtifactFound = true;
					SetMessageValue (criticalMessagesAtAlleles, true);
				}
			}

			if (mIsAMEL) {

				localAlleleName = nextSignal->GetAlleleName ();

				if (localAlleleName == "1")
					info = "X";

				else if (localAlleleName == "2")
					info = "Y";

				else
					info = localAlleleName;
			}
			
			else
				info = nextSignal->GetAlleleName ();

			AppendDataForSmartMessage (criticalMessagesAtAlleles, info);
		}
	}
}


void Locus :: TestAllelesAgainstOverloadThresholdSM () {

	//
	//  This is sample stage 4
	//

	smPeakAboveOverloadLimit peakAboveOverloadLimit;
	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;

	double alleleOverloadThreshold = Locus::GetAlleleOverloadThreshold ();

	if (alleleOverloadThreshold > 0.0) {

		while (nextSignal = (DataSignal*) it()) {

			if (nextSignal->Peak () >= alleleOverloadThreshold)
				nextSignal->SetMessageValue (peakAboveOverloadLimit, true);
		}
	}
}



void Locus :: ResolveAmbiguousExtendedLocusPeaksSM () {

	//
	//  This is sample stage 4
	//

	RGDListIterator it (LocusSignalList);
	DataSignal* nextSignal;
	smCouldBelongToLocusLeft couldBelongToLocusLeft;
	smCouldBelongToLocusRight couldBelongToLocusRight;
	smPossibleAmbiguousInterlocusAssignedToLocus ambiguousExtendedLocusAssignedToLocus;
	int location;
	bool belongsToLocusToLeft;
	bool belongsToLocusToRight;
	RGDList tempRemove;

	while (nextSignal = (DataSignal*) it ()) {

		if ((nextSignal->IsPossibleInterlocusAllele (-1)) && (nextSignal->IsPossibleInterlocusAllele (1))) {

			belongsToLocusToLeft = nextSignal->GetMessageValue (couldBelongToLocusLeft);
			belongsToLocusToRight = nextSignal->GetMessageValue (couldBelongToLocusRight);

			if (belongsToLocusToLeft && belongsToLocusToRight) {

				tempRemove.Append (nextSignal);
				continue;
			}

			location = LocationOfSignal (nextSignal);	// -1 means signal to left of locus; +1 means signal to right of locus; 0 means signal in core

			if (belongsToLocusToRight) {

				if (location < 0) {

					PromoteSignalToAllele (nextSignal);
					nextSignal->SetMessageValue (ambiguousExtendedLocusAssignedToLocus, true);
				}

				else if (location > 0)
					tempRemove.Append (nextSignal);
			}

			else if (belongsToLocusToLeft) {

				if (location > 0) {

					PromoteSignalToAllele (nextSignal);
					nextSignal->SetMessageValue (ambiguousExtendedLocusAssignedToLocus, true);
				}

				else if (location < 0)
					tempRemove.Append (nextSignal);
			}
		}
	}

	while (nextSignal = (DataSignal*)tempRemove.GetFirst ())
		RemoveSignalFromLocusList (nextSignal);
}


//*************************************************************************************************
//*************************************************************************************************
