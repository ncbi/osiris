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
*  FileName: rghashtable.h
*  Author:   Robert Goor
*
*/
//
//	class RGHashTable - hash table of doubly linked lists that is persistent.  It stores other 
//    RGPersistent objects that have defined a hash function (all, we hope)
//
//	The number of lists in the hash table determines the hash function parameter as well and this number
//		should be a prime for best (most even) distribution of the range of hash numbers over the number of 
//		tables.  To this end, a list of the primes between 500 and 1050 appear in the comment below:
//		503 
//		509       521       523       541       547       557       563       569 
//		571       577       587       593       599       601       607       613 
//		617       619       631       641       643       647       653       659 
//		661       673       677       683       691       701       709       719 
//		727       733       739       743       751       757       761       769 
//		773       787       797       809       811       821       823       827 
//		829       839       853       857       859       863       877       881 
//		883       887       907       911       919       929       937       941 
//		947       953       967       971       977       983       991       997 
//		1009      1013      1019      1021      1031      1033      1039      1049 
//
//	See primes.utm.edu for more prime numbers
//

#ifndef _RGHASHTABLE_H_
#define _RGHASHTABLE_H_

#include "rgpersist.h"
#include "rgdlist.h"


const int _RGHASHTABLE_ = 32;


PERSISTENT_PREDECLARATION (RGHashTable)


class RGHashTable : public RGPersistent {

friend class RGHashTableIterator;
PERSISTENT_DECLARATION (RGHashTable)

public:
	RGHashTable ();
	RGHashTable (int nTables);
	RGHashTable (const RGHashTable&);  // deep copy constructor - makes copies of links
	virtual ~RGHashTable ();

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	Boolean operator==(const RGHashTable&) const;
	RGHashTable& operator=(const RGHashTable&);

	void ResizeTable (int size);
	static void ResetDefaultSize (int size);

	RGPersistent* Append (RGPersistent*);
	RGPersistent* Prepend (RGPersistent*);
	Boolean Contains (const RGPersistent*) const;
	RGPersistent* Remove (const RGPersistent* target);
	RGPersistent* Find (const RGPersistent* target) const;
	
	int Clear ();
	int ClearAndDelete ();

	Boolean IsEmpty () const;
	int Entries () const;
	int OccurrencesOf (const RGPersistent*) const;
	int GetNumberOfTables () const { return NumberOfLists; }

	RGPersistent* Insert (RGPersistent*);
	
	Boolean ContainsReference (const RGPersistent*) const;  // contains pointer
	RGPersistent* FindReference (const RGPersistent*);
	int OccurrencesOfReference (const RGPersistent*) const;
	RGPersistent* RemoveReference (const RGPersistent*);
	RGPersistent* RemoveAllReferences (const RGPersistent*);

protected:
	RGDList** ListArray;
	int NumberOfEntries;
	unsigned long NumberOfLists;

	RGDList* SelectList (const RGPersistent*) const;

	static int RGDefaultNumberOfHashLists;
};



class RGHashTableIterator {

public:
	RGHashTableIterator (RGHashTable&);
	virtual ~RGHashTableIterator ();

	RGPersistent* FindNext (const RGPersistent*);
	RGPersistent* CurrentItem ();
	RGPersistent* operator()();  // Advances iterator to next item and returns it; at tail, stays and returns NULL
	RGPersistent* operator--();  // Goes back one and returns it; at head, returns NULL and Current goes to NULL
	RGPersistent* operator++();  // Goes forward one and returns it; at tail, stays and returns NULL
	void Reset ();   // Resets iterator to beginning - needed also whenever underlying list changes; Current set to NULL

	RGPersistent* FindNextReference (const RGPersistent*);  // finds next matching pointer
	RGPersistent* RemoveCurrentItem ();

protected:

	RGHashTable* List;
	RGDListIterator** ListIteratorArray;
	int CurrentListIndex;
	int NumberOfLists;
	RGDListIterator* CurrentIterator;

	unsigned FindHashNumber (const RGPersistent* p);
};


#endif   /*  _RGHASHTABLE_H_  */

