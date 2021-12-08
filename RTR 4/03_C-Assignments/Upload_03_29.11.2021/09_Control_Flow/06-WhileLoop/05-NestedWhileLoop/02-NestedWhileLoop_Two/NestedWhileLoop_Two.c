#include <stdio.h>

/* Entry Point function */
int main(void)
{
    /* variable declartions */
    int i_PRJ, j_PRJ, k_PRJ;

    /* code */
    printf("\n\n");
    i_PRJ = 1;

    while (i_PRJ <= 10)
    {
        printf("i = %d\n", i_PRJ);
        printf("-----------------------------\n\n");

        j_PRJ = 5;
        while (j_PRJ >= 1)
        {
            printf("\tj= %d\n", j_PRJ);
            printf("--------------\n\n");
            k_PRJ = 1;
            while (k_PRJ <= 3)
            {
                printf("\t\tk= %d\t\t", k_PRJ);
                k_PRJ++;
            }
            j_PRJ--;
            printf("\n\n");
        }
        i_PRJ++;
        printf("\n\n");
    }
    printf("\n");
    return (0);
}
