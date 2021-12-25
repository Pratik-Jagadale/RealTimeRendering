#include <stdio.h>

// DEFIENING STRUCT
struct MyPoint
{
    int x;
    int y;
};

int main(void)
{
    // Variable Declartion
    struct MyPoint point_A_PRJ, point_B_PRJ, point_C_PRJ, point_D_PRJ, point_E_PRJ;

    //code
    //User Input For The Data Members Of struct Mypoint variable point_A_PRJ'
    printf("Enter X-Coordinate For Point 'A' :");
    scanf("%d", &point_A_PRJ.x);
    printf("Enter Y-Coordinate For Point 'A' :");
    scanf("%d", &point_A_PRJ.y);

    //User Input For The Data Members Of struct Mypoint variable point_B_PRJ'
    printf("Enter X-Coordinate For Point 'B' :");
    scanf("%d", &point_B_PRJ.x);
    printf("Enter Y-Coordinate For Point 'B' :");
    scanf("%d", &point_B_PRJ.y);

    //User Input For The Data Members Of struct Mypoint variable point_C_PRJ'
    printf("Enter X-Coordinate For Point 'C' :");
    scanf("%d", &point_C_PRJ.x);
    printf("Enter Y-Coordinate For Point 'C' :");
    scanf("%d", &point_C_PRJ.y);

    //User Input For The Data Members Of struct Mypoint variable point_D_PRJ'
    printf("Enter X-Coordinate For Point 'D' :");
    scanf("%d", &point_D_PRJ.x);
    printf("Enter Y-Coordinate For Point 'D' :");
    scanf("%d", &point_D_PRJ.y);

    //User Input For The Data Members Of struct Mypoint variable point_E_PRJ'
    printf("Enter X-Coordinate For Point 'E' :");
    scanf("%d", &point_E_PRJ.x);
    printf("Enter Y-Coordinate For Point 'E' :");
    scanf("%d", &point_E_PRJ.y);

    //User Input For The Data Members Of struct Mypoint variable point_A_PRJ'
    printf("\n\n");
    printf("Co-ordinates (x,y) of Point 'A' Are: (%d,%d) \n\n", point_A_PRJ.x, point_A_PRJ.y);
    printf("Co-ordinates (x,y) of Point 'B' Are: (%d,%d) \n\n", point_B_PRJ.x, point_B_PRJ.y);
    printf("Co-ordinates (x,y) of Point 'C' Are: (%d,%d) \n\n", point_C_PRJ.x, point_C_PRJ.y);
    printf("Co-ordinates (x,y) of Point 'D' Are: (%d,%d) \n\n", point_D_PRJ.x, point_D_PRJ.y);
    printf("Co-ordinates (x,y) of Point 'E' Are: (%d,%d) \n\n", point_E_PRJ.x, point_E_PRJ.y);

    return (0);
}
