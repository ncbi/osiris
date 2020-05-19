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
*  FileName: STRLCAnalysisSM.cpp
*  Author:   Robert Goor
*
*/
//
//  class STRLCAnalysis has global functions which analyze digital data signals according to specified rules, using the locus-centric classes:
//  SmartMessage functions only
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
#include "STRSmartMessage.h"
#include "STRSmartNotices.h"
#include "BaseGenetics.h"
#include "Genetics.h"
#include "OsirisVersion.h"
#include "TracePrequalification.h"
#include "LeastMedianOfSquares.h"
#include "STRLCAnalysis.h"
#include "ModPairs.h"
#include <list>
#include <iostream>
#include <time.h>
#include <stdio.h>

#define _MATRIX_OUTPUT_

using namespace std;


STRLCAnalysis :: STRLCAnalysis (PopulationCollection* collection, const RGString& parentDirectoryForReports, const RGString& messageBookPath, bool isSmartMsgAnalysis) :
SmartMessagingObject (), mCollection (collection), mParentDirectoryForReports (parentDirectoryForReports), WorkingFile (NULL) {

	RGString MessageBookName;
	mValid = false;
	PreInitializeSmartMessages ();
	RGString ThresholdFileName;

	if (isSmartMsgAnalysis) {

		MessageBookName = messageBookPath + "/OsirisMessageBookV4.0.xml";
		int status = STRBaseSmartMessage::LoadAndActivateAllMessages (MessageBookName);

		if (status < 0)
			return;

		//ThresholdFileName = messageBookPath + "/MessageBookThresholdsV3.1.xml";
		//status = STRBaseSmartMessage::LoadAndActivateAllThresholds (ThresholdFileName);
		ParameterServer* server = new ParameterServer;
		bool ans = server->SetAllSmartMessageThresholds ();
		delete server;
		STRLCAnalysis::CreateAllInitializationMatrices ();

		if (!ans)
			return;
	}

	else {

		MessageBookName = messageBookPath + "/OsirisMessageBook2.xml";
		RGFile messages (MessageBookName, "rt");
		

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
	}

	InitializeSmartMessages ();
	mValid = true;
}


STRLCAnalysis :: STRLCAnalysis (const RGString& ladderInformationDirectory, const RGString& markerSetName, const RGString& parentDirectoryForReports, const RGString& messageBookPath, bool isSmartMsgAnalysis) :
SmartMessagingObject (), mCollection (NULL), mParentDirectoryForReports (parentDirectoryForReports), WorkingFile (NULL) {

	RGString MessageBookName;
	mValid = false;
	PreInitializeSmartMessages ();
	RGString ThresholdFileName;

	if (isSmartMsgAnalysis) {

		MessageBookName = messageBookPath + "/OsirisMessageBookV4.0.xml";
		int status = STRBaseSmartMessage::LoadAndActivateAllMessages (MessageBookName);
		STRLCAnalysis::CreateAllInitializationMatrices ();

		if (status < 0)
			return;
	}

	mCollection = new STRPopulationCollection (ladderInformationDirectory, markerSetName);

	if (!mCollection->IsValid ()) {

		cout << mCollection->GetErrorString ().GetData () << endl;
		cout << "Ladder info file is invalid.  Exiting..." << endl;
		return;
	}

	else
		cout << "Ladder information opened successfully" << endl;

	if (isSmartMsgAnalysis) {

		ParameterServer* server = new ParameterServer;
		bool ans = server->SetAllSmartMessageThresholds ();
		delete server;
		STRLCAnalysis::CreateAllInitializationMatrices ();

		if (!ans)
			return;
	}

	else {

		MessageBookName = messageBookPath + "/OsirisMessageBook2.xml";
		RGFile messages (MessageBookName, "rt");
		

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
	}

	InitializeSmartMessages ();
	mValid = true;
}


STRLCAnalysis :: STRLCAnalysis (const RGString& ladderInformationDirectory, const RGString& markerSetName, const RGString& parentDirectoryForReports, const RGString& messageBookPath, bool isSmartMsgAnalysis, bool isFileInput) :
SmartMessagingObject (), mCollection (NULL), mParentDirectoryForReports (parentDirectoryForReports), WorkingFile (NULL) {

	RGString MessageBookName;
	mValid = false;
	PreInitializeSmartMessages ();
	int status;

	if (isSmartMsgAnalysis) {

		MessageBookName = messageBookPath;
		mMessageBookName = MessageBookName;
		status = STRBaseSmartMessage::LoadAndActivateAllMessages (MessageBookName);
		STRLCAnalysis::CreateAllInitializationMatrices ();

		if (status < 0)
			return;
	}

	mCollection = new STRPopulationCollection (ladderInformationDirectory, markerSetName);

	if (!mCollection->IsValid ()) {

		cout << mCollection->GetErrorString ().GetData () << endl;
		cout << "Ladder info file is invalid.  Exiting..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage (mCollection->GetErrorString ());
		STRLCAnalysis::mFailureMessage->LadderInfoInvalid ();
		return;
	}

	else
		cout << "Ladder information opened successfully" << endl;

	if (isSmartMsgAnalysis) {

		ParameterServer* server = new ParameterServer;
		bool ans = server->SetAllSmartMessageThresholds ();
		status = server->SetAllLocusSpecificThresholds (mCollection);
		STRLCAnalysis::CreateAllInitializationMatrices ();

		if (status >= 0)
			cout << "Locus specific thresholds set successfully." << endl;

		delete server;

		//if (!ans)   // ans is always true, so not needed
			//return;
	}

	else {

		MessageBookName = messageBookPath + "/OsirisMessageBook2.xml";
		RGFile messages (MessageBookName, "rt");
		

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
	}

	InitializeSmartMessages ();
	mValid = true;
}


