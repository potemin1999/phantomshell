#!/usr/bin/env bash

cd src/generated || exit
lex ../lexer.l
yacc -v -d ../parser.y
cd .. || exit
cd .. || exit