#include <stdio.h>

/*
    Entry Point Function => main() => valid Return Type (int) and 3 Pqarameters
    (int argc, char* argv[], char *envp[])
*/
int main(int argc, char *argv[], char *envp[])
{
    //variable declartion
    int i;

    //code
    printf("\n\n");

    printf("Hello Wolrd !!!\n\n");

    printf("Number of Command Line Arguments = %d\n\n", argc);

    printf("Command Line Arguments Passed To this Program are : \n\n");
    for (i = 0; i < argc; i++)
    {
        printf("Command Line Argument Number %d = %s\n", (i + 1), argv[i]);
    }
    printf("\n\n");

    printf("All Environmental Variables Passed To This Program  Are: \n\n");
    for (i = 0; i < 86; i++)
    {
        printf("Enviromental Variable Number %d = %s\n", (i + 1), envp[i]);
    }
    printf("\n\n");
}
