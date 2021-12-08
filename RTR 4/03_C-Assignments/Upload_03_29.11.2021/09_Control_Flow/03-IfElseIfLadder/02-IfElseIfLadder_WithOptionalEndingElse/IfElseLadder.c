/* Header files */
#include <stdio.h>

/* Entry poitn functions */
int main(void)
{
    /* variable declartions */
    int num_PRJ;

    /*code*/
    printf("\n\nEnter your value for 'num' : ");
    scanf("%d", &num_PRJ);

    printf("\n\n");

        if (num_PRJ < 0)
    {
        printf("Num = %d Is Less Than 0 (Negative) !!!!\n\n", num_PRJ);
    }
    else if ((num_PRJ > 0) && (num_PRJ <= 100))
    {
        printf("Num = %d Is Between 0 to 100  !!!!\n\n", num_PRJ);
    }
    else if ((num_PRJ > 100) && (num_PRJ <= 200))
    {
        printf("Num = %d Is Between 100 to 200  !!!!\n\n", num_PRJ);
    }
    else if ((num_PRJ > 200) && (num_PRJ <= 300))
    {
        printf("Num = %d Is Between 200 to 300  !!!!\n\n", num_PRJ);
    }
    else if ((num_PRJ > 300) && (num_PRJ <= 400))
    {
        printf("Num = %d Is Between 300 to 400  !!!!\n\n", num_PRJ);
    }
    else if ((num_PRJ > 400) && (num_PRJ <= 500))
    {
        printf("Num = %d Is Between 400 to 500  !!!!\n\n", num_PRJ);
    }
    else if (num_PRJ > 500)
    {
        printf("Num = %d is greater  than 500 !!!\n\n", num_PRJ);
    }
    else
    {
        printf("Invalid value Entered !!!\n\n");
    }

    return (0);
}
