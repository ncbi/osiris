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
*  FileName: rgpscalar.h
*  Author:   Robert Goor
*
*/
//
// class RGPScalar or persistent scalar...it is a acalar, which can be subclassed as an integer, double, unsigned
// or boolean, but it can be collected in an RGDList and saved
//

#ifndef _RGPSCALAR_H_
#define _RGPSCALAR_H_

#include "rgpersist.h"
#include "rgstring.h"
#include "rgdefs.h"

class RGFile;
class RGVInStream;
class RGVOutStream;

const int _RGPINT_ = 8;
const int _RGPSCALAR_ = 11;
const int _RGPDOUBLE_ = 12;
const int _RGPUNSIGNED_ = 13;
const int _RGPBOOLEAN_ = 14;
const int _RGPDECIMAL_	= 39;
const int _RGXMLINT_ = 40;
const int _RGXMLDOUBLE_ = 41;
const int _RGXMLUNSIGNED_ = 42;
const int _RGXMLSTRINGSCALAR_ = 44;


PERSISTENT_PREDECLARATION (RGPScalar)
PERSISTENT_PREDECLARATION (RGPInt)
PERSISTENT_PREDECLARATION (RGPDouble)
PERSISTENT_PREDECLARATION (RGPUnsigned)

PERSISTENT_PREDECLARATION (RGPBoolean)
PERSISTENT_PREDECLARATION (RGPDecimal)

PERSISTENT_PREDECLARATION (RGXMLInt)
PERSISTENT_PREDECLARATION (RGXMLDouble)
PERSISTENT_PREDECLARATION (RGXMLUnsigned)
PERSISTENT_PREDECLARATION (RGXMLStringScalar)


class RGPScalar : public RGPersistent {

PERSISTENT_DECLARATION(RGPScalar)

public:
	RGPScalar ();
	RGPScalar (const RGPScalar&);
	virtual ~RGPScalar ();

	virtual RGPScalar& operator=(const RGPScalar& rhs);
	RGPScalar& operator=(int i) { SetTo (i); return *this; }
	RGPScalar& operator=(double d) { SetTo (d); return *this; }
	RGPScalar& operator=(unsigned long i) { SetTo (i); return *this; }

	Boolean operator==(const RGPScalar& rhs) const;
	Boolean operator<=(const RGPScalar& rhs) const;
	Boolean operator<(const RGPScalar& rhs) const;
	Boolean operator>=(const RGPScalar& rhs) const;
	Boolean operator>(const RGPScalar& rhs) const;
	Boolean operator!=(const RGPScalar& rhs) const;

	operator int() const { return GetInt (); }
	operator double() const { return GetDouble (); }
	operator unsigned long() const { return GetUnsigned (); }

	Boolean GetBooleanValue () const { return GetBoolean (); }
	RGString GetStringValue (unsigned long length) const { return GetString (length); }

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual int Compare (const RGPScalar& rhs) const;
	virtual int Compare (int i) const;
	virtual int Compare (double d) const;
	virtual int Compare (unsigned long i) const;
	virtual int Compare (const RGString& str) const;

	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual int GetInt () const;
	virtual double GetDouble () const;
	virtual unsigned long GetUnsigned () const;
	virtual Boolean GetBoolean () const;
	virtual RGString GetString (unsigned long length) const;

	virtual void SetTotalDigits (int n);
	virtual void SetFractionalDigits (int n);

	virtual void SetMaxInclusive (const RGPScalar& max);
	virtual void SetMinInclusive (const RGPScalar& min);

	virtual void SetMaxExclusive (const RGPScalar& max);
	virtual void SetMinExclusive (const RGPScalar& min);

	virtual void ResetBounds ();

	virtual unsigned long TotalLength () const;
	virtual unsigned long FractionDigits () const;
	virtual unsigned long TotalDigits () const;

	Boolean BoundsViolation () const { return ViolatedBounds; }

protected:
	Boolean ViolatedBounds;

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);
};


class RGPInt : public RGPScalar {

PERSISTENT_DECLARATION(RGPInt)

public:
	RGPInt () : RGPScalar (), Value (0) {}
	RGPInt (int val) : RGPScalar (), Value (val) {}
	RGPInt (const RGPInt& rhs) : RGPScalar (), Value (rhs.Value) {}

	~RGPInt ();

	virtual RGPScalar& operator=(const RGPScalar& rhs);
	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual int Compare (const RGPScalar& rhs) const;
	virtual int Compare (int i) const;
	virtual int Compare (double d) const;
	virtual int Compare (unsigned long i) const;
	virtual int Compare (const RGString& str) const;

	virtual int GetInt () const;
	virtual double GetDouble () const;
	virtual unsigned long GetUnsigned () const;
	virtual Boolean GetBoolean () const;
	virtual RGString GetString (unsigned long length) const;

	virtual unsigned long TotalLength () const;
	virtual unsigned long TotalDigits () const;

protected:
	int Value;

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);
};



