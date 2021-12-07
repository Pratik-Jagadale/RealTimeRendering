#include <stdio.h>

int main()
{
    /* variable  declartions */
    float a_PRJ = 10.20, b_PRJ = 10.26;
    int p_PRJ = 17, q_PRJ = 17;

    char ch_result_01_PRJ, ch_result_02_PRJ;
    float f_Result_01_PRJ, f_Result_02_PRJ;

    /* code */

    printf("\n----------------------------------------------------------------\n\n");

    ch_result_01_PRJ = (a_PRJ < b_PRJ) ? 'A' : 'B';
    f_Result_01_PRJ = (a_PRJ > b_PRJ) ? a_PRJ : b_PRJ;
    printf("Ternary Operator Answer 1---> %c and %0.2f.\n\n", ch_result_01_PRJ, f_Result_01_PRJ);

    ch_result_02_PRJ = (p_PRJ != q_PRJ) ? 'P' : 'Q';
    f_Result_02_PRJ = (p_PRJ += q_PRJ) ? p_PRJ : q_PRJ;
    printf("Ternary Operator Answer 2---> %c and %0.2f. \n\n", ch_result_02_PRJ, f_Result_02_PRJ);

    printf("\n----------------------------------------------------------------\n");

    return (0);
}