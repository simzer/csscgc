#include <graphics.h>
#include <spectrum.h>
#include <stdio.h>

char	value[100];

int score[5] = {0,15,30,40};

void main(void)
{
  int point = 0;
  
  clg();
  
  printf("The BORIS BECKER Tenis 2009\n");
  printf("TextGame version - trial\n");
  printf("\n");
  printf("You are on the court. There is - opposite of you - the legendary Boris Becker.\n");
  printf("He will start a game.\n");

  while (1)
  {
	printf("Boris gets a ball from his pocket and flips it to the air.\n");
	printf("A great serve. The ball speeds towards the left corner.\n");
	printf ("You must hurry, what are you doing?\n")
	fgets_cons(value, 1);
	printf ("You were too slow. The ball snapped, you lost.");
    point++;
	printf ("Lost Games: %d; Score: %d-0.\n Boris can not be beaten!\nPrepare for the next serve!\n\n", point/4,score[point%4]);
  }
}

