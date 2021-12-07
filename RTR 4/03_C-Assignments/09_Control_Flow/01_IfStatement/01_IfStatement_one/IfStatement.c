#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int a_PRJ, b_PRJ, p_PRJ;

    /* code */
    a_PRJ = 9;
    b_PRJ = 30;
    p_PRJ = 30;

    printf("\n------------------------------------------\n");

    if (a_PRJ < b_PRJ)
    {
        printf("A Is Less Than B !!!\n\n");
    }

    if (b_PRJ != p_PRJ)
    {
        printf("B Is Not Equal to P !!!\n\n.");
    }

    printf("Both Comparisons have been Done!!!.\n");
    printf("\n------------------------------------------\n");
    return (0);
}
