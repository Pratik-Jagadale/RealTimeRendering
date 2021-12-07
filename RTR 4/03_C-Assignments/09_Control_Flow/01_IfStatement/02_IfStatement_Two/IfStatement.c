#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int age_PRJ;

    /* code */
    printf("\n\n");
    printf("Enter your age:\n->");
    scanf("%d", &age_PRJ);

    if (age_PRJ >= 18)
    {
        printf("\nYou Are Eligible For Voting !!!\n\n");
    }
    return (0);
}
