//
//  Source code for Allele, Locus and Ladder that uses data input from bins and panels file and creates ladder info file
//


#include "LadderStructures.h"

bool Locus :: GenerateILSFamilies = false;
RGString Locus :: ILSName;


int CopyVolumeFile (const RGString& inputFileName, const RGString& outputFileName) {

	RGFile inputFile (inputFileName, "rt");

	if (!inputFile.isValid ()) {

		cout << "Could not read " << inputFileName.GetData () << " for default volume...exiting" << endl;
		return -1;
	}

	RGString inputFileString;
	inputFileString.ReadTextFile (inputFile);

	RGTextOutput outputFile (outputFileName, FALSE);

	if (!outputFile.FileIsValid ()) {

		cout << "Could not create " << outputFileName.GetData () << "...exiting" << endl;
		return -1;
	}

	outputFile << inputFileString;
	return 0;
}



STRAlleleName :: STRAlleleName () : mRepeats (0), mMicroVariant (0) {

}



STRAlleleName :: STRAlleleName (const RGString& name) {

	size_t position;
	RGString repeats;
	RGString variant;
	size_t len;

	if (name.FindNextSubstring (0, ".", position)) {

		repeats = name.ExtractSubstring (0, position - 1);
		mRepeats = repeats.ConvertToInteger ();
		len = name.Length () - 1;

		if (position <= len) {

			variant = name.ExtractSubstring (position + 1, len);
			mMicroVariant = variant.ConvertToInteger ();
		}

		else
			mMicroVariant = 0;
	}

	else {

		mRepeats = name.ConvertToInteger ();
		mMicroVariant = 0;
	}
}



STRAlleleName :: ~STRAlleleName () {

}


int STRAlleleName :: GetBPDifferenceFrom (STRAlleleName& strAllele, int coreRepeatNo) {

	//
	// Assumes that strAllele is lower than this allele
	//

	return (coreRepeatNo * (mRepeats - strAllele.mRepeats) + (mMicroVariant - strAllele.mMicroVariant));
}




Allele :: Allele () : mCurveNumber (0), mBP (0), mIsVirtual (false) {

}



Allele :: Allele (const RGString& name, int curveNumber, int bp) : mName (name), mCurveNumber (curveNumber), mBP (bp), mIsVirtual (false) {

}


Allele :: ~Allele () {

}


void Allele :: OutputTo (RGTextOutput& xmlFile) {

	RGString blank10 = "          ";
	RGString blank12 = "            ";

	xmlFile << blank10 << "<Allele>\n";
	xmlFile << blank12 << "<Name>" << mName.GetData () << "</Name>\n";
	xmlFile << blank12 << "<CurveNo>" << mCurveNumber << "</CurveNo>\n";
	xmlFile << blank12 << "<BP>" << mBP << "</BP>\n";

	if (mRelativeHeight.Length () > 0)
		xmlFile << blank12 << "<RelativeHeight>" << mRelativeHeight.GetData () << "</RelativeHeight>\n";

	xmlFile << blank10 << "</Allele>\n";
}



bool Allele :: isEqual (Allele* target) {

	if (mName.IsEqualTo (&(target->mName)) == 0)
		return false;

	return true;
}



Locus :: Locus () : mChannel (0), mCoreRepeat (0), mMinLocusBP (0), mMaxLocusBP (0), mMinSearchILSBP (0.0), mMaxSearchILSBP (0.0), mYLinked (false),
mMinExpectedAlleles (1), mMaxExpectedAlleles (2), mIsMerged (false), mOriginalMinSearchILSBP (0.0), mOriginalMaxSearchILSBP (0.0), mDoNotExtend (false), 
mNeedsRelativeHeightInfo (false), mIsQualityLocus (false) {

}


Locus :: Locus (const RGString& name, int channel, int coreRepeat) : mName (name), mChannel (channel), mCoreRepeat (coreRepeat), mMinLocusBP (0), mMaxLocusBP (0), 
	mMinSearchILSBP (0.0), mMaxSearchILSBP (0.0), mYLinked (false), mMinExpectedAlleles (1), mMaxExpectedAlleles (2), mIsMerged (false), 
	mOriginalMinSearchILSBP (0.0), mOriginalMaxSearchILSBP (0.0), mDoNotExtend (false), mNeedsRelativeHeightInfo (false), mIsQualityLocus (false) {

}


