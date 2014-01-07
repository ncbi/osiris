/*
 * =====================================================================
  
                              PUBLIC DOMAIN NOTICE
                 National Center for Biotechnology Information

    This software/database is a "United States Government Work" under 
    the terms of the United States Copyright Act.  It was written as 
    part of the author's official duties as a United States Government 
    employee and thus cannot be copyrighted.  This software/database is 
    freely available to the public for use. The National Library of 
    Medicine and the U.S. Government have not placed any restriction on 
    its use or reproduction.

    Although all reasonable efforts have been taken to ensure the 
    accuracy and reliability of the software and data, the NLM and the 
    U.S. Government do not and cannot warrant the performance or results 
    that may be obtained by using this software or data. The NLM and 
    the U.S. Government disclaim all warranties, express or implied, 
    including warranties of performance, merchantability or fitness for 
    any particular purpose.

    Please cite the author in any work or product based on this material.

 * =====================================================================
 *
 * Author:  Douglas J. Hoffman
 *          Contractor
 *          National Center For Biotechnology Information
 *          National Library of Medicine
 *          National Institutes of Health
 *
 * File Description:
 *  fsaDirEntry.cpp - class for storing data from Applied Biosystems' fsa files
 *    SEE ALSO: byteOrder.h/cpp; 
 *    example program - ../fsa2xml.cpp
 *
 */
#include <string.h>
#include <stdio.h>
#include "fsaDirEntry.h"
#include "ByteOrder.h"
#include "Exception.h"

// #define __BUG

#define _TYPE_UINT8 1
#define _TYPE_CHAR 2
#define _TYPE_CHAR_BYTE -2
#define _TYPE_UINT16 3
#define _TYPE_INT16 4
#define _TYPE_INT32 5
#define _TYPE_FLOAT 7
#define _TYPE_DOUBLE 8
#define _TYPE_DATE 10
#define _TYPE_TIME 11
#define _TYPE_PSTRING 18
#define _TYPE_CSTRING 19
#define _TYPE_THUMB 12
#define _TYPE_BOOL 13
// USER is 1024 or greater
#define _TYPE_USER 1024

const INT16 fsaDirEntry::TYPE_BYTE = _TYPE_UINT8;
const INT16 fsaDirEntry::TYPE_UINT8 = _TYPE_UINT8;
const INT16 fsaDirEntry::TYPE_CHAR = _TYPE_CHAR;
const INT16 fsaDirEntry::TYPE_WORD = _TYPE_UINT16;
const INT16 fsaDirEntry::TYPE_UINT16 = _TYPE_UINT16;
const INT16 fsaDirEntry::TYPE_SHORT = _TYPE_INT16;
const INT16 fsaDirEntry::TYPE_INT16 = _TYPE_INT16;
const INT16 fsaDirEntry::TYPE_LONG = _TYPE_INT32;
const INT16 fsaDirEntry::TYPE_INT32 = _TYPE_INT32;
const INT16 fsaDirEntry::TYPE_FLOAT = _TYPE_FLOAT;
const INT16 fsaDirEntry::TYPE_DOUBLE = _TYPE_DOUBLE;
const INT16 fsaDirEntry::TYPE_DATE = _TYPE_DATE;
const INT16 fsaDirEntry::TYPE_TIME = _TYPE_TIME;
const INT16 fsaDirEntry::TYPE_PSTRING = _TYPE_PSTRING;
const INT16 fsaDirEntry::TYPE_CSTRING = _TYPE_CSTRING;
const INT16 fsaDirEntry::TYPE_THUMB = _TYPE_THUMB;
const INT16 fsaDirEntry::TYPE_BOOL = _TYPE_BOOL;
const INT16 fsaDirEntry::TYPE_USER = _TYPE_USER;

char fsaDirEntry::g_DateString[24];

bool fsaDirEntry::ValidateName(const UINT8 *ps)
{
  bool bRtn = (ps[4] == 0);
  UINT8 c;

  for(int i = bRtn ? 0 : 4; i < 4; i++)
  {
    c = *ps;
    ps++;
    if(!c)
    {
      bRtn = false;
      i = 4;
    }
    else if( isalnum((int) c) || (strchr("_#",c) != NULL) ) ;
    else
    {
      bRtn = false;
      i = 4;
    }
  }
  return bRtn;
}

void fsaDirEntry::BuildIt
    (const char *_psName, // 4 byte name
    INT32 _nNumber,
    INT16 _nType, // see const INT16 TYPE_...
    INT32 _nNumElements,
    INT16 _nSize,     // use -1 to computer from type
    INT32 _nDataSize, // use -1 to computer from above
    void *_pData,     // this is raw data,
    bool bCopy        // set to true to make a copy of pData.  
                      // pData will be free()d upon destruction regardless of this value
    )
{
  if(ValidateName((UINT8 *) _psName))
  {
    memcpy(sName,_psName,4);
    sName[4] = 0;
    pString = NULL;
    nJUNK = 0;
    nDataOffset = 0;
    nNumber = _nNumber;
    nElementType = _nType;
    nNumElements = _nNumElements;
    nElementSize = (_nSize > 0) ? _nSize : GetElementSize();
    nDataSize = (_nDataSize > 0) ? _nDataSize : (nElementSize * nNumElements);
    if(bCopy)
    {
      AllocateData(false);
      if(pData != NULL)
      {
        memcpy(pData,_pData,(size_t)nDataSize);
      }
    }
    else
    {
      pData = _pData;
    }
    if(nDataSize <= 4 && pData != NULL)
    {
      memcpy(&nDataOffset, pData, (size_t) nDataSize);
    }
    else
    {
      nDataOffset = 0;
    }
  }
  else
  {
    Init();
  }
}

