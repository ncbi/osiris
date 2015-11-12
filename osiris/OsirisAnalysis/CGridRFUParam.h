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
*  FileName: CGridRFUParam.h
*  Author:   Douglas Hoffman
*
*  wxGrid for viewing RFU limits for sample, ILS, channel,
*    in the Parameters window (CDialogParameters.cpp/h)
*
*  This is inherited by CGridRFURun, used for running a new analysis
*  and CGridRFUParam, used for showing runtime parameters
*  The main difference is the former loads data from 
*
*/

#ifndef __C_GRID_RFU_PARAM_H__
#define __C_GRID_RFU_PARAM_H__

#include "CGridRFURunBase.h"

class CParmOsiris;

class CGridRFUParam : public CGridRFURunBase
{
public:
  CGridRFUParam(wxWindow *window, wxWindowID id = wxID_ANY) :
      CGridRFURunBase(window,id),
      m_pParms(NULL)
  {}
  void SetupParms(const CParmOsiris *pParm, const wxString &sKitName = wxEmptyString)
  {
    m_pParms = pParm;
    if(!sKitName.IsEmpty()) SetupKit(sKitName);
    SetAllReadOnly(true);
  }
  virtual bool TransferDataToWindow();
private:
  const CParmOsiris *m_pParms;
};
#endif