Locus :: ~Locus () {

	list <Allele*>::const_iterator AIterator;
	Allele* nextAllele;

	for (AIterator = mAlleleList.begin(); AIterator != mAlleleList.end(); AIterator++) {

		nextAllele = *AIterator;
		delete nextAllele;
	}
	
	mAlleleList.clear ();
}


int Locus :: AddAllele (Allele* newAllele) {
	
	// returns -1 if identical to existing allele

	list <Allele*>::const_iterator AIterator;
	Allele* nextAllele;
	int status = 0;

	for (AIterator = mAlleleList.begin(); AIterator != mAlleleList.end(); AIterator++) {

		nextAllele = *AIterator;

		if (nextAllele->isEqual (newAllele)) {

			status = -1;
			break;
		}
	}

	if (status == 0)
		mAlleleList.push_back (newAllele);

	return status;
}


void Locus :: SetMinMaxSearchILSBP (double min, double max) {

	mMinSearchILSBP = min; 
	mMaxSearchILSBP = max;

	if (!mIsMerged) {

		mOriginalMinSearchILSBP = min; 
		mOriginalMaxSearchILSBP = max;
	}
}
	

bool Locus :: isEqual (Locus* locus) {

	if (mName.IsEqualTo (&(locus->mName)) == 0)
		return false;

	return true;
}


void Locus :: AdjustSearchRegion () {

	int correction = mCoreRepeat - 1;
	mMinSearchILSBP -= correction;
	mMaxSearchILSBP += correction;
}



void Locus :: OutputTo (RGTextOutput& xmlFile) {

	RGString blank6 = "      ";
	RGString blank8 = "        ";
	RGString blank10 = "          ";
	RGString blank12 = "            ";

	xmlFile << blank6 << "<Locus>\n";
	xmlFile << blank8 << "<Name>" << mName.GetData () << "</Name>\n";
	xmlFile << blank8 << "<Channel>" << mChannel << "</Channel>\n";

	if (mDoNotExtend)
		xmlFile << blank8 << "<NoExtension>true</NoExtension>\n";

	xmlFile << blank8 << "<MinBP>" << mMinLocusBP << "</MinBP>\n";
	xmlFile << blank8 << "<MaxBP>" << mMaxLocusBP << "</MaxBP>\n";

	if (GetGenerateILSFamilies ()) {

		xmlFile << blank8 << "<SearchRegions>\n";
		xmlFile << blank10 << "<Region>\n";
		xmlFile << blank12 << "<ILSName>" << GetILSName () << "</ILSName>\n";
		xmlFile << blank12 << "<MinGrid>" << 0.01 * floor (100.0 * mMinSearchILSBP + 0.5) << "</MinGrid>\n";
		xmlFile << blank12 << "<MaxGrid>" << 0.01 * floor (100.0 * mMaxSearchILSBP + 0.5) << "</MaxGrid>\n";
		xmlFile << blank10 << "</Region>\n";
		xmlFile << blank8 << "</SearchRegions>\n";
	}

	else {

		xmlFile << blank8 << "<MinGridLSBasePair>" << 0.01 * floor (100.0 * mMinSearchILSBP + 0.5) << "</MinGridLSBasePair>\n";
		xmlFile << blank8 << "<MaxGridLSBasePair>" << 0.01 * floor (100.0 * mMaxSearchILSBP + 0.5) << "</MaxGridLSBasePair>\n";
	}

	if (mCoreRepeat != 4)
		xmlFile << blank8 << "<CoreRepeatNumber>" << mCoreRepeat << "</CoreRepeatNumber>\n";

	if (mYLinked)
		xmlFile << blank8 << "<YLinked>true</YLinked>\n";

	if (mIsQualityLocus)
		xmlFile << blank8 << "<QualityLocus>true</QualityLocus>\n";

	if (mMaxExpectedAlleles != 2)
		xmlFile << blank8 << "<MaxExpectedAlleles>" << mMaxExpectedAlleles << "</MaxExpectedAlleles>\n";

	if (mMinExpectedAlleles != 1)
		xmlFile << blank8 << "<MinExpectedAlleles>" << mMinExpectedAlleles << "</MinExpectedAlleles>\n";

	xmlFile << blank8 << "<LadderAlleles>\n";

	list <Allele*>::const_iterator AIterator;
	Allele* nextAllele;

	for (AIterator = mAlleleList.begin(); AIterator != mAlleleList.end(); AIterator++) {

		nextAllele = *AIterator;

		if (mNeedsRelativeHeightInfo)
			nextAllele->SetRelativeHeight ("H");

		nextAllele->OutputTo (xmlFile);
	}

	xmlFile << blank8 << "</LadderAlleles>\n";
	xmlFile << blank6 << "</Locus>\n";
}


