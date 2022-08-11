/* Header Files */
#include <windows.h>
#include "OGLSCENE2.h"
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "std_image.h"
#include "Common.h"

#include "helper_timer.h"

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

extern BOOL bScene02Done;

extern FILE *gpFile; // FILE* -> #include<stdio.h>
int day = 0;
int year = 0;
int moon = 0;

// unsigned char *image_data = NULL;

float AngleSphere = 90.0f;
GLfloat radius = 8.0f;
float yEyeVector = 0;

/* Global Function Declartion */
BOOL InitSeenTwo(void);
void UnInitiallizeSeenTwo(void);
void DisableAllLights();
void SetLightColorGold(void);
void SetLightColorWhite(void);
void SetLightColorSilver(void);
void SetLightColorRed(void);
void SetLightColorCyan(void);
void SetLightColorGreen(void);
BOOLEAN loadHeightMapData(char *filename);

////-----------------Seen Two Global Variables
GLUquadric *quadric_S2 = NULL;
GLUquadric *quadric_S2Cylinder = NULL;
GLuint texture_moon;
GLuint texture_stars_S2;
GLfloat angleMoon = 0.0f;
GLfloat angleSky_S2 = 0.0f;

unsigned char *image_data = NULL;
const int TERRAIN_MAP_X = 257;
const int TERRAIN_MAP_Z = 257;
float MAX_HEIGHT_COLOR = 5.0f;
float HEIGHT_MAP_SCALE = 30.0f;
float xObj;
float yObj = -2.0f;

float terrainHeightMap[TERRAIN_MAP_X][TERRAIN_MAP_Z][3];
GLuint texture_grass;
GLuint texture_Chourang;

BOOL gbLight_S2v = FALSE;

