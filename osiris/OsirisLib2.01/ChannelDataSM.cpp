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
*  FileName: ChannelDataSM.cpp
*  Author:   Robert Goor
*
*/
//
//     Abstract base class for sample data based on file input:  SmartMessage functions only
//

#include "RGTextOutput.h"
#include "OsirisMsg.h"
#include "ChannelData.h"
#include "TestCharacteristic.h"
#include "TracePrequalification.h"
#include "SampleData.h"
#include "CoreBioComponent.h"
#include "rgstring.h"
#include "OutputLevelManager.h"
#include "Notices.h"
#include "ListFunctions.h"
#include "IndividualGenotype.h"
#include "SmartMessage.h"
#include "SmartNotice.h"
#include "STRSmartNotices.h"


// Smart Message functions*******************************************************************************
//*******************************************************************************************************



bool ChannelData :: EvaluateSmartMessagesForStage (int stage, bool allMessages, bool signalsOnly) {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	bool status = true;
	bool evaluateNonSignals = allMessages || (!signalsOnly);
	bool evaluateSignals = allMessages || signalsOnly;

	if (evaluateNonSignals) {

		while (nextLocus = (Locus*) it ())
			nextLocus->EvaluateSmartMessagesForStage (stage);
	}

	if (evaluateSignals) {

		RGDListIterator it2 (SmartPeaks);
		DataSignal* nextSignal;

		while (nextSignal = (DataSignal*) it2 ())
			nextSignal->EvaluateSmartMessagesForStage (stage);
	}

	if (evaluateNonSignals)
		status = SmartMessage::EvaluateAllMessages (mMessageArray, mLocusList, stage, GetObjectScope ());  // ?????? Use PreliminaryCurveList instead??

	return status;
}


bool ChannelData :: EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly) {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	bool status = true;
	bool evaluateNonSignals = allMessages || (!signalsOnly);
	bool evaluateSignals = allMessages || signalsOnly;
	comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) this;
	int topNum = numHigherObjects + 1;
	bool isAmbiguous;
	bool isInterlocus;
	bool isInCoreLadderLocus;
	smPeakInCoreLadderLocus peakInCoreLadderLocus;

	if (evaluateNonSignals) {

		while (nextLocus = (Locus*) it ())
			nextLocus->EvaluateSmartMessagesForStage (comm, topNum, stage);
	}

	if (evaluateSignals) {

		RGDListIterator it2 (SmartPeaks);
		DataSignal* nextSignal;

		while (nextSignal = (DataSignal*) it2 ()) {

			isInCoreLadderLocus = nextSignal->GetMessageValue (peakInCoreLadderLocus);
			isAmbiguous = (nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1)) ||
				(nextSignal->IsPossibleInterlocusAllele (-1) && isInCoreLadderLocus) || (isInCoreLadderLocus && nextSignal->IsPossibleInterlocusAllele (1));
			isInterlocus = (!nextSignal->IsPossibleInterlocusAllele (-1)) && (!nextSignal->IsPossibleInterlocusAllele (1));

			if (isAmbiguous || (isInterlocus && !isInCoreLadderLocus))
				nextSignal->EvaluateSmartMessagesForStage (comm, topNum, stage);
		}
	}

	if (evaluateNonSignals)
		status = SmartMessage::EvaluateAllMessages (comm, topNum, stage, GetObjectScope ());

	return status;
}


bool ChannelData :: SetTriggersForAllMessages (bool* const higherMsgMatrix, int stage, bool allMessages, bool signalsOnly) {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	bool status = true;
	bool evaluateNonSignals = allMessages || (!signalsOnly);
	bool evaluateSignals = allMessages || signalsOnly;

	bool isAmbiguous;
	bool isInterlocus;
	bool isInCoreLadderLocus;
	smPeakInCoreLadderLocus peakInCoreLadderLocus;

	if (evaluateNonSignals) {

		while (nextLocus = (Locus*) it ())
			nextLocus->SetTriggersForAllMessages (mMessageArray, stage, allMessages, signalsOnly);
	}

	//
	// Should we even do this next loop????  Yes, but it's only for signals that are not in loci...
	//

	if (evaluateSignals) {

		RGDListIterator it2 (SmartPeaks);
		DataSignal* nextSignal;

		while (nextSignal = (DataSignal*) it2 ()) {

			isInCoreLadderLocus = nextSignal->GetMessageValue (peakInCoreLadderLocus);
			isAmbiguous = (nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1)) ||
				(nextSignal->IsPossibleInterlocusAllele (-1) && isInCoreLadderLocus) || (isInCoreLadderLocus && nextSignal->IsPossibleInterlocusAllele (1));
			isInterlocus = (!nextSignal->IsPossibleInterlocusAllele (-1)) && (!nextSignal->IsPossibleInterlocusAllele (1));

			if (isAmbiguous || (isInterlocus && !isInCoreLadderLocus))
				nextSignal->SetTriggersForAllMessages (mMessageArray, mMessageDataTable, GetObjectScope (), stage);
		}
	}

	if (evaluateNonSignals)
		status = SmartMessage::SetTriggersForAllMessages (mMessageArray, higherMsgMatrix, stage, GetObjectScope ());

	return status;
}


bool ChannelData :: SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly) {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	bool status = true;
	bool evaluateNonSignals = allMessages || (!signalsOnly);
	bool evaluateSignals = allMessages || signalsOnly;
	comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) this;
	int newNum = numHigherObjects + 1;

	bool isAmbiguous;
	bool isInterlocus;
	bool isInCoreLadderLocus;
	smPeakInCoreLadderLocus peakInCoreLadderLocus;

	if (evaluateNonSignals) {

		while (nextLocus = (Locus*) it ())
			nextLocus->SetTriggersForAllMessages (comm, newNum, stage, allMessages, signalsOnly);
	}

	//
	// Should we even do this next loop????  Yes, but it's only for signals that are not in loci...Now, this is taken care of
	// automatically by the signals themselves, so just run them all.
	//

	if (evaluateSignals) {

		RGDListIterator it2 (SmartPeaks);
		DataSignal* nextSignal;

		while (nextSignal = (DataSignal*) it2 ()) {

			isInCoreLadderLocus = nextSignal->GetMessageValue (peakInCoreLadderLocus);
			isAmbiguous = (nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1)) ||
				(nextSignal->IsPossibleInterlocusAllele (-1) && isInCoreLadderLocus) || (isInCoreLadderLocus && nextSignal->IsPossibleInterlocusAllele (1));
			isInterlocus = (!nextSignal->IsPossibleInterlocusAllele (-1)) && (!nextSignal->IsPossibleInterlocusAllele (1));

			if (isAmbiguous || (isInterlocus && !isInCoreLadderLocus))
				nextSignal->SetTriggersForAllMessages (comm, newNum, stage);
		}
	}

	if (evaluateNonSignals)
		status = SmartMessage::SetTriggersForAllMessages (comm, newNum, stage, GetObjectScope ());

	return status;
}


bool ChannelData :: EvaluateSmartMessagesAndTriggersForStage (SmartMessagingComm& comm, int numHigherObjects, int stage, bool allMessages, bool signalsOnly) {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	bool evaluateNonSignals = allMessages || (!signalsOnly);
	bool evaluateSignals = allMessages || signalsOnly;
	comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) this;
	int topNum = numHigherObjects + 1;

	bool isAmbiguous;
	bool isInterlocus;
	bool isInCoreLadderLocus;
	smPeakInCoreLadderLocus peakInCoreLadderLocus;

	RGDListIterator it2 (SmartPeaks);
	DataSignal* nextSignal;

	if (evaluateSignals) {

		while (nextLocus = (Locus*) it ())
			nextLocus->EvaluateSmartMessagesForStage (comm, topNum, stage, false, true);

		while (nextSignal = (DataSignal*) it2 ()) {

			isAmbiguous = nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1);
			isInterlocus = (!nextSignal->IsPossibleInterlocusAllele (-1)) && (!nextSignal->IsPossibleInterlocusAllele (1));
			isInCoreLadderLocus = nextSignal->GetMessageValue (peakInCoreLadderLocus);

			if (isAmbiguous || (isInterlocus && !isInCoreLadderLocus))
				nextSignal->EvaluateSmartMessagesForStage (comm, topNum, stage);
		}

		it.Reset ();
		it2.Reset ();

		while (nextLocus = (Locus*) it ())
			nextLocus->SetTriggersForAllMessages (comm, topNum, stage, false, true);

		while (nextSignal = (DataSignal*) it2 ()) {

			isAmbiguous = nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1);
			isInterlocus = (!nextSignal->IsPossibleInterlocusAllele (-1)) && (!nextSignal->IsPossibleInterlocusAllele (1));
			isInCoreLadderLocus = nextSignal->GetMessageValue (peakInCoreLadderLocus);

			if (isAmbiguous || (isInterlocus && !isInCoreLadderLocus))
				nextSignal->SetTriggersForAllMessages (comm, topNum, stage);
		}
	}

	if (evaluateNonSignals) {

		it.Reset ();

		while (nextLocus = (Locus*) it ())
			nextLocus->EvaluateSmartMessagesForStage (comm, topNum, stage, false, false);

		it.Reset ();

		while (nextLocus = (Locus*) it ())
			nextLocus->SetTriggersForAllMessages (comm, topNum, stage, false, false);

		SmartMessage::EvaluateAllMessages (comm, topNum, stage, GetObjectScope ());
		SmartMessage::SetTriggersForAllMessages (comm, topNum, stage, GetObjectScope ());
	}

	return true;
}


