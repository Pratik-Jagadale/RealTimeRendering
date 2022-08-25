/* Header Files */
#include <windows.h>
#include "OGLSCENE3.h"
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "std_image.h"

#include "helper_timer.h"

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

#include "Common.h"
extern BOOL bScene03Done;

// global variable declarations
extern HWND ghwnd;

extern FILE *gpFile; // FILE* -> #include<stdio.h>
extern BOOL gbLight;

GLUquadric *quadricDisk_S3 = NULL;
GLUquadric *quadricCylinder_S3 = NULL;
GLUquadric *quadric = NULL;

GLfloat gfLightAmbiant_Four_S3[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat gfLightDeffuse_Four_S3[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfLightSpicular_Four_S3[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfLightPositions_Four_S3[] = {1.0f, 1.0f, 1.0f, 1.0f};

// white
GLfloat gfMaterialAmbiant_Four_S3[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat gfMeterialDeffuse_Four_S3[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfMateralSpecular_Four_S3[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfMaterialShineeness_Four_S3 = 50.0f;

GLuint texture_mahal_main_base;
GLuint texture_mahal_red_designe_carpet;
GLuint texture_mahal_red_plan_carpet;
GLuint texture_mahal_blue_carpet_yagya;
GLuint texture_red_circle_design;
GLuint texture_mahal_purple;
GLuint texture_blue_curtain;
GLuint texture_flower;

// humal models textures
int shoulder = 0.0f;
int elbow = 0.0f;
int wrist = 0.0f;
GLuint texture_face;
GLuint texture_red_cloth;
GLuint texture_skin;
GLuint texture_face_King;
GLuint texture_face_Queen;
GLuint texture_crown_1;
GLuint texture_crown_2;
GLuint texture_org_cloth;
GLuint texture_face_hrushi;
GLuint texture_bowl;
GLuint texture_mahal_Wall;
GLuint texture_mahal_Door;
GLuint texture_mahal_ceiling;

#define OTHER 0
#define SIDEWALLS 1
#define FRONTANDBACKWALSS 2
int iSWallsTextrue = OTHER;

void SetLightColorGold(void);
void SetLightColorWhite(void);
void SetLightColorSilver(void);
void SetLightColorRed(void);
void SetLightColorCyan(void);
void SetLightColorGreen(void);
void drawKing(void);
void drawQueen(void);
void drawAgniDev(void);
void drawCrown(GLuint);
void drawHrushi(void);
void drawBowl(void);

/////////////////////////////////Fire Related varibles/////////////////
#define MAX_PARTICLES 500

float slowdown = 10.0f; // slow down particles
GLuint col = 0;

typedef struct
{
    bool avtive; // active (y / n)
    float life;  // Particle Life
    float fade;  // Fade shapes
    float r;     // red component
    float g;     // green component
    float b;     // blue component
    float x;     // X Position
    float y;     // Y Position
    float z;     // Z Position
    float xi;    // X Direction
    float yi;    // Y Direction
    float zi;    // Z Direction
    float xg;    // X Gravity
    float yg;    // y Gravity
    float zg;    // z Gravity

} PARTICLE; // Particle System

PARTICLE particle[MAX_PARTICLES]; // Particlr array(Room for Particle)
PARTICLE particleOne[MAX_PARTICLES];
PARTICLE particleTwo[MAX_PARTICLES];

static GLfloat colors[12][3] =
    {
        {1.0f, 0.5f, 0.5f}, {1.0f, 0.75f, 0.5f}, {1.0f, 1.0f, 0.5f}, {0.75f, 1.0f, 0.5f}, {0.5f, 1.0f, 0.5f}, {0.5f, 1.0f, 0.75f}, {0.5f, 1.0f, 1.0f}, {0.5f, 0.75f, 1.0f}, {0.5f, 0.5f, 1.0f}, {0.75f, 0.5f, 1.0f}, {1.0f, 0.5f, 1.0f}, {1.0f, 0.5f, 0.75f}};

GLuint texture_fire;
GLuint texture_rangoli_one;
GLuint texture_rangoli_two;
/*
BOOL LoadGLPNGTexture(GLuint *texture, char *image);
*/
GLfloat CameraVector_S3[3] = {-1.999f, 4.68999f, 8.2100f};
GLfloat EyeVector_S3[3] = {1.75f, 1.00f, -2.1157f};
GLfloat UpVector_S3[3] = {0.0f, 1.0f, 0.0f};

GLfloat bowlPosition[] = {0.0f, 5.0f, 4.0f};

////////////////////////////////////////////////////////
// Timer realted things

StopWatchInterface *pTimer_S3;

#define SCENE_01_DURATION_S3 15 // 10
#define SCENE_02_DURATION_S3 15
#define SCENE_03_DURATION_S3 15
#define SCENE_04_DURATION_S3 15
#define SCENE_05_DURATION_S3 15

unsigned int sceneNumber_S3 = 1; // when program begins we are showing the first texture by default hence '1'
float elapsedTime_S3;

GLfloat AganiDevSize = 0.5f;
GLfloat alpha_AgniDev = 1.0f;
GLfloat bEnableAgniDevAlph = FALSE;

BOOL initializeSceneThree(void)
{
    /* fucntion delcations */
    BOOL LoadGLTexture(GLuint *, TCHAR[]);
    void InitializeFireParticle(PARTICLE *);

    /* variable declartions */
    // quadric intiallize
    // create quadric.
    quadricDisk_S3 = gluNewQuadric();

    quadricCylinder_S3 = gluNewQuadric();

    quadric = gluNewQuadric();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // textures

    if (LoadGLTexture(&texture_mahal_main_base, MAKEINTRESOURCE(IDBITMAP_MAHAL_BASE_YELLOW_CARPET)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_mahal_red_designe_carpet, MAKEINTRESOURCE(IDBITMAP_MAHAL_RED_DESIGNE_CARPET)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_mahal_red_plan_carpet, MAKEINTRESOURCE(IDBITMAP_MAHAL_RED_PLAN_CARPET)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_mahal_blue_carpet_yagya, MAKEINTRESOURCE(IDBITMAP_MAHAL_BLUE_CARPET_YAGYA)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_red_circle_design, MAKEINTRESOURCE(IDBITMAP_MAHAL_RED_CIRCLE_DESIGNE)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_mahal_purple, MAKEINTRESOURCE(IDBITMAP_MAHAL_BLUE_DESIGN)) == FALSE)
        return FALSE;

    // gayga
    // initializing the  particle struct
    InitializeFireParticle(particle);

    if (LoadGLTexture(&texture_rangoli_one, MAKEINTRESOURCE(IDBITMAP_RANGOLI_ONE)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_rangoli_two, MAKEINTRESOURCE(IDBITMAP_RANGOLI_TWO)) == FALSE)
        return FALSE;

    if (LoadGLPNGTexture(&texture_fire, "./Models/texture/png.png") == FALSE)
        return FALSE;

    if (LoadGLPNGTexture(&texture_blue_curtain, "./Models/texture/png_blue_curtain.png") == FALSE)
        return FALSE;

    if (LoadGLPNGTexture(&texture_flower, "./Models/texture/texture_flower_png.png") == FALSE)
        return FALSE;

    // humal model textures
    if (LoadGLTexture(&texture_face, MAKEINTRESOURCE(IDBITMAP_FACE)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_red_cloth, MAKEINTRESOURCE(IDBITMAP_RED_CLOTH)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_skin, MAKEINTRESOURCE(IDBITMAP_SKIN)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_face_King, MAKEINTRESOURCE(IDBITMAP_FACE_KING)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_face_Queen, MAKEINTRESOURCE(IDBITMAP_FACE_QUEEN)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_org_cloth, MAKEINTRESOURCE(IDBITMAP_ORG_CLOTH)) == FALSE)
        return FALSE;

    if (LoadGLTexture(&texture_face_hrushi, MAKEINTRESOURCE(IDBITMAP_FACE_HRUSHI)) == FALSE)
        return FALSE;

    if (LoadGLPNGTexture(&texture_crown_1, "./Models/texture/texture_crown_1.png") == FALSE)
        return FALSE;

    if (LoadGLPNGTexture(&texture_crown_2, "./Models/texture/texture_crown_2.png") == FALSE)
        return FALSE;

    if (LoadGLPNGTexture(&texture_bowl, "./Models/texture/texture_bowl.png") == FALSE)
        return FALSE;

    if (LoadGLPNGTexture(&texture_mahal_Wall, "./Models/texture/texture_Mahal_Wall.png") == FALSE)
        return FALSE;

    if (LoadGLPNGTexture(&texture_mahal_Door, "./Models/texture/texture_mahal_Door.png") == FALSE)
        return FALSE;

    if (LoadGLPNGTexture(&texture_mahal_ceiling, "./Models/texture/texture_mahal_ceiling.png") == FALSE)
        return FALSE;

    glEnable(GL_TEXTURE_2D);

    // Creating the timer
    sdkCreateTimer(&pTimer_S3);

    return TRUE;
}

BOOL bInItDisplayDone = FALSE;

void callOncewhenDispayGetCalled(void)
{
    // Start them music here
    sdkStartTimer(&pTimer_S3);

    // light related
    glLightfv(GL_LIGHT4, GL_AMBIENT, gfLightAmbiant_Four_S3);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, gfLightDeffuse_Four_S3);
    glLightfv(GL_LIGHT4, GL_SPECULAR, gfLightSpicular_Four_S3);
    glLightfv(GL_LIGHT4, GL_POSITION, gfLightPositions_Four_S3);
    glEnable(GL_LIGHT4);

    glMaterialfv(GL_FRONT, GL_AMBIENT, gfMaterialAmbiant_Four_S3);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gfMeterialDeffuse_Four_S3);
    glMaterialfv(GL_FRONT, GL_SPECULAR, gfMateralSpecular_Four_S3);
    glMaterialf(GL_FRONT, GL_SHININESS, gfMaterialShineeness_Four_S3);

    glEnable(GL_LIGHTING);
}

void displaySceneThree(void)
{
    // function prototype
    void drawRajwadaBase(void);
    void fadeInFirstScene_S3(void);

    void fadeInFourthScene(void);
    void fadeOutFourthScene(void);

    static BOOL bScene01Done_S3 = FALSE;
    static BOOL bScene02Done_S3 = FALSE;
    static BOOL bScene03Done_S3 = FALSE;
    static BOOL bScene04Done_S3 = FALSE;
    static BOOL bScene05Done_S3 = FALSE;
    static BOOL bScene06Done_S3 = FALSE;

    /* Code */

    if (bInItDisplayDone == FALSE)
    {
        callOncewhenDispayGetCalled();
        bInItDisplayDone = TRUE;
    }

    elapsedTime_S3 = sdkGetTimerValue(&pTimer_S3); // THIS FUNCTION GIVES TIME IN MILLLIESECONDS
    elapsedTime_S3 = elapsedTime_S3 / 1000.0f;     // CONVERTIN TIME MILLISECONDS TO SECONDS
                                                   /*
                                                       TCHAR str[200];
                                                       swprintf_s(str, 200, TEXT("TIME = %4.2f ,CameraVector_S3[0] = %f, CameraVector_S3[1] = %f ,CameraVector_S3[2] = %f , EyeVector_S3[0] = %f, EyeVector_S3[1] = %f , EyeVector_S3[2] = %f \n"), elapsedTime_S3, CameraVector_S3[0], CameraVector_S3[1], CameraVector_S3[2], EyeVector_S3[0], EyeVector_S3[1], EyeVector_S3[2]); // "_s" for secure
                                               
                                                       SetWindowText(ghwnd, str);
                                                   */
    if (bScene01Done_S3 == FALSE)
    {

        if (elapsedTime_S3 > SCENE_01_DURATION_S3)
        {
            sceneNumber_S3++;
            sdkResetTimer(&pTimer_S3);
            bScene01Done_S3 = TRUE;

            CameraVector_S3[0] = 7.0f;
            CameraVector_S3[1] = 8.62f;
            CameraVector_S3[2] = -10.16f;

            EyeVector_S3[0] = 0.0f;
            EyeVector_S3[1] = 2.5f;
            EyeVector_S3[2] = 5.843224f;
        }
    }
    else if (bScene02Done_S3 == FALSE)
    {

        if (elapsedTime_S3 > SCENE_02_DURATION_S3)
        {
            sceneNumber_S3++;
            sdkResetTimer(&pTimer_S3);
            bScene02Done_S3 = TRUE;
        }
    }
    else if (bScene03Done_S3 == FALSE)
    {

        if (elapsedTime_S3 > SCENE_03_DURATION_S3)
        {
            sceneNumber_S3++;
            sdkResetTimer(&pTimer_S3);
            bScene03Done_S3 = TRUE;
        }
    }
    else if (bScene04Done_S3 == FALSE)
    {

        if (elapsedTime_S3 > SCENE_04_DURATION_S3)
        {
            sceneNumber_S3++;
            sdkResetTimer(&pTimer_S3);
            bScene04Done_S3 = TRUE;
        }
    }
    else if (bScene05Done_S3 == FALSE)
    {

        if (elapsedTime_S3 > SCENE_05_DURATION_S3)
        {
            sceneNumber_S3++;
            sdkResetTimer(&pTimer_S3);
            bScene05Done_S3 = TRUE;
            bScene03Done = FALSE;
        }
    }

    gluLookAt(CameraVector_S3[0], CameraVector_S3[1], CameraVector_S3[2],
              EyeVector_S3[0], EyeVector_S3[1], EyeVector_S3[2],
              0.0f, 1.0f, 0.0f);

    drawRajwadaBase();

    glDisable(GL_LIGHTING);
    if (sceneNumber_S3 == 1)
    {
        fadeInFirstScene_S3();
    }
    else if (sceneNumber_S3 == 5 && elapsedTime_S3 > 10.0f)
    {
        fadeOutFourthScene();
    }
    else if (sceneNumber_S3 == 5)
    {
        fadeInFourthScene();
    }
    else if (sceneNumber_S3 > 5)
    {
    }

    glEnable(GL_LIGHTING);
}

void cubeforfadeinfadeout_S3(void)
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

void fadeInFirstScene_S3(void)
{
    static float alpha1 = 1.0f;

    alpha1 = alpha1 - 0.3f;

    glPushMatrix();

    glTranslatef(CameraVector_S3[0], CameraVector_S3[1], CameraVector_S3[2] + 0.5f);

    glScalef(2.0f, 2.0f, 2.0f);

    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha1);
    cubeforfadeinfadeout_S3();

    glPopMatrix();
    glDisable(GL_BLEND);
}

void fadeInFourthScene(void)
{
    static float alpha1 = 1.0f;

    alpha1 = alpha1 - 0.02f;

    glPushMatrix();

    glTranslatef(CameraVector_S3[0], CameraVector_S3[1], CameraVector_S3[2] + 0.5f);

    glScalef(2.0f, 2.0f, 2.0f);

    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha1);
    cubeforfadeinfadeout_S3();

    glPopMatrix();
    glDisable(GL_BLEND);
}

void fadeOutFourthScene(void)
{
    static float alpha1 = 0.0f;

    alpha1 = alpha1 + 0.02f;

    glPushMatrix();

    glTranslatef(CameraVector_S3[0], CameraVector_S3[1], CameraVector_S3[2] - 0.5f);

    glScalef(2.0f, 2.0f, 2.0f);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha1);
    cubeforfadeinfadeout_S3();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

void updateSceneThree(void)
{
    /* code */
    void drawParticleAnimation(void);

    /* code */
    drawParticleAnimation();

    if (sceneNumber_S3 == 1)
    {
        if (CameraVector_S3[2] > 10.0f && AganiDevSize < 1.01f)
            AganiDevSize = AganiDevSize + 0.01f;

        if (CameraVector_S3[1] < 9.0f)
            CameraVector_S3[1] = CameraVector_S3[1] + 0.017f;

        if (CameraVector_S3[2] < 16.0f)
            CameraVector_S3[2] = CameraVector_S3[2] + 0.028f;
    }
    else if (sceneNumber_S3 == 2)
    {
        if (CameraVector_S3[0] > -7.0f)
            CameraVector_S3[0] = CameraVector_S3[0] - 0.02f;
    }
    else if (sceneNumber_S3 == 3)
    {

        if (CameraVector_S3[2] < 19.24)
            CameraVector_S3[2] = CameraVector_S3[2] + 0.05f;
    }
    else if (sceneNumber_S3 == 4)
    {
        if (CameraVector_S3[0] < 7.0f)
        {
            CameraVector_S3[0] = CameraVector_S3[0] + 0.04f;
            CameraVector_S3[2] = CameraVector_S3[2] - 0.002f;
        }

        if (bowlPosition[2] < 7.5f)
            bowlPosition[2] = bowlPosition[2] + 0.01f;
        bowlPosition[1] = bowlPosition[1] - 0.002f;
    }
    else if (sceneNumber_S3 == 5)
    {
        CameraVector_S3[0] = 8.939990f;
        CameraVector_S3[1] = 3.420002;
        CameraVector_S3[2] = 18.3556f;

        EyeVector_S3[0] = 1.3;
        EyeVector_S3[1] = 3.29f;
        EyeVector_S3[2] = 5.843f;

        if (elapsedTime_S3 > 6)
        {
            bEnableAgniDevAlph = TRUE;
            alpha_AgniDev = alpha_AgniDev - 0.01f;
        }
    }
}

void uninitialize_S3(void)
{

    /* code */

    /*  */
    // humal Model Related uninitializations
    if (texture_mahal_ceiling)
    {
        glDeleteTextures(1, &texture_mahal_ceiling);
        texture_mahal_ceiling = NULL;
    }

    if (texture_mahal_Door)
    {
        glDeleteTextures(1, &texture_mahal_Door);
        texture_mahal_Door = NULL;
    }

    if (texture_mahal_Wall)
    {
        glDeleteTextures(1, &texture_mahal_Wall);
        texture_mahal_Wall = NULL;
    }

    if (texture_face_hrushi)
    {
        glDeleteTextures(1, &texture_face_hrushi);
        texture_face_hrushi = NULL;
    }

    if (texture_org_cloth)
    {
        glDeleteTextures(1, &texture_org_cloth);
        texture_org_cloth = NULL;
    }

    if (texture_crown_1)
    {
        glDeleteTextures(1, &texture_crown_1);
        texture_crown_1 = NULL;
    }

    if (texture_crown_2)
    {
        glDeleteTextures(1, &texture_crown_2);
        texture_crown_2 = NULL;
    }

    if (texture_face_King)
    {
        glDeleteTextures(1, &texture_face_King);
        texture_face_King = NULL;
    }

    if (texture_face_Queen)
    {
        glDeleteTextures(1, &texture_face_Queen);
        texture_face_Queen = NULL;
    }

    if (texture_skin)
    {
        glDeleteTextures(1, &texture_skin);
        texture_skin = NULL;
    }

    if (texture_red_cloth)
    {
        glDeleteTextures(1, &texture_red_cloth);
        texture_red_cloth = NULL;
    }

    if (texture_bowl)
    {
        glDeleteTextures(1, &texture_bowl);
        texture_bowl = NULL;
    }

    // Mahal related uninilizations
    if (texture_flower)
    {
        glDeleteTextures(1, &texture_flower);
        texture_flower = 0;
    }

    if (texture_blue_curtain)
    {
        glDeleteTextures(1, &texture_blue_curtain);
        texture_blue_curtain = 0;
    }

    if (texture_fire)
    {
        glDeleteTextures(1, &texture_fire);
        texture_fire = 0;
    }

    if (texture_rangoli_one)
    {
        glDeleteTextures(1, &texture_rangoli_one);
        texture_rangoli_one = 0;
    }

    if (texture_rangoli_two)
    {
        glDeleteTextures(1, &texture_rangoli_two);
        texture_rangoli_two = 0;
    }

    if (texture_mahal_purple)
    {
        glDeleteTextures(1, &texture_mahal_purple);
        texture_mahal_purple = NULL;
    }

    if (texture_red_circle_design)
    {
        glDeleteTextures(1, &texture_red_circle_design);
        texture_red_circle_design = NULL;
    }

    if (texture_mahal_blue_carpet_yagya)
    {
        glDeleteTextures(1, &texture_mahal_blue_carpet_yagya);
        texture_mahal_blue_carpet_yagya = NULL;
    }

    if (texture_mahal_main_base)
    {
        glDeleteTextures(1, &texture_mahal_main_base);
        texture_mahal_main_base = NULL;
    }

    if (texture_mahal_red_designe_carpet)
    {
        glDeleteTextures(1, &texture_mahal_red_designe_carpet);
        texture_mahal_red_designe_carpet = NULL;
    }

    if (texture_mahal_red_plan_carpet)
    {
        glDeleteTextures(1, &texture_mahal_red_plan_carpet);
        texture_mahal_red_plan_carpet = NULL;
    }

    if (quadric)
    {
        gluDeleteQuadric(quadric);
        quadric = NULL;
    }

    if (quadricCylinder_S3)
    {
        gluDeleteQuadric(quadricCylinder_S3);
        quadricCylinder_S3 = NULL;
    }

    if (quadricDisk_S3)
    {
        gluDeleteQuadric(quadricDisk_S3);
        quadricDisk_S3 = NULL;
    }
}

void drawRajwadaBase(void)
{
    // variable declartions
    void S3_DrawBoxWithParam(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);
    void smallChair(void);
    void drawPolls(void);
    void kingChair(void);
    void Yagya(void);
    void drawCurtainAndFlower(void);
    void drawSideWalls(void);

    // code
    // changing materail color to white
    SetLightColorWhite();

    // ALL MAHAL DESGINE
    glPushMatrix();
    {
        // main Base
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
        S3_DrawBoxWithParam(0.3f, 0.3f, 25.0f, 25.0f, 40.0f, 40.0f);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
        // Main Middle Base
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, texture_mahal_blue_carpet_yagya);
        glTranslatef(0.0f, 0.3f, 3.0f);
        S3_DrawBoxWithParam(0.05f, 0.05f, 10.0f, 10.0f, 15.0f, 15.0f);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
        // Right Side Base
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
        glTranslatef(14.0f, 0.3f, 0.0f);
        S3_DrawBoxWithParam(0.3f, 0.3f, 6.0f, 6.0f, 40.0f, 40.0f);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
        // Left Side Base
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
        glTranslatef(-14.0f, 0.3f, 0.0f);
        S3_DrawBoxWithParam(0.3f, 0.3f, 6.0f, 6.0f, 40.0f, 40.0f);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
        // Base for carpet
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, texture_mahal_red_designe_carpet);
        glTranslatef(0.0f, 0.30f, 0.0f);
        S3_DrawBoxWithParam(0.03f, 0.03f, 4.0f, 4.0f, 40.0f, 40.0f);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
        {
            // Strairs Carpet
            // Base Stage bottom Strairs Carpet  - 3
            glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture_mahal_red_designe_carpet);
            glTranslatef(0.0f, 0.35f, -11.95f);
            S3_DrawBoxWithParam(1.0f, 1.0f, 4.0f, 4.0f, 13.0f, 13.0f);

            glPopMatrix();
            // Base Stage bottom Strairs Carpet - 2
            glPushMatrix();

            glTranslatef(0.0f, 0.65f, -11.95f);
            S3_DrawBoxWithParam(1.0f, 1.0f, 4.0f, 4.0f, 11.5f, 11.5f);

            glPopMatrix();
            // Base Stage bottom Strairs Carpet  - 1
            glPushMatrix();

            glTranslatef(0.0f, 0.95f, -11.95f);
            S3_DrawBoxWithParam(1.0f, 1.0f, 4.0f, 4.0f, 9.0f, 9.0f);
            glBindTexture(GL_TEXTURE_2D, 0);
            glPopMatrix();
        }
        // Base for Yagya
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, texture_red_circle_design);
        glTranslatef(0.0f, 0.3f, 3.0f);
        S3_DrawBoxWithParam(0.8f, 0.8f, 5.0f, 5.0f, 5.0f, 5.0f);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
        // Base Stage bottom  - 3
        glPushMatrix();

        SetLightColorCyan();
        glTranslatef(0.0f, 0.3f, -12.0f);
        S3_DrawBoxWithParam(1.0f, 1.0f, 17.0f, 15.0f, 13.0f, 13.0f);

        glPopMatrix();
        // Base Stage bottom  - 2
        glPushMatrix();

        glTranslatef(0.0f, 0.6f, -12.0f);
        S3_DrawBoxWithParam(1.0f, 1.0f, 16.0f, 14.0f, 11.5f, 11.5f);

        glPopMatrix();
        // Base Stage bottom  - 1
        glPushMatrix();

        glTranslatef(0.0f, 0.9f, -12.0f);
        S3_DrawBoxWithParam(1.0f, 1.0f, 15.0f, 13.0f, 9.0f, 9.0f);
        SetLightColorWhite();

        glPopMatrix();
        // Base Stage top  - 1
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, texture_mahal_purple);
        glTranslatef(0.0f, 1.2f, -12.0f);
        S3_DrawBoxWithParam(1.0f, 1.0f, 14.0f, 12.0f, 7.5f, 7.5f);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();

        // Wall & Door & Ceiling
        drawSideWalls();

        // Base for Small Chairs
        {
            glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
            // Right 1
            glPushMatrix();

            glTranslatef(8.0f, 0.3f, -2.0f);
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);

            glPopMatrix();

            // Right 2
            glPushMatrix();

            glTranslatef(8.0f, 0.3f, 4.0f);
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);

            glPopMatrix();

            // Right 3
            glPushMatrix();

            glTranslatef(8.0f, 0.3f, 10.0f);
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);

            glPopMatrix();

            // Right 4
            glPushMatrix();

            glTranslatef(8.0f, 0.3f, 16.0f);
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);

            glPopMatrix();

            // Left 1
            glPushMatrix();

            glTranslatef(-8.0f, 0.3f, -2.0f);
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);

            glPopMatrix();

            // Left 2
            glPushMatrix();

            glTranslatef(-8.0f, 0.3f, 4.0f);
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);

            glPopMatrix();

            // Left 3
            glPushMatrix();

            glTranslatef(-8.0f, 0.3f, 10.0f);
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);

            glPopMatrix();

            // Left 4
            glPushMatrix();

            glTranslatef(-8.0f, 0.3f, 16.0f);
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);

            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // King Chair

        glPushMatrix();

        glTranslatef(0.0f, 3.5f, -12.0f);
        glScalef(0.75f, 0.75f, 0.75f);
        kingChair();
        glPopMatrix();

        {
            // chairs
            // chair Right 1
            glPushMatrix();

            glTranslatef(8.0f, 2.0f, -2.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(0.75f, 0.75f, 0.75f);
            smallChair();

            glPopMatrix();
            // chair Right 2
            glPushMatrix();

            glTranslatef(8.0f, 2.0f, 4.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(0.75f, 0.75f, 0.75f);
            smallChair();

            glPopMatrix();
            // chair Right 3
            glPushMatrix();

            glTranslatef(8.0f, 2.0f, 10.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(0.75f, 0.75f, 0.75f);
            smallChair();

            glPopMatrix();
            // chair Right 4
            glPushMatrix();

            glTranslatef(8.0f, 2.0f, 16.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(0.75f, 0.75f, 0.75f);
            smallChair();

            glPopMatrix();
            // chair Left 1
            glPushMatrix();

            glTranslatef(-8.0f, 2.0f, -2.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(0.75f, 0.75f, 0.75f);
            smallChair();

            glPopMatrix();
            // chair Left 1
            glPushMatrix();

            glTranslatef(-8.0f, 2.0f, 4.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(0.75f, 0.75f, 0.75f);
            smallChair();

            glPopMatrix();
            // chair Left 1
            glPushMatrix();

            glTranslatef(-8.0f, 2.0f, 10.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(0.75f, 0.75f, 0.75f);
            smallChair();

            glPopMatrix();
            // chair Left 1
            glPushMatrix();

            glTranslatef(-8.0f, 2.0f, 16.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(0.75f, 0.75f, 0.75f);
            smallChair();

            glPopMatrix();
        }

        {
            // Polls
            // changing materail color to gold
            SetLightColorGold();

            // Right 0
            glPushMatrix();

            glTranslatef(10.0f, 6.0f, -17.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Right 1
            glPushMatrix();

            glTranslatef(10.0f, 6.0f, -11.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Right 2
            glPushMatrix();

            glTranslatef(10.0f, 6.0f, -5.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Right 3
            glPushMatrix();

            glTranslatef(10.0f, 6.0f, 1.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Right 4
            glPushMatrix();

            glTranslatef(10.0f, 6.0f, 7.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Right 5
            glPushMatrix();

            glTranslatef(10.0f, 6.0f, 13.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Right 5
            glPushMatrix();

            glTranslatef(10.0f, 6.0f, 19.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();

            // Left 0
            glPushMatrix();

            glTranslatef(-10.0f, 6.0f, -17.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Left 1
            glPushMatrix();

            glTranslatef(-10.0f, 6.0f, -11.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Left 2
            glPushMatrix();

            glTranslatef(-10.0f, 6.0f, -5.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Left 3
            glPushMatrix();

            glTranslatef(-10.0f, 6.0f, 1.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Left 4
            glPushMatrix();

            glTranslatef(-10.0f, 6.0f, 7.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Left 5
            glPushMatrix();

            glTranslatef(-10.0f, 6.0f, 13.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();

            glPopMatrix();
            // Left 5
            glPushMatrix();

            glTranslatef(-10.0f, 6.0f, 19.0f);
            glScalef(0.70f, 0.75f, 0.70f);
            drawPolls();
            SetLightColorWhite();

            glPopMatrix();
        }

        // QUEENS
        glPushMatrix();
        {
            glPushMatrix();

            glTranslatef(-5.0f, 3.2f, 3.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(0.85f, 0.85f, 0.85f);

            drawQueen();

            glTranslatef(0.0f, -3.3f, 0.0f);
            SetLightColorGold();
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);
            SetLightColorWhite();

            glPopMatrix();

            glPushMatrix();
            glTranslatef(-5.0f, 3.2f, 6.0f);
            glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
            glScalef(0.85f, 0.85f, 0.85f);
            drawQueen();

            glTranslatef(0.0f, -3.3f, 0.0f);
            SetLightColorGold();
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);
            SetLightColorWhite();

            glPopMatrix();

            glPushMatrix();
            glTranslatef(-5.0f, 3.2f, 0.0f);
            glRotatef(70.0f, 0.0f, 1.0f, 0.0f);
            glScalef(0.85f, 0.85f, 0.85f);
            drawQueen();

            glTranslatef(0.0f, -3.3f, 0.0f);
            SetLightColorGold();
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);
            SetLightColorWhite();

            glPopMatrix();
        }
        glPopMatrix();

        // HRUSHI
        glPushMatrix();
        {
            glTranslatef(5.0f, 1.8f, 3.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);

            glScalef(0.85f, 0.85f, 0.85f);
            drawHrushi();

            glTranslatef(0.0f, -1.5f, 0.0f);
            SetLightColorGold();
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);
            SetLightColorWhite();
        }
        glPopMatrix();

        // BHRAHM DEV
        glPushMatrix();
        {
            shoulder = -120.0f;
            elbow = 348.0f;
            wrist = 0;

            glTranslatef(0.0f, 4.0f, 3.0f);
            glScalef(0.85f, 0.85f, 0.85f);
            glScalef(AganiDevSize, AganiDevSize, AganiDevSize);
            if (CameraVector_S3[2] > 10.0f && sceneNumber_S3 == 1)
                drawAgniDev();
            else if (sceneNumber_S3 > 1)
                drawAgniDev();
        }
        glPopMatrix();

        // KING
        glPushMatrix();
        {

            shoulder = -96.0f;
            elbow = -107;
            wrist = 3;

            glTranslatef(0.0f, 3.2f, 8.0f);

            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
            glScalef(0.85f, 0.85f, 0.85f);

            drawKing();

            glTranslatef(0.0f, -3.3f, 0.0f);
            SetLightColorGold();
            S3_DrawBoxWithParam(0.2f, 0.2f, 3.0f, 3.0f, 3.0f, 3.0f);
            SetLightColorWhite();
        }
        glPopMatrix();

        // BOWL
        {
            glPushMatrix();

            glTranslatef(bowlPosition[0], bowlPosition[1], bowlPosition[2]);
            glScalef(0.3f, 0.3f, 0.3f);
            glScalef(AganiDevSize, AganiDevSize, AganiDevSize);

            if (CameraVector_S3[2] > 10.0f && sceneNumber_S3 == 1)
                drawBowl();
            else if (sceneNumber_S3 > 1)
                drawBowl();
            glPopMatrix();
        }

        // Yagya
        {
            // yagya
            glPushMatrix();
            glTranslatef(0.0f, 1.3f, 4.0f);
            glScalef(1.2f, 1.2f, 1.2f);
            Yagya();

            glPopMatrix();
        }

        // drawCurtainAndFlower
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Left 1
            glPushMatrix();
            glTranslatef(-7.91f, 7.3f, -14.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Left 2
            glPushMatrix();
            glTranslatef(-7.92, 7.3f, -8.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Left 3
            glPushMatrix();
            glTranslatef(-7.93f, 7.3f, -2.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Left 4
            glPushMatrix();
            glTranslatef(-7.92f, 7.3f, 4.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();
            // Left 5
            glPushMatrix();
            glTranslatef(-7.91f, 7.3f, 10.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Left 6
            glPushMatrix();
            glTranslatef(-7.90f, 7.3f, 16.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Right 1
            glPushMatrix();
            glTranslatef(7.91f, 7.3f, -14.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Right 2
            glPushMatrix();
            glTranslatef(7.92, 7.3f, -8.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Right 3
            glPushMatrix();
            glTranslatef(7.93f, 7.3f, -2.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Right 4
            glPushMatrix();
            glTranslatef(7.92f, 7.3f, 4.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Right 5
            glPushMatrix();
            glTranslatef(7.91f, 7.3f, 10.0f);
            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            // Right 6
            glPushMatrix();
            glTranslatef(7.90f, 7.3f, 16.0f);

            glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
            glScalef(3.3f, 2.1f, 1.0f);
            drawCurtainAndFlower();
            glPopMatrix();

            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        }
    }
    glPopMatrix();
}

void drawPolls(void)
{
    void S3_DrawBoxWithParam(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Top 1 Box
    glPushMatrix();

    glTranslatef(0.0f, 4.4f, 0.0f);
    S3_DrawBoxWithParam(0.8f, 0.8f, 2.5f, 2.5f, 2.5f, 2.5f);

    glPopMatrix();
    // Top 2 box
    glPushMatrix();

    glTranslatef(0.0f, 4.0f, 0.0f);
    S3_DrawBoxWithParam(0.7f, 0.7f, 2.0f, 2.0f, 2.0f, 2.0f);

    glPopMatrix();

    // Top 3 Sphere
    glPushMatrix();

    glTranslatef(0.0f, 3.4f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);

    glPopMatrix();

    // front circle - Top Disk
    glPushMatrix();

    glTranslatef(0.0f, 0.2f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    glScalef(1.0f, 1.0f, 3.0f);
    glDisable(GL_LIGHTING);
    gluQuadricTexture(quadricDisk_S3, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
    gluDisk(quadricDisk_S3, 0, 1.2, 50, 1);

    glPopMatrix();

    // front circle - Top Disk
    glPushMatrix();

    glTranslatef(0.0f, -3.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    glScalef(1.0f, 1.0f, 1.0f);
    gluDisk(quadricDisk_S3, 0, 1.5f, 50, 1);
    glBindTexture(GL_TEXTURE_2D, 0);
    gluQuadricTexture(quadricDisk_S3, 0);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // front circle - Bottom Cylinder
    glPushMatrix();

    glTranslatef(0.0f, 3.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadricCylinder_S3, 0.8f, 1.2f, 10.0f, 30, 30);

    glPopMatrix();

    // Bottom 1 Box
    glPushMatrix();

    glTranslatef(0.0f, -7.4f, 0.0f);
    S3_DrawBoxWithParam(0.8f, 0.8f, 2.8f, 2.8f, 2.8f, 2.8f);

    glPopMatrix();
    // Bottom 2 box
    glPushMatrix();

    glTranslatef(0.0f, -6.8f, 0.0f);
    S3_DrawBoxWithParam(0.7f, 0.7f, 2.5f, 2.5f, 2.5f, 2.5f);

    glPopMatrix();
}

void kingChair(void)
{
    // Function Prototype
    void S3_DrawBoxWithParam(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Back - Midle - Front Circle Base
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_mahal_purple);
    glTranslatef(0.0f, 0.0f, -1.0f);
    gluDisk(quadricDisk_S3, 0, 1.2, 50, 1);

    glPopMatrix();

    // Back - Left - Front Circle Base
    glPushMatrix();

    glTranslatef(-0.8f, 0.0f, -1.1f);
    gluDisk(quadricDisk_S3, 0, 0.6f, 50, 1);

    glPopMatrix();

    // Back - Right - Front Circle Base
    glPushMatrix();

    glTranslatef(0.8f, 0.0f, -1.1f);
    gluDisk(quadricDisk_S3, 0, 0.6f, 50, 1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    // Back - middle Circle Base
    glPushMatrix();
    SetLightColorRed();
    glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
    glTranslatef(0.0f, 0.3f, -1.2f);

    gluDisk(quadricDisk_S3, 0, 1.5, 50, 1);

    glPopMatrix();

    // Back - Left - Back Circle Base
    glPushMatrix();

    glTranslatef(-0.7f, 0.0f, -1.25f);
    gluDisk(quadricDisk_S3, 0, 1.2, 50, 1);

    glPopMatrix();

    // Back - Ritgh -Back Circle Base
    glPushMatrix();

    glTranslatef(0.7f, 0.0f, -1.25f);

    gluDisk(quadricDisk_S3, 0, 1.2, 50, 1);
    glBindTexture(GL_TEXTURE_2D, 0);
    SetLightColorWhite();

    glPopMatrix();

    // Main Base
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
    glTranslatef(0.0f, -1.0f, 0.0f);
    S3_DrawBoxWithParam(0.5f, 0.5f, 3.5f, 3.3f, 2.0f, 2.0f);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    // Bottom Base
    glPushMatrix();
    SetLightColorGold();
    glTranslatef(0.0f, -1.2f, 0.1f);
    glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
    S3_DrawBoxWithParam(0.1f, 0.1f, 3.5f, 3.5f, 2.0f, 2.0f);
    glBindTexture(GL_TEXTURE_2D, 0);
    SetLightColorWhite();
    glPopMatrix();

    //  Left leg
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_mahal_purple);

    glTranslatef(-2.0f, -1.2f, 0.0f);
    S3_DrawBoxWithParam(2.0f, 2.0f, 0.5f, 0.5f, 2.0f, 2.0f);

    glPopMatrix();

    //  Right leg
    glPushMatrix();

    glTranslatef(2.0f, -1.2f, 0.0f);
    S3_DrawBoxWithParam(2.0f, 2.0f, 0.5f, 0.5f, 2.0f, 2.0f);

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    // Left Hand Design -Cylinder
    glPushMatrix();
    glTranslatef(-2.0f, 0.2f, -1.1f);

    SetLightColorRed();
    glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
    gluCylinder(quadricCylinder_S3, 0.45f, 0.45f, 2.2f, 30, 30);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    // Back Hand Design -Cylinder
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
    glTranslatef(2.0f, 0.2f, -1.1f);
    gluCylinder(quadricCylinder_S3, 0.45f, 0.45f, 2.2f, 30, 30);
    glBindTexture(GL_TEXTURE_2D, 0);
    SetLightColorWhite();

    glPopMatrix();

    // Right Hand Design - front Circle
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
    glTranslatef(2.0f, 0.2f, 1.1f);
    gluDisk(quadricDisk_S3, 0, 0.45, 50, 1);

    glPopMatrix();

    // Left Hand Design - Front Circle
    glPushMatrix();

    glTranslatef(-2.0f, 0.2f, 1.1f);
    gluDisk(quadricDisk_S3, 0, 0.45, 50, 1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    // front circle - Top Disk
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
    glTranslatef(0.8f, -2.0f, 2.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quadricDisk_S3, 0, 0.5, 50, 1);

    glPopMatrix();

    // front circle - Bottom Cylinder
    glPushMatrix();

    glTranslatef(0.8f, -2.0f, 2.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadricCylinder_S3, 0.5f, 0.5f, 0.3f, 30, 30);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void smallChair(void)
{
    // Function Prototype
    void S3_DrawBoxWithParam(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // Front Circle Base

    glPushMatrix();

    SetLightColorRed();
    glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
    glTranslatef(0.0f, 0.0f, -1.0f);
    gluDisk(quadricDisk_S3, 0, 1, 50, 1);
    glBindTexture(GL_TEXTURE_2D, 0);
    SetLightColorWhite();

    glPopMatrix();

    // Back -  Circle Base
    glPushMatrix();

    // SetLightColorCyan();
    glBindTexture(GL_TEXTURE_2D, texture_mahal_purple);
    glTranslatef(0.0f, 0.0f, -1.1f);
    gluDisk(quadricDisk_S3, 0, 1.5, 50, 1);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, 0);

    // SetLightColorWhite();
    glPopMatrix();

    // Main Base
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
    glTranslatef(0.0f, -1.0f, 0.0f);
    S3_DrawBoxWithParam(0.5f, 0.5f, 2.0f, 1.8f, 2.0f, 2.0f);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    // Bottom Base
    glPushMatrix();

    SetLightColorGold();
    glTranslatef(0.0f, -1.2f, 0.1f);
    S3_DrawBoxWithParam(0.1f, 0.1f, 2.0f, 2.0f, 2.0f, 2.0f);
    SetLightColorWhite();

    glPopMatrix();

    //  Left leg
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_mahal_purple);
    glTranslatef(-1.25f, -1.2f, 0.0f);
    S3_DrawBoxWithParam(2.0f, 2.0f, 0.5f, 0.5f, 2.0f, 2.0f);

    glPopMatrix();

    //  Right leg
    glPushMatrix();

    glTranslatef(1.25f, -1.2f, 0.0f);
    S3_DrawBoxWithParam(2.0f, 2.0f, 0.5f, 0.5f, 2.0f, 2.0f);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    // Left Hand Design -Cylinder
    glPushMatrix();

    SetLightColorRed();
    glBindTexture(GL_TEXTURE_2D, texture_mahal_red_plan_carpet);
    glTranslatef(-1.27f, 0.2f, -1.1f);
    gluCylinder(quadricCylinder_S3, 0.45f, 0.45f, 2.2f, 30, 30);

    glPopMatrix();

    // Back Hand Design -Cylinder
    glPushMatrix();

    glTranslatef(1.27f, 0.2f, -1.1f);
    gluCylinder(quadricCylinder_S3, 0.45f, 0.45f, 2.2f, 30, 30);

    glPopMatrix();

    // Right Hand Design - front Circle
    glPushMatrix();

    glTranslatef(1.27f, 0.2f, 1.1f);
    gluDisk(quadricDisk_S3, 0, 0.45, 50, 1);

    glPopMatrix();

    // Left Hand Design - Front Circle
    glPushMatrix();

    glTranslatef(-1.27f, 0.2f, 1.1f);
    gluDisk(quadricDisk_S3, 0, 0.45, 50, 1);
    glBindTexture(GL_TEXTURE_2D, 0);
    SetLightColorWhite();

    glPopMatrix();
}

void S3_DrawBoxWithParam(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth)
{
    // code
    glPushMatrix();

    // geometry

    // FRONT FACE
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(0.0f, 0.0f);
    }
    else
        glTexCoord2f(0.0f, 0.0f);

    glVertex3f(topWidth / 2, rightHeight / 2, topDepth / 2);

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(4.0f, 0.0f);
    }
    else
        glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, topDepth / 2);

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(4.0f, 1.0f);
    }
    else
        glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), bottomDepth / 2);

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(0.0f, 1.0f);
    }
    else
        glTexCoord2f(0.0f, 1.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), bottomDepth / 2);
    glEnd();

    // RIGHT FACE
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(0.0f, 0.0f);
    }
    else
        glTexCoord2f(0.0f, 0.0f);
    glVertex3f(topWidth / 2, rightHeight / 2, -(topDepth / 2));

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(6.0f, 0.0f);
    }
    else
        glTexCoord2f(1.0f, 0.0f);
    glVertex3f(topWidth / 2, rightHeight / 2, topDepth / 2);

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(6.0f, 1.0f);
    }
    else
        glTexCoord2f(1.0f, 1.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), bottomDepth / 2);

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(0.0f, 1.0f);
    }
    else
        glTexCoord2f(0.0f, 1.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), -(bottomDepth / 2));
    glEnd();

    // BACK FACE
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(0.0f, 0.0f);
    }
    else
        glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, -(topDepth / 2));

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(3.0f, 0.0f);
    }
    else
        glTexCoord2f(1.0f, 0.0f);
    glVertex3f((topWidth / 2), rightHeight / 2, -(topDepth / 2));

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(3.0f, 1.0f);
    }
    else
        glTexCoord2f(1.0f, 1.0f);
    glVertex3f((bottomWidth / 2), -(rightHeight / 2), -(bottomDepth / 2));

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(0.0f, 1.0f);
    }
    else
        glTexCoord2f(0.0f, 1.0f);

    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), -bottomDepth / 2);
    glEnd();

    // LEFT FACE
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(0.0f, 0.0f);
    }
    else
        glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, (topDepth / 2));

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(6.0f, 0.0f);
    }
    else
        glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(topWidth / 2), leftHeight / 2, -(topDepth / 2)); // y = -1

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(6.0f, 1.0f);
    }
    else
        glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), -(bottomDepth / 2));

    if (iSWallsTextrue == SIDEWALLS)
    {
        glTexCoord2f(0.0f, 1.0f);
    }
    else
        glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), (bottomDepth / 2));
    glEnd();

    // TOP
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);

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
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), bottomDepth / 2);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(bottomWidth / 2, -(rightHeight / 2), -(bottomDepth / 2));

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), -(bottomDepth / 2));

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(leftHeight / 2), bottomDepth / 2);

    glEnd();

    glPopMatrix();
}

