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
*  FileName: STRSmartMessage.h
*  Author:   Robert Goor
*
*/
//
// Subclass of SmartMessage...this is the STR version that is based on data in XML Schema OsirisMessageBook3.1.xsd.  Each class object
// configures itself from an xml substring, with logical expressions that allow the message to reconfigure itself based on the totality
// of messages.
//

#ifndef _STRSMARTMESSAGE_H_
#define _STRSMARTMESSAGE_H_

#include "SmartMessage.h"
#include "rgstring.h"
#include "rgdlist.h"
#include "rgfile.h"
#include "rgvstream.h"

#include <list>

class Expression;

const int _STRSMARTMESSAGE_ = 2000;
const int _STRBASESMARTMESSAGE_ = 2004;
const int _STRSMARTCOUNTINGMESSAGE_ = 2005;
const int _STRSMARTPERCENTMESSAGE_ = 2006;
const int _CONDITIONALTRIGGERUNIT_ = 2007;
const int _STRSMARTPRESETMESSAGE_ = 2008;

PERSISTENT_PREDECLARATION (ConditionalTriggerUnit)
PERSISTENT_PREDECLARATION (STRBaseSmartMessage)
PERSISTENT_PREDECLARATION (STRSmartMessage)
PERSISTENT_PREDECLARATION (STRSmartCountingMessage)
PERSISTENT_PREDECLARATION (STRSmartPercentMessage)
PERSISTENT_PREDECLARATION (STRSmartPresetMessage)


struct TriggerInfo {

	int mScope;
	int mIndex;
};


struct DebugInfo {

	int mScope;
	int mIndex;
	RGString* mName;

	DebugInfo ();
	~DebugInfo ();
};


class ConditionalTriggerUnit : public RGPersistent {

	PERSISTENT_DECLARATION (ConditionalTriggerUnit)

public:
	ConditionalTriggerUnit ();
	ConditionalTriggerUnit (const RGString& triggerExpression);
	virtual ~ConditionalTriggerUnit ();

	bool isValid () const { return mValid; }

	int Initialize (int msgScope, SmartMessage* sourceMessage);
	int Compile ();
	virtual bool SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage);
	virtual bool SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage, int intBP, const RGString& alleleName);

protected:
	bool mValid;
	RGString mExpressionString;
	Expression* mTriggerExpression;
	RGDList mTriggerNames;
	list<TriggerInfo*> mTriggers;
};


class STRBaseSmartMessage : public SmartMessage {

	PERSISTENT_DECLARATION (STRBaseSmartMessage)

public:
	STRBaseSmartMessage ();
	STRBaseSmartMessage (int integerData, const RGString& name);
	STRBaseSmartMessage (const STRBaseSmartMessage& sm);
	virtual ~STRBaseSmartMessage ();

