#include <stdio.h>

int main(void)
{
    // Function Prototype
    void SwapNumbers(int, int);

    //Variable declartions
    int a_PRJ, b_PRJ;

    // code
    printf("\n\n");
    printf("Enter Value For 'A' :");
    scanf("%d", &a_PRJ);

    printf("\n\n");
    printf("Enter Value For 'B' : ");
    scanf("%d", &b_PRJ);

    printf("\n\n");
    printf("****BEFORE SWAPPING****\n\n");
    printf("Value of 'A' = %d \n\n", a_PRJ);
    printf("Value of 'B' = %d \n\n", b_PRJ);

    SwapNumbers(a_PRJ, b_PRJ); // *** Argument Passed By Value ***

    printf("\n\n");
    printf("****AFTER SWAPPING****\n\n");
    printf("Value of 'A' = %d \n\n", a_PRJ);
    printf("Value of 'B' = %d \n\n", b_PRJ);

    return (0);
}

void SwapNumbers(int x, int y)
{
    // Variable declartions
    int temp;

    //code
    printf("\n\n");
    printf("*** BEFORE SWAPPING ***\n\n");
    printf("Value of 'X' = %d \n\n", x);
    printf("Value of 'Y' = %d \n\n", y);

    temp = x;
    x = y;
    y = temp;

    printf("\n\n");
    printf("****AFTER SWAPPING****\n\n");
    printf("Value of 'X' = %d \n\n", x);
    printf("Value of 'Y' = %d \n\n", y);
}
