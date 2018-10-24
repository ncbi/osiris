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
*  FileName: OsirisInputFile.h
*  Author:   Robert Goor
*
*/
//
// LadderInputFile is class to input basic settings that determine details of ladder configuration file, etc.
// Assumes each line ends with a ";" and last line has nothing on it but a ";"
//

#ifndef _LADDERINPUTFILE_H_
#define _LADDERINPUTFILE_H_

#include "rgstring.h"
#include "rgtokenizer.h"
#include "rgfile.h"
#include "rgdlist.h"
#include "rgintarray.h"

class LadderInputFile {

public:
	LadderInputFile (bool debug);
	~LadderInputFile ();

	int ReadAllInputs (const RGString& inputFileName);
	int ReadAllInputsAppend (const RGString& inputFileName);
	int ReadAllInputs ();  // reads stdin
	int ReadAllInputsAppend ();  // reads stdin
	int ReadLine ();
	int ReadFirstLine ();
	int AssignString ();
	int AssignStringAppend ();
	void OutputAllData ();
	int AssembleInputs ();
	int AssembleInputsAppend ();

	int GetChannelForColorName (const RGString& color) const;
	int GetChannelForDyeName (const RGString& dye) const;

	RGString GetLadderFileName () const { return mLadderFileName; }
	RGString GetKitName () const { return mKitName; }
	RGString GetLadderDirectory () const { return mLadderDirectory; }
	int GetNumberOfDyes () const { return mNumberOfDyes; }
	RGString GetDyeName (int dye) const { return mDyeNames [dye]; }
	RGString GetColorName (int dye) const { return mColorNames [dye]; }	//???
	int GetILSChannel () const { return mChannelForILS; }

	RGString GetPanelsFileName () const { return mPanelsFileName; }
	RGString GetSearchString () const { return mSearchString; }
	RGString GetBinsFileName () const { return mBinsFileName; }

	RGString GetOutputConfigDirectoryPath () const { return mOutputConfigDirectoryPath; }

	bool GetYLinkedDefault () const { return mYLinkedDefault; }
	int GetMaxExpectedAllelesPerLocusDefault () const { return mMaxExpectedAllelesPerLocusDefault; }
	int GetMinExpectedAllelesPerLocusDefault () const { return mMinExpectedAllelesPerLocusDefault; }
	RGDList& GetILSNameList () { return mILSNames; }
	RGDList& YLinkedOverrides () { return mYLinkedDefaultOverrides; }
	RGDList& DoNotExtends () { return mDoNotExtends; }
	RGDList& GetQualityLoci () { return mQualityLoci; }

	bool GetHid () const { return mGenerateHID; }
	RGString GetStandardPositiveControlName () const { return mStandardPositiveControlName; }
	RGString GetVolumeDirectoryName () const { return mVolumeDirectoryName; }
	RGString GetDefaultVolumePath () const { return mDefaultVolumePath; }

	RGString GetVersion () const { return mVersion; }
	bool GenerateILSFamilies () const { return mGenerateILSFamilies; }

	int FindMaxExpectedAlleleOverride (const RGString& locusName);
	int FindMinExpectedAlleleOverride (const RGString& locusName);  // This and above return value in list, if present, and default otherwise

	int GetFsaChannelForKitChannel (int i) { return mChannelMap [i]; }

	void ResetInputLines ();
	RGString* GetNextInputLine ();
	bool LocusNeedsRelativeHeightInfo (const RGString& locus);

	static void SetNumberOfPanelsLinesSkipped (int n) { PanelsNumberOfLinesSkipped = n; }
	static void SetColorColumn (int n) { ColorColumn = n; }
	static void SetRepeatSizeColumn (int n) { RepeatSizeColumn = n; }
	static void SetAlleleListColumn (int n) { AlleleListColumn = n; }
	static void SetAlleleListDelineation (const RGString& s) { AlleleListDelineation = s; }
	static void SetColumnDelineation (const RGString& s) { ColumnDelineation = s; }
	static void SetBinsDelineation (const RGString& s) { BinsDelineation = s; }

	static int GetNumberOfPanelsLinesSkipped () { return PanelsNumberOfLinesSkipped; }
	static int GetColorColumn () { return ColorColumn; }
	static int GetRepeatSizeColumn () { return RepeatSizeColumn; }
	static int GetAlleleListColumn () { return AlleleListColumn; }
	static RGString GetAlleleListDelineation () { return AlleleListDelineation; }
	static RGString GetColumnDelineation () { return ColumnDelineation; }
	static RGString GetBinsDelineation () { return BinsDelineation; }

protected:
	bool mDebug;
	RGFile* mInputFile;

	RGString mLadderFileName;
	RGString mKitName;
	RGString mLadderDirectory;

	int mNumberOfDyes;
	RGStringArray mDyeNames;
	RGStringArray mColorNames;
	RGString mPanelsFileName;
	RGString mBinsFileName;
	RGString mSearchString;
	RGString mOutputConfigDirectoryPath;

	RGString mOutputString;

	RGDList mInputLines;
	RGDListIterator* mInputLinesIterator;

	RGString mStringLeft;
	RGString mStringRight;

	RGString mCumulativeStringWithNewLines;
	RGString mCumulativeStringWithoutNewLines;

	RGDList mDyeNumbers;
	RGDList mColorNumbers;
	RGDList mRelativeHeightOverrides;

	bool mYLinkedDefault;
	int mMaxExpectedAllelesPerLocusDefault;
	int mMinExpectedAllelesPerLocusDefault;

	RGDList mILSNames;
	RGDList mYLinkedDefaultOverrides;
	RGDList mMaxExpectedAllelesOverrides;
	RGDList mMinExpectedAllelesOverrides;
	RGDList mQualityLoci;

	RGString mILSFamilyName;

	RGDList mDoNotExtends;

	int mChannelForILS;
	RGIntArray mChannelMap;
	bool mGenerateHID;

	RGString mStandardPositiveControlName;
	RGString mVolumeDirectoryName;
	RGString mDefaultVolumePath;
	RGString mVersion;
	bool mGenerateILSFamilies;

	void RemoveLeadingAndTrailingBlanks (RGString& string);
	void SetEmbeddedSlashesToForward (RGString& string);
	RGString SplitUsingColon (const RGString& target, RGString& right);  // returns what's on left of colon, and empty if no colon

	static int PanelsNumberOfLinesSkipped;
	static int ColorColumn;
	static int RepeatSizeColumn;
	static int AlleleListColumn;
	static RGString AlleleListDelineation;
	static RGString ColumnDelineation;
	static RGString BinsDelineation;
};


#endif  /*  _LADDERINPUTFILE_H_  */

