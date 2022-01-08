#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    // Variable declaerations
    int iArray_PRJ[NUM_COLUMNS][NUM_ROWS];
    int i, j;

    int *ptr_iArray_Row_PRJ = NULL;

    // code
    /*
        // *** EVERY ROW OF A 2D ARRAY IS AN INTEGER ARRAY ITSELF COMPRISING OF 
        'NUM_COLUMNS' INTEGER ELEMENTS ***
        // *** THERE ARE 5 ROWS AND 3 COLUMNS IN A 2D INTEGER ARRAY. EACH OF THE 5 
        ROWS IS A 1D ARRAY OF 3 INTEGERS. 
        // *** HENCE, EACH OF THESE 5 ROWS THEMSELVES BEING ARRAYS, WILL BE THE 
        BASE ADDRESSES OF THEIR RESPECTIVE ROWS ***
    */

    for (i = 0; i < NUM_ROWS; i++)
    {
        ptr_iArray_Row_PRJ = iArray_PRJ[i]; // 'iArray[i] Is the Base Address of iTh ROW...
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            *(ptr_iArray_Row_PRJ + j) = (i + 1) * (j + 1);
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        ptr_iArray_Row_PRJ = iArray_PRJ[i];
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("*(ptr_iArray_PRJ %d) = %d \t\t At Addresses (ptr_iArray_PRJ + j) : %p\n", j, *(ptr_iArray_Row_PRJ + j), (ptr_iArray_Row_PRJ + j));
        }
        printf("\n\n");
    }

    return (0);
}
