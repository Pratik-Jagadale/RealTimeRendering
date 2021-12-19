#include <stdio.h>
#include <String.h> // for strrev()

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion protoype
    void MyStrcat(char *, char *);

    //variable declaration
    char chArray_One_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    char chArray_Two_PRJ[MAX_STRING_LENGTH];
    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_One_PRJ, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("Enter B string : ");
    gets_s(chArray_Two_PRJ, MAX_STRING_LENGTH);

    // *** STRING OUTPUT
    printf("*** BEFORE CONVATENATION ***");
    printf("\n\n");
    printf("The Orginal String  entred By You (i.e chArray_One_PRJ[] )Is : : \n\n %s \n\n", chArray_One_PRJ);

    printf("\n\n");
    printf("The Orginal String  entred By You (i.e chArray_Two_PRJ[] )Is : : \n\n %s \n\n", chArray_Two_PRJ);

    MyStrcat(chArray_One_PRJ, chArray_Two_PRJ);

    printf("\n\n");
    printf("*** AFTER CONCATENATION ***");
    printf("\n\n");
    printf("chArray_One_PRJ[] (Destination) Is : : \n\n %s \n\n", chArray_One_PRJ);

    printf("\n\n");
    printf("chArray_Two_PRJ[] Is : : \n\n %s \n\n", chArray_Two_PRJ);

    return (0);
}

void MyStrcat(char *str_destrination_PRJ, char *str_source_PRJ)
{
    //fucntion prototype
    int MyStelen(char[]);

    //variable declaration
    int iStringLength_Source_PRJ = 0, iStringLength_Destriantion_PRJ = 0;
    int i, j;

    //code
    iStringLength_Source_PRJ = MyStrlen(str_source_PRJ);
    iStringLength_Destriantion_PRJ = MyStrlen(str_destrination_PRJ);

    // ARRAY INDICES BEGIN FROM 0, HENCE, LAST VALID INDEX OF ARRAY WILL ALWAYS BE
    // (LENGTH - 1)
    // SO, CONCATENATION MUST BEGIN FROM INDEX NUMBER EQUAL TO LENGTH OF THE ARRAY
    // 'str_destination'
    // WE NEED TO PUT THE CHARACTER WHICH IS AT FIRST INDEX OF 'str_source' TO THE
    // (LAST INDEX + 1) OF 'str_destination'

    for (i = iStringLength_Destriantion_PRJ, j = 0; j < iStringLength_Source_PRJ; i++, j++)
    {
        str_destrination_PRJ[i] = str_source_PRJ[j];
    }
    str_destrination_PRJ[i] = '\0';
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
