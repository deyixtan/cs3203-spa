#!/bin/sh

cmake --build ../Code20/cmake-build-debug --target all &> /dev/null
python3 regression_tester.py ../Code20/cmake-build-debug/src/autotester/autotester
rm -f default.profraw out.xml &> /dev/null
