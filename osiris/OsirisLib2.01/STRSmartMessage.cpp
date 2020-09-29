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
*  FileName: STRSmartMessage.cpp
*  Author:   Robert Goor
*
*/
//
// Subclass of SmartMessage...this is the STR version that is based on data in XML Schema OsirisMessageBook3.1.xsd.  Each class object
// configures itself from an xml substring, with logical expressions that allow the message to reconfigure itself based on the totality
// of messages.
//

#include "STRSmartMessage.h"
#include "rgtokenizer.h"
#include "Expression.h"
#include "SmartMessagingObject.h"
#include "STRLCAnalysis.h"
#include <iostream>
#include <math.h>

using namespace std;


PERSISTENT_DEFINITION (ConditionalTriggerUnit, _CONDITIONALTRIGGERUNIT_, "ConditionalTriggerUnit")
PERSISTENT_DEFINITION (STRBaseSmartMessage, _STRBASESMARTMESSAGE_, "STRBaseSmartMessage")
PERSISTENT_DEFINITION (STRSmartMessage, _STRSMARTMESSAGE_, "STRSmartMessage")
PERSISTENT_DEFINITION (STRSmartCountingMessage, _STRSMARTCOUNTINGMESSAGE_, "STRSmartCountingMessage")
PERSISTENT_DEFINITION (STRSmartPercentMessage, _STRSMARTPERCENTMESSAGE_, "STRSmartPercentMessage")
PERSISTENT_DEFINITION (STRSmartPresetMessage, _STRSMARTPRESETMESSAGE_, "STRSmartPresetMessage")

/*
bool mValid;
	RGString mExpressionString;
	Expression* mTriggerExpression;
	RGDList mTriggerNames;
	list<TriggerInfo*> mTriggers;

*/



ConditionalTriggerUnit :: ConditionalTriggerUnit () : RGPersistent (), mValid (false), mTriggerExpression (NULL) {

}


ConditionalTriggerUnit :: ConditionalTriggerUnit (const RGString& triggerExpression) : RGPersistent (), mValid (false), mTriggerExpression (NULL) {

	// Assumes that the first character of triggerExpression is '('?
	size_t rightParensLocation;
	size_t leftParensLocation;
	size_t endPosition;
	size_t colonLocation;

	if (!triggerExpression.FindNextSubstring (0, ":", colonLocation)) {

		cout << "Cannot find defining colon in conditional trigger expression:  " << triggerExpression.GetData () << endl;
		return;
	}

	RGString conditionString = triggerExpression.ExtractSubstring (0, colonLocation - 1);
	RGString triggerList = triggerExpression.ExtractSubstring (colonLocation + 1, triggerExpression.Length () - 1);

	if (!conditionString.FindLastSubstring (")", rightParensLocation, endPosition)) {

		cout << "Unbalanced (left) parentheses in trigger conditional expression:  " << triggerExpression.GetData () << ".  Could not extract information." << endl;
		return;
	}

	if (!conditionString.FindNextSubstring (0, "(", leftParensLocation)) {

		cout << "Unbalanced (right) parentheses in trigger conditional expression:  " << triggerExpression.GetData () << ".  Could not extract information." << endl;
		return;
	}

	mExpressionString = conditionString.ExtractSubstring (leftParensLocation + 1, rightParensLocation - 1);

	size_t start = 0;

	RGStringTokenizer ExpressionData (triggerList);
	ExpressionData.AddDelimiter (',');
	ExpressionData.AddDelimiter (';');
	ExpressionData.AddRemoveItem (':');
	ExpressionData.AddRemoveItem (' ');

	RGString token;
	RGString delim;
	size_t position;
	RGString* newString;

	while (ExpressionData.NextToken (token, delim)) {

		if (token.FindNextSubstring (start, "sm", position)) {

			newString = new RGString (token);
			mTriggerNames.Append (newString);
		}
	}

	mValid = true;
}


ConditionalTriggerUnit :: ~ConditionalTriggerUnit () {

	delete mTriggerExpression;
	mTriggerNames.ClearAndDelete ();
	list<TriggerInfo*>::const_iterator c1Iterator;
	TriggerInfo* nextLink;

	for (c1Iterator = mTriggers.begin (); c1Iterator != mTriggers.end (); c1Iterator++) {

		nextLink = *c1Iterator;
		delete nextLink;
	}

	mTriggers.clear ();
}


int ConditionalTriggerUnit :: Initialize (int msgScope, SmartMessage* sourceMessage) {

	int status = 0;
	int result;

	TriggerInfo* nextLink;
	RGString* nextName;
	RGDListIterator nameIt (mTriggerNames);
	int scope;

	while (nextName = (RGString*) nameIt ()) {

		nextLink = new TriggerInfo;
		result = sourceMessage->FindMessageIndex (*nextName, scope);

		if (result < 0) {

			cout << "Could not find message index for trigger named " << nextName->GetData () << endl;
			status = -1;
		}

		if (scope < msgScope) {  // triggers are at lower level, which is an error
			
			cout << "Scope mismatch for trigger named " << nextName->GetData () << ".  Scope " << scope << " does not match expected scope " << msgScope << endl;
			status = -1;
		}

		nextLink->mIndex = result;
		nextLink->mScope = scope;
		mTriggers.push_back (nextLink);
	}

	if ((mTriggerExpression != NULL) && (mTriggerExpression->Initialize (scope, sourceMessage)) < 0) {

		cout << "Could not initialize trigger conditional expression:  " << mExpressionString.GetData () << endl;
		status = -1;
	}

	return status;
}


int ConditionalTriggerUnit :: Compile () {

	if (mExpressionString.Length () == 0)
		return -1;

	mTriggerExpression = Expression :: Compile (mExpressionString);

	if (mTriggerExpression == NULL) {

		cout << "Could not compile trigger conditional expression:  " << mExpressionString.GetData () << endl;
		return -1;
	}

	return 0;
}


bool ConditionalTriggerUnit :: SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage) {

	int i;
	int lastObject = numHigherObjects - 1;
	bool condition = true;

	if (lastObject < 0)
		return false;

	SmartMessagingObject* primeSMO = comm.SMOStack [lastObject];
	SmartMessagingObject* targetSMO;

	if (mTriggerExpression != NULL)
		condition = mTriggerExpression->Evaluate (comm, numHigherObjects);

	if (condition) {

		list<TriggerInfo*>::const_iterator c1Iterator;
		TriggerInfo* nextLink;

		for (c1Iterator = mTriggers.begin (); c1Iterator != mTriggers.end (); c1Iterator++) {

			nextLink = *c1Iterator;
			int index = nextLink->mIndex;
			int scope = nextLink->mScope;
			
			for (i=lastObject; i>=0; i--) {

				targetSMO = comm.SMOStack [i];

				if (scope == targetSMO->GetObjectScope ()) {

					targetSMO->SetMessageValue (scope, index, true);
					break;
				}
			}
		}

		return true;
	}

	return false;
}


bool ConditionalTriggerUnit :: SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage, int intBP, const RGString& alleleName) {

	int i;
	int lastObject = numHigherObjects - 1;
	bool condition = true;

	if (lastObject < 0)
		return false;

	SmartMessagingObject* primeSMO = comm.SMOStack [lastObject];
	SmartMessagingObject* targetSMO;

	if (mTriggerExpression != NULL)
		condition = mTriggerExpression->Evaluate (comm, numHigherObjects);

	if (condition) {

		list<TriggerInfo*>::const_iterator c1Iterator;
		TriggerInfo* nextLink;

		for (c1Iterator = mTriggers.begin (); c1Iterator != mTriggers.end (); c1Iterator++) {

			nextLink = *c1Iterator;
			int index = nextLink->mIndex;
			int scope = nextLink->mScope;
			
			for (i=lastObject; i>=0; i--) {

				targetSMO = comm.SMOStack [i];

				if (scope == targetSMO->GetObjectScope ()) {

					targetSMO->SetMessageValue (scope, index, true);

					if (targetSMO->AcceptsDataOnTrigger (index))
						targetSMO->AppendDataForSmartMessage (scope, index, intBP, alleleName);

					break;
				}
			}
		}

		return true;
	}

	return false;
}


DebugInfo :: DebugInfo () : mScope (0), mIndex (-1) {

	mName = new RGString;
}


DebugInfo :: ~DebugInfo () {

	delete mName;
}


STRBaseSmartMessage :: STRBaseSmartMessage () : SmartMessage (), mReportLevel (-1), mAlternateReportLevel (-1),
mRestrictionLevel (-1),
mReportExpression (NULL), 
mAlternateReportExpression (NULL), 
mCallExpression (NULL),
mRestrictionExpression (NULL),
mEditable (true),
mDisplayExportInfo (false),
mExportReportMirrorsOarReport (true) {

}



STRBaseSmartMessage :: STRBaseSmartMessage (int integerData, const RGString& name) : SmartMessage (integerData, name), 
mReportLevel (-1), mAlternateReportLevel (-1), mRestrictionLevel (-1),
mReportExpression (NULL), 
mAlternateReportExpression (NULL), 
mCallExpression (NULL),
mRestrictionExpression (NULL),
mEditable (true),
mDisplayExportInfo (false),
mExportReportMirrorsOarReport (true) {

}



STRBaseSmartMessage :: STRBaseSmartMessage (const STRBaseSmartMessage& sm) : SmartMessage (sm),
mReportCriterion (sm.mReportCriterion),
mReportLevel (sm.mReportLevel),
mAlternateReportLevel (sm.mAlternateReportLevel),
mAlternateReportCriterion (sm.mAlternateReportCriterion),
mCallCriterion (sm.mCallCriterion),
mTriggers (sm.mTriggers),
mRestrictionLevel (sm.mRestrictionLevel),
mRestrictionCriterion (sm.mRestrictionCriterion),
mReportExpression (sm.mReportExpression),
mAlternateReportExpression (sm.mAlternateReportExpression),
mCallExpression (sm.mCallExpression),
mRestrictionExpression (sm.mRestrictionExpression),
mEditable (sm.mEditable),
mDisplayExportInfo (sm.mDisplayExportInfo),
mExportReportMirrorsOarReport (sm.mExportReportMirrorsOarReport) {

	if (sm.mReportExpression != NULL)
		mReportExpression = sm.mReportExpression->MakeCopy ();

	if (sm.mAlternateReportExpression != NULL)
		mAlternateReportExpression = sm.mAlternateReportExpression->MakeCopy ();

	if (sm.mCallExpression != NULL)
		mCallExpression = sm.mCallExpression->MakeCopy ();

	if (sm.mRestrictionExpression != NULL)
		mRestrictionExpression = sm.mRestrictionExpression->MakeCopy ();
}



STRBaseSmartMessage :: ~STRBaseSmartMessage () {

	delete mReportExpression;
	delete mAlternateReportExpression;
	delete mCallExpression;
	delete mRestrictionExpression;
	mTriggerNames.ClearAndDelete ();
	mConditionalTriggerUnits.ClearAndDelete ();

	ClearTriggerList ();
	ClearDebugList ();
}




