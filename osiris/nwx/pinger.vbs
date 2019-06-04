' ===========================================================================
'
'                            PUBLIC DOMAIN NOTICE
'               National Center for Biotechnology Information
'
'  This software/database is a "United States Government Work" under the
'  terms of the United States Copyright Act.  It was written as part of
'  the author's official duties as a United States Government employee and
'  thus cannot be copyrighted.  This software/database is freely available
'  to the public for use. The National Library of Medicine and the U.S.
'  Government have not placed any restriction on its use or reproduction.
'
'  Although all reasonable efforts have been taken to ensure the accuracy
'  and reliability of the software and data, the NLM and the U.S.
'  Government do not and cannot warrant the performance or results that
'  may be obtained by using this software or data. The NLM and the U.S.
'  Government disclaim all warranties, express or implied, including
'  warranties of performance, merchantability or fitness for any particular
'  purpose.
'
'  Please cite the author in any work or product based on this material.
'
'  OSIRIS is a desktop tool working on your computer with your own data.
'  Your sample profile data is processed on your computer and is not sent
'  over the internet.
'
'  For quality monitoring, OSIRIS sends some information about usage
'  statistics  back to NCBI.  This information is limited to use of the
'  tool, without any sample, profile or batch data that would reveal the
'  context of your analysis.  For more details and instructions on opting
'  out, see the Privacy Information section of the OSIRIS User's Guide.
'
' ===========================================================================
'
'  FileName: pinger.vbs
'  Author:   Douglas Hoffman
'

Function ParentProcessId
' adapted from CurrProcessId
' https://stackoverflow.com/questions/8296037/find-my-own-process-id-in-vbscript
    Dim oChldPrcs, oCols, lOut, oWMI, sScript, sSQL
    lOut = 0
    sScript = Wscript.ScriptName
    sSql = "Select * From Win32_Process WHERE CommandLine Like '%" & sScript & "%'"
    ' if more than one instance of this is running, this may retrieve the wrong
    ' ppid, but it doesn't matter because it is called from GETSESSION which
    ' will have a different time stamp than the other process's session ID
    Set oWMI    = GetObject(_
        "winmgmts:{impersonationLevel=impersonate}!\\.\root\cimv2")
    Set oChldPrcs = oWMI.ExecQuery(sSql,,32)
    For Each oCols In oChldPrcs
        lOut = oCols.ParentProcessId 'get parent
        Exit For
    Next
    Set oCols = Nothing
    Set oChldPrcs = Nothing
    ParentProcessId = CStr(lOut)
End Function

Function OSVERSION()
  Dim SystemSet, System, sTmp
  OSVERSION = ""
  Set SystemSet = GetObject("winmgmts:").InstancesOf ("Win32_OperatingSystem") 
  for each System in SystemSet 
     sTmp = System.Caption + " " + System.Version 
     If Len(OSVERSION) > 0 Then
       OSVERSION = OSVERSION + "; "
     End If
     OSVERSION = OSVERSION + sTmp
  next 
  Set System = Nothing
  Set SystemSet = Nothing
End Function

Function INT2(n)
  INT2 = RIGHT("00" + CStr(n),2)
End Function

Function GETSESSION(tTime, sPID)
  Dim sDay, sTime
  sDay = INT2(Year(tTime)) + INT2(Month(tTime)) + INT2(Day(tTime))
  sTime = INT2(Hour(tTime)) + INT2(Minute(tTime)) + INT2(Second(tTime))
  GETSESSION = sDay + "." + sTime + "." + sPID
End Function

Function URL_ENCODEQ(s, allowEq)
  Dim OKCHARS, append, nLen, c
  OKCHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789._-"
  nLen = Len(s)
  URL_ENCODEQ = ""
  append = ""
  For i = 1 to nLen
    c = Mid(s,i,1)
    If allowEq and c = "=" Then
      allowEq = False
      append = c
    ElseIf InStr(OKCHARS,c) Then
      append = c
    Else
      append = "%" + Hex(Asc(c))
    End If
    URL_ENCODEQ = URL_ENCODEQ + append
  Next
End Function

Function ADD_PAIR(sList, sNew)
  Dim sTmp
  If Len(sNew) > 0 Then
    sTmp = URL_ENCODEQ(sNew, True)
    If InStr(sTmp,"=") = 0 Then
      sTmp = sTmp + "="
    End If
    If Len(sList) > 0 Then
      ADD_PAIR = sList + "&" + sTmp
    Else
      ADD_PAIR = sTmp
    End If
  Else
    ADD_PAIR = sList
  End If
