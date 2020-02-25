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
*  FileName: rgdlist.cpp
*  Author:   Robert Goor
*
*/
//
//	class RGDList - doubly linked list class that is persistent.  It stores other RGPersistent objects
//

#include "rgdlist.h"
#include "rgfile.h"
#include "rgvstream.h"

PERSISTENT_DEFINITION (RGDList, _RGDLIST_, "List")


PersistentLink :: PersistentLink () :

	Item (NULL),
	Next (NULL),
	Previous (NULL) {

}



PersistentLink :: ~PersistentLink () {

}


RGDList :: RGDList () : RGPersistent (), 
	NumberOfEntries (0),
	FirstLink (NULL),
	LastLink (NULL) {

}


RGDList :: RGDList (const RGDList& clone) : RGPersistent ((RGPersistent&) clone),
NumberOfEntries (0),
FirstLink (NULL),
LastLink (NULL) {

	RGDListIterator it ((RGDList&)clone);
	RGPersistent* nextItem;
	RGPersistent* InsertItem;
	it.Reset ();

	while (nextItem = it ()) {

		InsertItem = nextItem->Copy ();
		Append (InsertItem);
	}
}


RGDList :: ~RGDList () {

	Clear ();
}


size_t RGDList :: StoreSize () const {

	PersistentLink* CurrentLink = FirstLink;
	RGPersistent* CurrentItem;
	size_t Size = RGPersistent::StoreSize () + sizeof (int);

	while (CurrentLink) {

		CurrentItem = CurrentLink->Item;
		Size += CurrentItem->StoreSize ();
		CurrentLink = CurrentLink->Next;
	}

	return Size;
}



int RGDList :: CompareTo (const RGPersistent* target) const {

	RGPersistent* test1;
	RGPersistent* test2;
	int test;
	
	if (target->GetID () != _RGDLIST_)
		return -1;

	RGDListIterator it1 ((RGDList&)*this);
	RGDListIterator it2 ((RGDList&)*target);

	while (test1 = it1 ()) {

		test2 = it2 ();

		if (test2 == NULL)
			return 1;

		test = test1->CompareTo (test2);

		if (test != 0)
			return test;
	}

	if (test2 = it2 ())
		return -1;

	return 0;
}



unsigned RGDList :: HashNumber (unsigned long Base) const {

	return 0;
}



Boolean RGDList :: IsEqualTo (const RGPersistent* target) const {

	RGPersistent* test1;
	RGPersistent* test2;
	
	if (target->GetID () != _RGDLIST_)
		return FALSE;

	RGDListIterator it1 ((RGDList&)*this);
	RGDListIterator it2 ((RGDList&)*target);

	while (test1 = it1 ()) {

		test2 = it2 ();

		if (test2 == NULL)
			return FALSE;

		if (!test1->IsEqualTo (test2))
			return FALSE;
	}

	if (test2 = it2 ())
		return FALSE;

	return TRUE;
}



void RGDList :: RestoreAll (RGFile& f) {

	RGPersistent* NextItem;
	int N;
	ClearAndDelete ();
	
	RGPersistent :: RestoreAll (f);
	f.Read (N);
	NumberOfEntries = 0;

	for (int i=0; i<N; i++) {

		f >> NextItem;
		Append (NextItem);
	}
}



void RGDList :: RestoreAll (RGVInStream& vin) {

	RGPersistent* NextItem;
	int N;
	ClearAndDelete ();
	
	RGPersistent :: RestoreAll (vin);
	vin >> N;
	NumberOfEntries = 0;

	for (int i=0; i<N; i++) {

		vin >> NextItem;
		Append (NextItem);
	}
}



void RGDList :: SaveAll (RGFile& f) const {

	RGPersistent* NextItem;
	RGDListIterator it ((RGDList&)*this);
	
	RGPersistent :: SaveAll (f);
	f.Write (NumberOfEntries);

	while (NextItem = it ()) {

		NextItem->SaveAll (f);
	}
}



void RGDList :: SaveAll (RGVOutStream& vout) const {

	RGPersistent* NextItem;
	RGDListIterator it ((RGDList&)*this);
	
	RGPersistent :: SaveAll (vout);
	vout << NumberOfEntries;

	while (NextItem = it ()) {

		NextItem->SaveAll (vout);
	}
}



Boolean RGDList :: operator==(const RGDList& target) const {

	RGPersistent* test1;
	RGPersistent* test2;
	
	RGDListIterator it1 ((RGDList&)*this);
	RGDListIterator it2 ((RGDList&)target);

	while (test1 = it1 ()) {

		test2 = it2 ();

		if (test2 == NULL)
			return FALSE;

		if (!test1->IsEqualTo (test2))
			return FALSE;
	}

	if (test2 = it2 ())
		return FALSE;

	return TRUE;
}



