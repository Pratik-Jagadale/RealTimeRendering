#include <stdio.h>

/* Entry Point Function */
int main(int argc, char *argv[], char *envp[])
{
    //function prototype /declaration /signature
    void MyAddition(void);

    /* code */
    MyAddition();

    exit(0);
}
/* Function with No Return Value, No Parameters */

void MyAddition(void)
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

    printf("\n\n");
    printf("Sum of  %d and %d is %d \n\n", a, b, sum);
}
