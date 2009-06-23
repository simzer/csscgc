#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <input.h>
#include <spectrum.h>
#include "charsprite.h"
#include "mapchars.h"
#include "walls12.h"
#include "font.h"

/*
 cells

 start, end points
   1
 2[ ]0
   3

 code = start << 8 | end
 e.g: o=> = 0x20

 0x02 0x32 0x30 0x13 0x10 0x12
 <--  <-+  +->  |    |      |
        |  |    V    +->  <-+

 0x20 0x23 0x03 0x31 0x01 0x21
 -->  --+  +--  ^    ^      ^
        V  V    |    +--  --+

 2x2 cells

 cell ids: 01
 (in [])   23

 0    1    2    3    4    5
 0x02 0x32 0x30 0x13 0x10 0x12
 <╗╔X <═╗   ╔╗   ╔╝   ╔╝  <╗║
  ╚╝   ╔╝   ║╚>  ╚╗   ╚═>  ╚╝
       X    X     V
 6    7    8    9    A    B
 0x20 0x23 0x03 0x31 0x01 0x21
  ╔╗   ╔╗   ╔═X  ╚╗   ║╔X  ╚╗
 X╝╚> X╝║   ╚╗   ╔╝   ╚╝  X═╝
        V    V   X
*/

/* 12x12 sprites of labirinth walls */
#define LABCELL(num,row) (labcells[(num)*12 + (row)])
//unsigned short int labcells[6 * 12] =
//{
//0xFFF,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0xFFF, /* ═ */
//0xFFF,0x001,0x001,0x001,0x001,0x001,0x001,0x001,0x001,0x001,0x001,0x801, /* ╗ */
//0xFFF,0x800,0x800,0x800,0x800,0x800,0x800,0x800,0x800,0x800,0x800,0x801, /* ╔ */
//0x801,0x801,0x801,0x801,0x801,0x801,0x801,0x801,0x801,0x801,0x801,0x801, /* ║ */
//0x801,0x800,0x800,0x800,0x800,0x800,0x800,0x800,0x800,0x800,0x800,0xFFF, /* ╚ */
//0x801,0x001,0x001,0x001,0x001,0x001,0x001,0x001,0x001,0x001,0x001,0xFFF, /* ╝ */
//};

unsigned char cells[12] =
{// (0)=1 (1)=2 (2)=4 (3)=8
    /* 0: 0x02 */0x14, /* '═', 1: 0x32 */0x84, /* '╗', 2: 0x30 */0x81, /* '╔',*/
    /* 3: 0x13 */0x28, /* '║', 4: 0x10 */0x21, /* '╚', 5: 0x12 */0x24, /* '╝',*/
    /* 6: 0x20 */0x41, /* '═', 7: 0x23 */0x48, /* '╗', 8: 0x03 */0x18, /* '╔',*/
    /* 9: 0x31 */0x82, /* '║', A: 0x01 */0x12, /* '╚', B: 0x21 */0x42, /* '╝' */
};

#define CELLS2X2(i,j) (cells2x2[(i)*4 + (j)])
unsigned char cells2x2[12 * 4] =
{
    /* 0x32 */ 1,/* 0x03 */ 8,/* 0x01 */ 0x0A,/* 0x12 */ 5,
    /* 0x02 */ 0,/* 0x32 */ 1,/* 0x30 */ 2,   /* 0x21 */ 0x0B,
    /* 0x30 */ 2,/* 0x23 */ 7,/* 0x31 */ 9,   /* 0x10 */ 4,
    /* 0x03 */ 8,/* 0x12 */ 5,/* 0x10 */ 4,   /* 0x23 */ 7,
    /* 0x03 */ 8,/* 0x12 */ 5,/* 0x10 */ 4,   /* 0x20 */ 6,
    /* 0x32 */ 1,/* 0x13 */ 3,/* 0x01 */ 0x0A,/* 0x12 */ 5,

    /* 0x30 */ 2,   /* 0x23 */ 7,/* 0x21 */ 0x0B,/* 0x10 */ 4,
    /* 0x30 */ 2,   /* 0x23 */ 7,/* 0x21 */ 0x0B,/* 0x13 */ 3,
    /* 0x03 */ 8,   /* 0x02 */ 0,/* 0x10 */ 4,   /* 0x23 */ 7,
    /* 0x01 */ 0x0A,/* 0x32 */ 1,/* 0x30 */ 2,   /* 0x21 */ 0x0B,
    /* 0x31 */ 9,   /* 0x03 */ 8,/* 0x01 */ 0x0A,/* 0x12 */ 5,
    /* 0x01 */ 0x0A,/* 0x32 */ 1,/* 0x20 */ 6,   /* 0x21 */ 0x0B
};

