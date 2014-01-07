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
*  FileName: CGridCMF.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_GRID_CMF_H__
#define __C_GRID_CMF_H__

#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "CGridEdit.h"

class CDialogCMF;
class nwxXmlCMF;
class COARfile;
class COARsampleSort;
class COARsample2CMFSpecimen;

class CGridCMF : public _CGridEdit
{
public:
  CGridCMF(
    CDialogCMF *parent, 
    wxWindowID id,
    COARfile *pFile,
    COARsampleSort *pSort);
  virtual ~CGridCMF();

  bool TransferDataFromGrid(
    const wxString &sDefaultSpecimen,
    COARsample2CMFSpecimen *pMap,
    vector<wxString> *pErrorMsgs);

  bool TransferDataToGrid();
  void UpdateEmptyLoci();
  static const wxString DEFAULT;
private:
  vector<wxString> m_vsLocus;
  vector<COARsample *> m_vpSample;
    // sample index in COARfile::GetSample for each row of table

  CDialogCMF *m_pParent;
  nwxXmlCMF *m_pCMF;
  COARfile *m_pFile;
  COARsampleSort *m_pSort;
  static wxGridCellChoiceEditor *_CreateTypeEditor();
  static wxArrayString m_asChoices;
  static const char * const COL_LABELS[];
};

#endif
