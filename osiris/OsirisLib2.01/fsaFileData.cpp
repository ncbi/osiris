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
*  FileName: fsaFileData.cpp
*  Author:   Robert Goor
*
*/
//
//  class fsaFileData:  uses fsaFileInput to read an fsa file and provide references to all of the data.  class fsaFileData
//  returns the data as binary data based on intuitive function calls.  It can create a DataSignal object or a CoreBioSample object.
//

#include "fsaFileData.h"
#include "fsaDataDefs.h"

/*
const char* ABIModelNumberTag = "MODL";
const int ABIModelNumberID = 1;

const char* InstrumentNameTag = "MCHN";
const int InstrumentNameID = 1;

const char* NumberOfDataChannelsTag = "Dye#";
const int NumberOfDataChannelsID = 1;

const char* DataChannelElementsTag = "";
const int DataChannelElementsIDBase = 1;

const char* DyeNameForChannelTag = "DyeN";
const int DyeNameForChannelIDBase = 0;

const char* DyeSetNameTag = "DySN";
const int DyeSetNameID = 1;

const char* DataChannelTag = "DATA";
const int DataChannelIDBase = 8;
const int DataChannelPrimaryIDLimit = 4;
const int DataChannelSecondaryIDBase = 200;

const char* AmbientChannelTag = "DATA";
const int AmbientChannelIDBase = 0;
const int AmbientChannelIDLimit = 4;
*/


fsaFileData :: fsaFileData (const RGString& fsaFileName) : SampleData (fsaFileName) {

	fsaInput = new fsaFileInput (fsaFileName);
}


fsaFileData :: ~fsaFileData () {

	delete fsaInput;
}


Boolean fsaFileData :: IsValid () const {

	return fsaInput->isValid ();
}


const char* fsaFileData :: GetComment () {

	fsaDirEntry* entry = fsaInput->FindDirEntry (CommentsNameTag, CommentsTagID);

	if (entry == NULL)
		return "";

	return entry->GetDataString ();
}


const char* fsaFileData :: GetInstrumentModelNumber () {

	fsaDirEntry* entry = fsaInput->FindDirEntry (ABIModelNumberTag, ABIModelNumberID);

	if (entry == NULL)
		return "";

	return entry->GetDataString ();
}


const char* fsaFileData :: GetInstrumentName () {

	fsaDirEntry* entry = fsaInput->FindDirEntry (InstrumentNameTag, InstrumentNameID);

	if (entry == NULL)
		return "";

	return entry->GetDataString ();
}


const char* fsaFileData :: GetSampleName () {

	fsaDirEntry* entry;
	entry = fsaInput->FindDirEntry (GSSampleNameTag, GSSampleNameID);

	if (entry == NULL)
		return "";

	return entry->GetDataString ();
}


int fsaFileData :: GetNumberOfDataChannels () {

	const INT16* data;
	fsaDirEntry* entry = fsaInput->FindDirEntry (NumberOfDataChannelsTag, NumberOfDataChannelsID);

	if (entry == NULL)
		return 0;

	data = entry->GetDataINT16();

	if (data == NULL)
		return 0;

	return *data;
}


int fsaFileData :: GetNumberOfDataElementsForDataChannel (int channelNumber) {

	int ID;

	if (channelNumber > DataChannelPrimaryIDLimit)
		ID = DataChannelSecondaryIDBase + channelNumber;

	else
		ID = DataChannelIDBase + channelNumber;

	fsaDirEntry* entry = fsaInput->FindDirEntry (DataChannelTag, ID);

	if (entry == NULL)
		return 0;

	return (int)entry->NumElements ();
}


const char* fsaFileData :: GetDyeNameForDataChannel (int channelNumber) {

	int ID = DyeNameForChannelIDBase + channelNumber;
	fsaDirEntry* entry = fsaInput->FindDirEntry (DyeNameForChannelTag, ID);

	if (entry == NULL)
		return "";

	return entry->GetDataString ();
}


const char* fsaFileData :: GetDyeSetName () {

	fsaDirEntry* entry = fsaInput->FindDirEntry (DyeSetNameTag, DyeSetNameID);

	if (entry == NULL)
		return "";

	return entry->GetDataString ();
}


DataSignal* fsaFileData :: GetDataSignalForDataChannel (int channelNumber) {

	double* data;
	int ID;

	if (channelNumber > DataChannelPrimaryIDLimit)
		ID = DataChannelSecondaryIDBase + channelNumber;

	else
		ID = DataChannelIDBase + channelNumber;

	fsaDirEntry* entry = fsaInput->FindDirEntry (DataChannelTag, ID);

	if (entry == NULL)
		return NULL;

	int N = (int)entry->NumElements ();

	if (N <= 1)
		return NULL;

	const INT16* intData = entry->GetDataINT16 ();

	if (intData == NULL)
		return NULL;

	data = new double [N];

	for (int i=0; i<N; i++)
		data[i] = (double)intData [i];

	double spacing = 1.0;
	DataSignal* ds = new SampledData (N, 0.0, (N - 1) * spacing, data);
	return ds;
}