void S3_DrawBox(void)
{
    glBegin(GL_QUADS);
    // FRONT FACE
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

    // RIGHT FACE
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // BACK FACE
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    // LEFT FACE
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
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

void S3_DrawBox(float height, float topWidth, float bottomWidth, float topDepth, float bottomDepth)
{
    glBegin(GL_QUADS);
    // FRONT FACE
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(topWidth / 2, height / 2, topDepth / 2);

    glTexCoord2f(2.0f, 1.0f);
    glVertex3f(-(topWidth / 2), height / 2, topDepth / 2);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(height / 2), bottomDepth / 2);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(bottomWidth / 2, -(height / 2), bottomDepth / 2);
    glEnd();

    // RIGHT FACE
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);

    glVertex3f(topWidth / 2, height / 2, -(topDepth / 2));

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(topWidth / 2, height / 2, topDepth / 2);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(bottomWidth / 2, -(height / 2), bottomDepth / 2);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(bottomWidth / 2, -(height / 2), -(bottomDepth / 2));
    glEnd();

    // BACK FACE
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(topWidth / 2), height / 2, -(topDepth / 2));

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f((topWidth / 2), height / 2, -(topDepth / 2));

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f((bottomWidth / 2), -(height / 2), -(bottomDepth / 2));

    glTexCoord2f(0.0f, 0.0f);

    glVertex3f(-(bottomWidth / 2), -(height / 2), -bottomDepth / 2);
    glEnd();

    // LEFT FACE
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-(topWidth / 2), height / 2, (topDepth / 2));

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(topWidth / 2), height / 2, -(topDepth / 2)); // y = -1

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(height / 2), -(bottomDepth / 2));

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-(bottomWidth / 2), -(height / 2), (bottomDepth / 2));
    glEnd();

    // TOP
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(topWidth / 2, height / 2, topDepth / 2);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(topWidth / 2, height / 2, -(topDepth / 2));

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(topWidth / 2), height / 2, -(topDepth / 2));

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-(topWidth / 2), height / 2, topDepth / 2);
    glEnd();

    // BOTTOM FACE
    glNormal3f(0.0f, -1.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(bottomWidth / 2, -(height / 2), bottomDepth / 2);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(bottomWidth / 2, -(height / 2), -(bottomDepth / 2));

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-(bottomWidth / 2), -(height / 2), -(bottomDepth / 2));

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(bottomWidth / 2), -(height / 2), bottomDepth / 2);

    glEnd();
}

