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
 *  fsaDirEntry.h - class for storing data from Applied Biosystems' fsa files
 *    SEE ALSO: byteOrder.h/cpp; 
 *    example program - ../fsa2xml.cpp
 *
 *  classes:
 *
     fsaDirEntry - class containing the actual data for an entry 
        in the fsa file

     To use this class, you must first read the documentation 
        available at:
http://www.appliedbiosystems.com/support/software_community/ABIF_File_Format.pdf

     data types:
          typedef struct
          {
            INT16 year;
            UINT8 month;
            UINT8 day;
          } fsaDate;

          typedef struct
          {
            UINT8 hour;
            UINT8 minute;
            UINT8 second;
            UINT8 hsecond;
          } fsaTime;

          typedef struct
          {
            INT32 d;
            INT32 u;
            UINT8 c;
            UINT8 n;
          } fsaThumb;

          typedef struct
          {
            INT32 position;
            INT16 height;
            INT32 beginPos;
            INT32 endPos;
            INT16 beginHt;
            INT16 endHt;
            INT32 area;
            INT32 volume;
            float fragSize;
            INT16 isEdited;
            char label[64];
          } fsaPeak;   //###  used for PEAK tag, type user
     typedef UINT8 fsaBOOL;

     from rgdefs.h:

       UINT8 is unsigned char
       INTnn is an int with nn bits:
         INT16 - short, INT32 - long
       UINTnn is an unsigned int with nn bits
         UINT8 - unsigned char
         UINT16 - unsigned short


    *********************************************************************
    class fsaDirEntry

    constructor:

      fsaDirEntry(const UINT8 *pEntry, bool bBig = true)
        pEntry - 28 bytes as read from the file
        bBig - true if file is big endian, currently only big endian
            is supported

      fsaDirEntry::fsaDirEntry
          (const char *_psName, // 4 byte name
          INT32 _nNumber,
          INT16 _nType, // see const INT16 TYPE_...
          INT32 _nNumElements,
          INT16 _nSize,     // use -1 to computer from type
          INT32 _nDataSize, // use -1 to computer from above
          void *_pData,     // this is raw data,
          bool bCopy = false // set to true to make a copy of _pData.  this->pData will be free()d upon destruction, regardless.
          );

          for the above parameters, see ABI documentation

      fsaDirEntry::fsaDirEntry(const fsaDirEntry &entry)
          // copy constructor
      fsaDirEntry &operator =(const fsaDirEntry &entry)

    static public constants:

        The following constants are data types used in the directory 
        entries, except TYPE_USER is 1024 however, any number > 1023 is
        a user defined entry.  The descriptions are for the actual data

        static const INT16 TYPE_BYTE;      // unsigned character
        static const INT16 TYPE_UINT8;    // unsigned character
        static const INT16 TYPE_CHAR;      //  char
        static const INT16 TYPE_WORD;      // short
        static const INT16 TYPE_UINT16;    // unsigned short
        static const INT16 TYPE_SHORT;    // short
        static const INT16 TYPE_INT16;    // short
        static const INT16 TYPE_LONG;      // long
        static const INT16 TYPE_INT32;    // long
        static const INT16 TYPE_FLOAT;    // float
        static const INT16 TYPE_DOUBLE;    // double
        static const INT16 TYPE_DATE;      
                          // fsaDate - structure described above
        static const INT16 TYPE_TIME;      
                          // fsaTime - structure described above
        static const INT16 TYPE_PSTRING;  
                          // pascal string - data contains a byte with 
                          // the length (0-255) followed by the nul 
                          // terminated string
        static const INT16 TYPE_CSTRING;
                          // c string - nul terminated string
        static const INT16 TYPE_THUMB;
                          // fsaThumb - structure described above
        static const INT16 TYPE_BOOL;
                          // unsigned char, false if 0, true otherwise
        static const INT16 TYPE_USER;
                          // user defined type

    public static methods:

      static RGString MakeKey(const char *psName, INT32 n)

        *psName - name of entry to be searched or stored
        INT32 n - number of the entry

        creates a key for storing and searching for a directory entry
        using its name and number.  The input is NOT case sensitive, 
        because the retrieved string is uppercase.  This shouldn't be 
        needed outside this class and fsaFileInput.

      static INT16 ElementTypeEnum(INT16 n)

        retrieves the element type of this element, except for a user 
        defined type (> 1023) 1024 is retrieved instead of the actual 
        number.  The number retrieved is one of the "static int" numbers
        described above.

        This is usually used for a case statement.

      static bool IsTypeUser(INT16 n)

        retrieves true if the element type is user defined, false 
        otherwise.  Based on the value retrieved by 
        ElementTypeEnum(INT16 n) above:

      static const char *GetTypeString(INT16 nType)

        Retrieves a description of the specified data type.
        One of the following:

        TYPE_USER:    "user_defined"
        TYPE_UINT8:   "byte"
        TYPE_CHAR:    "char"
        TYPE_PSTRING: "pString"
        TYPE_CSTRING: "cString"
        TYPE_BOOL:    "bool"
        TYPE_UINT16:  "word"
        TYPE_INT16:   "short"
        TYPE_FLOAT:   "float"
        TYPE_INT32:   "long"
        TYPE_DOUBLE:  "double"
        TYPE_DATE:    "date"
        TYPE_TIME:    "time"
        TYPE_THUMB:   "thumb"

    static INT16 GetTypeFromString(const char *ps);
        opposite of GetTypeString(INT16 nType) above

    public methods:

      RGString MakeKey()

        calls static RGStringMakeKey() (described above) with
        the name and number in this directory entry.  This shouldn't
        be needed outside the fsaDirEntry and fsaFileInput classes

    GET methods
      const char *Name() const - name of this entry

      INT16 ElementType() const - data type of this entry, one of 
                    the public static const number described above

      INT16 ElementTypeEnum() const - same as ElementType, except
          user defined types always return 1023

      INT32 Number() const - number of this entry
        
      UINT16 ElementSize() const
          size in bytes of a single element of this entry, eg
          for TYPE_INT32 retrieves 4
              TYPE_CHAR retrives 1

      UINT32 NumElements() const
          number of elements in the actual data

      UINT32 DataSize() const
          number of bytes of actual data in this entry.  Usually
          NumElements() * ElementSize();

      UINT32 Offset() const

          offset in this file of the actual data, or if DataSize() <= 4,
          then this contains the actual data.

      const RGString &DataToString(bool bThrow = false)

          retrieves a string representing the actual data.  This 
          can be used for ANY data type.
          for arrays of UINT8, INT16, UNIT16, INT32, float or double
          a space separated list of  values is retrieved.
          for fsaBOOL, a list of strings (either 'true' or 'false')
          separated by spaces is retrieved

      INT16 GetElementSize(bool bThrow = false)
          retrieve the size of a single data element based on its type (this->nElementType);

      bool IsTypeUser()
          returns true is this entry is a user defined type, 
          false otherwise

      const void *GetDataRaw(bool bThrow = false)
          retrieves the raw data read from the file, however if this
          is a little endian computer, then the appropriate bytes are 
          reversed

    The following 'Get...' methods retrieve a pointer ONLY if the type
    is appropriate.  If the data type is NOT appropriate, then:
      if bThrow == false, NULL is returned, 
      otherwise an exception of type 'Exception *' is thrown.
    For these methods, an array is retrieved and the length is
      the value retrieved by NumElements(), except for 
      user defined types, use DataSize() for the number of bytes

      const UINT8 *GetDataUINT8(bool bThrow = false)
          retrieves a pointer to the UINT data

      const char *GetDataString(bool bThrow = false)
          retrieves a pointer to a NUL terminated string.
          if the type is pString, then decrementing the
          pointer will give an unsigned one byte length
          of the string (0 - 255)
          This works for TYPE_PSTRING, TYPE_STRING and TYPE_CHAR
        
      const UINT16 *GetDataUINT16(bool bThrow = false)
          pointer to an array of unsigned short

      const INT16 *GetDataINT16(bool bThrow = false)
          pointer to an array of short

      const INT32 *GetDataINT32(bool bThrow = false)
          pointer to an array of long

      const float *GetDataFloat(bool bThrow = false)
          pointer to an array of float

      const double *GetDataDouble(bool bThrow = false)
          pointer to an array of double

      const fsaDate *GetDataDate(bool bThrow = false)
          pointer to an array of struct fsaDate

      const fsaTime *GetDataTime(bool bThrow = false)
          pointer to an array of struct fsaTime

      const fsaThumb *GetDataThumb(bool bThrow = false);
          pointer to an array of struct fsaThumb

      const fsaBOOL *GetDataBool(bool bThrow = false);
          pointer to an array of fsaBool, which is actually UINT8

      const UINT8 *GetDataUser(bool bThrow = false);
          pointer to user defined data -- unchanged even if this is
          a little endian architecture

      static void *BuildRawPeakData(fsaPeak *pData, UINT32 nNumElements)
         returns a pointer to raw data created from an array of fsaPeak
         this pointer is created with malloc and must be freed
         this pointer can be used for the constructor of a "PEAK" entry
      static void BuildRawPeakData(fsaPeak *pData, UINT32 nNumElements, void *pDest)
         converts the peak data in pData into raw data into pDest
         pDest must have at least (nNumElements * 96) bytes
      static fsaPeak *BuildPeakData(void *p, UINT32 nDataSize)
         converts raw peak data into an array of fsaPeak structures
         the returned pointer is allocated using malloc()
         and should be free'd using one of the following:
            fsaDirEntry::FreeDataPeak(fsaPeak *)
            free(void *);
            an instance of class Malloc 
         this is used by GetDataPeak() illustrated below

      fsaPeak *GetDataPeak(bThrow = false)
         returns a pointer to an array of fsaPeak structures
         the returned pointer is allocated using malloc()
         and should be free'd using one of the following:
            fsaDirEntry::FreeDataPeak(fsaPeak *)
            free(void *);
            an instance of class Malloc 

      static void FreeDataPeak(fsaPeak *p) -- calls free((void *)p);
        typically used with a pointer returned from GetDataPeak() or BuildPeakData()

      // the following methods are used for modifying the data

      bool SetDataINT16(
          INT16 *pData,
          INT32 _nNumElements,
          bool bCopy = true,  // set to true to make a copy of pData.  
                      // pData will be free()d upon destruction regardless of this value
          bool bThrow = false)

      bool SetDataString(const char *pData, bool bCopy = true, bool bThrow = false);
      // build raw data from a string

      bool SetDataTime(
        fsaTime *_pData, 
        int _nNumElements = 1, 
        bool bCopy = true, 
        bool bThrow = false);

      bool SetDataDate(
        fsaDate *_pData, 
        int _nNumElements = 1, 
        bool bCopy = true, 
        bool bThrow = false);

      bool SetDataPeak(
        fsaPeak *_pData, // array
        UINT32 _nNumElements, // array count
        bool bThrow = false); // note, the data is always copied


    other public methods:

      void FreeString()
        frees the RGString created when GetDataString() is called.
        This is handled by the destructor.

      void FreeData()
        frees the memory calculated when and 'Get' method is called.
        This is handled by the destructor.

      const char *GetTypeString() const
        calls GetTypeString(INT16 nType) (described) above
        to return a string representation of the data type, EXCEPT,
        if the type is 'char' and there is at least one byte with a 
        value < 32, the 'char_byte' is returned instaed of 'char'
        This is for compatibility with the Data File Converter 
        program available at 
        http://www.appliedbiosystems.com/support/software_community/tools_for_accessing_files.cfm

      static const char *FormatDateString(fsaDate *pData)
        formats the date in the form of mmm dd, yyyy (eg., Jun 01, 2006)
        and was created for the SMED tag, although I
        have no idea why ABI chose this format

 */

