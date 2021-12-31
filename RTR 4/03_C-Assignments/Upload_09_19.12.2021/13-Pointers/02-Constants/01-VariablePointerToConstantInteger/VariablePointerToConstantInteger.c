#include <stdio.h>

int main(void)
{
    // Variable declartions
    int num_PRJ = 5;
    const int *ptr = NULL; // ptr is a pointer to integer constant

    //code
    ptr = &num_PRJ;
    printf("\n\n");
    printf("Current value of 'num_PRJ' = %d\n", num_PRJ);
    printf("Current 'ptr' (address of 'num_PRJ') = %p\n", ptr);

    num_PRJ++; // does not give error as we are modifying the value of the variable individually

    printf("\n\n");
    printf("After num_PRJ++, value of num_PRJ = %d\n", num_PRJ);

    //   (*ptr)++; //this line will give error beacuse ptr is pointer to constant integer variable

    ptr++; // here doest not give error because ptr is not constant pointer

    printf("\n\n");

    printf("\n\n");
    printf("After ptr++, value of 'ptr' = %p\n", ptr);
    printf("Value at this new 'ptr' = %d \n", *ptr); // garbage Value
    printf("\n\n");

    return (0);
}

/*
CONCLUSION
As 'ptr' is a variable pointer to  constant integer - we can change the value
of pointer 'ptr'
we can change the value of thee variable num individually - whose address is contained in 'ptr'
so in short we cannot change the value at address of ptr - wecannot change the value 
of num whith respect to ptr => (*ptr)++ is not allowed
But we can change the address ptr itself => So , ptr++ is allowed
*/