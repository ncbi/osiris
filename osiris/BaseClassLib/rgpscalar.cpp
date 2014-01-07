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
*  FileName: rgpscalar.cpp
*  Author:   Robert Goor
*
*/
//
// class RGPScalar, a persistent scalar class and subclasses
//

#include <math.h>
#include <stdlib.h>

#include "rgpscalar.h"
#include "rgfile.h"
#include "rgvstream.h"


PERSISTENT_DEFINITION (RGPScalar, _RGPSCALAR_, "Scalar")
PERSISTENT_DEFINITION (RGPInt, _RGPINT_, "integer")
PERSISTENT_DEFINITION (RGPDouble, _RGPDOUBLE_, "double")
PERSISTENT_DEFINITION (RGPUnsigned, _RGPUNSIGNED_, "unsignedLong")
PERSISTENT_DEFINITION (RGPBoolean, _RGPBOOLEAN_, "xs:boolean")
PERSISTENT_DEFINITION (RGPDecimal, _RGPDECIMAL_, "xs:decimal")

PERSISTENT_DEFINITION (RGXMLInt, _RGXMLINT_, "xs:int")
PERSISTENT_DEFINITION (RGXMLDouble, _RGXMLDOUBLE_, "xs:double")
PERSISTENT_DEFINITION (RGXMLUnsigned, _RGXMLUNSIGNED_, "xs:unsignedLong")
PERSISTENT_DEFINITION (RGXMLStringScalar, _RGXMLSTRINGSCALAR_, "XMLStringScalar")

RGPScalar :: RGPScalar () : RGPersistent (), ViolatedBounds (FALSE) {

}


RGPScalar :: RGPScalar (const RGPScalar& s) : RGPersistent (s), ViolatedBounds (FALSE) {

}


RGPScalar :: ~RGPScalar () {

}


Boolean RGPScalar :: operator==(const RGPScalar& rhs) const {

	return (Compare (rhs) == 0);
}


Boolean RGPScalar :: operator<=(const RGPScalar& rhs) const {

	return (Compare (rhs) <= 0);
}


Boolean RGPScalar :: operator<(const RGPScalar& rhs) const {

	return (Compare (rhs) < 0);
}


Boolean RGPScalar :: operator>=(const RGPScalar& rhs) const {

	return (Compare (rhs) >= 0);
}


Boolean RGPScalar :: operator>(const RGPScalar& rhs) const {

	return (Compare (rhs) > 0);
}


Boolean RGPScalar :: operator!=(const RGPScalar& rhs) const {

	return (Compare (rhs) != 0);
}


size_t RGPScalar :: StoreSize () const {

	return RGPersistent::StoreSize ();
}


int RGPScalar :: CompareTo (const RGPersistent* p) const {

	RGPScalar* scalar = (RGPScalar*) p;
	return Compare (*scalar);
}


unsigned RGPScalar :: HashNumber (unsigned long Base) const {

	return GetUnsigned ()%Base;
}


Boolean RGPScalar :: IsEqualTo (const RGPersistent* p) const {

	RGPScalar* scalar = (RGPScalar*) p;
	return (Compare (*scalar) == 0);
}


void RGPScalar :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
}


void RGPScalar :: RestoreAll (RGVInStream& v) {

	RGPersistent::RestoreAll (v);
}


void RGPScalar :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
}


void RGPScalar :: SaveAll (RGVOutStream& v) const {

	RGPersistent::SaveAll (v);
}


int RGPScalar :: Compare (const RGPScalar& rhs) const { return 0; }
int RGPScalar :: Compare (int i) const { return 0; }
int RGPScalar :: Compare (double d) const { return 0; }
int RGPScalar :: Compare (unsigned long i) const { return 0; }
int RGPScalar :: Compare (const RGString& str) const { return -1; }
void RGPScalar :: SetTo (int i) {}
void RGPScalar :: SetTo (double d) {}
void RGPScalar :: SetTo (unsigned long i) {}

int RGPScalar :: GetInt () const { return 0; }
double RGPScalar :: GetDouble () const { return 0.0; }
unsigned long RGPScalar :: GetUnsigned () const { return (unsigned long)0; }
Boolean RGPScalar :: GetBoolean () const { return 0; }
RGString RGPScalar :: GetString (unsigned long length) const { RGString ans; return ans; }

void RGPScalar :: SetValueFrom (const RGPScalar& rhs) {}

void RGPScalar :: SetValueFrom (const RGString& rhs) {}

RGPScalar& RGPScalar :: operator=(const RGPScalar& rhs) {

	 SetValueFrom (rhs);
	 return *this;
}


unsigned long RGPScalar :: TotalLength () const {

	return 1;
}


unsigned long RGPScalar :: FractionDigits () const {

	return 0;
}


unsigned long RGPScalar :: TotalDigits () const {

	return 1;
}


void RGPScalar :: SetTotalDigits (int n) {

}


void RGPScalar :: SetFractionalDigits (int n) {

}


void RGPScalar :: SetMaxInclusive (const RGPScalar& max) {

}


