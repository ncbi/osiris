@echo off
cd %~d0%~p0
cd site\Volumes
if ERRORLEVEL 1 GOTO ERROR
mode con cols=133 lines=40
findstr /S /C:"<VolumeName>" *LabSettings.xml 
cd ..\..
GOTO DONE
:ERROR
@echo Cannot find volumes directory
:DONE
pause
