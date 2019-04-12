
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