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
*  FileName: ILSInputData.cpp
*  Author:   Robert Goor
*
*/
//
// ILSInputData is class to input basic settings that determine location and identity of ILS file.
// Assumes each line ends with a ";" and last line has nothing on it but a ";"
//

#include "ILSInputData.h"
#include "rgpscalar.h"
#include "rgindexedlabel.h"
#include "rgstring.h"
#include <iostream>
#include <list>

using namespace std;




ILSInputData::ILSInputData (bool debug) : mDebug (debug), mInputFile (NULL), mIsValid (true), mNumberOfPeaks (0), mNumberOfRelativeHeights (0), 
mPeakSizeList (NULL), mNumberOfInitialPeaksToIgnore (3), mNumberOfFinalPeaksToIgnore (3), mCorrelation1 ("0.993"), mCorrelation2 ("0.9975") {

	mInputLinesIterator = new RGDListIterator (mInputLines);
}



ILSInputData :: ~ILSInputData () {

	if (!(mInputFile == NULL)) {

		mInputFile->Close ();
		delete mInputFile;
		mInputFile = NULL;
	}

	delete mInputLinesIterator;
	mInputLines.ClearAndDelete ();

	if (mPeakSizeList != NULL)
		delete[] mPeakSizeList;

	mPeakSizeList = NULL;
}