int Locus :: ComputeAllBPs () {

	list <Allele*>::const_iterator AIterator;
	Allele* nextAllele = mAlleleList.front ();
	STRAlleleName firstCore (nextAllele->GetName ());
	STRAlleleName* nextRep;
	int bpDisp;

	for (AIterator = mAlleleList.begin(); AIterator != mAlleleList.end(); AIterator++) {

		nextAllele = *AIterator;
		nextRep = new STRAlleleName (nextAllele->GetName ());
		bpDisp = nextRep->GetBPDifferenceFrom (firstCore, mCoreRepeat);
		nextAllele->SetBP (bpDisp + mFirstCoreLocusBP);
		delete nextRep;
	}

	nextRep = new STRAlleleName (mLastExtendedAllele);
	bpDisp = nextRep->GetBPDifferenceFrom (firstCore, mCoreRepeat);
	mMaxLocusBP = bpDisp + mFirstCoreLocusBP;
	delete nextRep;
	nextRep = new STRAlleleName (mFirstExtendedAllele);
	bpDisp = firstCore.GetBPDifferenceFrom (*nextRep, mCoreRepeat);
	mMinLocusBP = mFirstCoreLocusBP - bpDisp;
	delete nextRep;
	return 0;
}



Ladder :: Ladder () : mNumberOfChannels (0), mDefaultYLinked (false), mDefaultMinExpectedAllelesPerLocus (1), mDefaultMaxExpectedAllelesPerLocus (2) {

}



Ladder :: Ladder (const RGString& markerSetName) : mMarkerSetName (markerSetName), mNumberOfChannels (0), mDefaultYLinked (false), mDefaultMinExpectedAllelesPerLocus (1), 
	mDefaultMaxExpectedAllelesPerLocus (2) {

}


Ladder :: ~Ladder () {

	list <Locus*>::const_iterator locusIterator;
	Locus* nextLocus;

	for (locusIterator = mLocusList.begin(); locusIterator != mLocusList.end(); locusIterator++) {

		nextLocus = *locusIterator;
		delete nextLocus;
	}
	
	mLocusList.clear ();
	mILSList.ClearAndDelete ();
}



int Ladder :: AddLocus (Locus* newLocus) {

	list <Locus*>::const_iterator locusIterator;
	Locus* nextLocus;
	int status = 0;

	for (locusIterator = mLocusList.begin(); locusIterator != mLocusList.end(); locusIterator++) {

		nextLocus = *locusIterator;

		if (nextLocus->isEqual (newLocus)) {

			status = -1;
			break;
		}
	}

	if (status == 0)
		mLocusList.push_back (newLocus);

	return status;
}


Locus* Ladder :: FindLocusByName (const RGString& locusName) {

	Locus* nextLocus;
	list <Locus*>::const_iterator locusIterator;

	for (locusIterator = mLocusList.begin(); locusIterator != mLocusList.end(); locusIterator++) {

		nextLocus = *locusIterator;

		if (nextLocus->GetName () == locusName)
			return nextLocus;
	}

	return NULL;
}


