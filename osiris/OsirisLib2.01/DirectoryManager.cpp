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
*  FileName: DirectoryManager.cpp
*  Author:   Robert Goor
*
*/
//
// class DirectoryManager manages a directory and cycles (continuously) through selected files
//

#include "DirectoryManager.h"
#include "ParameterServer.h"
#include "STRLCAnalysis.h"
#include "fsaFileData.h"
#include "SmartMessage.h"
#include "SmartNotice.h"
#include "STRSmartMessage.h"
#include "STRSmartNotices.h"


Boolean DirectoryManager::Cycle = TRUE;
RGString DirectoryManager::DataFileType = "fsa";
RGString DirectoryManager::DataFileTypeWithDot = ".fsa";



DirectoryManager :: DirectoryManager (const RGString& name) : DirectoryName (name), Valid (TRUE),
mReadAllPosControls (FALSE), mReadAllNegControls (FALSE), mReadAllSamples (FALSE), mAnalysisDirectory (NULL) {

	Directory = new RGDirectory (name);

	if (!Directory->IsValid ())
		Valid = FALSE;

	mPServer = new ParameterServer;
}


DirectoryManager :: ~DirectoryManager () {

	delete Directory;
//	delete mPServer;
}


bool DirectoryManager :: Initialize () {

	return true;
}


Boolean DirectoryManager :: GetNextLadderFile (RGString& fileName, Boolean& Cycled) {

	Cycled = FALSE;

	while (Directory->ReadNextDirectory (fileName)) {

		if (TestForLadder (fileName))
			return TRUE;
	}

	Directory->RewindDirectory ();
	Cycled = TRUE;

	while (Directory->ReadNextDirectory (fileName)) {

		if (TestForLadder (fileName))
			return TRUE;
	}

	return FALSE;
}



Boolean DirectoryManager :: GetNextSampleFile (RGString& fileName, Boolean& Cycled) {

	Cycled = FALSE;

	while (Directory->ReadNextDirectory (fileName)) {

		if (TestForSample (fileName))
			return TRUE;
	}

	Directory->RewindDirectory ();
	Cycled = TRUE;

	while (Directory->ReadNextDirectory (fileName)) {

		if (TestForSample (fileName))
			return TRUE;
	}

	return FALSE;
}


Boolean DirectoryManager :: GetNextNonLadderFile (RGString& fileName, Boolean& Cycled) {

	Cycled = FALSE;

	while (Directory->ReadNextDirectory (fileName)) {

		if (TestForNonLadder (fileName))
			return TRUE;
	}

	Directory->RewindDirectory ();
	Cycled = TRUE;

	while (Directory->ReadNextDirectory (fileName)) {

		if (TestForNonLadder (fileName))
			return TRUE;
	}

	return FALSE;
}


bool DirectoryManager :: TestForExtension (const RGString& fileName) {

	RGString Extension;
	int length = fileName.Length ();
//	size_t position;
//	RGString localFileName (fileName);
//	localFileName.ToLower ();

	if (length > 4)
		Extension = fileName.ExtractSubstring (length-4, length-1);
	
	if ((length > 4) && (Extension == DirectoryManager::DataFileTypeWithDot))
		return true;

	return false;
}


void DirectoryManager :: ResetOrderedSearch () {

	mReadAllPosControls = FALSE;
	mReadAllNegControls = FALSE;
	mReadAllSamples = FALSE;
	Directory->RewindDirectory ();
}


Boolean DirectoryManager :: GetNextOrderedSampleFile (RGString& fileName) {

	if (!mReadAllPosControls) {

		while (Directory->ReadNextDirectory (fileName)) {

			if (TestForPositiveControl (fileName))
				return TRUE;
		}

		mReadAllPosControls = TRUE;
		Directory->RewindDirectory ();
	}

	if (!mReadAllNegControls) {

		while (Directory->ReadNextDirectory (fileName)) {

			if (TestForNegativeControl (fileName))
				return TRUE;
		}

		mReadAllNegControls = TRUE;
		Directory->RewindDirectory ();
	}

	if (!mReadAllSamples) {

		while (Directory->ReadNextDirectory (fileName)) {

			if (TestForSample (fileName))
				return TRUE;
		}

		mReadAllSamples = TRUE;
		return FALSE;
	}

	return FALSE;
}



