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
*  FileName: IArtifactGroup.h
*  Author:   Douglas Hoffman
*
*  interface class for CArtifactGroup and CArtifactGroupUser
*  in CArtifaceLabels.h/cpp and CArtifactLabelsUser.h/cpp
*  Used for copying items
* 
*/
#ifndef __I_ARTIFACT_GROUP_H__
#define __I_ARTIFACT_GROUP_H__

#include <wx/string.h>
class IArtifactGroup
{
public:
  virtual ~IArtifactGroup() {;}
  virtual const wxString &GetDisplayDefault() const = 0;
  virtual const wxString &GetDisplayUser() const = 0;
  virtual const wxString &GetDisplay() const = 0;
  virtual const wxString &GetLabel() const = 0;
  virtual const wxString &GetID() const = 0;
  virtual int GetPriority() const = 0;
  virtual int GetPriorityDefault() const = 0;
  virtual int GetPriorityUser() const = 0;
  virtual void SetDisplay(const wxChar *p) = 0;
  virtual void SetPriority(int n) = 0;
  static int UserDataEqual(const IArtifactGroup &x1, const IArtifactGroup &x2)
  {
    return (x1.GetDisplayUser() == x2.GetDisplayUser()) &&
      (x1.GetPriorityUser() == x2.GetPriorityUser()) &&
      (x1.GetLabel() == x2.GetLabel());
  }
  static int UserDataEqual(const IArtifactGroup *p1, const IArtifactGroup *p2)
  {
    return UserDataEqual(*p1,*p2);
  }
protected:
  IArtifactGroup() {;}

};

#endif