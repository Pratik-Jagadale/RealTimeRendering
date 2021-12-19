#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion prototype
    int MyStrlen(char[]);

    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    int iStringLength_PRJ = 0;

    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("String Entered By You Is : \n\n %s \n\n", chArray_PRJ);

    // *** STRING LENGHT ***
    printf("\n\n");
    iStringLength_PRJ = MyStrlen(chArray_PRJ);
    printf("Length Of String Is = %d Characters !!!\n\n", iStringLength_PRJ);

    return (0);
}

int MyStrlen(char str_PRJ[])
{
    //variable declartion
    int i;
    int string_length_PRJ = 0;

    //code
    for (i = 0; i < MAX_STRING_LENGTH; i++)
    {
        if (str_PRJ[i] == '\0')
            break;
        string_length_PRJ++;
    }
    return (string_length_PRJ);
}