void fsaDirEntry::_Setup(const UINT8	*pEntry, bool	bBig)
{
	if(pEntry	!= NULL)
	{
  	pData	=	NULL;
	  pString = NULL;
		strncpy(sName,(const char *)pEntry,4);
    sName[4] = 0;
		nNumber = ByteOrder::localInt32(pEntry + 4, bBig);
		nElementType = ByteOrder::localInt16(pEntry +	8, bBig);
		nElementSize = ByteOrder::localUint16(pEntry	+	10, bBig);
		nNumElements = ByteOrder::localUint32(pEntry	+	12, bBig);
		nDataSize = ByteOrder::localUint32(pEntry + 16, bBig);
		nDataOffset = ByteOrder::localUint32(pEntry + 20, bBig);
		nJUNK = ByteOrder::localInt32(pEntry + 24, bBig);
	}
}

fsaDirEntry::fsaDirEntry(const UINT8	*pEntry, bool	bBig)
{
  Init();
  _Setup(pEntry,bBig);
}


fsaDirEntry::fsaDirEntry
    (const char *_psName, // 4 byte name
    INT32 _nNumber,
    INT16 _nType, // see const INT16 TYPE_...
    INT32 _nNumElements,
    INT16 _nSize,     // use -1 to computer from type
    INT32 _nDataSize, // use -1 to computer from above
    void *_pData,     // this is raw data,
    bool bCopy        // set to true to make a copy of pData.  
                      // pData will be free()d upon destruction regardless of this value
    )
{
  BuildIt(_psName,_nNumber,_nType,_nNumElements,_nSize,_nDataSize,_pData,bCopy);
}
fsaDirEntry::fsaDirEntry
    (const char *_psName, // 4 byte name
    INT32 _nNumber,
    INT16 _nType, // see const INT16 TYPE_...
    INT32 _nNumElements,
    INT32 _nDataSize, 
    const char *pDataString
    )
{
  void *pData = StringToData(_nType, _nDataSize, pDataString);
  INT16 nTypeAbs = (_nType < 0) ? (-_nType) : _nType;
  INT16 _nElementSize = _nNumElements ? ((INT16)(_nDataSize / _nNumElements)) : 1;
  BuildIt(_psName,_nNumber,nTypeAbs,_nNumElements,_nElementSize,_nDataSize,pData,false);
}


fsaDirEntry::fsaDirEntry(const fsaDirEntry &entry)
{
  pData = NULL;
  pString = NULL;
  (*this) = entry;
}

fsaDirEntry& fsaDirEntry::operator =(const fsaDirEntry &entry)
{
  memcpy(sName,entry.sName,sizeof(sName));
  nNumber = entry.nNumber;
  nNumElements = entry.nNumElements;
  nDataSize = entry.nDataSize;
  nDataOffset = entry.nDataOffset;
  nJUNK = entry.nJUNK;
  nElementType = entry.nElementType;
  nElementSize = entry.nElementSize;
  pString = NULL;
  AllocateData(false);
  if(pData != NULL)
  {
    memcpy(pData,entry.pData,(size_t) nDataSize);
  }
  return (*this);
}

fsaDirEntry::~fsaDirEntry() 
{
	FreeString();
	FreeData();
}

RGString fsaDirEntry::MakeKey(const char *ps, INT32 n)
{
	char ss[32];
	memcpy(ss,ps,4);
	ss[4] = 0;
	strcat(ss,":");
	sprintf(&ss[strlen(ss)],"%d",n);
	RGString sRtn((const char *)ss);
	return sRtn;
}
size_t fsaDirEntry::BytesToRead()
{
	size_t nSize1 = nElementSize * nNumElements;
	size_t nSize2 = nDataSize;
  if(nSize1 < nSize2)
  {
    nSize1 = nSize2;
  }
  return nSize1;
}
size_t fsaDirEntry::AllocateData(bool bThrow, void *pRawData)
{
  size_t nSize = BytesToRead();
	size_t nAlloc = (nSize + 4) & (~3);
  if(nAlloc <= 262144)
  {
	  pData = malloc(nAlloc);
  }
  else
  {
    pData = NULL;
  }
	if(pData == NULL)
	{
		nAlloc = 0;
		if(bThrow)
		{
			throw new Exception("malloc failed");
		}
	}
	else
	{
    if(pRawData != NULL)
    {
      if(nAlloc > nSize)
      {
  		  memset((void *)((UINT8 *)pData + nSize), 0, nAlloc - nSize);
      }
      memcpy(pData,pRawData,nSize);
    }
    else
    {
      memset(pData,0,nAlloc);
    }
	}
	return nAlloc;
}

bool fsaDirEntry::CheckSize(size_t n, bool bThrow, const char *psType)
{
	bool bRtn = (n == (size_t)nElementSize);
	if((!bRtn) && bThrow)
	{
		char s[256];
		snprintf(s,256,"sizeof(%s) is %d, expected %d",
			psType,(int)n,(int)nElementSize);
		throw new Exception((const char *)s);
	}
	return bRtn;
}

void fsaDirEntry::LoadDataThumb(bool bThrow)
{
	const char *psType = GetTypeString();
	CheckSize(sizeof(fsaThumb),bThrow,psType);
	if(!ByteOrder::LocalBigEndian())
	{
		fsaThumb *p = (fsaThumb *) pData;
		for(UINT32 i = 0; i < nNumElements; ++i)
		{
			ByteOrder::reverse(&p->d);
			ByteOrder::reverse(&p->u);
			p++;
		}
	}
}

void fsaDirEntry::LoadDataBytes(size_t n)
{
	if((n > 1) && !ByteOrder::LocalBigEndian())
	{
		ByteOrder::reverse((char *)pData,n,nNumElements);
	}
}

