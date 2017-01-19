rem 改为相应Qt安装目录
call C:/Qt/Qt5.7.0/5.7/msvc2015_64/bin/qtenv2.bat
rem -o后为目标输出路径
qmake -tp vc  -o D:\Hisign\PU\trunk\PU-AFIS-10\trunk\CLIENT\C\CN\trunk\bin\VSProject\ -recursive D:\Hisign\PU\trunk\PU-AFIS-10\trunk\CLIENT\C\CN\trunk\src\hisign.pro
pause