#include <stdio.h>

int main(void)
{
    //Variable declarations
    char ch_PRJ;
    char* ptr_PRJ = NULL; //Declaration Method 2 :- 'ptr_PRJ' is a variable of type 'char*'

    //code
    ch_PRJ = 'P';

    printf("\n\n");

    printf(" *** BEFORE ptr = &num *** \n\n");
    printf("value of 'ch_PRJ'                = %c\n\n", ch_PRJ);
    printf("Addres of 'ch_PRJ'               = %p\n\n", &ch_PRJ);
    printf("value At Address of 'ch_PRJ'     = %c\n\n", *(&ch_PRJ));
    printf("Value At 'ptr_PRJ'               = %p\n\n", ptr_PRJ);

    // Assigning address of variable 'num_PRJ' to pointer variable 'ptr'
    // 'ptr' now contains address of 'num_PRJ'...hence, 'ptr_PRJ' is same as &num_PRJ

    ptr_PRJ = &ch_PRJ;

    printf("\n\n");
    printf(" *** After ptr = &ch_PRJ *** \n\n");
    printf("value of 'ch_PRJ'                                 = %c\n\n", ch_PRJ);
    printf("Addres of 'ch_PRJ'                                = %p\n\n", ptr_PRJ);
    printf("value At Address of ptr_PRJ i.e. *(&'ch_PRJ')     = %c\n\n", *ptr_PRJ);

    return (0);
}