End Function

Function PING(xHTTP, sBaseURL, sQuery, tStart)
  Dim tNow, sArg, sURL, sQ
  tNow = Date() + Time()
  sArg = "elapse=" + CStr(DateDiff("s",tStart,tNow))
  sQ = ADD_PAIR(sQuery, sArg)
  sURL = sBaseURL + "?" + sQ
  xHTTP.Open "GET", sURL
  xHTTP.Send
  PING = xHTTP.status
  Wscript.Echo "PING: " + sURL
  WScript.Echo "Status: " + CStr(xHTTP.status)
  WScript.Echo ""
End Function

Sub MAIN
  Dim objArgs, i, nArgCount, sURL, sAPP_VER, sAPP_NAME, sBASE_QUERY, sArg, tStart, sPID
  tStart = Date() + Time()
  sURL = "https://www.ncbi.nlm.nih.gov/stat"
  sAPP_VER = "v0"
  aAPP_NAME="ncbi pinger"
  sBASE_QUERY = ""
  sPID = ""
  Set objArgs = WScript.Arguments
  nArgCount = objArgs.Count - 1
  i = 0
  While i < nArgCount
    sArg = objArgs(i)
    if sArg = "-a" Then
      sBASE_QUERY = ADD_PAIR(sBASE_QUERY, "ncbi_app=" + objArgs(i+1))
    ElseIf sArg = "-v" Then
      sBASE_QUERY = ADD_PAIR(sBASE_QUERY, "app_ver=" + objArgs(i+1))
    ElseIf sArg = "-s" Then
      sBASE_QUERY = ADD_PAIR(sBASE_QUERY, objArgs(i+1))
    ElseIf sArg = "-p" Then
      sPID = objArgs(i+1)
    ElseIf sArg = "-u" Then
      sURL = objArgs(i+1)
    Else
      Wscript.StdErr.WriteLine sArg + ": invalid parameter, ignored"
      i = i - 1
    End If
    i = i + 2
  Wend
  If i = nArgCount Then
    '  i should be nArgCount + 1
    sArg = objArgs(i)
    Wscript.StdErr.WriteLine sArg + ": extra parameter, ignored"
  End If
  If Len(sPID) = 0 Then
    sPID = ParentProcessId()
  End If
  sBASE_QUERY = ADD_PAIR(sBASE_QUERY, "os=" + OSVERSION())
  sBASE_QUERY = ADD_PAIR(sBASE_QUERY, "session=" + GETSESSION(tStart, sPID))
  
  '  now do the pinging
  Dim sQuery, sLine, xHttp, tNow, nRtn, bDone, crash
  ' Msxml2.XMLHTTP.6.0 uses system proxy
  Set xHttp = CreateObject("Msxml2.XMLHTTP.6.0")
  sQuery = sBASE_QUERY
  crash = 1
  quit = 0
  nRtn = PING(xHttp, sURL, sQuery, tStart)
  do while nRtn = 200 and Not WScript.StdIn.AtEndOfStream and quit = 0
    sLine = Wscript.StdIn.Readline
    If sLine = "go" Then
      nRtn = PING(xHttp, sURL, sQuery, tStart)
      sQuery = sBASE_QUERY
    ElseIf sLine = "q" Then
      quit = 1
      crash = 0
      '  if calling program sends 'q' (quit) then it did not crash
    else
      sQuery = ADD_PAIR(sQuery, sLine)
    End If
  loop
  If nRtn = 200 Then
    sQuery = sBASE_QUERY
    sQuery = ADD_PAIR(sBASE_QUERY, "done=1")
    If crash = 1 Then
      ' calling program did not send 'q' to quit
      sQuery = ADD_PAIR(sBASE_QUERY, "crash=1")
    End If
    nRtn = PING(xHTTP, sURL, sQuery, tStart)
  End If
  
  If nRtn <> 200 Then
    Wscript.StdErr.WriteLine "Pinger failed: " + CStr(nRtn)
    nRtn = 1
  Else
    nRtn = 0
  End If
  Set xHttp = Nothing
  WScript.Quit(nRtn)
End Sub

MAIN