SmartMessage* STRBaseSmartMessage :: GetNewMessage (const RGString& inputString) const {

	//
	// Modify this to search inputString for MessageType element and then return appropriate type
	// of message based on result
	//

	size_t startIndex = 0;
	size_t endIndex;
	RGString localInputString (inputString);
	RGString messageTypeString;
	RGXMLTagSearch messageTypeSearch ("MessageType", localInputString);
	RGXMLTagSearch nameSearch ("Name", localInputString);
	SmartMessage* newMsg;
	RGString name;

	if (nameSearch.FindNextTag (startIndex, endIndex, name)) {

		startIndex = endIndex;
	}

	if (!messageTypeSearch.FindNextTag (startIndex, endIndex, messageTypeString))
		newMsg = new STRSmartMessage ();

	else if (messageTypeString == "boolean")
		newMsg = new STRSmartMessage ();

	else if (messageTypeString == "count")
		newMsg = new STRSmartCountingMessage ();

	else if (messageTypeString == "percent")
		newMsg = new STRSmartPercentMessage ();

	else if (messageTypeString == "preset")
		newMsg = new STRSmartPresetMessage ();

	else {

		newMsg = new STRSmartMessage ();
		cout << "Could not identify message type " << messageTypeString << endl;
	}

	newMsg->SetName (name);
	newMsg->Configure (inputString);
	return newMsg;
}


int STRBaseSmartMessage :: GetIntegerValue (SmartMessagingComm& comm, int numHigherObjects) const {

	bool value = comm.SMOStack [numHigherObjects - 1]->GetMessageValue (mWhichElementWithinDataArray);

	if (value)
		return 1;

	return 0;
}


SmartMessage* STRBaseSmartMessage :: GetNewMessageV4 (const RGString& inputString) const {

	size_t startIndex = 0;
	size_t endIndex;
	RGString localInputString (inputString);
	RGString messageTypeString;
	RGXMLTagSearch messageTypeSearch ("MessageType", localInputString);
	RGXMLTagSearch nameSearch ("Name", localInputString);
	SmartMessage* newMsg;
	RGString name;

	if (nameSearch.FindNextTag (startIndex, endIndex, name)) {

		startIndex = endIndex;
	}

	if (!messageTypeSearch.FindNextTag (startIndex, endIndex, messageTypeString))
		newMsg = new STRSmartMessage ();

	else if (messageTypeString == "boolean")
		newMsg = new STRSmartMessage ();

	else if (messageTypeString == "count")
		newMsg = new STRSmartCountingMessage ();

	else if (messageTypeString == "percent")
		newMsg = new STRSmartPercentMessage ();

	else if (messageTypeString == "preset")
		newMsg = new STRSmartPresetMessage ();

	else {

		newMsg = new STRSmartMessage ();
		cout << "Could not identify message type " << messageTypeString << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("type " + messageTypeString + "in message named " + name);
	}

	newMsg->SetName (name);
	newMsg->ConfigureV4 (inputString, startIndex);	// change to ConfigureV4
	return newMsg;
}


int STRBaseSmartMessage :: ConfigureV4 (const RGString& inputString, size_t startIndex) {

	return -1;
}


int STRBaseSmartMessage :: ConfigureDescriptionToSampleTypeV4 (const RGString& inputString, size_t startIndex, size_t& endIndex) {

	mValid = 0;

	RGString localInputString (inputString);
	RGXMLTagSearch descriptionSearch ("Description", localInputString);
	RGXMLTagSearch sampleTypeSearch ("SampleType", localInputString);

	RGString numericString;
	size_t overallStartIndex = startIndex;

	if (descriptionSearch.FindNextTag (overallStartIndex, endIndex, mDescription))
		overallStartIndex = endIndex;

	if (!sampleTypeSearch.FindNextTag (overallStartIndex, endIndex, numericString))
		return -1;

	overallStartIndex = endIndex;
	mSampleType = numericString.ConvertToInteger ();

	return 0;
}


