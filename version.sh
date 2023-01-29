#!/bin/bash
baseversion="0.3"
echo "===============================Generate new build number"
oldnumber=$(cat ./buildcounter.cnt)
echo "Old build number:    $oldnumber"
newnumber=$(($oldnumber + 1))
echo "New build number:    $newnumber"
echo $newnumber > buildcounter.cnt
version="\"$baseversion.$newnumber\""
echo "New version:         $version"
echo "#define APPLICATION_VERSION $version" > tnexcore/version.h
echo "#define APPLICATION_VERSION $version" > plugins/modbussource/version.h
