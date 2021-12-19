#include <stdio.h>

int main(void)
{
    //variable declartions

    //IN Line Initialization
    int iArray_PRJ[5][3][2] = {
        {
            {1, 2},
            {3, 6},
            {4, 8},
        },
        {
            {5, 10},
            {6, 12},
            {7, 17},
        },
        {
            {8, 16},
            {9, 18},
            {10, 20},
        },
        {
            {11, 22},
            {12, 24},
            {13, 26},
        },
        {
            {14, 28},
            {15, 30},
            {16, 32},
        },
    };
    int int_size_PRJ;
    int iArray_size_PRJ;
    int iArray_num_elements_PRJ, iArray_width_PRJ, iArray_height_PRJ, iArray_depth_PRJ;
    int i, j, k;

    // code
    printf("\n\n");

    int_size_PRJ = sizeof(int);

    iArray_size_PRJ = sizeof(iArray_PRJ);
    printf("Size Of Two Dimensional ( 3D ) Integer Array Is = %d \n\n", iArray_size_PRJ);

    iArray_width_PRJ = iArray_size_PRJ / sizeof(iArray_PRJ[0]);
    printf("Width In Three Dimensional ( 3D )  Integer Array is = %d\n\n", iArray_width_PRJ);

    iArray_height_PRJ = sizeof(iArray_PRJ[0]) / sizeof(iArray_PRJ[0][0]);
    printf("height of Three Dimensional ( 3D )  Integer Array is = %d\n\n", iArray_height_PRJ);

    iArray_depth_PRJ = sizeof(iArray_PRJ[0][0]) / int_size_PRJ;
    printf("Depth of Three Dimensional ( 3D )  Integer Array is = %d\n\n", iArray_depth_PRJ);

    iArray_num_elements_PRJ = iArray_width_PRJ * iArray_depth_PRJ * iArray_height_PRJ;
    printf("Number of Elements In Three Dimensional ( 3D ) Integer Array Is = %d\n", iArray_num_elements_PRJ);

    printf("\n\n");
    printf("Elements In The 3D Array : \n\n");

    for (i = 0; i < iArray_width_PRJ; i++)
    {
        printf(" *** ROW %d ***\n", (i + 1));
        for (j = 0; j < iArray_height_PRJ; j++)
        {
            printf("*** COLUMN ***%d\n\n", (j + 1));
            for (k = 0; k < iArray_depth_PRJ; k++)
            {
                printf("iArray_PRJ[%d][%d][%d] = %d\n", i, j, k, iArray_PRJ[i][j][k]);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    return (0);
}
