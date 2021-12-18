#include <stdio.h>

/* Entry Point Function */
int main(void)
{
    //varibale declartions
    int iArrayOne_PRJ[10];
    int iArrayTwo_PRJ[10];

    //code
    /* iArrayOne[]  */
    iArrayOne_PRJ[0] = 3;
    iArrayOne_PRJ[1] = 6;
    iArrayOne_PRJ[2] = 9;
    iArrayOne_PRJ[3] = 12;
    iArrayOne_PRJ[4] = 15;
    iArrayOne_PRJ[5] = 18;
    iArrayOne_PRJ[6] = 21;
    iArrayOne_PRJ[7] = 24;
    iArrayOne_PRJ[8] = 27;
    iArrayOne_PRJ[9] = 30;

    printf("\n\n");
    printf("Piece-meal (hard-coded) Assignment And Display Of Elemnets to Array to ArrayOne_PRJ[]' : \n\n");

    printf("1 St Element of Array 'iArrayOne_PRJ[]' or Element At 0th Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[0]);
    printf("2 nd Element of Array 'iArrayOne_PRJ[]' or Element At 1th Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[1]);
    printf("3 rd Element of Array 'iArrayOne_PRJ[]' or Element At 2 nd Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[2]);
    printf("4 th Element of Array 'iArrayOne_PRJ[]' or Element At 3 rd Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[3]);
    printf("5 th Element of Array 'iArrayOne_PRJ[]' or Element At 4th Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[4]);
    printf("6 th Element of Array 'iArrayOne_PRJ[]' or Element At 5th Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[5]);
    printf("7 th Element of Array 'iArrayOne_PRJ[]' or Element At 6th Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[6]);
    printf("8 th Element of Array 'iArrayOne_PRJ[]' or Element At 7th Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[7]);
    printf("9 th Element of Array 'iArrayOne_PRJ[]' or Element At 8th Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[8]);
    printf("10 th Element of Array 'iArrayOne_PRJ[]' or Element At 9th Index of Array 'iArrayOne_PRJ[]' : %d \n", iArrayOne_PRJ[9]);

    printf("\n\n");

    printf("Enter 1 st Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[0]);
    printf("Enter 2 nd Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[1]);
    printf("Enter 3 rd Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[2]);
    printf("Enter 4 th Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[3]);
    printf("Enter 5 th Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[4]);
    printf("Enter 6 th Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[5]);
    printf("Enter 7 th Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[6]);
    printf("Enter 8  th Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[7]);
    printf("Enter 9 th Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[8]);
    printf("Enter 10 th Element of Array 'iArrayTwo_PRJ[]' : ");
    scanf("%d", &iArrayTwo_PRJ[9]);

    printf("\n\n");
    printf("Piece-meal (User Input) Assignment And Display Of Elemnets to Array to ArrayTwo_PRJ[]' : \n\n");

    printf("1 St Element of Array 'ArrayTwo_PRJ[]' or Element At 0th Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[0]);
    printf("2 nd Element of Array 'ArrayTwo_PRJ[]' or Element At 1th Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[1]);
    printf("3 rd Element of Array 'ArrayTwo_PRJ[]' or Element At 2 nd Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[2]);
    printf("4 th Element of Array 'ArrayTwo_PRJ[]' or Element At 3 rd Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[3]);
    printf("5 th Element of Array 'ArrayTwo_PRJ[]' or Element At 4th Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[4]);
    printf("6 th Element of Array 'ArrayTwo_PRJ[]' or Element At 5th Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[5]);
    printf("7 th Element of Array 'ArrayTwo_PRJ[]' or Element At 6th Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[6]);
    printf("8 th Element of Array 'ArrayTwo_PRJ[]' or Element At 7th Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[7]);
    printf("9 th Element of Array 'ArrayTwo_PRJ[]' or Element At 8th Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[8]);
    printf("10 th Element of Array 'ArrayTwo_PRJ[]' or Element At 9th Index of Array 'ArrayTwo_PRJ[]' : %d \n", iArrayTwo_PRJ[9]);

    return (0);
}