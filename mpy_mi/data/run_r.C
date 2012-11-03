/* C source file created by SevenuP v1.20                                */
/* SevenuP (C) Copyright 2002-2006 by Jaime Tejedor Gomez, aka Metalbrain*/

/*
GRAPHIC DATA:
Pixel Size:      ( 16,  16)
Char Size:       (  2,   2)
Frames:             8
Sort Priorities: X char, Char line, Y char, Frame number
Data Outputted:  Gfx
*/

#define INDEX_16x16(sprite,frame,bitrow,bytecol) \
  (sprite)[(frame) * 32 + (bitrow) * 2 + (bytecol)]

unsigned char run_h[128] = {
// 0
  0,  0, 12,  0, 28,  0, 24,  0,
 56,  0, 48,  0, 48,  0,189,255,
 36,  0,108,  0, 72,  0, 76,  0,
206,  0,192,  0, 96,  0,255,255,
// 1
  0,  0,  1,128,  3,128,  3,  0,
  7,  0,  6,  0,  6,  0,246,255,
  7,  0,  5,128,  5,128, 31,  0,
  27,  0, 25,128,  8,  0,255,255,
// 2
  0,  0,  0, 96,  0,224,  0,192,
  1,192,  1,128,  1,128,253,191,
  1,192,  0,192,  7,192,  6,192,
  5,128,  1,128,  0,192,255,255,
// 3
  0,  0,  0, 24,  0, 56,  0, 48,
  0,112,  0, 96,  0, 96,255,111,
  0, 96,  0,112,  0,248,  0,240,
  0,224,  0,192,  0,224,255,255};
