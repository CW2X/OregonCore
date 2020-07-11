@ECHO OFF
@title MaNGOSRealm
CLS
ECHO Initializing Realm (Logon-Server)...
:1
start "MaNGOS Realm" /B /MIN /WAIT mangos-realm.exe -c mangosrealm.conf
if %errorlevel% == 0 goto end
goto 1
:end