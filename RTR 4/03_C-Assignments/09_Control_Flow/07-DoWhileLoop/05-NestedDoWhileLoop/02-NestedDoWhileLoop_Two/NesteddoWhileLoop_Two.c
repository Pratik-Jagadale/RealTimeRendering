#include <stdio.h>

/* Entry Point function */
int main(void)
{
    /* variable declartions */
    int i_PRJ, j_PRJ, k_PRJ;

    /* code */
    printf("\n\n");
    i_PRJ = 1;

    do
    {
        printf("i = %d\n", i_PRJ);
        printf("-----------------------------\n\n");

        j_PRJ = 1;
        do
        {
            printf("\tj= %d\n", j_PRJ);
            printf("--------------\n\n");
            k_PRJ = 1;

            do
            {
                printf("\t\tk= %d\t\t", k_PRJ);
                k_PRJ++;
            } while (k_PRJ <= 3);

            j_PRJ++;
            printf("\n\n");

        } while (j_PRJ <= 5);
        i_PRJ++;
        printf("\n\n");

    } while (i_PRJ <= 10);
    printf("\n");
    return (0);
}
