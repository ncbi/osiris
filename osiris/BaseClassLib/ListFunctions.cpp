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
*  FileName: ListFunctions.cpp
*  Author:   Robert Goor
*
*/
//
//  Miscellaneous non-friend list functions
//

#include "ListFunctions.h"


void MergeListAIntoListB (RGDList& listA, RGDList& listB) {

	// To use this method, both listA and listB must already be sorted in increasing order.
	// This function empties listA into listB

	RGPersistent* p;
	RGPersistent* q;

	RGDList temp;
	q = listB.GetFirst ();
	p = listA.GetFirst ();

	while (true) {

		if ((q == NULL) && (p == NULL))
			break;

		if (q == NULL) {

			temp.Append (p);
			p = listA.GetFirst ();
		}

		else if (p == NULL) {

			temp.Append (q);
			q = listB.GetFirst ();
		}

		else if (p == q) {

			p = listA.GetFirst ();
		}
		
		else if (p->CompareTo (q) <= 0) {

			temp.Append (p);
			p = listA.GetFirst ();
		}

		else {

			temp.Append (q);
			q = listB.GetFirst ();
		}
	}

	while (p = temp.GetFirst ())
		listB.Append (p);
}


void MergeAndSaveListAWithListB (RGDList& listA, RGDList& listB) {

	// To use this method, both listA and listB must already be sorted in increasing order.
	// This function merges listA into listB without emptying listA

	RGPersistent* p;
	RGPersistent* q;

	RGDListIterator it (listA);

	RGDList temp;
	q = listB.GetFirst ();
	p = it ();

	while (true) {

		if ((q == NULL) && (p == NULL))
			break;

		if (q == NULL) {

			temp.Append (p);
			p = it ();
		}

		else if (p == NULL) {

			temp.Append (q);
			q = listB.GetFirst ();
		}

		else if (p == q)
			p = it ();
		
		else if (p->CompareTo (q) <= 0) {

			temp.Append (p);
			p = it ();
		}

		else {

			temp.Append (q);
			q = listB.GetFirst ();
		}
	}

	while (p = temp.GetFirst ())
		listB.Append (p);
}


void MergeListAIntoListBByReference (RGDList& listA, RGDList& listB) {

	// To use this method, both listA and listB must already be sorted in increasing order.
	// This function empties listA into listB

	RGPersistent* p;
	RGPersistent* q;

	RGDList temp;
	q = listB.GetFirst ();
	p = listA.GetFirst ();

	while (true) {

		if ((q == NULL) && (p == NULL))
			break;

		if (q == NULL) {

			temp.InsertWithNoReferenceDuplication (p);
			p = listA.GetFirst ();
		}

		else if (p == NULL) {

			temp.InsertWithNoReferenceDuplication (q);
			q = listB.GetFirst ();
		}

		else if (p == q) {

			temp.InsertWithNoReferenceDuplication (q);
			p = listA.GetFirst ();
			q = listB.GetFirst ();
		}

		else if (p->CompareTo (q) <= 0) {

			temp.InsertWithNoReferenceDuplication (p);
			p = listA.GetFirst ();
		}

		else {

			temp.InsertWithNoReferenceDuplication (q);
			q = listB.GetFirst ();
		}
	}

	while (p = temp.GetFirst ())
		listB.Append (p);
}


void RemoveReferenceDuplicates (RGDList& list) {

	RGPersistent* q;
	RGPersistent* p;
	RGDList temp;

	while (q = list.GetLast ()) {

		p = temp.InsertWithNoReferenceDuplication (q);
	}

	while (q = temp.GetFirst ())
		list.Append (q);
}


