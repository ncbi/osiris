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
*  FileName: SmartMessage.cpp
*  Author:   Robert Goor
*
*/
//
// class SmartMessage is the core of the new messaging system.  It can perform computations and retains info to decide
// if, when true, whether a peak can be called, triggers, etc.
//

#include "SmartMessage.h"
#include "rgtokenizer.h"
#include "SmartNotice.h"
#include "STRLCAnalysis.h"



PERSISTENT_DEFINITION (SmartMessageData, _SMARTMESSAGEDATA_, "SmartMessageData")
PERSISTENT_DEFINITION (SmartMessageReporter, _SMARTMESSAGEREPORTER_, "SmartMessageReporter")


RGHashTable* SmartMessage :: OverAllMessageTable = new RGHashTable (113);
RGDList SmartMessage :: OverAllMessageList;
int SmartMessage :: NumberOfMessageTables = 7;
RGDList** SmartMessage :: MessageTables = new RGDList* [SmartMessage::NumberOfMessageTables];
SmartMessage*** SmartMessage :: MessageArrays = new SmartMessage** [SmartMessage::NumberOfMessageTables];
int* SmartMessage :: MessageArraySize = new int [SmartMessage::NumberOfMessageTables];
int SmartMessage :: LastStage = 0;
int** SmartMessage :: StageStartIndex = new int* [SmartMessage::NumberOfMessageTables];
int** SmartMessage :: StageEndIndex = new int* [SmartMessage::NumberOfMessageTables];
bool SmartMessage :: Initialized = false;
int* SmartMessage :: MessageValueArraySize = new int [SmartMessage::NumberOfMessageTables];
int SmartMessage :: DebugMode = 0;
RGFile* SmartMessage :: DebugFile = NULL;
RGTextOutput* SmartMessage :: DebugTextOutput = NULL;
RGString SmartMessage :: MsgBookText;
int SmartMessage :: SeverityTrigger = 15;
SmartMessageKill SmartMessage :: kill;


ABSTRACT_DEFINITION (SmartMessage)

SmartMessageKill :: SmartMessageKill () {

}


SmartMessageKill :: ~SmartMessageKill () {

	SmartMessage::ClearAllMessages ();
	STRLCAnalysis::ClearAllInitializationMatrices ();
}


/*
int mIntegerData;
RGString mName;
RGString mDescription;
int mSampleType;
RGString mMessageText;
RGString mAdditionalTextForData;
RGString mSeparatorText;  //e.g., "; "
int mScope;  // type of object message applies to, and which data array

int mWhichElementWithinDataArray;
*/


SmartMessage :: SmartMessage () : RGPersistent (), mStage (-1), mIntegerData (0), mSampleType (-1), mScope (-1), 
mWhichElementWithinDataArray (-1), mWhichElementWithinValueArray (0), mValid (0), mCompiled (0), mAcceptData (0), 
mShareWithCluster (0), mAllowPeakEdit (1), mDebugOn (false) {

}


SmartMessage :: SmartMessage (int integerData, const RGString& name) : RGPersistent (), mStage (-1), mIntegerData (integerData), 
mName (name), mSampleType (-1), mScope (-1), mWhichElementWithinDataArray (-1), mWhichElementWithinValueArray (0), mValid (0), mCompiled (0), mAcceptData (0), 
mShareWithCluster (0), mAllowPeakEdit (1), mDebugOn (false) {

}


SmartMessage :: SmartMessage (const SmartMessage& sm) : RGPersistent (), 
mStage (sm.mStage), 
mIntegerData (sm.mIntegerData),
mName (sm.mName),
mDescription (sm.mDescription),
mSampleType (sm.mSampleType),
mMessageText (sm.mMessageText),
mAdditionalTextForData (sm.mAdditionalTextForData),
mSeparatorText (sm.mSeparatorText),
mScope (sm.mScope),
mWhichElementWithinDataArray (sm.mWhichElementWithinDataArray), 
mValid (sm.mValid), 
mCompiled (sm.mCompiled), 
mAcceptData (sm.mAcceptData),
mShareWithCluster (sm.mShareWithCluster), 
mAllowPeakEdit (sm.mAllowPeakEdit), 
mDebugOn (sm.mDebugOn) {

}


SmartMessage :: ~SmartMessage () {

}



size_t SmartMessage :: StoreSize () const {

	size_t size = 8 * sizeof (int) + mName.StoreSize () + mDescription.StoreSize ();
	size += mMessageText.StoreSize () + mAdditionalTextForData.StoreSize () + mSeparatorText.StoreSize ();
	return size;
}


int SmartMessage :: CompareTo (const RGPersistent* p) const {

	SmartMessage* sm = (SmartMessage*) p;
	RGString temp = sm->GetName ();
	return mName.CompareTo (&temp);
}


unsigned SmartMessage :: HashNumber (unsigned long Base) const {

	return mName.HashNumber (Base);
}


Boolean SmartMessage :: IsEqualTo (const RGPersistent* p) const {

	SmartMessage* sm = (SmartMessage*) p;
	RGString temp = sm->GetName ();
	return mName.IsEqualTo (&temp);
}


void SmartMessage :: RestoreAll (RGFile& f) {

	f.Read (mStage);
	f.Read (mIntegerData);
	mName.RestoreAll (f);
	mDescription.RestoreAll (f);
	f.Read (mSampleType);
	mMessageText.RestoreAll (f);
	mAdditionalTextForData.RestoreAll (f);
	mSeparatorText.RestoreAll (f);
	f.Read (mScope);
	f.Read (mWhichElementWithinDataArray);
	f.Read (mWhichElementWithinValueArray);
	f.Read (mValid);
	f.Read (mCompiled);
	f.Read (mAcceptData);
	f.Read (mShareWithCluster);
}


void SmartMessage :: RestoreAll (RGVInStream& f) {

	f >> mStage;
	f >> mIntegerData;
	mName.RestoreAll (f);
	mDescription.RestoreAll (f);
	f >> mSampleType;
	mMessageText.RestoreAll (f);
	mAdditionalTextForData.RestoreAll (f);
	mSeparatorText.RestoreAll (f);
	f >> mScope;
	f >> mWhichElementWithinDataArray;
	f >> mWhichElementWithinValueArray;
	f >> mValid;
	f >> mCompiled;
	f >> mAcceptData;
	f >> mShareWithCluster;
}


void SmartMessage :: SaveAll (RGFile& f) const {

	f.Write (mStage);
	f.Write (mIntegerData);
	mName.SaveAll (f);
	mDescription.SaveAll (f);
	f.Write (mSampleType);
	mMessageText.SaveAll (f);
	mAdditionalTextForData.SaveAll (f);
	mSeparatorText.SaveAll (f);
	f.Write (mScope);
	f.Write (mWhichElementWithinDataArray);
	f.Write (mWhichElementWithinValueArray);
	f.Write (mValid);
	f.Write (mCompiled);
	f.Write (mAcceptData);
	f.Write (mShareWithCluster);
}


