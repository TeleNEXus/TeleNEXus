#!/bin/bash

qmake=qmake.exe
make=mingw32-make.exe

build.sh $1

#qmake=qmake.exe
#make=mingw32-make.exe

#v_launcher="`pwd`/tools/launcher"
#v_tnexcore="`pwd`/tnexcore"
#v_modbussource="`pwd`/plugins/modbussource"
#v_examplewidgets="`pwd`/examples/plugins/widgets/plugins"

#v_build_dir_name=__builds

#v_offset_line="================================================"

#echo "$v_offset_line test build `pwd`"

## Create work dirs
#createBuildDir ()
#{
#  if [ ! -d "$1/$v_build_dir_name" ] 
#  then
#    mkdir "$1/$v_build_dir_name"
#    echo "Create dir $1/$v_build_dir_name"
#  fi
#}

##Remove work dirs
#deleteBuildDir ()
#{
#  if [ -d "$1/$v_build_dir_name" ] 
#  then
#    rm -r "$1/$v_build_dir_name/"
#    echo "Delete dir $1/$v_build_dir_name"
#  fi
#}

## compil 
#makePrg ()
#{

#  cd "$1/$v_build_dir_name"
#  if [ "$2" == $qmake ]
#  then
#    $qmake ../
#  else
#    if [ "$2" == $make ]
#    then
#      $make $3 
#    fi
#  fi
#}


#if [ "$1" == "--clear" ]
#then
#  echo "$v_offset_line Clear builds"
#  deleteBuildDir $v_launcher
#  deleteBuildDir $v_modbussource
#  deleteBuildDir $v_tnexcore
#  deleteBuildDir $v_examplewidgets
#  exit 0
#fi

#createBuildDir $v_launcher
#createBuildDir $v_modbussource
#createBuildDir $v_tnexcore
#createBuildDir $v_examplewidgets

## ./version.sh

#echo "$v_offset_line qmake"
#makePrg $v_launcher $qmake
#makePrg $v_modbussource $qmake
#makePrg $v_tnexcore $qmake
#makePrg $v_examplewidgets $qmake

#echo "$v_offset_line make $1"
#makePrg $v_launcher $make $1
#makePrg "$v_modbussource" $make $1
#makePrg $v_tnexcore $make $1
#makePrg $v_examplewidgets $make $1

