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
*  FileName: rgxmlschema.h
*  Author:   Robert Goor
*
*/
//
//  Header file for XML Schema-based operations:  scanning .xsd files and building XML Name Data Lists
//

#ifndef _XMLSCHEMA_H_
#define _XMLSCHEMA_H_

#include "rgpersist.h"
#include "rgstring.h"
#include "rgdlist.h"
#include "rgdefs.h"
#include "rgpscalar.h"

class RGFile;
class RGVInStream;
class RGVOutStream;

const int _RGXMLTYPE_ = 30;
const int _RGXMLSCHEMASUMMARY_ = 31;
const int _RGXMLATTRIBUTELIST_ = 33;

const int _RGXMLCOMPLEXTYPE_ = 34;
const int _RGXMLELEMENTTYPE_ = 35;
const int _RGXMLCOMPLEXLIST_ = 36;
const int _RGXMLSUBELEMENTTYPE_ = 38;

const int _RGXMLSIMPLETYPE_ = 43;
const int _RGXMLSIMPLEENUMERATIONTYPE_ = 45;
const int _RGXMLRESTRICTEDSIMPLETYPE_ = 46;

class RGXMLAttributeList;
class RGXmlDataRestriction;
class RGXmlDataEnumeration;

enum RGAttributeType { RGNAME, RGTYPE, RGMAXOCCURS, RGSUBSTITUTIONGROUP, RGREFERENCE, RGABSTRACT, 
					  RGSELECTOR, RGFIELD, RGINCLUDEPATH, RGKEYREFERENCE, RGXPATH, RGBASE };



PERSISTENT_PREDECLARATION (RGXMLType)
PERSISTENT_PREDECLARATION (RGXMLSchemaSummary)
PERSISTENT_PREDECLARATION (RGXMLAttributeList)

PERSISTENT_PREDECLARATION (RGXMLSubElementType)
PERSISTENT_PREDECLARATION (RGXMLComplexType)
PERSISTENT_PREDECLARATION (RGXMLElementType)
PERSISTENT_PREDECLARATION (RGXMLComplexList)

PERSISTENT_PREDECLARATION (RGXMLSimpleType)
PERSISTENT_PREDECLARATION (RGXMLSimpleEnumerationType)
PERSISTENT_PREDECLARATION (RGXMLRestrictedSimpleType)


class RGXMLType : public RGPersistent {

PERSISTENT_DECLARATION (RGXMLType)

public:
	RGXMLType ();
	RGXMLType (const RGString& name);
	RGXMLType (const RGXMLType& info);
	~RGXMLType ();

	RGXMLType& operator= (const RGXMLType& info);

	void ResetName (const RGString& name);

	RGString GetName () const;
	RGString GetTrueName () const;

	Boolean IsFinal () const;
	Boolean IsReference () const;
	Boolean SetCppPointer (RGPersistent* ptr);
	RGPersistent* GetCppPointer ();

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual Boolean SetAttributes (const RGXMLAttributeList* list);
	virtual Boolean SetAbstract ();
	virtual Boolean IsAbstract () const;

	virtual Boolean IsContainer () const;
	virtual Boolean HasContainer () const;

	virtual RGString GetBase () const;
	virtual RGString GetType () const;
	virtual Boolean HasLinkedBaseSubElements () const;

	virtual RGXMLType* GetAntecedentType ();

	virtual void ResetType (const RGString& type);
	virtual void ResetBase (const RGString& base);

	virtual Boolean AddSubElement (RGXMLType* subElement);
	virtual Boolean AddSubElementList (RGDList& list);
	virtual Boolean SetContainerName (const RGString& name);
	virtual Boolean SetListElement (RGXMLType* subElement);
	virtual Boolean AddSubstitutionGroupName (const RGString& name);

	virtual RGXMLType* FindSubElementWithName (const RGString& name) const;
	virtual RGXMLType* FindSubElementMatching (const RGXMLType* match) const;
	virtual Boolean ResetSubElements ();
	virtual RGXMLType* GetNextSubElement ();
	virtual int NumberOfSubElements () const;
	virtual Boolean CopyAndPrependSubElementList (const RGXMLType* ExtensionBase);
	virtual RGDListIterator* GetSubElementIterator ();

	virtual Boolean GetContainerName (RGString& name) const;
	virtual const RGXMLType* GetListElement () const;
	virtual Boolean VerifySubstitutionGroupNameInList (const RGString& name) const;
	virtual Boolean GetSubstitutionGroupName (RGString& name) const;

	// virtuals for simpleType's

		// uses pointer restriction without creating a new one; takes responsibility for delete
		// once it adds to list.  If returns FALSE, restriction not deleted
	virtual Boolean AddRestriction (RGXmlDataRestriction* restriction);
		// empties out list - assumes that it is a temporary stack
		// if returns FALSE, first element of list duplicated restriction on RestrictionList
	virtual Boolean AddRestrictionList (RGDList& list);

