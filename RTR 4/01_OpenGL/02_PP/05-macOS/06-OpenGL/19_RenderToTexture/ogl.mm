#import <Foundation/Foundation.h>

#import <cocoa/cocoa.h>

#import <QuartzCore/CVDisplayLink.h>

#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>

#include "vmath.h"
using namespace vmath;

#include "sphere.h"

// Global function declarations
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);

// Global variable declarations
FILE *gpFile = NULL;

// interface declarations
@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@interface GLView : NSOpenGLView
@end


// Entry point function
int main(int argc, char* argv[]){

    // CODE
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
    
    NSApp = [NSApplication sharedApplication];
    
    // create app delegate
    AppDelegate *appDelegate = [[AppDelegate alloc]init];
    
    // Give our app delegate to NSApp
    [NSApp setDelegate:appDelegate];
    
    // Start Game loop
    [NSApp run];
    
    [pool release];
    
    return 0;
}

@implementation AppDelegate
{
    @private
    NSWindow *window;
    GLView *view;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    // CODE
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appDirPath = [appBundle bundlePath];
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/log.txt",parentDirPath];
    
    if(logFileNameWithPath == nil){
        fprintf(gpFile, "logFileNameWithPath == path not found\n");
        [NSApp terminate:self];
    }
    
    const char *pszLogFileNameWithPath = [logFileNameWithPath UTF8String];
    gpFile = fopen(pszLogFileNameWithPath, "w");
    if(gpFile == NULL){
        [NSApp terminate:self];
    }
    
    fprintf(gpFile, "program started successfully\n");
    
    NSRect rect = NSMakeRect(0.0, 0.0, 800, 600);
    
    window = [[NSWindow alloc]initWithContentRect:rect
                                        styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                          backing:NSBackingStoreBuffered
                                            defer:NO];
    [window setTitle:@"MacOSWindow : PRJ"];
    NSColor *backgroundColor = [NSColor blackColor];
    [window setBackgroundColor:backgroundColor];
    
    [window center];
    
    view = [[GLView alloc]initWithFrame:rect];
    
    [window setContentView:view];
    
    // Set windows delegate
    [window setDelegate:self];
    
    [window makeKeyAndOrderFront:self];
    
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
    // CODE
    if(gpFile){
        fprintf(gpFile, "program finish successfully\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}

- (void)windowWillClose:(NSNotification *)notification
{
    // CODE
    [NSApp terminate:self];
}

- (void)dealloc
{
    // CODE
    if(view){
        [view release];
        view = nil;
    }
    if(window){
        [window release];
        window = nil;
    }
    [super dealloc];
}

@end

@implementation GLView

{
    @private
    CVDisplayLinkRef displayLink;
    
    // PP Related Global Variables
    GLuint shaderProgramObject;

    enum
    {
        PRJ_ATRIBUTE_POSITION = 0,
        PRJ_ATRIBUTE_COLOR,
        PRJ_ATRIBUTE_NORMAL,
        PRJ_ATRIBUTE_TEXTURE0
    };

    // VAO VBO RELATED VARIABLES
    GLuint vao_Cube;		  // Vertex Array Object - Cube
    GLuint vbo_Cube_Position; // Vertex Buffer Object - Cube- Position
    GLuint vbo_Cube_TexCoord; // Vertex Buffer Object - Cube - Color

    // UNIFORMS RELATED VARIABLES
    GLuint mvpMatrixUniform; // model View Projection
    mat4 perspectiveProjectionMatrix;

    // TEXTURE CUBE RELATED VARIABLES
    GLfloat angleCube;
    GLuint textureSamplerUniform;

    // FBO RELATED GLOBAL VARIABLES
    GLuint fbo; // FRAME BUFFER OBJECT
    GLuint rbo; // RENDER BUFFER OBJECT
    GLuint fbo_texture;
    bool bBFOResult;

    // TEXTURE SCENE GLOBAL VARIABLES
    GLuint shaderProgramObject_sphere;

    // UNIFORM RELATED VARIABLES
    GLuint modelMatrixUniform_sphere;
    GLuint viewMatrixUniform_sphere;
    GLuint projectionMatrixUniform_sphere;

    mat4 perspectiveProjectionMatrix_sphere;

    // LIGHT RELATED VARIABLES
    // For Three Light on Sphere
    GLuint laUniform_sphere[3];			  // light Ambiant
    GLuint ldUniform_sphere[3];			  // light Diffuse
    GLuint lsUniform_sphere[3];			  // light Spec
    GLuint lighPositionUniform_sphere[3]; // light Position

    GLuint kaUniform_sphere; // material amb
    GLuint kdUniform_sphere; // mat diff
    GLuint ksUniform_sphere; // mat specular
    GLuint materialShininessUniform_sphere;

    GLuint lightingEnabledUniform_sphere;

    struct Light
    {
        vmath::vec4 lightAmbiant;
        vmath::vec4 lightDiffuse;
        vmath::vec4 lightSpecular;
        vmath::vec4 lightPositions;
    };

    struct Light lights_sphere[3];

    vec4 materialAmbiant_sphere;
    vec4 meterialDiffuse_sphere;
    vec4 materialSpecular_sphere;
    GLfloat materialShineeness_sphere;

    GLfloat lightAngleOne_sphere;
    GLfloat lightAngleTwo_sphere;
    GLfloat lightAngleZero_sphere;

    BOOL bLight;

	int winWidth;
	int winHeight;
    #define FBO_WIDTH 512
    #define FBO_HEIGHT 512
}

- (void)initializeVariables
{
    materialAmbiant_sphere = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    meterialDiffuse_sphere = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialSpecular_sphere = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialShineeness_sphere = 124.0f;

    lightAngleOne_sphere = 0.0f;
    lightAngleTwo_sphere = 150.0f;
    lightAngleZero_sphere = 300.0f;
}

- (id)initWithFrame:(NSRect)frame
{
    // CODE
    self = [super initWithFrame:frame];
    
    if(self){
        // Initialize the array of opengl pixel format attributes
        NSOpenGLPixelFormatAttribute openGLPixelFormatAttributes[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
            NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFADoubleBuffer,
            0
        };
        
        // Create opengl pixel format using above attributes
        NSOpenGLPixelFormat *glPixelFormat = [[[NSOpenGLPixelFormat alloc]initWithAttributes:openGLPixelFormatAttributes]autorelease];
        
        if(glPixelFormat == nil){
            fprintf(gpFile, "Failed to create glPixelFormat\n");
            [self uninitialise];
            [self release];
            [NSApp terminate:self];
        }
        
        // create opengl context using above pixel format
        NSOpenGLContext* glContext = [[[NSOpenGLContext alloc]initWithFormat:glPixelFormat shareContext:nil]autorelease];
        
        if(glContext == nil){
            fprintf(gpFile, "Failed to create glContext\n");
            [self uninitialise];
            [self release];
            [NSApp terminate:self];
        }

        // set view's pixel format
        [self setPixelFormat:glPixelFormat];
        
        // set view's opengl context
        [self setOpenGLContext:glContext];
    }
    
    return self;
}

// Define getFrameForTime custome method with predefined signature
- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    // CODE
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
    
    [self drawView];
    
    [pool release];
    
    return kCVReturnSuccess;
    
}

- (void)prepareOpenGL
{
    // CODE
    [super prepareOpenGL];
    
    // make opengl context current
    [[self openGLContext]makeCurrentContext];
    
    // set double buffer swapping interval to 1
    GLint swapInterval = 1;
    
    [[self openGLContext]setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
    
    // opengl log
    fprintf(gpFile, "OpenGL renderer = %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL version = %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "glsl version = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    // Call initialise
    [self initialise];
    
    // create displayLink
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // set callback
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
    
    // convert NSOpenGL pixel format to CGL pixel format
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];
    
    // convert NSOpenGL context to CGL context
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];
    
    // using above info set current CG display to CGL pixel format
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    // start displayLink
    CVDisplayLinkStart(displayLink);
}

- (void) reshape
{
    // CODE
    [super reshape];
    
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    
    NSRect rect = [self bounds];
    
    int width = rect.size.width;
    int height = rect.size.height;
    
    [self resize:width :height];
    
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
}

- (void) drawRect:(NSRect)dirtyRect
{
    // CODE
    
    [self drawView];
}

- (void) drawView
{
    // CODE
    [[self openGLContext]makeCurrentContext];
    
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    
    // call display
    [self display];
    
	[self myupdate];

    // Do double buffering
    CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
    
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
}

- (int) initialise
{
    // CODE
    [self initializeVariables];    

	// vartex Shader
	const GLchar *vertexShaderSourceCode =
		"#version 410 core"
		"\n"
		"in vec4 a_position;"
		"in vec2 a_texcoord;"
		"uniform mat4 u_mvpMatrix;"
		"out vec2 a_texcoord_out;"
		"void main(void)"
		"{"
		"gl_Position = u_mvpMatrix * a_position;"
		"a_texcoord_out = a_texcoord;"
		"}";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObject);

	GLint status;
	GLint infoLogLength;
	char *log = NULL;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "VERTEX SHADER COMPILATION LOG : %s\n", log);
				free(log);
				log = NULL;
				[self uninitialise];
                [self release];
                [NSApp terminate:self];
			}
		}
	}

	// fragment Shader

	status = 0;
	infoLogLength = 0;

	const GLchar *fragmentShaderSourceCode =
		"#version 410 core"
		"\n"
		"in vec2 a_texcoord_out;"
		"uniform sampler2D u_textureSampler;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"FragColor = texture(u_textureSampler , a_texcoord_out);"
		"}";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "FRAGMENT SHADER COMPILATION LOG : %s\n", log);
				free(log);
				log = NULL;
				[self uninitialise];
                [self release];
                [NSApp terminate:self];
			}
		}
	}

	// Shader Program Object
	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);

	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// prelinked binding
	// Binding Position Array
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_POSITION, "a_position");
	// Binding texcoord Array
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_TEXTURE0, "a_texcoord");

	// link
	glLinkProgram(shaderProgramObject);

	status = 0;
	infoLogLength = 0;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
				fprintf(gpFile, "SHADER PROGRAM LINK LOG: %s \n", log);
				free(log);
				log = NULL;
				[self uninitialise];
                [self release];
                [NSApp terminate:self];
			}
		}
	}

	// post link - getting
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

	const GLfloat CubePosition[] = {
		// top
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		// bottom
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		// front
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		// back
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		// right
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		// left
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f};

	const GLfloat CubeTexCoord[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f};

	// vao for Cube
	glGenVertexArrays(1, &vao_Cube);
	glBindVertexArray(vao_Cube);

	// vbo for position
	glGenBuffers(1, &vbo_Cube_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_Position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CubePosition), CubePosition, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Texcoord
	glGenBuffers(1, &vbo_Cube_TexCoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_TexCoord);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeTexCoord), CubeTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // ubind vao for Cube

	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// glShadeModel(GL_SMOOTH);

	/* Clear the  screen using blue color */
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Enabaling the texture
	glEnable(GL_TEXTURE_2D);

	perspectiveProjectionMatrix = mat4::identity();

	// warmup resize call
	// [self resize:WINWIDTH :WINHEIGHT];

	// FBO CODE
	bBFOResult = [self CreateFBO:FBO_WIDTH :FBO_HEIGHT];

	int iRetVal;
	if (bBFOResult == true)
	{
		iRetVal = [self initialize_sphere:FBO_WIDTH :FBO_HEIGHT];
	}
	else
	{
		fprintf(gpFile, "CREATE FBO FAILED...\n");
	}

    return 0;
}

