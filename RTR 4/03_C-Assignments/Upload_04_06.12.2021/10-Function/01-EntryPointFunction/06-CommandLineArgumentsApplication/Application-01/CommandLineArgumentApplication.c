#include <stdio.h>
#include <ctype.h>  //'ctype.h' contains declartion of 'atoi()'
#include <stdlib.h> //stdlib.h contains declartion of 'exit[]'

int main(int argc, char *argv[], char *envp[])
{
    //variable declartions
    int i;
    int num;
    int sum = 0;

    //code
    if (argc == 1)
    {
        printf("\n\n");
        printf("No Numbers Given For Addition !!! Exitting now...\n\n");
        printf("Usage: CommandLineArguemntsApplication <First number> <second Number>....\n\n");
        exit(0);
    }

    printf("\n\n");
    printf("Sum of All Integer Command Line Arguments Is : \n\n");
    for (i = 1; i < argc; i++)
    {
        num = atoi(argv[i]);
        sum = sum + num;
    }

    printf("Sum = %d\n\n", sum);

    exit(EXIT_SUCCESS);
}
