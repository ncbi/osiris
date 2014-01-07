@echo off

rem  ***** SET OSTOP to the top path containing the osiris program
rem  ***** all but one of the following lines containing 
rem  ***** "set OSTOP" should be commented out with rem

set OSTOP=%~d0%~p0
rem set OSTOP=C:\Program Files (x86)\NCBI\Osiris
rem set OSTOP=%USERPROFILE%\Apps\Osiris

IF NOT EXIST %OSTOP%\OsirisAnalysis.exe GOTO NOTFOUND

GOTO USAGE

set MSGBOOK=MessageBookV4.0.xml
set SRC=%OSTOP%\Config\Volumes\ID\ID_%MSGBOOK%
set DEST=%OSTOP%\site\Volumes

for /d %%i in ( "%DEST%\V-*" ) do call :DOCOPY "%%i"
GOTO DONE

:DOCOPY
SET DESTDIR=%~1
SET DESTBASE=%~n1
echo ....
echo %SRC%
echo %DESTDIR%\%DESTBASE%_%MSGBOOK%
copy "%SRC%" "%DESTDIR%\%DESTBASE%_%MSGBOOK%"
if ERRORLEVEL 1 GOTO ERROR
GOTO END

:USAGE
echo This batch file copies the Message Book xml file to all user defined volumes
echo but is currently disabled.  To enable this feature, edit this batch file and
echo remove the line containing "GOTO USAGE."
GOTO DONE

:NOTFOUND
echo cannot find TOP level OSIRIS directory
GOTO DONE

:ERROR
pause
exit

:DONE
pause


:END