bool ChannelData :: EvaluateAllReports (bool* const reportMatrix) {

	return SmartMessage::EvaluateAllReports (mMessageArray, reportMatrix, GetObjectScope ());
}


bool ChannelData :: TestAllMessagesForCall () {

	return SmartMessage::TestAllMessagesForCall (mMessageArray, GetObjectScope ());
}


bool ChannelData :: EvaluateAllReportLevels (int* const reportLevelMatrix) {

	return SmartMessage::EvaluateAllReportLevels (mMessageArray, reportLevelMatrix, GetObjectScope ());
}


bool ChannelData :: ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, bool reportLink) {

	if (NumberOfSmartNoticeObjects () > 0) {

		int msgLevel = GetHighestMessageLevelWithRestrictionSM ();

		if (msgLevel < 0)
			return false;

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

		if (mLocusList.Entries () > 0)
			text << "Channel " << mChannel << " Notices:  " << endLine;

		else
			text << "ILS Channel Notices:  " << endLine;

		text.ResetOutputLevel ();

		while (nextNotice = (SmartMessageReporter*) it ())
			text << indent2 << nextNotice->GetMessage () << nextNotice->GetMessageData () << endLine;

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


int ChannelData :: AddAllSmartMessageReporters () {

	int k = GetObjectScope ();
	int size = SmartMessage::GetSizeOfArrayForScope (k);
	int i;
	int nMsgs = 0;
	SmartMessageReporter* newMsg;
	SmartMessage* nextSmartMsg;
	SmartMessageData target;
	SmartMessageData* smd;

	RGDListIterator loci (mLocusList);
	Locus* nextLocus;

	while (nextLocus = (Locus*) loci ())
		nextLocus->AddAllSmartMessageReporters ();

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


int ChannelData :: AddAllSmartMessageReportersForSignals () {

	int nMsgs = 0;

	RGDListIterator signals (SmartPeaks);
	DataSignal* nextSignal;
	smPeakOutsideILS peakOutsideILS;

	while (nextSignal = (DataSignal*) signals ()) {

		//if (!nextSignal->GetMessageValue (peakOutsideILS))
			nMsgs += nextSignal->AddAllSmartMessageReporters ();
	}

	return nMsgs;
}


int ChannelData :: AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects) {

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

	RGDListIterator loci (mLocusList);
	Locus* nextLocus;

	while (nextLocus = (Locus*) loci ())
		nextLocus->AddAllSmartMessageReporters (comm, numHigherObjects);

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

		newMsg->SetPriorityLevel (nextSmartMsg->EvaluateReportLevel (comm, numHigherObjects));
		newMsg->SetRestrictionLevel (nextSmartMsg->EvaluateRestrictionLevel (comm, numHigherObjects));
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


int ChannelData :: AddAllSmartMessageReportersForSignals (SmartMessagingComm& comm, int numHigherObjects) {

	// We have to fix this to include the loci, at least for non-ILS channels

	int nMsgs = 0;

	RGDListIterator signals (SmartPeaks);
	DataSignal* nextSignal;
	smPeakOutsideILS peakOutsideILS;
	smPeakInCoreLadderLocus peakInCoreLadderLocus;

	bool isAmbiguous;
	bool isInCoreLadderLocus;
	bool isInterlocus;

	comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) this;
	int topNum = numHigherObjects + 1;

	RGDListIterator loci (mLocusList);
	Locus* nextLocus;

	while (nextLocus = (Locus*) loci ())
		nextLocus->AddAllSmartMessageReportersForSignals (comm, numHigherObjects);

	while (nextSignal = (DataSignal*) signals ()) {

		//if (!nextSignal->GetMessageValue (peakOutsideILS)) {

			isAmbiguous = nextSignal->IsPossibleInterlocusAllele (-1) && nextSignal->IsPossibleInterlocusAllele (1);
			isInterlocus = (!nextSignal->IsPossibleInterlocusAllele (-1)) && (!nextSignal->IsPossibleInterlocusAllele (1));
			isInCoreLadderLocus = nextSignal->GetMessageValue (peakInCoreLadderLocus);

			if (isAmbiguous || (isInterlocus && !isInCoreLadderLocus))
				nMsgs += nextSignal->AddAllSmartMessageReporters (comm, numHigherObjects);
		//}
	}

	return nMsgs;
}


Boolean ChannelData :: ReportAllSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink) {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	RGString indent2 = indent + "\t";
	bool report = false;

	if (reportLink)
		report = true;

	ReportSmartNoticeObjects (text, indent, delim, report);

	while (nextLocus = (Locus*) it())
		nextLocus->ReportSmartNoticeObjects (text, indent2, delim, report);

	return TRUE;
}



Boolean ChannelData :: ReportXMLSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	RGDListIterator it (*mSmartMessageReporters);
	SmartMessageReporter* nextNotice;
//		text.SetOutputLevel (mHighestMessageLevel);
//		tempText.SetOutputLevel (1);
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

	//if (!text.TestCurrentLevel () && ArtifactList.Entries () == 0) {

	//	text.ResetOutputLevel ();
	//	tempText.ResetOutputLevel ();
	//	return FALSE;
	//}

//		text.ResetOutputLevel ();
	text.SetOutputLevel (1);
	text << "\t\t\t\t<Channel>\n";
	text << "\t\t\t\t\t<ChannelNr>" << mChannel << "</ChannelNr>\n";
	text.ResetOutputLevel ();
	it.Reset ();
	//int msgLevel = GetHighestMessageLevelWithRestrictionSM ();
	int msgLevel;

	//if ((msgLevel > 0) && (msgLevel <= triggerLevel)) {

	while (nextNotice = (SmartMessageReporter*) it ()) {

		viable = nextNotice->HasViableExportInfo ();
		msgLevel = nextNotice->GetMessagePriority ();

		if (((msgLevel > 0) && (msgLevel <= triggerLevel)) || viable) {

			text.SetOutputLevel (1);
			tempText.SetOutputLevel (1);
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
			text.ResetOutputLevel ();
			tempText.ResetOutputLevel ();
		}
	}
	//}

	RGDListIterator art (SmartPeaks);
	DataSignal* nextSignal;
	text.SetOutputLevel (1);
	tempText.SetOutputLevel (1);

	while (nextSignal = (DataSignal*) art ())
		nextSignal->WriteSmartTableArtifactInfoToXML (text, tempText, "\t\t\t\t\t", "Artifact", "meanbps");

	text << "\t\t\t\t</Channel>\n";

	text.ResetOutputLevel ();
	tempText.ResetOutputLevel ();

	return TRUE;
}


Boolean ChannelData :: ReportXMLILSSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

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

		text.SetOutputLevel (1);
		text << "\t\t\t<ILSAlerts>\n";
		it.Reset ();

		while (nextNotice = (SmartMessageReporter*) it ()) {

			viable = nextNotice->HasViableExportInfo ();
			msgLevel = nextNotice->GetMessagePriority ();

			if (((msgLevel > 0) && (msgLevel <= triggerLevel)) || viable) {

				msgNum = Notice::GetNextMessageNumber ();
				nextNotice->SetMessageCount (msgNum);
				text << "\t\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
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

		text.ResetOutputLevel ();
		text << CLevel (1) << "\t\t\t</ILSAlerts>\n";

		text.ResetOutputLevel ();
		tempText.ResetOutputLevel ();
	}

	else
		return FALSE;

	return TRUE;
}


int ChannelData :: CountSignalsWithNoticeSM (const SmartNotice& target, ChannelData* laneStd) {

	int count = 0;
	RGDListIterator it (SmartPeaks);
	DataSignal* nextSignal;
	double minMean = laneStd->GetFirstAnalyzedMean ();
	double maxMean = laneStd->GetLastAnalyzedMean ();
	double mean;

	while (nextSignal = (DataSignal*) it()) {

		mean = nextSignal->GetMean ();

		if (!CoreBioComponent::SignalIsWithinAnalysisRegion (nextSignal, minMean))	// modified 03/13/2015
			continue;

		if (mean > maxMean)
			break;

		if (nextSignal->GetMessageValue (target))
			count++;
	}

	return count;
}


