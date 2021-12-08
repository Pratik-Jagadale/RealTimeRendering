/*
    Assigment : Constants
    date: 2021_21_11
*/

#include <stdio.h>

/* Constant using #define macro */
#define PRATIK_PI 3.25269226535897932

#define AMC_STRING "AstroMedicomp TRT"

/* constant using enum */
/*
If First Constant Is not Assignemnet A value ,It Is Assumed To be 0
i.e: 'SUNDAY' Will Be :0
And The Rest of the Constrants are assigment consecutive Interger
values from 0 onwards i.e 'MONDAY' will be 1, 'TUESDAY' will be2 and so on...
*/

/* 1. Unamed enum */
enum
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
};

enum
{
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

/* 2. Named enums */
enum Numbers
{
    ONE,
    TWO,
    THREE,
    FOUR = 4,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN
};

enum boolean
{
    FALSE,
    TRUE,
};

int main(void)
{
    printf("------------------------------------------------\n");
    printf("\t\tConstants\n");
    printf("------------------------------------------------\n");

    /* local constant declartions */
    const double c = 0.000001;
    printf("Local Constant Epsilom = %lf\n", c);

    printf("------------------------------------------------\n");

    printf("Sunday Is Day Number = %d\n", SUNDAY);
    printf("Monday Is Day Number = %d\n", MONDAY);
    printf("Tuesday Is Day Number = %d\n", TUESDAY);
    printf("Wednesday Is Day Number = %d\n", WEDNESDAY);
    printf("Thursday Is Day Number = %d\n", THURSDAY);
    printf("Friday Is Day Number = %d\n", FRIDAY);
    printf("Saturday Is Day Number = %d\n", SATURDAY);

    printf("------------------------------------------------\n");
    printf("One Is Enum Number = %d\n", ONE);
    printf("Two Is Enum Number = %d\n", TWO);
    printf("Three Is Enum Number = %d\n", THREE);
    printf("Four Is Enum Number = %d\n", FOUR);
    printf("Five Is Enum Number = %d\n", FIVE);
    printf("Six Is Enum Number = %d\n", SIX);
    printf("Seven Is Enum Number = %d\n", SEVEN);
    printf("Eight Is Enum Number = %d\n", EIGHT);
    printf("Nine Is Enum Number = %d\n", NINE);
    printf("Ten Is Enum Number = %d\n\n", TEN);

    printf("------------------------------------------------\n");
    printf("January Is Month Number = %d\n", JANUARY);
    printf("February Is Month Number = %d\n", FEBRUARY);
    printf("March Is Month Number = %d\n", MARCH);
    printf("April Is Month Number = %d\n", APRIL);
    printf("May Is Month Number = %d\n", MAY);
    printf("June Is Month Number = %d\n", JUNE);
    printf("July Is Month Number = %d\n", JULY);
    printf("August Is Month Number = %d\n", AUGUST);
    printf("September Is Month Number = %d\n", SEPTEMBER);
    printf("October Is Month Number = %d\n", OCTOBER);
    printf("November Is Month Number = %d\n", NOVEMBER);
    printf("December Is Month Number = %d\n\n", DECEMBER);

    printf("Value of TRUE Is = %d\n", TRUE);
    printf("Value of FALSE Is = %d\n", FALSE);
    printf("------------------------------------------------\n");

    printf("PATIK_PI Macro value is = %.10lf \n ", PRATIK_PI);
    float Area_of_Circle = PRATIK_PI * 2.0f * 2.0f;
    printf("Area of Circle of Raidius 2 units = %f\n", Area_of_Circle);
    printf("------------------------------------------------\n");

    printf(AMC_STRING);

    printf("\n\n AMC String is: %s\n", AMC_STRING);
    printf("------------------------------------------------\n");

    return (0);
}