@echo off
if exist %USERPROFILE%\Documents\digisign\sign-file.bat goto sign
echo Cannot find %USERPROFILE%\Documents\digisign\sign-file.bat
echo Not adding digital signature
goto end
:sign
call %USERPROFILE%\Documents\digisign\sign-file.bat %~d0%~p0\Release\*.exe
:end
