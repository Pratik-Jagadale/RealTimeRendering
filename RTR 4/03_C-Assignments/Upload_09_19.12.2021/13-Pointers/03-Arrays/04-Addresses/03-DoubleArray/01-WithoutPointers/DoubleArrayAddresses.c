#include <stdio.h>

int main(void)
{
    //Variable declartion
    double dArray_PRJ[10];
    int i;

    //code
    for (i = 0; i < 10; i++)
        dArray_PRJ[i] = (i + 1) * 1.55555;

    printf("\n\n");
    printf("Elements of The Double array : \n\n");
    for (i = 0; i < 10; i++)
        printf("dArray_PRJ[%d] = %lf \t\t Address = %p\n", i, dArray_PRJ[i], &dArray_PRJ[i]);

    printf("\n\n");
    return (0);
}
