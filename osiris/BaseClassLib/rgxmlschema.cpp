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
*  FileName: rgxmlschema.cpp
*  Author:   Robert Goor
*
*/
//
//  Source file for XML Schema-based operations:  scanning .xsd files and building XML Name Data Lists
//

#include "rgdefs.h"
#include "rgxmlschema.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgexec.h"
#include "rgwarehouse.h"
#include "rgxmlrestrict.h"

PERSISTENT_DEFINITION(RGXMLType, _RGXMLTYPE_, "XMLType")
PERSISTENT_DEFINITION(RGXMLAttributeList, _RGXMLATTRIBUTELIST_, "XMLAttributeList")
PERSISTENT_DEFINITION(RGXMLSchemaSummary, _RGXMLSCHEMASUMMARY_, "XMLSchemaSummary")

PERSISTENT_DEFINITION (RGXMLSubElementType, _RGXMLSUBELEMENTTYPE_, "XMLSubElementType")
PERSISTENT_DEFINITION (RGXMLComplexType, _RGXMLCOMPLEXTYPE_, "XMLComplexType")
PERSISTENT_DEFINITION (RGXMLElementType, _RGXMLELEMENTTYPE_, "XMLElement")
PERSISTENT_DEFINITION (RGXMLComplexList, _RGXMLCOMPLEXLIST_, "XMLComplexList")
PERSISTENT_DEFINITION (RGXMLSimpleType, _RGXMLSIMPLETYPE_, "XMLSimpleType")
PERSISTENT_DEFINITION (RGXMLSimpleEnumerationType, _RGXMLSIMPLEENUMERATIONTYPE_, "XMLSimpleEnumerationType")
PERSISTENT_DEFINITION (RGXMLRestrictedSimpleType, _RGXMLRESTRICTEDSIMPLETYPE_, "XMLRestrictedSimpleType")

const char* RGReferenceCharacter = "@";

Boolean RGXMLSchemaSummary::DebugFlag = FALSE;


RGXMLType :: RGXMLType () : RGPersistent (), CppPointer (NULL) {

}


RGXMLType :: RGXMLType (const RGString& name) : RGPersistent (), XMLName (name), CppPointer (NULL) {

}


RGXMLType :: RGXMLType (const RGXMLType& info) : RGPersistent (info), XMLName (info.XMLName),
CppPointer (info.CppPointer) {

}


RGXMLType :: ~RGXMLType () {

}


RGXMLType& RGXMLType :: operator= (const RGXMLType& info) {

	if (this == &info)
		return *this;

	XMLName = info.XMLName;
	CppPointer = info.CppPointer;
	return *this;
}


void RGXMLType :: ResetName (const RGString& name) {

	XMLName = name;
}


void RGXMLType :: ResetType (const RGString& type) {

}

void RGXMLType :: ResetBase (const RGString& base) {

}


RGString RGXMLType :: GetName () const {

	return XMLName;
}


RGString RGXMLType :: GetType () const {

	RGString temp;
	return temp;
}


Boolean RGXMLType :: HasLinkedBaseSubElements () const {

	return FALSE;
}


RGXMLType* RGXMLType :: GetAntecedentType () {

	return NULL;
}


RGString RGXMLType :: GetBase () const {

	RGString temp;
	return temp;
}


RGString RGXMLType :: GetTrueName () const {

	// If not a reference, true name = xmlname; otherwise, RGReferenceCharacter
	// has been prepended and must be removed to get the true name
	
	if (!IsReference ())
		return XMLName;

	int Z = XMLName.Length () - 1;
	int A = strlen (RGReferenceCharacter);
	RGString TempName = XMLName.ExtractSubstring (A, Z);
	return TempName;
}


Boolean RGXMLType :: IsFinal () const {

	return (CppPointer != NULL);
}


Boolean RGXMLType :: IsReference () const {

	if (XMLName.IsEmpty ())
		return FALSE;

	if (XMLName.GetCharacter (0) == RGReferenceCharacter [0])
		return TRUE;

	return FALSE;
}


Boolean RGXMLType :: SetCppPointer (RGPersistent* ptr) {

	//
	// Change this to check with current XML Data types in class library
	//

	
	if (CppPointer != NULL)
		return FALSE;

	CppPointer = ptr;
	return TRUE;
}


RGPersistent* RGXMLType :: GetCppPointer () {

	return CppPointer;
}



size_t RGXMLType :: StoreSize () const {

	size_t Size = RGPersistent::StoreSize ();
	Size += XMLName.StoreSize ();
	return Size;
}


int RGXMLType :: CompareTo (const RGPersistent* p) const {

	RGXMLType* q = (RGXMLType*) p;

	return XMLName.CompareTo (&(q->XMLName));
}


unsigned RGXMLType :: HashNumber (unsigned long Base) const {

	return XMLName.HashNumber (Base);
}


Boolean RGXMLType :: IsEqualTo (const RGPersistent* p) const {

	RGXMLType* q = (RGXMLType*) p;

	return XMLName.IsEqualTo (&(q->XMLName));
}


void RGXMLType :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	XMLName.RestoreAll (f);
}


void RGXMLType :: RestoreAll (RGVInStream& vin) {

	RGPersistent::RestoreAll (vin);
	XMLName.RestoreAll (vin);
}


void RGXMLType :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	XMLName.SaveAll (f);
}


void RGXMLType :: SaveAll (RGVOutStream& vout) const {

	RGPersistent::SaveAll (vout);
	XMLName.SaveAll (vout);
}


Boolean RGXMLType :: SetAttributes (const RGXMLAttributeList* list) {

	if (list == NULL)
		return FALSE;

	XMLName = list->GetName ();
	return TRUE;
}


Boolean RGXMLType :: SetAbstract () {

	return FALSE;
}


Boolean RGXMLType :: IsAbstract () const {

	return FALSE;
}

Boolean RGXMLType :: IsContainer () const {

	return FALSE;
}


Boolean RGXMLType :: HasContainer () const {

	return FALSE;
}


Boolean RGXMLType :: AddSubElement (RGXMLType* subElement) {

	return FALSE;
}


Boolean RGXMLType :: AddSubElementList (RGDList& list) {

	return FALSE;
}


Boolean RGXMLType :: SetContainerName (const RGString& name) {

	return FALSE;
}


Boolean RGXMLType :: SetListElement (RGXMLType* subElement) {

	return FALSE;
}


Boolean RGXMLType :: AddSubstitutionGroupName (const RGString& name) {

	return FALSE;
}


RGXMLType* RGXMLType :: FindSubElementWithName (const RGString& name) const {

	return NULL;
}


RGXMLType* RGXMLType :: FindSubElementMatching (const RGXMLType* match) const {

	return NULL;
}


Boolean RGXMLType :: ResetSubElements () {

	return FALSE;
}


RGXMLType* RGXMLType :: GetNextSubElement () {

	return NULL;
}


int RGXMLType :: NumberOfSubElements () const {

	return 0;
}

Boolean RGXMLType :: CopyAndPrependSubElementList (const RGXMLType* ExtensionBase) {

	return FALSE;
}


RGDListIterator* RGXMLType :: GetSubElementIterator () {

	return NULL;
}


Boolean RGXMLType :: GetContainerName (RGString& name) const {

	return FALSE;
}


const RGXMLType* RGXMLType :: GetListElement () const {

	return NULL;
}


