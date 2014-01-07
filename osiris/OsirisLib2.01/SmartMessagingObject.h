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
*  FileName: SmartMessagingObject.h
*  Author:   Robert Goor
*
*/
//
//	abstract class to provide interface for all objects that perform smart messaging.  These include:  DataSignal,
//  Locus, ChannelData, CoreBioComponent, STRLCAnalysis and BaseLocus.
//

#ifndef _SMARTMESSAGINGOBJECT_
#define _SMARTMESSAGINGOBJECT_

#include "rgpersist.h"
#include "rghashtable.h"
#include "rgdlist.h"
#include "SmartMessage.h"
#include "SmartNotice.h"
#include "RGLogBook.h"

const int SmartMessagingCommDepth = 7;

class SmartMessagingObject;


struct SmartMessagingComm {

	SmartMessagingObject* SMOStack [SmartMessagingCommDepth];
};


class SmartMessagingObject : public RGPersistent {

ABSTRACT_DECLARATION (SmartMessagingObject)

public:
	SmartMessagingObject ();
	SmartMessagingObject (const SmartMessagingObject& smo);
	virtual ~SmartMessagingObject () = 0;

	virtual int GetObjectScope () const = 0;

	virtual void SetMessageValue (int scope, int location, bool value);
	virtual void SetMessageValue (const SmartNotice& notice, bool value);
//	virtual void SetMessageValue (const SmartNotice& notice, bool value, bool useVirtualMethod);
	virtual void SetMessageValue (int scope, int location, bool value, bool useVirtualMethod);

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects) = 0;
	virtual RGString GetDebugIDIndent () const;

	bool GetMessageValue (const SmartNotice& notice) const;
	bool GetMessageValue (int location) const { return mMessageArray [location]; }
	int GetIntegerValue (int location) const;
	bool AcceptsDataOnTrigger (int index);
	bool AcceptsDataOnEvaluation (int index);
	int GetThreshold (const SmartNotice& notice) const;

	int FindRestrictionLevel ();
	int GetHighestMessageLevelWithRestrictionSM () const;

	void SetDataForSmartMessage (const SmartNotice& notice, const RGString& text);
	void AppendDataForSmartMessage (const SmartNotice& notice, const RGString& text);
	void AppendDataForSmartMessage (const SmartNotice& notice, int value);
	void AppendDataForSmartMessage (const SmartNotice& notice, double value);
	void AppendDataForSmartMessage (int scope, int index, int value, const RGString& text);

	virtual void ClearSmartNoticeObjects ();

	int NumberOfSmartNoticeObjects () const { return mSmartMessageReporters->Entries (); }
	virtual int AddSmartMessageReporter (SmartMessageReporter* smr);
	virtual int AddAllSmartMessageReporters () = 0;
	virtual int AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects) = 0;
	virtual void InitializeMessageData () = 0;

	void MergeAllSmartMessageReporters ();

	static void InsertExportSpecificationsIntoTable (SmartMessage* sm);
	static void ClearExportSpecifications ();
	static void ReportAllExportSpecifications (RGLogBook& oar);

protected:
	bool* mMessageArray;
	int* mValueArray;
	RGHashTable* mMessageDataTable;
	RGDList* mSmartMessageReporters;
	RGDList* mUnenabledMessageReporters;

	int mStageCompleted;
	int mTriggerStageCompleted;

	int mHighestSeverityLevel;
	int mHighestMessageLevel;

	static RGHashTable ExportSpecifications;

	void InitializeSmartMessages ();
	void InitializeSmartMessages (const SmartMessagingObject& smo);
	void CleanupSmartMessages ();
};


#endif  /*  _SMARTMESSAGINGOBJECT_  */

