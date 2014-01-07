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
*  FileName: PackedTime.h
*  Author:   Robert Goor
*
*/
//
//  class PackedTime to represent time in a manner consistent with FSA files
//

#ifndef _PACKEDTIME_H_
#define _PACKEDTIME_H_

#include "rgdefs.h"
#include "rgstring.h"
#include "fsaDirEntry.h"



class PackedTime {

public:
	PackedTime ();
	PackedTime (unsigned char hr, unsigned char minit, unsigned char sec, unsigned char hsec);
	PackedTime (const RGString& string);
	PackedTime (const PackedTime& time);
	~PackedTime ();

	PackedTime& operator=(const PackedTime& time);
	PackedTime& operator=(const RGString& string);

	RGString GetStringTime () const;
	fsaTime* GetFsaTime () const;
	RGString GetOARString () const;

	Boolean IsMoreThanOneDay () const;
	int Compare (const PackedTime* pt) const;
	int Distance (const PackedTime& time) const;
	unsigned HashNumber (unsigned long Base) const;
	Boolean IsEqualTo (const PackedTime* ptime) const;
	Boolean IsAfternoon () const { return hour >= 12; }
	void ReduceTimeByHours (int hrs) { hour -= hrs; }

	int NormalizeToLessThan24Hours ();  // returns number of days

protected:
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char hsecond;

	friend PackedTime operator+(const PackedTime& t1, const PackedTime& t2);
	friend PackedTime operator+(const PackedTime& t1, unsigned long seconds);
};

#endif  /*  _PACKEDTIME_H_  */

