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
*  FileName: fsaSaxHandler.h
*
*/
#ifndef __FSA_SAX_HANDLER_H__
#define __FSA_SAX_HANDLER_H__

#include "SaxHandler.h"
#include "rgstring.h"

class fsaFile;

class fsaSaxHandler : public SaxHandler
{
public:
  fsaSaxHandler(fsaFile *pFile);
  virtual ~fsaSaxHandler();

  virtual void startDocument();

  virtual void endDocument();

  virtual void startElement(
    const char *uri,
    const char *localName,
    const char *qName,
    const Attributes &attr);

  virtual void endElement(
    const char *uri,
    const char *localName,
    const char *qName);

  virtual void characters(const char *ch);
private:
  void FinishTag();
  bool GetValue(const RGString &sName, RGString *psValue);
  typedef map<RGString,RGString> MAPSTR;
  fsaFile *m_pFile;
  int m_nTagLevel;
  RGString m_strCurrentValue;
  MAPSTR m_mapTag;

  const RGString m_strName;
  const RGString m_strID;
  const RGString m_strType;
  const RGString m_strElements;
  const RGString m_strSize;
  const RGString m_strValue;

};

#endif