void ChannelData :: OutputDebugID (SmartMessagingComm& comm, int numHigherObjects) {

	RGString idData;
	int higherObjectIndex = numHigherObjects - 2;
	SmartMessagingObject* higherObj = comm.SMOStack [higherObjectIndex];
	higherObj->OutputDebugID (comm, higherObjectIndex + 1);
	idData << "\tChannel Number:  " << mChannel;
	SmartMessage::OutputDebugString (idData);
}


RGString ChannelData :: GetDebugIDIndent () const {

	return "\t";
}


void ChannelData :: ChannelIsPositiveControlSM (bool isPosControl) {

	//
	//  This is sample stage 1
	//

	Locus* nextLocus;
	RGDListIterator it (mLocusList);
	smChannelIsPosControl channelIsPosControl;
	smLocusIsPosControl locusIsPosControl;

	while (nextLocus = (Locus*) it ())
		nextLocus->SetMessageValue (locusIsPosControl, isPosControl);

	SetMessageValue (channelIsPosControl, isPosControl);
}


void ChannelData :: ChannelIsNegativeControlSM (bool isNegControl) {

	//
	//  This is sample stage 1
	//

	Locus* nextLocus;
	RGDListIterator it (mLocusList);
	smChannelIsNegControl channelIsNegControl;
	smLocusIsNegControl locusIsNegControl;

	while (nextLocus = (Locus*) it ())
		nextLocus->SetMessageValue (locusIsNegControl, isNegControl);

	SetMessageValue (channelIsNegControl, isNegControl);
}


void ChannelData :: MakePreliminaryCallsSM (bool isNegCntl, bool isPosCntl, GenotypesForAMarkerSet* pGenotypes) {

	//
	//  This is sample stage 3
	//

	Locus* nextLocus;
	RGDListIterator it (mLocusList);
	RGDList discardList;
	smResidualDisplacementTestPreset residualDisplacementTestPreset;

	while (nextLocus = (Locus*) it ()) {

		nextLocus->CallAllelesSM (isNegCntl, pGenotypes, ArtifactList, PreliminaryCurveList, CompleteCurveList);
		// This is what it used to be:  nextLocus->CallAlleles (CompleteCurveList, isNegCntl, pGenotypes, ArtifactList, PreliminaryCurveList);
	}

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->GetLargestPeak ();

	if (GetMessageValue (residualDisplacementTestPreset)) {

		it.Reset ();

		while (nextLocus = (Locus*) it ())
			nextLocus->TestResidualDisplacement ();
	}

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->PromoteOwnCoreSignalsToAllelesAndRemoveOthers (PreliminaryCurveList);

	//cout << "Promoted core signals to alleles" << endl;

	//cout << "Testing for multisignals" << endl;
	// The code below commented out on 01/29/2016 because, with new pull-up algorithms, we should not be reviewing prior decisions about craters/sigmoidal pull-ups...or not
	//TestForMultiSignalsSM ();

	TestForAlleleDuplicationSM ();
	it.Reset ();

	//cout << "Channel multisignals tested" << endl;

	while (nextLocus = (Locus*) it ())
		nextLocus->TestForDuplicateAllelesSM (ArtifactList, PreliminaryCurveList, CompleteCurveList, SmartPeaks, pGenotypes); // Replaces TestForMultiSignalsSM

	//cout << "Locus multisignals tested" << endl;

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->PromoteOwnCoreSignalsToAllelesAndRemoveOthers (PreliminaryCurveList);

	//cout << "Promoted core signals to alleles again!" << endl;

	TestProximityArtifactsSM ();
	TestForInterlocusProximityArtifactsSM ();
	//TestSignalsForOffScaleSM ();	//  Commented out 07/25/2014 to test moving it right after cross channel analysis for smarter pull-up analysis

	//cout << "Tested proximity artifacts" << endl;

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->TestForAcceptedTriAllele (isNegCntl, isPosCntl, pGenotypes);

	it.Reset ();

	while (nextLocus = (Locus*) it ())
		nextLocus->PromoteNonAmbiguousSignalsToAlleles (PreliminaryCurveList);
}


int ChannelData :: SetDataSM (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	mData = fileData.GetDataSignalForDataChannel (mFsaChannel);
	smFSAChannelUnreadable fsaUnreadable;
	RGString noticeInfo;

	if (mData == NULL) {

		ErrorString = "Could not get sample data signal from file for channel ";
		ErrorString << mChannel << "\n";
		SetMessageValue (fsaUnreadable, true);
		return -1;
	}

	if (mChannel == 1)
		CoreBioComponent::SetOffScaleDataLength (mData->GetNumberOfSamples ());

	FinalCurveList.Clear ();
	MarginalCurveList.Clear ();
	ArtifactList.Clear ();
	BleedThroughCandidateList.Clear ();
	PreliminaryCurveList.Clear ();
	CompleteCurveList.ClearAndDelete ();

	return 0;
}


int ChannelData :: SetRawDataSM (SampleData& fileData, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1
	//

	mData = fileData.GetRawDataSignalForDataChannel (mFsaChannel);
	smFSAChannelUnreadable fsaUnreadable;
	RGString noticeInfo;

	if (mData == NULL) {

		ErrorString = "Could not get sample data signal from file for channel ";
		ErrorString << mChannel << "\n";
		SetMessageValue (fsaUnreadable, true);
		return -1;
	}

	if (mChannel == 1)
		CoreBioComponent::SetOffScaleDataLength (mData->GetNumberOfSamples ());

	FinalCurveList.Clear ();
	MarginalCurveList.Clear ();
	ArtifactList.Clear ();
	BleedThroughCandidateList.Clear ();
	PreliminaryCurveList.Clear ();
	CompleteCurveList.ClearAndDelete ();

	return 0;
}


int ChannelData :: SetRawDataFromColorCorrectedArraySM (double* dataArray, int arraySize, TestCharacteristic* testConrolPeak, TestCharacteristic* testSamplePeak) {

	//
	//  This is ladder and sample stage 1; by default, dataArray will be deleted when mData is deleted so dataArray must be "deletable"
	//

	smFSAChannelUnreadable fsaUnreadable;
	RGString noticeInfo;

	if (dataArray == NULL) {

		ErrorString = "Could not get sample data signal from file for channel ";
		ErrorString << mChannel << "\n";
		SetMessageValue (fsaUnreadable, true);
		mData = NULL;
		return -1;
	}

	double spacing = 1.0;
	mData = new SampledData (arraySize, 0.0, (arraySize - 1) * spacing, dataArray);
//	mData = fileData.GetRawDataSignalForDataChannel (mFsaChannel);

	if (mChannel == 1)
		CoreBioComponent::SetOffScaleDataLength (mData->GetNumberOfSamples ());

	FinalCurveList.Clear ();
	MarginalCurveList.Clear ();
	ArtifactList.Clear ();
	BleedThroughCandidateList.Clear ();
	PreliminaryCurveList.Clear ();
	CompleteCurveList.ClearAndDelete ();

	return 0;
}


void ChannelData :: ClearAllPeaksBelowAnalysisThreshold () {

	smBelowMinRFU belowMinRFU;
	DataSignal* nextSignal;
	RGDListIterator it (PreliminaryCurveList);

	while (nextSignal = (DataSignal*) it ()) {

		if (nextSignal->GetMessageValue (belowMinRFU))
			it.RemoveCurrentItem ();
	}
}


int ChannelData :: TestFitCriteriaSM (DataSignal* signal) {

	//
	//  This is ladder and sample stage 1
	//

	double minAcceptableFit = ParametricCurve::GetMinimumFitThreshold ();
	double minFitForArtifactTest = ParametricCurve::GetTriggerForArtifactTest ();
	smCurveFitFailed fitUnacceptable;
	smCurveFitMarginal fitMarginal;
	double fit = signal->GetCurveFit ();

	if (fit < minAcceptableFit)
		signal->SetMessageValue (fitUnacceptable, true);

	else if (fit < minFitForArtifactTest)
		signal->SetMessageValue (fitMarginal, true);

	return 0;
}


