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
*  FileName: rgpersist.h
*  Author:   Robert Goor
*
*/
//
//	class RGPersistent - base class for all persistent class objects
//


#ifndef _RGPERSISTENT_H_
#define _RGPERSISTENT_H_

#include "rgdefs.h"
#include "rgwarehouse.h"
#include "rgsimplestring.h"

class RGDList;
class RGDListIterator;
class RGVInStream;
class RGVOutStream;
class RGFile;

const int _RGPERSIST_ = 1;

#define CreateNew(className) CreateNew##className
#define forceLoad(className) forceLoad##className
#define __RGDUMMY(className) __RGDUMMY##className

#define PERSISTENT_PREDECLARATION(ClassName)              \
extern int forceLoad(ClassName)();                        \
const int __RGDUMMY(ClassName) = forceLoad(ClassName)();  \



#define PERSISTENT_DECLARATION(ClassName)                   \
private:                                                    \
    static int myID;                                        \
	static RGSimpleString myXMLType;						\
    static int IDAdded;                                     \
    static RGPersistent* Prototype;                         \
    static RGPersistent* CreateNew(ClassName)();            \
public:                                                     \
    virtual RGPersistent* GetNew() const;                   \
    virtual RGPersistent* Copy () const;                    \
    virtual int GetID () const;                             \
	virtual const char* GetXMLType () const;				\
    static int GetClassID ();                               \
	static const char* GetClassXMLType ();					\
    friend RGVInStream&                                     \
        operator>>(RGVInStream& vin, ClassName*& ClPtr);    \
    friend RGFile&                                          \
        operator>>(RGFile& f, ClassName*& ClPtr);           \
    friend RGVOutStream&                                            \
    operator<<(RGVOutStream& vout, const ClassName& Cls) {          \
    Cls.SaveAll (vout); return vout; }                              \
    friend RGFile&                                                  \
    operator<<(RGFile& f, const ClassName& Cls) {                   \
    Cls.SaveAll (f); return f; }                                    \
    friend RGVInStream&                                             \
    operator>>(RGVInStream& vin, ClassName& Cls) {                  \
    Cls.RestoreAll (vin); return vin; }                             \
    friend RGFile&                                                  \
    operator>>(RGFile& f, ClassName& Cls) {                          \
    Cls.RestoreAll (f); return f; }                                 \


#define ABSTRACT_DECLARATION(ClassName)                   \
public:                                                     \
    friend RGVInStream&                                     \
        operator>>(RGVInStream& vin, ClassName*& ClPtr);    \
    friend RGFile&                                          \
        operator>>(RGFile& f, ClassName*& ClPtr);           \
    friend RGVOutStream&                                            \
    operator<<(RGVOutStream& vout, const ClassName& Cls) {          \
    Cls.SaveAll (vout); return vout; }                              \
    friend RGFile&                                                  \
    operator<<(RGFile& f, const ClassName& Cls) {                   \
    Cls.SaveAll (f); return f; }                                    \
    friend RGVInStream&                                             \
    operator>>(RGVInStream& vin, ClassName& Cls) {                  \
    Cls.RestoreAll (vin); return vin; }                             \
    friend RGFile&                                                  \
    operator>>(RGFile& f, ClassName& Cls) {                          \
    Cls.RestoreAll (f); return f; }                                 \



#define PERSISTENT_DEFINITION(ClassName,id,type)				\
int forceLoad(ClassName)() { return id; }						\
int ClassName::myID = id;                      					\
RGSimpleString ClassName::myXMLType ((const char*)type);		\
RGPersistent* ClassName::GetNew() const                			\
    			{ return new ClassName; }						\
RGPersistent* ClassName::Copy() const							\
    			{ return new ClassName(*this); }				\
int ClassName::GetID() const { return ClassName::myID; }        \
int ClassName::GetClassID() {return myID;}            			\
																\
const char* ClassName::GetXMLType () const {					\
	return ClassName::myXMLType.GetData();						\
}																\
																\
const char* ClassName::GetClassXMLType () {						\
	return myXMLType.GetData();									\
}																\
                                                                \
RGPersistent* ClassName::CreateNew(ClassName)()					\
{ RGWarehouse* NewWarehouse = new RGWarehouse;                  \
return new ClassName; }                                         \
                                                                \
RGPersistent* ClassName::Prototype =							\
       ClassName::CreateNew(ClassName)();						\
int ClassName::IDAdded =										\
       RGWarehouse::AddClassType (ClassName::Prototype);		\
																\
RGVInStream& operator>>(RGVInStream& vin, ClassName*& ClPtr) {	\
    RGPersistent* ptr = RGPersistent::RestoreFrom (vin);		\
    ClPtr = (ClassName*) ptr;									\
    if (ClPtr != NULL)											\
        ClPtr->RestoreAll (vin);								\
    return vin;													\
}                                                               \
																\
RGFile& operator>>(RGFile& fin, ClassName*& ClPtr) {			\
    RGPersistent* ptr = RGPersistent::RestoreFrom (fin);		\
    ClPtr = (ClassName*) ptr;							        \
    if (ClPtr != NULL)											\
        ClPtr->RestoreAll (fin);								\
    return fin;                                                 \
}																\


#define ABSTRACT_DEFINITION(ClassName)							\
RGVInStream& operator>>(RGVInStream& vin, ClassName*& ClPtr) {	\
    RGPersistent* ptr = RGPersistent::RestoreFrom (vin);		\
    ClPtr = (ClassName*) ptr;									\
    if (ClPtr != NULL)											\
        ClPtr->RestoreAll (vin);								\
    return vin;													\
}                                                               \
																\
RGFile& operator>>(RGFile& fin, ClassName*& ClPtr) {			\
    RGPersistent* ptr = RGPersistent::RestoreFrom (fin);		\
    ClPtr = (ClassName*) ptr;							        \
    if (ClPtr != NULL)											\
        ClPtr->RestoreAll (fin);								\
    return fin;                                                 \
}																\


//#ifdef _WINDOWS
//#define DllExport   __declspec( dllexport )
//#else
//#ifdef WIN32
//#define DllExport   __declspec( dllexport )
//#else
//#define DllExport
//#endif
//#endif

PERSISTENT_PREDECLARATION(RGPersistent)


class RGPersistent {

PERSISTENT_DECLARATION(RGPersistent)

public:
	RGPersistent ();

	virtual ~RGPersistent ();
	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual Boolean IsNullElement () const;

	static RGPersistent* RestoreFrom (RGVInStream& vin);  // Gets subclass type
	static RGPersistent* RestoreFrom (RGFile& fin);      // Gets subclass type
	
};


#endif   /* _RGPERSISTENT_H_  */
