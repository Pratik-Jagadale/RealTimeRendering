#include <stdio.h>

// global scope

int main(void)
{
    //function prototype
    void change_count(void);

    //variable declartions
    extern int global_count_PRJ;

    //code

    printf("\n\n");
    printf("Value of global_count before change_count() = %d\n", global_count_PRJ);
    change_count();
    printf("Value of global_count After change_count() = %d\n", global_count_PRJ);
    return (0);
}

int global_count_PRJ = 0;

void change_count(void)
{
    //code
    printf("\n\n");
    global_count_PRJ = 5;
    printf("Value of global_count In change_count() = %d\n", global_count_PRJ);
    printf("\n\n");
}
