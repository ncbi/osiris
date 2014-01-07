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
*  FileName: SmartMessage.h
*  Author:   Robert Goor
*
*/
//
// class SmartMessage is the core of the new messaging system.  It can perform computations and retains info to decide
// if, when true, whether a peak can be called, triggers, etc.
//

#ifndef _SMARTMESSAGE_H_
#define _SMARTMESSAGE_H_


#include "rgpersist.h"
#include "rgstring.h"
#include "rgdlist.h"
#include "rghashtable.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "RGTextOutput.h"

//#include "CoreBioComponent.h"
//#include "ChannelData.h"
//#include "Genetics.h"

class SmartNotice;


const int _SMARTMESSAGEDATA_ = 2002;
const int _SMARTMESSAGEREPORTER_ = 2003;

PERSISTENT_PREDECLARATION (SmartMessageData)
PERSISTENT_PREDECLARATION (SmartMessageReporter)


struct SmartMessagingComm;


class SmartMessageKill {

public:
	SmartMessageKill ();
	~SmartMessageKill ();
};


struct SmartMessageThresholdStruct {

	RGString mName;
	int mThreshold;
};



class SmartMessage : public RGPersistent {

ABSTRACT_DECLARATION (SmartMessage)

public:
	SmartMessage ();
	SmartMessage (int integerData, const RGString& name);
	SmartMessage (const SmartMessage& sm);
	virtual ~SmartMessage () = 0;

	void SetStage (int st) { mStage = st; }
	void SetIntegerData (int mn) { mIntegerData = mn; }
	void SetName (const RGString& name) { mName = name; }
	void SetDescription (const RGString& desc) { mDescription = desc; }
	void SetSampleType (int type) { mSampleType = type; }
	void SetMessageText (const RGString& text) { mMessageText = text; }
	void SetAdditionalTextForData (const RGString& dataText) { mAdditionalTextForData = dataText; }
	void SetSeparatorText (const RGString& text) { mSeparatorText = text; }
	void SetScope (int scope) { mScope = scope; }
	void SetWhichElementInMsgArray (int elt) { mWhichElementWithinDataArray = elt; }
	void SetWhichElementInValueArray (int elt) { mWhichElementWithinValueArray = elt; }
	void SetAcceptData (int accept) { mAcceptData = accept; }
	void SetShareWithCluster (int share) { mShareWithCluster = share; }
	void SetAllowPeakEdit (int allow) { mAllowPeakEdit = allow; }

	int GetStage () const { return mStage; }
	int GetScope () const { return mScope; }  // which kind of object can "own" this message?
	int GetIntegerData () const { return mIntegerData; }
	RGString GetName () const { return mName; }
	int GetSampleType () const { return mSampleType; }  // sample / ladder / directory / sample and ladder
	RGString GetMessageText () const { return mMessageText; }
	RGString GetTextForData () const { return mAdditionalTextForData; }
	RGString GetDataSeparator () const { return mSeparatorText; }
	RGString GetClosingTextForData () const { return mClosingText; }
	RGString GetDescription () const { return mDescription; }
	int GetWhichElementInMsgArray () const { return mWhichElementWithinDataArray; }
	int GetWhichElementInValueArray () const { return mWhichElementWithinValueArray; }
	int GetAcceptData () const { return mAcceptData; }
	int GetShareWithCluster () const { return mShareWithCluster; }
	int GetAllowPeakEdit () const { return mAllowPeakEdit; }
	bool DebugIsOn () const { return mDebugOn; }

	int IsValid () const { return mValid; }
	int CompiledSuccessfully () const { return mCompiled; }

	virtual SmartMessage* GetNewMessage (const RGString& inputString) const = 0;
	virtual int NumberOfIntegerValueCellsNeeded () const = 0;
	virtual int GetIntegerValue (SmartMessagingComm& comm, int numHigherObjects) const = 0;
	virtual int GetThreshold () const = 0;
	virtual void SetThreshold (int threshold) = 0;
	virtual bool GetInitialValue () const { return false; }

	virtual void ImportTestCriterion (const RGString& expression) {}
	virtual void ImportTriggerNames (const RGString& expression) {}
	virtual int ImportTriggerCondition (const RGString& expression) { return -1; }

	virtual int Configure (const RGString& inputString) = 0;
	virtual bool CompileAll () = 0;
	virtual int Initialize () = 0;

	virtual bool Evaluate (bool* const msgMatrix, RGDList& subObjects) const = 0;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const = 0;
	virtual int EvaluateReportLevel (const bool* msgMatrix) const = 0;
	virtual bool EvaluateReport (const bool* msgMatrix) const = 0;
	virtual bool EvaluateCall (const bool* msgMatrix) const = 0;
	virtual bool EvaluateRestriction (const bool* msgMatrix) const = 0;
	virtual int EvaluateRestrictionLevel (const bool* msgMatrix) const = 0;

