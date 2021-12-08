/*
    Assigment : Escape Sequences
    date: 2021_21_11
*/

/* Header Files */
#include <stdio.h>

/* Entry Point Function */
int main(void)
{
    /* code */
    printf("-----------------------------------------------------------------------------\n");
    printf("\t\tEscape Sequences \n");
    printf("-----------------------------------------------------------------------------\n");

    printf("Going On To Next Line... Using \\n Escape Sequence \n\n");
    printf("\rDemonstrating \t Horizontal \t Tab \t Using \t \\t Esxape Sequence !!!\n\n");
    printf("\" This Is a Double Quoted Outout \" Done Using \\\" \\\" Escape Sequence\n\n");
    printf("\' This Is a Single Quoted Output\' Done Using \\\' \\\' Escape Sequence\n\n");
    printf("BACKSPACE Turned To BACKSPACE \b Using Escape Sequence \\b \n\n");

    printf("-----------------------------------------------------------------------------\n");
    printf("Demonstrating Carriage Return Using \\r Escape Sequece \n");
    printf("Demonstrating \r Carriage Return Using \\r Escape Sequence \n");
    printf("Demonstrating Carriage \r Return Using \\r Escape Sequence\n\n");

    printf("-----------------------------------------------------------------------------\n");
    printf("Demonstrating \x50 "); /*0x80 Is The Hexadecimal code for letter 'P'. 'xhh' Is the place holder For 'x' follwed By 2 digits (hh) , altogether forming aa Hexadecimal Number*/
    printf("\x72 \x61 \x74 \x69 \x6B Using \\xhh Escape Sequence\n\n");

    printf("Demonstrating \102  using \\ooo Escape Sequence\n\n");
    /* /102 Is the Octal code for Letter 'B'.'ooo' Is the place holder for 3 Digits Forming octal Number */
    printf("-----------------------------------------------------------------------------\n");
    return (0);
}
