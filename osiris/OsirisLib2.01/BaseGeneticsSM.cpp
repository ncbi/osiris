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
*  FileName: BaseGeneticsSM.cpp
*  Author:   Robert Goor
*
*/
//
//  Source file for classes BaseAllele, BaseLocus, BasePopulationMarkerSet...see Genetics.h and Genetics.cpp:  this
//  file has SmartMessage-related functions only
//

#include "BaseGenetics.h"
#include "DataSignal.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgtokenizer.h"
#include "Genetics.h"
#include "coordtrans.h"
#include "ChannelData.h"


Boolean BaseLocus :: ExtractGridSignalsSM (RGDList& channelSignalList, const LaneStandard* ls, RGDList& locusDataSignalList, ChannelData* lsData, Locus* locus) {

	Msg << "Locus:  base class called incorrectly\n";
	return FALSE;
}


void BaseLocus :: OutputDebugID (SmartMessagingComm& comm, int numHigherObjects) {

	RGString idData;
	int higherObjectIndex = numHigherObjects - 2;
	SmartMessagingObject* higherObj = comm.SMOStack [higherObjectIndex];
	higherObj->OutputDebugID (comm, higherObjectIndex + 1);
	idData << "\t\tBase Locus:  " << LocusName;
	SmartMessage::OutputDebugString (idData);
}


int BaseLocus :: AddAllSmartMessageReporters () {

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


int BaseLocus :: AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects) {

	int k = GetObjectScope ();
	int size = SmartMessage::GetSizeOfArrayForScope (k);
	int i;
	int nMsgs = 0;
	SmartMessageReporter* newMsg;
	SmartMessage* nextSmartMsg;
	SmartMessageData target;
	SmartMessageData* smd;
	RGString exportProtocol;
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


void BaseLocus :: InitializeMessageData () {

	int size = SmartMessage::GetSizeOfArrayForScope (GetObjectScope ());
	BaseLocus::InitializeMessageMatrix (mMessageArray, size);
}


bool BaseLocus :: EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage) {

	return SmartMessage::EvaluateAllMessages (comm, numHigherObjects, stage, GetObjectScope ());
}


bool BaseLocus :: SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage) {

	return SmartMessage::SetTriggersForAllMessages (comm, numHigherObjects, stage, GetObjectScope ());
}


Boolean BaseLocus :: ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim) {

	if (NumberOfSmartNoticeObjects () > 0) {

		int msgLevel = GetHighestMessageLevelWithRestrictionSM ();
		RGDListIterator it (*mSmartMessageReporters);
		SmartMessageReporter* nextNotice;
		text.SetOutputLevel (msgLevel);

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			return FALSE;
		}

		Endl endLine;
		text << endLine;
		text << indent << "Summary Notices for Locus " << LocusName << ":  " << endLine;

		while (nextNotice = (SmartMessageReporter*) it ())
			text << indent << nextNotice->GetMessage () << nextNotice->GetMessageData () << endLine;

		text.ResetOutputLevel ();
		text.Write (1, "\n");
	}

	else
		return FALSE;

	return TRUE;
}


Boolean BaseLocus :: ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink) {

	if (NumberOfSmartNoticeObjects () > 0) {

		int msgLevel = GetHighestMessageLevelWithRestrictionSM ();
		RGDListIterator it (*mSmartMessageReporters);
		SmartMessageReporter* nextNotice;
		text.SetOutputLevel (msgLevel);
		RGString indent2 = indent + "\t";
		int link;

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			return FALSE;
		}

		Endl endLine;
		text << endLine;

		if (reportLink) {

			link = Notice::GetNextLinkNumber ();
			SetTableLink (link);
			text << mTableLink << "\t";
		}

		text << indent << "Summary Notices for Locus " << LocusName << ":  " << endLine;

		while (nextNotice = (SmartMessageReporter*) it ())
			text << indent << nextNotice->GetMessage () << nextNotice->GetMessageData () << endLine;

		text.ResetOutputLevel ();

		if (reportLink) {

			text.SetOutputLevel (msgLevel);
			text << mTableLink;
			text.ResetOutputLevel ();
		}

		text.Write (1, "\n");
	}

	else
		return FALSE;

	return TRUE;
}



Boolean BaseLocus :: ReportXMLSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	if (NumberOfSmartNoticeObjects () > 0) {

		int msgLevel = GetHighestMessageLevelWithRestrictionSM ();
		RGDListIterator it (*mSmartMessageReporters);
		SmartMessageReporter* nextNotice;
		text.SetOutputLevel (msgLevel);
		tempText.SetOutputLevel (1);
		int msgNum;
		int triggerLevel = Notice::GetMessageTrigger ();

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			tempText.ResetOutputLevel ();
			return FALSE;
		}

		text << CLevel (1) << "\t<BaseLocus>\n" << PLevel ();
		text << CLevel (1) << "\t\t<BaseLocusName>" << LocusName << "</BaseLocusName>\n" << PLevel ();
		text << CLevel (1) << "\t\t\t<BaseLocusAlerts>\n" << PLevel ();
		text.SetOutputLevel (1);

		if ((msgLevel > 0) && (msgLevel <= triggerLevel)) {

			while (nextNotice = (SmartMessageReporter*) it ()) {

				if (nextNotice->GetMessagePriority () <= triggerLevel) {

					msgNum = Notice::GetNextMessageNumber ();
					nextNotice->SetMessageCount (msgNum);
					text << "\t\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
					tempText << "\t\t<Message>\n";
					tempText << "\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
					tempText << "\t\t\t<Text>" << nextNotice->GetMessage () << nextNotice->GetMessageData () << "</Text>\n";
					tempText << "\t\t</Message>\n";
				}
			}
		}

		text.ResetOutputLevel ();
		text << CLevel (1) << "\t\t\t</BaseLocusAlerts>\n" << PLevel ();
		text << CLevel (1) << "\t</BaseLocus>\n";
		text.ResetOutputLevel ();
		tempText.ResetOutputLevel ();
	}

	else
		return FALSE;

	return TRUE;
}


Boolean BaseLocus :: ReportXMLSmartNoticeObjectsWithExport (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

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
		text << CLevel (1) << "\t<BaseLocus>\n" << PLevel ();
		text << CLevel (1) << "\t\t<BaseLocusName>" << LocusName << "</BaseLocusName>\n" << PLevel ();
		text << CLevel (1) << "\t\t\t<BaseLocusAlerts>\n" << PLevel ();
		text.SetOutputLevel (1);
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

				tempText << "\t\t</Message>\n";
			}
		}

		text.ResetOutputLevel ();
		text << CLevel (1) << "\t\t\t</BaseLocusAlerts>\n" << PLevel ();
		text << CLevel (1) << "\t</BaseLocus>\n";
		text.ResetOutputLevel ();
		tempText.ResetOutputLevel ();
	}

	else
		return FALSE;

	return TRUE;
}


void BaseLocus :: CreateInitializationData (int scope) {

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


void BaseLocus :: InitializeMessageMatrix (bool* matrix, int size) {

	int i;

	for (i=0; i<size; i++)
		matrix [i] = InitialMatrix [i];
}


