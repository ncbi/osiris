cd %EXTLIB%\libxslt-1.1.33\win32
if ERRORLEVEL 1 GOTO ERR
cscript configure.js static=yes prefix=%EXTLIB% include=%EXTLIB%\libxml2-2.9.8\include;%EXTLIB%\libiconv-win-build-1.15\include lib=%EXTLIB%\libxml2-2.9.8\win32\bin.msvc;%EXTLIB%\libiconv-win-build-1.15\build-VS2017\Release static=yes zlib=no
if ERRORLEVEL 1 GOTO ERR
nmake /f Makefile.msvc
if ERRORLEVEL 1 GOTO ERR
GOTO END

:ERR
echo configuration FAILED
:END
cd %~d0%~p0
echo done