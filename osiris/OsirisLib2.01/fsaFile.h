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


    *********************************************************************

    class fsaFile

    constructor:

      fsaFile(fsaFile &)
      fsaFile()

    methods:
      fsaDirEntry *nextDirEntry()
        retrieve a pointer to the next sequential fsaDirEntry instance
        after reaching the end of the list, NULL is returned.  This 
        does NOT retrieve the entry which contains the location of the
        other entries.

      int rewindDirEntries()
        sets the counter of fsaDirEntry instances to 0 so that 
        nextDirEntry() retrieves the first entry.  This is set 
        automatically  from the constructor.

      int setDirEntry(int n)
        set the n'th entry for nextDirectoryEntry()

      int dirEntryCount()
        retrieves the number of instances of fsaDirEntry.  This does NOT
        include the entry  with the location of the other entries.

      fsaDirEntry *GetDirectoryDirEntry()
        retrieve the fsaDirEntry instance which contains the location of
        the other entries.  This should not be needed outside the class.

      fsaDirEntry *FindDirEntry(const char *psName, int nNumber);
        search for an instance of fsaDirEntry with the specified 
        name and number.  The search is NOT case sensitive.

      INT16 version()
        retrieve the version of the fsa file (x100).  This has been 
        tested with v1.01 where the integer 101 is retrieved.

      void SetAnalysisDateTime(fsaDate *pD, fsaTime *pT)
        set the analysis date and/or time in each "STAT" entry
        if pD is NULL, then the time is set and the date is unchanged
        if pT is NULL, then the date is set and the time is unchanged
        if both are NULL, then you are an idiot

      void SetAnalysisDate(fsaDate *pD) 
        set the analysis date in each "STAT" entry
      void SetAnalysisTime(fsaTime *pT)
        set the analysis time in each "STAT" entry
      
        Using SetAnalysisDateTime is more efficient than
          using SetAnalysisDate AND SetAnalysisTime

 */

#ifndef __FSA_FILE_H__
#define __FSA_FILE_H__

#include "rgfile.h"
#include "rgdefs.h"
#include "rgstring.h"
#include "fsaDirEntry.h"
#include <map>
#include <vector>
#include <stdlib.h>

class fsaFile
{
public:
  fsaFile(): 
    _bFileBigEndian (true),
    _pDir (NULL), 
    _nNextEntry (0),
    _nVersion (101), 
    _bError(false),
    _bWarning(false),
    _bUseWarning(false)
  {};
  fsaFile(fsaFile &f): 
    _bFileBigEndian (true),
    _pDir (NULL), 
    _nNextEntry (0) ,
    _nVersion (101), 
    _bError(false),
    _bWarning(false),
    _bUseWarning(false)
  {
    _bError = !CopyEntries(f);
  }
  virtual ~fsaFile();
  int WriteXML (RGFile& file);

  virtual bool isValid();

  bool Error()
  {
    return !isValid();
  }
  bool Warning()
  {
    return _bWarning;
  }
  bool UseWarning()
  {
    return _bUseWarning;
  }
  void SetUseWarning(bool b = true)
  {
    _bUseWarning = b;
  }

  fsaDirEntry *nextDirEntry()
  {
    fsaDirEntry *pRtn = NULL;
    if(_nNextEntry < _vecDirEntry.size())
    {
      pRtn = _vecDirEntry.at(_nNextEntry);
      _nNextEntry++;
    }

    return pRtn;
  }

  UINT32 setDirEntry(UINT32 n)
  {
    int nRtn = (UINT32) _nNextEntry;
    _nNextEntry = n;
    return nRtn;
  }
  fsaDirEntry *getDirEntry(UINT32 n)
  {
    fsaDirEntry *p = (n < (UINT32)_vecDirEntry.size())
      ? _vecDirEntry.at(n)
      : NULL;
    return p;
  }
  int rewindDirEntries()
  {
    return setDirEntry(0);
  }

  int dirEntryCount()
  {
    return (int)_vecDirEntry.size();
  }
  fsaDirEntry *GetDirectoryDirEntry()
  {
    if(_pDir == NULL)
    {
      BuildDirEntry();
    }
    return _pDir;
  }

  fsaDirEntry *FindDirEntry(const char *psName, int nNumber);
  bool CopyAddDirEntry(const fsaDirEntry &Entry);
  bool AddDirEntry(fsaDirEntry *pEntry);
  bool CopyEntries(fsaFile &From);
  void ClearEntries();
  void SetAnalysisDateTime(fsaDate *pD, fsaTime *pT);
  void SetAnalysisTime(fsaTime *pT)
  {
    SetAnalysisDateTime((fsaDate *)NULL,pT);
  }
  void SetAnalysisDate(fsaDate *pD)
  {
    SetAnalysisDateTime(pD,(fsaTime *)NULL);
  }
  INT16 version()
  {
    return _nVersion;
  }
  void Sort();
  bool IsSorted();
protected:
  typedef map<RGString, fsaDirEntry *> MapDirEntry;
  void cleanup();
  void BuildDirEntry();

  bool _bFileBigEndian;
  MapDirEntry _mapDirEntry;
  vector<fsaDirEntry *> _vecDirEntry;
  fsaDirEntry *_pDir;
  vector<fsaDirEntry *>::size_type _nNextEntry;
  INT16 _nVersion;
  bool _bError;
  bool _bWarning;
  bool _bUseWarning;
};


#endif
