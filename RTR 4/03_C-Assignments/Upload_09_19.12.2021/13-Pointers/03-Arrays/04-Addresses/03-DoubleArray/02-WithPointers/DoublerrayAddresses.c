#include <stdio.h>

int main(void)
{
    // Variable declartions
    double dArray_PRJ[10];
    double *ptr_dArray_PRJ = NULL;
    int i;

    //code

    for (i = 0; i < 10; i++)
        dArray_PRJ[i] = (i + 1) * 3.55555555;

    // *** NAME OF ANY ARRAY IS ITS BASE ADDRESS ***
    // *** HENCE, 'dArray' IS THE BASE ADDRESS OF ARRAY dArray[] OR 'dArray' IS
    /// THE ADDRESS OF ELEMENT dArray[0] ***
    // *** ASSIGNING BASE ADDRESS OF ARRAY 'dArray[]' TO double POINTER
    // 'ptr_dArray'

    ptr_dArray_PRJ = dArray_PRJ; // ptr_dArray = &dArray[0]

    printf("\n\n");
    printf("Elements of The Double Array : \n\n");
    for (i = 0; i < 10; i++)
        printf("dArray[%d] = %lf\n", i, *(ptr_dArray_PRJ + i));

    printf("\n\n");
    printf("Elements of The Double Array :\n\n");
    for (i = 0; i < 10; i++)
        printf("dArray[%d] = %f \t\t Address = %p\n", i, *(ptr_dArray_PRJ + i), (ptr_dArray_PRJ + i));

    printf("\n\n");

    return (0);
}