RGDList& RGDList :: operator=(const RGDList& target) {

	RGPersistent* NewElement;
	RGPersistent* NextInTarget;
	
	ClearAndDelete ();
	RGDListIterator it ((RGDList&)target);

	while (NextInTarget = it ()) {

		NewElement = NextInTarget->Copy ();
		Append (NewElement);
	}

	return *this;
}



RGPersistent* RGDList :: Append (RGPersistent* NewItem) {

	PersistentLink* NewLink = new PersistentLink;
	NewLink->Item = NewItem;

	if (NumberOfEntries == 0) {

		FirstLink = NewLink;
		LastLink = NewLink;
		NumberOfEntries = 1;
	}

	else {

		LastLink->Next = NewLink;
		NewLink->Previous = LastLink;
		LastLink = NewLink;
		NumberOfEntries++;
	}

	return NewItem;
}



RGPersistent* RGDList :: Prepend (RGPersistent* NewItem) {

	PersistentLink* NewLink = new PersistentLink;
	NewLink->Item = NewItem;

	if (NumberOfEntries == 0) {

		FirstLink = NewLink;
		LastLink = NewLink;
		NumberOfEntries = 1;
	}

	else {

		FirstLink->Previous = NewLink;
		NewLink->Next = FirstLink;
		FirstLink = NewLink;
		NumberOfEntries++;
	}

	return NewItem;
}



Boolean RGDList :: Contains (const RGPersistent* target) const {

	RGPersistent* Next;
	RGDListIterator it ((RGDList&)*this);

	while (Next = it ()) {

		if (Next->IsEqualTo (target))
			return TRUE;
	}

	return FALSE;
}



RGPersistent* RGDList :: Remove (const RGPersistent* target) {

	RGPersistent* Next;
	RGDListIterator it (*this);

	while (Next = it ()) {

		if (Next->IsEqualTo (target)) {

			it.RemoveCurrentItem ();
			return Next;
		}
	}

	return NULL;
}



RGPersistent* RGDList :: Find (const RGPersistent* target) const {

	RGPersistent* Next;
	RGDListIterator it ((RGDList&)*this);

	while (Next = it ()) {

		if (Next->IsEqualTo (target))
			return Next;
	}

	return NULL;
}



RGPersistent* RGDList :: First () const {

	if (FirstLink == NULL)
		return NULL;
	
	return FirstLink->Item;
}



RGPersistent* RGDList :: Last () const {

	if (LastLink == NULL)
		return NULL;
	
	return LastLink->Item;
}



RGPersistent* RGDList :: GetFirst () { 
	
	// removes first record

	RGPersistent* FirstItem;
	PersistentLink* NewFirst;

	if (NumberOfEntries == 0)
		return NULL;

	if (NumberOfEntries == 1) {

		FirstItem = FirstLink->Item;
		delete FirstLink;
		FirstLink = LastLink = NULL;
		NumberOfEntries = 0;
		return FirstItem;
	}

	FirstItem = FirstLink->Item;
	NewFirst = FirstLink->Next;
	NewFirst->Previous = NULL;
	delete FirstLink;
	FirstLink = NewFirst;
	NumberOfEntries--;
	return FirstItem;
}



RGPersistent* RGDList :: GetLast () {
	
	// removes last record

	RGPersistent* LastItem;
	PersistentLink* NewLast;

	if (NumberOfEntries == 0)
		return NULL;

	if (NumberOfEntries == 1) {

		LastItem = LastLink->Item;
		delete LastLink;
		FirstLink = LastLink = NULL;
		NumberOfEntries = 0;
		return LastItem;
	}

	LastItem = LastLink->Item;
	NewLast = LastLink->Previous;
	NewLast->Next = NULL;
	delete LastLink;
	LastLink = NewLast;
	NumberOfEntries--;
	return LastItem;

}

int RGDList :: Clear () {

	PersistentLink* Temp;
	PersistentLink* NextLink = FirstLink;

	while (NextLink) {

		Temp = NextLink;
		NextLink = Temp->Next;
		delete Temp;
	}

	NumberOfEntries = 0;
	FirstLink = LastLink = NULL;
	return 0;
}



