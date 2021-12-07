#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int i_PRJ, j_PRJ;

    /* code */
    printf("\n\n");
    i_PRJ = 1;
    while (i_PRJ <= 10)
    {
        printf("i = %d\n", i_PRJ);
        j_PRJ = 1;
        while (j_PRJ <= 10)
        {
            printf("\tj= %d\n", j_PRJ);
            j_PRJ++;
        }
        printf("\n\n");
        i_PRJ++;
    }
    return (0);
}
