#include <strings.h>
#include <sound.h>
#include <input.h> 
#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>

#include "music.h"
#include "sprites.h"
#include "title.h"
#include "texts.h"

#define SCREEN_ADDR 0x4000
#define SCRATTR_ADDR 0x5800
#define TIERCE_SIZE 0x0800

#define FIRSTROW_YPOS 7
#define WC_YPOS 3
#define ROW_NUM 8
#define PISSDELTA 10

#define NUM_MOVE 6
#define ID_NOMOVE 0
#define ID_LEFT   1
#define ID_RIGHT  2
#define ID_DOWN   3
#define ID_UP     4
#define ID_PISS   5
#define ID_FORCEDPISS 6

#define NUM_PLAYERS 4

#define ID_X 0
#define ID_Y 1
#define ID_W 2
#define ID_H 3
#define ID_NUM 4

#define is_wc(wall_map,x_pos) ( (wall_map) & (0x01 << (x_pos)) )

char boldfont[768];

signed char moves[NUM_MOVE] = 
{ 
  0, /*no move*/
  -1, /* left */
  1, /* right */
  32, /* down */
  -32, /* up */  
  0  /* piss */
};

unsigned int move_sprites[NUM_MOVE];

#define LEVEL_NUM 2
#define LEVEL_LENGTH 5
#define TRACK_LENGHT 30

#define MENU_MAIN   0
#define MENU_README 1
#define MENU_GAME 10
#define MENU_GAME_OVER 11
#define MENU_GAME_END 11
#define MENU_GAME_WIN 11
unsigned char menu_state = MENU_MAIN;
unsigned char level = 1;
unsigned char music_on = 1;
unsigned char track[NUM_PLAYERS] = {0,0,0,0};
unsigned int tracks_level_1[LEVEL_LENGTH] = { 0x11, 0x16, 0x24, 0x00, 0x12};
unsigned int tracks_level_2[LEVEL_LENGTH] = { 0x11, 0x16, 0x24, 0x00, 0x12};
#define INIT_POS (FIRSTROW_YPOS * 32 + 1)
unsigned int pos[NUM_PLAYERS] = {INIT_POS,
                                   ((FIRSTROW_YPOS+2) * 32 + 1),
                                   ((FIRSTROW_YPOS+4) * 32 + 1),
                                   ((FIRSTROW_YPOS+6) * 32 + 1)};
signed char n = 0;
unsigned char move[NUM_PLAYERS] = {0,2,2,2};
  signed char piss_storage[NUM_PLAYERS] = {0,100,100,100};
unsigned char lives = 5;
unsigned int way = 0;
unsigned int blockmove = 0;
/* 
X     Y     W     H
*/
signed char move_coords[ID_NUM*NUM_MOVE] =
{
0,    0,    1,    2, // no move
-1,    0,    2,    2,// left 
0,    0,    2,    2, // right
0,    0,    1,    3, // down
0,   -1,    1,    3, // up
0,    0,    1,    2, // piss
};

struct in_UDK keyboard_joy;

void *joyfunc = in_JoyKeyboard;  

int music_play(void)
{
  static unsigned int i = 0;
  float duration;
  unsigned int dur_int;
  unsigned int ch = 0;
  unsigned char j;

//  while (1)
//  {
  /* Real frequency !  Duration is in s(72) */
  if (music_on)
  {
    duration = (float)tune[i+1] * 72 / (1000*30);
    dur_int = (int)(duration * 15000) / 20;
    for (j = 0; j < dur_int; j++)
    {
      bit_frequency(0.02, (float)tune[i] / 100);
      ch = in_Inkey();
      if (ch != 0) break;
    }
  
    i += 2;

    if (i >= 2*(138-32))
    {
      i = 0;
    }
  }
  else 
  {
    ch = in_Inkey();
  }
//  }
  return (ch);
}



