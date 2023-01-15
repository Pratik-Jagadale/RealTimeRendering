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

    
	// PP Related Global Variables
	GLuint shaderProgramObject_PF;
	GLuint shaderProgramObject_PV;

	GLuint gVao_sphere;			 // Vertex Array Object
	GLuint gVbo_sphere_position; // Vertex Buffer Object
	GLuint gVbo_sphere_normal;
	GLuint gVbo_sphere_element;

	GLuint modelMatrixUniform_PF;
	GLuint viewMatrixUniform_PF;
	GLuint projectionMatrixUniform_PF;

	GLuint modelMatrixUniform_PV;
	GLuint viewMatrixUniform_PV;
	GLuint projectionMatrixUniform_PV;

	mat4 perspectiveProjectionMatrix;

	// For Three Light on Sphere
	GLuint laUniform_PF[3];			  // light Ambiant
	GLuint ldUniform_PF[3];			  // light Diffuse
	GLuint lsUniform_PF[3];			  // light Spec
	GLuint lighPositionUniform_PF[3]; // light Position

	GLuint kaUniform_PF; // material amb
	GLuint kdUniform_PF; // mat diff
	GLuint ksUniform_PF; // mat specular
	GLuint materialShininessUniform_PF;

	GLuint lightingEnabledUniform_PF;

	// For Three Light on Sphere
	GLuint laUniform_PV[3];			  // light Ambiant
	GLuint ldUniform_PV[3];			  // light Diffuse
	GLuint lsUniform_PV[3];			  // light Spec
	GLuint lighPositionUniform_PV[3]; // light Position

	GLuint kaUniform_PV; // material amb
	GLuint kdUniform_PV; // mat diff
	GLuint ksUniform_PV; // mat specular
	GLuint materialShininessUniform_PV;

	GLuint lightingEnabledUniform_PV;

	BOOL bLight;

	struct Light
	{
		vmath::vec4 lightAmbiant;
		vmath::vec4 lightDiffuse;
		vmath::vec4 lightSpecular;
		vmath::vec4 lightPositions;
	};

	struct Light lights[3];

	vmath::vec4 materialAmbiant;
	vmath::vec4 meterialDiffuse ;
	vmath::vec4 materialSpecular;
	GLfloat materialShineeness;

	GLfloat lightAngleOne ;
	GLfloat lightAngleTwo ;
	GLfloat lightAngleZero;

	BOOL isPerFrag ;

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
    
	{

		// vartex Shader
		const GLchar *vertexShaderSourceCode_PF =
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

		GLuint vertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShaderObject_PF, 1, (const GLchar **)&vertexShaderSourceCode_PF, NULL);

		glCompileShader(vertexShaderObject_PF);

		GLint status;
		GLint infoLogLength;
		char *log = NULL;
		glGetShaderiv(vertexShaderObject_PF, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetShaderiv(vertexShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(vertexShaderObject_PF, infoLogLength, &written, log);
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

		const GLchar *fragmentShaderSourceCode_PF =
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

		GLuint fragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShaderObject_PF, 1, (const GLchar **)&fragmentShaderSourceCode_PF, NULL);

		glCompileShader(fragmentShaderObject_PF);

		glGetShaderiv(fragmentShaderObject_PF, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(fragmentShaderObject_PF, infoLogLength, &written, log);
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
		shaderProgramObject_PF = glCreateProgram();
		glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);

		glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

		// prelinked binding
		glBindAttribLocation(shaderProgramObject_PF, PRJ_ATRIBUTE_POSITION, "a_position");
		glBindAttribLocation(shaderProgramObject_PF, PRJ_ATRIBUTE_NORMAL, "a_normal");

		// link
		glLinkProgram(shaderProgramObject_PF);

		status = 0;
		infoLogLength = 0;

		glGetProgramiv(shaderProgramObject_PF, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetProgramiv(shaderProgramObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetProgramInfoLog(shaderProgramObject_PF, infoLogLength, &written, log);
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
		modelMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_modelMatrix");
		viewMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_viewMatrix");
		projectionMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_projectionMatrix");

		laUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "u_la[0]");
		ldUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "u_ld[0]");
		lsUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "u_ls[0]");
		lighPositionUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "u_lightPosition[0]");

		laUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "u_la[1]");
		ldUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "u_ld[1]");
		lsUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "u_ls[1]");
		lighPositionUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "u_lightPosition[1]");

		laUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "u_la[2]");
		ldUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "u_ld[2]");
		lsUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "u_ls[2]");
		lighPositionUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "u_lightPosition[2]");

		kaUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ka");
		kdUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_kd");
		ksUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ks");
		materialShininessUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_materialShininnes");

		lightingEnabledUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_lightingEnabled");
	}

	{
		// vartex Shader
		const GLchar *vertexShaderSourceCode_PV =
			"#version 410 core"
			"\n"
			"in vec4 a_position;"
			"in vec3 a_normal;"
			"uniform mat4 u_modelMatrix;"
			"uniform mat4 u_viewMatrix;"
			"uniform mat4 u_projectionMatrix;"
			"uniform vec3 u_la[3];"
			"uniform vec3 u_ld[3];"
			"uniform vec3 u_ls[3];"
			"uniform vec4 u_lightPosition[3];"
			"uniform vec3 u_ka;"
			"uniform vec3 u_ks;"
			"uniform vec3 u_kd;"
			"uniform float u_materialShininnes;"
			"uniform int u_lightingEnabled;"
			"out vec3 phong_ads_light;"
			"void main(void)"
			"{"
			"phong_ads_light = vec3(0.0,0.0,0.0);"
			"if(u_lightingEnabled == 1)"
			"{"
			"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;"
			"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);"
			"vec3 transformedNormals = normalize(normalMatrix * a_normal);"
			"vec3 viewerVector = normalize(-eyeCoordinates.xyz);"
			"vec3 ambiant[3];"
			"vec3 lightDirection[3];"
			"vec3 diffuse[3];"
			"vec3 reflectionVector[3];"
			"vec3 specular[3];"
			"for(int i = 0 ; i < 3 ; i++)"
			"{"
			"ambiant[i] = u_la[i] * u_ka;"
			"lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);" // Swizaling
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
			"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;"
			"}";

		GLuint vertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShaderObject_PV, 1, (const GLchar **)&vertexShaderSourceCode_PV, NULL);

		glCompileShader(vertexShaderObject_PV);

		GLint status;
		GLint infoLogLength;
		char *log = NULL;
		glGetShaderiv(vertexShaderObject_PV, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetShaderiv(vertexShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(vertexShaderObject_PV, infoLogLength, &written, log);
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

		const GLchar *fragmentShaderSourceCode_PV =
			"#version 410 core"
			"\n"
			"in vec3 phong_ads_light;"
			"out vec4 FragColor;"
			"void main(void)"
			"{"
			"FragColor = vec4(phong_ads_light, 1.0);"
			"}";

		GLuint fragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShaderObject_PV, 1, (const GLchar **)&fragmentShaderSourceCode_PV, NULL);

		glCompileShader(fragmentShaderObject_PV);

		glGetShaderiv(fragmentShaderObject_PV, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(fragmentShaderObject_PV, infoLogLength, &written, log);
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
		shaderProgramObject_PV = glCreateProgram();
		glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);

		glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

		// prelinked binding
		glBindAttribLocation(shaderProgramObject_PV, PRJ_ATRIBUTE_POSITION, "a_position");
		glBindAttribLocation(shaderProgramObject_PV, PRJ_ATRIBUTE_NORMAL, "a_normal");

		// link
		glLinkProgram(shaderProgramObject_PV);

		status = 0;
		infoLogLength = 0;

		glGetProgramiv(shaderProgramObject_PV, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetProgramiv(shaderProgramObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetProgramInfoLog(shaderProgramObject_PV, infoLogLength, &written, log);
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
		modelMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_modelMatrix");
		viewMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_viewMatrix");
		projectionMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_projectionMatrix");

		laUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "u_la[0]");
		ldUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "u_ld[0]");
		lsUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "u_ls[0]");
		lighPositionUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "u_lightPosition[0]");

		laUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "u_la[1]");
		ldUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "u_ld[1]");
		lsUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "u_ls[1]");
		lighPositionUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "u_lightPosition[1]");

		laUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "u_la[2]");
		ldUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "u_ld[2]");
		lsUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "u_ls[2]");
		lighPositionUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "u_lightPosition[2]");

		kaUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ka");
		kdUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_kd");
		ksUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ks");
		materialShininessUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_materialShininnes");

		lightingEnabledUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_lightingEnabled");
	}

	sphere = [[Sphere alloc]init];
    [sphere initialize:1.0 :100 :100];
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    perspectiveProjectionMatrix = mat4::identity();

	lights[0].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightDiffuse = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightSpecular = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights[1].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[1].lightDiffuse = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[1].lightSpecular = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[1].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights[2].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[2].lightDiffuse = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights[2].lightSpecular = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights[2].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	materialAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	meterialDiffuse = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialSpecular = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialShineeness = 124.0f;

	lightAngleOne = 0.0f;
	lightAngleTwo = 150.0f;
	lightAngleZero = 300.0f;

	BOOL isPerFrag = FALSE;

    return 0;
}

