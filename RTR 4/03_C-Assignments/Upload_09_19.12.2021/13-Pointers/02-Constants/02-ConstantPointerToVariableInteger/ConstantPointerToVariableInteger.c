#include <stdio.h>

int main(void)
{
    // Variable declartions
    int num_PRJ = 5;
    int *const ptr = &num_PRJ; // ptr is a constant pointer to integer Variable

    //code
    printf("\n\n");
    printf("Current value of 'num_PRJ' = %d\n", num_PRJ);
    printf("Current 'ptr' (address of 'num_PRJ') = %p\n", ptr);

    num_PRJ++; // does not give error as we are modifying the value of the variable individually

    printf("\n\n");
    printf("After num_PRJ++, value of num_PRJ = %d\n", num_PRJ);

    (*ptr)++; //this line will not give error beacuse value at address of 'ptr' is not constant

    // ptr++; // here does give error because ptr is a constant pointer

    printf("\n\n");

    printf("\n\n");
    printf("After (*ptr)++, value of 'ptr' = %p\n", ptr);
    printf("Value at this 'ptr' = %d \n", *ptr);
    printf("\n\n");

    return (0);
}

/*
CONCLUSION
As 'ptr' is a constant pointer to integer variable - we can change the value
stored ar address 'ptr' but we can not change 'ptr'
we can change the value of thee variable num individually - whose address is contained in 'ptr'
we also change the value at adress of ptr - we can change value of 'num' with 
respect to 'ptr' => (*ptr)++ is allowed
But we can not change the vlaue of  ptr itself => So , ptr++ is Not allowed
*/