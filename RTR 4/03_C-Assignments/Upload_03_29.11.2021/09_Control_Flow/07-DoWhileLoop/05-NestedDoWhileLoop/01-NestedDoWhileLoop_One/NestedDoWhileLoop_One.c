#include <stdio.h>

/* Entry Point Function */
int main(void)
{
    /* variable declartions */
    int i_PRJ, j_PRJ;

    /* code */
    printf("\n\n");
    i_PRJ = 1;
    do
    {
        printf("i = %d\n", i_PRJ);
        printf("------------------\n\n");
        j_PRJ = 1;
        do
        {
            printf("\tj= %d\n", j_PRJ);
            j_PRJ++;
        } while (j_PRJ <= 5);
        printf("\n\n");
        i_PRJ++;
    } while (i_PRJ <= 10);
    return (0);
}
