#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // fucntion prototype
    void MyStrcpy(char *, char *);
    int MyStrlen(char *);

    //Variable declartions
    char *chArray_PRJ = NULL; //Character Array Can Be Represented By A char pointer to Mark The Base Address (char *)
    char *chArray_Copy_PRJ;

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

    chArray_Copy_PRJ = (char *)malloc(iStringLength_PRJ * sizeof(char));
    if (chArray_PRJ == NULL)
    {
        printf("MEMMORY ALLOCATION TO COPIED CHARACTER ARRAY FAILED !!! EXITING NOW...\n\n");
    }

    // STRING COPY
    MyStrcpy(chArray_Copy_PRJ, chArray_PRJ);

    // STRING OUTPUT
    printf("\n\n");
    printf("The Origninal String Entered By You (i.e : 'chArray_PRJ') Is\n\n %s\n\n", chArray_PRJ);

    printf("\n\n");
    printf("The Copied String Entered By You (i.e : 'chArray_Copy_PRJ') Is\n\n %s\n\n", chArray_Copy_PRJ);

    if (chArray_PRJ)
    {
        free(chArray_PRJ);
        chArray_PRJ = NULL;
        printf("MEMORY ALLOCATED FOR ORIGINAL STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    if (chArray_Copy_PRJ)
    {
        free(chArray_Copy_PRJ);
        chArray_Copy_PRJ = NULL;
        printf("MEMORY ALLOCATED FOR COPIED STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
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