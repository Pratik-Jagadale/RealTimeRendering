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
   
	GLuint vao_Cube;		  // Vertex Array Object - Cube
	GLuint vbo_Cube_Position; // Vertex Buffer Object - Cube- Position
	GLuint vbo_Cube_TexCoord; // Vertex Buffer Object - Cube - Color
	GLuint vbo_Cube_Normal;
	GLuint vbo_Cube_Color; // Vertex Buffer Object - Cube - Color

	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLuint projectionMatrixUniform;
	GLuint textureSamplerUniform;

	GLuint texture;

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
	vmath::vec4 lightPositions;

	vmath::vec4 materialAmbiant ;
	vmath::vec4 meterialDeffuse;
	vmath::vec4 materialSpecular ;
	GLfloat materialShineeness;

	GLfloat angleCube;
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
		"in vec4 a_color;"
		"in vec3 a_normal;"
		"in vec2 a_texcoord;"
		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"uniform vec4 u_lightPosition;"
		"uniform int u_lightingEnabled;"
		"out vec3 transformedNormals;"
		"out vec3 lightDirection;"
		"out vec3 viewerVector;"
		"out vec4 a_color_out;"
		"out vec2 a_texcoord_out;"
		"void main(void)"
		"{"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;"
		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);"
		"transformedNormals = normalMatrix * a_normal;"
		"lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;" // Swizaling
		"viewerVector = -eyeCoordinates.xyz;"
		"}"
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;"
		"a_color_out = a_color;"
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
		"in vec3 transformedNormals;"
		"in vec3 lightDirection;"
		"in vec3 viewerVector;"
		"in vec4 a_color_out;"
		"in vec2 a_texcoord_out;"
		"uniform vec3 u_la;"
		"uniform vec3 u_ld;"
		"uniform vec3 u_ls;"
		"uniform vec3 u_ka;"
		"uniform vec3 u_ks;"
		"uniform vec3 u_kd;"
		"uniform float u_materialShininnes;"
		"uniform int u_lightingEnabled;"
		"uniform sampler2D u_textureSampler;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"vec3 phong_ads_color;"
		"vec3 tex;"
		"tex = texture(u_textureSampler, a_texcoord_out).xyz;"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec3 ambiant = u_la * u_ka;"
		"vec3 normalized_transformed_normals = normalize(transformedNormals);"
		"vec3 normalized_light_direction = normalize(lightDirection);"
		"vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction ,normalized_transformed_normals),0.0);"
		"vec3 reflectionVector = reflect(-normalized_light_direction,normalized_transformed_normals);"
		"vec3 normalized_view_vector = normalize(viewerVector);"
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector , normalized_view_vector), 0.0), u_materialShininnes);"
		"phong_ads_color =  ambiant + diffuse + specular;"
		"}"
		"else"
		"{"
		"phong_ads_color = vec3(1.0 , 1.0 , 1.0);"
		"}"
		"FragColor = vec4(tex * phong_ads_color * vec3(a_color_out), 1.0);"
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
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_COLOR, "a_color");
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_TEXTURE0, "a_texcoord");
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

	GLfloat cubeNormals[] = {
		// top surface
		0.0f, 1.0f, 0.0f, // top-right of top
		0.0f, 1.0f, 0.0f, // top-left of top
		0.0f, 1.0f, 0.0f, // bottom-left of top
		0.0f, 1.0f, 0.0f, // bottom-right of top

		// bottom surface
		0.0f, -1.0f, 0.0f, // top-right of bottom
		0.0f, -1.0f, 0.0f, // top-left of bottom
		0.0f, -1.0f, 0.0f, // bottom-left of bottom
		0.0f, -1.0f, 0.0f, // bottom-right of bottom

		// front surface
		0.0f, 0.0f, 1.0f, // top-right of front
		0.0f, 0.0f, 1.0f, // top-left of front
		0.0f, 0.0f, 1.0f, // bottom-left of front
		0.0f, 0.0f, 1.0f, // bottom-right of front

		// back surface
		0.0f, 0.0f, -1.0f, // top-right of back
		0.0f, 0.0f, -1.0f, // top-left of back
		0.0f, 0.0f, -1.0f, // bottom-left of back
		0.0f, 0.0f, -1.0f, // bottom-right of back

		// right surface
		1.0f, 0.0f, 0.0f, // top-right of right
		1.0f, 0.0f, 0.0f, // top-left of right
		1.0f, 0.0f, 0.0f, // bottom-left of right
		1.0f, 0.0f, 0.0f, // bottom-right of right

		// left surface
		-1.0f, 0.0f, 0.0f, // top-right of left
		-1.0f, 0.0f, 0.0f, // top-left of left
		-1.0f, 0.0f, 0.0f, // bottom-left of left
		-1.0f, 0.0f, 0.0f  // bottom-right of left

	};

	const GLfloat CubeColor[] =
		{
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			1.0f, 0.5f, 0.0f,
			1.0f, 0.5f, 0.0f,
			1.0f, 0.5f, 0.0f,
			1.0f, 0.5f, 0.0f,

			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f

		};

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

	// vbo for color
	glGenBuffers(1, &vbo_Cube_Color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_Color);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeColor), CubeColor, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Texcoord
	glGenBuffers(1, &vbo_Cube_TexCoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_TexCoord);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeTexCoord), CubeTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Normal
	glGenBuffers(1, &vbo_Cube_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_Normal);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // ubind vao for Cube

	// Depth Related Changes
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
	texture = [self loadGLTexture:"marble.bmp"];
    if(texture == 0)
    {
		fprintf(gpFile, "loadGLTexture Failed...\n");
        [self uninitialise];
        [self release];
        [NSApp terminate:self];           
    }

    perspectiveProjectionMatrix = mat4::identity();

    return 0;

	bLight = FALSE;

	lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lightDiffuse = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightSpecular = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightPositions = vmath::vec4(0.0f, 0.0f, 5.0f, 1.0f);

	materialAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	meterialDeffuse = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialSpecular = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	GLfloat materialShineeness = 120.0f;

	angleCube = 0.0f;
}

