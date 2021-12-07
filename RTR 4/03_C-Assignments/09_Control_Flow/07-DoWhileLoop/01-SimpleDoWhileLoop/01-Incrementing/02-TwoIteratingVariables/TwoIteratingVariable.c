#include <stdio.h>

/* Entry Point Function */
int main(void)
{
    /* Variable declartions */
    int i, j;

    /* code */
    printf("\n\n");

    printf("Printing Digits 1 to 10 and 10 to 1000 \n\n");

    i = 1;
    j = 100;
    do
    {
        printf("\t%d \t %d\n", i, j);
        i++;
        j = j + 100;
    } while (i <= 10, j <= 1000);
    printf("\n\n");

    return (0);
}
