#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion prototype
    int MyStrlen(char[]);

    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    char chArray_SpaceRemoved_PRJ[MAX_STRING_LENGTH];
    int iStringLength_PRJ = 0;
    int i, j;

    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    iStringLength_PRJ = MyStrlen(chArray_PRJ);

    for (i = 0, j = 0; i < iStringLength_PRJ; i++)
    {
        if (chArray_PRJ[i] == 32) //32 Is The Ascci value for space (' ') Character
        {
            continue;
        }
        chArray_SpaceRemoved_PRJ[j] = chArray_PRJ[i];
        j++;
    }

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n %s \n\n", chArray_PRJ);

    printf("\n\n");
    printf("String After Removal pf Spaces : \n\n %s \n\n", chArray_SpaceRemoved_PRJ);

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
