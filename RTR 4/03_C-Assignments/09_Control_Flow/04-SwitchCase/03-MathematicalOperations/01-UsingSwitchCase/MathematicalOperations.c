/* Header files */
#include <stdio.h>
#include <conio.h>

/* Entry point functions */
int main(void)
{
    /* variable declartions */
    int a_PRJ, b_PRJ;
    int result_PRJ;

    char option, option_divison;

    /*code*/
    printf("\n\nEnter Value for 'A' : ");
    scanf("%d", &a_PRJ);

    printf("\n\nEnter value for 'B' : ");
    scanf("%d", &b_PRJ);

    printf("\n\nEnter option in character : \n\n");
    printf("'A' or 'a'for addition \n");
    printf("'S' or 's'for Substraction \n");
    printf("'M' or 'm'for Multiplication \n");
    printf("'D' or 'd'for Division \n");
    printf("Enter options : ");
    option = getch();

    printf("\n\n");

    switch (option)
    {
    //FALL THROUGH CONDITION
    case 'A':
    case 'a':
        result_PRJ = a_PRJ + b_PRJ;
        printf("Addition of A = %d And B = %d gives result = %d !!!\n\n", a_PRJ, b_PRJ, result_PRJ);
        break;

        /*Fall Through Consition for 'S' and 's'*/
    case 'S':
    case 's':
        if (a_PRJ >= b_PRJ)
        {
            result_PRJ = a_PRJ - b_PRJ;
            printf("Substraction of A =%d And B = %d gives result = %d !!!\n\n", a_PRJ, b_PRJ, result_PRJ);
        }
        else
        {
            result_PRJ = b_PRJ - a_PRJ;
            printf("Substraction of B =%d And A = %d gives result = %d !!!\n\n", b_PRJ, a_PRJ, result_PRJ);
        }
        break;
    /*Fall Through Consition for 'M' and 'm'*/
    case 'M':
    case 'm':
        result_PRJ = a_PRJ * b_PRJ;
        printf("Multiplication of A = %d And B = %d gives result = %d !!!\n\n", a_PRJ, b_PRJ, result_PRJ);
        break;
    /*Fall Through Consition for 'D' and 'd'*/
    case 'D':
    case 'd':
        printf("Enter Option in  character:\n");
        printf(" 'Q' or 'q' or '/' For Quotient upon Division:\n");
        printf("'R' or 'r' or '%%' for Remainder Upon Division :\n");
        printf("Ennter Option: ");
        option_divison = getch();

        printf("\n\n");
        /* Nested Switch case */
        switch (option_divison)
        {
        /*Fall Through Consition for 'Q' and 'q' and '/' */
        case 'Q':
        case 'q':
        case '/':
            if (a_PRJ >= b_PRJ)
            {
                result_PRJ = a_PRJ / b_PRJ;
                printf("Division of A = %d And B = %d gives Quotient = %d !!!\n\n", a_PRJ, b_PRJ, result_PRJ);
            }
            else
            {
                result_PRJ = b_PRJ / a_PRJ;
                printf("Division of B = %d And A = %d gives Quotient = %d !!!\n\n", b_PRJ, a_PRJ, result_PRJ);
            }
            break;
        /*Fall Through Consition for 'R' and 'r' and  '%' */
        case 'R':
        case 'r':
        case '%':
            if (a_PRJ >= b_PRJ)
            {
                result_PRJ = a_PRJ % b_PRJ;
                printf("Division of A = %d And B = %d gives Remainder = %d !!!\n\n", a_PRJ, b_PRJ, result_PRJ);
            }
            else
            {
                result_PRJ = b_PRJ / a_PRJ;
                printf("Division of B = %d And A = %d gives Remainder = %d !!!\n\n", b_PRJ, a_PRJ, result_PRJ);
            }
            break;

        default:
            printf("Invalid character '%c' Please try again... \n\n", option_divison);
            break;
        }
        break; //break for case D and d.
    default:
        printf("Invalid character '%c' Please try again... \n\n", option);
        break;
    }

    printf("!!!------------------------------------------------------------------!!!\n");

    return (0);
}
