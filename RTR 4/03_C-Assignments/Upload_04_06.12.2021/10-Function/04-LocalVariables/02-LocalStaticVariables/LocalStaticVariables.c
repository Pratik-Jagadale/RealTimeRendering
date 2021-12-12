#include <stdio.h>

int main(void)
{
    //variable declartion

    int a_PRJ = 5;

    void change_count(void);

    //code
    printf("\n");
    printf("A = %d \n\n", a_PRJ);

    change_count();
    change_count();
    change_count();
    change_count();

    return (0);
}

void change_count(void)
{
    static int local_count = 0;

    //code
    local_count = local_count + 5;
    printf("Local Count = %d\n", local_count);
}
