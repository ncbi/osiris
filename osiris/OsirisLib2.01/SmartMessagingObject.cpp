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
*  FileName: SmartMessagingObject.cpp
*  Author:   Robert Goor
*
*/
//
//	abstract class to provide interface for all objects that perform smart messaging.  These include:  DataSignal,
//  Locus, ChannelData, CoreBioComponent, STRLCAnalysis and BaseLocus.
//

#include "SmartMessagingObject.h"

RGHashTable SmartMessagingObject :: ExportSpecifications;

ABSTRACT_DEFINITION (SmartMessagingObject)

/*
	SmartMessagingObject ();
	SmartMessagingObject (const SmartMessagingObject& smo);
	virtual ~SmartMessagingObject () = 0;

	virtual int GetObjectScope () const = 0;

	void SetMessageValue (int scope, int location, bool value);
	void SetMessageValue (const SmartNotice& notice, bool value);
	void SetMessageValue (const SmartNotice& notice, bool value, bool useVirtualMethod);

	bool GetMessageValue (const SmartNotice& notice) const;
	bool GetMessageValue (int location) const { return mMessageArray [location]; }

	void SetDataForSmartMessage (const SmartNotice& notice, const RGString& text);
	void AppendDataForSmartMessage (const SmartNotice& notice, const RGString& text);
	void AppendDataForSmartMessage (const SmartNotice& notice, int value);
	void AppendDataForSmartMessage (const SmartNotice& notice, double value);

	virtual void ClearSmartNoticeObjects ();

	virtual int AddSmartMessageReporter (SmartMessageReporter* smr);
	virtual int AddAllSmartMessageReporters () = 0;



protected:
	bool* mMessageArray;
	RGHashTable* mMessageDataTable;
	RGDList* mSmartMessageReporters;

	int mStageCompleted;
	int mTriggerStageCompleted;

	int mHighestSeverityLevel;
	int mHighestMessageLevel;

*/


SmartMessagingObject :: SmartMessagingObject () : RGPersistent (), mStageCompleted (0), mTriggerStageCompleted (0), 
mHighestSeverityLevel (-1), mHighestMessageLevel (-1) {

}


SmartMessagingObject :: SmartMessagingObject (const SmartMessagingObject& smo) : RGPersistent (smo), mStageCompleted (smo.mStageCompleted),
mTriggerStageCompleted (smo.mTriggerStageCompleted), mHighestSeverityLevel (smo.mHighestSeverityLevel), mHighestMessageLevel (smo.mHighestMessageLevel) {

}


SmartMessagingObject :: ~SmartMessagingObject () {

	CleanupSmartMessages ();
}


void SmartMessagingObject :: SetMessageValue (int scope, int location, bool value) {

	int myScope = GetObjectScope ();

	if (myScope == scope) {

		SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, location);
		msg->SetMessageValue (mMessageArray, mValueArray, location, value);
	}
}


void SmartMessagingObject :: SetMessageValue (const SmartNotice& notice, bool value) {

	//if (notice.GetScope () == GetObjectScope ())
	//	mMessageArray [notice.GetMessageIndex ()] = value;
	SetMessageValue (notice.GetScope (), notice.GetMessageIndex (), value);
}


//void SmartMessagingObject :: SetMessageValue (const SmartNotice& notice, bool value, bool useVirtualMethod) {
//
//	if (useVirtualMethod)
//		SetMessageValue (notice.GetScope (), notice.GetMessageIndex (), value);
//
//	else
//		SetMessageValue (notice, value);
//}


void SmartMessagingObject :: SetMessageValue (int scope, int location, bool value, bool useVirtualMethod) {

	if (useVirtualMethod)
		SetMessageValue (scope, location, value);

	else if (scope == GetObjectScope ())
		mMessageArray [location] = value;
}


RGString SmartMessagingObject :: GetDebugIDIndent () const {

	return "";
}


bool SmartMessagingObject :: GetMessageValue (const SmartNotice& notice) const {

	if (notice.GetScope () == GetObjectScope ())
		return mMessageArray [notice.GetMessageIndex ()];

	return false;
}


int SmartMessagingObject :: GetIntegerValue (int location) const {

	int size = SmartMessage::GetSizeOfValueArrayForScope (GetObjectScope ());

	if ((location < size) && (location >= 0))
		return mValueArray [location];

	else {

		size = SmartMessage::GetSizeOfArrayForScope (GetObjectScope ());

		if ((location < size) && (location >= 0)) {

			if (mMessageArray [location])
				return 1;
		}
	}

	return 0;
}


bool SmartMessagingObject :: AcceptsDataOnTrigger (int index) {

	SmartMessage* sm = SmartMessage::GetSmartMessageForScopeAndElement (GetObjectScope (), index);

	if (sm == NULL)
		return false;

	int accept = sm->GetAcceptData ();

	if ((accept == 1) || (accept == 2))
		return true;

	return false;
}