void RGPScalar :: SetMinInclusive (const RGPScalar& min) {

}


void RGPScalar :: SetMaxExclusive (const RGPScalar& max) {

}


void RGPScalar :: SetMinExclusive (const RGPScalar& min) {

}

void RGPScalar :: ResetBounds () {

}


RGPInt :: ~RGPInt () {

}


RGPScalar& RGPInt :: operator=(const RGPScalar& rhs) {

	 SetValueFrom (rhs);
	 return *this;
}


size_t RGPInt :: StoreSize () const {

	return RGPScalar::StoreSize () + sizeof (int);
}


int RGPInt :: CompareTo (const RGPersistent* p) const {

	RGPScalar* q = (RGPScalar*)p;
	return Compare (*q);
}



unsigned RGPInt :: HashNumber (unsigned long Base) const {

	if (Value <= 0)
		return (unsigned)0;

	return Value%Base;
}


Boolean RGPInt :: IsEqualTo (const RGPersistent* p) const {

	RGPScalar* q = (RGPScalar*)p;
	return Value == (int)(*q);
}


void RGPInt :: RestoreAll (RGFile& f) {

	RGPScalar::RestoreAll (f);
	f.Read (Value);
}


void RGPInt :: RestoreAll (RGVInStream& vin) {

	RGPScalar::RestoreAll (vin);
	vin >> Value;
}


void RGPInt :: SaveAll (RGFile& f) const {

	RGPScalar::SaveAll (f);
	f.Write (Value);
}


void RGPInt :: SaveAll (RGVOutStream& vout) const {

	RGPScalar::SaveAll (vout);
	vout << Value;
}


void RGPInt :: SetTo (int i) {

	Value = i;
}


void RGPInt :: SetTo (double d) {

	Value = (int)d;
}


void RGPInt :: SetTo (unsigned long i) {

	Value = i;
}


int RGPInt :: GetInt () const {

	return Value;
}


double RGPInt :: GetDouble () const {

	return (double)Value;
}


unsigned long RGPInt :: GetUnsigned () const {

	if (Value <= 0)
		return (unsigned long) 0;
	
	return (unsigned long)Value;
}


Boolean RGPInt :: GetBoolean () const {

	if (Value == 0)
		return 0;

	return 1;
}


RGString RGPInt :: GetString (unsigned long length) const {

	RGString answer;
	answer.Convert (Value, 10);
	return answer;
}


int RGPInt :: Compare (const RGPScalar& rhs) const {

	return -rhs.Compare (Value);
}


int RGPInt :: Compare (int i) const {

	if (Value < i)
		return -1;

	if (Value > i)
		return 1;

	return 0;
}


int RGPInt :: Compare (double d) const {

	double dValue = (double)Value;

	if (dValue < d)
		return -1;

	if (dValue > d)
		return 1;

	return 0;
}


int RGPInt :: Compare (unsigned long i) const {

	if (Value < 0)
		return -1;

	if ((unsigned long)Value < i)
		return -1;

	if (Value == i)
		return 0;

	return 1;
}


int RGPInt :: Compare (const RGString& str) const  {

	int i = str.ConvertToInteger ();

	if (Value < i)
		return -1;

	if (Value == i)
		return 0;

	return 1;
}


void RGPInt :: SetValueFrom (const RGPScalar& rhs) {

	Value = (int)rhs;
}


void RGPInt :: SetValueFrom (const RGString& rhs) {

	Value = (int)strtol (rhs.GetData (), (char**)NULL, 10);  //atoi (rhs.GetData ());
}


unsigned long RGPInt :: TotalLength () const {

	RGString temp;
	temp.Convert (Value, 10);
	return (unsigned long)temp.Length ();
}


unsigned long RGPInt :: TotalDigits () const {

	unsigned long i = TotalLength ();

	if (Value < 0)
		i--;

	return i;
}


RGPDouble :: ~RGPDouble () {

}


RGPScalar& RGPDouble :: operator=(const RGPScalar& rhs) {

	 SetValueFrom (rhs);
	 return *this;
}


size_t RGPDouble :: StoreSize () const {

	return RGPScalar::StoreSize () + sizeof (double);
}


int RGPDouble :: CompareTo (const RGPersistent* p) const {

	RGPScalar* q = (RGPScalar*)p;
	return Compare (*q);
}



unsigned RGPDouble :: HashNumber (unsigned long Base) const {

	if (Value <= 0.0)
		return (unsigned)0;

	return (unsigned)fmod (Value, Base);
}


Boolean RGPDouble :: IsEqualTo (const RGPersistent* p) const {

	RGPScalar* q = (RGPScalar*)p;
	return Value == (double)(*q);
}


void RGPDouble :: RestoreAll (RGFile& f) {

	RGPScalar::RestoreAll (f);
	f.Read (Value);
}


void RGPDouble :: RestoreAll (RGVInStream& vin) {

	RGPScalar::RestoreAll (vin);
	vin >> Value;
}