double* fsaFileData :: GetDataForDataChannel (int channelNumber) {

	double* data;
	int ID;

	if (channelNumber > DataChannelPrimaryIDLimit)
		ID = DataChannelSecondaryIDBase + channelNumber;

	else
		ID = DataChannelIDBase + channelNumber;

	fsaDirEntry* entry = fsaInput->FindDirEntry (DataChannelTag, ID);

	if (entry == NULL)
		return NULL;

	int N = (int)entry->NumElements ();
	const INT16* intData = entry->GetDataINT16 ();

	if (intData == NULL)
		return NULL;

	data = new double [N];

	for (int i=0; i<N; i++)
		data[i] = (double)intData [i];

	return data;
}


DataSignal* fsaFileData :: GetRawDataSignalForDataChannel (int channelNumber) {

	double* data;
	int ID;

	if (channelNumber > RawDataLimit)
		ID = RawDataSecondaryBase + channelNumber;

	else
		ID = RawDataBase + channelNumber;

	fsaDirEntry* entry = fsaInput->FindDirEntry (RawDataTag, ID);

	if (entry == NULL)
		return NULL;

	int N = (int)entry->NumElements ();
	const INT16* intData = entry->GetDataINT16 ();

	if (intData == NULL)
		return NULL;

//	intData += GetDataOffset ();  //This skips the "bad" stuff but gives an array mismatch
	data = new double [N];

	for (int i=0; i<N; i++)
		data[i] = (double)intData [i];

	double spacing = 1.0;
	DataSignal* ds = new SampledData (N, 0.0, (N - 1) * spacing, data);
	return ds;
}


double* fsaFileData :: GetRawDataForDataChannel (int channelNumber, int& numDataPoints) {

	double* data;
	int ID;

	if (channelNumber > RawDataLimit)
		ID = RawDataSecondaryBase + channelNumber;

	else
		ID = RawDataBase + channelNumber;

	fsaDirEntry* entry = fsaInput->FindDirEntry (RawDataTag, ID);

	if (entry == NULL)
		return NULL;

	numDataPoints = (int)entry->NumElements ();
	const INT16* intData = entry->GetDataINT16 ();

	if (intData == NULL)
		return NULL;

//	intData += GetDataOffset ();  //This skips the "bad" stuff but gives an array mismatch
	data = new double [numDataPoints];

	for (int i=0; i<numDataPoints; i++)
		data[i] = (double)intData [i];

	return data;
}


int fsaFileData :: GetNumberOfDataElementsForAmbientChannel (int channelNumber) {

	int ID;

	if (channelNumber > AmbientChannelIDLimit)
		return 0;

	ID = channelNumber + AmbientChannelIDBase;

	fsaDirEntry* entry = fsaInput->FindDirEntry (AmbientChannelTag, ID);

	if (entry == NULL)
		return 0;

	return (int)entry->NumElements ();
}


double* fsaFileData :: GetDataForAmbientChannel (int channelNumber) {

	int ID;
	double* data;

	if (channelNumber > AmbientChannelIDLimit)
		return 0;

	ID = channelNumber + AmbientChannelIDBase;

	fsaDirEntry* entry = fsaInput->FindDirEntry (AmbientChannelTag, ID);

	if (entry == NULL)
		return NULL;

	int N = (int)entry->NumElements ();

	const INT16* intData = entry->GetDataINT16 ();

	if (intData == NULL)
		return NULL;

	data = new double [N];

	for (int i=0; i<N; i++)
		data[i] = (double)intData [i];

	return data;
}

RGString fsaFileData :: GetRunStartXS ()
{
  RGString sRtn;
  fsaDirEntry *peDate = 
    fsaInput->FindDirEntry (RunStartDateTag, RunStartDateID);
  fsaDirEntry *peTime =
    fsaInput->FindDirEntry (RunStartTimeTag, RunStartTimeID);
  const fsaDate *pDate = (peDate == NULL) ? NULL : peDate->GetDataDate();
  const fsaTime *pTime = (peTime == NULL) ? NULL : peTime->GetDataTime();
  if(pDate != NULL && pTime != NULL)
  {
    sRtn = fsaDirEntry::FormatDateTimeXS(pDate,pTime);
  }
  return sRtn;
}


