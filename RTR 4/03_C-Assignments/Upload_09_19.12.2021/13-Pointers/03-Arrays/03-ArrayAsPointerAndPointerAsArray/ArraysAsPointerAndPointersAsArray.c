#include <stdio.h>

int main(void)
{
    // Variable declaration
    int iArray_PRJ[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int *ptr_iArray_PRJ = NULL;

    // *** Using Array Name as A Pointer  i.e Value Of xth Element of iArray :
    // *(iArray + x) And Address of xth Elemet of iArray_PRJ + x  ***
    printf("\n\n");
    printf("Using Array Name as A pointer  i.e Value Of xth Element of iArray *(iArray + x) And Address of xth Element of (iArray_PRJ + x)\n\n");

    printf("*(iArray_PRJ + 0) = %d \t At Address : %p\n", *(iArray_PRJ + 0), (iArray_PRJ + 0));
    printf("*(iArray_PRJ + 1) = %d \t At Address : %p\n", *(iArray_PRJ + 1), (iArray_PRJ + 1));
    printf("*(iArray_PRJ + 2) = %d \t At Address : %p\n", *(iArray_PRJ + 2), (iArray_PRJ + 2));
    printf("*(iArray_PRJ + 3) = %d \t At Address : %p\n", *(iArray_PRJ + 3), (iArray_PRJ + 3));
    printf("*(iArray_PRJ + 4) = %d \t At Address : %p\n", *(iArray_PRJ + 4), (iArray_PRJ + 4));
    printf("*(iArray_PRJ + 5) = %d \t At Address : %p\n", *(iArray_PRJ + 5), (iArray_PRJ + 5));
    printf("*(iArray_PRJ + 6) = %d \t At Address : %p\n", *(iArray_PRJ + 6), (iArray_PRJ + 6));
    printf("*(iArray_PRJ + 7) = %d \t At Address : %p\n", *(iArray_PRJ + 7), (iArray_PRJ + 7));
    printf("*(iArray_PRJ + 8) = %d \t At Address : %p\n", *(iArray_PRJ + 8), (iArray_PRJ + 8));
    printf("*(iArray_PRJ + 9) = %d \t At Address : %p\n", *(iArray_PRJ + 9), (iArray_PRJ + 9));

    // Assigning Base Address of Integer Array 'iArray' To integer Pointer
    // 'ptr_iArray'
    // Name of any Array is Its Own Base Address
    ptr_iArray_PRJ = iArray_PRJ; // Same As... ptr_iArray = &iarray[0]

    // *** Using Pointer as Array Name i.e : Value of xTh Element of iArrau :
    // (ptr_iArray[x]) And Address of xTh Element of iArray : (&ptr_iArray[x]) ***

    printf("\n\n");
    printf("Using Pointer as Array Name i.e : Value of xTh Element of iArray_PRJ :ptr_iArray[x] And Address of xTh Element of iArray : &ptr_iArray[x] \n\n");

    printf("ptr_iArray_PRJ[0] = %d \t At Address : %p\n", ptr_iArray_PRJ[0], &ptr_iArray_PRJ[0]);
    printf("ptr_iArray_PRJ[1] = %d \t At Address : %p\n", ptr_iArray_PRJ[1], &ptr_iArray_PRJ[1]);
    printf("ptr_iArray_PRJ[2] = %d \t At Address : %p\n", ptr_iArray_PRJ[2], &ptr_iArray_PRJ[2]);
    printf("ptr_iArray_PRJ[3] = %d \t At Address : %p\n", ptr_iArray_PRJ[3], &ptr_iArray_PRJ[3]);
    printf("ptr_iArray_PRJ[4] = %d \t At Address : %p\n", ptr_iArray_PRJ[4], &ptr_iArray_PRJ[4]);
    printf("ptr_iArray_PRJ[5] = %d \t At Address : %p\n", ptr_iArray_PRJ[5], &ptr_iArray_PRJ[5]);
    printf("ptr_iArray_PRJ[6] = %d \t At Address : %p\n", ptr_iArray_PRJ[6], &ptr_iArray_PRJ[6]);
    printf("ptr_iArray_PRJ[7] = %d \t At Address : %p\n", ptr_iArray_PRJ[7], &ptr_iArray_PRJ[7]);
    printf("ptr_iArray_PRJ[8] = %d \t At Address : %p\n", ptr_iArray_PRJ[8], &ptr_iArray_PRJ[8]);
    printf("ptr_iArray_PRJ[9] = %d \t At Address : %p\n", ptr_iArray_PRJ[9], &ptr_iArray_PRJ[9]);

    return (0);
}
