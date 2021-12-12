#include <stdio.h>

/* Entry Point Function */
int main(int argc, char *argv[], char *envp[])
{
    //function prototype /declaration /signature
    void MyAddition(int, int);

    //varibale declartion
    int a, b;
    /* code */

    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' :");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value For 'B' :");
    scanf("%d", &b);

    MyAddition(a, b);
    exit(0);
}

/* Function with No Return Value, Parameters */
void MyAddition(int a, int b)
{
    //varibale declartions
    int sum;
    //code
    sum = a + b;

    printf("\n\n");
    printf("Sum of %d and %d is = %d \n\n", a, b, sum);
}
