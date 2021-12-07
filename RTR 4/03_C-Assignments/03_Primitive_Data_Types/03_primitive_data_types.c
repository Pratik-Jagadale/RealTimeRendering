/*
    Assigment1: Primitive Data Types
    date: 2021_20_11
*/
#include <stdio.h>

/* Entry point function */
int main()
{
    /* Code */
    printf("\n\n");

    printf("Size of ineteger :'int'                 =%zd bytes\n", sizeof(int)); /* %zd - size_t */
    printf("Size of unsigned Integer'unsigned int'  =%zd bytes\n", sizeof(unsigned int));
    printf("size of 'long'                          =%zd bytes\n", sizeof(long));
    printf("Size of 'long long'                     =%zd bytes\n\n", sizeof(long long));

    printf("Size of 'float'                         =%zd bytes\n", sizeof(float));
    printf("Size of 'double'                        =%zd bytes\n", sizeof(double));
    printf("Size of 'long double'                   =%zd bytes\n", sizeof(long double));

    printf("\n\n");

    return (0);
}
