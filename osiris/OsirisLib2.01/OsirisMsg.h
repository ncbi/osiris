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
*  FileName: OsirisMsg.h
*  Author:   Robert Goor
*
*/
//
//  class OsirisMsg assembles messages for output to associated output file
//

#ifndef _OSIRISMSG_H_
#define _OSIRISMSG_H_

#include "rgstring.h"

class RGFile;
class Quadratic;
class DataSignal;

class OsirisMsg {

public:
	OsirisMsg (RGFile* file, const RGString& delimiter, int nlines);
	~OsirisMsg ();

	static void SetDoubleResolution (int newRes) { DoubleResolution = newRes; }
	static int GetDoubleResolution () { return DoubleResolution; }

	void WriteEmptyLine ();
	void WriteFileNumberAndName (int FileNo, const RGString& name);
	void WriteStartDate (const RGString& date);
	void WriteStartTime (const RGString& time);
	void WriteLaneNumber (const RGString& LaneNumber);
	void WriteInsufficientPeaksForILS ();
	void WriteMaximumCorrelationWithILS14 (int offset, double corr, int totalCandidates);
	void WriteInsufficientPeaksForControlSet (int npeaks);
	void WriteInsufficientPeaksForControlSet ();
	void WriteMaximumCorrelationWithControlSet (int offset, double corr, int NumPts, int totalCandidates);

	void WriteQuadraticRegressionFailed (const RGString& forWhat);
	void WriteQuadraticParameters (const RGString& forWhat, Quadratic& polynomial, double maxResid);
	void WriteQuadraticParametersForFile (const RGString& fileName, Quadratic& polynomial, double maxResid);
	void WriteQuadraticRegressionFailedForFile (const RGString& forFile);
	void WriteInsufficientFilesForCrossCorrelation ();

	void WriteLocusName (const RGString& name, int npeaks);

	void WriteCouldNotFindLadderPeakForChannel (int channel);

	void WriteBaseFileForMeanRegression (const RGString& fileName);

	void ResetMessage () { Message = ""; }
	void WriteMessage ();
	void AddMessageFoundDyeBlob (const DataSignal* ds);
	void AddMessageFoundPoorFit (const DataSignal* ds);
	void AddMessageFoundSmallPeak (const DataSignal* ds);
	void AddMessageFoundArtifactCanBeNegative (const DataSignal* ds);
	void AddMessageFoundArtifactSigmaTooLarge (const DataSignal* ds);
	void AddMessageFoundArtifactSigmaTooSmall (const DataSignal* ds);
	void AddMessageFoundArtifactPeakTooSmallAboveMin (const DataSignal* ds);

	void StartLine (int lineNo, const RGString& title, Boolean indent);
	void AddToLine (int lineNo, double data);
	void AddToLine (int lineNo, int data);
	void FlushLines (int HowMany);

	void AddToString (RGString& str, double data);
	void AddToString (RGString& str, const RGString& addend);
	void FlushStrings (RGString* lines, int HowMany, RGFile& file);

protected:
	RGFile* OutputFile;
	RGString Message;
	RGString Delim;
	RGString* Lines;
	int NLines;

	static int DoubleResolution;
	void AddDataToMessage (const DataSignal* ds);
};


#endif   /*  _OSIRISMSG_H_  */