	virtual bool OutputDebugInfo (SmartMessagingComm& comm, int numHigherObjects, int stage) const = 0;
	virtual int EvaluateReportLevel (SmartMessagingComm& comm, int numHigherObjects) const = 0;
	virtual bool EvaluateReport (SmartMessagingComm& comm, int numHigherObjects) const = 0;
	virtual bool EvaluateReportContingent (SmartMessagingComm& comm, int numHigherObjects) const = 0;
	virtual bool EvaluateCall (SmartMessagingComm& comm, int numHigherObjects) const = 0;
	virtual bool EvaluateRestriction (SmartMessagingComm& comm, int numHigherObjects) const = 0;
	virtual int EvaluateRestrictionLevel (SmartMessagingComm& comm, int numHigherObjects) const = 0;

	virtual bool SetAllTriggers (bool* const msgMatrix, bool* const higherMsgMatrix) const = 0;
	virtual bool SetAllTriggers (bool* const msgMatrix, bool* const higherMsgMatrix, RGHashTable* messageDataTable, int stage, int scope, int higherScope, int intBP, const RGString& alleleName) const = 0;
	virtual bool SetAllTriggers (bool** const msgMatrices, RGHashTable** messageDataTables, int stage, int* scopes, int numScopes, int intBP, const RGString& alleleName) const = 0;
	virtual bool SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage) = 0;
	virtual bool SetAllTriggers (SmartMessagingComm& comm, int numHigherObjects, int stage, int intBP, const RGString& alleleName) = 0;
	virtual bool AddDataItem (const RGString& data, RGHashTable* dataArray) const = 0;
	virtual int FindMessageIndex (const RGString& name, int& whichDataArray) const = 0;
	virtual SmartMessage* FindMessageByName (const RGString& name) const = 0;

	virtual bool IsTrue (const bool* msgMatrix) const = 0;
	virtual bool Report (const bool* msgMatrix) const = 0;
	virtual bool IsCritical (const bool* msgMatrix) const = 0;
	virtual bool IsEditable () const = 0;
	virtual bool DontCall (const bool* msgMatrix) const = 0;
	virtual RGString GetData (const RGString* dataArray) = 0;
	virtual RGString GetExportProtocolList () const = 0;
	virtual bool HasExportProtocolInfo () const = 0;
	virtual bool UseDefaultExportDisplayMode () const = 0;
	virtual bool DisplayExportInfo () const = 0;

	virtual int GetReportLevel (bool* msgMatrix) const = 0;
	virtual void SetMessageValue (bool* const msgMatrix, int* const valueMatrix, int index, bool value) = 0;

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	static int LoadAllMessages (const RGString& inputString, const SmartMessage* prototype);
	static void ClearAllMessages ();
	static int GetLastStage () { return LastStage; }
	static int GetMessageIndex (const SmartMessage* msg, int& whichDataArray);
	static int GetStartIndexForListAndStage (int list, int stage);
	static int GetEndIndexForListAndStage (int list, int stage);

	// The following are for the new Version 4.x style Smart MessageBook...
	static int LoadAllMessagesV4 (const RGString& inputString, SmartMessage* prototype);
	virtual SmartMessage* GetNewMessageV4 (const RGString& inputString) const = 0;
	virtual int ConfigureV4 (const RGString& inputString, size_t startIndex) = 0;

	static void SetDebugMode (int mode) { DebugMode = mode; }
	static int GetDebugMode () { return DebugMode; }

	static int GetSeverityTrigger () { return SeverityTrigger; }
	static void SetSeverityTrigger (int severity) { SeverityTrigger = severity; }

	static int CompileAllMessages ();
	static int InitializeAllMessages ();
	static bool EvaluateAllMessages (bool* const msgMatrix, RGDList& subObjects, int stage, int scope);
	static bool EvaluateAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, int scope);
	static bool SetTriggersForAllMessages (bool* const msgMatrix, bool* const higherMsgMatrix, int stage, int scope);
	static bool SetTriggersForAllMessages (bool* const msgMatrix, bool* const higherMsgMatrix, RGHashTable* messageDataTable, int stage, int scope, int higherScope, int intBP, const RGString& alleleName);
	static bool SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, int scope, int intBP, const RGString& alleleName);
	static bool SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage, int scope);
	static bool EvaluateAllReports (const bool* msgMatrix, bool* const reportMatrix, int scope);
	static bool TestAllMessagesForCall (const bool* msgMatrix, int scope);
	static int FindRestrictionLevel (const bool* msgMatrix, int scope);
	static bool EvaluateAllReportLevels (const bool* msgMatrix, int* const reportLevelMatrix, int scope);
	static bool IsInitialized () { return Initialized; }
	static int GetSizeOfArrayForScope (int scope);
	static int GetSizeOfValueArrayForScope (int scope);
	static SmartMessage* GetSmartMessageForScopeAndElement (int scope, int element) { return MessageArrays [scope][element]; }
	static bool CreateDebugFile (const RGString& fullPathDebugName);
	static bool OutputDebugString (const RGString& string);

	static int LoadAndActivateAllMessages (const RGString& messageFullPathFileName, SmartMessage* prototype);  // call this function to set up all SmartMessages
	static int WriteMsgBookFile (RGFile& msgFile);

