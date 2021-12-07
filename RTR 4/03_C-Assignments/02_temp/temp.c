#include <stdio.h>

/* constant using # define macro */
#define PRJ_PI	3.14	/* idetifers */

/* constant using enumeration */
/* by defualt it gives values from 0 to onwards to all idetifers we can change it */
enum {
	WHITE = 1,	/* idetifers */
	GRAY,		/* idetifers */
	BLACK		/* idetifers */
};

enum {
	ONE,		/* 0 */
	TWO,		/* 1 */
	THREE,		/* 2 */
	FOUR,		/* 3 */
	FIVE  = 5,	/* 5 */
	SIX,		/* 6 */
	SEVEN,		/* 7 */
	EIGHT,		/* 8 */
	NINE		/* 9 */
};

/* entry point function  */
int main(void)
{
	/* code */
	/* any idetifer must do not use before initialization */

	int a;		/* variable declartion,added into data section in bss(assembly) */
	a = 5;		/* varibale initialization */

	int b = 10; /* varibale declartion and initialization */

	const int c = 20; /* constant intiger variable declartion and initialization */
	
	printf("value of a is %d \n",a);

	printf("value of PRJ_PI is %f \n", PRJ_PI);

	printf("value of enum WHITE is %d \n", WHITE);
	printf("value of enum GRAY is %d \n", GRAY);
	printf("value of enum BLACK is %d \n", BLACK);

	return (0);
}

/* 
	values that which can given to computer 
	1.variable 
	2.constant
		-using const specifer.
		-using # define macro.
		-using enum.
*/

/*	HOME WORK	*/
/*
1. find 0 to 15 binnary.
2. The c programming Language 2nd edition.
*/
