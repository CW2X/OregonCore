@ECHO OFF
@title MaNGOSCore
CLS
ECHO Initializing Core (World-Server)...
:1
start "MaNGOS Core" /B /MIN /WAIT mangos-core.exe -c mangoscore.conf
if %errorlevel% == 0 goto end
goto 1
:end