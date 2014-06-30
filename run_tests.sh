#!/usr/bin/env bash
. build.sh
cmake --build . --target check_libplf
cmake --build . --target check_libdis


