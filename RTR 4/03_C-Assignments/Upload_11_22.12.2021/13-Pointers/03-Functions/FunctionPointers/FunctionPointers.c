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

    ptrFunc = Substract
}