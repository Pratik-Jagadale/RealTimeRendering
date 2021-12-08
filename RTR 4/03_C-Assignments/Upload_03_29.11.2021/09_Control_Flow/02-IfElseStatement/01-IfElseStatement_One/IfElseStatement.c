/*Header files*/
#include <stdio.h>

/* enrty poitn functions */
int main(void)
{
    /* variable declartions */
    int a_PRJ, b_PRJ, p_PRJ;

    /* code */
    a_PRJ = 10;
    b_PRJ = 50;
    p_PRJ = 50;
    printf("\n\n");

    if (a_PRJ < b_PRJ)
    {
        printf("Entering First if-block...\n\n");
        printf("A Is Less Than B !!!\n\n");
    }
    else
    {
        printf("Entering first else block..\n\n");
    }

    printf("\n\n");

    if (b_PRJ != p_PRJ)
    {
        printf("Entering the second if block...\n\n");
        printf("B Is Equal To P...\n\n");
    }
    else
    {
        printf("Entering Seocond block...\n\n");
        printf("B Is Equal tp P !!!.\n\n");
    }
    printf("Secpnd if-else Pair Done..");

    return (0);
}
