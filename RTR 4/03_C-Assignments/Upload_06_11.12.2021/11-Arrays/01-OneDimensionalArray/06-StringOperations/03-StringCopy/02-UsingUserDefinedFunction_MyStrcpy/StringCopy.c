#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion prototype
    void MyStrcpy(char *, char *);

    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    char chArray_Copy_PRJ[MAX_STRING_LENGTH];
    int iStringLength_PRJ = 0;

    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    // *** STRING COPY ***
    MyStrcpy(chArray_Copy_PRJ, chArray_PRJ);

    // *** STRING OUTPUT
    printf("\n\n");
    printf("The Orginal String (i.e : chArray_PRJ[]' ) Is : : \n\n %s \n\n", chArray_PRJ);

    printf("\n\n");
    printf("The Copied String (i.e : chArray_Copy_PRJ[]' ) Is : : \n\n %s \n\n", chArray_Copy_PRJ);

    return (0);
}

void MyStrcpy(char *str_destination, char *str_source)
{
    //fucntion prototype
    int MyStrlen(char[]);

    //variable declartions
    int iStringLength = 0;
    int i;

    //code
    iStringLength = MyStrlen(str_source);

    for (i = 0; i < iStringLength; i++)
    {
        str_destination[i] = str_source[i];
    }
    str_destination[i] = '\0';
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
