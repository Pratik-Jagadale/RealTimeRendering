#include <stdio.h>

//  GLOBAL SCOPE

//If not initialized by us, global variables are initialized to their zero values
//(with respect to their data types i.e: 0 for int, 0.0 for float and double,
//etc.) by default.
//But still, for good programming discipline, we shall explicitly initialize our
//global variable with 0.
int global_count_PRJ = 0; //by defualt global variable initializes to zer0

int main(void)
{
    //function prototype
    void change_count_one(void);
    void change_count_two(void);
    void change_count_three(void);

    printf("\n\n");

    printf("main() :  value of global_count_PRJ = %d \n", global_count_PRJ);

    change_count_one();
    change_count_two();
    change_count_three();

    printf("\n\n");
    return (0);
}

void change_count_one(void)
{
    //code
    global_count_PRJ = 100;
    printf("change_count_one() :  value of global_count_PRJ = %d \n", global_count_PRJ);
}

void change_count_two(void)
{

    //code
    global_count_PRJ = global_count_PRJ + 100;
    printf("change_count_two() :  value of global_count_PRJ = %d \n", global_count_PRJ);
}

void change_count_three(void)
{

    //code
    global_count_PRJ = global_count_PRJ + 100;
    printf("change_count_thre() :  value of global_count_PRJ = %d \n", global_count_PRJ);
}