int ILSInputData::ReadAllInputs (const RGString& inputFileName) {

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





int ILSInputData::ReadAllInputs () {

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

int ILSInputData::ReadLine () {

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


int ILSInputData::AssignString () {

	int status = -1;
	RGString temp;
	RGString value;
	int i;
	RGString tempName;

	RGString FamilyNameTag = "ILSFamilyName"; // required
	RGString BaseLaneStdTag = "BaseLaneStandardName"; // required
	RGString PeakSizeListTag = "PeakList";  // required
	RGString RelativeHeightListTag = "RelativeHeightList";  // optional
	RGString InitialPeaksToIgnoreTag = "NumInitialPeaksIgnored";  // default = 3
	RGString FinalPeaksToIgnoreTag = "NumFinalPeaksIgnored";  // default = 3;
	RGString DyeNameTag = "DyeName";  // required, but can be reset in ladder file
	RGString CorrelationAccept1 = "CorrelationAcceptance1";  // default = 0.993
	RGString CorrelationAccept2 = "CorrelationAcceptance2";  // default = 0.9975
	RGString OutputFolderTag = "OutputFolder";  //  optional:  default = ../Config/LadderSpecifications

	RGStringTokenizer parse (mStringRight);
	parse.AddDelimiter (" ");
	parse.AddDelimiter ("\t");
	
	RGStringArray delimiters;
	RGString Ans;
	//right = "";

	if (mStringLeft == FamilyNameTag) {

		mFamilyName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == BaseLaneStdTag) {

		mBaseLaneStdName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == PeakSizeListTag) {

		parse.ResetTokenizer ();
		RGStringArray tokens;
		parse.Split (tokens, delimiters);
		mNumberOfPeaks = tokens.Length ();
		list<int> sizes;
		int temp;
		bool nonPositivePeakSize = false;

		for (i=0; i<mNumberOfPeaks; i++) {

			temp = tokens [i].ConvertToInteger ();
			sizes.push_back (temp);

			if (temp <= 0) {

				cout << "Peak number " << i << " has a value that is less than or equal to 0.\n";
				nonPositivePeakSize = true;
			}
		}

		if (!nonPositivePeakSize) {

			mPeakSizeList = new int [mNumberOfPeaks];

			for (i=0; i<mNumberOfPeaks; i++) {

				temp = sizes.front ();
				mPeakSizeList [i] = temp;
				sizes.pop_front ();
			}

			for (i=1; i<mNumberOfPeaks; i++) {

				if (mPeakSizeList [i] <= mPeakSizeList [i-1]) {

					cout << "PeakSize " << i-1 << " or " << i << " are out of order\n";
					nonPositivePeakSize = true;
				}
			}

			if (!nonPositivePeakSize)
				status = 0;
		}
	}

	else if (mStringLeft == RelativeHeightListTag) {

		parse.ResetTokenizer ();
		parse.Split (mRelativeHeightList, delimiters);
		mNumberOfRelativeHeights = mRelativeHeightList.Length ();
		status = 0;
	}

	else if (mStringLeft == InitialPeaksToIgnoreTag) {

		mNumberOfInitialPeaksToIgnore = mStringRight.ConvertToInteger ();
		status = 0;
	}

	else if (mStringLeft == FinalPeaksToIgnoreTag) {

		mNumberOfFinalPeaksToIgnore = mStringRight.ConvertToInteger ();
		status = 0;
	}

	else if (mStringLeft == DyeNameTag) {

		mDyeName = mStringRight;
		status = 0;
	}

	else if (mStringLeft == CorrelationAccept1) {

		double temp = mStringRight.ConvertToDouble ();

		if ((temp > 0.0) && (temp < 1.0)) {

			mCorrelation1 = mStringRight;
			status = 0;
		}

		else {

			cout << "Correlations must be between 0 and 1.  Input correlation = " << mStringRight << endl;
		}		
	}

	else if (mStringLeft == CorrelationAccept2) {

		double temp = mStringRight.ConvertToDouble ();

		if ((temp > 0.0) && (temp < 1.0)) {

			mCorrelation2 = mStringRight;
			status = 0;
		}

		else {

			cout << "Correlations must be between 0 and 1.  Input correlation = " << mStringRight << endl;
		}
	}

	else if (mStringLeft == OutputFolderTag) {

			SetEmbeddedSlashesToForward (mStringRight);
			mOutputFolder = mStringRight;
			status = 0;
	}

	if (status == 0)
		mOutputString += mStringLeft + " = " + mStringRight + "\n";

	else
		cout << "Problem with assign string:  " << mStringLeft.GetData () << " = " << mStringRight.GetData () << endl;

	return status;
}


void ILSInputData::OutputAllData () {

	cout << "Raw input string:" << endl;
	cout << mCumulativeStringWithNewLines.GetData () << endl;
	cout << "\nAssigned input strings:" << endl;
	cout << mOutputString.GetData () << endl;
}


int ILSInputData::AssembleInputs () {

	int status = 0;

	if (mNumberOfPeaks == 0) {

		cout << "The list of ILS peak sizes is missing" << endl;
		status = -1;
	}

	if ((mNumberOfRelativeHeights > 0) && (mNumberOfPeaks != mNumberOfRelativeHeights)) {

		cout << "The number of relative heights is not equal to the number of peaks.  Number of peaks = " << mNumberOfPeaks << ".  Number of relative heights = " << mNumberOfRelativeHeights << "." << endl;
		status = -1;
	}

	if (mFamilyName.Length () == 0) {

		cout << "The ILS family name has not been specified." << endl;
		status = -1;
	}

	if (mBaseLaneStdName.Length () == 0) {

		cout << "The base lane standard name has not been specified." << endl;
		status = -1;
	}

	if (mDyeName.Length () == 0) {

		cout << "The dye name has not been specified." << endl;
		status = -1;
	}

	if (mNumberOfInitialPeaksToIgnore < 0)
		mNumberOfInitialPeaksToIgnore = 0;

	if (mNumberOfFinalPeaksToIgnore < 0)
		mNumberOfFinalPeaksToIgnore = 0;

	RGString relativeHeight;
	int i;

	for (i=0; i<mNumberOfPeaks; i++) {

			relativeHeight = mRelativeHeightList [i];

			if (!((relativeHeight == "L") || (relativeHeight == "ML") || (relativeHeight == "M") || (relativeHeight == "MH") || (relativeHeight == "H"))) {

				cout << "Relative height " << i << " is:  " << relativeHeight << ".  Must be one of 'L, ML, M, MH, H'\n";
				status = -1;
			}
	}

	return status;
}


//RGString* ILSInputData::GetNextInputLine () {
//
//	return (RGString*)(*mInputLinesIterator) ();
//}


void ILSInputData::RemoveLeadingAndTrailingBlanks (RGString& string) {

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


void ILSInputData::SetEmbeddedSlashesToForward (RGString& string) {

	string.FindAndReplaceAllSubstrings ("\\", "/");

	if (string.GetLastCharacter () == '/')
		string.RemoveLastCharacter ();
}


RGString ILSInputData::SplitUsingColon (const RGString& target, RGString& right) {

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


RGString ILSInputData::CreateXMLString () {

	RGString finalXML;

	RGString tab2 = "\t\t";
	RGString tab3 = "\t\t\t";
	RGString tab4 = "\t\t\t\t";
	RGString ils = "\t\t<ILS>\n";
	RGString endILS = "\t\t</ILS>\n";
	RGString laneStandard = "<LaneStandard>\n";
	RGString endLaneStandard = "</LaneStandard>\n";
	RGString endLaneStandards = "\t</LaneStandards>\n";
	RGString kitData = "<KitData>\n";
	RGString endKitData = "</KitData>\n";
	RGString Version = "\t<Version>2.7</Version>\n";
	RGString laneStandards = "\t<LaneStandards>\n";

	RGString correlations;
	correlations << tab4 << "<CorrelationAcceptanceThreshold>" << mCorrelation1 << "</CorrelationAcceptanceThreshold>\n";
	correlations << tab4 << "<CorrelationAutoAcceptanceThreshold>" << mCorrelation2 << "</CorrelationAutoAcceptanceThreshold>\n";

	finalXML << kitData;
	finalXML << Version;
	finalXML << laneStandards;
	finalXML << ils;
	finalXML << tab3 << "<ILSName>" << mFamilyName << "</ILSName>\n";
	finalXML << tab3 << "<DyeName>" << mDyeName << "</DyeName>\n";
	
	RGString characteristics;
	RGString relativeHeights;
	RGString nameString;
	RGString displayNameString;
	RGString rangeString;
	int firstSize;
	int lastSize;

	bool hasRelativeHeights = (mNumberOfRelativeHeights > 0);

	int i;
	int j;
	int k;
	
	for (i=0; i<=mNumberOfInitialPeaksToIgnore; i++) {

		for (j=0; j<=mNumberOfFinalPeaksToIgnore; j++) {

			characteristics = tab4;
			relativeHeights = tab4;
			nameString = tab4;
			displayNameString = tab4;
			nameString << "<Name>" << mBaseLaneStdName;
			displayNameString << "<DisplayName>";
			characteristics << "<Characteristics>";
			relativeHeights << "<RelativeHeights>";

			firstSize = mPeakSizeList [i];
			lastSize = mPeakSizeList [mNumberOfPeaks - 1 - j];
			int endIndex = mNumberOfPeaks - j;

			for (k=i; k<endIndex; k++) {

				if (k == i) {

					characteristics << mPeakSizeList [k];

					if (hasRelativeHeights)
						relativeHeights << mRelativeHeightList [k];
				}

				else {

					characteristics << " " << mPeakSizeList [k];

					if (hasRelativeHeights)
						relativeHeights << " " << mRelativeHeightList [k];
				}
			}

			rangeString ="";
			rangeString << firstSize << "-" << lastSize;
			nameString << "-" << rangeString << "</Name>\n";
			displayNameString << rangeString << "</DisplayName>\n";
			characteristics << "</Characteristics>\n";
			relativeHeights << "</RelativeHeights>\n";

			finalXML << tab3 << laneStandard;
			finalXML << nameString;
			finalXML << characteristics;

			if (hasRelativeHeights)
				finalXML << relativeHeights;

			finalXML << correlations;
			finalXML << tab3 << endLaneStandard;
		}
	}

	finalXML << endILS;
	finalXML << endLaneStandards;
	finalXML << endKitData;

	return finalXML;
}

