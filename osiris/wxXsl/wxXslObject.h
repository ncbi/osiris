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
*  FileName: wxXslObject.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __WX_XSL_OBJECT_H__
#define __WX_XSL_OBJECT_H__

#include <wx/object.h>
#include "wxXml2/wxXml2Object.h"
#include "libxslt/transform.h"


class WXDLLIMPEXP_BASE wxXslObject : public wxXml2Object
{
public:
  virtual ~wxXslObject()
  {
    g_nXslClass--;
    if(!g_nXslClass)
    {
      _cleanupXsl();
    }
  }
  wxXslObject()
  {
    if(!g_nXslClass)
    {
      _initXsl();
    }
    g_nXslClass++;
  }
protected:
  xsltStylesheetPtr GetSheetParam();
private:
  void _initXsl();
  void _cleanupXsl();
  static int g_nXslClass;
  static xsltStylesheetPtr g_pSheetParam;
  //DECLARE_ABSTRACT_CLASS(wxXml2Object);
};


#endif