#ifndef _FSA_DIR_ENTRY_H_
#define _FSA_DIR_ENTRY_H_

#include "rgfile.h"
#include "rgdefs.h"
#include "rgstring.h"
#include "Malloc.h"
#include <map>
#include <vector>
#include <stdlib.h>
#include "IReader.h"


typedef UINT8 fsaBOOL;

typedef struct
{
  INT16 year;
  UINT8 month;
  UINT8 day;
} fsaDate;

typedef struct
{
  UINT8 hour;
  UINT8 minute;
  UINT8 second;
  UINT8 hsecond;
} fsaTime;

typedef struct
{
  INT32 d;
  INT32 u;
  UINT8 c;
  UINT8 n;
} fsaThumb;
typedef struct
{
  long position;
  short height;
  long beginPos;
  long endPos;
  short beginHt;
  short endHt;
  long area;
  long volume;
  float fragSize;
  short isEdited;
  char label[64];
} fsaPeak;   //  used for PEAK tag, type user

class fsaFileInput;


class fsaDirEntry
{
public:
  fsaDirEntry
    (const UINT8 *pEntry, bool bBig = true);
  fsaDirEntry
    (const char *_psName, // 4 byte name
    INT32 _nNumber,
    INT16 _nType, // see const INT16 TYPE_...
    INT32 _nNumElements,
    INT16 _nSize,
    INT32 _nDataSize, // use -1 to computer from above
    void *_pData,     // this is raw data,
    bool bCopy = false // set to true to make a copy of pData.  pData will be free()d upon destruction.
    );
  fsaDirEntry
    (const char *_psName, // 4 byte name
    INT32 _nNumber,
    INT16 _nType, // see const INT16 TYPE_...
    INT32 _nNumElements,
    INT32 _nDataSize, // use -1 to compute from above
    const char *psDataString // previously created from BuildString or BuildStringUser (below)
    );

