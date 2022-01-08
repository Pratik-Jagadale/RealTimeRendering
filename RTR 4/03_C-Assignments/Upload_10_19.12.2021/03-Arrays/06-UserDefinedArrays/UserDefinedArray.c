#include <stdio.h>
#include <stdlib.h>

#define INT_SIZE sizeof(int)
#define FLOAT_SIZE sizeof(float)
#define DOUBLE_SIZE sizeof(double)
#define CHAR_SIZE sizeof(char)

int main(void)
{
    // Variable declartions
    int *ptr_iArray_PRJ = NULL;
    unsigned int iArrayLength_PRJ = 0;

    float *ptr_fArray_PRJ = NULL;
    unsigned int fArrayLength_PRJ = 0;

    double *ptr_dArray_PRJ = NULL;
    unsigned int dArrayLength_PRJ = 0;

    char *ptr_cArray_PRJ = NULL;
    unsigned int cArrayLength_PRJ = 0;

    int i;

    //code

    // -------------- INTEGER ARRAY ------------- //
    printf("\n\n");
    printf("Enter The Number Of Elements You want In The Integer Aray :");
    scanf("%u", &iArrayLength_PRJ);

    ptr_iArray_PRJ = (int *)malloc(INT_SIZE * iArrayLength_PRJ);
    if (ptr_iArray_PRJ == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOACATION FOR INTEGER ARRAY FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY SUCCEDED");
    }

    printf("\n\n");
    printf("Enter The %d Integer Elements To Fill Up The Integer Array :\n\n", iArrayLength_PRJ);
    for (i = 0; i < iArrayLength_PRJ; i++)
        scanf("%d", (ptr_iArray_PRJ + i));

    // -------------- FLOAT ARRAY ------------- //
    printf("\n\n");
    printf("Enter The Number Of Elements You want In The Float Aray :");
    scanf("%u", &fArrayLength_PRJ);

    ptr_fArray_PRJ = (float *)malloc(FLOAT_SIZE * fArrayLength_PRJ);
    if (ptr_fArray_PRJ == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOACATION FOR FLOAT ARRAY FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR FLOAT ARRAY SUCCEDED");
    }

    printf("\n\n");
    printf("Enter The %d floating-point Elements To Fill Up The Float Array :\n\n", fArrayLength_PRJ);
    for (i = 0; i < fArrayLength_PRJ; i++)
        scanf("%f", (ptr_fArray_PRJ + i));

    // -------------- DOULE ARRAY ------------- //
    printf("\n\n");
    printf("Enter The Number Of Elements You want In The Double Aray :");
    scanf("%u", &dArrayLength_PRJ);

    ptr_dArray_PRJ = (double *)malloc(DOUBLE_SIZE * dArrayLength_PRJ);
    if (ptr_dArray_PRJ == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOACATION FOR DOUBLE ARRAY FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR DOUBLE ARRAY SUCCEDED");
    }

    printf("\n\n");
    printf("Enter The %d Double Elements To Fill Up The Double Array :\n\n", dArrayLength_PRJ);
    for (i = 0; i < dArrayLength_PRJ; i++)
        scanf("%lf", (ptr_dArray_PRJ + i));

    // -------------- CHAR ARRAY ------------- //
    printf("\n\n");
    printf("Enter The Number Of Elements You want In The Character Aray :");
    scanf("%u", &cArrayLength_PRJ);

    ptr_cArray_PRJ = (char *)malloc(CHAR_SIZE * cArrayLength_PRJ);
    if (ptr_cArray_PRJ == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOACATION FOR CHARACTER ARRAY FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR CHARACTER ARRAY SUCCEDED");
    }

    printf("\n\n");
    printf("Enter The %d Charcater Elements To Fill Up The Char Array :\n\n", cArrayLength_PRJ);
    for (i = 0; i < cArrayLength_PRJ; i++)
        scanf("%c", (ptr_cArray_PRJ + i));

    // ------------------ DISPLAY OF ARRAYS -----------------

    // ----------------- INTEGER ARRAY -----------------
    printf("\n\n");
    printf("The Integer Array Entered  by You, Consisting Of %d Elements :\n\n", iArrayLength_PRJ);

    printf("\n\n");
    for (i = 0; i < iArrayLength_PRJ; i++)
    {
        printf("*(ptr_iArray_PRJ +  %d) = %d \t\t At address (ptr_iArray + %d) : %p\n", i, *(ptr_iArray_PRJ + i), i, (ptr_iArray_PRJ + i));
    }

    // ----------------- FLOAT ARRAY -----------------
    printf("\n\n");
    printf("The Flaoting-Point Array Entered  by You, Consisting Of %d Elements :\n\n", fArrayLength_PRJ);

    printf("\n\n");
    for (i = 0; i < fArrayLength_PRJ; i++)
    {
        printf("*(ptr_fArray_PRJ +  %d) = %f \t\t At address (ptr_iArray + %d) : %p\n", i, *(ptr_fArray_PRJ + i), i, (ptr_fArray_PRJ + i));
    }

    // ----------------- DOUBLE ARRAY -----------------
    printf("\n\n");
    printf("The Double Array Entered  by You, Consisting Of %d Elements :\n\n", dArrayLength_PRJ);

    printf("\n\n");
    for (i = 0; i < dArrayLength_PRJ; i++)
    {
        printf("*(ptr_dArray_PRJ +  %d) = %lf \t\t At address (ptr_iArray + %d) : %p\n", i, *(ptr_dArray_PRJ + i), i, (ptr_dArray_PRJ + i));
    }

    // ----------------- CHAR ARRAY -----------------
    printf("\n\n");
    printf("The Character Array Entered  by You, Consisting Of %d Elements :\n\n", cArrayLength_PRJ);

    printf("\n\n");
    for (i = 0; i < cArrayLength_PRJ; i++)
    {
        printf("*(ptr_cArray_PRJ +  %d) = %c \t\t At address (ptr_cArray + %d) : %p\n", i, *(ptr_cArray_PRJ + i), i, (ptr_cArray_PRJ + i));
    }

    // ------------ FREEING MEMORY OCCUPIED BY ARRAYS (IN REVERSE ORDER OF ALLOACTION )-------------------

    if (ptr_cArray_PRJ)
    {
        free(ptr_cArray_PRJ);
        ptr_cArray_PRJ = NULL;

        printf("MEMOMRY OCCUPIED BY CHARACTER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    if (ptr_dArray_PRJ)
    {
        free(ptr_dArray_PRJ);
        ptr_dArray_PRJ = NULL;

        printf("MEMOMRY OCCUPIED BY DOUBLE ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    if (ptr_fArray_PRJ)
    {
        free(ptr_fArray_PRJ);
        ptr_fArray_PRJ = NULL;

        printf("MEMOMRY OCCUPIED BY FLOAT ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    if (ptr_iArray_PRJ)
    {
        free(ptr_iArray_PRJ);
        ptr_iArray_PRJ = NULL;

        printf("MEMOMRY OCCUPIED BY INTEGER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    return (0);
}
