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
 *  fsaFileInput.cpp - class for reading Applied Biosystems' fsa files
 *    SEE ALSO: byteOrder.h/cpp; 
 *    example program - ../fsa2xml.cpp
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "fsaFileInput.h"
#include "ByteOrder.h"
#include "Exception.h"

#define MAX_FILE_SIZE 5000000

fsaFileInput::~fsaFileInput()
{
  _CleanupBuffer();
}


void fsaFileInput::Insert(UINT8 *pBuffer)
{
	fsaDirEntry *p = new fsaDirEntry(pBuffer);
	_mapDirEntry.insert(MapDirEntry::value_type(p->MakeKey(),p));
	_vecDirEntry.push_back(p);
}
bool fsaFileInput::_ReadBytes(void *pBuffer, size_t n)
{
  bool bRtn = true;
  if(((_pBufferSeek - _pBuffer) + n) > _nBufferSize)
  {
    bRtn = false;
  }
  else
  {
    memcpy((void *)pBuffer, _pBufferSeek, n);
    _pBufferSeek += n;
  }
  return bRtn;
}
bool fsaFileInput::_SeekTo(size_t nOffset)
{
  bool bRtn = false;
  if(nOffset <= _nBufferSize)
  {
      bRtn = true;
      _pBufferSeek = _pBuffer + nOffset;
  }
  return bRtn;
}
#if 0
void fsaFileInput::_SetupHIDOffset()
{
  UINT8 sType1[4];
  UINT8 sType2[4];
  size_t ndx = 0;
  char aChars[256];
  memset(aChars,0,sizeof(aChars));

  UINT32 nElems = _pDir->NumElements();
  UINT32 nOffset = _pDir->Offset();
  UINT32 nOffsetEnd = (nElems * 28);
  UINT32 nOffsetLast = nOffsetEnd - 28;
  if(!Read((void *) sType1,nOffset,4))
  {
    _bError = true;
  }
  else if(!Read((void *) sType2,nOffset + nOffsetLast,4))
  {
    _bError = true;
  }
  else
  {
    // begin the search
    // setup each char from tag name in the char
    //  char array;
    for(ndx = 0; ndx < 4; ndx++)
    {
      aChars[ sType1[ndx] ] = 1;
    }
    // start after 34 bytes (+3)
    UINT8 *pSearch = (UINT8 *)(_pBuffer + 37);
    UINT8 *pMax = pSearch + _nBufferSize - nOffsetLast - 28;
    UINT8 *pFromHeader = (UINT8 *)(_pBuffer + nOffset);
    UINT8 *pCompare;
    bool bFound;
    while(pSearch < pMax)
    {
      // if the 4th char is not in the name, then
      // we can eliminate all 4 charaters
      while( (!aChars[ *pSearch ]) && (pSearch < pMax) )
      {
        pSearch += 4;
      }
      if(pSearch < pMax)
      {
        bFound = false;
        for(pCompare = pSearch - 3;
            (pCompare <= pSearch);
            pCompare++)
        {
          if( (pCompare != pFromHeader) &&
              (!memcmp(pCompare,sType1,4)) &&
              (!memcmp(pCompare + nOffsetLast,sType2,4)) &&
              fsaDirEntry::ValidateName(pCompare + 28)
            )
          {
            // we found something
            bFound = true;
            _anHIDoffsets[_nOffsets] = (pCompare - _pBuffer);
            _nOffsets++;
            if(_nOffsets == NADDRS)
            {
              pSearch = pMax;
            }
            else
            {
              pSearch = pCompare + nOffsetEnd + 3;
            }
          }
        }
        if(!bFound)
        {
          pSearch += 4;
        }
      }
    }
    if(_nOffsets)
    {
      _pDir->SetOffset(_anHIDoffsets[0]);
    }
    else if(_bUseWarning)
    {
      _bWarning = true;
    }
  }
}
#else

const UINT8 *fsaFileInput::MEMSTR(
  const UINT8 *_pBuffer, size_t _nLen, 
  const char *pSearchString, size_t nSearchLen)
{
  const UINT8 *pRtn = NULL;
  bool bOK = true;
  if(nSearchLen < 1)
  {
    nSearchLen = strlen(pSearchString);
  }
  if(nSearchLen > 0)
  {
    size_t nLen = _nLen;
    const UINT8 *pBuffer = _pBuffer;
    size_t nSearchLen1;
    const UINT8 *pMax = pBuffer + nLen;
    const UINT8 *pFind;
    const UINT8 *pTmp;
    const char *pTmpC = pSearchString;
    char aChars[256];
    nSearchLen1 = nSearchLen - 1;
    // initialize aChars
    memset(aChars,0,sizeof(aChars));
    for(size_t ndx = 0; ndx < nSearchLen; ndx++)
    {
      aChars[ *pTmpC ] = 1;
      pTmpC++;
    }
    // END initialize aChars
    pFind = pBuffer + nSearchLen1;

#define SETOK (bOK = (pFind < pMax))

    SETOK;
    while(bOK && (pRtn == NULL))
    {
      while((!aChars[ *pFind ]) && bOK)
      {
        pFind += nSearchLen;
        SETOK;
      }
      if(bOK)
      {
        for(pTmp = pFind - nSearchLen1;
          pTmp <= pFind;
          pTmp++)
        {
          if(!memcmp(pTmp,pSearchString, nSearchLen))
          {
            pRtn = pTmp;
            pFind = pTmp - 1; // loop exit and 
          }
        }
        pFind += nSearchLen; // if found, then pFind points to end of search string
                      // and this is OK if pFind < pMax
                      // if not found, then the next (nSearchLen) bytes have been 
                      // eliminated in the search
        SETOK;
      }
    }
  }
  return bOK ? pRtn : NULL;
}