  fsaDirEntry(const fsaDirEntry &entry);  // copy constructor

  fsaDirEntry &operator =(const fsaDirEntry &entry);

  void Setup(const UINT8 *pEntry, bool bBig = true)
  {
    _Cleanup();
    _Setup(pEntry,bBig);
  }


  virtual ~fsaDirEntry();
  static RGString MakeKey(const char *ps, INT32 n);
  
  static bool ValidateName(const UINT8 *ps);
  bool ValidateName() const
  {
    return ValidateName((const UINT8 *) sName);
  }
  
  RGString MakeKey()
  {
    return MakeKey((const char *)sName,nNumber);
  }

    // GET methods
  const char *Name() const
  {
    return sName;
  }
  INT16 ElementType() const
  {
    return nElementType;
  }
  static INT16 ElementTypeEnum(INT16 n)
  {
    return IsTypeUser(n) ? TYPE_USER : n;
  }
  INT16 ElementTypeEnum() const
  {
    return ElementTypeEnum(nElementType);
  }
  INT32 Number() const
  {
    return nNumber;
  }
  UINT16 ElementSize() const
  {
    return nElementSize;
  }
  UINT32 NumElements() const
  {
    return nNumElements;
  }
  UINT32 DataSize() const
  {
    return nDataSize;
  }
  UINT32 Offset() const
  {
    return nDataOffset;
  }
  UINT32 Junk() const
  {
    return nJUNK;
  }
  UINT32 SetOffset(UINT32 nOffset)
  {
    UINT32 nRtn = nDataOffset;
    nDataOffset = nOffset;
    return nRtn;
  }
  void SetNumElements(UINT32 nNum)
  {
    nNumElements = nNum;
  }
  

