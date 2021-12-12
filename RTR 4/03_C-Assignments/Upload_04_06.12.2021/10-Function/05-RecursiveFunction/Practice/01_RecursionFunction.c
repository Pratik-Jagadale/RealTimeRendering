#include <stdio.h>

int main(void)
{
    //variable declaration

    unsigned int num_PRJ;

    //function prototype
    void recursive(unsigned int);

    //code
    printf("\n\n");
    printf("Enter Any Number : \n\n");
    scanf("%d", &num_PRJ);

    printf("\n\n");
    printf("Output of Recursive Function : \n\n");

    recursive(num_PRJ);

    printf("\n\n");
    return (0);
}

void recursive(unsigned int n)
{
    //code
    printf("n = %d\n", n);

    if (n > 0)
    {
        recursive(n - 1);
    }
}