void SmartMessage :: SaveAll (RGVOutStream& f) const {

	f << mStage;
	f << mIntegerData;
	mName.SaveAll (f);
	mDescription.SaveAll (f);
	f << mSampleType;
	mMessageText.SaveAll (f);
	mAdditionalTextForData.SaveAll (f);
	mSeparatorText.SaveAll (f);
	f << mScope;
	f << mWhichElementWithinDataArray;
	f << mWhichElementWithinValueArray;
	f << mValid;
	f << mCompiled;
	f << mAcceptData;
	f << mShareWithCluster;
}



int SmartMessage :: AssembleMessages () {

	RGDListIterator it (OverAllMessageList);
	SmartMessage* nextMsg;
	int i;
	int whichList;
	SmartNotice* assocNotice;
	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;

	NumberOfMessageTables--;
	MessageTables [0] = NULL;
	MessageArraySize [0] = 0;

	for (i=1; i<=NumberOfMessageTables; i++)
		MessageTables [i] = new RGDList;

	while (nextMsg = (SmartMessage*) it ()) {

		whichList = nextMsg->GetScope ();
		MessageTables [whichList]->Append (nextMsg);
	}

	RGDListIterator* msgIt;
	int size;
	int j;
	int stage;
	int valueSize = 0;

	for (i=1; i<=NumberOfMessageTables; i++) {

		msgIt = new RGDListIterator (*MessageTables [i]);
		size = MessageTables [i]->Entries ();
		MessageArraySize [i] = size;
		MessageArrays [i] = new SmartMessage* [size];

		while (nextMsg = (SmartMessage*) (*msgIt) ())
			valueSize += nextMsg->NumberOfIntegerValueCellsNeeded ();

		MessageValueArraySize [i] = valueSize;

		j = 0;
		valueSize = 0;
		int currentValueSize;

		for (stage=0; stage<=LastStage; stage++) {		// test stage 0

			msgIt->Reset ();
			
			while (nextMsg = (SmartMessage*) (*msgIt) ()) {

				if (nextMsg->GetStage () != stage)
					continue;

				MessageArrays [i][j] = nextMsg;
				nextMsg->SetWhichElementInMsgArray (j);
				currentValueSize = nextMsg->NumberOfIntegerValueCellsNeeded ();

				if (currentValueSize > 0) {

					nextMsg->SetWhichElementInValueArray (valueSize);
					valueSize += currentValueSize;
				}

				assocNotice = warehouse->FindType (nextMsg->GetName ());

				if (assocNotice != NULL)
					assocNotice->SetIndexAndScope (j, nextMsg->GetScope ());

				delete assocNotice;
				j++;
			}
		}

		delete msgIt;

		if (j != size) {

			delete warehouse;
			return -1;
		}
	}

	for (i=1; i<=NumberOfMessageTables; i++) {

		StageStartIndex [i] = new int [LastStage + 1];
		StageEndIndex [i] = new int [LastStage + 1];
	}

	int endOfLastStage;
	int endOfThisStage;

	for (i=1; i<=NumberOfMessageTables; i++) {

		endOfLastStage = -1;

		for (stage=0; stage<=LastStage; stage++) {		// test stage 0

			StageStartIndex [i][stage] = endOfLastStage + 1;
			endOfThisStage = endOfLastStage;
			size = MessageArraySize [i];

			for (j=StageStartIndex [i][stage]; j<size; j++) {

				if (MessageArrays [i][j]->GetStage () == stage)
					endOfThisStage = j;

				else
					break;
			}

			StageEndIndex [i][stage] = endOfThisStage;
			endOfLastStage = endOfThisStage;
		}
	}

	Initialized = true;
	delete warehouse;
	return 0;
}


int SmartMessage :: AssembleMessagesV4 (const RGString& inputString, size_t startIndex, SmartMessage* prototype) {

	int status = ImportMessageDynamicDataV4 (inputString, startIndex, prototype);

	if (status < 0)
		return status;

	SmartMessage* nextMsg;
	int i;
	SmartNotice* assocNotice;
	SmartNoticeWarehouse* warehouse = new SmartNoticeWarehouse;

	RGDListIterator* msgIt;
	int size;
	int j;
	int stage;
	int valueSize = 0;

	for (i=1; i<=NumberOfMessageTables; i++) {

		msgIt = new RGDListIterator (*MessageTables [i]);
		size = MessageTables [i]->Entries ();
		MessageArraySize [i] = size;
		MessageArrays [i] = new SmartMessage* [size];

		while (nextMsg = (SmartMessage*) (*msgIt) ())
			valueSize += nextMsg->NumberOfIntegerValueCellsNeeded ();

		MessageValueArraySize [i] = valueSize;

		j = 0;
		valueSize = 0;
		int currentValueSize;

		for (stage=0; stage<=LastStage; stage++) {		// test stage 0

			msgIt->Reset ();
			
			while (nextMsg = (SmartMessage*) (*msgIt) ()) {

				if (nextMsg->GetStage () != stage)
					continue;

				MessageArrays [i][j] = nextMsg;
				nextMsg->SetWhichElementInMsgArray (j);
				currentValueSize = nextMsg->NumberOfIntegerValueCellsNeeded ();

				if (currentValueSize > 0) {

					nextMsg->SetWhichElementInValueArray (valueSize);
					valueSize += currentValueSize;
				}

				assocNotice = warehouse->FindType (nextMsg->GetName ());

				if (assocNotice != NULL)
					assocNotice->SetIndexAndScope (j, nextMsg->GetScope ());

				delete assocNotice;
				j++;
			}
		}

		delete msgIt;

		if (j != size) {

			cout << "WARNING:  Some declared messages are unused for scope = " << i << "...Number of unused messages is " << size - j << endl;
			//delete warehouse;
			//return -1;
		}
	}

	for (i=1; i<=NumberOfMessageTables; i++) {

		StageStartIndex [i] = new int [LastStage + 1];
		StageEndIndex [i] = new int [LastStage + 1];
	}

	int endOfLastStage;
	int endOfThisStage;

	for (i=1; i<=NumberOfMessageTables; i++) {

		endOfLastStage = -1;

		for (stage=0; stage<=LastStage; stage++) {		// test stage 0

			StageStartIndex [i][stage] = endOfLastStage + 1;
			endOfThisStage = endOfLastStage;
			size = MessageArraySize [i];

			for (j=StageStartIndex [i][stage]; j<size; j++) {

				if (MessageArrays [i][j]->GetStage () == stage)
					endOfThisStage = j;

				else
					break;
			}

			StageEndIndex [i][stage] = endOfThisStage;
			endOfLastStage = endOfThisStage;
		}
	}

	Initialized = true;
	delete warehouse;
	return 0;
}


