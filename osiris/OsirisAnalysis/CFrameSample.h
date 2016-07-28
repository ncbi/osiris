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
*  FileName: CFrameSample.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_FRAME_SAMPLE_H__
#define __C_FRAME_SAMPLE_H__

#include "CMDIFrame.h"
#include "CHistoryTime.h"
class CNotebookEditSample;
class COARsample;
class COARfile;
class CHistoryTime;
class CFrameAnalysis;

class CFrameSample : public CMDIFrame
{
public:
  CFrameSample(
    CFrameAnalysis *pCreator,
    mainFrame *parent,
    wxSize sz, 
    COARfile *pFile,
    COARsample *pSample);
  virtual ~CFrameSample();
  virtual int GetType();
  virtual bool TransferDataToWindow();
  virtual void OnTimer(wxTimerEvent &);
  void OnClose(wxCloseEvent &e);
  void SetupTitle();

private:
  CHistoryTime m_Hist;
  CNotebookEditSample *m_pPanel;
  CFrameAnalysis *m_pCreator;
  mainFrame *m_pParent;
  COARfile *m_pOARfile;
  COARsample *m_pSample;

public:
  CNotebookEditSample *m_pNoteBook;
  DECLARE_EVENT_TABLE()
  DECLARE_ABSTRACT_CLASS(CFrameAnalysis)
};

#endif