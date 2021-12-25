#include <stdio.h>

//Global typedef
typedef int MY_INT_PRJ; //type int had been redefined  as MY_INT_PRJ /...now MY_INT_PRJ can be treated just like int

int main(void)
{
    //fucntion prototype
    MY_INT_PRJ Add(MY_INT_PRJ, MY_INT_PRJ);

    //Typedefs
    typedef float PRJ_fLOAT;
    typedef char CHARACTER_PRJ;
    typedef double MY_DOUBLE_PRJ;

    // *** JUST LIKE IN Win32SDK !!! ***
    typedef unsigned int UNIT;
    typedef UNIT HANDLE;
    typedef HANDLE HWND;
    typedef HWND HINSTANCE;

    //variable declartion
    MY_INT_PRJ a = 10, i;
    MY_INT_PRJ iArray[] = {9, 18, 27, 36, 45, 54, 63, 72, 81, 90};

    PRJ_fLOAT f_prj = 30.9f;
    const PRJ_fLOAT f_PRJ_pi = 3.14f;

    CHARACTER_PRJ ch = '*';
    CHARACTER_PRJ chArray_01[] = "Hello";
    CHARACTER_PRJ chArray_02[][10] = {"RTR", "Batch", "2021-2022"};

    MY_DOUBLE_PRJ d = 8.04155;

    // *** JUST RANDOM NUMBERS - THEY HAVE NOTHING TO DO WITH ANY WINDOW'S HANDLE
    // OR INSTANCE HANLE !!! THIS IS A JUST FOR UNDERSTANDING

    UNIT uint = 3555;
    HANDLE hHandle = 984;
    HWND hwnd = 98524;
    HINSTANCE hInstance = 1422;

    //code
    printf("\n\n");
    printf("Type MY_INT_PRJ varibale a = %d \n", a);

    printf("\n\n");

    for (i = 0; i < (sizeof(iArray) / sizeof(int)); i++)
    {
        printf("Type MY_INT_PRJ arrau variable iArray[%d] = %d\n", i + 1, iArray[i]);
    }

    printf("\n\n");

    printf("Type PRJ_FLOAT variable f = %f \n", f_prj);
    printf("Type PRJ_FLOAT constant f_prj =_pi = %f", f_prj);

    printf("\n\n");
    printf("Type MY_DOUBLE_PRJ variable d = %lf\n", d);

    printf("\n\n");
    printf("Type CHARACTER variable ch = %c\n", ch);

    printf("\n\n");
    printf("Type CHARACTER array variable chArray_01 = %s\n", chArray_01);

    printf("\n\n");

    for (i = 0; i < (sizeof(chArray_02) / sizeof(chArray_02[0])); i++)
    {
        printf("%s\t", chArray_02[i]);
    }

    printf("\n\n");
    printf("Type UINT varibale uint = %u\n\n", uint);
    printf("Type HANDLE varibale uint = %u\n\n", hHandle);
    printf("Type HWND varibale uint = %u\n\n", hwnd);
    printf("Type HISNTANCE varibale uint = %u\n\n", hInstance);

    printf("\n\n");

    MY_INT_PRJ x = 90;
    MY_INT_PRJ y = 30;
    MY_INT_PRJ ret;

    ret = Add(x, y);
    printf("ret = %d \n\n", ret);

    return (0);
}

MY_INT_PRJ Add(MY_INT_PRJ a, MY_INT_PRJ b)
{
    // code
    MY_INT_PRJ c;
    c = a + b;
    return (c);
}