#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int i_PRJ, j_PRJ;

    /* Code */
    printf("\n\n");

    printf("Outer Loop Prints Odd Numbers Between 1 and 10.\n\n");

    printf("Inner Loop Prints Even Numbers Between 1 and 10 For Every Odd Number Print By Outer Loop.\n\n");

    for (i_PRJ = 1; i_PRJ <= 10; i_PRJ++)
    {
        if (i_PRJ % 2 != 0) //odd number
        {
            printf("i = %d\n", i_PRJ);
            printf("--------------\n");

            for (j_PRJ = 1; j_PRJ <= 10; j_PRJ++)
            {
                if (j_PRJ % 2 == 0) //Even Number
                {
                    printf("\tj=%d\n", j_PRJ);
                }
                else //if Number (j) is Odd
                {
                    continue;
                }
            }
        }
        else //If  Number (i) is Even
        {
            continue;
        }
    }
    printf("\n\n");

    return (0);
}
