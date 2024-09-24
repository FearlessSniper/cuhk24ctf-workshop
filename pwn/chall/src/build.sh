#!/bin/bash
gcc -o chall -fno-stack-protector -no-pie -ffunction-sections chall.c -Wl,-Tlinker
