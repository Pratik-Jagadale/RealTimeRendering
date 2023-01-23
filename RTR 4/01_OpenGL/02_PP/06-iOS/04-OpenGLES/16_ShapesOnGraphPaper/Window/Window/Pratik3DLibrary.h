#include <stdlib.h>
#include <stdio.h>

// #define _USE_MATH_DEFINES
#include <math.h>

unsigned int numberOfVertices = 1080;

void getCircleVertexData(float *circlePositionCoords)
{

    // LOCAL VARIABLE DECLARTIONS
    float x = 0;
    float y = 0;
    float radius = 1.0f;

    // CODE
    for (int i = 0, angle = 0; i < 1080, angle < 360; i = i + 3, angle++)
    {
        float rAngle = angle * M_PI / 180.0f;

        x = radius * cos(rAngle);
        y = radius * sin(rAngle);

        circlePositionCoords[i] = x;
        circlePositionCoords[i + 1] = y;
        circlePositionCoords[i + 2] = 0.0f;
    }

    circlePositionCoords[1080 - 3] = circlePositionCoords[0];
    circlePositionCoords[1080 - 2] = circlePositionCoords[1];
    circlePositionCoords[1080 - 1] = circlePositionCoords[2];

}

unsigned int getNumberOfCircleVertices()
{
    return numberOfVertices / 3;
}

void deleteCircleVertexData(float *circlePositionCoords)
{
    free(circlePositionCoords);
    circlePositionCoords = NULL;
}
