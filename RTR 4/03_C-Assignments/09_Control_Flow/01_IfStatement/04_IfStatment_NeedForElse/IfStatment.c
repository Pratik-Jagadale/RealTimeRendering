#include <stdio.h>

int main(void)
{
    /* variable declartions*/
    int age_PRJ;

    /* code */
    printf("\n\n");
    printf("Enter your age:");
    scanf("%d", &age_PRJ);

    if (age_PRJ >= 18)
    {
        printf("You Are Eligible For Voting !!!\n\n");
    }
    printf("you Are Not Eligible For Voting !!!\n\n");

    return (0);
}
