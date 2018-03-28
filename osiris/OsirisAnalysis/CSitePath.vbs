Option Explicit
'
'  breaks
'    parent 695, 791, 796
'    child 827, 919
'
'


Public wshShell,fso,envProcess,sCacls,sIcacls,bVerbose,sWhoAmI, sEOL, STDOUT, STDERR, sCmdExe
Const CSCRIPT_PARAM = "/NOLOGO " ' must end with a space unless empty

bVerbose = False
sCacls = Null
sIcacls = Null
sCmdExe = ""
Set wshShell = CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")
Set envProcess = wshShell.Environment("PROCESS")
Set STDOUT = fso.GetStandardStream (1)
Set STDERR = fso.GetStandardStream (2)
sEOL = Chr(13) & Chr(10)
sWhoAmI = Null


Sub PRINT(sStr, bShow)
  If bShow Then
    STDERR.WriteLine sStr
  End If 
End Sub

Sub PRINT_ARRAY(sHeader, byRef aList, bShow)
  If bShow Then
    Dim i,n,bPrinted
    bPrinted = False
    If not IsNull(sHeader) Then
      PRINT sHeader,True
      bPrinted = True
    End If
    n = UBound(aList)
    If n >= 0 Then
      For i = 0 to n
        PRINT aList(i),True
      Next
      bPrinted = True
    End If
    If bPrinted Then
      PRINT "",True ' line feed
    End If
  End If
End Sub

Function CmpStr(s1,s2)
  If s1 < s2 Then
    CmpStr = -1
  ElseIf s2 < s1 Then
    CmpStr = 1
  Else
    CmpStr = 0
  End If
End Function

Function CmpStrNoCase(s1,s2)
  Dim rtn
  rtn = CmpStr(LCase(s1),LCase(s2))
  If rtn = 0 Then
    rtn = CmpStr(s1,s2)
  End If
  CmpStrNoCase = rtn
End Function

Sub SortArrayNoCase(byRef aList)
  Dim nBottom,i,j,i1,s, bDone, nCmp
  nBottom = UBound(aList) - 1
  nCmp = 0
  For j = nBottom to 0 Step -1
    bDone = True
    For i = 0 to j
      i1 = i + 1
      nCmp = nCmp + 1
      If CmpStrNoCase(aList(i),aList(i1)) > 0 Then
        bDone = False
        s = aList(i)
        aList(i) = aList(i1)
        aList(i1) = s
      End If
    Next
    If bDone Then
      Exit For
    End If
  Next
  PRINT "SortArrayNoCase compare count = " & nCmp & ", list size = " & (UBound(aList) + 1), bVerbose
End Sub

Function SplitString(s,c)
  Dim nLenC,ndx,nFind,sPath,sThis
  Redim aPath(20)
  nLenC = Len(c)
  sPath = s
  
  nFind = Instr(sPath,c)
  ndx = -1
  While nFind > 0
    If nFind > 1 Then
      ndx = ndx + 1
      If ndx > UBound(aPath) Then
        Redim PRESERVE aPath(ndx + 20)
      End If
      aPath(ndx) = Left(sPath,nFind - 1)
    End If
    sPath = Mid(sPath,nFind + nLenC)
    nFind = Instr(sPath,c)
  Wend
  If Len(sPath) > 0 Then
    ndx = ndx + 1
    if ndx > UBound(aPath) Then
      Redim PRESERVE aPath(ndx)
    End If
    aPath(ndx) = sPath
  End If
  If ndx < 0 Then
    SplitString = Null
  Else
    If ndx < UBound(aPath) Then
      Redim PRESERVE aPath(ndx)
    End If
    SplitString = aPath
  End If
End Function


Function EndsWith(s,c)
  Dim nLenS,nLenC
  nLenS = len(s)
  nLenC = len(c)
  If nLenC = 0 Then
    EndsWith = True
  Elseif nLenC > nLenS Then
    EndsWith = False
  ElseIf Right(s,nLenC) = c Then
    EndsWith = True
  Else
    EndsWith = False
  End If
End Function

Function MakeEndWith(s,c)
  If EndsWith(s,c) Then
    MakeEndWith = s
  Else
    MakeEndWith = s & c
  End If
End Function


