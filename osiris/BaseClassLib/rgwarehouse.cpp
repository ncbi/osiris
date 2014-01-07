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
*  FileName: rgwarehouse.cpp
*  Author:   Robert Goor
*
*/
//
// class RGWarehouse:  arranges that there is only one instance of itself and it collects instances of RGPersistent
// for reconstruction on input from a stream or file, or from an XML dataset.  Persistent class prototypes are kept
// in a hash table, indexed by id (which is unique to a class) for very fast retrieval with a minimum of searching.
//

#include "rgwarehouse.h"
#include "rgpersist.h"
#include "rgdlist.h"
#include "rghashtable.h"
#include "rgsimplestring.h"
#include "rgxmlschema.h"
#include "rgidwrapper.h"
#include <iostream>

using namespace std;

int RGWarehouse::ReferenceCount = 0;
RGWarehouse* RGWarehouse::OneAndOnlySelf = NULL;
Boolean RGWarehouse::DebugFlag = FALSE;


class RGWarehouseKill
{
public:
  RGWarehouseKill() {;}
  ~RGWarehouseKill()
  {
    RGWarehouse::Kill();
  }
};

static RGWarehouseKill g_kill;

RGWarehouse :: RGWarehouse () {

	if (RGWarehouse::ReferenceCount == 0) {

		ClassTypes = new RGHashTable (503);
		TypeIterator = new RGHashTableIterator (*ClassTypes);
		XMLClassAssociations = NULL;
	}

	RGWarehouse::ReferenceCount++;
}



RGWarehouse :: ~RGWarehouse () {

	RGWarehouse::ReferenceCount--;

	if (RGWarehouse::ReferenceCount <= 0) {

		if (XMLClassAssociations == NULL)
			ClassTypes->ClearAndDelete ();

		else {

			ClassTypes->Clear ();
			XMLClassAssociations->ClearAndDelete ();
			delete XMLClassAssociations;
		}

		delete ClassTypes;
		delete TypeIterator;
	}
}


void* RGWarehouse :: operator new (size_t nbytes) {

	void* p;
	
	if (RGWarehouse::ReferenceCount > 0) {
		
		return RGWarehouse::OneAndOnlySelf;
	}
	
	p = malloc (nbytes);
	RGWarehouse::OneAndOnlySelf = (RGWarehouse*) p;
	return p;
}



void RGWarehouse :: operator delete (void* p) {

	if (RGWarehouse::ReferenceCount <= 0)
		free (p);
}



int RGWarehouse :: AddType (RGPersistent* NewType) {

	RGIDWrapper* type = new RGIDWrapper (NewType);
	RGPersistent* test = ClassTypes->Append (type);
	int val;

	if (RGWarehouse::DebugFlag)
		cout << "Adding class to persistent list, ID = " << type->GetSearchID () << endl;

	if (test != NULL) {

		val = ClassTypes->Entries ();
	}

	else {
		
		val = 0;
	}

	return val;
}



RGPersistent* RGWarehouse :: FindType (int Target) {

	RGIDWrapper* Found;
	RGIDWrapper target;
	target.SetSearchID (Target);

	Found = (RGIDWrapper*)ClassTypes->Find (&target);

	if (RGWarehouse::DebugFlag) {

		cout << "Found class object of type:  " << (Found->GetPointer ())->GetID ();
		cout << ".  Looking for type:  " << Target << endl;
	}

	RGPersistent* ThisIsIt = Found->GetPointer ();
	return ThisIsIt->GetNew ();
}


RGPersistent* RGWarehouse :: FindXMLType (const RGString& tagname) {

	//
	// Assume that the hash table has been fully populated and linked
	//   tagname is a tag found in input XML and we want to associate it
	//   with a C++ class object
	//

	RGXMLType* Prototype = FindXMLPrototype (tagname);

	if (Prototype == NULL)  // No association with this name
		return NULL;

	//
	// Found an association!
	//

	RGPersistent* ThisIsIt = Prototype->GetCppPointer ();
	return ThisIsIt->GetNew ();
}