void fsaDirEntry::LoadDataDate(bool bThrow)
{
	const char *psType = GetTypeString();
	CheckSize(sizeof(fsaDate),bThrow,psType);
	if(!ByteOrder::LocalBigEndian())
	{
		fsaDate *p = (fsaDate *) pData;
		for(UINT32 i = 0; i < nNumElements; ++i)
		{
			ByteOrder::reverse(&p->year);
			p++;
		}
	}
}
void fsaDirEntry::LoadDataTime(bool bThrow)
{
	const char *psType = GetTypeString();
	CheckSize(sizeof(fsaTime),bThrow,psType);
}
void fsaDirEntry::SetupEndian(bool bThrow)
{
	int nBytes = 0;		
	switch (nElementType)
	{
	case _TYPE_UINT8:
	case _TYPE_CHAR:
	case _TYPE_PSTRING:
	case _TYPE_CSTRING:
	case _TYPE_BOOL:
		nBytes = 1;
		break;
	case _TYPE_UINT16:
	case _TYPE_INT16:
		nBytes = 2;
		break;
	case _TYPE_FLOAT:
	case _TYPE_INT32:
		nBytes = 4;
		break;
	case _TYPE_DOUBLE:
		nBytes = 8;
		break;
	case _TYPE_DATE:
		LoadDataDate(bThrow);
		break;
	case _TYPE_TIME:
		LoadDataTime(bThrow);
		break;
	case _TYPE_THUMB:
		LoadDataThumb(bThrow);
		break;
	default:
		break;
	}
	if(nBytes > 1)
	{
		LoadDataBytes(nBytes);
	}
}
void fsaDirEntry::LoadData(bool bThrow, IReader *pReader)
{
  if(pData != NULL)
  {
    if(bThrow)
    {
      throw new Exception("fsaDirEntry::LoadData() -- data already loaded");
    }
  }
  else
  {
    size_t nToRead = BytesToRead();
    AllocateData(bThrow);
    bool b = true;
    if(pData == NULL)
    {
      b = false;
    }
    else if(nToRead > 4)
    {
      b = pReader->Read(pData, this->nDataOffset,nToRead);
      if( (!b) && bThrow )
      {
        throw new Exception("fsaDirEntry::LoadData() -- could not read data");
      }
    }
    else
    {
      UINT32 nOffset = nDataOffset;
      if(!ByteOrder::LocalBigEndian())
	    {
  			ByteOrder::reverse(&nOffset);  // it was reversed once, reverse it back
      }
      memcpy(pData,&nOffset,nToRead);
    }
    if(b)
    {
      SetupEndian(bThrow);
    }
  }
}
void fsaDirEntry::LoadData(bool bThrow, void *pRawData, bool bLocalEndian, bool bCopy)
{
	if(pData != NULL) // if already loaded, do nothing
  {
    if(bThrow)
    {
      throw new Exception("fsaDirEntry::LoadData() -- data already loaded");
    }
  }
  else
  {
    if(bCopy)
    {
      AllocateData(bThrow,pRawData);
    }
    else
    {
      pData = pRawData;
    }
	  if((!bLocalEndian) && nElementType <= 1023)
	  {
      SetupEndian(bThrow);
	  }
  }
}

const char *fsaDirEntry::GetTypeString(INT16 n)
{
	const char *psRtn = NULL;
	switch(ElementTypeEnum(n))
	{
		case _TYPE_USER:
			psRtn = "user_defined";
			break;
		case _TYPE_UINT8:
			psRtn = "byte";
			break;
		case _TYPE_CHAR:
			psRtn = "char";
			break;
		case _TYPE_CHAR_BYTE:
			psRtn = "char_byte";
			break;
		case _TYPE_PSTRING:
			psRtn = "pString";
			break;
		case _TYPE_CSTRING:
			psRtn = "cString";
			break;
		case _TYPE_BOOL:
			psRtn = "bool";
			break;
		case _TYPE_UINT16:
			psRtn = "word";
			break;
		case _TYPE_INT16:
			psRtn = "short";
			break;
		case _TYPE_FLOAT:
			psRtn = "float";
			break;
		case _TYPE_INT32:
			psRtn = "long";
			break;
		case _TYPE_DOUBLE:
			psRtn = "double";
			break;
		case _TYPE_DATE:
			psRtn = "date";
			break;
		case _TYPE_TIME:
			psRtn = "time";
			break;
		case _TYPE_THUMB:
			psRtn = "thumb";
			break;
		default:
			psRtn = "unknown";
			break;
	}
	return psRtn;
}
INT16 fsaDirEntry::GetTypeFromString(const RGString &str)
{
  const INT16 list[] =
  {
    // sorted by frequency to make search 'faster', 
    // but a binary search or hash may be better
    _TYPE_INT16,
    _TYPE_PSTRING,
    _TYPE_INT32,
		_TYPE_USER,
    _TYPE_UINT8,
    _TYPE_CSTRING,
    _TYPE_CHAR,
    _TYPE_CHAR_BYTE,
    _TYPE_DATE,
    _TYPE_TIME,
    _TYPE_FLOAT,
    _TYPE_DOUBLE,
    _TYPE_BOOL,
    _TYPE_UINT16,
    _TYPE_THUMB
  };
  const int nLIST = sizeof(list) / sizeof(list[0]);
  const INT16 *pList = list;
  INT16 nRtn = _TYPE_USER;
  int i;
  for(i = 0; i < nLIST; i++)
  {
    if(str == GetTypeString(*pList))
    {
      nRtn = *pList;
      i = nLIST; // loop exit
    }
    else
    {
      pList++;
    }
  }
  if(i >= nLIST && nRtn == _TYPE_USER)
  {
    nRtn++;
  }
  return nRtn;
}

void fsaDirEntry::InvalidType(bool bThrow, const char *psType)
{
	if(bThrow)
	{
		char s[256];
		snprintf(s,256,
			"Cannot retrieve %s for type %s", 
			psType, GetTypeString(nElementType));
	}
}
const UINT8 *fsaDirEntry::GetDataUINT8(bool bThrow)
{
	const UINT8 *pRtn = NULL;
	switch (ElementTypeEnum())
	{
		case _TYPE_UINT8:
		case _TYPE_USER:
		case _TYPE_BOOL:
			pRtn = (const UINT8 *)pData;
			break;
		default:
			InvalidType(bThrow,"UINT8");
			break;
	}
	return pRtn;
}


const char *fsaDirEntry::GetDataString(bool bThrow)
{
	const char *pRtn = (const char *)pData;
	bool bP = false;
	INT16 nType = ElementTypeEnum();
	switch (ElementTypeEnum())
	{
		case _TYPE_PSTRING:
			bP = true;
		case _TYPE_CSTRING:
		case _TYPE_CHAR:
		case _TYPE_USER:
			pRtn = (const char *)pData;
			if(bP) {pRtn++;}
			break;
		default:
			InvalidType(bThrow,"string");
			break;
	}
	return pRtn;
}

