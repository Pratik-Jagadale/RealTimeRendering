#include <stdio.h>

struct Rectangle
{
    struct MyPoint
    {
        int x;
        int y;
    } point_01_PRJ, point_02_PRJ;
} rect_PRJ = {{2, 3}, {5, 6}};

// Entry Point Function
int main(void)
{
    // variable declartion
    int length_PRJ, breadth_PRJ, area_PRJ;

    // code

    length_PRJ = rect_PRJ.point_02_PRJ.y - rect_PRJ.point_01_PRJ.y;
    if (length_PRJ < 0)
        length_PRJ = length_PRJ * -1;

    breadth_PRJ = rect_PRJ.point_02_PRJ.x - rect_PRJ.point_01_PRJ.x;
    if (breadth_PRJ < 0)
        breadth_PRJ = breadth_PRJ * -1;

    area_PRJ = length_PRJ * breadth_PRJ;

    printf("\n\n");
    printf("Length of Rectangle = %d\n\n", length_PRJ);
    printf("Breadth of Rectangle = %d\n\n", breadth_PRJ);
    printf("Area Of Rectangle = %d \n\n", area_PRJ);

    return (0);
}
