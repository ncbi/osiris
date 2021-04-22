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
*  OSIRIS is a desktop tool working on your computer with your own data.
*  Your sample profile data is processed on your computer and is not sent
*  over the internet.
*
*  For quality monitoring, OSIRIS sends some information about usage
*  statistics  back to NCBI.  This information is limited to use of the
*  tool, without any sample, profile or batch data that would reveal the
*  context of your analysis.  For more details and instructions on opting
*  out, see the Privacy Information section of the OSIRIS User's Guide.
*
* ===========================================================================
*
*
*  FileName: CPositiveControls.cpp
*  Author:   Douglas Hoffman
*
*  classes for reading and writing standard positive control files.
*
*/
// all code is in header file
// the following #define is needed to instantiate static class variable
#include "CPositiveControls.h"
#include "mainApp.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/nwxString.h"
#include "ConfigDir.h"
#include <memory>

class _CPositiveControlAllele : public nwxXmlPersist
{
public:
  _CPositiveControlAllele() :
    m_sAllele(wxT("0")), m_sBioID("0")
  {
    RegisterAll(true);
  }
  _CPositiveControlAllele(const wxString &sName) :
    m_sBioID("0")
  {
    _SetAllele(sName);
    RegisterAll(true);
  }
  _CPositiveControlAllele(const _CPositiveControlAllele &a) :
    m_sBioID("0")
  {
    _SetAllele(a.GetAllele());
    RegisterAll(true);
  }
  virtual ~_CPositiveControlAllele() {}
  bool Less(const _CPositiveControlAllele &x) const
  {
    bool bRtn = false;
    if (nwxString::IsNumber(m_sAllele, false) &&
      nwxString::IsNumber(x.m_sAllele))
    {
      double d1 = atof(m_sAllele.utf8_str());
      double d2 = atof(x.m_sAllele.utf8_str());
      bRtn = d1 < d2;
    }
    else
    {
      bRtn = m_sAllele < x.m_sAllele;
    }
    return bRtn;
  }
  static wchar_t XYtoN(const wchar_t x)
  {
    wchar_t nRtn = x;
    switch (x)
    {
    case L'x':
    case L'X':
      nRtn = L'1';
      break;
    case L'y':
    case L'Y':
      nRtn = L'2';
      break;
    default:
      break;
    }
    return nRtn;
  }
  static wchar_t NtoXY(const wchar_t x)
  {
    wchar_t nRtn = x;
    switch (x)
    {
    case L'1':
      nRtn = L'X';
      break;
    case L'2':
      nRtn = L'Y';
      break;
    default:
      break;
    }
    return nRtn;
  }
  const wxString &GetAllele() const
  {
    return m_sAllele;
  }
  operator const wxString&() const
  {
    return GetAllele();
  }
  wxString GetAmelAllele() const
  {
    wxString sRtn = (m_sAllele.Len() == 1) ? NtoXY(m_sAllele.GetChar(0)) : m_sAllele;
    return sRtn;
  }
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(wxT("Name"), &m_sAllele);
    RegisterWxString(wxT("BioID"), &m_sBioID);
  }

private:
  void _SetAllele(const wxString &s)
  {
    m_sAllele = (s.Len() == 1) ? XYtoN(s.GetChar(0)) : m_sAllele;
  }
  wxString m_sBioID;
  wxString m_sAllele;
};

template <class T> class CLess
{
public:
  CLess<T>() {}
  bool operator()(const T &x1, const T &x2) const
  {
    return x1.Less(x2);
  }
  bool operator()(const T *px1, const T *px2) const
  {
    return px1->Less(*px2);
  }
};

typedef CLess<_CPositiveControlAllele> CLessAllele;