int Ladder :: MergeLocusIntoLadder (const Locus* locus) {
	
	// This combines info and then computes the rest

	Locus* matchingLocus = FindLocusByName (locus->GetName ());

	if (matchingLocus == NULL) {

		cout << "Could not find locus matching name:  " << locus->GetName ().GetData () << endl;
		return -1;
	}

	matchingLocus->SetFirstCoreLocusBP (locus->GetFirstCoreLocusBP ());
	matchingLocus->SetFirstExtendedAllele (locus->GetFirstExtendedAllele ());
	matchingLocus->SetLastExtendedAllele (locus->GetLastExtendedAllele ());
	matchingLocus->ComputeAllBPs ();
	matchingLocus->SetMinMaxSearchILSBP (locus->GetMinSearchILSBP (), locus->GetMaxSearchILSBP ());
	matchingLocus->AdjustSearchRegion ();
	matchingLocus->SetMerged ();

	return 0;
}


int Ladder :: MergeThisLadderInto (Ladder* ladder) {

	list <Locus*>::const_iterator locusIterator;
	Locus* nextLocus;
	Locus* prevLocus = NULL;
	double prevMax;
	double nextMin;
	double nextOriginal;
	double prevOriginal;

	for (locusIterator = mLocusList.begin(); locusIterator != mLocusList.end(); locusIterator++) {

		nextLocus = *locusIterator;
		ladder->MergeLocusIntoLadder (nextLocus);
	}

	// To do:  test that search regions do not overlap neighboring loci

	for (locusIterator = mLocusList.begin(); locusIterator != mLocusList.end(); locusIterator++) {

		nextLocus = *locusIterator;
		
		if (prevLocus == NULL) {

			prevLocus = nextLocus;
			continue;
		}

		if (nextLocus->GetChannel () != prevLocus->GetChannel ()) {

			prevLocus = nextLocus;
			continue;
		}

		prevMax = prevLocus->GetMaxSearchILSBP ();
		nextMin = nextLocus->GetMinSearchILSBP ();

		if (prevMax <= nextMin) {

			prevLocus = nextLocus;
			continue;
		}

		nextOriginal = nextLocus->GetOriginalMinSearchILSBP () - 0.55;
		prevOriginal = prevLocus->GetOriginalMaxSearchILSBP () + 0.55;

		if (prevOriginal > nextOriginal)
			prevOriginal = nextOriginal = 0.5 * (prevOriginal + nextOriginal);

		if (prevMax >= nextOriginal)
			prevLocus->SetMaxSearhILSBP (nextOriginal);

		if (nextMin <= prevOriginal)
			nextLocus->SetMinSearhILSBP (prevOriginal);

		prevLocus = nextLocus;
	}

	return 0;
}


bool Ladder :: TestAllLociMerged () {

	list <Locus*>::const_iterator locusIterator;
	Locus* nextLocus;

	for (locusIterator = mLocusList.begin(); locusIterator != mLocusList.end(); locusIterator++) {

		nextLocus = *locusIterator;

		if (!nextLocus->GetMerged ())
			return false;
	}

	return true;
}


int Ladder :: AddILS (const RGString& newILSName) {

	RGString* newName = new RGString (newILSName);
	mILSList.Append (newName);
	return 0;
}



