#include <stdio.h>
#include <conio.h>

int main(void)
{
    /* variable ddeclartion */
    char option, ch = '\0';

    /* code */
    printf("\n\n");

    printf("once The Infinite Loop Begins, Enter ctrl + c To Quite The Infinite For Loop:\n\n");

    printf("Enter 'Y' or 'y' To initiate user Controlled Infinite Loop :");
    option = getch();

    printf("\n\n");
    if (option == 'Y' || option == 'y')
    {
        for (;;) //Infinite Loop
        {
            printf("In Loop... (to exit enter 'ctrl + c')\n\n");
            //ch =getch();
            /*if (ch == 'Q' || ch == 'q')
            {
                printf("Exitting User Controlled Infifnite loop....\n\n");
                break; //user Controlled Exitting From Infinite loop.
            }*/
        }
    }
    else
    {
        printf("Inavlid Character...Please try Again...\n\n");
    }

    printf("\n");
    return (0);
}