int ChannelData :: FitAllCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	// This code should be moved to STRChannelData...it's not generic enough for ChannelData

	//
	//  This is ladder and sample stage 1
	//
	
	STRTracePrequalification trace;
	DataSignal* nextSignal;
	double fit;
	int TestResult;
	DataSignal* signature;
	double secondaryContent;
	double minRFU = GetMinimumHeight ();
	double maxRFU = GetMaximumHeight ();
	double detectionRFU = GetDetectionThreshold ();
	double minAcceptableFit = ParametricCurve::GetMinimumFitThreshold ();
	double minFitForArtifactTest = ParametricCurve::GetTriggerForArtifactTest ();
	double minFit = minFitForArtifactTest;
	double absoluteMinFit = ParametricCurve::GetAbsoluteMinimumFit ();

	if (minAcceptableFit > minFit)
		minFit = minAcceptableFit;

	if (CoreBioComponent::GetGaussianSignature ())
		signature = new NormalizedGaussian (0.0, ParametricCurve::GetSigmaForSignature ());

	else
		signature = new DoubleGaussian (0.0, ParametricCurve::GetSigmaForSignature ());

	ArtifactList.Clear ();
	MarginalCurveList.Clear ();
	FinalCurveList.Clear ();
	msg.ResetMessage ();
	PreliminaryCurveList.Clear ();
	CompleteCurveList.ClearAndDelete ();
	double lineFit;

	mData->ResetCharacteristicsFromRight (trace, text, detectionRFU, print);

	Endl endLine;
	ExcelText.SetOutputLevel (1);
	ExcelText << "Using minimum RFU = " << minRFU << endLine;
	ExcelText.ResetOutputLevel ();
	int dualReturn;
	double absoluteMinFitLessEpsilon = absoluteMinFit - 0.01;

	double constantHeight;
	int leftEndPoint;
	int rightEndPoint;
	RaisedBaseLineData* rbld;
	mRaisedBaseLines.clearAndDestroy ();

	while (nextSignal = mData->FindNextCharacteristicFromRight (*signature, fit, CompleteCurveList)) {

		secondaryContent = fabs(nextSignal->GetScale (2));
		double mean = nextSignal->GetMean ();
		lineFit = mData->TestConstantCharacteristicRetry (constantHeight, leftEndPoint, rightEndPoint);

		if (lineFit > minFitForArtifactTest) {

			rbld = new RaisedBaseLineData (constantHeight, leftEndPoint, rightEndPoint);
			mRaisedBaseLines.prepend (rbld);
			delete nextSignal;
			continue;
		}

		//if (nextSignal->GetStandardDeviation () > 0.0) {

		//	if (nextSignal->GetMean () < nextSignal->LeftEndPoint () + 0.2 * nextSignal->GetStandardDeviation ()) {

		//		delete nextSignal;
		//		continue;
		//	}
		//}

		TestFitCriteriaSM (nextSignal);

		//if ((!nextSignal->IsUnimodal ()) || (fit < absoluteMinFit) || nextSignal->MayBeUnacceptable () || mData->HasAtLeastOneLocalMinimum ()) {
		if ((!nextSignal->IsUnimodal ()) || (fit < absoluteMinFit) || nextSignal->MayBeUnacceptable () || (TestForDualSignal && mData->HasAtLeastOneLocalMinimum ()) || (TestForDualSignal && mData->TestForBiasedFit (nextSignal, minRFU))) {

			dualReturn = TestForDualPeakSM (minRFU, maxRFU, nextSignal, fit, CompleteCurveList, true);

			if (dualReturn <= 0)
				TestForArtifactsSM (nextSignal, absoluteMinFitLessEpsilon);

			continue;
		}

		if (secondaryContent > 0.9 * nextSignal->Peak ()) {

			TestForArtifactsSM (nextSignal, fit);
			continue;
		}

		if (fit < minFit) {

			dualReturn = TestForDualPeakSM (minRFU, maxRFU, nextSignal, fit, CompleteCurveList);

			if (dualReturn <= 0)
				TestForArtifactsSM (nextSignal, fit);

			continue;
		}

		else {  // nextSignal is acceptable for now, so add it to the CurveList

			PreliminaryCurveList.Prepend (nextSignal);
			CompleteCurveList.Prepend (nextSignal);
//			i++;
		}		
	}   //  We are done finding characteristics

	RGDListIterator it (PreliminaryCurveList);

	while (nextSignal = (DataSignal*) it ()) {

//		TestResult = mTestPeak->TestSM (nextSignal, minRFU, maxRFU);
		TestResult = mTestPeak->TestSM (nextSignal, detectionRFU, minRFU, maxRFU);

		if (TestResult < 0) {

			it.RemoveCurrentItem ();

			if (TestResult != -20) {

//				ArtifactList.InsertWithNoReferenceDuplication (nextSignal);
				nextSignal->ClearSmartNoticeObjects ();
			}
		}
	}

	DataSignal* prevSignal = NULL;
	RGDList tempList;
	prevSignal = (DataSignal*)PreliminaryCurveList.GetFirst ();
	double minDistance = ChannelData::GetMinimumDistanceBetweenPeaks ();

	while (nextSignal = (DataSignal*) PreliminaryCurveList.GetFirst ()) {

		if (prevSignal != NULL) {

			if (fabs(prevSignal->GetMean () - nextSignal->GetMean ()) < minDistance) {

				// "get rid" of the one that fits least well and use the other for the next test.
				// later, if we want, we can add redundant signal to artifact list with a notice...

				if (prevSignal->GetCurveFit () > nextSignal->GetCurveFit ()) {

					// keep prevSignal and "lose" nextSignal
					CompleteCurveList.RemoveReference (nextSignal);
		//			ArtifactList.RemoveReference (nextSignal);
					delete nextSignal;
					continue;
				}

				else {

					CompleteCurveList.RemoveReference (prevSignal);
		//			ArtifactList.RemoveReference (prevSignal);
					delete prevSignal;
					prevSignal = nextSignal;
					continue;
				}
			}

			else {

				tempList.Append (prevSignal);
				prevSignal = nextSignal;
			}
		}
	}

	if (prevSignal != NULL)
		tempList.Append (prevSignal);

	while (nextSignal = (DataSignal*) tempList.GetFirst ())
		PreliminaryCurveList.Append (nextSignal);

	delete signature;
//	ProjectNeighboringSignalsAndTest (1.0, 1.0);
	return 0;
}


int ChannelData :: FitAllNegativeCharacteristicsSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	return 0;
}


int ChannelData :: AssignSampleCharacteristicsToLociSM (CoreBioComponent* grid, CoordinateTransform* timeMap) {

	return -1;
}


int ChannelData :: AnalyzeLaneStandardChannelRecursivelySM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	return -1;
}


Boolean ChannelData :: AssignCharacteristicsToLociSM (ChannelData* lsData) {

	//
	//  This is ladder stage 1
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	Boolean returnValue = TRUE;

	//  All the characteristics we are looking for are in the PreliminaryCurveList

	while (nextLocus = (Locus*) it()) {

		if (!nextLocus->ExtractGridSignalsSM (PreliminaryCurveList, mLaneStandard, lsData)) {

			ErrorString << nextLocus->GetErrorString ();
			returnValue = FALSE;
		}
	}

	if (!returnValue)
		ErrorString << "Channel number " << mChannel << " could not assign characteristics to loci.  Skipping...\n";

	return returnValue;
}


int ChannelData :: AnalyzeGridLociSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	return -1;
}


int ChannelData :: AnalyzeSampleLociSM (ChannelData* lsData, RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	return -1;
}


int ChannelData :: TestFractionalFiltersSM () {

	return -1;
}


int ChannelData :: FinalTestForPeakSizeAndNumberSM (double averageHeight, Boolean isNegCntl, Boolean isPosCntl, GenotypesForAMarkerSet* pGenotypes) {

	return -1;
}


int ChannelData :: FinalTestForCriticalLaneStandardNoticesSM () {

	return -1;
}


int ChannelData :: HierarchicalLaneStandardChannelAnalysisSM (RGTextOutput& text, RGTextOutput& ExcelText, OsirisMsg& msg, Boolean print) {

	return -1;
}


