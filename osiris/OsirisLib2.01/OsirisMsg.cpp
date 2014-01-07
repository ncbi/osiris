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
*  FileName: OsirisMsg.cpp
*  Author:   Robert Goor
*
*/
//
//  class OsirisMsg assembles messages for output to associated output file
//

#include "OsirisMsg.h"
#include "rgfile.h"
#include "Quadratic.h"
#include "DataSignal.h"
#include "OsirisPosix.h"
#include <stdlib.h>


int OsirisMsg::DoubleResolution = 10;


OsirisMsg :: OsirisMsg (RGFile* file, const RGString& delimiter, int nlines) : OutputFile (file), Delim (delimiter),
NLines (nlines) {

	Lines = new RGString [nlines];
}


OsirisMsg :: ~OsirisMsg () {

	delete[] Lines;
}


void OsirisMsg :: WriteEmptyLine () {

	RGString Empty;
	Empty.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteFileNumberAndName (int FileNo, const RGString& name) {

	RGString FileNumber;
	FileNumber.Convert (FileNo, 10);
	RGString Line = "File Number:  " + FileNumber + " with name:  " + name;
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteStartDate (const RGString& date) {

	RGString Line = "Start date for data collection:  " + date;
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteStartTime (const RGString& time) {

	RGString Line = "Start time for data collection:  " + time;
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteLaneNumber (const RGString& LaneNumber) {

	RGString Line = "Lane number for data collection:  " + LaneNumber;
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteInsufficientPeaksForILS () {

	RGString Line = "Insufficiently many peaks for ILS";
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteMaximumCorrelationWithILS14 (int offset, double corr, int totalCandidates) {

	RGString Line = Delim + Delim + "Maximum correlation with ideal ILS (Total candidates = ";
	char buffer [30];
	Line += OsirisItoa (totalCandidates, buffer, 10);
//	Line += ") for curves ";
//	Line += OsirisItoa (offset + 1, buffer, 10);
//	Line += " - ";
//	Line += OsirisItoa (offset + 14, buffer, 10);
	Line += ") is:  ";
	Line += OsirisGcvt (corr, OsirisMsg::DoubleResolution, buffer);
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteInsufficientPeaksForControlSet (int npeaks) {

	char buffer [30];
	RGString Line = "Insufficiently many peaks (";
	Line += OsirisItoa (npeaks, buffer, 10);
	Line += ") for Control Set";
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteInsufficientPeaksForControlSet () {

	RGString Line = "Insufficiently many peaks for Control Set";
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteMaximumCorrelationWithControlSet (int offset, double corr, int NumPts, int totalCandidates) {

	RGString Line = Delim + Delim + "Maximum correlation with ideal Control Set (Total Candidates = ";
	char buffer [30];
	Line += OsirisItoa (totalCandidates, buffer, 10);
	Line += ") for curves ";
	Line += OsirisItoa (offset + 1, buffer, 10);
	Line += " - ";
	Line += OsirisItoa (offset + NumPts, buffer, 10);
	Line += " is:  ";
	Line += OsirisGcvt (corr, OsirisMsg::DoubleResolution, buffer);
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteQuadraticRegressionFailed (const RGString& forWhat) {

	RGString Line = Delim + Delim + "Quadratic regression for " + forWhat + " failed:  autocorrelation ";
	Line += "matrix is singular";
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteQuadraticParameters (const RGString& forWhat, Quadratic& polynomial, double maxResid) {

	char buffer [30];
	RGString Line = Delim + Delim + "Quadratic parameters for " + forWhat + " (quadratic, linear, constant) ";
	Line += "and Maximum (fractional) Residual";
	Line.WriteTextLine (*OutputFile);
	Line = Delim + Delim + Delim + OsirisGcvt (polynomial ((int)0), OsirisMsg::DoubleResolution, buffer) + Delim;
	Line += OsirisGcvt (polynomial ((int)1), OsirisMsg::DoubleResolution, buffer) + Delim;
	Line += OsirisGcvt (polynomial ((int)2), OsirisMsg::DoubleResolution, buffer) + Delim;
	Line += OsirisGcvt (maxResid, OsirisMsg::DoubleResolution, buffer);
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteQuadraticParametersForFile (const RGString& fileName, Quadratic& polynomial, double maxResid) {

	char buffer [30];
	RGString Line = Delim + Delim + "Quadratic parameters for file " + fileName + " (quadratic, linear, constant) ";
	Line += "and Maximum (fractional) Residual";
	Line.WriteTextLine (*OutputFile);
	Line = Delim + Delim + Delim + OsirisGcvt (polynomial ((int)0), OsirisMsg::DoubleResolution, buffer) + Delim;
	Line += OsirisGcvt (polynomial ((int)1), OsirisMsg::DoubleResolution, buffer) + Delim;
	Line += OsirisGcvt (polynomial ((int)2), OsirisMsg::DoubleResolution, buffer) + Delim;
	Line += OsirisGcvt (maxResid, OsirisMsg::DoubleResolution, buffer);
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteQuadraticRegressionFailedForFile (const RGString& forFile) {

	RGString Line = Delim + Delim + "Could not perform quadratic regression for file:  " + forFile;
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteInsufficientFilesForCrossCorrelation () {

	RGString Line = "Insufficiently many files to perform cross correlations";
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteLocusName (const RGString& name, int npeaks) {

	char buffer [30];
	RGString Line = Delim + "Locus name " + name + " with number of peaks ";
	Line += OsirisItoa (npeaks, buffer, 10);
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteCouldNotFindLadderPeakForChannel (int channel) {

	RGString Channel;
	Channel.Convert (channel, 10);
	RGString Line = "Could not find ladder peak for channel " + Channel;
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteBaseFileForMeanRegression (const RGString& fileName) {

	RGString Line = "Base file for quadratic regression of means vs means:  " + fileName;
	Line.WriteTextLine (*OutputFile);
}


void OsirisMsg :: WriteMessage () {

	Message.WriteTextLine (*OutputFile);
	Message = "";
}


void OsirisMsg :: AddMessageFoundDyeBlob (const DataSignal* ds) {

	Message += "Found dye blob with (Mean, Sigma, Peak, Fit):  " + Delim + Delim + Delim + Delim + Delim + Delim;
	AddDataToMessage (ds);
}


void OsirisMsg :: AddMessageFoundPoorFit (const DataSignal* ds) {

	Message += "Found dye blob with (Mean, Sigma, Peak, Fit):  " + Delim + Delim + Delim + Delim + Delim + Delim;
	AddDataToMessage (ds);
}



void OsirisMsg :: AddMessageFoundSmallPeak (const DataSignal* ds) {

	Message += "Found peak below threshold with (Mean, Sigma, Peak, Fit):  " + Delim + Delim + Delim + Delim + Delim + Delim;
	AddDataToMessage (ds);
}


void OsirisMsg :: AddMessageFoundArtifactCanBeNegative (const DataSignal* ds) {

	Message += "Found peak that goes negative with (Mean, Sigma, Peak, Fit):  " + Delim + Delim + Delim + Delim + Delim + Delim;
	AddDataToMessage (ds);
}


void OsirisMsg :: AddMessageFoundArtifactSigmaTooLarge (const DataSignal* ds) {

	Message += "Found peak that is too wide with (Mean, Sigma, Peak, Fit):  " + Delim + Delim + Delim + Delim + Delim + Delim;
	AddDataToMessage (ds);
}


void OsirisMsg :: AddMessageFoundArtifactSigmaTooSmall (const DataSignal* ds) {

	Message += "Found peak that is too narrow with (Mean, Sigma, Peak, Fit):  " + Delim + Delim + Delim + Delim + Delim + Delim;
	AddDataToMessage (ds);
}


void OsirisMsg :: AddMessageFoundArtifactPeakTooSmallAboveMin (const DataSignal* ds) {

	Message += "Found peak that is too low with (Mean, Sigma, Peak, Fit):  " + Delim + Delim + Delim + Delim + Delim + Delim;
	AddDataToMessage (ds);
}


void OsirisMsg :: StartLine (int lineNo, const RGString& title, Boolean indent) {

	if (indent)
		Lines [lineNo] = Delim + title + Delim;

	else
		Lines [lineNo] = title + Delim;
}


void OsirisMsg :: AddToLine (int lineNo, double data) {

	char buffer [30];
	Lines [lineNo] += OsirisGcvt (data, OsirisMsg::DoubleResolution, buffer) + Delim;
}



void OsirisMsg :: AddToLine (int lineNo, int data) {

	char buffer [30];
	Lines [lineNo] += OsirisItoa (data, buffer, 10) + Delim;
}



void OsirisMsg :: FlushLines (int HowMany) {

	for (int i=0; i<HowMany; i++)
		Lines [i].WriteTextLine (*OutputFile);
}


void OsirisMsg :: AddToString (RGString& str, double data) {

	char buffer [30];
	str += OsirisGcvt (data, OsirisMsg::DoubleResolution, buffer) + Delim;
}


void OsirisMsg :: AddToString (RGString& str, const RGString& addend) {

	str += addend + Delim;
}



void OsirisMsg :: FlushStrings (RGString* lines, int HowMany, RGFile& file) {

	for (int i=0; i<HowMany; i++)
		lines [i].WriteTextLine (file);
}


void OsirisMsg :: AddDataToMessage (const DataSignal* ds) {

	char buff [30];
	double left = ds->LeftEndPoint ();
	double right = ds->RightEndPoint ();
	double Where = ds->Centroid ();
	double Width = sqrt (ds->SecondMoment (left, right));
	double Peak = ds->Peak ();
	double fit = ds->GetCurveFit ();
	Message += OsirisGcvt (Where, OsirisMsg::DoubleResolution, buff) + Delim;
	Message += OsirisGcvt (Width, OsirisMsg::DoubleResolution, buff) + Delim;
	Message += OsirisGcvt (Peak, OsirisMsg::DoubleResolution, buff) + Delim;
	Message += OsirisGcvt (fit, OsirisMsg::DoubleResolution, buff);
	Message += "\n";
}


