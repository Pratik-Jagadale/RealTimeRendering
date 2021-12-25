#include <stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //variable declartion
    int iArray_2D_PRJ[NUM_ROWS][NUM_COLUMNS];

    int iArray_1D_PRJ[NUM_COLUMNS * NUM_ROWS];
    int i, j, num_PRJ;

    //code
    printf("Enter Elements Of Your Choices Tol Fill Up The Integer 2D array : \n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        printf("For ROW NUMBER %d : \n", (i + 1));
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("Enter Element Number %d : \n", (j + 1));
            scanf("%d", &num_PRJ);
            iArray_2D_PRJ[i][j] = num_PRJ;
        }
        printf("\n\n");
    }

    // *** display 2D Array ***
    printf("\n\n");
    printf("Two-Dimensional (2D) Array of Integers : \n\n");
    for (i = 0; i < NUM_COLUMNS; i++)
    {
        printf("*** ROW %d  ***\n", (i + 1));
        for (j = 0; j < NUM_ROWS; j++)
        {
            printf("iArray2D_PRJ[%d][%d] = %d\n", i, j, iArray_2D_PRJ[i][j]);
        }
        printf("\n\n");
    }

    // *** CONVERTING 2D INTEGR ARRAY TO 1D INTEGER ARRAY ***

    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            iArray_1D_PRJ[(i * NUM_COLUMNS) + j] = iArray_2D_PRJ[i][j];
        }
    }

    // *** PRINTING 1D ARRAY ***
    printf("\n\n");
    printf("One-Dimensional (1D) Array of Integer :\n\n");

    for (i = 0; i < (NUM_COLUMNS * NUM_ROWS); i++)
    {
        printf("iArray1D[%d] = %d\n", i, iArray_1D_PRJ[i]);
    }

    printf("\n\n");
    return (0);
}