const UINT16 *fsaDirEntry::GetDataUINT16(bool bThrow)
{
	UINT16 *pnRtn = NULL;
	if(nElementType == TYPE_UINT16)
	{
		pnRtn = (UINT16 *)pData;
	}
	else
	{
		InvalidType(bThrow,"UINT16");
	}
	return pnRtn;
}

const INT16 *fsaDirEntry::GetDataINT16(bool bThrow)
{
	INT16 *pnRtn = NULL;
	if(nElementType == TYPE_INT16)
	{
		pnRtn = (INT16 *)pData;
	}
	else
	{
		InvalidType(bThrow,"INT16");
	}
	return pnRtn;
}

const INT32 *fsaDirEntry::GetDataINT32(bool bThrow)
{
	INT32 *pnRtn = NULL;
	if(nElementType == TYPE_INT32)
	{
		pnRtn = (INT32 *) pData;
	}
	else
	{
		InvalidType(bThrow,"INT32");
	}
	return pnRtn;
}

const float *fsaDirEntry::GetDataFloat(bool bThrow)
{
	float *pfRtn = NULL;
	if(nElementType == TYPE_FLOAT)
	{
		pfRtn = (float *) pData;
	}
	else
	{
		InvalidType(bThrow,"float");
	}
	return pfRtn;
}

const double *fsaDirEntry::GetDataDouble(bool bThrow)
{
	double *pdRtn = NULL;
	if(nElementType == TYPE_DOUBLE)
	{
		pdRtn = (double *) pData;
	}
	else
	{
		InvalidType(bThrow,"double");
	}
	return pdRtn;
}
const fsaDate *fsaDirEntry::GetDataDate(bool bThrow)
{
	fsaDate *pRtn = NULL;
	if(nElementType == TYPE_DATE)
	{
		pRtn = (fsaDate *) pData;
	}
	else
	{
		InvalidType(bThrow,"fsaDate");
	}
	return pRtn;
}

const fsaTime *fsaDirEntry::GetDataTime(bool bThrow)
{
	const fsaTime *pRtn = NULL;
	if(nElementType == TYPE_TIME)
	{
		pRtn = (fsaTime *) pData;
	}
	else
	{
		InvalidType(bThrow,"fsaTime");
	}
	return pRtn;
}

const fsaThumb *fsaDirEntry::GetDataThumb(bool bThrow)
{
	const fsaThumb *pRtn = NULL;
	if(nElementType == TYPE_THUMB)
	{
		pRtn = (fsaThumb *)pData;
	}
	else
	{
		InvalidType(bThrow,"fsaThumb");
	}
	return pRtn;
}

const fsaBOOL *fsaDirEntry::GetDataBool(bool bThrow)
{
	const fsaBOOL *pRtn = NULL;
	if(nElementType == TYPE_BOOL)
	{
		pRtn = (const fsaBOOL *)GetDataUINT8(bThrow);
	}
	else
	{
		InvalidType(bThrow,"fsaBOOL");
	}
	return pRtn;
}
const UINT8 *fsaDirEntry::GetDataUser(bool bThrow)
{
	const UINT8 *pRtn = NULL;
	if(nElementType >= TYPE_USER)
	{
		pRtn = GetDataUINT8(bThrow);
	}
	else
	{
		InvalidType(bThrow,"User");
	}
	return pRtn;
}

RGString fsaDirEntry::FormatDateTimeXS(
  const fsaDate *pDate, const fsaTime *pTime)
{
  RGString sRtn;
  char s[64];
  sprintf(s,"%04d-%02u-%02uT%02u:%02u:%02u",
    (int) pDate->year,
    (unsigned int) pDate->month,
    (unsigned int) pDate->day,
    (unsigned int) pTime->hour,
    (unsigned int) pTime->minute,
    (unsigned int) pTime->second);
  sRtn = s;
  return sRtn;
}

#ifdef _DEBUG
bool fsaDirEntry::UnitTest_XS()
{
  const char *ps = "1972-06-11T14:12:10";
  fsaDate dt = { 1972,6,11 };
  fsaTime tt = { 14,12,10,0 };
  RGString str = FormatDateTimeXS(&dt,&tt);
  bool bRtn = !!(str == ps);
  return bRtn;
}
#endif

void fsaDirEntry::ReversePeakData(fsaPeak *p)
{
    ByteOrder::reverse(&p->position);
    ByteOrder::reverse(&p->height);
    ByteOrder::reverse(&p->beginPos);
    ByteOrder::reverse(&p->endPos);
    ByteOrder::reverse(&p->beginHt);
    ByteOrder::reverse(&p->endHt);
    ByteOrder::reverse(&p->area);
    ByteOrder::reverse(&p->volume);
    ByteOrder::reverse(&p->fragSize);
    ByteOrder::reverse(&p->isEdited);
}
fsaPeak *fsaDirEntry::BuildPeakData(void *_p, UINT32 nDataSize)
{
  UINT8 *p = (UINT8 *) _p;
  UINT32 nElements = nDataSize / 96;
  fsaPeak *pRtn = NULL;
  if(nElements > 0)
  {
    size_t nSize = sizeof(fsaPeak) * nElements;
    pRtn = (fsaPeak *)malloc( nSize );
    Malloc ptr((void *)pRtn); // will free memory if an exception is thrown
    memset((void *)pRtn,0,nSize);
    fsaPeak *pPeak = pRtn;
    fsaPeak peak;
    UINT32 i;
    bool bLocalSmall = !ByteOrder::LocalBigEndian();

    for(i = 0; i < nElements; i++)
    {
#define BYTECOPY(dest,src,n) memcpy((void *)&dest,src,n);  src += n;
      BYTECOPY(peak.position,p,4);
      BYTECOPY(peak.height,p,2);
      BYTECOPY(peak.beginPos,p,4);
      BYTECOPY(peak.endPos,p,4);
      BYTECOPY(peak.beginHt,p,2);
      BYTECOPY(peak.endHt,p,2);
      BYTECOPY(peak.area,p,4);
      BYTECOPY(peak.volume,p,4);
      BYTECOPY(peak.fragSize,p,4);
      BYTECOPY(peak.isEdited,p,2);
      BYTECOPY(peak.label,p,64);
#undef BYTECOPY
      memcpy((void *)pPeak,(void *)&peak,sizeof(fsaPeak));
      if(bLocalSmall)
      {
        ReversePeakData(pPeak);
      }
      pPeak++;
    }
    ptr.release();
  }
  return pRtn;
}

