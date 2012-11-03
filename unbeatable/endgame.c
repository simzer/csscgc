#include <spectrum.h>
#include <input.h>
#include "endgame.h"

#define WHITEROOK 0
#define WHITEKING 1
#define BLACKKING 2
#define NONE      3

#define PIECENUM  4

char pieces[4]    = {1, 0, 0, 2};
char pastpositions[4];
char positions[4];
unsigned char keys[8] = { 'q','w','e','a','d','z','x','c'};
signed char  moves[8] = { -9, -8, -7, -1,  1,  7,  8,  9};

char boldfont[768];

int abs(int a) { return (a < 0 ? -a : a); }
int x(int pos) { return (pos % 8); }
int y(int pos) { return (pos / 8); }

void init(void)
{
  int i;
  for (i = 0; i < 4; i++) pastpositions[i] = -1;
  positions[0] = 3;
  positions[1] = 7;
  positions[2] = 8;
  positions[3] = 60;
}

int near(int pos1, int pos2)
{
  return((  (abs(x(pos1) - x(pos2)) <= 1)
          &&(abs(y(pos1) - y(pos2)) <= 1)) ? 1 : 0);
}

void init_boldfont(void)
{
  unsigned int i;
  unsigned char val;
  unsigned char* addr = (char *)0x3D00;

  for (i = 0; i < 768; i++)
  {
    val = addr[i];
    val |= (val << 1);
    boldfont[i] = val;
  }
}

void print(unsigned char x,
           unsigned char y,
           char* string
           )
{
  unsigned char *charaddr;
  unsigned char *posaddr;
  unsigned char i, j;

  for (i = 0; string[i] != '\0'; i++)
  {
    posaddr = zx_cyx2saddr(y,x+i);
    charaddr = (char *)(boldfont + (string[i] - 32) * 8);
    for (j = 0; j < 8; j++)
    {
      posaddr[j * 8 * 32] = charaddr[j];
    }
  }
}

void drawPiece(int i, int j, int piece)
{
  unsigned char *posaddr;
  int k, n;
  unsigned char attr = PAPER_WHITE | INK_BLACK | BRIGHT;
  int x = 8 + 2 * i;
  int y = 4 + 2 * j;
  int background = (i + j) % 2;
  int index = piece*2 + background;
  if ( (i < 0) || (i > 7) || (j < 0) || (j > 7) ) return;
  for (n = 0; n < 2; n++)
  {
    posaddr = zx_cyx2saddr(y + n,x);
    for (k = 0; k < 8; k++)
    {
      posaddr[k * 8 * 32]     = symbols[index*32 + n*16 + k*2];
      posaddr[k * 8 * 32 + 1] = symbols[index*32 + n*16 + k*2+1];
    }
    posaddr = (char *)(0x5800+((23-y-n)*32)+x);
    *posaddr = attr;
    *(posaddr+1) = attr;
  }
}

void drawBorder(void)
{
  unsigned char *posaddr;
  int i, j, k, l, n;
  for (i = -1; i <= 16; i++) for (j = -1; j <= 16; j++)
  {
    if ((i == -1) || (i == 16) || (j == -1) || (j == 16))
    {
      posaddr = zx_cyx2saddr(4 + j, 8 + i);
      k = (i == -1) ? 0 : ((i == 16) ? 2 : 1);
      l = (j == -1) ? 0 : ((j == 16) ? 2 : 1);
      for (n = 0; n < 8; n++) posaddr[n * 8 * 32] = border[(l * 3 + k)*8 + n];
    }
  }
}

void drawTable(void)
{
  int i, j;
  //            01234567890123456789012345678901
  print(0, 0,  "           UNBEATABLE          ");
  print(0, 1,  "   The Ultimate Chess Program  ");
  print(26, 10,                           "Move:");
  print(26, 12,                           "q w e");
  print(26, 13,                           "a   d");
  print(26, 14,                           "z x c");
  print(0,06,                           "Comp");
  print(0,05,                           "White");
  print(26,06,                           "User");
  print(26,05,                           "Black");
  print(26,18,                           "Your");
  print(26,19,                           "Turn!");
  print(0, 22, "by Laszlo Simon - CSS CGC 2011 ");
  drawBorder();
  for (i = 0; i < 8; i++) for (j = 0; j < 8; j++)
  {
    drawPiece(i, j, NONE);
  }
}

int inLine(int line, int pos)
{
  return( ((pos >= line * 8) && (pos <= line * 8 + 7)) ? 1 : 0);
}

int endOfGame(void)
{
  if (   inLine(7,positions[3])
      && (  (inLine(7,positions[2]) && inLine(6,positions[1]))
          ||(inLine(6,positions[2]) && inLine(7,positions[1])) )
     ){
    return(1);
  } else {
    return(0);
  }
}


void drawPieces(void)
{
  int i, x, y;
  for (i = 0; i < PIECENUM; i++)
  {
    if (pastpositions[i] != positions[i])
    {
      x = positions[i] % 8;
      y = positions[i] / 8;
      drawPiece(x, y, pieces[i]);
      x = pastpositions[i] % 8;
      y = pastpositions[i] / 8;
      drawPiece(x, y, NONE);
      pastpositions[i] = positions[i];
    }
  }
}

int keypresshandle(char ch)
{
  static int released = 1;
  int moved = 0;
  int i;
  for (i = 0; i < 8; i++)
  {
    if (ch == keys[i])
    {
      if(released)
      {
        int newpos = positions[3] + moves[i];
        if(   (abs((newpos/8) - (positions[3]/8)) <= 1)
           && (abs((newpos%8) - (positions[3]%8)) <= 1)
           && ((newpos/8) >= 0)
           && ((newpos/8) <= 7)
           && ((newpos%8) >= 0)
           && ((newpos%8) <= 7)
           && (y(newpos) != y(positions[2]))
           && (y(newpos) != y(positions[1]))
           && (x(newpos) != x(positions[2]))
           && (x(newpos) != x(positions[1])))
        {
          positions[3] = newpos;
          released = 0;
          return(1);
        }
      }
      return(0);
    }
  }
  released = 1;
  return(0);
}

void AIstep(void)
{
  int lower = (y(positions[2]) < y(positions[1])) ? 2 : 1;
  int higher = (y(positions[2]) < y(positions[1])) ? 1 : 2;

  if (near(positions[3],positions[higher]))
  {
    if (x(positions[higher]) < 4) { positions[higher] += 6; }
    else                         { positions[higher] -= 6; }
  }
  else if (near(positions[3],positions[lower]+16))
  {
    if (x(positions[lower]) < 4) { positions[lower] += 6; }
    else                         { positions[lower] -= 6; }
  }
  else
  {
    positions[lower] += 16;
  }
}

void main(void)
{
  char c;
  int i, x, y;
  init_boldfont();
  while(1)
  {
    init();
    drawTable();
    drawPieces();
    while (!endOfGame())
    {
      while(c = in_Inkey(), !keypresshandle(c));
      drawPieces();
      AIstep();
      drawPieces();
    }
    print(0, 22, "Checkmate! Press any key to ...");
    while(c = in_Inkey(), c == 0);
  }
}