- (GLuint) loadGLTexture:(const char*)textureFileName
{
    // CODE
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appDirPath = [appBundle bundlePath];
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *textureFileNameWithPath = [NSString stringWithFormat:@"%@/%s",parentDirPath,textureFileName];

    if(textureFileNameWithPath == nil)
    {
        fprintf(gpFile, "Failed to create textureFile Path.\n");
        return 0;
    }

    // Get NSImage Representation of texture file
    NSImage *nsImage = [[NSImage alloc] initWithContentsOfFile:textureFileNameWithPath];
    
	if(nsImage == nil)
    {
        fprintf(gpFile, "NSImage Failed.\n");
        return 0;
    }

    // From NSImageRepresentation obtain get CGImage Represenation
    CGImageRef cgImageRef = [nsImage CGImageForProposedRect:nil context:nil hints:nil];

    // From this CGImageRepresenation get width and height of Image
    int width = (int)CGImageGetWidth(cgImageRef);
    int height = (int)CGImageGetHeight(cgImageRef);

    // From this CGImage Representation get CGDataProvider 
    CGDataProviderRef cgdataProviderRef = CGImageGetDataProvider(cgImageRef);

    // Using CGDataProvider get Imagedata in the form of CFData (core foundation)
    CFDataRef imageData = CGDataProviderCopyData(cgdataProviderRef);

    // Convert This CFData Formated Image data into void*
    void* pixel = (void*)CFDataGetBytePtr(imageData);

    // Procceed with usual texture creation code
    GLuint texture = 0;

    glGenTextures(1 , &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    // glPixelStorei(GL_UNPACK_ALLIGNMENT,1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // create the texture
		glTexImage2D(GL_TEXTURE_2D,	   // Targter
					 0,				   // MipMap Level (done by opengl)
					 GL_RGBA,		   // Opengl Image format
					 width,	   // Image Width
					 height,	   // Image Height
					 0,				   // Border Width
					 GL_RGBA,		   // Image Format
					 GL_UNSIGNED_BYTE, // Data type of bmp.bmBits
					 pixel);	   //

    glGenerateMipmap(GL_TEXTURE_2D); // Generate MipMap

    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

    CFRelease(imageData);

	return (texture);
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
	glUseProgram(shaderProgramObject);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 rotationMatrix_X = mat4::identity();
	mat4 rotationMatrix_Y = mat4::identity();
	mat4 rotationMatrix_Z = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f); // glTranslatef() is replaced by this line
	rotationMatrix_X = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	rotationMatrix_Y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	rotationMatrix_Z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
	rotationMatrix = rotationMatrix_X * rotationMatrix_Y * rotationMatrix_Z;

	modelMatrix = translationMatrix * rotationMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, meterialDeffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShineeness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}
	glActiveTexture(GL_TEXTURE0); //
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureSamplerUniform, 0); //

	glBindVertexArray(vao_Cube);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// unuse the shader program object
	glUseProgram(0);
}

- (void) myupdate
{
    // CODE
   	angleCube = angleCube + 1.0f;
	if (angleCube >= 360.0f)
		angleCube = angleCube - 360.0f;
}

- (void) uninitialise
{
    if (texture)
	{
		glDeleteTextures(1, &texture);
		texture = 0;
	}

	if (vbo_Cube_Normal)
	{
		glDeleteBuffers(1, &vbo_Cube_Normal);
		vbo_Cube_Normal = 0;
	}

	if (vbo_Cube_TexCoord)
	{
		glDeleteBuffers(1, &vbo_Cube_TexCoord);
		vbo_Cube_TexCoord = 0;
	}

	if (vbo_Cube_Color)
	{
		glDeleteBuffers(1, &vbo_Cube_Color);
		vbo_Cube_Color = 0;
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