fsaPeak *fsaDirEntry::GetDataPeak(bool bThrow)
{
  fsaPeak *pRtn = NULL;
  if(isPeak())
  {
    pRtn = BuildPeakData(pData,nDataSize);
  }
  if(bThrow && (pRtn != NULL))
  {
    char s[64];
    sprintf(s,"Cannot get peak data for %s",sName);
    throw new Exception(s);
  }
  return pRtn;
}

const void *fsaDirEntry::GetDataRaw(bool bThrow)
{
	return pData;
}

void fsaDirEntry::Split(char *ps, char delim, vector<char *> *pPtrs)
{
  char *psd = ps;
  bool done = !(*ps);
  pPtrs->clear();
  while( (psd != NULL) && (*ps) )
  {
    while( (*ps) == delim) 
    {
      ps++;
    }
    if (*ps)
    {
      pPtrs->push_back(ps); 
      psd = strchr(ps,delim);
      if(psd != NULL)
      {
        *psd = 0;
        ps = psd;
        ps++;
      }
    }
  }
}
bool fsaDirEntry::SetDataRaw(void *_pData, UINT32 _nDataSize, INT32 _nNumElements, bool bCopy)
{
  bool bRtn = true;
  if(!bCopy)
  {
    FreeData();
    pData = _pData;
  }
  else
  {
    if((pData == NULL) || (_nDataSize > nDataSize))
    {
      FreeData();
      pData = malloc(_nDataSize);
    }
    if(bRtn = (pData != NULL))
    {
      memcpy(pData,_pData, _nDataSize);
    }
  }
  if(bRtn)
  {
    nNumElements = _nNumElements;
    nDataSize = _nDataSize;
  }
  return bRtn;
}

bool fsaDirEntry::SetDataINT16(
          INT16 *_pData,
          INT32 _nNumElements,
          bool bCopy,  // set to true to make a copy of pData.  
                      // pData will be free()d upon destruction regardless of this value
          bool bThrow)
{
  bool bRtn = false;
  if(this->nElementType == TYPE_INT16)
  {
    UINT32 _nDataSize = sizeof(INT16) * _nNumElements;
    bRtn = SetDataRaw((void *)_pData, _nDataSize, _nNumElements, bCopy);
    if((!bRtn) && bThrow)
    {
      throw new Exception("malloc failed in SetDataINT16");
    }
  }
  else if(bThrow)
  {
    char s[64];
    sprintf(s,"Cannot call SetDataINT16, Invalid data type %d\n", (int) nElementType);
    throw new Exception(s);
  }
  return bRtn;
}
bool fsaDirEntry::SetDataTime(
    fsaTime *_pData, 
    int _nNumElements, 
    bool bCopy, 
    bool bThrow)
{
  bool bRtn = false;
  if(this->nElementType == TYPE_TIME)
  {
    UINT32 _nDataSize = sizeof(fsaTime) * _nNumElements;
    bRtn = SetDataRaw((void *)_pData, _nDataSize, _nNumElements, bCopy);
    if((!bRtn) && bThrow)
    {
      throw new Exception("malloc failed in SetDataTime");
    }
  }
  else if(bThrow)
  {
    char s[64];
    sprintf(s,"Cannot call SetDataTime, Invalid data type %d\n", (int) nElementType);
    throw new Exception(s);
  }
  return bRtn;
}

bool fsaDirEntry::SetDataDate(
    fsaDate *_pData, 
    int _nNumElements, 
    bool bCopy, 
    bool bThrow)
{
  bool bRtn = false;
  if(this->nElementType == TYPE_DATE)
  {
    UINT32 _nDataSize = sizeof(fsaDate) * _nNumElements;
    bRtn = SetDataRaw((void *)_pData, _nDataSize, _nNumElements, bCopy);
    if((!bRtn) && bThrow)
    {
      throw new Exception("malloc failed in SetDataDate");
    }
  }
  else if(bThrow)
  {
    char s[64];
    sprintf(s,"Cannot call SetDataDate, Invalid data type %d\n", (int) nElementType);
    throw new Exception(s);
  }
  return bRtn;
}


bool fsaDirEntry::SetDataString(const char *ps)
{
  void *pRtn;
  size_t nElem = StringToData(&pRtn, nElementType, ps);
  bool bRtn = !!nElem;
  if(bRtn)
  {
    FreeData();
    pData = pRtn;
    nNumElements = (INT32) nElem;
    nDataSize = nNumElements * nElementSize;
  }
  return(bRtn);
}

void fsaDirEntry::BuildRawPeakData(fsaPeak *_pPeak, UINT32 _nNumElements, void *pDest)
{
  UINT32 i;
  fsaPeak *pPeak = _pPeak;
  fsaPeak peak;
  UINT8 *p = (UINT8 *)pDest;
  bool bSmall = !ByteOrder::LocalBigEndian();
  for(i = 0; i < _nNumElements; i++)
  {
    memcpy((void *)&peak,pPeak,sizeof(peak));
    pPeak++;
    if(bSmall)
    {
      ReversePeakData(&peak);
    }
    // copy 96 bytes from fsaPeak to raw data.
    // because of alignment, fsaPeak is 100 bytes and a simple memcpy will not work
#define BYTECOPY(p,src,n) memcpy((void *)p,(void *)&src,n); p += n;
    BYTECOPY(p,peak.position,4);
    BYTECOPY(p,peak.height,2);
    BYTECOPY(p,peak.beginPos,4);
    BYTECOPY(p,peak.endPos,4);
    BYTECOPY(p,peak.beginHt,2);
    BYTECOPY(p,peak.endHt,2);
    BYTECOPY(p,peak.area,4);
    BYTECOPY(p,peak.volume,4);
    BYTECOPY(p,peak.fragSize,4);
    BYTECOPY(p,peak.isEdited,2);
    BYTECOPY(p,peak.label,64);
#undef BYTECOPY
  }
}