int STRBaseSmartMessage :: ConfigureReportCriterionToEndV4 (const RGString& inputString, size_t startIndex) {

	RGString localInputString (inputString);
	RGXMLTagSearch reportCriterionSearch ("ReportCriterion", localInputString);
	RGXMLTagSearch textSearch ("Text", localInputString);
	RGXMLTagSearch textForDataSearch ("TextForData", localInputString);
	RGXMLTagSearch separatorTextSearch ("SeparatorText", localInputString);
	RGXMLTagSearch closingTextSearch ("ClosingText", localInputString);
	RGXMLTagSearch reportLevelSearch ("ReportLevel", localInputString);
	RGXMLTagSearch alternateReportSearch ("AlternateReport", localInputString);
	RGXMLTagSearch scopeSearch ("Scope", localInputString);
	RGXMLTagSearch callCriterionSearch ("CallCriterion", localInputString);
	RGXMLTagSearch restrictionSearch ("Restriction", localInputString);
	RGXMLTagSearch acceptDataSearch ("AcceptTriggerDataMode", localInputString);
	RGXMLTagSearch shareWithClusterSearch ("ShareWithCluster", localInputString);
	RGXMLTagSearch allowPeakEdit ("AllowPeakEdit", localInputString);
	RGXMLTagSearch debugSearch ("Debug", localInputString);
	RGXMLTagSearch editableSearch ("Editable", localInputString);
	RGXMLTagSearch protocolSearch ("ExportProtocolList", localInputString);
	RGXMLTagSearch displayExportSearch ("DisplayExportInfo", localInputString);

	RGString numericString;
	RGString expressionString;
	RGString alternateReportString;
	RGString restrictionString;
	RGString watchString;
	RGString debugString;
	RGString tempString;

	RGXMLTagSearch alternateReportLevelSearch ("AlternateReportLevel", alternateReportString);
	RGXMLTagSearch alternateReportCriterionSearch ("AlternateReportCriterion", alternateReportString);
	RGXMLTagSearch expressionSearch ("Expression", expressionString);
	RGXMLTagSearch restrictionLevelSearch ("RestrictionLevel", restrictionString);
	RGXMLTagSearch restrictionCriterionSearch ("RestrictionCriterion", restrictionString);
	RGXMLTagSearch debugWatchSearch ("Watch", watchString);

	size_t overallStartIndex = startIndex;
	size_t overallEndIndex;

	size_t localStartIndex = 0;
	size_t localEndIndex;
	DebugInfo* debugInfo;

	if (!reportCriterionSearch.FindNextTag (overallStartIndex, overallEndIndex, expressionString))
		return -1;

	overallStartIndex = overallEndIndex;
	expressionSearch.ResetSearch ();
	localStartIndex = 0;

	if (!expressionSearch.FindNextTag (localStartIndex, localEndIndex, mReportCriterion))
		return -1;

	if (!textSearch.FindNextTag (overallStartIndex, overallEndIndex, mMessageText))
		return -1;

	overallStartIndex = overallEndIndex;

	if (textForDataSearch.FindNextTag (overallStartIndex, overallEndIndex, mAdditionalTextForData))
		overallStartIndex = overallEndIndex;

	if (separatorTextSearch.FindNextTag (overallStartIndex, overallEndIndex, mSeparatorText))
		overallStartIndex = overallEndIndex;

	if (closingTextSearch.FindNextTag (overallStartIndex, overallEndIndex, mClosingText))
		overallStartIndex = overallEndIndex;

	if (!reportLevelSearch.FindNextTag (overallStartIndex, overallEndIndex, numericString))
		return -1;

	overallStartIndex = overallEndIndex;
	mReportLevel = numericString.ConvertToInteger ();

	if (alternateReportSearch.FindNextTag (overallStartIndex, overallEndIndex, alternateReportString)) {

		overallStartIndex = overallEndIndex;
		alternateReportLevelSearch.ResetSearch ();
		alternateReportCriterionSearch.ResetSearch ();
		localStartIndex = 0;

		if (!alternateReportLevelSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		localStartIndex = localEndIndex;
		mAlternateReportLevel = numericString.ConvertToInteger ();

		if (!alternateReportCriterionSearch.FindNextTag (localStartIndex, localEndIndex, expressionString))
			return -1;

		expressionSearch.ResetSearch ();

		if (!expressionSearch.FindNextTag (0, localEndIndex, mAlternateReportCriterion))
			return -1;
	}

	if (!scopeSearch.FindNextTag (overallStartIndex, overallEndIndex, numericString))
		return -1;

	overallStartIndex = overallEndIndex;
	mScope = numericString.ConvertToInteger ();

	if (callCriterionSearch.FindNextTag (overallStartIndex, overallEndIndex, expressionString)) {

		overallStartIndex = overallEndIndex;
		expressionSearch.ResetSearch ();
		localStartIndex = 0;

		if (!expressionSearch.FindNextTag (localStartIndex, localEndIndex, mCallCriterion))
			return -1;
	}

	if (restrictionSearch.FindNextTag (overallStartIndex, overallEndIndex, restrictionString)) {

		overallStartIndex = overallEndIndex;
		localStartIndex = 0;
		restrictionLevelSearch.ResetSearch ();
		restrictionCriterionSearch.ResetSearch ();

		if (!restrictionLevelSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		localEndIndex = localStartIndex;
		mRestrictionLevel = numericString.ConvertToInteger ();

		if (!restrictionCriterionSearch.FindNextTag (localStartIndex, localEndIndex, expressionString))
			return -1;

		localStartIndex = 0;
		expressionSearch.ResetSearch ();

		if (!expressionSearch.FindNextTag (localStartIndex, localEndIndex, mRestrictionCriterion))
			return -1;
	}

	if (acceptDataSearch.FindNextTag (overallStartIndex, overallEndIndex, numericString)) {

		overallStartIndex = overallEndIndex;
		mAcceptData = numericString.ConvertToInteger ();
	}

	if (shareWithClusterSearch.FindNextTag (overallStartIndex, overallEndIndex, numericString)) {

		overallStartIndex = overallEndIndex;
		mShareWithCluster = numericString.ConvertToInteger ();
	}

	else
		mShareWithCluster = 0;

	if (allowPeakEdit.FindNextTag (overallStartIndex, overallEndIndex, numericString)) {

		overallStartIndex = overallEndIndex;
		mAllowPeakEdit = numericString.ConvertToInteger ();
	}

	else
		mAllowPeakEdit = 1;

	if (SmartMessage::GetDebugMode ()) {

		if (debugSearch.FindNextTag (overallStartIndex, overallEndIndex, watchString)) {

			overallStartIndex = overallEndIndex;
			localStartIndex = 0;
			debugWatchSearch.ResetSearch ();

			while (debugWatchSearch.FindNextTag (localStartIndex, localEndIndex, debugString)) {

				localStartIndex = localEndIndex;
				debugInfo = new DebugInfo;
				*(debugInfo->mName) = debugString;
				debugInfo->mIndex = debugInfo->mScope = 0;
				mWatchInfo.push_back (debugInfo);
			}
		}
	}

	if (editableSearch.FindNextTag (overallStartIndex, overallEndIndex, tempString)) {

		overallStartIndex = overallEndIndex;

		if (tempString == "false")
			mEditable = false;

		else
			mEditable = true;
	}

	if (displayExportSearch.FindNextTag (overallStartIndex, overallEndIndex, tempString)) {

		overallStartIndex = overallEndIndex;
		mExportReportMirrorsOarReport = false;

		if (tempString == "false")
			mDisplayExportInfo = false;

		else
			mDisplayExportInfo = true;
	}

	else {

		mDisplayExportInfo = false;
		mExportReportMirrorsOarReport = true;
	}

	if (protocolSearch.FindNextTag (overallStartIndex, overallEndIndex, mExportProtocolList))
		overallStartIndex = overallEndIndex;

	mValid = 1;
	return 0;
}


int STRBaseSmartMessage :: Configure (const RGString& inputString) {

	return -1;
}



bool STRBaseSmartMessage :: CompileAll () {
	
	// compiles expressions
	// To be completed with flex and bison...

	bool status = true;

	if (mReportCriterion.Length () > 0) {

		mReportExpression = Expression::Compile (mReportCriterion);

		if (mReportExpression == NULL)
			status = false;
	}

	if (mAlternateReportCriterion.Length () > 0) {

		mAlternateReportExpression = Expression::Compile (mAlternateReportCriterion);

		if (mAlternateReportExpression == NULL)
			status = false;
	}

	if (mCallCriterion.Length () > 0) {

		mCallExpression = Expression::Compile (mCallCriterion);

		if (mCallExpression == NULL)
			status = false;
	}

	if (mRestrictionCriterion.Length () > 0) {

		mRestrictionExpression = Expression::Compile (mRestrictionCriterion);

		if (mRestrictionExpression == NULL)
			status = false;
	}

	RGDListIterator triggerUnits (mConditionalTriggerUnits);
	ConditionalTriggerUnit* ctu;

	while (ctu = (ConditionalTriggerUnit*) triggerUnits ()) {

		if (ctu->Compile () < 0)
			status = false;
	}
	
	return status;
}


int STRBaseSmartMessage :: Initialize () {
	
	// replaces named data in expressions with pointers or indexes
	//	bool Evaluate (const bool* msgMatrix, int stage);

	int status = 0;
	int result;

	if (mReportExpression != NULL) {

		result = mReportExpression->Initialize (mScope, this);

		if (result < 0)
			status = -1;
	}

	if (mAlternateReportExpression != NULL) {

		result = mAlternateReportExpression->Initialize (mScope, this);

		if (result < 0)
			status = -1;
	}

	if (mCallExpression != NULL) {

		result = mCallExpression->Initialize (mScope, this);

		if (result < 0)
			status = -1;
	}

	if (mRestrictionExpression != NULL) {

		result = mRestrictionExpression->Initialize (mScope, this);

		if (result < 0)
			status = -1;
	}

	// Now we have to initialize the triggers...

	TriggerInfo* nextLink;
	RGString* nextName;
	RGDListIterator nameIt (mTriggerNames);
	int scope;

	while (nextName = (RGString*) nameIt ()) {

		nextLink = new TriggerInfo;
		result = FindMessageIndex (*nextName, scope);

		if (result < 0) {

			cout << "Could not find message index for trigger named " << nextName->GetData () << endl;
			status = -1;
		}

		if (scope < mScope) {  // triggers are at lower level, which is an error
			
			cout << "Scope mismatch for trigger named " << nextName->GetData () << ".  Scope " << scope << " does not match expected scope " << mScope << endl;
			status = -1;
		}

		nextLink->mIndex = result;
		nextLink->mScope = scope;
		mTriggers.push_back (nextLink);
	}

	RGDListIterator triggerUnits (mConditionalTriggerUnits);
	ConditionalTriggerUnit* ctu;

	while (ctu = (ConditionalTriggerUnit*) triggerUnits ()) {

		if (ctu->Initialize (mScope, this) < 0)
			status = -1;
	}

	if (SmartMessage::GetDebugMode ()) {

		list<DebugInfo*>::iterator c1Iterator;
		DebugInfo* nextDebugLink;

		for (c1Iterator = mWatchInfo.begin (); c1Iterator != mWatchInfo.end (); c1Iterator++) {

			nextDebugLink = *c1Iterator;
			result = FindMessageIndex (*(nextDebugLink->mName), scope);

			if (result < 0) {

				cout << "Could not find message index for debug watch element named " << (nextDebugLink->mName)->GetData () << endl;
			}

			else if (scope < mScope) {  // debug watch is at lower level, which is an error
				
				cout << "Scope mismatch for debug watch element named " << (nextDebugLink->mName)->GetData () << ".  Scope " << scope << " does not match expected scope " << mScope << endl;
				result = -1;
			}

			if (result < 0) {

//				c1Iterator = mWatchInfo.erase (c1Iterator);
//				delete nextDebugLink;
				status = -1;
			}

			else {

				nextDebugLink->mIndex = result;
				nextDebugLink->mScope = scope;
			}
		}

		if (mWatchInfo.size () > 0)
			mDebugOn = true;
	}

	if (status < 0)
		STRLCAnalysis::mFailureMessage->AddMessage (GetName ());

	return status;
}


int STRBaseSmartMessage :: GetThreshold () const {

	return 0;
}


void STRBaseSmartMessage :: SetThreshold (int threshold) {

}


void STRBaseSmartMessage :: ImportTriggerNames (const RGString& expression) {

	// extract names and append to mTriggerNames

	RGString localString (expression);
	size_t start = 0;

	RGStringTokenizer ExpressionData (localString);
	ExpressionData.AddDelimiter (',');
	ExpressionData.AddDelimiter (';');
	ExpressionData.AddRemoveItem (':');
	ExpressionData.AddRemoveItem (' ');

	RGString token;
	RGString delim;
	size_t position;
	RGString* newString;

	while (ExpressionData.NextToken (token, delim)) {

		if (token.FindNextSubstring (start, "sm", position)) {

			newString = new RGString (token);
			mTriggerNames.Append (newString);
		}
	}
}


int STRBaseSmartMessage :: ImportTriggerCondition (const RGString& expression) {

	ConditionalTriggerUnit* ctu = new ConditionalTriggerUnit (expression);

	if (!ctu->isValid ()) {

		delete ctu;
		return -1;
	}

	mConditionalTriggerUnits.Append (ctu);
	return 0;
}


bool STRBaseSmartMessage :: Evaluate (bool* const msgMatrix, RGDList& subObjects) const {

	return false;
}



bool STRBaseSmartMessage :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	return false;
}


int STRBaseSmartMessage :: EvaluateReportLevel (const bool* msgMatrix) const {

	if (!msgMatrix [mWhichElementWithinDataArray])
		return -1;

	bool altAns = false;

	if (mAlternateReportExpression != NULL)
		altAns = mAlternateReportExpression->EvaluateLocal (msgMatrix);

	if (altAns)
		return mAlternateReportLevel;

	return mReportLevel;
}


int STRBaseSmartMessage :: EvaluateReportLevel (SmartMessagingComm& comm, int numHigherObjects) const {

	int i;
	SmartMessagingObject* smo;
	bool ans = false;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];

		if (mScope == smo->GetObjectScope ()) {

			ans = smo->GetMessageValue (mWhichElementWithinDataArray);

			if (!ans)
				return -1;

			bool altAns = false;

			if (mAlternateReportExpression != NULL)
				altAns = mAlternateReportExpression->Evaluate (comm, numHigherObjects);

			if (altAns)
				return mAlternateReportLevel;

			return mReportLevel;
		}
	}

	return mReportLevel;
}


bool STRBaseSmartMessage :: EvaluateReport (const bool* msgMatrix) const {

	if (!msgMatrix [mWhichElementWithinDataArray])
		return false;

	if (mReportExpression != NULL)
		return mReportExpression->EvaluateLocal (msgMatrix);

	return true;
}


bool STRBaseSmartMessage :: EvaluateReport (SmartMessagingComm& comm, int numHigherObjects) const {

	int i;
	SmartMessagingObject* smo;
	bool ans = false;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];

		if (mScope == smo->GetObjectScope ()) {

			ans = smo->GetMessageValue (mWhichElementWithinDataArray);

			if (!ans)
				return false;

			if (mReportExpression != NULL)
				return mReportExpression->Evaluate (comm, numHigherObjects);

			return true;
		}
	}

	return true;
}


bool STRBaseSmartMessage :: EvaluateReportContingent (SmartMessagingComm& comm, int numHigherObjects) const {

	int i;
	SmartMessagingObject* smo;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];

		if (mScope == smo->GetObjectScope ()) {

			if (mReportExpression != NULL)
				return mReportExpression->Evaluate (comm, numHigherObjects);

			return true;
		}
	}

	return true;
}


bool STRBaseSmartMessage :: EvaluateCall (const bool* msgMatrix) const {

	if (!msgMatrix [mWhichElementWithinDataArray])
		return true;

	if (mCallExpression != NULL)
		return mCallExpression->EvaluateLocal (msgMatrix);

	return true;
}


bool STRBaseSmartMessage :: EvaluateCall (SmartMessagingComm& comm, int numHigherObjects) const {

	int i;
	SmartMessagingObject* smo;
	bool ans = false;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];

		if (mScope == smo->GetObjectScope ()) {

			ans = smo->GetMessageValue (mWhichElementWithinDataArray);

			if (!ans)
				return true;

			if (mCallExpression != NULL)
				return mCallExpression->Evaluate (comm, numHigherObjects);

			return true;
		}
	}

	return true;
}


bool STRBaseSmartMessage :: EvaluateRestriction (const bool* msgMatrix) const {

	if (!msgMatrix [mWhichElementWithinDataArray])
		return false;

	if (mRestrictionExpression != NULL)
		return mRestrictionExpression->EvaluateLocal (msgMatrix);

	return false;
}


bool STRBaseSmartMessage :: EvaluateRestriction (SmartMessagingComm& comm, int numHigherObjects) const {

	int i;
	SmartMessagingObject* smo;
	bool ans = false;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];

		if (mScope == smo->GetObjectScope ()) {

			ans = smo->GetMessageValue (mWhichElementWithinDataArray);

			if (!ans)
				return false;

			if (mRestrictionExpression != NULL)
				return mRestrictionExpression->Evaluate (comm, numHigherObjects);

			return false;
		}
	}

	return false;
}



int STRBaseSmartMessage :: EvaluateRestrictionLevel (const bool* msgMatrix) const {

	if (EvaluateRestriction (msgMatrix))
		return mRestrictionLevel;

	return -1;
}


int STRBaseSmartMessage :: EvaluateRestrictionLevel (SmartMessagingComm& comm, int numHigherObjects) const {

	if (EvaluateRestriction (comm, numHigherObjects))
		return mRestrictionLevel;

	return -1;
}