bool ChannelData :: TestForArtifactsSM (DataSignal* currentSignal, double fit) {

	//
	//  This is ladder and sample stage 1
	//

	NormalizedSuperGaussian BlobSignature3 (0.0, ParametricCurve::GetSigmaForSignature (), 6);
	NormalizedGaussian GaussianSignature (0.0, ParametricCurve::GetSigmaForSignature ());
	NormalizedSuperGaussian JustSuperGaussianSignature (0.0, ParametricCurve::GetSigmaForSignature (), 3);
	DataSignal* TestSignal;
	smCurveFitFailed fitFailed;
//	double fit3;
	bool rtn;
	double absoluteMinFit = ParametricCurve::GetAbsoluteMinimumFit ();
	double minFitForArtifactTest = ParametricCurve::GetTriggerForArtifactTest ();

	DataSignal* blobSignal;
	DataSignal* gaussianSignal;
	DataSignal* justSuperSignal;

	double blobFit;
	double gaussianFit;
	double justSuperFit;

	DataSignal* signalArray [3];
	double fitArray [3];
	char* idArray [3];
	int index = 0;

	smBlob blob;

	// Later...add test for spikes

	blobSignal = mData->FindNextCharacteristicRetry (BlobSignature3, blobFit, CompleteCurveList);
	gaussianSignal = mData->FindNextCharacteristicRetry (GaussianSignature, gaussianFit, CompleteCurveList);
	justSuperSignal = mData->FindNextCharacteristicRetry (JustSuperGaussianSignature, justSuperFit, CompleteCurveList);

	if ((blobSignal != NULL) && (blobFit > fit) && (blobSignal->GetStandardDeviation () >= 6.0) && (blobFit >= absoluteMinFit) && (!blobSignal->MayBeUnacceptable ())) {
	
		signalArray [0] = blobSignal;
		fitArray [0] = blobFit;
		idArray [0] = "Dye blob";
		blobSignal->SetMessageValue (blob, true);
	//	cout << "Possible dye blob found at " << blobSignal->GetMean () << " with fit = " << blobFit << " compared with original fit = " << fit << endl;
		index++;
	}

	else
		delete blobSignal;

	if ((gaussianSignal != NULL) && (gaussianFit > fit) && (gaussianFit >= absoluteMinFit) && (!gaussianSignal->MayBeUnacceptable ())) {

		signalArray [index] = gaussianSignal;
		fitArray [index] = gaussianFit;
		idArray [index] = "Simple Gaussian";
		index++;
	}

	else
		delete gaussianSignal;

	if ((justSuperSignal != NULL) && (justSuperFit > fit) && (justSuperFit >= absoluteMinFit) && (!justSuperSignal->MayBeUnacceptable ())) {

		signalArray [index] = justSuperSignal;
		fitArray [index] = justSuperFit;
		idArray [index] = "Degree 3 Super Gaussian";
		index++;
	}

	else
		delete justSuperSignal;

	double maxFit = 0.0;
	int i;
	int maxIndex = -1;

	for (i=0; i<index; i++) {

		if (fitArray [i] > maxFit) {

			maxFit = fitArray [i];
			maxIndex = i;
		}
	}

	if (maxIndex >= 0) {

		delete currentSignal;
		TestSignal = signalArray [maxIndex];
		rtn = true;

		CompleteCurveList.InsertWithNoReferenceDuplication (TestSignal);
		PreliminaryCurveList.InsertWithNoReferenceDuplication (TestSignal);
		TestFitCriteriaSM (TestSignal);
	//	cout << idArray [maxIndex] << " found at " << TestSignal->GetMean () << " with fit = " << fitArray [maxIndex] << " xcompared with original fit = " << fit << endl;

		for (i=0; i<index; i++) {

			if (i != maxIndex)
				delete signalArray [i];
		}
	}

	else {

		if (fit >= absoluteMinFit) {

			CompleteCurveList.InsertWithNoReferenceDuplication (currentSignal);
			PreliminaryCurveList.InsertWithNoReferenceDuplication (currentSignal);  // This is a test!  We keep poor fits in case they are needed later, at which point we point them out.
			rtn = false;
		}

		else {

			int location;
			double height = mData->GetModeHeightAndLocationFromDataInterval (location);
			PoorFitPeakData* pfpd = new PoorFitPeakData (height, location);
			mPoorFits.prepend (pfpd);
			delete currentSignal;
			currentSignal = new Gaussian ((double)location, 2.0);
			currentSignal->SetPeak (height);
			currentSignal->SetMessageValue (fitFailed, true);
			currentSignal->SetDataMode (height);
			currentSignal->SetCurveFit (0.795);
			CompleteCurveList.InsertWithNoReferenceDuplication (currentSignal);
			PreliminaryCurveList.InsertWithNoReferenceDuplication (currentSignal);  // This is a test!  We keep poor fits in case they are needed later, at which point we point them out.
			rtn = true;
		}				
	}

	return rtn;



	//
	//
	//TestSignal3 = mData->FindNextCharacteristicRetry (BlobSignature3, fit3, CompleteCurveList);

	//if ((TestSignal3 != NULL) && (fit3 > fit) && (TestSignal3->GetStandardDeviation () >= 6.0) && (fit3 >= absoluteMinFit) && (!TestSignal3->MayBeUnacceptable ())) {

	//	// it's a dye blob!  NOTE!!!  WE MAY HAVE TO CHANGE THIS WHEN WE USE RAW DATA!!!

	////	ArtifactList.Prepend (TestSignal3);	// Need this????????????????????????????????????????????????????
	//	TestSignal3->SetMessageValue (blob, true);
	//	TestSignal3->AddNoticeToList (1, "", "Suspected dye blob");
	//	CompleteCurveList.Prepend (TestSignal3);
	//	cout << "Dye blob found at " << TestSignal3->GetMean () << " with fit = " << fit3 << " compared with original fit = " << fit << endl;
	//	delete currentSignal;
	//	TestFitCriteriaSM (TestSignal3);
	//	rtn = true;
	//}

	//else {

	//	delete TestSignal3;
	//	TestSignal3 = mData->FindNextCharacteristicRetry (GaussianSignature, fit3, CompleteCurveList);

	//	if ((TestSignal3 != NULL) && (fit3 > fit) && (fit3 >= absoluteMinFit) && (!TestSignal3->MayBeUnacceptable ())) {

	//		// It's Gaussian and not an artifact
	//		CompleteCurveList.InsertWithNoReferenceDuplication (TestSignal3);
	//		PreliminaryCurveList.InsertWithNoReferenceDuplication (TestSignal3);
	//		delete currentSignal;
	//		TestFitCriteriaSM (TestSignal3);
	//		rtn = true;
	//	}

	//	else {

	//		delete TestSignal3;
	//		TestSignal3 = mData->FindNextCharacteristicRetry (JustSuperGaussianSignature, fit3, CompleteCurveList);

	//		if ((TestSignal3 != NULL) && (fit3 > fit) && (fit3 >= absoluteMinFit) && (!TestSignal3->MayBeUnacceptable ())) {

	//			// It's barely super Gaussian and not an artifact
	//			CompleteCurveList.InsertWithNoReferenceDuplication (TestSignal3);
	//			PreliminaryCurveList.InsertWithNoReferenceDuplication (TestSignal3);
	//			TestFitCriteriaSM (TestSignal3);
	//			delete currentSignal;
	//			rtn = true;
	//		}

	//		else {

	//			delete TestSignal3;

	//			if (fit >= absoluteMinFit) {

	//				CompleteCurveList.InsertWithNoReferenceDuplication (currentSignal);
	//				PreliminaryCurveList.InsertWithNoReferenceDuplication (currentSignal);  // This is a test!  We keep poor fits in case they are needed later, at which point we point them out.
	//	//			ArtifactList.InsertWithNoReferenceDuplication (currentSignal);	// Need this????????????????????????????????????????????????????
	//				rtn = false;
	//			}

	//			else {

	//				int location;
	//				double height = mData->GetModeHeightAndLocationFromDataInterval (location);
	//				PoorFitPeakData* pfpd = new PoorFitPeakData (height, location);
	//				mPoorFits.prepend (pfpd);
	//				delete currentSignal;
	//				rtn = true;
	//			}				
	//		}
	//	}
	//}

	//return rtn;
}


bool ChannelData :: TestForArtifactsSM (DataSignal* currentSignal, double fit, int dualSignal) {

	//
	//  Stage 1
	//

	if (dualSignal == 0)
		return TestForArtifactsSM (currentSignal, fit);

	NormalizedSuperGaussian BlobSignature3 (0.0, ParametricCurve::GetSigmaForSignature (), 6);
	DataSignal* TestSignal3;
	double fit3;
	bool rtn;
	double absoluteMinFit = ParametricCurve::GetAbsoluteMinimumFit ();

	smBlob blob;

	// Later...add test for spikes

	TestSignal3 = mData->FindNextCharacteristicRetry (BlobSignature3, fit3, CompleteCurveList, dualSignal);

	if ((TestSignal3 != NULL) && (fit3 > fit) && (TestSignal3->GetStandardDeviation () >= 6.0) && (fit3 >= absoluteMinFit)) {

		// it's a dye blob!  NOTE!!!  WE MAY HAVE TO CHANGE THIS WHEN WE USE RAW DATA!!!

//		ArtifactList.Prepend (TestSignal3);	// Need this????????????????????????????????????????????????????
		PreliminaryCurveList.Prepend (TestSignal3);
		TestSignal3->SetMessageValue (blob, true);
		TestSignal3->AddNoticeToList (1, "", "Suspected dye blob");
		CompleteCurveList.Prepend (TestSignal3);
		TestFitCriteriaSM (TestSignal3);
		delete currentSignal;
		rtn = true;
	}

	else {

		delete TestSignal3;
		
		if (fit >= absoluteMinFit) {

			CompleteCurveList.Prepend (currentSignal);
			PreliminaryCurveList.Prepend (currentSignal);  // This is a test!  We keep poor fits in case they are needed later, at which point we point them out.
	//		ArtifactList.Prepend (currentSignal);
			rtn = false;
		}

		else {

			int location;
			double height = mData->GetModeHeightAndLocationFromDataInterval (location);
			PoorFitPeakData* pfpd = new PoorFitPeakData (height, location);
			mPoorFits.prepend (pfpd);
			delete currentSignal;
			rtn = true;
		}
	}

	return rtn;

}