class _CPositiveControlLocus : public nwxXmlPersist
{
public:
  typedef std::set<_CPositiveControlAllele *, CLessAllele> ALLELE_SET;
  typedef ALLELE_SET::const_iterator ALLELE_ITR;
  _CPositiveControlLocus()
  {
    RegisterAll(true);
  }
#if 0
  // test when no longer read only
  _CPositiveControlLocus(const wxString &sName, const std::set<wxString> &setAlleles) : m_sName(sName)
  {
    RegisterAll(true);
    SetAlleles(setAlleles);
  }
#endif
  _CPositiveControlLocus(const wxString &sName) : m_sName(sName)
  {
    RegisterAll(true);
  }
  void ClearAlleles()
  {
    setptr<_CPositiveControlAllele, CLessAllele>::cleanup(&m_setAllele);
  }
  bool Less(const _CPositiveControlLocus &x) const
  {
    return GetName() < x.GetName();
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  const ALLELE_SET *GetAlleles() const
  {
    return &m_setAllele;
  }
  virtual ~_CPositiveControlLocus()
  {
    m_io.Cleanup();
  }
#if 0
  // debug when no longer read only
  void SetName(const wxString &sName)
  {
    m_sName = sName;
  }
  void AddAllele(const wxString &sName)
  {
    _CPositiveControlAllele a(sName);
    if (m_setAllele.find(&a) == m_setAllele.end())
    {
      m_setAllele.insert(new _CPositiveControlAllele(a));
    }
  }
  void SetAlleles(const std::set<wxString> &setAlleles)
  {
    ClearAlleles();
    for (std::set<wxString>::const_iterator itr = setAlleles.begin();
      itr != setAlleles.end();
      ++itr)
    {
      AddAllele(*itr);
    }
  }
#endif
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(wxT("Name"), &m_sName);
    Register(wxT("Allele"), &m_io, (void *)&m_setAllele);
  }
private:
  wxString m_sName;
  ALLELE_SET m_setAllele;
  TnwxXmlIOPersistSet<_CPositiveControlAllele, CLessAllele> m_io;
};

typedef CLess<_CPositiveControlLocus> CLessPositiveControlLocus;


class _CPositiveControl : public nwxXmlPersist
{
public:
  typedef TnwxXmlPersistSet<_CPositiveControlLocus, CLessPositiveControlLocus> LOCUS_SET_PERSIST;
  typedef std::set<_CPositiveControlLocus *, CLessPositiveControlLocus> LOCUS_SET;
  typedef std::set<_CPositiveControlLocus *, CLessPositiveControlLocus>::const_iterator LOCUS_ITR;
  _CPositiveControl() : m_setLoci(wxT("Locus"))
  {
    RegisterAll(true);
  }
  _CPositiveControl(const wxString &s) : m_sName(s), m_setLoci(wxT("Locus"))
  {
    RegisterAll(true);
  }
  virtual ~_CPositiveControl()
  {
    m_setLoci.Clear();
  }
  const LOCUS_SET *GetLoci() const
  {
    return m_setLoci.Get();
  }
  const _CPositiveControlLocus *Find(const wxString &sName) const
  {
    _CPositiveControlLocus x(sName);
    LOCUS_ITR itr = GetLoci()->find(&x);
    const _CPositiveControlLocus *pRtn = (itr == GetLoci()->end()) ? NULL : *itr;
    return pRtn;
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  bool Less(const _CPositiveControl &x) const
  {
    return GetName() < x.GetName();
  }

#if 0
  // test when no longer readonly
  void SetLocus(const wxString &sName, const std::set<wxString> &setAlleles)
  {
    unique_ptr<_CPositiveControlLocus> pLocus(new _CPositiveControlLocus(sName, setAlleles));
    LOCUS_SET *psetLocus = _GetLoci();
    _LOCUS_ITR itr = psetLocus->find(pLocus.get());
    if (itr != psetLocus->end)
    {
      psetLocus->erase(itr);
    }
    psetLocus->insert(pLocus.release());
  }
#endif
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("Name", &m_sName);
    Register("Loci", &m_setLoci);
  }
private:
  typedef std::set<_CPositiveControlLocus *, CLessPositiveControlLocus>::iterator _LOCUS_ITR;
  wxString m_sName;
  LOCUS_SET_PERSIST m_setLoci;

#if 0
  LOCUS_SET *_GetLoci()
  {
    return m_setLoci.Get();
  }
#endif
};

