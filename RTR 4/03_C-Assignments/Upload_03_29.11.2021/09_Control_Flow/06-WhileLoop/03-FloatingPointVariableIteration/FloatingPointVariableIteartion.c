#include <stdio.h>

int main(void)
{
    /* variable declartions */
    float f_PRJ;
    float f_num_PRJ = 1.12f;

    /* code */
    printf("\n\n");

    printf("Printing Numbers %f to %f: \n\n", f_num_PRJ, (f_num_PRJ * 10.0f));

    f_PRJ = f_num_PRJ;
    while (f_PRJ < (f_num_PRJ * 10.0f))
    {
        printf("\t%f\n", f_PRJ);
        f_PRJ = f_PRJ + f_num_PRJ;
    }
    printf("\n\n");

    return (0);
}