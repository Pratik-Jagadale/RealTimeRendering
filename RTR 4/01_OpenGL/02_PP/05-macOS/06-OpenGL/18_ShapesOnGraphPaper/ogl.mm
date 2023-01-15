#import <Foundation/Foundation.h>

#import <cocoa/cocoa.h>

#import <QuartzCore/CVDisplayLink.h>

#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>

#include "vmath.h"
using namespace vmath;

#include "Sphere.h"
#include "Pratik3DLibrary.h"

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
	
	GLuint mvpMatrixUniform; // model View Projection
	mat4 perspectiveProjectionMatrix;

	GLuint vao_Square;			// Vertex Array Object - Square
	GLuint vbo_Square_Position; // Vertex Buffer Object - Square- Position

	GLuint vao_Triangle;
	GLuint vbo_Triangle_Position;

	GLuint vao_Circle;
	GLuint vbo_Circle_Position;

	GLuint vao_Line;
	GLuint vbo_Line_Position;

	GLuint uniform_Color;

	BOOL bGraph ;
	BOOL bCircle ;
	BOOL bTriangle ;
	BOOL bSquare ;

	// GLfloat *CircleVertexdata = NULL;
	GLfloat CircleVertexdata[1080];
	GLuint numberOfVerticesOfCircle ;

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
		"uniform mat4 u_mvpMatrix;"
		"void main(void)"
		"{"
		"gl_Position = u_mvpMatrix * a_position;"
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
		"uniform vec3 u_Color;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"FragColor = vec4(u_Color, 1);"
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
	// Binding Position Array
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_POSITION, "a_position");

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
	uniform_Color = glGetUniformLocation(shaderProgramObject, "u_Color");

	// vao_Triangle and vba related code
	// declartions of vertex Data array
	const GLfloat SquarePosition[] = {
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f};

	const GLfloat trianglePosition[] = {
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f};

	const GLfloat LinePosition[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f};

	numberOfVerticesOfCircle = getNumberOfCircleVertices();
	getCircleVertexData(CircleVertexdata);

	// vao and vbo related code
	// vao for Square
	glGenVertexArrays(1, &vao_Square);
	glBindVertexArray(vao_Square);

	// vbo for position
	glGenBuffers(1, &vbo_Square_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Square_Position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(SquarePosition), SquarePosition, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // ubind vao for Square

	// vao for Circle
	glGenVertexArrays(1, &vao_Circle);
	glBindVertexArray(vao_Circle);

	// vbo for position
	glGenBuffers(1, &vbo_Circle_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Circle_Position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CircleVertexdata), CircleVertexdata, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // ubind vao for Square

	// vao for Line
	glGenVertexArrays(1, &vao_Line);
	glBindVertexArray(vao_Line);

	// vbo for position
	glGenBuffers(1, &vbo_Line_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Line_Position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(LinePosition), LinePosition, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// vao for Triangle
	glGenVertexArrays(1, &vao_Triangle);
	glBindVertexArray(vao_Triangle);

	// vbo for position
	glGenBuffers(1, &vbo_Triangle_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Triangle_Position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePosition), trianglePosition, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    perspectiveProjectionMatrix = mat4::identity();

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

- (void)drawGraph
{
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	// Verticle Lines
	translationMatrix = vmath::translate(0.0f, 0.0f, -4.1f);
	scaleMatrix = vmath::scale(1.0f, 1.5f, 0.0f);

	for (float i = -1.5; i < 1.5; i = i + 0.05f)
	{
		translationMatrix = vmath::translate(i, 0.0f, -4.0f);
		modelViewMatrix = translationMatrix * scaleMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		GLfloat ColorArray[3] = {0.0f, 0.0f, 1.0f};
		if (i > -0.02 && i < 0.02)
		{
			ColorArray[1] = 1.0f;
			ColorArray[2] = 0.0f;
		}

		glBindVertexArray(vao_Line);

		glUniform3fv(uniform_Color, 1, ColorArray);

		glDrawArrays(GL_LINES, 0, 2);

		glBindVertexArray(0);
	}

	// Horizontal Lines
	scaleMatrix = vmath::scale(1.0f, 1.5f, 1.0f);
	rotationMatrix = vmath::rotate(90.0f, 0.0f, 0.0f, 1.0f);

	for (float i = -1.5; i < 1.5; i = i + 0.05f)
	{
		translationMatrix = vmath::translate(0.0f, i, -4.0f);
		modelViewMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		GLfloat ColorArray[3] = {0.0f, 0.0f, 1.0f};
		if (i > -0.02 && i < 0.02)
		{
			ColorArray[0] = 1.0f;
			ColorArray[2] = 0.0f;
		}

		glBindVertexArray(vao_Line);

		glUniform3fv(uniform_Color, 1, ColorArray);

		glDrawArrays(GL_LINES, 0, 2);

		glBindVertexArray(0);
	}
}

- (void)drawTriangle
{
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);

	modelViewMatrix = translationMatrix;

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	GLfloat ColorArray[] = {1.0f, 1.0f, 0.0f};

	glBindVertexArray(vao_Triangle);

	glUniform3fv(uniform_Color, 1, ColorArray);

	glDrawArrays(GL_LINE_LOOP, 0, 3);

	glBindVertexArray(0);
}

- (void)drawCircle
{
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);

	modelViewMatrix = translationMatrix;

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	GLfloat ColorArray[] = {1.0f, 1.0f, 0.0f};

	glBindVertexArray(vao_Circle);

	glUniform3fv(uniform_Color, 1, ColorArray);

	glDrawArrays(GL_LINE_STRIP, 0, numberOfVerticesOfCircle);

	glBindVertexArray(0);
}