  static const INT16 TYPE_BYTE;
  static const INT16 TYPE_UINT8;
  static const INT16 TYPE_CHAR;
  static const INT16 TYPE_WORD;
  static const INT16 TYPE_UINT16;
  static const INT16 TYPE_SHORT;
  static const INT16 TYPE_INT16;
  static const INT16 TYPE_LONG;
  static const INT16 TYPE_INT32;
  static const INT16 TYPE_FLOAT;
  static const INT16 TYPE_DOUBLE;
  static const INT16 TYPE_DATE;
  static const INT16 TYPE_TIME;
  static const INT16 TYPE_PSTRING;
  static const INT16 TYPE_CSTRING;
  static const INT16 TYPE_THUMB;
  static const INT16 TYPE_BOOL;
  static const INT16 TYPE_USER;

  const RGString &DataToString(bool bThrow = false); // any data type
  static void *StringToData(INT16 nType, INT32 nSize, const char *psString);
  static size_t StringToData(void **pRtn, INT16 nType, const char *psString);
  static bool IsTypeUser(INT16 n)
  {
    return (n >= TYPE_USER);
  }
  bool IsTypeUser()
  {
    return IsTypeUser(nElementType);
  }
  const void *GetDataRaw(bool bThrow = false);
  const UINT8 *GetDataUINT8(bool bThrow = false);
  const char *GetDataString(bool bThrow = false);
  const UINT16 *GetDataUINT16(bool bThrow = false);
  const INT16 *GetDataINT16(bool bThrow = false);
  const INT32 *GetDataINT32(bool bThrow = false);
  const float *GetDataFloat(bool bThrow = false);
  const double *GetDataDouble(bool bThrow = false);
  const fsaDate *GetDataDate(bool bThrow = false);
  const fsaTime *GetDataTime(bool bThrow = false);
  const fsaThumb *GetDataThumb(bool bThrow = false);
  const fsaBOOL *GetDataBool(bool bThrow = false);
  const UINT8 *GetDataUser(bool bThrow = false);

