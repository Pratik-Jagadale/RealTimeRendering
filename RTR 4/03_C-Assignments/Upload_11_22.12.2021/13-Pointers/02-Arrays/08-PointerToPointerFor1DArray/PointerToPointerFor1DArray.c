#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    void MyaAlloc(int **ptr, unsigned int numberOfElements);

    //Variable declartions
    int *piArray = NULL;
    unsigned int num_elements_PRJ;
    int i;

    // code
    printf("\n\n");
    printf("How Many Elements You Want In Integer Array ?\n\n");
    scanf("%u", &num_elements_PRJ);

    printf("\n\n");
    MyAlloc(&piArray, num_elements_PRJ);

    printf("Enter  %u Elements To Fill Up Your Integer Array: \n\n", num_elements_PRJ);
    for (i = 0; i < num_elements_PRJ; i++)
        scanf("%d", &piArray[i]);

    printf("\n\n");
    printf("The %u Elements Entered By You In The Integer Array: \n\n", num_elements_PRJ);

    for (i = 0; i < num_elements_PRJ; i++)
        printf("%u\n", piArray[i]);

    printf("\n\n");
    if (piArray)
    {
        free(piArray);
        piArray = NULL;
        printf("Memmory Allocated Has Now Been Successfully Freed !!!\n\n");
    }

    return (0);
}

void MyAlloc(int **ptr, unsigned int numberOfElements)
{
    //code
    *ptr = (int *)malloc(numberOfElements * sizeof(int));
    if (*ptr == NULL)
    {
        printf("Could Not Allocated Memory !!! Exitting Now...\n\n");
        exit(0);
    }
    printf("MyAlloc() Has Successfully Allocated Allocated %lu Bytes For Integer Array !!!\n\n", (numberOfElements * sizeof(int)));
}
