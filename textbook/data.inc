
/* 0: 11*/
const char content_11[] = 
"- DON'T GIVE ME THAT, YOU SNOTTY-FACED HEAP OF PARROT DROPPINGS! SHUT YOUR FESTERING GOB, YOU TIT! YOUR TYPE REALLY MAKES ME PUKE, YOU PERVERT! - shout Mr. Champman from the abuse room.";
/* 1: 21*/
const char content_21[] = 
"Congratulation! You Win...\n"
"...another round with the game.";
/* 2: 7*/
const char content_7[] = 
"Mr. Cleese:\n"
"- Yes I did!\n"
"\n"
"Answer!:\n"
"1. No you didn't\n"
"2. You did not!\n"
"3. Didn't";
/* 3: 17*/
const char content_17[] = 
"Yes it is. - he says.\n"
"\n"
"You say:\n"
"1. No it isn't, it's just contradiction.\n"
"2. No it isn't.";
/* 4: 2*/
const char content_2[] = 
"You open the door and ask:\n"
"- Is this the right room for an argument?\n"
"Mr. Cleese is sitting at the desk and answer:-I've told you once.\n"
"\n"
"Well, this is odd, becouse he has not. What are you say?\n"
"1. No you haven't.\n"
"2. When?\n"
"3. Ok, I know the sketch, he just want me to contradict. ";
/* 5: 1*/
const char content_1[] = 
"Monty Python's Argument Clinic Textgame.\n"
"made for the 2009 CSSCGC competition\n"
"\n"
"by a text game generator script\n"
"which made by L.Simon\n"
"who made by ...\n"
"\n"
"Well, lets begin instead. You are in the argument clinic. You paid for a 5 min agrument just now.";
/* 6: 18*/
const char content_18[] = 
"No it isn't. - Mr. Cleese replies.\n"
"\n"
"Now you can say:\n"
"1. Yes, It is.\n"
"2.It is. You just contradicted me. ";
/* 7: 16*/
const char content_16[] = 
"Mr. Cleese can not stop:\n"
"- Yes I did.\n"
"\n"
"You may answer:\n"
"1. Look, this isn't an argument.\n"
"2. You did not.";
/* 8: 13*/
const char content_13[] = 
"Mr. Cleese is arguing:\n"
"- Yes I did.\n"
"\n"
"Say:\n"
"1. didn't\n"
"2. You did not.\n"
"3. You most certainly did not.";
/* 9: 6*/
const char content_6[] = 
"Mr. Cleese disagree:\n"
"-No I have not.\n"
"\n"
"What are you say?\n"
"1. No you haven't.\n"
"2. Yes you have\n"
"3. When?";
/* 10: 3*/
const char content_3[] = 
"Mr. Cleese say:\n"
"-Yes I have.\n"
"\n"
"What are you say?\n"
"1. No you haven't.\n"
"2. Yes you have\n"
"3. When?";
/* 11: 9*/
const char content_9[] = 
"- I'm sorry, is this a five minute argument, or the full half hour? - Ask Mr. Cleese suddenly.\n"
"\n"
"You may answer:\n"
"1. Oh ... Just a five-minute one.\n"
"2. Oh ... It's half an hour.";
/* 12: 12*/
const char content_12[] = 
"Mr. Cleese:\n"
"- Yes I did!\n"
"\n"
"Answer!:\n"
"1. didn't\n"
"2. You did not.";
/* 13: 20*/
const char content_20[] = 
"Mr.Cleese: - No, no, no, no, no.\n"
"You:       - You did, just then.\n"
"Mr.Cleese: - No, nonsense!\n"
"You:      - Oh look, this is futile.\n"
"";
/* 14: 14*/
const char content_14[] = 
"- Now, let's get one thing quite clear. I most definitely told you! - Mr. Cleese says.\n"
"\n"
"Something to answer?\n"
"1. didn't\n"
"2. You did not.\n"
"3. You most certainly did not.";
/* 15: 15*/
const char content_15[] = 
"Mr. Cleese:\n"
"- Yes I did!\n"
"\n"
"Answer!:\n"
"1. didn't\n"
"2. You did not.\n"
"3. You most certainly did not.";
/* 16: 8*/
const char content_8[] = 
"- Did.\n"
"\n"
"What can you say:\n"
"1. No you didn't\n"
"2. You did not!\n"
"3. Didn't";
/* 17: 4*/
const char content_4[] = 
"- Just now! - Mr. Cleese's reply.\n"
"\n"
"Now what?\n"
"1. No you didn't.\n"
"2.When?\n"
"3. I knew he will say this. I want a MP. and a holy grail game instead.";
/* 18: 19*/
const char content_19[] = 
"Mr. Cleese: \n"
"- No I didn't.\n"
"\n"
"How you say?\n"
"1.Ooh, you did!\n"
"2. Yes you did.\n"
"3. You did, just then.";
/* 19: 10*/
const char content_10[] = 
"- Fine. Thank you. Anyway, I did. - says Mr. Cleese.\n"
"\n"
"You can answer:\n"
"1. didn't\n"
"2. You did not.\n"
"3. You most certainly did not.";
/* 20: 5*/
const char content_5[] = 
"Oh, you ruined the sketch. ";


const unsigned short cont_pointer[22];

const unsigned char links[48] = 
{
19, /* 0: 11*/
1, /* 1: 21*/
2, 11, 16, /* 2: 7*/
6, 3, /* 3: 17*/
10, 17, 20, /* 4: 2*/
4, /* 5: 1*/
6, 18, /* 6: 18*/
3, 15, /* 7: 16*/
12, 8, 14, /* 8: 13*/
10, 9, 17, /* 9: 6*/
10, 9, 17, /* 10: 3*/
19, 0, /* 11: 9*/
15, 8, /* 12: 12*/
1, /* 13: 20*/
12, 8, 14, /* 14: 14*/
12, 7, 14, /* 15: 15*/
2, 11, 16, /* 16: 8*/
2, 17, 20, /* 17: 4*/
13, 13, 13, /* 18: 19*/
12, 8, 14, /* 19: 10*/
20, /* 20: 5*/
};

const unsigned short link_pointer[22] = 
{
0, 1, 2, 5, 7, 10, 11, 13, 15, 18, 
21, 24, 26, 28, 29, 32, 35, 38, 41, 44, 
47, 48
};

const unsigned char poi_def = 5;

void init(void)
{
  cont_pointer[0] = content_11;
  cont_pointer[1] = content_21;
  cont_pointer[2] = content_7;
  cont_pointer[3] = content_17;
  cont_pointer[4] = content_2;
  cont_pointer[5] = content_1;
  cont_pointer[6] = content_18;
  cont_pointer[7] = content_16;
  cont_pointer[8] = content_13;
  cont_pointer[9] = content_6;
  cont_pointer[10] = content_3;
  cont_pointer[11] = content_9;
  cont_pointer[12] = content_12;
  cont_pointer[13] = content_20;
  cont_pointer[14] = content_14;
  cont_pointer[15] = content_15;
  cont_pointer[16] = content_8;
  cont_pointer[17] = content_4;
  cont_pointer[18] = content_19;
  cont_pointer[19] = content_10;
  cont_pointer[20] = content_5;
 
}

