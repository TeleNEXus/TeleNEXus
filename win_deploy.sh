#!/bin/bash

# -----------------------------------
# Specify windows deploy utility
# -----------------------------------
WINDEPLOYQT=windeployqt.exe 

# -----------------------------------
# Specify the file paths to deploy.
# -----------------------------------

# TeleNEXus luncher
v_launcher_exe="`pwd`/tools/launcher/__builds/release/launcher.exe"

# TeleNEXus luncher translate files.
v_launcher_lang_ru="`pwd`/tools/launcher/main_ru.qm"
v_launcher_lang_en="`pwd`/tools/launcher/main_en.qm"

# Plugin Modbus.
v_modbussource="`pwd`/plugins/modbussource/__builds/release/modbussource.dll"

# TeleNEXus core application.
v_tnex="`pwd`/tnexcore/__builds/release/tnex.exe"

# -----------------------------------
# Specify deploy folder path. 
# -----------------------------------
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

# $WINDEPLOYQT --compiler-runtime --no-translations --no-system-d3d-compiler --release --no-quick-import  ./tnex.exe
# $WINDEPLOYQT --compiler-runtime --no-translations --no-system-d3d-compiler --release --no-quick-import  ./modbussource.dll
# $WINDEPLOYQT --compiler-runtime --no-translations --no-system-d3d-compiler --release --no-quick-import  ./launcher.exe

# $WINDEPLOYQT --compiler-runtime --no-translations --no-system-d3d-compiler --debug --no-quick-import  ./tnex.exe
# $WINDEPLOYQT --compiler-runtime --no-translations --no-system-d3d-compiler --debug --no-quick-import  ./modbussource.dll
# $WINDEPLOYQT --compiler-runtime --no-translations --no-system-d3d-compiler --debug --no-quick-import  ./launcher.exe

$WINDEPLOYQT --compiler-runtime --no-translations --no-system-d3d-compiler  --no-quick-import  ./tnex.exe
$WINDEPLOYQT --compiler-runtime --no-translations --no-system-d3d-compiler  --no-quick-import  ./modbussource.dll
$WINDEPLOYQT --compiler-runtime --no-translations --no-system-d3d-compiler  --no-quick-import  ./launcher.exe

# $WINDEPLOYQT  ./tnex.exe
# $WINDEPLOYQT  ./modbussource.dll
# $WINDEPLOYQT  ./launcher.exe

createDir plugins
mv ./modbussource.dll ./plugins/
mv ./launcher.exe ./tnexlauncher.exe

