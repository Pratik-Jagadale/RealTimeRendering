#include <stdio.h>

/* Entry Point Function */
int main(int argc, char *argv[], char *envp[])
{
    //function prototype /declaration /signature
    int MyAddition(void);

    //varibale declartion
    int result;
    /* code */
    result = MyAddition();
    printf("\n\n");
    printf("Sum of A and B is = %d \n\n", result);

    exit(0);
}

/* Function with Return Value, No Parameters */
int MyAddition(void)
{
    //variable declaration : local variable to MyAddition()
    int a, b, sum;

    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' :");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value For 'B' :");
    scanf("%d", &b);

    sum = a + b;

    return (sum);
}
