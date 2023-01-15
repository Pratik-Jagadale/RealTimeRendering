#include <stdio.h>
#include <stdlib.h>

#include "vmath.h"
using namespace vmath;

int matrixStackTop;
int MODEL_VIEW_MATRIX_STACK = 32;
mat4 *matrixStack;

void initializeMatrixStack(void)
{
	// code
    matrixStack = (mat4*)malloc(MODEL_VIEW_MATRIX_STACK * sizeof(mat4));
    
	matrixStackTop = 0;
	for (int i = matrixStackTop; i < MODEL_VIEW_MATRIX_STACK; i++)
		matrixStack[i] = mat4::identity();

	
}

void pushMatrix(mat4 matrix)
{
	// Function Prototype
	void uninitialize(void);

	//	Code
	if (matrixStackTop > (MODEL_VIEW_MATRIX_STACK - 1))
	{
		// [self uninitialise];
        // [self release];
        // [NSApp terminate:self];
	}

	matrixStack[matrixStackTop] = matrix;
	matrixStackTop++;

}

mat4 popMatrix(void)
{
	// function ptototype
	void uninitialize(void);

	// variable declartions
	mat4 matrix;

	// code

	if (matrixStackTop < 0)
	 {
	// 	[self uninitialise];
    //     [self release];
    //     [NSApp terminate:self];
	}

	matrixStack[matrixStackTop] = mat4::identity();
	matrixStackTop--;

	matrix = matrixStack[matrixStackTop];

	return (matrix);
}