void Quad(void)
{
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glPopMatrix();
}

void Yagya(void)
{
    // function declartions
    void drawYagya(void);
    void drawfire(void);

    // code
    glPushMatrix();
    drawYagya();
    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.0f, 0.5f, -0.9f);
    glEnable(GL_BLEND);
    drawfire();
    glDisable(GL_BLEND);

    glPopMatrix();
}

void SetLightColorGold(void)
{
    // changing materail color to gold
    glPushMatrix();

    // ambient material
    gfMaterialAmbiant_Four_S3[0] = 0.24725f; // r
    gfMaterialAmbiant_Four_S3[1] = 0.1995f;  // g
    gfMaterialAmbiant_Four_S3[2] = 0.0745f;  // b
    gfMaterialAmbiant_Four_S3[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, gfMaterialAmbiant_Four_S3);

    // diffuse material
    gfMeterialDeffuse_Four_S3[0] = 0.75164f; // r
    gfMeterialDeffuse_Four_S3[1] = 0.60648f; // g
    gfMeterialDeffuse_Four_S3[2] = 0.22648f; // b
    gfMeterialDeffuse_Four_S3[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gfMeterialDeffuse_Four_S3);

    // specular material
    gfMateralSpecular_Four_S3[0] = 0.528281f; // r
    gfMateralSpecular_Four_S3[1] = 0.455802f; // g
    gfMateralSpecular_Four_S3[2] = 0.266065f; // b
    gfMateralSpecular_Four_S3[3] = 1.0f;      // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, gfMateralSpecular_Four_S3);

    // shininess
    gfMaterialShineeness_Four_S3 = 0.4 * 50;
    glMaterialf(GL_FRONT, GL_SHININESS, gfMaterialShineeness_Four_S3);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
}

