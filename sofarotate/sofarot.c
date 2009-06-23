
#include <string.h>
#include <stdio.h>
#include <input.h>
#include <spectrum.h>
#include "sofadata.h"

/* actual 3D points of the sofa */
signed short vertices[VERTEXNUM * 3];

/* actual 2D projected points of the sofa */
unsigned char points[VERTEXNUM * 2];

/* actual 2D projected points of the sofa */
unsigned char edges[EDGENUM*2];

void data_init(void)
{
  unsigned char i;

  for(i = 0; i< SOFAEDGENUM; i++)
  {
    edges[i*2] = sofa_edges[i*2];
    edges[i*2+1] = sofa_edges[i*2+1];
  }
  for(i = 0; i< ROOMEDGENUM; i++)
  {
    edges[(i+SOFAEDGENUM)*2] = room_edges[i*2] + SOFAVERTEXNUM;
    edges[(i+SOFAEDGENUM)*2+1] = room_edges[i*2+1] + SOFAVERTEXNUM;
  }
  for(i = 0; i< ROOMVERTEXNUM; i++)
  {
    vertices[(i+SOFAVERTEXNUM)*3]   = room_vertices[i*3];
    vertices[(i+SOFAVERTEXNUM)*3+1] = room_vertices[i*3+1];
    vertices[(i+SOFAVERTEXNUM)*3+2] = room_vertices[i*3+2];
  }
}

void help(void)
{
  printf("SOFA MOVE SIMULATOR\n");
  printf("Based on Douglas Adam's\n");
  printf("Dirk Gently's Holistic Detective Agency\n");
  printf("for CSSCGC 2009 compo\n");
  printf("written by Laszlo Simon\n\n");
  printf("Goal is to move out the Richeard's sofa\n");
  printf("irrevocably stuck on the staircase.\n\n");
  printf("Controls:\n");
  printf("U-I move on X axis\n");
  printf("W-S move on Y axis\n");
  printf("E-D move on Z axis\n");
  printf("U-I move on X axis\n");
  printf("Q-A vertical rotate\n");
  printf("O-P horizontal rotate \n");
  printf("Z-X rotate view\n");
  printf("H help\n");
  printf("C cheat (no collision detect)\n\n");
}

/* draws a line */
void line_draw(unsigned char x0,
               unsigned char y0,
               unsigned char x1,
               unsigned char y1)
{
#define LINEOPT 1
  unsigned char mask;
  unsigned char dx, dy, x, y, temp;
#ifdef LINEOPT
  unsigned long d;
  unsigned long sum;
#endif

  #define PREC 65536
  dx = (x1 < x0) ? (x0 - x1) : (x1 - x0);
  dy = (y1 < y0) ? (y0 - y1) : (y1 - y0);

  if (dx>dy)
  {
    if (x1 < x0)
    {
      temp = x0; x0 = x1; x1 = temp;
      temp = y0; y0 = y1; y1 = temp;
    }

#ifdef LINEOPT
    d = ((signed long)y1-y0)*PREC/dx;
    sum = y0 * PREC;
#endif
    x = x0;
    while(x<x1)
    {
#ifndef LINEOPT
      y = y0 + ((signed short)y1-y0)*(x-x0)/dx;
#else
      sum += d;
//      y = sum>>16;
      y = *(((char*)(&sum))+2);
#endif
      *zx_pxy2saddr(x, y, &mask) |= mask;
      x++;
    }
  }
  else
  {
    if (y1 < y0)
    {
      temp = x0; x0 = x1; x1 = temp;
      temp = y0; y0 = y1; y1 = temp;
    }
#ifdef LINEOPT
    d = ((signed long)x1-x0)*PREC/dy;
    sum = x0 *PREC;
#endif
    y = y0;
    while(y<y1)
    {
#ifndef LINEOPT
      x = x0 + ((signed short)x1-x0)*(y-y0)/dy;
#else
      sum += d;
//      x = sum >> 16;
      x = *(((char*)(&sum))+2);
#endif
      *zx_pxy2saddr(x, y, &mask) |= mask;
      y++;
    }
  }
}


/* draws the sofa */
void sofa_draw(void)
{
  unsigned char i;
  unsigned char vertex0_id;
  unsigned char vertex1_id;

  unsigned short j;

  /* for each edge of the sofa */
  for (i = 0; i < EDGENUM; i++)
  {
    vertex0_id = edges[i*2];
    vertex1_id = edges[i*2+1];

    /* draw sofa edge */
    line_draw(points[vertex0_id * 2],
              points[vertex0_id * 2 + 1],
              points[vertex1_id * 2],
              points[vertex1_id * 2 + 1]);
  }
}

