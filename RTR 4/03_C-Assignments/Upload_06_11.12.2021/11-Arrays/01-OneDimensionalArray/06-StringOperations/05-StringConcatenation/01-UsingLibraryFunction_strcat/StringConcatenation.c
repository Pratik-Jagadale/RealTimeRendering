#include <stdio.h>
#include <String.h> // for strrev()

#define MAX_STRING_LENGTH 512

int main(void)
{
    //variable declaration
    char chArray_One_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    char chArray_Two_PRJ[MAX_STRING_LENGTH];
    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_One_PRJ, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("Enter B string : ");
    gets_s(chArray_Two_PRJ, MAX_STRING_LENGTH);

    // *** STRING OUTPUT
    printf("*** BEFORE CONVATENATION ***");
    printf("\n\n");
    printf("The Orginal String  entred By You (i.e chArray_One_PRJ[] )Is : : \n\n %s \n\n", chArray_One_PRJ);

    printf("\n\n");
    printf("The Orginal String  entred By You (i.e chArray_Two_PRJ[] )Is : : \n\n %s \n\n", chArray_Two_PRJ);

    strcat(chArray_One_PRJ, chArray_Two_PRJ); //(Destination, source)

    printf("\n\n");
    printf("*** AFTER CONVATENATION ***");
    printf("\n\n");
    printf("chArray_One_PRJ[] Is : : \n\n %s \n\n", chArray_One_PRJ); //destination

    printf("\n\n");
    printf("chArray_Two_PRJ[] Is : : \n\n %s \n\n", chArray_Two_PRJ);

    return (0);
}
