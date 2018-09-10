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
*  FileName: CGridCMF.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include <wx/arrstr.h>
#include <wx/settings.h>
#include <wx/dcclient.h>
#include "CDialogCMF.h"
#include "nwx/nwxXmlCMF.h"
#include "nwx/nwxGrid.h"
#include "COARfile.h"
#include "CLabSettings.h"
#include "CGridAnalysis.h"
#include "CGridCMF.h"
#include "COARsampleSort.h"

enum GRID_COLUMNS
{
  // if modified, COL_LABELS below must reflect changes

  EXCLUDE = 0,  // check box to exclude this sample
  SPECIMEN_TYPE,
  PARTIAL,      // checkbox for partial sample
  EMPTY_LOCI,
  SOURCE_ID,
  CASE_ID,
  SAMPLE,       //  sample name to override fsa/hid file name
  COMMENT,
  COLUMN_COUNT
};

const char * const CGridCMF::COL_LABELS[] = 
{
  // must correspond to enum above
  "Exclude",
  "Type",
  "Partial",
  "Missing Loci",
  "Source ID",
  "Case ID",
  "Sample Name",
  "Comments"
};

const wxString CGridCMF::DEFAULT("[Default]");
wxArrayString CGridCMF::m_asChoices;

CGridCMF::~CGridCMF() {}

wxGridCellChoiceEditor *CGridCMF::_CreateTypeEditor()
{
  if(m_asChoices.IsEmpty())
  {
    m_asChoices.Alloc(CLabSpecimenCategory::TypeCount() + 1);
    m_asChoices.Add(DEFAULT);
    CLabSpecimenCategory::AppendTypeArray(&m_asChoices,false);
  }
  wxGridCellChoiceEditor *pRtn = 
    new wxGridCellChoiceEditor(m_asChoices,false);
  return pRtn;
}

CGridCMF::CGridCMF(
  CDialogCMF *parent,
  wxWindowID id,
  COARfile *pFile,
  COARsampleSort *pSort)
  : _CGridEdit(parent,id,false),
    m_pFile(pFile),
    m_pSort(pSort)
{
  wxString s;
  vector<int> vnSample;
  COARsample *pSample;
  size_t n = m_pFile->GetSampleCount();
  size_t i;
  int j = 0;

  m_vpSample.reserve(n);
  vnSample.reserve(n);
    // a little overkill because of ladders and controls
  for(i = 0; i < n; i++)
  {
    j++;
    pSample = m_pSort->GetSample(i);
    if(pSample->IsSampleType() && !pSample->IsDisabled())
    {
      m_vpSample.push_back(pSample);
      vnSample.push_back(j);
    }
  }

  n = m_vpSample.size();
  if(!n)
  {
    CreateGrid(1,(int)COLUMN_COUNT);
    SetCellValue(0,0,"There are no samples");
    SetReadOnly(0,0,true);
    SetMinSize(wxSize(21,21));
    AutoSize();
  }
  else
  {
    int ii;
    CreateGrid((int)n, (int)COLUMN_COUNT);
    SetRowLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTRE);
    for(i = 0; i < n; i++)
    {
      ii = (int)i;
      pSample = m_vpSample.at(i);
      s = CGridAnalysis::FormatRowLabel(
        vnSample.at(i), (int)n, pSample->GetName());
      SetRowLabelValue((int)i,s);
      SetBoolCell(ii,EXCLUDE);
      SetBoolCell(ii,PARTIAL);
      SetCellEditor(ii,SPECIMEN_TYPE,_CreateTypeEditor());
      SetReadOnly(ii,EMPTY_LOCI,true);
    }
    for(i = 0; i < (size_t) COLUMN_COUNT; i++)
    {
      s = COL_LABELS[i];
      SetColLabelValue((int)i,s);
    }
    SetColLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTRE);
  }
}

bool CGridCMF::TransferDataToGrid()
{
  size_t n = m_vpSample.size();
  bool bRtn = (n > 0);
  if(bRtn)
  {

    // loop through samples

    const CLabSettings &lab(m_pFile->GetLabSettings());
    wxString sName;
    wxString sComment;
    wxString sCat;
    COARsample *pSample;
    const CLabNameStrings *pNameStr;
    size_t i;

    for(i = 0; i < n; i++)
    {
      pSample = m_vpSample.at(i);

      // sample name and category

      sName = pSample->GetName();
      sComment = pSample->GetSampleComment();
      pNameStr = lab.GetLabStrings();
      if(pNameStr == NULL)
      {
        sCat = DEFAULT;
      }
      else
      {
        sCat = pNameStr->GetCategory(sName);
        if(sCat.IsEmpty())
        {
          sCat = DEFAULT;
        }
      }


      // now set cell values

      SetBoolValue((int)i,EXCLUDE,false);
      SetCellValue((int)i,SPECIMEN_TYPE,sCat);
      SetCellValue((int)i,SAMPLE,sName);
      SetCellValue((int)i,COMMENT,sComment);
      SetTextLength((int)i,SAMPLE,nwxXmlCMFSpecimen::LEN_SPECIMEN_ID);
      SetTextLength((int)i,SOURCE_ID,nwxXmlCMFSpecimen::LEN_SOURCE_ID);
      SetTextLength((int)i,CASE_ID,nwxXmlCMFSpecimen::LEN_CASE_ID);
      SetTextLength((int)i,COMMENT,nwxXmlCMFSpecimen::LEN_SPECIMEN_COMMENT);
    }
    UpdateEmptyLoci();
    nwxGrid::UpdateLabelSizes(this);
    AutoSizeColumn(EXCLUDE);
    AutoSizeColumn(SAMPLE);
    AutoSizeColumn(PARTIAL);

    // compute size of SPECIMENT_TYPE columns
    int nWidth = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
    const char *ps = CLabSpecimenCategory::LongestType();
    wxClientDC dc(this);
    dc.SetFont(GetDefaultCellFont());
    wxSize sz = dc.GetTextExtent(ps);
    if(!nWidth)
    {
      nWidth = 20;
    }
    nWidth += sz.GetWidth();
    nWidth += 6;
    SetColSize(SPECIMEN_TYPE,nWidth);
    sz = dc.GetTextExtent(
      "This is sample text to determine the width of the comment column.");
    nWidth = sz.GetWidth();
    nWidth += 2;
    SetColSize(COMMENT,nWidth);
  }
  return bRtn;
}

