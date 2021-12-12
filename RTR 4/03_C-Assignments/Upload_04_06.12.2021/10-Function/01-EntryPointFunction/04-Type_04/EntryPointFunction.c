#include <stdio.h>

/*
    Entry-Point Function => main() valid Return Type (int)
    and 2 Parameters (int argc, char * argc[])
*/

int main(int argc, char *argv[])
{
    //variable declaration
    int i;

    /* code */
    printf("\n\n");
    printf("Hello wolrd !!!!\n");
    printf("Number of Command Line Arguments = %d\n\n", argc);

    printf("Command Line Arguments Passed To this Program are : \n\n");
    for (i = 0; i < argc; i++)
    {
        printf("Command Line Argument Number %d = %s\n", (i + 1), argv[i]);
    }
    printf("\n\n");
    return (0);
}
