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
*  FileName: SampleData.cpp
*  Author:   Robert Goor
*
*/
//
//     Abstract base class for sample data based on file input
//

#include "SampleData.h"

SampleData :: SampleData (const RGString& FileName) : mSampleFileName (FileName) {

}


SampleData :: ~SampleData () {

}


Boolean SampleData :: IsValid () const {

	return FALSE;
}


const char* SampleData :: GetComment () {

	return "";
}


const char* SampleData :: GetInstrumentModelNumber () {

	return "";
}


const char* SampleData :: GetInstrumentName () {

	return "";
}

int SampleData :: GetNumberOfDataChannels () {

	return 0;
}


int SampleData :: GetNumberOfDataElementsForDataChannel (int channelNumber) {

	return 0;
}


const char* SampleData :: GetDyeNameForDataChannel (int channelNumber) {

	return "";
}


const char* SampleData :: GetDyeSetName () {

	return "";
}


DataSignal* SampleData :: GetDataSignalForDataChannel (int channelNumber) {

	return NULL;
}


double* SampleData :: GetDataForDataChannel (int channelNumber) {

	return NULL;
}


DataSignal* SampleData :: GetRawDataSignalForDataChannel (int channelNumber) {

	return NULL;
}


double* SampleData :: GetRawDataForDataChannel (int channelNumber, int& numDataPoints) {

	return NULL;
}


int SampleData :: GetNumberOfDataElementsForAmbientChannel (int channelNumber) {

	return 0;
}


double* SampleData :: GetDataForAmbientChannel (int channelNumber) {

	return NULL;
}



RGString SampleData :: GetRunStartDate () {

	return RGString ("");
}


RGString SampleData :: GetCollectionStartDate () {

	return RGString ("");
}


RGString SampleData :: GetRunStartTime () {

	return RGString ("");
}


RGString SampleData :: GetCollectionStartTime () {

	return RGString ("");
}


const char* SampleData :: GetCapillaryNumber () {

	return RGString ("");
}


const int SampleData :: GetInjectionSeconds () {

	return 0;
}


const int SampleData :: GetInjectionVoltage () {

	return 0;
}


long SampleData :: GetDataOffset () {

	return -1;
}


const short* SampleData :: GetRawDataForChannel (int channel) {

	return NULL;
}


const int* SampleData :: GetOffScaleData (int& numElements) {

	return NULL;
}


double* SampleData :: GetMatrix (int& numElements) {

	return NULL;
}


