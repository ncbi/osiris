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
*  FileName: nwxFileUtil.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_FILE_DIALOG_H__
#define __NWX_FILE_DIALOG_H__

#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/datetime.h>
#include <wx/dir.h>

class nwxFileUtil
{
public:

  static wxFileDialog *CreateSaveDialog(
    wxWindow *parent,
    const wxString &sTitle,
    const wxString &sType,
    const wxString &sCurrentName,
    const wxString &sDefaultName);
  static wxString SetupFileName(
    const wxString &sOriginalFile,
    const wxString &sExt);
  static bool ShowFileFolder(const wxString &sFileName);
  static wxString BaseName(const wxString &sDir);
  static void NoEndWithSeparator(wxString *psDir);
  static void NoStartWithSeparator(wxString *psDir);
  static void EndWithSeparator(wxString *psDir);
  static bool UpDir(wxString *psDir, int n = 1);
  static bool MkDir(const wxString &sDir);
  static size_t GetAllFilesNoCase(const wxString &sDirName, wxArrayString *pasFile, const wxString &sFileSpec, int flags = wxDIR_DEFAULT);
  static bool IsNewer(const wxString &sFileName, time_t t)
  {
    bool bRtn = !t;
    if(!bRtn)
    {
      wxFileName fn(sFileName);
      if(fn.FileExists())
      {
        bRtn = (fn.GetModificationTime().GetTicks() >= t);
      }
    }
    return bRtn;
  }
  static bool IsNewer(const wxString &sFileName, const wxDateTime &dt)
  {
    return IsNewer(sFileName,dt.GetTicks());
  }
  static void SetDoNotSelectFile(bool b = true)
  {
    DO_NOT_SELECT_FILE = b;
  }
  static bool GetDoNotSelectFile()
  {
    return DO_NOT_SELECT_FILE;
  }
#if 0
  // written but not tested, used wxCopyFile instead
  static int CopyFile(
    const wxString &sFrom, const wxString &sTo,
    bool bOverwrite = false,
    wxString *psError = NULL);
  enum
  {
    COPY_OK = 0,
    INPUT_FILE_MISSING,
    OUTPUT_FILE_EXISTS,
    INPUT_OPEN_ERROR,
    OUTPUT_OPEN_ERROR,
    READ_ERROR,
    WRITE_ERROR
  } COPY_RETURN_CODE;
  static wxString CopyErrorMessage(
    int nRtn,
    const wxString &sFrom, 
    const wxString &sTo);
#endif
private:
  static bool DO_NOT_SELECT_FILE;
  static bool _ShowFolder(const wxString &sFileName);
  nwxFileUtil() {;} // prevent instantiation
};


#endif
