//
//  Reads bins file and parses
//

#ifndef _BINS_H_
#define _BINS_H_

#include "rgstring.h"
#include "rgtokenizer.h"
#include "LadderStructures.h"
#include "LadderInputFile.h"

class Bins {
public:
	Bins (const RGString& binsFileName);
	~Bins ();

	bool IsValid () { return mIsValid; }

	// Add functions to split by locus (based on "Marker Name\t" delimiter), then split each locus by line ("\n" delimiter)
	// and then split each line by field ("\t" delimiter) to create alleles

	int SplitFileIntoLocusStrings ();
	void SplitLocusStringIntoLines (int ithLocus);
	void SplitLocusLine (RGString& lineString);
	Locus* AssembleLocusFromLineStrings ();  // uses current values in mLocusLineStrings
	Ladder* AssembleAllLoci ();

protected:
	RGString mBinsText;
	RGString mBinsFileName;
	RGFile* mBinsInputFile;
	bool mIsValid;
	RGDList mLoci;
	RGStringArray mLocusStrings;
	RGStringArray mLocusLineStrings;
	RGStringArray mLocusLineFieldString;
	int mNumberOfLoci;
};

#endif  /*  _BINS_H_   */

