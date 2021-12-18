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
    printf("iArray[0] (1 st element) = %d\n", iArray_PRJ[0]);
    printf("iArray[1] (2 nd element) = %d\n", iArray_PRJ[1]);
    printf("iArray[2] (3 rd element) = %d\n", iArray_PRJ[2]);
    printf("iArray[3] (4 th element) = %d\n", iArray_PRJ[3]);
    printf("iArray[4] (5 th element) = %d\n", iArray_PRJ[4]);
    printf("iArray[5] (6 th element) = %d\n", iArray_PRJ[5]);
    printf("iArray[6] (7 th element) = %d\n", iArray_PRJ[6]);
    printf("iArray[7] (8 th element) = %d\n", iArray_PRJ[7]);
    printf("iArray[8] (9 th element) = %d\n", iArray_PRJ[8]);

    int_size_PRJ = sizeof(int);
    iArray_size_PRJ = sizeof(iArray_PRJ);
    iArray_num_elements_PRJ = iArray_size_PRJ / int_size_PRJ;
    printf("sizeof Data Type 'int' = %d\n", int_size_PRJ);
    printf("size of iArray[] = %d\n", iArray_size_PRJ);
    printf("number of elemmets of array = %d\n", iArray_num_elements_PRJ);

    /* fArray[] */
    printf("\n\n");

    printf("In-Line Initialization And Piece-meal Display Of Elements of Array 'fArray_PRJ[]' : \n\n");
    printf("fArray_PRJ[0] (1 st element) = %f\n", fArray_PRJ[0]);
    printf("fArray_PRJ[1] (2 nd element) = %f\n", fArray_PRJ[1]);
    printf("fArray_PRJ[2] (3 rd element) = %f\n", fArray_PRJ[2]);
    printf("fArray_PRJ[3] (4 th element) = %f\n", fArray_PRJ[3]);
    printf("fArray_PRJ[4] (5 th element) = %f\n", fArray_PRJ[4]);
    printf("fArray_PRJ[5] (6 th element) = %f\n", fArray_PRJ[5]);
    printf("fArray_PRJ[6] (7 th element) = %f\n", fArray_PRJ[6]);
    printf("fArray_PRJ[7] (8 th element) = %f\n", fArray_PRJ[7]);
    printf("fArray_PRJ[8] (9 th element) = %f\n", fArray_PRJ[8]);

    float_size_PRJ = sizeof(float);
    fArray_size_PRJ = sizeof(fArray_PRJ);
    fArray_num_elements_PRJ = fArray_size_PRJ / float_size_PRJ;
    printf("sizeof Data Type 'float' = %d\n", float_size_PRJ);
    printf("size of fArray[] = %d\n", fArray_size_PRJ);
    printf("number of elemmets of fArray = %d\n", fArray_num_elements_PRJ);

    /* cArray[] */
    printf("\n\n");

    printf("In-Line Initialization And Piece-meal Display Of Elements of Array 'cArray_PRJ[]' : \n\n");
    printf("Array_PRJ[0] (1 st element) = %c\n", cArray_PRJ[0]);
    printf("cArray_PRJ[1] (2 nd element) = %c\n", cArray_PRJ[1]);
    printf("cArray_PRJ[2] (3 rd element) = %c\n", cArray_PRJ[2]);
    printf("cArray_PRJ[3] (4 th element) = %c\n", cArray_PRJ[3]);
    printf("cArray_PRJ[4] (5 th element) = %c\n", cArray_PRJ[4]);
    printf("cArray_PRJ[5] (6 th element) = %c\n", cArray_PRJ[5]);
    printf("cArray_PRJ[6] (7 th element) = %c\n", cArray_PRJ[6]);
    printf("cArray_PRJ[7] (8 th element) = %c\n", cArray_PRJ[7]);
    printf("cArray_PRJ[8] (9 th element) = %c\n", cArray_PRJ[8]);
    printf("cArray_PRJ[9] (10 th element) = %c\n", cArray_PRJ[9]);
    printf("cArray_PRJ[10] (11 th element) = %c\n", cArray_PRJ[10]);
    printf("cArray_PRJ[11] (12 th element) = %c\n", cArray_PRJ[11]);
    printf("cArray_PRJ[12] (13 th element) = %c\n", cArray_PRJ[12]);

    char_size_PRJ = sizeof(char);
    cArray_size_PRJ = sizeof(cArray_PRJ);
    cArray_num_elements_PRJ = cArray_size_PRJ / char_size_PRJ;
    printf("sizeof Data Type 'char' = %d\n", char_size_PRJ);
    printf("size of cArray[] = %d\n", cArray_size_PRJ);
    printf("number of elemmets of cArray = %d\n", cArray_num_elements_PRJ);

    return (0);
}