struct in_UDK keyboard_joy;

unsigned char debug_ghost_mode = 0;


void *joyfunc = in_JoyKeyboard;

void draw_sprite(unsigned char x,
                 unsigned char y,
                 unsigned short int *sprite,
                 unsigned char mode);

void draw_map(unsigned char map[], unsigned char size);

void draw_minimap(unsigned char map[], unsigned char size);

void iter_map(unsigned char *map,
              unsigned char *psize,
              signed char iter);

void draw_bar(unsigned int level,
              unsigned int score,
              unsigned char mode);

void print(unsigned char x,
           unsigned char y,
           char* string);

void draw_sprite_xz(unsigned char x,
                    unsigned char y,
                    unsigned short int *sprite)
{
  unsigned char *lineaddr;
  unsigned char *pixaddr;
  unsigned short int n, cell;
  unsigned short int bg, mask, res;

  for (n = 0; n < 12; n++)
  {
    lineaddr = zx_py2saddr(y + n);

    pixaddr = (unsigned short int *)(lineaddr + (x / 8));
    cell = (sprite[n] << 4) & 0xFFF0;

    *(pixaddr+0) = *(pixaddr+0) ^ (unsigned char)(cell >> ((x % 8) + 8));
    *(pixaddr+1) = *(pixaddr+1) ^ (unsigned char)(cell >> (x % 8));
    *(pixaddr+2) = *(pixaddr+2) ^ (unsigned char)(cell << (8 - (x % 8)));
  }
}

void draw_sprite(unsigned char x,
                 unsigned char y,
                 unsigned short int *sprite,
                 unsigned char mode)
{
  unsigned char *lineaddr;
  unsigned char *pixaddr;
  unsigned short int n, cell;
  unsigned short int bg, mask, res;

  for (n = 0; n < 12; n++)
  {
    lineaddr = zx_py2saddr(y * 12 + n);

    pixaddr = (unsigned short int *)(lineaddr + (x * 3 / 2));
    cell = sprite[n];

    bg = *(pixaddr+1) | (*(pixaddr) << 8);

    if ((x % 2) == 0)
    {
      mask = 0xFFF0;
      cell = (cell << 4);
    }
    else
    {
      mask = 0x0FFF;
    }

    if  (mode == 1)
    {
      cell = cell ^ bg;
    }

    res  = (bg & (~mask)) | (cell & mask);
    *(pixaddr)   = (unsigned char)(res >> 8);
    *(pixaddr+1) = (unsigned char)res;
  }
}

void draw_bar(unsigned int level,
              unsigned int score,
              unsigned char mode)
{
           char s[7];
           char i = 0;
  unsigned char *screenaddr;
  unsigned char color;

  if (mode == 0)
  {
    screenaddr = 16384;
    memset(screenaddr, 0x00, 32 * 24 * 8);

    color = PAPER_GREEN;
    screenaddr = 16384 + 32*24*8;
    memset(screenaddr,           color | INK_RED , 32 * 2);
    memset(screenaddr + 32 * 2,  color | INK_BLACK, 32 * 22);

    /*              5678901 */
    print(25,i++,  " MONO- ");
    print(25,i++,  "birinth");
    print(25,i++,  "CSSCGC");
    print(25,i++,  " 2009");
    print(25,i++,  " ");
    print(25,i++,  "keys:");
    print(25,i++,  "left  O");
    print(25,i++,  "right P");
    print(25,i++, "up    Q");
    print(25,i++, "down  A");
    print(25,i++,  " ");
    print(25,i++, "Level:");
    sprintf(s, "%d", level);
    print(25,i  , s);  print(27,i++, "/64");
    print(25,i++, "Score");
    sprintf(s, "%d", score);
    print(25,i++, s);
//    print(25,i++,  "_______");
//    print(25,i++, "Made by");
//    print(25,i++, " Simzer");
  }
  else
  {
    sprintf(s, "%d", level);
    print(25,12, s);  print(27,12, "/64");
    sprintf(s, "%d", score);
    print(25,14, s);
  }
}


