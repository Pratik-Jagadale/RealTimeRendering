#include <stdio.h>
#include <String.h> // for strrev()

#define MAX_STRING_LENGTH 512

int main(void)
{
    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String

    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    // *** STRING OUTPUT
    printf("\n\n");
    printf("The Orginal String Is : : \n\n %s \n\n", chArray_PRJ);

    printf("\n\n");
    printf("The Reversed String Is : : \n\n %s \n\n", strrev(chArray_PRJ));

    return (0);
}
