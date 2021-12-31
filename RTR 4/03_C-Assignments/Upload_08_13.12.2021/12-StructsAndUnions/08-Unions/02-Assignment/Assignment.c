#include <stdio.h>

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
    union MyUnion u_PRJ, u2_PRJ;

    //code
    //Displaying Values Of The Data Members os 'struct MyData'
    printf("\n\n");
    printf("Members of union u1 Are : \n\n");

    u_PRJ.i = 6;
    u_PRJ.f = 1.2f;
    u_PRJ.d = 8.3333333;
    u_PRJ.c = 'P';

    printf("u_PRJ.i = %d\n", u_PRJ.i);
    printf("u_PRJ.f = %f\n", u_PRJ.f);
    printf("u_PRJ.d = %lf\n", u_PRJ.d);
    printf("u_PRJ.c = %c\n", u_PRJ.c);

    printf("Address of Members Of Union u_PRJ Are :\n\n");
    printf("u_PRJ.i = %p\n", &u_PRJ.i);
    printf("u_PRJ.f = %p\n", &u_PRJ.f);
    printf("u_PRJ.d = %p\n", &u_PRJ.d);
    printf("u_PRJ.c = %p\n", &u_PRJ.c);

    printf("\nMyUnion u_PRJ = %p\n\n", &u_PRJ);

    // *** MyUnion u2_PRJ ***
    printf("\n\n");
    printf("Members of union u2 Are : \n");

    u2_PRJ.i = 3;
    printf("u_PRJ.i = %d\n", u2_PRJ.i);

    u2_PRJ.f = 4.5f;
    printf("u_PRJ.f = %f\n", u2_PRJ.f);

    u2_PRJ.d = 5.51533333;
    printf("u_PRJ.d = %lf\n", u2_PRJ.d);

    u2_PRJ.c = 'J';
    printf("u_PRJ.c = %c\n\n", u2_PRJ.c);

    printf("Address of Members Of Union u2_PRJ Are :\n");
    printf("u2_PRJ.i = %p\n", &u2_PRJ.i);
    printf("u2_PRJ.f = %p\n", &u2_PRJ.f);
    printf("u2_PRJ.lf = %p\n", &u2_PRJ.d);
    printf("u2_PRJ.c = %p\n", &u2_PRJ.c);
    printf("\nMyUnion u2_PRJ = %p\n\n", &u2_PRJ);

    return (0);
}