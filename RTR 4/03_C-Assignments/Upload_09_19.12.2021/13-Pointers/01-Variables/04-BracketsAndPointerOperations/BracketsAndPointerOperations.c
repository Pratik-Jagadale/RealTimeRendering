#include <stdio.h>

int main(void)
{
    //Variable declaration
    int num_PRJ;
    int *ptr_PRJ = NULL;

    //code
    num_PRJ = 25000;
    ptr_PRJ = &num_PRJ;

    printf("\n\n");
    printf(" num_PRJ                    = %d\n", num_PRJ);
    printf(" &num_PRJ                   = %p\n", &num_PRJ);
    printf(" *(&num_PRJ)                = %d\n", *(&num_PRJ));
    printf(" ptr_PRJ                    = %p\n", ptr_PRJ);
    printf(" *ptr_PRJ                   = %d\n", *ptr_PRJ);

    printf("\n\n");

    // Add 10 to  'ptr' which is the address of 'num' ...
    // Hence, 10 will be added to the address of 'num' and
    // the resultant address will be displayed

    printf("Answer Of (ptr + 10) = %p\n", (ptr_PRJ + 10));

    // Add 10 to  'ptr' which is the address of 'num' ... and give value at address...
    // Hence, 10 will be added to the address of 'num' and
    // the resultant address will be displayed

    printf("Answer Of *(ptr + 10) = %d\n", *(ptr_PRJ + 10));

    // Add 10 to '*ptr' which is the value at address of 'num' (i.e 'num' i.e :
    // 5) and give new value . without any change in any adress...
    // Hence, 10 will be aadded to the '*ptr' (num = 5) and the resultant value
    // will be given (*ptr + 10) = (num + 10) = ( 5 + 10) = 15 ...

    printf("Answer Of (*ptr_PRJ + 10) = %d\n\n", (*ptr_PRJ + 10));

    // *** Associatitivity Of (Value At Address) And ++ And -- Opertors If From
    // Right To Left ***

    // (Rigth To Left) Consider value *ptr_PRJ ... Pre-incrrement *ptr ... that is,
    // value at address 'ptr' i.e : *ptr is pre-increment (++*ptr)
    ++*ptr_PRJ;
    // *ptr is preincremented ... *ptr is 5 ... after execuation of this statement ..*ptr = 6
    printf("Answer Of ++*ptr : %d\n", *ptr_PRJ); // Brcakets are not necessory pre-increment /post-drcrement

    *ptr_PRJ++; // incorrect method of post-increment a value using pointer
                // here will first ptr incremented then value at address ptr_PRJ will display
    printf("Aswer of *ptr++ : %d\n", *ptr_PRJ);

    ptr_PRJ = &num_PRJ;

    (*ptr_PRJ)++; // Correct method of post-increment method for value at address ptr_PRJ
    printf("Asnwer of (*ptr)++ : %d\n", *ptr_PRJ);

    return (0);
}
