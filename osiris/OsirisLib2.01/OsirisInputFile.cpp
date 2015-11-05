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
*  FileName: OsirisInputFile.cpp
*  Author:   Robert Goor
*
*/
//
// OsirisInputFile is class to input basic settings that determine location and identity of configuration files plus data input files, etc.
// Assumes each line ends with a ";" and last line has nothing on it but a ";"
//

#include "OsirisInputFile.h"
#include "rgtokenizer.h"
#include <iostream>

using namespace std;


OsirisInputFile :: OsirisInputFile (bool debug) : mDebug (debug), mInputFile (NULL), mCriticalOutputLevel (15), mMinSampleRFU (0.0),
mMinLadderRFU (0.0), mMinLaneStandardRFU (0.0), mMinInterlocusRFU (0.0), mMinLadderInterlocusRFU (0.0), mSampleDetectionThreshold (-1.0), 
mUseRawData (true), mUserNamedSettingsFiles (true) {

	mInputLinesIterator = new RGDListIterator (mInputLines);
	mAnalysisThresholds = new list<channelThreshold*>;
	mDetectionThresholds = new list<channelThreshold*>;
}



OsirisInputFile :: ~OsirisInputFile () {

	if (!(mInputFile == NULL)) {

		mInputFile->Close ();
		delete mInputFile;
		mInputFile = NULL;
	}

	delete mInputLinesIterator;
	mInputLines.ClearAndDelete ();

	list<channelThreshold*>::iterator c3Iterator;
	list<channelThreshold*>::iterator c4Iterator;
	channelThreshold* nextThreshold;

	for (c3Iterator = mAnalysisThresholds->begin (); c3Iterator != mAnalysisThresholds->end (); c3Iterator++) {

		nextThreshold = *c3Iterator;
		delete nextThreshold;
	}

	mAnalysisThresholds->clear ();
	delete mAnalysisThresholds;

	for (c4Iterator = mDetectionThresholds->begin (); c4Iterator != mDetectionThresholds->end (); c4Iterator++) {

		nextThreshold = *c4Iterator;
		delete nextThreshold;
	}

		mDetectionThresholds->clear ();
		delete mDetectionThresholds;
}



int OsirisInputFile :: ReadAllInputs (const RGString& inputFileName) {

	if (mDebug) {

		mInputFile = new RGFile (inputFileName, "rt");

		if (!mInputFile->isValid ()) {

			cout << "Basic input file named:  " << inputFileName.GetData () << " is not valid." << endl;
			delete mInputFile;
			mInputFile = NULL;
			return -1;
		}
	}

	return ReadAllInputs ();
}


int OsirisInputFile :: ReadAllInputs () {
	
	// reads either named file or stdin, depending on debug flag

	int status;
	int returnStatus;

	while (true) {

		status = ReadLine ();

		if (status == 0) {

			status = AssignString ();

			if (status != 0) {

				cout << "Assignment of data in input file failed.  Terminating..." << endl;
				returnStatus = -1;
				break;
			}
		}

		else if (status == 1) {

			// we're done and all was successful!

			returnStatus = 0;
			break;
		}

		else {

			cout << "Parsing input file terminated prematurely..." << endl;
			returnStatus = -1;
			break;
		}
	}

	if (mInputFile != NULL) {

		mInputFile->Close ();
		delete mInputFile;
		mInputFile = NULL;
	}

	return returnStatus;
}