#ifdef _DEBUG
bool fsaFileInput::TESTMEMSTR()
{
  // length of pBuffer should be a multiple of 12 for testing
  // the end with strings of length 3 and 4
  const UINT8 *pBuffer = (const UINT8 *)"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  const UINT8 *pFind;
  const char *pTemp;
  bool bFind;
  struct
  {
    const char *p;
    bool bFind;
  }
  TESTS[] = 
  {
    {"ABC",true},
    {"C",true},
    {"Z0",true},
    {"9",true},
    {"A",true},
    {"789",true},
    {"6789",true},
    {"56789",true},
    {"c",false},
    {"aBCD",false},
    {"a789",false},
    {"aKLMN",false}, // should skip over with no compare
    {"aHIJK",false}, // should find J and test (offset + 1 is multiple of length) and test
    {"a2345", false},
    {(const char *)pBuffer,true}
  };
  size_t nTestCount = sizeof(TESTS) / sizeof(TESTS[0]);
  size_t nLenBuffer = strlen((const char *)pBuffer);
  size_t nLen;
  bool bRtn = true;
  for(size_t i = 0; i < nTestCount; i++)
  {
    pTemp = TESTS[i].p;
    bFind = TESTS[i].bFind;
    nLen = strlen(pTemp);
    pFind = MEMSTR(pBuffer,nLenBuffer,pTemp,nLen);
    if((pFind != NULL) != bFind)
    {
      bRtn = false;
    }
    else if(bFind && memcmp(pFind,pTemp,nLen))
    {
      bRtn = false;
    }
  }
  // look for string match that overwrites the buffer
  if(MEMSTR(pBuffer,nLenBuffer - 1, "789",3) != NULL)
  {
    // matching indicates memory overrun.
    bRtn = false;
  }
  return bRtn;
}

#endif // _DEBUG

bool fsaFileInput::_EntryOK(const UINT8 *pBufferMid, bool bCheckType) const
{
  bool bRtn = true;
  if( (pBufferMid < _pBuffer) || // includes NULL
      (pBufferMid > (_pBuffer + _nBufferSize))
    )
  {
    bRtn = false;
  }
  else
  {
    fsaDirEntry x(pBufferMid,_bFileBigEndian);
    if(!x.ValidateName())
    {
      bRtn = false;
    }
    else if(bCheckType)
    {
#define DELIMXX "!!"
#define FIND_IN_THIS DELIMXX "user_defined" DELIMXX "unknown" DELIMXX
      std::string str("");
      str.reserve(64);
      str.append(DELIMXX);
      str.append(x.GetTypeString());
      str.append(DELIMXX);
      if(strstr(FIND_IN_THIS,str.c_str()) != NULL)
      {
        bRtn = false;
      }
#undef FIND_IN_THIS
#undef DELIMXX
    }
  }
  return bRtn;
}
const UINT8 *fsaFileInput::_FindEndEntry(const UINT8 *pBufferMid, bool bFirst) const
{
  int n = bFirst ? -28 : 28;
  const UINT8 *pRtn = NULL;
  const UINT8 *pBuffer = pBufferMid;
  while(_EntryOK(pBuffer,false))
  {
    pRtn = pBuffer;
    pBuffer += n;
  }
  return pRtn;
}