void RGPDouble :: SaveAll (RGFile& f) const {

	RGPScalar::SaveAll (f);
	f.Write (Value);
}


void RGPDouble :: SaveAll (RGVOutStream& vout) const {

	RGPScalar::SaveAll (vout);
	vout << Value;
}


void RGPDouble :: SetTo (int i) {

	Value = i;
}


void RGPDouble :: SetTo (double d) {

	Value = d;
}


void RGPDouble :: SetTo (unsigned long i) {

	Value = i;
}


int RGPDouble :: GetInt () const {

	return (int)Value;
}


double RGPDouble :: GetDouble () const {

	return Value;
}


unsigned long RGPDouble :: GetUnsigned () const {

	if (Value <= 0.0)
		return (unsigned long) 0;
	
	return (unsigned long)Value;
}


Boolean RGPDouble :: GetBoolean () const {

	if (Value == 0)
		return 0;

	return 1;
}


RGString RGPDouble :: GetString (unsigned long length) const {

	RGString answer;
	unsigned long ln = length;
	char buffer [30];

	if (length > 25)
		ln = 25;

  gcvt (Value, ln, buffer);
	answer = buffer;
	return answer;
}


int RGPDouble :: Compare (const RGPScalar& rhs) const {

	return -rhs.Compare (Value);
}


int RGPDouble :: Compare (int i) const {

	double j = (double)i;
	
	if (Value < j)
		return -1;

	if (Value > j)
		return 1;

	return 0;
}


int RGPDouble :: Compare (double d) const {

	if (Value < d)
		return -1;

	if (Value > d)
		return 1;

	return 0;
}


int RGPDouble :: Compare (unsigned long i) const {

	double j;
	
	if (Value < 0.0)
		return -1;

	j = (double)i;

	if (Value < j)
		return -1;

	if (Value == j)
		return 0;

	return 1;
}


int RGPDouble :: Compare (const RGString& str) const  {

	double i = str.ConvertToDouble ();

	if (Value < i)
		return -1;

	if (Value > i)
		return 1;

	return 0;
}


void RGPDouble :: SetValueFrom (const RGPScalar& rhs) {

	Value = (double)rhs;
}


void RGPDouble :: SetValueFrom (const RGString& rhs) {

	Value = strtod (rhs.GetData (), NULL);
}


unsigned long RGPDouble :: TotalLength () const {

	char* buffer;
	int exponent;
	double mantissa = frexp(Value, &exponent);
	double frac = exponent * log10 (2.0);
	int intExponent;
	RGString temp;
	intExponent = (int)frac;
	int digits;

	if (intExponent < 0) {

		buffer = new char [30 - intExponent];
		temp = gcvt (Value, 16 - intExponent, buffer);
	}

	else {
	
		buffer = new char [intExponent + 30];

		if (intExponent < 15)
			digits = 16;

		else
			digits = intExponent + 1;

		temp = gcvt (Value, digits, buffer);
	}

	delete[] buffer;	
	return (unsigned long)temp.Length ();
}


unsigned long RGPDouble :: FractionDigits () const {

	char* buffer;
	int exponent;
	double mantissa = frexp(Value, &exponent);
	double frac = exponent * log10 (2.0);
	int intExponent;
	intExponent = (int)frac;
	int digits;

	if (intExponent < 0) {

		buffer = new char [30 - intExponent];
		digits = 16 - intExponent;
	}

	else {
	
		buffer = new char [intExponent + 30];

		if (intExponent < 15)
			digits = 16;

		else
			digits = intExponent + 1;
	}

	double dint;
	double TenToTheFifteen = 1.E+15;
	double dfraction = modf (Value, &dint);
	RGString IntPart;
	RGString FracPart;

	if (Value < 0.0) {

		dfraction = -dfraction;
		dint = -dint;
	}

	IntPart = gcvt (dint, digits, buffer);
	unsigned long IntLn = (unsigned long)IntPart.Length ();

	if ((dint >= TenToTheFifteen) || (IntLn > 15)) {

		delete[] buffer;
		return 0;
	} 

	unsigned long fractionLength = 16 - IntLn;
	FracPart = gcvt (dfraction, fractionLength, buffer);
	unsigned long FracLn = (unsigned long)FracPart.Length ();

	if (FracLn <= 2)
		FracLn = 0;

	else
		FracLn -=2;
	
	delete[] buffer;
	return FracLn;
}


