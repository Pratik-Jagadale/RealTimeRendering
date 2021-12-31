#include <stdio.h>

int main(void)
{
    //Variable declaration
    int num_PRJ;
    int *ptr_PRJ = NULL;
    int *copy_ptr_PRJ = NULL;

    //code
    num_PRJ = 25000;
    ptr_PRJ = &num_PRJ;

    printf("\n\n");
    printf(" *** BEFORE copy_ptr = ptr *** \n\n");
    printf(" num_PRJ                    = %d\n", num_PRJ);
    printf(" &num_PRJ                   = %p\n", &num_PRJ);
    printf(" *(&num_PRJ)                = %d\n", *(&num_PRJ));
    printf(" ptr_PRJ                    = %p\n", ptr_PRJ);
    printf(" *ptr_PRJ                   = %d\n", *ptr_PRJ);

    // 'ptr_PRJ' is an integer pointer variable ... that it can hold the addres of
    // an integer  variable 'num_PRJ'
    // 'copy_ptr_PRJ' ia another integer pointer to variable
    // If ptr_PRJ  = &num_PRJ ... 'ptr_PRJ' will contain address of integer variable 'num_PRJ'
    // If 'ptr_PRj' is assigned to 'copy_ptr_PRJ', 'copy_pyr_PRJ' will also contain of
    // Integer variable 'mum_PRJ'
    //Hence , now , both 'ptr_PRJ' and 'copy_ptr_PRJ' will point to 'num_PRJ' ...

    copy_ptr_PRJ = ptr_PRJ; // copy_ptr_PRJ = ptr_PRJ = &num_PRJ

    printf("\n\n");
    printf(" *** After copy copy_ptr = ptr *** \n\n");
    printf(" num_PRJ                    = %d\n", num_PRJ);
    printf(" &num_PRJ                   = %p\n", &num_PRJ);
    printf(" *(&num_PRJ)                = %d\n", *(&num_PRJ));
    printf(" ptr_PRJ                    = %p\n", ptr_PRJ);
    printf(" *ptr_PRJ                   = %d\n", *ptr_PRJ);
    printf(" copy_ptr_PRJ               = %p\n", copy_ptr_PRJ);
    printf(" *copy_ptr_PRJ              = %d\n", *copy_ptr_PRJ);

    return (0);
}
