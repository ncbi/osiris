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
*  FileName: rgnull.h
*  Author:   Robert Goor
*
*/
//
// class RGNull which does nothing but signify a null pointer, but is Persistent
//

#ifndef _RGNULL_H_
#define _RGNULL_H_

#include "rgpersist.h"

const int _RGNULL_ = 7;

PERSISTENT_PREDECLARATION(RGNull)

class RGNull : public RGPersistent {

PERSISTENT_DECLARATION(RGNull)

public:
	RGNull () : RGPersistent () {}
	~RGNull ();

	virtual Boolean IsNullElement () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

};


#endif   /*  _RGNULL_H_  */
