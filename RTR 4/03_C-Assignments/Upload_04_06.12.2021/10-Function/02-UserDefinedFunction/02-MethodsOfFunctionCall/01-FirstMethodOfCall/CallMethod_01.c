#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    //function prototypes OR declartions
    void MyAddition(void);
    int MySubstarction(void);
    void MyMultiplication(int, int);
    int MyDivision(int, int);

    //variable declartions
    int result_substraction;
    int a_multiplication, b_multiplication;
    int a_division, b_division, result_division;

    //code

    /* ADDITION */
    MyAddition(); //function call

    /* Substraction */
    result_substraction = MySubstarction(); //function call
    printf("\n\n");
    printf("Substraction Yields Result = %d \n", result_substraction);

    /* Multiplication */
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Multiplication");
    scanf("%d", &a_multiplication);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Multiplication");
    scanf("%d", &b_multiplication);

    MyMultiplication(a_multiplication, b_multiplication); //function call

    // *** division ***
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Division :");
    scanf("%d", &a_division);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Division :");
    scanf("%d", &b_division);

    result_division = MyDivision(a_division, b_division);
    printf("\n\n");

    printf("Division of %dd and %d Gives = %d (Quotient)\n", a_division, b_division, result_division);

    printf("\n\n");

    return (0);
}

void MyAddition(void)
{
    //variable declaration: Local variable to MyAddition()
    int a, b, sum;

    /* code */
    printf("\n\n");
    printf("Enter Integer Value For 'A' for Addition : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Addition : ");
    scanf("%d", &b);

    sum = a + b;

    printf("\n\n");
    printf("Sum of %d and %d = %d \n\n", a, b, sum);
}

int MySubstarction(void)
{
    //variable declartion : localvariables to MySubstarction
    int a_PRJ, b_PRJ, substraction_PRJ;

    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Substarction : );
    scanf("%d", &a_PRJ);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Substarction : ");
    scanf("%d", &b_PRJ);

    substraction_PRJ = a_PRJ - b_PRJ;
    return (substraction_PRJ);
}

void MyMultiplication(int a_PRJ, int b_PRJ)
{
    //variable declaration
    int multiplication_PRJ;

    //code
    multiplication_PRJ = a_PRJ * b_PRJ;

    printf("\n\n");
    printf("Multiplication of %d and %d = %d \n\n", a_PRJ, b_PRJ, multiplication_PRJ);
}

int MyDivision(int a_PRJ, int b_PRJ)
{
    //code
    if (a_PRJ > b_PRJ)
        return (a_PRJ / b_PRJ);
    else
        return (b_PRJ / a_PRJ);
}