Boolean DirectoryManager :: TestForLadder (const RGString& fileName) {

	int length = (int)fileName.Length ();
	RGString Extension;
	Boolean FoundLadder;
//	size_t position;
//	RGString localFileName (fileName);
//	localFileName.ToLower ();

	if (length > 4)
		Extension = fileName.ExtractSubstring (length-4, length-1);

//	FoundLadder = localFileName.FindSubstring ("ladder", position);
	FoundLadder = mPServer->LadderDoesTargetStringContainASynonymCaseIndep (fileName);
	
	if ((length > 4) && (Extension == DirectoryManager::DataFileTypeWithDot) && FoundLadder)	//old = if ((length > 4) && (Extension == ".fsa") && FoundLadder)
		return TRUE;

	return FALSE;
}


Boolean DirectoryManager :: TestForSample (const RGString& fileName) {

	int length = (int)fileName.Length ();
	RGString Extension;
	Boolean FoundControl;
//	size_t position;
//	RGString localFileName (fileName);
//	localFileName.ToLower ();

	if (length > 4)
		Extension = fileName.ExtractSubstring (length-4, length-1);

//	FoundControl = localFileName.FindSubstring ("ladder", position) || localFileName.FindSubstring ("cntl", position) || 
//		localFileName.FindSubstring ("(+)", position) || localFileName.FindSubstring ("(-)", position);
	FoundControl = mPServer->LadderDoesTargetStringContainASynonymCaseIndep (fileName) ||
		mPServer->ControlDoesTargetStringContainASynonymCaseIndep (fileName);
	
	if ((length > 4) && (Extension == DirectoryManager::DataFileTypeWithDot) && !FoundControl)		//old = if ((length > 4) && (Extension == ".fsa") && !FoundControl)
		return TRUE;

	return FALSE;
}


Boolean DirectoryManager :: TestForNonLadder (const RGString& fileName) {

	int length = (int)fileName.Length ();
	RGString Extension;
	Boolean FoundControl;
//	size_t position;
//	RGString localFileName (fileName);
//	localFileName.ToLower ();

	if (length > 4)
		Extension = fileName.ExtractSubstring (length-4, length-1);

//	FoundControl = localFileName.FindSubstring ("ladder", position);
	FoundControl = mPServer->LadderDoesTargetStringContainASynonymCaseIndep (fileName);
	
	if ((length > 4) && (Extension == DirectoryManager::DataFileTypeWithDot) && !FoundControl)		//old = if ((length > 4) && (Extension == ".fsa") && !FoundControl)
		return TRUE;

	return FALSE;
}


Boolean DirectoryManager :: TestForPositiveControl (const RGString& fileName) {

	int length = (int)fileName.Length ();
	RGString Extension;
	Boolean FoundPosControl;
	Boolean FoundLadder;

	if (length > 4)
		Extension = fileName.ExtractSubstring (length-4, length-1);

	FoundPosControl = mPServer->PosControlDoesTargetStringContainASynonymCaseIndep (fileName);
	FoundLadder = mPServer->LadderDoesTargetStringContainASynonymCaseIndep (fileName);
	
	if ((length > 4) && (Extension == DirectoryManager::DataFileTypeWithDot) && FoundPosControl && !FoundLadder)		//old = if ((length > 4) && (Extension == ".fsa") && FoundPosControl && !FoundLadder)
		return TRUE;

	return FALSE;
}


Boolean DirectoryManager :: TestForNegativeControl (const RGString& fileName) {

	int length = (int)fileName.Length ();
	RGString Extension;
	Boolean FoundPosControl;
	Boolean FoundNegControl;
	Boolean FoundLadder;

	if (length > 4)
		Extension = fileName.ExtractSubstring (length-4, length-1);

	FoundPosControl = mPServer->PosControlDoesTargetStringContainASynonymCaseIndep (fileName);
	FoundNegControl = mPServer->NegControlDoesTargetStringContainASynonymCaseIndep (fileName);
	FoundLadder = mPServer->LadderDoesTargetStringContainASynonymCaseIndep (fileName);
	
	if ((length > 4) && (Extension == DirectoryManager::DataFileTypeWithDot) && !FoundPosControl && FoundNegControl && !FoundLadder)		//old = if ((length > 4) && (Extension == ".fsa") && !FoundPosControl && FoundNegControl && !FoundLadder)
		return TRUE;

	return FALSE;
}


