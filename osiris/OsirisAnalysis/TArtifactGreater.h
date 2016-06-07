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
*  FileName: TArtifactGreater.h
*  Author:   Douglas Hoffman
*
*  template class for sorting artifact labels
*  used by CArtifactGroup in CArtifactLabels.cpp and
*   CArtifactGroupUser in CArtifactLabelsUser.cpp
* 
*/
#ifndef __T_ARTIFACT_GREATER_H__
#define __T_ARTIFACT_GREATER_H__


template<class T> class TArtifactGreater
{
public:
  TArtifactGreater<T>() {;}
  bool operator()(const T &g1,const T &g2) const
  {
    int n1 = g1.GetPriority();
    int n2 = g2.GetPriority();
    bool bRtn = (n1 > n2) ||
      ((n1 == n2) && (g1.GetDisplay() > g2.GetDisplay()));
    return bRtn;
  }
  bool operator()(const T *p1,const T *p2) const
  {
    return (*this)(*p1,*p2);
  }

};


#endif