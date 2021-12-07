#include <stdio.h>

int main(void)
{
    /* variable declartions */
    int num_PRJ;

    /* code */
    printf("\n\n");

    printf("Enter value for num :");
    scanf("%d", &num_PRJ);

    if (num_PRJ < 0)
        printf("Num = %d Id Less Than 0 (Negavtive).\n\n", num_PRJ);

    if ((num_PRJ > 0) && (num_PRJ <= 100))
        printf("Num = %d Is Between 0 And 100.\n\n", num_PRJ);

    if ((num_PRJ > 100) && (num_PRJ <= 200))
        printf("Num = %d Is Between 100 And 200.\n\n", num_PRJ);

    if ((num_PRJ > 200) && (num_PRJ <= 500))
        printf("Num = %d Is Between 200 And 500.\n\n", num_PRJ);

    if ((num_PRJ > 500) && (num_PRJ <= 1000))
        printf("Num = %d Is Between 500 And 1000.\n\n", num_PRJ);

    if ((num_PRJ > 1000) && (num_PRJ <= 10000))
        printf("Num = %d Is Between 1000 And 10000.\n\n", num_PRJ);

    if ((num_PRJ > 10000))
        printf("Num = %d Is Greater Than 10,000.\n\n", num_PRJ);

    return (0);
}
