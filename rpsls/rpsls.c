#include "stdlib.h"
#include "strings.h"
#include <input.h>
#include "spectrum.h"
#include "screen_zx.h"

#define SCREEN_ADDR 0x4000
#define SCRATTR_ADDR 0x5800

char rowsink[24]   = {
      2,2,2,2,2,2,2,2,
      2,2,0,0,0,0,0,0,
      0,2,2,2,4,4,7,7};
char rowspaper[24] = {
      0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
      0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
      0x30,0x30,0x30,0x30,0x30,0x30,0x10,0x10};
char rowsbright[24] = {
      1,1,1,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,1,1,1,1};

/* 5 * x, y, w, h */
char signalpos[20] = {
    3,6,3,4,
    14,3,2,3,
    27,6,3,4,
    9,17,3,3,
    20,17,3,3
  };
char playerpos[40] = {
    1,7,2,2,
    12,3,2,2,
    25,7,2,3,
    7,18,2,2,
    18,18,3,2,
    6,6,2,3,
    16,3,2,2,
    29,7,2,3,
    12,18,2,2,
    23,18,3,2,
};

char colors[4] = { (0x40|0x30|INK_RED) , (0x40|0x30|INK_GREEN) ,
                   (0x40|0x30|INK_MAGENTA) , (0x40|0x30|INK_YELLOW) };


char fresult[25] = {
// L  R  P  Sp S
   0, 1,-1,-1, 1, // L
  -1, 0, 1, 1,-1, // R
   1,-1, 0,-1, 1, // P
   1,-1, 1, 0, 1, // Sp
  -1, 1,-1,-1, 0  // S
};

char textresult[25] = {
// L  R  P  Sp S
  10, 2, 6, 3, 5, // L
   2,10, 1, 8, 9, // R
   6, 1,10, 7, 0, // P
   3, 8, 7,10, 4, // Sp
   5, 9, 0, 4,10, // S
};


// 0L 1R 2P 3Sp 4S
char *results[11] =
{
    "      Scrissors cuts Paper      ", // 0: 0-4
    "        Paper covers Rock       ", // 1: 1-2
    "       Rock crushes Lizard      ", // 2: 0-1
    "      Lizard poisons Spock      ", // 3: 0-3
    "     Spock smashes Scrissors    ", // 4: 3-4
    "  Scrissors decapitates Lizard  ", // 5: 0-4
    "       Lizard  eats Paper       ", // 6: 0-2
    "     Paper disproves Spock      ", // 7: 2-3
    "      Spock vaporizes Rock      ", // 8: 1-3
    "     Rock crushes Scrissors     ", // 9: 1-4
    "             Equal              "  // 10: x-x
};

void signhighlight(char sign, char gamer);

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
    charaddr = (char *)(addr + (string[i] - 32) * 8);
    for (j = 0; j < 8; j++)
    {
      posaddr[j * 8 * 32] = charaddr[j];
    }
  }
}

void colorblock(char x, char y, char color, char dx, char dy)
{
  char *pos;
  char i, j;
  for (i = 0; i < dx; i++)
    for (j = 0; j < dy; j++)
    {
      pos = (char *)(SCRATTR_ADDR+(y+j)*32+x+i); *pos = color;
    }
}

void letters(char player, char color)
{
  char i;
  for(i = 0; i < 5; i++)
  {
    colorblock(playerpos[(player-1)*20 + i*4+0],
               playerpos[(player-1)*20 + i*4+1],
               color,
               playerpos[(player-1)*20 + i*4+2],
               playerpos[(player-1)*20 + i*4+3]);
  }
}

