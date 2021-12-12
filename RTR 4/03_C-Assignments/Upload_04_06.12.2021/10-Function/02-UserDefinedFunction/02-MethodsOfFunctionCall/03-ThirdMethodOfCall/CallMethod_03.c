#include <stdio.h>

// ****** USER DEFINED FUNCTIONS : METHOD OF CALLING FUNCTION 2 ******
// ****** CALLING ONLY TWO FUNCTIONS DIRECTLY IN main(), REST OF THE FUNCTIONS
//TRACE THEIR CALL INDIRECTLY TO main() ******

int main(int argc, char *argv[], char *envp[])
{
    //function Prototye
    void Function_Country(void);

    //code
    Function_Country(); //fucntion call

    return (0);
}

void Function_Country(void)
{
    //function prototype
    void FunctionOfAMC(void);

    //code
    FunctionOfAMC(); //FUNCTION CALL

    printf("\n\n");
    printf("I Live In India.");
    printf("\n\n");
}

void FunctionOfAMC(void)
{
    //function Prototye
    void Function_Surname(void);

    //code
    Function_Surname(); //FUNCTION CALL

    printf("\n\n");
    printf("Of ASTROMEDICOMP");
}

void Function_Surname(void)
{
    //function Prototye
    void Function_middleName(void);

    //code
    Function_middleName(); //FUNCTION CALL

    printf("\n\n");
    printf("Jagadale");
}

void Function_middleName(void)
{
    //function Prototye
    void Function_FirstName(void);

    //code
    Function_FirstName(); //FUNCTION CALL

    printf("\n\n");
    printf("Rajendra");
}

void Function_FirstName(void)
{
    //function Prototye
    void Function_Is(void);

    //code
    Function_Is(); //FUNCTION CALL

    printf("\n\n");
    printf("Pratik");
}

void Function_Is(void)
{
    //function Prototye
    void Function_Name(void);

    //code
    Function_Name(); //FUNCTION CALL

    printf("\n\n");
    printf("Is");
}

void Function_Name(void)
{
    //function Prototye
    void Function_My(void);

    //code
    Function_My(); //FUNCTION CALL

    printf("\n\n");
    printf("Name");
}

void Function_My(void)
{
    //code
    printf("\n\n");
    printf("My");
}
