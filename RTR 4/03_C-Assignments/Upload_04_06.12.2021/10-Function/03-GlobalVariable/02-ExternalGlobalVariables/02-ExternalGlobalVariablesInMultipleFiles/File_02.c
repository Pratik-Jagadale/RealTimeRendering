#include <stdio.h>

//redeclaring variable
extern int global_count_PRJ;

/* Global Scope  */
void change_count_two(void)
{
    //code
    global_count_PRJ = global_count_PRJ + 2;
    printf("change_count_two() : Value of global_count in File_02 = %d\n", global_count_PRJ);
}

// global_count is a global variable declared in source code file
// ExternalGlobalVariablesInMultipleFiles.c
// To access it in this file, it must first be re-declared as an external variable
// in the global scope of this file along with the 'extern' keyword and its proper
// data type
// Then, it can be used as any ordinary global variable throughout this file as
// well