	virtual SmartMessage* GetNewMessage (const RGString& inputString) const;
	virtual int NumberOfIntegerValueCellsNeeded () const { return 0; }
	virtual int GetIntegerValue (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual SmartMessage* GetNewMessageV4 (const RGString& inputString) const;
	virtual int ConfigureV4 (const RGString& inputString, size_t startIndex);
	int ConfigureDescriptionToSampleTypeV4 (const RGString& inputString, size_t startIndex, size_t& endIndex);
	int ConfigureReportCriterionToEndV4 (const RGString& inputString, size_t startIndex);

	virtual int Configure (const RGString& inputString);
	virtual bool CompileAll ();
	virtual int Initialize ();
	virtual int GetThreshold () const;
	virtual void SetThreshold (int threshold);
	virtual void ImportTestCriterion (const RGString& expression) {}
	virtual void ImportTriggerNames (const RGString& expression);
	virtual int ImportTriggerCondition (const RGString& expression);

	virtual bool Evaluate (bool* const msgMatrix, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual int EvaluateReportLevel (const bool* msgMatrix) const;
	virtual int EvaluateReportLevel (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual bool EvaluateReport (const bool* msgMatrix) const;
	virtual bool EvaluateReport (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual bool EvaluateReportContingent (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual bool EvaluateCall (const bool* msgMatrix) const;
	virtual bool EvaluateCall (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual bool EvaluateRestriction (const bool* msgMatrix) const;
	virtual bool EvaluateRestriction (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual int EvaluateRestrictionLevel (const bool* msgMatrix) const;
	virtual int EvaluateRestrictionLevel (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual bool OutputDebugInfo (SmartMessagingComm& comm, int numHigherObjects, int stage) const;

	virtual bool SetAllTriggers (bool* const msgMatrix, bool* const higherMsgMatrix) const;
	virtual bool SetAllTriggers (bool* const msgMatrix, bool* const higherMsgMatrix, RGHashTable* messageDataTable, int stage, int scope, int higherScope, int intBP, const RGString& alleleName) const;
	virtual bool SetAllTriggers (bool** const msgMatrices, RGHashTable** messageDataTables, int stage, int* scopes, int numScopes, int intBP, const RGString& alleleName) const;
	virtual bool SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage);
	virtual bool SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage, int intBP, const RGString& alleleName);
	virtual bool AddDataItem (const RGString& data, RGHashTable* dataArray) const;
	virtual int FindMessageIndex (const RGString& name, int& whichDataArray) const;
	virtual SmartMessage* FindMessageByName (const RGString& name) const;

	virtual bool IsTrue (const bool* msgMatrix) const;
	virtual bool Report (const bool* msgMatrix) const;
	virtual bool IsCritical (const bool* msgMatrix) const;
	virtual bool IsEditable () const;
	virtual bool DontCall (const bool* msgMatrix) const;
	virtual RGString GetData (const RGString* dataArray);
	virtual RGString GetExportProtocolList () const;
	virtual bool HasExportProtocolInfo () const;
	virtual bool UseDefaultExportDisplayMode () const;
	virtual bool DisplayExportInfo () const;

	virtual int GetReportLevel (bool* msgMatrix) const;
	virtual void SetMessageValue (bool* const msgMatrix, int* const valueMatrix, int index, bool value);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	int ConfigureMsgNumToSampleType (const RGString& inputString, size_t startIndex, size_t& endIndex);
	int ConfigureReportCriterionToEnd (const RGString& inputString, size_t startIndex);

	static int LoadAndActivateAllMessages (const RGString& messageFullPathFileName);
	static int LoadAndActivateAllThresholds (const RGString& thresholdFullPathFileName);

protected:
	RGString mReportCriterion; // expression to be evaluated, or true or false, to determine if message is to be reported
	int mReportLevel;  // 1 is most critical; the larger the number, the less critical
	int mAlternateReportLevel;
	RGString mAlternateReportCriterion;  // expression to determine if alternate report level is to be used
	RGString mCallCriterion;  // expression to determine if peak is to be a called allele (relavent only for DataSignals)
	RGDList mTriggerNames;
	list<TriggerInfo*> mTriggers;  // list of names of messages to be triggered (true) if this message is true
	list<DebugInfo*> mWatchInfo;  // list of watch names to be reported if debug is on and this message is true
	int mRestrictionLevel;
	RGString mRestrictionCriterion;
	RGDList mConditionalTriggerUnits;

	Expression* mReportExpression;
	Expression* mAlternateReportExpression;
	Expression* mCallExpression;
	Expression* mRestrictionExpression;

	bool mEditable;
	bool mDisplayExportInfo;
	bool mExportReportMirrorsOarReport;
	RGString mExportProtocolList;

	void ClearTriggerList ();
	void ClearDebugList ();
};


class STRSmartMessage : public STRBaseSmartMessage {

	PERSISTENT_DECLARATION (STRSmartMessage)

public:
	STRSmartMessage ();
	STRSmartMessage (int integerData, const RGString& name);
	STRSmartMessage (const STRSmartMessage& sm);
	virtual ~STRSmartMessage ();

	virtual void ImportTestCriterion (const RGString& expression);

	virtual int Configure (const RGString& inputString);  // extracts string data
	virtual bool CompileAll ();  // compiles expressions
	virtual int Initialize ();  // replaces named data in expressions with pointers

	virtual int ConfigureV4 (const RGString& inputString, size_t startIndex);

	virtual bool Evaluate (bool* const msgMatrix, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	RGString mTestCriterion;  // expression to be evaluated; if empty, this is a primitive, hard set by code
	Expression* mTestExpression;
};


class STRSmartCountingMessage : public STRBaseSmartMessage {

	PERSISTENT_DECLARATION (STRSmartCountingMessage)

public:
	STRSmartCountingMessage ();
	STRSmartCountingMessage (int integerData, const RGString& name);
	STRSmartCountingMessage (const STRSmartCountingMessage& sm);
	virtual ~STRSmartCountingMessage ();

	virtual int Configure (const RGString& inputString);  // extracts string data
//	virtual bool CompileAll ();  // compiles expressions...unneeded
//	virtual int Initialize ();  // replaces named data in expressions with pointers...unneeded
	virtual int GetThreshold () const;
	virtual void SetThreshold (int threshold);

	virtual int ConfigureV4 (const RGString& inputString, size_t startIndex);

	virtual int NumberOfIntegerValueCellsNeeded () const;
	virtual int GetIntegerValue (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual bool Evaluate (bool* const msgMatrix, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual void SetMessageValue (bool* const msgMatrix, int* const valueMatrix, int index, bool value);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	int mThreshold;
	bool mTestForGreater;
};



class STRSmartPercentMessage : public STRBaseSmartMessage {

	PERSISTENT_DECLARATION (STRSmartPercentMessage)

public:
	STRSmartPercentMessage ();
	STRSmartPercentMessage (int integerData, const RGString& name);
	STRSmartPercentMessage (const STRSmartPercentMessage& sm);
	virtual ~STRSmartPercentMessage ();

	virtual void ImportTestCriterion (const RGString& expression);

	virtual int Configure (const RGString& inputString);  // extracts string data
//	virtual bool CompileAll ();  // compiles expressions...unneeded
	virtual int Initialize ();
	virtual int GetThreshold () const;
	virtual void SetThreshold (int threshold);
	virtual int ConfigureV4 (const RGString& inputString, size_t startIndex);

	virtual int NumberOfIntegerValueCellsNeeded () const;
	virtual int GetIntegerValue (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual bool Evaluate (bool* const msgMatrix, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual void SetMessageValue (bool* const msgMatrix, int* const valueMatrix, int index, bool value);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	int mThreshold;
	bool mTestForGreater;
	RGString mNumeratorName;
	RGString mDenominatorName;
	int mNumeratorIndex;
	int mDenominatorIndex;
	int mNumeratorScope;
	int mDenominatorScope;
};



class STRSmartPresetMessage : public STRBaseSmartMessage {

	PERSISTENT_DECLARATION (STRSmartPresetMessage)

public:
	STRSmartPresetMessage ();
	STRSmartPresetMessage (int integerData, const RGString& name);
	STRSmartPresetMessage (const STRSmartPresetMessage& sm);
	virtual ~STRSmartPresetMessage ();

	virtual int Configure (const RGString& inputString);  // extracts string data
//	virtual bool CompileAll ();  // compiles expressions...unneeded
	virtual int Initialize ();
	virtual int GetThreshold () const;
	virtual void SetThreshold (int threshold);
	virtual bool GetInitialValue () const;

	virtual int ConfigureV4 (const RGString& inputString, size_t startIndex);

	virtual int GetIntegerValue (SmartMessagingComm& comm, int numHigherObjects) const;

	virtual bool Evaluate (bool* const msgMatrix, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual void SetMessageValue (bool* const msgMatrix, int* const valueMatrix, int index, bool value);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	bool mInitialValue;
};



#endif  /*  _STRSMARTMESSAGE_H_  */

