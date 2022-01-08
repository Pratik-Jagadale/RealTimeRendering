#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS_ONE 3
#define NUM_COLUMNS_TWO 8

int main(void)
{
    // Variable declaerations
    int *iArray_PRJ[NUM_ROWS];
    int i, j;

    // code

    printf("\n\n");
    printf("----------------FIRST MEMORY ALLOCATION TO 2D INTEGER ARRAYc ----------------- \n\n");

    for (i = 0; i < NUM_ROWS; i++)
    {
        iArray_PRJ[i] = (int *)malloc(NUM_COLUMNS_ONE * sizeof(int)); // A 2D Array which will have 5 rows and number of
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
        for (j = 0; j < NUM_COLUMNS_ONE; j++)
        {
            iArray_PRJ[i][j] = (i + 1) * (j + 1);
        }
    }

    // DISPLYAING 2D ARRAY ...
    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS_ONE; j++)
        {
            printf("iArray_PRJ[%d][%d] = %d \t\t At Addresses : %p\n", i, j, iArray_PRJ[i][j], &iArray_PRJ[i][j]);
        }
        printf("\n\n");
    }

    // FREEING MEMORY ASSIGNED TO 2D ARRAY (MUST BE DONE IN REVERSE ORDER)
    for (i = (NUM_ROWS - 1); i >= 0; i--)
    {
        free(iArray_PRJ[i]);
        iArray_PRJ[i] = NULL;
        printf("MEMOEY ALLOCATED TO ROW %d OF 2D INTEGER ARRAYS HAS BEEN SUCCESSFULLY FREED !!!\n\n", i);
    }

    //******** TWO (ALLOCATING MEMORY FOR AN ARRAY OF 8 INTEGERS ARRAY *************)
    printf("\n\n");
    printf("----------------SECOND MEMORY ALLOCATION TO 2D INTEGER ARRAYc ----------------- \n\n");

    for (i = 0; i < NUM_ROWS; i++)
    {
        iArray_PRJ[i] = (int *)malloc(NUM_COLUMNS_TWO * sizeof(int)); // A 2D Array which will have 5 rows and number of
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
        for (j = 0; j < NUM_COLUMNS_TWO; j++)
        {
            iArray_PRJ[i][j] = (i + 1) * (j + 1);
        }
    }

    // DISPLYAING 2D ARRAY ...
    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS_TWO; j++)
        {
            printf("iArray_PRJ[%d][%d] = %d \t\t At Addresses : %p\n", i, j, iArray_PRJ[i][j], &iArray_PRJ[i][j]);
        }
        printf("\n\n");
    }

    // FREEING MEMORY ASSIGNED TO 2D ARRAY (MUST BE DONE IN REVERSE ORDER)
    for (i = (NUM_ROWS - 1); i >= 0; i--)
    {
        free(iArray_PRJ[i]);
        iArray_PRJ[i] = NULL;
        printf("MEMOEY ALLOCATED TO ROW %d OF 2D INTEGER ARRAYS HAS BEEN SUCCESSFULLY FREED !!!\n\n", i);
    }

    return (0);
}
