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
*  FileName: LocusName.cpp
*  Author:   Robert Goor
*
*/
//
//     Abstract base class ChannelData for retaining data pertaining to one data channel, such as the sampled data
//  associated with that channel
//


#include "LocusName.h"


LocusName::LocusName (const RGString& name) : mName (name), mLowerCaseName (name) {

	mLowerCaseName.ToLower ();
}


LocusName :: ~LocusName () {

}


RGString LocusName::GetName () const {

	return mName;
}


bool LocusName::CompareLC (const RGString& testName) {

	RGString name = testName;
	name.ToLower ();

	if (name == mLowerCaseName)
		return true;

	return false;
}


SpecialName::SpecialName (const RGString& name) : LocusName (name) {

}


SpecialName::~SpecialName () {

	mSynonyms.ClearAndDelete ();
}



bool SpecialName::CompareLC (const RGString& testName) {

	RGString name = testName;
	name.ToLower ();
	//cout << "Comparing special name to input name:  " << testName << endl;

	if (name == mLowerCaseName)
		return true;

	RGDListIterator it (mSynonyms);
	RGString* nextSynonym;

	while (nextSynonym = (RGString*)it ()) {

		//cout << "Next synonym = " << (char*)nextSynonym->GetData () << endl;

		if (*nextSynonym == name)
			return true;
	}

	return false;
}


AMELName::AMELName () : SpecialName ("AMEL") {

	RGString* nextString = new RGString ("amelogenin");
	mSynonyms.Append (nextString);
}


TH01Name::TH01Name () : SpecialName ("TH01") {

	RGString* nextString = new RGString ("tho1");
	mSynonyms.Append (nextString);
}


PentaE::PentaE () : SpecialName ("Penta E") {

	RGString* string1 = new RGString ("pentae");
	mSynonyms.Append (string1);
}


PentaD::PentaD () : SpecialName ("Penta D") {

	RGString* string1 = new RGString ("pentad");
	mSynonyms.Append (string1);
}


DYS389I::DYS389I () : SpecialName ("DYS389I") {

	RGString* string1 = new RGString ("dys389 i");
	mSynonyms.Append (string1);
	RGString* string2 = new RGString ("dys389 1");
	mSynonyms.Append (string2);
}


DYS389II::DYS389II () : SpecialName ("DYS389II") {

	RGString* string1 = new RGString ("dys389 ii");
	mSynonyms.Append (string1);
	RGString* string2 = new RGString ("dys389 2");
	mSynonyms.Append (string2);
}


DYS385AB::DYS385AB () : SpecialName ("DYS385 a/b") {

	RGString* string1 = new RGString ("dys385ab");
	mSynonyms.Append (string1);
	RGString* string2 = new RGString ("dys389 ab");
	mSynonyms.Append (string2);
	RGString* string3 = new RGString ("dys385a/b");
	mSynonyms.Append (string3);
}


YGATAH4::YGATAH4 () : SpecialName ("Y-GATA-H4") {

	RGString* string1 = new RGString ("y gata h4");
	mSynonyms.Append (string1);
	RGString* string2 = new RGString ("ygatah4");
	mSynonyms.Append (string2);
	RGString* string3 = new RGString ("ygata");
	mSynonyms.Append (string3);
	RGString* string4 = new RGString ("y gata");
	mSynonyms.Append (string4);
	RGString* string5 = new RGString ("y-gata");
	mSynonyms.Append (string5);
}

