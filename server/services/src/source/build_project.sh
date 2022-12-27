#!/bin/sh

cmake -DSERVICE_DIR=/datas/services . -B build

cd build

make 

cd ..

#rm -rf build
