#!/bin/bash
v_install_path="/opt/tnex/"
v_plugins_path="plugins/"

uninstall ()
{
  echo "Remove programm path \"$v_install_path\""
  rm -r $v_install_path;
  echo "Remove programm link from \"/usr/bin/\""
  rm /usr/bin/tnex
}

if [ "$1" == "--uninstall" ] 
then
  echo "Start uninstall TeleNEXus" ;
  uninstall;
  echo "End uninstall TeleNEXus" ;
  exit 0;
fi

echo "Start install TeleNEXus."

if [ -d "$v_install_path" ] 
then
  echo "Install path \"$v_install_path\" is exists, removing it.";
  rm -r "$v_install_path";
fi

echo "Create install path \"$v_install_path\"...";
mkdir "$v_install_path";

echo "Create plugins path \"$v_install_path$v_plugins_path\"."

mkdir "$v_install_path$v_plugins_path"

if [ ! -f "./tnexcore/__builds/tnex" ]
then
  echo "Can't find executable TeleNEXus file \"tnex\"";
  uninstall;
  exit -1;
fi

echo "Copy TeleNEXus executable file."
cp "./tnexcore/__builds/tnex" "$v_install_path"


echo "Create program link \"tnex\" in \"/usr/bin/\""
ln -s -f /opt/tnex/tnex /usr/bin/tnex

if [ ! -f "./plugins/modbussource/__builds/libmodbussource.so" ]
then
  echo "Can't find MODBUS plugin library file \"libmodbussource\"";
fi

echo "Copy MODBUS plugin library file."
cp "./plugins/modbussource/__builds/libmodbussource.so" "${v_install_path}/${v_plugins_path}/"


echo "Copy files of the TeleNEXus launcher."


if ! cp "./tools/launcher/main_en.qm" "$v_install_path"
then
  echo "Can't copy endglish language map file."
fi 

if ! cp "./tools/launcher/main_ru.qm" "$v_install_path"
then
  echo "Can't copy russian language map file."
fi 

if ! cp "./tools/launcher/__builds/launcher" "$v_install_path"
then
  echo "Can't copy executable TeleNEXus launcher file"
else 
  echo "Create program link \"tnexlauncher\" in \"/usr/bin/\""
  ln -s -f /opt/tnex/launcher /usr/bin/tnexlauncher
fi


echo "End install TeleNEXus."