bool STRBaseSmartMessage :: OutputDebugInfo (SmartMessagingComm& comm, int numHigherObjects, int stage) const {

	if (mDebugOn) {

		list<DebugInfo*>::const_iterator c1Iterator;
		DebugInfo* nextLink;
		int scope;
		int index;
		bool value;
		int i;
		int nextToLastObject = numHigherObjects - 1;
		SmartMessagingObject* targetSMO;
		targetSMO = comm.SMOStack [numHigherObjects - 1];
		targetSMO->OutputDebugID (comm, numHigherObjects);
		bool foundScope;
		RGString indent = targetSMO->GetDebugIDIndent () + "\t";
		RGString dataOutput;
		dataOutput << indent << "Stage:  " << stage;
		SmartMessage::OutputDebugString (dataOutput);
		dataOutput = "";
		dataOutput << indent << "Debug:  " << mName << " = ";
		value = targetSMO->GetMessageValue (mWhichElementWithinDataArray);

		if (value)
			dataOutput << "true";

		else
			dataOutput << "false";

		SmartMessage::OutputDebugString (dataOutput);

		for (c1Iterator = mWatchInfo.begin (); c1Iterator != mWatchInfo.end (); c1Iterator++) {

			nextLink = *c1Iterator;
			scope = nextLink->mScope;
			index = nextLink->mIndex;
			foundScope = false;

			for (i=nextToLastObject; i>=0; i--) {

				targetSMO = comm.SMOStack [i];

				if (scope == targetSMO->GetObjectScope ()) {

					value = targetSMO->GetMessageValue (index);
					foundScope = true;
					break;
				}
			}

			if (foundScope) {

				dataOutput = "";
				dataOutput << indent << "Watch:  " << *(nextLink->mName) << " = ";

				if (value)
					dataOutput << "true";

				else
					dataOutput << "false";

				SmartMessage::OutputDebugString (dataOutput);
			}
		}

		dataOutput = "";
		SmartMessage::OutputDebugString (dataOutput);
	}

	return true;
}



bool STRBaseSmartMessage :: SetAllTriggers (bool* const msgMatrix, bool* const higherMsgMatrix) const {

	// Iterate through mTriggers and set each in appropriate list.
	// Requires that various objects, such as DataSignal, Locus, etc., have a member function called SetAllTriggers
	// that takes, as an argument, the bool array from the calling object.

	if (msgMatrix [mWhichElementWithinDataArray]) {

		list<TriggerInfo*>::const_iterator c1Iterator;
		TriggerInfo* nextLink;

		for (c1Iterator = mTriggers.begin (); c1Iterator != mTriggers.end (); c1Iterator++) {

			nextLink = *c1Iterator;
			
			if (nextLink->mScope == mScope)
				msgMatrix [nextLink->mIndex] = true;

			else if (higherMsgMatrix != NULL)
				higherMsgMatrix [nextLink->mIndex] = true;

			else
				return false;
		}

		return true;
	}
	
	return false;
}


bool STRBaseSmartMessage :: SetAllTriggers (bool* const msgMatrix, bool* const higherMsgMatrix, RGHashTable* messageDataTable, int stage, int scope, int higherScope, int intBP, const RGString& alleleName) const {

	// Iterate through mTriggers and set each in appropriate list.
	// Requires that various objects, such as DataSignal, Locus, etc., have a member function called SetAllTriggers
	// that takes, as an argument, the bool array from the calling object.

	RGString text;
	SmartMessageData* msg;
	SmartMessageData target;

	if (msgMatrix [mWhichElementWithinDataArray]) {

		list<TriggerInfo*>::const_iterator c1Iterator;
		TriggerInfo* nextLink;

		for (c1Iterator = mTriggers.begin (); c1Iterator != mTriggers.end (); c1Iterator++) {

			nextLink = *c1Iterator;
			int index = nextLink->mIndex;
			
			if (nextLink->mScope == mScope)
				msgMatrix [index] = true;

			else if (higherMsgMatrix != NULL) {

				higherMsgMatrix [index] = true;

				if (messageDataTable == NULL)
					continue;

				switch (mAcceptData) {

					case 0:
						break;

					case 1:
					case 2:
						target.SetIndex (index);
						msg = (SmartMessageData*)messageDataTable->Find (&target);
						text = "";

						if (mAcceptData == 1)
							text << intBP;

						else
							text = alleleName;

						if (msg == NULL)
							msg = new SmartMessageData (index, higherScope, text);
						
						else
							msg->AppendText (text, index, higherScope);

						break;

					default:
						break;
				}
			}

			else
				return false;
		}

		return true;
	}
	
	return false;
}


bool STRBaseSmartMessage :: SetAllTriggers (bool** const msgMatrices, RGHashTable** messageDataTables, int stage, int* scopes, int numScopes, int intBP, const RGString& alleleName) const {

	RGString text;
	SmartMessageData* msg;
	SmartMessageData target;
	int i;
	int acceptData;
	SmartMessage* triggeredMsg;

	if (numScopes == 0)
		return false;

	if (msgMatrices [0][mWhichElementWithinDataArray]) {

		list<TriggerInfo*>::const_iterator c1Iterator;
		TriggerInfo* nextLink;

		for (c1Iterator = mTriggers.begin (); c1Iterator != mTriggers.end (); c1Iterator++) {

			nextLink = *c1Iterator;
			int index = nextLink->mIndex;
			int scope = nextLink->mScope;
			
			if (nextLink->mScope == mScope)
				msgMatrices [0][index] = true;

			else {

				for (i=0; i<numScopes; i++) {

					if (scope == scopes [i]) {

						msgMatrices [i][index] = true;

						if (messageDataTables [i] != NULL) {

							triggeredMsg = SmartMessage::GetSmartMessageForScopeAndElement (scope, index);

							if (triggeredMsg != NULL) {

								acceptData = triggeredMsg->GetAcceptData ();

								switch (acceptData) {

									case 0:
										break;

									case 1:
									case 2:
										target.SetIndex (index);
										msg = (SmartMessageData*)messageDataTables [i]->Find (&target);
										text = "";

										if (acceptData == 1)
											text << intBP;

										else
											text = alleleName;

										if (msg == NULL)
											msg = new SmartMessageData (index, scope, text);
										
										else
											msg->AppendText (text, index, scope);

										break;

									default:
										break;
								}
							}
						}
					}
				}
			}
		}

		return true;
	}

	return false;
}


bool STRBaseSmartMessage :: SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage) {

	int i;
	int lastObject = numHigherObjects - 1;
	int nextToLastObject = lastObject - 1;

	if (lastObject < 0)
		return false;

	SmartMessagingObject* primeSMO = comm.SMOStack [lastObject];
	SmartMessagingObject* targetSMO;

	if (primeSMO->GetMessageValue (mWhichElementWithinDataArray)) {

		list<TriggerInfo*>::const_iterator c1Iterator;
		TriggerInfo* nextLink;

		for (c1Iterator = mTriggers.begin (); c1Iterator != mTriggers.end (); c1Iterator++) {

			nextLink = *c1Iterator;
			int index = nextLink->mIndex;
			int scope = nextLink->mScope;
			
			if (nextLink->mScope == mScope)
				primeSMO->SetMessageValue (scope, index, true, false);

			else {

				for (i=nextToLastObject; i>=0; i--) {

					targetSMO = comm.SMOStack [i];

					if (scope == targetSMO->GetObjectScope ()) {

						targetSMO->SetMessageValue (scope, index, true);
						break;
					}
				}
			}
		}

		RGDListIterator triggerUnits (mConditionalTriggerUnits);
		ConditionalTriggerUnit* ctu;

		while (ctu = (ConditionalTriggerUnit*) triggerUnits ())
			ctu->SetAllTriggers (comm, numHigherObjects, stage);

		return true;
	}

	return false;
}



bool STRBaseSmartMessage :: SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage, int intBP, const RGString& alleleName) {

	int i;
	int lastObject = numHigherObjects - 1;
	int nextToLastObject = lastObject - 1;

	if (lastObject < 0)
		return false;

	SmartMessagingObject* primeSMO = comm.SMOStack [lastObject];
	SmartMessagingObject* targetSMO;

	if (primeSMO->GetMessageValue (mWhichElementWithinDataArray)) {

		list<TriggerInfo*>::const_iterator c1Iterator;
		TriggerInfo* nextLink;

		for (c1Iterator = mTriggers.begin (); c1Iterator != mTriggers.end (); c1Iterator++) {

			nextLink = *c1Iterator;
			int index = nextLink->mIndex;
			int scope = nextLink->mScope;
			
			if (nextLink->mScope == mScope)
				primeSMO->SetMessageValue (scope, index, true, false);

			else {

				for (i=nextToLastObject; i>=0; i--) {

					targetSMO = comm.SMOStack [i];

					if (scope == targetSMO->GetObjectScope ()) {

						targetSMO->SetMessageValue (scope, index, true);

						if (targetSMO->AcceptsDataOnTrigger (index))
							targetSMO->AppendDataForSmartMessage (scope, index, intBP, alleleName);

						break;
					}
				}
			}
		}

		RGDListIterator triggerUnits (mConditionalTriggerUnits);
		ConditionalTriggerUnit* ctu;

		while (ctu = (ConditionalTriggerUnit*) triggerUnits ())
			ctu->SetAllTriggers (comm, numHigherObjects, stage, intBP, alleleName);

		return true;
	}

	return false;
}


bool STRBaseSmartMessage :: AddDataItem (const RGString& data, RGHashTable* dataArray) const {

	SmartMessageData test (mWhichElementWithinDataArray);
	SmartMessageData* label = (SmartMessageData*)dataArray->Find (&test);

	if (label == NULL) {

		label = new SmartMessageData (mWhichElementWithinDataArray, mScope, data);
		dataArray->Insert (label);
	}

	else {

		RGString existingText = label->GetText ();
		existingText += mSeparatorText + data;
		label->SetText (existingText);
	}

	return true;
}


int STRBaseSmartMessage :: FindMessageIndex (const RGString& name, int& whichDataArray) const {

	STRBaseSmartMessage sm (0, name);
	return SmartMessage::GetMessageIndex (&sm, whichDataArray);
}


SmartMessage* STRBaseSmartMessage :: FindMessageByName (const RGString& name) const {

	int scope;
	int element = FindMessageIndex (name, scope);

	if (element < 0)
		return NULL;

	return SmartMessage::GetSmartMessageForScopeAndElement (scope, element);
}



bool STRBaseSmartMessage :: IsTrue (const bool* msgMatrix) const {

	return msgMatrix [mWhichElementWithinDataArray];
}


bool STRBaseSmartMessage :: Report (const bool* msgMatrix) const {

	return EvaluateReport (msgMatrix);
}


bool STRBaseSmartMessage :: IsCritical (const bool* msgMatrix) const {

	int level = EvaluateReportLevel (msgMatrix);
	int criticalLevel = SmartMessage::SeverityTrigger;  // !!!!!!Change this when ready to integrate with OsirisLib

	if ((level > 0) && (level < criticalLevel))
		return true;

	return false;
}


bool STRBaseSmartMessage :: IsEditable () const {

	return mEditable;
}


bool STRBaseSmartMessage :: DontCall (const bool* msgMatrix) const {

	return !EvaluateCall (msgMatrix);
}



RGString STRBaseSmartMessage :: GetData (const RGString* dataArray) {

	RGString a;
	return a;
}


RGString STRBaseSmartMessage :: GetExportProtocolList () const {

	return mExportProtocolList;
}


bool STRBaseSmartMessage :: HasExportProtocolInfo () const {

	return (mExportProtocolList.Length () > 0);
}



