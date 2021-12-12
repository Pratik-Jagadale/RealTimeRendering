#include <stdio.h>

#define MAX_NUMBER(a, b) ((a > b) ? a : b)

int main(void)
{
    //variable declarations
    int iNum_01_PRJ;
    int iNum_02_PRJ;
    int iResult_PRJ;

    float fNum_01_PRJ;
    float fNum_02_PRJ;
    float fResult_PRJ;

    //code
    /** Comparing Integer Values **/
    printf("\n\n");
    printf("Enter an Integer Number : \n\n");
    scanf("%d", &iNum_01_PRJ);

    printf("\n\n");
    printf("Enter an Integer Number : \n\n");
    scanf("%d", &iNum_02_PRJ);

    iResult_PRJ = MAX_NUMBER(iNum_01_PRJ, iNum_02_PRJ);
    printf("\n\n");
    printf("Result of Macro fucntion MAX_NUMBER() = %d", iResult_PRJ);

    /** Comparing Floatig-Point Value **/

    printf("\n\n");
    printf("Enter an Floating Point Number : \n\n");
    scanf("%f", &fNum_01_PRJ);

    printf("\n\n");
    printf("Enter an Flaoting Point Number : \n\n");
    scanf("%f", &fNum_02_PRJ);

    fResult_PRJ = MAX_NUMBER(fNum_01_PRJ, fNum_02_PRJ);
    printf("\n\n");
    printf("Result of Macro fucntion MAX_NUMBER() = %f", fResult_PRJ);

    return (0);
}
