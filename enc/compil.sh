#!/usr/bin/env bash

nasm -f elf64 enc.s -o enc.o
nasm -f elf64 dec.s -o dec.o
gcc main.c enc.o dec.o -o test
