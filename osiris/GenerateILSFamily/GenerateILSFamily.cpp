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
*  FileName: GenerateILSFamily.cpp
*  Author:   Robert Goor
*
*/
//
//	Main program to generate an ILS file representing a single ILS family using user inputs provided by 
//	either text file input or input from stdin (e.g., from GUI shell)
//


#ifdef _WINDOWS
#include <tchar.h>
#else
#define _tmain main
#define _TCHAR char
#endif


#include "ILSInputData.h"
#include "rgstring.h"
#include "RGTextOutput.h"
#include "rgdirectory.h"
#include "rgtokenizer.h"
#include "rgfile.h"
#include "rgindexedlabel.h"
#include "rgdirectory.h"


#include <string>
#include <math.h>
#include <list>
#include <iostream>

using namespace std;


int _tmain(int argc, _TCHAR* argv[]) {

	bool debugMode;

#ifdef _DEBUG
	debugMode = true;

#else
	debugMode = false;

#endif

	//cout << "Hello world!" << endl;
	cout << "Starting program..." << endl;

	ILSInputData inputFile (debugMode);
	int inputStatus = inputFile.ReadAllInputs ();

	if (inputStatus != 0) {

		cout << "File input failed.  Terminating..." << endl;
		return -1;
	}

	cout << "Read all inputs successfully." << endl;

	inputFile.OutputAllData ();
	inputStatus = inputFile.AssembleInputs ();

	if (inputStatus != 0) {

		cout << "File input incomplete.  Terminating..." << endl;
		return -1;
	}

	RGString XML = inputFile.CreateXMLString ();
	RGString userFiles = inputFile.GetUserFiles ();
	RGString configDirectoryName = userFiles + "/Config";
	RGString volumeDirectoryName = userFiles + "/Config/Volumes";
	RGString ladderSpecsDirectoryName = userFiles + "/Config/LadderSpecifications";
	RGString ilsFamilyFileName = ladderSpecsDirectoryName + "/" + inputFile.GetOutputFileName () + "_ILSInfo.xml";

	int config = RGDirectory::FileOrDirectoryExists (configDirectoryName);

	if (!config) {

		config = RGDirectory::MakeDirectory (configDirectoryName);

		if (!config) {

			cout << "Could not make directory with path:  " << configDirectoryName << endl;
			cout << "Exiting" << endl;
			return -10;
		}

		else
			cout << "Created directory with path:  " << configDirectoryName << endl;

		config = RGDirectory::MakeDirectory (ladderSpecsDirectoryName);

		if (!config) {

			cout << "Could not make directory with path:  " << ladderSpecsDirectoryName << endl;
			cout << "Exiting" << endl;
			return -20;
		}

		else
			cout << "Created directory with path:  " << ladderSpecsDirectoryName << endl;
	}

	else if (!RGDirectory::FileOrDirectoryExists (ladderSpecsDirectoryName)) {

		config = RGDirectory::MakeDirectory (ladderSpecsDirectoryName);

		if (!config) {

			cout << "Could not make directory with path:  " << ladderSpecsDirectoryName << endl;
			cout << "Exiting" << endl;
			return -20;
		}

		else
			cout << "Created directory with path:  " << ladderSpecsDirectoryName << endl;
	}

	RGTextOutput outputFile (ilsFamilyFileName);

	if (!outputFile.FileIsValid ()) {

		cout << "Could not create new file named:  " << ilsFamilyFileName << ".  Terminating." << endl;
	}

	Endl endLine;
	outputFile << XML << endLine;
	cout << "All done!" << endl;
	return 0;
}