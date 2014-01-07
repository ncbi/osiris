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
*  FileName: fsaSaxHandler.cpp
*
*/
#include "fsaSaxHandler.h"
#include "fsaFile.h"
#include "Exception.h"
#include "Malloc.h"



fsaSaxHandler::fsaSaxHandler(fsaFile *pFile) :
  m_strName("Name"),
  m_strID("ID"),
  m_strType("Type"),
  m_strElements("Elements"),
  m_strSize("Size"),
  m_strValue("Value")

{
  m_pFile = pFile;
  m_nTagLevel = 0;
}
fsaSaxHandler::~fsaSaxHandler() {}

void fsaSaxHandler::startDocument() {}

void fsaSaxHandler::endDocument()
{
  if(!m_pFile->IsSorted())
  {
    m_pFile->Sort();
  }
}

void fsaSaxHandler::startElement(
  const char *uri, 
  const char *localName, 
  const char *qName, 
  const Attributes &attr)
{
  if( m_nTagLevel || (!strcmp(localName,"Tag")) )
  {
    m_nTagLevel++;
    m_strCurrentValue = "";
  }
}

void fsaSaxHandler::endElement(
  const char *uri, 
  const char *localName, 
  const char *qName)
{
  if(m_nTagLevel)
  {
    m_nTagLevel--;
    if(!m_nTagLevel)
    {
      // we found </Tag>
      FinishTag();
    }
    else
    {
      RGString str(localName);
      m_mapTag.insert(MAPSTR::value_type(str,m_strCurrentValue));
      m_strCurrentValue = "";
    }
  }
}


void fsaSaxHandler::characters(const char *ch)
{
  if(m_nTagLevel == 2)
  {
    m_strCurrentValue += ch;
  }
}
bool fsaSaxHandler::GetValue(const RGString &sName, RGString *psValue)
{
  MAPSTR::iterator itr = m_mapTag.find(sName);
  bool bRtn = (itr != m_mapTag.end());
  if(bRtn)
  {
    (*psValue) = itr->second;
  }
  return bRtn;
}
void fsaSaxHandler::FinishTag()
{
  /* example
  <Tag>
    <Name>AOFF</Name>
    <ID>2</ID>
    <Type>long</Type>
    <Elements>1</Elements>
    <Size>4</Size>
    <Value>0</Value>
  </Tag>
  */
  RGString sName;
  RGString sType;
  RGString sID;
  RGString sElements;
  RGString sSize;
  RGString sValue;
  INT32 nID = 0;
  INT32 nElements = 0;
  INT16 nElementType = 0;
  INT32 nElementSize = 0;
  MAPSTR::iterator itrEnd = m_mapTag.end();
  bool bProblem = !GetValue(m_strName,&sName);
  bProblem |= !GetValue(m_strType, &sType);
  bProblem |= !GetValue(m_strID, &sID);
  bProblem |= !GetValue(m_strElements, &sElements);
  bProblem |= !GetValue(m_strSize, &sSize);
  bProblem |= !GetValue(m_strValue,&sValue);
  if(bProblem)
  {
    RGString s;
    s = "Missing value in tag: \n<Name>";
    s += sName;
    s += "</Name>\n<ID>";
    s += sID;
    s += "</ID>\n<Type>";
    s += sType;
    s += "</Type>\n<Elements>";
    s += sElements;
    s += "</Elements>\n<Size>";
    s += sSize;
    s += "</Size>\n<Value>";
    s += sValue;
    s += "</Value>\n";
    throw new Exception((const char *)s);
  }
  m_mapTag.clear();
  nID = (INT32)atoi((const char *)sID);
  nElements = (INT32)atoi((const char *)sElements);
  nElementSize = (INT32)atoi((const char *) sSize);
  INT16 nType = fsaDirEntry::GetTypeFromString(sType);
  fsaDirEntry *pFsa = 
    new fsaDirEntry((const char *) sName,nID,
      nType,nElements,nElementSize,(const char *)sValue);
  m_pFile->AddDirEntry(pFsa);
}
