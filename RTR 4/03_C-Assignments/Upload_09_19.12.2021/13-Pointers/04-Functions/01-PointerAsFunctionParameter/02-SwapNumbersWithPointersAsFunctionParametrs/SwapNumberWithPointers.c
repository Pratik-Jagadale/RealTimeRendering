#include <stdio.h>

int main(void)
{
    // Function Prototype
    void SwapNumbers(int *, int *);

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

    SwapNumbers(&a_PRJ, &b_PRJ); // *** Argument Passed By References / Address ***

    printf("\n\n");
    printf("****AFTER SWAPPING****\n\n");
    printf("Value of 'A' = %d \n\n", a_PRJ);
    printf("Value of 'B' = %d \n\n", b_PRJ);

    return (0);
}

/* Address Of 'a_RPJ' is copied into 'x' and Address of 'b_PRJ' is copied into 'y' ... So,
'&a' and 'x' are pointing to ONE and the SAME address and '&b' and 'y' are 
pointing to ONE and the SAME address ...
// Swapping takes place between 'value at address of x' (value at &a i.e : 'a') 
and 'value at address of y' (value at &b i.e : 'b') ...
// Hence, Swapping in this case takes place between '*x' and '*y' AS WELL AS 
BETWEEN 'a' and 'b' ... */

void SwapNumbers(int *x, int *y)
{
    // Variable declartions
    int temp;

    //code
    printf("\n\n");
    printf("*** BEFORE SWAPPING ***\n\n");
    printf("Value of 'X' = %d \n\n", *x);
    printf("Value of 'Y' = %d \n\n", *y);

    temp = *x;
    *x = *y;
    *y = temp;

    printf("\n\n");
    printf("****AFTER SWAPPING****\n\n");
    printf("Value of 'X' = %d \n\n", *x);
    printf("Value of 'Y' = %d \n\n", *y);
}
