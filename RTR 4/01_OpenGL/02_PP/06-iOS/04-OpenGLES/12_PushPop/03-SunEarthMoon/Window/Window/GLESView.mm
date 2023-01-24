//
//  GLESView.mm
//  Window
//
//  Created by user226085 on 1/25/23.
//

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#import "GLESView.h"

#import "vmath.h"
#include "sphere.h"
#include "Matrix.h"

using namespace vmath;

@implementation GLESView
{
    @private
    EAGLContext* eaglContext;
    
    GLuint defaultFrameBuffer;
    GLuint colorRenderBuffer;
    GLuint depthRenderBuffer;
    
    CADisplayLink* displayLink; // CA == Core Animation
    GLint fps; // Frame Per Seconds
    BOOL isAnimating;

    enum
    {
    	PRJ_ATRIBUTE_POSITION = 0,
    	PRJ_ATRIBUTE_COLOR,
    	PRJ_ATRIBUTE_NORMAL,
    	PRJ_ATRIBUTE_TEXTURE0
    };

	GLuint shaderProgramObject;

	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLuint projectionMatrixUniform;

	mat4 perspectiveProjectionMatrix;

	int day ;
	int year ;
	int moon ;

	
}
-(id)initWithFrame:(CGRect)frame
{
    // Code
    self = [super initWithFrame:frame];
    
    if(self)
    {
        // Step 1 : Create Drawable Surface Which is Called As Layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer*)[super layer]; // To Allow this statement to gate the succeesfull, you have to impliment a static ,method , know as layerClass.
        
        // Step 2 : Set EAGLLayer Properties
        [eaglLayer setOpaque:YES];
        NSNumber *boolNumber = [NSNumber numberWithBool:NO];
        NSDictionary* dictionary = [NSDictionary dictionaryWithObjectsAndKeys:boolNumber,kEAGLDrawablePropertyRetainedBacking,     kEAGLColorFormatRGBA8,kEAGLDrawablePropertyColorFormat,Nil];
        [eaglLayer setDrawableProperties:dictionary];
        
        // Setp 3 : Create EAGL Context
        eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if(eaglContext == Nil)
        {
            printf("Failed To Create EAGL Context \n");
            [self uninitialise];
            [self release];
            exit (0);
        }
        
        // Step 4 : Set This Context As the Current Context
        [EAGLContext setCurrentContext:eaglContext];
        
        // Step 5 : Print the OpenGLES Info
        printf("OpenGLES renderer = %s\n", glGetString(GL_RENDERER));
        printf("OpenGLES version = %s\n", glGetString(GL_VERSION));
        printf("glsl version = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        
        // Step 6 : Create FrameBuffer
        glGenFramebuffers(1, &defaultFrameBuffer);
        
        // Step 7 : BindWith Abobe Frame Buffer
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
        
        // Step 8 : Create Color render Buffer
        glGenRenderbuffers(1, &colorRenderBuffer);
        
        // Step 9 : Bind Above Color Render Buffer
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        // Step 10 : Provide Storage To Render Buffer
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)[self layer]];
        
        // Step 11 : Set FrameBuffer According to RenderBuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
        
        // Step 12 : Declare Backing Width
        GLint backingWidth;
        
        // Step 13 : Declare Backing Height
        GLint backingHeight;
        
        // Step 14 : Get ColorRender Buffer Width Into backingWidth
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH, &backingWidth);
        
        // Step 15 : Get ColorRender Buffer Height Into backingWidth
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT, &backingHeight);
        
        // Step 16 : Create Depth Render Buffer
        glGenRenderbuffers(1, &depthRenderBuffer);
        
        // Step 17 : Bind Above Depth Render Buffer
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        
        // Step 18 : Provide Storage To Depth Render Buffer
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight); // iPhone recommands to give depth of 16 bit
        
        // Step 19 : Set FrameBuffer According to RenderBuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        
        // Step 20 : Check Status of FrameBuffer
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Failed To Create Frame Buffer.\n");
            [self uninitialise];
            [self release];
            exit (0);
        }
        
        // Call Initialize
        int iResult = [self initialize];
        
        fps = 60; // This is Started From iOS 8.0
        isAnimating = NO;
        
        // Gesture recognizer code
        // Single Tap
        UITapGestureRecognizer* singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        // Double Tap
        UITapGestureRecognizer* doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        // Differentiate between single tap and double tap
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        // Swipe
        UISwipeGestureRecognizer *swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector (onSwipe:)];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        // LongPress
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector (onLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
        
    }
    return self;
}