int SmartMessage :: ImportMessageDynamicDataV4 (const RGString& inputString, size_t startIndex, SmartMessage* prototype) {

	RGString localInputString (inputString);

	size_t endIndex;
	int i;

	RGString testUnitString;
	RGString messageString;

	RGString nameString ("First String");
	RGString testCriterionString;

	RGString unitString;
	RGString stageDynamicsString;
	RGString finalDetailString;

	size_t conditionalTestUnitStart;
	size_t conditionalTestUnitEnd;

	size_t stageLevelStart;
	size_t stageLevelEnd;

	size_t scopeDynamicStart;
	size_t scopeDynamicEnd;

	size_t scopeLevelStart;
	size_t scopeLevelEnd;

	size_t finalDetailStart;
	size_t finalDetailEnd;
	

	int scope;
	RGString numericString;

//	startIndex = 0;
	int currentStage = 0;
	SmartMessage* nextSM;
	NumberOfMessageTables--;
	int numberOfScopes = NumberOfMessageTables;

	for (i=1; i<=numberOfScopes; i++) {

		MessageTables [i] = new RGDList;
	}

	MessageTables [0] = NULL;
	MessageArraySize [0] = 0;

	// Now interpret the data in TestUnitsSampleLevel and TestUnitsDirectoryLevel...

	RGString stageString;
	RGString scopeDynamicsString;
	RGString dynamicScopeString;
	RGString expressionString;
	RGString triggerString;
	RGString subExpressionString;

	SmartMessage* foundMap;
	RGHashTable availableForTriggers;
	size_t equalsLocation;

	RGXMLTagSearch sampleLevelSearch ("TestUnitsSampleLevel", localInputString);
	RGXMLTagSearch directoryLevelSearch ("TestUnitsDirectoryLevel", localInputString);
	RGXMLTagSearch testUnitSearch ("ConditionalTestUnit", unitString);

	RGXMLTagSearch stageSearch ("Stage", testUnitString);
	RGXMLTagSearch stageDynamicsSearch ("StageDynamics", testUnitString);
	RGXMLTagSearch scopeDynamicsSearch ("ScopeDynamics", stageString);

	RGXMLTagSearch scopeInDynamicsSearch ("Scope", scopeDynamicsString);
	RGXMLTagSearch measuredMessagesSearch ("MeasuredMessages", scopeDynamicsString);
	RGXMLTagSearch triggeredMessagesSearch ("TriggeredMessages", scopeDynamicsString);
	RGXMLTagSearch evaluationsSearch ("Evaluations", scopeDynamicsString);
	RGXMLTagSearch triggersSearch ("Triggers", scopeDynamicsString);

	RGXMLTagSearch dynamicMessageNameSearch ("Name", finalDetailString);
	RGXMLTagSearch expressionSearch ("Expression", finalDetailString);
	RGXMLTagSearch triggerExpressionSearch ("TriggerExpression", finalDetailString);

	if (!sampleLevelSearch.FindNextTag (startIndex, endIndex, unitString)) {

		cout << "Could not find 'TestUnitsSampleLevel' tag.  Ending..." << endl;
		return -10;
	}

	startIndex = endIndex;
	conditionalTestUnitStart = 0;
	testUnitSearch.ResetSearch ();
	currentStage = 0;
	bool foundTriggerCondition;
	size_t leftParensLocation;

	while (testUnitSearch.FindNextTag (conditionalTestUnitStart, conditionalTestUnitEnd, testUnitString)) {

		conditionalTestUnitStart = conditionalTestUnitEnd;

		stageLevelStart = 0;
		stageSearch.ResetSearch ();
		stageDynamicsSearch.ResetSearch ();

		if (!stageSearch.FindNextTag (stageLevelStart, stageLevelEnd, numericString)) {

			cout << "Could not locate stage in conditional test unit for sample level.  Last stage was " << currentStage << ".  Ending..." << endl;
			return -52;
		}

		stageLevelStart = stageLevelEnd;
		LastStage = currentStage = numericString.ConvertToInteger ();

		if (!stageDynamicsSearch.FindNextTag (stageLevelStart, stageLevelEnd, stageString)) {

			cout << "Could not locate stage dynamics string for stage " << currentStage << ".  Ending..." << endl;
			return -53;
		}

		scopeDynamicStart = 0;
		scopeDynamicsSearch.ResetSearch ();
		scope = 0;

		while (scopeDynamicsSearch.FindNextTag (scopeDynamicStart, scopeDynamicEnd, scopeDynamicsString)) {

			scopeDynamicStart = scopeDynamicEnd;
			scopeInDynamicsSearch.ResetSearch ();
			measuredMessagesSearch.ResetSearch ();
			triggeredMessagesSearch.ResetSearch ();
			evaluationsSearch.ResetSearch ();
			triggersSearch.ResetSearch ();

			scopeLevelStart = 0;

			if (!scopeInDynamicsSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, numericString)) {

				cout << "Could not find scope in scope dynamics field for stage " << currentStage << ".  Last scope was " << scope << ".  Ending..." << endl;
				return -54;
			}

			scopeLevelStart = scopeLevelEnd;
			scope = numericString.ConvertToInteger ();

			if (measuredMessagesSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, finalDetailString)) {

				scopeLevelStart = scopeLevelEnd;
				finalDetailStart = 0;
				dynamicMessageNameSearch.ResetSearch ();

				while (dynamicMessageNameSearch.FindNextTag (finalDetailStart, finalDetailEnd, nameString)) {

					finalDetailStart = finalDetailEnd;
					prototype->SetName (nameString);
					nextSM = (SmartMessage*) OverAllMessageTable->Find (prototype);

					if (nextSM == NULL) {

						cout << "Could not find measured message named " << nameString.GetData () << " for scope = " << scope << ".  Ending..." << endl;
						return -55;
					}

					if (nextSM->GetScope () != scope) {

						cout << "Scope mismatch for measured message named " << nameString.GetData () << " for scopes = " << scope << " and " << nextSM->GetScope () << ".  Ending..." << endl;
						return -55;
					}

					nextSM->SetScope (scope);
					nextSM->SetStage (currentStage);
					MessageTables [scope]->Append (nextSM);
					availableForTriggers.Insert (nextSM);
				}
			}

			if (triggeredMessagesSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, finalDetailString)) {

				scopeLevelStart = scopeLevelEnd;
				finalDetailStart = 0;
				dynamicMessageNameSearch.ResetSearch ();

				while (dynamicMessageNameSearch.FindNextTag (finalDetailStart, finalDetailEnd, nameString)) {

					finalDetailStart = finalDetailEnd;
					prototype->SetName (nameString);
					nextSM = (SmartMessage*) OverAllMessageTable->Find (prototype);

					if (nextSM == NULL) {

						cout << "Could not find triggered message named " << nameString.GetData () << " for scope = " << scope << ".  Ending..." << endl;
						return -55;
					}

					if (nextSM->GetScope () != scope) {

						cout << "Scope mismatch for triggered message named " << nameString.GetData () << " for scopes = " << scope << " and " << nextSM->GetScope () << ".  Ending..." << endl;
						return -55;
					}

					nextSM->SetScope (scope);
					nextSM->SetStage (currentStage);
					MessageTables [scope]->Append (nextSM);
					availableForTriggers.Insert (nextSM);
				}
			}

			if (evaluationsSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, finalDetailString)) {

				scopeLevelStart = scopeLevelEnd;
				finalDetailStart = 0;
				expressionSearch.ResetSearch ();

				while (expressionSearch.FindNextTag (finalDetailStart, finalDetailEnd, expressionString)) {

					finalDetailStart = finalDetailEnd;

					if (!expressionString.FindSubstring ("=", equalsLocation)) {

						cout << "Could not find '=' in expression for stage = " << currentStage << " and scope = " << scope << ".  Ending..." << endl;
						return -56;
					}

					nameString = expressionString.ExtractSubstring (0, equalsLocation - 1);
					nameString.FindAndReplaceAllSubstrings (" ", "");
					subExpressionString = expressionString.ExtractSubstring (equalsLocation + 1, expressionString.Length () - 1);

					prototype->SetName (nameString);
					nextSM = (SmartMessage*) OverAllMessageTable->Find (prototype);

					if (nextSM == NULL) {

						cout << "Could not find evaluated message named " << nameString.GetData () << " for scope = " << scope << ".  Ending..." << endl;
						return -55;
					}

					if (nextSM->GetScope () != scope) {

						cout << "Scope mismatch for evaluated message named " << nameString.GetData () << " for scopes = " << scope << " and " << nextSM->GetScope () << ".  Ending..." << endl;
						return -55;
					}

					nextSM->SetScope (scope);
					nextSM->SetStage (currentStage);
					nextSM->ImportTestCriterion (subExpressionString);
					MessageTables [scope]->Append (nextSM);
					availableForTriggers.Insert (nextSM);
				}
			}

			if (triggersSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, finalDetailString)) {

				scopeLevelStart = scopeLevelEnd;
				finalDetailStart = 0;
				triggerExpressionSearch.ResetSearch ();

				while (triggerExpressionSearch.FindNextTag (finalDetailStart, finalDetailEnd, triggerString)) {

					finalDetailStart = finalDetailEnd;

					if (!triggerString.FindSubstring (":", equalsLocation)) {

						cout << "Could not find ':' in trigger expression for stage = " << currentStage << " and scope = " << scope << ".  Ending..." << endl;
						return -56;
					}

					nameString = triggerString.ExtractSubstring (0, equalsLocation - 1);

					if (nameString.FindSubstring ("(", leftParensLocation)) {

						foundTriggerCondition = true;
						nameString = triggerString.ExtractSubstring (0, leftParensLocation - 1);
						subExpressionString = triggerString.ExtractSubstring (leftParensLocation, triggerString.Length () - 1);
					}

					else {

						foundTriggerCondition = false;
						subExpressionString = triggerString.ExtractSubstring (equalsLocation + 1, triggerString.Length () - 1);
					}

					nameString.FindAndReplaceAllSubstrings (" ", "");
					prototype->SetName (nameString);
					nextSM = (SmartMessage*) OverAllMessageTable->Find (prototype);

					if (nextSM == NULL) {

						cout << "Could not find triggering message named " << nameString.GetData () << " for scope = " << scope << ".  Ending..." << endl;
						return -55;
					}

					if (nextSM->GetScope () != scope) {

						cout << "Scope mismatch for triggering message named " << nameString.GetData () << " for scopes = " << scope << " and " << nextSM->GetScope () << ".  Ending..." << endl;
						return -55;
					}

					if (foundTriggerCondition)
						nextSM->ImportTriggerCondition (subExpressionString);

					else
						nextSM->ImportTriggerNames (subExpressionString);
					
					foundMap = (SmartMessage*) availableForTriggers.Find (nextSM);

					if (foundMap == NULL) {

						cout << "Could not find triggering message named " << nameString.GetData () << " for scope = " << scope << " in this scope and stage list.  Ending..." << endl;
						return -56;
					}
				}
			}

			availableForTriggers.Clear ();
		}
	}

	if (!directoryLevelSearch.FindNextTag (startIndex, endIndex, unitString)) {

		cout << "Could not find 'TestUnitsDirectoryLevel' tag.  Ending..." << endl;
		return -10;
	}

	startIndex = endIndex;
	conditionalTestUnitStart = 0;
	testUnitSearch.ResetSearch ();
	currentStage = 0;

	while (testUnitSearch.FindNextTag (conditionalTestUnitStart, conditionalTestUnitEnd, testUnitString)) {

		conditionalTestUnitStart = conditionalTestUnitEnd;

		stageLevelStart = 0;
		stageSearch.ResetSearch ();
		stageDynamicsSearch.ResetSearch ();

		if (!stageSearch.FindNextTag (stageLevelStart, stageLevelEnd, numericString)) {

			cout << "Could not locate stage in conditional test unit for sample level.  Last stage was " << currentStage << ".  Ending..." << endl;
			return -52;
		}

		stageLevelStart = stageLevelEnd;
		currentStage = numericString.ConvertToInteger ();

		if (!stageDynamicsSearch.FindNextTag (stageLevelStart, stageLevelEnd, stageString)) {

			cout << "Could not locate stage dynamics string for stage " << currentStage << ".  Ending..." << endl;
			return -53;
		}

		scopeDynamicStart = 0;
		scopeDynamicsSearch.ResetSearch ();
		scope = 0;

		while (scopeDynamicsSearch.FindNextTag (scopeDynamicStart, scopeDynamicEnd, scopeDynamicsString)) {

			scopeDynamicStart = scopeDynamicEnd;
			scopeInDynamicsSearch.ResetSearch ();
			measuredMessagesSearch.ResetSearch ();
			triggeredMessagesSearch.ResetSearch ();
			evaluationsSearch.ResetSearch ();
			triggersSearch.ResetSearch ();

			scopeLevelStart = 0;

			if (!scopeInDynamicsSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, numericString)) {

				cout << "Could not find scope in scope dynamics field for stage " << currentStage << ".  Last scope was " << scope << ".  Ending..." << endl;
				return -54;
			}

			scopeLevelStart = scopeLevelEnd;
			scope = numericString.ConvertToInteger ();

			if (measuredMessagesSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, finalDetailString)) {

				scopeLevelStart = scopeLevelEnd;
				finalDetailStart = 0;
				dynamicMessageNameSearch.ResetSearch ();

				while (dynamicMessageNameSearch.FindNextTag (finalDetailStart, finalDetailEnd, nameString)) {

					finalDetailStart = finalDetailEnd;
					prototype->SetName (nameString);
					nextSM = (SmartMessage*) OverAllMessageTable->Find (prototype);

					if (nextSM == NULL) {

						cout << "Could not find measured message named " << nameString.GetData () << " for scope = " << scope << ".  Ending..." << endl;
						return -55;
					}

					if (nextSM->GetScope () != scope) {

						cout << "Scope mismatch for measured message named " << nameString.GetData () << " for scopes = " << scope << " and " << nextSM->GetScope () << ".  Ending..." << endl;
						return -55;
					}

					nextSM->SetScope (scope);
					nextSM->SetStage (currentStage);
					MessageTables [scope]->Append (nextSM);
					availableForTriggers.Insert (nextSM);
				}
			}

			if (triggeredMessagesSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, finalDetailString)) {

				scopeLevelStart = scopeLevelEnd;
				finalDetailStart = 0;
				dynamicMessageNameSearch.ResetSearch ();

				while (dynamicMessageNameSearch.FindNextTag (finalDetailStart, finalDetailEnd, nameString)) {

					finalDetailStart = finalDetailEnd;
					prototype->SetName (nameString);
					nextSM = (SmartMessage*) OverAllMessageTable->Find (prototype);

					if (nextSM == NULL) {

						cout << "Could not find triggered message named " << nameString.GetData () << " for scope = " << scope << ".  Ending..." << endl;
						return -55;
					}

					if (nextSM->GetScope () != scope) {

						cout << "Scope mismatch for triggered message named " << nameString.GetData () << " for scopes = " << scope << " and " << nextSM->GetScope () << ".  Ending..." << endl;
						return -55;
					}

					nextSM->SetScope (scope);
					nextSM->SetStage (currentStage);
					MessageTables [scope]->Append (nextSM);
					availableForTriggers.Insert (nextSM);
				}
			}

			if (evaluationsSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, finalDetailString)) {

				scopeLevelStart = scopeLevelEnd;
				finalDetailStart = 0;
				expressionSearch.ResetSearch ();

				while (expressionSearch.FindNextTag (finalDetailStart, finalDetailEnd, expressionString)) {

					finalDetailStart = finalDetailEnd;

					if (!expressionString.FindSubstring ("=", equalsLocation)) {

						cout << "Could not find '=' in expression for stage = " << currentStage << " and scope = " << scope << ".  Ending..." << endl;
						return -56;
					}

					nameString = expressionString.ExtractSubstring (0, equalsLocation - 1);
					nameString.FindAndReplaceAllSubstrings (" ", "");
					subExpressionString = expressionString.ExtractSubstring (equalsLocation + 1, expressionString.Length () - 1);

					prototype->SetName (nameString);
					nextSM = (SmartMessage*) OverAllMessageTable->Find (prototype);

					if (nextSM == NULL) {

						cout << "Could not find evaluated message named " << nameString.GetData () << " for scope = " << scope << ".  Ending..." << endl;
						return -55;
					}

					if (nextSM->GetScope () != scope) {

						cout << "Scope mismatch for evaluated message named " << nameString.GetData () << " for scopes = " << scope << " and " << nextSM->GetScope () << ".  Ending..." << endl;
						return -55;
					}

					nextSM->SetScope (scope);
					nextSM->SetStage (currentStage);
					nextSM->ImportTestCriterion (subExpressionString);
					MessageTables [scope]->Append (nextSM);
					availableForTriggers.Insert (nextSM);
				}
			}

			if (triggersSearch.FindNextTag (scopeLevelStart, scopeLevelEnd, finalDetailString)) {

				scopeLevelStart = scopeLevelEnd;
				finalDetailStart = 0;
				triggerExpressionSearch.ResetSearch ();

				while (triggerExpressionSearch.FindNextTag (finalDetailStart, finalDetailEnd, triggerString)) {

					//finalDetailStart = finalDetailEnd;

					//if (!triggerString.FindSubstring (":", equalsLocation)) {

					//	cout << "Could not find ':' in trigger expression for stage = " << currentStage << " and scope = " << scope << ".  Ending..." << endl;
					//	return -56;
					//}

					//nameString = triggerString.ExtractSubstring (0, equalsLocation - 1);
					//nameString.FindAndReplaceAllSubstrings (" ", "");
					//subExpressionString = triggerString.ExtractSubstring (equalsLocation + 1, triggerString.Length () - 1);

					//prototype->SetName (nameString);
					//nextSM = (SmartMessage*) OverAllMessageTable->Find (prototype);

					//if (nextSM == NULL) {

					//	cout << "Could not find triggering message named " << nameString.GetData () << " for scope = " << scope << ".  Ending..." << endl;
					//	return -55;
					//}

					//if (nextSM->GetScope () != scope) {

					//	cout << "Scope mismatch for triggering message named " << nameString.GetData () << " for scopes = " << scope << " and " << nextSM->GetScope () << ".  Ending..." << endl;
					//	return -55;
					//}

					//nextSM->ImportTriggerNames (subExpressionString);
					//foundMap = (SmartMessage*) availableForTriggers.Find (nextSM);

					//if (foundMap == NULL) {

					//	cout << "Could not find triggering message named " << nameString.GetData () << " for scope = " << scope << " in this scope and stage list.  Ending..." << endl;
					//	return -56;
					//}

					finalDetailStart = finalDetailEnd;

					if (!triggerString.FindSubstring (":", equalsLocation)) {

						cout << "Could not find ':' in trigger expression for stage = " << currentStage << " and scope = " << scope << ".  Ending..." << endl;
						return -56;
					}

					nameString = triggerString.ExtractSubstring (0, equalsLocation - 1);

					if (nameString.FindSubstring ("(", leftParensLocation)) {

						foundTriggerCondition = true;
						nameString = triggerString.ExtractSubstring (0, leftParensLocation - 1);
						subExpressionString = triggerString.ExtractSubstring (leftParensLocation, triggerString.Length () - 1);
					}

					else {

						foundTriggerCondition = false;
						subExpressionString = triggerString.ExtractSubstring (equalsLocation + 1, triggerString.Length () - 1);
					}

					nameString.FindAndReplaceAllSubstrings (" ", "");
					prototype->SetName (nameString);
					nextSM = (SmartMessage*) OverAllMessageTable->Find (prototype);

					if (nextSM == NULL) {

						cout << "Could not find triggering message named " << nameString.GetData () << " for scope = " << scope << ".  Ending..." << endl;
						return -55;
					}

					if (nextSM->GetScope () != scope) {

						cout << "Scope mismatch for triggering message named " << nameString.GetData () << " for scopes = " << scope << " and " << nextSM->GetScope () << ".  Ending..." << endl;
						return -55;
					}

					if (foundTriggerCondition)
						nextSM->ImportTriggerCondition (subExpressionString);

					else
						nextSM->ImportTriggerNames (subExpressionString);
					
					foundMap = (SmartMessage*) availableForTriggers.Find (nextSM);

					if (foundMap == NULL) {

						cout << "Could not find triggering message named " << nameString.GetData () << " for scope = " << scope << " in this scope and stage list.  Ending..." << endl;
						return -56;
					}
				}
			}

			availableForTriggers.Clear ();
		}
	}

	return 0;
}