Boolean RGXMLType :: VerifySubstitutionGroupNameInList (const RGString& name) const {

	return FALSE;
}


Boolean RGXMLType :: GetSubstitutionGroupName (RGString& name) const {

	return FALSE;
}


Boolean RGXMLType :: AddRestriction (RGXmlDataRestriction* restriction) {

	return FALSE;
}

	
Boolean RGXMLType :: AddRestrictionList (RGDList& list) {

	return FALSE;
}


Boolean RGXMLType :: ResetRestrictions () {

	return FALSE;
}


RGXmlDataRestriction* RGXMLType :: GetNextRestriction () {

	return NULL;
}


int RGXMLType :: NumberOfRestrictions () const {

	return 0;
}


Boolean RGXMLType :: SetEnumeration (RGXmlDataEnumeration* xmlEnum) {

	return FALSE;
}


RGString RGXMLType :: GetStringValue (unsigned long index) {

	return RGString ();
}


RGPScalar RGXMLType :: GetScalarValue (unsigned long index) {

	return RGPScalar ();
}


int RGXMLType :: Initialize () {

	return -1;
}


int RGXMLType :: Test (const RGPScalar& p) const {

	return -1;
}


int RGXMLType :: Test (const RGPScalar* ptr) const {

	return -1;
}


unsigned long RGXMLType :: NumberOfItems () const {

	return 0;
}



int RGXMLType :: PrintResults (const RGString& Indent) {

	cout << Indent << "Name = '" << XMLName << "'" << endl;
	return 0;
}


RGXMLLevel :: RGXMLLevel (const RGString& name, RGXMLLevel* currentLevel) : XMLTagNameRead (FALSE) {

	RGString base;
	RGXMLType* curr;
	
	if (currentLevel != NULL) {

		curr = currentLevel->GetCurrentType ();

		if (curr != NULL)
			CurrentTag = curr->FindSubElementWithName (name);

		else
			CurrentTag = NULL;
	}

	else
		CurrentTag = RGWarehouse::GlobalFindXMLPrototype (name);

	if (CurrentTag != NULL)
		PolymorphicType = CurrentTag->GetAntecedentType ();

	else 
		PolymorphicType = NULL;

	if (PolymorphicType != NULL)
		it = PolymorphicType->GetSubElementIterator ();

	else
		it = NULL;

	if (PolymorphicType != NULL) {

		base = PolymorphicType->GetBase ();

		if (!base.IsEmpty ())
			CurrentBase = RGWarehouse::GlobalFindXMLPrototype (base);

		else
			CurrentBase = NULL;
	}
	
	else
		CurrentBase = NULL;

	FirstTagInChain = CurrentTag;
	FirstPolymorphicTypeInChain = PolymorphicType;
	XMLTagNameRead = TRUE;
}


RGXMLLevel :: RGXMLLevel (RGXMLType* newType, RGXMLType* OriginalTag, RGXMLType* OriginalPolyType) {

	RGString base;
	CurrentTag = newType;

	if (CurrentTag != NULL)
		PolymorphicType = CurrentTag->GetAntecedentType ();

	else 
		PolymorphicType = NULL;

	if (PolymorphicType != NULL)
		it = PolymorphicType->GetSubElementIterator ();

	else
		it = NULL;

	if (PolymorphicType != NULL) {

		base = PolymorphicType->GetBase ();

		if (!base.IsEmpty ())
			CurrentBase = RGWarehouse::GlobalFindXMLPrototype (base);

		else
			CurrentBase = NULL;
	}
	
	else
		CurrentBase = NULL;

	if (CurrentBase == PolymorphicType)
		CurrentBase = NULL;

	FirstPolymorphicTypeInChain = OriginalPolyType;
	FirstTagInChain = OriginalTag;
	XMLTagNameRead = FALSE;
}


RGXMLLevel :: ~RGXMLLevel () {

	delete it;
}


Boolean RGXMLLevel :: LevelDefined () const {

	return (CurrentTag != NULL);
}


Boolean RGXMLLevel :: IsContainer () const {

	if (CurrentBase == NULL)
		return FALSE;
	
	return CurrentBase->IsContainer ();
}


Boolean RGXMLLevel :: HasBase () const {

	return (CurrentBase != NULL);
}

Boolean RGXMLLevel :: HasType () const {

	return (PolymorphicType != NULL);
}

void RGXMLLevel :: SetTagNameRead () {

	XMLTagNameRead = TRUE;
}


void RGXMLLevel :: SetTagNameNotRead () {

	XMLTagNameRead = FALSE;
}

RGXMLType* RGXMLLevel :: GetCurrentTag () {
	
	return CurrentTag;
}

RGXMLType* RGXMLLevel :: GetCurrentType () {
	
	return PolymorphicType;
}


RGXMLType* RGXMLLevel :: GetBase () {

	// allows getting subtypes
	return CurrentBase;
}

RGXMLType* RGXMLLevel :: GetOriginalTag () {
	
	// allows getting name of original element in chain
	return FirstTagInChain;
}


RGXMLType* RGXMLLevel :: GetOriginalType () {
	
	// allows getting structure and id of original type in chain
	return FirstPolymorphicTypeInChain;
}


RGXMLLevel* RGXMLLevel :: CreateBaseLevel () {

	if ((PolymorphicType == NULL) || (CurrentBase == NULL))
		return NULL;

	if (PolymorphicType == CurrentBase)
		return NULL;

	RGXMLLevel* NewLevel = new RGXMLLevel (CurrentBase, FirstTagInChain, FirstPolymorphicTypeInChain);
	return NewLevel;
}


RGXMLSubElementType :: RGXMLSubElementType () : RGXMLType () {

	it = new RGDListIterator (SubElementList);
}


RGXMLSubElementType :: RGXMLSubElementType (const RGString& name) : RGXMLType (name) {

	it = new RGDListIterator (SubElementList);
}


RGXMLSubElementType :: RGXMLSubElementType (const RGXMLSubElementType& info) : RGXMLType (info), 
SubElementList (info.SubElementList) {

	it = new RGDListIterator (SubElementList);
}


RGXMLSubElementType :: ~RGXMLSubElementType () {

	delete it;
	SubElementList.ClearAndDelete ();
}


RGXMLSubElementType& RGXMLSubElementType :: operator= (const RGXMLSubElementType& info) {

	if (this == &info)
		return *this;

	*(RGXMLType*)this = (RGXMLType&) info;
	SubElementList = info.SubElementList;
	delete it;
	it = new RGDListIterator (SubElementList);
	return *this;
}

size_t RGXMLSubElementType :: StoreSize () const {

	size_t size = RGXMLType::StoreSize () + SubElementList.StoreSize ();
	return size;
}


void RGXMLSubElementType :: RestoreAll (RGFile& f) {

	RGXMLType::RestoreAll (f);
	SubElementList.RestoreAll (f);
}


void RGXMLSubElementType :: RestoreAll (RGVInStream& f) {

	RGXMLType::RestoreAll (f);
	SubElementList.RestoreAll (f);
}


void RGXMLSubElementType :: SaveAll (RGFile& f) const {

	RGXMLType::SaveAll (f);
	SubElementList.SaveAll (f);
}


void RGXMLSubElementType :: SaveAll (RGVOutStream& f) const {

	RGXMLType::SaveAll (f);
	SubElementList.SaveAll (f);
}


