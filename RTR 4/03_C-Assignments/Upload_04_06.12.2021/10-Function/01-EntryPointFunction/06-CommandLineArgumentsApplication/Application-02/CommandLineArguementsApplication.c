#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    //variable declartions
    int i;

    //code
    if (argc != 4)
    {
        printf("\n\n");
        printf("Invalid Usgae !!! Exitting now...\n\n");
        printf("Usage: CommandLineArgumentsApplication <First nume> <middle name> <surename>....\n\n");
        exit(0);
    }

    printf("\n\n");
    printf("Your Full Name Is :");
    for (i = 1; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n\n");

    exit(EXIT_SUCCESS);
}