int SmartMessage :: GetMessageIndex (const SmartMessage* msg, int& whichDataArray) {

	SmartMessage* foundMsg = (SmartMessage*) OverAllMessageTable->Find (msg);

	if (foundMsg == NULL) {

		whichDataArray = -1;
		return -1;
	}

	whichDataArray = foundMsg->GetScope ();
	return foundMsg->GetWhichElementInMsgArray ();
}


int SmartMessage :: LoadAllMessages (const RGString& inputString, const SmartMessage* prototype) {

	int stage;
	RGString stageString;
	RGString testUnitString;
	RGString msgString;
	RGString overallString (inputString);
	RGString debugString;
	int debugInt;
	RGXMLTagSearch testUnitSearch ("ConditionalTestUnit", overallString);
	RGXMLTagSearch stageSearch ("Stage", testUnitString);
	RGXMLTagSearch msgSearch ("SmartMessage", testUnitString);
	RGXMLTagSearch debugSearch ("DebugMode", overallString);
	size_t startIndex = 0;
	size_t endIndex;

	size_t testStartIndex;
	size_t testEndIndex;

	SmartMessage* nextMsg;
	int status = 0;
	int i;

	for (i=1; i<NumberOfMessageTables; i++) {

		StageStartIndex [i] = NULL;
		StageEndIndex [i] = NULL;
	}

	if (debugSearch.FindNextTag (startIndex, endIndex, debugString)) {

		debugInt = debugString.ConvertToInteger ();

		if (debugInt == 0)
			DebugMode = 0;

		else
			DebugMode = 1;
	}

	else
		DebugMode = 0;

	while (testUnitSearch.FindNextTag (startIndex, endIndex, testUnitString)) {

		startIndex = endIndex;
		msgSearch.ResetSearch ();
		stageSearch.ResetSearch ();
		testStartIndex = 0;

		if (!stageSearch.FindNextTag (testStartIndex, testEndIndex, stageString))
			return -1;

		stage = stageString.ConvertToInteger ();
		testStartIndex = testEndIndex;
		LastStage = stage;

		while (msgSearch.FindNextTag (testStartIndex, testEndIndex, msgString)) {

			testStartIndex = testEndIndex;
			nextMsg = prototype->GetNewMessage (msgString);
			nextMsg->SetStage (stage);
			OverAllMessageTable->Append (nextMsg);
			OverAllMessageList.Append (nextMsg);

			if (!nextMsg->IsValid ())
				status = -1;
		}
	}

	if (status < 0)
		return status;

	return AssembleMessages ();
}


