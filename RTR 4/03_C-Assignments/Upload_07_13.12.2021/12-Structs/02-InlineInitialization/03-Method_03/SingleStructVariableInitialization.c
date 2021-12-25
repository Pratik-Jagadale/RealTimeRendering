#include <stdio.h>

int main(void)
{
    //Variable Declartion
    // DEFIENING STRUCT
    struct MyData
    {
        int i;
        float f;
        double d;
        char c;
    } data_PRJ = {662, 4.5f, 11.03355, 'P'}; //Declaring a Single Struct Variable of type 'struct MyData' Locally...

    //code
    //Displaying Values Of the Data Members Of struct MyData
    printf("\n\n");
    printf("DATA MEMBERS OF struct MyData Are : \n\n");
    printf("i = %d \n", data_PRJ.i);
    printf("f = %.2f \n", data_PRJ.f);
    printf("d = %.2lf \n", data_PRJ.d);
    printf("c = %c \n", data_PRJ.c);
    return (0);
}
