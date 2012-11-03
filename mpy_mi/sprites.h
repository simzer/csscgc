
// gets a byte from the 'bytewidth'x'byteheight' sized 
// 'sprite' at the given position
// frame * bytewidth*byteheight*8 + bitrow * bytewidth + bytecol
#define INDEX_XxY(sprite,bytewidth,byteheight,frame,bitrow,bytecol) \
  sprite[(((frame) * (bytewidth) * (byteheight)) << 3) \
         + (bitrow) * (bytewidth) + (bytecol)]

// 24 pix X 24 pix

/*
unsigned char wall1[72]= {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 85, 85, 85,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255};
*/
unsigned char wall1[72]= {
  0,  0,  0,127, 63,254, 73, 34, 34, 73, 63,254,127,  0,  0, 73,  3,255, 73,  2, 73,127,  2, 73,
 73,123,255, 73, 74, 73,127, 74, 73, 73,123,255, 73, 74, 73, 73, 74, 73,127,123,255,  0,  0,  0,
 85, 85, 85,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255}; 
  
unsigned char wall2[72] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0, 34,  9,  8, 54, 15, 24, 28, 70, 80,  8,194,117,200,146, 99, 13,166, 98, 69, 36, 38, 71, 44,
 60, 62, 40, 49,188, 48, 35, 24, 16, 51, 25,144, 19, 25,152, 19, 25,152, 51, 29,152,255,255,255};
/*
unsigned char wc1[72] = {
  0,  0,  0,  1, 83,  0,  1, 84,  0,  1, 84,  0,  0,227,  0,  0,  0,  0,  3,255,192,  3,255,192,
  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,  3,255,192,
 87,  0,213,  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,255,255,255};
*/
unsigned char wc1[72] = {
  0,  0,  0,121, 83,127, 73, 84, 73, 73, 84, 73,120,227,127, 72,  0,  0, 75,255,223,123,255,213,
 75,  0,213, 75,  0,223,123,  0,213,  3,  0,223,123,  0,213, 75,  0,213,123,  0,223,  3,255,192,
 87,  0,213,  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,  3,  0,192,255,255,255};
  
unsigned char wc2[81] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  0,128,105,  0,128,105,  0,169, 79,  0, 73, 67,  0, 69,194,  0, 70,110,255,198,
 56,  0,108, 56,  0, 60, 48,  0, 56, 48,  0, 48, 48,  0, 48, 48,  0,112, 48,  0,112,255,255,255};

// 16 pix X 16 pix

// frame * 2*2*8 + bitrow * 2 + bytecol
#define INDEX_16x16(sprite,frame,bitrow,bytecol) \
  sprite[((frame) << 5) | ((bitrow) << 1) | (bytecol)]

unsigned char warming[5*16] = {
// 0 frame
  0,  12,  28, 24, 56, 48, 60, 177,
 60,  20,  20, 22, 23, 24, 28, 255,
// 1 frame
  0,  12,  28, 24, 56, 48, 60,179,
 56,  30,  15, 28, 56, 28,  0,255,
// 2 frame
  0,  12,  28, 24, 56, 48, 60,177,
 62,  34,  36, 38, 39, 48, 56,255,
// 3 frame 
  0,  12,  28, 24, 56, 48, 60,177,
 56,  28,  14, 28, 56, 28,  0,255,
// 4 frame = empty
  0,   0,   0,  0,  0,  0,  0,255,
  0,   0,   0,  0,  0,  0,  0,255
  };

/*
// 0 frame
  0,  0, 12,  0, 28,  0, 24,  0, 56,  0, 48,  0, 60,  0,177,255,
 60,  0, 20,  0, 20,  0, 22,  0, 23,  0, 24,  0, 28,  0,255,255,
// 1 frame
  0,  0, 12,  0, 28,  0, 24,  0, 56,  0, 48,  0, 60,  0,179,255,
 56,  0, 30,  0, 15,  0, 28,  0, 56,  0, 28,  0,  0,  0,255,255,
// 2 frame
  0,  0, 12,  0, 28,  0, 24,  0, 56,  0, 48,  0, 60,  0,177,255,
 62,  0, 34,  0, 36,  0, 38,  0, 39,  0, 48,  0, 56,  0,255,255,
// 3 frame 
  0,  0, 12,  0, 28,  0, 24,  0, 56,  0, 48,  0, 60,  0,177,255,
 56,  0, 28,  0, 14,  0, 28,  0, 56,  0, 28,  0,  0,  0,255,255};
*/
unsigned char start[64] = {
// 0 frame
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12,255,189,
  0,120,  0,232,  1,200,  3,248, 11, 40, 31,108, 19,116,255,255,
// 1 frame 
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12,251,253,
  7,248,  7,  8,  7,136, 12,200, 24,104, 48,108, 24,116,255,255};

unsigned char run_right[5*32] = {
// 0 frame
  0,  0, 12,  0, 28,  0, 24,  0, 56,  0, 48,  0, 48,  0,189,255,
 36,  0,108,  0, 72,  0, 76,  0,206,  0,192,  0, 96,  0,255,255,
// 1 frame
  0,  0,  1,128,  3,128,  3,  0,  7,  0,  6,  0,  6,  0,246,255,
  7,  0,  5,128,  5,128, 31,  0,  27,  0, 25,128,  8,  0,255,255,
// 2 frame
  0,  0,  0, 96,  0,224,  0,192,  1,192,  1,128,  1,128,253,191,
  1,192,  0,192,  7,192,  6,192,  5,128,  1,128,  0,192,255,255,
// 3 frame
  0,  0,  0, 24,  0, 56,  0, 48,  0,112,  0, 96,  0, 96,255,111,
  0, 96,  0,112,  0,248,  0,240,  0,224,  0,192,  0,224,255,255,
  // 4 frame = empty
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255};