int STRLCAnalysis :: AnalyzeIncrementallySM (const RGString& prototypeInputDirectory, const RGString& markerSet, int outputLevel, const RGString& graphicsDirectory, const RGString& commandInputs) {

	Boolean print = TRUE;
	RGString errorMsg;
	smDefaultsAreOverridden defaultsAreOverridden;
	smUseSampleNamesForControlSampleTestsPreset useSampleNamesForControlSampleTests;
	smMakeMixturesDefaultSampleTypePreset makeMixturesDefaultTypePreset;
	smSampleAnalysisTruncatedPrematurely analysisTruncatedPrematurely;
	smSamplesTerminatedPrematurely samplesTerminatedPrematurely;
	smSamplesHadNoDataForChannels samplesHadNoDataForChannels;
	smSampleMissingDataForChannels sampleMissingDataForChannels;
	smSamplesAreNotValidInputFiles samplesNotValidInputFiles;
	smSampleIsNotValidInputFile sampleNotValidInputFile;

	bool makeMixturesDefaultType = GetMessageValue (makeMixturesDefaultTypePreset);

	//double yLMS [8] = {579, 117, 103, 323, 276, 750, 689, 2070 };
	//double xLMS [8] = {8381, 7726, 7866, 8191, 7167, 9522, 9427, 11068.0};
	//double yLMS [9] = {159, 20, 1, 15, -1, 5, 2, -2, 20 };
	//double xLMS [9] = {1485, 1590, 1699, 1394, 1608, 1478, 1285, 1342, 1422};
	//double yLMS [8] = {34, 13, 16, 16, 11, 15, 18, 8 };
	//double xLMS [8] = {2109, 1625, 1402, 1309, 1141, 1162, 1696, 1823};
	//int iLMS;
	//int num = 8;

	//LeastMedianOfSquares1D lmsTest (num, xLMS, yLMS);
	//list<double> xs;
	//list<double> ys;

	//if (!lmsTest.DataIsOK ())
	//	cout << "Least median of squares data problematic" << endl;

	//else {

	//	double result = lmsTest.CalculateLMS ();

	//	if (result < 0.0) {

	//		cout << "Could not find valid least median values" << endl;
	//	}

	//	else {

	//		double leastMed = lmsTest.GetLMSValue ();
	//		cout << "Least median value = " << leastMed << endl;
	//		double lMed2 = lmsTest.GetMedianSquaredForLMS ();
	//		cout << "Least median squared = " << lMed2 << endl;
	//		cout << "Outlier threshold = " << lmsTest.GetOutlierThreshold () << endl;
	//		cout << "Data included in minimum:  ";

	//		for (iLMS=0; iLMS<num; iLMS++) {

	//			if (lmsTest.ElementIsOutlier (iLMS))
	//				continue;

	//			cout << yLMS [iLMS] << ", ";
	//			xs.push_back (xLMS [iLMS]);
	//			ys.push_back (yLMS [iLMS]);
	//		}

	//		cout << "..." << endl;
	//		cout << "Outliers:  ";

	//		for (iLMS=0; iLMS<num; iLMS++) {

	//			if (lmsTest.ElementIsOutlier (iLMS))
	//				cout << yLMS [iLMS] << ", ";
	//		}

	//		cout << "..." << endl;
	//	}
	//}

	//LeastSquaresQuadraticModel LQS (xs, ys);
	//xs.clear ();
	//ys.clear ();

	//if (!LQS.DataIsOK ())
	//	cout << "Least squares model could not set up..." << endl;

	//else {

	//	double linearLS;
	//	double quadraticLS;

	//	double LS = LQS.CalculateLeastSquare (linearLS, quadraticLS);

	//	cout << "\n\nLeast squares model:  linear = " << linearLS << ", and quadratic = " << quadraticLS << endl;
	//	cout << "Least squares = " << LS << endl << endl;
	//}

	//LeastMedianOfSquares2DExhaustive lmsTest2 (num, xLMS, yLMS);

	//if (!lmsTest2.DataIsOK ())
	//	cout << "Least median of squares 2 data problematic" << endl;

	//else {

	//	lmsTest2.CalculateLMS ();
	//	double leastMed = lmsTest2.GetLMSValue ();
	//	double leastMedSlope = lmsTest2.GetLMSValue2 ();
	//	cout << "Least median value = " << leastMed << " and Least median slope = " << leastMedSlope << endl;
	//	double lMed2 = lmsTest2.GetMedianSquaredForLMS ();
	//	cout << "Least median squared = " << lMed2 << endl;
	//	cout << "Outlier threshold = " << lmsTest2.GetOutlierThreshold () << endl;
	//	cout << "Data included in minimum:  ";

	//	for (iLMS=0; iLMS<num; iLMS++) {

	//		if (lmsTest2.ElementIsOutlier (iLMS))
	//			continue;

	//		cout << yLMS [iLMS] << ", ";
	//	}

	//	cout << "..." << endl;
	//	cout << "Outliers:  ";

	//	for (iLMS=0; iLMS<num; iLMS++) {

	//		if (lmsTest2.ElementIsOutlier (iLMS))
	//			cout << yLMS [iLMS] << ", ";
	//	}

	//	cout << "..." << endl;
	//}

	RGString localFileName;
	ParameterServer* pServer = new ParameterServer;
	GenotypeSet* gSet = pServer->GetGenotypeCollection ();

	if (gSet == NULL) {

		cout << "Could not retrieve genotype collection set from parameter server.  Exiting..." << endl;
		STRLCAnalysis::mFailureMessage->CouldNotRetrieveGenotypeCollection ();
		STRLCAnalysis::mFailureMessage->SetPingValue (280);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -20;
	}

	GenotypesForAMarkerSet* pGenotypes = gSet->FindMarkerSetCollection (markerSet);

	if (pGenotypes == NULL) {

		cout << "Could not retrieve genotype collection for marker set name " << markerSet << " from parameter server.  Exiting..." << endl;
		pGenotypes = new GenotypesForAMarkerSet;
		STRLCAnalysis::mFailureMessage->CouldNotRetrieveGenotype (markerSet);
		STRLCAnalysis::mFailureMessage->SetPingValue (290);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -20;
		//return -20;
	}

	if (!STRLCAnalysis::OverrideStringIsEmpty ()) {

		SetMessageValue (defaultsAreOverridden, true);
		AppendDataForSmartMessage (defaultsAreOverridden, STRLCAnalysis::GetOverrideString ());
	}

	//Notice* newNotice;
	//newNotice = new BlobFound;
	//CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	//newNotice = new SpikeFound;
	//CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	//newNotice = new PullUpFound;
	//CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	//newNotice = new InterLocusPeak;
	//CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

	//newNotice = new PullUpPrimaryChannel;
	//CoreBioComponent::AddNoticeToChannelArtifactTestList (newNotice);

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

	RGString outputSubDirectory = STRLCAnalysis::GetOutputSubDirectory ();

	if (outputSubDirectory.Length () > 0) {

		FullPathForReports += "/" + outputSubDirectory;
		RGDirectory::MakeDirectory (FullPathForReports);
	}

	cout << "Starting..." << endl;

	int i;

	//DirectoryManager LadderDirectory (DirectoryName);

	//if (!LadderDirectory.IsValid ()) {

	//	cout << "Could not find ladder directory:  " << DirectoryName << endl;
	//	cout << "Closing..." << endl;
	//	return -1;
	//}

	//else
	//	cout << "Ladder directory opened successfully" << endl;

	DirectoryManager* SampleDirectory;
	
	if (!GetMessageValue (useSampleNamesForControlSampleTests))
		SampleDirectory = new DirectoryManager (DirectoryName);

	else
		SampleDirectory = new SampleNameDirectoryManager (DirectoryName);

	if (!SampleDirectory->IsValid ()) {

		cout << "Could not find sample directory:  " << DirectoryName << endl;
		cout << "Closing..." << endl;
		STRLCAnalysis::mFailureMessage->CouldNotFindSampleDirectory (DirectoryName);
		STRLCAnalysis::mFailureMessage->SetPingValue (300);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -1;
	}

	else
		cout << "Sample directory opened successfully" << endl;

	int ChannelNumber = 1;
	SampleDirectory->Initialize ();
	int NSampleFiles = SampleDirectory->GetNumberOfFilesInDirectory ();
	double Progress;
	int SamplesProcessed = 0;

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
	RGString MsgBookOutputFullPath = FullPathForReports + "/" + OutputFileName + ".msgBook.xml";

	RGString tempSummaryFullPath = FullPathForReports + "/temp" + OutputFileName + "Summary.txt";
	RGString tempSummaryFullPathWithLinks = FullPathForReports + "/temp" + OutputFileName + ".txt";
	RGString tempXMLSummaryFullPathWithLinks = FullPathForReports + "/temp" + OutputFileName + "XML.txt";

	RGString debugOutputFullPath = FullPathForReports + "/" + OutputFileName + "Debug.txt";

	RGFile OutputSummary (SummaryFullPath, "wt");
	RGFile OutputSummaryLinks (SummaryFullPathWithLinks, "wt");
	RGFile XMLOutputSummaryLinks (XMLSummaryFullPathWithLinks, "wt");
	RGFile MsgBookOutputFile (MsgBookOutputFullPath, "wt");

	if (MsgBookOutputFile.isValid ()) {

		SmartMessage::WriteMsgBookFile (MsgBookOutputFile);
		MsgBookOutputFile.Flush ();
	}

	MsgBookOutputFile.Close ();

	RGString OutputLadderInfo = FullPathForReports + "/LadderInfo.xml";
	RGTextOutput* LadderInfo = new RGTextOutput (OutputLadderInfo, FALSE);

	if (LadderInfo->FileIsValid ()) {

		*LadderInfo << mCollection->GetGridData ();
	}

	delete LadderInfo;

	RGFile tempOutputSummary (tempSummaryFullPath, "wt");
	RGFile tempOutputSummaryLinks (tempSummaryFullPathWithLinks, "wt");
	RGFile tempXMLOutputSummaryLinks (tempXMLSummaryFullPathWithLinks, "wt");

	RGString ConsoleOutputFileName = OutputFileName + "ConsoleData.txt";
	RGString WholeConsoleName = FullPathForReports + "/" + ConsoleOutputFileName;

	WorkingFileName = FullPathForReports + "/Working.txt";
	WorkingFile = new RGFile (WorkingFileName, "wt");

	if (!WorkingFile->isValid ())
		cout << "Could not open output file:  " << WorkingFileName << endl;

	RGString CInputFileName = FullPathForReports + "/BaseInputFile.txt";
	RGTextOutput CInputFile (CInputFileName, FALSE);
	
	RGTextOutput text (WholeConsoleName, FALSE);
//	RGTextOutput wholeSample (WholeFullPath, FALSE);
	OsirisMsg Message (&OutputFile, "\t", 10);
	Endl endLine;

	//RGString accumulatedHeightDataName = FullPathForReports + "/PeakHeightData.tab";
	//RGTextOutput* heightText = new RGTextOutput (accumulatedHeightDataName, FALSE);

	//if (heightText->FileIsValid ()) {

	//	*heightText << "Cumulative Height Data" << endLine;
	//	*heightText << "A = Corrected Peak Heights" << endLine;
	//	*heightText << "B = Uncorrected Peak Heights" << endLine;
	//	*heightText << "C = percent A / B" << endLine;
	//	*heightText << "D = Corrected Peak Heights of Corrected Peaks Only" << endLine;
	//	*heightText << "E = Uncorrected Peak Heights of Corrected Peaks Only" << endLine;
	//	*heightText << "F = percent D / E" << endLine;
	//	*heightText << "G = Total Heights of Primary Peaks" << endLine;
	//	*heightText << "H = percent G / B" << endLine;
	//	*heightText << "I = Corrected Peak Heights of Corrected Partial Pullup Peaks Only" << endLine;
	//	*heightText << "J = percent I / B" << endLine;
	//	*heightText << "K = Uncorrected Peak Heights of Partial Pullup Peaks Only" << endLine;
	//	*heightText << "L = percent I / K" << endLine << endLine;
	//	*heightText << "A\t\tB\t\tC\t\tD\t\tE\t\tF\t\tG\t\tH\t\tI\t\tJ\t\tK\t\tL" << endLine;
	//}

	//CoreBioComponent::SetHeightFile (heightText);

	smMinImbalanceThresholdForCreatingNoisyPeak noiseImbalanceThreshold;

	double heightFraction = 0.01 * (double)GetThreshold (noiseImbalanceThreshold);
	Locus::SetImbalanceThresholdForNoisyPeak (heightFraction);
	cout << "Imbalance threshold for noisy peak = " << Locus::GetImbalanceThresholdForNoisyPeak () << "\n";

	RGString nonLaserOffScalePullupFractionName = FullPathForReports + "/PullupFractions.tab";
	RGTextOutput* nonLaserOffScalePullupFractions = new RGTextOutput (nonLaserOffScalePullupFractionName, FALSE);
	CoreBioComponent::SetNonLaserOffScalePUCoeffsFile (nonLaserOffScalePullupFractions);

	if (!CInputFile.FileIsValid ()) {

		cout << "Could not open Command Input file:  " << CInputFileName << endl;
		cout << "Information is reproduced in Details below\n";
	}

	else {

		CInputFile << commandInputs;
	}

	if (!OutputFile.isValid ()) {

		cout << "Could not open output file:  " << OutputFullPath << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File path:  " + OutputFullPath);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Output File");
		STRLCAnalysis::mFailureMessage->SetPingValue (310);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -1;
	}

	if (!text.FileIsValid ()) {

		cout << "Could not open text echo file:  " << WholeConsoleName << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File path:  " + WholeConsoleName);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Text Echo File");
		STRLCAnalysis::mFailureMessage->SetPingValue (320);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!OutputSummary.isValid ()) {

		cout << "Could not open output summary file:  " << SummaryFullPath << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File path:  " + SummaryFullPath);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Output Summary File");
		STRLCAnalysis::mFailureMessage->SetPingValue (330);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!OutputSummaryLinks.isValid ()) {

		cout << "Could not open output summary file with embedded links:  " << SummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File path:  " + SummaryFullPathWithLinks);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Output Summary File With Embedded Links");
		STRLCAnalysis::mFailureMessage->SetPingValue (340);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!XMLOutputSummaryLinks.isValid ()) {

		cout << "Could not open output xml summary file with embedded links:  " << XMLSummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File path:  " + XMLSummaryFullPathWithLinks);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("XML Output Summary File With Embedded Links");
		STRLCAnalysis::mFailureMessage->SetPingValue (350);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!tempOutputSummary.isValid ()) {

		cout << "Could not open temporary output summary file:  " << tempSummaryFullPath << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File path:  " + tempSummaryFullPath);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Temporary Summary File");
		STRLCAnalysis::mFailureMessage->SetPingValue (360);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!tempOutputSummaryLinks.isValid ()) {

		cout << "Could not open temporary output summary file with embedded links:  " << tempSummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File path:  " + tempSummaryFullPathWithLinks);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Temporary Summary File with Embedded Links");
		STRLCAnalysis::mFailureMessage->SetPingValue (370);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!tempXMLOutputSummaryLinks.isValid ()) {

		cout << "Could not open temporary output xml summary file with embedded links:  " << tempXMLSummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File path:  " + tempXMLSummaryFullPathWithLinks);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("XML Temporary Summary File with Embedded Links");
		STRLCAnalysis::mFailureMessage->SetPingValue (380);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
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
	SmartMessage::CreateDebugFile (debugOutputFullPath);
	GridDataStruct* GridData = new GridDataStruct (mCollection, markerSetName, testPeak, text, ExcelText, Message, print);
	SampleDataStruct* SampleData = new SampleDataStruct (mCollection, markerSetName, testPeak, testPeak, text, ExcelText, Message, print);
	PopulationMarkerSet* set = mCollection->GetNamedPopulationMarkerSet (markerSetName);

	if (set == NULL) {

		cout << "Could not find marker set named:  " << markerSetName << ".  Ending..." << endl;
		STRLCAnalysis::mFailureMessage->CouldNotFindNamedMarkerSet (markerSetName);
		STRLCAnalysis::mFailureMessage->SetPingValue (390);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
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
	DataSignal::SetNumberOfChannels (expectedNumberOfChannels);
	Locus::SetNumberOfChannels (expectedNumberOfChannels);

	pServer->Report (text, "");
	pServer->Report (ExcelText, "");
	ExcelText.ResetOutputLevel ();

	ExcelSummary.SetOutputLevel (1);

	int minSampleRFU = (int)floor (STRSampleChannelData::GetMinRFU () + 0.5);
	int minLadderRFU = (int)floor (STRLadderChannelData::GetMinRFU () + 0.5);
	int minLaneStdRFU = (int)floor (STRLaneStandardChannelData::GetMinRFU () + 0.5);
	int minInterlocusRFU = (int)floor (STRSampleChannelData::GetMinInterlocusRFU () + 0.5);
	int minLadderInterlocusRFU = (int)floor (STRLadderChannelData::GetMinInterlocusRFU () + 0.5);
	int minRFUSampleDetectionRFU = (int)floor (STRSampleChannelData::GetSampleDetectionThreshold () + 0.5);

	ExcelSummary << "OAR 2.0" << endLine;
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

	ExcelLinks << "OAR 2.0" << endLine;
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
	XMLExcelLinks << "<OsirisAnalysisReport>\n\t<Version>" << OSIRIS_VERSION << "</Version>\n\t";
	XMLExcelLinks << "<FullVersion>" << OSIRIS_FULLNAME << "</FullVersion>\n\t";
	XMLExcelLinks << "<MsgBookBuildTime>" << STRLCAnalysis::GetMsgBookBuildTime () << "</MsgBookBuildTime>\n\t<Heading>\n\t\t<FileName>";
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
	XMLExcelLinks << "\t\t\t<minRFUladderInterlocus>" << minLadderInterlocusRFU << "</minRFUladderInterlocus>\n";
	XMLExcelLinks << "\t\t\t<minRFUsampleDetection>" << minRFUSampleDetectionRFU << "</minRFUsampleDetection>\n";
	pResult = pServer->BuildChannelThresholdOverridesForOAR ();

	if (pResult.Length () > 0)
		XMLExcelLinks << pResult;

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

	PlotString << "\t<Version>" << OSIRIS_VERSION << "</Version>\n";
	PlotString << "\t<FullVersion>" << OSIRIS_FULLNAME << "</FullVersion>\n";
	PlotString << "\t<MsgBookBuildTime>" << STRLCAnalysis::GetMsgBookBuildTime () << "</MsgBookBuildTime>\n";
	PlotString << "\t<parameters>\n\t\t<inputDirectory>" << xmlwriter::EscAscii (DirectoryName, &pResult) << "</inputDirectory>\n\t\t<outputDirectory>";
	PlotString << xmlwriter::EscAscii (FullPathForReports, &pResult) << "</outputDirectory>\n\t\t<kit>";
	PlotString << xmlwriter::EscAscii (markerSet, &pResult) << "</kit>\n\t\t<ls>";
	PlotString << lsName << "</ls>\n\t\t<minRFUsample>" << minSampleRFU << "</minRFUsample>\n";

	pResult = pServer->BuildChannelThresholdOverridesForPLT ();

	if (pResult.Length () > 0)
		PlotString << pResult;

	PlotString << "\t\t<minRFUILS>" << minLaneStdRFU << "</minRFUILS>\n\t\t<minRFUladder>" << minLadderRFU;
	PlotString << "</minRFUladder>\n\t\t<minRFUinterlocus>" << minInterlocusRFU << "</minRFUinterlocus>\n";
	PlotString << "\t\t<minRFUladderInterlocus>" << minLadderInterlocusRFU << "</minRFUladderInterlocus>\n";
	PlotString << "\t\t<minRFUsampleDetection>" << minRFUSampleDetectionRFU << "</minRFUsampleDetection>\n";
	PlotString << "\t\t<DataAnalyzed>";

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
//	RGString OsirisGraphics (graphicsDirectory);
	bool ladderOK;
	bool hasPosControl = false;
	bool hasNegControl = false;
	bool foundALadder = true;
	smLadderFailed ladderFailed;
	smSampleIsLadder sampleIsLadder;
	smSampleUnanalyzable sampleFailed;
	smSampleIsPosCtrl sampleIsPosCtrl;
	smSampleIsNegCtrl sampleIsNegCtrl;
	smNoPosCtrlFound noPosCtrlFound;
	smNoNegCtrlFound noNegCtrlFound;
	smSampleSatisfiesPossibleMixtureIDCriteria sampleSatisfiesMixtureCriteria;
	smDisableLowLevelFiltersForKnownMixturesPreset disableLowLevelFilters;
	smSaveLadderILSHistoryPreset saveLadderILSHistory;
	smLatitudeForILSFit latitudeForILSFit;
	smUseLadderEndPointILSAlgorithmPreset useLadderEndPointILSAlgorithm;
	smPlusLatitudeForLadderEndPointILSFit plusLatitudeForLadderEndPointILSFit;
	smTailHeightFittingThresholdFactor tailFittingHeightModifier;
	smTailSlopeFittingThresholdFactor tailFittingSlopeModifier;

	double tailHeightModifier = 0.01 * (double) GetThreshold (tailFittingHeightModifier);
	double tailSlopeModifier = 0.01 * (double) GetThreshold (tailFittingSlopeModifier);
	TracePrequalification::SetLowHeightModifier (tailHeightModifier);
	TracePrequalification::SetLowSlopeModifier (tailSlopeModifier);

	smStage1Successful stage1Successful;
	smStage2Successful stage2Successful;
	smStage3Successful stage3Successful;
	smStage4Successful stage4Successful;
	smStage5Successful stage5Successful;

	smDisableStutterFilter disableStutterFilter;
	smDisableAdenylationFilter disableAdenylationFilter;
	smCallOnLadderAdenylationPreset callOnLadderAdenylation;

	RGString pullupMatrixFileName = FullPathForReports + "/matrix.txt";
	RGTextOutput* pullupMatrixFile = NULL;

	if (GetMessageValue (saveLadderILSHistory)) {

		STRLCAnalysis::SetCollectILSHistory (true);
		ChannelData::SetUseILSHistory (false);

		int threshold = GetThreshold (latitudeForILSFit);

		if (threshold > 0)
			ChannelData::SetLatitudeFactorForILSHistory (0.0001 * (double) threshold);

		else
			ChannelData::SetLatitudeFactorForILSHistory (0.01);

		cout << "Collecting Ladder ILS History with latitude factor = " << 0.0001 * (double) threshold << "\n";
	}

	else
		cout << "Not collecting ladder ILS History...\n";

	if (GetMessageValue (useLadderEndPointILSAlgorithm)) {

		ChannelData::SetUseILSLadderEndPointAlgorithm (true);
		ChannelData::SetLatitudeFactorForLadderILS (0.02);
		cout << "Using End Point Algorithm for Ladder ILS Analysis...\n";
		int threshold2 = GetThreshold (plusLatitudeForLadderEndPointILSFit);  // Substitute ladder latitude

		if (threshold2 > 0)
			ChannelData::SetLatitudeFactorForLadderILS (0.0001 * (double) threshold2); // Substitute ladder latitude

		else
			ChannelData::SetLatitudeFactorForLadderILS (0.05);  // Substitute ladder latitude

		cout << "Testing Ladder ILS start and end points with latitude factor = " << 0.0001 * (double) threshold2 << "\n";
	}

	else
		cout << "Not using End Point Algorithm for Ladder ILS Analysis...\n";

	bool ignoreNoise;

	//if (OsirisGraphics.GetLastCharacter () != '/')
	//	OsirisGraphics += "/";

/*	if (graphicsDirectory.Length () > 0)
		printGraphics = true;

	else
		printGraphics = false;*/

//	RGString GraphicsDirectory = OsirisGraphics + OutputDirectoryBase;
	RGString GraphicsDirectory = FullPathForReports;
	RGString idString;

	cout << "Expected dye names:  ";

	for (i=1; i<=expectedNumberOfChannels; i++) {

		if (i > 1)
			cout << ", ";

		cout << CoreBioComponent::GetDyeName (i).GetData ();
	}

	cout << "\n\n";

	//if (printGraphics)
	//	RGDirectory::MakeDirectory (GraphicsDirectory);

	SmartMessagingComm commSM;
	commSM.SMOStack [0] = (SmartMessagingObject*) this;
	int numHigherObjects = 2;
	bool isFirstLadder = true;
	RGString ABIModelNumber;
	int nLadders = 0;
	//ChannelData::SetTestForDualSignal (false);
	ChannelData::SetTestForDualSignal (true);    // 01/24/2017 This is a test because ladders are missing split peaks that should not be missed.  It used to be set to true.

	double leastBP = set->GetMinimumILSBPSearchRegion ();
	CoreBioComponent::SetMinBioIDForLadderLoci (leastBP);
	cout << "Minimum ILS BP for ladder loci = " << leastBP << "\n";

	int leastBPForSamples = CoreBioComponent::GetMinBioIDForArtifacts ();
	int leastBPInt = (int) floor (leastBP);
	int oldLeastBPForSamples = leastBPForSamples;
	double minLadderILSBP = 0.0;
	bool testedLadderMinILSBP = false;

	if ((leastBPForSamples > 0) && (leastBPForSamples > leastBPInt)) {

		cout << "Minimum ILS BP for sample artifacts setting = " << leastBPForSamples << " is too high.  Resetting to " << leastBPInt << "\n\n";
		CoreBioComponent::SetMinBioIDForArtifacts (leastBPInt);
	}

	else
		cout << "\n";

	RGString commentField;
	RGString modsFileString;
	RGString modsFileName = PrototypeInputDirectory + "/batch-mods.txt";
	RGFile modsFile (modsFileName, "rt");
	OverallModList* oml = NULL;
	SampleModList* sml = NULL;

	if (modsFile.isValid ()) {

		modsFileString.ReadTextFile (modsFile);
		oml = new OverallModList (expectedNumberOfChannels);
		oml->ReadOverallModList (modsFileString);
		//cout << "Read mods file containing:  \n";
		//cout << modsFileString << endl;
		//oml->PrintList ();
	}

	while (SampleDirectory->GetNextLadderFile (LadderFileName, cycled) && !cycled) {

		FullPathName = DirectoryName + "/" + LadderFileName;
		cout << "Found ladder name " << (char*)FullPathName.GetData () << endl;
		nLadders++;

		if (oml != NULL)
			sml = oml->GetSampleModList (LadderFileName);

		else
			sml = NULL;

		if (sml != NULL) {

			cout << "Found mods file for ladder named " << LadderFileName << endl;
			sml->PrintList ();
		}

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
			STRLCAnalysis::mFailureMessage->AddMessage ("Data is not valid in ladder named " + LadderFileName + "...Skipping");
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
		ABIModelNumber = stringData;

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

			errorMsg << "Markerset mismatch...expecting " << expectedNumberOfChannels << " channels and ladder file contains " << NChannels << " channels for ladder named " << FullPathName << " ...Terminating";
			STRLCAnalysis::mFailureMessage->LadderSpecificationMismatch (errorMsg);
			STRLCAnalysis::mFailureMessage->SetPingValue (400);
			STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();

			XMLExcelLinks << CLevel (1) << "\t\t<Sample>\n\t\t\t<Name>Marker Set Mismatch</Name>\n\t\t\t<Type></Type>\n\t\t</Sample>\n" << PLevel ();
			XMLExcelLinks << CLevel (1) << "\t</Table>\n" << PLevel ();
			XMLExcelLinks << CLevel (1) << "</OsirisAnalysisReport>" << endLine << PLevel ();
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
		ladderBioComponent->SetSampleName (data->GetSampleName ());
		ladderBioComponent->SetFileName (LadderFileName);
		ladderBioComponent->SetSampleModifications (sml);
		commSM.SMOStack [1] = (SmartMessagingObject*) ladderBioComponent;
		ladderBioComponent->SetMessageValue (sampleIsLadder, true);

		if (isFirstLadder) {

			isFirstLadder = false;
			ladderBioComponent->ReevaluateNoiseThresholdBasedOnMachineType (ABIModelNumber);
		}

		commentField = data->GetComment ();
		ladderBioComponent->SetComments (commentField);

		ignoreNoise = ladderBioComponent->GetIgnoreNoiseAboveDetectionInSmoothingFlag ();
		SampledData::SetIgnoreNoiseAnalysisAboveDetectionInSmoothing (ignoreNoise);

		status = ladderBioComponent->AnalyzeGridSM (*data, GridData);	// after this, stage 2 is complete; skip stage 3;

		if (status >= 0)
			ladderBioComponent->GetAllAmbientData (data);

		//
		// End Stage 1 here!!!
		//
		
		ladderBioComponent->SetMessageValue (stage1Successful, true);
		ladderBioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 1, true, false);

//		ladderBioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 1, true, false);
//		ladderBioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 1, true, false);

		if ((status >= 0) && (ladderBioComponent->AnalyzeGridLociSM (GridData->mText, GridData->mExcelText, GridData->mMsg, print) < 0)) {

			// ErrorString already populated; if status is already negative, we'll come here anyway
			status = -4;
		}

		if (status == 0) {

			ladderBioComponent->SetProgress (6);
		}

		ladderBioComponent->MakeNonCoreLadderArtifactsNoncritical ();

		//
		// End Stage 2 here!!!
		//

		ladderBioComponent->SetMessageValue (stage2Successful, true);
		ladderBioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 2, true, false);

//		ladderBioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 2, true, false);
//		ladderBioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 2, true, false);

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

		SamplesProcessed++;
		Progress = 100.0 * (double)SamplesProcessed / (double)NSampleFiles;
		cout << "Progress = " << Progress << "%." << endl;

		delete data;
		data = NULL;
		
		if ((status < 0) || (ladderBioComponent->GridQualityTestSM () < 0)) {

			ladderOK = false;
			RGString ladderName = "-- " + ladderBioComponent->GetSampleName ();
			ladderBioComponent->SetMessageValue (ladderFailed, true);
			ladderBioComponent->AppendDataForSmartMessage (ladderFailed, ladderName);
		}

		ladderBioComponent->SetMessageValue (stage3Successful, true);
		ladderBioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 3, true, false);

		//
		// Now end stages 4 and, for peaks only, stage 5
		//

		ladderBioComponent->SetMessageValue (stage4Successful, true);
		ladderBioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 4, true, false);

		if (ladderOK)
			ladderBioComponent->RemoveInterlocusSignalsSM ();

