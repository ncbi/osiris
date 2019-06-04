@echo off
if exist ..\..\..\digisign\sign-file.bat goto sign
goto end
:sign
call ..\..\..\digisign\sign-file.bat %~d0%~p0\Release\*.exe
:end