bool SmartMessagingObject :: AcceptsDataOnEvaluation (int index) {

	SmartMessage* sm = SmartMessage::GetSmartMessageForScopeAndElement (GetObjectScope (), index);

	if (sm == NULL)
		return false;

	int accept = sm->GetAcceptData ();

	if ((accept == 3) || (accept == 4))
		return true;

	return false;
}


int SmartMessagingObject :: GetThreshold (const SmartNotice& notice) const {

	int scope = notice.GetScope ();
	int index = notice.GetMessageIndex ();
	SmartMessage* sm = SmartMessage::GetSmartMessageForScopeAndElement (scope, index);

	if (sm == NULL)
		return -1;

	return sm->GetThreshold ();
}


int SmartMessagingObject :: FindRestrictionLevel () {

	return SmartMessage::FindRestrictionLevel (mMessageArray, GetObjectScope ());
}


int SmartMessagingObject :: GetHighestMessageLevelWithRestrictionSM () const {

	int temp = mHighestMessageLevel;

	if (mHighestSeverityLevel > temp)
		temp = mHighestSeverityLevel;

	return temp;
}


void SmartMessagingObject :: SetDataForSmartMessage (const SmartNotice& notice, const RGString& text) {

	if (notice.GetScope () == GetObjectScope ()) {

		int index = notice.GetMessageIndex ();
		int scope = notice.GetScope ();
		SmartMessageData target (index);
		SmartMessageData* smd = (SmartMessageData*) mMessageDataTable->Find (&target);

		if (smd == NULL) {

			smd = new SmartMessageData (index, scope, text);
			mMessageDataTable->Insert (smd);
		}

		else {

			smd->SetText (text);
		}
	}
}


void SmartMessagingObject :: AppendDataForSmartMessage (const SmartNotice& notice, const RGString& text) {

	if (text.Length () == 0)
		return;

	if (notice.GetScope () == GetObjectScope ()) {

		int index = notice.GetMessageIndex ();
		int scope = notice.GetScope ();
		SmartMessageData target (index);
		SmartMessageData* smd = (SmartMessageData*) mMessageDataTable->Find (&target);

		if (smd == NULL) {

			smd = new SmartMessageData (index, scope, text);
			mMessageDataTable->Insert (smd);
		}

		else {

			smd->AppendText (text, notice);
		}
	}
}


void SmartMessagingObject :: AppendDataForSmartMessage (const SmartNotice& notice, int value) {

	RGString data;
	data << value;
	AppendDataForSmartMessage (notice, data);
}


void SmartMessagingObject :: AppendDataForSmartMessage (const SmartNotice& notice, double value) {

	RGString data;
	data << value;
	AppendDataForSmartMessage (notice, data);
}


void SmartMessagingObject :: AppendDataForSmartMessage (int scope, int index, int value, const RGString& text) {

	if (scope = GetObjectScope ()) {

		SmartMessage* thisMsg = SmartMessage::GetSmartMessageForScopeAndElement (scope, index);
		int acceptData = thisMsg->GetAcceptData ();
		RGString chosenText;

		switch (acceptData) {

			case 0:
				return;

			case 1:
			case 3:
				chosenText << value;
				break;

			case 2:
			case 4:
				chosenText = text;
				break;

			default:
				break;
		}

		if (chosenText.Length () == 0)
			return;

		SmartMessageData target (index);
		SmartMessageData* smd = (SmartMessageData*) mMessageDataTable->Find (&target);

		if (smd == NULL) {

			smd = new SmartMessageData (index, scope, chosenText);
			mMessageDataTable->Insert (smd);
		}

		else {

			smd->AppendText (chosenText, index, scope);
		}
	}
}


void SmartMessagingObject :: ClearSmartNoticeObjects () {

	mSmartMessageReporters->ClearAndDelete ();
	mUnenabledMessageReporters->ClearAndDelete ();
	mHighestSeverityLevel = mHighestMessageLevel = -1;
	int size = SmartMessage::GetSizeOfArrayForScope (GetObjectScope ());
	int i;

	for (i=0; i<size; i++)
		mMessageArray [i] = false;
}


int SmartMessagingObject :: AddSmartMessageReporter (SmartMessageReporter* smr) {

	int msgLevel = smr->GetMessagePriority ();
	bool isEnabled = smr->IsEnabled ();

	if (!isEnabled) {

		mUnenabledMessageReporters->Prepend (smr);
		return 0;
	}

	if (mSmartMessageReporters->Entries () == 0) {

		mHighestSeverityLevel = msgLevel;
		mHighestMessageLevel = smr->GetMessageRestriction ();
		mSmartMessageReporters->Append (smr);
	}

	else {

		int temp = msgLevel;

		if (mHighestSeverityLevel >= temp) {

			mHighestSeverityLevel = temp;
			mSmartMessageReporters->Prepend (smr);
		}

		else {

			RGDListIterator it (*mSmartMessageReporters);
			SmartMessageReporter* nextNotice;
			bool noticeInserted = false;

			while (nextNotice = (SmartMessageReporter*) it ()) {

				if (temp < nextNotice->GetMessagePriority ()) {

					--it;
					it.InsertAfterCurrentItem (smr);
					noticeInserted = true;
					break;
				}
			}

			if (!noticeInserted)
				mSmartMessageReporters->Append (smr);
		}

		temp = smr->GetMessageRestriction ();

		if (mHighestMessageLevel < temp)
			mHighestMessageLevel = temp;
	}

	return mSmartMessageReporters->Entries ();
}


