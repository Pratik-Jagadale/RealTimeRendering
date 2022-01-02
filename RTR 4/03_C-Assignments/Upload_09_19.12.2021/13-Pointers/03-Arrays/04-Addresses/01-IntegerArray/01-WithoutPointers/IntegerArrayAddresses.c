#include <stdio.h>

int main(void)
{
    //Variable declartion
    int iArray[10];
    int i;

    //code
    for (i = 0; i < 10; i++)
        iArray[i] = (i + 1) * 3;

    printf("\n\n");
    printf("Elements of The Integer array : \n\n");
    for (i = 0; i < 10; i++)
        printf("iArray[%d] = %d \t\t Address = %p\n", i, iArray[i], &iArray[i]);

    printf("\n\n");
    return (0);
}
