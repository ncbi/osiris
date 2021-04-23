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
#include <iostream>

using namespace std;


int _tmain(int argc, _TCHAR* argv[]) {

	bool debugMode;

#ifdef _DEBUG
	debugMode = true;

#else
	debugMode = false;

#endif
	int select;
	LadderInputFile inputFile (debugMode);
	select = inputFile.ReadFirstLine ();
//	bool success;

	if (select < 0) {

		cout << "Could not read ladder input file named 'LadderInputFile.txt'.  Ending..." << endl;
		return -5;
	}

	if (select == 1) {

		cout << "You have chosen to augment an existing ladder." << endl;
		LadderInputFile* inFileAppend = new LadderInputFile (debugMode);
		int inStatus = inputFile.ReadAllInputsAppend ("LadderInputFileAppend.txt");

		if (inStatus != 0) {

			cout << "Could not read input file named 'LadderInputFileAppend.txt'.  Terminating..." << endl;
			return -1;
		}

		inStatus = inputFile.AssembleInputsAppend ();

		if (inStatus != 0) {

			cout << "File input incomplete.  Terminating..." << endl;
			return -1;
		}

		cout << "File input succeeded.  Continuing..." << endl << endl;

		// create full path names to open files.  First, echo data:
		RGString ILSName = Locus::GetILSFamilyName ();
		cout << "Ladder Directory = " << inputFile.GetLadderDirectory ().GetData () << endl;
		cout << "Bins file name = " << inputFile.GetBinsFileName ().GetData () << endl;  // This is now the full path name of the bins file
		cout << "Output Config Path = " << inputFile.GetOutputConfigDirectoryPath ().GetData () << endl;  // This is the user site directory
		cout << "Ladder file name = " << inputFile.GetLadderFileName ().GetData () << endl;
		cout << "ILS name = " << ILSName.GetData () << endl;

		RGString userSiteDirectory = inputFile.GetOutputConfigDirectoryPath ();
	//	RGString configurationToolsDirectory = userSiteDirectory + "/ConfigurationTools";
	//	RGString ladderGenerationConfigDirectory = configurationToolsDirectory + "/LadderGeneration";
	//	RGString positiveControlsConfigDirectory = configurationToolsDirectory + "/StandardPositiveControl";
		RGString ladderDirectory = userSiteDirectory + "/Config/LadderSpecifications";
		RGString ladderPath = ladderDirectory + "/" + inputFile.GetLadderFileName ();

	//	success = LadderInputFile::AddLocusNamesAndControlNamesFromControlFile (userSiteDirectory + "/ConfigurationTools/StandardPositiveControl/StandardPositiveControlList.txt");

		RGString oldLadderString;
		RGString newLadderString;
		//RGString ladderPath = inputFile.GetOutputConfigDirectoryPath () + "/" + inputFile.GetLadderFileName ();
		RGFile* oldLadderFile = new RGFile (ladderPath, "rt");

		if (!oldLadderFile->isValid ()) {

			cout << "Could not open ladder file named '"<< ladderPath << " for editing.  Exiting..." << endl;
			return -52;
		}

		oldLadderString.ReadTextFile (*oldLadderFile);
		oldLadderFile->Close ();
		delete oldLadderFile;

		RGString binsFullPath = inputFile.GetBinsFileName ();  // this should be full path to bins file

		Bins* binsAppend = new Bins (binsFullPath);

		if (!binsAppend->IsValid ()) {

			cout << "Could not open bins file.  Exiting..." << endl;
			return -54;
		}

		RGDList tempList;
		Ladder* binsLadderAppend = binsAppend->AssembleAllLoci (tempList);

		if (binsLadderAppend == NULL) {

			cout << "Bin file failed" << endl;
			return -25;
		}

		inStatus = binsLadderAppend->AmendLadderData (inputFile, oldLadderString);
		return inStatus;
	}

	int inputStatus = inputFile.ReadAllInputs ("LadderInputFile.txt");
	//	inputFile.OutputAllData ();

	if (inputStatus != 0) {

		cout << "File input failed.  Terminating..." << endl;
		return -1;
	}

	cout << "Ladder Input File:  Kit Name = " << inputFile.GetKitName ().GetData () << endl;
	cout << "Panels file Name = " << inputFile.GetPanelsFileName ().GetData () << endl;

	inputStatus = inputFile.AssembleInputs ();

	if (inputStatus != 0) {

		cout << "File input incomplete.  Terminating..." << endl;
		return -1;
	}

	RGString acceptedOLAlleles = inputFile.GetAcceptedOLAlleles ();
	cout << "File input succeeded.  Continuing..." << endl << endl;

	int nDyes = inputFile.GetNumberOfDyes ();
	int i;

	cout << "Accepted OL Alleles specified:  " << acceptedOLAlleles << endl;

	for (i=1; i<=nDyes; i++)
		cout << "Dye for Channel " << i << " = " << (char*)(inputFile.GetDyeName (i)).GetData () << endl;

	for (i=1; i<=nDyes; i++)
		cout << "Color for Channel " << i << " = " << (inputFile.GetColorName (i)).GetData () << endl;

	RGString userSiteDirectory = inputFile.GetOutputConfigDirectoryPath ();
	RGString configurationToolsDirectory = userSiteDirectory + "/ConfigurationTools";
	RGString ladderGenerationConfigDirectory = configurationToolsDirectory + "/LadderGeneration";
	RGString positiveControlsConfigDirectory = configurationToolsDirectory + "/StandardPositiveControl";

//	success = LadderInputFile::AddLocusNamesAndControlNamesFromControlFile (configurationToolsDirectory + "/StandardPositiveControl/StandardPositiveControlList.txt");

	RGString configDirectoryName = userSiteDirectory + "/Config";
	RGString ladderDirectoryName = configDirectoryName + "/LadderSpecificatons";
	RGString volumeDirectoryName = configDirectoryName + "/Volumes";

	int config = RGDirectory::FileOrDirectoryExists (configDirectoryName);

	if (!config) {

		config = RGDirectory::MakeDirectory (configDirectoryName);

		if (!config) {

			cout << "Could not make directory with path:  " << configDirectoryName << endl;
			cout << "Exiting" << endl;
			return -10;
		}

	}

	config = RGDirectory::FileOrDirectoryExists (configDirectoryName);

	if (!config) {

		config = RGDirectory::MakeDirectory (ladderDirectoryName);

		if (!config) {

			cout << "Could not make directory with path:  " << ladderDirectoryName << endl;
			cout << "Exiting" << endl;
			return -20;
		}
	}

	config = RGDirectory::FileOrDirectoryExists (volumeDirectoryName);

	if (!config) {

		config = RGDirectory::MakeDirectory (volumeDirectoryName);

		if (!config) {

			cout << "Could not make directory with path:  " << volumeDirectoryName << endl;
			cout << "Exiting" << endl;
			return -20;
		}
	}

//	RGString ladderDirectory = userSiteDirectory + "/Config/LadderSpecifications";
	RGString ladderPath = ladderDirectoryName + "/" + inputFile.GetLadderFileName ();

	cout << "Ladder Directory = " << ladderDirectoryName.GetData () << endl;
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

	while (nextString = (RGString*)nameIt ()) {

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
	RGDList doNotExtend = inputFile.DoNotExtends ();
	RGDList qualityLoci = inputFile.GetQualityLoci ();
	RGString locusName;

	while (true) {

		newLocus = panelData.ReadNextLine (inputFile);

		if (newLocus == NULL) {

			if (!panelData.isValid ()) {

				// This is an error
				cout << "Locus name unidentified" << endl;
				return -100;
			}

			break;
		}

		newLocus->SetRelativeHeightInfo (inputFile.LocusNeedsRelativeHeightInfo (newLocus->GetName ()));
		status = panelsLadder->AddLocus (newLocus);

		if (status < 0) {

			cout << "Duplicate locus with name:  " << newLocus->GetName () << endl;
			break;
		}

		locusName = newLocus->GetName ();

		if (doNotExtend.Contains (&locusName))
			newLocus->SetDoNotExtend ();

		if (qualityLoci.Contains (&locusName))
			newLocus->SetQualityLocus (true);
	}

	RGDList ilsList = inputFile.GetILSNameList ();
	RGString* ilsName;
	RGDListIterator ilsIt (ilsList);


	while (ilsName = (RGString*)ilsIt ()) {

		panelsLadder->AddILS (*ilsName);
	}

	panelsLadder->SetSuffix (inputFile.GetSearchString ());
	panelsLadder->SetILSChannel (inputFile.GetILSChannel ());

	int nLoci = panelsLadder->GetNumberOfLoci ();
	cout << "Number of loci from panels file = " << nLoci << endl;

	Bins binData (binsFileFullPath);

	cout << "Now preparing to read bins file and merging loci there with loci in panels file..." << endl;

	if (!binData.IsValid ()) {

		cout << "bins file named " << binsFileFullPath << " is unreadable" << endl;
		return -100;
	}

	Ladder* binsLadder = binData.AssembleAllLoci (doNotExtend);

	if (binsLadder == NULL) {

		cout << "Bin file:  locus assembly failed" << endl;
		return -25;
	}

	binsLadder->MergeThisLadderInto (panelsLadder);

	if (!panelsLadder->TestAllLociMerged ()) {

		cout << "Not all loci were merged" << endl;
		return -35;
	}

	//  Open output file and write ladder
	RGString ladderOutputFileName = inputFile.GetOutputConfigDirectoryPath () + "/Config/LadderSpecifications/" + inputFile.GetLadderFileName ();
	cout << "Writing ladder named " << ladderOutputFileName << endl;
	RGTextOutput ladderOutput (ladderOutputFileName, FALSE);

	if (!ladderOutput.FileIsValid ()) {

		cout << "Could not open ladder output file:  " << ladderOutputFileName.GetData () << endl;
		return -1;
	}

	panelsLadder->OutputTo (ladderOutput, inputFile);

	RGString volumePathName = volumeDirectoryName;
	RGString fsaVolumeDirectory = volumePathName + "/" + inputFile.GetVolumeDirectoryName ();

	cout << "Creating directory with fsa volume name:  " << fsaVolumeDirectory << endl;

	RGDirectory::MakeDirectory (fsaVolumeDirectory);
	RGString hidVolumeDirectory = fsaVolumeDirectory + "_HID";
	RGString defaultVolumeDirectoryName = inputFile.GetVolumeDirectoryName ();
	RGString fsaPrefix = fsaVolumeDirectory + "/" + defaultVolumeDirectoryName;

	RGString accessTextName = ladderGenerationConfigDirectory + "/Generic_access.txt";
	RGString stdSettingsTextName = ladderGenerationConfigDirectory + "/Generic_StdSettings.xml";
	RGString MessageBookTextName = ladderGenerationConfigDirectory + "/Generic_MessageBookV4.0.xml";
	RGString labSettingsTextName = ladderGenerationConfigDirectory + "/Generic_LabSettings.xml";
	RGString hidLabSettingsTextName = ladderGenerationConfigDirectory + "/GenericHid_LabSettings.xml";

	CopyVolumeFile (accessTextName, fsaPrefix + "_access.txt");
	CopyVolumeFile (stdSettingsTextName, fsaPrefix + "_StdSettings.xml");
	CopyVolumeFile (MessageBookTextName, fsaPrefix + "_MessageBookV4.0.xml");

	RGFile labSettingsGenericFile (labSettingsTextName, "rt");

	if (!labSettingsGenericFile.isValid ()) {

		cout << "Could not read generic lab settings named 'Generic_LabSettings.xml' for default volume...exiting" << endl;
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
	RGString insert;
	RGString OLPlaceholder = "<!--AcceptOLs-->";

	if (acceptedOLAlleles.Length () != 0) {

		insert = "<OffLadderAlleles>\n";
		insert << acceptedOLAlleles;
		insert << "      </OffLadderAlleles>";
		labSettingsFileString.FindAndReplaceAllSubstrings (OLPlaceholder, insert);
	}

	labSettingsOutputFile << labSettingsFileString;

	RGString hidPrefix;

	if (inputFile.GetHid ()) {

		RGDirectory::MakeDirectory (hidVolumeDirectory);
		hidPrefix = fsaVolumeDirectory + "_HID/" + defaultVolumeDirectoryName + "_HID";

		CopyVolumeFile (accessTextName, hidPrefix + "_access.txt");
		CopyVolumeFile (stdSettingsTextName, hidPrefix + "_StdSettings.xml");
		CopyVolumeFile (MessageBookTextName, hidPrefix + "_MessageBookV4.0.xml");

		RGFile labSettingsGenericFile (labSettingsTextName, "rt");

		RGFile hidLabSettingsGenericFile (hidLabSettingsTextName, "rt");

		if (!hidLabSettingsGenericFile.isValid ()) {

			cout << "Could not read generic hid lab settings file named 'GenericHid_LabSettings.xml' for default volume...exiting" << endl;
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

		if (insert.Length () != 0) {

			hidLabSettingsFileString.FindAndReplaceAllSubstrings (OLPlaceholder, insert);
		}

		hidLabSettingsOutputFile << hidLabSettingsFileString;
	}

	cout << "Ladder created successfully.  Check and test ladder info file created to validate that all inputs were specified correctly." << endl;
	return 0;
}