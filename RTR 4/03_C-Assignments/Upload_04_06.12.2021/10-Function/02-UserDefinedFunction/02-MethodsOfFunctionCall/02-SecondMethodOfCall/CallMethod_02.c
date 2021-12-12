#include <stdio.h>

// ****** USER DEFINED FUNCTIONS : METHOD OF CALLING FUNCTION 2 ******
// ****** CALLING ONLY TWO FUNCTIONS DIRECTLY IN main(), REST OF THE FUNCTIONS
//TRACE THEIR CALL INDIRECTLY TO main() ******

int main(int argc, char *argv[], char *envp[])
{
    //function Prototye
    void display_information(void);
    void Function_Country(void);

    //code
    display_information(); //function call
    Function_Country();    //fucntion call

    return (0);
}

void display_information(void)
{
    //function prototype
    void Function_My(void);
    void Function_Name(void);
    void Function_Is(void);
    void Function_FirstName(void);
    void Function_middleName(void);
    void Function_Surname(void);
    void FunctionOfAMC(void);

    //code
    // Function Calls
    Function_My();
    Function_Name();
    Function_Is();
    Function_FirstName();
    Function_middleName();
    Function_Surname();
    FunctionOfAMC();
}

void Function_My(void)
{
    printf("\n\n");
    printf("My");
}

void Function_Name(void)
{
    printf("\n\n");
    printf("Name");
}

void Function_Is(void)
{
    printf("\n\n");
    printf("Is");
}

void Function_FirstName(void)
{
    printf("\n\n");
    printf("Pratik");
}

void Function_middleName(void)
{
    printf("\n\n");
    printf("Rajendra");
}

void Function_Surname(void)
{
    printf("\n\n");
    printf("Jagadale");
}

void FunctionOfAMC(void)
{
    printf("\n\n");
    printf("Of ASTROMEDICOMP");
}

void Function_Country(void)
{
    printf("\n\n");
    printf("I Live In India.");
    printf("\n\n");
}
