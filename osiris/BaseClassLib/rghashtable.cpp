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
*  FileName: rghashtable.cpp
*  Author:   Robert Goor
*
*/
//
//	class RGHashTable - hash table of doubly linked lists that is persistent.  It stores other 
//    RGPersistent objects that have defined a hash function (all, we hope)
//

#include "rghashtable.h"
#include "rgfile.h"
#include "rgvstream.h"

int RGHashTable::RGDefaultNumberOfHashLists = 257;  // This is a prime number:  see rghashtable.h

PERSISTENT_DEFINITION (RGHashTable, _RGHASHTABLE_, "HashTable")


RGHashTable :: RGHashTable () : RGPersistent (), NumberOfEntries (0), 
NumberOfLists (RGHashTable::RGDefaultNumberOfHashLists) {

	ListArray = new RGDList* [NumberOfLists];

	for (int i=0; i<NumberOfLists; i++)
		ListArray [i] = new RGDList;
}

RGHashTable :: RGHashTable (int nTables) : RGPersistent (), NumberOfEntries (0),
NumberOfLists (nTables) {

	if (NumberOfLists <= 0)
		NumberOfLists = RGHashTable::RGDefaultNumberOfHashLists;

	ListArray = new RGDList* [NumberOfLists];

	for (int i=0; i<NumberOfLists; i++)
		ListArray [i] = new RGDList;
}


RGHashTable :: RGHashTable (const RGHashTable& hTable) : RGPersistent ((RGPersistent&) hTable), 
NumberOfEntries (hTable.NumberOfEntries), NumberOfLists (hTable.NumberOfLists) {
	
	//
	// deep copy constructor - makes copies of links
	//

	ListArray = new RGDList* [NumberOfLists];

	for (int i=0; i<NumberOfLists; i++)
		ListArray [i] = new RGDList (*hTable.ListArray [i]);
}


RGHashTable :: ~RGHashTable () {

	for (int i=0; i<NumberOfLists; i++) {

		ListArray [i]->ClearAndDelete ();
		delete ListArray [i];
	}

	delete[] ListArray;
}


size_t RGHashTable :: StoreSize () const {

	size_t size = 0;

	for (int i=0; i<NumberOfLists; i++)
		size += ListArray [i]->StoreSize ();

	size += 2 * sizeof (int);
	return size;
}


int RGHashTable :: CompareTo (const RGPersistent* p) const {

	return -2;
}


unsigned RGHashTable :: HashNumber (unsigned long Base) const {

	return 0;
}


Boolean RGHashTable :: IsEqualTo (const RGPersistent* p) const {

	return FALSE;
}


void RGHashTable :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	ClearAndDelete ();
	int TempNumberOfLists;
	f.Read (TempNumberOfLists);
	RGDList** TempArray;

	if (TempNumberOfLists < NumberOfLists) {

		TempArray = new RGDList* [TempNumberOfLists];
		
		for (int i=0; i<NumberOfLists; i++) {

			if (i >= TempNumberOfLists)
				delete ListArray [i];

			else
				TempArray [i] = ListArray [i];
		}

		delete[] ListArray;
		ListArray = TempArray;
		NumberOfLists = TempNumberOfLists;
	}

	else if (TempNumberOfLists > NumberOfLists) {

		TempArray = new RGDList* [TempNumberOfLists];

		for (int i=0; i<TempNumberOfLists; i++) {

			if (i < NumberOfLists)
				TempArray [i] = ListArray [i];

			else
				TempArray [i] = new RGDList;
		}

		delete[] ListArray;
		ListArray = TempArray;
		NumberOfLists = TempNumberOfLists;
	}

	f.Read (NumberOfEntries);

	for (int i=0; i<NumberOfLists; i++)
		ListArray [i]->RestoreAll (f);

}


void RGHashTable :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
	ClearAndDelete ();
	int TempNumberOfLists;
	f >> TempNumberOfLists;
	RGDList** TempArray;

	if (TempNumberOfLists < NumberOfLists) {

		TempArray = new RGDList* [TempNumberOfLists];
		
		for (int i=0; i<NumberOfLists; i++) {

			if (i >= TempNumberOfLists)
				delete ListArray [i];

			else
				TempArray [i] = ListArray [i];
		}

		delete[] ListArray;
		ListArray = TempArray;
		NumberOfLists = TempNumberOfLists;
	}

	else if (TempNumberOfLists > NumberOfLists) {

		TempArray = new RGDList* [TempNumberOfLists];

		for (int i=0; i<TempNumberOfLists; i++) {

			if (i < NumberOfLists)
				TempArray [i] = ListArray [i];

			else
				TempArray [i] = new RGDList;
		}

		delete[] ListArray;
		ListArray = TempArray;
		NumberOfLists = TempNumberOfLists;
	}

	f >> NumberOfEntries;

	for (int i=0; i<NumberOfLists; i++)
		ListArray [i]->RestoreAll (f);
}


