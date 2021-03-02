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
*  FileName: fsa2XML.cpp
*
*/
#include <memory>
#include "fsa2XML.h"

void fsa2XML::WriteXML(fsaFile *pFsa, RGFile *pFile)
{
  unique_ptr<xmlwriter> XML(new xmlwriter (pFile));
	fsaDirEntry *pEntry;

	XML->CreateTag("AB_Root");
	XML->CreateTag("Header");
	fsaDirEntry *pDir = pFsa->GetDirectoryDirEntry();
	XML->CreateChild ("Version", (int)pFsa->version());
	XML->CreateChild ("Directory_Tag_Name", pDir->Name());
	XML->CreateChild ("Directory_Tag_Number", (int)pDir->Number());
	XML->CreateChild ("Directory_Elements", (int)pDir->NumElements());
	XML->CloseLastTag();
	XML->CreateTag("Data");

	RGString tagTag("Tag");
	RGString tagName("Name");
	RGString tagID("ID");
	RGString tagType("Type");
  RGString tagTypeNumber("TypeNumber");
	RGString tagElements("Elements");
	RGString tagSize("Size");
	RGString tagValue("Value");
  RGString tagNoValue("NoValue");
  RGString tagOffset("FileOffset");

  RGString EMPTY("");
  char s[32];

	int n = pFsa->dirEntryCount();
  pFsa->rewindDirEntries();
	for (int i = 0; i < n; i++)
	{
		pEntry = pFsa->nextDirEntry();
		const RGString& rgs = pEntry->DataToString();
		XML->CreateTag (tagTag);
		XML->CreateChild (tagName, pEntry->Name());
		XML->CreateChild (tagID, pEntry->Number());
		XML->CreateChild (tagType, pEntry->GetTypeString());
    XML->CreateChild (tagTypeNumber,(int)pEntry->ElementTypeEnum());
		XML->CreateChild (tagElements, pEntry->NumElements());
		XML->CreateChildUnsigned (tagSize, (unsigned)pEntry->DataSize());
    sprintf(s,"%08x",(unsigned)pEntry->Offset());
    XML->CreateChild(tagOffset, s);
    if(&rgs != NULL)
    {
		  XML->CreateChild (tagValue, (const char *)rgs);
    }
    else
    {
      XML->CreateChild(tagNoValue,EMPTY);
    }
		XML->CloseLastTag();
	}

	XML->CloseAllTags();
}