RGString fsaFileData :: GetRunStartDate () {

	const fsaDate* data;
	RGString Answer;
	fsaDirEntry* entry = fsaInput->FindDirEntry (RunStartDateTag, RunStartDateID);

	if (entry == NULL)
		return Answer;

	data = entry->GetDataDate();

	if (data == NULL)
		return Answer;

	RGString WorkString;
	WorkString.Convert (data->year, 10);
	Answer = WorkString + ":";
	WorkString.Convert (data->month, 10);
	Answer += WorkString + ":";
	WorkString.Convert (data->day, 10);
	Answer += WorkString;

	return Answer;
}


RGString fsaFileData :: GetCollectionStartDate () {

	const fsaDate* data;
	RGString Answer;
	fsaDirEntry* entry = fsaInput->FindDirEntry (CollectionStartDateTag, CollectionStartDateID);

	if (entry == NULL)
		return Answer;

	data = entry->GetDataDate();

	if (data == NULL)
		return Answer;

	RGString WorkString;
	WorkString.Convert (data->year, 10);
	Answer = WorkString + ":";
	WorkString.Convert (data->month, 10);
	Answer += WorkString + ":";
	WorkString.Convert (data->day, 10);
	Answer += WorkString;

	return Answer;
}


RGString fsaFileData :: GetRunStartTime () {

	const fsaTime* data;
	RGString Answer;
	fsaDirEntry* entry = fsaInput->FindDirEntry (RunStartTimeTag, RunStartTimeID);

	if (entry == NULL)
		return Answer;

	data = entry->GetDataTime();

	if (data == NULL)
		return Answer;

	RGString WorkString;
	WorkString.Convert (data->hour, 10);
	Answer = WorkString + ":";
	WorkString.Convert (data->minute, 10);
	Answer += WorkString + ":";
	WorkString.Convert (data->second, 10);
	Answer += WorkString + ":";
	WorkString.Convert (data->hsecond, 10);
	Answer += WorkString;

	return Answer;
}


RGString fsaFileData :: GetCollectionStartTime () {

	const fsaTime* data;
	RGString Answer;
	fsaDirEntry* entry = fsaInput->FindDirEntry (CollectionStartTimeTag, CollectionStartTimeID);

	if (entry == NULL)
		return Answer;

	data = entry->GetDataTime();

	if (data == NULL)
		return Answer;

	RGString WorkString;
	WorkString.Convert (data->hour, 10);
	Answer = WorkString + ":";
	WorkString.Convert (data->minute, 10);
	Answer += WorkString + ":";
	WorkString.Convert (data->second, 10);
	Answer += WorkString + ":";
	WorkString.Convert (data->hsecond, 10);
	Answer += WorkString;

	return Answer;
}


const char* fsaFileData :: GetCapillaryNumber () {

	RGString data;
	fsaDirEntry* entry = fsaInput->FindDirEntry (LaneTag, LaneID);

	if (entry == NULL)
		return "";

	data = entry->DataToString();
	return data.GetData ();
}


long fsaFileData :: GetDataOffset () {

	RGString data;
	fsaDirEntry* entry = fsaInput->FindDirEntry (DataOffsetTag, DataOffsetID);

	if (entry == NULL)
		return 0;

	long* AnsPtr = (long*)entry->GetDataINT32 ();

	if (AnsPtr == NULL)
		return 0;

	return *AnsPtr;
}


const short* fsaFileData :: GetRawDataForChannel (int channel) {

	int ID;

	if (channel > RawDataLimit)
		ID = RawDataSecondaryBase + channel;

	else
		ID = RawDataBase + channel;

	fsaDirEntry* entry = fsaInput->FindDirEntry (RawDataTag, ID);

	if (entry == NULL)
		return NULL;

	return (const short*) entry->GetDataINT16 ();;
}


const int* fsaFileData :: GetOffScaleData (int& numElements) {

	const int* data;
	fsaDirEntry* entry = fsaInput->FindDirEntry (OffScaleDataTag, OffScaleDataID);

	if (entry == NULL)
		return NULL;

	data = (const int*) entry->GetDataINT32();
	numElements = (int)entry->NumElements ();
	return data;
}


double* fsaFileData :: GetMatrix (int& numElements) {

	const short* matrix;
	double scale = 0.001;
	fsaDirEntry* entry = fsaInput->FindDirEntry (MatrixTag, MatrixID);

	if (entry == NULL)
		return NULL;

	matrix = (const short*) entry->GetDataINT16();

	if (matrix == NULL)
		return NULL;

	numElements = (int)entry->NumElements ();
	double* data = new double [numElements];
	int i;

	for (i=0; i<numElements; i++)
		data [i] = scale * ((double) matrix [i]);
	
	return data;
}


