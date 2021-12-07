#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int a_PRJ;

    /* code */
    printf("\n\n");

    a_PRJ = 10;

    if (a_PRJ) //non-zero positive value
    {
        printf("if-block 1: 'A' Exists And has vaue = %d !!!\n\n", a_PRJ);
    }

    a_PRJ = -5;
    if (a_PRJ) // non-zero negavtive
    {
        printf("if-block 2 :  'A' and has value = %d !!!\n\n", a_PRJ);
    }

    a_PRJ = 0;
    if (a_PRJ) //zero value
    {
        printf("if-block 3 : 'A' Exits has value = %d !!!\n\n", a_PRJ);
    }

    return (0);
}