Boolean RGXMLSubElementType :: AddSubElement (RGXMLType* subElement) {

	// uses pointer subElement without creating a new one; takes responsibility for delete
	// once it adds element.  If returns FALSE, element not deleted
	
	if (SubElementList.Contains (subElement))
		return FALSE;

	SubElementList.Append (subElement);
	return TRUE;
}


Boolean RGXMLSubElementType :: AddSubElementList (RGDList& list) {

	// empties out list - assumes that it is a temporary stack
	// if returns FALSE, first element of list duplicated sub-element of SubElementList

	RGXMLType* NextType;

	while (NextType = (RGXMLType*)list.GetFirst ()) {

		if (SubElementList.Contains (NextType)) {

			list.Prepend (NextType);
			return FALSE;
		}

		SubElementList.Append (NextType);
	}

	return TRUE;
}


RGXMLType* RGXMLSubElementType :: FindSubElementWithName (const RGString& name) const {

	RGXMLType* match = new RGXMLType (name);
	RGXMLType* found = (RGXMLType*)SubElementList.Find (match);
	delete match;
	return found;
}


RGXMLType* RGXMLSubElementType :: FindSubElementMatching (const RGXMLType* match) const {

	return (RGXMLType*)SubElementList.Find (match);
}


Boolean RGXMLSubElementType :: ResetSubElements () {

	it->Reset ();
	return TRUE;
}


RGXMLType* RGXMLSubElementType :: GetNextSubElement () {

	return (RGXMLType*)(*it)();
}


int RGXMLSubElementType :: NumberOfSubElements () const {

	return SubElementList.Entries ();
}

RGDListIterator* RGXMLSubElementType :: GetSubElementIterator () {

	RGDListIterator* it = new RGDListIterator (SubElementList);
	return it;
}

Boolean RGXMLSubElementType :: IsContainer () const {

	return !SubElementList.IsEmpty ();
}


int RGXMLSubElementType :: PrintResults (const RGString& Indent) {

	RGXMLType* type;
	
	if (SubElementList.Entries () > 0)
		cout << Indent << "With sequence:" << endl;

	else
		return 0;

	it->Reset ();
	RGString NewIndent = Indent + "    ";

	while (type = (RGXMLType*)(*it)()) {

		type->PrintResults (NewIndent);
	}

	return 0;
}



RGXMLComplexType :: RGXMLComplexType () : RGXMLSubElementType (), SubElementsLinked (FALSE) {

}


RGXMLComplexType :: RGXMLComplexType (const RGString& name) : RGXMLSubElementType (name), SubElementsLinked (FALSE) {

}


RGXMLComplexType :: RGXMLComplexType (const RGString& name, const RGString& base) : RGXMLSubElementType (name),
XMLBase (base), SubElementsLinked (FALSE) {

}


RGXMLComplexType :: RGXMLComplexType (const RGXMLComplexType& info) : RGXMLSubElementType (info), 
XMLBase (info.XMLBase), SubElementsLinked (FALSE) {

}


RGXMLComplexType :: ~RGXMLComplexType () {

}


RGXMLComplexType& RGXMLComplexType :: operator= (const RGXMLComplexType& info) {

	if (this == &info)
		return *this;

	*(RGXMLSubElementType*)this = (RGXMLSubElementType&) info;
	XMLBase = info.XMLBase;
	SubElementsLinked = info.SubElementsLinked;
	return *this;
}


size_t RGXMLComplexType :: StoreSize () const {

	size_t size = RGXMLSubElementType::StoreSize () + XMLBase.StoreSize ();
	return size;
}


void RGXMLComplexType :: RestoreAll (RGFile& f) {

	RGXMLSubElementType::RestoreAll (f);
	XMLBase.RestoreAll (f);
}


void RGXMLComplexType :: RestoreAll (RGVInStream& f) {

	RGXMLSubElementType::RestoreAll (f);
	XMLBase.RestoreAll (f);
}


void RGXMLComplexType :: SaveAll (RGFile& f) const {

	RGXMLSubElementType::SaveAll (f);
	XMLBase.SaveAll (f);
}


void RGXMLComplexType :: SaveAll (RGVOutStream& f) const {

	RGXMLSubElementType::SaveAll (f);
	XMLBase.SaveAll (f);
}


Boolean RGXMLComplexType :: SetAttributes (const RGXMLAttributeList* list) {

	Boolean status = RGXMLType::SetAttributes (list);

	if (status == FALSE)
		return FALSE;

	XMLBase = list->GetBase ();
	return TRUE;
}


RGString RGXMLComplexType :: GetBase () const {

	return XMLBase;
}


Boolean RGXMLComplexType :: HasLinkedBaseSubElements () const {

	return SubElementsLinked || (XMLBase.IsEmpty ());
}


Boolean RGXMLComplexType :: CopyAndPrependSubElementList (const RGXMLType* ExtensionBase) {

	RGXMLComplexType* Base = (RGXMLComplexType*) ExtensionBase;
	RGDListIterator ExtensionIterator (Base->SubElementList);
	ExtensionIterator.ResetToEnd ();
	RGXMLType* FoundType;
	RGXMLType* CopyType;

	while (FoundType = (RGXMLType*)ExtensionIterator.CurrentItem ()) {

		CopyType = (RGXMLType*)FoundType->Copy ();
		SubElementList.Prepend (CopyType);
		--ExtensionIterator;
	}

	SubElementsLinked = TRUE;
	return TRUE;
}


RGXMLType* RGXMLComplexType :: GetAntecedentType () {

	return (RGXMLComplexType*) this;
}


void RGXMLComplexType :: ResetBase (const RGString& base) {

	XMLBase = base;
}


int RGXMLComplexType :: PrintResults (const RGString& Indent) {

	cout << endl;
	RGString NewIndent = Indent + "    ";
	cout << Indent << "ComplexType:  ";
	RGXMLType::PrintResults ("");

	if (!XMLBase.IsEmpty ())
		cout << Indent << "Extension Base = '" << XMLBase << "'" << endl;

	RGXMLSubElementType::PrintResults (Indent);
	cout << endl;
	return 0;
}




RGXMLElementType :: RGXMLElementType () : RGXMLSubElementType (), isAbstract (FALSE) {

}


RGXMLElementType :: RGXMLElementType (const RGString& name) : RGXMLSubElementType (name), isAbstract (FALSE) {

}



RGXMLElementType :: RGXMLElementType (const RGString& name, const RGString& type) : RGXMLSubElementType (name), 
isAbstract (FALSE), XMLType (type) {

}



RGXMLElementType :: RGXMLElementType (const RGXMLElementType& info) : RGXMLSubElementType (info), 
isAbstract (FALSE),
XMLType (info.XMLType),
SubstitutionGroupList (info.SubstitutionGroupList),
ParentName (info.ParentName) {

}


RGXMLElementType :: ~RGXMLElementType () {

	SubstitutionGroupList.ClearAndDelete ();
}



RGXMLElementType& RGXMLElementType :: operator= (const RGXMLElementType& info) {

	if (this == &info)
		return *this;

	*(RGXMLSubElementType*)this = (RGXMLSubElementType&) info;
	SubstitutionGroupList = info.SubstitutionGroupList;
	isAbstract = info.isAbstract;
	ParentName = info.ParentName;
	SubstitutionGroupName = info.SubstitutionGroupName;
	XMLType = info.XMLType;
	return *this;
}


