/* Header files */
#include <stdio.h>

/* Entry poitn functions */
int main(void)
{
    /* variable declartions */
    int num_month_PRJ;

    /*code*/
    printf("\n\nEnter the Number of Month (1 to 12) : ");
    scanf("%d", &num_month_PRJ);

    printf("\n\n");

    if (num_month_PRJ == 1)
    {
        printf("month Number is = %d Is January !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 2)
    {
        printf("month Number is = %d Is February !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 3)
    {
        printf("month Number is = %d Is March !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 4)
    {
        printf("month Number is = %d Is April !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 5)
    {
        printf("month Number is = %d Is May !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 6)
    {
        printf("month Number is = %d Is June !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 7)
    {
        printf("month Number is = %d Is Jully !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 8)
    {
        printf("month Number is = %d Is Augast !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 9)
    {
        printf("month Number is = %d Is September !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 10)
    {
        printf("month Number is = %d Is octomber !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 11)
    {
        printf("month Number is = %d Is November !!!\n\n", num_month_PRJ);
    }
    else if (num_month_PRJ == 12)
    {
        printf("month Number is = %d Is December !!!\n\n", num_month_PRJ);
    }
    else
    {
        printf("month Number %d is invalid !!!\n\n", num_month_PRJ);
    }

    return (0);
}
