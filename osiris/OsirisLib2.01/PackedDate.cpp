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
*  FileName: PackedDate.cpp
*  Author:   Robert Goor
*
*/
//
//  class PackedDate to represent the date in a manner consistent with FSA files
//

#include "PackedDate.h"
#include "rgtokenizer.h"



PackedDate :: PackedDate () : year (1900), month (1), day (1) {

}


PackedDate :: PackedDate (unsigned short yr, unsigned char mon, unsigned char dy) :
year (yr), month (mon), day (dy) {

}


PackedDate :: PackedDate (const RGString& string) {

	RGString temp (string);
	RGStringTokenizer Separator (temp);
	RGStringArray Components (3);
	RGStringArray Delimiters (3);

	Separator.AddDelimiter (':');
	Separator.Split (Components, Delimiters);
	year = (unsigned short) Components.GetIntegerValue (0);
	month = (unsigned char) Components.GetIntegerValue (1);
	day = (unsigned char) Components.GetIntegerValue (2);
}


PackedDate :: PackedDate (const RGString& date, const RGString& delimiter) {

	RGString temp (date);
	RGStringTokenizer Separator (temp);
	RGStringArray Components (3);
	RGStringArray Delimiters (3);

	Separator.AddDelimiter (delimiter);
	Separator.Split (Components, Delimiters);
	year = (unsigned short) Components.GetIntegerValue (0);
	month = (unsigned char) Components.GetIntegerValue (1);
	day = (unsigned char) Components.GetIntegerValue (2);
}


PackedDate :: PackedDate (const PackedDate& date) :
year (date.year), month (date.month), day (date.day) {

}


PackedDate :: ~PackedDate () {

}

PackedDate& PackedDate :: operator=(const PackedDate& date) {

	if (this == &date)
		return *this;

	year = date.year;
	month = date.month;
	day = date.day;
	return *this;
}


PackedDate& PackedDate :: operator=(const RGString& string) {

	RGString temp (string);
	RGStringTokenizer Separator (temp);
	RGStringArray Components (3);
	RGStringArray Delimiters (3);

	Separator.AddDelimiter (':');
	Separator.Split (Components, Delimiters);
	year = (unsigned short) Components.GetIntegerValue (0);
	month = (unsigned char) Components.GetIntegerValue (1);
	day = (unsigned char) Components.GetIntegerValue (2);
	return *this;
}


RGString PackedDate :: GetStringDate () const {

	RGString temp;
	RGString Ans;
	temp.Convert (year, 10);
	Ans = temp + ":";
	temp.Convert (month, 10);
	Ans += temp + ":";
	temp.Convert (day, 10);
	Ans += temp;
	return Ans;
}



fsaDate* PackedDate :: GetFsaDate () const {

	fsaDate* Ans = new fsaDate;
	Ans->year = year;
	Ans->month = month;
	Ans->day = day;
	return Ans;
}


RGString PackedDate :: GetOARString () const {

	RGString ans;
	ans << year;
	
	if (month < 10)
		ans << "-0" << month;
	
	else
		ans << "-" << month;

	if (day < 10)
		ans << "-0" << day << "T";

	else
		ans << "-" << day << "T";
	return ans;
}


int PackedDate :: Compare (const PackedDate* pd) const {

	if (year < pd->year)
		return -1;

	if (year > pd->year)
		return 1;

	if (month < pd->month)
		return -1;

	if (month > pd->month)
		return 1;

	if (day < pd->day)
		return -1;

	if (day > pd->day)
		return 1;

	return 0;
}


int PackedDate :: Distance (const PackedDate& date) const {

	int diff = abs (day - date.day);
	diff += 30 * abs (month - date.month);
	diff += 365 * abs (year - date.year);
	return diff;
}


unsigned PackedDate :: HashNumber (unsigned long Base) const {

	unsigned total = day + 30 * month + 365 * year;
	return total%Base;
}


Boolean PackedDate :: IsEqualTo (const PackedDate* pdate) const {

	return ((day == pdate->day) && (month == pdate->month) && (year == pdate->year));
}

