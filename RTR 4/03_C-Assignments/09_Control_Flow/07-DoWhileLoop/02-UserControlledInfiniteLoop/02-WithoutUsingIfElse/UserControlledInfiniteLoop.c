#include <stdio.h>

/* Entry Point Function */
int main(void)
{
    /* variable declartions */
    char option_PRJ, ch_PRJ;

    /* Code */
    printf("\n\n");
    printf("Once the Infinite loop Begins, Enter 'Q' or 'q to Quite The Infinite For Loop: \n\n");
    printf("Enter 'y' or 'y' To Initiate user Controlled Infinite loop : ");
    option_PRJ = getch();

    printf("\n\n");

    do
    {
        do
        {
            printf("In Loop...\n");
            ch_PRJ = getch();

        } while (ch_PRJ != 'Q' && ch_PRJ != 'q');
        printf("\n\n");
        printf("Exitting from Infinite Loop....");

        printf("\n\n");
        printf("Do You Want to Begin User Controlled Infinite Loop Again?...(Y/y Yes, Any Other Key - No: ");
        option_PRJ = getch();

    } while (option_PRJ == 'Y' || option_PRJ == 'y');

    return (0);
}
