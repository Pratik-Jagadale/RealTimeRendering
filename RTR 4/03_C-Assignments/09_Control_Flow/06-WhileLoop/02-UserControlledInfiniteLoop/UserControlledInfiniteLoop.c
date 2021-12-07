#include <stdio.h>

#define INFINITE_LOOP_PRJ 1
/* Entry Point Function */
int main(void)
{
    /* variable declartions */
    char option_PRJ, ch_PRJ;

    /* Code */
    printf("\n\n");
    printf("Once the Infinite loop Begins, Enter 'Q' or 'q to Quite The Infinite For Loop: \n\n");
    printf("Enter 'y' or 'y' To Initaiate user Controlled Infinite loop : ");
    option_PRJ = getch();

    printf("\n\n");

    if (option_PRJ == 'Y' || option_PRJ == 'y')
    {
        while (INFINITE_LOOP_PRJ)
        {
            printf("In Loop...\n");
            ch_PRJ = getch();
            if (ch_PRJ == 'Q' || ch_PRJ == 'q')
            {
                printf("Exitting from Infinite Loop....");
                break; //User Controlled Exiting From Infinite Loop
            }
        }
    }
    else
        printf("You Must Press 'Y' or 'y' To Infinite The User Controlled Infinite Loop....Please try Again.. ");

    return (0);
}
