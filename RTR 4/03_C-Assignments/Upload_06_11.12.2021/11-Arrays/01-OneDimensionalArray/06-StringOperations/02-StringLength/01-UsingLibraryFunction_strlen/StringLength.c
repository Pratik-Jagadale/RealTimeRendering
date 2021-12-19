#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
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
    iStringLength_PRJ = strlen(chArray_PRJ);
    printf("Length Of String Is = %d Characters !!!\n\n", iStringLength_PRJ);

    return (0);
}