void SmartMessage :: ClearAllMessages () {

	int i;
//	int j;

	for (i=1; i<=NumberOfMessageTables; i++) {

		if (Initialized) {
			
			MessageTables [i]->Clear ();
			delete MessageTables [i];
			delete[] MessageArrays [i];
		}
	}

	delete[] MessageTables;
	delete[] MessageArrays;
	delete[] MessageArraySize;
	delete[] MessageValueArraySize;
	OverAllMessageTable->Clear ();
	delete OverAllMessageTable;
	OverAllMessageList.ClearAndDelete ();

	if (Initialized) {

		for (i=1; i<=NumberOfMessageTables; i++) {

			delete[] StageStartIndex [i];
			delete[] StageEndIndex [i];
		}
	}

	delete[] StageStartIndex;
	delete[] StageEndIndex;

	if (DebugMode != 0) {

		if (DebugFile != NULL)
			DebugFile->Flush ();

		delete DebugTextOutput;
		delete DebugFile;
	}
}


int SmartMessage :: GetStartIndexForListAndStage (int list, int stage) {

	if (stage > LastStage)
		return 0;

	if (StageStartIndex [list] != NULL)
		return StageStartIndex [list][stage];

	return 0;
}


int SmartMessage :: GetEndIndexForListAndStage (int list, int stage) {

	if (stage > LastStage)
		return -1;

	if (StageEndIndex [list] != NULL)
		return StageEndIndex [list][stage];

	return -1;
}


