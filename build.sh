#!/bin/bash
mkdir -p build
cd build
cmake ..
make
cd ..
./bin/ZzRenderer
rm -rf bin
