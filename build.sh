#!/bin/bash

BUILDDir=build

if [ ! -d "$BUILDDir" ]; then
  mkdir $BUILDDir
fi

cd $BUILDDir

cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ../
make -j8