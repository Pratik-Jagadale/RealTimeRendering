#include <stdio.h>

struct MyData
{
    int i;
    float f;
    double d;
    char c;
};

union MyUnion
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{
    //variable ddeclartions
    union MyUnion u_PRJ;
    struct MyData s_PRJ;

    //code
    printf("\n\n");
    printf("Members of Struct Are : \n\n");

    s_PRJ.i = 6;
    s_PRJ.f = 1.2f;
    s_PRJ.d = 8.3333333;
    s_PRJ.c = 'P';

    printf("s_PRJ.i = %d\n", s_PRJ.i);
    printf("s_PRJ.f = %f\n", s_PRJ.f);
    printf("s_PRJ.d = %lf\n", s_PRJ.d);
    printf("s_PRJ.c = %c\n\n", s_PRJ.c);

    printf("Address of Members Of Struct s_PRJ Are :\n");
    printf("s_PRJ.i = %p\n", &s_PRJ.i);
    printf("s_PRJ.f = %p\n", &s_PRJ.f);
    printf("s_PRJ.d = %p\n", &s_PRJ.d);
    printf("s_PRJ.c = %p\n", &s_PRJ.c);

    printf("MyStruct s_PRJ = %p \n\n", &s_PRJ);

    printf("\n\n");
    printf("Members of Union Are :\n");

    u_PRJ.i = 3;
    printf("u_PRJ.i = %d\n", u_PRJ.i);

    u_PRJ.f = 4.3f;
    printf("u_PRJ.f = %f\n", u_PRJ.f);

    u_PRJ.d = 4.4443333;
    printf("u_PRJ.d = %lf\n", u_PRJ.d);

    u_PRJ.c = 'K';
    printf("u_PRJ.c = %c\n\n", u_PRJ.c);

    printf("Address of Members Of Union u_PRJ Are :\n");
    printf("u_PRJ.i = %p\n", &u_PRJ.i);
    printf("u_PRJ.f = %p\n", &u_PRJ.f);
    printf("u_PRJ.d = %p\n", &u_PRJ.d);
    printf("u_PRJ.c = %p\n", &u_PRJ.c);

    printf("\nMyUnion u_PRJ = %p\n\n", &u_PRJ);

    return (0);
}