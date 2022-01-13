#include <stdio.h>

int main(void)
{
    // fucntion prototype
    int AddIntegers(int, int);
    int SubstractIntgers(int, int);
    float AddFloats(float, float);

    // variable declartions
    typedef int (*AddIntsFnPtr)(int, int);

    AddIntsFnPtr ptrAddTwoIntegers = NULL;
    AddIntsFnPtr ptrFunc = NULL;

    typedef float (*AddFloatsFnPtr)(float, float);

    AddFloatsFnPtr ptrAddTwoFloats = NULL;

    int iAnswer = 0;
    float fAnswer = 0.0f;

    //code
    ptrAddTwoIntegers = AddIntegers;
    iAnswer = ptrAddTwoIntegers(9, 30);
    printf("\n\n");
    printf("Sum Of Integers = %d \n\n", iAnswer);

    ptrFunc = SubstractIntgers;
    iAnswer = ptrFunc(30, 10);
    printf("\n\n");
    printf("Subtraction Of Integers = %d \n\n", iAnswer);

    ptrAddTwoFloats = AddFloats;
    fAnswer = ptrAddTwoFloats(12.23f, 63.2f);
    printf("\n\n");
    printf("Sum Of Floating-Point Numbers = %f\n\n", fAnswer);

    return (0);
}

int AddIntegers(int a, int b)
{
    // code
    return (a + b);
}

int SubstractIntgers(int a, int b)
{
    if (a > b)
        return (a - b);
    else
        return (b - a);
}

float AddFloats(float a, float b)
{
    //code
    return (a + b);
}
