#include <stdio.h>

int main(void)
{
    //variable declartions

    //IN Line Initialization
    int iArray_ONE_PRJ[7];
    int iArray_TWo_PRJ[7][4];
    int iArray_Three_PRJ[7][4][5];

    int num_row_2D_PRJ;
    int num_Columns_2D_PRJ;

    int num_rows_3D_PRJ;
    int num_Columns_3D_PRJ;
    int depth_3D;

    int int_size_PRJ;

    // code
    printf("\n\n");

    printf("Size Of One Dimensional ( 1D ) Integer Array Is = %lu \n\n", sizeof(iArray_ONE_PRJ));
    printf("Number Of Elements in 1-D integer Array iArray_ONE_PRJ =%lu \n\n", sizeof(iArray_ONE_PRJ) / sizeof(int));

    printf("\n\n");

    printf("Size Of Two Dimensional ( 2D ) Integer Array Is = %lu \n\n", sizeof(iArray_TWo_PRJ));

    printf("Number of Rows in Two Dimensional ( 2D ) Integer Array Is = %lu \n\n", sizeof(iArray_TWo_PRJ) / sizeof(iArray_TWo_PRJ[0]));
    num_row_2D_PRJ = sizeof(iArray_TWo_PRJ) / sizeof(iArray_TWo_PRJ[0]);

    printf("Number of elements (Columns) in each row in Two Dimensional ( 2D ) Integer Array Is = %lu \n\n", sizeof(iArray_TWo_PRJ[0]) / sizeof(iArray_TWo_PRJ[0][0]));
    num_Columns_2D_PRJ = sizeof(iArray_TWo_PRJ[0]) / sizeof(iArray_TWo_PRJ[0][0]);

    printf("Number Of Elements in 2-D integer Array iArray_Two_PRJ =%lu \n", num_Columns_2D_PRJ * num_row_2D_PRJ);

    printf("\n\n");

    printf("Size Of Three Dimensional ( 3D ) Integer Array Is = %lu \n\n", sizeof(iArray_Three_PRJ));

    printf("Number of Rows in Three Dimensional ( 3D ) Integer Array Is = %lu \n\n", sizeof(iArray_Three_PRJ) / sizeof(iArray_Three_PRJ[0]));
    num_rows_3D_PRJ = sizeof(iArray_Three_PRJ) / sizeof(iArray_Three_PRJ[0]);

    printf("Number of elements (Columns) in each row in Three Dimensional ( 3D ) Integer Array Is = %lu \n\n", sizeof(iArray_Three_PRJ[0]) / sizeof(iArray_Three_PRJ[0][0]));
    num_Columns_3D_PRJ = sizeof(iArray_Three_PRJ[0]) / sizeof(iArray_Three_PRJ[0][0]);

    printf("Number of elements (Depth) in each Column in Three Dimensional ( 3D ) Integer Array Is = %lu \n\n", sizeof(iArray_Three_PRJ[0][0]) / sizeof(iArray_Three_PRJ[0][0][0]));
    depth_3D = sizeof(iArray_Three_PRJ[0][0]) / sizeof(iArray_Three_PRJ[0][0][0]);

    printf("Number Of Elements in 3-D integer Array iArray_Two_PRJ =%lu \n", num_rows_3D_PRJ * num_Columns_3D_PRJ * depth_3D);

    printf("\n\n");

    return (0);
}