typedef CLess<_CPositiveControl> CLessPositiveControl;

class _CPositiveControls : public nwxXmlPersist
{
public:
  typedef std::set<_CPositiveControl *, CLessPositiveControl> CONTROL_SET;
  typedef CONTROL_SET::const_iterator CONTROL_ITR;
  _CPositiveControls()
  {
    RegisterAll(true);
  }
  virtual ~_CPositiveControls()
  {
    m_io.Cleanup();
  }
  virtual const wxString &RootNode(void) const
  {
    return g_ROOT;
  }
  const _CPositiveControl *Find(const wxString &sName) const
  {
    _CPositiveControl x(sName);
    CONTROL_ITR itr = m_setControls.find(&x);
    const _CPositiveControl *pRtn = (itr == m_setControls.end()) ? NULL : *itr;
    return pRtn;
  }
protected:
  void RegisterAll(bool = false)
  {
    Register(wxT("PositiveControl"), &m_io, (void *)&m_setControls);
  }
private:
  TnwxXmlIOPersistSet<_CPositiveControl, CLessPositiveControl> m_io;
  CONTROL_SET m_setControls;
  static const wxString g_ROOT;
};

const wxString _CPositiveControls::g_ROOT(wxT("PositiveControls"));


nwxIMPLEMENT_GLOBAL_OBJECT(CPositiveControlsAll)

void CPositiveControlsAll::_Cleanup()
{
  if (m_pPosSite != NULL)
  {
    delete m_pPosSite;
    m_pPosSite = NULL;
  }
}

void CPositiveControlsAll::_CheckSiteFile()
{
  if (!wxFileName::IsFileReadable(m_sSiteFileName))
  {
    _Cleanup();
  }
  else if(m_pPosSite == NULL)
  {
    m_pPosSite = new _CPositiveControls();
    if (!m_pPosSite->LoadFile(m_sSiteFileName))
    {
      _Cleanup();
      if (!m_bErrorLogged)
      {
        wxString s = wxT("Cannot load positive control file:\n");
        s.Append(m_sSiteFileName);
        mainApp::LogMessage(s);
        mainApp::ShowError(s, NULL);
        m_bErrorLogged = true;
      }
    }
  }
  else
  {
    m_pPosSite->CheckFileModification(true);
  }
}
CPositiveControlsAll::~CPositiveControlsAll()
{
  _Cleanup();
  delete m_pPosOsiris;
}

CPositiveControlsAll::CPositiveControlsAll() : 
  m_pPosSite(NULL), m_bErrorLogged(false)
{
  ConfigDir *pDir = mainApp::GetConfig();
  wxString sOsirisFileName = pDir->GetPositiveControlsFileName();
  m_sSiteFileName = pDir->GetSitePositiveControlsFileName();
  m_pPosOsiris = new _CPositiveControls();
  m_pPosOsiris->LoadFile(sOsirisFileName);
  _CheckSiteFile();
}

const wxString CPositiveControlsAll::GetFileName(const wxString &sControlName)
{
  wxString sRtn;
  CPositiveControlsAll *p = CPositiveControlsAll::GetGlobal();
  const _CPositiveControls *pp[] = {p->m_pPosOsiris, p->m_pPosSite};
  const size_t nSize = sizeof(pp) / sizeof(pp[0]);
  const _CPositiveControls *pos;
  for (size_t ndx = 0; ndx < nSize; ++ndx)
  {
    pos = pp[ndx];
    if (pos == NULL)
    {
      p->_CheckSiteFile();
      pos = pp[ndx];
    }
    if( (pos != NULL) && ((pos)->Find(sControlName) != NULL) )
    {
      sRtn = pos->GetLastFileName();
      ndx = nSize; // done
    }
  }
  return sRtn;
}