- (void) resize:(int)width :(int)height
{
    // CODE
    if(height < 0){
        height = 1;
    }
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

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
	if (isPerFrag)
		glUseProgram(shaderProgramObject_PF);
	else
		glUseProgram(shaderProgramObject_PV);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); // glTranslatef() is replaced by this line

	modelMatrix = translationMatrix;

	// Light Related Code
	// Set Light Zero Position - rotating zero light - x Around
	float angle = lightAngleZero * (M_PI / 180.0f);
	float x = 5.0f * cos(angle);
	float y = 5.0f * sin(angle);
	lights[0].lightPositions[1] = x;
	lights[0].lightPositions[2] = y;

	// Set Light One Position  rotating One Light - Y Rotation
	angle = (lightAngleOne * M_PI) / 180.0f;
	x = 5.0f * cos(angle);
	y = 5.0f * sin(angle);
	lights[1].lightPositions[0] = x;
	lights[1].lightPositions[2] = y;

	// Set Light Two Position rotating Two Light Z Rotation
	angle = (lightAngleTwo * M_PI) / 180.0f;
	x = 5.0f * cos(angle);
	y = 5.0f * sin(angle);
	lights[2].lightPositions[0] = x;
	lights[2].lightPositions[1] = y;
	if (isPerFrag == TRUE)
	{
		glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, perspectiveProjectionMatrix);

		// draw the desired graphics
		// drawing code -- magic

		if (bLight == TRUE)
		{
			glUniform1i(lightingEnabledUniform_PF, 1);

			for (int i = 0; i < 3; i++)
			{
				glUniform3fv(laUniform_PF[i], 1, lights[i].lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform_PF
				glUniform3fv(ldUniform_PF[i], 1, lights[i].lightDiffuse);
				glUniform3fv(lsUniform_PF[i], 1, lights[i].lightSpecular);
				glUniform4fv(lighPositionUniform_PF[i], 1, lights[i].lightPositions);
			}

			glUniform3fv(kaUniform_PF, 1, materialAmbiant);
			glUniform3fv(kdUniform_PF, 1, meterialDiffuse);
			glUniform3fv(ksUniform_PF, 1, materialSpecular);
			glUniform1f(materialShininessUniform_PF, materialShineeness);
		}
		else
		{
			glUniform1i(lightingEnabledUniform_PF, 0);
		}
	}
	else
	{
		glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, perspectiveProjectionMatrix);

		if (bLight == TRUE)
		{
			glUniform1i(lightingEnabledUniform_PV, 1);

			for (int i = 0; i < 3; i++)
			{
				glUniform3fv(laUniform_PV[i], 1, lights[i].lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
				glUniform3fv(ldUniform_PV[i], 1, lights[i].lightDiffuse);
				glUniform3fv(lsUniform_PV[i], 1, lights[i].lightSpecular);
				glUniform4fv(lighPositionUniform_PV[i], 1, lights[i].lightPositions);
			}

			glUniform3fv(kaUniform_PV, 1, materialAmbiant);
			glUniform3fv(kdUniform_PV, 1, meterialDiffuse);
			glUniform3fv(ksUniform_PV, 1, materialSpecular);
			glUniform1f(materialShininessUniform_PV, materialShineeness);
		}
		else
		{
			glUniform1i(lightingEnabledUniform_PV, 0);
		}
	}

	glBindVertexArray(gVao_sphere);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, [sphere getNumberOfSphereVertices]);
	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);
}

