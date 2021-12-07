#include <stdio.h>

/* Entry Point Function */
int main(void)
{
    /* Variable declartions */
    int i;

    /* code */
    printf("\n\n");

    printf("Printing Digits 10 to 1: \n\n");

    i = 10;
    do
    {
        printf("\t%d\n", i);
        i = i--;
    } while (i >= 1);
    printf("\n\n");

    return (0);
}