/* move and rotate the sofa */
void sofa_transf(signed short dx,
                 signed short dy,
                 signed short dz,
                 unsigned char u1,
                 unsigned char u2)
{
  /* rotation matrix */
  signed char rotm[9];
  signed long x, y, z;
  unsigned int i;

  rotm[0] =                cos[u2];
  rotm[1] =                0;
  rotm[2] =               -sin[u2];
  rotm[3] = -(signed short)sin[u1]*sin[u2]/128;
  rotm[4] =                cos[u1];
  rotm[5] = -(signed short)sin[u1]*cos[u2]/128;
  rotm[6] =  (signed short)cos[u1]*sin[u2]/128;
  rotm[7] =                sin[u1];
  rotm[8] =  (signed short)cos[u1]*cos[u2]/128;

  for (i = 0; i < SOFAVERTEXNUM; i++)
  {
    x = sofa_vertices[i*3];
    y = sofa_vertices[i*3+1];
    z = sofa_vertices[i*3+2];

    /* transform */
    vertices[i*3]   = ((x*rotm[0] + y*rotm[1] + z*rotm[2])>>7) + dx;
    vertices[i*3+1] = ((x*rotm[3] + y*rotm[4] + z*rotm[5])>>7) + dy;
    vertices[i*3+2] = ((x*rotm[6] + y*rotm[7] + z*rotm[8])>>7) + dz;
  }
}

/* sofa perspective projection */
void sofa_proj(unsigned char c)
{
  unsigned char i;
  signed short ox =  RWX/2;
  signed short oy = -400;
  signed short oz =  RWZ/2;
  signed short ky =  220;

  signed short x,z,y;
  signed long x0, y0;
  signed short dy;

  for (i = 0; i < VERTEXNUM; i++)
  {
    /* transform */
    x0 = vertices[i*3] - RWX/2;
    y0 = vertices[i*3+1];
    z = vertices[i*3+2];

    x = RWX/2
      +(( cos[c] * x0 + sin[c] * y0) >> 7);
    y = (-sin[c] * x0 + cos[c] * y0) >> 7;

    dy = y - oy;

    points[i*2]   = 127 + (x-ox)/4*ky/dy*3;
    points[i*2+1] = 95 - (z-oz)/4*ky/dy*3;
  }
}

unsigned char sofa_check(void)
{
  /* rotation matrix */
  signed short x, y, z, x2, y2, xm, ym;
  unsigned int i;
  unsigned char valid = 1;

  for (i = 0; i < SOFAVERTEXNUM; i++)
  {
    x = vertices[i*3];
    y = vertices[i*3+1];
    z = vertices[i*3+2];

    if(
      (x < 0) || (x > RWX) ||
      (y < -RWY) || (y > RWY) ||
      (z < 0) || (z > RWZ)
      )
    { valid = 0; }

    if (y < -RWY)
    {
      valid = 2;
      return(valid);
    }
  }

  xm = RWX/2;
  for (i = 0; i < SOFAEDGENUM; i++)
  {
    x = vertices[edges[i*2]*3];
    y = vertices[edges[i*2]*3+1];
    x2 = vertices[edges[i*2+1]*3];
    y2 = vertices[edges[i*2+1]*3+1];

    if(
        ((x < xm)&&(x2>xm)) ||
        ((x2< xm)&&(x >xm))
      )
    {
      ym = y + ((signed long)y2-y)*(xm-x)/(x2-x);

      if (ym < 0)
      {
        valid = 0;
      }
    }
  }

  return(valid);
}

void main(void)
{
  unsigned int key;
  unsigned char init = 0;
  unsigned char timer = 0;

  unsigned char cam = 240;
  signed short dx,dy,dz;
  unsigned char u1,u2;
  signed short ndx,ndy,ndz;
  unsigned char nu1,nu2;
  unsigned char valid, drawreq, helpreq;
  unsigned char check = 1;

  dx = 10 + 100;
  dy = 40;
  dz = 0;
  data_init();

  while(1)
  {
    if   (timer>0)
         {timer--;}
    else
    { zx_border((valid == 2) ? GREEN : YELLOW);}

    if(!init || (key = in_Inkey(), key))
    {
      helpreq = (!init) ? 1 : 0;

      ndx = dx;
      ndy = dy;
      ndz = dz;
      nu1 = u1;
      nu2 = u2;

      drawreq = 1;
      switch(key)
      {
        case 'o' : nu1 += 10; break;
        case 'p' : nu1 -= 10; break;
        case 'q' : nu2 += 10; break;
        case 'a' : nu2 -= 10; break;
        case 'w' : ndy += 10; break;
        case 's' : ndy -= 10; break;
        case 'e' : ndz += 10; break;
        case 'd' : ndz -= 10; break;
        case 'i' : ndx += 10; break;
        case 'u' : ndx -= 10; break;
        case 'z' : cam += 10; break;
        case 'x' : cam -= 10; break;
        case 'h' : helpreq = 1;break;
        case 'c' : check = !check;break;
        default  :
          if (init) drawreq = 0;
        break;
      }

      sofa_transf(ndx,ndy,ndz,nu1,nu2);

      init = 1;

      if (
          ((valid = sofa_check(), valid) || !check)
          &&(drawreq==1))
      {
        sofa_proj(cam);

        /* clear screen */
        memset(0x4000,0x00,256*192/8);

        if (helpreq == 1) help();

        sofa_draw();

        dx = ndx;
        dy = ndy;
        dz = ndz;
        u1 = nu1;
        u2 = nu2;

        if(valid == 2)
        {
          printf("Congratulation, You WIN!\n");
          zx_border(GREEN);
        }
      }
      else
      {
        /* revert */
        zx_border(RED);
        timer = 255;
      }
    }
  }
}
