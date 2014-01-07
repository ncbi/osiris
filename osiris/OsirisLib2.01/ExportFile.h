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
*  FileName: ExportFile.h
*  Author:   Robert Goor
*
*/
//
// Declaration file for export classes and for export list
//

#ifndef _EXPORTFILE_H_
#define _EXPORTFILE_H_

#include "rgstring.h"
#include "rgfile.h"
#include "RGTextOutput.h"
#include "rgdlist.h"
#include <list>

using namespace std;

class FlatExportFile;

class ExportFile {

public:
	ExportFile (const RGString& outputNameRoot);
	virtual ~ExportFile ();

	virtual int Initialize (const RGString& specificationFile) = 0;
	virtual int Export (const RGString& osirisAnalysisRecord) = 0;

protected:
	RGString mOutputNameRoot;
	RGString mSpecificationFile;
	RGFile* mOutputFile;
	RGTextOutput* mOutputTextFile;
	bool mEnabled;
};


class FlatFileColumn {

public:
	FlatFileColumn (const RGString& heading);
	virtual ~FlatFileColumn ();

	virtual int Initialize (const RGString& specificationString);
	virtual int ExtractDataForColumn (const RGString& oarSubString, RGString& currentRowString, FlatExportFile* parent);

	RGString GetHeading () const { return mHeading; }
	RGString GetDataType () const { return mDataType; }
	RGString GetDataScope () const { return mDataScope; }
	RGString* GetLastDataLocationTag () const { return (RGString*) mDataLocationTags.Last (); }
	int GetCharacterLimit () const { return mCharacterLimit; }
	bool Concatenate () const { return mConcatenate; }
	RGString GetConcatenationCharacter () const { return mConcatenationCharacter; }

	static void SetCurrentDirectory (const RGString& name) { CurrentDirectory = name; }
	static void SetCurrentSampleName (const RGString& name) { CurrentSampleName = name; }
	static void SetCurrentSampleFileName (const RGString& name) { CurrentSampleFileName = name; }
	static void SetCurrentLocusName (const RGString& name) { CurrentLocusName = name; }
	static void SetCurrentAlleleName (const RGString& name) { CurrentAlleleName = name; }

	static RGString GetCurrentDirectory () { return CurrentDirectory; }
	static RGString GetCurrentSampleName () { return CurrentSampleName; }
	static RGString GetCurrentSampleFileName () { return CurrentSampleFileName; }
	static RGString GetCurrentLocusName () { return CurrentLocusName; }
	static RGString GetCurrentAlleleName () { return CurrentAlleleName; }

	static void SetDirectoryPosition (size_t p) { CurrentDirectoryPosition = p; }
	static void SetSamplePosition (size_t p) { CurrentSamplePosition = p; }
	static void SetLocusPosition (size_t p) { CurrentLocusPosition = p; }

	static size_t GetDirectoryPosition () { return CurrentDirectoryPosition; }
	static size_t GetSamplePosition () { return CurrentSamplePosition; }
	static size_t GetLocusPosition () { return CurrentLocusPosition; }

protected:
	RGString mHeading;
	RGString mDataType;
	RGString mDataScope;
	int mDataTypeIndex;
	RGDList mDataLocationTags;
	int mCharacterLimit;
	bool mConcatenate;
	RGString mConcatenationCharacter;
	bool mPriorityBased;

	static RGString CurrentDirectory;
	static RGString CurrentSampleName;
	static RGString CurrentSampleFileName;
	static RGString CurrentLocusName;
	static RGString CurrentAlleleName;

	static size_t CurrentDirectoryPosition;
	static size_t CurrentSamplePosition;
	static size_t CurrentLocusPosition;
};


class FlatFileColumnForLocusRecord : public FlatFileColumn {

public:
	FlatFileColumnForLocusRecord (const RGString& heading);
	virtual ~FlatFileColumnForLocusRecord ();

	virtual int Initialize (const RGString& specificationString);
	virtual int ExtractDataForColumn (const RGString& oarSubString, RGString& currentRowString, FlatExportFile* parent);
};


class FlatFileColumnForAlleleRecord : public FlatFileColumn {

public:
	FlatFileColumnForAlleleRecord (const RGString& heading);
	virtual ~FlatFileColumnForAlleleRecord ();

	virtual int Initialize (const RGString& specificationString);
	virtual int ExtractDataForColumn (const RGString& oarSubString, RGString& currentRowString, FlatExportFile* parent);
};


class FlatFileColumnForSampleRecord : public FlatFileColumn {

public:
	FlatFileColumnForSampleRecord (const RGString& heading);
	virtual ~FlatFileColumnForSampleRecord ();

	virtual int Initialize (const RGString& specificationString);
	virtual int ExtractDataForColumn (const RGString& oarSubString, RGString& currentRowString, FlatExportFile* parent);
};


class FlatFileColumnForDirectoryRecord : public FlatFileColumn {

public:
	FlatFileColumnForDirectoryRecord (const RGString& heading);
	virtual ~FlatFileColumnForDirectoryRecord ();

	virtual int Initialize (const RGString& specificationString);
	virtual int ExtractDataForColumn (const RGString& oarSubString, RGString& currentRowString, FlatExportFile* parent);
};


class FlatExportFile : public ExportFile {

public:
	FlatExportFile (const RGString& outputNameRoot);
	virtual ~FlatExportFile ();

	virtual int Initialize (const RGString& specificationFile);
	virtual int Export (const RGString& osirisAnalysisRecord);

	RGString GetCurrentDirectorySubstring () const { return mCurrentDirectorySubstring; }
	RGString GetCurrentSampleSubstring () const { return mCurrentSampleSubstring; }
	RGString GetCurrentLocusSubstring () const { return mCurrentLocusSubstring; }
	RGString GetCurrentAlleleSubstring () const { return mCurrentAlleleSubstring; }

	RGString GetNextSampleSubstring ();
	RGString GetNextLocusSubstring ();
	RGString GetNextAlleleSubstring ();

protected:
	RGString mName;
	RGString mFileName;
	RGString mFileNameSuffix;
	RGString mDelimiter;
	RGString mRecordDefinition; // directory, sample, channel, locus, allele
	bool mFirstLineSpecial; // true means to include header
	RGString mLastLine;  // empty means don't include
	list<FlatFileColumn*> mColumnList;

	RGString mCurrentDirectorySubstring;
	RGString mCurrentSampleSubstring;
	RGString mCurrentLocusSubstring;
	RGString mCurrentAlleleSubstring;
};


class ExportFileList {

public:
	ExportFileList (const RGString& osirisAnalysisRecord, const RGString& specificationFile);
	~ExportFileList ();

	int Initialize (const RGString& outputNameRoot);
	int ExportAll ();

protected:
	list<ExportFile*> mExportFileList;
	RGString mOsirisAnalysisRecordName;
	RGString mSpecificationFileName;
	RGString mOutputNameRoot;
};


#endif  /*  _EXPORTFILE_H_  */

