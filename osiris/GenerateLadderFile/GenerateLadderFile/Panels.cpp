//
//  Source for reading and parsing panels files
//

#include "Panels.h"
#include "rgindexedlabel.h"

/*

Panels (const RGString& panelFileName);
	~Panels ();

	bool isValid () const;
	Locus* ReadNextLine (LadderInputFile& inputFile);

protected:
	RGString mPanelFileName;
	RGFile* mPanelInputFile;
	bool mFoundFirstLine;
	bool mAtEndOfFile;
	bool mValid;

*/


Panels :: Panels (const RGString& panelFileName) : mPanelFileName (panelFileName), mFoundFirstLine (false), mAtEndOfFile (false), mValid (true),
mLineTokenizer (NULL), mAlleleTokenizer (NULL) {

	mPanelInputFile = new RGFile (mPanelFileName, "rt");

	if (!mPanelInputFile->isValid ()) {

		cout << "Panel input file named:  " << panelFileName.GetData () << " is not valid." << endl;
		delete mPanelInputFile;
		mPanelInputFile = NULL;
		mValid = false;
		return;
	}

	mLineTokenizer = new RGStringTokenizer (mLineString);
	mAlleleTokenizer = new RGStringTokenizer (mAlleleString);
	mLineTokenizer->AddDelimiter ("\t");
	mLineTokenizer->AddRemoveItem ('\"');
	mAlleleTokenizer->AddDelimiter (",");
	mAlleleTokenizer->AddRemoveItem (' ');
	mAlleleTokenizer->AddRemoveItem ('\"');
}



Panels :: ~Panels () {

	if (mPanelInputFile != NULL)
		mPanelInputFile->Close ();

	delete mPanelInputFile;
}


Locus* Panels :: ReadNextLine (LadderInputFile& inputFile) {

	char line [512];
	int status;
	Locus* newLocus;
	RGStringArray lineArrayTokens;
	RGStringArray lineArrayDelimiters;
	RGStringArray alleleArrayTokens;
	RGStringArray alleleArrayDelimiters;
	RGString* testString;
	RGString* foundString;

	if (!mFoundFirstLine) {

		while (true) {

			status = mPanelInputFile->ReadLine (line);

			if (!status) {

				mAtEndOfFile = true;
				return NULL;
			}

			mLineString = line;
			mLineTokenizer->ResetTokenizer ();
			lineArrayTokens.ResetArray ();
			lineArrayDelimiters.ResetArray ();
			mLineTokenizer->Split (lineArrayTokens, lineArrayDelimiters);

			if (lineArrayTokens.Length () < 7) {

				lineArrayTokens.ResetArray ();
				lineArrayDelimiters.ResetArray ();
				continue;
			}

			mFoundFirstLine = true;
			break;
		}
	}

	else if (mAtEndOfFile)
		return NULL;

	else {

		status = mPanelInputFile->ReadLine (line);

		if (!status) {

			mAtEndOfFile = true;
			return NULL;
		}

		mLineString = line;
		mLineTokenizer->ResetTokenizer ();
		mLineTokenizer->Split (lineArrayTokens, lineArrayDelimiters);

		if (lineArrayTokens.Length () < 7) {

			mAtEndOfFile = true;
			return NULL;
		}
	}

	RGString locusName = lineArrayTokens [0];
	//cout << "Panels locus name = " << locusName.GetData () << endl;
	RGString color = lineArrayTokens [1];
	//color.ToUpper ();
	int length = lineArrayTokens.Length ();

	// there could be an extra comma at the end of line so we have to account for a possible extra token:

	if (lineArrayTokens [length - 1].Length () == 0)
		length--;

	int coreRepeat = lineArrayTokens [length - 3].ConvertToInteger ();
	mAlleleString = lineArrayTokens [length - 1];
	
	int channel = inputFile.GetChannelForColorName (color);
	Allele* newAllele;

	if (locusName == "AMEL") {

		newLocus = new Locus (locusName, channel, 6);
		newAllele = new Allele ("1", 5, 0);
		newLocus->AddAllele (newAllele);
		newAllele = new Allele ("2", 10, 0);
		newLocus->AddAllele (newAllele);
		newLocus->SetYLinked (false);
		return newLocus;
	}

	newLocus = new Locus (locusName, channel, coreRepeat);
	//cout << "New panels locus with name, channel, core repeat = " << locusName.GetData () << ", " << channel << ", " << coreRepeat << endl;
	//cout << "Allele string:  " << mAlleleString.GetData () << endl;
	mAlleleTokenizer->ResetTokenizer ();
	mAlleleTokenizer->Split (alleleArrayTokens, alleleArrayDelimiters);
	int aLength = alleleArrayTokens.Length ();

	// there could be an extra comma at the end of line so we have to account for a possible extra token:

	if (alleleArrayTokens [aLength - 1].Length () == 0)
		aLength--;

	int i;
	
	int curveNo = 5;
	int bp = 0;
	//cout << "Allele values:  ";

	for (i=0; i<aLength; i++) {

		curveNo += 5;
		newAllele = new Allele (alleleArrayTokens [i], curveNo, bp);
		//cout << alleleArrayTokens [i].GetData () << ";";
		newLocus->AddAllele (newAllele);
	}

	//  Test for YLinked status

	testString = new RGString (locusName);
	RGDList YList = inputFile.YLinkedOverrides ();
	foundString = (RGString*)YList.Find (testString);
	bool yLinkedDefault = inputFile.GetYLinkedDefault ();

	if (foundString == NULL) {

		// use default
		newLocus->SetYLinked (yLinkedDefault);
	}

	else
		newLocus->SetYLinked (!yLinkedDefault);

	delete testString;

	//  Test for max and min expected alleles overrides

	newLocus->SetMaxExpectedAlleles (inputFile.FindMaxExpectedAlleleOverride (locusName));
	newLocus->SetMinExpectedAlleles (inputFile.FindMinExpectedAlleleOverride (locusName));

	return newLocus;
}