void draw_title(void)
{
  unsigned char j;
  unsigned int i;
  char *row_addr;
  char *tit_addr;

  for (j = 0; j < 16; j++)
  {
    row_addr = (char *)( SCREEN_ADDR + ((j%8) << 8) + ((j >> 3) << 5) );
	tit_addr = (char *)(mi_title + j * 32);
	memcpy(row_addr, tit_addr, 32);
  }

  if (level == 1)
  {
    memset((char *)(SCRATTR_ADDR),0x38,32);
    memset((char *)(SCRATTR_ADDR+32),0x78,64);
  }
  if (level == 2)
  {
    memset((char *)(SCRATTR_ADDR),/*0x62*/ 0x60,32);
    memset((char *)(SCRATTR_ADDR+32),/*0x22*/0x20,32);
    memset((char *)(SCRATTR_ADDR+64),96,32);
  }
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

void draw_sprite(char *sprite,
                 unsigned char w,
                 unsigned char h,
                 unsigned char x,
                 unsigned char y,
                 unsigned char frame)
{
  unsigned char i, j, n;
  unsigned char *posaddr;

  for (n = 0; n < h; n++)
  {
    posaddr = zx_cyx2saddr(y+n,x);
    for (j = n*8; j < (n+1)*8; j++)
    {
      for (i = 0; i < w; i++)
      {
        *(posaddr+i) = INDEX_XxY(sprite,w,h,frame,j,i);
      }
      posaddr += 256;
    }
  }
}

void draw_walls(void)
{
  unsigned char i;
  unsigned int track_map;
  unsigned char iswc;
  static unsigned char prev_track_map = 0xFF;
  static unsigned char prev_level = 0xFF;

  if (level == 1) track_map = tracks_level_1[track[0]];
  if (level == 2) track_map = tracks_level_2[track[0]];

  if (level != prev_level)
  {
    if (level == 1) memset((char *)(SCRATTR_ADDR+3*32),0x78,2*32);
    if (level == 1) memset((char *)(SCRATTR_ADDR+5*32),0x68,1*32);
    if (level == 1) memset((char *)(SCRATTR_ADDR+6*32),0x3F,1*32);
    if (level == 1) memset((char *)(SCRATTR_ADDR+7*32),0x38,1*32);
      
    if (level == 2) memset((char *)(SCRATTR_ADDR+3*32),0x62,2*32);
    if (level == 2) memset((char *)(SCRATTR_ADDR+5*32),/*0x50*//*34*/98,32);
    if (level == 2) memset((char *)(SCRATTR_ADDR+6*32),18,1*32);
    if (level == 2) memset((char *)(SCRATTR_ADDR+7*32),16,1*32);
  }
    
  for (i = 0; i <= 9; i++)
  {
    iswc = (is_wc(track_map, i));
    if (   (prev_track_map == 0xFFFF)
        || (level != prev_level)     
        || (iswc != (is_wc(prev_track_map, i)))
        )
    { 
      if (iswc)
      {
        if (level == 1) 
        {
          draw_sprite(wc1, 3, 3, i*3+1, WC_YPOS, 0);
          *(char *)(SCRATTR_ADDR + (WC_YPOS+1) * 32 + i*3+2) = 0x38;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+2) * 32 + i*3+2) = 0x38;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+3) * 32 + i*3+2) = 0x30;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+4) * 32 + i*3+2) = 0x30;
        }
        if (level == 2) 
        {
          draw_sprite(wc2, 3, 3, i*3+1, WC_YPOS, 0);
          *(char *)(SCRATTR_ADDR + (WC_YPOS+1) * 32 + i*3+2) = 16;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+2) * 32 + i*3+2) = 16;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+3) * 32 + i*3+2) = 16;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+4) * 32 + i*3+2) = 16;
        }
      }
      else
      {
        if (level == 1) 
        {
          draw_sprite(wall1, 3, 3, i*3+1, WC_YPOS, 0);
          *(char *)(SCRATTR_ADDR + (WC_YPOS+1) * 32 + i*3+2) = 0x78;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+2) * 32 + i*3+2) = 0x68;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+3) * 32 + i*3+2) = 0x3F;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+4) * 32 + i*3+2) = 0x38;
        }
        if (level == 2) 
        {
          draw_sprite(wall2, 3, 3, i*3+1, WC_YPOS, 0);
          *(char *)(SCRATTR_ADDR + (WC_YPOS+1) * 32 + i*3+2) = 98;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+2) * 32 + i*3+2) = 98;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+3) * 32 + i*3+2) = 18;
          *(char *)(SCRATTR_ADDR + (WC_YPOS+4) * 32 + i*3+2) = 16;
        }
      }
    }
  }
  prev_track_map = track_map;
  prev_level = level;
}

void chartostr(char *str,unsigned char i)
{
  str[0] = 48 + i / 100;
  str[1] = 48 + (i / 10) % 10;
  str[2] = 48 + i % 10;
  str[3] = '\0';
}

