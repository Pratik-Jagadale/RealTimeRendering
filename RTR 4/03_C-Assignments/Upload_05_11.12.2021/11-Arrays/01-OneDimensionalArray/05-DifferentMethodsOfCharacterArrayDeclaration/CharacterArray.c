#include <stdio.h>

int main(void)
{
    //variable declaration
    char chArray_01[] = {'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P', '\0'}; //must give \0 for proper initialization

    char chArray_02[9] = {'W', 'E', 'L', 'C', 'O', 'M', 'E', '\0'};

    char chArray_03[] = {'Y', 'O', 'U', '\0'};

    char chArray_04[] = "TO"; // \0 is assumed given 3 , although string has only two character

    char chArray_05[] = "REAL TIME READERING BATCH 2021-2022 ";

    char chArray_WithoutNullTerminator[] = {'H', 'e', 'i', 'o', 'u'};

    //code

    printf("\n\n");

    printf("size of chArray_01 : %lu \n\n", sizeof(chArray_01));
    printf("size of chArray_02 : %lu \n\n", sizeof(chArray_02));
    printf("size of chArray_03 : %lu \n\n", sizeof(chArray_03));
    printf("size of chArray_04 : %lu \n\n", sizeof(chArray_04));
    printf("size of chArray_05 : %lu \n\n", sizeof(chArray_05));

    printf("\n\n");

    printf("The String Are : \n\n");
    printf("chArray_01 : %s \n\n", chArray_01);
    printf("chArray_02 : %s \n\n", chArray_02);
    printf("cArray_03 : %s \n\n", chArray_03);
    printf("chArray_04 : %s \n\n", chArray_04);
    printf("chArray_05 : %s \n\n", chArray_05);

    printf("\n\n");
    printf("Size of chArray_WithoutNullTerminnator: %lu \n\n", sizeof(chArray_WithoutNullTerminator));

    printf("chArray_wihout_NullTerminator : %s \n\n", chArray_WithoutNullTerminator);

    return (0);
}
