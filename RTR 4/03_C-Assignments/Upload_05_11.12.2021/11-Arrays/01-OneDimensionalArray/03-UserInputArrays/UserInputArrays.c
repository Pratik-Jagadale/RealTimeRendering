#include <stdio.h>

// MAcro constant used as array size in subscript and as length.
// hence , this programs arrays sizes can be simply changed bt changing these
//following 3 global macro constant values, before compiling , linking and
//executing the pogram...

#define INT_ARRAY_NUM_ELEMENTS 5
#define FLOAT_ARRAY_NUM_ELEMETS 3
#define CHAR_ARRAY_NUM_ELEMETS 15

// entry point function
int main(void)
{
    //variable declartion
    int iArray[INT_ARRAY_NUM_ELEMENTS];
    float fArray[FLOAT_ARRAY_NUM_ELEMETS];
    char cArray[CHAR_ARRAY_NUM_ELEMETS];
    int i, num;

    //Array Elements Input
    printf("\n\n");
    printf("Enter Elements For 'Integer' Array : \n");
    for (i = 0; i < INT_ARRAY_NUM_ELEMENTS; i++)
        scanf("%d", &iArray[i]);

    printf("\n\n");
    printf("Enter Elements For 'Floating Point' Array : \n");
    for (i = 0; i < FLOAT_ARRAY_NUM_ELEMETS; i++)
        scanf("%f", &fArray[i]);

    printf("\n\n");
    printf("Enter Elements For 'Character' Array : \n");
    for (i = 0; i < CHAR_ARRAY_NUM_ELEMETS; i++)
        scanf("%c", &cArray[i]);

    //Array Elements OUTPUT
    printf("\n\n");
    printf("Integer Array Entered By You: \n");
    for (i = 0; i < INT_ARRAY_NUM_ELEMENTS; i++)
        printf("%d\t", iArray[i]);

    printf("\n\n");
    printf("Floating Point Array Entered By You : \n");
    for (i = 0; i < FLOAT_ARRAY_NUM_ELEMETS; i++)
        printf("%f\t", fArray[i]);

    printf("\n\n");
    printf("Character Array Entered By You :\n");
    for (i = 0; i < CHAR_ARRAY_NUM_ELEMETS; i++)
        printf("%c\t", cArray[i]);

    printf("\n\n");
    return (0);
}