size_t RGXMLElementType :: StoreSize () const {

	size_t size = RGXMLSubElementType::StoreSize ();
	size += SubstitutionGroupList.StoreSize ();
	size += sizeof (isAbstract);
	size += XMLType.StoreSize ();
	size += ParentName.StoreSize ();
	size += SubstitutionGroupName.StoreSize ();
	return size;
}


void RGXMLElementType :: RestoreAll (RGFile& f) {

	RGXMLSubElementType::RestoreAll (f);
	f.Read (isAbstract);
	XMLType.RestoreAll (f);
	SubstitutionGroupList.RestoreAll (f);
	ParentName.RestoreAll (f);
	SubstitutionGroupName.RestoreAll (f);
}


void RGXMLElementType :: RestoreAll (RGVInStream& f) {

	RGXMLSubElementType::RestoreAll (f);
	f >> isAbstract;
	XMLType.RestoreAll (f);
	SubstitutionGroupList.RestoreAll (f);
	ParentName.RestoreAll (f);
	SubstitutionGroupName.RestoreAll (f);
}


void RGXMLElementType :: SaveAll (RGFile& f) const {

	RGXMLSubElementType::SaveAll (f);
	f.Write (isAbstract);
	XMLType.SaveAll (f);
	SubstitutionGroupList.SaveAll (f);
	ParentName.SaveAll (f);
	SubstitutionGroupName.SaveAll (f);
}


void RGXMLElementType :: SaveAll (RGVOutStream& f) const {

	RGXMLSubElementType::SaveAll (f);
	f << isAbstract;
	XMLType.SaveAll (f);
	SubstitutionGroupList.SaveAll (f);
	ParentName.SaveAll (f);
	SubstitutionGroupName.SaveAll (f);
}


Boolean RGXMLElementType :: SetAttributes (const RGXMLAttributeList* list) {

	Boolean status = RGXMLType::SetAttributes (list);

	if (status == FALSE)
		return FALSE;

	XMLType = list->GetType ();
	list->GetSubstitutionGroup (SubstitutionGroupName);
	isAbstract = list->IsAbstract ();
	return TRUE;
}


Boolean RGXMLElementType :: SetAbstract () {

	isAbstract = TRUE;
	return TRUE;
}


Boolean RGXMLElementType :: IsAbstract () const {

	return isAbstract;
}

Boolean RGXMLElementType :: HasContainer () const {

	return !ParentName.IsEmpty ();
}


RGString RGXMLElementType :: GetType () const {

	return XMLType;
}


RGXMLType* RGXMLElementType :: GetAntecedentType () {

	return RGWarehouse::GlobalFindXMLPrototype (XMLType);
}


void RGXMLElementType :: ResetType (const RGString& type) {

	XMLType = type;
}


Boolean RGXMLElementType :: SetContainerName (const RGString& name) {

	ParentName = name;
	return TRUE;
}


Boolean RGXMLElementType :: AddSubstitutionGroupName (const RGString& name) {

	RGString* NewName = new RGString (name);
	
	if (SubstitutionGroupList.Contains (NewName)) {

		delete NewName;
		return TRUE;
	}

	SubstitutionGroupList.Append (NewName);
	return TRUE;
}


Boolean RGXMLElementType :: GetContainerName (RGString& name) const {

	name = ParentName;
	return TRUE;
}


Boolean RGXMLElementType :: VerifySubstitutionGroupNameInList (const RGString& name) const {

	return SubstitutionGroupList.Contains (&name);
}


Boolean RGXMLElementType :: GetSubstitutionGroupName (RGString& name) const {

	if (SubstitutionGroupName.Length () == 0)
		return FALSE;

	name = SubstitutionGroupName;
	return TRUE;
}


int RGXMLElementType :: PrintResults (const RGString& Indent) {

	RGString NewIndent = Indent + "    ";
	RGXMLType* type;
	cout << endl;
	cout << Indent << "XML Element:" << endl;
	RGXMLType::PrintResults (NewIndent);
	
	if (!XMLType.IsEmpty())
		cout << NewIndent << "Type = '" << XMLType << "'" << endl;

	if (isAbstract)
		cout << NewIndent << "Element is abstract";

	if (ParentName.Length () > 0)
		cout << NewIndent << "With Container Name = " << ParentName;

	if (SubstitutionGroupName.Length() > 0)
		cout << NewIndent << "With Substitution Group Name = " << SubstitutionGroupName;

	RGXMLSubElementType::PrintResults (Indent);

	RGDListIterator* Itt;
	NewIndent += "    ";

	if (SubstitutionGroupList.Entries () > 0) {

		Itt = new RGDListIterator (SubstitutionGroupList);
		cout << "Substitution Group Elements:" << endl;

		while (type = (RGXMLType*)(*Itt)()) {

			type->PrintResults (NewIndent);
		}
	}

	cout << endl;
	return 0;
}


RGXMLComplexList :: RGXMLComplexList () : RGXMLType () {

	ElementType = new RGXMLElementType ();
}


RGXMLComplexList :: RGXMLComplexList (const RGString& name) : RGXMLType (name) {

	ElementType = new RGXMLElementType ();
}


RGXMLComplexList :: RGXMLComplexList (const RGXMLComplexList& info) : RGXMLType (info) {

	ElementType = new RGXMLElementType (*info.ElementType);
}


RGXMLComplexList :: ~RGXMLComplexList () {

	delete ElementType;
}

RGXMLComplexList& RGXMLComplexList :: operator= (const RGXMLComplexList& info) {

	if (this == &info)
		return *this;

	*(RGXMLType*)this = (RGXMLType&) info;
	*ElementType = *info.ElementType;
	return *this;
}


size_t RGXMLComplexList :: StoreSize () const {

	size_t size = RGXMLType::StoreSize ();
	size += ElementType->StoreSize ();
	return size;
}


void RGXMLComplexList :: RestoreAll (RGFile& f) {

	RGXMLType::RestoreAll (f);
	ElementType->RestoreAll (f);
}


void RGXMLComplexList :: RestoreAll (RGVInStream& f) {

	RGXMLType::RestoreAll (f);
	ElementType->RestoreAll (f);
}


void RGXMLComplexList :: SaveAll (RGFile& f) const {

	RGXMLType::SaveAll (f);
	ElementType->SaveAll (f);
}


void RGXMLComplexList :: SaveAll (RGVOutStream& f) const {

	RGXMLType::SaveAll (f);
	ElementType->SaveAll (f);
}


Boolean RGXMLComplexList :: SetAttributes (const RGXMLAttributeList* list) {

	Boolean Success = RGXMLType::SetAttributes (list);

	if (!Success)
		return FALSE;

	ResetType ("List");
	return TRUE;
}


Boolean RGXMLComplexList :: SetListElement (RGXMLType* subElement) {

	RGXMLElementType* p = (RGXMLElementType*) subElement;
	*ElementType = *p;
	return TRUE;
}


const RGXMLType* RGXMLComplexList :: GetListElement () const {

	return ElementType;
}


RGXMLType* RGXMLComplexList :: GetAntecedentType () {

	RGString list ("List");
	return RGWarehouse::GlobalFindXMLPrototype (list);
}