RGPersistent* RGWarehouse :: FindXMLType (const RGSimpleString& tagname) {

	RGString Tag (tagname);
	return FindXMLType (Tag);
}


RGXMLType* RGWarehouse :: FindXMLPrototype (const RGString& tagname) {

	//
	// Assume that the hash table has been fully populated and linked
	//   tagname is a tag found in input XML and we want to associate it
	//   with a C++ class object
	//
	
	RGXMLType type (tagname);

	if (XMLClassAssociations == NULL)  // well, check the table is not NULL or we crash
		return NULL;

	return (RGXMLType*)XMLClassAssociations->Find (&type);
}


RGXMLType* RGWarehouse :: FindXMLPrototype (const RGSimpleString& tagname) {

	RGString Tag (tagname);
	return FindXMLPrototype (Tag);
}


Boolean RGWarehouse :: LinkXMLClasses (RGDList& ScannedSchemaData) {

	// This function imports all the relevant persistent class types and combines them with scanned
	// names and types from a schema.  Then it resolves all of the name-type pairs so that each name
	// connects to a prototype RGPersistent class object.  That way, an XML nametag can be re-interpreted
	// as a C++ class, which will then read itself in.
	
	RGIDWrapper* NextWrapper;
	RGXMLType* NextType;
	RGXMLType* NextSubType;
	RGXMLType* NewType;
	RGPersistent* NextObject;
	TypeIterator->Reset ();
	RGString SubGroupName;
	RGString Type;
	RGXMLType LookupType;
	Boolean Success;
	RGString TrueTypeName;
	Boolean LocalDebug = RGWarehouse::DebugFlag;

	XMLClassAssociations = new RGHashTable (503); // create with 503 linked lists

	while (NextWrapper = (RGIDWrapper*)(*TypeIterator)()) {

		// Get NextItem->pointer, create an RGXMLType object and insert name and pointer
		// Then, add to XMLClassAssociations

		NextObject = NextWrapper->GetPointer ();
		NextType = new RGXMLType (NextObject->GetXMLType ());
		NextType->SetCppPointer (NextObject);
		XMLClassAssociations->Append (NextType);
	}

	// Now, link in XMLType objects from ScannedSchemaData and complete associations of type with Cpp type (pointer)

	RGDListIterator ScannedIterator (ScannedSchemaData);
	RGDList SubstitutionGroupList;

	while (NextType = (RGXMLType*)ScannedIterator ()) {

		NewType = (RGXMLType*)NextType->Copy ();  // insures that NewType is of the correct subclass of RGXMLType

		//
		//  We want to add the scanned object, but if there is an automatically generated one with the same
		// name, we want to remove it first so we have no duplicates.  We want to use the scanned object because
		// it may contain more detailed structural info (the auto object is bare bones), but we want to transfer
		// the C++ pointer from the auto object first.  Now we can build a universal BaseSchema.xsd that has all
		// the XML tagnames needed to associate with the base library persistent classes.
		//

		NextType = (RGXMLType*)XMLClassAssociations->Remove (NewType); // we don't need the "old" NextType anymore

		if (NextType != NULL) {

			NewType->SetCppPointer (NextType->GetCppPointer ());  // This makes NewType "final" if CppPointer != NULL
			delete NextType;
		}

		XMLClassAssociations->Append (NewType);

		if (NewType->IsAbstract ())
			SubstitutionGroupList.Append (NewType);
	}

	RGHashTableIterator HashIterator (*XMLClassAssociations);

	// here is where we should iterate through the hash table and resolve name and base linkages for 
	// simple types.  We need a function to do that.

	while (NextType = (RGXMLType*)HashIterator()) {

		// Check for several conditions:
		// If NextType is final, continue.  It's probably a complex type.
		// If NextType has no XML base, skip it.  It's an element.
		// If NextType is abstract, it is an element, so skip it.
		// Then, resolve the base-name chain to get a cpp pointer

		if (NextType->IsFinal ())
			continue;

		if (NextType->GetBase ().IsEmpty ())
			continue;

		if (NextType->IsAbstract ())
			continue;

		Success = ResolveXMLRestriction (NextType);  // gets base type of simple restriction

		if (!Success) {

			if (LocalDebug) {
			
				cout << "Could not find base for simple Type:" << endl;
				NextType->PrintResults ("     ");
			}

			SubstitutionGroupList.Clear ();
			return FALSE;   // make sure everything is cleaned up before returning
		}
	}

	// here is where we should iterate through the hash table and resolve name and type linkages.
	// We need a function to do that.

	while (NextType = (RGXMLType*)HashIterator()) {

		// Check for several conditions:  
		// If NextType belongs to a substitution group, look up group name and add (copy of?)  
		//    it to that group in SubstitutionGroupList
		// If NextType is abstract, skip it because its tagname will never appear in XML data
		// If NextType is already "final", ie., has a cppPointer, skip it
		// If NextType has no type, and is not "final", that is an error because it cannot be 
		//    created:  return FALSE
		// Then, resolve the type-name chain to get a cpp pointer

		if (NextType->GetSubstitutionGroupName (SubGroupName)) {

			LookupType.ResetName (SubGroupName);
			NewType = (RGXMLType*)SubstitutionGroupList.Find (&LookupType);

			if (NewType == NULL) {  // Now what?

				NewType = (RGXMLType*)XMLClassAssociations->Find (&LookupType);  // look in the larger list

				if (NewType == NULL) {  // Could not find Substitution Group name anywhere!!
					
					SubstitutionGroupList.Clear ();
					
					if (LocalDebug)
						cout << "Could not find substitution group name " << SubGroupName << endl;

					return FALSE;   // make sure everything is cleaned up before returning
				}

				SubstitutionGroupList.Append (NewType);  // It wasn't abstract; add it to list
			}

			TrueTypeName = NextType->GetTrueName ();
			Success = NewType->AddSubstitutionGroupName (TrueTypeName);  // Adds name, with no duplicates

			if (!Success) {  // This should never happen - function always returns TRUE

				if (LocalDebug) {

					cout << "Could not add substitution group name " << TrueTypeName << " to type:" << endl;
					NewType->PrintResults ("     ");
				}

				SubstitutionGroupList.Clear ();
				return FALSE;   // make sure everything is cleaned up before returning
			}
		}
		
		Success = ResolveXMLType (NextType);  // Tests for abstract, final, and sets pointer in NextType 
											// and for every intermediate object in chain as well

		if (!Success) {

			if (LocalDebug) {
			
				cout << "Could not find type for element:" << endl;
				NextType->PrintResults ("     ");
			}

			SubstitutionGroupList.Clear ();
			return FALSE;   // make sure everything is cleaned up before returning
		}
	}

	//
	// Now start all over at the beginning of the hash list and look for elements that are subelements of
	// other elements.  We should be finding elements only and those should either be references or have 
	// well-defined types.  Even if abstract, they should be references to global elements
	//

	HashIterator.Reset ();

	while (NextType = (RGXMLType*)HashIterator()) {

		//
		// Now look for subelement lists and iterate through those
		//

		if (!NextType->ResetSubElements ())
			continue;

		while (NextSubType = NextType->GetNextSubElement ()) {

			// Now it's just like above:  test for substitution group (all should already be in list)
			//   and then resolve chain

			if (NextType->GetSubstitutionGroupName (SubGroupName)) {

				LookupType.ResetName (SubGroupName);
				NewType = (RGXMLType*)SubstitutionGroupList.Find (&LookupType);

				if (NewType == NULL) {  // Now what?

					if (LocalDebug) {
					
						cout << "Could not find substitution group name " << SubGroupName << " for subelement" << endl;
						NextType->PrintResults ("     ");
					}

					SubstitutionGroupList.Clear ();
					return FALSE;   // make sure everything is cleaned up before returning
				}

				TrueTypeName = NewType->GetTrueName ();
				Success = NewType->AddSubstitutionGroupName (TrueTypeName);  // Adds name, with no duplicates

				if (!Success) {  // This should never happen - function always returns TRUE

					SubstitutionGroupList.Clear ();
					return FALSE;   // make sure everything is cleaned up before returning
				}
			}

			Success = ResolveXMLType (NextType);  // Tests for abstract, final, and sets pointer in NextType 
											// and for every intermediate object in chain as well

			if (!Success) {

				if (LocalDebug) {
				
					cout << "Could not set final pointer for subelement:" << endl;
					NextType->PrintResults ("     ");
				}

				SubstitutionGroupList.Clear ();
				return FALSE;   // make sure everything is cleaned up before returning
			}
		}

		//
		// That should do it!
		//
	}

	//
	// Now start all over at the beginning of the hash list and expand the subelement lists for complex Types
	// that are extensions of other complex Types.  We should be finding complexTypes only.
	//

	HashIterator.Reset ();

	while (NextType = (RGXMLType*)HashIterator()) {

		Success = ResolveXMLExtension (NextType);  //  Tests for already linked with bases' subelements and
		                                    // if not, links subelements all the way up the chain to highest base
		
		if (!Success) {

			if (LocalDebug) {
			
				cout << "Could not link base extension for complex type:" << endl;
				NextType->PrintResults ("     ");
			}

			SubstitutionGroupList.Clear ();
			return FALSE;
		}
	}

	//  Whew!  We made it through the gauntlet and none the worse for wear!  If we got here, we should have 
	// completed everything successfully.  XMLClassAssociations is now a pointer to a hash list of XML names and types
	// that associate with already compiled C++ classes - now ready to be read and written consistent with the 
	// schema files, both specified and included.  If we didn't get here, it is because of duplicates or incomplete
	// schema declarations that did not completely map out the associations between the C++ classes and the XML
	// tagnames, or the schema files themselves did not validate, or else the schema files did not obey the 
	// C++ restrictions specified in the documentation.  In short, these entail that there be no multi-level 
	// declarations.  All complex types must be built up from named complex elements and complex types.  Ultimately,
	// any level of complexity is allowed as long as it is declared one level at a time.

	//  Wow, this function is complicated!

	SubstitutionGroupList.Clear ();
	return TRUE;
}


