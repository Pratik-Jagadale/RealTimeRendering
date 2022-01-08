#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // fucntion prototype
    int MyStrlen(char *);

    //Variable declartions
    char *chArray_PRJ = NULL; //Character Array Can Be Represented By A char pointer to Mark The Base Address (char *)

    int iStringLength_PRJ = 0;

    // code
    printf("\n\n");
    chArray_PRJ = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_PRJ == NULL)
    {
        printf("MEMMORY ALLOCATION TO CHARACTER ARRAY !!! EXITING NOW...\n\n");
    }

    // STRING INPUT
    printf("ENTER A STRING : \n\n");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    // STRING OUTPUT
    printf("\n\n");
    printf("String Entered By You Is : %s\n", chArray_PRJ);

    // STRING LENGTH
    printf("\n\n");
    iStringLength_PRJ = MyStrlen(chArray_PRJ);
    printf("Length of String is = %d Characters !!!\n\n", iStringLength_PRJ);

    if (chArray_PRJ)
    {
        free(chArray_PRJ);
        chArray_PRJ = NULL;
    }
    return (0);
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
