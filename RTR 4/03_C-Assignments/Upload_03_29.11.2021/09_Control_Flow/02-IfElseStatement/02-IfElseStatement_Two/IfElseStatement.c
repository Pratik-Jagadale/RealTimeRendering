/*Header files*/
#include <stdio.h>

/* enrty poitn functions */
int main(void)
{
    /* variable declartions */
    int age_PRJ;

    /*code*/
    printf("\n\nEnter your Age: ");
    scanf("%d", &age_PRJ);
    printf("\n\n");

    if (age_PRJ >= 18)
    {
        printf("Entering if-block...\n\n");
        printf("You Are Eligible For Voting !!!\n\n");
    }
    else
    {
        printf("Entering else block..\n\n");
        printf("You Are Not Eligible For Voting !!!\n\n");
    }

    printf("---------------------------------------------------");

    return (0);
}