int ChannelData :: TestForDualPeakSM (double minRFU, double maxRFU, DataSignal* currentSignal, double currentFit, RGDList& previous, bool originalUnacceptable) {

	//
	//  This is ladder and sample stage 1
	//

	//if (!TestForDualSignal)
	//	return -1;

	DualDoubleGaussian signature;
	double fit;
	DualDoubleGaussian* TestDual = (DualDoubleGaussian*) mData->FindNextCharacteristicRetry (signature, fit, previous);
	DataSignal* leftSignal;
	DataSignal* rightSignal;
	double secondaryContent;
	int rtnValue = 2;
	double minAcceptableFit = ParametricCurve::GetMinimumFitThreshold ();
	double minFitForArtifactTest = ParametricCurve::GetTriggerForArtifactTest ();
	double minFit = minFitForArtifactTest;
	double absoluteMinFit = ParametricCurve::GetAbsoluteMinimumFit ();
	smPartOfDualSignal partOfDualSignal;

	if (minAcceptableFit > minFit)
		minFit = minAcceptableFit;

	if (TestDual != NULL) {

		leftSignal = TestDual->GetFirstCurve ();
		rightSignal = TestDual->GetSecondCurve ();

		if ((leftSignal == NULL) || (rightSignal == NULL)) {

			delete leftSignal;
			delete rightSignal;
			delete TestDual;
			return -1;
		}

		leftSignal->SetMessageValue (partOfDualSignal, true);
		rightSignal->SetMessageValue (partOfDualSignal, true);
		double leftMean = leftSignal->GetMean ();
		double rightMean = rightSignal->GetMean ();
	//	double delta = rightMean - leftMean;
		double currentMean = currentSignal->GetMean ();
		bool biased = false;
	//	double ratio = (currentMean - leftMean) / delta;

		if ((currentMean <= leftMean + 0.5) || (currentMean >= rightMean - 0.5))
			biased = true;

		//else if ((ratio <= 0.2) || (ratio >= 0.8))
		//	biased = true;

		if ((!originalUnacceptable) && (rightSignal->GetCurveFit () < currentFit) && (leftSignal->GetCurveFit () < currentFit)  && (!biased)) {

			delete leftSignal;
			delete rightSignal;
			delete TestDual;
			return -1;
		}

		secondaryContent = fabs(rightSignal->GetScale (2));

		if ((!biased) && ((!rightSignal->IsUnimodal ()) || (secondaryContent > 0.9 * rightSignal->Peak ()) || (rightSignal->GetCurveFit () < absoluteMinFit))) {

			delete rightSignal;
			rightSignal = NULL;
			rtnValue--;
		}

		else {

			fit = rightSignal->GetCurveFit ();
			TestFitCriteriaSM (rightSignal);

			if (fit < minFit) {

				// In this case, the fit is totally unacceptable or marginal, so try for alternate signatures
				if (TestForArtifactsSM (rightSignal, fit, 2)) {

					rightSignal = NULL;
					rtnValue--;
				}
			}

			else {  // rightSignal is acceptable for now, so add it to the CurveList

				PreliminaryCurveList.Prepend (rightSignal);
				CompleteCurveList.Prepend (rightSignal);
			}		
		}

		secondaryContent = fabs(leftSignal->GetScale (2));

		if ((!biased) && ((!leftSignal->IsUnimodal ()) || (secondaryContent > 0.9 * leftSignal->Peak ()) || (leftSignal->GetCurveFit () < absoluteMinFit))) {

			delete leftSignal;
			leftSignal = NULL;
			rtnValue--;
		}

		else {

			fit = leftSignal->GetCurveFit ();
			TestFitCriteriaSM (leftSignal);

			if (fit < minFit) {

				// In this case, the fit is totally unacceptable or marginal, so try for alternate signatures
				if (TestForArtifactsSM (leftSignal, fit, 1)) {

					leftSignal = NULL;
					rtnValue--;
				}
			}

			else {  // leftSignal is acceptable for now, so add it to the CurveList

				PreliminaryCurveList.Prepend (leftSignal);
				CompleteCurveList.Prepend (leftSignal);
			}		
		}
	}

	else
		rtnValue = -1;

	if (rtnValue > 0) {

		delete currentSignal;

		if (IsControlChannel () && (leftSignal != NULL) && (rightSignal != NULL)) {

			if (leftSignal->Peak () >= rightSignal->Peak ()) {

				PreliminaryCurveList.RemoveReference (rightSignal);
				CompleteCurveList.RemoveReference (rightSignal);
				delete rightSignal;
			}

			else {

				PreliminaryCurveList.RemoveReference (leftSignal);
				CompleteCurveList.RemoveReference (leftSignal);
				delete leftSignal;
			}
		}
	}

	delete TestDual;
	return rtnValue;
}


int ChannelData :: TestArtifactListForNoticesWithinLaneStandardSM (ChannelData* laneStandard, CoreBioComponent* associatedGrid) {

	// Look for critical level notices in SmartPeaks list on channel.  This list should only consist of peaks within lane standard
	// and above user specified min BioID for reporting artifacts

	//
	//  This is ladder and sample stage 5
	//

	RGDListIterator it (SmartPeaks);
	DataSignal* nextSignal;
	double bp;
	int ibp;
	int msgLevel;
	int trigger = Notice::GetMessageTrigger ();
	bool isCore;

	smChannelPeaksHaveCriticalArtifacts criticalArtifacts;

	while (nextSignal = (DataSignal*) it ()) {

		isCore = (nextSignal->GetLocus (0) != NULL);

		if (nextSignal->IsPossibleInterlocusAllele (1) || nextSignal->IsPossibleInterlocusAllele (-1) || isCore)
			continue;

		msgLevel = nextSignal->GetHighestMessageLevelWithRestrictionSM ();

		if ((msgLevel > 0) && (msgLevel <= trigger)) {

			bp = nextSignal->GetApproximateBioID ();
			ibp = (int) floor (bp + 0.5);
			SetMessageValue (criticalArtifacts, true);
			AppendDataForSmartMessage (criticalArtifacts, ibp);
		}
	}

	return 0;
}


int ChannelData :: TestForMultiSignalsSM () {

	return -1;
}


int ChannelData :: TestForAlleleDuplicationSM () {

	return -1;
}



int ChannelData :: TestProximityArtifactsSM () {

	return -1;
}



int ChannelData :: AssignSignalToFirstLocusAndDeleteFromSecondSM (DataSignal* target, Locus* locus1, Locus* locus2) {

	return -1;
}


int ChannelData :: RemoveSignalFromBothLociSM (DataSignal* target, Locus* locus1, Locus* locus2) {

	return -1;
}


bool ChannelData :: TestIfSignalBelongsToFirstLocusSM (DataSignal* target, Locus* locus1, Locus* locus2) {

	return false;
}


int ChannelData :: ResolveAmbiguousInterlocusSignalsSM () {

	return -1;
}


int ChannelData :: MeasureInterlocusSignalAttributesSM () {

	return -1;
}


int ChannelData :: ResolveAmbiguousInterlocusSignalsUsingSmartMessageDataSM () {

	return -1;
}


int ChannelData :: RemoveSignalsOutsideLaneStandardSM (ChannelData* laneStandard) {

	//
	//  This is ladder and sample stage 1
	//

	//	This function removes peaks to left of ILS or specified minimum BioID (ILS) for reporting artifacts.  The purpose is to minimize
	// the effects of the primer peaks on the reported artifacts and alleles of a sample.

	RGDListIterator it (PreliminaryCurveList);
	DataSignal* nextSignal;
	double left = laneStandard->GetFirstAnalyzedMean ();
	double right = laneStandard->GetLastAnalyzedMean ();
	double mean;
	smPeakOutsideILS peakOutsideILS;
	smPeakToRightOfILS peakToRightOfILS;
	smTestRelativeBaselinePreset testRelativeBaselinePreset;
	double reportMin = (double) CoreBioComponent::GetMinBioIDForArtifacts ();
	double reportMinTime;
	
	// if..else clause below modified 03/13/2015
	if (reportMin > 0.0)
		reportMinTime = laneStandard->GetTimeForSpecifiedID (reportMin);

	else
		reportMinTime = -1.0;

	TestForRaisedBaselineAndExcessiveNoiseSM (left, reportMinTime);

	while (nextSignal = (DataSignal*) it())
		nextSignal->TestSignalGroupsWithinILS (left, right, reportMin);	// This doesn't do anything as of 03/13/2015

	it.Reset ();

	while (nextSignal = (DataSignal*) it()) {

		mean = nextSignal->GetMean ();

		if (!CoreBioComponent::SignalIsWithinAnalysisRegion (nextSignal, left)) {	// modified 03/13/2015

			it.RemoveCurrentItem ();
			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", "Signal lies outside internal lane standard interval");
			nextSignal->SetMessageValue (peakOutsideILS, true);
			continue;
		}

		else if (mean < left) {	// added 03/13/2015

			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", "Signal lies outside internal lane standard interval");
			nextSignal->SetMessageValue (peakOutsideILS, true);
			//continue;
		}

		if (mean > right) {

			nextSignal->AddNoticeToList (OutputLevelManager::PeakOutsideLaneStandard, "", "Signal lies to right of internal lane standard interval");
			nextSignal->SetMessageValue (peakToRightOfILS, true);
		}

		if (!nextSignal->DontLook ())
			SmartPeaks.Append (nextSignal);
	}

	AnalyzeDynamicBaselineSM ((int) left, reportMinTime);

	return 0;
}


