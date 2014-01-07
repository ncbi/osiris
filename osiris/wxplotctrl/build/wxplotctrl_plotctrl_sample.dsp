# Microsoft Developer Studio Project File - Name="wxplotctrl_plotctrl_sample" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=plotctrl_sample - Win32 Release Multilib
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxplotctrl_plotctrl_sample.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxplotctrl_plotctrl_sample.mak" CFG="plotctrl_sample - Win32 Release Multilib"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plotctrl_sample - Win32 DLL Unicode Debug Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 DLL Unicode Debug Multilib" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 DLL Unicode Release Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 DLL Unicode Release Multilib" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 DLL Debug Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 DLL Debug Multilib" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 DLL Release Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 DLL Release Multilib" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 Unicode Debug Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 Unicode Debug Multilib" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 Unicode Release Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 Unicode Release Multilib" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 Debug Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 Debug Multilib" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 Release Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "plotctrl_sample - Win32 Release Multilib" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "plotctrl_sample - Win32 DLL Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrlud.lib wxthingsud.lib wxmsw26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrlud.lib wxthingsud.lib wxmsw26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 DLL Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrlud.lib wxthingsud.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrlud.lib wxthingsud.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 DLL Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrlu.lib wxthingsu.lib wxmsw26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrlu.lib wxthingsu.lib wxmsw26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 DLL Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrlu.lib wxthingsu.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrlu.lib wxthingsu.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 DLL Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrld.lib wxthingsd.lib wxmsw26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrld.lib wxthingsd.lib wxmsw26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 DLL Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrld.lib wxthingsd.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrld.lib wxthingsd.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 DLL Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrl.lib wxthings.lib wxmsw26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrl.lib wxthings.lib wxmsw26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 DLL Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrl.lib wxthings.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrl.lib wxthings.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrlud.lib wxthingsud.lib wxmsw26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrlud.lib wxthingsud.lib wxmsw26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrlud.lib wxthingsud.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrlud.lib wxthingsud.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrlu.lib wxthingsu.lib wxmsw26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrlu.lib wxthingsu.lib wxmsw26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrlu.lib wxthingsu.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrlu.lib wxthingsu.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrld.lib wxthingsd.lib wxmsw26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrld.lib wxthingsd.lib wxmsw26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrld.lib wxthingsd.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrld.lib wxthingsd.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /debug /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrl.lib wxthings.lib wxmsw26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrl.lib wxthings.lib wxmsw26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"

!ELSEIF  "$(CFG)" == "plotctrl_sample - Win32 Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\plotctrl"
# PROP BASE Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\plotctrl"
# PROP Intermediate_Dir "msvc6prj\plotctrl_sample"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\samples\plotctrl\plotctrl_sample.pdb /I "$(WXCODE)\components\wxthings\include" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_WINDOWS" /i "..\samples\plotctrl" /i "..\samples\plotctrl" /i $(WXCODE)\components\wxthings\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxplotctrl.lib wxthings.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"
# ADD LINK32 ..\lib\wxplotctrl.lib wxthings.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\plotctrl\plotctrl_sample.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /subsystem:windows /libpath:"$(WXCODE)\components\wxthings\lib"

!ENDIF

# Begin Target

# Name "plotctrl_sample - Win32 DLL Unicode Debug Monolithic"
# Name "plotctrl_sample - Win32 DLL Unicode Debug Multilib"
# Name "plotctrl_sample - Win32 DLL Unicode Release Monolithic"
# Name "plotctrl_sample - Win32 DLL Unicode Release Multilib"
# Name "plotctrl_sample - Win32 DLL Debug Monolithic"
# Name "plotctrl_sample - Win32 DLL Debug Multilib"
# Name "plotctrl_sample - Win32 DLL Release Monolithic"
# Name "plotctrl_sample - Win32 DLL Release Multilib"
# Name "plotctrl_sample - Win32 Unicode Debug Monolithic"
# Name "plotctrl_sample - Win32 Unicode Debug Multilib"
# Name "plotctrl_sample - Win32 Unicode Release Monolithic"
# Name "plotctrl_sample - Win32 Unicode Release Multilib"
# Name "plotctrl_sample - Win32 Debug Monolithic"
# Name "plotctrl_sample - Win32 Debug Multilib"
# Name "plotctrl_sample - Win32 Release Monolithic"
# Name "plotctrl_sample - Win32 Release Multilib"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\samples\plotctrl\wxplotctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\..\samples\plotctrl\wxplotctrl.rc
# End Source File
# End Group
# End Target
# End Project

