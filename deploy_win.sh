#!/bin/bash

qmake=qmake.exe
make=mingw32-make.exe



v_modbussource_builds="`pwd`/plugins/modbussource/__builds/modbussource.dll"

v_tnex_builds="`pwd`/tnexcore/__builds/tnexcore.exe"


v_deploy_path="`pwd`/deploy_win/"


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


if ! cp $v_tnex_builds  $v_deploy_path/tnex.exe 
then
  exit 0
fi 

if ! cp $v_modbussource_builds $v_deploy_path/
then
  exit 0
fi 

cd "$v_deploy_path"

# /mnt/d/Programs/Qt5.14.0/5.14.0/mingw73_32/bin/windeployqt.exe -compiler-runtime ./
/mnt/d/Programs/Qt5.14.0/5.14.0/mingw73_32/bin/windeployqt.exe --compiler-runtime --no-translations --no-system-d3d-compiler --release --no-quick-import  ./tnex.exe
# /mnt/d/Programs/Qt5.14.0/5.14.0/mingw73_32/bin/windeployqt.exe --no-translations --no-system-d3d-compiler --release --no-quick-import  ./modbussource.dll
