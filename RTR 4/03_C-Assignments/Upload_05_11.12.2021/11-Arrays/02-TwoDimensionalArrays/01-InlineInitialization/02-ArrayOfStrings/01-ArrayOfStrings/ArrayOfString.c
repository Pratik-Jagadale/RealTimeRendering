#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    int MyStrlen(char[]);

    //  A 'STRING' IS AN ARRAY OF CHARACTERS ... so char[] IS A char ARRAY AND ENCE, char[] IS A 'STRING'
    //  AN ARRAY OF char ARRAYS IS AN ARRAY OF STRINGS
    //  HENCE, char[] IS ONE char ARRAY AND HENCE, IS ONE STRING
    //  HENCE, char[][] IS AN ARRAY OF char ARRAYS AND HENCE, IS AN ARRAY OFSTRINGS
    //Here, the string array can allow a maximum number of 10 strings (10 rows)
    //and each of these 10 strings can have only upto 15 characters maximum(15 columns)

    char strArray[10][15] = {"Hello..", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2021-22)", "Of", "ASTROMEDICOMP."};

    int char_size_PRJ;
    int strArray_size_PRJ;
    int strArray_num_elements_PRJ, strArray_num_rows_PRJ, strArray_num_Clumns_PRJ;
    int strActaul_num_chars_PRJ = 0;
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

    for (i = 0; i < strArray_num_rows_PRJ; i++)
    {
        strActaul_num_chars_PRJ = strActaul_num_chars_PRJ + MyStrlen(strArray[i]);
    }

    printf("Actaul Number of Elemets (Charavcters) In Two Dimensional (2D) Character Array (String Array) IS = %d\n\n", strActaul_num_chars_PRJ);

    printf("\n\n");
    printf("String In The 2D Array : \n\n");

    printf("%s ", strArray[0]);
    printf("%s ", strArray[1]);
    printf("%s ", strArray[2]);
    printf("%s ", strArray[3]);
    printf("%s ", strArray[4]);
    printf("%s ", strArray[5]);
    printf("%s ", strArray[6]);
    printf("%s ", strArray[7]);
    printf("%s ", strArray[8]);
    printf("%s ", strArray[9]);

    return (0);
}

int MyStrlen(char str[])
{
    int j;
    int string_length_PRJ = 0;

    for (j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if (str[j] == '\0')
            break;
        else
            string_length_PRJ++;
    }
    return (string_length_PRJ);
}
