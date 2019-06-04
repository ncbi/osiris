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
*  FileName: COARheading.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_HEADING_H__
#define __COAR_HEADING_H__

#include "CParmOsiris.h"
#include "CStandardSettings.h"
#include "CLabSettings.h"
#include "COARmisc.h"


//************************************************* COARchannel

class COARchannel : public nwxXmlPersist
{
public:
  COARchannel()
  {
    RegisterAll(true);
  }
  COARchannel(const COARchannel &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARchannel() {;}
  virtual void RegisterAll(bool = false);
  const wxString &GetLocusName(size_t nIndex) const
  {
    return m_vsLocusName.at(nIndex);
  }
  size_t GetLocusCount() const
  {
    return m_vsLocusName.size();
  }
  /*  failed on 8/12/2016
  const wxString &GetChannelName() const
  {
    return m_sChannelName;
  }
  */
  int GetChannelNr() const
  {
    return m_nChannelNr;
  }
private:
  vector<wxString> m_vsLocusName;
  wxString m_sChannelName;
  int m_nChannelNr;
};


//************************************************* COARheading

class COARheading : public nwxXmlPersist
{
public:
  COARheading() :
    m_tCreationTime((time_t)0),
    m_tNewFileCreationTime((time_t)0),
    m_vsCommandLine("argv") ,
    m_setSaveHistory("Time"),
    m_bTampered(false)
  {
    RegisterAll(true);
  }
  COARheading(const COARheading &x) : m_setSaveHistory("Time")
  {
    RegisterAll(true);
    (*this) = x;
  }
  COARheading &operator =(const COARheading &x)
  {
    COARcopy(m_sFileName);
    COARcopy(m_sNewFileName);
    COARcopy(m_tCreationTime);
    COARcopy(m_tNewFileCreationTime);
    COARcopy(m_vsCommandLine);
    COARcopy(m_sStatusText);
    COARcopy(m_LabSettings);
    COARcopy(m_StandardSettings);
    COARcopy(m_RuntimeSettings);
    COARcopy(m_sMarkerSet);
    COARcopy(m_nDeletedSamples);
    COARcopy(m_nStatus);
    COARcopy(m_nILSchannelNr);
    COARcopyVP(COARchannel, m_vpChannel);
    COARcopy(m_setSaveHistory);
    COARcopy(m_bTampered);
    return *this;
  }
  virtual ~COARheading()
  {
    vectorptr<COARchannel>::cleanup(&m_vpChannel);
  }
  virtual void RegisterAll(bool = false);
  void AddDeletedSamples(size_t n)
  {
    m_nDeletedSamples += n;
  }
  unsigned int GetDeletedSampleCount() const
  {
    return m_nDeletedSamples;
  }
  void CorrectFileTime(const wxDateTime &dt);
    //
    // bug fix in file corrected in this function
    // see comment block in .cpp file inside this function
    //
  size_t GetChannelCount() const
  {
    return m_vpChannel.size();
  }
  const COARchannel *GetChannel(size_t nIndex) const
  {
    // nIndex, index into the array, not channel #
    return m_vpChannel.at(nIndex);
  }
  const COARchannel *GetChannelByNr(unsigned int n) const;
  const vector<COARchannel *> *GetAllChannels() const
  {
    return &m_vpChannel;
  }
  const CParmOsiris &GetParameters() const
  {
    return m_RuntimeSettings;
  }
  int ILSchannelNr()
  {
    return m_nILSchannelNr;
  }
  int GetStatus()
  {
    return m_nStatus; // exit status of analysis program
  }
  const wxString &GetFileName() const
  {
    return m_sFileName;
  }
  const wxString &GetNewFileName() const
  {
    return m_sNewFileName;
  }
  void SetNewFileName(const wxString &s)
  {
    m_sNewFileName = s;
  }
  void SetNewFileCreationTime()
  {
    m_tNewFileCreationTime.SetToCurrent();
    m_setSaveHistory.GetSetPtr()->insert(m_tNewFileCreationTime);
  }
  const set<wxDateTime> *GetHistory() const
  {
    return m_setSaveHistory.GetSetPtr();
  }
  const wxDateTime &GetCreationTime() const
  {
    return m_tCreationTime;
  }
  const wxString &GetKitName() const
  {
    return m_RuntimeSettings.GetKitName();
  }
  const wxDateTime &GetFileTime() const
  {
    const set<wxDateTime> *pset = m_setSaveHistory.GetSetPtr();
    if(pset->size())
    {
      set<wxDateTime>::const_reverse_iterator itr =
        pset->rbegin();
      return *itr;
    }
    return m_tCreationTime;
  }
  bool IsTampered() const
  {
    return m_bTampered;
  }
  void SetTampered(bool b = true) const
  {
    m_bTampered = b;
  }
  const CLabSettings &GetLabSettings() const
  {
    return m_LabSettings;
  }
  CLabSettings &GetLabSettings()
  {
    return m_LabSettings;
  }
  const wxString &GetMarkerSet() const
  {
    return m_sMarkerSet;
  }
  const std::vector<wxString> &GetCommandLine() const
  {
    return m_vsCommandLine.GetVector();
  }
private:
  wxString m_sFileName;
  wxString m_sNewFileName;
  wxDateTime m_tCreationTime;
  wxDateTime m_tNewFileCreationTime;
  nwxXmlPersistVectorWxString m_vsCommandLine;
  wxString m_sStatusText;
  CLabSettings m_LabSettings;
  CStandardSettings m_StandardSettings;
  CParmOsirisLite m_RuntimeSettings;
  wxString m_sMarkerSet;
  vector<COARchannel *> m_vpChannel;
  nwxXmlPersistSetWxDateTime m_setSaveHistory;
  unsigned int m_nDeletedSamples;
  int m_nStatus;
  int m_nILSchannelNr;
  mutable bool m_bTampered;
private:
  TnwxXmlIOPersistVector<COARchannel> m_ioChannel;
};

#endif