bool ChannelData :: FindLimitsOnPrimaryPullupPeaks () {

	return false;
}


bool ChannelData :: ValidateAndCorrectCrossChannelAnalysisSM () {

	return false;
}


int ChannelData :: CorrectCrossChannelAnalysesSM (bool isNegCntl) {

	//
	// Do we need this method at all.  Need this???????????????????????????????????????????????????????????????????????????????????
	// Everything can be done within SmartMessage itself!!!  Maybe just need to perform a stage evaluation and then collect "garbage"?
	//

	//
	//  This is sample stage 4
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	//StutterFound stutter;
	//PullUpFound pullup;
	//AdenylationFound adenylation;
	//PullUpPrimaryChannel primary;
	//RemovablePrimaryPullUp removable;

	//SignalPeakBelowFractionalFilterHeight fractFilter;
	//PeakBelowFractionalFilterLeft fractFilterLeft;
	//PeakBelowFractionalFilterRight fractFilterRight;
	//PeakBelowPullupFractionalFilterLeft pullupFFLeft;
	//PeakBelowPullupFractionalFilterRight pullupFFRight;
	//PeakBelowPullupFractionalFilter pullupFractionalFilter;

//	Notice* newNotice;
//	UnexpectedNumberOfPeaks target;
//	InterchannelLinkage* iChannel;
	//bool pullupFF;
	//bool FF;
	//smPullUp pullup;
	////smStutter stutter;
	////smAdenylation adenylation;
	////smPrimaryInterchannelLink primaryLink;

	//smHeightBelowFractionalFilter fractFilter;
	//smBelowFractionalFilterLeft fractFilterLeft;
	//smBelowFractionalFilterRight fractFilterRight;
	//smHeightBelowPullupFractionalFilter pullupFractionalFilter;
	//smBelowPullupFractionalFilterLeft pullupFFLeft;
	//smBelowPullupFractionalFilterRight pullupFFRight;

	while (nextLocus = (Locus*) it ())
		nextLocus->CorrectCrossChannelAnalysesSM (ArtifactList, isNegCntl);

	//RGDListIterator art (SmartPeaks);
	//DataSignal* nextSignal;

	//while (nextSignal = (DataSignal*) art ()) {

	//	if (nextSignal->GetMessageValue (pullup)) {

	//		if (nextSignal->IsPossibleInterlocusAllele (-1)) {

	//			if (nextSignal->IsPossibleInterlocusAllele (1))
	//				pullupFF = ((nextSignal->GetMessageValue (pullupFFLeft)) && (nextSignal->GetMessageValue (pullupFFRight)));

	//			else if (nextSignal->GetMessageValue (pullupFFLeft))
	//				pullupFF = true;

	//			else
	//				pullupFF = false;
	//		}

	//		else {

	//			if (nextSignal->IsPossibleInterlocusAllele (1)) {
	//				
	//				if (nextSignal->GetMessageValue (pullupFFRight))
	//					pullupFF = true;

	//				else
	//					pullupFF = false;
	//			}

	//			else
	//				pullupFF = false;
	//		}

	//		if (pullupFF) {

	//			nextSignal->SetMessageValue (pullupFractionalFilter, true);
	//			nextSignal->SetAlleleName ("");
	//			nextSignal->SetAlleleName ("", -1);
	//			nextSignal->SetAlleleName ("", 1);
	//		}
	//	}

	//	if (!nextSignal->GetMessageValue (fractFilter)) {

	//		if (nextSignal->IsPossibleInterlocusAllele (-1)) {

	//			if (nextSignal->IsPossibleInterlocusAllele (1))
	//				FF = ((nextSignal->GetMessageValue (fractFilterLeft)) && (nextSignal->GetMessageValue (fractFilterRight)));

	//			else if (nextSignal->GetMessageValue (fractFilterLeft))
	//				FF = true;

	//			else
	//				FF = false;
	//		}

	//		else {

	//			if (nextSignal->IsPossibleInterlocusAllele (1)) {
	//				
	//				if (nextSignal->GetMessageValue (fractFilterRight))
	//					FF = true;

	//				else
	//					FF = false;
	//			}

	//			else
	//				FF = false;
	//		}

	//		if (FF) {

	//			nextSignal->SetMessageValue (fractFilter, true);
	//			nextSignal->SetAlleleName ("");
	//		}
	//	}
	//}

	//if (isNegCntl) {

	//	it.Reset ();

	//	while (nextLocus = (Locus*) it ()) {

	//		if (nextLocus->GetMessageValue (unexpectedNumberOfPeaks)) {

	//			SetMessageValue (unexpectedNumberOfPeaksInChannel, true);
	//		}
	//	}
	//}

	return 0;
}


int ChannelData :: CorrectLaneStandardCrossChannelAnalysisSM () {

	return 0;
}


int ChannelData :: CleanUpLocusSignalListsSM () {

	//
	//  This is sample stage 4
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;

	while (nextLocus = (Locus*) it ())
		nextLocus->CleanUpSignalListSM (ArtifactList);

	return 0;
}


int ChannelData :: CorrectGridLocusSignalsSM () {

	// This method does absolutely nothing!!!

	//
	//  This is ladder stage 3
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;

	//smStutter stutter;
	//smPullUp pullup;
	//smAdenylation adenylation;

	while (nextLocus = (Locus*) it ())
		nextLocus->CorrectGridSignalListSM (ArtifactList);	// This doesn't do anything!!

	//RGDListIterator art (ArtifactList);
	//DataSignal* nextSignal;

	//while (nextSignal = (DataSignal*) art ()) {

	//	if (nextSignal->GetMessageValue (pullup)) {

	//		if ((nextSignal->GetMessageValue (&stutter)) || (nextSignal->GetMessageValue (&adenylation)))
	//			nextSignal->RemoveCrossChannelSignalLinkSM ();
	//	}
	//}

	return 0;
}


int ChannelData :: CleanUpGridLocusSignalsSM () {

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	int retValue = 0;

	while (nextLocus = (Locus*) it ()) {

		if (nextLocus->CleanUpGridSignalListSM (ArtifactList) < 0)
			retValue = -1;
	}

	return retValue;
}


int ChannelData :: RemoveInterlocusSignalsSM (bool isNegCntl) {

	DataSignal* nextSignal;

	while (nextSignal = (DataSignal*) PreliminaryCurveList.GetFirst ()) {

	//	ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this???????????????????????????????
		nextSignal->AddNoticeToList (OutputLevelManager::OffLadderSignalBetweenLoci, "", "Off ladder signal between loci");
	//	nextSignal->SetMessageValue (interlocusPeak, true);

		//if (isNegCntl)
		//	nextSignal->SetMessageValue (unexpectedNumberOfPeaks, true);
	}

	return 0;
}


int ChannelData :: TestSampleInterlocusMinRFUSM (bool isNegCntl, double left, double ilsLeft, double right, CoreBioComponent* assocGrid, CoordinateTransform* transform) {

	//
	//  This is sample stage 4
	//

	DataSignal* nextSignal;
	smHeightBelowInterlocusMinRFU belowMinInterlocusRFU;

	if (isNegCntl) {

		nextSignal = (DataSignal*) PreliminaryCurveList.Last ();

		//if (nextSignal == NULL)
		//	return 0;

		//if (nextSignal->GetMean () >= ilsLeft)
		//	SetMessageValue (unexpectedNumberOfPeaks, true);

		PreliminaryCurveList.Clear ();
		return 0;
	}

	RGDListIterator pIt (PreliminaryCurveList);

	while (nextSignal = (DataSignal*) pIt ()) {

		if (TestInterlocusSignalHeightBelowThreshold (nextSignal)) {

			pIt.RemoveCurrentItem ();
			nextSignal->SetMessageValue (belowMinInterlocusRFU, true);
		}
	}

	return 0;
}