int OsirisInputFile :: ReadLine () {

	RGString temp;
	size_t equalsPosition = 0;
	char T;
	bool leftOfEquals = true;
	RGString* nextLine;
	RGString thisLine;

	if (mDebug && (mInputFile != NULL)) {

		// input is coming from file...

		temp.ReadTextLine (*mInputFile);
		mCumulativeStringWithNewLines += temp + "\n";
		mCumulativeStringWithoutNewLines += temp;
		nextLine = new RGString (temp);
		mInputLines.Append (nextLine);

		if (mInputFile->Eof ()) {

			cout << "Found end of file in osiris input file" << endl;
			return 1;
		}

		if (temp.Length () == 0) {

			cout << "Found end of input in osiris input file" << endl;
			return 1;
		}

		if (temp.GetFirstCharacter () == ';') {

			cout << "Found end of input in osiris input file" << endl;
			return 1;
		}

		if (!temp.FindNextSubstring (0, "=", equalsPosition)) {

			cout << "Found no equal sign in line:  " << temp.GetData () << endl;
			return -1;
		}

		if ((equalsPosition == 0) || (equalsPosition == temp.Length () - 2)) {

			cout << "Found equal sign too close to end in line:  " << temp.GetData () << endl;
			return -1;
		}

		mStringLeft = temp.ExtractSubstring (0, equalsPosition - 1);
		mStringRight = temp.ExtractSubstring (equalsPosition + 1, temp.Length () - 1);

		RemoveLeadingAndTrailingBlanks (mStringLeft);
		RemoveLeadingAndTrailingBlanks (mStringRight);

		while (mStringRight.Length () > 0) {

			T = mStringRight.RemoveLastCharacter ();

			if (T == ';')
				break;
		}

		if (mStringRight.Length () == 0) {

			cout << "Found no semi-colon in line:  " << temp.GetData () << endl;
			return -1;
		}

		cout << "Found osiris input file line:  " << mStringLeft.GetData () << " = " << mStringRight.GetData () << endl;
		return 0;
	}

	if (mInputFile != NULL) {
		
		cout << "Not in debug mode and yet we have an osiris input file...what gives?" << endl;
		return -1;
	}

	// Assume input is coming on stdin...

	mStringLeft = "";
	mStringRight = "";

	while (true) {

		cin >> noskipws >> T;

		if (T == '\n')
			continue;

		mCumulativeStringWithNewLines.Append (T);
		mCumulativeStringWithoutNewLines.Append (T);
		thisLine.Append (T);

		if (T == ';') {

			mCumulativeStringWithNewLines.Append ('\n');	//???????
			break;
		}

		if (leftOfEquals) {

			if (T == '=') {

				leftOfEquals = false;
			}

			else
				mStringLeft.Append (T);
		}

		else
			mStringRight.Append (T);
	}

	nextLine = new RGString (thisLine);
	mInputLines.Append (nextLine);

	RemoveLeadingAndTrailingBlanks (mStringLeft);
	RemoveLeadingAndTrailingBlanks (mStringRight);

	if (mStringLeft.Length () == 0) {

		cout << "Found end of data:  " << thisLine.GetData () << endl;
		return 1;
	}

	if (leftOfEquals) {

		cout << "Found no equal sign in line:  " << thisLine.GetData () << endl;
		return -1;
	}

	if (mStringRight.Length () == 0) {

		cout << "Warning:  found no right hand side in line:  " << thisLine.GetData () << endl;
	}

	return 0;
}


