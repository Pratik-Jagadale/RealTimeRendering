#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_STRING_LENGTH 1024

struct CharacterCount
{
    char ch_PRJ;
    int ch_count_PRJ;
} character_and_count_PRJ[] = {
    {'A', 0},
    {'B', 0},
    {'C', 0},
    {'D', 0},
    {'E', 0},
    {'F', 0},
    {'G', 0},
    {'H', 0},
    {'I', 0},
    {'J', 0},
    {'K', 0},
    {'L', 0},
    {'M', 0},
    {'N', 0},
    {'O', 0},
    {'P', 0},
    {'Q', 0},
    {'R', 0},
    {'S', 0},
    {'T', 0},
    {'U', 0},
    {'V', 0},
    {'W', 0},
    {'X', 0},
    {'Y', 0},
    {'Z', 0}}; // character_and_count[25].ch = 'z' &
               // character_and_count[25].ch_count = 0

#define SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS sizeof(character_and_count_PRJ)
#define SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS sizeof(character_and_count_PRJ[0])
#define NUM_ELEMENTS_IN_ARRAY (SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS / SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS)

//ENRTY POINT FUCNTION
int main(void)
{
    //variable declartion
    char str_PRJ[MAX_STRING_LENGTH];
    int i, j, actual_string_length = 0;

    // code
    printf("\n\n");
    printf("Enter A string : \n\n");
    gets_s(str_PRJ, MAX_STRING_LENGTH);

    actual_string_length = strlen(str_PRJ);

    printf("\n\n");
    printf("The string You Have Entered Is :\n\n");
    printf("%s\n\n", str_PRJ);

    for (i = 0; i < actual_string_length; i++)
    {
        for (j = 0; j < NUM_ELEMENTS_IN_ARRAY; j++)
        {
            str_PRJ[i] = toupper(str_PRJ[i]); //If Character in lower case then convert it to Upper case

            if (str_PRJ[i] == character_and_count_PRJ[j].ch_PRJ) //If Character is Present
                character_and_count_PRJ[j].ch_count_PRJ++;       //Increment its Coutn by 1...
        }
    }

    printf("\n\n");
    printf("The Number Of Occurences of All character from the alphabet are as follows :\n\n");
    for (i = 0; i < NUM_ELEMENTS_IN_ARRAY; i++)
    {
        printf("Character %c = %d\n", character_and_count_PRJ[i].ch_PRJ, character_and_count_PRJ[i].ch_count_PRJ);
    }
    printf("\n\n");

    return (0);
}
