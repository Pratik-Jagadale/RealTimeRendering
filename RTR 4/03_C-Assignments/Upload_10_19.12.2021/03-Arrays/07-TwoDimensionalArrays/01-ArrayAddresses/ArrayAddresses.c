#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    // Variable declaerations
    int iArray_PRJ[NUM_COLUMNS][NUM_ROWS];
    int i, j;

    // code

    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            iArray_PRJ[i][j] = (i + 1) * (j + 1);
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("iArray_PRJ[%d][%d] = %d \t\t At Addresses : %p\n", i, j, iArray_PRJ[i][j], &iArray_PRJ[i][j]);
        }
        printf("\n\n");
    }
    return (0);
}
