//Header Files
#include <stdio.h>

// Constants
#define INT_ARRAY_SIZE 10
#define FLOAT_ARRAY_SIZE 5
#define CHAR_ARRAY_SIZE 26

#define NUM_STRING 10
#define MAX_CHARACATERS_PER_STRING 20

#define ALPHABET_BEGINNING 65 //'A'

// struct definition
struct MyDataOne
{
    int iArray[INT_ARRAY_SIZE];
    float fArray[FLOAT_ARRAY_SIZE];
};

struct MyDataTwo
{
    char cArray[CHAR_ARRAY_SIZE];
    char strArray[NUM_STRING][MAX_CHARACATERS_PER_STRING];
};

// Entry Point Function
int main(void)
{
    //Variable Declaration
    struct MyDataOne data_one;
    struct MyDataTwo data_two;
    int i;

    //code
    // *** PORCE-MEAL ASSIGNMENT ***
    data_one.fArray[0] = 0.1f;
    data_one.fArray[1] = 1.1f;
    data_one.fArray[2] = 2.1f;
    data_one.fArray[3] = 3.1f;
    data_one.fArray[4] = 4.1f;

    // *** LOOP ASSIGNMENT ***
    for (i = 0; i < 10; i++)
        data_one.iArray[i] = 111 + (111 * i);

    for (i = 0; i < CHAR_ARRAY_SIZE; i++)
        data_two.cArray[i] = (char)(i + ALPHABET_BEGINNING);

    // ** PIECEL-MEAL ASSIGNMENT ***
    strcpy(data_two.strArray[0], "Welcome !!!");
    strcpy(data_two.strArray[1], "This");
    strcpy(data_two.strArray[2], "Is ");
    strcpy(data_two.strArray[3], "ASTROMEDICOMP ");
    strcpy(data_two.strArray[4], "Real");
    strcpy(data_two.strArray[5], "Time");
    strcpy(data_two.strArray[6], "Rendering ");
    strcpy(data_two.strArray[7], "BATCH");
    strcpy(data_two.strArray[8], "OF");
    strcpy(data_two.strArray[9], "2021-22");

    // *** DISPLAYING DATA MEMBERS OF 'STRUCT DataOne' THEIR VALUES ***
    printf("\n\n");
    printf("Members of 'struct DataOne' Algorithm Their Assigned vlalues Are :\n");
    printf("\n\n");

    printf("Integer Array (data_one.iArray[]) : \n\n");
    for (i = 0; i < INT_ARRAY_SIZE; i++)
        printf("Data_one.iArray[%d] = [%d]\n", i, data_one.iArray[i]);

    printf("\n\n");
    printf("Floating-Point Array (data_one.fArray[]) : \n\n");
    for (i = 0; i < FLOAT_ARRAY_SIZE; i++)
        printf("Data_one.fArray[%d] = [%f \n]", i, data_one.fArray[i]);

    // *** DIPLAYING DATA-MEMBERS OF  'struct DataTwo' AND THEIR VALUES ***
    printf("\n\n");
    printf("Members of 'struct DataTwo' Algorithm Their Assigned vlalues Are :\n");
    printf("\n\n");

    printf("Character Array (data_two.cArray[]) : \n\n");
    for (i = 0; i < CHAR_ARRAY_SIZE; i++)
        printf("Data_two.cArray[%d] =%c \n", i, data_two.cArray[i]);

    printf("\n\n");
    printf("String Array (data_two.strArray[]) : \n\n");
    for (i = 0; i < NUM_STRING; i++)
        printf("Data_two.strArray[%d] = %s\n", i, data_two.strArray[i]);

    return (0);
}