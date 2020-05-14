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
#include <map>
#include <vector>
#include "nwx/nsstd.h"
#include "nwx/stde.h"
#include <wx/timer.h>

class nwxTimerTask
{
  // a task associated with a nwxTimerReceiver instance
  // that can be delayed and either run once or frequently
  // subclass must implement Run() which returns true
  // when this task is finished and can be deleted
public:
  nwxTimerTask() { ; }
  virtual ~nwxTimerTask() { ; }
  virtual bool Run(wxTimerEvent &e) = 0; // return true when done
  virtual bool IsReady(wxTimerEvent &) 
  {
    // virtual - return true when ready to call Run()
    return true; 
  }
};

class nwxTimerReceiver
{
public:
  typedef std::multimap<nwxTimerReceiver *, nwxTimerTask *> TASK_SET;
  typedef TASK_SET::iterator TASK_ITERATOR;
  typedef std::pair<TASK_ITERATOR, TASK_ITERATOR> TASK_RANGE;

  nwxTimerReceiver();
  virtual ~nwxTimerReceiver();
  virtual void OnTimer(wxTimerEvent &) = 0;
  bool AddTask(nwxTimerTask *pTask);
  void DispatchTasks(wxTimerEvent &e);
  void StopReceiver()
  {
    UnRegisterTimerReceiver(this);
  }
  static void DispatchTimer(wxTimerEvent &e)
  {
    set<nwxTimerReceiver *>::iterator itr;
    for (itr = g_Receivers.begin();
      itr != g_Receivers.end();
      ++itr)
    {
      (*itr)->OnTimer(e);
      (*itr)->DispatchTasks(e);
    }
  }

private:

  // static functions

  static void RegisterTimerReceiver(nwxTimerReceiver *p)
  {
    g_Receivers.insert(p);
  }
  static void UnRegisterTimerReceiver(nwxTimerReceiver *p);
  static set<nwxTimerReceiver *> g_Receivers;
  static TASK_SET g_Tasks;
};

class nwxTimerInterval
{
  //
  //  if the "OnTimer()" function in nwxTimerReceiver
  //  is to use a larger time interval than 1that of the 
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
  void ResetWaitTime()
  {
    m_nWaiting = m_nIntervalMS;
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

class nwxTimerTaskMs : public nwxTimerTask
{
  // nwxTimerTask where approximate milliseconds can be set
public:
  nwxTimerTaskMs(int nIntervalMs) : nwxTimerTask(), m_interval(nIntervalMs)
  {}
  virtual ~nwxTimerTaskMs() {}
  virtual bool IsReady(wxTimerEvent &e)
  {
    bool r = m_interval.CheckTimer(e);
    return r;
  }
private:
  nwxTimerInterval m_interval;
};

class nwxTimerTaskCount : public nwxTimerTask
{
  // nwxTimerTask that counts timer ticks
public:
  nwxTimerTaskCount(unsigned int nTicks = 2) :
    nwxTimerTask(), 
    m_nCount(nTicks),
    m_nInterval(nTicks)
  {}
  virtual ~nwxTimerTaskCount() {}
  virtual bool IsReady(wxTimerEvent &)
  {
    m_nCount--;
    bool bRtn = false;
    if (!m_nCount)
    {
      m_nCount = m_nInterval;
      bRtn = true;
    }
    return bRtn;
  }
private:
  unsigned int m_nCount;
  unsigned int m_nInterval;
};


#endif
