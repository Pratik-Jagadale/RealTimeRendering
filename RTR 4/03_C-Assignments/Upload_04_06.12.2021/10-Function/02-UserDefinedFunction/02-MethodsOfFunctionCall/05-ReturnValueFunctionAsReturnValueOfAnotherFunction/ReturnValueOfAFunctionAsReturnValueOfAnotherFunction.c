#include <stdio.h>

int main(void)
{
    //function prototypes OR declaratio s
    int MyAddition(int, int);

    //variable declarations
    int num_01_PRJ, num_02_PRJ;

    //code
    num_01_PRJ = 10;
    num_02_PRJ = 20;

    printf("\n\n");
    printf("%d + %d = %d ", num_01_PRJ, num_02_PRJ, MyAddition(num_01_PRJ, num_02_PRJ));
    printf("\n\n");

    return (0);
}

int MyAddition(int a_PRJ, int b_PRJ)
{
    //variable declartion
    int sum;

    //code
    sum = a_PRJ + b_PRJ;
    return (sum);
}