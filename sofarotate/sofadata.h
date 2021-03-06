#define ROOMVERTEXNUM 12
#define ROOMEDGENUM 11

#define SOFAEDGENUM 32
#define SOFAVERTEXNUM 22

#define VERTEXNUM (ROOMVERTEXNUM+SOFAVERTEXNUM)
#define EDGENUM (ROOMEDGENUM+SOFAEDGENUM)

#define RWX 220
#define RWY 100
#define RWZ 220

/*
    SOFA 3D datas

         z                                  z
         |    13___________12              7|------------------------|6
        15___/\____________\11_10           |\                       |\
         |\14\/C           B\ .\            | \                      | \
         | \ .\D           A|\. \           |  \                     |  \
      y  |  \7_\6           | \__\2         |   \3           |9      |   \2
       \ |  |.8.|__________9| |3  |        4|----------------|-------|5
        E|..|...|...........\.|F  |          \               |        \
         \  |  .|____________\|.  |           \              |         \
          \ |   5             4 . |            \             |          \
           \|_____________________| __x        0\            |8          \1
            0                     1
*/

/* edges (start-, end vertex ids) */
/* vertices x, y, z coords in cm */

unsigned char sofa_edges[SOFAEDGENUM*2] =
{
0x00,0x01, 0x01,0x02, 0x02,0x03, 0x03,0x04, 0x04,0x05, 0x05,0x06, 0x06,0x07, 0x07,0x00,
0x08,0x09, 0x09,0x0A, 0x0A,0x0B, 0x0B,0x0C, 0x0C,0x0D, 0x0D,0x08,
0x0E,0x0F, 0x0F,0x10, 0x10,0x11, 0x11,0x12, 0x12,0x13, 0x13,0x14, 0x14,0x15, 0x15,0x0E,
0x00,0x0E, 0x01,0x0F, 0x02,0x10, 0x03,0x11, 0x04,0x09, 0x05,0x08, 0x06,0x14, 0x07,0x15,
0x0B,0x12, 0x0C,0x13
};

signed char sofa_vertices[SOFAVERTEXNUM * 3] =
{
-100, -40,   0, /* 00   */
 100, -40,   0, /* 01 */
 100, -40,  60, /* 02 */
  80, -40,  60, /* 03 */
  80, -40,  30, /* 04 */
 -80, -40,  30, /* 05 */
 -80, -40,  60, /* 06 */
-100, -40,  60, /* 07 */
 -80,  -5,  30, /* 08 */
  80,  -5,  30, /* 09 */
  80,  10,  60, /* 0A */
  60,  20,  90, /* 0B */
 -60,  20,  90, /* 0C */
 -80,  10,  60, /* 0D */
-100,  40,   0, /* 0E */
 100,  40,   0, /* 0F */
 100,  40,  60, /* 10 */
  80,  40,  60, /* 11 */
  60,  40,  90, /* 12 */
 -60,  40,  90, /* 13 */
 -80,  40,  60, /* 14 */
-100,  40,  60, /* 15 */
};

signed short room_vertices[ROOMVERTEXNUM * 3] =
{
   0,-RWY,   0, /* 00   */
 RWX,-RWY,   0, /* 01 */
 RWX,-RWY, RWZ, /* 02 */
   0,-RWY, RWZ, /* 03 */
   0, RWY,   0, /* 04 */
 RWX, RWY,   0, /* 05 */
 RWX, RWY, RWZ, /* 06 */
   0, RWY, RWZ, /* 07 */
RWX/2,  0,   0, /* 08 */
RWX/2,  0, RWZ, /* 09 */
RWX/2,-RWY,  0, /* 0A */
RWX/2,-RWY,RWZ, /* 0B */
};

unsigned char room_edges[ROOMEDGENUM*2] =
{
0x00,0x04, 0x04,0x05, 0x05,0x01,
0x03,0x07, 0x07,0x06, 0x06,0x02,
0x04,0x07, 0x05,0x06, 0x08,0x09,
0x08,0x0A, 0x09,0x0B,
};

signed char cos[256] =
{
127,127,127,127,126,126,126,125,125,124,
123,122,122,121,120,119,117,116,115,114,
112,111,109,107,106,104,102,100,98,96,
94,92,90,88,86,83,81,78,76,73,
71,68,66,63,60,58,55,52,49,46,
43,40,37,34,31,28,25,22,19,16,
13,10,7,4,1,-1,-5,-8,-11,-14,
-17,-20,-23,-26,-29,-32,-35,-38,-41,-44,
-47,-50,-53,-55,-58,-61,-64,-66,-69,-71,
-74,-76,-79,-81,-84,-86,-88,-90,-92,-95,
-97,-98,-100,-102,-104,-106,-107,-109,-110,-112,
-113,-115,-116,-117,-118,-119,-120,-121,-122,-123,
-123,-124,-124,-125,-125,-126,-126,-126,-126,-126,
-126,-126,-126,-125,-125,-124,-124,-123,-123,-122,
-121,-120,-119,-118,-117,-116,-114,-113,-112,-110,
-109,-107,-105,-104,-102,-100,-98,-96,-94,-92,
-90,-88,-85,-83,-81,-78,-76,-73,-71,-68,
-66,-63,-60,-58,-55,-52,-49,-46,-43,-41,
-38,-35,-32,-29,-26,-23,-19,-16,-13,-10,
-7,-4,-1,1,4,7,10,14,17,20,
23,26,29,32,35,38,41,44,47,50,
52,55,58,61,63,66,69,71,74,76,
79,81,84,86,88,90,93,95,97,99,
101,102,104,106,108,109,111,112,114,115,
116,118,119,120,121,122,123,123,124,125,
125,126,126,126,127,127
};

signed char sin[256]=
{
0,3,6,9,12,15,19,22,25,28,
31,34,37,40,43,46,48,51,54,57,
60,62,65,68,70,73,75,78,80,83,
85,87,90,92,94,96,98,100,102,104,
105,107,109,110,112,113,115,116,117,118,
119,120,121,122,123,124,124,125,126,126,
126,127,127,127,127,127,127,127,126,126,
126,125,125,124,123,123,122,121,120,119,
118,116,115,114,112,111,109,108,106,104,
103,101,99,97,95,93,90,88,86,84,
81,79,76,74,71,69,66,64,61,58,
55,52,50,47,44,41,38,35,32,29,
26,23,20,17,14,11,7,4,1,-1,
-4,-7,-10,-13,-16,-19,-22,-25,-29,-32,
-35,-37,-40,-43,-46,-49,-52,-55,-58,-60,
-63,-66,-68,-71,-73,-76,-78,-81,-83,-85,
-88,-90,-92,-94,-96,-98,-100,-102,-104,-105,
-107,-109,-110,-112,-113,-114,-116,-117,-118,-119,
-120,-121,-122,-122,-123,-124,-124,-125,-125,-126,
-126,-126,-126,-126,-126,-126,-126,-125,-125,-124,
-124,-123,-123,-122,-121,-120,-119,-118,-117,-116,
-115,-113,-112,-111,-109,-107,-106,-104,-102,-100,
-99,-97,-95,-93,-90,-88,-86,-84,-81,-79,
-76,-74,-71,-69,-66,-64,-61,-58,-55,-53,
-50,-47,-44,-41,-38,-35,-32,-29,-26,-23,
-20,-17,-14,-11,-8,-5
};
