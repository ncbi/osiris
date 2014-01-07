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
*  FileName: SmartNotice.h
*  Author:   Robert Goor
*
*/
//
//	Declaration file for class SmartNotice, which is the base class for subclasses designed to give access to message arrays
//

#ifndef _SMARTNOTICE_H_
#define _SMARTNOTICE_H_


#include "rgstring.h"
#include "rgpersist.h"
//#include "RGTextOutput.h"
#include "rgfile.h"
#include "rghashtable.h"


class SmartNotice : public RGPersistent {

ABSTRACT_DECLARATION (SmartNotice)

public:
	SmartNotice ();
	virtual ~SmartNotice ();

	void SetName (const RGString& name) { SmartNotice::sName = name; }

	virtual void SetIndexAndScope (int index, int scope) const {;}
	virtual int GetMessageIndex () const { return -1; }
//	virtual int GetMessageNumber () const { return mCurrentMsgNumber; }
	virtual int GetScope () const { return -1; }
	virtual RGString GetName () const { return SmartNotice::sName; }

//	void SetMessageNumber (int msgNo) { mCurrentMsgNumber = msgNo; }

	virtual Boolean IsEqualTo (const RGPersistent* note) const;
	virtual unsigned HashNumber (unsigned long Base) const;

	static int LoadType () { return -1; }

protected:
	static int sSubject;
	static RGString sName;

//	int mCurrentMsgNumber;
};



class SmartNoticeWarehouse {

public:
	SmartNoticeWarehouse ();
	~SmartNoticeWarehouse ();

	void* operator new (size_t);
	void operator delete (void*);

	int AddType (SmartNotice* NewType);
//	SmartNotice* FindType (int Target);
	SmartNotice* FindType (const RGString& name);
	
	static int AddClassType (SmartNotice* NewType);

	static void SetDebugOn ();
	static void SetDebugOff ();  // it's off by default
 
	static void Kill();

protected:
	RGHashTable* ClassTypes;
	RGHashTableIterator* TypeIterator;

	static int ReferenceCount;
	static SmartNoticeWarehouse* OneAndOnlySelf;
	static bool DebugFlag;
};



#endif /*  _SMARTNOTICE_H_  */


