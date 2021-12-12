#include <stdio.h>

/* Global Scope  */
int global_count_PRJ = 0;

// Entry Point Function
int main(void)
{
    //function prototype
    void change_count(void);
    void change_count_one(void); //function defines in file_01.c
    void change_count_two(void); //function defines in File_02.c

    //code
    printf("\n");

    change_count();
    change_count_one();
    change_count_two();

    return (0);
}

void change_count(void)
{
    //code
    global_count_PRJ = global_count_PRJ + 1;
    printf("Global Count = %d\n", global_count_PRJ);
}