//		ladderBioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 4, true, false);
//		ladderBioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 4, true, false);

		if ((ladderOK) && (ladderBioComponent->GridQualityTestSMPart2 (commSM, numHigherObjects) < 0)) {

			ladderOK = false;
			RGString ladderName = "-- " + ladderBioComponent->GetSampleName ();
			ladderBioComponent->SetMessageValue (ladderFailed, true);
			ladderBioComponent->AppendDataForSmartMessage (ladderFailed, ladderName);
		}

//		ladderBioComponent->FilterSmartNoticesBelowMinBioID ();  // Already done earlier and too late now anyhow...signals are all wrapped up in GridQualityTestSM
		ladderBioComponent->OrganizeNoticeObjectsSM ();

		//
		// Finish stage 5 for grid and add all (non-signal) SmartMessageReporters
		//

		ladderBioComponent->SetMessageValue (stage5Successful, true);
		ladderBioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 5, false, false);	// this is for non-signals only

//		ladderBioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 5, false, false);	// this is for non-signals only
//		ladderBioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 5, false, false);	// these are for non-signals only (signals have already been done)
		ladderBioComponent->AddAllSmartMessageReporters (commSM, numHigherObjects);	// this does non-signals only; signals have already been done

		ladderBioComponent->WriteXMLGraphicDataSM (GraphicsDirectory, LadderFileName, data, 4, PlotString);
		ladderBioComponent->PrepareLociForOutput ();
		ladderBioComponent->ReportGridTableRow (ExcelSummary);
		ladderBioComponent->ReportGridTableRowWithLinks (ExcelLinks);
		ladderBioComponent->ReportXMLSmartGridTableRowWithLinks (XMLExcelLinks, tempXMLExcelLinks);
		ladderBioComponent->ReportAllSmartNoticeObjects (tempExcelSummary, "", " ", FALSE);
		ladderBioComponent->ReportAllSmartNoticeObjects (tempExcelLinks, "", " ", TRUE);

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

			if (CollectILSHistory) {

				ladderBioComponent->AddILSToHistory ();
				cout << "Ladder ILS added to history..." << endl;
			}

			//  Find minILSBP for ladder locus and save

			int temp = ladderBioComponent->GetMinimumILSBPForLoci ();

			if (temp < oldLeastBPForSamples)
				oldLeastBPForSamples = temp;
		}
	}

	cout << "Processed all ladders.  Number of ladders = " << LadderList.Entries () << endl;
	ChannelData::SetTestForDualSignal (true);
	ChannelData::SetUseILSLadderEndPointAlgorithm (false);
	CoreBioComponent::SetMinBioIDForArtifacts (oldLeastBPForSamples);
	cout << "Minimum ILS BP for reporting alleles and artifacts = " << oldLeastBPForSamples << "\n";
	RGString SampleName;
	bool sampleOK;
	bool populatedBaseLocusList = false;
	bool possibleMixture;

	if (nLadders == 0) {

		NoticeStr << "PROJECT DID NOT MEET EXPECTATIONS...NO LADDER FOUND IN DIRECTORY...ENDING";
		STRLCAnalysis::mFailureMessage->NoLadderDataFound ();
		STRLCAnalysis::mFailureMessage->SetPingValue (410);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		cout << NoticeStr << endl;
		ExcelText << CLevel (1) << NoticeStr << "\n" << PLevel ();
		text << NoticeStr << "\n";
		foundALadder = false;
		return -42;
	}

	if (LadderList.Entries () == 0) {
		
		STRLCAnalysis::mFailureMessage->NoLaddersAnalyzedSuccessfully ();
		STRLCAnalysis::mFailureMessage->SetPingValue (420);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		NoticeStr << "PROJECT DID NOT MEET EXPECTATIONS...NO SATISFACTORY LADDER FOUND...ENDING";
		cout << NoticeStr << endl;
		ExcelText << CLevel (1) << NoticeStr << "\n" << PLevel ();
		text << NoticeStr << "\n";
		foundALadder = false;
	//	goto finishOutput;
	}

	else {

		ladderBioComponent = (CoreBioComponent*) LadderList.First ();
		int numChannels = ladderBioComponent->GetNumberOfChannels ();

		if (nonLaserOffScalePullupFractions->FileIsValid ()) {

			*nonLaserOffScalePullupFractions << "Laser Not Off Scale Pullup Fractions (Linear and Quadratic Parts) Per Channel Pairs" << endLine;
			int ii;
			int jj;

			for (ii=1; ii<=numChannels; ii++) {

				for (jj=1; jj<=numChannels; jj++) {

					if (ii == jj)
						continue;

					*nonLaserOffScalePullupFractions << ii << "->" << jj << " lin.\t" << ii << "->" << jj << " quad.\t";
				}
			}

			*nonLaserOffScalePullupFractions << endLine;
		}
	}

	if (foundALadder) {
//		goto finishOutput;

	SampleDirectory->RewindDirectory ();
	pServer->AddLabPositiveControlsToControlStrings (pGenotypes);
	
	if (CollectILSHistory) {

		STRLCAnalysis::SetCollectILSHistory (false);
		ChannelData::SetUseILSHistory (true);
		ladderBioComponent = (CoreBioComponent*) LadderList.First ();
		ladderBioComponent->ResetBoundsForILSUsingFactor (ChannelData::GetLatitudeFactorForILSHistory ());
		cout << "All ladder ILS history collected and bounds reset..." << endl;
	}

#ifdef _MATRIX_OUTPUT_
	pullupMatrixFile = new RGTextOutput (pullupMatrixFileName, FALSE);

	if (!pullupMatrixFile->FileIsValid ()) {

		cout << "Could not open Matrix Output File:  " << pullupMatrixFileName << endl;
		delete pullupMatrixFile;
		pullupMatrixFile = NULL;
	}

	CoreBioComponent::SetPullupMatrixFile (pullupMatrixFile);
#endif

	// Modify below functions to accumlate partial work, as possible, in spite of "errors", and report

	int CrashResponse = 10;

	while (SampleDirectory->GetNextOrderedSampleFile (FileName)) {

		CoreBioComponent::ResetCrashMode (false);
		CoreBioComponent::SetCurrentStage (1);
		CoreBioComponent::AddOneToCrashCount ();
		CoreBioComponent::ResetNoDataChannels ();
		CoreBioComponent::ResetCrashCode ();

		if (oml != NULL)
			sml = oml->GetSampleModList (FileName);

		else
			sml = NULL;

		CrashResponse = 10;
		sampleOK = true;
		localFileName = FileName;
		FullPathName = DirectoryName + "/" + FileName;

		if (WorkingFile != NULL) {

			WorkingFile->Write (FullPathName + "\n");
			WorkingFile->Flush ();
		}

		data = new fsaFileData (FullPathName);
		bioComponent = new STRSampleCoreBioComponent (data->GetName ());
		bioComponent->SetSampleName (data->GetSampleName ());
		bioComponent->SetFileName (FileName);
		Locus::SetCallOnLadderAdenylation (bioComponent->GetMessageValue (callOnLadderAdenylation));
		Locus::SetSingleSourceSample (false);

		bioComponent->SetSampleModifications (sml);

		commentField = data->GetComment ();
		bioComponent->SetComments (commentField);

		try {  //#######

			if (!data->IsValid ()) {

				CoreBioComponent::SetCrashCode (37);
				throw 37;
			}

			if (GetMessageValue (useSampleNamesForControlSampleTests))
				bioComponent->SetControlIdName (bioComponent->GetDataSampleName ());

			else
				bioComponent->SetControlIdName (FileName);

			commSM.SMOStack [1] = (SmartMessagingObject*)bioComponent;

			if (bioComponent->PrepareSampleForAnalysisSM (*data, SampleData) < 0) {

				sampleOK = false;
				NoticeStr = "";
				NoticeStr << "COULD NOT INITIALIZE AND PREPARE FOR ANALYSIS, FOR FILE:  " << FileName << "\n";
				ExcelText << CLevel (1) << NoticeStr << bioComponent->GetError ();
				ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
				cout << NoticeStr.GetData () << endl;
				NoticeStr = "";
				bioComponent->SetMessageValue (sampleFailed, true);
			}

			if (sampleOK && !populatedBaseLocusList) {

				populatedBaseLocusList = true;
				bioComponent->AppendAllBaseLociToList (mBaseLocusList);
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

			if (sampleOK && (bioComponent->PreliminarySampleAnalysisSM (LadderList, SampleData) < 0)) {

				sampleOK = false;
				NoticeStr = "";
				NoticeStr << "COULD NOT PERFORM PRELIMINARY ANALYSIS, FILE:  " << FileName << "\n";
				ExcelText << CLevel (1) << NoticeStr << bioComponent->GetError () << "\n";
				ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
				NoticeStr = "";
				bioComponent->SetMessageValue (sampleFailed, true);
			}

			else
				bioComponent->GetAllAmbientData (data);

			//cout << "Preliminary Analysis Complete" << endl;

			bioComponent->SetNegativeControlFalseSM ();
			bioComponent->SetPositiveControlFalseSM ();
			idString = bioComponent->GetControlIdName ();
			Locus::SetDisableAdenylationFilter (false);
			Locus::SetDisableStutterFilter (false);
			ChannelData::SetDisableAdenylationFilter (false);
			ChannelData::SetDisableStutterFilter (false);
			possibleMixture = false;
			//	cout << "ID String = " << (char*) idString.GetData () << " for file name " << (char*) FileName.GetData () << endl;
			Locus::SetControlSample (false);

			if (sampleOK && pServer->ControlDoesTargetStringContainASynonymCaseIndep (idString)) {

				if (pServer->NegControlDoesTargetStringContainASynonymCaseIndep (idString)) {

					hasNegControl = true;;
					bioComponent->SetNegativeControlTrueSM ();
					bioComponent->SetMessageValue (sampleIsNegCtrl, true);
					Locus::SetSingleSourceSample (true);
					Locus::SetControlSample (true);
				}

				else if (pServer->PosControlDoesTargetStringContainASynonymCaseIndep (idString)) {

					hasPosControl = true;
					bioComponent->SetPositiveControlTrueSM ();
					bioComponent->SetMessageValue (sampleIsPosCtrl, true);
					Locus::SetSingleSourceSample (true);
					Locus::SetControlSample (true);
				}

				bioComponent->SetPossibleMixtureIDFalseSM ();
				bioComponent->SetMessageValue (sampleSatisfiesMixtureCriteria, false);
			}

			//else if (bioComponent->IsLabPositiveControl (idString, pGenotypes)) {

			//	hasPosControl = true;
			//	bioComponent->SetPositiveControlTrueSM ();
			//	bioComponent->SetMessageValue (sampleIsPosCtrl, true);
			//	bioComponent->SetPossibleMixtureIDFalseSM ();
			//	bioComponent->SetMessageValue (sampleSatisfiesMixtureCriteria, false);
			//}

			//else if (sampleOK && bioComponent->GetMessageValue (disableLowLevelFilters)) {

			else if (sampleOK && bioComponent->GetMessageValue (disableLowLevelFilters)) {

				if (pServer->DoesTargetStringContainMixtureCriteriaCaseIndep (idString, makeMixturesDefaultType)) {

					bioComponent->SetPossibleMixtureIDTrueSM ();
					bioComponent->SetMessageValue (sampleSatisfiesMixtureCriteria, true);
					possibleMixture = true;
					Locus::SetSingleSourceSample (false);
					cout << "Sample is being treated as a mixture\n";
				}

				else {

					bioComponent->SetPossibleMixtureIDFalseSM ();
					bioComponent->SetMessageValue (sampleSatisfiesMixtureCriteria, false);
					possibleMixture = false;
					Locus::SetSingleSourceSample (true);
					cout << "Sample is being treated as a single source sample\n";
				}
			}

			//
			// End stage 1 for sample
			//

			bioComponent->SetMessageValue (stage1Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 1, true, false);
			CoreBioComponent::SetCurrentStage (2);

			if (possibleMixture) {

				// Have to wait until stage 1 complete to evaluate disable stutter and adenylation messages below

				Locus::SetDisableAdenylationFilter (bioComponent->GetMessageValue (disableAdenylationFilter));
				ChannelData::SetDisableAdenylationFilter (bioComponent->GetMessageValue (disableAdenylationFilter));
				Locus::SetDisableStutterFilter (bioComponent->GetMessageValue (disableStutterFilter));
				ChannelData::SetDisableStutterFilter (bioComponent->GetMessageValue (disableStutterFilter));
			}

			//cout << "Stage 1 complete" << endl;

	//		bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 1, true, false);
	//		bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 1, true, false);

			if (sampleOK && (bioComponent->AnalyzeSampleLociSM (text, ExcelText, Message, TRUE) < 0)) {

				NoticeStr = "";
				NoticeStr << "COULD NOT ANALYZE LOCI FOR FILE:  " << FileName << "\n";
				ExcelText << CLevel (1) << NoticeStr << bioComponent->GetError () << "\n";
				ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
				NoticeStr = "";
			}

			//cout << "Sample locus analysis complete" << endl;

			//
			// Question:  do we put the SmartMessage activation before or after relevant code.  The intent was that the code performs
			// tests, then the SmartMessages perform the logic and then the code cleans up the results.  We probably have to reorganize
			// somewhat!!!
			//

			if (sampleOK)
				bioComponent->TestFractionalFiltersSM ();	// first tests for stutter and adenylation; then removes peaks below fractional filter(s)

			bioComponent->SetMessageValue (stage2Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 2, true, false);
			CoreBioComponent::SetCurrentStage (3);

			//cout << "Stage 2 complete" << endl;

	//		bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 2, true, false);
	//		bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 2, true, false);

			if (sampleOK)
				bioComponent->MakePreliminaryCallsSM (pGenotypes);

			//cout << "Preliminary Calls complete" << endl;

			//cout << "Preliminary calls done" << endl;

	//		if (sampleOK)
	//			bioComponent->ValidateAndCorrectCrossChannelAnalysesSM ();

	//		if (sampleOK)	// Resolution now occurs after end of stage 3, below.
	//			bioComponent->ResolveAmbiguousInterlocusSignalsSM ();	// at this point, easily resolved ambiguities already removed

			if (sampleOK)
				bioComponent->MeasureAllInterlocusSignalAttributesSM ();	// at this point, easily resolved ambiguities already removed

			bioComponent->SetMessageValue (stage3Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 3, true, false);
			CoreBioComponent::SetCurrentStage (4);

			//cout << "Stage 3 complete" << endl;

	//		bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 3, true, false);
	//		bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 3, true, false);

			// Now, resolve ambiguities based on calculations from end of stage 3:

			if (sampleOK) {

				bioComponent->ResolveAmbiguousInterlocusSignalsUsingSmartMessageDataSM ();	// Removes ambiguous signals from loci or assigns them and records message
				bioComponent->FilterSmartNoticesBelowMinBioID ();
			}

			if (sampleOK)
				bioComponent->SignalQualityTestSM ();

			bioComponent->SetMessageValue (stage4Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 4, true, false);
			CoreBioComponent::SetCurrentStage (5);

			//cout << "Stage 4 complete" << endl;

			// The following records, at the locus level, interlocus peaks to right and left for purposes of reporting

			if (sampleOK)
				bioComponent->RemoveInterlocusSignalsSM ();

			//	Put residual displacement test here...02/11/2014

	//		bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 4, true, false);
	//		bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 4, true, false);

			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 5, false, true);
			bioComponent->AddAllSmartMessageReportersForSignals (commSM, numHigherObjects);	// still in wrong place???

			if (sampleOK)
				bioComponent->SampleQualityTestSM (pGenotypes);

			//
			//  Put peak height accumulation algorithm here (12/16/2016)
			//

			//if (sampleOK)
			//	bioComponent->WriteDataToHeightFileSM ();

			//cout << "Sample quality test complete" << endl;

			if (sampleOK)
				bioComponent->TestPositiveControlSM (pGenotypes);

			// Moved this section because it requires setting artifact for signal, which must precede stage 4 and it should only depend on removing extraneous
			// signals from each locus, which should be done in "SignalQualityTest" above.
			//if (sampleOK)
			//	bioComponent->TestPositiveControlSM (pGenotypes);

			bioComponent->OrganizeNoticeObjectsSM ();  // Have to do this here, before last evaluation and adding smart message reporters!!

			bioComponent->SetMessageValue (stage5Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 5, false, false);	// These do not include signals...already done

			//cout << "Stage 5 complete" << endl;

	//		bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 5, false, false);	// These do not include signals...already done
	//		bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 5, false, false);

			if (!sampleOK)
				bioComponent->LocatePositiveControlName (pGenotypes);
		}

		catch (...) {

			int cc = CoreBioComponent::GetCrashCode ();

			if (cc == 42) {

				CrashResponse = 0;
				cout << "********Sample " << data->GetName () << " has one or more channels with no data" << endl;
				CoreBioComponent::ResetCrashMode (true);
				STRLCAnalysis::mFailureMessage->SetPingValue (590);
				STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
			}

			else if (cc == 37) {

				CrashResponse = 1;
				cout << "********Sample " << data->GetName () << " is not a valid fsa/hid file" << endl;
				CoreBioComponent::ResetCrashMode (true);
				STRLCAnalysis::mFailureMessage->SetPingValue (600);
				STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
			}

			else {

				cout << "********We crashed on sample:  " << data->GetName () << "." << endl;
				CoreBioComponent::ResetCrashMode (true);
				STRLCAnalysis::mFailureMessage->SetPingValue (610);
				STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
			}

			CoreBioComponent::ResetCrashMode (true);
		}

		if (CoreBioComponent::GetCrashMode ()) {

			delete bioComponent;
			bioComponent = new STRSampleCoreBioComponent (data->GetName ());
			bioComponent->SetSampleName (data->GetSampleName ());
			bioComponent->SetFileName (FileName);
			commSM.SMOStack [1] = (SmartMessagingObject*)bioComponent;
			bioComponent->SetMessageValue (stage1Successful, true);
			bioComponent->SetMessageValue (stage2Successful, true);
			bioComponent->SetMessageValue (stage3Successful, true);
			bioComponent->SetMessageValue (stage4Successful, true);

			switch (CrashResponse) {

			case 0:
				bioComponent->SetMessageValue (sampleMissingDataForChannels, true);
				int c;

				while (true) {
					
					c = CoreBioComponent::GetNextNoDataChannel ();

					if (c < 0)
						break;

					bioComponent->AppendDataForSmartMessage (sampleMissingDataForChannels, c);
				}

				bioComponent->SetMessageValue (stage5Successful, true);
				SetMessageValue (samplesHadNoDataForChannels, true);
				AppendDataForSmartMessage (samplesHadNoDataForChannels, localFileName);
				break;

			case 1:
				bioComponent->SetMessageValue (sampleNotValidInputFile, true);
				bioComponent->SetMessageValue (stage5Successful, true);
				SetMessageValue (samplesNotValidInputFiles, true);
				AppendDataForSmartMessage (samplesNotValidInputFiles, localFileName);
				break;

			default:
				bioComponent->SetMessageValue (analysisTruncatedPrematurely, true);
				bioComponent->AppendDataForSmartMessage (analysisTruncatedPrematurely, CoreBioComponent::GetCurrentStage ());
				bioComponent->SetMessageValue (stage5Successful, true);
				SetMessageValue (samplesTerminatedPrematurely, true);
				AppendDataForSmartMessage (samplesTerminatedPrematurely, localFileName);
			}
			
			CoreBioComponent::ResetCrashMode (true);
			cout << "Crash phase 1" << endl;
			bioComponent->AddAllSmartMessageReportersCrashMode (commSM, numHigherObjects);	// These do not include signals...already done
			// Add message for crash and set up trigger for directory level message; include current stage for crash data
			// Then evaluate and set triggers for all messages for stage 5
			cout << "Crash phase 2" << endl;
			bioComponent->ReportSampleTableRowCrashMode (ExcelSummary);
			cout << "Crash phase 3" << endl;
			bioComponent->ReportSampleTableRowWithLinksCrashMode (ExcelLinks);
			cout << "Crash phase 4" << endl;
			bioComponent->ReportXMLSmartSampleTableRowWithLinksCrashMode (XMLExcelLinks, tempXMLExcelLinks);
			cout << "Crash phase 5" << endl;

			bioComponent->ReportAllSmartNoticeObjectsCrashMode (tempExcelSummary, "", " ", FALSE);
			cout << "Crash phase 6" << endl;
			bioComponent->ReportAllSmartNoticeObjectsCrashMode (tempExcelLinks, "", " ", TRUE);
			cout << "Crash phase 7" << endl;
			delete bioComponent;
			bioComponent = NULL;
			delete data;
			data = NULL;
			continue;
		}

		bioComponent->AddAllSmartMessageReporters (commSM, numHigherObjects);	// These do not include signals...already done

		if (bioComponent->SampleIsValid () && !CoreBioComponent::GetCrashMode ()) {

			bioComponent->ReportSampleData(ExcelText);
			bioComponent->WriteXMLGraphicDataSM (GraphicsDirectory, FileName, data, 4, PlotString);
		}
		
		// Modify functions below to restrict scope if crash mode = true
		bioComponent->PrepareLociForOutput ();
		bioComponent->ReportSampleTableRow (ExcelSummary);
		bioComponent->ReportSampleTableRowWithLinks (ExcelLinks);
		bioComponent->ReportXMLSmartSampleTableRowWithLinks (XMLExcelLinks, tempXMLExcelLinks);

		bioComponent->ReportAllSmartNoticeObjects (tempExcelSummary, "", " ", FALSE);
		bioComponent->ReportAllSmartNoticeObjects (tempExcelLinks, "", " ", TRUE);
		//cout << "All data reported" << endl;

		SamplesProcessed++;
		Progress = 100.0 * (double)SamplesProcessed / (double)NSampleFiles;
		cout << "Progress = " << Progress << "%." << endl;
		delete data;
		delete bioComponent;
		data = NULL;
		bioComponent = NULL;
		//cout << "Clean up time and on to the next" << endl;
	}

	if (!hasPosControl) {

		SetMessageValue (noPosCtrlFound, true);
	}

	if (!hasNegControl) {

		SetMessageValue (noNegCtrlFound, true);
	}
  }
