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
*  FileName: rgpint.bak.h
*  Author:   Robert Goor
*
*/
//
// class RGPInt or persistent integer...it is an integer, but it can be collected in an RGDList and saved
//

#ifndef _RGPINT_H_
#define _RGPINT_H_

#include "rgpersist.h"

class RGFile;
class RGVInStream;
class RGVOutStream;

const int _RGPINT_ = 8;

PERSISTENT_PREDECLARATION (RGPInt)

class DllExport RGPInt : public RGPersistent {

PERSISTENT_DECLARATION(RGPInt)

public:
	RGPInt () : Value (0) {}
	RGPInt (int val) : Value (val) {}
	RGPInt (const RGPInt& rhs) : Value (rhs.Value) {}
	~RGPInt ();

	RGPInt& operator=(const RGPInt& rhs) { Value = rhs.Value; return *this; }
	RGPInt& operator=(int i) { Value = i; return *this; }
	Boolean operator==(const RGPInt& rhs) const { return Value == rhs.Value; }
	operator int() const {return Value;}

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber () const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;


protected:
	int Value;
};

#endif  /*  _RGPINT_H_  */