void SetLightColorWhite(void)
{
    // changing materail color to white
    glPushMatrix();

    // ambient material
    gfMaterialAmbiant_Four_S3[0] = 0.0f; // r
    gfMaterialAmbiant_Four_S3[1] = 0.0;  // g
    gfMaterialAmbiant_Four_S3[2] = 0.0;  // b
    gfMaterialAmbiant_Four_S3[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, gfMaterialAmbiant_Four_S3);

    // diffuse material
    gfMeterialDeffuse_Four_S3[0] = 1.0;  // r
    gfMeterialDeffuse_Four_S3[1] = 1.0;  // g
    gfMeterialDeffuse_Four_S3[2] = 1.0;  // b
    gfMeterialDeffuse_Four_S3[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gfMeterialDeffuse_Four_S3);

    // specular material
    gfMateralSpecular_Four_S3[0] = 1.0;  // r
    gfMateralSpecular_Four_S3[1] = 1.0;  // g
    gfMateralSpecular_Four_S3[2] = 1.0;  // b
    gfMateralSpecular_Four_S3[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, gfMateralSpecular_Four_S3);

    // shininess
    gfMaterialShineeness_Four_S3 = 50.0f;
    glMaterialf(GL_FRONT, GL_SHININESS, gfMaterialShineeness_Four_S3);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
}

