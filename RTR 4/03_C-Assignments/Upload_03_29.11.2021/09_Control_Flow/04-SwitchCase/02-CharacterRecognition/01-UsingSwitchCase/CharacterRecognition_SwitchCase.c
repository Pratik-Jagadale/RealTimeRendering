/* Header files */
#include <stdio.h>
#include <conio.h> /* for getch() */

//ASCII Values For 'A' to 'Z' -> 65 to 90
#define CHAR_ALPHABET_UPPER_CASE_BEGINNING 65
#define CHAR_ALPHABET_UPPER_CASE_ENDING 90

//ASCII Values For 'a' to 'z' -> 97 to 122
#define CHAR_ALPHABET_LOWER_CASE_BEGINNING 97
#define CHAR_ALPHABET_LOWER_CASE_ENDING 122

//ASCII Values For '0' to '9' -> 48 to 57
#define CHAR_DIGIT_BEGINNING 48
#define CHAR_DIGIT_ENDING 57

/* Entry point functions */
int main(void)
{
    /* variable declartions */
    char ch_PRJ;
    int ch_value_PRJ;

    /*code*/
    printf("\n\nEnter Character : ");
    ch_PRJ = getch();

    printf("\n\n");

    switch (ch_PRJ)
    {
    //FALL THROUGH CONDTION
    case 'A':
    case 'a':

    case 'E':
    case 'e':

    case 'I':
    case 'i':

    case 'O':
    case 'o':

    case 'U':
    case 'u':
        printf("Character \'%c\' Entered By You , Is A Vowel Character From The English Aplhabet !!!!\n\n", ch_PRJ);

    default:
        ch_value_PRJ = (int)ch_PRJ; /* type casting */
        break;
    }
    printf("ASCII value of ch = %d\n\n", ch_value_PRJ);

    /*If The character has ascii value between 65 and 90 or between 97 and 
    122, it is still a letter of the alphabet, But It is A constant and not a
    vowel...
     */

    if ((ch_value_PRJ >= CHAR_ALPHABET_UPPER_CASE_BEGINNING && ch_value_PRJ <= CHAR_ALPHABET_UPPER_CASE_ENDING) || (ch_value_PRJ >= CHAR_ALPHABET_LOWER_CASE_BEGINNING && ch_value_PRJ <= CHAR_ALPHABET_LOWER_CASE_ENDING))
    {
        printf("Character \'%c\' Entered by you, Is A Constant Characterr !!!\n\n", ch_PRJ);
    }
    else if (ch_value_PRJ >= CHAR_DIGIT_BEGINNING && ch_value_PRJ <= CHAR_DIGIT_ENDING)
    {
        printf("character \'%c\' Entered by you, is a DIGIT Character !!!\n\n", ch_PRJ);
    }
    else
    {
        printf("character \'%c\' Entered by you, is a SPECAIL Character !!!\n\n", ch_PRJ);
    }

    printf("!!!------------------------------------------------------------------!!!\n");

    return (0);
}
