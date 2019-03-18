#!/usr/bin/env bash

nasm -g -f elf64 enc.s -o enc.o
nasm -g -f elf64 dec.s -o dec.o
gcc main.c enc.o dec.o -o test
