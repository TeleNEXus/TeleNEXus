#!/bin/bash

qmake=qmake.exe
make=mingw32-make.exe


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


if ! cp $v_tnex  $v_deploy_path/
then
  exit 0
fi 

if ! cp $v_modbussource $v_deploy_path/
then
  exit 0
fi 

cd "$v_deploy_path"

windeployqt.exe --compiler-runtime --no-translations --no-system-d3d-compiler --release --no-quick-import  ./tnex.exe
windeployqt.exe --compiler-runtime --no-translations --no-system-d3d-compiler --release --no-quick-import  ./modbussource.dll

createDir plugins
mv ./modbussource.dll ./plugins/