RGString RGXMLComplexList :: GetType () const {

	RGString ReturnValue ("List");
	return ReturnValue;
}


int RGXMLComplexList :: PrintResults (const RGString& Indent) {

	RGString NewIndent = Indent + "    ";
	cout << endl;
	cout << Indent << "Complex Unbounded List" << endl;
	RGXMLType::PrintResults (Indent);
	cout << Indent << "With element:" << endl;
	ElementType->PrintResults (NewIndent);
	cout << endl;	
	return 0;
}


RGXMLSimpleType :: RGXMLSimpleType () : RGXMLType () {

}


RGXMLSimpleType :: RGXMLSimpleType (const RGString& name) : RGXMLType (name) {

}


RGXMLSimpleType :: RGXMLSimpleType (const RGXMLSimpleType& info) : RGXMLType (info), XMLBase (info.XMLBase) {

}


RGXMLSimpleType :: ~RGXMLSimpleType () {

}


RGXMLSimpleType& RGXMLSimpleType :: operator= (const RGXMLSimpleType& info) {

	if (this == &info)
		return *this;

	*(RGXMLType*)this = (RGXMLType&) info;
	XMLBase = info.XMLBase;
	return *this;
}


size_t RGXMLSimpleType :: StoreSize () const {

	size_t size = RGXMLType::StoreSize () + XMLBase.StoreSize ();
	return size;
}

void RGXMLSimpleType :: RestoreAll (RGFile& f) {

	RGXMLType::RestoreAll (f);
	XMLBase.RestoreAll (f);
}


void RGXMLSimpleType :: RestoreAll (RGVInStream& f) {

	RGXMLType::RestoreAll (f);
	XMLBase.RestoreAll (f);
}


void RGXMLSimpleType :: SaveAll (RGFile& f) const {

	RGXMLType::SaveAll (f);
	XMLBase.SaveAll (f);
}


void RGXMLSimpleType :: SaveAll (RGVOutStream& f) const {

	RGXMLType::SaveAll (f);
	XMLBase.SaveAll (f);
}


Boolean RGXMLSimpleType :: SetAttributes (const RGXMLAttributeList* list) {

	Boolean Success = RGXMLType::SetAttributes (list);

	if (Success == FALSE)
		return FALSE;

	XMLBase = list->GetBase ();
	return TRUE;
}


Boolean RGXMLSimpleType :: HasLinkedBaseSubElements () const {

	return TRUE;
}


RGString RGXMLSimpleType :: GetBase () const {

	return XMLBase;
}


void RGXMLSimpleType :: ResetBase (const RGString& base) {

	XMLBase = base;
}


RGXMLType* RGXMLSimpleType :: GetAntecedentType () {

	RGXMLType* type1;
	RGXMLType* type2;
	RGString CurrentBase = XMLBase;
	type2 = (RGXMLType*) this;

	while (TRUE) {

		type1 = RGWarehouse::GlobalFindXMLPrototype (CurrentBase);

		if ((type1 == NULL) || (type1 == type2))
			return type2;

		type2 = type1;
		CurrentBase = type1->GetBase ();

		if (CurrentBase.IsEmpty ())
			return type2;
	}

	return NULL;
}


int RGXMLSimpleType :: PrintResults (const RGString& Indent) {

	cout << endl;
	RGString NewIndent = Indent + "    ";
	cout << Indent << "SimpleType:  ";
	RGXMLType::PrintResults ("");

	if (!XMLBase.IsEmpty ())
		cout << Indent << "Restriction Base = '" << XMLBase << "'" << endl;

	cout << endl;
	return 0;
}


RGXMLSimpleEnumerationType :: RGXMLSimpleEnumerationType () : RGXMLSimpleType (), XMLEnum (NULL) {

}


RGXMLSimpleEnumerationType :: RGXMLSimpleEnumerationType (const RGString& name) : RGXMLSimpleType (name), XMLEnum (NULL) {

}


RGXMLSimpleEnumerationType :: RGXMLSimpleEnumerationType (const RGXMLSimpleEnumerationType& info) : RGXMLSimpleType (info) {

    XMLEnum = new RGXmlDataEnumeration (*info.XMLEnum);
}


RGXMLSimpleEnumerationType :: ~RGXMLSimpleEnumerationType () {

	delete XMLEnum;
}


RGXMLSimpleEnumerationType& RGXMLSimpleEnumerationType :: operator= (const RGXMLSimpleEnumerationType& info) {

	if (this == &info)
		return *this;

	*(RGXMLSimpleType*)this = (RGXMLSimpleType&) info;
	XMLEnum = new RGXmlDataEnumeration (*info.XMLEnum);
	return *this;
}


size_t RGXMLSimpleEnumerationType :: StoreSize () const {

	size_t size = RGXMLSimpleType::StoreSize () + sizeof (Boolean);

	if (XMLEnum != NULL) {
	
		size += XMLEnum->StoreSize ();
	}

	return size;
}


void RGXMLSimpleEnumerationType :: RestoreAll (RGFile& f) {

	Boolean NullPointer;
	
	RGXMLSimpleType::RestoreAll (f);
	f.Read (NullPointer);

	if (NullPointer == FALSE) {

		delete XMLEnum;
		f >> XMLEnum;
	}
}


void RGXMLSimpleEnumerationType :: RestoreAll (RGVInStream& f) {

	Boolean NullPointer;
	
	RGXMLSimpleType::RestoreAll (f);
	f >> NullPointer;

	if (NullPointer == FALSE) {

		delete XMLEnum;
		f >> XMLEnum;
	}
}


void RGXMLSimpleEnumerationType :: SaveAll (RGFile& f) const {

	Boolean NullPointer;
	
	RGXMLSimpleType::SaveAll (f);

	if (XMLEnum == NULL)
		NullPointer = TRUE;

	else
		NullPointer = FALSE;

	f.Write (NullPointer);

	if (!NullPointer)
		XMLEnum->SaveAll (f);
}


void RGXMLSimpleEnumerationType :: SaveAll (RGVOutStream& f) const {

	Boolean NullPointer;
	
	RGXMLSimpleType::SaveAll (f);

	if (XMLEnum == NULL)
		NullPointer = TRUE;

	else
		NullPointer = FALSE;

	f << NullPointer;

	if (!NullPointer)
		XMLEnum->SaveAll (f);
}


Boolean RGXMLSimpleEnumerationType :: SetEnumeration (RGXmlDataEnumeration* xmlEnum) {

	delete XMLEnum;
	XMLEnum = xmlEnum;
	return TRUE;
}


RGString RGXMLSimpleEnumerationType :: GetStringValue (unsigned long index) {

	if (XMLEnum == NULL)
		return RGString ();
	
	return XMLEnum->GetStringValue (index);
}


RGPScalar RGXMLSimpleEnumerationType :: GetScalarValue (unsigned long index) {

	if (XMLEnum == NULL)
		return RGPScalar ();
	
	return XMLEnum->GetScalarValue (index);
}



int RGXMLSimpleEnumerationType :: Initialize () {

	if (XMLEnum == NULL)
		return -1;
	
	return XMLEnum->Initialize (XMLBase);  // Modify this to add in capability to search for base data type
}


int RGXMLSimpleEnumerationType :: Test (const RGPScalar& p) const {

	if (XMLEnum == NULL)
		return -1;
	
	return XMLEnum->Test (p);
}


