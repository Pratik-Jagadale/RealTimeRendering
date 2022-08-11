/* Header Files */
#include <windows.h>
#include "OGLSCENE1.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "helper_timer.h"

#include "Common.h"

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity
/*
typedef unsigned char stbi_uc;
typedef unsigned short stbi_us;

void stbi_image_free(void *retval_from_stbi_load);
stbi_uc *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);
*/

/* Global Function Declartion */
BOOL LoadGLTexture(GLuint *texture, TCHAR ImageResourceID[]);
void ToggleFullScreen();
void fadeOutMatrixgroup(void);
void fadeOutPayasdan(void);
void drawPNGPayasdan(void);

// global variable declarations
extern BOOL bScene01Done;
extern HWND ghwnd;
extern FILE *gpFile; // FILE* -> #include<stdio.h>
extern HDC ghdc;

GLfloat gfLightAmbiant[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfLightDeffuse[] = {1.0f, 1.0f, 1.0f, 0.0f};
GLfloat gflightSpecular[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat gfLightPositions[] = {0.0f, 0.0f, -4.0f, 1.0f};
GLfloat gfLightSpotLightDirection[] = {0.0f, 0.0f, -13.0f};

GLfloat materialAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialDefuse[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialSpecular[] = {1.0f, 0.f, 0.0f, 1.0f};
GLfloat materialShininess = 120.0f;

BOOL gbLight = FALSE;
GLUquadric *quadric_S1 = NULL;

// Light Atributre for saturn

GLfloat gfLightAmbiantThree[] = {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat gfLightDeffuseThree[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfLightSpicularThree[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfLightPositionsThree[] = {105.0f, 30.0f, 105.0f, 1.0f};
GLfloat gfMaterialAmbiantThree[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat gfMeterialDeffuseThree[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfMateralSpecularThree[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat gfMaterialShineenessThree = 128.0f;

// Variables

GLuint texture_letters_White_Gold_Design;
GLuint texture_letters_Yello_Gold_Design;
GLuint texture_letters_Matrix;
GLuint texture_temp = 0;

GLuint texture_GL;
GLuint texture_group_members;
GLuint texture_refrences;
GLuint texture_Sir;
GLuint texture_sountrack;
GLuint texture_special_thanks;
GLuint texture_Technology;
GLuint texture_yogeshwar_sir;

GLUquadric *quadric_S1Disk = NULL;

GLuint texture_saturn;
GLuint texture_stars;
GLuint texture_saturn_ring;
GLuint texture_letters_Payasdan;
GLfloat angleSaturn = 0.0f;
GLfloat angleSky = 0.0f;

GLfloat tempCounterForAnimation = 0.0f;

static unsigned int sceneNumber_One = 1; // when program begins we are showing the first texture by default hence '1'

enum OPENINGSCENE
{
    SCENE_01_DURATION_S1 = 15, // OPENINGSCENE_SATURN
    SCENE_02_DURATION_S1 = 10, // OPENINGSCENE_ASTROMEDICOMP
    SCENE_03_DURATION_S1 = 10, // OPENINGSCENE_MATRIXGROUP
    SCENE_04_DURATION_S1 = 10  // OPENINGSCENE_PAYASDAN
};

StopWatchInterface *pTimer_SceneOne;

BOOL resetFadeIn = TRUE;
BOOL resetFadeOut = FALSE;
BOOL isFadeOutBool = FALSE;
float elapsedTime_S1;

void fadeIn(void);
void fadeOut(void);

GLfloat CameraVector_S1[3] = {0.0, -1.76f, 0.3f};
GLfloat EyeVector_S1[3] = {0.0f, -1.9f, 0.0f};
GLfloat UpVector_S1[3] = {0.0f, 1.0f, 0.0f};

const GLfloat angle1[] = {10.0f};
const GLfloat angle2[] = {3.0f};

BOOL initializeSceneOne(void)
{

    // Ligth related
    /*
        glLightfv(GL_LIGHT1, GL_AMBIENT, gfLightAmbiant);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, gfLightDeffuse);
        glLightfv(GL_LIGHT1, GL_POSITION, gfLightPositions);
        glLightfv(GL_LIGHT1, GL_SPECULAR, gflightSpecular);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, gfLightSpotLightDirection);
        glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, angle1);
        glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, angle2);
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5f);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);

        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDefuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);
    */
    // Ligth related Changes

    // glEnable(GL_LIGHT1);

    // texture

    if (LoadGLPNGTexture(&texture_letters_White_Gold_Design, "./Models/images/texture_letters_White_Gold_Design.png") == FALSE)
    {
        fprintf(gpFile, "texture_letters_White_Gold_Design Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_letters_Yello_Gold_Design, "./Models/images/texture_letters_Yello_Gold_Design.png") == FALSE)
    {
        fprintf(gpFile, "texture_letters_Yello_Gold_Design Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_letters_Matrix, "./Models/images/texture_letters_matrix.png") == FALSE)
    {
        fprintf(gpFile, "texture_letters_Matrix Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLTexture(&texture_saturn, MAKEINTRESOURCE(IDBITMAP_SATURN_TEXTRURE)) == FALSE)
    {
        fprintf(gpFile, "texture_saturn Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE; // write log in wndproc
    }
    if (LoadGLTexture(&texture_stars, MAKEINTRESOURCE(IDBITMAP_STARS_TEXTRURE1)) == FALSE)
    {
        fprintf(gpFile, "texture_stars Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture(&texture_saturn_ring, MAKEINTRESOURCE(IDBITMAP_SATURN_TEXTURE)) == FALSE)
    {
        fprintf(gpFile, "texture_saturn_ring Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE; // write log in wndproc
    }

    if (LoadGLPNGTexture(&texture_letters_Payasdan, "./Models/images/texture_letters_Payasdan.png") == FALSE)
    {
        fprintf(gpFile, "texture_letters_Payasdan Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_GL, "./Models/Credits/texture_GL.png") == FALSE)
    {
        fprintf(gpFile, "texture_GL Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_group_members, "./Models/Credits/texture_group_memembers.png") == FALSE)
    {
        fprintf(gpFile, "texture_group_members Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_refrences, "./Models/Credits/texture_refrences.png") == FALSE)
    {
        fprintf(gpFile, "texture_refrences Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_Sir, "./Models/Credits/texture_Sir.png") == FALSE)
    {
        fprintf(gpFile, "texture_Sir Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_sountrack, "./Models/Credits/texture_soundtrack.png") == FALSE)
    {
        fprintf(gpFile, "texture_sountrack Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_special_thanks, "./Models/Credits/texture_special_thanks.png") == FALSE)
    {
        fprintf(gpFile, "texture_special_thanks Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_Technology, "./Models/Credits/texture_Technology.png") == FALSE)
    {
        fprintf(gpFile, "texture_Technology Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    if (LoadGLPNGTexture(&texture_yogeshwar_sir, "./Models/Credits/texture_yogeshwar_sir.png") == FALSE)
    {
        fprintf(gpFile, "texture_yogeshwar_sir Failed.\n");
        uninitializeSceneOneSceneOne();
        return FALSE;
    }

    glEnable(GL_TEXTURE_2D);

    // Creating the timer
    sdkCreateTimer(&pTimer_SceneOne);

    // Start them music here
    sdkStartTimer(&pTimer_SceneOne);

    // quadric_S1 intialliza
    // create quadric_S1
    quadric_S1 = gluNewQuadric();

    quadric_S1Disk = gluNewQuadric();

    return TRUE;
}

void displaySceneOne(void)
{
    // variable

    ///////////light realted intilization
    glLightfv(GL_LIGHT1, GL_AMBIENT, gfLightAmbiant);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, gfLightDeffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, gfLightPositions);
    glLightfv(GL_LIGHT1, GL_SPECULAR, gflightSpecular);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, gfLightSpotLightDirection);
    glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, angle1);
    glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, angle2);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDefuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);
    //////////////////////

    static BOOL bScene01Done_S1 = FALSE;
    static BOOL bScene02Done_S1 = FALSE;
    static BOOL bScene03Done_S1 = FALSE;
    static BOOL bScene04Done_S1 = FALSE;
    static BOOL bScene05Done_S1 = FALSE;

    // function declartioins
    void OpeningScene(void);
    void drawAstroMediComp(void);
    void drawMatrixGroup(void);
    void drawPaysadan(void);
    void drawApostrophe(void);
    void drawSaturn(void);
    void drawStars(void);
    void fadeIn(void);
    void fadeOut(void);

    /* Code */
    //    glLightfv(GL_FRONT, GL_POSITION, gfLightPositions);

    elapsedTime_S1 = sdkGetTimerValue(&pTimer_SceneOne); // THIS FUNCTION GIVES TIME IN MILLLIESECONDS
    elapsedTime_S1 = elapsedTime_S1 / 1000.0f;           // CONVERTIN TIME MILLISECONDS TO SECONDS
    TCHAR str[100];
    // swprintf_s(str, 100, TEXT("TIME = %4.2f ,CameraVector_S1[2] = %f ,CameraVector_S1[1] = %f , EyeVector_S1[1] = %f \n"), elapsedTime_S1, CameraVector_S1[2], CameraVector_S1[1], EyeVector_S1[1]); // "_s" for secure

    // SetWindowText(ghwnd, str);

    if (bScene01Done_S1 == FALSE)
    {

        if (elapsedTime_S1 > SCENE_01_DURATION_S1)
        {
            sceneNumber_One++;
            sdkResetTimer(&pTimer_SceneOne);
            resetFadeIn = TRUE;
            bScene01Done_S1 = FALSE;
        }
    }
    else if (bScene02Done_S1 == FALSE)
    {

        if (elapsedTime_S1 > SCENE_02_DURATION_S1)
        {
            sceneNumber_One++;
            sdkResetTimer(&pTimer_SceneOne);
            bScene02Done_S1 = FALSE;
            resetFadeIn = TRUE;
        }
    }
    else if (bScene03Done_S1 == FALSE)
    {
        if (elapsedTime_S1 > SCENE_03_DURATION_S1)
        {
            sceneNumber_One++;
            sdkResetTimer(&pTimer_SceneOne);
            bScene03Done_S1 = FALSE;
            // resetFadeIn = TRUE;
        }
    }
    else if (bScene04Done_S1 == FALSE)
    {

        if (elapsedTime_S1 > SCENE_04_DURATION_S1)
        {
            sceneNumber_One++;
            sdkResetTimer(&pTimer_SceneOne);
            bScene04Done_S1 = FALSE;
            resetFadeIn = TRUE;
        }
    }

    // Actual Scene
    /*gluLookAt(radius * cos(angle), yEyeVector_S1, radius * sin(angle),
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
*/

    gluLookAt(CameraVector_S1[0], CameraVector_S1[1], CameraVector_S1[2],
              EyeVector_S1[0], EyeVector_S1[1], EyeVector_S1[2],
              0.0f, 1.0f, 0.0f);

    {

        if (sceneNumber_One == 1)
        {
            {
                // Moon & Stars
                glPushMatrix();
                glScalef(30.0f, 30.0f, 30.0f);
                // drawStars();

                // Saturn
                glPopMatrix();
                glPushMatrix();

                glTranslatef(0.0f, -4.5f, -5.0f);
                drawSaturn();

                glPopMatrix();
            }

            // glScalef(0.75f, 0.75f, 0.75f);
            glTranslatef(0.0f, 3.0f, -9.0f);
            drawAstroMediComp();
            glPopMatrix();
        }
        else if (sceneNumber_One == 2)
        {

            {
                // Moon & Stars
                glPushMatrix();
                glScalef(30.0f, 30.0f, 30.0f);
                // drawStars();

                // Saturn
                glPopMatrix();
                glPushMatrix();

                glTranslatef(0.0f, -4.5f, -5.0f);
                drawSaturn();

                glPopMatrix();
            }

            glPushMatrix();

            // glScalef(0.75f, 0.75f, 0.75f);
            glTranslatef(0.0f, 3.0f, -9.0f);
            drawAstroMediComp();

            glPopMatrix();
        }
        else if (sceneNumber_One == 3)
        {
            {
                // Moon & Stars
                glPushMatrix();
                glScalef(30.0f, 30.0f, 30.0f);
                drawStars();

                // Saturn
                glPopMatrix();
                glPushMatrix();

                glTranslatef(0.0f, -4.5f, -5.0f);
                drawSaturn();

                glPopMatrix();
            }

            glPushMatrix();

            glTranslatef(0.0f, 2.5f, -9.0f);
            drawMatrixGroup();

            glPopMatrix();
        }
        else if (sceneNumber_One == 4)
        {
            {
                // Moon & Stars
                glPushMatrix();
                glScalef(30.0f, 30.0f, 30.0f);
                drawStars();

                // Saturn
                glPopMatrix();
                // glPushMatrix();

                /* glTranslatef(0.0f, -4.5f, -5.0f);
                 drawSaturn();*/

                // glPopMatrix();
            }

            glPushMatrix();
            glTranslatef(0.0f, -1.5f, -9.0f);
            drawPNGPayasdan();
            glPopMatrix();
        }
        else
        {
            //  Going to Another Scene
            bScene01Done = FALSE;
        }
    }
    ////////////////////////////////////

    glPushMatrix();
    if (sceneNumber_One == 1)
        glTranslatef(CameraVector_S1[0], CameraVector_S1[1], CameraVector_S1[2] - 0.1f);
    else
        // glTranslatef(0.0f, 1.75f, 14.0f);
        glTranslatef(CameraVector_S1[0], CameraVector_S1[1], CameraVector_S1[2] - 0.5f);

    glScalef(100.0f, 100.0f, 100.0f);

    if (sceneNumber_One != 2)
        fadeIn();

    if (sceneNumber_One == 2 && elapsedTime_S1 > 8)
        fadeOut();

    if (sceneNumber_One == 3 && elapsedTime_S1 > 8)
        fadeOutMatrixgroup();

    if (sceneNumber_One == 4 && elapsedTime_S1 > 8)
        fadeOutPayasdan();

    glPopMatrix();
}

void updateSceneOne(void)
{
    /* code */

    if (sceneNumber_One == 1)
    {
        if (elapsedTime_S1 > 4.0f)
        {
            if (CameraVector_S1[2] < 15.4f)
                CameraVector_S1[2] = CameraVector_S1[2] + 0.012f;

            //   if (CameraVector_S1[1] < 1.0f && CameraVector_S1[2] > 7.0f)
            //     CameraVector_S1[1] = CameraVector_S1[1] + 0.00001f;

            // if (EyeVector_S1[1] < 0.04f)
            //    EyeVector_S1[1] = EyeVector_S1[1] + 0.0000000001f;
        }
    }
    else if (sceneNumber_One == 2)
    {
        if (elapsedTime_S1 < 5.0f)
        {
            if (CameraVector_S1[2] < 15.4f)
            {
                CameraVector_S1[2] = CameraVector_S1[2] + 0.012f;
            }

            if (CameraVector_S1[1] < 1.0f && CameraVector_S1[2] > 7.0f)
                CameraVector_S1[1] = CameraVector_S1[1] + 0.00001f;

            if (EyeVector_S1[1] < 0.04f)
                EyeVector_S1[1] = EyeVector_S1[1] + 0.00000001f;
        }
    }

    angleSaturn = angleSaturn + 0.02f;
    if (angleSaturn > 360.0f)
        angleSaturn = 0.0f;

    angleSky = angleSky + 0.002f;
    if (angleSky > 360.0f)
        angleSky = 0.0f;
}

void uninitializeSceneOneSceneOne(void)
{
    /* code */

    /*  */
    if (texture_GL)
    {
        glDeleteTextures(1, &texture_GL);
        texture_GL = 0;
    }

    if (texture_group_members)
    {
        glDeleteTextures(1, &texture_group_members);
        texture_group_members = 0;
    }

    if (texture_refrences)
    {
        glDeleteTextures(1, &texture_refrences);
        texture_refrences = 0;
    }

    if (texture_Sir)
    {
        glDeleteTextures(1, &texture_Sir);
        texture_Sir = 0;
    }

    if (texture_Technology)
    {
        glDeleteTextures(1, &texture_Technology);
        texture_Technology = 0;
    }

    if (texture_yogeshwar_sir)
    {
        glDeleteTextures(1, &texture_yogeshwar_sir);
        texture_yogeshwar_sir = 0;
    }

    if (texture_sountrack)
    {
        glDeleteTextures(1, &texture_sountrack);
        texture_sountrack = 0;
    }

    if (quadric_S1)
    {
        gluDeleteQuadric(quadric_S1);
        quadric_S1 = NULL;
    }

    if (quadric_S1Disk)
    {
        gluDeleteQuadric(quadric_S1Disk);
        quadric_S1Disk = NULL;
    }

    if (texture_letters_Payasdan)
    {
        glDeleteTextures(1, &texture_letters_Payasdan);
        texture_letters_Payasdan = 0;
    }

    if (texture_saturn_ring)
    {
        glDeleteTextures(1, &texture_saturn_ring);
        texture_saturn_ring = 0;
    }

    if (texture_saturn)
    {
        glDeleteTextures(1, &texture_saturn);
        texture_saturn = 0;
    }

    if (texture_stars)
    {
        glDeleteTextures(1, &texture_stars);
        texture_stars = 0;
    }

    if (texture_letters_White_Gold_Design)
    {
        glDeleteTextures(1, &texture_letters_White_Gold_Design);
        texture_letters_White_Gold_Design = 0;
    }

    if (texture_letters_Yello_Gold_Design)
    {
        glDeleteTextures(1, &texture_letters_Yello_Gold_Design);
        texture_letters_Yello_Gold_Design = 0;
    }

    if (texture_letters_Matrix)
    {
        glDeleteTextures(1, &texture_letters_Matrix);
        texture_letters_Matrix = 0;
    }
}

void resetLight(void)
{
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);

    glPushMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
}

BOOL LoadGLTexture(GLuint *texture, TCHAR ImageResourceID[])
{
    // variable declartions
    HBITMAP hBitmap = NULL;
    BITMAP bmp;
    BOOL bResult = FALSE;

    // code
    hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), ImageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    if (hBitmap)
    {
        bResult = TRUE;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        glGenTextures(1, texture);

        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // create the texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

        glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

        // DELETE Object
        DeleteObject(hBitmap);
    }
    return bResult;
}

void OpeningScene(void)
{
    /*// function declartioins
    void drawAstroMediComp(void);
    void drawMatrixGroup(void);
    void drawPaysadan(void);
    void drawApostrophe(void);
    void drawSaturn(void);
    void drawStars(void);
    void fadeIn(void);
    void fadeOut(void);

    // code

    // Background Rendering
    glPushMatrix();
    fadeIn();
    glScalef(30.0f, 30.0f, 30.0f);
    drawStars();

    // Saturn
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, -4.5f, -5.0f);
    drawSaturn();

    glPopMatrix();
    glPushMatrix();

    switch (sceneOfOpeningCredit)
    {
    case OPENINGSCENE_OPEN:

        break;

    case OPENINGSCENE_SATURN:
        break;

    case OPENINGSCENE_ASTROMEDICOMP:
        glPushMatrix();
        fadeIn();

        // glScalef(0.75f, 0.75f, 0.75f);
        glTranslatef(1.0f, 3.0f, -9.0f);
        drawAstroMediComp();

        fadeOut();
        glPopMatrix();
        break;

    case OPENINGSCENE_MATRIXGROUP:
        glPushMatrix();
        fadeIn();

        glTranslatef(1.0f, 3.0f, -9.0f);
        drawMatrixGroup();

        fadeOut();
        glPopMatrix();
        break;

    case OPENINGSCENE_PAYASDAN:
        glPushMatrix();
        fadeIn();

        glTranslatef(1.0f, 3.0f, -6.0f);
        drawPaysadan();

        fadeOut();
        glPopMatrix();
        break;

    default:
        break;
    }

    glPopMatrix();*/
}

void fadeIn(void)
{
    static float alpha = 1.0f;

    if (resetFadeIn == TRUE)
    {
        alpha = 1.0f;
        resetFadeIn = FALSE;
    }
    else if (sceneNumber_One == 1)
        alpha = alpha - 0.002f;
    else
        alpha = alpha - 0.003f;

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    glBegin(GL_QUADS);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void fadeOut(void)
{
    static float alpha = 0.0f;

    if (resetFadeOut == TRUE)
    {
        alpha = 0.0f;
        resetFadeOut = FALSE;
    }
    else
        alpha = alpha + 0.003f;

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    glBegin(GL_QUADS);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void fadeOutMatrixgroup(void)
{
    static float alpha = 0.0f;

    if (resetFadeOut == TRUE)
    {
        alpha = 0.0f;
        resetFadeOut = FALSE;
    }
    else
        alpha = alpha + 0.003f;

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    glBegin(GL_QUADS);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void fadeOutPayasdan(void)
{
    static float alpha = 0.0f;

    if (resetFadeOut == TRUE)
    {
        alpha = 0.0f;
        resetFadeOut = FALSE;
    }
    else
        alpha = alpha + 0.003f;

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    glBegin(GL_QUADS);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void drawPaysadan(void)
{
    // Function declartions
    void drawA(void);
    void drawS(void);
    void drawT(void);
    void drawR(void);
    void drawF(void);
    void drawM(void);
    void drawE(void);
    void drawP(void);
    void drawD(void);
    void drawI(void);
    void drawO(void);
    void drawX(void);
    void drawU(void);
    void drawN(void);
    void drawG(void);
    void drawY(void);
    void drawC(void);
    void drawApostrophe(void);

    GLfloat strtPoint = -7.5f;

    // Code
    texture_temp = texture_letters_Yello_Gold_Design;

    glPushMatrix();

    glTranslatef(strtPoint, 0.0f, 0.0f);
    drawP();
    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 2.2, 0.0f, 0.0f);
    drawA();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 4.2, 0.0f, 0.0f);
    drawY();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 6.2, 0.0f, 0.0f);
    drawA();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 8.2, 0.0f, 0.0f);
    drawS();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 10.1, 0.0f, 0.0f);
    drawD();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 12.3, 0.0f, 0.0f);
    drawA();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 14.7, 0.0f, 0.0f);
    drawN();

    glPopMatrix();
}

void drawMatrixGroup(void)
{
    // Function declartions
    void drawA(void);
    void drawS(void);
    void drawT(void);
    void drawR(void);
    void drawF(void);
    void drawM(void);
    void drawE(void);
    void drawP(void);
    void drawD(void);
    void drawI(void);
    void drawO(void);
    void drawX(void);
    void drawU(void);
    void drawN(void);
    void drawG(void);
    void drawY(void);
    void drawC(void);
    void drawApostrophe(void);

    GLfloat strtPoint = -10.5f;
    GLfloat yOfUpperLine = 2.0f;
    GLfloat yOfLowerLine = -2.0f;

    // Code
    glPushMatrix();
    ///////////////////////////////////////////////////////////////////////////////////////////////

    glPushMatrix();

    // -------------------------------------------------------------------------------------------

    glTranslatef(strtPoint, yOfUpperLine, 0.0f);
    texture_temp = texture_letters_Matrix;
    drawM();
    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 2.4, yOfUpperLine, 0.0f);
    drawA();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 4.2, yOfUpperLine, 0.0f);
    drawT();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 6.2, yOfUpperLine, 0.0f);
    drawR();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 8.1, yOfUpperLine, 0.0f);
    drawI();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 10.2, yOfUpperLine, 0.0f);
    drawX();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 13.0, yOfUpperLine, 0.0f);
    drawG();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 15.0, yOfUpperLine, 0.0f);
    drawR();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 17.0, yOfUpperLine, 0.0f);
    drawO();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 19.0, yOfUpperLine, 0.0f);
    drawU();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 21.0, yOfUpperLine, 0.0f);
    drawP();

    glPopMatrix();
    glScalef(0.75, 0.75, 0.75);
    glPushMatrix();

    glTranslatef(strtPoint + 4.0, yOfLowerLine, 0.0f);
    drawP();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 6.0, yOfLowerLine, 0.0f);
    drawR();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 8.0, yOfLowerLine, 0.0f);
    drawE();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 10.0, yOfLowerLine, 0.0f);
    drawS();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 12.0, yOfLowerLine, 0.0f);
    drawE();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 14.0, yOfLowerLine, 0.0f);
    drawN();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 16.0, yOfLowerLine, 0.0f);
    drawT();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 18.0, yOfLowerLine, 0.0f);
    drawS();

    glPopMatrix();

    ///////////////////////////////////////////////////////////////////////
    glPopMatrix();
    texture_temp = 0;
}

void drawAstroMediComp(void)
{
    // Function declartions
    void drawA(void);
    void drawS(void);
    void drawT(void);
    void drawR(void);
    void drawF(void);
    void drawM(void);
    void drawE(void);
    void drawP(void);
    void drawD(void);
    void drawI(void);
    void drawO(void);
    void drawX(void);
    void drawU(void);
    void drawN(void);
    void drawG(void);
    void drawY(void);
    void drawC(void);
    void drawApostrophe(void);

    GLfloat strtPoint = -13.0f;

    // Code
    texture_temp = texture_letters_White_Gold_Design;
    glPushMatrix();

    glTranslatef(strtPoint, 0.0f, 0.0f);
    drawA();
    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 2.2, 0.0f, 0.0f);
    drawS();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 4.2, 0.0f, 0.0f);
    drawT();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 6.2, 0.0f, 0.0f);
    drawR();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 8.1, 0.0f, 0.0f);
    drawO();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 10.2, 0.0f, 0.0f);
    drawM();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 12.2, 0.0f, 0.0f);
    drawE();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 14.0, 0.0f, 0.0f);
    drawD();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 16.0, 0.0f, 0.0f);
    drawI();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 17.8, 0.0f, 0.0f);
    drawC();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 19.6, 0.0f, 0.0f);
    drawO();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 21.6, 0.0f, 0.0f);
    drawM();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 23.8, 0.0f, 0.0f);
    drawP();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 24.9, 0.0f, 0.0f);
    drawApostrophe();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(strtPoint + 26.0, 0.0f, 0.0f);
    drawS();

    glPopMatrix();
    texture_temp = 0;
}

