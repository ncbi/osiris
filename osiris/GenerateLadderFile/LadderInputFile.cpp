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
*  FileName: LadderInputFile.cpp
*  Author:   Robert Goor
*
*/
//
// LadderInputFile is class to input basic settings that determine location and identity of configuration files plus data input files, etc.
// Assumes each line ends with a ";" and last line has nothing on it but a ";"
//

#include "LadderInputFile.h"
#include "LadderStructures.h"
#include "rgpscalar.h"
#include "rgindexedlabel.h"
#include <iostream>

using namespace std;


int LadderInputFile::PanelsNumberOfLinesSkipped = 4;
int LadderInputFile::ColorColumn = 2;
int LadderInputFile::RepeatSizeColumn = 6;
int LadderInputFile::AlleleListColumn = 8;
RGString LadderInputFile::AlleleListDelineation = ",";
RGString LadderInputFile::ColumnDelineation = "\t";
RGString LadderInputFile::BinsDelineation = "\t";
LocusNames LadderInputFile::StandardNames;



LadderInputFile :: LadderInputFile (bool debug) : mDebug (debug), mInputFile (NULL), mNumberOfDyes (0), mYLinkedDefault (false),
mMaxExpectedAllelesPerLocusDefault (2), mMinExpectedAllelesPerLocusDefault (1), mChannelForILS (0), mVersion ("2.7"), 
mGenerateILSFamilies (false) {

	mInputLinesIterator = new RGDListIterator (mInputLines);
	mGenerateILSFamilies = true;
	Locus::SetGenerateILSFamilies (true);
}



LadderInputFile :: ~LadderInputFile () {

	if (!(mInputFile == NULL)) {

		mInputFile->Close ();
		delete mInputFile;
		mInputFile = NULL;
	}

	delete mInputLinesIterator;
	mInputLines.ClearAndDelete ();
	mDyeNumbers.ClearAndDelete ();
	mColorNumbers.ClearAndDelete ();
	mILSNames.ClearAndDelete ();
	mRelativeHeightOverrides.ClearAndDelete ();
	mQualityLoci.ClearAndDelete ();
}