void draw_states(void)
{
  char *temp;
  unsigned int distance, piss_indicator, distance_indicator;
  
  piss_indicator = (piss_storage[0] + 5) / 10;
  memset((char *)(SCRATTR_ADDR + 21*32),0x70,piss_indicator); 
  memset((char *)(SCRATTR_ADDR + 21*32 + piss_indicator),0x08,10 - piss_indicator); 
   
  distance_indicator = 15 * 
             (((level-1) * LEVEL_LENGTH + track[0]) * TRACK_LENGHT + (pos[0] % 32) - 1) /
             (LEVEL_NUM * LEVEL_LENGTH * TRACK_LENGHT);
  memset((char *)(SCRATTR_ADDR + 21*32 + 11),96,distance_indicator); 
  memset((char *)(SCRATTR_ADDR + 21*32 + 11 + distance_indicator),32,15 - distance_indicator); 
  
  memset((char *)(SCRATTR_ADDR + 21*32 + 27),70,lives);
  memset((char *)(SCRATTR_ADDR + 21*32 + 27 + lives),0x00,5-lives);
}

void draw_ui(void)
{   
  char i;

  print(0,19,"BLADDER      DISTANCE      LIVES");
  print(0,23,"INFO: prepare to the race!");
 
  draw_sprite(corners, 1, 1, 0, 17, 0);    
  draw_sprite(corners, 1, 1, 31, 17, 1); 
  draw_sprite(corners, 1, 1, 31, 2, 2);    
  draw_sprite(corners, 1, 1, 0, 2, 3);   
  draw_sprite(empty_field, 1, 1, 0, 5, 0);    
  draw_sprite(empty_field, 1, 1, 31, 5, 0);  

  draw_sprite(indicator_left, 1, 1, 0, 21, 0);    
  draw_sprite(indicator_left, 1, 1, 11, 21, 0);  

  draw_sprite(indicator_right, 1, 1, 9, 21, 0);    
  draw_sprite(indicator_right, 1, 1, 25, 21, 0);  
  
  for (i = 0; i < 5; i++)
  {
    draw_sprite(life, 1, 1, 27+i, 21, 0);  
  }

  memset((char *)(SCRATTR_ADDR+18*32),0x47,6*32);
  
  memset((char *)(SCRATTR_ADDR + 21*32),0x08,10); 
  memset((char *)(SCRATTR_ADDR + 21*32 + 11),32,15); 
  memset((char *)(SCRATTR_ADDR + 21*32 + 27),70,5);
}

void draw_field(void)
{
  signed char i;
  char* row_addr;

  for (i = -1; i <= ROW_NUM; i++)
  { 
    row_addr = (char *)(SCREEN_ADDR 
                         + ((FIRSTROW_YPOS + i) % 8) * 32
                         + (7 * 8 * 32)
                         + ((FIRSTROW_YPOS + i) / 8) * 2048 ); 
    memset(row_addr,0xFF,32);
  } 

  if (level == 1) 
  {
    memset((char *)(SCRATTR_ADDR+8*32),0x30,32*8);
    memset((char *)(SCRATTR_ADDR+16*32),0x38,2*32);
  }
  if (level == 2) 
  {
    memset((char *)(SCRATTR_ADDR+8*32),0x38,32*8);
    memset((char *)(SCRATTR_ADDR+16*32),/*0x62*/16,2*32);
  }
}

int check_keyboard(void)
{
  unsigned char dirs, ret;
  
  dirs = (joyfunc)(&keyboard_joy);
  
  ret = 0;

  if (dirs & in_RIGHT) ret = (ID_RIGHT);
  if (dirs & in_UP)    ret = (ID_UP);
  if (dirs & in_DOWN)  ret = (ID_DOWN);
  if (dirs & in_LEFT)  ret = (ID_LEFT);
  if (dirs & in_FIRE)  ret = (ID_PISS);
  
  return (ret);
}