void Ladder :: OutputTo (RGTextOutput& xmlFile, LadderInputFile& inputFile) {

	RGString link ("http://www.w3.org/2001/XMLSchema-instance");
	RGString blank2 = "  ";
	RGString blank4 = "    ";
	RGString blank6 = "      ";
	RGString blank8 = "        ";
	RGString blank10 = "          ";
	RGString blank12 = "            ";

	xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	xmlFile << "<KitData xmlns:xsi=\"" << link.GetData () << "\" xsi:noNamespaceSchemaLocation=\"MarkerSet.xsd\">\n";
	xmlFile << blank2 << "<Version>" << inputFile.GetVersion () << "</Version>\n";
	xmlFile << blank2 << "\t<Kits>\n";
	xmlFile << blank4 << "<Set>\n";
	xmlFile << blank6 << "<Name>" << mMarkerSetName.GetData () << "</Name>\n";
	xmlFile << blank6 << "<NChannels>" << mNumberOfChannels << "</NChannels>\n";
	xmlFile << blank6 << "<ILS>\n";

	if (inputFile.GenerateILSFamilies ()) {

		xmlFile << blank8 << "<LSBases>\n";
		OutputILSFamilyListTo (xmlFile);
		xmlFile << blank8 << "</LSBases>\n";
	}

	else {

		xmlFile << blank6 << "<LS>\n";
		OutputILSListTo (xmlFile);
		xmlFile << blank6 << "</LS>\n";
	}

	xmlFile << blank8 << "<ChannelNo>" << mChannelForILS << "</ChannelNo>\n";
	xmlFile << blank6 << "</ILS>\n";
	xmlFile << blank6 << "<FileNameSuffix>" << mSuffix << "</FileNameSuffix>\n";
	xmlFile << blank6 << "<GenotypeSuffix>" << mSuffix << "</GenotypeSuffix>\n";
	xmlFile << blank6 << "<DirectorySearchString>" << mSuffix << "</DirectorySearchString>\n";
	OutputChannelMapTo (xmlFile, inputFile);

	list <Locus*>::const_iterator locusIterator;
	Locus* nextLocus;

	for (locusIterator = mLocusList.begin(); locusIterator != mLocusList.end(); locusIterator++) {

		nextLocus = *locusIterator;
		nextLocus->OutputTo (xmlFile);
	}

	xmlFile << blank4 << "</Set>\n";
	xmlFile << blank2 << "</Kits>\n";
	xmlFile << "</KitData>";
}


void Ladder :: OutputILSListTo (RGTextOutput& xmlFile) {

	RGString blank8 = "        ";
	
	RGDListIterator it (mILSList);
	RGString* nextName;

	while (nextName = (RGString*) it ())
		xmlFile << blank8 << "<LSName>" << nextName->GetData () << "</LSName>\n";
}


void Ladder :: OutputILSFamilyListTo (RGTextOutput& xmlFile) {

	RGString blank10 = "          ";

	RGDListIterator it (mILSList);
	RGString* nextName;

	while (nextName = (RGString*) it ())
		xmlFile << blank10 << "<ILSName>" << nextName->GetData () << "</ILSName>\n";
}


void Ladder :: OutputChannelMapTo (RGTextOutput& xmlFile, LadderInputFile& inputFile) {

	int i;
	RGString blank6 = "      ";
	RGString blank8 = "        ";
	RGString blank10 = "          ";

	xmlFile << blank6 << "<FsaChannelMap>\n";

	for (i=1; i<=mNumberOfChannels; i++) {

		xmlFile << blank8 << "<Channel>\n";
		xmlFile << blank10 << "<KitChannelNumber>" << i << "</KitChannelNumber>\n";
		xmlFile << blank10 << "<fsaChannelNumber>" << inputFile.GetFsaChannelForKitChannel (i) << "</fsaChannelNumber>\n";
		xmlFile << blank10 << "<Color>" << inputFile.GetColorName (i).GetData () << "</Color>\n";
		xmlFile << blank10 << "<DyeName>" << inputFile.GetDyeName (i).GetData () << "</DyeName>\n";
		xmlFile << blank8 << "</Channel>\n";
	}

	xmlFile << blank6 << "</FsaChannelMap>\n";
}