+ (Class)layerClass
{
    // Code
    return [CAEAGLLayer class];
}

- (void)layoutSubviews
{
    // Code
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
    
    // Declare Backing Width
    GLint backingWidth;
    
    // Step 13 : Declare Backing Height
    GLint backingHeight;
    
    // Get ColorRender Buffer Width Into backingWidth
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH, &backingWidth);
    
    // Get ColorRender Buffer Height Into backingWidth
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    // Create Depth Render Buffer
    glGenRenderbuffers(1, &depthRenderBuffer);
    
    // Bind Above Depth Render Buffer
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    
    // Provide Storage To Depth Render Buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight); // iPhone recommands to give depth of 16 bit
    
    // Set FrameBuffer According to RenderBuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    
    // Call Our Resize Here
    [self resize:backingWidth :backingHeight];
    
    // Call DrawView After Resize
    [self drawView:Nil];
    
    // Check Status of FrameBuffer
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Failed To Create Frame Buffer.\n");
        [self uninitialise];
        [self release];
        exit (0);
    }
    
}

/*
- (void)drawRect:(CGRect)dirtyRect
{
    // Code
    [self drawView];
}
*/

- (void)drawView:(id)sender
{
    // Code
    // Set This Context As the Current Context
    [EAGLContext setCurrentContext:eaglContext];
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
    
    
    // Call Our Display & Update Here
    [self display];
    [self myupdate];
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)startAnimation
{
    // Code
    if(isAnimating == NO)
    {
        // Create Display Link
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        // Set FPS for the display Link
        [displayLink setPreferredFramesPerSecond:fps];
        // Add This Display Link TO Run Loop
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        isAnimating = YES;
    }
}

- (void)stopAnimation
{
    // Code
    if(isAnimating == YES)
    {
        // Remove Display Link from Run Loop
        [displayLink invalidate];
        
        isAnimating = NO;
    }
}

- (int)initialize
{
    // code
    
	// vartex Shader
	const GLchar *vertexShaderSourceCode =
		"#version 300 es"
		"\n"
		"in vec4 a_position;"
		"in vec4 a_color;"
		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"out vec4 a_color_out;"
		"void main(void)"
		"{"
		"a_color_out = a_color;"
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
				printf("VERTEX SHADER COMPILATION LOG : %s\n", log);
				[self uninitialise];
        [self release];
        exit (0);
			}
		}
	}

	// fragment Shader

	status = 0;
	infoLogLength = 0;

	const GLchar *fragmentShaderSourceCode =
		"#version 300 es"
		"\n"
		"precision highp float;"
		"in vec4 a_color_out;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"FragColor = a_color_out;"
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
				printf("FRAGMENT SHADER COMPILATION LOG : %s\n", log);
				[self uninitialise];
        [self release];
        exit (0);
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
				printf("SHADER PROGRAM LINK LOG: %s \n", log);
				[self uninitialise];
        [self release];
        exit (0);
			}
		}
	}

	// post link - getting
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");

	newSphere(fRadius,iSlices,iStacks,(&sphereVertices_new),(&sphereNormals_new),&iNoOfVertices,false,(&sphereElements_new));

	// vao and vbo related code
	// vao for Sphere
	glGenVertexArrays(1, &Vao_sphere);
	glBindVertexArray(Vao_sphere);

	// vbo for position
	glGenBuffers(1, &Vbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, 12 * iNoOfVertices, sphereVertices_new, GL_STATIC_DRAW);

	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Elements vbo
    glGenBuffers(1, &Vbo_sphere_element);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort) * iNoOfElements,sphereElements_new, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	// Depth Related Changes
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    perspectiveProjectionMatrix = mat4::identity();

	// Initializing Model View Matrix Stack
	initializeMatrixStack();

    return (0);
}

- (void)resize:(int)width :(int)height
{
    // Code
    if(height < 0)
        height = 1;
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}

