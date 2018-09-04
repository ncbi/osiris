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
*  FileName: TestAnalysisDirectoryLC.cpp
*  Author:   Robert Goor
*
*/
//
// TestValidationDBDirectory.cpp : Defines the entry point for the console application...to build sample data using 
// already existing ladders and ILS's
//
//	This is for version 2.0
//

#if defined(_WINDOWS)
#include "stdafx.h"
#else
#define _tmain main
#define _TCHAR char
#endif

//#include "fsaFileData.h"
#include "rgstring.h"
#include "DataSignal.h"
#include "RGTextOutput.h"
#include "rgdirectory.h"
#include "ParameterServer.h"
#include "Quadratic.h"
#include "OsirisMsg.h"
#include "TestCharacteristic.h"
#include "DirectoryManager.h"
#include "coordtrans.h"
#include "rgtokenizer.h"
#include "PackedTime.h"
#include "PackedDate.h"
#include "fsaPeakData.h"
#include "fsaDirEntry.h"
#include "STRLCAnalysis.h"
#include "Genetics.h"
#include "STRGenetics.h"
#include "Notice.h"
#include "CoreBioComponent.h"
#include "rgtokenizer.h"
#include "STRChannelData.h"
#include "TracePrequalification.h"
#include "xmlwriter.h"
#include "OsirisInputFile.h"
#include "IndividualGenotype.h"
#include "rgparray.h"


#include <string>
#include <math.h>
#include <list>

using namespace std;



