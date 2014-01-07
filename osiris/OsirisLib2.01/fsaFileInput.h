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
 *  fsaFileInput.h - class for reading Applied Biosystems' fsa files
 *    SEE ALSO: byteOrder.h/cpp; 
 *    example program - ../fsa2xml.cpp
 *
 *  classes:
 *
     fsaFileInput - class for reading data from an fsa file

     To use this class, you must first read the documentation 
        available at:
http://www.appliedbiosystems.com/support/software_community/ABIF_File_Format.pdf


    *********************************************************************

    class fsaFileInput

    constructor:

      fsaFileInput(const char *psFileName)
       psFileName - name of the file to be opened
       The constructor opens the file and reads all of the 
       directory entries.

 */

#ifndef _FSA_FILE_INPUT_H_
#define _FSA_FILE_INPUT_H_
#include "fsaFile.h"
#include "IReader.h"
class fsaFileInput : public fsaFile, public IReader
{
public:
  static const UINT8 *MEMSTR(
    const UINT8 *pBuffer, size_t nLen, const char *pSearchString, size_t nSearchLen = -1);
#ifdef _DEBUG
  static bool TESTMEMSTR();
#endif
  fsaFileInput(const char* name, bool bUseWarning = false);
  virtual ~fsaFileInput();
  virtual bool isValid();

  virtual bool Read(void *pData, UINT32 nOffset, size_t nBytes)
  {
    bool bRtn = _SeekTo(nOffset) && _ReadBytes(pData,nBytes);
    return bRtn;
  }
  bool IsHID() const
  {
    return _bHID;
  }
private:
  enum
  {
     NADDRS = 12
  };
  void _CleanupBuffer()
  {
    if(_pBuffer != NULL)
    {
      free(_pBuffer);
      _pBuffer = NULL;
    }
    _pBufferSeek = NULL;
    _nBufferSize = 0;
  }
  bool _SeekTo(size_t nOffset);
  size_t _Tell()
  {
    return (size_t)(_pBufferSeek - _pBuffer);
  }
  bool _ReadBytes(void *pBuffer, size_t n);
  void Insert(UINT8 *p);
  bool _LoadFileBuffer(const char *psName);
  bool _EntryOK(const UINT8 *p, bool bCheckType = false) const;
  const UINT8 *_FindEndEntry(const UINT8 *pMid, bool bFirst) const;
  void _SetupHIDOffset();

  UINT8 *_pBuffer;
  UINT8 *_pBufferSeek;
  UINT32 _anHIDoffsets[NADDRS];
  size_t _nOffsets;
  size_t _nBufferSize;
  bool _bHID;
};


#endif
