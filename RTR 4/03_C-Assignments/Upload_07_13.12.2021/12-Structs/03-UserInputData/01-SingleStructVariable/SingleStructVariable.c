#include <stdio.h>
#include <conio.h>

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
    //variable declartions
    struct MyData data_PRJ; //Declaring a Single Struct Variable of type 'struct MyData' locally...

    //code
    // User Input For Values of Data Members of 'struct MyData'
    printf("\n\n");
    printf("Enter Integer Value For Data Member 'i' Of 'Struct MyData' : \n");
    scanf("%d", &data_PRJ.i);

    printf("Enter Floatig Value For Data Member 'f' Of 'Struct MyData' : \n");
    scanf("%f", &data_PRJ.f);

    printf("Enter 'Double' Value For Data Member 'd' Of 'Struct MyData' : \n");
    scanf("%lf", &data_PRJ.d);

    printf("Enter 'char' Value For Data Member 'c' Of 'Struct MyData' : \n");
    data_PRJ.c = getch();

    //Displaying Values Of the Data Members Of struct MyData
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' Are : \n\n");
    printf("i = %d \n", data_PRJ.i);
    printf("f = %f \n", data_PRJ.f);
    printf("d = %lf \n", data_PRJ.d);
    printf("c = %c \n", data_PRJ.c);

    return (0);
}
