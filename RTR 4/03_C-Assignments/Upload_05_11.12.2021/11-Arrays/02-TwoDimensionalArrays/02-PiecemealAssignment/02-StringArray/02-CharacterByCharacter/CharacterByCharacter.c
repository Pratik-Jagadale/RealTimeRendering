#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //variable delcations
    char strArray[5][10];

    int char_size_PRJ;
    int strArray_size_PRJ;
    int strArray_num_elements_PRJ, strArray_num_rows_PRJ, strArray_num_Clumns_PRJ;
    int i = 0;

    //code
    printf("\n\n");

    char_size_PRJ = sizeof(char);

    strArray_size_PRJ = sizeof(strArray);
    printf("Size of Two Dimensional (2D) Array (String Arrau) Is = %d \n\n", strArray_size_PRJ);

    strArray_num_rows_PRJ = strArray_size_PRJ / sizeof(strArray[0]);
    printf("Number of Rows (String) In Two Dimensional (2D) Character Array (String Array) Is = %d\n\n", strArray_num_rows_PRJ);

    strArray_num_Clumns_PRJ = sizeof(strArray[0]) / char_size_PRJ;
    printf("Number of Columns In Two Dimensional (2D) Character Array (String Array) Is = %d\n\n", strArray_num_Clumns_PRJ);

    strArray_num_elements_PRJ = strArray_num_rows_PRJ * strArray_num_Clumns_PRJ;
    printf("Maximum Number of Elements (Characters) In Two Dimensional (2D) Character Array (String Array) Is = %d \n\n", strArray_num_elements_PRJ);

    //*** PEACEMEAL ASSIGNEMENT ***
    //*** ROW 1/ STRING ***
    strArray[0][0] = 'M';
    strArray[0][1] = 'y';
    strArray[0][2] = '\0';

    //*** ROW2 / STRING ***
    strArray[1][0] = 'N';
    strArray[1][1] = 'a';
    strArray[1][2] = 'm';
    strArray[1][3] = 'e';
    strArray[1][4] = '\0';

    //*** ROW 3 / STRING ***
    strArray[2][0] = 'I';
    strArray[2][1] = 'S';
    strArray[2][2] = '\0';

    //*** ROW 4 / STRING ***
    strArray[3][0] = 'P';
    strArray[3][1] = 'r';
    strArray[3][2] = 'a';
    strArray[3][3] = 't';
    strArray[3][4] = 'i';
    strArray[3][5] = 'k';
    strArray[3][6] = '\0';

    //*** ROW 5 / STRING ***
    strArray[4][0] = 'J';
    strArray[4][1] = 'a';
    strArray[4][2] = 'g';
    strArray[4][3] = 'a';
    strArray[4][4] = 'd';
    strArray[4][5] = 'a';
    strArray[4][6] = 'l';
    strArray[4][7] = 'e';
    strArray[4][8] = '\0';

    printf("\n\n");
    printf("The String in the 2D Character Array Are : \n\n");
    for (i = 0; i < strArray_num_rows_PRJ; i++)
    {
        printf("%s ", strArray[i]);
    }

    printf("\n\n");
    return (0);
}
