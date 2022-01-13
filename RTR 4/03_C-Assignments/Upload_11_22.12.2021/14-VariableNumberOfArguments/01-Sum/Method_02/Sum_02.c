#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int main(void)
{
    // fucntion prototype
    int CalculatingSum(int, ...);

    // variable declartions
    int answer_PRJ;

    // code
    printf("\n\n");

    answer_PRJ = CalculatingSum(5, 10, 20, 30, 40, 50);
    printf("Answer = %d\n\n", answer_PRJ);

    answer_PRJ = CalculatingSum(10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    printf("Answer = %d\n\n", answer_PRJ);

    answer_PRJ = CalculatingSum(0);
    printf("Answer = %d\n\n", answer_PRJ);

    return (0);
}

int CalculatingSum(int num, ...) //VARIADUC FUNCTION
{
    // variable declartions
    int sum_total = 0;
    int n;

    va_list numbers_list;

    //code

    va_start(numbers_list, num);

    sum_total = va_CalculateSum(num, numbers_list);

    va_end(numbers_list);
    return (sum_total);
}

int va_CalculateSum(int num, va_list list)
{
    // variable declartions
    int n;
    int sum_total = 0;

    while (num)
    {
        n = va_arg(list, int);
        sum_total = sum_total + n;
        num--;
    }
    return (sum_total);
}