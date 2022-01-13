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
    // function prototype
    void ChangeValues(struct MyData_PRJ *);

    // Variable declartions
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
    pData->i = 30;
    pData->f = 11.45f;
    pData->d = 1.25553;

    //Diplaying Values Of the Data Members of 'struct MyData_PRJ'
    printf("\n\n");
    printf("DATA MEMBERS OF struct  MyData+PRJ : \n\n");
    printf(" i = %d \n", pData->i);
    printf(" f = %f \n", pData->f);
    printf(" d = %lf \n", pData->d);

    ChangeValues(pData);

    //Diplaying Values Of the Data Members of 'struct MyData_PRJ'
    printf("\n\n");
    printf("DATA MEMBERS OF struct  MyData+PRJ : \n\n");
    printf(" i = %d \n", pData->i);
    printf(" f = %f \n", pData->f);
    printf(" d = %lf \n", pData->d);

    if (pData)
    {
        free(pData);
        pData = NULL;
        printf("MEMORY ALLOCATED TO 'struct MyData' Has BEEM SUCCESSFULLY FREED !!!\n\n");
    }

    return (0);
}

void ChangeValues(struct MyData_PRJ *pParamData)
{
    //code
    pParamData->i = 9;
    pParamData->f = 1.3f;
    pParamData->d = 6.25115;
}
