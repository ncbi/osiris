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
*  FileName: SmartNotice.cpp
*  Author:   Robert Goor
*
*/
//
//  class SmartNotice is an abstract base class.  Subclasses allow access to various (but not necessarily all) SmartMessage array values
//

//
// class SmartNoticeWarehouse:  arranges that there is only one instance of itself and it collects instances of RGPersistent
// for reconstruction on input from a stream or file.  Persistent class prototypes are kept
// in a hash table, indexed by id (which is unique to a class) for very fast retrieval with a minimum of searching.
//

#include "SmartNotice.h"
#include "rghashtable.h"
#include "rgidwrapper.h"
#include "rgindexedlabel.h"
#include "rgtokenizer.h"
#include <iostream>

using namespace std;

int SmartNotice::sSubject = 1;
RGString SmartNotice::sName;
int SmartNoticeWarehouse::ReferenceCount = 0;
SmartNoticeWarehouse* SmartNoticeWarehouse::OneAndOnlySelf = NULL;
bool SmartNoticeWarehouse::DebugFlag = false;

ABSTRACT_DEFINITION (SmartNotice)


class SmartNoticeWarehouseKill {

public:
  SmartNoticeWarehouseKill() {;}

  ~SmartNoticeWarehouseKill() {
    SmartNoticeWarehouse::Kill();
  }
};

static SmartNoticeWarehouseKill g_kill;


SmartNotice :: SmartNotice () : RGPersistent () {

}


SmartNotice :: ~SmartNotice () {

}


Boolean SmartNotice :: IsEqualTo (const RGPersistent* note) const {

	const SmartNotice* oNote = (SmartNotice*) note;
	return (GetName ().IsEqualTo (&(oNote->GetName ())));
}


unsigned SmartNotice :: HashNumber (unsigned long Base) const {

	return GetName ().HashNumber (Base);
}



SmartNoticeWarehouse :: SmartNoticeWarehouse () {

	if (SmartNoticeWarehouse::ReferenceCount == 0) {

		ClassTypes = new RGHashTable (53);
		TypeIterator = new RGHashTableIterator (*ClassTypes);
	}

	SmartNoticeWarehouse::ReferenceCount++;
}



SmartNoticeWarehouse :: ~SmartNoticeWarehouse () {

	SmartNoticeWarehouse::ReferenceCount--;

	if (SmartNoticeWarehouse::ReferenceCount <= 0) {
			
		ClassTypes->ClearAndDelete ();
		delete ClassTypes;
		delete TypeIterator;
	}
}


void* SmartNoticeWarehouse :: operator new (size_t nbytes) {

	void* p;
	
	if (SmartNoticeWarehouse::ReferenceCount > 0) {
		
		return SmartNoticeWarehouse::OneAndOnlySelf;
	}
	
	p = malloc (nbytes);
	SmartNoticeWarehouse::OneAndOnlySelf = (SmartNoticeWarehouse*) p;
	return p;
}



void SmartNoticeWarehouse :: operator delete (void* p) {

	if (SmartNoticeWarehouse::ReferenceCount <= 0)
		free (p);
}



int SmartNoticeWarehouse :: AddType (SmartNotice* NewType) {

	RGPersistent* test = ClassTypes->Append (NewType);
	int val;

	if (SmartNoticeWarehouse::DebugFlag) {

		RGString name (NewType->GetName ());
		cout << "Adding class to notice list, Name = " << name.GetData () << endl;
	}

	if (test != NULL) {

		val = ClassTypes->Entries ();
	}

	else {
		
		val = 0;
	}

	return val;
}



//SmartNotice* SmartNoticeWarehouse :: FindType (int Target) {
//
//	SmartNotice target;
//	target.SetMessageNumber (Target);
//	SmartNotice* Found;
//
//	Found = (SmartNotice*)ClassTypes->Find (&target);
//
//	if (Found == NULL)
//		return NULL;
//
//	if (SmartNoticeWarehouse::DebugFlag) {
//
//		cout << "Found class object of type:  " << Found->GetMessageNumber ();
//		cout << ".  Looking for type:  " << Target << endl;
//	}
//
//	return (SmartNotice*)Found->GetNew ();
//}


SmartNotice* SmartNoticeWarehouse :: FindType (const RGString& name) {

	SmartNotice target;
	target.SetName (name);
	SmartNotice* Found;

	Found = (SmartNotice*)ClassTypes->Find (&target);	// Assumes name-based test for equality!!!

	if (Found == NULL)
		return NULL;

	if (SmartNoticeWarehouse::DebugFlag) {

		cout << "Found class object named:  " << Found->GetName ();
		cout << ".  Looking for name:  " << name << endl;
	}

	return (SmartNotice*)Found->GetNew ();
}


int SmartNoticeWarehouse :: AddClassType (SmartNotice* NewType) { 

	if (OneAndOnlySelf != NULL) 
		return OneAndOnlySelf->AddType (NewType);

	return 0;
}



void SmartNoticeWarehouse :: SetDebugOn () {

	DebugFlag = true;
}


void SmartNoticeWarehouse :: SetDebugOff () {

	DebugFlag = false;
}


void SmartNoticeWarehouse :: Kill() {

	if(OneAndOnlySelf != NULL) {

		ReferenceCount = 1;
		delete OneAndOnlySelf;
		OneAndOnlySelf = NULL;
	}
}