//finishOutput:

	delete SampleDirectory;
	delete pullupMatrixFile;
	pullupMatrixFile = NULL;

	for (i=1; i<=5; i++) {

		EvaluateSmartMessagesForStage (commSM, 1, i);
		SetTriggersForAllMessages (commSM, 1, i);
	}

	numHigherObjects = 1;

	//
	// Add all SmartMessageReporters for file level
	//

	AddAllSmartMessageReporters (commSM, numHigherObjects);

	ExcelSummary << CLevel (1) << "\n" << PLevel ();
	ExcelLinks << CLevel (1) << "\n" << PLevel ();

	XMLExcelLinks << CLevel (1) << "\t</Table>\n" << PLevel ();
	ReportXMLSmartNoticeObjects (XMLExcelLinks, tempXMLExcelLinks, " ");
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
	SmartMessagingObject::ReportAllExportSpecifications (XMLExcelLinks);

	// First add directory alerts and then...Merge here to get messages...

	XMLExcelLinks << CLevel (1) << "</OsirisAnalysisReport>" << endLine << PLevel ();
	SmartMessagingObject::ClearExportSpecifications ();

	tempInputXMLSummaryLinks.Close ();
	
	ReportSmartNoticeObjects (ExcelSummary, "", " ", FALSE);
	ReportSmartNoticeObjects (ExcelLinks, "", " ", TRUE);

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


