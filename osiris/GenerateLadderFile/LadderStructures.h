//
//	The ladder structures needed to retain allele, locus, channel and full ladder data
//

#ifndef _LADDERSTRUCTURES_H_
#define _LADDERSTRUCTURES_H_

#include "rgstring.h"
#include "rgdlist.h"
#include "RGTextOutput.h"
#include "rgintarray.h"
#include "LadderInputFile.h"
#include <list>

using namespace std;


int CopyVolumeFile (const RGString& inputFileName, const RGString& outputFileName);


class STRAlleleName {
public:
	STRAlleleName ();
	STRAlleleName (const RGString& name);
	~STRAlleleName ();

	int GetBPDifferenceFrom (STRAlleleName& strAllele, int coreRepeatNo);

protected:
	int mRepeats;
	int mMicroVariant;
};


class Allele {

public:
	Allele ();
	Allele (const RGString& name, int curveNumber, int bp);
	~Allele ();

	void SetName (const RGString& name) { mName = name; }
	void SetCurveNumber (int num) { mCurveNumber = num; }
	void SetBP (int bp) { mBP = bp; }
	void SetRelativeHeight (const RGString& height) { mRelativeHeight = height; }
	void SetVirtual (bool v) { mIsVirtual = v; }

	RGString GetName () const { return mName; }
	int GetCurveNumber () const { return mCurveNumber; }
	int GetBP () const { return mBP; }
	RGString GetRelativeHeight () const { return mRelativeHeight; }
	bool IsVirtual () const { return mIsVirtual; }
	
	void OutputTo (RGTextOutput& xmlFile);

	bool isEqual (Allele* target);

protected:
	RGString mName;
	int mCurveNumber;
	int mBP;
	RGString mRelativeHeight;
	bool mIsVirtual;
};


class Locus {

public:
	Locus ();
	Locus (const RGString& name, int channel, int coreRepeat);
	~Locus ();

	void SetName (const RGString& name) { mName = name; }
	int AddAllele (Allele* newAllele);  // returns -1 if identical to existing allele
	void SetYLinked (bool linked) { mYLinked = linked; }
	void SetMinExpectedAlleles (int num) { mMinExpectedAlleles = num; }
	void SetMaxExpectedAlleles (int num) { mMaxExpectedAlleles = num; }
	void SetFirstExtendedAllele (const RGString& first) { mFirstExtendedAllele = first; }
	void SetLastExtendedAllele (const RGString& last) { mLastExtendedAllele = last; }
	void SetFirstCoreAllele (const RGString& first) { mFirstCoreAllele = first; }
	void SetFirstCoreLocusBP (int bp) { mFirstCoreLocusBP = bp; }
	void SetMerged () { mIsMerged = true; }
	void ResetCoreRepeat (int n) { mCoreRepeat = n; }

	RGString GetFirstExtendedAllele () const { return mFirstExtendedAllele; }
	RGString GetLastExtendedAllele () const { return mLastExtendedAllele; }
	RGString GetFirstCoreAllele () const { return mFirstCoreAllele; }
	int GetFirstCoreLocusBP () const { return mFirstCoreLocusBP; }
	double GetMinSearchILSBP () const { return mMinSearchILSBP; }
	double GetMaxSearchILSBP () const { return mMaxSearchILSBP; }
	double GetOriginalMinSearchILSBP () const { return mOriginalMinSearchILSBP; }
	double GetOriginalMaxSearchILSBP () const { return mOriginalMaxSearchILSBP; }
	bool GetMerged () const { return mIsMerged; }
	int GetChannel () const { return mChannel; }
	bool IsQualityLocus () const { return mIsQualityLocus; }
	bool HasTwoAlleles () const { return mAlleleList.size () == 2; }

	void SetMinMaxLocusBP (int min, int max) { mMinLocusBP = min; mMaxLocusBP = max; }
	void SetMinMaxSearchILSBP (double min, double max);
	void SetMinSearhILSBP (double min) { mMinSearchILSBP = min; }
	void SetMaxSearhILSBP (double max) { mMaxSearchILSBP = max; }
	void SetRelativeHeightInfo (bool rh) { mNeedsRelativeHeightInfo = rh; }
	void SetQualityLocus (bool b) { mIsQualityLocus = b; }

	void SetDoNotExtend () { mDoNotExtend = true; }

	RGString GetName () const { return mName; }

	bool isEqual (Locus* locus);
	void AdjustSearchRegion ();

	void OutputTo (RGTextOutput& xmlFile);
	int ComputeAllBPs ();

	static void SetGenerateILSFamilies (bool s) { GenerateILSFamilies = s; }
	static bool GetGenerateILSFamilies () { return GenerateILSFamilies; }

	static void SetILSName (const RGString& name) { ILSName = name; }
	static RGString GetILSName () { return ILSName; }

protected:
	RGString mName;
	int mChannel;
	int mCoreRepeat;
	int mMinLocusBP;
	int mMaxLocusBP;
	double mMinSearchILSBP;
	double mMaxSearchILSBP;
	bool mYLinked;
	int mMinExpectedAlleles;
	int mMaxExpectedAlleles;
	list<Allele*> mAlleleList;
	RGString mFirstExtendedAllele;
	RGString mLastExtendedAllele;
	RGString mFirstCoreAllele;
	int mFirstCoreLocusBP;
	bool mIsMerged;
	double mOriginalMinSearchILSBP;
	double mOriginalMaxSearchILSBP;
	bool mDoNotExtend;
	bool mNeedsRelativeHeightInfo;
	bool mIsQualityLocus;

	static bool GenerateILSFamilies;
	static RGString ILSName;
};


class Ladder {

public:
	Ladder ();
	Ladder (const RGString& markerSetName);
	~Ladder ();

	void SetMarkerSetName (const RGString& name) { mMarkerSetName = name; }
	void SetNumberOfChannels (int num) { mNumberOfChannels = num; }
	void SetSuffix (const RGString& suffix) { mSuffix = suffix; }
	void SetDefaultYLinked (bool linked) { mDefaultYLinked = linked; }
	void SetDefaultExpectedAllelesPerLocus (int min, int max) { mDefaultMinExpectedAllelesPerLocus = min; mDefaultMaxExpectedAllelesPerLocus = max; }
	void SetILSChannel (int channel) { mChannelForILS = channel; }

	int AddLocus (Locus* locus);
	Locus* FindLocusByName (const RGString& locusName);
	int MergeLocusIntoLadder (const Locus* locus);  // This combines info and then computes the rest
	int MergeThisLadderInto (Ladder* ladder);
	bool TestAllLociMerged ();
	int AddILS (const RGString& newILSName);

	void OutputTo (RGTextOutput& xmlFile, LadderInputFile& inputFile);
	void OutputILSListTo (RGTextOutput& xmlFile);
	void OutputILSFamilyListTo (RGTextOutput& xmlFile);
	void OutputChannelMapTo (RGTextOutput& xmlFile, LadderInputFile& inputFile);

	int AmendLadderData (LadderInputFile* inFile, RGString& oldLadderString);

	int GetNumberOfLoci () const;

protected:
	RGString mMarkerSetName;
	int mNumberOfChannels;
	RGString mSuffix;
	bool mDefaultYLinked;
	int mDefaultMinExpectedAllelesPerLocus;
	int mDefaultMaxExpectedAllelesPerLocus;
	list<Locus*> mLocusList;
	RGDList mILSList;
	int mChannelForILS;
};


#endif  /*  _LADDERSTRUCTURES_H_  */

