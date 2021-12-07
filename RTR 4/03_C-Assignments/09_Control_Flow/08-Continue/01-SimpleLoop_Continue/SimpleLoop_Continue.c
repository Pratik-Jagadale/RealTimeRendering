#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int i_PRJ;

    /* Code */
    printf("\n\n");

    printf("Printing Even Numbers From 0 to 100: \n\n");

    for (i_PRJ = 0; i_PRJ <= 100; i_PRJ++)
    {
        if (i_PRJ % 2 != 0)
        {
            continue;
        }
        else
        {
            printf("\t%d\n", i_PRJ);
        }
    }
    printf("\n\n");

    return (0);
}