- (int) initialize_sphere:(int)width :(int)height
{
	/* fucntion delcations */

	// vartex Shader
	const GLchar *vertexShaderSourceCode =
		"#version 410 core"
		"\n"
		"in vec4 a_position;"
		"in vec3 a_normal;"
		"uniform vec4 u_lightPosition[3];"
		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"uniform int u_lightingEnabled;"
		"out vec3 transformedNormals;"
		"out vec3 lightDirection[3];"
		"out vec3 viewerVector;"
		"void main(void)"
		"{"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;"
		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);"
		"transformedNormals = normalize(normalMatrix * a_normal);"
		"viewerVector = normalize(-eyeCoordinates.xyz);"
		"for(int i = 0 ; i < 3 ; i++)"
		"{"
		"lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);" // Swizaling
		"}"
		"}"
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;"
		"}";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObject);

	GLint status;
	GLint infoLogLength;
	char *log = NULL;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "VERTEX SHADER COMPILATION LOG FOR SPHERE: %s\n", log);
				free(log);
				log = NULL;
				[self uninitialize_sphere];
			}
		}
	}

	// fragment Shader

	status = 0;
	infoLogLength = 0;

	const GLchar *fragmentShaderSourceCode =
		"#version 410 core"
		"\n"
		"in vec3 transformedNormals;"
		"in vec3 lightDirection[3];"
		"in vec3 viewerVector;"
		"uniform vec3 u_la[3];"
		"uniform vec3 u_ld[3];"
		"uniform vec3 u_ls[3];"
		"uniform vec4 u_lightPosition[3];"
		"uniform vec3 u_ka;"
		"uniform vec3 u_ks;"
		"uniform vec3 u_kd;"
		"uniform float u_materialShininnes;"
		"uniform int u_lightingEnabled;"
		"out vec4 FragColor;"
		"vec3 phong_ads_light;"
		"void main(void)"
		"{"
		"phong_ads_light = vec3(0.0,0.0,0.0);"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec3 ambiant[3];"
		"vec3 diffuse[3];"
		"vec3 reflectionVector[3];"
		"vec3 specular[3];"
		"for(int i = 0 ; i < 3 ; i++)"
		"{"
		"ambiant[i] = u_la[i] * u_ka;"
		"diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i] ,transformedNormals),0.0);"
		"reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);"
		"specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininnes);"
		"phong_ads_light = phong_ads_light + ambiant[i] + diffuse[i] +  specular[i];"
		"}"
		"}"
		"else"
		"{"
		"phong_ads_light = vec3(1.0,1.0,1.0);"
		"}"
		"FragColor = vec4(phong_ads_light, 1.0);"
		"}";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "FRAGMENT SHADER COMPILATION LOG FOR SPHERE: %s\n", log);
				free(log);
				log = NULL;
				[self uninitialize_sphere];
			}
		}
	}

	// Shader Program Object
	shaderProgramObject_sphere = glCreateProgram();

	glAttachShader(shaderProgramObject_sphere, vertexShaderObject);
	glAttachShader(shaderProgramObject_sphere, fragmentShaderObject);

	// prelinked binding
	glBindAttribLocation(shaderProgramObject_sphere, PRJ_ATRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject_sphere, PRJ_ATRIBUTE_NORMAL, "a_normal");

	// link
	glLinkProgram(shaderProgramObject_sphere);

	status = 0;
	infoLogLength = 0;

	glGetProgramiv(shaderProgramObject_sphere, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject_sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject_sphere, infoLogLength, &written, log);
				fprintf(gpFile, "SHADER PROGRAM LINK LOG FOR SPHER: %s \n", log);
				free(log);
				log = NULL;
				[self uninitialize_sphere];
			}
		}
	}

	// post link - getting
	modelMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_modelMatrix");
	viewMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_viewMatrix");
	projectionMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_projectionMatrix");

	laUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[0]");
	ldUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[0]");
	lsUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[0]");
	lighPositionUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[0]");

	laUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[1]");
	ldUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[1]");
	lsUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[1]");
	lighPositionUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[1]");

	laUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[2]");
	ldUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[2]");
	lsUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[2]");
	lighPositionUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[2]");

	kaUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_ka");
	kdUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_kd");
	ksUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_ks");
	materialShininessUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_materialShininnes");

	lightingEnabledUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_lightingEnabled");

	// declartions of vertex Data array
	// getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	// gNumVertices_sphere = getNumberOfSphereVertices();
	// gNumElements_sphere = getNumberOfSphereElements();

    newSphere(fRadius,iSlices,iStacks,(&sphereVertices_new),(&sphereNormals_new),&iNoOfVertices,false,(&sphereElements_new));


    // VAO and VBO related code
    glGenVertexArrays(1, &Vao_sphere);
    glBindVertexArray(Vao_sphere);
 
    // Position vbo
    glGenBuffers(1, &Vbo_sphere_position);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo_sphere_position);
    glBufferData(GL_ARRAY_BUFFER, 12 * iNoOfVertices, sphereVertices_new, GL_STATIC_DRAW);
    glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Normal vbo
    glGenBuffers(1, &Vbo_sphere_normal);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo_sphere_normal);
    glBufferData(GL_ARRAY_BUFFER, 12 * iNoOfVertices, sphereNormals_new, GL_STATIC_DRAW);
    glVertexAttribPointer(PRJ_ATRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(PRJ_ATRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Elements vbo
    glGenBuffers(1, &Vbo_sphere_element);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort) * iNoOfElements,sphereElements_new, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
     
    glBindVertexArray(0);




	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* Clear the  screen using black color */

	lights_sphere[0].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[0].lightDiffuse = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[0].lightSpecular = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[0].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights_sphere[1].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[1].lightDiffuse = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights_sphere[1].lightSpecular = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights_sphere[1].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights_sphere[2].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[2].lightDiffuse = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights_sphere[2].lightSpecular = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights_sphere[2].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix_sphere = mat4::identity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// warmup resize call
	[self resize_sphere:FBO_WIDTH :FBO_HEIGHT];

	return (0);
}


