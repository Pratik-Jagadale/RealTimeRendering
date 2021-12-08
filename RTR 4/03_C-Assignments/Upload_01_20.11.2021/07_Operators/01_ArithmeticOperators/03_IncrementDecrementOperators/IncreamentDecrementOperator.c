/*
    Assignment: Increment Decrement Operators
    date: 2021_21_11
*/

/* Header File */
#include <stdio.h>

/* Entry Point Function*/
int main(void)
{
    int a = 5;
    int b = 10;

    /* code */
    printf("\n\n");
    printf("Increment Operator\n");
    printf("A = %d\n", a);                           //5
    printf("A = %d\tPostIncrement Operator\n", a++); //5        /* PostIncrement Operator*/
    printf("A = %d\n", a);                           //6
    printf("A = %d\tPreIncrement Operator\n ", ++a); //7        /* PreIncrement Operator */

    printf("Decrement Operator\n");
    printf("B = %d\n", b);                           //10
    printf("B = %d\tPostDecrement Operator\n", b--); //10       /* PostDecrement Operator */
    printf("B = %d\n", b);                           //9
    printf("B = %d\tPreDecrement Operator\n", --b);  //8        /* PreDecrement Operator */

    return (0);
}