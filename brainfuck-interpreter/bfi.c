#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <input.h>

char arr[1024];
char *ptr;
char prog[2048];

char pcnt;
char nesting;
short pbegin[100];
short pend[100];

char availableKeys[] = "trkjmnyu01\n";
char resultChars[]   = "><+-.,[]01\n";

char hello[113] = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.\n";

char key2char(char ch)
{
  int i;
  for (i = 0; i < 12/*sizeOf(availableKeys) - not supported*/; i++)
  {
    if (ch == availableKeys[i]) return(resultChars[i]);
  }
  return(0);
}

void __FASTCALL__ zxputchar(char ch)
{
#asm
ld  a, l
rst 16
#endasm
}

void __FASTCALL__ zxprint(char *string)
{
#asm
printline:
ld a,(hl)
cp 0
jp z,printend
rst 16
inc hl
jp printline
printend:
#endasm
}

void cmd(char ch)
{
 char c;
 switch(ch)
 {
   case '>':   ptr++; break;
   case '<':   ptr--; break;
   case '+':   (*ptr)++; break;
   case '-':   (*ptr)--; break;
   case '.':   zxputchar(*ptr); break;
   case ',':   while(c = in_Inkey(), c==0); *ptr=c; zxputchar(c); break;
   case '[':
   {
     if(*ptr) { nesting++; } else { pcnt = pend[nesting+1]; }
     break;
   }
   case ']':
   {
     if(*ptr) { pcnt = pbegin[nesting]; } else { nesting--; }
     break;
   }
 }
}

void preproc(void)
{
 short i;
 nesting = -1;
 for (i = 0; i  < strlen(prog); i++)
 {
   if (prog[i] == '[')
   {
     nesting++;
     pbegin[nesting] = i;
   }
   if (prog[i] == ']')
   {
     pend[nesting] = i;
     nesting--;
   }
 }
}

void run(void)
{
  memset(arr,0,1024);
  ptr = arr;
  nesting = -1;
  pcnt = 0;
  while (prog[pcnt] != '\0')
  {
    cmd(prog[pcnt]);
    pcnt++;
  }
}


void getstring(char *s)
{
  char *p = s;
  char c = 0;
  char cprev = 0;
  int i, timer = 100;
  char cursor = '|';
  while (c != '\n')
  {
    while(c = in_Inkey(), ((key2char(c)==0) || (c == cprev)) )
    {
      cprev = c;
      timer--;
      if (timer <= 0) {
        timer = 100;
        zxputchar('\b');
        cursor = (cursor == '|') ? ' ' : '|';
        zxputchar(cursor);
      }
    };
    cprev = c;
    zxputchar('\b');
    if (c == '0') {
      zxputchar(' ');
      zxputchar('\b');
      zxputchar('\b');
      zxputchar(' ');
      zxputchar('\b');
      p--;
    } else if (c == '1') {
      p = s;
      for(i = 0; i < 113; i++)
      {
        *p = hello[i];
        if ((*p != '\n') && (*p != '\0')) zxputchar(*p);
        p++;
      }
      *p = '\0';
      return;
    } else if (c == '\n') {
      *p = '\0';
      return;
    } else {
      *p = key2char(c);
      p++;
      zxputchar(key2char(c));
    }
    zxputchar('|');
  }
}

void main(void)
{
  zxprint("\nZX Brainfuck Interpreter\n\n");
  zxprint("T > increment data pointer\n");
  zxprint("R < decrement data pointer\n");
  zxprint("K + increment data at pointer\n");
  zxprint("J - decrement data at pointer\n");
  zxprint("M . write out data at pointer\n");
  zxprint("N , read byte to data\n");
  zxprint("Y [ conditional jump to ]\n");
  zxprint("U ] conditional jump back to [\n");
  zxprint("0 delete\n");
  zxprint("1 Hello World\n");
  zxprint("ENTER run the program\n\n|");

  while(1)
  {
    getstring(prog);
    preproc();
    zxprint("\nStart program\n");
    run();
    zxprint("\nProgram finished\n|");
  }
}