- (void)display
{
    // Code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// variable declartions
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	mat4 rotationMatrix_Day;
	mat4 rotationMatrix_Year;

	vec3 eye = vec3(0.0f, 0.0f, 5.0f);
	vec3 center = vec3(0.0f, 0.0f, 0.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	// View Transformation
	viewMatrix = vmath::lookat(eye, center, up); // like gluLookAt()

	// save the Camera Matrix "Push"
	// glPushMatrix()
	pushMatrix(viewMatrix);

	// Parallel to radius 0.75 given in gluSphere()
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// gluSphere()
	glVertexAttrib3f(PRJ_ATRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);

	glBindVertexArray(Vao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element);
    glDrawElements(GL_TRIANGLE_STRIP,iNoOfElements,GL_UNSIGNED_SHORT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	viewMatrix = popMatrix();

	// Using the restored "popped" camera Matrix
	pushMatrix(viewMatrix); // Push Two

	rotationMatrix_Year = vmath::rotate((GLfloat)year, 0.0f, 1.0f, 0.0f);
	translationMatrix = vmath::translate(1.5f, 0.0f, 0.0f);

	rotationMatrix_Day = vmath::rotate((GLfloat)day, 0.0f, 1.0f, 0.0f);

	scaleMatrix = vmath::scale(0.3f, 0.3f, 0.3f);

	modelMatrix = rotationMatrix_Year * translationMatrix; // Revolution Arround Sun
	modelMatrix = modelMatrix * rotationMatrix_Day;		   // Rotation of Earth Around itsewlf

	// Pushing the Model Matrix of earth position when he revolve and rotate
	pushMatrix(modelMatrix);

	modelMatrix = modelMatrix * scaleMatrix; // Scale Earth to 0.2f;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// gluSphere()

	glVertexAttrib3f(PRJ_ATRIBUTE_COLOR, 0.4f, 0.9f, 1.0f);

	glBindVertexArray(Vao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element);
    glDrawElements(GL_TRIANGLE_STRIP,iNoOfElements,GL_UNSIGNED_SHORT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	// Moon
	modelMatrix = popMatrix();

	modelMatrix = modelMatrix * rotationMatrix_Day;

	translationMatrix = vmath::translate(0.5f, 0.0f, 0.0f);

	modelMatrix = modelMatrix * translationMatrix;

	scaleMatrix = vmath::scale(0.17f, 0.17f, 0.17f);

	modelMatrix = modelMatrix * scaleMatrix; // Scale Earth to 0.2f;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// gluSphere()
	glVertexAttrib3f(PRJ_ATRIBUTE_COLOR, 1.0f, 1.0f, 1.0f);

	glBindVertexArray(Vao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element);
    glDrawElements(GL_TRIANGLE_STRIP,iNoOfElements,GL_UNSIGNED_SHORT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	viewMatrix = popMatrix();

	// unuse the shader program object
	glUseProgram(0);
}

- (void) myupdate
{
    // Code
}

- (void) uninitialise
{
    // Code0
    unInitializeNewSphere();

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

    if(depthRenderBuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderBuffer);
        depthRenderBuffer = 0;
    }
    
    if(colorRenderBuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderBuffer);
        colorRenderBuffer = 0;
    }
    
    if(defaultFrameBuffer)
    {
        glDeleteFramebuffers(1, &defaultFrameBuffer);
        defaultFrameBuffer = 0;
    }
    
    if([EAGLContext currentContext] == eaglContext)
    {
        [EAGLContext setCurrentContext:Nil];
        [eaglContext release];
        eaglContext = Nil;
    }
}

-(BOOL)acceptsFirstResponder
{
    return YES;
}

-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    // Code
}

-(void)onSingleTap:(UITapGestureRecognizer*)tgr
{
    // Code
	day = (day + 3) % 360;
}

-(void)onDoubleTap:(UITapGestureRecognizer*)tgr
{
    // Code
	year = (year + 3) % 360;
}

-(void)onSwipe:(UISwipeGestureRecognizer*)stgr
{
    // Code
    [self  uninitialise];
    
    [self release];
    
    exit(0);
}

-(void)onLongPress:(UILongPressGestureRecognizer*)lptgr
{
    // Code
}

-(void)dealloc
{
    // Code
    [self uninitialise];
    
    if(displayLink)
    {
        [displayLink release];
        displayLink = Nil;
    }
    
    
    [super dealloc];
}

@end
