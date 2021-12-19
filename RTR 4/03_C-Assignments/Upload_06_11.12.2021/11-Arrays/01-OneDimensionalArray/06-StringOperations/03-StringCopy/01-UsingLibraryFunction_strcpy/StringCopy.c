#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
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
    strcpy(chArray_Copy_PRJ, chArray_PRJ);

    // *** STRING OUTPUT
    printf("\n\n");
    printf("The Orginal String (i.e : chArray_PRJ[]' ) Is : : \n\n %s \n\n", chArray_PRJ);

    printf("\n\n");
    printf("The Copied String (i.e : chArray_Copy_PRJ[]' ) Is : : \n\n %s \n\n", chArray_Copy_PRJ);

    return (0);
}
