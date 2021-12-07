/*
    Assignment: TheFive Arithmetic Operators And Assignment Operators
    date: 2021_21_11
*/

#include <stdio.h>

int main()
{
    /* local variable declartions :.data section*/
    int a;
    int b;
    int result;

    /* Code */
    printf("\n---------------------------Operators---------------------------\n");
    printf("Enter A number :");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter B Number :");
    scanf("%d", &b);

    result = a + b;
    printf("Addition of A = %d And b = %d Gives %d.\n", a, b, result);

    result = a - b;
    printf("Substraction of A = %d And b = %d Gives %d.\n", a, b, result);

    result = a * b;
    printf("Multiplication of A = %d And b = %d Gives %d.\n", a, b, result);

    result = a / b;
    printf("Division of A = %d And b = %d Gives Quotitent %d.\n", a, b, result);

    result = a % b; /* Modulo Operator */
    printf("Divison of A = %d And b = %d Gives Eemainder %d.\n", a, b, result);

    printf("\n---------------------------------------------------------------\n");

    return (0);
}
