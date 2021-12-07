/*
    Assignment: Compound Assignment Operators
    date: 2021_21_11
*/

/* Header File */
#include <stdio.h>

/* Entry Point Function*/
int main(void)
{
    /* variable declartion */
    int a;
    int b;
    int x;

    /* code */
    printf("\n\n");
    printf("Enter A Number : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter another number : ");
    scanf("%d", &b);

    printf("\n\n");

    /*  Since, In All The Following 5 Cases , The Operand on the left 'a' Is 
        getting repeated immeiately On the Right(eg. a: = a + 1 or a = a - b)

        We are using compound Assigment Operators +=,-=,*=,/= and %=    
    */

    /*since , 'a' Will Be Assignment The Value of (a + b) at The Expression (a += b) */
    x = a;
    a += b; /* a = a + b */
    printf("Addition of A = %d And B = %d Gives %d\n", x, b, a);

    /* Since , 'a' Will Be Assigment The value of (a-b) ate the expression (a -= b) */
    x = a;
    a -= b; /* a = a - b*/
    printf("Substraction of A = %d And B = %d.\n", x, b, a);

    /* Since , 'a' Will be assigned The alue of (a*b) at the expression (a*=b) */
    x = a;
    a *= b; /* a = a * b */
    printf("Multilpication of A = %d And B = %d Gives %d.\n", x, b, a);

    /*Since 'a' will be assigned the value of (a /b ) at the expression (a /=b)*/
    x = a;
    a /= b; /* a = a / b*/
    printf("Division of A = %d And  B =%d Gives Quotient %d.\n", x, b, a);

    /*Since 'a' Will be assigned The value of (a % b) at the expression (a %= b)*/
    x = a;
    a %= b; /*a = a % b */
    printf("Divsion of A = %d And B = %d gives Remainder = %d\n", x, b, a);

    printf("\n---------------------------------------------------------\n");
    return (0);
}
