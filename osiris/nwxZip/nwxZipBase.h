#ifndef __NWX_ZIP_BASE_H__
#define __NWX_ZIP_BASE_H__

#include <wx/string.h>

class nwxZipBase
{
public:
  nwxZipBase() {}
  virtual ~nwxZipBase() {}
  wxString Getdir()
  {
    return m_sCWD;
  }
  const wxString &GetOpenZipFilePath()
  {
    return m_sZipPath;
  }

protected:
  friend class DIR_HOLD;
  class DIR_HOLD
  {
  public:
    DIR_HOLD(nwxZipBase *p) :
        m_pZip(p),
        m_sCWD(p->_Dir())
    {}
    virtual ~DIR_HOLD()
    {
      m_pZip->_Chdir(m_sCWD);
    }
  private:
    wxString m_sCWD;
    nwxZipBase *m_pZip;
  };

  wxString _BUILD_PATH(
    const wxString &sFileName, 
    bool bFullPath)
  {
    wxString sRtn;
    if(bFullPath)
    {
      sRtn = sFileName;
    }
    else
    {
      sRtn = __MAKE_PATH(_Dir(),sFileName);
    }
    return sRtn;
  }
  void _Chdir(const wxString &s)
  {
    m_sCWD = s;
  }
  void _ClearDir()
  {
    m_sCWD.Empty();
  }
  void _ClearBase()
  {
    _ClearDir();
    m_sZipPath.Empty();
  }
  const wxString &_Dir() const
  {
    return m_sCWD;
  }
  void _SetOpenZipFilePath(const wxString &s)
  {
    m_sZipPath = s;
  }
  static wxString __MAKE_PATH(const wxString &sDir, const wxString &sPath);
  // __MAKE_PATH is implemented in nwxZipInput.cpp
private:
  wxString m_sCWD;
  wxString m_sZipPath;

};


#endif