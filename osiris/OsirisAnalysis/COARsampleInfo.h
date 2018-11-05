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
*  FileName: COARsampleInfo.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_SAMPLE_INFO_H__
#define __COAR_SAMPLE_INFO_H__

#include "nwx/vectorptr.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "COARmisc.h"

class COARsampleInfoChannel : public nwxXmlPersist
{
public:
  COARsampleInfoChannel() :
    m_dNoise(0.0),
    m_dChannelLocusTotalAreaRatioMaxToMin(0.0),
    m_dChannelYLinkedLocusTotalAreaMaxToMin(0.0),
    m_nNumber(0)
  {
    RegisterAll(true);
  }
  COARsampleInfoChannel(const COARsampleInfoChannel &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARsampleInfoChannel() {}
  COARsampleInfoChannel &operator =(const COARsampleInfoChannel &x)
  {
    COARcopy(m_dNoise);
    COARcopy(m_dChannelLocusTotalAreaRatioMaxToMin);
    COARcopy(m_dChannelYLinkedLocusTotalAreaMaxToMin);
    COARcopy(m_nNumber);
    return *this;
  }
  virtual void RegisterAll(bool = false)
  {
    RegisterInt(wxS("Number"), &m_nNumber);
    RegisterDouble(wxS("Noise"), &m_dNoise);
    RegisterDouble(wxS("ChannelLocusTotalAreaRatioMaxToMin"),
      &m_dChannelLocusTotalAreaRatioMaxToMin);
    RegisterDouble(wxS("ChannelYLinkedLocusTotalAreaMaxToMin"),
      &m_dChannelYLinkedLocusTotalAreaMaxToMin);
  }
private:
  double m_dNoise;
  double m_dChannelLocusTotalAreaRatioMaxToMin;
  double m_dChannelYLinkedLocusTotalAreaMaxToMin;
  int m_nNumber;
};

class COARsampleInfo : public nwxXmlPersist
{
public:
  COARsampleInfo() :
    m_dMaxLinearPullupCoefficient(0.0),
    m_dMaxNonlinearPullupCoefficient(0.0),
    m_dMaxBPErrorSampleToLadder(0.0),
    m_dWidthOfLastILSPeak(0.0),
    m_dSampleLocusTotalAreaRatioMaxToMin(0.0),
    m_dSampleYLinkedLocusTotalAreaRatioMaxToMin(0.0),
    m_dStartingTemperature(0.0),
    m_dMaxMinusMinTemperature(0.0),
    m_dStartingVoltage(0.0),
    m_dMaxMinusMinVoltage(0.0),
    m_dStartingCurrent(0.0),
    m_dMaxMinusMinCurrent(0.0),
    m_dStartingPower(0.0),
    m_dMaxMinusMinPower(0.0)
  {
    RegisterAll(true);
    _cleanup();
  }
  COARsampleInfo(const COARsampleInfo &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARsampleInfo()
  {
    _cleanup();
  }
  COARsampleInfo &operator =(const COARsampleInfo &x)
  {
    COARcopy(m_dMaxLinearPullupCoefficient);
    COARcopy(m_dMaxNonlinearPullupCoefficient);
    COARcopy(m_dMaxBPErrorSampleToLadder);
    COARcopy(m_dWidthOfLastILSPeak);
    COARcopy(m_dSampleLocusTotalAreaRatioMaxToMin);
    COARcopy(m_dSampleYLinkedLocusTotalAreaRatioMaxToMin);
    COARcopy(m_dStartingTemperature);
    COARcopy(m_dMaxMinusMinTemperature);
    COARcopy(m_dStartingVoltage);
    COARcopy(m_dMaxMinusMinVoltage);
    COARcopy(m_dStartingCurrent);
    COARcopy(m_dMaxMinusMinCurrent);
    COARcopy(m_dStartingPower);
    COARcopy(m_dMaxMinusMinPower);
    COARcopy(m_sRunDate);
    COARcopy(m_sRunTime);
    COARcopy(m_sCapillaryNumber);
    COARcopy(m_sInjectionSeconds);
    COARcopy(m_sInjectionVoltage);

    COARcopyVP(COARsampleInfoChannel, m_vpChannel);
    return *this;
  }
  virtual void Init()
  {
    nwxXmlPersist::Init();
    _cleanup();
  }
  virtual bool Skip(void *pObj)
  {
    COARsampleInfo &x(*((COARsampleInfo *)pObj));
    return x.m_vpChannel.empty();
  }
  virtual void RegisterAll(bool = false)
  {
    RegisterDouble(wxS("MaxLinearPullup"),
      &m_dMaxLinearPullupCoefficient);
    RegisterDouble(wxS("MaxNonlinearPullup"),
      &m_dMaxNonlinearPullupCoefficient);
    RegisterDouble(wxS("MaxBPErrorSampleToLadder"),
      &m_dMaxBPErrorSampleToLadder);
    RegisterDouble(wxS("WidthOfLastILSPeak"),
      &m_dWidthOfLastILSPeak);
    RegisterDouble(wxS("SampleLocusTotalAreaRatioMaxToMin"),
      &m_dSampleLocusTotalAreaRatioMaxToMin);
    RegisterDouble(wxS("SampleYLinkedLocusTotalAreaRatioMaxToMin"),
      &m_dSampleYLinkedLocusTotalAreaRatioMaxToMin);
    RegisterDouble(wxS("StartingTemperature"),
      &m_dStartingTemperature);
    RegisterDouble(wxS("MaxMinusMinTemperature"),
      &m_dMaxMinusMinTemperature);
    RegisterDouble(wxS("StartingVoltage"),
      &m_dStartingVoltage);
    RegisterDouble(wxS("MaxMinusMinVoltage"),
      &m_dMaxMinusMinVoltage);
    RegisterDouble(wxS("StartingCurrent"),
      &m_dStartingCurrent);
    RegisterDouble(wxS("MaxMinusMinCurrent"),
      &m_dMaxMinusMinCurrent);
    RegisterDouble(wxS("StartingPower"),
      &m_dStartingPower);
    RegisterDouble(wxS("MaxMinusMinPower"),
      &m_dMaxMinusMinPower);
    RegisterWxStringNotEmpty(wxS("RunDate"), &m_sRunDate);
    RegisterWxStringNotEmpty(wxS("RunTime"), &m_sRunTime);
    RegisterWxStringNotEmpty(wxS("CapillaryNumber"), &m_sCapillaryNumber);
    RegisterWxStringNotEmpty(wxS("InjectionSeconds"), &m_sInjectionSeconds);
    RegisterWxStringNotEmpty(wxS("InjectionVoltage"), &m_sInjectionVoltage);
    Register(wxS("Channel"), &m_ioChannel, (void *) &m_vpChannel);
  }
private:
  void _cleanup()
  {
    vectorptr<COARsampleInfoChannel>::cleanup(&m_vpChannel);
  }
  TnwxXmlIOPersistVector<COARsampleInfoChannel> m_ioChannel;
  std::vector<COARsampleInfoChannel *> m_vpChannel;

  // OS-993, add the following 5 variables
  wxString m_sRunDate;
  wxString m_sRunTime;
  wxString m_sCapillaryNumber;
  wxString m_sInjectionSeconds;
  wxString m_sInjectionVoltage;


  double m_dMaxLinearPullupCoefficient;
  double m_dMaxNonlinearPullupCoefficient;
  double m_dMaxBPErrorSampleToLadder;
  double m_dWidthOfLastILSPeak;
  double m_dSampleLocusTotalAreaRatioMaxToMin;
  double m_dSampleYLinkedLocusTotalAreaRatioMaxToMin;
  double m_dStartingTemperature;
  double m_dMaxMinusMinTemperature;
  double m_dStartingVoltage;
  double m_dMaxMinusMinVoltage;
  double m_dStartingCurrent;
  double m_dMaxMinusMinCurrent;
  double m_dStartingPower;
  double m_dMaxMinusMinPower;
};


#endif