void drawApostrophe(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    glPushMatrix();

    glTranslatef(0.0, 1.0, 0.0);
    DrawBox(0.5f, 0.5f, 0.3f, 0.3f, 0.5f, 0.5f);

    glPopMatrix();
}
void drawG(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();
    glTranslatef(0.0f, 0.75f, 0.011f);
    DrawBox(0.5f, 0.5f, 1.0f, 1.5f, 0.5f, 0.5f);
    // glTranslatef(-0.0f, 0.75f, 0.0f);
    // DrawBox(0.5f, 0.5f, 0.7f, 0.7f, 0.5f, 0.5f);

    // Right vertical
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.5f, -0.2f, -0.011f);
    DrawBox(0.7f, 0.7f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Right Horizont
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.4f, 0.0f, 0.01f);
    // glRotatef(90.0, 0, 0, 1);
    DrawBox(0.5f, 0.5f, 0.7f, 0.7f, 0.5f, 0.5f);

    // left Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, 0.012f);
    DrawBox(1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Bottom Horizontal
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.0f, -0.75, -0.012f);
    DrawBox(0.5f, 0.5f, 0.7f, 0.7f, 0.5f, 0.5f);

    // Bottom Right Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.43f, -0.71f, -0.013f);
    glRotatef(70.0, 0, 0, 1);
    DrawBox(0.0f, 0.45f, 0.5f, 0.60f, 0.5f, 0.5f);

    // Top left Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.44f, 0.71f, -0.01112f);
    glRotatef(69.0, 0, 0, 1);
    // DrawBox(0.45f, 0.0f, 0.65f, 0.5f, 0.5f, 0.5f);

    // Bottom left Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.44f, -0.70f, 0.0118f);
    glRotatef(-72.0, 0, 0, 1);
    DrawBox(0.45f, 0.0f, 0.65f, 0.65f, 0.5f, 0.5f);

    glPopMatrix();
}
void drawY(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    glPushMatrix();

    glTranslatef(0.4, 0.6, 0.0);
    glRotatef(-35.0, 0.0, 0.0, 1.0);
    DrawBox(1.0f, 1.5f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.4, 0.6, 0.0058);
    glRotatef(35.0, 0.0, 0.0, 1.0);
    DrawBox(1.5f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // middle Vertical
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0, -0.2, 0.0036);
    DrawBox(1.5f, 1.5f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawX(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    glPushMatrix();
    glTranslatef(0.0, 0.1, 0.0);

    // top Horizontal
    glPushMatrix();

    glRotatef(-35.0, 0.0, 0.0, 1.056);
    DrawBox(2.0f, 2.5f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
    glPushMatrix();

    glRotatef(35.0, 0.0, 0.0, 1.00856);
    DrawBox(2.5f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
    glPopMatrix();
}

void drawN(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // left Side
    glPushMatrix();
    glTranslatef(-0.7f, 0.0f, 0.01658f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // middle Left
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.012f);
    glRotatef(39.0f, 0.0f, 0.0f, 1.0f);

    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Right Side
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.7f, 0.0f, -0.01f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawU(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    // Right vertical
    glTranslatef(0.5f, 0.3f, 0.0f);
    DrawBox(1.6f, 1.6f, 0.5f, 0.5f, 0.5f, 0.5f);

    // left Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5f, 0.3f, 0.015f);
    DrawBox(1.6f, 1.6f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Bottom Horizontal
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.0f, -0.75, -0.015f);
    DrawBox(0.5f, 0.5f, 0.7f, 0.7f, 0.5f, 0.5f);

    // Bottom Right Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.43f, -0.71f, 0.005f);
    glRotatef(70.0, 0, 0, 1);
    DrawBox(0.0f, 0.45f, 0.5f, 0.60f, 0.5f, 0.5f);

    // Bottom left Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.44f, -0.70f, -0.001f);
    glRotatef(-72.0, 0, 0, 1);
    DrawBox(0.45f, 0.0f, 0.65f, 0.65f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawO(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glScalef(1.10, 1.10, 1.10);
    glPushMatrix();

    glTranslatef(-0.0f, 0.75f, 0.01f);
    DrawBox(0.5f, 0.5f, 0.7f, 0.7f, 0.5f, 0.5f);

    // Right vertical
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.5f, 0.0f, -0.01f);
    DrawBox(1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // left Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, -0.015f);
    DrawBox(1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Bottom Horizontal
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.0f, -0.75, 0.015f);
    DrawBox(0.5f, 0.5f, 0.7f, 0.7f, 0.5f, 0.5f);

    // Top Right Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.44f, 0.71f, -0.0123f);
    glRotatef(-71.0, 0, 0, 1);
    DrawBox(0.0f, 0.45f, 0.60f, 0.5f, 0.5f, 0.5f);

    // Bottom Right Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.43f, -0.71f, 0.014f);
    glRotatef(70.0, 0, 0, 1);
    DrawBox(0.0f, 0.45f, 0.5f, 0.60f, 0.5f, 0.5f);

    // Top left Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.44f, 0.71f, 0.0153f);
    glRotatef(69.0, 0, 0, 1);
    DrawBox(0.45f, 0.0f, 0.65f, 0.5f, 0.5f, 0.5f);

    // Bottom left Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.44f, -0.70f, 0.0137f);
    glRotatef(-72.0, 0, 0, 1);
    DrawBox(0.45f, 0.0f, 0.65f, 0.65f, 0.5f, 0.5f);

    glPopMatrix();
    glPopMatrix();
}

void drawD(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glTranslatef(-0.0f, 0.75f, 0.01742f);
    DrawBox(0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f);

    // Right vertical
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.7f, 0.0f, +0.017452f);
    DrawBox(1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // left Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, 0.025f);

    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Bottom Horizontal
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.0f, -0.75, 0.014f);
    DrawBox(0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f);

    // Bottom Right Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.61f, -0.72f, -0.014f);
    glRotatef(65.0, 0, 0, 1);
    DrawBox(0.0f, 0.45f, 0.5f, 0.68f, 0.5f, 0.5f);

    // Top Right Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.61f, 0.72f, 0.015f);
    glRotatef(-65.0, 0, 0, 1);
    DrawBox(0.0f, 0.45f, 0.68f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawC(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glTranslatef(0.0f, 0.75f, 0.013f);
    DrawBox(0.5f, 0.5f, 1.0f, 1.5f, 0.5f, 0.5f);

    // left Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, 0.017f);
    DrawBox(1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Bottom Horizontal
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.0f, -0.75, 0.019f);
    DrawBox(0.5f, 0.5f, 1.5f, 1.0f, 0.5f, 0.5f);

    // Top left Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.44f, 0.71f, -0.0045f);
    glRotatef(69.0, 0, 0, 1);
    // DrawBox(0.45f, 0.0f, 0.65f, 0.5f, 0.5f, 0.5f);

    // Bottom left Corner
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.44f, -0.70f, 0.00455f);
    glRotatef(-72.0, 0, 0, 1);
    // DrawBox(0.45f, 0.0f, 0.65f, 0.65f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawE(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glTranslatef(0.0f, 0.8f, 0.0095f);
    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    // middle Horizontal
    glPopMatrix();
    glPushMatrix();

    DrawBox(0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f);

    // left Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, 0.0014f);

    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Bottom Horizontal
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, -0.8f, 0.0024f);
    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawP(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glTranslatef(0.0f, 0.8f, 0.005f);
    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    // middle Horizontal
    glPopMatrix();
    glPushMatrix();

    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    // Right vertical
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.5f, 0.5f, -0.0054f);
    DrawBox(0.7f, 0.7f, 0.5f, 0.5f, 0.5f, 0.5f);

    // left Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, 0.00654f);

    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawM(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // left Side
    glPushMatrix();
    glTranslatef(-0.7f, 0.0f, 0.001f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // middle Left
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.28f, 0.47f, -0.01f);
    glRotatef(40.0f, 0.0f, 0.0f, 1.0f);

    DrawBox(1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // middle Right
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.28f, 0.47f, -0.015f);
    glRotatef(-40.0f, 0.0f, 0.0f, 1.0f);
    DrawBox(1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Right Side
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.7f, 0.0f, 0.015f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Midddle Base
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.08f, -0.0054f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    DrawBox(0.3f, 0.3f, 0.5f, 0.35f, 0.35f, 0.5f);

    glPopMatrix();
}

void drawR(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glTranslatef(0.0f, 0.8f, 0.015f);
    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    // middle Horizontal
    glPopMatrix();
    glPushMatrix();

    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    // Right vertical
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.5f, 0.5f, 0.011f);
    DrawBox(0.7f, 0.7f, 0.5f, 0.5f, 0.5f, 0.5f);

    // left Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, 0.012f);

    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Right Backword
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.2f, -0.5f, -0.011f);

    glRotatef(35.0f, 0.0f, 0.0f, 1.0f);

    DrawBox(0.9f, 0.9f, 0.6f, 0.5f, 0.5f, 0.5f);

    // Right Backword
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.55f, -0.8f, -0.0143f);

    // glRotatef(20.0f, 0.0f, 0.0f, 1.0f);

    DrawBox(0.47f, 0.47f, 0.0f, 0.6f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawT(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glTranslatef(0.0f, 0.8f, 0.016f);
    DrawBox(0.5f, 0.5f, 1.9f, 1.7f, 0.5f, 0.5f);

    // middle Vertical
    glPopMatrix();
    glPushMatrix();

    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawI(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glTranslatef(0.0f, 0.8f, 0.014f);
    DrawBox(0.5f, 0.5f, 1.7f, 1.5f, 0.5f, 0.5f);

    // middle Vertical
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, -0.8f, -0.130f);
    DrawBox(0.5f, 0.5f, 1.5f, 1.7f, 0.5f, 0.5f);

    // middle Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.014f);
    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawF(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glTranslatef(0.0f, 0.8f, 0.01f);
    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    // middle Horizontal
    glPopMatrix();
    glPushMatrix();

    DrawBox(0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f);

    // left Vertical
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, -0.01f);

    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawS(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // top Horizontal
    glPushMatrix();

    glTranslatef(0.0f, 0.8f, 0.01f);
    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    // middle Horizontal
    glPopMatrix();
    glPushMatrix();
    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    // botom Horizontal
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, -0.01f);
    DrawBox(0.5f, 0.5f, 1.5f, 1.5f, 0.5f, 0.5f);

    // left vertical
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.5f, 0.5f, 0.013f);
    DrawBox(0.7f, 0.7f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Right vertical
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.5f, -0.5f, -0.013f);
    DrawBox(0.7f, 0.7f, 0.5f, 0.5f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawA(void)
{
    // Function declartions
    void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Code
    // left Side
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, 0.01f);
    glRotatef(-25.0f, 0.0f, 0.0f, 1.0f);
    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Right
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.5f, 0.0f, -0.01f);
    glRotatef(25.0f, 0.0f, 0.0f, 1.0f);
    DrawBox(2.0f, 2.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    // Middle
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.011f);
    DrawBox(0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f);

    // Top
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, -0.011f);
    DrawBox(0.28f, 0.28f, 0.6f, 0.6f, 0.5f, 0.5f);

    glPopMatrix();
}

void drawBox(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    glBegin(GL_QUADS);
    // FRONT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // RIGHT FACE
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // BACK FACE
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // LEFT FACE
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // TOP FACE
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // BOTTOM FACE
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}

void DrawBox(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth)
{
    // code
    glPushMatrix();

    // ***** 5th sphere on 2nd column, gold *****
    // diffuse material
    materialAmbiant[0] = 0.75164; // r
    materialAmbiant[1] = 0.60648; // g
    materialAmbiant[2] = 0.22648; // b
    materialAmbiant[3] = 1.0f;    // a
    // glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // specular material
    materialDefuse[0] = 0.628281; // r
    materialDefuse[1] = 0.555802; // g
    materialDefuse[2] = 0.366065; // b
    materialDefuse[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDefuse);

    // shininess
    materialShininess = 0.4 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // geometry
    glPopMatrix();

    if (texture_temp != 0)
        glBindTexture(GL_TEXTURE_2D, texture_temp);

    glBegin(GL_QUADS);
    // FRONT FACE
    // colorSetcolor(242.0f, 78.0f, 41.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(topWidth / 2, rightHeight / 2, topDepth / 2);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, topDepth / 2);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), bottomDepth / 2);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), bottomDepth / 2);
    glEnd();

    // RIGHT FACE
    glPushMatrix();

    // diffuse material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDefuse[0] = 1.0;  // r
    materialDefuse[1] = 1.0;  // g
    materialDefuse[2] = 1.0;  // b
    materialDefuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDefuse);

    // shininess
    //  materialShininess = 0.4 * 128;
    // glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    if (texture_temp == texture_letters_Yello_Gold_Design)
        colorSetcolor(89.0f, 29.0f, 15.0f);
    else if (texture_temp == texture_letters_Matrix)
        colorSetcolor(0.0f, 0.0f, 139.0f);
    else if (texture_temp == texture_letters_White_Gold_Design)
        colorSetcolor(255.0f, 228.0f, 196.0f);
    else
        colorSetcolor(89.0f, 29.0f, 15.0f);

    // glColor3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);

    glVertex3f(topWidth / 2, rightHeight / 2, -(topDepth / 2));

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(topWidth / 2, rightHeight / 2, topDepth / 2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), bottomDepth / 2);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), -(bottomDepth / 2));
    glEnd();

    // BACK FACE
    glBegin(GL_QUADS);
    // glColor3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, -(topDepth / 2));

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f((topWidth / 2), rightHeight / 2, -(topDepth / 2));

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f((bottomWidth / 2), -(rightHeight / 2), -(bottomDepth / 2));

    glTexCoord2f(0.0f, 0.0f);

    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), -bottomDepth / 2);
    glEnd();

    // LEFT FACE
    glBegin(GL_QUADS);
    // glColor3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, (topDepth / 2));

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, -(topDepth / 2)); // y = -1

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), -(bottomDepth / 2));

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), (bottomDepth / 2));
    glEnd();

    // TOP
    glBegin(GL_QUADS);
    // glColor3f(1.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(topWidth / 2, rightHeight / 2, topDepth / 2);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(topWidth / 2, rightHeight / 2, -(topDepth / 2));

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, -(topDepth / 2));

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, topDepth / 2);
    glEnd();

    // BOTTOM FACE
    // glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), bottomDepth / 2);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), -(bottomDepth / 2));

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), -(bottomDepth / 2));

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), bottomDepth / 2);

    glEnd();
}

