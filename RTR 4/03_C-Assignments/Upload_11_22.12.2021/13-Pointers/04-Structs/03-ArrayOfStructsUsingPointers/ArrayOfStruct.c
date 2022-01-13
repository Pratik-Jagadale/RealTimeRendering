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
        printf("SUCCESSFFULLY ALLOCATED MEMORY FOR %d EMPLOYEES !!!\n\n", num_employees);

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

        printf("\n\n");
        printf("Enter Employees Sex (M/m For Male , F/f For female) : ");
        pEmployeeRecord[i].sex_PRJ = getch();
        printf("%c", pEmployeeRecord[i].sex_PRJ);
        pEmployeeRecord[i].sex_PRJ = toupper(pEmployeeRecord[i].sex_PRJ);

        printf("\n\n");
        printf("Enter Employee's Salary (in Indian Ruppes) :");
        scanf("%f", &pEmployeeRecord[i].salary_PRJ);

        printf("\n\n");
        printf("Is The Employee Married (Y/y For yes , N/n For No) : ");
        pEmployeeRecord[i].marital_status_PRJ = getch();
        printf("%c", pEmployeeRecord[i].marital_status_PRJ);
        pEmployeeRecord[i].marital_status_PRJ = toupper(pEmployeeRecord[i].marital_status_PRJ);
    }

    // DISPLAY
    printf("\n\n");
    printf("------ DISPLAYING EMPLOYEE RECORDS-------\n\n");
    printf("\n\n");
    printf("*** DISPLAYING EMPLOYEE RECORDS ***");
    for (i = 0; i < num_employees; i++)
    {
        printf("*** Employee Number %d **** \n\n", (i + 1));
        printf(" Name                   = %s \n", pEmployeeRecord[i].name_PRJ);
        printf("Age                    = %d \n", pEmployeeRecord[i].age_PRJ);

        if (pEmployeeRecord[i].sex_PRJ == 'M' || pEmployeeRecord[i].sex_PRJ == 'm')
            printf("Sex                    = Male \n");
        else
            printf("Sex                    = Female \n");

        printf("Salary                 = %.2f \n", pEmployeeRecord[i].salary_PRJ);

        if (pEmployeeRecord[i].marital_status_PRJ == 'Y' || pEmployeeRecord[i].marital_status_PRJ == 'y')
            printf("Marital Status          Married\n");
        else
            printf("Marital Status          UnMarried\n");

        printf("\n\n");
    }

    if (pEmployeeRecord)
    {
        free(pEmployeeRecord);
        pEmployeeRecord = NULL;
        printf("MEMORY ALLOCATION TO %d EMPLOYEES HAS BEEN SUCCESFULLY FREED !!!\n\n", num_employees);
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