  static RGString FormatDateTimeXS(const fsaDate *pDate, const fsaTime *pTime);
  static void BuildRawPeakData(fsaPeak *_pPeak, UINT32 _nNumElements, void *pDest);
  static void *BuildRawPeakData(fsaPeak *pPeak, UINT32 nNumElements);
  static fsaPeak *BuildPeakData(void *p, UINT32 nDataSize);
  fsaPeak *GetDataPeak(bool bThrow = false);
  static void FreeDataPeak(fsaPeak *p)
  {
    free((void *)p);
  }

  void FreeString()
  {
    if(pString != NULL)
    {
      delete pString;
      pString = NULL;
    }
  }
  void FreeData()
  {
    if(pData != NULL)
    {
      free(pData);
      pData = NULL;
    }
  }
  static const char *GetTypeString(INT16 nType);
  static INT16 GetTypeFromString(const RGString &s);
  static INT16 GetTypeFromString(const char *s)
  {
    return GetTypeFromString(RGString(s));
  }
  const char *GetTypeString() const
  {
    return 
      CharByte()
      ? "char_byte"
      : GetTypeString(nElementType);
  }
  int Compare(const fsaDirEntry *p) const
  {
    int nRtn = strcmp(sName,p->sName);
    if(nRtn) {}
    else if(nNumber < p->nNumber)
    {
      nRtn = -1;
    }
    else if(nNumber > p->nNumber)
    {
      nRtn = 1;
    }
    return nRtn;
  }
  bool operator < (const fsaDirEntry &p) const
  {
    return Compare(&p) < 0;
  }
  bool operator > (const fsaDirEntry &p) const
  {
    return Compare(&p) > 0;
  }
  bool operator >= (const fsaDirEntry &p) const
  {
    return Compare(&p) >= 0;
  }
  bool operator <= (const fsaDirEntry &p) const
  {
    return Compare(&p) <= 0;
  }
  bool operator == (const fsaDirEntry &p) const
  {
    return Compare(&p) == 0;
  }
  size_t BytesToRead();
  void LoadData(bool bThrow, void *pRawData, bool bLocalEndian = true, bool bCopy = false);
  void LoadData(bool bThrow, IReader *pReader);

  void LoadNewData(
    void *_pRawData,  // the actual data
    INT32 _nNumElements,  // the number of elements
    bool bCopy = true,
        // true if memory should be 
        // allocated and data copied, false 
        // if using the pointer stored in _pRawData.  Regardless, the memory will be freed using free()
    bool bThrow = true  // if true and an error occurs, then throw an exception
    )
  {
    this->nNumElements = nNumElements;
    this->nDataSize = nNumElements * nElementSize;
    FreeData();  // if data is already loaded, then free it, otherwise an error will occur.
    LoadData( bThrow, (void *) _pRawData, true, bCopy);
  }

  static void Split(char *ps, char delim, vector<char *> *pPtrs);
  static const char *FormatDateString(fsaDate *pDate, char* const dataField);
  bool SetDataString(const char *pData);
  bool SetDataINT16(
      INT16 *pData,
      INT32 _nNumElements,
      bool bCopy = true,  // set to true to make a copy of pData.  
                   // pData will be free()d upon destruction regardless of this value
      bool bThrow = false);
  bool SetDataTime(
    fsaTime *_pData, 
    int _nNumElements = 1, 
    bool bCopy = true, 
    bool bThrow = false);

  bool SetDataDate(
    fsaDate *_pData, 
    int _nNumElements = 1, 
    bool bCopy = true, 
    bool bThrow = false);