	virtual Boolean ResetRestrictions ();
	virtual RGXmlDataRestriction* GetNextRestriction ();
	virtual int NumberOfRestrictions () const;

	// Virtual functions for RGXMLSimpleEnumerationType

	virtual Boolean SetEnumeration (RGXmlDataEnumeration* xmlEnum);
	virtual RGString GetStringValue (unsigned long index);
	virtual RGPScalar GetScalarValue (unsigned long index);
	virtual int Initialize ();
	virtual int Test (const RGPScalar& p) const;
	virtual int Test (const RGPScalar* ptr) const;
	virtual unsigned long NumberOfItems () const;

	virtual int PrintResults (const RGString& Indent);

protected:
	RGString XMLName;
	RGPersistent* CppPointer;
};


class RGXMLLevel {

public:
	RGXMLLevel (const RGString& name, RGXMLLevel* currentLevel);
	RGXMLLevel (RGXMLType* newType, RGXMLType* OriginalTag, RGXMLType* OriginalPolyType);
	~RGXMLLevel ();

	Boolean LevelDefined () const;
	Boolean IsContainer () const;
	Boolean HasBase () const;
	Boolean HasType () const;
	Boolean TagNameHasBeenRead () const { return XMLTagNameRead; }
	void SetTagNameRead ();
	void SetTagNameNotRead ();

	RGXMLType* GetCurrentTag ();  // allows getting current name
	RGXMLType* GetCurrentType (); // allows getting subtypes
	RGXMLType* GetBase ();  // allows getting parent of current type
	RGXMLType* GetOriginalTag ();  // allows getting name of original element in chain
	RGXMLType* GetOriginalType ();  // allows getting structure and id of original type in chain

	RGXMLLevel* CreateBaseLevel ();  // creates next level up for inheritance chains

private:
	RGXMLType* CurrentTag;
	RGXMLType* PolymorphicType;
	RGXMLType* CurrentBase;
	RGXMLType* FirstPolymorphicTypeInChain;
	RGXMLType* FirstTagInChain;
	Boolean XMLTagNameRead;
	RGDListIterator* it;
};



class RGXMLSubElementType : public RGXMLType {

PERSISTENT_DECLARATION (RGXMLSubElementType)

public:
	RGXMLSubElementType ();
	RGXMLSubElementType (const RGString& name);
	RGXMLSubElementType (const RGXMLSubElementType& info);
	~RGXMLSubElementType ();

	RGXMLSubElementType& operator= (const RGXMLSubElementType& info);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

		// uses pointer subElement without creating a new one; takes responsibility for delete
		// once it adds element.  If returns FALSE, element not deleted
	virtual Boolean AddSubElement (RGXMLType* subElement);
		// empties out list - assumes that it is a temporary stack
		// if returns FALSE, first element of list duplicated sub-element of SubElementList
	virtual Boolean AddSubElementList (RGDList& list);

	virtual RGXMLType* FindSubElementWithName (const RGString& name) const;
	virtual RGXMLType* FindSubElementMatching (const RGXMLType* match) const;
	virtual Boolean ResetSubElements ();
	virtual RGXMLType* GetNextSubElement ();
	virtual int NumberOfSubElements () const;
	virtual RGDListIterator* GetSubElementIterator ();

	virtual Boolean IsContainer () const;

	virtual int PrintResults (const RGString& Indent);

protected:
	RGDList SubElementList;
	RGDListIterator* it;
};




class RGXMLComplexType : public RGXMLSubElementType {

PERSISTENT_DECLARATION (RGXMLComplexType)

public:
	RGXMLComplexType ();
	RGXMLComplexType (const RGString& name);
	RGXMLComplexType (const RGString& name, const RGString& base);
	RGXMLComplexType (const RGXMLComplexType& info);
	~RGXMLComplexType ();

	RGXMLComplexType& operator= (const RGXMLComplexType& info);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual Boolean SetAttributes (const RGXMLAttributeList* list);

	virtual RGString GetBase () const;
	virtual Boolean HasLinkedBaseSubElements () const;
	virtual Boolean CopyAndPrependSubElementList (const RGXMLType* ExtensionBase);

	virtual RGXMLType* GetAntecedentType ();

	virtual void ResetBase (const RGString& base);
	virtual int PrintResults (const RGString& Indent);

protected:
	RGString XMLBase;
	Boolean SubElementsLinked;
};


class RGXMLElementType : public RGXMLSubElementType {

PERSISTENT_DECLARATION (RGXMLElementType)

public:
	RGXMLElementType ();
	RGXMLElementType (const RGString& name);
	RGXMLElementType (const RGString& name, const RGString& type);
	RGXMLElementType (const RGXMLElementType& info);
	~RGXMLElementType ();

