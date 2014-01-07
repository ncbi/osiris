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
*  FileName: rgxmlrestrict.cpp
*  Author:   Robert Goor
*
*/
//
// classes for XML data restriction and/or enumeration
//

#include <math.h>
#include <stdlib.h>

#include "rgxmlrestrict.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgdlist.h"



PERSISTENT_DEFINITION (RGXmlDataRestriction, _RGXMLDATARESTRICTION_, "DataConstraint")
PERSISTENT_DEFINITION (RGXmlDataEnumeration, _RGXMLDATAENUMERATION_, "EnumerationConstraint")
PERSISTENT_DEFINITION (RGXmlSizeRestriction, _RGXMLSIZERESTRICTION_, "SizeConstraint")
PERSISTENT_DEFINITION (RGXmlDataLength, _RGXMLDATALENGTH_, "DataLengthConstraint")
PERSISTENT_DEFINITION (RGXmlDataFractionDigits, _RGXMLDATAFRACTIONDIGITS_, "FractionDigitsConstraint")
PERSISTENT_DEFINITION (RGXmlDataMaxLength, _RGXMLDATAMAXLENGTH_, "MaxLengthConstraint")
PERSISTENT_DEFINITION (RGXmlDataMinLength, _RGXMLDATAMINLENGTH_, "MinLengthConstraint")
PERSISTENT_DEFINITION (RGXmlDataTotalDigits, _RGXMLDATATOTALDIGITS_, "TotalDigitsConstraint")
PERSISTENT_DEFINITION (RGXmlDataLimit, _RGXMLDATALIMIT_, "DataLimitConstraint")
PERSISTENT_DEFINITION (RGXmlDataMaxInclusive, _RGXMLDATAMAXINCLUSIVE_, "MaxInclusiveConstraint")
PERSISTENT_DEFINITION (RGXmlDataMinInclusive, _RGXMLDATAMININCLUSIVE_, "MinInclusiveConstraint")
PERSISTENT_DEFINITION (RGXmlDataMaxExclusive, _RGXMLDATAMAXEXCLUSIVE_, "MaxExclusiveConstraint")
PERSISTENT_DEFINITION (RGXmlDataMinExclusive, _RGXMLDATAMINEXCLUSIVE_, "MinExclusiveConstraint")


RGXmlDataRestriction :: RGXmlDataRestriction () : RGPersistent () {

}


RGXmlDataRestriction :: RGXmlDataRestriction (const RGXmlDataRestriction& x) : RGPersistent (x) {

}


RGXmlDataRestriction :: ~RGXmlDataRestriction () {}


Boolean RGXmlDataRestriction :: Test (const RGPScalar& p) const {

	return FALSE;
}


Boolean RGXmlDataRestriction :: Test (const RGPScalar* p) const {

	return FALSE;
}


Boolean RGXmlDataRestriction :: Test (const RGDList& p) const {

	return FALSE;
}


Boolean RGXmlDataRestriction :: Test (const RGDList* p) const {

	return FALSE;
}


void RGXmlDataRestriction :: SetLimit (const RGPScalar& l) {

}


RGPScalar RGXmlDataRestriction :: GetLimit () const {

	RGPBoolean Ans (0);
	return Ans;
}

Boolean RGXmlDataRestriction :: SetStringValue (RGString* str) {

	return FALSE;
}


RGString RGXmlDataRestriction :: GetStringValue () const {

	return RGString ();
}


RGString RGXmlDataRestriction :: BuildString (const RGPScalar& p, int& param1, int& param2) {

	RGString str;
	return str;
}


size_t RGXmlDataRestriction :: StoreSize () const {

	return RGPersistent::StoreSize ();
}


int RGXmlDataRestriction :: CompareTo (const RGPersistent* p) const {

	return -1;
}


unsigned RGXmlDataRestriction :: HashNumber (unsigned long Base) const {

	return 0;
}


Boolean RGXmlDataRestriction :: IsEqualTo (const RGPersistent* p) const {

	return FALSE;
}

void RGXmlDataRestriction :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
}


void RGXmlDataRestriction :: RestoreAll (RGVInStream& v) {

	RGPersistent::RestoreAll (v);
}


void RGXmlDataRestriction :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
}


void RGXmlDataRestriction :: SaveAll (RGVOutStream& v) const {

	RGPersistent::SaveAll (v);
}


