#include <stdio.h>
#include <String.h> // for strrev()

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion prototype
    void MyStrrev(char *str_destinantion, char *str_source);

    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    char chArray_reverse_PRJ[MAX_STRING_LENGTH];
    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    // *** STRING OUTPUT
    printf("\n\n");
    printf("The Orginal String Is : : \n\n %s \n\n", chArray_PRJ);

    printf("\n\n");
    MyStrrev(chArray_reverse_PRJ, chArray_PRJ);
    printf("The Reversed String Is : : \n\n %s \n\n", chArray_reverse_PRJ);

    return (0);
}

void MyStrrev(char *str_destination, char *str_source)
{
    //function prototype
    int MyStrlen(char[]);

    //variable declartions
    int iStringLength_PRJ = 0;
    int i, j, len_PRJ;

    //code
    iStringLength_PRJ = MyStrlen(str_source);

    // Array INDICES BAEGINS FROM 0, HENCE, LAST WILL ALWAYS BE (LENGTH - 1)
    len_PRJ = iStringLength_PRJ - 1;

    //  WE NEED TO PUT THE CHARACTER WHICH IS LAST INDEX OF str_source  TO THE
    //  FIRST INDEX OF 'str_destination' and so on...
    for (i = 0, j = len_PRJ; i < iStringLength_PRJ, j >= 0; i++, j--)
    {
        str_destination[i] = str_source[j];
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
