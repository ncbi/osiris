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
*  FileName: Notice.cpp
*  Author:   Robert Goor
*
*/
//
//  class Notice is an abstract base class.  Subclasses define specific notices and dictate severity level, message level
//  and mode of printing
//

//
// class NoticeWarehouse:  arranges that there is only one instance of itself and it collects instances of RGPersistent
// for reconstruction on input from a stream or file.  Persistent class prototypes are kept
// in a hash table, indexed by id (which is unique to a class) for very fast retrieval with a minimum of searching.
//

#include "Notice.h"
#include "rgdlist.h"
#include "rghashtable.h"
#include "rgidwrapper.h"
#include "rgindexedlabel.h"
#include "rgtokenizer.h"
#include "OsirisPosix.h"
#include <iostream>

using namespace std;

int Notice::CurrentLinkNumber = 0;
int Notice::sSubject = 1;
int Notice::MessageTrigger = 15;
int Notice::SeverityTrigger = 15;
int Notice::CurrentMsgNumber = 0;
int NoticeWarehouse::ReferenceCount = 0;
NoticeWarehouse* NoticeWarehouse::OneAndOnlySelf = NULL;
Boolean NoticeWarehouse::DebugFlag = FALSE;

ABSTRACT_DEFINITION (Notice)


class NoticeWarehouseKill
{
public:
  NoticeWarehouseKill() {;}
  ~NoticeWarehouseKill()
  {
    NoticeWarehouse::Kill();
  }
};

static NoticeWarehouseKill g_kill;


Notice :: Notice () : RGPersistent (), mEnded (FALSE), mMessageNumber (0) {

}


Notice :: Notice (const Notice& note) : RGPersistent ((const RGPersistent&) note), mEnded (FALSE), mMessageNumber (note.mMessageNumber) {

	mDataList = note.mDataList;
}


Notice :: ~Notice () {

	mDataList.ClearAndDelete ();
}


void Notice :: Report (RGTextOutput& text, const RGString& indent, const RGString& separator) {

	text << CLevel (GetMessageLevel ()) << indent << AssembleString (separator) << "\n" << PLevel ();
}


void Notice :: ReportDataOnly (RGTextOutput& text, const RGString& indent, const RGString& separator) {

	text << CLevel (GetMessageLevel ()) << indent << AssembleStringOfDataOnly (separator) << "\n" << PLevel ();
}


void Notice :: AddDataItem (int i) {

	if (!mEnded) {

		RGString* newString = new RGString;
		newString->Convert (i, 10);
		mDataList.Append (newString);
	}
}


void Notice :: AddDataItem (double d) {

	if (!mEnded) {

		char buffer [30];
		OsirisGcvt (d, 10, buffer);
		RGString* newString = new RGString (buffer);
		mDataList.Append (newString);
	}
}


void Notice :: AddDataItem (const RGString& s) {

	if (!mEnded) {

		RGString* newString = new RGString (s);
		mDataList.Append (newString);
	}
}


Notice& Notice :: operator<<(const RGString& s) {

	AddDataItem (s);
	return *this;
}


Notice& Notice :: operator<<(const char* s) {

	AddDataItem (s);
	return *this;
}


Notice& Notice :: operator<<(int i) {

	AddDataItem (i);
	return *this;
}


Notice& Notice :: operator<<(double d) {

	AddDataItem (d);
	return *this;
}


Notice& Notice :: operator<<(const DataEnd& d) {

	mEnded = TRUE;
	return *this;
}


RGString Notice :: GetDataItem (int i) {

	RGString returnValue;
	RGString* nextString;

	if ((i < 0) || (i >= mDataList.Entries ()))
		return returnValue;

	if (i == 0)
		returnValue = *(RGString*)mDataList.First ();
	
	else if (i == mDataList.Entries () - 1)
		returnValue = *(RGString*)mDataList.Last ();

	else {

		RGDListIterator it (mDataList);

		for (int j=0; j<=i; j++)
			nextString = (RGString*) it ();

		returnValue = *nextString;
	}

	return returnValue;
}


int Notice :: GetNumberOfDataItems () const {

	return (int)mDataList.Entries ();
}


RGString Notice :: AssembleString (const RGString& separator) {

	RGString TotalString;
	int N = GetNumberOfMessageParts ();
	RGString item;
	
	if (N <= 1) {

		TotalString << GetMessagePart (0) << separator;
		TotalString << AssembleStringOfDataOnly (separator);
		return TotalString;
	}
  int i;
	for (i=0; i<N; i++) {

		if (i > 0)
			TotalString << separator;
		
		TotalString << GetMessagePart (i);
		item = GetDataItem (i);

		if (item.Length () > 0)
			TotalString << separator << GetDataItem (i); 
	}

	int M = GetNumberOfDataItems ();

	for (i=N; i<M; i++)
		TotalString << separator << GetDataItem (i);

	return TotalString;
}


RGString Notice :: AssembleStringOfDataOnly (const RGString& separator) {

	RGString data;
	RGString TotalString;
	int N = GetNumberOfDataItems ();

	for (int i=0; i<N; i++) {

		data = GetDataItem (i);

		if (i > 0)
			TotalString << separator;

		TotalString << data;
	}

	return TotalString;
}


