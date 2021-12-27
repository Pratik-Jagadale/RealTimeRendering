//Header Files
#include <stdio.h>

// Constants
#define NAME_LENGTH 100
#define MARITAL_STATUS 10

//struct definition
struct Employee
{
    char name[NAME_LENGTH];
    int age;
    float salary;
    char sex;
    char marital_status[MARITAL_STATUS];
};

// Entry Point Function
int main(void)
{
    //Variable Declaration
    struct Employee EmployeeRecord[5]; // An Array of 5 sructs - Each being type

    char employee_rajesh[] = "Rajesh";
    char employee_sameer[] = "Sameer";
    char employee_kalyani[] = "Kalyani";
    char employee_sonali[] = "Sonali";
    char employee_shanatunu[] = "Shanatanu";

    int i;

    //code
    // *** HARD CODE INITIALIZATION ofArray 'struct Employee' ***

    // *** EMPLOYEE 1 ***
    strcpy(EmployeeRecord[0].name, employee_rajesh);
    EmployeeRecord[0].age = 30;
    EmployeeRecord[0].sex = 'M';
    EmployeeRecord[0].salary = 50000.0f;
    strcpy(EmployeeRecord[0].marital_status, "Unmarried");

    // *** EMPLOYEE 2 ***
    strcpy(EmployeeRecord[1].name, employee_sameer);
    EmployeeRecord[1].age = 32;
    EmployeeRecord[1].sex = 'M';
    EmployeeRecord[1].salary = 50000.0f;
    strcpy(EmployeeRecord[1].marital_status, "Married");

    // *** EMPLOYEE 3 ***
    strcpy(EmployeeRecord[2].name, employee_kalyani);
    EmployeeRecord[2].age = 29;
    EmployeeRecord[2].sex = 'F';
    EmployeeRecord[2].salary = 62000.0f;
    strcpy(EmployeeRecord[2].marital_status, "Unmarried");

    // *** EMPLOYEE 4 ***
    strcpy(EmployeeRecord[3].name, employee_sonali);
    EmployeeRecord[3].age = 33;
    EmployeeRecord[3].sex = 'F';
    EmployeeRecord[3].salary = 50000.0f;
    strcpy(EmployeeRecord[3].marital_status, "Married");

    // *** EMPLOYEE 5 ***
    strcpy(EmployeeRecord[4].name, employee_shanatunu);
    EmployeeRecord[4].age = 35;
    EmployeeRecord[4].sex = 'M';
    EmployeeRecord[4].salary = 85000.0f;
    strcpy(EmployeeRecord[4].marital_status, "Married");

    // *** LOOP ASSIGNMENT ***
    printf("\n\n");
    printf("*** DISPLAYING EMPLOYEE RECORDS ***");
    for (i = 0; i < 5; i++)
    {
        printf("*** Employee Number %d **** \n\n", (i + 1));
        printf("Name                   = %s \n", EmployeeRecord[i].name);
        printf("Age                    = %d \n", EmployeeRecord[i].age);

        if (EmployeeRecord[i].sex == 'M' || EmployeeRecord[i].sex == 'm')
            printf("Sex                    = Male \n");
        else
            printf("Sex                    = Female \n");

        printf("Salary                 = %f \n", EmployeeRecord[i].salary);
        printf("Marital Status         = %s \n", EmployeeRecord[i].marital_status);
        printf("\n\n");
    }
    return (0);
}