int RGXMLSimpleEnumerationType :: Test (const RGPScalar* ptr) const {

	if (XMLEnum == NULL)
		return -1;
	
	return XMLEnum->Test (ptr);
}


unsigned long RGXMLSimpleEnumerationType :: NumberOfItems () const {

	if (XMLEnum == NULL)
		return 0;
	
	return XMLEnum->NumberOfItems ();
}



int RGXMLSimpleEnumerationType :: PrintResults (const RGString& Indent) {

	RGXMLSimpleType::PrintResults (Indent);

	if (XMLEnum == NULL)
		return -1;

	RGString Indent2 = Indent + "    ";

	unsigned long i;
	unsigned long L = XMLEnum->NumberOfItems ();
	XMLEnum->Initialize (XMLBase);
	RGString value;
	cout << Indent << "Enumeration values (total number = " << L << "):" << endl;

	for (i=0; i<L; i++) {

		value = XMLEnum->GetStringValue (i);
		cout << Indent2 << value << endl;
	}

	return 0;
}


RGXMLRestrictedSimpleType :: RGXMLRestrictedSimpleType () : RGXMLSimpleType () {

	it = new RGDListIterator (RestrictionList);
}


RGXMLRestrictedSimpleType :: RGXMLRestrictedSimpleType (const RGString& name) : RGXMLSimpleType (name) {

	it = new RGDListIterator (RestrictionList);
}


RGXMLRestrictedSimpleType :: RGXMLRestrictedSimpleType (const RGXMLRestrictedSimpleType& info) : 
RGXMLSimpleType (info), RestrictionList (info.RestrictionList) {

	it = new RGDListIterator (RestrictionList);
}


RGXMLRestrictedSimpleType :: ~RGXMLRestrictedSimpleType () {

	RestrictionList.ClearAndDelete ();
	delete it;
}


RGXMLRestrictedSimpleType& RGXMLRestrictedSimpleType :: operator= (const RGXMLRestrictedSimpleType& info) {

	if (this == &info)
		return *this;

	*(RGXMLSimpleType*)this = (RGXMLSimpleType&) info;
	RestrictionList = info.RestrictionList;
	delete it;
	it = new RGDListIterator (RestrictionList);
	return *this;
}


size_t RGXMLRestrictedSimpleType :: StoreSize () const {

	size_t size = RGXMLSimpleType::StoreSize ();
	size += RestrictionList.StoreSize ();
	return size;
}


void RGXMLRestrictedSimpleType :: RestoreAll (RGFile& f) {

	RGXMLSimpleType::RestoreAll (f);
	RestrictionList.RestoreAll (f);
}


void RGXMLRestrictedSimpleType :: RestoreAll (RGVInStream& f) {

	RGXMLSimpleType::RestoreAll (f);
	RestrictionList.RestoreAll (f);
}


void RGXMLRestrictedSimpleType :: SaveAll (RGFile& f) const {

	RGXMLSimpleType::SaveAll (f);
	RestrictionList.SaveAll (f);
}


void RGXMLRestrictedSimpleType :: SaveAll (RGVOutStream& f) const {

	RGXMLSimpleType::SaveAll (f);
	RestrictionList.SaveAll (f);
}


	
Boolean RGXMLRestrictedSimpleType :: AddRestriction (RGXmlDataRestriction* restriction) {

	// uses pointer restriction without creating a new one; takes responsibility for delete
	// once it adds to list.

	RestrictionList.Append (restriction);
	return TRUE;
}


Boolean RGXMLRestrictedSimpleType :: AddRestrictionList (RGDList& list) {

	// empties out list - assumes that it is a temporary stack
	
	RGXmlDataRestriction* NextRestriction;

	while (NextRestriction = (RGXmlDataRestriction*)list.GetFirst ()) {

		RestrictionList.Append (NextRestriction);
	}

	return TRUE;
}


Boolean RGXMLRestrictedSimpleType :: ResetRestrictions () {

	it->Reset ();
	return TRUE;
}


RGXmlDataRestriction* RGXMLRestrictedSimpleType :: GetNextRestriction () {

	return (RGXmlDataRestriction*)(*it)();
}


int RGXMLRestrictedSimpleType :: NumberOfRestrictions () const {

	return RestrictionList.Entries ();
}


Boolean RGXMLRestrictedSimpleType :: SetAttributes (const RGXMLAttributeList* list) {

	Boolean Success = RGXMLSimpleType::SetAttributes (list);
	return Success;
}


int RGXMLRestrictedSimpleType :: PrintResults (const RGString& Indent) {

	return RGXMLSimpleType::PrintResults (Indent);  // augment when we know how to print restrictions!!!
}



RGXMLAttributeList :: RGXMLAttributeList () : isList (FALSE) {

}


RGXMLAttributeList :: RGXMLAttributeList (const RGXMLAttributeList& al) :
NameAttribute (al.NameAttribute),
TypeAttribute (al.TypeAttribute),
MaxOccursAttribute (al.MaxOccursAttribute),
SubstitutionGroupAttribute (al.SubstitutionGroupAttribute),
AbstractAttribute (al.AbstractAttribute),
SelectorAttribute (al.SelectorAttribute),
FieldAttribute (al.FieldAttribute),
IncludePathAttribute (al.IncludePathAttribute),
KeyReferenceAttribute (al.KeyReferenceAttribute),
BaseAttribute (al.BaseAttribute),
isList (al.isList),
XPathStack (al.XPathStack) {

}


RGXMLAttributeList :: ~RGXMLAttributeList () {

	XPathStack.ClearAndDelete ();
}

Boolean RGXMLAttributeList :: AddAttribute (RGAttributeType type, const RGString& value) {
	
	// returns TRUE if successful (no multiple name, etc)

	Boolean returnValue = FALSE;
	RGString* newXValue;

	switch (type) {

		case RGNAME:

			if (NameAttribute.Length() == 0) {

				NameAttribute = value;
				returnValue = TRUE;
			}

			break;
			
		case RGTYPE:

			if (TypeAttribute.Length() == 0) {

				TypeAttribute = value;
				returnValue = TRUE;
			}

			break;
			
		case RGMAXOCCURS:

			if (MaxOccursAttribute.Length() == 0) {

				MaxOccursAttribute = value;
				returnValue = TRUE;
			}

			if (value == "unbounded")
				isList = TRUE;

			break;
			
		case RGSUBSTITUTIONGROUP:

			if (SubstitutionGroupAttribute.Length() == 0) {

				SubstitutionGroupAttribute = value;
				returnValue = TRUE;
			}

			break;
			
		case RGREFERENCE:

			if (TypeAttribute.Length() == 0) {

				//
				// Normally, a reference has the same name as the element referred to; this would mess us up
				// because it means that the name = type, so prepend a "reference character" which will 
				// act as a signal that the element is a reference and keep it from being retrieved by name
				// when we are searching for the original "type".  
				//

				TypeAttribute = value;
				NameAttribute = RGReferenceCharacter + value;
				returnValue = TRUE;
			}

			break;
			
		case RGABSTRACT:

			if (AbstractAttribute.Length() == 0) {

				AbstractAttribute = value;
				returnValue = TRUE;
			}

			break;
					  
		case RGSELECTOR:

			if (SelectorAttribute.Length() == 0) {

				SelectorAttribute = value;
				returnValue = TRUE;
			}

			break;
			
		case RGFIELD:

			if (FieldAttribute.Length() == 0) {

				FieldAttribute = value;
				returnValue = TRUE;
			}

			break;
			
		case RGINCLUDEPATH:

			if (IncludePathAttribute.Length() == 0) {

				IncludePathAttribute = value;
				returnValue = TRUE;
			}

			break;
			
		case RGKEYREFERENCE:

			if (KeyReferenceAttribute.Length() == 0) {

				KeyReferenceAttribute = value;
				returnValue = TRUE;
			}

			break;
			
		case RGXPATH:

			newXValue = new RGString (value);
			XPathStack.Prepend (newXValue);
			returnValue = TRUE;
			break;

		case RGBASE:

			if (BaseAttribute.Length () == 0) {

				BaseAttribute = value;
				returnValue = TRUE;
			}

			break;

		default:
			break;
	}

	return returnValue;
}