//------from Home
GLfloat lightAmbinatZero_S2[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDefuseZero_S2[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecularZero_S2[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPositionZero_S2[] = {0.0f, 0.0f, 0.0f, 0.0f};

GLfloat lightAmbinatOne_S2[] = {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat lightDefuseOne_S2[] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat lightSpecularOne_S2[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPositionOne_S2[] = {1.0f, 1.0f, 1.0f, 1.0f};

GLfloat materialAmbiant_S2[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat materialDefuse_S2[] = {0.9f, 0.9f, 0.9f, 1.0f};
GLfloat materialSpecular_S2[] = {1.0f, 1.f, 1.0f, 1.0f};
GLfloat materialShininess_S2 = 50.0f;

GLfloat lightAmbiantTwo_S2[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDeffuseTwo_S2[] = {0.5f, 0.5f, 0.5f, 0.0f};
GLfloat lightSpecularTwo_S2[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightPositionsTwo_S2[] = {0.0f, 30.0f, -6.0f, 1.0f};
GLfloat lightSpotLightDirectionTwo_S2[] = {0.0f, -1.0f, 0.0f};
const GLfloat angleSpotCutOffTwo_S2 = 10.0f;
const GLfloat angleSpotExpoTwo_S2 = 2.0f;

GLfloat lightAngleOne = 0.0f;
GLfloat lightAngleTwo = 0.0f;
GLfloat lightAngleZero = 0.0f;

GLuint texture_home_BricksWallpaper;
GLuint texture_home_WhiteWallpaper;
GLuint texture_home_Roof;
GLuint texture_home_window;
GLuint texture_home_door;
GLuint texture_home_basebricks;
GLuint texture_trunk;
GLuint texture_book;
GLuint texture_blank_book_Page;
GLuint texture_GaneshWallpaper;
GLuint texture_RamSitaWallpaper;
GLuint texture_leaf;
GLuint texture_water;
GLuint texture_book_Cover;

GLfloat anglePageOne = 10.0f;
GLfloat anglePageTwo = 10.0f;
GLfloat anglePageThree = 10.0f;
GLfloat anglePageFour = 10.0f;
GLfloat anglePageFive = 10.0f;
BOOL isTrunk = TRUE;

// timer related

StopWatchInterface *pTimer_S2;
BOOL bTimerStarted_S2 = FALSE;

#define SCENE_01_DURATION_S2 9 // 10
#define SCENE_02_DURATION_S2 18
#define SCENE_03_DURATION_S2 16
#define SCENE_04_DURATION_S2 17
#define SCENE_05_DURATION_S2 16


int sceneFadeIn = 0.0f;

GLfloat CameraVector[3] = {-28.7f, 10.59f, -13.69f};
GLfloat EyeVector[3] = {0.8f, 2.799f, 5.28f};
GLfloat UpVector[3] = {0.0f, 1.0f, 0.0f};

float angleOfCamera = 165.0f;
float outeCameraHeight = 6.0;
float xUpVectorForOutercamera = -0.5;

float xCam;
float yCam;
float xEye;
float yEye;

unsigned int sceneNumber_S2 = 1; // when program begins we are showing the first texture by default hence '1'
float elapsedTime_S2;

BOOL initializeSceneTwo(void)
{
    /* fucntion delcations */
    BOOL LoadGLTexture_S2(GLuint *, TCHAR[]);
    BOOL InitSeenTwo(void);

    // Seem Two Init
    if (InitSeenTwo() == FALSE)
        return FALSE;

    return TRUE;
}

void drawonceAfterinit(void)
{
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT3);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbinatZero_S2);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDefuseZero_S2);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularZero_S2);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPositionZero_S2);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT5, GL_AMBIENT, lightAmbinatOne_S2);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, lightDefuseOne_S2);
    glLightfv(GL_LIGHT5, GL_SPECULAR, lightSpecularOne_S2);
    glLightfv(GL_LIGHT5, GL_POSITION, lightPositionOne_S2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbiant_S2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDefuse_S2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular_S2);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess_S2);

    glEnable(GL_LIGHT5);

    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbiantTwo_S2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDeffuseTwo_S2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecularTwo_S2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPositionsTwo_S2);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightSpotLightDirectionTwo_S2);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, angleSpotCutOffTwo_S2);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, angleSpotExpoTwo_S2);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.5f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0f);
    glEnable(GL_LIGHT2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    GLfloat lmodel_ambient[] = {1.0f, 1.0f, 1.0f, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    GLfloat mat_amb_diff[] = {0.2, 0.2, 0.2f, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 mat_amb_diff);

    // BLEND
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void displaySceneTwo(void)
{

    // function prototype
    void drawSeenTwo(void);
    void fadeInFirstScene(void);
    void fadeOutFirstScene(void);
    void fadeInSecondScene(void);
    void fadeOutSecondScene(void);
    void fadeInThirdScene(void);
    void fadeOutFifthScene(void);

    /* Code */

    /////////////////////////////////////
    // Light related changes
    // For Home
    glEnable(GL_TEXTURE_2D);

    ///////////////////////////////////////

    static BOOL bScene01Done_S2 = FALSE;
    static BOOL bScene02Done_S2 = FALSE;
    static BOOL bScene03Done_S2 = FALSE;
    static BOOL bScene04Done_S2 = FALSE;
    static BOOL bScene05Done_S2 = FALSE;
    static BOOL bScene06Done_S2 = FALSE;

    /* Code */
    if (bTimerStarted_S2 == FALSE)
    {
        drawonceAfterinit();
        bTimerStarted_S2 = TRUE;
        // Start them music here
        sdkStartTimer(&pTimer_S2);
    }
    elapsedTime_S2 = sdkGetTimerValue(&pTimer_S2); // THIS FUNCTION GIVES TIME IN MILLLIESECONDS
    elapsedTime_S2 = elapsedTime_S2 / 1000.0f;

    if (bScene01Done_S2 == FALSE)
    {

        if (elapsedTime_S2 > SCENE_01_DURATION_S2)
        {
            sceneNumber_S2++;
            sdkResetTimer(&pTimer_S2);
            bScene01Done_S2 = TRUE;
            outeCameraHeight = 4.0f;
        }
    }
    else if (bScene02Done_S2 == FALSE)
    {

        if (elapsedTime_S2 > SCENE_02_DURATION_S2)
        {
            sceneNumber_S2++;
            sdkResetTimer(&pTimer_S2);
            bScene02Done_S2 = TRUE;

            // Assign Values for home camera
            CameraVector[0] = -2.0f;
            CameraVector[1] = 0.3f;
            CameraVector[2] = 1.8f;

            EyeVector[0] = 0.2f;
            EyeVector[1] = 0.0f;
            EyeVector[2] = 0.0f;
        }
    }
    else if (bScene03Done_S2 == FALSE)
    {

        if (elapsedTime_S2 > SCENE_03_DURATION_S2)
        {
            sceneNumber_S2++;
            sdkResetTimer(&pTimer_S2);
            bScene03Done_S2 = TRUE;
        }
    }
    else if (bScene04Done_S2 == FALSE)
    {

        if (elapsedTime_S2 > SCENE_04_DURATION_S2)
        {
            sceneNumber_S2++;
            sdkResetTimer(&pTimer_S2);
            bScene04Done_S2 = TRUE;
        }
    }
    else if (bScene05Done_S2 == FALSE)
    {

        if (elapsedTime_S2 > SCENE_05_DURATION_S2)
        {
            sceneNumber_S2++;
            sdkResetTimer(&pTimer_S2);
            bScene05Done_S2 = TRUE;
            bScene02Done = FALSE;
        }
    }

    if (sceneNumber_S2 == 2)
    {
        float angle = (angleOfCamera * M_PI) / 180.0f;

        xCam = (14.35 * cos(angle)) - 14.35f;
        yCam = 30.35 * sin(angle);
        xEye = (14.35 * cos(angle + 5)) - 14.35f;
        yEye = 30.35 * sin(angle + 5);

        // for the 2 nd outer scene
        gluLookAt(xCam + 3.0f, CameraVector[1] + outeCameraHeight, yCam,
                  xEye, EyeVector[1] - 2.0f + outeCameraHeight, yEye,
                  xUpVectorForOutercamera, 1.0f, 0.0f);
    }
    else
    {
        // First Outer Scene
        gluLookAt(CameraVector[0], CameraVector[1], CameraVector[2],
                  EyeVector[0], EyeVector[1], EyeVector[2],
                  0.0f, 1.0f, 0.0f);
    }

    glPushMatrix();
    {
        if (sceneNumber_S2 == 1 && sceneFadeIn == 1)
        {
            drawSeenTwo();
        }
        else if (sceneNumber_S2 == 2 && sceneFadeIn == 2)
        {
            drawSeenTwo();
        }
        else if (sceneNumber_S2 == 3 && sceneFadeIn == 3)
        {
            drawSeenTwo();
        }
        else if (sceneNumber_S2 == 4)
        {
            drawSeenTwo();
        }
    }
    glPopMatrix();

    glDisable(GL_LIGHTING);

    if (sceneNumber_S2 == 1)
    {
        if (elapsedTime_S2 < 4)
        {
            fadeInFirstScene();
            sceneFadeIn = 1;
        }
        else if (elapsedTime_S2 > SCENE_01_DURATION_S2 - 4)
            fadeOutFirstScene();
    }
    else if (sceneNumber_S2 == 2)
    {
        if (elapsedTime_S2 < 5)
        {
            sceneFadeIn = 2;
            fadeInSecondScene();
        }
        else if (angleOfCamera < 35.0f)
            fadeOutSecondScene(); 
    }
    else if (sceneNumber_S2 == 3)
    {
        if (elapsedTime_S2 < 6)
        {
            sceneFadeIn = 3;
            fadeInThirdScene();
        }
    }

    glEnable(GL_LIGHTING);
}

void updateSceneTwo(void)
{
    /* code */
    angleMoon = angleMoon + 0.02f;
    if (angleMoon > 360.0f)
        angleMoon = 0.0f;

    angleSky_S2 = angleSky_S2 + 0.002f;
    if (angleSky_S2 > 360.0f)
        angleSky_S2 = 0.0f;

    if (sceneNumber_S2 == 2)
    {
        angleOfCamera = angleOfCamera - 0.15f;

        if (angleOfCamera > 70.0f)
            outeCameraHeight = outeCameraHeight - 0.022f;
        else if (angleOfCamera > 50.0f)
            EyeVector[1] = EyeVector[1] + 0.033f;
        else if (angleOfCamera > 30.0f)
            EyeVector[1] = EyeVector[1] + 0.035f;
        else
            outeCameraHeight = outeCameraHeight + 0.04f;

        if (angleOfCamera < 80.0f && xUpVectorForOutercamera <= 0)
            xUpVectorForOutercamera = xUpVectorForOutercamera + 0.008f;
    }
    else if (sceneNumber_S2 == 3)
    {
        if (elapsedTime_S2 > 4)
        {
            if (CameraVector[0] < 3.39f)
                CameraVector[0] = CameraVector[0] + 0.015f;

            if (CameraVector[1] < 0.6f)
                CameraVector[1] = CameraVector[1] + 0.005f;

            if (CameraVector[2] > 0.2f)
                CameraVector[2] = CameraVector[2] - 0.005f;

            if (EyeVector[0] < 3.0f)
                EyeVector[0] = EyeVector[0] + 0.01f;

            if (EyeVector[1] < 0.4f)
                EyeVector[1] = EyeVector[1] + 0.005f;

            if (EyeVector[2] > -5.79f)
                EyeVector[2] = EyeVector[2] - 0.015f;
        }
    }
    else if (sceneNumber_S2 == 4)
    {
        if (elapsedTime_S2 > 2)
        {
            if (CameraVector[0] < 3.39f)
                CameraVector[0] = CameraVector[0] + 0.002f;

            if (CameraVector[1] > -0.35f)
                CameraVector[1] = CameraVector[1] - 0.0075f;
            
            if (CameraVector[2] > -1.90f)
                CameraVector[2] = CameraVector[2] - 0.005f;
            else
                if (anglePageOne < 171.0f)
                    anglePageOne = anglePageOne + 0.7f;

            if (EyeVector[0] < 3.39f)
                EyeVector[0] = EyeVector[0] + 0.002f;

            if (EyeVector[1] > -8.0f)
                EyeVector[1] = EyeVector[1] - 0.02f;

            if (EyeVector[2] > -1.8f)
                EyeVector[2] = EyeVector[2] - 5.5f;
           
        }
    }
}

void cubeforfadeinfadeout(void)
{
    glBegin(GL_QUADS);

    // FRONT FACE
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // RIGHT FACE
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // BACK FACE
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // LEFT FACE
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // TOP FACE
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // BOTTOM FACE
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}

void fadeInFirstScene(void)
{
    TCHAR str[200];

    static float alpha1 = 1.0f;

    alpha1 = alpha1 - 0.025f;

    glPushMatrix();

    glTranslatef(CameraVector[0] + 1.0f, CameraVector[1] + 1.0f, CameraVector[2] + 1.5f);

    glScalef(5.0f, 4.0f, 5.0f);

    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha1);
    cubeforfadeinfadeout();
    glPopMatrix();
}

void fadeOutFirstScene(void)
{
    static float alpha = 0.0f;

    alpha = alpha + 0.025f;

    glPushMatrix();
    glTranslatef(CameraVector[0] + 1.0f, CameraVector[1] + 1.0f, CameraVector[2] + 1.5f);
    glScalef(5.0f, 3.0f, 5.0f);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, -1.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);
    glVertex3f(0.0f, -1.0f, 1.0f);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void fadeInSecondScene(void)
{

    glPushMatrix();
    static float alpha = 1.0f;

    alpha = alpha - 0.03f;

    glTranslatef(xCam + 3.5f, CameraVector[1] + outeCameraHeight, yCam + 2.5f);
    glScalef(5.0f, 5.0f, 5.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    cubeforfadeinfadeout();
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void fadeOutSecondScene(void)
{

    static float alpha = 0.0f;

    alpha = alpha + 0.03f;
    glPushMatrix();
    glTranslatef(xCam + 3.5f, CameraVector[1] + outeCameraHeight + 0.5f, yCam - 2.5f);
    glScalef(2.5f, 1.5f, 2.5f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    cubeforfadeinfadeout();

    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPopMatrix();
}

void fadeInThirdScene(void)
{

    static float alpha = 1.0f;

    alpha = alpha - 0.01f;

    glPushMatrix();
    {
        glTranslatef(CameraVector[0] + 1.0f, CameraVector[1], CameraVector[2] - 1.5f);

        glScalef(2.0f, 2.0f, 2.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, alpha);

        cubeforfadeinfadeout();

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glPopMatrix();
}

void fadeOutFifthScene(void)
{

    static float alpha = 0.0f;

    alpha = alpha + 0.01f;

    glPushMatrix();
    {
        glTranslatef(6.2f, -3.8f, -7.5);
        glScalef(2.0f, 2.0f, 2.0f);
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, alpha);

        glBegin(GL_QUADS);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, -1.0f);
        glEnd();

        glDisable(GL_BLEND);
    }
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
}

void uninitializeSceneTwo(void)
{
    /* function declarations */
    void UnInitiallizeSeenTwo(void);

    /* code */

    /*  */
    UnInitiallizeSeenTwo();
}

BOOL LoadGLTexture_S2(GLuint *texture, TCHAR ImageResourceID[])
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
//-----------------------------------------------------------------------------------------------------------------------------------

// Function Definitions
BOOL InitSeenTwo(void)
{
    // Function Protype
    void initializeTerrainCoords(void);

    // code
    // quadric_S2 intialliza
    // create quadric_S2

    quadric_S2 = gluNewQuadric();

    quadric_S2Cylinder = gluNewQuadric();

    if (LoadGLTexture_S2(&texture_moon, MAKEINTRESOURCE(IDBITMAP_MOON_TEXTRURE)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_moon Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_stars_S2, MAKEINTRESOURCE(IDBITMAP_STARS_TEXTRURE)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_stars_S2 Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_grass, MAKEINTRESOURCE(IDBITMAP_TEXTURE_GRASS)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_grass Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (loadHeightMapData("Height_Map.bmp") == FALSE)
    {
        fprintf(gpFile, "loadHeightMapData Failed...\n");
        return FALSE; // write log in wndproc
    }

    // texture for home
    if (LoadGLTexture_S2(&texture_home_BricksWallpaper, MAKEINTRESOURCE(IDBITMAP_BRICKSWALLPAPER)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2.bmp() for texture_home_BricksWallpaper Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_home_WhiteWallpaper, MAKEINTRESOURCE(IDBITMAP_WHITEWALLPAPER)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_home_WhiteWallpaper Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_home_Roof, MAKEINTRESOURCE(IDBITMAP_ROOFWALLPAPER)) == FALSE)
    {
        fprintf(gpFile, "iLoadGLTexture_S2() for texture_home_Roof Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_home_door, MAKEINTRESOURCE(IDBITMAP_HOME_DOORTEXTURE)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_home_door Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_home_window, MAKEINTRESOURCE(IDBITMAP_HOME_WINDOWTEXTURE)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_home_window Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_home_basebricks, MAKEINTRESOURCE(IDBITMAP_HOME_BASEBRICKSTEXTURE)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_home_basebricks Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_trunk, MAKEINTRESOURCE(IDBITMAP_WOODTEXTURE)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_trunk Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_book, MAKEINTRESOURCE(IDBITMAP_BOOKTEXTURE)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_book Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_GaneshWallpaper, MAKEINTRESOURCE(IDBITMAP_GANESHWALLPAPER)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_GaneshWallpaper Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_RamSitaWallpaper, MAKEINTRESOURCE(IDBITMAP_RAMSITAWALLPAPER)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2() for texture_RamSitaWallpaper Failed...\n");
        return FALSE; // write log in wndproc
    }

    if (LoadGLTexture_S2(&texture_trunk, MAKEINTRESOURCE(IDBITMAP_TRUNK)) == FALSE)
    {

        fprintf(gpFile, "LoadGLTexture_S2 failed for texture_trunk");
        return FALSE;
    }

    if (LoadGLTexture_S2(&texture_leaf, MAKEINTRESOURCE(IDBITMAP_LEAF)) == FALSE)
    {
        fprintf(gpFile, "LoadGLTexture_S2 failed for texture_leaf");
        return FALSE;
    }

    if (LoadGLTexture_S2(&texture_water, MAKEINTRESOURCE(IDBITMAP_WATER)) == FALSE)
    {

        fprintf(gpFile, "LoadGLTexture_S2 failed for texture_water");
        return FALSE;
    }

    if (LoadGLTexture_S2(&texture_Chourang, MAKEINTRESOURCE(IDBITMAP_CHOURANG_TEXTURE)) == FALSE)
    {

        fprintf(gpFile, "LoadGLTexture_S2 failed for texture_Chourang");
        return FALSE;
    }

    if (LoadGLTexture_S2(&texture_blank_book_Page, MAKEINTRESOURCE(IDBITMAP_BLANK_BOOK_PAGE)) == FALSE)
    {

        fprintf(gpFile, "LoadGLTexture_S2 failed for texture_blank_book_Page");
        return FALSE;
    }

    if (LoadGLTexture_S2(&texture_book_Cover, MAKEINTRESOURCE(IDBITMAP_BOOK_COVER)) == FALSE)
    {

        fprintf(gpFile, "LoadGLTexture failed for texture_blank_book_Page");
        return FALSE;
    }

    glEnable(GL_TEXTURE_2D);

    // initializeTerrainCoords
    initializeTerrainCoords();

    // Creating the timer
    sdkCreateTimer(&pTimer_S2);

    return TRUE;
}

void initializeTerrainCoords(void)
{
    int heightMultiplier = 2;

    for (int z = 0; z < TERRAIN_MAP_Z; z++)
    {
        for (int x = 0; x < TERRAIN_MAP_X; x++)
        {
            terrainHeightMap[x][z][0] = float(x) * HEIGHT_MAP_SCALE;
            // terrainHeightMap[x][z][1] = 0.0f;
            terrainHeightMap[x][z][1] = image_data[(x + z * TERRAIN_MAP_Z) * 3] * heightMultiplier;
            terrainHeightMap[x][z][2] = -float(z) * HEIGHT_MAP_SCALE;
        }
        glEnd();
    }
}

BOOLEAN loadHeightMapData(char *filename)
{
    int w;
    int h;
    int channels_in_file;
    image_data = stbi_load(filename, &w, &h, &channels_in_file, 0);

    if (image_data == NULL)
    {
        fprintf(gpFile, "File %s loading Failed..,.\'n", filename);
        return FALSE;
    }

    return TRUE;
}

void drawTerrain(void)
{

    glDisable(GL_LIGHTING);

    glPushMatrix();
    {
        glBindTexture(GL_TEXTURE_2D, texture_grass);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_2D, GL_MODULATE);
        for (int z = 0; z < (TERRAIN_MAP_Z - 1); z++)
        {
            glBegin(GL_TRIANGLE_STRIP);
            for (int x = 0; x < (TERRAIN_MAP_X - 1); x++)
            {
                // glColor3f(1.0f, 1.0f, 1.0f);
                float scaleHeight = terrainHeightMap[x][z][1] / HEIGHT_MAP_SCALE;

                float nextScaleHeight = terrainHeightMap[x][z + 1][1] / HEIGHT_MAP_SCALE;

                float color = 0.5f + 0.5f * scaleHeight / MAX_HEIGHT_COLOR;

                float nextColor = 0.5f + 0.5f * nextScaleHeight / MAX_HEIGHT_COLOR;
                glNormal3f(0.0f, 1.0f, 0.0f);
                glTexCoord2f(0.0f, 0.0f);
                glColor3f(color, color, color);
                glVertex3f(terrainHeightMap[x][z][0], terrainHeightMap[x][z][1], terrainHeightMap[x][z][2]);

                glNormal3f(0.0f, -1.0f, 0.0f);
                glTexCoord2f(1.0f, 0.0f);
                glColor3f(color, color, color);
                glVertex3f(terrainHeightMap[x + 1][z][0], terrainHeightMap[x + 1][z][1], terrainHeightMap[x + 1][z][2]);

                glNormal3f(0.0f, -1.0f, 0.0f);
                glTexCoord2f(0.0f, 1.0f);
                glColor3f(nextColor, nextColor, nextColor);
                glVertex3f(terrainHeightMap[x][z + 1][0], terrainHeightMap[x][z + 1][1], terrainHeightMap[x][z + 1][2]);

                glNormal3f(0.0f, 1.0f, 0.0f);
                glTexCoord2f(1.0f, 1.0f);
                glColor3f(nextColor, nextColor, nextColor);
                glVertex3f(terrainHeightMap[x + 1][z + 1][0], terrainHeightMap[x + 1][z + 1][1], terrainHeightMap[x + 1][z + 1][2]);
            }
            glEnd();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void UnInitiallizeSeenTwo(void)
{
    // code
    if (texture_book_Cover)
    {
        glDeleteTextures(1, &texture_book_Cover);
        texture_book_Cover = 0;
    }

    if (texture_blank_book_Page)
    {
        glDeleteTextures(1, &texture_blank_book_Page);
        texture_blank_book_Page = 0;
    }

    if (texture_Chourang)
    {
        glDeleteTextures(1, &texture_Chourang);
        texture_Chourang = 0;
    }

    if (texture_water)
    {
        glDeleteTextures(1, &texture_water);
        texture_water = 0;
    }

    if (quadric_S2Cylinder)
    {
        gluDeleteQuadric(quadric_S2Cylinder);
        quadric_S2Cylinder = NULL;
    }

    if (texture_trunk)
    {
        glDeleteTextures(1, &texture_trunk);
        texture_trunk = 0;
    }

    if (texture_leaf)
    {
        glDeleteTextures(1, &texture_leaf);
        texture_leaf = 0;
    }

    if (texture_grass)
    {
        glDeleteTextures(1, &texture_grass);
        texture_grass = 0;
    }

    if (texture_moon)
    {
        glDeleteTextures(1, &texture_moon);
        texture_moon = 0;
    }

    if (texture_stars_S2)
    {
        glDeleteTextures(1, &texture_stars_S2);
        texture_stars_S2 = 0;
    }

    if (quadric_S2)
    {
        gluDeleteQuadric(quadric_S2);
        quadric_S2 = NULL;
    }

    // home
    if (texture_GaneshWallpaper)
    {
        glDeleteTextures(1, &texture_GaneshWallpaper);
        texture_GaneshWallpaper = 0;
    }

    if (texture_RamSitaWallpaper)
    {
        glDeleteTextures(1, &texture_RamSitaWallpaper);
        texture_RamSitaWallpaper = 0;
    }

    if (texture_trunk)
    {
        glDeleteTextures(1, &texture_trunk);
        texture_trunk = 0;
    }

    if (texture_home_basebricks)
    {
        glDeleteTextures(1, &texture_home_basebricks);
        texture_home_basebricks = 0;
    }

    if (texture_home_door)
    {
        glDeleteTextures(1, &texture_home_door);
        texture_home_door = 0;
    }

    if (texture_home_window)
    {
        glDeleteTextures(1, &texture_home_window);
        texture_home_window = 0;
    }

    if (texture_home_BricksWallpaper)
    {
        glDeleteTextures(1, &texture_home_BricksWallpaper);
        texture_home_BricksWallpaper = 0;
    }

    if (texture_home_WhiteWallpaper)
    {
        glDeleteTextures(1, &texture_home_WhiteWallpaper);
        texture_home_WhiteWallpaper = 0;
    }

    if (texture_home_Roof)
    {
        glDeleteTextures(1, &texture_home_Roof);
        texture_home_Roof = 0;
    }
}

void drawSeenTwo(void)
{
    // Funtion Prototype
    void drawMoon(void);
    void drawStars_S2(void);
    void drawHomeSeen(void);
    void drawRoomWithoutInterior(void);
    void drawLake(void);
    void drawTerrain(void);
    void drawTree(GLfloat, GLfloat, GLfloat);
    void drawChourang(void);
    void drawPat(void);

    // code
    // Moon
    glPushMatrix();

    drawMoon();

    glPopMatrix();
    // Terrain
    glPushMatrix();
    glTranslatef(xObj, yObj, -20.0f);
    glTranslatef(-130.5f, -1.5f, 150.0f);
    glScalef(0.05f, 0.05f, 0.05f);

    drawTerrain();

    glPopMatrix();

    // Stars

    glPushMatrix();

    glScalef(200.0f, 200.0f, 200.0f);

    drawStars_S2();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(0.30f, 0.30f, 0.30f);
    drawHomeSeen();

    glPopMatrix();

    {
        // houses
        glPushMatrix();

        glTranslatef(10.0f, 1.5f, 10.0f);
        glRotatef(80.0f, 0.0f, -1.0f, 0.0f);
        drawRoomWithoutInterior();

        glPopMatrix();

        glPushMatrix();

        glTranslatef(-7.0f, 1.5f, 10.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        drawRoomWithoutInterior();

        glPopMatrix();

        glPushMatrix();

        glTranslatef(10.0f, 1.5f, 20.0f);
        glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
        drawRoomWithoutInterior();

        glPopMatrix();
    }

    {
        // Water
        glDisable(GL_LIGHTING);
        glPushMatrix();

        glTranslatef(-7.0f, -2.2f, -13.0f);
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
        glScalef(3.0f, 3.0f, 3.5f);
        drawLake();

        glPopMatrix();
        glEnable(GL_LIGHTING);
    }

    {
        // Trees
        glDisable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef(-6.0f, -2.0f, -8.0f);
        glScalef(0.35f, 0.30f, 0.35f);
        drawTree(0.5f, 0.5f, 10.0f);

        glPopMatrix();

        glPushMatrix();

        glTranslatef(10.0f, -2.0f, 0.0f);
        glScalef(0.35f, 0.30f, 0.35f);
        drawTree(0.5f, 0.5f, 10.0f);

        glPopMatrix();

        glPushMatrix();

        glTranslatef(-10.0f, -1.5f, 22.0f);
        glScalef(0.35f, 0.35f, 0.35f);
        drawTree(0.5f, 0.5f, 10.0f);

        glPopMatrix();

        glPushMatrix();

        glTranslatef(14.0f, -2.0f, 5.0f);
        glScalef(0.35f, 0.30f, 0.35f);
        drawTree(0.5f, 0.5f, 10.0f);

        glPopMatrix();

        glPushMatrix();

        glTranslatef(17.0f, -2.0f, 15.0f);
        glScalef(0.35f, 0.30f, 0.35f);
        drawTree(0.5f, 0.5f, 10.0f);

        glPopMatrix();

        glPushMatrix();

        glTranslatef(16.0f, -2.0f, 21.0f);
        glScalef(0.35f, 0.30f, 0.35f);
        drawTree(0.5f, 0.5f, 10.0f);

        glPopMatrix();

        glPushMatrix();

        glTranslatef(8.0f, -2.0f, 30.0f);
        glScalef(0.30f, 0.30f, 0.30f);
        drawTree(0.5f, 0.5f, 10.0f);
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }
}

void drawMoon(void)
{
    // variable declartions
    GLfloat gfLightAmbiantThree[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat gfLightDeffuseThree[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat gfLightSpicularThree[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat gfLightPositionsThree[] = {100.0f, 40.0f, 40.0f, 1.0f};
    GLfloat gfMaterialAmbiant_S2Three[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat gfMeterialDeffuseThree[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat gfMateralSpecularThree[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat gfMaterialShineenessThree = 128.0f;

    // code
    // SAVE Camera Matrix (Push)
    glPushMatrix();

    // Ligth related Changes - for Moon
    glLightfv(GL_LIGHT3, GL_AMBIENT, gfLightAmbiantThree);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, gfLightDeffuseThree);
    glLightfv(GL_LIGHT3, GL_SPECULAR, gfLightSpicularThree);
    glLightfv(GL_LIGHT3, GL_POSITION, gfLightPositionsThree);
    glMaterialfv(GL_FRONT, GL_AMBIENT, gfMaterialAmbiant_S2Three);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gfMeterialDeffuseThree);
    glMaterialfv(GL_FRONT, GL_SPECULAR, gfMateralSpecularThree);
    glMaterialf(GL_FRONT, GL_SHININESS, gfMaterialShineenessThree);

    DisableAllLights();
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(15.0f, 45.0f, -120.0f);

    // beutification - 1
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    // beutification - 2
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // beutification - 3
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    gluQuadricTexture(quadric_S2, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_moon);

    glRotatef((GLfloat)angleMoon, 0.0f, 0.0f, 1.0f);

    gluSphere(quadric_S2, 5.0f, 100, 100);

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    glPushMatrix();

    // Ligth related Changes - for Moon
    glDisable(GL_LIGHT3);
    glEnable(GL_LIGHT5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
}

void drawStars_S2(void)
{
    glDisable(GL_LIGHTING);
    glRotatef((GLfloat)angleSky_S2, 1.0f, 1.0f, 1.0f);

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_stars_S2);

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

    glEnable(GL_LIGHTING);
}

void drawHomeSeen(void)
{

    // funnction declartions
    void drawHome(void);
    void drawTable(void);
    void drawBook(void);
    void DrawBox(void);
    void drawRoomWithoutInterior(void);
    void drawKhurchi(void);
    void drawChourang(void);
    void drawPat(void);

    // code
    glPushMatrix();

    glLightfv(GL_LIGHT5, GL_AMBIENT, lightAmbinatOne_S2);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, lightDefuseOne_S2);
    glLightfv(GL_LIGHT5, GL_SPECULAR, lightSpecularOne_S2);
    glLightfv(GL_LIGHT5, GL_POSITION, lightPositionOne_S2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbiant_S2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDefuse_S2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular_S2);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess_S2);

    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();

    glPushMatrix();
    // Home
    {
        glTranslatef(5.0f, 0.0f, 0.0f);
        glPushMatrix();

        glScalef(12.0f, 5.0f, 9.0f);
        drawHome();
        glPopMatrix();

        // Table
        glPushMatrix();

        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, -2.0f, -9.0f);
        glScalef(1.0f, 1.0f, 1.1f);
        drawTable();

        // Chourang
        glPopMatrix();
        glPushMatrix();

        glTranslatef(6.2f, -4.2f, -7.5);
        glScalef(1.0f, 1.0f, 1.1f);
        glBindTexture(GL_TEXTURE_2D, texture_Chourang);
        drawChourang();
        glBindTexture(GL_TEXTURE_2D, 0);

        // Pat
        glPopMatrix();
        glPushMatrix();
        glTranslatef(6.2f, -4.45f, -4.5);
        glScalef(1.0f, 1.0f, 1.1f);

        glEnable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, texture_Chourang);
        drawPat();
        glBindTexture(GL_TEXTURE_2D, 0);

        // khurchi
        glPopMatrix();
        glPushMatrix();

        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, -4.0f, -5.0f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

        glDisable(GL_LIGHTING);
        drawKhurchi();
        glEnable(GL_LIGHTING);

        // Ganesh Frame
        glPopMatrix();
        glPushMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPopMatrix();
        glPushMatrix();

        glTranslatef(-6.0f, 1.0f, -8.9f);
        glScalef(1.2f, 1.2f, 0.02f);
        glBindTexture(GL_TEXTURE_2D, texture_GaneshWallpaper);
        DrawBox();
        glBindTexture(GL_TEXTURE_2D, 0);

        // RamSita Frame
        glPopMatrix();
        glPushMatrix();

        glTranslatef(6.0f, 1.0f, -8.9f);
        glScalef(1.5f, 1.0f, 0.02f);
        glBindTexture(GL_TEXTURE_2D, texture_RamSitaWallpaper);
        DrawBox();
        glBindTexture(GL_TEXTURE_2D, 0);

        // Book
        glPopMatrix();
        glPushMatrix();

        glTranslatef(6.2f, -3.8f, -7.5);
        glScalef(0.65f, 0.65f, 0.65f);
        drawBook();

        glPopMatrix();
    }
    glPopMatrix();
}

void drawHome(void)
{
    // function declartions
    void DrawBox(void);

    // code
    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);

    glPushMatrix();
    {
        // FRONT FACE
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glEnd();

        glPopMatrix();
    }
    glPushMatrix();

    // window Right
    glBindTexture(GL_TEXTURE_2D, texture_home_window);
    glTranslatef(0.65f, 0.0f, 0.001f);
    glScalef(0.15f, 0.4f, 1.0001f);

    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // window Left
    glTranslatef(-0.65f, 0.0f, 0.001f);
    glScalef(0.15f, 0.4f, 1.00001f);

    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // window Back
    glTranslatef(0.0f, 0.0f, 0.001f);
    glScalef(0.15f, 0.4f, 1.00001f);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // Dooor
    glBindTexture(GL_TEXTURE_2D, texture_home_door);
    glTranslatef(0.0f, -0.2f, 0.001f);
    glScalef(0.2f, 0.8f, 1.0f);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);

    glPushMatrix();
    // RIGHT FACE
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    glPopMatrix();
    // RIGHT FACE Traigle
    glPushMatrix();

    glTranslatef(1.0001f, 1.2f, 0.0);
    glScalef(1.0f, 0.25f, 1.1f);

    glBegin(GL_TRIANGLES);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 1.0f);
    glEnd();

    // LEFT FACE Traigle
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-1.001f, 1.2f, 0.0);
    glScalef(1.0f, 0.25f, 1.1f);

    glBegin(GL_TRIANGLES);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    // BACK FACE
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    // LEFT FACE
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    // TOP
    // front side
    glPushMatrix();

    glTranslatef(0.0f, 0.19f, 0.45f);
    glScalef(1.2f, 1.0f, 0.8f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_home_Roof);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 10.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // back side
    glTranslatef(0.0f, 0.19f, -0.45f);
    glScalef(1.2f, 1.0f, 0.8f);
    glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_home_Roof);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture_home_WhiteWallpaper);
    // BOTTOM FACE
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
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
    // bottom base , ground
    glPushMatrix();

    glTranslatef(0.0f, -1.31f, 0.0f);
    glScalef(1.5f, 0.3f, 1.5f);
    //    colorSetcolor(160, 82, 45);
    //    glColor3f(222.0f / 255.0f, 184.0f / 255.0f, 135.0f / 255.0f);
    glBindTexture(GL_TEXTURE_2D, texture_home_basebricks);
    DrawBox();
    glBindTexture(GL_TEXTURE_2D, 0);
    // colorSetcolor(1, 1, 1);
    glColor3f(1.0f, 1.0f, 1.0f);

    // stairs
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, -1.31f, 1.5f);
    glScalef(0.3f, 0.299f, 0.2f);

    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);
    DrawBox();
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, -1.36f, 1.7f);
    glScalef(0.3f, 0.24f, 0.2f);

    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);
    DrawBox();
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, -1.41f, 1.9f);
    glScalef(0.3f, 0.19f, 0.2f);

    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);
    DrawBox();
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