void SmartMessagingObject :: MergeAllSmartMessageReporters () {

	SmartMessageReporter* nextSMR;

	while (nextSMR = (SmartMessageReporter*) mUnenabledMessageReporters->GetFirst ())
		mSmartMessageReporters->Prepend (nextSMR);
}


void SmartMessagingObject :: InsertExportSpecificationsIntoTable (SmartMessage* sm) {

	if (ExportSpecifications.Contains (sm))
		return;

	ExportSpecifications.Append (sm);
}


void SmartMessagingObject :: ClearExportSpecifications () {

	ExportSpecifications.Clear ();
}


void SmartMessagingObject :: ReportAllExportSpecifications (RGLogBook& oar) {

	if (ExportSpecifications.Entries () == 0)
		return;

	oar.SetOutputLevel (1);
	oar << "\t<ExportSpecifications>\n";

	RGHashTableIterator it (ExportSpecifications);
	SmartMessage* sm;

	while (sm = (SmartMessage*) it ()) {

		oar << "\t\t\t<MsgExport>\n";
		oar << "\t\t\t\t<MsgName>" << sm->GetName () << "</MsgName>";
		oar << "\t\t\t" << sm->GetExportProtocolList ();
		oar << "</MsgExport>\n";
	}

	oar << "\t</ExportSpecifications>\n";
	oar.ResetOutputLevel ();
}


void SmartMessagingObject :: InitializeSmartMessages () {

	if (SmartMessage::IsInitialized ()) {

		int scope = GetObjectScope ();
		int size = SmartMessage::GetSizeOfArrayForScope (scope);
		int i;
		mMessageArray = new bool [size];
		InitializeMessageData ();
		
		// Removed below code because "InitializeMessageData ()" performs initialization to specified initial values
		//for (i=0; i<size; i++)
		//	mMessageArray [i] = false;

		size = SmartMessage::GetSizeOfValueArrayForScope (scope);

		if (size > 0) {

			mValueArray = new int [size];

			for (i=0; i<size; i++)
				mValueArray [i] = 0;
		}

		else {

			mValueArray = new int [1];
			mValueArray [0] = 0;
		}

		mMessageDataTable = new RGHashTable (29);
		mSmartMessageReporters = new RGDList;
		mUnenabledMessageReporters = new RGDList;
	}

	else {

		mMessageArray = NULL;
		mMessageDataTable = NULL;
		mSmartMessageReporters = NULL;
		mUnenabledMessageReporters = NULL;
	}
}


void SmartMessagingObject :: InitializeSmartMessages (const SmartMessagingObject& smo) {

	int scope = GetObjectScope ();

	if (SmartMessage::IsInitialized ()) {

		int size = SmartMessage::GetSizeOfArrayForScope (scope);
		int i;
		mMessageArray = new bool [size];

		if (smo.mMessageArray == NULL) {

			for (i=0; i<size; i++)
				mMessageArray [i] = false;
		}

		else {

			for (i=0; i<size; i++)
				mMessageArray [i] = smo.mMessageArray [i];
		}

		size = SmartMessage::GetSizeOfValueArrayForScope (scope);

		if (size > 0) {

			mValueArray = new int [size];

			if (smo.mValueArray == NULL) {

				for (i=0; i<size; i++)
					mValueArray [i] = 0;
			}

			else {

				for (i=0; i<size; i++)
					mValueArray [i] = smo.mValueArray [i];
			}
		}

		else {

			mValueArray = new int [1];
			mValueArray [0] = 0;
		}

		if (smo.mMessageDataTable == NULL)
			mMessageDataTable = new RGHashTable (29);

		else
			mMessageDataTable = new RGHashTable (*smo.mMessageDataTable);

		if (smo.mSmartMessageReporters == NULL)
			mSmartMessageReporters = new RGDList;

		else
			mSmartMessageReporters = new RGDList (*smo.mSmartMessageReporters);

		if (smo.mUnenabledMessageReporters == NULL)
			mUnenabledMessageReporters = new RGDList;

		else
			mUnenabledMessageReporters = new RGDList (*smo.mUnenabledMessageReporters);
	}

	else {

		mMessageArray = NULL;
		mMessageDataTable = NULL;
		mSmartMessageReporters = NULL;
		mUnenabledMessageReporters = NULL;
	}
}


void SmartMessagingObject :: CleanupSmartMessages () {

	if (SmartMessage::IsInitialized () && (mMessageArray != NULL)) {

		delete[] mMessageArray;
		delete[] mValueArray;
		mMessageDataTable->ClearAndDelete ();
		delete mMessageDataTable;
		mSmartMessageReporters->ClearAndDelete ();
		delete mSmartMessageReporters;
		mUnenabledMessageReporters->ClearAndDelete ();
		delete mUnenabledMessageReporters;
	}
}