void drawSaturn(void)
{
    // code
    // SAVE Camera Matrix (Push)
    glPushMatrix();

    // Enable Lighting
    glLightfv(GL_LIGHT3, GL_AMBIENT, gfLightAmbiantThree);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, gfLightDeffuseThree);
    glLightfv(GL_LIGHT3, GL_SPECULAR, gfLightSpicularThree);
    glLightfv(GL_LIGHT3, GL_POSITION, gfLightPositionsThree);
    glEnable(GL_LIGHT3);

    glMaterialfv(GL_FRONT, GL_AMBIENT, gfMaterialAmbiantThree);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gfMeterialDeffuseThree);
    glMaterialfv(GL_FRONT, GL_SPECULAR, gfMateralSpecularThree);
    glMaterialf(GL_FRONT, GL_SHININESS, gfMaterialShineenessThree);

    glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    // beutification - 1
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    // beutification - 2
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // beutification - 3
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    gluQuadricTexture(quadric_S1, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_saturn);

    glRotatef((GLfloat)angleSaturn, 0.0f, 0.0f, 1.0f);

    gluSphere(quadric_S1, 1.5f, 100, 100);

    glBindTexture(GL_TEXTURE_2D, 0);

    /////////// --------------- Ring
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glScalef(1.0f, 1.3f, 1.0f);
    glTranslatef(0.0f, -0.1f, 0.0f);
    glRotatef(90.0f, 1.0f, -0.15f, 0.0f);

    gluQuadricNormals(quadric_S1Disk, GLU_FLAT);
    gluQuadricTexture(quadric_S1Disk, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, texture_saturn_ring);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINEAR);

    glRotatef(angleSaturn, 0.0f, 0.0f, 1.0f);

    gluDisk(quadric_S1Disk, 2, 3, 50, 1);

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

