#!/bin/bash

set -e

cat langtest.txt | ./libcc > out.asm
nasm -f elf -l out.lst out.asm
gcc -m32 -o out out.o