void *fsaDirEntry::BuildRawPeakData(fsaPeak *p, UINT32 n)
{
  void *pRtn = NULL;
  if(n)
  {
    pRtn = malloc((size_t)(n * 96));
    if(pRtn != NULL)
    {
      Malloc mTmp(pRtn);
      BuildRawPeakData(p,n,pRtn);
      mTmp.release();
    }
  }
  return pRtn;
}

bool fsaDirEntry::SetDataPeak(fsaPeak *_pData, UINT32 _nNumElements, bool bThrow)
{
  bool bRtn = false;
  if(!_nNumElements)
  {
    if(bThrow)
    {
      throw new Exception("fsaDirEntry::SetDataPeak called with 0 elements");
    }
  }
  else if(isPeak())
  {
    UINT32 nMalloc = 96 * _nNumElements;
    if((pData == NULL) || (nMalloc > this->nDataSize))
    {
      FreeData();
      pData = malloc((size_t) nMalloc);
    }
    if(bRtn = (pData != NULL))
    {
      this->nDataSize = nMalloc;
      BuildRawPeakData(_pData, _nNumElements, pData);
    }
    else if(bThrow)
    {
      throw new Exception("fsaDirEntry::SetDataPeak, malloc failed");
    }
  }
  else if(bThrow)
  {
    char s[64];
    sprintf(s,"Cannot set peak data for %s", this->sName);
    throw new Exception(s);
  }
  return bRtn;
}

//  static formatting functions

static void FormatINT16(INT16 *p,char *ps)
{
	sprintf(ps,"%d",(int)(*p));
}
static void FormatUINT16(UINT16 *p, char *ps)
{
	sprintf(ps,"%u",(unsigned int)(*p));
}
static void FormatUINT8(UINT8 *p,char *ps)
{
#ifdef __BUG
	char c = *((char *)p);
	sprintf(ps,"%u",(unsigned int)(c));
#else
	sprintf(ps,"%u",(unsigned int)(*p));
#endif
}
static void FormatChar(char *p,char *ps)
{
	sprintf(ps,"%d",(int)(*p));
}

static void FormatINT32(INT32 *p, char *ps)
{
	sprintf(ps,"%ld",(long)(*p));
}
static void FormatFloat(float *p,char *ps)
{
	sprintf(ps,"%g",(double)(*p));
}
static void FormatDouble(double *p, char *ps)
{
	sprintf(ps,"%g",*p);
}
static void FormatDate(fsaDate *p, char *ps)
{
#ifdef __BUG
	sprintf(ps,"%u/%u/%d",
		(unsigned int)p->month,
		(unsigned int)p->day,
		(int)p->year);
#else
	sprintf(ps,"%04d-%02u-%02u",
		(int)(p->year),
		(unsigned int)(p->month),
		(unsigned int)(p->day));
#endif
}
static void FormatTime(fsaTime *p, char *ps)
{
#ifdef __BUG
	sprintf(ps,"%u:%u:%u",
		(unsigned int)p->hour,
		(unsigned int)p->minute,
		(unsigned int)p->second);
#else
	sprintf(ps,"%02u:%02u:%02u.%02u",
		(unsigned int)p->hour,
		(unsigned int)p->minute,
		(unsigned int)p->second,
		(unsigned int)p->hsecond);
#endif
}
static void FormatBOOL(fsaBOOL *p, char *ps)
{
	strcpy(ps,(*p) ? "true" : "false");
}
static void FormatThumb(fsaThumb *p, char *ps)
{
	sprintf(ps,"%lu.%lu.%u.%u",
		(unsigned long)p->d,
		(unsigned long)p->u,
		(unsigned int)p->c,
		(unsigned int)p->n);
}

//  static un-formatting functions

static void UnformatChar(char *p, const char *ps)
{
  //int n = atoi(ps);
	int n = (int)strtol (ps, (char**)NULL, 10);
  *p = (char)(n & 255);
}
static void UnformatBOOL(UINT8 *p, const char *ps)
{
  int n = !strncmp(ps,"true",4); // 1 of true, 0 otherwise
  *p = (char)(n & 255);
}
static void UnformatUINT16(UINT16 *p, const char *ps)
{
  //int n = atoi(ps);
  int n = (int)strtol (ps, (char**)NULL, 10);
  *p = (UINT16)(n & 0xffff);
}
static void UnformatINT16(INT16 *p, const char *ps)
{
  //int n = atoi(ps);
  int n = (int)strtol (ps, (char**)NULL, 10);
  *p = (UINT16)(n & 0xffff);
}
static void UnformatINT32(INT32 *p, const char *ps)
{
  //long n = atol(ps);
  long n = strtol (ps, (char**)NULL, 10);
  *p = (INT32)n;
}
static void UnformatFloat(float *p, const char *ps)
{
  //*p = (float)atof(ps);
  *p = (float)strtod (ps, (char**)NULL);
}
static void UnformatDouble(double *p, const char *ps)
{
  //*p = atof(ps);
  *p = strtod (ps, (char**)NULL);
}
static void UnformatDate(fsaDate *p, const char *ps)
{
  Malloc mem(strdup(ps));
  char *pnew = (char *)mem.ptr();
  memset(p,0,sizeof(fsaDate));
  if(pnew != NULL)
  {
    vector<char *> aStr;
    bool bError = false;
    int mdy[] = {0,1,2};
    int ymd[] = {1,2,0};
    int *pMDY;
    char cSplit;
    if(strchr(pnew,'/') != NULL)
    {
      cSplit = '/';
      pMDY = mdy;
    }
    else
    {
      cSplit = '-';
      pMDY = ymd;
    }
    fsaDirEntry::Split(pnew,cSplit,&aStr);
    if(aStr.size() == 3)
    {
      UnformatChar((char *) &(p->month),aStr.at(*pMDY));
      pMDY++;
      UnformatChar((char *) &(p->day),aStr.at(*pMDY));
      pMDY++;
      UnformatINT16(&(p->year),aStr.at(*pMDY));
    }
  }
}
static void UnformatTime(fsaTime *p, const char *ps)
{
  Malloc mem(strdup(ps));
  char *pnew = (char *) mem.ptr();
  memset(p,0,sizeof(fsaTime));
  if(pnew != NULL)
  {
    vector<char *> aStr;
    fsaDirEntry::Split(pnew,':',&aStr);
    if(aStr.size() == 3)
    {
      UnformatChar((char *)&(p->hour), aStr.at(0));
      UnformatChar((char *)&(p->minute), aStr.at(1));
      char *pSec = aStr.at(2);
      char *pDot = strchr(pSec,'.');
      if(pDot != NULL)
      {
        *pDot = 0;
        pDot++;
        UnformatChar((char *)&(p->hsecond),pDot);
      }
      UnformatChar((char *)&p->second,pSec);
    }
  }
}
static void UnformatThumb(fsaThumb *p, const char *ps)
{
  Malloc mem(strdup(ps));
  char *pnew = (char *) mem.ptr();
  memset(p,0,sizeof(fsaThumb));
  if(pnew != NULL)
  {
    vector<char *> aStr;
    fsaDirEntry::Split(pnew,'.',&aStr);
    if(aStr.size() == 4)
    {
      UnformatINT32(&(p->d),aStr.at(0));
      UnformatINT32(&(p->u),aStr.at(1));
      UnformatChar((char *)&(p->c),aStr.at(2));
      UnformatChar((char *)&(p->n),aStr.at(3));
    }
  }
}



