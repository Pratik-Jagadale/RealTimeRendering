#include <stdio.h>

struct MyData
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{
    //variable ddeclartions
    struct MyData data_PRJ;

    //code
    //Assigning Data values To The Data Members Of 'struct MyData'
    data_PRJ.i = 30;
    data_PRJ.f = 11.45f;
    data_PRJ.d = 1.2365;
    data_PRJ.c = 'P';

    //Displaying Values Of The Data Members os 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE :\n\n");
    printf(" i = %d\n", data_PRJ.i);
    printf("f  = %f\n", data_PRJ.f);
    printf("d = %lf\n", data_PRJ.d);
    printf("c = %c\n", data_PRJ.c);

    printf("\n\n");
    printf("ADDRESSES OF THE DATA MEMBERS OF 'struct MyData' ARE :\n\n");
    printf("'i' Occupies Addresses From %p\n", &data_PRJ.i);
    printf("'f' Occupies Addresses From %p\n", &data_PRJ.f);
    printf("'d' Occupies Addresses From %p\n", &data_PRJ.d);
    printf("'c' Occupies Addresses From %p\n\n", &data_PRJ.c);

    printf("Starting Address of 'struct MyData' Variable 'data_PRJ'  = %p \n\n", &data_PRJ);

    return (0);
}