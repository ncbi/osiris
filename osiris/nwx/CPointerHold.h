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
*  FileName: CPointerHold.h
*  Author:   Douglas Hoffman
*
*  Saves the value of a pointer, changes it to the specified value
*  and restores it upon destruction
*
*/
#ifndef __C_POINTER_HOLD_H__
#define __C_POINTER_HOLD_H__

template <class T> class CPointerHold
{
public:
  CPointerHold<T>(T* &p, T *pNewValue = NULL) : m_p(p), m_pSave(p)
  {
    p = pNewValue;
  }
  virtual ~CPointerHold<T>()
  {
    m_p = m_pSave;
  }
private:
  T* &m_p;
  T* m_pSave;
};
#endif
