#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <input.h>
#include <spectrum.h>

unsigned char blocksprite[8] = { 1, 43, 85, 43, 85, 43, 85, 255};

char boldfont[768];
int score = 0;

#define LENGTH 22
char width = 2;
#define MAXWIDTH 8
#define OBJTYPENUM 8
char objects[OBJTYPENUM * 4 * 2] =
{ 
  0, 0,    1, 0,    0, 1,    0,-1, // .:.
  1, 0,    0, 0,    1, 1,    1,-1, // .:.
  0, 0,    0, 1,    0, 2,    0,-1, // ....
  1, 0,    1, 1,    1, 2,    1,-1, // ....
  0, 0,    0,-1,    0, 1,    1, 1, // ..:
  1, 0,    1,-1,    1, 1,    0, 1, // ..:
  0, 0,    0, 1,    1, 0,    1,-1, // .:'
  1, 0,    1, 1,    0, 0,    0,-1  // .:'
};
#define OBJECTS(object,block,coord) (objects[(8*(object) + 2*(block) + (coord))])

char gamespace[LENGTH*MAXWIDTH];
#define GAMESPACE(x,y) gamespace[(y)*MAXWIDTH + (x)]
char position[2];
char blocks[4*2]; /* 1 = 0.5 unit*/
#define BLOCKS(block,coord) (blocks[(2*(block) + (coord))])
#define C(i,n) ( position[(n)] + BLOCKS((i),(n)) )
char gameover = 0;
char attr;

#define BLOCKCOLOR 0x77
#define EMPTYCOLOR 0x3F
#define BORDERCOLOR 0x78


extern void print(unsigned char x,
           unsigned char y,
           char* string);

