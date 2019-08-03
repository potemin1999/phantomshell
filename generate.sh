#!/usr/bin/env bash

cd src || exit
lex lexer.l
yacc -v -d parser.y
cd .. || exit