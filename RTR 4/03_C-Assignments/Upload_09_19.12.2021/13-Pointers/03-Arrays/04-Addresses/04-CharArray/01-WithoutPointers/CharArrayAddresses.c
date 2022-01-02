#include <stdio.h>

int main(void)
{
    //Variable declartion
    char cArray_PRJ[10];
    int i;

    //code
    for (i = 0; i < 10; i++)
        cArray_PRJ[i] = (char)(i + 65);

    printf("\n\n");
    printf("Elements of The Character array : \n\n");
    for (i = 0; i < 10; i++)
        printf("cArray_PRJ[%d] = %c \t\t Address = %p\n", i, cArray_PRJ[i], &cArray_PRJ[i]);

    printf("\n\n");
    return (0);
}
