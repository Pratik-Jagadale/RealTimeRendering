#include <stdio.h>

#define MAX_NAME_LENGTH 100

struct Employee
{
    char name[MAX_NAME_LENGTH];
    unsigned int age;
    char gender;
    double salary;
};

struct MyData
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{

    // Typedef
    typedef struct Employee MY_EMPLOYEE_TYPE;
    typedef struct MyData MY_DATA_TYPE;

    //variable declartion
    struct Employee emp_PRJ = {"ABC", 25, 'M', 10000.00};
    MY_EMPLOYEE_TYPE emp_typedef_PRJ = {"XYZ", 23, 'F', 20000.00};

    struct MyData md_PRJ = {30, 11.14f, 26.2555, 'x'};
    MY_DATA_TYPE md_typedef;

    //code
    md_typedef.i = 9;
    md_typedef.f = 1.3f;
    md_typedef.d = 8.251151;
    md_typedef.c = 'P';

    printf("\n\n");
    printf("struct Employee : \n\n");
    printf("emp_PRJ.name = %s\n", emp_PRJ.name);
    printf("emp_PRJ.age = %d\n", emp_PRJ.age);
    printf("emp_PRJ.gender = %c\n", emp_PRJ.gender);
    printf("emp_PRJ.salary = %lf\n", emp_PRJ.salary);
    printf("\n\n");

    printf("MY_EMPLOYEE_TYPE : \n\n");
    printf("emp_typedef_PRJ.name = %s\n", emp_typedef_PRJ.name);
    printf("emp_typedef_PRJ.age = %d\n", emp_typedef_PRJ.age);
    printf("emp_typedef_PRJ.gender = %c\n", emp_typedef_PRJ.gender);
    printf("emp_typedef_PRJ.salary = %lf\n", emp_typedef_PRJ.salary);

    printf("\n\n");
    printf("struct MyData : \n\n");
    printf("md_PRJ.i = %d\n", md_PRJ.i);
    printf("md_PRJ.f = %f\n", md_PRJ.f);
    printf("md_PRJ.d = %lf\n", md_PRJ.d);
    printf("md_PRJ.c = %c\n", md_PRJ.c);

    printf("\n\n");
    printf("MY_DATA_TYPE : \n\n");
    printf("md_typedef.i = %d\n", md_typedef.i);
    printf("md_typedef.f = %f\n", md_typedef.f);
    printf("md_typedef.d = %lf\n", md_typedef.d);
    printf("md_typedef.c = %c\n", md_typedef.c);

    return (0);
}
