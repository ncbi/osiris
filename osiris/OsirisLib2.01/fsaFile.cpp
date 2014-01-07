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
 *  fsaFile.h - class for storing Applied Biosystems' fsa file data
 *    Inherited classes -- fsaFileInput - class for reading an fsa file
 *                         fsaFileOutput - class for writing XML from fsa file data
 *    SEE ALSO: byteOrder.h/cpp; 
 *    example program - ../fsa2xml.cpp
 *
 *  classes:
 *
     fsaFile - class for reading data from an fsa file

     To use this class, you must first read the documentation 
        available at:
http://www.appliedbiosystems.com/support/software_community/ABIF_File_Format.pdf
      more documentation on this file is in the comment block in fsaFile.h

 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsaFile.h"
#include "ByteOrder.h"
#include "Exception.h"
#include "fsa2XML.h"

void fsaFile::ClearEntries()
{
	vector<fsaDirEntry *>::iterator itrEnd = _vecDirEntry.end();
	vector<fsaDirEntry *>::iterator itr;
	fsaDirEntry *p;
	for(itr = _vecDirEntry.begin();
			itr != itrEnd;
			++itr)
	{
		p = *itr;
		if(p != NULL)
		{
			delete p;
		}
	}
	_vecDirEntry.clear();
	_mapDirEntry.clear();
}
void fsaFile::cleanup()
{
  ClearEntries();
	if(_pDir != NULL)
	{
		delete _pDir;
		_pDir = NULL;
	}
}
fsaFile::~fsaFile()
{
		cleanup();
}



fsaDirEntry *fsaFile::FindDirEntry(const char *ps, INT32 n)
{
  RGString s = fsaDirEntry::MakeKey(ps,n);
  MapDirEntry::iterator itr = _mapDirEntry.find(s);
  fsaDirEntry *pRtn = (itr == _mapDirEntry.end()) ? NULL : itr->second;
  return pRtn;
}

static int CompareEntry(const void *pp1, const void *pp2)
{
  const fsaDirEntry **p1 = (const fsaDirEntry **) pp1;
  const fsaDirEntry **p2 = (const fsaDirEntry **) pp2;
  return (**p1).Compare(*p2);
}
void fsaFile::Sort()
{
  const int MAX = 2000;
  int n = dirEntryCount();
  fsaDirEntry *pEntries[MAX];
  rewindDirEntries();
  memset(pEntries,0,sizeof(pEntries));
  if((n > MAX) && !IsSorted())
  {
    throw new Exception("Too many dir entries in fsa data, cannot sort");
  }
  for(int i = 0; i < n; i++)
  {
    pEntries[i] = nextDirEntry();
  }
  qsort((void *) pEntries,sizeof(fsaDirEntry *),(size_t) n, CompareEntry);
  _vecDirEntry.clear();
  for(int i = 0; i < n; i++)
  {
    _vecDirEntry.push_back(pEntries[i]);
  }
}
bool fsaFile::IsSorted()
{
  int n = dirEntryCount();
  fsaDirEntry *prev = (n > 0) ? _vecDirEntry.at(0) : NULL;
  fsaDirEntry *p;
  bool bRtn = true;
  for(int i = 1; bRtn && (i < n); i++)
  {
    p = _vecDirEntry.at(i);
    if(p->Compare(prev) < 0)
    {
      bRtn = false;
    }
    else
    {
      prev = p;
    }
  }
  return bRtn;
}

int fsaFile::WriteXML(RGFile &file)
{
  if(!IsSorted())
  {
    Sort();
  }
  rewindDirEntries();
  fsa2XML::WriteXML(this,&file);
  return 0;
}

bool fsaFile::CopyAddDirEntry(const fsaDirEntry &Entry)
{
  bool bRtn = Entry.ValidateName();
  if(bRtn)
  {
 	  fsaDirEntry *p = new fsaDirEntry(Entry);
    bRtn = AddDirEntry(p);
  }
  return bRtn;
}
bool fsaFile::AddDirEntry(fsaDirEntry *pEntry)
{
  bool bRtn = true;
  if(pEntry->GetDataRaw() == NULL)
  {
    bRtn = false;
  }
  else
  {
    RGString sKey(pEntry->MakeKey());
    MapDirEntry::iterator itr = _mapDirEntry.find(sKey);
    if(itr != _mapDirEntry.end())
    {
      bRtn = false;
    }
    else
    {
	    _mapDirEntry.insert(MapDirEntry::value_type(sKey,pEntry));
	    _vecDirEntry.push_back(pEntry);
    }
  }
  return bRtn;
}

bool fsaFile::CopyEntries(fsaFile &From)
{
  int nSave = From.rewindDirEntries();
  ClearEntries();
  int n = From.dirEntryCount();
  fsaDirEntry *pEntry;
  bool bRtn = true;
  for(int i = 0; bRtn && (i < n); i++)
  {
    pEntry = From.nextDirEntry();
    bRtn = CopyAddDirEntry(*pEntry);
  }
  From.setDirEntry(nSave);
  return bRtn;
}
bool fsaFile::isValid()
{ 
  return !_bError; 
}

void fsaFile::BuildDirEntry()
{
  if(_pDir == NULL)
  {
    int nCount = this->dirEntryCount();
    _pDir = new fsaDirEntry("tdir",1,1023,nCount,28,nCount * 28,NULL,0);
  }
}

void fsaFile::SetAnalysisDateTime(fsaDate *pD, fsaTime *pT)
{
  fsaDirEntry *pDir;
  INT32 n = 1;
  size_t nCopy = 0;  // number of bytes to copy, 4 for date, 3 for time, 7 for both
  size_t nOffset = 4; // offset into fsaDirEntry raw data to copy bytes, 4 for date, 7 for time w/o date
  size_t nRawOffset = 0; // offset into pDateTimeRaw to copy time, 4 if date is used, 0 otherwise
  UINT8 pDateTimeRaw[7];
  UINT8 *pRawData;

  if(pD != NULL)
  {
    // set up date
    INT16 nYear = pD->year;
    if(!ByteOrder::LocalBigEndian())
    {
      ByteOrder::reverse(&nYear);
    }
    memcpy(pDateTimeRaw,&nYear,2);
    pDateTimeRaw[2] = pD->month;
    pDateTimeRaw[3] = pD->day;
    nRawOffset = 4;
    nCopy = 4;
  }
  else
  {
    nOffset = 7;
  }

  if(pT != NULL)
  {
    // set up time
    nCopy += 3;
    pDateTimeRaw[nRawOffset++] = pT->hour;
    pDateTimeRaw[nRawOffset++] = pT->minute;
    pDateTimeRaw[nRawOffset++] = pT->second;
  }

  if (nCopy)
  {
    while (pDir = FindDirEntry("STAT",n))
    {
      pRawData = (UINT8 *)pDir->GetDataRaw();
      memcpy(&pRawData[nOffset],pDateTimeRaw,nCopy);
	  n++;
    }
  }
}
