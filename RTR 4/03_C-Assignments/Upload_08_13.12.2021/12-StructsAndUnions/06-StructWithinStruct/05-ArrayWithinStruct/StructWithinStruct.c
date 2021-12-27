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
    struct MyNumber a;
    struct MyNumber b;
    struct MyNumber c;
};

// Entry Point Function
int main(void)
{
    //variable declartion
    struct NumTables tables;
    int i;

    // code
    tables.a.num_PRJ = 2;
    for (i = 0; i < 10; i++)
        tables.a.num_table_PRJ[i] = tables.a.num_PRJ * (i + 1);
    printf("\n\n");
    printf("Table Of %d : \n\n", tables.a.num_PRJ);
    for (i = 0; i < 10; i++)
        printf("%d * %d = %d\n", tables.a.num_PRJ, (i + 1), tables.a.num_table_PRJ[i]);

    tables.b.num_PRJ = 3;
    for (i = 0; i < 10; i++)
        tables.b.num_table_PRJ[i] = tables.b.num_PRJ * (i + 1);
    printf("\n\n");
    printf("Table Of %d : \n\n", tables.b.num_PRJ);
    for (i = 0; i < 10; i++)
        printf("%d * %d = %d\n", tables.b.num_PRJ, (i + 1), tables.b.num_table_PRJ[i]);

    tables.c.num_PRJ = 4;
    for (i = 0; i < 10; i++)
        tables.c.num_table_PRJ[i] = tables.c.num_PRJ * (i + 1);
    printf("\n\n");
    printf("Table Of %d : \n\n", tables.c.num_PRJ);
    for (i = 0; i < 10; i++)
        printf("%d * %d = %d\n", tables.c.num_PRJ, (i + 1), tables.c.num_table_PRJ[i]);

    return (0);
}
