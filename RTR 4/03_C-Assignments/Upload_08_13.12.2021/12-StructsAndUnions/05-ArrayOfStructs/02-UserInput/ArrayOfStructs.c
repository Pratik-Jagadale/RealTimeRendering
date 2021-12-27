//Header Files
#include <stdio.h>
#include <ctype.h>

// Constants
#define NUM_EMPLOYEE 5 // Simply Change this constant value to have as many number of employee records as you please...

#define NAME_LENGTH 100

//struct definition
struct Employee
{
    char name[NAME_LENGTH];
    int age;
    float salary;
    char sex;
    char marital_status;
};

// Entry Point Function
int main(void)
{
    //fucntion prototype
    void MyGetString(char[], int);

    //Variable Declaration
    struct Employee EmployeeRecord_PRJ[NUM_EMPLOYEE]; // An Array of NUM_EMPLOYEE structs - Each being type structs Employee

    int i;

    //code
    // *** User Input Initialization of Array of 'struct Employee' ***
    for (i = 0; i < NUM_EMPLOYEE; i++)
    {
        printf("*** Data Entry for Employee Number %d **** \n\n", (i + 1));

        printf("\n\n");
        printf("Enter Employee name :\n");
        MyGetString(EmployeeRecord_PRJ[i].name, NAME_LENGTH);

        printf("\n\n");
        printf("Enter Employee's Age (in years) :");
        scanf("%d", &EmployeeRecord_PRJ[i].age);

        printf("\n\n");
        printf("Enter Employee's Sex (M/m for male, f/F for female) :");
        EmployeeRecord_PRJ[i].sex = getch();
        printf("%c", EmployeeRecord_PRJ[i].sex);
        EmployeeRecord_PRJ[i].sex = toupper(EmployeeRecord_PRJ[i].sex);

        printf("\n\n");
        printf("Enter Employee's Salary (In Indian Rupees :");
        scanf("%f", &EmployeeRecord_PRJ[i].salary);

        printf("\n\n");
        printf("Is Employee MArried ?(Y/y for Yes, n/N for No) : ");
        EmployeeRecord_PRJ[i].marital_status = getch();
        printf("%c \n", EmployeeRecord_PRJ[i].marital_status);
        EmployeeRecord_PRJ[i].marital_status = toupper(EmployeeRecord_PRJ[i].marital_status);
        printf("\n\n");
    }

    // *** Display ***
    printf("\n\n");
    printf("*** DISPLAYING EMPLOYEE RECORDS ***");
    for (i = 0; i < NUM_EMPLOYEE; i++)
    {
        printf("*** Employee Number %d **** \n\n", (i + 1));
        printf("Name                   = %s \n", EmployeeRecord_PRJ[i].name);
        printf("Age                    = %d \n", EmployeeRecord_PRJ[i].age);

        if (EmployeeRecord_PRJ[i].sex == 'M' || EmployeeRecord_PRJ[i].sex == 'm')
            printf("Sex                    = Male \n");
        else
            printf("Sex                    = Female \n");

        printf("Salary                 = %.2f \n", EmployeeRecord_PRJ[i].salary);

        if (EmployeeRecord_PRJ[i].marital_status == 'Y' || EmployeeRecord_PRJ[i].marital_status == 'y')
            printf("Marital Status          Married\n");
        else
            printf("Marital Status          UnMarried\n");

        printf("\n\n");
    }
    return (0);
}

void MyGetString(char str[], int str_size)
{
    //VARIABLE DECLATION
    int i;
    char ch = '\0';

    //code
    i = 0;
    do
    {
        ch = getch();
        str[i] = ch;
        printf("%c", str[i]);
        i++;
    } while ((ch != '\r') && (i < str_size));

    if (i == str_size)
        str[i - 1] = '\0';
    else
        str[i] = '\0';
}