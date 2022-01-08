#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMN 3

int main(void)
{
    // Variable declartions
    int i, j;
    int **ptr_iArray_PRJ = NULL;

    // code
    /*
        - Every Row of a 2D Array is An Integer Array Itself Comprising of NUM_COLUMN
          INTEGER Elements
        - There Are 5 Rows And 3 Columns in a 2D Integer Array,  Each Of The
          5 Rows is a 1D Array of 3 integers.
        - Hence,Each Of These 5 Rows Themselves being arrays, will be the base 
          Addresse of Thier Respective Rows...
    */
    // MEMORY ALLOCATION
    ptr_iArray_PRJ = (int **)malloc(NUM_ROWS * sizeof(int *));

    if (ptr_iArray_PRJ == NULL)
    {
        printf("MEMORY ALLOCATION TO THE 1D ARRAY OF BASE ADDRESES OF %d ROWS FAILED !!! EXITTING NOW..\n\n", NUM_ROWS);
        exit(0);
    }
    else
    {
        printf("MEMORY ALLOCATION TO THE 1D ARRAY OF BASE ADDRESES OF %d HAS SUCCEEDED !!!\n\n", NUM_ROWS);
    }

    // ALLOCATING MEMORY TO EACH ROW

    for (i = 0; i < NUM_ROWS; i++)
    {
        ptr_iArray_PRJ[i] = (int *)malloc(NUM_COLUMN * sizeof(int));
        if (ptr_iArray_PRJ[i] == NULL)
        {
            printf("MEMORY ALLOCATION TO THE COLUMNS OF ROW %d ROWS FAILED !!! EXITTING NOW..\n\n", i);
            exit(0);
        }
        else
        {
            printf("MEMORY ALLOCATION TO THE COLUMNS OF ROW %d HAS SUCCEEDED !!!\n\n", i);
        }
    }

    // ASSIGNING VALUES
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMN; j++)
        {
            *(*(ptr_iArray_PRJ + i) + j) = (i + 1) * (j + 1);
        }
    }

    // DISPLAYING VALUES
    printf("\n\n");
    printf("2D Integer Array  Elements Along With Addresses :\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMN; j++)
        {
            printf("ptr_iArray_Row[%d][%d] = %d \t\t At Address &ptr_iArray_Row[%d][%d] : %p\n", i, j, ptr_iArray_PRJ[i][j], i, j, &ptr_iArray_PRJ[i][j]);
        }
        printf("\n\n");
    }

    //--- FREEING ALLOCATED MEMORY ---
    //--- FREEING MEMORY OF EACH ROW ---
    for (i = (NUM_ROWS - 1); i >= 0; i--)
    {
        if (*(ptr_iArray_PRJ + i))
        {
            free(*(ptr_iArray_PRJ + i));
            *(ptr_iArray_PRJ + i) = NULL;
            printf("MEMORY ALLOCATED TO ROW %d HAS BEEN SUCCESSFULLY FREED !!!\n\n", i);
        }
    }

    if (ptr_iArray_PRJ)
    {
        free(ptr_iArray_PRJ);
        ptr_iArray_PRJ = NULL;
        printf("MEMMORY ALLOCATED TO ptr_iArray HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    return (0);
}