- (void) resize:(int)width :(int)height
{
    // CODE
    if(height < 0){
        height = 1;
    }
    
	winWidth = width;
	winHeight = height;

    [self resize_sphere:width :height];

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}

-(void)resize_sphere:(int)width :(int)height
{
	/* code */
	if (height == 0) // to avoid devided by zero
		height = 1;

	glViewport(0, 0, width, height);

	perspectiveProjectionMatrix_sphere = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}

-   (bool)CreateFBO:(GLint)textureWidth :(GLint)textureHeight
{
	//
	int maxRenderbufferSize;

	// CODE
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);

	// CHECK AVAILABLE BUFFER SIZE
	if (maxRenderbufferSize < textureWidth || maxRenderbufferSize < textureHeight)
	{
		fprintf(gpFile, "UnSufficient Render Buffer Size...\n");
		return (false);
	}

	// CREATE FRAME BUFFER OBJECT
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// CREATE RENDER BUFFER OBJECT
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	// WHERE TO KEEP THIS RENDER BUFFER AND WHAT WILL BE THE FORMAT OF RENDER BUFFER
	glRenderbufferStorage(
		GL_RENDERBUFFER,
		GL_DEPTH_COMPONENT16, // NOT RELATED TO DEPTH - BUT 16 MACRO SIZE THIS ONE HENCE USED HERE
		textureWidth,
		textureHeight);

	// CREATE EMPTY TEXTURE
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		textureWidth,
		textureHeight,
		0,
		GL_RGB,
		GL_UNSIGNED_SHORT_5_6_5,
		NULL // NULL - EMPTY TEXTURE
	);

	// GIVE ABOVE TEXTURE TO FBO
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		fbo_texture,
		0 // MIPMAP LEVEL
	);

	// GIVE RENDER BUFFER TO FBO
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		rbo //
	);

	// CHECK WATHER FBO IS CREATED SUCCESSFULLY OR NOT
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "FRAME BUFFER IS NOT COMPLETE...\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // IMPLICITEL UNBINDS THE RBO AND TEXTURE BUFFER OBJECT

	return true;
}


