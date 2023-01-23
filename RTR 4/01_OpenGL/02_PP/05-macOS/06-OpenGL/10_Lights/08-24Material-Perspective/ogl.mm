#import <Foundation/Foundation.h>

#import <cocoa/cocoa.h>

#import <QuartzCore/CVDisplayLink.h>

#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>

#include "vmath.h"
using namespace vmath;

#include "Sphere.h"

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
    
    // OPENGL RELATED VARIABLES
    enum
    {
    	PRJ_ATRIBUTE_POSITION = 0,
    	PRJ_ATRIBUTE_COLOR,
    	PRJ_ATRIBUTE_NORMAL,
    	PRJ_ATRIBUTE_TEXTURE0
    };

    GLuint shaderProgramObject;

	GLuint gVao_sphere;			 // Vertex Array Object
	GLuint gVbo_sphere_position; // Vertex Buffer Object
	GLuint gVbo_sphere_normal;
	GLuint gVbo_sphere_element;

	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLuint projectionMatrixUniform;

	GLuint laUniform;			// light Ambiant
	GLuint ldUniform;			// light Diffuse
	GLuint lsUniform;			// light Spec
	GLuint lighPositionUniform; // light Position

	GLuint kaUniform; // material amb
	GLuint kdUniform; // mat diff
	GLuint ksUniform; // mat specular
	GLuint materialShininessUniform;

	GLuint lightingEnabledUniform;

	mat4 perspectiveProjectionMatrix;


	BOOL bLight ;

	vmath::vec4 lightAmbiant ;
	vmath::vec4 lightDiffuse ;
	vmath::vec4 lightSpecular ;
	vmath::vec4 lightPositions ;

	vmath::vec4 materialAmbiant ;
	vmath::vec4 materialDiffuse ;
	vmath::vec4 materialSpecular;
	GLfloat materialShinniness ;

	GLfloat angleForRotation;

	GLint keyPressed ;

	int currentWindowWidth;
	int currentWindowHeight;

	Sphere *sphere;
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
    
	// vartex Shader
	const GLchar *vertexShaderSourceCode =
		"#version 410 core"
		"\n"
		"in vec4 a_position;"
		"in vec3 a_normal;"
		"uniform vec4 u_lightPosition;"
		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"uniform int u_lightingEnabled;"
		"out vec3 transformedNormals;"
		"out vec3 lightDirection;"
		"out vec3 viewerVector;"
		"void main(void)"
		"{"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;"
		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);"
		"transformedNormals = normalize(normalMatrix * a_normal);"
		"viewerVector = normalize(-eyeCoordinates.xyz);"
		"lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);" // Swizaling
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
		"in vec3 transformedNormals;"
		"in vec3 lightDirection;"
		"in vec3 viewerVector;"
		"uniform vec3 u_la;"
		"uniform vec3 u_ld;"
		"uniform vec3 u_ls;"
		"uniform vec4 u_lightPosition;"
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
		"vec3 ambiant;"
		"vec3 diffuse;"
		"vec3 reflectionVector;"
		"vec3 specular;"
		"ambiant = u_la * u_ka;"
		"diffuse = u_ld * u_kd * max(dot(lightDirection ,transformedNormals),0.0);"
		"reflectionVector = reflect(-lightDirection,transformedNormals);"
		"specular = u_ls * u_ks * pow(max(dot(reflectionVector, viewerVector), 0.0), u_materialShininnes);"
		"phong_ads_light = phong_ads_light + ambiant + diffuse +  specular;"
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
	// pr
	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);

	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// prelinked binding
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_NORMAL, "a_normal");

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
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");

	laUniform = glGetUniformLocation(shaderProgramObject, "u_la");
	ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
	lsUniform = glGetUniformLocation(shaderProgramObject, "u_ls");
	lighPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");

	kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
	kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
	materialShininessUniform = glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

	lightingEnabledUniform = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

	sphere = [[Sphere alloc]init];
    [sphere initialize:0.5 :100 :100];
	// vao and vbo related code
	// vao for Sphere
	glGenVertexArrays(1, &gVao_sphere);
	glBindVertexArray(gVao_sphere);

	// vbo for position
	glGenBuffers(1, &gVbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, [sphere getNumberOfSphereVertices] * sizeof(GLfloat), [sphere getSphereVertex], GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &gVbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, [sphere getNumberOfSphereNormal] * sizeof(GLfloat), [sphere getSphereNormal], GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Depth Related Changes
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    perspectiveProjectionMatrix = mat4::identity();

	bLight = FALSE;

	lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lightDiffuse = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightSpecular = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	materialAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	materialDiffuse = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialSpecular = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialShinniness = 120.0f;

	angleForRotation = 0.0f;

	keyPressed = 0;


    return 0;
}