RGXmlDataEnumeration :: RGXmlDataEnumeration () : RGPersistent (), Array (NULL), Size (0) {

	ValueIterator = new RGDListIterator (PossibleValues);
}


RGXmlDataEnumeration :: RGXmlDataEnumeration (const RGXmlDataEnumeration& x) : RGPersistent (x), 
PossibleValues (x.PossibleValues), Array (NULL), Size (x.Size) {

	ValueIterator = new RGDListIterator (PossibleValues);
}


RGXmlDataEnumeration :: ~RGXmlDataEnumeration () {

	delete ValueIterator;
	delete[] Array;
	PossibleValues.ClearAndDelete ();
}


int RGXmlDataEnumeration :: Initialize (const RGString& base) {

	//
	//  Provide another method to look for the base type and then
	//  change the list of strings into the array of scalars of whatever 
	//  type the base is.
	//
	
	if (Array != NULL)
		return 0;
	
	Size = PossibleValues.Entries ();

	if (Size == 0)
		return -1;

	RGPScalar* BaseDataType = (RGPScalar*) RGWarehouse::GlobalFindXMLType (base);

	if (BaseDataType == NULL)
		return -1;

	Array = new RGPScalar* [Size];
	ValueIterator->Reset ();
	RGString* NextValue;
	int i = 0;

	while (NextValue = (RGString*)(*ValueIterator)()) {
	
		Array [i] = (RGPScalar*)BaseDataType->Copy ();
		Array [i]->SetValueFrom (*NextValue);
		i++;
	}

	return 0;
}


int RGXmlDataEnumeration :: Test (const RGPScalar& p) const {

	for (unsigned long i=0; i<Size; i++) {

		if (Array [i]->Compare (p) == 0)
				return i;
		}

	return -1;
}


int RGXmlDataEnumeration :: Test (const RGPScalar* p) const {

	for (unsigned long i=0; i<Size; i++) {

		if (Array [i]->Compare (*p) == 0)
			return i;
	}

	return -1;
}


int RGXmlDataEnumeration :: AddValue (RGPScalar* p) {

	PossibleValues.Append (p);
	return 0;
}


int RGXmlDataEnumeration :: AddValue (RGString* p) {

	PossibleValues.Append (p);
	return 0;
}


RGString RGXmlDataEnumeration :: GetStringValue (unsigned long index) {

	if ((Size == 0) || (index >= Size))
		return RGString ("");

	RGPScalar* p = Array [index];
	int Length = p->TotalLength ();
	return p->GetStringValue (Length);
}


RGPScalar RGXmlDataEnumeration :: GetScalarValue (unsigned long index) {

	if ((Size == 0) || (index >= Size))
		return RGPScalar ();

	return *Array [index];
}


size_t RGXmlDataEnumeration :: StoreSize () const {

	unsigned long size = RGPersistent::StoreSize ();
	size += PossibleValues.StoreSize ();
	return size;
}


void RGXmlDataEnumeration :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	PossibleValues.RestoreAll (f);
}


void RGXmlDataEnumeration :: RestoreAll (RGVInStream& v) {

	RGPersistent::RestoreAll (v);
	PossibleValues.RestoreAll (v);
}


void RGXmlDataEnumeration :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	PossibleValues.SaveAll (f);
}


void RGXmlDataEnumeration :: SaveAll (RGVOutStream& v) const {

	RGPersistent::SaveAll (v);
	PossibleValues.SaveAll (v);
}


RGXmlSizeRestriction :: RGXmlSizeRestriction () : RGXmlDataRestriction (), SizeLimit (0) {

}


RGXmlSizeRestriction :: RGXmlSizeRestriction (const RGXmlSizeRestriction& s) : RGXmlDataRestriction (s),
SizeLimit (s.SizeLimit) {

}


RGXmlSizeRestriction :: ~RGXmlSizeRestriction () {

}


void RGXmlSizeRestriction :: SetLimit (const RGPScalar& p) {

	int temp = (int)p;

	if (temp < 0)
		SizeLimit = 0;

	else
		SizeLimit = temp;
}


RGPScalar RGXmlSizeRestriction :: GetLimit () const {

	RGPUnsigned Ans (SizeLimit);
	return Ans;
}


size_t RGXmlSizeRestriction :: StoreSize () const {

	unsigned long size = RGXmlDataRestriction::StoreSize ();
	size += sizeof (unsigned long);
	return size;
}


