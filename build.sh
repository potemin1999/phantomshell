#!/usr/bin/env bash

COMPILER="/usr/bin/gcc"
PWD="$(pwd)"
echo "Working directory ${PWD}"
./generate
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=${COMPILER} --build "${PWD}/cmake-build-debug" "${PWD}"
cmake --build "${PWD}/cmake-build-debug" --target psh -- -j 2