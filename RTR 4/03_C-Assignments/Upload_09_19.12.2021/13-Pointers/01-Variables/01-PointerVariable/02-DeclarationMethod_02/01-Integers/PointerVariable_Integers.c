#include <stdio.h>

int main(void)
{
    //Variable declarations
    int num_PRJ;
    int* ptr_PRJ = NULL; //Declaration Method 2 :- 'ptr' is a variable of type 'int*'

    //code
    num_PRJ = 10;

    printf("\n\n");

    printf(" *** BEFORE ptr = &num *** \n\n");
    printf("value of 'num_PRJ'               =%d\n\n", num_PRJ);
    printf("Addres of 'num_PRJ'              =%p\n\n", &num_PRJ);
    printf("value At Address of 'num_PRJ'    =%d\n\n", *(&num_PRJ));
    printf("Value At 'ptr_PRJ' '(ptr_PRJ)'   =%p\n\n", ptr_PRJ);

    //Assigning address of variable 'num_PRJ' to pointer varuable 'ptr'
    //'ptr' now contains address of 'num_PRJ'...hence, 'ptr_PRJ' is same as &num_PRJ

    ptr_PRJ = &num_PRJ;

    printf("\n\n");
    printf(" *** After ptr = &num *** \n\n");
    printf("value of 'num_PRJ'                                 =%d\n\n", num_PRJ);
    printf("Addres of 'num_PRJ'                                =%p\n\n", ptr_PRJ);
    printf("value At Address of ptr_PRJ i.e. *(&'num_PRJ')     =%d\n\n", *ptr_PRJ);

    return (0);
}
