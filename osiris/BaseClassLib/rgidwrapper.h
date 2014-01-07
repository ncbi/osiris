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
*  FileName: rgidwrapper.h
*  Author:   Robert Goor
*
*/
//
// This class wraps a persistent pointer and redirects the persistent object's numerical ID to the wrapper's hash 
// function.  Equality is also computed on the basis of the persistent class's hash function.  The purpose is to 
// allow a straightforward use of the HashTable interface to store persistent prototypes and enable fast
// retrieval
//

#ifndef _RGIDWRAPPER_H_
#define _RGIDWRAPPER_H_

#include "rgpersist.h"

const int _RGIDWRAPPER_ = 37;

PERSISTENT_PREDECLARATION (RGIDWrapper)

class RGIDWrapper : public RGPersistent {

PERSISTENT_DECLARATION (RGIDWrapper)

public:
	RGIDWrapper ();
	RGIDWrapper (RGPersistent* p);
	virtual ~RGIDWrapper ();

	void SetSearchID (int id);
	RGPersistent* GetPointer ();
	int GetSearchID () const;

	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

protected:
	RGPersistent* ptr;
	int SearchID;
};


#endif  /*  _RGIDWRAPPER_H_  */

