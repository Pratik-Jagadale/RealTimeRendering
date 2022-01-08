#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // fucntion prototype
    void MultiplyArrayElementsByNumber(int *, int, int);

    // Varibale delclarations
    int *iArray_PRJ = NULL;
    int num_elements_PRJ;
    int i, num;

    // code
    printf("\n\n");
    printf("Enter How Many Elements You Want In The Integer Array :");
    scanf("%d", &num_elements_PRJ);

    iArray_PRJ = (int *)malloc(num_elements_PRJ * sizeof(int));
    if (iArray_PRJ == NULL)
    {
        printf("MEMMORY ALLOACTION TO 'iArray_PRJ' HAS FAILED !!! EXITTING NOW...\n\n");
        exit(0);
    }

    printf("\n\n");
    printf("Enter %d Elements For The Integer Array :\n\n", num_elements_PRJ);
    for (i = 0; i < num_elements_PRJ; i++)
        scanf("%d", &iArray_PRJ[i]);

    //--- ONE ---
    printf("\n\n");
    printf("Array Before passing To Function MuliplyArrayElementsByNumber() :\n\n");
    for (i = 0; i < num_elements_PRJ; i++)
        printf("iArray_PRJ[%d] = %d\n", i, iArray_PRJ[i]);

    printf("\n\n");
    printf("Enter The Number By Which You Want To Multiply Each Array Element :");
    scanf("%d", &num);

    MultiplyArrayElementsByNumber(iArray_PRJ, num_elements_PRJ, num);

    printf("\n\n");
    printf("Array Returned by Function MultiplicationArrayyElementsByNumber() : \n\n");
    for (i = 0; i < num_elements_PRJ; i++)
        printf("iArray[%d] = %d\n", i, iArray_PRJ[i]);

    if (iArray_PRJ)
    {
        free(iArray_PRJ);
        iArray_PRJ = NULL;
        printf("\n\n");
        printf("MEMORY ALLOCATED TO 'iArray_PRJ' HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }
    return (0);
}

void MultiplyArrayElementsByNumber(int *arr, int iNumElements, int n)
{
    // variable declartions
    int i;

    //code
    for (i = 0; i < iNumElements; i++)
        arr[i] = arr[i] * n;
}