unsigned char run_left[5*32] = {
// 0 frame
  0,  0,  0, 48,  0, 56,  0, 24,  0, 28,  0, 12,  0, 12,255,189,
  0, 36,  0, 54,  0, 18,  0, 50,  0,115,  0,  3,  0,  6,255,255,
// 1 frame
  0,  0,  1,128,  1,192,  0,192,  0,224,  0, 96,  0, 96,255,111,
  0,224,  1,160,  1,160,  0,248,  0,216,  1,152,  0, 16,255,255,
// 2 frame
  0,  0,  6,  0,  7,  0,  3,  0,  3,128,  1,128,  1,128,253,191,
  3,128,  3,  0,  3,224,  3, 96,  1,160,  1,128,  3,  0,255,255,
// 3 frame
  0,  0, 24,  0, 28,  0, 12,  0, 14,  0,  6,  0,  6,  0,246,255,
  6,  0, 14,  0, 31,  0, 15,  0,  7,  0,  3,  0,  7,  0,255,255,
  // 4 frame = empty
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255};

// 8 pix X 24 pix

#define INDEX_8x24(sprite,frame,bitrow) \
  sprite[((frame) * 3) << 3 | (bitrow)]

unsigned char run_up[5*24] = {
// 0 frame
  0,  0,  0,  0,  0,  0,  0,255,
  0, 24, 24, 24, 60, 60, 60,185,
 60, 60, 52, 44, 44, 96, 96,255,
// 1 frame
  0,  0,  0,  0,  0,  0,  0,219,
 24, 24, 60, 60, 60, 56, 60,189,
 20, 52, 60, 60, 12,  0,  0,255,
// 2 frame
  0,  0,  0,  0,  0, 24, 24,153,
 60, 60, 60, 56, 60, 60, 28,185,
 56, 60, 28,  0,  0,  0,  0,255,
// 3 frame
  0,  0, 24, 24, 24, 60, 60,189,
 56, 60, 60, 28, 20, 52, 60,255,
  0,  0,  0,  0,  0,  0,  0,255,
/*
  0,  0,  0, 24, 24, 24, 60,189,
 60, 56, 60, 60, 28, 52, 60,189,
 12,  0,  0,  0,  0,  0,  0,255
*/
// 4 frame = empty
  0,  0,  0,  0,  0,  0,255,255,
  0,  0,  0,  0,  0,  0,255,255,
  0,  0,  0,  0,  0,  0,255,255};
  
unsigned char run_down[5*24] = {
// 0 frame
  0, 24, 24, 24, 60, 60, 60,185,
 56, 56,168,232,104, 12, 12,255,
  0,  0,  0,  0,  0,  0,  0,255,
// 1 frame
  0,  0,  0, 24, 24, 24, 60,189,
 60, 56, 56, 60, 54, 60,108,101,
 32,  0,  0,  0,  0,  0,  0,255,
// 2 frame 
  0,  0,  0,  0,  0, 24, 24,219,
 60, 60, 60, 56, 56, 60,124,121,
 32, 48, 48,  0,  0,  0,  0,255,
// 3 frame
  0,  0,  0,  0,  0,  0,  0,255,//219,
 24, 24, 60, 60, 60, 56, 56,187,
 60, 36,124,120, 48,  0,  0,255,
// 4 frame = empty
  0,  0,  0,  0,  0,  0,255,255,
  0,  0,  0,  0,  0,  0,255,255,
  0,  0,  0,  0,  0,  0,255,255};

// 8 pix X 16 pix

#define INDEX_8x16(sprite,frame,bitrow) \
  sprite[(frame) << 4 | (bitrow)]

unsigned char piss[5*16] = {
// 0 frame
  0, 24, 24, 24, 62, 62, 62, 28,
 60, 60,124,102, 82,195,211,255,
// 1 frame
  0, 24, 24, 24, 62, 62, 62, 28,
 60, 60,124,118, 66,211,235,255,
// 2 frame
  0, 24, 24, 24, 62, 62, 62, 28,
 60, 60,124,102, 82,195,211,255,
// 3 frame
  0, 24, 24, 24, 62, 62, 62, 28,
 60, 60,124,118, 66,211,235,255,
// 4 frame = empty
  0,   0,   0,  0,  0,  0,  0,255,
  0,   0,   0,  0,  0,  0,  0,255
  };

// 8 pix X 8 pix

#define INDEX_8x8(sprite,frame,bitrow) \
  sprite[(frame) << 3 | (bitrow)]

unsigned char bottle1[8] = 
{  0, 24, 36, 60, 60, 60, 24,255};

unsigned char bottle2[8] = {
  0, 24, 24, 52, 52, 52, 60,255};
  
unsigned char floor[8] = {
  0,  0,  0,  0,  0,  0,  0,255};
  
unsigned char food[8] = {
  0, 12, 22, 44,120, 48,  0,255};
  
unsigned char life[8] = {
 56, 56, 16,126,122, 60,110,110};
 
unsigned char indicator_left[8] = {
224,128,128,  0,  0,128,128,224};

unsigned char indicator_right[8] = {
  7,  1,  1,  0,  0,  1,  1,  7};

unsigned char empty_field[8] = {
  0,  0,  0,  0,  0,  0,  0,  255};

unsigned char corners[32] = {
// bottom left
  0,  0,  0,  0,128,128,192,240,
// bottom right
  0,  0,  0,  0,  1,  1,  3, 15,
// top left
 15,  3,  1,  1,  0,  0,  0,  0,
// top right
240,192,128,128,  0,  0,  0,  0};