	RGXMLElementType& operator= (const RGXMLElementType& info);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual Boolean SetAttributes (const RGXMLAttributeList* list);
	virtual Boolean SetAbstract ();
	virtual Boolean IsAbstract () const;
	virtual Boolean HasContainer () const;

	virtual RGString GetType () const;
	virtual void ResetType (const RGString& type);

	virtual RGXMLType* GetAntecedentType ();

	virtual Boolean SetContainerName (const RGString& name);
	virtual Boolean AddSubstitutionGroupName (const RGString& name);

	virtual Boolean GetContainerName (RGString& name) const;
	virtual Boolean VerifySubstitutionGroupNameInList (const RGString& name) const;
	virtual Boolean GetSubstitutionGroupName (RGString& name) const;

	virtual int PrintResults (const RGString& Indent);

protected:
	Boolean isAbstract;
	RGString XMLType;
	RGDList SubstitutionGroupList;
	RGString ParentName;
	RGString SubstitutionGroupName;
};


class RGXMLComplexList : public RGXMLType {

PERSISTENT_DECLARATION (RGXMLComplexList)

public:
	RGXMLComplexList ();
	RGXMLComplexList (const RGString& name);
	RGXMLComplexList (const RGXMLComplexList& info);
	~RGXMLComplexList ();

	RGXMLComplexList& operator= (const RGXMLComplexList& info);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual Boolean SetAttributes (const RGXMLAttributeList* list);
	virtual Boolean SetListElement (RGXMLType* subElement);
	virtual const RGXMLType* GetListElement () const;

	virtual RGXMLType* GetAntecedentType ();

	virtual RGString GetType () const;

	virtual int PrintResults (const RGString& Indent);

protected:
	RGXMLElementType* ElementType;
};


class RGXMLSimpleType : public RGXMLType {

PERSISTENT_DECLARATION (RGXMLSimpleType)

public:
	RGXMLSimpleType ();
	RGXMLSimpleType (const RGString& name);
	RGXMLSimpleType (const RGXMLSimpleType& info);
	~RGXMLSimpleType ();

	RGXMLSimpleType& operator= (const RGXMLSimpleType& info);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

		// uses pointer restriction without creating a new one; takes responsibility for delete
		// once it adds to list.  If returns FALSE, did not add restriction.
	//virtual Boolean AddRestriction (RGXmlDataRestriction* restriction);
		// empties out list - assumes that it is a temporary stack
		// If returns FALSE, did not empty out list
	//virtual Boolean AddRestrictionList (RGDList& list);

	//virtual Boolean ResetRestrictions ();
	//virtual RGXmlDataRestriction* GetNextRestriction ();
	//virtual int NumberOfRestrictions () const;

	virtual Boolean SetAttributes (const RGXMLAttributeList* list);
	virtual Boolean HasLinkedBaseSubElements () const;

	virtual RGString GetBase () const;
	virtual void ResetBase (const RGString& base);

	virtual RGXMLType* GetAntecedentType ();

	// Virtual functions for RGXMLSimpleEnumerationType

	//virtual Boolean SetEnumeration (RGXmlDataEnumeration* xmlEnum);
	//virtual RGString GetStringValue (unsigned long index);
	//virtual RGPScalar GetScalarValue (unsigned long index);
	//virtual int Initialize ();
	//virtual int Test (const RGPScalar& p) const;
	//virtual int Test (const RGPScalar* ptr) const;
	//virtual unsigned long NumberOfItems () const;

	virtual int PrintResults (const RGString& Indent);

protected:
	RGString XMLBase;
};


class RGXMLSimpleEnumerationType : public RGXMLSimpleType {

PERSISTENT_DECLARATION (RGXMLSimpleEnumerationType)

public:
	RGXMLSimpleEnumerationType ();
	RGXMLSimpleEnumerationType (const RGString& name);
	RGXMLSimpleEnumerationType (const RGXMLSimpleEnumerationType& info);
	~RGXMLSimpleEnumerationType ();

	RGXMLSimpleEnumerationType& operator= (const RGXMLSimpleEnumerationType& info);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual Boolean SetEnumeration (RGXmlDataEnumeration* xmlEnum);
	virtual RGString GetStringValue (unsigned long index);
	virtual RGPScalar GetScalarValue (unsigned long index);
	virtual int Initialize ();
	virtual int Test (const RGPScalar& p) const;
	virtual int Test (const RGPScalar* ptr) const;
	virtual unsigned long NumberOfItems () const;

	virtual int PrintResults (const RGString& Indent);

protected:
	RGXmlDataEnumeration* XMLEnum;
};


