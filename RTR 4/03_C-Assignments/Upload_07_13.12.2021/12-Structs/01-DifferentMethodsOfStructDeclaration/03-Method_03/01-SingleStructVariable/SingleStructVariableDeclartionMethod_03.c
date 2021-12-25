#include <stdio.h>

int main(void)
{
    // DEFIENING STRUCT
    struct MyData
    {
        int i;
        float f;
        double d;
    } data_PRJ; //Declaring a Single Struct Variable pf type struct MyData locally...

    //variable declartions
    int i_size_PRJ;
    int f_size_PRJ;
    int d_size_PRJ;
    int struct_MyData_size_PRJ;

    //code
    //Assigning Data Values To The Data Members Of struct MyData
    data_PRJ.i = 562;
    data_PRJ.f = 3.13f;
    data_PRJ.d = 3651.255;

    //Displaying Values Of the Data Members Of struct MyData
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' Are : \n\n");
    printf("i = %d \n", data_PRJ.i);
    printf("f = %f \n", data_PRJ.f);
    printf("d = %lf \n", data_PRJ.d);

    //Calculaitng Sizes (In Bytes) of the data members of 'struct MyData'
    i_size_PRJ = sizeof(data_PRJ.i);
    f_size_PRJ = sizeof(data_PRJ.f);
    d_size_PRJ = sizeof(data_PRJ.d);

    //Displaying sizes (In Bytes) Of the Data Me,bers of struct MyDara
    printf("\n\n");
    printf("SIZES (in bytes ) of  DATA MEBEERS of 'struct MyData' Are :\n\n");
    printf("size of 'i' = %d bytes\n", i_size_PRJ);
    printf("size of 'f' = %d bytes\n", f_size_PRJ);
    printf("Size in 'd' = %d in bytes\n", d_size_PRJ);

    //Calculating Size (In Bytes) of the etire struct MyData
    struct_MyData_size_PRJ = sizeof(data_PRJ); //can also give struct name -> sizeof(MyData)

    printf("\n\n");
    printf("Size of 'struct MyData' : %d bytes \n\n", struct_MyData_size_PRJ);

    return (0);
}
