#include <stdio.h>

int main(void)
{
    // Variable declaration
    int iArray_PRJ[] = {12, 24, 36, 48, 60, 72, 84, 108, 120, 120};
    float fArray_PRJ[] = {9.8f, 8.7f, 7.6f, 6.5f, 5.4f};
    double dArray_PRJ[] = {1.2222222, 2.3333333, 3.4444444};
    char cArray_PRJ[] = {'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P', '\0'};

    //code
    printf("\n\n");
    printf("Integer Array Elements And the Address They occpy Are as Folows\n\n");

    printf("iArray[0] = %d \t At Address : %p\n", *(iArray_PRJ + 0), (iArray_PRJ + 0));
    printf("iArray[1] = %d \t At Address : %p\n", *(iArray_PRJ + 1), (iArray_PRJ + 1));
    printf("iArray[2] = %d \t At Address : %p\n", *(iArray_PRJ + 2), (iArray_PRJ + 2));
    printf("iArray[3] = %d \t At Address : %p\n", *(iArray_PRJ + 3), (iArray_PRJ + 3));
    printf("iArray[4] = %d \t At Address : %p\n", *(iArray_PRJ + 4), (iArray_PRJ + 4));
    printf("iArray[5] = %d \t At Address : %p\n", *(iArray_PRJ + 5), (iArray_PRJ + 5));
    printf("iArray[6] = %d \t At Address : %p\n", *(iArray_PRJ + 6), (iArray_PRJ + 6));
    printf("iArray[7] = %d \t At Address : %p\n", *(iArray_PRJ + 7), (iArray_PRJ + 7));
    printf("iArray[8] = %d \t At Address : %p\n", *(iArray_PRJ + 8), (iArray_PRJ + 8));
    printf("iArray[9]] = %d \t At Address : %p\n", *(iArray_PRJ + 9), (iArray_PRJ + 9));

    printf("\n\n");
    printf("Float Array Elements And the Address They occpy Are as Folows\n\n");

    printf("fArray_PRJ[0] = %f \t At Address : %p\n", *(fArray_PRJ + 0), (fArray_PRJ + 0));
    printf("fArray_PRJ[1] = %f \t At Address : %p\n", *(fArray_PRJ + 1), (fArray_PRJ + 1));
    printf("fArray_PRJ[2] = %f \t At Address : %p\n", *(fArray_PRJ + 2), (fArray_PRJ + 2));
    printf("fArray_PRJ[3] = %f \t At Address : %p\n", *(fArray_PRJ + 3), (fArray_PRJ + 3));
    printf("fArray_PRJ[4] = %f \t At Address : %p\n", *(fArray_PRJ + 4), (fArray_PRJ + 4));

    printf("\n\n");
    printf("double Array Elements And the Address They occupy Are as Folows\n\n");

    printf("dArray_PRJ[0] = %lf \t At Address : %p\n", *(dArray_PRJ + 0), (dArray_PRJ + 0));
    printf("dArray_PRJ[1] = %lf \t At Address : %p\n", *(dArray_PRJ + 1), (dArray_PRJ + 1));
    printf("dArray_PRJ[2] = %lf \t At Address : %p\n", *(dArray_PRJ + 2), (dArray_PRJ + 2));

    printf("\n\n");
    printf("char Array Elements And the Address They occpy Are as Folows\n\n");

    printf("cArray_PRJ[0] = %c \t At Address : %p\n", *(cArray_PRJ + 0), (cArray_PRJ + 0));
    printf("cArray_PRJ[1] = %c \t At Address : %p\n", *(cArray_PRJ + 1), (cArray_PRJ + 1));
    printf("cArray_PRJ[2] = %c \t At Address : %p\n", *(cArray_PRJ + 2), (cArray_PRJ + 2));
    printf("cArray_PRJ[3] = %c \t At Address : %p\n", *(cArray_PRJ + 3), (cArray_PRJ + 3));
    printf("cArray_PRJ[4] = %c \t At Address : %p\n", *(cArray_PRJ + 4), (cArray_PRJ + 4));
    printf("cArray_PRJ[5] = %c \t At Address : %p\n", *(cArray_PRJ + 5), (cArray_PRJ + 5));
    printf("cArray_PRJ[6] = %c \t At Address : %p\n", *(cArray_PRJ + 6), (cArray_PRJ + 6));
    printf("cArray_PRJ[7] = %c \t At Address : %p\n", *(cArray_PRJ + 7), (cArray_PRJ + 7));
    printf("cArray_PRJ[8] = %c \t At Address : %p\n", *(cArray_PRJ + 8), (cArray_PRJ + 8));
    printf("cArray_PRJ[9] = %c \t At Address : %p\n", *(cArray_PRJ + 9), (cArray_PRJ + 9));
    printf("cArray_PRJ[10] = %c \t At Address : %p\n", *(cArray_PRJ + 10), (cArray_PRJ + 10));
    printf("cArray_PRJ[11] = %c \t At Address : %p\n", *(cArray_PRJ + 11), (cArray_PRJ + 11));
    printf("cArray_PRJ[12] = %c \t At Address : %p\n", *(cArray_PRJ + 12), (cArray_PRJ + 12));
    printf("cArray_PRJ[13] = %c \t At Address : %p\n", *(cArray_PRJ + 13), (cArray_PRJ + 13));

    return (0);
}