void DrawCylinder(GLfloat base, GLfloat top, GLfloat height)
{
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    gluQuadricTexture(quadric_S2, GL_TRUE);
    gluCylinder(quadric_S2, base, top, height, 10, 10);
    glPopMatrix();
}

void DrawBox(void)
{

    glBegin(GL_QUADS);
    // FRONT FACE
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    // RIGHT FACE
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // BACK FACE
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    // LEFT FACE
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    // TOP
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    // BOTTOM FACE

    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}

void drawTable(void)
{
    // variable declartions
    void DrawBox(void);
    glDisable(GL_LIGHTING);

    // code
    glPushMatrix();

    // Base
    glScalef(3.0f, 0.10f, 2.0f);
    glBindTexture(GL_TEXTURE_2D, texture_trunk);
    DrawBox();

    // Left Base Support
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.8f, -0.20f, 0.0f);
    glScalef(0.15f, 0.10f, 2.0f);

    DrawBox();

    // Rigth Base Support
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.8f, -0.20f, 0.0f);
    glScalef(0.15f, 0.10f, 2.0f);

    DrawBox();

    // Middle Base Supprt
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, -0.20f, 0.0f);
    glScalef(3.0f, 0.10f, 0.10f);
    DrawBox();

    // front-Left Leg
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.8f, -1.5001f, 1.8f);
    glScalef(0.10f, 1.6f, 0.10f);
    DrawBox();

    // back-Left Leg
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.8f, -1.5001f, -1.8f);
    glScalef(0.10f, 1.6f, 0.10f);
    DrawBox();

    // front-Right Leg
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.8f, -1.5001f, 1.8f);
    glScalef(0.10f, 1.6f, 0.10f);
    DrawBox();

    // Back-Right Leg
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.8f, -1.5001f, -1.8f);
    glScalef(0.10f, 1.6f, 0.10f);
    DrawBox();

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawBook(void)
{
    // variable declartions
    void DrawBox(void);
    void drawPageQuad(void);
    void DrawGeetramayan(void);

    // code
    glPushMatrix();

    // left side pages Base
    glTranslatef(0.8f, 0.0f, 0.0f);
    glRotatef(10.0f, 0.0f, 0.0f, 1.0f);

    glScalef(0.9f, 0.18f, 1.5f);

    DrawGeetramayan();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.0f, 0.085f, 0.0f);
    glRotatef(anglePageOne, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.8f, 0.0f, 0.0f);
    glScalef(0.9f, 0.1f, 1.5f);

    DrawGeetramayan();

    glPopMatrix();

    glPushMatrix();
    // Page No 1

    glTranslatef(0.0f, 0.085f, 0.0f);
    glRotatef(anglePageOne, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.8f, 0.0f, 0.0f);
    glScalef(0.85f, 0.18f, 1.5f);

    glBindTexture(GL_TEXTURE_2D, texture_blank_book_Page);
    drawPageQuad();

    glPopMatrix();

    glPushMatrix();
    // Page No 5

    glTranslatef(0.0f, 0.085f, 0.0f);
    glRotatef(anglePageFive, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.8f, 0.0f, 0.0f);
    glScalef(0.85f, 0.18f, 1.5f);

    glBindTexture(GL_TEXTURE_2D, texture_book);
    drawPageQuad();

    glPopMatrix();
}

