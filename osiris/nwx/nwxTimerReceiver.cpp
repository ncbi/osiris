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
*  FileName: nwxTimerReceiver.cpp
*  Author:   Douglas Hoffman
*
*/

#include "nwx/nwxTimerReceiver.h"


set<nwxTimerReceiver *> nwxTimerReceiver::g_Receivers;
nwxTimerReceiver::TASK_SET nwxTimerReceiver::g_Tasks;

nwxTimerReceiver::nwxTimerReceiver()
{
  RegisterTimerReceiver(this);
}

nwxTimerReceiver::~nwxTimerReceiver()
{
  UnRegisterTimerReceiver(this);
}

void nwxTimerReceiver::UnRegisterTimerReceiver(nwxTimerReceiver *p)
{
  set<nwxTimerReceiver *>::iterator itrR =
    g_Receivers.find(p);
  if (itrR != g_Receivers.end())
  {
    g_Receivers.erase(itrR);
  }
  TASK_RANGE task_range = g_Tasks.equal_range(p);
  bool bFound = false;
  for (TASK_ITERATOR itr = task_range.first;
    itr != task_range.second;
    ++itr)
  {
    bFound = true;
    delete itr->second;
  }
  if (bFound)
  {
    g_Tasks.erase(task_range.first, task_range.second);
  }
}

bool nwxTimerReceiver::AddTask(nwxTimerTask *pTask)
{
  bool bRtn = (g_Receivers.find(this) != g_Receivers.end());
  if(bRtn)
  {
    g_Tasks.insert(TASK_SET::value_type(this, pTask));
  }
  else
  {
    delete pTask;
  }
  return bRtn;
}


void nwxTimerReceiver::DispatchTasks(wxTimerEvent &e)
{
  TASK_RANGE task_range = g_Tasks.equal_range(this);
  std::vector<TASK_ITERATOR> kill_list;
  std::vector<TASK_ITERATOR>::reverse_iterator itrKill;
  TASK_ITERATOR itr;
  nwxTimerTask *pTask;
  for (itr = task_range.first;
    itr != task_range.second;
    ++itr)
  {
    pTask = itr->second;
    if (pTask->IsReady(e) ? pTask->Run(e) : false)
    {
      // if Run() returns true, this task is done
      //  add to kill list and delete in the next loop
      kill_list.push_back(itr);
    }
  }
  for (itrKill = kill_list.rbegin();
      itrKill != kill_list.rend();
      ++itrKill)
  {
    itr = *itrKill;
    pTask = itr->second;
    delete pTask;
    g_Tasks.erase(itr);
  }
}