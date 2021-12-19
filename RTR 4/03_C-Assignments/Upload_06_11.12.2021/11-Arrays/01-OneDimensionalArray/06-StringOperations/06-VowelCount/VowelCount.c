#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion prototype
    int MyStrlen(char[]);

    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    int iStringLength_PRJ = 0;
    int count_A = 0, count_E = 0, count_I = 0, count_O = 0, count_U = 0; //iantial count 0
    int i;

    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n %s \n\n", chArray_PRJ);

    iStringLength_PRJ = MyStrlen(chArray_PRJ);

    for (i = 0; i < iStringLength_PRJ; i++)
    {
        switch (chArray_PRJ[i])
        {
        case 'A':
        case 'a':
            count_A++;
            break;
        case 'E':
        case 'e':
            count_E++;
            break;

        case 'I':
        case 'i':
            count_I++;
            break;
        case 'O':
        case 'o':
            count_O++;
            break;
        case 'U':
        case 'u':
            count_U++;
            break;
        default:
            break;
        }
    }
    printf("\n\n");
    printf("In the String Entered By you, The Vowels and the Number of Their Occurences Are Follows :\n\n");

    printf("'A' HAS occured = %d Times !!!\n\n", count_A);
    printf("'E' HAS occured = %d Times !!!\n\n", count_E);
    printf("'I' HAS occured = %d Times !!!\n\n", count_I);
    printf("'O' HAS occured = %d Times !!!\n\n", count_O);
    printf("'U' HAS occured = %d Times !!!\n\n", count_U);

    return (0);
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
