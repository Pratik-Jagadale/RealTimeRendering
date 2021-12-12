#include <stdio.h>

/* Global Scope */

int main(void)
{
    // ***local scope of main() begins

    //variable declarations
    // 'a' is a Local Vriable. It is Local to main() only

    int a_PRJ = 5;

    // function prototype
    void change_count(void);

    // code
    printf("\n");
    printf("A_PRJ = %d\n\n", a_PRJ);

    // local count is intialized to 0
    // local count = local_count + 1 = 1 + 2 = 1;
    change_count();

    // since ,local count is an ordinary local variable of change_count(), it
    // will NOT retain its value from previous call to change_count().
    // do local_counbt is Again initialized to 0
    // local_count = local_count + 1 = 0 + 1 = 1

    change_count();

    return (0);
    /*** Local scope of main() ends ***/
}

void change_count(void)
{
    /** Local Scope of change_count() begins **/

    // variable declaration
    // 'Local_Count 'is a Local Variable. It is local to change_count() only.
    int local_count = 0;

    //code
    local_count = local_count + 1;
    printf("Local Count = %d \n", local_count);

    //local scope of change_count() ends..
}
//global scope //