- (void) resize:(int)width :(int)height
{
    // CODE
    if(height < 0){
        height = 1;
    }
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	currentWindowWidth = width;
	currentWindowHeight = height;

    perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}

- (void) display
{
    // CODE
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	[self draw24Sphere];

	// use shader program obejct
	glUseProgram(0);

}

- (void) myupdate
{
    // CODE
	if (keyPressed == 1 || keyPressed == 2 || keyPressed == 3)
	{
		angleForRotation = angleForRotation + 1.0f;
		if (angleForRotation > 360.0f)
			angleForRotation = angleForRotation - 360.0f;

		// Set Light Zero Position - rotating  light
		GLfloat angle = (angleForRotation * M_PI) / 180.0f;
		GLfloat x = 10.0f * cos(angle);
		GLfloat y = 10.0f * sin(angle);

		if (keyPressed == 1)
		{
			lightPositions[1] = x;
			lightPositions[2] = y;
		}
		if (keyPressed == 2)
		{
			lightPositions[0] = x;
			lightPositions[2] = y;
		}
		if (keyPressed == 3)
		{
			lightPositions[0] = x;
			lightPositions[1] = y;
		}
	}
}

- (void)draw24Sphere
{
	// Variable Declartions
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	// Code

	int horDistance = currentWindowWidth / 6;
	int verDistance = currentWindowHeight / 7;

	scaleMatrix = vmath::scale(1.3f, 1.3f, 1.3f);

	// ***** 1st sphere on 1st column, emerald *****
	// translationMatrix = vmath::translate(-9.0f, 6.0f, -21.0f); // glTranslatef() is replaced by this line
	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glViewport(0.0f, verDistance * 5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.0215; // r
	materialAmbiant[1] = 0.1745; // g
	materialAmbiant[2] = 0.0215; // b
	materialAmbiant[3] = 1.0f;	 // a

	// diffuse material
	materialDiffuse[0] = 0.07568; // r
	materialDiffuse[1] = 0.61424; // g
	materialDiffuse[2] = 0.07568; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.633;	// r
	materialSpecular[1] = 0.727811; // g
	materialSpecular[2] = 0.633;	// b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.6 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 2
	// ***** 2nd sphere on 1st column, jade *****
	//	glViewport(0.0f, (GLsizei)(WINHEIGHT) - (WINHEIGHT) / 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	// glViewport(0.0f, 120 * 4.4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(0.0f, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.135;	 // r
	materialAmbiant[1] = 0.2225; // g
	materialAmbiant[2] = 0.1575; // b
	materialAmbiant[3] = 1.0f;	 // a

	// diffuse material
	materialDiffuse[0] = 0.54; // r
	materialDiffuse[1] = 0.89; // g
	materialDiffuse[2] = 0.63; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.316228; // r
	materialSpecular[1] = 0.316228; // g
	materialSpecular[2] = 0.316228; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.1 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// ***** 3rd sphere on 1st column, obsidian *****
	// 3
	// glViewport(0.0f, (GLsizei)(WINHEIGHT) - ((WINHEIGHT) / 2), (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	// glViewport(0.0f, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(0.0f, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.05375; // r
	materialAmbiant[1] = 0.05;	  // g
	materialAmbiant[2] = 0.06625; // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.18275; // r
	materialDiffuse[1] = 0.17;	  // g
	materialDiffuse[2] = 0.22525; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.332741; // r
	materialSpecular[1] = 0.328634; // g
	materialSpecular[2] = 0.346435; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.3 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 4
	// ***** 4th sphere on 1st column, pearl *****
	// glViewport(0.0f, (GLsizei)(WINHEIGHT) - ((WINHEIGHT) / 1.3f), (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	// glViewport(0.0f, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(0.0f, verDistance * 2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.25;	  // r
	materialAmbiant[1] = 0.20725; // g
	materialAmbiant[2] = 0.20725; // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 1.0;	// r
	materialDiffuse[1] = 0.829; // g
	materialDiffuse[2] = 0.829; // b
	materialDiffuse[3] = 1.0f;	// a

	// specular material
	materialSpecular[0] = 0.296648; // r
	materialSpecular[1] = 0.296648; // g
	materialSpecular[2] = 0.296648; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.088 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 5
	// ***** 5th sphere on 1st column, ruby *****
	glViewport(0.0f, verDistance, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.1745;  // r
	materialAmbiant[1] = 0.01175; // g
	materialAmbiant[2] = 0.01175; // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.61424; // r
	materialDiffuse[1] = 0.04136; // g
	materialDiffuse[2] = 0.04136; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.727811; // r
	materialSpecular[1] = 0.626959; // g
	materialSpecular[2] = 0.626959; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.6 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 6
	// ***** 6th sphere on 1st column, turquoise *****
	//	glViewport(0.0f, 0.0f, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(0.0f, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.1;	  // r
	materialAmbiant[1] = 0.18725; // g
	materialAmbiant[2] = 0.1745;  // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.396;	  // r
	materialDiffuse[1] = 0.74151; // g
	materialDiffuse[2] = 0.69102; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.297254; // r
	materialSpecular[1] = 0.30829;	// g
	materialSpecular[2] = 0.306678; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.1 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 2nd column, brass *****
	// ambient material
	// 6
	// glViewport(425.0f, 120 * 5.5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, verDistance * 5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	materialAmbiant[0] = 0.329412; // r
	materialAmbiant[1] = 0.223529; // g
	materialAmbiant[2] = 0.027451; // b
	materialAmbiant[3] = 1.0f;	   // a

	// diffuse material
	materialDiffuse[0] = 0.780392; // r
	materialDiffuse[1] = 0.568627; // g
	materialDiffuse[2] = 0.113725; // b
	materialDiffuse[3] = 1.0f;	   // a

	// specular material
	materialSpecular[0] = 0.992157; // r
	materialSpecular[1] = 0.941176; // g
	materialSpecular[2] = 0.807843; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.21794872 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 7
	// ***** 2nd sphere on 2nd column, bronze *****
	// glViewport(425.0f, 120 * 4.4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	glViewport(horDistance * 1.5, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.2125; // r
	materialAmbiant[1] = 0.1275; // g
	materialAmbiant[2] = 0.054;	 // b
	materialAmbiant[3] = 1.0f;	 // a

	// diffuse material
	materialDiffuse[0] = 0.714;	  // r
	materialDiffuse[1] = 0.4284;  // g
	materialDiffuse[2] = 0.18144; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.393548; // r
	materialSpecular[1] = 0.271906; // g
	materialSpecular[2] = 0.166721; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.2 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 8
	// ***** 3rd sphere on 2nd column, chrome *****
	// glViewport(425.0f, 120 * 3.3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.25; // r
	materialAmbiant[1] = 0.25; // g
	materialAmbiant[2] = 0.25; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.774597; // r
	materialSpecular[1] = 0.774597; // g
	materialSpecular[2] = 0.774597; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.6 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 9
	// ***** 4th sphere on 2nd column, copper *****
	// glViewport(425.0f, 120 * 2.2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, verDistance * 2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.19125; // r
	materialAmbiant[1] = 0.0735;  // g
	materialAmbiant[2] = 0.0225;  // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.7038;  // r
	materialDiffuse[1] = 0.27048; // g
	materialDiffuse[2] = 0.0828;  // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.256777; // r
	materialSpecular[1] = 0.137622; // g
	materialSpecular[2] = 0.086014; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.1 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 10
	// ***** 5th sphere on 2nd column, gold *****
	//	glViewport(425.0f, 120 * 1.1, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, verDistance, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.24725; // r
	materialAmbiant[1] = 0.1995;  // g
	materialAmbiant[2] = 0.0745;  // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.75164; // r
	materialDiffuse[1] = 0.60648; // g
	materialDiffuse[2] = 0.22648; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.628281; // r
	materialSpecular[1] = 0.555802; // g
	materialSpecular[2] = 0.366065; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.4 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 11
	// ***** 6th sphere on 2nd column, silver *****
	// glViewport(425.0f, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.19225; // r
	materialAmbiant[1] = 0.19225; // g
	materialAmbiant[2] = 0.19225; // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.50754; // r
	materialDiffuse[1] = 0.50754; // g
	materialDiffuse[2] = 0.50754; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.508273; // r
	materialSpecular[1] = 0.508273; // g
	materialSpecular[2] = 0.508273; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.4 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 12
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 3rd column, black *****
	// glViewport(850.0f, 120 * 5.5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 3, verDistance * 5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.50; // r
	materialSpecular[1] = 0.50; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// ***** 2nd sphere on 3rd column, cyan *****
	// 13
	glViewport(850.0f, 120 * 4.4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 3, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.1;  // g
	materialAmbiant[2] = 0.06; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.0;		 // r
	materialDiffuse[1] = 0.50980392; // g
	materialDiffuse[2] = 0.50980392; // b
	materialDiffuse[3] = 1.0f;		 // a

	// specular material
	materialSpecular[0] = 0.50185078; // r
	materialSpecular[1] = 0.50185078; // g
	materialSpecular[2] = 0.50185078; // b
	materialSpecular[3] = 1.0f;		  // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 14
	// ***** 3rd sphere on 2nd column, green *****
	glViewport(horDistance * 3, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.1;  // r
	materialDiffuse[1] = 0.35; // g
	materialDiffuse[2] = 0.1;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.45; // r
	materialSpecular[1] = 0.55; // g
	materialSpecular[2] = 0.45; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 15
	// ***** 4th sphere on 3rd column, red *****
	// glViewport(850.0f, 120 * 2.2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 3, verDistance * 2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.0;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.6;	// g
	materialSpecular[2] = 0.6;	// b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 16
	// ***** 5th sphere on 3rd column, white *****
	// glViewport(850.0f, 120 * 1.1, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 3, verDistance, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.55; // r
	materialDiffuse[1] = 0.55; // g
	materialDiffuse[2] = 0.55; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.70; // r
	materialSpecular[1] = 0.70; // g
	materialSpecular[2] = 0.70; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 17
	// ***** 6th sphere on 3rd column, yellow plastic *****
	glViewport(horDistance * 3, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.60; // r
	materialSpecular[1] = 0.60; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 18
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 4th column, black *****
	// ambient material
	glViewport(horDistance * 4.5, verDistance * 5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	materialAmbiant[0] = 0.02; // r
	materialAmbiant[1] = 0.02; // g
	materialAmbiant[2] = 0.02; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.4;	// r
	materialSpecular[1] = 0.4;	// g
	materialSpecular[2] = 0.4;	// b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 19
	// ***** 2nd sphere on 4th column, cyan *****
	glViewport(horDistance * 4.5, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.05; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.7;	// b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 20
	// ***** 3rd sphere on 4th column, green *****
	glViewport(horDistance * 4.5, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 21
	// ***** 4th sphere on 4th column, red *****
	glViewport(horDistance * 4.5, verDistance * 2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.05; // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.04; // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 22
	// ***** 5th sphere on 4th column, white *****
	glViewport(horDistance * 4.5, verDistance, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.05; // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.05; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.7;	// b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// 23
	// ***** 6th sphere on 4th column, yellow rubber *****
	glViewport(horDistance * 4.5, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	// ambient material
	materialAmbiant[0] = 0.05; // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);
}



- (void) uninitialise
{
    // CODE
    [sphere uninitialize];

    // delete vbo_Square_Position
	
	// deletion of gVbo_sphere_position
	if (gVbo_sphere_position)
	{
		glDeleteBuffers(1, &gVbo_sphere_position);
		gVbo_sphere_position = 0;
	}

	if (gVbo_sphere_normal)
	{
		glDeleteBuffers(1, &gVbo_sphere_position);
		gVbo_sphere_position = 0;
	}

	// deletion of gVao_sphere
	if (gVao_sphere)
	{
		glDeleteVertexArrays(1, &gVao_sphere);
		gVao_sphere = 0;
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

		case 'L':
		case 'l':
			if (bLight == FALSE)
			{
				bLight = TRUE;
			}
			else
			{
				bLight = FALSE;
			}
			break;

		
		case 'x':
		case 'X':
			keyPressed = 1;
			angleForRotation = 0.0f; // reset
			lightPositions[0] = 0;
			lightPositions[1] = 0;
			lightPositions[2] = 0;
			break;

		case 'y':
		case 'Y':
			keyPressed = 2;
			angleForRotation = 0.0f; // reset
			lightPositions[0] = 0;
			lightPositions[1] = 0;
			lightPositions[2] = 0;
			break;

		case 'z':
		case 'Z':
			keyPressed = 3;
			angleForRotation = 0.0f; // reset
			lightPositions[0] = 0;
			lightPositions[1] = 0;
			lightPositions[2] = 0;
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

