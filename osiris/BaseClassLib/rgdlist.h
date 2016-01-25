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
*  FileName: rgdlist.h
*  Author:   Robert Goor
*
*/
//
//	class RGDList - doubly linked list class that is persistent.  It stores other RGPersistent objects
//

#ifndef _RGDLIST_H_
#define _RGDLIST_H_

#include "rgpersist.h"


const int _RGDLIST_ = 2;


struct PersistentLink {

	RGPersistent* Item;
	PersistentLink* Next;
	PersistentLink* Previous;

	PersistentLink ();
	~PersistentLink ();
};


PERSISTENT_PREDECLARATION (RGDList)


class RGDList : public RGPersistent {

friend class RGDListIterator;
PERSISTENT_DECLARATION (RGDList)

public:

	RGDList ();
	RGDList (const RGDList&);  // deep copy constructor - makes copies of links
	virtual ~RGDList ();

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	Boolean operator==(const RGDList&) const;
	RGDList& operator=(const RGDList&);

	RGPersistent* Append (RGPersistent*);
	RGPersistent* Prepend (RGPersistent*);
	Boolean Contains (const RGPersistent*) const;
	RGPersistent* Remove (const RGPersistent* target);
	RGPersistent* Find (const RGPersistent* target) const;
	RGPersistent* First () const;
	RGPersistent* Last () const;
	RGPersistent* GetFirst ();  // removes first record
	RGPersistent* GetLast ();   // removes last record

	int Clear ();
	int ClearAndDelete ();

	Boolean IsEmpty () const;
	int Entries () const;
	int Index (const RGPersistent*) const;
	int OccurrencesOf (const RGPersistent*) const;

	RGPersistent* Insert (RGPersistent*);
	RGPersistent* InsertAt (int i, RGPersistent*);
	RGPersistent* InsertWithNoDuplication (RGPersistent*);
	RGPersistent* InsertWithNoReferenceDuplication (RGPersistent*);

	Boolean ContainsReference (const RGPersistent*) const;  // contains pointer
	RGPersistent* FindReference (const RGPersistent*);
	int OccurrencesOfReference (const RGPersistent*) const;
	RGPersistent* RemoveReference (const RGPersistent*);
	RGPersistent* RemoveAllReferences (const RGPersistent*);

protected:

	int NumberOfEntries;
	PersistentLink* FirstLink;
	PersistentLink* LastLink;

};



class RGDListIterator {

public:
	RGDListIterator (RGDList&);
	virtual ~RGDListIterator ();

	RGPersistent* FindNext (const RGPersistent*);
	RGPersistent* CurrentItem ();
	RGPersistent* operator()();  // Advances iterator to next item and returns it; at tail, stays and returns NULL
	RGPersistent* operator--();  // Goes back one and returns it; at head, returns NULL and Current goes to NULL
	RGPersistent* operator++();  // Goes forward one and returns it; at tail, stays and returns NULL
	void Reset ();   // Resets iterator to beginning - needed also whenever underlying list changes; Current set to NULL
	void ResetToEnd ();
	void ResetTo (const RGDListIterator& it) { Current = it.Current; }

	RGPersistent* FindNextReference (const RGPersistent*);  // finds next matching pointer
	RGPersistent* InsertAfterCurrentItem (RGPersistent*);
	RGPersistent* RemoveCurrentItem ();
	RGPersistent* RemoveNext (const RGPersistent*);  // finds next matching item and removes it
	RGPersistent* RemoveNextReference (const RGPersistent*);   // finds next matching pointer and removes it

protected:

	RGDList* List;
	PersistentLink* Current;
};


#endif   /*  _RGDLIST_H_  */