unsigned long RGPDouble :: TotalDigits () const {

	char* buffer;
	int exponent;
	double mantissa = frexp(Value, &exponent);
	double frac = exponent * log10 (2.0);
	int intExponent;
	intExponent = (int)frac;
	int digits;

	if (intExponent < 0) {

		buffer = new char [30 - intExponent];
		digits = 16 - intExponent;
	}

	else {
	
		buffer = new char [intExponent + 30];

		if (intExponent < 15)
			digits = 16;

		else
			digits = intExponent + 1;
	}

	double dint;
	double TenToTheFifteen = 1.E+15;
	double dfraction = modf (Value, &dint);
	RGString IntPart;
	RGString FracPart;

	if (Value < 0.0) {

		dfraction = -dfraction;
		dint = -dint;
	}

	IntPart = gcvt (dint, digits, buffer);
	unsigned long IntLn = (unsigned long)IntPart.Length ();

	if ((dint >= TenToTheFifteen) || (IntLn > 15)) {

		delete[] buffer;
		return IntLn - 1;
	} 

	unsigned long fractionLength = 16 - IntLn;
	FracPart = gcvt (dfraction, fractionLength, buffer);
	unsigned long FracLn = (unsigned long)FracPart.Length ();

	if (FracLn <= 2)
		FracLn = 0;

	else
		FracLn -=2;

	delete[] buffer;
	return IntLn + FracLn - 1;
}



RGPUnsigned :: ~RGPUnsigned () {

}


RGPScalar& RGPUnsigned :: operator=(const RGPScalar& rhs) {

	 SetValueFrom (rhs);
	 return *this;
}


size_t RGPUnsigned :: StoreSize () const {

	return RGPScalar::StoreSize () + sizeof (unsigned long);
}


int RGPUnsigned :: CompareTo (const RGPersistent* p) const {

	RGPScalar* q = (RGPScalar*)p;
	return Compare (*q);
}



unsigned RGPUnsigned :: HashNumber (unsigned long Base) const {

	return Value%Base;
}


Boolean RGPUnsigned :: IsEqualTo (const RGPersistent* p) const {

	RGPScalar* q = (RGPScalar*)p;
	return Value == (unsigned long)(*q);
}


void RGPUnsigned :: RestoreAll (RGFile& f) {

	RGPScalar::RestoreAll (f);
	f.Read (Value);
}


void RGPUnsigned :: RestoreAll (RGVInStream& vin) {

	RGPScalar::RestoreAll (vin);
	vin >> Value;
}


void RGPUnsigned :: SaveAll (RGFile& f) const {

	RGPScalar::SaveAll (f);
	f.Write (Value);
}


void RGPUnsigned :: SaveAll (RGVOutStream& vout) const {

	RGPScalar::SaveAll (vout);
	vout << Value;
}


void RGPUnsigned :: SetTo (int i) {

	if (i <= 0)
		Value = 0;

	else	
		Value = (unsigned long)i;
}


void RGPUnsigned :: SetTo (double d) {

	if (d <= 0.0)
		Value = 0;

	else	
		Value = (unsigned long)d;
}


void RGPUnsigned :: SetTo (unsigned long i) {

	Value = i;
}


int RGPUnsigned :: GetInt () const {

	return (int)Value;
}


double RGPUnsigned :: GetDouble () const {

	return (double)Value;
}


unsigned long RGPUnsigned :: GetUnsigned () const {

	return Value;
}


Boolean RGPUnsigned :: GetBoolean () const {

	if (Value == 0)
		return 0;

	return 1;
}


RGString RGPUnsigned :: GetString (unsigned long length) const {

	RGString answer;
	answer.Convert (Value, 10);
	return answer;
}


int RGPUnsigned :: Compare (const RGPScalar& rhs) const {

	return -rhs.Compare (Value);
}


int RGPUnsigned :: Compare (int i) const {

	unsigned long j;
	
	if (i < 0)
		return 1;

	j = (unsigned long) i;
	
	if (Value < j)
		return -1;

	if (Value > j)
		return 1;

	return 0;
}


int RGPUnsigned :: Compare (double d) const {

	if (d < 0.0)
		return 1;
	
	double dValue = (double)Value;

	if (dValue < d)
		return -1;

	if (dValue > d)
		return 1;

	return 0;
}


int RGPUnsigned :: Compare (unsigned long i) const {

	if (Value < i)
		return -1;

	if (Value == i)
		return 0;

	return 1;
}


int RGPUnsigned :: Compare (const RGString& str) const  {

	unsigned long i = str.ConvertToUnsignedLong ();

	if (Value < i)
		return -1;

	if (Value == i)
		return 0;

	return 1;
}


void RGPUnsigned :: SetValueFrom (const RGPScalar& rhs) {

	Value = (unsigned long)rhs;
}


void RGPUnsigned :: SetValueFrom (const RGString& rhs) {

	int a = strtoul (rhs.GetData (), NULL, 10);

	if (a < 0)
		Value = 0;

	else
		Value = a;

}


unsigned long RGPUnsigned :: TotalLength () const {

	RGString temp;
	temp.Convert (Value, 10);
	return (unsigned long)temp.Length ();
}


unsigned long RGPUnsigned :: TotalDigits () const {

	return TotalLength ();
}



RGPBoolean :: ~RGPBoolean () {

}


RGPScalar& RGPBoolean :: operator=(const RGPScalar& rhs) {

	 SetValueFrom (rhs);
	 return *this;
}


size_t RGPBoolean :: StoreSize () const {

	return RGPScalar::StoreSize () + sizeof (Boolean);
}


int RGPBoolean :: CompareTo (const RGPersistent* p) const {

	RGPScalar* q = (RGPScalar*)p;
	return Compare (*q);
}



