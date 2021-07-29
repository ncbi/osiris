@echo off
cls
set PATH={OSIRISEXEDIR};%PATH%
set OSIRISFILES={OSIRISFILES}
set OSIRISTOOLS=%OSIRISFILES%\Config\ConfigurationTools
cmd /K cd /d "%OSIRISTOOLS%"