unsigned char validate(unsigned int pos, 
                         unsigned char move)
{
  unsigned int newpos;
  unsigned char valid = 1;
  unsigned int x_pos, y_pos, x_wall_pos, y_min_pos;
  unsigned int track_map;

  if (level == 1) track_map = tracks_level_1[track[0]];
  if (level == 2) track_map = tracks_level_2[track[0]];
    
  newpos = pos;
  newpos += moves[move];
  
  x_pos = newpos % 32;
  y_pos = newpos / 32;
  
  x_wall_pos = (x_pos - 1)/3;
  
  y_min_pos = (is_wc(track_map,x_wall_pos) 
                 && ((x_pos % 3) == 2) )
              ? (WC_YPOS + 1)
              : (FIRSTROW_YPOS);
  
  if (y_pos < y_min_pos) valid = 0;
  if (y_pos > (FIRSTROW_YPOS+ROW_NUM-1)) valid = 0;
  if (x_pos < 1) valid = 0;
  if (x_pos > 30) valid = 0;

  return valid;
}

void draw_runner(unsigned char move,
                  unsigned char frame,
                  unsigned int pos,
                  unsigned char clear)
{
  unsigned char X, Y, moveindex;
  char *sprite;
  unsigned char w,h,x,y,i, j, n;
  unsigned char *posaddr;
  
  moveindex = move * ID_NUM;
  
  X = pos % 32;
  Y = pos / 32;
  if (clear) { frame = 4; }
  
  draw_sprite(move_sprites[move], 
              move_coords[moveindex + ID_W],
              move_coords[moveindex + ID_H],
              X + move_coords[moveindex + ID_X],
              Y + move_coords[moveindex + ID_Y],
              frame);  
}

void init(void)
{
  move_sprites[ID_NOMOVE] = warming;
  move_sprites[ID_LEFT] = run_left;
  move_sprites[ID_RIGHT] = run_right;
  move_sprites[ID_DOWN] = run_down;
  move_sprites[ID_UP] = run_up;  
  move_sprites[ID_PISS] = piss;  

  keyboard_joy.fire  = in_LookupKey('m');
  keyboard_joy.left  = in_LookupKey('o');
  keyboard_joy.right = in_LookupKey('p');
  keyboard_joy.up    = in_LookupKey('q');
  keyboard_joy.down  = in_LookupKey('a');
}

void redraw(unsigned char clear)
{
  signed char i;
  static signed char prevlevel = 0;
  static signed char prevtrack = -1;

  if (clear == 0)
  {
    if (level != prevlevel)
    {
      zx_border(0);
      draw_ui();
      draw_title();
      draw_field();
    }
    prevlevel = level;

    if (track[0] != prevtrack)
    {
      draw_walls();
    }
    prevtrack = track[0];

    draw_states();
  }
  /*
  draw_runner(move[0], n, pos[0], clear);
  draw_runner(move[1], n+1, pos[1], clear);
  draw_runner(move[2], n+2, pos[2], clear);
  draw_runner(move[3], n+3, pos[3], clear);
*/
  for (i=0; i< NUM_PLAYERS; i++)
  {
    if(track[0]==track[i])
      draw_runner(move[i], ((n+4-i)%4), pos[i], clear);
  }
}

void move_AI(void)
{
  int i;
  int x_pos, y_pos, x_wall_pos;
  char iswc, istop, isdown, ispissed;
  unsigned int track_map;

  if (level == 1) track_map = tracks_level_1[track[0]];
  if (level == 2) track_map = tracks_level_2[track[0]];

  if (level == 1) track_map = tracks_level_1[track[0]];
  if (level == 2) track_map = tracks_level_2[track[0]];


  for (i=1; i< NUM_PLAYERS; i++)
  {
    x_pos = pos[i] % 32;
    y_pos = pos[i] / 32;
    x_wall_pos = (x_pos - 1)/3;
    iswc = is_wc(track_map,x_wall_pos) && ((x_pos % 3) == 2);
    istop = (y_pos <= WC_YPOS + 1);
    isdown = (y_pos == FIRSTROW_YPOS);

    if(iswc)
    {
      if (piss_storage[i] < PISSDELTA)
      {
        if(isdown)
        {
          move[i] = ID_RIGHT;
        }
        else
        {
          move[i] = ID_DOWN;
        }
      }
      else
      {
        if (istop)
        {
          move[i] = ID_PISS;
          piss_storage[i] = piss_storage[i] - PISSDELTA;
        }
        else
        {
          move[i] = ID_UP;
        }
      }
    }
    else
    {
      piss_storage[i] = 100;
      move[i] = ID_RIGHT;
    }
  }
}

