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
*  FileName: fsaFileData.h
*  Author:   Robert Goor
*
*/
//
//  class fsaFileData:  uses fsaFileInput to read an fsa file and provide references to all of the data.  class fsaFileData
//  returns the data as binary data based on intuitive function calls.  It can create a DataSignal object or a CoreBioSample object.
//

#ifndef _FSAFILEINPUT_H_
#define _FSAFILEINPUT_H_

#include "SampleData.h"
#include "fsaFileInput.h"
#include "rgdefs.h"
#include "rgstring.h"
#include "DataSignal.h"


class fsaFileData : public SampleData {

public:
	fsaFileData (const RGString& fsaFileName);
	~fsaFileData ();

  fsaFileInput *GetFsaFileInput()
  {
    return fsaInput;
  }
	virtual Boolean IsValid () const;

	virtual const char* GetComment ();

	virtual const char* GetInstrumentModelNumber ();
	virtual const char* GetInstrumentName ();
	virtual const char* GetSampleName ();

	virtual int GetNumberOfDataChannels ();
	virtual int GetNumberOfDataElementsForDataChannel (int channelNumber);
	virtual const char* GetDyeNameForDataChannel (int channelNumber);
	virtual const char* GetDyeSetName ();

	virtual DataSignal* GetDataSignalForDataChannel (int channelNumber);
	virtual double* GetDataForDataChannel (int channelNumber);

	virtual DataSignal* GetRawDataSignalForDataChannel (int channelNumber);
	virtual double* GetRawDataForDataChannel (int channelNumber, int& numDataPoints);

	virtual int GetNumberOfDataElementsForAmbientChannel (int channelNumber);
	virtual double* GetDataForAmbientChannel (int channelNumber);

	virtual RGString GetRunStartDate ();
	virtual RGString GetCollectionStartDate ();

	virtual RGString GetRunStartTime ();
	virtual RGString GetCollectionStartTime ();
	virtual RGString GetRunStartXS();

	virtual const char* GetCapillaryNumber ();

	virtual long GetDataOffset ();
	virtual const short* GetRawDataForChannel (int channel);

	virtual const int* GetOffScaleData (int& numElements);
	virtual double* GetMatrix (int& numElements);

private:
	fsaFileInput* fsaInput;
};


#endif  /*  _FSAFILEINPUT_H_  */

