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

    int iStrLenght[10];

    int strArray_size_PRJ;
    int strArray_num_elements_PRJ, strArray_num_rows_PRJ;
    int strActaul_num_chars_PRJ = 0;
    int i = 0;

    //code
    printf("\n\n");

    //code
    strArray_size_PRJ = sizeof(strArray);

    strArray_num_rows_PRJ = strArray_size_PRJ / sizeof(strArray[0]);

    //string lenghths of all the strings...
    for (i = 0; i < strArray_num_rows_PRJ; i++)
        iStrLenght[i] = MyStrlen(strArray[i]);

    printf("\n\n");
    printf("The Entire String Array : \n\n");
    for (i = 0; i < strArray_num_rows_PRJ; i++)
    {
        printf("%s ", strArray[i]);
    }

    printf("\n\n");
    printf("String In The 2D Array : \n\n");

    for (i = 0; i < strArray_num_rows_PRJ; i++)
    {
        printf("String Number %d => %s \n\n", (i + 1), strArray[i]);
        for (int j = 0; j < iStrLenght[i]; j++)
        {
            printf("Character %d = %c \n", (j + 1), strArray[i][j]);
        }
        printf("\n\n");
    };

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
