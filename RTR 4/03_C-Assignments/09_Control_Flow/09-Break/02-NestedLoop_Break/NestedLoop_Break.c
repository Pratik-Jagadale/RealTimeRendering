#include <stdio.h>
#include <conio.h>
int main(void)
{
    /* variable declartions */
    int i_PRJ, j_PRJ;

    /* Code */
    printf("\n\n");

    for (i_PRJ = 1; i_PRJ <= 20; i_PRJ++)
    {
        for (j_PRJ = 1; j_PRJ <= 20; j_PRJ++)
        {
            if (j_PRJ > i_PRJ)
            {
                printf("\n-------------Exiting Inner Loop---------------\n");
                break;
            }
            else
            {
                printf("* ");
            }
            if (j_PRJ % 5 == 0) //printing '*'s in 5 columns
                printf("\n");
        }
    }
    printf("\n\n");

    return (0);
}