Boolean Notice :: IsEqualTo (const RGPersistent* note) const {

	const Notice* oNote = (Notice*) note;
	return (GetID () == oNote->GetID ());
}


unsigned Notice :: HashNumber (unsigned long Base) const {

	return (GetID ()%Base);
}



NoticeWarehouse :: NoticeWarehouse () {

	if (NoticeWarehouse::ReferenceCount == 0) {

		ClassTypes = new RGHashTable (50);
		TypeIterator = new RGHashTableIterator (*ClassTypes);
	}

	NoticeWarehouse::ReferenceCount++;
}



NoticeWarehouse :: ~NoticeWarehouse () {

	NoticeWarehouse::ReferenceCount--;

	if (NoticeWarehouse::ReferenceCount <= 0) {
			
		ClassTypes->ClearAndDelete ();
		delete ClassTypes;
		delete TypeIterator;
	}
}


void* NoticeWarehouse :: operator new (size_t nbytes) {

	void* p;
	
	if (NoticeWarehouse::ReferenceCount > 0) {
		
		return NoticeWarehouse::OneAndOnlySelf;
	}
	
	p = malloc (nbytes);
	NoticeWarehouse::OneAndOnlySelf = (NoticeWarehouse*) p;
	return p;
}



void NoticeWarehouse :: operator delete (void* p) {

	if (NoticeWarehouse::ReferenceCount <= 0)
		free (p);
}



int NoticeWarehouse :: AddType (Notice* NewType) {

	RGIDWrapper* type = new RGIDWrapper (NewType);
	RGPersistent* test = ClassTypes->Append (type);
	int val;

	if (NoticeWarehouse::DebugFlag)
		cout << "Adding class to notice list, ID = " << type->GetSearchID () << endl;

	if (test != NULL) {

		val = ClassTypes->Entries ();
	}

	else {
		
		val = 0;
	}

	return val;
}



Notice* NoticeWarehouse :: FindType (int Target) {

	RGIDWrapper* Found;
	RGIDWrapper target;
	target.SetSearchID (Target);

	Found = (RGIDWrapper*)ClassTypes->Find (&target);

	if (NoticeWarehouse::DebugFlag) {

		cout << "Found class object of type:  " << (Found->GetPointer ())->GetID ();
		cout << ".  Looking for type:  " << Target << endl;
	}

	Notice* ThisIsIt = (Notice*)Found->GetPointer ();
	return (Notice*)ThisIsIt->GetNew ();
}


int NoticeWarehouse :: InitializeAll (NoticeBook& nBook) {

	RGIDWrapper* nextWrapper;
	Notice* nextNotice;
	TypeIterator->Reset ();
	int status = 0;

	while (nextWrapper = (RGIDWrapper*) (*TypeIterator)()) {

		nextNotice = (Notice*) nextWrapper->GetPointer ();

		if (nextNotice->Initialize (nBook) < 0)
			status = -1;
	}

	return status;
}



void NoticeWarehouse :: SetDebugOn () {

	DebugFlag = TRUE;
}


void NoticeWarehouse :: SetDebugOff () {

	DebugFlag = FALSE;
}



NoticeBook :: NoticeBook (RGFile& inputFile) : mNoticeTable (61), mValid (TRUE) {

	RGString noticeString;
	noticeString.ReadTextFile (inputFile);
	mValid = Initialize (noticeString);
}


NoticeBook :: NoticeBook (const RGString& fileName) : mNoticeTable (61), mValid (TRUE) {

	RGFile* file = new RGFile (fileName, "rt");

	if (!file->isValid ()) {

		cout << "Could not read notice book input file..." << endl;
		mValid = FALSE;
	}

	else {

		RGString noticeString;
		noticeString.ReadTextFile (*file);
		mValid = Initialize (noticeString);
	}

	delete file;
}


NoticeBook :: ~NoticeBook () {

	mNoticeTable.ClearAndDelete ();
}


RGString NoticeBook :: GetNoticeStringForIndex (int index) {

	RGIndexedLabel target (index);
	RGIndexedLabel::SetComparisonToIndex ();
	RGIndexedLabel* found = (RGIndexedLabel*) mNoticeTable.Find (&target);
	RGString returnValue;

	if (found != NULL)
		returnValue = found->GetText ();

	return returnValue;
}


Boolean NoticeBook :: Initialize (const RGString& bookString) {

	RGIndexedLabel* newLabel;
	RGString BookString (bookString);
	RGBracketStringSearch MessageSearch ("<Message>", "</Message>", BookString);
	size_t startPos = 0;
	size_t endPos;
	size_t indexedEnd;
	RGString FoundString;
	RGBracketStringSearch IndexSearch ("<Index>", "</Index>", FoundString);
	RGString IndexString;
	unsigned long index;
	Boolean status = TRUE;

	while (MessageSearch.FindNextBracketedString (startPos, endPos, FoundString)) {

		IndexSearch.ResetSearch ();

		if (!IndexSearch.FindNextBracketedString (0, indexedEnd, IndexString))
			status = FALSE;

		else {

			index = IndexString.ConvertToUnsignedLong ();
			newLabel = new RGIndexedLabel (index, "", FoundString);
			RGIndexedLabel::SetComparisonToIndex ();
			mNoticeTable.Insert (newLabel);
		}
		
		startPos = endPos;
	}

	return status;
}


