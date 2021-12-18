#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    void MyStrcpy(char[], char *);

    //  A 'STRING' IS AN ARRAY OF CHARACTERS ... so char[] IS A char ARRAY AND ENCE, char[] IS A 'STRING'
    //  AN ARRAY OF char ARRAYS IS AN ARRAY OF STRINGS
    //  HENCE, char[] IS ONE char ARRAY AND HENCE, IS ONE STRING
    //  HENCE, char[][] IS AN ARRAY OF char ARRAYS AND HENCE, IS AN ARRAY OFSTRINGS
    //Here, the string array can allow a maximum number of 10 strings (10 rows)
    //and each of these 10 strings can have only upto 15 characters maximum(15 columns)

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
    MyStrcpy("My", strArray[0]);
    MyStrcpy("Name", strArray[1]);
    MyStrcpy("Is", strArray[2]);
    MyStrcpy("Pratik", strArray[3]);
    MyStrcpy("Jagadle", strArray[4]);

    printf("\n\n");
    printf("The String in the 2D Character Array Are : \n\n");
    for (i = 0; i < strArray_num_rows_PRJ; i++)
    {
        printf("%s ", strArray[i]);
    }

    printf("\n\n");
    return (0);
}

void MyStrcpy(char str_source[], char *str_destination)
{
    //function declartions
    int MyStrlen(char[]);

    //variable declartion
    int iStringLength = 0;
    int j;

    //code
    iStringLength = MyStrlen(str_source);
    for (j = 0; j < iStringLength; j++)
        str_destination[j] = str_source[j];

    str_destination[j] = '\0';
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
