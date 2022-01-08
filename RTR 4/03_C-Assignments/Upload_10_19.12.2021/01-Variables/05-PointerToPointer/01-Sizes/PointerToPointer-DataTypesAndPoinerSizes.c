#include <stdio.h>

struct Employee_PRJ
{
    char name[100];
    int age;
    float salary;
    char sex;
    char merrital_status;
};

int main(void)
{
    printf("\n\n");
    printf("SIZES OF DATA TYPES AND POINTERS TO THOSE RESPECTIVE DATA TYPES ARE :\n\n");

    printf("Size of (int)     :%d \t\t\t Size of Pointer to int (int*)    %d \t\t\t  size of pointer to pointer int (int**)    :%d\n\n", sizeof(int), sizeof(int *), sizeof(int **));
    printf("Size of (float)     :%d \t\t\t Size of Pointer to float (float*)    %d \t\t\t  size of pointer to pointer float (float**)    :%d\n\n", sizeof(float), sizeof(float *), sizeof(float **));
    printf("Size of (double)     :%d \t\t\t Size of Pointer to double (double*)    %d \t\t\t  size of pointer to pointer double (double**)    :%d\n\n", sizeof(double), sizeof(double *), sizeof(double **));
    printf("Size of (char)     :%d \t\t\t Size of Pointer to char (char*)    %d \t\t\t  size of pointer to pointer char (char**)    :%d\n\n", sizeof(char), sizeof(char *), sizeof(char **));

    printf("Size of (struct Employye_PRJ)     :%d \t\t\t Size of Pointer to struct Employye_PRJ (struct Employye_PRJ*)    %d \t\t\t  size of pointer to pointer struct Employye_PRJ (struct Employye_PRJ**)    :%d\n\n", sizeof(struct Employee_PRJ), sizeof(struct Employee_PRJ *), sizeof(struct Employee_PRJ **));

    return (0);
}