- (void)uninitialize_sphere
{
	
	if (shaderProgramObject_sphere)
	{
		glUseProgram(shaderProgramObject_sphere);

		GLsizei numAttachedShaders;

		glGetProgramiv(shaderProgramObject_sphere, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint *shaderObject = NULL;
		shaderObject = (GLuint *)malloc(numAttachedShaders * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject_sphere, numAttachedShaders, &numAttachedShaders, shaderObject);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject_sphere, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject_sphere);
		shaderProgramObject_sphere = 0;
	}
}


- (void) display
{
    // CODE
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Code */
	if (bBFOResult == true)
	{
		[self display_sphere:FBO_WIDTH :FBO_HEIGHT];
		[self update_sphere];
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	[self resize:winWidth :winHeight];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	// Cube
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix_X = mat4::identity();
	mat4 rotationMatrix_Y = mat4::identity();
	mat4 rotationMatrix_Z = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); // glTranslatef() is replaced by this line

	rotationMatrix_X = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	rotationMatrix_Y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	rotationMatrix_Z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
	rotationMatrix = rotationMatrix_X * rotationMatrix_Y * rotationMatrix_Z;

	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glActiveTexture(GL_TEXTURE0); //
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(textureSamplerUniform, 0); //

	glBindVertexArray(vao_Cube);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);

}