int STRLCAnalysis :: AnalyzeIncrementallySMLF (const RGString& prototypeInputDirectory, const RGString& markerSet, int outputLevel, const RGString& graphicsDirectory, const RGString& commandInputs) {

	cout << "Ladder Free Analysis Function...\n";
	
	Boolean print = TRUE;
	smDefaultsAreOverridden defaultsAreOverridden;
	smUseSampleNamesForControlSampleTestsPreset useSampleNamesForControlSampleTests;
	smMakeMixturesDefaultSampleTypePreset makeMixturesDefaultTypePreset;
	smMinImbalanceThresholdForCreatingNoisyPeak noiseImbalanceThreshold;
	smSampleAnalysisTruncatedPrematurely analysisTruncatedPrematurely;
	smSamplesTerminatedPrematurely samplesTerminatedPrematurely;
	smSamplesHadNoDataForChannels samplesHadNoDataForChannels;
	smSampleMissingDataForChannels sampleMissingDataForChannels;
	smSamplesAreNotValidInputFiles samplesNotValidInputFiles;
	smSampleIsNotValidInputFile sampleNotValidInputFile;
	bool makeMixturesDefaultType = GetMessageValue (makeMixturesDefaultTypePreset);

	double heightFraction = 0.01 * (double)GetThreshold (noiseImbalanceThreshold);
	Locus::SetImbalanceThresholdForNoisyPeak (heightFraction);
	cout << "Imbalance threshold for noisy peak = " << Locus::GetImbalanceThresholdForNoisyPeak () << "\n";

	ParameterServer* pServer = new ParameterServer;
	GenotypeSet* gSet = pServer->GetGenotypeCollection ();

	if (gSet == NULL) {

		cout << "Could not retrieve genotype collection set from parameter server.  Exiting..." << endl;
		STRLCAnalysis::mFailureMessage->CouldNotRetrieveGenotypeCollection ();
		return -20;
	}

	GenotypesForAMarkerSet* pGenotypes = gSet->FindMarkerSetCollection (markerSet);

	if (pGenotypes == NULL) {

		cout << "Could not retrieve genotype collection for marker set name " << markerSet << " from parameter server.  Exiting..." << endl;
		pGenotypes = new GenotypesForAMarkerSet;
		STRLCAnalysis::mFailureMessage->CouldNotRetrieveGenotype (markerSet);
		STRLCAnalysis::mFailureMessage->SetPingValue (430);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -20;
	}

	if (!STRLCAnalysis::OverrideStringIsEmpty ()) {

		SetMessageValue (defaultsAreOverridden, true);
		AppendDataForSmartMessage (defaultsAreOverridden, STRLCAnalysis::GetOverrideString ());
	}

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

	RGString outputSubDirectory = STRLCAnalysis::GetOutputSubDirectory ();

	if (outputSubDirectory.Length () > 0) {

		FullPathForReports += "/" + outputSubDirectory;
		RGDirectory::MakeDirectory (FullPathForReports);
	}

	cout << "Starting..." << endl;

	int i;

	DirectoryManager* SampleDirectory;
	pServer->FlushLadderSynonyms ();
	
	if (!GetMessageValue (useSampleNamesForControlSampleTests))
		SampleDirectory = new DirectoryManager (DirectoryName);

	else
		SampleDirectory = new SampleNameDirectoryManager (DirectoryName);

	if (!SampleDirectory->IsValid ()) {

		cout << "Could not find sample directory:  " << DirectoryName << endl;
		cout << "Closing..." << endl;
		STRLCAnalysis::mFailureMessage->CouldNotFindSampleDirectory (DirectoryName);
		STRLCAnalysis::mFailureMessage->SetPingValue (440);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -1;
	}

	else
		cout << "Sample directory opened successfully" << endl;

	int ChannelNumber = 1;
	SampleDirectory->Initialize ();
	int NSampleFiles = SampleDirectory->GetNumberOfFilesInDirectory ();
	double Progress;
	int SamplesProcessed = 0;

	TestCharacteristic* testPeak = new STRTestControlCharacteristic ();
	testPeak->SetTestForNegative (TestCharacteristic::GetGlobalTestForNegative ());
	RGString LaneNumber;
	RGString StartDate;
	RGString StartTime;
	RGString Extension;
	int NumFiles = 0;
	RGString FileNumber;
//	int NSamples;
//	int status;
	RGString CurrentMarkerSet;

	CoreBioComponent* bioComponent;
//	CoreBioComponent* ladderBioComponent;

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
	RGString MsgBookOutputFullPath = FullPathForReports + "/" + OutputFileName + ".msgBook.xml";

	RGString tempSummaryFullPath = FullPathForReports + "/temp" + OutputFileName + "Summary.txt";
	RGString tempSummaryFullPathWithLinks = FullPathForReports + "/temp" + OutputFileName + ".txt";
	RGString tempXMLSummaryFullPathWithLinks = FullPathForReports + "/temp" + OutputFileName + "XML.txt";

	RGString debugOutputFullPath = FullPathForReports + "/" + OutputFileName + "Debug.txt";

	RGFile OutputSummary (SummaryFullPath, "wt");
	RGFile OutputSummaryLinks (SummaryFullPathWithLinks, "wt");
	RGFile XMLOutputSummaryLinks (XMLSummaryFullPathWithLinks, "wt");
	RGFile MsgBookOutputFile (MsgBookOutputFullPath, "wt");

	if (MsgBookOutputFile.isValid ()) {

		SmartMessage::WriteMsgBookFile (MsgBookOutputFile);
		MsgBookOutputFile.Flush ();
	}

	MsgBookOutputFile.Close ();

	RGString OutputLadderInfo = FullPathForReports + "/LadderInfo.xml";
	RGTextOutput* LadderInfo = new RGTextOutput (OutputLadderInfo, FALSE);

	if (LadderInfo->FileIsValid ()) {

		*LadderInfo << mCollection->GetGridData ();
	}

	delete LadderInfo;

	RGFile tempOutputSummary (tempSummaryFullPath, "wt");
	RGFile tempOutputSummaryLinks (tempSummaryFullPathWithLinks, "wt");
	RGFile tempXMLOutputSummaryLinks (tempXMLSummaryFullPathWithLinks, "wt");

	RGString ConsoleOutputFileName = OutputFileName + "ConsoleData.txt";
	RGString WholeConsoleName = FullPathForReports + "/" + ConsoleOutputFileName;

	WorkingFileName = FullPathForReports + "/Working.txt";
	WorkingFile = new RGFile (WorkingFileName, "wt");

	if (!WorkingFile->isValid ())
		cout << "Could not open output file:  " << WorkingFileName << endl;

	RGString CInputFileName = FullPathForReports + "/BaseInputFile.txt";
	RGTextOutput CInputFile (CInputFileName, FALSE);
	
	RGTextOutput text (WholeConsoleName, FALSE);
	OsirisMsg Message (&OutputFile, "\t", 10);
	Endl endLine;

	RGString nonLaserOffScalePullupFractionName = FullPathForReports + "/PullupFractions.tab";
	RGTextOutput* nonLaserOffScalePullupFractions = new RGTextOutput (nonLaserOffScalePullupFractionName, FALSE);
	CoreBioComponent::SetNonLaserOffScalePUCoeffsFile (nonLaserOffScalePullupFractions);

	if (!CInputFile.FileIsValid ()) {

		cout << "Could not open Command Input file:  " << CInputFileName << endl;
		cout << "Information is reproduced in Details below\n";
	}

	else {

		CInputFile << commandInputs;
	}

	if (!OutputFile.isValid ()) {

		cout << "Could not open output file:  " << OutputFullPath << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File Path:  " + OutputFullPath);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Output File");
		STRLCAnalysis::mFailureMessage->SetPingValue (450);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -1;
	}

	if (!text.FileIsValid ()) {

		cout << "Could not open text echo file:  " << WholeConsoleName << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File Path:  " + WholeConsoleName);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Text Echo File");
		STRLCAnalysis::mFailureMessage->SetPingValue (460);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!OutputSummary.isValid ()) {

		cout << "Could not open output summary file:  " << SummaryFullPath << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File Path:  " + SummaryFullPath);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Output Summary File");
		STRLCAnalysis::mFailureMessage->SetPingValue (470);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!OutputSummaryLinks.isValid ()) {

		cout << "Could not open output summary file with embedded links:  " << SummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File Path:  " + SummaryFullPathWithLinks);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Output Summary File With Embedded Links");
		STRLCAnalysis::mFailureMessage->SetPingValue (480);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!XMLOutputSummaryLinks.isValid ()) {

		cout << "Could not open output xml summary file with embedded links:  " << XMLSummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File Path:  " + XMLSummaryFullPathWithLinks);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Output XML Summary File With Embedded Links");
		STRLCAnalysis::mFailureMessage->SetPingValue (490);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!tempOutputSummary.isValid ()) {

		cout << "Could not open temporary output summary file:  " << tempSummaryFullPath << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File Path:  " + tempSummaryFullPath);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Temporary Output Summary File");
		STRLCAnalysis::mFailureMessage->SetPingValue (500);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!tempOutputSummaryLinks.isValid ()) {

		cout << "Could not open temporary output summary file with embedded links:  " << tempSummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File Path:  " + tempSummaryFullPathWithLinks);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Temporary Output Summary File With Embedded Links");
		STRLCAnalysis::mFailureMessage->SetPingValue (510);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
		return -2;
	}

	if (!tempXMLOutputSummaryLinks.isValid ()) {

		cout << "Could not open temporary output xml summary file with embedded links:  " << tempXMLSummaryFullPathWithLinks << endl;
		cout << "Ending..." << endl;
		STRLCAnalysis::mFailureMessage->AddMessage ("File Path:  " + tempXMLSummaryFullPathWithLinks);
		STRLCAnalysis::mFailureMessage->CouldNotOpenFile ("Temporary Output XML Summary File With Embedded Links");
		STRLCAnalysis::mFailureMessage->SetPingValue (520);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
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
	SmartMessage::CreateDebugFile (debugOutputFullPath);
	GridDataStruct* GridData = new GridDataStruct (mCollection, markerSetName, testPeak, text, ExcelText, Message, print);
	SampleDataStruct* SampleData = new SampleDataStruct (mCollection, markerSetName, testPeak, testPeak, text, ExcelText, Message, print);
	PopulationMarkerSet* set = mCollection->GetNamedPopulationMarkerSet (markerSetName);

	if (set == NULL) {

		cout << "Could not find marker set named:  " << markerSetName << ".  Ending..." << endl;
		STRLCAnalysis::mFailureMessage->CouldNotFindNamedMarkerSet (markerSetName);
		STRLCAnalysis::mFailureMessage->SetPingValue (530);
		STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
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
	DataSignal::SetNumberOfChannels (expectedNumberOfChannels);
	Locus::SetNumberOfChannels (expectedNumberOfChannels);

	pServer->Report (text, "");
	pServer->Report (ExcelText, "");
	ExcelText.ResetOutputLevel ();

	ExcelSummary.SetOutputLevel (1);

	int minSampleRFU = (int)floor (STRSampleChannelData::GetMinRFU () + 0.5);
	int minLadderRFU = (int)floor (STRLadderChannelData::GetMinRFU () + 0.5);
	int minLaneStdRFU = (int)floor (STRLaneStandardChannelData::GetMinRFU () + 0.5);
	int minInterlocusRFU = (int)floor (STRSampleChannelData::GetMinInterlocusRFU () + 0.5);
	int minLadderInterlocusRFU = (int)floor (STRLadderChannelData::GetMinInterlocusRFU () + 0.5);
	int minRFUSampleDetectionRFU = (int)floor (STRSampleChannelData::GetSampleDetectionThreshold () + 0.5);

	ExcelSummary << "OAR 2.0" << endLine;
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

	ExcelLinks << "OAR 2.0" << endLine;
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
	XMLExcelLinks << "<OsirisAnalysisReport>\n\t<Version>" << OSIRIS_VERSION << "</Version>\n\t";
	XMLExcelLinks << "<FullVersion>" << OSIRIS_FULLNAME << "</FullVersion>\n\t";
	XMLExcelLinks << "<MsgBookBuildTime>" << STRLCAnalysis::GetMsgBookBuildTime () << "</MsgBookBuildTime>\n\t<Heading>\n\t\t<FileName>";
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
	XMLExcelLinks << "\t\t\t<minRFUladderInterlocus>" << minLadderInterlocusRFU << "</minRFUladderInterlocus>\n";
	XMLExcelLinks << "\t\t\t<minRFUsampleDetection>" << minRFUSampleDetectionRFU << "</minRFUsampleDetection>\n";
	pResult = pServer->BuildChannelThresholdOverridesForOAR ();

	if (pResult.Length () > 0)
		XMLExcelLinks << pResult;

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

	PlotString << "\t<Version>" << OSIRIS_VERSION << "</Version>\n";
	PlotString << "\t<FullVersion>" << OSIRIS_FULLNAME << "</FullVersion>\n";
	PlotString << "\t<MsgBookBuildTime>" << STRLCAnalysis::GetMsgBookBuildTime () << "</MsgBookBuildTime>\n";
	PlotString << "\t<parameters>\n\t\t<inputDirectory>" << xmlwriter::EscAscii (DirectoryName, &pResult) << "</inputDirectory>\n\t\t<outputDirectory>";
	PlotString << xmlwriter::EscAscii (FullPathForReports, &pResult) << "</outputDirectory>\n\t\t<kit>";
	PlotString << xmlwriter::EscAscii (markerSet, &pResult) << "</kit>\n\t\t<ls>";
	PlotString << lsName << "</ls>\n\t\t<minRFUsample>" << minSampleRFU << "</minRFUsample>\n";

	pResult = pServer->BuildChannelThresholdOverridesForPLT ();

	if (pResult.Length () > 0)
		PlotString << pResult;

	PlotString << "\t\t<minRFUILS>" << minLaneStdRFU << "</minRFUILS>\n\t\t<minRFUladder>" << minLadderRFU;
	PlotString << "</minRFUladder>\n\t\t<minRFUinterlocus>" << minInterlocusRFU << "</minRFUinterlocus>\n";
	PlotString << "\t\t<minRFUladderInterlocus>" << minLadderInterlocusRFU << "</minRFUladderInterlocus>\n";
	PlotString << "\t\t<minRFUsampleDetection>" << minRFUSampleDetectionRFU << "</minRFUsampleDetection>\n";
	PlotString << "\t\t<DataAnalyzed>";

	if (CoreBioComponent::GetUseRawData ())
		PlotString << "false</DataAnalyzed>\n\t\t<StdPosControlName>";

	else
		PlotString << "true</DataAnalyzed>\n\t\t<StdPosControlName>";

	PlotString << xmlwriter::EscAscii (pServer->GetStandardPositiveControlName (), &pResult) << "</StdPosControlName>\n\t</parameters>\n";

	RGString stringData;

	testPeak->Report (ExcelText, "", "\t");

//	int NChannels;

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
//	RGString OsirisGraphics (graphicsDirectory);
//	bool ladderOK;
	bool hasPosControl = false;
	bool hasNegControl = false;
	bool foundALadder = true;
	smLadderFailed ladderFailed;
	smSampleIsLadder sampleIsLadder;
	smSampleUnanalyzable sampleFailed;
	smSampleIsPosCtrl sampleIsPosCtrl;
	smSampleIsNegCtrl sampleIsNegCtrl;
	smNoPosCtrlFound noPosCtrlFound;
	smNoNegCtrlFound noNegCtrlFound;
	smSampleSatisfiesPossibleMixtureIDCriteria sampleSatisfiesMixtureCriteria;
	smDisableLowLevelFiltersForKnownMixturesPreset disableLowLevelFilters;
	smSaveLadderILSHistoryPreset saveLadderILSHistory;
	smLatitudeForILSFit latitudeForILSFit;
	smUseLadderEndPointILSAlgorithmPreset useLadderEndPointILSAlgorithm;
	smPlusLatitudeForLadderEndPointILSFit plusLatitudeForLadderEndPointILSFit;
	smTailHeightFittingThresholdFactor tailFittingHeightModifier;
	smTailSlopeFittingThresholdFactor tailFittingSlopeModifier;

	double tailHeightModifier = 0.01 * (double) GetThreshold (tailFittingHeightModifier);
	double tailSlopeModifier = 0.01 * (double) GetThreshold (tailFittingSlopeModifier);
	TracePrequalification::SetLowHeightModifier (tailHeightModifier);
	TracePrequalification::SetLowSlopeModifier (tailSlopeModifier);

	smStage1Successful stage1Successful;
	smStage2Successful stage2Successful;
	smStage3Successful stage3Successful;
	smStage4Successful stage4Successful;
	smStage5Successful stage5Successful;

	smDisableStutterFilter disableStutterFilter;
	smDisableAdenylationFilter disableAdenylationFilter;
	smCallOnLadderAdenylationPreset callOnLadderAdenylation;

	RGString pullupMatrixFileName = FullPathForReports + "/matrix.txt";
	RGTextOutput* pullupMatrixFile = NULL;

	if (GetMessageValue (saveLadderILSHistory)) {

		STRLCAnalysis::SetCollectILSHistory (true);
		ChannelData::SetUseILSHistory (false);

		int threshold = GetThreshold (latitudeForILSFit);

		if (threshold > 0)
			ChannelData::SetLatitudeFactorForILSHistory (0.0001 * (double) threshold);

		else
			ChannelData::SetLatitudeFactorForILSHistory (0.01);

		cout << "Collecting Ladder ILS History with latitude factor = " << 0.0001 * (double) threshold << "\n";
	}

	else
		cout << "Not collecting ladder ILS History...\n";

	if (GetMessageValue (useLadderEndPointILSAlgorithm)) {

		ChannelData::SetUseILSLadderEndPointAlgorithm (true);
		ChannelData::SetLatitudeFactorForLadderILS (0.02);
		cout << "Using End Point Algorithm for Ladder ILS Analysis...\n";
		int threshold2 = GetThreshold (plusLatitudeForLadderEndPointILSFit);  // Substitute ladder latitude

		if (threshold2 > 0)
			ChannelData::SetLatitudeFactorForLadderILS (0.0001 * (double) threshold2); // Substitute ladder latitude

		else
			ChannelData::SetLatitudeFactorForLadderILS (0.05);  // Substitute ladder latitude

		cout << "Testing Ladder ILS start and end points with latitude factor = " << 0.0001 * (double) threshold2 << "\n";
	}

	else
		cout << "Not using End Point Algorithm for Ladder ILS Analysis...\n";

//	bool ignoreNoise;

	RGString GraphicsDirectory = FullPathForReports;
	RGString idString;

	cout << "Expected dye names:  ";

	for (i=1; i<=expectedNumberOfChannels; i++) {

		if (i > 1)
			cout << ", ";

		cout << CoreBioComponent::GetDyeName (i).GetData ();
	}

	cout << "\n\n";

	SmartMessagingComm commSM;
	commSM.SMOStack [0] = (SmartMessagingObject*) this;
	int numHigherObjects = 2;
	bool isFirstLadder = true;
	RGString ABIModelNumber;
	int nLadders = 0;
	ChannelData::SetTestForDualSignal (true);    // 01/24/2017 This is a test because ladders are missing split peaks that should not be missed.  It used to be set to true.

	//double leastBP = set->GetMinimumILSBPSearchRegion ();
	//CoreBioComponent::SetMinBioIDForLadderLoci (leastBP);
	//cout << "Minimum ILS BP for ladder loci = " << leastBP << "\n";

	//int leastBPForSamples = CoreBioComponent::GetMinBioIDForArtifacts ();
	//int leastBPInt = (int) floor (leastBP);

	//if ((leastBPForSamples > 0) && (leastBPForSamples > leastBPInt)) {

	//	cout << "Minimum ILS BP for sample artifacts setting = " << leastBPForSamples << " is too high.  Resetting to " << leastBPInt << "\n\n";
	//	CoreBioComponent::SetMinBioIDForArtifacts (leastBPInt);
	//}

	//else
	//	cout << "\n";

	RGString commentField;
	RGString localFileName;

	ChannelData::SetTestForDualSignal (true);
	ChannelData::SetUseILSLadderEndPointAlgorithm (false);
	RGString SampleName;
	bool sampleOK;
	bool populatedBaseLocusList = false;
	bool possibleMixture;

	SampleDirectory->RewindDirectory ();
	pServer->AddLabPositiveControlsToControlStrings (pGenotypes);
	
	//if (CollectILSHistory) {

	//	STRLCAnalysis::SetCollectILSHistory (false);
	//	ChannelData::SetUseILSHistory (true);
	//	ladderBioComponent = (CoreBioComponent*) LadderList.First ();
	//	ladderBioComponent->ResetBoundsForILSUsingFactor (ChannelData::GetLatitudeFactorForILSHistory ());
	//	cout << "All ladder ILS history collected and bounds reset..." << endl;
	//}

#ifdef _MATRIX_OUTPUT_
	pullupMatrixFile = new RGTextOutput (pullupMatrixFileName, FALSE);

	if (!pullupMatrixFile->FileIsValid ()) {

		cout << "Could not open Matrix Output File:  " << pullupMatrixFileName << endl;
		delete pullupMatrixFile;
		pullupMatrixFile = NULL;
	}

	CoreBioComponent::SetPullupMatrixFile (pullupMatrixFile);
#endif

	//  Test for channel remapping and remap if positive

	smChannelRemappingForLadderFreeOverrideDefault remapChannels;
	smFsaChannelForOsirisChannel1 newChannel1Map;
	smFsaChannelForOsirisChannel2 newChannel2Map;
	smFsaChannelForOsirisChannel3 newChannel3Map;
	smFsaChannelForOsirisChannel4 newChannel4Map;
	smFsaChannelForOsirisChannel5 newChannel5Map;

	if (GetMessageValue (remapChannels)) {

		int* newMap = new int [expectedNumberOfChannels + 1];
		newMap [0] = 0;
		int j;

		for (j=1; j<=expectedNumberOfChannels; j++) {

			switch (j) {

			case 1:
				newMap [j] = GetThreshold (newChannel1Map);
				break;

			case 2:
				newMap [j] = GetThreshold (newChannel2Map);
				break;

			case 3:
				newMap [j] = GetThreshold (newChannel3Map);
				break;

			case 4:
				newMap [j] = GetThreshold (newChannel4Map);
				break;

			case 5:
				newMap [j] = GetThreshold (newChannel5Map);
				break;

			default:
				newMap [0] = 0;
			}

			if ((j != 0) && ((newMap [j] < 1) || (newMap [j] > 8))) {

				cout << "Remap index for Osiris " << j << "th channel = " << newMap [j] << " is out of range.  Resetting to " << j << ".\n";
				newMap [j] = j;
			}
		}

		const int* currentMap = set->GetChannelMap ();
		cout << "\n";

		for (j=1; j<=expectedNumberOfChannels; j++) {

			cout << "Old fsa channel for Osiris channel " << j << " = " << currentMap [j] << "\n";
		}

		set->ResetChannelMap (newMap);
		delete[] newMap;
		currentMap = set->GetChannelMap ();
		cout << "\n";

		for (j=1; j<=expectedNumberOfChannels; j++) {

			cout << "New fsa channel for Osiris channel " << j << " = " << currentMap [j] << "\n";
		}
	}

	else {

		cout << "\n";
		const int* currentMap = set->GetChannelMap ();
		int j;

		for (j=1; j<=expectedNumberOfChannels; j++) {

			cout << "Default fsa channel for Osiris channel " << j << " = " << currentMap [j] << "\n";
		}

		cout << "\n";
	}

	// Modify below functions to accumlate partial work, as possible, in spite of "errors", and report

	int CrashResponse = 10;

	RGString modsFileString;
	RGString modsFileName = PrototypeInputDirectory + "/batch-mods.txt";
	RGFile modsFile (modsFileName, "rt");
	OverallModList* oml = NULL;
	SampleModList* sml = NULL;

	if (modsFile.isValid ()) {

		modsFileString.ReadTextFile (modsFile);
		oml = new OverallModList (expectedNumberOfChannels);
		oml->ReadOverallModList (modsFileString);
		//cout << "Read mods file containing:  \n";
		//cout << modsFileString << endl;
		//oml->PrintList ();
	}

	while (SampleDirectory->GetNextOrderedSampleFile (FileName)) {

		CoreBioComponent::ResetCrashMode (false);
		CoreBioComponent::SetCurrentStage (1);
		CoreBioComponent::AddOneToCrashCount ();
		CoreBioComponent::ResetNoDataChannels ();
		CoreBioComponent::ResetCrashCode ();

		if (oml != NULL)
			sml = oml->GetSampleModList (FileName);

		else
			sml = NULL;

		CrashResponse = 10;
		sampleOK = true;
		localFileName = FileName;
		sampleOK = true;
		FullPathName = DirectoryName + "/" + FileName;
		DirectoryCrashMode = false;

		if (WorkingFile != NULL) {

			WorkingFile->Write (FullPathName + "\n");
			WorkingFile->Flush ();
		}

		data = new fsaFileData (FullPathName);
		bioComponent = new STRSampleCoreBioComponent (data->GetName ());
		bioComponent->SetSampleName (data->GetSampleName ());
		bioComponent->SetFileName (FileName);
	//	Locus::SetCallOnLadderAdenylation (bioComponent->GetMessageValue (callOnLadderAdenylation));

		commentField = data->GetComment ();
		bioComponent->SetComments (commentField);

		bioComponent->SetSampleModifications (sml);

		try { //##########

			if (!data->IsValid ()) {

				CoreBioComponent::SetCrashCode (37);
				throw 37;
			}

			if (GetMessageValue (useSampleNamesForControlSampleTests))
				bioComponent->SetControlIdName (bioComponent->GetDataSampleName ());

			else
				bioComponent->SetControlIdName (FileName);

			commSM.SMOStack [1] = (SmartMessagingObject*) bioComponent;

			if (bioComponent->PrepareSampleForAnalysisSM (*data, SampleData) < 0) {

				sampleOK = false;
				NoticeStr = "";
				NoticeStr << "COULD NOT INITIALIZE AND PREPARE FOR ANALYSIS, FOR FILE:  " << FileName << "\n";
				ExcelText << CLevel (1) << NoticeStr << bioComponent->GetError ();
				ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
				cout << NoticeStr.GetData () << endl;
				NoticeStr = "";
				bioComponent->SetMessageValue (sampleFailed, true);
			}

			//if (sampleOK && !populatedBaseLocusList) {

			//	populatedBaseLocusList = true;
			//	bioComponent->AppendAllBaseLociToList (mBaseLocusList);
			//}

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

			bioComponent->RemoveAllSignalsOutsideLaneStandardSMLF ();

			if (sampleOK && (bioComponent->PreliminarySampleAnalysisSMLF () < 0)) {

				sampleOK = false;
				NoticeStr = "";
				NoticeStr << "COULD NOT PERFORM PRELIMINARY ANALYSIS, FILE:  " << FileName << "\n";
				ExcelText << CLevel (1) << NoticeStr << bioComponent->GetError () << "\n";
				ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
				NoticeStr = "";
				bioComponent->SetMessageValue (sampleFailed, true);
			}

			else
				bioComponent->GetAllAmbientData (data);

			//cout << "Preliminary Analysis Complete" << endl;

			//  do we want to do this next part?  Negative controls certainly make sense.  How about Positive Controls?  Mixtures?

			bioComponent->SetNegativeControlFalseSM ();
			bioComponent->SetPositiveControlFalseSM ();
			idString = bioComponent->GetControlIdName ();
			Locus::SetDisableAdenylationFilter (false);
			Locus::SetDisableStutterFilter (false);
			ChannelData::SetDisableAdenylationFilter (false);
			ChannelData::SetDisableStutterFilter (false);
			possibleMixture = false;
		//	cout << "ID String = " << (char*) idString.GetData () << " for file name " << (char*) FileName.GetData () << endl;
			Locus::SetControlSample (false);

			if (sampleOK && pServer->ControlDoesTargetStringContainASynonymCaseIndep (idString)) {

				if (pServer->NegControlDoesTargetStringContainASynonymCaseIndep (idString)) {

					hasNegControl = true;;
					bioComponent->SetNegativeControlTrueSM ();
					bioComponent->SetMessageValue (sampleIsNegCtrl, true);
					Locus::SetSingleSourceSample (true);
					Locus::SetControlSample (true);
				}

				else if (pServer->PosControlDoesTargetStringContainASynonymCaseIndep (idString)) {

					hasPosControl = true;
					bioComponent->SetPositiveControlTrueSM ();
					bioComponent->SetMessageValue (sampleIsPosCtrl, true);
					Locus::SetSingleSourceSample (true);
					Locus::SetControlSample (true);
				}

				bioComponent->SetPossibleMixtureIDFalseSM ();
				bioComponent->SetMessageValue (sampleSatisfiesMixtureCriteria, false);
			}

			//else if (bioComponent->IsLabPositiveControl (idString, pGenotypes)) {

			//	hasPosControl = true;
			//	bioComponent->SetPositiveControlTrueSM ();
			//	bioComponent->SetMessageValue (sampleIsPosCtrl, true);
			//	bioComponent->SetPossibleMixtureIDFalseSM ();
			//	bioComponent->SetMessageValue (sampleSatisfiesMixtureCriteria, false);
			//}

			//else if (sampleOK && bioComponent->GetMessageValue (disableLowLevelFilters)) {

			else if (sampleOK && bioComponent->GetMessageValue (disableLowLevelFilters)) {

				if (pServer->DoesTargetStringContainMixtureCriteriaCaseIndep (idString, makeMixturesDefaultType)) {

					bioComponent->SetPossibleMixtureIDTrueSM ();
					bioComponent->SetMessageValue (sampleSatisfiesMixtureCriteria, true);
					possibleMixture = true;
					Locus::SetSingleSourceSample (false);
					cout << "Sample is being treated as a mixture\n";
				}

				else {

					bioComponent->SetPossibleMixtureIDFalseSM ();
					bioComponent->SetMessageValue (sampleSatisfiesMixtureCriteria, false);
					possibleMixture = false;
					Locus::SetSingleSourceSample (true);
					cout << "Sample is being treated as a single source sample\n";
				}
			}

			//
			// End stage 1 for sample
			//

			bioComponent->SetMessageValue (stage1Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 1, true, false);

			if (possibleMixture) {

				// Have to wait until stage 1 complete to evaluate disable stutter and adenylation messages below

				Locus::SetDisableAdenylationFilter (bioComponent->GetMessageValue (disableAdenylationFilter));
				ChannelData::SetDisableAdenylationFilter (bioComponent->GetMessageValue (disableAdenylationFilter));
				Locus::SetDisableStutterFilter (bioComponent->GetMessageValue (disableStutterFilter));
				ChannelData::SetDisableStutterFilter (bioComponent->GetMessageValue (disableStutterFilter));
			}

			//cout << "Stage 1 complete" << endl;
			bioComponent->TestAllFractionalFiltersSMLF ();

			bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 1, true, false);
			bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 1, true, false);

			/*if (sampleOK && (bioComponent->AnalyzeSampleLociSM (text, ExcelText, Message, TRUE) < 0)) {

				NoticeStr = "";
				NoticeStr << "COULD NOT ANALYZE LOCI FOR FILE:  " << FileName << "\n";
				ExcelText << CLevel (1) << NoticeStr << bioComponent->GetError () << "\n";
				ExcelText << "COULD NOT ANALYZE FSA FILE:  " << FullPathName << ".  Skipping..." << "\n" << PLevel ();
				NoticeStr = "";
			}*/

			//cout << "Sample locus analysis complete" << endl;

			//
			// Question:  do we put the SmartMessage activation before or after relevant code.  The intent was that the code performs
			// tests, then the SmartMessages perform the logic and then the code cleans up the results.  We probably have to reorganize
			// somewhat!!!
			//

			//if (sampleOK)
			//	bioComponent->TestFractionalFiltersSM ();	// first tests for stutter and adenylation; then removes peaks below fractional filter(s),  Do this on a channel basis???  If so, need new function

			bioComponent->SetMessageValue (stage2Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 2, true, false);

			//cout << "Stage 2 complete" << endl;

			bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 2, true, false);
			bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 2, true, false);

			if (sampleOK) {

				bioComponent->TestForNearlyDuplicateAllelesSMLF ();
			}

			//cout << "Preliminary Calls complete" << endl;

			//cout << "Preliminary calls done" << endl;
		
	//		if (sampleOK)
	//			bioComponent->ValidateAndCorrectCrossChannelAnalysesSM ();

	//		if (sampleOK)	// Resolution now occurs after end of stage 3, below.
	//			bioComponent->ResolveAmbiguousInterlocusSignalsSM ();	// at this point, easily resolved ambiguities already removed

			//if (sampleOK)
			//	bioComponent->MeasureAllInterlocusSignalAttributesSM ();	// at this point, easily resolved ambiguities already removed

			bioComponent->SetMessageValue (stage3Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 3, true, false);

			//cout << "Stage 3 complete" << endl;

			bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 3, true, false);
			bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 3, true, false);

			// Now, resolve ambiguities based on calculations from end of stage 3:

			if (sampleOK) {

				//bioComponent->ResolveAmbiguousInterlocusSignalsUsingSmartMessageDataSM ();	// Removes ambiguous signals from loci or assigns them and records message
				bioComponent->FilterSmartNoticesBelowMinBioID ();
			}

			//if (sampleOK)
			//	bioComponent->SignalQualityTestSM ();

			bioComponent->SetMessageValue (stage4Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 4, true, false);

			//cout << "Stage 4 complete" << endl;

			// The following records, at the locus level, interlocus peaks to right and left for purposes of reporting

			//if (sampleOK)
			//	bioComponent->RemoveInterlocusSignalsSM ();

			//	Put residual displacement test here...02/11/2014

			bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 4, true, false);
			bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 4, true, false);

			if (sampleOK)
				bioComponent->SampleQualityTestSMLF ();   // Have to revamp process quality tests?

			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 5, false, true);
			bioComponent->AddAllSmartMessageReportersForSignals (commSM, numHigherObjects);	// still in wrong place???

			//
			//  Put peak height accumulation algorithm here (12/16/2016)
			//

			if (sampleOK)
				bioComponent->WriteDataToHeightFileSM ();

			//cout << "Sample quality test complete" << endl;

			//if (sampleOK)
			//	bioComponent->TestPositiveControlSM (pGenotypes);   // How will we do this???

			// Moved this section because it requires setting artifact for signal, which must precede stage 4 and it should only depend on removing extraneous
			// signals from each locus, which should be done in "SignalQualityTest" above.
			//if (sampleOK)
			//	bioComponent->TestPositiveControlSM (pGenotypes);

			bioComponent->OrganizeNoticeObjectsSM ();  // Have to do this here, before last evaluation and adding smart message reporters!!

			bioComponent->SetMessageValue (stage5Successful, true);
			bioComponent->EvaluateSmartMessagesAndTriggersForStage (commSM, numHigherObjects, 5, false, false);	// These do not include signals...already done

			//cout << "Stage 5 complete" << endl;

			bioComponent->EvaluateSmartMessagesForStage (commSM, numHigherObjects, 5, false, false);	// These do not include signals...already done
			bioComponent->SetTriggersForAllMessages (commSM, numHigherObjects, 5, false, false);

			//if (!sampleOK)
			//	bioComponent->LocatePositiveControlName (pGenotypes);

			bioComponent->AddAllSmartMessageReporters (commSM, numHigherObjects);	// These do not include signals...already done

		}

		catch (...) {

			DirectoryCrashMode = true;
			int cc = CoreBioComponent::GetCrashCode ();

			if (cc == 42) {

				CrashResponse = 0;
				cout << "********Sample " << data->GetName () << " has one or more channels with no data" << endl;
				CoreBioComponent::ResetCrashMode (true);
				STRLCAnalysis::mFailureMessage->SetPingValue (620);
				STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
				DirectoryCrashMode = true;
			}

			else if (cc == 37) {

				CrashResponse = 1;
				cout << "********Sample " << data->GetName () << " is not a valid fsa/hid file" << endl;
				CoreBioComponent::ResetCrashMode (true);
				STRLCAnalysis::mFailureMessage->SetPingValue (630);
				STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
				DirectoryCrashMode = true;
			}

			else {

				cout << "********We crashed on sample:  " << data->GetName () << "." << endl;
				CoreBioComponent::ResetCrashMode (true);
				STRLCAnalysis::mFailureMessage->SetPingValue (640);
				STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
				DirectoryCrashMode = true;
			}

			CoreBioComponent::ResetCrashMode (true);
			DirectoryCrashMode = true;
		}

		if (DirectoryCrashMode) {

			delete bioComponent;
			bioComponent = new STRSampleCoreBioComponent (data->GetName ());
			bioComponent->SetSampleName (data->GetSampleName ());
			bioComponent->SetFileName (FileName);
			commSM.SMOStack [1] = (SmartMessagingObject*)bioComponent;
			bioComponent->SetMessageValue (stage1Successful, true);
			bioComponent->SetMessageValue (stage2Successful, true);
			bioComponent->SetMessageValue (stage3Successful, true);
			bioComponent->SetMessageValue (stage4Successful, true);

			switch (CrashResponse) {

			case 0:
				bioComponent->SetMessageValue (sampleMissingDataForChannels, true);
				int c;

				while (true) {

					c = CoreBioComponent::GetNextNoDataChannel ();

					if (c < 0)
						break;

					bioComponent->AppendDataForSmartMessage (sampleMissingDataForChannels, c);
				}

				bioComponent->SetMessageValue (stage5Successful, true);
				SetMessageValue (samplesHadNoDataForChannels, true);
				AppendDataForSmartMessage (samplesHadNoDataForChannels, localFileName);
				break;

			case 1:
				bioComponent->SetMessageValue (sampleNotValidInputFile, true);
				bioComponent->SetMessageValue (stage5Successful, true);
				SetMessageValue (samplesNotValidInputFiles, true);
				AppendDataForSmartMessage (samplesNotValidInputFiles, localFileName);
				break;

			default:
				bioComponent->SetMessageValue (analysisTruncatedPrematurely, true);
				bioComponent->AppendDataForSmartMessage (analysisTruncatedPrematurely, CoreBioComponent::GetCurrentStage ());
				bioComponent->SetMessageValue (stage5Successful, true);
				SetMessageValue (samplesTerminatedPrematurely, true);
				AppendDataForSmartMessage (samplesTerminatedPrematurely, localFileName);
				break;
			}

			CoreBioComponent::ResetCrashMode (true);
			DirectoryCrashMode = true;
			cout << "Crash phase 1" << endl;
			bioComponent->AddAllSmartMessageReportersCrashMode (commSM, numHigherObjects);	// These do not include signals...already done
			// Add message for crash and set up trigger for directory level message; include current stage for crash data
			// Then evaluate and set triggers for all messages for stage 5
			cout << "Crash phase 2" << endl;
			bioComponent->ReportSampleTableRowCrashMode (ExcelSummary);
			cout << "Crash phase 3" << endl;
			bioComponent->ReportSampleTableRowWithLinksCrashMode (ExcelLinks);
			cout << "Crash phase 4" << endl;
			bioComponent->ReportXMLSmartSampleTableRowWithLinksCrashMode (XMLExcelLinks, tempXMLExcelLinks);
			cout << "Crash phase 5" << endl;

			bioComponent->ReportAllSmartNoticeObjectsCrashMode (tempExcelSummary, "", " ", FALSE);
			cout << "Crash phase 6" << endl;
			bioComponent->ReportAllSmartNoticeObjectsCrashMode (tempExcelLinks, "", " ", TRUE);
			cout << "Crash phase 7" << endl;
			delete bioComponent;
			bioComponent = NULL;
			delete data;
			data = NULL;
			continue;
		}

		if (bioComponent->SampleIsValid () && !DirectoryCrashMode) {

			bioComponent->ReportSampleData(ExcelText);
			bioComponent->WriteXMLGraphicDataSM (GraphicsDirectory, FileName, data, 4, PlotString);
		}

		bioComponent->PrepareLociForOutput ();
		bioComponent->ReportSampleTableRow (ExcelSummary);
		bioComponent->ReportSampleTableRowWithLinks (ExcelLinks);
		bioComponent->ReportXMLSmartSampleTableRowWithLinks (XMLExcelLinks, tempXMLExcelLinks);

		bioComponent->ReportAllSmartNoticeObjects (tempExcelSummary, "", " ", FALSE);
		bioComponent->ReportAllSmartNoticeObjects (tempExcelLinks, "", " ", TRUE);
		//cout << "All data reported" << endl;

		SamplesProcessed++;
		Progress = 100.0 * (double)SamplesProcessed / (double)NSampleFiles;
		cout << "Progress = " << Progress << "%." << endl;
		delete data;
		delete bioComponent;
		data = NULL;
		bioComponent = NULL;
		//cout << "Clean up time and on to the next" << endl;
	}

	if (!hasPosControl) {

		SetMessageValue (noPosCtrlFound, true);
	}

	if (!hasNegControl) {

		SetMessageValue (noNegCtrlFound, true);
	}

	delete SampleDirectory;
	delete pullupMatrixFile;
	pullupMatrixFile = NULL;

	for (i=1; i<=5; i++) {

		EvaluateSmartMessagesForStage (commSM, 1, i);
		SetTriggersForAllMessages (commSM, 1, i);
	}

	numHigherObjects = 1;

	//
	// Add all SmartMessageReporters for file level
	//

	AddAllSmartMessageReporters (commSM, numHigherObjects);

	ExcelSummary << CLevel (1) << "\n" << PLevel ();
	ExcelLinks << CLevel (1) << "\n" << PLevel ();

	XMLExcelLinks << CLevel (1) << "\t</Table>\n" << PLevel ();
	ReportXMLSmartNoticeObjects (XMLExcelLinks, tempXMLExcelLinks, " ");
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
	SmartMessagingObject::ReportAllExportSpecifications (XMLExcelLinks);

	// First add directory alerts and then...Merge here to get messages...

	XMLExcelLinks << CLevel (1) << "</OsirisAnalysisReport>" << endLine << PLevel ();
	SmartMessagingObject::ClearExportSpecifications ();

	tempInputXMLSummaryLinks.Close ();
	
	ReportSmartNoticeObjects (ExcelSummary, "", " ", FALSE);
	ReportSmartNoticeObjects (ExcelLinks, "", " ", TRUE);

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


