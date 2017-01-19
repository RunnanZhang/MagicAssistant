@set svn_bin=C:\Program Files\TortoiseSVN\bin
@set svn_work=D:\Hisign
D:

@echo off

cd "%svn_work%"
for /f "delims=" %%i in ('dir /adh/b/s/on .svn') do (
    echo %%i&&cd %%i&&cd ../
    "%svn_bin%"\TortoiseProc.exe/command:update /path:%%i\..\ /notempfile /closeonend:2
)
cd "%svn_work%"
pause