#include <stdio.h>

// DEFIENING STRUCT
struct MyData
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{
    //variable declartion
    /*
        data_One_PRJ -> 
        662 will assign to 'i'
        4.5 will assign to 'f'
        11.03335 will assign to 'c' 
    */
    struct MyData data_One_PRJ = {662, 4.5f, 11.03355, 'P'}; //Declaring a Single Struct Variable of type 'struct MyData' locally....

    /*
        data_Two_PRJ -> 
        ASCCI Value of 'P' will assign to 'i'
        4.5 will assign to 'f'
        Corresponding ASCCI symbol character  of 68 will assign to c   
    */
    struct MyData data_Two_PRJ = {'P', 4.5f, 11.03355, 68};

    /*
        data_Three_PRJ -> 
        36 will assign to 'i'
        ASCCI Value of 'P' will assign to 'f' by typecasting ascci value to float  
        0.00000 wil assign to 'd'
        no character will assign to 
    */
    struct MyData data_Three_PRJ = {36, 'P'};

    /*
        data_Three_PRJ -> 
        79 will assign to 'i'
        0.00000 will assign to 'f'
        0.00000 wil assign to 'd'
        no character will assign to 
    */
    struct MyData data_Four_PRJ = {79};

    //code
    //Displaying Values Of the Data Members Of struct MyData
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData data_One_PRJ' Are : \n\n");
    printf("i = %d \n", data_One_PRJ.i);
    printf("f = %.2f \n", data_One_PRJ.f);
    printf("d = %.2lf \n", data_One_PRJ.d);
    printf("c = %c \n", data_One_PRJ.c);
    printf("\n\n--------------------------------\n");

    printf("DATA MEMBERS OF 'struct MyData data_Two_PRJ' Are : \n\n");
    printf("i = %d \n", data_Two_PRJ.i);
    printf("f = %.2f \n", data_Two_PRJ.f);
    printf("d = %.2lf \n", data_Two_PRJ.d);
    printf("c = %c \n", data_Two_PRJ.c);
    printf("\n\n--------------------------------\n");

    printf("DATA MEMBERS OF 'struct MyData data_Three_PRJ' Are : \n\n");
    printf("i = %d \n", data_Three_PRJ.i);
    printf("f = %.2f \n", data_Three_PRJ.f);
    printf("d = %.2lf \n", data_Three_PRJ.d);
    printf("c = %c \n", data_Three_PRJ.c);
    printf("\n\n--------------------------------\n");

    printf("DATA MEMBERS OF 'struct MyData data_Four_PRJ' Are : \n\n");
    printf("i = %d \n", data_Four_PRJ.i);
    printf("f = %.2f \n", data_Four_PRJ.f);
    printf("d = %.2lf \n", data_Four_PRJ.d);
    printf("c = %c \n", data_Four_PRJ.c);
    printf("--------------------------------\n");

    return (0);
}