bool STRLCAnalysis :: EvaluateSmartMessagesForStage (int stage) {

	RGDList temp;
	return SmartMessage::EvaluateAllMessages (mMessageArray, temp, stage, GetObjectScope ());
}


bool STRLCAnalysis :: EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage) {

	RGDListIterator it (mBaseLocusList);
	BaseLocus* nextBaseLocus;
	int newNum = numHigherObjects + 1;

	while (nextBaseLocus = (BaseLocus*) it ()) {

		comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) nextBaseLocus;
		nextBaseLocus->EvaluateSmartMessagesForStage (comm, newNum, stage);
	}

	it.Reset ();

	while (nextBaseLocus = (BaseLocus*) it ()) {

		comm.SMOStack [numHigherObjects] = (SmartMessagingObject*) nextBaseLocus;
		nextBaseLocus->SetTriggersForAllMessages (comm, newNum, stage);
	}

	return SmartMessage::EvaluateAllMessages (comm, numHigherObjects, stage, GetObjectScope ());
}


bool STRLCAnalysis :: SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage) {

	return SmartMessage::SetTriggersForAllMessages (comm, numHigherObjects, stage, GetObjectScope ());
}


bool STRLCAnalysis :: SetTriggersForAllMessages (bool* const higherMsgMatrix, int stage) {

	return SmartMessage::SetTriggersForAllMessages (mMessageArray, higherMsgMatrix, stage, GetObjectScope ());
}


