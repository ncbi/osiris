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
*  FileName: OsirisInputFile.h
*  Author:   Robert Goor
*
*/
//
//	Main program to generate a ladder file using standard bins and panels files as inputs.  Additional input is provided by 
//	either text file input or input from stdin (e.g., from GUI shell)
//


#ifdef _WINDOWS
#include <tchar.h>
#else
#define _tmain main
#define _TCHAR char
#endif


#include "rgstring.h"
#include "RGTextOutput.h"
#include "rgdirectory.h"
#include "rgtokenizer.h"
#include "rgtokenizer.h"
#include "rgfile.h"
#include "LadderInputFile.h"
#include "rgindexedlabel.h"
#include "LadderStructures.h"
#include "Panels.h"
#include "Bins.h"
#include "rgdirectory.h"


#include <string>
#include <math.h>
#include <list>

using namespace std;


int _tmain(int argc, _TCHAR* argv[]) {

	bool debugMode;

#ifdef _DEBUG
	debugMode = true;

#else
	debugMode = false;

#endif

	LadderInputFile inputFile (debugMode);

	int inputStatus = inputFile.ReadAllInputs ("LadderInputFile.txt");
//	inputFile.OutputAllData ();

	if (inputStatus != 0) {

		cout << "File input failed.  Terminating..." << endl;
		return -1;
	}

	inputStatus = inputFile.AssembleInputs ();

	if (inputStatus != 0) {

		cout << "File input incomplete.  Terminating..." << endl;
		return -1;
	}

	cout << "File input succeeded.  Continuing..." << endl << endl;

	int nDyes = inputFile.GetNumberOfDyes ();
	int i;

	for (i=1; i<=nDyes; i++)
		cout << "Dye for Channel " << i << " = " << (char*)(inputFile.GetDyeName (i)).GetData () << endl;

	for (i=1; i<=nDyes; i++)
		cout << "Color for Channel " << i << " = " << (inputFile.GetColorName (i)).GetData () << endl;

	cout << "Ladder Directory = " << inputFile.GetLadderDirectory ().GetData () << endl;
	cout << "Output Config Path = " << inputFile.GetOutputConfigDirectoryPath ().GetData () << endl;
	cout << "Kit Name = " << inputFile.GetKitName ().GetData () << endl;

	RGString panelsFileFullPath = inputFile.GetLadderDirectory ();
	RGString binsFileFullPath = panelsFileFullPath;
	char last = panelsFileFullPath.GetLastCharacter ();

	if (last != '/')
		panelsFileFullPath += "/";

	panelsFileFullPath += inputFile.GetPanelsFileName ();
	cout << "Panel file full path name = " << panelsFileFullPath.GetData () << endl;

	last = binsFileFullPath.GetLastCharacter ();

	if (last != '/')
		binsFileFullPath += "/";

	binsFileFullPath += inputFile.GetBinsFileName ();
	cout << "Bins file full path name = " << binsFileFullPath.GetData () << endl;

	cout << "Max Expected Alleles (default) = " << inputFile.GetMaxExpectedAllelesPerLocusDefault () << endl;
	cout << "Min Expected Alleles (default) = " << inputFile.GetMinExpectedAllelesPerLocusDefault () << endl;

	if (inputFile.GetYLinkedDefault ())
		cout << "YLinked default = true" << endl << endl;

	else
		cout << "YLinked default = false" << endl << endl;

	RGDList tempList = inputFile.GetILSNameList ();
	RGDListIterator nameIt (tempList);
	RGString* nextString;
	RGString firstILS;
	bool first = true;

	while (nextString = (RGString*) nameIt ()) {

		if (first) {

			firstILS = *nextString;
			first = false;
		}

		cout << "Allowed ILS = " << nextString->GetData () << endl;
	}

	cout << endl;

	Panels panelData (panelsFileFullPath);

	if (!panelData.isValid ())
		return -50;

	Ladder* panelsLadder = new Ladder (inputFile.GetKitName ());
	panelsLadder->SetNumberOfChannels (nDyes);
	
	Locus* newLocus;
	int status = 0;

	while (true) {

		newLocus = panelData.ReadNextLine (inputFile);

		if (newLocus == NULL)
			break;

		status = panelsLadder->AddLocus (newLocus);

		if (status < 0) {

			cout << "Duplicate locus with name:  " << newLocus->GetName () << endl;
			break;
		}
	}

	RGDList ilsList = inputFile.GetILSNameList ();
	RGString* ilsName;
	RGDListIterator ilsIt (ilsList);

	while (ilsName = (RGString*) ilsIt ()) {

		panelsLadder->AddILS (*ilsName);
	}

	panelsLadder->SetSuffix (inputFile.GetSearchString ());
	panelsLadder->SetILSChannel (inputFile.GetILSChannel ());

	int nLoci = panelsLadder->GetNumberOfLoci ();
	cout << "Number of loci from panels file = " << nLoci << endl;

	Bins binData (binsFileFullPath);

	if (!binData.IsValid ())
		return -100;

	Ladder* binsLadder = binData.AssembleAllLoci ();

	if (binsLadder == NULL) {

		cout << "Bin file failed" << endl;
		return -25;
	}

	binsLadder->MergeThisLadderInto (panelsLadder);

	if (!panelsLadder->TestAllLociMerged ()) {

		cout << "Not all loci were merged" << endl;
		return -35;
	}

	//  Open output file and write ladder
	RGString ladderOutputFileName = inputFile.GetOutputConfigDirectoryPath () + "/" + inputFile.GetLadderFileName ();
	RGTextOutput ladderOutput (ladderOutputFileName, FALSE);

	if (!ladderOutput.FileIsValid ()) {

		cout << "Could not open ladder output file:  " << ladderOutputFileName.GetData () << endl;
		return -1;
	}

	panelsLadder->OutputTo (ladderOutput, inputFile);

	RGString volumePathName = inputFile.GetDefaultVolumePath ();
	RGString fsaVolumeDirectory = volumePathName + "/" + inputFile.GetVolumeDirectoryName ();
	RGDirectory::MakeDirectory (fsaVolumeDirectory);
	RGString hidVolumeDirectory = fsaVolumeDirectory + "_HID";
	RGString volumeDirectoryName = inputFile.GetVolumeDirectoryName ();
	RGString fsaPrefix = fsaVolumeDirectory + "/" + volumeDirectoryName;

	CopyVolumeFile ("Generic_access.txt", fsaPrefix + "_access.txt");
	CopyVolumeFile ("Generic_StdSettings.xml", fsaPrefix + "_StdSettings.xml");
	CopyVolumeFile ("Generic_MessageBookV4.0.xml", fsaPrefix + "_MessageBookV4.0.xml");

	RGFile labSettingsGenericFile ("Generic_LabSettings.xml", "rt");

	if (!labSettingsGenericFile.isValid ()) {

		cout << "Could not read generic lab settings for default volume...exiting" << endl;
		return -1;
	}

	RGString labSettingsFileString;
	labSettingsFileString.ReadTextFile (labSettingsGenericFile);
	RGString labOutputName = fsaPrefix + "_LabSettings.xml";

	RGTextOutput labSettingsOutputFile (labOutputName, FALSE);

	if (!labSettingsOutputFile.FileIsValid ()) {

		cout << "Could not create " << labOutputName.GetData () << "...exiting" << endl;
		return -1;
	}

	//  Now do lab settings
	
	labSettingsFileString.FindAndReplaceAllSubstrings ("$VolumeName$", inputFile.GetKitName ());
	labSettingsFileString.FindAndReplaceAllSubstrings ("$StdCtrl$", inputFile.GetStandardPositiveControlName ());
	labSettingsFileString.FindAndReplaceAllSubstrings ("$KitName$", inputFile.GetKitName ());
	labSettingsFileString.FindAndReplaceAllSubstrings ("$LadderFileName$", inputFile.GetLadderFileName ());
	labSettingsFileString.FindAndReplaceAllSubstrings ("$PrimaryILS$", firstILS);
	labSettingsOutputFile << labSettingsFileString;

	RGString hidPrefix;

	if (inputFile.GetHid ()) {

		RGDirectory::MakeDirectory (hidVolumeDirectory);
		hidPrefix = fsaVolumeDirectory + "_HID/" + volumeDirectoryName + "_HID";

		CopyVolumeFile ("Generic_access.txt", hidPrefix + "_access.txt");
		CopyVolumeFile ("Generic_StdSettings.xml", hidPrefix + "_StdSettings.xml");
		CopyVolumeFile ("Generic_MessageBookV4.0.xml", hidPrefix + "_MessageBookV4.0.xml");

		RGFile hidLabSettingsGenericFile ("GenericHid_LabSettings.xml", "rt");

		if (!hidLabSettingsGenericFile.isValid ()) {

			cout << "Could not read generic hid lab settings for default volume...exiting" << endl;
			return -1;
		}

		RGString hidLabSettingsFileString;
		hidLabSettingsFileString.ReadTextFile (hidLabSettingsGenericFile);
		RGString hidLabOutputName = hidPrefix + "_LabSettings.xml";

		RGTextOutput hidLabSettingsOutputFile (hidLabOutputName, FALSE);

		if (!hidLabSettingsOutputFile.FileIsValid ()) {

			cout << "Could not create " << hidLabOutputName.GetData () << "...exiting" << endl;
			return -1;
		}

		//  Now do lab settings
	
		hidLabSettingsFileString.FindAndReplaceAllSubstrings ("$VolumeName$", inputFile.GetKitName ());
		hidLabSettingsFileString.FindAndReplaceAllSubstrings ("$StdCtrl$", inputFile.GetStandardPositiveControlName ());
		hidLabSettingsFileString.FindAndReplaceAllSubstrings ("$KitName$", inputFile.GetKitName ());
		hidLabSettingsFileString.FindAndReplaceAllSubstrings ("$LadderFileName$", inputFile.GetLadderFileName ());
		hidLabSettingsFileString.FindAndReplaceAllSubstrings ("$PrimaryILS$", firstILS);
		hidLabSettingsOutputFile << hidLabSettingsFileString;
	}

	return 0;
}