void fsaDirEntry::BuildStringUser()
{
	pString = new RGString((size_t) (nDataSize * 4 + 1));
	unsigned char *psData = (unsigned char *)pData;
	char s[8];
	char *p = &s[1];
	s[0] = ' ';
	for(UINT32 i = 0; i < nDataSize; ++i)
	{
		sprintf(&s[1],"%02x",(unsigned int)(*psData));
		pString->Append((const char *)p);
		psData++;
		p = &s[0];
	}
}
bool fsaDirEntry::CharByte() const
{
	bool bRtn = false;
	if( 
		(pData != NULL) &&
		(ElementType() == TYPE_CHAR)
		)
	{
    const char *p = (const char *)pData;
    for(INT32 i = 0; !bRtn && (i < (INT32)nNumElements); i++)
		{
			bRtn = ((*p) < 32);
      p++;
		}
	}
	return bRtn;
}
INT16 fsaDirEntry::GetElementSize(bool bThrow)
{
  INT16 nRtn = 0;
  switch(ElementTypeEnum(nElementType))
	{
		case _TYPE_BOOL:
		case _TYPE_USER:
		case _TYPE_PSTRING:
		case _TYPE_CSTRING:
		case _TYPE_UINT8:
		case _TYPE_CHAR:
      nRtn = 1;
			break;
		case _TYPE_UINT16:
		case _TYPE_INT16:
      nRtn = 2;
			break;
		case _TYPE_TIME:
		case _TYPE_DATE:
		case _TYPE_INT32:
		case _TYPE_FLOAT:
      nRtn = 4;
			break;
		case _TYPE_DOUBLE:
      nRtn = 8;
			break;
		case _TYPE_THUMB:
      nRtn = 10;
      break;
    default:
      if(bThrow)
      {
        char s[64];
        sprintf(s,"Invalid data type %d\n", (int) nElementType);
        throw new Exception(s);
      }
  }
  return nRtn;
}
size_t fsaDirEntry::StringToData(void **pRtn, INT16 nType, const char *psString)
{
  size_t nRtn = 0;
  switch(ElementTypeEnum(nType))
  {
		case _TYPE_USER:
		case _TYPE_CHAR_BYTE:
		case _TYPE_UINT8:
      nRtn = BuildArray<char>((char **)pRtn, psString, UnformatChar);
      break;
		case _TYPE_CHAR:
      {
        nRtn = strlen(psString);
        Malloc str(nRtn + 1);
        strcpy((char *)str.ptr(),psString);
        *pRtn = str.release();
      }
      break;
		case _TYPE_BOOL:
      nRtn = BuildArray<UINT8>((UINT8 **)pRtn,psString,UnformatBOOL);
      break;
		case _TYPE_PSTRING:
      {
        nRtn = strlen(psString);
        Malloc str(nRtn + 2);
        char *ps = (char *) str.ptr();
        *ps = (char)(nRtn & 255);
        ps++;
        strcpy(ps,psString);
        nRtn++;
        *pRtn = (void *)(str.release());
      }
      break;
		case _TYPE_CSTRING:
      {
        Malloc str(strdup(psString));
        *pRtn = (void *)(str.release());
        nRtn = strlen(psString) + 1;
      }
      break;
		case _TYPE_UINT16:
      nRtn = BuildArray<UINT16>((UINT16 **)pRtn, psString, UnformatUINT16);
      break;
		case _TYPE_INT16:
      nRtn = BuildArray<INT16>((INT16 **)pRtn, psString, UnformatINT16);
      break;
    case _TYPE_FLOAT:
      nRtn = BuildArray<float>((float **)pRtn, psString, UnformatFloat);
      break;
		case _TYPE_INT32:
      nRtn = BuildArray<INT32>((INT32 **)pRtn, psString, UnformatINT32);
      break;
		case _TYPE_DOUBLE:
      nRtn = BuildArray<double>((double **)pRtn, psString, UnformatDouble);
      break;
		case _TYPE_DATE:
      nRtn = BuildArray<fsaDate>((fsaDate **)pRtn, psString, UnformatDate);
      break;
		case _TYPE_TIME:
      nRtn = BuildArray<fsaTime>((fsaTime **)pRtn, psString, UnformatTime);
      break;
		case _TYPE_THUMB:
      nRtn = BuildArray<fsaThumb>((fsaThumb **)pRtn, psString, UnformatThumb);
      break;
		default:
      break;
  }
  return nRtn;
}


