/* OpenGL Header files */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

enum
{
    TGA_UNSUPPORTED = 1,
    TGA_NO_IMAGE = 2,
    TGA_MAP = 4,
    TGA_RGB = 8,
    TGA_BW = 16,
    TGA_RLE = 32
};

struct ImageHeader
{
    int width;
    int height;
    char bitsPerPixel;
};

struct TGAHEADER
{
    // Lenght of id string
    char cIdLength;

    // Image Storage Info
    char cColorMapType;
    char cImgaetype;

    // Image Description
    short FirstEntry;
    short NumEntries;
    short BitsPerEntry;

    // Image descrption
    short sXOrigin;
    short sYOrigin;
    short sWidth;
    short sheight;
    char cBitsPer;
    char cDescriptor;
};

GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);