class RGXMLRestrictedSimpleType : public RGXMLSimpleType {

PERSISTENT_DECLARATION (RGXMLRestrictedSimpleType)

public:
	RGXMLRestrictedSimpleType ();
	RGXMLRestrictedSimpleType (const RGString& name);
	RGXMLRestrictedSimpleType (const RGXMLRestrictedSimpleType& info);
	~RGXMLRestrictedSimpleType ();

	RGXMLRestrictedSimpleType& operator= (const RGXMLRestrictedSimpleType& info);

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

		// uses pointer restriction without creating a new one; takes responsibility for delete
		// once it adds to list.  If returns FALSE, did not add restriction.
	virtual Boolean AddRestriction (RGXmlDataRestriction* restriction);
		// empties out list - assumes that it is a temporary stack
		// If returns FALSE, did not empty out list
	virtual Boolean AddRestrictionList (RGDList& list);

	virtual Boolean ResetRestrictions ();
	virtual RGXmlDataRestriction* GetNextRestriction ();
	virtual int NumberOfRestrictions () const;

	virtual Boolean SetAttributes (const RGXMLAttributeList* list);

	virtual int PrintResults (const RGString& Indent);

protected:
	RGDList RestrictionList;
	RGDListIterator* it;
};


class RGXMLAttributeList : public RGPersistent {

PERSISTENT_DECLARATION (RGXMLAttributeList)

public:
	RGXMLAttributeList ();
	RGXMLAttributeList (const RGXMLAttributeList&);
	~RGXMLAttributeList ();

	Boolean AddAttribute (RGAttributeType type, const RGString& value);  // returns TRUE if successful (no multiple name, etc)
	Boolean AddAttribute (RGAttributeType type, const RGString* value);

	int GetNameAndType (RGString& name, RGString& type) const;  // returns -10 if name is empty, 0 otherwise
	int GetNameAndBase (RGString& name, RGString& base) const;  // returns -10 if name is empty, 0 otherwise
	int GetIncludePath (RGString& path) const;

	RGString GetName () const;
	RGString GetBase () const;
	RGString GetType () const;

	Boolean GetSubstitutionGroup (RGString& name) const;
	Boolean IsList () const;
	Boolean IsTerminalType () const;
	Boolean IsAbstract () const;
	int SetTypeFromAttributeList (const RGXMLAttributeList* list);
	int SetBaseFromAttributeList (const RGXMLAttributeList* list);

	RGString* GetFirstXPath ();

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
	RGString NameAttribute;
	RGString TypeAttribute;
	RGString MaxOccursAttribute;
	RGString SubstitutionGroupAttribute;
	RGString AbstractAttribute;
	RGString SelectorAttribute;
	RGString FieldAttribute;
	RGString IncludePathAttribute;
	RGString KeyReferenceAttribute;
	RGString BaseAttribute;
	Boolean isList;

	RGDList XPathStack;
};



class RGXMLSchemaSummary : public RGPersistent {

PERSISTENT_DECLARATION (RGXMLSchemaSummary)

public:
	RGXMLSchemaSummary ();
	RGXMLSchemaSummary (const RGString& schemaPath);
	~RGXMLSchemaSummary ();

	int SetSchemaPath (const RGString& schemaPath);

	int CompileSchema ();  // scans schema(s) to build table and stores binary table version plus XML version?
	Boolean LinkSchemaTable ();

	RGXMLAttributeList* GetCurrentAttributeList ();
	void SaveCurrentAttributes ();
	void ClearAttributes ();

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	int PrintResults ();
	Boolean LinkSchemaDataWithWarehouseData ();

	static void SetDebugOn ();
	static void SetDebugOff ();  // it's off by default

protected:
	RGString SchemaPath;  // save
	RGString BinaryRestorePath;  // save
	RGString XMLRestorePath;  // save
	int MostRecent;
	RGDList AttributeStack;
	RGDList XPathStack;
	RGDList LocalSchemasNeeded;  // save:  RGString's that are paths to schema files
	RGDList SchemaDataResults;   // save
	RGDList IncludedSchemaSummaries;  // save:  RGXMLSchemaSummary's built from included schema
	RGDList FilesAlreadyDone;
	RGDList AttributeData;

	RGDList SequencedElementsStack;
	RGDList ListElementStack;
	RGDList NestedElementNameStack;

	//  Add list(s) for key/keyReference info

	RGDList* FilesScanned;
	RGDList* ResultList;

	RGXMLAttributeList* CurrentAttributeList;

	static Boolean DebugFlag;

	int CompileIncludedSchema (RGDList& FilesAlreadyScanned, RGDList& Results);  // scans included schema(s) to build table
	int ChooseDataMethod ();  // -1 = Scan Schema; +1 = binary restore; 0 = XML restore?
	int LinkSchemaTable (RGDList& FilesAlreadyScanned, RGDList& Results);  // uses most efficient method depending on
			// file modification dates
	
	int YYPARSE ();
};

#endif   /*  _XMLSCHEMA_H_  */

