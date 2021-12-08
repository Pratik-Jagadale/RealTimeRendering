#include <stdio.h>

int main()
{
    /*fucntion prototypes (function declartions) */
    void printBinaryFormOfNumber(unsigned int);

    /* variable declartions */
    unsigned int a;
    unsigned int b;
    unsigned int result;

    /* code */
    printf("\n\n");
    printf("Enter An Interger = ");
    scanf("%u", &a);

    printf("\n\n");
    printf("Enter another Interger = ");
    scanf("%d", &b);

    printf("\n\n\n\n");
    result = a | b;
    printf("Bitwise OR-ing Of \nA=%d (decimal) and BS= %d (decimal ) gives result %d (decimal).\n\n", a, b, result);

    printBinaryFormOfNumber(a);
    printBinaryFormOfNumber(b);
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
