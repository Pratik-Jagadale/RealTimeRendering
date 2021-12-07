#include <stdio.h>

/* Entry Point Function */
int main(void)
{
    /*  variable declartions */
    int i, j, c;

    /* code */
    i = 0;
    do
    {
        j = 0;
        do
        {
            c = ((i & 0x4) == 0) ^ ((j & 0x8) == 0);

            if (c == 0)
                printf("  ");

            if (c == 1)
                printf("* ");
            j++;
        } while (j < 64);
        printf("\n\n");
        i++;
    } while (i < 64);
    return (0);
}
