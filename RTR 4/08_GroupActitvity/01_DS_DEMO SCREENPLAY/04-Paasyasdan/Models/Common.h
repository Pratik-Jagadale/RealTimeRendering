#ifndef _COMMON_H_
#define _COMMON_H_

#include <windows.h>
#include "OGLSCENE1.h"
#include <stdio.h>
#include <stdlib.h>

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

/* Texture Loading Related Function */
BOOL LoadGLPNGTexture(GLuint *texture, char *image);
void colorSetcolor(GLfloat r, GLfloat g, GLfloat b);

#endif