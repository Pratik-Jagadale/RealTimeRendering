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

    // ROW 1
    printf("*******ROW*******\n");
    printf("iArray[0][0] = %d \n", iArray_PRJ[0][0]); //COLUMN ONE O'TH ELEMENT
    printf("iArray[0][1] = %d \n", iArray_PRJ[0][1]); //COLUMN TWO 1'ST ELEMENT
    printf("iArray[0][2] = %d \n", iArray_PRJ[0][2]); //COLUMN THREE 2 nd ELEMENT

    printf("\n\n");

    /* Row */
    printf("*** Row2 ***\n");
    printf("iArray[1][0] = %d\n", iArray_PRJ[1][0]);  //Column 1 (0 th Element)
    printf("iArray[1][1] = %d \n", iArray_PRJ[1][1]); //COLUMN TWO 1'ST ELEMENT
    printf("iArray[1][2] = %d \n", iArray_PRJ[1][2]); //COLUMN THREE 2 nd ELEMENT

    printf("\n\n");

    /* Row 3 */
    printf("*** Row3 ***\n");
    printf("iArray[2][0] = %d\n", iArray_PRJ[2][0]);  //Column 1 (0 th Element)
    printf("iArray[2][1] = %d \n", iArray_PRJ[2][1]); //COLUMN TWO 1'ST ELEMENT
    printf("iArray[2][2] = %d \n", iArray_PRJ[2][2]); //COLUMN THREE 2 nd ELEMENT

    printf("\n\n");

    /* Row 4 */
    printf("*** Row4 ***\n");
    printf("iArray[3][0] = %d\n", iArray_PRJ[3][0]);  //Column 1 (0 th Element)
    printf("iArray[3][1] = %d \n", iArray_PRJ[3][1]); //COLUMN TWO 1'ST ELEMENT
    printf("iArray[3][2] = %d \n", iArray_PRJ[3][2]); //COLUMN THREE 2 nd ELEMENT

    printf("\n\n");

    /* Row 5 */
    printf("*** Row5 ***\n");
    printf("iArray[4][0] = %d\n", iArray_PRJ[4][0]);  //Column 1 (0 th Element)
    printf("iArray[4][1] = %d \n", iArray_PRJ[4][1]); //COLUMN TWO 1'ST ELEMENT
    printf("iArray[4][2] = %d \n", iArray_PRJ[4][2]); //COLUMN THREE 2 nd ELEMENT

    printf("\n\n");

    return (0);
}
