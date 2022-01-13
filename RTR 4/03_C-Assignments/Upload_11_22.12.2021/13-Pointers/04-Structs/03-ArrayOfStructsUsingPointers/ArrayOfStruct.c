#include <stdio.h>
#include <ctype.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee_PRJ
{
    char name_PRJ[NAME_LENGTH];
    int age_PRJ;
    char sex_PRJ;
    float salary_PRJ;
    char marital_status_PRJ;
};

int main(void)
{
    // fucntion prototype
    void MyGetString(char[], int);

    // variable declartions
    struct Employee_PRJ *pEmployeeRecord = NULL;
    int num_employees, i;

    // code
    printf("\n\n");
    printf("Enter Number Of Employyes whose Details You want To Record :");
    scanf("%d", &num_employees);

    printf("\n\n");
    pEmployeeRecord = (struct Employee_PRJ *)malloc(sizeof(struct Employee_PRJ));

    if (pEmployeeRecord == NULL)
    {
        printf("FAILED TO ALLOCATED MEMORY FOR %d EMPLOYEES !!! EXITTING NOW...\n\n", num_employees);
        exit(0);
    }
    else
        printf("SUCCESSFFULLY ALLOCATED MEMORY FOR %d EEMPLOYEES !!!\n\n", num_employees);

    // *** USER INPUT INITAILIZATION OF ARRAY struct EMPLOYEE ***

    for (i = 0; i < num_employees; i++)
    {
        printf("\n\n\n\n");
        printf("-------------------DATA ENTRY FOR EMPLOYEE NUMBER %d---------------\n", (i + 1));
        printf("\n\n");
        printf("Enter Employee Name: ");
        MyGetString(pEmployeeRecord[i].name_PRJ, NAME_LENGTH);

        printf("\n\n\n");
        printf("Enter Employees age (in years) : ");
        scanf("%d", &pEmployeeRecord[i].age_PRJ);
    }
    return (0);
}