int SmartMessage :: LoadAllMessagesV4 (const RGString& inputString, SmartMessage* prototype) {

	RGString declarationString;
	RGString msgString;
	RGString overallString (inputString);
	RGString debugString;
	RGString buildString;
	int debugInt;
	RGXMLTagSearch declarationsSearch ("MessageDeclarations", overallString);
	RGXMLTagSearch msgSearch ("SmartMessage", declarationString);
	RGXMLTagSearch debugSearch ("DebugMode", overallString);
	RGXMLTagSearch buildTimeSearch ("BuildTime", overallString);
	size_t startIndex = 0;
	size_t endIndex;

	size_t testStartIndex;
	size_t testEndIndex;

	SmartMessage* nextMsg;
	int status = 0;
	int i;

	for (i=1; i<NumberOfMessageTables; i++) {

		StageStartIndex [i] = NULL;
		StageEndIndex [i] = NULL;
	}

	if (buildTimeSearch.FindNextTag (startIndex, endIndex, buildString)) {

		startIndex = endIndex;
		STRLCAnalysis::SetMsgBookBuildTime (buildString);
	}

	if (debugSearch.FindNextTag (startIndex, endIndex, debugString)) {

		debugInt = debugString.ConvertToInteger ();
		startIndex = endIndex;

		if (debugInt == 0)
			DebugMode = 0;

		else
			DebugMode = 1;
	}

	else
		DebugMode = 0;

	if (!declarationsSearch.FindNextTag (startIndex, endIndex, declarationString)) {

		cout << "There are no smart message declarations.  MessageBook is empty!" << endl;
		return -1;
	}

	startIndex = endIndex;
	msgSearch.ResetSearch ();
	testStartIndex = 0;

	while (msgSearch.FindNextTag (testStartIndex, testEndIndex, msgString)) {

		testStartIndex = testEndIndex;
		nextMsg = prototype->GetNewMessageV4 (msgString);
		OverAllMessageTable->Append (nextMsg);
//		OverAllMessageList.Append (nextMsg);

		if (!nextMsg->IsValid ())
			status = -1;
	}

	if (status < 0)
		return status;

	return AssembleMessagesV4 (overallString, startIndex, prototype);  // change to AssembleMessagesV4
}


int SmartMessage :: CompileAllMessages () {

	RGHashTableIterator it (*OverAllMessageTable);
	SmartMessage* nextMessage;
	int status = 0;
	bool result;

	while (nextMessage = (SmartMessage*) it ()) {

		result = nextMessage->CompileAll ();

		if (!result)
			status = -1;
	}

	return status;
}