Boolean RGWarehouse :: ResolveXMLType (RGXMLType* Target) {

	//
	//  Checks for abstract Target and for Target already Final (contains its associated RGPersistent pointer) in
	// which case there is nothing to do.  Otherwise, searches a chain of name/type pairs until it arrives at a 
	// final object, in which case there is a pointer to associate with every link in the chain, including Target
	// itself, or until the chain ends (error - open chain), or until the chain loops back on itself (error - closed
	// chain with no link to RGPersistent objects)
	//

	Boolean LocalDebug = RGWarehouse::DebugFlag;

	if (LocalDebug) {
	
		cout << "Looking for type for:" << endl;
		Target->PrintResults ("    ");
	}

	if (Target->IsAbstract () || Target->IsFinal ())
		return TRUE;

	RGString Type = Target->GetType ();

	if (Type.Length () == 0)
		return FALSE;
	
	RGXMLType type;
	type.ResetName (Type);
	RGDList AlreadySearched;
	RGXMLType* found;
	RGXMLType* NextType;
	RGPersistent* cppPointer;
	AlreadySearched.Append (Target);
	
	while (TRUE) {

		found = (RGXMLType*) XMLClassAssociations->Find (&type);

		if (found == NULL) {

			//
			// No more links:  error - open chain
			//

			if (LocalDebug)
				cout << "Could not find link for type:  " << Type << endl;

			AlreadySearched.Clear ();
			return FALSE;
		}

		if (found->IsFinal ()) {

			//
			// Found final link in chain - now fill in value all the way back
			//
			
			if (LocalDebug)
				cout << "Found link for type:  " << Type << endl;

			cppPointer = found->GetCppPointer ();

			//
			// retrieve each pointer in the chain and reset its Cpp pointer.  This includes
			// Target itself since it was first into the list.
			//

			while (NextType = (RGXMLType*)AlreadySearched.GetFirst ())
				NextType->SetCppPointer (cppPointer);  // This clears list AlreadySearched

			return TRUE;
		}

		else {

			if (AlreadySearched.ContainsReference (found)) {

				//
				// Found circular chain - link points back to a link that was already searched:  error
				//

				if (LocalDebug)
					cout << "Found circular reference to type:  " << Type << endl;

				AlreadySearched.Clear ();
				return FALSE;
			}

			else {

				//
				// Keep looking...set up for next link in chain
				//

				if (LocalDebug) {
				
					cout << "Searching next link from:" << endl;
					found->PrintResults ("        ");
				}

				AlreadySearched.Append (found);
				type.ResetName (found->GetType ());  // The next name in the chain is the previous type
			}
		}
	}

	// Should never get here!

	AlreadySearched.Clear ();
	return FALSE;
}


