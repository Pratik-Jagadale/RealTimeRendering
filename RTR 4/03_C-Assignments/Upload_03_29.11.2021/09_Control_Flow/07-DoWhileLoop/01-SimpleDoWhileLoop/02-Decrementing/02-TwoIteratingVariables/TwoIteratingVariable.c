#include <stdio.h>

/* Entry Point Function */
int main(void)
{
    /* Variable declartions */
    int i, j;

    /* code */
    printf("\n\n");

    printf("Printing Digits 10 to 1 and 100 to 10 \n\n");

    i = 10;
    j = 100;
    do
    {
        printf("\t%d \t %d\n", i, j);
        i--;
        j = j - 10;
    } while (i >= 1, j >= 10);
    printf("\n\n");

    return (0);
}
