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
    // function prototpye
    struct MyData AddStructMembers(struct MyData, struct MyData, struct MyData);

    //variable ddeclartions
    struct MyData data1_PRJ, data2_PRJ, data3_PRJ, answer_data;

    //code
    // *** data1 ***
    printf("\n\n");
    printf("**** DATA 1 ****\n\n");
    printf("Enter Integer Value For 'i of struct MyData1 :");
    scanf("%d", &data1_PRJ.i);

    printf("\n\n");
    printf("Enter float Value For 'f' of struct MyData1 :");
    scanf("%f", &data1_PRJ.f);

    printf("\n\n");
    printf("Enter double Value For 'd' of struct MyData1 :");
    scanf("%lf", &data1_PRJ.d);

    printf("\n\n");
    printf("Enter char Value For 'c' of struct MyData1 :");
    data1_PRJ.c = getch();
    printf("%c", data1_PRJ.c);

    // *** data2 ***
    printf("\n\n");
    printf("**** DATA 2 ****\n\n");
    printf("Enter Integer Value For 'i of struct MyData2 :");
    scanf("%d", &data2_PRJ.i);

    printf("\n\n");
    printf("Enter float Value For 'f' of struct MyData2 :");
    scanf("%f", &data2_PRJ.f);

    printf("\n\n");
    printf("Enter double Value For 'd' of struct MyData2 :");
    scanf("%lf", &data2_PRJ.d);

    printf("\n\n");
    printf("Enter char Value For 'c' of struct MyData2 :");
    data2_PRJ.c = getch();
    printf("%c", data2_PRJ.c);

    // *** data 3 ***
    printf("\n\n");
    printf("**** DATA 3 ****\n\n");
    printf("Enter Integer Value For 'i of struct MyData3 :");
    scanf("%d", &data3_PRJ.i);

    printf("\n\n");
    printf("Enter float Value For 'f' of struct MyData2 :");
    scanf("%f", &data3_PRJ.f);

    printf("\n\n");
    printf("Enter double Value For 'd' of struct MyData3 :");
    scanf("%lf", &data3_PRJ.d);

    printf("\n\n");
    printf("Enter char Value For 'c' of struct MyData3 :");
    data3_PRJ.c = getch();
    printf("%c", data3_PRJ.c);

    // *** CALLING FUNCTION AddStructMembers() WHICH ACCEPTS THREE VARIABLES OF
    // TYPE 'struct MyData' AS PARAMETERS AND ADDS UP THE RESPECTIVE MEMBERS AND
    // RETURNS THE ANSWER IN ANOTHER struct OF SAME TYPE ***

    answer_data = AddStructMembers(data1_PRJ, data2_PRJ, data3_PRJ);

    printf("\n\n\n");
    printf("*** ANSWERS ***\n");
    printf("answer_data.i = %d\n", answer_data.i);
    printf("answer_data.f = %f\n", answer_data.d);
    printf("answer_data.d = %lf\n", answer_data.f);

    answer_data.c = data1_PRJ.c;
    printf("answer_data.c (from data1) = %c\n", answer_data.c);

    answer_data.c = data2_PRJ.c;
    printf("answer_data.c (from data2) = %c\n", answer_data.c);

    answer_data.c = data3_PRJ.c;
    printf("answer_data.c (from data3) = %c\n", answer_data.c);

    return (0);
}

struct MyData AddStructMembers(struct MyData md_one_PRJ, struct MyData md_two_PRJ, struct MyData md_three_PRJ)
{
    //variable declarations
    struct MyData answer_PRJ;

    //code
    answer_PRJ.i = md_one_PRJ.i + md_two_PRJ.i + md_three_PRJ.i;
    answer_PRJ.f = md_one_PRJ.f + md_two_PRJ.f + md_three_PRJ.f;
    answer_PRJ.d = md_one_PRJ.d + md_two_PRJ.d + md_three_PRJ.d;

    return (answer_PRJ);
}
