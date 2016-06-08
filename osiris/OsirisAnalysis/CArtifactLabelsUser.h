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
*  FileName: CArtifactLabelsUser.h
*  Author:   Douglas Hoffman
*
*  Class for loading and storing the labels shown
*  for artifacts on a plot as entered by the user.
*  These labels override the default labels in the
*  OSIRIS distribution and are stored in SiteArtifactLabels.xml
*  in the installation's site folder.
*
*  When loading from a file, the info is merged with the global 
*  CArtifactLabels instance which contains data from ArtifactLabels.xml 
*  from the OSIRIS distribution.
*
*  When editing, a new instance of CArtifactLabelsUser is created
*  from the instance of CArtifactLabels, the changes are written
*  to SiteArtifactLabels.xml and merged back into the same instance
*  of CArtifactsLabels.
* 
*/
#ifndef __C_ARTIFACT_LABELS_USER_H__
#define __C_ARTIFACT_LABELS_USER_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "IArtifactGroup.h"

class CArtifactGroup;
class CArtifactLabels;

class CArtifactGroupUser : public nwxXmlPersist, public IArtifactGroup
{
public:
  CArtifactGroupUser()
  {
    RegisterAll(true);
    ClearDistDefaults();
  }
  CArtifactGroupUser(const IArtifactGroup &g);
  virtual ~CArtifactGroupUser() {}
  void Set(const IArtifactGroup &g);
  virtual const wxString &GetID() const
  {
    return m_sID;
  }
  virtual const wxString &GetLabel() const
  {
    return m_sLabel;
  }
  virtual const wxString &GetDisplay() const
  {
    const wxString *pRtn = 
      m_sDisplayUser.IsEmpty()
        ? &m_sDisplay
        : &m_sDisplayUser;
    return *pRtn;
  }
  virtual const wxString &GetDisplayDefault() const
  {
    return m_sDisplay;
  }
  virtual const wxString &GetDisplayUser() const
  {
    return m_sDisplayUser;
  }
  virtual int GetPriority() const
  {
    return (m_nPriorityUser >= 0) ? m_nPriorityUser : m_nPriority;
  }
  virtual int GetPriorityDefault() const
  {
    return m_nPriority;
  }
  virtual int GetPriorityUser() const
  {
    return m_nPriorityUser;
  }
  virtual void SetDisplay(const wxChar *p)
  {
    m_sDisplayUser = p;
  }
  virtual void SetPriority(int n)
  {
    m_nPriorityUser = n;
  }
  virtual void Init()
  {
    nwxXmlPersist::Init();
    ClearDistDefaults();
  }
  virtual void Init(void *p)
  {
    nwxXmlPersist::Init(p);
    if(p == (void *)this)
    {
      ClearDistDefaults();
    }
  }
  void ClearDistDefaults()
  {
    m_sDisplay.Clear();
    m_nPriority = -1;
  }
  void ClearUserData()
  {
    m_sDisplayUser.Clear();
    m_nPriorityUser = -1;
  }
protected:
  virtual void RegisterAll(bool = false);
private:
  wxString m_sID;
  wxString m_sLabel;
  wxString m_sDisplay;
  wxString m_sDisplayUser;
  int m_nPriority;
  int m_nPriorityUser;
};

class CArtifactLabelsUser : public nwxXmlPersist
{
public:
  CArtifactLabelsUser(bool bLoadFile = false);
  virtual ~CArtifactLabelsUser()
  {
    m_io.Cleanup();
    vectorptr<CArtifactGroupUser>::cleanup(&m_vector);
  }
  virtual const wxString &RootNode(void) const
  {
    return g_sROOT;
  }
  virtual bool LoadFile(const wxString &sFileName, bool bLock)
  {
    bool bRtn = nwxXmlPersist::LoadFile(sFileName,bLock);
    Sort();
    return bRtn;
  }
  void AddLabel(const IArtifactGroup &g)
  {
    m_vector.push_back(new CArtifactGroupUser(g));
  }
  virtual void Init()
  {
    nwxXmlPersist::Init();
    m_io.Cleanup();
  }
  virtual void Init(void *p)
  {
    nwxXmlPersist::Init(p);
    if(p == (void *) this)
    {
      m_io.Cleanup();
    }
  }
  const CArtifactGroupUser *Get(size_t n) const
  {
    const CArtifactGroupUser *pRtn = (n < m_vector.size()) ? m_vector[n] : NULL;
    return pRtn;
  }
  CArtifactGroupUser *Get(size_t n)
  {
    CArtifactGroupUser *pRtn = (n < m_vector.size()) ? m_vector[n] : NULL;
    return pRtn;
  }
  size_t GetCount() const
  {
    return m_vector.size();
  }

  void Sort();
protected:
  virtual void RegisterAll(bool = false)
  {
    Register(wxT("SubGroup"),&m_io,&m_vector);
  }
private:
  TnwxXmlIOPersistVector<CArtifactGroupUser> m_io;
  std::vector<CArtifactGroupUser *> m_vector;
  static const wxString g_sROOT;
};

#endif