Boolean RGWarehouse :: ResolveXMLExtension (RGXMLType* Target) {

	//
	//  Checks for Target already linked with extension base (copied base subelements up the chain to itself) in
	// which case there is nothing to do.  Otherwise, searches a chain of name/base pairs until it arrives at a 
	// "final" object (is not an extension of a base type), in which case each type in chain imports subelements
	// up the chain, or until the chain ends because the base cannot be found (error - open chain), or until the 
	// chain loops back on itself (error - closed chain with no final base type to RGPersistent objects)
	//

	Boolean LocalDebug = RGWarehouse::DebugFlag;

	if (LocalDebug) {
	
		cout << "Looking for base for:" << endl;
		Target->PrintResults ("    ");
	}

	if (Target->HasLinkedBaseSubElements ())
		return TRUE;

	RGString Base = Target->GetBase ();

	if (Base.Length () == 0)
		return TRUE;  // There is no base:  this is not an extension of anything
	
	RGXMLType base;
	base.ResetName (Base);
	RGDList AlreadySearched;
	RGXMLType* found;
	RGXMLType* NextType;
	AlreadySearched.Append (Target);
	
	while (TRUE) {

		found = (RGXMLType*) XMLClassAssociations->Find (&base);

		if (found == NULL) {

			//
			// Extension base cannot be found:  error - open chain
			//

			if (LocalDebug)
				cout << "Could not find link for base:  " << Base << endl;

			AlreadySearched.Clear ();
			return FALSE;
		}

		if (found->HasLinkedBaseSubElements ()) {

			//
			// Found final link in chain - now transfer subElements all the way back...
			// Retrieve each pointer in the chain and, using previous pointer, transfer subelements
			// from previous to current.  This includes Target itself since it was first into the list.
			// The "GetFirst" operator clears the list AlreadySearched during the process.
			//
			
			while (NextType = (RGXMLType*)AlreadySearched.GetFirst ()) {

				NextType->CopyAndPrependSubElementList (found);
				found = NextType;  // "found" becomes the "previous" type to the next "NextType" in the chain
			}

			return TRUE;
		}

		else {

			if (AlreadySearched.ContainsReference (found)) {

				//
				//    If schema is valid, this should not happen...
				// Found circular chain - link points back to a link that was already searched:  error
				//

				AlreadySearched.Clear ();
				return FALSE;
			}

			else {

				//
				// Keep looking...set up for next link in chain
				//

				AlreadySearched.Append (found);
				base.ResetName (found->GetBase ());
			}
		}
	}

	// Should never get here!

	AlreadySearched.Clear ();
	return FALSE;
}


