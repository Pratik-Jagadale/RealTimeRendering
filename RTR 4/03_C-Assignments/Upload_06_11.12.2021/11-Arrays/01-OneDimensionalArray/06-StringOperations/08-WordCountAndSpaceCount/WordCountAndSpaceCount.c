#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion prototype
    int MyStrlen(char[]);
    void MyStrcpy(char *str_destination, char *str_source);

    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    int iStringLength_PRJ = 0;
    int i, iWordCount = 0, iSpaceCount = 0;

    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    iStringLength_PRJ = MyStrlen(chArray_PRJ);

    iStringLength_PRJ = MyStrlen(chArray_PRJ);

    for (i = 0; i < iStringLength_PRJ; i++)
    {
        switch (chArray_PRJ[i])
        {
        case 32: //32 Is The Ascci value for space (' ') Character
            iSpaceCount++;
            break;

        default:
            break;
        }
    }
    iWordCount = iSpaceCount + 1;

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n %s \n\n", chArray_PRJ);

    printf("\n\n");
    printf("Number of Spaces In The Input String = %d \n\n", iSpaceCount);
    printf("Number of Words In The Input String = %d \n\n", iWordCount);

    printf("Number of Words USING word_Count() Function In The Input String = %d \n\n", word_count(chArray_PRJ));

    return (0);
}

int word_count(char *Arr)
{
    int i;
    int iStr_Len_PRJ = MyStrlen(Arr);
    int iWordCount = 0;

    for (i = 0; i < iStr_Len_PRJ; i++)
    {
        while (Arr[i] == 32) // Iterates All immediate SPACES
            i++;

        while (Arr[i] != 32) //Iterates all immedite characters
            i++;
        iWordCount = iWordCount + 1;
    }
    return (iWordCount);
}

void MyStrcpy(char *str_destination, char *str_source)
{
    //variable declartions
    int iStringLength = 0;
    int i;

    //code
    iStringLength = MyStrlen(str_source);

    for (i = 0; i < iStringLength; i++)
    {
        str_destination[i] = str_source[i];
    }
    str_destination[i] = '\0';
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
