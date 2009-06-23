
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <spectrum.h>

#define const

#include "data.inc"
/*
void print_substr(unsigned char pfrom, unsigned char pto)
{
//  char text[(pto-pfrom+1)];
    char text[512];

//  char *text;
//  text = (char *)malloc((pto-pfrom)+1);

  memcpy(text, &contents[pfrom], (pto-pfrom));

  text[(pto-pfrom)] = '\0';

  printf("%s\n", text);
}
*/
int main(void)
{
  unsigned char poi;
  unsigned char isnew;
  unsigned char poi_prev;
  unsigned int ch;

  init();

  clg();

  poi = poi_def;

  while(1)
  {
    printf("\n");
//    printf("pos:%d\n", poi);
    printf("\n");
//    print_substr(cont_pointer[poi], cont_pointer[poi+1]);
    printf(cont_pointer[poi]);
    printf("\n");

    poi_prev = poi;

    if (link_pointer[poi+1] - link_pointer[poi] == 1)
    {
      if (links[link_pointer[poi]] == poi)
      {
        printf("Game over, Press any key to restart the game.\n");
        poi = poi_def;
      }
      else
      {
        printf("Press any key to continue.\n");
        poi = links[link_pointer[poi]];
      }
      ch = getchar();
    }
    else
    {
      isnew = 0;
      while(!isnew)
      {
        printf("Please choose (%d-%d):", 1, (link_pointer[poi+1] - link_pointer[poi]) );

        ch = getchar();
        printf("%c\n", ch);
        ch = ch - '0';

        //scanf("%d", &ch);
        ch--;
        if ((ch >= 0) &&
            (ch < link_pointer[poi+1] - link_pointer[poi]))
        {
          poi = links[link_pointer[poi] + ch];
          isnew = 1;
        }
        else
        {
          printf("Invalid choise!\n");
        }
      }
    }
  }
}

