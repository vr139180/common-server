#!/bin/sh

cmake -DSERVICE_DIR=/datas/services -DCMAKE_BUILD_TYPE=Release . -B build

cd build

make 

cd ..

rm -rf build
