#include <stdio.h>
#include <string.h>

int main(void)
{
    // variable declartions
    char ch_PRJ, ch_i_PRJ;
    unsigned int ascii_ch_PRJ = 0;

    //code
    printf("\n\n");
    printf("Enter The First Character Of First Name :");
    ch_PRJ = getch();

    ch_PRJ = toupper(ch_PRJ);

    for (ch_i_PRJ = 'A'; ch_i_PRJ <= 'Z'; ch_i_PRJ++)
    {
        if (ch_PRJ == ch_i_PRJ)
        {
            ascii_ch_PRJ = (unsigned int)ch_PRJ;
            goto result_output;
        }
    }
    printf("\n\n");
    printf("Goto Statement not executed , so printing \\ \"Hello Wolrd !!!\"\n ");

result_output:
    if (ascii_ch_PRJ == 0)
    {
        printf("You have a strange name! could not find the character %c in the entire English Alphabet !\n", ch_PRJ);
    }
    else
    {
        printf("Character %c Found. It Has Ascii value %u. \n", ch_PRJ, ascii_ch_PRJ);
    }

    printf("\n\n");
    return (0);
}
