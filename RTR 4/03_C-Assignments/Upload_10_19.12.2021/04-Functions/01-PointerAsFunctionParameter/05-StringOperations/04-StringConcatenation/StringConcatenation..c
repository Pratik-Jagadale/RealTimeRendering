#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // fucntion prototype
    void MyStrcat(char *, char *);
    int MyStrlen(char *);

    //Variable declartions
    char *chArray_One_PRJ = NULL; //Character Array Can Be Represented By A char pointer to Mark The Base Address (char *)
    char *chArray_Two_PRJ;

    int iStringLength_PRJ = 0;

    // code
    printf("\n\n");
    chArray_One_PRJ = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_One_PRJ == NULL)
    {
        printf("MEMMORY ALLOCATION TO FIRST CHARACTER ARRAY FAILED !!! EXITING NOW...\n\n");
    }

    // STRING INPUT
    printf("ENTER A FIRST STRING : \n\n");
    gets_s(chArray_One_PRJ, MAX_STRING_LENGTH);

    printf("\n\n");
    chArray_Two_PRJ = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_Two_PRJ == NULL)
    {
        printf("MEMMORY ALLOCATION TO SECOND CHARACTER ARRAY FAILED !!! EXITING NOW...\n\n");
    }

    // STRING INPUT
    printf("ENTER A SECOND STRING : \n\n");
    gets_s(chArray_Two_PRJ, MAX_STRING_LENGTH);

    // STRING CONCAT
    printf("\n\n");
    printf("--- BEFORE CONCATENATION ---\n\n");
    printf("\n\n");
    printf("The Orginal First String Entred By You Is: %s\n\n", chArray_One_PRJ);

    printf("\n\n");
    printf("The Orginal Second String Entred By You Is: %s\n\n", chArray_Two_PRJ);

    MyStrcat(chArray_One_PRJ, chArray_Two_PRJ);

    // STRING OUTPUT

    printf("\n\n");
    printf("--- AFTER CONCATENATION ---\n\n");
    printf("\n\n");
    printf("The Orginal First String Entred By You Is: %s\n\n", chArray_One_PRJ);

    printf("\n\n");
    printf("The Orginal Second String Entred By You Is: %s\n\n", chArray_Two_PRJ);

    if (chArray_One_PRJ)
    {
        free(chArray_One_PRJ);
        chArray_One_PRJ = NULL;
        printf("MEMORY ALLOCATED FOR FIRST ORIGINAL STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    if (chArray_Two_PRJ)
    {
        free(chArray_Two_PRJ);
        chArray_Two_PRJ = NULL;
        printf("MEMORY ALLOCATED FOR SECOND STRING HAS BEEN SUCCESSFULLY FREED !!!\n\n");
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

void MyStrcat(char *str_destination, char *str_source)
{
    //fucntion prototype
    int MyStrlen(char *);

    // variable declartions
    int iStringLength_Source = 0, iStringLength_Destination = 0;
    int i, j;

    //code
    iStringLength_Source = MyStrlen(str_source);
    iStringLength_Destination = MyStrlen(str_destination);

    for (i = iStringLength_Destination, j = 0; j < iStringLength_Source; i++, j++)
        *(str_destination + i) = *(str_source + j);

    *(str_destination + i) = '\0';
}