int ChannelData :: RemoveInterlocusSignalsSM (double left, double ilsLeft, double right, CoreBioComponent* assocGrid) {

	//
	//  This is ladder and sample stage 5
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	Locus* previousLocus = NULL;
	RGString info;
	bool foundInterlocusLeft = false;
	bool foundInterlocusRight = false;
	bool finishedLastSignal = false;
	DataSignal* nextSignal;
	double mean;
	Locus* gridLocus;
//	Locus* lastLocus;
	double bp;
	int ibp;
	Locus* previousGridLocus;
	bool thisPeakRight;
	bool thisPeakLeft;
	smInterlocusPeaksToRight interlocusToRight;
	smInterlocusPeaksToLeft interlocusToLeft;
	smRestrictedPriorityPeak restrictedPriorityPeak;

	while (nextSignal = (DataSignal*) PreliminaryCurveList.GetFirst ()) {

		mean = nextSignal->GetMean ();

		if (!CoreBioComponent::SignalIsWithinAnalysisRegion (nextSignal, ilsLeft)) {

	//		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this???????????????????????????????????
			continue;
		}

		else {

			PreliminaryCurveList.Prepend (nextSignal);
			break;
		}
	}

	if (PreliminaryCurveList.Entries () == 0)
		return 0;

	while (nextLocus = (Locus*) it ()) {

		foundInterlocusLeft = false;
		foundInterlocusRight = false;

		if (assocGrid == NULL)
			gridLocus = NULL;

		else {

			gridLocus = assocGrid->FindLocus (mChannel, nextLocus->GetLocusName ());

			if (gridLocus == NULL)
				return -1;
		}

		while (nextSignal = (DataSignal*) PreliminaryCurveList.GetFirst ()) {

			mean = nextSignal->GetMean ();
			thisPeakLeft = thisPeakRight = false;

			if (mean > right) {

				finishedLastSignal = true;
				PreliminaryCurveList.Clear ();
				break;
			}

			else if (TestIfTimeIsLeftOfLocus (mean, nextLocus, gridLocus)) {

				if (!nextSignal->GetMessageValue (restrictedPriorityPeak)) {

					bp = nextSignal->GetApproximateBioID ();
					ibp = (int) floor (bp + 0.5);
					foundInterlocusRight = true;

					info = "";
					info << ibp;
					thisPeakRight = true;
					nextLocus->SetMessageValue (interlocusToLeft, true);
					nextLocus->AppendDataForSmartMessage (interlocusToLeft, info);

					if (previousLocus != NULL) {

						foundInterlocusLeft = true;
						thisPeakLeft = true;
						previousLocus->SetMessageValue (interlocusToRight, true);
						previousLocus->AppendDataForSmartMessage (interlocusToRight, info);
					}

			//		ArtifactList.InsertWithNoReferenceDuplication (nextSignal);	// Need this???????????????????

					//if (thisPeakRight || thisPeakLeft)
					//	nextSignal->SetMessageValue (interlocusPeak, true);
				}
			}

			else {

				PreliminaryCurveList.Prepend (nextSignal);
				break;
			}
		}

		if (finishedLastSignal)
			break;

		previousLocus = nextLocus;
		previousGridLocus = gridLocus;
	}

	if (finishedLastSignal)
		return 0;

	if (previousLocus == NULL)
		return -1;

	foundInterlocusLeft = false;

	while (nextSignal = (DataSignal*) PreliminaryCurveList.GetFirst ()) {

		mean = nextSignal->GetMean ();
		thisPeakLeft = false;

		if (mean > right) {

			finishedLastSignal = true;
			PreliminaryCurveList.Clear ();
			break;
		}

		else if (!TestIfTimeIsLeftOfLocus (mean, previousLocus, previousGridLocus)) {

			if (!nextSignal->GetMessageValue (restrictedPriorityPeak)) {

				bp = nextSignal->GetApproximateBioID ();
				ibp = (int) floor (bp + 0.5);

				info = "";
				info << ibp;
				thisPeakLeft = true;

				previousLocus->SetMessageValue (interlocusToRight, true);
				previousLocus->AppendDataForSmartMessage (interlocusToRight, info);
			}
		}

		else {
			continue;
		}
	}

	return 0;
}



int ChannelData :: TestInterlocusCharacteristicsSM (double left, double ilsLeft, double right, CoreBioComponent* assocGrid, ChannelData* laneStd, bool isNegCntl) {

	//
	//  This is sample stage 4
	//

	CoordinateTransform* globalSouthern = laneStd->GetIDMap ();

	if (globalSouthern == NULL)
		return -1;

	// Code below is no longer needed:  Locus::TestInterlocusSignals essentially looks for stutter and adenylation, which is now done earlier.  Must modify RemoveInterlocusSignals
	// to remove only those PreliminaryCurveList peaks that are not ambiguous...they are truly interlocus.  Also, it no longer needs to test for fractional filters and stutter and adenylation.
	// These are already done.

//	RGDListIterator it (mLocusList);

	//while (nextLocus = (Locus*) it ()) {

	//	nextLocus->TestInterlocusSignals (PreliminaryCurveList, ArtifactList, laneStd);
	//}

	TestSampleInterlocusMinRFUSM (isNegCntl, left, ilsLeft, right, assocGrid, globalSouthern);
	return 0;
}


int ChannelData :: TestForInterlocusProximityArtifactsSM () {

	return -1;
}


int ChannelData :: TestPositiveControlSM (IndividualGenotype* genotype) {

	//
	//  This is sample stage 5
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;
	RGString locusName;
	IndividualLocus* locus;
	int returnValue = 0;
	smPosCtrlLocusNotFound locusNotFound;

	while (nextLocus = (Locus*) it ()) {

		locusName = nextLocus->GetLocusName ();
		locus = genotype->FindLocus (locusName);

		if (locus == NULL) {

			nextLocus->SetMessageValue (locusNotFound, true);
		//	AppendDataForSmartMessage (locusNotFound, locusName);
			cout << "Positive Control Locus Not Found:  " << (char*)locusName.GetData () << endl;
			returnValue = -1;
		}

		else {

			if (nextLocus->TestPositiveControlSM (locus, ArtifactList) < 0)
				returnValue = -1;
		}
	}

	return returnValue;
}


int ChannelData :: FilterSmartNoticesBelowMinimumBioID (ChannelData* laneStd, int minBioID) {

	//  This function is redundant - 11/22/2010

	//if (minBioID <= 0)
	//	return 0;

	//double cutOffTime = laneStd->GetTimeForSpecifiedID ((double) minBioID);
	//RGDListIterator it (SmartPeaks);
	//DataSignal* nextSignal;
	//smPeakOutsideILS peakOutsideILS;

	//while (nextSignal = (DataSignal*) it ()) {

	//	if (nextSignal->GetMean () <= cutOffTime)
	//		nextSignal->SetMessageValue (peakOutsideILS, true);

	//	else
	//		return 0;
	//}

	return 0;
}


void ChannelData :: TestAllelesAgainstOverloadThresholdSM () {

	//
	//  This is sample stage 4
	//

	RGDListIterator it (mLocusList);
	Locus* nextLocus;

	while (nextLocus = (Locus*) it ())
		nextLocus->TestAllelesAgainstOverloadThresholdSM ();
}


int ChannelData :: TestSignalsForOffScaleSM () {

	return -1;
}


int ChannelData :: TestSignalsForRaisedBaseline (double left, double report) {

	return -1;
}


int ChannelData :: TestForRaisedBaselineAndExcessiveNoiseSM (double left, double report) {

	return -1;
}


void ChannelData :: InitializeMessageData () {

	int size = SmartMessage::GetSizeOfArrayForScope (GetObjectScope ());
	ChannelData::InitializeMessageMatrix (mMessageArray, size);
}


int ChannelData :: AnalyzeDynamicBaselineSM (int startTime, double reportMinTime) {

	return 0;
}


int ChannelData :: AnalyzeDynamicBaselineAndNormalizeRawDataSM (int startTime, double reportMinTime) {

	return 1;
}


void ChannelData :: CreateInitializationData (int scope) {

	int size = SmartMessage::GetSizeOfArrayForScope (scope);
	int i;
	SmartMessage* msg;
	delete[] InitialMatrix;
	InitialMatrix = new bool [size];

	for (i=0; i<size; i++) {

		msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, i);

		if (msg != NULL)
			InitialMatrix [i] = msg->GetInitialValue ();
	}
}


void ChannelData :: InitializeMessageMatrix (bool* matrix, int size) {

	int i;

	for (i=0; i<size; i++)
		matrix [i] = InitialMatrix [i];
}


//*******************************************************************************************************
//*******************************************************************************************************