- (void) myupdate
{
    // CODE
	lightAngleOne = lightAngleOne + 1.0f;
	if(lightAngleOne > 360.0f)
		lightAngleOne = 0.0f;

	lightAngleZero = lightAngleZero + 1.0f;
	if(lightAngleZero > 360.0f)
		lightAngleZero = 0.0f;

	lightAngleTwo = lightAngleTwo + 1.0f;
	if(lightAngleTwo > 360.0f)
		lightAngleTwo = 0.0f;

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

	if (shaderProgramObject_PF)
	{
		glUseProgram(shaderProgramObject_PF);

		GLsizei numAttachedShaders;

		glGetProgramiv(shaderProgramObject_PF, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint *shaderObject = NULL;
		shaderObject = (GLuint *)malloc(numAttachedShaders * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject_PF, numAttachedShaders, &numAttachedShaders, shaderObject);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject_PF, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject_PF);
		shaderProgramObject_PF = 0;
	}

	if (shaderProgramObject_PV)
	{
		glUseProgram(shaderProgramObject_PV);

		GLsizei numAttachedShaders;

		glGetProgramiv(shaderProgramObject_PV, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint *shaderObject = NULL;
		shaderObject = (GLuint *)malloc(numAttachedShaders * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject_PV, numAttachedShaders, &numAttachedShaders, shaderObject);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject_PV, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject_PV);
		shaderProgramObject_PV = 0;
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

