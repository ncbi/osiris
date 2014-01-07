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
*  FileName: COARmisc.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_MISC_H__
#define __COAR_MISC_H__

#include <math.h>
#include <time.h>
#include <ctype.h>
#include <wx/datetime.h>
#include <wx/string.h>
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"

#define COARcopy(m) { m = x.m; }
#define COARcopyVP(T,m) { vectorptr<T>::copy(&m,x.m); }



//************************************************* COARalerts


class COARalerts : public nwxXmlPersist
{
  // uses COARfile::g_iovn to handle the XML IO for a list of integers

public:
  COARalerts()
  {
    RegisterAll(true);
  }
  COARalerts(const COARalerts &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARalerts() {;}
  virtual bool Skip(void *)
  {
    return m_vnMessageNumber.empty();
  }
  vector<int> *Get() { return &m_vnMessageNumber;}
  const vector<int> *Get() const { return &m_vnMessageNumber;}
  const vector<int> *GetConst() const { return &m_vnMessageNumber;}
  vector<int> * operator ->()    { return Get(); }
  const vector<int> * operator ->() const    { return Get(); }
  virtual void RegisterAll(bool = false);
  void AppendToList(vector<int> *pvn) const;
private:
  vector<int> m_vnMessageNumber;
};


#endif
