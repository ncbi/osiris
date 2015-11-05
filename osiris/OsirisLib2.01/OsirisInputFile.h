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
// OsirisInputFile is class to input basic settings that determine location and identity of configuration files plus data input files, etc.
// Assumes each line ends with a ";" and last line has nothing on it but a ";"
//

#ifndef _OSIRISINPUTFILE_H_
#define _OSIRISINPUTFILE_H_

#include "rgstring.h"
#include "rgfile.h"
#include "rgdlist.h"
#include "ParameterServer.h"

class OsirisInputFile {

public:
	OsirisInputFile (bool debug);
	~OsirisInputFile ();

	int ReadAllInputs (const RGString& inputFileName);
	int ReadAllInputs ();  // reads stdin
	int ReadLine ();
	int AssignString ();
	void OutputAllData ();
	int AssembleInputs ();

	RGString GetInputDirectory () const { return mInputDirectory; }
	RGString GetOutputSubDirectory () const { return mOutputSubDirectory; }
	RGString GetLadderDirectory () const { return mLadderDirectory; }
	RGString GetReportDirectory () const { return mReportDirectory; }
	RGString GetMarkerSetName () const { return mMarkerSetName; }
	RGString GetLaneStandardName () const { return mLaneStandardName; }

	RGString GetGroupName () const { return mGroupName; }
	RGString GetProtocolName () const { return mProtocolName; }
	RGString GetPathToNamedSettingsFiles () const { return mPathToNamedSettingsFiles; }

	RGString GetFullPathToMessageBook () const { return mFullPathToMessageBook; }
	RGString GetFullPathToLabAndStdSettings () const { return mFullPathToLabAndStdSettings; }

	RGString GetFinalMessageBookName () const { return mFinalMessageBookName; }
	RGString GetFinalLabSettingsName () const { return mFinalLabSettingsName; }
	RGString GetFinalStdSettingsName () const { return mFinalStdSettingsName; }

	list<channelThreshold*>* GetAnalysisThresholdOverrideList () { return mAnalysisThresholds; }
	list<channelThreshold*>* GetDetectionThresholdOverrideList () { return mDetectionThresholds; }

	RGString GetOverrideString () const { return mOverrideString; }
	bool OverrideStringIsEmpty () const { return (mOverrideString.Length () == 0); }

	int GetCriticalOutputLevel () const { return mCriticalOutputLevel; }
	double GetMinSampleRFU () const { return mMinSampleRFU; }
	double GetMinLadderRFU () const { return mMinLadderRFU; }
	double GetMinLaneStandardRFU () const { return mMinLaneStandardRFU; }
	double GetMinInterlocusRFU () const { return mMinInterlocusRFU; }
	double GetMinLadderInterlocusRFU () const { return mMinLadderInterlocusRFU; }
	double GetSampleDetectionThreshold () const { return mSampleDetectionThreshold; }

	bool UseRawData () const { return mUseRawData; }
	bool UserNamedSettingsFiles () const { return mUserNamedSettingsFiles; }

	void ResetInputLines ();
	RGString* GetNextInputLine ();

	void OutputAnalysisThresholdOverrides ();
	void OutputDetectionThresholdOverrides ();

protected:
	bool mDebug;
	RGFile* mInputFile;
	RGString mCumulativeStringWithNewLines;
	RGString mCumulativeStringWithoutNewLines;
	RGString mOutputString;

	RGDList mInputLines;
	RGDListIterator* mInputLinesIterator;

	RGString mStringLeft;
	RGString mStringRight;

	RGString mInputDirectory;
	RGString mOutputSubDirectory;
	RGString mLadderDirectory;
	RGString mReportDirectory;
	RGString mMarkerSetName;
	RGString mLaneStandardName;
	
	RGString mGroupName;
	RGString mProtocolName;
	RGString mPathToNamedSettingsFiles;
	
	RGString mFullPathToMessageBook;
	RGString mFullPathToLabAndStdSettings;

	RGString mGroupNamePrefix;
	RGString mFullPathToNamedSettingsFiles;

	RGString mFinalMessageBookName;
	RGString mFinalLabSettingsName;
	RGString mFinalStdSettingsName;

	RGString mOverrideString;

	int mCriticalOutputLevel;
	double mMinSampleRFU;
	double mMinLadderRFU;
	double mMinLaneStandardRFU;
	double mMinInterlocusRFU;
	double mMinLadderInterlocusRFU;
	double mSampleDetectionThreshold;

	bool mUseRawData;
	bool mUserNamedSettingsFiles;

	list<channelThreshold*>* mAnalysisThresholds;
	list<channelThreshold*>* mDetectionThresholds;

	void RemoveLeadingAndTrailingBlanks (RGString& string);
	void SetEmbeddedSlashesToForward (RGString& string);
	RGString SplitUsingColon (const RGString& target, RGString& right);  // returns what's on left of colon, and empty if no colon
};


#endif  /*  _OSIRISINPUTFILE_H_  */

