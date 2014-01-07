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
*  FileName: rgpersist.cpp
*  Author:   Robert Goor
*
*/
//
//	class RGPersistent - base class for all persistent class objects
//


#include "rgpersist.h"
#include "rgdlist.h"
#include "rgfile.h"
#include "rgvstream.h"


PERSISTENT_DEFINITION (RGPersistent, _RGPERSIST_, "PersistentBase")



RGPersistent :: RGPersistent () {

}


RGPersistent :: ~RGPersistent () {

}


size_t RGPersistent :: StoreSize () const {

	return 2 * sizeof (int);
}



int RGPersistent :: CompareTo (const RGPersistent* target) const {

	return -1;
}



unsigned RGPersistent :: HashNumber (unsigned long Base) const {

	return 0;
}



Boolean RGPersistent :: IsEqualTo (const RGPersistent* target) const {

	return FALSE;
}



void RGPersistent :: RestoreAll (RGFile& f) {

	int temp;
	int temp2 = GetID ();
	f.Read (temp);

	if (temp == temp2)
		f.Read (temp);
}



void RGPersistent :: RestoreAll (RGVInStream& vin) {

	int temp;
	int temp2 = GetID ();
	vin >> temp;

	if (temp == temp2)
		vin >> temp;
}



void RGPersistent :: SaveAll (RGFile& f) const {

	int temp = GetID ();
	f.Write (temp);
	f.Write (-temp);
}



void RGPersistent :: SaveAll (RGVOutStream& vout) const {

	int temp = GetID ();
	vout << temp;
	vout << -temp;
}



RGPersistent* RGPersistent :: RestoreFrom (RGVInStream& vin) {
	
	// Gets subclass type

	int TempID;
	vin >> TempID;
	return RGWarehouse::TestForID (TempID);
}



RGPersistent* RGPersistent :: RestoreFrom (RGFile& fin) {
	
	// Gets subclass type

	int TempID;
	fin.Read (TempID);
	RGPersistent* temp = RGWarehouse::TestForID (TempID);
	return temp;
}


Boolean RGPersistent :: IsNullElement () const {

	return FALSE;
}