int RGDList :: ClearAndDelete () {

	PersistentLink* Temp;
	RGPersistent* CurrentItem;
	PersistentLink* NextLink = FirstLink;

	while (NextLink) {

		Temp = NextLink;
		CurrentItem = Temp->Item;
		NextLink = Temp->Next;
		delete CurrentItem;
		delete Temp;
	}

	NumberOfEntries = 0;
	FirstLink = LastLink = NULL;
	return 0;
}



Boolean RGDList :: IsEmpty () const {

	return (NumberOfEntries == 0);
}



int RGDList :: Entries () const {

	return NumberOfEntries;
}



int RGDList :: Index (const RGPersistent* target) const {

	RGPersistent* Next;
	int i = 0;
	RGDListIterator it ((RGDList&)*this);

	while (Next = it ()) {

		if (Next->IsEqualTo (target))
			return i;

		i++;
	}

	return -1;
}



int RGDList :: OccurrencesOf (const RGPersistent* target) const {

	RGPersistent* Next;
	int i = 0;
	RGDListIterator it ((RGDList&)*this);

	while (Next = it ()) {

		if (Next->IsEqualTo (target))
			i++;
	}

	return i;
}



RGPersistent* RGDList :: Insert (RGPersistent* target) {

	PersistentLink* NextLink;
	PersistentLink* PreviousLink;
	PersistentLink* NewLink;

	if (NumberOfEntries == 0) {

		NewLink = new PersistentLink;
		NewLink->Item = target;
		FirstLink = LastLink = NewLink;
		NumberOfEntries = 1;
	}

	else if (target->CompareTo (FirstLink->Item) <= 0)
		Prepend (target);

	else if (target->CompareTo (LastLink->Item) >= 0)
		Append (target);

	else {

		NewLink = new PersistentLink;
		NewLink->Item = target;
		PreviousLink = FirstLink;
		NextLink = FirstLink->Next;

		while (NextLink) {

			if (target->CompareTo (NextLink->Item) <= 0) {  // insert here

				PreviousLink->Next = NewLink;
				NextLink->Previous = NewLink;
				NewLink->Previous = PreviousLink;
				NewLink->Next = NextLink;
				NumberOfEntries++;
				return target;
			}

			PreviousLink = NextLink;
			NextLink = NextLink->Next;
		}

		Append (target);
	}
	
	return target;
}


RGPersistent* RGDList :: InsertWithNoDuplication (RGPersistent* target) {

	PersistentLink* NextLink;
	PersistentLink* PreviousLink;
	PersistentLink* NewLink;
	int compare;

	if (NumberOfEntries == 0) {

		NewLink = new PersistentLink;
		NewLink->Item = target;
		FirstLink = LastLink = NewLink;
		NumberOfEntries = 1;
	}

	else if (target->CompareTo (FirstLink->Item) == 0)
		return NULL;

	else if (target->CompareTo (LastLink->Item) == 0)
		return NULL;

	else if (target->CompareTo (FirstLink->Item) < 0)
		Prepend (target);

	else if (target->CompareTo (LastLink->Item) > 0)
		Append (target);

	else {

		PreviousLink = FirstLink;
		NextLink = FirstLink->Next;

		while (NextLink) {

			compare = target->CompareTo (NextLink->Item);

			if (compare < 0) {  // insert here

				NewLink = new PersistentLink;
				NewLink->Item = target;
				PreviousLink->Next = NewLink;
				NextLink->Previous = NewLink;
				NewLink->Previous = PreviousLink;
				NewLink->Next = NextLink;
				NumberOfEntries++;
				return target;
			}

			else if (compare == 0)
				return NULL;

			PreviousLink = NextLink;
			NextLink = NextLink->Next;
		}

		Append (target);
	}
	
	return target;
}



RGPersistent* RGDList :: InsertWithNoReferenceDuplication (RGPersistent* target) {

	PersistentLink* NextLink;
	PersistentLink* PreviousLink;
	PersistentLink* NewLink;
	int compare;

	if (NumberOfEntries == 0) {

		NewLink = new PersistentLink;
		NewLink->Item = target;
		FirstLink = LastLink = NewLink;
		NumberOfEntries = 1;
	}

	else if (target == FirstLink->Item)
		return NULL;

	else if (target == LastLink->Item)
		return NULL;

	else if (target->CompareTo (FirstLink->Item) == 0)
		Prepend (target);

	else if (target->CompareTo (LastLink->Item) == 0)
		Append (target);

	else if (target->CompareTo (FirstLink->Item) < 0)
		Prepend (target);

	else if (target->CompareTo (LastLink->Item) > 0)
		Append (target);

	else {

		PreviousLink = FirstLink;
		NextLink = FirstLink->Next;

		while (NextLink) {

			compare = target->CompareTo (NextLink->Item);

			if (target == NextLink->Item)
				return NULL;

			else if (compare <= 0) {  // insert here

				NewLink = new PersistentLink;
				NewLink->Item = target;
				PreviousLink->Next = NewLink;
				NextLink->Previous = NewLink;
				NewLink->Previous = PreviousLink;
				NewLink->Next = NextLink;
				NumberOfEntries++;
				return target;
			}

			PreviousLink = NextLink;
			NextLink = NextLink->Next;
		}

		return NULL;
	}
	
	return target;
}