void RGHashTable :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	f.Write (NumberOfLists);
	f.Write (NumberOfEntries);

	for (int i=0; i<NumberOfLists; i++)
		ListArray [i]->SaveAll (f);
}


void RGHashTable :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
	f << NumberOfLists;
	f << NumberOfEntries;

	for (int i=0; i<NumberOfLists; i++)
		ListArray [i]->SaveAll (f);
}


Boolean RGHashTable :: operator==(const RGHashTable& hTable) const {

	return FALSE;
}


RGHashTable& RGHashTable :: operator=(const RGHashTable& hTable) {

	if (this == &hTable)
		return *this;

	for (int i=0; i<NumberOfLists; i++) {

		ListArray [i]->ClearAndDelete ();
		delete ListArray [i];
	}

	delete[] ListArray;

	NumberOfLists = hTable.NumberOfLists;
	NumberOfEntries = hTable.NumberOfEntries;

	ListArray = new RGDList* [NumberOfLists];

	for (int i=0; i<NumberOfLists; i++)
		ListArray [i] = new RGDList (*hTable.ListArray [i]);

	return *this;
}



void RGHashTable :: ResizeTable (int size) {

	RGPersistent* next;
	unsigned hash;
	unsigned index;
	int i;
	if (NumberOfLists < size) {

		RGDList** NewArray = new RGDList* [size];

		for (i=0; i<size; i++) {

			if (i < NumberOfLists)
				NewArray [i] = ListArray [i];

			else
				NewArray [i] = new RGDList;
		}

		delete[] ListArray;
		ListArray = NewArray;

		for (i=0; i<NumberOfLists; i++) {

			if (ListArray [i]->Entries() > 0) {

				RGDListIterator* it = new RGDListIterator (*ListArray [i]);

				while (next = (*it)()) {

					hash = next->HashNumber (size);
					index = hash%size;

					if (index != i) {

						it->RemoveCurrentItem ();
						ListArray [index]->Insert (next);
					}
				}

				delete it;
			}
		}

		NumberOfLists = size;
	}

	else if ((NumberOfLists > size) && (size > 0)) {

		RGDList** NewArray = new RGDList* [size];

		for (int i=0; i<NumberOfLists; i++) {

			if (i < size)
				NewArray [i] = ListArray [i];

			if (ListArray [i]->Entries() > 0) {

				RGDListIterator* it = new RGDListIterator (*ListArray [i]);

				while (next = (*it)()) {

					hash = next->HashNumber (size);
					index = hash%size;

					if (index != i) {

						it->RemoveCurrentItem ();
						ListArray [index]->Insert (next);
					}
				}

				delete it;
			}

			if (i >= size)
				delete ListArray [i];
		}

		delete[] ListArray;
		ListArray = NewArray;
		NumberOfLists = size;
	}
}


void RGHashTable :: ResetDefaultSize (int size) {

	if (size > 0)
		RGDefaultNumberOfHashLists = size;
}



RGPersistent* RGHashTable :: Append (RGPersistent* p) {

	RGDList* list = SelectList (p);
	NumberOfEntries++;
	return list->Append (p);
}



RGPersistent* RGHashTable :: Prepend (RGPersistent* p) {

	RGDList* list = SelectList (p);
	NumberOfEntries++;
	return list->Prepend (p);
}


Boolean RGHashTable :: Contains (const RGPersistent* p) const {

	RGDList* list = SelectList (p);
	return list->Contains (p);
}


RGPersistent* RGHashTable :: Remove (const RGPersistent* target) {

	RGDList* list = SelectList (target);
	RGPersistent* q = list->Remove (target);

	if (q != NULL)
		NumberOfEntries--;

	return q;
}


RGPersistent* RGHashTable :: Find (const RGPersistent* target) const {

	RGDList* list = SelectList (target);
	return list->Find (target);
}


int RGHashTable :: Clear () {

	for (int i=0; i<NumberOfLists; i++)
		ListArray[i]->Clear ();

	NumberOfEntries = 0;
	return 0;
}


int RGHashTable :: ClearAndDelete () {

	for (int i=0; i<NumberOfLists; i++)
		ListArray[i]->ClearAndDelete ();

	NumberOfEntries = 0;
	return 0;
}


Boolean RGHashTable :: IsEmpty () const {

	return (NumberOfEntries == 0);
}


int RGHashTable :: Entries () const {

	return NumberOfEntries;
}


int RGHashTable :: OccurrencesOf (const RGPersistent* p) const {

	RGDList* list = SelectList (p);
	return list->OccurrencesOf (p);
}



RGPersistent* RGHashTable :: Insert (RGPersistent* p) {

	RGDList* list = SelectList (p);
	NumberOfEntries++;
	return list->Insert (p);
}

Boolean RGHashTable :: ContainsReference (const RGPersistent* p) const {
	
	// contains pointer

	RGDList* list = SelectList (p);
	return list->ContainsReference (p);
}


RGPersistent* RGHashTable :: FindReference (const RGPersistent* p) {

	RGDList* list = SelectList (p);
	return list->FindReference (p);
}


