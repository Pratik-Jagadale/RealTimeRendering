#include <stdio.h>

/* Entry Point Function */
int main(int argc, char *argv[], char *envp[])
{
    //function prototype /declaration /signature
    int MyAddition(int, int);

    //varibale declartion
    int result, a, b;
    /* code */

    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' :");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value For 'B' :");
    scanf("%d", &b);

    result = MyAddition(a, b);
    printf("\n\n");
    printf("Sum of %d and %d is = %d \n\n", a, b, result);

    exit(0);
}

/* Function with Return Value  int , Parameters int a and int b*/
int MyAddition(int a, int b)
{
    //local variable declartion
    int sum = 0;

    //code
    sum = a + b;
    return (sum);
}
