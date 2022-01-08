#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    //fucntion prototype
    void MathematicalOperations(int, int, int *, int *, int *, int *, int *);

    // Variable declartions
    int a_PRJ;
    int b_PRJ;
    int *answer_sum_PRJ = NULL;
    int *answer_difference_PRJ = NULL;
    int *answer_Product_PRJ = NULL;
    int *answer_quotient_PRJ = NULL;
    int *answer_remainder_PRJ = NULL;

    // code
    printf("\n\n");
    printf("Enter Value of 'A' :");
    scanf("%d", &a_PRJ);

    printf("\n\n");
    printf("Enter Value of 'B' :");
    scanf("%d", &b_PRJ);

    /* PASSING ADDRESSES TO FUNCTION ... FUNCTION WILL FILL THEM UP WITH 
    VALUES ... HENCE, THEY GO INTO THE FUNCTION AS ADDRESS PARAMETERS AND COME 
    OUT OF THE FUNCTION FILLED WITH VALID VALUES
    THUS, (&answer_sum, &answer_difference, &answer_product, &answer_quotient, 
    &answer_remainder) ARE CALLED "OUT PARAMETERS" OR "PARAMETERIZED RETURN 
    VALUES" ... RETURN VALUES OF FUNCTIONS COMING VIA PARAMETERS
    HENCE, ALTHOUGH EACH FUNCTION HAS ONLY ONE RETURN VALUE, USING THE CONCEPT 
    OF "PARAMETERIZED RETURN VALUES", OUR FUNCTION "MathematicalOperations()" 
    HAS GIVEN US 5 RETURN VALUES !!!
    */
    answer_sum_PRJ = (int *)malloc(1 * sizeof(int));
    if (answer_sum_PRJ == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_sum_PRJ'. Exitting Now...\n\n");
        exit(0);
    }

    answer_difference_PRJ = (int *)malloc(1 * sizeof(int));
    if (answer_difference_PRJ == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_difference_PRJ'. Exitting Now...\n\n");
        exit(0);
    }

    answer_Product_PRJ = (int *)malloc(1 * sizeof(int));
    if (answer_Product_PRJ == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_Product_PRJ'. Exitting Now...\n\n");
        exit(0);
    }

    answer_quotient_PRJ = (int *)malloc(1 * sizeof(int));
    if (answer_quotient_PRJ == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_quotient_PRJ'. Exitting Now...\n\n");
        exit(0);
    }

    answer_remainder_PRJ = (int *)malloc(1 * sizeof(int));
    if (answer_remainder_PRJ == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_remainder_PRJ'. Exitting Now...\n\n");
        exit(0);
    }

    printf("--------------- Result ---------------\n\n");
    MathematicalOperations(a_PRJ, b_PRJ, answer_sum_PRJ, answer_difference_PRJ, answer_Product_PRJ, answer_quotient_PRJ, answer_remainder_PRJ);

    printf("\n\n");
    printf("--------------- Result ---------------\n\n");
    printf("Sum = %d \n\n", *answer_sum_PRJ);
    printf("Difference = %d \n\n", *answer_difference_PRJ);
    printf("Product = %d \n\n", *answer_Product_PRJ);
    printf("Quotient = %d \n\n", *answer_quotient_PRJ);
    printf("Remainder = %d \n\n", *answer_remainder_PRJ);

    if (answer_sum_PRJ)
    {
        free(answer_sum_PRJ);
        answer_sum_PRJ = NULL;
        printf("Memory Allocated For 'answer_sum_PRJ' Successfully Freed !!!\n\n");
    }

    if (answer_difference_PRJ)
    {
        free(answer_difference_PRJ);
        answer_difference_PRJ = NULL;
        printf("Memory Allocated For 'answer_difference_PRJ' Successfully Freed !!!\n\n");
    }

    if (answer_Product_PRJ)
    {
        free(answer_Product_PRJ);
        answer_Product_PRJ = NULL;
        printf("Memory Allocated For 'answer_Product_PRJ' Successfully Freed !!!\n\n");
    }

    if (answer_quotient_PRJ)
    {
        free(answer_quotient_PRJ);
        answer_quotient_PRJ = NULL;
        printf("Memory Allocated For 'answer_quotient_PRJ' Successfully Freed !!!\n\n");
    }

    if (answer_remainder_PRJ)
    {
        free(answer_remainder_PRJ);
        answer_remainder_PRJ = NULL;
        printf("Memory Allocated For 'answer_remainder_PRJ' Successfully Freed !!!\n\n");
    }

    return (0);
}

void MathematicalOperations(int x, int y, int *answer_sum_PRJ, int *answer_difference_PRJ, int *answer_Product_PRJ, int *answer_quotient_PRJ, int *answer_remainder_PRJ)
{
    //code
    *answer_sum_PRJ = x + y;
    *answer_difference_PRJ = x - y;
    *answer_Product_PRJ = x * y;
    *answer_quotient_PRJ = x / y;
    *answer_remainder_PRJ = x % y;
}