Boolean RGXMLAttributeList :: AddAttribute (RGAttributeType type, const RGString* value) {

	return AddAttribute (type, *value);
}


int RGXMLAttributeList :: GetNameAndType (RGString& name, RGString& type) const {
	
	// returns -10 if name is empty, 0 otherwise

	if (NameAttribute.IsEmpty ()) {

		return -10;
	}

	// NameAttribute is not empty

	name = NameAttribute;
	type = TypeAttribute;

	return 0;
}


int RGXMLAttributeList :: GetNameAndBase (RGString& name, RGString& base) const {

	// returns -10 if name is empty, 0 otherwise

	if (NameAttribute.IsEmpty ()) {

		return -10;
	}

	// NameAttribute is not empty

	name = NameAttribute;
	base = BaseAttribute;

	return 0;
}


int RGXMLAttributeList :: GetIncludePath (RGString& path) const {

	path = IncludePathAttribute;
	return 0;
}


RGString RGXMLAttributeList :: GetName () const {

	return NameAttribute;
}


RGString RGXMLAttributeList :: GetBase () const {

	return BaseAttribute;
}


RGString RGXMLAttributeList :: GetType () const {

	return TypeAttribute;
}


Boolean RGXMLAttributeList :: GetSubstitutionGroup (RGString& name) const {

	if (SubstitutionGroupAttribute.Length () == 0)
		return FALSE;

	name = SubstitutionGroupAttribute;
	return TRUE;
}


Boolean RGXMLAttributeList :: IsList () const {

	return isList;
}


Boolean RGXMLAttributeList :: IsTerminalType () const {

	if (NameAttribute.IsEmpty ())
		return FALSE;

	if (TypeAttribute.IsEmpty ())
		return TRUE;

	if (NameAttribute == TypeAttribute)
		return TRUE;

	return FALSE;
}


Boolean RGXMLAttributeList :: IsAbstract () const {

	if (AbstractAttribute == "true")
		return TRUE;

	return FALSE;
}

int RGXMLAttributeList :: SetTypeFromAttributeList (const RGXMLAttributeList* list) {

	TypeAttribute = list->TypeAttribute;
	return 0;
}


int RGXMLAttributeList :: SetBaseFromAttributeList (const RGXMLAttributeList* list) {

	BaseAttribute = list->BaseAttribute;
	return 0;
}


RGString* RGXMLAttributeList :: GetFirstXPath () {

	return (RGString*) XPathStack.GetFirst ();
}


size_t RGXMLAttributeList :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += NameAttribute.StoreSize () + TypeAttribute.StoreSize () + MaxOccursAttribute.StoreSize ();
	size += SubstitutionGroupAttribute.StoreSize ();
	size += AbstractAttribute.StoreSize () + SelectorAttribute.StoreSize () + FieldAttribute.StoreSize ();
	size += IncludePathAttribute.StoreSize () + KeyReferenceAttribute.StoreSize ();
	size += sizeof (isList);
	size += XPathStack.StoreSize ();
	return size;
}


int RGXMLAttributeList :: CompareTo (const RGPersistent* p) const {

	RGXMLAttributeList* q = (RGXMLAttributeList*) p;
	return NameAttribute.CompareTo (&q->NameAttribute);
}


unsigned RGXMLAttributeList :: HashNumber (unsigned long Base) const {

	return NameAttribute.HashNumber (Base);
}


Boolean RGXMLAttributeList :: IsEqualTo (const RGPersistent* p) const {

	RGXMLAttributeList* q = (RGXMLAttributeList*) p;
	return NameAttribute.IsEqualTo (&q->NameAttribute);
}


void RGXMLAttributeList :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	NameAttribute.RestoreAll (f);
	TypeAttribute.RestoreAll (f);
	MaxOccursAttribute.RestoreAll (f);
	SubstitutionGroupAttribute.RestoreAll (f);
	AbstractAttribute.RestoreAll (f);
	SelectorAttribute.RestoreAll (f);
	FieldAttribute.RestoreAll (f);
	IncludePathAttribute.RestoreAll (f);
	KeyReferenceAttribute.RestoreAll (f);

	f.Read (isList);
	XPathStack.RestoreAll (f);
}


void RGXMLAttributeList :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
	NameAttribute.RestoreAll (f);
	TypeAttribute.RestoreAll (f);
	MaxOccursAttribute.RestoreAll (f);
	SubstitutionGroupAttribute.RestoreAll (f);
	AbstractAttribute.RestoreAll (f);
	SelectorAttribute.RestoreAll (f);
	FieldAttribute.RestoreAll (f);
	IncludePathAttribute.RestoreAll (f);
	KeyReferenceAttribute.RestoreAll (f);

	f >> isList;
	XPathStack.RestoreAll (f);
}


void RGXMLAttributeList :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	NameAttribute.SaveAll (f);
	TypeAttribute.SaveAll (f);
	MaxOccursAttribute.SaveAll (f);
	SubstitutionGroupAttribute.SaveAll (f);
	AbstractAttribute.SaveAll (f);
	SelectorAttribute.SaveAll (f);
	FieldAttribute.SaveAll (f);
	IncludePathAttribute.SaveAll (f);
	KeyReferenceAttribute.SaveAll (f);

	f.Write (isList);
	XPathStack.SaveAll (f);
}


void RGXMLAttributeList :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
	NameAttribute.SaveAll (f);
	TypeAttribute.SaveAll (f);
	MaxOccursAttribute.SaveAll (f);
	SubstitutionGroupAttribute.SaveAll (f);
	AbstractAttribute.SaveAll (f);
	SelectorAttribute.SaveAll (f);
	FieldAttribute.SaveAll (f);
	IncludePathAttribute.SaveAll (f);
	KeyReferenceAttribute.SaveAll (f);

	f << isList;
	XPathStack.SaveAll (f);
}



RGXMLSchemaSummary :: RGXMLSchemaSummary () : MostRecent (-1), 
FilesScanned (NULL), ResultList (NULL),
CurrentAttributeList (NULL) {

}


RGXMLSchemaSummary :: RGXMLSchemaSummary (const RGString& schemaPath) : SchemaPath (schemaPath), MostRecent (-1),
FilesScanned (NULL), ResultList (NULL),
CurrentAttributeList (NULL) {

	size_t startPosition, endPosition, fixedEndPosition;
	Boolean Found = SchemaPath.FindLastSubstring (".xsd", startPosition, fixedEndPosition);

	if (Found) {

		if (fixedEndPosition == SchemaPath.Length () - 1) {

			endPosition = fixedEndPosition;
			BinaryRestorePath = XMLRestorePath = SchemaPath;
			BinaryRestorePath.ReplaceSubstring (startPosition, endPosition, ".brx");
			endPosition = fixedEndPosition;
			XMLRestorePath.ReplaceSubstring (startPosition, endPosition, ".xrx");
		}
	}
}


