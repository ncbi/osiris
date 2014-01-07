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
*  FileName: rgxmlrestrict.h
*  Author:   Robert Goor
*
*/
//
//  XML Restriction classes, including base class and enumeration class
//

#ifndef _RGXMLDATARESTRICTION_H_
#define _RGXMLDATARESTRICTION_H_

#include "rgpersist.h"
#include "rgdlist.h"
#include "rgpscalar.h"

class RGFile;
class RGVInStream;
class RGVOutStream;


const int _RGXMLDATARESTRICTION_ = 15;
const int _RGXMLDATAENUMERATION_ = 16;
const int _RGXMLSIZERESTRICTION_ = 19;
const int _RGXMLDATALENGTH_ = 20;
const int _RGXMLDATAFRACTIONDIGITS_ = 21;
const int _RGXMLDATAMAXLENGTH_ = 22;
const int _RGXMLDATAMINLENGTH_ = 23;
const int _RGXMLDATATOTALDIGITS_ = 24;
const int _RGXMLDATALIMIT_ = 25;
const int _RGXMLDATAMAXINCLUSIVE_ = 26;
const int _RGXMLDATAMININCLUSIVE_ = 27;
const int _RGXMLDATAMAXEXCLUSIVE_ = 28;
const int _RGXMLDATAMINEXCLUSIVE_ = 29;

//  Add RGXMLCollapseSpaces!



PERSISTENT_PREDECLARATION (RGXmlDataRestriction)
PERSISTENT_PREDECLARATION (RGXmlDataEnumeration)
PERSISTENT_PREDECLARATION (RGXmlSizeRestriction)
PERSISTENT_PREDECLARATION (RGXmlDataLength)
PERSISTENT_PREDECLARATION (RGXmlDataFractionDigits)
PERSISTENT_PREDECLARATION (RGXmlDataMaxLength)
PERSISTENT_PREDECLARATION (RGXmlDataMinLength)
PERSISTENT_PREDECLARATION (RGXmlDataTotalDigits)
PERSISTENT_PREDECLARATION (RGXmlDataLimit)
PERSISTENT_PREDECLARATION (RGXmlDataMaxInclusive)
PERSISTENT_PREDECLARATION (RGXmlDataMinInclusive)
PERSISTENT_PREDECLARATION (RGXmlDataMaxExclusive)
PERSISTENT_PREDECLARATION (RGXmlDataMinExclusive)


class RGXmlDataRestriction : public RGPersistent {

	PERSISTENT_DECLARATION(RGXmlDataRestriction)

public:
	RGXmlDataRestriction ();
	RGXmlDataRestriction (const RGXmlDataRestriction&);
	~RGXmlDataRestriction ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;
	virtual Boolean Test (const RGDList&) const;
	virtual Boolean Test (const RGDList*) const;

	virtual void SetLimit (const RGPScalar&);
	virtual RGPScalar GetLimit () const;
	virtual Boolean SetStringValue (RGString* str);
	virtual RGString GetStringValue () const;

	virtual RGString BuildString (const RGPScalar& p, int& param1, int& param2);

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;
};



class RGXmlDataEnumeration : public RGPersistent {

	PERSISTENT_DECLARATION(RGXmlDataEnumeration)

public:
	RGXmlDataEnumeration ();
	RGXmlDataEnumeration (const RGXmlDataEnumeration&);
	~RGXmlDataEnumeration ();

	int Initialize (const RGString& base);
	int Test (const RGPScalar&) const;
	int Test (const RGPScalar*) const;
	unsigned long NumberOfItems () const { return PossibleValues.Entries (); }

	int AddValue (RGPScalar*);
	int AddValue (RGString*);
	RGString GetStringValue (unsigned long index);
	RGPScalar GetScalarValue (unsigned long index);

	virtual size_t StoreSize () const;
	
	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	RGDList PossibleValues;
	RGDListIterator* ValueIterator;
	RGPScalar** Array;
	unsigned long Size;
};


class RGXmlSizeRestriction : public RGXmlDataRestriction {

	PERSISTENT_DECLARATION(RGXmlSizeRestriction)

public:
	RGXmlSizeRestriction ();
	RGXmlSizeRestriction (const RGXmlSizeRestriction&);
	~RGXmlSizeRestriction ();

