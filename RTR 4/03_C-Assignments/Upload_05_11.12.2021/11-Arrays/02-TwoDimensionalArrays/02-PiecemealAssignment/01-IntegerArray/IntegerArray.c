#include <stdio.h>

int main(void)
{
    //variable declartions
    int iArray_PRJ[3][5];
    int int_size_PRJ;
    int iArray_size_PRJ;
    int iArray_num_elements_PRJ, iArray_num_rows_PRJ, iArray_num_colums_PRJ;
    int i, j;

    // code
    printf("\n\n");

    int_size_PRJ = sizeof(int);

    iArray_size_PRJ = sizeof(iArray_PRJ);
    printf("Size Of Two Dimensional ( 2D ) Integer Array Is = %d \n\n", iArray_size_PRJ);

    iArray_num_rows_PRJ = iArray_size_PRJ / sizeof(iArray_PRJ[0]);
    printf("Number of Rows In Two Dimensional ( 2D )  Integer Array is = %d\n\n", iArray_num_rows_PRJ);

    iArray_num_colums_PRJ = sizeof(iArray_PRJ[0]) / int_size_PRJ;
    printf("Number of Rows In Two Dimensional ( 2D )  Integer Array is = %d\n\n", iArray_num_colums_PRJ);

    iArray_num_elements_PRJ = iArray_num_rows_PRJ * iArray_num_colums_PRJ;
    printf("Number of Elements In Two Dimensional ( 2D ) Integer Array Is = %d\n", iArray_num_elements_PRJ);

    printf("\n\n");
    printf("Elements In The 2D Arrayu : \n\n");

    // Array Indices Begin From 0 , Hence, 1st ROW is Actually 0 th Row and 1st Column is Actually 0th Column

    //PIECE MEAL ASSIGNEMENT

    // ROW 1
    iArray_PRJ[0][0] = 21;
    iArray_PRJ[0][1] = 42;
    iArray_PRJ[0][2] = 63;
    iArray_PRJ[0][3] = 84;
    iArray_PRJ[0][4] = 105;

    // ROW 2
    iArray_PRJ[1][0] = 22;
    iArray_PRJ[1][1] = 44;
    iArray_PRJ[1][2] = 66;
    iArray_PRJ[1][3] = 88;
    iArray_PRJ[1][4] = 110;

    // ROW 3
    iArray_PRJ[2][0] = 23;
    iArray_PRJ[2][1] = 46;
    iArray_PRJ[2][2] = 69;
    iArray_PRJ[2][3] = 92;
    iArray_PRJ[2][4] = 115;

    /***  Display ***/
    for (i = 0; i < iArray_num_rows_PRJ; i++)
    {
        printf("***** ROW %d *****\n", (i + 1));
        for (j = 0; j < iArray_num_colums_PRJ; j++)
        {
            printf("iArray[%d][%d] = %d \n", i, j, iArray_PRJ[i][j]);
        }
        printf("\n\n");
    }

    return (0);
}
