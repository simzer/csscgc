#!/bin/bash
perl generate.pl
zcc +zx -lndos -lm -create-app main.c -o main.bin
cp main.tap argument.tap
fuse argument.tap