void draw_map(unsigned char map[], unsigned char size)
{
  unsigned short int i, j, labcell;
  unsigned char color;

  for (j = 0; j < 24; j++)
    for (i = 0; i < 24; i++)
      *(zx_cyx2aaddr(j,i)) = 0;

  for (j = 0; j < size; j++)
  {
    for (i = 0; i < size; i++)
    {
      labcell = map[j * size + i]  % 6;
      draw_sprite(i,j,&(labcells[labcell * 12]),0);
    }

    color = PAPER_GREEN | INK_BLACK | BRIGHT;
    for (i = 0; i < 24; i++) {
      *(zx_cyx2aaddr(j * 3 / 2,i)) = color;
      *(zx_cyx2aaddr(j * 3 / 2 + 1,i)) = color;
    }
  }
}

void refresh_minimap(unsigned char pi,
                     unsigned char pj)
{
  unsigned short int i, j;

  unsigned char *addr;

  for (j = 0; j < 8; j++)
  {
    for (i = 0; i < 8; i++)
    {
      addr = (unsigned short int *)
             (zx_cyx2aaddr(16+j,24+i));

      if   ((i == pi) && (j == pj))
           {
             if (debug_ghost_mode)
                  { *(addr) = BRIGHT | FLASH | PAPER_RED | INK_GREEN;}
             else { *(addr) = FLASH | PAPER_RED | INK_GREEN;}
           }
      else { *(addr) = 0x14;/*0x39;*/ }
      //*(addr) = 0x02;
    }
  }
}

void draw_minimap(unsigned char map[], unsigned char size)
{
  unsigned short int i, j, labcell;

  unsigned char *pixaddr;
  unsigned short int n, cell;

  for (j = 0; j < size; j++)
  {
    for (i = 0; i < size; i++)
    {
      labcell = map[j * size + i];

      for (n = 0; n < 8; n++)
      {
        pixaddr = (unsigned short int *)
                  (zx_py2saddr((16 + j) * 8 + n) + 24 + i);

        cell = wall[labcell * 8 + n];

        *(pixaddr) = cell;
      }
    }
  }
}

void iter_map(unsigned char *map,
              unsigned char *psize,
              signed char iter)
{
  unsigned char i, j;

  //unsigned char *doublemap;
  unsigned char doublemap[16 * 16];

  //doublemap = (char *)malloc((*psize * 2) * (*psize * 2));

  if (iter >= 0)
  {
    for (j = 0; j < *psize; j++)
    {
      for (i = 0; i < *psize; i++)
      {
        doublemap[(j*2) * (*psize * 2) + (i*2)]     = CELLS2X2(map[j * (*psize) + i],0);
        doublemap[(j*2) * (*psize * 2) + (i*2+1)]   = CELLS2X2(map[j * (*psize) + i],1);
        doublemap[(j*2+1) * (*psize * 2) + (i*2)]   = CELLS2X2(map[j * (*psize) + i],2);
        doublemap[(j*2+1) * (*psize * 2) + (i*2+1)] = CELLS2X2(map[j * (*psize) + i],3);
      }
    }

    iter--;
    *psize = *psize * 2;

    for (j = 0; j < *psize; j++)
    {
      for (i = 0; i < *psize; i++)
      {
        map[j * (*psize) + i] = doublemap[j * (*psize) + i];
      }
    }

    iter_map(map, psize, iter);

    return;
  }
  else
  {
    return;
  }
}


void print(unsigned char x,
           unsigned char y,
           char* string)
{
  unsigned char *charaddr;
  unsigned char *posaddr;
  unsigned char i, j;

  unsigned char* addr = (char *)(0x3D00);

//  unsigned char* addr = (char *)font;

  for (i = 0; string[i] != '\0'; i++)
  {
    posaddr = zx_cyx2saddr(y,x+i);
    if ( ((string[i]>='A') &&(string[i]<='Z'))
        || ((string[i]>='a') &&(string[i]<='z')))
    {
      charaddr = (char *)(font + (string[i] - 0x41) * 8);
    }
    else
    {
      charaddr = (char *)(addr + (string[i] - 32) * 8);
    }
    for (j = 0; j < 8; j++)
    {
      posaddr[j * 8 * 32] = charaddr[j];
    }
  }
}

int check_keyboard(void)
{
  unsigned char dirs, ret;

  dirs = (joyfunc)(&keyboard_joy);

  ret = 0;

  if (dirs & in_RIGHT) ret = 'p';
  if (dirs & in_UP)    ret = 'q';
  if (dirs & in_DOWN)  ret = 'a';
  if (dirs & in_LEFT)  ret = 'o';
  if (dirs & in_FIRE)  ret = 'g';

  return (ret);
}