Boolean RGWarehouse :: ResolveXMLRestriction (RGXMLType* Target) {

	//
	//  Checks for Target already linked with restriction base in
	// which case there is nothing to do.  Otherwise, searches a chain of name/base pairs until it arrives at a 
	// "final" object (is not an restriction of a base type), in which case each type in chain imports pointer
	// up the chain, or until the chain ends because the base cannot be found (error - open chain), or until the 
	// chain loops back on itself (error - closed chain with no final base type to RGPersistent objects)
	//

	Boolean LocalDebug = RGWarehouse::DebugFlag;
	RGPersistent* Pointer;

	if (LocalDebug) {
	
		cout << "Looking for base for:" << endl;
		Target->PrintResults ("    ");
	}

	if (Target->IsFinal ())
		return TRUE;

	RGString Base = Target->GetBase ();

	if (Base.Length () == 0)
		return TRUE;  // There is no base:  this is not an extension of anything
	
	RGXMLType base;
	base.ResetName (Base);
	RGDList AlreadySearched;
	RGXMLType* found;
	RGXMLType* NextType;
	AlreadySearched.Append (Target);
	
	while (TRUE) {

		found = (RGXMLType*) XMLClassAssociations->Find (&base);

		if (found == NULL) {

			//
			// Restriction base cannot be found:  error - open chain
			//

			if (LocalDebug)
				cout << "Could not find restriction link for base:  " << Base << endl;

			AlreadySearched.Clear ();
			return FALSE;
		}

		if (found->IsFinal ()) {

			//
			// Found final link in chain - now transfer pointers all the way back...
			// Retrieve each pointer in the chain and, using previous pointer, transfer pointer
			// to each.  This includes Target itself since it was first into the list.
			// The "GetFirst" operator clears the list AlreadySearched during the process.
			//

			Pointer = found->GetCppPointer ();
			
			while (NextType = (RGXMLType*)AlreadySearched.GetFirst ())
				NextType->SetCppPointer (Pointer);

			return TRUE;
		}

		else {

			if (AlreadySearched.ContainsReference (found)) {

				//
				//    If schema is valid, this should not happen...
				// Found circular chain - link points back to a link that was already searched:  error
				//

				AlreadySearched.Clear ();
				return FALSE;
			}

			else {

				//
				// Keep looking...set up for next link in chain
				//

				AlreadySearched.Append (found);
				base.ResetName (found->GetBase ());
			}
		}
	}

	// Should never get here!

	AlreadySearched.Clear ();
	return FALSE;
}


