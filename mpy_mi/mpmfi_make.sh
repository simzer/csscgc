#!/bin/bash
zcc +zx -lndos -lm -create-app _mpymi.c -o _mpymi.bin 2>&1 | perl zcc2gcc.pl
#snap2tzx -1 "Monty Python's" -2 "Marathon for Incontinents" -b 0 -l imarath -o imarath.tzx -$ loadscr.scr _mpymi.bin
fuse _mpymi.tap