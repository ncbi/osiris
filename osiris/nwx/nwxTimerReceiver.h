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
*  FileName: nwxTimerReceiver.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_TIMER_RECEIVER_H__
#define __NWX_TIMER_RECEIVER_H__

#include "nwx/stdb.h"
#include <set>
#include "nwx/nsstd.h"
#include "nwx/stde.h"
#include <wx/timer.h>

class nwxTimerReceiver
{
public:
  nwxTimerReceiver();
  virtual ~nwxTimerReceiver();
  virtual void OnTimer(wxTimerEvent &) = 0;

  static void DispatchTimer(wxTimerEvent &e)
  {
    set<nwxTimerReceiver *>::iterator itr;
    for(itr = g_Receivers.begin();
      itr != g_Receivers.end();
      ++itr)
    {
      (*itr)->OnTimer(e);
    }
  }

private:
  // static functions

  static void RegisterTimerReceiver(nwxTimerReceiver *p)
  {
    g_Receivers.insert(p);
  }
  static void UnRegisterTimerReceiver(nwxTimerReceiver *p)
  {
    set<nwxTimerReceiver *>::iterator itr = 
      g_Receivers.find(p);
    if(itr != g_Receivers.end())
    {
      g_Receivers.erase(itr);
    }
  }
  static set<nwxTimerReceiver *> g_Receivers;

};

class nwxTimerInterval
{
  //
  //  if the "OnTimer()" function in nwxTimerReceiver
  //  is to use a larger time interval than that of the 
  //  wxTimer instance, this class will keep track if the 
  //  interval.
  //
  //  for example, if the timer interval is 250 ms
  //  and it is desired to do an operation every 1000 ms
  //  then this class should be initialized with an interval
  //  of 1000 and the OnTimer() function should be as follows
  //
  //  void OnTimer(wxTimeEvent &e)
  //  {
  //     if(TimerInterval.CheckTimer(e))
  //     {
  //       DoSomething();
  //     }
  //  }
  //
  //
public:
  nwxTimerInterval(int nIntervalMS = 0, int initWait = -1) :
    m_nIntervalMS(nIntervalMS),
    m_nWaiting(initWait)
  {
    if(m_nWaiting < 0)
    {
      m_nWaiting = m_nIntervalMS;
    }
  }
  virtual ~nwxTimerInterval() {}
  bool CheckTimer(wxTimerEvent &e)
  {
    int n = e.GetInterval();
    bool bRtn = (m_nWaiting <= n);
    if(bRtn)
    {
      m_nWaiting = m_nIntervalMS;
    }
    else
    {
      m_nWaiting -= n;
    }
    return bRtn;
  }
  void SetWaitTime(int nms = 0)
  {
    m_nWaiting = nms;
  }
  void SetInterval(int nms = 0)
  {
    m_nIntervalMS = nms;
  }
  int GetInterval()
  {
    return m_nIntervalMS;
  }
  int GetWaitTime()
  {
    return m_nWaiting;
  }
private:
  int m_nIntervalMS;
  int m_nWaiting;
};

#endif