void draw_bg(void)
{
  unsigned char j;
  unsigned int i;
  char *row_addr;
  char *tit_addr;

  zx_border(INK_YELLOW);

  for (j = 0; j < 192; j++)
  {
    row_addr = (char *)(zx_py2saddr(j));
    tit_addr = (char *)(screen + (4 + j) * 32);
    memcpy(row_addr, tit_addr, 32);
  }

  for (j = 0; j < 24; j++)
  {
    memset((char *)(SCRATTR_ADDR+j*32),
           rowspaper[j] | rowsink[j] | ((1-rowsbright[j])*BRIGHT),32);
  }
//  memset((char *)(SCRATTR_ADDR+22*32+16),0x30|3,16);
  print(0,22,"                                ");
  memset((char *)(SCRATTR_ADDR+21*32+16),0x30|3,16);
  memset((char *)(SCRATTR_ADDR+20*32+16),0x30|3,16);

  colorblock(8,6,0x40|0x30|INK_BLACK,17,5);

  for (i = 0; i < 5; i++) { signhighlight(i,0); }
  letters(1, colors[3]);
  letters(2, colors[3]);
}

void signhighlight(char sign, char gamer)
{
  colorblock(signalpos[sign*4+0],
      signalpos[sign*4+1],
      colors[gamer],
      signalpos[sign*4+2],
      signalpos[sign*4+3]);
}

void inttostr(char num, char res[3])
{
  res[0] = '0' + (num / 10);
  res[1] = '0' + (num % 10);
  res[2] = '\0';
}

void main(void)
{
  char i;
  char key;
  char player = 0;
  char sign1 = -1;
  char sign2 = -1;
  char score1 = 0;
  char score2 = 0;
  char scoretext[3];

  draw_bg();
  while (1)
  {
    switch (player)
    {
      case 0:
      {
        print(0,23,"     Press 0 to start game!     ");
        letters(1, colors[3]);
        letters(2, colors[3]);
        if ((sign1 != -1) && (sign2 != -1))
        {
          print(0,22,(char *)(results[textresult[sign1*5 + sign2]]));
          switch(fresult[sign2*5+sign1]) {
            case  1: { score1++;
                       memset((char *)(SCRATTR_ADDR+22*32),0x20|0,32);
                       break; }
            case -1: { score2++;
                       memset((char *)(SCRATTR_ADDR+22*32),0x18|0,32);
                       break; }
          }
          inttostr(score1, scoretext);
          print(0,21,scoretext);
          inttostr(score2, scoretext);
          print(30,21,scoretext);
          signhighlight(sign1,1);
          signhighlight(sign2,2);
        }
        while(player == 0)
        {
          if((key = in_Inkey(), key))
          {
            player = 1;
            switch (key)
            {
              case '0' : break;
              default:   player = 0;
            }
          }
        }
        break;
      }
      case 1:
      {
        memset((char *)(SCRATTR_ADDR+22*32),0x10|7,32);
        print(0,22,"                                ");
        print(0,23," Player 1 turn. Select a symbol!");
        for (i = 0; i < 5; i++) { signhighlight(i,0); }
        letters(1, colors[1]);
        letters(2, colors[3]);
        while(player == 1)
        {
          if((key = in_Inkey(), key))
          {
            player = 2;
            switch (key)
            {
              case 'a' : sign1 = 0; break;
              case 'w' : sign1 = 1; break;
              case 'd' : sign1 = 2; break;
              case 'z' : sign1 = 3; break;
              case 'x' : sign1 = 4; break;
              default:   player = 1;
            }
          }
        }
        break;
      }
      case 2:
      {
        print(0,23," Player 2 turn. Select a symbol!");
        letters(1, colors[3]);
        letters(2, colors[2]);
        while(player == 2)
        {
          if((key = in_Inkey(), key))
          {
            player = 0;
            switch (key)
            {
              case 'h' : sign2 = 0; break;
              case 'u' : sign2 = 1; break;
              case 'k' : sign2 = 2; break;
              case 'n' : sign2 = 3; break;
              case 'm' : sign2 = 4; break;
              default:   player = 2;
            }
          }
        }
        break;
      }
    }
  }
}
