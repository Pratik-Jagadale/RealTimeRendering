#include <stdio.h>

// DEFINING STRUCT
struct MyData
{
    int *ptr_i;
    int i;

    float *ptr_f;
    float f;

    double *ptr_d;
    double d;
};

int main(void)
{
    //variable declartion
    struct MyData *pdata = NULL;

    pdata = (struct MyData *)malloc(sizeof(struct MyData));

    if (pdata == NULL)
    {
        printf("FAILED TO ALLOCATE MEMORY TO 'struct MyData' !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
        printf("SUCCESSFULLY ALLOCATED MEMORY TO 'struct MyData' !!!\n\n");

    // code
    (*pdata).i = 9;
    (*pdata).ptr_i = &(*pdata).i;

    (*pdata).f = 11.45f;
    (*pdata).ptr_f = &(*pdata).f;

    (*pdata).d = 30.2151;
    (*pdata).ptr_d = &(*pdata).d;

    printf("\n\n");
    printf("i = %d\n", *((*pdata).ptr_i));
    printf("Address of 'i' = %p \n", (*pdata).ptr_i);

    printf("\n\n");
    printf("f = %f\n", *((*pdata).ptr_f));
    printf("Address of 'f' = %p \n", (*pdata).ptr_f);

    printf("\n\n");
    printf("d = %lf\n", *((*pdata)).ptr_d);
    printf("Address of 'd' = %p \n", (*pdata).ptr_d);

    if (pdata)
    {
        free(pdata);
        pdata = NULL;
        printf("MEMORY ALOCATED TO 'struct MyData' HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    return (0);
}
