#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    //variable declartions
    int iArray[NUM_ELEMENTS];
    int i, num_PRJ, sum_PRJ = 0;

    //code
    printf("\n\n");
    printf("Enter Integer Elemets For Array : \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &num_PRJ);
        iArray[i] = num_PRJ;
    }

    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        sum_PRJ = sum_PRJ + iArray[i];
    }

    printf("\n\n");
    printf("Sum Of All Elements Of Array = %d \n\n", sum_PRJ);

    return (0);
}
