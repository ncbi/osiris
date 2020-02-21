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
*  FileName: FailureMessages.cpp
*  Author:   Robert Goor
*
*/
//
//     class FailureMessages encapsulates the general failure messages for circumstances in which an output
//           window may not appear
//

#include "FailureMessages.h"
#include "xmlwriter.h"
#include <list>
#include <iostream>
#include <time.h>
#include <stdio.h>

using namespace std;


void MainMessages:: InputFileUnreadable (const RGString& fileName) {

	Header ();
	cout << "Base command input file named " << XML (fileName) << "unreadable.";
	Tail ();
}


void MainMessages :: InputStringFormatError () {

	RGString* nextMessage;
	Header ();
	cout << "Parsing input file terminated prematurely due to format error in string:";

	while (mMessages.Entries () > 0) {

		nextMessage = (RGString*)mMessages.GetFirst ();
		cout << "\n" << XML (*nextMessage);
		delete nextMessage;
	}

	Tail ();
}


void MainMessages :: PrematureFileEnd () {

	Header ();
	cout << "Base input file had unexpected end of file.";
	Tail ();
}


void MainMessages::IncompleteSetOfInputs () {

	RGString* nextMessage;
	Header ();
	cout << "Base input file:  incomplete set of inputs or could not open Lab Settings.";

	while (mMessages.Entries () > 0) {

		nextMessage = (RGString*)mMessages.GetFirst ();
		cout << "\n" << XML (*nextMessage);
		delete nextMessage;
	}

	Tail ();
}



// Input file failure (lab settings standard settings, etc)
void MainMessages::CouldNotOpenFile (const RGString& fileType) {

	Header ();
	cout << "Could not open " << XML (fileType) << " file.";
	Tail ();
}



void MainMessages::FileInvalid (const RGString& fileType) {
	
	Header ();
	cout << "Could not read " << XML (fileType) << ".  File format errors.";
	Tail ();
}



void MainMessages::OutputFileCouldNotBeCreated (const RGString& fullPathName) {

	Header ();
	cout << "Output file named " << XML (fullPathName) << " could not be created.";
	Tail ();
}



// Analysis failure
void MainMessages::CouldNotInitialize () {

	RGString* nextMessage;
	Header ();
	cout << "Could not initialize:";

	while (mMessages.Entries () > 0) {

		nextMessage = (RGString*)mMessages.GetFirst ();
		cout << "\n" << XML (*nextMessage);
		delete nextMessage;
	}

	Tail ();
}



void MainMessages::LadderBasedAnalysisFailed () {

	RGString* nextMessage;
	Header ();
	cout << "Ladder-based Analysis Failed:";

	while (mMessages.Entries () > 0) {

		nextMessage = (RGString*)mMessages.GetFirst ();
		cout << "\n" << XML (*nextMessage);
		delete nextMessage;
	}

	Tail ();
}



void MainMessages::FragmentAnalysisFailed () {

	RGString* nextMessage;
	Header ();
	cout << "Fragment Analysis Failed:";

	while (mMessages.Entries () > 0) {

		nextMessage = (RGString*)mMessages.GetFirst ();
		cout << "\n" << XML (*nextMessage);
		delete nextMessage;
	}

	Tail ();
}



// Message functions

void MainMessages::AddMessage (const RGString& message) {

	RGString* newMessage = new RGString (message);
	mMessages.Append (newMessage);
}



void MainMessages::DeleteAllMessages () {

	mMessages.ClearAndDelete ();
}


void MainMessages::Header () {

	cout << "<*****>";
}



void MainMessages::Tail () {

	cout << "  Terminating...</*****>" << endl;
}


RGString MainMessages::XML (const RGString& input) {

	RGString pResult;
	return xmlwriter::EscAscii (input, &pResult);
}