int _tmain(int argc, _TCHAR* argv[]) {

	// These are all input
	int* testMemory = new int [4];
	testMemory [0] = 1;
	RGString PrototypeInputDirectory;
	RGString LadderInformationDirectory;
	RGString ParentDirectoryForReports;
	RGString OutputSubDirectory;
	RGString MarkerSetName;
	int OutputLevel;
	double minSampleRFU;
	double minLaneStandardRFU;
	double minLadderRFU;
	double minInterlocusRFU = -1.0;
	double minLadderInterlocusRFU = -1.0;
	double sampleDetectionThreshold = -1.0;
	int severity = 0;
	int testRatio = 1;
	RGString MessageBookPath;
	RGString graphicsDirectory;
//	CoreBioComponent::DontUseRawData ();
	CoreBioComponent::SetOtherSignature ();
	RGString RfuString;
	RGString CommandLine;
	RGString pResult;
	
	RGString token;
	RGString LevelString;
	RGString Comma (",");
	RGStringArray tokens (3);
	RGStringArray delims (3);
	RGString UserLaneStandardName;
	RGString rawDataString;

	RGString stdSettingsFileName;
	RGString labSettingsFileName;
	RGString defaultStdSettingsFile ("../OsirisXML/OsirisSTRStandardSettings.xml");
	RGString defaultLabSettingsFile ("../OsirisXML/GenericLabSettings.xml");
	RGString ConfigDirectory;
	CoreBioComponent::SetUseRawData ();
	RGPArray::ResetDefaultSize (20);
	RGPArray::ResetDefaultIncrement (20);

	//
	//	OsirisInputFile is always started with a debug value of "false", even for debug runs, because the debugger
	//	is specified to stream the file BaseInputFile.txt to standard in.  Therefore, this program always looks for
	//	its input on standard in, whether debugging or not
	//

	bool debugMode;

#ifdef _DEBUG
	debugMode = true;

#else
	debugMode = false;

#endif

	OsirisInputFile inputFile (debugMode);

	int inputStatus = inputFile.ReadAllInputs ("BaseInputFile.txt");
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

	inputFile.ResetInputLines ();
	RGString* nextInputLine;

	while (nextInputLine = inputFile.GetNextInputLine ())
		CommandLine << "\t\t\t<argv>" << xmlwriter::EscAscii (*nextInputLine, &pResult) << "</argv>\n";

	PrototypeInputDirectory = inputFile.GetInputDirectory ();
	ConfigDirectory = LadderInformationDirectory = inputFile.GetLadderDirectory ();
	graphicsDirectory = ParentDirectoryForReports = inputFile.GetReportDirectory ();
	OutputSubDirectory = inputFile.GetOutputSubDirectory ();
	MarkerSetName = inputFile.GetMarkerSetName ();
	UserLaneStandardName = inputFile.GetLaneStandardName ();
	OutputLevel = inputFile.GetCriticalOutputLevel ();
	Notice::SetSeverityTrigger (OutputLevel);
	SmartMessage::SetSeverityTrigger (OutputLevel);
	MessageBookPath = inputFile.GetFinalMessageBookName ();

	STRLCAnalysis::SetOutputSubDirectory (OutputSubDirectory);
	GenotypesForAMarkerSet::SetPathToStandardControlFile (ConfigDirectory);

	if (!inputFile.OverrideStringIsEmpty ())
		STRLCAnalysis::SetOverrideString (inputFile.GetOverrideString ());

	RGString CommandInputs;

	CommandInputs << "InputDirectory = " << PrototypeInputDirectory.GetData () << ";\n";
	CommandInputs << "LadderDirectory = " << LadderInformationDirectory.GetData () << ";\n";
	CommandInputs << "ReportDirectory = " << ParentDirectoryForReports.GetData () << ";\n";

	if (OutputSubDirectory.Length () > 0)
		CommandInputs << "OutputSubdirectory = " << OutputSubDirectory.GetData () << ";\n";

	CommandInputs << "MarkerSetName = " << MarkerSetName.GetData () << ";\n";
	CommandInputs << "LaneStandardName = " << UserLaneStandardName.GetData () << ";\n";
	CommandInputs << "CriticalOutputLevel = " << OutputLevel << ";\n";
	//cout << "MessageBook = " << MessageBookPath.GetData () << ";" << endl;

	stdSettingsFileName = inputFile.GetFinalStdSettingsName ();
	labSettingsFileName = inputFile.GetFinalLabSettingsName ();
	RGString fullPathMessageBookName = inputFile.GetFinalMessageBookName ();

	CommandInputs << "StandardSettings = " << stdSettingsFileName << ";\n";
	CommandInputs << "LabSettings = " << labSettingsFileName << ";\n";
	CommandInputs << "MessageBook = " << fullPathMessageBookName << ";\n";

	minSampleRFU = inputFile.GetMinSampleRFU ();
	minLaneStandardRFU = inputFile.GetMinLaneStandardRFU ();
	minLadderRFU = inputFile.GetMinLadderRFU ();
	minInterlocusRFU = inputFile.GetMinInterlocusRFU ();
	minLadderInterlocusRFU = inputFile.GetMinLadderInterlocusRFU ();
	sampleDetectionThreshold = inputFile.GetSampleDetectionThreshold ();

	CommandInputs << "MinSampleRFU = " << minSampleRFU << ";\n";
	CommandInputs << "MinLaneStandardRFU = " << minLaneStandardRFU << ";\n";
	CommandInputs << "MinLadderRFU = " << minLadderRFU << ";\n";
	CommandInputs << "MinInterlocusRFU = " << minInterlocusRFU << ";\n";
	CommandInputs << "MinLadderInterlocusRFU = " << minLadderInterlocusRFU << ";\n";
	CommandInputs << "SampleDetectionThreshold = " << sampleDetectionThreshold << ";\n";

	bool isLadderFree = false;
	size_t posn = 0;

	if (MarkerSetName.FindSubstringCaseIndependent ("LadderFree", posn))
		isLadderFree = true;

	inputFile.OutputAnalysisThresholdOverrides (CommandInputs);
	inputFile.OutputDetectionThresholdOverrides (CommandInputs);

	bool useRawData = inputFile.UseRawData ();

	if (useRawData) {

		CoreBioComponent::SetUseRawData ();
		CommandInputs << "RawDataString = R;\n\n\n";
		cout << CommandInputs.GetData ();
		cout << "Use raw data...\n";
	}

	else {

		CoreBioComponent::DontUseRawData ();
		CommandInputs << "RawDataString = A;\n\n\n";
		cout << CommandInputs.GetData ();
		cout << "Don't use raw data...\n";
	}

	cout << "\nNumber of arguments = " << argc << endl << endl;

	// It seems we are no longer using "TestRatio" and so no need to execute:  "Locus::SetTestRatio (testRatio);  // default = 1  (option = 0)"

	double minPeak = minSampleRFU;

	if (minLadderRFU < minPeak)
		minPeak = minLadderRFU;

	if (minLaneStandardRFU < minPeak)
		minPeak = minLaneStandardRFU;

	// This all assumes that reading the lab settings file below does not change the minRFU values...which was true as of 07/29/2009!!

	DataSignal::SetMinimumHeight (minPeak);
	STRSampleChannelData::SetSampleMinimumRFU (minSampleRFU);
	STRSampleChannelData::SetMinInterlocusRFU (minInterlocusRFU);
	STRLaneStandardChannelData::SetLSMinimumRFU (minLaneStandardRFU);
	STRLadderChannelData::SetLadderMinimumRFU (minLadderRFU);
	STRLadderChannelData::SetMinInterlocusRFU (minLadderInterlocusRFU);
	STRSampleChannelData::SetDetectionThreshold (sampleDetectionThreshold);

	RGFile stdSettingsFile (stdSettingsFileName, "rt");
	RGFile labSettingsFile (labSettingsFileName, "rt");

	if (!stdSettingsFile.isValid ()) {

		cout << "Could not open standard settings file " << stdSettingsFileName << ".  Exiting..." << endl;
		return -17;
	}

	if (!labSettingsFile.isValid ()) {

		cout << "Could not open laboratory settings file " << labSettingsFileName << ".  Exiting..." << endl;
		return -17;
	}

	RGString stdXML;
	stdXML.ReadTextFile (stdSettingsFile);
	RGString labXML;
	labXML.ReadTextFile (labSettingsFile);

	ParameterServer* pServer = new ParameterServer (stdXML);

	if (!pServer->isValid ()) {

		cout << "Could not interpret standard input settings file " << (char*)stdSettingsFileName.GetData () << ".  Exiting..." << endl;
		return -100;
	}

	else
		cout << "Read standard settings correctly..." << endl;

	pServer->SetChannelSpecificThresholds (inputFile.GetAnalysisThresholdOverrideList (), inputFile.GetDetectionThresholdOverrideList ());

	if (!pServer->AddGenotypeCollection (labXML, true)) {

		cout << "Could not interpret laboratory input settings file " << (char*)labSettingsFileName.GetData () << ".  Exiting..." << endl;
		return -100;
	}

	else
		cout << "Read lab settings correctly..." << endl;

	pServer->SetMarkerSetName (MarkerSetName);

	cout << "Window Width = " << TracePrequalification::GetWindowWidth () << endl;
	cout << "Min RFU for homozygote = " << Locus::GetMinBoundForHomozygote () << endl;

	if (UserLaneStandardName.Length () > 0)
		PopulationMarkerSet::SetUserLaneStandardName (UserLaneStandardName);

	size_t startPosition = 0;
	size_t endPosition;
	PrototypeInputDirectory.FindLastSubstring ("/", startPosition, endPosition);
	size_t nChars = PrototypeInputDirectory.Length () - endPosition - 1;
	RGString OutputDirectoryBase = PrototypeInputDirectory.ExtractLastCharacters (nChars);

	RGString FullPathForReports = ParentDirectoryForReports + "/" + OutputDirectoryBase;

	RGString DirectoryName = PrototypeInputDirectory;
	RGString CommandLineName = FullPathForReports + "/CommandLine.txt";

	cout << "Maximum sample peak = " << STRSampleChannelData::GetMaxRFU () << endl;
	bool isSmartMessageAnalysis = true;

	STRLCAnalysis analysis (LadderInformationDirectory, MarkerSetName, ParentDirectoryForReports, fullPathMessageBookName, isSmartMessageAnalysis, true);

	if (!analysis.isValid ())
		return -5;

	analysis.SetXMLCommandLineString (CommandLine);
	int status = 0;

	try {
		if (!isLadderFree)
			status = analysis.AnalyzeIncrementallySM (PrototypeInputDirectory, MarkerSetName, OutputLevel, graphicsDirectory, CommandInputs);

		else
			status = analysis.AnalyzeIncrementallySMLF (PrototypeInputDirectory, MarkerSetName, OutputLevel, graphicsDirectory, CommandInputs);
	}

	catch (...) {
		status = -1170;
	}

	if (status < 0) {

		cout << "Could not complete analysis...exiting with status:  " << status << endl;
		return status;
	}

	cout << endl << "Analysis Complete" << endl;
	return 0;

}