void CGridCMF::UpdateEmptyLoci()
{
  size_t nRows = m_vpSample.size();
  if(nRows)
  {
    wxFont fnBold = GetDefaultCellFont();
    wxString sEmptyLoci;
    wxString COMMA(", ");
    size_t i;
    size_t j;
    size_t nLocusCount;
    bool bPartial;
    bool bEmpty;
    COARlocus *pLocus;
    COARsample *pSample;
    fnBold.SetWeight(wxFONTWEIGHT_BOLD);

    if(m_vsLocus.empty())
    {
        // setup locus names

      size_t nChannelCount = m_pFile->GetChannelCount();
      size_t nLocusCount;
      const COARchannel *pChannel;

      m_vsLocus.reserve(24);
      for(i = 0; i < nChannelCount; i++)
      {
        pChannel = m_pFile->GetChannel(i);
        nLocusCount = pChannel->GetLocusCount();
        for(j = 0; j < nLocusCount; j++)
        {
          m_vsLocus.push_back(pChannel->GetLocusName(j));
        }
      }
    }

    nLocusCount = m_vsLocus.size();
    for(i = 0; i < nRows; i++)
    {
      // now look for empty loci
      bPartial = false;
      bEmpty = true;
      sEmptyLoci.Empty();
      pSample = m_vpSample.at(i);
      for(j = 0; j < nLocusCount; j++)
      {
        pLocus = pSample->FindLocus(m_vsLocus.at(j));
        if( (pLocus == NULL) ||
            (!pLocus->EnabledAlleleCount()) )
        {
          // this is an empty locus

          if(!sEmptyLoci.IsEmpty())
          {
            sEmptyLoci.Append(COMMA);
          }
          sEmptyLoci.Append(pLocus->GetName());
          bPartial = true;
        }
        else
        {
          bEmpty = false;
        }
      }
      if(bEmpty)
      {
        SetBoolValue((int)i,EXCLUDE,bEmpty);
        sEmptyLoci = "ALL LOCI";
        SetCellFont((int)i,EMPTY_LOCI,fnBold);
      }
      else
      {
        //
        // may need to be restored if user pressed
        // 'finish later' and then edited alleles
        //
        SetCellFont((int)i,EMPTY_LOCI,GetDefaultCellFont());
      }
      SetCellValue((int)i,EMPTY_LOCI,sEmptyLoci);
      if(bPartial)
      {
        SetBoolValue((int)i,PARTIAL,bPartial);
      }
      DisableEditRow((int) i,bEmpty);
      SetReadOnly((int)i,EMPTY_LOCI,true);
    }
    AutoSizeColumn(EMPTY_LOCI,false);
  }
}

bool CGridCMF::TransferDataFromGrid(
  const wxString &sDefaultSpecimen,
  COARsample2CMFSpecimen *pMap,
  vector<wxString> *pMsgs)
{

  wxString s;
  wxString sMsg;
  size_t nRows = m_vpSample.size();
  size_t i;
  COARsample *pSample;
  nwxXmlCMFSpecimen *pCMFSpecimen;
  bool bRtn = true;
  bool bExclude;

  for(i = 0; i < nRows; i++)
  {
    bExclude = GetBoolValue((int)i,EXCLUDE);
    pSample = m_vpSample.at(i);
    pCMFSpecimen = pMap->Find(pSample);
    if(pCMFSpecimen == NULL)
    {
      if(!bExclude)
      {
        bRtn = false;
        s = GetCellValue((int)i,SAMPLE);
        if(s.IsEmpty())
        {
          s = pSample->GetName();
        }
        sMsg = "Internal error:\nCannot find specimen, ";
        sMsg.Append(s);
        sMsg.Append(", in CMF file");
        pMsgs->push_back(sMsg);
      }
    }
    else if(bExclude)
    {
      pCMFSpecimen->SetExclude(true);
    }
    else
    {
      pCMFSpecimen->SetExclude(false);
      s = GetCellValue((int)i,SAMPLE);
      if( (!s.IsEmpty()) && (s != pSample->GetName()) )
      {
        pCMFSpecimen->SetSpecimenID(s);
      }
      s = GetCellValue((int)i,SPECIMEN_TYPE);
      if(s.IsEmpty() || !s.Cmp(DEFAULT))
      {
        s = sDefaultSpecimen;
      }
      pCMFSpecimen->SetSpecimenCategory(s);
      s = GetCellValue((int)i,SOURCE_ID);
      if(!s.IsEmpty())
      {
        pCMFSpecimen->SetSourceID(s);
      }
      s = GetCellValue((int)i,CASE_ID);
      if(!s.IsEmpty())
      {
        pCMFSpecimen->SetCaseID(s);
      }
      s = GetCellValue((int)i,COMMENT);
      if(!s.IsEmpty())
      {
        pCMFSpecimen->SetSpecimenComment(s);
      }
      pCMFSpecimen->SetPartial(GetBoolValue((int)i,PARTIAL));
    }
  }
  return bRtn;
}