void RGWarehouse :: SetDebugOn () {

	DebugFlag = TRUE;
}


void RGWarehouse :: SetDebugOff () {

	DebugFlag = FALSE;
}


RGPersistent* RGWarehouse :: GlobalFindXMLType (const RGString& tagname) {

	//
	// Assume that the hash table has been fully populated and linked
	//   tagname is a tag found in input XML and we want to associate it
	//   with a C++ class object
	//

	if (OneAndOnlySelf == NULL)
		OneAndOnlySelf = new RGWarehouse ();

	return OneAndOnlySelf->FindXMLType (tagname);
}


RGPersistent* RGWarehouse :: GlobalFindXMLType (const RGSimpleString& tagname) {

	if (OneAndOnlySelf == NULL)
		OneAndOnlySelf = new RGWarehouse ();

	return OneAndOnlySelf->FindXMLType (tagname);
}


RGXMLType* RGWarehouse :: GlobalFindXMLPrototype (const RGString& tagname) {

	//
	// Assume that the hash table has been fully populated and linked
	//   tagname is a tag found in input XML and we want to associate it
	//   with a C++ class object
	//

	if (OneAndOnlySelf == NULL)
		OneAndOnlySelf = new RGWarehouse ();

	return OneAndOnlySelf->FindXMLPrototype (tagname);
}


RGXMLType* RGWarehouse :: GlobalFindXMLPrototype (const RGSimpleString& tagname) {

	if (OneAndOnlySelf == NULL)
		OneAndOnlySelf = new RGWarehouse ();

	return OneAndOnlySelf->FindXMLPrototype (tagname);
}


void RGWarehouse :: PrintXMLClasses () {

	TypeIterator->Reset ();
	RGIDWrapper* NextClass;
	RGPersistent* Target;

	while (NextClass = (RGIDWrapper*)(*TypeIterator)()) {

		Target = NextClass->GetPointer ();
		cout << "XML Type:  " << Target->GetXMLType () << endl;
	}
}


void RGWarehouse :: PrintXMLLinks () {

	RGHashTableIterator HashIterator (*XMLClassAssociations);
	RGXMLType* Target;

	while (Target = (RGXMLType*)HashIterator()) {

		Target->PrintResults ("    ");
	}
}


void RGWarehouse :: PrintXMLNames () {

	RGHashTableIterator HashIterator (*XMLClassAssociations);
	RGXMLType* Target;
	cout << "XML Names in Warehouse:" << endl;

	while (Target = (RGXMLType*)HashIterator()) {

		cout << "    " << Target->GetName () << endl;
	}
}

