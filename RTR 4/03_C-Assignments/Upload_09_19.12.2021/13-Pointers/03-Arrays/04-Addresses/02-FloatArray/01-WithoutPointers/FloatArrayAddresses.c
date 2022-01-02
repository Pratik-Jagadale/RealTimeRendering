#include <stdio.h>

int main(void)
{
    //Variable declartion
    float fArray[10];
    int i;

    //code
    for (i = 0; i < 10; i++)
        fArray[i] = (float)(i + 1) * 3.5f;

    printf("\n\n");
    printf("Elements of The Float array : \n\n");
    for (i = 0; i < 10; i++)
        printf("fArray[%d] = %f \t\t Address = %p\n", i, fArray[i], &fArray[i]);

    printf("\n\n");
    return (0);
}
