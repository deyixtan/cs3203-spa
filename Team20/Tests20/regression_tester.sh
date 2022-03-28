#!/bin/sh

cmake --build ../Code20/cmake-build-debug --target all
python3 regression_tester.py ../Code20/cmake-build-debug/src/autotester/autotester