bool STRBaseSmartMessage :: UseDefaultExportDisplayMode () const {

	return mExportReportMirrorsOarReport;
}


bool STRBaseSmartMessage :: DisplayExportInfo () const {

	return mDisplayExportInfo;
}


int STRBaseSmartMessage :: GetReportLevel (bool* msgMatrix) const {

	return EvaluateReportLevel (msgMatrix);
}


void STRBaseSmartMessage :: SetMessageValue (bool* const msgMatrix, int* const valueMatrix, int index, bool value) {

	msgMatrix [index] = value;
}


size_t STRBaseSmartMessage :: StoreSize () const {

	size_t size = SmartMessage::StoreSize () + 2 * sizeof (int) + 3 * sizeof (Expression*);
	size += mReportCriterion.StoreSize ();
	size += mAlternateReportCriterion.StoreSize () + mCallCriterion.StoreSize ();
	size += mTriggerNames.StoreSize ();
	return size;
}


void STRBaseSmartMessage :: RestoreAll (RGFile& f) {

	SmartMessage::RestoreAll (f);
	mReportCriterion.RestoreAll (f);
	f.Read (mReportLevel);
	f.Read (mAlternateReportLevel);
	mAlternateReportCriterion.RestoreAll (f);
	mCallCriterion.RestoreAll (f);
	mTriggerNames.ClearAndDelete ();
	mTriggerNames.RestoreAll (f);
}


void STRBaseSmartMessage :: RestoreAll (RGVInStream& f) {

	SmartMessage::RestoreAll (f);
	mReportCriterion.RestoreAll (f);
	f >> mReportLevel;
	f >> mAlternateReportLevel;
	mAlternateReportCriterion.RestoreAll (f);
	mCallCriterion.RestoreAll (f);
	mTriggerNames.ClearAndDelete ();
	mTriggerNames.RestoreAll (f);
}


void STRBaseSmartMessage :: SaveAll (RGFile& f) const {

	SmartMessage::SaveAll (f);
	mReportCriterion.SaveAll (f);
	f.Write (mReportLevel);
	f.Write (mAlternateReportLevel);
	mAlternateReportCriterion.SaveAll (f);
	mCallCriterion.SaveAll (f);
	mTriggerNames.SaveAll (f);
}


void STRBaseSmartMessage :: SaveAll (RGVOutStream& f) const {

	SmartMessage::SaveAll (f);
	mReportCriterion.SaveAll (f);
	f << mReportLevel;
	f << mAlternateReportLevel;
	mAlternateReportCriterion.SaveAll (f);
	mCallCriterion.SaveAll (f);
	mTriggerNames.SaveAll (f);
}


int STRBaseSmartMessage :: ConfigureMsgNumToSampleType (const RGString& inputString, size_t startIndex, size_t& overallEndIndex) {

	mValid = 0;

	RGString localInputString (inputString);
//	RGXMLTagSearch nameSearch ("Name", localInputString);
	RGXMLTagSearch descriptionSearch ("Description", localInputString);
	RGXMLTagSearch sampleTypeSearch ("SampleType", localInputString);

	RGString numericString;
	size_t overallStartIndex = startIndex;

	//if (!nameSearch.FindNextTag (overallStartIndex, overallEndIndex, mName))
	//	return -1;

	//overallStartIndex = overallEndIndex;

	if (descriptionSearch.FindNextTag (overallStartIndex, overallEndIndex, mDescription))
		overallStartIndex = overallEndIndex;

	if (!sampleTypeSearch.FindNextTag (overallStartIndex, overallEndIndex, numericString))
		return -1;

	overallStartIndex = overallEndIndex;
	mSampleType = numericString.ConvertToInteger ();

	return 0;
}



int STRBaseSmartMessage :: ConfigureReportCriterionToEnd (const RGString& inputString, size_t startIndex) {

	RGString localInputString (inputString);
	RGXMLTagSearch reportCriterionSearch ("ReportCriterion", localInputString);
	RGXMLTagSearch textSearch ("Text", localInputString);
	RGXMLTagSearch textForDataSearch ("TextForData", localInputString);
	RGXMLTagSearch separatorTextSearch ("SeparatorText", localInputString);
	RGXMLTagSearch closingTextSearch ("ClosingText", localInputString);
	RGXMLTagSearch reportLevelSearch ("ReportLevel", localInputString);
	RGXMLTagSearch alternateReportSearch ("AlternateReport", localInputString);
	RGXMLTagSearch scopeSearch ("Scope", localInputString);
	RGXMLTagSearch callCriterionSearch ("CallCriterion", localInputString);
	RGXMLTagSearch triggersSearch ("Triggers", localInputString);
	RGXMLTagSearch restrictionSearch ("Restriction", localInputString);
	RGXMLTagSearch acceptDataSearch ("AcceptTriggerDataMode", localInputString);
	RGXMLTagSearch shareWithClusterSearch ("ShareWithCluster", localInputString);
	RGXMLTagSearch debugSearch ("Debug", localInputString);

	RGString numericString;
	RGString expressionString;
	RGString alternateReportString;
	RGString triggerString;
	RGString triggerName;
	RGString restrictionString;
	RGString watchString;
	RGString debugString;

	RGXMLTagSearch alternateReportLevelSearch ("AlternateReportLevel", alternateReportString);
	RGXMLTagSearch alternateReportCriterionSearch ("AlternateReportCriterion", alternateReportString);
	RGXMLTagSearch expressionSearch ("Expression", expressionString);
	RGXMLTagSearch triggerSearch ("TriggerName", triggerString);
	RGXMLTagSearch restrictionLevelSearch ("RestrictionLevel", restrictionString);
	RGXMLTagSearch restrictionCriterionSearch ("RestrictionCriterion", restrictionString);
	RGXMLTagSearch debugWatchSearch ("Watch", watchString);

	size_t overallStartIndex = startIndex;
	size_t overallEndIndex;

	size_t localStartIndex = 0;
	size_t localEndIndex;
	DebugInfo* debugInfo;

	if (!reportCriterionSearch.FindNextTag (overallStartIndex, overallEndIndex, expressionString))
		return -1;

	overallStartIndex = overallEndIndex;
	expressionSearch.ResetSearch ();
	localStartIndex = 0;

	if (!expressionSearch.FindNextTag (localStartIndex, localEndIndex, mReportCriterion))
		return -1;

	if (!textSearch.FindNextTag (overallStartIndex, overallEndIndex, mMessageText))
		return -1;

	overallStartIndex = overallEndIndex;

	if (textForDataSearch.FindNextTag (overallStartIndex, overallEndIndex, mAdditionalTextForData))
		overallStartIndex = overallEndIndex;

	if (separatorTextSearch.FindNextTag (overallStartIndex, overallEndIndex, mSeparatorText))
		overallStartIndex = overallEndIndex;

	if (closingTextSearch.FindNextTag (overallStartIndex, overallEndIndex, mClosingText))
		overallStartIndex = overallEndIndex;

	if (!reportLevelSearch.FindNextTag (overallStartIndex, overallEndIndex, numericString))
		return -1;

	overallStartIndex = overallEndIndex;
	mReportLevel = numericString.ConvertToInteger ();

	if (alternateReportSearch.FindNextTag (overallStartIndex, overallEndIndex, alternateReportString)) {

		overallStartIndex = overallEndIndex;
		alternateReportLevelSearch.ResetSearch ();
		alternateReportCriterionSearch.ResetSearch ();
		localStartIndex = 0;

		if (!alternateReportLevelSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		localStartIndex = localEndIndex;
		mAlternateReportLevel = numericString.ConvertToInteger ();

		if (!alternateReportCriterionSearch.FindNextTag (localStartIndex, localEndIndex, expressionString))
			return -1;

		expressionSearch.ResetSearch ();

		if (!expressionSearch.FindNextTag (0, localEndIndex, mAlternateReportCriterion))
			return -1;
	}

	if (!scopeSearch.FindNextTag (overallStartIndex, overallEndIndex, numericString))
		return -1;

	overallStartIndex = overallEndIndex;
	mScope = numericString.ConvertToInteger ();

	if (callCriterionSearch.FindNextTag (overallStartIndex, overallEndIndex, expressionString)) {

		overallStartIndex = overallEndIndex;
		expressionSearch.ResetSearch ();
		localStartIndex = 0;

		if (!expressionSearch.FindNextTag (localStartIndex, localEndIndex, mCallCriterion))
			return -1;
	}

	RGString* newTriggerName;

	if (triggersSearch.FindNextTag (overallStartIndex, overallEndIndex, triggerString)) {

		overallStartIndex = overallEndIndex;
		localStartIndex = 0;
		triggerSearch.ResetSearch ();

		while (triggerSearch.FindNextTag (localStartIndex, localEndIndex, triggerName)) {

			localStartIndex = localEndIndex;
			newTriggerName = new RGString (triggerName);
			mTriggerNames.Append (newTriggerName);
		}

		if (mTriggerNames.Entries () == 0)
			return -1;
	}

	if (restrictionSearch.FindNextTag (overallStartIndex, overallEndIndex, restrictionString)) {

		overallStartIndex = overallEndIndex;
		localStartIndex = 0;
		restrictionLevelSearch.ResetSearch ();
		restrictionCriterionSearch.ResetSearch ();

		if (!restrictionLevelSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		localEndIndex = localStartIndex;
		mRestrictionLevel = numericString.ConvertToInteger ();

		if (!restrictionCriterionSearch.FindNextTag (localStartIndex, localEndIndex, expressionString))
			return -1;

		localStartIndex = 0;
		expressionSearch.ResetSearch ();

		if (!expressionSearch.FindNextTag (localStartIndex, localEndIndex, mRestrictionCriterion))
			return -1;
	}

	if (acceptDataSearch.FindNextTag (overallStartIndex, overallEndIndex, numericString)) {

		overallStartIndex = overallEndIndex;
		mAcceptData = numericString.ConvertToInteger ();
	}

	if (shareWithClusterSearch.FindNextTag (overallStartIndex, overallEndIndex, numericString)) {

		overallStartIndex = overallEndIndex;
		mShareWithCluster = numericString.ConvertToInteger ();
	}

	else
		mShareWithCluster = 0;

	if (SmartMessage::GetDebugMode ()) {

		if (debugSearch.FindNextTag (overallStartIndex, overallEndIndex, watchString)) {

			overallStartIndex = overallEndIndex;
			localStartIndex = 0;
			debugWatchSearch.ResetSearch ();

			while (debugWatchSearch.FindNextTag (localStartIndex, localEndIndex, debugString)) {

				localStartIndex = localEndIndex;
				debugInfo = new DebugInfo;
				*(debugInfo->mName) = debugString;
				debugInfo->mIndex = debugInfo->mScope = 0;
				mWatchInfo.push_back (debugInfo);
			}
		}
	}

	mValid = 1;
	return 0;
}


int STRBaseSmartMessage :: LoadAndActivateAllMessages (const RGString& messageFullPathFileName) {

	SmartMessage* sm = new STRBaseSmartMessage;
	int status = SmartMessage::LoadAndActivateAllMessages (messageFullPathFileName, sm);
	delete sm;
	return status;
}


int STRBaseSmartMessage :: LoadAndActivateAllThresholds (const RGString& thresholdFullPathFileName) {

	RGFile inputFile (thresholdFullPathFileName, "rt");

	if (!inputFile.isValid ()) {

		cout << "Could not open smart messagebook threshold file:  " << thresholdFullPathFileName.GetData () << endl;
		return -1;
	}

	RGString inputString;
	inputString.ReadTextFile (inputFile);
	size_t overallStartIndex = 0;
	size_t overallEndIndex;
	size_t localStartIndex = 0;
	size_t localEndIndex;
	RGString thresholdString;
	RGString name;
	RGString numericString;
	int threshold;

	RGXMLTagSearch thresholdInfoSearch ("MessageBookThreshold", inputString);
	RGXMLTagSearch messageNameSearch ("MessageName", thresholdString);
	RGXMLTagSearch valueSearch ("Value", thresholdString);

	SmartMessage* sm;
	STRBaseSmartMessage bsm;

	while (thresholdInfoSearch.FindNextTag (overallStartIndex, overallEndIndex, thresholdString)) {

		overallStartIndex = overallEndIndex;
		messageNameSearch.ResetSearch ();
		valueSearch.ResetSearch ();
		localStartIndex = 0;

		if (messageNameSearch.FindNextTag (localStartIndex, localEndIndex, name))
			localStartIndex = localEndIndex;

		else {

			cout << "Expected message name in threshold list not found..." << endl;
			continue;
		}

		if (valueSearch.FindNextTag (localStartIndex, localEndIndex, numericString)) {

			localStartIndex = localEndIndex;
			threshold = numericString.ConvertToInteger ();
		}

		else {

			cout << "Could not find threshold for message named " << name.GetData () << endl;
			continue;
		}

		sm = bsm.FindMessageByName (name);

		if (sm != NULL)
			sm->SetThreshold (threshold);

		else
			cout << "Could not find threshold message named " << name.GetData () << endl;
	}

	return 0;
}



void STRBaseSmartMessage :: ClearTriggerList () {

	list<TriggerInfo*>::const_iterator c1Iterator;
	TriggerInfo* nextLink;

	for (c1Iterator = mTriggers.begin (); c1Iterator != mTriggers.end (); c1Iterator++) {

		nextLink = *c1Iterator;
		delete nextLink;
	}

	mTriggers.clear ();
}


void STRBaseSmartMessage :: ClearDebugList () {

	list<DebugInfo*>::const_iterator c1Iterator;
	DebugInfo* nextLink;

	for (c1Iterator = mWatchInfo.begin (); c1Iterator != mWatchInfo.end (); c1Iterator++) {

		nextLink = *c1Iterator;
		delete nextLink;
	}

	mWatchInfo.clear ();
}



STRSmartMessage :: STRSmartMessage () : STRBaseSmartMessage (), mTestExpression (NULL) {

}


STRSmartMessage :: STRSmartMessage (int integerData, const RGString& name) : STRBaseSmartMessage (integerData, name), 
mTestExpression (NULL) {

}



STRSmartMessage :: STRSmartMessage (const STRSmartMessage& sm) : STRBaseSmartMessage (sm), mTestCriterion (sm.mTestCriterion),
mTestExpression (sm.mTestExpression) {

	if (sm.mTestExpression != NULL)
		mTestExpression = sm.mTestExpression->MakeCopy ();
}



STRSmartMessage :: ~STRSmartMessage () {

	delete mTestExpression;
}


void STRSmartMessage :: ImportTestCriterion (const RGString& expression) {

	mTestCriterion = expression;
}


int STRSmartMessage :: Configure (const RGString& inputString) {
	
	// extracts string data

	mValid = 0;

	RGString localInputString (inputString);
	RGXMLTagSearch testCriterionSearch ("TestCriterion", localInputString);

	RGString expressionString;
	RGXMLTagSearch expressionSearch ("Expression", expressionString);


	size_t overallStartIndex = 0;
	size_t overallEndIndex;

	size_t localStartIndex = 0;
	size_t localEndIndex;

	if (ConfigureMsgNumToSampleType (localInputString, overallStartIndex, overallEndIndex) < 0)
		return -1;

	overallStartIndex = overallEndIndex;

	if (testCriterionSearch.FindNextTag (overallStartIndex, overallEndIndex, expressionString)) {

		overallStartIndex = overallEndIndex;
		expressionSearch.ResetSearch ();
		localStartIndex = 0;

		if (!expressionSearch.FindNextTag (localStartIndex, localEndIndex, mTestCriterion))
			return -1;
	}

	if (ConfigureReportCriterionToEnd (localInputString, overallStartIndex) < 0)
		return -1;

	mValid = 1;
	return 0;
}



bool STRSmartMessage :: CompileAll () {
	
	// compiles expressions
	// To be completed with flex and bison...

	bool status = true;

	if (mTestCriterion.Length () > 0) {

		mTestExpression = Expression::Compile (mTestCriterion);

		if (mTestExpression == NULL)
			status = false;
	}

	if (!STRBaseSmartMessage::CompileAll ())
		status = false;
	
	return status;
}


int STRSmartMessage :: Initialize () {
	
	// replaces named data in expressions with pointers or indexes
	//	bool Evaluate (const bool* msgMatrix, int stage);

	int status = 0;
	int result;

	if (mTestExpression != NULL) {
	
		result = mTestExpression->Initialize (mScope, this);

		if (result < 0)
			status = -1;
	}

	result = STRBaseSmartMessage::Initialize ();

	if (result < 0)
		status = -1;

	return status;
}


int STRSmartMessage :: ConfigureV4 (const RGString& inputString, size_t startIndex) {

	// extracts string data from Version 4.x SmartMessage book declarations

	mValid = 0;

	size_t overallStartIndex = startIndex;
	size_t overallEndIndex = startIndex;

	if (ConfigureDescriptionToSampleTypeV4 (inputString, overallStartIndex, overallEndIndex) < 0)
		return -1;

	overallStartIndex = overallEndIndex;

	if (ConfigureReportCriterionToEndV4 (inputString, overallStartIndex) < 0)	// change to ConfigureReportCriterionToEndV4
		return -1;

	mValid = 1;
	return 0;
}


bool STRSmartMessage :: Evaluate (bool* const msgMatrix, RGDList& subObjects) const {

	bool ans = msgMatrix [mWhichElementWithinDataArray];
	
	if (mTestExpression != NULL) {

		ans = mTestExpression->Evaluate (msgMatrix, subObjects);
		msgMatrix [mWhichElementWithinDataArray] = ans;
	}

	return ans;
}


bool STRSmartMessage :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	int i;
	SmartMessagingObject* smo;
	bool ans = false;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];

		if (mScope == smo->GetObjectScope ()) {

			ans = smo->GetMessageValue (mWhichElementWithinDataArray);

			if ((mTestExpression != NULL) && !(ans && mShareWithCluster)) {

				ans = mTestExpression->Evaluate (comm, numHigherObjects);
				smo->SetMessageValue (mScope, mWhichElementWithinDataArray, ans, false);
			}

			if (mDebugOn && ans) {

				OutputDebugInfo (comm, i + 1, mStage);
			}
			 
			return ans;
		}
	}

	return false;
}