void drawStars(void)
{
    glRotatef((GLfloat)angleSky, 1.0f, 1.0f, 1.0f);

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_stars);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);

    // FRONT FACE
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // RIGHT FACE
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // BACK FACE
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // LEFT FACE
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // TOP
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // BOTTOM FACE
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void drawPNGPayasdan(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_letters_Payasdan);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glScalef(12.0f, 7.0f, 1.0f);
    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void drawTechnologyCredits(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_Technology);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glScalef(8.0f, 5.0f, 5.0f);
    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void drawBooksAndRefremncesCredits(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_refrences);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glScalef(8.0f, 5.0f, 5.0f);
    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void drawMusicTrackCredits(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_sountrack);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glScalef(8.0f, 5.0f, 5.0f);
    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void drawSpecialThanksCredits(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_special_thanks);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glScalef(8.0f, 5.0f, 5.0f);
    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void drawSirAndMadamCredits(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_Sir);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glScalef(8.0f, 5.0f, 5.0f);
    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void drawGLCredits(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_GL);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glScalef(8.0f, 5.0f, 5.0f);
    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void drawYoheshswarSirCredits(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_yogeshwar_sir);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glScalef(8.0f, 5.0f, 5.0f);
    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void drawThannksToMatrixGroupMembersCredits(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_group_members);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glScalef(8.0f, 5.0f, 5.0f);
    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void fadeOurSirName(void)
{

    static float alpha = 0.0f;

    alpha = alpha + 0.003f;

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    glBegin(GL_QUADS);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    TCHAR str[100];
    swprintf_s(str, 100, TEXT("Alpha = %f \n"), alpha); // "_s" for secure
    SetWindowText(ghwnd, str);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void Credits(void)
{
    glDisable(GL_LIGHTING);
    static int sceneNumber_credit = 0;

    TCHAR str[100];
    swprintf_s(str, 100, TEXT("TIME = %4.2f , \n"), elapsedTime_S1); // "_s" for secure

    SetWindowText(ghwnd, str);

    static BOOL CREDIT_1 = FALSE;
    static BOOL CREDIT_2 = FALSE;
    static BOOL CREDIT_3 = FALSE;
    static BOOL CREDIT_4 = FALSE;
    static BOOL CREDIT_5 = FALSE;
    static BOOL CREDIT_6 = FALSE;
    static BOOL CREDIT_7 = FALSE;
    static BOOL CREDIT_8 = FALSE;
    static BOOL CREDIT_9 = FALSE;
    static BOOL CREDIT_10 = FALSE;

    elapsedTime_S1 = sdkGetTimerValue(&pTimer_SceneOne); // THIS FUNCTION GIVES TIME IN MILLLIESECONDS
    elapsedTime_S1 = elapsedTime_S1 / 1000.0f;           // CONVERTIN TIME MILLISECONDS TO SECONDS

    if (CREDIT_1 == FALSE)
    {

        if (elapsedTime_S1 > 7)
        {
            sceneNumber_credit++;
            sdkResetTimer(&pTimer_SceneOne);
            resetFadeIn = TRUE;
            CREDIT_1 = FALSE;
            resetFadeIn = TRUE;
        }
    }
    else if (CREDIT_2 == FALSE)
    {

        if (elapsedTime_S1 > 7)
        {
            sceneNumber_credit++;
            sdkResetTimer(&pTimer_SceneOne);
            CREDIT_2 = FALSE;
            resetFadeIn = TRUE;
        }
    }
    else if (CREDIT_3 == FALSE)
    {
        if (elapsedTime_S1 > 7)
        {
            sceneNumber_credit++;
            sdkResetTimer(&pTimer_SceneOne);
            CREDIT_3 = FALSE;
            // resetFadeIn = TRUE;
        }
    }
    else if (CREDIT_4 == FALSE)
    {

        if (elapsedTime_S1 > 7)
        {
            sceneNumber_credit++;
            sdkResetTimer(&pTimer_SceneOne);
            CREDIT_4 = FALSE;
            // resetFadeIn = TRUE;
        }
    }
    else if (CREDIT_5 == FALSE)
    {

        if (elapsedTime_S1 > 7)
        {
            sceneNumber_credit++;
            sdkResetTimer(&pTimer_SceneOne);
            CREDIT_5 = FALSE;
            // resetFadeIn = TRUE;
        }
    }
    else if (CREDIT_6 == FALSE)
    {

        if (elapsedTime_S1 > 7)
        {
            sceneNumber_credit++;
            sdkResetTimer(&pTimer_SceneOne);
            CREDIT_6 = FALSE;
            //  resetFadeIn = TRUE;
        }
    }
    else if (CREDIT_7 == FALSE)
    {

        if (elapsedTime_S1 > 7)
        {
            sceneNumber_credit++;
            sdkResetTimer(&pTimer_SceneOne);
            CREDIT_7 = FALSE;
            resetFadeIn = TRUE;
        }
    }
    else if (CREDIT_8 == FALSE)
    {

        if (elapsedTime_S1 > 7)
        {
            sceneNumber_credit++;
            sdkResetTimer(&pTimer_SceneOne);
            CREDIT_8 = FALSE;
            resetFadeIn = TRUE;
        }
    }
    else if (CREDIT_9 == FALSE)
    {

        if (elapsedTime_S1 > 10)
        {
            sceneNumber_credit++;
            sdkResetTimer(&pTimer_SceneOne);
            CREDIT_9 = FALSE;
            resetFadeIn = TRUE;
        }
    }

    gluLookAt(0, 0, 6.0f,
              0, 0, 0,
              0.0f, 1.0f, 0.0f);

    if (sceneNumber_credit == 1)
    {
        {
            // Moon & Stars
            glPushMatrix();
            glScalef(30.0f, 30.0f, 30.0f);
            drawStars();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -14.0f);
        drawTechnologyCredits();
        glPopMatrix();
    }
    else if (sceneNumber_credit == 2)
    {
        {
            // Moon & Stars
            glPushMatrix();
            glScalef(30.0f, 30.0f, 30.0f);
            drawStars();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -14.0f);
        drawBooksAndRefremncesCredits();
        glPopMatrix();
    }
    else if (sceneNumber_credit == 3)
    {
        {
            // Moon & Stars
            glPushMatrix();
            glScalef(30.0f, 30.0f, 30.0f);
            drawStars();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -14.0f);
        drawMusicTrackCredits();
        glPopMatrix();
    }
    else if (sceneNumber_credit == 4)
    {
        {
            // Moon & Stars
            glPushMatrix();
            glScalef(30.0f, 30.0f, 30.0f);
            drawStars();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -14.0f);
        drawThannksToMatrixGroupMembersCredits();
        glPopMatrix();
    }
    else if (sceneNumber_credit == 5)
    {
        {
            // Moon & Stars
            glPushMatrix();
            glScalef(30.0f, 30.0f, 30.0f);
            drawStars();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -14.0f);
        drawSpecialThanksCredits();
        glPopMatrix();
    }
    else if (sceneNumber_credit == 6)
    {
        {
            // Moon & Stars
            glPushMatrix();
            glScalef(30.0f, 30.0f, 30.0f);
            drawStars();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -14.0f);
        drawYoheshswarSirCredits();
        glPopMatrix();
    }

    else if (sceneNumber_credit == 7)
    {
        {
            // Moon & Stars
            glPushMatrix();
            glScalef(30.0f, 30.0f, 30.0f);
            drawStars();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -14.0f);
        drawGLCredits();
        glPopMatrix();
    }
    else if (sceneNumber_credit == 8)
    {
        {
            // Moon & Stars
            glPushMatrix();
            glScalef(30.0f, 30.0f, 30.0f);
            drawStars();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -12.0f);
        drawSirAndMadamCredits();
        glPopMatrix();

        if (elapsedTime_S1 > 5)
        {
            glTranslatef(0.0f, 0.0f, 5.5f);
            fadeOurSirName();
        }
    }

    glTranslatef(0.0f, 0.0f, 5.5f);
    fadeIn();
}