RGXMLSchemaSummary :: ~RGXMLSchemaSummary () {

	AttributeStack.ClearAndDelete ();
	XPathStack.ClearAndDelete ();
	LocalSchemasNeeded.ClearAndDelete ();
	SchemaDataResults.ClearAndDelete ();
	IncludedSchemaSummaries.ClearAndDelete ();
	FilesAlreadyDone.ClearAndDelete ();
	AttributeData.ClearAndDelete ();
	SequencedElementsStack.ClearAndDelete ();
	NestedElementNameStack.ClearAndDelete ();
	ListElementStack.ClearAndDelete ();

	delete CurrentAttributeList;

	//  Add list(s) for key/keyReference info

//	RGDList* FilesScanned;
//	RGDList* ResultList;
}


int RGXMLSchemaSummary :: SetSchemaPath (const RGString& schemaPath) {

	SchemaPath = schemaPath;

	size_t startPosition, endPosition, fixedEndPosition;
	Boolean Found = SchemaPath.FindLastSubstring (".xsd", startPosition, fixedEndPosition);

	if (Found) {

		if (fixedEndPosition == SchemaPath.Length () - 1) {

			BinaryRestorePath = XMLRestorePath = SchemaPath;
			endPosition = fixedEndPosition;
			BinaryRestorePath.ReplaceSubstring (startPosition, endPosition, ".brx");
			endPosition = fixedEndPosition;
			XMLRestorePath.ReplaceSubstring (startPosition, endPosition, ".xrx");
			return 0;
		}
	}

	return -1;
}

int RGXMLSchemaSummary :: LinkSchemaTable () {
	
	// Do we need this???

	return -1;
}


RGXMLAttributeList* RGXMLSchemaSummary :: GetCurrentAttributeList () {

	if (CurrentAttributeList == NULL)
		CurrentAttributeList = new RGXMLAttributeList ();

	return CurrentAttributeList;
}


void RGXMLSchemaSummary :: SaveCurrentAttributes () {

	if (CurrentAttributeList != NULL) {
	
		AttributeStack.Prepend (CurrentAttributeList);
		CurrentAttributeList = NULL;
	}
}


void RGXMLSchemaSummary :: ClearAttributes () {

	AttributeStack.ClearAndDelete ();
}


size_t RGXMLSchemaSummary :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += SchemaPath.StoreSize () + BinaryRestorePath.StoreSize () + XMLRestorePath.StoreSize ();
	size += LocalSchemasNeeded.StoreSize () + SchemaDataResults.StoreSize ();
	size += IncludedSchemaSummaries.StoreSize ();
	return size;
}


int RGXMLSchemaSummary :: CompareTo (const RGPersistent* p) const {

	RGXMLSchemaSummary* q = (RGXMLSchemaSummary*) p;
	return SchemaPath.CompareTo (&q->SchemaPath);
}


unsigned RGXMLSchemaSummary :: HashNumber (unsigned long Base) const {

	return SchemaPath.HashNumber (Base);
}


Boolean RGXMLSchemaSummary :: IsEqualTo (const RGPersistent* p) const {

	RGXMLSchemaSummary* q = (RGXMLSchemaSummary*) p;
	return SchemaPath.IsEqualTo (&q->SchemaPath);
}


void RGXMLSchemaSummary :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	SchemaPath.RestoreAll (f);
	BinaryRestorePath.RestoreAll (f);
	XMLRestorePath.RestoreAll (f);
	LocalSchemasNeeded.RestoreAll (f);
	SchemaDataResults.RestoreAll (f);
	IncludedSchemaSummaries.RestoreAll (f);
}


void RGXMLSchemaSummary :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
	SchemaPath.RestoreAll (f);
	BinaryRestorePath.RestoreAll (f);
	XMLRestorePath.RestoreAll (f);
	LocalSchemasNeeded.RestoreAll (f);
	SchemaDataResults.RestoreAll (f);
	IncludedSchemaSummaries.RestoreAll (f);
}


void RGXMLSchemaSummary :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	SchemaPath.SaveAll (f);
	BinaryRestorePath.SaveAll (f);
	XMLRestorePath.SaveAll (f);
	LocalSchemasNeeded.SaveAll (f);
	SchemaDataResults.SaveAll (f);
	IncludedSchemaSummaries.SaveAll (f);
}


void RGXMLSchemaSummary :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
	SchemaPath.SaveAll (f);
	BinaryRestorePath.SaveAll (f);
	XMLRestorePath.SaveAll (f);
	LocalSchemasNeeded.SaveAll (f);
	SchemaDataResults.SaveAll (f);
	IncludedSchemaSummaries.SaveAll (f);
}



int RGXMLSchemaSummary :: ChooseDataMethod () {
	
	// -1 = Scan Schema; +1 = binary restore; 0 = XML restore?

	int SchemaTime;
	int BinaryTime;
	int XMLTime;
	int order = 3;

	SchemaTime = RGExec::FileModificationTime (SchemaPath);
	BinaryTime = RGExec::FileModificationTime (BinaryRestorePath);
	XMLTime = RGExec::FileModificationTime (XMLRestorePath);

	if ((SchemaTime >= BinaryTime) && (SchemaTime >= XMLTime))
		order = 0;

	else if ((BinaryTime >= SchemaTime) && (BinaryTime >= XMLTime))
		order = 1;

	else if ((XMLTime >= SchemaTime) && (XMLTime >= BinaryTime))
		order = 2;

	switch (order) {

		case 0:
			if (SchemaTime == -1)
				return -2;

			return -1;

		case 1:
			if (BinaryTime == -1)
				return -2;

			return 1;

		case 2:
			if (XMLTime == -1)
				return -2;

			return 0;

		case 3:
			break;
	}

	return -2;
}


int RGXMLSchemaSummary :: LinkSchemaTable (RGDList& FilesAlreadyScanned, RGDList& Results) {
	
	// uses most efficient method depending on file modification dates

	return -1;

}


int RGXMLSchemaSummary :: PrintResults () {

	RGDListIterator it (SchemaDataResults);
	RGXMLType* type;

	while (type = (RGXMLType*)it())
		type->PrintResults ("    ");

	return 0;
}


Boolean RGXMLSchemaSummary :: LinkSchemaDataWithWarehouseData () {

	RGWarehouse* wh = new RGWarehouse;
	Boolean Success = wh->LinkXMLClasses (SchemaDataResults);

	if (RGXMLSchemaSummary::DebugFlag) {

		cout << endl << endl;
		cout << "      Results of Linking" << endl;
		wh->PrintXMLLinks ();
	}

	cout << endl << endl;

	if (RGXMLSchemaSummary::DebugFlag)
		wh->PrintXMLNames ();

	delete wh;
	return Success;
}


void RGXMLSchemaSummary :: SetDebugOn () {

	DebugFlag = TRUE;
}


void RGXMLSchemaSummary :: SetDebugOff () {

	DebugFlag = FALSE;
}


