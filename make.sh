#!/bin/bash
zcc +zx -lndos -lm -create-app $1.c -o $1.bin 2>&1 | perl ../zcc2gcc.pl
fuse $1.tap