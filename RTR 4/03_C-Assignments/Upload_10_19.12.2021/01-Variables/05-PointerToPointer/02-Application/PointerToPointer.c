#include <stdio.h>

int main(void)
{
    // Variable declartions
    int num_PRJ;
    int *ptr = NULL;
    int **pptr = NULL;

    //code
    num_PRJ = 10;

    printf("\n\n");

    printf("--------------BEFORE ptr = &num_PRJ ---------------- \n\n");
    printf("Value of 'num_PRJ' \t\t             =%d\n\n", num_PRJ);
    printf("Address of 'num_PRJ' \t\t           = %p\n\n ", &num_PRJ);
    printf("Value At Address of 'num_PRJ'\t\t   = %d\n\n", *(&num_PRJ));

    ptr = &num_PRJ;

    printf("\n\n");
    printf("--------------After ptr = &num_PRJ ---------------- \n\n");
    printf("Value of 'num_PRJ' \t\t             =%d\n\n", num_PRJ);
    printf("Address of 'num_PRJ' \t\t           = %p\n\n ", ptr);
    printf("Value At Address of 'num_PRJ'\t\t   = %d\n\n", *ptr);

    printf("\n\n");

    pptr = &ptr;

    printf("-------------------- AFTER pptr = &ptr ------------------\n\n");
    printf("Value of 'num_PRJ'                          =%d\n\n", num_PRJ);
    printf("Address of 'num_PRJ' (ptr_PRJ)              = %p\n\n ", ptr);
    printf("Address of 'ptr' (pptr)                     = %p\n\n", pptr);
    printf("Value at Address of 'ptr' (*pptr)           = %p\n\n", *pptr);
    printf("value ata Address of 'num (*ptr) (**pptr)   = %d\n\n", **pptr);

    return (0);
}
