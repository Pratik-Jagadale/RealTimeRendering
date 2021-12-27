// Header Files
#include <stdio.h>

// STRUCT definition
struct MyNumber
{
    int num_PRJ;
    int num_table_PRJ[10];
};

struct NumTables
{
    struct MyNumber n;
};

// Entry Point Function
int main(void)
{
    //variable declartion
    struct NumTables tables[10]; //an arrayof 10 'struct numTables'
    int i, j;

    // code

    for (i = 0; i < 10; i++)
    {
        tables[i].n.num_PRJ = (i + 1);
    }

    for (i = 0; i < 10; i++)
    {
        printf("\n\n");
        printf("Table of %d : \n\n", tables[i].n.num_PRJ);

        for (j = 0; j < 10; j++)
        {
            tables[i].n.num_table_PRJ[j] = tables[i].n.num_PRJ * (j + 1);
            printf("%d * %d = %d  \n\n", tables[i].n.num_PRJ, (j + 1), tables[i].n.num_table_PRJ[j]);
        }
    }

    return (0);
}
