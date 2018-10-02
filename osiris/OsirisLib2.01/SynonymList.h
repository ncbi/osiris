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
*  FileName: SynonymList.h
*  Author:   Robert Goor
*
*/
//
//  class SynonymList retains a list of synonyms for a given word or concept and tests if a word equals one of the synonyms 
//  or if a word contains one of the synonyms.  Later, we'll add xml input capability
//

#ifndef _SYNONMYMLIST_H_
#define _SYNONMYMLIST_H_

#include "rgdlist.h"
#include "rgstring.h"

class SynonymList {

public:
	SynonymList ();
	SynonymList (const RGString& tagName, const RGString& xmlString);
	~SynonymList ();

	bool IsValid () const { return mValid; }
	Boolean IsEmpty () const { return mList.IsEmpty (); }

	void AddSynonym (const RGString& str);
	void AddSynonym (const char* str);
	void FlushSynonymList () { mList.ClearAndDelete (); }

	bool DoesTargetStringEqualASynonym (const RGString& target);
	bool DoesTargetStringContainASynonym (const RGString& target);

	bool DoesTargetStringEqualASynonymCaseIndep (const RGString& target);
	bool DoesTargetStringContainASynonymCaseIndep (const RGString& target);

protected:
	RGDList mList;
	RGDListIterator mIt;
	bool mValid;
};


#endif   /*  _SYNONMYMLIST_H_  */