int SmartMessage :: InitializeAllMessages () {

	RGHashTableIterator it (*OverAllMessageTable);
	SmartMessage* nextMessage;
	int status = 0;
	int result;

	while (nextMessage = (SmartMessage*) it ()) {

		result = nextMessage->Initialize ();

		if (result < 0)
			status = -1;
	}

	return status;
}


bool SmartMessage :: EvaluateAllMessages (bool* const msgMatrix, RGDList& subObjects, int stage, int scope) {

	int startIndex = GetStartIndexForListAndStage (scope, stage);
	int endIndex = GetEndIndexForListAndStage (scope, stage);
	int i;

	for (i=startIndex; i<=endIndex; i++)
		MessageArrays [scope][i]->Evaluate (msgMatrix, subObjects);

	return true;
}


bool SmartMessage :: EvaluateAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, int scope) {

	int startIndex = GetStartIndexForListAndStage (scope, stage);
	int endIndex = GetEndIndexForListAndStage (scope, stage);
	int i;

	for (i=startIndex; i<=endIndex; i++) {

		MessageArrays [scope][i]->Evaluate (comm, numHigherObjects);
	}

	return true;
}


bool SmartMessage :: SetTriggersForAllMessages (bool* const msgMatrix, bool* const higherMsgMatrix, int stage, int scope) {

	int startIndex = GetStartIndexForListAndStage (scope, stage);
	int endIndex = GetEndIndexForListAndStage (scope, stage);
	int i;

	for (i=startIndex; i<=endIndex; i++)
		MessageArrays [scope][i]->SetAllTriggers (msgMatrix, higherMsgMatrix);

	return true;
}


bool SmartMessage :: SetTriggersForAllMessages (bool* const msgMatrix, bool* const higherMsgMatrix, RGHashTable* messageDataTable, int stage, int scope, int higherScope, int intBP, const RGString& alleleName) {

	int startIndex = GetStartIndexForListAndStage (scope, stage);
	int endIndex = GetEndIndexForListAndStage (scope, stage);
	int i;

	for (i=startIndex; i<=endIndex; i++)
		MessageArrays [scope][i]->SetAllTriggers (msgMatrix, higherMsgMatrix);

	return true;
}


bool SmartMessage :: SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, int scope, int intBP, const RGString& alleleName) {

	int startIndex = GetStartIndexForListAndStage (scope, stage);
	int endIndex = GetEndIndexForListAndStage (scope, stage);
	int i;

	for (i=startIndex; i<=endIndex; i++)
		MessageArrays [scope][i]->SetAllTriggers (comm, numHigherObjects, stage, intBP, alleleName);

	return true;
}


bool SmartMessage :: SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, int scope) {

	int startIndex = GetStartIndexForListAndStage (scope, stage);
	int endIndex = GetEndIndexForListAndStage (scope, stage);
	int i;

	for (i=startIndex; i<=endIndex; i++)
		MessageArrays [scope][i]->SetAllTriggers (comm, numHigherObjects, stage);

	return true;
}


bool SmartMessage :: EvaluateAllReports (const bool* msgMatrix, bool* const reportMatrix, int scope) {

	int size = MessageArraySize [scope];
	int i;

	for (i=0; i<size; i++) {

		reportMatrix [i] = MessageArrays [scope][i]->EvaluateReport (msgMatrix);
	}

	return true;
}


bool SmartMessage :: TestAllMessagesForCall (const bool* msgMatrix, int scope) {

	// This should only be called for scope = 1, DataSignal, because the decision to call or not
	// only applies to peaks
	
	int size = MessageArraySize [scope];
	int i;

	for (i=0; i<size; i++) {

		if (!MessageArrays [scope][i]->EvaluateCall (msgMatrix))
			return false;
	}

	return true;
}


int SmartMessage :: FindRestrictionLevel (const bool* msgMatrix, int scope) {

	// This should only be called for scope = 1, DataSignal, because restriction levels
	// only apply to peaks, at least in the current version
	
	int size = MessageArraySize [scope];
	int i;
	int restrictionLevel = -1;
	int currentLevel;

	for (i=0; i<size; i++) {

		currentLevel = MessageArrays [scope][i]->EvaluateRestrictionLevel (msgMatrix);
		
		if (currentLevel > restrictionLevel)
			restrictionLevel = currentLevel;
	}

	return restrictionLevel;
}


bool SmartMessage :: EvaluateAllReportLevels (const bool* msgMatrix, int* const reportLevelMatrix, int scope) {

	int size = MessageArraySize [scope];
	int i;

	for (i=0; i<size; i++) {

		reportLevelMatrix [i] = MessageArrays [scope][i]->EvaluateReportLevel (msgMatrix);
	}

	return true;
}


int SmartMessage :: GetSizeOfArrayForScope (int scope) {

	if (Initialized) {

		if ((1 <= scope) && (scope <= NumberOfMessageTables))
			return MessageArraySize [scope];

		return 0;
	}

	return 0;
}


int SmartMessage :: GetSizeOfValueArrayForScope (int scope) {

	if (Initialized) {

		if ((1 <= scope) && (scope <= NumberOfMessageTables))
			return MessageValueArraySize [scope];

		return 0;
	}

	return 0;
}


bool SmartMessage :: CreateDebugFile (const RGString& fullPathDebugName) {

	if (!DebugMode)
		return false;

	DebugFile = new RGFile (fullPathDebugName, "wt");

	if (!DebugFile->isValid ()) {

		cout << "Could not open debug file named:  " << fullPathDebugName << endl;
		delete DebugFile;
		DebugFile = NULL;
		DebugMode = 0;
		return false;
	}

	DebugTextOutput = new RGTextOutput (DebugFile, FALSE);
	cout << "Message Book Debug File opened successfully" << endl;
	return true;
}


bool SmartMessage :: OutputDebugString (const RGString& string) {

	if (DebugTextOutput == NULL)
		return false;

	Endl endLine;

	*DebugTextOutput << string << endLine;
	return true;
}


int SmartMessage :: LoadAndActivateAllMessages (const RGString& messageFullPathFileName, SmartMessage* prototype) {
	
	// call this function to set up all SmartMessages

	RGFile inputFile (messageFullPathFileName, "rt");

	if (!inputFile.isValid ()) {

		cout << "Could not open smart messagebook file:  " << messageFullPathFileName.GetData () << endl;
		return -1;
	}

	RGString inputString;
	inputString.ReadTextFile (inputFile);
	SmartMessage::MsgBookText = inputString;

	int status = SmartMessage::LoadAllMessagesV4 (inputString, prototype);

	if (status < 0) {

		cout << "Could not load messagebook file:  " << messageFullPathFileName.GetData () << endl;
		return -2;
	}

	status = SmartMessage::CompileAllMessages ();

	if (status < 0) {

		cout << "Could not compile messagebook file:  " << messageFullPathFileName.GetData () << endl;
		return -3;
	}

	status = SmartMessage::InitializeAllMessages ();

	if (status < 0) {

		cout << "Could not initialize messagebook file:  " << messageFullPathFileName.GetData () << endl;
		return -4;
	}

	return 0;
}


