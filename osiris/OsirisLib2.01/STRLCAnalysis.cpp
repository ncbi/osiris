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
*  FileName: STRLCAnalysis.cpp
*  Author:   Robert Goor
*
*/
//
//  class STRLCAnalysis has global functions which analyze digital data signals according to specified rules, using the locus-centric classes
//


#include "STRLCAnalysis.h"
#include "fsaFileData.h"
#include "DataSignal.h"
#include "RGTextOutput.h"
#include "rgdirectory.h"
#include "ParameterServer.h"
#include "OsirisMsg.h"
#include "TestCharacteristic.h"
#include "ChannelData.h"
#include "STRChannelData.h"
#include "CoreBioComponent.h"
#include "STRCoreBioComponent.h"
#include "STRGenetics.h"
#include "DirectoryManager.h"
#include "RGLogBook.h"
#include "STRNotices.h"
#include "SynonymList.h"
#include "IndividualGenotype.h"
#include "xmlwriter.h"
#include "SmartMessage.h"
#include "SmartNotice.h"
#include <list>
#include <iostream>
#include <time.h>
#include <stdio.h>

using namespace std;

bool* STRLCAnalysis::InitialMatrix = NULL;
RGString STRLCAnalysis::OverrideString;
RGString STRLCAnalysis::OutputSubDirectory;
RGString STRLCAnalysis::MsgBookBuildTime;


PERSISTENT_DEFINITION (STRLCAnalysis, _STRLCANALYSIS_, "STRLCAnalysis")


bool DoesStringContainOneOfSubset (const RGString& target, RGDList& subset) {

	RGString* nextString;
	RGDListIterator it (subset);
	size_t pos = 0;

	while (nextString = (RGString*) it()) {

		if (target.FindSubstringCaseIndependent (*nextString, pos))
			return true;
	}

	return false;
}


STRLCAnalysis :: STRLCAnalysis () : SmartMessagingObject (), WorkingFile (NULL), mCollection (NULL) {

	InitializeSmartMessages ();
	mValid = false;
}


STRLCAnalysis :: STRLCAnalysis (PopulationCollection* collection, const RGString& parentDirectoryForReports, const RGString& messageBookPath) :
SmartMessagingObject (), mCollection (collection), mParentDirectoryForReports (parentDirectoryForReports), WorkingFile (NULL) {

	RGString MessageBookName = messageBookPath + "/OsirisMessageBook2.xml";
	RGFile messages (MessageBookName, "rt");
	mValid = false;

	if (!messages.isValid ()) {

		cout << "Could not open message book...exiting" << endl;
		return;
	}

	NoticeBook book (messages);

	if (!book.IsValid ()) {

		cout << "Could not read messages in book...exiting" << endl;
		return;
	}

	NoticeWarehouse* warehouse = new NoticeWarehouse;

	if (warehouse->InitializeAll (book) < 0) {

		cout << "Could not initialize notices...exiting" << endl;
		return;
	}

	InitializeSmartMessages ();
	mValid = true;
}



STRLCAnalysis :: ~STRLCAnalysis () {

	NewNoticeList.ClearAndDelete ();
	mBaseLocusList.Clear ();

	if (WorkingFile != NULL) {

		WorkingFile->Write (":end:\n");
		WorkingFile->Flush ();
		WorkingFile->Close ();
	}

	if (WorkingFileName.Length () > 0)
		remove (WorkingFileName.GetData ());

	//CleanupSmartMessages ();
}



