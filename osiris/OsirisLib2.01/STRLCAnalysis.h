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
*  FileName: STRLCAnalysis.h
*  Author:   Robert Goor
*
*/
//
//  class STRLCAnalysis has global functions which analyze digital data signals according to specified rules, using the locus-centric classes
//

#ifndef _STRLCANALYSIS_H_
#define _STRLCANALYSIS_H_

#include "rgstring.h"
#include "rgdlist.h"
#include "Genetics.h"
#include "Notice.h"
#include "rgpersist.h"
#include "SmartMessagingObject.h"
#include "FailureMessages.h"

#include <string>
#include <math.h>
#include <list>

using namespace std;

class SmartMessage;
class SmartNotice;

const int _STRLCANALYSIS_ = 1507;

PERSISTENT_PREDECLARATION (STRLCAnalysis)


class STRLCAnalysis : public SmartMessagingObject {

	PERSISTENT_DECLARATION (STRLCAnalysis)

public:
	STRLCAnalysis ();
	STRLCAnalysis (PopulationCollection* collection, const RGString& parentDirectoryForReports, const RGString& messageBookPath);  // Legacy message constructor
	STRLCAnalysis (PopulationCollection* collection, const RGString& parentDirectoryForReports, const RGString& messageBookPath, bool isSmartMsgAnalysis);  // Smart message constructor
	STRLCAnalysis (const RGString& ladderInformationDirectory, const RGString& markerSetName, const RGString& parentDirectoryForReports, const RGString& messageBookPath, bool isSmartMsgAnalysis);  // Smart message constructor2
	STRLCAnalysis (const RGString& ladderInformationDirectory, const RGString& markerSetName, const RGString& parentDirectoryForReports, const RGString& messageBookPath, bool isSmartMsgAnalysis, bool isFileInput);  // Smart message constructor3
	virtual ~STRLCAnalysis ();

	static MainMessages* mFailureMessage;

	// Non-message functions***************************************************************************************************************************
	// ************************************************************************************************************************************************

	bool isValid () const { return mValid; }
	void SetXMLCommandLineString (const RGString& cmdString) { mCommandLineString = cmdString; }
	void SetTableLink (int linkNumber);

	static void SetMaximumNumberOfChannels (int maxChannels) { mMaximumNumberOfChannels = maxChannels; }
	static int GetMaximumNumberOfChannels () { return mMaximumNumberOfChannels; }

	static void SetMsgBookBuildTime (const RGString& buildTime) { MsgBookBuildTime = buildTime; }
	static RGString GetMsgBookBuildTime () { return MsgBookBuildTime; }

	// Legacy message functions************************************************************************************************************************
	// ************************************************************************************************************************************************

	int Analyze (const RGString& prototypeInputDirectory, const RGString& markerSet, int outputLevel, const RGString& graphicsDirectory);	// Obsolete - not converting
	int AnalyzeIncrementally (const RGString& prototypeInputDirectory, const RGString& markerSet, int outputLevel, const RGString& graphicsDirectory);	//$

	int AddNoticeToList (Notice* newNotice);	//$
	Boolean IsNoticeInList (const Notice* target);	//$
	Notice* GetNotice (const Notice* target);	//$
	Boolean ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim);	//$
	Boolean ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink);	//$
	Boolean ReportXMLNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);	//$
	void ClearNoticeObjects ();	//$
	int NumberOfNoticeObjects ();	//$

	// Smart message functions*************************************************************************************************************************
	// ************************************************************************************************************************************************

	int AnalyzeIncrementallySM (const RGString& prototypeInputDirectory, const RGString& markerSet, int outputLevel, const RGString& graphicsDirectory, const RGString& commandInputs);
	int AnalyzeIncrementallySMLF (const RGString& prototypeInputDirectory, const RGString& markerSet, int outputLevel, const RGString& graphicsDirectory, const RGString& commandInputs);

	int GetObjectScope () const { return 6; }

	bool EvaluateSmartMessagesForStage (int stage);
	bool EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage);
	bool SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage);
	bool SetTriggersForAllMessages (bool* const higherMsgMatrix, int stage);
	bool EvaluateAllReports (bool* const reportMatrix);
	bool TestAllMessagesForCall ();
	bool EvaluateAllReportLevels (int* const reportLevelMatrix);
	int NumberOfSmartNoticeObjects () const { return mSmartMessageReporters->Entries (); }

	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);
	virtual int AddAllSmartMessageReporters ();
	virtual int AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects);

	virtual void InitializeMessageData ();

	Boolean ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim);
	Boolean ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink);
	Boolean ReportXMLSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);

	static void CreateInitializationData (int scope);
	static void InitializeMessageMatrix (bool* matrix, int size);
	static void ClearInitializationMatrix () { delete[] InitialMatrix; InitialMatrix = NULL; }
	static int GetScope () { return 6; }
	static void CreateAllInitializationMatrices ();
	static void ClearAllInitializationMatrices ();
	static void SetOverrideString (const RGString& str) { OverrideString = str; }
	static bool OverrideStringIsEmpty () { return (OverrideString.Length () == 0); }
	static RGString GetOverrideString () { return OverrideString; }

	static void SetOutputSubDirectory (const RGString& subDir) { OutputSubDirectory = subDir; }
	static RGString GetOutputSubDirectory () { return OutputSubDirectory; }
	static void SetCollectILSHistory (bool s) { CollectILSHistory = s; }

	static void ResetDirectoryCrashMode (bool m) {
		DirectoryCrashMode = m;
	}
	static bool GetDirectoryCrashMode () {
		return DirectoryCrashMode;
	}

	static void AddInvalidFile (const RGString& name);
	static bool FileNameIsInInvalidList (const RGString& name);

protected:
	PopulationCollection* mCollection;
	RGString mParentDirectoryForReports;
	RGString mCurrentInputDirectory;
	RGString mMessageBookName;
	//int mHighestSeverityLevel;
	//int mHighestMessageLevel;

	RGDList NewNoticeList;
	RGString mTableLink;
	bool mValid;
	RGString mCommandLineString;

	RGString WorkingFileName;
	RGFile* WorkingFile;
	RGDList mBaseLocusList;	// This is the list of BaseLoci - the directory level equivalent of Locus links

	// Smart Message Data*****************************************************************************************************************
	//************************************************************************************************************************************

	//bool* mMessageArray;
	//RGHashTable* mMessageDataTable;
	//RGDList* mSmartMessageReporters;

	//************************************************************************************************************************************
	//************************************************************************************************************************************

	void PreInitializeSmartMessages ();
	//void InitializeSmartMessages ();
	//void CleanupSmartMessages ();

	static int mMaximumNumberOfChannels;
	static bool* InitialMatrix;
	static RGString OverrideString;
	static RGString OutputSubDirectory;
	static RGString MsgBookBuildTime;
	static bool CollectILSHistory;
	static bool DirectoryCrashMode;
	static RGDList InvalidFilesByName;
};


#endif  /*  _STRLCANALYSIS_H_  */

