::@echo off

@set exepath="..\..\srdt_prj\build-srdt-Desktop_Qt_5_9_9_MinGW_32bit-Debug\debug"
::@set envpath="D:\Programs\Qt\5.9.9\mingw53_32\bin"

@xcopy /Y  %exepath%\srdt.exe
::@D:\Programs\Qt\5.9.9\mingw53_32\bin\windeployqt.exe --compiler-runtime --debug --force ./srdt.exe
::@windeployqt.exe --compiler-runtime --no-translations --no-system-d3d-compiler --debug  --no-quick-import  srdt.exe
@windeployqt.exe --no-translations --no-system-d3d-compiler --debug  --no-quick-import  srdt.exe
::@xcopy /Y  %envpath%\libgcc_s_dw2-1.dll     
::@xcopy /Y  %envpath%\libstdc++-6.dll        
::@xcopy /Y  %envpath%\libwinpthread-1.dll    

@pause


