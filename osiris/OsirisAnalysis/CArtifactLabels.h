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
*  FileName: CArtifactLabels.ch
*  Author:   Douglas Hoffman
*
*  Class for loading and retrieving the labels shown
*  for artifacts on a plot.  These are stored in the file,
*  ArtifactLabels.xml in the Config/LadderSpecifications directory
*  of the OSIRIS distribution
* 
*/
#ifndef __C_ARTIFACT_LABELS_H__
#define __C_ARTIFACT_LABELS_H__

#include <wx/string.h>
#include <wx/regex.h>
#include <wx/arrstr.h>
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "IArtifactGroup.h"

class CArtifactLabelsUser;

class CArtifactGroup : public nwxXmlPersist, public IArtifactGroup
{
public:
  CArtifactGroup();
  virtual ~CArtifactGroup() 
  {
    _cleanupRegEx();
  }
  void SetDisplay(const IArtifactGroup &g);
  bool IsStringMatch(const wxString &s) const;
  bool IsMsgTypeMatch(const wxString &s) const;
  virtual const wxString &GetID() const
  {
    return m_sID;
  }
  virtual const wxString &GetLabel() const
  {
    return m_sLabel;
  }
  virtual const wxString &GetDisplayDefault() const
  {
    return m_sDisplay;
  }
  virtual const wxString &GetDisplayUser() const
  {
    return m_sDisplayUser;
  }
  virtual const wxString &GetDisplay() const
  {
    const wxString *pRtn =
      m_sDisplayUser.IsEmpty()
        ? &m_sDisplay
        : &m_sDisplayUser;
    return *pRtn;
  }
  virtual int GetPriorityDefault() const
  {
    return m_nPriority;
  }
  virtual int GetPriorityUser() const
  {
    return m_nPriorityUser;
  }
  virtual int GetPriority() const
  {
    return (m_nPriorityUser >= 0)
      ? m_nPriorityUser
      : m_nPriority;
  }
  virtual void SetPriority(int n)
  {
    m_nPriorityUser = n;
  }
  virtual void SetDisplay(const wxChar *p)
  {
    m_sDisplayUser = p;
  }
  virtual void Init()
  {
    nwxXmlPersist::Init();
    SetDefaults();
  }
  virtual void Init(void *p)
  {
    nwxXmlPersist::Init(p);
    if(p == (void *)this)
    {
      SetDefaults();
    }
  }
  void SetDefaults()
  {
    m_sDisplayUser.Clear();
    m_nPriorityUser = -1;
  }
protected:
  virtual void RegisterAll(bool = false);
private:
  void _buildRegEx() const;
  void _cleanupRegEx() const;
  nwxXmlIOPersistVectorWxString m_ios;
  std::vector<wxString> m_vsMsgName;
  std::vector<wxString> m_vsSearchString;
  mutable std::vector<wxRegEx *> m_pvSearchRE;
  wxString m_sID;
  wxString m_sLabel;
  wxString m_sDisplay;
  wxString m_sDisplayUser;
  int m_nPriority;
  int m_nPriorityUser;
};


class CArtifactLabels : public nwxXmlPersist
{
public:
  CArtifactLabels();
  virtual ~CArtifactLabels();
  virtual bool LoadFile(const wxString &sFileName, bool bLock);
  virtual void Init()
  {
    nwxXmlPersist::Init();
    m_io.Cleanup();
    m_bOK = true;
  }
  virtual void Init(void *p)
  {
    nwxXmlPersist::Init(p);
    if(p == (void *) this)
    {
      m_io.Cleanup();
    }
  }
  bool IsOK() const
  {
    return m_bOK;
  }
  const CArtifactGroup *Get(size_t n) const
  {
    const CArtifactGroup *pRtn = (n < m_vector.size()) ? m_vector[n] : NULL;
    return pRtn;
  }
  size_t GetCount() const
  {
    return m_vector.size();
  }
  void Sort();
  bool SetUserLabels(const CArtifactLabelsUser &labels);
  CArtifactLabelsUser *BuildUserLabels() const;
  void ResetDefaults();
  const wxString &GetDisplayFromString(const wxString &sArtifactString) const;
  const wxString &GetDisplayFromMsgType(const wxString &sMsgType) const;
  const wxArrayString &GetChoiceList() const;
protected:
  virtual void RegisterAll(bool = false)
  {
    Register(wxT("SubGroup"),&m_io,&m_vector);
  }
private:
  void _ClearChoiceList() const
  {
    m_asChoiceList.Clear();
  }
  TnwxXmlIOPersistVector<CArtifactGroup> m_io;
  std::vector<CArtifactGroup *> m_vector;
  mutable wxArrayString m_asChoiceList;
  bool m_bOK;
  static const wxString g_sDEFAULT_LABEL;
};


#endif