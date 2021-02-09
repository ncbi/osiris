
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
*  FileName: ILSInputData.h
*  Author:   Robert Goor
*
*/
//
// ILSInputData is class to input basic settings that determine details of ILS file, etc.
// Assumes each line ends with a ";" and last line has nothing on it but a ";"
//

#ifndef _ILSINPUTDATA_H_
#define _ILSINPUTDATA_H_

#include "rgstring.h"
#include "rgtokenizer.h"
#include "rgfile.h"
#include "rgdlist.h"
#include "rgintarray.h"

class ILSInputData {

public:
	ILSInputData (bool debug);
	~ILSInputData ();

	bool IsValid () const {
		return mIsValid;
	}

	int ReadAllInputs (const RGString& inputFileName);
	int ReadAllInputs ();  // reads stdin
	int ReadLine ();
	int AssignString ();
	void OutputAllData ();
	int AssembleInputs ();

	RGString GetOutputFolder () const { return mOutputFolder; }
	RGString CreateXMLString ();

	RGString GetFamilyName () const {
		return mFamilyName;
	}

protected:
	bool mDebug;
	RGFile* mInputFile;
	bool mIsValid;
	int mNumberOfPeaks;
	int mNumberOfRelativeHeights;

	RGString mFamilyName;
	RGString mBaseLaneStdName;
	int* mPeakSizeList;
	RGStringArray mRelativeHeightList;

	int mNumberOfInitialPeaksToIgnore;
	int mNumberOfFinalPeaksToIgnore;
	RGString mDyeName;
	RGString mCorrelation1;
	RGString mCorrelation2;

	RGString mOutputFolder;

	RGString mOutputString;

	RGDList mInputLines;
	RGDListIterator* mInputLinesIterator;

	RGString mStringLeft;
	RGString mStringRight;

	RGString mCumulativeStringWithNewLines;
	RGString mCumulativeStringWithoutNewLines;

	void RemoveLeadingAndTrailingBlanks (RGString& string);
	void SetEmbeddedSlashesToForward (RGString& string);
	RGString SplitUsingColon (const RGString& target, RGString& right);  // returns what's on left of colon, and empty if no colon
};


#endif  /*  _ILSINPUTDATA_H_  */