int OsirisInputFile :: AssignString () {

	char T;
	int status = -1;
	size_t posn;
	RGString temp;
	RGString channelStr;
	double threshold;
	int channel;
	channelThreshold* nextThreshold;

	if (mStringLeft == "InputDirectory") {

		SetEmbeddedSlashesToForward (mStringRight);
		mInputDirectory = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "LadderDirectory") {

		SetEmbeddedSlashesToForward (mStringRight);
		mLadderDirectory = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "ReportDirectory") {

		SetEmbeddedSlashesToForward (mStringRight);
		mReportDirectory = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "OutputSubdirectory") {

		mOutputSubDirectory = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "MarkerSetName") {

		mMarkerSetName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "LaneStandardName") {

		mLaneStandardName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "CriticalOutputLevel") {

		mCriticalOutputLevel = mStringRight.ConvertToInteger ();
		cout << "Critical level = " << mCriticalOutputLevel << endl;
		status = 0;
	}

	else if (mStringLeft == "MinSampleRFU") {

		mMinSampleRFU = mStringRight.ConvertToDouble ();
		cout << "Min Sample RFU = " << mMinSampleRFU << endl;
		status = 0;
	}

	else if (mStringLeft == "MinLaneStandardRFU") {

		mMinLaneStandardRFU = mStringRight.ConvertToDouble ();
		cout << "Min Lane Standard RFU = " << mMinLaneStandardRFU << endl;
		status = 0;
	}

	else if (mStringLeft == "MinLadderRFU") {

		mMinLadderRFU = mStringRight.ConvertToDouble ();
		cout << "Min Ladder RFU = " << mMinLadderRFU << endl;
		status = 0;
	}

	else if (mStringLeft == "MinInterlocusRFU") {

		mMinInterlocusRFU = mStringRight.ConvertToDouble ();
		cout << "Min Interlocus RFU = " << mMinInterlocusRFU << endl;
		status = 0;
	}

	else if (mStringLeft == "MinLadderInterlocusRFU") {

		mMinLadderInterlocusRFU = mStringRight.ConvertToDouble ();
		cout << "Min Ladder Interlocus RFU = " << mMinLadderInterlocusRFU << endl;
		status = 0;
	}

	else if (mStringLeft == "SampleDetectionThreshold") {

		mSampleDetectionThreshold = mStringRight.ConvertToDouble ();
		cout << "Sample Detection Threshold = " << mSampleDetectionThreshold << endl;
		status = 0;
	}

	else if (mStringLeft == "RawDataString") {

		if (!(mStringRight.Length () == 0)) {

			T = mStringRight.GetCharacter (0);

			if ((T == 'a') || (T == 'A')) {

				cout << "Don't use raw data..." << endl;
				mUseRawData = false;
			}

			else {

				cout << "Use raw data..." << endl;
				mUseRawData = true;
			}

			status = 0;
		}

		else {

			mUseRawData = true;
			cout << "Using raw data by default..." << endl;
			status = 0;
		}
	}

	else if (mStringLeft == "UserNamedSettingsFiles") {

		if (!(mStringRight.Length () == 0)) {

			T = mStringRight.GetCharacter (0);

			if ((T == 'F') || (T == 'f')) {

				cout << "Don't input user-named settings files..." << endl;
				mUserNamedSettingsFiles = false;
			}

			else {

				cout << "Input user-named settings files..." << endl;
				mUserNamedSettingsFiles = true;
			}

			status = 0;
		}
	}

	else if (mStringLeft == "LabSettings") {

		SetEmbeddedSlashesToForward (mStringRight);
		mFinalLabSettingsName = mStringRight;
		return 0;
	}

	else if (mStringLeft == "StandardSettings") {

		SetEmbeddedSlashesToForward (mStringRight);
		mFinalStdSettingsName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "MessageBook") {

		SetEmbeddedSlashesToForward (mStringRight);
		mFinalMessageBookName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "override") {

		mOverrideString = mStringRight;
		status = 0;
	}

	else if (mStringLeft.FindSubstring ("AnalysisThresholdOverride", posn)) {

		temp = SplitUsingColon (mStringLeft, channelStr);

		if (temp.Length () == 0)
			status = -1;

		else {

			channel = channelStr.ConvertToInteger ();
			threshold = mStringRight.ConvertToDouble ();
			nextThreshold = new channelThreshold (channel, threshold);
			mAnalysisThresholds->push_back (nextThreshold);
			status = 0;
		}
	}

	else if (mStringLeft.FindSubstring ("DetectionThresholdOverride", posn)) {

		temp = SplitUsingColon (mStringLeft, channelStr);

		if (temp.Length () == 0)
			status = -1;

		else {

			channel = channelStr.ConvertToInteger ();
			threshold = mStringRight.ConvertToDouble ();
			nextThreshold = new channelThreshold (channel, threshold);
			mDetectionThresholds->push_back (nextThreshold);
			status = 0;
		}
	}

	else if (mStringLeft == "#")
		status = 0;

	if (status == 0)
		mOutputString += mStringLeft + " = " + mStringRight + "\n";

	else
		cout << "Problem with assign string:  " << mStringLeft.GetData () << " = " << mStringRight.GetData () << endl;

	return status;
}


void OsirisInputFile :: OutputAllData () {

	cout << "Raw input string:" << endl;
	cout << mCumulativeStringWithNewLines.GetData () << endl;
	cout << "\nAssigned input strings:" << endl;
	cout << mOutputString.GetData () << endl;
}


