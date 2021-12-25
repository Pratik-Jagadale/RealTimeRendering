#include <stdio.h>

//Defining struct
struct MyPoint
{
    int x;
    int y;
} point_PRJ;

struct MyPointProperties
{
    int quadrant;
    char axis_location[10];
} point_properties; //delcaring a single variable of type struct MyPpintProperties globaly

int main(void)
{
    //code
    //User Input for The data Member Of struct MyPoint Variables point_A_PRJ;
    printf("\n\n");
    printf("Enter X-Coordinate For A Point : ");
    scanf("%d", &point_PRJ.x);

    printf("Enter Y-Coordinate For A Point : ");
    scanf("%d", &point_PRJ.y);

    printf("\n\n");
    printf("Point Co-ordinate (x, y) Are : (%d, %d) !!!\n\n", point_PRJ.x, point_PRJ.y);

    if (point_PRJ.x == 0 && point_PRJ.y == 0)
        printf("The Point Is The Orgin (%d , %d) !!!\n\n", point_PRJ.x, point_PRJ.y);
    else
    {
        if (point_PRJ.x == 0)
        {
            if (point_PRJ.y < 0)
                strcpy(point_properties.axis_location, "Negative y");

            if (point_PRJ.y > 0)
                strcpy(point_properties.axis_location, "Positive y");

            point_properties.quadrant = 0; //A point lying on any of the co=oediante Axes Is Not A part of Any Quadrant
            printf("The Point Lies On The %s Axis !!!\n\n", point_properties.axis_location);
        }
        else if (point_PRJ.y == 0)
        {
            if (point_PRJ.x < 0)
                strcpy(point_properties.axis_location, "Negative -x");

            if (point_PRJ.x > 0)
                strcpy(point_properties.axis_location, "Positive - x");

            point_properties.quadrant = 0; //A point lying on any of the co=oediante Axes Is Not A part of Any Quadrant
            printf("The Point Lies On The %s Axis !!!\n\n", point_properties.axis_location);
        }
        else
        {
            point_properties.axis_location[0] = '\0';

            if (point_PRJ.x > 0 && point_PRJ.y < 0) // 'X' Is +ve And 'Y Is +ve
                point_properties.quadrant = 1;
            else if (point_PRJ.x < 0 && point_PRJ.y > 0) //'X' Is -Ve And 'Y' is +Ve
                point_properties.quadrant = 2;
            else if (point_PRJ.x < 0 && point_PRJ.y < 0) //'X' Is -Ve And 'Y' Is -Ve
                point_properties.quadrant = 3;
            else //'X' Is +Ve  And 'Y' Is -Ve
                point_properties.quadrant = 4;
            printf("The Point Lies In Quadrant Number %d !!!\n\n", point_properties.quadrant);
        }
    }
    return (0);
}
