#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // Variable declaerations
    int **ptr_iArray_PRJ = NULL;
    int i, j;
    int iNumRows, iNumColumns;

    // code

    //ACCEPT NUMBER OF ROWS 'num_Rows; From User
    printf("\n\n");
    printf("Enter Number Of Rows : ");
    scanf("%d", &iNumRows);

    //ACCEPT NUMBER OF COLUMNS 'num_columns; From User
    printf("\n\n");
    printf("Enter Number Of Columns : ");
    scanf("%d", &iNumColumns);

    printf("\n\n");

    // ***  ALLOCATING MEMORY TO 1D ARRAY CONSISTING BASE ADDRESS OF ROWS ***
    ptr_iArray_PRJ = (int **)malloc(iNumRows * sizeof(int *));
    if (ptr_iArray_PRJ == NULL)
    {
        printf("FAILED TO ALLOCATE MEMORYTo %d  OF 2D INTEGER ARRAY !!! EXITTING NOW...\n\n", iNumRows);
        exit(0);
    }
    else
        printf("MEMORY ALLOCATION TO ROW %d  OF 2D ARRAY SUCCEEDED !!!\n\n", iNumRows);

    for (i = 0; i < iNumRows; i++)
    {
        ptr_iArray_PRJ[i] = (int *)malloc(iNumColumns * sizeof(int)); // A 2D Array which will have 5 rows and number of
                                                                      // columns can be decided later on ...
        if (ptr_iArray_PRJ[i] == NULL)
        {
            printf("FAILED TO ALLOCATE MEMORY TO ROW %d OF 2D INTEGER ARRAY !!! EXITTING NOW...\n\n", i);
            exit(0);
        }
        else
            printf("MEMORY ALLOCATION TO ROW %d  OF 2D ARRAY SUCCEEDED !!!\n\n", i);
    }

    //ASSIGNING VALUES TO 2D ARRAY
    for (i = 0; i < iNumRows; i++)
    {
        for (j = 0; j < iNumColumns; j++)
        {
            ptr_iArray_PRJ[i][j] = (i + 1) * (j + 1);
        }
    }

    // DISPLYAING 2D ARRAY ...
    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for (i = 0; i < iNumRows; i++)
    {
        for (j = 0; j < iNumColumns; j++)
        {
            printf("ptr_iArray_PRJ[%d][%d] = %d \t\t At Addresses : %p\n", i, j, ptr_iArray_PRJ[i][j], &ptr_iArray_PRJ[i][j]);
        }
        printf("\n\n");
    }

    // FREEING MEMORY ASSIGNED TO 2D ARRAY (MUST BE DONE IN REVERSE ORDER)
    for (i = (iNumRows - 1); i >= 0; i--)
    {
        free(ptr_iArray_PRJ[i]);
        ptr_iArray_PRJ[i] = NULL;
        printf("MEMORY ALLOCATED TO ROW %d OF 2D INTEGER ARRAYS HAS BEEN SUCCESSFULLY FREED !!!\n\n", i);
    }

    if (ptr_iArray_PRJ)
    {
        free(ptr_iArray_PRJ);
        ptr_iArray_PRJ = NULL;
        printf("MEMORY ALLOCATED TO ptr_iArray_PRJ  HAS BEEN SUCCESSFULLY FREED !!!\n\n", i);
    }

    return (0);
}
