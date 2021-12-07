#include <stdio.h>

int main(void)
{
    /* Variable declartions */
    int i, j;

    /* code */
    printf("\n\n");

    printf("Printing Digits 10 to 1 and 1000 to 100 \n\n");

    i = 10;
    j = 1000;
    while (i >= 1, j >= 100)
    {
        printf("\t%d \t %d\n", i, j);
        i--;
        j = j - 100;
    }
    printf("\n\n");

    return (0);
}
