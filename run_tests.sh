#!/usr/bin/env bash
./build.sh

cd build
cmake --build . --target check

