// *** THSI PROGRAM REPLACES ALL VOWELS IN THE INPUT STRING WITH THE * (asterisk) SYMBOL ***

#define MAX_STRING_LENGTH 512

int main(void)
{
    //fucntion prototype
    int MyStrlen(char[]);
    void MyStrcpy(char *str_destination, char *str_source);

    //variable declaration
    char chArray_PRJ[MAX_STRING_LENGTH]; // A character Array is a String
    char chArray_VowelsReplaced[MAX_STRING_LENGTH];
    int iStringLength_PRJ = 0;
    int i;

    //code

    //*** String Input ***
    printf("\n\n");
    printf("Enter A string : ");
    gets_s(chArray_PRJ, MAX_STRING_LENGTH);

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n %s \n\n", chArray_PRJ);

    // COPY STRING
    MyStrcpy(chArray_VowelsReplaced, chArray_PRJ);

    iStringLength_PRJ = MyStrlen(chArray_PRJ);

    for (i = 0; i < iStringLength_PRJ; i++)
    {
        switch (chArray_VowelsReplaced[i])
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
            chArray_VowelsReplaced[i] = '*';
            break;

        default:
            break;
        }
    }
    printf("\n\n");
    printf("String After Replacement Of Vowels By * Is :\n\n%s\n\n", chArray_VowelsReplaced);

    return (0);
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