bool STRLCAnalysis :: EvaluateAllReports (bool* const reportMatrix) {

	return SmartMessage::EvaluateAllReports (mMessageArray, reportMatrix, GetObjectScope ());
}


bool STRLCAnalysis :: TestAllMessagesForCall () {

	return SmartMessage::TestAllMessagesForCall (mMessageArray, GetObjectScope ());
}


bool STRLCAnalysis :: EvaluateAllReportLevels (int* const reportLevelMatrix) {

	return SmartMessage::EvaluateAllReportLevels (mMessageArray, reportLevelMatrix, GetObjectScope ());
}


void STRLCAnalysis :: OutputDebugID (SmartMessagingComm& comm, int numHigherObjects) {

	RGString idData = "Directory Level Debug";
	SmartMessage::OutputDebugString (idData);
}


int STRLCAnalysis :: AddAllSmartMessageReporters () {

	int k = GetObjectScope ();
	int size = SmartMessage::GetSizeOfArrayForScope (k);
	int i;
	int nMsgs = 0;
	SmartMessageReporter* newMsg;
	SmartMessage* nextSmartMsg;
	SmartMessageData target;
	SmartMessageData* smd;

	RGDListIterator loci (mBaseLocusList);
	BaseLocus* nextLocus;

	while (nextLocus = (BaseLocus*) loci ())
		nextLocus->AddAllSmartMessageReporters ();

	for (i=0; i<size; i++) {

		if (!mMessageArray [i])
			continue;

		nextSmartMsg = SmartMessage::GetSmartMessageForScopeAndElement (k, i);

		if (!nextSmartMsg->EvaluateReport (mMessageArray))
			continue;

		target.SetIndex (i);
		smd = (SmartMessageData*) mMessageDataTable->Find (&target);
		newMsg = new SmartMessageReporter;
		newMsg->SetSmartMessage (nextSmartMsg);
		
		if (smd != NULL)
			newMsg->SetData (smd);

		newMsg->SetPriorityLevel (nextSmartMsg->EvaluateReportLevel (mMessageArray));
		newMsg->SetRestrictionLevel (nextSmartMsg->EvaluateRestrictionLevel (mMessageArray));
		nMsgs = AddSmartMessageReporter (newMsg);
	}

	MergeAllSmartMessageReporters ();
	return nMsgs;
}


