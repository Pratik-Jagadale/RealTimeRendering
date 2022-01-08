#include <stdio.h>
#include <stdlib.h> // contains prototype of malloc() and free

int main(void)
{
    //Variable declartions
    int *ptr_iArray_PRJ = NULL; /*IT IS GOOD DISCIPLINE TO INITIALIZE ANY POINTER 
                            WITH NULL ADDRESS TO PREVENT ANY GARBAGE VALUE GETTING INTO IT, THAT WAY,
                            IT MAKES IT EASY TO CHECK FOR SUCCESS OR FAILURE OF MEMORY ALLOCATION 
                            LATER ON AFTER malloc()...*/

    unsigned int iArrayLength_PRJ = 0;
    int i;

    // code
    printf("\n\n");
    printf("Enter The Number Of Elements You want In your Integer Array :");
    scanf("%d", &iArrayLength_PRJ);

    /*
        // ****** ALLOCATING AS MUCH MEMORY REQUIRED TO THE INTEGER ARRAY ******
        // ****** MEMORY REQUIRED FOR INTEGER ARRAY = SIZE IN BYTES OF ONE INTEGER 
        * NUMBER OF INTEGERS TO BE STORED IN ARRAY ******
        // ****** TO ALLOCATE SAID AMOUNT OF MEMORY, FUNCTION malloc() WILL BE USED
        // ****** malloc() WILL ALLOCATE SAID AMOUNT OF MEMORY AND WILL RETURN THE 
        INITIAL / STARTING / BASE ADDRESS OF THE ALLOCATED MEMORY, WHICH MUST BE 
        CAPTURED IN A POINTER VARIABLE ******
        // ****** USING THIS BASE ADDRESS, THE INTEGER ARRAY CAN BE ACCESSED AND 
        USED ******
    */

    ptr_iArray_PRJ = (int *)malloc(iArrayLength_PRJ * sizeof(int));

    if (ptr_iArray_PRJ == NULL)
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY HAS FAILED !!! EXITTING NOW....\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTEGR ARRAY HAS SUCCEEDED !!! \n\n");
        printf("MEMORY ADDRESSES FROM %p To %p HAVE BEEN ALLOCATED TO INTEGER ARRAY !!!\n\n", ptr_iArray_PRJ, (ptr_iArray_PRJ + (iArrayLength_PRJ - 1)));
    }
    /*
        if ptr_iArray_PRJ IS NULL , EVEN After call to malloc(), IT means malloc
        has failed to allocate memmory and No Adress has been returned by malloc()
        int ptr_iArray_PRJ...
    */

    printf("\n\n");
    printf("Enter %d Elements for The Integer Array : \n\n", iArrayLength_PRJ);
    for (i = 0; i < iArrayLength_PRJ; i++)
        scanf("%d", (ptr_iArray_PRJ + i));

    printf("\n\n");
    printf("The Integer Array Entered  by You, Consisting Of %d Elements :\n\n", iArrayLength_PRJ);

    for (i = 0; i < iArrayLength_PRJ; i++)
    {
        printf("ptr_iArray[%d] = %d \t\t At Address &ptr_iArray[%d]\n", i, ptr_iArray_PRJ[i], i, &ptr_iArray_PRJ[i]);
    }

    printf("\n\n");
    for (i = 0; i < iArrayLength_PRJ; i++)
    {
        printf("*(ptr_iArray_PRJ +  %d) = %d \t\t At address (ptr_iArray + %d) : %p\n", i, *(ptr_iArray_PRJ + i), i, (ptr_iArray_PRJ + i));
    }

    if (ptr_iArray_PRJ)
    {
        free(ptr_iArray_PRJ);
        ptr_iArray_PRJ = NULL;

        printf("\n\n");
        printf("MEMORY ALLOCATED FOR INTEGER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    return (0);
}
