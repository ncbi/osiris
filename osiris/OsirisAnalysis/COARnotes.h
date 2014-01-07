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
*  FileName: COARnotes.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_NOTES_H__
#define __COAR_NOTES_H__

#include "COARmisc.h"

//************************************************* COARnotes

class COARnotes : public nwxXmlPersist
{
public:
  static const wxString g_sNode;
  COARnotes()  : m_Update((time_t)0)
  {
    RegisterAll(true);
  }
  COARnotes(const COARnotes &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  COARnotes(const wxString &s, wxDateTime &t)
  {
    RegisterAll(true);
    m_sText = s;
    m_Update = t;
  }
  COARnotes &operator = (const COARnotes &x)
  {
    COARcopy(m_Update);
    COARcopy(m_sText);
    return *this;
  }
  virtual ~COARnotes() {;}
  virtual bool Skip(void *)
  {
    return m_sText.IsEmpty() && (!m_Update.GetTicks());
  }
  virtual void RegisterAll(bool = false);
  virtual const wxString &RootNode(void) const
  {
    return g_sNode;
  }
  const wxString &GetText() const
  {
    return m_sText;
  }
  static const wxString &GetText(const COARnotes *p)
  {
    if(p == NULL)
    {
      return g_EmptyString;
    }
    return p->GetText();
  }
  const wxDateTime &GetTime() const
  {
    return m_Update;
  }
  void SetText(const wxString &s)
  {
    m_sText = s;
  }
  void SetTime(wxDateTime &x)
  {
    m_Update = x;
  }
  void SetTime()
  {
    m_Update.SetToCurrent();
  }
  void SetTime(time_t t)
  {
    m_Update.Set(t);
  }
  void SetTime(const wxDateTime &t)
  {
    m_Update = t;
  }
  bool operator ==(const COARnotes &x) const
  {
    return m_sText == x.m_sText;
  }
  bool operator !=(const COARnotes &x) const
  {
    return m_sText != x.m_sText;
  }
  bool IsEmpty() const
  {
    return m_sText.IsEmpty();
  }
  bool NotEmpty() const
  {
    return !(m_sText.IsEmpty());
  }
  bool Update(
    const COARnotes &x,
    vector<COARnotes *> *pvpOldNotes,
    const wxDateTime &t);
  void Append(
    const wxString &sText,
    const wxString &sUser,
    vector<COARnotes *> *pvpOldNotes
    );

  bool Update(const COARnotes &x, vector<COARnotes *> *pvpOldNotes)
  {
    wxDateTime t;
    t.SetToCurrent();
    return Update(x,pvpOldNotes,t);
  }
  static wxString AppendNotes(
    const wxString &sNotes,
    const wxString &sAppend,
    const wxString &sUser);

  static const COARnotes *_FindNotes(
    const COARnotes *pNotesCurrent,
    const vector<COARnotes *> *pvNotesOld,
    const wxDateTime *pTime);
  static bool _AreNotesEdited(
    const COARnotes *pNotesCurrent,
    const vector<COARnotes *> *pvNotesOld,
    const wxDateTime *pTime);

private:
  wxDateTime m_Update;
  wxString m_sText;
  static const wxString g_EmptyString;
};


#endif
