#ifndef __NWX_ZIP_OUTPUT_H__
#define __NWX_ZIP_OUTPUT_H__

#include <wx/string.h>
#include <wx/stream.h>
#include <wx/datetime.h>
#include "nwxZip/nwxZipBase.h"
#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"

class wxZipOutputStream;
class wxZipEntry;
class wxTempFileOutputStream;


class nwxZipOutput : public nwxZipBase
{
public:
  nwxZipOutput() :
    m_pStream(NULL),
    m_pFileStream(NULL)
    {}

  nwxZipOutput(const wxString sZipFilePath) :
    m_pStream(NULL),
    m_pFileStream(NULL)
    {
      OpenZip(sZipFilePath);
    }
  virtual ~nwxZipOutput()
  {
    CloseZip();
  }

  bool OpenZip(const wxString &sZipFilePath);
  bool CloseZip();
  void Discard()
  {
    _Cleanup();
  }
  bool IsZipOpen()
  {
    return (m_pStream != NULL);
  }
  wxOutputStream *OpenFile(const wxString &sFileName,bool bFullPath = false, const wxDateTime &dt = wxDateTime::Now() );
  bool CloseFile();

  bool WriteFileToCwd(const wxString &sInputFilePath);
  bool WriteFile(const wxString &sInputFilePath, const wxString &sFileName, bool bFullPath = false);
  bool WriteDir(const wxString &sInputPath);
  bool Chdir(const wxString &sFileName, bool bFullPath = false);
private:
  bool _BeenDoneRaw(const wxString &s)
  {
    std::set<wxString>::iterator itr = m_setDone.find(s);
    bool bRtn = (itr != m_setDone.end());
    return bRtn;
  }
  void _SetDoneRaw(const wxString &s)
  {
    m_setDone.insert(s);
  }
#ifdef __WXMSW__
  bool _BeenDone(const wxString &s)
  {
    wxString sl(s);
    sl.MakeLower();
    return _BeenDoneRaw(sl);
  }
  void _SetDone(const wxString &s)
  {
    wxString sl(s);
    sl.MakeLower();
    _SetDoneRaw(sl);
  }
#else
  bool _BeenDone(const wxString &s)
  {
    return _BeenDoneRaw(s);
  }
  void _SetDone(const wxString &s);
  {
    _SetDoneRaw(s);
  }
#endif


  void _Cleanup();
  std::set<wxString> m_setDone;
  wxString m_sZipPath;
  wxZipOutputStream *m_pStream;
  wxTempFileOutputStream *m_pFileStream;
};





#endif
