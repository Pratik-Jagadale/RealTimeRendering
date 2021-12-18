#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    //variable declartions
    int iArray_PRJ[NUM_ELEMENTS];
    int i, j, num_PRJ, count_RPJ = 0;

    //code

    //Array Elemet Inputs
    printf("Enter Integer Element For Array : \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &num_PRJ);

        if (num_PRJ < 0)
            num_PRJ = -1 * num_PRJ;

        iArray_PRJ[i] = num_PRJ;
    }

    //PRINTING ENTIRE ARRAY
    printf("\n\n");
    printf("Array Elements Are \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        printf("%d\n", iArray_PRJ[i]);
    }

    //SEPARATING OUT EVEN NUMBERS FROM ARRAY ELEMENTS
    printf("\n\n");
    printf("Prime Numbers Amongst The Array Elements Are: \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        for (j = 2; j <= (iArray_PRJ[i]) / 2; j++)
        {
            if ((iArray_PRJ[i] % j) == 0)
                count_RPJ++;
        }

        if (count_RPJ == 0)
            printf("%d\n", iArray_PRJ[i]);

        count_RPJ = 0;
    }

    return (0);
}
