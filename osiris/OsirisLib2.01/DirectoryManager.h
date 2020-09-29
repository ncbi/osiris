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
*  FileName: DirectoryManager.h
*  Author:   Robert Goor
*
*/
//
// class DirectoryManager manages a directory and cycles (continuously) through selected files
//

#ifndef _DIRECTORYMANAGER_H_
#define _DIRECTORYMANAGER_H_

#include "rgdirectory.h"
#include "rgstring.h"
#include "rgdefs.h"
#include "rgdlist.h"
#include "STRLCAnalysis.h"

class ParameterServer;

class DirectoryManager {

public:
	DirectoryManager (const RGString& name);
	virtual ~DirectoryManager ();

	virtual bool Initialize ();

	Boolean IsValid () const { return Valid; }
	void RewindDirectory () { Directory->RewindDirectory (); }
	void SetAnalysisDirectory (STRLCAnalysis* dir) { mAnalysisDirectory = dir; }

	static void SetCycleOn () { Cycle = TRUE; }
	static void SetCycleOff () { Cycle = FALSE; }

	virtual Boolean GetNextLadderFile (RGString& fileName, Boolean& Cycled);
	virtual Boolean GetNextSampleFile (RGString& fileName, Boolean& Cycled);
	virtual Boolean GetNextNonLadderFile (RGString& fileName, Boolean& Cycled);

	bool TestForExtension (const RGString& fileName);

	void ResetOrderedSearch ();
	virtual Boolean GetNextOrderedSampleFile (RGString& fileName);

	int GetNumberOfFilesInDirectory () const;

	static void SetDataFileType (const RGString& type) { DataFileType = type; DataFileTypeWithDot = "." + type; }
	static RGString GetDataFileType () { return DataFileType; }
	static RGString GetDataFileTypeWithDot () { return DataFileTypeWithDot; }

protected:
	RGString DirectoryName;
	RGDirectory* Directory;
	Boolean Valid;
	ParameterServer* mPServer;
	Boolean mReadAllPosControls;
	Boolean mReadAllNegControls;
	Boolean mReadAllSamples;

	STRLCAnalysis* mAnalysisDirectory;

	Boolean TestForLadder (const RGString& fileName);
	Boolean TestForSample (const RGString& fileName);
	Boolean TestForNonLadder (const RGString& fileName);

	Boolean TestForPositiveControl (const RGString& fileName);
	Boolean TestForNegativeControl (const RGString& fileName);


	static Boolean Cycle;
	static RGString DataFileType;
	static RGString DataFileTypeWithDot;
};


class SampleNameDirectoryManager : public DirectoryManager {

public:
	SampleNameDirectoryManager (const RGString& name);
	virtual ~SampleNameDirectoryManager ();

	virtual bool Initialize ();

	virtual Boolean GetNextLadderFile (RGString& fileName, Boolean& Cycled);
	virtual Boolean GetNextOrderedSampleFile (RGString& fileName);
//	virtual Boolean GetNextSampleFile (RGString& fileName, Boolean& Cycled);
//	virtual Boolean GetNextNonLadderFile (RGString& fileName, Boolean& Cycled);

protected:
	RGDList mLadderList;
	RGDList mPositiveControlList;
	RGDList mNegativeControlList;
	RGDList mSampleList;
};

#endif  /*  _DIRECTORYMANAGER_H_  */

