#include "Common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

extern FILE *gpFile;

BOOL LoadGLPNGTexture(GLuint *texture, char *image)
{
    // variable declartions
    int iWidth, iHeight, iComponents;

    // code
    // hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), ImageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    unsigned char *Data = stbi_load(image, &iWidth, &iHeight, &iComponents, 0);

    if (!Data)
    {

        fprintf(gpFile, "Cubemap tex failed to %s: ", image);
        stbi_image_free(Data);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        glGenTextures(1, texture);

        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // create the texture
        // gluBuild2DMipmaps(GL_TEXTURE_2D, 3, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, Data);

        if (iComponents == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
        else if (iComponents == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);

        glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

        // DELETE Object
        stbi_image_free(Data);
        return TRUE;
    }

    return FALSE;
}

void colorSetcolor(GLfloat r, GLfloat g, GLfloat b)
{
    glColor3f(r / 255, g / 255, b / 255);
}