void drawPageQuad(void)
{
    glPushMatrix();

    glBegin(GL_QUADS);
    // FRONT FACE
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glEnd();

    glPopMatrix();
}

void drawLake(void)
{
    glBindTexture(GL_TEXTURE_2D, texture_water);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void DisableAllLights()
{
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
}

void drawRoomWithoutInterior(void)
{
    // funnction declartions
    void drawHomeWithoutInterior(void);

    // code
    glTranslatef(0.0f, -2.3f, 0.0f);
    glScalef(3.0f, 1.3f, 2.25f);
    drawHomeWithoutInterior();
}

void drawHomeWithoutInterior(void)
{

    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);

    // FRONT FACE
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glPushMatrix();
    // window Right
    glBindTexture(GL_TEXTURE_2D, texture_home_window);
    glTranslatef(0.65f, 0.0f, 0.001f);
    glScalef(0.15f, 0.4f, 1.0001f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // window Left
    glTranslatef(-0.65f, 0.0f, 0.001f);
    glScalef(0.15f, 0.4f, 1.00001f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // Dooor
    glBindTexture(GL_TEXTURE_2D, texture_home_door);
    glTranslatef(0.0f, -0.2f, 0.001f);
    glScalef(0.2f, 0.8f, 1.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);

    // RIGHT FACE
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // RIGHT FACE Traigle
    glPushMatrix();

    glTranslatef(1.0001f, 1.2f, 0.0);
    glScalef(1.0f, 0.25f, 1.1f);

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 1.0f);
    glEnd();

    // LEFT FACE Traigle
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-1.001f, 1.2f, 0.0);
    glScalef(1.0f, 0.25f, 1.1f);

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    // BACK FACE
    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // LEFT FACE
    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    // TOP
    // front side
    glPushMatrix();

    glTranslatef(0.0f, 0.19f, 0.45f);
    glScalef(1.0f, 1.0f, 0.8f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_home_Roof);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // back side
    glTranslatef(0.0f, 0.19f, -0.45f);
    glScalef(1.0f, 1.0f, 0.8f);
    glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_home_Roof);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, texture_home_WhiteWallpaper);
    // BOTTOM FACE
    glBegin(GL_QUADS);
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
}

