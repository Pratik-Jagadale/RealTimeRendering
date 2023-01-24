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

	GLuint vao_Sphere;		  // Vertex Array Object - Sphere
	GLuint vbo_Sphere_Position; // Vertex Buffer Object - Sphere - Position
	GLuint vbo_Sphere_Normal; // Vertex Buffer Object - Sphere - Normal
    GLuint vbo_Sphere_Element;

	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLuint projectionMatrixUniform;

	mat4 perspectiveProjectionMatrix;

	GLuint ldUniform; // light direction
	GLuint kdUniform; // material diffuse
	GLuint lightPositionUniform;
	GLuint lightingEnbaledUniform;

	GLfloat lightDiffuse[4];
	GLfloat materialDiffuse[4];
	GLfloat lightPositions[4];
	
	BOOL bLight;

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
		"in vec3 a_normal;"
		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"uniform vec3 u_ld;"
		"uniform vec3 u_kd;"
		"uniform vec4 u_lightPosition;"
		"uniform mediump int u_lightEnabled;"
		"out vec3 diffuse_light_color;"
		"void main(void)"
		"{"
		"if(u_lightEnabled == 1)"
		"{"
		"vec4 eyeCoordinate = u_viewMatrix * u_modelMatrix * a_position;"
		"mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));"
		"vec3 transformedNormals = normalize(normalMatrix * a_normal);"
		"vec3 lightDirection = vec3(normalize(u_lightPosition - eyeCoordinate));"
		"diffuse_light_color = u_ld * u_kd * max(dot(lightDirection, transformedNormals), 0.0);"
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
		"in vec3 diffuse_light_color;"
		"uniform mediump int u_lightEnabled;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"if(u_lightEnabled == 1)"
		"{"
		"FragColor = vec4(diffuse_light_color,1.0);"
		"}"
		"else"
		"{"
		"FragColor = vec4(1.0,1.0,1.0,1.0);"
		"}"
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
	printf("%d\n",projectionMatrixUniform);

	ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
	kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");
	lightingEnbaledUniform = glGetUniformLocation(shaderProgramObject, "u_lightEnabled");

    newSphere(fRadius,iSlices,iStacks,(&sphereVertices_new),(&sphereNormals_new),&iNoOfVertices,false,(&sphereElements_new));

	// vao and vbo related code
	// vao for Sphere
	glGenVertexArrays(1, &vao_Sphere);
	glBindVertexArray(vao_Sphere);

	// vbo for position
	glGenBuffers(1, &vbo_Sphere_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Sphere_Position);
	glBufferData(GL_ARRAY_BUFFER, 12 * iNoOfVertices, sphereVertices_new, GL_STATIC_DRAW);
    glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for Normal
	glGenBuffers(1, &vbo_Sphere_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Sphere_Normal);
	glBufferData(GL_ARRAY_BUFFER, 12 * iNoOfVertices, sphereNormals_new, GL_STATIC_DRAW);
    glVertexAttribPointer(PRJ_ATRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Elements vbo
    glGenBuffers(1, &vbo_Sphere_Element);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Sphere_Element);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort) * iNoOfElements,sphereElements_new, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	// Depth Related Changes
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	lightDiffuse[0] = 1.0f;
	lightDiffuse[1] = 1.0f;
	lightDiffuse[2] = 1.0f;
	lightDiffuse[3] = 1.0f;
	
	materialDiffuse[0] = 0.5f;
	materialDiffuse[1] = 0.5f;
	materialDiffuse[2] = 0.5f;
	materialDiffuse[3] = 1.0f;

	lightPositions[0] = 0.0f;
	lightPositions[1] = 0.0f;
	lightPositions[2] = 10.0f;
	lightPositions[3] = 1.0f;

    perspectiveProjectionMatrix = mat4::identity();

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

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line

	modelMatrix = translationMatrix;
	
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnbaledUniform, 1);
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform4fv(lightPositionUniform, 1, lightPositions);
	}
	else
	{
		glUniform1i(lightingEnbaledUniform, 0);
	}

    glBindVertexArray(vao_Sphere);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Sphere_Element);
    glDrawElements(GL_TRIANGLE_STRIP,iNoOfElements,GL_UNSIGNED_SHORT,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);
}

- (void) myupdate
{
    // Code
}

- (void) uninitialise
{
    // Code
    unInitializeNewSphere();

	if (vbo_Sphere_Element)
	{
		glDeleteBuffers(1, &vbo_Sphere_Element);
		vbo_Sphere_Element = 0;
	}

	// deletion of vbo_Sphere_Position
	if (vbo_Sphere_Position)
	{
		glDeleteBuffers(1, &vbo_Sphere_Position);
		vbo_Sphere_Position = 0;
	}

	if (vbo_Sphere_Normal)
	{
		glDeleteBuffers(1, &vbo_Sphere_Position);
		vbo_Sphere_Position = 0;
	}

	// deletion of vao_Sphere
	if (vao_Sphere)
	{
		glDeleteVertexArrays(1, &vao_Sphere);
		vao_Sphere = 0;
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
    if (bLight == FALSE)
	{
		bLight = TRUE;
	}
	else
	{
		bLight = FALSE;
	}
}

-(void)onDoubleTap:(UITapGestureRecognizer*)tgr
{
    // Code
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
