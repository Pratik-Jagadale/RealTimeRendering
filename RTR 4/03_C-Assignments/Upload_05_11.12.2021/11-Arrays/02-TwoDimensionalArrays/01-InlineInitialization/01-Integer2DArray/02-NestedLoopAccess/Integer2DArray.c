#include <stdio.h>

int main(void)
{
    //variable declartions
    int iArray_PRJ[5][3] = {
        {1, 2, 3},
        {2, 4, 6},
        {3, 6, 9},
        {4, 8, 12},
        {5, 40, 15}}; //inline initialization
    int int_size_PRJ;
    int iArray_size_PRJ;
    int iArray_num_elements_PRJ, iArray_num_rows_PRJ, iArray_num_colums_PRJ;

    // code

    printf("-----------------------------------------------------");

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

    for (int i = 0; i < iArray_num_rows_PRJ; i++)
    {
        for (int j = 0; j < iArray_num_colums_PRJ; j++)
        {
            printf("iArray[%d][%d] = %d \n", i, j, iArray_PRJ[i][j]); //COLUMN ONE O'TH ELEMENT
        }
        printf("\n\n");
    }

    printf("-----------------------------------------------------");

    return (0);
}
