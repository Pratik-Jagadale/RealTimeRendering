#include <stdio.h>

int main(void)
{
    /* function prototype */
    void printBinaryFormOfNumber(unsigned int);

    /* varibale declartion */
    unsigned int a;
    unsigned int result;

    /* code */
    printf("\n\n");
    printf("Enter an Integer = ");
    scanf("%u", &a);

    printf("\n\n\n\n");
    result = ~a;
    printf("Bitwise COMPLEMENTING of \n A = %d(Decimal) gives result %d (Decimal).\n\n", a, result);

    printBinaryFormOfNumber(a);
    printBinaryFormOfNumber(result);

    return (0);
}

void printBinaryFormOfNumber(unsigned int decimal_number)
{
    /* variable declarations */
    unsigned int quotient, remainder;
    unsigned int num;
    unsigned int binary_array[8];
    int i;

    /* code */
    for (i = 0; i < 8; i++)
        binary_array[i] = 0;

    printf("The Binary form of The Decimal Integer %d Is \t=\t", decimal_number);
    num = decimal_number;
    i = 7;
    while (num != 0)
    {
        quotient = num / 2;
        remainder = num % 2;
        binary_array[i] = remainder;
        num = quotient;
        i--;
    }

    for (i = 0; i < 8; i++)
        printf("%u", binary_array[i]);

    printf("\n\n");
}
