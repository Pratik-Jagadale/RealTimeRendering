#include <stdio.h>

int main(void)
{
    // Variable declartions
    float fArray_PRJ[10];
    float *ptr_fArray_PRJ = NULL;
    int i;

    //code

    for (i = 0; i < 10; i++)
        fArray_PRJ[i] = (float)(i + 1) * 1.5f;

    // *** NAME OF ANY ARRAY IS ITS BASE ADDRESS ***
    // *** HENCE, 'ifArray' IS THE BASE ADDRESS OF ARRAY fArray[] OR 'fArray' IS
    /// THE ADDRESS OF ELEMENT fArray[0] ***
    // *** ASSIGNING BASE ADDRESS OF ARRAY 'fArray[]' TO Float POINTER
    // 'ptr_fArray'

    ptr_fArray_PRJ = fArray_PRJ; // ptr_fArray = &fArray[0]

    printf("\n\n");
    printf("Elements of The Float Array : \n\n");
    for (i = 0; i < 10; i++)
        printf("fArray[%d] = %f\n", i, *(ptr_fArray_PRJ + i));

    printf("\n\n");
    printf("Elements of The Float Array :\n\n");
    for (i = 0; i < 10; i++)
        printf("fArray[%d] = %f \t\t Address = %p\n", i, *(ptr_fArray_PRJ + i), (ptr_fArray_PRJ + i));

    printf("\n\n");

    return (0);
}
