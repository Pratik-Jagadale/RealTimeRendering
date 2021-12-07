#include <stdio.h>
#include <conio.h>
int main(void)
{
    /* variable declartions */
    int i_PRJ;
    char ch_PRJ;

    /* Code */
    printf("\n\n");

    printf("Printing Even Numbers From 0 to 100 For Every User Input. Exitting The Loop When User Enters Character 'Q' or 'q': \n\n");
    printf("Enter Character 'Q' or 'q' To Exit Loop: \n\n");

    for (i_PRJ = 0; i_PRJ <= 100; i_PRJ++)
    {
        printf("\t%d\n", i_PRJ);
        ch_PRJ = getch();
        if (ch_PRJ == 'Q' || ch_PRJ == 'q')
        {
            printf("EXITTING LOOP...");
            printf("\n\n");
            break;
        }
    }
    printf("\n\n");

    return (0);
}
