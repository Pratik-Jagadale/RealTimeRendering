#include <stdio.h>

int main(void)
{
    //variable declartion
    int iArray_PRJ[] = {9, 30, 6, 12, 98, 26, 54, 23, 12};
    int int_size_PRJ;
    int iArray_size_PRJ;
    int iArray_num_elements_PRJ;

    float fArray_PRJ[] = {1.2f, 2.3f, 3.2f, 3.7f, 4.1f, 4.2f, 4.5f, 5.5f, 9.3f};
    int float_size_PRJ;
    int fArray_size_PRJ;
    int fArray_num_elements_PRJ;

    char cArray_PRJ[] = {'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P'};
    int char_size_PRJ;
    int cArray_size_PRJ;
    int cArray_num_elements_PRJ;

    //code

    /* iArray[]  */
    printf("In-Line Initialization And Piece-meal Display Of Elements of Array 'IArray_PRJ[]' : \n\n");

    int_size_PRJ = sizeof(int);
    iArray_size_PRJ = sizeof(iArray_PRJ);
    iArray_num_elements_PRJ = iArray_size_PRJ / int_size_PRJ;

    for (int i = 0; i < iArray_num_elements_PRJ; i++)
    {
        printf("iArray[%d] (element %d) = %d\n", i, (i + 1), iArray_PRJ[0]);
    }

    printf("sizeof Data Type 'int' = %d\n", int_size_PRJ);
    printf("size of iArray[] = %d\n", iArray_size_PRJ);
    printf("number of elemmets of array = %d\n", iArray_num_elements_PRJ);

    /* fArray[] */
    printf("\n\n");

    printf("In-Line Initialization And Piece-meal Display Of Elements of Array 'fArray_PRJ[]' : \n\n");

    float_size_PRJ = sizeof(float);
    fArray_size_PRJ = sizeof(fArray_PRJ);
    fArray_num_elements_PRJ = fArray_size_PRJ / float_size_PRJ;

    for (int i = 0; i < fArray_num_elements_PRJ; i++)
    {
        printf("fArray_PRJ[%d] (element %d)= %f\n", i, (i + 1), fArray_PRJ[i]);
    }

    printf("sizeof Data Type 'float' = %d\n", float_size_PRJ);
    printf("size of fArray[] = %d\n", fArray_size_PRJ);
    printf("number of elemmets of fArray = %d\n", fArray_num_elements_PRJ);

    /* cArray[] */
    printf("\n\n");

    printf("In-Line Initialization And Piece-meal Display Of Elements of Array 'cArray_PRJ[]' : \n\n");
    char_size_PRJ = sizeof(char);
    cArray_size_PRJ = sizeof(cArray_PRJ);
    cArray_num_elements_PRJ = cArray_size_PRJ / char_size_PRJ;

    for (int i = 0; i < cArray_num_elements_PRJ; i++)
    {
        printf("cArray_PRJ[%d] (%d - element) = %c\n", i, (i + 1), cArray_PRJ[i]);
    }
    printf("sizeof Data Type 'char' = %d\n", char_size_PRJ);
    printf("size of cArray[] = %d\n", cArray_size_PRJ);
    printf("number of elemmets of cArray = %d\n", cArray_num_elements_PRJ);

    return (0);
}
