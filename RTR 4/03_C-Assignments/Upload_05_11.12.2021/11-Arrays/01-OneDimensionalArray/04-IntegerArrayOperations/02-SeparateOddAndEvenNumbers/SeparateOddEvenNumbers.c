#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    //variable declartions
    int iArray_PRJ[NUM_ELEMENTS];
    int i, num_PRJ, sum_PRJ = 0;

    //code

    //Array Elemet Inputs
    printf("Enter Integer Element For Array : \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &num_PRJ);
        iArray_PRJ[i] = num_PRJ;
    }

    //SEPARATING OUT EVEN NUMBERS FROM ARRAY ELEMENTS
    printf("\n\n");
    printf("Even Numbers Amongst The Array Elements Are: \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        if ((iArray_PRJ[i] % 2) == 0)
            printf("%d\n", iArray_PRJ[i]);
    }

    //SEPARATING OUT ODD NUMBERS FROM ARRAY ELEMENTS
    printf("\n\n");
    printf("Odd Numbers Amongst The Array Elements Are: \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        if ((iArray_PRJ[i] % 2) != 0)
            printf("%d\n", iArray_PRJ[i]);
    }

    return (0);
}