  bool SetDataPeak(
    fsaPeak *_pData,
    UINT32 _nNumElements,
    bool bThrow = false); // note, the data is always copied

#ifdef _DEBUG
  static bool UnitTest_XS();
#endif

protected:
  void _Cleanup()
  {
    FreeString();
    FreeData();
    Init();
  }
  bool isPeak()
  {
    return !strcmp(this->sName,"PEAK");
  }
  void _Setup(const UINT8 *pEntry, bool bBig);
  static void ReversePeakData(fsaPeak *p);
  bool SetDataRaw(void *p, UINT32 _nDataSize, INT32 _numElements,  bool bCopy);
  INT16 GetElementSize(bool bThrow = false);
  virtual void BuildStringUser();
  bool CharByte() const;
  bool CheckSize(size_t n, bool bThrow, const char *psType);
  size_t AllocateData(bool bThrow, void *pRawData = NULL);
  void LoadDataBytes(size_t n);
  void LoadDataDate(bool bThrow);
  void LoadDataTime(bool bThrow);
  void LoadDataThumb(bool bThrow);
  void LoadDataUser(bool bThrow);
  void SetupEndian(bool bThrow);
  void InvalidType(bool bThrow,const char *psType);

  //  BuildArray -- returns number of elements
  template<class T>
  static size_t BuildArray(
    T **pRtn,
    const char *ps,
    void (*unformatFunc)(T*, const char *)
    )
  {
    vector<char *> aPtrs;
    Malloc NewStr(strdup(ps));
    char *psNew = (char *)(NewStr.ptr());
    Split(psNew,' ',&aPtrs);
    size_t nCount = aPtrs.size();
    Malloc NewArray(nCount * sizeof(T));
    T* ptr = (T *)(NewArray.ptr());

    for(size_t i = 0; i < nCount; i++)
    {
      (*unformatFunc)(ptr, aPtrs.at(i));
      ptr++;
    }
    *pRtn = (T *)(NewArray.release());
    return nCount;
  }

  template<class T>
  static int BuildArray(
    void *pRtn, 
    const char *ps,
    size_t nSize,
    void (*unformatFunc)(T *, const char *)
  )
  {
    int nRtn = 0;
    T *pT = (T *)pRtn;
    Malloc NewStr(strdup(ps));
    char *psNew = (char *)(NewStr.ptr());
    vector<char *> aPtrs;
    size_t nMax = (nSize / sizeof(T));
    aPtrs.reserve(nMax + 1);
    Split(psNew,' ',&aPtrs);
    size_t nPtrSize = aPtrs.size();
    nRtn = int(nPtrSize) - (int)nMax;
    if(nMax > nPtrSize)
    {
      nMax = nPtrSize;
    }
    for(size_t i = 0; i < nMax; i++)
    {
      (*unformatFunc)(pT, aPtrs.at(i));
      pT++;
    }
    return nRtn;
  };

  template<class T> 
  void BuildString(
    T *pArray,
    void (*formatFunc)(T *,char *),
    size_t nItemSize)
  {
    char s[64];
    char *ps = &s[1]; // skip first byte in first loop
    s[0] = ' ';
    pString = new RGString
      ((size_t) ((nItemSize + 1) * nNumElements + 1));
    for(UINT32 i = 0; i < nNumElements; ++i)
    {
      (*formatFunc)(pArray,&s[1]); // format element
      pString->Append((const char *)ps);
      ps = s; // skip first byte in first loop only
      ++pArray;
    }
  }

private:
  void BuildIt
    (const char *_psName, // 4 byte name
    INT32 _nNumber,
    INT16 _nType, // see const INT16 TYPE_...
    INT32 _nNumElements,
    INT16 _nSize,     // use -1 to computer from type
    INT32 _nDataSize, // use -1 to computer from above
    void *_pData,     // this is raw data,
    bool bCopy        // set to true to make a copy of pData.  
                      // pData will be free()d upon destruction regardless of this value
    );
  void Init()
  {
    memset(sName,0,sizeof(sName));
    pData = NULL;
    pString = NULL;
    nNumber = 0;
    nNumElements = 0;
    nElementType = 0;
    nElementSize = 0;
    nDataSize = 0;
    nJUNK = 0;
    nDataOffset = 0;
  }

  char sName[5];
  INT32 nNumber;
  UINT32 nNumElements;
  UINT32 nDataSize;
  UINT32 nDataOffset;
  INT32  nJUNK;
  void *pData;
  RGString *pString;
  INT16 nElementType;
  UINT16 nElementSize;
  static char g_DateString[24];
};

#endif
