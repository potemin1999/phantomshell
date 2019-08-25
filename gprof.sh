#!/usr/bin/env bash

PWD="$(pwd)"
FOLDER="${PWD}/build/bin"
gprof "${FOLDER}/psh" "${FOLDER}/gmon.out" >> "${FOLDER}/gmon.txt"