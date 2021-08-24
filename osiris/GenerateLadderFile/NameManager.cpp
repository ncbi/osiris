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
*  FileName: NameManager.cpp
*  Author:   Robert Goor
*
*/
//
//     Abstract base class ChannelData for retaining data pertaining to one data channel, such as the sampled data
//  associated with that channel
//


#include "NameManager.h"
#include "LocusName.h"
#include "rgfile.h"
#include "rgtokenizer.h"

#include <string>
#include <iostream>

using namespace std;


LocusNames::~LocusNames () {

	LocusName* nextLocus;
	RGString* nextControlName;
	list<LocusName*>::const_iterator c1Iterator;

	for (c1Iterator = mLocusNames.begin (); c1Iterator != mLocusNames.end (); c1Iterator++) {

		nextLocus = *c1Iterator;
		delete nextLocus;
	}

	mLocusNames.clear ();
	list<RGString*>::const_iterator c2Iterator;

	for (c2Iterator = mPositiveControlNames.begin (); c2Iterator != mPositiveControlNames.end (); c2Iterator++) {

		nextControlName = *c2Iterator;
		delete nextControlName;
	}

	mPositiveControlNames.clear ();
}

bool LocusNames::AddLocusNamesAndControlNamesFromControlFile (const RGString& fileName) {

	RGFile positiveControls (fileName, "rt");
	char oneLine [1000];

	if (!positiveControls.isValid ()) {

		cout << "Standard Positive Controls text file named:  " << fileName << " is not readable" << endl;
		return false;
	}

	if (!positiveControls.ReadLine (oneLine)) {

		cout << "Could not read first line of input file.  Exiting..." << endl;
		return false;
	}

	RGStringArray tokens;
	RGStringArray stdControls;
	RGStringArray delimiters;
	RGString lineString = oneLine;
	RGStringTokenizer oneLineTokens (lineString);
	oneLineTokens.AddDelimiter ("\t");
	oneLineTokens.Split (stdControls, delimiters);
	int n = stdControls.Length ();
	RGString* nextControlName;

	if (n < 6) {

		cout << "Not enough columns.  n = " << n << ".  Exiting..." << endl;
		return false;
	}

	cout << "There are " << n - 1 << " named standard controls" << endl;
	cout << "They are:  ";

	int i;

	//  Having read the first line, extract the standard positive control names and add them to the positive control list

	for (i=1; i<n; i++) {

		cout << (char*)stdControls [i].GetData ();
		nextControlName = new RGString (stdControls [i]);
		mPositiveControlNames.push_back (nextControlName);

		if (i < n-1)
			cout << ", ";
	}

	cout << endl;

	// Now read each line for the first token, the locus name, and add to list

	RGString nameOfLocus;
	LocusName* newLocus;
	RGString lcLocusName;

	while (true) {

		if (!positiveControls.ReadLine (oneLine))
			break;

		lineString = oneLine;
		oneLineTokens.ResetTokenizer ();
		oneLineTokens.Split (tokens, delimiters);

		//if (tokens.Length () < n)
		//	break;

		nameOfLocus = tokens [0];
		lcLocusName = nameOfLocus;
		lcLocusName.ToLower ();

		cout << "Locus name:  " << nameOfLocus << " and, LC: " << lcLocusName << "\n";

		if (nameOfLocus.Length () < 1) {

			cout << "Error.  Locus name has zero length.  Exiting..." << endl;
			return false;
		}

		if (nameOfLocus == "AMEL") {

			newLocus = new AMELName ();
			mLocusNames.push_back (newLocus);
		}

		else if (nameOfLocus == "TH01") {

			newLocus = new TH01Name ();
			mLocusNames.push_back (newLocus);
		}

		else if (nameOfLocus == "Penta E") {

			newLocus = new PentaE ();
			mLocusNames.push_back (newLocus);
		}

		else if (nameOfLocus == "Penta D") {

			newLocus = new PentaD ();
			mLocusNames.push_back (newLocus);
		}

		else if (nameOfLocus == "DYS389I") {

			newLocus = new DYS389I ();
			mLocusNames.push_back (newLocus);
		}

		else if (nameOfLocus == "DYS389II") {

			newLocus = new DYS389II ();
			mLocusNames.push_back (newLocus);
		}

		else if (nameOfLocus == "DYS385 a/b") {

			newLocus = new DYS385AB ();
			mLocusNames.push_back (newLocus);
		}

		else if (nameOfLocus == "Y-GATA-H4") {

			newLocus = new YGATAH4 ();
			mLocusNames.push_back (newLocus);
		}

		else {

			newLocus = new LocusName (nameOfLocus);
			mLocusNames.push_back (newLocus);
		}
	}

	return true;
}



RGString LocusNames::FindPositiveControlNameInList (const RGString& name) {
	// returns empty string if not found

	list<RGString*>::const_iterator c2Iterator;
	RGString* nextControlName;
	RGString nameLC = name;
	nameLC.ToLower ();
	RGString storedControlName;
	RGString nullAnswer;

	for (c2Iterator = mPositiveControlNames.begin (); c2Iterator != mPositiveControlNames.end (); c2Iterator++) {

		nextControlName = *c2Iterator;
		storedControlName = *nextControlName;
		storedControlName.ToLower ();

		if (storedControlName == nameLC) {

			cout << "Found positive control name = " << nameLC << endl;
			return *nextControlName;
		}
	}

	return nullAnswer;
}



RGString LocusNames::FindLocusNameInList (const RGString& name) {

	list<LocusName*>::const_iterator c1Iterator;
	LocusName* nextLocus;
	RGString nameLC = name;
	nameLC.ToLower ();
	RGString nullAnswer;

	for (c1Iterator = mLocusNames.begin (); c1Iterator != mLocusNames.end (); c1Iterator++) {

		nextLocus = *c1Iterator;
		
		if (nextLocus->CompareLC (nameLC)) {

			return nextLocus->GetName ();
		}
	}

	return nullAnswer;
}