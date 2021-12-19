#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String

    //code
    printf("\n\n");

    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("String Entered By You Is : \n\n %s \n\n", chArray_PRJ);

    return (0);
}