void SetLightColorSilver(void)
{
    glPushMatrix();

    // ***** 6th sphere on 2nd column, silver *****
    // ambient material
    gfMaterialAmbiant_Four_S3[0] = 0.19225; // r
    gfMaterialAmbiant_Four_S3[1] = 0.19225; // g
    gfMaterialAmbiant_Four_S3[2] = 0.19225; // b
    glMaterialfv(GL_FRONT, GL_AMBIENT, gfMaterialAmbiant_Four_S3);

    // diffuse material
    gfMeterialDeffuse_Four_S3[0] = 0.50754; // r
    gfMeterialDeffuse_Four_S3[1] = 0.50754; // g
    gfMeterialDeffuse_Four_S3[2] = 0.50754; // b
    gfMeterialDeffuse_Four_S3[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gfMeterialDeffuse_Four_S3);

    // specular material
    gfMateralSpecular_Four_S3[0] = 0.508273; // r
    gfMateralSpecular_Four_S3[1] = 0.508273; // g
    gfMateralSpecular_Four_S3[2] = 0.508273; // b
    gfMateralSpecular_Four_S3[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, gfMateralSpecular_Four_S3);

    // shininess
    gfMaterialShineeness_Four_S3 = 0.4 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, gfMaterialShineeness_Four_S3);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
}

void SetLightColorRed(void)
{
    glPushMatrix();

    // *****4th sphere on 3rd column, red *****
    // ambient material
    gfMaterialAmbiant_Four_S3[0] = 0.1745f;   // r
    gfMaterialAmbiant_Four_S3[1] = 0.01175f;  // g
    gfMaterialAmbiant_Four_S3[2] = 00.01175f; // b
    gfMaterialAmbiant_Four_S3[3] = 0.55f;     // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, gfMaterialAmbiant_Four_S3);

    // diffuse material
    gfMeterialDeffuse_Four_S3[0] = 0.61424f; // r
    gfMeterialDeffuse_Four_S3[1] = 0.04136f; // g
    gfMeterialDeffuse_Four_S3[2] = 0.04136f; // b
    gfMeterialDeffuse_Four_S3[3] = 0.55f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gfMeterialDeffuse_Four_S3);

    // specular material
    gfMateralSpecular_Four_S3[0] = 0.727811f; // r
    gfMateralSpecular_Four_S3[1] = 0.626959f; // g
    gfMateralSpecular_Four_S3[2] = 0.626959f; // b
    gfMateralSpecular_Four_S3[3] = 0.55f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, gfMateralSpecular_Four_S3);

    // shininess
    gfMaterialShineeness_Four_S3 = 0.25 * 76.8f;
    glMaterialf(GL_FRONT, GL_SHININESS, gfMaterialShineeness_Four_S3);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
}

void SetLightColorCyan(void)
{
    glPushMatrix();

    // ***** 2nd sphere on 3rd column, cyan *****
    // ambient material
    gfMaterialAmbiant_Four_S3[0] = 0.0;  // r
    gfMaterialAmbiant_Four_S3[1] = 0.1;  // g
    gfMaterialAmbiant_Four_S3[2] = 0.06; // b
    gfMaterialAmbiant_Four_S3[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, gfMaterialAmbiant_Four_S3);

    // diffuse material
    gfMeterialDeffuse_Four_S3[0] = 0.0;        // r
    gfMeterialDeffuse_Four_S3[1] = 0.50980392; // g
    gfMeterialDeffuse_Four_S3[2] = 0.50980392; // b
    gfMeterialDeffuse_Four_S3[3] = 1.0f;       // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gfMeterialDeffuse_Four_S3);

    // specular material
    gfMateralSpecular_Four_S3[0] = 0.50196078; // r
    gfMateralSpecular_Four_S3[1] = 0.50196078; // g
    gfMateralSpecular_Four_S3[2] = 0.50196078; // b
    gfMateralSpecular_Four_S3[3] = 1.0f;       // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, gfMateralSpecular_Four_S3);

    // shininess
    gfMaterialShineeness_Four_S3 = 0.25 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, gfMaterialShineeness_Four_S3);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
}

void SetLightColorGreen(void)
{
    glPushMatrix();

    // ***** 3rd sphere on 2nd column, green *****
    // ambient material
    gfMaterialAmbiant_Four_S3[0] = 0.0;  // r
    gfMaterialAmbiant_Four_S3[1] = 0.0;  // g
    gfMaterialAmbiant_Four_S3[2] = 0.0;  // b
    gfMaterialAmbiant_Four_S3[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, gfMaterialAmbiant_Four_S3);

    // diffuse material
    gfMeterialDeffuse_Four_S3[0] = 0.1;  // r
    gfMeterialDeffuse_Four_S3[1] = 0.35; // g
    gfMeterialDeffuse_Four_S3[2] = 0.1;  // b
    gfMeterialDeffuse_Four_S3[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gfMeterialDeffuse_Four_S3);

    // specular material
    gfMateralSpecular_Four_S3[0] = 0.45; // r
    gfMateralSpecular_Four_S3[1] = 0.55; // g
    gfMateralSpecular_Four_S3[2] = 0.45; // b
    gfMateralSpecular_Four_S3[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, gfMateralSpecular_Four_S3);

    // shininess
    gfMaterialShineeness_Four_S3 = 0.25 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, gfMaterialShineeness_Four_S3);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
}

//////Yagya

void drawYagya(void)
{
    // funnction declartions
    void S3_DrawBox(void);

    // code

    // Inner Box - Front Side
    glPushMatrix();
    glScalef(1.0f, 0.50f, 0.05f);
    glBindTexture(GL_TEXTURE_2D, texture_rangoli_one);
    S3_DrawBox();

    // Inner Box - Right Box
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(1.011f, 0.0f, 0.0f);
    glScalef(1.0f, 0.50f, 0.05f);
    S3_DrawBox();

    // Inner Box - Left Box
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
    glTranslatef(-1.011f, 0.0f, 0.0f);
    glScalef(1.0f, 0.50f, 0.05f);
    S3_DrawBox();

    // Inner Box - Back Box
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -2.0f);
    glScalef(1.0f, 0.50f, 0.05f);
    S3_DrawBox();

    glBindTexture(GL_TEXTURE_2D, texture_rangoli_two);
    // Middle Box - Front Side
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, -0.13f, 0.16f);
    glScalef(1.0f, 0.35f, 0.13f);
    S3_DrawBox();

    // Middle Box - Right Box
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.10f, -0.13f, 0.00f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(1.011f, 0.0f, 0.0f);
    glScalef(1.30f, 0.35f, 0.13f);
    S3_DrawBox();

    // Middle Box - Left Box
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.10f, -0.13f, 0.0f);
    glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
    glTranslatef(-1.011f, 0.0f, 0.0f);
    glScalef(1.30f, 0.35f, 0.13f);
    S3_DrawBox();

    // Middle Box - Back Box
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, -0.13f, -2.16f);
    glScalef(1.0f, 0.35f, 0.13f);
    S3_DrawBox();

    glBindTexture(GL_TEXTURE_2D, texture_rangoli_one);
    // outer Box - Front Side
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, -0.21f, 0.40f);
    glScalef(1.47f, 0.25f, 0.13f);
    S3_DrawBox();

    // outer Box - Right Box
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.35f, -0.211f, 0.35f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(1.30f, 0.0f, 0.0f);
    glScalef(1.40f, 0.25f, 0.13f);
    S3_DrawBox();

    // outer Box - Left Box
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.35f, -0.2111f, 0.35f);
    glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
    glTranslatef(-1.30f, 0.0f, 0.0f);
    glScalef(1.40f, 0.25f, 0.13f);
    S3_DrawBox();

    // outer Box - Back Box
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, -0.21f, -2.40f);
    glScalef(1.47f, 0.25f, 0.13f);
    S3_DrawBox();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

void InitializeFireParticle(PARTICLE *p_Particle)
{

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        p_Particle[i].avtive = true;
        p_Particle[i].life = 1.0f;
        p_Particle[i].fade = 0.1f;

        p_Particle[i].x = 0.0f;
        p_Particle[i].y = 0.0f;
        p_Particle[i].z = 0.0f;

        p_Particle[i].r = colors[i * (12 / MAX_PARTICLES)][0];
        p_Particle[i].g = colors[i * (12 / MAX_PARTICLES)][1];
        p_Particle[i].b = colors[i * (12 / MAX_PARTICLES)][2];
        p_Particle[i].xi = 0.0f;
        p_Particle[i].yi = 0.0f;
        p_Particle[i].zi = 0.0f;
        p_Particle[i].xg = 0.0f;
        p_Particle[i].yg = 0.3f;
        p_Particle[i].zg = 0.0f;
    }
}

void drawfire(void)
{
    // Function Prototype
    void drawParticles(PARTICLE * p_particle);

    // code
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.0f);
    drawParticles(particle);

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.3f, 0.0f, -0.2f);
    glRotatef(95.0f, 0.0f, 1.0f, 0.0f);
    drawParticles(particle);

    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.3f, 0.0f, 0.0f);
    glRotatef(50.0f, 0.0f, 1.0f, 0.0f);

    drawParticles(particle);

    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.3f);
    glRotatef(160.0f, 0.0f, 1.0f, 0.0f);

    drawParticles(particle);

    glPopMatrix();
}

void drawParticles(PARTICLE *p_particle)
{
    glDisable(GL_LIGHTING);
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (particle[i].avtive)
        {
            float x = particle[i].x;
            float y = particle[i].y;
            float z = particle[i].z;

            glBindTexture(GL_TEXTURE_2D, texture_fire);
            glBegin(GL_TRIANGLE_STRIP);
            glColor4f(particle[i].r, particle[i].g, particle[i].b, particle[i].life);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(x + 0.3f, y + 0.3f, z);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(x - 0.3f, y + 0.3f, z);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(x + 0.3f, y - 0.3f, z);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(x - 0.3f, y - 0.3f, z);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(x + 0.3f, y + 0.3f, z);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(x - 0.3f, y + 0.3f, z);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(x + 0.3f, y - 0.3f, z);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(x - 0.3f, y - 0.3f, z);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_LIGHTING);
}

void drawParticleAnimation(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (particle[i].avtive)
        {
            particle[i].x += particle[i].xi / (slowdown * MAX_PARTICLES);
            particle[i].y += particle[i].yi / (slowdown * MAX_PARTICLES);
            particle[i].z += particle[i].zi / (slowdown * MAX_PARTICLES);

            if (particle[i].y > 4.0f)
                particle[i].avtive = false;

            particle[i].xi += particle[i].xg;
            particle[i].yi += particle[i].yg;
            particle[i].zi += particle[i].zg;

            // decrementating the life of particle
            particle[i].life -= particle[i].fade * 1.0f;

            //
            if (particle[i].life < 0.0f)
            {
                particle[i].life = 1.0f;
                particle[i].fade = float(rand() / 1000.0f);
                particle[i].x = 0.0f;
                particle[i].y = 0.0f;
                particle[i].z = 0.0f;

                particle[i].xi = float((rand() % 60) - 32.0f);
                particle[i].yi = float((rand() % 60) - 30.0f);
                particle[i].zi = float((rand() % 60) - 30.0f);

                particle[i].r = colors[col][0];
                particle[i].g = colors[col][2];
                particle[i].b = colors[col][3];
            }
        }
    }
}

void drawCurtainAndFlower(void)
{
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glPushMatrix();

    {
        glPushMatrix();
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, -0.25f, 0.0f);
        glScalef(1.0f, 1.5f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_blue_curtain);

        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.2f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.2f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.2f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.2f);

        glEnd();
        glPopMatrix();
    }

    {
        glPushMatrix();
        glTranslatef(0.0f, 0.8f, -2.2f);
        glScalef(1.0f, 0.35f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_flower);

        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.1f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.1f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.1f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.1f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

void drawSideWalls(void)
{
    glBindTexture(GL_TEXTURE_2D, texture_mahal_Wall);
    // Left
    glPushMatrix();
    iSWallsTextrue = SIDEWALLS;
    glTranslatef(-16.5f, 4.75f, 0.0f);
    S3_DrawBoxWithParam(10.0f, 10.0f, 0.2f, 0.2f, 40.0f, 40.0f);
    glPopMatrix();

    // Right
    glPushMatrix();
    iSWallsTextrue = SIDEWALLS;
    glTranslatef(16.5f, 4.75f, 0.0f);
    S3_DrawBoxWithParam(10.0f, 10.0f, 0.2f, 0.2f, 40.0f, 40.0f);
    glPopMatrix();

    // Back
    glPushMatrix();
    iSWallsTextrue = SIDEWALLS;
    glTranslatef(0.0f, 4.75f, -20.0f);
    S3_DrawBoxWithParam(10.0f, 10.0f, 34.0f, 34.0f, 0.2f, 0.2f);
    glPopMatrix();

    // Front
    glPushMatrix();
    iSWallsTextrue = SIDEWALLS;
    glTranslatef(0.0f, 4.75f, 20.0f);
    S3_DrawBoxWithParam(10.0f, 10.0f, 34.0f, 34.0f, 0.2f, 0.2f);
    iSWallsTextrue = OTHER;
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, texture_mahal_Door);
    glPushMatrix();
    glTranslatef(0.0f, 4.75f, 19.9f);
    S3_DrawBoxWithParam(10.0f, 10.0f, 11.3f, 11.3f, 0.2f, 0.2f);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, texture_mahal_ceiling);
    glPushMatrix();
    glTranslatef(0.0f, 10.0f, 0.0f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    S3_DrawBoxWithParam(0.2f, 0.2f, 34.0f, 34.0f, 40.0f, 40.0f);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}
