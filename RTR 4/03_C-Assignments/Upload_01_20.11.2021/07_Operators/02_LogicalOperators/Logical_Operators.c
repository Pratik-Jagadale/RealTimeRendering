/*
    Assigment : Logical Operators
    date: 2021_21_11
*/

/* Header Files */
#include <stdio.h>

/* Entry Point Function*/
int main(void)
{

    /* variable declartions */
    int a;
    int b;
    int c;
    int result;

    /* code */
    printf("\n\n");
    printf("Enter First Integer : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Second Integer : ");
    scanf("%d", &b);

    printf("\n\n");
    printf("Enter Third inetger : ");
    scanf("%d", &c);

    printf("\n\n");
    printf("If Answer = 0 , It Is 'false'. \n");
    printf("If Answer = 1, It Is 'True'. \n\n");

    result = (a <= b) && (b != c);

    printf("LOGICAL AND (&&): Answer is TRUE (1) If and Only If Both Condition are True .The Answer isFalse (0), If Any Or both conditions are flase\n\n");

    printf("A = %d Is Less Than or Equal To B = %d And B = %d Is Not Equal to c = %d \t answer = %d \n\n", b, a, a, c, result);

    result = (b >= a) || (a == c);
    printf("LOGICAL OR (||) : Answer is False (0) If And Only If Both Conditon Are False. The answer is True (1) , If Any onre or Both Condtions are true \n\n ");
    printf("Either B= %d Is greater Than or Equal To A= %d OR a= %d Is Equal to C =%d \t Answer = %d \n\n", b, a, a, c, result);

    result = !a;
    printf("A= %d And using Logical NOT (! )  Operator on A Gives Result =%d \n\n ", a, result);

    result = !b;
    printf("B= %d And Using Logical Not (!) Operator on b Gives Result = %d\n\n", b, result);

    result = !c;
    printf("C = %d And using Logical NOT (!) Opertor on B gives Reslt = %d \n\n", c, result);

    result = (!(a <= b) && !(b != c));
    printf("Using Logical NOT (!) On (a <= b)  And Also On (b != c) And  theing Them Afterwards Gives Result = %d\n", result);

    printf("\n\n");

    result = !((b >= a) || (a == c));
    printf("Using Logical NOT (!) On Entire Logical Expression (b >= a) || (a == c) Gives Result = %d \n", result);

    printf("\n\n");
    return (0);
}