size_t STRSmartMessage :: StoreSize () const {

	size_t size = STRBaseSmartMessage::StoreSize () + sizeof (Expression*);
	size += mTestCriterion.StoreSize ();
	return size;
}


/*
	RGString mTestCriterion;  // expression to be evaluated; if empty, this is a primitive, hard set by code
	RGString mReportCriterion; // expression to be evaluated, or true or false, to determine if message is to be reported
	int mReportLevel;  // 1 is most critical; the larger the number, the less critical
	int mAlternateReportLevel;
	RGString mAlternateReportCriterion;  // expression to determine if alternate report level is to be used
	RGString mCallCriterion;  // expression to determine if peak is to be a called allele (relavent only for DataSignals)
	RGDList mTriggerNames;
*/



void STRSmartMessage :: RestoreAll (RGFile& f) {

	STRBaseSmartMessage::RestoreAll (f);
	mTestCriterion.RestoreAll (f);
}


void STRSmartMessage :: RestoreAll (RGVInStream& f) {

	STRBaseSmartMessage::RestoreAll (f);
	mTestCriterion.RestoreAll (f);
}


void STRSmartMessage :: SaveAll (RGFile& f) const {

	STRBaseSmartMessage::SaveAll (f);
	mTestCriterion.SaveAll (f);
}


void STRSmartMessage :: SaveAll (RGVOutStream& f) const {

	STRBaseSmartMessage::SaveAll (f);
	mTestCriterion.SaveAll (f);
}


STRSmartCountingMessage :: STRSmartCountingMessage () : STRBaseSmartMessage (), mThreshold (0), mTestForGreater (true) {

	mAcceptData = 3;
}



STRSmartCountingMessage :: STRSmartCountingMessage (int integerData, const RGString& name) : STRBaseSmartMessage (integerData, name),
mThreshold (0), mTestForGreater (true) {

	mAcceptData = 3;
}



STRSmartCountingMessage :: STRSmartCountingMessage (const STRSmartCountingMessage& sm) : STRBaseSmartMessage (sm),
mThreshold (sm.mThreshold), mTestForGreater (sm.mTestForGreater) {

	mAcceptData = sm.mAcceptData;
}


STRSmartCountingMessage :: ~STRSmartCountingMessage () {

}


