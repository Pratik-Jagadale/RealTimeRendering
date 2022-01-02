#include <stdio.h>

enum
{
    NEGAVTIVE = -1,
    ZERO,
    POSITIVE
};

int main(void)
{
    //fucntion prototype
    int Difference(int, int, int *);

    // Variable declartions
    int a_PRJ;
    int b_PRJ;
    int answer_PRJ;
    int ret_PRJ;

    // code
    printf("\n\n");
    printf("Enter Value of 'A' :");
    scanf("%d", &a_PRJ);

    printf("\n\n");
    printf("Enter Value of 'B' :");
    scanf("%d", &b_PRJ);

    ret_PRJ = Difference(a_PRJ, b_PRJ, &answer_PRJ);

    printf("\n\n");
    printf("---------------Result-----------------\n\n");
    printf("Diffrence Of %d And %d = %d \n\n", a_PRJ, b_PRJ, answer_PRJ);

    if (ret_PRJ == POSITIVE)
        printf("Difference of %d And %d Is Positive !!!\n\n", a_PRJ, b_PRJ);
    else if (ret_PRJ == NEGAVTIVE)
        printf("Difference of %d And %d Is Negavtive !!!\n\n", a_PRJ, b_PRJ);
    else
        printf("Difference of %d And %d Is Zero !!!\n\n", a_PRJ, b_PRJ);

    return (0);
}

int Difference(int x, int y, int *answer_PRJ)
{
    //code
    *answer_PRJ = x - y;

    if (*answer_PRJ > 0)
        return (POSITIVE);
    else if (*answer_PRJ < 0)
        return (NEGAVTIVE);
    else
        return (ZERO);
}
