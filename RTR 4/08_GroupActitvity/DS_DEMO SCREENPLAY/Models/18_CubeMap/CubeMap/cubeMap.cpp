#include "cubeMap.h"

GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
    // Variable declartions
    FILE *pFile;              // File Pointer
    TGAHEADER tgaHeader;      // TGA file header
    unsigned long lImageSize; // size in bytes of image
    GLbyte *pBits = NULL;     // pointer to bits
    short sDepth;

    // code
    *iWidth = 0;
    iHeight = 0;
    *eFormat = GL_BGR_EXT;
    *iComponents = GL_RGB8;

    // open file
    pFile = fopen(szFileName, "rb");
    if (pFile == NULL)
        return NULL;

    // Read in header(binary )
    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

    // Do byte swap for big vs little endian
    //
    //
    // Get width , height , and depth of texture
    *iWidth = tgaHeader.sWidth;
    *iHeight = tgaHeader.sheight;
    sDepth = tgaHeader.cBitsPer / 8;

    // validate Depth bits
    if (tgaHeader.cBitsPer != 8 && tgaHeader.cBitsPer != 24 && tgaHeader.cBitsPer != 32)
        return NULL;

    lImageSize = tgaHeader.sWidth * tgaHeader.sheight * sDepth;

    // Allocate Memory
    pBits = (GLbyte *)malloc(lImageSize * sizeof(GLbyte));

    if (pBits == NULL)
    {
        free(pBits);
        return NULL;
    }

    // Set OpenGL format expected
    switch (sDepth)
    {
    case 3: // Most likely case
        *eFormat = GL_BGR_EXT;
        *iComponents = GL_RGB8;
        break;
    case 4:
        *eFormat = GL_BGRA_EXT;
        *iComponents = GL_RGBA8;
        break;
    case 1:
        *eFormat = GL_LUMINANCE;
        *iComponents = GL_LUMINANCE8;
        break;
    };

    // close file
    fclose(pFile);

    return (pBits);
}