int SmartMessage :: WriteMsgBookFile (RGFile& msgFile) {

	MsgBookText.WriteTextLine (msgFile);
	return 0;
}


SmartMessageData :: SmartMessageData () : RGPersistent (), mIndex (-1), mScope (-1) {

}


SmartMessageData :: SmartMessageData (int index) : RGPersistent (), mIndex (index), mScope (0) {

}


SmartMessageData :: SmartMessageData (int index, int scope, const RGString& text) : RGPersistent (),
mIndex (index), mScope (scope) {

	SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, index);

	if (msg != NULL)
		mText = msg->GetTextForData ();
	
	mText += text;
}


SmartMessageData :: SmartMessageData (const SmartMessageData& smd) : RGPersistent (smd),
mIndex (smd.mIndex), mScope (smd.mScope), mText (smd.mText) {

}


SmartMessageData :: ~SmartMessageData () {

}


void SmartMessageData :: AppendText (const RGString& text, const SmartNotice& notice) {

	int whichArray = notice.GetScope ();
	int whichElement = notice.GetMessageIndex ();
	SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (whichArray, whichElement);

	if (msg != NULL) {

		if (mText.Length () > 0) {

			RGString connector = msg->GetDataSeparator ();
			mText.Append (connector);
		}

		//else {

		//	RGString start = msg->GetTextForData ();
		//	mText.Append (start);
		//}
	}

	mText.Append (text);
}


void SmartMessageData :: AppendText (const RGString& text, int index, int scope) {

	SmartMessage* msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, index);
	RGString connectorString;
	RGString startString;

	if (msg != NULL) {

		startString = msg->GetTextForData ();
		connectorString = msg->GetDataSeparator ();
	}

	if (mText.Length () > 0)
		mText.Append (connectorString);
	
	//else
	//	mText.Append (startString);

	mText.Append (text);
}


size_t SmartMessageData :: StoreSize () const {

	size_t size = RGPersistent::StoreSize () + sizeof (int);
	size += mText.StoreSize ();
	return size;
}


int SmartMessageData :: CompareTo (const RGPersistent* p) const {

	SmartMessageData* q = (SmartMessageData*) p;

	int q1 = q->mIndex;

	if (mIndex < q1)
		return -1;

	if (mIndex > q1)
		return 1;

	return 0;
}


unsigned SmartMessageData :: HashNumber (unsigned long Base) const {

	return mIndex%Base;
}


Boolean SmartMessageData :: IsEqualTo (const RGPersistent* p) const {

	SmartMessageData* q = (SmartMessageData*) p;

	if (mIndex != q->mIndex)
		return FALSE;

	return TRUE;
}


/*

const SmartMessage* mPrototype;
const SmartMessageData* mData;
int mMessageCount;
int mPriorityLevel;
int mRestriction;
bool mIsDoNotCall;

*/


SmartMessageReporter :: SmartMessageReporter () : RGPersistent (),
mPrototype (NULL),
mData (NULL),
mMessageCount (0),
mPriorityLevel (-1),
mRestriction (-1),
mIsDoNotCall (false),
mCritical (true),
mEnabled (true),
mEditable (true),
mDisplayExportInfo (true),
mDisplayOsirisInfo (true),
mViableExportInfo (true) {

}


SmartMessageReporter :: SmartMessageReporter (const SmartMessage* prototype) : RGPersistent (),
mPrototype (prototype),
mData (NULL),
mMessageCount (0),
mPriorityLevel (-1),
mRestriction (-1),
mIsDoNotCall (false),
mCritical (true),
mEnabled (true),
mEditable (prototype->IsEditable ()),
mDisplayExportInfo (true),
mDisplayOsirisInfo (true),
mViableExportInfo (true) {

}


SmartMessageReporter :: SmartMessageReporter (const SmartMessageReporter& smr) : RGPersistent (smr),
mPrototype (smr.mPrototype),
mData (smr.mData),
mMessageCount (smr.mMessageCount),
mPriorityLevel (smr.mPriorityLevel),
mRestriction (smr.mRestriction),
mIsDoNotCall (smr.mIsDoNotCall),
mCritical (smr.mCritical),
mEnabled (smr.mEnabled),
mEditable (smr.mEditable),
mDisplayExportInfo (smr.mDisplayExportInfo),
mDisplayOsirisInfo (smr.mDisplayOsirisInfo),
mViableExportInfo (smr.mViableExportInfo) {

}



SmartMessageReporter :: ~SmartMessageReporter () {

}


void SmartMessageReporter :: SetPriorityLevel (int prio) { 
	
	mPriorityLevel = prio;

	if (prio < SmartMessage::GetSeverityTrigger ())
		mCritical = true;

	else
		mCritical = false;
}


void SmartMessageReporter :: ComputeViabilityOfExportInfo () {

	bool hasExportInfo = (mExportProtocolInformation.Length () > 0);
	mViableExportInfo = hasExportInfo && mDisplayExportInfo && (mEnabled || mEditable);
}


RGString SmartMessageReporter :: GetMessageName () const {

	if (mPrototype != NULL)
		return mPrototype->GetName ();

	RGString name;
	return name;
}


RGString SmartMessageReporter :: GetMessageData () const {

	RGString temp;

	if (mData != NULL) {

		if (mData->GetTextLength () > 0) {

//			temp = mPrototype->GetTextForData ();
			temp += mData->GetText ();
			temp += mPrototype->GetClosingTextForData ();
		}
	}

	return temp;
}


size_t SmartMessageReporter :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += 3 + sizeof (int) + sizeof (bool);
	size += sizeof (SmartMessage*);
	size += sizeof (SmartMessageData*);
	return size;
}


int SmartMessageReporter :: CompareTo (const RGPersistent* p) const {

	SmartMessageReporter* q = (SmartMessageReporter*) p;

	if ((mPrototype == NULL) || (q->mPrototype == NULL)) {

		int p1 = GetID ();
		int p2 = q->GetID ();

		if (p1 < p2)
			return -1;

		if (p1 > p2)
			return 1;

		return 0;
	}

	if (mPriorityLevel < q->mPriorityLevel)
		return -1;

	if (mPriorityLevel > q->mPriorityLevel)
		return 1;

	return 0;
}


unsigned SmartMessageReporter :: HashNumber (unsigned long Base) const {

	if (mPrototype == NULL)
		return GetID ()%Base;

	return mPriorityLevel%Base;
}


Boolean SmartMessageReporter :: IsEqualTo (const RGPersistent* p) const {

	SmartMessageReporter* q = (SmartMessageReporter*) p;

	if ((mPrototype == NULL) || (q->mPrototype == NULL)) {

		return (GetID () == q->GetID ());
	}

	return (mPriorityLevel == q->mPriorityLevel);
}

