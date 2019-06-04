@echo off
cd %~d0%~p0
cd Volumes
if ERRORLEVEL 1 GOTO ERROR
mode con cols=133 lines=40
cls
findstr /S /C:"<VolumeName>" *LabSettings.xml 
cd ..
GOTO DONE
:ERROR
cls
@echo.
@echo.
@echo Cannot find the Lab Settings directory.
cd Config
if ERRORLEVEL 1 GOTO DONE
cd ..
@echo.
@echo.
@echo From the OSIRIS menu bar select "Tools",
@echo then select "Show site settings folder..."
@echo When the window appears, double click on the
@echo highlighted directory.  Copy this file, %~nx0 
@echo to that folder and run it again.
@echo.
@echo.
:DONE
pause
