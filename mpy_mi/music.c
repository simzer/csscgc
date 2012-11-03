#include <sound.h> 
#include <stdio.h>

#define NUL  100

#define	G0	17461
#define	Gis0	18499
#define	A0	19600
#define	Ais0	20765
#define	H0	22000
#define	C1	23308
#define	Cis1	24694
#define	D1	26162
#define	Dis1	27718
#define	E1	29366
#define	F1	31113
#define	Fis1	32963
#define	G1	34923
#define	Gis1	36999
#define	A1	39200
#define	Ais1	41530
#define	H1	44000
#define	C2	46616
#define	Cis2	49388
#define	D2	52325
#define	Dis2	55437
#define	E2	58733
#define	F2	62226



// 14 sec = 14000 ms = 192 unit
// 1/8 = 2 unit
unsigned int tune[2*138] = 
{
// frequency, duration
	// 1  (15)
	E1, 4, E1, 2, E1, 2, Dis1, 2, E1, 2,
	C2, 4, G1, 2, G1, 4, E1, 2,
	F1, 4, E1, 2, F1, 4, G1, 2,
	A1, 10, F1, 2,
	// 2  (15)
	D1, 4, D1, 2, D1, 2, Cis1, 2, D1, 2,
	H1, 4, A1, 2, A1, 4, F1, 2,
	E1, 4, Dis1, 2, E1, 4, F1, 2,
	G1, 10, G1, 2,
	// 3  (15)
	E1, 4,E1, 2,E1, 2,Dis1, 2,E1, 2,
	E2, 4,C2, 2,C2, 4,G1, 2,
	Fis1, 4,D2, 2,D2, 4,D2, 2,
	D2, 10, C2, 2,
	// 4  (16)
	H1, 4,D2, 2,D2, 2,Cis2, 2,D2, 2,
	A1, 4,D2, 2,D2, 2,Cis2, 2,D2, 2,
	G1, 4,Fis1, 2,G1, 4,Fis1, 2,
	G1, 6, F1, 6,
	// 5  (10)
	E1, 2,Dis1, 2,E1, 2,A1, 4,G1, 2,
	E1, 6,C1, 6,
	A0, 6,D1, 6,
	C1, 10,
	//6   (11)
	D1, 2,Cis1, 2,D1, 2,H1, 4,A1, 2,
	G1, 6,C2, 6,
	H1, 6,A1, 6,
	G1, 10, G1, 2,
	// 7   (13)
	A1, 4,A1, 2,A1, 2,Gis1, 2,A1, 2, 
	H1, 6,H1, 6,
	C2, 4,C2, 2,D2, 4,D2, 2,
	E2, 10, E2, 2,
	// 8  (11)
	D2, 4,C2, 2,A1, 4,F1, 2,
	E1, 6,C1, 6,
	D1, 6,H0, 6,
	C1, 4,C1, 2, D1, 6,
	// 9   (16)
	G1, 4,G1, 2,Fis1, 2,G1, 2,A1, 2,
	G1, 4,G1, 2,G1, 4,D1, 2,
	H1, 4,H1, 2,A1, 2,H1, 2,C2, 2,
	H1, 10, H1, 2,
	// 10  (16) 
	C2, 4,A1, 2,Fis1, 4,E2, 2,
	D2, 4,C2, 2,H1, 4,D2, 2,
	D2, 2,E2, 2,D2, 2,C2, 2,H1, 2,A1, 2,
	G1, 4,NUL, 6
}
/*
		D2, 4,C2, 2,A1, 4,F1, 2,
	E1, 6,C2, 6,
	D1, 6,H1, 6,
	C2, 4,C2, 2, D1, 6,
	*/
int main(void)
{
  unsigned int i = 0;
  
  bit_open();
  
  while (1)
  {
    /* Real frequency !  Duration is in s(72) */
    bit_frequency((float)tune[i+1] * 72 / 1000, (float)tune[i] / 100);

printf('%d, ',i);

    i += 2;

    if (i >= 2*(138-32)) 
    { 
	  i = 0;
	}
  }
  
  bit_close();
}
