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
*  FileName: Notice.h
*  Author:   Robert Goor
*
*/
//
//  class Notice is an abstract base class.  Subclasses define specific notices and dictate severity level, message level
//  and mode of printing
//

#ifndef _NOTICE_H_
#define _NOTICE_H_

#include "rgdlist.h"
#include "rgstring.h"
#include "rgpersist.h"
#include "RGTextOutput.h"
#include "rgfile.h"
#include "rghashtable.h"


class NoticeBook;

struct DataEnd {

	DataEnd () {}
};


class Notice : public RGPersistent {

ABSTRACT_DECLARATION (Notice)

public:
	Notice ();
	Notice (const Notice& note);
	virtual ~Notice () = 0;

	void SetMessageNumber (int n) { mMessageNumber = n; }
	int GetMessageNumber () const { return mMessageNumber; }

	virtual int Initialize (NoticeBook& nBook) = 0;
	virtual int GetSubject () const = 0;
	virtual RGString GetLabel () const = 0;
	virtual bool IsTriggerForLowPriority () const = 0;
	virtual bool IsDoNotCall () const = 0;

	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& separator);
	virtual void ReportDataOnly (RGTextOutput& text, const RGString& indent, const RGString& separator);

	virtual void AddDataItem (int i);
	virtual void AddDataItem (double d);
	virtual void AddDataItem (const RGString& s);

	virtual Notice& operator<<(const RGString& s);
	virtual Notice& operator<<(const char* s);
	virtual Notice& operator<<(int i);
	virtual Notice& operator<<(double d);
	virtual Notice& operator<<(const DataEnd& d);

	virtual RGString GetDataItem (int i);
	virtual RGString GetMessagePart (int i) const = 0;
	virtual int GetMessageLevel () const = 0;
	virtual int GetNumberOfMessageParts () const = 0;
	virtual int GetNumberOfDataItems () const;

	virtual int GetSeverityLevel () const = 0;
	virtual void SetIndexAndScope (int index, int scope) const = 0;
	virtual int GetMessageIndex () const = 0;
	virtual int GetScope () const = 0;

	virtual RGString AssembleString (const RGString& separator);
	virtual RGString AssembleStringOfDataOnly (const RGString& separator);

	virtual Boolean IsEqualTo (const RGPersistent* note) const;
	virtual unsigned HashNumber (unsigned long Base) const;

	static int LoadType () { return -1; }
	static int GetNextLinkNumber () { CurrentLinkNumber++; return CurrentLinkNumber; }
	static int GetNextMessageNumber () { CurrentMsgNumber++; return CurrentMsgNumber; }

	static int GetSubjectCategory () { return sSubject; }

	static int GetMessageTrigger () { return MessageTrigger; }
	static void SetMessageTrigger (int severity) { MessageTrigger = severity; }

	static int GetSeverityTrigger () { return SeverityTrigger; }
	static void SetSeverityTrigger (int severity) { SeverityTrigger = severity; }


protected:
//  The following static items appear only in the subclasses...
//	static RGDList sMessageList;
//	static int sMessageNumber;
//	static int sSeverityLevel;
//	static int sMessageLevel;
//  static bool sTriggerLowPriority;
	RGDList mDataList;
	Boolean mEnded;
	int mMessageNumber;

	static int CurrentLinkNumber;
	static int sSubject;
	static int MessageTrigger;
	static int SeverityTrigger;
	static int CurrentMsgNumber;
};



class NoticeWarehouse {

public:
	NoticeWarehouse ();
	~NoticeWarehouse ();

	void* operator new (size_t);
	void operator delete (void*);

	int AddType (Notice* NewType);
	Notice* FindType (int Target);
	
	static int AddClassType (Notice* NewType) { 
		if (OneAndOnlySelf != NULL) 
			return OneAndOnlySelf->AddType (NewType);
		return 0;
	}

	static Notice* TestForID (int Target) {

		if (OneAndOnlySelf != NULL)
			return OneAndOnlySelf->FindType (Target);
		return NULL;
	}

	int InitializeAll (NoticeBook& nBook);

	static void SetDebugOn ();
	static void SetDebugOff ();  // it's off by default
  static void Kill()
  {
    if(OneAndOnlySelf != NULL)
    {
      ReferenceCount = 1; // problem with reference count
      delete OneAndOnlySelf;
      OneAndOnlySelf = NULL;
    }
  }

protected:
	RGHashTable* ClassTypes;
	RGHashTableIterator* TypeIterator;

	static int ReferenceCount;
	static NoticeWarehouse* OneAndOnlySelf;
	static Boolean DebugFlag;
};


class NoticeBook {

public:
	NoticeBook (RGFile& inputFile);
	NoticeBook (const RGString& fileName);
	~NoticeBook ();

	Boolean IsValid () { return mValid; }
	RGString GetNoticeStringForIndex (int index);
	RGString GetLabelStringForIndex (int index);

	Boolean Initialize (const RGString& bookString);

protected:
	RGHashTable mNoticeTable;
	Boolean mValid;
};


#endif  /*  _NOTICE_H_  */