protected:
	int mStage;
	int mIntegerData;
	RGString mName;
	RGString mDescription;
	int mSampleType;
	RGString mMessageText;
	RGString mAdditionalTextForData;
	RGString mSeparatorText;  //e.g., "; "
	RGString mClosingText;
	int mScope;  // type of object message applies to, and which data array
	int mWhichElementWithinDataArray;
	int mWhichElementWithinValueArray;

	int mValid;
	int mCompiled;
	int mAcceptData; // 0 means no; 1 means integer (bp from signal); 2 means string (allele name from signal); 3 means int on Evaluation only; 4 means string on evaluation only
	int mShareWithCluster;
	int mAllowPeakEdit;
	bool mDebugOn;

	static RGHashTable* OverAllMessageTable;
	static RGDList OverAllMessageList;
	static int NumberOfMessageTables;
	static RGDList** MessageTables;
	static SmartMessage*** MessageArrays;
	static int* MessageArraySize;
	static int LastStage;
	static int** StageStartIndex;
	static int** StageEndIndex;
	static bool Initialized;
	static int* MessageValueArraySize;
	static int DebugMode;
	static RGFile* DebugFile;
	static RGTextOutput* DebugTextOutput;
	static RGString MsgBookText;
	static int SeverityTrigger;

	static int AssembleMessages ();
	static int AssembleMessagesV4 (const RGString& inputString, size_t startIndex, SmartMessage* prototype);
	static int ImportMessageDynamicDataV4 (const RGString& inputString, size_t startIndex, SmartMessage* prototype);
	static SmartMessageKill kill;
};


class SmartMessageData : public RGPersistent {

PERSISTENT_DECLARATION (SmartMessageData)

public:
	SmartMessageData ();
	SmartMessageData (int index);
	SmartMessageData (int index, int scope, const RGString& text);
	SmartMessageData (const SmartMessageData& smd);
	~SmartMessageData ();

	void SetIndex (int index) { mIndex = index; }
	void SetText (const RGString& text) { mText = text; }
	void AppendText (const RGString& text, const SmartNotice& notice);
	void AppendText (const RGString& text, int index, int scope);

	int GetIndex () const { return mIndex; }
	RGString GetText () const { return mText; }
	size_t GetTextLength () const { return mText.Length (); }

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

public:
	int mIndex;
	int mScope;
	RGString mText;
};


class SmartMessageReporter : public RGPersistent {

PERSISTENT_DECLARATION (SmartMessageReporter)

public:
	SmartMessageReporter ();
	SmartMessageReporter (const SmartMessage* prototype);
	SmartMessageReporter (const SmartMessageReporter& smr);
	virtual ~SmartMessageReporter ();

	void SetSmartMessage (const SmartMessage* prototype) { mPrototype = prototype; }
	void SetData (const SmartMessageData* data) { mData = data; }
	void SetMessageCount (int msgCount) { mMessageCount = msgCount; }
	void SetPriorityLevel (int prio);
	void SetRestrictionLevel (int restrict) { mRestriction = restrict; }
	void SetDoNotCall (bool doNotCall) { mIsDoNotCall = doNotCall; }

	void SetCritical (bool critical) { mCritical = critical; }
	void SetEnabled (bool enabled) { mEnabled = enabled; }
	void SetEditable (bool editable) { mEditable = editable; }
	void SetDisplayExportInfo (bool display) { mDisplayExportInfo = display; }
	void SetDisplayOsirisInfo (bool display) { mDisplayOsirisInfo = display; }
	void SetExportProtocolInformation (const RGString& info) { mExportProtocolInformation = info; }
	void ComputeViabilityOfExportInfo ();

	int GetMessageCount () const { return mMessageCount; }
//	int GetMessageNumber () const { return mPrototype->GetMessageNumber (); }
	int GetMessagePriority () const { return mPriorityLevel; }
	int GetMessageRestriction () const { return mRestriction; }
	bool IsDoNotCall () const { return mIsDoNotCall; }
	RGString GetMessageName () const;

	bool IsCritical () const { return mCritical; }
	bool IsEnabled () const { return mEnabled; }
	bool IsEditable () const { return mEditable; }
	bool GetDisplayExportInfo () const { return mDisplayExportInfo; }
	bool GetDisplayOsirisInfo () const { return mDisplayOsirisInfo; }
	bool HasExportProtocolInformation () const { return (mExportProtocolInformation.Length () > 0); }
	bool HasViableExportInfo () const { return mViableExportInfo; }
	RGString GetExportProtocolInformation () const { return mExportProtocolInformation; }

	RGString GetMessage () const { return mPrototype->GetMessageText (); }
	RGString GetMessageData () const;

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

protected:
	const SmartMessage* mPrototype;
	const SmartMessageData* mData;
	int mMessageCount;
	int mPriorityLevel;
	int mRestriction;
	bool mIsDoNotCall;
	bool mCritical;
	bool mEnabled;
	bool mEditable;
	bool mDisplayExportInfo;
	bool mDisplayOsirisInfo;
	bool mViableExportInfo;
	RGString mExportProtocolInformation;
};


#endif   /*  _SMARTMESSAGE_H_   */