void fsaFileInput::_SetupHIDOffset()
{

  // new algorithm 1/4/2013
  // find DATA beginning at point
  //  if type is short, keep it
  //  otherwise find Dye and set pointer then repeat

  UINT32 nElems = _pDir->NumElements();
  UINT32 nOffset = _pDir->Offset();
  UINT32 nOffsetEnd = (nElems * 28);
  UINT32 nOffsetLast = nOffsetEnd - 28;
  const UINT8 *pOffset = _pBuffer + nOffset;
  const UINT8 *pOffsetEnd = pOffset + nOffsetEnd;
  const UINT8 *pNewOffset = NULL;
  const UINT8 *pNewOffsetLast = NULL;
  bool bDone = false;

#define BUFSIZE(p) (_nBufferSize - (size_t)(p - _pBuffer))

  const UINT8 *pDYE = NULL;
  const UINT8 *pDATA = MEMSTR(_pBuffer + 34,_nBufferSize - 34,"DATA",4);

  while(!(bDone || _bError))
  {
    pDYE = 
      (pDATA == NULL)
      ? NULL
      : MEMSTR(pDATA,BUFSIZE(pDATA), "DyeN",4);
    if(pDYE == NULL)
    {
      _bError = true;
    }
    else if( ( pDATA >= pOffset && pDATA < pOffsetEnd ) ||
        (!_EntryOK(pDATA, true))  ||
        (!_EntryOK(pDYE,true))
        )
    {
      // skipping
      pDATA = (pDYE != NULL)
        ? MEMSTR(pDYE,BUFSIZE(pDATA),"DATA",4)
        : NULL;
    }
    else
    {
        bDone = true;
    }
  }
  if(!_bError)
  {
    pNewOffset = _FindEndEntry(pDATA,true);
    pNewOffsetLast = _FindEndEntry(pDATA,false);
    if(pDYE > pNewOffsetLast)
    {
      // could not find dye
      _bError = true;
    }
    else
    {
      size_t n = ((pNewOffsetLast - pNewOffset) / 28) + 1;
      _pDir->SetOffset((UINT32)(pNewOffset - _pBuffer));
      _pDir->SetNumElements(n);
    }
  }
}

#endif

bool fsaFileInput::_LoadFileBuffer(const char *psName)
{
  RGFile File(psName,"rb");
  _nBufferSize = (size_t)File.GetSizeOfFile();
  if(!File.Exists())
  {
    _bError = true;
  }
  else if((_nBufferSize = File.GetSizeOfFile()) < 100)
  {
    _bError = true;
  }
  else if (_nBufferSize > MAX_FILE_SIZE)
  {
    _bError = true;
  }
  else if((_pBuffer = (UINT8 *)malloc((size_t)_nBufferSize)) == NULL)
  {
    _bError = true;
  }
  else 
  {
    FILE *pf = File.GetFile();
    size_t nCHUNK = 8192;
    size_t nToRead = _nBufferSize;
    size_t nRead;
    UINT8 *p = _pBuffer;
    memset(_pBuffer,0,(size_t)_nBufferSize);
    while((nToRead > 0) && (!_bError))
    {
      if(nToRead < nCHUNK)
      {
        nCHUNK = nToRead;
      }
      nRead = fread(p,1,nCHUNK,pf);
      if(nRead > 0)
      {
        p += nRead;
        nToRead -= nRead;
      }
      else
      {
        _bError = true;
      }
    }
    if(_bError) {}
    else if(strncmp((char *)_pBuffer,"ABIF",4))
    {
      _bError = true;
    }
    else
    {
      RGString sName(psName);
      sName.ToLower();
      const char *psNameLower = (const char *)sName;
      size_t nLen = sName.Length();
      _bHID = ((nLen > 4) && !strncmp(psNameLower + nLen - 4,".hid",4));
      _pBufferSeek = _pBuffer;
      UINT8 sType[34];
      if(!_ReadBytes(sType,34))
      {
        _bError = true;
      }
      _nVersion = ByteOrder::localInt16(&sType[4]);
      _pDir = new fsaDirEntry(&sType[6]);
      if(_bHID)
      {
        _SetupHIDOffset();
      }
      if(!_SeekTo(_pDir->Offset()))
      {
        _bError = true;
      }
    }
  }
  return !_bError;
}

fsaFileInput::fsaFileInput(const char *psName,bool bUseWarning) : 
  fsaFile(),
  _pBuffer(NULL),
  _pBufferSeek(NULL),
  _nOffsets(0),
  _nBufferSize(0),
  _bHID(false)
{

	UINT8 sType[28];
  SetUseWarning(bUseWarning);
  memset(_anHIDoffsets,0,sizeof(_anHIDoffsets));
  if(!_LoadFileBuffer(psName))
  {
    _bError = true;
  }
  else
  {
		UINT32 nMax = _pDir->NumElements();
		for(UINT32 i = 0; i < nMax; i++)
		{
      if(_ReadBytes(sType,28) && fsaDirEntry::ValidateName(sType))
			{
				Insert(sType);
			}	
			else if(_bUseWarning && (i > 100))
      {
        _bWarning = true;
				nMax = i; // loop exit
        _pDir->SetNumElements(nMax);
      }
      else
			{
        _bError = true;
				i = nMax; // loop exit
			}
		}
    for(UINT32 i = _bError ? nMax : 0; i < nMax; i++)
    {
      // do this loop after the previous because the previous is sequential reading
      //  and this requires seeking for each read.
      this->_vecDirEntry.at(i)->LoadData(false,this);
    }
	}
  _CleanupBuffer();
}
bool fsaFileInput::isValid()
{ 
  return ( (_pDir != NULL) && fsaFile::isValid() ); 
}