unsigned RGPBoolean :: HashNumber (unsigned long Base) const {

	return Value;
}


Boolean RGPBoolean :: IsEqualTo (const RGPersistent* p) const {

	RGPScalar* q = (RGPScalar*)p;
	return Value == (int)(*q);
}


void RGPBoolean :: RestoreAll (RGFile& f) {

	RGPScalar::RestoreAll (f);
	f.Read (Value);
}


void RGPBoolean :: RestoreAll (RGVInStream& vin) {

	RGPScalar::RestoreAll (vin);
	vin >> Value;
}


void RGPBoolean :: SaveAll (RGFile& f) const {

	RGPScalar::SaveAll (f);
	f.Write (Value);
}


void RGPBoolean :: SaveAll (RGVOutStream& vout) const {

	RGPScalar::SaveAll (vout);
	vout << Value;
}


void RGPBoolean :: SetTo (int i) {

	if (i == 0)
		Value = 0;

	else	
		Value = 1;
}


void RGPBoolean :: SetTo (double d) {

	if (d == 0.0)
		Value = 0;

	else	
		Value = 1;
}


void RGPBoolean :: SetTo (unsigned long i) {

	if (i == 0)
		Value = 0;

	else	
		Value = 1;
}


int RGPBoolean :: GetInt () const {

	return Value;
}


double RGPBoolean :: GetDouble () const {

	if (Value)
		return 1.0;

	return 0.0;
}


unsigned long RGPBoolean :: GetUnsigned () const {

	if (Value == 0)
		return 0;

	return 1;
}


Boolean RGPBoolean :: GetBoolean () const {

	return Value;
}


RGString RGPBoolean :: GetString (unsigned long length) const {

	RGString answer;

	if (Value == 0) {
		
		if (length >= 5)
			answer = "FALSE";

		else
			answer = "F";
	}

	else {
		
		if (length >= 4)
			answer = "TRUE";

		else
			answer = "T";
	}

	return answer;
}


int RGPBoolean :: Compare (const RGPScalar& rhs) const {

	return -rhs.Compare (Value);
}


int RGPBoolean :: Compare (int i) const {

	unsigned long j;
	
	if (i < 0)
		return 1;

	j = (unsigned long) i;
	
	if ((unsigned long)Value < j)
		return -1;

	if ((unsigned long)Value > j)
		return 1;

	return 0;
}


int RGPBoolean :: Compare (double d) const {

	if (d < 0.0)
		return 1;
	
	double dValue = (double)Value;

	if (dValue < d)
		return -1;

	if (dValue > d)
		return 1;

	return 0;
}


int RGPBoolean :: Compare (unsigned long i) const {

	if ((unsigned long)Value < i)
		return -1;

	if (Value == i)
		return 0;

	return 1;
}


int RGPBoolean :: Compare (const RGString& str) const  {

	int j = str.ConvertToInteger ();
	Boolean i;

	if (j == 0)
		i = FALSE;

	else
		i = TRUE;

	if (Value < i)
		return -1;

	if (Value == i)
		return 0;

	return 1;
}


void RGPBoolean :: SetValueFrom (const RGPScalar& rhs) {

	Boolean value = (Boolean)rhs;

	if (value == 0)
		Value = 0;

	else
		Value = 1;
}


void RGPBoolean :: SetValueFrom (const RGString& rhs) {

	int a = (int)strtol (rhs.GetData (), (char**)NULL, 10);  //atoi (rhs.GetData ());

	if (a != 0)
		Value = 1;

	else
		Value = 0;
}


RGXMLInt :: ~RGXMLInt () {

}


RGPScalar& RGXMLInt :: operator=(const RGPScalar& rhs) {

	 SetValueFrom (rhs);
	 return *this;
}


size_t RGXMLInt :: StoreSize () const {

	return RGPInt::StoreSize () + 2 * sizeof (int);
}


void RGXMLInt :: RestoreAll (RGFile& f) {

	RGPInt::RestoreAll (f);
	f.Read (Maximum);
	f.Read (Minimum);
}


void RGXMLInt :: RestoreAll (RGVInStream& vin) {

	RGPInt::RestoreAll (vin);
	vin >> Maximum;
	vin >> Minimum;
}


void RGXMLInt :: SaveAll (RGFile& f) const {

	RGPInt::SaveAll (f);
	f.Write (Maximum);
	f.Write (Minimum);
}


void RGXMLInt :: SaveAll (RGVOutStream& vout) const {

	RGPInt::SaveAll (vout);
	vout << Maximum;
	vout << Minimum;
}


void RGXMLInt :: SetTo (int i) {

	if ((i <= Maximum) && (i >= Minimum))
		RGPInt::SetTo (i);

	else
		ViolatedBounds = TRUE;
}


void RGXMLInt :: SetTo (double d) {

	int i = (int)d;
	SetTo (i);
}


void RGXMLInt :: SetTo (unsigned long i) {

	int k = i;
	SetTo (k);
}