RGPersistent* RGDList :: InsertAt (int i, RGPersistent* target) {

	int N;
	RGPersistent* NextItem;
	
	if (i == 0)
		return Prepend (target);

	if (i >= NumberOfEntries)
		return Append (target);

	RGDListIterator it (*this);

	for (N=0; N<i; N++) {

		NextItem = it ();
	}

	it.InsertAfterCurrentItem (target);
	return target;
}



Boolean RGDList :: ContainsReference (const RGPersistent* target) const {
	
	// contains pointer
	RGPersistent* NextItem;
	RGDListIterator it ((RGDList&)*this);

	while (NextItem = it ()) {

		if (NextItem == target)
			return TRUE;
	}

	return FALSE;
}



RGPersistent* RGDList :: FindReference (const RGPersistent* target) {

	RGPersistent* NextItem;
	RGDListIterator it (*this);

	while (NextItem = it ()) {

		if (NextItem == target)
			return NextItem;
	}

	return NULL;
}



int RGDList :: OccurrencesOfReference (const RGPersistent* target) const {

	RGPersistent* NextItem;
	RGDListIterator it ((RGDList&)*this);
	int i = 0;

	while (NextItem = it ()) {

		if (NextItem == target)
			i++;
	}

	return i;
}



RGPersistent* RGDList :: RemoveReference (const RGPersistent* target) {

	RGPersistent* NextItem;
	RGDListIterator it (*this);

	while (NextItem = it ()) {

		if (NextItem == target) {

			return (it.RemoveCurrentItem ());
		}
	}

	return NULL;
}



RGPersistent* RGDList :: RemoveAllReferences (const RGPersistent* target) {

	RGPersistent* NextItem;
	RGPersistent* SaveItem = NULL;
	RGDListIterator it (*this);
	int i = 0;

	while (NextItem = it ()) {

		if (NextItem == target) {

			i++;
			SaveItem = NextItem;
			it.RemoveCurrentItem ();  // be careful...implement correctly so don't need to reset
		}			
	}

	if (i == 0)
		return NULL;

	return SaveItem;
}




RGDListIterator :: RGDListIterator (RGDList& list) : List (&list), 
Current (NULL) {

}



RGDListIterator :: ~RGDListIterator () {

}



RGPersistent* RGDListIterator :: FindNext (const RGPersistent* target) {

	// starts just beyond current pointer...do we reset Current Pointer?

	PersistentLink* NextLink;
	RGPersistent* NextItem;

	if (List->NumberOfEntries == 0)
		return NULL;

	if (Current == NULL)
		NextLink = List->FirstLink;

	else
		NextLink = Current->Next;

	while (NextLink) {

		NextItem = NextLink->Item;
		
		if (NextItem->IsEqualTo (target))
			return NextItem;

		NextLink = NextLink->Next;
	}

	return NULL;
}



RGPersistent* RGDListIterator :: CurrentItem () {

	if (Current == NULL)
		return NULL;
	
	return Current->Item;
}



RGPersistent* RGDListIterator :: operator()() {

	// Advances iterator to next item and returns it; at tail, stays and returns NULL

	if (List->NumberOfEntries == 0)
		return NULL;

	if (Current == NULL) {

		Current = List->FirstLink;
		return Current->Item;
	}

	if (Current == List->LastLink)
		return NULL;

	Current = Current->Next;
	return Current->Item;
}



RGPersistent* RGDListIterator :: operator--() {
	
	// Goes back one and returns it; at head, returns NULL and Current goes to NULL

	if (List->NumberOfEntries == 0)
		return NULL;

	if (Current == NULL)
		return NULL;

	if (Current == List->FirstLink) {

		Current = NULL;
		return NULL;
	}

	Current = Current->Previous;
	return Current->Item;
}



RGPersistent* RGDListIterator :: operator++() {
	
	// Goes forward one and returns it; at tail, stays and returns NULL

	if (List->NumberOfEntries == 0)
		return NULL;

	if (Current == NULL) {

		Current = List->FirstLink;
		return Current->Item;
	}

	if (Current == List->LastLink)
		return NULL;

	Current = Current->Next;
	return Current->Item;
}