///////////////////////////////////////////////

void drawKing(void)
{
    // funnction declartions
    void S3_DrawBox(float, float, float, float, float);
    void S3_DrawBoxWithParam(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // code
    glScalef(0.85f, 1.0f, 0.50f);
    // UPPER BODY
    {
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        // Shoulder + neck
        glPushMatrix();
        glTranslatef(0.0f, 1.4f, 0.0f);
        colorSetcolor(232, 176, 167);
        S3_DrawBox(0.4f, 1.3f, 1.5f, 1.3f, 1.5f);

        // left shoulder
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.5f, 1.4f, 0.0f);
        S3_DrawBox(0.4f, 1.1f, 0.6f, 1.2f, 1.5f);

        // Right shoulder
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.5f, 1.4f, 0.0f);
        S3_DrawBox(0.4f, 1.1f, 0.6f, 1.2f, 1.5f);

        // chest
        glPopMatrix();
        glPushMatrix();
        colorSetcolor(232, 195, 167);
        glTranslatef(0.0f, 0.5f, 0.0f);
        S3_DrawBox(1.5f, 1.5f, 1.2f, 1.5f, 1.0f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glRotatef(50.0f, 0.0f, 0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_red_cloth);
        S3_DrawBoxWithParam(2.3f, 2.0f, 0.2f, 0.7f, 1.5f, 1.5f);
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }

    // LOWER BODY - LEGS
    {

        // left leg
        glPushMatrix();
        glTranslatef(-0.3f, -1.3f, 0.0f);
        glScalef(0.40f, 0.75f, 0.75f);
        colorSetcolor(232, 195, 167);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.0f, 0.9f);

        // left bottom leg
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.3f, -2.4f, 0.0f);
        glScalef(0.29f, 0.70f, 0.55f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.4f, 0.7f);

        // Right leg
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.3f, -1.3f, 0.0f);
        glScalef(0.40f, 0.75f, 0.75f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.0f, 1.0f);

        // Right bottom leg
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.3f, -2.4f, 0.0f);
        glScalef(0.29f, 0.70, 0.55f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.3f, 0.7f);

        // toes Left
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.3f, -3.0f, 0.1f);
        S3_DrawBox(0.2f, 0.30f, 0.34f, 0.6f, 0.7f);

        // toes Right
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.3f, -3.0f, 0.1f);
        S3_DrawBox(0.2f, 0.30f, 0.34f, 0.6f, 0.7f);

        glPopMatrix();

        // Stomch bottom
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture_red_cloth);
        glTranslatef(0.0f, -0.2f, 0.0f);
        S3_DrawBoxWithParam(0.2f, 0.2f, 1.3f, 1.3f, 1.25f, 1.25);
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();

        // bootom
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
        glTranslatef(0.0f, -0.8f, 0.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        S3_DrawBox(0.2f, 1.2f, 1.2f, 1.25f, 1.2f);
        glPopMatrix();

        // stomoch
        glPushMatrix();
        glTranslatef(0.0f, -0.5f, 0.0f);
        S3_DrawBox(0.5f, 1.0f, 1.2f, 1.2f, 1.2f);
        glPopMatrix();

        // Right leg
        glPushMatrix();
        glTranslatef(0.3f, -1.3f, 0.1f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glScalef(0.6f, 1.5f, 0.7f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        gluQuadricTexture(quadric, GL_TRUE);
        gluSphere(quadric, 1.0f, 100, 100);
        glPopMatrix();

        // Left leg
        glPushMatrix();
        glTranslatef(-0.3f, -1.3f, 0.1f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glScalef(0.6f, 1.5f, 0.7f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        gluSphere(quadric, 1.0f, 100, 100);
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Right Hand
    glPushMatrix();
    {
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        glTranslatef(1.2f, 1.5f, 0.4f);
        glScalef(0.50f, 0.75f, 0.75f);
        glRotatef(35.0f, -1.0f, 0.0f, -1.0f);
        glRotatef(10.0f, -1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glRotatef((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);

        glTranslatef(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 1.0f, 1.0f);

        // Draw Arm
        // we are drawing sphere but we are scalling which will look like ellipse
        glColor3f(1.0f, 1.0f, 1.0f);
        gluSphere(quadric, 0.5f, 100, 100);

        SetLightColorGold();
        glScalef(0.5f, 1.1f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
        S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
        SetLightColorWhite();

        // Fore Arm
        glTranslatef(0.8f, 0.0f, 0.0f);

        glRotatef((GLfloat)elbow, 0.0f, 0.0f, 1.0f);

        glTranslatef(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 0.7f, 1.0f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        glPushMatrix();

        // fore arm - palm
        glTranslatef(0.8f, 0.0f, 0.0f);
        glRotatef(45.0f, 1.0f, -1.0f, 0.0f);

        glRotatef((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

        glTranslatef(0.4f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(0.6f, 0.5f, 0.2f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        // FINGERS
        // 1
        glTranslatef(0.3f, 0.2f, 0.0f);

        glPushMatrix();

        glScalef(0.3f, 0.1f, 0.2f);

        gluSphere(quadric, 0.5f, 10, 10);

        // 2
        glPopMatrix();
        glTranslatef(0.1f, -0.12f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 3
        glPopMatrix();
        glTranslatef(0.0f, -0.1f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        quadric = gluNewQuadric();

        gluSphere(quadric, 0.5f, 5, 5);

        // 4
        glPopMatrix();
        glTranslatef(0.0f, -0.09f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 5
        glPopMatrix();
        glTranslatef(-0.1f, -0.1f, 0.0f);
        glPushMatrix();

        glScalef(0.25f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }

    glPopMatrix();

    // Left Hand
    glPushMatrix();
    {
        glTranslatef(-1.2f, 1.5f, 0.4f);
        glScalef(0.50f, 0.75f, 0.75f);
        glRotatef(35.0f, -1.0f, 0.0f, 1.0f);
        glRotatef(10.0f, -1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glRotatef((GLfloat)shoulder, 0.0f, 0.0f, -1.0f);

        glTranslatef(-1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 1.0f, 1.0f);

        // Draw Arm

        // we are drawing sphere but we are scalling which will look like ellipse
        gluSphere(quadric, 0.5f, 100, 100);

        // band
        SetLightColorGold();
        glScalef(0.5f, 1.1f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
        S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        SetLightColorWhite();
        glColor3f(1.0f, 1.0f, 1.00f);

        glPopMatrix();

        // Fore Arm
        glTranslatef(-0.8f, 0.0f, 0.0f);

        glRotatef((GLfloat)elbow, 0.0f, 0.0f, -1.0f);

        glTranslatef(-1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 0.7f, 1.0f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        glPushMatrix();

        // fore arm - palm
        glTranslatef(-0.8f, 0.0f, 0.0f);
        glRotatef(45.0f, 1.0f, 1.0f, 0.0f);

        glRotatef((GLfloat)wrist, 0.0f, 0.0f, -1.0f);

        glTranslatef(-0.4f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(0.6f, 0.5f, 0.2f);

        gluSphere(quadric, 0.5f, 50, 50);

        glPopMatrix();

        // FINGERS
        // 1
        glTranslatef(-0.3f, 0.2f, 0.0f);

        glPushMatrix();

        glScalef(0.3f, 0.1f, 0.2f);

        gluSphere(quadric, 0.5f, 10, 10);

        // 2
        glPopMatrix();
        glTranslatef(-0.1f, -0.12f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 3
        glPopMatrix();
        glTranslatef(0.0f, -0.1f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        quadric = gluNewQuadric();

        gluSphere(quadric, 0.5f, 5, 5);

        // 4
        glPopMatrix();
        glTranslatef(0.0f, -0.09f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 5
        glPopMatrix();
        glTranslatef(0.2f, -0.1f, 0.0f);
        glPushMatrix();

        glScalef(0.25f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }

    // Neack
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    colorSetcolor(232, 195, 167);
    gluCylinder(quadricCylinder_S3, 0.3f, 0.3f, 0.5f, 10, 10);

    glPopMatrix();

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadric, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_face_King);

    glTranslatef(0.0f, 2.4f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    gluSphere(quadric, 0.58f, 30, 30);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    // crown
    glPushMatrix();
    glTranslatef(0.0f, 3.5f, 0.0f);
    glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
    drawCrown(texture_crown_1);

    glPopMatrix();
}

void drawQueen(void)
{
    // funnction declartions
    void S3_DrawBox(float, float, float, float, float);
    void S3_DrawBoxWithParam(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    // VARIABLE DECLARTIONS

    shoulder = -96.0f;
    elbow = -107;
    wrist = 3;

    // code
    glScalef(0.75f, 1.0f, 0.45f);
    // UPPER BODY
    {
        glBindTexture(GL_TEXTURE_2D, texture_red_cloth);
        // Shoulder + neck
        glPushMatrix();
        glTranslatef(0.0f, 1.4f, 0.0f);
        S3_DrawBox(0.4f, 1.3f, 1.4f, 1.3f, 1.5f);

        // left shoulder
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.5f, 1.4f, 0.0f);
        S3_DrawBox(0.4f, 1.1f, 0.6f, 1.2f, 1.5f);

        // right Shoulder
        glPopMatrix();
        glPushMatrix();

        // Right shoulder
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.5f, 1.4f, 0.0f);
        S3_DrawBox(0.4f, 1.1f, 0.6f, 1.2f, 1.5f);

        // Top chest
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0f, 0.8f, 0.0f);
        S3_DrawBox(0.8f, 1.5f, 1.0f, 1.65f, 1.05f);

        // stomch
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0f, 0.3f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        S3_DrawBox(0.5f, 1.0f, 1.0f, 1.05f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, 0);

        // stomoch
        glPopMatrix();
        glPushMatrix();

        glTranslatef(0.0f, 0.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, texture_red_cloth);
        S3_DrawBox(0.5f, 1.0f, 1.01f, 1.0f, 1.0f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, 0.85f, 0.0f);
        glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
        S3_DrawBoxWithParam(2.3f, 2.0f, 0.5f, 0.8f, 1.5f, 1.5f);
        glPopMatrix();
    }

    // LOWER BODY - LEGS
    {
        // bottom
        glPushMatrix();
        glTranslatef(0.0f, -1.5f, 0.0f);
        S3_DrawBox(2.5f, 1.0f, 2.0f, 1.0f, 2.4f);

        // left leg
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.3f, -1.3f, 0.0f);
        glScalef(0.40f, 0.75f, 0.75f);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.5f, 0.9f);

        // left bottom leg
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.3f, -2.4f, 0.0f);
        glScalef(0.29f, 0.70f, 0.55f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.3f, 0.7f);

        // Right leg
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.3f, -1.3f, 0.0f);
        glScalef(0.40f, 0.75f, 0.75f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.5f, 1.0f);

        // Right bottom leg
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.3f, -2.4f, 0.0f);
        glScalef(0.29f, 0.70, 0.55f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.3f, 0.7f);

        // toes Left
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.3f, -3.0f, 0.1f);
        S3_DrawBox(0.2f, 0.30f, 0.34f, 0.6f, 0.7f);

        // toes Right
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.3f, -3.0f, 0.1f);
        S3_DrawBox(0.2f, 0.30f, 0.34f, 0.6f, 0.7f);

        glPopMatrix();
    }

    // Right hand
    glPushMatrix();
    {
        // Shoulder
        glPushMatrix();
        glTranslatef(1.2f, 1.5f, 0.0f);
        glScalef(0.8f, 0.4f, 1.5f);
        glBindTexture(GL_TEXTURE_2D, texture_red_cloth);
        gluSphere(quadric, 0.5f, 10, 10);

        glBindTexture(GL_TEXTURE_2D, texture_skin);
        glPopMatrix();

        glTranslatef(1.2f, 1.5f, 0.4f);
        glScalef(0.50f, 0.75f, 0.75f);
        glRotatef(35.0f, -1.0f, 0.0f, -1.0f);
        glRotatef(10.0f, -1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glRotatef((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);

        glTranslatef(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 1.0f, 1.0f);

        // Draw Arm

        // we are drawing sphere but we are scalling which will look like ellipse
        gluSphere(quadric, 0.5f, 100, 100);

        // band
        glPushMatrix();
        {
            SetLightColorGold();
            glScalef(0.2f, 1.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
            S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_2D, texture_skin);
            glColor3f(1.0f, 1.0f, 1.0f);
            SetLightColorWhite();
        }
        glPopMatrix();

        glPopMatrix();

        // Fore Arm
        glTranslatef(0.8f, 0.0f, 0.0f);

        glRotatef((GLfloat)elbow, 0.0f, 0.0f, 1.0f);

        glTranslatef(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 0.7f, 1.0f);

        gluSphere(quadric, 0.5f, 100, 100);

        // band
        glPushMatrix();
        {
            glTranslatef(0.45f, 0.0f, 0.0f);
            SetLightColorGold();
            glScalef(0.2f, 0.5f, 0.5f);
            glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
            S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_2D, texture_skin);
            glColor3f(1.0f, 1.0f, 1.0f);
            SetLightColorWhite();
        }
        glPopMatrix();

        glPopMatrix();

        glPushMatrix();

        // fore arm - palm
        glTranslatef(0.8f, 0.0f, 0.0f);
        glRotatef(45.0f, 1.0f, -1.0f, 0.0f);

        glRotatef((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

        glTranslatef(0.4f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(0.6f, 0.5f, 0.2f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        // FINGERS
        // 1
        glTranslatef(0.3f, 0.2f, 0.0f);

        glPushMatrix();

        glScalef(0.3f, 0.1f, 0.2f);

        gluSphere(quadric, 0.5f, 10, 100);

        // 2
        glPopMatrix();
        glTranslatef(0.1f, -0.12f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 50, 50);

        // 3
        glPopMatrix();
        glTranslatef(0.0f, -0.1f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        quadric = gluNewQuadric();

        gluSphere(quadric, 0.5f, 50, 50);

        // 4
        glPopMatrix();
        glTranslatef(0.0f, -0.09f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 5
        glPopMatrix();
        glTranslatef(-0.1f, -0.1f, 0.0f);
        glPushMatrix();

        glScalef(0.25f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 50, 50);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }

    glPopMatrix();

    // Left Hand
    glPushMatrix();
    {
        // shoulder
        glPushMatrix();
        glTranslatef(-1.2f, 1.5f, 0.0f);
        glScalef(0.8f, 0.4f, 1.5f);
        glBindTexture(GL_TEXTURE_2D, texture_red_cloth);
        gluSphere(quadric, 0.5f, 100, 100);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        glPopMatrix();

        glTranslatef(-1.2f, 1.5f, 0.4f);
        glScalef(0.50f, 0.75f, 0.75f);
        glRotatef(35.0f, -1.0f, 0.0f, 1.0f);
        glRotatef(10.0f, -1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glRotatef((GLfloat)shoulder, 0.0f, 0.0f, -1.0f);

        glTranslatef(-1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 1.0f, 1.0f);

        // Draw Arm

        // we are drawing sphere but we are scalling which will look like ellipse
        gluSphere(quadric, 0.5f, 100, 100);
        // band
        glPushMatrix();
        {
            SetLightColorGold();
            glScalef(0.2f, 1.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
            S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_2D, texture_skin);
            glColor3f(1.0f, 1.0f, 1.0f);
            SetLightColorWhite();
        }
        glPopMatrix();

        glPopMatrix();

        // Fore Arm
        glTranslatef(-0.8f, 0.0f, 0.0f);

        glRotatef((GLfloat)elbow, 0.0f, 0.0f, -1.0f);

        glTranslatef(-1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 0.7f, 1.0f);

        gluSphere(quadric, 0.5f, 100, 100);

        // band
        glPushMatrix();
        {
            glTranslatef(-0.45f, 0.0f, 0.0f);
            SetLightColorGold();
            glScalef(0.2f, 0.5f, 0.5f);
            glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
            S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_2D, texture_skin);
            glColor3f(1.0f, 1.0f, 1.0f);
            SetLightColorWhite();
        }
        glPopMatrix();

        glPopMatrix();

        glPushMatrix();

        // fore arm - palm
        glTranslatef(-0.8f, 0.0f, 0.0f);
        glRotatef(45.0f, 1.0f, 1.0f, 0.0f);

        glRotatef((GLfloat)wrist, 0.0f, 0.0f, -1.0f);

        glTranslatef(-0.4f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(0.6f, 0.5f, 0.2f);

        gluSphere(quadric, 0.5f, 30, 30);

        glPopMatrix();

        // FINGERS
        // 1
        glTranslatef(-0.3f, 0.2f, 0.0f);

        glPushMatrix();

        glScalef(0.3f, 0.1f, 0.2f);

        gluSphere(quadric, 0.5f, 30, 30);

        // 2
        glPopMatrix();
        glTranslatef(-0.1f, -0.12f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 30, 30);

        // 3
        glPopMatrix();
        glTranslatef(0.0f, -0.1f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        quadric = gluNewQuadric();

        gluSphere(quadric, 0.5f, 30, 30);

        // 4
        glPopMatrix();
        glTranslatef(0.0f, -0.09f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 30, 30);

        // 5
        glPopMatrix();
        glTranslatef(0.2f, -0.1f, 0.0f);
        glPushMatrix();

        glScalef(0.25f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 20, 30);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }

    glPopMatrix();

    // Neck
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadricCylinder_S3, 0.3f, 0.3f, 0.5f, 10, 10);

    glPopMatrix();

    // Head
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadric, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_face_Queen);

    glTranslatef(0.0f, 2.4f, 0.0f);
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    glScalef(0.9f, 1.0f, 1.0f);
    gluSphere(quadric, 0.58f, 30, 30);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslatef(0.0f, 2.0f, -0.5f);
    glColor3f(0.0f, 0.0f, 0.0f);
    gluSphere(quadric, 0.3f, 30, 30);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void drawAgniDev(void)
{
    // funnction declartions
    void S3_DrawBox(float, float, float, float, float);
    void S3_DrawBoxWithParam(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);
    // void colorSetcolorWithalpha(GLfloat r, GLfloat g, GLfloat b);

    // code
    glScalef(0.75f, 1.0f, 0.50f);

    if (bEnableAgniDevAlph == TRUE)
    {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    // UPPER BODY
    {
        glBindTexture(GL_TEXTURE_2D, texture_skin);

        // Shoulder + neck
        glPushMatrix();
        glTranslatef(0.0f, 1.4f, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, alpha_AgniDev);

        // TCHAR str[200];
        // swprintf_s(str, 200, TEXT("Alphs = %f \n"), alpha_AgniDev); // "_s" for secure

        // SetWindowText(ghwnd, str);

        S3_DrawBox(0.4f, 1.3f, 1.5f, 1.3f, 1.5f);

        glPopMatrix();

        // left shoulder
        glPushMatrix();
        glTranslatef(-0.5f, 1.4f, 0.0f);
        S3_DrawBox(0.4f, 1.1f, 0.6f, 1.2f, 1.5f);
        glPopMatrix();

        // Right shoulder
        glPushMatrix();
        glTranslatef(0.5f, 1.4f, 0.0f);
        S3_DrawBox(0.4f, 1.1f, 0.6f, 1.2f, 1.5f);
        glPopMatrix();

        // chest
        glPushMatrix();
        glTranslatef(0.0f, 0.5f, 0.0f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.5f, 1.0f);
        glPopMatrix();

        // Shela
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glRotatef(50.0f, 0.0f, 0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_red_cloth);
        S3_DrawBoxWithParam(2.3f, 2.0f, 0.2f, 0.7f, 1.5f, 1.5f);
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }

    // LOWER BODY - LEGS
    {

        // left leg
        glPushMatrix();
        glTranslatef(-0.3f, -1.3f, 0.0f);
        glScalef(0.40f, 0.75f, 0.75f);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.5f, 0.9f);
        glPopMatrix();

        // left bottom leg
        glPushMatrix();
        glTranslatef(-0.3f, -2.4f, 0.0f);
        glScalef(0.29f, 0.70f, 0.55f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.3f, 0.7f);
        glPopMatrix();

        // Right leg
        glPushMatrix();
        glTranslatef(0.3f, -1.3f, 0.0f);
        glScalef(0.40f, 0.75f, 0.75f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.5f, 1.0f);
        glPopMatrix();

        // Right bottom leg
        glPushMatrix();
        glTranslatef(0.3f, -2.4f, 0.0f);
        glScalef(0.29f, 0.70, 0.55f);
        S3_DrawBox(1.5f, 1.5f, 1.0f, 1.3f, 0.7f);
        glPopMatrix();

        // toes Left
        glPushMatrix();
        glTranslatef(-0.3f, -3.0f, 0.1f);
        S3_DrawBox(0.2f, 0.30f, 0.34f, 0.6f, 0.7f);
        glPopMatrix();

        // toes Right
        glPushMatrix();
        glTranslatef(0.3f, -3.0f, 0.1f);
        S3_DrawBox(0.2f, 0.30f, 0.34f, 0.6f, 0.7f);
        glPopMatrix();

        // Stomch bottom
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture_org_cloth);
        glTranslatef(0.0f, -0.2f, 0.0f);
        S3_DrawBoxWithParam(0.2f, 0.2f, 1.3f, 1.3f, 1.25f, 1.25);
        glPopMatrix();

        // bootom
        glPushMatrix();
        glTranslatef(0.0f, -0.8f, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, alpha_AgniDev);
        S3_DrawBox(0.2f, 1.2f, 1.2f, 1.25f, 1.2f);
        glPopMatrix();

        // stomoch
        glPushMatrix();
        glTranslatef(0.0f, -0.5f, 0.0f);
        S3_DrawBox(0.5f, 1.0f, 1.2f, 1.2f, 1.2f);
        glPopMatrix();

        // Right leg
        glPushMatrix();
        glTranslatef(0.3f, -1.3f, 0.1f);
        glColor4f(1.0f, 1.0f, 1.0f, alpha_AgniDev);
        glScalef(0.6f, 1.5f, 0.7f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        gluQuadricTexture(quadric, GL_TRUE);
        gluSphere(quadric, 1.0f, 100, 100);
        glPopMatrix();

        // Left leg
        glPushMatrix();
        glTranslatef(-0.3f, -1.3f, 0.1f);
        glColor4f(1.0f, 1.0f, 1.0f, alpha_AgniDev);
        glScalef(0.6f, 1.5f, 0.7f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        gluSphere(quadric, 1.0f, 100, 100);
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Right Hand
    glPushMatrix();
    {
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        glTranslatef(1.2f, 1.5f, 0.4f);
        glScalef(0.50f, 0.75f, 0.75f);

        glPushMatrix();
        glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
        glRotatef((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);
        glTranslatef(1.0f, 0.0f, 0.0f);
        glPushMatrix();

        glScalef(2.0f, 1.0f, 1.0f);

        // Draw Arm
        glColor4f(1.0f, 1.0f, 1.0f, alpha_AgniDev);
        gluSphere(quadric, 0.5f, 100, 100);

        SetLightColorGold();
        glScalef(0.5f, 1.1f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
        S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        glColor4f(1.0f, 1.0f, 1.0f, alpha_AgniDev);
        glPopMatrix();
        SetLightColorWhite();

        // Fore Arm
        glTranslatef(0.8f, 0.0f, 0.0f);

        glRotatef((GLfloat)elbow, 0.0f, 0.0f, 1.0f);
        glRotatef(45, 0.0f, -1.0f, 0.0f);

        glTranslatef(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 0.7f, 0.7f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        glPushMatrix();

        // fore arm - palm
        glTranslatef(0.8f, 0.0f, 0.0f);
        glRotatef(45.0f, 1.0f, .0f, 0.0f);

        glRotatef((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

        glTranslatef(0.4f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(0.6f, 0.5f, 0.2f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        // FINGERS
        // 1
        glTranslatef(0.3f, 0.2f, 0.0f);

        glPushMatrix();

        glScalef(0.3f, 0.1f, 0.2f);

        gluSphere(quadric, 0.5f, 10, 10);

        // 2
        glPopMatrix();
        glTranslatef(0.1f, -0.12f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 3
        glPopMatrix();
        glTranslatef(0.0f, -0.1f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        quadric = gluNewQuadric();

        gluSphere(quadric, 0.5f, 5, 5);

        // 4
        glPopMatrix();
        glTranslatef(0.0f, -0.09f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 5
        glPopMatrix();
        glTranslatef(-0.1f, -0.1f, 0.0f);
        glPushMatrix();

        glScalef(0.25f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }

    glPopMatrix();

    // Left Hand
    glPushMatrix();
    {

        glTranslatef(-1.2f, 1.5f, 0.4f);
        glScalef(0.50f, 0.75f, 0.75f);
        glRotatef(35.0f, -1.0f, 0.0f, 1.0f);
        glRotatef(10.0f, -1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glRotatef((GLfloat)shoulder + 6, 0.0f, 0.0f, -1.0f);

        glTranslatef(-1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 1.0f, 1.0f);

        // Draw Arm
        gluSphere(quadric, 0.5f, 100, 100);

        // band
        SetLightColorGold();
        glScalef(0.5f, 1.1f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_mahal_main_base);
        S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        SetLightColorWhite();

        glPopMatrix();

        // Fore Arm
        glTranslatef(-0.8f, 0.0f, 0.0f);

        glRotatef((GLfloat)elbow, 0.0f, 0.0f, -1.0f);
        glRotatef(35, 0.0f, 1.0f, 0.0f);

        glTranslatef(-1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 0.7f, 1.0f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        glPushMatrix();

        // fore arm - palm
        glTranslatef(-0.8f, 0.0f, 0.0f);
        glRotatef(45.0f, 1.0f, 0.0f, 0.0f);

        glRotatef((GLfloat)wrist, 0.0f, 0.0f, -1.0f);

        glTranslatef(-0.4f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(0.6f, 0.5f, 0.2f);

        gluSphere(quadric, 0.5f, 10, 10);

        glPopMatrix();

        // FINGERS
        // 1
        glTranslatef(-0.3f, 0.2f, 0.0f);

        glPushMatrix();

        glScalef(0.3f, 0.1f, 0.2f);

        gluSphere(quadric, 0.5f, 10, 10);

        // 2
        glPopMatrix();
        glTranslatef(-0.1f, -0.12f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 3
        glPopMatrix();
        glTranslatef(0.0f, -0.1f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        quadric = gluNewQuadric();

        gluSphere(quadric, 0.5f, 5, 5);

        // 4
        glPopMatrix();
        glTranslatef(0.0f, -0.09f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 5
        glPopMatrix();
        glTranslatef(0.2f, -0.1f, 0.0f);
        glPushMatrix();

        glScalef(0.25f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }

    glPopMatrix();

    // neck
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    //   colorSetcolorWithalpha(232, 195, 167);
    glColor4f(1.0f, 1.0f, 1.0f, alpha_AgniDev);
    gluCylinder(quadricCylinder_S3, 0.3f, 0.3f, 0.5f, 10, 10);

    glPopMatrix();

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadric, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_face);

    glTranslatef(0.0f, 2.4f, 0.0f);
    glColor4f(1.0f, 1.0f, 1.0f, alpha_AgniDev);
    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    gluSphere(quadric, 0.58f, 30, 30);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    // crown
    glPushMatrix();
    glTranslatef(0.0f, 3.5f, 0.0f);
    glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
    drawCrown(texture_crown_2);

    glPopMatrix();

    if (bEnableAgniDevAlph == TRUE)
    {
        glEnable(GL_LIGHTING);
    }
}

void drawHrushi(void)
{
    // funnction declartions
    void S3_DrawBox(float, float, float, float, float);
    void S3_DrawBoxWithParam(float leftHeight, float rightHeight, float topWidth, float bottomWidth, float topDepth, float bottomDepth);

    shoulder = -96.0f;
    elbow = -107;
    wrist = 3;

    // code
    glScalef(0.85f, 1.0f, 0.50f);
    // UPPER BODY
    {
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        // Shoulder + neck
        glPushMatrix();
        glTranslatef(0.0f, 1.4f, 0.0f);
        colorSetcolor(232, 176, 167);
        S3_DrawBox(0.4f, 1.3f, 1.5f, 1.3f, 1.5f);

        // left shoulder
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.5f, 1.4f, 0.0f);
        S3_DrawBox(0.4f, 1.1f, 0.6f, 1.2f, 1.5f);

        // Right shoulder
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.5f, 1.4f, 0.0f);
        S3_DrawBox(0.4f, 1.1f, 0.6f, 1.2f, 1.5f);

        // chest
        glPopMatrix();
        glPushMatrix();
        colorSetcolor(232, 195, 167);
        glTranslatef(0.0f, 0.5f, 0.0f);
        S3_DrawBox(1.5f, 1.5f, 1.2f, 1.5f, 1.0f);
        glPopMatrix();
    }

    // LOWER BODY - LEGS
    {
        glPushMatrix();
        glTranslatef(-1.0f, -0.5f, 0.2f);
        glRotatef(80.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(20.0f, 0.0f, 1.0f, 0.0f);
        glScalef(1.0f, 0.7f, 1.6f);
        {
            // left leg
            glPushMatrix();
            glTranslatef(-0.3f, -1.3f, 0.0f);
            glScalef(0.40f, 0.75f, 0.75f);
            colorSetcolor(232, 195, 167);
            glBindTexture(GL_TEXTURE_2D, texture_skin);
            S3_DrawBox(1.5f, 1.5f, 1.0f, 1.0f, 0.9f);
            glPopMatrix();

            // left bottom leg
            glPushMatrix();
            glTranslatef(-0.3f, -2.4f, 0.0f);
            glScalef(0.29f, 0.70f, 0.55f);
            S3_DrawBox(1.5f, 1.5f, 1.0f, 1.4f, 0.7f);
            glPopMatrix();

            // toes Left
            glPushMatrix();
            glTranslatef(-0.3f, -3.0f, 0.1f);
            S3_DrawBox(0.2f, 0.30f, 0.34f, 0.6f, 0.7f);
            glPopMatrix();

            // Left leg
            glPushMatrix();
            glTranslatef(-0.3f, -1.3f, 0.1f);
            glColor3f(1.0f, 1.0f, 1.0f);
            glScalef(0.6f, 1.5f, 0.7f);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            glBindTexture(GL_TEXTURE_2D, texture_org_cloth);
            gluSphere(quadric, 1.0f, 100, 100);
            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.8f, -0.5f, 0.2f);
        glRotatef(80.0f, 0.0f, 0.0f, -1.0f);
        glRotatef(20.0f, 0.0f, 1.0f, 0.0f);
        glScalef(1.0f, 0.7f, 1.6f);
        {
            // Right leg
            glPushMatrix();
            glTranslatef(0.3f, -1.3f, 0.0f);
            glScalef(0.40f, 0.75f, 0.75f);
            glBindTexture(GL_TEXTURE_2D, texture_skin);
            S3_DrawBox(1.5f, 1.5f, 1.0f, 1.0f, 1.0f);
            glPopMatrix();

            // Right bottom leg
            glPushMatrix();
            glTranslatef(0.3f, -2.4f, 0.0f);
            glScalef(0.29f, 0.70, 0.55f);
            S3_DrawBox(1.5f, 1.5f, 1.0f, 1.3f, 0.7f);
            glPopMatrix();

            // toes Right
            glPushMatrix();
            glTranslatef(0.3f, -3.0f, 0.1f);
            S3_DrawBox(0.2f, 0.30f, 0.34f, 0.6f, 0.7f);
            glPopMatrix();

            // Right leg
            glPushMatrix();
            glTranslatef(0.3f, -1.3f, 0.1f);
            glColor3f(1.0f, 1.0f, 1.0f);
            glScalef(0.6f, 1.5f, 0.7f);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            gluQuadricTexture(quadric, GL_TRUE);
            glBindTexture(GL_TEXTURE_2D, texture_org_cloth);
            gluSphere(quadric, 1.0f, 100, 100);
            glPopMatrix();
        }
        glPopMatrix();

        {
            // bootom
            glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture_org_cloth);
            glTranslatef(0.0f, -0.8f, 0.0f);
            glColor3f(1.0f, 1.0f, 1.0f);
            S3_DrawBox(0.2f, 1.2f, 1.2f, 1.25f, 1.2f);
            glPopMatrix();

            // stomoch
            glPushMatrix();
            glTranslatef(0.0f, -0.5f, 0.0f);
            S3_DrawBox(0.5f, 1.0f, 1.2f, 1.2f, 1.2f);
            glPopMatrix();
        }
    }

    // Right Hand
    glPushMatrix();
    {
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        glTranslatef(1.2f, 1.5f, 0.4f);
        glScalef(0.50f, 0.75f, 0.75f);
        glRotatef(35.0f, -1.0f, 0.0f, -1.0f);
        glRotatef(10.0f, -1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glRotatef((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);

        glTranslatef(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 1.0f, 1.0f);

        // Draw Arm
        // we are drawing sphere but we are scalling which will look like ellipse
        glColor3f(1.0f, 1.0f, 1.0f);
        gluSphere(quadric, 0.5f, 100, 100);

        SetLightColorGold();
        glScalef(0.5f, 1.1f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_org_cloth);
        S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
        SetLightColorWhite();

        // Fore Arm
        glTranslatef(0.8f, 0.0f, 0.0f);

        glRotatef((GLfloat)elbow, 0.0f, 0.0f, 1.0f);

        glTranslatef(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 0.7f, 1.0f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        glPushMatrix();

        // fore arm - palm
        glTranslatef(0.8f, 0.0f, 0.0f);
        glRotatef(45.0f, 1.0f, -1.0f, 0.0f);

        glRotatef((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

        glTranslatef(0.4f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(0.6f, 0.5f, 0.2f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        // FINGERS
        // 1
        glTranslatef(0.3f, 0.2f, 0.0f);

        glPushMatrix();

        glScalef(0.3f, 0.1f, 0.2f);

        gluSphere(quadric, 0.5f, 10, 10);

        // 2
        glPopMatrix();
        glTranslatef(0.1f, -0.12f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 3
        glPopMatrix();
        glTranslatef(0.0f, -0.1f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        quadric = gluNewQuadric();

        gluSphere(quadric, 0.5f, 5, 5);

        // 4
        glPopMatrix();
        glTranslatef(0.0f, -0.09f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 5
        glPopMatrix();
        glTranslatef(-0.1f, -0.1f, 0.0f);
        glPushMatrix();

        glScalef(0.25f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }
    glPopMatrix();

    // Left Hand
    glPushMatrix();
    {
        glTranslatef(-1.2f, 1.5f, 0.4f);
        glScalef(0.50f, 0.75f, 0.75f);
        glRotatef(35.0f, -1.0f, 0.0f, 1.0f);
        glRotatef(10.0f, -1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glRotatef((GLfloat)shoulder, 0.0f, 0.0f, -1.0f);

        glTranslatef(-1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 1.0f, 1.0f);

        // Draw Arm

        // we are drawing sphere but we are scalling which will look like ellipse
        gluSphere(quadric, 0.5f, 100, 100);

        // band
        SetLightColorGold();
        glScalef(0.5f, 1.1f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_org_cloth);
        S3_DrawBoxWithParam(1.0f, 0.95f, 0.3f, 0.3f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, texture_skin);
        SetLightColorWhite();
        glColor3f(1.0f, 1.0f, 1.00f);

        glPopMatrix();

        // Fore Arm
        glTranslatef(-0.8f, 0.0f, 0.0f);

        glRotatef((GLfloat)elbow, 0.0f, 0.0f, -1.0f);

        glTranslatef(-1.0f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(2.0f, 0.7f, 1.0f);

        gluSphere(quadric, 0.5f, 100, 100);

        glPopMatrix();

        glPushMatrix();

        // fore arm - palm
        glTranslatef(-0.8f, 0.0f, 0.0f);
        glRotatef(45.0f, 1.0f, 1.0f, 0.0f);

        glRotatef((GLfloat)wrist, 0.0f, 0.0f, -1.0f);

        glTranslatef(-0.4f, 0.0f, 0.0f);

        glPushMatrix();

        glScalef(0.6f, 0.5f, 0.2f);

        gluSphere(quadric, 0.5f, 50, 50);

        glPopMatrix();

        // FINGERS
        // 1
        glTranslatef(-0.3f, 0.2f, 0.0f);

        glPushMatrix();

        glScalef(0.3f, 0.1f, 0.2f);

        gluSphere(quadric, 0.5f, 10, 10);

        // 2
        glPopMatrix();
        glTranslatef(-0.1f, -0.12f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 3
        glPopMatrix();
        glTranslatef(0.0f, -0.1f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        quadric = gluNewQuadric();

        gluSphere(quadric, 0.5f, 5, 5);

        // 4
        glPopMatrix();
        glTranslatef(0.0f, -0.09f, 0.0f);

        glPushMatrix();

        glScalef(0.4f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        // 5
        glPopMatrix();
        glTranslatef(0.2f, -0.1f, 0.0f);
        glPushMatrix();

        glScalef(0.25f, 0.09f, 0.2f);

        gluSphere(quadric, 0.5f, 5, 5);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }

    // Neack
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    colorSetcolor(232, 195, 167);
    gluCylinder(quadricCylinder_S3, 0.3f, 0.3f, 0.5f, 10, 10);

    glPopMatrix();

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadric, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_face_hrushi);

    glTranslatef(0.0f, 2.3f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRotatef(18.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.75f, 1.0f, 1.0f);
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    gluSphere(quadric, 0.58f, 30, 30);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

void drawBowl(void)
{
    glPushMatrix();
    {
        glTranslatef(0.0f, -0.2f, 0.0f);
        gluQuadricNormals(quadric, GLU_FLAT);
        glScalef(0.8f, 0.25f, 0.8f);
        glColor3f(1.0f, 0.0f, 0.0f);
        gluSphere(quadric, 1.0f, 30.0f, 30.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        gluQuadricTexture(quadric, GL_TRUE);
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, texture_bowl);
        glColor3f(1.0f, 1.0f, 0.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        gluQuadricNormals(quadric, GLU_FLAT);
        gluSphere(quadric, 1.0f, 30.0f, 30.0f);
        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    glPopMatrix();
}

void drawCrown(GLuint texture)
{

    // Code
    SetLightColorGold();

    glPushMatrix();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    gluQuadricTexture(quadricCylinder_S3, GL_TRUE);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 0.0f);
    if (texture == texture_crown_2)
    {
        glColor4f(1.0f, 1.0f, 0.0f, alpha_AgniDev);
    }
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluQuadricNormals(quadricCylinder_S3, GLU_FLAT);
    gluCylinder(quadricCylinder_S3, 0.4f, 0.6f, 1.0f, 30.0f, 30.0f);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glPopMatrix();

    SetLightColorWhite();
}
