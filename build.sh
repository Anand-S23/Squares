#!/bin/bash

set -xe

name=output
file=src/squares.c

gcc -Wall -Werror -std=c11 -ggdb -o $name $file -lSDL2 -lSDL2main

