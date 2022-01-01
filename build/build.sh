#!/bin/bash

name=app
src=src/main.c

inc=(
    -I../libs
)

flags=(
    -std=gnu99 -ldl -lGL -lX11 -pthread -lXi -lm
)

gcc ${inc[*]} $src ${flags[*]} -o $name