int STRSmartCountingMessage :: Configure (const RGString& inputString) {
	
	// extracts string data

	mValid = 0;

	RGString localInputString (inputString);
	RGXMLTagSearch countingDataInfoSearch ("CountingDataInfo", localInputString);

	RGString subString;
	RGString numericString;
	RGString comparisonString;
	RGXMLTagSearch thresholdSearch ("Threshold", subString);
	RGXMLTagSearch comparisonSearch ("Comparison", subString);


	size_t overallStartIndex = 0;
	size_t overallEndIndex;

	size_t localStartIndex = 0;
	size_t localEndIndex;

	if (ConfigureMsgNumToSampleType (localInputString, overallStartIndex, overallEndIndex) < 0)
		return -1;

	overallStartIndex = overallEndIndex;

	if (countingDataInfoSearch.FindNextTag (overallStartIndex, overallEndIndex, subString)) {

		overallStartIndex = overallEndIndex;
		thresholdSearch.ResetSearch ();
		comparisonSearch.ResetSearch ();
		localStartIndex = 0;

		if (!thresholdSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		mThreshold = numericString.ConvertToInteger ();
		localStartIndex = localEndIndex;

		if (!comparisonSearch.FindNextTag (localStartIndex, localEndIndex, comparisonString))
			mTestForGreater = true;

		else {

			comparisonString.ToLower ();

			if (comparisonString == "less")
				mTestForGreater = false;

			else
				mTestForGreater = true;
		}
	}

	else
		return -1;

	if (ConfigureReportCriterionToEnd (localInputString, overallStartIndex) < 0)
		return -1;

	mValid = 1;
	return 0;
}


int STRSmartCountingMessage :: GetThreshold () const {

	return mThreshold;
}


void STRSmartCountingMessage :: SetThreshold (int threshold) {

	mThreshold = threshold;
}


int STRSmartCountingMessage :: ConfigureV4 (const RGString& inputString, size_t startIndex) {

	// extracts string data from Version 4.x SmartMessage books

	mValid = 0;

	RGString localInputString (inputString);
	RGXMLTagSearch countingDataInfoSearch ("CountingDataInfo", localInputString);

	RGString subString;
	RGString numericString;
	RGString comparisonString;
	RGXMLTagSearch thresholdSearch ("Threshold", subString);
	RGXMLTagSearch comparisonSearch ("Comparison", subString);


	size_t overallStartIndex = startIndex;
	size_t overallEndIndex = startIndex;

	size_t localStartIndex = 0;
	size_t localEndIndex;

	if (ConfigureDescriptionToSampleTypeV4 (localInputString, overallStartIndex, overallEndIndex) < 0)
		return -1;

	overallStartIndex = overallEndIndex;

	if (countingDataInfoSearch.FindNextTag (overallStartIndex, overallEndIndex, subString)) {

		overallStartIndex = overallEndIndex;
		thresholdSearch.ResetSearch ();
		comparisonSearch.ResetSearch ();
		localStartIndex = 0;

		if (!thresholdSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		mThreshold = numericString.ConvertToInteger ();
		localStartIndex = localEndIndex;

		if (!comparisonSearch.FindNextTag (localStartIndex, localEndIndex, comparisonString))
			mTestForGreater = true;

		else {

			comparisonString.ToLower ();

			if (comparisonString == "less")
				mTestForGreater = false;

			else
				mTestForGreater = true;
		}
	}

	else
		return -1;

	if (ConfigureReportCriterionToEndV4 (localInputString, overallStartIndex) < 0)
		return -1;

	mValid = 1;
	return 0;
}



int STRSmartCountingMessage :: NumberOfIntegerValueCellsNeeded () const {

	return 1;
}


int STRSmartCountingMessage :: GetIntegerValue (SmartMessagingComm& comm, int numHigherObjects) const {

	SmartMessagingObject* smo;
	int i;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];

		if (smo->GetObjectScope () == mScope)
			return smo->GetIntegerValue (mWhichElementWithinValueArray);
	}

	return 0;
}


bool STRSmartCountingMessage :: Evaluate (bool* const msgMatrix, RGDList& subObjects) const {

	return false;
}


bool STRSmartCountingMessage :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	SmartMessagingObject* smo = comm.SMOStack [numHigherObjects - 1];
	int value = smo->GetIntegerValue (mWhichElementWithinValueArray);
	int scope = smo->GetObjectScope ();
	bool ans = false;
	RGString valueString;

	if (mThreshold < 0.0) {

		smo->SetMessageValue (scope, mWhichElementWithinDataArray, false, false);
		return false;
	}
	
	if (mTestForGreater) {

		if (value > mThreshold)
			ans = true;

		else
			ans = false;
	}

	else if (value < mThreshold)
		ans = true;

	else
		ans = false;

	smo->SetMessageValue (scope, mWhichElementWithinDataArray, ans, false);

	if (ans) {

		smo->AppendDataForSmartMessage (scope, mWhichElementWithinDataArray, value, valueString);
		smo->AppendDataForSmartMessage (scope, mWhichElementWithinDataArray, mThreshold, valueString);
	}

	if (mDebugOn && ans) {

		OutputDebugInfo (comm, numHigherObjects, mStage);
	}

	return ans;
}

	

void STRSmartCountingMessage :: SetMessageValue (bool* const msgMatrix, int* const valueMatrix, int index, bool value) {

	valueMatrix [mWhichElementWithinValueArray]++;
}


size_t STRSmartCountingMessage :: StoreSize () const {

	size_t size = STRBaseSmartMessage::StoreSize () + 2 * sizeof (int);
	return size;
}


void STRSmartCountingMessage :: RestoreAll (RGFile& f) {

	STRBaseSmartMessage::RestoreAll (f);
	f.Read (mThreshold);
	int value;
	f.Read (value);

	if (value == 0)
		mTestForGreater = false;

	else
		mTestForGreater = true;
}


void STRSmartCountingMessage :: RestoreAll (RGVInStream& f) {

	STRBaseSmartMessage::RestoreAll (f);
	f >> mThreshold;
	int value;
	f >> value;

	if (value == 0)
		mTestForGreater = false;

	else
		mTestForGreater = true;
}



void STRSmartCountingMessage :: SaveAll (RGFile& f) const {

	STRBaseSmartMessage::SaveAll (f);
	f.Write (mThreshold);
	f.Write ((int) mTestForGreater);
}


void STRSmartCountingMessage :: SaveAll (RGVOutStream& f) const {

	STRBaseSmartMessage::SaveAll (f);
	f << mThreshold;
	f << (int) mTestForGreater;
}



STRSmartPercentMessage :: STRSmartPercentMessage () : STRBaseSmartMessage (), mThreshold (0), mTestForGreater (true),
mNumeratorIndex (0), mDenominatorIndex (0), mNumeratorScope (0), mDenominatorScope (0) {

	mAcceptData = 3;
}


STRSmartPercentMessage :: STRSmartPercentMessage (int integerData, const RGString& name) : STRBaseSmartMessage (integerData, name),
mThreshold (0), mTestForGreater (true), mNumeratorIndex (0), mDenominatorIndex (0), mNumeratorScope (0), mDenominatorScope (0) {

	mAcceptData = 3;
}


STRSmartPercentMessage :: STRSmartPercentMessage (const STRSmartPercentMessage& sm) : STRBaseSmartMessage (sm), mThreshold (sm.mThreshold),
mTestForGreater (sm.mTestForGreater), mNumeratorName (sm.mNumeratorName), mDenominatorName (sm.mDenominatorName), mNumeratorIndex (sm.mNumeratorIndex),
mDenominatorIndex (sm.mDenominatorIndex), mNumeratorScope (sm.mNumeratorScope), mDenominatorScope (sm.mDenominatorScope) {

	mAcceptData = sm.mAcceptData;
}


STRSmartPercentMessage :: ~STRSmartPercentMessage () {

}


void STRSmartPercentMessage :: ImportTestCriterion (const RGString& expression) {

	size_t position;
	expression.FindSubstring ("/", position);
	mNumeratorName = expression.ExtractSubstring (0, position - 1);
	size_t L = expression.Length () - 1;
	mDenominatorName = expression.ExtractSubstring (position + 1, L);
	mNumeratorName.FindAndReplaceAllSubstrings (" ", "");
	mDenominatorName.FindAndReplaceAllSubstrings (" ", "");
}


int STRSmartPercentMessage :: Configure (const RGString& inputString) {
	
	// extracts string data

	mValid = 0;

	RGString localInputString (inputString);
	RGXMLTagSearch percentDataInfoSearch ("PercentDataInfo", localInputString);

	RGString subString;
	RGString numericString;
	RGString comparisonString;
	RGXMLTagSearch thresholdSearch ("Threshold", subString);
	RGXMLTagSearch comparisonSearch ("Comparison", subString);
	RGXMLTagSearch numeratorSearch ("NumeratorName", subString);
	RGXMLTagSearch denominatorSearch ("DenominatorName", subString);

	size_t overallStartIndex = 0;
	size_t overallEndIndex;

	size_t localStartIndex = 0;
	size_t localEndIndex;

	if (ConfigureMsgNumToSampleType (localInputString, overallStartIndex, overallEndIndex) < 0)
		return -1;

	overallStartIndex = overallEndIndex;

	if (percentDataInfoSearch.FindNextTag (overallStartIndex, overallEndIndex, subString)) {

		overallStartIndex = overallEndIndex;
		thresholdSearch.ResetSearch ();
		comparisonSearch.ResetSearch ();
		numeratorSearch.ResetSearch ();
		denominatorSearch.ResetSearch ();
		localStartIndex = 0;

		if (!thresholdSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		mThreshold = numericString.ConvertToInteger ();
		localStartIndex = localEndIndex;

		if (!comparisonSearch.FindNextTag (localStartIndex, localEndIndex, comparisonString))
			mTestForGreater = true;

		else {

			localStartIndex = localEndIndex;
			comparisonString.ToLower ();

			if (comparisonString == "less")
				mTestForGreater = false;

			else
				mTestForGreater = true;
		}

		if (!numeratorSearch.FindNextTag (localStartIndex, localEndIndex, mNumeratorName))
			return -1;

		localStartIndex = localEndIndex;

		if (!denominatorSearch.FindNextTag (localStartIndex, localEndIndex, mDenominatorName))
			return -1;

		localStartIndex = localEndIndex;
	}

	else
		return -1;

	if (ConfigureReportCriterionToEnd (localInputString, overallStartIndex) < 0)
		return -1;

	mValid = 1;
	return 0;
}


int STRSmartPercentMessage :: Initialize () {

	int status = STRBaseSmartMessage::Initialize ();
	int msgIndex;
	SmartMessage* sm;
	msgIndex = FindMessageIndex (mNumeratorName, mNumeratorScope);
	sm = SmartMessage::GetSmartMessageForScopeAndElement (mNumeratorScope, msgIndex);

	if (sm == NULL) {

		cout << "Could not find message index for numerator named " << mNumeratorName.GetData () << endl;
		status = -1;
		mNumeratorIndex = 0;
	}

	else
		mNumeratorIndex = sm->GetWhichElementInValueArray ();

	msgIndex = FindMessageIndex (mDenominatorName, mDenominatorScope);
	sm = SmartMessage::GetSmartMessageForScopeAndElement (mDenominatorScope, msgIndex);

	if (sm == NULL) {

		cout << "Could not find message index for denominator named " << mDenominatorName.GetData () << endl;
		status = -1;
		mDenominatorIndex = 0;
	}

	else
		mDenominatorIndex = sm->GetWhichElementInValueArray ();

	return status;
}


int STRSmartPercentMessage :: GetThreshold () const {

	return mThreshold;
}


void STRSmartPercentMessage :: SetThreshold (int threshold) {

	mThreshold = threshold;
}


int STRSmartPercentMessage :: ConfigureV4 (const RGString& inputString, size_t startIndex) {

	// extracts string data from Version 4.x SmartMessage book declarations

	mValid = 0;

	RGString localInputString (inputString);
	RGXMLTagSearch percentDataInfoSearch ("PercentDataInfo", localInputString);

	RGString subString;
	RGString numericString;
	RGString comparisonString;
	RGXMLTagSearch thresholdSearch ("Threshold", subString);
	RGXMLTagSearch comparisonSearch ("Comparison", subString);
	RGXMLTagSearch numeratorSearch ("NumeratorName", subString);
	RGXMLTagSearch denominatorSearch ("DenominatorName", subString);

	size_t overallStartIndex = startIndex;
	size_t overallEndIndex = startIndex;

	size_t localStartIndex = 0;
	size_t localEndIndex;

	if (ConfigureDescriptionToSampleTypeV4 (localInputString, overallStartIndex, overallEndIndex) < 0)
		return -1;

	overallStartIndex = overallEndIndex;

	if (percentDataInfoSearch.FindNextTag (overallStartIndex, overallEndIndex, subString)) {

		overallStartIndex = overallEndIndex;
		thresholdSearch.ResetSearch ();
		comparisonSearch.ResetSearch ();
		numeratorSearch.ResetSearch ();
		denominatorSearch.ResetSearch ();
		localStartIndex = 0;

		if (!thresholdSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		mThreshold = numericString.ConvertToInteger ();
		localStartIndex = localEndIndex;

		if (!comparisonSearch.FindNextTag (localStartIndex, localEndIndex, comparisonString))
			mTestForGreater = true;

		else {

			localStartIndex = localEndIndex;
			comparisonString.ToLower ();

			if (comparisonString == "less")
				mTestForGreater = false;

			else
				mTestForGreater = true;
		}

		//if (numeratorSearch.FindNextTag (localStartIndex, localEndIndex, mNumeratorName))
		//	localStartIndex = localEndIndex;

		//if (denominatorSearch.FindNextTag (localStartIndex, localEndIndex, mDenominatorName))
		//	localStartIndex = localEndIndex;
	}

	else
		return -1;

	if (ConfigureReportCriterionToEndV4 (localInputString, overallStartIndex) < 0)	// change to ConfigureReportCriterionToEndV4
		return -1;

	mValid = 1;
	return 0;
}


int STRSmartPercentMessage :: NumberOfIntegerValueCellsNeeded () const {

	return 1;
}


int STRSmartPercentMessage :: GetIntegerValue (SmartMessagingComm& comm, int numHigherObjects) const {

	SmartMessagingObject* smo;
	int i;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];

		if (smo->GetObjectScope () == mScope)
			return smo->GetIntegerValue (mWhichElementWithinValueArray);
	}

	return 0;
}