int RGHashTable :: OccurrencesOfReference (const RGPersistent* p) const {

	RGDList* list = SelectList (p);
	return list->OccurrencesOfReference (p);
}



RGPersistent* RGHashTable :: RemoveReference (const RGPersistent* p) {

	RGDList* list = SelectList (p);
	RGPersistent* q = list->RemoveReference (p);

	if (q != NULL)
		NumberOfEntries--;

	return q;
}


RGPersistent* RGHashTable :: RemoveAllReferences (const RGPersistent* p) {

	RGDList* list = SelectList (p);
	int NEntriesStart = list->Entries ();
	int NEntriesEnd;
	RGPersistent* q = list->RemoveAllReferences (p);
	NEntriesEnd = list->Entries ();
	NumberOfEntries -= (NEntriesStart - NEntriesEnd);
	return q;
}


RGDList* RGHashTable :: SelectList (const RGPersistent* p) const {

	unsigned hash = p->HashNumber (NumberOfLists);

	if (hash >= NumberOfLists)
		hash = hash%NumberOfLists;

	return ListArray [hash];
}



RGHashTableIterator :: RGHashTableIterator (RGHashTable& ht) : List (&ht), CurrentListIndex (0), 
NumberOfLists (ht.NumberOfLists) {

	ListIteratorArray = new RGDListIterator* [NumberOfLists];

	for (int i=0; i<NumberOfLists; i++)
		ListIteratorArray [i] = new RGDListIterator (*List->ListArray [i]);

	CurrentIterator = ListIteratorArray [0];
}


RGHashTableIterator :: ~RGHashTableIterator () {

	for (int i=0; i<NumberOfLists; i++)
		delete ListIteratorArray [i];

	delete[] ListIteratorArray;
}

RGPersistent* RGHashTableIterator :: FindNext (const RGPersistent* target) {

	unsigned hash = FindHashNumber (target);

	if (hash < CurrentListIndex)
		return NULL;

	return ListIteratorArray [hash]->FindNext (target);
}


RGPersistent* RGHashTableIterator :: CurrentItem () {

	return CurrentIterator->CurrentItem ();
}


RGPersistent* RGHashTableIterator :: operator()() {
	
	// Advances iterator to next item and returns it; at tail, stays and returns NULL

	RGPersistent* Next = (*CurrentIterator) ();

	if (Next)
		return Next;

	if (CurrentListIndex == NumberOfLists - 1)
		return NULL;

	while (CurrentListIndex < NumberOfLists - 1) {

		CurrentListIndex++;
		CurrentIterator = ListIteratorArray [CurrentListIndex];
		Next = (*CurrentIterator) ();

		if (Next)
			return Next;
	}

	return NULL;

}


RGPersistent* RGHashTableIterator :: operator--() {
	
	// Goes back one and returns it; at head, returns NULL and Current goes to NULL

	RGPersistent* Previous = --(*CurrentIterator);

	if (Previous)
		return Previous;

	if (CurrentListIndex == 0)
		return NULL;

	while (CurrentListIndex > 0) {

		CurrentListIndex--;
		CurrentIterator = ListIteratorArray [CurrentListIndex];
		Previous = --(*CurrentIterator);

		if (Previous)
			return Previous;
	}

	return NULL;
}


RGPersistent* RGHashTableIterator :: operator++() {
	
	// Goes forward one and returns it; at tail, stays and returns NULL

	RGPersistent* Next = ++(*CurrentIterator);

	if (Next)
		return Next;

	if (CurrentListIndex == NumberOfLists - 1)
		return NULL;

	while (CurrentListIndex < NumberOfLists - 1) {

		CurrentListIndex++;
		CurrentIterator = ListIteratorArray [CurrentListIndex];
		Next = ++(*CurrentIterator);

		if (Next)
			return Next;
	}

	return NULL;
}


void RGHashTableIterator :: Reset () {
	
	// Resets iterator to beginning - needed also whenever underlying list changes; Current set to NULL

	CurrentListIndex = 0;
	CurrentIterator = ListIteratorArray [0];

	for (int i=0; i<NumberOfLists; i++)
		ListIteratorArray [i]->Reset ();
}



RGPersistent* RGHashTableIterator :: FindNextReference (const RGPersistent* target) {
	
	// finds next matching pointer

	unsigned hash = FindHashNumber (target);

	if (hash < CurrentListIndex)
		return NULL;

	return ListIteratorArray [hash]->FindNextReference (target);
}


RGPersistent* RGHashTableIterator :: RemoveCurrentItem () {

	RGPersistent* current = CurrentIterator->RemoveCurrentItem ();

	if (current == NULL)
		return NULL;

	if (CurrentIterator->CurrentItem () == NULL) {

		--(*this);
	}

	return current;
}


unsigned RGHashTableIterator :: FindHashNumber (const RGPersistent* target) {

	unsigned hash = target->HashNumber (NumberOfLists);
	hash = hash%NumberOfLists;
	return hash;
}

