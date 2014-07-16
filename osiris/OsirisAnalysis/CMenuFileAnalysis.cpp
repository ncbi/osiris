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
*  FileName: CMenuFile.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CMenuFileAnalysis.h"
#include <wx/menuitem.h>
#include "CXSLExportFileType.h"
#include "nwx/CIncrementer.h"
#include "CMenuBar.h"

const int CMenuFileAnalysis::UPDATE_INTERVAL(5000);
const int CMenuFileAnalysis::MAX_FILE_MENU_COUNT(1);


const wxString CMenuFileAnalysis::EXPORT_CMF("Export CMF File...");
const wxString CMenuFileAnalysis::EXPORT_CMF_HELP(
  "Export analysis data to a CODIS CMF 3.2 file.");


CMenuFileAnalysis::CMenuFileAnalysis() :
  m_TimerInterval(CMenuFileAnalysis::UPDATE_INTERVAL,0),
  m_pSubMenu(NULL),
  m_nUpdating(0),
  m_bNeedsUpdate(true)
{
  size_t nInsert = CMenuBar::GetPosition(this,IDlistMRU);
  Insert(nInsert++,wxID_SAVE);
  Insert(nInsert++,wxID_SAVEAS);
  Insert(nInsert++,
    IDExportCMF,EXPORT_CMF,EXPORT_CMF_HELP);
}

CMenuFileAnalysis::~CMenuFileAnalysis()
{}

void CMenuFileAnalysis::_Clear()
{
  wxMenuItem *pItem;
  if(m_vsFileTypes.IsEmpty())
  {}
  else if(m_pSubMenu != NULL)
  {
    pItem = Remove(m_pSubMenu);
    if(pItem != NULL)
    {
      delete pItem;
      m_pSubMenu = NULL;
    }
  }
  else
  {
    int i;
    int nID = IDExportUser;
    int nIDend = nID + (int)m_vsFileTypes.GetCount();
    for(i = nID; i < nIDend; i++)
    {
      Delete(i);
    }
  }
  m_vsFileTypes.Empty();
}

bool CMenuFileAnalysis::CheckUpdate()
{
  // check if the export configuration has changed
  // (labels only) and if so, update and return true
  // if no change, return false
  bool bRtn = false;
  if(!IsUpdating())
  {
    CIncrementer XXX(m_nUpdating);
    CExportFiles *pFiles = CExportFiles::GetGlobal();
    bool bSetup = m_bNeedsUpdate;
    if(pFiles == NULL)
    {
      bSetup = false;
    }
    else
    {
      if(pFiles->CheckXSLmodification())
      {
        bSetup = true;
      }
      if(pFiles->CheckFileModification(true))
      {
        bSetup = true;
      }
    }
    if(bSetup)
    {
      wxArrayString as;
      vector<bool> vb;
      size_t i = 0;
      m_bNeedsUpdate = false;
      pFiles->GetItems(&as);
      size_t nCount = as.GetCount();
      if(nCount)
      {
        vb.reserve(nCount);
        for(i = 0; i < nCount; i++)
        {
          CExportFiles::iterator itr = 
            pFiles->Get()->find(as.Item(i));
          vb.push_back(itr->second->XSLfileOK());
        }
      }

      if(as != m_vsFileTypes || vb != m_vbValid)
      {
        wxString sLabel;
        const wxChar *psPrefix(wxS(""));
        const wxChar *psSuffix(wxS("..."));
        int nID = IDExportUser;
        int nPOS;
        _Clear();
        if(nCount > ID_MAX_USER_EXPORT)
        {
          nCount = ID_MAX_USER_EXPORT;
        }
        wxMenu *pMenu;
        size_t nExportPosition = CMenuBar::GetPosition(this,IDlistMRU);

        if((int) nCount > MAX_FILE_MENU_COUNT)
        {
          nPOS = 0;
          pMenu = new wxMenu(0L);
          m_pSubMenu = new wxMenuItem(
            this,IDExportUserSubMenu,
            "Export",
            wxEmptyString,
            wxITEM_NORMAL,
            pMenu);
          Insert(nExportPosition,m_pSubMenu);
        }
        else
        {
          psPrefix = wxS("Export ");
          nPOS = nExportPosition;
          pMenu = this;
        }

        for(i = 0; i < nCount; i++)
        {
          sLabel = psPrefix;
          sLabel.Append(as.Item(i));
          sLabel.Append(psSuffix);
          wxMenuItem *pItem = pMenu->Insert(nPOS,nID,sLabel);
          pItem->Enable(vb.at(i));
          nPOS++;
          nID++;
        }
        m_vsFileTypes = as;
        m_vbValid = vb;
        bRtn = true;
        if(!TableExportsEnabled())
        {
          EnableTableExports(false);
        }
      }
    }
  }
  return bRtn;
}

void CMenuFileAnalysis::OnTimer(wxTimerEvent &e)
{
  if(m_TimerInterval.CheckTimer(e))
  {
    CheckUpdate();
  }
}

CXSLExportFileType *CMenuFileAnalysis::GetFileTypeByID(int nID)
{
  CXSLExportFileType *pRtn = NULL;
  if(nID >= IDExportUser && nID < IDExportUserSubMenu)
  {
    size_t nOffset = (size_t)(nID - IDExportUser);
    const wxString &sName = m_vsFileTypes.Item(nOffset);
    CExportFiles *pFiles = CExportFiles::GetGlobal();
    pRtn = pFiles->Find(sName);
  }
  return pRtn;
}

void CMenuFileAnalysis::EnableTableExports(bool bEnable)
{
  Enable(IDExportCMF,bEnable);
  if(m_vsFileTypes.IsEmpty())
  {}
  else if(m_pSubMenu != NULL)
  {
    m_pSubMenu->Enable(bEnable);
  }
  else
  {
    int i;
    int ndx = 0;
    int nID = IDExportUser;
    int nIDend = nID + (int)m_vsFileTypes.GetCount();
    for(i = nID; i < nIDend; i++)
    {
      Enable(i,bEnable && m_vbValid.at(ndx));
      ndx++;
    }
  }
}
