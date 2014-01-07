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
*  FileName: rgwarehouse.h
*  Author:   Robert Goor
*
*/
//
// class RGWarehouse:  arranges that there is only one instance of itself and it collects prototypes of RGPersistent
// class objects for reconstruction on input from a stream or file.  It also builds a linkage between XML tagnames 
// and RGPersistent class object prototypes.  These can then be retrieved easily when performing XML input.
//

#ifndef _RGWAREHOUSE_H_
#define _RGWAREHOUSE_H_

#include <cstdlib>
#include "rgdefs.h"


class RGPersistent;
class RGDList;
class RGHashTable;
class RGHashTableIterator;
class RGSimpleString;
class RGXMLType;
class RGString;

#ifndef NULL
#define NULL    0
#endif


class RGWarehouse {

public:
	RGWarehouse ();
	~RGWarehouse ();

	void* operator new (size_t);
	void operator delete (void*);

	int AddType (RGPersistent* NewType);
	RGPersistent* FindType (int Target);
	RGPersistent* FindXMLType (const RGString& tagname);
	RGPersistent* FindXMLType (const RGSimpleString& tagname);

	RGXMLType* FindXMLPrototype (const RGString& tagname);
	RGXMLType* FindXMLPrototype (const RGSimpleString& tagname);

	static RGXMLType* GlobalFindXMLPrototype (const RGString& tagname);
	static RGXMLType* GlobalFindXMLPrototype (const RGSimpleString& tagname);

	Boolean LinkXMLClasses (RGDList& ScannedSchemaData);  // builds hash table from RGPersistent list and
														// imported list ScannedSchemaData
	
	static int AddClassType (RGPersistent* NewType) { 
		if (OneAndOnlySelf != NULL) 
			return OneAndOnlySelf->AddType (NewType);
		return 0;
	}

	static RGPersistent* TestForID (int Target) {

		if (OneAndOnlySelf != NULL)
			return OneAndOnlySelf->FindType (Target);
		return NULL;
	}

	static void SetDebugOn ();
	static void SetDebugOff ();  // it's off by default
	static RGPersistent* GlobalFindXMLType (const RGString& tagname);
	static RGPersistent* GlobalFindXMLType (const RGSimpleString& tagname);

	void PrintXMLClasses ();
	void PrintXMLLinks ();
	void PrintXMLNames ();
  static void Kill()
  {
    if(OneAndOnlySelf != NULL)
    {
      ReferenceCount = 1; // problem with reference count
      delete OneAndOnlySelf;
      OneAndOnlySelf = NULL;
    }
  }

protected:
	RGHashTable* ClassTypes;
	RGHashTableIterator* TypeIterator;

	RGHashTable* XMLClassAssociations;

	static int ReferenceCount;
	static RGWarehouse* OneAndOnlySelf;
	static Boolean DebugFlag;

	Boolean ResolveXMLType (RGXMLType* Target);
	Boolean ResolveXMLExtension (RGXMLType* Target);
	Boolean ResolveXMLRestriction (RGXMLType* Target);

};


#endif  /*  _RGWAREHOUSE_H_  */
