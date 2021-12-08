#include <stdio.h>

int main(void)
{

    /* Variable declartions */
    int i, j;
    char ch_01, ch_02;

    int a, result_int;
    float f, result_float;

    int i_explicit;
    float f_explicit;

    /* code */
    printf("\n\n");

    /* INTERCONVERSION AND IMPLICIT TYPE-CASTING BETWEEN 'char' and 'int' TYpes... */
    i = 70;
    ch_01 = i;
    printf("I = %d\n", i);
    printf("Character 1 (after ch_01 = i ) = %c \n\n", ch_01);

    ch_02 = 'Q';
    j = ch_02;
    printf("j (After j = ch_02) = %d\n\n", j);

    /* Implicit CONVERSION OF 'int' To  'float' */
    a = 5;
    f = 7.8f;
    result_float = a + f;
    printf("Integer a = %d And Floating-point number %f Added Gives Floating-Point Sum = %f \n\n", a, f, result_float);

    result_int = a + f;
    printf("Integer a =%d And floating-point Number %f Added Gives Integer Sum = %d\n\\n", a, f, result_int);

    /* Explicit Type Conversion */
    f_explicit = 30.232995f;
    i_explicit = (int)f_explicit;
    printf("Floating Point  Number Which Will Be Type Casted Explicitely = %f \n", f_explicit);

    printf("Resultant Integer after Explicit type Casting Of %f = %d \n\n", f_explicit, i_explicit);

    return (0);
}