int main(void)
{

  unsigned int movesprite[8];

  unsigned char i;
  unsigned char key;
  unsigned char dir = 0;
  unsigned char prev = 0;

    signed char iter = 3;

  unsigned char size = 1;
  unsigned char map[16*16];

  unsigned char levelsize = 1;
  unsigned char levelmap[8*8];

    signed long posx = 0;
    signed long posy = 0;
    signed long posxn, posyn = 0;

    signed long levposx = 0;
    signed long levposy = 0;
    signed long levposxn, levposyn = 0;

  unsigned int level = 1;
  unsigned int score = 0;

  zx_border(0x04);

  movesprite[0] = char0A;
  movesprite[1] = char1A;
  movesprite[2] = char2A;
  movesprite[3] = char3A;
  movesprite[4] = char0B;
  movesprite[5] = char1B;
  movesprite[6] = char2B;
  movesprite[7] = char3B;

  keyboard_joy.fire  = in_LookupKey('g');
  keyboard_joy.left  = in_LookupKey('o');
  keyboard_joy.right = in_LookupKey('p');
  keyboard_joy.up    = in_LookupKey('q');
  keyboard_joy.down  = in_LookupKey('a');



  draw_bar(level, score, 0);

  levelmap[0] = 0x00;
  levelsize = 1;
  iter_map(levelmap, &levelsize, 2);
  draw_minimap(levelmap, levelsize);
  refresh_minimap(levposx, levposy);

  map[0] = levelmap[0];
  iter_map(map, &size, iter);
  draw_map(map, size);

  draw_sprite_xz(posx * 12, posy * 12, movesprite[dir]);
  prev = dir;

  while (1)
  {
    key = check_keyboard();

    posxn = posx;
    posyn = posy;

    switch (key)
    {
      case 'p' :
        if ( debug_ghost_mode ||
             cells[map[posy * size + posx]] & (0x01 | 0x10)) { posxn++; }
        dir = 0;
      break;
      case 'q' :
        if ( debug_ghost_mode ||
             cells[map[posy * size + posx]] & (0x02 | 0x20)) { posyn--;}
        dir = 1;
      break;
      case 'o' :
        if ( debug_ghost_mode ||
             cells[map[posy * size + posx]] & (0x04 | 0x40)) { posxn--;}
        dir = 2;
      break;
      case 'a' :
        if ( debug_ghost_mode ||
             cells[map[posy * size + posx]] & (0x08 | 0x80)) { posyn++;}
        dir = 3;
      break;
      case 'g' :
        debug_ghost_mode = !debug_ghost_mode;
        refresh_minimap(levposx, levposy);
      break;
    };

    if (   (posyn < 0) || (posyn >= 16)
        || (posxn < 0) || (posxn >= 16) )
    {
      if (posyn < 0)   { posyn = posy = posyn + 16;}
      if (posyn >= 16) { posyn = posy = posyn - 16;}
      if (posxn < 0)   { posxn = posx = posxn + 16;}
      if (posxn >= 16) { posxn = posx = posxn - 16;}

      switch (dir)
      {
        case 0 : levposx++; break;
        case 1 : levposy--; break;
        case 2 : levposx--; break;
        case 3 : levposy++; break;
      };

      if (levposx < 0)
      {
        print(0,0, "WRONG ENTRANCE. RESTART THE GAMEOR NOT...");
        while(1) {}
      }
      if (levposx > 7)
      {
        print(0,0, "CONGRATULATION, YOU WIN.        WANT TO PLAY AGAIN?");
        while(1) {}
      }

      map[0] = levelmap[levposy * levelsize + levposx];
      size = 1;
      iter_map(map, &size, iter);
      draw_map(map, size);
      refresh_minimap(levposx, levposy);
      level++;

      draw_sprite_xz(posx * 12, posy * 12, movesprite[dir]);
      prev = dir;

    }

    if ((posx != posxn) || (posy != posyn))
    {
      for(i = 0; i < 12; i = i + 2)
      {
        draw_sprite_xz(posx * 12 + i * (posxn - posx),
                       posy * 12 + i * (posyn - posy),
                       movesprite[prev]);
        draw_sprite_xz(posx * 12 + (i+2) * (posxn - posx),
                       posy * 12 + (i+2) * (posyn - posy),
                       movesprite[dir]);
        prev = dir;
        if (dir < 4) { dir += 4; } else { dir -= 4; }
      }

      score++;
      draw_bar(level, score, 1);

      posx = posxn;
      posy = posyn;
    }
  }
}
