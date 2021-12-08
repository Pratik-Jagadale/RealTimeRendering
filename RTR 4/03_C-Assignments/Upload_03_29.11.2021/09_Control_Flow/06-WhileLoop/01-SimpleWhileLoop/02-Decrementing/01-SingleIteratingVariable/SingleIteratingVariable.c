#include <stdio.h>

int main(void)
{
    /* Variable declartions */
    int i;

    /* code */
    printf("\n\n");

    printf("Printing Digits 10 to 1: \n\n");

    i = 10;
    while (i >= 1)
    {
        printf("\t%d\n", i);
        i = i--;
    }
    printf("\n\n");

    return (0);
}
