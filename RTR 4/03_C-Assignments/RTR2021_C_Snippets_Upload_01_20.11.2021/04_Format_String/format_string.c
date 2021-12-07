/*
    Assignmet: Format String
    date: 2021_11_21
*/
/* Header Files */
#include <stdio.h>

/* Entry Point Function */
int main(void)
{
    printf("-----------------------------------------------------------------------------\n");
    printf("Format Strings\n\n");
    printf("-----------------------------------------------------------------------------\n");

    int a = 13;
    printf("interger Decimal value of 'a' = %d\n", a);
    printf("interger Octal Value of 'a' =%o\n", a);
    printf("Interger Hexadecimal Value of 'a' (Hexadecimal Letters in Lower Case) = %x\n", a);
    printf("Interger Hexadecimal Value of 'a' (Hexadecimal Letters in Upper Case) = %X\n", a);
    printf("-----------------------------------------------------------------------------\n");

    char ch = 'P';
    printf("Charater ch =%c\n", ch);
    char str[] = "AstroMedicomp's Real Time Readering Batch";
    printf("String str = %s\n", str);
    printf("-----------------------------------------------------------------------------\n");

    long num = 30121995L;
    printf("Long Interger = %ld\n", num);
    printf("-----------------------------------------------------------------------------\n");

    unsigned int b = 7;
    printf("Long Integer = %u\n", b);
    printf("-----------------------------------------------------------------------------\n");

    float f_num = 3012.1995f;
    printf("Floating Point Number With Just %%f 'f_num' =%f\n", f_num);
    printf("Floating Point Number With %%4.2f 'f_num' = %2.3f\n ", f_num);
    printf("Floating Point Number With %%6.5f 'f_num' = %6.5f\n", f_num);
    printf("-----------------------------------------------------------------------------\n");

    double P_pi = 3.14159265358979323846;
    printf("Double Precision Floating Point Number wihtout Exponential = %g\n", P_pi);
    printf("Double Precision Floating Point Number With Exponential (Lowecase) = %e\n", P_pi);
    printf("Double Precision Floating Point Number with Exponential (Uppercase) = %E\n\n", P_pi);
    printf("Double Hexadecimal Value of 'P_pi' (Hexadecimal Letters in Lowercase) = %a\n", P_pi);
    printf("Double Hexadecimal Value of 'P_pi' (Hexadecimal Letters In UpperCase) = %A\n\n", P_pi);
    printf("-----------------------------------------------------------------------------\n");

    return (0);
}
