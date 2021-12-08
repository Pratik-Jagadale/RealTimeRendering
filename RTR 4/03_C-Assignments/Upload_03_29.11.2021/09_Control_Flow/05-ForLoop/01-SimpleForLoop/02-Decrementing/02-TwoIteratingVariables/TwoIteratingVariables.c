#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int i_PRJ, j_PRJ;

    /* code */
    printf("\n\n");

    printf("Printing Digits 1 to 10 and 10 to 1: \n\n");

    for (i_PRJ = 1, j_PRJ = 10; i_PRJ <= 10, j_PRJ >= 1; i_PRJ++, j_PRJ--)
        printf("\t%d\t%d\t\n", i_PRJ, j_PRJ);

    printf("\n\n");

    return (0);
}