- (void)drawsquare
{
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); // glTranslatef() is replaced by this line

	modelViewMatrix = translationMatrix;

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	GLfloat ColorArray[] = {1.0f, 1.0f, 0.0f};

	glBindVertexArray(vao_Square);

	glUniform3fv(uniform_Color, 1, ColorArray);

	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glBindVertexArray(0);
}

- (void) display
{
    // CODE
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	[self drawGraph];

	[self drawsquare];

	[self drawCircle];

	[self drawTriangle];

	glUseProgram(0);
}

- (void) myupdate
{
    // CODE
 
}

- (void) uninitialise
{
    // CODE
    
    // delete vbo_Line_Position
	if (vbo_Line_Position)
	{
		glDeleteBuffers(1, &vbo_Line_Position);
		vbo_Line_Position = 0;
	}

	// deletion of vao_Line
	if (vao_Line)
	{
		glDeleteVertexArrays(1, &vao_Line);
		vao_Line = 0;
	}

	// delete vbo_Triangle_Position
	if (vbo_Triangle_Position)
	{
		glDeleteBuffers(1, &vbo_Triangle_Position);
		vbo_Triangle_Position = 0;
	}

	// deletion of vao_Triangle
	if (vao_Triangle)
	{
		glDeleteVertexArrays(1, &vao_Triangle);
		vao_Triangle = 0;
	}

	// delete vbo_Circle_Position
	if (vbo_Circle_Position)
	{
		glDeleteBuffers(1, &vbo_Circle_Position);
		vbo_Circle_Position = 0;
	}

	// deletion of vao_Square
	if (vao_Circle)
	{
		glDeleteVertexArrays(1, &vao_Circle);
		vao_Circle = 0;
	}

	// delete vbo_Square_Position
	if (vbo_Square_Position)
	{
		glDeleteBuffers(1, &vbo_Square_Position);
		vbo_Square_Position = 0;
	}

	// deletion of vao_Square
	if (vao_Square)
	{
		glDeleteVertexArrays(1, &vao_Square);
		vao_Square = 0;
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

