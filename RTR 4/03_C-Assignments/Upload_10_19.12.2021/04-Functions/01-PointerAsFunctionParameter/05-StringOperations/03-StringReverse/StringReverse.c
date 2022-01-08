#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // fucntion prototype
    void MyStrrev(char *, char *);
    int MyStrlen(char *);

    //Variable declartions
    char *chArray_PRJ = NULL; //Character Array Can Be Represented By A char pointer to Mark The Base Address (char *)
    char *chArray_Reversed_PRJ;

    int iStringLength_PRJ = 0;

    // code
    printf("\n\n");
    chArray_PRJ = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_PRJ == NULL)
    {
        printf("MEMMORY ALLOCATION TO ORINAL CHARACTER ARRAY FAILED !!! EXITING NOW...\n\n");
    }

    // STRING INPUT
    printf("ENTER A STRING : \n\n");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    // STRING LENGTH
    printf("\n\n");
    iStringLength_PRJ = MyStrlen(chArray_PRJ);

    chArray_Reversed_PRJ = (char *)malloc(iStringLength_PRJ * sizeof(char));
    if (chArray_PRJ == NULL)
    {
        printf("MEMMORY ALLOCATION TO REVERSED CHARACTER ARRAY FAILED !!! EXITING NOW...\n\n");
    }

    // STRING COPY
    MyStrrev(chArray_Reversed_PRJ, chArray_PRJ);

    // STRING OUTPUT
    printf("\n\n");
    printf("The Origninal String Entered By You (i.e : 'chArray_PRJ') Is\n\n %s\n\n", chArray_PRJ);

    printf("\n\n");
    printf("The Reversed String Entered By You (i.e : 'chArray_Reversed_PRJ') Is\n\n %s\n\n", chArray_Reversed_PRJ);

    if (chArray_PRJ)
    {
        free(chArray_PRJ);
        chArray_PRJ = NULL;
        printf("MEMORY ALLOCATED FOR ORIGINAL STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    if (chArray_Reversed_PRJ)
    {
        free(chArray_Reversed_PRJ);
        chArray_Reversed_PRJ = NULL;
        printf("MEMORY ALLOCATED FOR REVERESD STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
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

void MyStrrev(char *str_destination, char *str_source)
{
    //fucntion prototype
    int MyStrlen(char *);

    // variable declartions
    int iStringLength = 0;
    int i, j;

    //code
    iStringLength = MyStrlen(str_source);

    // ARRAY INDICES BEGIN FROM 0, HENCE LAST INDEX WILL ALWAYS BE (Lenth - 1)
    iStringLength = iStringLength - 1;

    for (i = 0, j = iStringLength; j >= 0; j--, i++)
        *(str_destination + i) = *(str_source + j);

    *(str_destination + i) = '\0';
}