int RGXmlSizeRestriction :: CompareTo (const RGPersistent* p) const {

	RGXmlSizeRestriction* q = (RGXmlSizeRestriction*) p;

	if (SizeLimit < q->SizeLimit)
		return -1;

	if (SizeLimit == q->SizeLimit)
		return 0;

	return 1;
}


unsigned RGXmlSizeRestriction :: HashNumber (unsigned long Base) const {

	return SizeLimit%Base;
}


Boolean RGXmlSizeRestriction :: IsEqualTo (const RGPersistent* p) const {

	return (CompareTo (p) == 0);
}

void RGXmlSizeRestriction :: RestoreAll (RGFile& f) {

	RGXmlDataRestriction::RestoreAll (f);
	f.Read (SizeLimit);
}


void RGXmlSizeRestriction :: RestoreAll (RGVInStream& v) {

	RGXmlDataRestriction::RestoreAll (v);
	v >> SizeLimit;
}


void RGXmlSizeRestriction :: SaveAll (RGFile& f) const {

	RGXmlDataRestriction::SaveAll (f);
	f.Write (SizeLimit);
}


void RGXmlSizeRestriction :: SaveAll (RGVOutStream& v) const {

	RGXmlDataRestriction::SaveAll (v);
	v << SizeLimit;
}


RGXmlDataLength :: RGXmlDataLength () : RGXmlSizeRestriction () {

}


RGXmlDataLength :: RGXmlDataLength (const RGXmlDataLength& x) : RGXmlSizeRestriction (x) {

}


RGXmlDataLength :: ~RGXmlDataLength () {

}