void RGDListIterator :: Reset () {
	
	// Resets iterator to beginning - needed also whenever underlying list changes; Current set to NULL

	Current = NULL;
}


void RGDListIterator :: ResetToEnd () {

	// Resets iterator to last element - needed for iterating from last to first.  Must test at beginning
	// of while loop and iterate "--" at end.

	Current = List->LastLink;
}



RGPersistent* RGDListIterator :: FindNextReference (const RGPersistent* target) {
	
	// finds next matching pointer starting just beyond Current pointer...Do we reset Current Pointer?

	PersistentLink* NextLink;
	RGPersistent* NextItem;

	if (List->NumberOfEntries == 0)
		return NULL;

	if (Current == NULL)
		NextLink = List->FirstLink;

	else
		NextLink = Current->Next;

	while (NextLink) {

		NextItem = NextLink->Item;
		
		if (target == NextItem)
			return NextItem;

		NextLink = NextLink->Next;
	}

	return NULL;
}



RGPersistent* RGDListIterator :: InsertAfterCurrentItem (RGPersistent* target) {

	// Reset Current Pointer to new link

	RGPersistent* Targ;

	if (Current == NULL) {

		Targ = List->Prepend (target);
		Current = List->FirstLink;
		return Targ;
	}

	if (Current == List->LastLink) {

		Targ = List->Append (target);
		Current = List->LastLink;
		return Targ;
	}

	PersistentLink* NewLink = new PersistentLink;
	PersistentLink* NextLink = Current->Next;
	NewLink->Item = target;
	NextLink->Previous = NewLink;
	Current->Next = NewLink;
	NewLink->Next = NextLink;
	NewLink->Previous = Current;
	Current = NewLink;
	List->NumberOfEntries++;
	return target;
}



RGPersistent* RGDListIterator :: RemoveCurrentItem () {

	// Reset Current Pointer to previous, unless at head

	RGPersistent* Item;
	PersistentLink* Prev;
	PersistentLink* Next;

	if (Current == NULL)
		return NULL;

	if (Current == List->FirstLink) {

		Item = List->GetFirst ();
		Current = NULL;
	}

	else if (Current == List->LastLink) {

		Item = List->GetLast ();
		Current = List->LastLink;
	}

	else {

		Prev = Current->Previous;
		Next = Current->Next;
		Item = Current->Item;
		Prev->Next = Next;
		Next->Previous = Prev;
		List->NumberOfEntries--;
		delete Current;
		Current = Prev;
	}

	return Item;
}



RGPersistent* RGDListIterator :: RemoveNext (const RGPersistent* target) {
	
	// finds next matching item starting beyond Current and removes it

	PersistentLink* NextLink;
	PersistentLink* Prev;
	PersistentLink* Next;
	RGPersistent* NextItem;

	if (List->NumberOfEntries == 0)
		return NULL;

	if (Current == NULL)
		NextLink = List->FirstLink;

	else
		NextLink = Current->Next;

	while (NextLink) {

		NextItem = NextLink->Item;
		
		if (NextItem->IsEqualTo (target)) {

			Prev = NextLink->Previous;
			Next = NextLink->Next;

			if (Prev == NULL)
				List->FirstLink = Next;

			else
				Prev->Next = Next;

			if (Next == NULL)
				List->LastLink = Prev;

			else
				Next->Previous = Prev;

			List->NumberOfEntries--;
			delete NextLink;
			return NextItem;
		}

		NextLink = NextLink->Next;
	}

	return NULL;
}



RGPersistent* RGDListIterator :: RemoveNextReference (const RGPersistent* target) {
	
	// finds next matching pointer starting beyond Current and removes it

	PersistentLink* NextLink;
	PersistentLink* Prev;
	PersistentLink* Next;
	RGPersistent* NextItem;

	if (List->NumberOfEntries == 0)
		return NULL;

	if (Current == NULL)
		NextLink = List->FirstLink;

	else
		NextLink = Current->Next;

	while (NextLink) {

		NextItem = NextLink->Item;
		
		if (NextItem == target) {

			Prev = NextLink->Previous;
			Next = NextLink->Next;

			if (Prev == NULL)
				List->FirstLink = Next;

			else
				Prev->Next = Next;

			if (Next == NULL)
				List->LastLink = Prev;

			else
				Next->Previous = Prev;

			List->NumberOfEntries--;
			delete NextLink;
			return NextItem;
		}

		NextLink = NextLink->Next;
	}

	return NULL;
}