int STRLCAnalysis :: AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects) {

	int k = GetObjectScope ();
	int size = SmartMessage::GetSizeOfArrayForScope (k);
	int i;
	int nMsgs = 0;
	SmartMessageReporter* newMsg;
	SmartMessage* nextSmartMsg;
	SmartMessageData target;
	SmartMessageData* smd;

	bool editable;
	bool enabled;
	bool hasExportProtocolInfo;
	bool report;
	bool mirror;
	bool displayExport;

	RGDListIterator loci (mBaseLocusList);
	BaseLocus* nextLocus;

	while (nextLocus = (BaseLocus*) loci ())
		nextLocus->AddAllSmartMessageReporters (comm, numHigherObjects);

	for (i=0; i<size; i++) {

		nextSmartMsg = SmartMessage::GetSmartMessageForScopeAndElement (k, i);
		editable = nextSmartMsg->IsEditable ();
		hasExportProtocolInfo = nextSmartMsg->HasExportProtocolInfo ();

		if (!mMessageArray [i]) {

			enabled = false;

			if (!editable)
				continue;

			if (!hasExportProtocolInfo)
				continue;
		}

		else
			enabled = true;

		report = nextSmartMsg->EvaluateReportContingent (comm, numHigherObjects);
		mirror = nextSmartMsg->UseDefaultExportDisplayMode ();

		if (mirror)
			displayExport = report;

		else
			displayExport = nextSmartMsg->DisplayExportInfo ();

		if (!report && !displayExport)
			continue;

		target.SetIndex (i);
		smd = (SmartMessageData*) mMessageDataTable->Find (&target);
		newMsg = new SmartMessageReporter;
		newMsg->SetSmartMessage (nextSmartMsg);
		
		if (smd != NULL)
			newMsg->SetData (smd);

		newMsg->SetPriorityLevel (nextSmartMsg->EvaluateReportLevel (comm, numHigherObjects));
		newMsg->SetRestrictionLevel (nextSmartMsg->EvaluateRestrictionLevel (comm, numHigherObjects));
		newMsg->SetEditable (editable);
		newMsg->SetEnabled (enabled);
		newMsg->SetDisplayExportInfo (displayExport);
		newMsg->SetDisplayOsirisInfo (report);

		if (hasExportProtocolInfo) {
			
			newMsg->SetExportProtocolInformation (nextSmartMsg->GetExportProtocolList ());
			SmartMessagingObject::InsertExportSpecificationsIntoTable (nextSmartMsg);
		}

		newMsg->ComputeViabilityOfExportInfo ();
		nMsgs = AddSmartMessageReporter (newMsg);
	}

	MergeAllSmartMessageReporters ();
	return nMsgs;
}


void STRLCAnalysis :: InitializeMessageData () {

	int size = SmartMessage::GetSizeOfArrayForScope (GetObjectScope ());
	STRLCAnalysis::InitializeMessageMatrix (mMessageArray, size);
}


Boolean STRLCAnalysis :: ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim) {

	int nBaseMsgs = 0;
	RGDListIterator loci (mBaseLocusList);
	BaseLocus* nextBaseLocus;

	while (nextBaseLocus = (BaseLocus*) loci ())
		nBaseMsgs += nextBaseLocus->NumberOfSmartNoticeObjects ();

	if ((NumberOfSmartNoticeObjects () > 0) || (nBaseMsgs > 0)) {

		int msgLevel = GetHighestMessageLevelWithRestrictionSM ();
		RGDListIterator it (*mSmartMessageReporters);
		SmartMessageReporter* nextNotice;
		text.SetOutputLevel (msgLevel);

		//if (!text.TestCurrentLevel ()) {

		//	text.ResetOutputLevel ();
		//	return FALSE;
		//}

		Endl endLine;
		text << endLine;
		text << indent << "Notices for directory " << mCurrentInputDirectory << ":  " << endLine;

		while (nextNotice = (SmartMessageReporter*) it ())
			text << indent << nextNotice->GetMessage () << nextNotice->GetMessageData () << endLine;

		text.ResetOutputLevel ();
		text.Write (1, "\n");

		if (nBaseMsgs > 0) {

			loci.Reset ();

			while (nextBaseLocus = (BaseLocus*) loci ())
				nextBaseLocus->ReportSmartNoticeObjects (text, indent, delim);
		}
	}

	else
		return FALSE;

	return TRUE;
}


Boolean STRLCAnalysis :: ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink) {

	int nBaseMsgs = 0;
	RGDListIterator loci (mBaseLocusList);
	BaseLocus* nextBaseLocus;

	while (nextBaseLocus = (BaseLocus*) loci ())
		nBaseMsgs += nextBaseLocus->NumberOfSmartNoticeObjects ();

	if ((NumberOfSmartNoticeObjects () > 0) || (nBaseMsgs > 0)) {

		int msgLevel = GetHighestMessageLevelWithRestrictionSM ();
		RGDListIterator it (*mSmartMessageReporters);
		SmartMessageReporter* nextNotice;
		text.SetOutputLevel (msgLevel);
		RGString indent2 = indent + "\t";
		int link;

		//if (!text.TestCurrentLevel ()) {

		//	text.ResetOutputLevel ();
		//	return FALSE;
		//}

		Endl endLine;
		text << endLine;

		if (reportLink) {

			link = Notice::GetNextLinkNumber ();
			SetTableLink (link);
			text << mTableLink << "\t";
		}

		text << indent << "Notices for directory " << mCurrentInputDirectory << ":  " << endLine;

		while (nextNotice = (SmartMessageReporter*) it ())
			text << indent << nextNotice->GetMessage () << nextNotice->GetMessageData () << endLine;

		text.ResetOutputLevel ();

		if (reportLink) {

			text.SetOutputLevel (msgLevel);
			text << mTableLink;
			text.ResetOutputLevel ();
		}

		text.Write (1, "\n");

		if (nBaseMsgs > 0) {

			loci.Reset ();

			while (nextBaseLocus = (BaseLocus*) loci ())
				nextBaseLocus->ReportSmartNoticeObjects (text, indent, delim);
		}
	}

	else
		return FALSE;

	return TRUE;
}


Boolean STRLCAnalysis :: ReportXMLSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim) {

	int nBaseMsgs = 0;
	RGDListIterator loci (mBaseLocusList);
	BaseLocus* nextBaseLocus;

	while (nextBaseLocus = (BaseLocus*) loci ())
		nBaseMsgs += nextBaseLocus->NumberOfSmartNoticeObjects ();

	if ((NumberOfSmartNoticeObjects () > 0) || (nBaseMsgs > 0)) {

		int msgLevel = GetHighestMessageLevelWithRestrictionSM ();
		RGDListIterator it (*mSmartMessageReporters);
		SmartMessageReporter* nextNotice;
//		text.SetOutputLevel (msgLevel);
		tempText.SetOutputLevel (1);
		int msgNum;
		int triggerLevel = Notice::GetMessageTrigger ();
		bool viable;

		//if (!text.TestCurrentLevel ()) {

		//	text.ResetOutputLevel ();
		//	tempText.ResetOutputLevel ();
		//	return FALSE;
		//}

		text << CLevel (1) << "\t<DirectoryAlerts>\n" << PLevel ();
		text.SetOutputLevel (1);

		while (nextNotice = (SmartMessageReporter*) it ()) {

			viable = nextNotice->HasViableExportInfo ();
			msgLevel = nextNotice->GetMessagePriority ();

			if (((msgLevel > 0) && (msgLevel <= triggerLevel)) || viable) {

				msgNum = Notice::GetNextMessageNumber ();
				nextNotice->SetMessageCount (msgNum);
				text << "\t\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
				tempText << "\t\t<Message>\n";
				tempText << "\t\t\t<MessageNumber>" << msgNum << "</MessageNumber>\n";
				tempText << "\t\t\t<Text>" << nextNotice->GetMessage () << nextNotice->GetMessageData () << "</Text>\n";

				if (viable) {

					if (nextNotice->IsEnabled ())
						tempText << "\t\t\t<Hidden>false</Hidden>\n";

					else
						tempText << "\t\t\t<Hidden>true</Hidden>\n";

					if (!nextNotice->IsCritical ())
						tempText << "\t\t\t<Critical>false</Critical>\n";

					if (nextNotice->IsEnabled ())
						tempText << "\t\t\t<Enabled>true</Enabled>\n";

					else
						tempText << "\t\t\t<Enabled>false</Enabled>\n";

					if (!nextNotice->IsEditable ())
						tempText << "\t\t\t<Editable>false</Editable>\n";

					if (nextNotice->GetDisplayExportInfo ())
						tempText << "\t\t\t<DisplayExportInfo>true</DisplayExportInfo>\n";

					else
						tempText << "\t\t\t<DisplayExportInfo>false</DisplayExportInfo>\n";

					if (!nextNotice->GetDisplayOsirisInfo ())
						tempText << "\t\t\t<DisplayOsirisInfo>false</DisplayOsirisInfo>\n";

					tempText << "\t\t\t<MsgName>" << nextNotice->GetMessageName () << "</MsgName>\n";

					//tempText << "\t\t\t<ExportProtocolList>";
					//tempText << "\t\t\t" << nextNotice->GetExportProtocolInformation ();
					//tempText << "\t\t\t</ExportProtocolList>\n";
				}

				else
					tempText << "\t\t\t<MsgName>" << nextNotice->GetMessageName () << "</MsgName>\n";

				tempText << "\t\t</Message>\n";
			}
		}

		if (nBaseMsgs > 0) {

			loci.Reset ();

			while (nextBaseLocus = (BaseLocus*) loci ())
				nextBaseLocus->ReportXMLSmartNoticeObjectsWithExport (text, tempText, delim);
		}

		text.ResetOutputLevel ();
		text << CLevel (1) << "\t</DirectoryAlerts>\n";
		text.ResetOutputLevel ();
		tempText.ResetOutputLevel ();
	}

	else
		return FALSE;

	return TRUE;
}


void STRLCAnalysis :: CreateInitializationData (int scope) {

	int size = SmartMessage::GetSizeOfArrayForScope (scope);
	int i;
	SmartMessage* msg;
	delete[] InitialMatrix;
	InitialMatrix = new bool [size];

	for (i=0; i<size; i++) {

		msg = SmartMessage::GetSmartMessageForScopeAndElement (scope, i);

		if (msg != NULL)
			InitialMatrix [i] = msg->GetInitialValue ();
	}
}


void STRLCAnalysis :: InitializeMessageMatrix (bool* matrix, int size) {

	int i;

	for (i=0; i<size; i++)
		matrix [i] = InitialMatrix [i];
}


void STRLCAnalysis :: CreateAllInitializationMatrices () {

	int scope = DataSignal::GetScope ();
	DataSignal::CreateInitializationData (scope);

	scope = Locus::GetScope ();
	Locus::CreateInitializationData (scope);

	scope = ChannelData::GetScope ();
	ChannelData::CreateInitializationData (scope);

	scope = CoreBioComponent::GetScope ();
	CoreBioComponent::CreateInitializationData (scope);

	scope = BaseLocus::GetScope ();
	BaseLocus::CreateInitializationData (scope);

	scope = STRLCAnalysis::GetScope ();
	STRLCAnalysis::CreateInitializationData (scope);
}


void STRLCAnalysis :: ClearAllInitializationMatrices () {

	DataSignal::ClearInitializationMatrix ();
	Locus::ClearInitializationMatrix ();
	ChannelData::ClearInitializationMatrix ();
	CoreBioComponent::ClearInitializationMatrix ();
	BaseLocus::ClearInitializationMatrix ();
	STRLCAnalysis::ClearInitializationMatrix ();
}


void STRLCAnalysis :: PreInitializeSmartMessages () {

	mMessageArray = NULL;
	mMessageDataTable = NULL;
	mSmartMessageReporters = NULL;
}

