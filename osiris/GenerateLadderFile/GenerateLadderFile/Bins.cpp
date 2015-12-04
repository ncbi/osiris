//
//	Source file for reading and parsing bins file
//

#include "Bins.h"
#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;


Bins :: Bins (const RGString& binsFileName) : mBinsFileName (binsFileName), mIsValid (true), mNumberOfLoci (0) {

	mBinsInputFile = new RGFile (binsFileName, "rt");

	if (!mBinsInputFile->isValid ())
		mIsValid = false;

	else {

		mBinsText.ReadTextFile (*mBinsInputFile);

		if (mBinsText.GetLastCharacter () == '\n')
			mBinsText += "Marker Name";

		else
			mBinsText += "\nMarker Name";
	}
}


Bins :: ~Bins () {

	mLoci.ClearAndDelete ();
	mBinsInputFile->Close ();
	delete mBinsInputFile;
}


/*
	RGString mBinsFileName;
	RGFile* mBinsInputFile;
	RGString mBinsText;
	bool mIsValid;
	RGDList mLoci;
	*/

int Bins :: SplitFileIntoLocusStrings () {

	RGStringTokenizer fileSplit (mBinsText);
	fileSplit.AddDelimiter ("Marker Name");
	RGStringArray delimiters;

	mLocusStrings.ResetArray ();
	fileSplit.Split (mLocusStrings, delimiters);
	mNumberOfLoci = mLocusStrings.Length () - 1;  // subtract off header info

	cout << "Number of loci found in bins file = " << mNumberOfLoci << endl;

	if (delimiters.Length () != mNumberOfLoci + 1) {

		cout << "Possible mismatch in number of loci found in bins file, number of delimiters = " << delimiters.Length () << endl;
		return -mNumberOfLoci;
	}

	return mNumberOfLoci;
}


void Bins :: SplitLocusStringIntoLines (int ithLocus) {

	RGStringTokenizer locusSplit (mLocusStrings [ithLocus]);
	locusSplit.AddDelimiter ('\n');
	RGStringArray delimiters;

	mLocusLineStrings.ResetArray ();
	locusSplit.Split (mLocusLineStrings, delimiters);
}


void Bins :: SplitLocusLine (RGString& lineString) {

	mLocusLineFieldString.ResetArray ();
	RGStringTokenizer lineSplit (lineString);
	lineSplit.AddDelimiter ('\t');
	RGStringArray delimiters;

	lineSplit.Split (mLocusLineFieldString, delimiters);

	//unsigned long i;
	//cout << "Locus line length = " << mLocusLineFieldString.Length () << endl;
	//cout << "Locus line:  ";

	//for (i=0; i<mLocusLineFieldString.Length (); i++) {

	//	cout << mLocusLineFieldString [i].GetData ();

	//	if (i < mLocusLineFieldString.Length () - 1)
	//		cout << ";";
	//}

	//cout << endl;
}


Locus* Bins :: AssembleLocusFromLineStrings () {
	
	// uses current values in mLocusLineStrings

	Locus* newLocus = new Locus ();
	int nLines = mLocusLineStrings.Length ();
	SplitLocusLine (mLocusLineStrings [0]);  // This should give us the name field
	RGString name = mLocusLineFieldString [1];
	cout << "Locus name = " << name.GetData () << endl;

	//cout << "Bins locus line 1:  ";
	int i;
	int len = mLocusLineFieldString.Length ();

	//for (i=0; i<len; i++) {

	//	cout << mLocusLineFieldString [i].GetData ();

	//	if (i < len - 1)
	//		cout << ", ";
	//}

	//cout << endl;

	RGString firstAlleleName;
	int firstLocusBP;
	double firstILSBP;
	double lastILSBP;
	RGString firstExtendedAlleleName;
	RGString lastExtendedAlleleName;
	int lineForFirstAllele = 0;

	for (i=1; i<nLines; i++) {

		SplitLocusLine (mLocusLineStrings [i]);

		if (mLocusLineFieldString.Length () < 4) {

			cout << "Could not find first allele for locus " << name.GetData () << endl;
			delete newLocus;
			return NULL;
		}

		if (mLocusLineFieldString.Length () >= 5) {

			if (mLocusLineFieldString [4] == "virtual") {

				if (i == 1) {

					firstExtendedAlleleName = mLocusLineFieldString [0];
				}

				continue;
			}
		}

		firstAlleleName = mLocusLineFieldString [0];
		firstILSBP = mLocusLineFieldString [1].ConvertToDouble ();
		firstLocusBP = (int) floor (firstILSBP + 0.5);
		lineForFirstAllele = i;
		break;
	}

	if (firstExtendedAlleleName.Length () == 0)
		firstExtendedAlleleName = firstAlleleName;

	bool foundLastExtendedAllele = false;

	for (i=nLines-1; i>lineForFirstAllele; i--) {
	
		SplitLocusLine (mLocusLineStrings [i]);

		if (mLocusLineFieldString.Length () < 4)
			continue;

		if (!foundLastExtendedAllele) {
		
			lastExtendedAlleleName = mLocusLineFieldString [0];
			foundLastExtendedAllele = true;
		}

		if ((mLocusLineFieldString.Length () >= 5) && (mLocusLineFieldString [4] == "virtual"))
			continue;

		lastILSBP = mLocusLineFieldString [1].ConvertToDouble ();
		break;
	}

	if (lastExtendedAlleleName.Length () == 0) {

		cout << "Could not locate last extended allele name for locus:  " << name.GetData () << endl;
		delete newLocus;
		return NULL;
	}

	newLocus->SetFirstCoreAllele (firstAlleleName);
	newLocus->SetFirstExtendedAllele (firstExtendedAlleleName);
	newLocus->SetLastExtendedAllele (lastExtendedAlleleName);
	newLocus->SetMinMaxSearchILSBP (firstILSBP, lastILSBP);  // These will have to be adjusted later when core repeat number is known
	newLocus->SetFirstCoreLocusBP (firstLocusBP);
	newLocus->SetName (name);

	//  Need to add to Locus ability to compute first and last extended bp

	return newLocus;
}


Ladder* Bins :: AssembleAllLoci (RGDList& doNotExtend) {

	int nLoci;
	Ladder* newLadder = new Ladder ();
	Locus* nextLocus;
	nLoci = SplitFileIntoLocusStrings ();
	int i;
	int status;
	RGString name;

	if (nLoci <= 0) {

		cout << "Could not complete ladder setup" << endl;
		delete newLadder;
		return NULL;
	}

	for (i=1; i<=nLoci; i++) {

		SplitLocusStringIntoLines (i);
		nextLocus = AssembleLocusFromLineStrings ();
		status = newLadder->AddLocus (nextLocus);

		if (status < 0) {

			cout << "Locus name repeated in bins file:  " << nextLocus->GetName ().GetData () << endl;
			delete newLadder;
			return NULL;
		}

		name = nextLocus->GetName ();

		if (doNotExtend.Contains (&name))
			nextLocus->SetDoNotExtend ();
	}

	return newLadder;
}


/*

RGStringArray mLocusStrings;
	RGStringArray mLocusLineStrings;
	RGStringArray mLocusLineFieldString;

*/