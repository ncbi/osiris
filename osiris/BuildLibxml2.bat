cd %EXTLIB%\libiconv-win-build-1.15\build-VS2017\Release
if ERRORLEVEL 1 GOTO ERR
copy libiconv-static.lib iconv.lib
if ERRORLEVEL 1 GOTO ERR
cd %EXTLIB%\libxml2-2.9.8\win32
if ERRORLEVEL 1 GOTO ERR
cscript configure.js static=yes prefix=%EXTLIB% include=%EXTLIB%\libiconv-win-build-1.15\include lib=%EXTLIB%\libiconv-win-build-1.15\build-VS2017\Release static=yes ftp=no http=no zlib=no lzma=no
if ERRORLEVEL 1 GOTO ERR
nmake /f Makefile.msvc
if ERRORLEVEL 1 GOTO ERR
GOTO END

:ERR
echo configuration FAILED
:END
cd %~d0%~p0
echo done