- (void) myupdate
{
    // CODE
    angleCube = angleCube + 2.1f;
	if (angleCube >= 360.0f)
		angleCube = angleCube - 360.0f;

	// Set Light Zero Position - rotating zero light - x Around
	float angle = lightAngleZero_sphere * (M_PI / 180.0f);
	float x = 10.0f * cos(angle);
	float y = 10.0f * sin(angle);
	lights_sphere[0].lightPositions[1] = x;
	lights_sphere[0].lightPositions[2] = y;

	// Set Light One Position  rotating One Light - Y Rotation
	angle = (lightAngleOne_sphere * M_PI) / 180.0f;
	x = 10.0f * cos(angle);
	y = 10.0f * sin(angle);
	lights_sphere[1].lightPositions[0] = x;
	lights_sphere[1].lightPositions[2] = y;

	// Set Light Two Position rotating Two Light Z Rotation
	angle = (lightAngleTwo_sphere * M_PI) / 180.0f;
	x = 10.0f * cos(angle);
	y = 10.0f * sin(angle);
	lights_sphere[2].lightPositions[0] = x;
	lights_sphere[2].lightPositions[1] = y;
}

- (void)display_sphere:(GLint)textureWidth :(GLint)textureHeight
{
	/* Code */
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	[self resize_sphere:textureWidth :textureHeight];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject_sphere);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); // glTranslatef() is replaced by this line

	modelMatrix = translationMatrix;

	glUniformMatrix4fv(modelMatrixUniform_sphere, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform_sphere, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform_sphere, 1, GL_FALSE, perspectiveProjectionMatrix_sphere);

	// Light Related Code

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform_sphere, 1);

		for (int i = 0; i < 3; i++)
		{
			glUniform3fv(laUniform_sphere[i], 1, lights_sphere[i].lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
			glUniform3fv(ldUniform_sphere[i], 1, lights_sphere[i].lightDiffuse);
			glUniform3fv(lsUniform_sphere[i], 1, lights_sphere[i].lightSpecular);
			glUniform4fv(lighPositionUniform_sphere[i], 1, lights_sphere[i].lightPositions);
		}

		glUniform3fv(kaUniform_sphere, 1, materialAmbiant_sphere);
		glUniform3fv(kdUniform_sphere, 1, meterialDiffuse_sphere);
		glUniform3fv(ksUniform_sphere, 1, materialSpecular_sphere);
		glUniform1f(materialShininessUniform_sphere, materialShineeness_sphere);
	}
	else
	{
		glUniform1i(lightingEnabledUniform_sphere, 0);
	}

	glBindVertexArray(Vao_sphere);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element);
    glDrawElements(GL_TRIANGLE_STRIP,iNoOfElements,GL_UNSIGNED_SHORT,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unuse the shader program object
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

- (void)update_sphere
{
	/* code */
	lightAngleZero_sphere = lightAngleZero_sphere + 2.1f;
	if (lightAngleZero_sphere > 360.0f)
		lightAngleZero_sphere = lightAngleZero_sphere - 360.0f;

	lightAngleOne_sphere = lightAngleOne_sphere + 2.1f;
	if (lightAngleOne_sphere > 360.0f)
		lightAngleOne_sphere = lightAngleOne_sphere - 360.0f;

	lightAngleTwo_sphere = lightAngleTwo_sphere + 2.1f;
	if (lightAngleTwo_sphere > 360.0f)
		lightAngleTwo_sphere = lightAngleTwo_sphere - 360.0f;
}


- (void) uninitialise
{
    // CODE
	
	if (vbo_Cube_TexCoord)
	{
		glDeleteBuffers(1, &vbo_Cube_TexCoord);
		vbo_Cube_TexCoord = 0;
	}

	// delete vbo_Cube_Position
	if (vbo_Cube_Position)
	{
		glDeleteBuffers(1, &vbo_Cube_Position);
		vbo_Cube_Position = 0;
	}

	// deletion of vao_Cube
	if (vao_Cube)
	{
		glDeleteVertexArrays(1, &vao_Cube);
		vao_Cube = 0;
	}

	if (shaderProgramObject)
	{
		glUseProgram(shaderProgramObject);

		GLsizei numAttachedShaders;

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint *shaderObject = NULL;
		shaderObject = (GLuint *)malloc(numAttachedShaders * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject, numAttachedShaders, &numAttachedShaders, shaderObject);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}
}

- (BOOL)acceptsFirstResponder
{
    // CODE
    [[self window]makeFirstResponder:self];
    return YES;
}

- (void)keyDown:(NSEvent *)event
{
    // CODE
    int key = (int)[[event characters]characterAtIndex:0];
    
    switch(key){
        case 27:
            [self uninitialise];
            [self release];
            [NSApp terminate:self];
            break;

		case 'l':
		case 'L':
			if(bLight)
			{
				bLight = FALSE;
			}
			else
			{
				bLight = TRUE;
			}
			break;

        case 'F':
        case 'f':
            [[self window]toggleFullScreen:self];
            break;
            
        default:
            break;
    }
}

- (void)mouseDown:(NSEvent *)event{
    // CODE
    
}

- (void)dealloc
{
    // CODE
    [super dealloc];
    
    if(displayLink){
        CVDisplayLinkStop(displayLink);
        CVDisplayLinkRelease(displayLink);
        displayLink = nil;
    }
}

@end

// implement the display link callback function
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* currentTime, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* view)
{
    // CODE
    CVReturn result = [(GLView*)view getFrameForTime:outputTime];
    
    return result;
}

