#include <stdio.h>

// DEFIENING STRUCT
struct Mypoint
{
    int x;
    int y;
};

struct Mypoint point_A_PRJ, point_B_PRJ, point_C_PRJ, point_D_PRJ, point_E_PRJ; //Declaring 5  Struct Variable of type 'struct Mypoint' globally...

int main(void)
{
    //code
    //Assigning Data Values To The Data Members Of struct Mypoint variable point_A_PRJ
    point_A_PRJ.x = 6;
    point_A_PRJ.y = 3;

    //Assigning Data Values To The Data Members Of struct Mypoint variable point_B_PRJ
    point_B_PRJ.x = 4;
    point_B_PRJ.y = 2;

    //Assigning Data Values To The Data Members Of struct Mypoint variable point_C_PRJ
    point_C_PRJ.x = 8;
    point_C_PRJ.y = 4;

    //Assigning Data Values To The Data Members Of struct Mypoint variable point_D_PRJ
    point_D_PRJ.x = 3;
    point_D_PRJ.y = 5;

    //Assigning Data Values To The Data Members Of struct Mypoint variable point_E_PRJ
    point_E_PRJ.x = 11;
    point_E_PRJ.y = 8;

    //Displaying Values Of the Data Members Of struct MyPoint
    printf("\n\n");
    printf("Co-ordinates (x,y) of Point 'A' Are: (%d,%d) \n\n", point_A_PRJ.x, point_A_PRJ.y);
    printf("Co-ordinates (x,y) of Point 'B' Are: (%d,%d) \n\n", point_B_PRJ.x, point_B_PRJ.y);
    printf("Co-ordinates (x,y) of Point 'C' Are: (%d,%d) \n\n", point_C_PRJ.x, point_C_PRJ.y);
    printf("Co-ordinates (x,y) of Point 'D' Are: (%d,%d) \n\n", point_D_PRJ.x, point_D_PRJ.y);
    printf("Co-ordinates (x,y) of Point 'E' Are: (%d,%d) \n\n", point_E_PRJ.x, point_E_PRJ.y);

    return (0);
}
