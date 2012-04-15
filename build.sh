#!/bin/sh
gcc -Wno-write-strings -g -L. -lstdc++ cpu.c opcode.c util.c run.c -o run 
