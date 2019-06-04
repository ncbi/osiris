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

	PanelsNumberOfLinesSkipped = LadderInputFile::GetNumberOfPanelsLinesSkipped ();
	ColorColumn = LadderInputFile::GetColorColumn () - 1;
	RepeatSizeColumn = LadderInputFile::GetRepeatSizeColumn () - 1;
	AlleleListColumn = LadderInputFile::GetAlleleListColumn () - 1;
	AlleleListDelineation = LadderInputFile::GetAlleleListDelineation ();
	ColumnDelineation = LadderInputFile::GetColumnDelineation ();

	mLineTokenizer = new RGStringTokenizer (mLineString);
	mAlleleTokenizer = new RGStringTokenizer (mAlleleString);
	mLineTokenizer->AddDelimiter (ColumnDelineation);
	mLineTokenizer->AddRemoveItem ('\"');
	mAlleleTokenizer->AddDelimiter (AlleleListDelineation);
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
	RGStringArray lineArrayTokens (250);
	RGStringArray lineArrayDelimiters (250);
	RGStringArray alleleArrayTokens (250);
	RGStringArray alleleArrayDelimiters (250);
	RGString* testString;
	RGString* foundString;
//	size_t position;

	if (!mFoundFirstLine) {

		int i;

		for (i=1; i<=PanelsNumberOfLinesSkipped; i++)
			status = mPanelInputFile->ReadLine (line);
	}

	if (!mFoundFirstLine) {

		while (true) {

			status = mPanelInputFile->ReadLine (line);

			if (!status) {

				mAtEndOfFile = true;
				return NULL;
			}

			mLineString = line;

			/*if (mLineString.FindSubstring ("Version", position))
				continue;

			else if (mLineString.FindSubstring ("Kit type", position))
				continue;

			else if (mLineString.FindSubstring ("Chemistry Kit", position))
				continue; 

			else if (mLineString.FindSubstring ("Panel", position))
				continue; */

			mLineTokenizer->ResetTokenizer ();
			lineArrayTokens.ResetArray ();
			lineArrayDelimiters.ResetArray ();
			mLineTokenizer->Split (lineArrayTokens, lineArrayDelimiters);

			if (lineArrayTokens.Length () < 7) {

				mAtEndOfFile = true;
				return NULL;
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
	RGString color = lineArrayTokens [ColorColumn];
	//color.ToUpper ();
	int length = lineArrayTokens.Length ();

	// there could be an extra comma at the end of line so we have to account for a possible extra token:

	if (lineArrayTokens [length - 1].Length () == 0)
		length--;

	int coreRepeat = lineArrayTokens [RepeatSizeColumn].ConvertToInteger ();
	mAlleleString = lineArrayTokens [AlleleListColumn];
	
	int channel = inputFile.GetChannelForColorName (color);
	Allele* newAllele;

	//if (locusName == "AMEL") {

	//	newLocus = new Locus (locusName, channel, 6);
	//	newAllele = new Allele ("1", 5, 0);
	//	newLocus->AddAllele (newAllele);
	//	newAllele = new Allele ("2", 10, 0);
	//	newLocus->AddAllele (newAllele);
	//	newLocus->SetYLinked (false);
	//	cout << "Locus = AMEL with Core Repeat = 6 on Channel = " << channel << endl;
	//	return newLocus;
	//}

	newLocus = new Locus (locusName, channel, coreRepeat);
	cout << "Locus = " << locusName.GetData () << " with Core Repeat = " << coreRepeat << " on Channel = " << channel << endl;
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

	if (aLength == 2) {

			curveNo += 5;
			newAllele = new Allele ("1", curveNo, bp);
			newLocus->AddAllele (newAllele);
			curveNo += 5;
			newAllele = new Allele ("2", curveNo, bp);
			newLocus->AddAllele (newAllele);
	}

	else {

		for (i=0; i<aLength; i++) {

			curveNo += 5;
			newAllele = new Allele (alleleArrayTokens [i], curveNo, bp);
			//cout << alleleArrayTokens [i].GetData () << ";";
			newLocus->AddAllele (newAllele);
		}
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