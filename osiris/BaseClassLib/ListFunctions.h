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
*  FileName: ListFunctions.h
*  Author:   Robert Goor
*
*/
//
//  These are miscellaneous list functions that do not depend on being friends of RGDList
//

#ifndef _LISTFUNCTIONS_H_
#define _LISTFUNCTIONS_H_

#include "rgpersist.h"
#include "rgdlist.h"


	// To use this method, both listA and listB must already be sorted in increasing order.
	// This function empties listA into listB
void MergeListAIntoListB (RGDList& listA, RGDList& listB);
	// This function merges listA into listB without emptying listA
void MergeAndSaveListAWithListB (RGDList& listA, RGDList& listB);
	// This function empties listA and merges by reference, with no duplication into listB
void MergeListAIntoListBByReference (RGDList& listA, RGDList& listB);
	// This function removes reference duplicates
void RemoveReferenceDuplicates (RGDList& list);


#endif  /*  _LISTFUNCTIONS_H_  */