void RGXMLInt :: SetValueFrom (const RGPScalar& rhs) {

	int i = (int)rhs;
	SetTo (i);
}


void RGXMLInt :: SetValueFrom (const RGString& rhs) {

	int i = (int)strtol (rhs.GetData (), (char**)NULL, 10);  //atoi (rhs.GetData ());
	SetTo (i);
}


void RGXMLInt :: SetMaxInclusive (const RGPScalar& maximum) {

	int Max = maximum.GetInt ();

	if (Max < Maximum)
		Maximum = Max;
}


void RGXMLInt :: SetMinInclusive (const RGPScalar& min) {

	int Min = min.GetInt ();

	if (Min > Minimum)
		Minimum = Min;
}


void RGXMLInt :: SetMaxExclusive (const RGPScalar& max) {

	int Max = max.GetInt () - 1;

	if (Max < Maximum)
		Maximum = Max;
}


void RGXMLInt :: SetMinExclusive (const RGPScalar& min) {

	int Min = min.GetInt () + 1;

	if (Min > Minimum)
		Minimum = Min;
}


void RGXMLInt :: ResetBounds () {

	Maximum = INTMAX;
	Minimum = INTMIN;
}


RGXMLDouble :: ~RGXMLDouble () {

}


RGPScalar& RGXMLDouble :: operator=(const RGPScalar& rhs) {

	 SetValueFrom (rhs);
	 return *this;
}


size_t RGXMLDouble :: StoreSize () const {

	return RGPDouble::StoreSize () + 4 * sizeof (double);
}


void RGXMLDouble :: RestoreAll (RGFile& f) {

	RGPDouble::RestoreAll (f);
	f.Read (Maximum);
	f.Read (Minimum);
	f.Read (EMaximum);
	f.Read (EMinimum);
}


void RGXMLDouble :: RestoreAll (RGVInStream& f) {

	RGPDouble::RestoreAll (f);
	f >> Maximum;
	f >> Minimum;
	f >> EMaximum;
	f >> EMinimum;
}


void RGXMLDouble :: SaveAll (RGFile& f) const {

	RGPDouble::SaveAll (f);
	f.Write (Maximum);
	f.Write (Minimum);
	f.Write (EMaximum);
	f.Write (EMinimum);
}


void RGXMLDouble :: SaveAll (RGVOutStream& f) const {

	RGPDouble::SaveAll (f);
	f << Maximum;
	f << Minimum;
	f << EMaximum;
	f << EMinimum;
}


void RGXMLDouble :: SetTo (int i) {

	double d = i;
	SetTo (d);
}


void RGXMLDouble :: SetTo (double d) {

	if ((d <= Maximum) && (d >= Minimum) && (d < EMaximum) && (d > EMinimum))
		RGPDouble::SetTo (d);

	else
		ViolatedBounds = TRUE;
}


void RGXMLDouble :: SetTo (unsigned long i) {

	double d = i;
	SetTo (d);
}


void RGXMLDouble :: SetMaxInclusive (const RGPScalar& max) {

	Maximum = max.GetDouble ();
}


void RGXMLDouble :: SetMinInclusive (const RGPScalar& min) {

	Minimum = min.GetDouble ();
}


void RGXMLDouble :: SetMaxExclusive (const RGPScalar& max) {

	EMaximum = max.GetDouble ();
}


void RGXMLDouble :: SetMinExclusive (const RGPScalar& min) {

	EMinimum = min.GetDouble ();
}


void RGXMLDouble :: ResetBounds () {

	Maximum = EMaximum = DOUBLEMAX;
	Minimum = EMinimum = DOUBLEMIN;
}


void RGXMLDouble :: SetValueFrom (const RGPScalar& rhs) {

	double d = (double)rhs;
	SetTo (d);
}


void RGXMLDouble :: SetValueFrom (const RGString& rhs) {

	double d = strtod (rhs.GetData (), NULL);
	SetTo (d);
}


RGXMLUnsigned :: ~RGXMLUnsigned () {

}


RGPScalar& RGXMLUnsigned :: operator=(const RGPScalar& rhs) {

	SetValueFrom (rhs);
	return *this;
}


void RGXMLUnsigned :: SetValueFrom (const RGPScalar& rhs) {

	unsigned long i = (unsigned long)rhs;
	SetTo (i);
}


void RGXMLUnsigned :: SetValueFrom (const RGString& rhs) {

	unsigned long i = strtoul (rhs.GetData (), NULL, 10);
	SetTo (i);
}

size_t RGXMLUnsigned :: StoreSize () const {

	size_t size = RGPUnsigned::StoreSize () + 2 * sizeof (unsigned long);
	return size;
}


void RGXMLUnsigned :: RestoreAll (RGFile& f) {

	RGPUnsigned::RestoreAll (f);
	f.Read (Maximum);
	f.Read (Minimum);
}


void RGXMLUnsigned :: RestoreAll (RGVInStream& f) {

	RGPUnsigned::RestoreAll (f);
	f >> Maximum;
	f >> Minimum;
}


