
#ifndef __NWX_ZIP_INPUT_H__
#define __NWX_ZIP_INPUT_H__

#include <wx/arrstr.h>
#include <wx/string.h>
#include <wx/stream.h>
#include <wx/datetime.h>
#include <wx/dir.h>
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"
#include "nwxZip/nwxZipBase.h"

class wxZipInputStream;
class wxZipEntry;
class wxFFileInputStream;

class nwxZipInput : public nwxZipBase
{
public:
  nwxZipInput():
    m_pCacheEntry(NULL),
    m_pStream(NULL),
    m_pFileStream(NULL)
  {}

  nwxZipInput(const wxString &sZipFilePath) : 
    m_pCacheEntry(NULL),
    m_pStream(NULL),
    m_pFileStream(NULL)
  {
    OpenZip(sZipFilePath);
  }
  virtual ~nwxZipInput()
  {
    _Cleanup();
  }
  bool OpenZip(const wxString &sZipFilePath);
  void CloseZip()
  {
    _Cleanup();
  }
  bool IsZipOpen()
  {
    return (m_pStream != NULL);
  }

  wxInputStream *OpenFile(const wxString &sFileName,bool bFullPath = false);
  bool WriteFile(const wxString &sOutputFilePath, const wxString &sFileName, bool bFullPath = false);
  bool WriteDir(const wxString &sOutputPath, const wxString &sDir, bool bFullPath = false);
  bool WriteAll(const wxString &sOutputPath);
  bool Chdir(const wxString &sFileName, bool bFullPath = false);
  bool IsFile(const wxString &sFileName, bool bFullPath = false);
  bool IsDir(const wxString &sFileName, bool bFullPath = false);
  bool InCwd(const wxString &sFileName, bool bFullPath = false);
  wxDateTime GetDateTime(const wxString &sFileName, bool bFullPath = false);
  size_t GetFiles(wxArrayString *pas, bool bCwd = false, bool bDirs = true);
  size_t GetDirs(wxArrayString *pas, bool bCwd = false);
  size_t GetSize(const wxString &sFileName, bool bFullPath = false);
private:
  void _Cleanup();
  wxZipEntry *_FindEntry(const wxString &sFileName, bool bFullPath);
  wxArrayString m_sAll;
  std::map<wxString, wxZipEntry *> m_pMapEntries;
  wxZipEntry *m_pCacheEntry;
  wxZipInputStream *m_pStream;
  wxFFileInputStream *m_pFileStream;
};


#endif
