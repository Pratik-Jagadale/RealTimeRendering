#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    // Variable declaerations
    int iArray_PRJ[NUM_ROWS][NUM_COLUMNS];
    int i, j;

    // code
    /*
        // *** NAME OF AN ARRAY ITSELF IS ITS BASE ADDRESS ***
        // *** HENCE, 'iArray' IS BASE ADDRESS OF 2D INTEGER ARRAY iArray[][]
        // iArray[5][3] => iArray IS A 2D ARRAY HAVING 5 ROWS AND 3 COLUMNS. EACH 
            OF THESE 5 ROWS IS A 1D INTGER ARRAY OF 3 INTEGERS ***
        // iArray[0] => IS THE 0TH ROW ... HENCE, IS THE BASE ADDRESS OF ROW 0 
    
        iArray[0], iArray[1] ... ARE 1D INTEGR ARRAYS AND HENCE CAN BE TREATED 
            AS 1D INTEGER ARRAYS USING POINTERS ...
        // 'iArray' IS THE NAME AND BASE ADDRESS OF 2D INTEGER ARRAY ***
        // (*(iArray + 0) + 0) = (iArray[0] + 0) = ADDRESS OF 0TH ELEMENT FROM BASE
            ADDRESS OF 0TH ROW = (iArray[0] + 0) = (iArray[0][0])
    */
        {

    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
            *(*(iArray_PRJ + i) + j) = (i + 1) * (j + 1); // 'iArray[][]' Can be treated as As 1D Array using Pointers...
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("*(*(iArray_PRJ +%d )+ %d) = %d \t\t At Addresses : %p\n", i, j, *(*(iArray_PRJ + i) + j), (*(iArray_PRJ + i) + j));
        }
        printf("\n\n");
    }
    return (0);
}