void RGXMLUnsigned :: SaveAll (RGFile& f) const {

	RGPUnsigned::SaveAll (f);
	f.Write (Maximum);
	f.Write (Minimum);
}


void RGXMLUnsigned :: SaveAll (RGVOutStream& f) const {

	RGPUnsigned::SaveAll (f);
	f << Maximum;
	f << Minimum;
}


void RGXMLUnsigned :: SetMaxInclusive (const RGPScalar& max) {

	unsigned long Max = max.GetUnsigned ();

	if (Max < Maximum)
		Maximum = Max;
}


void RGXMLUnsigned :: SetMinInclusive (const RGPScalar& min) {

	unsigned long Min = min.GetUnsigned ();

	if (Min > Minimum)
		Minimum = Min;
}


void RGXMLUnsigned :: SetMaxExclusive (const RGPScalar& max) {

	unsigned long Max = max.GetUnsigned () - 1;

	if (Max < Maximum)
		Maximum = Max;
}


void RGXMLUnsigned :: SetMinExclusive (const RGPScalar& min) {

	unsigned long Min = min.GetUnsigned () + 1;

	if (Min > Minimum)
		Minimum = Min;
}


void RGXMLUnsigned :: ResetBounds () {

	Maximum = ULONGMAX;
	Minimum = ULONGMIN;
}


void RGXMLUnsigned :: SetTo (int i) {

	unsigned long k = i;
	SetTo (k);
}


void RGXMLUnsigned :: SetTo (double d) {

	unsigned long i = (unsigned long)d;
	SetTo (i);
}


void RGXMLUnsigned :: SetTo (unsigned long i) {

	if ((i <= Maximum) && (i >= Minimum))
		RGPUnsigned::SetTo (i);

	else
		ViolatedBounds = TRUE;
}


RGPDecimal :: ~RGPDecimal () {

}

RGPScalar& RGPDecimal :: operator=(const RGPScalar& rhs) {

	RGPDouble::SetValueFrom (rhs);
	return *this;
}


void RGPDecimal :: SetValueFrom (const RGPScalar& rhs) {

	RGPDouble::SetValueFrom (rhs);
}


void RGPDecimal :: SetValueFrom (const RGString& rhs) {

	RGPDouble::SetValueFrom (rhs);
}


void RGPDecimal :: RestoreAll (RGFile& f) {

	RGPDouble::RestoreAll (f);
}


void RGPDecimal :: RestoreAll (RGVInStream& f) {

	RGPDouble::RestoreAll (f);
}


void RGPDecimal :: SaveAll (RGFile& f) const {

	RGPDouble::SaveAll (f);
}


void RGPDecimal :: SaveAll (RGVOutStream& f) const {

	RGPDouble::SaveAll (f);
}


RGString RGPDecimal :: GetString (unsigned long length) const {

	return ConvertDouble (Value);
}


void RGPDecimal :: SetTotalDigits (int n) {

	TotalDigs = n;
}


void RGPDecimal :: SetFractionalDigits (int n) {

	FractionalDigs = n;
}


void RGPDecimal :: SetTo (int i) {

	double d = i;
	SetTo (d);
}


void RGPDecimal :: SetTo (double d) {

	if ((d <= Maximum) && (d >= Minimum) && (d < EMaximum) && (d > EMinimum)) {

		RGPDouble::SetTo (d);
	}

	else
		ViolatedBounds = TRUE;
}


void RGPDecimal :: SetTo (unsigned long i) {

	double d = i;
	SetTo (d);
}


RGString RGPDecimal :: ConvertDouble (double d) const {

	RGString answer;
	RGString Workspace;
	char buffer [400];
	int decPt;
	int sign;
	Boolean isZero = TRUE;
	size_t i;
	
	if (FractionalDigs >= 0) {

		Workspace = fcvt (d, FractionalDigs, &decPt, &sign);  // output string has digits only, no "." or "-"
		int NRemoved = RemoveTrailingZeroes (Workspace, decPt);

		if ((TotalDigs >= 0) && (Workspace.Length () > (size_t)TotalDigs)) {

			int IntegralDigs = (int)Workspace.Length () + NRemoved - FractionalDigs;

			if (IntegralDigs > TotalDigs) {

				return answer;
			}

			Workspace = ecvt (d, TotalDigs, &decPt, &sign);
			RemoveTrailingZeroes (Workspace, decPt);
		}

		for (i=0; i<Workspace.Length(); i++) {

			if (Workspace.GetCharacter (i) != '0') {

				isZero = FALSE;
				break;
			}
		}

		if (isZero) {

			answer = "0.";
			return answer;
		}

		if (sign)
			answer = "-";


		if (decPt > 0)
			answer += Workspace.ExtractAndRemoveSubstring (0, decPt - 1);

		answer += "." + Workspace;
		return answer;		
	}

	if (TotalDigs >= 0) {

		Workspace = ecvt (d, TotalDigs, &decPt, &sign);  // output string has digits only, no "." or "-"
		RemoveTrailingZeroes (Workspace, decPt);

		for (i=0; i<Workspace.Length(); i++) {

			if (Workspace.GetCharacter (i) != '0') {

				isZero = FALSE;
				break;
			}
		}

		if (isZero) {

			answer = "0.";
			return answer;
		}

		if (sign)
			answer = "-";

		if (decPt > 0)
			answer += Workspace.ExtractAndRemoveSubstring (0, decPt - 1);

		answer += "." + Workspace;
		return answer;		
	}

	gcvt (d, 25, buffer);
	answer = buffer;
	return answer;
}


