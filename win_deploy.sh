#!/bin/bash

qmake=qmake.exe
make=mingw32-make.exe

v_launcher_exe="`pwd`/tools/launcher/__builds/release/launcher.exe"
v_launcher_lang_ru="`pwd`/tools/launcher/main_ru.qm"
v_launcher_lang_en="`pwd`/tools/launcher/main_en.qm"
v_modbussource="`pwd`/plugins/modbussource/__builds/release/modbussource.dll"
v_tnex="`pwd`/tnexcore/__builds/release/tnex.exe"

v_deploy_path="`pwd`/win_deploy/"

v_offset_line="================================================"

echo "$v_offset_line Deploy for Windows `pwd`"

# Create work dirs
createDir ()
{
    if [ ! -d "$1" ] 
    then
        mkdir "$1"
        echo "Create dir $1"
    fi
}

#Remove work dirs
deleteDir ()
{
    if [ -d "$1" ] 
    then
        rm -r "$1"
        echo "Delete dir $1"
    fi
}

if [ "$1" == "--clear" ]
then
    echo "$v_offset_line Clear win deploy"
    deleteDir "$v_deploy_path"
    exit 0
fi

createDir "$v_deploy_path"

# if ! cp $v_tnex  $v_deploy_path/
# then
#   exit 0
# fi 

cp $v_tnex  $v_deploy_path/
cp $v_modbussource $v_deploy_path/
cp $v_launcher_exe $v_deploy_path/
cp $v_launcher_lang_ru $v_deploy_path/
cp $v_launcher_lang_en $v_deploy_path/

cd "$v_deploy_path"

windeployqt.exe --compiler-runtime --no-translations --no-system-d3d-compiler --release --no-quick-import  ./tnex.exe
windeployqt.exe --compiler-runtime --no-translations --no-system-d3d-compiler --release --no-quick-import  ./modbussource.dll
windeployqt.exe --compiler-runtime --no-translations --no-system-d3d-compiler --release --no-quick-import  ./launcher.exe

createDir plugins
mv ./modbussource.dll ./plugins/