void drawBlock(char x,
               char y,
               char draw,
               char attr)
{
  unsigned char *posaddr;
  unsigned char j;

  posaddr = zx_cyx2saddr(23-y,x);
  for (j = 0; j < 8; j++)
  {
    posaddr[j * 8 * 32] = blocksprite[j] * draw;
  }
  posaddr = (char *)(0x5800+((23-y)*32)+x);
  *posaddr = attr;
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


void drawBG(void)
{
  char x, y;

  memset((char *)(0x4000),0x00,32 * 192);

  for(y = 0; y < 24; y++)
  {
    drawBlock(0, y, 1, BORDERCOLOR);
    drawBlock(width+1, y, 1, BORDERCOLOR);
  }
  for(x = 0; x < width+2; x++)
  {
    drawBlock(x, 0, 1, BORDERCOLOR);
  }

  for(y = LENGTH-1; y >= 0; y--)
   for(x = 0; x < width; x++)
    drawBlock(x+1,y+1,0, EMPTYCOLOR);

  print(12, 1,  "   DEFECTIVE   ");
  print(12, 2,  "      BY DESIGN");
  print(12, 3,  "           TETRIS");
  print(12, 5,  " == CSS CGC 2010 ==");
  print(12, 7,  "Controls:");
  print(12, 9,  "P     - move left");
  print(12, 8,  "O     - move right");
  print(12, 10, "Q     - rotate left");
  print(12, 11, "A     - rotate right");
  print(12, 12, "M     - drop");
  print(12, 13, "8     - nothing");
  print(12, 14, "        special");
  print(12, 15, "unplug");
  print(12, 16, "power - quit");
  print(12, 18, "Score:");
  print(12, 19, "       0");
  print(12, 22, "Created by L. Simon");

}


void drawGamespace(void)
{
  char x,y,i;
  static char prvblks[4*2];
  static char prvgs[LENGTH*MAXWIDTH];
  static char init = 1;

  for(i = 0; i < 4; i++)
  {
    if (init == 0)
    {
      drawBlock(prvblks[i*2],prvblks[i*2+1],0, EMPTYCOLOR);
    }
    prvblks[i*2] = C(i,0)+1;
    prvblks[i*2+1] = C(i,1)+1;
  }
  if (init == 1)     { init = 0; }

  for(i = 0; i < 4; i++)
  {
    drawBlock(prvblks[i*2],prvblks[i*2+1],1, attr);
  }

  for(y = LENGTH-1; y >= 0; y--)
  {
   for(x = 0; x < width; x++)
   {
     if (prvgs[y * MAXWIDTH + x] != GAMESPACE(x,y))
     {
       if (GAMESPACE(x,y) > 0)
       {
         drawBlock(x+1,y+1,1, GAMESPACE(x,y));
       }
       else
       {
         drawBlock(x+1,y+1,0, EMPTYCOLOR);
       }
     }
     prvgs[y * MAXWIDTH + x] = GAMESPACE(x,y);
   }
  }
}

char validposition(void)
{
 char i;
 char x, y;
 for (i = 0; i < 4; i++)
 {
   x = C(i,0);
   y = C(i,1);
   if ((x < 0) || (x > width-1) || (y < 0) || (y > LENGTH-1)) return 0;
   if (GAMESPACE(x,y) != 0) return 0;
 }
 return 1;
}

void renderobject(void)
{
 char i, x, y;
 for (i = 0; i < 4; i++) {
   x = C(i,0);
   y = C(i,1);
   GAMESPACE(x,y) = attr;
 }
}

void moveobject(char o)
{
 position[0] += o;
 if (!validposition()) { position[0] -= o; }
}

void newobject(void)
{
 char i, id;

 position[1] = LENGTH - 3;
 position[0] = width / 2 - 1;
 id=rand()%OBJTYPENUM;
 attr = 1 + rand() % 6;
 attr = BRIGHT | (attr << 3);
 for (i = 0; i < 4; i++)
 {
   BLOCKS(i,0) = OBJECTS(id,i,0);
   BLOCKS(i,1) = OBJECTS(id,i,1);
 }
 if (!validposition()) { gameover = 1; }
}

void rotate(char o)
{
 char tmp, i;
 for (i = 0; i < 4; i++)
 {
   tmp = BLOCKS(i,0);
   BLOCKS(i,0) = -1 * o * BLOCKS(i,1);
   BLOCKS(i,1) = +1 * o * tmp;
 }
 if (!validposition())
 {
   for (i = 0; i < 4; i++)
   {
     tmp = BLOCKS(i,0);
     BLOCKS(i,0)= +1 * o * BLOCKS(i,1);
     BLOCKS(i,1) = -1 * o * tmp;
   }
 }
}


void print(unsigned char x,
           unsigned char y,
           char* string
//           unsigned char num
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


void clearfulls()
{
 char x,y,y2,rf,cp;
 cp = 0;

 for(y = 0; y < LENGTH; y++)
 {
   rf = 1;
   for(x = 0; x < width; x++) { if(GAMESPACE(x,y)==0) {rf = 0;} }

   if (rf == 1)
   {
     for(y2 = y; y2 < LENGTH-1; y2++)
       for(x = 0; x < width; x++) { GAMESPACE(x,y2) = GAMESPACE(x,y2+1); }
     for(x = 0; x < width; x++) { GAMESPACE(x,LENGTH-1) = 0; }
     y--;
     score += 100;
   }
 }
}

void drop()
{
 position[1]--;
 if (!validposition())   {
   position[1]++; renderobject(); clearfulls(); newobject();
 }
}


int main(void)
{
 char c;
 char counter = 0;

 init_boldfont();
 newobject();

 drawBG();
 drawGamespace();

 while (1)
 {
   c = in_Inkey();

   if(gameover)
   {
     if (counter % 10 == 0)
       memset((char *)(0x5800),rand(),32 * 24);
     print(rand() % 32, rand() % 24, "     GAME OVER     ");
   }
   else
   {
     switch (c)
     {
     case 'o' : moveobject(-1); drawGamespace(); break;
     case 'p' : moveobject(1);  drawGamespace(); break;
     case 'm' : drop();   drawGamespace(); break;
     case 'q' : rotate(1);  drawGamespace(); break;
     case 'a' : rotate(-1); drawGamespace(); break;
     case '8' : width = 8;  drawBG();drawGamespace(); break;
     }
   }

   counter++;
   if (counter == 0) {
     drop();
     drawGamespace();
   }
 }
}
