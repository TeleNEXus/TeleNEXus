#!/bin/bash


# QMAKE=qmake
# MAKE=make

if [ -z ${QMAKE} ]; then
  QMAKE=qmake
fi

if [ -z ${MAKE} ]; then
  MAKE=make
fi



v_launcher="`pwd`/tools/launcher"
v_modbussource="`pwd`/plugins/modbussource"
v_tnexcore="`pwd`/tnexcore"
v_examplewidgets="`pwd`/examples/plugins/widgets/plugins"

v_build_dir_name=__builds

v_offset_line="================================================"

echo "$v_offset_line test build `pwd`"

# Create work dirs
createBuildDir ()
{
  if [ ! -d "$1/$v_build_dir_name" ] 
  then
    mkdir "$1/$v_build_dir_name"
    echo "Create dir $1/$v_build_dir_name"
  fi
}

#Remove work dirs
deleteBuildDir ()
{
  if [ -d "$1/$v_build_dir_name" ] 
  then
    rm -r "$1/$v_build_dir_name/"
    echo "Delete dir $1/$v_build_dir_name"
  fi
}

# compil 
MAKEPrg ()
{

  cd "$1/$v_build_dir_name"
  if [ "$2" == $QMAKE ]
  then
    $QMAKE ../
  else
    if [ "$2" == $MAKE ]
    then
      $MAKE $3 
    fi
  fi
}


if [ "$1" == "--clear" ]
then
  echo "$v_offset_line Clear builds"
  deleteBuildDir $v_launcher
  deleteBuildDir "$v_modbussource"
  deleteBuildDir "$v_tnexcore"
  deleteBuildDir "$v_examplewidgets"
  exit 0
fi

createBuildDir "$v_launcher"
createBuildDir "$v_modbussource"
createBuildDir "$v_tnexcore"
createBuildDir "$v_examplewidgets"

# ./version.sh

echo "$v_offset_line QMAKE"
MAKEPrg "$v_launcher" $QMAKE
MAKEPrg "$v_modbussource" $QMAKE
MAKEPrg "$v_tnexcore" $QMAKE
MAKEPrg "$v_examplewidgets" $QMAKE

echo "$v_offset_line MAKE $1"
MAKEPrg "$v_launcher" $MAKE $1
MAKEPrg "$v_modbussource" $MAKE $1
MAKEPrg "$v_tnexcore" $MAKE $1
MAKEPrg "$v_examplewidgets" $MAKE $1

