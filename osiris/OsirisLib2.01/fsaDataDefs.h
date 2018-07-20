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
*  FileName: fsaDataDefs.h
*  Author:   Robert Goor
*
*/
//
// Definitions for accessing fsa file data
//

#ifndef _FSAFILEDEFS_H_
#define _FSAFILEDEFS_H_

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
const int AmbientChannelIDBase = 4;
const int AmbientChannelIDLimit = 4;

const char* RunStartDateTag = "RUND";
const int RunStartDateID = 1;

const char* CollectionStartDateTag = "RUND";
const int CollectionStartDateID = 3;

const char* RunStartTimeTag = "RUNT";
const int RunStartTimeID = 1;

const char* CollectionStartTimeTag = "RUNT";
const int CollectionStartTimeID = 3;

const char* LaneTag = "LANE";
const int LaneID = 1;

const char* SampleNameTag = "SMPL";
const int SampleNameIDBase = 0;

const char* GSSampleNameTag = "SpNm";
const int GSSampleNameID = 1;

const char* PeakTag = "PEAK";
const int PeakIDBase = 0;

const char* PeakNumberTag = "PK_#";
const int PeakNumberBase = 0;

const char* RawDataTag = "DATA";
const int RawDataBase = 0;
const int RawDataLimit = 4;
const int RawDataSecondaryBase = 100;

const char* DataOffsetTag = "AOFF";
const int DataOffsetID = 1;

const char* GelExpirationDateTag = "SMED";
const int GelExpirationDateID = 1;

const char* OffScaleDataTag = "OfSc";
const int OffScaleDataID = 1;

const char* MatrixTag = "MTRX";
const int MatrixID = 4;

const char* CommentsNameTag = "CMNT";
const int CommentsTagID = 1;


#endif  /*  _FSAFILEDEFS_H_  */