void check_piss_state(void)
{
  static char prev_piss_state = 0;
  
  if (
         (move[0] == ID_PISS)
      && (prev_piss_state == 0)
      )
  {
    if ((pos[0] / 32) > (WC_YPOS + 1))
    {      
      if (lives>0) lives--; 
    }
  }
    
  prev_piss_state = (move[0] == ID_PISS);
  
  piss_storage[0] = (piss_storage[0] < 100)
                    ? (piss_storage[0]
                       + ( ((rand() % 10) > 8) ? 1 : 0 ) )
                    : (100);
  if (move[0] == ID_PISS)
    piss_storage[0] = (piss_storage[0] < PISSDELTA)
                      ? 0
                      : piss_storage[0] - PISSDELTA;

  if(piss_storage[0]==100) {
    move[0] = ID_PISS;
    blockmove = 1;
  }

  if(piss_storage[0]==0)
  {
    blockmove = 0;
  }
}

int game_main(void)
{
  unsigned int time = 0;
  unsigned char place = 255;
  unsigned int i;

  while (lives > 0)
  {
    if ((move[0] == 0) && (check_keyboard() != 0)) move[0] = 0;
    if (!blockmove)
    {
      if (n == 0) move[0] = check_keyboard();
    }
    move_AI();
    if (!validate(pos[n], move[n])) move[n] = 0;

    if (pos[n] % 32 == 30)
    {
      redraw(1);
      
      track[n]++;
      if (track[n] >= LEVEL_LENGTH)
      {
        track[n] = 0;
        if (n==0) level++;
      }
      pos[n] = (pos[n] / 32) * 32 + 1;
    } 
    
    redraw(0);
        
    pos[(n+1)%4] += moves[move[(n+1)%4]];

    // if (n == 0)
    check_piss_state();

    time++;
    n = time % 4;
  }
  
  if (lives <= 0) return(MENU_GAME_OVER);
  
  place = 1;
  for (i = 1; i < NUM_PLAYERS; i++) 
    if (pos[0] < pos[i]) place++;
  
  if (place == 1) return(MENU_GAME_WIN);
  
  if (place > 1) return(MENU_GAME_END);
}

void draw_init()
{
  zx_border(0);
  memset(SCREEN_ADDR,0x00,0x1800);
  draw_title();      
  memset(SCRATTR_ADDR + 64,0x07,768 - 64);
}

void draw_read_me(void)
{
  unsigned char i;

  for (i=0; i<21; i++)
  {
    memset(SCRATTR_ADDR + (i+3)*32,read_me_colors[i],32);   
    print(0,i+3,read_me[i]); 
  }
}

void draw_menu_main(void)
{
  unsigned char i;

  for (i=0; i<21; i++)
  {
    memset(SCRATTR_ADDR + (i+3)*32,main_menu_colors[i],32);   
    print(0,i+3,main_menu[i]); 
  }
  
  for (i = 0; i < 16; i++)
  {
    draw_sprite(move_sprites[ID_RIGHT],2,2,i*2,19,i%4);
  }

}


int main(void)
{
  unsigned int ch;
  int *temp;
  int prev_menu_state = 255;  
  
  bit_open();
  
  init();
  
  init_boldfont();
  
  while(1)
  {
    switch(menu_state)
    {
      case MENU_MAIN:
        if (prev_menu_state != menu_state)
        {
          draw_init();
          draw_menu_main();
        }
        else
        {
          in_Wait(300);
        }
        ch = 0;
        while 
        (
           (ch != '1') &&
           (ch != '2') &&
           (ch != '3')
        )
        {
 //         ch = in_Inkey();
          ch = music_play();
        }
        prev_menu_state = menu_state;
        if (ch == '1') menu_state = MENU_GAME;
        if (ch == '2') menu_state = MENU_README;
        if (ch == '3')
        {
          music_on = !music_on;
          zx_border(0);
        }
      break;
      case MENU_README:
        draw_init();
        draw_read_me();
        ch = 0;
        while(ch == 0)
        {
//          ch = in_Inkey();
          ch = music_play();
        }
        prev_menu_state = menu_state;
        menu_state = MENU_MAIN;
      break;
      case MENU_GAME:
        draw_init();
        game_main();
        prev_menu_state = menu_state;
        menu_state = MENU_GAME_END;
      break;
      case MENU_GAME_OVER:
        ch = getkey();
        prev_menu_state = menu_state;
        menu_state = MENU_MAIN;
      break;
    }
  }
  
  bit_close();
}