int OsirisInputFile :: AssembleInputs () {

	int status = 0;
	RGString labSettings1;
	RGString labSettings2;
	
	if (mInputDirectory.Length () == 0) {

		cout << "Input directory is unspecified." << endl;
		status = -1;
	}

	if (mLadderDirectory.Length () == 0) {

		cout << "Ladder information directory is unspecified." << endl;
		status = -1;
	}

	if (mReportDirectory.Length () == 0) {

		cout << "Report directory is unspecified." << endl;
		status = -1;
	}

	if (mMarkerSetName.Length () == 0) {

		cout << "Marker set name is unspecified." << endl;
		status = -1;
	}

	if (mLaneStandardName.Length () == 0) {

		cout << "Lane standard name is unspecified." << endl;
		status = -1;
	}

	if (mFinalMessageBookName.Length () == 0) {

		cout << "Message Book file is unspecified." << endl;
		status = -1;
	}

	if (mFinalLabSettingsName.Length () == 0) {

		cout << "Lab Settings file is unspecified." << endl;
		status = -1;
	}

	if (mFinalStdSettingsName.Length () == 0) {

		cout << "Standard Settings file is unspecified." << endl;
		status = -1;
	}

	if (mMinSampleRFU == 0.0) {

		cout << "Min sample RFU is unspecified." << endl;
		status = -1;
	}

	if (mMinLadderRFU == 0.0) {

		cout << "Min ladder RFU is unspecified." << endl;
		status = -1;
	}

	if (mMinLaneStandardRFU == 0.0) {

		cout << "Min lane standard RFU is unspecified." << endl;
		status = -1;
	}

	if (mSampleDetectionThreshold < 0.0) {

		cout << "Sample Detection Threshold is unspecified.  Using Min Sample RFU..." << endl;
		mSampleDetectionThreshold = mMinSampleRFU;
	}

	if (mMinInterlocusRFU < mMinSampleRFU)
		mMinInterlocusRFU = mMinSampleRFU;

	if (mMinLadderInterlocusRFU < mMinLadderRFU)
		mMinLadderInterlocusRFU = mMinLadderRFU;

	if (mSampleDetectionThreshold > mMinSampleRFU) {

		cout << "Sample Detection Threshold is larger than Min Sample RFU...Making them equal."  << endl;
		mSampleDetectionThreshold = mMinSampleRFU;
	}

	if (!mUserNamedSettingsFiles) {

		if (mFullPathToMessageBook.Length () == 0) {

			cout << "Message book path unspecified." << endl;
			status = -1;
		}

		if (mFullPathToLabAndStdSettings.Length () == 0) {

			cout << "Lab settings path and standard settings path unspecified." << endl;
			status = -1;
		}

		if (status == 0) {

			mFinalMessageBookName = mFullPathToMessageBook + "/OsirisMessageBookV4.0.xml";
			mFinalStdSettingsName = mFullPathToLabAndStdSettings + "/OsirisSTRStandardSettings.xml";
			labSettings1 = mFullPathToLabAndStdSettings + "/GenericLabSettings.xml";
			labSettings2 = mFullPathToLabAndStdSettings + "/defaultlab.xml";

			FILE *fp = fopen((const char *)labSettings1.GetData (),"rb");

			if (fp == NULL) {

				fp = fopen((const char *)labSettings2.GetData (),"rb");

				if (fp == NULL) {

					cout << "Cannot locate appropriate lab settings file, either " << labSettings1.GetData () << " or " << labSettings2.GetData () << endl;
					status = -1;
				}

				else {

					fclose (fp);
					mFinalLabSettingsName = labSettings2;
				}
			}

			else {

				fclose (fp);
				mFinalLabSettingsName = labSettings1;
			}
		}
	}

	return 0;
}


void OsirisInputFile :: ResetInputLines () {

	mInputLinesIterator->Reset ();
}


RGString* OsirisInputFile :: GetNextInputLine () {

	return (RGString*) (*mInputLinesIterator) ();
}


void OsirisInputFile :: OutputAnalysisThresholdOverrides () {

	list<channelThreshold*>::iterator cIterator;
	channelThreshold* nextThreshold;

	for (cIterator = mAnalysisThresholds->begin (); cIterator != mAnalysisThresholds->end (); cIterator++) {

		nextThreshold = *cIterator;
		cout << "AnalysisThresholdOverride:" << nextThreshold->mChannel << " = " << (int)nextThreshold->mThreshold << "\n";
	}
}



void OsirisInputFile :: OutputDetectionThresholdOverrides () {

	list<channelThreshold*>::iterator cIterator;
	channelThreshold* nextThreshold;

	for (cIterator = mDetectionThresholds->begin (); cIterator != mDetectionThresholds->end (); cIterator++) {

		nextThreshold = *cIterator;
		cout << "DetectionThresholdOverride:" << nextThreshold->mChannel << " = " << (int)nextThreshold->mThreshold << "\n";
	}
}



void OsirisInputFile :: RemoveLeadingAndTrailingBlanks (RGString& string) {

	char T;

	while (string.Length () > 0) {

		T = string.GetLastCharacter ();

		if ((T == ' ') || (T == '\t') || (T == '\n'))
			string.RemoveLastCharacter ();

		else
			break;
	}

	string.Reverse ();

	while (string.Length () > 0) {

		T = string.GetLastCharacter ();

		if ((T == ' ') || (T == '\t') || (T == '\n'))
			string.RemoveLastCharacter ();

		else
			break;
	}

	string.Reverse ();
}


void OsirisInputFile :: SetEmbeddedSlashesToForward (RGString& string) {

	string.FindAndReplaceAllSubstrings ("\\", "/");

	if (string.GetLastCharacter () == '/')
		string.RemoveLastCharacter ();
}


RGString OsirisInputFile :: SplitUsingColon (const RGString& target, RGString& right) { 

	// returns what's on left of colon, and empty if no colon

	RGString temp = target;
	RGStringTokenizer parse (temp);
	parse.AddDelimiter (":");
	RGStringArray tokens;
	RGStringArray delimiters;
	RGString Ans;
	right = "";

	parse.Split (tokens, delimiters);

	if (tokens.Length () != 2)
		return Ans;

	right = tokens [1];
	return tokens [0];
}