int LadderInputFile :: ReadAllInputs (const RGString& inputFileName) {

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


int LadderInputFile :: ReadAllInputsAppend (const RGString& inputFileName) {

	if (mDebug) {

		mInputFile = new RGFile (inputFileName, "rt");

		if (!mInputFile->isValid ()) {

			cout << "Basic input file named:  " << inputFileName.GetData () << " could not be read." << endl;
			delete mInputFile;
			mInputFile = NULL;
			return -1;
		}
	}

	return ReadAllInputsAppend ();
}


int LadderInputFile :: ReadAllInputs () {
	
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

			cout << "Input file read and data assigned successfully." << endl;
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


int LadderInputFile :: ReadAllInputsAppend () {

	// reads either named file or stdin, depending on debug flag

	int status;
	int returnStatus;

	while (true) {

		status = ReadLine ();

		if (status == 0) {

			status = AssignStringAppend ();

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


int LadderInputFile :: ReadLine () {

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

		cout << "Found ladder input file line:  " << mStringLeft.GetData () << " = " << mStringRight.GetData () << endl;
		return 0;
	}

	if (mInputFile != NULL) {
		
		cout << "Not in debug mode and yet we have a ladder input file...what gives?" << endl;
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


int LadderInputFile :: ReadFirstLine () {

	int status = ReadLine ();

	if (status != 0) {

		cout << "***ERROR***  Incorrect first line." << endl;
		return -1;
	}

	if (mStringLeft != "LadderOperation") {

		cout << "***ERROR***  First line must begin with 'LadderOperation = '" << endl;
		return -2;
	}

	if (mStringRight == "New")
		return 0;

	if (mStringRight == "Amend")
		return 1;

	cout << "***ERROR***  First liine must end with either 'New' or 'Amend'." << endl;
	return -2;
}


int LadderInputFile :: AssignString () {

	int status = -1;
	RGString temp;
	int channel;
	RGPInt* num;
	int tempInt;
	unsigned long tempUL;
	RGString* newString;
	RGString locus;
	RGString value;
	RGIndexedLabel* nextIndexedLabel;
	size_t position;
	RGString channelString;
	RGString locusOLString;
	int i;
	double dVersion;
	RGString tempName;

	if (mStringLeft == "LadderFileName") {

		SetEmbeddedSlashesToForward (mStringRight);
		mLadderFileName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "KitName") {

		SetEmbeddedSlashesToForward (mStringRight);
		mKitName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "LadderDirectory") {

		SetEmbeddedSlashesToForward (mStringRight);
		mLadderDirectory = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "NumberOfDyes") {

		mNumberOfDyes = mStringRight.ConvertToInteger ();
		mChannelMap.ExpandToSize (mNumberOfDyes + 1);

		for (i=1; i<=mNumberOfDyes; i++) {

			if (mChannelMap [i] == 0)
				mChannelMap [i] = i;
		}

		status = 0;
	}

	else if (mStringLeft == "RelativeHeightOverride") {

		tempName = LadderInputFile::FindLocusNameInList (mStringRight);  // tempName is the accepted name of the  locus
		
		if (tempName.Length () != 0) {

			newString = new RGString (tempName);
			mRelativeHeightOverrides.Append (newString);
			status = 0;
		}

		else
			cout << "Locus named " << mStringRight << " not recognized" << endl;
	}

	else if (mStringLeft == "PanelsFileName") {

		SetEmbeddedSlashesToForward (mStringRight);
		mPanelsFileName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "BinsFileName") {

		SetEmbeddedSlashesToForward (mStringRight);
		mBinsFileName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "SearchString") {

		mSearchString = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "OutputConfigPath") {

		SetEmbeddedSlashesToForward (mStringRight);
		mOutputConfigDirectoryPath = mStringRight;
		AddLocusNamesAndControlNamesFromControlFile (mOutputConfigDirectoryPath + "/ConfigurationTools/StandardPositiveControl/StandardPositiveControlList.txt");
		status = 0;
	}

	else if (mStringLeft == "ILSChannel") {

		mChannelForILS = mStringRight.ConvertToInteger ();
		status = 0;
	}

	else if (mStringLeft == "YLinkedDefault") {

		if ((mStringRight == "true") || (mStringRight == "1")) {

			mYLinkedDefault = true;
			status = 0;
		}

		else if ((mStringRight == "false") || (mStringRight == "0")) {

			mYLinkedDefault = false;
			status = 0;
		}
	}

	else if (mStringLeft == "MaxExpectedAllelesPerLocusDefault") {

		tempInt = mStringRight.ConvertToInteger ();

		if (tempInt > 0) {

			mMaxExpectedAllelesPerLocusDefault = tempInt;
			status = 0;
		}
	}

	else if (mStringLeft == "MinExpectedAllelesPerLocusDefault") {

		tempInt = mStringRight.ConvertToInteger ();

		if (tempInt > 0) {

			mMinExpectedAllelesPerLocusDefault = tempInt;
			status = 0;
		}
	}

	else if (mStringLeft == "ILSName") {

		newString = new RGString (mStringRight);
		
		mILSNames.Append (newString);
//		Locus::SetILSName (mStringRight);  // ????  Nope...this is for ILS families only
		status = 0;
	}

	else if (mStringLeft == "ILSFamilyName") {

		mILSFamilyName = mStringRight;
		Locus::SetILSFamilyName (mStringRight);  // Yes!  For ILS families
		newString = new RGString(mStringRight);
//		mILSNames.Append(newString);
		status = 0;
	}

	else if (mStringLeft == "Version") {

		mVersion = mStringRight;
		dVersion = mVersion.ConvertToDouble ();

		if (dVersion > 2.69) {

			mGenerateILSFamilies = true;
			Locus::SetGenerateILSFamilies (true);
		}

		else {

			mGenerateILSFamilies = false;
			Locus::SetGenerateILSFamilies (false);
		}

		status = 0;
	}

	else if (mStringLeft == "YLinkedOverride") {
		
		tempName = LadderInputFile::FindLocusNameInList (mStringRight);

		if (tempName.Length () != 0) {

			newString = new RGString (tempName);
			mYLinkedDefaultOverrides.Append (newString);
			status = 0;
		}

		else
			cout << "Locus named " << mStringRight << " not recognized" << endl;
	}

	else if (mStringLeft.FindSubstring ("KitDataChannelOverride", position)) {

		temp = SplitUsingColon (mStringLeft, channelString);

		if (temp.Length () == 0)
			cout << "This shouldn't happen, but here goes... couldn't find kit data channel override string" << endl;

		else {

			channel = channelString.ConvertToInteger ();
			mChannelMap [channel] = mStringRight.ConvertToInteger ();
			status = 0;
		}
	}

	else if (mStringLeft.FindSubstring ("AcceptedOL", position)) {

		temp = SplitUsingColon (mStringLeft, locusOLString);

		if (temp.Length () == 0)
			cout << "This shouldn't happen, but here goes... couldn't find OL locus string" << endl;

		else {

			//  locusOLString is the locus for the OL allele and mStringRight is the OL allele name
			tempName = LadderInputFile::FindLocusNameInList (locusOLString);  // tempName is the accepted locus name

			if (tempName.Length () != 0) {

				mAcceptedOLAlleles << "        <Locus>\n";
				mAcceptedOLAlleles << "          <Name>" << tempName << "</Name>\n";
				mAcceptedOLAlleles << "          <Allele>" << mStringRight << "</Allele>\n";
				mAcceptedOLAlleles << "        </Locus>\n";
				cout << "Found accepted OL Allele Specification:  " << tempName << " = " << mStringRight << "\n";
				status = 0;
			}

			else
				cout << "Locus named " << locusOLString << " not recognized" << endl;
		}

	}

	else if (mStringLeft == "HID") {

		if (mStringRight == "true")
			mGenerateHID = true;

		else
			mGenerateHID = false;

		status = 0;
	}

	else if (mStringLeft == "StdControl") {

		tempName = LadderInputFile::FindPositiveControlNameInList (mStringRight);

		if (tempName.Length () == 0) {

			cout << mStringRight.GetData () << " does not match any of the accepted standard positive control names" << endl;
		}

		else {
				
			mStandardPositiveControlName = tempName;
			status = 0;
		}
	}

	else if (mStringLeft == "VolumeDirectoryName") {

		mVolumeDirectoryName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "DefaultVolumePath") {

		//mStringRight.FindAndReplaceAllSubstrings ("\\", "\/");
		mDefaultVolumePath = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "QualityLocus") {

		tempName = LadderInputFile::FindLocusNameInList (mStringRight);  // tempName is the accepted locus name

		if (tempName.Length () != 0) {

			newString = new RGString (tempName);
			mQualityLoci.Append (newString);
			status = 0;
		}

		else
			cout << "Locus named " << mStringRight << " not recognized" << endl;
	}

	else if (mStringLeft == "MaxExpectedAlleles") {

		locus = SplitUsingColon (mStringRight, value);

		if ((locus.Length () != 0) && (value.Length () != 0)) {

			tempUL = value.ConvertToUnsignedLong ();

			if (tempUL > 0) {

				tempName = LadderInputFile::FindLocusNameInList (locus);  // tempName is the accepted locus name

				if (tempName.Length () != 0) {

					nextIndexedLabel = new RGIndexedLabel (tempUL, tempName, "MaxExpectedAlleles");
					mMaxExpectedAllelesOverrides.Append (nextIndexedLabel);
					status = 0;
				}

				else
					cout << "Locus named " << locus << " not recognized" << endl;
			}
		}
	}

	else if (mStringLeft == "MinExpectedAlleles") {

		locus = SplitUsingColon (mStringRight, value);

		if ((locus.Length () != 0) && (value.Length () != 0)) {

			tempUL = value.ConvertToUnsignedLong ();

			if (tempUL > 0) {
			
				tempName = LadderInputFile::FindLocusNameInList (locus);  // tempName is the accepted locus name

				if (tempName.Length () != 0) {

					nextIndexedLabel = new RGIndexedLabel (tempUL, tempName, "MinExpectedAlleles");
					mMinExpectedAllelesOverrides.Append (nextIndexedLabel);
					status = 0;
				}

				else
					cout << "Locus named " << locus << " not recognized" << endl;
			}
		}
	}

	else if (mStringLeft == "DoNotExtend") {
	
		tempName = LadderInputFile::FindLocusNameInList (mStringRight);  // tempName is the accepted locus name

		if (tempName.Length () != 0) {

			newString = new RGString (tempName);
			mDoNotExtends.Append (newString);
			cout << "Do not extend " << tempName.GetData () << endl;
			status = 0;
		}

		else
			cout << "Locus named " << mStringRight << " not recognized" << endl;
	}

	else if (mStringLeft == "NumberOfPanelsLinesSkipped") {

		int n = mStringRight.ConvertToInteger ();
		LadderInputFile::SetNumberOfPanelsLinesSkipped (n);
		status = 0;
	}

	else if (mStringLeft == "ColorColumn") {

		int n = mStringRight.ConvertToInteger ();
		LadderInputFile::SetColorColumn (n);
		status = 0;
	}

	else if (mStringLeft == "RepeatSizeColumn") {

		int n = mStringRight.ConvertToInteger ();
		LadderInputFile::SetRepeatSizeColumn (n);
		status = 0;
	}

	else if (mStringLeft == "AlleleListColumn") {

		int n = mStringRight.ConvertToInteger ();
		LadderInputFile::SetAlleleListColumn (n);
		status = 0;
	}

	else if (mStringLeft == "AlleleListDelineation") {

		mStringRight.FindAndReplaceAllSubstrings (" ", "");

		if (mStringRight == "tab")
			LadderInputFile::SetAlleleListDelineation ("\t");

		else
			LadderInputFile::SetAlleleListDelineation (mStringRight);

		//cout << "Allele List Delineation = " << (char*)mStringRight.GetData () << endl;
		status = 0;
	}

	else if (mStringLeft == "ColumnDelineation") {

		mStringRight.FindAndReplaceAllSubstrings (" ", "");

		if (mStringRight == "tab")
			LadderInputFile::SetColumnDelineation ("\t");

		else
			LadderInputFile::SetColumnDelineation (mStringRight);

		//cout << "Column Delineation = " << (char*)mStringRight.GetData () << endl;
		status = 0;
	}

	else if (mStringLeft == "BinsDelineation") {

		mStringRight.FindAndReplaceAllSubstrings (" ", "");

		if (mStringRight == "tab")
			LadderInputFile::SetBinsDelineation ("\t");

		else
			LadderInputFile::SetBinsDelineation (mStringRight);
		
		//cout << "Bins line Delineation = " << (char*)mStringRight.GetData () << endl;
		status = 0;
	}

	else {

		// parse mStringLeft looking for Dye #
		temp = mStringLeft;

		if (temp.FindAndReplaceAllSubstrings ("Dye", "")) {

			channel = temp.ConvertToInteger ();

			if (channel > 0) {

				mStringRight.ToUpper();
				mDyeNames.Replace (channel, mStringRight);
				num = new RGPInt (channel);

				if (!mDyeNumbers.Contains (num)) {
				
					status = 0;
					mDyeNumbers.Append (num);
				}
			}
		}

		else if (temp.FindAndReplaceAllSubstrings ("Color", "")) {

			channel = temp.ConvertToInteger ();

			if (channel > 0) {

				mStringRight.ToUpper ();
				mColorNames.Replace (channel, mStringRight);
				num = new RGPInt (channel);

				if (!mColorNumbers.Contains (num)) {
				
					status = 0;
					mColorNumbers.Append (num);
				}
			}
		}
	}

	if (status == 0)
		mOutputString += mStringLeft + " = " + mStringRight + "\n";

	else
		cout << "Problem with assign string:  " << mStringLeft.GetData () << " = " << mStringRight.GetData () << endl;

	return status;
}


int LadderInputFile :: AssignStringAppend () {

	int status = -1;
//	RGString* newString;

	if (mStringLeft == "LadderFileName") {

		SetEmbeddedSlashesToForward (mStringRight);
		mLadderFileName = mStringRight;
		status = 0;
	}

	//else if (mStringLeft == "LadderDirectory") {

	//	SetEmbeddedSlashesToForward (mStringRight);
	//	mLadderDirectory = mStringRight;
	//	status = 0;
	//}

	else if (mStringLeft == "BinsFileName") {

		SetEmbeddedSlashesToForward (mStringRight);  // This is the full path to the bins file
		mBinsFileName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == "OutputConfigPath") {

		SetEmbeddedSlashesToForward (mStringRight);
		mOutputConfigDirectoryPath = mStringRight;
		AddLocusNamesAndControlNamesFromControlFile (mOutputConfigDirectoryPath + "/ConfigurationTools/StandardPositiveControl/StandardPositiveControlList.txt");
		status = 0;
	}

	else if (mStringLeft == "ILSFamilyName") {

		mILSFamilyName = mStringRight;
		Locus::SetILSFamilyName (mStringRight);
		status = 0;
	}

	else if (mStringLeft == "BinsDelineation") {

		mStringRight.FindAndReplaceAllSubstrings (" ", "");

		if (mStringRight == "tab")
			LadderInputFile::SetBinsDelineation ("\t");

		else
			LadderInputFile::SetBinsDelineation (mStringRight);

		status = 0;
	}

	if (status == 0)
		mOutputString += mStringLeft + " = " + mStringRight + "\n";

	else
		cout << "Assign string did not match expected format:  " << mStringLeft.GetData () << " = " << mStringRight.GetData () << endl;

	return status;
}


void LadderInputFile :: OutputAllData () {

	cout << "Raw input string:" << endl;
	cout << mCumulativeStringWithNewLines.GetData () << endl;
	cout << "\nAssigned input strings:" << endl;
	cout << mOutputString.GetData () << endl;
}


int LadderInputFile :: AssembleInputs () {

	int status = 0;
	RGString labSettings1;
	RGString labSettings2;
	int i;
	
	if (mLadderFileName.Length () == 0) {

		cout << "Ladder File Name is unspecified." << endl;
		status = -1;
	}

	if (mLadderDirectory.Length () == 0) {

		cout << "Ladder information directory is unspecified." << endl;
		status = -1;
	}

	if (mKitName.Length () == 0) {

		cout << "Kit name is unspecified." << endl;
		status = -1;
	}

	if (mNumberOfDyes == 0) {

		cout << "Number of dyes is unspecified." << endl;
		status = -1;
	}

	if (mPanelsFileName.Length () == 0) {

		cout << "Panels file name is unspecified." << endl;
		status = -1;
	}

	if (mBinsFileName.Length () == 0) {

		cout << "Bins file name is unspecified." << endl;
		status = -1;
	}

	if (mSearchString.Length () == 0) {

		cout << "Search string is unspecified." << endl;
		status = -1;
	}

	if (mDyeNames.Length () != mNumberOfDyes + 1) {

		cout << "Number of dyes specified does not match number of dyes input." << endl;
		status = -1;
	}

	if (mColorNames.Length () != mNumberOfDyes + 1) {

		cout << "Number of dyes specified does not match number of colors input." << endl;
		status = -1;
	}

	if (mChannelForILS == 0) {

		mChannelForILS = mNumberOfDyes;
		cout << "ILS channel is unspecified.  Setting ILS channel = last channel." << endl;
	}

	for (i=1; i<=mNumberOfDyes; i++) {

		if (mDyeNames [i].Length () == 0) {

			cout << "Missing dye number " << i << endl;
			status = -1;
		}
	}

	for (i=1; i<=mNumberOfDyes; i++) {

		if (mColorNames [i].Length () == 0) {

			cout << "Missing color number " << i << endl;
			status = -1;
		}
	}

	if (mILSNames.Entries () == 0) {

		cout << "No ILS names specified" << endl;
		status = -1;
	}

	if ((mVersion.ConvertToDouble () > 2.69) && (mILSFamilyName.Length () == 0)) {

		cout << "No ILS Family Name specified for Version 2.7" << endl;
		status = -1;
	}

	if ((mILSNames.Entries () > 1) && mGenerateILSFamilies) {

		cout << "More than one ILS family name specified" << endl;
		status = -1;
	}

	return status;
}


int LadderInputFile :: AssembleInputsAppend () {

	int status = 0;
	
	if (mLadderFileName.Length () == 0) {

		cout << "Ladder File Name is unspecified." << endl;
		status = -1;
	}

	//if (mLadderDirectory.Length () == 0) {

	//	cout << "Ladder information directory is unspecified." << endl;
	//	status = -1;
	//}

	//if (mOutputConfigDirectoryPath.Length () == 0) {

	//	cout << "Output Config Directory Path is unspecified." << endl;
	//	status = -1;
	//}

	if (mBinsFileName.Length () == 0) {

		cout << "Bins file name is unspecified." << endl;
		status = -1;
	}

	if (mILSFamilyName.Length () == 0) {

		cout << "No ILS family name specified" << endl;
		status = -1;
	}

	if (mOutputConfigDirectoryPath.Length () == 0) {

		cout << "Full path user directory unspecified" << endl;
		status = -1;
	}

	//if ((mILSNames.Entries () > 1) && mGenerateILSFamilies) {

	//	cout << "More than one ILS family name specified" << endl;
	//	status = -1;
	//}

	return status;
}



int LadderInputFile :: GetChannelForColorName (const RGString& color) const {

	int i;
	RGString temp = color;
	temp.ToUpper ();

	for (i=1; i<=mNumberOfDyes; i++) {
    RGString s(mColorNames[i]);
		if (temp.IsEqualTo (&s))
			return i;
	}

	return -1;
}


int LadderInputFile :: GetChannelForDyeName (const RGString& dye) const {

	int i;
	RGString temp = dye;
	temp.ToUpper ();

	for (i=1; i<=mNumberOfDyes; i++) {
    RGString s(mColorNames[i]);
		if (temp.IsEqualTo (&s))
			return i;
	}

	return -1;
}


int LadderInputFile :: FindMaxExpectedAlleleOverride (const RGString& locusName) {

	RGDListIterator it (mMaxExpectedAllelesOverrides);
	RGIndexedLabel* nextLabel;

	while (nextLabel = (RGIndexedLabel*) it ()) {

		if (nextLabel->GetLabel () == locusName)
			return (int) nextLabel->GetIndex ();
	}

	return mMaxExpectedAllelesPerLocusDefault;
}


int LadderInputFile :: FindMinExpectedAlleleOverride (const RGString& locusName) {

	RGDListIterator it (mMinExpectedAllelesOverrides);
	RGIndexedLabel* nextLabel;

	while (nextLabel = (RGIndexedLabel*) it ()) {

		if (nextLabel->GetLabel () == locusName)
			return (int) nextLabel->GetIndex ();
	}

	return mMinExpectedAllelesPerLocusDefault;
}


void LadderInputFile :: ResetInputLines () {

	mInputLinesIterator->Reset ();
}


RGString* LadderInputFile :: GetNextInputLine () {

	return (RGString*) (*mInputLinesIterator) ();
}


bool LadderInputFile :: LocusNeedsRelativeHeightInfo (const RGString& locus) {

	if (mRelativeHeightOverrides.Contains (&locus))
		return true;

	return false;
}


void LadderInputFile :: RemoveLeadingAndTrailingBlanks (RGString& string) {

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


void LadderInputFile :: SetEmbeddedSlashesToForward (RGString& string) {

	string.FindAndReplaceAllSubstrings ("\\", "/");

	if (string.GetLastCharacter () == '/')
		string.RemoveLastCharacter ();
}


RGString LadderInputFile :: SplitUsingColon (const RGString& target, RGString& right) { 

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