void *fsaDirEntry::StringToData(INT16 nType, INT32 nSize, const char *pString)
{
  Malloc holder((size_t)nSize + 1);
  UINT8 *pRtn = (UINT8 *)holder.ptr();

  if(pRtn != NULL)
  {
    memset((void *)pRtn,0,(size_t)nSize + 1);
    size_t nLen = strlen(pString);
    switch(ElementTypeEnum(nType))
    {
		  case _TYPE_USER:
		  case _TYPE_CHAR_BYTE:
		  case _TYPE_UINT8:
          BuildArray<char>((char *)pRtn,pString,(size_t)nSize, UnformatChar);
          break;
		  case _TYPE_CHAR:
        if(strlen(pString) <= (size_t)nSize)
        {
          strncpy((char *) pRtn, pString,nSize);
        }
        else
        {
          BuildArray<char>((char *)pRtn,pString,(size_t)nSize, UnformatChar);
        }
        break;
		  case _TYPE_BOOL:
        BuildArray<UINT8>(pRtn,pString,(size_t)nSize, UnformatBOOL);
        break;
		  case _TYPE_PSTRING:
        if(nLen >= (size_t) nSize)
        {
           nLen = (nSize - 1);
        }
        pRtn[0] = (UINT8) (nLen & 255);
        memcpy((char *) &pRtn[1], pString, nLen);
        break;
		  case _TYPE_CSTRING:
        {
          size_t n = nSize;
          n--;
          strncpy((char *)pRtn,pString,n);
          pRtn[n] = 0;
        }
        break;
		  case _TYPE_UINT16:
        BuildArray<UINT16>(pRtn,pString,(size_t) nSize, UnformatUINT16);
        break;
		  case _TYPE_INT16:
        BuildArray<INT16>(pRtn,pString,(size_t) nSize, UnformatINT16);
        break;
      case _TYPE_FLOAT:
        BuildArray<float>(pRtn,pString,(size_t) nSize, UnformatFloat);
        break;
		  case _TYPE_INT32:
        BuildArray<INT32>(pRtn,pString,(size_t) nSize, UnformatINT32);
        break;
		  case _TYPE_DOUBLE:
        BuildArray<double>(pRtn,pString,(size_t) nSize, UnformatDouble);
        break;
		  case _TYPE_DATE:
        BuildArray<fsaDate>(pRtn,pString,(size_t) nSize, UnformatDate);
        break;
		  case _TYPE_TIME:
        BuildArray<fsaTime>(pRtn,pString,(size_t) nSize, UnformatTime);
        break;
		  case _TYPE_THUMB:
        BuildArray<fsaThumb>(pRtn,pString,(size_t) nSize, UnformatThumb);
        break;
		  default:
        break;
    }
  }
  return holder.release();
}


const RGString &fsaDirEntry::DataToString(bool bThrow)
{
	if(pString == NULL)
	{
		switch(ElementTypeEnum(nElementType))
		{
		case _TYPE_UINT8:
			BuildString<UINT8>(
				(UINT8 *)pData, FormatUINT8,3);
			break;
		case _TYPE_CHAR:
			{
				if(CharByte())
				{
					BuildString<char>((char *)pData,FormatChar,3);
				}
				else
				{
					pString = new RGString((const char *)pData);
				}
			}
			break;
		case _TYPE_UINT16:
			BuildString<UINT16>(
				(UINT16 *)pData,FormatUINT16,5);
			break;
		case _TYPE_INT16:
			BuildString<INT16>(
				(INT16 *)pData,FormatINT16,6);
			break;
		case _TYPE_INT32:
			BuildString<INT32>(
				(INT32 *)pData,FormatINT32,11);
			break;
		case _TYPE_FLOAT:
			BuildString<float>(
				(float *)pData,FormatFloat,12);
			break;
		case _TYPE_DOUBLE:
			BuildString<double>(
				(double *)pData,FormatDouble,12);
			break;
		case _TYPE_DATE:
			BuildString<fsaDate>(
				(fsaDate *)pData,FormatDate,10);
			break;
		case _TYPE_TIME:
			BuildString<fsaTime>(
				(fsaTime *)pData,FormatTime,11);
			break;
		case _TYPE_PSTRING:
			{
				char *ps;
				if(pData == NULL)
				{
					ps = "";
				}
				else
				{
					ps = (char *)pData;
					++ps;
				}
				pString = new RGString((const char *)ps);
			}
			break;
		case _TYPE_CSTRING:
			{
				const char *ps;
				ps = (pData != NULL) ? (const char *)pData : "";
				pString = new RGString(ps);
			}
			break;
		case _TYPE_THUMB:
			BuildString<fsaThumb>(
				(fsaThumb *)pData,FormatThumb,32);
		case _TYPE_BOOL:
			BuildString<fsaBOOL>(
				(fsaBOOL *)pData,FormatBOOL,5);
			break;			
		case _TYPE_USER:
    default:
			BuildStringUser();
			break;
		}
	}
	return (const RGString &)(*pString);
}

const char *fsaDirEntry::FormatDateString(fsaDate *pDate, char* const dataField)
{
  static char *MONTH[] =
    {"ERR","Jan","Feb","Mar","Apr","May","Jun",
      "Jul","Aug","Sep","Oct","Nov","DEC"};
  int nYear = pDate->year;
  int nMonth = pDate->month;
  int nDay = pDate->day;

  // check bounds for all

  if(nMonth < 1 || nMonth > 12)
  {
    nMonth = 0;
  }
  if(nDay > 31 || nDay < 1)
  {
    nDay = 99;
  }
  if(nYear < 0)
  {
     nYear = 9999;
  }
  else if(nYear < 50)
  {
    nYear += 2000;
  }
  else if(nYear < 100)
  {
    nYear += 1900;
  }
  else if(nYear > 9999 || nYear < 1900)
  {
    nYear = 9999;
  }
  
  sprintf(dataField,"%s %02d, %04d",MONTH[nMonth],nDay, nYear);
  return (const char *) dataField;
}