Function SystemFind(sFileName)
  Dim systemRoot
  systemRoot = envProcess("SystemRoot")
  if Len(systemRoot) = 0 Then
    SystemFind = Null
  Else
    Dim sPath,sFullPath
    sPath = MakeEndWith(systemRoot,"\")
    sFullPath = sPath & sFileName
    if fso.FileExists(sFullPath) Then
      SystemFind = sFullPath
    Else
      sFullPath = sPath & "System32\" & sFileName
      if fso.FileExists(sFullPath) Then
        SystemFind = sFullPath
      Else
        SystemFind = Null
      End If
    End If
  End If
End Function


Function PathFind(sFileName)
  Dim sPath,aPath,sFilePath, s
  sPath = envProcess("Path")
  aPath = SplitString(sPath,";")
  PathFind = Null
  For Each s in aPath
    sFilePath = MakeEndWith(s,"\") & sFileName
    if fso.FileExists(sFilePath) Then
      PathFind = sFilePath
      Exit For
    End If
  Next
End Function


Sub AddToArray(ByRef aList, value, nNdx)
  Dim nMax
  nMax = UBound(aList)
  If nMax < nNdx Then
    While nMax < nNdx
      nMax = nMax + nMax
    Wend
    Redim PRESERVE aList(nMax)
  End If
  aList(nNdx) = value
End Sub

Sub StreamToArray(ByRef aList, ByRef stream)
  Dim n,sLine
  n = 0
  While Not stream.atEndOfStream
    sLine = stream.ReadLine()
    AddToArray aList,sLine,n 
    n = n + 1
  Wend
  Redim PRESERVE aList(n - 1)
End Sub


Function ShellRun(sCmdLine)
  Dim shell
  Set shell = CreateObject("WScript.Shell")
  Set ShellRun = shell.Exec(sCmdLine)
End Function  
  

Function ShellPiped(sCmdLine)
  Dim rtn
  ReDim aStdOut(32)
  ReDim aStdErr(8)
  Dim exec,nStdErr,nStdOut,nAllocStdOut, nAllocStdErr, sLine
  nStdErr = 0
  nStdOut = 0
  Set rtn = CreateObject("Scripting.Dictionary")
  Set exec = ShellRun(sCmdLine)
  If (Not IsNull(sCacls)) And (InStr(1,sCmdLine,sCacls,1) > 0) Then
    exec.StdIn.WriteLine "y"
  End If
  StreamToArray aStdOut,exec.StdOut
  StreamToArray aStdErr,exec.StdErr
  While exec.Status = 0
    Sleep 50
  Wend
  rtn.Add "return", exec.Status
  rtn.Add "stdout", aStdOut
  rtn.Add "stderr", aStdErr
'  If bVerbose Then
'    Dim n,i
'    PRINT "Command: " &  sCmdLine, True
'    PRINT_ARRAY "stdout:", aStdOut, True
'    PRINT_ARRAY "stderr:", aStdErr, True
'  End If
  Set exec = Nothing
  Set ShellPiped = rtn
  Set rtn = Nothing
End Function

Dim objWMIService
objWMIService = Null

Function GetFileOwner(strFileName)
    ' https://www.vbsedit.com/scripts/security/ownership/scr_1386.asp
  On Error Resume Next
  Dim objFileSecuritySettings, intRetVal, objSD

  If IsNull(objWMIService) Then
    Set objWMIService = GetObject("winmgmts:")
  End If
  Set objFileSecuritySettings = _
    objWMIService.Get("Win32_LogicalFileSecuritySetting='" & strFileName & "'")
  If IsEmpty(objFileSecuritySettings) Then
    intRetVal = 1
  Else
    intRetVal = objFileSecuritySettings.GetSecurityDescriptor(objSD)
    If IsEmpty(objSD) Then
      intRetVal = 1
    End If
  End If
  If intRetVal = 0 Then
    GetFileOwner = objSD.Owner.Domain & "\" & objSD.Owner.Name
  Else
    GetFileOwner = Null
    PRINT "Cannot find owner of " & strFileName, bVerbose
  End If

End Function

Sub SetupCmdExe()
  If sCmdExe = "" Then
    sCmdExe = SystemFind("cmd.exe")
  End If
End Sub

Function IsFileOwner(sPath,sUserLower)
  Dim sOwner
  sOwner = GetFileOwner(sPath)
  If IsNull(sOwner) or (LCase(sOwner) <> sUserLower) Then
    IsFileOwner = False
  Else
    IsFileOwner = True
  End If
End Function  

Function GETOWNER(sPath)
  Dim rtn,sOwner
  If fso.FolderExists(sPath) or fso.FileExists(sPath) Then
    sOwner = GetFileOwner(sPath)
    If (sOwner = "") Then
      rtn = False
    Else
      If bVerbose Then
        STDERR.WriteLine sOwner
      End If
      rtn = (sOwner <> "\")
    End If
  Else
    rtn = False
  End If
  GETOWNER = rtn
End Function

Function ISOWNER(sPath, sUser)
  '
  ' If sPath is a directory
  ' traverse the directory until a file is found
  '  that is not owned by 'sUser'
  ' If all files owned by sUser, then return True
  '   Note: very slow if many files
  '   are being checked
  '
  '
  ' If sPath is a file, return True is file is owned
  '   by 'sUser'
  '
  Dim rtn, sUserLower
  sUserLower = LCase(sUser)
  rtn = True
  SetupCmdExe
  If IsNull(sCmdExe) Then
    PRINT "Cannot find cmd.exe",True
    rtn = False
  ElseIf fso.FolderExists(sPath) Then
    If Not IsFileOwner(sPath,sUserLower) Then
      rtn = False
    Else
      Dim sCmd, exec, sLine
      sCmd = """" & sCmdExe & """ /c dir /b /s """ & sPath & """"
      Set exec = ShellRun(sCmd)
      While rtn and (Not exec.stdOut.atEndOfStream)
        sLine = exec.StdOut.ReadLine()
        rtn = (Len(sLine) = 0) Or _
            IsFileOwner(sLine,sUserLower)
      Wend
      If rtn And (exec.Status > 1)  Then
        rtn = False
      End If
      While Not exec.StdErr.atEndOfStream
        PRINT exec.StdOut.ReadLine(),True
      Wend
      Set exec = Nothing
    End If
  ElseIf fso.FileExists(sPath) Then
    rtn = IsFileOwner(sPath,sUserLower)
  Else
    PRINT "Path, " & sPath & ", does not exist", True
    rtn = False
  End If
  ISOWNER = rtn
End Function

Function GetGroups(byRef aList)
  Dim nNdx
  nNdx = 0
  GetGroups = False
  If IsNull(sWhoAmI) Then
    sWhoAmI = SystemFind("whoami.exe")
  End If
  if Not IsNull(sWhoAmI) Then
    Dim objShell, aStdErr, aStdout, nLines, i, sLine, sLastGroup
    Set objShell = ShellPiped(sWhoAmI & " /GROUPS /FO LIST")
    If objShell.Item("return") = 1 Then
      GetGroups = True
      aStdOut = objShell.Item("stdout")
      nLines = UBound(aStdOut)
      If nLines > 1 Then
        i = Int((nLines + 3) * 0.25)
        Redim aList(i)
        sLastGroup = ""
        For i = 0 to nLines
          sLine = aStdOut(i)
          if Left(sLine,11) = "Group Name:" Then
            sLastGroup = Trim(Mid(sLine,12))
          ElseIf Len(sLine) > 5 _
              And Left(sLine,5) = "Type:" _
              And InStr(1,sLine,"Group",1) > 0 _
              And Len(sLastGroup) > 0 Then
            AddToArray aList,sLastGroup,nNdx
            nNdx = nNdx + 1
            sLastGroup = ""
          End If
        Next
      End If
    End If
  End If
  Redim PRESERVE aList(nNdx - 1)
  SortArrayNoCase aList
End Function

Function SETUP_CACLS()
  Dim sExe
  If IsNull(sIcacls) And IsNull(sCacls) Then
    SETUP_CACLS = False
    sExe = SystemFind("icacls.exe")
    If not IsNull(sExe) Then
      sIcacls = sExe
      SETUP_CACLS = True
    End If
    sExe = SystemFind("cacls.exe")
    If Not IsNull(sExe) Then
      sCacls = sExe
      SETUP_CACLS = True
    End If
  Else
    SETUP_CACLS = True
  End If
  If Not SETUP_CACLS Then
    PRINT "Cannot find icacls.exe not cacls.exe",bVerbose
  End If
End Function

Function CACLS_OK(byRef aStdOut)
  Dim i,n,str
  n = UBound(aStdOut)
  CACLS_OK = True
  For i = 0 to n
    str = aStdOut(i)
    If InStr(1,str,"Successfully processed 0 files",1) > 0 Then
      CACLS_OK = False
      Exit For
    ElseIf InStr(1,str,"Failed processing",1) > 0 And _
           InStr(1,str,"Failed processing 0 ",1) < 1 Then
      CACLS_OK = False
      Exit For
    End If
  Next
    
End Function

Function CACLS_RESET(sPath)
  CACLS_RESET = False
  If SETUP_CACLS() Then
    Dim sCmd, objResult
    If Not IsNull(sIcacls) Then
      sCmd = """" & sIcacls & """ """ & sPath & """ /reset /T /Q"
      Set objResult = ShellPiped(sCmd)
      If (objResult.Item("return") = 1) And _
          CACLS_OK(objResult.Item("stdout")) Then
        CACLS_RESET = True
      End If
    End If
  End If
End Function

Function CmdGrantAll(sPath,sWho)
  If Not IsNull(sIcacls) Then
    CmdGrantAll = """" & sIcacls & """ """ & sPath & """ /grant """ & sWho & ":(OI)(CI)F"" /T /Q"
  ElseIf Not IsNull(sCacls) Then
    CmdGrantAll = """" & sCacls & """ """ & sPath & """ /G """ & sWho & ":F"" /T "
  Else
    CmdGrantAll = Null
  End If

End Function

Function CACLS_SETGROUP(sPath, sGroup)
  CACLS_SETGROUP = False
  If sGroup <> "" Then
    Dim sCmd
    sCmd = CmdGrantAll(sPath,sGroup)
    If Not IsNull(sCmd) Then
      Dim objResult
      Set objResult = ShellPiped(sCmd)
      If objResult.Item("return") = 1 Then
        CACLS_SETGROUP = CACLS_OK(objResult.Item("stdout"))
      End If
    End If
  End If
End Function

Function CACLS_SETOWNER(sPath, sUser)
  ' note if icacls /setowner fails, the return code
  ' it still 0, so checking stdout
  CACLS_SETOWNER = False
  If sUser <> "" Then
    If Not IsNull(sIcacls) Then
      Dim objResult,sCmd
      sCmd = """" & sIcacls & """ """ & sPath & """ /setowner """ & _
          sUser & """ /T /Q "
      Set objResult = ShellPiped(sCmd)
      If objResult.Item("return") = 1 Then
        CACLS_SETOWNER = CACLS_OK(objResult.Item("stdout"))
      End If
    End If
  End If
End Function

Function MakePath(sPath)
  Dim rtn
  rtn = True
  If fso.FolderExists(sPath) Then
    'rtn = True
  ElseIf not MakePath(fso.GetParentFolderName(sPath)) Then
    rtn = False
  Else
    On Error Resume Next
    Dim folder
    Set folder = fso.CreateFolder(sPath)
    If  Err.Number <> 0 or not fso.FolderExists(sPath) Then
      PRINT "CreateFolder(""" & sPath & """) failed",True
      rtn = False
    End If
  End If
  MakePath = rtn
End Function


' functions CHGRP, MKDIR, and GROUPS
' the functions specified on the command line

Function CHGRP(sPath,sUser,sGroup)
  Dim aAllGroups
  Dim rtn
  rtn = False
  PRINT "CHGRP(""" & sPath & """, """ & sUser & """, """ & sGroup & """)",bVerbose
  If not fso.FolderExists(sPath) Then
    PRINT "Directory, """ & sPath & """ does not exist", True
  ElseIf sUser <> "" Or sGroup <> "" Then
    rtn = CACLS_RESET(sPath)
    If rtn Then
      If sUser <> "" Then
        Dim obj,sCmd,n
        n = CACLS_SETOWNER(sPath,sUser)
        ' not checking return code because 
        ' it probably won't work on XP and isn't
        ' that important as long as CmdGrantAll below
        ' works
        sCmd = CmdGrantAll(sPath,sUser)
        Set obj = ShellPiped(sCmd)
        rtn = (obj.Item("return") = 1) And CACLS_OK(obj.Item("stdout"))
        If not rtn Then
          PRINT "Grant full access on """ & sPath & """ to """ & sUser & """ failed.", True
        ElseIf bVerbose Then
          PRINT "Granted full access on """ & sPath & """ to """ & sUser & """", True
        End If
      End If
      If rtn and sGroup <> "" Then
        rtn = CACLS_SETGROUP(sPath,sGroup)
        If not rtn Then
          PRINT "Grant full access on """ & sPath & """ to """ & sGroup & """ failed.", True
        ElseIf bVerbose Then
          PRINT "Granted full access on """ & sPath & """ to """ & sGroup & """", True
        End If
      End If
    End If
  End If
  CHGRP = rtn
End Function

Function ISNTFS(sPath)
  ISNTFS = False
  If Not(fso.FolderExists(sPath) or fso.FileExists(sPath)) Then
    ISNTFS = False
    PRINT "cannot find " & sPath, bVerbose
  ElseIf SETUP_CACLS() Then
    Dim hOutput, aStdOut, aStdErr,i,n,cmd,sOwner,sCheck1,sCheck2,s
    ISNTFS = True
    sOwner = GetFileOwner(sPath)
    If IsNull(sOwner) or (sOwner = "\") Then
      ISNTFS = False
    Else
      If Not IsNull(sCacls) Then
        cmd = sCacls & " """ & sPath & """"
      Else
        cmd = sIcacls & " """ & sPath & """"
      End If
      Set hOutput = ShellPiped(cmd)
      aStdOut = hOutput.Item("stdout")
      n = UBound(aStdOut)
      ISNTFS = True
      sCheck1 = "The Cacls command can be run only on disk drives that use the NTFS file system."
      sCheck2 = "No permissions are set. All users have full control."
      For i = 0 to n
        s = aStdOut(i)
        If InStr(1,s,sCheck1,1) > 0 Then
          ISNTFS = False
          Exit For
        ElseIf InStr(1,s,sCheck2,1) > 0 Then
          ISNTFS = False
          Exit For
        End If
      Next
    End If
  End If
  If ISNTFS Then
    PRINT "Is NTFS",bVerbose
  Else
    PRINT "Is not NTFS", bVerbose
  End If
End Function

Function MKDIR(sPath,sUser,sGroup)
  Dim rtn
  rtn = True
  PRINT "MKDIR(""" & sPath & """, """ & sUser & """, """ & sGroup & """)",bVerbose
  If not MakePath(sPath) Then
    rtn = False
  else
    rtn = CHGRP(sPath,sUser,sGroup)
  End If
  MKDIR = rtn
End Function

Function GROUPS()
  Dim rtn
  PRINT "GROUPS()", bVerbose
  Redim aList(0)
  rtn = GetGroups(aList)
  If rtn Then
    Dim i,n
    n = UBound(aList)
    For i = 0 to n
      STDOUT.WriteLine aList(i)
    Next
  ElseIf IsNull(sWhoAmI) Then
    STDERR.WriteLine "Cannot find the program whoami.exe"
  End If
  GROUPS = rtn
End Function

Sub USAGE
  STDERR.WriteLine _
    "Usage: CSitePath.vbs [-v] [-r folder] MKDIR|CHGRP path user group" & sEOL & _
    "       CSitePath.vbs [-v] [-r folder] GROUPS " & sEOL & _
    "       CSitePath.vbs [-v] [-r folder] ISOWNER path user" & sEOL & _
    "       CSitePath.vbs [-v] [-r folder] GETOWNER path" & sEOL & _
    "  -v        - verbose output" & sEOL & _
    "  -r folder - run as administrator.  'folder' is the location for a temp file" & sEOL & _
    "  MKDIR     - create the directory specified by 'path'" & sEOL & _
    "  CHGRP     - change the access rights of the directory specified by 'path'" & sEOL & _
    "              for full access by the specified user and/or group" & sEOL & _
    "  GROUPS    - list the groups for the current user" & sEOL & _
    "  GETOWNER  - if -v is used display the owner of the file or directory.  Returns 0 if an owner is found" & sEOL & _
    "  ISOWNER   - return 0 if the specified user owner all contents of 'path'" & sEOL & _
    "  path      - The full path of the directory being created or modified" & sEOL & _
    "  user      - The user or log in name to be granted full access to the directory" & sEOL & _
    "  group     - The group to be granted full access to the directory"
End Sub

' return size of file in bytes, -1 if file doesn't exist

Function FileSize(sFileName)
  On Error Resume Next
  Dim hFile
  FileSize = -1
  Set hFile = fso.GetFile(sFileName)
  If hFile Then
    FileSize = hFile.Size
    Set hFile = Nothing
  End If
End Function

'  BEGIN temp file operations
'  for communication between calling process and
'  elevated process

Const sReturnEqual = "return="


'  InitTempFile - called from elevated process
'  write < 100 bytes to the temp file to
'  to signal to the calling process that this has started
'  return true if OK, false otherwise
'
Function InitTempFile(sFileName)
  On Error Resume Next
  Dim hFile
  Set hFile = fso.OpenTextFile(sFileName, 8, False, 0)
  If hFile Then
    hFile.WriteLine "Begin"
    hFile.Close
    Set hFile = Nothing
    InitTempFile = True
  Else
    InitTempFile = False
  End If   
End Function

'  FinishTempFile - called from elevated process
'  Write the return code to the temp file
'  then make sure the size is > 100 bytes
'  for the calling process to know this is done
'  
Sub FinishTempFile(sFileName, nReturn)
  On Error Resume Next
  Dim hFile, sStr
  ' open existing file to append
  Set hFile = fso.OpenTextFile(sFileName, 8, False, 0)
  If hFile Then
    hFile.WriteLine sReturnEqual & nReturn
    ' setting file > 100 bytes signals the calling process
    ' that the file is ready
    sStr = "Done ---------------------------------------------"
    hFile.WriteLine sStr
    hFile.WriteLine sStr
    hFile.Close
    Set hFile = Nothing
  End If
End Sub
'
'  Before elevating, create an empty file
'  and pass then name to elevated process
'

Const TMP_FILE_LOOP = 9
Function GetFileName(i)
  GetFileName = "tmp_" & i & ".txt"
End Function

Function CleanupPath(sFilePath)
'
'  sFilePath - path ending with a \
'   called from CreateTempFile
'
  On Error Resume Next
  Dim i,sFileName, hFile, dNow, dMod, diffMin
  dNow = Now
  For i = 1 to TMP_FILE_LOOP
    sFileName = sFilePath & GetFileName(i)
    If fso.FileExists(sFileName) Then
      Set hFile = fso.GetFile(sFileName)
      If hFile Then
        dMod = hFile.DateLastModified
        Set hFile = Nothing
        diffMin = DateDiff("n",dMod,dNow)
        If (diffMin > 60)  Then ' older than 60 minutes
          fso.DeleteFile(sFileName)
        End If
      End If
    End If
  Next
End Function

Function CreateTempFile(sFilePath)
  Dim i, sFileName, bDone
  CreateTempFile = Null
  bDone = Not fso.FolderExists(sFilePath)
  If Not bDone Then
    If Right(sFilePath,1) <> "\" Then
      sFilePath = sFilePath & "\"
    End If
    CleanupPath(sFilePath)
    i = 1
    While not bDone
      sFileName = sFilePath & "tmp_" & i & ".txt"
      If fso.FileExists(sFileName) Then
        i = i + 1
        bDone = (i > TMP_FILE_LOOP)
      Else
        Dim hFile
        On Error Resume Next
        bDone = True
        Set hFile = fso.CreateTextFile(sFileName)
        If hFile Then
          hFile.Close
          Set hFile = Nothing
          If fso.FileExists(sFileName) Then
            CreateTempFile = sFileName
          End If
        End If
      End If
    Wend
  End If
End Function

'  Called from process that is creating an elevated process
'
'  Wait for the size of the temp file to be >= nMinSize
'  
'  if > 0 and < 100 bytes, the elevated process started
'  if > 100 bytes, the elevated process finished
'  return True for success, False otherwise
'
Function WaitForTempFileSize(sFileName, nMinSize, nTimeout)
  Dim i,nSize, nLoops
  nSize = 0
  WaitForTempFileSize = False
  nLoops = nTimeout * 10
  For i = 1 to nLoops
    nSize = FileSize(sFileName)
    If nSize < 0 Then
      Exit For ' error, return false
    ElseIf nSize >= nMinSize Then
      WaitForTempFileSize = True
      Exit For
    ElseIf i < nLoops Then
      WScript.Sleep(100) ' try again in 1/10 second
    End If
  Next
End Function

'  Wait for the size of the temp file to exceed
'  100 bytes to indicate that the elevated process
'  is done.  Then delete the file
'     return the return code from the file
'     or 10 for timeout
'
Function WaitForFinishTempFile(sFileName)
  WaitForFinishTempFile = WaitForTempFileSize(sFileName,100,5)
End Function
Function WaitForInitTempFile(sFileName)
  WaitForInitTempFile = WaitForTempFileSize(sFileName,2,2)
End Function

Function GetReturnCode(sFileName)
  '  look for return= in file
  '  if found, return number after =
  '  else return -1
  '
  On Error Resume Next
  Dim hFile,sLine, nLen, nLineLen, bDone,sRtn
  Set hFile = fso.OpenTextFile(sFileName,1,False,0) ' open for reading, ascii
  GetReturnCode = -1
  If hFile Then
    bDone = False
    nLen = Len(sReturnEqual)
    While Not (bDone or hFile.AtEndOfStream)
      sLine = hFile.ReadLine
      nLineLen = Len(sLine)
      If (nLineLen > nLen) And (Left(sLine,nLen) = sReturnEqual) Then
        sRtn = Trim(Right(sLine,nLineLen - nLen))
        GetReturnCode = CInt(sRtn)
        bDone = True
      End If
    Wend
    hFile.Close
    Set hFile = Nothing
    If GetReturnCode = 0 Then
      fso.DeleteFile(sFileName)
    End If
  End If
End Function


'  END temp file operations

Function PrepareParm(s)
'   prepare a command line parameter
'    if empty then return quoted empty string
'    if it contains a space, then return quoted string
'      to prevent it from being more than one parameter
'
  If Len(s) = 0 Then
    PrepareParm = """"""
  ElseIf Instr(s," ") > 0 Then
    PrepareParm = """" & s & """"
  Else
    PrepareParm = s
  End If
End Function

Function Elevate()
'
'  Elevate this process to run as administrator, then wait
'
  Dim objArgs,nCount,objShell,sCmd, sCmdLine,i, sFileName,sArg
  Set objArgs = WScript.Arguments
  Set objShell = CreateObject("Shell.Application")
  nCount = objArgs.Count
  sCmdLine = CSCRIPT_PARAM & PrepareParm(WScript.ScriptFullName)
  Elevate = False
  sFileName = Null
  For i = 0 to nCount - 1
    sArg = objArgs(i)
    If sArg = "-r" Then
      sCmdLine = sCmdLine & " -R"
      i = i + 1
      sArg = objArgs(i)
      sFileName = CreateTempFile(sArg)
      If IsNull(sFileName) Then
        Exit For
      Else
        sCmdLine = sCmdLine & " " & PrepareParm(sFileName)
      End If
    Else
      sCmdLine = sCmdLine & " " & PrepareParm(sArg)
    End If
  Next
  If Not IsNull(sFileName) Then
    sCmd = PrepareParm(wScript.FullName)
    PRINT "Elevate:" & sCmd & " " & sCmdLine, bVerbose
    objShell.ShellExecute sCmd, sCmdLine, "", "runas",0
    If Not WaitForInitTempFile(sFileName) Then
      ' failed - this will fail if the user declines running as admin
    ElseIf WaitForFinishTempFile(sFileName) Then
      Dim nRtn
      nRtn = GetReturnCode(sFileName)
      Elevate = (nRtn = 0) ' if 0 then success, else failed
    End If
  End If
End Function


sub MAIN
  Dim bUsage, objArgs, nCount, i, rtn, sFunction, sUser, sPath, sGroup, nArg, sArg, bElevate, sTempFile
  Set objArgs = WScript.Arguments
  bUsage = False
  nCount = objArgs.Count
  rtn = True ' not failed, yet
  sFunction = ""
  sUser = ""
  sGroup = ""
  sPath = ""
  nArg = 0
  bElevate = False
  sTempFile = Null
  i = 0
  While i < nCount
    sArg = objArgs(i)
    If sArg = "-v" Then
      bVerbose = True
    ElseIf sArg = "-r" Then
      bElevate = True
      i = i + 1 ' skip next arg which is temp path for -r
    ElseIf sArg = "-R" Then
      '  this is elevated, InitTempFile
      i = i + 1
      sTempFile = objArgs(i)
      If Not InitTempFile(sTempFile) Then
        PRINT "Cannot initialize temp file: " & sTempFile, True
        rtn = False ' failed
        sTempFile = Null
      End If
    ElseIf sArg = "-h" Then
      Usage()
      WScript.Quit(0)
    Else
      nArg = nArg + 1
      Select Case nArg
        Case 1
          sFunction = sArg
        Case 2
          sPath = sArg
        Case 3
          sUser = sArg
        Case 4
          sGroup = sArg
        Case 5
          PRINT "Excessive parameter(s): ",True
          PRINT "    " & sArg, True
        Case else
          PRINT "    " & sArg, True
      End Select
    End If
    i = i + 1
  Wend

  Dim sAllCmd, sNoPathCommand, sNoUserCommand, sFunctionSearch
  sAllCmd = "_:CHGRP:MKDIR:GROUPS:ISOWNER:GETOWNER:ISNTFS:"
  sNoUserCommand = "_:GROUPS:ISOWNER:GETOWNER:ISNTFS:"
  sFunctionSearch = ":" & sFunction & ":"

  If rtn Then ' not failed yet
    If (sFunction = "") OR (InStr(1, sAllCmd,sFunctionSearch) < 1) Then
      STDERR.WriteLine "No function (CHGRP, MKDIR, GROUPS, ISOWNER, ISNTFS) specified"
      bUsage = True
    Else 
      If sPath = "" and sFunction <> "GROUPS" Then
        STDERR.WriteLine "No path specified"
        bUsage = True
      End If
      If sUser = "" and sGroup = "" and InStr(1,sNoUserCommand, sFunctionSearch) < 1 Then
        STDERR.WriteLine "No user nor group specified"
        bUsage = True
      End If
    End If
    If (not bUsage) And (sFunction = "ISOWNER") Then
      If sUser = "" Then
        Dim o,sWhoAmI, aOut
        sWhoAmI = SystemFind("whoami.exe")
        If not IsNull(sWhoAmI) Then
          Set o = ShellPiped("""" & sWhoAmI & """")
          aOut = o.Item("stdout")
          If UBound(aOut) >= 0 Then
            sUser = aOut(0)
          End If
        End If
        If sUser = "" Then
          STDERR.WriteLine "User name is not specified"
          bUsage = True
        End If
      End If
      If sGroup <> "" Then
        STDERR.WriteLine "Group name ignored for " & sFunction
      End If
    End If
    If bUsage Then
      USAGE
    ElseIf bElevate Then
      rtn = Elevate()
    ElseIf sFunction = "CHGRP" Then
      rtn = CHGRP(sPath,sUser,sGroup)
    ElseIf sFunction = "MKDIR" Then
      rtn = MKDIR(sPath,sUser,sGroup)
    ElseIf sFunction = "GROUPS" Then
      rtn = GROUPS()
    ElseIf sFunction = "ISOWNER" Then
      rtn = ISOWNER(sPath, sUser)
    ElseIf sFunction = "GETOWNER" Then
      rtn = GETOWNER(sPath)
    ElseIf sFunction = "ISNTFS" Then
      rtn = ISNTFS(sPath)
    Else
      PRINT "Invalid function, not caught earlier - BUG",True
    End If
  End If
  Dim n
  If rtn Then
    n = 0
  Else
    n = 1
  End If
  If Not IsNull(sTempFile) Then
    ' elevated process, write return code to temp file
    FinishTempFile sTempFile,n
  End If
  WScript.Quit(n)
End Sub

MAIN