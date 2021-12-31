#include <stdio.h>

int main(void)
{
    // Variable declartions
    int num_PRJ = 5;
    const int *const ptr = &num_PRJ; // ptr is a constant pointer to integer constant

    //code
    printf("\n\n");
    printf("Current value of 'num_PRJ' = %d\n", num_PRJ);
    printf("Current 'ptr' (address of 'num_PRJ') = %p\n", ptr);

    num_PRJ++; // does not give error as we are modifying the value of the variable individually

    printf("\n\n");
    printf("After num_PRJ++, value of num_PRJ = %d\n", num_PRJ);

    // (*ptr)++; //this line not give error beacause ptr is pointer to constatnt integer

    // ptr++; // here does give error because ptr is a constant pointer
    printf("\n\n");

    return (0);
}

/*
CONCLUSION
As 'ptr' is a constant pointer to integer cosntant - we can not change the value
stored at address 'ptr' also we can not change 'ptr'
we can change the value of the variable num individually - whose address is contained in 'ptr'
we also change not the value at adress of ptr & we can not change value of 'num' with 
respect to 'ptr' i.e => (*ptr)++ is Not allowed
But we can not change the vlaue of  ptr itself => So , ptr++ is Not allowed
*/