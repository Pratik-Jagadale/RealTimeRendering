/* Header files */
#include <stdio.h>

/* Entry poitn functions */
int main(void)
{
    /* variable declartions */
    int num_month_PRJ;

    /*code*/
    printf("\n\nEnter the Number of Month : ");
    scanf("%d", &num_month_PRJ);

    printf("\n\n");

    switch (num_month_PRJ)
    {

    case 1:
        printf("month Number is = %d Is January !!!\n\n", num_month_PRJ);
        break;
    case 2:
        printf("month Number is = %d Is February !!!\n\n", num_month_PRJ);
        break;
    case 3:
        printf("month Number is = %d Is March !!!\n\n", num_month_PRJ);
        break;
    case 4:
        printf("month Number is = %d Is April !!\n\n", num_month_PRJ);
        break;
    case 5:
        printf("month Number is = %d Is May !!!\n\n", num_month_PRJ);
        break;
    case 6:
        printf("month Number is = %d Is June !!!\n\n", num_month_PRJ);
        break;
    case 7:
        printf("month Number is = %d Is jully !!!\n\n", num_month_PRJ);
        break;
    case 8:
        printf("month Number is = %d Is August !!!\n\n", num_month_PRJ);
        break;
    case 9:
        printf("month Number is = %d Is September !!!\n\n", num_month_PRJ);
        break;
    case 10:
        printf("month Number is = %d Is Octomber !!!\n\n", num_month_PRJ);
        break;
    case 11:
        printf("month Number is = %d Is November !!!\n\n", num_month_PRJ);
        break;
    case 12:
        printf("month Number is = %d Is December !!!\n\n", num_month_PRJ);
        break;
    default:
        printf("Invalid month number %d Entered !!! please try again...\n\n", num_month_PRJ);
        break;
        /* 
        like ending optional else..just like terminating else is
        optional is if else if ladder , so is the defualt case 
        optional in switch case.
        */
    }

    return (0);
}
