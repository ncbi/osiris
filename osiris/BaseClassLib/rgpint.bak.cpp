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
*  FileName: rgpint.bak.cpp
*  Author:   Robert Goor
*
*/
//
// class RGPInt, a persistent integer class
//

#include "rgpint.h"
#include "rgfile.h"
#include "rgvstream.h"

PERSISTENT_DEFINITION (RGPInt, _RGPINT_)

RGPInt :: ~RGPInt () {

}


size_t RGPInt :: StoreSize () const {

	return RGPersistent::StoreSize () + sizeof (int);
}


int RGPInt :: CompareTo (const RGPersistent* p) const {

	RGPInt* q = (RGPInt*)p;
	int val = q->Value;

	if (Value < val)
		return -1;

	if (Value > val)
		return 1;

	return 0;
}



unsigned RGPInt :: HashNumber () const {

	return Value;
}


Boolean RGPInt :: IsEqualTo (const RGPersistent* p) const {

	RGPInt* q = (RGPInt*)p;
	return Value == q->Value;
}


void RGPInt :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	f.Read (Value);
}


void RGPInt :: RestoreAll (RGVInStream& vin) {

	RGPersistent::RestoreAll (vin);
	vin >> Value;
}


void RGPInt :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	f.Write (Value);
}


void RGPInt :: SaveAll (RGVOutStream& vout) const {

	RGPersistent::SaveAll (vout);
	vout << Value;
}