void drawTree(GLfloat base, GLfloat top, GLfloat height)
{
    void Cylinder(GLfloat, GLfloat, GLfloat);

    if (isTrunk == TRUE)
    {

        Cylinder(base, top, height);

        isTrunk = FALSE;
    }
    else
    {
        if (height <= 2.0f)
        {
            glEnable(GL_BLEND);
            glBindTexture(GL_TEXTURE_2D, texture_leaf);
            glBegin(GL_QUADS);
            glColor3f(0.0f, 0.6f, 0.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.5f, 0.5f, 0.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-0.5f, 0.5f, 0.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, 0.0f);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.5f, -0.5f, 0.0f);
            glColor3f(1.0f, 1.0f, 1.0f);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_BLEND);
        }
        else
        {
            Cylinder(base, top, height);
        }
    }

    if (height <= 1.0f)
    {

        return;
    }
    else
    {
        glPushMatrix();
        glTranslatef(0.0f, height, 0.0f);
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
        drawTree(top, base * 0.5f, height * 0.6f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, height * 0.7f, 0.0f);
        glRotatef(45.0f, 0.0f, 0.0f, -1.0f);
        drawTree(top, base * 0.5f, height * 0.6f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, height * 0.9f, 0.0f);
        glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
        drawTree(top, base * 0.5f, height * 0.6f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, height * 0.5f, 0.0f);
        glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
        drawTree(top, base * 0.5f, height * 0.6f);
        glPopMatrix();
    }
}