int RGPDecimal :: RemoveTrailingZeroes (RGString& Work, int decimalPt) const {

	// Assumes Work contains only digits, no sign or decimal point
	
	size_t i = Work.Length () - 1;
	int Count = 0;
	int Point = decimalPt;

	if (Point < 0)
		Point = 0;
	
	while (i >= (size_t)decimalPt) {

		if (Work.GetCharacter (i) == '0') {

			Work.ExtractAndRemoveSubstring (i, i);
			Count++;
			i--;
		}

		else
			break;
	}

	return Count;
}


RGXMLStringScalar :: RGXMLStringScalar () : RGPScalar () {

}


RGXMLStringScalar :: RGXMLStringScalar (const RGXMLStringScalar& s) : RGPScalar (s), SValue (s.SValue) {

}


RGXMLStringScalar :: ~RGXMLStringScalar () {

}


RGXMLStringScalar& RGXMLStringScalar :: operator=(const RGXMLStringScalar& rhs) {

	if (this != &rhs)
		SValue = rhs.SValue;

	return *this;
}


size_t RGXMLStringScalar :: StoreSize () const {

	size_t size = RGPScalar::StoreSize () + SValue.StoreSize ();
	return size;
}


unsigned RGXMLStringScalar :: HashNumber (unsigned long Base) const {

	return SValue.HashNumber (Base);
}


void RGXMLStringScalar :: RestoreAll (RGFile& f) {

	RGPScalar::RestoreAll (f);
	SValue.RestoreAll (f);
}


void RGXMLStringScalar :: RestoreAll (RGVInStream& f) {

	RGPScalar::RestoreAll (f);
	SValue.RestoreAll (f);
}


void RGXMLStringScalar :: SaveAll (RGFile& f) const {

	RGPScalar::SaveAll (f);
	SValue.SaveAll (f);
}


void RGXMLStringScalar :: SaveAll (RGVOutStream& f) const {

	RGPScalar::SaveAll (f);
	SValue.SaveAll (f);
}

int RGXMLStringScalar :: Compare (const RGPScalar& rhs) const {

	return -rhs.Compare (SValue);
}


int RGXMLStringScalar :: Compare (int i) const {

	int j = SValue.ConvertToInteger ();

	if (j < i)
		return -1;

	if (j == i)
		return 0;

	return 1;
}


int RGXMLStringScalar :: Compare (double d) const {

	double j = SValue.ConvertToDouble ();

	if (j < d)
		return -1;

	if (j == d)
		return 0;

	return 1;
}


int RGXMLStringScalar :: Compare (unsigned long i) const {

	unsigned long j = SValue.ConvertToUnsignedLong ();

	if (j < i)
		return -1;

	if (j == i)
		return 0;

	return 1;
}


int RGXMLStringScalar :: Compare (const RGString& str) const {

	return SValue.CompareTo (&str);
}


void RGXMLStringScalar :: SetValueFrom (const RGPScalar& rhs) {

	SValue = rhs.GetString (rhs.TotalLength () + 1);
}


void RGXMLStringScalar :: SetValueFrom (const RGString& rhs) {

	SValue = rhs;
}

int RGXMLStringScalar :: GetInt () const {

	return SValue.ConvertToInteger ();
}


double RGXMLStringScalar :: GetDouble () const {

	return SValue.ConvertToDouble ();
}


unsigned long RGXMLStringScalar :: GetUnsigned () const {

	return SValue.ConvertToUnsignedLong ();
}


Boolean RGXMLStringScalar :: GetBoolean () const {

	RGString temp = SValue;
	temp.ToLower ();

	if (temp == "true")
		return TRUE;

	else if (temp == "false")
		return FALSE;

	int i = SValue.ConvertToInteger ();

	if (i == 0)
		return FALSE;

	return TRUE;
}


RGString RGXMLStringScalar :: GetString (unsigned long length) const {

	RGString temp;

	if (length < SValue.Length ()) {

		temp = SValue.ExtractSubstring (0, length - 1);
		return temp;
	}

	return SValue;
}


unsigned long RGXMLStringScalar :: TotalLength () const {

	return (unsigned long)SValue.Length ();
}


void RGXMLStringScalar :: SetTo (int i) {

	SValue.Convert (i, 10);
}


void RGXMLStringScalar :: SetTo (double d) {

	RGPDouble temp (d);
	SValue = temp.GetString (temp.TotalLength () + 4);
}


void RGXMLStringScalar :: SetTo (unsigned long i) {

	SValue.Convert (i, 10);
}

