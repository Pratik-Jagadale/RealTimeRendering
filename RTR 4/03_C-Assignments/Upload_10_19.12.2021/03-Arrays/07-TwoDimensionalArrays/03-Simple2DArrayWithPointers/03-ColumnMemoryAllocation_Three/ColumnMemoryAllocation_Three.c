#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 5

int main(void)
{
    // Variable declaerations
    int *iArray_PRJ[NUM_ROWS];
    int i, j;

    // code
    printf("\n\n");
    printf("---------------- MEMORY ALLOCATION TO 2D INTEGER ARRAY ----------------\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        iArray_PRJ[i] = (int *)malloc((NUM_COLUMNS - i) * sizeof(int)); // A 2D Array which will have 5 rows and number of
                                                                        // columns can be decided later on ...

        if (iArray_PRJ[i] == NULL)
        {
            printf("FAILED TO ALLOCATE MEMORY TO ROW %d OF 2D INTEGER ARRAY !!! EXITTING NOW...\n\n", i);
            exit(0);
        }
        else
            printf("MEMORY ALLOCATION TO ROW %d  OF 2D ARRAY SUCCEEDED !!!\n\n", i);
    }

    //ASSIGNING VALUES TO 2D ARRAY
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < (NUM_COLUMNS - i); j++)
        {
            iArray_PRJ[i][j] = (i + 1) * (j + 1);
        }
    }

    // DISPLYAING 2D ARRAY ...
    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < (NUM_COLUMNS - i); j++)
        {
            printf("iArray_PRJ[%d][%d] = %d \t\t At Addresses : %p\n", i, j, iArray_PRJ[i][j], &iArray_PRJ[i][j]);
        }
        printf("\n\n");
    }

    // FREEING MEMORY ASSIGNED TO 2D ARRAY (MUST BE DONE IN REVERSE ORDER)
    for (i = 0; i < NUM_ROWS; i++)
    {
        free(iArray_PRJ[i]);
        iArray_PRJ[i] = NULL;
        printf("MEMOEY ALLOCATED TO ROW %d OF 2D INTEGER ARRAYS HAS BEEN SUCCESSFULLY FREED !!!\n\n", i);
        if (iArray_PRJ[i] != NULL)
            printf("FAILED\n\n");
    }
    return (0);
}