int DirectoryManager :: GetNumberOfFilesInDirectory () const {

	RGDirectory* directory = new RGDirectory (DirectoryName);
	int Count = 0;
	RGString fileName;

	while (directory->ReadNextDirectory (fileName)) {

		Count++;
	}

	delete directory;
	return Count;
}


SampleNameDirectoryManager :: SampleNameDirectoryManager (const RGString& name) : DirectoryManager (name) {

}



SampleNameDirectoryManager :: ~SampleNameDirectoryManager () {

	mLadderList.ClearAndDelete ();
	mPositiveControlList.ClearAndDelete ();
	mNegativeControlList.ClearAndDelete ();
	mSampleList.ClearAndDelete ();
}



bool SampleNameDirectoryManager :: Initialize () {

	fsaFileData* data;
	RGString fileName;
	RGString sampleName;
	RGString* nextFileName;
	RGString NoticeStr;
	smSamplesAreNotValidInputFiles samplesNotValidInputFiles;

	while (Directory->ReadNextDirectory (fileName)) {

		if (!TestForExtension (fileName))
			continue;

		if (STRLCAnalysis::FileNameIsInInvalidList (fileName)) {

			continue;
		}

		data = new fsaFileData (DirectoryName + "/" + fileName);

		if (!data->IsValid ()) {
			
			STRLCAnalysis::AddInvalidFile (fileName);

			//NoticeStr << "\n" << fileName.GetData () << " is not valid...Skipping\n";
			//STRLCAnalysis::mFailureMessage->AddMessage ("Data is not valid in file named " + fileName + "...Skipping");
			STRLCAnalysis::mFailureMessage->FsaHidFileInvalid (fileName);
			//cout << NoticeStr << endl;
			//ExcelText.Write (1, NoticeStr);
			//text << NoticeStr;
			STRLCAnalysis::mFailureMessage->SetPingValue (630);
			STRLCAnalysis::mFailureMessage->WriteAndResetCurrentPingValue ();
			mAnalysisDirectory->SetMessageValue (samplesNotValidInputFiles, true);
			mAnalysisDirectory->AppendDataForSmartMessage (samplesNotValidInputFiles, MainMessages::XML (fileName));
			delete data;
			continue;
		}

		sampleName = data->GetSampleName ();
		delete data;

		if (mPServer->LadderDoesTargetStringContainASynonymCaseIndep (sampleName)) {

			nextFileName = new RGString (fileName);
			mLadderList.Append (nextFileName);
		}

		else if (mPServer->PosControlDoesTargetStringContainASynonymCaseIndep (sampleName)) {

			nextFileName = new RGString (fileName);
			mPositiveControlList.Append (nextFileName);
		}

		else if (mPServer->NegControlDoesTargetStringContainASynonymCaseIndep (sampleName)) {

			nextFileName = new RGString (fileName);
			mNegativeControlList.Append (nextFileName);
		}

		else {

			nextFileName = new RGString (fileName);
			mSampleList.Append (nextFileName);
		}
	}
	
	return true;
}

Boolean SampleNameDirectoryManager :: GetNextLadderFile (RGString& fileName, Boolean& Cycled) {

	RGString* name = (RGString*) mLadderList.GetFirst ();

	if (name == NULL) {

		Cycled = TRUE;
		return FALSE;
	}

	fileName = *name;
	delete name;
	Cycled = FALSE;
	return TRUE;
}



Boolean SampleNameDirectoryManager :: GetNextOrderedSampleFile (RGString& fileName) {

	RGString* name;

	if (!mReadAllPosControls) {

		name = (RGString*) mPositiveControlList.GetFirst ();

		if (name == NULL) {

			mReadAllPosControls = TRUE;
		}

		else {
			
			fileName = *name;
			delete name;
			return TRUE;
		}
	}

	if (!mReadAllNegControls) {

		name = (RGString*) mNegativeControlList.GetFirst ();

		if (name == NULL) {

			mReadAllNegControls = TRUE;
		}

		else {
			
			fileName = *name;
			delete name;
			return TRUE;
		}
	}

	if (!mReadAllSamples) {

		name = (RGString*) mSampleList.GetFirst ();

		if (name == NULL) {

			mReadAllSamples = TRUE;
		}

		else {
			
			fileName = *name;
			delete name;
			return TRUE;
		}
	}

	return FALSE;
}