int Ladder :: AmendLadderData (LadderInputFile* inFile, RGString& oldLadderString) {

	RGString newLadderString;

	// Parse oldLadder data into pieces for individual edits
	RGString locusString;
	RGString* newLocusString;
	RGDList locusStrings;
	size_t startPos = 0;
	size_t endPos;
	RGXMLTagSearch locusSearch ("Locus", oldLadderString);
	RGXMLTagSearch searchRegionsSearch ("SearchRegions", oldLadderString);
	RGString blank2 = "  ";
	RGString blank4 = "    ";
	RGString blank6 = "      ";
	RGString blank8 = "        ";
	RGString blank10 = "          ";
	RGString blank12 = "            ";

	RGString* ilsName = (RGString*)inFile->GetILSNameList ().First ();
	endPos = 0;
	oldLadderString.FindNextSubstring (0, "      <Locus>", endPos);
	RGString insertBase;
	insertBase << blank10 << "<ILSName>" << ilsName->GetData () << "</ILSName>\n";
	insertBase << blank8 << "</LSBases>";
	RGString leadString = oldLadderString.ExtractSubstring (0, endPos - 1);
	//cout << "Lead string = \n" << leadString.GetData () << endl;
	endPos = 0;
	RGString blank8PlusLSBases = blank8 + "</LSBases>";
	leadString.FindAndReplaceNextSubstring (blank8PlusLSBases, insertBase, endPos);
	//cout << "Lead string = \n" << leadString.GetData () << endl;
	newLadderString << leadString;


	startPos = 0;

	while (locusSearch.FindNextTag (startPos, endPos, locusString)) {

		newLocusString = new RGString (locusString);
		locusStrings.Append (newLocusString);
		startPos = endPos;
	}

	if (mLocusList.size () != locusStrings.Entries ()) {

		cout << "Number of loci in bins file does not match number of loci in ladder file" << endl;
		return -152;
	}

	Locus* nextLocus;
	RGString locusInsert;
	RGString currentLocusString;
	RGString nameString;
	RGXMLTagSearch locusNameSearch ("Name", currentLocusString);
	RGXMLTagSearch coreRepeatSearch ("CoreRepeatNumber", currentLocusString);
	double minSearch;
	double maxSearch;
	RGString repeatString;
	int repeatNumber;
	RGString blank8PlusSearchRegions = blank8 + "</SearchRegions>";

	while (locusStrings.Entries () > 0) {

		newLocusString = (RGString*) locusStrings.GetFirst ();
		currentLocusString = *newLocusString;
		locusNameSearch.ResetSearch ();
		coreRepeatSearch.ResetSearch ();

		locusNameSearch.FindNextTag (0, endPos, nameString);
		nextLocus = FindLocusByName (nameString);

		if (nextLocus == NULL) {

			cout << "Could not find locus named " << nameString.GetData () << ".  Exiting..." << endl;
			return -155;
		}

		if (!coreRepeatSearch.FindNextTag (0, endPos, repeatString))
			repeatNumber = 4;

		else
			repeatNumber = repeatString.ConvertToInteger ();

		locusInsert = "";
		minSearch = nextLocus->GetMinSearchILSBP () - repeatNumber + 1;
		maxSearch = nextLocus->GetMaxSearchILSBP () + repeatNumber -1;

		locusInsert << blank10 << "<Region>\n";
		locusInsert << blank12 << "<ILSName>" << ilsName->GetData () << "</ILSName>\n";
		locusInsert << blank12 << "<MinGrid>" << 0.01 * floor (100.0 * minSearch + 0.5) << "</MinGrid>\n";
		locusInsert << blank12 << "<MaxGrid>" << 0.01 * floor (100.0 * maxSearch + 0.5) << "</MaxGrid>\n";
		locusInsert << blank10 << "</Region>\n";
		locusInsert << blank8 << "</SearchRegions>";
		endPos = 0;
		currentLocusString.FindAndReplaceNextSubstring (blank8PlusSearchRegions, locusInsert, endPos);

		newLadderString << "\t\t\t<Locus>" << currentLocusString << "</Locus>\n";
		delete newLocusString;
	}

	newLadderString <<  blank4 << "</Set>\n";
	newLadderString << blank2 << "</Kits>\n";
	newLadderString << "</KitData>\n";

	RGString ladderPath = inFile->GetOutputConfigDirectoryPath () + "/" + inFile->GetLadderFileName ();
	RGTextOutput ladderOutput (ladderPath, FALSE);

	if (!ladderOutput.FileIsValid ()) {

		cout << "Could not open ladder output file:  " << ladderPath.GetData () << endl;
		return -161;
	}

	ladderOutput << newLadderString;

	cout << "Ladder update completed successfully..." << endl;
	return 0;
}


int Ladder :: GetNumberOfLoci () const {

	return mLocusList.size ();
}


