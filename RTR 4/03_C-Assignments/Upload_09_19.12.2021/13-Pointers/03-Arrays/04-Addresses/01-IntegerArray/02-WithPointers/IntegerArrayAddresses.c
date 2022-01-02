#include <stdio.h>

int main(void)
{
    // Variable declartions
    int iArray_PRJ[10];
    int *ptr_iArray_PRJ = NULL;
    int i;

    //code
    for (i = 0; i < 10; i++)
        iArray_PRJ[i] = (i + 1) * 3;

    // *** NAME OF ANY ARRAY IS ITS BASE ADDRESS ***
    // *** HENCE, 'iArray' IS THE BASE ADDRESS OF ARRAY iArray[] OR 'iArray' IS
    /// THE ADDRESS OF ELEMENT iArray[0] ***
    // *** ASSIGNING BASE ADDRESS OF ARRAY 'iArray[]' TO INTEGER POINTER
    // 'ptr_iArray'

    ptr_iArray_PRJ = iArray_PRJ; // ptr_iArray = &iArray[0]

    printf("\n\n");
    printf("Elements of The Integer Array : \n\n");
    for (i = 0; i < 10; i++)
        printf("iArray[%d] = %d\n", i, *(ptr_iArray_PRJ + i));

    printf("\n\n");
    printf("Elements of The Integer Array :\n\n");
    for (i = 0; i < 10; i++)
        printf("iArray[%d] = %d \t\t Address = %p\n", i, *(ptr_iArray_PRJ + i), (ptr_iArray_PRJ + i));

    printf("\n\n");

    return (0);
}