void Cylinder(GLfloat base, GLfloat top, GLfloat height)
{
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture_trunk);
    gluQuadricTexture(quadric_S2Cylinder, GL_TRUE);
    gluCylinder(quadric_S2Cylinder, base, top, height, 10, 10);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void drawKhurchi(void)
{
    glBindTexture(GL_TEXTURE_2D, texture_trunk);
    // left leg front
    glPushMatrix();
    {
        glTranslatef(-0.8f, -0.5f, 0.9f);
        glScalef(0.1f, 0.8f, 0.1f);
        DrawBox();
    }
    glPopMatrix();

    // Right leg front
    glPushMatrix();
    {
        glTranslatef(0.8f, -0.5f, 0.9f);
        glScalef(0.1f, 0.8f, 0.1f);
        DrawBox();
    }
    glPopMatrix();

    // left leg back
    glPushMatrix();
    {
        glTranslatef(-0.8f, -0.5f, -0.9f);
        glScalef(0.1f, 0.8f, 0.1f);
        DrawBox();
    }
    glPopMatrix();

    // Right leg back
    glPushMatrix();
    {
        glTranslatef(0.8f, -0.5f, -0.9f);
        glScalef(0.1f, 0.8f, 0.1f);
        DrawBox();
    }
    glPopMatrix();

    // khurchi Base
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.4f, 0.0f);
        glScalef(1.0f, 0.1f, 1.0f);
        DrawBox();
    }
    glPopMatrix();

    // Back Base
    glPushMatrix();
    {
        glRotatef(5.0f, -1.0f, 0.0f, 0.0f);
        // top line
        glPushMatrix();
        {
            glTranslatef(0.0f, 2.5f, -0.8f);
            glScalef(1.0f, 0.1f, 0.1f);
            DrawBox();
        }
        glPopMatrix();

        // top line
        glPushMatrix();
        {
            glTranslatef(0.0f, 2.0f, -0.8f);
            glScalef(1.0f, 0.1f, 0.1f);
            DrawBox();
        }
        glPopMatrix();
        // top line
        glPushMatrix();
        {
            glTranslatef(0.0f, 1.5f, -0.8f);
            glScalef(1.0f, 0.1f, 0.1f);
            DrawBox();
        }
        glPopMatrix();

        // top line
        glPushMatrix();
        {
            glTranslatef(0.0f, 1.0f, -0.8f);
            glScalef(1.0f, 0.1f, 0.1f);
            DrawBox();
        }
        glPopMatrix();

        // back left vertical line
        glPushMatrix();
        {
            glTranslatef(-0.9f, 1.5f, -0.8f);
            glScalef(0.1f, 1.2f, 0.1f);
            DrawBox();
        }
        glPopMatrix();

        // back Right vertical line
        glPushMatrix();
        {
            glTranslatef(0.9f, 1.5f, -0.8f);
            glScalef(0.1f, 1.2f, 0.1f);
            DrawBox();
        }
        glPopMatrix();
    }
    glPopMatrix();

    // left line
    glPushMatrix();
    {
        glTranslatef(-0.9f, 1.6f, 0.0f);
        glScalef(0.1f, 0.1f, 1.0f);
        DrawBox();
    }
    glPopMatrix();

    // Right line
    glPushMatrix();
    {
        glTranslatef(0.9f, 1.6f, 0.0f);
        glScalef(0.1f, 0.1f, 1.0f);
        DrawBox();
    }
    glPopMatrix();

    // front left vertical line
    glPushMatrix();
    {
        glTranslatef(-0.9f, 1.0f, 0.8f);
        glScalef(0.09f, 0.5f, 0.09f);
        DrawBox();
    }
    glPopMatrix();

    // front Right vertical line
    glPushMatrix();
    {
        glTranslatef(0.9f, 1.0f, 0.8f);
        glScalef(0.09f, 0.5f, 0.09f);
        DrawBox();
    }
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawChourang(void)
{
    // Upper box
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.2f, 0.0f);
        glScalef(1.2f, 0.05f, 1.2f);
        DrawBox();
    }
    glPopMatrix();

    // bottom box
    glPushMatrix();
    {
        glTranslatef(0.0f, -0.1f, 0.0f);
        glScalef(1.2f, 0.05f, 1.2f);
        DrawBox();
    }
    glPopMatrix();

    // Middle Box
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.05f, 0.0f);
        glScalef(1.0f, 0.15f, 1.0f);
        DrawBox();
    }
    glPopMatrix();

    // front left leg
    glPushMatrix();
    {
        glTranslatef(-0.9f, -0.5f, 0.7f);
        glScalef(0.15f, 0.4f, 0.15f);
        DrawBox();
    }
    glPopMatrix();

    // Front Right leg
    glPushMatrix();
    {
        glTranslatef(0.9f, -0.5f, 0.7f);
        glScalef(0.15f, 0.4f, 0.15f);
        DrawBox();
    }
    glPopMatrix();

    // Back left leg
    glPushMatrix();
    {
        glTranslatef(-0.9f, -0.5f, -0.7f);
        glScalef(0.15f, 0.4f, 0.15f);
        DrawBox();
    }
    glPopMatrix();

    // Back Right leg
    glPushMatrix();
    {
        glTranslatef(0.9f, -0.5f, -0.7f);
        glScalef(0.15f, 0.4f, 0.15f);
        DrawBox();
    }
    glPopMatrix();
}

void drawPat(void)
{
    void drawChourang(void);

    glScalef(1.3f, 0.5f, 0.8f);
    glPushMatrix();
    {
        drawChourang();
    }
    glPopMatrix();
}

void DrawGeetramayan(void)
{
    glBindTexture(GL_TEXTURE_2D, texture_blank_book_Page);
    glBegin(GL_QUADS);
    // FRONT FACE
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // RIGHT FACE

    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // BACK FACE
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    // LEFT FACE
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    // TOP
    glBindTexture(GL_TEXTURE_2D, texture_book_Cover);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // BOTTOM FACE
    glBindTexture(GL_TEXTURE_2D, texture_blank_book_Page);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
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
}
