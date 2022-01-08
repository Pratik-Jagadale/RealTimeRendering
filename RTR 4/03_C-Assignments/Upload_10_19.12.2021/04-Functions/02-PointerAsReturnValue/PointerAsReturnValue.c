#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion declartions
    char *ReplaceVowelsWithHashSymbol(char *);

    //variable declartions
    char string_PRJ[MAX_STRING_LENGTH];
    char *replaced_string = NULL;

    //code
    printf("\n\n");
    printf("Enter String : ");
    gets_s(string_PRJ, MAX_STRING_LENGTH);

    replaced_string = ReplaceVowelsWithHashSymbol(string_PRJ);

    if (replaced_string == NULL)
    {
        printf("ReplaceVowelsWithHashSymbol() function Has Failed !!! Exitting now...\n\n");
        exit(0);
    }
    printf("\n\n");
    printf("Replaced String Is : \n\n");
    printf("%s\n\n", replaced_string);

    if (replaced_string)
    {
        free(replaced_string);
        replaced_string = NULL;
    }
    return (0);
}

char *ReplaceVowelsWithHashSymbol(char *s)
{
    //fucntion prototype
    void MyStrcpy(char *, char *);
    int MyStrlen(char *);

    //variable declartions
    char *new_string_PRJ = NULL;
    int i;

    //code
    new_string_PRJ = (char *)malloc(MyStrlen(s) * sizeof(char));
    if (new_string_PRJ == NULL)
    {
        printf("COULD NOT ALLOCATE MEMORY FOR NEW STRING !!!\n\n");
        return (NULL);
    }

    MyStrcpy(new_string_PRJ, s);
    for (i = 0; i < MyStrlen(new_string_PRJ); i++)
    {

        switch (new_string_PRJ[i])
        {
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
            new_string_PRJ[i] = '#';
            break;
        default:
            break;
        }
    }
    return (new_string_PRJ);
}

int MyStrlen(char *str)
{
    // Variable declarations
    int j;
    int string_length_PRJ = 0;

    // code
    /* DETERMINING EAXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE
        OF NULL-TERMINATIING CHARCTER
     */
    for (j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if (*(str + j) == '\0')
            break;
        else
            string_length_PRJ++;
    }
    return (string_length_PRJ);
}

void MyStrcpy(char *str_destination, char *str_source)
{
    //fucntion prototype
    int MyStrlen(char *);

    // variable declartions
    int iStringLength = 0;
    int j;

    //code
    iStringLength = MyStrlen(str_source);
    for (j = 0; j < iStringLength; j++)
        *(str_destination + j) = *(str_source + j);

    *(str_destination + j) = '\0';
}
