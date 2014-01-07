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
*  FileName: PackedTime.cpp
*  Author:   Robert Goor
*
*/
//
//  class PackedTime to represent time in a manner consistent with FSA files
//


#include "PackedTime.h"
#include "rgtokenizer.h"


PackedTime :: PackedTime () : hour (0), minute (0), second (0), hsecond (0) {

}


PackedTime :: PackedTime (unsigned char hr, unsigned char min, unsigned char sec, unsigned char hsec) :
hour (hr), minute (min), second (sec), hsecond (hsec) {

}


PackedTime :: PackedTime (const RGString& string) {
	
	RGString temp (string);
	RGStringTokenizer Separator (temp);
	RGStringArray Components (4);
	RGStringArray Delimiters (4);

	Separator.AddDelimiter (':');
	Separator.Split (Components, Delimiters);
	hour = (unsigned char) Components.GetIntegerValue (0);
	minute = (unsigned char) Components.GetIntegerValue (1);
	second = (unsigned char) Components.GetIntegerValue (2);
	hsecond = (unsigned char) Components.GetIntegerValue (3);
}


PackedTime :: PackedTime (const PackedTime& time) :
hour (time.hour), minute (time.minute), second (time.second), hsecond (time.hsecond) {

}


PackedTime :: ~PackedTime () {}

PackedTime& PackedTime :: operator=(const PackedTime& time) {

	if (this == &time)
		return *this;

	hour = time.hour;
	minute = time.minute;
	second = time.second;
	hsecond = time.hsecond;
	return *this;
}


PackedTime& PackedTime :: operator=(const RGString& string) {

	RGString temp (string);
	RGStringTokenizer Separator (temp);
	RGStringArray Components (4);
	RGStringArray Delimiters (4);

	Separator.AddDelimiter (':');
	Separator.Split (Components, Delimiters);
	hour = (unsigned char) Components.GetIntegerValue (0);

	if (Components.Length () > 1)
		minute = (unsigned char) Components.GetIntegerValue (1);

	else
		minute = (unsigned char)0;

	if (Components.Length () > 2)
		second = (unsigned char) Components.GetIntegerValue (2);

	else
		second = (unsigned char)0;

	if (Components.Length () > 3)
		hsecond = (unsigned char) Components.GetIntegerValue (3);

	else
		hsecond = (unsigned char)0;

	return *this;
}


RGString PackedTime :: GetStringTime () const {

	RGString temp;
	RGString Ans;
	temp.Convert (hour, 10);
	Ans = temp + ":";
	temp.Convert (minute, 10);
	Ans += temp + ":";
	temp.Convert (second, 10);
	Ans += temp + ":";
	temp.Convert (hsecond, 10);
	Ans += temp;
	return Ans;
}


fsaTime* PackedTime :: GetFsaTime () const {

	fsaTime* Ans = new fsaTime;
	Ans->hour = hour;
	Ans->minute = minute;
	Ans->second = second;
	Ans->hsecond = hsecond;
	return Ans;
}


RGString PackedTime :: GetOARString () const {

	RGString ans;

	if (hour < 10)
		ans << "0" << hour;

	else
		ans << hour;

	if (minute < 10)
		ans << ":0" << minute;
	
	else
		ans << ":" << minute;
	
	if (second < 10)
		ans << ":0" << second;

	else
		ans << second;

	return ans;
}


Boolean PackedTime :: IsMoreThanOneDay () const {

	if (hour > 23)
		return TRUE;
	
	return FALSE;
}


int PackedTime :: Compare (const PackedTime* pt) const {

	if (hour < pt->hour)
		return -1;

	if (hour > pt->hour)
		return 1;

	if (minute < pt->minute)
		return -1;

	if (minute > pt->minute)
		return 1;

	if (second < pt->second)
		return -1;

	if (second > pt->second)
		return 1;

	if (hsecond < pt->hsecond)
		return -1;

	if (hsecond > pt->hsecond)
		return 1;

	return 0;
}


int PackedTime :: Distance (const PackedTime& time) const {

	int diff = abs (hsecond - time.hsecond);
	diff += 100 * abs (second - time.second);
	diff += 6000 * abs (minute - time.minute);
	diff += 360000 * abs (hour - time.hour);
	return diff;
}


unsigned PackedTime :: HashNumber (unsigned long Base) const {

	unsigned total = hsecond + 100 * second + 6000 * minute + 360000 * hour;
	return total%Base;
}


Boolean PackedTime :: IsEqualTo (const PackedTime* ptime) const {

	return ((hsecond == ptime->hsecond) && (second == ptime->second) && (minute == ptime->minute) &&
		(hour == ptime->hour));
}


int PackedTime :: NormalizeToLessThan24Hours () {

	int days = hour / 24;
	hour = hour%24;
	return days;
}


PackedTime operator+(const PackedTime& t1, const PackedTime& t2) {

	int temp;
	int carry;
	PackedTime Ans;

	temp = t1.hsecond + t2.hsecond;

	if (temp > 99) {

		carry = 1;
		temp -= 100;
	}

	else
		carry = 0;

	Ans.hsecond = temp;
	temp = t1.second + t2.second + carry;

	if (temp > 59) {

		carry = 1;
		temp -= 60;
	}

	else
		carry = 0;

	Ans.second = temp;
	temp = t1.minute + t2.minute + carry;

	if (temp > 59) {

		carry = 1;
		temp -= 60;
	}

	else
		carry = 0;

	Ans.minute = temp;
	temp = t1.hour + t2.hour + carry;
	Ans.hour = temp;  // the hour can be unnormalized in this operator...test for it!

	return Ans;
}


PackedTime operator+(const PackedTime& t1, unsigned long seconds) {

	PackedTime t2;
	t2.hsecond = 0;
	int mins = seconds / 60;
	t2.second = (unsigned char)seconds%60;
	int hrs = mins / 60;
	t2.minute = mins%60;
	t2.hour = hrs;
	return t1 + t2;
}