	virtual void SetLimit (const RGPScalar&);
	virtual RGPScalar GetLimit () const;

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	unsigned long SizeLimit;
};



class RGXmlDataLength : public RGXmlSizeRestriction {

	PERSISTENT_DECLARATION(RGXmlDataLength)

public:
	RGXmlDataLength ();
	RGXmlDataLength (const RGXmlDataLength&);
	~RGXmlDataLength ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;
	virtual Boolean Test (const RGDList&) const;
	virtual Boolean Test (const RGDList*) const;
};


class RGXmlDataFractionDigits : public RGXmlSizeRestriction {

	PERSISTENT_DECLARATION(RGXmlDataFractionDigits)

public:
	RGXmlDataFractionDigits ();
	RGXmlDataFractionDigits (const RGXmlDataFractionDigits&);
	~RGXmlDataFractionDigits ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;

	virtual RGString BuildString (const RGPScalar& p, int& param1, int& param2);
};


class RGXmlDataMaxLength : public RGXmlSizeRestriction {

	PERSISTENT_DECLARATION(RGXmlDataMaxLength)

public:
	RGXmlDataMaxLength ();
	RGXmlDataMaxLength (const RGXmlDataMaxLength&);
	~RGXmlDataMaxLength ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;
	virtual Boolean Test (const RGDList&) const;
	virtual Boolean Test (const RGDList*) const;
};


class RGXmlDataMinLength : public RGXmlSizeRestriction {

	PERSISTENT_DECLARATION(RGXmlDataMinLength)

public:
	RGXmlDataMinLength ();
	RGXmlDataMinLength (const RGXmlDataMinLength&);
	~RGXmlDataMinLength ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;
	virtual Boolean Test (const RGDList&) const;
	virtual Boolean Test (const RGDList*) const;
};


class RGXmlDataTotalDigits : public RGXmlSizeRestriction {

	PERSISTENT_DECLARATION(RGXmlDataTotalDigits)

public:
	RGXmlDataTotalDigits ();
	RGXmlDataTotalDigits (const RGXmlDataTotalDigits&);
	~RGXmlDataTotalDigits ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;

	virtual RGString BuildString (const RGPScalar& p, int& param1, int& param2);
};


class RGXmlDataLimit : public RGXmlDataRestriction {

	PERSISTENT_DECLARATION(RGXmlDataLimit)

public:
	RGXmlDataLimit ();
	RGXmlDataLimit (const RGXmlDataLimit&);
	~RGXmlDataLimit ();

	int Initialize (const RGString& base);

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;

	virtual void SetLimit (const RGPScalar&);
	virtual RGPScalar GetLimit () const;
	virtual Boolean SetStringValue (RGString* str);
	virtual RGString GetStringValue () const;

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	RGPScalar* SizeLimit;
	RGString* StringSizeLimit;
};


class RGXmlDataMaxInclusive : public RGXmlDataLimit {

	PERSISTENT_DECLARATION(RGXmlDataMaxInclusive)

public:
	RGXmlDataMaxInclusive ();
	RGXmlDataMaxInclusive (const RGXmlDataMaxInclusive&);
	~RGXmlDataMaxInclusive ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;
};


class RGXmlDataMinInclusive : public RGXmlDataLimit {

	PERSISTENT_DECLARATION(RGXmlDataMinInclusive)

public:
	RGXmlDataMinInclusive ();
	RGXmlDataMinInclusive (const RGXmlDataMinInclusive&);
	~RGXmlDataMinInclusive ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;
};


class RGXmlDataMaxExclusive : public RGXmlDataLimit {

	PERSISTENT_DECLARATION(RGXmlDataMaxExclusive)

public:
	RGXmlDataMaxExclusive ();
	RGXmlDataMaxExclusive (const RGXmlDataMaxExclusive&);
	~RGXmlDataMaxExclusive ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;
};


class RGXmlDataMinExclusive : public RGXmlDataLimit {

	PERSISTENT_DECLARATION(RGXmlDataMinExclusive)

public:
	RGXmlDataMinExclusive ();
	RGXmlDataMinExclusive (const RGXmlDataMinExclusive&);
	~RGXmlDataMinExclusive ();

	virtual Boolean Test (const RGPScalar&) const;
	virtual Boolean Test (const RGPScalar*) const;
};



#endif  /*  _RGXMLDATARESTRICTION_H_  */
