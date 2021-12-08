#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int i_PRJ, j_PRJ;

    /* code */
    printf("\n\n");

    printf("Enter An Integer  value from which Iteration Must Begin : ");
    scanf("%d", &i_PRJ);

    printf("How many Digits Do You want to Print from %d Onwards ? :", i_PRJ);
    scanf("%d", &j_PRJ);

    printf("Printing Digits %d to %d : \n\n", i_PRJ, j_PRJ);

    for (i_PRJ; i_PRJ <= j_PRJ; i_PRJ++)
        printf("\t%d\n", i_PRJ);

    printf("\n");

    return (0);
}