Boolean RGXmlDataLength :: Test (const RGPScalar& x) const {

	if (x.TotalLength () == SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataLength :: Test (const RGPScalar* x) const {

	if (x->TotalLength () == SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataLength :: Test (const RGDList& x) const {

	if (x.Entries () == SizeLimit)
				return TRUE;

	return FALSE;
}


Boolean RGXmlDataLength :: Test (const RGDList* x) const {

	if (x->Entries () == SizeLimit)
				return TRUE;

	return FALSE;
}


RGXmlDataFractionDigits :: RGXmlDataFractionDigits () : RGXmlSizeRestriction () {

}


RGXmlDataFractionDigits :: RGXmlDataFractionDigits (const RGXmlDataFractionDigits& x) : 
RGXmlSizeRestriction (x) {

}


RGXmlDataFractionDigits :: ~RGXmlDataFractionDigits () {

}


Boolean RGXmlDataFractionDigits :: Test (const RGPScalar& x) const {

	if (x.FractionDigits () <= SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataFractionDigits :: Test (const RGPScalar* x) const {

	if (x->FractionDigits () <= SizeLimit)
		return TRUE;

	return FALSE;
}

RGString RGXmlDataFractionDigits :: BuildString (const RGPScalar& p, int& param1, int& param2) {

	int fractionalDigits = SizeLimit;
	char* str;
	double d = p.GetDouble ();
	int decimalPoint;
	int sign;
	str = fcvt (d, fractionalDigits, &decimalPoint, &sign);
	RGString Result = str;
	param1 = decimalPoint;
	param2 = sign;
	return Result;
}


RGXmlDataMaxLength :: RGXmlDataMaxLength () : RGXmlSizeRestriction () {

}


RGXmlDataMaxLength :: RGXmlDataMaxLength (const RGXmlDataMaxLength& x) : RGXmlSizeRestriction (x) {

}


RGXmlDataMaxLength :: ~RGXmlDataMaxLength () {

}


Boolean RGXmlDataMaxLength :: Test (const RGPScalar& x) const {

	if (x.TotalLength () <= SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataMaxLength :: Test (const RGPScalar* x) const {

	if (x->TotalLength () <= SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataMaxLength :: Test (const RGDList& x) const {

	if (x.Entries () <= SizeLimit)
				return TRUE;

	return FALSE;
}


Boolean RGXmlDataMaxLength :: Test (const RGDList* x) const {

	if (x->Entries () <= SizeLimit)
				return TRUE;

	return FALSE;
}



RGXmlDataMinLength :: RGXmlDataMinLength () : RGXmlSizeRestriction () {

}


RGXmlDataMinLength :: RGXmlDataMinLength (const RGXmlDataMinLength& x) : RGXmlSizeRestriction (x) {

}


RGXmlDataMinLength :: ~RGXmlDataMinLength () {

}


Boolean RGXmlDataMinLength :: Test (const RGPScalar& x) const {

	if (x.TotalLength () >= SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataMinLength :: Test (const RGPScalar* x) const {

	if (x->TotalLength () >= SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataMinLength :: Test (const RGDList& x) const {

	if (x.Entries () >= SizeLimit)
				return TRUE;

	return FALSE;
}


Boolean RGXmlDataMinLength :: Test (const RGDList* x) const {

	if (x->Entries () >= SizeLimit)
				return TRUE;

	return FALSE;
}


RGXmlDataTotalDigits :: RGXmlDataTotalDigits () : RGXmlSizeRestriction () {

}


RGXmlDataTotalDigits :: RGXmlDataTotalDigits (const RGXmlDataTotalDigits& x) : 
RGXmlSizeRestriction (x) {

}


RGXmlDataTotalDigits :: ~RGXmlDataTotalDigits () {

}


Boolean RGXmlDataTotalDigits :: Test (const RGPScalar& x) const {

	if (x.TotalDigits () <= SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataTotalDigits :: Test (const RGPScalar* x) const {

	if (x->TotalDigits () <= SizeLimit)
		return TRUE;

	return FALSE;
}

RGString RGXmlDataTotalDigits :: BuildString (const RGPScalar& p, int& param1, int& param2) {

	int totalDigits = SizeLimit;
	char* str;
	double d = p.GetDouble ();
	int decimalPoint;
	int sign;
	str = ecvt (d, totalDigits, &decimalPoint, &sign);
	RGString Result = str;
	param1 = decimalPoint;
	param2 = sign;
	return Result;
}


RGXmlDataLimit :: RGXmlDataLimit () : RGXmlDataRestriction (), SizeLimit (NULL), StringSizeLimit (NULL) {

}


RGXmlDataLimit :: RGXmlDataLimit (const RGXmlDataLimit& dl) : RGXmlDataRestriction (dl) {
	
	if (dl.SizeLimit != NULL)
		SizeLimit = (RGPScalar*)dl.SizeLimit->Copy ();

	else
		SizeLimit = NULL;

	if (dl.StringSizeLimit != NULL)
		StringSizeLimit = new RGString (*dl.StringSizeLimit);

	else
		StringSizeLimit = NULL;
}


RGXmlDataLimit :: ~RGXmlDataLimit () {

	delete SizeLimit;
	delete StringSizeLimit;
}


int RGXmlDataLimit :: Initialize (const RGString& base) {

	RGPScalar* BaseDataType = (RGPScalar*) RGWarehouse::GlobalFindXMLType (base);

	if (BaseDataType == NULL)
		return -1;

	delete SizeLimit;

	SizeLimit = (RGPScalar*)BaseDataType->Copy ();

	if (StringSizeLimit != NULL)
		SizeLimit->SetValueFrom (*StringSizeLimit);

	else
		*SizeLimit = (int)0;

	return 0;
}


Boolean RGXmlDataLimit :: Test (const RGPScalar& x) const {

	return FALSE;
}


Boolean RGXmlDataLimit :: Test (const RGPScalar* x) const {

	return FALSE;
}


void RGXmlDataLimit :: SetLimit (const RGPScalar& s) {

	delete SizeLimit;
	SizeLimit = (RGPScalar*)s.Copy ();
}


RGPScalar RGXmlDataLimit :: GetLimit () const {

	if (SizeLimit == NULL)
		return RGPScalar ();

	return *SizeLimit;
}

Boolean RGXmlDataLimit :: SetStringValue (RGString* str) {

	delete StringSizeLimit;
	StringSizeLimit = str;
	return TRUE;
}


RGString RGXmlDataLimit :: GetStringValue () const {

	if (StringSizeLimit != NULL)
		return *StringSizeLimit;

	if (SizeLimit != NULL)
		return SizeLimit->GetString (SizeLimit->TotalLength ());

	return RGString ();
}

size_t RGXmlDataLimit :: StoreSize () const {

	size_t size = RGXmlDataRestriction::StoreSize ();
	size += StringSizeLimit->StoreSize ();
	return size;
}


int RGXmlDataLimit :: CompareTo (const RGPersistent* x) const {

	if (SizeLimit != NULL)
		return SizeLimit->CompareTo (x);

	return -1;
}


unsigned RGXmlDataLimit :: HashNumber (unsigned long Base) const {

	if (SizeLimit != NULL)
		return SizeLimit->HashNumber (Base);

	else if (StringSizeLimit != NULL)
		return StringSizeLimit->HashNumber (Base);

	return 0;
}


Boolean RGXmlDataLimit :: IsEqualTo (const RGPersistent* x) const {

	if (SizeLimit != NULL)
		return SizeLimit->IsEqualTo (x);

	else if (StringSizeLimit != NULL)
		return StringSizeLimit->IsEqualTo (x);

	return FALSE;
}

void RGXmlDataLimit :: RestoreAll (RGFile& f) {

	RGXmlDataRestriction::RestoreAll (f);
	StringSizeLimit->RestoreAll (f);
}


void RGXmlDataLimit :: RestoreAll (RGVInStream& v) {

	RGXmlDataRestriction::RestoreAll (v);
	StringSizeLimit->RestoreAll (v);
}


void RGXmlDataLimit :: SaveAll (RGFile& f) const {

	RGXmlDataRestriction::SaveAll (f);
	StringSizeLimit->SaveAll (f);
}


void RGXmlDataLimit :: SaveAll (RGVOutStream& v) const {

	RGXmlDataRestriction::SaveAll (v);
	StringSizeLimit->SaveAll (v);
}


RGXmlDataMaxInclusive :: RGXmlDataMaxInclusive () : RGXmlDataLimit () {

}


RGXmlDataMaxInclusive :: RGXmlDataMaxInclusive (const RGXmlDataMaxInclusive& x) : RGXmlDataLimit (x) {

}


RGXmlDataMaxInclusive :: ~RGXmlDataMaxInclusive () {

}


Boolean RGXmlDataMaxInclusive :: Test (const RGPScalar& x) const {

	if (SizeLimit == NULL)
		return FALSE;
	
	if (x <= *SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataMaxInclusive :: Test (const RGPScalar* x) const {

	if (SizeLimit == NULL)
		return FALSE;
	
	if (*x <= *SizeLimit)
		return TRUE;

	return FALSE;
}


RGXmlDataMinInclusive :: RGXmlDataMinInclusive () : RGXmlDataLimit () {

}


RGXmlDataMinInclusive :: RGXmlDataMinInclusive (const RGXmlDataMinInclusive& x) : RGXmlDataLimit (x) {

}


RGXmlDataMinInclusive :: ~RGXmlDataMinInclusive () {

}


Boolean RGXmlDataMinInclusive :: Test (const RGPScalar& x) const {

	if (SizeLimit == NULL)
		return FALSE;
	
	if (x >= *SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataMinInclusive :: Test (const RGPScalar* x) const {

	if (SizeLimit == NULL)
		return FALSE;
	
	if (*x >= *SizeLimit)
		return TRUE;

	return FALSE;
}


RGXmlDataMaxExclusive :: RGXmlDataMaxExclusive () : RGXmlDataLimit () {

}


RGXmlDataMaxExclusive :: RGXmlDataMaxExclusive (const RGXmlDataMaxExclusive& x) : RGXmlDataLimit (x) {

}


RGXmlDataMaxExclusive :: ~RGXmlDataMaxExclusive () {

}


Boolean RGXmlDataMaxExclusive :: Test (const RGPScalar& x) const {

	if (SizeLimit == NULL)
		return FALSE;
	
	if (x < *SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataMaxExclusive :: Test (const RGPScalar* x) const {

	if (SizeLimit == NULL)
		return FALSE;
	
	if (*x < *SizeLimit)
		return TRUE;

	return FALSE;
}


RGXmlDataMinExclusive :: RGXmlDataMinExclusive () : RGXmlDataLimit () {

}


RGXmlDataMinExclusive :: RGXmlDataMinExclusive (const RGXmlDataMinExclusive& x) : RGXmlDataLimit (x) {

}


RGXmlDataMinExclusive :: ~RGXmlDataMinExclusive () {

}


Boolean RGXmlDataMinExclusive :: Test (const RGPScalar& x) const {

	if (SizeLimit == NULL)
		return FALSE;
	
	if (x > *SizeLimit)
		return TRUE;

	return FALSE;
}


Boolean RGXmlDataMinExclusive :: Test (const RGPScalar* x) const {

	if (SizeLimit == NULL)
		return FALSE;
	
	if (*x > *SizeLimit)
		return TRUE;

	return FALSE;
}
