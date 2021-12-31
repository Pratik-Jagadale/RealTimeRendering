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
    struct MyData s_PRJ;
    union MyUnion u_PRJ;

    //code
    //Displaying Values Of The Data Members os 'struct MyData'
    printf("\n\n");
    printf("Size Of MyStruct = %lu\n", sizeof(s_PRJ));
    printf("\n\n");
    printf("Size of MyUnion = %lu\n\n", sizeof(u_PRJ)); // Occupies Size of the biggest data type...

    return (0);
}