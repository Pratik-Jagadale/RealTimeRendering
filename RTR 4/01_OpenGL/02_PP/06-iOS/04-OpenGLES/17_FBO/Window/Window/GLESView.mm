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
    
    printf("\nbackingWidth : %d , backingHeight : %d \n",backingWidth,backingHeight);

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
        printf("Failed To Create Frame Buffer in layoutSubviews.\n");
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
    [self initializeVariables];    

	// vartex Shader
	const GLchar *vertexShaderSourceCode =
		"#version 300 es"
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
				printf("VERTEX SHADER COMPILATION LOG : %s\n", log);
				free(log);
				log = NULL;
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
		"in vec2 a_texcoord_out;"
		"uniform highp sampler2D u_textureSampler;"
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
				printf("FRAGMENT SHADER COMPILATION LOG : %s\n", log);
				free(log);
				log = NULL;
				[self uninitialise];
                [self release];
                exit (0);
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
				printf("SHADER PROGRAM LINK LOG: %s \n", log);
				free(log);
				log = NULL;
				[self uninitialise];
                [self release];
                exit (0);
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
		printf("CREATE FBO FAILED...\n");
	}

    return (0);
}

- (int) initialize_sphere:(int)width :(int)height
{
	/* fucntion delcations */

	// vartex Shader
	const GLchar *vertexShaderSourceCode =
		"#version 300 es"
		"\n"
		"in vec4 a_position;"
		"in vec3 a_normal;"
		"uniform vec4 u_lightPosition[3];"
		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"uniform mediump int u_lightingEnabled;"
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
				printf("VERTEX SHADER COMPILATION LOG FOR SPHERE: %s\n", log);
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
		"#version 300 es"
		"\n"
		"precision highp float;"
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
		"uniform mediump int u_lightingEnabled;"
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
				printf("FRAGMENT SHADER COMPILATION LOG FOR SPHERE: %s\n", log);
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
				printf("SHADER PROGRAM LINK LOG FOR SPHER: %s \n", log);
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




	glClearDepthf(1.0f);
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

- (void)resize:(int)width :(int)height
{
    // Code
    if(height < 0)
        height = 1;
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    winWidth = width;
	winHeight = height;

    [self resize_sphere:width :height];

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
		printf("UnSufficient Render Buffer Size...\n");
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
		printf("FRAME BUFFER IS NOT COMPLETE...\n");
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

- (void)display
{
    // CODE
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Code */
	// if (bBFOResult == true)
	// {
	 	[self display_sphere:FBO_WIDTH :FBO_HEIGHT];
	 	[self update_sphere];
	// }

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
    // Code
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
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
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
    // Code
    unInitializeNewSphere();

    
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
