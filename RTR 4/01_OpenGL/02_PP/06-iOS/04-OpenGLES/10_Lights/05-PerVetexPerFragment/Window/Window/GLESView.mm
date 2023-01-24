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

	GLuint shaderProgramObject_PV;
	GLuint shaderProgramObject_PF;

	GLuint vao_Sphere;			 // Vertex Array Object
	GLuint vbo_Sphere_Position; // Vertex Buffer Object
	GLuint vbo_Sphere_Normal;
	GLuint vbo_sphere_element;
	GLuint vbo_sphere_element;

	GLuint modelMatrixUniform_PV;
	GLuint viewMatrixUniform_PV;
	GLuint projectionMatrixUniform_PV;

	GLuint laUniform_PV;		   // light Ambiant
	GLuint ldUniform_PV;		   // light Diffuse
	GLuint lsUniform_PV;		   // light Spec
	GLuint lighPositionUniform_PV; // light Position

	GLuint kaUniform_PV; // material amb
	GLuint kdUniform_PV; // mat diff
	GLuint ksUniform_PV; // mat specular
	GLuint materialShininessUniform_PV;

	GLuint lightingEnabledUniform_PV;

	GLuint modelMatrixUniform_PF;
	GLuint viewMatrixUniform_PF;
	GLuint projectionMatrixUniform_PF;

	GLuint laUniform_PF;		   // light Ambiant
	GLuint ldUniform_PF;		   // light Diffuse
	GLuint lsUniform_PF;		   // light Spec
	GLuint lighPositionUniform_PF; // light Position

	GLuint kaUniform_PF; // material amb
	GLuint kdUniform_PF; // mat diff
	GLuint ksUniform_PF; // mat specular
	GLuint materialShininessUniform_PF;

	GLuint lightingEnabledUniform_PF;

	mat4 perspectiveProjectionMatrix;

	BOOL bLight ;
	BOOL bPerFragShader;

	vmath::vec4 lightAmbiant ;
	vmath::vec4 lightDiffuse ;
	vmath::vec4 lightSpecular ;
	vmath::vec4 lightPositions;

	vmath::vec4 materialAmbiant ;
	vmath::vec4 meterialDeffuse ;
	vmath::vec4 materialSpecular;
	GLfloat materialShineeness;

	Sphere *sphere;
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
    
	// Per Vertex
	{
		// vartex Shader
		const GLchar *vertexShaderSourceCode_PV =
			"#version 300 es"
			"\n"
			"in vec4 a_position;"
			"in vec3 a_normal;"
			"uniform mat4 u_modelMatrix;"
			"uniform mat4 u_viewMatrix;"
			"uniform mat4 u_projectionMatrix;"
			"uniform vec3 u_la;"
			"uniform vec3 u_ld;"
			"uniform vec3 u_ls;"
			"uniform vec4 u_lightPosition;"
			"uniform vec3 u_ka;"
			"uniform vec3 u_ks;"
			"uniform vec3 u_kd;"
			"uniform float u_materialShininnes;"
			"uniform mediump int u_lightingEnabled;"
			"out vec3 phong_ads_out;"
			"void main(void)"
			"{"
			"if(u_lightingEnabled == 1)"
			"{"
			"vec3 ambiant = u_la * u_ka;"
			"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;"
			"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);"
			"vec3 transformedNormals = normalize(normalMatrix * a_normal);"
			"vec3 lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);" // Swizaling
			"vec3 diffuse = u_ld * u_kd * max(dot(lightDirection ,transformedNormals),0.0);"
			"vec3 reflectionVector = reflect(-lightDirection,transformedNormals);"
			"vec3 viewerVector = normalize(-eyeCoordinates.xyz);"
			"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector , viewerVector), 0.0), u_materialShininnes);"
			"phong_ads_out = ambiant + diffuse +  specular;"
			"}"
			"else"
			"{"
			"phong_ads_out = vec3(1.0,1.0,1.0);"
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

		const GLchar *fragmentShaderSourceCode_PV =
			"#version 300 es"
			"\n"
			"in vec3 phong_ads_out;"
			"out vec4 FragColor;"
			"void main(void)"
			"{"
			"FragColor = vec4(phong_ads_out, 1.0);"
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
		modelMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_modelMatrix");
		viewMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_viewMatrix");
		projectionMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_projectionMatrix");

		laUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_la");
		ldUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ld");
		lsUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ls");
		lighPositionUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_lightPosition");

		kaUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ka");
		kdUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_kd");
		ksUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ks");

		materialShininessUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_materialShininnes");

		lightingEnabledUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_lightingEnabled");
	}

	// Per Fragment
	{
		// vartex Shader
		const GLchar *vertexShaderSourceCode_PF =
			"#version 300 es"
			"\n"
			"in vec4 a_position;"
			"in vec3 a_normal;"
			"uniform mat4 u_modelMatrix;"
			"uniform mat4 u_viewMatrix;"
			"uniform mat4 u_projectionMatrix;"
			"uniform vec4 u_lightPosition;"
			"uniform mediump int u_lightingEnabled;"
			"out vec3 transformedNormals;"
			"out vec3 lightDirection;"
			"out vec3 viewerVector;"
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

		const GLchar *fragmentShaderSourceCode_PF =
			"#version 300 es"
			"\n"
			"in vec3 transformedNormals;"
			"in vec3 lightDirection;"
			"in vec3 viewerVector;"
			"uniform vec3 u_la;"
			"uniform vec3 u_ld;"
			"uniform vec3 u_ls;"
			"uniform vec3 u_ka;"
			"uniform vec3 u_ks;"
			"uniform vec3 u_kd;"
			"uniform float u_materialShininnes;"
			"uniform mediump int u_lightingEnabled;"
			"out vec4 FragColor;"
			"void main(void)"
			"{"
			"vec3 phong_ads_color;"
			"if(u_lightingEnabled == 1)"
			"{"
			"vec3 ambiant = u_la * u_ka;"
			"vec3 normalized_transformed_normals = normalize(transformedNormals);"
			"vec3 normalized_light_direction = normalize(lightDirection);"
			"vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction ,normalized_transformed_normals),0.0);"
			"vec3 reflectionVector = reflect(-normalized_light_direction,normalized_transformed_normals);"
			"vec3 normalized_view_vector = normalize(viewerVector);"
			"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector , normalized_view_vector), 0.0), u_materialShininnes);"
			"phong_ads_color = ambiant + diffuse +  specular;"
			"}"
			"else"
			"{"
			"phong_ads_color = vec3(1.0,1.0,1.0);"
			"}"
			"FragColor = vec4(phong_ads_color, 1.0);"
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
		modelMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_modelMatrix");
		viewMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_viewMatrix");
		projectionMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_projectionMatrix");

		laUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_la");
		ldUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ld");
		lsUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ls");
		lighPositionUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_lightPosition");

		kaUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ka");
		kdUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_kd");
		ksUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ks");

		materialShininessUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_materialShininnes");

		lightingEnabledUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_lightingEnabled");
	}

	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");

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

	// normal vbo
	glGenBuffers(1, &vbo_Sphere_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Sphere_Normal);
	glBufferData(GL_ARRAY_BUFFER, 12 * iNoOfVertices, sphereNormals_new, GL_STATIC_DRAW);

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

	lightAmbiant = vmath::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	lightDiffuse = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightSpecular = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightPositions = vmath::vec4(100.0f, 100.0f, 100.0f, 1.0f);

	materialAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	meterialDeffuse = vmath::vec4(0.5f, 0.2f, 0.7f, 1.0f);
	materialSpecular = vmath::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	materialShineeness = 120.0f;    

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
	if (bPerFragShader == FALSE)
	{
		glUseProgram(shaderProgramObject_PV);
	}
	else
	{
		glUseProgram(shaderProgramObject_PF);
	}
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); // glTranslatef() is replaced by this line

	modelMatrix = translationMatrix;
	if (bPerFragShader == FALSE)
	{
		glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, perspectiveProjectionMatrix);

		if (bLight == TRUE)
		{

			glUniform1i(lightingEnabledUniform_PV, 1);

			glUniform3fv(laUniform_PV, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
			glUniform3fv(ldUniform_PV, 1, lightDiffuse);
			glUniform3fv(lsUniform_PV, 1, lightSpecular);
			glUniform4fv(lighPositionUniform_PV, 1, lightPositions);

			glUniform3fv(kaUniform_PV, 1, materialAmbiant);
			glUniform3fv(kdUniform_PV, 1, meterialDeffuse);
			glUniform3fv(ksUniform_PV, 1, materialSpecular);
			glUniform1f(materialShininessUniform_PV, materialShineeness);
		}
		else
		{
			glUniform1i(lightingEnabledUniform_PV, 0);
		}
	}
	else
	{
		glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, perspectiveProjectionMatrix);

		if (bLight == TRUE)
		{

			glUniform1i(lightingEnabledUniform_PF, 1);

			glUniform3fv(laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
			glUniform3fv(ldUniform_PF, 1, lightDiffuse);
			glUniform3fv(lsUniform_PF, 1, lightSpecular);
			glUniform4fv(lighPositionUniform_PF, 1, lightPositions);

			glUniform3fv(kaUniform_PF, 1, materialAmbiant);
			glUniform3fv(kdUniform_PF, 1, meterialDeffuse);
			glUniform3fv(ksUniform_PF, 1, materialSpecular);
			glUniform1f(materialShininessUniform_PF, materialShineeness);
		}
		else
		{
			glUniform1i(lightingEnabledUniform_PF, 0);
		}
	}

	glBindVertexArray(vao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
    glDrawElements(GL_TRIANGLE_STRIP,iNoOfElements,GL_UNSIGNED_SHORT,0);

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

    // delete vbo_Square_Position
	
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
    if (bPerFragShader == FALSE)
	{
		bPerFragShader = TRUE;
	}
	else
	{
		bPerFragShader = FALSE;
	}
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
