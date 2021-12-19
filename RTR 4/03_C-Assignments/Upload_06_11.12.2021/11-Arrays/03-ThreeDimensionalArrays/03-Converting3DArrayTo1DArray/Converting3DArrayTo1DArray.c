#include <stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3
#define DEPTH 2

int main(void)
{

    //*** variable declartions ***
    //IN Line Initialization
    int iArray_PRJ[NUM_ROWS][NUM_COLUMNS][DEPTH] = {
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

    int iArray_1D_PRJ[NUM_ROWS * NUM_COLUMNS * DEPTH];

    int int_size_PRJ;
    int iArray_size_PRJ;
    int iArray_num_elements_PRJ, iArray_width_PRJ, iArray_height_PRJ, iArray_depth_PRJ;
    int i, j, k;

    // code
    // *** DISPLAY 3D ARRAY ***
    printf("\n\n");
    printf("Elements In The 3D Array : \n\n");

    for (i = 0; i < NUM_ROWS; i++)
    {
        printf(" *** ROW %d ***\n", (i + 1));
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("*** COLUMN ***%d\n\n", (j + 1));
            for (k = 0; k < DEPTH; k++)
            {
                printf("iArray_PRJ[%d][%d][%d] = %d\n", i, j, k, iArray_PRJ[i][j][k]);
            }
            printf("\n");
        }
        printf("\n\n");
    }

    // *** CONVERTING 3D ARRAY TO 1D Array

    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            for (k = 0; k < DEPTH; k++)
            {
                iArray_1D_PRJ[(i * NUM_COLUMNS * DEPTH) + (j * DEPTH) + k] = iArray_PRJ[i][j][k];
            }
        }
    }

    //*** Display 1D Array ***
    printf("\n\n\n");
    printf("Elements In The 1D Array :\n\n");
    for (i = 0; i < (NUM_ROWS * NUM_COLUMNS * DEPTH); i++)
    {
        printf("iArray_1D[%d] = %d\n", i, iArray_1D_PRJ[i]);
    }

    return (0);
}
