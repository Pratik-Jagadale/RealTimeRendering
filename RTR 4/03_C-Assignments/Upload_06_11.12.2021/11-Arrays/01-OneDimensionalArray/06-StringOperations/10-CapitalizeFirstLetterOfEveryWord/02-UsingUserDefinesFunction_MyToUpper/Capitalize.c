#include <stdio.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion prototype
    int MyStrlen(char[]);
    char MyToUpper(char);

    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    char chArray_CapitalizedFirstLetterOfEveryWord[MAX_STRING_LENGTH];
    int iStringLength_PRJ = 0;
    int i, j;

    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    iStringLength_PRJ = MyStrlen(chArray_PRJ);

    for (i = 0, j = 0; i < iStringLength_PRJ; i++)
    {
        if (i == 0)
        {
            chArray_CapitalizedFirstLetterOfEveryWord[i] = MyToUpper(chArray_PRJ[i]);
        }
        else if (chArray_PRJ[i] == ' ')
        {
            chArray_CapitalizedFirstLetterOfEveryWord[j] = chArray_PRJ[i];
            chArray_CapitalizedFirstLetterOfEveryWord[j + 1] = MyToUpper(chArray_PRJ[i + 1]);

            i++;
            j++;
        }
        else
            chArray_CapitalizedFirstLetterOfEveryWord[j] = chArray_PRJ[i];
        j++;
    }

    chArray_CapitalizedFirstLetterOfEveryWord[j] = '\0';

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n %s \n\n", chArray_PRJ);

    printf("\n\n");
    printf("String After Capitalizing First Letter Of Every Word : \n\n %s \n\n", chArray_CapitalizedFirstLetterOfEveryWord);

    return (0);
}

char MyToUpper(char ch)
{
    int num_PRJ;
    int c;

    num_PRJ = (int)'a' - (int)'A'; // ASCII VALUE OF 'a' (97) - ASCII VALUE OF 'A' (65) = 32

    if ((int)ch >= 97 && (int)ch <= 122) // ASCII VALUES OF 'a' to 'z' => 97 TO 122, ASCII VALUES OF 'A' to 'Z' => 65 TO 90
    {
        c = (int)ch - num_PRJ;
        return ((char)c);
    }
    else
        return (ch);
}

int MyStrlen(char str_PRJ[])
{
    //variable declartion
    int i;
    int string_length_PRJ = 0;

    //code
    for (i = 0; i < MAX_STRING_LENGTH; i++)
    {
        if (str_PRJ[i] == '\0')
            break;
        string_length_PRJ++;
    }
    return (string_length_PRJ);
}