class RGPDouble : public RGPScalar {

PERSISTENT_DECLARATION(RGPDouble)

public:
	RGPDouble () : RGPScalar (), Value (0.0) {}
	RGPDouble (double val) : RGPScalar (), Value (val) {}
	RGPDouble (const RGPDouble& rhs) : RGPScalar (), Value (rhs.Value) {}
	~RGPDouble ();

	virtual RGPScalar& operator=(const RGPScalar& rhs);
	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual int Compare (const RGPScalar& rhs) const;
	virtual int Compare (int i) const;
	virtual int Compare (double d) const;
	virtual int Compare (unsigned long i) const;
	virtual int Compare (const RGString& str) const;

	virtual int GetInt () const;
	virtual double GetDouble () const;
	virtual unsigned long GetUnsigned () const;
	virtual Boolean GetBoolean () const;
	virtual RGString GetString (unsigned long length) const;

	virtual unsigned long TotalLength () const;
	virtual unsigned long FractionDigits () const;
	virtual unsigned long TotalDigits () const;

protected:
	double Value;

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);
};



class RGPUnsigned : public RGPScalar {

PERSISTENT_DECLARATION(RGPUnsigned)

public:
	RGPUnsigned () : RGPScalar (), Value (0) {}
	RGPUnsigned (unsigned long val) : RGPScalar (), Value (val) {}
	RGPUnsigned (const RGPUnsigned& rhs) : RGPScalar (), Value (rhs.Value) {}
	~RGPUnsigned ();

	virtual RGPScalar& operator=(const RGPScalar& rhs);
	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual int Compare (const RGPScalar& rhs) const;
	virtual int Compare (int i) const;
	virtual int Compare (double d) const;
	virtual int Compare (unsigned long i) const;
	virtual int Compare (const RGString& str) const;

	virtual int GetInt () const;
	virtual double GetDouble () const;
	virtual unsigned long GetUnsigned () const;
	virtual Boolean GetBoolean () const;
	virtual RGString GetString (unsigned long length) const;

	virtual unsigned long TotalLength () const;
	virtual unsigned long TotalDigits () const;

protected:
	unsigned long Value;

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);
};



class RGPBoolean : public RGPScalar {

PERSISTENT_DECLARATION(RGPBoolean)

public:
	RGPBoolean () : RGPScalar (), Value (0) {}

	RGPBoolean (unsigned long val) : RGPScalar () {
		if (val == 0) Value = 0;
		else Value = 1;
	}

	RGPBoolean (const RGPBoolean& rhs) : RGPScalar (), Value (rhs.Value) {}
	~RGPBoolean ();

	virtual RGPScalar& operator=(const RGPScalar& rhs);
	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual int Compare (const RGPScalar& rhs) const;
	virtual int Compare (int i) const;
	virtual int Compare (double d) const;
	virtual int Compare (unsigned long i) const;
	virtual int Compare (const RGString& str) const;

	virtual int GetInt () const;
	virtual double GetDouble () const;
	virtual unsigned long GetUnsigned () const;
	virtual Boolean GetBoolean () const;
	virtual RGString GetString (unsigned long length) const;

protected:
	Boolean Value;

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);
};


class RGPDecimal : public RGPDouble {

PERSISTENT_DECLARATION(RGPDecimal)

public:
	RGPDecimal () : RGPDouble (), TotalDigs (-1), FractionalDigs (-1), Maximum (DOUBLEMAX), Minimum (DOUBLEMIN),
	EMaximum (DOUBLEMAX), EMinimum (DOUBLEMIN) {}
	RGPDecimal (double val) : RGPDouble (val), TotalDigs (-1), FractionalDigs (-1), Maximum (DOUBLEMAX), Minimum (DOUBLEMIN),
	EMaximum (DOUBLEMAX), EMinimum (DOUBLEMIN) {}
	RGPDecimal (const RGPDecimal& rhs) : RGPDouble (rhs), 
		TotalDigs (rhs.TotalDigs), FractionalDigs (rhs.FractionalDigs), Maximum (DOUBLEMAX), Minimum (DOUBLEMIN),
	EMaximum (DOUBLEMAX), EMinimum (DOUBLEMIN) {}
	~RGPDecimal ();

	virtual RGPScalar& operator=(const RGPScalar& rhs);
	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual RGString GetString (unsigned long length) const;

	virtual void SetTotalDigits (int n);
	virtual void SetFractionalDigits (int n);

protected:
	int TotalDigs;
	int FractionalDigs;
	double Maximum;
	double Minimum;
	double EMaximum;
	double EMinimum;

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);

	RGString ConvertDouble (double d) const;
	int RemoveTrailingZeroes (RGString& Work, int decimalPt) const;
};


