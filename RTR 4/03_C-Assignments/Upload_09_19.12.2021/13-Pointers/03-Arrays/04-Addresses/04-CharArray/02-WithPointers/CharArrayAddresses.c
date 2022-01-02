#include <stdio.h>

int main(void)
{
    // Variable declartions
    char cArray_PRJ[10];
    char *ptr_cArray_PRJ = NULL;
    int i;

    //code

    for (i = 0; i < 10; i++)
        cArray_PRJ[i] = (char)(i + 65);

    // *** NAME OF ANY ARRAY IS ITS BASE ADDRESS ***
    // *** HENCE, 'cArray' IS THE BASE ADDRESS OF ARRAY cArray[] OR 'cArray' IS
    /// THE ADDRESS OF ELEMENT cArray[0] ***
    // *** ASSIGNING BASE ADDRESS OF ARRAY 'cArray[]' TO double POINTER
    // 'ptr_cArray'

    ptr_cArray_PRJ = cArray_PRJ; // ptr_cArray = &cArray[0]

    printf("\n\n");
    printf("Elements of The Character Array : \n\n");
    for (i = 0; i < 10; i++)
        printf("cArray[%d] = %c\n", i, *(ptr_cArray_PRJ + i));

    printf("\n\n");
    printf("Elements of The Character Array :\n\n");
    for (i = 0; i < 10; i++)
        printf("cArray[%d] = %c \t\t Address = %p\n", i, *(ptr_cArray_PRJ + i), (ptr_cArray_PRJ + i));

    printf("\n\n");

    return (0);
}
