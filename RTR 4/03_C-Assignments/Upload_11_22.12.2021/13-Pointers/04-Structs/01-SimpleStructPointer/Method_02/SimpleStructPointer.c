#include <stdio.h>

// DEFINING STRUCT
struct MyData_PRJ
{
    int i;
    float f;
    double d;
};

int main(void)
{
    // Variable declartions
    int i_size_PRJ;
    int f_size_PRJ;
    int d_size_PRJ;
    int struct_My_Data_Size_PRJ;
    int PointerToStructMyDataSize;

    struct MyData_PRJ *pData = NULL;

    // code
    printf("\n\n");

    pData = (struct MyData_PRJ *)malloc(sizeof(struct MyData_PRJ));

    if (pData == NULL)
    {
        printf("FAILED TO ALLOCATE MEMORY TO 'struct MyDataa' !!! EXITTING NOW...\n\n");
        exit(0);
    }
    else
        printf("SUCCESSFULLY ALLOCATED MEMORY TO 'struct MyData' !!!\n\n");

    //Assigning Data Values To The Data Members Of 'struct MyData'
    (*pData).i = 30;
    (*pData).f = 11.45f;
    (*pData).d = 1.25553;

    //Diplaying Values Of the Data Members of 'struct MyData_PRJ'
    printf("\n\n");
    printf("DATA MEMBERS OF struct  MyData+PRJ : \n\n");
    printf(" i = %d \n", pData->i);
    printf(" f = %f \n", pData->f);
    printf(" d = %lf \n", pData->d);

    //Calculating Sizes (In Bytes) Of the Data Memebrs Of struct MyData_PRJ
    i_size_PRJ = sizeof(pData->i);
    f_size_PRJ = sizeof(pData->f);
    d_size_PRJ = sizeof(pData->d);

    // Displaying Sizes (In Bytes) Of The Members of Stuct MyData_PRJ
    printf("\n\n");
    printf("Sizes (in Byes) DATA MEMBERS OF struct  MyData_PRJ : \n\n");
    printf("Size of i = %d \n", i_size_PRJ);
    printf("Size of  f = %d \n", f_size_PRJ);
    printf("Size of  d = %d \n", d_size_PRJ);

    // CALCULAITNG SIZES INT BYTES OF THE ENTIRE struct MyData
    struct_My_Data_Size_PRJ = sizeof(struct MyData_PRJ);
    PointerToStructMyDataSize = sizeof(struct MyData_PRJ *);

    // Displaying Sizes in Bytes Of The entire struct Mydata
    printf("\n\n");
    printf("Size of 'struct MyData_PRJ' : %d bytes\n\n", struct_My_Data_Size_PRJ);
    printf("size of pointer to 'struct MyData : %d bytes \n\n", PointerToStructMyDataSize);

    if (pData)
    {
        free(pData);
        pData = NULL;
        printf("MEMORY ALLOCATED TO 'struct MyData' Has BEEM SUCCESSFULLY FREED !!!\n\n");
    }

    return (0);
}