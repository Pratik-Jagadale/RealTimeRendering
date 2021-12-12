#include <stdio.h>

int main(void)
{
    //function prototypes OR declaratio s
    int MyAddition(int, int);

    //variable declarations
    int r_PRJ;
    int num_01_PRJ, num_02_PRJ;
    int num_03_PRJ, num_04_PRJ;

    //code
    num_01_PRJ = 10;
    num_02_PRJ = 20;
    num_03_PRJ = 30;
    num_04_PRJ = 40;

    r_PRJ = MyAddition(MyAddition(num_01_PRJ, num_02_PRJ), MyAddition(num_03_PRJ, num_04_PRJ));

    printf("\n\n");
    printf("%d + %d + %d + %d  = %d ", num_01_PRJ, num_02_PRJ, num_03_PRJ, num_04_PRJ, r_PRJ);
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