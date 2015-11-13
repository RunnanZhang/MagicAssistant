
call C:/Qt/Qt5.4.1/5.4/msvc2013_opengl/bin/qtenv2.bat vsvars
C:
cd C:\Workspace\OrigineBeta\Builds\Win\OrigineSandBox
qmake -tp vc -recursive C:\Workspace\OrigineBeta\Sources\OrigineSandBox\OrigineSandBox.pro
pause