int STRLCAnalysis :: Analyze (const RGString& prototypeInputDirectory, const RGString& markerSet, int outputLevel, const RGString& graphicsDirectory) {

	Boolean print = TRUE;
/*	SynonymList ControlIDs;
	SynonymList NegCntlIDs;
	SynonymList PosCntlIDs;

	ControlIDs.AddSynonym ("cntl");
	ControlIDs.AddSynonym ("cntrl");
	ControlIDs.AddSynonym ("ctl");
	ControlIDs.AddSynonym ("neg");
	ControlIDs.AddSynonym ("pos");
	ControlIDs.AddSynonym ("(+)");
	ControlIDs.AddSynonym ("(-)");
	ControlIDs.AddSynonym ("amp+");
	ControlIDs.AddSynonym ("blank");

	NegCntlIDs.AddSynonym ("neg");
	NegCntlIDs.AddSynonym ("(-)");
	NegCntlIDs.AddSynonym ("cntl-");
	NegCntlIDs.AddSynonym ("cntrl-");
	NegCntlIDs.AddSynonym ("ctl-");
	NegCntlIDs.AddSynonym ("blank");
	NegCntlIDs.AddSynonym ("ext-");

	PosCntlIDs.AddSynonym ("pos");
	PosCntlIDs.AddSynonym ("(+)");
	PosCntlIDs.AddSynonym ("amp+");
	PosCntlIDs.AddSynonym ("cntl+");
	PosCntlIDs.AddSynonym ("cntrl+");
	PosCntlIDs.AddSynonym ("ctl+");*/

	ParameterServer* pServer = new ParameterServer;
	GenotypeSet* gSet = pServer->GetGenotypeCollection ();

	if (gSet == NULL) {

		cout << "Could not retrieve genotype collection set from parameter server.  Exiting..." << endl;
		return -20;
	}

	GenotypesForAMarkerSet* pGenotypes = gSet->FindMarkerSetCollection (markerSet);

	if (pGenotypes == NULL) {

		cout << "Could not retrieve genotype collection for marker set name " << markerSet << " from parameter server.  Exiting..." << endl;
		return -20;
	}

	Notice* newNotice;
	newNotice = new BlobFound;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	newNotice = new SpikeFound;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	newNotice = new PullUpFound;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	newNotice = new InterLocusPeak;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	newNotice = new PullUpPrimaryChannel;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	Notice::SetMessageTrigger (outputLevel);

	// These are all input
	RGString PrototypeInputDirectory (prototypeInputDirectory);
	mCurrentInputDirectory = prototypeInputDirectory;

	RGString DirectoryName = PrototypeInputDirectory;
	RGDirectory::MakeDirectory (mParentDirectoryForReports);

	RGString markerSetName (markerSet);
	RGString AnalysisDateStr;
	RGString AnalysisTimeStr;
	RGString GelExpirationDateStr;

	RGString FullPathForReports;
	RGString OutputDirectoryBase;
	size_t startPosition = 0;
	size_t endPosition;

	RGDList SampleList;
	RGDList PositiveControlList;
	RGDList NegativeControlList;

	if (PrototypeInputDirectory.GetLastCharacter () == '/')
		PrototypeInputDirectory.ExtractAndRemoveLastCharacters (1);

	if (mParentDirectoryForReports.GetLastCharacter () == '/')
		mParentDirectoryForReports.ExtractAndRemoveLastCharacters (1);

	PrototypeInputDirectory.FindLastSubstring ("/", startPosition, endPosition);
	size_t nChars = PrototypeInputDirectory.Length () - endPosition - 1;
	OutputDirectoryBase = PrototypeInputDirectory.ExtractLastCharacters (nChars);

	FullPathForReports = mParentDirectoryForReports + "/" + OutputDirectoryBase;

	RGDirectory::MakeDirectory (FullPathForReports);

	cout << "Starting..." << endl;

	int i;

	DirectoryManager LadderDirectory (DirectoryName);

	if (!LadderDirectory.IsValid ()) {

		cout << "Could not find ladder directory:  " << DirectoryName << endl;
		cout << "Closing..." << endl;
		return -1;
	}

	else
		cout << "Ladder directory opened successfully" << endl;

	DirectoryManager SampleDirectory (DirectoryName);

	if (!SampleDirectory.IsValid ()) {

		cout << "Could not find sample directory:  " << DirectoryName << endl;
		cout << "Closing..." << endl;
		return -1;
	}

	else
		cout << "Sample directory opened successfully" << endl;

	int ChannelNumber = 1;
	int NSampleFiles = SampleDirectory.GetNumberOfFilesInDirectory ();
	double Progress;
	int SamplesProcessed = 0;

/*	RGString testSampleName;
	cout << "Test of sample name retrieval" << endl << endl;

	while (SampleDirectory.GetNextOrderedSampleFile (testSampleName)) {

		cout << testSampleName << endl;
		SamplesProcessed++;
	}

	cout << "Processed " << SamplesProcessed << " files..." << endl;

	if (true)
		return 0;
*/

	TestCharacteristic* testPeak = new STRTestControlCharacteristic ();
	RGString LaneNumber;
	RGString StartDate;
	RGString StartTime;
	RGString Extension;
	int NumFiles = 0;
	RGString FileNumber;
	int NSamples;
	int status;
	RGString CurrentMarkerSet;

	CoreBioComponent* bioComponent;
	CoreBioComponent* ladderBioComponent;

	RGString FileName;
	RGString FullPathName;  // = DirectoryName + "/" + FileName;
//	int length;
	fsaFileData* data;
	pServer->SetDoubleGaussianSigmaRatio (4.0);

	RGString OutputFileName = OutputDirectoryBase;
	RGString OutputFullPath = FullPathForReports + "/" + OutputFileName + ".tab";
	RGFile OutputFile (OutputFullPath, "wt");
	RGString WholeOutputFileName = OutputFileName + "WholeSample.txt";
	RGString WholeFullPath = FullPathForReports + "/" + WholeOutputFileName;

	RGString SummaryFullPath = FullPathForReports + "/" + OutputFileName + "Summary.tab";
	RGString SummaryFullPathWithLinks = FullPathForReports + "/" + OutputFileName + ".oar";

	RGFile OutputSummary (SummaryFullPath, "wt");
	RGFile OutputSummaryLinks (SummaryFullPathWithLinks, "wt");

	RGString ConsoleOutputFileName = OutputFileName + "ConsoleData.txt";
	RGString WholeConsoleName = FullPathForReports + "/" + ConsoleOutputFileName;
	
	RGTextOutput text (WholeConsoleName, FALSE);
//	RGTextOutput wholeSample (WholeFullPath, FALSE);
	OsirisMsg Message (&OutputFile, "\t", 10);
	Endl endLine;

	if (!OutputFile.isValid ()) {

		cout << "Could not open output file:  " << OutputFullPath << endl;
		cout << "Ending..." << endl;
		return -1;
	}

	if (!text.FileIsValid ()) {

		cout << "Could not open text echo file:  " << WholeConsoleName << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	if (!OutputSummary.isValid ()) {

		cout << "Could not open output summary file:  " << SummaryFullPath << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	if (!OutputSummaryLinks.isValid ()) {

		cout << "Could not open output summary file with embedded links:  " << SummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	cout << "Opened output file:  " << OutputFullPath << " and text echo file:  " << WholeConsoleName << endl;

	RGLogBook ExcelText (&OutputFile, outputLevel, FALSE);
	RGLogBook ExcelSummary (&OutputSummary, outputLevel, FALSE);
	RGLogBook ExcelLinks (&OutputSummaryLinks, outputLevel, FALSE);
	GridDataStruct* GridData = new GridDataStruct (mCollection, markerSetName, testPeak, text, ExcelText, Message, print);
	SampleDataStruct* SampleData = new SampleDataStruct (mCollection, markerSetName, testPeak, testPeak, text, ExcelText, Message, print);
	PopulationMarkerSet* set = mCollection->GetNamedPopulationMarkerSet (markerSetName);

	if (set == NULL) {

		cout << "Could not find marker set named:  " << markerSetName << ".  Ending..." << endl;
		return -10000;
	}

	RGString lsName = set->GetLaneStandardName ();
	text << "Directory:" << endLine;
	text << "    " << DirectoryName.GetData() << endLine;

	ExcelText.SetOutputLevel (1);
	ExcelText << "Directory:" << endLine;
	ExcelText << "    " << DirectoryName.GetData() << endLine;

	pServer->Report (text, "");
	pServer->Report (ExcelText, "");
	ExcelText.ResetOutputLevel ();

	ExcelSummary.SetOutputLevel (1);
//	ExcelSummary << "Directory:" << endLine;
//	ExcelSummary << "    " << DirectoryName.GetData() << endLine << endLine;

	int minSampleRFU = (int)floor (STRSampleChannelData::GetMinRFU () + 0.5);
	int minLadderRFU = (int)floor (STRLadderChannelData::GetMinRFU () + 0.5);
	int minLaneStdRFU = (int)floor (STRLaneStandardChannelData::GetMinRFU () + 0.5);

	ExcelSummary << "OAR 1.1" << endLine;
	ExcelSummary << "Parameters:" << endLine;
	ExcelSummary << "Input Directory:  " << DirectoryName.GetData () << endLine;
	ExcelSummary << "Output Directory:  " << FullPathForReports.GetData () << endLine;
	ExcelSummary << "Kit Name:  " << markerSet << endLine;
	ExcelSummary << "Internal Lane Standard:  " << lsName.GetData () << endLine;
	ExcelSummary << "Minimum RFU:  " << minSampleRFU << "," << minLaneStdRFU << ",";
	ExcelSummary << minLadderRFU << endLine << endLine;
	ExcelSummary.ResetOutputLevel ();

	ExcelLinks.SetOutputLevel (1);
//	ExcelLinks << "Directory:" << endLine;
//	ExcelLinks << "    " << DirectoryName.GetData() << endLine << endLine;

	ExcelLinks << "OAR 1.1" << endLine;
	ExcelLinks << "Parameters:" << endLine;
	ExcelLinks << "Input Directory:  " << DirectoryName.GetData () << endLine;
	ExcelLinks << "Output Directory:  " << FullPathForReports.GetData () << endLine;
	ExcelLinks << "Kit Name:  " << markerSet << endLine;
	ExcelLinks << "Internal Lane Standard:  " << lsName.GetData () << endLine;
	ExcelLinks << "Minimum RFU:  " << minSampleRFU << "," << minLaneStdRFU << ",";
	ExcelLinks << minLadderRFU << endLine << endLine;
	ExcelLinks.ResetOutputLevel ();

	RGString PlotString;

	PlotString << "\t<parameters>\n\t\t<inputDirectory>" << DirectoryName << "</inputDirectory>\n\t\t<outputDirectory>";
	PlotString << FullPathForReports << "</outputDirectory>\n\t\t<kit>" << markerSet << "</kit>\n\t\t<ls>";
	PlotString << lsName << "</ls>\n\t\t<minRFUsample>" << minSampleRFU << "</minRFUsample>\n\t\t<minRFUILS>";
	PlotString << minLaneStdRFU << "</minRFUILS>\n\t\t<minRFUladder>" << minLadderRFU;
	PlotString << "</minRFUladder>\n\t</parameters>\n";

	RGString stringData;

	testPeak->Report (ExcelText, "", "\t");

	int NChannels;

	Boolean cycled;
	RGString LadderFileName;

	RGString PersonNum;
	RGString LineTitle;

	RGString OutputLadderName;
	RGString OutputSampleName;
	RGString FullPathOutputLadderName;
	RGString FullPathOutputSampleName;
	RGString TempString;
	int LadderNumber = 1;
	RGString LadderNumberString;
	RGDList LadderList;
	RGString LadderName;
	PackedTime testTime;
	bool printGraphics = false;

	cout << "Ready to read ladders..." << endl;
	cycled = FALSE;
	RGString Notice;
	RGTextOutput* SampleOutput;
	RGString FitDataName;
	RGString OsirisGraphics (graphicsDirectory);

	if (OsirisGraphics.GetLastCharacter () != '/')
		OsirisGraphics += "/";

/*	if (graphicsDirectory.Length () > 0)
		printGraphics = true;

	else
		printGraphics = false;*/

	RGString GraphicsDirectory = OsirisGraphics + OutputDirectoryBase;

	if (printGraphics)
		RGDirectory::MakeDirectory (GraphicsDirectory);

	while (LadderDirectory.GetNextLadderFile (LadderFileName, cycled) && !cycled) {

		FullPathName = DirectoryName + "/" + LadderFileName;

		if (WorkingFile != NULL) {

			WorkingFile->Write (FullPathName + "\n");
			WorkingFile->Flush ();
		}

		data = new fsaFileData (FullPathName);
		NumFiles++;
		//cout << "Beginning file:  " << NumFiles << endl;

		if (!data->IsValid ()) {

			Notice << "Oops, " << LadderFileName.GetData () << " is not valid...Skipping";
			cout << Notice << endl;
			Notice << "\n";
			ExcelText.Write (1, Notice);
			text << Notice;
			delete data;
			continue;
		}

		ExcelText.SetOutputLevel (1);
		text << endLine << endLine;
		ExcelText << endLine << endLine;

		text << "File Name:" << endLine;
		text << "    " << LadderFileName.GetData () << endLine;

		ExcelText << "File Name:" << endLine;
		ExcelText << "    " << LadderFileName.GetData () << endLine;

		stringData = data->GetInstrumentModelNumber ();
		text << "ABI model number:  " << stringData << endLine;
		ExcelText << "ABI model number:  " << stringData << endLine;

		stringData = data->GetInstrumentName ();
		text << "Instrument Name:  " << stringData << endLine;
		ExcelText << "Instrument Name:  " << stringData << endLine;

		NChannels = data->GetNumberOfDataChannels ();
		text << "Number of channels of data = " << NChannels << endLine;
		ExcelText << "Number of channels of data = " << NChannels << endLine;

		stringData = data->GetDyeSetName ();
		text << "Dye set name:  " << stringData << endLine;
		ExcelText << "Dye set name:  " << stringData << endLine;

		for (i=1; i<=NChannels; i++) {

			stringData = data->GetDyeNameForDataChannel (i);
			text << "Dye name for channel " << i << ":  " << stringData << endLine;
			ExcelText << "Dye name for channel " << i << ":  " << stringData << endLine;
		}

		StartDate = data->GetCollectionStartDate ();
		StartTime = data->GetCollectionStartTime ();
		LaneNumber = data->GetCapillaryNumber ();

		text << "Start date for data collection:  " << StartDate << endLine;
		text << "Start time for data collection:  " << StartTime << endLine;
		text << "Lane number for sample:  " << LaneNumber << endLine;

		ExcelText << "Start date for data collection:  " << StartDate << endLine;
		ExcelText << "Start time for data collection:  " << StartTime << endLine;
		ExcelText << "Lane number for sample:  " << LaneNumber << endLine;

		NSamples = data->GetNumberOfDataElementsForDataChannel (NChannels);
		text << "Number of samples for data channel " << NChannels << ":  " << NSamples << endLine;
		ExcelText << "Number of samples for data channel " << NChannels << ":  " << NSamples << endLine;
		ExcelText.ResetOutputLevel ();

		ladderBioComponent = new STRLadderCoreBioComponent (data->GetName ());

		status = ladderBioComponent->AnalyzeGrid (*data, GridData);
		CurrentMarkerSet = markerSetName;

		if (printGraphics) {
			
			FitDataName = GraphicsDirectory + "/Fit" + LadderFileName + ".txt";
			SampleOutput = new RGTextOutput (FitDataName, FALSE);

			if (SampleOutput->FileIsValid ())
				ladderBioComponent->WriteRawDataAndFitData (*SampleOutput, data);

			else
				cout << "Could not write graphics info to file " << FitDataName << ".  Skipping..." << endl;

			delete SampleOutput;
			SampleOutput = NULL;
		}

		ladderBioComponent->WriteXMLGraphicData (GraphicsDirectory, LadderFileName, data, 4, PlotString);

		if (status < 0) {

			Notice = "BIOCOMPONENT COULD NOT ANALYZE LADDER.  Skipping...";
			cout << Notice << "Return = " << status << endl;
			ExcelText << CLevel (1) << Notice << "\n";
			text << Notice << "\n";
			ExcelText << ladderBioComponent->GetError () << PLevel ();
			text << ladderBioComponent->GetError ();
			delete ladderBioComponent;
			delete data;
			ladderBioComponent = NULL;
			data = NULL;
			continue;
		}

		Notice = "";
		Notice << CurrentMarkerSet.GetData () << " ladder fit was successful";
		cout << Notice << endl;
		Notice << "\n";
		ExcelText.Write (1, Notice);
		text << Notice;

		SamplesProcessed++;
		Progress = 100.0 * (double)SamplesProcessed / (double)NSampleFiles;
		cout << "Progress = " << Progress << "%." << endl;

		LadderList.Append (ladderBioComponent);
		delete data;
	}

	cout << "Processed all ladders.  Number of ladders = " << LadderList.Entries () << endl;

	if (LadderList.Entries () == 0) {

		Notice << "PROJECT DID NOT MEET EXPECTATIONS...NO SATISFACTORY LADDER FOUND...ENDING";
		cout << Notice << endl;
		ExcelText << CLevel (1) << Notice << "\n" << PLevel ();
		text << Notice << "\n";
		return -20;
	}
	
	RGString SampleName;
	cycled = false;

	// Modify below functions to accumlate partial work, as possible, in spite of "errors", and report

	while (SampleDirectory.GetNextNonLadderFile (FileName, cycled) && !cycled) {

		FullPathName = DirectoryName + "/" + FileName;

		if (WorkingFile != NULL) {

			WorkingFile->Write (FullPathName + "\n");
			WorkingFile->Flush ();
		}

		data = new fsaFileData (FullPathName);  // this is the fsa file that we must modify!!!
		bioComponent = new STRSampleCoreBioComponent (data->GetName ());

		if (bioComponent->PrepareSampleForAnalysis (*data, SampleData) < 0) {

			Notice = "";
			Notice << "COULD NOT INITIALIZE AND PREPARE FOR ANALYSIS, FOR FILE:  " << FileName << "\n";
			ExcelText << CLevel (1) << Notice << bioComponent->GetError ();
			delete data;
			delete bioComponent;
			ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
			Notice = "";
			continue;
		}

		if (printGraphics) {

			FitDataName = GraphicsDirectory + "/Fit" + FileName + ".txt";
			SampleOutput = new RGTextOutput (FitDataName, FALSE);

			if (SampleOutput->FileIsValid ())
				bioComponent->WriteRawDataAndFitData (*SampleOutput, data);

			else
				cout << "Could not write graphics info for file " << FitDataName << ".  Skipping..." << endl;

			delete SampleOutput;
			SampleOutput = NULL;
		}

		if (bioComponent->PreliminarySampleAnalysis (LadderList, SampleData) < 0) {

			Notice = "";
			Notice << "COULD NOT PERFORM PRELIMINARY ANALYSIS, FILE:  " << FileName << "\n";
			ExcelText << CLevel (1) << Notice << bioComponent->GetError () << "\n";
			delete data;
			delete bioComponent;
			ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
			Notice = "";
			continue;
		}

		bioComponent->SetNegativeControlFalse ();
		bioComponent->SetPositiveControlFalse ();

		if (bioComponent->IsLabPositiveControl (FileName, pGenotypes)) {

			PositiveControlList.Append (bioComponent);
			bioComponent->SetPositiveControlTrue ();
		}

		else if (pServer->ControlDoesTargetStringContainASynonymCaseIndep (FileName)) {

		//	if (pServer->PosControlDoesTargetStringContainASynonymCaseIndep (FileName)) {
			if (pServer->PosControlDoesTargetStringContainASynonymCaseIndep (FileName)) {

				PositiveControlList.Append (bioComponent);
				bioComponent->SetPositiveControlTrue ();
			}

			else if (pServer->NegControlDoesTargetStringContainASynonymCaseIndep (FileName)) {

				NegativeControlList.Append (bioComponent);
				bioComponent->SetNegativeControlTrue ();
			}

			else {

				PositiveControlList.Append (bioComponent);
				bioComponent->SetPositiveControlTrue ();
			}
		}

		else
			SampleList.Append (bioComponent);

		if (bioComponent->AnalyzeSampleLoci (text, ExcelText, Message, TRUE) < 0) {

			Notice = "";
			Notice << "COULD NOT ANALYZE LOCI FOR FILE:  " << FileName << "\n";
			ExcelText << CLevel (1) << Notice << bioComponent->GetError () << "\n";
			ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
			Notice = "";
		}

		bioComponent->MakePreliminaryCalls (pGenotypes);
		bioComponent->SampleQualityTest (pGenotypes);
		bioComponent->TestPositiveControl (pGenotypes);
		bioComponent->OrganizeNoticeObjects ();

		bioComponent->ReportSampleData (ExcelText);
		bioComponent->WriteXMLGraphicData (GraphicsDirectory, FileName, data, 4, PlotString);
		SamplesProcessed++;
		Progress = 100.0 * (double)SamplesProcessed / (double)NSampleFiles;
		cout << "Progress = " << Progress << "%." << endl;
		delete data;
	}

	// Add output functions here!  First, put in the table, then, list the notices
/*	PopulationMarkerSet* set = mCollection->GetNamedPopulationMarkerSet (CurrentMarkerSet);

	if (set == NULL) {

		cout << "Could not retrieve marker set:  " << CurrentMarkerSet << ".  Ending..." << endl;
		return -25;
	}*/

	RGString TableHeading ("Sample Name\tILS\tChannels");
	Locus* nextLocus;
	set->ResetLocusList ();

	while (nextLocus = set->GetNextLocus ())
		TableHeading << "\t" << nextLocus->GetLocusName () << "-" << nextLocus->GetLocusChannel ();

	TableHeading << "\n";
	ExcelSummary.SetOutputLevel (1);
	ExcelLinks.SetOutputLevel (1);
	ExcelSummary << TableHeading;
	ExcelLinks << TableHeading;

	if (PositiveControlList.Entries () == 0) {

		newNotice = new NoPositiveControlInDirectory;
		AddNoticeToList (newNotice);
	}

	if (NegativeControlList.Entries () == 0) {

		newNotice = new NoNegativeControlInDirectory;
		AddNoticeToList (newNotice);
	}

	RGDListIterator it0 (LadderList);
	RGDListIterator it1 (NegativeControlList);
	RGDListIterator it2 (PositiveControlList);
	RGDListIterator it3 (SampleList);

	while (bioComponent = (CoreBioComponent*) it0 ()) {

		bioComponent->PrepareLociForOutput ();
//		bioComponent->UpdateNoticesFromLoci ();
		bioComponent->ReportGridTableRow (ExcelSummary);
		bioComponent->ReportGridTableRowWithLinks (ExcelLinks);
	}

	while (bioComponent = (CoreBioComponent*) it1 ()) {

		bioComponent->PrepareLociForOutput ();
//		bioComponent->UpdateNoticesFromLoci ();
		bioComponent->ReportSampleTableRow (ExcelSummary);
		bioComponent->ReportSampleTableRowWithLinks (ExcelLinks);
	}

	while (bioComponent = (CoreBioComponent*) it2()) {

		bioComponent->PrepareLociForOutput ();
//		bioComponent->UpdateNoticesFromLoci ();
		bioComponent->ReportSampleTableRow (ExcelSummary);
		bioComponent->ReportSampleTableRowWithLinks (ExcelLinks);
	}

	while (bioComponent = (CoreBioComponent*) it3 ()) {

		bioComponent->PrepareLociForOutput ();
//		bioComponent->UpdateNoticesFromLoci ();
		bioComponent->ReportSampleTableRow (ExcelSummary);
		bioComponent->ReportSampleTableRowWithLinks (ExcelLinks);
	}

	ExcelSummary << CLevel (1) << "\n" << PLevel ();
	ExcelLinks << CLevel (1) << "\n" << PLevel ();
	it0.Reset ();
	it1.Reset ();
	it2.Reset ();
	it3.Reset ();

	ReportNoticeObjects (ExcelSummary, "", " ", FALSE);
	ReportNoticeObjects (ExcelLinks, "", " ", TRUE);

	while (bioComponent = (CoreBioComponent*) it0 ()) {

		bioComponent->ReportAllNoticeObjects (ExcelSummary, "", " ", FALSE);
		bioComponent->ReportAllNoticeObjects (ExcelLinks, "\t", " ", TRUE);
	}

	while (bioComponent = (CoreBioComponent*) it1 ()) {

		bioComponent->ReportAllNoticeObjects (ExcelSummary, "", " ", FALSE);
		bioComponent->ReportAllNoticeObjects (ExcelLinks, "\t", " ", TRUE);
	}

	while (bioComponent = (CoreBioComponent*) it2()) {

		bioComponent->ReportAllNoticeObjects (ExcelSummary, "", " ", FALSE);
		bioComponent->ReportAllNoticeObjects (ExcelLinks, "\t", " ", TRUE);
	}

	while (bioComponent = (CoreBioComponent*) it3 ()) {

		bioComponent->ReportAllNoticeObjects (ExcelSummary, "", " ", FALSE);
		bioComponent->ReportAllNoticeObjects (ExcelLinks, "\t", " ", TRUE);
	}

	LadderList.ClearAndDelete ();
	delete SampleData;
	delete GridData;
	delete testPeak;
	
	OutputFile.Flush ();
	OutputFile.Close ();

	SampleList.ClearAndDelete ();
	PositiveControlList.ClearAndDelete ();
	NegativeControlList.ClearAndDelete ();

	return 0;
}








int STRLCAnalysis :: AnalyzeIncrementally (const RGString& prototypeInputDirectory, const RGString& markerSet, int outputLevel, const RGString& graphicsDirectory) {

	Boolean print = TRUE;
/*	SynonymList ControlIDs;
	SynonymList NegCntlIDs;
	SynonymList PosCntlIDs;

	ControlIDs.AddSynonym ("cntl");
	ControlIDs.AddSynonym ("cntrl");
	ControlIDs.AddSynonym ("ctl");
	ControlIDs.AddSynonym ("neg");
	ControlIDs.AddSynonym ("pos");
	ControlIDs.AddSynonym ("(+)");
	ControlIDs.AddSynonym ("(-)");
	ControlIDs.AddSynonym ("amp+");
	ControlIDs.AddSynonym ("blank");

	NegCntlIDs.AddSynonym ("neg");
	NegCntlIDs.AddSynonym ("(-)");
	NegCntlIDs.AddSynonym ("cntl-");
	NegCntlIDs.AddSynonym ("cntrl-");
	NegCntlIDs.AddSynonym ("ctl-");
	NegCntlIDs.AddSynonym ("blank");
	NegCntlIDs.AddSynonym ("ext-");

	PosCntlIDs.AddSynonym ("pos");
	PosCntlIDs.AddSynonym ("(+)");
	PosCntlIDs.AddSynonym ("amp+");
	PosCntlIDs.AddSynonym ("cntl+");
	PosCntlIDs.AddSynonym ("cntrl+");
	PosCntlIDs.AddSynonym ("ctl+");*/

	ParameterServer* pServer = new ParameterServer;
	GenotypeSet* gSet = pServer->GetGenotypeCollection ();

	if (gSet == NULL) {

		cout << "Could not retrieve genotype collection set from parameter server.  Exiting..." << endl;
		return -20;
	}

	GenotypesForAMarkerSet* pGenotypes = gSet->FindMarkerSetCollection (markerSet);

	if (pGenotypes == NULL) {

		cout << "Could not retrieve genotype collection for marker set name " << markerSet << " from parameter server.  Exiting..." << endl;
		return -20;
	}

	Notice* newNotice;
	newNotice = new BlobFound;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	newNotice = new SpikeFound;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	newNotice = new PullUpFound;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	newNotice = new InterLocusPeak;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	newNotice = new PullUpPrimaryChannel;
	CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	Notice::SetMessageTrigger (outputLevel);

	// These are all input
	RGString PrototypeInputDirectory (prototypeInputDirectory);
	mCurrentInputDirectory = prototypeInputDirectory;

	RGString DirectoryName = PrototypeInputDirectory;
	RGDirectory::MakeDirectory (mParentDirectoryForReports);

	RGString markerSetName (markerSet);
	RGString AnalysisDateStr;
	RGString AnalysisTimeStr;
	RGString GelExpirationDateStr;

	RGString FullPathForReports;
	RGString OutputDirectoryBase;
	size_t startPosition = 0;
	size_t endPosition;

	if (PrototypeInputDirectory.GetLastCharacter () == '/')
		PrototypeInputDirectory.ExtractAndRemoveLastCharacters (1);

	if (mParentDirectoryForReports.GetLastCharacter () == '/')
		mParentDirectoryForReports.ExtractAndRemoveLastCharacters (1);

	PrototypeInputDirectory.FindLastSubstring ("/", startPosition, endPosition);
	size_t nChars = PrototypeInputDirectory.Length () - endPosition - 1;
	OutputDirectoryBase = PrototypeInputDirectory.ExtractLastCharacters (nChars);

	FullPathForReports = mParentDirectoryForReports + "/" + OutputDirectoryBase;

	RGDirectory::MakeDirectory (FullPathForReports);

	cout << "Starting..." << endl;

	int i;

	DirectoryManager LadderDirectory (DirectoryName);

	if (!LadderDirectory.IsValid ()) {

		cout << "Could not find ladder directory:  " << DirectoryName << endl;
		cout << "Closing..." << endl;
		return -1;
	}

	else
		cout << "Ladder directory opened successfully" << endl;

	DirectoryManager SampleDirectory (DirectoryName);

	if (!SampleDirectory.IsValid ()) {

		cout << "Could not find sample directory:  " << DirectoryName << endl;
		cout << "Closing..." << endl;
		return -1;
	}

	else
		cout << "Sample directory opened successfully" << endl;

	int ChannelNumber = 1;
	int NSampleFiles = SampleDirectory.GetNumberOfFilesInDirectory ();
	double Progress;
	int SamplesProcessed = 0;

/*	RGString testSampleName;
	cout << "Test of sample name retrieval" << endl << endl;

	while (SampleDirectory.GetNextOrderedSampleFile (testSampleName)) {

		cout << testSampleName << endl;
		SamplesProcessed++;
	}

	cout << "Processed " << SamplesProcessed << " files..." << endl;

	if (true)
		return 0;
*/

	TestCharacteristic* testPeak = new STRTestControlCharacteristic ();
	testPeak->SetTestForNegative (TestCharacteristic::GetGlobalTestForNegative ());
	RGString LaneNumber;
	RGString StartDate;
	RGString StartTime;
	RGString Extension;
	int NumFiles = 0;
	RGString FileNumber;
	int NSamples;
	int status;
	RGString CurrentMarkerSet;

	CoreBioComponent* bioComponent;
	CoreBioComponent* ladderBioComponent;

	RGString FileName;
	RGString FullPathName;  // = DirectoryName + "/" + FileName;
//	int length;
	fsaFileData* data;
	pServer->SetDoubleGaussianSigmaRatio (4.0);

	RGString OutputFileName = OutputDirectoryBase;
	RGString OutputFullPath = FullPathForReports + "/" + OutputFileName + ".tab";
	RGFile OutputFile (OutputFullPath, "wt");
	RGString WholeOutputFileName = OutputFileName + "WholeSample.txt";
	RGString WholeFullPath = FullPathForReports + "/" + WholeOutputFileName;

	RGString SummaryFullPath = FullPathForReports + "/" + OutputFileName + "Summary.tab";
	RGString SummaryFullPathWithLinks = FullPathForReports + "/" + OutputFileName + "_.txt";
	RGString XMLSummaryFullPathWithLinks = FullPathForReports + "/" + OutputFileName + ".oar";

	RGString tempSummaryFullPath = FullPathForReports + "/temp" + OutputFileName + "Summary.txt";
	RGString tempSummaryFullPathWithLinks = FullPathForReports + "/temp" + OutputFileName + ".txt";
	RGString tempXMLSummaryFullPathWithLinks = FullPathForReports + "/temp" + OutputFileName + "XML.txt";

	RGFile OutputSummary (SummaryFullPath, "wt");
	RGFile OutputSummaryLinks (SummaryFullPathWithLinks, "wt");
	RGFile XMLOutputSummaryLinks (XMLSummaryFullPathWithLinks, "wt");

	RGFile tempOutputSummary (tempSummaryFullPath, "wt");
	RGFile tempOutputSummaryLinks (tempSummaryFullPathWithLinks, "wt");
	RGFile tempXMLOutputSummaryLinks (tempXMLSummaryFullPathWithLinks, "wt");

	RGString ConsoleOutputFileName = OutputFileName + "ConsoleData.txt";
	RGString WholeConsoleName = FullPathForReports + "/" + ConsoleOutputFileName;

	WorkingFileName = FullPathForReports + "/Working.txt";
	WorkingFile = new RGFile (WorkingFileName, "wt");

	if (!WorkingFile->isValid ())
		cout << "Could not open output file:  " << WorkingFileName << endl;
	
	RGTextOutput text (WholeConsoleName, FALSE);
//	RGTextOutput wholeSample (WholeFullPath, FALSE);
	OsirisMsg Message (&OutputFile, "\t", 10);
	Endl endLine;

	if (!OutputFile.isValid ()) {

		cout << "Could not open output file:  " << OutputFullPath << endl;
		cout << "Ending..." << endl;
		return -1;
	}

	if (!text.FileIsValid ()) {

		cout << "Could not open text echo file:  " << WholeConsoleName << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	if (!OutputSummary.isValid ()) {

		cout << "Could not open output summary file:  " << SummaryFullPath << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	if (!OutputSummaryLinks.isValid ()) {

		cout << "Could not open output summary file with embedded links:  " << SummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	if (!XMLOutputSummaryLinks.isValid ()) {

		cout << "Could not open output xml summary file with embedded links:  " << XMLSummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	if (!tempOutputSummary.isValid ()) {

		cout << "Could not open temporary output summary file:  " << tempSummaryFullPath << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	if (!tempOutputSummaryLinks.isValid ()) {

		cout << "Could not open temporary output summary file with embedded links:  " << tempSummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	if (!tempXMLOutputSummaryLinks.isValid ()) {

		cout << "Could not open temporary output xml summary file with embedded links:  " << tempXMLSummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		return -2;
	}

	cout << "Opened output file:  " << OutputFullPath << " and text echo file:  " << WholeConsoleName << endl;

	RGLogBook ExcelText (&OutputFile, outputLevel, FALSE);
	RGLogBook ExcelSummary (&OutputSummary, outputLevel, FALSE);
	RGLogBook ExcelLinks (&OutputSummaryLinks, outputLevel, FALSE);
	RGLogBook XMLExcelLinks (&XMLOutputSummaryLinks, outputLevel, FALSE);
	RGLogBook tempExcelSummary (&tempOutputSummary, outputLevel, FALSE);
	RGLogBook tempExcelLinks (&tempOutputSummaryLinks, outputLevel, FALSE);
	RGLogBook tempXMLExcelLinks (&tempXMLOutputSummaryLinks, outputLevel, FALSE);
	GridDataStruct* GridData = new GridDataStruct (mCollection, markerSetName, testPeak, text, ExcelText, Message, print);
	SampleDataStruct* SampleData = new SampleDataStruct (mCollection, markerSetName, testPeak, testPeak, text, ExcelText, Message, print);
	PopulationMarkerSet* set = mCollection->GetNamedPopulationMarkerSet (markerSetName);

	if (set == NULL) {

		cout << "Could not find marker set named:  " << markerSetName << ".  Ending..." << endl;
		return -10000;
	}

	RGString lsName = set->GetLaneStandardName ();
	int ilsChannelNumber = set->GetLaneStandardChannel ();
	text << "Directory:" << endLine;
	text << "    " << DirectoryName.GetData() << endLine;

	ExcelText.SetOutputLevel (1);
	ExcelText << "Directory:" << endLine;
	ExcelText << "    " << DirectoryName.GetData() << endLine;

	int expectedNumberOfChannels = set->GetNumberOfChannels ();

	pServer->Report (text, "");
	pServer->Report (ExcelText, "");
	ExcelText.ResetOutputLevel ();

	ExcelSummary.SetOutputLevel (1);
//	ExcelSummary << "Directory:" << endLine;
//	ExcelSummary << "    " << DirectoryName.GetData() << endLine << endLine;

	int minSampleRFU = (int)floor (STRSampleChannelData::GetMinRFU () + 0.5);
	int minLadderRFU = (int)floor (STRLadderChannelData::GetMinRFU () + 0.5);
	int minLaneStdRFU = (int)floor (STRLaneStandardChannelData::GetMinRFU () + 0.5);
	int minInterlocusRFU = (int)floor (STRSampleChannelData::GetMinInterlocusRFU () + 0.5);

	ExcelSummary << "OAR 1.2" << endLine;
	ExcelSummary << "Parameters:" << endLine;
	ExcelSummary << "Input Directory:  " << DirectoryName.GetData () << endLine;
	ExcelSummary << "Output Directory:  " << FullPathForReports.GetData () << endLine;
	ExcelSummary << "Kit Name:  " << markerSet << endLine;
	ExcelSummary << "Internal Lane Standard:  " << lsName.GetData () << endLine;
	ExcelSummary << "Minimum RFU:  " << minSampleRFU << "," << minLaneStdRFU << ",";
	ExcelSummary << minLadderRFU << "," << minInterlocusRFU << endLine;
	ExcelSummary << "Standard Positive Control Name:  " << pServer->GetStandardPositiveControlName () << endLine << endLine;
	ExcelSummary.ResetOutputLevel ();

	ExcelLinks.SetOutputLevel (1);
//	ExcelLinks << "Directory:" << endLine;
//	ExcelLinks << "    " << DirectoryName.GetData() << endLine << endLine;

	ExcelLinks << "OAR 1.2" << endLine;
	ExcelLinks << "Parameters:" << endLine;
	ExcelLinks << "Input Directory:  " << DirectoryName.GetData () << endLine;
	ExcelLinks << "Output Directory:  " << FullPathForReports.GetData () << endLine;
	ExcelLinks << "Kit Name:  " << markerSet << endLine;
	ExcelLinks << "Internal Lane Standard:  " << lsName.GetData () << endLine;
	ExcelLinks << "Minimum RFU:  " << minSampleRFU << "," << minLaneStdRFU << ",";
	ExcelLinks << minLadderRFU << "," << minInterlocusRFU << endLine;
	ExcelLinks << "Standard Positive Control Name:  " << pServer->GetStandardPositiveControlName () << endLine << endLine;
	ExcelLinks.ResetOutputLevel ();

	time_t thisTime;
	time (&thisTime);
	RGString pResult;

	XMLExcelLinks.SetOutputLevel (1);
	XMLExcelLinks << "<OsirisAnalysisReport>\n\t<Version>1.3</Version>\n\t<Heading>\n\t\t<FileName>";
	XMLExcelLinks << xmlwriter::EscAscii (XMLSummaryFullPathWithLinks, &pResult) << "</FileName>\n\t\t";
	XMLExcelLinks << "<CreationTime>" << (long)thisTime << "</CreationTime>\n\t\t";
	XMLExcelLinks << "<CommandLine>\n" << mCommandLineString.GetData () << "\t\t</CommandLine>\n\t\t";
	pServer->WriteSettingsToDotOAR (XMLExcelLinks);
	XMLExcelLinks << "\t\t<RuntimeSettings>\n";
	XMLExcelLinks << "\t\t\t<inputDirectory>" << xmlwriter::EscAscii (DirectoryName, &pResult) << "</inputDirectory>\n";
	XMLExcelLinks << "\t\t\t<outputDirectory>" << xmlwriter::EscAscii (FullPathForReports, &pResult) << "</outputDirectory>\n";
	XMLExcelLinks << "\t\t\t<kit>" << xmlwriter::EscAscii (markerSet, &pResult) << "</kit>\n";
	XMLExcelLinks << "\t\t\t<ls>" << xmlwriter::EscAscii (lsName, &pResult) << "</ls>\n";
	XMLExcelLinks << "\t\t\t<minRFUsample>" << minSampleRFU << "</minRFUsample>\n";
	XMLExcelLinks << "\t\t\t<minRFUILS>" << minLaneStdRFU << "</minRFUILS>\n";
	XMLExcelLinks << "\t\t\t<minRFUladder>" << minLadderRFU << "</minRFUladder>\n";
	XMLExcelLinks << "\t\t\t<minRFUinterlocus>" << minInterlocusRFU << "</minRFUinterlocus>\n";
	XMLExcelLinks << "\t\t\t<DataAnalyzed>";

	if (CoreBioComponent::GetUseRawData ())
		XMLExcelLinks << "false</DataAnalyzed>\n";

	else
		XMLExcelLinks << "true</DataAnalyzed>\n";

	XMLExcelLinks << "\t\t</RuntimeSettings>\n";
	XMLExcelLinks << "\t\t<MarkerSet>" << xmlwriter::EscAscii (markerSet, &pResult) << "</MarkerSet>\n";
	XMLExcelLinks << "\t\t<ILSchannelNr>" << ilsChannelNumber << "</ILSchannelNr>\n";

	RGString channelListString;
	set->BuildChannelAndLocusListForHeader (channelListString);
	XMLExcelLinks << channelListString;

	//
	//  Keep going with xml Header!!!
	//

	XMLExcelLinks << "\t</Heading>\n";   //  Change this when done with header and moving on to Table, etc...!!!!!
	XMLExcelLinks << "\t<Table>\n";
	XMLExcelLinks.ResetOutputLevel ();

	RGString TableHeading ("Sample Name\tILS\tChannels");
	Locus* nextLocus;
	set->ResetLocusList ();

	while (nextLocus = set->GetNextLocus ())
		TableHeading << "\t" << nextLocus->GetLocusName () << "-" << nextLocus->GetLocusChannel ();

	TableHeading << "\t+Cntrl";
	TableHeading << "\n";
	ExcelSummary.SetOutputLevel (1);
	ExcelLinks.SetOutputLevel (1);
	ExcelSummary << TableHeading;
	ExcelLinks << TableHeading;

	RGString PlotString;

	PlotString << "\t<parameters>\n\t\t<inputDirectory>" << xmlwriter::EscAscii (DirectoryName, &pResult) << "</inputDirectory>\n\t\t<outputDirectory>";
	PlotString << xmlwriter::EscAscii (FullPathForReports, &pResult) << "</outputDirectory>\n\t\t<kit>";
	PlotString << xmlwriter::EscAscii (markerSet, &pResult) << "</kit>\n\t\t<ls>";
	PlotString << lsName << "</ls>\n\t\t<minRFUsample>" << minSampleRFU << "</minRFUsample>\n\t\t<minRFUILS>";
	PlotString << minLaneStdRFU << "</minRFUILS>\n\t\t<minRFUladder>" << minLadderRFU;
	PlotString << "</minRFUladder>\n\t\t<minRFUinterlocus>" << minInterlocusRFU << "</minRFUinterlocus>\n\t\t<DataAnalyzed>";

	if (CoreBioComponent::GetUseRawData ())
		PlotString << "false</DataAnalyzed>\n\t\t<StdPosControlName>";

	else
		PlotString << "true</DataAnalyzed>\n\t\t<StdPosControlName>";

	PlotString << xmlwriter::EscAscii (pServer->GetStandardPositiveControlName (), &pResult) << "</StdPosControlName>\n\t</parameters>\n";

	RGString stringData;

	testPeak->Report (ExcelText, "", "\t");

	int NChannels;

	Boolean cycled;
	RGString LadderFileName;

	RGString PersonNum;
	RGString LineTitle;

	RGString OutputLadderName;
	RGString OutputSampleName;
	RGString FullPathOutputLadderName;
	RGString FullPathOutputSampleName;
	RGString TempString;
	int LadderNumber = 1;
	RGString LadderNumberString;
	RGDList LadderList;
	RGString LadderName;
	PackedTime testTime;
	bool printGraphics = false;

	cout << "Ready to read ladders..." << endl;
	cycled = FALSE;
	RGString NoticeStr;
	RGTextOutput* SampleOutput;
	RGString FitDataName;
	RGString OsirisGraphics (graphicsDirectory);
	bool ladderOK;
	bool hasPosControl = false;
	bool hasNegControl = false;
	bool foundALadder = true;

	if (OsirisGraphics.GetLastCharacter () != '/')
		OsirisGraphics += "/";

/*	if (graphicsDirectory.Length () > 0)
		printGraphics = true;

	else
		printGraphics = false;*/

	RGString GraphicsDirectory = OsirisGraphics + OutputDirectoryBase;

	if (printGraphics)
		RGDirectory::MakeDirectory (GraphicsDirectory);

	while (LadderDirectory.GetNextLadderFile (LadderFileName, cycled) && !cycled) {

		FullPathName = DirectoryName + "/" + LadderFileName;

		if (WorkingFile != NULL) {

			WorkingFile->Write (FullPathName + "\n");
			WorkingFile->Flush ();
		}

		data = new fsaFileData (FullPathName);
		NumFiles++;
		ladderOK = true;
		//cout << "Beginning file:  " << NumFiles << endl;

		if (!data->IsValid ()) {

			NoticeStr << "Oops, " << LadderFileName.GetData () << " is not valid...Skipping";
			cout << NoticeStr << endl;
			NoticeStr << "\n";
			ExcelText.Write (1, NoticeStr);
			text << NoticeStr;
			delete data;
			continue;
		}

		ExcelText.SetOutputLevel (1);
		text << endLine << endLine;
		ExcelText << endLine << endLine;

		text << "File Name:" << endLine;
		text << "    " << LadderFileName.GetData () << endLine;

		ExcelText << "File Name:" << endLine;
		ExcelText << "    " << LadderFileName.GetData () << endLine;

		stringData = data->GetInstrumentModelNumber ();
		text << "ABI model number:  " << stringData << endLine;
		ExcelText << "ABI model number:  " << stringData << endLine;

		stringData = data->GetInstrumentName ();
		text << "Instrument Name:  " << stringData << endLine;
		ExcelText << "Instrument Name:  " << stringData << endLine;

		NChannels = data->GetNumberOfDataChannels ();
		text << "Number of channels of data = " << NChannels << endLine;
		ExcelText << "Number of channels of data = " << NChannels << endLine;

		stringData = data->GetDyeSetName ();
		text << "Dye set name:  " << stringData << endLine;
		ExcelText << "Dye set name:  " << stringData << endLine;

		if (expectedNumberOfChannels > NChannels) {

			cout << "MARKERSET MISMATCH...EXPECTING " << expectedNumberOfChannels << " CHANNELS AND FILE CONTAINS " << NChannels << " CHANNELS" << endl;
			cout << "ENDING..." << endl;

			text << "MARKERSET MISMATCH...EXPECTING " << expectedNumberOfChannels << " CHANNELS AND FILE CONTAINS " << NChannels << " CHANNELS" << endLine;
			text << "ENDING..." << endLine;

			ExcelText << "MARKERSET MISMATCH...EXPECTING " << expectedNumberOfChannels << " CHANNELS AND FILE CONTAINS " << NChannels << " CHANNELS" << endLine;
			ExcelText << "ENDING..." << endLine;
			return -150;
		}

		for (i=1; i<=expectedNumberOfChannels; i++) {

			stringData = data->GetDyeNameForDataChannel (i);
			text << "Dye name for channel " << i << ":  " << stringData << endLine;
			ExcelText << "Dye name for channel " << i << ":  " << stringData << endLine;
		}

		StartDate = data->GetCollectionStartDate ();
		StartTime = data->GetCollectionStartTime ();
		LaneNumber = data->GetCapillaryNumber ();

		text << "Start date for data collection:  " << StartDate << endLine;
		text << "Start time for data collection:  " << StartTime << endLine;
		text << "Lane number for sample:  " << LaneNumber << endLine;

		ExcelText << "Start date for data collection:  " << StartDate << endLine;
		ExcelText << "Start time for data collection:  " << StartTime << endLine;
		ExcelText << "Lane number for sample:  " << LaneNumber << endLine;

		NSamples = data->GetNumberOfDataElementsForDataChannel (expectedNumberOfChannels);
		text << "Number of samples for data channel " << expectedNumberOfChannels << ":  " << NSamples << endLine;
		ExcelText << "Number of samples for data channel " << expectedNumberOfChannels << ":  " << NSamples << endLine;
		ExcelText.ResetOutputLevel ();

		ladderBioComponent = new STRLadderCoreBioComponent (data->GetName ());
		status = ladderBioComponent->AnalyzeGrid (*data, GridData);
		CurrentMarkerSet = markerSetName;

		if (printGraphics) {
			
			FitDataName = GraphicsDirectory + "/Fit" + LadderFileName + ".txt";
			SampleOutput = new RGTextOutput (FitDataName, FALSE);

			if (SampleOutput->FileIsValid ())
				ladderBioComponent->WriteRawDataAndFitData (*SampleOutput, data);

			else
				cout << "Could not write graphics info to file " << FitDataName << ".  Skipping..." << endl;

			delete SampleOutput;
			SampleOutput = NULL;
		}

//		ladderBioComponent->WriteXMLGraphicData (GraphicsDirectory, LadderFileName, data, 4, PlotString);

//		if (status < 0) {
//
//			ladderOK = false;
//			NoticeStr = "BIOCOMPONENT COULD NOT ANALYZE LADDER.  Skipping...";
//			cout << NoticeStr << "Return = " << status << endl;
//			ExcelText << CLevel (1) << NoticeStr << "\n";
//			text << NoticeStr << "\n";
//			ExcelText << ladderBioComponent->GetError () << PLevel ();
//			text << ladderBioComponent->GetError ();
//			newNotice = new LadderReqReview;
//			ladderBioComponent->AddNoticeToList (newNotice);
//			delete ladderBioComponent;
//			delete data;
//			ladderBioComponent = NULL;
//			data = NULL;
//			continue;
//		}

		SamplesProcessed++;
		Progress = 100.0 * (double)SamplesProcessed / (double)NSampleFiles;
		cout << "Progress = " << Progress << "%." << endl;

		delete data;
		data = NULL;
		
		if ((ladderBioComponent->GridQualityTest () < 0) || (status < 0)) {

			ladderOK = false;
			RGString ladderName = "-- " + ladderBioComponent->GetSampleName ();
			newNotice = new LadderReqReview;
			newNotice->AddDataItem (ladderName);
			AddNoticeToList (newNotice);
		}

		ladderBioComponent->FilterNoticesBelowMinBioID ();
		ladderBioComponent->OrganizeNoticeObjects ();
		ladderBioComponent->WriteXMLGraphicData (GraphicsDirectory, LadderFileName, data, 4, PlotString);
		ladderBioComponent->PrepareLociForOutput ();
		ladderBioComponent->ReportGridTableRow (ExcelSummary);
		ladderBioComponent->ReportGridTableRowWithLinks (ExcelLinks);
		ladderBioComponent->ReportXMLGridTableRowWithLinks (XMLExcelLinks, tempXMLExcelLinks);
		ladderBioComponent->ReportAllNoticeObjects (tempExcelSummary, "", " ", FALSE);
		ladderBioComponent->ReportAllNoticeObjects (tempExcelLinks, "", " ", TRUE);

		if (!ladderOK) {

			NoticeStr = "BIOCOMPONENT COULD NOT ANALYZE LADDER.  Skipping...";
			cout << NoticeStr << "Return = " << status << endl;
			ExcelText << CLevel (1) << NoticeStr << "\n";
			text << NoticeStr << "\n";
			ExcelText << ladderBioComponent->GetError () << PLevel ();
			text << ladderBioComponent->GetError ();

			delete ladderBioComponent;
			ladderBioComponent = NULL;
		}

		else {

			LadderList.Append (ladderBioComponent);
			NoticeStr = "";
			NoticeStr << CurrentMarkerSet.GetData () << " ladder fit was successful";
			cout << NoticeStr << endl;
			NoticeStr << "\n";
			ExcelText.Write (1, NoticeStr);
			text << NoticeStr;
		}
	}

	cout << "Processed all ladders.  Number of ladders = " << LadderList.Entries () << endl;
	RGString SampleName;
	bool sampleOK;

	if (LadderList.Entries () == 0) {

		NoticeStr << "PROJECT DID NOT MEET EXPECTATIONS...NO SATISFACTORY LADDER FOUND...ENDING";
		cout << NoticeStr << endl;
		ExcelText << CLevel (1) << NoticeStr << "\n" << PLevel ();
		text << NoticeStr << "\n";
		foundALadder = false;
		goto finishOutput;
//		return -20;
	}

	// Modify below functions to accumlate partial work, as possible, in spite of "errors", and report

	while (SampleDirectory.GetNextOrderedSampleFile (FileName)) {

		sampleOK = true;
		FullPathName = DirectoryName + "/" + FileName;

		if (WorkingFile != NULL) {

			WorkingFile->Write (FullPathName + "\n");
			WorkingFile->Flush ();
		}

		data = new fsaFileData (FullPathName);  // this is the fsa file that we must modify!!!
		bioComponent = new STRSampleCoreBioComponent (data->GetName ());

		if (bioComponent->PrepareSampleForAnalysis (*data, SampleData) < 0) {

			sampleOK = false;
			NoticeStr = "";
			NoticeStr << "COULD NOT INITIALIZE AND PREPARE FOR ANALYSIS, FOR FILE:  " << FileName << "\n";
			ExcelText << CLevel (1) << NoticeStr << bioComponent->GetError ();
			ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
			NoticeStr = "";
			newNotice = new SampleReqReview;
			bioComponent->AddNoticeToList (newNotice);
		}

		if (sampleOK && printGraphics) {

			FitDataName = GraphicsDirectory + "/Fit" + FileName + ".txt";
			SampleOutput = new RGTextOutput (FitDataName, FALSE);

			if (SampleOutput->FileIsValid ())
				bioComponent->WriteRawDataAndFitData (*SampleOutput, data);

			else
				cout << "Could not write graphics info for file " << FitDataName << ".  Skipping..." << endl;

			delete SampleOutput;
			SampleOutput = NULL;
		}

		if (sampleOK && (bioComponent->PreliminarySampleAnalysis (LadderList, SampleData) < 0)) {

			sampleOK = false;
			NoticeStr = "";
			NoticeStr << "COULD NOT PERFORM PRELIMINARY ANALYSIS, FILE:  " << FileName << "\n";
			ExcelText << CLevel (1) << NoticeStr << bioComponent->GetError () << "\n";
//			delete data;
//			delete bioComponent;
			ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
			NoticeStr = "";
			newNotice = new SampleReqReview;
			bioComponent->AddNoticeToList (newNotice);
//			continue;
		}

		bioComponent->SetNegativeControlFalse ();
		bioComponent->SetPositiveControlFalse ();

		if (sampleOK && pServer->ControlDoesTargetStringContainASynonymCaseIndep (FileName)) {

			if (pServer->NegControlDoesTargetStringContainASynonymCaseIndep (FileName)) {

				hasNegControl = true;;
				bioComponent->SetNegativeControlTrue ();
			}

			else if (pServer->PosControlDoesTargetStringContainASynonymCaseIndep (FileName)) {

				hasPosControl = true;
				bioComponent->SetPositiveControlTrue ();
			}
		}

//		else
//			SampleList.Append (bioComponent);

		if (sampleOK && (bioComponent->AnalyzeSampleLoci (text, ExcelText, Message, TRUE) < 0)) {

			NoticeStr = "";
			NoticeStr << "COULD NOT ANALYZE LOCI FOR FILE:  " << FileName << "\n";
			ExcelText << CLevel (1) << NoticeStr << bioComponent->GetError () << "\n";
			ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
			NoticeStr = "";
		}


		if (sampleOK) {

			bioComponent->MakePreliminaryCalls (pGenotypes);
			bioComponent->ResolveAmbiguousInterlocusSignals ();
			bioComponent->SampleQualityTest (pGenotypes);
			bioComponent->TestPositiveControl (pGenotypes);
		}

		else
			bioComponent->LocatePositiveControlName (pGenotypes);

		bioComponent->FilterNoticesBelowMinBioID ();
		bioComponent->OrganizeNoticeObjects ();
		bioComponent->ReportSampleData (ExcelText);
		bioComponent->WriteXMLGraphicData (GraphicsDirectory, FileName, data, 4, PlotString);

		bioComponent->PrepareLociForOutput ();
		bioComponent->ReportSampleTableRow (ExcelSummary);
		bioComponent->ReportSampleTableRowWithLinks (ExcelLinks);
		bioComponent->ReportXMLSampleTableRowWithLinks (XMLExcelLinks, tempXMLExcelLinks);

		bioComponent->ReportAllNoticeObjects (tempExcelSummary, "", " ", FALSE);
		bioComponent->ReportAllNoticeObjects (tempExcelLinks, "", " ", TRUE);

		SamplesProcessed++;
		Progress = 100.0 * (double)SamplesProcessed / (double)NSampleFiles;
		cout << "Progress = " << Progress << "%." << endl;
		delete data;
		delete bioComponent;
		data = NULL;
		bioComponent = NULL;
	}

	// Add output functions here!  First, put in the table, then, list the notices
/*	PopulationMarkerSet* set = mCollection->GetNamedPopulationMarkerSet (CurrentMarkerSet);

	if (set == NULL) {

		cout << "Could not retrieve marker set:  " << CurrentMarkerSet << ".  Ending..." << endl;
		return -25;
	}*/

	if (!hasPosControl) {

		newNotice = new NoPositiveControlInDirectory;
		AddNoticeToList (newNotice);
	}

	if (!hasNegControl) {

		newNotice = new NoNegativeControlInDirectory;
		AddNoticeToList (newNotice);
	}

finishOutput:
	ExcelSummary << CLevel (1) << "\n" << PLevel ();
	ExcelLinks << CLevel (1) << "\n" << PLevel ();

	XMLExcelLinks << CLevel (1) << "\t</Table>\n" << PLevel ();
	ReportXMLNoticeObjects (XMLExcelLinks, tempXMLExcelLinks, " ");
	XMLExcelLinks << CLevel (1) << "\t<Messages>\n" << PLevel ();

	tempXMLOutputSummaryLinks.Flush ();
	tempXMLOutputSummaryLinks.Close ();
	RGFile tempInputXMLSummaryLinks (tempXMLSummaryFullPathWithLinks, "rt");

	if (!tempInputXMLSummaryLinks.isValid ()) {

		cout << "Could not complete output xml summary; temporary file unavailable..." << endl;
		return -5;
	}

	RGString in3;
	in3.ReadTextFile (tempInputXMLSummaryLinks);
	XMLExcelLinks << CLevel (1) << in3;
	XMLExcelLinks << "\t</Messages>\n" << PLevel ();

	// First add directory alerts and then...Merge here to get messages...

	XMLExcelLinks << CLevel (1) << "</OsirisAnalysisReport>" << endLine << PLevel ();

	tempInputXMLSummaryLinks.Close ();
	
	ReportNoticeObjects (ExcelSummary, "", " ", FALSE);
	ReportNoticeObjects (ExcelLinks, "", " ", TRUE);

	tempOutputSummary.Flush ();
	tempOutputSummary.Close ();
	tempOutputSummaryLinks.Flush ();
	tempOutputSummaryLinks.Close ();

	LadderList.ClearAndDelete ();
	delete SampleData;
	delete GridData;
	delete testPeak;

	RGFile tempInputSummary (tempSummaryFullPath, "rt");
	RGFile tempInputSummaryLinks (tempSummaryFullPathWithLinks, "rt");

	if (!tempInputSummary.isValid ()) {

		cout << "Could not complete output summary; temporary file unavailable..." << endl;
		return -5;
	}

	if (!tempInputSummaryLinks.isValid ()) {

		cout << "Could not complete output summary; temporary file unavailable..." << endl;
		return -5;
	}
	
	RGString in1;
	RGString in2;

	in1.ReadTextFile (tempInputSummary);
	in2.ReadTextFile (tempInputSummaryLinks);

	tempInputSummary.Close ();
	tempInputSummaryLinks.Close ();

	// delete temporary files

	remove (tempSummaryFullPath.GetData ());
	remove (tempSummaryFullPathWithLinks.GetData ());
	remove (tempXMLSummaryFullPathWithLinks.GetData ());

	ExcelSummary.SetOutputLevel (1);
	ExcelLinks.SetOutputLevel (1);

	ExcelSummary << in1 << "\n";
	ExcelLinks << in2 << "\n";
	
	OutputFile.Flush ();
	OutputFile.Close ();

	if (!foundALadder)
		return -20;

	return 0;
}




int STRLCAnalysis :: AddNoticeToList (Notice* newNotice) {

	if (NewNoticeList.Entries () == 0) {

		mHighestSeverityLevel = newNotice->GetSeverityLevel ();
		mHighestMessageLevel = newNotice->GetMessageLevel ();
		NewNoticeList.Append (newNotice);
	}

	else {

		int temp = newNotice->GetSeverityLevel ();

		if (mHighestSeverityLevel >= temp) {

			mHighestSeverityLevel = temp;
			NewNoticeList.Prepend (newNotice);
		}

		else {

			RGDListIterator it (NewNoticeList);
			Notice* nextNotice;
			bool noticeInserted = false;

			while (nextNotice = (Notice*) it ()) {

				if (temp < nextNotice->GetSeverityLevel ()) {

					--it;
					it.InsertAfterCurrentItem (newNotice);
					noticeInserted = true;
					break;
				}
			}

			if (!noticeInserted)
				NewNoticeList.Append (newNotice);
		}

		temp = newNotice->GetMessageLevel ();

		if (mHighestMessageLevel > temp)
			mHighestMessageLevel = temp;
	}

	return NewNoticeList.Entries ();
}



Boolean STRLCAnalysis :: IsNoticeInList (const Notice* target) {

	if (NewNoticeList.Find (target))
		return TRUE;

	return FALSE;
}


Notice* STRLCAnalysis :: GetNotice (const Notice* target) {

	return (Notice*) NewNoticeList.Find (target);
}



Boolean STRLCAnalysis :: ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim) {

	if (NewNoticeList.Entries () > 0) {

		RGDListIterator it (NewNoticeList);
		Notice* nextNotice;
		text.SetOutputLevel (mHighestMessageLevel);

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			return FALSE;
		}

		Endl endLine;
		text << endLine;
		text << indent << "Notices for directory " << mCurrentInputDirectory << ":  " << endLine;
		text.ResetOutputLevel ();

		while (nextNotice = (Notice*) it ())
			nextNotice->Report (text, indent, delim);

		text.Write (1, "\n");
	}

	else
		return FALSE;

	return TRUE;
}


Boolean STRLCAnalysis :: ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink) {

	if (NewNoticeList.Entries () > 0) {

		RGDListIterator it (NewNoticeList);
		Notice* nextNotice;
		text.SetOutputLevel (mHighestMessageLevel);
		RGString indent2 = indent + "\t";
		int link;

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			return FALSE;
		}

		Endl endLine;
		text << endLine;

		if (reportLink) {

			link = Notice::GetNextLinkNumber ();
			SetTableLink (link);
			text << mTableLink << "\t";
		}

		text << indent << "Notices for directory " << mCurrentInputDirectory << ":  " << endLine;
		text.ResetOutputLevel ();

		while (nextNotice = (Notice*) it ())
			nextNotice->Report (text, indent, delim);

		if (reportLink) {

			text.SetOutputLevel (mHighestMessageLevel);
			text << mTableLink;
			text.ResetOutputLevel ();
		}

		text.Write (1, "\n");
	}

	else
		return FALSE;

	return TRUE;
}


Boolean STRLCAnalysis :: ReportXMLNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	if (NewNoticeList.Entries () > 0) {

		RGDListIterator it (NewNoticeList);
		Notice* nextNotice;
		text.SetOutputLevel (mHighestMessageLevel);
		tempText.SetOutputLevel (1);
		int msgNum;
		int triggerLevel = Notice::GetMessageTrigger ();

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			tempText.ResetOutputLevel ();
			return FALSE;
		}

		text << CLevel (1) << "\t<DirectoryAlerts>\n" << PLevel ();

		while (nextNotice = (Notice*) it ()) {

			if (nextNotice->GetMessageLevel () <= triggerLevel) {

				msgNum = Notice::GetNextMessageNumber ();
				nextNotice->SetMessageNumber (msgNum);
				text << "\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
				tempText << "\t\t<Message>\n";
				tempText << "\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
				tempText << "\t\t\t<Index>" << nextNotice->GetID () << "</Index>\n";
				tempText << "\t\t\t<Text>" << (nextNotice->AssembleString (delim)).GetData () << "</Text>\n";
				tempText << "\t\t</Message>\n";
			}
		}

		text << CLevel (1) << "\t</DirectoryAlerts>\n";
		text.ResetOutputLevel ();
		tempText.ResetOutputLevel ();
	}

	else
		return FALSE;

	return TRUE;
}


void STRLCAnalysis :: ClearNoticeObjects () {

	NewNoticeList.ClearAndDelete ();
}



int STRLCAnalysis :: NumberOfNoticeObjects () {

	return NewNoticeList.Entries ();
}


void STRLCAnalysis :: SetTableLink (int linkNumber) {

	RGString temp;
	temp.Convert (linkNumber, 10);
	mTableLink = "&" + temp + "&";
}

