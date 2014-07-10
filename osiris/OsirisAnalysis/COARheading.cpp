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
*  FileName: COARheading.cpp
*  Author:   Douglas Hoffman
*
*/
#include "COARfile.h"

void COARchannel::RegisterAll(bool)
{
  RegisterWxString("ChannelName",&m_sChannelName);
  RegisterInt("ChannelNr",&m_nChannelNr);
  Register(
    COARfile::g_sLocusName,
    &COARfile::g_ioLocusName,
    (void *)&m_vsLocusName);
}

void COARheading::RegisterAll(bool)
{
  RegisterWxString("FileName",&m_sFileName);
  RegisterWxString("NewFileName",&m_sNewFileName);
  RegisterWxDateTime("CreationTime",&m_tCreationTime);
  RegisterWxDateTime("NewFileCreationTime",&m_tNewFileCreationTime);
  Register("SaveHistory",&m_setSaveHistory);
  Register("CommandLine",&m_vsCommandLine);
  RegisterBoolSkipFalse("StatusOK",&m_bTampered);
  RegisterInt("Status",&m_nStatus);
  RegisterWxString("StatusText",&m_sStatusText);
  Register("LabSettings",&m_LabSettings);
  Register("StandardSettings",&m_StandardSettings);
  Register("RuntimeSettings",&m_RuntimeSettings);
  RegisterWxString("MarkerSet",&m_sMarkerSet);
  RegisterInt("ILSchannelNr",&m_nILSchannelNr);
  Register("Channel",&m_ioChannel,(void *) &m_vpChannel);
  m_ioChannel.Manage(&m_vpChannel);
}

void COARheading::CorrectFileTime(const wxDateTime &dt)
{
  // there was a bug where the file was saved with 
  // m_tCreationTime where m_tNewFileCreationTime should be
  // Due to the redundancy of m_tNewFileCreationTime
  // also being in the history list, this is corrected
  // by using this function from COARfile::LoadFile
  //
  if(dt > m_tNewFileCreationTime)
  {
    // use the actual file date/time
    // obtained from the file system

    m_tNewFileCreationTime = dt;
  }
  set<wxDateTime> *pSet = m_setSaveHistory.GetSetPtr();
  if(pSet->size())
  {
    //
    //  some older files have problems with
    //  inconsistent dates/times or the
    //
    set<wxDateTime>::reverse_iterator itrr = pSet->rbegin();
    set<wxDateTime>::iterator itr = pSet->begin();
    if((*itrr) > m_tNewFileCreationTime)
    {
      m_tNewFileCreationTime = *itrr;
    }
    if((*itr) < m_tCreationTime)
    {
      m_tCreationTime = *itr;
    }
  }
}

const COARchannel *COARheading::GetChannelByNr(unsigned int n) const
{
  const COARchannel *pRtn(NULL);
  size_t nSize = m_vpChannel.size();
  size_t i;
  int nChannel = (int) n;
  if(n > 0 && n <= nSize)
  {
    // this should take care of it if the channels are in order
    //  channel 1 should have offset 0, etc
    pRtn = m_vpChannel.at(n - 1);
    if(pRtn->GetChannelNr() == nChannel)
    {
      // we found it on the first try
      nSize = 0; // prevent loop below
    }
    else
    {
      pRtn = NULL; // didn't find it here
    }
  }
  for(i = 0; i < nSize; i++)
  {
    pRtn = m_vpChannel.at(i);
    if(pRtn->GetChannelNr() != nChannel)
    {
      pRtn = NULL; // not here
    }
    else
    {
      // found it, yeah
      nSize = 0; // loop exit
    }
  }
  return pRtn;
}
