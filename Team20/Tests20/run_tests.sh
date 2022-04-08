#!/bin/sh

cmake --build ../Code20/cmake-build-debug --target all &> /dev/null

echo "Run Unit Tests"
../Code20/cmake-build-debug/src/unit_testing/unit_testing

echo "Run Integration Tests"
../Code20/cmake-build-debug/src/integration_testing/integration_testing

echo "Run Regression Tests"
echo "==============================================================================="
python3 regression_tester.py ../Code20/cmake-build-debug/src/autotester/autotester

rm -f default.profraw out.xml &> /dev/null
