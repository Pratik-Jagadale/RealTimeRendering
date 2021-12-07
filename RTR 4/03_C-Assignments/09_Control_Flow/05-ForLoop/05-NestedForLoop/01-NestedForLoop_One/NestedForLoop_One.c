#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int i_PRJ, j_PRJ;

    /* code */
    printf("\n\n");

    for (i_PRJ = 1; i_PRJ <= 10; i_PRJ++)
    {
        printf("i = %d\n", i_PRJ);

        for (j_PRJ = 1; j_PRJ <= 5; j_PRJ++)
        {
            printf("\tj= %d\n", j_PRJ);
        }
        printf("\n\n");
    }
    printf("\n");

    return (0);
}