class RGXMLInt : public RGPInt {

PERSISTENT_DECLARATION(RGXMLInt)

public:
	RGXMLInt () : RGPInt (), Maximum (INTMAX), Minimum (INTMIN) {}
	RGXMLInt (int val) : RGPInt (val), Maximum (INTMAX), Minimum (INTMIN) {}
	RGXMLInt (const RGXMLInt& rhs) : RGPInt (rhs), Maximum (rhs.Maximum), Minimum (rhs.Minimum) {}

	~RGXMLInt ();

	virtual RGPScalar& operator=(const RGPScalar& rhs);
	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual void SetMaxInclusive (const RGPScalar& max);
	virtual void SetMinInclusive (const RGPScalar& min);

	virtual void ResetBounds ();

	virtual void SetMaxExclusive (const RGPScalar& max);
	virtual void SetMinExclusive (const RGPScalar& min);

protected:
	int Maximum;  // These are max and min inclusive; for exclusive, modify max and min by 1 (these are ints
	int Minimum;  // for cryin' out loud!)

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);
};


class RGXMLDouble : public RGPDouble {

PERSISTENT_DECLARATION(RGXMLDouble)

public:
	RGXMLDouble () : RGPDouble (), Maximum (DOUBLEMAX), Minimum (DOUBLEMIN),
	EMaximum (DOUBLEMAX), EMinimum (DOUBLEMIN) {}
	RGXMLDouble (double val) : RGPDouble (val), Maximum (DOUBLEMAX), Minimum (DOUBLEMIN),
	EMaximum (DOUBLEMAX), EMinimum (DOUBLEMIN) {}
	RGXMLDouble (const RGXMLDouble& rhs) : RGPDouble (rhs), Maximum (rhs.Maximum), Minimum (rhs.Minimum),
		EMaximum (rhs.EMaximum), EMinimum (rhs.EMinimum) {}
	~RGXMLDouble ();

	virtual RGPScalar& operator=(const RGPScalar& rhs);
	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual void SetMaxInclusive (const RGPScalar& max);
	virtual void SetMinInclusive (const RGPScalar& min);

	virtual void SetMaxExclusive (const RGPScalar& max);
	virtual void SetMinExclusive (const RGPScalar& min);

	virtual void ResetBounds ();

protected:
	double Maximum;
	double Minimum;
	double EMaximum;
	double EMinimum;

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);
};


class RGXMLUnsigned : public RGPUnsigned {

PERSISTENT_DECLARATION(RGXMLUnsigned)

public:
	RGXMLUnsigned () : RGPUnsigned (), Maximum (ULONGMAX), Minimum (ULONGMIN) {}
	RGXMLUnsigned (unsigned long val) : RGPUnsigned (val), Maximum (ULONGMAX), Minimum (ULONGMIN) {}
	RGXMLUnsigned (const RGXMLUnsigned& rhs) : RGPUnsigned (rhs), Maximum (rhs.Maximum), Minimum (rhs.Minimum) {}
	~RGXMLUnsigned ();

	virtual RGPScalar& operator=(const RGPScalar& rhs);
	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual void SetMaxInclusive (const RGPScalar& max);
	virtual void SetMinInclusive (const RGPScalar& min);

	virtual void SetMaxExclusive (const RGPScalar& max);
	virtual void SetMinExclusive (const RGPScalar& min);

	virtual void ResetBounds ();

protected:
	unsigned long Maximum;  // These will suffice for both inclusive and exclusive bounds
	unsigned long Minimum;

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);
};


class RGXMLStringScalar : public RGPScalar {

PERSISTENT_DECLARATION(RGXMLStringScalar)

public:
	RGXMLStringScalar ();
	RGXMLStringScalar (const RGXMLStringScalar&);
	virtual ~RGXMLStringScalar ();

	virtual RGXMLStringScalar& operator=(const RGXMLStringScalar& rhs);
	RGXMLStringScalar& operator=(int i) { SetTo (i); return *this; }
	RGXMLStringScalar& operator=(double d) { SetTo (d); return *this; }
	RGXMLStringScalar& operator=(unsigned long i) { SetTo (i); return *this; }

	virtual size_t StoreSize () const;
	virtual unsigned HashNumber (unsigned long Base) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual int Compare (const RGPScalar& rhs) const;
	virtual int Compare (int i) const;
	virtual int Compare (double d) const;
	virtual int Compare (unsigned long i) const;
	virtual int Compare (const RGString& str) const;

	virtual void SetValueFrom (const RGPScalar& rhs);
	virtual void SetValueFrom (const RGString& rhs);

	virtual int GetInt () const;
	virtual double GetDouble () const;
	virtual unsigned long GetUnsigned () const;
	virtual Boolean GetBoolean () const;
	virtual RGString GetString (unsigned long length) const;

	virtual unsigned long TotalLength () const;

protected:
	RGString SValue;

	virtual void SetTo (int i);
	virtual void SetTo (double d);
	virtual void SetTo (unsigned long i);
};

#endif  /*  _RGPSCALAR_H_  */
