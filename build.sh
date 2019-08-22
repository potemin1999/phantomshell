#!/usr/bin/env bash

BUILD_TYPE="${1:-Debug}"
COMPILER="/usr/bin/gcc"
PWD="$(pwd)"
FOLDER="${PWD}/cmake-build-debug"
echo "Working directory ${PWD}"
./generate.sh
cmake -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" -DCMAKE_CXX_COMPILER=${COMPILER} --build "${FOLDER}" "${PWD}"
cmake --build "${FOLDER}" --target psh -- -j 2