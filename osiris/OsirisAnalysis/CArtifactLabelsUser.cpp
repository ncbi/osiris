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
*  FileName: CArtifactLabelsUser.cpp
*  Author:   Douglas Hoffman
*
*  Class for loading and storing the labels shown
*  for artifacts on a plot as entered by the user.
*  These labels override the default labels in the
*  OSIRIS distribution and are stored in ArtifactLabels.xml
*  in the installation's site folder.
* 
*/
#include <wx/filename.h>
#include "CArtifactLabelsUser.h"
#include "CArtifactLabels.h"
#include "TArtifactGreater.h"
#include "mainApp.h"
#include "ConfigDir.h"
#include "nwx/vectorSort.h"

CArtifactGroupUser::CArtifactGroupUser(const IArtifactGroup &g)
{
  RegisterAll(true);
  m_nPriorityUser = g.GetPriorityUser();
  m_nPriority = g.GetPriorityDefault();
  m_sDisplayUser = g.GetDisplayUser();
  m_sDisplay = g.GetDisplayDefault();
  m_sLabel = g.GetLabel();
  m_sID = g.GetID();
}

void CArtifactGroupUser::RegisterAll(bool b)
{
  nwxXmlPersist::RegisterAll(b);
  RegisterWxString(wxT("ID"),  &m_sID);
  RegisterWxString(wxT("Display"),&m_sDisplayUser);
  RegisterInt(wxT("Priority"),    &m_nPriorityUser);
}

const wxString CArtifactLabelsUser::g_sROOT(wxT("UserArtifactLabels"));

CArtifactLabelsUser::CArtifactLabelsUser(bool bLoadFile)
{
  RegisterAll(true);
  if(bLoadFile)
  {
    wxString sFileName = mainApp::GetConfig()->GetArtifactLabelsUserFileName();
    if(wxFileName::IsFileReadable(sFileName))
    {
      LoadFile(sFileName,false);
    }
  }
}

void CArtifactLabelsUser::Sort()
{
  typedef class TArtifactGreater<CArtifactGroupUser> ALESS;
  vectorSort<CArtifactGroupUser *,ALESS>::SortMulti(&m_vector);

  //
  //  renumber priority
  //
  vector<CArtifactGroupUser *>::iterator itr;
  int nPriority;
  for(itr = m_vector.begin(), nPriority = m_vector.size();
    itr != m_vector.end();
    ++itr, --nPriority)
  {
    (*itr)->SetPriority(nPriority);
  }
}