bool STRSmartPercentMessage :: Evaluate (bool* const msgMatrix, RGDList& subObjects) const {

	return false;
}


bool STRSmartPercentMessage :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	SmartMessagingObject* smo; 
	int i;
	int numeratorValue = 0;
	int denominatorValue = 0;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];
		
		if (smo->GetObjectScope () == mNumeratorScope) {

			numeratorValue = smo->GetIntegerValue (mNumeratorIndex);
			break;
		}
	}

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];
		
		if (smo->GetObjectScope () == mDenominatorScope) {

			denominatorValue = smo->GetIntegerValue (mDenominatorIndex);
			break;
		}
	}

	double value;

	if ((numeratorValue == 0) || (denominatorValue == 0))
		value = 0.0;

	else
		value = 100.0 * ((double)numeratorValue / (double)denominatorValue);

	smo = NULL;

	for (i=numHigherObjects-1; i>=0; i--) {

		smo = comm.SMOStack [i];
		
		if (smo->GetObjectScope () == mScope)
			break;
	}

	if (smo == NULL)
		return false;

	int scope = smo->GetObjectScope ();
	bool ans = false;
	RGString valueString;

	if (mThreshold < 0.0) {

		smo->SetMessageValue (scope, mWhichElementWithinDataArray, false, false);
		return false;
	}
	
	if (mTestForGreater) {

		if (value > (double)mThreshold)
			ans = true;

		else
			ans = false;
	}

	else if (value < (double)mThreshold)
		ans = true;

	else
		ans = false;

	smo->SetMessageValue (scope, mWhichElementWithinDataArray, ans, false);
	int intValue = (int) floor (value);

	if (ans) {

		smo->AppendDataForSmartMessage (scope, mWhichElementWithinDataArray, intValue, valueString);
		smo->AppendDataForSmartMessage (scope, mWhichElementWithinDataArray, mThreshold, valueString);
	}

	if (mDebugOn && ans) {

		OutputDebugInfo (comm, numHigherObjects, mStage);
	}

	return ans;
}


void STRSmartPercentMessage :: SetMessageValue (bool* const msgMatrix, int* const valueMatrix, int index, bool value) {

}


size_t STRSmartPercentMessage :: StoreSize () const {

	size_t size = STRBaseSmartMessage::StoreSize () + 6 * sizeof (int);
	size += mNumeratorName.StoreSize () + mDenominatorName.StoreSize ();
	return size;
}

void STRSmartPercentMessage :: RestoreAll (RGFile& f) {

	STRBaseSmartMessage::RestoreAll (f);
	f.Read (mThreshold);
	int value;
	f.Read (value);

	if (value == 0)
		mTestForGreater = false;

	else
		mTestForGreater = true;

	mNumeratorName.RestoreAll (f);
	mDenominatorName.RestoreAll (f);
}


void STRSmartPercentMessage :: RestoreAll (RGVInStream& f) {

	STRBaseSmartMessage::RestoreAll (f);
	f >> mThreshold;
	int value;
	f >> value;

	if (value == 0)
		mTestForGreater = false;

	else
		mTestForGreater = true;

	mNumeratorName.RestoreAll (f);
	mDenominatorName.RestoreAll (f);
}


void STRSmartPercentMessage :: SaveAll (RGFile& f) const {

	STRBaseSmartMessage::SaveAll (f);
	f.Write (mThreshold);
	f.Write ((int) mTestForGreater);
	mNumeratorName.SaveAll (f);
	mDenominatorName.SaveAll (f);
}


void STRSmartPercentMessage :: SaveAll (RGVOutStream& f) const {

	STRBaseSmartMessage::SaveAll (f);
	f << mThreshold;
	f << (int) mTestForGreater;
	mNumeratorName.SaveAll (f);
	mDenominatorName.SaveAll (f);
}



STRSmartPresetMessage :: STRSmartPresetMessage () : STRBaseSmartMessage (), mInitialValue (false) {

}


STRSmartPresetMessage :: STRSmartPresetMessage (int integerData, const RGString& name) : STRBaseSmartMessage (integerData, name),
mInitialValue (false) {

}


STRSmartPresetMessage :: STRSmartPresetMessage (const STRSmartPresetMessage& sm) : STRBaseSmartMessage (sm), mInitialValue (sm.mInitialValue) {

}


STRSmartPresetMessage :: ~STRSmartPresetMessage () {

}


int STRSmartPresetMessage :: Configure (const RGString& inputString) {
	
	// extracts string data

	mValid = 0;

	RGString localInputString (inputString);
	RGXMLTagSearch percentDataInfoSearch ("PresetDataInfo", localInputString);

	RGString subString;
	RGString numericString;
	RGString comparisonString;
	RGXMLTagSearch thresholdSearch ("Threshold", subString);

	size_t overallStartIndex = 0;
	size_t overallEndIndex;

	size_t localStartIndex = 0;
	size_t localEndIndex;
	int val;

	if (ConfigureMsgNumToSampleType (localInputString, overallStartIndex, overallEndIndex) < 0)
		return -1;

	overallStartIndex = overallEndIndex;

	if (percentDataInfoSearch.FindNextTag (overallStartIndex, overallEndIndex, subString)) {

		overallStartIndex = overallEndIndex;
		thresholdSearch.ResetSearch ();
		localStartIndex = 0;

		if (!thresholdSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		if (numericString == "false")
			mInitialValue = false;

		else if (numericString == "true")
			mInitialValue = true;

		else {

			val = numericString.ConvertToInteger ();

			if (val == 0)
				mInitialValue = false;

			else
				mInitialValue = true;
		}

		localStartIndex = localEndIndex;
	}

	else
		return -1;

	if (ConfigureReportCriterionToEnd (localInputString, overallStartIndex) < 0)
		return -1;

	mValid = 1;
	return 0;
}


int STRSmartPresetMessage :: Initialize () {

	return 0;
}


int STRSmartPresetMessage :: GetThreshold () const {

	return (int)mInitialValue;
}


void STRSmartPresetMessage :: SetThreshold (int threshold) {

	if (threshold == 0)
		mInitialValue = false;

	else
		mInitialValue = true;
}


bool STRSmartPresetMessage :: GetInitialValue () const {

	return mInitialValue;
}


int STRSmartPresetMessage :: ConfigureV4 (const RGString& inputString, size_t startIndex) {

	// extracts string data from Version 4.x SmartMessage book declarations

	mValid = 0;

	RGString localInputString (inputString);
	RGXMLTagSearch presetDataInfoSearch ("PresetDataInfo", localInputString);

	RGString subString;
	RGString numericString;
	RGString comparisonString;
	RGXMLTagSearch thresholdSearch ("Threshold", subString);

	size_t overallStartIndex = startIndex;
	size_t overallEndIndex = startIndex;

	size_t localStartIndex = 0;
	size_t localEndIndex;
	int val;

	if (ConfigureDescriptionToSampleTypeV4 (localInputString, overallStartIndex, overallEndIndex) < 0)
		return -1;

	overallStartIndex = overallEndIndex;

	if (presetDataInfoSearch.FindNextTag (overallStartIndex, overallEndIndex, subString)) {

		overallStartIndex = overallEndIndex;
		thresholdSearch.ResetSearch ();
		localStartIndex = 0;

		if (!thresholdSearch.FindNextTag (localStartIndex, localEndIndex, numericString))
			return -1;

		if (numericString == "false")
			mInitialValue = false;

		else if (numericString == "true")
			mInitialValue = true;

		else {

			val = numericString.ConvertToInteger ();

			if (val == 0)
				mInitialValue = false;

			else
				mInitialValue = true;
		}

		localStartIndex = localEndIndex;
	}

	else
		return -1;

	if (ConfigureReportCriterionToEndV4 (localInputString, overallStartIndex) < 0)	// change to ConfigureReportCriterionToEndV4
		return -1;

	mValid = 1;
	return 0;
}


int STRSmartPresetMessage :: GetIntegerValue (SmartMessagingComm& comm, int numHigherObjects) const {

	if (mInitialValue)
		return 1;

	return 0;
}


bool STRSmartPresetMessage :: Evaluate (bool* const msgMatrix, RGDList& subObjects) const {

	return mInitialValue;
}


bool STRSmartPresetMessage :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	return mInitialValue;
}


void STRSmartPresetMessage :: SetMessageValue (bool* const msgMatrix, int* const valueMatrix, int index, bool value) {

}


size_t STRSmartPresetMessage :: StoreSize () const {

	size_t size = STRBaseSmartMessage::StoreSize () + sizeof (bool);
	return size;
}

void STRSmartPresetMessage :: RestoreAll (RGFile& f) {

	STRBaseSmartMessage::RestoreAll (f);
	int value;
	f.Read (value);

	if (value == 0)
		mInitialValue = false;

	else
		mInitialValue = true;
}


void STRSmartPresetMessage :: RestoreAll (RGVInStream& f) {

	STRBaseSmartMessage::RestoreAll (f);
	int value;
	f >> value;

	if (value == 0)
		mInitialValue = false;

	else
		mInitialValue = true;
}


void STRSmartPresetMessage :: SaveAll (RGFile& f) const {

	STRBaseSmartMessage::SaveAll (f);
	f.Write ((int) mInitialValue);
}


void STRSmartPresetMessage :: SaveAll (RGVOutStream& f) const {

	STRBaseSmartMessage::SaveAll (f);
	f << (int) mInitialValue;
}



