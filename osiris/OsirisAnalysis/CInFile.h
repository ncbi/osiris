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

*  FileName: CInFile.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_IN_FILE_H__
#define __C_IN_FILE_H__

#include <wx/wx.h>
#include <wx/string.h>
#include "nwx/stdb.h"
#include <vector>
#include <string>
#include <map>
#include "nwx/stde.h"
#include "CParmOsiris.h"
#include "nwx/nwxString.h"
#include "nwx/nsstd.h"

class CInFile;

class CInFileAllele
{
public:
  CInFileAllele()
  {
    dAllele = 0.0;
    bOffLadder = false;
    nError = 0;
  }
  CInFileAllele(const CInFileAllele &x)
  {
    (*this) = x;
  }
  CInFileAllele(wxString &s, CInFile *pIn)
  {
    Setup(s,pIn);
  }
  bool Setup(wxString &s, CInFile *pIn);
  operator double() { return dAllele;}
  CInFileAllele &operator = (const CInFileAllele &x)
  {
    dAllele = x.dAllele;
    bOffLadder = x.bOffLadder;
    nError = x.nError;
    return (*this);
  }
  double dAllele;
  int nError;
  bool bOffLadder;
};

class CInFileAlleleList : public vector<CInFileAllele>
{
public:
  CInFileAlleleList(wxString &s,CInFile *pInFile)
  {
    Setup(s,pInFile);
  }
  CInFileAlleleList() 
  {
    m_bError = false;
  }
  void Setup(wxString &s, CInFile *pInFile);
  virtual ~CInFileAlleleList();
  wxString GetDisplay(bool bAmel = false) const
  {
    wxString sRtn;
    if(bAmel)
    {
      sRtn = AmelString(m_sDisplay);
    }
    else
    {
      sRtn = m_sDisplay;
    }
    return sRtn;
  }
  bool HasError()
  {
    return m_bError;
  }
  static wxString AmelString(const wxString &sDisplay)
  {
    wxString sRtn(sDisplay);
    sRtn.Replace(_T("1"),_T("X"));
    sRtn.Replace(_T("2"),_T("Y"));
    return sRtn;
  }
private:
  void Clear()
  {
    clear();
    m_sDisplay.clear();
    m_bError = false;
  }
  wxString m_sDisplay;
  bool m_bError;
};

class CInFileSample
{
public:
  CInFileSample()
  {
    m_nILSerror = 0;
    m_nSampleError = 0;
    m_nChannelError = 0;
  }
  CInFileSample(vector<wxString> &sLine,CInFile *pInFile)
  {
    m_nILSerror = 0;
    m_nSampleError = 0;
    m_nChannelError = 0;
    Load(sLine,pInFile);
  }
  virtual ~CInFileSample();
  void Load(vector<wxString> &sLine, CInFile *pInFile);
  void SetFullErrorReport(const wxString &s)
  {
    m_sError = s;
  }
  void ClearFullErrorReport()
  {
    m_sError.Empty();
  }
  void AppendFullErrorReport(const wxString &s)
  {
    m_sError.Append(s);
  }
  const wxString &GetFullErrorReport()
  {
    return m_sError;
  }
  const wxString &GetName()
  {
    return m_sName;
  }
  const wxString &GetILS()
  {
    return m_sILS;
  }
  const wxString &GetChannel()
  {
    return m_sChannel;
  }
  CInFileAlleleList *GetAlleles(int n)
  {
    CInFileAlleleList *pRtn = 
      (n < (int)m_vpAlleles.size())
      ? m_vpAlleles.at(n)
      : NULL;
    return pRtn;
  }
  int GetILSerror()
  {
    return m_nILSerror;
  }
  int GetSampleError()
  {
    return m_nSampleError;
  }
  int GetChannelError()
  {
    return m_nChannelError;
  }
  int GetAlleleCount()
  {
    return (int)m_vpAlleles.size();
  }
  bool HasError()
  {
    return !!(m_nILSerror | m_nSampleError | m_nILSerror);
  }
  const wxString &GetFileName()
  {
    return m_sFullName;
  }
  const wxString &GetPosControl()
  {
    return m_sPosControl;
  }
  void UpdateErrorMap(map<int,size_t> *pmap, size_t ndx);
  wxString m_sPosControl;
  wxString m_sName;
  wxString m_sILS;
  wxString m_sChannel;
  wxString m_sFullName;
  wxString m_sError;
private:
  int m_nILSerror;
  int m_nSampleError;
  int m_nChannelError;
  vector<CInFileAlleleList *> m_vpAlleles;
  void Clear();
};

class CInFile
{
public:
  CInFile()
  {
    m_bPosCtrlColumn = false;
  }
  CInFile(const char *psFileName)
  {
    Load(psFileName);
  }
  virtual ~CInFile();
  bool Load(const char *psFileName);
  void Clear();
  bool IsEmpty()
  {
    return m_asColumns.empty() || m_apSamples.empty();
  }
  static bool _isFsa(wxString &s);
//  static bool _isLadder(vector<wxString> &vs);
  static int _extractErrorStr(wxString &s, wxString *psStr, int *pnErr);
  static size_t _split(const char *ps,vector<wxString> *pvs, char cSep = '\t')
  {
    return nwxString::Split(ps,pvs,cSep);
  }
  void AddToError(int nError, vector<wxString> &vs);

  int GetColumnCount()
  {
    return (int) m_asColumns.size();
  }
  int GetRowCount()
  {
    return (int) m_apSamples.size();
  }
  const vector<wxString> &GetColumns()
  {
    return m_asColumns;
  }
  const wxString &GetColumn(int i)
  {
    return m_asColumns.at(i);
  }
  const vector<CInFileSample *> &GetSamples()
  {
    return m_apSamples;
  }
  wxString *GetError(int n, bool bAdd = false);
  CInFileSample *GetSample(int nRow)
  {
    return 
      (nRow >= 0 && nRow < (int)m_apSamples.size())
      ? m_apSamples.at(nRow)
      : NULL;
  }
  const CParmOsiris &GetParameters()
  {
    return m_parms;
  }
  bool PositiveControlColumn()
  {
    return m_bPosCtrlColumn;
  }
  const wxString &GetFileName() const
  {
    return m_sFileName;
  }
private:
  void BuildErrorToSample();
  CInFileSample *GetSampleFromError(int nError);
  static void _chomp(char *ps);
  vector<wxString> m_asColumns;
  vector<CInFileSample *> m_apSamples;
  map<int, wxString*> m_apErrors;
  map<int,size_t> m_mapErrorToSample;
  CParmOsiris m_parms;
  wxString m_sFileName;
  bool m_bPosCtrlColumn;
};

#endif
